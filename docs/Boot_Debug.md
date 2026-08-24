# Boot Debug - Current Blocker (Reserved Instruction Crash)

This document tracks the current boot blocker. The game does NOT boot yet. If
you pick this project up, start here - this is the honest state of the fight.

## Current Symptom

- The build compiles, links, and produces a PS-EXE/ISO. Boot reaches the CD-ROM
  init warm-up (`CdInitRom`, debug marker D1=0x10) past the IRQ2 storm (fixed
  2026-08-12, see below).
- Then it crashes with a Reserved Instruction (RI) exception storm instead of
  reaching the license screen. The CPU executes the `D_80066828` data block at
  `0x8007D864` as code: the word at `0x8007D86C` (=template+8) decodes to
  funct 1, a reserved instruction.
- The earlier "hangs on the licensing screen" theory is RETIRED and wrong. That
  hang is long gone; the current blocker is the RI crash. Old license-screen
  findings (below) are kept as historical context only.

## Observed Crash Path

From a live debug session on the running game:

```
main -> CdInitRom -> EntityAllocSmall(D_80066828) -> CdModeInitDream ->
func_8003af8c -> CdModeRunTask(mode=0x13) -> call that lands in the template
```

The exact control-transfer instruction that lands execution in the template is
still unknown.

## Verified at Runtime

- The block struct at `0x800C8310`: `block[0x20] = 0x8007D864` (the template),
  `block[0] = 0x80084404` (vtable).
- The descriptor at `0x8008552C` holds real function pointers: `0x800254C4`
  (func_8003af8c), `0x8002557C` (func_8003b044), etc.
- Function-pointer tables (e.g. `D_8006D4AC`) hold real pointers, not the
  template - so a data table is not directly pointing at the template.

## Investigation So Far (what has been ruled out)

- ISR save state (re-read fresh, `0x80059DC0`): `$t9=0x8003B75C` (NopSub_27e68),
  `$gp=0x800B01D0`, `$ra=0x8007D864` (template), `$v1=0x13` (mode), `$a0=
  0x800C8340` (block+0x30), `$t0=0x8007D86C`, `$s0=0x800C8310` (block),
  `$s2=0x8006F72C` (CdReadyCallback), `$s3=0x8006FB2C`, `$a1=$s1=0x801FFE98`.
- No `lw ra,32(...)` / `lw t9,32(...)` (non-`sp` base) exists anywhere in the
  disassembly - the template value is not being loaded via that pattern.
- No static data in `.main`/`.rdata` contains dword `0x8007D864` (earlier ELF
  hits were in padding/zero beyond loaded sections, not real tables).
- `NopSub_260a4` (0x80052B0C) divides `*(0x1F800000)` by 365, tail-jumps via
  `jr t9` -> `0x8002D90C` (func_80048cfc, rand wrapper). Not the crash path.
- `NopSub_26108` (0x80052B38): if `block[0x18]==0` -> func_8003b20c ->
  `lw t9,68(v0)` = descriptor[0x44] = func_8003b044 -> `jr t9`.
- `CdModeRunTask` (0x80052698) mode==0x13 path: `jalr v0` (v0=0x8003B75C),
  iterates the `D_8006D4AC` function-pointer table (real pointers, terminator
  0).

## Static Verification (2026-08-23)

Cross-checked the runtime claims above against the splat data files. Results:

Confirmed:

- The template word at +8 really is `0x00000001` (`asm/lsdde/data/57070.data.s`),
  which decodes as SPECIAL funct=1, a reserved instruction on the R3000A.
- `func_8003af8c` and `func_8003b044` are real function bodies in
  `asm/lsdde/psyq_SpuSetMute.s`, not empty stubs.
- `D_8006D4AC` holds 14 real function pointers followed by a NULL terminator,
  exactly as described.
- No static data file contains the dword value `0x8007D864`.

New problem found, address space mixing:

- This document mixes runtime addresses from our build with splat-layout
  labels without saying so. They are not the same coordinate system. In the
  splat layout `D_80066828` sits at `0x80066828` and the bytes at `0x8007D864`
  are an unrelated repeating table (`00010005` / `0000FE00`), not the template.
