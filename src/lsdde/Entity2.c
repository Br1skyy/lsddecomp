#include "common.h"
#ifndef ENTITY_VTABLE
#define ENTITY_VTABLE 0
#endif
#include "DreamSys.h"
#include "LocationGrid.h"
#include "dat_globals.h"
#include "lsdde/structs.h"
/* Forward declarations for functions used before defined */
void **Init_DreamParticleSystem_Phase2(void);
void  *Init_DreamParticleSystem_Phase3(void);
int   *Init_ParticleSpawn_Default(int *list, int count, int *tickPtr);
int   *Init_ParticleSpawn_Extra(int *out, int count, int *tickPtr);
int   *Init_ParticleSpawn_Type0(int *out, int *tickPtr);
int   *Init_ParticleSpawn_Type2(int *out, int *tickPtr);
int   *Spawn_SecondaryEntity(int *out, int *pos, int *pos3, int *state);
void  *Find_SecondaryEntitySlot(int *distOut, int *idxOut, int *pos3);
void   Spawn_ParticleRandomVelocity(int tickPtr, int velY);
void   Spawn_ParticleFall(int tickPtr, int colour);
int   *entity_VisibleRender(int *entity);
/* Extern data not in dat_globals.h */
extern u32 SubType2StateArray;
extern u32 Ptr_SecondEntity;
extern u32 ParticleModeIndexTable;
extern u32 Ptr_DreamParticle;
extern u32 ParticlePaletteDefault;
extern u32 ParticlePaletteAlt;
extern u32 ParticleEffectObjectType;
extern u32 ParticleEffectParam2;
extern u32 MusicVolLineTargetLocation2;
extern u32 MusicVolLineTargetLocation3;
extern u32 Unk_EntityBufC8;
extern u32 ParticleFixedVelocity;
extern u32 Unk_ParticleBuf1C;
extern u32 ParticleGroundVelocity3;
extern u32 ParticleColourTable;
extern u32 Unk_ParticleBuf28;
extern u32 PTR_LocationSecondaryEntityTablePtr;
/* -----------------------------------------------------------------------
   Entity2.c  -  LSD: Dream Emulator entity/particle/effect subsystem
   ----------------------------------------------------------------------- */
/* -----------------------------------------------------------------------
   73 functions across 2,810 lines of decompiler output.
   Vtable dispatch: (*(code*)(vtable + offset))(obj, ...) calls virtual
   method at vtable[offset/4].
   ----------------------------------------------------------------------- */
/***********************************************************************
 * VTABLE GETTERS
 **********************************************************************/
void *Get_vtable_Entity2(void)
{
    return &Vtable_Entity2;
}
void *Get_vtable_Entity3(void)
{
    return &Vtable_Entity3;
}
void *Get_vtable_Entity4(void)
{
    return &Vtable_Entity4;
}
void *Get_vtable_DreamParticle(void)
{
    return &Vtable_DreamParticle;
}
void *Get_vtable_EntitySubObject(void)
{
    return &GameManagerPtr;
}
void *Get_vtable_EntitySubObject2(void)
{
    return &Vtable_EntitySubObject2;
}
void *Get_vtable_DreamSymbol(void)
{
    return &Vtable_DreamSymbol;
}
/***********************************************************************
 * ALLOCATORS / CONSTRUCTORS
 ***********************************************************************/
/* Allocate 0x54-byte entity, return obj ptr or 0 */
int New_Entity2(int type, int arg)
{
    int obj;
    int vtable;
    obj = (int)MemAlloc(SIZEOF_ENTITY_TYPE0);
    vtable = 0;
    if (obj != 0) {
        vtable = (int)Get_vtable_Entity3();
        (*(code*)(vtable + 8))(obj, type, arg); /* vtable[2] */
        vtable = obj;
    }
    return vtable;
}
/* Allocate 0x88-byte entity (5 ctor params) */
int New_Entity_Big(int type, int arg1, int arg2, int arg3, int arg4)
{
    int obj;
    int vtable;
    obj = (int)MemAlloc(SIZEOF_ENTITY_TYPE1);
    vtable = 0;
    if (obj != 0) {
        vtable = (int)Get_vtable_Entity4();
        (*(code*)(vtable + 8))(obj, type, arg1, arg2, arg3, arg4); /* vtable[2] */
        vtable = obj;
    }
    return vtable;
}
/* Allocate 0x98-byte particle object, return obj or 0 */
int New_DreamParticleObject(int type, int *velTbl, int sysPtr, int tickPtr)
{
    int obj;
    int vtable;
    int result;
    obj = (int)MemAlloc(SIZEOF_ENTITY_TYPE2);
    if (obj != 0) {
        vtable = (int)Get_vtable_DreamParticle();
        result = (*(code*)(vtable + 8))(obj, type, velTbl, sysPtr, tickPtr); /* vtable[2] */
        if (result == 0) {
            MemFree(obj);
            return 0;
        }
    }
    return obj;
}
/* Allocate 0x58-byte sub-object */
int New_EntitySubObject(void)
{
    int obj;
    int vtable;
    obj = (int)MemAlloc(SIZEOF_SUBOBJECT);
    if (obj != 0) {
        vtable = (int)Get_vtable_EntitySubObject();
        if ((*(code*)(vtable + 8))(obj) == 0) { /* vtable[2] */
            MemFree(obj);
            obj = 0;
        }
    }
    return obj;
}
/* Allocate 0xa8-byte sub-object 2 */
int New_EntitySubObject2(int arg1, int arg2, int allocator)
{
    int obj;
    int vtable;
    obj = (int)MemAlloc(SIZEOF_SUBOBJECT2);
    if (obj != 0) {
        vtable = (int)Get_vtable_EntitySubObject2();
        (*(code*)(vtable + 8))(obj, arg1, arg2, allocator); /* vtable[2] */
    }
    return obj;
}
/***********************************************************************
 * FIELD / STATE HELPERS
 ***********************************************************************/
/* Zero entity fields at +0x34, +0x38, +0x50 */
void Clear_EntityFields_34_50(int *entity)
{
    *(int*)(entity + 0x34) = 0;
    *(int*)(entity + 0x38) = 0;
    *(int*)(entity + 0x50) = 0;
}
/* Set entity fields at +0x20/0x24/0x28; if trigger set, fire vtable cb */
int *entity_SetFields_20_28(int *entity, int val1, int val2, int val3, int trigger)
{
    int *piVar1;
    *(int*)(entity + 0x20) = val1;
    *(int*)(entity + 0x24) = val2;
    *(int*)(entity + 0x28) = val3;
    if (trigger != 0) {
        piVar1 = *(int**)((val3 - val1) * 4 + entity + 0x40);
        (*(code*)(*piVar1 + 0xb8))(piVar1, &EntityFieldSetupData); /* vtable[0x2e] */
    }
}
/* Copy a name from a string table into 0x1a-byte buffer with space padding */
u8 *Entity_GetPaddedName(u8 *dst, u8 *src, int nameBase, int nameTable)
{
    int len;
    int idx;
    u8 *p;
    idx = (nameBase + nameTable) * 4;
    len = strlen((char*)(nameTable + *(int*)(idx + *(int*)(src + 0x18))));
    if (26 < len) { len = 26; }
    memcpy(dst, (u8*)(nameTable + *(int*)(idx + *(int*)(src + 0x18))), len);
    if (len < 26) {
        p = dst + len;
        do { *p = ' '; p = p + 1; } while ((int)p < (int)(dst + 26));
    }
    dst[26] = '\0';
    return dst;
}
/* 3-element vector add: dst = a + b */
void AddVec3(int *dst, int *a, int *b)
{
    *dst = *a + *b;
    dst[1] = a[1] + b[1];
    dst[2] = a[2] + b[2];
}
/* Set entity render state via vtable methods */
void Setup_EntityState(int *entity, int state, int *pos, int colour, int other)
{
    (*(code*)(entity + 0x4c))(); /* vtable[0x13] */
    (*(code*)(entity + 0x44))(entity, 1, colour); /* vtable[0x11] */
    (*(code*)(entity + 0x48))(entity, 1, other); /* vtable[0x12] */
}
/* Release an array of objects by calling vtable[4] on each */
void ReleaseObjectArray(int *array, int count)
{
    for (; 0 < count; count = count - 1) {
        *array = (*(code*)(*(int*)*array + 4))(); /* vtable[1] */
        array = array + 1;
    }
}
/***********************************************************************
 * ENTITY UPDATE / DESTROY STATE MACHINES
 ***********************************************************************/
