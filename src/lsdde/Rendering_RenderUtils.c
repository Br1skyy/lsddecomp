/* Rendering_RenderUtils.c -- split from Rendering.c */
#include "Rendering_Shared.h"
void RenderUtils_CallDraw(uint id,int index)
{
  uint uVar1;
  int iVar2;
  if (index == 0) {
    uVar1 = rand();
    index = *(int *)(&ParticleRandomColourArray + (uVar1 & 3) * 4);
  }
  ParticleScratch_YPos = index;
  iVar2 = rand();
  ParticleScratch_XPos = (iVar2 % 0x17) * 0x800;
  uVar1 = rand();
  if ((uVar1 & 1) != 0) {
    ParticleScratch_XPos = -ParticleScratch_XPos;
  }
  iVar2 = rand();
  ParticleScratch_Move = (iVar2 % 0x17) * 0x800;
  uVar1 = rand();
  if ((uVar1 & 1) != 0) {
    ParticleScratch_Move = -ParticleScratch_Move;
  }
  uVar1 = rand();
  ParticleScratch_ColPtr = &ParticleColourTable + (uVar1 % 7) * 0xc;
  iVar2 = rand();
  ParticleScratch_Index = iVar2 % 5;
}

void RenderUtils_ApplyPpu(void)
{
  int iVar1;
  uint uVar2;
  uint uVar3;
  rand();
  ParticleScratch_YPos = 0xffffd800;
  iVar1 = rand();
  ParticleScratch_Move = 0xa000;
  ParticleScratch_XPos = (iVar1 % 0x14) * 0x800;
  if (ParticleEffectSelector % 3 == 1) {
    uVar2 = 0xffff6000;
  }
  else {
    uVar2 = 0x800;
    if (ParticleEffectSelector % 3 != 2) goto LAB_800554d0;
  }
  ParticleScratch_Move = uVar2;
LAB_800554d0:
  uVar3 = rand();
  ParticleScratch_ColPtr = &ParticleColourTable + (uVar3 % 7) * 0xc;
  iVar1 = rand();
  ParticleScratch_Index = iVar1 % 5;
}
int * RenderUtils_InitSubEntity(int *index,int *value)
{
  int iVar1;
  iVar1 = RenderUtils_CreateSub(index + 1,index + 4,value);
  if (iVar1 == 0) {
    index = NULL;
  }
  else {
    *index = iVar1;
    AnimInitializeBase(*((u32 *)(ParticleWorldPtr)),index + 5,(int)*(char *)(iVar1 + 6),index,
                 *(uint *)(&EntityLayerRegisterFuncTable + *(char *)(iVar1 + 6) * 4));
    if ((int)*(char *)(iVar1 + 6) == *value) {
      *value = -(int)*(char *)(iVar1 + 6);
    }
    *(char *)(iVar1 + 6) = -*(char *)(iVar1 + 6);
  }
  return index;
}
void * RenderUtils_CreateSub(int *index,int *value,int *arg2)
{
  u8 *puVar1;
  u16 uVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  char *pcVar6;
  void *puVar7;
  int iVar8;
  int iVar9;
  u8 auStack_40 [4];
  u8 auStack_3c [4];
  u16 local_38;
  if (arg2 != NULL) {
    iVar8 = 0;
    iVar9 = (uint)(byte)(&StageSecondaryEntityCount)[ParticleStageIndex] - ParticleEntityScanIndex;
    puVar7 = (&PTR_PTR_StageSecondaryEntityTablePtr)[ParticleStageIndex] + ParticleEntityScanIndex * 8;
    if (0 < iVar9) {
      pcVar6 = puVar7 + 6;
      do {
        ParticleEntityScanIndex = ParticleEntityScanIndex + 1;
        if (0 < *pcVar6) {
          uVar5 = (uint)(puVar7 + 3) & 3;
          uVar3 = (uint)puVar7 & 3;
          *(uint*)auStack_40 = ((*(int *)(puVar7 + 3 + -uVar5) << (3 - uVar5) * 8 |
                        (int)*pcVar6 & 0xffffffffU >> (uVar5 + 1) * 8) & -1 << (4 - uVar3) * 8 |
                       *(uint *)(puVar7 + -uVar3) >> uVar3 * 8);
          puVar1 = auStack_40 + 3;
          uVar5 = (uint)puVar1 & 3;
          *(uint *)(puVar1 + -uVar5) =
               *(uint *)(puVar1 + -uVar5) & -1 << (uVar5 + 1) * 8 |
               (uint)auStack_40 >> (3 - uVar5) * 8;
          iVar4 = (uint)(byte)pcVar6[-2] * 6;
          uVar5 = (uint)(&Rendering_PartColEf + iVar4) & 3;
          uVar3 = (uint)(&Rendering_PartColEc + iVar4) & 3;
          *(uint*)auStack_3c = ((*(int *)(&Rendering_PartColEf + iVar4 + -uVar5) << (3 - uVar5) * 8 |
                        (uint)(byte)pcVar6[-2] & 0xffffffffU >> (uVar5 + 1) * 8) &
                        -1 << (4 - uVar3) * 8 |
                       *(uint *)(&Rendering_PartColEc + iVar4 + -uVar3) >> uVar3 * 8);
          uVar2 = *(u16 *)(&Rendering_PartColF0 + iVar4);
          puVar1 = auStack_3c + 3;
          uVar5 = (uint)puVar1 & 3;
          *(uint *)(puVar1 + -uVar5) =
               *(uint *)(puVar1 + -uVar5) & -1 << (uVar5 + 1) * 8 |
               (uint)auStack_3c >> (3 - uVar5) * 8;
          local_38 = uVar2;
          ((int (*)(int,int,int))(*(void **)(*((u32 *)(ParticleSysEntityPtr)) + 0xe8)))(ParticleSysEntityPtr,index,auStack_40);
          uVar5 = *index - *arg2;
          if ((int)uVar5 < 0) {
            uVar5 = ~uVar5 + 1;
          }
          iVar4 = index[2] - arg2[2];
          if (iVar4 < 0) {
            iVar4 = -iVar4;
          }
          *value = uVar5 + iVar4;
          if ((int)(uVar5 + iVar4) < *(int *)(&SecondaryEntityRangeTable + *pcVar6 * 4)) {
            return puVar7;
          }
        }
        iVar8 = iVar8 + 1;
        pcVar6 = pcVar6 + 8;
        puVar7 = puVar7 + 8;
      } while (iVar8 < iVar9);
    }
  }
  return NULL;
}

uint RenderUtils_ClipSub(int *index)
{
  AnimReset(*((u32 *)(ParticleWorldPtr)),index + 5);
  *(char *)(*index + 6) = -*(char *)(*index + 6);
  return 0;
}

bool RenderUtils_CheckSeed(int index,u8 *arg1,uint id)
{
  int iVar1;
  iVar1 = RenderUtils_GetSub((int *)index,(int *)arg1);
  if (iVar1 != 0) {
    AnimTick(*((u32 *)(ParticleWorldPtr)),index + 0x14);
  }
  return iVar1 != 0;
}

uint RenderUtils_GetSub(int *index,int *value)
{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uVar1 = 0;
  if (value != NULL) {
    uVar3 = ((EntityObj *)index)->nPosX - *value;
    if ((int)uVar3 < 0) {
      uVar3 = ~uVar3 + 1;
    }
    iVar2 = ((EntityObj *)index)->nPosZ - value[2];
    if (iVar2 < 0) {
      iVar2 = -iVar2;
    }
    ((EntityObj *)index)->nDist = uVar3 + iVar2;
    uVar1 = 1;
    if (*(int *)(&SecondaryEntityRangeTable + *(char *)(*index + 6) * -4) <= (int)(uVar3 + iVar2)) {
      uVar1 = 0;
    }
  }
  return uVar1;
}

