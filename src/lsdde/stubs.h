#ifndef STUBS_H
#define STUBS_H

#include <string.h>
#include <stdio.h>
#include <libgte.h>
#include <libgpu.h>
#include <libgs.h>
#include <libapi.h>
#include <libcd.h>
#include <libspu.h>
#include <libmath.h>
#include <libcard.h>

extern void *GetGameManager(void);
extern void trap(unsigned int code);
extern unsigned int strlen_imp(const char *s);
extern int IsTileActive(void);
extern void Entity_VisibleRender(void *entity);
extern void SetObjectTransform(void *obj, void *xfrm);
extern void RemoveEntityFromLayer(void *e, int layer);
extern void RegisterEntityOnLayer(void *a, void *b, int c, void *d, int e);
extern void UpdateEntityLayer(void *a, void *b);
extern void Spawn_DreamParticles_Grid(void);
extern void *FindNearbySecondaryEntity(void);
extern void *GetCdObj(void);
extern int GetVideoMode(void);
extern unsigned int GetSoundSize(void *snd);
extern void *ExecSoundAction(void *a, void *b, void *c, void *d);
extern void *GetStageDataTable(int *outSize);
extern void ProcessEntityCallback(void *entity, void *cb, int arg3);
extern void *GetActiveSoundObject(int a, int b, int c, int d);
extern int GetRandomValue(void);
extern int NewSoundEntity(char actionType, void *entity, void *params, int param_val);
extern void *GetEntityVtable2(void);
extern int BuildVisibleChunkListSingle(void *chunk);
extern int ENTITY_FindByFlags(int a, int b, int c, int d);
extern void UpdateChunkBoundaries(int *grid);
extern int CreateRenderObject(int chunkCol, int tileCol, int tileRow, int pos, int arg5);
extern void *GetVtable_ObjectType21(void);
extern void *GetVtable_ObjectType23(void);

extern void gte_ReadRotMatrix(int *m);
extern void gte_SetRotMatrix(int *m);
extern void gte_ldv0(int *v);
extern void gte_ldv3(int *v);
extern void gte_stsxy(int *v);
extern void gte_stsxy3(int *v);
extern void gte_rtps(int *v);
extern void gte_rtpt(int *v);
extern void gte_rtir(int *v);
extern void gte_ncds(int *v);
extern void gte_nclip(int *v);
extern void gte_avsz3(int *v);
extern void gte_dpcs(int *v);
extern void gte_dpct(int *v);
extern void gte_ldclmv(int *v);
extern void gte_ldrgb(int *v);
extern void gte_ldrgb3(int *v);
extern void gte_ldrgb3c(int *v);
extern void gte_strgb(int *v);
extern void gte_strgb3(int *v);
extern void gte_strgb3_g3(int *v);
extern void gte_stclmv(int *v);
extern void gte_stdp(int *v);
extern void gte_stflg_4(int *v);
extern void gte_stopz(int *v);
extern void gte_stotz(int *v);
extern void gte_stsz3(int *v);
extern void gte_stsz4(int *v);
extern void *MemAlloc(int size);
extern void MemFree(void *p);
extern void *MemSet(void *p, int val, int len);
extern void *PsyqSpuMalloc(int size);
extern int CardWriteRead(int base, int size);
extern void *CdDaMode_GetVtable(void);
extern void *CdDaMode_Op(void *mode, int op);
extern int CdDaMode_CheckWait(int mode);
extern int CdDaMode_WaitReady(int file_idx, int param_2, int mode);
extern int CdDaMode_PollVsync(void);
extern int CdDaMode_CheckComplete(void);
extern void CdDaMode_GetVtable2(void);
extern void *CdReadMode_GetVtable(void);
extern void *CdReadMode_Op(void *mode, int op);
extern int CdReadMode_WaitReady(int param_1, int param_2);
extern int CdReadComplete(void);
extern int _card_info(void *info);
extern int _card_load(long chan);
extern int _card_clear(void);
extern int _SpuInit(int mode);
extern void _SpuDataCallback(void);
extern int SpuParseSeqData(int channel, int soundBankId, void *seqData);
extern void *SpuGetVtable_ObjectC(void);
extern void *SpuGetVtable_SeqLineDraw(void);
extern void *SpuGetVtable_SeqPlayer(void);
extern void Sound_InitHot(void);
extern void *LookupSoundType(int type);
extern void *LookupSoundResult(void);
extern int ParseSoundFormat(int *idx_buf, int fmt_val);
extern unsigned char BSPClassifyPoint(short *box, short *point);
extern void RandomRange(unsigned int *buf, int min, int max);
extern void FormatIntToString(char *dst, int val, int pad, int mode);

/* Renamed functions from Ghidra decompilation */
extern void  MathTransformByMatrix(int *obj, int *out, void *param3);
extern void  MathReadScaledShorts(int obj, short *out);
extern int   MathIsPointInBox(int *box, int margin, int *pt);
extern void  MathTransformMultipleVectors(VECTOR *dst, VECTOR *src, int count, MATRIX *mat);
extern void  MathComputeAngleToTarget(int *entity, int target, int zeroHeading, int skipAngleOffset, int animParam);
extern void *GetPrimitiveBaseTable(void);
extern unsigned int MathBitfieldOp(unsigned int *reg, unsigned int bitOffset, int fieldWidth, int newVal);

/* Real implementations from Ghidra decompilation */
extern int Sound_AdvanceDataPtrEx(int bank, int voiceIndex);

#endif /* STUBS_H */
