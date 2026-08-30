/* Rendering_World.c -- split from Rendering.c */
#include "Rendering_Shared.h"
int LocationGrid_Alloc(uint id,uint type)
{
  int iVar1;
  int iVar2;
  iVar1 = MemAllocImpl(SIZEOF_LOCATION_GRID);
  iVar2 = 0;
  if (iVar1 != 0) {
    iVar2 = Location_GetChunkVtable();
    ((int (*)(int,int,int))(*(void **)(iVar2 + 8)))(iVar1,id,type);
    iVar2 = iVar1;
  }
  return iVar2;
}

void LocationGrid_Init(int *index,int *value,uint id)
{
  int iVar1;
  int iVar2;
  uint uVar3;
  int *piVar4;
  int *piVar5;
  int iVar6;
  u16 *puVar7;
  int *piVar8;
  int local_38;
  uint local_34;
  int local_30;
  iVar1 = LocationGrid_GetDataPtr();
  ((int (*)(int))(*(void **)(iVar1 + 8)))(index);
  iVar1 = Location_GetChunkVtable();
  *index = iVar1;
  if (value == NULL) {
    ((LocationGrid *)index)->nSoundId = 0;
    ((LocationGrid *)index)->pChild = 0;
    ((LocationGrid *)index)->nField_5c = 0;
  }
  else {
    iVar1 = value[1];
    iVar6 = value[2];
    ((LocationGrid *)index)->nSoundId = *value;
((EntityObj *)index)->pChild = (void *)iVar1;
    ((LocationGrid *)index)->nField_5c = iVar6;
  }
  iVar1 = 0;
  iVar6 = SIZEOF_CHUNK_DATA;
  ((LocationGrid *)index)->sg_0x1B0 = 0;
  *(u16 *)(index + 0x6d) = 0;
  ((LocationGrid *)index)->sg_0x1B8 = 0;
  ((LocationGrid *)index)->nChildCount = 0;
  ((LocationGrid *)index)->nSubTypeCount = 0;
  ((LocationGrid *)index)->field_0xe8 = 0;
  ((LocationGrid *)index)->sg_0x1E0 = 0;
  do {
    puVar7 = (u16 *)((int)index + iVar6);
    iVar2 = Snd_New();
    *(int *)(puVar7 + 2) = iVar2;
    *(ushort *)(iVar2 + 0x20) = (ushort)(*(int *)(iVar2 + 0x10) != 0);
    *(short *)(*(int *)(puVar7 + 2) + 0x32) = (short)iVar1;
    ((int (*)(int,int))(*(void **)(**(int **)(puVar7 + 2) + 0x88)))(*(int **)(puVar7 + 2),id);
    *(uint *)(puVar7 + 10) = 0;
    *(uint *)(puVar7 + 0xc) = 0;
    puVar7[1] = (short)iVar1;
    *puVar7 = 0;
    uVar3 = Cd_New(0);
    *(uint *)(puVar7 + 4) = uVar3;
    piVar4 = (int *)ChunkDataTable_Alloc();
    *(int **)(puVar7 + 6) = piVar4;
    ((int (*)(int,int,int))(*(void **)(*piVar4 + 0x4c)))(piVar4,index,index + 0x15);
    iVar2 = MemAllocImpl(0x668);
    *(int *)(puVar7 + 8) = iVar2;
    if (iVar2 == 0) {
      return;
    }
    local_38 = 0x400;
    local_34 = 0;
    local_30 = 0x400;
    piVar4 = *(int **)(puVar7 + 8);
    piVar8 = piVar4 + 0x19a;
    for (; piVar4 < piVar8; piVar4 = piVar4 + 1) {
      piVar5 = (int *)ChunkDataTable_Alloc();
      *piVar4 = (int)piVar5;
      ((int (*)(int,int,int))(*(void **)(*piVar5 + 0x4c)))(piVar5,*(uint *)(puVar7 + 6),&local_38);
      local_38 = local_38 + 0x800;
      if (0xa400 < local_38) {
        local_38 = 0x400;
        local_30 = local_30 + 0x800;
      }
      ((int (*)(int,int))(*(void **)(*(int *)*piVar4 + 0x70)))((int *)*piVar4,1);
      *(uint *)(*piVar4 + 0x10) = *(uint *)(*piVar4 + 0x10) | 0x80000000;
    }
    iVar1 = iVar1 + 1;
    iVar6 = iVar6 + 0x1c;
  } while (iVar1 < 7);
  uVar3 = NopSub_20c5c();
  ((int (*)(int,int))(*(void **)(*index + 0x10)))(index,uVar3);
  ((int (*)(int))(*(void **)(*index + 0x40)))(index);
}