uint RenderUtils_CreateDisplay(int index,uint id,uint type)
{
  bool bVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  u8 *puVar6;
  uint local_res8 [2];
  u8 auStack_28 [16];
  puVar6 = (u8 *)0x0;
  local_res8[0] = type;
  if (index != 0) {
    puVar6 = auStack_28;
    ((int (*)(int,int,int))(*(void **)(*((u32 *)(ParticleSysEntityPtr)) + 0xe8)))(ParticleSysEntityPtr,puVar6,index);
  }
  iVar2 = ParticleFrameCounter + 1;
  bVar1 = ParticleFrameCounter == 0;
  ParticleFrameCounter = iVar2;
  if (bVar1) {
    VramParticle_Spawn();
    VramParticle_SpawnRing();
    VramParticle_SpawnByType(puVar6);
  }
  iVar5 = 0;
  VramParticle_UpdateAnim();
  VramParticle_DrawAll(puVar6);
  iVar2 = 0;
  StageGrid_DrawTiles();
  ParticleEntityScanIndex = 0;
  piVar4 = &ParticleSecondarySlots;
  do {
    if (*piVar4 == 0) {
      iVar3 = RenderUtils_InitSubEntity(&Unk_SpuBuf154 + iVar2,local_res8);
      *piVar4 = iVar3;
    }
    else {
      iVar3 = RenderUtils_CheckSeed(*piVar4,puVar6,id);
      if (iVar3 == 0) {
        iVar3 = RenderUtils_ClipSub(*piVar4);
        *piVar4 = iVar3;
      }
    }
    iVar2 = iVar2 + 0x68;
    iVar5 = iVar5 + 1;
    piVar4 = piVar4 + 1;
  } while (iVar5 < 2);
  return local_res8[0];
}

void StageGrid_DrawTiles(void)
{
  void *puVar1;
  void *puVar2;
  if (ParticleStageIndex == 2) {
    puVar1 = &MusicVolLineDataStage2;
    puVar2 = &MusicVolLineTargetStage2;
  }
  else {
    if (2 < ParticleStageIndex - 3U) {
      return;
    }
    puVar1 = &MusicVolLineDataStage3;
    puVar2 = &MusicVolLineTargetStage3;
  }
  Prim_DrawTiles(puVar1,1,puVar2);
}

void AnimTick_Type0(uint id,int index)
{
  uint uVar1;
  int iVar2;
  uVar1 = ComputeAnimDiv();
  iVar2 = *(int *)(index + 4);
  *(uint *)(index + 0x10) = uVar1;
  if (iVar2 == 0) {
    *(uint *)(index + 0x1c) = 7;
    *(uint *)(index + 0x20) = 0;
  }
  else if (iVar2 == 2) {
    *(uint *)(index + 0x30) = 7;
    *(uint *)(index + 0x34) = 0;
  }
  else if (iVar2 == 5) {
    *(uint *)(index + 0x44) = 7;
    *(uint *)(index + 0x48) = 0;
  }
  else if (7 < iVar2) {
    *(uint *)(index + 4) = 0xffffffff;
  }
}

void AnimTick_Type1(uint id,int index)
{
  uint uVar1;
  uVar1 = ComputeAnimDiv();
  *(uint *)(index + 0x10) = uVar1;
  if (*(int *)(index + 4) == 0) {
    *(uint *)(index + 0x1c) = 0x18;
    *(uint *)(index + 0x20) = 0xfffffffe;
  }
  else if (0x400 < *(int *)(index + 4)) {
    *(uint *)(index + 4) = 0xffffffff;
  }
}

void AnimTick_Type2(uint id,int index)
{
  uint uVar1;
  uVar1 = ComputeAnimDiv();
  *(uint *)(index + 0x10) = uVar1;
  if (*(int *)(index + 4) == 0) {
    *(uint *)(index + 0x1c) = 0xc;
    *(uint *)(index + 0x20) = 2;
  }
  else if (4 < *(int *)(index + 4)) {
    *(uint *)(index + 4) = 0xffffffff;
  }
}

void AnimTick_Type3(uint id,int index)
{
  uint uVar1;
  uVar1 = ComputeAnimDiv();
  *(uint *)(index + 0x10) = uVar1;
  if (*(int *)(index + 4) == (*(int *)(index + 4) / 0x14) * 0x14) {
    *(uint *)(index + 0x1c) = 0x1e;
    *(uint *)(index + 0x24) = 0x20;
    *(uint *)(index + 0x20) = 0;
    *(uint *)(index + 0x28) = 10;
  }
  if (*(int *)(index + 4) == (*(int *)(index + 4) / 400) * 400) {
    *(uint *)(index + 0x30) = 0x1e;
    *(uint *)(index + 0x34) = 0;
  }
  *(uint *)(index + 0x44) = 6;
  *(uint *)(index + 0x4c) = 0x20;
  *(uint *)(index + 0x48) = 0;
  *(uint *)(index + 0x50) = 10;
}

void AnimTick_Type4(uint id,int index)
{
  uint uVar1;
  int iVar2;
  uVar1 = ComputeAnimDiv();
  *(uint *)(index + 0x10) = uVar1;
  if (*(int *)(index + 4) == (*(int *)(index + 4) / 3) * 3) {
    *(uint *)(index + 0x1c) = 0x1e;
    *(uint *)(index + 0x20) = 0;
  }
  iVar2 = *(int *)(index + 4);
  if (iVar2 == (iVar2 / 5) * 5) {
    *(uint *)(index + 0x30) = 0x1e;
    *(uint *)(index + 0x34) = 0;
    *(uint *)(index + 0x38) = 0x18;
    *(uint *)(index + 0x3c) = 0x18;
    iVar2 = *(int *)(index + 4);
  }
  if (iVar2 == (iVar2 / 7) * 7) {
    *(uint *)(index + 0x1c) = 0x1e;
    *(uint *)(index + 0x20) = 0;
  }
  *(uint *)(index + 0x44) = 6;
  *(uint *)(index + 0x48) = 1;
  *(uint *)(index + 0x4c) = 0x2a;
  *(uint *)(index + 0x50) = 10;
}

void AnimTick_Type5(uint id,int index)
{
  uint uVar1;
  int iVar2;
  uVar1 = ComputeAnimDiv();
  iVar2 = *(int *)(index + 4);
  *(uint *)(index + 0x10) = uVar1;
  if (iVar2 == 0) {
    *(uint *)(index + 0x1c) = 0x1e;
    *(uint *)(index + 0x20) = 0xffffffff;
  }
  else if ((iVar2 < 0x32) && ((iVar2 / 5) * 5 == iVar2 + -4)) {
    *(uint *)(index + 0x30) = 0x1e;
    *(uint *)(index + 0x34) = 0;
    iVar2 = *(int *)(index + 0x38) + *(int *)(index + 4) * -2;
    *(int *)(index + 0x38) = iVar2;
    *(int *)(index + 0x3c) = iVar2;
  }
  else if (*(int *)(index + 4) - 0x65U < 9) {
    *(uint *)(index + 0x44) = 0xd;
    *(uint *)(index + 0x48) = 1;
  }
  else if (200 < *(int *)(index + 4)) {
    *(uint *)(index + 4) = 0xffffffff;
  }
}

