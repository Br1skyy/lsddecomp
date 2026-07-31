/* loader.s -- first-stage boot loader for lsddecomp.
 *
 * The full game payload (~0xA47E8 bytes) exceeds the BIOS PS-EXE
 * 256-sector (524288 byte) load limit, so the boot EXE on the disc is
 * this tiny loader instead. It opens GAME.BIN (the full payload EXE,
 * header included) via the BIOS VFS B-calls, parses the PS-EXE header
 * inside it, streams the payload to its declared load address, then
 * jumps to its declared entry point (_start).
 *
 * The loader itself is linked at 0x800C0000, well above the payload
 * region (0x80010000..0x800B47E8), so it is not overwritten while the
 * payload streams in.
 */

.set noreorder
.set noat

/* The 2048-byte PS-EXE header is synthesized by build_loader.ps1 (this file is
 * linked as raw code only). Header fields written there:
 *   0x00: "PS-X EXE"   0x10: PC = 0x800C0000   0x18: load = 0x800C0000
 *   0x1C: file size excl header   0x30: sp = 0x801FFFF0
 */

.section .text
.global loader_start
loader_start:
    /* Save callee-saved regs we use (BIOS B-calls clobber R1-R15/R24/R25/RA,
     * so fd/load/size/entry/loaded live in s0-s4 which the BIOS preserves). */
    addiu   $sp, $sp, -32
    sw      $ra, 28($sp)
    sw      $s0, 24($sp)
    sw      $s1, 20($sp)
    sw      $s2, 16($sp)
    sw      $s3, 12($sp)
    sw      $s4,  8($sp)

    /* D0 = 1 (loader entered) */
    lui     $t0, 0x800B
    addiu   $t0, $t0, 0xAFF0
    addiu   $t1, $zero, 1
    sw      $t1, 0($t0)

    /* fd = open("cdrom0:\GAME.BIN;1", 1) */
    lui     $a0, %hi(fname)
    addiu   $a0, $a0, %lo(fname)
    addiu   $a1, $zero, 1     /* accessmode: read */
    jal     open_b
    nop
    bltz    $v0, fail_loop
    nop
    move    $s0, $v0          /* s0 = fd */

    /* D0 = 2 (open ok) */
    lui     $t0, 0x800B
    addiu   $t0, $t0, 0xAFF0
    addiu   $t1, $zero, 2
    sw      $t1, 0($t0)

    /* read 2048-byte PS-EXE header into hdr_buf */
    move    $a0, $s0
    lui     $a1, %hi(hdr_buf)
    addiu   $a1, $a1, %lo(hdr_buf)
    addiu   $a2, $zero, 0x800
    jal     read_b
    nop
    bltz    $v0, fail_loop
    nop

    /* D0 = 3 (header read ok) */
    lui     $t0, 0x800B
    addiu   $t0, $t0, 0xAFF0
    addiu   $t1, $zero, 3
    sw      $t1, 0($t0)

    /* parse header: load_addr @0x18, size @0x1C, entry @0x10 */
    lui     $t0, %hi(hdr_buf)
    addiu   $t0, $t0, %lo(hdr_buf)
    lw      $s1, 0x18($t0)    /* s1 = load_addr */
    lw      $s2, 0x1C($t0)    /* s2 = payload size */
    lw      $s3, 0x10($t0)    /* s3 = entry PC */
    addiu   $s4, $zero, 0     /* s4 = bytes loaded */

    /* stream payload: loop read(fd, load_addr+loaded, min(0x800, remaining)) */
load_loop:
    sltu    $t0, $s4, $s2
    beqz    $t0, load_done
    nop
    subu    $t2, $s2, $s4     /* remaining */
    sltiu   $t5, $t2, 0x800   /* remaining < 0x800 ? */
    bnez    $t5, 1f
    nop
    addiu   $t2, $zero, 0x800 /* chunk = 0x800 */
1:
    move    $a0, $s0          /* fd */
    addu    $a1, $s1, $s4     /* dst = load_addr + loaded */
    move    $a2, $t2          /* chunk */
    jal     read_b
    nop
    blez    $v0, fail_loop    /* EOF/error before payload complete */
    nop
    addu    $s4, $s4, $v0
    b       load_loop
    nop

load_done:
    /* close(fd) */
    move    $a0, $s0
    jal     close_b
    nop

    /* D0 = 0xDD, D1 = 1 (payload loaded; matches CdLoadStage2 marker) */
    lui     $t0, 0x800B
    addiu   $t0, $t0, 0xAFF0
    addiu   $t1, $zero, 0xDD
    sw      $t1, 0($t0)
    addiu   $t1, $zero, 1
    sw      $t1, 4($t0)

    /* invalidate instruction cache for payload region using BIOS A(44h) */
    move    $a0, $s1          /* load_addr */
    move    $a1, $s2          /* size */
    li      $t1, 0x44         /* A(44h) = FlushCache */
    syscall 0xA0

    /* restore and jump to payload entry */
    lw      $ra, 28($sp)
    lw      $s0, 24($sp)
    lw      $s1, 20($sp)
    lw      $s2, 16($sp)
    lw      $s3, 12($sp)
    lw      $s4,  8($sp)
    addiu   $sp, $sp, 32
    jr      $s3
    nop

fail_loop:
    /* D0 = 0xEE, D1 = failing fd/rc, then hang */
    lui     $t0, 0x800B
    addiu   $t0, $t0, 0xAFF0
    addiu   $t1, $zero, 0xEE
    sw      $t1, 0($t0)
    sw      $v0, 4($t0)
1:  b       1b
    nop

/* ---- BIOS B-vector wrappers (correct PS1 BIOS syscall implementation) ---- */
open_b:
    li      $t1, 0x32         /* B(32h) = open */
    syscall 0xA0
    jr      $ra
    nop

read_b:
    li      $t1, 0x34         /* B(34h) = read */
    syscall 0xA0
    jr      $ra
    nop

close_b:
    li      $t1, 0x36         /* B(36h) = close */
    syscall 0xA0
    jr      $ra
    nop

.balign 4
fname:
    .ascii  "cdrom:GAME.BIN;1"
    .byte   0

.balign 4
hdr_buf:
    .space  2048
