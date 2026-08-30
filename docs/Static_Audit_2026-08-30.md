# Static Audit 2026-08-30 - Boot Path Without Runtime

No Ghidra, no emulator - pure `asm/` + `src/` + `build_ps1/lsddecomp.map` (verified `7778757`).

## 1. func_8003af8c (0x8003AF8C, `asm/lsdde/psyq_SpuSetMute.s:5215`) vs `src/lsdde/Entity3_CdMode.c:280` `CdModeInitDream`

Disasm (`psyq_SpuSetMute.s:5215-5255`):
```
jal func_80018390; lw v0,8(v0); jalr v0(a0=s0)      ; BasicClass__BasicClass(obj)
jal func_8003b20c                                  ; &D_8006E4F0 -> sw 0(s0)
lw v1, D_8008A8DC; bnez v1, L; sw v0,0(s0)          ; one-time guard
jal func_8003b294                                  ; init if first
L: addu a0,s1; jal func_80026cfc; sw zero,0x18(s0) ; CdModeRunTask(mode in s1, zero 0x18)
lw v0,0(a0=s0); lw v0,0x40(v0); jalr v0             ; obj->vt[0x40](obj,0)
```

C at `src/lsdde/Entity3_CdMode.c:280` matches exactly: `func_80018390` via `GetCoordSystemVtable()+8`, `func_8003b20c` (=`&D_8006E4F0`), guard `D_8008A8DC`, `func_80026cfc` before final `vt[0x40]`. Single `CdModeSubD(i,base)` before walk at `src/lsdde/Entity3_CdMode.c:118` is correct shape - original does **one** `jal func_80026cfc` then one `0x40` dispatch, not a loop wipe. Old bug (CdModeSubD inside `D_8006D4AC` walk) would zero `+0x40..0x58` after every primitive, matching RI at `0x8007D864` (`+8=0x00000001`).

Ghidra not needed - `psyq_SpuSetMute.s` is the source. Ghidra would only add xref graph for `func_80026cfc` alias (`lsdde_defsyms.ld:238`).

## 2. 26x + 6x dispatch audit

* `grep -r func_80026cac asm/lsdde --include=*.s` → 26 hits (2 in `psyq_SpuSetMute.s:5582,5615`, 24 in `psyq_memset.s:32B40..39274`). All are `jal func_80026cac` = `CdModePoll` (`lsdde_defsyms.ld:238`).
* `grep -r "0x6C(\$v0)" asm --include=*.s` → 6 hits (`psyq_SpuSetMute.s:5596` `lw v0,0x6C(v0); jalr`, `psyq_memset.s:32B80,34128,34438,34750,34F84,355E4` etc) = instance `+0x6C` dispatch (statically `func_800272C8..func_80027D70` when table intact). If `CdModeSubD` wiped `+0x40..0x58`/`+0x68..0x74`, these `jalr` through `0` or garbage land in data like `0x8007D864`.

Boot-ranked at `docs/Linker_Duplicates.md:111` #1. No new sites beyond those 32.

## 3. EntityAllocSmall divergence

`src/lsdde/Entity2.c:1311` vs `src/lsdde/Entity3_CdMode.c:269`:
```c
int EntityAllocSmall(int type){ int obj=MemAllocImpl(0x2c); if(obj){int vtab=NopSub_269e0(); (*(code*)(vtab+8))(obj,type);} return obj; }
```
Diff is only `int obj; int vtab;` vs `int obj = ...; int vtab = ...` - identical body. Winner `Entity2.c.o` alphabetical (`nm` `T 00002ce4` vs `T 000006bc`). No behavioral divergence today, but keep in sync or delete one to kill `Entity3_CdMode.c.o` vs `Entity2.c.o` duplicate (`docs/Linker_Duplicates.md:111` #2) - it **is** on `main->CdInitRom->EntityAllocSmall` crash path.

No Ghidra needed for any of the 3 - all via `asm/` + `src/` + `build_ps1/*.map`.
