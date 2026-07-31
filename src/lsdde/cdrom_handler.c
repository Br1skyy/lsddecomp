#include "common.h"
#include "dat_globals.h"
#include <libcd.h>
/* VSync callback table (defined in System.c) */
extern void *VSyncCallbackTable[16];
/* Interrupt callback table / IntrTab (defined in System.c) */
extern void *IntrCallbackTable[16];
__asm__(
    ".set noreorder\n"
    ".set noat\n"
    ".section .text\n"
    ".balign 4\n"
    ".globl saved_sp\n"
    "saved_sp:\n"
    "    .space 4\n"
    ".balign 8\n"
    "isr_stack:\n"
    "    .space 2048\n"
    "isr_stack_end:\n"
    ".balign 32\n"
    ".globl bfc_skip_count\n"
    "bfc_skip_count:\n"
    "    .space 4\n"
    ".globl handler_saved_flag\n"
    "handler_saved_flag:\n"
    "    .space 4\n"
    ".balign 4\n"
    ".globl bios_handler_addr\n"
    "bios_handler_addr:\n"
    "    .space 4\n"
    ".balign 4\n"
    ".globl Vcount\n"
    "Vcount:\n"
    "    .space 4\n"
);
extern u32 bfc_skip_count;
__asm__(
    ".globl exception_handler\n"
    ".ent exception_handler\n"
    "exception_handler:\n"
    "    mfc0    $k1, $13\n"
    "    nop\n"
    "    andi    $k1, $k1, 0x007C\n"
    "    bnez    $k1, non_interrupt\n"
    "     nop\n"
    "    lui     $k0, %hi(saved_sp)\n"
    "    sw      $sp, %lo(saved_sp)($k0)\n"
    "    lui     $sp, %hi(isr_stack_end)\n"
    "    addiu   $sp, $sp, %lo(isr_stack_end)\n"
    "    addiu   $sp, $sp, -128\n"
    "    sw      $at,  0($sp)\n"
    "    sw      $v0,  4($sp)\n"
    "    sw      $v1,  8($sp)\n"
    "    sw      $a0, 12($sp)\n"
    "    sw      $a1, 16($sp)\n"
    "    sw      $a2, 20($sp)\n"
    "    sw      $a3, 24($sp)\n"
    "    sw      $t0, 28($sp)\n"
    "    sw      $t1, 32($sp)\n"
    "    sw      $t2, 36($sp)\n"
    "    sw      $t3, 40($sp)\n"
    "    sw      $t4, 44($sp)\n"
    "    sw      $t5, 48($sp)\n"
    "    sw      $t6, 52($sp)\n"
    "    sw      $t7, 56($sp)\n"
    "    sw      $s0, 60($sp)\n"
    "    sw      $s1, 64($sp)\n"
    "    sw      $s2, 68($sp)\n"
    "    sw      $s3, 72($sp)\n"
    "    sw      $s4, 76($sp)\n"
    "    sw      $s5, 80($sp)\n"
    "    sw      $s6, 84($sp)\n"
    "    sw      $s7, 88($sp)\n"
    "    sw      $t8, 92($sp)\n"
    "    sw      $t9, 96($sp)\n"
    "    sw      $gp, 100($sp)\n"
    "    sw      $ra, 104($sp)\n"
    "    lui     $s0, 0x1F80\n"
    "    lhu     $s1, 0x1070($s0)\n"
    "    lhu     $s2, 0x1074($s0)\n"
    "    and     $s5, $s1, $s2\n"
    "    beqz    $s5, restore_context\n"
    "     nop\n"
    "    la      $s3, IntrCallbackTable\n"
    "    move    $s1, $s5\n"
    "    li      $s4, 0\n"
    "bit_loop:\n"
    "    andi    $t0, $s1, 1\n"
    "    beqz    $t0, skip_bit\n"
    "     nop\n"
    "dispatch_intrtab:\n"
    "    sll     $t0, $s4, 2\n"
    "    addu    $t0, $s3, $t0\n"
    "    lw      $t0, 0($t0)\n"
    "    beqz    $t0, skip_bit\n"
    "     nop\n"
    "    move    $a0, $zero\n"
    "    move    $t8, $ra\n"
    "    jalr    $t0\n"
    "     nop\n"
    "    move    $ra, $t8\n"
    "skip_bit:\n"
    "    srl     $s1, $s1, 1\n"
    "    addiu   $s4, $s4, 1\n"
    "    slti    $t0, $s4, 16\n"
    "    bnez    $t0, bit_loop\n"
    "     nop\n"
    "    sh      $s5, 0x1070($s0)\n"
    "restore_context:\n"
    "    lw      $at,  0($sp)\n"
    "    lw      $v0,  4($sp)\n"
    "    lw      $v1,  8($sp)\n"
    "    lw      $a0, 12($sp)\n"
    "    lw      $a1, 16($sp)\n"
    "    lw      $a2, 20($sp)\n"
    "    lw      $a3, 24($sp)\n"
    "    lw      $t0, 28($sp)\n"
    "    lw      $t1, 32($sp)\n"
    "    lw      $t2, 36($sp)\n"
    "    lw      $t3, 40($sp)\n"
    "    lw      $t4, 44($sp)\n"
    "    lw      $t5, 48($sp)\n"
    "    lw      $t6, 52($sp)\n"
    "    lw      $t7, 56($sp)\n"
    "    lw      $s0, 60($sp)\n"
    "    lw      $s1, 64($sp)\n"
    "    lw      $s2, 68($sp)\n"
    "    lw      $s3, 72($sp)\n"
    "    lw      $s4, 76($sp)\n"
    "    lw      $s5, 80($sp)\n"
    "    lw      $s6, 84($sp)\n"
    "    lw      $s7, 88($sp)\n"
    "    lw      $t8, 92($sp)\n"
    "    lw      $t9, 96($sp)\n"
    "    lw      $gp, 100($sp)\n"
    "    lw      $ra, 104($sp)\n"
    "    lui     $k0, %hi(saved_sp)\n"
    "    lw      $sp, %lo(saved_sp)($k0)\n"
    "    .word   0x42000010\n"
    "     nop\n"
    "non_interrupt:\n"
    "    lui     $k0, %hi(bios_handler_addr)\n"
    "    lw      $k0, %lo(bios_handler_addr)($k0)\n"
    "    nop\n"
    "    jr      $k0\n"
    "     nop\n"
    ".end exception_handler\n"
);
__asm__(
    ".ent install_jump\n"
    "install_jump:\n"
    "    lui     $t3, 0x8000\n"
    "    lw      $t3, 0x0080($t3)\n"
    "    sll     $t4, $t3, 6\n"
    "    srl     $t4, $t4, 6\n"
    "    sll     $t4, $t4, 2\n"
    "    lui     $t5, 0x8000\n"
    "    or      $t4, $t4, $t5\n"
    "    lui     $t5, %hi(bios_handler_addr)\n"
    "    sw      $t4, %lo(bios_handler_addr)($t5)\n"
    "    lui     $t0, %hi(exception_handler)\n"
    "    addiu   $t0, $t0, %lo(exception_handler)\n"
    "    srl     $t0, $t0, 2\n"
    "    lui     $at, 0x03FF\n"
    "    ori     $at, $at, 0xFFFF\n"
    "    and     $t0, $t0, $at\n"
    "    lui     $t1, 0x0800\n"
    "    or      $t0, $t0, $t1\n"
    "    lui     $t1, 0xA000\n"
    "    sw      $t0, 0x0080($t1)\n"
    "    lui     $t1, 0x8000\n"
    "    sw      $t0, 0x0080($t1)\n"
    "    lui     $t2, 0x1F80\n"
    "    lw      $t0, 0x1000($t2)\n"
    "    addiu   $t1, $zero, 1\n"
    "    xor     $t0, $t0, $t1\n"
    "    sw      $t0, 0x1000($t2)\n"
    "    xor     $t0, $t0, $t1\n"
    "    sw      $t0, 0x1000($t2)\n"
    "    jr      $ra\n"
    "     nop\n"
    ".end install_jump\n"
);
void ReinstallHandler(void)
{
    __asm__ volatile(
        "addiu   $sp, $sp, -16\n"
        "sw      $ra, 12($sp)\n"
        "sw      $s0, 8($sp)\n"
        "jal     install_jump\n"
        "nop\n"
        "lui     $s0, 0x1F80\n"
        "lhu     $t0, 0x1074($s0)\n"
        "ori     $t0, $t0, 0x001D\n"
        "sh      $t0, 0x1074($s0)\n"
        "lw      $s0, 8($sp)\n"
        "lw      $ra, 12($sp)\n"
        "addiu   $sp, $sp, 16\n"
        "jr      $ra\n"
        "nop\n"
    );
}
__asm__(
    ".globl startIntrVSync\n"
    ".ent startIntrVSync\n"
    "startIntrVSync:\n"
    "    lui     $t0, 0x1F80\n"
    "    sh      $zero, 0x1110($t0)\n"
    "    li      $t1, 0x030E\n"
    "    sh      $t1, 0x1114($t0)\n"
    "    li      $t1, 0x0107\n"
    "    sh      $t1, 0x1118($t0)\n"
    "    jr      $ra\n"
    "    nop\n"
    ".end startIntrVSync\n"
);
void EnableCdRomInterrupt(void)
{
    __asm__ volatile(
        "addiu   $sp, $sp, -8\n"
        "sw      $ra, 0($sp)\n"
        "jal     drain_response_fifo\n"
        "nop\n"
        "lui     $t0, 0x1F80\n"
        "lhu     $t1, 0x1070($t0)\n"
        "andi    $t1, $t1, 0x0004\n"
        "beq     $t1, $zero, 1f\n"
        "nop\n"
        "sh      $t1, 0x1070($t0)\n"
        "1:\n"
        "lhu     $t1, 0x1074($t0)\n"
        "ori     $t1, $t1, 0x0004\n"
        "sh      $t1, 0x1074($t0)\n"
        "lw      $ra, 0($sp)\n"
        "addiu   $sp, $sp, 8\n"
        "jr      $ra\n"
        "nop\n"
    );
}
void disable_cdrom_hw_irq(void)
{
    __asm__ volatile(
        "lui     $t0, 0x1F80\n"
        "li      $t1, 3\n"
        "sb      $t1, 0($t0)\n"
        "sb      $zero, 1($t0)\n"
        "sb      $zero, 0($t0)\n"
        "li      $t1, 0x0004\n"
        "sh      $t1, 0x1070($t0)\n"
        "jr      $ra\n"
        "nop\n"
    );
}
__asm__(
    ".ent drain_response_fifo\n"
    "drain_response_fifo:\n"
    "    lui     $t0, 0x1F80\n"
    "    sb      $zero, 0($t0)\n"
    "1:\n"
    "    lbu     $t1, 0($t0)\n"
    "    nop\n"
    "    andi    $t2, $t1, 0x08\n"
    "    beqz    $t2, 2f\n"
    "    nop\n"
    "    lbu     $t1, 1($t0)\n"
    "    nop\n"
    "    b       1b\n"
    "    nop\n"
    "2:\n"
    "    jr      $ra\n"
    "    nop\n"
    ".end drain_response_fifo\n"
);
__asm__(
    ".globl vsync_rel_handler\n"
    ".ent vsync_rel_handler\n"
    "vsync_rel_handler:\n"
    "    bnez    $a0, vsync_register\n"
    "     nop\n"
    "    addiu   $sp, $sp, -12\n"
    "    sw      $ra, 0($sp)\n"
    "    lui     $t0, %hi(Vcount)\n"
    "    lw      $t1, %lo(Vcount)($t0)\n"
    "    addiu   $t1, $t1, 1\n"
    "    sw      $t1, %lo(Vcount)($t0)\n"
    "    la      $s0, VSyncCallbackTable\n"
    "    li      $t1, 16\n"
    "    sw      $s0, 4($sp)\n"
    "0:\n"
    "    lw      $t2, 0($s0)\n"
    "    beqz    $t2, 1f\n"
    "     nop\n"
    "    sw      $t1, 8($sp)\n"
    "    jalr    $t2\n"
    "     nop\n"
    "    lw      $t1, 8($sp)\n"
    "1:\n"
    "    addiu   $s0, $s0, 4\n"
    "    addiu   $t1, $t1, -1\n"
    "    bgtz    $t1, 0b\n"
    "     nop\n"
    "    lw      $ra, 0($sp)\n"
    "    lw      $s0, 4($sp)\n"
    "    addiu   $sp, $sp, 12\n"
    "    jr      $ra\n"
    "     nop\n"
    "vsync_register:\n"
    "    la      $t0, VSyncCallbackTable\n"
    "    sll     $a0, $a0, 2\n"
    "    addu    $t0, $t0, $a0\n"
    "    lw      $v0, 0($t0)\n"
    "    sw      $a1, 0($t0)\n"
    "    jr      $ra\n"
    "     nop\n"
    ".end vsync_rel_handler\n"
);
/* Minimal CD callbacks for PS-EXE (before GAME.BIN overlay loads) */
volatile int cd_data_ready = 0;
volatile int cd_ready_complete = 0;
volatile int cd_sync_complete = 0;
void cd_data_callback(void)
{
    cd_data_ready = 1;
}
void cd_ready_callback(u_char intr, u_char *result)
{
    cd_ready_complete = 1;
}
void cd_sync_callback(u_char intr, u_char *result)
{
    cd_sync_complete = 1;
}
/* === migrated from stubs.c: CdModeStopReading === */
void CdModeStopReading(void) { if (CdModeSelector == CD_MODE_STOPPED) DisableCdRead(); }
/* === migrated from stubs.c: CdGetFilePathPrefix === */
void *CdGetFilePathPrefix(void) { return Cd_FilePathPrefix; }
/* === migrated from stubs.c: CdDaMode_PollVsync === */
int CdDaMode_PollVsync(void) { return 0; }

