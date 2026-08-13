#include "common.h"
#include "dat_globals.h"
/*
 * SpuInit.c - SPU sequence playback and voice management
 *
 * Handles SPU initialization/shutdown, sequence (SEQ) data parsing,
 * voice playback control, volume fading, and channel allocation
 * for the PsyQ SPU library used in LSD: Dream Emulator.
 *
 * Voice struct layout (0xAC bytes per voice, same as Sound.c):
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
 * Voice flags (offset 0x90):
 *   bit 0: keyOn
 *   bit 1: needsUpdate
 *   bit 2: resetPending
 *   bit 3: pendingKeyOn
 *   bit 4: fadeActive
 *   bit 5: fadeReverse
 *   bit 9: stopped
 *
 * Global data:
 *   SpuInitialized: SPU initialized flag (1=init, 0=shutdown)
 *   SpuTransferInProgress: SPU transfer in-progress flag
 *   SpuVoicePtrTable: Array of voice array pointers, one per sound bank
 *   SpuVoicesPerBank: Voices per bank
 *   SpuOpenChannelMask: Open channel bitmask
 *   SpuDmaEventId: SPU DMA event ID
 *   SpuTickRate: Frames per second / tick rate
 */
/* Forward declarations for PsyQ / external functions */
extern void _SpuInit(s32 mode);
extern void _SpuDataCallback(s32 flag);
extern void CloseEvent(s32 event);
extern void DisableEvent(s32 event);
extern void EnterCriticalSection(void);
extern void ExitCriticalSection(void);
extern void SpuSetVoiceVolume(s32 id, s32 p1, s32 p2, s32 p3);
extern void SpuStopVoice(s32 id);
extern void SpuGetVoiceVolume(s32 id, s32 p1, s32 p2);
extern s32 Sound_AdvanceDataPointer(void);
extern void *GetGpuDisplayList(void);
extern void printf(const char *fmt, ...);
/* Forward declarations for vtable getters */
extern void *SpuGetVtable_SeqPlayer(void);
extern void *SpuGetVtable_SeqChannel(void);
extern void *SpuGetVtable_SeqTrack(void);
extern void *SpuGetVtable_SeqLineDraw(void);
/* Forward declarations for constructor/vtable helpers */
extern void *SpuGetVtable_ObjectA(void);
extern void *SpuGetVtable_ObjectB(void);
extern void *SpuGetVtable_ObjectC(void);
/* Forward declarations for internal sequence functions */
extern u32 SpuParseSeqData(u16 channel, s16 soundBankId, char *seqData);
extern u32 SpuOpenSequence(void *seqDataPtr, short channel);
extern void SpuSetSeqChannelFlags(u16 bank, s16 voiceIndex, s32 flags);
extern void SpuGetSeqChannelVolume(u16 bank, s16 voiceIndex, s32 *volL, s32 *volR);
extern void SpuSetSeqChannelVolume(u16 bank, s16 voiceIndex, s32 volL, s32 volR, u32 fadeFrames);
extern void SpuResetAllVoices(s16 bank);
/* ---- Global data references ---- */
extern u32 SpuInitialized;
extern u32 SpuTransferInProgress;
extern u32 SpuDmaEventId;
extern u32 SpuDmaCallbackData1;
extern u32 SpuDmaCallbackData2;
extern u32 SpuVoicePtrTable[];
extern u32 SpuVoicesPerBank;
extern u32 SpuOpenChannelMask;
extern u32 SpuTickRate;
/* Helper: compute voice struct pointer from bank + voice index */
static int GetVoicePtr(s16 bank, s16 voiceIndex)
{
    return voiceIndex * 0xac + SpuVoicePtrTable[bank];
}
/* ========================================================================
 * 1. SpuInit - Initialize SPU with cold boot
 *
 * Calls _SpuInit with mode 0 (cold initialization).
 * This sets up SPU hardware, allocates SPU memory, and prepares
 * the sound system for use.
 * ======================================================================== */
void SpuInit(void)
{
    _SpuInit(0);
}
/* ========================================================================
 * 2. SpuQuit - Shut down SPU and release resources
 *
 * If the SPU is currently initialized (flag == 1), disables all
 * callbacks, closes the DMA event, and unlocks the CD-ROM drive.
 * ======================================================================== */
void SpuQuit(void)
{
    if (SpuInitialized == 1) {
        SpuInitialized = 0;
        EnterCriticalSection();
        SpuDmaCallbackData1 = 0;
        SpuDmaCallbackData2 = 0;
        _SpuDataCallback(0);
        CloseEvent(SpuDmaEventId);
        DisableEvent(SpuDmaEventId);
        ExitCriticalSection();
    }
}
/* ========================================================================
 * 3. SpuSetInTransfer - Set SPU DMA transfer state
 *
 * Sets the transfer-in-progress flag. When flag is 1, marks
 * transfer as complete (flag becomes 0). When flag is not 1,
 * marks transfer as in-progress (flag becomes 1).
 *
 * flag: 1 = transfer complete, other = transfer in progress
 * ======================================================================== */
