/* Rendering_Chunk.c -- split from Rendering.c */
#include "Rendering_Shared.h"
uint Chunk_CalcTileCoord(int index,uint id)
{
  Prim_WriteModDiv(index,id,(int)*(short *)(*(int *)(*(int *)(index + 0x1bc) + 4) + 0x30));
  return *(uint *)(index + 0x1bc);
}

int Chunk_FindByType(int index,int value)
{
  int iVar1;
  int iVar2;
  iVar2 = 0;
  iVar1 = SIZEOF_CHUNK_DATA;
  do {
    if (*(short *)(*(int *)(index + iVar1 + 4) + 0x32) == value) {
      return index + iVar1;
    }
    iVar2 = iVar2 + 1;
    iVar1 = iVar1 + 0x1c;
  } while (iVar2 < 7);
  return 0;
}

int Chunk_FindByCoord(int *index,int *value)
{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  iVar5 = 0;
  iVar4 = 0;
  do {
    iVar1 = ((int (*)(int,int))(*(void **)(*index + 0x118)))(index,iVar5);
    iVar3 = *(int *)(*(int *)(iVar1 + 0xc) + 0x14);
    iVar2 = *(int *)(iVar3 + 0x18);
    if ((iVar2 <= *value) && (*value < iVar2 + 0xa000)) {
      iVar2 = *(int *)(iVar3 + 0x20);
      if ((iVar2 <= value[2]) && (value[2] < iVar2 + 0xa000)) {
        if (*(int *)(((EntityObj *)index)->nObjState + 4) == 0) {
          return iVar1;
        }
        if ((value[1] <= iVar4) && (iVar4 + -0x800 < value[1])) {
          return iVar1;
        }
      }
    }
    iVar5 = iVar5 + 1;
    iVar4 = iVar4 + -0x800;
    if (6 < iVar5) {
      return 0;
    }
  } while( true );
}

int Chunk_FindIndexByType(int index,int value)
{
  int iVar1;
  int iVar2;
  iVar1 = 0;
  iVar2 = SIZEOF_CHUNK_DATA;
  do {
    if (*(short *)(*(int *)(index + iVar2 + 4) + 0x32) == value) {
      return iVar1;
    }
    iVar1 = iVar1 + 1;
    iVar2 = iVar2 + 0x1c;
  } while (iVar1 < 7);
  return 0;
}

int Chunk_FindActiveIndex(int index,int value)
{
  int iVar1;
  int iVar2;
  int iVar3;
  iVar2 = 0;
  iVar3 = SIZEOF_CHUNK_DATA;
  while ((iVar1 = *(int *)(index + iVar3 + 4), *(short *)(iVar1 + 0x30) != value ||
         (*(short *)(iVar1 + 0x2c) == 0))) {
    iVar2 = iVar2 + 1;
    iVar3 = iVar3 + 0x1c;
    if (6 < iVar2) {
      return -1;
    }
  }
  return iVar2;
}

void Chunk_RenderDispatch(int index)
{
  short sVar1;
  if (*(int *)(index + 0x1b8) != 0) {
    sVar1 = *(short *)(index + 0x78);
    Chunk_ProcessRenderList(index,0);
    if (*(int *)(*(int *)(index + 0x68) + 4) == 0) {
      Chunk_UpdateRenderTransform(index,(int)sVar1 << 1,(int)*(short *)(index + 0x7a));
    }
    else {
      Chunk_RenderGpu(index);
    }
    Chunk_ProcessRenderList(index,1);
  }
}