/* === extern declarations for CD functions (moved from stubs.c) === */
extern int GetCdDaModeVtable(void);
extern int GetCdDaModeVtableEx(void);
extern int ReadCdByte(void);
extern void WriteCdByte(int a, int b);
extern int Ptr_CdDaModeTable[];
extern void EnableCdRead(void);
extern void DisableCdRead(void);
extern void CdResetState(void);
/* === migrated from stubs.c === */
/* CdModeRunTask */
void CdModeRunTask(int mode)
{
    int vtab;
    int unknown;
    if (mode == CD_MODE_STOPPED)
        vtab = GetCdDaModeVtable();
    else
        vtab = GetCdDaModeVtableEx();
    unknown = ReadCdByte();
    for (int i = 0; ; i++) {
        WriteCdByte(unknown, vtab);
        int (*fn)() = (int (*)())Ptr_CdDaModeTable[i];
        if (fn == 0) break;
        unknown = fn();
    }
}
/* CdModePoll */
void CdModePoll(void)
{
    if (CdModeSelector == CD_MODE_STARTING)
        GetCdDaModeVtableEx();
    else
        GetCdDaModeVtable();
}
/* CdModeStoreTriplet */
void CdModeStoreTriplet(int *dst, int a, int b, int c)
{
    dst[0] = a;
    dst[1] = b;
    dst[2] = c;
}
/* CardWriteRead */
int CardWriteRead(int base, int size)
{
    (void)base; (void)size;
    return (int)_card_load(0);
}
/* CdDaMode_Op */
void *CdDaMode_Op(void *mode, int op)
{
    (void)mode; (void)op;
    return (void *)0x8006d9bc;
}
/* CdDaMode_CheckWait */
int CdDaMode_CheckWait(int mode)
{
    (void)mode;
    return CdSync(0, 0) >= 0 ? 1 : 0;
}
/* CdDaMode_CheckComplete */
int CdDaMode_CheckComplete(void)
{
    u_char result;
    return CdSync(0, &result) >= 0 ? 1 : 0;
}
/* CdReadMode_GetVtable */
void *CdReadMode_GetVtable(void)
{
    return (void *)0x8006d4e8;
}
/* CdReadMode_Op */
void *CdReadMode_Op(void *mode, int op)
{
    (void)mode; (void)op;
    return (void *)0x8006d9bc;
}
/* CdReadMode_WaitReady */
int CdReadMode_WaitReady(int width, int height)
{
    extern unsigned int Cd_ReadModeWidth, Cd_ReadModeHeight;
    Cd_ReadModeWidth = width;
    Cd_ReadModeHeight = height;
    return 1;
}
/* CdReadComplete */
int CdReadComplete(void)
{
    CdResetState();
    return 0;
}