void LocationGrid_Destroy(int *index)
{
  uint uVar1;
  uint *puVar2;
  int *piVar3;
  int iVar4;
  uint *puVar5;
  int iVar6;
  int iVar7;
  uVar1 = NopSub_20c5c();
  iVar6 = 0;
  iVar7 = SIZEOF_CHUNK_DATA;
  ((int (*)(int,int))(*(void **)(*index + 0x14)))(index,uVar1);
  do {
    iVar4 = (int)index + iVar7;
    ((int (*)(int,int,int,int))(*(void **)(*index + 0x88)))(index,6,iVar4,iVar6);
    if (*(int **)(iVar4 + 4) != NULL) {
      ((int (*)(void))(*(void **)(**(int **)(iVar4 + 4) + 4)))();
    }
    if (*(int *)(iVar4 + 8) != 0) {
      piVar3 = *(int **)(*(int *)(iVar4 + 8) + 0x2c);
      if (piVar3 != NULL) {
        ((int (*)(void))(*(void **)(*piVar3 + 4)))();
      }
      uVar1 = ((int (*)(void))(*(void **)(**(int **)(iVar4 + 8) + 4)))();
      *(uint *)(iVar4 + 8) = uVar1;
    }
    if (*(int **)(iVar4 + 0xc) != NULL) {
      ((int (*)(void))(*(void **)(**(int **)(iVar4 + 0xc) + 4)))();
    }
    puVar2 = *(uint **)(iVar4 + 0x10);
    puVar5 = puVar2 + 0x19a;
    while (puVar2 < puVar5) {
      piVar3 = (int *)*puVar2;
      puVar2 = puVar2 + 1;
      if (piVar3 != NULL) {
        ((int (*)(void))(*(void **)(*piVar3 + 4)))();
      }
    }
    iVar7 = iVar7 + 0x1c;
    iVar6 = iVar6 + 1;
    MemFreeImpl(*(uint *)(iVar4 + 0x10));
  } while (iVar6 < 7);
  iVar6 = LocationGrid_GetDataPtr();
  ((int (*)(int))(*(void **)(iVar6 + 0xc)))(index);
}

void LocationGrid_DrawPrimitive(int *index, uint *id, uint id_2)
{
  int iVar1;
  iVar1 = GetPrimitiveBaseTable();
  ((int (*)(int,int,int))(*(void **)(iVar1 + 0x38)))(index,id,id);
  if ((*(uint *)*id & 0xf) == 1) {
    ((int (*)(int,int,int))(*(void **)(*index + 0x100)))(index,id,id);
  }
}

void LocationGrid_ResetState(int *index)
{
  ((LocationGrid *)index)->nObjState = 0;
  ((LocationGrid *)index)->field_0xe8 = 0;
  ((LocationGrid *)index)->pChild2 = 0;
  ((int (*)(int,int))(*(void **)(*index + 0xdc)))(index,0xa000);
  ((LocationGrid *)index)->sg_0x1CC = -1;
  ((LocationGrid *)index)->sg_0x1D0 = -1;
  ((LocationGrid *)index)->sg_0x1D4 = -1;
  ((LocationGrid *)index)->sg_0x1D8 = -1;
}

void LocationGrid_HandleChannelEvent(int *index, int index_2, int value)
{
  int iVar1;
  uint uVar2;
  iVar1 = GetPrimitiveBaseTable();
  ((int (*)(int,int))(*(void **)(iVar1 + 0x88)))(index,index);
  if (index_2 == 6) {
    if (*(int **)(value + 0x14) != NULL) {
      uVar2 = ((int (*)(void))(*(void **)(**(int **)(value + 0x14) + 4)))();
      *(uint *)(value + 0x14) = uVar2;
    }
  }
  else if (index_2 != 7) {
    return;
  }
  ((LocationGrid *)index)->sg_0x1BC = value;
  ((int (*)(int,int))(*(void **)(*index + 0x30)))(index,index);
}

void LocationGrid_Submit(int *index)
{
  if (((LocationGrid *)index)->nChildCount != 0) {
    ((int (*)(void))(*(void **)(*index + 0xf4)))();
    ((int (*)(int))(*(void **)(*index + 0x13c)))(index);
  }
}

void LocationGrid_CheckDrawMode(int *index,uint *id)
{
  if (*(char *)*id == '4') {
    ((int (*)(void))(*(void **)(*index + 0xd0)))();
  }
}

void LocationGrid_ResetChannels(int *index)
{
  uint uVar1;
  int *piVar2;
  u16 *puVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  iVar5 = 0;
  iVar6 = SIZEOF_CHUNK_DATA;
  do {
    puVar3 = (u16 *)((int)index + iVar6);
    ((int (*)(void))(*(void **)(**(int **)(puVar3 + 2) + 0x74)))();
    *puVar3 = 0;
    ((int (*)(int,int))(*(void **)(*index + 0x108)))(index,puVar3);
    iVar4 = *(int *)(puVar3 + 4);
    piVar2 = *(int **)(iVar4 + 0x2c);
    iVar6 = iVar6 + 0x1c;
    if (piVar2 != NULL) {
      uVar1 = ((int (*)(void))(*(void **)(*piVar2 + 4)))();
      *(uint *)(iVar4 + 0x2c) = uVar1;
    }
    ((int (*)(int,int,int,int))(*(void **)(*index + 0x88)))(index,6,puVar3,iVar5);
    iVar5 = iVar5 + 1;
    ((int (*)(void))(*(void **)(**(int **)(puVar3 + 2) + 0x84)))();
  } while (iVar5 < 7);
  ((LocationGrid *)index)->sg_0x1B8 = 0;
  *(u16 *)(index + 0x6d) = 0;
  ((int (*)(int))(*(void **)(*index + 0x140)))(index);
}

void LocationGrid_SetupChunks(int *index, int index_2, int value, int arg2)
{
  int *piVar1;
  int iVar2;
  int iVar3;
  iVar2 = 0;
  if (0 < index) {
    do {
      iVar3 = iVar2 + 1;
      piVar1 = (int *)((int (*)(int,int))(*(void **)(*index + 0xb8)))(index,iVar2);
      ((int (*)(int,int,int))(*(void **)(*piVar1 + 0x44)))(piVar1,1,arg2);
      ((int (*)(int,int,int))(*(void **)(*piVar1 + 0x48)))(piVar1,1,value);
      iVar2 = iVar3;
      value = value + 6;
      arg2 = arg2 + 3;
    } while (iVar3 < index);
  }
}

