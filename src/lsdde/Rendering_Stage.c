/* Rendering_Location.c (formerly Rendering_Stage.c) -- Location/Field rendering per wiki: https://dreamemulator.fandom.com/wiki/Locations -- split from Rendering.c */
#include "Rendering_Shared.h"
/* Extern declarations for private globals */
extern void *GetPrimitiveBaseTable(void);
extern char Rendering_StrBuf80011464[];
extern u32 Rendering_Unk11474;
extern u32 Rendering_Unk1149c;
extern u32 Rendering_Unk114b4;
extern u32 Rendering_Unk66841;
extern u32 Rendering_LocationGrid50;
extern u32 Rendering_LocationGrid5c;
extern u32 Rendering_Adj38;
extern u32 Rendering_Adj3c;
extern u32 Rendering_Adj40;
extern u32 Rendering_AdjDirAc;
extern u32 Rendering_AdjDirB0;
extern u32 Rendering_ChunkFc;
extern u32 Rendering_Chunk04;
extern u32 Rendering_Chunk74;
extern u32 Rendering_Chunk9c;
extern u32 Rendering_ChunkA8;
extern u32 Rendering_ChunkB4;
extern u32 Rendering_ChunkC0;
extern u32 Rendering_ChunkCc;
extern u32 Rendering_McAc;
extern u32 Rendering_Mc80;
extern u32 Rendering_McC4;
extern u32 Rendering_EntVtab7c;
extern u32 Rendering_Ent28;
extern u32 Rendering_Ent0c;
extern u32 Rendering_Ent18;
extern u32 Rendering_EntField50;
extern u32 Rendering_EntField5c;
extern u32 Rendering_EntField68;
extern u32 Rendering_PartByte1f;
extern u32 Rendering_Part24;
extern u32 Rendering_PartColEc;
extern u32 Rendering_PartColEf;
extern u32 Rendering_PartColF0;
extern u32 Rendering_SecEnt8c;
extern u32 Rendering_SecEntA4;
extern u32 Rendering_SecEntA6;
extern u32 Rendering_NavB4;
extern u32 Rendering_McState28;
extern u32 Rendering_McState2c;
extern u32 Rendering_McState30;
extern u32 Rendering_Mc94;
extern u32 Rendering_McPathC8;
extern u32 Rendering_McPathCc;
extern u32 Rendering_McPathD4;
extern u32 Rendering_McPathDc;
extern u32 Rendering_McPathF0;
extern u32 Rendering_McPathF8;
extern u32 Rendering_McPath0c;
extern u32 Rendering_McPath24;
extern u32 Rendering_McPath38;
extern u32 Rendering_McPath40;
extern u32 Rendering_PartSysA6;
extern u32 Rendering_PartSysA8;
extern u32 Rendering_PartSysAc;
extern u32 Rendering_PartSysB4;
extern u32 Rendering_PartSysBc;
extern u32 Rendering_PartList88;
extern u32 ParticleScratch_XPos;
extern u32 ParticleScratch_YPos;
extern u32 ParticleScratch_Move;
extern u32 ParticleScratch_ColPtr;
extern u32 ParticleScratch_EntPtr;
extern u32 ParticleScratch_Index;
extern u32 ParticleScratch_ColIndex;
extern u32 ParticleScratch_BufPtr;
extern u32 ParticleScratch_Flag;
extern u32 ParticleScratch_List;
extern u32 ParticleScratch_Alloc;
extern u32 Rendering_Spu8c;
extern u32 Rendering_Spu90;
extern u32 Rendering_Spu94;
extern u32 Rendering_Spu98;
extern u32 Rendering_Spu9c;
extern u32 ParticleColourTable;
extern u32 Unk_EntityBufC8;
extern u32 Unk_ParticleBuf1C;
extern u32 Unk_ParticleBuf28;
extern u32 ParticlePaletteAlt;
extern u32 ParticlePaletteDefault;
extern u32 ParticleModeIndexTable;
extern u32 MusicVolLineTargetLocation2;
extern u32 MusicVolLineTargetLocation3;
extern u32 SubType2StateArray;
extern u32 ParticleEffectParam2;
extern u32 ParticleEffectObjectType;
extern u32 Unk_SpuBuf154;
/* Ghidra compatibility typedefs */
typedef long ssize_t;
typedef long __off_t;
typedef unsigned long ulong;
/* Hardware register declarations */
extern volatile unsigned int Rendering_McSavePath;
extern volatile unsigned int PTR_Ptr_DreamParticle;
extern volatile unsigned int PTR_Ptr_SecondEntity;
extern volatile unsigned int PTR_PTR_LocationSecondaryEntityTablePtr;
extern volatile unsigned int PTR_s_ETC_FONTICON_TIM_80086d44;
extern volatile unsigned int DMA_CDROM_CHCR;
extern volatile unsigned int CD_VOL_R;
/* Forward declarations for functions defined later in this file */
void * GetLocationChunkVtable(void);
void * GetLocationGridVtable(void);
uint LocationGrid_GetVisMask(int id,int type,int flags);
uint LocationGrid_GetChunkEntry(int index,uint *type,int value,int arg2,int arg3,uint id,int arg5);
int LocationGrid_Alloc(uint id,uint type);
int LocationGrid_CountActiveChannels(int index);
void LocationGrid_DrawTiles(void);
extern int LocationGrid_New(void);
extern int LocationGrid_GetDataPtr(void);
extern int Prim_DrawTiles(int *ptr,int count,int arg2);
bool Prim_IsPointInRect(short *id,char *type);
void ChunkData_ApplyFade(int *index,int index_2);
void Chunk_RenderFadeScreen(uint *id,int *index);
void * Location_GetChunkVtable(void);
void * Location_GetChunkDataVtable(void);
void * Location_GetChunkDataTableVtable(void);
void LocationEntity_SetDreamFlag(int id,int type);
void * MemoryCard_GetVtable(void);
uint MemoryCard_DetectCard(int id,uint *type,uint *flags);
uint MemoryCard_LoadCardData(int id,uint *type,uint *flags);
uint MemoryCard_ReadFileTitle(int id,char *type,char *flags);
bool MemoryCard_DeleteFile(int id,uint type,int flags);
bool MemoryCard_ReadDataFile(int id,uint type,void *flags,size_t arg0);
uint MemoryCard_WriteFileSaveData(int index,uint id,char *flags,byte val,int value,void *arg0,int arg4);
char * MemoryCard_BuildPath(char *id,int type,char *flags);
uint MemoryCard_PollEvents(uint *id,int type);
uint NavMenu_CardCheckState(int *id);
void * MemoryCardLocation_GetVtable(void);
uint todigit(char id);
void * Entity2_GetVtable(void);
uchar * TextInput_FormatLine(int index,uchar *arg0,int *value,int arg2,int arg3);
void * GetTextInputVtable(void);
void Font_ProcessChar(int index,int value,uint id,uint type);
void * GetVramTextureVtable(void);
uint VramTexture_DrawFunc(int index,uint id,uint type,uint flags,uint arg4);
void ** VramTexture_SetupDraw(void);
void * GetVramTexture2Vtable(void);
uint VramParticle_CreateType0(uint *id,int type,uint flags);
uint VramParticle_CreateType1(uint *id,int type,uint flags);
uint VramParticle_CreateType2(uint *id,uint type);
uint VramParticle_CreateType3(uint *id,uint type);
void RenderUtils_CallDraw(uint id,int type);
void RenderUtils_ApplyPpu(void);
int * RenderUtils_InitSubEntity(int *id,int *type);
void * RenderUtils_CreateSub(int *id,int *type,int *flags);
uint RenderUtils_ClipSub(int *id);
bool RenderUtils_CheckSeed(int id,u8 *type,uint flags);
uint RenderUtils_GetSub(int *id,int *type);
void RenderUtils_CallDisplay(uint id,int type);
uint RenderUtils_CreateDisplay(int id,uint type,uint flags);
void * GetRenderStateVtable(void);
void * GetGameManagerAlias(void);
void * GetNngVtable(void);
uint RenderUtils_GetDisplay(int *id);
uint LocationArea_NumToPos(int index,int value);
void * GetLocationAreaVtable(void);
uint Entity_SoundInit(int *id,int type);
uint Entity_ParticleInit(int id);
extern char LAB_80087204[];
int EntityAlloc50(uint id,uint type,uint flags)
{
  int iVar1;
  int iVar2;
  iVar1 = MemAllocImpl(SIZEOF_RENDER_CTX);
  iVar2 = 0;
  if (iVar1 != 0) {
    iVar2 = GetLocationChunkVtable();
    ((int (*)(int,int,int,int))(*(void **)(iVar2 + 8)))(iVar1,id,type,flags);
    iVar2 = iVar1;
  }
  return iVar2;
}

