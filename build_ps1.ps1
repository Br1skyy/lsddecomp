param([switch]$IgnoreUnresolved)

$ErrorActionPreference = "Stop"
$root   = $PSScriptRoot
$build  = Join-Path $root "build_ps1"
$asmDir = Join-Path $root "asm/lsdde"
$srcDir = Join-Path $root "src/lsdde"
$incDir = Join-Path $root "include"
$psyqInc = Join-Path $root "psyq/include"
$psyqLib = Join-Path $root "psyq/lib"

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

# need the extracted disc for the ISO
if (-not (Test-Path "$root\disk\extract\extract.xml")) {
    throw "no disk/extract/extract.xml - run: pwsh tools/extract.ps1 -Image <your-lsd.cue-or-bin>"
}
$cc      = Join-Path $toolDir "mipsel-none-elf-gcc"
$ld      = Join-Path $toolDir "mipsel-none-elf-ld"
$objcopy = Join-Path $toolDir "mipsel-none-elf-objcopy"
$nm      = Join-Path $toolDir "mipsel-none-elf-nm"

$cflags  = @("-std=gnu99","-mips1","-msoft-float","-mno-gpopt","-G0","-fno-builtin",
             "-Os","-g","-fno-merge-constants","-fpermissive","-mlong-calls",
             "-Wno-implicit-function-declaration","-Wno-int-conversion",
             "-Wno-pointer-to-int-cast","-Wno-int-to-pointer-cast",
             "-Wno-incompatible-pointer-types","-I$incDir","-I$psyqInc","-DPSX")
$asflags = @("-mips1","-msoft-float","-mno-gpopt","-G0","-I$incDir","-I$asmDir")
$ldflags = @("-nostdlib","-L$psyqLib","-G0","--allow-multiple-definition","--unresolved-symbols=ignore-all","--relax")

Write-Host "=== build ==="

# Clean + create dirs
Remove-Item -Recurse -Force "$build\asm","$build\src","$build\ps-exe" -ErrorAction SilentlyContinue
foreach ($d in @("$build\asm\lsdde\data","$build\src\lsdde","$build\ps-exe")) {
    New-Item -ItemType Directory -Path $d -Force | Out-Null
}

# Placeholder LBA table (no longer used -- single EXE, no GAME.BIN split)
$lbaInc = "$incDir\lba_table.inc"
".ifndef LBA_TABLE_INC
.equ LBA_GAME_BIN, 275644
.equ GAME_BIN_SECTORS, 87
.endif
" | Set-Content $lbaInc
# Pre-generate C header for first pass
$lbaHdr = "#ifndef LBA_TABLE_H
#define LBA_TABLE_H
#define LBA_GAME_BIN 275644
#define GAME_BIN_SECTORS 87
#endif
"
$lbaHdr | Set-Content "$incDir\lba_table.h"

# [1] header
Write-Host "[1/4] header..."
$savedEAP = $ErrorActionPreference; $ErrorActionPreference = "Continue"
& $cc -c @asflags -x assembler-with-cpp "$asmDir\header.s" -o "$build\asm\lsdde\header.s.o" 2>&1 | Out-Null
$ec = $LASTEXITCODE; $ErrorActionPreference = $savedEAP
if ($ec -ne 0) { throw "header.s FAILED" }

# [2] root ASM
Write-Host "[2/4] asm files..."
$asmFiles = Get-ChildItem "$asmDir\*.s" | Where-Object { $_.Name -ne "header.s" }
$ok=0; $fail=0
foreach ($f in $asmFiles) {
    $savedEAP = $ErrorActionPreference; $ErrorActionPreference = "Continue"
    $out = & $cc -c @asflags -x assembler-with-cpp $f.FullName -o "$build\asm\lsdde\$($f.Name).o" 2>&1
    $ec = $LASTEXITCODE; $ErrorActionPreference = $savedEAP
    if ($ec -eq 0) { $ok++ } else { $fail++; Write-Host "  FAIL: $($f.Name)" -ForegroundColor Red }
}
Write-Host "  ASM: $ok OK, $fail FAIL"
# [3] data ASM
$dok=0; $dfail=0
foreach ($f in (Get-ChildItem "$asmDir\data\*.s")) {
    $savedEAP = $ErrorActionPreference; $ErrorActionPreference = "Continue"
    $out = & $cc -c @asflags -x assembler-with-cpp $f.FullName -o "$build\asm\lsdde\data\$($f.Name).o" 2>&1
    $ec = $LASTEXITCODE; $ErrorActionPreference = $savedEAP
    if ($ec -eq 0) { $dok++ } else { $dfail++; Write-Host "  FAIL: data/$($f.Name)" -ForegroundColor Red }
}
Write-Host "  DATA: $dok OK, $dfail FAIL"

