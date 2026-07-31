/* Rendering_Entity.c -- split from Rendering.c */
#include "Rendering_Shared.h"
void Entity_Setup(int *index)
{
  int iVar1;
  ((int (*)(void))(*(void **)(*index + 0xf8)))();
  iVar1 = GetGameManagerAlias();
  ((int (*)(int))(*(void **)(iVar1 + 0xc)))(index);
}

void Entity_UpdateCond(int *index, uint *id, int index_2)
{
  int iVar1;
  iVar1 = GetGameManagerAlias();
  ((int (*)(int,int,int))(*(void **)(iVar1 + 0x38)))(index,id,index);
  if (((*(short *)*id == 0x5f03) && (index_2 == 1)) && (((EntityObj *)index)->nField_60 == 0)) {
    ((int (*)(int))(*(void **)(*index + 4)))(index);
  }
}

void Entity_Init(int *index)
{
  int iVar1;
  iVar1 = GetGameManagerAlias();
  ((int (*)(int,int))(*(void **)(iVar1 + 0x60)))(index,0);
  ((int (*)(int,int))(*(void **)(*index + 0xf0)))(index,1);
  ((int (*)(int,int))(*(void **)(*index + 0xe4)))(index,300);
  ((int (*)(int))(*(void **)(*index + 0x114)))(index);
  ((int (*)(int,int))(*(void **)(*index + 0x10c)))(index,0x41);
  ((int (*)(int))(*(void **)(*index + 0x130)))(index);
  ((int (*)(int,int))(*(void **)(*index + 0x128)))(index,0);
  if (((EntityObj *)index)->nObjState != 0) {
    GsLinkObject2(index,*(uint *)(((EntityObj *)index)->nObjState + 0x20));
  }
}

void Entity_SetupSub(int *index, uint id, int index_2, uint type, uint flags)
{
  int iVar1;
  if (index[3] == 0) {
    iVar1 = GetGameManagerAlias();
    ((int (*)(int,int,int))(*(void **)(iVar1 + 0x4c)))(index,type,flags);
    if ((index_2 != 0) && (((EntityObj *)index)->nField_50 == 0)) {
      ((int (*)(int,int))(*(void **)(*index + 0x10)))(index,index);
    }
    ((int (*)(int,int))(*(void **)(*index + 0x13c)))(index,id);
  }
}

void Entity_CleanupSub(int *index)
{
  int iVar1;
  if (index[3] != 0) {
    ((int (*)(void))(*(void **)(*index + 0x140)))();
    if (((EntityObj *)index)->nField_50 != 0) {
      ((int (*)(int))(*(void **)(*index + 0x14)))(index);
    }
    iVar1 = GetGameManagerAlias();
    ((int (*)(int))(*(void **)(iVar1 + 0x50)))(index);
  }
}

void Entity_ForEachChild(int index,uint id)
{
  int iVar1;
  uint *puVar2;
  puVar2 = *(uint **)(index + 0x70);
  iVar1 = 0;
  if (0 < *(int *)(index + 0x6c)) {
    do {
      iVar1 = iVar1 + 1;
      ((int (*)(int,int))(*(void **)(*(int *)*puVar2 + 0x60)))((int *)*puVar2,id);
      puVar2 = puVar2 + 1;
    } while (iVar1 < *(int *)(index + 0x6c));
  }
}

void Entity_NotifyChildren(int index,uint id)
{
  int iVar1;
  uint *puVar2;
  puVar2 = *(uint **)(index + 0x70);
  iVar1 = 0;
  if (0 < *(int *)(index + 0x6c)) {
    do {
      iVar1 = iVar1 + 1;
      ((int (*)(int,int))(*(void **)(*(int *)*puVar2 + 0x70)))((int *)*puVar2,id);
      puVar2 = puVar2 + 1;
    } while (iVar1 < *(int *)(index + 0x6c));
  }
  iVar1 = GetGameManagerAlias();
  ((int (*)(int,int))(*(void **)(iVar1 + 0x70)))(index,id);
}

void Entity_HandleEvent(int *index, uint id, int index_2)
{
  if (index_2 == 2) {
    ((int (*)(void))(*(void **)(*index + 0x108)))();
  }
  if (index_2 == 4) {
    ((int (*)(int))(*(void **)(*index + 4)))(index);
  }
}

void Entity_SetSoundFlag(int index,uint id)
{
  *(uint *)(index + 100) = id;
}

uint Entity_SoundInitCheck(int index, int index_2)
{
  uint uVar1;
  uVar1 = 0;
  if (*(int *)(index + 0x5c) == 0) {
    uVar1 = Entity_SoundInit((int *)index,index);
  }
  return uVar1;
}

void Entity_SoundDestroyCheck(int index)
{
  if (*(int *)(index + 0x5c) != 0) {
    Entity_SoundDestroy();
  }
}

uint Entity_SoundInit(int *index, int index_2)
{
  int iVar1;
  uint uVar2;
  if (*(int *)(index + 0xc) == 0) {
    iVar1 = Spu_New();
    ((EntityObj *)index)->nField_5c = iVar1;
    ((EntityObj *)index)->nField_60 = 1;
  }
  else {
    ((EntityObj *)index)->nField_5c = *(int *)(index + 0xc);
    ((EntityObj *)index)->nField_60 = 0;
  }
  if (((EntityObj *)index)->nField_5c == 0) {
    Entity_SoundDestroy(index);
    uVar2 = 1;
  }
  else {
    uVar2 = ((int (*)(int))(*(void **)(*index + 0x100)))(index);
  }
  return uVar2;
}

void Entity_SoundDestroy(int *index)
{
  int iVar1;
  ((int (*)(void))(*(void **)(*index + 0x104)))();
  if (((EntityObj *)index)->nField_60 == 0) {
    ((EntityObj *)index)->nField_5c = 0;
  }
  else {
    iVar1 = ((int (*)(void))(*(void **)(*(int *)((EntityObj *)index)->nField_5c + 4)))();
    ((EntityObj *)index)->nField_5c = iVar1;
  }
}

int Entity_FindChild(int index,char c)
{
  char *pcVar1;
  int iVar2;
  pcVar1 = *(char **)(index + 0x74);
  if (pcVar1 != (char *)0x0) {
    iVar2 = 0;
    if (0 < *(int *)(index + 0x6c)) {
      do {
        if (*pcVar1 == c) {
          return iVar2;
        }
        iVar2 = iVar2 + 1;
        pcVar1 = pcVar1 + 1;
      } while (iVar2 < *(int *)(index + 0x6c));
    }
  }
  return -1;
}

uint Entity_ParticleInitCheck(int index)
{
  uint uVar1;
  uVar1 = 0;
  if (*(int *)(index + 0x70) == 0) {
    uVar1 = Entity_ParticleInit(index);
  }
  return uVar1;
}