void Chunk_UpdateRenderTransform(int *index, int index_2, int value)
{
  short sVar1;
  int iVar2;
  int unaff_s3;
  int iVar3;
  u8 auStack_88 [2];
  char local_86;
  char local_85;
  u8 local_58 [24];
  long local_40;
  int local_3c;
  int local_38;
  iVar2 = *(int *)(*(int *)(((EntityObj *)index)->nSubTypeCount + 0x14) + 0x44);
  ((int (*)(int,int,int))(*(void **)(*index + 0x10c)))(index,auStack_88,0);
  sVar1 = *(short *)(iVar2 + 0x12);
  if (*(short *)(iVar2 + 0x12) < 0) {
    sVar1 = sVar1 + FIXED_12;
  }
  *(volatile unsigned short*)&(local_58) = (short)Rendering_Spu8c;
  ((volatile unsigned short*)&(local_58))[1] = ((volatile unsigned short*)&(Rendering_Spu8c))[1];
  *(volatile unsigned short*)(local_58 + 4) = (short)Rendering_Spu90;
  *(volatile unsigned short*)(local_58 + 6) = ((volatile unsigned short*)&(Rendering_Spu90))[1];
  *(volatile unsigned short*)(local_58 + 8) = (short)Rendering_Spu94;
  *(volatile unsigned short*)(local_58 + 10) = ((volatile unsigned short*)&(Rendering_Spu94))[1];
  *(volatile unsigned short*)(local_58 + 12) = (short)Rendering_Spu98;
  *(volatile unsigned short*)(local_58 + 14) = ((volatile unsigned short*)&(Rendering_Spu98))[1];
  *((volatile unsigned int*)((int)&(local_58) + 16)) = Rendering_Spu9c;
  *((volatile unsigned int*)((int)&(local_58) + 20)) = 0;
  local_40 = 0;
  local_3c = ((EntityObj *)index)->nConfigVal;
  RotMatrix((SVECTOR *)(iVar2 + 0x10),(MATRIX *)local_58);
  ApplyMatrixLV((MATRIX *)local_58,(VECTOR *)(local_58 + 0x14),(VECTOR *)(local_58 + 0x14));
  if (((ushort)(sVar1 - 0x200U) < DEGREES_90) || ((ushort)(sVar1 - 0xa00U) < DEGREES_90)) {
    ((EntityObj *)index)->nCondition = value;
    ((EntityObj *)index)->nSubState = index;
    sVar1 = (short)local_86;
    if (((volatile unsigned int*)local_58)[5] < 1) {
      sVar1 = ((short)local_86 - (short)index) + 1;
    }
    *(short *)(index + 0x1f) = sVar1;
    if (local_3c < 1) {
      sVar1 = ((short)local_85 - (short)((EntityObj *)index)->nRndVal) + 1;
    }
    else {
      sVar1 = ((short)local_85 - (short)((EntityObj *)index)->nRndVal) + -1;
    }
    *(short *)((int)index + 0x7e) = sVar1;
    local_38 = 0;
    unaff_s3 = local_3c;
  }
  else if (((ushort)(sVar1 - 0x600U) < DEGREES_90) || (0xbff < (ushort)(sVar1 - 0x200U))) {
    ((EntityObj *)index)->nCondition = index;
    ((EntityObj *)index)->nSubState = value;
    if (((volatile unsigned int*)local_58)[5] < 1) {
      sVar1 = ((short)local_86 - (short)((EntityObj *)index)->nRndVal) + 1;
    }
    else {
      sVar1 = ((short)local_86 - (short)((EntityObj *)index)->nRndVal) + -1;
    }
    *(short *)(index + 0x1f) = sVar1;
    sVar1 = (short)local_85;
    if (local_3c < 1) {
      sVar1 = ((short)local_85 - (short)value) + 1;
    }
    *(short *)((int)index + 0x7e) = sVar1;
    local_38 = 1;
    unaff_s3 = *((volatile unsigned int*)((int)&(local_58) + 20));
  }
  iVar2 = (int)(short)((EntityObj *)index)->nRndVal;
  iVar3 = unaff_s3 >> 0xb;
  if (iVar2 <= iVar3) {
    iVar3 = iVar2 + -1;
  }
  if (iVar3 <= -iVar2) {
    iVar3 = -iVar2 + 1;
  }
  if (local_38 == 0) {
    *(short *)((int)index + 0x7e) = *(short *)((int)index + 0x7e) + (short)iVar3;
  }
  else {
    *(short *)(index + 0x1f) = (short)((EntityObj *)index)->pSubObj + (short)iVar3;
  }
  Chunk_BuildSliceEntries(index);
}