# [4] C sources (exclude stubs.c -- its invented names are handled by --defsym)
Write-Host "[3/4] c sources..."
$otherCFiles = @("System.c","Sound.c",
                 "SpuInit.c","GameLoop.c","StageGrid2.c",
                 "Entity2.c","Other.c",
                 "Entity_Spu.c","NavMenu_TextInput.c","NavMenu_Spu.c",
                 "Entity3_behavior.c","Entity3_CdMode.c","Entity3_core.c",
                 "Entity3_PadManager.c","Entity3_behavior_auto.c",
                 "cdrom_handler.c",
                 "DreamSys_nonmatchings.c",
                 "Rendering_Stage.c","Rendering_World.c","Rendering_Chunk.c",
                 "Rendering_NavMemCard.c","Rendering_UI.c",
                 "Rendering_RenderUtils.c","Rendering_Entity.c")
# Files with known compilation issues; ASM .s provides the implementations.
# Merged/deleted: stubs.c -> GameLoop.c, 5CAB4.c -> DreamSys.c,
# class_16334.c -> Entity3_PadManager.c (dup), cd_stage2.c -> cdrom_handler.c
$skipCFiles = @("psyq_15d04.c","psyq_GsLinkObject4.c","psyq_PadInit.c","psyq_rand.c","psyq_rcpolyf3.c","psyq_rcpolyf4.c","psyq_rcpolyft3.c","psyq_rcpolyg3.c","Rendering.c","Entity3_vtable.c","2C054.c","5CAB4.c","class_16334.c","cd_stage2.c")
$cok=0; $cfail=0; $cskip=0
foreach ($f in Get-ChildItem "$srcDir\*.c") {
    if ($f.Name -eq "globals.c") { continue }
    if ($f.Name -in $skipCFiles) { $cskip++; Write-Host "  SKIP: $($f.Name) (using ASM)" -ForegroundColor Yellow; continue }
    $saveEA=$ErrorActionPreference; $ErrorActionPreference="Continue"
    $compileOut = & $cc -c @cflags $f.FullName -o "$build\src\lsdde\$($f.Name).o" 2>&1
    if ($LASTEXITCODE -eq 0) { $cok++ } else { $cfail++; Write-Host "  FAIL: $($f.Name)" -ForegroundColor Red; Write-Host $compileOut -ForegroundColor Yellow }
    $ErrorActionPreference=$saveEA
}
Write-Host "  C: $cok OK, $cfail FAIL, $cskip SKIP"
if ($cfail -gt 0) { throw "C compilation failures" }

# [5] Link
Write-Host "[4/4] link..."
$ldScript = Get-Content "$root\lsdde.ld" -Raw
# C files' .rodata/.text/.data/.bss are caught by the wildcards at the end of each
# output section (*(.rodata), *(.text) *(.text.*), *(.data) *(.data.*), etc.)
# Do NOT insert them here explicitly -- that would push fixed-offset data/asm files
# to wrong addresses and break hardcoded pointers within those files.
$ldScript = $ldScript -replace 'build/', 'build_ps1/'

$ldScript | Set-Content "$build\lsdde_updated.ld"

$objs = @()
$objs += "build_ps1/asm/lsdde/header.s.o"
Get-ChildItem "$asmDir\*.s" | Where-Object { $_.Name -ne "header.s" } | ForEach-Object { $objs += "build_ps1/asm/lsdde/$($_.Name).o" }
Get-ChildItem "$asmDir\data\*.s" | ForEach-Object { $objs += "build_ps1/asm/lsdde/data/$($_.Name).o" }
$objs += "build_ps1/src/lsdde/DreamSys.c.o"
foreach ($cf in $otherCFiles) {
    if ($cf -notin $skipCFiles) { $objs += "build_ps1/src/lsdde/$cf.o" }
    else { Write-Host "  SKIP LINK: $cf.o (using ASM)" -ForegroundColor Yellow }
}

