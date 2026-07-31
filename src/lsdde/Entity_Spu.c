/* 2C054_part1.c -- split from 2C054.c */
#include "LateLink_Shared.h"

/* globals provided by dat_globals.h */
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
/* Possible GS_101.OBJ/GsSetNearClip Possible GS_102.OBJ/GsSetFarClip Possible GS_124.OBJ/GsSetWorkBase */ void  RenderView_SetClipNear (int obj);
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
/* Possible GS_106.OBJ/GsSetProjection Possible _OP_VDEL.OBJ/__builtin_vec_delete Possible _OP_VNEW.OBJ/__builtin_vec_new */ void  RenderView_SetGeomScreen (long obj);
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
/* Possible GS_101.OBJ/GsSetNearClip Possible GS_102.OBJ/GsSetFarClip Possible GS_124.OBJ/GsSetWorkBase */ void  RenderView_SetClipFar (int obj);
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
/* WARNING: Removing unreachable block (ram,0x800401e8) */ int  Entity_SetupAnimation (int *obj,int arg2,int arg3,int arg4);
void  Sprite_SetTexture (int *obj,int arg2,int arg3);
void  NavMenu_SelectState (int *obj);
void  Entity_InitState (int *obj,int arg2);
void  Spu_SetSeqTrackReverb (int obj,int val);
void  Spu_SetSeqTrackState (int *obj,int arg2)
{
  int iVar1;
  code *pcVar2;
  iVar1 =  NavMenu_GetDataPtr ();
  /* vtable[24] */
  (**(code **)(iVar1 + 0x60))(obj,arg2);
  if (arg2 == 7) {
    obj[0x36] = 1;
  }
  else if (arg2 < 8) {
    if (arg2 == 5) {
      obj[0x36] = 0;
    }
  }
  else {
    if (arg2 == 8) {
      if (obj[0x35] != 0) {
        return;
      }
      obj = (int *)obj[0x2d];
      /* vtable[19] */
      pcVar2 = *(code **)(*obj + 0x4c);
    }
    else {
      if (arg2 != 0x12) {
        return;
      }
      /* vtable[37] */
      pcVar2 = *(code **)(*obj + 0x94);
    }
    (*pcVar2)(obj);
  }
}
void  SpuVoice_SetUnk44 (int obj,int val)
{
  if (*(int *)(obj + 0x70) == 0) {
    *(int *)(obj + 0x44) = val;
  }
}
MATRIX * TransposeMatrix(MATRIX *m0,MATRIX *m1)
{
  short sVar1;
  short sVar2;
  m1->m[0][0] = m0->m[0][0];
  sVar1 = m0->m[2][0];
  m1->m[0][1] = m0->m[1][0];
  sVar2 = m0->m[0][1];
  m1->m[0][2] = sVar1;
  sVar1 = m0->m[1][1];
  m1->m[1][0] = sVar2;
  sVar2 = m0->m[2][1];
  m1->m[1][1] = sVar1;
  sVar1 = m0->m[0][2];
  m1->m[1][2] = sVar2;
  sVar2 = m0->m[1][2];
  m1->m[2][0] = sVar1;
  sVar1 = m0->m[2][2];
  m1->m[2][1] = sVar2;
  m1->m[2][2] = sVar1;
  return m1;
}
void  Entity_SetDuration (int obj,int val)
{
  *(int *)(obj + 0x44) = val;
}
int  EntityAllocLarge (int obj,int arg2,int arg3)
{
  int iVar1;
  int iVar2;
  iVar1 =  MemAllocImpl (0xa0);
  iVar2 = 0;
  if (iVar1 != 0) {
    iVar2 =  Palette_GetBaseTable ();
    (**(code **)(iVar2 + 8))(iVar1,obj,arg2,arg3);
    iVar2 = iVar1;
  }
  return iVar2;
}
void  Entity_StateUpdate (int *obj,int arg2)
{
  int iVar1;
  int uVar2;
  iVar1 = *obj;
  if (obj[0x1b] == 0) {
    return;
  }
  if (obj[0x1b] == 1) {
    uVar2 = 5;
    if (obj[0x26] != 0) goto exit;
    /* vtable[24] */
    (**(code **)(iVar1 + 0x60))(obj,0);
  }
  else {
    uVar2 = 6;
    if (obj[0x26] == 0) goto exit;
    if (obj[0x1e] == 0xf) {
      /* vtable[46] */
      (**(code **)(iVar1 + 0xb8))(obj,1,&Mem_TypeEntry8);
    }
  }
  (**(code **)(iVar1 + 100))(obj,0);
exit:
  /* vtable[5] */
  (**(code **)(iVar1 + 0x14))(obj,arg2);
  if (obj[0x1d] < 0) {
    obj[0x1d] = -obj[0x1d];
  }
  obj[0x1b] = 0;
  /* vtable[12] */
  (**(code **)(iVar1 + 0x30))(obj,uVar2);
}
/* Possible GS_101.OBJ/GsSetNearClip
   Possible GS_102.OBJ/GsSetFarClip
   Possible GS_124.OBJ/GsSetWorkBase */