void LocationSub_SetPosition(int index,uint id,uint type)
{
  ((EntityObj *)index)->nField_60 = id;
  ((EntityObj *)index)->nField_64 = type;
}

void LocationSub_SetFlag(int index,uint id)
{
  ((EntityObj *)index)->field_0xe8 = id;
}

void LocationSub_CheckIdList(int *index, uint *id, int index_2)
{
  int iVar1;
  int *piVar2;
  if ((1 < index) &&
     (((index_2 < 4 || ((index_2 < 9 && (4 < index)))) &&
      (piVar2 = (int *)((EntityObj *)index)->field_0xe8, piVar2 != NULL)))) {
    iVar1 = *piVar2;
    while (iVar1 != 0) {
      iVar1 = *piVar2;
      piVar2 = piVar2 + 1;
      if (iVar1 == *(int *)*id) {
        ((int (*)(int,int,int))(*(void **)(*index + 300)))(index,index,index);
      }
      iVar1 = *piVar2;
    }
  }
}

void LocationSub_Render(int *index, int index_2, uint id)
{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int *piVar6;
  int local_78 [24];
  if (((EntityObj *)index)->nField_30 == 0) {
    iVar4 = 0;
  }
  else {
    iVar4 = ((EntityObj *)index)->nField_50 + 0x38;
  }
  iVar4 = ((int (*)(int,int,int))(*(void **)(*index + 0x110)))(index,local_78 + 0xc,iVar4);
  piVar6 = local_78;
  if (iVar4 == 0) {
    iVar4 = ((EntityObj *)index)->pChild2;
    piVar5 = index + 0x23;
    do {
      iVar1 = piVar5[1];
      iVar2 = piVar5[2];
      iVar3 = piVar5[3];
      *piVar6 = *piVar5;
      piVar6[1] = iVar1;
      piVar6[2] = iVar2;
      piVar6[3] = iVar3;
      piVar5 = piVar5 + 4;
      piVar6 = piVar6 + 4;
    } while (piVar5 != index + 0x2f);
    if (*(int *)(((EntityObj *)index)->nObjState + 4) == 0) {
      Render_SetViewport(index,local_78 + 0xc,3);
    }
    else {
      Render_CalcClipRect(index,local_78 + 0xc,3);
    }
    Render_SubmitChunks(index,index,id);
((EntityObj *)index)->pChild2 = (void *)iVar4;
    index_2 = index + 0x23;
    piVar6 = local_78;
    do {
      iVar4 = piVar6[1];
      iVar1 = piVar6[2];
      iVar2 = piVar6[3];
      *index = *piVar6;
      ((EntityObj *)index)->nPosX = iVar4;
      ((EntityObj *)index)->nPosY = iVar1;
      ((EntityObj *)index)->nPosZ = iVar2;
      piVar6 = piVar6 + 4;
      index_2 = index + 4;
    } while (piVar6 != local_78 + 0xc);
  }
}

void Render_SetViewport(int index,int value,uint id)
{
  char cVar1;
  ((EntityObj *)index)->pSubObj = *(char *)(value + 2) + -1;
  cVar1 = *(char *)(value + 3);
  ((EntityObj *)index)->nCondition = id;
  ((EntityObj *)index)->nSubState = id;
  *(short *)(index + 0x7e) = cVar1 + -1;
  Chunk_BuildSliceEntries();
}

void Render_CalcClipRect(int *index, int index_2, short val)
{
  char cVar1;
  char cVar2;
  int iVar3;
  short sVar4;
  short sVar5;
  short sVar6;
  int arg1;
  int arg1_2;
  cVar1 = *(char *)(index + 2);
  cVar2 = *(char *)(index + 3);
  if (cVar1 == '\0') {
    sVar4 = 0;
    sVar6 = arg1 + -1;
  }
  else {
    sVar4 = cVar1 + -1;
    sVar6 = arg1;
  }
  if (cVar1 == '\x13') {
    sVar6 = sVar6 + -1;
  }
  if (cVar2 == '\0') {
    arg1 = arg1 + -1;
    sVar5 = 0;
  }
  else {
    sVar5 = cVar2 + -1;
  }
  if (cVar2 == '\x13') {
    arg1 = arg1 + -1;
  }
((EntityObj *)index)->pChild2 = (void *)1;
  iVar3 = ((int (*)(int,int))(*(void **)(*index + 0x124)))(index,((EntityObj *)index)->field_0xa0);
((EntityObj *)index)->pSubVtbl = (void *)iVar3;
  ((EntityObj *)index)->nParam2 = sVar4;
  *(short *)((int)index + 0x92) = sVar5;
((EntityObj *)index)->pContainer = (void *)sVar6;
  *(short *)((int)index + 0x96) = arg1;
}

