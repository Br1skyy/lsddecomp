#ifndef REGISTERS_H
#define REGISTERS_H

/* Ghidra-decompiled hardware register names for PS1 SPU */
/* These are memory-mapped I/O registers at fixed addresses    */
/* in the PS1 address space (0x1f801c00 range).                */

/* SPU control register (SPUCNT) at 0x1f801c0a */
extern volatile u16 DAT_1f801c0a;
#define SPU_CTRL_REG_CPUCNT DAT_1f801c0a

/* SPU channel noise mode register - struct with two u16 fields */
typedef struct {
    u16 _0_2_;
    u16 _2_2_;
} SpuChannelReg;

extern volatile SpuChannelReg SPU_VOICE_CHN_NOISE_MODE;
extern volatile SpuChannelReg SPU_VOICE_KEY_OFF;
extern volatile SpuChannelReg SPU_VOICE_KEY_ON;
extern volatile SpuChannelReg SPU_VOICE_CHN_REVERB_MODE;

/* SPU voice 0 registers (offset base for voice block array) */
extern volatile u16 VOICE_00_LEFT_RIGHT;
extern volatile u16 VOICE_00_ADPCM_SAMPLE_RATE;
extern volatile u16 VOICE_00_ADPCM_START_ADDR;
extern volatile u32 VOICE_00_ADSR_ATT_DEC_SUS_REL;

#endif /* REGISTERS_H */