void  RenderView_SetClipNear (int obj)
{
  Gpu_DrawMode2 = obj;
}
void Gssub_make_matrix(int *m,short angle,short val,char axis)
{
  int uVar1;
  int uVar2;
  uVar2 = Spu_ChanAttrC;
  uVar1 = Spu_ChanAttrB;
  *m = Spu_ChanAttr;
  m[1] = uVar1;
  m[2] = uVar2;
  uVar2 = Spu_ChanAttrF;
  uVar1 = Spu_ChanAttrE;
  m[3] = Spu_ChanAttrD;
  m[4] = uVar1;
  m[5] = uVar2;
  uVar1 = Spu_ChanAttr11;
  m[6] = Spu_ChanAttr10;
  m[7] = uVar1;
  switch(axis) {
  case 'X':
  case 'x':
    *(short *)(m + 2) = val;
    *(short *)(m + 4) = val;
    *(short *)((int)m + 10) = -angle;
    *(short *)((int)m + 0xe) = angle;
    GS_123_OBJ_C0();
    return;
  case 'Y':
  case 'y':
    *(short *)m = val;
    *(short *)(m + 4) = val;
    *(short *)(m + 1) = angle;
    *(short *)(m + 3) = -angle;
    GS_123_OBJ_C0();
    return;
  case 'Z':
  case 'z':
    *(short *)m = val;
    *(short *)(m + 2) = val;
    *(short *)((int)m + 2) = -angle;
    *(short *)((int)m + 6) = angle;
  }
}
void  Entity_SetLoopMask (int obj,uint arg2)
{
  *(int *)(obj + 0x68) = (1 << (arg2 & 0x1f)) + -1;
}
void  Palette_SetParams (int obj,int val,int arg3)
{
  *(int *)(obj + 0x98) = val;
  *(int *)(obj + 0x9c) = arg3;
}
int * Palette_GetBaseTable (void)
{
  return &Memory_ObjType1Vtable;
}
void  Font_Start (int obj)
{
  (**(code **)(*(int *)**(int **)(obj + 0xc) + 0x4c))();
  *(int *)(obj + 0x1c) = 0;
}
void  RenderView_SetScale (int obj,short *arg2)
{
  int iVar1;
  int iVar2;
  int iVar3;
  if (*(int *)(obj + 0x10) != 0) {
    iVar1 = (int)*arg2;
    iVar3 = (int)arg2[1];
    if (iVar3 == 0) {
      trap(TRAP_DIVZERO);
    }
    if ((iVar3 == -1) && (iVar1 == -0x80000000)) {
      trap(TRAP_OVERFLOW);
    }
    iVar2 = iVar1 % iVar3 << 0xc;
    if (iVar3 == 0) {
      trap(TRAP_DIVZERO);
    }
    if ((iVar3 == -1) && (iVar2 == -0x80000000)) {
      trap(TRAP_OVERFLOW);
    }
    *(int *)(obj + 0x2c) = (iVar1 / iVar3) * FIXED_12 + iVar2 / iVar3;
  }
}
void  RenderView_SetFlags (int obj,int val)
{
  *(int *)(obj + 0xb8) = val;
}
int * Primitive_GetVtable (void)
{
  return &Memory_ObjType2Vtable;
}
void  Spu_SetSeqTrackVolume (int obj,int val)
{
  *(int *)(obj + 0xd0) = val;
}
void  Spu_MuteSeqTrack (int obj)
{
  int iVar1;
  iVar1 =  NavMenu_GetDataPtr ();
  /* vtable[32] */
  (**(code **)(iVar1 + 0x80))(obj);
}
void  Entity_Complete (int *obj,int arg2,int arg3,int arg4)
{
  /* vtable[19] */
  (**(code **)(*obj + 0x4c))();
  obj[0x12] = 0;
  obj[0x13] = arg4;
}
int  RenderView_GetTailNode (int obj)
{
  int iVar1;
  iVar1 = *(int *)(obj + 0xc);
  while (iVar1 != 0) {
    obj = *(int *)(obj + 0xc);
    iVar1 = *(int *)(obj + 0xc);
  }
  return obj;
}
void  Entity_SetFlag (int obj,int arg2)
{
   MathBitfieldOp (obj + 0x58,0x1e,1,arg2 != 0);
}
int  NavMenu_Notify (int *obj)
{
  int iVar1;
  int iVar2;
  char local_18;
  char local_17;
  char local_16;
  iVar2 = obj[7];
  iVar1 = obj[0x21];
  local_16 = (char)(iVar2 * iVar1);
  local_18 = local_16 + (char)obj[0x24];
  local_17 = local_16 + *(char *)((int)obj + 0x91);
  local_16 = local_16 + *(char *)((int)obj + 0x92);
  /* vtable[57] */
  (**(code **)(*obj + 0xe4))(obj,&local_18);
  (**(code **)(*(int *)obj[0x1e] + 0xb8))((int *)obj[0x1e],1,&local_18);
  return 0x80 < (iVar2 * iVar1 & 0xffU);
}
/* Possible GS_106.OBJ/GsSetProjection
   Possible _OP_VDEL.OBJ/__builtin_vec_delete
   Possible _OP_VNEW.OBJ/__builtin_vec_new */