void Render_SubmitChunks(int index,uint id,uint type)
{
  int iVar1;
  int iVar2;
  short *psVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  int *local_48;
  local_48 = (int *)(index + 0x8c);
  iVar6 = 0;
  if (0 < ((EntityObj *)index)->pChild2) {
    psVar3 = (short *)(index + 0x96);
    do {
      iVar1 = index + *local_48 * 0x1c + SIZEOF_CHUNK_DATA;
      if (*(short *)(*(int *)(iVar1 + 4) + 0x2c) != 0) {
        iVar5 = 0;
        piVar4 = (int *)(psVar3[-3] * 4 + *(int *)(iVar1 + 0x10) + psVar3[-2] * 0x50);
        if (0 < *psVar3) {
          do {
            iVar1 = 0;
            if (0 < psVar3[-1]) {
              do {
                *(u16 *)(index + 0x1c0) = *(u16 *)(index + 0xbc);
                *(char *)(index + 0x1c2) = (char)psVar3[-3] + (char)iVar1;
                *(char *)(index + 0x1c3) = (char)psVar3[-2] + (char)iVar5;
                Chunk_RenderIfVisible(*piVar4,id,type);
                for (iVar2 = *(int *)(*piVar4 + 0x38); iVar2 != 0; iVar2 = *(int *)(iVar2 + 0x38)) {
                   Chunk_RenderIfVisible(iVar2,id,type);
}
                iVar1 = iVar1 + 1;
                piVar4 = piVar4 + 1;
              } while (iVar1 < psVar3[-1]);
}
            iVar5 = iVar5 + 1;
             piVar4 = piVar4 + (SIZEOF_RENDERNODE - psVar3[-1]);
          } while (iVar5 < *psVar3);
        }
      }
      local_48 = local_48 + 3;
      iVar6 = iVar6 + 1;
      psVar3 = psVar3 + 6;
    } while (iVar6 < ((EntityObj *)index)->pChild2);
  }
}

void Chunk_RenderIfVisible(int *index)
{
  if ((index != NULL) && ((*(ushort *)((int)index + 0x36) & 0x80) != 0)) {
    ((int (*)(void))(*(void **)(*index + 0x38)))();
  }
}

int LocationGrid_GetBufferEnd(int index)
{
  return index + 0x1c0;
}

void LocationGrid_Nop(void)
{
}

void LocationSub_SetDepthShift(int index,int value)
{
  ((EntityObj *)index)->nConfigVal = value;
  *(short *)(index + 0x7a) = (short)(value >> 0xb);
  ((EntityObj *)index)->nRndVal = (short)(value >> 0xc);
}

void LocationSub_SetMode(int *index, int index_2)
{
  ((int (*)(void))(*(void **)(*index + 0x40)))();
  ((EntityObj *)index)->nObjState = index;
}

void Render_UnpackPrimitive(int *index, uint id, int index_2, uint type)
{
  u16 uVar1;
  uint uVar2;
  uint *puVar3;
  uint in_v0;
  uint uVar4;
  uint uVar5;
  uint in_v1;
  uint uVar6;
  u8 auStack_18 [16];
  ((EntityObj *)index)->nSubTypeCount = index;
  uVar2 = type + 3 & 3;
  uVar6 = type & 3;
  uVar4 = (*(int *)((type + 3) - uVar2) << (3 - uVar2) * 8 |
          in_v0 & 0xffffffffU >> (uVar2 + 1) * 8) & -1 << (4 - uVar6) * 8 |
          *(uint *)(type - uVar6) >> uVar6 * 8;
  uVar2 = type + 7 & 3;
  uVar6 = type + 4 & 3;
  uVar6 = (*(int *)((type + 7) - uVar2) << (3 - uVar2) * 8 |
          in_v1 & 0xffffffffU >> (uVar2 + 1) * 8) & -1 << (4 - uVar6) * 8 |
          *(uint *)((type + 4) - uVar6) >> uVar6 * 8;
  uVar1 = *(u16 *)(type + 8);
  uVar2 = (int)index + 0xbfU & 3;
  puVar3 = (uint *)(((int)index + 0xbfU) - uVar2);
  *puVar3 = *puVar3 & -1 << (uVar2 + 1) * 8 | uVar4 >> (3 - uVar2) * 8;
  uVar2 = (uint)(index + 0x2f) & 3;
  puVar3 = (uint *)((int)(index + 0x2f) - uVar2);
  *puVar3 = *puVar3 & 0xffffffffU >> (4 - uVar2) * 8 | uVar4 << uVar2 * 8;
  uVar2 = (int)index + 0xc3U & 3;
  puVar3 = (uint *)(((int)index + 0xc3U) - uVar2);
  *puVar3 = *puVar3 & -1 << (uVar2 + 1) * 8 | uVar6 >> (3 - uVar2) * 8;
  uVar2 = (uint)(index + 0x30) & 3;
  puVar3 = (uint *)((int)(index + 0x30) - uVar2);
  *puVar3 = *puVar3 & 0xffffffffU >> (4 - uVar2) * 8 | uVar6 << uVar2 * 8;
  *(u16 *)(index + 0x31) = uVar1;
  uVar5 = Prim_CalcScreenCoord(id,auStack_18,((EntityObj *)index)->nObjState,index + 0x15,type);
  ((int (*)(int,int,int,int))(*(void **)(*index + 0xf8)))(index,uVar5,auStack_18,&Rendering_Chunk04);
}

void Prim_CalcScreenPos(int index,uint id,uint type)
{
  u8 auStack_18 [16];
  Prim_CalcScreenCoord(id,auStack_18,*(uint *)(index + 0x68),index + 0x54,type);
}