void Entity_ParticleDestroyCheck(int index)
{
  if (*(int *)(index + 0x70) != 0) {
    Entity_ParticleDestroy();
  }
}

uint Entity_ParticleInit(int index)
{
  uint uVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int local_28 [4];
  uVar1 = ((int (*)(int,int,int))(*(void **)(**(int **)(index + 0x5c) + 0x80)))(*(int **)(index + 0x5c),0,local_28);
  uVar1 = uVar1 & 0xff;
  iVar2 = MemAllocImpl(uVar1 << 2);
  *(int *)(index + 0x70) = iVar2;
  if (iVar2 != 0) {
    iVar2 = MemAllocImpl(uVar1);
    *(int *)(index + 0x74) = iVar2;
    if (iVar2 != 0) {
      ((int (*)(int,int,int))(*(void **)(**(int **)(index + 0x5c) + 0x80)))(*(int **)(index + 0x5c),iVar2,local_28);
      piVar4 = *(int **)(index + 0x70);
      iVar2 = 0;
      *(uint *)(index + 0x6c) = 0;
      if (uVar1 != 0) {
        do {
          iVar3 = ParticlePrimitiveAlloc();
          *piVar4 = iVar3;
          piVar4 = piVar4 + 1;
          if (iVar3 == 0) goto LAB_80065f00;
          iVar2 = iVar2 + 1;
          *(int *)(index + 0x6c) = *(int *)(index + 0x6c) + 1;
        } while (iVar2 < (int)uVar1);
      }
      *(uint *)(index + 0x68) = *(uint *)(local_28[0] * 4 + *(int *)(index + 0x70));
      return 0;
    }
  }
  *(uint *)(index + 0x74) = 0;
LAB_80065f00:
  Entity_ParticleDestroy(index);
  return 1;
}

void Entity_ParticleDestroy(int index)
{
  uint uVar1;
  int iVar2;
  uint *puVar3;
  int *piVar4;
  puVar3 = *(uint **)(index + 0x70);
  if ((puVar3 != (uint *)0x0) && (*(int *)(index + 0x74) != 0)) {
    iVar2 = *(int *)(index + 0x6c);
    while (*(int *)(index + 0x6c) = iVar2 + -1, 0 < iVar2) {
      piVar4 = (int *)*puVar3;
      puVar3 = puVar3 + 1;
      ((int (*)(void))(*(void **)(*piVar4 + 4)))();
      iVar2 = *(int *)(index + 0x6c);
    }
    *(uint *)(index + 0x68) = 0;
  }
  uVar1 = MemFreeImpl(*(uint *)(index + 0x74));
  *(uint *)(index + 0x74) = uVar1;
  uVar1 = MemFreeImpl(*(uint *)(index + 0x70));
  *(uint *)(index + 0x70) = uVar1;
}

void Entity_TickAnim(int *index)
{
  int iVar1;
  index[9] = index[9] + 1;
  if (((EntityObj *)index)->pSubVtbl != 0) {
    (*(code *)((EntityObj *)index)->nRndVal)();
  }
  if ((((EntityObj *)index)->nParam2 != 0) && (1 < ((EntityObj *)index)->nCondition)) {
    iVar1 = ((int (*)(int,int,int))(*(void **)(*index + 0x134)))(index,((EntityObj *)index)->pChild2,0);
    ((EntityObj *)index)->pChild2 = iVar1;
    iVar1 = ((EntityObj *)index)->nSubState;
    ((EntityObj *)index)->nSubState = iVar1 + 1;
    if (((EntityObj *)index)->nCondition <= iVar1 + 1) {
      ((EntityObj *)index)->nSubState = 0;
      ((EntityObj *)index)->pChild2 =
           *(int *)(*(int *)((int)((EntityObj *)index)->pSubObj * 4 + *(int *)(*(int *)(((EntityObj *)index)->nField_5c + 0x30) + 0x10) + 8
                            ) + 0x10) + 8;
    }
  }
  *(uint *)index[5] = 0;
}

void Entity_SetAnimCallback(int *index,byte val)
{
  if (val == 0x42) {
    ((EntityObj *)index)->nRndVal = *(int *)(*index + 0x11c);
  }
  else if (val < 0x43) {
    if (val == 0x41) {
      ((EntityObj *)index)->nRndVal = *(int *)(*index + 0x118);
    }
  }
  else if (val == 0x43) {
    ((EntityObj *)index)->nRndVal = *(int *)(*index + 0x120);
  }
}

void Entity_Show(int index)
{
  *(uint *)(index + 0x8c) = 1;
}

void Entity_Hide(int index)
{
  *(uint *)(index + 0x8c) = 0;
}

void Entity_UpdateVisible(int *index)
{
  int *piVar1;
  ((int (*)(int,int,int))(*(void **)(*index + 0xc4)))(index,0xffffffe2,0);
  if ((((EntityObj *)index)->nField_64 == 1) && (piVar1 = (int *)((EntityObj *)index)->nObjState, piVar1 != NULL)) {
    ((int (*)(int,int))(*(void **)(*piVar1 + 0x88)))(piVar1,6);
  }
}

void Entity_Nop(void)
{
}

void Entity_Nop2(void)
{
}

void Entity_SendToParent(int index,uint id)
{
  int *piVar1;
  piVar1 = *(int **)(index + 0x58);
  if (piVar1 != NULL) {
    ((int (*)(int,int,int,int))(*(void **)(*piVar1 + 0x80)))(piVar1,id,0x6e,0x6e);
  }
}

void Entity_SelectAnim(int *index, int index_2)
{
  int iVar1;
  ((EntityObj *)index)->pSubObj = index;
  ((EntityObj *)index)->nCondition =
       *(int *)(*(int *)(*(int *)((int)index * 4 + *(int *)(*(int *)(((EntityObj *)index)->nField_5c + 0x30) + 0x10) + 8)
                        + 0x10) + 4);
  iVar1 = *(int *)(*(int *)((int)((EntityObj *)index)->pSubObj * 4 + *(int *)(*(int *)(((EntityObj *)index)->nField_5c + 0x30) + 0x10) + 8)
                  + 0x10);
  ((EntityObj *)index)->nSubState = 0;
  ((EntityObj *)index)->pChild2 = iVar1 + 8;
  ((int (*)(int,int,int))(*(void **)(*index + 0x134)))(index,((EntityObj *)index)->pChild2,0);
}

void Entity_EnableSpawn(int index)
{
  *(uint *)(index + 0x90) = 1;
}

void Entity_DisableSpawn(int index)
{
  *(uint *)(index + 0x90) = 0;
}

int Entity_ParseCmdList(int *index, int index_2, uint id)
{
  ushort uVar1;
  bool bVar2;
  uint uVar3;
  uVar1 = *(ushort *)(index + 2);
  index_2 = index + 8;
  if (uVar1 != 0) {
    uVar3 = 1;
    do {
      index_2 = ((int (*)(int,int,int))(*(void **)(*index + 0x138)))(index,index,id);
      bVar2 = uVar3 < uVar1;
      uVar3 = uVar3 + 1;
    } while (bVar2);
  }
  return index;
}

