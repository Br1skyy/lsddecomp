// DreamSys - the dream state machine. ~2k lines that actually compile.
// the rest of this project is a war zone but at least THIS file works.
#include "common.h"
#include "MEMORY.H"
#include "RAND.H"
#include "DreamSys.h"
#include "StageGrid.h"
// BSS globals (sbss region)
s8 (*gpNavChallengesComplete)[30];
s32 *gpDinamicLinkPenalty;
s32 Spawn_CurrentChunkIdx;
s32 Spawn_CurrentEntryIdx;
s32 Spawn_CurrentType;
s32 Spawn_CurrentSubType;
// Forward declarations for functions defined later in this file
static int TestTunnelDirection(int *outDir1, int *outDir2, int angle);
static bool CheckAngleProximity(int angle, u8 dir);
static // Validates stage link. Returns stage ID or -1.
s32 ValidateStageLink(int linkCoords, int stage, int *data, u32 flags);
static // Gets staircase type pointer for current stage.
void *GetStaircaseTypePtr(void);
static u64 TestStaircaseDirection(void *outDir1, void *outDir2, void *angle);
// 10-bit fixed-point lerp.
int InterpolateValue(int a, int b, int t);
// Decrements flashback sequence counter.
void DreamSys__StepFlashbackSequence(DreamSys *this);
// Direction vector from current toward staircase exit.
void DreamSys__SetStaircaseDirection(DreamSys *this, s16 *target, s16 *current);
// Looks up staircase type by stage.
int GetStaircaseType(void);
// Direction shift from global staircase state.
u8 GetStaircaseShift(void);
// Core spawn matcher: iterates triggers for a position match.
int GetStaticSpawn(u32 outPos, s16 *entry, int chunkIdx, int countBase, int entryBase, int indexBase, int updateFlag);
// External functions (defined in other translation units)
extern int MemAlloc(int size);           // MemAlloc
extern void MemFree(void *ptr);          // MemFree
extern void UpdateTransformPosition();               // some utility
extern void ReadDreamSysVector();               // some utility
extern int RangeCompare();                // some utility
/* GetGameManager: returns singleton GameManager object pointer (GameManagerPtr) */
extern int D_800878D4;
#define GetGameManager() (int)(&D_800878D4)
extern void SceneInitTimeline();
extern void SceneUpdateTimeline();
extern void SceneReleaseTimeline();
// Global state variables (extern)
extern int DreamDefaultVisual;
extern int DreamColorMapTable[3][3];
extern int DreamCamMotionAmp;
extern s8 DreamLinkActionScale[];
extern int DreamLinkActionFuncs;
extern int DreamCamSwayAccel;
extern int DreamCamSwayLimit;
extern int DreamCamMotionStep;
extern int DreamCamMotionClamp;
extern int DreamColourEffectData;
extern s16 DreamCamMotionStepVal;
extern int DreamFlashbackReverb;
extern s8 DreamLinkSoundBank[];
extern s8 DreamLinkSoundOffset[];
extern int DreamInterpParamBuf;
extern int DreamInterpParamStore;
extern void *PTR_func_8005AB2C_80087eec[];
extern int DreamStaircaseColourAC;
extern int DreamStaircaseColourBD;
extern s16 DreamStaircaseDirData[];
extern u8 DreamSpawnPosData[];
extern u8 DreamSpawnTileData[];
extern u8 DreamSpawnPosAdjust[];
extern int Unk_DreamBuf58[];
extern s16 DreamAngleDataTable[];
extern void *Ptr_Unk88858;
extern void *Ptr_Unk889b8;
extern void *Ptr_Unk88bdc;
extern void *Ptr_Unk88ba4;
extern void *Ptr_Unk88c4c;
extern void *Ptr_Unk88c84;
extern int DreamStaircaseSpawnCount;
extern int DreamStaircaseDefaultData;
extern int DreamInstantTeleportFlag;
extern s16 DreamStaircaseTargetA[];
extern s16 DreamStaircaseTargetB[];
extern s16 DreamStaircaseTargetC[];
extern s16 DreamStaircaseTargetD[];
extern u8 Spawn_VisitFlagArray[];
extern MoodGraphPoint DreamNullMoodPoint;
// Standard library
// Internal field offset constants (fields not named in DreamSys struct)
#define OFFSET_0x04          0x04   // unknown_values_0x4[0]
#define OFFSET_0x14          0x14   // unknown_values_0x4[16..19] as int
#define OFFSET_0x28          0x28   // unknown_values_0x28 (byte[28])
#define OFFSET_0x44          0x44   // unknwon_int_0x44
#define OFFSET_0x48          0x48   // unknown_values_0x48[32]
#define OFFSET_0x4C          0x4C   // s32 entity pointer in 0x48 range
#define OFFSET_0x58          0x58   // s32 object ptr in 0x48 range
#define OFFSET_0x5C          0x5C   // s32 object ptr in 0x48 range
#define OFFSET_0x60          0x60   // s32 in 0x48 range
#define OFFSET_0x64          0x64   // s32 in 0x48 range
#define OFFSET_0x68          0x68   // isFlashbackSession (or color value)
#define OFFSET_0x6C          0x6C   // s32 mode flag
#define OFFSET_0x70          0x70   // s32 flag / link state
#define OFFSET_0x74          0x74   // s32 flag
#define OFFSET_0x78          0x78   // s32 flag
#define OFFSET_0x7C          0x7C   // s32 flag
#define OFFSET_0x80          0x80   // callback ptr or state
#define OFFSET_0x84          0x84   // link type
#define OFFSET_0x88          0x88   // camera motion type
#define OFFSET_0x8C          0x8C   // camera motion counter
#define OFFSET_0x90          0x90   // camera motion param (also timer in seconds calc)
#define OFFSET_0x94          0x94   // camera motion param
#define OFFSET_0x98          0x98   // callback ptr or state
#define OFFSET_0x9C          0x9C   // derail state
#define OFFSET_0xA0          0xA0   // flashback/effect seq counter
#define OFFSET_0xA4          0xA4   // color effect state
#define OFFSET_0xA8          0xA8   // flashback/effect flag
#define OFFSET_0xAC          0xAC   // staircase/link type
#define OFFSET_0xB0          0xB0   // saved staircase/link type
#define OFFSET_0xB4          0xB4   // flashback effect counter
#define OFFSET_0xB8          0xB8   // sound ID for links
#define OFFSET_0xBC          0xBC   // sound handle
#define OFFSET_0xC0          0xC0   // unknown
#define OFFSET_0xC4          0xC4   // flashback reverb flag
#define OFFSET_0xC8          0xC8   // flashback flag
#define OFFSET_0xCC          0xCC   // flashback buffer start
#define OFFSET_0x120         0x120  // timer modulo value
#define OFFSET_0x124         0x124  // tunnel coord / timer check
#define OFFSET_0x128         0x128  // tunnel coord
#define OFFSET_0x12C         0x12C  // tunnel coord
#define OFFSET_0x130         0x130  // tunnel coord
#define OFFSET_0x164         0x164  // currentStage (also in struct)
#define OFFSET_0x16C         0x16C  // linkCoordinates (struct)
#define OFFSET_0x178         0x178  // unknown_sdata_0x178
#define OFFSET_0x17C         0x17C  // currentYear
#define OFFSET_0x180         0x180  // currentDay (also mood count)
#define OFFSET_0x184         0x184  // totalFlasbackUnlockScore
#define OFFSET_0x188         0x188  // navigationFlasbackUnlockScore
#define OFFSET_0x18C         0x18C  // instanceFlasbackUnlockScore
#define OFFSET_0x190         0x190  // moodPreviousDays[0]
#define OFFSET_0x678         0x678  // unknown in 0x684 area
#define OFFSET_0x880         0x880  // unknown_values_0x880[0]
#define OFFSET_0x884         0x884  // unknown_values_0x880[1]
#define OFFSET_0x888         0x888  // unknown_values_0x880[2]
#define OFFSET_0x890         0x890  // flashback save buffer
#define OFFSET_0x8E0         0x8E0  // flashback save buffer
#define OFFSET_0x900         0x900  // flashback save buffer
#define OFFSET_0x908         0x908  // staircase active flag
#define OFFSET_0x90C         0x90C  // staircase flag
#define OFFSET_0x910         0x910  // staircase anim func ptr
#define OFFSET_0x914         0x914  // staircase anim frame
#define OFFSET_0x91B         0x91B  // staircase link chunk data
#define OFFSET_0x91F         0x91F  // staircase link pos data
#define OFFSET_0x924         0x924  // stored/unknown
#define GET_INT(base, off)     (*(s32 *)((int)(base) + (off)))
#define SET_INT(base, off, v) (*(s32 *)((int)(base) + (off)) = (s32)(v))
#define GET_UINT(base, off)   (*(u32 *)((int)(base) + (off)))
#define GET_PTR(base, off)    (*(void **)((int)(base) + (off)))
int New_DreamSys(void *manager, void *subsysRef, void *subsysRef2) {
    int alloc;
    int result;
    alloc = MemAlloc(sizeof(DreamSys));
    result = 0;
    if (alloc != 0) {
        result = (int)Get_vtable_DreamSys();
        (**(code **)(result + 8))(alloc, manager, subsysRef, subsysRef2);
        result = alloc;
    }
    return result;
}
// Constructor. Sets up vtable, stores manager + subsystem refs.
void DreamSys__DreamSys(DreamSys *this, void *manager, int objPtr, int objPtr2) {
    int gm;
    void *uVar2;
    gm = GetGameManager();
    // god I hate this vtable dispatch pattern
    (**(code **)(gm + 8))(this);
    this->vt = Get_vtable_DreamSys();
    SET_INT(this, 0x58, objPtr);
    SET_INT(this, 0x5c, objPtr2);
    SET_INT(this, 0x64, 0);
    SET_INT(this, 0x60, (int)manager);
    uVar2 = (**(code **)(*(int *)manager + 0x80))(manager, 0);
    (**(code **)((int)this->vt + 0x10))(this, uVar2);
    (**(code **)((int)this->vt + 0x104))(this, -1);
    SET_INT(this, 0x70, 1);
    SET_INT(this, 0x6c, 0);
    SET_INT(this, 0x878, 1);
    (**(code **)((int)this->vt + 0x198))(this);
    (**(code **)((int)this->vt + 0x40))(this);
}
void DreamSys__InitFields(DreamSys *this) {
    (**(code **)((int)this->vt + 0x60))(this, 0);
    (**(code **)((int)this->vt + 0x44))(this, 1, (int *)&DreamDefaultVisual);
    SET_INT(this, 0x80, 0);
    SET_INT(this, 0x98, 0);
    SET_INT(this, 0xcc, 0);
    SET_INT(this, 0x908, 0);
    SET_INT(this, 0x90c, 0);
    SET_INT(this, 0x910, 0);
    SET_INT(this, 0x78, 0);
    SET_INT(this, 0x924, 0);
}
// Wires up GameManager data, special stages, and flashback offsets.
void DreamSys__SetupFromManager(DreamSys *this, void *manager) {
    int gm;
    s8 buffer[16];
    (**(code **)(*(int *)manager + 0xe4))(manager, buffer, this, &this->linkCoordinates);
    gm = GetGameManager();
    (**(code **)(gm + 0x4c))(this, manager, buffer);
    (**(code **)((int)this->vt + 0x10))(this, manager);
    if (this->unknwon_int_0x44 == 0xe) {
        int idx = GET_INT(this, 0x87c);
        (**(code **)((int)this->vt + 0x44))(this, 1, (int)this + idx * 0x24 + 0x47e);
        (**(code **)((int)this->vt + 0x104))(this, *(s16 *)((int)this + idx * 0x24 + 0x48a) + 4);
        SET_INT(this, 0x87c, idx + 1);
    }
    if ((GET_INT(this, 0x6c) != 0) && (GET_INT(this, 0x888) != 0)) {
        (**(code **)((int)this->vt + 0x44))(this, 1);
    }
}
void DreamSys__Shutdown(DreamSys *this) {
    int gm;
    (**(code **)(**(int **)GET_PTR(this, 0x4c) + 0xf0))();
    (**(code **)((int)this->vt + 0x14))(this, GET_PTR(this, 0x4c));
    gm = GetGameManager();
    (**(code **)(gm + 0x50))(this);
}
// -2 = dynamic, -1 = normal link, others = direct transition.
void DreamSys__HandleLinkTransition(DreamSys *this, int linkType) {
    int gm;
    gm = GetGameManager();
    (**(code **)(gm + 0x88))(this, linkType);
    if (linkType == -2) {
        int *entity = (int *)GET_PTR(this, 0x4c);
        int result = (**(code **)(*entity + 0x11c))(entity, GET_INT(this, 0x14) + 0x18);
        if (*(s16 *)(*(int *)(result + 4) + 0x2c) != 2) {
            (**(code **)((int)this->vt + 0x1d4))(this);
            return;
        }
    } else if (linkType == -1) {
        u32 val = *(u16 *)(GET_INT(this, 0x28) + 0x36) & 0x7f;
        SET_INT(this, 0xb8, val);
        if (val > 0x17) {
            SET_INT(this, 0xb8, 0);
        }
        if ((this->unknwon_int_0x44 == 0xf) && (GET_INT(this, 0xb8) == 0)) {
            SET_INT(this, 0xb8, 2);
        }
        if (this->currentStage != 9) {
            return;
        }
    } else {
        return;
    }
    void *spawnData = (**(code **)(**(int **)GET_PTR(this, 0x4c) + 0x10c))(GET_PTR(this, 0x4c), 0, 0);
    (**(code **)((int)this->vt + 0x1c4))(this, spawnData);
}
void DreamSys__HandleInstanceEffect(DreamSys *this, void *unused, int effect) {
    if ((GET_INT(this, 0x6c) == 0) && (GET_INT(this, 0x70) == 0) && (GET_INT(this, 0x908) == 0)) {
        switch (effect) {
        case 2:  SET_INT(this, 0xa0, 1); break;
        case 3:  SET_INT(this, 0xa0, 2); break;
        case 4:  SET_INT(this, 0xa4, 1); break;
        case 5:  SET_INT(this, 0xa4, 2); break;
        case 6:  SET_INT(this, 0x88, 1); break;
        case 7:
            if (GET_INT(this, 0xa0) == 1) {
                (**(code **)((int)this->vt + 0x184))(this, 4);
            }
            break;
        case 8:  SET_INT(this, 0x88, 2); break;
        case 0xd: SET_INT(this, 0x90, 2); break;
        case 0xe: SET_INT(this, 0xa0, 4); break;
        case 0xf: SET_INT(this, 0x90, 1); break;
        case 0x10: SET_INT(this, 0xa0, 3); break;
        case 0x19: SET_INT(this, 0x74, 1); break;
        case 0x22:
            (**(code **)((int)this->vt + 0x188))();
        }
    }
}
// Core dream timer. Triggers flashbacks or day transitions when time runs out.
void DreamSys__TimerTick(DreamSys *this, void *unused, int tickType) {
    u32 timer;
    if (tickType == 2) {
        timer = this->dreamTimer;
        this->dreamTimer = timer + 1;
        if (timer < (u32)this->dreamTimeLimit) {
            (**(code **)((int)this->vt + 0x118))(this);
            (**(code **)((int)this->vt + 0x11c))(this);
        } else {
            if (GET_INT(this, 0x68) == 0) {
                (**(code **)((int)this->vt + 0x218))(this, 0, 0x10);
            } else if ((this->unknwon_int_0x44 != 0) ||
                       (s32)(**(code **)((int)this->vt + 0x1cc))(this, 0) != 0) {
                this->dreamTimer = 0;
                return;
            }
            (**(code **)((int)this->vt + 0x30))(this, 10);
            this->dreamTimer = 0;
        }
    }
}
void DreamSys__ProcessAreaLink(DreamSys *this, void *linkData, void *entity) {
    int gm;
    gm = GetGameManager();
    (**(code **)(gm + 0x9c))(this, linkData, entity);
    if ((*(u32 *)linkData & 0xfff) == 0x114) {
        (**(code **)((int)this->vt + 0x1e4))(this, linkData, entity);
    }
}
void DreamSys__ProcessEntityLink(DreamSys *this, void *linkData, void *entity) {
    int gm;
    gm = GetGameManager();
    (**(code **)(gm + 0xdc))(this, linkData, entity);
    if ((*(u32 *)linkData & 0xfffff) == 0x1f234) {
        (**(code **)((int)this->vt + 0x1e8))(this, linkData, entity);
    }
}
// Player hit a wall. Checks static link, falls back to dynamic.
void DreamSys__WallLink(DreamSys *this, void *entity, int linkDirection) {
    int gm;
    gm = GetGameManager();
    (**(code **)(gm + 0xe0))(this, entity, linkDirection);
    if ((linkDirection == 4) && (this->unknwon_int_0x44 == 0)) {
        int *piVar7 = (int *)GET_PTR(this, 0x4c);
        // TODO: what the actual fuck is this. unaligned read from a u64 returned by a vtable call,
        // then used as bit offsets for manual byte packing. Ghidra went insane here.
        // this is probably a Vec3 or a struct-of-three-shorts that got flattened into a u64
        // and then unpacked onto the linkCoordinates struct. rewrite this.
        u64 uVar9 = (**(code **)(*piVar7 + 0xd4))(piVar7, entity);
        u32 uVar5 = (u32)uVar9;
        u32 uVar2 = uVar5 + 3 & 3;
        u32 uVar8 = uVar5 & 3;
        u32 uVar6 = (*(int *)((uVar5 + 3) - uVar2) << (3 - uVar2) * 8 |
                (u32)(uVar9 >> 32) & 0xffffffffU >> (uVar2 + 1) * 8) &
                -1 << (4 - uVar8) * 8 | *(u32 *)(uVar5 - uVar8) >> uVar8 * 8;
        uVar2 = uVar5 + 7 & 3;
        uVar8 = uVar5 + 4 & 3;
        uVar8 = (*(int *)((uVar5 + 7) - uVar2) << (3 - uVar2) * 8 |
                (u32)piVar7 & 0xffffffffU >> (uVar2 + 1) * 8) & -1 << (4 - uVar8) * 8 |
                *(u32 *)((uVar5 + 4) - uVar8) >> uVar8 * 8;
        u16 uVar1 = *(u16 *)(uVar5 + 8);
        // Unaligned write to linkCoordinates chunk and tile
        uVar2 = (int)this + 0x16fU & 3;
        *(u32 *)(((int)this + 0x16fU) - uVar2) = *(u32 *)(((int)this + 0x16fU) - uVar2) & -1 << (uVar2 + 1) * 8 | uVar6 >> (3 - uVar2) * 8;
        uVar2 = (u32)(&this->linkCoordinates) & 3;
        *(u32 *)((int)(&this->linkCoordinates) - uVar2) = *(u32 *)((int)(&this->linkCoordinates) - uVar2) & 0xffffffffU >> (4 - uVar2) * 8 | uVar6 << uVar2 * 8;
        // Unaligned write to linkCoordinates position
        uVar2 = (int)this + 0x173U & 3;
        *(u32 *)(((int)this + 0x173U) - uVar2) = *(u32 *)(((int)this + 0x173U) - uVar2) & -1 << (uVar2 + 1) * 8 | uVar8 >> (3 - uVar2) * 8;
        uVar2 = (u32)(&this->linkCoordinates + 4) & 3;
        *(u32 *)((int)(&this->linkCoordinates + 4) - uVar2) = *(u32 *)((int)(&this->linkCoordinates + 4) - uVar2) & 0xffffffffU >> (4 - uVar2) * 8 | uVar8 << uVar2 * 8;
        *(u16 *)((int)&this->linkCoordinates + 10) = uVar1;
        int linkResult = (**(code **)((int)this->vt + 0x1c8))(this, &this->linkCoordinates);
        if ((linkResult == 0) && (GET_INT(this, 0x124) != 0)) {
            (**(code **)((int)this->vt + 0x1c4))(this);
        }
        (**(code **)((int)this->vt + 0x1d4))(this);
        (**(code **)((int)this->vt + 0xe8))(this);
    }
}
// why does this exist
void DreamSys__EmptyFunc(void) {
}
// Returns current color. value < 0 pulls from mood data instead.
s32 DreamSys__GetSetDreamColor(DreamSys *this, s32 *color, s32 value) {
    s32 oldColor = GET_INT(this, 0x68);
    if (value < 0) {
        *color = CalcDreamColor((MoodGraphPoint *)((int)this + GET_INT(this, 0x180) * 2 + 0x190));
    } else {
        SET_INT(this, 0x68, value);
    }
    return oldColor;
}
// Enters/exits flashback mode.
void DreamSys__SetFlashbackMode(DreamSys *this, int mode) {
    SET_INT(this, 0x6c, mode);
    if (mode != 0) {
        (**(code **)((int)this->vt + 0x180))(this, 1);
        if (GET_INT(this, 0x884) != 0) {
            (**(code **)((int)this->vt + 0x44))(this, 1);
        }
    }
}
// Wipes per-day state. Called every new dream day.
void DreamSys__ResetDayState(DreamSys *this, int mode, void *spawnData) {
    s8 buffer[8];
    s16 val16[2];
    (**(code **)((int)this->vt + 0x1f4))(this, &this->linkCoordinates);
    (**(code **)((int)this->vt + 0x138))(this, 1);
    (**(code **)((int)this->vt + 0x13c))(this, 1);
    (**(code **)((int)this->vt + 0x180))(this, mode);
    SET_INT(this, 0xbc, -1);
    SET_INT(this, 0xb4, 0);
    SET_INT(this, 0xb8, 0);
    SET_INT(this, 0xa0, 0);
    SET_INT(this, 0xa4, 0);
    SET_INT(this, 0x88, 0);
    SET_INT(this, 0x90, 0);
    SET_INT(this, 0x8c, 0);
    SET_INT(this, 0x94, 0);
    (**(code **)((int)this->vt + 0x18c))(this, 0, 1, 1, 1);
    (**(code **)((int)this->vt + 0x190))(this, spawnData);
    *(u16 *)((int)this + 0x16a) = 0xffff;
    SET_INT(this, 0x70, 0);
    this->unknwon_int_0x44 = 0;
    SET_INT(this, 0x74, 0);
    SET_INT(this, 0x908, 0);
    SET_INT(this, 0x90c, 0);
    SET_INT(this, 0x910, 0);
    SET_INT(this, 0x78, 0);
    ReadDreamSysVector(this, buffer);
    val16[0] = 0;
    val16[1] = 1;
    (**(code **)((int)this->vt + 0x44))(this, 1, buffer);
}
void DreamSys__SetFlag70(DreamSys *this) {
    SET_INT(this, 0x70, 1);
}
s32 DreamSys__GetValue74(DreamSys *this) {
    return GET_INT(this, 0x74);
}
// Returns previous limit. Negative value = read-only (store no change).
s32 DreamSys__GetSetDreamTimeLimit(DreamSys *this, s32 value) {
    const s32 TARGET_FPS = 15;
    s32 newLimit;
    s32 oldLimit;
    u32 oldLimitUnsigned;
    newLimit = value;
    if (newLimit >= 0) {
        newLimit *= TARGET_FPS;
    }
    oldLimit = this->dreamTimeLimit;
    oldLimitUnsigned = oldLimit;
    this->dreamTimeLimit = newLimit;
    if (oldLimit >= 0) {
        oldLimit = oldLimitUnsigned / TARGET_FPS;
    }
    return oldLimit;
}
// Frames at 15fps -> seconds.
u32 DreamSys__GetRemainingTimeSeconds(DreamSys *this) {
    return GET_UINT(this, 0x90) / 0xf;
}
void DreamSys__SetOffset58(DreamSys *this, void *value) {
    SET_INT(this, 0x58, value);
}
void DreamSys__SetOffset5C(DreamSys *this, void *value) {
    SET_INT(this, 0x5c, value);
}
void DreamSys__SetOffset64(DreamSys *this, void *value) {
    SET_INT(this, 0x64, value);
}
// Sets +0x74 if dream timer evenly divides +0x120.
void DreamSys__CheckTimerModulo(DreamSys *this) {
    if (GET_INT(this, 0x70) == 0) {
        if (GET_UINT(this, 0x120) == 0) {
            trap(TRAP_DIVZERO); // division by zero guard. ask Japan why this isn't checked earlier.
        }
        SET_INT(this, 0x74, 0);
        SET_INT(this, 0x124, (u32)(GET_UINT(this, 0x90) % GET_UINT(this, 0x120) == 0));
    }
}
// Fires two stored callback pointers.
void DreamSys__CallCallbacks(DreamSys *this) {
    if (*(code **)((int)this + 0x80) != NULL) {
        (**(code **)((int)this + 0x80))();
    }
    if (*(code **)((int)this + 0x98) != NULL) {
        (**(code **)((int)this + 0x98))(this);
    }
}
// Score via interpolation + range comparison.
s32 DreamSys__CalculateScore(DreamSys *this, s32 *outScore, void *interpData, int rangeCount, void *rangeData) {
    int iVar1;
    s32 result;
    s32 local_28;
    int local_24;
    s32 local_20 = 0;
    DreamInterpParamStore = (int)interpData;
    UpdateTransformPosition(this, &local_28, &DreamInterpParamBuf, 0);
    local_24 = InterpolateValue(GET_INT(this, 0x5c) + 0x14, GET_INT(this, 0x5c) + 0x20, (int)interpData);
    if (GET_INT(this, 0xc) == 0) {
        iVar1 = 0;
    } else {
        iVar1 = GET_INT(this, 0x14) + 0x38;
    }
    local_24 += GET_INT(iVar1, 4);
    if (outScore != NULL) {
        *outScore = local_28;
        outScore[1] = local_24;
        outScore[2] = local_20;
    }
    if (rangeCount == 0) {
        result = 0;
    } else {
        result = RangeCompare(&local_28, rangeData, rangeCount);
    }
    return result;
}
// 10-bit fixed-point lerp.
int InterpolateValue(int a, int b, int t) {
    int diff;
    int result;
    if (t < 0) {
        t += 0x3ff;
    }
    diff = GET_INT(b, 8) - GET_INT(a, 8);
    if (diff < 0) {
        diff += 0x3ff;
    }
    diff >>= 10;
    if (diff == 0) {
        diff = 1;
    }
    result = (GET_INT(b, 4) - GET_INT(a, 4)) * (t >> 10);
    if (diff == 0) {
        trap(TRAP_DIVZERO); // same div-by-zero hack as above. should not reach here.
    }
    if ((diff == -1) && (result == -0x80000000)) {
        trap(TRAP_OVERFLOW);
    }
    return result / diff + GET_INT(a, 4);
}
void DreamSys__ResetOffset7C(DreamSys *this) {
    SET_INT(this, 0x7c, 0);
}
void DreamSys__ResetOffset78(DreamSys *this) {
    SET_INT(this, 0x78, 0);
}
s32 DreamSys__AlwaysZero(void) {
    return 0;
}
void DreamSys__DisableEffects(DreamSys *this, int flag) {
    (**(code **)((int)this->vt + 0x13c))(this, 0);
    if (flag != 0) {
        (**(code **)((int)this->vt + 0x138))(this, 0);
    }
}
void DreamSys__SetEffects(DreamSys *this, void *unused, int flag) {
    (**(code **)((int)this->vt + 0x13c))();
    (**(code **)((int)this->vt + 0x138))(this, flag);
}
// Sets type + looks up handler in vtable.
void DreamSys__SetLinkType(DreamSys *this, int linkType) {
    int vt = (int)this->vt;
    SET_INT(this, 0x84, linkType);
    if (linkType == 1) {
        SET_INT(this, 0x80, *(s32 *)(vt + 0x140));
    } else if (linkType < 2) {
        if (linkType == 0) {
            SET_INT(this, 0x80, 0);
        }
    } else if (linkType == 2) {
        SET_INT(this, 0x80, *(s32 *)(vt + 0x14c));
    } else if (linkType == 3) {
        SET_INT(this, 0x80, *(s32 *)(vt + 0x150));
    }
}
void DreamSys__SetDerailState(DreamSys *this, int state) {
    int vt = (int)this->vt;
    if (GET_INT(this, 0x9c) == 2) {
        (**(code **)(vt + 0x17c))(this, 0);
    }
    SET_INT(this, 0x9c, state);
    if (state == 1) {
        SET_INT(this, 0x98, *(s32 *)(vt + 0x154));
    } else if (state < 2) {
        if (state == 0) {
            SET_INT(this, 0x98, 0);
        }
    } else if (state == 2) {
        SET_INT(this, 0x98, *(s32 *)(vt + 0x178));
        SET_INT(this, 0xc4, 1);
        SET_INT(this, 0xc8, 1);
        SceneInitTimeline(GET_PTR(this, 0x58), (int)this + 0xcc, 1, this, *(void **)(vt + 0x194));
    }
}
void DreamSys__UpdateFlashbackState(DreamSys *this) {
    (**(code **)((int)this->vt + 0x144))();
    (**(code **)((int)this->vt + 0x148))(this);
}
// Camera sway oscillation.
void DreamSys__ApplyCameraSway(DreamSys *this) {
    int index;
    u32 newOffset;
    int objBase = GET_INT(this, 0x5c);
    index = GET_INT(this, 0x88) * 4;
    if (GET_INT(this, 0x88) == 0) {
        if (GET_INT(this, 0x8c) == 0) {
            return;
        }
        int step = -600;
        if (GET_INT(this, 0x8c) < 0) {
            step = 600;
        }
        SET_INT(objBase, 0x24, step + GET_INT(objBase, 0x24));
        SET_INT(this, 0x8c, step + GET_INT(this, 0x8c));
        return;
    }
    newOffset = *(u32 *)((int)&DreamCamSwayAccel + index) + GET_INT(this, 0x8c);
    if ((s32)newOffset < 0) {
        if (*(s32 *)((int)&DreamCamSwayLimit + index) <= (s32)(~newOffset + 1)) goto resetSway;
    } else if (*(s32 *)((int)&DreamCamSwayLimit + index) <= (s32)newOffset) {
        SET_INT(this, 0x88, 0);
        return;
    }
    SET_INT(objBase, 0x24, *(s32 *)((int)&DreamCamSwayAccel + index) + GET_INT(objBase, 0x24));
    SET_INT(this, 0x8c, newOffset);
resetSway:
    SET_INT(this, 0x88, 0);
}
// Steps the camera motion state machine.
void DreamSys__UpdateCameraMotion(DreamSys *this) {
    int step;
    u32 offset;
    SET_INT(this, 0xa8, (u32)(GET_INT(this, 0xa0) == 1));
    if (GET_INT(this, 0x90) == 0) {
        if (GET_INT(this, 0x94) == 0) {
            return;
        }
        step = -0x2d;
        if (GET_INT(this, 0x94) < 0) {
            step = 0x2d;
        }
        DreamCamMotionStepVal = (s16)step;
        (**(code **)((int)this->vt + 0x44))(this, 0, &DreamColourEffectData);
        SET_INT(this, 0x94, step + GET_INT(this, 0x94));
        goto applyStep;
    }
    step = GET_INT(this, 0x90) * 4;
    offset = *(u32 *)((int)&DreamCamMotionStep + step) + GET_INT(this, 0x94);
    if ((s32)offset < 0) {
        if ((s32)(~offset + 1) < *(s32 *)((int)&DreamCamMotionClamp + step)) goto resetMotion;
    } else if (*(s32 *)((int)&DreamCamMotionClamp + step) <= (s32)offset) {
        SET_INT(this, 0x90, 0);
        goto applyStep;
    }
    DreamCamMotionStepVal = (s16)*(s32 *)((int)&DreamCamMotionStep + step);
    (**(code **)((int)this->vt + 0x44))(this, 0, &DreamColourEffectData);
    SET_INT(this, 0x94, offset);
resetMotion:
    SET_INT(this, 0x90, 0);
applyStep:
    DreamSys__StepFlashbackSequence(this);
}
// Decrements flashback sequence counter.
void DreamSys__StepFlashbackSequence(DreamSys *this) {
    u32 val = GET_UINT(this, 0xa0);
    SET_INT(this, 0xa8, 0);
    if (val != 0) {
        int next = val + 1;
        if ((val & 1) == 0) {
            next = val - 1;
        }
        SET_INT(this, 0xa0, next);
    }
}
void DreamSys__Empty2(void) {}
void DreamSys__Empty3(void) {}
void DreamSys__DispatchByMode(DreamSys *this) {
    code handler;
    if (GET_INT(this, 0x6c) == 0) {
        (**(code **)((int)this->vt + 0x174))();
        handler = *(code *)((int)this->vt + 0x158);
    } else if (GET_INT(this, 0x6c) == 2) {
        handler = *(code *)((int)this->vt + 0x160);
    } else {
        handler = *(code *)((int)this->vt + 0x15c);
    }
    handler(this);
}
void DreamSys__UpdateFlashbackTimer(DreamSys *this) {
    if (GET_INT(this, 0x70) == 0) {
        void *result = (**(code **)((int)this->vt + 0x164))(this, 1);
        (**(code **)((int)this->vt + 0x170))(this, result);
    }
}
void DreamSys__StartFlashbackTimer(DreamSys *this) {
    SET_INT(this, 0xa0, 1);
    if (GET_INT(this, 0x70) == 0) {
        void *result = (**(code **)((int)this->vt + 0x164))(this, 1);
        (**(code **)((int)this->vt + 0x170))(this, result);
    } else {
        (**(code **)((int)this->vt + 0x164))(this, 0);
    }
}
void DreamSys__SetFlashbackSequenceFlag(DreamSys *this) {
    SET_INT(this, 0xa0, 1);
}
int DreamSys__HandleFlashbackEffect(DreamSys *this, int applyPlayerOffset) {
    bool effectDone;
    int effectType;
    int result;
    effectDone = false;
    effectType = GET_INT(this, 0xa0);
    result = 0;
    if (effectType != 0) {
        u32 counter = GET_UINT(this, 0xb4) + 1;
        SET_INT(this, 0xb4, counter);
        if ((s32)counter < 4) {
            if (GET_INT(this, 0xac) == 4) {
                effectDone = (counter & 1) == 0;
            }
        } else {
            SET_INT(this, 0xa0, 0);
            effectDone = true;
        }
        if (effectDone) {
            (**(code **)((int)this->vt + 0x168))(this);
        }
        result = GET_INT(this, 0x5c);
        if (((result != 0) && (GET_INT(this, 0x678) != 0)) && (applyPlayerOffset != 0)) {
            int offset = -0x32;
            if (GET_INT(this, 0xb4) > 2) {
                offset = 0x32;
            }
            SET_INT(result, 0x18, offset + GET_INT(result, 0x18));
            SET_INT(result, 0x24, offset + GET_INT(result, 0x24));
        }
        result = effectType;
        if (GET_INT(this, 0xa0) == 0) {
            SET_INT(this, 0xb4, 0);
        }
    }
    if (!effectDone) {
        (**(code **)((int)this->vt + 0x16c))(this);
    }
    return result;
}
// Looks up stage sound ID and plays transition SFX.
void DreamSys__PlayLinkSound(DreamSys *this) {
    void *someObj = GET_PTR(this, 0x58);
    int soundId = GET_INT(this, 0xb8);
    if (soundId != 0) {
        s8 bank = DreamLinkSoundBank[soundId];
        s8 offset = DreamLinkSoundOffset[soundId];
        (**(code **)(*(int *)someObj + 0x9c))(someObj, offset);
        void *result = (**(code **)(*(int *)someObj + 0x80))(someObj, bank << 4, 0x6e, 0x6e);
        SET_INT(this, 0xbc, result);
        if (soundId != 0x16) {
            SET_INT(this, 0xbc, -1);
        }
        if (soundId == 0xb) {
            (**(code **)(*(int *)someObj + 0x9c))(someObj, 1);
            (**(code **)(*(int *)someObj + 0x80))(someObj, bank << 4, 0x6e, 0x6e);
            (**(code **)(*(int *)someObj + 0x9c))(someObj, 2);
            (**(code **)(*(int *)someObj + 0x80))(someObj, 0x90, 0x6e, 0x6e);
        }
    }
}
// Stops the link transition sound.
void DreamSys__StopLinkSound(DreamSys *this) {
    if (GET_INT(this, 0xbc) >= 0) {
        (**(code **)(**(int **)GET_PTR(this, 0x58) + 0x84))();
        SET_INT(this, 0xbc, -1);
    }
}
void DreamSys__PerformLinkAction(DreamSys *this, int action) {
    if (action != 0) {
        s8 scale = DreamLinkActionScale[action];
        int magnitude = *(s32 *)((int)&DreamCamMotionAmp + GET_INT(this, 0xac) * 4);
        (**(code **)((int)this->vt + 0x12c))();
        void *spawnData = (**(code **)(**(int **)GET_PTR(this, 0x4c) + 0x10c))(GET_PTR(this, 0x4c), 0, 0);
        int linkResult1 = (**(code **)((int)this->vt + 0x1c4))(this, spawnData);
        int linkResult2 = (**(code **)((int)this->vt + 0x1c8))(this, spawnData);
        int linkResult3 = (**(code **)((int)this->vt + 0x1cc))(this, spawnData);
        if ((linkResult1 == 0) && (linkResult2 == 0) && (linkResult3 == 0)) {
            (**(code **)((int)this->vt + 0x1d4))(this);
            ((void (*)(DreamSys *, int, int))(&DreamLinkActionFuncs + action * 4))(this, scale * magnitude, GET_INT(this, 0x90c) == 0);
            if ((this->currentStage == 0) &&
                (GET_INT(GET_INT(this, 0x14), 0x1c) < -2000) &&
                (-500 < GET_INT(GET_INT(this, 0x14), 0x18))) {
                (**(code **)(this->vt->unknown_functions_0xe4[0]))(this, this, 4);
            }
        }
        *(s32 *)GET_INT(this, 0x14) = 0;
    }
}
// Resets all dream color / tint effects.
void DreamSys__ClearColorEffects(DreamSys *this) {
    if (GET_INT(this, 0xa4) != 0) {
        (**(code **)((int)this->vt + 0x44))(this, 0, (int)&DreamColourEffectData + GET_INT(this, 0xa4) * 0xc);
        SET_INT(this, 0xa4, 0);
    }
}
// Removes overlay + reverb.
void DreamSys__ClearFlashbackEffects(DreamSys *this) {
    if (GET_INT(this, 0xc4) != 0) {
        (**(code **)((int)this->vt + 0xbc))(this, &DreamFlashbackReverb);
        SET_INT(GET_INT(this, 0x5c), 0x24, GET_INT(GET_INT(this, 0x5c), 0x24) + -600);
    }
    if (GET_INT(this, 0xc8) != 0) {
        SceneUpdateTimeline(GET_PTR(this, 0x58), (int)this + 0xcc);
    }
}
void DreamSys__InitInstanceLink(DreamSys *this, int enableTimeline) {
    SET_INT(this, 0xc4, 0);
    SET_INT(this, 0xc8, enableTimeline);
    if (enableTimeline != 0) {
        SceneReleaseTimeline(GET_PTR(this, 0x58), (int)this + 0xcc);
    }
}
s32 DreamSys__GetSetOffsetAC(DreamSys *this, int value) {
    s32 old = GET_INT(this, 0xac);
    if (value >= 0) {
        SET_INT(this, 0xac, value);
        SET_INT(this, 0xb0, value);
    }
    return old;
}
void DreamSys__SetOffsetAC(DreamSys *this, int value) {
    if (GET_INT(this, 0xac) != value) {
        SET_INT(this, 0xb0, GET_INT(this, 0xac));
        SET_INT(this, 0xac, value);
    }
}
void DreamSys__RestoreOffsetAC(DreamSys *this) {
    SET_INT(this, 0xac, GET_INT(this, 0xb0));
}
void DreamSys__SetTunnelCoords(DreamSys *this, int a, int b, int c, int d) {
    if (a >= 0) SET_INT(this, 0x124, a);
    if (b >= 0) SET_INT(this, 0x128, b);
    if (c >= 0) SET_INT(this, 0x12c, c);
    if (d >= 0) SET_INT(this, 0x130, d);
}
void DreamSys__SetOffset120(DreamSys *this, void *value) {
    SET_INT(this, 0x120, value);
}
void NavChallengeCheck(void *unused, int *challengeData) {
    if (*challengeData == 1) {
        if (challengeData[1] == (challengeData[1] / 0x14) * 0x14) {
            challengeData[7] = 9;
            challengeData[8] = -1;
        } else {
            challengeData[0xc] = 9;
            challengeData[0xd] = -1;
        }
    }
}
void DreamSys__InitNewGame(DreamSys *this) {
    this->currentYear = 0;
    this->currentDay = 0;
    this->totalFlasbackUnlockScore = 0;
    this->navigationFlasbackUnlockScore = 0;
    this->instanceFlasbackUnlockScore = 0;
    this->amountFlashbacksAvailable = 0;
    this->unknown_values_0x5d8[7] = 0;
    this->unknown_values_0x5d8[0] = 0;
    this->screenShakeOn = 1;
    this->unknown_word_0x67c = 0;
    this->unknown_word_0x680 = 0;
    this->unknown_sdata_0x178 = 0x31304A;
    InitNavChallengesArray((s8(*)[0x1E])this->navChallengesArray, &this->amountDynamicLinksDone);
    memset((u8 *)this->unknown_values_0x684, 0U, 0x1F4);
}
void DreamSys__GetSetScreenShake(DreamSys *this, bool *value) {
    bool oldValue = this->screenShakeOn;
    this->screenShakeOn = *value;
    *value = oldValue;
}
// Returns mood count + optionally writes last logged mood.
int DreamSys__GetMoodCounts(DreamSys *this, s32 *outYear) {
    if (outYear != NULL) {
        *outYear = GET_INT(this, 0x17c);
    }
    return GET_INT(this, 0x180) + 1;
}
s32 DreamSys__AdvanceDay(DreamSys *this) {
    s32 nextDay = this->currentDay + 1;
    this->currentDay = nextDay;
    if (nextDay > 364) {
        this->currentDay = 0;
        this->currentYear++;
    }
    return this->currentDay;
}
void DreamSys__ClearUnk878(DreamSys *this) {
    this->unk_0x878 = 0;
}
s32 DreamSys__GetUnk878(DreamSys *this) {
    return this->unk_0x878;
}
void *DreamSys__GetMoodDataPtr(DreamSys *this, s32 *outSize) {
    if (outSize != NULL) {
        *outSize = 0x700;
    }
    return (int *)this + 0x5e;
}
s32 DreamSys__StartDay(DreamSys *this) {
    MoodGraphPoint *specialMood;
    s32 startStage;
    this->currentFlashbackIndex = 0;
    this->dreamTimer = 0;
    this->storedDay = this->currentDay;
    if (this->isFlashbackSession) {
        this->vt->LoadNextFlashback(this, 1);
    } else {
        specialMood = IsDaySpecial(&this->nextCinematic, this->currentDay + 1);
        this->vt->InitMoodContibutors(this, specialMood);
        if (specialMood != NULL) {
            return -1;
        }
        this->vt->InitSpawnLoc(this);
    }
    startStage = this->currentStage;
    return startStage;
}
s32 DreamSys__EndDay(DreamSys *this, s32 arg1) {
    this->currentDay = this->storedDay;
    if ((this->isFlashbackSession == false) && (arg1 == 0)) {
        this->vt->CalcUnlockScore(this);
        this->vt->UpdateDreamChart(this, &this->moodPreviousDays[this->currentDay]);
        this->vt->AdvanceDay(this);
    } else if (arg1 == 2) {
        this->vt->InitNewGame(this);
        this->unk_0x878 = 1;
    }
    return this->isFlashbackSession;
}
CinematicCall DreamSys__GetCinematic(DreamSys *this) {
    return this->nextCinematic;
}
void DreamSys__InitSpawnLoc(DreamSys *this) {
    MoodGraphPoint mood;
    s32 timeLimit;
    this->vt->GetPreviousDayMood(this, &mood, 1);
    this->currentStage = GenerateInitialSpawn(
        &this->linkCoordinates,
        &timeLimit,
        &mood,
        this->currentDay
    );
    timeLimit = this->vt->GetSetDreamTimeLimit(this, timeLimit);
    this->unknwon_int_0x44 = 0xB;
}
void DreamSys__DynamicLink(DreamSys *this) {
    s32 stage;
    if (this->unknwon_int_0x44 == 0) {
        stage = GetRandomSpawnFromStage(&this->linkCoordinates, this->currentStage, this->dreamTimer);
        ExecuteLink(this, stage, 0xC, 1);
    }
}
bool DreamSys__StaticWallLink(DreamSys *this, PlayerSpawnPoint *currentPos) {
    int linkStage;
    if (this->unknwon_int_0x44 == 0) {
        linkStage = TestForStaticLink(&this->linkCoordinates, currentPos, this->currentStage);
        if (linkStage >= 0) {
            ExecuteLink(this, linkStage, 0xd, 1);
            return true;
        }
    }
    return false;
}
bool DreamSys__LoadNextFlashback(DreamSys *this, bool unknown) {
    FlashbackEntry *entry;
    if (this->currentFlashbackIndex < this->amountFlashbacksAvailable) {
        this->unknwon_int_0x44 = 0xe;
        entry = &this->storedFlasbacks[this->currentFlashbackIndex];
        if (unknown == 0) {
            (**(code **)(*(int *)this + 0x30))(this, 0xe);
        }
        this->currentDay = entry->day;
        this->currentStage = entry->stageID;
        this->linkCoordinates.chunk = entry->position.chunk;
        this->linkCoordinates.tile = entry->position.tile;
        this->linkCoordinates.position = entry->position.position;
        return true;
    }
    return false;
}
int DreamSys__TryTunnelLink(DreamSys *this, void *linkData) {
    int linkIdx;
    s8 buffer[16];
    if (this->unknwon_int_0x44 != 0) {
        return 0;
    }
    linkIdx = Test4TunnelLinks(&this->linkCoordinates, linkData, GET_PTR(this, 0x164));
    if (linkIdx >= 0) {
        ReadDreamSysVector(this, buffer);
        int valid = TestTunnelDirection((int *)((int)this + 0x888), (int *)((int)this + 0x884), (int)buffer);
        if (valid == 0) {
            return 0;
        }
        if (GET_INT(this, 0xa8) != 0) {
            ExecuteLink(this, linkIdx, 0xf, 0);
            return 1;
        }
    }
    return 0;
}
int DreamSys__TryInstanceLink(DreamSys *this, void *data) {
    int linkIdx;
    if ((this->unknwon_int_0x44 == 0) &&
        (linkIdx = ValidateStageLink((int)&this->linkCoordinates,
                                     GET_INT(this, 0x164), (int *)data,
                                     GET_UINT(this, 0x90)),
         linkIdx >= 0)) {
        void *staircaseType = GetStaircaseTypePtr();
        SET_INT(this, 0x880, staircaseType);
        SET_INT(this, 0x884, 0);
        SET_INT(this, 0x888, 0);
        ExecuteLink(this, linkIdx, 0x10, 0);
        return 1;
    }
    return 0;
}
int DreamSys__TryInstantTeleport(DreamSys *this, void *linkData) {
    int linkIdx;
    s8 buffer[16];
    linkIdx = Test4InstantTeleporters(&this->linkCoordinates, linkData, this->currentStage);
    if (linkIdx >= 0) {
        u8 shift = GetStaircaseShift();
        linkIdx = ExecuteLink(this, linkIdx, 0x11, 0);
        if (linkIdx != 0) {
            this->unknwon_int_0x44 = 0;
            (**(code **)(**(int **)GET_PTR(this, 0x4c) + 0xe8))(GET_PTR(this, 0x4c), buffer, &this->linkCoordinates);
            (**(code **)((int)this->vt + 0xb8))(this, buffer);
            if (shift == 0) {
                return 1;
            }
            if (GET_INT(this, 0x68) == 0) {
                int extra = (**(code **)((int)this->vt + 0x108))(this);
                (**(code **)((int)this->vt + 0x104))(this, extra + shift);
            }
        }
        return 1;
    }
    return 0;
}
bool ExecuteLink(DreamSys *this, int stage, int linkType, int playSound) {
    this->unknwon_int_0x44 = linkType;
    (**(code **)(*(int *)this + 0x30))();
    if (this->unknwon_int_0x44 != 0) {
        this->currentStage = stage;
        if (this->isFlashbackSession != 0) {
            this->dreamTimer = 0;
        }
        if (playSound != 0) {
            (**(code **)(*(int *)this->vt + 0x80))(this, 0x90, 0x6e, 0x6e);
        }
        return 1;
    }
    return 0;
}
int DreamSys__TryStaircaseLink(DreamSys *this, u32 nodePtr) {
    s8 buffer[16];
    if (this->unknwon_int_0x44 != 0) {
        return 0;
    }
    // If a staircase animation is in progress, advance it
    if (GET_PTR(this, 0x910) != NULL) {
        int done = (*(s32(*)(DreamSys *))GET_PTR(this, 0x910))(this);
        if (done != 0) {
            SET_INT(this, 0x908, 0);
            SET_INT(this, 0x910, 0);
            SET_INT(this, 0x90c, 0);
            if (GET_INT(this, 0xac) == 4) {
                (**(code **)((int)this->vt + 0x188))(this);
            }
        }
        return 0;
    }
    int linkIdx = Test4StaircaseNodes(&this->linkCoordinates, (void *)nodePtr, this->currentStage);
    if (linkIdx >= 0) {
        ReadDreamSysVector(this, buffer);
        u64 valid = TestStaircaseDirection((void *)((int)this + 0x888), (void *)((int)this + 0x884), buffer);
        if (((s32)valid != 0) && (GET_INT(this, 0xa8) != 0)) {
            // Copy link data with unaligned access (from LWL/LWR instructions)
            u32 valL = *(u32 *)(nodePtr);
            u32 valH = (u32)(valid >> 32);
            u16 valS = *(u16 *)(nodePtr + 8);
            u32 uVar2 = (int)this + 0x91bU & 3;
            *(u32 *)(((int)this + 0x91bU) - uVar2) = *(u32 *)(((int)this + 0x91bU) - uVar2) & -1 << (uVar2 + 1) * 8 | valL >> (3 - uVar2) * 8;
            uVar2 = (u32)((int *)this + 0x246) & 3;
            *(u32 *)((int)((int *)this + 0x246) - uVar2) = *(u32 *)((int)((int *)this + 0x246) - uVar2) & 0xffffffffU >> (4 - uVar2) * 8 | valL << uVar2 * 8;
            uVar2 = (int)this + 0x91fU & 3;
            *(u32 *)(((int)this + 0x91fU) - uVar2) = *(u32 *)(((int)this + 0x91fU) - uVar2) & -1 << (uVar2 + 1) * 8 | valH >> (3 - uVar2) * 8;
            uVar2 = (u32)((int *)this + 0x247) & 3;
            *(u32 *)((int)((int *)this + 0x247) - uVar2) = *(u32 *)((int)((int *)this + 0x247) - uVar2) & 0xffffffffU >> (4 - uVar2) * 8 | valH << uVar2 * 8;
            *(u16 *)((int)this + 0x248) = valS;
            SET_INT(this, 0x908, 1);
            SET_INT(this, 0x90c, 1);
            SET_INT(this, 0x914, 0);
            int stairType = GetStaircaseType();
            SET_INT(this, 0x910, (int)PTR_func_8005AB2C_80087eec[stairType]);
            (**(code **)((int)this->vt + 0x44))(this, 1, GET_PTR(this, 0x884));
            (*(s32(*)(DreamSys *))GET_INT(this, 0x910))(this);
        }
    }
    return 0;
}
s32 DreamSys__StaircaseAnimA(DreamSys *this) {
    int frame;
    if (GET_INT(this, 0x914) == 0) {
        DreamSys__SetStaircaseDirection(this, DreamStaircaseTargetA, (s16 *)((int)this + 0x247));
    }
    if (GET_INT(this, 0xac) == 4) {
        frame = GET_INT(this, 0x914);
        if (frame > 0x12) return 1;
        if ((frame - 8U < 2) || (frame - 0xdU < 2)) {
            (**(code **)((int)this->vt + 0x44))(this, 0, &DreamStaircaseColourAC);
        }
    } else {
        frame = GET_INT(this, 0x914);
        if (frame > 0x84) return 1;
        if ((frame - 0x2bU < 0xf) || (frame - 0x4bU < 0xf)) {
            SET_INT(this, 0xa4, 2);
        }
    }
    SET_INT(this, 0xa0, 1);
    SET_INT(this, 0x914, GET_INT(this, 0x914) + 1);
    return 0;
}
s32 DreamSys__StaircaseAnimB(DreamSys *this) {
    int frame;
    if (GET_INT(this, 0x914) == 0) {
        DreamSys__SetStaircaseDirection(this, DreamStaircaseTargetB, (s16 *)((int)this + 0x247));
    }
    if (GET_INT(this, 0xac) == 4) {
        frame = GET_INT(this, 0x914);
        if (frame > 0x18) return 1;
        if ((frame - 6U < 2) || (frame - 0xbU < 2) || (frame - 0x14U < 2)) {
            (**(code **)((int)this->vt + 0x44))(this, 0, &DreamStaircaseColourBD);
        }
        if (GET_UINT(this, 0x914) - 3U < 0xe) {
            SET_INT(this, 0x88, 2);
        }
    } else {
        frame = GET_INT(this, 0x914);
        if (frame > 0x94) return 1;
        if ((frame - 0x16U < 0xf) || (frame - 0x39U < 0x10) || (frame - 0x6eU < 0xf)) {
            SET_INT(this, 0xa4, 1);
        }
        if (GET_UINT(this, 0x914) - 0x39U < 0x35) {
            SET_INT(this, 0x88, 2);
        }
    }
    SET_INT(this, 0xa0, 1);
    SET_INT(this, 0x914, GET_INT(this, 0x914) + 1);
    return 0;
}
s32 DreamSys__StaircaseAnimC(DreamSys *this) {
    int frame;
    if (GET_INT(this, 0x914) == 0) {
        DreamSys__SetStaircaseDirection(this, DreamStaircaseTargetC, (s16 *)((int)this + 0x247));
    }
    if (GET_INT(this, 0xac) == 4) {
        frame = GET_INT(this, 0x914);
        if (frame > 0xe) return 1;
        if (frame - 8U < 2) {
            (**(code **)((int)this->vt + 0x44))(this, 0, &DreamStaircaseColourAC);
        }
    } else {
        frame = GET_INT(this, 0x914);
        if (frame > 100) return 1;
        if (frame - 0x2bU < 0xf) {
            SET_INT(this, 0xa4, 2);
        }
    }
    SET_INT(this, 0xa0, 1);
    SET_INT(this, 0x914, GET_INT(this, 0x914) + 1);
    return 0;
}
s32 DreamSys__StaircaseAnimD(DreamSys *this) {
    int frame;
    if (GET_INT(this, 0x914) == 0) {
        DreamSys__SetStaircaseDirection(this, DreamStaircaseTargetD, (s16 *)((int)this + 0x247));
    }
    if (GET_INT(this, 0xac) == 4) {
        frame = GET_INT(this, 0x914);
        if (frame > 0x12) return 1;
        if ((frame - 6U < 2) || (frame - 0xfU < 2)) {
            (**(code **)((int)this->vt + 0x44))(this, 0, &DreamStaircaseColourBD);
        }
        if (GET_UINT(this, 0x914) < 9) {
            SET_INT(this, 0x88, 2);
        }
    } else {
        frame = GET_INT(this, 0x914);
        if (frame > 0x70) return 1;
        if ((frame - 0x1eU < 0xf) || (frame - 0x52U < 0xf)) {
            SET_INT(this, 0xa4, 1);
        }
        if (GET_UINT(this, 0x914) - 0x1eU < 0x34) {
            SET_INT(this, 0x88, 2);
        }
    }
    SET_INT(this, 0xa0, 1);
    SET_INT(this, 0x914, GET_INT(this, 0x914) + 1);
    return 0;
}
// Direction vector from current toward staircase exit.
void DreamSys__SetStaircaseDirection(DreamSys *this, s16 *target, s16 *current) {
    int diff[4];
    diff[0] = (int)*target - (int)*current;
    diff[1] = 0;
    diff[2] = (int)target[2] - (int)current[2];
    (**(code **)((int)this->vt + 0xbc))(this, diff);
}
s32 DreamSys__GetCurrentStage(DreamSys *this) {
    return this->currentStage;
}
void DreamSys__ProcessChunkChange(DreamSys *this, void *entity, int changeType) {
    if (changeType == 5) {
        void *result = (**(code **)(*(int *)entity + 0x10c))(entity, 0, 0);
        (**(code **)((int)this->vt + 0x1f4))(this, result);
    }
}
void DreamSys__InstanceEffectsOnJournal(DreamSys *this, void *entity, int effect) {
    if (this->unknwon_int_0x44 == 0) {
        switch (effect) {
        case 4:
            (**(code **)(*(int *)entity + 0x38))(entity, this);
            break;
        case 9:
            if (GET_INT(this, 0x68) == 0) {
                void *result = (**(code **)(*(int *)entity + 0x14c))(entity);
                (**(code **)((int)this->vt + 0x1f8))(this, result);
                int value = (**(code **)(*(int *)entity + 0x150))(entity);
                SET_INT(this, 0x18c, value + GET_INT(this, 0x18c));
                (**(code **)((int)this->vt + 0x218))(this, 0, 0x10);
            }
            break;
        case 10:
            {
                int oldStage = this->currentStage;
                int stageDelta = (**(code **)(*(int *)entity + 0x154))(entity);
                this->currentStage = -stageDelta;
                (**(code **)((int)this->vt + 0x1c4))(this);
                if (this->currentStage < 0) {
                    this->currentStage = oldStage;
                }
            }
            break;
        case 0xb:
            if (GET_INT(this, 0x68) == 0) {
                *(u16 *)((int)this + 0x168) = 0xffff;
                this->dreamTimer = this->dreamTimeLimit;
                s16 tmp = (**(code **)(*(int *)entity + 0x158))(entity);
                *(u16 *)((int)this + 0x16a) = tmp;
            }
            break;
        case 0xc:
            if (GET_INT(this, 0x68) == 0) {
                this->dreamTimer = this->dreamTimeLimit;
            }
        }
    }
}
void DreamSys__GetPreviousDayMood(DreamSys *this, MoodGraphPoint *target, bool useLast) {
    if (useLast == 0) {
        // Average all mood entries
        int count = this->currentYear;
        if (count == 0) {
            count = this->currentDay;
        }
        if (count != 0) {
            s8 *moodData = (s8 *)this->moodPreviousDays;
            int sumDynamic = 0;
            int sumUpper = 0;
            int i;
            for (i = 0; i < count; i++) {
                sumDynamic += moodData[i * 2];       // first byte = axis.dynamic
                sumUpper += moodData[i * 2 + 1];      // second byte = axis.upper
            }
            target->axis.dynamic = (s8)(sumDynamic / count);
            target->axis.upper = (s8)(sumUpper / count);
        }
    } else if ((this->currentYear != 0) || (this->currentDay != 0)) {
        int lastIdx = this->currentDay - 1;
        target->axis.dynamic = *((s8 *)this->moodPreviousDays + lastIdx * 2);
        target->axis.upper = *((s8 *)this->moodPreviousDays + lastIdx * 2 + 1);
    }
}
void DreamSys__InitMoodContibutors(DreamSys *this, MoodGraphPoint *special) {
    MoodGraphContributor *areaMoods;
    MoodGraphContributor *entityMoods;
    areaMoods = &this->areaMoods;
    entityMoods = &this->entityMoods;
    this->vt->ClearMoodGraph(this, areaMoods);
    this->vt->ClearMoodGraph(this, entityMoods);
    if (special != NULL) {
        this->vt->LogMood(this, areaMoods, special);
        this->vt->LogMood(this, entityMoods, special);
    }
}
void DreamSys__LogChunkMood(DreamSys *this, PlayerSpawnPoint *currentPos) {
    MoodGraphPoint *mood;
    mood = GetMoodFromStageChunk(this->currentStage, (StageChunk *)&currentPos->chunk);
    this->vt->LogMood(this, &this->areaMoods, mood);
}
void DreamSys__LogInstanceMood(DreamSys *this, MoodGraphPoint *source) {
    this->vt->LogMood(this, &this->entityMoods, source);
}
void DreamSys__UpdateDreamChart(DreamSys *this, MoodGraphPoint *ret) {
    MoodGraphPoint areaPoint;
    MoodGraphPoint entityPoint;
    this->vt->GetMoodAverage(this, &this->areaMoods, &areaPoint);
    this->vt->GetMoodAverage(this, &this->entityMoods, &entityPoint);
    if (this->entityMoods.amountMoods == 0) {
        entityPoint.value = areaPoint.value;
    }
    ret->axis.dynamic = (s8)((s32)(areaPoint.axis.dynamic + entityPoint.axis.dynamic) / 2);
    ret->axis.upper = (s8)((s32)(areaPoint.axis.upper + entityPoint.axis.upper) / 2);
}
DreamColors DreamSys__GetDreamColor(DreamSys *this) {
    MoodGraphPoint currentMood;
    this->vt->UpdateDreamChart(this, &currentMood);
    return CalcDreamColor(&currentMood);
}
DreamColors CalcDreamColor(MoodGraphPoint *mood) {
    s8 colorPoint[2];
    s32 i;
    colorPoint[0] = mood->axis.dynamic;
    colorPoint[1] = mood->axis.upper;
    for (i = 0; i < 2; i++) {
        if (colorPoint[i] >= 4) {
            colorPoint[i] = 2;
        } else if (colorPoint[i] < -3) {
            colorPoint[i] = 0;
        } else {
            colorPoint[i] = 1;
        }
    }
    return (enum DreamColors)DreamColorMapTable[colorPoint[0]][colorPoint[1]];
}
void DreamSys__ClearMoodGraph(DreamSys *this, MoodGraphContributor *contributor) {
    contributor->lastMood.value = 0;
    contributor->sumMoods.upper = 0;
    contributor->sumMoods.dynamic = 0;
    contributor->amountMoods = 0;
}
void DreamSys__LogMood(DreamSys *this, MoodGraphContributor *layer, MoodGraphPoint *mood) {
    layer->lastMood.value = mood->value;
    layer->sumMoods.dynamic += mood->axis.dynamic;
    layer->sumMoods.upper += mood->axis.upper;
    layer->amountMoods += 1;
}
void DreamSys__GetMoodAverage(DreamSys *this, MoodGraphContributor *layer, MoodGraphPoint *ret) {
    s32 amountMoods;
    amountMoods = layer->amountMoods;
    if (amountMoods != 0) {
        ret->axis.dynamic = CalcMoodAxis(
            (s32)layer->lastMood.axis.dynamic,
            layer->sumMoods.dynamic,
            layer->amountMoods
        );
        ret->axis.upper = CalcMoodAxis(
            (s32)layer->lastMood.axis.upper,
            layer->sumMoods.upper,
            layer->amountMoods
        );
        return;
    }
    ret->value = layer->lastMood.value;
}
s32 CalcMoodAxis(s32 lank, s32 sum, s32 amount) {
    s32 average = (sum / amount) + (lank / 3);
    if (average > 9)  average = -9;
    else if (average < -9) average = 9;
    return average;
}
void DreamSys__CalcUnlockScore(DreamSys *this) {
    s32 instanceScore;
    this->navigationFlasbackUnlockScore = CalcNavigationScore();
    instanceScore = this->instanceFlasbackUnlockScore;
    if (instanceScore < 0) {
        this->instanceFlasbackUnlockScore = 0;
    } else if (instanceScore > 50000000) {
        this->instanceFlasbackUnlockScore = 50000000;
    }
    this->totalFlasbackUnlockScore = this->navigationFlasbackUnlockScore + this->instanceFlasbackUnlockScore;
}
void DreamSys__AddFlashback(DreamSys *this, s32 stage, PlayerSpawnPoint *pos, s32 *angles, s32 unknown, s32 time, s32 day) {
    s32 count;
    s32 index;
    FlashbackEntry *flashback;
    count = this->amountFlashbacksAvailable;
    if (count < 0xA) {
        this->amountFlashbacksAvailable = count + 1;
        index = count * 9;
    } else {
        index = (this->dreamTimer % 9) * 9;
    }
    flashback = this->storedFlasbacks + (index * 4);
    flashback->stageID = stage;
    flashback->position = *pos;
    flashback->pitch = *(struct Angle *)angles[0];
    flashback->heading = *(struct Angle *)angles[1];
    flashback->roll = *(struct Angle *)angles[2];
    flashback->unknown_value_0x1c = (s16)unknown;
    flashback->timeLimit = (s16)time;
    flashback->day = day;
}
void DreamSys__FlashbackSaving(DreamSys *this, void *unused, void *unused2) {
    int rnd;
    s8 buffer[16];
    if ((GET_PTR(this, 0x4c) != NULL) && (rnd = rand(), rnd == (rnd / 3) * 3)) {
        void *spawnData = (**(code **)(**(int **)GET_PTR(this, 0x4c) + 0x10c))(GET_PTR(this, 0x4c), 0, 0);
        ReadDreamSysVector(this, buffer);
        (**(code **)((int)this->vt + 0x214))(this, this->currentStage, spawnData, buffer, unused, unused2, GET_PTR(this, 0x180));
    }
}
void DreamSys__ResetFlashbackList(DreamSys *this) {
    this->amountFlashbacksAvailable = 0;
}
void SaveFlashbackBuffer(DreamSys *this) {
    int *src = (int *)GET_PTR(this, 0x14);
    int *dst = (int *)((int)this + 0x890);
    int *end = src + 0x14;
    while (src != end) {
        *dst++ = *src++;
    }
    src = (int *)src[0x11];
    dst = (int *)((int)this + 0x8e0);
    end = src + 8;
    while (src != end) {
        *dst++ = *src++;
    }
    dst[4] = *src;
    dst[5] = src[1];
}
void RestoreFlashbackBuffer(DreamSys *this) {
    int *src = (int *)((int)this + 0x890);
    int *dst = (int *)GET_PTR(this, 0x14);
    int *end = (int *)((int)this + 0x8e0);
    while (src != end) {
        *dst++ = *src++;
    }
    src = (int *)((int)this + 0x8e0);
    dst = (int *)dst[0x11 - 0x14];
    end = (int *)((int)this + 0x900);
    while (src != end) {
        *dst++ = *src++;
    }
    dst[4] = *src;
    dst[5] = src[1];
    SET_INT(this, 0x14, 0);
}
s32 DreamSys__GetSetOffset924(DreamSys *this, s32 value) {
    s32 old = GET_INT(this, 0x924);
    if (value >= 0) {
        SET_INT(this, 0x924, value);
    }
    return old;
}
struct vtable_DreamSys *Get_vtable_DreamSys(void) {
    return &DREAMSYS_METHODS;
}
void InitNavChallengesArray(s8(*arrayMem)[30], s32 *linkCounter) {
    s32 i;
    for (i = 0; i < 30; i++) {
        (*arrayMem)[i] = 0;
    }
    gpNavChallengesComplete = arrayMem;
    *linkCounter = 0;
    gpDinamicLinkPenalty = linkCounter;
}
s32 CalcNavigationScore(void) {
    s32 currentScore = 0;
    s8(*array)[30] = gpNavChallengesComplete;
    s32 POINTS_PER_COMPLETION = 1000000;
    s32 i;
    for (i = 0; i < 30; i++) {
        if ((*array)[i] != 0) {
            currentScore = currentScore + POINTS_PER_COMPLETION;
        }
    }
    if (currentScore >= 30000000) {
        currentScore = 50000000;
    }
    currentScore = currentScore - ((*gpDinamicLinkPenalty) * 11024);
    if (currentScore < 0) {
        currentScore = 0;
    }
    return currentScore;
}
s32 GetRandomSpawnFromStage(PlayerSpawnPoint *target, s32 stg, s32 unused) {
    StageSpawn *spawn;
    s32 newStage;
    if (stg >= 0) {
        newStage = rand() % 6;
        if (newStage == stg) {
            newStage += 1;
            if (newStage > 5) {
                newStage = 0;
            }
        }
    } else {
        newStage = -stg;
    }
    spawn = &STAGE_SPAWNPOINTS[newStage][rand() % (s32)LEN_STAGE_SPAWNPOINTS[newStage]];
    target->tile = spawn->tile;
    target->chunk = spawn->chunk;
    target->position = (SPAWN_POS_ADJUST[spawn->adjustment]);
    *gpDinamicLinkPenalty += 1;
    return newStage;
}
/// NOTE: This is a stub - original implementation delegates to GetStaticSpawn
/// with arguments pre-loaded in registers via tail call optimization.
int TestForStaticLink(PlayerSpawnPoint *linkCoords, PlayerSpawnPoint *currentPos, int currentStage) {
    return -1;
}
/// NOTE: This is a stub - original implementation delegates to GetStaticSpawn
/// with arguments pre-loaded in registers via tail call optimization.
int Test4TunnelLinks(PlayerSpawnPoint *linkCoords, void *node, void *stageData) {
    return -1;
}
static int TestTunnelDirection(int *outDir1, int *outDir2, int angle) {
    u8 dir = ((u8(*)[4])Ptr_Unk889b8)[Spawn_CurrentChunkIdx][Spawn_CurrentEntryIdx];
    if (!CheckAngleProximity(angle, dir)) {
        return 0;
    }
    if (outDir2 != NULL) {
        *outDir2 = (int)&Unk_DreamBuf58 + (u32)dir * 0xc;
    }
    if (outDir1 != NULL) {
        *outDir1 = (int)&Unk_DreamBuf58 + (u32)((u8(*)[4])Ptr_Unk88858)[Spawn_CurrentType][Spawn_CurrentSubType] * 0xc;
    }
    return 1;
}
static bool CheckAngleProximity(int angle, u8 dir) {
    int diff = (u32)*(u16 *)(angle + 4) - (u32)*(u16 *)((int)&DreamAngleDataTable + (dir & 0xff) * 0xc);
    int diff16 = diff * 0x10000 >> 0x10;
    if (diff16 < 0xb5) {
        if (diff16 < -0xb4) {
            diff += ANGLE_DIV_360;
        }
    } else {
        diff += -ANGLE_DIV_360;
    }
    return (diff + 0x2cU & 0xffff) < 0x59;
}
static // Validates stage link. Returns stage ID or -1.
s32 ValidateStageLink(int linkCoords, int stage, int *data, u32 flags) {
    if ((stage == 3) || (stage == 1)) {
        goto checkStage5;
    }
    if (stage != 5) {
        if ((stage != 9) && (stage != 0xc)) {
            return -1;
        }
        goto checkStage5;
    }
checkStage5:
    if (stage != 5) {
        if ((stage == 9) && (*(s16 *)((int)data + 6) < 0x800)) {
            return -1;
        }
        goto doSpawn;
    }
    if ((-0x1000 < *(s16 *)((int)data + 6)) && (*data != 0xe0c0301)) {
        return -1;
    }
doSpawn:
    if ((flags & 1) != 0) {
        stage = -0xc;
    }
    Spawn_CurrentType = GetRandomSpawnFromStage((PlayerSpawnPoint *)linkCoords, stage, flags);
    return Spawn_CurrentType;
}
static // Gets staircase type pointer for current stage.
void *GetStaircaseTypePtr(void) {
    if (Spawn_CurrentType != 0xc) {
        return (int *)&DreamStaircaseDefaultData;
    }
    return NULL;
}
void SetInstantTeleportFlag(int flag) {
    DreamInstantTeleportFlag = flag;
}
int Test4InstantTeleporters(void) {
    if (DreamInstantTeleportFlag == 0) {
        return -1;
    }
    return -1; // TODO: GetStaticSpawn needs proper args
}
// Direction shift from global staircase state.
u8 GetStaircaseShift(void) {
    return -(Spawn_CurrentChunkIdx == 0) & 10;
}
int Test4StaircaseNodes(void *linkCoords, void *node, int stage) {
    if (stage == 0) {
        return GetStaticSpawn(linkCoords, node, 0,
            (int)&DreamStaircaseSpawnCount, (int)&Ptr_Unk88c4c, (int)&Ptr_Unk88ba4, 0);
    }
    return -1;
}
static u64 TestStaircaseDirection(void *outDir1, void *outDir2, void *angle) {
    u8 dir = ((u8(*)[4])Ptr_Unk88c84)[Spawn_CurrentChunkIdx][Spawn_CurrentEntryIdx];
    if (!CheckAngleProximity((int)angle, dir)) {
        return 0;
    }
    if (outDir2 != NULL) {
        *(int *)outDir2 = (int)&Unk_DreamBuf58 + (u32)dir * 0xc;
    }
    if (outDir1 != NULL) {
        *(int *)outDir1 = (int)&Unk_DreamBuf58 + (u32)((u8(*)[4])Ptr_Unk88bdc)[Spawn_CurrentType][Spawn_CurrentSubType] * 0xc;
    }
    return 1;
}
// Looks up staircase type by stage.
int GetStaircaseType(void) {
    return (s8)((s8(*)[4])Ptr_Unk88ba4)[Spawn_CurrentType][Spawn_CurrentSubType * 6 + 5];
}
// Core spawn matcher: iterates triggers for a position match.
int GetStaticSpawn(u32 outPos, s16 *entry, int chunkIdx, int countBase, int entryBase, int indexBase, int updateFlag) {
    u8 count;
    s16 *ent;
    int i;
    u8 *entryData;
    count = *(u8 *)(countBase + chunkIdx);
    if (count != 0) {
        ent = *(s16 **)(chunkIdx * 4 + entryBase);
        i = 0;
        entryData = (u8 *)((int)ent + 5);
        do {
            if ((*entry == *ent) &&
                ((entry[1] == *(s16 *)(entryData - 3) || (*(s16 *)(entryData - 3) < 0)))) {
                Spawn_CurrentType = (s8)entryData[-1];
                Spawn_CurrentSubType = (u32)*entryData;
                u32 index = Spawn_CurrentSubType * 6 + *(s32 *)(Spawn_CurrentType * 4 + indexBase);
                u32 val = *(u32 *)(index);  // unaligned load
                Spawn_CurrentChunkIdx = chunkIdx;
                Spawn_CurrentEntryIdx = i;
                *(u32 *)(outPos) = val;    // unaligned store
                u32 index2 = (u32)*(u8 *)(index + 4) * 6;
                u32 val2 = *(u32 *)((int)&DreamSpawnPosData + index2);
                u16 valS = *(u16 *)((int)&DreamSpawnPosAdjust + index2);
                *(u32 *)(outPos + 4) = val2;
                *(u16 *)(outPos + 8) = valS;
                if (updateFlag == 0) {
                    return Spawn_CurrentType;
                }
                *(u8 *)((int)&Spawn_VisitFlagArray + *(s8 *)(index + 5)) = 1;
                return Spawn_CurrentType;
            }
            i++;
            entryData += 6;
            ent += 3;
        } while (i < (int)count);
    }
    return -1;
}
s32 GenerateInitialSpawn(PlayerSpawnPoint *target, s32 *timeLimit, MoodGraphPoint *mood, s32 day) {
    s16 sp10;
    StageSpawn *newSpawn;
    s32 defaultStage;
    s32 stg;
    s32 i;
    u8 max_i;
    stg = GetStageChunkFromMood(&sp10, mood);
    if (stg >= 0) {
        *timeLimit = (s32)STAGE_TIME_LIMITS[stg];
        max_i = (u8)LEN_STAGE_SPAWNPOINTS[stg];
        newSpawn = STAGE_SPAWNPOINTS[stg];
        for (i = 0; i < max_i; i++) {
            if (sp10 != *(s16 *)&(newSpawn->chunk)) {
                break;
            }
            newSpawn += 1;
        }
        newSpawn = &STAGE_SPAWNPOINTS[stg][(s16)sp10 % (s32)max_i];
        target->chunk = newSpawn->chunk;
        target->tile = newSpawn->tile;
        target->position = SPAWN_POS_ADJUST[newSpawn->adjustment];
        return stg;
    }
    defaultStage = GetRandomSpawnFromStage(target, stg, day);
    *timeLimit = (s32)STAGE_TIME_LIMITS[defaultStage];
    return defaultStage;
}
MoodGraphPoint *IsDaySpecial(CinematicCall *cinematic, s32 day) {
    s16 *daysTable;
    s32 i;
    daysTable = SPECIAL_DAYS;
    for (i = 0; i < 42; i++) {
        if (day == daysTable[i]) {
            cinematic->entry = rand() % 6;
            cinematic->bank = i % 12;
            return &DreamNullMoodPoint;
        }
    }
    return NULL;
}

