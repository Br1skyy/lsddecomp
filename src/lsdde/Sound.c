#include "common.h"
#include <libcd.h>
#include "libspu.h"
/*
 * Sound.c - PS1 SPU audio system for LSD: Dream Emulator
 *
 * Manages SPU voice attributes, sound bank loading/unloading,
 * reverb effects, and voice playback control via the PsyQ SPU library.
 *
 * Voice struct layout (0xAC bytes per voice):
 *   0x00: reserved
 *   0x04: dataPtr (current read position in sequence data)
 *   0x08: dataBasePtr (start of sequence data)
 *   0x0c: loopStartPtr (loop restart position)
 *   0x10: mode
 *   0x11: keyOff
 *   0x12: channelIndex
 *   0x13: effect1Type
 *   0x14: effect2Type
 *   0x15: volumeParam
 *   0x16: sfxType
 *   0x17-0x26: volume[16] (per-channel volume levels)
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
 * Global data:
 *   SpuVoicePtrTable: Array of voice array pointers, one per sound bank
 *   SpuVoicesPerBank: Voices per bank
 *   SpuBankLoadedFlags: Bank state array (0=free, 1=loaded, 2=pending)
 *   SpuBankSpuAddrTable: SPU memory addresses per bank
 *   SpuBankTotalSizes: SPU data sizes per bank
 *   SpuLoadedBankCount: Loaded bank count
 *   SpuCurrentProgramData: Sound program base pointer (set by SpuSelectProgram)
 *   SpuCurrentVoiceAttr: Voice attribute data base pointer
 *   SpuCurrentVoiceAttrIdx: Current voice index within program
 *   SoundReverbAttr-268: Reverb mode parameters
 *   SoundDamperMode: Damper mode (0=off, 2=on)
 *   SpuTickRate: Frames per second / tick rate
 */
/* Vtable pointer placeholders (provided by linker) */
#ifndef SPU_VTABLE_SEQPLAYER
#define SPU_VTABLE_SEQPLAYER 0
#endif
#ifndef SPU_VTABLE_OBJECTB
#define SPU_VTABLE_OBJECTB 0
#endif
#ifndef SPU_VTABLE_OBJECTC
#define SPU_VTABLE_OBJECTC 0
#endif
/* === extern declarations (moved from stubs.c) === */
extern short SpuCurrentChannelIdx;
extern int SpuVoiceCount;
extern short VoiceSoundId[];
extern short SpuCurrentVoiceId;
extern int VoiceStatus[];
extern int VoiceBankIdx[];
extern int VoicePitch[];
extern unsigned int SpuKeyOnMaskL, SpuKeyOnMaskH;
extern unsigned int Spu_ReverbMaskL, Spu_ReverbMaskH;
extern int Scene_CurrentId;
extern int DreamInstantTeleportFlag;
extern int New_Entity(int a, int b, int c);
extern int Other_SoundEntityParam;
extern void SetSpuParam(int a, int b);
extern void InitSpu(int a);
extern int SoundDspProcess(unsigned int param, int mode);
extern int SoundGetTick(void);
extern int SoundGetMode(int mode);
/* Forward declarations for functions defined in other translation units */
extern s32 SpuSelectProgram(u16 bank, s16 program);
extern void SpuStopVoice(s32 id);
extern void SpuSetInTransfer(s32 flag);
/* Forward declarations for functions in this file */
static int GetVoicePtr(s16 bank, s16 voiceIndex);
u32  Sound_ReadVoiceAttr(s16 bank, s16 program, int voiceOff, u8 *out);
void Sound_SetMasterVolume(s16 leftVol, s16 rightVol);
u32  Sound_ReadProgramHeader(s16 bank, s16 voiceIndex, u8 *out);
s32  Sound_LoadBank(u8 *data, u16 bankIndex);
void Sound_SetMute(s8 param);
s32  Sound_IsTransferComplete(s16 checkFlag);
void Sound_FreeBank(u16 bankIndex);
void Sound_PlayVoice(u16 bank, s16 voiceIndex, u8 playMode);
void Sound_StopVoice(s16 bank, s16 voiceIndex);
void Sound_ChangeVolume(u16 bank, s16 voiceIndex, u8 newVol);
void Sound_SetVoiceType(s16 bank, s16 voiceIndex, s8 type);
void Sound_SetEffect1(s16 bank, s16 voiceIndex, u8 effectType);
void Sound_SetEffect2(s16 bank, s16 voiceIndex, u8 effectType);
void Sound_ProcessSfx(s16 bank, s16 voiceIndex, u8 sfxParam);
void Sound_ProcessSpuRegister(s16 bank, s16 voiceIndex, s16 regType, u32 regValue);
s32  Sound_AdvanceDataPointer(void);
void Sound_UnpackSfxFields(u32 word1, u32 word2, u16 *outFields);
void Sound_PackSfxFields(u16 *inFields, u32 *outWord1, u16 *outWord2);
void Sound_SetReverbOn(void);
void Sound_SetReverbOff(void);
s32  Sound_SetReverbMode(u16 mode);
void Sound_SetReverbVolume(s16 leftVol, s16 rightVol);
void Sound_SetReverbDelay(s16 delay);
void Sound_SetReverbFeedback(s16 feedback);
u32  Sound_WriteVoiceAttr(s16 bank, s16 program, int voiceOff, u8 *data);
void Sound_DamperOn(void);
void Sound_DamperOff(void);
void Sound_ResetVoice(s16 bank, s16 voiceIndex);
void Sound_ResetVoiceBank(s16 bank);
void Sound_InitHot(void);
/* ---- Global data references ---- */
extern u32 SpuVoicePtrTable[];
extern u32 SpuVoicesPerBank;
extern u32 SpuBankLoadedFlags[];
extern u32 SpuBankSpuAddrTable[];
extern u32 SpuBankTotalSizes[];
extern u32 SpuLoadedBankCount;
extern s32 SpuCurrentProgramData;
extern s32 SpuCurrentVoiceAttr;
extern u8  SpuCurrentVoiceAttrIdx;
extern u8  SpuCurrentBankIdx;
extern u8  SpuCurrentProgramIdx;
extern s32 SoundReverbAttr;
extern s32 SoundReverbMode;
extern s32 SoundReverbDepthLeft;
extern s32 SoundReverbDepthRight;
extern s32 SoundReverbFeedback;
extern s32 SoundReverbDelay;
extern u32 SoundDamperMode;
extern u32 SpuTickRate;
/* ---- Helper: compute voice struct pointer from bank + voice index ---- */
static int GetVoicePtr(s16 bank, s16 voiceIndex)
{
    return voiceIndex * 0xac + SpuVoicePtrTable[bank];
}
/* ========================================================================
 * 1. Sound_ReadVoiceAttr - Read voice attribute data from SPU buffer
 *
 * Reads 14 bytes + 4 halfwords of voice attribute data from the
 * voice attribute buffer for a given voice within a sound bank program.
 * Used to snapshot current SPU voice state before modification.
 *
 * bank:      Sound bank index (0-15)
 * program:   Program number within the bank
 * voiceOff:  Byte offset into the program's voice table
 * out:       Output buffer (18 bytes: 14 u8 + 4 u16)
 * Returns 0 on success, -1 on failure.
 * ======================================================================== */
u32 Sound_ReadVoiceAttr(s16 bank, s16 program, int voiceOff, u8 *out)
{
    int voiceAttrOffset;
    int i;
    if (SpuBankLoadedFlags[bank] != 1) {
        return (u32)-1;
    }
    SpuSelectProgram(bank, program);
    /* Compute byte offset into voice attribute data buffer */
    voiceAttrOffset = (int)(((voiceOff + (u32)SpuCurrentVoiceAttrIdx * 0x10) * 0x10000) >> 0xb);
    /* Copy 14 bytes of voice attribute data */
    for (i = 0; i < 14; i++) {
        out[i] = ((u8 *)(voiceAttrOffset + SpuCurrentVoiceAttr))[i];
    }
    /* Copy 4 halfwords (8 bytes) of extended voice data */
    {
        u8 *src = (u8 *)(voiceAttrOffset + SpuCurrentVoiceAttr);
        *(u16 *)(out + 0x10) = *(u16 *)(src + 0x10);
        *(u16 *)(out + 0x12) = *(u16 *)(src + 0x12);
        *(u16 *)(out + 0x14) = *(u16 *)(src + 0x14);
        *(u16 *)(out + 0x16) = *(u16 *)(src + 0x16);
    }
    return 0;
}
/* ========================================================================
 * 2. Sound_SetMasterVolume - Set SPU master volume (left/right)
 *
 * Sets the SPU main output volume for left and right channels.
 * Each value is scaled by 0x81 to convert from 0-127 to SPU range.
 *
 * leftVol:   Left channel volume (0-127)
 * rightVol:  Right channel volume (0-127)
 * ======================================================================== */
