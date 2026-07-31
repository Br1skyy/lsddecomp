/* 2C054_Shared.h -- shared declarations for 2C054 split files */
#ifndef INC_2C054_SHARED_H
#define INC_2C054_SHARED_H

#include "common.h"
#include "dat_globals.h"
#include "libetc.h"
#include <stddef.h>
#include "libgte.h"
#include "libgpu.h"
#include "libgs.h"
#include "libmath.h"
#include "libcd.h"
#include "libspu.h"
#include "libsnd.h"


extern u32 UNK_8006ea90;

extern int  MemAllocImpl ();
extern int  MemFreeImpl ();
extern int  GetCoordSystemVtable ();
extern int  ArrayInitByVtable ();
extern int  SceneNodeAlloc ();
extern int  GetPrimitiveBaseTable ();
extern int  MathBitfieldOp ();
extern int  EntityAllocMedium ();
extern int  Texture_New ();
extern int  TextInput_NewLabel ();
extern int  Label_GetVtable ();
extern int  StageGrid_New ();
extern int  StageRow_New ();
extern int  NavMenu_NewPage ();
extern int  NavMenu_NewList ();
extern int  NavMenu_NewMenu ();
extern int  Spu_NewSeqTrack ();

void  Spu_SetSeqTrackState (int *obj,int arg2);
void  SpuVoice_SetUnk44 (int obj,int val);
void  Entity_SetDuration (int obj,int val);
int  EntityAllocLarge (int obj,int arg2,int arg3);
void  Entity_StateUpdate (int *obj,int arg2);
void  Entity_SetLoopMask (int obj,uint arg2);
void  Palette_SetParams (int obj,int val,int arg3);
int * Palette_GetBaseTable (void);
void  Font_Start (int obj);
void  RenderView_SetScale (int obj,short *arg2);
void  RenderView_SetFlags (int obj,int val);
int * Primitive_GetVtable (void);
void  Spu_SetSeqTrackVolume (int obj,int val);
void  Spu_MuteSeqTrack (int obj);
void  Entity_Complete (int *obj,int arg2,int arg3,int arg4);
int  RenderView_GetTailNode (int obj);
void  Entity_SetFlag (int obj,int arg2);
int  NavMenu_Notify (int *obj);
void  Spu_Nop2 (void);
void  NavMenu_SetAllPositions (int obj,int arg2);
void  NavMenu_InitContent (int *obj);
void  NavMenu_Confirm (int *obj);
void  NavMenu_LoadTexture (int obj,int val,int arg3);
void  Palette_SaveAndSet (int obj,u16 *val,int *arg3);
void  Spu_UnmuteSeqTrack (int obj);
void  RenderView_HandleMode (int *obj,int arg2,int arg3);
void  TextInput_DrawPositioned (int obj,int *arg2);
void  RenderView_SetTranslation (int obj,int *arg2);
void  NavMenu_DrawItems (int obj,int arg2,int arg3);
void  Spu_SetSeqTrackPosition (int obj,int val,int arg3,int arg4, int arg5);
void  RenderView_Deactivate (int *obj);
void  ChunkDataObj_SetChild (int obj,int *val);
void  NavMenu_CancelAction (int *obj);
void  NavMenu_SetItemSpacing (int obj,int val);
void  NavMenu_Callback (int *obj);
void  Entity_ConstructFromTemplate (int *obj,int arg2,int arg3,int arg4);
void  NavMenu_PageNext (int *obj);
void  ChunkDataObj_Dispatch (int *obj,int *arg2,int arg3);
void  NavMenu_Prev (int *obj);
void  NavMenu_UpdateItem (int *obj,int arg2,int arg3);
int  NavMenu_GetField (int obj,int arg2,int arg3);
void  NavMenu_InitPage (int *obj,int arg2,int arg3,int arg4);
void  TextInput_Return (void);
int  TextInput_New (int obj,int arg2,int arg3);
void  NavMenu_SelectNext (int *obj);
void  NavMenu_DelPage (int *obj);
void  Entity_SetBlendMode (int obj,int arg2);
void  SpuVoice_SetColor (int obj,u8 *arg2);
void  Spu_StopSeqTrack (int *obj);
void  RenderView_FlipBuffers (int obj);
void  NavMenu_SetOption (int *obj,int arg2);
int  TextInput_Update (int obj,int arg2,int arg3);
void  Font_ResetState (int obj);
int  RenderView_GetBackground (int obj);
void  Spu_Nop (void);
int  NavMenu_GetSelection (int obj);
void  NavMenu_SetOption2 (int *obj,int arg2);
void  Font_EventProc (int obj,int arg2,int arg3);
void  Font_Init (int *obj,int *arg2,int arg3);
void  NavMenu_PagePrev (int *obj);
void  RenderView_StubNop (void);
void  TextInput_SetSpacing (int obj,int val);
int  Spu_NewSeqTrack (int obj,int arg2,int arg3,int arg4);
void  TextInput_SetChar (int obj,u8 arg2,int arg3);
void  ChunkDataObj_RemoveChild (int obj,int *arg2);
int * TextInput_GetVtable (void);
void  SpuVoice_SetUnk40 (int obj,int val);
void  NavMenu_SetColor (int obj,u8 *arg2,u8 *arg3,u8 *arg4);
int  NavMenu_Check (int *obj);
void  Font_Construct (int *obj);
void  TextInput_Draw (int obj,int arg2,int *arg3);
void  RenderView_SetRotation (int obj,int *arg2);
void  NavMenu_Update (int obj);
void  Entity_SetFrame (int obj,int val);
void  NavMenu_SetPosition (int *obj,int arg2);
void  Entity_ApplyColor (int obj,int arg2,int arg3);
void  Spu_SetSeqTrackPitch (int obj,int val);
void  TextInput_Activate (int *obj,int arg2);
void  Spu_UpdateSeqTrack (int *obj);
char * Sjis_EncodeFromAscii (u8 *obj,byte *arg2);
void  TextInput_PrintString (int obj,char *arg2);
void  NavMenu_SetBounds (int obj,int val,int arg3);
int * Spu_GetSeqTrackVtable (void);
void  Color_AddOrSet (int obj,char *arg2,char *arg3,int arg4);
char *  Sjis_Decode (char *obj,char *arg2);
int * Font_GetDataPtr (void);
void  RenderView_SetDrawMode (int obj,int val);
void  Entity_SetPosition (int obj,u16 *val);
void  ChunkDataObj_Reset (int obj);
void  Entity_StateEnter (int *obj);
void  Sjis_FormatNumber (int obj,int arg2,int arg3,int arg4);
void  NavMenu_SetItemState (int *obj,int arg2);
void  NavMenu_Destroy (int *obj);
int  Particle_New (int obj,int arg2,int arg3);
void  RenderView_SetBackground (int obj,int *val);
void  RenderView_InitProjection (int *obj,int val,int arg3,int arg4, int *arg5);
void  Font_Destroy (int *obj);
void  ChunkDataObj_Construct (int *obj);
void  NavMenu_ChangePage (int *obj);
int * ChunkData_GetDataPtr (void);
void  NavMenu_AddItems (int obj,int arg2,int arg3);
void  Font_Tick (int obj);
void  SpuVoice_InitDefaults (int obj);
void  ChunkDataObj_Destroy (int *obj);
void  Font_Stop (int obj);
int * TypeEntry1_GetDataPtr (void);
void  NavMenu_ConfirmState (int *obj);
void  TextInput_Nop (void);
void  Spu_ResetSeqTrack (int obj);
void  SpuVoice_SetColor2 (int obj,u8 *arg2);
void  NavMenu_CancelState (int *obj);
int  NavMenu_NewPage (int obj,int arg2,int arg3);
void  NavMenu_Draw (int *obj,int arg2);
void  Spu_SetSeqTrackPitchMod (int obj,int val);
int  NavMenu_FadeCheck (int *obj);
void  RenderView_StubNop2 (void);
void  Entity_ProcessState (int *obj,int arg2,int arg3);
void  NavMenu_FreeItems (int obj);
void  NavMenu_Resume (int *obj);
void  Spu_DelSeqTrack (int obj);
void  TextInput_Destroy (int obj);
void  Spu_InitSeqTrack (int *obj,int val,int arg3,int arg4,int *arg5 );
void  TextInput_InitLabels (int *obj,int arg2,int arg3,int arg4);
void  SpuVoice_SetUnk54 (int obj,int val);
void  SpuVoice_SetDepth (int obj,int val);
void  TextInput_SetPosition (int obj,int arg2);
void  Spu_SetSeqTrackADSR (int obj,int arg2);
void  Entity_StateExit (int *obj);
void  RenderView_SetViewport (int obj,int val);
int  ChunkData_New (void);
void  TextInput_Kill (int obj);
void  Spu_SetSeqTrackPan (int obj,int val);
void  Font_Dispatch (int *obj,int *arg2,int arg3);
void  NavMenu_SetSelection (int *obj,int arg2,int arg3);
void  SpuVoice_Nop2 (void);
void  Font_SetMode (int *obj,int arg2);
int * Palette_GetTable (int obj);
void  NavMenu_Init (int *obj,int *arg2);
void  Spu_BranchSeqTrack (int *obj);
void  NavMenu_Render (int *obj);
void  RenderView_AdvanceFrame (int *obj,int arg2,int arg3);
void  NavMenu_SetItemColor (int obj,int arg2);
void  RenderView_InitOt (int obj);
int * NavMenu_GetDataPtr (void);
void  NavMenu_HandleState (int *obj,int arg2,int arg3);
void  Entity_SetColor (int obj,int *arg2);
void  SpuVoice_SetDimensions (int obj,int *arg2);
void  Palette_Restore (int obj);
void  NavMenu_NavigateToItem (int *obj,int arg2,int arg3);
int  NavMenu_GetEntryData (int obj);
void  SpuVoice_SetUnk48 (int obj,int val);
void  NavMenu_Next (int *obj);
int  NavMenu_Advance (int *obj);
void  NavMenu_SelectPrev (int *obj);
void  Spu_CheckSeqTrack (int *obj,int arg2,int arg3);
void  RenderView_RenderScene (int *obj);
void  RenderView_FreeOt (int obj);
void  NavMenu_SetField (int obj,int val);
int  Entity_TestFlag (int obj,int arg2);
void  SpuVoice_Nop1 (void);
void  Primitive_Construct (int *obj,int arg2,int arg3,int arg4);
void  Sprite_Init (int *obj,u16 *arg2,int *arg3,int arg4);
void  Sprite_SetTexture (int *obj,int arg2,int arg3);
void  NavMenu_SelectState (int *obj);
void  Entity_InitState (int *obj,int arg2);
void  Spu_SetSeqTrackReverb (int obj,int val);

#endif /* INC_2C054_SHARED_H */