/* Entity state update - destroys secondary entity when needed */
int *entity_Update_DestroyState(int *entity, int *secondaryEnt)
{
    int iVar1;
    int uVar2;
    if (((EntityObj *)entity)->nField_60 != 0) {
        if (secondaryEnt[0x20] == 0) {
            if (secondaryEnt[0xf] != 0) {
                iVar1 = ((EntityObj *)entity)->nField_50;
                if (*(int*)(iVar1 + 0x14) == 2)
                    uVar2 = *(int*)(iVar1 + 0xc);
                else
                    uVar2 = *(int*)(iVar1 + 0x18);
                (*(code*)(*secondaryEnt + 0x7c))(secondaryEnt, uVar2); /* vtable[0x1f] */
                (*(code*)(*secondaryEnt + 4))(secondaryEnt); /* vtable[1] */
                ((EntityObj *)entity)->nField_60 = 0;
                (*(code*)(entity + 0x80))(entity); /* vtable[0x20] */
            }
        } else {
            (*(code*)(*secondaryEnt + 4))(secondaryEnt); /* vtable[1] */
            ((EntityObj *)entity)->nField_60 = 0;
            (*(code*)(entity + 0x80))(entity); /* vtable[0x20] */
            iVar1 = (*(code*)(*(int*)entity[0xf] + 0x108))(); /* vtable[0x42] */
            (*(code*)(*(int*)entity[0xf] + 0x104))((int*)entity[0xf], iVar1 + 0x1e); /* vtable[0x41] */
        }
        if (((EntityObj *)entity)->nField_60 != 0) return;
    }
    if ((*(short*)(((EntityObj *)entity)->pData + 0x1b4) == 0) && (((EntityObj *)entity)->nObjState == 0)) {
        ((EntityObj *)entity)->nField_64 = 1;
        (*(code*)(entity + 0x88))(entity); /* vtable[0x22] */
    }
}
/* Notify entity of position/orientation change */
int *entity_NotifyPosition(int *entity, int posX, int posY, int callbackObj, int trigger)
{
    int *world;
    world = (int*)(*(code*)(*(int*)((EntityObj *)entity)->pWorld + 0xac))(); /* vtable[0x2b] */
    if (callbackObj != 0) (*(code*)(*world + 0xd0))(world, callbackObj); /* vtable[0x34] */
    if (trigger != 0) (*(code*)(entity + 0x10))(entity, world); /* vtable[4] */
    (*(code*)(*world + 0xd8))(world, ((EntityObj *)entity)->nDist, posX, posY); /* vtable[0x36] */
}
/***********************************************************************
 * ENTITY SUBTYPE SPAWN / ANIM / DESTROY
 ***********************************************************************/