void Sound_SetMasterVolume(s16 leftVol, s16 rightVol)
{
    SpuCommonAttr attr;
    attr.mask = 3;  /* mask bits for mvol.left and mvol.right */
    attr.mvol.left = leftVol * 0x81;
    attr.mvol.right = rightVol * 0x81;
    SpuSetCommonAttr(&attr);
}
/* ========================================================================
 * 3. Sound_ReadProgramHeader - Read program header from sound data buffer
 *
 * Reads 5 bytes + 1 halfword of program header data from the sound
 * program buffer. Used to get voice count and per-voice config.
 *
 * bank:       Sound bank index
 * voiceIndex: Voice index within the bank
 * out:        Output buffer (8 bytes: 5 u8 + 1 u16)
 * Returns 0 on success, -1 on failure.
 * ======================================================================== */
u32 Sound_ReadProgramHeader(s16 bank, s16 voiceIndex, u8 *out)
{
    int offset;
    u8 *src;
    if (SpuBankLoadedFlags[bank] != 1) {
        return (u32)-1;
    }
    SpuSelectProgram(bank, voiceIndex);
    offset = voiceIndex * 0x10;
    src = (u8 *)(offset + SpuCurrentProgramData);
    /* Copy 5 bytes of program header */
    out[0] = src[0];
    out[1] = src[1];
    out[2] = src[2];
    out[3] = src[3];
    out[4] = src[4];
    /* Copy 1 halfword (voice count / flags) */
    *(u16 *)(out + 6) = *(u16 *)(src + 6);
    return 0;
}
/* ========================================================================
 * 4. Sound_LoadBank - Load sound bank data into SPU memory
 *
 * Loads PCM/waveform data from the game buffer into SPU RAM.
 * The sound bank must be in state 2 (pending) to be loaded.
 * After loading, the bank state transitions to 1 (loaded).
 *
 * data:       Pointer to sound data in main RAM
 * bankIndex:  Bank to load (0-16)
 * Returns bank index on success, -1 on failure.
 * ======================================================================== */
s32 Sound_LoadBank(u8 *data, u16 bankIndex)
{
    u32 spuAddr;
    s32 idx;
    if (bankIndex >= SOUND_BANK_MAX) {
        SpuSetInTransfer(0);
        return -1;
    }
    idx = (s32)(s16)bankIndex;
    if (SpuBankLoadedFlags[idx] != 2) {
        SpuSetInTransfer(0);
        return -1;
    }
    spuAddr = SpuBankSpuAddrTable[idx];
    SpuSetTransferMode(0);
    SpuSetTransferStartAddr(spuAddr);
    SpuRead(data, SpuBankTotalSizes[idx]);
    SpuBankLoadedFlags[idx] = 1;
    return idx;
}
/* ========================================================================
 * 5. Sound_SetMute - Enable or disable SPU muting
 *
 * param: 0 = mute off, 1 = mute on, other = ignore
 * ======================================================================== */
void Sound_SetMute(s8 param)
{
    long on_off;
    if (param == 0) {
        on_off = 0;
    } else if (param == 1) {
        on_off = 1;
    } else {
        return;
    }
    SpuSetMute(on_off);
}
/* ========================================================================
 * 6. Sound_IsTransferComplete - Check if SPU DMA transfer is done
 *
 * checkFlag: If nonzero, waits for completion; if zero, just queries
 * Returns 1 if complete, 0 if still in progress.
 * ======================================================================== */
s32 Sound_IsTransferComplete(s16 checkFlag)
{
    long result;
    result = SpuIsTransferCompleted((s32)checkFlag);
    return (s32)(s16)result;
}
/* ========================================================================
 * 7. Sound_FreeBank - Free a loaded sound bank from SPU memory
 *
 * Releases the SPU memory allocated for a sound bank and resets
 * its state to free (0). Decrements the loaded bank counter.
 *
 * bankIndex: Bank to free (0-15)
 * ======================================================================== */
void Sound_FreeBank(u16 bankIndex)
{
    s32 idx;
    if (bankIndex < SOUND_CHANNEL_MAX) {
        idx = (s32)(s16)bankIndex;
        if (SpuBankLoadedFlags[idx] == 1) {
            SpuFree(SpuBankSpuAddrTable[idx]);
            SpuBankLoadedFlags[idx] = 0;
            SpuLoadedBankCount = SpuLoadedBankCount - 1;
        }
    }
}
/* ========================================================================
 * 8. Sound_PlayVoice - Play a voice with pitch/volume control
 *
 * Reads program data to determine the number of sub-voices, then
 * configures each sub-voice's volume and pitch based on playMode.
 * Starts playback by updating the data pointer.
 *
 * bank:       Sound bank index
 * voiceIndex: Voice index
 * playMode:   Pitch/volume mode (0x00-0x3F = low, 0x40-0x7F = mid)
 * ======================================================================== */
void Sound_PlayVoice(u16 bank, s16 voiceIndex, u8 playMode)
{
    s32 voicePtr;
    s32 channelDataPtr;
    u32 chIdx;
    u8 header[16];
    u8 voiceAttr[18];
    s32 i;
    s32 voiceOff;
    voicePtr = GetVoicePtr((s16)bank, voiceIndex);
    /* channelDataPtr = voicePtr + channelOrder[channelIndex] */
    chIdx = (u32)*(u8 *)(voicePtr + 0x12);
    channelDataPtr = voicePtr + chIdx;
    /* Read program header: voice count and channel config */
    Sound_ReadProgramHeader(
        (s32)*(s16 *)(voicePtr + 0x4c),
        *(u8 *)(channelDataPtr + 0x2c),
        header
    );
    /* Configure each sub-voice's attributes */
    i = 0;
    voiceOff = 0;
    if (header[0] != 0) {
        do {
            /* Read voice attributes for this sub-voice */
            Sound_ReadVoiceAttr(
                (s32)*(s16 *)(voicePtr + 0x4c),
                *(u8 *)(channelDataPtr + 0x2c),
                voiceOff >> 16,
                voiceAttr
            );
            /* Set pitch mode based on playMode range */
            if (playMode < 0x40) {
                voiceAttr[7] = 2;  /* low pitch range */
            } else if ((u8)(playMode - 0x40) < 0x40) {
                voiceAttr[7] = 0;  /* mid pitch range */
            }
            i++;
            /* Write back modified attributes */
            Sound_WriteVoiceAttr(
                (s32)*(s16 *)(voicePtr + 0x4c),
                *(u8 *)(channelDataPtr + 0x2c),
                voiceOff >> 16,
                voiceAttr
            );
            voiceOff = voiceOff + 0x10000;
        } while (i < (s32)(u32)header[0]);
    }
    /* Start playback by advancing data pointer */
    *(u32 *)(voicePtr + 0x88) = Sound_AdvanceDataPointer();
}
/* ========================================================================
 * 9. Sound_StopVoice - Stop a voice and reset its state
 *
 * Turns off reverb, disables damper, restores default values for
 * volume, pitch, and effect parameters. Resets playback pointer.
 *
 * bank:       Sound bank index
 * voiceIndex: Voice index
 * ======================================================================== */
