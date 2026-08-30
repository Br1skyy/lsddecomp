# Linker Duplicates and Symbol Resolution Catalog

Why this file exists: `scripts/build.sh` links with
`--allow-multiple-definition` and `--unresolved-symbols=ignore-all`. That
turns two classes of mistake into silent behavior instead of errors:

1. When two objects define the same symbol, the first object on the
   linker command line wins and every later definition is ignored.
2. Undefined symbols do not fail the link; references can resolve to 0
   or garbage without any diagnostic.

Both have already produced real bugs (see Boot_Debug.md, 2026-08-23
entries), so this file records the rules and every known case.

## Link order (scripts/build.sh)

The ld line is `C_OBJECTS` followed by `ASM_OBJECTS`.

- `C_OBJECTS`: `src/lsdde/*.c` in shell glob order (alphabetical on
  most systems), then `src/globals.c`.
- `ASM_OBJECTS`: `find asm/lsdde -name "*.s"` output order. On a fresh
  checkout that lists root `.s` files first (`stub_fns`, `psyq_*`,
  `override`, `missing_stubs`, `missing_aliases`, `main`, `header`),
  then everything under `data/`, then `class_*`, `cdrom_handler`,
  `cd_stage2*`, and finally the `auto_*` files.

Practical consequences:

- C definitions beat all asm definitions.
- Root asm stubs (including `missing_stubs.s`) beat real tables under
  `data/`.
- `auto_*` stub files link after `data/`, so they lose to real tables.

NOTE: `find`/readdir order is not guaranteed across filesystems or
platforms. If a resolution below ever looks wrong on your machine,
print `ASM_OBJECTS` from build.sh before trusting it. Sorting the find
output explicitly would make this deterministic but would also reshuffle
every resolution described here, so it has not been done yet.

## Known duplicate definitions

### Data shadowed by text stubs (removed 2026-08-23, verified on Windows)

These symbols existed both as text stubs in `missing_stubs.s` and as
real tables under `asm/lsdde/data/`. The stubs won until their
definitions were removed. Verified with a fresh Windows rebuild
(`powershell -ExecutionPolicy Bypass -File scripts/build_ps1.ps1`,
`-Wl,-Map=build_ps1/ps-exe/lsddecomp.map`, `lsdde.ld:240` /
`scripts/build_ps1.ps1:153`):

| Symbol | Stub was | Real definition | Map after fix (`.data` `0x8007d828-0x800a8194`) |
| --- | --- | --- | --- |
| `DREAMSYS_METHODS` | `j _stub_ret` in `.text` | DreamSys class vtable, `data/DreamSys.data.s` | `0x8009ebc0` `.data 0x8009ebc0 0x114c DreamSys.data.s.o` |
| `D_800878D4` (`GameManagerPtr`) | `.word 0` in `.text` | GameManager data, `data/76DC8.data.s` | `0x8009e8b8` `.data 0x8009d5ac 0x1614 76DC8.data.s.o` |
| `SPAWN_POS_ADJUST` | `.word 0` in `.text` | spawn table, `data/DreamSys.data.s` | `0x8009???` in `DreamSys.data.s.o` `.data` (dup gone, `nm` shows single `D`) |
| `SPECIAL_DAYS` | `j _stub_ret` in `.text` | `data/DreamSys.data.s` | single `D` in `DreamSys.data.s.o` |
| `LOCATION_SPAWNPOINTS` | `j _stub_ret` in `.text` | `data/DreamSys.data.s` | `0x8009f59c` `.data` `DreamSys.data.s.o` |
| `LOCATION_TIME_LIMITS` | `j _stub_ret` in `.text` | `data/DreamSys.data.s` | `0x8009eef8` `.data` `DreamSys.data.s.o` |
| `LOCATION_GRID_DIMENSIONS` | — (was `STAGE_GRID_DIMENSIONS` in `StageGrid2.c.o`) | `src/lsdde/StageGrid2.c` `LOCATION_GRID_DIMENSIONS[14]` | `0x8009d558` `.data 0x8009d1d4 0x3d8 StageGrid2.c.o` |
| `LOCATION_CHUNK_MOODS` | — | `src/lsdde/StageGrid2.c` `LOCATION_CHUNK_MOODS[14]` | `0x8009d1d4` `.data` `StageGrid2.c.o` |

`nm --defined-only build_ps1/**/*.o | sort | uniq -d` now reports **zero**
duplicates for these six (previously `T` in `missing_stubs.s.o` vs `D` in
`data/*.o`). `readelf -s` confirms each as `OBJECT GLOBAL DEFAULT` in section
`2` (`.main`) at a `.data` address above `main_TEXT_END 0x8007d828`.