- Assuming the crash site identification is right, our build shifts `.data`
  by roughly `+0x1703C` relative to the splat layout, which is why the template
  shows up at `0x8007D864` at runtime. Text placement can shift independently
  because `lsdde.ld` reorders objects.
- The pairing in "Verified at Runtime" of descriptor value `0x800254C4` with
  `func_8003af8c` does not reconcile: `psyq_SpuSetMute.s` places that function
  at `0x8003AF8C`, and no uniform shift explains both this reference and the
  data shift. Treat that specific mapping as unverified.
- Static content at splat address `0x8008552C` is zeroed, so the descriptor
  values seen at runtime are installed by initialization code and cannot be
  confirmed statically.

Action item: build with `-Wl,-Map` and redo the address bookkeeping in one
coordinate system before trusting the "ruled out" list above.

## Second Static Pass and Fixes Applied (2026-08-23)

Traced every indirect call on the boot path through source and asm.

Confirmed:

- The runtime `.data` shift is real and measured. `block[0] = 0x80084404`
  equals `D_8006D3C8 + 0x1703C` exactly, and `block[0x20] = 0x8007D864`
  equals `D_80066828 + 0x1703C` exactly. Two independent matches, so
  `+0x1703C` is confirmed as our build's `.data` offset versus splat.
- Every call on `EntityAllocSmall -> CdModeInitDream -> func_8003af8c ->
  CdModeRunTask` resolves to valid code statically. `func_8003af8c`
  (psyq_SpuSetMute.s line 5215) was fully decoded: it calls
  `BasicClass__BasicClass` via `GetCoordSystemVtable()+8`, stores
  `&D_8006E4F0` into `obj[0]`, runs one-time init guarded by
  `D_8008A8DC`, calls `CdModeRunTask(mode)` (`func_80026cfc` alias),
  then dispatches `obj->vt[+0x40]`. Nothing on this chain jumps into
  data by itself.
- The ISR register picture (`$t9=NopSub_27e68`, `$v1=0x13`) fits an
  interruption during normal CD mode selection/polling (the
  `func_80026cac` / `CdModePoll` shape), i.e. after the GameState ctor
  finished, which matches the fully constructed block snapshot.

Fixed (source edits only, needs a Windows build to verify):

- `CdModeRunTask` called `CdModeSubD(i, base)` inside the primitive
  walk. That re-copied base slots `[16..21]` and `[26..29]` onto the
  instance before every primitive call, keeping instance offsets
  `+0x40..+0x58` and `+0x68..+0x74` zeroed (statically those hold
  `func_800272C8` .. `func_80027D70`) and wiping whatever a primitive
  had just stored there. Intact asm still dispatches through those
  offsets after boot init: `func_8003b3fc` loads `0x6C(v0)` and calls
  it (psyq_SpuSetMute.s line 5596), and there are 26+ `func_80026cac`
  dispatch sites across the psyq files. Calling through a wiped slot
  transfers control to 0 or garbage, which is exactly the kind of path
  that ends up executing non-code bytes like the template at
  `0x8007D864`, whose word at +8 decodes to funct 1 (RI). `CdModeSubD`
  now runs once before the walk (Entity3_CdMode.c).
- Six real data tables were shadowed by text stubs: `missing_stubs.o`
  links before the `data/` objects and `--allow-multiple-definition`
  keeps the first definition, so the stubs won. Affected symbols:
  `DREAMSYS_METHODS` (DreamSys vtable), `D_800878D4` (GameManager data
  behind `GetGameManager()`), `SPAWN_POS_ADJUST`, `SPECIAL_DAYS`,
  `STAGE_SPAWNPOINTS`, `STAGE_TIME_LIMITS`. While the bug was live,
  `New_DreamSys` and `DreamSys__DreamSys` called `*(vt+8)` / `*(gm+8)`
  through instruction bytes, and gameplay code read instruction
  encodings as table values. The stub definitions were removed from
  missing_stubs.s so the real tables link. Full catalog of duplicate
  resolutions: `docs/Linker_Duplicates.md`.

Not done here:

- No rebuild and no runtime test: the MIPS toolchain currently only
  runs on the Windows side. Both fixes are static edits. Verify with
  the next Windows build (`-Wl,-Map` recommended) and record the
  outcome here, whichever way it goes.