void Location_Init(int *index,int index2,int *value,int value2)
{
  uint uVar1;
  int iVar2;
  int *piVar3;
  uint local_28;
  char *local_24;
  uVar1 = Snd_GetCurrentAmbient(0);
  iVar2 = GetLocationGridVtable();
  ((int (*)(int,int,int))(*(void **)(iVar2 + 8)))(index,uVar1,0);
  iVar2 = GetLocationChunkVtable();
  *index = iVar2;
  SpuCinema_Init();
  piVar3 = (int *)Texture_New("ETC\\ETC.TIM");
  ((RenderCtx *)index)->pTexture = (int)piVar3;
  ((int (*)(int))(*(void **)(*piVar3 + 0x78)))(piVar3);
  ((int (*)(void))(*(void **)(*(int *)((RenderCtx *)index)->pTexture + 0x5c)))();
  local_28 = 0;
  local_24 = "ETC\\DREAMER.TMD";
  iVar2 = Tmd_New(&local_28);
((RenderCtx *)index)->pTmdModel = (void *)iVar2;
  uVar1 = Snd_SelectAmbient(0);
  iVar2 = LocationChunk_New(uVar1,0,1);
((RenderCtx *)index)->pLocationChunk = (void *)iVar2;
  Cd_SplitStreamRead(1);
  CdModeSubE(value == 0,1,1);
  ((RenderCtx *)index)->pLocationGridAlloc = index;
  uVar1 = ChunkData_Alloc();
((RenderCtx *)index)->pChunkData = (void *)uVar1;
  uVar1 = LocationGrid_New();
((RenderCtx *)index)->pLocationGrid = (void *)uVar1;
  uVar1 = LocationGrid_Alloc(0,1);
((RenderCtx *)index)->pLocationGridAlloc = (void *)uVar1;
  ((RenderCtx *)index)->pValueParam2 = (int)value;
  ((int (*)(int,int))(*(void **)(*index + 0x10)))(index,value);
  ((int (*)(int,int))(*(void **)(*value + 0x10c)))(value,((RenderCtx *)index)->pValueParam);
  ((int (*)(int,int))(*(void **)(*value + 0x114)))(value,((RenderCtx *)index)->pTexture);
  ((int (*)(int))(*(void **)(*index + 0x40)))(index);
}