/* Create subtype-0 sub-objects (0..2 children from +0x6c count setting) */
void Create_EntitySubType0(int *entity, int recreateFlag)
{
    int iVar3, iVar4;
    int *puVar2;
    int local_30, local_2c, local_28;
    iVar4 = *(int*)(entity + 0x6c);
    if (iVar4 != 0) {
        local_30 = 0; local_2c = 0; local_28 = 0;
        puVar2 = (int*)(entity + 0x7c);
        iVar3 = 0;
        do {
            if (iVar4 < 3)
                local_30 = (int)*(short*)(entity + 0x68) * *(int*)(&SubType0OffsetTable + iVar4 * 4) + local_30;
            else
                local_2c = local_2c + *(int*)(&SubType0OffsetTable + iVar4 * 4);
            if (recreateFlag == 0) {
                *puVar2 = New_EntitySubObject();
                (*(code*)(*(int*)*puVar2 + 0x4c))((int*)*puVar2, entity, 0); /* vtable[0x13] */
                (*(code*)(*(int*)*puVar2 + 0xb8))((int*)*puVar2, *(int*)(entity + 0x74)); /* vtable[0x2e] */
            } else {
                (*(code*)(*(int*)*puVar2 + 0xb8))((int*)*puVar2, &local_30); /* vtable[0x2e] */
            }
            iVar3 = iVar3 + 1;
            puVar2 = puVar2 + 1;
        } while (iVar3 < 2);
    }
}
/* Animate subtype-0 - periodic child-object swap when timer ((EntityObj *)entity)->nTimer > 500 */
void Anim_EntitySubType0(int *entity)
{
    int iVar3, iVar4, iVar5;
    u32 uVar6;
    int local_30, local_2c, local_28;
    iVar5 = ((EntityObj *)entity)->nChildCount;
    if (((((EntityObj *)entity)->nSubTypeCount != 0) && (*(int*)(&SubType0AnimFrameTable + iVar5 * 4) != 0)) && (500 < (u32)((EntityObj *)entity)->nTimer)) {
        iVar4 = 0; iVar3 = 0;
        (*(code*)(entity + 0x44))(entity, 0, &SubType0ColourSetting); /* vtable[0x11] */
    do {
        local_30 = 0; local_2c = 0;
        local_28 = iVar3 + *(int*)(&SubType0AnimFrameTable + iVar5 * 4);
        iVar3 = iVar3 + 3;
        (*(code*)(*(int*)((EntityObj *)entity)->pSubObj + 0xbc))((int*)((EntityObj *)entity)->pSubObj, &local_30); /* vtable[0x2f] */
        (*(code*)(*(int*)((EntityObj *)entity)->pSubObj + 0x44))((int*)((EntityObj *)entity)->pSubObj, 0, &SubType0ColourSetting); /* vtable[0x11] */
            iVar4 = iVar4 + 1;
        } while (iVar4 < 2);
        uVar6 = PARTICLE_GRAVITY / *(int*)(&SubType0AnimFrameTable + iVar5 * 4);
        if (*(int*)(&SubType0AnimFrameTable + iVar5 * 4) == 0) trap(TRAP_DIVZERO);
        if ((u32)((EntityObj *)entity)->nTimer % uVar6 == 0) Create_EntitySubType0(entity, 1);
    }
    *(int*)((EntityObj *)entity)->pData = 0;
}
/* Destroy subtype-0 child array (2 entries) */
void Destroy_EntitySubType0(int *entity)
{
    if (*(int*)(entity + 0x6c) != 0) ReleaseObjectArray(entity + 0x7c, 2);
}
/* Create sub-type 2 sub-objects (5 children with random colour) */
void Create_EntitySubType2(int *entity, int unused)
{
    int rnd;
    int *colourPtr;
    int *child;
    rnd = rand();
    if (rnd % 2 == 0)
        colourPtr = (int*)&SubType2Colour0;
    else
        colourPtr = 0;
    Spawn_EntitySubObjectPool(entity, 0, 0, colourPtr);
    child = *(int**)(entity + 0x88);
    if (*(int*)(entity + 0x70) < 2) {
        (*(code*)(*child + 100))(child, 1); /* vtable[0x19] */
        (*(code*)(*child + 0x68))(child, 0); /* vtable[0x1a] */
        if (rnd % 2 != 0)
            colourPtr = (int*)&SubType2Colour1;
        else
            colourPtr = (int*)&SubType2Colour2;
        (*(code*)(*child + 0x48))(child, 1, colourPtr); /* vtable[0x12] */
    } else {
        SubType2TempState = *(int*)(&SubType2StateArray + *(int*)(entity + 0x70) * 4);
        SoundOffsetAdd(child, 0);
        rnd = *(int*)(entity + 0x78);
        if (rnd == 0) rnd = *(int*)(entity + 0x74);
        (*(code*)(*child + 0xb8))(child, rnd); /* vtable[0x2e] */
    }
    (*(code*)(**(int**)(entity + 0x8c) + 0x60))(*(int**)(entity + 0x8c), 0); /* vtable[0x18] */
}
/* Allocate 5 sub-objects linked to parent; each 0xa8 bytes */
void Spawn_EntitySubObjectPool(int *entity, int unused1, int unused2, int *colour)
{
    int iVar3;
    int *puVar2;
    puVar2 = (int*)(entity + 0x84);
    iVar3 = 0;
    do {
        *puVar2 = New_EntitySubObject2(unused2, 0, ParticleSpawnAllocator);
        (*(code*)(*(int*)*puVar2 + 0x4c))((int*)*puVar2, entity, 0); /* vtable[0x13] */
        (*(code*)(*(int*)*puVar2 + 0xb8))((int*)*puVar2, *(int*)(entity + 0x74)); /* vtable[0x2e] */
        iVar3 = iVar3 + 1;
        if (colour != 0)
            (*(code*)(*(int*)*puVar2 + 0x48))((int*)*puVar2, 1, colour); /* vtable[0x12] */
        puVar2 = puVar2 + 1;
    } while (iVar3 < 5);
}
/* Stub: subtype 2 animate does nothing */
void Anim_EntitySubType2_Stub(void) { }
/* Stub */
void Destroy_EntitySubType2(void) { }
/* Destroy subtype-2 child array (5 entries) */
void Destroy_EntitySubType2_Pool(int *entity)
{
    ReleaseObjectArray(entity + 0x84, 5);
}
/* Init subtype-3 - delegates to pool spawner (original:  func_8002cc84 ) */
void Init_EntitySubType3(int *entity, int unused)
{
    Spawn_EntitySubObjectPool(entity, 0, 0, 0);
}
/* Init subtype-2 - delegates to pool spawner (original:  func_8002cd08 ) */
void Init_EntitySubType2(int *entity)
{
    Create_EntitySubType2(entity, 0);
}
/* Animate subtype-3 - random colour cycling */
void Anim_EntitySubType3(int *entity)
{
    u32 uVar1;
    int iVar2, iVar4;
    int *piVar3;
    piVar3 = (int*)(entity + 0x88);
    iVar4 = 0;
    do {
        iVar4 = iVar4 + 1;
        uVar1 = rand();
        (*(code*)(*(int*)*piVar3 + 0x48))((int*)*piVar3, 1, (int)&Unk_EntityBuf8C + (uVar1 % 6) * 0xc); /* vtable[0x12] */
        iVar2 = rand();
        *(int*)(*piVar3 + 0x84) = (iVar2 % ANGLE_DIV_360) * FIXED_12;
        piVar3 = piVar3 + 1;
    } while (iVar4 < 4);
}
/* Destroy subtype-3 pool */
void Destroy_EntitySubType3(int *entity)
{
    ReleaseObjectArray(entity + 0x84, 5);
}
/* State dispatcher: entity subtype 0/2/3 init */
void Init_EntityRenderState(int *entity, int state, int *pos)
{
    int iVar2;
    int auStack_20[4];
    ParticleTickTimerValue = *(int*)(ParticleTickTimer + 0x18);
    AddVec3(auStack_20, (int*)pos, (int*)(entity + 0x58));
    Setup_EntityState((int*)entity, state, auStack_20, *(int*)(entity + 100), *(int*)(entity + 0x68));
    iVar2 = *(int*)(entity + 0x54);
    if (iVar2 < 2) {
        int uVar1 = (*(code*)(*(u32 *)ParticleLocationObject + 0x80))(ParticleLocationObject, (&LocationChildObjects)[iVar2]); /* vtable[0x20] */
        LinkObjectToParent(entity, uVar1);
        iVar2 = *(int*)(entity + 0x54);
    }
    if (iVar2 == 2)      Create_EntitySubType2(entity, 0);
    else if (iVar2 == 0) Create_EntitySubType0(entity, 0);
    else if (iVar2 == 3) Init_EntitySubType3(entity, 0);
}
/* Per-frame animation dispatcher */
void Anim_EntityStateUpdate(int *entity, int *pos)
{
    int iVar1;
    int auStack_20[4];
    AddVec3(auStack_20, (int*)pos, entity + 0x16);
    (*(code*)(entity + 0xb8))(entity, auStack_20); /* vtable[0x2e] */
    iVar1 = ((EntityObj *)entity)->nSoundId;
    if (iVar1 == 2)      Anim_EntitySubType2_Stub();
    else if (iVar1 == 0) Anim_EntitySubType0(entity);
    else if (iVar1 == 3) Anim_EntitySubType3(entity);
}
/* Destroy dispatcher */
void Destroy_EntitySubState(int *entity)
{
    int iVar1 = *(int*)(entity + 0x54);
    if (iVar1 == 2)      Destroy_EntitySubType2_Pool(entity);
    else if (iVar1 == 0) Destroy_EntitySubType0(entity);
    else if (iVar1 == 3) Destroy_EntitySubType3(entity);
}
/***** Position/velocity helpers for sub-object 2 *****/
void Apply_EntitySubType2(int obj)
{
    Apply_EntitySubType2_Internal(obj, 0, obj);
}
void Apply_EntitySubType2_Internal(int obj, int mode, int *vec)
{
    int iVar3 = *(int*)(obj + 0x14);
    if (mode == 0) {
        *(int*)(iVar3 + 0x18) = *(int*)(iVar3 + 0x18) + *vec;
        *(int*)(iVar3 + 0x1c) = *(int*)(iVar3 + 0x1c) + vec[1];
        *(int*)(iVar3 + 0x20) = *(int*)(iVar3 + 0x20) + vec[2];
    } else {
        *(int*)(iVar3 + 0x18) = *vec;
        *(int*)(iVar3 + 0x1c) = vec[1];
        *(int*)(iVar3 + 0x20) = vec[2];
    }
    **(int**)(obj + 0x14) = 0;
}
/***********************************************************************
 * VISUAL STATE / RENDER HELPERS
 ***********************************************************************/
