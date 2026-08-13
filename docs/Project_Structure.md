# Project Structure

This document describes the organization and layout of the LSD: Dream Emulator C reconstruction project. It's mostly accurate. Mostly.

## Repository Layout

```
lsddecomp-main/
├── asm/              # MIPS assembly source code
├── build_ps1/       # Build output directory (gitignored)
├── config/          # Build configuration files
├── docs/            # Project documentation
├── include/         # Header files and assembly macros
├── psyq/            # PsyQ SDK (gitignored; fetched by scripts/fetch_psyq.ps1, auto-runs in build.sh)
├── scripts/         # Build and utility scripts
├── src/             # C reconstruction source code
├── tools/           # Build tools
└── disk/            # Extracted game data (gitignored)
```

**Notable root files not shown above:**
- `lsdde.ld` - Linker script (main output at 0x80010000; INCLUDEs lsdde_defsyms.ld at line 240)
- `lsdde_defsyms.ld` - Additional symbol definitions (PROVIDE aliases, required by the linker script, tracked in git)

## Directory Descriptions

### asm/ (Assembly Source)

Contains MIPS assembly source code for functions not yet decompiled to C, plus data sections. Someday this folder will be a historical relic. We look forward to that day.

**Key subdirectories:**
- `asm/lsdde/` - Main assembly implementation files
- `asm/lsdde/data/` - Assembly data sections (.rodata, .data, .sdata, .sbss)

**Purpose:** Houses assembly stubs and implementations for functions that haven't been converted to C yet.

### build_ps1/ (Build Output)

Generated build artifacts (gitignored).

**Contents:**
- Compiled object files
- Linked ELF executables
- PS-EXE files
- Generated ISO images

**Purpose:** Temporary build output directory, not committed to version control.

### config/ (Build Configuration)

Configuration files for the build system and symbol mapping.

**Key files:**
- `disk.xml` - ISO generation configuration (points at build_ps1/ps-exe/SLPS_015.56 and disk/extract/CDI)
- `splat.slps01556.lsdde.yaml` - Symbol map configuration
- `symbols.slps01556.lsdde.txt` - Symbol mappings
- `check.slps01556.sha` - Expected SHA of the extracted SLPS_015.56 (integrity check)
- `undefined_funcs_auto.slps01556.lsdde.txt` / `undefined_syms_auto.slps01556.lsdde.txt` - splat auto-generated function/symbol tables (gitignored pattern `*auto.*.txt`)
- `original_layout.xml` / `test_layout.xml` / `license.bin` - dumpsxiso extraction artifacts (gitignored)

**Purpose:** Controls how the build system links and packages the final PS1 executable.

### docs/ (Documentation)

Project documentation and analysis guides.

**Files:**
- Project_Structure.md - This file
- Game_Overview.md - High-level game mechanics and structure
- Data_Formats.md - File format specifications
- Reverse_Engineering_Guide.md - RE guidelines and methodology
- Code_Style.md - Code style and formatting standards

**Purpose:** Comprehensive documentation for understanding and contributing to the project.

### include/ (Headers)

Header files and assembly macros.

**Key subdirectories:**
- `include/lsdde/` - Project-specific headers
  - `ghidra_compat.h` - Ghidra compatibility definitions
  - `magic_numbers.h` - Game-specific constants
  - `psx_types.h` - PS1 type definitions
  - `structs.h` - Structure definitions

**Other files:**
- `*.inc` - Assembly macro files
- `*.h` - C header files

**Purpose:** Shared headers and macros used across the codebase.

### psyq/ (PsyQ SDK)

Sony's PlayStation SDK (gitignored, auto-downloaded).

**Structure:**
- `psyq/include/` - SDK header files
- `psyq/lib/` - SDK library files

**Purpose:** Official PlayStation 1 development libraries required for building.

### scripts/ (Build Scripts)

Build and utility scripts for development.

**Key scripts:**
- `build_ps1.ps1` - Main build script (PowerShell)
- `build.sh` - Main build script (Bash)
- `extract.ps1` - Disc extraction (PowerShell)
- `extract.sh` - Disc extraction (Bash)
- `fetch_psyq.ps1` - SDK download script
- `track_progress.py` - Progress tracking utility

**Purpose:** Automation for building, extracting, and tracking project progress.

### src/ (C Source)

C reconstruction source code.

**Key subdirectory:**
- `src/lsdde/` - Main C implementation files (25 files)