void SpuSetInTransfer(s32 flag)
{
    SpuTransferInProgress = (u32)(flag != 1);
}
/* ========================================================================
 * 4. SpuGetInTransfer - Query SPU DMA transfer state
 *
 * Returns true if no DMA transfer is currently in progress.
 * ======================================================================== */
bool SpuGetInTransfer(void)
{
    return SpuTransferInProgress == 0;
}
/* ========================================================================
 * 5. CreateSeqPlayer - Allocate and construct a sequence player object
 *
 * Allocates a 0x24-byte object, retrieves the SEQ player vtable,
 * and calls its constructor. Returns the new object, or 0 on failure.
 *
 * seqData: Sequence data pointer
 * soundBankId: Sound bank ID
 * channel: Channel/program number
 * ======================================================================== */
int CreateSeqPlayer(void *seqData, s32 soundBankId, s32 channel)
{
    int obj;
    int vtable;
    obj = MemAlloc(SIZEOF_PLAYER_SEQ);
    vtable = 0;
    if (obj != 0) {
        vtable = (int)SpuGetVtable_SeqPlayer();
        (*(code *)(vtable + 8))(obj, seqData, soundBankId, channel);
        vtable = obj;
    }
    return vtable;
}
/* ========================================================================
 * 6. SpuStartSequence - Start playback of a sequence on a voice
 *
 * Validates that both the sound bank and program pointers are valid,
 * then opens the sequence and configures playback. Stores the
 * returned sequence handle in the player struct.
 *
 * player: Pointer to sequence player object (0x24 bytes)
 *   +0x0c: soundBankPtr (must be non-NULL with non-zero seqType at +0x58)
 *   +0x10: programPtr (must be non-NULL with non-zero data at +0x2c)
 *   +0x14: sequence handle (output)
 *   +0x1a: playback state (set to 2 = playing)
 *
 * Returns 1 on success, 0 on failure.
 * ======================================================================== */
int SpuStartSequence(int player)
{
    int soundBankPtr;
    int programPtr;
    short seqHandle;
    soundBankPtr = *(int *)(player + 0xc);
    programPtr = *(int *)(player + 0x10);
    /* Validate sound bank and program are loaded */
    if (soundBankPtr == 0 || programPtr == 0) {
        return 0;
    }
    if (*(short *)(soundBankPtr + 0x58) == 0) {
        return 0;
    }
    if (*(int *)(programPtr + 0x2c) == 0) {
        return 0;
    }
    /* Open the sequence: pass program data pointer and sound bank ID */
    seqHandle = SpuOpenSequence(
        *(int *)(programPtr + 0x10),
        (int)*(short *)(soundBankPtr + 0x54)
    );
    *(short *)(player + 0x14) = seqHandle;
    if (seqHandle == -1) {
        printf("Seq Open error in WBgmHandleMonitorEvent", 0x34);
    }
    /* Configure the opened sequence */
    SpuConfigureSequence((int)*(short *)(player + 0x14), 0x34, 0x34);
    /* Mark as playing */
    *(short *)(player + 0x1a) = 2;
    return 1;
}
/* ========================================================================
 * 7. SpuGetVtable_SeqPlayerAccessor - Get SEQ player vtable
 *
 * Returns a pointer to the SEQ player vtable (SpuSeqPlayerVtable).
 * Used by CreateSeqPlayer to look up the constructor.
 * ======================================================================== */
void *SpuGetVtable_SeqPlayerAccessor(void)
{
    return &SpuSeqPlayerVtable;
}
/* ========================================================================
 * 8. SpuGetFrameRate - Get current frame rate / tick count
 *
 * Returns the value of SpuFrameCounter, which represents the
 * current frame rate or tick counter for timing calculations.
 * ======================================================================== */
u32 SpuGetFrameRate(void)
{
    return SpuFrameCounter;
}
/* ========================================================================
 * 9. SpuGetVtable_SeqChannelAccessor - Get SEQ channel vtable
 *
 * Returns a pointer to the SEQ channel vtable (SpuSeqChannelVtable).
 * ======================================================================== */
void *SpuGetVtable_SeqChannelAccessor(void)
{
    return &SpuSeqChannelVtable;
}
/* ========================================================================
 * 10. SpuConfigureSequence - Configure a sequence for playback
 *
 * Wrapper around SpuSetVoiceVolume that sets up sequence parameters.
 * Called after SpuOpenSequence to apply initial configuration.
 *
 * seqHandle: Sequence handle returned by SpuOpenSequence
 * volLeft:   Left channel volume
 * volRight:  Right channel volume
 * ======================================================================== */