int Prim_CalcScreenCoord(int *index,int *value,short *val,int *arg2,char *c)
{
  int iVar1;
  int iVar2;
  uint uVar3;
  int iVar4;
  int iVar5;
  if (*(int *)(val + 2) == 0) {
    uVar3 = (uint)c[1];
    iVar4 = (int)val[1];
    iVar5 = (int)*c + (int)*val * uVar3;
  }
  else {
    iVar4 = 1;
    uVar3 = 0;
    iVar5 = 0;
  }
  iVar1 = *arg2 + *val * -0x5000 + *c * 0xa000;
  iVar2 = arg2[2];
  *value = iVar1;
  if ((uVar3 & 1) != 0) {
    *value = iVar1 + -0x5000;
  }
  iVar1 = arg2[1];
  value[2] = iVar2 + iVar4 * -0x5000 + uVar3 * 0xa000;
  value[1] = iVar1;
  *index = c[2] * 0x800 + *value + *(short *)(c + 4) + 0x400;
  index[1] = (int)*(short *)(c + 6) + value[1];
  index[2] = c[3] * 0x800 + value[2] + *(short *)(c + 8) + 0x400;
  *value = *value + 0x5000;
  value[2] = value[2] + 0x5000;
  return iVar5;
}

void LocationSub_SetActive(int index)
{
  ((EntityObj *)index)->nChildCount = 1;
}

void LocationSub_ClearBuffer(int *index)
{
  ((int (*)(void))(*(void **)(*index + 0xc0)))();
  ((EntityObj *)index)->nChildCount = 0;
}

int LocationSub_RefreshState(int *index)
{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  int *piVar8;
  int *piVar9;
  short local_40 [6];
  u8 local_34 [20];
  int iStack_20;
  int local_1c;
  uint local_18;
  iVar2 = ((int (*)(int,int,int))(*(void **)(*index + 0x10c)))(index,local_40,0);
  iVar3 = 0;
  if (iVar2 != 0) {
    iVar3 = (int)(char)(&Rendering_ChunkFc)[*(short *)(*(int *)(local_1c + 4) + 0x32)];
    if (*(int *)(((EntityObj *)index)->nObjState + 4) == 0) {
      ((int (*)(int,int,int,int))(*(void **)(*index + 0xf8)))(index,local_18,local_34,*(uint *)(&Rendering_Chunk74 + iVar3 * 4));
    }
    ((int (*)(int))(*(void **)(*index + 0x128)))(index);
    iVar2 = ((EntityObj *)index)->field_0xbc;
    piVar8 = (int *)local_40;
    piVar1 = index + 0x2f;
    do {
      piVar9 = piVar1;
      piVar7 = piVar8;
      iVar4 = piVar7[1];
      iVar5 = piVar7[2];
      iVar6 = piVar7[3];
      *piVar9 = *piVar7;
      piVar9[1] = iVar4;
      piVar9[2] = iVar5;
      piVar9[3] = iVar6;
      piVar8 = piVar7 + 4;
      piVar1 = piVar9 + 4;
    } while (piVar8 != &iStack_20);
    iVar4 = piVar7[5];
    iVar5 = piVar7[6];
    piVar9[4] = *piVar8;
    piVar9[5] = iVar4;
    piVar9[6] = iVar5;
    if ((short)iVar2 != local_40[0]) {
      ((int (*)(int,int))(*(void **)(*index + 0x30)))(index,5);
    }
  }
  return iVar3;
}

void LocationGrid_RenderChannels(int *index, int index_2, int *value, byte *val)
{
  int iVar1;
  int iVar2;
  int iVar3;
  u8 *puVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  uint uVar9;
  u8 auStack_88 [88];
  uint local_30;
  if (val != (byte *)0x0) {
    iVar8 = (int)*(short *)((LocationGrid *)index)->nObjState;
    if (iVar8 == 0) {
      trap(0x1c00);
    }
    if ((iVar8 == -1) && (index_2 == -0x80000000)) {
      trap(0x1800);
    }
    iVar7 = 0;
    iVar5 = 0;
    iVar6 = 0;
    uVar9 = (int)index / iVar8 & 1;
    local_30 = LocationGrid_GetVisMask((int)index,(int)index,uVar9);
    do {
      iVar1 = ((int (*)(int,int))(*(void **)(*index + 0x118)))(index,iVar5);
      *(ushort *)(iVar1 + 2) = (ushort)*val;
      if (val[1] != 0) {
        iVar2 = (uint)*val * 0xc;
        iVar3 = *(int *)(*(int *)(iVar1 + 0xc) + 0x14);
        if (*(int *)(((LocationGrid *)index)->nObjState + 4) == 0) {
          *(int *)(iVar3 + 0x18) = *value + *(int *)(&Rendering_Adj38 + iVar2);
          *(int *)(iVar3 + 0x1c) = value[1];
          iVar2 = value[2] + *(int *)(&Rendering_Adj40 + iVar2);
        }
        else {
          *(int *)(iVar3 + 0x18) = *value + -0x5000;
          *(int *)(iVar3 + 0x1c) = value[1] + *(int *)(&Rendering_Adj3c + iVar2);
          iVar2 = value[2] + -0x5000;
        }
        *(int *)(iVar3 + 0x20) = iVar2;
        puVar4 = auStack_88 + iVar6;
        iVar6 = iVar6 + 0xc;
        **(uint **)(*(int *)(iVar1 + 0xc) + 0x14) = 0;
        iVar7 = iVar7 + 1;
        LocationGrid_GetChunkEntry((int)index,(uint *)puVar4,iVar8,uVar9,(uint)index,local_30,*val);
      }
      iVar5 = iVar5 + 1;
      val = val + 2;
    } while (iVar5 < 7);
    iVar5 = 0;
    iVar8 = SIZEOF_CHUNK_DATA;
    do {
      iVar5 = iVar5 + 1;
      *(u16 *)(*(int *)((int)index + iVar8 + 4) + 0x32) =
           *(u16 *)((int)index + iVar8 + 2);
      iVar8 = iVar8 + 0x1c;
    } while (iVar5 < 7);
    ((int (*)(int,int,int))(*(void **)(*index + 0xfc)))(index,auStack_88,iVar7);
  }
}