void AnimTick_Type6(uint id,int index)
{
  uint uVar1;
  uVar1 = ComputeAnimDiv();
  *(uint *)(index + 0x10) = uVar1;
  if (*(int *)(index + 4) == 0) {
    *(uint *)(index + 0x1c) = 7;
    *(uint *)(index + 0x20) = 2;
  }
  else if (0x1a < *(int *)(index + 4)) {
    *(uint *)(index + 4) = 0xffffffff;
  }
}

void AnimTick_Type7(uint id,int index)
{
  uint uVar1;
  int iVar2;
  uVar1 = ComputeAnimDiv();
  iVar2 = *(int *)(index + 4);
  *(uint *)(index + 0x10) = uVar1;
  if (iVar2 == 0) {
    *(uint *)(index + 0x1c) = 0x14;
    *(uint *)(index + 0x20) = 1;
  }
  else if (iVar2 == 3) {
    *(uint *)(index + 0x20) = 2;
    *(uint *)(index + 0x24) = 0x18;
    *(uint *)(index + 0x1c) = 3;
    *(uint *)(index + 0x28) = 0x14;
  }
  else if (0x32 < iVar2) {
    *(uint *)(index + 4) = 0xffffffff;
  }
}

void AnimTick_Type8(uint id,int index)
{
  uint uVar1;
  uVar1 = ComputeAnimDiv();
  *(uint *)(index + 0x10) = uVar1;
  if (*(int *)(index + 4) == (*(int *)(index + 4) / 0x14) * 0x14) {
    *(uint *)(index + 0x1c) = 9;
    *(uint *)(index + 0x20) = 0;
    *(uint *)(index + 0x24) = 0x40;
    *(uint *)(index + 0x28) = 0x40;
  }
}

void AnimTick_Type9(uint id,int index)
{
  uint uVar1;
  uVar1 = ComputeAnimDiv();
  *(uint *)(index + 0x10) = uVar1;
  if (*(int *)(index + 4) == (*(int *)(index + 4) / 0x14) * 0x14) {
    *(uint *)(index + 0x1c) = 9;
    *(uint *)(index + 0x20) = 0xfffffffe;
  }
}

void RenderUtils_CallDisplay(uint id,int index)
{
  uint uVar1;
  int iVar2;
  uVar1 = ComputeAnimDiv();
  *(uint *)(index + 0x10) = uVar1;
  iVar2 = *(int *)(index + 4) % 0x14;
  if (iVar2 == 1) {
    *(uint *)(index + 0x1c) = 9;
    *(uint *)(index + 0x20) = 0xfffffffe;
  }
  else if (iVar2 == 0x10) {
    *(uint *)(index + 0x30) = 9;
    *(uint *)(index + 0x34) = 0xfffffffe;
  }
}

void AnimTick_Type11(uint id,int index)
{
  uint uVar1;
  int iVar2;
  RenderUtils_CallDisplay(id,index);
  iVar2 = *(int *)(index + 4) % 0x46;
  if (iVar2 == 0x32) {
    *(uint *)(index + 0x44) = 0x14;
    uVar1 = 1;
  }
  else if (iVar2 - 0x36U < 5) {
    *(uint *)(index + 0x44) = 0xd;
    uVar1 = 1;
  }
  else {
    if (iVar2 != 0x3d) {
      return;
    }
    *(uint *)(index + 0x44) = 9;
    uVar1 = 0xffffffff;
  }
  *(uint *)(index + 0x48) = uVar1;
}

void AnimTick_Type12(uint id,int index)
{
  uint uVar1;
  int iVar2;
  uVar1 = ComputeAnimDiv();
  iVar2 = *(int *)(index + 4);
  *(uint *)(index + 0x10) = uVar1;
  if (iVar2 == 0) {
    *(uint *)(index + 0x1c) = 0x14;
    *(uint *)(index + 0x20) = 0xfffffffe;
    *(uint *)(index + 0x30) = 0x14;
    *(uint *)(index + 0x34) = 0xfffffffe;
  }
  else if (iVar2 == 4) {
    *(uint *)(index + 0x30) = 0x14;
    *(uint *)(index + 0x34) = 0xfffffffe;
  }
  else if (iVar2 == 0x14) {
    *(uint *)(index + 0x1c) = 0x10;
    *(uint *)(index + 0x20) = 0xfffffffe;
    *(uint *)(index + 0x30) = 0x12;
    *(uint *)(index + 0x34) = 0xfffffffe;
  }
  else if (200 < iVar2) {
    *(uint *)(index + 4) = 0xffffffff;
  }
}

void AnimTick_Type13(uint id,int index)
{
  uint uVar1;
  uVar1 = ComputeAnimDiv();
  *(uint *)(index + 0x10) = uVar1;
  if (*(int *)(index + 4) == 0) {
    *(uint *)(index + 0x1c) = 0x18;
    *(uint *)(index + 0x20) = 0;
  }
}

int ComputeAnimDiv(int *index)
{
  int iVar1;
  int iVar2;
  iVar1 = index[10];
  iVar2 = *(int *)(&SecondaryEntityRangeTable + *(char *)(*index + 6) * -4) / iVar1;
  if (iVar1 == 0) {
    trap(0x1c00);
  }
  if ((iVar1 == -1) && (*(int *)(&SecondaryEntityRangeTable + *(char *)(*index + 6) * -4) == -0x80000000)) {
    trap(0x1800);
  }
  if (iVar2 == 0) {
    trap(0x1c00);
  }
  if ((iVar2 == -1) && (index[4] == -0x80000000)) {
    trap(0x1800);
  }
  return index[4] / iVar2;
}

uint ParticleIsModeOff(void)
{
  return ParticleMode & 1 ^ 1;
}

int ParticleAllocate(uint id,uint type,uint flags,uint arg3)
{
  int iVar1;
  int iVar2;
  int iVar3;
  iVar1 = MemAllocImpl(0x98);
  iVar2 = 0;
  if (iVar1 != 0) {
    iVar2 = GetRenderStateVtable();
    iVar3 = ((int (*)(int,int,int,int,int))(*(void **)(iVar2 + 8)))(iVar1,id,type,flags,arg3);
    iVar2 = iVar1;
    if (iVar3 == 0) {
      MemFreeImpl(iVar1);
      iVar2 = 0;
    }
  }
  return iVar2;
}
int * ParticleConstruct(int *index,int index_2,uint *id,uint type,uint flags
                  )
{
  int iVar1;
  int *piVar2;
  iVar1 = GetGameManagerAlias();
  iVar1 = ((int (*)(int))(*(void **)(iVar1 + 8)))(index);
  piVar2 = NULL;
  if (iVar1 != 0) {
    iVar1 = GetRenderStateVtable();
    *id = iVar1;
    id[0x11] = 0;
    id[0x15] = index;
    ((int (*)(int,int))(*(void **)(*id + 0x40)))(id,id);
    ParticleLinkToStage(id,type,flags);
    piVar2 = id;
  }
  return piVar2;
}

void ParticleDestruct(uint id)
{
  int iVar1;
  ParticleSubtypeCleanup();
  iVar1 = GetGameManagerAlias();
  ((int (*)(int))(*(void **)(iVar1 + 0xc)))(id);
}

void ParticleCopyParams(int index,uint *id)
{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  uint *puVar4;
  uint *puVar5;
  puVar4 = (uint *)(index + 0x58);
  puVar5 = id + 8;
  do {
    uVar1 = id[1];
    uVar2 = id[2];
    uVar3 = id[3];
    *puVar4 = *id;
    puVar4[1] = uVar1;
    puVar4[2] = uVar2;
    puVar4[3] = uVar3;
    id = id + 4;
    puVar4 = puVar4 + 4;
  } while (id != puVar5);
  *puVar4 = *id;
  *(uint *)(index + 0x24) = 0;
}