int Entity_ProcessAnimCmds(int index,int value)
{
  uint *puVar1;
  int iVar2;
  uint *puVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  code *pcVar7;
  int iVar8;
  int iVar9;
  ushort *puVar10;
  uint *puVar11;
  int *piVar12;
  int *piVar13;
  int *piVar14;
  u8 local_20;
  byte local_1f;
  byte local_1e;
  byte local_1d [5];
  puVar1 = (uint *)((int (*)(int,int,int,int,int,int))(*(void **)(**(int **)(index + 0x5c) + 0x84)))(*(uint *)(index + 0x5c),value,&local_20,&local_1f,&local_1e,
                              local_1d);
  iVar2 = Entity_FindChild(index,local_20);
  if (-1 < iVar2) {
    piVar14 = *(int **)(iVar2 * 4 + *(int *)(index + 0x70));
    puVar3 = (uint *)piVar14[5];
    *puVar3 = 0;
    piVar13 = (int *)puVar3[0x11];
    if (local_1f == 1) {
      if ((local_1e & 1) == 0) {
        if ((local_1e & 2) != 0) {
          piVar12 = piVar13 + 4;
          iVar2 = 0;
          puVar11 = puVar1;
          do {
            uVar5 = *puVar11;
            puVar11 = puVar11 + 1;
            iVar2 = iVar2 + 1;
            *(short *)piVar12 =
                 (short)((ulonglong)((longlong)(int)uVar5 * 0xb60b60b7) >> 0x28) -
                 (short)((int)uVar5 >> 0x1f);
            piVar12 = (int *)((int)piVar12 + 2);
          } while (iVar2 < 3);
          puVar1 = puVar1 + 3;
        }
        if ((local_1e & 4) != 0) {
          iVar2 = 0;
          puVar11 = puVar1;
          piVar12 = piVar13;
          do {
            uVar5 = *puVar11;
            puVar11 = (uint *)((int)puVar11 + 2);
            iVar2 = iVar2 + 1;
            *piVar12 = (int)(short)(ushort)uVar5;
            piVar12 = piVar12 + 1;
          } while (iVar2 < 3);
          puVar1 = puVar1 + 2;
        }
        puVar11 = (uint *)(piVar13 + 6);
        if ((local_1e & 8) == 0) goto LAB_80066718;
        iVar2 = 0;
        do {
          uVar5 = *puVar1;
          puVar1 = puVar1 + 1;
          iVar2 = iVar2 + 1;
          *puVar11 = uVar5;
          puVar11 = puVar11 + 1;
        } while (iVar2 < 3);
      }
      else {
        if ((local_1e & 2) != 0) {
          puVar10 = (ushort *)(piVar13 + 4);
          iVar2 = 0;
          puVar11 = puVar1;
          do {
            iVar9 = (uint)*puVar10 + (int)*puVar11 / 0x168;
            iVar4 = iVar9 * FIXED_16;
            iVar8 = iVar4 >> 0x10;
            *puVar10 = (ushort)iVar9;
            if (iVar8 < 0) {
              iVar8 = iVar8 + 0xfff;
            }
            *puVar10 = (short)((uint)iVar4 >> 0x10) + (short)(iVar8 >> 0xc) * -FIXED_12;
            puVar11 = puVar11 + 1;
            iVar2 = iVar2 + 1;
            puVar10 = puVar10 + 1;
          } while (iVar2 < 3);
          puVar1 = puVar1 + 3;
        }
        if ((local_1e & 4) != 0) {
          iVar2 = 0;
          piVar12 = piVar13;
          puVar11 = puVar1;
          do {
            iVar4 = (int)(short)(ushort)*puVar11 * *piVar12;
            puVar11 = (uint *)((int)puVar11 + 2);
            if (iVar4 < 0) {
              iVar4 = iVar4 + 0xfff;
            }
            *piVar12 = iVar4 >> 0xc;
            iVar2 = iVar2 + 1;
            piVar12 = piVar12 + 1;
          } while (iVar2 < 3);
          puVar1 = puVar1 + 2;
        }
        piVar12 = piVar13 + 6;
        if ((local_1e & 8) == 0) goto LAB_80066718;
        iVar2 = 0;
        do {
          uVar5 = *puVar1;
          puVar1 = puVar1 + 1;
          iVar2 = iVar2 + 1;
          *piVar12 = *piVar12 + uVar5;
          piVar12 = piVar12 + 1;
        } while (iVar2 < 3);
      }
      iVar2 = piVar14[5];
      iVar4 = piVar13[7];
      iVar8 = piVar13[8];
      *(int *)(iVar2 + 0x18) = piVar13[6];
      *(int *)(iVar2 + 0x1c) = iVar4;
      *(int *)(iVar2 + 0x20) = iVar8;
    }
    else if (local_1f < 2) {
      if (local_1f == 0) {
        piVar14[4] = piVar14[4] & *puVar1 | puVar1[1];
      }
    }
    else if (local_1f == 2) {
      if (((ushort)*puVar1 != 0) && (piVar14[8] == 0)) {
        piVar13 = *(int **)(*(int *)(index + 0x5c) + 0x2c);
        uVar6 = ((int (*)(int,int))(*(void **)(*piVar13 + 0x80)))(piVar13,(ushort)*puVar1 - 1);
        GsLinkObject2(piVar14,uVar6);
      }
    }
    else if (local_1f == 3) {
      if ((*puVar1 == 0) || (*puVar1 == 0xffff)) {
        pcVar7 = *(code **)(*piVar14 + 0x4c);
      }
      else {
        iVar2 = Entity_FindChild(index,(char)*puVar1);
        index = *(int *)(iVar2 * 4 + *(int *)(index + 0x70));
        pcVar7 = *(code **)(*piVar14 + 0x4c);
      }
      ((int (*)(...))(pcVar7))(piVar14,index,0);
    }
  }
LAB_80066718:
  return value + (uint)local_1d[0] * 4;
}

void Entity_LinkToParent(int *index,int *value)
{
  if (value != NULL) {
    ((int (*)(int,int))(*(void **)(*value + 0x10)))(value,index);
    ((int (*)(int,int))(*(void **)(*index + 0x10)))(index,value);
    ((EntityObj *)index)->pContainer = (int)value;
  }
}