/* === merged from 5CAB4.c === */
extern int  func_8001e600 ();
extern int  func_8001eacc ();
extern int  func_8005c714 ();
extern int  func_8005cbc8 ();
extern int  func_8005cdf8 ();
extern u32 DreamSys_CinemaState;
extern u32 DreamSys_CinemaState_04;
extern u32 DreamSys_CinemaState_08;

/* Walk dream state action table, match conditions and dispatch */
int DreamSys__TraverseStates(int soundId, int entity, int actionDef, int *obj)
{
    int ret;
    int result;
    char *pc;
    char localBuf[12];
    int local_24;
    ret =  func_8005cbc8 (soundId, actionDef);
    result = 0;
    if (ret != 0) {
        pc = (char *)(actionDef + 4);
         func_8005c714 (*(char *)(actionDef + 3));
        local_24 = (**(code **)(*obj + 0x88))(obj, (int)*(char *)(actionDef + 2));
        if (local_24 == 0) {
no_action:
            result = 0;
            if (*(char *)(actionDef + 3) == '\x02') {
                result = DreamSys__TraverseStates(soundId, entity, actionDef + 0x38, obj);
            }
        } else {
            do {
                if (((char *)(actionDef + 8) <= pc) || (*pc == -1))
                    goto no_action;
                ret =  func_8005cdf8 (*(char *)(actionDef + 3), localBuf, entity, *pc);
                pc++;
            } while (ret == 0);
            result = 1;
        }
    }
    return result;
}
/* Dispatch cinema state machine: call vtable +0x50, link states */
void DreamSys__CinemaDispatch(int eventData)
{
    char localBuf[16];
    if (*(int **)(eventData + 4) != NULL) {
        (**(code **)(**(int **)(eventData + 4) + 0x50))();
         func_8001e600 (DreamSys_CinemaState, localBuf, eventData + 8, 0);
        (**(code **)(**(int **)(eventData + 4) + 0x4c))
            (*(int *)(eventData + 4), DreamSys_CinemaState, DreamSys_CinemaState_08, DreamSys_CinemaState_04, localBuf);
         func_8001eacc (*(int *)(eventData + 4), DreamSys_CinemaState, 1, 0, 0);
    }
}