/* Set entity transparency + colour */
void SetEntityVisualState(int *entity, short *valueOut, short value, int trigger, int cbArg)
{
    *valueOut = value;
    *(short*)(entity + 0x12) = value;
    (*(code*)(entity + 0xc0))(entity, &EntityVisualColourSet); /* vtable[0x30] */
    *valueOut = 0;
    if (trigger != 0) (*(code*)(entity + 0x88))(entity, cbArg); /* vtable[0x22] */
}
/* Call a render callback; if it sets entity+0x28 == 0, call fallback */
int *entity_RenderOrReject(int *entity, void (*cb)(int, int, int), int arg1, int arg2)
{
    *(int*)(entity + 0x28) = 0;
    (*cb)(entity, arg1, arg2);
    if (*(int*)(entity + 0x28) == 0)
        entity_VisibleRender(entity);
}
/* Chunk/sector tile walk - returns 1 if renderable */
int Entity_ShouldRender(int *entity)
{
    short auStack_68[24];
    int auStack_38[12];
    int auStack_98[12];
    int auStack_28[4];
    int iVar2, iVar3;
    int *piVar4;
    piVar4 = (int*)((EntityObj *)entity)->pObject;
    if (piVar4 != 0) {
        iVar2 = ((EntityObj *)entity)->pData;
        iVar3 = (*(code*)(*piVar4 + 0x110))(piVar4, auStack_98, iVar2 + 0x18); /* vtable[0x44] */
        if (iVar3 == 0) return 0;
        iVar2 = Render_EntityChunkWalk(entity, auStack_68, auStack_38, auStack_98, 1);
        iVar3 = Render_EntityCheckSectors(entity, auStack_28, iVar2 + 0x18, iVar2, auStack_68, auStack_38);
        ((EntityObj *)entity)->nConfig = iVar3;
        if (iVar3 == 0) {
            (*(code*)(entity + 0x88))(entity, 0xfffffffe); /* vtable[0x22] */
            return 0;
        }
        (*(code*)(entity + 0xbc))(entity, auStack_28); /* vtable[0x2f] */
        (*(code*)(entity + 0x88))(entity, 0xffffffff); /* vtable[0x22] */
        return 1;
    }
    return 0;
}
/* Walk chunk data to build tile spans; returns tile-group count */
int Render_EntityChunkWalk(int *entity, short *out, int *tileList, int chunk, u32 flags)
{
    char cVar1, cVar2;
    int iVar3, iVar4, iVar10;
    short *psVar5;
    u32 uVar11;
    int *piVar12;
    cVar1 = *(char*)(chunk + 2);
    cVar2 = *(char*)(chunk + 3);
    if ((flags & 1) == 0) flags = flags + 1;
    if (flags == 1) {
        *out = (short)cVar1;
        out[1] = (short)cVar2;
        out[2] = 1; out[3] = 0; out[4] = 1; out[5] = 0;
        iVar10 = *(int*)(chunk + 0x24);
        *tileList = iVar10;
        piVar12 = *(int**)(entity + 0x4c);
        iVar3 = 1;
        if (*(int*)(piVar12[0x1a] + 4) == 1) {
            iVar10 = (int)*(short*)(*(int*)(iVar10 + 4) + 0x32);
            iVar3 = 1;
            if (iVar10 + 1 < (int)*(short*)(piVar12[0x1a] + 2)) {
                iVar3 = 2;
                iVar4 = (*(code*)(*piVar12 + 0x118))(piVar12); /* vtable[0x46] */
                tileList[1] = iVar4;
                *(int*)(out + 6) = *(int*)out;
                *(int*)(out + 8) = *(int*)(out + 2);
                *(int*)(out + 10) = *(int*)(out + 4);
            }
            if (-1 < iVar10 - 1) {
                iVar10 = (*(code*)(*piVar12 + 0x118))(piVar12); /* vtable[0x46] */
                tileList[iVar3] = iVar10;
                psVar5 = out + iVar3 * 6;
                *(int*)psVar5 = *(int*)out;
                *(int*)(psVar5 + 2) = *(int*)(out + 2);
                *(int*)(psVar5 + 4) = *(int*)(out + 4);
                iVar3 = iVar3 + 1;
            }
        }
    } else {
        if (cVar1 == '\0') { *out = 0; uVar11 = flags - 1; }
        else { *out = cVar1 - 1; uVar11 = flags; }
        if (cVar1 == '\x13') uVar11 = uVar11 - 1;
        if (cVar2 == '\x13') { flags = flags - 1; out[1] = 0x13; }
        else out[1] = cVar2 + 1;
        *(u32*)(out + 2) = uVar11;
        *(u32*)(out + 4) = flags;
        iVar3 = 1;
        *tileList = *(int*)(chunk + 0x24);
    }
    return iVar3;
}
/* Iterate sectors, return first tile that passes render check */
int Render_EntityCheckSectors(int *entity, int *result, int tileCount, int sectorCount, short *sectors, int *tileList)
{
    int iVar1, iVar2;
    iVar2 = 0;
    if (0 < sectorCount) {
        do {
            iVar2 = iVar2 + 1;
            if ((*(short*)(*(int*)(*tileList + 4) + 0x2c) != 0) &&
                (iVar1 = Render_CheckSectorTile(entity, result, tileCount, sectors, *tileList), iVar1 != 0))
                return iVar1;
            sectors = (short*)((int)sectors + 0xc);
            tileList = tileList + 1;
        } while (iVar2 < sectorCount);
    }
    return 0;
}
/* Check a single tile within a sector; walks linked tile list */
int Render_CheckSectorTile(int tile, int *state, int tileCount, short *sector, int tileListBase)
{
    int iVar1, iVar2, iVar4, iVar5;
    int *piVar3;
    iVar5 = 0;
    piVar3 = (int*)(sector[1] * 0x50 + *(int*)(tileListBase + 0x10) + *sector * 4);
    if (0 < *(int*)(sector + 4)) {
        do {
            iVar4 = 0;
            if (0 < *(int*)(sector + 2)) {
                do {
                    iVar1 = CheckTileRenderState(*piVar3, state, tileCount);
                    if (iVar1 != 0) return *piVar3;
                    for (iVar1 = *(int*)(*piVar3 + 0x38); iVar1 != 0; iVar1 = *(int*)(iVar1 + 0x38)) {
                        iVar2 = CheckTileRenderState(iVar1, state, tileCount);
                        if (iVar2 != 0) return iVar1;
                    }
                    iVar4 = iVar4 + 1;
                    piVar3 = piVar3 + 1;
                } while (iVar4 < *(int*)(sector + 2));
            }
            iVar5 = iVar5 + 1;
            piVar3 = (int*)((int)piVar3 - (*(int*)(sector + 2) * 4 + 0x50));
        } while (iVar5 < *(int*)(sector + 4));
    }
    return 0;
}
/* Returns tile ptr if non-null + active, else 0 */
int CheckTileRenderState(int tile)
{
    if ((tile == 0) || (IsTileActive() == 0)) return 0;
    return tile;
}
/***********************************************************************
 * DREAM PARTICLE SYSTEM - GLOBALS
 *
 * ParticleSysEntityPtr  = primary entity/system pointer
 * ParticleLocationIndex  = dream stage index
 * ParticleFrameCounter  = frame counter
 * ParticleEffectSelector  = effect colour/type selector
 * ParticleWorldPtr  = world/instance pointer
 * ParticleMode  = particle mode (-1 = inactive)
 * ParticleModeSubIndex  = sub-index within mode table
 * ParticleActiveCount  = active particle count
 * ParticleGridTier  = particle grid tier (0=off,1=normal,2=alt)
 * ParticleEffectVtable  = selected vtable entry for effect
 * ParticleChunkPalette  = chunk particle palette pointer
 * ParticleExtraChunkPtr  = extra chunk pointer
 * ParticleEffectObject  = effect object
 * ParticleEntityScanIndex  = scan index into entity list
 * ParticleLocationObject  = stage object
 * ParticleSpawnAllocator  = spawn object allocator
 * ParticleTickTimer  = tick timer object
 *
 * Global arrays:
 *   ParticleInstanceArray[...]  = spawned particle instances
 *   ParticleGridEntries[...]  = grid particle entries (0x12 count)
 *   ParticleSecondarySlots[2]    = secondary entity slots
 *   LocationChildObjects[2]    = stage object children
 ***********************************************************************/