void Sound_StopVoice(s16 bank, s16 voiceIndex)
{
    s32 voicePtr;
    u32 chIdx;
    voicePtr = GetVoicePtr(bank, voiceIndex);
    Sound_SetReverbOff();
    Sound_DamperOff();
    /* Restore channel order index */
    chIdx = (u32)*(u8 *)(voicePtr + 0x12);
    *(u8 *)(voicePtr + chIdx + 0x2c) = *(u8 *)(voicePtr + 0x12);
    /* Reset effect types */
    *(u8 *)(voicePtr + 0x13) = 0;
    *(u8 *)(voicePtr + 0x14) = 0;
    /* Reset volume to default (0x7f) */
    *(u16 *)(chIdx * 2 + voicePtr + 0x4e) = SOUND_VOLUME_DEFAULT;
    /* Reset volume parameter to default (0x40) */
    *(u8 *)(voicePtr + chIdx + 0x17) = SOUND_VOLUME_HALF;
    /* Advance data pointer and store */
    *(u32 *)(voicePtr + 0x88) = Sound_AdvanceDataPointer();
}
/* ========================================================================
 * 10. Sound_ChangeVolume - Change volume with damper/hold logic
 *
 * Sets a new volume value for a voice. If damper mode is active
 * and no pending effects, sets the damper counter. Otherwise,
 * sets the volume parameter and increments the change counter.
 * Also triggers a callback if the voice type is 0x28.
 *
 * bank:       Sound bank index
 * voiceIndex: Voice index
 * newVol:     New volume value
 * ======================================================================== */
void Sound_ChangeVolume(u16 bank, s16 voiceIndex, u8 newVol)
{
    s32 voicePtr;
    s8 sfxType;
    voicePtr = GetVoicePtr((s16)bank, voiceIndex);
    /* If damper mode is 1 and no mode active, set damper counter */
    if ((*(s8 *)(voicePtr + 0x27) == 1) && (*(s8 *)(voicePtr + 0x10) == 0)) {
        *(u8 *)(voicePtr + 0x28) = newVol;
        *(u8 *)(voicePtr + 0x10) = 1;
    }
    else {
        sfxType = *(s8 *)(voicePtr + 0x16);
        /* Skip if sfxType is 0x1e or 0x14 */
        if (sfxType == SOUND_TYPE_AMBIENT || sfxType == SOUND_TYPE_SFX) {
            goto check_callback;
        }
        /* Set volume parameter and increment change counter */
        *(u8 *)(voicePtr + 0x15) = newVol;
        *(s8 *)(voicePtr + 0x2a) = *(s8 *)(voicePtr + 0x2a) + 1;
    }
check_callback:
    sfxType = *(s8 *)(voicePtr + 0x16);
    /* If sfxType is 0x28, invoke callback from table */
    if (sfxType == SOUND_TYPE_VOICE) {
        /* Callback table: Unk_AttrBlock68 + voiceIndex * 4 + bank * 0x40 */
        s32 tableEntry = voiceIndex * 4 + (s16)bank * 0x40;
        if (tableEntry != 0) {
            ((void (*)(s32, s32, u8))tableEntry)(
                (s32)(s16)bank, (s32)voiceIndex, newVol
            );
        }
    }
    *(u32 *)(voicePtr + 0x88) = Sound_AdvanceDataPointer();
}
/* ========================================================================
 * 11. Sound_SetVoiceType - Set voice type and manage damper/playback
 *
 * Handles different sfxType values (0x14, 0x1e, and others).
 * For type 0x14: enables damper mode and saves loop pointer.
 * For type 0x1e: decrements damper counter; stops if exhausted.
 * For other types: sets the type directly and increments counter.
 *
 * bank:       Sound bank index
 * voiceIndex: Voice index
 * type:       SFX type to set
 * ======================================================================== */
void Sound_SetVoiceType(s16 bank, s16 voiceIndex, s8 type)
{
    s32 voicePtr;
    voicePtr = GetVoicePtr(bank, voiceIndex);
    if (type == SOUND_TYPE_SFX) {
        /* Enable damper mode */
        *(u8 *)(voicePtr + 0x16) = SOUND_TYPE_SFX;
        *(u8 *)(voicePtr + 0x27) = 1;
        *(u32 *)(voicePtr + 0x88) = Sound_AdvanceDataPointer();
        *(u32 *)(voicePtr + 0x0c) = *(u32 *)(voicePtr + 4);
    }
    else if (type == SOUND_TYPE_AMBIENT) {
        /* Decrement damper counter */
        *(u8 *)(voicePtr + 0x16) = SOUND_TYPE_AMBIENT;
        if (*(u8 *)(voicePtr + 0x28) != 0) {
            if (*(u8 *)(voicePtr + 0x28) > 0x7e) {
                /* Damper exhausted - stop voice */
                Sound_AdvanceDataPointer();
                *(u32 *)(voicePtr + 0x88) = 0;
                *(u32 *)(voicePtr + 4) = *(u32 *)(voicePtr + 0x0c);
                return;
            }
            *(u8 *)(voicePtr + 0x28) = *(u8 *)(voicePtr + 0x28) - 1;
            *(u32 *)(voicePtr + 0x88) = Sound_AdvanceDataPointer();
            if (*(s8 *)(voicePtr + 0x28) != 0) {
                /* Still active - restore loop start */
                *(u32 *)(voicePtr + 4) = *(u32 *)(voicePtr + 0x0c);
                return;
            }
            /* Damper counter reached zero */
            *(u8 *)(voicePtr + 0x10) = 0;
            return;
        }
        /* No damper active */
        *(u8 *)(voicePtr + 0x10) = 0;
    }
    else {
        /* Default: set type directly */
        *(s8 *)(voicePtr + 0x16) = type;
        *(s8 *)(voicePtr + 0x2a) = *(s8 *)(voicePtr + 0x2a) + 1;
    }
    *(u32 *)(voicePtr + 0x88) = Sound_AdvanceDataPointer();
}
/* ========================================================================
 * 12. Sound_SetEffect1 - Set effect type 1 and increment change counter
 *
 * bank:       Sound bank index
 * voiceIndex: Voice index
 * effectType: Effect type value
 * ======================================================================== */
void Sound_SetEffect1(s16 bank, s16 voiceIndex, u8 effectType)
{
    s32 voicePtr;
    voicePtr = GetVoicePtr(bank, voiceIndex);
    *(u8 *)(voicePtr + 0x13) = effectType;
    *(s8 *)(voicePtr + 0x29) = *(s8 *)(voicePtr + 0x29) + 1;
    *(u32 *)(voicePtr + 0x88) = Sound_AdvanceDataPointer();
}
/* ========================================================================
 * 13. Sound_SetEffect2 - Set effect type 2 and increment change counter
 *
 * bank:       Sound bank index
 * voiceIndex: Voice index
 * effectType: Effect type value
 * ======================================================================== */
void Sound_SetEffect2(s16 bank, s16 voiceIndex, u8 effectType)
{
    s32 voicePtr;
    voicePtr = GetVoicePtr(bank, voiceIndex);
    *(u8 *)(voicePtr + 0x14) = effectType;
    *(s8 *)(voicePtr + 0x29) = *(s8 *)(voicePtr + 0x29) + 1;
    *(u32 *)(voicePtr + 0x88) = Sound_AdvanceDataPointer();
}
/* ========================================================================
 * 14. Sound_ProcessSfx - Complex sound effect processor
 *
 * Main sound effect dispatch. Handles volume changes, effect type
 * changes, pitch adjustments, and special SPU register writes
 * based on the current voice state. The most complex function in
 * the sound system with multiple processing paths.
 *
 * bank:       Sound bank index
 * voiceIndex: Voice index
 * sfxParam:   Sound effect parameter
 * ======================================================================== */
