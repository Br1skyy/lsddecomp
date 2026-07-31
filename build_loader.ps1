param()

$ErrorActionPreference = "Stop"
$root   = $PSScriptRoot
$build  = Join-Path $root "build_loader"
$asmDir = Join-Path $root "asm/loader"
$incDir = Join-Path $root "include"

# Find toolchain - check PATH first, then common locations
$tool = "mipsel-none-elf-gcc"
$toolPath = (Get-Command "${tool}" -ErrorAction SilentlyContinue).Source
if (-not $toolPath) {
    $candidates = @(
        "C:\mipsel-none-elf\bin\mipsel-none-elf-gcc.exe",
        "/usr/local/mipsel-none-elf/bin/mipsel-none-elf-gcc",
        "/opt/mipsel-none-elf/bin/mipsel-none-elf-gcc"
    )
    foreach ($c in $candidates) { if (Test-Path $c) { $toolPath = $c; break } }
}
if (-not $toolPath) { throw "mipsel-none-elf-gcc not found in PATH or standard locations" }
$toolDir = Split-Path $toolPath -Parent

# build_loader reads disk/extract/extract.xml for the file listing
if (-not (Test-Path "$root\disk\extract\extract.xml")) {
    throw "no disk/extract/extract.xml - run: pwsh tools/extract.ps1 -Image <your-lsd.cue-or-bin>"
}
$cc      = Join-Path $toolDir "mipsel-none-elf-gcc"
$ld      = Join-Path $toolDir "mipsel-none-elf-ld"
$objcopy = Join-Path $toolDir "mipsel-none-elf-objcopy"
$nm      = Join-Path $toolDir "mipsel-none-elf-nm"

$asflags = @("-mips2","-msoft-float","-mno-gpopt","-G0","-I$incDir","-I$asmDir")
$ldflags = @("-nostdlib","-G0","--relax")

Write-Host "=== loader build ==="

# Clean + create dirs
Remove-Item -Recurse -Force "$build\asm","$build\ps-exe","$build\*.bin","$build\*.cue" -ErrorAction SilentlyContinue
foreach ($d in @("$build\asm\loader","$build\ps-exe")) {
    New-Item -ItemType Directory -Path $d -Force | Out-Null
}

# [1] Assemble loader.s
Write-Host "[1/3] assemble loader.s..."
$savedEAP = $ErrorActionPreference; $ErrorActionPreference = "Continue"
$asmOut = & $cc -c @asflags -x assembler-with-cpp "$asmDir\loader.s" -o "$build\asm\loader\loader.s.o" 2>&1
$ec = $LASTEXITCODE; $ErrorActionPreference = $savedEAP
if ($ec -ne 0) { Write-Host $asmOut; throw "loader.s FAILED" }

# [2] Link with loader.ld
Write-Host "[2/3] link..."
Push-Location $root
$savedEAP = $ErrorActionPreference; $ErrorActionPreference = "Continue"
$linkOut = & $ld @ldflags -T "asm/loader/loader.ld" -Map "$build\ps-exe\loader.map" -o "$build\ps-exe\loader.elf" "build_loader/asm/loader/loader.s.o" 2>&1
$ec = $LASTEXITCODE; $ErrorActionPreference = $savedEAP
Pop-Location
Write-Host $linkOut
if ($ec -ne 0) { throw "Link FAILED" }

# [3] Convert code to binary, then prepend a synthesized 2048-byte PS-EXE header
Write-Host "[3/3] ps-exe..."
& $objcopy -O binary "$build/ps-exe/loader.elf" "$build/ps-exe/loader_code.bin" 2>&1
if ($LASTEXITCODE -ne 0) { throw "objcopy FAILED" }

# Synthesize the 2048-byte PS-EXE header (mirrors asm/loader/loader.s .header section)
$hdr = New-Object byte[] 2048
$hdrBytes = [System.Text.Encoding]::ASCII.GetBytes("PS-X EXE")
[Array]::Copy($hdrBytes, 0, $hdr, 0, 8)
# 0x10: initial PC = 0x800C0000 (loader_start)
$pc = 0x800C0000
$hdr[0x10] = $pc -band 0xFF; $hdr[0x11] = ($pc -shr 8) -band 0xFF
$hdr[0x12] = ($pc -shr 16) -band 0xFF; $hdr[0x13] = ($pc -shr 24) -band 0xFF
# 0x18: load address = 0x800C0000
$hdr[0x18] = $pc -band 0xFF; $hdr[0x19] = ($pc -shr 8) -band 0xFF
$hdr[0x1A] = ($pc -shr 16) -band 0xFF; $hdr[0x1B] = ($pc -shr 24) -band 0xFF
# 0x30: initial sp = 0x801FFFF0
$sp = 0x801FFFF0
$hdr[0x30] = $sp -band 0xFF; $hdr[0x31] = ($sp -shr 8) -band 0xFF
$hdr[0x32] = ($sp -shr 16) -band 0xFF; $hdr[0x33] = ($sp -shr 24) -band 0xFF
# Sony copyright filler
$sony = [System.Text.Encoding]::ASCII.GetBytes("Sony Computer Entertainment Inc. for Japan area")
[Array]::Copy($sony, 0, $hdr, 0x80, $sony.Length)

$codeBytes = [System.IO.File]::ReadAllBytes("$build/ps-exe/loader_code.bin")
$fileSize = $codeBytes.Length
$hdr[0x1C] = $fileSize -band 0xFF; $hdr[0x1D] = ($fileSize -shr 8) -band 0xFF
$hdr[0x1E] = ($fileSize -shr 16) -band 0xFF; $hdr[0x1F] = ($fileSize -shr 24) -band 0xFF

