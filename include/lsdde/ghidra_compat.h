#ifndef GHIDRA_COMPAT_H
#define GHIDRA_COMPAT_H

#include "lsdde/psx_types.h"

typedef unsigned char  undefined;
typedef unsigned char  undefined1;
typedef unsigned short undefined2;
typedef unsigned int   undefined4;
typedef unsigned long  undefined8;

/* Ghidra function-pointer pattern: (**(code **)(base+off))(args).
   Returns int so the same type works for both statement context (discarding
   the return value) and expression context (assigning the result). */
typedef int  (*code)();
typedef int  (**codeptr)();
typedef int  (*_CODE)();
typedef int  (*code_ret)();

/* Vtable function pointer: function taking 4 unspecified args.
   Used by Ghidra-decompiled code for vtable dispatch. */
typedef int  (*VtableFn_4)();

#define BYTE_PTR(p) ((unsigned char *)(int)(p))
#define BOFF(p, off) (*(unsigned char *)((int)(p) + (off)))

#define CONCAT11(h, l)  ((unsigned char)((unsigned char)(h) | (unsigned char)(l)))
#define CONCAT22(h, l)  ((unsigned int)((unsigned short)(h) << 16) | (unsigned short)(l))
#define CONCAT31(h, l)  ((unsigned int)((unsigned long)(h) << 8) | (unsigned char)(l))
#define CONCAT44(h, l)  ((unsigned long)((unsigned long)(h) << 32) | (unsigned int)(l))
#define CONCAT21(h, l)  ((unsigned int)((unsigned char)(h) << 16) | (unsigned short)(l))

/* Byte access matching Ghidra's _0_1_/_1_1_ on undefined2/uint values */
#define B0(v) (*(unsigned char *)&(v))
#define B1(v) (*((unsigned char *)&(v) + 1))

#define HWORD(v) ((unsigned short)((unsigned int)(v) >> 16))
#define LOWORD(v) ((unsigned short)((unsigned int)(v) & 0xFFFF))

/* trap(x) -> inline break instruction (Ghidra generates trap(imm) for break codes) */
#define trap(x) __asm__ volatile("break")

/* Ghidra ULONGLONG / LONGLONG types */
typedef unsigned long long ulonglong;
typedef long long longlong;

#endif