void  RenderView_SetGeomScreen (long obj)
{
  SetGeomScreen(obj);
}
void  Spu_Nop2 (void)
{
}
void  NavMenu_SetAllPositions (int obj,int arg2)
{
  int iVar1;
  int *piVar2;
  int *puVar3;
  int iVar4;
  iVar1 = *(int *)(obj + 0x58) * 4;
  iVar4 = *(int *)(iVar1 + *(int *)(obj + 0x5c));
  puVar3 = *(int **)(iVar1 + *(int *)(obj + 100));
  iVar1 = 0;
  if (0 < iVar4) {
    do {
      piVar2 = (int *)*puVar3;
      puVar3 = puVar3 + 1;
      iVar1 = iVar1 + 1;
      /* vtable[46] */
      (**(code **)(*piVar2 + 0xb8))(piVar2,arg2);
    } while (iVar1 < iVar4);
  }
}
void  NavMenu_InitContent (int *obj)
{
  int iVar1;
  iVar1 = *obj;
  /* vtable[27] */
  (**(code **)(iVar1 + 0x6c))(obj,0xffffffff);
  /* vtable[41] */
  (**(code **)(iVar1 + 0xa4))(obj,&Mem_TypeEntry2,&Mem_TypeEntry3,&Mem_TypeEntry4);
  /* vtable[39] */
  (**(code **)(iVar1 + 0x9c))(obj,1);
  /* vtable[40] */
  (**(code **)(iVar1 + 0xa0))(obj,1);
  obj[0x21] = 9;
  obj[10] = 3;
  obj[0xb] = 300;
  obj[0xc] = 0x40;
  obj[0x27] = 0;
  obj[0x28] = 0;
  obj[0xd] = 1;
  obj[0xf] = 0;
}
void  NavMenu_Confirm (int *obj)
{
  int uVar1;
  if (obj[0x13] != 0) {
    /* vtable[28] */
    (**(code **)(*obj + 0x70))(obj,0x10);
    uVar1 = 0xf;
    if (obj[0xf] == 1) {
      uVar1 = 0xb;
    }
    /* vtable[24] */
    (**(code **)(*obj + 0x60))(obj,uVar1);
  }
}
void  NavMenu_LoadTexture (int obj,int val,int arg3)
{
  int *piVar1;
  if (val == 0) {
    *(int *)(obj + 0x74) = arg3;
    *(int *)(obj + 0x70) = 0;
  }
  else {
    if (*(int *)(obj + 0x70) != 0) {
      (**(code **)(**(int **)(obj + 0x74) + 4))();
    }
    piVar1 = (int *) Texture_New (val);
    *(int **)(obj + 0x74) = piVar1;
    /* vtable[30] */
    (**(code **)(*piVar1 + 0x78))(piVar1);
    (**(code **)(**(int **)(obj + 0x74) + 0x5c))();
    *(int *)(obj + 0x70) = val;
  }
}
void  Palette_SaveAndSet (int obj,u16 *val,int *arg3)
{
  int uVar1;
  if (*(int *)(obj + 0xc) != 0) {
    *(uint *)(obj + 0x88) = (uint)*(ushort *)(obj + 0x60);
    *(uint *)(obj + 0x8c) = (uint)*(ushort *)(obj + 0x62);
    *(int *)(obj + 0x90) = *(int *)(obj + 0x50);
    *(int *)(obj + 0x94) = *(int *)(obj + 0x54);
    *(u16 *)(obj + 0x60) = *val;
    *(u16 *)(obj + 0x62) = val[2];
    uVar1 = arg3[1];
    *(int *)(obj + 0x50) = *arg3;
    *(int *)(obj + 0x54) = uVar1;
  }
}
void  Spu_UnmuteSeqTrack (int obj)
{
  int iVar1;
  iVar1 =  NavMenu_GetDataPtr ();
  /* vtable[33] */
  (**(code **)(iVar1 + 0x84))(obj);
}
void  RenderView_HandleMode (int *obj,int arg2,int arg3)
{
  if (arg3 == 2) {
    /* vtable[41] */
    (**(code **)(*obj + 0xa4))();
  }
}
void  TextInput_DrawPositioned (int obj,int *arg2)
{
  int iVar1;
  int iVar2;
  int *puVar3;
  int local_20;
  int local_1c;
  if (*(int *)(obj + 0xc) != 0) {
    iVar2 = 0;
    iVar1 =  Label_GetVtable ();
    /* vtable[47] */
    (**(code **)(iVar1 + 0xbc))(obj,arg2);
    local_20 = *arg2;
    local_1c = arg2[1];
    puVar3 = *(int **)(obj + 0xb4);
    if (*(char *)(obj + 0xa9) != '\0') {
      do {
        (**(code **)(*(int *)*puVar3 + 0xbc))((int *)*puVar3,&local_20);
        local_20 = local_20 + *(int *)(obj + 0xb0);
        iVar2 = iVar2 + 1;
        puVar3 = puVar3 + 1;
      } while (iVar2 < (int)(uint)*(byte *)(obj + 0xa9));
    }
  }
}
void  RenderView_SetTranslation (int obj,int *arg2)
{
  int uVar1;
  int uVar2;
  if (*(int *)(obj + 0x10) != 0) {
    uVar1 = arg2[1];
    uVar2 = arg2[2];
    *(int *)(obj + 0x20) = *arg2;
    *(int *)(obj + 0x24) = uVar1;
    *(int *)(obj + 0x28) = uVar2;
  }
}
void  NavMenu_DrawItems (int obj,int arg2,int arg3)
{
  int iVar1;
  int *piVar2;
  int *puVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int local_48;
  int local_44;
  int local_40;
  int local_3c;
  iVar6 = *(int *)(obj + 0x58);
  iVar1 = iVar6 * 4;
  puVar3 = *(int **)(iVar1 + *(int *)(obj + 100));
  iVar5 = *(int *)(iVar1 + *(int *)(obj + 0x5c));
  iVar7 = *(int *)(*(int *)(iVar1 + *(int *)(*(int *)(obj + 0x4c) + 0x24)) + 4);
  iVar1 = 0;
  if (0 < iVar5) {
    do {
      piVar2 = (int *)*puVar3;
      puVar3 = puVar3 + 1;
      iVar1 = iVar1 + 1;
      /* vtable[20] */
      (**(code **)(*piVar2 + 0x50))();
    } while (iVar1 < iVar5);
  }
  iVar1 = *(int *)(iVar6 * 4 + *(int *)(*(int *)(obj + 0x4c) + 0x24));
  local_48 = *(int *)(iVar1 + 0x10);
  local_44 = *(int *)(iVar1 + 0x14) + iVar7 * -10;
  if (arg3 == 0) {
    (**(code **)(**(int **)(obj + 0x68) + 0x50))();
  }
  else {
    (**(code **)(**(int **)(obj + 0x68) + 0x4c))
              (*(int **)(obj + 0x68),*(int *)(obj + 0x14));
    local_40 = 0x28;
    local_3c = iVar5 * 0xc;
    (**(code **)(**(int **)(obj + 0x68) + 0xc0))(*(int **)(obj + 0x68),&local_40);
  }
  iVar4 = 0;
  iVar1 = *(int *)(obj + 100);
  puVar3 = *(int **)(iVar6 * 4 + iVar1);
  if (0 < iVar5) {
    do {
      (**(code **)(*(int *)*puVar3 + 0x4c))((int *)*puVar3,arg2,&local_48);
      iVar4 = iVar4 + 1;
      (**(code **)(*(int *)*puVar3 + 0x60))((int *)*puVar3,arg3);
      local_44 = local_44 + 10;
      puVar3 = puVar3 + 1;
    } while (iVar4 < iVar5);
    iVar1 = *(int *)(obj + 100);
  }
  piVar2 = *(int **)(iVar7 * 4 + *(int *)(iVar6 * 4 + iVar1));
  /* vtable[24] */
  (**(code **)(*piVar2 + 0x60))(piVar2,1);
}
void  Spu_SetSeqTrackPosition (int obj,int val,int arg3,int arg4,
                 int arg5)
{
  int iVar1;
  *(int *)(obj + 0xb8) = arg3;
  *(int *)(obj + 0xbc) = arg4;
  *(int *)(obj + 0xc0) = arg5;
  iVar1 =  NavMenu_GetDataPtr ();
  /* vtable[17] */
  (**(code **)(iVar1 + 0x44))(obj,val,0);
}
void  RenderView_Deactivate (int *obj)
{
  if (obj[4] != 0) {
    /* vtable[5] */
    (**(code **)(*obj + 0x14))();
  }
}
void  ChunkDataObj_SetChild (int obj,int *val)
{
  int iVar1;
  iVar1 =  GetCoordSystemVtable ();
  /* vtable[4] */
  (**(code **)(iVar1 + 0x10))(obj,val);
  if ((*(uint *)*val & 0xf) == 4) {
    *(int **)(obj + 0x10) = val;
    *(int *)(obj + 0x30) = val[5];
  }
  else if ((*(uint *)*val & 0xf) == 1) {
    *(int **)(obj + 0xc) = val;
  }
}
void  NavMenu_CancelAction (int *obj)
{
  if ((obj[0x13] != 0) && (obj[0xf] != 1)) {
    /* vtable[28] */
    (**(code **)(*obj + 0x70))(obj,0x10);
    /* vtable[24] */
    (**(code **)(*obj + 0x60))(obj,0x11);
  }
}
/* Possible GS_112.OBJ/GsDrawOtIO */
void GsDrawOt(GsOT *ot)
{
  DrawOTag((u_long *)ot->tag);
}
void  NavMenu_SetItemSpacing (int obj,int val)
{
  *(int *)(obj + 0x40) = val;
  if (-1 < val) {
    *(int *)(obj + 0x40) = val * 0x14;
  }
}
void  NavMenu_Callback (int *obj)
{
  if ((code *)obj[0x27] != (code *)0x0) {
    (*(code *)obj[0x27])(obj[0x28]);
  }
  /* vtable[24] */
  (**(code **)(*obj + 0x60))(obj,7);
}
void  Entity_ConstructFromTemplate (int *obj,int arg2,int arg3,int arg4)
{
  int iVar1;
  int *puVar2;
  iVar1 =  Primitive_GetVtable ();
  if (arg3 == 0) {
    puVar2 = &Mem_TypeEntry8;
  }
  else {
    puVar2 = &UNK_8006ea90 + arg3 * 3;
  }
  (**(code **)(iVar1 + 8))(obj,arg2,puVar2,arg4);
  iVar1 =  Palette_GetBaseTable ();
  *obj = iVar1;
  /* vtable[16] */
  (**(code **)(iVar1 + 0x40))(obj,arg3);
}
void  NavMenu_PageNext (int *obj)
{
  int iVar1;
  iVar1 = *(int *)(obj[0x16] * 4 + obj[0x18]) + 1;
  if (*(int *)(obj[0x16] * 4 + obj[0x17]) <= iVar1) {
    iVar1 = 0;
  }
  /* vtable[71] */
  (**(code **)(*obj + 0x11c))(obj,iVar1,1);
}
void  ChunkDataObj_Dispatch (int *obj,int *arg2,int arg3)
{
  int iVar1;
  code *pcVar2;
  iVar1 =  GetCoordSystemVtable ();
  /* vtable[14] */
  (**(code **)(iVar1 + 0x38))(obj,arg2,arg3);
  if ((*(uint *)*arg2 & 0xf) == 5) {
    /* vtable[37] */
    pcVar2 = *(code **)(*obj + 0x94);
  }
  else {
    if ((*(uint *)*arg2 & 0xf) != 1) {
      return;
    }
    /* vtable[38] */
    pcVar2 = *(code **)(*obj + 0x98);
  }
  (*pcVar2)(obj,arg2,arg3);
}
void  NavMenu_Prev (int *obj)
{
  code *pcVar1;
  if (obj[0x13] != 0) {
    if (obj[0xf] == 1) {
      /* vtable[58] */
      pcVar1 = *(code **)(*obj + 0xe8);
    }
    else {
      if (obj[0xf] != 2) {
        return;
      }
      /* vtable[69] */
      pcVar1 = *(code **)(*obj + 0x114);
    }
    (*pcVar1)();
  }
}
void  NavMenu_UpdateItem (int *obj,int arg2,int arg3)
{
  int iVar1;
  code *pcVar2;
  int uVar3;
  int iVar4;
  iVar4 = *obj;
  iVar1 =  Font_GetDataPtr ();
  /* vtable[23] */
  (**(code **)(iVar1 + 0x5c))(obj,arg2,arg3);
  if ((obj[0xf] != 0) && ((uint)obj[0x10] < (uint)obj[7])) {
    /* vtable[24] */
    (**(code **)(iVar4 + 0x60))(obj,6);
  }
  iVar1 = obj[8];
  if (iVar1 == 4) {
    /* vtable[43] */
    pcVar2 = *(code **)(iVar4 + 0xac);
code_r0x8003c5fc:
    (*pcVar2)(obj);
  }
  else {
    if (iVar1 < 5) {
      if (iVar1 != 2) {
        return;
      }
      /* vtable[24] */
      pcVar2 = *(code **)(iVar4 + 0x60);
      uVar3 = 4;
    }
    else {
      if (iVar1 == 7) {
        /* vtable[48] */
        pcVar2 = *(code **)(iVar4 + 0xc0);
        goto code_r0x8003c5fc;
      }
      if (iVar1 != 8) {
        return;
      }
      /* vtable[24] */
      pcVar2 = *(code **)(iVar4 + 0x60);
      uVar3 = 3;
    }
    (*pcVar2)(obj,uVar3);
  }
}
int  NavMenu_GetField (int obj,int arg2,int arg3)
{
  int iVar1;
  iVar1 =  Font_GetDataPtr ();
  /* vtable[17] */
  (**(code **)(iVar1 + 0x44))(obj,arg2,arg3);
  return *(int *)(obj + 0x38);
}
void  NavMenu_InitPage (int *obj,int arg2,int arg3,int arg4)
{
  int iVar1;
  iVar1 =  Font_GetDataPtr ();
  (**(code **)(iVar1 + 8))(obj);
  iVar1 =  NavMenu_GetDataPtr ();
  *obj = iVar1;
  /* vtable[54] */
  (**(code **)(iVar1 + 0xd8))(obj,arg2);
  if (arg3 == 0) {
    obj[0x12] = arg4;
  }
  else {
    iVar1 =  EntityAllocMedium (arg3);
    obj[0x12] = iVar1;
  }
  obj[0x11] = arg3;
  /* vtable[53] */
  (**(code **)(*obj + 0xd4))(obj,0,0);
  iVar1 =  NavMenu_NewMenu (0);
  obj[0x20] = iVar1;
  iVar1 =  NavMenu_NewList (0);
  obj[0x1f] = iVar1;
  iVar1 =  NavMenu_NewPage (iVar1,1,0);
  obj[0x1e] = iVar1;
  /* vtable[16] */
  (**(code **)(*obj + 0x40))(obj);
}
void  TextInput_Return (void)
{
}
int  TextInput_New (int obj,int arg2,int arg3)
{
  int iVar1;
  int iVar2;
  iVar1 =  MemAllocImpl (0xb8);
  iVar2 = 0;
  if (iVar1 != 0) {
    iVar2 =  TextInput_GetVtable ();
    (**(code **)(iVar2 + 8))(iVar1,obj,arg2,arg3);
    iVar2 = iVar1;
  }
  return iVar2;
}
void  NavMenu_SelectNext (int *obj)
{
  int iVar1;
  if (obj[0x13] != 0) {
    iVar1 = obj[0x16];
    do {
      iVar1 = iVar1 + 1;
      if (obj[0x14] <= iVar1) {
        iVar1 = 0;
      }
    } while ((iVar1 != obj[0x16]) && (*(int *)(iVar1 * 4 + *(int *)(obj[0x13] + 0x18)) != 0)
            );
    /* vtable[60] */
    (**(code **)(*obj + 0xf0))(obj,iVar1,1);
  }
}
void  NavMenu_DelPage (int *obj)
{
  int iVar1;
  (**(code **)(*(int *)obj[0x1e] + 4))();
  (**(code **)(*(int *)obj[0x1f] + 4))();
  (**(code **)(*(int *)obj[0x20] + 4))();
  if (obj[0x11] != 0) {
    (**(code **)(*(int *)obj[0x12] + 4))();
  }
  if (obj[0x1c] != 0) {
    (**(code **)(*(int *)obj[0x1d] + 4))();
  }
  /* vtable[55] */
  (**(code **)(*obj + 0xdc))(obj);
  iVar1 =  Font_GetDataPtr ();
  /* vtable[3] */
  (**(code **)(iVar1 + 0xc))(obj);
}
void  Entity_SetBlendMode (int obj,int arg2)
{
   MathBitfieldOp (obj + 0x58,0x1c,2,arg2);
}
void  SpuVoice_SetColor (int obj,u8 *arg2)
{
  char uVar1;
  char uVar2;
  uVar1 = arg2[1];
  uVar2 = arg2[2];
  *(u8 *)(obj + 0x58) = *arg2;
  *(u8 *)(obj + 0x59) = uVar1;
  *(u8 *)(obj + 0x5a) = uVar2;
}
void  Spu_StopSeqTrack (int *obj)
{
  int iVar1;
  iVar1 =  NavMenu_GetDataPtr ();
  /* vtable[30] */
  (**(code **)(iVar1 + 0x78))(obj);
  if (obj[0x33] != 0) {
    obj[0xe] = 2;
    /* vtable[24] */
    (**(code **)(*obj + 0x60))(obj,0x12);
  }
}
void  RenderView_FlipBuffers (int obj)
{
  int uVar1;
  int iVar2;
  if (*(int *)(obj + 0x70) == 0) {
    return;
  }
  uVar1 = (**(code **)(**(int **)(obj + 0xc) + 0x54))();
  *(int *)(obj + 0x74) = uVar1;
  if (*(int *)(obj + 0xb8) != 0) {
    ResetGraph(1);
    (**(code **)(**(int **)(obj + 0xc) + 0x50))();
    if (*(int *)(obj + 0xb4) == 0) {
skip:
      iVar2 = *(int *)(obj + 0x74);
    }
    else {
      iVar2 = *(int *)(obj + 0x74);
      if (iVar2 == 0) {
        (**(code **)(**(int **)(obj + 0xc) + 0x50))();
        goto skip;
      }
    }
    GsSortClear(*(uchar *)(obj + 0x58),*(uchar *)(obj + 0x59),*(uchar *)(obj + 0x5a),
                *(GsOT **)(iVar2 * 4 + obj + 0x78));
    GsDrawOt(*(GsOT **)(*(int *)(obj + 0x74) * 4 + obj + 0x78));
    if (*(int *)(obj + 0xb4) != 0) {
      iVar2 = *(int *)(obj + 0x74);
      if (iVar2 != 0) goto exit;
      (**(code **)(**(int **)(obj + 0xc) + 0x50))();
    }
  }
  iVar2 = *(int *)(obj + 0x74);
exit:
  *(uint *)(obj + 0x74) = (uint)(iVar2 == 0);
}
void  NavMenu_SetOption (int *obj,int arg2)
{
  if (arg2 == 0) {
    obj[0x22] = 0;
  }
  else if (arg2 == 1) {
    obj[0x22] = *(int *)(*obj + 0xb0);
  }
}
int  TextInput_Update (int obj,int arg2,int arg3)
{
  uint uVar1;
  int *piVar2;
  int *puVar3;
  uVar1 = (uint)*(byte *)(obj + 0xac);
  puVar3 = (int *)(uVar1 * 4 + *(int *)(obj + 0xb4));
  if (uVar1 < uVar1 + *(byte *)(obj + 0xab)) {
    do {
      piVar2 = (int *)*puVar3;
      puVar3 = puVar3 + 1;
      uVar1 = uVar1 + 1;
      /* vtable[24] */
      arg3 = (**(code **)(*piVar2 + 0x60))(piVar2,arg2);
    } while ((int)uVar1 < (int)((uint)*(byte *)(obj + 0xac) + (uint)*(byte *)(obj + 0xab)));
  }
  return arg3;
}
void  Font_ResetState (int obj)
{
  *(int *)(obj + 0x1c) = 0;
  *(int *)(obj + 0x20) = 0;
}
int  RenderView_GetBackground (int obj)
{
  return *(int *)(obj + 0xb0);
}
void  Spu_Nop (void)
{
}
int  NavMenu_GetSelection (int obj)
{
  return *(int *)(obj + 0x58);
}

