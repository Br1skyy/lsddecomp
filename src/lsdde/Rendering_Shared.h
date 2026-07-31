/* Rendering_Shared.h -- shared extern declarations for all Rendering_* split files */
#ifndef RENDERING_SHARED_H
#define RENDERING_SHARED_H

#include "common.h"
#include "dat_globals.h"
#include "libcd.h"
#include "libgte.h"
#include "libgpu.h"
#include "libgs.h"
#include "libapi.h"
#include "libspu.h"
#include "libsnd.h"
#include "lsdde/structs.h"




































/* Extern declarations for private globals */



extern void *GetPrimitiveBaseTable(void);



extern char Rendering_StrBuf80011464[];



extern u32 Rendering_Unk11474;



extern u32 Rendering_Unk1149c;







extern u32 Rendering_Unk114b4;



extern u32 Rendering_Unk66841;



extern u32 Rendering_StageGrid50;



extern u32 Rendering_StageGrid5c;



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



extern u32 MusicVolLineTargetStage2;



extern u32 MusicVolLineTargetStage3;



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



extern volatile unsigned int PTR_PTR_StageSecondaryEntityTablePtr;



extern volatile unsigned int PTR_s_ETC_FONTICON_TIM_80086d44;



extern volatile unsigned int DMA_CDROM_CHCR;



extern volatile unsigned int CD_VOL_R;







/* Forward declarations for functions defined later in this file */



void * GetStageChunkVtable(void);



void * GetStageGridVtable(void);



uint StageGrid_GetVisMask(int id,int type,int flags);



uint StageGrid_GetChunkEntry(int index,uint *type,int value,int arg2,int arg3,uint id,int arg5);
int StageGrid_Alloc(uint id,uint type);
int StageGrid_CountActiveChannels(int index);
void StageGrid_DrawTiles(void);
extern int StageGrid_New(void);
extern int StageGrid_GetDataPtr(void);
extern int Prim_DrawTiles(int *ptr,int count,int arg2);

bool Prim_IsPointInRect(short *id,char *type);



void ChunkData_ApplyFade(int *index,int index_2);



void Chunk_RenderFadeScreen(uint *id,int *index);



void * Stage_GetChunkVtable(void);



void * Stage_GetChunkDataVtable(void);



void * Stage_GetChunkDataTableVtable(void);



void StageEntity_SetDreamFlag(int id,int type);



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



void * MemoryCardStage_GetVtable(void);



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



uint StageArea_NumToPos(int index,int value);



void * GetStageAreaVtable(void);



uint Entity_SoundInit(int *id,int type);



uint Entity_ParticleInit(int id);



extern char LAB_80087204[];













#endif /* RENDERING_SHARED_H */