void SpuConfigureSequence(short seqHandle, s32 volLeft, s32 volRight)
{
    SpuSetVoiceVolume((int)seqHandle, volLeft, volRight, 0);
}
/* ========================================================================
 * 11. SpuSetVoiceFlags - Set voice control flags with state checks
 *
 * Sets the pendingKeyOn flag (bit 3) and marks the voice for update
 * (bit 0) on the specified voice. Only modifies flags if the voice
 * is not stopped (bit 9) and not in reset-pending state (bit 2).
 * Clears the needsUpdate flag (bit 1) before setting new flags.
 *
 * bank:       Sound bank index
 * voiceIndex: Voice index within the bank
 * ======================================================================== */
void SpuSetVoiceFlags(s16 bank, s16 voiceIndex)
{
    u32 flags;
    int voicePtr;
    voicePtr = GetVoicePtr(bank, voiceIndex);
    flags = *(u32 *)(voicePtr + 0x90);
    /* Only modify if not stopped (bit 9) and not in reset (bit 2) */
    if (((flags & VOICE_FLAG_KEYOFF) == 0) && ((flags & VOICE_FLAG_PITCH) == 0)) {
        /* Clear needsUpdate (bit 1), set pendingKeyOn (bit 3), set keyOn (bit 0) */
        *(u32 *)(voicePtr + 0x90) = flags & ~0x02;
        *(u32 *)(voicePtr + 0x90) = *(u32 *)(voicePtr + 0x90) | VOICE_FLAG_REVERB;
        *(u32 *)(voicePtr + 0x90) = *(u32 *)(voicePtr + 0x90) | VOICE_FLAG_ACTIVE;
    }
}
/* ========================================================================
 * 12. SpuSetVoiceFlagsSimple - Set voice flags for voice 0
 *
 * Convenience wrapper for SpuSetVoiceFlags that always targets
 * voice index 0 of the given bank.
 * ======================================================================== */
void SpuSetVoiceFlagsSimple(s16 bank)
{
    SpuSetVoiceFlags(bank, 0);
}
/* ========================================================================
 * 13. SpuSetVoiceFade - Configure volume fade on a voice
 *
 * Sets up a linear volume fade from fadeStartVol to fadeTargetVol
 * over fadeTotalFrames. The fade direction (sign of fadeDelta) is
 * determined by which volume is larger. If the voice is not stopped
 * (bit 9) and not in reset (bit 2), applies the fade parameters
 * and marks the voice for fade processing (bit 4) and clears the
 * reverse flag (bit 5).
 *
 * bank:       Sound bank index
 * voiceIndex: Voice index within the bank
 * fadeStartVol: Starting volume level (absolute value used for delta)
 * fadeTotalFrames: Number of frames for the fade to complete
 * ======================================================================== */
void SpuSetVoiceFade(s16 bank, s16 voiceIndex, s16 fadeStartVol, u32 fadeTotalFrames)
{
    u32 absVol;
    u32 flags;
    int voicePtr;
    int *bankVoices;
    voicePtr = GetVoicePtr(bank, voiceIndex);
    /* Only apply fade if voice is not stopped or in reset */
    if (((*(u32 *)(voicePtr + 0x90) & VOICE_FLAG_LOOP) == 0) &&
        ((*(u32 *)(voicePtr + 0x90) & VOICE_FLAG_PITCH) == 0)) {
        absVol = (u32)fadeStartVol;
        if (absVol != 0) {
            /* Use absolute value for delta calculation */
            if ((s32)absVol < 0) {
                absVol = -absVol;
            }
            /* Store fade parameters */
            *(s16 *)(voicePtr + 0x3e) = fadeStartVol;   /* fadeStartVol */
            *(u32 *)(voicePtr + 0x94) = fadeTotalFrames; /* fadeTargetVol2 */
            *(s16 *)(voicePtr + 0x40) = fadeStartVol;   /* fadeTargetVol */
            *(u32 *)(voicePtr + 0x98) = fadeTotalFrames; /* fadeTotalFrames */
            /* Calculate fade delta: positive = fade up, negative = fade down */
            if (absVol < fadeTotalFrames) {
                if (absVol == 0) {
                    trap(0x1c00);  /* Division by zero */
                }
                *(s16 *)(voicePtr + 0x42) = (s16)(fadeTotalFrames / absVol);
            } else {
                if (fadeTotalFrames == 0) {
                    trap(0x1c00);  /* Division by zero */
                }
                *(s16 *)(voicePtr + 0x42) = -(s16)(absVol / fadeTotalFrames);
            }
        }
    }
    /* Mark voice for fade processing: set bit 4, clear bit 5 */
    bankVoices = (int *)&SpuVoicePtrTable + (bank * 4);
    voicePtr = voiceIndex * 0xac + *bankVoices;
    *(u32 *)(voicePtr + 0x90) = *(u32 *)(voicePtr + 0x90) | VOICE_FLAG_PLAYING;
    voicePtr = voiceIndex * 0xac + *bankVoices;
    *(u32 *)(voicePtr + 0x90) = *(u32 *)(voicePtr + 0x90) & ~VOICE_FLAG_STOPPED;
}
/* ========================================================================
 * 14. SpuSetVoiceFadeSimple - Configure fade on voice 0
 *
 * Convenience wrapper for SpuSetVoiceFade targeting voice 0.
 * ======================================================================== */
