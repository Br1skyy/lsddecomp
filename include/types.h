#ifndef TYPES_H
#define TYPES_H

/* Standard integer types (only define if common.h didn't already) */
#ifndef COMMON_H
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
#endif

/* stdint-style types */
typedef char int8_t;
typedef short int16_t;
typedef int int32_t;
typedef long long int64_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

/* float types */
typedef float f32;
typedef double f64;

/* bool -- provided by common.h / ghidra_compat.h */

#ifndef NULL
#define NULL (0)
#endif

#endif
