/* Entity3_Shared.h -- shared extern declarations for Entity3_* split files */
#ifndef ENTITY3_SHARED_H
#define ENTITY3_SHARED_H

#include "common.h"
#include "dat_globals.h"
#include "lsdde/structs.h"


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
extern int CdSync(int mode, unsigned char *result);
extern int NewStageArea(int a0);
extern int StageEntity_Alloc(int a0);
extern int func_8003B20C(void);
extern int func_80048CF0(void);
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

#endif /* ENTITY3_SHARED_H */
