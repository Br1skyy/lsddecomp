param(
    [string]$Mirror = "https://psx.arthus.net/sdk/Psy-Q",
    [switch]$Force
)

# Downloads the official PSY-Q SDK (v4.7, converted for modern GNU toolchains)
# into psyq/. The game was built against this SDK; headers and libs both come
# from here. psyq/ stays gitignored, this is just the "bring your own" step
# made automatic.
#
# Sony released the SDK as freeware so this is just grabbing that mirror.
# If you already have your own copy, drop it in psyq/ (include/ + lib/) and
# this script will leave it alone.

$ErrorActionPreference = "Stop"
$root = Split-Path $PSScriptRoot -Parent
$psyq = Join-Path $root "psyq"

$keyHdr = Join-Path $psyq "include/libgpu.h"
$keyLib = Join-Path $psyq "lib/libapi.a"

if ((Test-Path $keyHdr) -and (Test-Path $keyLib) -and (-not $Force)) {
    Write-Host "psyq/ already present (found include/libgpu.h + lib/libapi.a). nothing to do."
    Write-Host "(pass -Force to re-download anyway)"
    exit 0
}

$file = "psyq-4_7-converted-light.zip"
$url = "$Mirror/$file"
$expected = "10FFDCD912FEFFDDEBE6955EA346BBD5DE2F912CE952BFC703994CDE49180C2A"

$tmp = Join-Path ([System.IO.Path]::GetTempPath()) "psyq-fetch"
New-Item -ItemType Directory -Path $tmp -Force | Out-Null
$zip = Join-Path $tmp $file

Write-Host "downloading $url"
Invoke-WebRequest -Uri $url -OutFile $zip -UseBasicParsing

$got = (Get-FileHash $zip -Algorithm SHA256).Hash
if ($got -ne $expected) {
    throw "hash mismatch (got $got, expected $expected) - mirror changed? update scripts/fetch_psyq.ps1"
}

Write-Host "unpacking into $psyq"
$ex = Join-Path $tmp "x"
New-Item -ItemType Directory -Path $ex -Force | Out-Null
Expand-Archive -Path $zip -DestinationPath $ex -Force
$pkg = Join-Path $ex "psyq-4_7-converted"

New-Item -ItemType Directory -Path $psyq -Force | Out-Null
Copy-Item -Recurse -Force (Join-Path $pkg "include") (Join-Path $psyq "include")
Copy-Item -Recurse -Force (Join-Path $pkg "lib") (Join-Path $psyq "lib")

# make sure the build's actual needs are there
$missing = @()
foreach ($p in @("include/libgpu.h","include/libcd.h","include/libspu.h",
                 "lib/libapi.a","lib/libgs.a","lib/libspu.a")) {
    if (-not (Test-Path (Join-Path $psyq $p))) { $missing += $p }
}
if ($missing.Count -gt 0) {
    throw "extraction incomplete, missing: $($missing -join ', ')"
}

Remove-Item -Recurse -Force $tmp
Write-Host "done. PSY-Q 4.7 (converted) in psyq/"
Write-Host "next: pwsh scripts/build_ps1.ps1"
