#include "common.h"
#include "DreamSys.h"
#include "StageGrid.h"
#include <libcd.h>
/*
 * GameLoop.c - SPU voice management, frame timing, and game loop helpers
 *
 * This file manages:
 *   - Voice volume control (L/R volume, clamping, SPU register flushing)
 *   - Voice allocation/state tracking (active bitmasks, pending operations)
 *   - Sound program selection and voice attribute parsing
 *   - SPU system init/shutdown and timer setup
 *   - Frame rate configuration and SPU memory allocation
 *
 * Voice struct layout (0xAC bytes per voice):
 *   0x00: reserved
 *   0x04: dataPtr (current read position)
 *   0x08: dataBasePtr (start of sequence data)
 *   0x0c: loopStartPtr
 *   0x10: mode
 *   0x11: keyOff
 *   0x12: channelIndex
 *   0x13: effect1Type
 *   0x14: effect2Type
 *   0x15: volumeParam
 *   0x16: sfxType
 *   0x17-0x26: volume[16]
 *   0x27: damperMode
 *   0x28: damperCounter
 *   0x29: effectChangeCount
 *   0x2a: volumeChangeCount
 *   0x2b: keyOn
 *   0x2c-0x3b: channelOrder[16]
 *   0x3c: reserved
 *   0x3e: fadeStartVol
 *   0x40: fadeTargetVol
 *   0x42: fadeDelta
 *   0x46: panning
 *   0x48: reserved
 *   0x4a: seqType
 *   0x4c: soundBankId
 *   0x4e-0x6d: effectVol[16]
 *   0x6e: playSpeed
 *   0x70: playSpeedDefault
 *   0x72: playSpeedBackup
 *   0x74: volumeL
 *   0x76: volumeR
 *   0x78: pitch
 *   0x7a: reserved
 *   0x7c: initialDataOffset
 *   0x80: seqOffset
 *   0x84: sampleRate
 *   0x88: currentDataOffset
 *   0x8c: sampleRateBackup
 *   0x90: flags
 *   0x94: fadeTargetVol2
 *   0x98: fadeTotalFrames
 *   0x9c: reserved
 *   0xa0: reserved
 *   0xa4: reserved
 *   0xa8: panVolL
 *   0xaa: panVolR
 *
 * Per-voice state (stride 0x1a = 26 bytes, base VoicePitch):
 *   +0x00 (d988): s16 pitch/tone
 *   +0x02 (d98a): s16 age/priority
 *   +0x04 (d98c): s16 sample rate
 *   +0x06 (d98e): s16 lifetime
 *   +0x0e (d996): s16 voice/sound ID
 *   +0x10 (d998): s16 program index
 *   +0x12 (d99a): s16 channel
 *   +0x14 (d99c): s16 pitch/bank index
 *   +0x16 (d99e): s16 stage
 *
 * Per-voice status (stride 0x34 = 52 bytes, base VoiceAge):
 *   +0x0a (d994): s16 volume target
 *   +0x19 (d9a3): u8  status/type
 *
 * SpuVolShadowL[] (stride 8): s16 SPU register shadow (left)
 * SpuVolShadowR[] (stride 8): s16 SPU register shadow (right)
 * VoiceDirtyFlags[]:            u16 voice dirty flags
 */