void ParticleTickUpdate(int index)
{
  *(int *)(index + 0x24) = *(int *)(index + 0x24) + 1;
  ParticleUpdatePosition();
}

void ParticleLinkToStage(int index,uint id,uint type)
{
  uint uVar1;
  int iVar2;
  u8 auStack_20 [16];
  ParticleTickTimerValue = *(uint *)(ParticleTickTimer + 0x18);
  VectorAdd3(auStack_20,type,index + 0x58);
  ParticleLinkSubObjects(index,id,auStack_20,*(uint *)(index + 100),
               *(uint *)(index + 0x68));
  iVar2 = *(int *)(index + 0x54);
  if (iVar2 < 2) {
    uVar1 = ((int (*)(int,int))(*(void **)(*((u32 *)(ParticleStageObject)) + 0x80)))(ParticleStageObject,(&StageChildObjects)[iVar2]);
    GsLinkObject2(index,uVar1);
    iVar2 = *(int *)(index + 0x54);
  }
  if (iVar2 == 2) {
    SubType2Render(index,0);
  }
  else if (iVar2 < 3) {
    if (iVar2 == 0) {
      SubType0Render(index,0);
    }
  }
  else if (iVar2 == 3) {
    SubType2Init(index,0);
  }
}

void ParticleUpdatePosition(int *index,uint id)
{
  int iVar1;
  u8 auStack_20 [4];
  int local_1c;
  VectorAdd3(auStack_20,id,index + 0x16);
  local_1c = (*(int *)(ParticleTickTimer + 0x18) - ParticleTickTimerValue) + local_1c;
  ((int (*)(int,int))(*(void **)(*index + 0xb8)))(index,auStack_20);
  iVar1 = ((EntityObj *)index)->nSoundId;
  if (iVar1 == 2) {
    SubType2AnimUpdateStub(index,id);
  }
  else if (iVar1 < 3) {
    if (iVar1 == 0) {
      SubType0AnimUpdate(index,id);
    }
  }
  else if (iVar1 == 3) {
    SubType3AnimUpdate(index,id);
  }
}

void ParticleSubtypeCleanup(int index)
{
  int iVar1;
  iVar1 = *(int *)(index + 0x54);
  if (iVar1 == 2) {
    SubType0FreeChildren();
  }
  else if (iVar1 < 3) {
    if (iVar1 == 0) {
      SubType0Cleanup();
    }
  }
  else if (iVar1 == 3) {
    SubType3Cleanup();
  }
}

void VectorAdd3(int *index,int *value,int *arg2)
{
  *index = *value + *arg2;
  index[1] = value[1] + arg2[1];
  index[2] = value[2] + arg2[2];
}
void ParticleLinkSubObjects(int *index, uint id, uint type, uint flags,
    uint id_3)
{
  ((int (*)(void))(*(void **)(*index + 0x4c)))();
  ((int (*)(int,int,int))(*(void **)(*index + 0x44)))(index,1,flags);
  ((int (*)(int,int,int))(*(void **)(*index + 0x48)))(index,1,id_3);
}

void SubType0Render(int index,int value)
{
  uint uVar1;
  uint *puVar2;
  int iVar3;
  int iVar4;
  int local_30;
  int local_2c;
  uint local_28;
  iVar4 = *(int *)(index + 0x6c);
  if (iVar4 != 0) {
    local_30 = 0;
    local_2c = 0;
    local_28 = 0;
    puVar2 = (uint *)(index + 0x7c);
    iVar3 = 0;
    do {
      if (iVar4 < 3) {
        local_30 = (int)**(short **)(index + 0x68) * *(int *)(&SubType0OffsetTable + iVar4 * 4) +
                   local_30;
      }
      else {
        local_2c = local_2c + *(int *)(&SubType0OffsetTable + iVar4 * 4);
      }
      if (value == 0) {
        uVar1 = ParticlePrimitiveAlloc();
        *puVar2 = uVar1;
        GsLinkObject2(uVar1,*(uint *)(index + 0x20));
        ParticleLinkSubObjects(*puVar2,index,&local_30,*(uint *)(index + 100),
                     *(uint *)(index + 0x68));
      }
      else {
        ((int (*)(int,int))(*(void **)(*(int *)*puVar2 + 0xb8)))((int *)*puVar2,&local_30);
      }
      iVar3 = iVar3 + 1;
      puVar2 = puVar2 + 1;
    } while (iVar3 < 2);
  }
}
/* WARNING: Removing unreachable block (ram,0x80056af0) */
void SubType0AnimUpdate(int *index)
{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  uint local_30;
  uint local_2c;
  int local_28;
  iVar5 = ((EntityObj *)index)->nChildCount;
  if (((((EntityObj *)index)->nSubTypeCount != 0) && (*(int *)(&SubType0AnimFrameTable + iVar5 * 4) != 0)) &&
     (piVar2 = index + 0x1f, 500 < (uint)index[9])) {
    iVar4 = 0;
    iVar3 = 0;
    ((int (*)(int,int,int))(*(void **)(*index + 0x44)))(index,0,&SubType0ColourSetting);
    do {
      local_30 = 0;
      local_2c = 0;
      local_28 = iVar3 + *(int *)(&SubType0AnimFrameTable + iVar5 * 4);
      iVar4 = iVar4 + 1;
      iVar3 = iVar3 + 3;
      ((int (*)(int,int))(*(void **)(*(int *)*piVar2 + 0xbc)))((int *)*piVar2,&local_30);
      piVar1 = (int *)*piVar2;
      piVar2 = piVar2 + 1;
      ((int (*)(int,int,int))(*(void **)(*piVar1 + 0x44)))(piVar1,0,&SubType0ColourSetting);
    } while (iVar4 < 2);
    uVar6 = 0x5fb4 / *(int *)(&SubType0AnimFrameTable + iVar5 * 4);
    if (*(int *)(&SubType0AnimFrameTable + iVar5 * 4) == 0) {
      trap(0x1c00);
    }
    if ((int)uVar6 < 0) {
      uVar6 = ~uVar6 + 1;
      if (uVar6 == 0) {
        trap(0x1c00);
      }
    }
    else if (uVar6 == 0) {
      trap(0x1c00);
    }
    if ((uint)index[9] % uVar6 == 0) {
      SubType0Render(index,1);
    }
  }
  *(uint *)index[5] = 0;
}

void SubType0Cleanup(int index)
{
  if (*(int *)(index + 0x6c) != 0) {
    ArrayInitByVtable(index + 0x7c,2);
  }
}

void SubType2Render(int index)
{
  int iVar1;
  void *puVar2;
  int *piVar3;
  iVar1 = rand();
  puVar2 = NULL;
  if (iVar1 % 2 == 0) {
    puVar2 = &SubType2Colour0;
  }
  SubType2SpawnParticles(index,0,0,puVar2);
  if (*(int *)(index + 0x70) < 2) {
    piVar3 = *(int **)(index + 0x88);
    ((int (*)(int,int))(*(void **)(*piVar3 + 100)))(piVar3,1);
    ((int (*)(int,int))(*(void **)(*piVar3 + 0x68)))(piVar3,0);
    puVar2 = &SubType2Colour2;
    if (iVar1 % 2 != 0) {
      puVar2 = &SubType2Colour1;
    }
    ((int (*)(int,int,int))(*(void **)(*piVar3 + 0x48)))(piVar3,1,puVar2);
  }
  else {
    piVar3 = *(int **)(index + 0x88);
    SubType2TempState = *(uint *)(&SubType2StateArray + *(int *)(index + 0x70) * 4);
    ParticleStateSetAbsolute(piVar3,&SubType2TempState);
    iVar1 = *(int *)(index + 0x78);
    if (iVar1 == 0) {
      iVar1 = *(int *)(index + 0x74);
    }
    ((int (*)(int,int))(*(void **)(*piVar3 + 0xb8)))(piVar3,iVar1);
  }
  ((int (*)(int,int))(*(void **)(**(int **)(index + 0x8c) + 0x60)))(*(int **)(index + 0x8c),0);
}

