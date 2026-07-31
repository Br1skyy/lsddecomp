#ifndef PSX_TYPES_H
#define PSX_TYPES_H

/* Minimal PS1 type definitions needed before PsyQ headers.
   PsyQ headers (libgte.h, libgpu.h) define the full SVECTOR, VECTOR, MATRIX, etc. */

typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
#ifndef u_long
typedef unsigned long u_long;
#endif

#endif