Impact while the bug was live:

- `Get_vtable_DreamSys()` returned a text address, so `New_DreamSys`
  called `*(vt+8)` and `DreamSys__DreamSys` called `*(gm+8)` through
  instruction bytes instead of vtable entries.
- `GetGameManager()` (src/lsdde/DreamSys.c line 43) returned a text
  address, so every manager field access read code bytes.
- Gameplay code read instruction encodings as spawn points, day counts
  and time limits.
- `LOCATION_GRID_DIMENSIONS` was defined as `STAGE_GRID_DIMENSIONS` in the
  old `StageGrid2.c.o` `D` but never shadowed by `missing_stubs.s`; the wiki
  rename to `LocationGrid` (`include/LocationGrid.h:1` guard fix,
  `src/lsdde/StageGrid2.c:238` `SIZEOF_LOCATION_*`) is now reflected in the map.
  The file itself is still `StageGrid2.c` and `lsdde.ld:55` / `scripts/build_ps1.ps1:98`
  still reference `StageGrid2.c.o` — a pure wiki rename to `LocationGrid2.c` is
  pending if desired (static-only, no runtime effect).

### Function duplicates where the C version wins

Because all C objects precede all asm objects, these resolve to the C
side:

- `CdModeRunTask`, `CdModePoll`, `CdModeStopReading`,
  `CdModeStoreTriplet`: `Entity3_CdMode.c.o` beats `cdrom_handler.c.o`
  because glob order puts `Entity3_CdMode.c` first. The `cdrom_handler.c`
  copies disagree heavily with the Entity3 versions (different logic
  entirely), so treat them as stale sketches, not alternatives. They are
  dead code today; consider deleting or reconciling them later.
- `EntityAllocSmall`: `Entity2.c.o` beats `Entity3_CdMode.c.o`
  (alphabetical glob order). Both bodies are equivalent today.
- `func_8003b20c`: `Sound.c` beats `psyq_SpuSetMute.s`. Both versions
  return `&D_8006E4F0`, so nothing changes in practice.
- `func_8002C438`, `func_8002C3A8`, `func_8002CC0C`: `Sound.c` wins over
  any asm copy. Unlike func_8003b20c these have no asm counterpart in
  the current tree, so they are simply the only definitions.
- `GetCoordSystemVtable`: defined in `System.c`, wins over anything else.

### Aliases installed by lsdde_defsyms.ld

`PROVIDE` only defines a symbol when nothing else did, so aliases cannot
override real definitions, but they matter when auditing surprises:

- `func_80025FDC = CdModeInitDream`: class vtable slot +0x08 of
  `D_8006D3C8` calls straight into the per-type constructor.
- `func_80026cfc = CdModeRunTask`: intact asm calls this alias.
- `func_80018390 = GetCoordSystemVtable`.

## Audit after 2026-08-23 rebuild (static, Windows, no boot)

Re-ran `nm --defined-only` over `build_ps1/**/*.o` after the fresh map build.
Total duplicates: 29 (down from 36 before the six text-stub removals). Filtering
`--allow-multiple-definition` winners vs `data/*.s` real tables:

### Still `missing_stubs.o` vs `data/*.s` (not already in `lsdde_defsyms.ld:238 Location_*`)

**None for the `Location_*` class.** Every `Location_*` / `STAGE_*` symbol that
was previously shadowed now has a single `D` definition in its `data/*.s.o` or
`StageGrid2.c.o`. The only remaining `missing_stubs.o` vs `data/`-like duplicates are:

| Symbol | Objects | Winner (link order) | Boot relevance |
| --- | --- | --- | --- |
| `D_80010764` | `F28.rodata.s.o` `R 0000003c` vs `missing_stubs.s.o` `t 00000588` | `F28.rodata.s.o` (`data/`-adjacent rodata, but `missing_stubs` is `.text` t) — different sections, no real shadowing | **Low** — rodata padding / zero beyond loaded sections, not on `main->CdInitRom` path |
| `D_800879C4` (`GameManagerPtr2`) | `76DC8.data.s.o` `D 000013fc` vs `auto_gen_stubs.s.o` `D 00000020` | `76DC8.data.s.o` ( `lsdde.ld` lists `76DC8` before `auto_gen_stubs`, and `auto_*` links after `data/` per link-order note) | **Low** — second game-manager slot, not used in early boot; `D_800878D4` (first slot) already fixed and verified in `.data` |