void SpuSetVoiceFadeSimple(s16 bank, s16 fadeStartVol, u32 fadeTotalFrames)
{
    SpuSetVoiceFade(bank, 0, fadeStartVol, fadeTotalFrames);
}
/* ========================================================================
 * 15. SpuParseSeqHeader - Parse a SEQ format sequence header
 *
 * Parses a PS1 SEQ sequence data stream. Detects the format version,
 * reads the sample rate, calculates playback speed based on the
 * frame rate, and initializes the voice's sequence state.
 *
 * Supports two SEQ format versions:
 *   - Version 1 ('S'/'p' header + version byte 0x01): Full parsing
 *   - Old format: Returns 0 (no additional parsing needed)
 *
 * channel:    Channel index (used as voice identifier)
 * soundBankId: Sound bank ID to associate with this sequence
 * seqData:    Pointer to the raw SEQ data stream
 *
 * Returns 0 on success, -1 on invalid format.
 * ======================================================================== */
u32 SpuParseSeqHeader(u16 channel, s16 soundBankId, char *seqData)
{
    int voicePtr;
    u32 sampleRate;
    u32 result;
    int i;
    s32 framesPerSec;
    short playSpeedCalc;
    int voiceOffset;
    result = 0;
    voicePtr = GetVoicePtr((s16)channel, 0);
    /* Initialize voice fields */
    *(s16 *)(voicePtr + 0x4c) = soundBankId;
    *(u16 *)(voicePtr + 0x4a) = 0;     /* seqType */
    *(u8 *)(voicePtr + 0x13) = 0;      /* effect1Type */
    *(u8 *)(voicePtr + 0x14) = 0;      /* effect2Type */
    *(u8 *)(voicePtr + 0x29) = 0;      /* effectChangeCount */
    *(u8 *)(voicePtr + 0x15) = 0;      /* volumeParam */
    *(u8 *)(voicePtr + 0x16) = 0;      /* sfxType */
    *(u8 *)(voicePtr + 0x2a) = 0;      /* volumeChangeCount */
    *(u8 *)(voicePtr + 0x12) = 0;      /* channelIndex */
    *(u32 *)(voicePtr + 0x7c) = 0;     /* initialDataOffset */
    *(u32 *)(voicePtr + 0x80) = 0;     /* seqOffset */
    *(u32 *)(voicePtr + 0x84) = 0;     /* sampleRate */
    *(u16 *)(voicePtr + 0x72) = 0;     /* playSpeedBackup */
    *(u16 *)(voicePtr + 0x48) = 0;     /* reserved */
    *(u8 *)(voicePtr + 0x2b) = 0;      /* keyOn */
    *(u32 *)(voicePtr + 0x88) = 0;     /* currentDataOffset */
    *(u8 *)(voicePtr + 0x27) = 0;      /* damperMode */
    *(u8 *)(voicePtr + 0x28) = 0;      /* damperCounter */
    *(u8 *)(voicePtr + 0x10) = 0;      /* mode */
    *(u8 *)(voicePtr + 0x11) = 0;      /* keyOff */
    /* Default values */
    *(u16 *)(voicePtr + 0xa8) = SOUND_VOLUME_DEFAULT;  /* panVolL */
    *(u16 *)(voicePtr + 0xaa) = 0;     /* panVolR */
    /* Initialize per-channel volumes and effect levels */
    voiceOffset = voicePtr;
    for (i = 0; i < 0x10; i++) {
        *(s8 *)(voicePtr + i + 0x2c) = (s8)i;           /* channelOrder[i] = i */
        *(u8 *)(voicePtr + i + 0x17) = SOUND_VOLUME_HALF;             /* volume[i] = default (0x40) */
        *(u16 *)(voiceOffset + 0x4e) = SOUND_VOLUME_DEFAULT;             /* effectVol[i] = max */
        i++;
        voiceOffset += 2;
    }
    /* Set default play speed */
    *(u16 *)(voicePtr + 0x6e) = 1;    /* playSpeed */
    /* Store sequence data pointer */
    *(char **)(voicePtr + 4) = seqData;
    /* Check for 'S' or 'p' header (SEQ format marker) */
    if ((*seqData == 'S') || (*seqData == 'p')) {
        /* Skip 8-byte header */
        *(char **)(voicePtr + 4) = seqData + 8;
        if (seqData[7] == '\x01') {
            /* Version 1 SEQ format - full parsing */
            char highByte;
            char lowByte;
            byte b1, b2, b3;
            byte *dataPtr;
            u32 frameRate;
            u32 speedRatio;
            u32 ticksPerSec;
            /* Read sequence type (2 bytes big-endian) */
            *(char **)(voicePtr + 4) = seqData + 9;
            highByte = seqData[8];
            *(char **)(voicePtr + 4) = seqData + 10;
            lowByte = seqData[9];
            *(u16 *)(voicePtr + 0x4a) = (u16)((u8)highByte << 8 | (u8)lowByte);
            /* Read 3-byte sample rate (big-endian) */
            dataPtr = *(byte **)(voicePtr + 4);
            *(byte **)(voicePtr + 4) = dataPtr + 1;
            b1 = *dataPtr;
            *(byte **)(voicePtr + 4) = dataPtr + 2;
            b2 = dataPtr[1];
            *(byte **)(voicePtr + 4) = dataPtr + 3;
            b3 = dataPtr[2];
            sampleRate = (u32)b1 << 0x10 | (u32)b2 << 8 | (u32)b3;
            if (sampleRate == 0) {
                trap(0x1c00);  /* Division by zero */
            }
            /* Calculate frames per second from sample rate */
            *(u32 *)(voicePtr + 0x84) = sampleRate;
            if (sampleRate >> 1 < 60000000U % sampleRate) {
                *(u32 *)(voicePtr + 0x84) = 60000000 / sampleRate + 1;
            } else {
                *(u32 *)(voicePtr + 0x84) = 60000000 / sampleRate;
            }
            /* Store backup of sample rate */
            *(u32 *)(voicePtr + 0x8c) = *(u32 *)(voicePtr + 0x84);
            /* Skip 2 padding bytes */
            *(int *)(voicePtr + 4) = *(int *)(voicePtr + 4) + 2;
            /* Get initial data offset from sequence */
            result = Sound_AdvanceDataPointer();
            /* Calculate total ticks per second */
            speedRatio = (int)*(short *)(voicePtr + 0x4a) * *(int *)(voicePtr + 0x84);
            /* Store initial data offset and base pointers */
            *(u32 *)(voicePtr + 0x0c) = *(u32 *)(voicePtr + 4);  /* loopStartPtr */
            framesPerSec = SpuTickRate;
            *(u32 *)(voicePtr + 0x7c) = result;   /* initialDataOffset */
            *(u32 *)(voicePtr + 0x88) = result;   /* currentDataOffset */
            *(u32 *)(voicePtr + 8) = *(u32 *)(voicePtr + 4);      /* dataBasePtr */
            ticksPerSec = framesPerSec * 0x3c;
            /* Calculate play speed: ticks per second vs sequence speed */
            if (speedRatio * 10 < ticksPerSec) {
                /* Sequence is slower than frame rate - calculate speed multiplier */
                if (speedRatio == 0) {
                    trap(0x1c00);  /* Division by zero */
                }
                playSpeedCalc = (short)((u32)(framesPerSec * 600) / speedRatio);
                *(u16 *)(voicePtr + 0x6e) = playSpeedCalc;     /* playSpeed */
                *(u16 *)(voicePtr + 0x70) = playSpeedCalc;     /* playSpeedDefault */
            } else {
                /* Sequence is faster than frame rate - set max speed */
                if (ticksPerSec == 0) {
                    trap(0x1c00);  /* Division by zero */
                }
                if (ticksPerSec == 0) {
                    trap(0x1c00);  /* Division by zero */
                }
                *(u16 *)(voicePtr + 0x6e) = 0xffff;  /* playSpeed = max */
                playSpeedCalc = (short)((u32)(
                    (int)*(short *)(voicePtr + 0x4a) *
                    *(int *)(voicePtr + 0x84) * 10
                ) / ticksPerSec);
                *(s16 *)(voicePtr + 0x70) = playSpeedCalc;
                /* Round up if remainder exceeds half-tick */
                if ((u32)(framesPerSec * 0x1e) <
                    (u32)((int)*(short *)(voicePtr + 0x4a) *
                          *(int *)(voicePtr + 0x84) * 10) % ticksPerSec) {
                    *(s16 *)(voicePtr + 0x70) = playSpeedCalc + 1;
                }
            }
            /* Store play speed backup */
            result = 0;
            *(u16 *)(voicePtr + 0x72) = *(u16 *)(voicePtr + 0x70);
        } else {
            /* Unknown SEQ version */
            printf("This is not SEQ Data.\n");
            result = (u32)-1;
        }
    } else {
        /* Old SEQ format - no additional parsing needed */
        printf("This is an old SEQ Data Format.\n");
        result = 0;
    }
    return result;
}
/* ========================================================================
 * 16. SpuOpenSequence - Open a sequence for playback
 *
 * Allocates a free channel from the open channel bitmask,
 * initializes the SEQ parser for that channel, and returns
 * the channel index. Returns -1 if no channels are available.
 *
 * seqDataPtr: Pointer to the raw sequence data
 * channel:    Channel/program to associate with
 *
 * Returns channel index on success, -1 on failure.
 * ======================================================================== */
