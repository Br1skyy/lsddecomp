# LSD: Dream Emulator - C Reconstruction

This project is reverse-engineering LSD: Dream Emulator (1998, PS1) and rewriting it as C.

**Important Note**: This is NOT a byte-matching decomp. I am not trying to rebuild the original binary bit for bit. What I want is the game's logic back in C that a person can read, study, and eventually mod or port if they wish sort of a documentation-grade reconstruction, built on top of the Ghidra output.

If you came here expecting a matching project like the Crash or Spyro decomps this is not it. You will be dissapointed.

## Documentation

This project has documentation across several files:

- **Project Structure** - Overview of the repository layout and module organization
- **Game Overview** - High-level game mechanics, structure, and hardware/engine documentation  
- **Data Formats** - Overview of file formats used throughout the repository
- **Reverse Engineering Guide** - Guidelines and documentation on reverse-engineering, analyzing, or decompiling the codebase
- **Code Style** - Style guide, formatting rules, and code standards for the project
- **Boot Debug** - The current boot blocker (Reserved Instruction crash) and the investigation so far

All docs live in `docs/`.

## Project Status

**Honest status: the game does not boot yet. The build crashes early in boot and the root cause is currently unknown.**

What works:
- The C + asm reconstruction compiles, links, and produces a PS-EXE/ISO.
- Boot reaches the CD-ROM init warm-up (`CdInitRom`) past the IRQ2 storm that
  previously starved `main()` (fixed 2026-08-12 by using 32-bit `sw` for the
  I_STAT ack in cdrom_handler.c instead of 16-bit `sh`).

What breaks:
- The game crashes with a Reserved Instruction (RI) exception storm shortly
  after CD init. The CPU ends up executing the `D_80066828` data block at
  `0x8007D864` as code (the word at `0x8007D86C` decodes to funct 1, a
  reserved instruction). Observed execution chain: `main` -> `CdInitRom` ->
  `EntityAllocSmall` -> `CdModeInitDream` -> `func_8003af8c` -> `CdModeRunTask`
  (mode 0x13) -> a call into the entity template.
- The exact control-transfer that lands execution in that data block has not
  been found. This is the current blocker; work on it is stuck.

The earlier "hangs on the licensing screen" diagnosis (in older docs and
Notes.txt) was wrong and is retired.

## Code Quality Warning

Since this is a reconstruction, the code quality is still rough:

- **Most of the C is still raw Ghidra output.** Thousands of iVar/uVar variables, many indirect vtable calls, and hundreds of pointer casts standing in for struct fields. Structifying and naming all of it is the actual project. We'll get there eventualy.
- **The build leans on linker hacks**: --allow-multiple-definition, --unresolved-symbols=ignore-all, and generated stub assembly for symbols not yet written in C. If it links, it ships. That is the operating principle here.
- **Assembly stubs**: Many functions remain as assembly stubs in asm/lsdde/ for functions not yet written in C.

## Build Instructions

This project supports cross-platform building on Windows, Linux, and macOS.

### Prerequisites

**Common requirements:**
- **PowerShell Core (pwsh)** - Required for all platforms (for some tools). This is a separate install, not bundled with Windows.
  - Windows: winget install Microsoft.PowerShell (or from the GitHub releases)
  - Linux: sudo apt install powershell (Debian/Ubuntu) or sudo yum install powershell (RHEL/CentOS)
  - macOS: brew install powershell
- **mipsel-none-elf-gcc** cross-compiler must be on PATH
- **PsyQ SDK** - The build expects it in psyq/ (gitignored). Run scripts/fetch_psyq.ps1 to pull PSY-Q SDK 4.7 from the psx.arthus.net mirror into psyq/ (build.sh auto-runs it if psyq/ is missing). Or drop your own copy in psyq/ (include/ + lib/).

**Platform-specific requirements:**
- **Windows**: Native tools work out of the box
- **Linux/macOS**: May need Wine for Windows-only binary tools (see tools/README.md)

### Initial Setup

**1. Extract the game disc:**

Windows (PowerShell):
```bash
pwsh scripts/extract.ps1 -Image <your-lsd.cue-or-bin>
```

Linux/macOS (Bash):
```bash
./scripts/extract.sh <your-lsd.cue-or-bin>
```

*(You need the game disc it is copyrighted and not in this repo you should definitleyy buy a real copy and dump it!)*

**2. Make shell scripts executable (Linux/macOS only):**
```bash
chmod +x scripts/*.sh
```

### Main Build