void Sound_ProcessSfx(s16 bank, s16 voiceIndex, u8 sfxParam)
{
    s32 voicePtr;
    u32 chIdx;
    s32 i;
    s32 voiceOff;
    u8 voiceAttr[18];
    voicePtr = GetVoicePtr((s16)bank, voiceIndex);
    chIdx = (u32)*(u8 *)(voicePtr + 0x12);
    /* Read current voice attributes */
    Sound_ReadProgramHeader(
        (s32)*(s16 *)(voicePtr + 0x4c),
        *(u8 *)(chIdx + voicePtr + 0x2c),
        voiceAttr
    );
    /* Damper mode with pending effect */
    if ((*(s8 *)(voicePtr + 0x27) == 1) && (*(s8 *)(voicePtr + 0x10) == 0)) {
        *(u8 *)(voicePtr + 0x28) = sfxParam;
        *(u8 *)(voicePtr + 0x10) = 1;
    }
    /* Volume/effect change processing */
    else if (*(s8 *)(voicePtr + 0x16) == SOUND_TYPE_AMBIENT || *(s8 *)(voicePtr + 0x16) == SOUND_TYPE_SFX) {
        if (*(s8 *)(voicePtr + 0x29) == 2) {
            /* Effect change count == 2: process effect changes */
            if (*(s8 *)(voicePtr + 0x13) == 0 && *(s8 *)(voicePtr + 0x14) == 0) {
                /* No effects pending - just update volume */
                voiceOff = 0;
                i = 0;
                if (voiceAttr[0] != 0) {
                    do {
                        Sound_ReadVoiceAttr(
                            (s32)*(s16 *)(voicePtr + 0x4c),
                            *(u8 *)(chIdx + voicePtr + 0x2c),
                            voiceOff >> 16,
                            voiceAttr
                        );
                        Sound_WriteVoiceAttr(
                            (s32)*(s16 *)(voicePtr + 0x4c),
                            *(u8 *)(chIdx + voicePtr + 0x2c),
                            voiceOff >> 16,
                            voiceAttr
                        );
                        i++;
                        voiceOff = i * 0x10000;
                    } while (i < (s32)(u32)voiceAttr[0]);
                }
            }
            if (*(s8 *)(voicePtr + 0x13) == 1 && *(s8 *)(voicePtr + 0x14) == 0) {
                /* Effect type 1 active */
                i = 0;
                if (voiceAttr[0] != 0) {
                    voiceOff = 0;
                    do {
                        Sound_ReadVoiceAttr(
                            (s32)*(s16 *)(voicePtr + 0x4c),
                            *(u8 *)(chIdx + voicePtr + 0x2c),
                            voiceOff >> 16,
                            voiceAttr
                        );
                        Sound_WriteVoiceAttr(
                            (s32)*(s16 *)(voicePtr + 0x4c),
                            *(u8 *)(chIdx + voicePtr + 0x2c),
                            voiceOff >> 16,
                            voiceAttr
                        );
                        i++;
                        voiceOff = i * 0x10000;
                    } while (i < (s32)(u32)voiceAttr[0]);
                }
            }
            if (*(s8 *)(voicePtr + 0x13) == 2 && *(s8 *)(voicePtr + 0x14) == 0) {
                /* Effect type 2 active */
                i = 0;
                if (voiceAttr[0] != 0) {
                    voiceOff = 0;
                    do {
                        Sound_ReadVoiceAttr(
                            (s32)*(s16 *)(voicePtr + 0x4c),
                            *(u8 *)(chIdx + voicePtr + 0x2c),
                            voiceOff >> 16,
                            voiceAttr
                        );
                        Sound_WriteVoiceAttr(
                            (s32)*(s16 *)(voicePtr + 0x4c),
                            *(u8 *)(chIdx + voicePtr + 0x2c),
                            voiceOff >> 16,
                            voiceAttr
                        );
                        i++;
                        voiceOff = i * 0x10000;
                    } while (i < (s32)(u32)voiceAttr[0]);
                }
            }
            /* Update data pointer and clear effect change counter */
            *(u32 *)(voicePtr + 0x88) = Sound_AdvanceDataPointer();
            *(u8 *)(voicePtr + 0x29) = 0;
            return;
        }
        if (*(s8 *)(voicePtr + 0x2a) == 2) {
            /* Volume change count == 2: process SPU register writes */
            s8 sfxType = *(s8 *)(voicePtr + 0x16);
            if (sfxType == 0x10) {
                /* Multi-voice SPU register update */
                i = 0;
                if (voiceAttr[0] != 0) {
                    do {
                        Sound_ProcessSpuRegister(
                            (s32)*(s16 *)(voicePtr + 0x4c),
                            *(u8 *)(chIdx + voicePtr + 0x2c),
                            0,
                            0
                        );
                        i++;
                    } while (i < (s32)(u32)voiceAttr[0]);
                }
            } else {
                /* Single-voice SPU register update */
                Sound_ProcessSpuRegister(
                    (s32)*(s16 *)(voicePtr + 0x4c),
                    *(u8 *)(chIdx + voicePtr + 0x2c),
                    sfxType,
                    (u32)voiceAttr
                );
            }
            /* Update data pointer and clear volume change counter */
            *(u32 *)(voicePtr + 0x88) = Sound_AdvanceDataPointer();
            *(u8 *)(voicePtr + 0x2a) = 0;
            return;
        }
    }
    else {
        /* Default: set volume parameter */
        *(u8 *)(voicePtr + 0x15) = sfxParam;
        *(s8 *)(voicePtr + 0x2a) = *(s8 *)(voicePtr + 0x2a) + 1;
    }
    *(u32 *)(voicePtr + 0x88) = Sound_AdvanceDataPointer();
}
/* ========================================================================
 * 15. Sound_ProcessSpuRegister - Process SPU register writes via switch
 *
 * Dispatches SPU register write operations based on a command type.
 * Commands 0-2 handle volume/pitch setup. Commands 4-0xE handle
 * specific SPU voice registers (volume, pitch, ADSR, etc.).
 * Commands 0xF-0x13 handle reverb and damper settings.
 *
 * bank:       Sound bank index
 * voiceIndex: Voice index
 * regType:    Register command type (0-0x13)
 * regValue:   Register value to write
 * ======================================================================== */
void Sound_ProcessSpuRegister(s16 bank, s16 voiceIndex, s16 regType, u32 regValue)
{
    u32 attrWords[2];
    u16 sfxFields[9];
    s32 cmdIdx;
    u8 valByte;
    u32 val;
    valByte = (u8)((regValue >> 24) & 0xff);
    cmdIdx = (s32)regValue;
    val = (u32)valByte;
    /* Read current voice attribute state */
    Sound_ReadVoiceAttr((s32)bank, (s32)voiceIndex, (s32)regType, (u8 *)attrWords);
    switch ((s16)cmdIdx) {
    case 0:
        /* Set high byte of first attribute word */
        ((u8 *)attrWords)[1] = valByte;
        goto write_attr;
    case 1:
        /* Set low byte of first attribute word, write, then handle damper */
        ((u16 *)attrWords)[0] = (u16)((valByte << 8) | ((u8 *)attrWords)[0]);
        Sound_WriteVoiceAttr((s32)bank, (s32)voiceIndex, (s32)regType, (u8 *)attrWords);
        if (val == 0) {
            Sound_SetReverbOff();
        } else if (val == 4) {
            Sound_SetReverbOn();
        }
        break;
    case 2:
        /* Same as case 0 */
        goto write_attr;
    case 3:
write_attr:
        Sound_WriteVoiceAttr((s32)bank, (s32)voiceIndex, (s32)regType, (u8 *)attrWords);
        break;
    case 4: case 5: case 6: case 7: case 8: case 9: case 10:
    case 0xb: case 0xc: case 0xd: case 0xe:
        /* SPU register manipulation commands */
        Sound_UnpackSfxFields(attrWords[0], attrWords[1], sfxFields);
        {
            u32 regCmd = (cmdIdx - 4) * 0x10000 >> 0x10;
            if (regCmd < 0xb) {
                u16 val16 = (u16)valByte;
                switch (regCmd) {
                case 0:
                    sfxFields[5] = 0;
                    sfxFields[0] = val16;
                    break;
                case 1:
                    sfxFields[5] = 1;
                    sfxFields[0] = val16;
                    break;
                case 2:
                    sfxFields[1] = val16;
                    break;
                case 3:
                    sfxFields[2] = val16;
                    break;
                case 4:
                    sfxFields[8] = 0;
                    sfxFields[3] = val16;
                    break;
                case 5:
                    sfxFields[8] = 1;
                    sfxFields[3] = val16;
                    break;
                case 6:
                    sfxFields[6] = 0;
                    sfxFields[4] = val16;
                    break;
                case 7:
                    sfxFields[6] = 1;
                    sfxFields[4] = val16;
                    break;
                case 8:
                    /* Set damper mode based on volume range */
                    if (val == 0 || val > 0x3f) {
                        if (val - 0x40 < 0x40) {
                            sfxFields[7] = 1;
                        }
                    } else {
                        sfxFields[7] = 0;
                    }
                    break;
                case 9:
                case 10:
                    break;
                }
            }
        }
        Sound_PackSfxFields(sfxFields, &attrWords[0], (u16 *)&attrWords[1]);
        Sound_WriteVoiceAttr((s32)bank, (s32)voiceIndex, (s32)regType, (u8 *)attrWords);
        break;
    case 0xf:
        Sound_SetReverbMode((u16)val);
        break;
    case 0x10:
        Sound_SetReverbVolume((s16)val, (s16)val);
        break;
    case 0x11:
        Sound_SetReverbDelay((s16)val);
        break;
    case 0x12:
    case 0x13:
        Sound_SetReverbFeedback((s16)val);
        break;
    }
}
/* ========================================================================
 * 16. Sound_AdvanceDataPointer - Parse command sequence from voice data
 *
 * Reads a variable-length command byte from the sequence data stream.
 * If the high bit is set, reads continuation bytes to form a larger
 * offset. Returns the total number of data units consumed.
 *
 * Returns: Number of data units (in words) consumed, or 0 for end.
 * ======================================================================== */