$libs = @("api","c","c2","card","cd","comb","ds","etc","gpu","gs","gte","gun","hmd","math","mcrd","mcx","pad","press","sio","sn","snd","spu","tap")
$libFlags = $libs | ForEach-Object { "-l$_" }

Write-Host "  Linking..." -ForegroundColor Yellow
Push-Location $root
$saveEA=$ErrorActionPreference; $ErrorActionPreference="Continue"
$linkArgs = @()
$linkArgs += $ldflags
$linkArgs += "-T","build_ps1/lsdde_updated.ld","-Map","build_ps1/ps-exe/lsddecomp.map","-o","build_ps1/ps-exe/lsddecomp.elf"
$linkArgs += $objs
# NOTE: --defsym files have been replaced by asm/lsdde/override.s
# PSYQ libraries provide SquareRoot0, ApplyMatrixLV, GsLinkObject4, etc.
$linkArgs += "--start-group"
$linkArgs += $libFlags
$linkArgs += "--end-group"
$linkOut = & $ld $linkArgs 2>&1 | Out-String
$ec=$LASTEXITCODE; $ErrorActionPreference=$saveEA
Pop-Location
Write-Host $linkOut
if ($ec -ne 0) { Write-Host "Link FAILED" -ForegroundColor Red; exit 1 }

Write-Host "Converting to PS-EXE..."
& $objcopy -O binary "$build/ps-exe/lsddecomp.elf" "$build/ps-exe/SLPS_015.56" 2>&1
if ($LASTEXITCODE -ne 0) { throw "objcopy FAILED" }

Write-Host "Patching PS-EXE header with correct values..."
$exeBytes = [System.IO.File]::ReadAllBytes("$build/ps-exe/SLPS_015.56")

# Find _start entry point address from ELF nm output
$nmOut = & $nm "$build/ps-exe/lsddecomp.elf" 2>&1
# Collect ALL _start symbols (libraries may provide their own)
$startCandidates = New-Object System.Collections.ArrayList
foreach ($line in $nmOut) {
    if ($line -match '^([0-9a-fA-F]+)\s+[Tt]\s+_start$') {
        $addr = [Convert]::ToUInt32($Matches[1], 16)
        $startCandidates.Add($addr) | Out-Null
    }
}
if ($startCandidates.Count -eq 0) { throw "Could not find _start symbol in ELF" }
# Pick the one that starts with 'lui sp, 0x801F' (0x3C1D801F) - our real _start
$startVal = $null
foreach ($candidate in $startCandidates) {
    $fileOff = $candidate - 0x80010000 + 0x800
    if ($fileOff -ge 0 -and $fileOff -lt $exeBytes.Length - 4) {
        $instr = [bitconverter]::ToUInt32($exeBytes, $fileOff)
        if ($instr -eq 0x3C1D801F) { $startVal = $candidate; break }
    }
}
# Fallback: use the last (highest address) candidate - PsyQ lib _start is low
if (-not $startVal) { $startVal = $startCandidates[$startCandidates.Count - 1] }
Write-Host "  _start = 0x$($startVal.ToString('X8'))"

# PS-EXE header layout (standard Sony PlayStation format):
#   0x00: "PS-X EXE" (8 bytes)
#   0x08: Reserved (8 bytes, zero)
#   0x10: Initial PC / entry point (4 bytes) -- patched to _start
#   0x14: Initial GP / r28 (4 bytes)
#   0x18: Load address / destination in RAM (4 bytes) = 0x80010000
#   0x1C: File size excluding header (4 bytes) -- patched
#   0x20: Reserved (4 bytes)
#   0x24: Reserved (4 bytes)
#   0x30: SP base (4 bytes)
#   0x34: SP offset (4 bytes)

# Patch _start as initial PC at standard offset 0x10
$exeBytes[0x10] = $startVal -band 0xFF
$exeBytes[0x11] = ($startVal -shr 8) -band 0xFF
$exeBytes[0x12] = ($startVal -shr 16) -band 0xFF
$exeBytes[0x13] = ($startVal -shr 24) -band 0xFF

# Compute file size = total binary size - 2048 (header)
$fileSize = $exeBytes.Length - 2048
Write-Host "  file size = 0x$($fileSize.ToString('X8')) ($fileSize bytes)"