void Chunk_BuildSliceEntries(int *index)
{
  bool bVar1;
  u16 uVar2;
  short sVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  char cVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  iVar5 = (int)(short)((EntityObj *)index)->pSubObj;
  iVar9 = ((EntityObj *)index)->nCondition;
  iVar10 = ((EntityObj *)index)->nSubState;
  bVar1 = -1 < iVar5;
  cVar7 = '\x03';
  if (!bVar1) {
    iVar5 = iVar5 + 0x14;
    cVar7 = '\x02';
  }
  iVar8 = (int)*(short *)((int)index + 0x7e);
  if (iVar8 < 0) {
    iVar8 = iVar8 + 0x14;
    if (bVar1) {
      cVar7 = 9 < iVar5;
      if ((bool)cVar7) {
        iVar5 = iVar5 + -10;
      }
      else {
        iVar5 = iVar5 + 10;
      }
    }
    else {
      iVar5 = iVar5 + -10;
      cVar7 = '\0';
    }
  }
  iVar4 = ((int (*)(int,int))(*(void **)(*index + 0x120)))(index,cVar7);
  ((EntityObj *)index)->pSubVtbl = iVar4;
  iVar4 = iVar5;
  if (iVar5 < 0) {
    iVar4 = 0;
  }
  *(short *)(index + 0x24) = (short)iVar4;
  *(short *)((int)index + 0x92) = (short)iVar8;
  if (iVar5 + iVar9 < 0x15) {
    *(short *)(index + 0x25) = (short)iVar9;
    iVar5 = Chunk_BuildSliceOverflow(index,index + 0x23,0,cVar7,iVar5,iVar8,iVar9,iVar10);
  }
  else {
    sVar3 = (short)(iVar5 + iVar9) + -0x14;
    *(short *)(index + 0x25) = (short)iVar9 - sVar3;
    iVar5 = Chunk_BuildSliceOverflow(index,index + 0x23,0,cVar7,iVar5,iVar8,iVar9,iVar10);
    iVar5 = iVar5 + 1;
    piVar6 = index + iVar5 * 3 + 0x23;
    iVar9 = ((int (*)(int,int))(*(void **)(*index + 0x120)))(index,cVar7 + '\x01');
    *piVar6 = iVar9;
    *(u16 *)(piVar6 + 1) = 0;
    uVar2 = *(u16 *)((int)index + 0x92);
    *(short *)(piVar6 + 2) = sVar3;
    *(u16 *)((int)piVar6 + 6) = uVar2;
    *(u16 *)((int)piVar6 + 10) = *(u16 *)((int)index + 0x96);
  }
  ((EntityObj *)index)->pChild2 = iVar5 + 1;
}
int Chunk_BuildSliceOverflow(int *index, int index_3, int value, int arg2, int arg3, int arg4,
    int index_4, int value_3)
{
  short sVar1;
  short sVar2;
  int iVar3;
  int *piVar4;
  if (arg4 + value_3 < 0x15) {
    *(short *)(index + 10) = (short)value_3;
  }
  else {
    sVar1 = (short)(arg4 + value_3) + -0x14;
    *(short *)(index + 10) = (short)value_3 - sVar1;
    piVar4 = index + (value + 1) * 3 + 0x23;
    if (arg3 < 10) {
      arg2 = arg2 + 2;
      iVar3 = ((int (*)(int,int))(*(void **)(*index + 0x120)))(index,arg2);
      *piVar4 = iVar3;
      sVar2 = (short)arg3 + 10;
    }
    else {
      arg2 = arg2 + 3;
      iVar3 = ((int (*)(int,int))(*(void **)(*index + 0x120)))(index,arg2);
      *piVar4 = iVar3;
      sVar2 = (short)arg3 + -10;
    }
    *(short *)(piVar4 + 1) = sVar2;
    *(short *)((int)piVar4 + 10) = sVar1;
    iVar3 = (short)piVar4[1] + index_4;
    *(u16 *)((int)piVar4 + 6) = 0;
    if (iVar3 < 0x15) {
      *(short *)(piVar4 + 2) = (short)index_4;
      value = value + 1;
    }
    else {
      sVar2 = (short)iVar3;
      *(short *)(piVar4 + 2) = ((short)index_4 + 0x14) - sVar2;
      piVar4 = index + (value + 2) * 3 + 0x23;
      iVar3 = ((int (*)(int,int))(*(void **)(*index + 0x120)))(index,arg2 + 1);
      *piVar4 = iVar3;
      *(u16 *)(piVar4 + 1) = 0;
      *(u16 *)((int)piVar4 + 6) = 0;
      *(short *)(piVar4 + 2) = sVar2 + -0x14;
      *(short *)((int)piVar4 + 10) = sVar1;
      value = value + 2;
    }
  }
  return value;
}