void Location_Destroy(int *index)
{
  uint uVar1;
  int iVar2;
  iVar2 = ((RenderCtx *)index)->pLocationGridAlloc;
  ((int (*)(int,int))(*(void **)(*index + 0x14)))(index,((RenderCtx *)index)->pValueParam2);
  uVar1 = ((int (*)(void))(*(void **)(**(int **)(iVar2 + 0xc) + 4)))();
  *(uint *)(iVar2 + 0xc) = uVar1;
  uVar1 = ((int (*)(void))(*(void **)(**(int **)(iVar2 + 8) + 4)))();
  *(uint *)(iVar2 + 8) = uVar1;
  uVar1 = ((int (*)(void))(*(void **)(**(int **)(iVar2 + 0x10) + 4)))();
  *(uint *)(iVar2 + 0x10) = uVar1;
  ((int (*)(void))(*(void **)(*(int *)((RenderCtx *)index)->pLocationChunk + 4)))();
  ((int (*)(void))(*(void **)(*(int *)((RenderCtx *)index)->pTmdModel + 4)))();
  ((int (*)(void))(*(void **)(*(int *)((RenderCtx *)index)->pTexture + 4)))();
  SpuCinema_Free();
  iVar2 = GetLocationGridVtable();
  ((int (*)(int))(*(void **)(iVar2 + 0xc)))(index);
}

void Location_HandleMessage(int *index,uint *id,uint idVal)
{
  int iVar1;
  code *pcVar2;
  iVar1 = GetLocationGridVtable();
  ((int (*)(int,int,int))(*(void **)(iVar1 + 0x38)))(index,id,idVal);
  if ((*(uint *)*id & 0xffff) == 0x1f34) {
    pcVar2 = *(code **)(*index + 0x80);
  }
  else {
    if ((*(uint *)*id & 0xfffff) != 0x2f230) {
      return;
    }
    pcVar2 = *(code **)(*index + 0x84);
  }
  ((int (*)(...))(pcVar2))(index,id,idVal);
}

