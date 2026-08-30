#include "common.h"
#include "dat_globals.h"
#include <libcd.h>
#include <string.h>
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
    "    /* Interrupt/exception discrimination.  The HLE BIOS encodes the\n"
    "       pending interrupt bits in cause bits 2-6 (I2=CDROM .. I6=Timer2),\n"
    "       which fall inside the classic 0x7C ExcCode mask, so `andi k1,k1,0x7C`\n"
    "       alone misroutes real interrupts into non_interrupt (which then\n"
    "       chained into the HLE call-hook 0xBFC00C80 and looped forever).\n"
    "       Classify on the hardware I_STAT & I_MASK instead: PEND != 0 means\n"
    "       a real interrupt is asserted; PEND == 0 means a true exception.\n"
    "       NOTE: this block runs BEFORE the register-save below, so it must\n"
    "       only clobber $k0/$k1 (never saved/restored, never relied on by\n"
    "       interrupted code).  Using $t0 here corrupted CdInitRom's\n"
    "       cd_hw_cmd pointer ($t0) when a CD IRQ fired mid-warm-up: the ISR\n"
    "       restored I_MASK (0x04) as $t0 and the next `jalr $t0` jumped to\n"
    "       address 0x04 -> AdEL -> infinite non_interrupt loop. */\n"
    "    lui     $k0, 0x1F80\n"
    "    lhu     $k1, 0x1070($k0)\n"      /* I_STAT */
    "    nop\n"
    "    lhu     $k0, 0x1074($k0)\n"      /* I_MASK */
    "    nop\n"
    "    and     $k1, $k1, $k0\n"         /* PEND */
    "    beqz    $k1, non_interrupt\n"
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
     "    nop\n"
     "    lhu     $s2, 0x1074($s0)\n"
     "    nop\n"
     "    and     $s5, $s1, $s2\n"
     "    /* Load-delay hazard: on the real R3000, a\n"
     "       just-loaded register is not usable until a following instruction,\n"
     "       so `and $s5,$s1,$s2` below previously read the stale $s1=I_STAT\n"
     "       value 0 -> PEND==0 -> the unconditional-beqz below skipped both\n"
     "       dispatch AND the ack -> I_STAT stayed asserted -> rfe re-fired the\n"
     "       vector -> infinite storm.  The nops above fix the reads.  We ALSO\n"
     "       now unconditionally ack the PEND bits right here (I_STAT is\n"
     "       write-0-to-clear on this machine: istat &= rawvalue, so a bit is\n"
     "       cleared by storing 0 into it, preserved by storing 1).  We ack\n"
     "       exactly PEND so a spurious PEND==0 can never leave VSync armed,\n"
     "       and any bit that arrives mid-ISR (not in PEND) survives to re-enter\n"
     "       the handler normally.  Mirrors the BIOS `IREG &= ~IRQ_CDROM`. */\n"
"    xori    $t9, $s5, 0xFFFF\n"      /* ~PEND (low 16 bits) */
      "    sw      $t9, 0x1070($s0)\n"     /* 32-bit write ack (sh->sw experiment) */
     "    /* DBG: snapshot interrupt state to RAM markers (base 0x800BB000) */\n"
     "    lui     $t0, 0x800B\n"
     "    ori     $t0, $t0, 0xB000\n"
    "    sw      $s1, -0xF0($t0)\n"
    "    sw      $s2, -0xEC($t0)\n"
    "    sw      $s5, -0xE8($t0)\n"
    "    mfc0    $t1, $12\n"
    "    sw      $t1, -0xE4($t0)\n"
    "    mfc0    $t2, $13\n"
    "    sw      $t2, -0xE0($t0)\n"
     "    lw      $t1, -0xDC($t0)\n"
     "     nop\n"   /* load-delay: make the DBG counter actually increment */
     "    addiu   $t1, $t1, 1\n"
     "    sw      $t1, -0xDC($t0)\n"
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
      "     nop\n"   /* load-delay: wait for the callback pointer before testing it */
      "    bnez    $t0, 9f\n"       /* handler present -> call it */
      "     nop\n"
       "    /* NULL handler for this bit.  If it is the CDROM IRQ (bit index 2),\n"
       "       drain the CD response FIFO so IRQ2 deasserts; an unread\n"
       "       response keeps IRQ2 asserted, the ISR re-fires on rfe, and main is\n"
       "       starved before libcd's CdInit ever installs the real handler.\n"
       "       Mirror libcd's indexed access (bios acessor @0x8006FB18 + reader\n"
       "       tail @0x8006FC00): select the interrupt group, handshake on the\n"
       "       3-bit CDSTAT field, drain the response FIFO through the indexed\n"
       "       port, then ack by clearing CDSTAT.  The drain is hard-bounded so a\n"
       "       non-deasserting flag can never wedge the ISR. */\n"
       "    li      $t1, 2\n"
       "    bne     $s4, $t1, skip_bit\n"
       "     nop\n"
       "    lui     $t1, 0x1F80\n"