uint LocationGrid_GetVisMask(int index,int value,int arg2)
{
  short *psVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  psVar1 = ((LocationGrid *)index)->nObjState;
  iVar4 = (int)*psVar1;
  iVar3 = (int)psVar1[1];
  if (*(int *)(psVar1 + 2) == 0) {
    uVar2 = -(uint)(value < iVar4) & 3;
    if (iVar4 * (iVar3 + -1) <= value) {
      uVar2 = uVar2 | 0x60;
    }
    if (iVar4 == 0) {
      trap(0x1c00);
    }
    if ((iVar4 == -1) && (value == -0x80000000)) {
      trap(0x1800);
    }
    if (value % iVar4 == 0) {
      if (arg2 == 0) {
        uVar2 = uVar2 | 4;
      }
      else {
        uVar2 = uVar2 | 0x25;
      }
    }
    if (iVar4 == 0) {
      trap(0x1c00);
    }
    if ((iVar4 == -1) && (value + 1 == -0x80000000)) {
      trap(0x1800);
    }
    if ((value + 1) % iVar4 == 0) {
      if (arg2 == 0) {
        uVar2 = uVar2 | 0x52;
      }
      else {
        uVar2 = uVar2 | 0x10;
      }
    }
  }
  else {
    uVar2 = 0xffffffff;
    iVar4 = 0;
    if (0 < iVar3) {
      do {
        iVar4 = iVar4 + 1;
        uVar2 = uVar2 << 1;
      } while (iVar4 < iVar3);
    }
  }
  return ~uVar2;
}
uint
LocationGrid_GetChunkEntry(int index,uint *id,int value,int arg2,int arg3,uint id_2,
            int arg5)
{
  uint uVar1;
  int iVar2;
  int iVar3;
  uVar1 = 0;
  if ((*id & *(uint *)(&Location_AdjEdgeFlags + arg5 * 4)) == 0) {
    *id = 0;
  }
  else {
    iVar2 = arg3 + arg5;
    if (*(int *)(*(int *)(index + 0x68) + 4) == 0) {
      iVar2 = arg5 * 0xc;
      if (*(int *)(&Location_AdjDirectionOffsets + iVar2) == 0) {
        iVar2 = arg3 + *(int *)(&Rendering_AdjDirAc + iVar2);
      }
      else {
        if (arg2 == 0) {
          iVar3 = *(int *)(&Rendering_AdjDirB0 + iVar2);
        }
        else {
          iVar3 = *(int *)(&Rendering_AdjDirAc + iVar2);
        }
        iVar2 = arg3 + index * *(int *)(&Location_AdjDirectionOffsets + iVar2) + iVar3;
      }
    }
    id[1] = iVar2;
    uVar1 = ((int (*)(int,int,int,int))(*(void **)(index + 0x60)))(*(uint *)(index + 100),id[1],0,0);
    *id = uVar1;
    uVar1 = 1;
  }
  id[2] = arg5;
  return uVar1;
}

void LocationGrid_UpdateChannels(int *index, int *value, int index_2)
{
  u16 uVar1;
  u16 *puVar2;
  int iVar3;
  int *piVar4;
  iVar3 = 0;
  if (0 < index) {
    piVar4 = value + 1;
    do {
      puVar2 = (u16 *)((int (*)(int,int))(*(void **)(*index + 0x118)))(index,piVar4[1]);
      ((int (*)(int,int,int,int))(*(void **)(*index + 0x88)))(index,6,puVar2,iVar3);
      if (*value == 0) {
        if (*(short *)(*(int *)(puVar2 + 2) + 0x2c) != 0) {
          ((int (*)(int,int))(*(void **)(*index + 0x108)))(index,puVar2);
        }
        if (*(short *)((int)*(int **)(puVar2 + 2) + 0x2a) != 0) {
          ((int (*)(void))(*(void **)(**(int **)(puVar2 + 2) + 0x74)))();
          *puVar2 = 0;
        }
      }
      else {
        if (*(short *)(*(int *)(puVar2 + 2) + 0x2c) != 0) {
          ((int (*)(int,int))(*(void **)(*index + 0x108)))(index,puVar2);
        }
        *(short *)(*(int *)(puVar2 + 2) + 0x30) = (short)*piVar4;
        ((int (*)(int,int))(*(void **)(**(int **)(puVar2 + 2) + 0x78)))(*(int **)(puVar2 + 2),*value);
        *puVar2 = 1;
        ((LocationGrid *)index)->sg_0x1B0 = 1;
      }
      piVar4 = piVar4 + 3;
      iVar3 = iVar3 + 1;
      value = value + 3;
    } while (iVar3 < index);
  }
  uVar1 = LocationGrid_CountActiveChannels(index);
  *(u16 *)(index + 0x6d) = uVar1;
}