void Location_ResetState(int index)
{
  ((RenderCtx *)index)->dwModeFlags = 0;
}

void Location_Update(int index)
{
  int iVar1;
  int *piVar2;
  piVar2 = ((RenderCtx *)index)->pValueParam2;
  ((int (*)(int,int))(*(void **)(*piVar2 + 0x10)))(piVar2,*(uint *)(((RenderCtx *)index)->pLocationGridAlloc + 4));
  ((int (*)(int,int))(*(void **)(*piVar2 + 0x10)))(piVar2,*(uint *)(((RenderCtx *)index)->pLocationGridAlloc + 8));
  ((int (*)(int,int))(*(void **)(*piVar2 + 0x110)))(piVar2,*(uint *)(((RenderCtx *)index)->pLocationGridAlloc + 0x10));
  iVar1 = GetLocationGridVtable();
  ((int (*)(int,int,int))(*(void **)(iVar1 + 0x44)))(index,((RenderCtx *)index)->pLocationGridAlloc,0);
}

void Location_Draw(int index)
{
  int iVar1;
  int *piVar2;
  piVar2 = ((RenderCtx *)index)->pValueParam2;
  iVar1 = GetLocationGridVtable();
  ((int (*)(int))(*(void **)(iVar1 + 0x48)))(index);
  ((int (*)(int,int))(*(void **)(*piVar2 + 0x110)))(piVar2,0);
  ((int (*)(int,int))(*(void **)(*piVar2 + 0x14)))(piVar2,*(uint *)(((RenderCtx *)index)->pLocationGridAlloc + 4));
  ((int (*)(int,int))(*(void **)(*piVar2 + 0x14)))(piVar2,((RenderCtx *)index)->pChunkData);
}

void Location_SetupRender(int index)
{
  uint uVar1;
  int *piVar2;
  int *piVar3;
  piVar3 = ((RenderCtx *)index)->pField_18;
  uVar1 = ((int (*)(int,int))(*(void **)(*(int *)*(uint **)((RenderCtx *)index)->pLocationGridAlloc + 0x7c)))((int *)*(uint **)((RenderCtx *)index)->pLocationGridAlloc,0);
  ((int (*)(int,int))(*(void **)(*piVar3 + 0x44)))(piVar3,uVar1);
  piVar2 = (int *)((int (*)(int))(*(void **)(*piVar3 + 0xac)))(piVar3);
  ((int (*)(int,int))(*(void **)(*piVar2 + 0x60)))(piVar2,1);
  ((int (*)(int,int))(*(void **)(*piVar3 + 0x4c)))(piVar3,OT_DEPTH_DEFAULT);
  ((int (*)(int,int,int,int,int))(*(void **)(*piVar3 + 0x70)))(piVar3,((RenderCtx *)index)->pValueParam2,&Rendering_LocationGrid50,&Rendering_LocationGrid5c,0);
  ((int (*)(int))(*(void **)(*piVar3 + 0x8c)))(piVar3);
  ((RenderCtx *)index)->dwModeFlags = 1;
}

void Location_Release(int index)
{
  int *piVar1;
  piVar1 = ((RenderCtx *)index)->pField_18;
  ((int (*)(int))(*(void **)(*piVar1 + 0x90)))(piVar1);
  ((int (*)(int))(*(void **)(*piVar1 + 0x74)))(piVar1);
}

void Location_HandleState(int *index, uint id, int index_2)
{
  int iVar1;
  iVar1 = GetLocationGridVtable();
  ((int (*)(int,int,int))(*(void **)(iVar1 + 0x54)))(index,id,index_2);
  if (index_2 == 2) {
    iVar1 = ((RenderCtx *)index)->dwModeFlags;
    if (iVar1 != 2) {
      if (iVar1 < 3) {
        if (iVar1 != 1) {
          return;
        }
        iVar1 = ((int (*)(void))(*(void **)(*(int *)((RenderCtx *)index)->pValueParam2 + 0x1b4)))();
        if (iVar1 < 0) {
          ((int (*)(int,int))(*(void **)(*(int *)((RenderCtx *)index)->pValueParam2 + 0x1b8)))((int *)((RenderCtx *)index)->pValueParam2,0);
          ((RenderCtx *)index)->nField_28 = 2;
          ((int (*)(int,int))(*(void **)(*index + 0x60)))(index,3);
          return;
        }
      }
      else {
        if (iVar1 != 3) {
          return;
        }
        ((int (*)(void))(*(void **)(*(int *)((RenderCtx *)index)->nField_4c + 0x48)))();
        ((int (*)(void))(*(void **)(*(int *)((RenderCtx *)index)->nField_4c + 4)))();
        iVar1 = ((int (*)(void))(*(void **)(*(int *)((RenderCtx *)index)->pValueParam2 + 0x1e0)))();
      }
      Location_CreateToken(index,iVar1);
    }
  }
}