**Major components:**
- `DreamSys.c` - Dream state machine
- `System.c` - GS library wrappers
- `Sound.c` / `SpuInit.c` - Audio system
- `GameLoop.c` - Frame loop
- `Rendering_*.c` - Rendering system (7 files)
- `Entity2.c` / `Entity3_*.c` - Entity system (6 files)
- `NavMenu_*.c` - Menu/UI system (2 files)
- `Other.c` - Miscellaneous functionality

**Purpose:** The main reconstructed game logic in readable C.

### tools/ (Build Tools)

Binary tools for building.

**Key tools:**
- `mkpsxiso.exe` - ISO generation tool (Windows)
- `dumpsxiso.exe` - Disc extraction tool (Windows)

**Purpose:** Essential tools for building the PS1 executable.

### disk/ (Game Data)

Extracted game disc data (gitignored).

**Contents:**
- Extracted game files
- CD-ROM data
- Game assets

**Purpose:** Contains copyrighted game data, not committed to version control.

## Module Organization

### Core Systems

1. **Dream System** (DreamSys.c)
   - State machine for dream progression
   - Stage linking and navigation
   - Flashback system

2. **Rendering** (Rendering_*.c)
   - World rendering
   - Entity rendering
   - UI rendering
   - Stage rendering
   - Chunk rendering

3. **Entity System** (Entity*.c)
   - Entity lifecycle management
   - Entity behaviors
   - Particle systems
   - Entity interactions

4. **Audio** (Sound.c, SpuInit.c)
   - SPU initialization
   - Sound playback
   - Voice management
   - CD audio

5. **Input/Navigation** (NavMenu_*.c)
   - Menu system
   - Text input
   - Memory card interface

## File Naming Conventions

- **C files**: PascalCase with descriptive names (DreamSys.c, GameLoop.c)
- **Assembly files**: Lowercase with underscores (main.s, cdrom_handler.s)
- **Headers**: Match corresponding C files or use descriptive names
- **Scripts**: Descriptive names with appropriate extensions (.ps1, .sh, .py)

## Dependencies

### Build Dependencies
- mipsel-none-elf-gcc cross-compiler
- PsyQ SDK 4.7
- PowerShell Core (pwsh)
- Python 3 (for utilities)

### Runtime Dependencies
- PS1 emulator or real hardware
- Original LSD: Dream Emulator game disc

## Build Facts

### Prerequisites for a Build

- Own PsyQ SDK present (`psyq/` - headers + libs). NOT committed, and must
  not be: it's Sony IP.
- Extracted disc in `disk/extract/` (`extract.xml`, `SLPS_015.56`,
  `SYSTEM.CNF`, `CDI/`). Extraction: `scripts/extract.ps1` (auto-detects
  `.cue/.bin` in `disk/` or takes `-Image`).
- Tools: `tools/mkpsxiso.exe`, `tools/dumpsxiso.exe` (v2.30).
- Toolchain resolution order: PATH, then `C:\mipsel-none-elf\bin`,
  `/usr/local/mipsel-none-elf/bin`, `/opt/mipsel-none-elf/bin`. Needs
  gcc/ld/objcopy/nm.

### skipCFiles

These C files are NOT built; the asm stub takes their place (list in
`scripts/build_ps1.ps1`):

```
psyq_15d04.c, psyq_GsLinkObject4.c, psyq_PadInit.c, psyq_rand.c,
psyq_rcpolyf3.c, psyq_rcpolyf4.c, psyq_rcpolyft3.c, psyq_rcpolyg3.c,
Rendering.c, Entity3_vtable.c, 2C054.c, 5CAB4.c, class_16334.c,
cd_stage2.c
```

### Copyright Audit (done)

- No ROM, no game assets (textures/audio), no Sony SDK headers committed.
- `include/psyq/` (verbatim Sony headers) was deleted - never on the build
  include path anyway (C sources include lowercase libgpu.h/libcd.h which
  resolve from the bundled psyq/include).
- `asm/lsdde/psyq_*.s` kept: ROM disassembly, not verbatim SDK.

### Environment Gotchas

- PowerShell 5.1 execution policy blocks unsuffixed nested script runs; tests
  use `powershell -ExecutionPolicy Bypass`. README documents the right
  invocation.
- Windows + PS1 dev: `disk/dumpsxiso -x <dir>` writes extract.xml; `-s` sets
  the XML name/path.
