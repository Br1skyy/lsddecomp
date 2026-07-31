/* Entity3_behavior_auto.c -- split from Entity3_vtable.c */
#include "Entity3_Shared.h"
/* =======================================================================
 * Migrated from ref/Entity_cleaned2.c - auto-generated behavior functions
 * Cleaned from Ghidra decompilation (state pointer, constants, naming)
 * =======================================================================
 */
void  EntityBehavior_AnimSet_Random_800650f4 (EntityObj *entity)
{
  int iVar1;
  void *puVar2;
  iVar1 = rand();
  if (iVar1 != (iVar1 / 3) * 3) {
    iVar1 = rand();
    puVar2 = &Entity_Field70;
    if (iVar1 != (iVar1 / 3) * 3) {
      puVar2 = &Entity_Field64;
    }
    (**(code **)(*(int *)entity + 0x44))(entity,0,puVar2);
  }
  return;
}
void  EntityBehavior_AnimSet_Reset_Velocity_Periodic_80064fbc(EntityObj *entity,EvyMsg *state,int arg3,int arg4,int arg5)
{
  int iVar1;
  state->nReturn = 0;
  if (state->nCounter == 6) {
    state->nDisp = 4;
    state->nParam2 = 4;
    state->nParam4 = 4;
  }
  iVar1 = entity->nSubType;
  if ((((arg3 <= iVar1) && (iVar1 <= arg3 + 0x5b)) ||
      ((arg3 + 0x155 <= iVar1 && (iVar1 <= arg3 + 0x1b1)))) ||
     ((arg3 + 0x2ba <= iVar1 && (iVar1 <= arg3 + 0x317)))) {
    (**(code **)(*(int *)entity + 0x44))(entity,0,&Entity_Field118);
  }
  (**(code **)(*(int *)entity + 0xc4))(entity,arg5,0);
  if (entity->nSubType == arg4) {
    (**(code **)(*(int *)entity + 0x160))(entity);
    entity->nState = 1;
  }
  return;
}
void  EntityBehavior_AnimSet_State_ResetAnim_Velocity_Random_Child_80063874 (EntityObj *entity,int state)
{
  int iVar1;
  int uVar2;
  iVar1 = entity->nState;
  if (iVar1 == 0) {
    if (entity->nSubState == 5) {
       EntityBehavior_State_80063c84(state);
    }
    if (entity->nSubType != entity->nCondition) {
      return;
    }
    (**(code **)(*(int *)entity + 0x130))(entity);
    iVar1 = 10;
  }
  else if (iVar1 == 10) {
    if (entity->nSubType < 10) {
      (**(code **)(*(int *)entity + 0x44))(entity,0,&Entity_Field64);
      iVar1 = (**(code **)(*(int *)entity->pContainer + 0x100))();
      if (iVar1 == 0) {
        return;
      }
       EntityBehavior_State_80063c84(state);
      iVar1 = 0xc;
    }
    else {
      (**(code **)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer,1);
      iVar1 = 0xb;
    }
  }
  else {
    if (iVar1 != 0xb) {
      if (iVar1 == 0xd) {
        if (entity->nSubType < 0x5a) {
          if ((entity->nSubType == ENTITY_STATE_COOLDOWN) && (iVar1 =  EntityFindChildByType (entity,0,0,10,0), iVar1 != 0)) {
            (**(code **)(*(int *)((int *)entity)[0x40] + 0xd8))((int *)((int *)entity)[0x40],entity->nField_50,0,0);
          }
          (**(code **)(*(int *)entity->pContainer + 0x44))((int *)entity->pContainer,0,&Entity_FieldD0);
          return;
        }
         EntityBehavior_State_80063cac(state);
        (**(code **)(*(int *)entity->pContainer + 0x44))((int *)entity->pContainer,1,&Entity_Field7C);
        iVar1 = rand();
        uVar2 = 0xc;
        if (iVar1 != (iVar1 / 5) * 5) {
          uVar2 = 10;
        }
        (**(code **)(*(int *)entity + 0x30))(entity,uVar2);
        iVar1 = 0xe;
      }
      else {
        if (iVar1 != 0xc) {
          return;
        }
        if (entity->nSubType < 10) {
          (**(code **)(*(int *)entity + 0x44))(entity,0,&Entity_FieldDC);
          return;
        }
         EntityBehavior_State_80063cac(state);
        (**(code **)(*(int *)entity + 0x16c))(entity);
        iVar1 = 1;
      }
      entity->nState = iVar1;
      return;
    }
     func_8001eacc (entity,entity->pContainer,1,0,0);
    if (entity->nSubType < 0x1e) {
      (**(code **)(*(int *)entity + 0xc4))(entity,0xfffffff6,0);
      return;
    }
     EntityBehavior_State_80063c84(state);
    iVar1 =  EntityFindChildByType (entity,0,0,0x1e,0);
    if (iVar1 != 0) {
      (**(code **)(*(int *)((int *)entity)[0x40] + 0xd4))((int *)((int *)entity)[0x40],entity->nField_50,7,0);
    }
    iVar1 = 0xd;
  }
  entity->nState = iVar1;
  entity->nSubType = -1;
  return;
}
void  EntityBehavior_AnimSet_Timed_Move_Periodic_80060148(EntityObj *entity,EvyMsg *state)
{
  int uVar1;
  int iVar2;
  void *puVar3;
  puVar3 = (void *)0x0;
  if (state->nCounter == (state->nCounter / 7) * 7) {
    uVar1 = (**(code **)(*(int *)entity + 0x148))(entity);
    state->nReturn = uVar1;
    state->nDisp = 3;
    state->field_0x24 = 0x40;
    state->field_0x28 = 0x40;
  }
  iVar2 = entity->nSubType;
  if (iVar2 == 200) {
    puVar3 = &Entity_Field94;
  }
  else if (iVar2 == 400) {
    puVar3 = &Entity_Field7C;
  }
  else if (iVar2 == 600) {
    puVar3 = &Entity_Field88;
  }
  else if (iVar2 == 800) {
    puVar3 = &Entity_Field7C;
    entity->nSubType = -1;
  }
  if (puVar3 != (void *)0x0) {
    (**(code **)(*(int *)entity + 0x44))(entity,0,puVar3);
  }
  (**(code **)(*(int *)entity + 0xd0))(entity,0xffffffe2,0);
  if (entity->nConfig != 0) {
    (**(code **)(*(int *)entity + 0xcc))(entity,0xffffff38,0);
  }
  return;
}
void  EntityBehavior_AnimSet_Timed_Periodic_80062fac(EntityObj *entity,EvyMsg *state)
{
  int uVar1;
  uVar1 = (**(code **)(*(int *)entity + 0x148))();
  state->nReturn = uVar1;
  if (state->nCounter == (state->nCounter / 10) * 10) {
    state->nDisp = 0x19;
    state->nParam1 = 2;
  }
  (**(code **)(*(int *)entity + 0x44))(entity,0,&Entity_FieldA0);
  if ((entity->nState == 0) && (entity->nResultFlag != 0)) {
    (**(code **)(*(int *)entity + 0x30))(entity,0xb);
    entity->nState = 0xb;
  }
  return;
}
void  EntityBehavior_AnimSet_Timed_Periodic_Random_800604dc(EntityObj *entity,EvyMsg *state)
{
  int uVar1;
  uint uVar2;
  void *puVar3;
   EntityBehavior_Anim_Random_80060710(0);
  uVar1 = (**(code **)(*(int *)entity + 0x148))(entity);
  state->nReturn = uVar1;
  if ((entity->nSubState == 0) || (entity->nSubState == 0xf)) {
    state->nDisp = ENTITY_STATE_TRIGGER;
    state->nParam2 = ENTITY_STATE_TRIGGER;
  }
  if (0x140 < entity->nSubType) {
    uVar2 = rand();
    uVar1 = 0x3c;
    if ((uVar2 & 1) != 0) {
      uVar1 = 0xffffffc4;
    }
    (**(code **)(*(int *)entity + 200))(entity,uVar1,0);
    uVar2 = rand();
    puVar3 = &Entity_Field64;
    if ((uVar2 & 3) != 0) {
      puVar3 = &Entity_Field70;
    }
    (**(code **)(*(int *)entity + 0x44))(entity,0,puVar3);
  }
  return;
}
void  EntityBehavior_AnimSet_Timed_ResetAnim_Move_Periodic_Random_80062a40(EntityObj *entity,EvyMsg *state)
{
  int uVar1;
  uint uVar2;
  int iVar3;
  uVar1 = (**(code **)(*(int *)entity + 0x148))();
  state->nReturn = uVar1;
  if (state->nCounter == (state->nCounter / 5) * 5) {
    state->nDisp = 0x11;
    state->nParam1 = 0xfffffffe;
  }
  if (entity->pSubObj == 0) {
    if (entity->nSubType == entity->nCondition) {
      (**(code **)(*(int *)entity + 0x128))(entity,1);
      uVar2 = rand();
      if ((uVar2 & 1) != 0) {
        entity->nState = 0xb;
      }
    }
  }
  else if (entity->nState == 0) {
    iVar3 = entity->nSubType;
    if ((((iVar3 == 0x3c) || (iVar3 == 0xd4)) || (iVar3 == 0x122)) || (iVar3 == 0x140)) {
      (**(code **)(*(int *)entity + 0x44))(entity,0,&Entity_Field88);
      iVar3 = entity->nSubType;
    }
    if (iVar3 == 0x18e) {
      (**(code **)(*(int *)entity + 0x44))(entity,0,&Entity_Field94);
    }
    (**(code **)(*(int *)entity + 0xd0))(entity,0xffffffce,0);
  }
  else {
    if ((entity->nSubType == 0x3c) || (entity->nSubType == 0x8c)) {
      (**(code **)(*(int *)entity + 0x44))(entity,0,&Entity_Field88);
    }
    if (entity->nSubType < 0xae) {
      (**(code **)(*(int *)entity + 0xd0))(entity,0xffffffce,0);
    }
    if (entity->nSubType == 0xae) {
      (**(code **)(*(int *)entity + 0x16c))(entity);
      entity->nState = 1;
    }
  }
  return;
}
void  EntityBehavior_AnimSet_Timed_ResetAnim_Velocity_Periodic_Random_Child_800621a8(EntityObj *entity,EvyMsg *state)
{
  int uVar1;
  uint uVar2;
  int iVar3;
  short uVar4;
  int iVar5;
  uVar1 = (**(code **)(*(int *)entity + 0x148))();
  state->nReturn = uVar1;
  if (state->nCounter == 0) {
    uVar2 = rand();
    uVar4 = 0xff40;
    if ((uVar2 & 1) != 0) {
      uVar4 = 0xfe8a;
    }
    *(short *)(entity + 0x12) = uVar4;
  }
  if (entity->nState == 0) {
    iVar5 = state->nCounter;
    iVar3 = (int)((ulonglong)((longlong)iVar5 * 0x66666667) >> 0x20);
    if (iVar5 == (iVar5 / 10) * 10) {
      state->nDisp = 0x1c;
      iVar5 = state->nCounter;
      iVar3 = (int)((ulonglong)((longlong)iVar5 * 0x66666667) >> 0x20);
    }
    iVar5 = iVar5 + ((iVar3 >> 3) - (iVar5 >> 0x1f)) * -0x14;
    if (iVar5 == 0) {
      state->nParam2 = 0x17;
      state->nParam3 = 0xffffffff;
      state->nParam4 = 0x17;
      state->nParam5 = 0xffffffff;
    }
    else if (iVar5 == 0xe) {
      state->nParam2 = 0xfffffffe;
      state->nParam4 = 0xfffffffe;
    }
    if (((entity->nSubType & 1U) == 0) &&
       (iVar3 = (**(code **)(*(int *)entity->pContainer + 0x100))(), iVar3 != 0)) {
      entity->nSubType = -1;
      entity->nState = 10;
      state->nDisp = ENTITY_STATE_TRIGGER;
    }
     func_8001eacc (entity,entity->pContainer,1,0,0);
    (**(code **)(*(int *)entity + 0xc4))(entity,(int)(short)entity->nStateFlag,1);
  }
  else if (entity->nState == 10) {
    if (entity->nSubType < 8) {
      (**(code **)(*(int *)entity + 0x44))(entity,0,&Entity_FieldC4);
      (**(code **)(*(int *)entity + 0xbc))(entity,&LAB_80089d6c);
    }
    else {
      state->nDisp = ENTITY_STATE_TRIGGER;
      state->nParam2 = 3;
      (**(code **)(*(int *)entity + 0x16c))(entity);
      uVar2 = rand();
      entity->nState = (uint)((uVar2 & 1) == 0);
    }
  }
  return;
}
void  EntityBehavior_AnimSet_Timed_Reset_Move_Periodic_80060f38(EntityObj *entity,EvyMsg *state)
{
  int uVar1;
  int iVar2;
  uVar1 = (**(code **)(*(int *)entity + 0x148))();
  state->nReturn = uVar1;
  iVar2 = entity->nSubType;
  if (iVar2 < 0xbc) {
    if (iVar2 == 0x54) {
      (**(code **)(*(int *)entity + 0x44))(entity,0,&Entity_Field7C);
    }
    if (state->nCounter == (state->nCounter / 0x14) * 0x14) {
      state->nDisp = 9;
    }
  }
  else if (iVar2 < 200) {
    (**(code **)(*(int *)entity + 0x44))(entity,0,&Entity_Field64);
  }
  else {
    (**(code **)(*(int *)entity + 0x160))(entity);
    state->nParam2 = ENTITY_STATE_COOLDOWN;
    entity->nState = 1;
  }
  (**(code **)(*(int *)entity + 0xd0))(entity,0xfffffe00,0);
  return;
}
void  EntityBehavior_AnimSet_Timed_State_Periodic_Random_Chase_Child_80061400(EntityObj *entity,EvyMsg *state)
{
  int iVar1;
  int uVar2;
  code *pcVar3;
  int *piVar4;
  if (entity->nSubType == 0) {
    state->nReturn = 0;
    state->nDisp = 0xc;
    iVar1 = (**(code **)(*(int *)entity->pContainer + 0x200))();
    if (iVar1 == 6) {
      entity->nState = 0xb;
    }
    else {
      iVar1 = rand();
      if (iVar1 == (iVar1 / 3) * 3) {
        entity->nState = 0xc;
      }
    }
  }
  if (state->nCounter == (state->nCounter / 100) * 100) {
    uVar2 = (**(code **)(*(int *)entity + 0x148))(entity);
    state->nReturn = uVar2;
    state->nDisp = 0xc;
    state->nParam1 = 0xffffffff;
  }
  if (entity->nState == 0xb) {
    iVar1 = (**(code **)(*(int *)entity + 0x144))(entity,entity->pContainer);
    if (iVar1 < 0x400) {
      (**(code **)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer,0);
      iVar1 = 0xd;
LAB_800615a8:
      entity->nState = iVar1;
      entity->nSubType = 0;
    }
  }
  else if ((entity->nState == 0xc) &&
          (iVar1 = (**(code **)(*(int *)entity + 0x144))(entity,entity->pContainer), iVar1 < 0x400)) {
    (**(code **)(*(int *)entity + 0x130))(entity);
    iVar1 = 0xe;
    goto LAB_800615a8;
  }
  iVar1 = entity->nState;
  if (iVar1 != 0xd) goto LAB_80061680;
  iVar1 = entity->nSubType;
  uVar2 = 0xffffffec;
  if (iVar1 < 0x32) {
    piVar4 = (int *)entity->pContainer;
    pcVar3 = *(code **)(*piVar4 + 0xcc);
code_r0x80061644:
    (*pcVar3)(piVar4,uVar2,0);
  }
  else {
    if (iVar1 < 500) {
      piVar4 = (int *)entity->pContainer;
      uVar2 = 5;
      if (iVar1 % 0x28 < 0x14) {
        uVar2 = 0xfffffffb;
      }
      pcVar3 = *(code **)(*piVar4 + 200);
      goto code_r0x80061644;
    }
    if (iVar1 == 500) {
      (**(code **)(*(int *)entity + 0x30))(entity,0xc);
    }
  }
  iVar1 = entity->nState;
LAB_80061680:
  if (iVar1 == 0xe) {
    if (entity->nSubType < 10) {
      (**(code **)(*(int *)entity + 0xcc))(entity,200,0);
    }
    else if (entity->nSubType == 10) {
      state->nDisp = ENTITY_STATE_TRIGGER;
      state->nReturn = 0;
      state->nParam2 = 3;
      (**(code **)(*(int *)entity + 0x44))(entity,1,&Entity_Field124);
      (**(code **)(*(int *)entity + 200))(entity,0x960,0);
      (**(code **)(*(int *)entity + 0xcc))(entity,0x5dc,0);
      (**(code **)(**(int **)(entity->nChildCount + 4) + 0x60))(*(int **)(entity->nChildCount + 4),0);
      entity->nState = 1;
    }
  }
  return;
}
void  EntityBehavior_AnimSet_Velocity_Random_800624bc (EntityObj *entity)
{
  uint uVar1;
  int uVar2;
  if ((entity->nSubType == 0) && (uVar1 = rand(), (uVar1 & 1) != 0)) {
    entity->nState = 0xb;
  }
  if (entity->nSubType == 300) {
    (**(code **)(*(int *)entity + 0x44))(entity,0,&Entity_Field7C);
  }
  if (entity->nSubType < 600) {
    uVar2 = 0x100;
    if (entity->nState == 0) {
      uVar2 = 0xffffff00;
    }
    (**(code **)(*(int *)entity + 0xc4))(entity,uVar2,0);
  }
  return;
}
void  EntityBehavior_AnimSet_Velocity_Random_80064928 (EntityObj *entity)
{
  int iVar1;
  if ((entity->nSubType == 700) && (iVar1 = rand(), iVar1 == (iVar1 / 3) * 3)) {
    entity->nState = 0xb;
  }
  if (entity->nState == 0xb) {
    if (entity->nSubType < 0x3fc) {
      (**(code **)(*(int *)entity + 0x44))(entity,0,&Entity_FieldB8);
      (**(code **)(*(int *)entity + 0xcc))(entity,0x1e,0);
    }
    if (entity->nSubType == 0x3a2) {
      (**(code **)(*(int *)entity + 0x30))(entity,10);
    }
  }
  else if (((entity->nSubType == 100) || (entity->nSubType == 800)) &&
          (iVar1 = rand(), iVar1 == (iVar1 / 5) * 5)) {
    (**(code **)(*(int *)entity->pObject + 0x138))((int *)entity->pObject,4,0);
  }
  (**(code **)(*(int *)entity + 0xc4))(entity,0xffffffe2,0);
  return;
}
void  EntityBehavior_Anim_80064aa4 (EntityObj *entity)
{
  (**(code **)(*(int *)entity + 0x48))(entity,1,&Entity_Field1CC);
  if (entity->nSubType - 0xc9U < 99) {
    (**(code **)(*(int *)entity + 0xcc))(entity,0xffffffe0,0);
  }
  return;
}
void  EntityBehavior_Anim_80064ca4 (EntityObj *entity)
{
   EntityBehavior_Timed_Velocity_Periodic_Random_80062570();
  (**(code **)(*(int *)entity + 0x48))(entity,1,&Entity_Field238);
  return;
}
void  EntityBehavior_Anim_8006519c (EntityObj *entity)
{
  (**(code **)(*(int *)entity + 0x48))(entity,1,&Entity_Field238);
  return;
}
void  EntityBehavior_Anim_800651d0 (EntityObj *entity)
{
  (**(code **)(*(int *)entity + 0x48))(entity,1,&Entity_Field238);
  return;
}
void  EntityBehavior_Anim_80065204 (EntityObj *entity)
{
  (**(code **)(*(int *)entity + 0x48))(entity,1,&Entity_Field1CC);
  return;
}
void  EntityBehavior_Anim_Periodic_Child_80064078(EntityObj *entity,EvyMsg *state)
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
          iVar2 = (**(code **)(*(int *)entity + 0x134))(entity,entity->pChild2,0);
          iVar3 = entity->nSubState;
          entity->pChild2 = iVar2;
          entity->nSubState = iVar3 + 1;
        } while (iVar3 + 1 < 0x18);
      }
      goto LAB_80064190;
    }
     func_8001eacc (entity,entity->pContainer,1,0,0);
    (**(code **)(*(int *)entity + 0x128))(entity,1);
    pcVar1 = *(code **)(*(int *)entity->pContainer + 0x130);
    uVar5 = 1;
    piVar4 = (int *)entity->pContainer;
  }
  else {
    if (entity->nSubState == 0) {
      state->nReturn = 0;
      state->nDisp = 0x16;
      goto LAB_80064190;
    }
    if (entity->nSubState != entity->nCondition + -1) goto LAB_80064190;
    state->nReturn = 0;
    state->nParam2 = ENTITY_STATE_TRIGGER;
    pcVar1 = *(code **)(*(int *)entity + 0x30);
    uVar5 = 10;
    piVar4 = entity;
  }
  (*pcVar1)(piVar4,uVar5);
LAB_80064190:
  (**(code **)(*(int *)entity + 0x48))(entity,1,&Entity_Field1E4);
  return;
}
void  EntityBehavior_Anim_Periodic_Random_Child_80060800(EntityObj *entity,EvyMsg *state)
{
  int iVar1;
  if (entity->nSubType == 0) {
    iVar1 = (**(code **)(*(int *)entity->pContainer + 0x1a0))((int *)entity->pContainer,0);
    if (iVar1 % 3 == 0) {
      iVar1 = rand();
      if (iVar1 != (iVar1 / 3) * 3) goto LAB_800608c0;
    }
    else if (iVar1 % 3 != 1) goto LAB_800608c0;
    (**(code **)(*(int *)entity + 0x48))(entity,1,&Entity_Field238);
  }
LAB_800608c0:
  if (state->nCounter == 0) {
    state->nReturn = 0;
    state->nDisp = ENTITY_STATE_TRIGGER;
  }
   func_8001eacc (entity,entity->pContainer,1,0,0);
  return;
}
void  EntityBehavior_Anim_Random_800602ac (EntityObj *entity,int state)
{
  int iVar1;
  int iVar2;
  if (entity->nSubType == 0) {
    iVar1 = rand();
    entity->nState = iVar1 % 5 + 10;
  }
  if ((entity->nState < 0xe) || (entity->nSubType < 0x140)) {
     EntityBehavior_AnimSet_Reset_Velocity_Periodic_80064fbc(entity,state,3000,500,0xffffff00);
  }
  else if ((entity->nState == 0xe) && ((entity->nSubType & 3U) == 0)) {
    iVar2 = rand();
    iVar1 = iVar2;
    if (iVar2 < 0) {
      iVar1 = iVar2 + 0x1f;
    }
    Entity_Field2A2 = (short)iVar2 + (short)(iVar1 >> 5) * -0x20 + 1;
    (**(code **)(*(int *)entity + 0x48))(entity,1,&Entity_Field298);
  }
  return;
}
void  EntityBehavior_Anim_Random_80060710 (EntityObj *entity)
{
  int iVar1;
  if (entity->nSubType == 0) {
    iVar1 = rand();
    if (iVar1 % 10 < 8) {
      if (4 < iVar1 % 10) {
        entity->nState = 10;
      }
    }
    else {
      (**(code **)(*(int *)entity + 0x48))(entity,1,&Entity_Field28C);
    }
  }
  if ((entity->nState == 10) && (200 < entity->nSubType)) {
    (**(code **)(*(int *)entity + 0xbc))(entity,&Entity_Field1C0);
  }
  return;
}
void  EntityBehavior_Anim_State_Chase_80064d48 (EntityObj *entity)
{
  int iVar1;
  (**(code **)(*(int *)entity + 0x48))(entity,1,&Entity_Field244);
  (**(code **)(*(int *)entity + 0x130))(entity);
  if ((entity->nState == 0) &&
     (iVar1 = (**(code **)(*(int *)entity + 0x144))(entity,entity->pContainer), iVar1 < 0x800)) {
    entity->nState = 10;
    entity->nSubType = 0;
  }
  if (entity->nState == 10) {
    if (entity->nSubType < 0x2d) {
      (**(code **)(*(int *)entity + 0x44))(entity,0,&Entity_Field100);
    }
    if (500 < entity->nSubType) {
      entity->nState = 0;
    }
  }
  return;
}
void  EntityBehavior_Anim_Timed_Periodic_Child_800646d8(EntityObj *entity,EvyMsg *state)
{
  int uVar1;
  int iVar2;
  void *puVar3;
  if (entity->nSubType == 0) {
    (**(code **)(*(int *)entity + 0xcc))(entity,0xfffffe00,0);
  }
  uVar1 = (**(code **)(*(int *)entity + 0x148))(entity);
  state->nReturn = uVar1;
  if (entity->nSubState == entity->nCondition / 2) {
    state->nDisp = 7;
    state->nParam1 = 0xfffffffe;
    state->nParam2 = 3;
    state->nParam3 = 0xfffffffe;
  }
  if (0x32 < entity->nSubType) {
    (**(code **)(*(int *)entity + 0x44))(entity,0,&Entity_FieldAC);
  }
  if (entity->nSubType < 0x30d) {
    iVar2 = *(int *)entity;
    uVar1 = 0xffffff00;
  }
  else {
     func_8001eacc (entity,entity->pContainer,1,0,0);
    iVar2 = entity->nSubType;
    if (iVar2 < 0x790) {
      if (iVar2 < 0x78b) {
        if (iVar2 < 0x786) {
          puVar3 = &Entity_Field220;
          if (0x780 < iVar2) {
            puVar3 = &Entity_Field1CC;
          }
        }
        else {
          puVar3 = &Entity_Field1D8;
        }
      }
      else {
        puVar3 = &Entity_Field1E4;
      }
    }
    else {
      puVar3 = &Entity_Field214;
    }
    (**(code **)(*(int *)entity + 0x48))(entity,1,puVar3);
    if (1999 < entity->nSubType) {
      entity->nState = 1;
      goto LAB_80064888;
    }
    iVar2 = *(int *)entity;
    uVar1 = 0xffffffc0;
  }
  (**(code **)(iVar2 + 0xc4))(entity,uVar1,0);
LAB_80064888:
  if ((entity->nResultFlag != 0) && (entity->nState == 0)) {
    entity->nState = 0xc;
    (**(code **)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer,1);
    (**(code **)(*(int *)entity + 0x30))(entity,10);
  }
  if (entity->nState == 0xc) {
    (**(code **)(*(int *)entity->pContainer + 0xc4))((int *)entity->pContainer,0x100,0);
  }
  return;
}
void  EntityBehavior_Anim_Timed_Periodic_Random_80061070(EntityObj *entity,EvyMsg *state)
{
  int iVar1;
  int uVar2;
  int iVar3;
  iVar3 = entity->nSubState;
  if ((entity->nSubType == 0) && (iVar1 = rand(), iVar1 == (iVar1 / 3) * 3)) {
    (**(code **)(*(int *)entity + 0x48))(entity,1,&Entity_Field250);
  }
  uVar2 = (**(code **)(*(int *)entity + 0x148))(entity);
  state->nReturn = uVar2;
  if (0x1f < iVar3) {
    iVar3 = iVar3 + -0x20;
  }
  if ((((iVar3 == 9) || (iVar3 == 0x11)) || (iVar3 == 0x17)) &&
     (state->nDisp = 0x13, iVar3 == 0x17)) {
    state->nParam2 = 0x13;
  }
  return;
}
void  EntityBehavior_Anim_Timed_Reset_Move_Periodic_Random_Chase_Child_80063144(EntityObj *entity,EvyMsg *state)
{
  uint uVar1;
  int iVar2;
  int uVar3;
  if ((entity->nSubType == 0) && (uVar1 = rand(), (uVar1 & 1) != 0)) {
    iVar2 = rand();
    entity->nState = iVar2 % 3;
  }
  if ((entity->nState == 0) || (entity->nResultFlag == 0)) {
    uVar3 = (**(code **)(*(int *)entity + 0x148))(entity);
    state->nReturn = uVar3;
    if (state->nCounter == (state->nCounter / 0x16) * 0x16) {
      state->nDisp = 0x1c;
    }
    if (500 < entity->nSubType) {
       func_8001eacc (entity,entity->pContainer,1,0,0);
    }
    iVar2 = (**(code **)(*(int *)entity + 0x144))(entity,entity->pContainer);
    uVar3 = 0xffffffec;
    if (iVar2 < 0x800) {
      (**(code **)(*(int *)entity->pContainer + 0xc4))((int *)entity->pContainer,0xfffff800,0);
    }
  }
  else {
    if (0x3c < state->nCounter) {
      state->nCounter = 0;
    }
    iVar2 = state->nCounter % 0x14;
    if (iVar2 == 0) {
      state->nDisp = 0x17;
      state->nParam2 = 0x17;
      state->nParam4 = 0x17;
      state->nReturn = 0;
      state->nParam1 = 0xffffffff;
      state->nParam3 = 0xffffffff;
      state->nParam5 = 0xfffffffe;
    }
    else if (iVar2 == 0xe) {
      state->nDisp = 0xfffffffe;
      state->nParam2 = 0xfffffffe;
      state->nParam4 = 0xfffffffe;
    }
     func_8001eacc (entity,entity->pContainer,1,0,0);
    if (entity->nState == 1) {
      uVar3 = 0xfffffe8a;
      (**(code **)(*(int *)entity + 0x48))(entity,0,&Entity_Field208);
      iVar2 = (**(code **)(*(int *)entity + 0x144))(entity,entity->pContainer);
      if (iVar2 < 0x200) {
        (**(code **)(*(int *)entity + 0x160))(entity);
        entity->nState = 1;
      }
    }
    else {
      (**(code **)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer,1);
       func_8001eacc (entity->pContainer,entity,1,1,0);
      uVar3 = 0;
      if (entity->nState == 2) {
        iVar2 = (**(code **)(*(int *)entity + 0x144))(entity,entity->pContainer);
        uVar3 = 0xffffffa0;
        if (iVar2 < 0x960) {
          entity->nState = 0xb;
          (**(code **)(*(int *)entity + 0x30))(entity,0xc);
        }
      }
    }
  }
  (**(code **)(*(int *)entity + 0xd0))(entity,uVar3,1);
  if (entity->nConfig != 0) {
    (**(code **)(*(int *)entity + 0xcc))(entity,0xffffff38,0);
  }
  return;
}
void  EntityBehavior_Anim_Timed_Reset_Velocity_Periodic_Random_8006536c(EntityObj *entity,EvyMsg *state)
{
  uint uVar1;
  int uVar2;
  int iVar3;
  if (((entity->nSubType == 0) && (uVar1 = rand(), (uVar1 & 3) == 0)) || (entity->nSubType == 0xe10)) {
    (**(code **)(*(int *)entity + 0x160))(entity);
    entity->nState = 1;
  }
  (**(code **)(*(int *)entity + 0x48))(entity,1,&Entity_Field244);
  uVar2 = (**(code **)(*(int *)entity + 0x148))(entity);
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
  (**(code **)(*(int *)entity + 0xc4))(entity,0xfffffff6,0);
  return;
}
void  EntityBehavior_Anim_Timed_State_ResetAnim_Periodic_Random_80062c58(EntityObj *entity,EvyMsg *state)
{
  int iVar1;
  int uVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  void *puVar6;
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
  uVar2 = (**(code **)(*(int *)entity + 0x148))(entity);
  state->nReturn = uVar2;
  if ((entity->nParam2 != 0) && ((state->nCounter & 3) == 0)) {
    state->nDisp = 0x1c;
  }
  iVar1 = entity->nCondition;
  iVar5 = entity->nSubType;
  if (iVar5 == iVar1 + -1) {
    entity->nSubType = -1;
  }
  else {
    iVar4 = iVar1 / 2;
    if (iVar1 < 0) {
      iVar1 = iVar1 + 3;
    }
    if (iVar5 < iVar4 + (iVar1 >> 2)) {
      if (iVar5 < iVar4) {
        if (iVar5 < iVar1 >> 2) goto LAB_80062de4;
        (**(code **)(*(int *)entity + 0x130))(entity);
        iVar1 = *(int *)entity;
        uVar2 = 0xffffff92;
      }
      else {
        if (iVar5 == iVar4) {
          state->nDisp = ENTITY_STATE_BUSY;
        }
        iVar1 = *(int *)entity;
        uVar2 = 0x6e;
      }
      (**(code **)(iVar1 + 0xc4))(entity,uVar2,0);
    }
    else {
      (**(code **)(*(int *)entity + 300))(entity);
    }
  }
LAB_80062de4:
  if ((entity->nState == 0xb) && (state->nCounter == 0x1fe)) {
    (**(code **)(*(int *)entity + 0xcc))(entity,0xfffffe84,0);
    (**(code **)(*(int *)entity + 0x44))(entity,0,&Entity_Field10C);
    (**(code **)(*(int *)entity + 0x16c))(entity);
    entity->nState = 1;
    Entity_DataCC = 1;
  }
  else if ((0xb < entity->nState) &&
          ((iVar1 = state->nCounter, 0x149 < iVar1 && ((iVar1 / 0x3c) * 0x3c == iVar1 + -0x1e)
           ))) {
    uVar7 = 0;
    uVar3 = rand();
    if ((uVar3 & 1) == 0) {
      puVar6 = &Entity_Field214;
      if (entity->nState == 0xd) {
        uVar7 = 0xfffffe70;
      }
      entity->nState = 0xc;
    }
    else {
      puVar6 = &Entity_Field250;
      iVar1 = entity->nState;
      entity->nState = 0xd;
      uVar7 = -(uint)(iVar1 == 0xc) & 400;
    }
    (**(code **)(*(int *)entity + 0x48))(entity,1,puVar6);
    (**(code **)(*(int *)entity + 0xcc))(entity,uVar7,0);
  }
  if ((state->nCounter == 0x208) && (Entity_DataCC != 0)) {
    (**(code **)(*(int *)entity + 0x16c))(entity);
    entity->nState = 1;
  }
  return;
}
void  EntityBehavior_Anim_Timed_Velocity_Periodic_Random_80060d80(EntityObj *entity,EvyMsg *state)
{
  int iVar1;
  int uVar2;
  uint uVar3;
  void *puVar4;
  if (((entity->nSubType == 0) && (iVar1 = rand(), iVar1 == (iVar1 / 5) * 5)) && (entity->nState == 0))
  {
    (**(code **)(*(int *)entity + 0x48))(entity,1,&Entity_Field238);
    (**(code **)(*(int *)entity + 0xcc))(entity,800,0);
    entity->nState = 0xb;
  }
  puVar4 = (void *)0x0;
  if (state->nCounter == (state->nCounter / 5) * 5) {
    uVar2 = (**(code **)(*(int *)entity + 0x148))(entity);
    state->nReturn = uVar2;
    state->nDisp = 8;
  }
  iVar1 = entity->nSubType;
  if (iVar1 == 0x5a) {
    puVar4 = &Entity_Field94;
  }
  else if (iVar1 == 0xa0) {
    puVar4 = &Entity_Field88;
  }
  else if ((iVar1 == 0xdc) && (uVar3 = rand(), (uVar3 & 1) != 0)) {
    puVar4 = &Entity_Field7C;
  }
  if (puVar4 != (void *)0x0) {
    (**(code **)(*(int *)entity + 0x44))(entity,0,puVar4);
  }
  (**(code **)(*(int *)entity + 0xc4))(entity,0xffffffb0,1);
  return;
}
void  EntityBehavior_Anim_Timed_Velocity_Periodic_Random_Child_80064294(EntityObj *entity,EvyMsg *state)
{
  int iVar1;
  int uVar2;
  uint uVar3;
  if ((entity->nSubType == 0) && (iVar1 = (**(code **)(*(int *)entity->pContainer + 0x200))(), iVar1 != 7))
  {
    entity->nState = 0xb;
  }
  uVar2 = (**(code **)(*(int *)entity + 0x148))(entity);
  state->nReturn = uVar2;
  if (state->nCounter == (state->nCounter / 10) * 10) {
    state->nDisp = 0xe;
  }
  if (entity->nSubType == entity->nCondition) {
    (**(code **)(*(int *)entity + 0x128))(entity,1);
    if ((entity->nState != 0) && (uVar3 = rand(), (uVar3 & 1) == 0)) {
      (**(code **)(*(int *)entity + 0x48))(entity,1,&Entity_Field238);
      (**(code **)(*(int *)entity + 0xcc))(entity,0x800,0);
    }
    iVar1 = rand();
    if (iVar1 == (iVar1 / 3) * 3) {
      (**(code **)(*(int *)entity + 0x44))(entity,0,&Entity_Field7C);
    }
  }
  if (entity->pSubObj != 0) {
    (**(code **)(*(int *)entity + 0xc4))(entity,0xffffff80,1);
  }
  return;
}
void  EntityBehavior_Anim_Velocity_80064cec (EntityObj *entity)
{
  (**(code **)(*(int *)entity + 0x48))(entity,1,&Entity_Field1D8);
  (**(code **)(*(int *)entity + 0xc4))(entity,0xfffffff6,0);
  return;
}
void  EntityBehavior_Anim_Velocity_800654a0 (EntityObj *entity)
{
   func_8001eacc (entity,entity->pContainer,1,0,0);
  (**(code **)(*(int *)entity + 0x48))(entity,1,&Entity_Field280);
  (**(code **)(*(int *)entity + 0xc4))(entity,0xffffffe2,1);
  return;
}
void  EntityBehavior_Anim_Velocity_Periodic_Random_Child_80063ed4(EntityObj *entity,EvyMsg *state)
{
  int iVar1;
  uint uVar2;
  int iVar3;
  if (entity->nSubType == 0) {
    iVar1 =  EntityFindChildByType (entity,0,0,5,0);
    if (iVar1 != 0) {
      uVar2 = rand();
      if ((uVar2 & 1) != 0) {
        (**(code **)(*(int *)entity + 0xbc))(entity,&Entity_Field190);
      }
      (**(code **)(*(int *)((int *)entity)[0x40] + 0xd4))((int *)((int *)entity)[0x40],entity->nField_50,0,0);
    }
  }
  else {
    iVar1 = entity->nSubState;
    if (iVar1 != 0) goto LAB_80063fc0;
    do {
      iVar1 = (**(code **)(*(int *)entity + 0x134))(entity,entity->pChild2,0);
      iVar3 = entity->nSubState;
      entity->pChild2 = iVar1;
      entity->nSubState = iVar3 + 1;
    } while (iVar3 + 1 < 0x18);
  }
  iVar1 = entity->nSubState;
LAB_80063fc0:
  if (0x18 < iVar1) {
    (**(code **)(*(int *)entity + 0xc4))(entity,0xffffffec,0);
    (**(code **)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer,1);
  }
  if (entity->nSubType == 0x32) {
    (**(code **)(*(int *)entity + 0x30))(entity,10);
  }
  else if (entity->nSubType == 0xc) {
    state->nReturn = 0;
    state->nDisp = 0x15;
  }
  (**(code **)(*(int *)entity + 0x48))(entity,1,&Entity_Field1E4);
  return;
}
void  EntityBehavior_Anim_Velocity_Random_80061f30 (EntityObj *entity)
{
  int iVar1;
  if ((entity->nSubType == 0) && (iVar1 = rand(), iVar1 == (iVar1 / 3) * 3)) {
    (**(code **)(*(int *)entity + 0x48))(entity,1,&Entity_Field1D8);
    (**(code **)(*(int *)entity + 0xcc))(entity,0xfffffed4,0);
    (**(code **)(*(int *)entity + 0x44))(entity,1,&Entity_Field88);
    entity->nState = 0xb;
  }
  if (entity->nState == 0xb) {
    if (entity->nSubType == 2000) {
      (**(code **)(*(int *)entity + 0x44))(entity,0,&Entity_Field94);
    }
    (**(code **)(*(int *)entity + 0xc4))(entity,0xffffffec,0);
  }
  return;
}
void  EntityBehavior_Child_8006090c (EntityObj *entity)
{
  int iVar1;
  if (entity->nResultFlag != 0) {
    iVar1 = entity->nState;
    if (iVar1 == 0) {
      entity->nState = 0xc;
      entity->nSubType = 0;
    }
    else if (iVar1 == 0xc) {
      if (entity->nSubType < 0x1e) {
        iVar1 = (**(code **)(*(int *)entity->pContainer + 0x100))();
        if (iVar1 != 0) {
          (**(code **)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer,0);
          entity->nSubType = 0;
          entity->nState = 0xb;
        }
      }
      else {
        (**(code **)(*(int *)entity + 0x30))(entity,0xb);
        entity->nState = 10;
      }
    }
    else if (iVar1 == 0xb) {
      if (entity->nSubType == 100) {
        (**(code **)(*(int *)entity + 0x30))(entity,0xc);
      }
      else {
        (**(code **)(*(int *)entity->pContainer + 0xcc))((int *)entity->pContainer,0xffffff9c,0);
      }
    }
  }
  return;
}
void  EntityBehavior_Periodic_80061c04(int entity,EvyMsg *state)
{
  if (*(int *)(entity + 0x84) == 0x1e) {
    state->nDisp = ENTITY_STATE_TRIGGER;
    state->nReturn = 0;
    state->nParam1 = 0xffffffff;
  }
  return;
}
void  EntityBehavior_Random_80064b14 (EntityObj *entity)
{
  int iVar1;
  iVar1 = rand();
  (**(code **)(*(int *)entity + 0x60))(entity,iVar1 % 0x14 == 0);
  return;
}
void  EntityBehavior_Random_Child_80062730 (EntityObj *entity)
{
  int iVar1;
  if (entity->nSubType == 0) {
    (**(code **)(**(int **)(entity->pContainer + 0x5c) + 100))
              (*(int **)(entity->pContainer + 0x5c),&Entity_CdField);
    iVar1 = rand();
    entity->nState = iVar1 % 3;
    if (*(int *)(*(int *)(entity->pContainer + 0x14) + 0x20) < 0x262) {
      entity->nState = 0;
    }
  }
  if (entity->nState == 0) {
    if ((entity->nSubType - 0x14U < 100) &&
       ((**(code **)(*(int *)entity->pContainer + 0xc4))
                  ((int *)entity->pContainer,(entity->nSubType + -0x13) * -0x20,1), entity->nSubType == 0x55))
    {
      (**(code **)(*(int *)entity->pContainer + 0x134))((int *)entity->pContainer,1,1);
    }
  }
  else {
    if (entity->nCondition / 2 < entity->nSubType) {
      (**(code **)(*(int *)entity->pContainer + 0xc4))((int *)entity->pContainer,0x80,0);
    }
    if (entity->nSubType == entity->nCondition + -0x1e) {
      (**(code **)(*(int *)entity + 0x30))(entity,10);
    }
  }
  return;
}
void  EntityBehavior_ResetAnim_80063bc0 (EntityObj *entity,int state)
{
  code *pcVar1;
  if (entity->nSubType < 10) {
    pcVar1 = *(code **)(*(int *)entity + 0x130);
  }
  else {
    if (entity->nSubType != 10) goto LAB_80063c20;
    pcVar1 = *(code **)(*(int *)entity + 300);
  }
  (*pcVar1)(entity);
LAB_80063c20:
  if (entity->nSubState == 10) {
     EntityBehavior_State_80063cac(state);
  }
  if (entity->nSubType == entity->nCondition + 10) {
    (**(code **)(*(int *)entity + 0x16c))(entity);
    entity->nState = 1;
  }
  return;
}
void  EntityBehavior_ResetAnim_Periodic_800636e4(EntityObj *entity,EvyMsg *state)
{
  if (entity->nSubState == (entity->nSubState / 0xf) * 0xf) {
    state->nDisp = 0xc;
    state->nReturn = 0;
    state->nParam1 = 2;
  }
  if (entity->nSubType == entity->nCondition) {
    state->nDisp = 0xfffffffe;
    (**(code **)(*(int *)entity + 0x16c))(entity);
    entity->nState = 1;
  }
  return;
}
void  EntityBehavior_ResetAnim_Periodic_Random_80063dc8(EntityObj *entity,EvyMsg *state)
{
  uint uVar1;
  if (entity->nSubType == ENTITY_STATE_ACTIVE) {
    state->nDisp = ENTITY_STATE_TRIGGER;
    state->nReturn = 0;
    state->nParam2 = 3;
  }
  else if (entity->nSubType == entity->nCondition) {
    (**(code **)(*(int *)entity + 0x16c))(entity);
    entity->nState = 1;
    uVar1 = rand();
    if ((uVar1 & 1) != 0) {
      (**(code **)(*(int *)entity + 0x30))(entity,0xb);
    }
  }
  return;
}
void  EntityBehavior_Reset_Periodic_80060cf0(EntityObj *entity,EvyMsg *state)
{
  if (entity->nSubType < entity->nCondition * 5) {
    if ((entity->nSubState == 0xf) || (entity->nSubState == 0x46)) {
      state->nReturn = 0;
      state->nDisp = 7;
      state->nParam2 = 7;
      state->nParam4 = 7;
    }
  }
  else {
    (**(code **)(*(int *)entity + 0x160))(entity);
    entity->nState = 1;
  }
  return;
}
void  EntityBehavior_Reset_Periodic_Child_80062660(EntityObj *entity,EvyMsg *state)
{
  state->nReturn = 0;
  if (state->nCounter == 0) {
    (**(code **)(*(int *)entity->pContainer + 0x44))((int *)entity->pContainer,1,&Entity_Field88);
    (**(code **)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer,1);
    state->nDisp = 0x19;
    state->nParam2 = 0x19;
    state->nParam4 = 0x19;
  }
  else if (state->nCounter == 0x14) {
    state->nParam2 = 0xd;
  }
  if (entity->nSubType == entity->nCondition + -1) {
    (**(code **)(*(int *)entity + 0x160))(entity);
  }
  return;
}
void  EntityBehavior_Reset_Velocity_Child_80064618 (EntityObj *entity)
{
  int iVar1;
  if (entity->nResultFlag != 0) {
    iVar1 =  EntityFindChildByType (entity,0,0,10,0);
    if (iVar1 != 0) {
      (**(code **)(*(int *)((int *)entity)[0x40] + 0xd4))((int *)((int *)entity)[0x40],entity->nField_50,7,0);
      (**(code **)(*(int *)entity + 0x160))(entity);
      (**(code **)(*(int *)entity->pContainer + 0x21c))();
    }
  }
  (**(code **)(*(int *)entity + 0xc4))(entity,0xffffffe2,1);
  return;
}
void  EntityBehavior_State_80025ba0 (EntityObj *entity,int state,int arg3)
{
  int bVar1;
  int iVar2;
  iVar2 =  func_80018390 ();
  (**(code **)(iVar2 + 8))(entity);
  iVar2 =  func_80025e9c ();
  *(int *)entity = iVar2;
  iVar2 = Pad_RefCount + 1;
  bVar1 = Pad_RefCount == 0;
  Pad_RefCount = iVar2;
  if (bVar1) {
    PadInit(state);
  }
  (**(code **)(*(int *)entity + 0x40))(entity,arg3);
  return;
}
void  EntityBehavior_State_80025c30 (int entity)
{
  int iVar1;
  Pad_RefCount = Pad_RefCount + -1;
  if (Pad_RefCount == 0) {
    PadStop();
  }
  iVar1 =  func_80018390 ();
  (**(code **)(iVar1 + 0xc))(entity);
  return;
}
void  EntityBehavior_State_80025c84 (EntityObj *entity,int state)
{
  *(ushort *)(entity + 3) = (ushort)(state != 0);
  entity->nDist = 0;
  entity->pData = 0;
  entity->pWorld = 0;
  (**(code **)(*(int *)entity + 0x50))();
  return;
}
void  EntityBehavior_State_80025cc4 (int entity)
{
  u_long uVar1;
  uint uVar2;
  uint uVar3;
  uVar1 = PadRead((uint)*(ushort *)(entity + 0xc));
  uVar3 = *(uint *)(entity + 0x10);
  *(u_long *)(entity + 0x10) = uVar1;
  uVar2 = uVar1 ^ uVar3;
  *(uint *)(entity + 0x14) = uVar2 & uVar3;
  *(uint *)(entity + 0x18) = uVar2 & uVar1;
  return;
}
void  EntityBehavior_State_80025d10 (EntityObj *entity)
{
  uint uVar1;
  int iVar2;
  int iVar3;
  uint *puVar4;
  uint uVar5;
  uint uVar6;
  uint uVar7;
  int *piVar8;
  code *pcVar9;
  int local_58 [16];
  uVar7 = entity->nDist;
  uVar6 = entity->pData;
  uVar5 = entity->pWorld;
  piVar8 = local_58;
  if (((uVar7 != 0) || (uVar6 != 0)) || (uVar5 != 0)) {
    iVar3 = 0;
    puVar4 = &Pad_InputMaskTable;
    do {
      uVar1 = *puVar4;
      iVar2 = -1;
      if ((uVar6 & uVar1) == 0) {
        if ((uVar5 & uVar1) == 0) {
          if ((uVar7 & uVar1) != 0) {
            iVar2 = 2;
          }
        }
        else {
          iVar2 = 0x12;
        }
      }
      else {
        iVar2 = 0x22;
      }
      puVar4 = puVar4 + 1;
      if (-1 < iVar2) {
        *piVar8 = iVar2 + iVar3;
        piVar8 = piVar8 + 1;
      }
      iVar3 = iVar3 + 1;
    } while (iVar3 < 0x10);
    pcVar9 = *(code **)(*(int *)entity + 0x30);
    while (piVar8 = piVar8 + -1, local_58 <= piVar8) {
      (*pcVar9)(entity,*piVar8);
    }
  }
  return;
}
void  EntityBehavior_State_80025e14 (void)
{
  return;
}
void  EntityBehavior_State_80025e1c (void)
{
    u32 local_40[16];
    u32 *puVar8 = (u32 *)&Pad_InputMaskTable;
    int i;
    for (i = 0; i < 16; i++)
        local_40[i] = ((u32 *)&D_80010764)[i];
    for (i = 0; i < 16; i++)
        puVar8[i] = local_40[i];
}
void  EntityBehavior_State_80025e94 (void)
{
  return;
}
void  EntityBehavior_State_800260a4 (void)
{
     func_80048cfc (*(int *)0x1f800000 % 365, 0);
}
void  EntityBehavior_State_80026170 (int entity)
{
    int *vobj;
    int texId;
    int clutId;
    int local_18[2];
    if (*(int *)(*(int *)(entity + 0x20) + 0xc) != 0) {
         func_80026f34 (0, 0, 0);
         EntityBehavior_State_80026254(entity, "ETC\\ASMKLOGO.TIM");
        vobj = (int *) Spu_NewSeqTrack (0, 0, 0, 0);
        texId = (int) func_800490f4 (local_18);
        clutId = (int) func_800493c8 (local_18[0]);
        (**(code **)(*vobj + 0x44))(vobj, *(int *)(entity + 0x1c), texId, clutId, 1);
        (**(code **)(*vobj + 4))(vobj);
         EntityBehavior_State_80026254(entity, "ETC\\OSDLOGO.TIM");
    }
}
void  EntityBehavior_State_80026254 (int entity, int state)
{
    int *vobj;
    vobj = (int *) NavMenu_NewPage (0, 0, 0);
    (**(code **)(*vobj + 0x98))(vobj, SpuInit, entity);
    (**(code **)(*vobj + 0x6c))(vobj, 0);
    (**(code **)(*vobj + 0xd4))(vobj, state, 0);
    (**(code **)(*vobj + 0x44))(vobj, *(int *)(entity + 0x1c), 0);
    (**(code **)(*vobj + 4))(vobj);
}
void  EntityBehavior_State_80026348 (int entity)
{
    int *vobj;
    int texId;
    int clutId;
    int local_18[2];
    if (*(int *)(*(int *)(entity + 0x20) + 8) != 0) {
         func_80026f34 (0, 0, 0);
        vobj = (int *) Spu_NewSeqTrack (0, 0, 0, 0);
        texId = (int) func_8004913c (local_18, 0);
        clutId = (int) func_800493c8 (local_18[0]);
        (**(code **)(*vobj + 0x44))(vobj, *(int *)(entity + 0x1c), texId, clutId, 1);
        (**(code **)(*vobj + 4))(vobj);
    }
}
void  EntityBehavior_State_800607f8 (void)
{
  return;
}
void  EntityBehavior_State_80061158 (EntityObj *entity)
{
  if (entity->nSubType == 0) {
    (**(code **)(*(int *)entity + 0xcc))(entity,0xffffff38,0);
  }
  return;
}
void  EntityBehavior_State_80062970 (EntityObj *entity)
{
  code *pcVar1;
  void *puVar2;
  if (entity->nResultFlag == 0) {
    (**(code **)(*(int *)entity + 0x130))(entity);
    pcVar1 = *(code **)(*(int *)entity + 0x44);
    puVar2 = &Entity_Field64;
  }
  else {
    (**(code **)(*(int *)entity + 300))();
    if (entity->nSubState != entity->nCondition + -1) {
      return;
    }
    (**(code **)(*(int *)entity + 0x130))(entity);
    pcVar1 = *(code **)(*(int *)entity + 0x48);
    puVar2 = &Entity_Field1FC;
  }
  (*pcVar1)(entity,0,puVar2);
  return;
}
void  EntityBehavior_State_80063c84 (int entity)
{
  *(int *)(entity + 0x10) = 0;
  *(int *)(entity + 0x1c) = 7;
  *(int *)(entity + 0x20) = 0xfffffffe;
  *(int *)(entity + 0x30) = 7;
  *(int *)(entity + 0x34) = 0xfffffffe;
  *(int *)(entity + 0x44) = 7;
  *(int *)(entity + 0x48) = 0xfffffffe;
  return;
}
void  EntityBehavior_State_80063cac (int entity)
{
  *(int *)(entity + 0x1c) = 0x12;
  *(int *)(entity + 0x10) = 0;
  *(int *)(entity + 0x30) = 3;
  *(int *)(entity + 0x44) = 3;
  return;
}
void  EntityBehavior_State_800650d4 (EntityObj *entity,EvyMsg *state)
{
   EntityBehavior_Anim_Timed_Velocity_Periodic_Random_80060d80(entity,state);
  return;
}
void  EntityBehavior_State_Move_Random_80065514 (EntityObj *entity)
{
  int iVar1;
  if (entity->nSubType == 0) {
    iVar1 = rand();
    entity->nState = iVar1 % 2 + 10;
  }
  (**(code **)(*(int *)entity + 0x130))(entity);
  if (200 < entity->nSubType) {
     func_8001eacc (entity,entity->pContainer,1,0,0);
    if (entity->nState == 10) {
      (**(code **)(*(int *)entity + 0xd0))(entity,0xfffffe00,0);
    }
  }
  return;
}
void  EntityBehavior_State_Periodic_800628d4(EntityObj *entity,EvyMsg *state)
{
  if (state->nCounter == 0) {
    state->nReturn = 0;
    state->nDisp = 0x19;
    state->nParam2 = 0x19;
    state->nParam4 = 0x19;
     func_8001eacc (entity,entity->pContainer,1,0,0);
  }
  if (entity->nSubType == entity->nCondition) {
    (**(code **)(*(int *)entity + 0x130))(entity);
    (**(code **)(*(int *)entity + 0x30))(entity,10);
  }
  return;
}
void  EntityBehavior_State_Periodic_80063094(EntityObj *entity,EvyMsg *state)
{
  if (entity->nSubType < entity->nCondition) {
    if ((entity->nSubState != 0) && (entity->nSubState == 0x14)) {
      state->nReturn = 0;
      state->nDisp = ENTITY_STATE_BUSY;
    }
  }
  else {
    (**(code **)(*(int *)entity + 0x130))(entity);
    (**(code **)(*(int *)entity + 0xbc))(entity,&Entity_Field154);
  }
   func_8001eacc (entity,entity->pContainer,1,0,0);
  return;
}
void  EntityBehavior_State_Periodic_Child_80064e34(EntityObj *entity,EvyMsg *state)
{
  int iVar1;
  int uVar2;
  if ((entity->nSubType == 0) && (iVar1 = (**(code **)(*(int *)entity->pContainer + 0x200))(), iVar1 == 5))
  {
    entity->nState = 0xb;
  }
  if ((entity->nState == 0) || (iVar1 = entity->nSubType, iVar1 < 0x870)) {
    uVar2 = 0x884;
  }
  else {
    if (iVar1 - 0x870U < 0x191) {
      if (iVar1 == 0x870) {
        (**(code **)(*(int *)entity + 0x130))(entity);
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
      (**(code **)(*(int *)entity + 300))(entity);
      state->nCounter = 1;
      return;
    }
    if (iVar1 < 0xa03) {
      return;
    }
    if (0xaf0 < iVar1) {
      (**(code **)(*(int *)entity + 0xcc))(entity,0xffffffe0,0);
    }
    uVar2 = 4000;
  }
   EntityBehavior_AnimSet_Reset_Velocity_Periodic_80064fbc(entity,state,0x1e1,uVar2,0xffffffc4);
  return;
}
void  EntityBehavior_State_Random_80064b80 (EntityObj *entity)
{
  uint uVar1;
  code *pcVar2;
  int uVar3;
  void *puVar4;
  if ((entity->nSubType == 0) && (uVar1 = rand(), (uVar1 & 1) == 0)) {
    entity->nState = 0xb;
  }
  if (entity->nState == 0xb) {
    if (entity->nSubType == 0) {
      (**(code **)(*(int *)entity + 0x130))(entity);
    }
    pcVar2 = *(code **)(*(int *)entity + 0x48);
    puVar4 = &Entity_Field22C;
    uVar3 = 1;
  }
  else {
    if (entity->nSubType == 0) {
      (**(code **)(*(int *)entity + 0x128))(entity,1);
    }
    uVar3 = 0xffffffe0;
    if (entity->nSubType % 0x14 < 10) {
      uVar3 = 0x20;
    }
    pcVar2 = *(code **)(*(int *)entity + 200);
    puVar4 = (void *)0x0;
  }
  (*pcVar2)(entity,uVar3,puVar4);
  return;
}
void  EntityBehavior_State_ResetAnim_Velocity_Periodic_Random_Chase_Child_800634a8(EntityObj *entity,EvyMsg *state)
{
  int iVar1;
  uint uVar2;
  code *pcVar3;
  if ((entity->nState == 0) && (entity->nSubType == 0)) {
    iVar1 = rand();
    if (iVar1 == (iVar1 / 3) * 3) {
      (**(code **)(*(int *)entity + 0x16c))(entity);
      (**(code **)(*(int *)entity + 0xc4))(entity,0xffffb000,0);
      rand();
    }
    else {
      uVar2 = rand();
      iVar1 = 0xc;
      if ((uVar2 & 1) != 0) {
        iVar1 = 0xb;
      }
      entity->nState = iVar1;
    }
  }
  if (entity->nState == 0xc) {
     func_8001eacc (entity,entity->pContainer,1,0,0);
    if (entity->nSubType == ENTITY_STATE_ACTIVE) {
      state->nDisp = ENTITY_STATE_TRIGGER;
      state->nReturn = 0;
      state->nParam2 = 3;
      (**(code **)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer,1);
    }
    if (0x14 < entity->nSubType) {
      (**(code **)(*(int *)entity + 0xc4))(entity,0xffffffd8,0);
    }
    if (entity->nSubType == ENTITY_STATE_DAMPER) {
      (**(code **)(*(int *)entity + 0x30))(entity,10);
    }
  }
  else if (entity->nState == 0xb) {
    (**(code **)(*(int *)entity + 0x130))(entity);
    iVar1 = (**(code **)(*(int *)entity + 0x144))(entity,entity->pContainer);
    if (iVar1 < 0x200) {
      iVar1 = rand();
      if (iVar1 == (iVar1 / 3) * 3) {
        pcVar3 = *(code **)(*(int *)entity + 0x16c);
      }
      else {
        pcVar3 = *(code **)(*(int *)entity + 0x160);
      }
      (*pcVar3)(entity);
    }
  }
  return;
}
void  EntityBehavior_State_ResetAnim_Velocity_Random_Child_80065238 (EntityObj *entity)
{
  int iVar1;
  if ((entity->nSubType == 0) && (iVar1 = rand(), iVar1 == (iVar1 / 5) * 5)) {
    entity->nState = 0xb;
  }
  (**(code **)(*(int *)entity + 0x130))(entity);
  (**(code **)(*(int *)entity + 0xc4))(entity,100,0);
  if (entity->nSubType == 1000) {
    (**(code **)(*(int *)entity + 0x16c))(entity);
    entity->nState = 1;
  }
  if ((entity->nState == 0xb) && (300 < entity->nSubType)) {
    (**(code **)(*(int *)entity->pContainer + 0x94))((int *)entity->pContainer,0,2);
    (**(code **)(*(int *)entity->pContainer + 0x94))((int *)entity->pContainer,0,7);
  }
  return;
}
void  EntityBehavior_State_Velocity_Periodic_800603c4(EntityObj *entity,EvyMsg *state)
{
  int uVar1;
  int iVar2;
  int iVar3;
  iVar3 = entity->nSubType;
  if (iVar3 < 0x14) {
    (**(code **)(*(int *)entity + 0x130))(entity);
    (**(code **)(*(int *)entity + 0xc4))(entity,0xffffffe2,0);
  }
  else {
    if (iVar3 == 0x14) {
      (**(code **)(*(int *)entity + 300))(entity);
      uVar1 = 5;
      state->nReturn = 0;
    }
    else {
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
      (**(code **)(*(int *)entity + 0x130))(entity);
      uVar1 = 0xfffffffe;
    }
    state->nDisp = uVar1;
  }
  return;
}
void  EntityBehavior_Timed_Periodic_8006204c(EntityObj *entity,EvyMsg *state)
{
  int uVar1;
  uVar1 = (**(code **)(*(int *)entity + 0x148))();
  state->nReturn = uVar1;
  if (state->nCounter == (state->nCounter / 0x1e) * 0x1e) {
    state->nDisp = 0xd;
  }
  return;
}
void  EntityBehavior_Timed_Periodic_800623e8(EntityObj *entity,EvyMsg *state)
{
  int uVar1;
  uVar1 = (**(code **)(*(int *)entity + 0x148))();
  state->nReturn = uVar1;
  if (entity->nSubState == entity->nCondition + -1) {
    state->nDisp = 0x19;
    state->nParam1 = 0xfffffffe;
  }
  if ((state->nCounter & 3) == 0) {
    state->nParam2 = 0x15;
    state->nParam3 = 0xffffffff;
  }
  if (state->nCounter == (state->nCounter / 200) * 200) {
    state->nParam4 = 0xd;
    state->nParam5 = 1;
  }
  return;
}
void  EntityBehavior_Timed_Periodic_80063cc8(EntityObj *entity,EvyMsg *state)
{
  int uVar1;
  uVar1 = (**(code **)(*(int *)entity + 0x148))();
  state->nReturn = uVar1;
  if (state->nCounter == 0) {
    state->nDisp = ENTITY_STATE_TRIGGER;
  }
  if (entity->nCondition + -1 <= state->nCounter) {
    state->nCounter = 0xffffffff;
  }
  return;
}
void  EntityBehavior_Timed_Periodic_80063d40(EntityObj *entity,EvyMsg *state)
{
  int uVar1;
  uVar1 = (**(code **)(*(int *)entity + 0x148))();
  state->nReturn = uVar1;
  if (state->nCounter == entity->nCondition / 2) {
    state->nDisp = ENTITY_STATE_TRIGGER;
  }
  if (entity->nCondition + -1 <= state->nCounter) {
    state->nCounter = 0xffffffff;
  }
  return;
}
void  EntityBehavior_Timed_Periodic_Random_800605d0(EntityObj *entity,EvyMsg *state)
{
  int uVar1;
  code *pcVar2;
  uint uVar3;
  int iVar4;
  void *puVar5;
   EntityBehavior_Anim_Random_80060710(0);
  uVar1 = (**(code **)(*(int *)entity + 0x148))(entity);
  state->nReturn = uVar1;
  if ((entity->nSubState == 7) || (entity->nSubState == 0x16)) {
    state->nDisp = 3;
  }
  iVar4 = entity->nSubType;
  if (iVar4 - 300U < 0x14) {
    uVar1 = 0xffffffc4;
    pcVar2 = *(code **)(*(int *)entity + 0xc4);
    puVar5 = (void *)0x0;
  }
  else {
    uVar1 = 0;
    if (iVar4 - 0x141U < 0x13) {
      pcVar2 = *(code **)(*(int *)entity + 0x44);
      puVar5 = &Entity_Field70;
    }
    else {
      if (iVar4 < 0x141) {
        return;
      }
      uVar3 = rand();
      uVar1 = 0xffffff80;
      if ((uVar3 & 1) != 0) {
        uVar1 = 0x80;
      }
      (**(code **)(*(int *)entity + 200))(entity,uVar1,1);
      uVar3 = rand();
      puVar5 = &Entity_Field64;
      if ((uVar3 & 3) != 0) {
        puVar5 = &Entity_Field70;
      }
      pcVar2 = *(code **)(*(int *)entity + 0x44);
      uVar1 = 0;
    }
  }
  (*pcVar2)(entity,uVar1,puVar5);
  return;
}
void  EntityBehavior_Timed_Periodic_Random_80063e68(EntityObj *entity,EvyMsg *state)
{
  int uVar1;
  uint uVar2;
  uVar1 = (**(code **)(*(int *)entity + 0x148))();
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
  return;
}
void  EntityBehavior_Timed_Periodic_Random_800644e8(EntityObj *entity,EvyMsg *state)
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
    (**(code **)(*(int *)entity + 0x128))(entity,iVar1 + (iVar3 >> 2) * -4);
  }
  else {
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
      (**(code **)(*(int *)entity + 0x128))(entity,iVar1 + (iVar3 >> 2) * -4);
      uVar2 = (**(code **)(*(int *)entity + 0x148))(entity);
      state->nReturn = uVar2;
      state->nDisp = 0x16;
      state->nParam1 = 2;
      state->field_0x24 = 0x40;
      state->field_0x28 = 0x20;
    }
  }
  return;
}
void  EntityBehavior_Timed_ResetAnim_Periodic_80063784(EntityObj *entity,EvyMsg *state)
{
  int uVar1;
  int iVar2;
  uVar1 = (**(code **)(*(int *)entity + 0x148))();
  state->nReturn = uVar1;
  iVar2 = entity->nSubState;
  if (iVar2 < 0x28) {
    state->nDisp = 0xc;
    state->nParam1 = 0xfffffffe;
    state->nParam4 = 5;
    state->nParam5 = 0xffffffff;
  }
  else if (iVar2 == 0x28) {
    state->nDisp = 0xfffffffe;
    state->nParam4 = 0xfffffffe;
  }
  else if (iVar2 == 0x2d) {
    state->nParam2 = ENTITY_STATE_TRIGGER;
    state->nParam3 = 1;
  }
  else if (iVar2 == 0x40) {
    state->nDisp = 7;
  }
  else if (iVar2 == 0x59) {
    (**(code **)(*(int *)entity + 0x16c))(entity);
    entity->nState = 1;
  }
  return;
}
void  EntityBehavior_Timed_State_Reset_ResetAnim_Periodic_Random_Chase_Child_80061778(EntityObj *entity,EvyMsg *state)
{
  int iVar1;
  int uVar2;
  code *pcVar3;
  uint uVar4;
  int *piVar5;
  int uVar6;
  if (entity->nState == 0) {
    iVar1 =  EntityCheckTimerThreshold (entity,0x800);
    if (iVar1 != 0) {
      entity->nState = 0xb;
       func_8001eacc (entity,entity->pContainer,1,0,0);
       func_8001eacc (entity->pContainer,entity,1,1,0);
      (**(code **)(*(int *)entity + 0x15c))(entity);
      (**(code **)(*(int *)entity + 0x168))(entity);
      (**(code **)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer,1);
      state->nReturn = 0;
      state->nDisp = 0xc;
      entity->nSubType = 0;
    }
    if (entity->nState == 0) {
      (**(code **)(*(int *)entity + 0x160))(entity);
      (**(code **)(*(int *)entity + 0x16c))(entity);
      goto LAB_80061a24;
    }
  }
  if (state->nCounter == (state->nCounter / 100) * 100) {
    uVar2 = (**(code **)(*(int *)entity + 0x148))(entity);
    state->nReturn = uVar2;
    state->nDisp = 0xc;
    state->nParam1 = 0xffffffff;
  }
  uVar4 = entity->nSubType;
  piVar5 = entity;
  if ((int)uVar4 < 3) {
    uVar2 = 0x96;
    pcVar3 = *(code **)(*(int *)entity + 0xcc);
    uVar6 = 0;
LAB_800619a4:
    (*pcVar3)(piVar5,uVar2,uVar6);
  }
  else {
    if ((int)uVar4 < 7) {
      uVar2 = 0x32;
      if ((uVar4 & 1) != 0) {
        uVar2 = 0xffffffce;
      }
      pcVar3 = *(code **)(*(int *)entity + 0xcc);
      uVar6 = 0;
      goto LAB_800619a4;
    }
    if (uVar4 == 100) {
      uVar4 = rand();
      if ((uVar4 & 1) != 0) {
        entity->nState = 0xc;
        (**(code **)(*(int *)entity + 0x130))(entity);
      }
    }
    else {
      uVar2 = 1;
      if (uVar4 == 0xf0) {
        piVar5 = (int *)entity->pContainer;
        pcVar3 = *(code **)(*piVar5 + 0x134);
        uVar6 = 1;
        goto LAB_800619a4;
      }
    }
  }
  if (entity->nState == 0xc) {
    iVar1 = entity->nSubType;
    if (iVar1 < 0x82) {
      pcVar3 = *(code **)(*(int *)entity + 0xcc);
      uVar2 = 10;
    }
    else {
      if (0x9f < iVar1) {
        if (iVar1 < 0x12d) goto LAB_80061a24;
         func_8001eacc (entity,entity->pContainer,1,0,0);
      }
      uVar2 = 0xffffffe2;
      pcVar3 = *(code **)(*(int *)entity + 0xc4);
    }
    (*pcVar3)(entity,uVar2,0);
  }
LAB_80061a24:
  iVar1 = (**(code **)(*(int *)entity + 0x144))(entity,entity->pContainer);
  if (iVar1 < 0x200) {
    (**(code **)(*(int *)entity + 0x160))(entity);
    (**(code **)(*(int *)entity + 0x30))(entity,10);
  }
  return;
}
void  EntityBehavior_Timed_State_Velocity_Periodic_Random_80061198(EntityObj *entity,EvyMsg *state)
{
  int iVar1;
  int uVar2;
  uint uVar3;
  code *pcVar4;
  if (entity->nSubType == 0) {
    iVar1 = rand();
    entity->nState = iVar1 % 3;
    if (iVar1 % 3 == 0) {
      (**(code **)(*(int *)entity + 0x130))(entity);
      (**(code **)(*(int *)entity + 0xcc))(entity,0x1800,0);
    }
  }
  uVar2 = (**(code **)(*(int *)entity + 0x148))(entity);
  state->nReturn = uVar2;
  if (entity->nState == 0) {
    state->nDisp = 0xc;
    state->nParam1 = 0xffffffff;
    (**(code **)(*(int *)entity + 0xc4))(entity,0xfffffe00,0);
    if (entity->nSubType - 0x80U < 0xc2) {
      (**(code **)(*(int *)entity + 0xcc))(entity,0xffffff80,0);
      return;
    }
    if (entity->nSubType != 0x142) {
      return;
    }
    pcVar4 = *(code **)(*(int *)entity + 300);
  }
  else {
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
      iVar1 =  EntityMoveToward (entity,entity->pData + 0x18,0xf,10);
      if (iVar1 == 0) {
        return;
      }
      iVar1 =  EntityFindChildByType (entity,0,0,10,0);
      if (iVar1 != 0) {
        (**(code **)(*(int *)((int *)entity)[0x40] + 0xd4))((int *)((int *)entity)[0x40],entity->nField_50,4,0);
      }
      uVar3 = rand();
      if ((uVar3 & 1) == 0) {
        return;
      }
      (**(code **)(*(int *)entity + 0x30))(entity,0xb);
      return;
    }
    if (iVar1 != 0x3b) {
      return;
    }
    pcVar4 = *(code **)(*(int *)entity + 0x160);
  }
  (*pcVar4)(entity);
  entity->nState = 1;
  return;
}
void  EntityBehavior_Timed_Velocity_Periodic_80060a4c(EntityObj *entity,EvyMsg *state)
{
  int uVar1;
  if (entity->nSubState == 0x26) {
     func_8001eacc (entity,entity->pContainer,1,0,0);
    uVar1 = (**(code **)(*(int *)entity + 0x148))(entity);
    state->nReturn = uVar1;
    state->nDisp = 6;
  }
  uVar1 = 0x1e;
  if (entity->nSubType % 10 < 5) {
    uVar1 = 0xffffffe2;
  }
  (**(code **)(*(int *)entity + 0xcc))(entity,uVar1,0);
  (**(code **)(*(int *)entity + 0xc4))(entity,0xffffffe2,1);
  return;
}
void  EntityBehavior_Timed_Velocity_Periodic_80061e60(EntityObj *entity,EvyMsg *state)
{
  int uVar1;
  int iVar2;
  iVar2 = state->nCounter % 300;
  uVar1 = (**(code **)(*(int *)entity + 0x148))();
  state->nReturn = uVar1;
  if (iVar2 < 0x14) {
    state->nDisp = 5;
    state->nParam1 = 0xfffffffe;
  }
  else if (iVar2 == 0x16) {
    state->nDisp = 0xfffffffe;
  }
  (**(code **)(*(int *)entity + 0xc4))(entity,0xfffffff6,0);
  return;
}
void  EntityBehavior_Timed_Velocity_Periodic_800641c0(EntityObj *entity,EvyMsg *state)
{
  int uVar1;
  uVar1 = (**(code **)(*(int *)entity + 0x148))();
  state->nReturn = uVar1;
  if (state->nCounter == (state->nCounter / 10) * 10) {
    state->nDisp = 3;
  }
  if (entity->nSubType == entity->nCondition) {
    (**(code **)(*(int *)entity + 0x128))(entity,1);
  }
  if (entity->pSubObj == 1) {
    (**(code **)(*(int *)entity + 0xc4))(entity,0xffffff80,1);
  }
  return;
}
void  EntityBehavior_Timed_Velocity_Periodic_Random_80061a90(EntityObj *entity,EvyMsg *state)
{
  int iVar1;
  int uVar2;
  uint uVar3;
  if ((entity->nSubType == 0) && (iVar1 = rand(), iVar1 == (iVar1 / 10) * 10)) {
    entity->nState = 0xc;
  }
  if (state->nCounter == (state->nCounter / 10) * 10) {
    uVar2 = (**(code **)(*(int *)entity + 0x148))(entity);
    state->nReturn = uVar2;
    state->nDisp = 0xc;
    state->nParam1 = 0xffffffff;
  }
  if ((entity->nSubType == 0) && (uVar3 = rand(), (uVar3 & 1) != 0)) {
    (**(code **)(*(int *)entity + 0xcc))(entity,0x800,0);
  }
  (**(code **)(*(int *)entity + 0xc4))(entity,0xffffff80,0);
  if ((entity->nState == 0xc) && (entity->nSubType == 300)) {
    (**(code **)(*(int *)entity->pObject + 0x138))((int *)entity->pObject,1,1);
  }
  return;
}
void  EntityBehavior_Timed_Velocity_Periodic_Random_80062570(EntityObj *entity,EvyMsg *state)
{
  int uVar1;
  int iVar2;
  uVar1 = (**(code **)(*(int *)entity + 0x148))();
  state->nReturn = uVar1;
  if (state->nCounter == 0) {
    state->nDisp = 0;
    iVar2 = rand();
    (**(code **)(*(int *)entity + 200))(entity,(iVar2 % 3) * 0xc800,0);
  }
  if (0x960 < entity->nSubType) {
     func_8001eacc (entity,entity->pContainer,1,0,0);
  }
  (**(code **)(*(int *)entity + 0xc4))(entity,0xffffffe2,0);
  return;
}
void  EntityBehavior_Velocity_80064450 (EntityObj *entity)
{
  code *pcVar1;
  int uVar2;
  uVar2 = 3;
  if (entity->nSubType == 0) {
    pcVar1 = *(code **)(*(int *)entity + 0x128);
  }
  else {
    if (entity->nSubType != entity->nCondition) goto LAB_800644ac;
    pcVar1 = *(code **)(*(int *)entity + 0x128);
    uVar2 = 1;
  }
  (*pcVar1)(entity,uVar2);
LAB_800644ac:
  if (entity->pSubObj == 1) {
    (**(code **)(*(int *)entity + 0xc4))(entity,0xffffff80,0);
  }
  return;
}
void  EntityBehavior_Velocity_Periodic_Child_80060b34(EntityObj *entity,EvyMsg *state)
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
    }
    else {
      if (iVar1 == 10) {
        if (entity->nSubType != 10) {
          iVar1 = (**(code **)(*(int *)entity->pContainer + 0x100))();
          if (iVar1 != 0) {
            if (entity->nPosZ == 0) {
              iVar1 = 0;
            }
            else {
              iVar1 = entity->pData + 0x38;
            }
            (**(code **)(*(int *)entity->pContainer + 0xb8))(entity->pContainer,iVar1);
            (**(code **)(*(int *)entity->pContainer + 0x44))((int *)entity->pContainer,1,&Entity_Field94);
            (**(code **)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer,0);
            entity->nSubType = 0;
            entity->nState = 0xb;
          }
          goto LAB_80060cc4;
        }
      }
      else {
        if (iVar1 != 0xb) goto LAB_80060cc4;
        iVar1 = 0;
        if (entity->nPosZ != 0) {
          iVar1 = entity->pData + 0x38;
        }
        (**(code **)(*(int *)entity->pContainer + 0xb8))(entity->pContainer,iVar1);
        if (entity->nSubType != 100) goto LAB_80060cc4;
      }
      (**(code **)(*(int *)entity + 0x30))(entity,10);
    }
  }