void Chunk_RenderGpu(int *index)
{
  int iVar1;
  u8 auStack_40 [2];
  u8 auStack_3e [38];
  int local_18;
  ((int (*)(int,int,int))(*(void **)(*index + 0x10c)))(index,auStack_40,0);
  ((EntityObj *)index)->pChild2 = 0;
  iVar1 = Chunk_SetupRenderSlot(index);
  ((EntityObj *)index)->pChild2 = iVar1;
  iVar1 = Prim_IsPointInRect(index[0x77],auStack_3e);
  if ((iVar1 != 0) && (local_18 + 1 < (int)*(short *)(((EntityObj *)index)->nObjState + 2))) {
    iVar1 = Chunk_SetupRenderSlot(index);
    ((EntityObj *)index)->pChild2 = iVar1;
  }
  if (-1 < local_18 + -1) {
    iVar1 = Chunk_SetupRenderSlot(index);
    ((EntityObj *)index)->pChild2 = iVar1;
  }
}

bool Prim_IsPointInRect(short *val,char *c)
{
  bool bVar1;
  bVar1 = true;
  if ((((val != (short *)0x0) && ((int)*val <= (int)*c)) &&
      ((int)*c <= *(int *)(val + 2))) && ((int)val[1] <= (int)c[1])) {
    bVar1 = *(int *)(val + 4) < (int)val[1];
  }
  return bVar1;
}

int Chunk_SetupRenderSlot(int *index, uint id, int index_2, uint type)
{
  int iVar1;
  int *piVar2;
  piVar2 = index + (int)index * 3 + 0x23;
  *piVar2 = -1;
  piVar2[1] = 0;
  piVar2[2] = 0x140014;
  iVar1 = ((int (*)(int,int))(*(void **)(*index + 0x124)))(index,type);
  *piVar2 = iVar1;
  return index + 1;
}

void Chunk_ProcessRenderList(int index,int value)
{
  uint uVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  short *psVar5;
  int iVar6;
  int iVar7;
  int *piVar8;
  piVar8 = (int *)(index + 0x8c);
  iVar7 = 0;
  if (0 < *(int *)(index + 0x88)) {
    psVar5 = (short *)(index + 0x96);
    do {
      iVar2 = index + *piVar8 * 0x1c + SIZEOF_CHUNK_DATA;
      if (*(short *)(*(int *)(iVar2 + 4) + 0x2c) != 0) {
        iVar6 = 0;
        piVar4 = (int *)(psVar5[-3] * 4 + *(int *)(iVar2 + 0x10) + psVar5[-2] * 0x50);
        if (0 < *psVar5) {
          do {
            iVar2 = 0;
            if (0 < psVar5[-1]) {
              do {
                if (value == 0) {
                  iVar3 = *piVar4;
                  uVar1 = *(uint *)(iVar3 + 0x10) | 0x80000000;
                }
                else {
                  iVar3 = *piVar4;
                  uVar1 = *(uint *)(iVar3 + 0x10) & 0x7fffffff;
                }
                *(uint *)(iVar3 + 0x10) = uVar1;
                for (iVar3 = *(int *)(*piVar4 + 0x38); iVar3 != 0; iVar3 = *(int *)(iVar3 + 0x38)) {
                  if (value == 0) {
                    uVar1 = *(uint *)(iVar3 + 0x10) | 0x80000000;
                  }
                  else {
                    uVar1 = *(uint *)(iVar3 + 0x10) & 0x7fffffff;
                  }
                  *(uint *)(iVar3 + 0x10) = uVar1;
                }
                iVar2 = iVar2 + 1;
                piVar4 = piVar4 + 1;
              } while (iVar2 < psVar5[-1]);
            }
            iVar6 = iVar6 + 1;
             piVar4 = piVar4 + (SIZEOF_RENDERNODE - psVar5[-1]);
          } while (iVar6 < *psVar5);
        }
      }
      psVar5 = psVar5 + 6;
      iVar7 = iVar7 + 1;
      piVar8 = piVar8 + 3;
    } while (iVar7 < *(int *)(index + 0x88));
  }
}

int Chunk_GetFadeArea(int index)
{
  return index + 0x1cc;
}

void Chunk_SetFadeValue(int index,uint id)
{
  *(uint *)(index + 0x1dc) = id;
}