void SubType2SpawnParticles(int index,uint id,uint type,int value)
{
  int *piVar1;
  uint *puVar2;
  int iVar3;
  puVar2 = (uint *)(index + 0x84);
  iVar3 = 0;
  do {
    piVar1 = (int *)NewNng(type,0,ParticleSpawnAllocator);
    *puVar2 = piVar1;
    ((int (*)(int,int,int))(*(void **)(*piVar1 + 0x4c)))(piVar1,index,0);
    ((int (*)(int,int))(*(void **)(*(int *)*puVar2 + 0xb8)))((int *)*puVar2,*(uint *)(index + 0x74));
    iVar3 = iVar3 + 1;
    if (value != 0) {
      ((int (*)(int,int,int))(*(void **)(*(int *)*puVar2 + 0x48)))((int *)*puVar2,1,value);
    }
    puVar2 = puVar2 + 1;
  } while (iVar3 < 5);
}

void SubType2AnimUpdateStub(void)
{
}

void SubType0FreeChildren(int index)
{
  ArrayInitByVtable(index + 0x84,5);
}

void SubType2Init(uint id)
{
  SubType2SpawnParticles(id,0,0,0);
}

void SubType3AnimUpdate(int index)
{
  uint uVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  piVar3 = (int *)(index + 0x88);
  iVar4 = 0;
  do {
    iVar4 = iVar4 + 1;
    uVar1 = rand();
    ((int (*)(int,int,int))(*(void **)(*(int *)*piVar3 + 0x48)))((int *)*piVar3,1,&Unk_EntityBuf8C + (uVar1 % 6) * 0xc);
    iVar2 = rand();
    *(int *)(*piVar3 + 0x84) = (iVar2 % 0x168) * FIXED_12;
    piVar3 = piVar3 + 1;
  } while (iVar4 < 4);
}

void SubType3Cleanup(int index)
{
  ArrayInitByVtable(index + 0x84,5);
}
void * GetRenderStateVtable(void)
{
  return &Vtable_DreamParticle;
}

void ParticleSysInit(uint id,int *index,uint type,uint flags)
{
  uint uVar1;
  uint *puVar2;
  int iVar3;
  iVar3 = 0;
  puVar2 = &StageChildObjects;
  ParticleStageObject = index;
  ParticleSpawnAllocator = type;
  ParticleTickTimer = flags;
  do {
    uVar1 = *puVar2;
    puVar2 = puVar2 + 1;
    iVar3 = iVar3 + 1;
    uVar1 = ((int (*)(int,int))(*(void **)(*index + 0x80)))(index,uVar1);
    Prim_SetDrawOffset(uVar1,&StageChildTransform);
  } while (iVar3 < 2);
}

int ParticlePrimitiveAlloc(void)
{
  int iVar1;
  int iVar2;
  int iVar3;
  iVar1 = MemAllocImpl(0x58);
  iVar2 = 0;
  if (iVar1 != 0) {
    iVar2 = GetGameManagerAlias();
    iVar3 = ((int (*)(int))(*(void **)(iVar2 + 8)))(iVar1);
    iVar2 = iVar1;
    if (iVar3 == 0) {
      MemFreeImpl(iVar1);
      iVar2 = 0;
    }
  }
  return iVar2;
}
int * PrimitiveConstruct(int *index)
{
  int iVar1;
  int *piVar2;
  iVar1 = GetPrimitiveBaseTable();
  iVar1 = ((int (*)(int))(*(void **)(iVar1 + 8)))(index);
  piVar2 = NULL;
  if (iVar1 != 0) {
    iVar1 = GetGameManagerAlias();
    *index = iVar1;
    ((EntityObj *)index)->nState = 0;
    ((EntityObj *)index)->pObject = 0;
    ((EntityObj *)index)->nField_50 = 0;
    ((int (*)(int))(*(void **)(*index + 0x40)))(index);
    piVar2 = index;
  }
  return piVar2;
}

void PrimitiveBind(int index,uint *id)
{
  int iVar1;
  iVar1 = GetPrimitiveBaseTable();
  ((int (*)(int,int))(*(void **)(iVar1 + 0x10)))(index,id);
  if ((*(uint *)*id & 0xfff) == 0x114) {
    *(uint **)(index + 0x4c) = id;
  }
  else if ((*(uint *)*id & 0xf) == 5) {
    *(uint **)(index + 0x50) = id;
  }
}

void PrimitiveUnbind(int index,uint *id)
{
  int iVar1;
  if ((*(uint *)*id & 0xfff) == 0x114) {
    *(uint *)(index + 0x4c) = 0;
  }
  else if ((*(uint *)*id & 0xf) == 5) {
    *(uint *)(index + 0x50) = 0;
  }
  iVar1 = GetPrimitiveBaseTable();
  ((int (*)(int,int))(*(void **)(iVar1 + 0x14)))(index,id);
}

void PrimitiveDestruct(int index)
{
  int iVar1;
  *(uint *)(index + 0x4c) = 0;
  *(uint *)(index + 0x50) = 0;
  iVar1 = GetPrimitiveBaseTable();
  ((int (*)(int))(*(void **)(iVar1 + 0x18)))(index);
}

void PrimitiveInitDefaults(int index)
{
  *(u16 *)(index + 0x48) = 300;
  *(uint *)(index + 0x54) = 0;
}

void PrimitiveDraw(int *index, int index_2)
{
  int iVar1;
  char *pcVar2;
  uint uVar3;
  u8 auStack_48 [56];
  iVar1 = GetPrimitiveBaseTable();
  ((int (*)(int,int))(*(void **)(iVar1 + 0x88)))(index,index);
  if ((((index_2 < 9) && (4 < index)) && (index[8] != 0)) &&
     (iVar1 = GpuGetReady(), iVar1 != 0)) {
    ((int (*)(int,int))(*(void **)(*index + 0x8c)))(index,auStack_48);
    if (index_2 != 5) {
      uVar3 = (uint)(short)((EntityObj *)index)->nStateFlag;
      if ((int)uVar3 < 0) {
        iVar1 = -((EntityObj *)index)->nSoundId;
      }
      else {
        iVar1 = ((EntityObj *)index)->nSoundId;
      }
      GpuOffsetAABB(auStack_48,index_2 == 7,~uVar3 >> 0x1f,uVar3 + iVar1);
    }
    ((int (*)(int,int,int))(*(void **)(*index + 0x90)))(index,auStack_48,index);
    if (((uint *)index[10] != (uint *)0x0) &&
       (pcVar2 = *(char **)index[10], *pcVar2 == '4')) {
      ((int (*)(void))(*(void **)(pcVar2 + 0xe8)))();
    }
  }
}

void PrimitiveExecCommand(int *index,uint *id)
{
  code *pcVar1;
  if (*(char *)*id == '4') {
    pcVar1 = *(code **)(*index + 0xdc);
  }
  else {
    if (*(char *)*id != '$') {
      return;
    }
    pcVar1 = *(code **)(*id + 0xe0);
  }
  ((int (*)(...))(pcVar1))();
}