/* ---- Forward declarations (defined later in this file) ---- */
s32 SpuSelectProgram(u16 bank, s16 program);
s32 SpuMallocInternal(u32 *vabData, s32 bankIndex, s16 skipAlloc, s32 spuAddr);
/* ---- Externals: functions from other translation units ---- */
extern void  EnterCriticalSection(void);
extern void  ExitCriticalSection(void);
extern u32   SpuFindFreeVoice(void);
extern void  SpuSetVoiceSample(void);
extern void  SpuSetNoiseVoice(u32 voiceIndex);
extern u16   SpuCalcSamplePitch(u16 sampleRate, u16 pitch);
extern void  SpuSetVoiceFade(s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void  SpuInitAllVoices(u32 count);
extern s32   GetVideoMode(void);
extern void  SpuInit(void);
extern void  Sound_InitHot(void);
extern void  SpuQuit(void);
extern s32   SpuGetInTransfer(void);
extern void  SpuSetInTransfer(s32 flag);
extern void *MemAlloc(u32 size);
extern void  MemFree(void *ptr);
extern void  ResetCallback(void);
extern void  VSyncCallback(void *cb);
extern void * InterruptCallback(s32 channel, void *cb);
extern void  ResetRCnt(u32 spec);
extern void  SetRCnt(u32 spec, u16 count, u16 mode);
extern void  StartRCnt(u32 spec);
extern s32   PsyqSpuMalloc(s32 size);
/* ---- Global data references ---- */
extern u32   TimerSpinCounter;
extern s32   TimerMode;
extern u32   TimerCountUp;
extern s32   VideoMode;
extern void *SavedInterruptCB;
extern u32   FrameRateSetting;
extern u32   FrameRatePalFlag;
extern u16   SpuRegDefaults[];
extern u16   SpuMasterVolDefaults[];
extern u32   SpuBankProgramTables[];
extern u32   SpuBankVabDataPtrs[];
extern u32   SpuBankVoiceAttrTables[];
extern u32   SpuCurrentProgramData;
extern u32   SpuCurrentBankData;
extern u32   SpuCurrentVoiceAttr;
extern u16   SpuOpenChannelMask;
extern s8    SpuVoiceAttrType;
extern s8    SpuCurrentBankIdx;
extern s8    SpuVoiceVolParam1;
extern s8    SpuVoiceVolParam2;
extern s8    SpuVoiceStep1;
extern char  SpuVoiceStepCalc;
extern s8    SpuCurrentProgramIdx;
extern u8    SpuCurrentVoiceAttrIdx;
extern u8    SpuVoiceAttrByte1;
extern u8    SpuVoiceAttrByte4;
extern u8    SpuVoiceAttrStoredIdx;
extern u8    SpuVoiceAttrByte3;
extern u8    SpuVoiceAttrByte4b;
extern u8    SpuVoiceAttrByte0;
extern u8    SpuVoiceAttrByte5;
extern u8    SpuVoiceAttrByte6;
extern u8    SpuVoiceAttrByte7;
extern u8    SpuVoiceAttrByte8;
extern u8    SpuVoiceAttrByte1b;
extern u16   SpuCurrentVoiceId;
extern u16   SpuCurrentPitch;
extern u16   SpuCurrentChannelIdx;
extern u8    SpuBankLoadedFlags[];
extern u32   SpuKeyOnMaskL;
extern u32   SpuKeyOnMaskH;
extern s32   SpuMonoMixFlag;
extern u32   SpuSeqBusyFlag;
extern u32   SpuMaxPrograms;
extern u32   SpuVoiceCount;
extern u32   SpuTickRate;
extern u32   SpuVoicePtrTable[];
extern u32   SpuAttributeBlockBase;
extern u32   SpuTotalVoiceCount;
extern u32   SpuVoicesPerBank;
extern u32   SpuLoadedBankCount;
extern u32   SpuBankSpuAddrTable[];
extern u32   SpuBankTotalSizes[];
extern u32   SpuBankVoiceOffsetTable[];
extern u32   SpuVoiceFreeMaskL;
extern u32   SpuVoiceFreeMaskH;
extern s16   SpuVolShadowL[];
extern s16   SpuVolShadowR[];
extern u16   VoiceDirtyFlags[];
extern s16   VoicePitch[];
extern s16   VoiceAge[];
extern u16   VoiceSampleRate[];
extern u16   VoiceLifetime[];
extern s16   VoiceVolume[];
extern s16   Spu_DefaultParam[];
extern s16   VoiceSoundId[];
extern s16   VoiceProgramIdx[];
extern s16   VoiceChannel[];
extern s16   VoiceBankIdx[];
extern s16   VoiceStage[];
extern u8    VoiceStatus[];
extern u32   TimerCallback_SpuSet;
extern u32   TimerCallback_VSync;
extern u32   TimerCallback_Default;
/* ---- SPU hardware register access ---- */
#define SPU_MAIN_VOL_L  (*(volatile u16 *)0x1f801d80)
/* ========================================================================
 * 1. SpuSetVoiceVolume - Set left/right volume on a voice
 *
 * Given a voice ID ((voice << 8) | bank), writes volumeL/volumeR at
 * offsets 0x74/0x76 in the 0xAC-byte voice struct. Clamps to 0x7f
 * (max volume). If flushToSpu is set, also writes scaled volume to
 * the SPU register shadow array for all matching voice state entries.
 *
 * Returns the voice ID.
 * ======================================================================== */
s32 SpuSetVoiceVolume(u32 voiceId, s16 volumeL, s16 volumeR, s16 flushToSpu)
{
    s32 i, idx;
    s32 voicePtr;
    voicePtr = ((voiceId & 0xffff) >> 8) * 0xac + SpuVoicePtrTable[voiceId & 0xff];
    SpuCurrentVoiceId = (s16)voiceId;
    *(s16 *)(voicePtr + 0x74) = volumeL;
    *(s16 *)(voicePtr + 0x76) = volumeR;
    if (SOUND_VOLUME_DEFAULT < *(u16 *)(voicePtr + 0x74))
        *(s16 *)(voicePtr + 0x74) = SOUND_VOLUME_DEFAULT;
    if (SOUND_VOLUME_DEFAULT < *(u16 *)(voicePtr + 0x76))
        *(s16 *)(voicePtr + 0x76) = SOUND_VOLUME_DEFAULT;
    if ((flushToSpu == 1) && (SpuVoiceCount != 0)) {
        for (i = 0; i < (s32)SpuVoiceCount; i++) {
            if (VoiceSoundId[i * 0x1a / 2] == (s16)voiceId) {
                idx = i * 0x10; /* (i << 0x13) >> 0xf = i * 16 */
                *(s16 *)((s32)&SpuVolShadowL[0] + idx) = volumeL * 0x81;
                *(s16 *)((s32)&SpuVolShadowR[0] + idx) = volumeR * 0x81;
                VoiceDirtyFlags[i] |= 3;
            }
        }
    }
    return (s32)SpuCurrentVoiceId;
}
/* ========================================================================
 * 2. SpuGetVoiceVolume - Read left/right volume from a voice
 *
 * Reads volumeL (offset +0x74) and volumeR (offset +0x76) from the
 * voice struct and returns them via output parameters.
 * Returns the voice ID.
 * ======================================================================== */
s32 SpuGetVoiceVolume(u32 voiceId, s16 *outVolL, s16 *outVolR)
{
    s32 voicePtr;
    SpuCurrentVoiceId = (s16)voiceId;
    voicePtr = ((s32)(voiceId & 0xff00) >> 8) * 0xac + SpuVoicePtrTable[voiceId & 0xff];
    *outVolL = *(s16 *)(voicePtr + 0x74);
    *outVolR = *(s16 *)(voicePtr + 0x76);
    return (s32)SpuCurrentVoiceId;
}
/* ========================================================================
 * 3. SpuStopVoice - Stop a voice by sound ID
 *
 * Scans the voice state table for entries matching soundId, clears
 * each entry's state fields and removes it from the active bitmasks.
 * ======================================================================== */
void SpuStopVoice(s16 soundId)
{
    u32 idx;
    u16 bitLow, bitHigh;
    u8 i;
    if (SpuVoiceCount != 0) {
        for (i = 0; i < SpuVoiceCount; i++) {
            if (VoiceSoundId[i * 0x1a / 2] == soundId) {
                idx = (u32)(u16)i;
                bitHigh = (u16)(1 << (idx & 0x1f));
                if (idx < SOUND_CHANNEL_MAX)
                    bitLow = 0;
                else {
                    bitHigh = 0;
                    bitLow = (u16)(1 << (idx - SOUND_CHANNEL_MAX & 0x1f));
                }
                SpuCurrentChannelIdx = (u16)i;
                VoiceStatus[idx * 0x34] = 0;
                VoiceSampleRate[idx * 0x1a / 2] = 0;
                VoicePitch[idx * 0x1a / 2] = 0;
                SpuVoiceFreeMaskL = bitHigh | SpuVoiceFreeMaskL;
                SpuKeyOnMaskL &= ~SpuVoiceFreeMaskL;
                SpuVoiceFreeMaskH = bitLow | SpuVoiceFreeMaskH;
                SpuKeyOnMaskH &= ~SpuVoiceFreeMaskH;
            }
        }
    }
}
/* ========================================================================
 * 4. SpuSetVoiceAttr - Set voice attribute byte
 *
 * Selects the bank/program, then writes attrValue into the attribute
 * table at (program * 0x10 + base + 1). Returns the byte written,
 * or 0xFFFFFFFF on failure.
 * ======================================================================== */
u32 SpuSetVoiceAttr(s16 bank, s16 program, u8 attrValue)
{
    s32 result;
    result = SpuSelectProgram((s32)bank, (s32)program);
    if (result == 0) {
        *(u8 *)(program * 0x10 + SpuCurrentProgramData + 1) = attrValue;
        return (u32)*(u8 *)(program * 0x10 + SpuCurrentProgramData + 1);
    }
    return 0xffffffff;
}
/* ========================================================================
 * 5. SpuMixVolume - Compute mixed L/R volumes for a voice
 *
 * Finds voice state entries matching voiceId, program and stage,
 * then applies a multi-stage volume formula using attributes from
 * the program and voice attribute tables. Final volumes are written
 * to the SPU register shadow arrays.
 *
 * Returns the number of entries updated.
 * ======================================================================== */
s32 SpuMixVolume(u32 voiceId, s16 stage, s16 program, u32 volMultiplier, u16 envAdjust)
{
    u32 idx;
    u32 attrA, attrB, envC, envD;
    u32 base, scaledL, scaledR;
    s32 matchCount;
    s32 voicePtr;
    s32 pitchVal;
    u8 i;
    matchCount = 0;
    voicePtr = ((s32)(voiceId & 0xff00) >> 8) * 0xac + SpuVoicePtrTable[voiceId & 0xff];
    SpuSelectProgram((s32)stage, (s32)program);
    SpuCurrentVoiceId = (s16)voiceId;
    if (SpuVoiceCount != 0) {
        for (i = 0; i < SpuVoiceCount; i++) {
            if ((VoiceSoundId[i * 0x1a / 2] == (s16)voiceId) &&
                ((s32)VoiceChannel[i * 0x1a / 2] == (s32)program) &&
                (VoiceStage[i * 0x1a / 2] == stage)) {
                attrA = (u32)*(u8 *)((s16)VoiceProgramIdx[i * 0x1a / 2] * 0x10 + SpuCurrentProgramData + 1);
                attrB = (u32)*(u8 *)((s16)VoiceChannel[i * 0x1a / 2] * 0x10 + SpuCurrentProgramData + 1);
                if (attrA != attrB)
                    attrA = attrB;
                pitchVal = (s32)VoiceVolume[i * 0x1a / 2] * (s32)(volMultiplier & 0xffff) / SOUND_VOLUME_DEFAULT;
                base = ((u32)*(u8 *)(SpuCurrentBankData + 0x18) * pitchVal * SPU_PITCH_MAX) / SPU_PITCH_MIN;
                base = base * attrA *
                       (u32)*(u8 *)((s16)VoiceBankIdx[i * 0x1a / 2] * 0x20 + SpuCurrentVoiceAttr + 2) / SPU_PITCH_MIN;
                scaledL = (base * *(u16 *)(voicePtr + 0x74)) / SOUND_VOLUME_DEFAULT;
                envC = (u32)*(u8 *)((s16)VoiceBankIdx[i * 0x1a / 2] * 0x20 + SpuCurrentVoiceAttr + 3);
                scaledR = (base * *(u16 *)(voicePtr + 0x76)) / SOUND_VOLUME_DEFAULT;
                if (envC < SOUND_VOLUME_HALF)
                    scaledR = (scaledR * envC) / 0x3f;
                else
                    scaledL = (scaledL * (SOUND_VOLUME_DEFAULT - envC)) / 0x3f;
                envD = (u32)*(u8 *)((s16)VoiceProgramIdx[i * 0x1a / 2] * 0x10 + SpuCurrentProgramData + 4);
                if (envD < SOUND_VOLUME_HALF)
                    scaledR = (scaledR * envD) / 0x3f;
                else
                    scaledL = (scaledL * (SOUND_VOLUME_DEFAULT - envD)) / 0x3f;
                envD = envAdjust & 0xff;
                if (envD < SOUND_VOLUME_HALF)
                    scaledR = (scaledR * envD) / 0x3f;
                else
                    scaledL = (scaledL * (SOUND_VOLUME_DEFAULT - envD)) / 0x3f;
                if (SpuMonoMixFlag == 1) {
                    if (scaledL < scaledR)
                        scaledL = scaledR;
                    scaledR = scaledL;
                }
                idx = (u32)i;
                SpuVolShadowL[idx * 8] = (s16)((scaledL * scaledL) / SPU_PITCH_MAX);
                SpuVolShadowR[idx * 8] = (s16)((scaledR * scaledR) / SPU_PITCH_MAX);
                matchCount++;
                VoiceDirtyFlags[idx] |= 3;
            }
        }
    }
    return matchCount;
}
/* ========================================================================
 * 6. SpuStartSeqVoice - Start a sequence voice with full init
 *
 * Selects the program, reads envelope attributes from the voice
 * attribute tables, allocates a voice slot, fills the voice state
 * entry, and triggers playback via SpuSetVoiceSample.
 *
 * Returns the allocated voice index, or 0xFFFFFFFF on failure.
 * ======================================================================== */
u32 SpuStartSeqVoice(s16 soundId, u16 program, u8 voiceAttrIdx,
                     u16 volParam1, u16 volParam2, s16 step1, s16 step2)
{
    u16  envVal;
    s32  result;
    s32  voicePtr;
    u32  voiceIdx;
    if (SpuSeqBusyFlag != 1) {
        SpuSeqBusyFlag = 1;
        result = SpuSelectProgram((s32)soundId, (s32)(s16)program);
        if (result == 0) {
            SpuCurrentVoiceId = 0x21;
            SpuVoiceVolParam1 = (s8)volParam1;
            SpuVoiceVolParam2 = (s8)volParam2;
            SpuVoiceStep1 = (s8)step1;
            if (step1 == step2) {
                SpuVoiceStepCalc = '@';
            } else if (step2 < step1) {
                if (step1 == 0) trap(0x1c00);
                if ((step1 == -1) && (step2 << 6 == -0x80000000)) trap(0x1800);
                SpuVoiceStepCalc = (char)((step2 << 6) / step1);
            } else {
                if (step2 == 0) trap(0x1c00);
                if ((step2 == -1) && (step1 << 6 == -0x80000000)) trap(0x1800);
                SpuVoiceStep1 = (s8)step2;
                SpuVoiceStepCalc = (char)SOUND_VOLUME_DEFAULT - (char)((step1 << 6) / step2);
            }
            /* Read program attribute bytes */
            voicePtr = (s32)((u32)program << 0x10) >> 0xc;
            SpuVoiceAttrByte1 = *(u8 *)(voicePtr + SpuCurrentProgramData + 1);
            SpuVoiceAttrByte4 = *(u8 *)(voicePtr + SpuCurrentProgramData + 4);
            SpuVoiceAttrType = *(u8 *)(voicePtr + SpuCurrentProgramData);
            /* Read voice attribute bytes */
            voicePtr = ((u32)voiceAttrIdx + (u32)SpuCurrentVoiceAttrIdx * 0x10) * 0x20 + SpuCurrentVoiceAttr;
            SpuVoiceAttrByte0 = *(u8 *)voicePtr;
            SpuCurrentPitch = *(s16 *)(voicePtr + 0x16);
            SpuVoiceAttrByte3 = *(u8 *)(voicePtr + 2);
            SpuVoiceAttrByte4b = *(u8 *)(voicePtr + 3);
            SpuVoiceAttrByte5 = *(u8 *)(voicePtr + 4);
            SpuVoiceAttrByte6 = *(u8 *)(voicePtr + 5);
            SpuVoiceAttrByte1b = *(u8 *)(voicePtr + 1);
            SpuVoiceAttrByte7 = *(u8 *)(voicePtr + 6);
            SpuVoiceAttrByte8 = *(u8 *)(voicePtr + 7);
            SpuVoiceAttrStoredIdx = voiceAttrIdx;
            if (SpuCurrentPitch != 0) {
                voiceIdx = SpuFindFreeVoice() & 0xff;
                if (voiceIdx != SpuVoiceCount) {
                    SpuCurrentChannelIdx = (u16)voiceIdx;
                    VoiceSoundId[voiceIdx * 0x1a / 2] = 0x21;
                    VoiceStage[voiceIdx * 0x1a / 2] = soundId;
                    VoiceChannel[voiceIdx * 0x1a / 2] = program;
                    VoiceProgramIdx[voiceIdx * 0x1a / 2] = (u16)SpuCurrentVoiceAttrIdx;
                    VoicePitch[voiceIdx * 0x1a / 2] = SpuCurrentPitch;
                    Spu_DefaultParam[voiceIdx * 0x34 / 2] = volParam1;
                    VoiceStatus[voiceIdx * 0x34] = 1;
                    VoiceAge[voiceIdx * 0x1a / 2] = 0;
                    VoiceBankIdx[voiceIdx * 0x1a / 2] = (u16)SpuVoiceAttrStoredIdx;
                    SpuSetVoiceSample();
                    if (SpuCurrentPitch == 0xff)
                        SpuSetNoiseVoice(voiceIdx);
                    else {
                        envVal = SpuCalcSamplePitch(volParam1, volParam2);
                        SpuSetVoiceVolume(1, envVal, 0, 0);
                    }
                    SpuSeqBusyFlag = 0;
                    return voiceIdx;
                }
            }
        }
        SpuSeqBusyFlag = 0;
    }
    return 0xffffffff;
}
/* ========================================================================
 * 7. SpuStopSeqChannel - Stop a sequence channel
 *
 * Clears the voice state for a given channel index (< 0x18) and
 * removes it from the active channel bitmask.
 *
 * Returns 0 on success, 0xFFFFFFFF on failure.
 * ======================================================================== */
s32 SpuStopSeqChannel(u16 channel)
{
    u32 idx;
    u16 bitLow, bitHigh;
    if (SpuSeqBusyFlag != 1) {
        if (channel < 0x18) {
            idx = (u32)channel;
            bitHigh = (u16)(1 << (idx & 0x1f));
            if (idx < SOUND_CHANNEL_MAX)
                bitLow = 0;
            else {
                bitHigh = 0;
                bitLow = (u16)(1 << (idx - SOUND_CHANNEL_MAX & 0x1f));
            }
            SpuCurrentChannelIdx = channel;
            VoiceStatus[idx * 0x34] = 0;
            VoiceSampleRate[idx * 0x1a / 2] = 0;
            VoicePitch[idx * 0x1a / 2] = 0;
            SpuKeyOnMaskL &= ~(bitHigh | SpuVoiceFreeMaskL);
            SpuKeyOnMaskH &= ~(bitLow | SpuVoiceFreeMaskH);
            SpuSeqBusyFlag = 0;
            SpuVoiceFreeMaskL = bitHigh | SpuVoiceFreeMaskL;
            SpuVoiceFreeMaskH = bitLow | SpuVoiceFreeMaskH;
            return 0;
        }
        SpuSeqBusyFlag = 0;
    }
    return 0xffffffff;
}
/* ========================================================================
 * 8. SpuSetSeqParam - Set sequence playback parameter
 * ======================================================================== */
s32 SpuSetSeqParam(u16 channel, s16 voiceIndex, s16 fadeStartVol, s16 fadeTotalFrames)
{
    if (channel < 0x18) {
        SpuSetVoiceFade((s32)(s16)channel, (s32)voiceIndex, (s32)fadeStartVol, (s32)fadeTotalFrames);
        return 0;
    }
    return 0xffffffff;
}
/* ========================================================================
 * 9. SpuResetAllVoices - Reset all voice state to defaults
 *
 * Clears all voice state fields, resets SPU hardware registers,
 * and removes all voices from the active bitmasks.
 * ======================================================================== */
void SpuResetAllVoices(void)
{
    u32 idx;
    u16 bitLow, bitHigh;
    s32 regOffs;
    u8 i;
    if (SpuVoiceCount != 0) {
        for (i = 0; (s16)i < (s16)(u16)SpuVoiceCount; i++) {
            VoiceAge[i * 0x1a / 2] = 0x18;
            VoicePitch[i * 0x1a / 2] = 0xff;
            VoiceStatus[i * 0x34] = 0;
            VoiceSampleRate[i * 0x1a / 2] = 0;
            VoiceLifetime[i * 0x1a / 2] = 0;
            VoiceSoundId[i * 0x1a / 2] = 0xff;
            VoiceProgramIdx[i * 0x1a / 2] = 0;
            VoiceChannel[i * 0x1a / 2] = 0;
            VoiceBankIdx[i * 0x1a / 2] = 0xff;
            regOffs = i * 0x10;
            *(u16 *)(REG_SPU_VOICE_BASE + regOffs) = 0x200;
            *(u16 *)(0x1f801c04 + regOffs) = FIXED_12;
            *(u16 *)(0x1f801c08 + regOffs) = SPU_NOISE_FREQ;
            *(u16 *)(0x1f801c0a + regOffs) = 0;
            *(u16 *)(0x1f801c0c + regOffs) = 0;
            *(u16 *)(0x1f801c0a + regOffs) = SPU_NOTE_HOLD;
            idx = (u32)(u16)i;
            bitHigh = (u16)(1 << (idx & 0x1f));
            if (idx < SOUND_CHANNEL_MAX)
                bitLow = 0;
            else {
                bitHigh = 0;
                bitLow = (u16)(1 << (idx - SOUND_CHANNEL_MAX & 0x1f));
            }
            SpuCurrentChannelIdx = i;
            VoiceStatus[idx * 0x34] = 0;
            VoiceSampleRate[idx * 0x1a / 2] = 0;
            VoicePitch[idx * 0x1a / 2] = 0;
            SpuVoiceFreeMaskL = bitHigh | SpuVoiceFreeMaskL;
            SpuKeyOnMaskL &= ~SpuVoiceFreeMaskL;
            SpuVoiceFreeMaskH = bitLow | SpuVoiceFreeMaskH;
            SpuKeyOnMaskH &= ~SpuVoiceFreeMaskH;
        }
    }
}
/* ========================================================================
 * 10. SpuSelectProgram - Select sound program for a bank
 *
 * Validates the bank is loaded and program is in range, then sets
 * global program data pointers and current bank/program indices.
 *
 * Returns 0 on success, 0xFFFFFFFF on failure.
 * ======================================================================== */
s32 SpuSelectProgram(u16 bank, s16 program)
{
    s8 bankIdx;
    if (bank >= SOUND_CHANNEL_MAX)
        return 0xffffffff;
    bankIdx = (s8)bank;
    if (SpuBankLoadedFlags[bankIdx] != 1)
        return 0xffffffff;
    if ((s32)program >= (s32)SpuMaxPrograms)
        return 0xffffffff;
    /* Byte-level access: each bank owns 4 bytes in these tables */
    SpuCurrentProgramData = *(u32 *)((u8 *)SpuBankProgramTables + bankIdx * 4);
    SpuCurrentBankData = *(u32 *)((u8 *)SpuBankVabDataPtrs + bankIdx * 4);
    SpuCurrentVoiceAttr = *(u32 *)((u8 *)SpuBankVoiceAttrTables + bankIdx * 4);
    SpuCurrentBankIdx = bankIdx;
    SpuCurrentProgramIdx = (s8)program;
    SpuCurrentVoiceAttrIdx = *(u8 *)(program * 0x10 + SpuCurrentProgramData + 8);
    return 0;
}
/* ========================================================================
 * 11. SpuInitSystem - Initialize SPU sound system
 *
 * Full SPU init: callbacks, cold/hot init, register defaults for 24
 * voices, master volume, voice state allocation, and region query.
 * ======================================================================== */
void SpuInitSystem(s32 isColdBoot)
{
    s32 i, j;
    u16 val;
    volatile u16 *regPtr;
    volatile u16 *volPtr;
    if (isColdBoot == 0)
        SpuInit();
    else
        Sound_InitHot();
    /* Init SPU register shadows: 8 regs x 24 voices */
    regPtr = (volatile u16 *)0x1f801c0a;
    i = 0;
    do {
        j = 0;
        do {
            val = SpuRegDefaults[j];
            *regPtr = val;
            regPtr++;
            j++;
        } while (j < 8);
        i++;
    } while (i < 0x18);
    /* Set master volume */
    volPtr = &SPU_MAIN_VOL_L;
    i = 0;
    do {
        *volPtr = SpuMasterVolDefaults[i];
        volPtr++;
        i++;
    } while (i < SOUND_CHANNEL_MAX);
    SpuInitAllVoices(0x18);
    /* Clear attribute blocks: 32 channels x 0x40 bytes */
    for (i = 0; i < 0x20; i++)
        for (j = 0; j < 0x10; j++)
            *(u32 *)((s32)&SpuAttributeBlockBase + i * 0x40 + j * 4) = 0;
    SpuTickRate = 0x3c;
    SpuOpenChannelMask = 0;
    TimerSpinCounter = 0;
    TimerMode = 0xffffffff;
    TimerCountUp = 0;
    SavedInterruptCB = 0;
    VideoMode = GetVideoMode();
    SpuSeqBusyFlag = 0;
}
/* ========================================================================
 * 12. SpuInitCold - Cold SPU init wrapper
 * ======================================================================== */
void SpuInitCold(void)
{
    SpuInitSystem(0);
}
/* ========================================================================
 * 13. SpuInitVoiceArray - Initialize voice array and clear state
 *
 * Populates the voice pointer table from a base address, then clears
 * all voice state fields and sets default L/R volume to 0x7f.
 * ======================================================================== */
void SpuInitVoiceArray(s32 baseAddr, s16 voiceCount, s16 voicesPerBank)
{
    s32 i, j, voiceOffs;
    s32 *ptrTable;
    SpuTotalVoiceCount = voiceCount;
    SpuVoicesPerBank = voicesPerBank;
    if (0 < voiceCount) {
        ptrTable = (s32 *)SpuVoicePtrTable;
        i = 0;
        do {
            *ptrTable = baseAddr + i * 0xac;
            ptrTable++;
            i += voicesPerBank;
        } while (i < voiceCount);
    }
    for (i = 0; i < SpuTotalVoiceCount; i++) {
        ptrTable = (s32 *)SpuVoicePtrTable + i;
        for (j = 0; j < (s32)SpuVoicesPerBank; j++) {
            voiceOffs = j * 0xac;
            *(u32 *)(voiceOffs + *ptrTable + 0x90) = 0;
            *(u8 *)(voiceOffs + *ptrTable + 0x3c) = 0xff;
            *(u8 *)(voiceOffs + *ptrTable) = 0;
            *(s16 *)(voiceOffs + *ptrTable + 0x3e) = 0;
            *(s16 *)(voiceOffs + *ptrTable + 0x40) = 0;
            *(u32 *)(voiceOffs + *ptrTable + 0x94) = 0;
            *(u32 *)(voiceOffs + *ptrTable + 0x98) = 0;
            *(s16 *)(voiceOffs + *ptrTable + 0x42) = 0;
            *(u32 *)(voiceOffs + *ptrTable + 0xa4) = 0;
            *(u32 *)(voiceOffs + *ptrTable + 0xa0) = 0;
            *(u32 *)(voiceOffs + *ptrTable + 0x9c) = 0;
            *(s16 *)(voiceOffs + *ptrTable + 0x44) = 0;
            *(s16 *)(voiceOffs + *ptrTable + 0x74) = SOUND_VOLUME_DEFAULT;
            *(s16 *)(voiceOffs + *ptrTable + 0x76) = SOUND_VOLUME_DEFAULT;
            *(s16 *)(voiceOffs + *ptrTable + 0x78) = SOUND_VOLUME_DEFAULT;
            *(s16 *)(voiceOffs + *ptrTable + 0x7a) = SOUND_VOLUME_DEFAULT;
        }
    }
}
/* ========================================================================
 * 14. SpuSetFrameRate - Set frame rate / tick rate
 *
 * Configures tick interval (SpuTickRate) based on setting.
 * Bit 0x1000 indicates PAL mode. Supports 0,1,2,3,4,5 and >5.
 * ======================================================================== */
void SpuSetFrameRate(u32 setting)
{
    bool isPal;
    isPal = (setting & 0x1000) != 0;
    FrameRateSetting = setting;
    if (isPal)
        FrameRateSetting = setting & 0xfff;
    FrameRatePalFlag = (u32)isPal;
    if (5 < (s32)FrameRateSetting) {
        SpuTickRate = FrameRateSetting;
        return;
    }
    switch (FrameRateSetting) {
    case 0:
        break;
    case 1:
        if (VideoMode == 0) {
            FrameRateSetting = 5;
            SpuTickRate = 0x3c;
        } else {
            FrameRateSetting = 0x3c;
            SpuTickRate = 0x3c;
        }
        return;
    case 2:
        SpuTickRate = 0xf0;
        return;
    case 3:
        SpuTickRate = 0x78;
        return;
    case 4:
        if (VideoMode == 1) {
            FrameRateSetting = 5;
            SpuTickRate = 0x32;
        } else {
            FrameRateSetting = 0x32;
            SpuTickRate = 0x32;
        }
        return;
    case 5:
        break;
    default:
        goto useDefault;
    }
    if ((VideoMode == 0) || (VideoMode != 1))
useDefault:
        SpuTickRate = 0x3c;
    else
        SpuTickRate = 0x32;
}
/* ========================================================================
 * 15. SpuStartTimer - Start SPU/VSync timer
 *
 * Configures and starts a root counter for SPU timing. For frame rates
 * 2 and 3, uses fixed counter values; for others, computes divider.
 * Registers appropriate interrupt callbacks.
 *
 * WARNING: unreachable blocks at 0x80032838/0x8003288c removed.
 * ======================================================================== */
void SpuStartTimer(s32 arg)
{
    s32 i;
    u32 count;
    u32 spec;
    /* Spin wait */
    for (i = 0x3e6; -1 < i; i--);
    if (FrameRateSetting == 2) {
        count = 0x44e8;
        spec = 0xf2000002;
        TimerMode = 6;
    } else if (FrameRateSetting < 3) {
        if (FrameRateSetting == 0) return;
    } else {
        if (FrameRateSetting == 3) {
            count = 0x89d0;
            spec = 0xf2000002;
            TimerMode = 6;
        } else if (FrameRateSetting == 5) {
            if (arg == 0) {
                TimerSpinCounter++;
                return;
            } else {
                spec = 0xf2000003;
                TimerMode = 0;
                count = 1;
            }
            goto setupTimer;
        }
    }
    if (FrameRatePalFlag != 0) return;
    TimerMode = 6;
    spec = 0xf2000002;
    if (FrameRateSetting < 0x46) {
        count = 0x204cc0 / FrameRateSetting;
        TimerCountUp++;
    } else {
        count = 0x409980 / FrameRateSetting;
    }
setupTimer:
    if (TimerSpinCounter == 0) {
        EnterCriticalSection();
        ResetRCnt(spec);
        SetRCnt(spec, count & 0xffff, 0x1000);
        for (i = 0x7ce; -1 < i; i--);
        for (i = 0x7ce; -1 < i; i--);
        StartRCnt(spec);
        if (TimerMode == 0) {
            SavedInterruptCB = (void *) InterruptCallback (0, 0);
             InterruptCallback (6, &TimerCallback_SpuSet);
        } else {
            if (TimerCountUp == 0)
                 InterruptCallback (6, &TimerCallback_Default);
            else
                 InterruptCallback (6, &TimerCallback_VSync);
        }
        ExitCriticalSection();
    } else {
        VSyncCallback(TimerCallback_Default);
    }
}
/* ========================================================================
 * 16. SpuStartTimerHot - Start timer, hot init
 * ======================================================================== */
void SpuStartTimerHot(void)
{
    SpuStartTimer(1);
}
/* ========================================================================
 * 17. SpuStopTimer - Stop SPU/VSync timer
 *
 * Restores previous interrupt callback or clears VSync callback.
 * ======================================================================== */
void SpuStopTimer(void)
{
    s32 i;
    void *cb;
    if (FrameRatePalFlag != 0) return;
    TimerCountUp = 0;
    EnterCriticalSection();
    if (TimerSpinCounter == 0) {
        if (TimerMode != -1) {
            i = TimerMode;
            cb = 0;
            if (TimerMode == 0) {
                i = 0;
                cb = SavedInterruptCB;
            }
             InterruptCallback (i, cb);
            TimerMode = -1;
        }
    } else {
        VSyncCallback(0);
        TimerSpinCounter = 0;
    }
    ExitCriticalSection();
}
/* ========================================================================
 * 18. SpuCallInit - Delegate SPU initialization
 * ======================================================================== */
void SpuCallInit(void)
{
    SpuQuit();
}
/* ========================================================================
 * 19. SpuMallocSimple - SPU memory allocation convenience
 *
 * Wraps SpuMallocInternal with skipAlloc=0, spuAddr=0.
 * Returns bank index or -1 on failure.
 * ======================================================================== */
s32 SpuMallocSimple(u32 *vabData, s16 bankIndex)
{
    return SpuMallocInternal(vabData, (s32)bankIndex, 0, 0);
}
/* ========================================================================
 * 20. SpuMallocInternal - Allocate SPU memory for VAB bank
 *
 * Parses a VAB sound bank header, counts programs and voices,
 * builds a size table, allocates SPU memory via PsyqSpuMalloc(),
 * and writes program SPU offset values into the program table.
 *
 * vabData:   VAB file data pointer
 * bankIndex: target bank (-1 = auto-assign)
 * skipAlloc: 0 = allocate, non-zero = use spuAddr directly
 * spuAddr:   pre-allocated SPU address (used when skipAlloc != 0)
 *
 * Returns bank index on success, -1 on failure.
 * ======================================================================== */
s32 SpuMallocInternal(u32 *vabData, s32 bankIndex, s16 skipAlloc, s32 spuAddr)
{
    u8   progCount;
    s16  bankId;
    s32  totalSize;
    s32  *sizeTable;
    u32 *progTable;   /* base of program entries (vabData + 8), preserved */
    u32 *iter;        /* iterator over program entries */
    u32 *voiceTbl;    /* voice attribute table pointer */
    u32  tmp;
    s32  i, j;
    s32  local_428[0x100];
    s32  vabVersion;
    bankId = 0x10;
    if (SpuGetInTransfer() == 1) return -1;
    SpuSetInTransfer(1);
    i = (s32)(s16)bankIndex;
    if (0xf < i) {
        SpuSetInTransfer(0);
        return -1;
    }
    /* Find or assign bank slot */
    if (i == -1) {
        for (j = 0; j < SOUND_CHANNEL_MAX; j++) {
            if (SpuBankLoadedFlags[j] == '\0') {
                SpuBankLoadedFlags[j] = 1;
                bankIndex = j;
                goto found;
            }
        }
    } else {
        if (SpuBankLoadedFlags[i] == '\0') {
            SpuBankLoadedFlags[i] = 1;
found:
            SpuLoadedBankCount++;
            bankId = (s16)bankIndex;
        }
    }
    if (bankId >= SOUND_CHANNEL_MAX)
        goto cleanup;
    /* Store VAB data pointer for this bank */
    *(u32 *)((u8 *)SpuBankVabDataPtrs + bankId * 4) = (u32)vabData;
    /* Validate VAB signature: top byte = 'VAB' (SOUND_VAB_MAGIC) */
    if (*vabData >> 8 != SOUND_VAB_MAGIC) {
        SpuBankLoadedFlags[bankId] = 0;
        goto cleanup;
    }
    SpuMaxPrograms = 0x40; /* default max programs */
    vabVersion = *vabData & 0xff;
    if ((vabVersion == 0x70) && (4 < (s32)vabData[1]))
        SpuMaxPrograms = 0x80;
    if (*(u16 *)((s32)vabData + 0x12) > (s32)SpuMaxPrograms)
        goto clearBank;
    /* Program table starts at vabData + 8. Store base for later writes. */
    progTable = vabData + 8;
    *(u32 *)((u8 *)SpuBankProgramTables + bankId * 4) = (u32)progTable;
    /* Count active programs using iterator (does not modify progTable) */
    i = 0;
    tmp = 0;
    iter = progTable;
    if (SpuMaxPrograms != 0) {
        do {
            iter[2] = tmp;
            if ((s8)*iter != '\0')
                tmp++;
            i++;
            iter += 4; /* 4 u32 = 16 bytes per program entry */
        } while (i < (s32)SpuMaxPrograms);
    }
    /* iter now points past program table = start of voice attribute data */
    i = 0;
    sizeTable = local_428;
    *(u32 *)((u8 *)SpuBankVoiceAttrTables + bankId * 4) = (u32)iter;
    progCount = *(u8 *)((s32)vabData + 0x16);
    /* Skip past voice attribute entries (numVoices * 0x80 bytes) */
    voiceTbl = (u32 *)((u8 *)iter +
                       (u32)*(u16 *)((s32)vabData + 0x12) * 0x80);
    /* Build size table from the SPU offset table entries (2 bytes each).
       voiceTbl points past voice attribute data to the offset table. */
    do {
        if ((s32)sizeTable <= (s32)(sizeTable + progCount)) {
            totalSize = (u32)(u16)*(u16 *)voiceTbl << 2;
            if (4 < (s32)vabData[1])
                totalSize = (u32)(u16)*(u16 *)voiceTbl << 3;
            *sizeTable = totalSize;
            i += *sizeTable;
        }
        sizeTable++;
        voiceTbl = (u32 *)((u8 *)voiceTbl + 2);
    } while ((s32)sizeTable < (s32)(local_428 + progCount + 1));
    *(u32 *)((u8 *)SpuBankVoiceOffsetTable + bankId * 4) = (u32)voiceTbl;
    /* Allocate SPU memory if requested */
    if ((skipAlloc == 0) && (spuAddr = PsyqSpuMalloc(i), spuAddr == -1)) {
        SpuBankLoadedFlags[bankId] = 0;
        goto cleanup;
    }
    if ((u32)(spuAddr + i) >= 0x80001)
        goto clearBank;
    *(s32 *)((u8 *)SpuBankSpuAddrTable + bankId * 4) = spuAddr;
    /* Write program SPU offsets into the program table (using progTable base).
       Each 16-byte program entry gets offsets at +12 and +14 bytes. */
    tmp = 0;
    j = 0;
    sizeTable = local_428;
    do {
        j += *sizeTable;
        if ((tmp & 1) == 0) {
            /* Even index: write at +3 u32 = byte offset +12 */
            *(s16 *)(progTable + ((s32)tmp / 2) * 4 + 3) =
                (s16)((u32)(spuAddr + j) >> 3);
        } else {
            /* Odd index: write at byte offset ((tmp/2)*16 + 14) */
            *(s16 *)((u8 *)progTable + ((s32)tmp / 2) * 0x10 + 0xe) =
                (s16)((u32)(spuAddr + j) >> 3);
        }
        tmp++;
        sizeTable++;
    } while ((s32)tmp <= (s32)(u32)progCount);
    *(s32 *)((u8 *)SpuBankTotalSizes + bankId * 4) = j;
    SpuBankLoadedFlags[bankId] = 2;
    SpuSetInTransfer(0);
    return bankId;
clearBank:
    SpuBankLoadedFlags[bankId] = 0;
cleanup:
    SpuSetInTransfer(0);
    SpuLoadedBankCount--;
    return -1;
}

/* === merged from stubs.c === */
#ifdef delete
#undef delete
#endif
#ifdef trap
#undef trap
#endif
/* PsyQ library wrappers — call the real PsyQ implementation */
int  func_80018390 (int mode) { return CdReset(mode); }
u_long  func_80036a54 (void *func) { return CdSyncCallback((void (*)())func); }
u_long  func_8002cc28 (void *func) { return CdReadyCallback((void (*)())func); }
u_long  func_80030404 (void *func) { return CdReadCallback((void (*)())func); }
char * func_80030584 (char *dst, const char *src) { return strcpy(dst, src); }
char * func_80026cfc (char *dst, const char *src) { return strcat(dst, src); }
int  func_80026cac (u_char com, u_char *param, u_char *result) { return CdControl(com, param, result); }
int  func_80026ce8 (u_char com, u_char *param) { return CdControlF(com, param); }
CdlFILE * func_80026e0c (CdlFILE *f, char *name) { return CdSearchFile(f, name); }
int  func_80026e38 (int mode, u_char *result) { return CdSync(mode, result); }
int  func_80032368 (void *func) { return CdDataCallback((void (*)())func); }
int  func_8005cbc8 (int mode, u_char *result) { return CdReady(mode, result); }
extern int Sound_AdvanceDataPtrEx(int bank, int voiceIndex);
/* SoundAdvanceDataPointer alias — ASM callers use short voiceIndex */
int SoundAdvanceDataPointer(int bank, short voiceIndex)
{
    return Sound_AdvanceDataPtrEx(bank, voiceIndex);
}
void __attribute__((used)) CdDaMode_GetVtable2(void) { (void)0; }