u32 SpuOpenSequence(void *seqDataPtr, short channel)
{
    u32 chanIdx;
    bool found;
    short parseResult;
    if (SpuOpenChannelMask == 0xffffffff) {
        /* All 32 channels in use */
        printf("Can\'t Open Sequence data any more\n\n");
        return (u32)-1;
    }
    chanIdx = 0;
    found = false;
    /* Find first free channel bit */
    do {
        if ((1 << (chanIdx & 0x1f) & SpuOpenChannelMask) == 0) {
            found = true;
        }
        chanIdx++;
    } while (!found);
    chanIdx--;
    /* Mark channel as in-use */
    SpuOpenChannelMask = 1 << (chanIdx & 0x1f) | SpuOpenChannelMask;
    /* Parse the sequence data for this channel */
    parseResult = SpuParseSeqData(chanIdx, (int)channel, seqDataPtr);
    if (parseResult == -1) {
        return (u32)-1;
    }
    return chanIdx;
}
/* ========================================================================
 * 17. SpuStartVoicePlayback - Start voice playback with initial config
 *
 * Configures a voice for playback: clears the stopped flag, sets
 * the panning value, and triggers initial voice setup based on
 * the playMode parameter.
 *
 * bank:       Sound bank index
 * voiceIndex: Voice index
 * playMode:   Playback mode (0x01 = start, 0x00 = pause)
 * initialPan: Initial panning value
 * ======================================================================== */
