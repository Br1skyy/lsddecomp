/* Entity3_behavior.c -- split from Entity3_vtable.c */
#include "Entity3_Shared.h"

/* Extern globals not in dat_globals.h */
extern int LAB_80089d60;
extern int LAB_80089d6c;
extern int D_8006D3C8;
extern int D_8006D430;
extern int D_8006D4AC;
extern int D_8008A84C;
extern int D_8008A850;
extern int D_8008A854;
extern int D_80010764;
extern int D_8008A848;
extern void func_8002C468(void);
extern int NopSub_27f18(int a0, int a1, int a2);
extern int NewStageArea(int a0);
extern int StageEntity_Alloc(int a0);
extern int func_8003b20c(void);
extern int func_80048cf0(void);
extern int Tmd_New(int *a0);
extern int New_DreamSys(int a0, int a1, int a2);
extern int MemAlloc(int size);
extern void SpuInit(void);
extern void PadInit(int a0);
extern void PadStop(void);
extern int PadRead(unsigned short mask);
extern void GsLinkObject4(int a, void *b, int c);
extern int ApplyRawMatrixToVectors(int *out, int matrixIdx, int flag, char *buf);
extern int PTR_EntityBehavior_WanderMultiPhase_80089eb0;
/* Forward declarations */
extern int MemAllocImpl(int size);
extern void MemFreeImpl(int ptr);
extern void *GetCoordSystemVtable(void);
extern void MathComputeAngleToTarget(EntityObj *entity, int target, int zeroHeading, int skipAngleOffset, int animParam);
extern void AnimReset(int obj, int *ptr);
extern EntityObj *entityFindChildByType(int entity, int arg2, int arg3, int arg4, int arg5);
extern int EntityMoveToward(int obj, int *vec, int speed, int divisor);
extern int EntityCheckTimerThreshold(EntityObj *entity, int threshold);
extern void EntityBehavior_InitMoveAim(EntityObj *entity, EvyMsg *state);
extern void EntityState_SetTrigger(EvyMsg *state);
extern void EntityState_SetFields(int state);
extern void EntityBehavior_ThresholdRangeReset(EntityObj *entity, EvyMsg *state, int threshold, int endThreshold, int offset);
extern void *Get_vtable_Entity(void);
extern int *Get_vtable_EntitySub(void);
extern int New_EntityAttachment(int type, int arg2, int arg3);
extern int PadManagerGetVtable(void);
extern void EntityBehavior_RandomAnim(EntityObj *entity);
extern int *CdModeUnknown(void);
extern void CdModeSubD(int *dst, int *src);
extern char *StrCat(char *dst, char *src);
/* =========================================================================
 * Entity3 vtable behavior methods
 * Cleaned from Ghidra decompilation of ref/Entity.c
 * ========================================================================= */