void Chunk_SetupFadeParams(int index,uint id,int value)
{
  void *puVar1;
  if ((int)id < 1) {
    puVar1 = &Rendering_ChunkB4;
    if (value != 0) {
      puVar1 = &Rendering_ChunkC0;
    }
  }
  else {
    puVar1 = &Rendering_Chunk9c;
    if (value != 0) {
      *(void **)(index + 0x1e4) = &Rendering_ChunkA8;
      goto LAB_8004cffc;
    }
  }
  *(void **)(index + 0x1e4) = puVar1;
LAB_8004cffc:
  if ((int)id < 0) {
    id = ~id + 1;
  }
  *(uint *)(index + 0x1e0) = (int)*(short *)(*(int *)(index + 0x1e4) + 6) * id;
}

void Chunk_ApplyFadeTick(int index)
{
  int iVar1;
  if (0 < *(int *)(index + 0x1e0)) {
    Chunk_ApplyToAll(index,ChunkData_ApplyFade,0);
    iVar1 = *(int *)(index + 0x1e0) + -1;
    *(int *)(index + 0x1e0) = iVar1;
    if (iVar1 == 0) {
      *(uint *)(index + 0x1e0) = 0xffffffff;
    }
  }
}

void Chunk_ResetFade(int index)
{
  if (*(int *)(index + 0x1e0) != 0) {
    Chunk_ApplyToAll(index,Chunk_RenderFadeScreen,0);
    *(uint *)(index + 0x1e0) = 0;
  }
}

void ChunkData_ApplyFade(int *index, int index_2)
{
  ((int (*)(int,int,int))(*(void **)(*index + 0x48)))(index,0,*(uint *)(index + 0x1e4));
}

void Chunk_RenderFadeScreen(uint *id, int *index)
{
  ((int (*)(int,int,int))(*(void **)(*id + 0x48)))(id,1,&Rendering_ChunkCc);
}

void Chunk_ApplyToAll(int *index, uint id, code *arg0)
{
  int iVar1;
  int iVar2;
  iVar2 = 0;
  iVar1 = SIZEOF_CHUNK_DATA;
  do {
    if (arg0 != (code *)0x0) {
      ((int (*)(...))(arg0))(index,index + iVar1);
    }
    Chunk_ApplyToAllEntries(index,id,index + iVar1);
    iVar2 = iVar2 + 1;
    iVar1 = iVar1 + 0x1c;
  } while (iVar2 < 7);
}

void Chunk_ApplyToAllEntries(uint id,code *arg0,int index)
{
  uint *puVar1;
  uint *puVar2;
  puVar1 = *(uint **)(index + 0x10);
  puVar2 = puVar1 + 0x19a;
  for (; puVar1 < puVar2; puVar1 = puVar1 + 1) {
    ((int (*)(...))(arg0))(id,*puVar1);
  }
}
void * Location_GetChunkVtable(void)
{
  return &Location_MapChunkVtable;
}

int ChunkData_Alloc(void)
{
  int iVar1;
  int iVar2;
  iVar1 = MemAllocImpl(0xdc);
  iVar2 = 0;
  if (iVar1 != 0) {
    iVar2 = Location_GetChunkDataVtable();
    ((int (*)(int))(*(void **)(iVar2 + 8)))(iVar1);
    iVar2 = iVar1;
  }
  return iVar2;
}

void ChunkData_Constructor(int *index)
{
  int iVar1;
  iVar1 = ChunkData_GetDataPtr();
  ((int (*)(int))(*(void **)(iVar1 + 8)))(index);
  iVar1 = Location_GetChunkDataVtable();
  *index = iVar1;
  ((int (*)(int))(*(void **)(iVar1 + 0x40)))(index);
}

void ChunkData_Nop(void)
{
}

void ChunkData_Render(int index)
{
  int iVar1;
  if ((*(int *)(index + 0x10) != 0) && (*(int *)(index + 0x70) != 0)) {
    iVar1 = ChunkData_GetDataPtr();
    ((int (*)(int))(*(void **)(iVar1 + 0x9c)))(index);
  }
}

void ChunkDataTable_Nop01(void)
{
}

void ChunkDataTable_Nop02(void)
{
}

void ChunkDataTable_Nop03(void)
{
}

void ChunkDataTable_Nop04(void)
{
}
void * Location_GetChunkDataVtable(void)
{
  return &Location_ChunkDataVtable;
}