void SpuStartVoicePlayback(u16 bank, s16 voiceIndex, s8 playMode, s16 initialPan)
{
    int voicePtr;
    int *bankVoices;
    bankVoices = (int *)&SpuVoicePtrTable + ((int)((u32)bank << 0x10) >> 0xe);
    voicePtr = voiceIndex * 0xac + *bankVoices;
    /* Clear stopped (bit 9) and reset-pending (bit 2) flags */
    *(u32 *)(voicePtr + 0x90) = *(u32 *)(voicePtr + 0x90) & ~VOICE_FLAG_KEYON;
    *(u32 *)(voicePtr + 0x90) = *(u32 *)(voicePtr + 0x90) & ~VOICE_FLAG_LOOP;
    /* Store panning value */
    *(s16 *)(voicePtr + 0x46) = initialPan;
    if (playMode == '\x01') {
        /* Start playback: set keyOn (bit 0), clear reserved, set keyOn flag */
        *(u32 *)(voicePtr + 0x90) = *(u32 *)(voicePtr + 0x90) | VOICE_FLAG_ACTIVE;
        *(u16 *)(voicePtr + 0x48) = 0;
        *(u8 *)(voicePtr + 0x2b) = 1;
        /* Trigger initial voice setup via PsyQ */
        SpuSetVoiceVolume(
            (int)(short)(bank | voiceIndex << 8),
            *(u16 *)(voicePtr + 0x74),   /* volumeL */
            *(u16 *)(voicePtr + 0x76),   /* volumeR */
            0
        );
    } else if (playMode == '\0') {
        /* Pause: set needsUpdate (bit 1) */
        *(u32 *)(voicePtr + 0x90) = *(u32 *)(voicePtr + 0x90) | 0x02;
    }
}
/* ========================================================================
 * 18. SpuStartVoicePlaybackSimple - Start voice 0 playback
 *
 * Convenience wrapper for SpuStartVoicePlayback targeting voice 0.
 * ======================================================================== */
void SpuStartVoicePlaybackSimple(s16 bank, s8 playMode, s16 initialPan)
{
    SpuStartVoicePlayback((int)bank, 0, playMode, initialPan);
}
/* ========================================================================
 * 19. SpuResetVoiceBank - Reset all voices in a sound bank
 *
 * Stops all voice playback, releases the channel from the open
 * bitmask, and resets every voice's state fields to defaults.
 * Sets volume to 0x7f (max) for left and right channels.
 *
 * bank: Sound bank index to reset
 * ======================================================================== */