/* Phase 1 - set globals and call into phase 2 */
int Init_DreamParticleSystem(int mainPtr, int stage, int worldPtr, int effectIdx, int configParam)
{
    int cnt;
    int *p;
    if (ParticleSysEntityPtr != 0) return 0;
    cnt = 1;
    p = &ParticleInitScratch;
    ParticleMode = 0xffffffff;
    ParticleConfigParam = configParam;
    ParticleFrameCounter = 0;
    ParticleSysEntityPtr = mainPtr;
    ParticleLocationIndex = stage;
    ParticleEffectSelector = effectIdx;
    ParticleWorldPtr = worldPtr;
    do { *p = 0; cnt = cnt - 1; p = p - 1; } while (-1 < cnt);
    return Init_DreamParticleSystem_Phase2();
}
/* Phase 2 - select definition table */
void **Init_DreamParticleSystem_Phase2(void)
{
    int def;
    def = *(int*)(&ParticleLocationDefTable + ParticleLocationIndex * 4);
    if (def == 0) def = Init_DreamParticleSystem_Phase3();
    Init_ParticleVtableTable(&Ptr_SecondEntity, (char*)def);
    if (3 < *(char*)(def + 1))
        ParticleEffectVtable = (int)&Unk_ParticleBufC4 + *(char*)(def + 2) * 3;
    return &Ptr_SecondEntity;
}
/* Populate vtable table from data definition */
void Init_ParticleVtableTable(int *table, char *raw)
{
    *(int*)(table + 0xc) = (int)&Unk_ParticleBufC4 + raw[3] * 3;
    *(int*)(table + 0x18) = (int)&Unk_ParticleBufC4 + raw[2] * 3;
    *(int*)(table + 0x1c) = *(int*)(&ParticleDefVtableArray + raw[1] * 4);
    *(int*)(table + 0x14) = (int)*raw;
}
/* Phase 3 - select record and set mode tier */
void *Init_DreamParticleSystem_Phase3(void)
{
    u32 idx;
    int denom;
    u8 *record;
    idx = ParticleEffectSelector + ParticleLocationIndex;
    ParticleMode = (int)(char)(&ParticleModeIndexTable)[idx & 0xf];
    denom = (int)(char)(&ParticleModeDenomTable)[ParticleMode];
    ParticleModeSubIndex = (int)idx % denom;
    if (denom == 0) trap(TRAP_DIVZERO);
    if ((denom == -1) && (idx == 0x80000000)) trap(TRAP_OVERFLOW);
    record = (&Ptr_DreamParticle)[ParticleMode] + ParticleModeSubIndex * 4;
    if (ParticleMode == 0) {
        ParticleChunkPalette = (int)&ParticlePaletteDefault;
        ParticleExtraChunkPtr = (int)&Unk_ParticleBufC4 + (char)record[3] * 3;
        if (record[2] != 0x12) ParticleChunkPalette = (int)&ParticlePaletteAlt;
        if ((3 < ParticleModeSubIndex) && (ParticleModeSubIndex < 6))
            ParticleGridTier = 2;
        else
            ParticleGridTier = 1;
    } else {
        ParticleGridTier = 0;
    }
    return record;
}
/* Spawn the current particle effect from stored vtable */
void Spawn_DreamParticleEffect(void)
{
    if (ParticleEffectVtable != 0) {
        ParticleEffectObject = (int*)Create_ObjectType1(&ParticleEffectObjectType, ParticleEffectVtable, 0);
        (*(code*)(*(u32 *)ParticleEffectObject + 100))(ParticleEffectObject, 1); /* vtable[0x19] */
        (*(code*)(*(u32 *)ParticleEffectObject + 0x68))(ParticleEffectObject, 0); /* vtable[0x1a] */
        int uVar1 = (*(code*)(**(int**)(ParticleWorldPtr + 0xc) + 0xac))(); /* vtable[0x2b] */
        (*(code*)(*(u32 *)ParticleEffectObject + 0x4c))(ParticleEffectObject, uVar1, &ParticleEffectParam2); /* vtable[0x13] */
    }
}
/* Destroy active effect */
void Destroy_DreamParticleEffect(void)
{
    if (ParticleEffectVtable != 0) {
        (*(code*)(*(u32 *)ParticleEffectObject + 4))(); /* vtable[1] */
        ParticleEffectVtable = 0;
    }
}
/* Init particle environment globals */
void Init_DreamParticleEnv(int mode, int *stageObj, int allocPtr, int tickObj)
{
    int i;
    int *p;
    i = 0;
    p = &LocationChildObjects;
    ParticleLocationObject = stageObj;
    ParticleSpawnAllocator = (int*)allocPtr;
    ParticleTickTimer = tickObj;
    do {
        *p = (int)(*(code*)(*stageObj + 0x80))(stageObj, *p); /* vtable[0x20] */
        SetObjectTransform(*p, &LocationChildTransform);
        i = i + 1;
        p = p + 1;
    } while (i < 2);
}
/***********************************************************************
 * PARTICLE GRID - 0x12 tiles
 ***********************************************************************/
/* Allocate 18 particle entries in a moving grid pattern */
void Alloc_DreamParticles_Grid(void)
{
    int iVar4, iVar5;
    int *piVar1;
    int uVar2;
    int *puVar3;
    int local_28, local_24, local_20, local_1c;
    if (ParticleGridTier != 0) {
        local_28 = -100;
        local_24 = -60;
        if (ParticleGridTier == 2) local_24 = -30;
        local_20 = 0x140;
        local_1c = 0x90;
        iVar5 = 1;
        iVar4 = 3;
        ParticleGridEntries = (int*)Create_ObjectType1(&local_20, ParticleChunkPalette, PARTICLE_FALL_MAX);
        puVar3 = &ParticleGridEntries;
        do {
            puVar3 = puVar3 + 1;
            iVar5 = iVar5 + 1;
            piVar1 = (int*)Create_ObjectType1(&local_20, iVar4 + ParticleChunkPalette, PARTICLE_FALL_MAX);
            *puVar3 = piVar1;
            iVar4 = iVar4 + 3;
            (*(code*)(*piVar1 + 0x4c))(piVar1, ParticleGridEntries, &local_28); /* vtable[0x13] */
            local_24 = local_24 + 3;
            local_1c = local_1c - 7;
        } while (iVar5 < 0x12);
        uVar2 = (*(code*)(**(int**)(ParticleWorldPtr + 0xc) + 0xac))(); /* vtable[0x2b] */
        (*(code*)(*(u32 *)ParticleGridEntries + 0x4c))(ParticleGridEntries, uVar2, &local_28); /* vtable[0x13] */
    }
}
/* Update grid positions from world animation params */
void Update_DreamParticles_Anim(void)
{
    int iVar1, iVar2, iVar4, iVar5;
    int *puVar3;
    int *piVar6;
    int auStack_28[2];
    if (ParticleGridTier != 0) {
        piVar6 = *(int**)(ParticleWorldPtr + 0xc);
        iVar2 = (piVar6[6] - piVar6[9]) / 600;
        iVar1 = iVar2 * 3;
        if (0 < iVar1) {
            int local_20 = -100;
            int local_1c = -60;
            iVar5 = 0;
            if (ParticleGridTier == 2) local_1c = -30;
            puVar3 = &ParticleGridEntries;
            iVar4 = 0;
            local_1c = iVar2 * 9 + local_1c;
            do {
                Calc_ParticleOffset_3((char*)auStack_28, (char*)(iVar4 + ParticleChunkPalette), (char)iVar1);
                (*(code*)(*(int*)*puVar3 + 0xb8))((int*)*puVar3, 1, auStack_28); /* vtable[0x2e] */
                iVar5 = iVar5 + 1;
                iVar4 = iVar4 + 3;
                (*(code*)(*(int*)*puVar3 + 0xbc))((int*)*puVar3, &local_20); /* vtable[0x2f] */
                local_1c = local_1c + 3;
                puVar3 = puVar3 + 1;
            } while (iVar5 < 0x12);
            Calc_ParticleOffset_3((char*)auStack_28, (char*)ParticleExtraChunkPtr, (char)iVar1);
            (*(code*)(*piVar6 + 100))(piVar6, auStack_28); /* vtable[0x19] */
        }
    }
}
/* 3-byte offset helper: dst[0]=src[0]-d, dst[1]=src[1]-d, dst[2]=src[2]+d */
void Calc_ParticleOffset_3(char *dst, char *src, char delta)
{
    *dst = *src - delta;
    dst[1] = src[1] - delta;
    dst[2] = src[2] + delta;
}
/* Free all 18 grid particle entries */
void Free_DreamParticles_Grid(void)
{
    if (ParticleGridTier != 0) {
        ReleaseObjectArray(&ParticleGridEntries, 0x12);
        ParticleGridTier = 0;
    }
}
/********************************************************************
 * PARTICLE INSTANCES - randomised per-frame spawning
 ********************************************************************/