void Location_CreateToken(int *index,uint id)
{
  int iVar1;
  iVar1 = TextInput_CreateToken(((RenderCtx *)index)->pValueParam,((RenderCtx *)index)->pLocationChunk,((RenderCtx *)index)->pTexture,((RenderCtx *)index)->pTmdModel,id);
  ((RenderCtx *)index)->nField_4c = iVar1;
  ((int (*)(int,int))(*(void **)(*index + 0x10)))(index,iVar1);
  ((int (*)(int,int,int))(*(void **)(*(int *)((RenderCtx *)index)->nField_4c + 0x44)))((int *)((RenderCtx *)index)->nField_4c,((RenderCtx *)index)->pLocationGridAlloc,((RenderCtx *)index)->pValueParam2);
  ((RenderCtx *)index)->dwModeFlags = 2;
}

void Location_Nop(void)
{
}

void Location_Nop2(void)
{
}

void Location_HandleTransition(int *index, uint id, int index_2)
{
  int iVar1;
  code *pcVar2;
  uint uVar3;
  u8 auStack_18 [2];
  short local_16;
  switch(index_2) {
  case 4:
    ((int (*)(void))(*(void **)(*(int *)((RenderCtx *)index)->nField_4c + 0x48)))();
    ((int (*)(void))(*(void **)(*(int *)((RenderCtx *)index)->nField_4c + 4)))();
    iVar1 = ((int (*)(int,int))(*(void **)(*(int *)((RenderCtx *)index)->pValueParam2 + 0x1b8)))((int *)((RenderCtx *)index)->pValueParam2,0);
    if (iVar1 == 0) {
      ((int (*)(int))(*(void **)(*(int *)((RenderCtx *)index)->pValueParam2 + 0x1bc)))(auStack_18);
      iVar1 = 2;
      if (local_16 < 0) {
        iVar1 = 1;
      }
      ((RenderCtx *)index)->nField_28 = iVar1;
    }
    else {
      ((RenderCtx *)index)->nField_28 = 3;
    }
    pcVar2 = *(code **)(*index + 0x60);
    break;
  case 5:
  case 6:
  case 7:
  case 8:
  case 10:
    ((RenderCtx *)index)->dwModeFlags = 3;
    return;
  default:
    goto switchD_80049ef0_caseD_9;
  case 0xc:
  case 0xd:
    ((int (*)(void))(*(void **)(*(int *)((RenderCtx *)index)->nField_4c + 0x48)))();
    ((int (*)(void))(*(void **)(*(int *)((RenderCtx *)index)->nField_4c + 4)))();
    uVar3 = 1;
    if (index_2 != 0xc) {
      uVar3 = 2;
    }
    ((int (*)(int,int))(*(void **)(*(int *)((RenderCtx *)index)->pValueParam2 + 0x1b8)))((int *)((RenderCtx *)index)->pValueParam2,uVar3);
    ((RenderCtx *)index)->nField_28 = 3;
    pcVar2 = *(code **)(*index + 0x60);
  }
  ((int (*)(...))(pcVar2))(index,3);
switchD_80049ef0_caseD_9:
}
void * GetLocationChunkVtable(void)
{
  return &Location_ChunkVtable;
}