# Patch file size at standard offset 0x1C (little-endian)
$exeBytes[0x1C] = $fileSize -band 0xFF
$exeBytes[0x1D] = ($fileSize -shr 8) -band 0xFF
$exeBytes[0x1E] = ($fileSize -shr 16) -band 0xFF
$exeBytes[0x1F] = ($fileSize -shr 24) -band 0xFF

[System.IO.File]::WriteAllBytes("$build/ps-exe/SLPS_015.56", $exeBytes)

# ---- Full-payload PS-EXE (no GAME.BIN split) ----
# The PS-EXE embeds the complete image (DREAMSYS_METHODS, callback tables,
# libcd data, ...). The console BIOS loads it entirely at boot; CdLoadStage2's
# skip-check (src/lsdde/cdrom_handler.c) then sees the preloaded data and skips
# CD operations, so no separate GAME.BIN / ZZGAME.BIN file is needed on the disc.
$exePath = "$build/ps-exe/SLPS_015.56"

$exeSize = (Get-Item $exePath).Length
Write-Host ""
Write-Host "EXE file size: $exeSize bytes ($($exeSize / 2048) sectors)"
$mkp = Join-Path $root "tools/mkpsxiso.exe"
if (-not (Test-Path $mkp)) {
    $mkp = (Get-Command "mkpsxiso" -ErrorAction SilentlyContinue).Source
}
if (-not $mkp) { throw "mkpsxiso not found at tools/mkpsxiso or in PATH" }

# Copy new EXE to extract dir so mkpsxiso includes it in the ISO
    Write-Host "Copying files to disk/extract/..." -ForegroundColor Yellow
    Copy-Item "$build\ps-exe\SLPS_015.56" "$root\disk\extract\SLPS_015.56" -Force
    # GAME.BIN is now embedded in the PS-EXE; remove any stale disc copy
    if (Test-Path "$root\disk\extract\CDI\ZZGAME.BIN") {
        Remove-Item "$root\disk\extract\CDI\ZZGAME.BIN" -Force
        Write-Host "  Removed stale disk/extract/CDI/ZZGAME.BIN" -ForegroundColor Yellow
    }

# check the exe is sane before iso build
$exeGuard = "$build\ps-exe\SLPS_015.56"
if (-not (Test-Path $exeGuard)) { Write-Error "EXE missing at $exeGuard - aborting ISO build"; exit 1 }
$exeGuardSize = (Get-Item $exeGuard).Length
if ($exeGuardSize -le 2048) { Write-Error "EXE at $exeGuard is too small ($exeGuardSize bytes) - aborting ISO build"; exit 1 }
$exeGuardBytes = [System.IO.File]::ReadAllBytes($exeGuard)
$exeGuardMagic = [System.Text.Encoding]::ASCII.GetString($exeGuardBytes, 0, 8)
if ($exeGuardMagic -ne "PS-X EXE") { Write-Error "EXE at $exeGuard has bad magic ('$exeGuardMagic') - aborting ISO build"; exit 1 }
Write-Host "  Guard OK: $exeGuard ($exeGuardSize bytes, magic='$exeGuardMagic')" -ForegroundColor Green

# Build the final ISO using mkpsxiso (creates proper ISO9660 filesystem)
Write-Host "--- mkpsxiso: Building ISO (first pass) ---" -ForegroundColor Yellow
if (Test-Path "$build\lsddecomp.bin") { Remove-Item "$build\lsddecomp.bin" -Force -ErrorAction SilentlyContinue }
if (Test-Path "$build\lsddecomp.cue") { Remove-Item "$build\lsddecomp.cue" -Force -ErrorAction SilentlyContinue }
Push-Location $build; $isoOut = & $mkp -y -lba "$build\lbas.txt" "$root\config\disk.xml" 2>&1; Pop-Location
if ($LASTEXITCODE -ne 0) {
    Write-Host "mkpsxiso FAILED" -ForegroundColor Red
    Write-Host $isoOut
    exit 1
}
Write-Host $isoOut

