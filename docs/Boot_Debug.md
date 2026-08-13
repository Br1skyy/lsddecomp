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

## Next Steps

1. Find the control-transfer that reaches the template. Since `$ra=0x8007D864`
   at the ISR save, hunt for the call/return that put the template address in
   `$ra` - likely a `jalr`/`jr` through a block/descriptor field, or a
   corruption of the stack/return address earlier in the chain.
2. Set breakpoints earlier in the chain (CdModeInitDream entry, func_8003af8c,
   CdModeRunTask) and single-step forward with register capture until `$ra`
   becomes the template.
3. Cross-check the descriptor/block field layout against the C in
   `Entity3_CdMode.c` / `class_39e08.c` - a wrong struct offset in our
   reconstruction could feed a data value where a code pointer belongs.
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
- `src/lsdde/DreamSys.c`: New_DreamSys (0x800307F4).
- `_bu_init()` is syscall 0x70 - hangs in the BIOS, stubbed no-op
  (System.c ~line 340).