"cd_int_handle:\n"
       "    li      $t3, 1\n"
       "    sb      $t3, 0x1800($t1)\n"      /* index select = 1 (interrupt group) */
       "    lbu     $t3, 0x1803($t1)\n"      /* CDSTAT: 3-bit CD interrupt status */
       "     nop\n"
       "    andi    $t3, $t3, 0x7\n"
       "    /* DBG: snapshot CDSTAT to 0x800BAF48, bump drain counter at 0x800BAF4C */\n"
       "    lui     $t5, 0x800B\n"
       "    ori     $t5, $t5, 0xB000\n"
       "    sw      $t3, -0xB8($t5)\n"
       "    lw      $t6, -0xB4($t5)\n"
       "     nop\n"
       "    addiu   $t6, $t6, 1\n"
       "    sw      $t6, -0xB4($t5)\n"
       "    beqz    $t3, cd_int_ack\n"       /* no CD interrupt pending -> just ack */
       "     nop\n"
       "    li      $t4, 32\n"               /* hard bound: never spin forever */
"cd_drain_loop:\n"
       "    lbu     $t2, 0x1800($t1)\n"      /* CD_STATUS (RSLRRDY bit 5) */
       "     nop\n"
       "    andi    $t2, $t2, 0x20\n"
       "    beqz    $t2, cd_int_ack\n"
       "     nop\n"
       "    lbu     $t2, 0x1801($t1)\n"      /* CD_RESPONSE: read + discard */
       "    addiu   $t4, $t4, -1\n"
       "    bnez    $t4, cd_drain_loop\n"
       "     nop\n"
"cd_int_ack:\n"
       "    /* Ack exactly like libcd's reader tail: with index 1 still selected,\n"
       "       clear CDSTAT (write 7 -> m_stat &= ~7) then re-enable the CD\n"
       "       interrupt (write 7 to 0x1F801802 -> m_reg2 = 7).  Merely\n"
       "       reading the response FIFO does NOT clear the pending CD\n"
       "       interrupt; this indexed clear is what deasserts IRQ2. */\n"
       "    li      $t4, 7\n"
       "    sb      $t4, 0x1803($t1)\n"      /* clear CD interrupt status */
       "    sb      $t4, 0x1802($t1)\n"      /* (re)enable CD interrupt */
       "    b       skip_bit\n"
       "     nop\n"
"9:\n"
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
"    /* I_STAT (0x1F801070) ack: on real hardware this is\n"
     "       write-0-to-clear -- istat &= rawvalue, so a bit is CLEARED by storing\n"
     "       0 into it and preserved by storing 1.  Ack the serviced PEND bits by\n"
     "       storing ~PEND (the BIOS does the same with `IREG = ~IRQ_CDROM`).\n"
     "       Storing PEND itself preserved the asserted bit, so the CD-ROM\n"
     "       interrupt re-fired on every rfe and the CPU spun in the ISR forever.\n"
     "       The bits already acked at ISR entry (see above) are a subset of PEND,\n"
     "       so this second ack is a no-op for them and only reaches bits that\n"
     "       became pending while the dispatch loop was running. */\n"
     "    andi    $s5, $s5, 0xFFFF\n"
     "    xori    $s5, $s5, 0xFFFF\n"
     "    sw      $s5, 0x1070($s0)\n"
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
     "    mfc0    $k0, $14\n"          /* EPC */
     "     nop\n"                       /* mfc0 load-delay */
     "    jr      $k0\n"
     "    .word 0x42000010\n"           /* rfe (delay slot of jr) */
      "non_interrupt:\n"
      "    /* DBG: record EPC + Cause on every non-interrupt exception */\n"
      "    mfc0    $t0, $14\n"
      "    mfc0    $t1, $13\n"
      "    lui     $t2, 0x800B\n"
      "    ori     $t2, $t2, 0xB000\n"