int LocationGrid_CountActiveChannels(int index)
{
  short *psVar1;
  int iVar2;
  int iVar3;
  iVar3 = 0;
  iVar2 = 0;
  do {
    psVar1 = (short *)(index + SIZEOF_CHUNK_DATA);
    index = index + 0x1c;
    if (*psVar1 != 0) {
      iVar3 = iVar3 + 1;
    }
    iVar2 = iVar2 + 1;
  } while (iVar2 < 7);
  return iVar3;
}

void LocationGrid_ProcessChannelState(int *index, uint *id, int index_2)
{
  short sVar1;
  short *psVar2;
  int iVar3;
  int iVar4;
  if (index_2 == 2) {
    iVar3 = 0;
    iVar4 = SIZEOF_CHUNK_DATA;
    do {
      psVar2 = (short *)((int)index + iVar4);
      if (*(short *)(*(int *)(psVar2 + 2) + 0x2e) != 0) {
        *(u16 *)(*(int *)(psVar2 + 2) + 0x2e) = 0;
        ((int (*)(int,int,int,int))(*(void **)(*index + 0x88)))(index,7,psVar2,iVar3);
      }
      if ((((LocationGrid *)index)->sg_0x1B0 == 1) && (*psVar2 != 0)) {
        if (*(short *)(*(int *)(psVar2 + 2) + 0x2c) == 0) {
          if (*(short *)(*(int *)(psVar2 + 2) + 0x2a) == 0) {
            *psVar2 = 0;
          }
        }
        else {
          ((int (*)(int,int))(*(void **)(*index + 0x104)))(index,psVar2);
          *(u16 *)(*(int *)(psVar2 + 2) + 0x2c) = 2;
          *psVar2 = 0;
          sVar1 = (short)((LocationGrid *)index)->sg_0x1B4 + -1;
          ((LocationGrid *)index)->sg_0x1B4 = sVar1;
          if (sVar1 == 0) {
            *(u16 *)(id + 0x6d) = 0;
            id[0x6c] = 0;
            id[0x6e] = 1;
          }
        }
      }
      iVar3 = iVar3 + 1;
      iVar4 = iVar4 + 0x1c;
    } while (iVar3 < 7);
  }
}

void Tmd_LoadModel(uint id,int index)
{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  int *piVar7;
  int iVar8;
  u8 auStack_70 [12];
  uint local_64;
  uint local_60;
  uint local_5c;
  u16 local_56;
  u16 local_42;
  int local_40;
  int local_3c;
  int local_30 [4];
  iVar4 = *(int *)(index + 4);
  piVar7 = *(int **)(index + 8);
  iVar2 = *(int *)(iVar4 + 0x10);
  iVar3 = *(int *)(iVar2 + 4);
  piVar7[5] = 0;
  piVar7[4] = iVar2 + iVar3;
  if ((int *)piVar7[0xb] != NULL) {
    ((int (*)(void))(*(void **)(*(int *)piVar7[0xb] + 4)))();
  }
  iVar8 = 0;
  iVar3 = 0;
  iVar2 = *(int *)(iVar4 + 0x10);
  local_30[0] = iVar2 + *(int *)(iVar2 + 4) + *(int *)(iVar2 + 8);
  iVar2 = Tmd_New(local_30);
  piVar7[0xb] = iVar2;
  local_3c = 0;
  iVar2 = 0x640;
  while (iVar4 = ((int (*)(int,int,int))(*(void **)(*piVar7 + 0x78)))(piVar7,auStack_70,iVar8), iVar4 != 0) {
    iVar6 = iVar2;
    if (iVar4 == -1) {
      piVar5 = (int *)(iVar3 + *(int *)(index + 0x10));
      *(uint *)(*piVar5 + 0x10) = *(uint *)(*piVar5 + 0x10) | 0x80000000;
      *(uint *)(*piVar5 + 0x20) = 0;
      *(uint *)(*piVar5 + 0x18) = 0;
    }
    else {
      iVar1 = iVar3;
      if (local_40 != 0) {
        iVar6 = iVar2 + 4;
        iVar1 = iVar2;
      }
      piVar5 = (int *)(*(int *)(index + 0x10) + iVar1);
      *(int *)(*piVar5 + 0x20) = iVar4;
      *(uint *)(*piVar5 + 0x18) = *(uint *)(*(int *)(*piVar5 + 0x20) + 0x10);
      GsLinkObject4(*(ulong *)(*(int *)(*piVar5 + 0x20) + 0x10),(GsDOBJ2 *)(*piVar5 + 0x10),0);
      iVar2 = *(int *)(*piVar5 + 0x14);
      *(uint *)(iVar2 + 0x18) = local_64;
      *(uint *)(iVar2 + 0x1c) = local_60;
      *(uint *)(iVar2 + 0x20) = local_5c;
      iVar2 = *(int *)(*(int *)(*piVar5 + 0x14) + 0x44);
      *(u16 *)(iVar2 + 0x10) = 0;
      *(u16 *)(iVar2 + 0x14) = 0;
      *(u16 *)(iVar2 + 0x12) = local_56;
      *(u16 *)(*piVar5 + 0x36) = local_42;
      **(uint **)(*piVar5 + 0x14) = 0;
      *(uint *)(*piVar5 + 0x10) = *(uint *)(*piVar5 + 0x10) | 0x80000000;
    }
    iVar2 = iVar6;
    if (local_3c == 0) {
      iVar3 = iVar3 + 4;
      iVar8 = iVar8 + 1;
      *(uint *)(*piVar5 + 0x38) = 0;
    }
    else {
      *(uint *)(*piVar5 + 0x38) = *(uint *)(iVar6 + *(int *)(index + 0x10));
    }
  }
}