## Next Steps

1. Rebuild on the Windows side with `-Wl,-Map=build_ps1/lsddecomp.map`
   and confirm in the map that `DREAMSYS_METHODS` and `D_800878D4` now
   land inside `.data` (they used to resolve into `.text` stubs).
2. Boot the rebuilt ISO. If the RI storm is gone, note where it gets
   to (license screen or a new blocker). If it still crashes at
   `0x8007D864`, the next suspects are the remaining duplicate-symbol
   resolutions listed in `docs/Linker_Duplicates.md` and the exact
   semantics of the original `func_80026cfc`, whose body was never
   recovered.
3. If a crash persists, breakpoint `func_80026cac` return paths and
   single-step the first dispatch through an instance slot above +0x40,
   capturing registers when `$pc` leaves known code.
4. When register/step reads error out, fall back to reading the ISR save
   block at `0x80059DC0` - it reliably holds the CPU state from the last
   exception.

## Debug Markers (0x800BAFF0-0x800BAF08)

- D0=0x800BAFF0: step counter (1=SetMem, 5=post heap, 6=after func_800206e0,
  7=PadManager, 8/9=vtable 0x44 call, 11=done)
- D8=0x800BAF04: 0xCAFEBABE sentinel written before System_HeapInit; becomes
  0x800C82FC if heap init ran
- D3/D5/D7=heap firstBlock[0] snapshots; D4/D6=free list head snapshots

## Boot Flow (from System.c)

1. `_start`: clears .sbss/.bss, sets heap via BMemPMgrInit at 0x800C82FC,
   installs exception handler (ReinstallHandler), sets up vsync_rel_handler at
   0x800B2CA0 and 0x800B2CE0, then calls main().
2. `main()`: heap init -> game[6]=0 gate -> vtable call (*game + 0x44) ->
   CdInit() -> CdLoadStage2(_gamedata_vaddr, LBA_GAME_BIN, GAME_BIN_SECTORS) ->
   log_gp0_state() -> vtable call (*game + 0x4C). D0 = 1..11 set at each step.
3. After main() returns, `_start` spins at "b 1b" - the game is
   callback/event-driven (VSync + CD callbacks), NOT a main loop.

## Resolved: IRQ2 Storm (2026-08-12)

- Root cause: the I_STAT ack writes were 16-bit `sh`. The CD device
  re-asserted IRQ2 while a pending response sat unread, so the handler
  re-fired on rfe (51,686,592 ISR entries observed) and main() was starved
  before libcd's CdInit could install the real handler.
- Fix: use 32-bit `sw` for both ack sites in cdrom_handler.c (entry ~line
  106-107, end-of-dispatch ~line 212). After a clean boot the ISR counter
  marker at 0x800BAF24 holds at 1. The storm is gone.
- Note: `sh` vs `sw` is semantically identical on this machine; the fix likely
  un-wedged handler timing rather than changing the ack value. Keep `sw`.

## Relevant Code Locations

- `src/lsdde/System.c`: `_start` inline asm (~line 244), main() (~line 330),
  HookEntryInt sets I_MASK=0x00DD, VSyncCallback/InterruptCallback overrides.
- `src/lsdde/cdrom_handler.c`: CdLoadStage2 skip-check, ReinstallHandler,
  EnableCdRomInterrupt, minimal CD callbacks, CdCallbackTable[4].
- `src/lsdde/DreamSys.c`: state machine (~1743 lines), OFFSET_0x4C,
  DreamSys__HandleLinkTransition, AdvanceDay.
- `src/lsdde/Entity3_CdMode.c`: 0x80052320-0x80052B0C (CdModeInitDream,
  NopSub_260a4, NopSub_26108).
- `asm/lsdde/psyq_SpuSetMute.s`: func_8003af8c / func_8003b044.
- `src/lsdde/Sound.c`: NopSub_27e68 (~line 1561).
- `docs/Linker_Duplicates.md`: link order rules and every known
  duplicate symbol resolution.
- `src/lsdde/DreamSys.c`: New_DreamSys (0x800307F4).
- `_bu_init()` is syscall 0x70 - hangs in the BIOS, stubbed no-op
  (System.c ~line 340).