int ChunkDataTable_Alloc(void)
{
  int iVar1;
  int iVar2;
  iVar1 = MemAllocImpl(0x3c);
  iVar2 = 0;
  if (iVar1 != 0) {
    iVar2 = Location_GetChunkDataTableVtable();
    ((int (*)(int))(*(void **)(iVar2 + 8)))(iVar1);
    iVar2 = iVar1;
  }
  return iVar2;
}

void ChunkDataTable_Constructor(uint *id)
{
  int iVar1;
  uint uVar2;
  iVar1 = GetPrimitiveBaseTable();
  ((int (*)(int))(*(void **)(iVar1 + 8)))(id);
  uVar2 = Location_GetChunkDataTableVtable();
  *id = uVar2;
  *(u16 *)(id + 0xd) = 0;
  *(u16 *)((int)id + 0x36) = 0;
  id[0xe] = 0;
}

void ChunkDataTable_Nop05(void)
{
}

void ChunkDataTable_CheckChar4(int *index,uint *id)
{
  if (*(char *)*id == '4') {
    ((int (*)(void))(*(void **)(*index + 0xb8)))();
  }
}

void ChunkDataTable_ProcessInput(int index, uint *id, int index_2)
{
  int iVar1;
  iVar1 = GetPrimitiveBaseTable();
  ((int (*)(int,int,int))(*(void **)(iVar1 + 0x9c)))(index,id,index);
  if ((index_2 < 9) && (4 < index)) {
    ((int (*)(int,int,int))(*(void **)(*id + 0xa0)))(id,id,index);
  }
}

uint ChunkDataTable_Identity(uint id)
{
  return id;
}
void * Location_GetChunkDataTableVtable(void)
{
  return &Location_ChunkDataTableVtable;
}

int LocationEntity_Alloc(uint id)
{
  int iVar1;
  int iVar2;
  iVar1 = MemAllocImpl(0xc4);
  iVar2 = 0;
  if (iVar1 != 0) {
    iVar2 = MemoryCard_GetVtable();
    ((int (*)(int,int))(*(void **)(iVar2 + 8)))(iVar1,id);
    iVar2 = iVar1;
  }
  return iVar2;
}

void LocationEntity_Constructor(int *index,int *value)
{
  int iVar1;
  uint uVar2;
  iVar1 = NavMenu_GetDataPtr();
  ((int (*)(int,int,int,int))(*(void **)(iVar1 + 8)))(index,&PTR_s_ETC_FONTICON_TIM_80086d44,"ETC\\ETCSE",0);
  iVar1 = MemoryCard_GetVtable();
  *index = iVar1;
  ((void (*)(int *,unsigned int))(*(int *)((EntityObj *)index)->nStateFlag + 0x9c))((int *)((EntityObj *)index)->nStateFlag,0xffffffff);
  ((EntityObj *)index)->field_0xa4 = (int)value;
  ((EntityObj *)index)->field_0xac = 0;
  iVar1 = ((int (*)(int,int))(*(void **)(*value + 0x1b0)))(value,index + 0x30);
  ((EntityObj *)index)->field_0xbc = iVar1;
  uVar2 = ((int (*)(int,int))(*(void **)(*value + 0x1a0)))(value,0);
  Location_SetupChallengeData(uVar2);
  ((int (*)(int,int))(*(void **)(*index + 0xd8)))(index,&PTR_s_ETC_FONTICON_TIM_80086d44);
  ((int (*)(int,int))(*(void **)(*index + 0x40)))(index,value);
  /* GameStateCheck reads obj[0x38] via NavMenu_GetField (vtable[0x44]) and
     freezes the boot when it returns 0 (main loop never re-runs the screen
     updater). The original's value came from heap garbage; set it explicitly. */
  ((EntityObj *)index)->nField_38 = 1;
}

void LocationEntity_SetDreamFlag(int index,int value)
{
  uint uVar1;
  uVar1 = 1;
  if (9999999 < *(int *)(((EntityObj *)index)->field_0xbc + 0xc)) {
    uVar1 = (uint)(*(int *)(((EntityObj *)index)->field_0xbc + 0x2f4) == 0);
  }
  *(uint *)(*(int *)(value + 0x18) + 4) = uVar1;
}

void Location_SetupChallengeData(uint id)
{
  Sjis_FormatNumber(&Location_NavChallengeBuf,id,3,0);
  Location_NavChallengeWord1 = Location_NavChallengeWord1 & 0xffff | 0x36370000;
  Location_NavChallengeWord2 = 0x32333435;
}
