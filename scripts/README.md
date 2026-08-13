# Scripts Directory

This directory contains build and utility scripts for the LSD: Dream Emulator reconstruction project. None of them have been harmed while writing this file.

## Build Scripts

### build_ps1.ps1
Main build script for Windows and PowerShell Core (cross-platform).

**Usage:**
```bash
# From project root
pwsh scripts/build_ps1.ps1
```

**Features:**
- Compiles C source code
- Assembles MIPS assembly
- Links using custom linker script
- Generates PS-EXE and ISO
- Cross-platform tool detection
- Requires the PsyQ SDK in psyq/ (run fetch_psyq.ps1 first if missing;
  unlike build.sh, the PowerShell script does not auto-fetch it)

### build.sh
Bash equivalent of build_ps1.ps1 for Linux/macOS.

**Usage:**
```bash
# From project root
./scripts/build.sh
```

**Prerequisites:**
```bash
chmod +x scripts/build.sh
```

**Features:**
- Near-feature parity with the PowerShell version (also auto-fetches the
  PsyQ SDK if psyq/ is missing)
- Cross-platform tool detection
- Consistent build behavior
- Native Unix compatibility

## Utility Scripts

### extract.ps1
Disc extraction script for PowerShell.

**Usage:**
```bash
pwsh scripts/extract.ps1 -Image <path-to-lsd.cue-or-bin>
```

**Features:**
- Automatic disc detection
- Cross-platform binary tool handling
- Extracts to disk/extract/
- Validates extraction completeness

### extract.sh
Bash equivalent of extract.ps1 for Linux/macOS.

**Usage:**
```bash
./scripts/extract.sh <path-to-lsd.cue-or-bin>
```

**Prerequisites:**
```bash
chmod +x scripts/extract.sh
```

**Features:**
- Automatic disc detection
- Cross-platform binary tool handling
- Consistent behavior with PowerShell version

### fetch_psyq.ps1
PsyQ SDK download script.

**Usage:**
```bash
pwsh scripts/fetch_psyq.ps1
```

**Features:**
- Downloads PSY-Q SDK 4.7
- Extracts to psyq/ directory
- Gitignored (not committed)
- Auto-runs from build.sh when psyq/ is missing (build_ps1.ps1 expects
  psyq/ to already be present)

### track_progress.py
Progress tracking utility for monitoring project completion.

**Usage:**
```bash
python scripts/track_progress.py
```

**Features:**
- Counts stub functions
- Counts raw Ghidra variables
- Checks linker hack status
- Estimates boot progress
- Prints summary to console

## Script Execution

### Windows
```bash
# Build
pwsh scripts/build_ps1.ps1

# Extract
pwsh scripts/extract.ps1 -Image game.cue

# Progress
python scripts/track_progress.py
```

### Linux/macOS
```bash
# Make scripts executable first
chmod +x scripts/*.sh

# Build
./scripts/build.sh

# Extract
./scripts/extract.sh game.cue

# Progress
python scripts/track_progress.py
```

### Cross-Platform (PowerShell)
```bash
# Works on any platform with pwsh installed
pwsh scripts/build_ps1.ps1
pwsh scripts/extract.ps1 -Image game.cue
python scripts/track_progress.py
```

## Dependencies

### Common Requirements
- **PowerShell Core (pwsh)** - Required for .ps1 scripts
- **Python 3** - Required for track_progress.py
- **mipsel-none-elf-gcc** - Cross-compiler for building

### Platform-Specific
- **Windows**: Native tool support
- **Linux/macOS**: May need Wine for Windows binary tools

## Script Organization

Scripts are organized by function:

- **Build**: build_ps1.ps1, build.sh
- **Extraction**: extract.ps1, extract.sh
- **Utilities**: fetch_psyq.ps1, track_progress.py

All scripts use relative paths and can be run from the project root.

## Error Handling

Scripts include comprehensive error handling:

- Missing tool detection
- File validation
- Build failure reporting
- Clear error messages
