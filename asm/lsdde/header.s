.section .data
.global header_start
header_start:

/* PS-EXE header (standard Sony PlayStation format) */
/* See nocash PSX specs: https://problemkaputt.de/psx-spx.htm#cdromfileexecution */

.ascii  "PS-X EXE"         /* 0x00: Magic number (8 bytes) */
.word   0x00000000         /* 0x08: Reserved (should be zero) */
.word   0x00000000         /* 0x0C: Reserved (should be zero) */

.word   0x00000000         /* 0x10: Initial PC (entry point) -- patched by build script */
.word   0x00000000         /* 0x14: Initial $gp/r28 */
.word   0x80010000         /* 0x18: Load address (destination in RAM) = base of .text */
.word   0x00000000         /* 0x1C: File size (excluding 2048-byte header) -- patched by build script */
.word   0x00000000         /* 0x20: Reserved (memfill addr in some docs) */
.word   0x00000000         /* 0x24: Reserved (memfill size in some docs) */
.word   0x00000000         /* 0x28: Reserved */
.word   0x00000000         /* 0x2C: Reserved */
.word   0x801FFFF0         /* 0x30: Initial stack pointer */
.word   0x00000000         /* 0x34: Stack pointer offset */
.word   0x00000000         /* 0x38: Reserved */
.word   0x00000000         /* 0x3C: Reserved */
.word   0x00000000         /* 0x40: Reserved */
.word   0x00000000         /* 0x44: Reserved */
.word   0x00000000         /* 0x48: Reserved */
.word   0x00000000         /* 0x4C: Reserved */

/* Fill remaining header bytes with Sony copyright string + zeros */
.ascii  "Sony Computer Entertainment Inc. for Japan area"

.balign 2048, 0
header_end:
