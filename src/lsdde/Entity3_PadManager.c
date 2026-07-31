/* Entity3_PadManager.c -- split from Entity3_vtable.c */
#include "Entity3_Shared.h"
/* ==== class_16334.c ==== */
void PadManagerUpdate(int *obj)
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
  uVar7 = obj[4];
  uVar6 = obj[5];
  uVar5 = obj[6];
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
    pcVar9 = *(code **)(*obj + 0x30);
    while (piVar8 = piVar8 + -1, local_58 <= piVar8) {
      (*pcVar9)(obj,*piVar8);
    }
  }
}
void PadManagerAlloc(int obj)
{
  int iVar1;
  Pad_RefCount = Pad_RefCount + -1;
  if (Pad_RefCount == 0) {
    PadStop();
  }
  iVar1 = GetCoordSystemVtable();
  (**(code **)(iVar1 + 0xc))(obj);
}
void PadManagerAnalogStick(void)
{
}
void PadManagerPollInput(void)
{
    u32 local_40[16];
    u32 *puVar8 = (u32 *)&Pad_InputMaskTable;
    int i;
    for (i = 0; i < 16; i++)
        local_40[i] = ((u32 *)&D_80010764)[i];
    for (i = 0; i < 16; i++)
        puVar8[i] = local_40[i];
}
void PadManagerSetEnabled(int *obj, int active)
{
  *(ushort *)(obj + 3) = (ushort)(active != 0);
  obj[4] = 0;
  obj[5] = 0;
  obj[6] = 0;
  (**(code **)(*obj + 0x50))();
}
void PadManagerUnknownA(void)
{
}
void PadManagerConfigure(int *obj, int padMode, int arg3)
{
  int bVar1;
  int iVar2;
  iVar2 = GetCoordSystemVtable();
  (**(code **)(iVar2 + 8))(obj);
  iVar2 = PadManagerGetVtable();
  *obj = iVar2;
  iVar2 = Pad_RefCount + 1;
  bVar1 = Pad_RefCount == 0;
  Pad_RefCount = iVar2;
  if (bVar1) {
    PadInit(padMode);
  }
  (**(code **)(*obj + 0x40))(obj, arg3);
}
int PadManagerConstructor(int type, int arg2)
{
  /* Original function was a stub (jr $ra) — returns 0, pad never initialized */
  (void)type;
  (void)arg2;
  return 0;
}
void PadManagerDestructor(int obj)
{
  u_long uVar1;
  uint uVar2;
  uint uVar3;
  uVar1 = PadRead((uint)*(ushort *)(obj + 0xc));
  uVar3 = *(uint *)(obj + 0x10);
  *(u_long *)(obj + 0x10) = uVar1;
  uVar2 = uVar1 ^ uVar3;
  *(uint *)(obj + 0x14) = uVar2 & uVar3;
  *(uint *)(obj + 0x18) = uVar2 & uVar1;
}
int PadManagerGetVtable(void)
{
  return (int)&Pad_Vtable;
}
/* ======================================================================== */
/* orphan_stubs.c redistributed functions                                    */
/* ======================================================================== */
/* Apply entity's matrix transform to position vector including parent offset */
void UpdateTransformPosition(EntityObj *entity, int *outPos, int matrixIdx)
{
    int *pi;
    int vec;
    char localBuf[32];
    (**(code **)(*(int *)entity + 0x84))(entity, localBuf, 0, 0);
    ApplyRawMatrixToVectors(outPos, matrixIdx, 1, localBuf);
    pi = 0;
    if (entity->nPosZ != 0) {
        pi = (int *)(entity->pData + 0x38);
    }
    *outPos += *pi;
    vec = (**(code **)(Other_DreamSysStatePtr + 0x200))(0, 0, 0, 0);
    if (entity->nPosZ != 0) {
        vec = (**(code **)(Other_DreamSysStatePtr + 0x200))(0, 0, 0, 0);
    }
    outPos[1] += *(int *)(vec + 4);
    vec = (**(code **)(Other_DreamSysStatePtr + 0x200))(0, 0, 0, 0);
    if (entity->nPosZ != 0) {
        vec = (**(code **)(Other_DreamSysStatePtr + 0x200))(0, 0, 0, 0);
    }
    outPos[2] += *(int *)(vec + 8);
}
/* Link child entity to parent (set parent pointer and link scene graph) */
void LinkObjectToParent(int child, int parent)
{
    *(int *)(child + 0x20) = parent;
    *(int *)(child + 0x18) = *(int *)(parent + 0x10);
    GsLinkObject4(*(int *)(*(int *)(child + 0x20) + 0xc) + 0xc, (void *)(child + 0x10), 0);
}
/* Walk a linked entity list calling vtable+0x104 on each head */
void EntityListCallEach(int *list)
{
    int i;
    (**(code **)(*list + 0x104))(0, 0, 0, 0);
    if (list[0x18] == 0) {
        list[0x17] = 0;
    } else {
        i = (**(code **)(*(int *)list[0x17] + 4))(0, 0, 0, 0);
        list[0x17] = i;
    }
}
/* Set teleport flag when entering specific dream state types */
void DreamCheckTeleport(int stateType)
{
    if (stateType != 0x4e) {
        if (stateType < 0x4f) {
            if ((stateType != 0xb) && (stateType != 0x38))
                return;
        } else if (stateType != 0x5d) {
            return;
        }
    }
    DreamInstantTeleportFlag = 1;
}