s32 Sound_AdvanceDataPointer(void)
{
    s32 voicePtr;
    u8 *dataPtr;
    u8 cmdByte;
    u32 offset;
    s32 result;
    voicePtr = GetVoicePtr(SpuCurrentBankIdx, SpuCurrentProgramIdx);
    dataPtr = *(u8 **)(voicePtr + 4);
    *(u8 **)(voicePtr + 4) = dataPtr + 1;
    cmdByte = *dataPtr;
    offset = (u32)cmdByte;
    if (offset == 0) {
        return 0;
    }
    result = offset << 2;
    /* Multi-byte variable-length encoding */
    if ((cmdByte & 0x80) != 0) {
        offset = offset & 0x7f;
        do {
            dataPtr = *(u8 **)(voicePtr + 4);
            *(u8 **)(voicePtr + 4) = dataPtr + 1;
            cmdByte = *dataPtr;
            offset = offset * 0x80 + (cmdByte & 0x7f);
        } while ((cmdByte & 0x80) != 0);
        result = offset * 4;
    }
    result = (result + offset) * 2;
    *(s32 *)(voicePtr + 0x80) = result + *(s32 *)(voicePtr + 0x80);
    return result;
}
/* ========================================================================
 * 17. Sound_UnpackSfxFields - Unpack SPU register words into fields
 *
 * Decomposes two 32-bit SPU register words into individual field
 * values for manipulation. These fields correspond to SPU voice
 * attributes like volume, pitch, ADSR, reverb, etc.
 *
 * word1:      First SPU register word
 * word2:      Second SPU register word
 * outFields:  Output array of 9 u16 fields
 * ======================================================================== */
void Sound_UnpackSfxFields(u32 word1, u32 word2, u16 *outFields)
{
    u16 w1 = (u16)(word1 & 0xffff);
    u16 w2 = (u16)(word2 & 0xffff);
    outFields[5] = w1 & 0x8000;   /* flag: bit 15 of word1 */
    outFields[6] = w2 & 0x8000;   /* flag: bit 15 of word2 */
    outFields[8] = w2 & 0x4000;   /* flag: bit 14 of word2 */
    outFields[7] = w2 & 0x0020;   /* flag: bit 5 of word2 */
    outFields[0] = (w1 >> 8) & 0x7f;  /* volume/sustain: bits 8-14 */
    outFields[1] = (w1 >> 4) & 0x0f;  /* attack: bits 4-7 */
    outFields[2] = w1 & 0x0f;          /* decay: bits 0-3 */
    outFields[3] = (w2 >> 6) & 0x7f;  /* release: bits 6-12 */
    outFields[4] = w2 & 0x1f;          /* sustain level: bits 0-4 */
}
/* ========================================================================
 * 18. Sound_PackSfxFields - Pack field values back into SPU register words
 *
 * Inverse of Sound_UnpackSfxFields. Reconstructs two 16-bit SPU
 * register values from individual fields.
 *
 * inFields:   Input array of 9 u16 fields
 * outWord1:   First output register value
 * outWord2:   Second output register value
 * ======================================================================== */
void Sound_PackSfxFields(u16 *inFields, u32 *outWord1, u16 *outWord2)
{
    u16 w1, w2;
    u16 f3, f4;
    /* Reconstruct flags from field array */
    u16 flagWord2 = -(u16)(inFields[6] != 0) & 0x8000;
    if (inFields[8] != 0) {
        flagWord2 = flagWord2 | 0x4000;
    }
    f3 = inFields[3];
    f4 = inFields[4];
    /* Pack first word: flag | volume << 8 | attack << 4 | decay */
    w1 = (u16)(-(u16)(inFields[5] != 0) & 0x8000
       | (*inFields & 0x7f) << 8
       | (inFields[1] & 0x0f) << 4
       | inFields[2] & 0x0f);
    /* Pack second word: flag | release << 6 | sustain */
    w2 = (u16)(flagWord2 | (f3 & 0x7f) << 6 | f4 & 0x1f);
    *outWord1 = (u32)w1;
    *outWord2 = w2;
}
/* ========================================================================
 * 19. Sound_SetReverbOn - Enable SPU reverb
 * ======================================================================== */
void Sound_SetReverbOn(void)
{
    SpuSetReverb(1);
}
/* ========================================================================
 * 20. Sound_SetReverbOff - Disable SPU reverb
 * ======================================================================== */
void Sound_SetReverbOff(void)
{
    SpuSetReverb(0);
}
/* ========================================================================
 * 21. Sound_SetReverbMode - Set reverb mode with depth
 *
 * Selects one of the built-in reverb modes (0-9).
 * If the high bit is set in the mode value, the reverb is inverted.
 * Mode 0 also disables reverb before applying.
 *
 * mode:  Reverb mode (0-9), negative = inverted
 * Returns 0 on success, -1 on invalid mode.
 * ======================================================================== */
s32 Sound_SetReverbMode(u16 mode)
{
    s32 result;
    u16 adjustedMode;
    s32 isNegative = (s32)((u32)mode << 0x10) < 0;
    if (isNegative) {
        mode = -mode;
    }
    if (mode < 10) {
        SoundReverbAttr = 1;
        adjustedMode = mode;
        if (isNegative) {
            adjustedMode = mode | 0x100;
        }
        SoundReverbMode = (s32)(s16)adjustedMode;
        result = (s32)(s16)mode;
        if (result == 0) {
            SpuSetReverb(0);
        }
        SpuSetReverbModeParam((SpuReverbAttr *)&SoundReverbAttr);
    } else {
        result = -1;
    }
    return result;
}
/* ========================================================================
 * 22. Sound_SetReverbVolume - Set reverb depth (left and right)
 *
 * Scales input volumes from 0-127 to 0-0x7FFF range.
 * Sets reverb depth mask (6) and applies via SpuSetReverbModeParam.
 *
 * leftVol:   Left reverb depth
 * rightVol:  Right reverb depth
 * ======================================================================== */
void Sound_SetReverbVolume(s16 leftVol, s16 rightVol)
{
    SoundReverbDepthLeft = (leftVol * 0x7fff) / SOUND_VOLUME_DEFAULT;
    SoundReverbAttr = 6;  /* mask: depth left + depth right */
    SoundReverbDepthRight = (rightVol * 0x7fff) / SOUND_VOLUME_DEFAULT;
    SpuSetReverbModeParam((SpuReverbAttr *)&SoundReverbAttr);
}
/* ========================================================================
 * 23. Sound_SetReverbDelay - Set reverb delay time
 *
 * Sets the delay time for the reverb effect.
 * Uses mask value 0x10 to select the delay parameter.
 *
 * delay: Delay time value
 * ======================================================================== */
void Sound_SetReverbDelay(s16 delay)
{
    SoundReverbDelay = (s32)delay;
    SoundReverbAttr = 0x10;  /* mask: delay */
    SpuSetReverbModeParam((SpuReverbAttr *)&SoundReverbAttr);
}
/* ========================================================================
 * 24. Sound_SetReverbFeedback - Set reverb feedback level
 *
 * Sets the feedback level for the reverb effect.
 * Uses mask value 8 to select the feedback parameter.
 *
 * feedback: Feedback level value
 * ======================================================================== */
void Sound_SetReverbFeedback(s16 feedback)
{
    SoundReverbFeedback = (s32)feedback;
    SoundReverbAttr = 8;  /* mask: feedback */
    SpuSetReverbModeParam((SpuReverbAttr *)&SoundReverbAttr);
}
/* ========================================================================
 * 25. Sound_WriteVoiceAttr - Write voice attribute data to SPU buffer
 *
 * Writes 14 bytes + 4 halfwords of voice attribute data into the
 * voice attribute buffer. Inverse of Sound_ReadVoiceAttr.
 *
 * bank:      Sound bank index
 * program:   Program number within the bank
 * voiceOff:  Byte offset into the program's voice table
 * data:      Input buffer (18 bytes: 14 u8 + 4 u16)
 * Returns 0 on success, -1 on failure.
 * ======================================================================== */