void Entity_UnlinkParent(int *index)
{
  int *piVar1;
  piVar1 = (int *)((EntityObj *)index)->pContainer;
  if (piVar1 != NULL) {
    ((int (*)(int,int))(*(void **)(*piVar1 + 0x14)))(piVar1,index);
    ((int (*)(int,int))(*(void **)(*index + 0x14)))(index,((EntityObj *)index)->pContainer);
    ((EntityObj *)index)->pContainer = 0;
  }
}
void * GetDebugInputVtable(void)
{
  return &Other_DebugInputVtable;
}
/* === migrated from stubs.c: gte_ReadRotMatrix === */
void gte_ReadRotMatrix(int *m) {
    __asm__ volatile("cfc2 %0, $1" : "=r"(m[0]));
    __asm__ volatile("cfc2 %0, $2" : "=r"(m[1]));
    __asm__ volatile("cfc2 %0, $3" : "=r"(m[2]));
}
/* === migrated from stubs.c: gte_SetRotMatrix === */
void gte_SetRotMatrix(int *m) {
    __asm__ volatile("ctc2 %0, $1" : : "r"(m[0]));
    __asm__ volatile("ctc2 %0, $2" : : "r"(m[1]));
    __asm__ volatile("ctc2 %0, $3" : : "r"(m[2]));
}
/* === migrated from stubs.c: gte_ldv0 === */
void gte_ldv0(int *v) {
    __asm__ volatile("lwc2 $0, 0(%0); lwc2 $1, 4(%0)" : : "r"(v) : "memory");
}
/* === migrated from stubs.c: gte_ldv3 === */
void gte_ldv3(int *v) {
    __asm__ volatile(
        "lwc2 $0, 0(%0); lwc2 $1, 4(%0);"
        "lwc2 $2, 8(%0); lwc2 $3, 12(%0);"
        "lwc2 $4, 16(%0); lwc2 $5, 20(%0)"
        : : "r"(v) : "memory");
}
/* === migrated from stubs.c: gte_stsxy === */
void gte_stsxy(int *v) {
    __asm__ volatile("swc2 $14, 0(%0)" : : "r"(v) : "memory");
}
/* === migrated from stubs.c: gte_stsxy3 === */
void gte_stsxy3(int *v) {
    __asm__ volatile("swc2 $12, 0(%0); swc2 $13, 4(%0); swc2 $14, 8(%0)"
        : : "r"(v) : "memory");
}
/* === migrated from stubs.c: gte_rtps === */
void gte_rtps(int *v) { (void)v; __asm__ volatile("nop; nop; .word 0x0000007f"); }
/* === migrated from stubs.c: gte_rtpt === */
void gte_rtpt(int *v) { (void)v; __asm__ volatile("nop; nop; .word 0x000000bf"); }
/* === migrated from stubs.c: gte_rtir === */
void gte_rtir(int *v) { (void)v; __asm__ volatile("nop; nop; .word 0x0000013f"); }
/* === migrated from stubs.c: gte_ncds === */
void gte_ncds(int *v) { (void)v; __asm__ volatile("nop; nop; .word 0x0000053f"); }
/* === migrated from stubs.c: gte_nclip === */
void gte_nclip(int *v) { (void)v; __asm__ volatile("nop; nop; .word 0x0000117f"); }
/* === migrated from stubs.c: gte_avsz3 === */
void gte_avsz3(int *v) { (void)v; __asm__ volatile("nop; nop; .word 0x00001dbf"); }
/* === migrated from stubs.c: gte_dpcs === */
void gte_dpcs(int *v) { (void)v; __asm__ volatile("nop; nop; .word 0x0000193f"); }
/* === migrated from stubs.c: gte_dpct === */
void gte_dpct(int *v) { (void)v; __asm__ volatile("nop; nop; .word 0x000019bf"); }
/* === migrated from stubs.c: gte_ldclmv === */
void gte_ldclmv(int *v) { (void)v; __asm__ volatile("nop; nop; .word 0x00001fbf"); }
/* === migrated from stubs.c: gte_ldrgb === */
void gte_ldrgb(int *v) { __asm__ volatile("lwc2 $17, 0(%0)" : : "r"(v) : "memory"); }
/* === migrated from stubs.c: gte_ldrgb3 === */
void gte_ldrgb3(int *v) {
    __asm__ volatile("lwc2 $17, 0(%0); lwc2 $18, 4(%0); lwc2 $19, 8(%0)"
        : : "r"(v) : "memory");
}
/* === migrated from stubs.c: gte_ldrgb3c === */
void gte_ldrgb3c(int *v) {
    __asm__ volatile("lwc2 $15, 0(%0); lwc2 $17, 4(%0); lwc2 $18, 8(%0); lwc2 $19, 12(%0)"
        : : "r"(v) : "memory");
}
/* === migrated from stubs.c: gte_strgb === */
void gte_strgb(int *v) { __asm__ volatile("swc2 $17, 0(%0)" : : "r"(v) : "memory"); }
/* === migrated from stubs.c: gte_strgb3 === */
void gte_strgb3(int *v) {
    __asm__ volatile("swc2 $17, 0(%0); swc2 $18, 4(%0); swc2 $19, 8(%0)"
        : : "r"(v) : "memory");
}
/* === migrated from stubs.c: gte_strgb3_g3 === */
void gte_strgb3_g3(int *v) {
    __asm__ volatile("swc2 $17, 0(%0); swc2 $18, 4(%0); swc2 $19, 8(%0)"
        : : "r"(v) : "memory");
}
/* === migrated from stubs.c: gte_stclmv === */
void gte_stclmv(int *v) { __asm__ volatile("swc2 $25, 0(%0)" : : "r"(v) : "memory"); }
/* === migrated from stubs.c: gte_stdp === */
void gte_stdp(int *v) { __asm__ volatile("swc2 $26, 0(%0)" : : "r"(v) : "memory"); }
/* === migrated from stubs.c: gte_stflg_4 === */
void gte_stflg_4(int *v) { __asm__ volatile("cfc2 %0, $31" : "=r"(*(int*)v) : : ); }
/* === migrated from stubs.c: gte_stopz === */
void gte_stopz(int *v) { __asm__ volatile("swc2 $7, 0(%0)" : : "r"(v) : "memory"); }
/* === migrated from stubs.c: gte_stotz === */
void gte_stotz(int *v) { __asm__ volatile("swc2 $7, 0(%0)" : : "r"(v) : "memory"); }
/* === migrated from stubs.c: gte_stsz3 === */
void gte_stsz3(int *v) {
    __asm__ volatile("swc2 $20, 0(%0); swc2 $21, 4(%0); swc2 $22, 8(%0)"
        : : "r"(v) : "memory");
}
/* === migrated from stubs.c: gte_stsz4 === */
void gte_stsz4(int *v) {
    __asm__ volatile("swc2 $20, 0(%0); swc2 $21, 4(%0); swc2 $22, 8(%0); swc2 $23, 12(%0)"
        : : "r"(v) : "memory");
}
/* === migrated from stubs.c: OuterProduct0 === */
void OuterProduct0(VECTOR *v0, VECTOR *v1, VECTOR *dest) {
    dest->vx = v0->vy * v1->vz - v0->vz * v1->vy;
    dest->vy = v0->vz * v1->vx - v0->vx * v1->vz;
    dest->vz = v0->vx * v1->vy - v0->vy * v1->vx;
}
/* === migrated from stubs.c: GetPrimitiveBaseTable === */
void *GetPrimitiveBaseTable(void) { return (void *)0x8006b5cc; }
/* === migrated from stubs.c === */
/* BuildVisibleChunkListSingle */
int BuildVisibleChunkListSingle(void *grid)
{
    /* Grid structure: [0]=vtable, [1]=chunkCount, [2]=worldPos, [3+]=chunkRefs */
    int count = 0;
    int *g = (int *)grid;
    if (!g) return 0;
    int maxChunks = g[1];
    if (maxChunks <= 0 || maxChunks > 256) maxChunks = 64;
    int *chunks = g + 3;
    for (int i = 0; i < maxChunks; i++) {
        if (chunks[i] != 0)
            count++;
    }
    return count;
}
/* CreateRenderObject */
int CreateRenderObject(int chunkCol, int tileCol, int tileRow, int pos, int extra)
{
    (void)chunkCol; (void)tileCol; (void)tileRow; (void)pos; (void)extra;
    return New_Entity2(3, 0);
}
/* Forward declarations for RenderSceneView */
extern void *GetPrimitiveBaseTable(void);
extern void LAB_800196D4(void);
extern void LAB_800196E8(void);
extern void LAB_800196FC(void);
extern void LAB_80019710(void);
extern void LAB_80019724(void);
extern void LAB_8001974C(void);
extern void LAB_80019774(void);
extern void LAB_8001979C(void);
extern void RenderOpXor(int r0, int renderCtx);
extern int RenderOpAdd(int r0, int renderCtx, unsigned short a, unsigned short b, unsigned short c, void *d);
extern int RenderOpComplex(int r0, int renderCtx, unsigned short a, unsigned short b, unsigned short c, unsigned short d, void *e);
extern unsigned int *RenderOpBlack(unsigned int *r0, int renderCtx);
extern unsigned int *RenderOpFadeIn(unsigned int *r0, int renderCtx);
extern unsigned int *RenderOpSlide(unsigned int *r0, int renderCtx);
extern unsigned int *RenderOpMorph(unsigned int *r0, int renderCtx);
extern unsigned int *RenderOpBounce(unsigned int *r0, int renderCtx);
extern unsigned int *RenderOpShake(unsigned int *r0, int renderCtx);
extern unsigned int *RenderOpWave(unsigned int *r0, int renderCtx);
extern unsigned int *RenderOpOverlay(unsigned int *r0, int renderCtx);
extern void RenderOpCopyData(int *p, int base, int count);
/* RenderSceneView */
void RenderSceneView(unsigned int *renderState, int sceneObj, int modeVal, int *renderBuf)
{
    unsigned int *r0;
    unsigned int *puVar52;
    int iVar44;
    int iVar53;
    unsigned int uVar45;
    unsigned int uVar47;
    unsigned int uVar48;
    unsigned int uVar49;
    int *puVar43;
    CVECTOR *pCVar50;
    unsigned int *puVar51;
    if ((int)*renderState < 0) return;
    renderBuf[1] = modeVal;
    renderBuf[0] = *(int *)(sceneObj + 4);
    RenderOpCopyData(renderBuf + 0x22, (int)Other_OTEntryTriBuf, 3);
    RenderOpCopyData(renderBuf + 0x25, (int)Other_OTEntryQuadBuf, 4);
    puVar43 = (int *)renderState[2];
    iVar53 = puVar43[5];
    puVar52 = (unsigned int *)puVar43[4];
    renderBuf[3] = puVar43[0];
    renderBuf[4] = *(int *)(renderState[2] + 8);
    if (*(int *)(renderState[1] + 0x48) != 0) {
        gte_ReadRotMatrix(renderBuf + 0xe);
        uVar45 = renderState[1];
        gte_SetRotMatrix((int *)(*(int *)(uVar45 + 0x48) + 0x24));
        gte_ldclmv((int *)(uVar45 + 0x24));
        gte_rtir(0);
        gte_stclmv((int *)(uVar45 + 0x24));
        gte_ldclmv((int *)(renderState[1] + 0x26));
        gte_rtir(0);
        gte_stclmv((int *)(renderState[1] + 0x26));
        gte_ldclmv((int *)(renderState[1] + 0x28));
        gte_rtir(0);
        gte_stclmv((int *)(renderState[1] + 0x28));
        gte_SetRotMatrix(renderBuf + 0xe);
    }
    uVar45 = *renderState;
    uVar47 = *renderState;
    uVar48 = *renderState;
    uVar49 = *renderState;
    renderBuf[2] = 10;
    Gpu_PageMode = uVar45 >> 9 & 7;
    Gpu_DitherMode = uVar47 >> 6 & 1;
    Gpu_TexMapFlag = uVar48 >> 5 & 1;
    Gpu_SemiTrans = uVar49 >> 3 & 3;
    ((unsigned char *)renderBuf)[0x34] = 0x80;
    ((unsigned char *)renderBuf)[0x35] = 0x80;
    ((unsigned char *)renderBuf)[0x36] = 0x80;
    if (Gpu_TexMapFlag != 0 && Gpu_TexDisableFlag != 0)
        uVar45 = 9;
    else if (Gpu_SemiTrans != 0)
        uVar45 = 9;
    else
        uVar45 = 0x10;
    renderBuf[0xb] = uVar45;
    if (iVar53 == 0) return;
    do {
        r0 = Gpu_CurPrimPtr;
        renderBuf[6] = *(unsigned short *)((int)puVar52 + 2) & 0xfd07;
        uVar47 = (unsigned int)*(unsigned short *)puVar52;
        uVar45 = renderBuf[6];
        renderBuf[7] = *puVar52 >> 0x19 & 1;
        iVar53 = iVar53 - uVar47;
        if (uVar45 == PRIM_ID_QUAD_TEX) {
            ((unsigned char *)r0)[3] = 5;
            ((unsigned char *)r0)[7] = PRIM_HDR_QUAD;
            RenderOpXor((int)r0, (int)renderBuf);
            pCVar50 = (CVECTOR *)(puVar52 + 1);
            do {
                iVar44 = RenderOpComplex((int)r0, (int)renderBuf,
                    *(unsigned short *)(&pCVar50[1].b),
                    *(unsigned short *)(&pCVar50[1].cd),
                    *(unsigned short *)(&pCVar50[2].r),
                    *(unsigned short *)(&pCVar50[2].b),
                    &LAB_80019724);
                puVar52 = puVar52 + 4;
                if (iVar44 == 0) {
                    gte_ldrgb((int *)pCVar50);
                    gte_dpcs(0);
                    gte_strgb((int *)(r0 + 1));
                    ((unsigned char *)r0)[7] = ((unsigned char *)renderBuf)[0x15];
                    r0 = RenderOpSlide(r0, (int)renderBuf);
                }
                uVar47 = uVar47 - 1;
                pCVar50 = pCVar50 + 4;
            } while (uVar47 != 0);
        }
        else if (uVar45 < 0x2902) {
            if (uVar45 == PRIM_ID_TRI_TEX) {
                ((unsigned char *)r0)[3] = 4;
                ((unsigned char *)r0)[7] = PRIM_HDR_TRI;
                RenderOpXor((int)r0, (int)renderBuf);
                pCVar50 = (CVECTOR *)(puVar52 + 1);
                do {
                    iVar44 = RenderOpAdd((int)r0, (int)renderBuf,
                        *(unsigned short *)(&pCVar50[1].r),
                        *(unsigned short *)(&pCVar50[1].b),
                        *(unsigned short *)(&pCVar50[2].r),
                        &LAB_800196D4);
                    puVar52 = puVar52 + 4;
                    if (iVar44 == 0) {
                        gte_ldrgb((int *)pCVar50);
                        gte_dpcs(0);
                        gte_strgb((int *)(r0 + 1));
                        ((unsigned char *)r0)[7] = ((unsigned char *)renderBuf)[0x15];
                        r0 = RenderOpBlack(r0, (int)renderBuf);
                    }
                    uVar47 = uVar47 - 1;
                    pCVar50 = pCVar50 + 4;
                } while (uVar47 != 0);
            }
            else if (uVar45 == PRIM_ID_TRI) {
                ((unsigned char *)r0)[3] = 4;
                ((unsigned char *)r0)[7] = PRIM_HDR_TRI;
                RenderOpXor((int)r0, (int)renderBuf);
                pCVar50 = (CVECTOR *)(puVar52 + 1);
                do {
                    iVar44 = RenderOpAdd((int)r0, (int)renderBuf,
                        *(unsigned short *)(&pCVar50[1].b),
                        *(unsigned short *)(&pCVar50[2].r),
                        *(unsigned short *)(&pCVar50[2].b),
                        &LAB_800196D4);
                    puVar52 = puVar52 + 4;
                    if (iVar44 == 0) {
                        gte_ldv0((int *)((unsigned int)*(unsigned short *)(&pCVar50[1].r) * 8 + renderBuf[4]));
                        gte_ldrgb((int *)pCVar50);
                        gte_ncds(0);
                        gte_strgb((int *)(r0 + 1));
                        ((unsigned char *)r0)[7] = ((unsigned char *)renderBuf)[0x15];
                        r0 = RenderOpBlack(r0, (int)renderBuf);
                    }
                    uVar47 = uVar47 - 1;
                    pCVar50 = pCVar50 + 4;
                } while (uVar47 != 0);
            }
            else if (uVar45 == PRIM_ID_TEXTURED_TRI) {
                ((unsigned char *)r0)[3] = 6;
                ((unsigned char *)r0)[7] = PRIM_HDR_GOURAUD_TRI;
                RenderOpXor((int)r0, (int)renderBuf);
                pCVar50 = (CVECTOR *)(puVar52 + 3);
                do {
                    iVar44 = RenderOpAdd((int)r0, (int)renderBuf,
                        *(unsigned short *)(&pCVar50[1].b),
                        *(unsigned short *)(&pCVar50[2].r),
                        *(unsigned short *)(&pCVar50[2].b),
                        &LAB_800196E8);
                    if (iVar44 == 0) {
                        gte_ldv0((int *)((unsigned int)*(unsigned short *)(&pCVar50[1].r) * 8 + renderBuf[4]));
                        gte_ldrgb((int *)(puVar52 + 1));
                        gte_ncds(0);
                        gte_strgb((int *)(r0 + 1));
                        ((unsigned char *)r0)[7] = ((unsigned char *)renderBuf)[0x15];
                        gte_ldrgb((int *)(puVar52 + 2));
                        gte_ncds(0);
                        gte_strgb((int *)(r0 + 3));
                        gte_ldrgb((int *)pCVar50);
                        gte_ncds(0);
                        gte_strgb((int *)(r0 + 5));
                        r0 = RenderOpFadeIn(r0, (int)renderBuf);
                    }
                    pCVar50 = pCVar50 + 6;
                    uVar47 = uVar47 - 1;
                    puVar52 = puVar52 + 6;
                } while (uVar47 != 0);
            }
            else if (uVar45 == PRIM_ID_SPRITE_16) {
                ((unsigned char *)r0)[3] = 7;
                ((unsigned char *)r0)[7] = PRIM_HDR_TEXTURED_TRI;
                RenderOpXor((int)r0, (int)renderBuf);
                pCVar50 = (CVECTOR *)(puVar52 + 4);
                do {
                    iVar44 = RenderOpAdd((int)r0, (int)renderBuf,
                        *(unsigned short *)(&pCVar50[1].r),
                        *(unsigned short *)(&pCVar50[1].b),
                        *(unsigned short *)(&pCVar50[2].r),
                        &LAB_800196FC);
                    puVar52 = puVar52 + 7;
                    if (iVar44 == 0) {
                        *(CVECTOR *)(r0 + 3) = pCVar50[-3];
                        *(CVECTOR *)(r0 + 5) = pCVar50[-2];
                        *(CVECTOR *)(r0 + 7) = pCVar50[-1];
                        *(short *)((int)r0 + 0xe) = *(short *)((int)r0 + 0xe) + (short)((int)renderBuf[9] >> (renderBuf[0xb] & 0x1f)) * 0x40;
                        gte_ldrgb((int *)pCVar50);
                        gte_dpcs(0);
                        gte_strgb((int *)(r0 + 1));
                        ((unsigned char *)r0)[7] = ((unsigned char *)renderBuf)[0x15];
                        r0 = RenderOpMorph(r0, (int)renderBuf);
                    }
                    uVar47 = uVar47 - 1;
                    pCVar50 = pCVar50 + 7;
                } while (uVar47 != 0);
            }
            else if (uVar45 == PRIM_ID_SPRITE_TEX) {
                ((unsigned char *)r0)[3] = 7;
                ((unsigned char *)r0)[7] = PRIM_HDR_TEXTURED_TRI;
                RenderOpXor((int)r0, (int)renderBuf);
                gte_ldrgb((int *)(renderBuf + 0xd));
                puVar51 = puVar52 + 4;
                do {
                    iVar44 = RenderOpAdd((int)r0, (int)renderBuf,
                        *(unsigned short *)((int)puVar51 + 2),
                        (unsigned short)puVar51[1],
                        *(unsigned short *)((int)puVar51 + 6),
                        &LAB_800196FC);
                    puVar52 = puVar52 + 6;
                    if (iVar44 == 0) {
                        r0[3] = puVar51[-3];
                        r0[5] = puVar51[-2];
                        r0[7] = puVar51[-1];
                        *(short *)((int)r0 + 0xe) = *(short *)((int)r0 + 0xe) + (short)((int)renderBuf[9] >> (renderBuf[0xb] & 0x1f)) * 0x40;
                        gte_ldv0((int *)((unsigned int)*(unsigned short *)puVar51 * 8 + renderBuf[4]));
                        gte_ncds(0);
                        gte_strgb((int *)(r0 + 1));
                        ((unsigned char *)r0)[7] = ((unsigned char *)renderBuf)[0x15];
                        r0 = RenderOpMorph(r0, (int)renderBuf);
                    }
                    uVar47 = uVar47 - 1;
                    puVar51 = puVar51 + 6;
                } while (uVar47 != 0);
            }
            else if (uVar45 == PRIM_ID_QUAD) {
                ((unsigned char *)r0)[3] = 5;
                ((unsigned char *)r0)[7] = PRIM_HDR_QUAD;
                RenderOpXor((int)r0, (int)renderBuf);
                pCVar50 = (CVECTOR *)(puVar52 + 1);
                do {
                    iVar44 = RenderOpComplex((int)r0, (int)renderBuf,
                        *(unsigned short *)(&pCVar50[1].b),
                        *(unsigned short *)(&pCVar50[2].r),
                        *(unsigned short *)(&pCVar50[2].b),
                        *(unsigned short *)(&pCVar50[3].r),
                        &LAB_80019724);
                    puVar52 = puVar52 + 5;
                    if (iVar44 == 0) {
                        gte_ldv0((int *)((unsigned int)*(unsigned short *)(&pCVar50[1].r) * 8 + renderBuf[4]));
                        gte_ldrgb((int *)pCVar50);
                        gte_ncds(0);
                        gte_strgb((int *)(r0 + 1));
                        ((unsigned char *)r0)[7] = ((unsigned char *)renderBuf)[0x15];
                        r0 = RenderOpSlide(r0, (int)renderBuf);
                    }
                    uVar47 = uVar47 - 1;
                    pCVar50 = pCVar50 + 5;
                } while (uVar47 != 0);
            }
        }
        else if (uVar45 == PRIM_ID_GOURAUD_TRI_TEX) {
            ((unsigned char *)r0)[3] = 6;
            ((unsigned char *)r0)[7] = PRIM_HDR_GOURAUD_TRI;
            RenderOpXor((int)r0, (int)renderBuf);
            pCVar50 = (CVECTOR *)(puVar52 + 1);
            do {
                iVar44 = RenderOpAdd((int)r0, (int)renderBuf,
                    *(unsigned short *)(&pCVar50[3].r),
                    *(unsigned short *)(&pCVar50[3].b),
                    *(unsigned short *)(&pCVar50[4].r),
                    &LAB_800196E8);
                puVar52 = puVar52 + 6;
                if (iVar44 == 0) {
                    gte_ldrgb3c((int *)pCVar50);
                    gte_dpct(0);
                    gte_strgb3_g3((int *)r0);
                    ((unsigned char *)r0)[7] = ((unsigned char *)renderBuf)[0x15];
                    r0 = RenderOpFadeIn(r0, (int)renderBuf);
                }
                uVar47 = uVar47 - 1;
                pCVar50 = pCVar50 + 6;
            } while (uVar47 != 0);
        }
        else if (uVar45 == 0x2c00) {
            ((unsigned char *)r0)[3] = 9;
            ((unsigned char *)r0)[7] = PRIM_HDR_TEXTURED_QUAD;
            RenderOpXor((int)r0, (int)renderBuf);
            gte_ldrgb((int *)(renderBuf + 0xd));
            puVar51 = puVar52 + 5;
            do {
                iVar44 = RenderOpComplex((int)r0, (int)renderBuf,
                    *(unsigned short *)((int)puVar51 + 2),
                    (unsigned short)puVar51[1],
                    *(unsigned short *)((int)puVar51 + 6),
                    (unsigned short)puVar51[2],
                    &LAB_80019774);
                puVar52 = puVar52 + 8;
                if (iVar44 == 0) {
                    r0[3] = puVar51[-4];
                    r0[5] = puVar51[-3];
                    r0[7] = puVar51[-2];
                    r0[9] = puVar51[-1];
                    *(short *)((int)r0 + 0xe) = *(short *)((int)r0 + 0xe) + (short)((int)renderBuf[9] >> (renderBuf[0xb] & 0x1f)) * 0x40;
                    gte_ldv0((int *)((unsigned int)*(unsigned short *)puVar51 * 8 + renderBuf[4]));
                    gte_ncds(0);
                    gte_strgb((int *)(r0 + 1));
                    ((unsigned char *)r0)[7] = ((unsigned char *)renderBuf)[0x15];
                    r0 = RenderOpBounce(r0, (int)renderBuf);
                }
                uVar47 = uVar47 - 1;
                puVar51 = puVar51 + 8;
            } while (uVar47 != 0);
        }
        else if (uVar45 == 0x2d01) {
            ((unsigned char *)r0)[3] = 9;
            ((unsigned char *)r0)[7] = PRIM_HDR_TEXTURED_QUAD;
            RenderOpXor((int)r0, (int)renderBuf);
            pCVar50 = (CVECTOR *)(puVar52 + 5);
            do {
                iVar44 = RenderOpComplex((int)r0, (int)renderBuf,
                    *(unsigned short *)(&pCVar50[1].r),
                    *(unsigned short *)(&pCVar50[1].b),
                    *(unsigned short *)(&pCVar50[2].r),
                    *(unsigned short *)(&pCVar50[2].b),
                    &LAB_80019774);
                puVar52 = puVar52 + 8;
                if (iVar44 == 0) {
                    *(CVECTOR *)(r0 + 3) = pCVar50[-4];
                    *(CVECTOR *)(r0 + 5) = pCVar50[-3];
                    *(CVECTOR *)(r0 + 7) = pCVar50[-2];
                    *(CVECTOR *)(r0 + 9) = pCVar50[-1];
                    *(short *)((int)r0 + 0xe) = *(short *)((int)r0 + 0xe) + (short)((int)renderBuf[9] >> (renderBuf[0xb] & 0x1f)) * 0x40;
                    gte_ldrgb((int *)pCVar50);
                    gte_dpcs(0);
                    gte_strgb((int *)(r0 + 1));
                    ((unsigned char *)r0)[7] = ((unsigned char *)renderBuf)[0x15];
                    r0 = RenderOpBounce(r0, (int)renderBuf);
                }
                uVar47 = uVar47 - 1;
                pCVar50 = pCVar50 + 8;
            } while (uVar47 != 0);
        }
        else if (uVar45 == PRIM_ID_GOURAUD_QUAD_TEX) {
            ((unsigned char *)r0)[3] = 8;
            ((unsigned char *)r0)[7] = PRIM_HDR_GOURAUD_QUAD;
            RenderOpXor((int)r0, (int)renderBuf);
            pCVar50 = (CVECTOR *)(puVar52 + 4);
            do {
                iVar44 = RenderOpComplex((int)r0, (int)renderBuf,
                    *(unsigned short *)(&pCVar50[1].r),
                    *(unsigned short *)(&pCVar50[1].b),
                    *(unsigned short *)(&pCVar50[2].r),
                    *(unsigned short *)(&pCVar50[2].b),
                    &LAB_8001974C);
                if (iVar44 == 0) {
                    gte_ldrgb3c((int *)(puVar52 + 1));
                    gte_dpct(0);
                    gte_strgb3_g3((int *)r0);
                    ((unsigned char *)r0)[7] = ((unsigned char *)renderBuf)[0x15];
                    gte_ldrgb((int *)pCVar50);
                    gte_dpcs(0);
                    gte_strgb((int *)(r0 + 7));
                    r0 = RenderOpShake(r0, (int)renderBuf);
                }
                pCVar50 = pCVar50 + 7;
                uVar47 = uVar47 - 1;
                puVar52 = puVar52 + 7;
            } while (uVar47 != 0);
        }
        else if (uVar45 == PRIM_ID_SPRITE_SHADE_16) {
            ((unsigned char *)r0)[3] = 9;
            ((unsigned char *)r0)[7] = PRIM_HDR_GOURAUD_TEX_TRI;
            RenderOpXor((int)r0, (int)renderBuf);
            pCVar50 = (CVECTOR *)(puVar52 + 6);
            do {
                iVar44 = RenderOpAdd((int)r0, (int)renderBuf,
                    *(unsigned short *)(&pCVar50[1].r),
                    *(unsigned short *)(&pCVar50[1].b),
                    *(unsigned short *)(&pCVar50[2].r),
                    &LAB_80019710);
                uVar47 = uVar47 - 1;
                if (iVar44 == 0) {
                    *(CVECTOR *)(r0 + 3) = pCVar50[-5];
                    *(CVECTOR *)(r0 + 6) = pCVar50[-4];
                    *(CVECTOR *)(r0 + 9) = pCVar50[-3];
                    *(short *)((int)r0 + 0xe) = *(short *)((int)r0 + 0xe) + (short)((int)renderBuf[9] >> (renderBuf[0xb] & 0x1f)) * 0x40;
                    __asm__ volatile(
                        "lwc2 $17, 0(%0); lwc2 $18, 0(%1); lwc2 $19, 0(%2)"
                        : : "r"(puVar52 + 4), "r"(puVar52 + 5), "r"(pCVar50) : "memory");
                    gte_dpct(0);
                    __asm__ volatile(
                        "swc2 $17, 0(%0); swc2 $18, 0(%1); swc2 $19, 0(%2)"
                        : : "r"(r0 + 1), "r"(r0 + 4), "r"(r0 + 7) : "memory");
                    ((unsigned char *)r0)[7] = ((unsigned char *)renderBuf)[0x15];
                    r0 = RenderOpWave(r0, (int)renderBuf);
                }
                pCVar50 = pCVar50 + 9;
                puVar52 = puVar52 + 9;
            } while (uVar47 != 0);
        }
        else if (uVar45 == 0x3d01) {
            ((unsigned char *)r0)[3] = 0xc;
            ((unsigned char *)r0)[7] = PRIM_HDR_GOURAUD_TEX_QUAD;
            RenderOpXor((int)r0, (int)renderBuf);
            pCVar50 = (CVECTOR *)(puVar52 + 8);
            do {
                iVar44 = RenderOpComplex((int)r0, (int)renderBuf,
                    *(unsigned short *)(&pCVar50[1].r),
                    *(unsigned short *)(&pCVar50[1].b),
                    *(unsigned short *)(&pCVar50[2].r),
                    *(unsigned short *)(&pCVar50[2].b),
                    &LAB_8001979C);
                if (iVar44 == 0) {
                    *(CVECTOR *)(r0 + 3) = pCVar50[-7];
                    *(CVECTOR *)(r0 + 6) = pCVar50[-6];
                    *(CVECTOR *)(r0 + 9) = pCVar50[-5];
                    *(CVECTOR *)(r0 + 0xc) = pCVar50[-4];
                    *(short *)((int)r0 + 0xe) = *(short *)((int)r0 + 0xe) + (short)((int)renderBuf[9] >> (renderBuf[0xb] & 0x1f)) * 0x40;
                    __asm__ volatile(
                        "lwc2 $17, 0(%0); lwc2 $18, 0(%1); lwc2 $19, 0(%2)"
                        : : "r"(puVar52 + 5), "r"(puVar52 + 6), "r"(puVar52 + 7) : "memory");
                    gte_dpct(0);
                    __asm__ volatile(
                        "swc2 $17, 0(%0); swc2 $18, 0(%1); swc2 $19, 0(%2)"
                        : : "r"(r0 + 1), "r"(r0 + 4), "r"(r0 + 7) : "memory");
                    ((unsigned char *)r0)[7] = ((unsigned char *)renderBuf)[0x15];
                    gte_ldrgb((int *)pCVar50);
                    gte_dpcs(0);
                    gte_strgb((int *)(r0 + 10));
                    r0 = RenderOpOverlay(r0, (int)renderBuf);
                }
                pCVar50 = pCVar50 + 0xb;
                uVar47 = uVar47 - 1;
                puVar52 = puVar52 + 0xb;
            } while (uVar47 != 0);
        }
        Gpu_CurPrimPtr = r0;
    } while (iVar53 != 0);
}
/* UpdateChunkBoundaries */
void UpdateChunkBoundaries(int *grid)
{
    if (!grid) return;
    /* Grid layout: [0]=vtable, [1]=chunkCount, [2]=worldPos, [3+]=chunkRefs */
    int chunkCount = grid[1];
    if (chunkCount <= 0 || chunkCount > 256) return;
    int *chunks = grid + 3;
    for (int i = 0; i < chunkCount; i++) {
        int chunk = chunks[i];
        if (!chunk) continue;
        /* Chunk: [0]=vtable, [1]=x, [2]=z, [3]=width, [4]=depth, [5]=minY, [6]=maxY */
        /* Re-derive bounds from tile data for frustum culling */
        int cx = *(short*)(chunk + 4);
        int cz = *(short*)(chunk + 8);
        int cw = *(short*)(chunk + 12);
        int cd = *(short*)(chunk + 16);
        *(short*)(chunk + 20) = cx;           /* minX */
        *(short*)(chunk + 22) = cx + cw;       /* maxX */
        *(short*)(chunk + 24) = cz;           /* minZ */
        *(short*)(chunk + 26) = cz + cd;       /* maxZ */
    }
}