No remaining `missing_stubs.o` `T` vs `data/*.s` `D` for `LOCATION_GRID_DIMENSIONS` /
`DREAMSYS_METHODS` / `LOCATION_TIME_LIMITS` / `LOCATION_SPAWNPOINTS` — those six
were the boot-critical set and are now clean (see table above).

### Other duplicate classes (ranked by boot-path relevance)

These are not `missing_stubs.o` vs `data/*.s`, but they still decide the boot
image under `--allow-multiple-definition`. Ranked for the current RI crash at
`0x8007D864` → `0x80059DC0` ISR (`$t9=NopSub_27e68`, `$ra=template`, `$v1=0x13`):

1. **High — `CdModeRunTask` / `CdModePoll` / `CdModeStopReading` / `CdModeStoreTriplet`**  
   `Entity3_CdMode.c.o` `T` vs `cdrom_handler.c.o` `T`. Winner is `Entity3_CdMode.c.o`
   (glob alphabetical). The `cdrom_handler.c` copies are stale sketches with different
   logic — intact asm callers (`psyq_SpuSetMute.s:5596` `lw v0,0x6C(v0)` → `jalr`,
   plus 26+ `func_80026cac` (`CdModePoll`) sites) still dispatch through instance
   offsets `+0x40..+0x58` and `+0x68..+0x74` (statically `func_800272C8..func_80027D70`).
   If `CdModeSubD` had been called inside the primitive walk (fixed in
   `src/lsdde/Entity3_CdMode.c:118` to a single call before the walk), those slots
   would be wiped to zero/garbage and a `jalr` through `0x6C(v0)` lands in data —
   exactly the RI template at `0x8007D864` (`+8 = 0x00000001` SPECIAL funct 1).
   `func_8003af8c` at `psyq_SpuSetMute.s:5215` shows the expected shape: one
   `jal func_80026cfc` before the final `lw v0,0x40(v0)` → `jalr`.

2. **High — `EntityAllocSmall`**  
   `Entity2.c.o` `T 00002ce4` vs `Entity3_CdMode.c.o` `T 000006bc`. Winner `Entity2.c.o`
   (alphabetical). Both bodies are equivalent today (`MemAllocImpl 0x2c` → `NopSub_269e0`
   → `vt+8`), but it is **on the crash path** (`main->CdInitRom->EntityAllocSmall`),
   so any divergence would be boot-blocking. Keep the two in sync or delete one.

3. **Medium — `func_8003b20c`**  
   `Sound.c` `T 00001da0` vs `psyq_SpuSetMute.s` `T 00004740`. Winner `Sound.c`.
   Both return `&D_8006E4F0`; no behavioral change today, but `func_8003af8c`
   (line 5227 `jal func_8003b20c`) depends on it during `CdModeInitDream`.

4. **Low — `CdGetFilePathPrefix` / `EntityAlloc50` / `EntityAllocLarge` / `NavMenu_*` / `TextInput_Update` / `SoundSetTeleportFlag` etc.**  
   C-vs-C or C-vs-asm dups where the C version wins due to `C_OBJECTS` before
   `ASM_OBJECTS`. None are on the immediate `func_8003af8c → CdModeRunTask(0x13)`
   chain, but they should be audited when touching rendering / SPU / card code.

5. **Low — absolute symbols (`LBA_GAME_BIN`, `GAME_BIN_SECTORS`, `SLPS_SECTORS`, `LBA_SLPS`)**  
   `main.s.o` vs `cd_stage2.s.o` each `a`. Identical values from `include/lba_table.inc`;
   harmless.

`lsdde_defsyms.ld:238` `Location_*` aliases (e.g. `Location_ChunkSpawnCounts = D_80086170`,
`Location_AdjEdgeFlags = D_8008688C`, `Location_GridVtable = D_80086668`, etc.) are
`PROVIDE`s and only matter if nothing else defines the symbol — they do not override
real definitions and do not contribute to the duplicates above.

## How to audit for more of these

1. On the Windows side, link once with `-Wl,-Map=build_ps1/ps-exe/lsddecomp.map`
   (historic docs name is `build_ps1/lsddecomp.map`; `scripts/build_ps1.ps1:153`
   writes to `build_ps1/ps-exe/lsddecomp.map`).
2. For each symbol you care about, confirm its address lands in the
   input object you expect inside the map (e.g. `LOCATION_GRID_DIMENSIONS`
   `0x8009d558` in `StageGrid2.c.o` `.data`, not `.text`).
3. Before linking, run
   `nm --defined-only *.o | sort | uniq -d` over `build_ps1` to list
   every duplicated symbol while it is still cheap to fix.