void SpuResetVoiceBank(s16 bank)
{
    int voicePtr;
    int i;
    int *bankVoices;
    u32 chanMask;
    chanMask = (u32)bank;
    /* Stop playback and release channel */
    SpuSetVoiceVolume(chanMask, 0, 0, 1);
    SpuStopVoice(chanMask);
    /* Release this bank's channel from the open bitmask */
    SpuOpenChannelMask = ~(1 << (chanMask & 0x1f)) & SpuOpenChannelMask;
    /* Reset all voices in this bank */
    i = 0;
    if (0 < (int)SpuVoicesPerBank) {
        bankVoices = (int *)&SpuVoicePtrTable + chanMask;
        voicePtr = 0;
        do {
            /* Clear all state fields */
            *(u32 *)(voicePtr + *bankVoices + 0x90) = 0;       /* flags */
            *(u8 *)(voicePtr + *bankVoices + 0x3c) = 0xff;     /* reserved */
            *(u8 *)(voicePtr + *bankVoices) = 0;                /* reserved */
            *(s16 *)(voicePtr + *bankVoices + 0x3e) = 0;       /* fadeStartVol */
            *(s16 *)(voicePtr + *bankVoices + 0x40) = 0;       /* fadeTargetVol */
            *(u32 *)(voicePtr + *bankVoices + 0x94) = 0;       /* fadeTargetVol2 */
            *(u32 *)(voicePtr + *bankVoices + 0x98) = 0;       /* fadeTotalFrames */
            *(s16 *)(voicePtr + *bankVoices + 0x42) = 0;       /* fadeDelta */
            *(u32 *)(voicePtr + *bankVoices + 0xa4) = 0;       /* reserved */
            *(u32 *)(voicePtr + *bankVoices + 0xa0) = 0;       /* reserved */
            *(u32 *)(voicePtr + *bankVoices + 0x9c) = 0;       /* reserved */
            *(s16 *)(voicePtr + *bankVoices + 0x44) = 0;       /* reserved */
            i++;
            /* Set default volume L/R to max (0x7f) */
            *(u16 *)(voicePtr + *bankVoices + 0x74) = SOUND_VOLUME_DEFAULT;    /* volumeL */
            voicePtr = voicePtr + 0xac;
            *(u16 *)(voicePtr + *bankVoices - 0xac + 0x76) = SOUND_VOLUME_DEFAULT; /* volumeR */
        } while (i < (int)SpuVoicesPerBank);
    }
}
/* ========================================================================
 * 20. SpuResetVoiceBankSimple - Reset voice bank (convenience)
 *
 * Convenience wrapper for SpuResetVoiceBank.
 * ======================================================================== */
void SpuResetVoiceBankSimple(s16 bank)
{
    SpuResetVoiceBank((int)bank);
}
/* ========================================================================
 * 21. SpuReadVoiceVolume - Read current volume and pan from a voice
 *
 * Queries the PsyQ library for the current volume and pan values
 * of a voice, then clears the keyOn (bit 0) and pendingKeyOn (bit 3)
 * flags while setting the needsUpdate flag (bit 1) to indicate
 * the voice state has been read.
 *
 * bank:       Sound bank index
 * voiceIndex: Voice index
 * ======================================================================== */
void SpuReadVoiceVolume(u16 bank, s16 voiceIndex)
{
    int voicePtr;
    int *bankVoices;
    bankVoices = (int *)&SpuVoicePtrTable + ((int)((u32)bank << 0x10) >> 0xe);
    voicePtr = voiceIndex * 0xac + *bankVoices;
    /* Read current volume L/R from PsyQ */
    SpuGetVoiceVolume(
        (int)(short)(bank | voiceIndex << 8),
        voicePtr + *bankVoices + 0x78,    /* pitch address */
        voicePtr + *bankVoices + 0x7a     /* reserved address */
    );
    /* Clear keyOn and pendingKeyOn, set needsUpdate */
    *(u32 *)(voicePtr + 0x90) = *(u32 *)(voicePtr + 0x90) & ~VOICE_FLAG_ACTIVE;
    *(u32 *)(voicePtr + 0x90) = *(u32 *)(voicePtr + 0x90) & ~VOICE_FLAG_REVERB;
    *(u32 *)(voicePtr + 0x90) = *(u32 *)(voicePtr + 0x90) | 0x02;
}
/* ========================================================================
 * 22. SpuReadVoiceVolumeSimple - Read volume from voice 0
 *
 * Convenience wrapper for SpuReadVoiceVolume targeting voice 0.
 * ======================================================================== */
void SpuReadVoiceVolumeSimple(s16 bank)
{
    SpuReadVoiceVolume((int)bank, 0);
}
/* ========================================================================
 * 23. SpuGetVtable_ObjectA - Get object type A vtable
 *
 * Returns pointer to SpuSeqObjectVtable vtable for sequence-related objects.
 * ======================================================================== */