/* Spawn particle instances based on current mode */
void Spawn_DreamParticles_Instances(int *tickPtr)
{
    u32 r;
    int count1, count2;
    if (-1 < ParticleMode) {
        Init_DreamParticleEnv(ParticleMode,
            *(int**)(ParticleWorldPtr + 4),
            *(int*)(ParticleWorldPtr + 8),
            *(int*)(ParticleWorldPtr + 0xc));
        r = rand();
        count1 = (int)(char)(&ParticleSpawnCountTable)[r & 3];
        count2 = 0;
        if (ParticleMode == 2) count2 = 0x10 - count1;
        ParticleActiveCount = count1 + count2;
        Init_ParticleSpawn_Default(&ParticleInstanceArray, count1, tickPtr);
        Init_ParticleSpawn_Extra(&ParticleInstanceArray, count2, tickPtr);
        if (ParticleMode == 0)
            Init_ParticleSpawn_Type0(&ParticleInstanceArray, tickPtr);
        else if (ParticleMode == 2)
            Init_ParticleSpawn_Type2(&ParticleInstanceArray, tickPtr);
        ParticleActiveCount = ParticleActiveCount + 1;
    }
}
/* Update all active particle instances (vtable[0xec]/4) */
void Update_ParticleFrame(int *tick)
{
    int i;
    int *p;
    if ((-1 < ParticleMode) && (0 < ParticleActiveCount)) {
        p = &ParticleInstanceArray;
        for (i = 0; i < ParticleActiveCount; i = i + 1, p = p + 1)
            (*(code*)(*(int*)*p + 0xec))((int*)*p, tick); /* vtable[0x3b] */
    }
}
/* Free active particle instance array */
void Free_ActiveParticles(void)
{
    if (-1 < ParticleMode)
        ReleaseObjectArray(&ParticleInstanceArray, ParticleActiveCount);
}
/* Full system shutdown */
void Shutdown_DreamParticleSystem(void)
{
    int i;
    int *p;
    Destroy_DreamParticleEffect();
    Free_DreamParticles_Grid();
    Free_ActiveParticles();
    i = 0;
    p = &ParticleSecondarySlots;
    do {
        i = i + 1;
        *p = Release_SecondaryEntity(*p);
        p = p + 1;
    } while (i < 2);
    if (ParticleSysEntityPtr != 0) ParticleSysEntityPtr = 0;
}
/* Main frame tick - init on frame 0, then update & manage 2 entity slots */
int Tick_DreamParticleSystem(int tick, int state, int local)
{
    int iVar2, iVar3, iVar5;
    int *piVar4;
    int local_res8;
    local_res8 = local;
    if (tick != 0)
        (*(code*)(*(int*)ParticleSysEntityPtr + 0xe8))(ParticleSysEntityPtr, &local_res8, tick); /* vtable[0x3a] */
    iVar2 = ParticleFrameCounter + 1;
    if (ParticleFrameCounter == 0) {
        ParticleFrameCounter = iVar2;
        Spawn_DreamParticleEffect();
        Spawn_DreamParticles_Grid();
        Spawn_DreamParticles_Instances(&local_res8);
    } else {
        ParticleFrameCounter = iVar2;
    }
    iVar5 = 0;
    Update_DreamParticles_Anim();
    Update_ParticleFrame(&local_res8);
    iVar2 = 0;
    Set_MusicVolumeForLocation();
    ParticleEntityScanIndex = 0;
    piVar4 = &ParticleSecondarySlots;
    do {
        if (*piVar4 == 0) {
            iVar3 = (int)Spawn_SecondaryEntity(&Unk_SpuBuf154 + iVar2, &local_res8, &local_res8, state);
            *piVar4 = iVar3;
        } else {
            iVar3 = (int)CheckSecondaryEntityProximity(*piVar4, &local_res8, state);
            if (iVar3 == 0) {
                iVar3 = Release_SecondaryEntity(*piVar4);
                *piVar4 = iVar3;
            }
        }
        iVar2 = iVar2 + 0x68;
        iVar5 = iVar5 + 1;
        piVar4 = piVar4 + 1;
    } while (iVar5 < 2);
    return local_res8;
}
/* Adjust music volume by dream stage */
void Set_MusicVolumeForLocation(void)
{
    if (ParticleLocationIndex == 2) {
        SpuDrawLineSegments(&MusicVolLineDataLocation2, 1, &MusicVolLineTargetLocation2);
    } else if (ParticleLocationIndex - 3U <= 2) {
        SpuDrawLineSegments(&MusicVolLineDataLocation3, 1, &MusicVolLineTargetLocation3);
    }
}
/********************************************************************
 * PARTICLE SPAWNERS (velocity tables, colour tables)
 ********************************************************************/
/* Main spawner: creates 'count' particles with random settings */
int *Init_ParticleSpawn_Default(int *list, int count, int *tickPtr)
{
    int i;
    u32 r;
    int colour;
    void (*spawnFunc)(int, int);
    r = rand();
    ParticleAnimSelector = r % 7;
    r = rand();
    ParticleExtraVelPtr = (int)&Unk_EntityBufC8 + (r % 5) * 0xc;
    r = rand();
    colour = 0;
    if (r % 5 != 0)
        colour = *(int*)(&ParticleRandomColourArray + (r % 5) * 4);
    spawnFunc = Spawn_ParticleFall;
    if (ParticleEffectSelector != (ParticleEffectSelector / 7) * 7)
        spawnFunc = Spawn_ParticleRandomVelocity;
    for (i = 0; i < count; i = i + 1) {
        spawnFunc(tickPtr, colour);
        *list = New_DreamParticleObject(0, &ParticleVelX, ParticleSysEntityPtr, tickPtr);
        list = list + 1;
    }
    return list;
}
/* Extra spawner - fixed vertical velocity */
int *Init_ParticleSpawn_Extra(int *out, int count, int *tickPtr)
{
    int i;
    ParticleExtraVelPtr = (int)&ParticleFixedVelocity;
    for (i = 0; i < count; i = i + 1) {
        Spawn_ParticleRandomVelocity(tickPtr, 0xffffc800);
        *out = New_DreamParticleObject(1, &ParticleVelX, ParticleSysEntityPtr, tickPtr);
        out = out + 1;
    }
    return out;
}
/* Type-0 spawner - ground/underground mode */
int *Init_ParticleSpawn_Type0(int *out, int *tickPtr)
{
    u32 r;
    Spawn_ParticleRandomVelocity(tickPtr, 0xffffc800);
    if ((ParticleGridTier == 0) || (ParticleChunkPalette != (int)&ParticlePaletteDefault)) {
        if (0 < ParticleVelZ) ParticleVelZ = -ParticleVelZ;
        if (ParticleVelZ < -PARTICLE_LIFT) ParticleVelZ = -PARTICLE_LIFT;
        r = rand();
        ParticleOffsetDataPtr = (int)&Unk_ParticleBuf1C + (r % 3) * 3;
    } else {
        ParticleVelX = 0xffff5000;
        ParticleVelY = 0xffffe000;
        ParticleVelZ = 0;
        ParticleOffsetDataPtr = (int)&ParticleGroundVelocity3;
    }
    ParticleColourTablePtr = (int)&ParticleColourTable;
    *out = New_DreamParticleObject(3, &ParticleVelX, ParticleSysEntityPtr, tickPtr);
    return out + 1;
}
/* Type-2 spawner - colour cycling */
int *Init_ParticleSpawn_Type2(int *out, int *tickPtr)
{
    u32 r;
    int v;
    r = rand();
    ParticleOffsetDataPtr = (int)&Unk_ParticleBuf28 + (r % 3) * 3;
    ParticleCounter = 0;
    Spawn_ParticleRandomVelocity(tickPtr, 0xffffc800);
    ParticleColourTablePtr = (int)&ParticleColourTable;
    v = rand();
    ParticleAnimSelector = v % 6;
    *out = New_DreamParticleObject(2, &ParticleVelX, ParticleSysEntityPtr, tickPtr);
    return out + 1;
}
/* Fill global velocity tables with random values */
void Spawn_ParticleRandomVelocity(int tickPtr, int velY)
{
    u32 r;
    int i;
    if (velY == 0) {
        r = rand();
        velY = *(int*)(&ParticleRandomColourArray + (r & 3) * 4);
    }
    ParticleVelY = velY;
    i = rand();
    ParticleVelX = (i % 0x17) * PARTICLE_VEL_SCALE;
    r = rand();
    if ((r & 1) != 0) ParticleVelX = -ParticleVelX;
    i = rand();
    ParticleVelZ = (i % 0x17) * PARTICLE_VEL_SCALE;
    r = rand();
    if ((r & 1) != 0) ParticleVelZ = -ParticleVelZ;
    r = rand();
    ParticleColourTablePtr = (int)&ParticleColourTable + (r % 7) * 0xc;
    i = rand();
    ParticleSpawnSelector = i % 5;
}
/* Fall-effect: upward drift with horizontal spread */
void Spawn_ParticleFall(int tickPtr, int colour)
{
    int i;
    u32 r;
    rand();
    ParticleVelY = 0xffffd800;
    i = rand();
    ParticleVelZ = 0xa000;
    ParticleVelX = (i % 0x14) * PARTICLE_VEL_SCALE;
    if (ParticleEffectSelector % 3 == 1)
        ParticleVelZ = 0xffff6000;
    else if (ParticleEffectSelector % 3 == 2)
        ParticleVelZ = PARTICLE_VEL_SCALE;
    r = rand();
    ParticleColourTablePtr = (int)&ParticleColourTable + (r % 7) * 0xc;
    i = rand();
    ParticleSpawnSelector = i % 5;
}
/********************************************************************
 * SECONDARY ENTITIES - spawn + management
 ********************************************************************/
