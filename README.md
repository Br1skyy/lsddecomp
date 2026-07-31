# LSD: Dream Emulator - C Reconstruction

This project is reverse-engineering LSD: Dream Emulator (1998, PS1) and rewriting it as C.

Lets be honest about what this is and what it is not, because the word decomp is thrown around a TON.

This is NOT a byte-matching decomp. I am not trying to rebuild the original binary bit for bit honestly it's not achievable for me, and I don't have a `% matched` number. I don't care about that at all. What I want is the game's logic back in C that a person can read, study, and eventually mod or port which I would love to do! a documentation-grade reconstruction, built on top of the Ghidra output.

If you came here expecting a matching project like the Crash or Spyro decomps this is not it.

## Where it stands

The build works.  It hangs on the licensing screen, I am not sure why yet but I want to get this build out since we have lost progress from data corruption. Expect the hang after licensing! Ok?

That boot issue is the current job and the one thing blocking everything else, because without a booting baseline I can't test any of the C restructuring without risking a totally silent break.

## The freakish parts

Since this is a reconstruction, the code quality is the product I want to sell you on. Right now it's not good at all, and I'd rather say so than dress it up:

- **Most of the C is still raw Ghidra output.** Thousands of `iVar`/`uVar` variables, a thousand-plus `(**(code **)(ptr + 0xNN))` vtable calls, and hundreds of `(int *)` casts standing in for struct fields. Structifying and naming all of it is the actual project. The `Rendering_*.c` files in particular are ~90% blank lines from the decompiler export - readable-ish but bloated.
- **The build leans on linker hacks to get anything to boot**: `--allow-multiple-definition`, `--unresolved-symbols=ignore-all`, and generated stub assembly for symbols I haven't written real C for yet. These are scaffolding, not the destination.
- **74 functions sit in `asm/lsdde/nonmatchings/`** - moved out because I stopped working on them, kept because they're still useful notes.

That's the honest state of it.

## Build

The build script is PowerShell and portable (it derives paths from `$PSScriptRoot`, so it works from any checkout location on Windows/macOS/Linux with `pwsh`). The `mipsel-none-elf-gcc` cross-compiler must be on `PATH` (or in a standard location: `C:\mipsel-none-elf\bin`, `/usr/local/mipsel-none-elf/bin`, `/opt/mipsel-none-elf/bin`).

**Prerequisites (once):**

1. PsyQ SDK - gitignored, provide your own. Drop it in `psyq/` (headers and libraries both come from your copy; the C sources include `libgpu.h`/`libcd.h`/etc. which resolve against `psyq/include`).
2. Extract the disc once with `tools/extract.ps1`. It runs the bundled `tools/dumpsxiso.exe`, fills `disk/extract/` (the game's CD data + `extract.xml`), and checks the output. It auto-detects a `.cue`/`.bin` dropped in `disk/`, or you can point it at one:
   ```bash
   pwsh tools/extract.ps1 -Image <your-lsd.cue-or-bin>
   ```
   (You need the game disc - it's copyrighted and not in this repo. `disk/extract/` is gitignored.)

**`./build_ps1.ps1` - the main game build.**

Compiles the C reconstruction (`src/lsdde/`) plus the MIPS assembly (`asm/lsdde/`), links with `lsdde.ld`, and runs `tools/mkpsxiso.exe` with `config/disk.xml` to produce the ISO. Assembles everything under `build_ps1/`:

- `build_ps1/lsddecomp.cue` + `build_ps1/lsddecomp.bin` - load the `.cue` in DuckStation or on a real console
- `build_ps1/ps-exe/lsddecomp.elf` - the ELF
- `build_ps1/ps-exe/SLPS_015.56` - the PS-EXE the ISO boots
- `build_ps1/lbas.txt` - LBA table, regenerated each run and turned into `include/lba_table.inc`/`.h`

```bash
pwsh ./build_ps1.ps1
```

The game used to boot through a two-stage loader (`build_loader.ps1` + `asm/loader/`) that read a GAME.BIN payload off the CD. That's retired: the whole image is now embedded in a single PS-EXE (`SLPS_015.56`), so `CdLoadStage2` just skips the CD read and finds the data already in RAM. One script, one binary.

Known state: the game boots, draws its own license screen, then hangs on it. Rendering and init are fine; the state machine never advances past the license. Current theory: the per-frame driver (the VSync callback / timer tick that steps the license pages) is stubbed out and never registered, so nothing advances the page each frame. That's the current job, not a build failure.

## Layout

```
asm/                  # MIPS assembly: lsdde/ splits (incl. data/ + nonmatchings/)
include/              # Headers (DreamSys.h, dat_globals.h, lsdde/) + asm macros
src/lsdde/            # The C - 25 files, ~32k lines
  DreamSys.c          # dream state machine
  System.c            # GS library wrappers
  Sound.c / GameLoop.c# audio + frame loop
  Rendering_*.c       # rendering, split 7 ways
  Entity2.c / Entity3_*.c  # entities & behaviors
  NavMenu_*.c         # menu/UI
  Other.c             # everything that didn't fit elsewhere
config/               # mkpsxiso config (disk.xml) + splat symbol maps
disk/                 # your extracted CD data (gitignored; not committed) - made by tools/extract.ps1
tools/                # mkpsxiso.exe + dumpsxiso.exe (bundled) + extract.ps1
build_ps1.ps1         # the one build script (C -> ELF -> PS-EXE -> ISO)
lsdde.ld              # linker script
```

## Style

Named things get PascalCase with a `__` system prefix (`DreamSys__AdvanceDay`), locals are camelCase, and I'm trying to kill every `undefined4`, `DAT_800`, and `param_1` left in the tree. There are still plenty. That's the work.

## Credit

- **FirecatFG** - the original decomp work this builds on
- The PS1 decomp community - tools, knowledge, and patience
- **ethteck** - splat
- **matt-kempster** - m2c

## License

MIT for my code. The game itself belongs to Asmik Ace (and everyone else with a stake in it), and the ROM is not in this repo bring your own. The PsyQ SDK headers/libraries are not bundled either; provide your own copy I don't know how you'll get it!