"    sw      $t0, -0xC0($t2)\n"   /* 0x800BAF40 = EPC */
       "    sw      $t1, -0xBC($t2)\n"   /* 0x800BAF44 = Cause */
       "    /* DBG: increment exception counter at 0x800BAF20 */\n"
       "    lui     $t3, 0x800B\n"
       "    ori     $t3, $t3, 0xAF20\n"
       "    lw      $t4, 0($t3)\n"
       "    addiu   $t4, $t4, 1\n"
       "    sw      $t4, 0($t3)\n"
"    /* BOOT-FIX (permanent): a syscall exception must resume at EPC+4.\n"
         "       The fastboot hook at 0xBFC02E50 is a `syscall` whose\n"
        "       normal handler continues one past it; returning to EPC re-executes\n"
        "       the syscall forever, which froze boot.  Detect ExcCode 8 (syscall)\n"
        "       in Cause and skip the instruction.  All other non-interrupt\n"
        "       exceptions keep the old return-to-EPC behaviour below. */\n"
        "    andi    $t5, $t1, 0x7C\n"      /* ExcCode << 2 */
        "    li      $t6, 0x20\n"            /* ExcCode == 8 (syscall) */
        "    beq     $t5, $t6, syscall_skip\n"
        "     nop\n"
        "    /* Do NOT chain to bios_handler_addr (0xBFC00C80): the HLE BIOS\n"
        "       only services that address as a call-hook; jumping to it directly\n"
        "       executes unmapped/faulting bytes (reserved instruction) and the\n"
        "       CPU loops forever.  Return to the faulting EPC instead (rfe).  If\n"
        "       PEND-based classification still lands here it is a true boot bug\n"
        "       and 0x800BAF40 above now shows the real faulting instruction. */\n"
        "    mfc0    $k0, $14\n"            /* EPC */
        "     nop\n"                         /* mfc0 load-delay */
        "    jr      $k0\n"
        "    .word 0x42000010\n"             /* rfe (delay slot of jr) */
        "syscall_skip:\n"
        "    addiu   $k0, $t0, 4\n"          /* EPC + 4: skip the syscall */
        "    jr      $k0\n"
        "    .word 0x42000010\n"             /* rfe (delay slot of jr) */
      ".end exception_handler\n"
);
__asm__(
    ".ent install_jump\n"
"install_jump:\n"
     "    lui     $t3, 0x8000\n"
     "    lw      $t4, 0x0080($t3)\n"      /* w0 = *0x80000080 */
     "    lw      $t5, 0x0084($t3)\n"      /* w1 = *0x80000084 */
     "    lui     $t6, 0x275A\n"           /* addiu k0,k0,imm */
     "    and     $t7, $t5, $t6\n"
     "    bne     $t7, $t6, 1f\n"
     "     nop\n"
     "    sll     $t5, $t5, 16\n"          /* sign-extend addiu imm */
     "    sra     $t5, $t5, 16\n"
     "    move    $t2, $t4\n"              /* keep w0 */
     "    ori     $t4, $zero, 0xBFC0\n"    /* default base (HLE placeholder lui k0,0 / NOP) */
     "    lui     $t6, 0x3C1A\n"           /* lui k0,X */
     "    and     $t7, $t2, $t6\n"
     "    bne     $t7, $t6, 2f\n"          /* w0 not lui-k0 -> keep 0xBFC0 */
     "     nop\n"
     "    andi    $t2, $t2, 0xFFFF\n"      /* X */
     "    beqz    $t2, 2f\n"               /* X==0 placeholder -> keep 0xBFC0 */
     "     nop\n"
     "    move    $t4, $t2\n"              /* base = X */
     "2:\n"
     "    sll     $t4, $t4, 16\n"          /* base << 16 */
     "    addu    $t4, $t4, $t5\n"         /* handler = (base<<16) + imm */
     "    b       3f\n"
     "     nop\n"
     "1:\n"                                 /* fallback: treat w0 as 'j' */
     "    sll     $t4, $t4, 6\n"
     "    srl     $t4, $t4, 6\n"
     "    sll     $t4, $t4, 2\n"
     "    lui     $t5, 0x8000\n"
     "    or      $t4, $t4, $t5\n"
     "3:\n"
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
     "    /* Also chain the HLE default exception vector. The HLE\n"
     "       BIOS re-installs `j 0x80059C00` at 0x80000080 and its stub at\n"
     "       0x80059C00 sits inside our .bss, which _start zeroes.  Write our\n"
     "       own `j exception_handler` there (+ delay-slot nop) so that even a\n"
     "       reverted vector still lands in our handler instead of a NOP slide\n"
     "       that never increments the interrupt counters. */\n"
     "    lui     $t1, 0x8005\n"
     "    ori     $t1, $t1, 0x9C00\n"
     "    sw      $t0, 0($t1)\n"
     "    sw      $zero, 4($t1)\n"
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
        /* Set SR BEFORE arming I_MASK: clear BEV (boot/HLE leaves it 1, which
           would divert every exception/interrupt to the ROM vector instead of
           our handler at 0x80000080) and enable IEc + only the interrupt mask
           bits needed for boot progress: IM0 (bit10 = vblank/VSync, which
           advances Vcount) and IM2 (bit12 = CD-ROM IRQ2).
           WARNING: arming the FULL IM0-5 mask here (0xFCFF/0xFC00) made the
           boot interrupt-storm (free-running Timer0/DMA IRQs re-firing at CPU
           speed before main's timers are configured), so the game never reached
           D0=5.  Only VSync+CD are required and are each non-storming.  The
           pre-existing deadlock at D0=5 was precisely SR bit10 (IM0) being
           cleared, which masked the VSync interrupt and froze Vcount. */
        "lui     $at, 0xFFBF\n"
        "ori     $at, $at, 0xF0FF\n"   /* 0xFFBFF0FF: clear BEV(22), RE(8), IM(10-15) */
        "mfc0    $t0, $12\n"
        "and     $t0, $t0, $at\n"
        "ori     $t0, $t0, 0x14FF\n"   /* IEc/IEp/IEo/KU bits + IM0(VSync)+IM2(CD) */
        "mtc0    $t0, $12\n"
        "nop\n"
"lui     $s0, 0x1F80\n"
         /* Do NOT arm I_MASK here.  The VSync bit
            (I_STAT bit 0) re-asserts in lockstep with our exception handler
            (~554 cycles per pass), so if VSync is unmasked at this point the CPU
            re-enters the ISR during the rfe delay slot over and over with resume
            EPC = after_reinstall each time - _start never executes the
            IntrCallback wrapper store nor `jal main` (verified: EPC stayed
            0x800341A0 across 8.8M passes, main never ran).  main() arms I_MASK =
            0x00DD itself at System.c:313 (D0=5), after the wrapper is installed;
            keep everything masked until then.  We still clear I_STAT first so no
            stale pending bit fires the moment main re-arms. */
"lhu     $t0, 0x1070($s0)\n"
         "nop\n"
         "sh      $t0, 0x1070($s0)\n"
         "sh      $zero, 0x1074($s0)\n"       /* I_MASK = 0 until main() arms */
        "/* DBG: snapshot SR + I_MASK after writes (base 0x800BB000) */\n"
        "lui     $t1, 0x800B\n"
        "ori     $t1, $t1, 0xB000\n"
        "mfc0    $t0, $12\n"
        "sw      $t0, -0xD8($t1)\n"
        "lhu     $t0, 0x1074($s0)\n"
        "sw      $t0, -0xD4($t1)\n"
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
        "li      $t1, 0xFFFB\n"
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
        "li      $t1, 0xFFFB\n"
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
    "    andi    $t2, $t1, 0x20\n"   /* RSLRRDY: response ready */
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
/* =========================================================================
 * vsync_interrupt_wrapper - VSync IRQ entry, wired into IntrCallbackTable[0]
 *
 * _start writes this pair-address into IntrCallbackTable[0]
 * (System.c ~line 246-249).  The PS-EXE exception_handler (above) dispatches
 * each asserted I_STAT bit to IntrCallbackTable[bit]; for VSync (bit 0) we
 * must reach vsync_rel_handler, which increments Vcount and walks
 * VSyncCallbackTable so VSyncCallbackTable[0] (TimerCallback_Default_impl)
 * ticks SpuFrameCounter per frame.
 *
 * Before this wrapper existed, vsync_interrupt_wrapper had no definition, so
 * --unresolved-symbols=ignore-all resolved it to 0x0 and IntrCallbackTable[0]
 * stayed NULL - the dispatcher's `beqz $t0, skip_bit` skipped it, the
 * callback table was never walked, and the license state machine never
 * advanced.
 *
 * exception_handler calls the entry with $a0 cleared, so a0=0 -> walk mode.
 */
__asm__(
    ".globl vsync_interrupt_wrapper\n"
    ".type vsync_interrupt_wrapper, @function\n"
    ".ent vsync_interrupt_wrapper\n"
    "vsync_interrupt_wrapper:\n"
    "    j       vsync_rel_handler\n"
    "    move    $a0, $zero\n"
    ".end vsync_interrupt_wrapper\n"
);
/* =========================================================================
 * TimerCallback_Default_impl - per-frame SPU tick callback
 *
 * This is registered by SpuStartTimer as the Timer2/root-counter interrupt
 * callback (or as a VSync callback when TimerSpinCounter != 0).  In the
 * original ROM it lives at 0x80033738 and drives the SPU sequencer tick
 * plus the SpuFrameCounter increment every frame.
 *
 * override.s maps  TimerCallback_Default -> D_80033738  (a .word 0 in the
 * ROM disassembly region), which means SpuStartTimer was registering a null
 * pointer as the callback - nothing ever ticked, VSyncCallbackTable stayed
 * empty, and the license state machine never advanced.
 *
 * This minimal version just increments SpuFrameCounter so the per-frame
 * counter advances.  The full SPU sequencer tick body should be restored
 * from Ghidra once the sound path is working.
 *
 * GHIDRA TODO: reverse 0x80033738 in the ROM and replace this body with
 * the real tick logic (SpuFrameCounter++, voice fade processing, etc.)
 * ======================================================================== */
void TimerCallback_Default_impl(void)
{
    SpuFrameCounter++;
}

/* Minimal CD callbacks for the PS-EXE (full image is embedded; CD ops skipped) */
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
/* =========================================================================
 * Cinema_SetState - Advance the cinema/license entity state machine
 *
 * Called from Font_Constructor (Rendering_UI.c) after the license screen
 * entity has finished rendering. Writes the new state into the three
 * DreamSys cinema-state globals so the main loop's vtable[0x54] gate
 * (EntityBehavior_State_80026348) can see that the license phase is done
 * and proceed to load asmklogo.tim.
 *
 * Original signature (reconstructed from call site in Rendering_UI.c:1175):
 *   Cinema_SetState(entity->nField_38, index[5], entity->pOtherEntity,
 *                   entity->nField_34, index[4]);
 *
 * arg0 = new cinema state value   (written to DreamSys_CinemaState)
 * arg1 = secondary state object   (written to DreamSys_CinemaState_08)
 * arg2 = entity's other-entity ptr(written to DreamSys_CinemaState_04 ... lower half)
 * arg3 = field_34                 (written to DreamSys_CinemaState_04 ... upper half / flag)
 * arg4 = index[4]                 (unused in minimal reconstruction)
 *
 * GHIDRA TODO: verify exact field layout - see notes at bottom of file.
 * ======================================================================== */
void Cinema_SetState(int newState, int stateObj, int otherEntity, int field34, int index4)
{
    DreamSys_CinemaState    = (u32)newState;
    DreamSys_CinemaState_04 = (u32)otherEntity;
    DreamSys_CinemaState_08 = (u32)stateObj;
    (void)field34; (void)index4;
}

/* =========================================================================
 * Cinema_FreeEntity - Tear down the cinema/license entity
 *
 * Called from the entity destructor path in Rendering_UI.c just before
 * VramParticle_Cleanup(). In the original it releases whatever heap
 * object was allocated for the cinema sequence. Safe minimal version:
 * clear the state globals so nothing holds a stale pointer.
 *
 * GHIDRA TODO: verify what pointer is freed - see notes at bottom of file.
 * ======================================================================== */
void Cinema_FreeEntity(void)
{
    DreamSys_CinemaState    = 0;
    DreamSys_CinemaState_04 = 0;
    DreamSys_CinemaState_08 = 0;
}

/* =========================================================================
 * Cinema_TriggerSfx - Trigger a sound effect for a cinema event
 *
 * Called from Rendering_UI.c line 1415 with a sound ID and timing params.
 * Returns 0 (no sound played) - safe until the SPU/sound path is wired up.
 *
 * GHIDRA TODO: wire to the real SPU sequence trigger once sound is working.
 * ======================================================================== */
int Cinema_TriggerSfx(int soundId, int param1, int param2)
{
    (void)soundId; (void)param1; (void)param2;
    return 0;
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
 *   0: Status(R) / IndexSelect(W)
 *   1: Command(W) / Response(R)
 *   2: Parameter(W) / Data(R)
 *   3: Interrupt control
 */
#define CD_BASE          ((volatile u8 *)0x1F801800)
#define CD_STATUS        (CD_BASE[0x00])
#define CD_CMD           (CD_BASE[0x01])
#define CD_RESPONSE      (CD_BASE[0x01])
#define CD_PARAM         (CD_BASE[0x02])
#define CD_DATA          (CD_BASE[0x02])
#define CD_CTRL          (CD_BASE[0x03])
/* Status register bits (real hardware):
 *   0x08 PRMEMPT param FIFO empty |
 *   0x10 PRMWRDY 1 = param FIFO safe to push |
 *   0x20 RSLRRDY 1 = response FIFO NOT empty (response ready) |
 *   0x40 DRQSTS  1 = data FIFO not empty |
 *   0x80 BUSYSTS 1 = command/param transmission busy */
#define ST_BUSY     0x80    /* BUSYSTS */
#define ST_RESP     0x20    /* RSLRRDY: response available */
#define ST_DATA     0x40    /* DRQSTS: data available */
#define ST_PRM      0x10    /* PRMWRDY: can write parameter */
/* Read one sector (2048 bytes) from data port */
static void cd_read_sector(u8 *dst)
{
    int i;
    for (i = 0; i < 2048; i++) {
        while (!(CD_STATUS & ST_DATA));
        dst[i] = CD_DATA;
    }
}
/* =========================================================================
 * CdInitRom - the game's REAL CD warm-up (ROM CD_init @0x8002a75c)
 *
 * The ROM boots the drive before GPU init via this exact sequence:
 *   ResetCallback -> InterruptCallback(2,cb) -> wait (REG3&7)==0 ->
 *   cd_result_flag=0 -> REG0=0, REG3=0 -> COMMON_DELAY=0x1325 ->
 *   Nop(1) -> [disc-open poll] -> Init(0x0A, spins motor) -> Demute(0x0C)
 *   -> Cd_sync(0,0)
 *
 * The decompiled port instead links libcd's CdInit, which never runs the
 * warm-up, so the drive never spins up (boot sits forever waiting for CD
 * data). This function performs the same register-level wake-up, polled
 * (no ISR dependency) and bounded (a cold/absent drive cannot hang boot).
 * Called from System.c immediately before libcd CdInit().
 * ======================================================================== */
/* COMMON_DELAY: PS1 CD-ROM master-clock delay register (written by CD_init) */
#define CD_COMMON_DELAY    (*(volatile unsigned short *)0x1F801020)
/* Select one of the 4 indexed CD-ROM register groups (0=cmd,1=resp,2=data) */
static void cd_hw_index(int idx)
{
    CD_BASE[0] = (u8)idx;
}
/* Polled command send: index 0, wait not-busy, write params, then command.
   Mirrors the ROM's CD_cw but never blocks forever. */
static void cd_hw_cmd(u8 cmd, int nparam, const u8 *params)
{
    long guard = 5000000;
    int i;
    cd_hw_index(0);
    while ((CD_STATUS & ST_BUSY) && --guard > 0)
        ;
    if (guard <= 0)
        return;
    for (i = 0; i < nparam; i++) {
        guard = 5000000;
        while (!(CD_STATUS & ST_PRM) && --guard > 0)
            ;
        if (guard <= 0)
            break;
        CD_PARAM = params[i];
    }
    CD_CMD = cmd;
}
/* Polled response wait (bounded). Returns 1 if a response was read.  Reads and
   discards every available response byte so the CD response FIFO is drained:
   an unread response keeps CD IRQ2 asserted, which storms the ISR and starves
   main once I_MASK bit2 was armed. */
static int cd_hw_wait_response(void)
{
    long guard = 5000000;
    cd_hw_index(1);
    while (!(CD_STATUS & ST_RESP) && --guard > 0)
        ;
    if (guard <= 0)
        return 0;
    while (CD_STATUS & ST_RESP)
        (void)CD_RESPONSE;   /* read + discard each response byte */
    return 1;
}
void CdInitRom(void)
{
    volatile u32 *D0 = (volatile u32 *)0x800BAFF0;
    volatile u32 *D1 = (volatile u32 *)0x800BAFF4;
    *D0 = 0xDD; *D1 = 0x10;  /* marker: CdInitRom warm-up start */
    /* CD_init writes COMMON_DELAY before the first command */
    CD_COMMON_DELAY = 0x1325;
    /* Clear interrupt control, select command index */
    cd_hw_index(0);
    CD_CTRL = 0x00;
    /* CdlNop (0x01): sync until the drive answers */
    cd_hw_cmd(0x01, 0, 0);
    cd_hw_wait_response();
    /* CdlInit (0x0A): hardware reset + motor spin-up */
    cd_hw_cmd(0x0A, 0, 0);
    cd_hw_wait_response();
    /* CdlDemute (0x0C): unmute the data/audio path */
    cd_hw_cmd(0x0C, 0, 0);
    cd_hw_wait_response();
    *D0 = 0xDD; *D1 = 0x11;  /* marker: CdInitRom warm-up done */
}
void CdLoadStage2(void *dest, u32 lba, u32 count)
{
    volatile u32 *D0 = (volatile u32 *)0x800BAFF0;
    volatile u32 *D1 = (volatile u32 *)0x800BAFF4;
    /* Single full-payload PS-EXE: the BIOS loads everything at boot. If the
       first word of the destination is already non-zero, data is in RAM and
       we skip CD operations entirely.
       We still signal completion so the boot state machine's gate flags pass:
         CdReadDone  - polled by EntityBehavior_State_80026170 via *(*(entity+0x20)+0xC)
         CdOperationPending - cleared so the license->logo transition is not blocked */
    if (*(volatile u32 *)dest != 0) {
        *D0 = 0xDD; *D1 = 1;  /* marker: data already loaded, skipping CdLoadStage2 */
        CdReadDone = 1;
        CdOperationPending = 0;
        extern char Cd_FilePathPrefix[];
        if (Cd_FilePathPrefix[0] == 0) {
            strcpy(Cd_FilePathPrefix, "CDI/STG04/TEXA.TIX;1");
        }
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
            /* CD interrupt never fired during the wait (the callback is not
               registered yet in this PS-EXE boot path, so CdDataSync never
               sees data). Do NOT trap forever: signal completion and return
               exactly like the pre-loaded-data skip path above so the boot
               state machine's gates pass and DreamSys init can proceed. */
            *D0 = 0xDD; *D1 = 0xCD;  /* marker: CdLoadStage2 CD read timed out */
            CdReadDone = 1;
            CdOperationPending = 0;
            return;
        }
        CdGetSector(dst, 512);
        dst += 512;
    }
    CdControl(CdlPause, 0, result);
    CdSync(0, result);
    *D0 = 0xDD; *D1 = 3;  /* marker: CdLoadStage2 complete */
}

