/* Entity3_core.c -- split from Entity3_vtable.c */
#include "Entity3_Shared.h"
/* ==== Entity3.c core ==== */
void *Get_vtable_Entity(void)
{
    return &Vtable_Entity3;
}
int New_Entity(int arg1, int arg2, int arg3)
{
    int obj;
    int vtable;
    int result;
    obj = MemAllocImpl(0x108);
    if (obj != 0) {
        vtable = (int)Get_vtable_Entity();
        result = (*(code *)(vtable + 8))(obj, arg1, arg2, arg3);
        if (result != 0) {
            return obj;
        }
        MemFreeImpl(obj);
    }
    return 0;
}
EntityObj *entity__Entity(int *obj, int type, int arg3, int arg4)
{
    int iVar1;
    int *piVar2;
    iVar1 = Get_vtable_EntitySub();
    iVar1 = (*(code *)(iVar1 + 8))(obj, arg3, arg4);
    piVar2 = 0;
    if (iVar1 != 0) {
        iVar1 = (int)Get_vtable_Entity();
        *obj = iVar1;
        obj[0x26] = type;
        obj[0x27] = 0;
        obj[0x40] = 0;
        obj[0x41] = 0;
        (*(code *)(*obj + 0x40))(obj);
        piVar2 = obj;
    }
    return piVar2;
}
EntityObj *entityFindChildByType(int entity, int arg2, int arg3, int arg4, int arg5)
{
    int *obj;
    obj = *(int **)(entity + 0x100);
    if (obj == 0) {
        if (arg2 == 0) {
            arg2 = (int)&DefaultAttachmentParam_B;
        }
        obj = (int *)New_EntityAttachment(arg2, 0, arg5);
        if (obj == 0) {
            return 0;
        }
        *(int **)(entity + 0x100) = obj;
    }
    (*(code *)(*obj + 0x50))(obj);
    if (arg3 == 0) {
        arg3 = (int)&DefaultAttachmentParam_A;
    }
    (*(code *)(*obj + 0x4c))(obj, entity, arg3);
    (*(code *)(*obj + 0xd0))(obj, arg4);
    return obj;
}
void EntityDestroy(int obj)
{
    int *att;
    att = *(int **)(obj + 0x100);
    if (att != 0) {
        (*(code *)(*att + 4))();
    }
    att = *(int **)(obj + 0x104);
    if (att != 0) {
        (*(code *)(*att + 4))();
    }
    Get_vtable_EntitySub();
    (*(code *)((*(int *)Get_vtable_EntitySub()) + 0xc))(obj);
}
void EntityProcessMoodState(int *obj)
{
    int idx;
    s8 val;
    idx = obj[0x26];
    val = *(s8 *)((int)&EntityMoodEffectWeight + idx * 0x10);
    if ((u8)(val - 1) < 9) {
        (*(code *)(*obj + 0x70))(obj, 1);
    }
    (*(code *)(*obj + 0x10c))(obj, 0x42);
    (*(code *)(*obj + 0x160))(obj);
}
void EntityReceiveChild(int *obj, int arg2, int arg3, int arg4, int arg5)
{
    int iVar1;
    if (obj[3] == 0) {
        iVar1 = Get_vtable_EntitySub();
        (*(code *)(iVar1 + 0x4c))(obj, arg2, arg3, arg4, arg5);
        obj[0x13] = arg4;
        if (((*(s8 *)((int)&EntityMoodEffectWeight + obj[0x26] * 0x10 + 1) == 0) &&
            ((*(code *)(*obj + 0x15c))(obj),
             *(s8 *)((int)&EntityActiveFlag + obj[0x26] * 0x10) == 0))) {
            (*(code *)(*obj + 0x168))(obj);
        }
    }
}
void EntityReleaseChild(int *obj)
{
    int iVar1;
    if (obj[3] != 0) {
        (*(code *)(*obj + 0x160))();
        iVar1 = Get_vtable_EntitySub();
        (*(code *)(iVar1 + 0x50))(obj);
        obj[0x13] = 0;
    }
}
void EntityProcessFrame(int *obj, int arg2, int arg3)
{
    int iVar1;
    int subsys;
    iVar1 = (*(code *)(*obj + 0x170))();
    if (iVar1 != 0) {
        (*(code *)(*obj + 0x174))(obj);
    }
    iVar1 = (*(code *)(*obj + 0x17c))(obj);
    if (iVar1 != 0) {
        (*(code *)(*obj + 0x180))(obj);
    }
    (*(code *)(*obj + 0x178))(obj);
    subsys = Get_vtable_EntitySub();
    (*(code *)(subsys + 0x98))(obj, arg2, arg3);
}
void EntityHandleMessage(int *obj, int arg2, int arg3)
{
    s8 cVar1;
    int iVar2;
    int uVar3;
    cVar1 = *(s8 *)((int)&EntityLinkLocation + obj[0x26] * 0x10);
    if ((arg3 - 2 > 6) || (cVar1 > 0)) {
        iVar2 = Get_vtable_EntitySub();
        (*(code *)(iVar2 + 0xdc))(obj, arg2, arg3);
        if ((arg3 == 4) && (cVar1 > 0)) {
            if (cVar1 == 0x7f) {
                uVar3 = 0xc;
                if (*(s8 *)((int)&EntityEventVideo + obj[0x26] * 0x10) != 0) {
                    uVar3 = 0xb;
                }
            } else {
                uVar3 = 10;
            }
            (*(code *)(*obj + 0x30))(obj, uVar3);
        }
    }
}
void EntityHandleCollision(int *obj, int arg2, int arg3)
{
    int iVar1;
    iVar1 = Get_vtable_EntitySub();
    (*(code *)(iVar1 + 0xe0))(obj, arg2, arg3);
    if (arg3 == 4) {
        (*(code *)(*obj + 0x160))(obj);
    }
}
void EntityTickAnimation(int obj)
{
    ArrayCtrlProcessCd(*(int *)(obj + 0x58), obj + 0x9c);
    *(int *)(obj + 0xfc) += 1;
}
int EntityMoveToward(int obj, int *vec, int speed, int divisor)
{
    int local_18[3];
    int iVar1;
    local_18[0] = vec[0];
    local_18[1] = vec[1];
    local_18[2] = vec[2];
    if ((u8)(*(s8 *)((int)&EntityMoodEffectWeight + *(int *)(obj + 0x98) * 0x10) + 9) < 9) {
        local_18[1] += (*(s8 *)((int)&EntityMoodEffectWeight + *(int *)(obj + 0x98) * 0x10) << 18) >> 14;
    }
    if ((int)divisor < 0) {
        iVar1 = DEGREES_180 / (int)(~divisor + 1);
    } else {
        iVar1 = divisor << 11;
    }
    (*(code *)(**(int **)(obj + 0x94) + 0x120))
        (*(int *)(obj + 0x94), 0, speed << 11, local_18, iVar1);
    return iVar1;
}
int EntityCalcManhattanDist(int obj, int other)
{
    int *piVar3;
    uint uVar2;
    int iVar1;
    piVar3 = 0;
    if (*(int *)(other + 0xc) != 0) {
        piVar3 = (int *)(*(int *)(other + 0x14) + 0x38);
    }
    uVar2 = *(int *)(*(int *)(obj + 0x14) + 0x18) - *piVar3;
    if ((int)uVar2 < 0) {
        uVar2 = (uint)(-(int)uVar2);
    }
    iVar1 = *(int *)(*(int *)(obj + 0x14) + 0x20) - piVar3[2];
    if (iVar1 < 0) {
        iVar1 = -iVar1;
    }
    return uVar2 + iVar1;
}
int EntityCalcMoveProgress(int *obj)
{
    int iVar1;
    int iVar2;
    int iVar3;
    iVar1 = -1;
    if (obj[0x25] != 0) {
        iVar2 = (*(code *)(*obj + 0x144))();
        iVar3 = (s8)*(s8 *)((int)&EntityMoveScaleFactor + obj[0x26] * 0x10) << 11;
        iVar1 = -1;
        if (iVar2 <= iVar3) {
            iVar1 = obj[0x2c];
            iVar1 = iVar2 / (iVar3 / iVar1);
        }
    }
    return iVar1;
}
int Entity__GetMoodEffect(int obj)
{
    return (int)&EntityEffectDataTable + *(int *)(obj + 0x98) * 0x10;
}
int Entity__GetUnlockEffect(int obj)
{
    return (s8)*(s8 *)((int)&EntityMoodEffectWeight + *(int *)(obj + 0x98) * 0x10) * 1000;
}
int Entity__GetLinkLocation(int obj)
{
    uint uVar1;
    uVar1 = (uint)(s8)*(s8 *)((int)&EntityLinkLocation + *(int *)(obj + 0x98) * 0x10);
    if ((int)uVar1 < 0) {
        uVar1 = ~uVar1;
    } else {
        uVar1 = uVar1 - 1;
    }
    return uVar1;
}
int Entity__GetEventVideo(int obj)
{
    return (s8)*(s8 *)((int)&EntityEventVideo + *(int *)(obj + 0x98) * 0x10) - 1;
}
int GameStateCheck(int obj)
{
    int iVar1;
    if (*(int *)(*(int *)(obj + 0x20) + 0x10) == 0) {
        iVar1 = 2;
    } else {
        CdModeSubE(0, 0, 0);
        iVar1 = (*(code *)(**(int **)(obj + 0x28) + 0x1a0))(*(int **)(obj + 0x28), 0);
        if (((iVar1 != 1) && (*(int *)(obj + 0x24) == 0)) &&
            (            iVar1 = NopSub_26518(NewLocationArea, *(int *)(obj + 0x28),
                                  *(int *)(obj + 0x1c)), iVar1 == 2)) {
            NopSub_2658c(obj);
        }
        while (1) {
            iVar1 = NopSub_26518(LocationEntity_Alloc, *(int *)(obj + 0x28),
                                  *(int *)(obj + 0x1c));
            if (iVar1 != 2) break;
            NopSub_26518(NewLocationArea, *(int *)(obj + 0x28), *(int *)(obj + 0x1c));
        }
        *(int *)(obj + 0x24) = 0;
        iVar1 = (iVar1 == 0) ? 2 : 0;
    }
    return iVar1;
}
int NopSub_26518(int (*fn)(int), int arg2, int arg3)
{
    int *vobj;
    int result;
    vobj = (int *)fn(arg2);
    result = (*(code *)(*vobj + 0x44))(vobj, arg3, 0);
    (*(code *)(*vobj + 4))(vobj);
    return result;
}
void NopSub_2658c(int obj)
{
    int *vobj;
    int texId;
    int local_18[2];
    if (*(int *)(*(int *)(obj + 0x20) + 8) != 0) {
        CdModeSubE(0, 0, 0);
        vobj = (int *)Spu_NewSeqTrack(0, 0, 0, 0);
        texId = (int)func_800493e4(local_18, 0, 10);
        (*(code *)(*vobj + 0x6c))(vobj, local_18[0] / 15);
        (*(code *)(*vobj + 300))(vobj, 0);
        (*(code *)(*vobj + 0x44))(vobj, *(int *)(obj + 0x1c), texId, -1, 1);
        (*(code *)(*vobj + 4))(vobj);
    }
}
void NopSub_26690(void) { }
int EntityProcessLocationAlloc(int obj)
{
    int *vobj;
    int result;
    int local_18[2];
    vobj = (int *)EntityAlloc50(*(int *)(obj + 0x1c), *(int *)(obj + 0x28),
                                *(int *)(*(int *)(obj + 0x20) + 4));
    result = (*(code *)(*vobj + 0x44))(vobj);
    (*(code *)(*vobj + 4))(vobj);
    if (result == 2) {
        NopSub_2677c(obj);
    } else if (result == 3) {
        *(int *)(obj + 0x24) = 1;
    }
    result = (*(code *)(**(int **)(obj + 0x28) + 0x1a0))(*(int **)(obj + 0x28), local_18);
    return local_18[0] != 0 && result == 1;
}
void NopSub_2677c(int obj)
{
    int uVar1;
    int *vobj;
    int uVar3;
    int local_28[2];
    int local_20[4];
    (*(code *)(**(int **)(obj + 0x28) + 0x1bc))(local_28);
    uVar1 = (int)func_80049334(local_20, local_28[0]);
    CdModeSubE(0, 0, 0);
    if (local_20[0] == -1) {
        vobj = (int *)NavMenu_NewPage(0, 0, 0);
        (*(code *)(*vobj + 0x6c))(vobj, 10);
        (*(code *)(*vobj + 0xd4))(vobj, uVar1, 0);
        (*(code *)(*vobj + 0x44))(vobj, *(int *)(obj + 0x1c), 0);
    } else {
        if (*(int *)(*(int *)(obj + 0x20) + 8) == 0) {
            return;
        }
        vobj = (int *)Spu_NewSeqTrack(0, 0, 0, 0);
        (*(code *)(*vobj + 300))(vobj, 0);
        uVar3 = (int)func_800493c8(local_20[0]);
        (*(code *)(*vobj + 0x44))(vobj, *(int *)(obj + 0x1c), uVar1, uVar3, 1);
    }
    (*(code *)(*vobj + 4))(vobj);
}
void SpuPlaySequence(int obj)
{
    int *vobj;
    int texId;
    int clutId;
    int local_18[2];
    if (*(int *)(*(int *)(obj + 0x20) + 8) != 0) {
        CdModeSubE(0, 0, 0);
        vobj = (int *)Spu_NewSeqTrack(0, 0, 0, 0);
        (*(code *)(*vobj + 300))(vobj, 0);
        texId = (int)func_800491fc(local_18, 0);
        clutId = (int)func_800493c8(local_18[0]);
        (*(code *)(*vobj + 0x44))(vobj, *(int *)(obj + 0x1c), texId, clutId, 1);
        (*(code *)(*vobj + 4))(vobj);
    }
}

