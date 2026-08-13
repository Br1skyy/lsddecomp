# Tools Directory

This directory contains the prebuilt third-party Windows binaries used by
the build scripts. They are invoked automatically by the scripts in
`scripts/` - do not call them by hand as part of the normal workflow.

## Build Tools

### mkpsxiso.exe
ISO generation tool for creating PlayStation 1 disc images (v2.30).
Invoked by `scripts/build_ps1.ps1` / `scripts/build.sh`. `build_ps1.ps1`
runs a two-pass build: the first pass writes the ISO's LBA table
(`build_ps1/lbas.txt`, via mkpsxiso's `-lba` flag) so the PS-EXE can be
rebuilt with the correct boot sector, then the second pass produces the
final ISO. `build.sh` runs a single mkpsxiso pass.
Source: https://github.com/Lameguy64/mkpsxiso

### dumpsxiso.exe
Disc extraction tool for extracting data from PS1 CD images (v2.30).
Invoked by `scripts/extract.ps1` / `scripts/extract.sh`.
Same project as mkpsxiso (above).

## Platform Compatibility

- **Windows**: the bundled `.exe` binaries work natively.
- **Linux/macOS**: the Bash scripts (`build.sh`, `extract.sh`) prefer the
  bundled `tools/*.exe` and automatically run it under Wine when available,
  or use a native mkpsxiso/dumpsxiso build on PATH. The PowerShell scripts
  (`build_ps1.ps1`, `extract.ps1`) fall back to a native binary on PATH.