# Set t_addr to 0x800027C0 (actual BIOS load address)
$t_addr = 0x800027C0
$hdr[0x18] = $t_addr -band 0xFF
$hdr[0x19] = ($t_addr -shr 8) -band 0xFF
$hdr[0x1A] = ($t_addr -shr 16) -band 0xFF
$hdr[0x1B] = ($t_addr -shr 24) -band 0xFF

# Set pc0 to 0x800027C0 (entry point at actual load address)
$pc0 = 0x800027C0
$hdr[0x10] = $pc0 -band 0xFF
$hdr[0x11] = ($pc0 -shr 8) -band 0xFF
$hdr[0x12] = ($pc0 -shr 16) -band 0xFF
$hdr[0x13] = ($pc0 -shr 24) -band 0xFF

$exeBytes = New-Object byte[] (2048 + $codeBytes.Length)
[Array]::Copy($hdr, 0, $exeBytes, 0, 2048)
[Array]::Copy($codeBytes, 0, $exeBytes, 2048, $codeBytes.Length)

# Pad to original file size (522,240 bytes) to match BIOS t_size expectation
$targetSize = 522240
if ($exeBytes.Length -lt $targetSize) {
    $paddedBytes = New-Object byte[] $targetSize
    [Array]::Copy($exeBytes, 0, $paddedBytes, 0, $exeBytes.Length)
    $exeBytes = $paddedBytes
}

# Update t_size in header to match padded size (522,240 - 2048 = 520,192)
# BIOS reads t_size bytes from the file, so t_size must match the total file size minus header
$paddedTextSize = $targetSize - 2048
$exeBytes[0x1C] = $paddedTextSize -band 0xFF
$exeBytes[0x1D] = ($paddedTextSize -shr 8) -band 0xFF
$exeBytes[0x1E] = ($paddedTextSize -shr 16) -band 0xFF
$exeBytes[0x1F] = ($paddedTextSize -shr 24) -band 0xFF

[System.IO.File]::WriteAllBytes("$build/ps-exe/SLPS_015.56", $exeBytes)
Write-Host "  Loader EXE: $((Get-Item "$build/ps-exe/SLPS_015.56").Length) bytes (padded to $targetSize bytes, t_size=$paddedTextSize)" -ForegroundColor Green

# ---- Stage-2 payload (GAME.BIN) = full payload EXE from the main build ----
$payload = "$root\build_ps1\ps-exe\SLPS_015.56"
if (-not (Test-Path $payload)) {
    $payload = "$root\disk\extract\SLPS_015.56"
}
if (-not (Test-Path $payload)) {
    throw "Payload EXE not found (expected build_ps1\ps-exe\SLPS_015.56 or disk\extract\SLPS_015.56) -- run build_ps1.ps1 first"
}
Copy-Item $payload "$build\ps-exe\GAME.BIN" -Force
Write-Host "  GAME.BIN (payload): $((Get-Item "$build\ps-exe\GAME.BIN").Length) bytes" -ForegroundColor Green

# ---- Copy bootloader files to disk/extract for mkpsxiso compatibility ----
Copy-Item "$build\ps-exe\SLPS_015.56" "$root\disk\extract\SLPS_015.56" -Force
Copy-Item "$build\ps-exe\GAME.BIN" "$root\disk\extract\GAME.BIN" -Force
Write-Host "  Copied bootloader files to disk/extract for ISO build" -ForegroundColor Green

# ---- Use extract.xml which has full file listing ----
Write-Host "Using extract.xml with full file listing..." -ForegroundColor Yellow

# ---- Build ISO with mkpsxiso ----
$mkp = Join-Path $root "tools/mkpsxiso.exe"
if (-not (Test-Path $mkp)) {
    $mkp = (Get-Command "mkpsxiso" -ErrorAction SilentlyContinue).Source
}
if (-not $mkp) { throw "mkpsxiso not found at tools/mkpsxiso or in PATH" }

Write-Host "--- mkpsxiso: Building loader ISO ---" -ForegroundColor Yellow
if (Test-Path "$root\disk\extract\lsddecomp.bin") { Remove-Item "$root\disk\extract\lsddecomp.bin" -Force -ErrorAction SilentlyContinue }
if (Test-Path "$root\disk\extract\lsddecomp.cue") { Remove-Item "$root\disk\extract\lsddecomp.cue" -Force -ErrorAction SilentlyContinue }
Push-Location "$root\disk\extract"
$isoOut = & $mkp -y "$root\disk\extract\extract.xml" 2>&1
Pop-Location
# Copy generated ISO to build_loader directory
Copy-Item "$root\disk\extract\lsddecomp.bin" "$build\lsddecomp.bin" -Force
Copy-Item "$root\disk\extract\lsddecomp.cue" "$build\lsddecomp.cue" -Force
if ($LASTEXITCODE -ne 0) {
    Write-Host "mkpsxiso FAILED" -ForegroundColor Red
    Write-Host $isoOut
    exit 1
}
Write-Host $isoOut

Write-Host ""
Write-Host "=== done ==="
Write-Host ("Boot EXE (loader): $build\ps-exe\SLPS_015.56  ($((Get-Item "$build\ps-exe\SLPS_015.56").Length) bytes)")
Write-Host ("GAME.BIN (payload): $build\ps-exe\GAME.BIN  ($((Get-Item "$build\ps-exe\GAME.BIN").Length) bytes)")
Write-Host ("ISO:  $build\lsddecomp.bin  ($((Get-Item "$build\lsddecomp.bin").Length) bytes)")
Write-Host ("CUE:  $build\lsddecomp.cue")
