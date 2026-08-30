#ifndef COMMON_H
#define COMMON_H

/* ------------------------------------------------------------------ */
/* Ghidra decompiler compatibility                                     */
/* "code" is Ghidra's internal type for executable code.              */
/* code *  ==  void (*)(void)   - a function pointer                  */
/* code ** ==  void (**)(void)  - pointer to function pointer         */
/* ------------------------------------------------------------------ */
#include "lsdde/ghidra_compat.h"

/* ------------------------------------------------------------------ */
/* Standard integer types (PS1 is 32-bit: int/long = 32 bits)         */
/* ------------------------------------------------------------------ */
typedef signed char        s8;
typedef unsigned char      u8;
typedef signed short       s16;
typedef unsigned short     u16;
typedef signed int         s32;
typedef unsigned int       u32;
typedef signed long long   s64;
typedef unsigned long long u64;

/* ------------------------------------------------------------------ */
/* Ghidra decompiler compatibility types                               */
/* ------------------------------------------------------------------ */
typedef unsigned char byte;
typedef unsigned int  uint;
typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int  dword;

/* bool, true, false via <stdbool.h> (must be before DreamSys.h) */
#include <stdbool.h>

/* ------------------------------------------------------------------ */
/* LSD-specific types (must be defined before DreamSys.h / LocationGrid.h) */
/* ------------------------------------------------------------------ */
typedef union MoodGraphPoint {
    s16 value;
    struct axis {
        s8 dynamic;
        s8 upper;
    } axis;
} MoodGraphPoint;

/* ------------------------------------------------------------------ */
/* Original LSD header files                                          */
/* ------------------------------------------------------------------ */
#include "types.h"
#include "DreamSys.h"
#include "LocationGrid.h"

/* ------------------------------------------------------------------ */
#include "lsdde/magic_numbers.h"

#define SPU_FRAME_SIZE 0x8000

#endif /* COMMON_H */