/* Find & create a secondary entity from spawn table */
int *Spawn_SecondaryEntity(int *out, int *pos, int *pos3, int *state)
{
    int idx;
    idx = Find_SecondaryEntitySlot(out + 1, out + 4, pos3);
    if (idx == 0) return 0;
    *out = idx;
    RegisterEntityOnLayer(*(int*)ParticleWorldPtr, out + 5, (int)*(char*)(idx + 6), out,
        *(int*)(&EntityLayerRegisterFuncTable + *(char*)(idx + 6) * 4));
    if ((int)*(char*)(idx + 6) == *state)
        *state = -(int)*(char*)(idx + 6);
    *(char*)(idx + 6) = -*(char*)(idx + 6);
    return out;
}
/* Scan current stage's entity list for a valid slot; returns ptr + writes dist */
void *Find_SecondaryEntitySlot(int *distOut, int *idxOut, int *pos3)
{
    int count, i;
    u8 *scan;
    int distX, distY;
    if (pos3 == 0) return 0;
    count = (u32)((u8*)&LocationSecondaryEntityCount)[ParticleLocationIndex] - ParticleEntityScanIndex;
    scan = (&PTR_LocationSecondaryEntityTablePtr)[ParticleLocationIndex] + ParticleEntityScanIndex * 8;
    if (0 < count) {
        for (i = 0; i < count; i = i + 1) {
            ParticleEntityScanIndex = ParticleEntityScanIndex + 1;
            if (0 < *(char*)(scan + 6)) {
                (*(code*)(*(int*)ParticleSysEntityPtr + 0xe8))(ParticleSysEntityPtr, distOut, scan); /* vtable[0x3a] */
                distX = *distOut - *pos3;
                if (distX < 0) distX = ~distX + 1;
                distY = distOut[2] - pos3[2];
                if (distY < 0) distY = -distY;
                *idxOut = distX + distY;
                if (distX + distY < *(int*)(&SecondaryEntityRangeTable + *(char*)(scan + 6) * 4))
                    return scan;
            }
            scan = scan + 8;
        }
    }
    return 0;
}
/* Release secondary entity - delist + flip used flag */
int Release_SecondaryEntity(int *entity)
{
     AnimReset(*(int*)ParticleWorldPtr, entity + 5);
    *(char*)(entity + 6) = -*(char*)(entity + 6);
    return 0;
}
/* Proximity check - refresh layer if entity is close enough */
int CheckSecondaryEntityProximity(int *entity, void *ptr, int extra)
{
    (void)extra;
    int result = FindNearbySecondaryEntity((int *)entity, (int *)ptr);
    if (result)
         AnimTick(*(int*)ParticleWorldPtr, entity + 0x14);
    return result;
}
/* Manhattan distance + range test */
int CheckEntityRange(int *entity, int *pos)
{
    u32 dx;
    int dy;
    if (pos == 0) return 0;
    dx = ((EntityObj *)entity)->nPosX - *pos;
    if ((int)dx < 0) dx = ~dx + 1;
    dy = ((EntityObj *)entity)->nPosZ - pos[2];
    if (dy < 0) dy = -dy;
    ((EntityObj *)entity)->nDist = dx + dy;
    if (*(int*)(&SecondaryEntityRangeTable + *(char*)(entity + 6) * -4) <= (int)(dx + dy))
        return 0;
    return 1;
}
/********************************************************************
 * DREAM SYMBOL SEQUENCE MATCHING
 ********************************************************************/
/* Match up to 4 dream symbols against ring buffer; returns 1 when all match */
int Match_DreamSymbolSequence(int *entity, int *state)
{
    int iVar1, iVar2, iVar4, iVar7;
    u32 slot;
    if (*(char*)(state + 0x467) != '\0') return 0;
    iVar7 = *(int*)(state + 8);
    if ((*(int*)(state + 4) != 0) || (100 < iVar7)) iVar7 = 100;
    for (slot = 0; slot < 4; slot = slot + 1) {
        int matches = 0;
        iVar1 = *(int*)(state + 8);
        for (iVar4 = 0; iVar4 < iVar7; iVar4 = iVar4 + 1) {
            iVar1 = iVar1 - 1;
            iVar2 = iVar1 * 2;
            if (iVar1 < 0) { iVar1 = 0x16c; iVar2 = 0x2d8; }
            if ((&DreamSymbolSequence)[slot] == *(short*)(state + iVar2 + 0x18)) {
                matches = matches + 1;
                *(char*)(*(int*)(entity + 0x240) + slot) = (char)iVar4;
            }
        }
        if (matches == 0) return 0;
    }
    *(char*)(state + 0x467) = 1;
    *(int*)(entity + 0x23c) = 0;
    return 1;
}
/* Get vtable for a symbol/sequence-related entity type */
/* (declared above as Get_vtable_DreamSymbol) */

/* === migrated from stubs.c: Entity_VisibleRender === */
int *entity_VisibleRender(int *entity) {
    /* Call vtable render callback at offset 0x100 */
    if (entity && entity)
        (**(void (**)(void *))(entity + 0x100))(entity);
}
/* === migrated from stubs.c: SetObjectTransform === */
void SetObjectTransform(void *obj, void *xfrm) {
    /* Copy transform struct (6 short values: pos xyz + rot/scale) */
    if (obj && xfrm)
        __builtin_memcpy(obj, xfrm, 12);
}
/* === migrated from stubs.c: RemoveEntityFromLayer === */
void RemoveEntityFromLayer(void *layerMgr, int slot) {
    /* Remove entity from layer by unlinking from layer list */
    if (layerMgr && slot > 0)
        *(int *)(slot) = 0;
}
/* === migrated from stubs.c: RegisterEntityOnLayer === */
void RegisterEntityOnLayer(void *layerMgr, int *slot, int type, int *entity, int regFunc) {
    (void)layerMgr; (void)slot; (void)type; (void)entity; (void)regFunc;
}
/* === migrated from stubs.c: UpdateEntityLayer === */
void UpdateEntityLayer(void *layerMgr, int *entity) { (void)layerMgr; (void)entity; }
/* === migrated from stubs.c: GetEntityVtable2 === */
void *GetEntityVtable2(void) { return (void *)ENTITY_VTABLE; }
/* === migrated from stubs.c: GetVtable_ObjectType21 === */
void *GetVtable_ObjectType21(void) { return (void *)ENTITY_VTABLE; }
/* === migrated from stubs.c: GetVtable_ObjectType23 === */
void *GetVtable_ObjectType23(void) { return (void *)ENTITY_VTABLE; }
/* === migrated from stubs.c: BasicClass__func_17eb0 === */
int BasicClass__func_17eb0(void *o) { MemFreeImpl((int)o); return 0; }
/* === migrated from stubs.c: BasicClass__func_18350 === */
void BasicClass__func_18350(void) { }
void BasicClass__func_17f98(void) { }
void BasicClass__func_17ff0(void) { }
void BasicClass__func_18040(void) { }
void BasicClass__func_18358(void) { }
/* === BasicClass ctor: stores CoordSystem vtable, zeroes +4/+8 (retail 0x80017ef8) === */
extern void *GetCoordSystemVtable(void);
void *BasicClass__BasicClass(void *o) {
    void *vtab = GetCoordSystemVtable();
    *(void **)o = vtab;
    *(int *)((char *)o + 8) = 0;
    *(int *)((char *)o + 4) = 0;
    return vtab;
}
/* === BasicClass dispatch: vtab[0x30](o,1), vtab[0x18](o), vtab[0x28](o) (retail 0x80017f2c) === */
void BasicClass__func_17f2c(void *o) {
    void *vtab = *(void **)o;
    ((void (*)(void *, int))((void **)vtab)[0x30 / 4])(o, 1);
    ((void (*)(void *))((void **)vtab)[0x18 / 4])(o);
    ((void (*)(void *))((void **)vtab)[0x28 / 4])(o);
}
/* === migrated from stubs.c: class_65650__Constructor === */
int *class_65650__Constructor(void *o, int a, int b) { (void)a; (void)b; (void)o; return 0; }