void Cd_SplitStreamRead(int index)
{
  uint uVar1;
  int iVar2;
  int local_18 [2];
  uVar1 = Snd_GetAmbientPath(local_18);
  iVar2 = Location_McSplitIoState + 1;
  if (iVar2 == 1) {
    Location_McSplitIoState = Location_McSplitIoState + 2;
    if (index == 0) {
      Location_McSplitHalfSize = local_18[0] / 2;
      Location_McSplitIoState = iVar2;
      local_18[0] = Location_McSplitHalfSize;
    }
  }
  else if (iVar2 == 2) {
    local_18[0] = local_18[0] - Location_McSplitHalfSize;
    Location_McSplitIoState = iVar2;
  }
  else {
    local_18[0] = 0;
    Location_McSplitIoState = iVar2;
  }
  do {
    iVar2 = CdModeSubF(uVar1,local_18[0]);
  } while (iVar2 == 0);
}

int LocationSub_Alloc(uint id,uint type)
{
  int iVar1;
  int iVar2;
  iVar1 = MemAllocImpl(SIZEOF_RENDER_SUB);
  iVar2 = 0;
  if (iVar1 != 0) {
    iVar2 = GetLocationGridVtable();
    ((int (*)(int,int,int))(*(void **)(iVar2 + 8)))(iVar1,id,type);
    iVar2 = iVar1;
  }
  return iVar2;
}

void LocationSub_Init(int *index, int index_2, int value)
{
  int iVar1;
  iVar1 = Font_GetDataPtr();
  ((int (*)(int))(*(void **)(iVar1 + 8)))(index);
  iVar1 = GetLocationGridVtable();
  *index = iVar1;
  if (index_2 == 0) {
    ((EntityObj *)index)->nField_34 = value;
  }
  else {
    iVar1 = EntityAllocMedium(index);
    ((EntityObj *)index)->nField_34 = iVar1;
  }
  ((EntityObj *)index)->nField_30 = index;
  ((int (*)(int))(*(void **)(*index + 0x40)))(index);
}

void LocationSub_Destroy(int index)
{
  int iVar1;
  if (((EntityObj *)index)->nField_30 != 0) {
    ((int (*)(void))(*(void **)(((EntityObj *)index)->nField_34 + 4)))();
  }
  iVar1 = Font_GetDataPtr();
  ((int (*)(int))(*(void **)(iVar1 + 0xc)))(index);
}

void LocationSub_Reset(int *index)
{
  ((int (*)(int,int))(*(void **)(*index + 0x6c)))(index,0xffffffff);
}

uint LocationSub_Process(int index,uint id,uint type)
{
  int iVar1;
  ((EntityObj *)index)->nConfig = 0;
  iVar1 = Font_GetDataPtr();
  ((int (*)(int,int,int))(*(void **)(iVar1 + 0x44)))(index,id,type);
  return ((EntityObj *)index)->nConfig;
}

void LocationSub_Run(uint id)
{
  int iVar1;
  iVar1 = Font_GetDataPtr();
  ((int (*)(int))(*(void **)(iVar1 + 0x48)))(id);
}

void LocationSub_Nop(void)
{
}

void LocationSub_CheckBuffer(int *index,uint id,uint type)
{
  int iVar1;
  iVar1 = Font_GetDataPtr();
  ((int (*)(int,int,int))(*(void **)(iVar1 + 0x5c)))(index,id,type);
  if ((uint)((EntityObj *)index)->nVabIdx < (uint)((EntityObj *)index)->nField_1c) {
    ((int (*)(int,int))(*(void **)(*index + 0x60)))(index,4);
  }
}

void LocationSub_HandleEvent(int *index, int index_2)
{
  int iVar1;
  iVar1 = Font_GetDataPtr();
  ((int (*)(int,int))(*(void **)(iVar1 + 0x60)))(index,index);
  if (index_2 == 4) {
    ((EntityObj *)index)->nConfig = 1;
    ((int (*)(int))(*(void **)(*index + 0x7c)))(index);
  }
}

void LocationSub_SetMaxCount(int index,int value)
{
  ((EntityObj *)index)->nVabIdx = value;
  if (-1 < value) {
    ((EntityObj *)index)->nVabIdx = value * SIZEOF_RENDERNODE;
  }
}

void LocationSub_SetVolume(int index,uint id)
{
  int *piVar1;
  piVar1 = ((EntityObj *)index)->nField_34;
  if (piVar1 != NULL) {
    ((int (*)(int,int,int,int))(*(void **)(*piVar1 + 0x80)))(piVar1,id,SOUND_VOLUME_DEFAULT,SOUND_VOLUME_DEFAULT);
  }
}
void * GetLocationGridVtable(void)
{
  return &Location_GridVtable;
}