u32 Sound_WriteVoiceAttr(s16 bank, s16 program, int voiceOff, u8 *data)
{
    int voiceAttrOffset;
    int i;
    if (SpuBankLoadedFlags[bank] != 1) {
        return (u32)-1;
    }
    SpuSelectProgram(bank, program);
    voiceAttrOffset = (int)(((voiceOff + (u32)SpuCurrentVoiceAttrIdx * 0x10) * 0x10000) >> 0xb);
    /* Write 14 bytes of voice attribute data */
    for (i = 0; i < 14; i++) {
        ((u8 *)(voiceAttrOffset + SpuCurrentVoiceAttr))[i] = data[i];
    }
    /* Write 4 halfwords of extended voice data */
    {
        u8 *dst = (u8 *)(voiceAttrOffset + SpuCurrentVoiceAttr);
        *(u16 *)(dst + 0x10) = *(u16 *)(data + 0x10);
        *(u16 *)(dst + 0x12) = *(u16 *)(data + 0x12);
        *(u16 *)(dst + 0x14) = *(u16 *)(data + 0x14);
        *(u16 *)(dst + 0x16) = *(u16 *)(data + 0x16);
    }
    return 0;
}
/* ========================================================================
 * 26. Sound_DamperOn - Enable SPU volume damper
 *
 * The damper gradually reduces volume to zero over time.
 * Setting SoundDamperMode to 2 enables this effect globally.
 * ======================================================================== */
void Sound_DamperOn(void)
{
    SoundDamperMode = 2;
}
/* ========================================================================
 * 27. Sound_DamperOff - Disable SPU volume damper
 *
 * Setting SoundDamperMode to 0 disables the damper effect globally.
 * ======================================================================== */
void Sound_DamperOff(void)
{
    SoundDamperMode = 0;
}
/* ========================================================================
 * 28. Sound_ResetVoice - Reset a voice to its default state
 *
 * Clears all voice control flags, resets effect types, volume,
 * pitch, and sequence state. Restores default values from
 * backup fields. Initializes per-channel volume array.
 *
 * bank:       Sound bank index
 * voiceIndex: Voice index to reset
 * ======================================================================== */
void Sound_ResetVoice(s16 bank, s16 voiceIndex)
{
    s32 voicePtr;
    s32 i;
    voicePtr = voiceIndex * 0xac + SpuVoicePtrTable[bank];
    /* Clear flags (bits 0, 1, 2) and set bit 3 (reset pending) */
    *(u32 *)(voicePtr + 0x90) = *(u32 *)(voicePtr + 0x90) & ~1;
    *(u32 *)(voicePtr + 0x90) = *(u32 *)(voicePtr + 0x90) & ~2;
    *(u32 *)(voicePtr + 0x90) = *(u32 *)(voicePtr + 0x90) & ~8;
    *(u32 *)(voicePtr + 0x90) = *(u32 *)(voicePtr + 0x90) | 4;
    /* Stop voice playback */
    SpuStopVoice((s32)voiceIndex << 8 | (s32)bank);
    /* Reset all state fields to zero */
    *(u8 *)(voicePtr + 0x2b) = 0;   /* keyOn */
    *(u32 *)(voicePtr + 0x80) = 0;  /* seqOffset */
    *(u8 *)(voicePtr + 0x27) = 0;   /* damperMode */
    *(u8 *)(voicePtr + 0x13) = 0;   /* effect1Type */
    *(u8 *)(voicePtr + 0x14) = 0;   /* effect2Type */
    *(u8 *)(voicePtr + 0x29) = 0;   /* effectChangeCount */
    *(u8 *)(voicePtr + 0x15) = 0;   /* volumeParam */
    *(u8 *)(voicePtr + 0x16) = 0;   /* sfxType */
    *(u8 *)(voicePtr + 0x2a) = 0;   /* volumeChangeCount */
    *(u8 *)(voicePtr + 0x12) = 0;   /* channelIndex */
    *(u16 *)(voicePtr + 0x48) = 0;  /* reserved */
    *(u8 *)(voicePtr + 0x27) = 0;   /* damperMode (double clear) */
    *(u8 *)(voicePtr + 0x28) = 0;   /* damperCounter */
    *(u8 *)(voicePtr + 0x10) = 0;   /* mode */
    *(u8 *)(voicePtr + 0x11) = 0;   /* keyOff */
    /* Restore default values from backup fields */
    *(u32 *)(voicePtr + 0x88) = *(u32 *)(voicePtr + 0x7c);  /* currentDataOffset = initial */
    *(u32 *)(voicePtr + 0x8c) = *(u32 *)(voicePtr + 0x84);  /* sampleRateBackup = sampleRate */
    *(u16 *)(voicePtr + 0x70) = *(u16 *)(voicePtr + 0x72);  /* playSpeed = default */
    *(u32 *)(voicePtr + 4)    = *(u32 *)(voicePtr + 8);     /* dataPtr = dataBasePtr */
    *(u32 *)(voicePtr + 0x0c) = *(u32 *)(voicePtr + 8);     /* loopStart = dataBasePtr */
    /* Initialize per-channel volume and pitch arrays */
    i = 0;
    do {
        *(s8 *)(voicePtr + i + 0x2c) = (s8)i;      /* channelOrder[i] = i */
        *(u8 *)(voicePtr + i + 0x17) = SOUND_VOLUME_HALF;        /* volume[i] = 0x40 (default) */
        *(u16 *)(voicePtr + i * 2 + 0x4e) = SOUND_VOLUME_DEFAULT;   /* effectVol[i] = 0x7f */
        i++;
    } while (i < SOUND_CHANNEL_MAX);
    /* Set default pitch and volume */
    *(u16 *)(voicePtr + 0x78) = SOUND_VOLUME_DEFAULT;  /* pitch */
    *(u16 *)(voicePtr + 0x7a) = SOUND_VOLUME_DEFAULT;  /* reserved */
}
/* ========================================================================
 * 29. Sound_ResetVoiceBank - Reset all voices in bank 0
 *
 * Convenience wrapper that resets voice 0 of the given bank.
 * Used to initialize a sound bank's default voice state.
 *
 * bank: Sound bank index
 * ======================================================================== */
void Sound_ResetVoiceBank(s16 bank)
{
    Sound_ResetVoice(bank, 0);
}
/* ========================================================================
 * 30. Sound_InitHot - Initialize SPU in hot-reload mode
 *
 * Initializes the SPU hardware with hot-reload support.
 * The parameter 1 enables hot-reload mode (vs cold init).
 * ======================================================================== */
void Sound_InitHot(void)
{
    _SpuInit(1);
}
/* === migrated from stubs.c: GetCdObj === */
void *GetCdObj(void) { return malloc(SPU_FRAME_SIZE); }
/* === migrated from stubs.c: SpuParamSetter === */
void SpuParamSetter(short param) { SetSpuParam((int)param, 0); }
/* === migrated from stubs.c: AnimGetCurrentId === */
int AnimGetCurrentId(void) { return Scene_CurrentId; }
/* === migrated from stubs.c: SpuinitWrapper === */
void SpuinitWrapper(void) { InitSpu(0); }
/* === migrated from stubs.c: SoundSetTeleportFlag === */
void SoundSetTeleportFlag(int a) { DreamInstantTeleportFlag = a; }
/* === migrated from stubs.c: InitializeSpuHot === */
void InitializeSpuHot(void) { _SpuInit(1); }
/* === migrated from stubs.c: SpuGetVtable_SeqPlayer === */
void *SpuGetVtable_SeqPlayer(void) { return (void *)SPU_VTABLE_SEQPLAYER; }
/* === migrated from stubs.c: SpuGetVtable_SeqLineDraw === */
void *SpuGetVtable_SeqLineDraw(void) { return (void *)SPU_VTABLE_OBJECTB; }
/* === migrated from stubs.c: SpuGetVtable_ObjectC === */
void *SpuGetVtable_ObjectC(void) { return (void *)SPU_VTABLE_OBJECTC; }
/* === migrated from stubs.c: LookupSoundResult === */
void *LookupSoundResult(void) { return 0; }

