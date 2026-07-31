.include "macro.inc"

.set noat
.set noreorder

.section .text, "ax"

/* CdLoadStage2 and lba_to_msf are now implemented in src/lsdde/cdrom_handler.c.
 * This file is kept empty to suppress the auto-build inclusion of the ASM version,
 * which would override the C implementation via --allow-multiple-definition. */
