.include "macro.inc"
.include "lba_table.inc"

.set noat
.set noreorder

.section .text, "ax"

/* _start, main game init, and log_gp0_state are now implemented
 * in src/lsdde/System.c. This file is kept empty to suppress the
 * auto-build inclusion of the ASM version, which would override the C
 * implementation via --allow-multiple-definition. */