# Parse LBA table and generate lba_table.inc
$lbasPath = "$build\lbas.txt"
if (Test-Path $lbasPath) {
    $lbaLines = Get-Content $lbasPath
    $gameLba = 0; $gameSectors = 0; $exeLba = 0; $exeSectors = 0
    foreach ($ln in $lbaLines) {
        if ($ln -match 'ZZGAME\.BIN\s+\|\s*(\d+)\s+\|\s*(\d+)') {
            $gameSectors = [int]$Matches[1]
            $gameLba = [int]$Matches[2]
        }
        if ($ln -match 'SLPS_015\.56\s+\|\s*(\d+)\s+\|\s*(\d+)') {
            $exeSectors = [int]$Matches[1]
            $exeLba = [int]$Matches[2]
        }
    }
    if ($gameLba -gt 0 -or $exeLba -gt 0) {
        if ($gameLba -gt 0) {
            Write-Host "  GAME.BIN on CD: LBA=$gameLba sectors=$gameSectors" -ForegroundColor Yellow
        }
        Write-Host "  SLPS_015.56 on CD: LBA=$exeLba sectors=$exeSectors" -ForegroundColor Yellow
        $lbaContent = ".ifndef LBA_TABLE_INC
.equ LBA_GAME_BIN, $gameLba
.equ GAME_BIN_SECTORS, $gameSectors
.equ LBA_SLPS, $exeLba
.equ SLPS_SECTORS, $exeSectors
.endif
"
        $lbaContent | Set-Content "$incDir\lba_table.inc"
        $lbaHdr = "#ifndef LBA_TABLE_H
#define LBA_TABLE_H
#define LBA_GAME_BIN $gameLba
#define GAME_BIN_SECTORS $gameSectors
#define LBA_SLPS $exeLba
#define SLPS_SECTORS $exeSectors
#endif
"
        $lbaHdr | Set-Content "$incDir\lba_table.h"
        Write-Host "  Generated $incDir\lba_table.inc + lba_table.h" -ForegroundColor Yellow
    } else {
        Write-Host "  WARNING: Could not parse LBA table" -ForegroundColor Red
    }
} else {
    Write-Host "  WARNING: $lbasPath not found" -ForegroundColor Red
}

# need the exe LBA for the second pass (no GAME.BIN on disc anymore)
if ((-not $exeLba) -or $exeLba -eq 0) {
    Write-Error "Could not determine SLPS_015.56 LBA from mkpsxiso output - aborting"
    exit 1
}