void ParticleStateSetRelative(uint id,uint type)
{
  ParticleStateSetCore(id,1,type);
}

void ParticleStateSetAbsolute(uint id,uint type)
{
  ParticleStateSetCore(id,0,type);
}

void ParticleStateSetCore(int *index, int value, int index_2)
{
  int iVar1;
  int iVar2;
  int iVar3;
  iVar3 = *(int *)(index + 0x14);
  if (value == 0) {
    *(int *)(iVar3 + 0x18) = *(int *)(iVar3 + 0x18) + *index;
    *(int *)(iVar3 + 0x1c) = *(int *)(iVar3 + 0x1c) + index[1];
    *(int *)(iVar3 + 0x20) = *(int *)(iVar3 + 0x20) + index[2];
  }
  else {
    iVar1 = index[1];
    iVar2 = index[2];
    *(int *)(iVar3 + 0x18) = *index;
    *(int *)(iVar3 + 0x1c) = iVar1;
    *(int *)(iVar3 + 0x20) = iVar2;
  }
  **(uint **)(index + 0x14) = 0;
}

void ParticleApplyTransform(int *index, uint *id)
{
  u8 auStack_18 [16];
  MathApplyWorldTransform(id,auStack_18,id);
  ((int (*)(int,int))(*(void **)(*id + 0xbc)))(id,auStack_18);
}

void ParticleSetColour(uint id,uint type,uint flags)
{
  VramPutImageCore(id,&Rendering_PartSysA8,type,flags,6);
}

void VramPutImageClut(uint id,uint type,uint flags)
{
  VramPutImageCore(id,&EntityVisualColourSet,type,flags,7);
}

void VramPutImagePartSys6(uint id,uint type,uint flags)
{
  VramPutImageCore(id,&Rendering_PartSysA6,type,flags,8);
}
void VramPutImageCore(int *index,u16 *arg1,u16 arg0,int flags,uint id
                 )
{
  *arg1 = flags;
  *(u16 *)(index + 0x12) = flags;
  ((int (*)(int,int))(*(void **)(*index + 0xc0)))(index,&EntityVisualColourSet);
  *arg1 = 0;
  if (index != 0) {
    ((int (*)(int,int))(*(void **)(*index + 0x88)))(index,id);
  }
}

void RenderUtils_LoadImage(int *index,uint id,uint type)
{
  RenderUtils_LoadStoreImage(index,*(uint *)(*index + 0xc4),id,type);
}

void RenderUtils_StoreImage(int *index,uint id,uint type)
{
  RenderUtils_LoadStoreImage(index,*(uint *)(*index + 200),id,type);
}

void VtableStub_Nop(void)
{
}

void RenderUtils_LoadStoreImage(int index,code *arg0,uint id,uint type)
{
  *(uint *)(index + 0x28) = 0;
  ((int (*)(...))(arg0))(index,id,type);
  if (*(int *)(index + 0x28) == 0) {
    RenderUtils_GetDisplay(index);
  }
}

uint RenderUtils_GetDisplay(int *index)
{
  uint uVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  u8 auStack_98 [48];
  u8 auStack_68 [48];
  u8 auStack_38 [16];
  u8 auStack_28 [16];
  piVar4 = (int *)((EntityObj *)index)->pObject;
  uVar1 = 0;
  if (piVar4 != NULL) {
    iVar2 = ((EntityObj *)index)->pData;
    iVar3 = ((int (*)(int,int,int))(*(void **)(*piVar4 + 0x110)))(piVar4,auStack_98,iVar2 + 0x18);
    uVar1 = 0;
    if (iVar3 == 0) {
      uVar1 = RenderUtils_CalcDisplayParams(index,auStack_68,auStack_38,auStack_98,1);
      iVar2 = RenderUtils_ProcessPrimitives(index,auStack_28,iVar2 + 0x18,uVar1,auStack_68,auStack_38);
      ((EntityObj *)index)->nConfig = iVar2;
      if (iVar2 == 0) {
        ((int (*)(int,int))(*(void **)(*index + 0x88)))(index,0xfffffffe);
        uVar1 = 0;
      }
      else {
        ((int (*)(int,int))(*(void **)(*index + 0xbc)))(index,auStack_28);
        ((int (*)(int,int))(*(void **)(*index + 0x88)))(index,0xffffffff);
        uVar1 = 1;
      }
    }
  }
  return uVar1;
}

int RenderUtils_CalcDisplayParams(int *index, short *val, int index_2, int value, uint id)
{
  char cVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  short *psVar5;
  uint uVar6;
  short sVar7;
  uint uVar8;
  short sVar9;
  int iVar10;
  uint uVar11;
  int *piVar12;
  cVar1 = *(char *)(value + 2);
  cVar2 = *(char *)(value + 3);
  if ((id & 1) == 0) {
    id = id + 1;
  }
  if (id == 1) {
    *val = (short)cVar1;
    val[1] = (short)cVar2;
    val[2] = 1;
    val[3] = 0;
    val[4] = 1;
    val[5] = 0;
    iVar10 = *(int *)(value + 0x24);
    *index = iVar10;
    piVar12 = *(int **)(index + 0x4c);
    iVar3 = 1;
    if (*(int *)(piVar12[0x1a] + 4) == 1) {
      iVar10 = (int)*(short *)(*(int *)(iVar10 + 4) + 0x32);
      iVar3 = 1;
      if (iVar10 + 1 < (int)*(short *)(piVar12[0x1a] + 2)) {
        iVar3 = 2;
        iVar4 = ((int (*)(int))(*(void **)(*piVar12 + 0x118)))(piVar12);
        index[1] = iVar4;
        *(uint *)(val + 6) = *(uint *)val;
        *(uint *)(val + 8) = *(uint *)(val + 2);
        *(uint *)(val + 10) = *(uint *)(val + 4);
      }
      if (-1 < iVar10 + -1) {
        iVar10 = ((int (*)(int))(*(void **)(*piVar12 + 0x118)))(piVar12);
        index[iVar3] = iVar10;
        psVar5 = val + iVar3 * 6;
        uVar6 = *(uint *)(val + 2);
        uVar8 = *(uint *)(val + 4);
        *(uint *)psVar5 = *(uint *)val;
        *(uint *)(psVar5 + 2) = uVar6;
        *(uint *)(psVar5 + 4) = uVar8;
        iVar3 = iVar3 + 1;
      }
    }
  }
  else {
    if (cVar1 == '\0') {
      sVar7 = 0;
      uVar11 = id - 1;
    }
    else {
      sVar7 = cVar1 + -1;
      uVar11 = id;
    }
    if (cVar1 == '\x13') {
      uVar11 = uVar11 - 1;
    }
    if (cVar2 == '\x13') {
      id = id - 1;
      sVar9 = 0x13;
    }
    else {
      sVar9 = cVar2 + 1;
    }
    *val = sVar7;
    if (cVar2 == '\0') {
      id = id - 1;
    }
    val[1] = sVar9;
    *(uint *)(val + 2) = uVar11;
    *(uint *)(val + 4) = id;
    iVar3 = 1;
    *(int *)(value + 0x24) = *(int *)(value + 0x24);
  }
  return iVar3;
}
int RenderUtils_ProcessPrimitives(uint id, uint type, uint flags, int *index, int value,
    int index_3)
{
  int iVar1;
  int iVar2;
  iVar2 = 0;
  if (0 < index) {
    do {
      iVar2 = iVar2 + 1;
      if ((*(short *)(*(int *)(*index + 4) + 0x2c) != 0) &&
         (iVar1 = RenderUtils_FindVisibleNode(id,type,flags,value,*index), iVar1 != 0)) {
        return iVar1;
      }
      value = value + 0xc;
      index = index + 1;
    } while (iVar2 < index);
  }
  return 0;
}
int RenderUtils_FindVisibleNode(uint id,uint type,uint flags,short *val,int index
                )
{
  int iVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  int iVar5;
  iVar5 = 0;
  piVar3 = (int *)(val[1] * 0x50 + *(int *)(index + 0x10) + *val * 4);
  if (0 < *(int *)(val + 4)) {
    do {
      iVar4 = 0;
      if (0 < *(int *)(val + 2)) {
        do {
          iVar1 = Node_CheckVisible(*piVar3,type,flags);
          if (iVar1 != 0) {
            return *piVar3;
          }
          for (iVar1 = *(int *)(*piVar3 + 0x38); iVar1 != 0; iVar1 = *(int *)(iVar1 + 0x38)) {
            iVar2 = Node_CheckVisible(iVar1,type,flags);
            if (iVar2 != 0) {
              return iVar1;
            }
          }
          iVar4 = iVar4 + 1;
          piVar3 = piVar3 + 1;
        } while (iVar4 < *(int *)(val + 2));
      }
      iVar5 = iVar5 + 1;
      piVar3 = (int *)((int)piVar3 - (*(int *)(val + 2) * 4 + 0x50));
    } while (iVar5 < *(int *)(val + 4));
  }
  return 0;
}