/* === merged from cd_stage2.c === */
/* SECTOR_SIZE from libcd.h is in words (512 words = 2048 bytes) */
#define CD_SECTOR_BYTES 2048
/* CD-ROM hardware registers at 0x1F801800
 *   0: Status(R) / Command(W)
 *   1: Response(R) / Parameter(W)
 *   2: Data(R/W)
 */
#define CD_BASE          ((volatile u8 *)0x1F801800)
#define CD_STATUS        (CD_BASE[0x00])
#define CD_CMD           (CD_BASE[0x00])
#define CD_RESPONSE      (CD_BASE[0x01])
#define CD_PARAM         (CD_BASE[0x01])
#define CD_DATA          (CD_BASE[0x02])
/* Status register bits */
#define ST_BUSY     0x20    /* BUSYSTS */
#define ST_RESP     0x08    /* RSLRRDY: response available */
#define ST_DATA     0x10    /* DRQSTS: data available */
#define ST_PRM      0x04    /* PRMWRDY: can write parameter */
/* CD-ROM commands */
#define CMD_SETLOC  0x02
#define CMD_READN   0x06
#define CMD_SEEKL   0x15
#define CMD_MOTORON 0x0D
#define CMD_STANDBY 0x10
#define CMD_GETID   0x14
#define CMD_DEMUTE  0x1E
/* Wait until ready to accept parameter */
static void cd_wait_prm(void)
{
    while (!(CD_STATUS & ST_PRM));
}
/* Send command with optional parameters */
static void cd_send_cmd(u8 cmd, int nparam, const u8 *params)
{
    int i;
    while (CD_STATUS & ST_BUSY);
    CD_CMD = cmd;
    for (i = 0; i < nparam; i++) {
        cd_wait_prm();
        CD_PARAM = params[i];
    }
}
/* Read one sector (2048 bytes) from data port */
static void cd_read_sector(u8 *dst)
{
    int i;
    for (i = 0; i < 2048; i++) {
        while (!(CD_STATUS & ST_DATA));
        dst[i] = CD_DATA;
    }
}
void CdLoadStage2(void *dest, u32 lba, u32 count)
{
    volatile u32 *D0 = (volatile u32 *)0x800BAFF0;
    volatile u32 *D1 = (volatile u32 *)0x800BAFF4;
    /* PS-EXE now embeds all data beyond 256-sector limit. If the first
       word of the destination is already non-zero, data was loaded by
       the EXE loader and we skip CD operations entirely. */
    if (*(volatile u32 *)dest != 0) {
        *D0 = 0xDD; *D1 = 1;  /* marker: data already loaded, skipping CdLoadStage2 */
        return;
    }
    CdlLOC loc;
    u8 result[8];
    u8 mode;
    u32 sector;
    u32 *dst = (u32 *)dest;
    *D0 = 0xDD; *D1 = 2;  /* marker: CdLoadStage2 starting CD read */
    mode = CdlModeSpeed;
    CdControl(CdlSetmode, &mode, result);
    CdSync(0, result);
    CdIntToPos((int)lba, &loc);
    CdControl(CdlSetloc, (u_char *)&loc, result);
    CdSync(0, result);
    CdControl(CdlReadN, 0, result);
    for (sector = 0; sector < count; sector++) {
        volatile int cd_timeout = 500000;
        while (CdDataSync(1) > 0 && --cd_timeout > 0)
            ;
        if (cd_timeout <= 0) {
            *D0 = 0xCD; *D1 = sector;
            __asm__ volatile("1: b 1b\n nop");
        }
        CdGetSector(dst, 512);
        dst += 512;
    }
    CdControl(CdlPause, 0, result);
    CdSync(0, result);
    *D0 = 0xDD; *D1 = 3;  /* marker: CdLoadStage2 complete */
}