void LocationGrid_ResetPrimitives(uint id,int index)
{
  int *piVar1;
  int *piVar2;
  if (-1 < (short)(*(int **)(index + 4))[0xc]) {
    ((int (*)(void))(*(void **)(**(int **)(index + 4) + 0x7c)))();
    piVar1 = *(int **)(index + 0x10);
    piVar2 = piVar1 + 0x19a;
    for (; piVar1 < piVar2; piVar1 = piVar1 + 1) {
      *(uint *)(*piVar1 + 0x10) = *(uint *)(*piVar1 + 0x10) | 0x80000000;
      *(uint *)(*piVar1 + 0x20) = 0;
      *(uint *)(*piVar1 + 0x18) = 0;
    }
  }
}

int LocationGrid_GetRenderBuffer(int *index, int index_2, int *value)
{
  int iVar1;
  iVar1 = *(int *)(((LocationGrid *)index)->nSubTypeCount + 0x14) + 0x18;
  if (value != NULL) {
    *value = iVar1;
  }
  if ((index_2 != 0) && (iVar1 = ((int (*)(int,int,int))(*(void **)(*index + 0x110)))(index,index,iVar1), iVar1 != 0)
     ) {
    return NULL;
  }
  return index + 0x2f;
}

uint LocationGrid_WorldToTile(int *index, int index_2, int *value)
{
  short sVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  iVar2 = ((int (*)(int,int))(*(void **)(*index + 0x11c)))(index,value);
  if (iVar2 == 0) {
    uVar4 = 1;
  }
  else {
    ((LocationGrid *)index)->field_0xa0 = (int)*(short *)(*(int *)(iVar2 + 4) + 0x30);
    Prim_WriteModDiv(index,index);
    iVar3 = ((int (*)(int,int))(*(void **)(*index + 0x118)))(index,(int)*(short *)(*(int *)(iVar2 + 4) + 0x32));
    iVar3 = *(int *)(*(int *)(iVar3 + 0xc) + 0x14);
    ((LocationGrid *)index)->nField_30 = *(int *)(iVar3 + 0x18) + 0x5000;
    ((LocationGrid *)index)->pSomePtr = *(uint *)(iVar3 + 0x1c);
    ((LocationGrid *)index)->nField_50 = *(int *)(iVar3 + 0x20) + 0x5000;
    iVar5 = *(int *)(*(int *)(iVar2 + 0xc) + 0x14);
    ((LocationGrid *)index)->nField_60 = *value - ((LocationGrid *)index)->nField_30;
    ((LocationGrid *)index)->nChildCount = value[1];
    ((LocationGrid *)index)->nCondition = value[2] - ((LocationGrid *)index)->nField_50;
    iVar3 = *value - *(int *)(iVar5 + 0x18);
    if (iVar3 < 0) {
      iVar3 = iVar3 + 0x7ff;
    }
    *(char *)(index + 2) = (char)(iVar3 >> 0xb);
    iVar3 = value[2] - *(int *)(iVar5 + 0x20);
    if (iVar3 < 0) {
      iVar3 = iVar3 + 0x7ff;
    }
    *(char *)(index + 3) = (char)(iVar3 >> 0xb);
    ((LocationGrid *)index)->nDist =
         ((short)*value + -0x400) - (*(short *)(iVar5 + 0x18) + *(char *)(index + 2) * 0x800);
    ((LocationGrid *)index)->pWorld = (short)value[1];
    uVar4 = 0;
    iVar3 = value[2];
    sVar1 = *(short *)(iVar5 + 0x20);
    ((LocationGrid *)index)->nParam2 = iVar2;
    ((LocationGrid *)index)->nField_20 = ((short)iVar3 + -0x400) - (sVar1 + *(char *)(index + 3) * 0x800);
  }
  return uVar4;
}

void Prim_WriteModDiv(int index,u8 *arg0,int value)
{
  int iVar1;
  iVar1 = (int)**(short **)(index + 0x68);
  if (iVar1 == 0) {
    trap(0x1c00);
  }
  if ((iVar1 == -1) && (value == -0x80000000)) {
    trap(0x1800);
  }
  *arg0 = (char)(value % iVar1);
  iVar1 = (int)**(short **)(index + 0x68);
  if (iVar1 == 0) {
    trap(0x1c00);
  }
  if ((iVar1 == -1) && (value == -0x80000000)) {
    trap(0x1800);
  }
  arg0[1] = (char)(value / iVar1);
}
/* Tmd_New -- Allocate and initialize a TMD object */
extern int MemAllocImpl(int size);
extern void MemFreeImpl(int ptr);
extern u32 D_8006F13C[];
int Tmd_New(int *a0)
{
    int obj;
    int vtab;
    int result;
    obj = MemAllocImpl(0x30);
    if (obj != 0) {
        vtab = (int)D_8006F13C;
        result = ((int (*)(int, int *))(*(void **)(vtab + 8)))(obj, a0);
        if (result != 0) {
            return obj;
        }
        MemFreeImpl(obj);
    }
    return 0;
}