int Node_CheckVisible(int index)
{
  int iVar1;
  if ((index == 0) || (iVar1 = SceneNodeAimAt(), iVar1 == 0)) {
    index = 0;
  }
  return index;
}

void RenderUtils_DrawPrimitiveList(int *index, uint id, int index_2)
{
  int iVar1;
  iVar1 = GetPrimitiveBaseTable();
  ((int (*)(int,int,int))(*(void **)(iVar1 + 0x9c)))(index,id,index);
  if ((index_2 < 9) && (4 < index)) {
    ((int (*)(int,int,int))(*(void **)(*index + 0xa0)))(index,id,index);
  }
}

void RenderUtils_DrawPrimitive(uint index,uint id,uint type,uint flags)
{
  int iVar1;
  iVar1 = GetPrimitiveBaseTable();
  ((int (*)(int,int,int))(*(void **)(iVar1 + 0x9c)))(id,type,flags);
}

void RenderUtils_SetParam48(int index,u16 arg0)
{
  *(u16 *)(index + 0x48) = arg0;
}

void VtableStub_Nop2(void)
{
}

void RenderUtils_SetParam54(int index,uint id)
{
  *(uint *)(index + 0x54) = id;
}
void * GetGameManagerAlias(void)
{
  return &GameManagerPtr;
}

int NewNng(uint index,uint id,uint type,uint flags)
{
  int iVar1;
  int iVar2;
  iVar1 = MemAllocImpl(0xa8);
  iVar2 = 0;
  if (iVar1 != 0) {
    iVar2 = GetNngVtable();
    ((int (*)(int,int,int,int))(*(void **)(iVar2 + 8)))(iVar1,id,type,flags);
    iVar2 = iVar1;
  }
  return iVar2;
}

void FontNng_Init(int *index, int index_2, uint id, uint type)
{
  int iVar1;
  iVar1 = Font_GetVtableData();
  ((int (*)(int,int,int,int,int,int))(*(void **)(iVar1 + 8)))(index,type,0,&Rendering_SecEnt8c + (int)index * 0xc,id,0);
  iVar1 = GetNngVtable();
  *index = iVar1;
  ((EntityObj *)index)->field_0xa4 = 0;
  ((int (*)(int,int))(*(void **)(*index + 0x40)))(index,index);
}

void RenderUtils_SetAnimTable(int index, int index_2, int value)
{
  *(int *)(index + 0xa0) = value;
  *(u16 *)(index + 0x74) = *(u16 *)(&Rendering_SecEntA4 + value * 4);
  *(u16 *)(index + 0x76) = *(u16 *)(&Rendering_SecEntA6 + value * 4);
}

void CalcFixedPointDivide(int index,uint id,short *val)
{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  iVar6 = (int)*val;
  iVar1 = (int)val[1];
  if (iVar1 == 0) {
    trap(0x1c00);
  }
  if ((iVar1 == -1) && (iVar6 == -0x80000000)) {
    trap(0x1800);
  }
  iVar4 = iVar6 % iVar1 << 0xc;
  if (iVar1 == 0) {
    trap(0x1c00);
  }
  if ((iVar1 == -1) && (iVar4 == -0x80000000)) {
    trap(0x1800);
  }
  iVar5 = (int)val[2];
  iVar2 = (int)val[3];
  if (iVar2 == 0) {
    trap(0x1c00);
  }
  if ((iVar2 == -1) && (iVar5 == -0x80000000)) {
    trap(0x1800);
  }
  iVar3 = iVar5 % iVar2 << 0xc;
  if (iVar2 == 0) {
    trap(0x1c00);
  }
  if ((iVar2 == -1) && (iVar3 == -0x80000000)) {
    trap(0x1800);
  }
  iVar6 = (iVar6 / iVar1) * FIXED_12 + iVar4 / iVar1;
  iVar1 = (iVar5 / iVar2) * FIXED_12 + iVar3 / iVar2;
  if (*(int *)(index + 0x58) == 0) {
    *(short *)(index + 0x80) = (short)iVar6;
    *(short *)(index + 0x82) = (short)iVar1;
  }
  else {
    *(int *)(index + 0x5c) = (iVar6 * FIXED_16 >> 0x10) * *(int *)(index + 0x5c) >> 0xc;
    *(int *)(index + 0x60) = (iVar1 * FIXED_16 >> 0x10) * *(int *)(index + 0x60) >> 0xc;
  }
}

void VtableStub_Nop3(void)
{
}

void VtableStub_Nop4(void)
{
}

void VtableStub_Nop5(void)
{
}

void VtableStub_Nop6(void)
{
}
void * GetNngVtable(void)
{
  return &Vtable_EntitySubObject2;
}

int NewStageArea(uint index, uint id)
{
  int iVar1;
  int iVar2;
  iVar1 = MemAllocImpl(0x244);
  iVar2 = 0;
  if (iVar1 != 0) {
    iVar2 = GetStageAreaVtable();
    ((int (*)(int,int))(*(void **)(iVar2 + 8)))(iVar1,id);
    iVar2 = iVar1;
  }
  return iVar2;
}

void StageArea_Init(int *index, int index_2)
{
  int iVar1;
  iVar1 = NavMenu_GetDataPtr();
  ((int (*)(int,int,int,int))(*(void **)(iVar1 + 8)))(index,0,"ETC\\ETCSE",0);
  iVar1 = GetStageAreaVtable();
  *index = iVar1;
  ((int (*)(int,int))(*(void **)(*(int *)((EntityObj *)index)->nStateFlag + 0x9c)))((int *)((EntityObj *)index)->nStateFlag,0xffffffff);
  ((EntityObj *)index)->field_0xa4 = index;
  ((int (*)(int,int))(*(void **)(*index + 0xd8)))(index,0);
  ((int (*)(int,int))(*(void **)(*index + 0x40)))(index,index);
}

void StageArea_InitHGraph(int *index)
{
  ((EntityObj *)index)->nSubState = 5;
  ((EntityObj *)index)->nVabIdx = 400;
  ((int (*)(int,int,int))(*(void **)(*index + 0xd4)))(index,"ETC\\HGRAPH.TIM",0);
  ((int (*)(int,int))(*(void **)(*index + 0x6c)))(index,10);
}