**Windows (PowerShell):**
```bash
pwsh scripts/build_ps1.ps1
```

**Linux/macOS (Bash):**
```bash
./scripts/build.sh
```

*(Or use PowerShell on any platform: pwsh scripts/build_ps1.ps1)*

This compiles C reconstruction (src/lsdde/) plus MIPS assembly (asm/lsdde/), links with lsdde.ld, and runs tools/mkpsxiso to produce the ISO under build_ps1/:
- build_ps1/lsddecomp.cue + build_ps1/lsddecomp.bin - load in a PS1 emulator or real console
- build_ps1/ps-exe/lsddecomp.elf - the ELF
- build_ps1/ps-exe/SLPS_015.56 - the PS-EXE the ISO boots

### Build Products (reference sizes)

| File | Size | Notes |
|------|------|-------|
| build_ps1/ps-exe/SLPS_015.56 | 668,472 B | PS-EXE the ISO boots; full payload embedded (no GAME.BIN split) |
| build_ps1/ps-exe/lsddecomp.elf | 964,172 B | linked ELF (with debug info) |
| build_ps1/lsddecomp.bin | 648,277,056 B | ISO image (data track) |
| build_ps1/lsddecomp.cue | 75 B | cue sheet for the ISO |

The disc carries no GAME.BIN/ZZGAME.BIN. The full game image is embedded in the single PS-EXE, so the BIOS loads it at boot and CdLoadStage2's skip-check skips CD reads. In other words, the disc is one big PS-EXE stamped onto a CD.

## Project Structure

### Root Level

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

### Key Directories

**asm/lsdde/**: MIPS assembly source code
- **data/**: Assembly data sections (.rodata, .data, .sdata, .sbss)
- *.s: Assembly implementation files

**include/**: Header files and assembly macros
- **lsdde/**: Project-specific headers (ghidra_compat.h, magic_numbers.h, psx_types.h, structs.h)
- *.inc: Assembly macro files
- *.h: C header files

**src/lsdde/**: C reconstruction source code (25 files)
- DreamSys.c: Dream state machine
- System.c: GS library wrappers
- Sound.c / SpuInit.c: Audio system
- GameLoop.c: Frame loop
- Rendering_*.c: Rendering system (split 7 ways: Chunk, Entity, NavMemCard, RenderUtils, Stage, UI, World)
- Entity2.c / Entity3_*.c: Entity system and behaviors (5 Entity3 files)
- NavMenu_*.c: Menu/UI system (2 files)
- Other.c: Miscellaneous functionality

**scripts/**: Build and utility scripts
- build_ps1.ps1: Main build script (Windows/PowerShell)
- build.sh: Main build script (Linux/macOS/Bash)
- extract.ps1: Disc extraction script (PowerShell)
- extract.sh: Disc extraction script (Bash)
- fetch_psyq.ps1: PsyQ SDK download script
- track_progress.py: Progress tracking utility

**tools/**: Build tools
- mkpsxiso.exe: ISO generation tool (Windows)
- dumpsxiso.exe: Disc extraction tool (Windows)

**config/**: Build configuration files
- disk.xml: ISO generation config
- splat.slps01556.lsdde.yaml: Symbol map config
- symbols.slps01556.lsdde.txt: Symbol mappings

### Root Configuration Files

- **lsdde.ld**: Linker script
- **lsdde_defsyms.ld**: Additional symbol definitions
- **docs/**: Project documentation (Project Structure, Game Overview, Data Formats, Reverse Engineering Guide, Code Style, Boot Debug)
- **LICENSE**: MIT license for the reconstruction code
- **.gitattributes**: Git configuration for proper line ending handling

## Progress Tracking

Run the progress tracker to see current completion status:

```bash
python scripts/track_progress.py
```

This generates detailed metrics including:
- Overall completion percentage
- Stub function count
- Raw Ghidra variable count
- Linker hack removal status
- Boot status assessment

## Style Guide

Named things get PascalCase with a __ system prefix (DreamSys__AdvanceDay), locals are camelCase. The goal is to eliminate every undefined4, DAT_800, and param_1 from the codebase.

See Code Style for detailed formatting rules and code standards.

## Credit

- **FirecatFG** - the original decomp work this builds on
- The PS1 decomp community - tools, knowledge, and patience
- **ethteck** - splat
- **matt-kempster** - m2c

## License

MIT for my code. The game itself belongs to Asmik Ace (and everyone else with a stake in it), and the ROM is not in this repo bring your own. The PsyQ SDK headers/libraries are not bundled either; provide your own copy!