# ---- second pass: rebuild with correct LBA ----
Write-Host ""
Write-Host "=== second pass ==="
    # Re-assemble files that include lba_table.inc: main.s, cd_stage2.s
    # Also re-compile System.c for updated LBA constants
    $srcPath = "$srcDir\System.c"
    $outPath = "$build\src\lsdde\System.c.o"
    Write-Host "  Re-compiling System.c for updated LBA constants ..." -ForegroundColor Yellow
    $savedEAP = $ErrorActionPreference; $ErrorActionPreference = "Continue"
    $out = & $cc -c @cflags -MMD -MP "$srcPath" -o $outPath 2>&1
    $ec = $LASTEXITCODE; $ErrorActionPreference = $savedEAP
    if ($ec -ne 0) { Write-Host $out; Write-Host "  FAILED to re-compile System.c" -ForegroundColor Red; exit 1 }
    # Re-assemble files that include lba_table.inc: main.s, cd_stage2.s
    foreach ($asmFile in @("main.s","cd_stage2.s")) {
        $srcPath = "$asmDir\$asmFile"
        $outPath = "$build\asm\lsdde\$asmFile.o"
        Write-Host "  Re-assembling $asmFile ..." -ForegroundColor Yellow
        $savedEAP = $ErrorActionPreference; $ErrorActionPreference = "Continue"
        $out = & $cc -c @asflags -x assembler-with-cpp $srcPath -o $outPath 2>&1
        $ec = $LASTEXITCODE; $ErrorActionPreference = $savedEAP
        if ($ec -ne 0) {
            Write-Host "  FAILED to re-assemble $asmFile" -ForegroundColor Red
            Write-Host $out
            exit 1
        }
    }
    # Re-link
    Write-Host "  Re-linking ..." -ForegroundColor Yellow
    Push-Location $root
    $saveEA=$ErrorActionPreference; $ErrorActionPreference="Continue"
    $linkOut = & $ld $linkArgs 2>&1 | Out-String
    $ec=$LASTEXITCODE; $ErrorActionPreference=$saveEA
    Pop-Location
    Write-Host $linkOut
    if ($ec -ne 0) { Write-Host "Link FAILED" -ForegroundColor Red; exit 1 }

    # Convert to PS-EXE again and patch (full payload, no GAME.BIN split)
    & $objcopy -O binary "$build/ps-exe/lsddecomp.elf" "$build/ps-exe/SLPS_015.56" 2>&1
    $exeBytes2 = [System.IO.File]::ReadAllBytes("$build/ps-exe/SLPS_015.56")
    $nmOut2 = & $nm "$build/ps-exe/lsddecomp.elf" 2>&1
    $startCandidates2 = New-Object System.Collections.ArrayList
    foreach ($line in $nmOut2) {
        if ($line -match '^([0-9a-fA-F]+)\s+[Tt]\s+_start$') {
            $addr = [Convert]::ToUInt32($Matches[1], 16)
            $startCandidates2.Add($addr) | Out-Null
        }
    }
    $startVal2 = $null
    foreach ($candidate in $startCandidates2) {
        $fileOff2 = $candidate - 0x80010000 + 0x800
        if ($fileOff2 -ge 0 -and $fileOff2 -lt $exeBytes2.Length - 4) {
            $instr2 = [bitconverter]::ToUInt32($exeBytes2, $fileOff2)
            if ($instr2 -eq 0x3C1D801F) { $startVal2 = $candidate; break }
        }
    }
    if (-not $startVal2) { $startVal2 = $startCandidates2[$startCandidates2.Count - 1] }
    Write-Host "  _start2 = 0x$($startVal2.ToString('X8'))"
    $exeBytes2[0x10] = $startVal2 -band 0xFF
    $exeBytes2[0x11] = ($startVal2 -shr 8) -band 0xFF
    $exeBytes2[0x12] = ($startVal2 -shr 16) -band 0xFF
    $exeBytes2[0x13] = ($startVal2 -shr 24) -band 0xFF
    $fileSize2 = $exeBytes2.Length - 2048
    $exeBytes2[0x1C] = $fileSize2 -band 0xFF
    $exeBytes2[0x1D] = ($fileSize2 -shr 8) -band 0xFF
    $exeBytes2[0x1E] = ($fileSize2 -shr 16) -band 0xFF
    $exeBytes2[0x1F] = ($fileSize2 -shr 24) -band 0xFF

    # Write patched PS-EXE back to file (full payload, no GAME.BIN split,
    # no truncation -- the BIOS loads the complete image at boot)
    [System.IO.File]::WriteAllBytes("$build/ps-exe/SLPS_015.56", $exeBytes2)
    # Remove any stale disc copy of GAME.BIN
    if (Test-Path "$root\disk\extract\CDI\ZZGAME.BIN") {
        Remove-Item "$root\disk\extract\CDI\ZZGAME.BIN" -Force
        Write-Host "  Removed stale disk/extract/CDI/ZZGAME.BIN" -ForegroundColor Yellow
    }
Write-Host "  Second pass PS-EXE: $(Get-Item $build\ps-exe\SLPS_015.56 | Select-Object -ExpandProperty Length) bytes" -ForegroundColor Yellow

    # Build final ISO using mkpsxiso with updated EXE (second pass)
    Write-Host "--- mkpsxiso: Building final ISO (second pass) ---" -ForegroundColor Yellow
    if (Test-Path "$build\lsddecomp.bin") { Remove-Item "$build\lsddecomp.bin" -Force -ErrorAction SilentlyContinue }
    if (Test-Path "$build\lsddecomp.cue") { Remove-Item "$build\lsddecomp.cue" -Force -ErrorAction SilentlyContinue }
    Push-Location $build; $isoOut2 = & $mkp -y -lba "$build\lbas.txt" "$root\config\disk.xml" 2>&1; Pop-Location
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Second pass mkpsxiso FAILED" -ForegroundColor Red
        Write-Host $isoOut2
        exit 1
    }
    Write-Host $isoOut2

    Write-Host ""
    Write-Host "=== done ==="
    $finalBinPath = "$build\lsddecomp.bin"
    $finalCuePath = "$build\lsddecomp.cue"
    $isoSizeBytes = if (Test-Path $finalBinPath) { (Get-Item $finalBinPath).Length } else { 0 }
    Write-Host ("PS-EXE: $build\ps-exe\SLPS_015.56 ($exeSize bytes)")
    Write-Host ("ISO:    $finalBinPath ($isoSizeBytes bytes)")
    Write-Host ("CUE:    $finalCuePath")