/* === migrated from stubs.c === */
/* GetSoundSize */
unsigned int GetSoundSize(void *snd)
{
    (void)snd;
    return SPU_FRAME_SIZE;
}
/* GetActiveSoundObject */
void *GetActiveSoundObject(int a, int b, int c, int d)
{
    (void)a; (void)b; (void)c; (void)d;
    return malloc(0x80);
}
/* NewSoundEntity */
int NewSoundEntity(char actionType, void *entity, void *params, int param)
{
    (void)actionType; (void)entity; (void)params; (void)param;
    return 0;
}
/* EntitySetParam */
int EntitySetParam(unsigned int param, short val1, short val2, int flags)
{
    int idx = ((param & 0xff00) >> 8) * 0xac + SpuVoicePtrTable[param & 0xff];
    SpuCurrentChannelIdx = (short)param;
    *(short *)(idx + 0x74) = val1;
    *(short *)(idx + 0x76) = val2;
    if (*(unsigned short *)(idx + 0x74) > SOUND_VOLUME_DEFAULT) *(short *)(idx + 0x74) = SOUND_VOLUME_DEFAULT;
    if (*(unsigned short *)(idx + 0x76) > SOUND_VOLUME_DEFAULT) *(short *)(idx + 0x76) = SOUND_VOLUME_DEFAULT;
    return (int)SpuCurrentChannelIdx;
}
/* EntityGetParam */
int EntityGetParam(unsigned int param, short *out1, short *out2)
{
    SpuCurrentChannelIdx = (short)param;
    int idx = ((param & 0xff00) >> 8) * 0xac + SpuVoicePtrTable[param & 0xff];
    *out1 = *(short *)(idx + 0x74);
    *out2 = *(short *)(idx + 0x76);
    return (int)SpuCurrentChannelIdx;
}
/* SoundMathTransformByMatrix */
int SoundMathTransformByMatrix(unsigned int param, unsigned char *cmd)
{
    int op = cmd[1];
    if (op == 1) goto done;
    if ((int)op < 0) {
        if (cmd[0] != 0) return 0;
        op = -op + 1;
    }
    switch (op) {
    case 2: case 3: case 4:
        param = SoundDspProcess(param, op - 1); break;
    case 5: if ((int)param % 3 != 0) return 0; goto done;
    case 6: if ((int)param % 3 == 0) return 0; goto done;
    case 7: param = SoundGetTick(); break;
    case 8: case 9: if ((int)param % 3 != op - 7) return 0; goto done;
    default: if ((int)op < 10) goto done; param = SoundGetMode(op); break;
    case 0x14: if ((param & 1) != 0) return 0; goto done;
    case 0x15: param = param & 1;
    }
    if (param == 0) return 0;
done:
    *cmd = 1;
    return 1;
}
/* SoundTriggerConditional */
void SoundTriggerConditional(int param)
{
    if (param == 0x4e || param == 0xb || param == 0x38 || param == 0x5d)
        SoundSetTeleportFlag(1);
}
/* SoundEntitySpawn */
int SoundEntitySpawn(int a, int b, short *params, int param4)
{
    (void)params; (void)param4;
    int *ent = (int *)New_Entity(a, b, Other_SoundEntityParam);
    return ent == 0;
}
/* EntityClearSoundChannel */
void EntityClearSoundChannel(short param)
{
    if (SpuVoiceCount == 0) return;
    for (unsigned char i = 0; i < SpuVoiceCount; i++) {
        if (VoiceSoundId[i * 0x1a] == param) {
            unsigned short mask = 1 << i;
            unsigned int maskLo, maskHi;
            if (i < 16) { maskLo = mask; maskHi = 0; }
            else { maskLo = 0; maskHi = (unsigned short)(1 << (i - 16)); }
            SpuCurrentVoiceId = i;
            VoiceStatus[i * 0x34] = 0;
            VoiceBankIdx[i * 0x1a] = 0;
            VoicePitch[i * 0x1a] = 0;
            SpuKeyOnMaskL |= maskLo;
            Spu_ReverbMaskL &= ~SpuKeyOnMaskL;
            SpuKeyOnMaskH |= maskHi;
            Spu_ReverbMaskH &= ~SpuKeyOnMaskH;
        }
    }
}
/* _SpuinitWrapper */
int _SpuinitWrapper(int mode)
{
    (void)mode;
    return 0;
}
/* _SpuDataCallback */
void _SpuDataCallback(void)
{
}
/* Sound_AdvanceDataPointer */
int Sound_AdvanceDataPtrEx(int bank, int voiceIndex)
{
    unsigned int val;
    int offset;
    unsigned char *pb;
    int voicePtr;
    voicePtr = voiceIndex * 0xac + SpuVoicePtrTable[bank];
    pb = (unsigned char *)*(int *)(voicePtr + 4);
    *(int *)(voicePtr + 4) = (int)(pb + 1);
    val = *pb;
    if (val == 0) {
        offset = 0;
    } else {
        offset = val << 2;
        if ((val & 0x80) != 0) {
            val &= 0x7f;
            do {
                pb = (unsigned char *)*(int *)(voicePtr + 4);
                *(int *)(voicePtr + 4) = (int)(pb + 1);
                val = val * 0x80 + (*pb & 0x7f);
            } while ((*pb & 0x80) != 0);
            offset = val * 4;
        }
        offset = (offset + val) * 2;
        *(int *)(voicePtr + 0x80) = offset + *(int *)(voicePtr + 0x80);
    }
    return offset;
}
/* SpuParseSeqData */
int SpuParseSeqData(int channel, int soundBankId, void *seqData)
{
    unsigned int sampleRate;
    int result;
    int i;
    int framesPerSec;
    short playSpeedCalc;
    int voicePtr;
    unsigned int speedRatio, ticksPerSec;
    char *seq = (char *)seqData;
    result = 0;
    voicePtr = channel * 0xac + SpuVoicePtrTable[channel];
    *(short *)(voicePtr + 0x4c) = (short)soundBankId;
    *(short *)(voicePtr + 0x4a) = 0;
    *(char *)(voicePtr + 0x13) = 0;
    *(char *)(voicePtr + 0x14) = 0;
    *(char *)(voicePtr + 0x29) = 0;
    *(char *)(voicePtr + 0x15) = 0;
    *(char *)(voicePtr + 0x16) = 0;
    *(char *)(voicePtr + 0x2a) = 0;
    *(char *)(voicePtr + 0x12) = 0;
    *(int *)(voicePtr + 0x7c) = 0;
    *(int *)(voicePtr + 0x80) = 0;
    *(int *)(voicePtr + 0x84) = 0;
    *(short *)(voicePtr + 0x72) = 0;
    *(short *)(voicePtr + 0x48) = 0;
    *(char *)(voicePtr + 0x2b) = 0;
    *(int *)(voicePtr + 0x88) = 0;
    *(char *)(voicePtr + 0x27) = 0;
    *(char *)(voicePtr + 0x28) = 0;
    *(char *)(voicePtr + 0x10) = 0;
    *(char *)(voicePtr + 0x11) = 0;
    *(short *)(voicePtr + 0xa8) = SOUND_VOLUME_DEFAULT;
    *(short *)(voicePtr + 0xaa) = 0;
    for (i = 0; i < 0x10; i++) {
        *(char *)(voicePtr + i + 0x2c) = (char)i;
        *(unsigned char *)(voicePtr + i + 0x17) = SOUND_VOLUME_HALF;
        *(short *)(voicePtr + (i * 2) + 0x4e) = SOUND_VOLUME_DEFAULT;
    }
    *(short *)(voicePtr + 0x6e) = 1;
    *(int *)(voicePtr + 4) = (int)seq;
    if ((*seq == 'S') || (*seq == 'p')) {
        *(int *)(voicePtr + 4) = (int)(seq + 8);
        if (seq[7] == 0x01) {
            char highByte, lowByte;
            unsigned char b1, b2, b3;
            unsigned char *dataPtr;
            *(int *)(voicePtr + 4) = (int)(seq + 9);
            highByte = seq[8];
            *(int *)(voicePtr + 4) = (int)(seq + 10);
            lowByte = seq[9];
            *(short *)(voicePtr + 0x4a) = (short)((unsigned char)highByte << 8 | (unsigned char)lowByte);
            dataPtr = (unsigned char *)*(int *)(voicePtr + 4);
            *(unsigned char **)(voicePtr + 4) = dataPtr + 1;
            b1 = *dataPtr;
            *(unsigned char **)(voicePtr + 4) = dataPtr + 2;
            b2 = dataPtr[1];
            *(unsigned char **)(voicePtr + 4) = dataPtr + 3;
            b3 = dataPtr[2];
            sampleRate = (unsigned int)b1 << 16 | (unsigned int)b2 << 8 | (unsigned int)b3;
            if (sampleRate == 0) trap(0x1c00);
            *(int *)(voicePtr + 0x84) = (int)sampleRate;
            if (sampleRate >> 1 < 60000000U % sampleRate)
                *(int *)(voicePtr + 0x84) = (int)(60000000 / sampleRate + 1);
            else
                *(int *)(voicePtr + 0x84) = (int)(60000000 / sampleRate);
            *(int *)(voicePtr + 0x8c) = *(int *)(voicePtr + 0x84);
            *(int *)(voicePtr + 4) = *(int *)(voicePtr + 4) + 2;
            result = Sound_AdvanceDataPtrEx(channel, 0);
            speedRatio = (unsigned int)(*(short *)(voicePtr + 0x4a) * *(int *)(voicePtr + 0x84));
            *(int *)(voicePtr + 0x0c) = *(int *)(voicePtr + 4);
            framesPerSec = SpuTickRate;
            *(int *)(voicePtr + 0x7c) = result;
            *(int *)(voicePtr + 0x88) = result;
            *(int *)(voicePtr + 8) = *(int *)(voicePtr + 4);
            ticksPerSec = (unsigned int)framesPerSec * 0x3c;
            if (speedRatio * 10 < ticksPerSec) {
                if (speedRatio == 0) trap(0x1c00);
                playSpeedCalc = (short)((unsigned int)(framesPerSec * 600) / speedRatio);
                *(short *)(voicePtr + 0x6e) = playSpeedCalc;
                *(short *)(voicePtr + 0x70) = playSpeedCalc;
            } else {
                if (ticksPerSec == 0) trap(0x1c00);
                if (ticksPerSec == 0) trap(0x1c00);
                *(short *)(voicePtr + 0x6e) = (short)0xffff;
                playSpeedCalc = (short)(((unsigned int)(*(short *)(voicePtr + 0x4a) * *(int *)(voicePtr + 0x84) * 10)) / ticksPerSec);
                *(short *)(voicePtr + 0x70) = playSpeedCalc;
                if ((unsigned int)(framesPerSec * 0x1e) <
                    ((unsigned int)(*(short *)(voicePtr + 0x4a) * *(int *)(voicePtr + 0x84) * 10)) % ticksPerSec)
                    *(short *)(voicePtr + 0x70) = playSpeedCalc + 1;
            }
            result = 0;
            *(short *)(voicePtr + 0x72) = *(short *)(voicePtr + 0x70);
        } else {
            printf("This is not SEQ Data.\n");
            result = -1;
        }
    } else {
        printf("This is an old SEQ Data Format.\n");
        result = 0;
    }
    return result;
}
/* ParseSoundFormat */
int ParseSoundFormat(int *idx_buf, int fmt_val)
{
    (void)fmt_val;
    if (idx_buf) *idx_buf = 0;
    return 0;
}
/* === Snd_* sound manager stubs (replaces missing_stubs.s entries) === */
void *Snd_New(void)
{
    return malloc(128);
}
int Snd_SelectAmbient(int id)
{
    (void)id;
    return 0;
}
int Snd_GetCurrentAmbient(int id)
{
    (void)id;
    return 0;
}
int Snd_GetAmbientPath(void *path)
{
    (void)path;
    return 0;
}
int Snd_SelectSfx(unsigned int id, int mode)
{
    (void)id; (void)mode;
    return 0;
}
int Snd_RandomSelect(unsigned int id, int mode, int prev)
{
    (void)id; (void)mode; (void)prev;
    return 0;
}
void *Cd_New(int mode)
{
    (void)mode;
    return malloc(1024);
}
/* === Additional function stubs from missing_stubs.s === */
/* These provide safe return values so callers don't loop forever or crash */
/* EntityObject rotation/transform helpers */
int func_8001cba4(int a, int b) { (void)a;(void)b; return 0; }
int func_8001cc48(int a) { (void)a; return 0; }
int func_8001ccb4(int data) { (void)data; return 0; }
int func_8001cd20(int data) { (void)data; return 0; }
int func_8001cd60(int data) { (void)data; return 0; }
int func_8001ce30(int a, int b, int c, int d) { (void)a;(void)b;(void)c;(void)d; return 0; }
int func_8001ceb4(int a, int b, int c, int d) { (void)a;(void)b;(void)c;(void)d; return 0; }
int func_8001d008(int a, int b, int c, int d) { (void)a;(void)b;(void)c;(void)d; return 0; }
int func_8001d1a4(int a) { (void)a; return 0; }
int func_8001d3f8(short *a, int b, int c, int d) { (void)a;(void)b;(void)c;(void)d; return 0; }
int func_8001d424(int a, int b, int c, int d) { (void)a;(void)b;(void)c;(void)d; return 0; }
int func_8001d450(int a, int b, int c, int d) { (void)a;(void)b;(void)c;(void)d; return 0; }
int func_8001d480(int a, int b, int c, int d, int e, int f) { (void)a;(void)b;(void)c;(void)d;(void)e;(void)f; return 0; }
int func_8001e57c(int a) { (void)a; return 0; }
int func_8001e770(int a) { (void)a; return 0; }
int func_8001d0ec(void) { return 0; }
int func_8001d33c(int a, int b, int c, int d) { (void)a;(void)b;(void)c;(void)d; return 0; }
int func_8001d344(void) { return 0; }
int func_8001d374(int a, int b, int c, int d) { (void)a;(void)b;(void)c;(void)d; return 0; }
int func_8001d3a0(int a, int b) { (void)a;(void)b; return 0; }
int func_8001d3cc(int a, int b) { (void)a;(void)b; return 0; }
int func_8001d4ac(int a, int b) { (void)a;(void)b; return 0; }
int func_8001d4dc(int a, int b, int c, int d) { (void)a;(void)b;(void)c;(void)d; return 0; }
int func_8001d568(int a, int b) { (void)a;(void)b; return 0; }
int func_8001e600(int a, int b, int c, int d) { (void)a;(void)b;(void)c;(void)d; return 0; }
int func_8001e7b0(int data) { (void)data; return 0; }
int func_8001eacc(int a, int b, int c, int d, int e) { (void)a;(void)b;(void)c;(void)d;(void)e; return 0; }
int func_80025e9c(void) { return 0; }
int func_80026f34(int a, int b, int c) { (void)a;(void)b;(void)c; return 1; }
/* CD-ROM mode function stubs - break busy-wait loops in CdModeSubE */
int func_8003b20c_obj_construct(int obj, int src) { (void)obj; (void)src; return 0; }
int func_8003b20c_sub44(int obj, int a, int b, int c) { (void)obj;(void)a;(void)b;(void)c; return 0; }
extern int D_8006E4F0;
int func_8003b20c(void)
{
    return (int)&D_8006E4F0;
}
int func_8002C438(void) { return 0; }
void func_8002C448(void) { }
int func_8002C468(int a0, int a1, int a2) { (void)a0;(void)a1;(void)a2; return 1; }
void func_8002C478(void) { }
int func_80027EC8(void) { return 0; }
int func_80027ED4(void) { return 0; }
int func_80027EE0(void) { return 0; }
int func_80027EEC(void) { return 0; }
void func_80027EF8(void) { }
void func_80028B6C(void) { }
int NopSub_27e68(void) { return 0; }
int NopSub_27f18(int a0, int a1, int a2) { (void)a0;(void)a1;(void)a2; return 1; }
/* Label vtable getter - return a minimal valid vtable stub */
static int LabelVtableFiller(int obj, int *arg2) { (void)obj; (void)arg2; return 0; }
int Label_GetVtable(void)
{
    static int vt[64];
    static int inited = 0;
    if (!inited) {
        vt[0xbc/4] = (int)LabelVtableFiller;
        inited = 1;
    }
    return (int)vt;
}