LAB_80060cc4:
  (**(code **)(*(int *)entity + 0xc4))(entity,0xffffff00,0);
  return;
}
void  EntityBehavior_Velocity_Periodic_Random_Chase_Child_80061c2c(EntityObj *entity,EvyMsg *state)
{
  uint uVar1;
  int iVar2;
  int uVar3;
  if (entity->nSubState == (entity->nSubState / 0x1e) * 0x1e) {
    state->nDisp = 3;
    state->nReturn = 0;
    state->nParam1 = 0xfffffffe;
  }
  if (100 < entity->nSubType) {
     func_8001eacc (entity,entity->pContainer,1,0,0);
  }
  (**(code **)(*(int *)entity + 0xc4))(entity,0xfffffffb,0);
  iVar2 = entity->nSubType;
  if (iVar2 == 300) {
    iVar2 = (**(code **)(*(int *)entity + 0x144))(entity,entity->pContainer);
    if (iVar2 < 0x1000) {
      (**(code **)(*(int *)entity->pContainer + 0x130))((int *)entity->pContainer,0);
      goto LAB_80061d5c;
    }
    iVar2 = entity->nSubType;
  }
  if (iVar2 == 500) {
    (**(code **)(*(int *)entity->pContainer + 0x134))((int *)entity->pContainer,1,1);
  }
LAB_80061d5c:
  if ((entity->nState == 0) &&
     (iVar2 = (**(code **)(*(int *)entity + 0x144))(entity,entity->pContainer), iVar2 < 0x400)) {
    uVar1 = rand();
    if ((uVar1 & 1) == 0) {
      iVar2 = 0xb;
    }
    else {
      state->nParam2 = 6;
      state->nReturn = 0;
      state->nParam3 = 0xffffffff;
      uVar1 = rand();
      if ((uVar1 & 1) != 0) {
        (**(code **)(*(int *)entity->pObject + 0x138))((int *)entity->pObject,0xffffffff,0);
      }
      iVar2 = 10;
      entity->nSubType = 0;
    }
    entity->nState = iVar2;
  }
  if ((entity->nState == 10) && (entity->nSubType == 0x46)) {
    uVar1 = rand();
    uVar3 = 0xb;
    if ((uVar1 & 1) != 0) {
      uVar3 = 0xc;
    }
    (**(code **)(*(int *)entity + 0x30))(entity,uVar3);
  }
  return;
}
void  EntityBehavior_Velocity_Random_800620c4 (EntityObj *entity)
{
  int iVar1;
  if ((entity->nSubType == 0) && (iVar1 = rand(), iVar1 == (iVar1 / 3) * 3)) {
    entity->nState = 0xb;
  }
  if (entity->nState == 0xb) {
    if (entity->nSubType == 0x1f6) {
      (**(code **)(*(int *)entity + 0xcc))(entity,0x800,0);
       func_8001eacc (entity,entity->pContainer,1,0,0);
    }
    if (500 < entity->nSubType) {
      (**(code **)(*(int *)entity + 0xc4))(entity,0xfffffe00,0);
    }
  }
  return;
}