/* === migrated from stubs.c === */
/* FindNearbySecondaryEntity */
int FindNearbySecondaryEntity(int *entity, int *targetPos)
{
    if (!targetPos) return 0;
    int dx = ((EntityObj *)entity)->nPosX - targetPos[0];
    if (dx < 0) dx = -dx;
    int dz = ((EntityObj *)entity)->nPosZ - targetPos[2];
    if (dz < 0) dz = -dz;
    int dist = dx + dz;
    ((EntityObj *)entity)->nDist = dist;
    /* Look up range from SecondaryEntityRangeTable using entity type */
    int type = *(char *)(*(int *)entity + 6);
    int range = ((int *)&SecondaryEntityRangeTable)[-type];
    return dist < range ? 1 : 0;
}
/* ProcessEntityCallback */
void ProcessEntityCallback(int *entity, void *cb, int flags)
{
    (void)flags;
    if (entity && cb)
        ((void (*)(void *))cb)(entity);
}
extern void *MemAllocImpl(int size);
/* EntityAlloc50 */
int EntityAlloc50(int unused1, int unused2, int unused3)
{
    (void)unused1; (void)unused2; (void)unused3;
    void *ptr = MemAllocImpl(0x50);
    return ptr ? (int)ptr : 0;
}
/* AnimInitializeVariadic */
int AnimInitializeVariadic(int *entity, int *state, int initA, int initB, int initC)
{
    (void)entity;
    if (state[0]== 0) {
        int *p = state + 6;
        state[0]= initA;
        state[2] = initB;
        state[3] = initC;
        p[0] = -1;
        p[5] = -1;
        p[10] = -1;
        state[1] = 0;
        state[5] = 10;
        return 1;
    }
    return 0;
}
/* AnimReset */
void AnimReset(int *entity, int *state)
{
    (void)entity;
    for (int i = 0; i < 3; i++) {
        int *p = state + 6 + i * 5;
        if (*p >= 0) *p = -1;
    }
    state[0]= 0;
}
/* AnimTick */
void AnimTick(int *entity, int *state)
{
    (void)entity;
    if (state[0]<= 0) return;
    int *slot = state + 10;
    int *track = state + 6;
    for (int i = 0; i < 3; i++) {
        slot[-4] = -1;
        slot[-3] = 0;
        slot[-2] = SOUND_VOLUME_DEFAULT;
        slot[-1] = SOUND_VOLUME_HALF;
        slot += 5;
        track += 5;
    }
    state[4] = 0;
    if ((void *)state[3] != 0)
        ((void (*)(int, int *))state[3])(state[2], state);
    if (state[4] < 0) { state[1]++; return; }
    slot = state + 10;
    track = state + 6;
    for (int i = 0; i < 3; i++) {
        if (slot[-3] >= 0) {
            int denom = state[5];
            int va = slot[-1], vb = slot[0];
            *track = (slot[-3] << 4) +
                     (va - (va / denom) * state[4]) -
                     (vb - (vb / denom) * state[4]);
        }
        track += 5;
        slot += 5;
    }
    state[1]++;
}
/* EntityAllocSmall */
extern int *NopSub_269e0(void);
int EntityAllocSmall(int type)
{
    int obj = (int)MemAllocImpl(0x2c);
    if (obj != 0) {
        int vtab = (int)NopSub_269e0();
        (*(code *)(vtab + 8))(obj, type);
    }
    return obj;
}
/* BasicClass__func_180bc */
void BasicClass__func_180bc(int this, int *out, int *iter)
{
    if (*out == 0) {
        *iter = *(int *)(this + 4);
    }
    LinkedListPopFront((void **)out, (int **)iter);
}
/* BasicClass__func_180fc */
void BasicClass__func_180fc(int this, int value)
{
    int *head = (int *)(this + 8);
    int *node = (int *)MemAllocImpl(8);
    if (node) {
        node[1] = value;
        *node = *head;
        *head = (int)node;
    }
}
/* BasicClass__func_1811c */
void BasicClass__func_1811c(int this, int value)
{
    int *head = (int *)(this + 8);
    int *curr = (int *)*head;
    int *prev = 0;
    while (curr) {
        if (curr[1] == value) break;
        prev = curr;
        curr = (int *)*curr;
    }
    if (curr) {
        if (prev)
            *(int *)prev = *curr;
        else
            *head = *curr;
        MemFreeImpl((int)curr);
    }
}
/* BasicClass__func_1813c */
void BasicClass__func_1813c(int this)
{
    int *head = (int *)(this + 8);
    int *curr = (int *)*head;
    while (curr) {
        int *next = (int *)*curr;
        MemFreeImpl((int)curr);
        curr = next;
    }
    *(int *)(this + 8) = 0;
}
/* BasicClass__func_1816c */
void BasicClass__func_1816c(int this, int *out, int *iter)
{
    if (*out == 0) {
        *iter = *(int *)(this + 8);
    }
    LinkedListPopFront((void **)out, (int **)iter);
}
/* BasicClass__func_182cc */
void BasicClass__func_182cc(int this, int param2)
{
    int item;
    int iter = *(int *)(this + 8);
    LinkedListPopFront((void **)&item, (int **)&iter);
    while (item) {
        (**(void (**)(int, int, int))(*(int *)item + 0x38))(item, this, param2);
        LinkedListPopFront((void **)&item, (int **)&iter);
    }
}
/* EntityAllocMedium */
int EntityAllocMedium(int unused)
{
    (void)unused;
    return (int)MemAllocImpl(100);
}
/* AnimStateMathTransformMultipleVectors */
int AnimStateMathTransformMultipleVectors(int a, int *state, int frame)
{
    (void)a;
    if (frame >= 400) return 0;
    char *entry;
    if (state[0xd] == 0) {
        state[0xc] = 0;
        entry = (char *)(frame * 0xc + 8);
    } else {
        state[0xc] = 1;
        entry = (char *)(state[0xd]);
    }
    state[0xd] = 0;
    if (entry == 0) return -1;
    state[3] = (frame % 0x14) * DEGREES_180 + DEGREES_90;
    state[5] = (frame / 0x14) * DEGREES_180 + DEGREES_90;
    state[4] = 0;
    *(unsigned short *)(state + 6) = 0;
    *(unsigned short *)(state + 0xb) = 0;
    *(unsigned short *)(state + 0xd) = 0;
    state[0xe] = 0;
    return 0;
}
/* AnimInitializeBase */
int AnimInitializeBase(int *entity, int *state, int initA, int initB, int initC)
{
    (void)entity;
    if (*state != 0) return 0;
    state[6] = -1;
    state[11] = -1;
    state[1] = 0;
    state[5] = 10;
    return 1;
}
/* EntityAllocLarge */
int EntityAllocLarge(int unused1, int unused2, int unused3)
{
    (void)unused1; (void)unused2; (void)unused3;
    return (int)MemAllocImpl(0xa0);
}
/* ENTITY_FindByFlags */
int ENTITY_FindByFlags(int flags, int matchMask, int startIdx, int maxIdx)
{
    (void)flags; (void)matchMask; (void)startIdx; (void)maxIdx;
/* ParticleWorldPtr is in dat_globals.h */
    int *world = (int *)ParticleWorldPtr;
    if (!world) return 0;
    int *layerSlots = (int *)world[0];
    if (!layerSlots) return 0;
    int count = world[1];
    if (count <= 0) return 0;
    /* Walk the entity slot array (8 ints per slot) and find first non-null */
    for (int i = 0; i < count && i < 64; i++) {
        int *slot = &layerSlots[i * 8];
        if (slot[0] != 0)
            return (int)slot;
    }
    return 0;
}

/* === extern declarations for entity functions (moved from stubs.c) === */
extern void *MemAllocImpl(int size);
extern void MemFreeImpl(int ptr);
extern unsigned int Other_MemHeapPtr;
