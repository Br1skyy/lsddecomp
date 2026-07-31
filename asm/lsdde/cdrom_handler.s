.set noreorder
.set noat
.section .text

/* Interrupt handler, CD-ROM mode handler, and stage2 loader are implemented
 * in src/lsdde/cdrom_handler.c. This file is kept empty to suppress the
 * auto-build inclusion of the ASM version, which would override the C
 * implementation via --allow-multiple-definition. */

/* saved_sp, isr_stack, bfc_skip_count, handler_saved_flag */
.balign 4
.globl saved_sp
saved_sp:
    .space 4
.balign 8
isr_stack:
    .space 2048
isr_stack_end:
.balign 4
.globl bfc_skip_count
bfc_skip_count:
    .space 4
.globl handler_saved_flag
handler_saved_flag:
    .space 4