void *SpuGetVtable_ObjectA(void)
{
    return &SpuSeqObjectVtable;
}
/* ========================================================================
 * 24. CreateSeqChannel - Allocate and construct a SEQ channel object
 *
 * Allocates a 0x50-byte object, retrieves the SEQ channel vtable,
 * and calls its constructor. Returns the new object, or 0 on failure.
 *
 * initData: Initialization parameter passed to constructor
 * ======================================================================== */
int CreateSeqChannel(void *initData)
{
    int obj;
    int vtable;
    obj = MemAlloc(0x50);
    vtable = 0;
    if (obj != 0) {
        vtable = (int)SpuGetVtable_SeqChannelAccessor();
        (*(code *)(vtable + 8))(obj, initData);
        vtable = obj;
    }
    return vtable;
}
/* ========================================================================
 * 25. SpuGetVtable_SeqTrackAccessor - Get SEQ track vtable
 *
 * Returns pointer to SpuSeqTrackVtable vtable for SEQ track objects.
 * ======================================================================== */
void *SpuGetVtable_SeqTrackAccessor(void)
{
    return &SpuSeqTrackVtable;
}
/* ========================================================================
 * 26. SpuDrawLineSegments - Draw line segments for sequence visualization
 *
 * Draws a series of horizontal and vertical line segments using
 * the PsyQ line drawing primitive. Used for visualizing sequence
 * playback state or debug display.
 *
 * segments:   Array of segment definitions (x, y, length, color)
 * count:      Number of segments to draw
 * endpoints:  Start/end points for line drawing
 * ======================================================================== */
void SpuDrawLineSegments(short *segments, int count, short *endpoints)
{
    int *lineBuffer;
    int segIdx;
    code *drawLine;
    short lineX, lineY;
    int lineLen;
    u32 lineColor;
    lineBuffer = (int *)GetGpuDisplayList();
    drawLine = *(code **)(*lineBuffer + 100);
    if ((count != 0) && (segIdx = 0, 0 < count)) {
        do {
            /* Draw horizontal line segment */
            lineX = *segments + segments[2] - 1;
            lineY = segments[1];
            lineLen = 1;
            lineColor = *(u32 *)(segments + 4);
            (*drawLine)(lineBuffer, &lineX, (int)*endpoints, (int)endpoints[1]);
            /* Draw vertical line segment */
            lineX = *segments;
            lineY = segments[1];
            lineLen = *(int *)(segments + 2) - 1;
            lineColor = *(u32 *)(segments + 4);
            (*drawLine)(lineBuffer, &lineX, *segments + 1, (int)segments[1]);
            /* Draw connecting line from endpoint to next segment */
            lineX = *endpoints;
            lineY = endpoints[1];
            segIdx++;
            lineLen = 1;
            lineColor = *(u32 *)(segments + 4);
            (*drawLine)(lineBuffer, &lineX, (int)*segments, (int)segments[1]);
        } while (segIdx < count);
    }
}
/* ========================================================================
 * 27. CreateSeqLineDraw - Allocate and construct a line draw object
 *
 * Allocates a 0xdc-byte object, retrieves the line draw vtable,
 * and calls its constructor. Returns the new object, or 0 on failure.
 *
 * initParam1-4: Initialization parameters passed to constructor
 * ======================================================================== */
int CreateSeqLineDraw(void *initParam1, void *initParam2, void *initParam3, void *initParam4)
{
    int obj;
    int vtable;
    obj = MemAlloc(0xdc);
    vtable = 0;
    if (obj != 0) {
        vtable = (int)SpuGetVtable_SeqLineDraw();
        (*(code *)(vtable + 8))(obj, initParam1, initParam2, initParam3, initParam4);
        vtable = obj;
    }
    return vtable;
}
/* ========================================================================
 * 28. SpuGetVtable_ObjectB - Get object type B vtable
 *
 * Returns pointer to SpuSeqTrackVtableB vtable for object type B.
 * ======================================================================== */
void *SpuGetVtable_ObjectB(void)
{
    return &SpuSeqTrackVtableB;
}
/* ========================================================================
 * 29. CreateSeqTrack - Allocate and construct a SEQ track object
 *
 * Allocates a 0xa4-byte object, retrieves the SEQ track vtable,
 * and calls its constructor. Returns the new object, or 0 on failure.
 *
 * initParam1-3: Initialization parameters passed to constructor
 * ======================================================================== */
int CreateSeqTrack(void *initParam1, void *initParam2, void *initParam3)
{
    int obj;
    int vtable;
    obj = MemAlloc(0xa4);
    vtable = 0;
    if (obj != 0) {
        vtable = (int)SpuGetVtable_ObjectC();
        (*(code *)(vtable + 8))(obj, initParam1, initParam2, initParam3);
        vtable = obj;
    }
    return vtable;
}
