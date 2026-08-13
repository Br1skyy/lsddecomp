param(
    [string]$Image = ""
)

$ErrorActionPreference = "Stop"
$root = Split-Path $PSScriptRoot -Parent

# find dumpsxiso (cross-platform)
$isWindows = $PSVersionTable.Platform -eq "Win32NT" -or $null -eq $PSVersionTable.Platform
$dx = Join-Path $root "tools/dumpsxiso"
if ($isWindows) {
    $dx = $dx + ".exe"
}

if (-not (Test-Path $dx)) { 
    $dx = (Get-Command "dumpsxiso" -ErrorAction SilentlyContinue).Source 
}
if (-not $dx) { throw "dumpsxiso not found" }

# no image given, look in disk/
if (-not $Image) {
    $found = @()
    $found += Get-ChildItem (Join-Path $root "disk") -Filter "*.cue" -File -ErrorAction SilentlyContinue
    $found += Get-ChildItem (Join-Path $root "disk") -Filter "*.bin" -File -ErrorAction SilentlyContinue
    $found = $found | Sort-Object Length -Descending
    if ($found.Count -eq 0) {
        throw "no .cue/.bin in disk/. pass one: pwsh scripts/extract.ps1 -Image path/to/LSD.cue"
    }
    $Image = $found[0].FullName
}

$Image = (Resolve-Path $Image).Path
$dest = Join-Path $root "disk/extract"
New-Item -ItemType Directory -Path $dest -Force | Out-Null

Write-Host "extracting $Image -> $dest"
& $dx -x $dest $Image
if ($LASTEXITCODE -ne 0) { throw "dumpsxiso failed (exit $LASTEXITCODE)" }

# make sure we got what the build needs
$missing = @()
foreach ($p in @("extract.xml","SLPS_015.56","SYSTEM.CNF","CDI")) {
    if (-not (Test-Path (Join-Path $dest $p))) { $missing += $p }
}
if ($missing.Count -gt 0) { throw "extraction missing: $($missing -join ', ') - wrong image?" }

Write-Host "done. extract.xml, SLPS_015.56, CDI/ in disk/extract/"
Write-Host "next: pwsh scripts/build_ps1.ps1"
