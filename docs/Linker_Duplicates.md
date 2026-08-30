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

### Data shadowed by text stubs (removed 2026-08-23)

These symbols existed both as text stubs in `missing_stubs.s` and as
real tables under `asm/lsdde/data/`. The stubs won until their
definitions were removed:

| Symbol | Stub was | Real definition |
| --- | --- | --- |
| `DREAMSYS_METHODS` | `j _stub_ret` in `.text` | DreamSys class vtable, `data/DreamSys.data.s` |
| `D_800878D4` | `.word 0` in `.text` | GameManager data, `data/76DC8.data.s` |
| `SPAWN_POS_ADJUST` | `.word 0` in `.text` | spawn table, `data/DreamSys.data.s` |
| `SPECIAL_DAYS` | `j _stub_ret` in `.text` | `data/DreamSys.data.s` |
| `LOCATION_SPAWNPOINTS` | `j _stub_ret` in `.text` | `data/DreamSys.data.s` |
| `LOCATION_TIME_LIMITS` | `j _stub_ret` in `.text` | `data/DreamSys.data.s` |

Impact while the bug was live:

- `Get_vtable_DreamSys()` returned a text address, so `New_DreamSys`
  called `*(vt+8)` and `DreamSys__DreamSys` called `*(gm+8)` through
  instruction bytes instead of vtable entries.
- `GetGameManager()` (src/lsdde/DreamSys.c line 43) returned a text
  address, so every manager field access read code bytes.
- Gameplay code read instruction encodings as spawn points, day counts
  and time limits.

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

## How to audit for more of these

1. On the Windows side, link once with `-Wl,-Map=build_ps1/lsddecomp.map`.
2. For each symbol you care about, confirm its address lands in the
   input object you expect inside the map.
3. Before linking, run
   `nm --defined-only *.o | sort | uniq -d` over `build_ps1` to list
   every duplicated symbol while it is still cheap to fix.