void StageArea_Draw(int *index,uint id,uint type)
{
  int iVar1;
  iVar1 = NavMenu_GetDataPtr();
  ((int (*)(int,int,int))(*(void **)(iVar1 + 0x5c)))(index,id,type);
  if ((((EntityObj *)index)->pOtherEntity == 1) &&
     ((iVar1 = ((int (*)(int,int))(*(void **)(*(int *)((EntityObj *)index)->field_0xa4 + 0x1b0)))((int *)((EntityObj *)index)->field_0xa4,0),
      *(int *)(iVar1 + 4) != 0 || (*(int *)(iVar1 + 8) != 0)))) {
    ((int (*)(int,int))(*(void **)(*(int *)((EntityObj *)index)->field_0xa8 + 0x60)))((int *)((EntityObj *)index)->field_0xa8,index[7] & 1);
  }
  ((int (*)(int))(*(void **)(*index + 0x124)))(index);
}

void StageArea_Update(int *index)
{
  if (index[0x8e] == 0) {
    ((int (*)(int,int))(*(void **)(*index + 0x70)))(index,0x10);
    ((int (*)(int))(*(void **)(*index + 0x94)))(index);
  }
}

void ParticleSys_Create(int index)
{
  int iVar1;
  uint uVar2;
  char cVar3;
  int iVar4;
  char local_18;
  char local_17;
  char local_16;
  uVar2 = Particle_New(&Rendering_PartSysAc,&Rendering_PartSysB4,0);
  *(uint *)(index + 0xa8) = uVar2;
  local_18 = -1;
  local_17 = -1;
  local_16 = -1;
  iVar4 = 1;
  iVar1 = index;
  do {
    uVar2 = Particle_New(&Rendering_PartSysAc,&local_18,0);
    *(uint *)(iVar1 + 0xac) = uVar2;
    cVar3 = '\x01';
    if (iVar4 < 7) {
      cVar3 = '\x14';
    }
    iVar4 = iVar4 + 1;
    local_18 = local_18 - cVar3;
    local_16 = local_16 - cVar3;
    local_17 = local_17 - cVar3;
    iVar1 = iVar1 + 4;
  } while (iVar4 < 100);
  uVar2 = MemAllocImpl(4);
  *(uint *)(index + 0x240) = uVar2;
}

void ParticleSys_Destroy(int index)
{
  uint *puVar1;
  int iVar2;
  int iVar3;
  iVar3 = 0;
  MemFreeImpl(*(uint *)(index + 0x240));
  iVar2 = index;
  do {
    puVar1 = (uint *)(iVar2 + 0xa8);
    iVar2 = iVar2 + 4;
    iVar3 = iVar3 + 1;
    ((int (*)(void))(*(void **)(*(int *)*puVar1 + 4)))();
  } while (iVar3 < 100);
  iVar2 = NavMenu_GetDataPtr();
  ((int (*)(int))(*(void **)(iVar2 + 0xdc)))(index);
}

uint StageArea_GetState(int index,uint id,uint type)
{
  int iVar1;
  uint uVar2;
  iVar1 = NavMenu_GetDataPtr();
  ((int (*)(int,int,int))(*(void **)(iVar1 + 0x44)))(index,id,type);
  uVar2 = 2;
  if (*(int *)(index + 0x238) == 0) {
    uVar2 = *(uint *)(index + 0x38);
  }
  return uVar2;
}

void StageArea_SetupParticles(int index,uint id)
{
  bool bVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int local_40;
  int local_3c;
  int local_38;
  int local_34;
  iVar2 = NavMenu_GetDataPtr();
  ((int (*)(int,int))(*(void **)(iVar2 + 0xe0)))(index,id);
  iVar2 = ((int (*)(int,int))(*(void **)(**(int **)(index + 0xa4) + 0x1b0)))(*(int **)(index + 0xa4),0);
  uVar3 = StageArea_NumToPos(index,iVar2);
  *(uint *)(index + 0x238) = uVar3;
  bVar1 = false;
  if ((*(int *)(iVar2 + 4) != 0) || (iVar8 = *(int *)(iVar2 + 8), 100 < iVar8)) {
    iVar8 = 100;
  }
  iVar4 = *(int *)(iVar2 + 8);
  iVar6 = 0;
  iVar7 = index;
  if (0 < iVar8) {
    do {
      iVar4 = iVar4 + -1;
      iVar5 = iVar4 * 2;
      if (iVar4 < 0) {
        iVar4 = 0x16c;
        iVar5 = 0x2d8;
      }
      local_40 = *(char *)(iVar2 + iVar5 + 0x18) * 10 + -5;
      local_3c = *(char *)(iVar2 + iVar5 + 0x19) * -10 + -5;
      if (iVar6 == 0) {
        bVar1 = true;
        local_38 = local_40;
        local_34 = local_3c;
      }
      else {
        ((int (*)(int,int,int,int))(*(void **)(**(int **)(iVar7 + 0xa8) + 0xc4)))(*(int **)(iVar7 + 0xa8),id,&local_40,0);
      }
      iVar6 = iVar6 + 1;
      iVar7 = iVar7 + 4;
    } while (iVar6 < iVar8);
  }
  if (bVar1) {
    ((int (*)(int,int,int,int))(*(void **)(**(int **)(index + 0xa8) + 0xc4)))(*(int **)(index + 0xa8),id,&local_38,0);
  }
}

uint StageArea_NumToPos(int index, int value)
{
  int iVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  if (*(char *)(value + 0x467) == '\0') {
    if ((*(int *)(value + 4) != 0) || (iVar7 = *(int *)(value + 8), 100 < iVar7)) {
      iVar7 = 100;
    }
    uVar6 = 0;
    do {
      iVar4 = 0;
      iVar1 = *(int *)(value + 8);
      iVar5 = 0;
      if (0 < iVar7) {
        do {
          iVar1 = iVar1 + -1;
          iVar2 = iVar1 * 2;
          if (iVar1 < 0) {
            iVar1 = 0x16c;
            iVar2 = 0x2d8;
          }
          if ((&DreamSymbolSequence)[uVar6] == *(short *)(value + iVar2 + 0x18)) {
            iVar5 = iVar5 + 1;
            *(char *)(*(int *)(index + 0x240) + uVar6) = (char)iVar4;
          }
          iVar4 = iVar4 + 1;
        } while (iVar4 < iVar7);
      }
      uVar6 = uVar6 + 1;
      if (iVar5 == 0) goto LAB_80058684;
    } while (uVar6 < 4);
    uVar3 = 1;
    *(u8 *)(value + 0x467) = 1;
    *(uint *)(index + 0x23c) = 0;
  }
  else {
LAB_80058684:
    uVar3 = 0;
  }
  return uVar3;
}

void StageArea_UpdateParticles(int index)
{
  uint uVar1;
  int *piVar2;
  if ((((*(int *)(index + 0x238) != 0) && (uVar1 = *(uint *)(index + 0x1c), 0x1e < uVar1)) &&
      (*(uint *)(index + 0x23c) < 4)) && (uVar1 == (uVar1 / 0x18) * 0x18)) {
    piVar2 = *(int **)(*(char *)(*(int *)(index + 0x240) + *(uint *)(index + 0x23c)) * 4 +
                       index + 0xa8);
    ((int (*)(int,int,int))(*(void **)(*piVar2 + 0xb8)))(piVar2,1,&Rendering_PartSysBc);
    *(int *)(index + 0x23c) = *(int *)(index + 0x23c) + 1;
  }
}
void * GetStageAreaVtable(void)
{
  return &Vtable_DreamSymbol;
}