void EntityBehavior_RandomBranch(EntityObj *entity, EvyMsg *state)
{
    int iVar1;
    int uVar2;
    code *pcVar3;
    if (entity->nSubType == 0) {
        iVar1 = (*(code *)(*(int *)entity->pContainer + 0x1a0))((int *)entity->pContainer, 0);
        if (iVar1 % 3 == 0) {
            iVar1 = rand();
            if (iVar1 == (iVar1 / 3) * 3) {
                (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field25C);
            }
        } else if (iVar1 % 3 == 2) {
            (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field25C);
        }
    }
    if (state->nCounter == (state->nCounter / 0x16) * 0x16) {
        uVar2 = (*(code *)(*(int *)entity + 0x148))(entity);
        state->nReturn = uVar2;
        state->nDisp = 2;
    }
    iVar1 = rand();
    if (iVar1 == (iVar1 / 0xc) * 0xc) {
        pcVar3 = *(code **)(*(int *)entity + 0x130);
    } else {
        iVar1 = rand();
        if (iVar1 != (iVar1 / 6) * 6) {
            return;
        }
        pcVar3 = *(code **)(*(int *)entity + 300);
    }
    (*pcVar3)(entity);
}
void EntityBehavior_PeriodicJump(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    int iVar2;
    iVar2 = entity->nCondition;
    if (iVar2 == 0) {
        trap(TRAP_DIVZERO);
    }
    if ((iVar2 == -1) && (state->nCounter == -0x80000000)) {
        trap(TRAP_OVERFLOW);
    }
    if (state->nCounter % iVar2 == 0) {
        uVar1 = (*(code *)(*(int *)entity + 0x148))();
        state->nReturn = uVar1;
        state->nDisp = ENTITY_STATE_SPECIAL;
    }
    uVar1 = 0xfffffe80;
    if (entity->nSubType == ENTITY_STATE_LINKED) {
        uVar1 = 0xffffd300;
    }
    (*(code *)(*(int *)entity + 0xd0))(entity, uVar1, 0);
}
void EntityBehavior_RandomAnimPhase(EntityObj *entity, EvyMsg *state)
{
    int iVar1;
    uint uVar2;
    code *pcVar3;
    if ((entity->nState == 0) && (entity->nSubType == 0)) {
        iVar1 = rand();
        if (iVar1 == (iVar1 / 3) * 3) {
            (*(code *)(*(int *)entity + 0x16c))(entity);
            (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffb000, 0);
            rand();
        } else {
            uVar2 = rand();
            iVar1 = 0xc;
            if ((uVar2 & 1) != 0) {
                iVar1 = 0xb;
            }
            entity->nState = iVar1;
        }
    }
    if (entity->nState == 0xc) {
        MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
        if (entity->nSubType == 0x14) {
            state->nDisp = ENTITY_STATE_TRIGGER;
            state->nReturn = 0;
            state->nParam2 = 3;
            (*(code *)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer, 1);
        }
        if (0x14 < entity->nSubType) {
            (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffffd8, 0);
        }
        if (entity->nSubType == 0x28) {
            (*(code *)(*(int *)entity + 0x30))(entity, 10);
        }
    } else if (entity->nState == 0xb) {
        (*(code *)(*(int *)entity + 0x130))(entity);
        iVar1 = (*(code *)(*(int *)entity + 0x144))(entity, entity->pContainer);
        if (iVar1 < 0x200) {
            iVar1 = rand();
            if (iVar1 == (iVar1 / 3) * 3) {
                pcVar3 = *(code **)(*(int *)entity + 0x16c);
            } else {
                pcVar3 = *(code **)(*(int *)entity + 0x160);
            }
            (*pcVar3)(entity);
        }
    }
}
void EntityBehavior_PlayAnimSimple(int *obj)
{
    (*(code *)(*obj + 0x48))(obj, 1, &Entity_Field238);
}
void EntityBehavior_MoveConstant(int *obj)
{
    (*(code *)(*obj + 0xcc))(obj, 0xffffffa6, 0);
}
void EntityBehavior_MultiPhaseMove(EntityObj *entity, EvyMsg *state)
{
    int iVar1;
    int uVar2;
    uint uVar3;
    int iVar4;
    int iVar5;
    char *puVar6;
    uint uVar7;
    if (state->nCounter == 0) {
        Entity_DataCC = 0;
        iVar1 = rand();
        if (iVar1 % 3 == 1) {
            entity->nState = 0xb;
        }
        if (iVar1 % 3 == 2) {
            entity->nState = 0xc;
        }
    }
    uVar2 = (*(code *)(*(int *)entity + 0x148))(entity);
    state->nReturn = uVar2;
    if ((entity->nParam2 != 0) && ((state->nCounter & ENTITY_STATE_TYPE_MASK) == 0)) {
        state->nDisp = ENTITY_STATE_FLAG_MASK;
    }
    iVar1 = entity->nCondition;
    iVar5 = entity->nSubType;
    if (iVar5 == iVar1 + -1) {
        entity->nSubType = -1;
    } else {
        iVar4 = iVar1 / 2;
        if (iVar1 < 0) {
            iVar1 = iVar1 + 3;
        }
        if (iVar5 < iVar4 + (iVar1 >> 2)) {
            if (iVar5 < iVar4) {
                if (!(iVar5 < iVar1 >> 2)) {
                    (*(code *)(*(int *)entity + 0x130))(entity);
                    iVar1 = *(int *)entity;
                    uVar2 = 0xffffff92;
                    (*(code *)(iVar1 + 0xc4))(entity, uVar2, 0);
                }
            } else {
                if (iVar5 == iVar4) {
                    state->nDisp = ENTITY_STATE_BUSY;
                }
                iVar1 = *(int *)entity;
                uVar2 = 0x6e;
                (*(code *)(iVar1 + 0xc4))(entity, uVar2, 0);
            }
        } else {
            (*(code *)(*(int *)entity + 300))(entity);
        }
    }
    if ((entity->nState == 0xb) && (state->nCounter == 0x1fe)) {
        (*(code *)(*(int *)entity + 0xcc))(entity, 0xfffffe84, 0);
        (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field10C);
        (*(code *)(*(int *)entity + 0x16c))(entity);
        entity->nState = 1;
        Entity_DataCC = 1;
    } else if ((0xb < entity->nState) &&
               ((iVar1 = state->nCounter, 0x149 < iVar1 && ((iVar1 / 0x3c) * 0x3c == iVar1 + -0x1e)))) {
        uVar7 = 0;
        uVar3 = rand();
        if ((uVar3 & 1) == 0) {
            puVar6 = &Entity_Field214;
            if (entity->nState == 0xd) {
                uVar7 = 0xfffffe70;
            }
            entity->nState = 0xc;
        } else {
            puVar6 = &Entity_Field250;
            iVar1 = entity->nState;
            entity->nState = 0xd;
            uVar7 = -(uint)(iVar1 == 0xc) & 400;
        }
        (*(code *)(*(int *)entity + 0x48))(entity, 1, puVar6);
        (*(code *)(*(int *)entity + 0xcc))(entity, uVar7, 0);
    }
    if ((state->nCounter == 0x208) && (Entity_DataCC != 0)) {
        (*(code *)(*(int *)entity + 0x16c))(entity);
        entity->nState = 1;
    }
}
void EntityBehavior_PlayAnim1CC(int *obj)
{
    (*(code *)(*obj + 0x48))(obj, 1, &Entity_Field1CC);
}
void EntityBehavior_InitSubStateB(EntityObj *entity, EvyMsg *state)
{
    state->nReturn = 0;
    if (state->nCounter == 0) {
        state->nDisp = 0xb;
        state->nParam2 = 0xb;
        state->nParam4 = 0xb;
    }
    (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffffe2, 0);
}
void EntityBehavior_AimAtTarget(int obj)
{
    MathComputeAngleToTarget(obj, *(int *)(obj + 0x94), 1, 0, 0);
}
void EntityBehavior_RandomRoll(EntityObj *entity, EvyMsg *state)
{
    int iVar1;
    int uVar2;
    int iVar3;
    iVar3 = entity->nSubType;
    if (iVar3 == 0) {
        iVar1 = rand();
        iVar3 = iVar1;
        if (iVar1 < 0) {
            iVar3 = iVar1 + 3;
        }
        (*(code *)(*(int *)entity + 0x128))(entity, iVar1 + (iVar3 >> 2) * -4);
    } else {
        iVar1 = entity->nCondition;
        if (iVar1 == 0) {
            trap(TRAP_DIVZERO);
        }
        if ((iVar1 == -1) && (iVar3 == -0x80000000)) {
            trap(TRAP_OVERFLOW);
        }
        if (iVar3 % iVar1 == 0) {
            iVar1 = rand();
            iVar3 = iVar1;
            if (iVar1 < 0) {
                iVar3 = iVar1 + 3;
            }
            (*(code *)(*(int *)entity + 0x128))(entity, iVar1 + (iVar3 >> 2) * -4);
            uVar2 = (*(code *)(*(int *)entity + 0x148))(entity);
            state->nReturn = uVar2;
            state->nDisp = 0x16;
            state->nParam1 = 2;
            state->field_0x24 = ENTITY_STATE_DETACHED;
            state->field_0x28 = ENTITY_STATE_ATTACHED;
        }
    }
}
int EntityCheckTriggerState(int *obj)
{
    int bVar1;
    int iVar2;
    int iVar3;
    uint uVar4;
    if (obj[0x3c] != 0) {
        return obj[0x3c];
    }
    if (obj[0x11] == 1) {
        return obj[0x3c];
    }
    iVar2 = obj[0x26] * 0x10;
    bVar1 = 0;
    if (((char *)&Entity_Field2A7)[iVar2] != '\0') {
        if (((char *)&Entity_Field2A7)[iVar2] == '\x04') {
            uVar4 = rand();
            if ((uVar4 & 0x7f) == 0) {
                bVar1 = 1;
            }
        } else if ((char)((char *)&Entity_Field2A9)[iVar2] != 0) {
            iVar3 = EntityMoveToward(obj, obj[5] + 0x18,
                                  (int)(char)((char *)&Entity_Field2A9)[iVar2],
                                  (int)(char)((char *)&Entity_Field2AD)[iVar2]);
            if (iVar3 == 0) {
                if (((char *)&Entity_Field2A7)[iVar2] == '\x02') {
                    bVar1 = 1;
                }
            } else {
                if (((char *)&Entity_Field2A7)[iVar2] == '\x01') {
                    bVar1 = 1;
                } else if (((char *)&Entity_Field2A7)[iVar2] == '\x03') {
                    uVar4 = rand();
                    if ((uVar4 & 0x7f) == 0) {
                        bVar1 = 1;
                    }
                }
            }
        }
    }
    if (bVar1) {
        (*(code *)(*obj + 0x15c))(obj);
    }
    return obj[0x3c];
}
void EntityBehavior_ThresholdTrigger(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    int iVar2;
    int iVar3;
    iVar3 = entity->nSubType;
    if (iVar3 < 0x14) {
        (*(code *)(*(int *)entity + 0x130))(entity);
        (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffffe2, 0);
    } else {
        if (iVar3 == 0x14) {
            (*(code *)(*(int *)entity + 300))(entity);
            uVar1 = 5;
            state->nReturn = 0;
        } else {
            iVar2 = entity->nCondition * 3 + 0x14;
            if (iVar2 == 0) {
                trap(TRAP_DIVZERO);
            }
            if ((iVar2 == -1) && (iVar3 == -0x80000000)) {
                trap(TRAP_OVERFLOW);
            }
            if (iVar3 % iVar2 != 0) {
                return;
            }
            (*(code *)(*(int *)entity + 0x130))(entity);
            uVar1 = 0xfffffffe;
        }
        state->nDisp = uVar1;
    }
}
void EntityBehavior_Nop(void)
{
}
void EntityBehavior_AnimWithOffset(int *obj)
{
    EntityBehavior_InitMoveAim(obj, 0);
    (*(code *)(*obj + 0x48))(obj, 1, &Entity_Field238);
}
void EntityBehavior_ThresholdJump(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    uVar1 = (*(code *)(*(int *)entity + 0x148))();
    state->nReturn = uVar1;
    if (entity->nSubState == 10) {
        state->nDisp = 0xd;
    }
    (*(code *)(*(int *)entity + 0xc4))(entity, 0xfffffff6, 0);
}
void EntityBehavior_PeriodicBounce(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    int iVar2;
    uVar1 = (*(code *)(*(int *)entity + 0x148))();
    state->nReturn = uVar1;
    iVar2 = entity->nCondition / 2;
    if (iVar2 == 0) {
        trap(TRAP_DIVZERO);
    }
    if ((iVar2 == -1) && (state->nCounter == -0x80000000)) {
        trap(TRAP_OVERFLOW);
    }
    if (state->nCounter % iVar2 == 0) {
        state->nDisp = 10;
    }
    (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffffe2, 0);
}
void EntityBehavior_PeriodicAnimState(EntityObj *entity, EvyMsg *state)
{
    uint uVar1;
    int uVar2;
    int iVar3;
    if (((entity->nSubType == 0) && (uVar1 = rand(), (uVar1 & 3) == 0)) || (entity->nSubType == 0xe10)) {
        (*(code *)(*(int *)entity + 0x160))(entity);
        entity->nState = 1;
    }
    (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field244);
    uVar2 = (*(code *)(*(int *)entity + 0x148))(entity);
    state->nReturn = uVar2;
    iVar3 = entity->nCondition / 2;
    if (iVar3 == 0) {
        trap(TRAP_DIVZERO);
    }
    if ((iVar3 == -1) && (state->nCounter == -0x80000000)) {
        trap(TRAP_OVERFLOW);
    }
    if (state->nCounter % iVar3 == 0) {
        state->nDisp = 10;
        state->nParam1 = 1;
    }
    (*(code *)(*(int *)entity + 0xc4))(entity, 0xfffffff6, 0);
}
void EntityBehavior_RandomActivate(int *obj)
{
    int iVar1;
    iVar1 = rand();
    (*(code *)(*obj + 0x60))(obj, iVar1 % 0x14 == 0);
}
void EntityBehavior_FallbackState(EntityObj *entity, int state)
{
    int iVar1;
    int iVar2;
    if (entity->nSubType == 0) {
        iVar1 = rand();
        entity->nState = iVar1 % 5 + 10;
    }
    if ((entity->nState < 0xe) || (entity->nSubType < 0x140)) {
        EntityBehavior_ThresholdRangeReset(entity, state, 3000, 500, 0xffffff00);
    } else if ((entity->nState == 0xe) && ((entity->nSubType & 3U) == 0)) {
        iVar2 = rand();
        iVar1 = iVar2;
        if (iVar2 < 0) {
            iVar1 = iVar2 + 0x1f;
        }
        Entity_Field2A2 = (short)iVar2 + (short)(iVar1 >> 5) * -0x20 + 1;
        (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field298);
    }
}
void EntityBehavior_RandomAnimSelect(int *obj)
{
    int iVar1;
    char *puVar2;
    iVar1 = rand();
    if (iVar1 != (iVar1 / 3) * 3) {
        iVar1 = rand();
        puVar2 = &Entity_Field70;
        if (iVar1 != (iVar1 / 3) * 3) {
            puVar2 = &Entity_Field64;
        }
        (*(code *)(*obj + 0x44))(obj, 0, puVar2);
    }
}
void EntityResetSequence(int *obj)
{
    AnimReset(obj[0x16], obj + 0x27);
    (*(code *)(*obj + 0x130))(obj);
    (*(code *)(*obj + 0x114))(obj);
    obj[0x3e] = 0;
}
void EntityBehavior_MultiThresholdState(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    int iVar2;
    uVar1 = (*(code *)(*(int *)entity + 0x148))();
    state->nReturn = uVar1;
    iVar2 = entity->nSubState;
    if (iVar2 < 0x28) {
        state->nDisp = 0xc;
        state->nParam1 = 0xfffffffe;
        state->nParam4 = 5;
        state->nParam5 = 0xffffffff;
    } else if (iVar2 == 0x28) {
        state->nDisp = 0xfffffffe;
        state->nParam4 = 0xfffffffe;
    } else if (iVar2 == 0x2d) {
        state->nParam2 = ENTITY_STATE_TRIGGER;
        state->nParam3 = 1;
    } else if (iVar2 == 0x40) {
        state->nDisp = 7;
    } else if (iVar2 == 0x59) {
        (*(code *)(*(int *)entity + 0x16c))(entity);
        entity->nState = 1;
    }
}
void EntityBehavior_AimAndAnimate(int *obj)
{
    MathComputeAngleToTarget(obj, obj[0x25], 1, 0, 0);
    (*(code *)(*obj + 0x48))(obj, 1, &Entity_Field280);
    (*(code *)(*obj + 0xc4))(obj, 0xffffffe2, 1);
}
void EntityBehavior_SwitchAnimOnTimer(int *obj)
{
    int iVar1;
    int uVar2;
    iVar1 = obj[0x3f];
    if (iVar1 - 400U < 10) {
        (*(code *)(*obj + 0x44))(obj, 0, &Entity_Field64);
    } else if ((iVar1 - 700U < 10) || (iVar1 - 0x33eU < 4)) {
        (*(code *)(*obj + 0x44))(obj, 0, &Entity_Field70);
    } else if (0x352 < iVar1) {
        (*(code *)(*obj + 0x160))(obj);
    }
    uVar2 = 0xfffffe00;
    if (obj[0x3f] < 800) {
        uVar2 = 0xffffffc4;
    }
    (*(code *)(*obj + 0xc4))(obj, uVar2, 1);
}
int EntityHandleActivityTimeout(int *obj)
{
    int iVar1;
    if (obj[0x3c] != 0) {
        if (obj[0x3e] == 0) {
            return 0;
        }
        if (((char)((char *)&Entity_Field2AF)[obj[0x26] * 0x10] < 0) &&
           (iVar1 = EntityMoveToward(obj, obj[5] + 0x18,
                                  ~(int)(char)((char *)&Entity_Field2AF)[obj[0x26] * 0x10] + 1,
                                  (int)(char)((char *)&Entity_Field2AD)[obj[0x26] * 0x10]), iVar1 == 0)) {
            (*(code *)(*obj + 0x16c))(obj);
        }
    }
    return obj[0x3e];
}
void EntityBehavior_ScanOrAim(EntityObj *entity, EvyMsg *state)
{
    code *pcVar1;
    int iVar2;
    int iVar3;
    int *piVar4;
    int uVar5;
    if (entity->pSubObj == 0) {
        if (entity->nResultFlag == 0) {
            if (entity->nSubState == 0) {
                do {
                    iVar2 = (*(code *)(*(int *)entity + 0x134))(entity, entity->pChild2, 0);
                    iVar3 = entity->nSubState;
                    entity->pChild2 = iVar2;
                    entity->nSubState = iVar3 + 1;
                } while (iVar3 + 1 < 0x18);
            }
        } else {
            MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
            (*(code *)(*(int *)entity + 0x128))(entity, 1);
            pcVar1 = *(code **)(*(int *)entity->pContainer + 0x130);
            uVar5 = 1;
            piVar4 = (int *)entity->pContainer;
            (*pcVar1)(piVar4, uVar5);
        }
    } else {
        if (entity->nSubState == 0) {
            state->nReturn = 0;
            state->nDisp = 0x16;
        } else if (entity->nSubState == entity->nCondition + -1) {
            state->nReturn = 0;
            state->nParam2 = ENTITY_STATE_TRIGGER;
            pcVar1 = *(code **)(*(int *)entity + 0x30);
            uVar5 = 10;
            piVar4 = entity;
            (*pcVar1)(piVar4, uVar5);
        }
    }
    (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field1E4);
}
void EntityBehavior_ThreePhaseState(EntityObj *entity, EvyMsg *state)
{
    int iVar1;
    int uVar2;
    state->nReturn = 0;
    if (entity->nSubType < 100) {
        if (state->nCounter == (state->nCounter / 3) * 3) {
            state->nDisp = 0x16;
            state->nParam1 = 1;
        }
        iVar1 = *(int *)entity;
        uVar2 = 0xffffffc0;
    } else if (entity->nSubType < 300) {
        state->nDisp = 0xc;
        state->nParam1 = 0xffffffff;
        state->nParam2 = 0xc;
        state->nParam3 = 0xffffffff;
        state->nParam4 = 0xc;
        state->nParam5 = 0xffffffff;
        iVar1 = *(int *)entity;
        uVar2 = 0xffffff00;
    } else {
        (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field58);
        iVar1 = *(int *)entity;
        uVar2 = 0xfffffe00;
    }
    (*(code *)(iVar1 + 0xcc))(entity, uVar2, 0);
}
void EntityBehavior_PeriodicAnimSwitch(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    int iVar2;
    char *puVar3;
    puVar3 = (char *)0x0;
    if (state->nCounter == (state->nCounter / 7) * 7) {
        uVar1 = (*(code *)(*(int *)entity + 0x148))(entity);
        state->nReturn = uVar1;
        state->nDisp = 3;
        state->field_0x24 = ENTITY_STATE_DETACHED;
        state->field_0x28 = ENTITY_STATE_DETACHED;
    }
    iVar2 = entity->nSubType;
    if (iVar2 == 200) {
        puVar3 = &Entity_Field94;
    } else if (iVar2 == 400) {
        puVar3 = &Entity_Field7C;
    } else if (iVar2 == 600) {
        puVar3 = &Entity_Field88;
    } else if (iVar2 == 800) {
        puVar3 = &Entity_Field7C;
        entity->nSubType = -1;
    }
    if (puVar3 != (char *)0x0) {
        (*(code *)(*(int *)entity + 0x44))(entity, 0, puVar3);
    }
    (*(code *)(*(int *)entity + 0xd0))(entity, 0xffffffe2, 0);
    if (entity->nConfig != 0) {
        (*(code *)(*(int *)entity + 0xcc))(entity, 0xffffff38, 0);
    }
}
void EntityBehavior_PeriodicStateEnd(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    uVar1 = (*(code *)(*(int *)entity + 0x148))();
    state->nReturn = uVar1;
    if (state->nCounter == (state->nCounter / 10) * 10) {
        state->nDisp = 3;
    }
    if (entity->nSubType == entity->nCondition) {
        (*(code *)(*(int *)entity + 0x128))(entity, 1);
    }
    if (entity->pSubObj == 1) {
        (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffff80, 1);
    }
}
void EntityBehavior_InitThenMove(int *obj)
{
    int iVar1;
    if (obj[0x3f] == 0) {
        (*(code *)(**(int **)(obj[0x25] + 0x5c) + 100))
                  (*(int **)(obj[0x25] + 0x5c), &Entity_CdField);
        iVar1 = rand();
        obj[0x11] = iVar1 % 3;
        if (*(int *)(*(int *)(obj[0x25] + 0x14) + 0x20) < 0x262) {
            obj[0x11] = 0;
        }
    }
    if (obj[0x11] == 0) {
        if ((obj[0x3f] - 0x14U < 100) &&
           ((*(code *)(*(int *)obj[0x25] + 0xc4))
                     ((int *)obj[0x25], (obj[0x3f] + -0x13) * -0x20, 1), obj[0x3f] == 0x55))
        {
            (*(code *)(*(int *)obj[0x25] + 0x134))((int *)obj[0x25], 1, 1);
        }
    } else {
        if (obj[0x20] / 2 < obj[0x3f]) {
            (*(code *)(*(int *)obj[0x25] + 0xc4))((int *)obj[0x25], 0x80, 0);
        }
        if (obj[0x3f] == obj[0x20] + -0x1e) {
            (*(code *)(*obj + 0x30))(obj, 10);
        }
    }
}
void EntityBehavior_ChasePeriodic(EntityObj *entity, EvyMsg *state)
{
    MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
    (*(code *)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer, 1);
    if (state->nCounter % 10 < 3) {
        state->nReturn = 0;
        state->nDisp = 0xd;
        state->nParam2 = 0xd;
        state->nParam4 = 0xd;
    }
    if (entity->nSubType == entity->nCondition) {
        (*(code *)(*(int *)entity + 0x130))(entity);
        (*(code *)(*(int *)entity + 0x30))(entity, 10);
    }
}
void EntityBehavior_RandomMove(int *obj)
{
    uint uVar1;
    int uVar2;
    if ((obj[0x3f] == 0) && (uVar1 = rand(), (uVar1 & 1) != 0)) {
        obj[0x11] = 0xb;
    }
    if (obj[0x3f] == 300) {
        (*(code *)(*obj + 0x44))(obj, 0, &Entity_Field7C);
    }
    if (obj[0x3f] < 600) {
        uVar2 = 0x100;
        if (obj[0x11] == 0) {
            uVar2 = 0xffffff00;
        }
        (*(code *)(*obj + 0xc4))(obj, uVar2, 0);
    }
}
void EntityBehavior_ThresholdAnimState(EntityObj *entity, EvyMsg *state)
{
    int iVar1;
    int uVar2;
    int iVar3;
    iVar3 = entity->nSubState;
    if ((entity->nSubType == 0) && (iVar1 = rand(), iVar1 == (iVar1 / 3) * 3)) {
        (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field250);
    }
    uVar2 = (*(code *)(*(int *)entity + 0x148))(entity);
    state->nReturn = uVar2;
    if (0x1f < iVar3) {
        iVar3 = iVar3 + -0x20;
    }
    if ((((iVar3 == 9) || (iVar3 == 0x11)) || (iVar3 == 0x17)) &&
       (state->nDisp = 0x13, iVar3 == 0x17)) {
        state->nParam2 = 0x13;
    }
}
void EntityBehavior_SlowDecelPeriodic(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    uVar1 = (*(code *)(*(int *)entity + 0x148))();
    state->nReturn = uVar1;
    if (state->nCounter == (state->nCounter / 10) * 10) {
        state->nDisp = 0x11;
    }
    (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffff00, 0);
}
void EntityBehavior_PeriodicAction(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    if (state->nCounter == (state->nCounter / 0x78) * 0x78) {
        uVar1 = (*(code *)(*(int *)entity + 0x148))(entity);
        state->nReturn = uVar1;
        state->nDisp = 1;
    }
}
void EntityBehavior_MultiPhaseThreshold(EntityObj *entity, EvyMsg *state)
{
    int iVar1;
    int uVar2;
    if ((entity->nSubType == 0) && (iVar1 = (*(code *)(*(int *)entity->pContainer + 0x200))(), iVar1 == 5))
    {
        entity->nState = 0xb;
    }
    if ((entity->nState == 0) || (iVar1 = entity->nSubType, iVar1 < 0x870)) {
        uVar2 = 0x884;
    } else {
        if (iVar1 - 0x870U < 0x191) {
            if (iVar1 == 0x870) {
                (*(code *)(*(int *)entity + 0x130))(entity);
                state->nDisp = 0xfffffffe;
                state->nParam2 = 0xfffffffe;
                state->nParam4 = 0xfffffffe;
                return;
            }
            if (iVar1 - 0x9f6U < 10) {
                state->nDisp = 5;
                state->nParam1 = 0xfffffffe;
                return;
            }
            if (iVar1 != 0xa00) {
                return;
            }
            (*(code *)(*(int *)entity + 300))(entity);
            state->nCounter = 1;
            return;
        }
        if (iVar1 < 0xa03) {
            return;
        }
        if (0xaf0 < iVar1) {
            (*(code *)(*(int *)entity + 0xcc))(entity, 0xffffffe0, 0);
        }
        uVar2 = 4000;
    }
    EntityBehavior_ThresholdRangeReset(entity, state, 0x1e1, uVar2, 0xffffffc4);
}
void EntityBehavior_MultiPhaseSound(int *obj)
{
    code *pcVar1;
    int iVar2;
    MathComputeAngleToTarget(obj, obj[0x25], 1, 0, 0);
    if (obj[0x3d] == 0) {
        iVar2 = obj[0x3f];
        if (iVar2 == 0) {
            (*(code *)(*obj + 0xbc))(obj, &Entity_Field148);
            return;
        }
        if (iVar2 < 0x41) {
            (*(code *)(*obj + 0xbc))(obj, &LAB_80089d60);
            return;
        }
        if (iVar2 < 0x47) {
            (*(code *)(*obj + 0xbc))(obj, &Entity_Field178);
            pcVar1 = *(code **)(*obj + 0xc4);
            iVar2 = -0x1e;
        } else {
            if (iVar2 < 0x100) {
                iVar2 = -0x41 - iVar2;
            } else {
                iVar2 = 0xff;
            }
            pcVar1 = *(code **)(*obj + 0xc4);
        }
    } else {
        if (0x40 < obj[0x3f]) {
            obj[0x3f] = 0;
        }
        if (obj[0x3f] < 7) {
            (*(code *)(*obj + 0xbc))(obj, &Entity_Field19C);
            return;
        }
        (*(code *)(*obj + 0xbc))(obj, &Entity_Field178);
        pcVar1 = *(code **)(*obj + 0xc4);
        iVar2 = 10;
    }
    (*pcVar1)(obj, iVar2, 0);
}
void EntityBehavior_ProximityChase(int *obj)
{
    uint uVar1;
    int iVar2;
    if ((obj[0x3f] == 0) && (uVar1 = rand(), (uVar1 & 1) == 0)) {
        obj[0x11] = 0xb;
    }
    if (*(int *)(obj[5] + 0x1c) < 2000) {
        MathComputeAngleToTarget(obj, obj[0x25], 1, 0, 0);
    }
    if (obj[0x11] == 0xb) {
        iVar2 = (*(code *)(*obj + 0x144))(obj, obj[0x25]);
        if (iVar2 < 0xa00) {
            (*(code *)(*(int *)obj[0x13] + 0x138))((int *)obj[0x13], 1, 1);
            obj[0x3f] = 1;
            obj[0x11] = 0xc;
        }
    } else if ((obj[0x11] == 0xc) && (iVar2 = obj[0x3f], obj[0x3f] = iVar2 + 1, iVar2 == 300)) {
        (*(code *)(*obj + 0x30))(obj, 0xc);
    }
}
void EntityBehavior_ProgressiveAnim(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    int iVar2;
    int *puVar3;
    if (entity->nSubType == 0) {
        (*(code *)(*(int *)entity + 0xcc))(entity, 0xfffffe00, 0);
    }
    uVar1 = (*(code *)(*(int *)entity + 0x148))(entity);
    state->nReturn = uVar1;
    if (entity->nSubState == entity->nCondition / 2) {
        state->nDisp = 7;
        state->nParam1 = 0xfffffffe;
        state->nParam2 = 3;
        state->nParam3 = 0xfffffffe;
    }
    if (0x32 < entity->nSubType) {
        (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_FieldAC);
    }
    if (entity->nSubType < 0x30d) {
        iVar2 = *(int *)entity;
        uVar1 = 0xffffff00;
    } else {
        MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
        iVar2 = entity->nSubType;
        if (iVar2 < 0x790) {
            if (iVar2 < 0x78b) {
                if (iVar2 < 0x786) {
                    puVar3 = &Entity_Field220;
                    if (0x780 < iVar2) {
                        puVar3 = &Entity_Field1CC;
                    }
                } else {
                    puVar3 = &Entity_Field1D8;
                }
            } else {
                puVar3 = &Entity_Field1E4;
            }
        } else {
            puVar3 = &Entity_Field214;
        }
        (*(code *)(*(int *)entity + 0x48))(entity, 1, puVar3);
        if (1999 < entity->nSubType) {
            entity->nState = 1;
        } else {
            iVar2 = *(int *)entity;
            uVar1 = 0xffffffc0;
            (*(code *)(iVar2 + 0xc4))(entity, uVar1, 0);
        }
    }
    if ((entity->nResultFlag != 0) && (entity->nState == 0)) {
        entity->nState = 0xc;
        (*(code *)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer, 1);
        (*(code *)(*(int *)entity + 0x30))(entity, 10);
    }
    if (entity->nState == 0xc) {
        (*(code *)(*(int *)entity->pContainer + 0xc4))((int *)entity->pContainer, 0x100, 0);
    }
}
void EntityBehavior_ChildScan(EntityObj *entity, EvyMsg *state)
{
    int iVar1;
    uint uVar2;
    int iVar3;
    if (entity->nSubType == 0) {
        iVar1 = EntityFindChildByType(entity, 0, 0, 5, 0);
        if (iVar1 != 0) {
            uVar2 = rand();
            if ((uVar2 & 1) != 0) {
                (*(code *)(*(int *)entity + 0xbc))(entity, &Entity_Field190);
            }
            (*(code *)(*(int *)((int *)entity)[0x40] + 0xd4))((int *)((int *)entity)[0x40], entity->nField_50, 0, 0);
        }
    } else {
        iVar1 = entity->nSubState;
        if (iVar1 == 0) {
            do {
                iVar1 = (*(code *)(*(int *)entity + 0x134))(entity, entity->pChild2, 0);
                iVar3 = entity->nSubState;
                entity->pChild2 = iVar1;
                entity->nSubState = iVar3 + 1;
            } while (iVar3 + 1 < 0x18);
            iVar1 = entity->nSubState;
        }
    }
    if (0x18 < iVar1) {
        (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffffec, 0);
        (*(code *)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer, 1);
    }
    if (entity->nSubType == 0x32) {
        (*(code *)(*(int *)entity + 0x30))(entity, 10);
    } else if (entity->nSubType == 0xc) {
        state->nReturn = 0;
        state->nDisp = 0x15;
    }
    (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field1E4);
}
void EntityBehavior_SubStateAnim(int *obj)
{
    int iVar1;
    if ((obj[0x3f] == 0) && (iVar1 = rand(), iVar1 == (iVar1 / 3) * 3)) {
        (*(code *)(*obj + 0x48))(obj, 1, &Entity_Field1D8);
        (*(code *)(*obj + 0xcc))(obj, 0xfffffed4, 0);
        (*(code *)(*obj + 0x44))(obj, 1, &Entity_Field88);
        obj[0x11] = 0xb;
    }
    if (obj[0x11] == 0xb) {
        if (obj[0x3f] == 2000) {
            (*(code *)(*obj + 0x44))(obj, 0, &Entity_Field94);
        }
        (*(code *)(*obj + 0xc4))(obj, 0xffffffec, 0);
    }
}
void EntityBehavior_TriggerOnDelay(EntityObj *entity, EvyMsg *state)
{
    uint uVar1;
    if (entity->nSubType == 0x14) {
        state->nDisp = ENTITY_STATE_TRIGGER;
        state->nReturn = 0;
        state->nParam2 = 3;
    } else if (entity->nSubType == entity->nCondition) {
        (*(code *)(*(int *)entity + 0x16c))(entity);
        entity->nState = 1;
        uVar1 = rand();
        if ((uVar1 & 1) != 0) {
            (*(code *)(*(int *)entity + 0x30))(entity, 0xb);
        }
    }
}
void EntityBehavior_MultiMoveState(EntityObj *entity, EvyMsg *state)
{
    int iVar1;
    int uVar2;
    uint uVar3;
    code *pcVar4;
    if (entity->nSubType == 0) {
        iVar1 = rand();
        entity->nState = iVar1 % 3;
        if (iVar1 % 3 == 0) {
            (*(code *)(*(int *)entity + 0x130))(entity);
            (*(code *)(*(int *)entity + 0xcc))(entity, 0x1800, 0);
        }
    }
    uVar2 = (*(code *)(*(int *)entity + 0x148))(entity);
    state->nReturn = uVar2;
    if (entity->nState == 0) {
        state->nDisp = 0xc;
        state->nParam1 = 0xffffffff;
        (*(code *)(*(int *)entity + 0xc4))(entity, 0xfffffe00, 0);
        if (entity->nSubType - 0x80U < 0xc2) {
            (*(code *)(*(int *)entity + 0xcc))(entity, 0xffffff80, 0);
            return;
        }
        if (entity->nSubType != 0x142) {
            return;
        }
        pcVar4 = *(code **)(*(int *)entity + 300);
    } else {
        iVar1 = entity->nSubState;
        if (iVar1 < 0x1e) {
            state->nDisp = 0xc;
            state->nParam1 = 0xffffffff;
            return;
        }
        if (iVar1 == 0x1e) {
            state->nDisp = 0xfffffffe;
            return;
        }
        if (iVar1 == 0x23) {
            state->nParam4 = 0x16;
            state->nParam5 = 0xfffffffe;
            return;
        }
        if (iVar1 == 0x30) {
            iVar1 = EntityMoveToward(entity, entity->pData + 0x18, 0xf, 10);
            if (iVar1 == 0) {
                return;
            }
            iVar1 = EntityFindChildByType(entity, 0, 0, 10, 0);
            if (iVar1 != 0) {
                (*(code *)(*(int *)((int *)entity)[0x40] + 0xd4))((int *)((int *)entity)[0x40], entity->nField_50, 4, 0);
            }
            uVar3 = rand();
            if ((uVar3 & 1) == 0) {
                return;
            }
            (*(code *)(*(int *)entity + 0x30))(entity, 0xb);
            return;
        }
        if (iVar1 != 0x3b) {
            return;
        }
        pcVar4 = *(code **)(*(int *)entity + 0x160);
    }
    (*pcVar4)(entity);
    entity->nState = 1;
}
void EntityDeactivateState(int *obj)
{
    (*(code *)(*obj + 0x60))(obj, 0);
    (*(code *)(*obj + 0x16c))(obj);
    (*(code *)(*obj + 0x164))(obj, 0);
    obj[0x3c] = 0;
}
void EntityBehavior_Oscillate(int *obj)
{
    int iVar1;
    code *pcVar2;
    int iVar3;
    int *puVar4;
    int uVar5;
    int iVar6;
    iVar6 = obj[0x3f] % 500;
    uVar5 = 0x40;
    if (obj[0x3f] % 6 < 3) {
        uVar5 = 0xffffffc0;
    }
    (*(code *)(*obj + 0xcc))(obj, uVar5, 0);
    uVar5 = 0x40;
    if (obj[0x3f] % 0xc < 6) {
        uVar5 = 0xffffffc0;
    }
    (*(code *)(*obj + 200))(obj, uVar5, 0);
    iVar3 = obj[0x3f];
    iVar1 = iVar3;
    if (iVar3 < 0) {
        iVar1 = iVar3 + 0x3f;
    }
    uVar5 = 0x80;
    if (iVar3 + (iVar1 >> 6) * -0x40 < 0x20) {
        uVar5 = 0xffffff80;
    }
    (*(code *)(*obj + 0xc4))(obj, uVar5, 0);
    if (iVar6 < 0x20) {
        pcVar2 = *(code **)(*obj + 0xbc);
        puVar4 = &Entity_Field178;
    } else {
        if (0x3f < iVar6) {
            return;
        }
        pcVar2 = *(code **)(*obj + 0xbc);
        puVar4 = &LAB_80089d60;
    }
    (*pcVar2)(obj, puVar4);
}
void EntityUpdateSubState(int *obj)
{
    int iVar1;
    if ((((char)(&Entity_Field2AB)[obj[0x26] * 0x10] < '\0') && ((char)(&Entity_Field2AC)[obj[0x26] * 0x10] != 0)) && (iVar1 = EntityCheckTimerThreshold(obj, (int)(char)(&Entity_Field2AC)[obj[0x26] * 0x10] << 9), iVar1 != 0)) {
        (*(code *)(*obj + 0x30))(obj, 10);
    }
}
void EntityBehavior_RollAtBounds(int *obj)
{
    code *pcVar1;
    int uVar2;
    uVar2 = 3;
    if (obj[0x3f] == 0) {
        pcVar1 = *(code **)(*obj + 0x128);
        (*pcVar1)(obj, uVar2);
    } else if (obj[0x3f] == obj[0x20]) {
        pcVar1 = *(code **)(*obj + 0x128);
        uVar2 = 1;
        (*pcVar1)(obj, uVar2);
    }
    if (obj[0x1f] == 1) {
        (*(code *)(*obj + 0xc4))(obj, 0xffffff80, 0);
    }
}
void EntityBehavior_InitQuick(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    uVar1 = (*(code *)(*(int *)entity + 0x148))();
    state->nReturn = uVar1;
    if (state->nCounter == 0) {
        state->nDisp = 0x17;
    }
}
void EntityBehavior_ThresholdRangeReset(EntityObj *entity, EvyMsg *state, int threshold, int endThreshold, int offset)
{
    int iVar1;
    state->nReturn = 0;
    if (state->nCounter == 6) {
        state->nDisp = 4;
        state->nParam2 = 4;
        state->nParam4 = 4;
    }
    iVar1 = entity->nSubType;
    if ((((threshold <= iVar1) && (iVar1 <= threshold + 0x5b)) || ((threshold + 0x155 <= iVar1 && (iVar1 <= threshold + 0x1b1)))) || ((threshold + 0x2ba <= iVar1 && (iVar1 <= threshold + 0x317)))) {
        (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field118);
    }
    (*(code *)(*(int *)entity + 0xc4))(entity, offset, 0);
    if (entity->nSubType == endThreshold) {
        (*(code *)(*(int *)entity + 0x160))(entity);
        entity->nState = 1;
    }
}
void EntityBehavior_MidCycleAnim(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    int iVar2;
    uVar1 = (*(code *)(*(int *)entity + 0x148))();
    state->nReturn = uVar1;
    if (entity->nSubState == entity->nCondition / 2) {
        state->nDisp = 7;
        state->nParam1 = 0xfffffffe;
        state->nParam2 = 3;
        state->nParam3 = 0xfffffffe;
    }
    if (state->nCounter % 0x5a < 3) {
        state->nParam4 = 6;
        state->nParam5 = 0xffffffff;
    }
    if (entity->nSubType < 0x79) {
        if ((0x37 < entity->nSubType) || (iVar2 = EntityMoveToward(entity, entity->pData + 0x18, 1, 1), iVar2 != 0)) {
            (*(code *)(*(int *)entity + 0xbc))(entity, &Entity_Field178);
            return;
        }
        if (entity->nSubType < 10) {
            MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
            return;
        }
        MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
        iVar2 = *(int *)entity;
        uVar1 = 0xffffff00;
    } else {
        (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_FieldA0);
        iVar2 = *(int *)entity;
        uVar1 = 0xfffffec0;
    }
    (*(code *)(iVar2 + 0xc4))(entity, uVar1, 0);
}
void EntitySetInitVelocity(int *obj)
{
    if (obj[0x3f] == 0) {
        (*(code *)(*obj + 0xcc))(obj, 0xffffff38, 0);
    }
}
void EntityBehavior_PeriodicStateAdvance(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    int iVar2;
    int iVar3;
    uVar1 = (*(code *)(*(int *)entity + 0x148))();
    state->nReturn = uVar1;
    iVar2 = entity->nCondition / 2;
    iVar3 = state->nCounter % iVar2;
    if (iVar2 == 0) {
        trap(TRAP_DIVZERO);
    }
    if ((iVar2 == -1) && (state->nCounter == -0x80000000)) {
        trap(TRAP_OVERFLOW);
    }
    if (iVar3 == 0) {
        state->nDisp = 10;
    } else if (iVar3 == 3) {
        state->nParam2 = 0xd;
    }
    (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffffe2, 1);
}
void EntityBehavior_RandomDirState(EntityObj *entity, EvyMsg *state)
{
    int iVar1;
    int uVar2;
    uint uVar3;
    if ((entity->nSubType == 0) && (iVar1 = rand(), iVar1 == (iVar1 / 10) * 10)) {
        entity->nState = 0xc;
    }
    if (state->nCounter == (state->nCounter / 10) * 10) {
        uVar2 = (*(code *)(*(int *)entity + 0x148))(entity);
        state->nReturn = uVar2;
        state->nDisp = 0xc;
        state->nParam1 = 0xffffffff;
    }
    if ((entity->nSubType == 0) && (uVar3 = rand(), (uVar3 & 1) != 0)) {
        (*(code *)(*(int *)entity + 0xcc))(entity, DEGREES_180, 0);
    }
    (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffff80, 0);
    if ((entity->nState == 0xc) && (entity->nSubType == 300)) {
        (*(code *)(*(int *)entity->pObject + 0x138))((int *)entity->pObject, 1, 1);
    }
}
void EntityBehavior_PhaseEnd(EntityObj *entity, EvyMsg *state)
{
    code *pcVar1;
    if (entity->nSubType < 10) {
        pcVar1 = *(code **)(*(int *)entity + 0x130);
        (*pcVar1)(entity);
    } else if (entity->nSubType == 10) {
        pcVar1 = *(code **)(*(int *)entity + 300);
        (*pcVar1)(entity);
    }
    if (entity->nSubState == 10) {
        EntityState_SetTrigger(state);
    }
    if (entity->nSubType == entity->nCondition + 10) {
        (*(code *)(*(int *)entity + 0x16c))(entity);
        entity->nState = 1;
    }
}
void EntityBehavior_RandomBranchState(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    uint uVar2;
    uVar1 = (*(code *)(*(int *)entity + 0x148))();
    state->nReturn = uVar1;
    if (state->nCounter == 0) {
        uVar2 = rand();
        uVar1 = 1;
        if ((uVar2 & 1) != 0) {
            uVar1 = 2;
        }
        state->nDisp = uVar1;
        state->nParam1 = 2;
    }
}
void EntityBehavior_SetVelocitySimple(int *obj)
{
    (*(code *)(*obj + 0xc4))(obj, 0xffffffe2, 0);
}
void EntityBehavior_ProximityMultiState(EntityObj *entity, EvyMsg *state)
{
    int iVar1;
    int uVar2;
    code *pcVar3;
    int *piVar4;
    if (entity->nSubType == 0) {
        state->nReturn = 0;
        state->nDisp = 0xc;
        iVar1 = (*(code *)(*(int *)entity->pContainer + 0x200))();
        if (iVar1 == 6) {
            entity->nState = 0xb;
        } else {
            iVar1 = rand();
            if (iVar1 == (iVar1 / 3) * 3) {
                entity->nState = 0xc;
            }
        }
    }
    if (state->nCounter == (state->nCounter / 100) * 100) {
        uVar2 = (*(code *)(*(int *)entity + 0x148))(entity);
        state->nReturn = uVar2;
        state->nDisp = 0xc;
        state->nParam1 = 0xffffffff;
    }
    if (entity->nState == 0xb) {
        iVar1 = (*(code *)(*(int *)entity + 0x144))(entity, entity->pContainer);
        if (iVar1 < DEGREES_90) {
            (*(code *)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer, 0);
            entity->nState = 0xd;
            entity->nSubType = 0;
        }
    } else if ((entity->nState == 0xc) && (iVar1 = (*(code *)(*(int *)entity + 0x144))(entity, entity->pContainer), iVar1 < DEGREES_90)) {
        (*(code *)(*(int *)entity + 0x130))(entity);
        entity->nState = 0xe;
        entity->nSubType = 0;
    }
    iVar1 = entity->nState;
    if (iVar1 == 0xd) {
        iVar1 = entity->nSubType;
        uVar2 = 0xffffffec;
        if (iVar1 < 0x32) {
            piVar4 = (int *)entity->pContainer;
            pcVar3 = *(code **)(*piVar4 + 0xcc);
            (*pcVar3)(piVar4, uVar2, 0);
        } else {
            if (iVar1 < 500) {
                piVar4 = (int *)entity->pContainer;
                uVar2 = 5;
                if (iVar1 % 0x28 < 0x14) {
                    uVar2 = 0xfffffffb;
                }
                pcVar3 = *(code **)(*piVar4 + 200);
                (*pcVar3)(piVar4, uVar2, 0);
            }
            if (iVar1 == 500) {
                (*(code *)(*(int *)entity + 0x30))(entity, 0xc);
            }
        }
    }
    if (entity->nState == 0xe) {
        if (entity->nSubType < 10) {
            (*(code *)(*(int *)entity + 0xcc))(entity, 200, 0);
        } else if (entity->nSubType == 10) {
            state->nDisp = ENTITY_STATE_TRIGGER;
            state->nReturn = 0;
            state->nParam2 = 3;
            (*(code *)(*(int *)entity + 0x44))(entity, 1, &Entity_Field124);
            (*(code *)(*(int *)entity + 200))(entity, 0x960, 0);
            (*(code *)(*(int *)entity + 0xcc))(entity, 0x5dc, 0);
            (*(code *)(**(int **)(entity->nChildCount + 4) + 0x60))(*(int **)(entity->nChildCount + 4), 0);
            entity->nState = 1;
        }
    }
}
void EntityBehavior_StateFlagAnim(EntityObj *entity, EvyMsg *state)
{
    if (entity->nResultFlag == 0) {
        if (state->nCounter == (state->nCounter / 0x1e) * 0x1e) {
            state->nReturn = 0;
            state->nDisp = 3;
        }
    } else {
        (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field1CC);
    }
    (*(code *)(*(int *)entity + 0xd0))(entity, 0xffffffe2, 0);
    if (entity->nConfig != 0) {
        (*(code *)(*(int *)entity + 0xcc))(entity, 0xffffff38, 0);
    }
}
void EntityActivateState(int *obj)
{
    (*(code *)(*obj + 0x60))(obj, 1);
    obj[0x3c] = 1;
    obj[9] = 0;
}
void EntityBehavior_PlayAnim238A(int *obj)
{
    (*(code *)(*obj + 0x48))(obj, 1, &Entity_Field238);
}
void EntityBehavior_StateOnMatch(EntityObj *entity, EvyMsg *state)
{
    if (entity->nSubType < entity->nCondition * 5) {
        if ((entity->nSubState == 0xf) || (entity->nSubState == 0x46)) {
            state->nReturn = 0;
            state->nDisp = 7;
            state->nParam2 = 7;
            state->nParam4 = 7;
        }
    } else {
        (*(code *)(*(int *)entity + 0x160))(entity);
        entity->nState = 1;
    }
}
void EntityBehavior_Nop2(void)
{
}
int EntityCheckTimerThreshold(EntityObj *entity, int threshold)
{
    int uVar1;
    int iVar2;
    iVar2 = *(int *)(*(int *)(entity->pContainer + 0x14) + 0x1c);
    uVar1 = 0;
    if ((*(int *)(entity->pData + 0x1c) <= iVar2 + 0x200) && (uVar1 = 0, iVar2 + -0x200 <= *(int *)(entity->pData + 0x1c))) {
        iVar2 = (*(code *)(*(int *)entity + 0x144))();
        uVar1 = 1;
        if (threshold <= iVar2) {
            uVar1 = 0;
        }
    }
    return uVar1;
}
void EntityBehavior_RandomMoveAnim(int *obj)
{
    uint uVar1;
    int iVar2;
    int *puVar3;
    if ((obj[0x3f] == 0) && (uVar1 = rand(), (uVar1 & 1) != 0)) {
        obj[0x11] = 0xb;
    }
    if (obj[0x11] == 0) {
        if (obj[0x3f] < 0x40) {
            (*(code *)(*obj + 0xc4))(obj, 0xffffffa6, 0);
        } else if (obj[0x3f] == 0x40) {
            uVar1 = rand();
            puVar3 = &Entity_Field94;
            if ((uVar1 & 1) != 0) {
                puVar3 = &Entity_Field88;
            }
            (*(code *)(*obj + 0x44))(obj, 0, puVar3);
            (*(code *)(*obj + 0xbc))(obj, &Entity_Field13C);
        } else {
            iVar2 = rand();
            (*(code *)(*obj + 0xd0))(obj, 0xfffffe8a, iVar2 % 2);
        }
    } else if (obj[0x11] == 0xb) {
        if (obj[0x3f] == (obj[0x3f] / 5) * 5) {
            (*(code *)(*obj + 0x44))(obj, 0, &Entity_Field88);
        }
        (*(code *)(*obj + 0xc4))(obj, 0xfffff800, 0);
        iVar2 = rand();
        (*(code *)(*obj + 0x60))(obj, iVar2 % 7 == 0);
    }
}
void EntityBehavior_WanderMultiPhase(EntityObj *entity, EvyMsg *state)
{
    int iVar1;
    int uVar2;
    code *pcVar3;
    int *puVar4;
    if ((state->nCounter == 0) && (iVar1 = (*(code *)(*(int *)entity->pContainer + 0x200))(), iVar1 == 5)) {
        entity->nState = 100;
    }
    uVar2 = (*(code *)(*(int *)entity + 0x148))(entity);
    state->nReturn = uVar2;
    if (entity->nState == 0) {
        if (state->nCounter == (state->nCounter / 10) * 10) {
            state->nDisp = 5;
            state->nParam1 = 0xfffffffe;
        }
        if (entity->nSubType == 0x960) {
            entity->nSubType = -1;
            return;
        }
        if (entity->nSubType < 0x4b0) {
            uVar2 = 0x32;
            pcVar3 = *(code **)(*(int *)entity + 0xc4);
            puVar4 = 0;
        } else {
            uVar2 = 0xffffffce;
            pcVar3 = *(code **)(*(int *)entity + 0xc4);
            puVar4 = 0;
        }
    } else {
        iVar1 = entity->nSubType;
        if (iVar1 < 0xfa) {
            if (state->nCounter == (state->nCounter / 10) * 10) {
                state->nDisp = 5;
                state->nParam1 = 0xfffffffe;
            }
            if (99 < entity->nSubType) {
                if (0xf9 < entity->nSubType) {
                    return;
                }
                (*(code *)(*(int *)entity + 0xbc))(entity, &Entity_Field1A8);
                return;
            }
            uVar2 = 0x32;
            pcVar3 = *(code **)(*(int *)entity + 0xc4);
            puVar4 = 0;
        } else {
            if (iVar1 == 0xfa) {
                (*(code *)(*(int *)entity + 0x130))(entity);
                state->nDisp = 0xfffffffe;
                return;
            }
            if (iVar1 - 0x105U < 0x133) {
                (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffffce, 0);
                pcVar3 = *(code **)(*(int *)entity + 0x44);
                puVar4 = &Entity_FieldE8;
                uVar2 = 1;
            } else {
                uVar2 = 1;
                if (iVar1 < 0x239) {
                    return;
                }
                pcVar3 = *(code **)(*(int *)entity + 0x44);
                puVar4 = &Entity_FieldF4;
            }
        }
    }
    (*pcVar3)(entity, uVar2, puVar4);
}
void EntityBehavior_InitApproach(EntityObj *entity, EvyMsg *state)
{
    state->nReturn = 0;
    if (state->nCounter == 0) {
        state->nDisp = ENTITY_STATE_ACTIVE;
        state->nParam2 = ENTITY_STATE_ACTIVE;
        state->nParam4 = ENTITY_STATE_ACTIVE;
        (*(code *)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer, 1);
    }
    MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
    (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffffa6, 0);
    if (entity->nSubType == 0x1e) {
        (*(code *)(*(int *)entity + 0x30))(entity, 10);
    }
}
void EntitySetChildState(EntityObj *entity, int state)
{
    if (state != 0) {
        (*(code *)(*(int *)entity + 0x30))(entity, 9);
    }
    entity->nResultFlag = state;
}
void EntityBehavior_PeriodicStateD(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    uVar1 = (*(code *)(*(int *)entity + 0x148))();
    state->nReturn = uVar1;
    if (state->nCounter == (state->nCounter / 0x1e) * 0x1e) {
        state->nDisp = 0xd;
    }
}
void EntityBehavior_AnimOnFlag(int *obj)
{
    code *pcVar1;
    int *puVar2;
    if (obj[0x3d] == 0) {
        (*(code *)(*obj + 0x130))(obj);
        pcVar1 = *(code **)(*obj + 0x44);
        puVar2 = &Entity_Field64;
    } else {
        (*(code *)(*obj + 300))();
        if (obj[0x21] != obj[0x20] + -1) {
            return;
        }
        (*(code *)(*obj + 0x130))(obj);
        pcVar1 = *(code **)(*obj + 0x48);
        puVar2 = &Entity_Field1FC;
    }
    (*pcVar1)(obj, 0, puVar2);
}
void EntityState_SetTrigger(EvyMsg *state)
{
    state->nDisp = ENTITY_STATE_TRIGGER;
    state->nReturn = 0;
    state->nParam2 = 3;
    state->nParam4 = 3;
}
void EntityBehavior_ProximitySubstate(int *obj)
{
    int iVar1;
    (*(code *)(*obj + 0x48))(obj, 1, &Entity_Field244);
    (*(code *)(*obj + 0x130))(obj);
    if ((obj[0x11] == 0) && (iVar1 = (*(code *)(*obj + 0x144))(obj, obj[0x25]), iVar1 < DEGREES_180)) {
        obj[0x11] = 10;
        obj[0x3f] = 0;
    }
    if (obj[0x11] == 10) {
        if (obj[0x3f] < 0x2d) {
            (*(code *)(*obj + 0x44))(obj, 0, &Entity_Field100);
        }
        if (500 < obj[0x3f]) {
            obj[0x11] = 0;
        }
    }
}
void EntityBehavior_PeriodicAnimFlow(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    if (state->nCounter == (state->nCounter / 0xf) * 0xf) {
        uVar1 = (*(code *)(*(int *)entity + 0x148))(entity);
        state->nReturn = uVar1;
        state->nDisp = 7;
        state->nParam1 = 0xfffffffe;
    }
    (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field1F0);
    (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_FieldA0);
    (*(code *)(*(int *)entity + 0xc4))(entity, 0xfffffe00, 0);
}
void EntityBehavior_PlayAnim238B(int *obj)
{
    (*(code *)(*obj + 0x48))(obj, 1, &Entity_Field238);
}
void EntityBehavior_AnimateThenReset(int *obj)
{
    int iVar1;
    if ((obj[0x3f] == 0) && (iVar1 = rand(), iVar1 == (iVar1 / 5) * 5)) {
        obj[0x11] = 0xb;
    }
    (*(code *)(*obj + 0x130))(obj);
    (*(code *)(*obj + 0xc4))(obj, 100, 0);
    if (obj[0x3f] == 1000) {
        (*(code *)(*obj + 0x16c))(obj);
        obj[0x11] = 1;
    }
    if ((obj[0x11] == 0xb) && (300 < obj[0x3f])) {
        (*(code *)(*(int *)obj[0x25] + 0x94))((int *)obj[0x25], 0, 2);
        (*(code *)(*(int *)obj[0x25] + 0x94))((int *)obj[0x25], 0, 7);
    }
}
void EntityBehavior_ModuloState(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    int iVar2;
    iVar2 = state->nCounter % 300;
    uVar1 = (*(code *)(*(int *)entity + 0x148))();
    state->nReturn = uVar1;
    if (iVar2 < 0x14) {
        state->nDisp = 5;
        state->nParam1 = 0xfffffffe;
    } else if (iVar2 == 0x16) {
        state->nDisp = 0xfffffffe;
    }
    (*(code *)(*(int *)entity + 0xc4))(entity, 0xfffffff6, 0);
}
void EntityBehavior_AnimPhaseState(EntityObj *entity, EvyMsg *state)
{
    int iVar2;
    iVar2 = (*(code *)(*(int *)entity + 0x148))();
    state->nReturn = iVar2;
    iVar2 = entity->nSubType;
    if (iVar2 < 0xbc) {
        if (iVar2 == 0x54)
            (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field7C);
        if (state->nCounter % 0x14 == 0)
            state->nDisp = 9;
    } else if (iVar2 < 200) {
        (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field64);
    } else {
        (*(code *)(*(int *)entity + 0x160))(entity);
        state->nParam2 = 0x1e;
        entity->nState = 1;
    }
    (*(code *)(*(int *)entity + 0xd0))(entity, 0xfffffe00, 0);
}
void EntityBehavior_MidpointTrigger(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    uVar1 = (*(code *)(*(int *)entity + 0x148))();
    state->nReturn = uVar1;
    if (state->nCounter == entity->nCondition / 2)
        state->nDisp = ENTITY_STATE_TRIGGER;
    if (entity->nCondition - 1 <= state->nCounter)
        state->nCounter = -1;
}
void EntityBehavior_RandomSubstate(EntityObj *entity)
{
    int iVar1;
    if (entity->nSubType == 0) {
        iVar1 = rand();
        entity->nState = iVar1 % 2 + 10;
    }
    (*(code *)(*(int *)entity + 0x130))(entity);
    if (200 < entity->nSubType) {
        MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
        if (entity->nState == 10)
            (*(code *)(*(int *)entity + 0xd0))(entity, 0xfffffe00, 0);
    }
}
void EntityBehavior_AnimAndMove(EntityObj *entity)
{
    (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field1D8);
    (*(code *)(*(int *)entity + 0xc4))(entity, 0xfffffff6, 0);
}
void EntityBehavior_TimedAnimState(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    uint uVar2;
    int iVar3;
    uVar1 = (*(code *)(*(int *)entity + 0x148))();
    state->nReturn = uVar1;
    if (state->nCounter % 5 == 0) {
        state->nDisp = 0x11;
        state->nParam1 = -2;
    }
    if (entity->pSubObj == 0) {
        if (entity->nSubType == entity->nCondition) {
            (*(code *)(*(int *)entity + 0x128))(entity, 1);
            uVar2 = rand();
            if ((uVar2 & 1) != 0)
                entity->nState = 0xb;
        }
    } else if (entity->nState == 0) {
        iVar3 = entity->nSubType;
        if ((((iVar3 == 0x3c) || (iVar3 == 0xd4)) || (iVar3 == 0x122)) || (iVar3 == 0x140)) {
            (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field88);
            iVar3 = entity->nSubType;
        }
        if (iVar3 == 0x18e)
            (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field94);
        (*(code *)(*(int *)entity + 0xd0))(entity, 0xffffffce, 0);
    } else {
        if ((entity->nSubType == 0x3c) || (entity->nSubType == 0x8c))
            (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field88);
        if (entity->nSubType < 0xae)
            (*(code *)(*(int *)entity + 0xd0))(entity, 0xffffffce, 0);
        if (entity->nSubType == 0xae) {
            (*(code *)(*(int *)entity + 0x16c))(entity);
            entity->nState = 1;
        }
    }
}
void EntityBehavior_RandomAnimMove(EntityObj *entity, EvyMsg *state)
{
    int uVar1, iVar4;
    uint uVar3;
    EntityBehavior_RandomAnim(0);
    uVar1 = (*(code *)(*(int *)entity + 0x148))(entity);
    state->nReturn = uVar1;
    if ((entity->nSubState == 7) || (entity->nSubState == 0x16))
        state->nDisp = 3;
    iVar4 = entity->nSubType;
    if (iVar4 - 300U < 0x14) {
        (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffffc4, 0);
    } else if (iVar4 - 0x141U < 0x13) {
        (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field70);
    } else {
        if (iVar4 < 0x141)
            return;
        uVar3 = rand();
        uVar1 = 0xffffff80;
        if ((uVar3 & 1) != 0)
            uVar1 = 0x80;
        (*(code *)(*(int *)entity + 200))(entity, uVar1, 1);
        uVar3 = rand();
        if ((uVar3 & 3) != 0)
            (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field70);
        else
            (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field64);
    }
}
void EntityBehavior_ChildInteraction(EntityObj *entity)
{
    int iVar1;
    if (entity->nResultFlag != 0) {
        iVar1 = entity->nState;
        if (iVar1 == 0) {
            entity->nState = 0xc;
            entity->nSubType = 0;
        } else if (iVar1 == 0xc) {
            if (entity->nSubType < 0x1e) {
                iVar1 = (*(code *)(*(int *)entity->pContainer + 0x100))();
                if (iVar1 != 0) {
                    (*(code *)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer, 0);
                    entity->nSubType = 0;
                    entity->nState = 0xb;
                }
            } else {
                (*(code *)(*(int *)entity + 0x30))(entity, 0xb);
                entity->nState = 10;
            }
        } else if (iVar1 == 0xb) {
            if (entity->nSubType == 100)
                (*(code *)(*(int *)entity + 0x30))(entity, 0xc);
            else
                (*(code *)(*(int *)entity->pContainer + 0xcc))((int *)entity->pContainer, 0xffffff9c, 0);
        }
    }
}
int EntityUpdateDirection(EntityObj *entity)
{
    int iVar1, iVar2;
    uint uVar3;
    iVar1 = entity->nAnimIdx * 0x10;
    if (entity->nActiveFlag != 0) {
        if (entity->nResultFlag == 0) {
            uVar3 = (uint)(char)(&Entity_Field2AA)[iVar1];
            if ((int)uVar3 < 0)
                uVar3 = ~uVar3 + 1;
            iVar2 = EntityMoveToward(entity, entity->pData + 0x18, uVar3, (int)(char)(&Entity_Field2AD)[iVar1]);
            if (iVar2 != 0)
                (*(code *)(*(int *)entity + 0x164))(entity, 1);
        }
        if ((char)(&Entity_Field2AA)[iVar1] < '\0')
            MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
    }
    return entity->nResultFlag;
}
void EntityBehavior_TimedChildOp(EntityObj *entity)
{
    int iVar1;
    if ((entity->nSubType == 700) && (iVar1 = rand(), iVar1 % 3 == 0))
        entity->nState = 0xb;
    if (entity->nState == 0xb) {
        if (entity->nSubType < 0x3fc) {
            (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_FieldB8);
            (*(code *)(*(int *)entity + 0xcc))(entity, 0x1e, 0);
        }
        if (entity->nSubType == 0x3a2)
            (*(code *)(*(int *)entity + 0x30))(entity, 10);
    } else if (((entity->nSubType == 100) || (entity->nSubType == 800)) &&
               (iVar1 = rand(), iVar1 % 5 == 0)) {
        (*(code *)(*(int *)entity->pObject + 0x138))((int *)entity->pObject, 4, 0);
    }
    (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffffe2, 0);
}
void EntityBehavior_MultiDirState(EntityObj *entity, EvyMsg *state)
{
    uint uVar1;
    int iVar2;
    int uVar3;
    if ((entity->nSubType == 0) && (uVar1 = rand(), (uVar1 & 1) != 0)) {
        iVar2 = rand();
        entity->nState = iVar2 % 3;
    }
    if ((entity->nState == 0) || (entity->nResultFlag == 0)) {
        uVar3 = (*(code *)(*(int *)entity + 0x148))(entity);
        state->nReturn = uVar3;
        if (state->nCounter % 0x16 == 0)
            state->nDisp = ENTITY_STATE_FLAG_MASK;
        if (500 < entity->nSubType)
            MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
        iVar2 = (*(code *)(*(int *)entity + 0x144))(entity, entity->pContainer);
        uVar3 = 0xffffffec;
        if (iVar2 < DEGREES_180)
            (*(code *)(*(int *)entity->pContainer + 0xc4))((int *)entity->pContainer, 0xfffff800, 0);
    } else {
        if (0x3c < state->nCounter)
            state->nCounter = 0;
        iVar2 = state->nCounter % 0x14;
        if (iVar2 == 0) {
            state->nDisp = 0x17;
            state->nParam2 = 0x17;
            state->nParam4 = 0x17;
            state->nReturn = 0;
            state->nParam1 = -1;
            state->nParam3 = -1;
            state->nParam5 = -2;
        } else if (iVar2 == 0xe) {
            state->nDisp = -2;
            state->nParam2 = -2;
            state->nParam4 = -2;
        }
        MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
        if (entity->nState == 1) {
            uVar3 = 0xfffffe8a;
            (*(code *)(*(int *)entity + 0x48))(entity, 0, &Entity_Field208);
            iVar2 = (*(code *)(*(int *)entity + 0x144))(entity, entity->pContainer);
            if (iVar2 < 0x200) {
                (*(code *)(*(int *)entity + 0x160))(entity);
                entity->nState = 1;
            }
        } else {
            (*(code *)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer, 1);
            MathComputeAngleToTarget(entity->pContainer, entity, 1, 1, 0);
            uVar3 = 0;
            if (entity->nState == 2) {
                iVar2 = (*(code *)(*(int *)entity + 0x144))(entity, entity->pContainer);
                uVar3 = 0xffffffa0;
                if (iVar2 < 0x960) {
                    entity->nState = 0xb;
                    (*(code *)(*(int *)entity + 0x30))(entity, 0xc);
                }
            }
        }
    }
    (*(code *)(*(int *)entity + 0xd0))(entity, uVar3, 1);
    if (entity->nConfig != 0)
        (*(code *)(*(int *)entity + 0xcc))(entity, 0xffffff38, 0);
}
void EntityBehavior_ChaseTarget(EntityObj *entity)
{
    uint uVar1;
    if (entity->nState == 0) {
        uVar1 = rand();
        if ((uVar1 & 1) != 0)
            (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field1F0);
        else
            (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field238);
        entity->nState = 0xb;
    }
    MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
    if ((*(code *)(*(int *)entity + 0x144))(entity, entity->pContainer) < 0x7000)
        (*(code *)(*(int *)entity + 0xc4))(entity, 0x100, 0);
}
void EntityBehavior_DirectionState(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    uint uVar2;
    int iVar3, iVar5;
    uVar1 = (*(code *)(*(int *)entity + 0x148))();
    state->nReturn = uVar1;
    if (state->nCounter == 0) {
        uVar2 = rand();
        if ((uVar2 & 1) != 0)
            *(short *)(entity + 0x12) = (short)0xfe8a;
        else
            *(short *)(entity + 0x12) = (short)0xff40;
    }
    if (entity->nState == 0) {
        iVar5 = state->nCounter;
        if (state->nCounter % 10 == 0) {
            state->nDisp = ENTITY_STATE_FLAG_MASK;
            iVar5 = state->nCounter;
        }
        iVar3 = iVar5 % 0x14;
        if (iVar3 == 0) {
            state->nParam2 = 0x17;
            state->nParam3 = -1;
            state->nParam4 = 0x17;
            state->nParam5 = -1;
        } else if (iVar3 == 0xe) {
            state->nParam2 = -2;
            state->nParam4 = -2;
        }
        if (((entity->nSubType & 1U) == 0) &&
           (iVar3 = (*(code *)(*(int *)entity->pContainer + 0x100))(), iVar3 != 0)) {
            entity->nSubType = -1;
            entity->nState = 10;
            state->nDisp = ENTITY_STATE_TRIGGER;
        }
        MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
        (*(code *)(*(int *)entity + 0xc4))(entity, (int)(short)entity->nStateFlag, 1);
    } else if (entity->nState == 10) {
        if (entity->nSubType < 8) {
            (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_FieldC4);
            (*(code *)(*(int *)entity + 0xbc))(entity, &LAB_80089d6c);
        } else {
            state->nDisp = ENTITY_STATE_TRIGGER;
            state->nParam2 = 3;
            (*(code *)(*(int *)entity + 0x16c))(entity);
            uVar2 = rand();
            entity->nState = (uint)((uVar2 & 1) == 0);
        }
    }
}
void EntityBehavior_RandomAnim(EntityObj *entity)
{
    int iVar1;
    if (entity->nSubType == 0) {
        iVar1 = rand();
        if (iVar1 % 10 < 8) {
            if (4 < iVar1 % 10)
                entity->nState = 10;
        } else {
            (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field28C);
        }
    }
    if ((entity->nState == 10) && (200 < entity->nSubType))
        (*(code *)(*(int *)entity + 0xbc))(entity, &Entity_Field1C0);
}
void EntityBehavior_OscillateMove(EntityObj *entity)
{
    uint uVar1;
    if ((entity->nSubType == 0) && (uVar1 = rand(), (uVar1 & 1) == 0))
        entity->nState = 0xb;
    if (entity->nState == 0xb) {
        if (entity->nSubType == 0)
            (*(code *)(*(int *)entity + 0x130))(entity);
        (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field22C);
    } else {
        if (entity->nSubType == 0)
            (*(code *)(*(int *)entity + 0x128))(entity, 1);
        if (entity->nSubType % 0x14 < 10)
            (*(code *)(*(int *)entity + 200))(entity, 0x20, 0);
        else
            (*(code *)(*(int *)entity + 200))(entity, 0xffffffe0, 0);
    }
}
void EntityBehavior_PlayAnim(EntityObj *entity)
{
    (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field1D8);
}
void EntityBehavior_PeriodicReset(EntityObj *entity, EvyMsg *state)
{
    if (entity->nSubState % 0xf == 0) {
        state->nDisp = 0xc;
        state->nReturn = 0;
        state->nParam1 = 2;
    }
    if (entity->nSubType == entity->nCondition) {
        state->nDisp = -2;
        (*(code *)(*(int *)entity + 0x16c))(entity);
        entity->nState = 1;
    }
}
void EntityBehavior_DelayedAnim(EntityObj *entity)
{
    int iVar1;
    if ((entity->nSubType == 0) && (iVar1 = rand(), iVar1 % 3 == 0))
        entity->nState = 0xb;
    if (entity->nState == 0xb) {
        if (entity->nSubType == 0x1f6) {
            (*(code *)(*(int *)entity + 0xcc))(entity, DEGREES_180, 0);
            MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
        }
        if (500 < entity->nSubType)
            (*(code *)(*(int *)entity + 0xc4))(entity, 0xfffffe00, 0);
    }
}
void EntityBehavior_RandomAnimState(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    uint uVar2;
    EntityBehavior_RandomAnim(0);
    uVar1 = (*(code *)(*(int *)entity + 0x148))(entity);
    state->nReturn = uVar1;
    if ((entity->nSubState == 0) || (entity->nSubState == 0xf)) {
        state->nDisp = ENTITY_STATE_TRIGGER;
        state->nParam2 = ENTITY_STATE_TRIGGER;
    }
    if (0x140 < entity->nSubType) {
        uVar2 = rand();
        if ((uVar2 & 1) != 0)
            (*(code *)(*(int *)entity + 200))(entity, 0xffffffc4, 0);
        else
            (*(code *)(*(int *)entity + 200))(entity, 0x3c, 0);
        uVar2 = rand();
        if ((uVar2 & 3) != 0)
            (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field70);
        else
            (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field64);
    }
}
void EntityBehavior_TriggerAt30(EntityObj *entity, EvyMsg *state)
{
    if (entity->nSubState == 0x1e) {
        state->nDisp = ENTITY_STATE_TRIGGER;
        state->nReturn = 0;
        state->nParam1 = -1;
    }
}
void EntityBehavior_MultiPeriodState(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    uVar1 = (*(code *)(*(int *)entity + 0x148))();
    state->nReturn = uVar1;
    if (entity->nSubState == entity->nCondition - 1) {
        state->nDisp = 0x19;
        state->nParam1 = -2;
    }
    if ((state->nCounter & ENTITY_STATE_TYPE_MASK) == 0) {
        state->nParam2 = 0x15;
        state->nParam3 = -1;
    }
    if (state->nCounter % 200 == 0) {
        state->nParam4 = 0xd;
        state->nParam5 = 1;
    }
}
void EntityBehavior_ChildOpOnFlag(EntityObj *entity)
{
    int iVar1;
    if (entity->nResultFlag != 0) {
        iVar1 = EntityFindChildByType(entity, 0, 0, 10, 0);
        if (iVar1 != 0) {
            (*(code *)(*(int *)((int *)entity)[0x40] + 0xd4))((int *)((int *)entity)[0x40], entity->nField_50, 7, 0);
            (*(code *)(*(int *)entity + 0x160))(entity);
            (*(code *)(*(int *)entity->pContainer + 0x21c))();
        }
    }
    (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffffe2, 1);
}
void EntityBehavior_TimedAnimSwitch(EntityObj *entity, EvyMsg *state)
{
    int iVar1, uVar2;
    uint uVar3;
    if (((entity->nSubType == 0) && (iVar1 = rand(), iVar1 % 5 == 0)) && (entity->nState == 0)) {
        (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field238);
        (*(code *)(*(int *)entity + 0xcc))(entity, 800, 0);
        entity->nState = 0xb;
    }
    if (state->nCounter % 5 == 0) {
        uVar2 = (*(code *)(*(int *)entity + 0x148))(entity);
        state->nReturn = uVar2;
        state->nDisp = 8;
    }
    iVar1 = entity->nSubType;
    if (iVar1 == 0x5a)
        (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field94);
    else if (iVar1 == 0xa0)
        (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field88);
    else if ((iVar1 == 0xdc) && (uVar3 = rand(), (uVar3 & 1) != 0))
        (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field7C);
    (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffffb0, 1);
}
void EntityBehavior_ProximityComplex(EntityObj *entity, EvyMsg *state)
{
    uint uVar1;
    int iVar2;
    if (entity->nSubState % 0x1e == 0) {
        state->nDisp = 3;
        state->nReturn = 0;
        state->nParam1 = -2;
    }
    if (100 < entity->nSubType)
        MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
    (*(code *)(*(int *)entity + 0xc4))(entity, 0xfffffffb, 0);
    if (entity->nSubType == 300) {
        iVar2 = (*(code *)(*(int *)entity + 0x144))(entity, entity->pContainer);
        if (iVar2 < FIXED_12)
            (*(code *)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer, 0);
    } else if (entity->nSubType == 500) {
        (*(code *)(*(int *)entity->pContainer + 0x134))((int *)entity->pContainer, 1, 1);
    }
    if ((entity->nState == 0) &&
       (iVar2 = (*(code *)(*(int *)entity + 0x144))(entity, entity->pContainer), iVar2 < DEGREES_90)) {
        uVar1 = rand();
        if ((uVar1 & 1) == 0) {
            entity->nState = 0xb;
        } else {
            state->nParam2 = 6;
            state->nReturn = 0;
            state->nParam3 = -1;
            uVar1 = rand();
            if ((uVar1 & 1) != 0)
                (*(code *)(*(int *)entity->pObject + 0x138))((int *)entity->pObject, -1, 0);
            entity->nState = 10;
            entity->nSubType = 0;
        }
    }
    if ((entity->nState == 10) && (entity->nSubType == 0x46)) {
        uVar1 = rand();
        if ((uVar1 & 1) != 0)
            (*(code *)(*(int *)entity + 0x30))(entity, 0xc);
        else
            (*(code *)(*(int *)entity + 0x30))(entity, 0xb);
    }
}
int EntityCheckActivity(EntityObj *entity)
{
    int iVar1;
    uint uVar2;
    if (entity->nActiveFlag != 0) {
        if (entity->nReturnVal != 0)
            return entity->nReturnVal;
        if (entity->nState != 1) {
            uVar2 = (uint)(char)(&Entity_Field2AF)[entity->nAnimIdx * 0x10];
            if (uVar2 != 0) {
                if ((int)uVar2 < 0)
                    uVar2 = ~uVar2 + 1;
                iVar1 = EntityMoveToward(entity, entity->pData + 0x18, uVar2,
                                      (int)(char)(&Entity_Field2AD)[entity->nAnimIdx * 0x10]);
                if (iVar1 != 0)
                    (*(code *)(*(int *)entity + 0x168))(entity);
            }
        }
    }
    return entity->nReturnVal;
}
void EntityBehavior_OscillateVelocity(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    if (entity->nSubState == 0x26) {
        MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
        uVar1 = (*(code *)(*(int *)entity + 0x148))(entity);
        state->nReturn = uVar1;
        state->nDisp = 6;
    }
    if (entity->nSubType % 10 < 5)
        uVar1 = 0xffffffe2;
    else
        uVar1 = 0x1e;
    (*(code *)(*(int *)entity + 0xcc))(entity, uVar1, 0);
    (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffffe2, 1);
}
void EntityBehavior_PeriodicSubstate(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    uVar1 = (*(code *)(*(int *)entity + 0x148))();
    state->nReturn = uVar1;
    if (state->nCounter % 10 == 0) {
        state->nDisp = 0x19;
        state->nParam1 = 2;
    }
    (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_FieldA0);
    if ((entity->nState == 0) && (entity->nResultFlag != 0)) {
        (*(code *)(*(int *)entity + 0x30))(entity, 0xb);
        entity->nState = 0xb;
    }
}
void EntityBehavior_InitTargetState(EntityObj *entity, EvyMsg *state)
{
    state->nReturn = 0;
    if (state->nCounter == 0) {
        (*(code *)(*(int *)entity->pContainer + 0x44))((int *)entity->pContainer, 1, &Entity_Field88);
        (*(code *)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer, 1);
        state->nDisp = 0x19;
        state->nParam2 = 0x19;
        state->nParam4 = 0x19;
    } else if (state->nCounter == 0x14) {
        state->nParam2 = 0xd;
    }
    if (entity->nSubType == entity->nCondition - 1)
        (*(code *)(*(int *)entity + 0x160))(entity);
}
void EntityBehavior_ComplexStateMove(EntityObj *entity, EvyMsg *state)
{
    int uVar1, iVar2;
    uint uVar3;
    *(short *)(entity + 0x12) = (short)0xffec;
    uVar1 = (*(code *)(*(int *)entity + 0x148))();
    state->nReturn = uVar1;
    iVar2 = entity->nCondition / 2;
    if (iVar2 == 0)
        trap(TRAP_DIVZERO);
    if ((iVar2 == -1) && (state->nCounter == -0x80000000))
        trap(TRAP_OVERFLOW);
    if (state->nCounter % iVar2 == 0) {
        state->nDisp = 10;
        state->nParam1 = 1;
    }
    if (entity->nState == 0xb) {
        iVar2 = entity->nSubType;
        if (iVar2 == 0xa8c)
            (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field94);
        if (iVar2 == 0xc6c)
            (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field88);
        if (iVar2 == 0xe10)
            (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field94);
        if ((iVar2 - 0xd5dU < 0x78) &&
           (iVar2 = (*(code *)(*(int *)entity->pContainer + 0x100))(), iVar2 != 0)) {
            entity->nSubType = 0;
            entity->nState = 0xd;
        }
    } else if (entity->nState == 0xc) {
        iVar2 = entity->nSubType;
        if (iVar2 == 0x7bc)
            (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field94);
    } else if (entity->nState == 0xd) {
        *(short *)(entity + 0x12) = (short)0xff88;
        MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
        (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field1D8);
        iVar2 = (*(code *)(*(int *)entity + 0x144))(entity, entity->pContainer);
        if (iVar2 < DEGREES_90)
            (*(code *)(*(int *)entity + 0x30))(entity, 0xb);
    }
    iVar2 = entity->nSubType;
    if (iVar2 == 0x618) {
        uVar3 = rand();
        if ((uVar3 & 1) == 0) {
            (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field94);
            entity->nState = 0xc;
        } else {
            (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field88);
            entity->nState = 0xb;
        }
    }
    (*(code *)(*(int *)entity + 0xd0))(entity, (int)(short)entity->nStateFlag, 0);
    if ((entity->nState != 0xc) && (entity->nConfig != 0))
        (*(code *)(*(int *)entity + 0xcc))(entity, 0xffffff38, 0);
}
void EntityBehavior_RandomSubstateInit(EntityObj *entity)
{
    int iVar1;
    if (entity->nSubType == 0) {
        iVar1 = (*(code *)(*(int *)entity->pContainer + 0x200))();
        if (iVar1 == 7) {
            (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field274);
            (*(code *)(*(int *)entity + 0xcc))(entity, 0xffff8800, 0);
        }
        iVar1 = rand();
        entity->nState = iVar1 % 5;
    }
    if (entity->nState == 0)
        (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field118);
}
void EntityBehavior_BoundCheckState(EntityObj *entity, EvyMsg *state)
{
    if (entity->nSubType < entity->nCondition) {
        if ((entity->nSubState != 0) && (entity->nSubState == 0x14)) {
            state->nReturn = 0;
            state->nDisp = ENTITY_STATE_BUSY;
        }
    } else {
        (*(code *)(*(int *)entity + 0x130))(entity);
        (*(code *)(*(int *)entity + 0xbc))(entity, &Entity_Field154);
    }
    MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
}
void EntityBehavior_PlayAnimSound(EntityObj *entity)
{
    (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field1F0);
    (*(code *)(*(int *)entity + 0xbc))(entity, &Entity_Field178);
}
void EntityBehavior_FrameZeroCheck(EntityObj *entity, EvyMsg *state)
{
    if (entity->nSubType == 0) {
        state->nReturn = 0;
        state->nDisp = 0xf;
    }
}
void EntityInitSequence(EntityObj *entity)
{
    AnimInitializeBase(entity->pChild, entity + 0x27, entity->nAnimIdx + 1, entity,
                 (&PTR_EntityBehavior_WanderMultiPhase_80089eb0)[entity->nAnimIdx * 4]);
    (*(code *)(*(int *)entity + 300))(entity);
    (*(code *)(*(int *)entity + 0x110))(entity);
    entity->nSubType = 0;
    entity->nReturnVal = 1;
}
void EntityBehavior_EndOfLifeAnim(EntityObj *entity, EvyMsg *state)
{
    int iVar1, uVar2;
    uint uVar3;
    if ((entity->nSubType == 0) && (iVar1 = (*(code *)(*(int *)entity->pContainer + 0x200))(), iVar1 != 7))
        entity->nState = 0xb;
    uVar2 = (*(code *)(*(int *)entity + 0x148))(entity);
    state->nReturn = uVar2;
    if (state->nCounter % 10 == 0)
        state->nDisp = 0xe;
    if (entity->nSubType == entity->nCondition) {
        (*(code *)(*(int *)entity + 0x128))(entity, 1);
        if ((entity->nState != 0) && (uVar3 = rand(), (uVar3 & 1) == 0)) {
            (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field238);
            (*(code *)(*(int *)entity + 0xcc))(entity, DEGREES_180, 0);
        }
        iVar1 = rand();
        if (iVar1 % 3 == 0)
            (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field7C);
    }
    if (entity->pSubObj != 0)
        (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffff80, 1);
}
void EntityBehavior_InitAimState(EntityObj *entity, EvyMsg *state)
{
    int iVar1;
    if (entity->nSubType == 0) {
        iVar1 = (*(code *)(*(int *)entity->pContainer + 0x1a0))((int *)entity->pContainer, 0);
        if (((iVar1 % 3 == 0) && (rand() % 3 == 0)) || (iVar1 % 3 == 1))
            (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field238);
    }
    if (state->nCounter == 0) {
        state->nReturn = 0;
        state->nDisp = ENTITY_STATE_TRIGGER;
    }
    MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
}
void EntityBehavior_ComplexStateMachine(EntityObj *entity, int state)
{
    int iVar1, uVar2;
    iVar1 = entity->nState;
    if (iVar1 == 0) {
        if (entity->nSubState == 5)
            EntityState_SetFields(state);
        if (entity->nSubType != entity->nCondition)
            return;
        (*(code *)(*(int *)entity + 0x130))(entity);
        iVar1 = 10;
    } else if (iVar1 == 10) {
        if (entity->nSubType < 10) {
            (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_Field64);
            iVar1 = (*(code *)(*(int *)entity->pContainer + 0x100))();
            if (iVar1 == 0)
                return;
            EntityState_SetFields(state);
            iVar1 = 0xc;
        } else {
            (*(code *)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer, 1);
            iVar1 = 0xb;
        }
    } else {
        if (iVar1 != 0xb) {
            if (iVar1 == 0xd) {
                if (entity->nSubType < 0x5a) {
                    if ((entity->nSubType == 0x1e) &&
                        (iVar1 = EntityFindChildByType(entity, 0, 0, 10, 0), iVar1 != 0)) {
                        (*(code *)(*(int *)((int *)entity)[0x40] + 0xd8))((int *)((int *)entity)[0x40], entity->nField_50, 0, 0);
                    }
                    (*(code *)(*(int *)entity->pContainer + 0x44))((int *)entity->pContainer, 0, &Entity_FieldD0);
                    return;
                }
                EntityState_SetTrigger(state);
                (*(code *)(*(int *)entity->pContainer + 0x44))((int *)entity->pContainer, 1, &Entity_Field7C);
                iVar1 = rand();
                if (iVar1 % 5 == 0)
                    uVar2 = 0xc;
                else
                    uVar2 = 10;
                (*(code *)(*(int *)entity + 0x30))(entity, uVar2);
                iVar1 = 0xe;
            } else {
                if (iVar1 != 0xc)
                    return;
                if (entity->nSubType < 10) {
                    (*(code *)(*(int *)entity + 0x44))(entity, 0, &Entity_FieldDC);
                    return;
                }
                EntityState_SetTrigger(state);
                (*(code *)(*(int *)entity + 0x16c))(entity);
                iVar1 = 1;
            }
            entity->nState = iVar1;
            return;
        }
        MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
        if (entity->nSubType < 0x1e) {
            (*(code *)(*(int *)entity + 0xc4))(entity, 0xfffffff6, 0);
            return;
        }
        EntityState_SetFields(state);
        iVar1 = EntityFindChildByType(entity, 0, 0, 0x1e, 0);
        if (iVar1 != 0)
            (*(code *)(*(int *)((int *)entity)[0x40] + 0xd4))((int *)((int *)entity)[0x40], entity->nField_50, 7, 0);
        iVar1 = 0xd;
    }
    entity->nState = iVar1;
    entity->nSubType = -1;
}
void EntityBehavior_CountedState(EntityObj *entity, EvyMsg *state)
{
    int uVar1, iVar2;
    uVar1 = (*(code *)(*(int *)entity + 0x148))();
    state->nReturn = uVar1;
    if (state->nCounter == 0) {
        state->nDisp = 0xc;
        entity->nState = entity->nState + 1;
    } else if (entity->nCondition - 1 <= state->nCounter) {
        state->nCounter = -1;
    }
    if ((entity->nState == 0x24) && (iVar2 = rand(), iVar2 % 3 == 0))
        (*(code *)(*(int *)entity + 0x30))(entity, 0xb);
}
void EntityBehavior_InitResetState(EntityObj *entity, EvyMsg *state)
{
    if (state->nCounter == 0) {
        state->nReturn = 0;
        state->nDisp = 0x19;
        state->nParam2 = 0x19;
        state->nParam4 = 0x19;
        MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
    }
    if (entity->nSubType == entity->nCondition) {
        (*(code *)(*(int *)entity + 0x130))(entity);
        (*(code *)(*(int *)entity + 0x30))(entity, 10);
    }
}
void EntityBehavior_TimerProximityState(EntityObj *entity, EvyMsg *state)
{
    int iVar1;
    uint uVar4;
    int skip = 0;
    if (entity->nState == 0) {
        iVar1 = EntityCheckTimerThreshold(entity, DEGREES_180);
        if (iVar1 != 0) {
            entity->nState = 0xb;
            MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
            MathComputeAngleToTarget(entity->pContainer, entity, 1, 1, 0);
            (*(code *)(*(int *)entity + 0x15c))(entity);
            (*(code *)(*(int *)entity + 0x168))(entity);
            (*(code *)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer, 1);
            state->nReturn = 0;
            state->nDisp = 0xc;
            entity->nSubType = 0;
        }
        if (entity->nState == 0) {
            (*(code *)(*(int *)entity + 0x160))(entity);
            (*(code *)(*(int *)entity + 0x16c))(entity);
            skip = 1;
        }
    }
    if (!skip) {
        if (state->nCounter % 100 == 0) {
            state->nReturn = (*(code *)(*(int *)entity + 0x148))(entity);
            state->nDisp = 0xc;
            state->nParam1 = -1;
        }
        uVar4 = entity->nSubType;
        if ((int)uVar4 < 3) {
            (*(code *)(*(int *)entity + 0xcc))(entity, 0x96, 0);
        } else if ((int)uVar4 < 7) {
            if ((uVar4 & 1) != 0)
                (*(code *)(*(int *)entity + 0xcc))(entity, 0xffffffce, 0);
            else
                (*(code *)(*(int *)entity + 0xcc))(entity, 0x32, 0);
        } else if (uVar4 == 100) {
            uVar4 = rand();
            if ((uVar4 & 1) != 0) {
                entity->nState = 0xc;
                (*(code *)(*(int *)entity + 0x130))(entity);
            }
        } else if (uVar4 == 0xf0) {
            (*(code *)(*(int *)entity->pContainer + 0x134))((int *)entity->pContainer, 1, 1);
        }
        if (entity->nState == 0xc) {
            iVar1 = entity->nSubType;
            if (iVar1 < 0x82) {
                (*(code *)(*(int *)entity + 0xcc))(entity, 10, 0);
            } else {
                if (iVar1 > 0x9f && iVar1 >= 0x12d)
                    MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
                if (iVar1 <= 0x9f || iVar1 >= 0x12d)
                    (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffffe2, 0);
            }
        }
    }
    iVar1 = (*(code *)(*(int *)entity + 0x144))(entity, entity->pContainer);
    if (iVar1 < 0x200) {
        (*(code *)(*(int *)entity + 0x160))(entity);
        (*(code *)(*(int *)entity + 0x30))(entity, 10);
    }
}
void EntityBehavior_WrapState(EntityObj *entity, EvyMsg *state)
{
    int uVar1;
    uVar1 = (*(code *)(*(int *)entity + 0x148))();
    state->nReturn = uVar1;
    if (state->nCounter == 0)
        state->nDisp = ENTITY_STATE_TRIGGER;
    if (entity->nCondition - 1 <= state->nCounter)
        state->nCounter = -1;
}
void EntityBehavior_SlowPeriodicState(EntityObj *entity, EvyMsg *state)
{
    if (state->nCounter % 0x46 == 0) {
        state->nReturn = 0;
        state->nDisp = 0x1b;
    }
    (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffff80, 0);
    if (entity->nSubType < 100) {
        (*(code *)(*(int *)entity + 0xcc))(entity, 0x20, 0);
    } else if (entity->nSubType < 0x12d) {
        return;
    } else {
        (*(code *)(*(int *)entity + 0xcc))(entity, 0xffffffe0, 0);
    }
}
void EntityBehavior_AnimDecel(EntityObj *entity)
{
    (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field1CC);
    if (entity->nSubType - 0xc9U < 99)
        (*(code *)(*(int *)entity + 0xcc))(entity, 0xffffffe0, 0);
}
int EntityCheckLinkState(EntityObj *entity)
{
    char cVar1, cVar3;
    int iVar4, iVar5;
    uint uVar6;
    int bVar2;
    iVar4 = 0;
    if (entity->nActiveFlag != 0) {
        iVar4 = entity->nAnimIdx * 0x10;
        EntityUpdateSubState(entity);
        uVar6 = (uint)(byte)(&Entity_Field2A8)[iVar4];
        bVar2 = 0;
        if ((uVar6 != 0) && (uVar6 != 3)) {
            if (uVar6 < 10) {
                if ((char)(&Entity_Field2A9)[iVar4] != 0) {
                    iVar5 = EntityMoveToward(entity, entity->pData + 0x18,
                                          (int)(char)(&Entity_Field2A9)[iVar4],
                                          (int)(char)(&Entity_Field2AD)[iVar4]);
                    if (iVar5 == 0) {
                        cVar1 = (&Entity_Field2A8)[iVar4];
                        cVar3 = '\x02';
                    } else {
                        cVar1 = (&Entity_Field2A8)[iVar4];
                        cVar3 = '\x01';
                    }
                    if (cVar1 == cVar3)
                        bVar2 = 1;
                }
            } else {
                bVar2 = entity->nTimer == uVar6 * 0xf;
            }
        }
        if (bVar2)
            (*(code *)(*(int *)entity + 0x160))(entity);
        iVar4 = entity->nActiveFlag;
    }
    return iVar4;
}
void EntityBehavior_MultiModeAnim(EntityObj *entity)
{
    int iVar1;
    if (entity->nState == 0) {
        iVar1 = (*(code *)(*(int *)entity->pContainer + 0x200))();
        if (iVar1 == 1)
            entity->nState = 0xb;
        else
            entity->nState = 0xc;
    }
    if (entity->nState == 0xc) {
        (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field1F0);
        (*(code *)(*(int *)entity + 0xcc))(entity, 0xffffffe2, 0);
    } else if (entity->nState == 0xb) {
        (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffff9c, 0);
        if (entity->nSubType - 0x55U < 0x1e) {
            (*(code *)(*(int *)entity + 0xcc))(entity, 0x50, 0);
        } else if (entity->nSubType == 0x78) {
            entity->nState = 0xd;
        }
    } else if (entity->nState == 0xd) {
        (*(code *)(*(int *)entity + 0xb8))(entity, *(int *)(entity->pContainer + 0x14) + 0x18);
        (*(code *)(*(int *)entity + 0xbc))(entity, &Entity_Field1B4);
    }
}
void EntityBehavior_RandomTriggerPeriodic(EntityObj *entity, EvyMsg *state)
{
    int iVar1, uVar2;
    if ((entity->nSubType == 0) && (iVar1 = rand(), iVar1 % 7 == 0))
        (*(code *)(*(int *)entity + 0x48))(entity, 1, &Entity_Field250);
    if ((state->nCounter & ENTITY_STATE_TYPE_MASK) == 0) {
        uVar2 = (*(code *)(*(int *)entity + 0x148))(entity);
        state->nReturn = uVar2;
        state->nDisp = ENTITY_STATE_FLAG_MASK;
    }
    (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffff9c, 0);
}
void EntityBehavior_FlagDrivenState(EntityObj *entity, EvyMsg *state)
{
    int iVar1;
    if (state->nCounter == 6) {
        state->nReturn = 0;
        state->nDisp = 4;
        state->nParam2 = 4;
        state->nParam4 = 4;
    }
    if (entity->nResultFlag != 0) {
        iVar1 = entity->nState;
        if (iVar1 == 0) {
            entity->nState = 10;
            entity->nSubType = 0;
        } else if (iVar1 == 10) {
            if (entity->nSubType != 10) {
                iVar1 = (*(code *)(*(int *)entity->pContainer + 0x100))();
                if (iVar1 != 0) {
                    if (entity->nPosZ == 0)
                        iVar1 = 0;
                    else
                        iVar1 = entity->pData + 0x38;
                    (*(code *)(*(int *)entity->pContainer + 0xb8))(entity->pContainer, iVar1);
                    (*(code *)(*(int *)entity->pContainer + 0x44))((int *)entity->pContainer, 1, &Entity_Field94);
                    (*(code *)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer, 0);
                    entity->nSubType = 0;
                    entity->nState = 0xb;
                }
            } else {
                (*(code *)(*(int *)entity + 0x30))(entity, 10);
            }
        } else if (iVar1 == 0xb) {
            iVar1 = 0;
            if (entity->nPosZ != 0)
                iVar1 = entity->pData + 0x38;
            (*(code *)(*(int *)entity->pContainer + 0xb8))(entity->pContainer, iVar1);
            if (entity->nSubType == 100)
                (*(code *)(*(int *)entity + 0x30))(entity, 10);
        }
    }
    (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffff00, 0);
}
void EntityState_SetFields(int state)
{
    *(int *)(state + 0x10) = 0;
    *(int *)(state + 0x1c) = 7;
    *(int *)(state + 0x20) = -2;
    *(int *)(state + 0x30) = 7;
    *(int *)(state + 0x34) = -2;
    *(int *)(state + 0x44) = 7;
    *(int *)(state + 0x48) = -2;
}
void EntityBehavior_InitMoveAim(EntityObj *entity, EvyMsg *state)
{
    int uVar1, iVar2;
    uVar1 = (*(code *)(*(int *)entity + 0x148))();
    state->nReturn = uVar1;
    if (state->nCounter == 0) {
        state->nDisp = 0;
        iVar2 = rand();
        (*(code *)(*(int *)entity + 200))(entity, (iVar2 % 3) * 0xc800, 0);
    }
    if (0x960 < entity->nSubType)
        MathComputeAngleToTarget(entity, entity->pContainer, 1, 0, 0);
    (*(code *)(*(int *)entity + 0xc4))(entity, 0xffffffe2, 0);
}

