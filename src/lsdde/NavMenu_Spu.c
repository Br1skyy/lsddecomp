/* 2C054_part3.c -- split from 2C054.c */
#include "LateLink_Shared.h"
/* BasicClass vtables (declared in Entity2.c) */
extern void BasicClass__func_17eb0(void *o);
extern void BasicClass__func_17f98(void);
extern void BasicClass__func_17ff0(void);
extern void BasicClass__func_18040(void);
extern void BasicClass__func_180bc(int this, int *out, int *iter);
extern void BasicClass__func_180fc(int this, int value);
extern void BasicClass__func_1811c(int this, int value);
extern void BasicClass__func_1813c(int this);
extern void BasicClass__func_1816c(int this, int *out, int *iter);
extern void BasicClass__func_182cc(int this, int param2);
extern void BasicClass__func_18350(void);
extern void BasicClass__func_18358(void);
/* Font vtable functions */
extern void Font_Dispatch(int *obj, int *arg2, int arg3);
extern void Font_Destroy(int *obj);
/* SPU seq track functions */
extern void Spu_ResetSeqTrack(int obj);
extern void Spu_SetSeqTrackPosition(int obj, int val, int arg3, int arg4, int arg5);
extern void Spu_UpdateSeqTrack(int *obj);
extern void Spu_CheckSeqTrack(int *obj, int arg2, int arg3);
extern void Spu_SetSeqTrackState(int *obj, int arg2);
extern void Spu_StopSeqTrack(int *obj);
extern void Spu_MuteSeqTrack(int obj);
extern void Spu_UnmuteSeqTrack(int obj);
extern void Spu_BranchSeqTrack(int *obj);
extern void Spu_SetSeqTrackPitch(int obj, int val);
extern void Spu_SetSeqTrackVolume(int obj, int val);
/* NavMenu extended functions */
extern void NavMenu_SetItemColor(int obj, int arg2);
extern void NavMenu_SetOption(int *obj, int arg2);
extern void NavMenu_DrawItems(int obj, int arg2, int arg3);
extern void NavMenu_SetAllPositions(int obj, int arg2);
extern void NavMenu_ConfirmState(int *obj);
extern void NavMenu_CancelState(int *obj);
extern void NavMenu_PageNext(int *obj);
extern void NavMenu_PagePrev(int *obj);
/* NavMenu Page vtable */
static void *NavMenu_PageVtable[64] = {
    (void*)0x130,                        /* [0]  type metadata */
    BasicClass__func_17eb0,              /* [1]  destructor */
    NavMenu_InitPage,                    /* [2]  constructor */
    NavMenu_DelPage,                     /* [3]  page deletion */
    BasicClass__func_17f98,              /* [4]  basic class */
    BasicClass__func_17ff0,              /* [5]  basic class */
    BasicClass__func_18040,              /* [6]  basic class */
    BasicClass__func_180bc,              /* [7]  basic class */
    BasicClass__func_180fc,              /* [8]  basic class */
    BasicClass__func_1811c,              /* [9]  basic class */
    BasicClass__func_1813c,              /* [10] basic class */
    BasicClass__func_1816c,              /* [11] basic class */
    BasicClass__func_182cc,              /* [12] basic class */
    BasicClass__func_18350,              /* [13] basic class */
    Font_Dispatch,                       /* [14] font dispatch */
    0,                                   /* [15] NULL */
    NavMenu_InitContent,                 /* [16] init content */
    NavMenu_GetField,                    /* [17] get field */
    Font_Destroy,                        /* [18] font destroy */
    NavMenu_Render,                      /* [19] render */
    NavMenu_Update,                      /* [20] update */
    BasicClass__func_18358,              /* [21] unknown (font?) */
    NavMenu_HandleState,                 /* [22] handle state */
    NavMenu_UpdateItem,                  /* [23] update item */
    NavMenu_SetItemState,                /* [24] set item state */
    BasicClass__func_18358,              /* [25] unknown (font?) */
    BasicClass__func_18358,              /* [26] unknown (font?) */
    NavMenu_SetItemSpacing,              /* [27] set item spacing */
    BasicClass__func_18358,              /* [28] unknown (SetFont?) */
    BasicClass__func_18358,              /* [29] unknown (GetFont?) */
    BasicClass__func_18358,              /* [30] unknown (GetOption?) */
    NavMenu_CancelAction,                /* [31] cancel action */
    NavMenu_Next,                        /* [32] next */
    NavMenu_Prev,                        /* [33] prev */
    0,                                   /* [34] NULL */
    0,                                   /* [35] NULL */
    NavMenu_ChangePage,                  /* [36] change page */
    NavMenu_Callback,                    /* [37] callback */
    NavMenu_SetBounds,                   /* [38] set bounds */
    BasicClass__func_18358,              /* [39] unknown (GetBounds?) */
    NavMenu_SetOption2,                  /* [40] set option 2 */
    NavMenu_SetColor,                    /* [41] set color */
    NavMenu_SetField,                    /* [42] set field */
    NavMenu_Check,                       /* [43] check */
    NavMenu_Notify,                      /* [44] notify */
    0,                                   /* [45] NULL */
    0,                                   /* [46] NULL */
    0,                                   /* [47] NULL */
    NavMenu_Advance,                     /* [48] advance */
    NavMenu_FadeCheck,                   /* [49] fade check */
    0,                                   /* [50] NULL */
    0,                                   /* [51] NULL */
    0,                                   /* [52] NULL */
    NavMenu_LoadTexture,                 /* [53] load texture */
    NavMenu_Init,                        /* [54] init */
    NavMenu_Destroy,                     /* [55] destroy */
    NavMenu_Draw,                        /* [56] draw */
    NavMenu_SetPosition,                 /* [57] set position */
    NavMenu_SelectNext,                  /* [58] select next */
    NavMenu_SelectPrev,                  /* [59] select prev */
    NavMenu_SetSelection,                /* [60] set selection */
    NavMenu_GetSelection,                /* [61] get selection */
    NavMenu_AddItems,                    /* [62] add items */
    NavMenu_FreeItems,                   /* [63] free items */
};
/* SpuSeqTrackVtableB -- vtable for SPU sequence track objects (64+14 = 78 entries)
 * Entries 0-63 mirror the NavMenu vtable with SPU-specific overrides at [2], [3], [16], [17],
 * [19], [23], [24], [27], [30], [32]-[35], [37].
 * Extended entries [64]-[77] are SPU-specific (track draw, state management, hardware params).
 * Extracted from D_8006E5F8 in the original binary.
 */
void *SpuSeqTrackVtableB[78] = {
    (void *)0x00001130,                    /* [0]  size (4400 bytes) */
    BasicClass__func_17eb0,                /* [1]  destructor */
    Spu_InitSeqTrack,                      /* [2]  constructor */
    Spu_DelSeqTrack,                       /* [3]  destroy */
    BasicClass__func_17f98,                /* [4] */
    BasicClass__func_17ff0,                /* [5] */
    BasicClass__func_18040,                /* [6] */
    BasicClass__func_180bc,                /* [7] */
    BasicClass__func_180fc,                /* [8] */
    BasicClass__func_1811c,                /* [9] */
    BasicClass__func_1813c,                /* [10] */
    BasicClass__func_1816c,                /* [11] */
    BasicClass__func_182cc,                /* [12] */
    BasicClass__func_18350,                /* [13] */
    Font_Dispatch,                         /* [14] */
    0,                                     /* [15] */
    Spu_ResetSeqTrack,                     /* [16] */
    Spu_SetSeqTrackPosition,               /* [17] */
    Font_Destroy,                          /* [18] */
    Spu_UpdateSeqTrack,                    /* [19] */
    NavMenu_Update,                        /* [20] */
    Font_EventProc,                        /* [21] */
    NavMenu_HandleState,                   /* [22] */
    Spu_CheckSeqTrack,                     /* [23] */
    Spu_SetSeqTrackState,                  /* [24] */
    Font_Stop,                             /* [25] */
    Font_Start,                            /* [26] */
    Spu_SetSeqTrackPitchMod,               /* [27] */
    NavMenu_SetItemColor,                  /* [28] */
    NavMenu_Resume,                        /* [29] */
    Spu_StopSeqTrack,                      /* [30] */
    NavMenu_CancelAction,                  /* [31] */
    Spu_MuteSeqTrack,                      /* [32] */
    Spu_UnmuteSeqTrack,                    /* [33] */
    Spu_Nop,                               /* [34] */
    Spu_Nop2,                              /* [35] */
    NavMenu_ChangePage,                    /* [36] */
    Spu_BranchSeqTrack,                    /* [37] */
    NavMenu_SetBounds,                     /* [38] */
    NavMenu_SetOption,                     /* [39] */
    NavMenu_SetOption2,                    /* [40] */
    NavMenu_SetColor,                      /* [41] */
    NavMenu_SetField,                      /* [42] */
    NavMenu_Check,                         /* [43] */
    NavMenu_Notify,                        /* [44] */
    0, 0, 0,                               /* [45-47] */
    NavMenu_Advance,                       /* [48] */
    NavMenu_FadeCheck,                     /* [49] */
    0, 0, 0,                               /* [50-52] */
    NavMenu_LoadTexture,                   /* [53] */
    NavMenu_Init,                          /* [54] */
    NavMenu_Destroy,                       /* [55] */
    NavMenu_Draw,                          /* [56] */
    NavMenu_SetPosition,                   /* [57] */
    NavMenu_SelectNext,                    /* [58] */
    NavMenu_SelectPrev,                    /* [59] */
    NavMenu_SetSelection,                  /* [60] */
    NavMenu_GetSelection,                  /* [61] */
    NavMenu_AddItems,                      /* [62] */
    NavMenu_FreeItems,                     /* [63] */
    NavMenu_DrawItems,                     /* [64] */
    NavMenu_SetAllPositions,               /* [65] */
    NavMenu_SelectState,                   /* [66] */
    NavMenu_ConfirmState,                  /* [67] */
    NavMenu_CancelState,                   /* [68] */
    NavMenu_PageNext,                      /* [69] */
    NavMenu_PagePrev,                      /* [70] */
    NavMenu_NavigateToItem,                /* [71] */
    NavMenu_GetEntryData,                  /* [72] */
    Spu_SetSeqTrackPitch,                  /* [73] */
    Spu_SetSeqTrackADSR,                   /* [74] */
    Spu_SetSeqTrackPan,                    /* [75] */
    Spu_SetSeqTrackVolume,                 /* [76] */
    Spu_SetSeqTrackReverb,                 /* [77] */
};
void  NavMenu_Draw (int *obj,int arg2)
{
  int iVar1;
  int *puVar2;
  int iVar3;
  if (obj[0x13] != 0) {
    puVar2 = (int *)obj[0x15];
    iVar3 = *(int *)(obj[0x13] + 0x20);
    iVar1 = 0;
    if (0 < obj[0x14]) {
      do {
        if (*(int *)(iVar1 * 4 + *(int *)(obj[0x13] + 0x18)) == 0) {
          (*(code *)(*(int *)*puVar2 + 0x4c))((int *)*puVar2,arg2,iVar3);
          if (*(int *)(iVar1 * 4 + *(int *)(obj[0x13] + 0x24)) != 0) {
            obj[0x16] = iVar1;
            /* vtable[64] */
            (*(code *)(*obj + 0x100))(obj,arg2,0);
          }
        }
        else {
          (*(code *)(*(int *)*puVar2 + 0x50))((int *)*puVar2,arg2);
        }
        iVar1 = iVar1 + 1;
        puVar2 = puVar2 + 1;
        iVar3 = iVar3 + 8;
      } while (iVar1 < obj[0x14]);
    }
  }
}
void  Spu_SetSeqTrackPitchMod (int obj,int val)
{
  *(int *)(obj + 0x40) = val;
  if (-1 < val) {
    *(int *)(obj + 0x40) = val * 0xf;
  }
}
int  NavMenu_FadeCheck (int *obj)
{
  uint uVar1;
  char local_18;
  char local_17;
  char local_16;
  uVar1 = 0x80 - obj[7] * obj[0x21];
  local_18 = (u8)uVar1;
  local_17 = local_18;
  local_16 = local_18;
  /* vtable[57] */
  (*(code *)(*obj + 0xe4))(obj,&local_18);
  (*(code *)(*(int *)obj[0x1e] + 0xb8))((int *)obj[0x1e],1,&local_18);
  return 0x80 < (uVar1 & 0xff);
}
void  RenderView_StubNop2 (void)
{
}
void  Entity_ProcessState (int *obj,int arg2,int arg3)
{
  int iVar1;
  if (arg3 == 2) {
    iVar1 = obj[0x20];
    obj[0x20] = iVar1 + -1;
    if (iVar1 < 1) {
      /* vtable[56] */
      (*(code *)(*obj + 0xe0))();
    }
    else if (obj[0x1f] != 9) {
      if ((obj[0x1e] & 4U) != 0) {
        *(char *)(obj + 0x19) = (char)obj[0x19] + (char)obj[0x1d];
      }
      if ((obj[0x1e] & 2U) != 0) {
        *(char *)((int)obj + 0x65) = *(char *)((int)obj + 0x65) + (char)obj[0x1d];
      }
      if ((obj[0x1e] & 1U) != 0) {
        *(char *)((int)obj + 0x66) = *(char *)((int)obj + 0x66) + (char)obj[0x1d];
      }
    }
  }
}
void  NavMenu_FreeItems (int obj)
{
  int iVar1;
  iVar1 = *(int *)(obj + 0x58) * 4;
   ArrayInitByVtable (*(int *)(iVar1 + *(int *)(obj + 100)),
               *(int *)(iVar1 + *(int *)(obj + 0x5c)));
   MemFreeImpl (*(int *)(*(int *)(obj + 0x58) * 4 + *(int *)(obj + 100)));
}
void  NavMenu_Resume (int *obj)
{
  if (obj[0x13] != 0) {
    /* vtable[28] */
    (*(code *)(*obj + 0x70))(obj,0x10);
    /* vtable[24] */
    (*(code *)(*obj + 0x60))(obj,10);
  }
}
void  Spu_DelSeqTrack (int obj)
{
  int iVar1;
  (*(code *)(**(int **)(obj + 0xb4) + 4))();
  iVar1 =  NavMenu_GetDataPtr ();
  /* vtable[3] */
  (*(code *)(iVar1 + 0xc))(obj);
}
void  TextInput_Destroy (int obj)
{
  int uVar1;
  int iVar2;
   ArrayInitByVtable (*(int *)(obj + 0xb4),*(u8 *)(obj + 0xa9));
  uVar1 =  MemFreeImpl (*(int *)(obj + 0xb4));
  *(int *)(obj + 0xb4) = uVar1;
  iVar2 =  Label_GetVtable ();
  /* vtable[3] */
  (*(code *)(iVar2 + 0xc))(obj);
}
void  Spu_InitSeqTrack (int *obj,int val,int arg3,int arg4,int *arg5
                 )
{
  int iVar1;
  int *piVar2;
  int uVar3;
  int iVar4;
  iVar1 =  NavMenu_GetDataPtr ();
  (*(code *)(iVar1 + 8))(obj,val,arg3,arg4);
  iVar1 =  Spu_GetSeqTrackVtable ();
  *obj = iVar1;
  if (arg5 == NULL) {
    piVar2 = (int *) TypeEntry1_GetDataPtr ();
    iVar1 = piVar2[1];
    iVar4 = piVar2[2];
    obj[0x2a] = *piVar2;
    obj[0x2b] = iVar1;
    obj[0x2c] = iVar4;
  }
  else {
    iVar1 = arg5[1];
    iVar4 = arg5[2];
    obj[0x2a] = *arg5;
    obj[0x2b] = iVar1;
    obj[0x2c] = iVar4;
  }
  uVar3 =  TypeEntry1_GetDataPtr ();
  iVar1 =  Spu_NewSeqTrack (uVar3,0,0,0);
  obj[0x2d] = iVar1;
  obj[0x2e] = 0;
  /* vtable[16] */
  (*(code *)(*obj + 0x40))(obj);
}
void  TextInput_InitLabels (int *obj,int arg2,int arg3,int arg4)
{
  int iVar1;
  int *puVar2;
  int uVar3;
  iVar1 =  Label_GetVtable ();
  (*(code *)(iVar1 + 8))(obj,arg2,0x20);
  iVar1 =  TextInput_GetVtable ();
  *obj = iVar1;
  *(char *)((int)obj + 0xa9) = (char)arg3;
  *(char *)((int)obj + 0xab) = (char)arg3;
  *(u8 *)(obj + 0x2b) = 0;
  *(u8 *)((int)obj + 0xaa) = 0;
  puVar2 = (int *) MemAllocImpl (arg3 << 2);
  if (puVar2 != NULL) {
    obj[0x2d] = (int)puVar2;
    iVar1 = 0;
    if (0 < arg3) {
      do {
        uVar3 =  TextInput_NewLabel (arg2,0x20);
        *puVar2 = uVar3;
        iVar1 = iVar1 + 1;
        puVar2 = puVar2 + 1;
      } while (iVar1 < arg3);
    }
    /* vtable[16] */
    (*(code *)(*obj + 0x40))(obj,arg4);
  }
}
void  SpuVoice_SetUnk54 (int obj,int val)
{
  *(int *)(obj + 0x54) = val;
}
void  SpuVoice_SetDepth (int obj,int val)
{
  *(int *)(obj + 0x3c) = val;
}
void  TextInput_SetPosition (int obj,int arg2)
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
      /* vtable[46] */
      (*(code *)(*piVar2 + 0xb8))(piVar2,arg2);
      uVar1 = uVar1 + 1;
    } while ((int)uVar1 < (int)((uint)*(byte *)(obj + 0xac) + (uint)*(byte *)(obj + 0xab)));
  }
}
void  Spu_SetSeqTrackADSR (int obj,int arg2)
{
  *(int *)(obj + 200) = arg2;
}
void  Entity_StateExit (int *obj)
{
  int iVar1;
  if (obj[0x1b] == 0) {
    /* vtable[55] */
    iVar1 = (*(code *)(*obj + 0xdc))();
    if (obj[0x26] == 0) {
      /* vtable[46] */
      (*(code *)(*obj + 0xb8))(obj,1,&Mem_TypeEntry8 + iVar1 * 3);
    }
    else {
      obj[0x20] = obj[0x20] + -1;
    }
    obj[0x1b] = 2;
  }
}
void  RenderView_SetViewport (int obj,int val)
{
  *(int *)(obj + 0x60) = val;
}
int  ChunkData_New (void)
{
  int iVar1;
  int iVar2;
  iVar1 =  MemAllocImpl (0xbc);
  iVar2 = 0;
  if (iVar1 != 0) {
    iVar2 =  ChunkData_GetDataPtr ();
    (*(code *)(iVar2 + 8))(iVar1);
    iVar2 = iVar1;
  }
  return iVar2;
}
void  TextInput_Kill (int obj)
{
  int iVar1;
  uint uVar2;
  int *piVar3;
  int *puVar4;
  if (*(int *)(obj + 0xc) != 0) {
    if (*(int *)(obj + 0xb4) != 0) {
      uVar2 = (uint)*(byte *)(obj + 0xac);
      puVar4 = (int *)(uVar2 * 4 + *(int *)(obj + 0xb4));
      if (uVar2 < uVar2 + *(byte *)(obj + 0xab)) {
        do {
          piVar3 = (int *)*puVar4;
          puVar4 = puVar4 + 1;
          /* vtable[20] */
          (*(code *)(*piVar3 + 0x50))();
          uVar2 = uVar2 + 1;
        } while ((int)uVar2 <
                 (int)((uint)*(byte *)(obj + 0xac) + (uint)*(byte *)(obj + 0xab)));
      }
    }
    iVar1 =  Label_GetVtable ();
    /* vtable[20] */
    (*(code *)(iVar1 + 0x50))(obj);
  }
}
void  Spu_SetSeqTrackPan (int obj,int val)
{
  *(int *)(obj + 0xcc) = val;
}
void  Font_Dispatch (int *obj,int *arg2,int arg3)
{
  int iVar1;
  code *pcVar2;
  uint uVar3;
  iVar1 =  GetCoordSystemVtable ();
  /* vtable[14] */
  (*(code *)(iVar1 + 0x38))(obj,arg2,arg3);
  uVar3 = *(uint *)*arg2 & 0xf;
  if (uVar3 == 1) {
    /* vtable[21] */
    pcVar2 = *(code **)(*obj + 0x54);
  }
  else if (uVar3 == 2) {
    /* vtable[22] */
    pcVar2 = *(code **)(*obj + 0x58);
  }
  else {
    if (uVar3 != 5) {
      return;
    }
    /* vtable[23] */
    pcVar2 = *(code **)(*obj + 0x5c);
  }
  (*pcVar2)(obj,arg2,arg3);
}
void  NavMenu_SetSelection (int *obj,int arg2,int arg3)
{
  int *piVar1;
  int *piVar2;
  if (obj[0x13] != 0) {
    piVar1 = *(int **)(obj[0x16] * 4 + obj[0x15]);
    piVar2 = *(int **)(arg2 * 4 + obj[0x15]);
    if (-1 < obj[0x16]) {
      /* vtable[46] */
      (*(code *)(*piVar1 + 0xb8))(piVar1,obj[0x13] + 0x10);
    }
    /* vtable[46] */
    (*(code *)(*piVar2 + 0xb8))(piVar2,obj[0x13] + 0x13);
    obj[0x16] = arg2;
    if (arg3 != 0) {
      /* vtable[28] */
      (*(code *)(*obj + 0x70))(obj,0);
    }
    /* vtable[24] */
    (*(code *)(*obj + 0x60))(obj,9);
  }
}
void  SpuVoice_Nop2 (void)
{
}
void  Font_SetMode (int *obj,int arg2)
{
  code *pcVar1;
  int iVar2;
  iVar2 = *obj;
  obj[8] = arg2;
  /* vtable[12] */
  (*(code *)(iVar2 + 0x30))();
  if (arg2 == 2) {
    pcVar1 = *(code **)(iVar2 + 100);
  }
  else {
    if (arg2 != 3) {
      return;
    }
    /* vtable[26] */
    pcVar1 = *(code **)(iVar2 + 0x68);
  }
  (*pcVar1)(obj);
}
int * Palette_GetTable (int obj)
{
  int *puVar1;
  if (*(int *)(obj + 0x78) == 0xf) {
    puVar1 = &Mem_TypeEntry8;
  }
  else {
    puVar1 = &UNK_8006ea90 + *(int *)(obj + 0x78) * 3;
  }
  return puVar1;
}
void  NavMenu_Init (int *obj,int *arg2)
{
  int iVar1;
  int *puVar2;
  int uVar3;
  int *piVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  obj[0x13] = (int)arg2;
  if (arg2 != NULL) {
    piVar4 = (int *)arg2[7];
    iVar7 = 0;
    while (iVar1 = *piVar4, piVar4 = piVar4 + 1, iVar1 != 0) {
      iVar7 = iVar7 + 1;
    }
    iVar5 = iVar7 << 2;
    puVar2 = (int *) MemAllocImpl (iVar5);
    obj[0x15] = (int)puVar2;
    iVar1 =  MemAllocImpl (iVar5);
    obj[0x17] = iVar1;
    iVar1 =  MemAllocImpl (iVar5);
    obj[0x18] = iVar1;
    iVar1 =  MemAllocImpl (iVar5);
    obj[0x19] = iVar1;
    obj[0x14] = iVar7;
    if (*arg2 == 0) {
      piVar4 = (int *)arg2[1];
    }
    else {
      piVar4 = (int *) Texture_New ();
      /* vtable[30] */
      (*(code *)(*piVar4 + 0x78))(piVar4);
      /* vtable[23] */
      (*(code *)(*piVar4 + 0x5c))(piVar4);
    }
    piVar6 = (int *)arg2[7];
    iVar7 = 0;
    iVar1 = *piVar6;
    while (iVar1 != 0) {
      iVar5 = *(int *)(iVar7 * 4 + arg2[9]);
      iVar1 = strlen((char *)*piVar6);
      uVar3 =  TextInput_New (piVar4,iVar1,*piVar6);
      *puVar2 = uVar3;
      puVar2 = puVar2 + 1;
      if (iVar5 != 0) {
        obj[0x16] = iVar7;
        /* vtable[62] */
        (*(code *)(*obj + 0xf8))(obj,iVar5,piVar4);
      }
      piVar6 = piVar6 + 1;
      iVar7 = iVar7 + 1;
      iVar1 = *piVar6;
    }
    iVar7 =  Particle_New (&Sys_VideoModeA8E8,&Sys_RegParam,0);
    obj[0x1a] = iVar7;
    arg2[1] = (int)piVar4;
  }
}
void  Spu_BranchSeqTrack (int *obj)
{
  if (obj[0x35] == 0) {
    /* vtable[24] */
    (*(code *)(*obj + 0x60))(obj,7);
  }
  else {
    (*(code *)(*(int *)obj[0x2d] + 0x4c))();
  }
}
void SetFogNear(long a,long h)
{
  if (h == 0) {
    trap(TRAP_DIVZERO);
  }
  if ((h == -1) && (a * -SCREEN_W == -0x80000000)) {
    trap(TRAP_OVERFLOW);
  }
  SetDQA((a * -SCREEN_W) / h);
  SetDQB(0x1400000);
}
void  NavMenu_Render (int *obj)
{
  int *piVar1;
  int iVar2;
  piVar1 = (int *)obj[6];
  iVar2 = *piVar1;
  /* vtable[56] */
  (*(code *)(*obj + 0xe0))(obj,obj[5]);
  (*(code *)(*(int *)obj[0x1e] + 0x4c))((int *)obj[0x1e],obj[5],0);
  if (obj[0x22] != 0) {
    /* vtable[57] */
    (*(code *)(*obj + 0xe4))(obj,obj + 0x24);
    (*(code *)(*(int *)obj[0x1e] + 0xb8))((int *)obj[0x1e],1,obj + 0x24);
  }
  if (obj[0x1d] == 0) {
    (*(code *)(**(int **)obj[3] + 0x78))(*(int **)obj[3],obj + 0x24,&Mem_TypeEntry1);
  }
  (*(code *)(**(int **)obj[3] + 0x78))(*(int **)obj[3],obj + 0x24,0);
  /* vtable[18] */
  (*(code *)(iVar2 + 0x48))(piVar1,obj[10]);
  /* vtable[19] */
  (*(code *)(iVar2 + 0x4c))(piVar1,obj[0xb]);
  /* vtable[20] */
  (*(code *)(iVar2 + 0x50))(piVar1,obj[0xc]);
  /* vtable[28] */
  (*(code *)(iVar2 + 0x70))(piVar1,obj[5],&Mem_TypeEntry5,&Mem_TypeEntry5,0);
  /* vtable[35] */
  (*(code *)(iVar2 + 0x8c))(piVar1);
  obj[0xe] = 0;
}
void  RenderView_AdvanceFrame (int *obj,int arg2,int arg3)
{
  obj[0x24] = obj[0x24] + 1;
  if (arg3 - 2U < 2) {
    /* vtable[39] */
    (*(code *)(*obj + 0x9c))();
  }
}
void  NavMenu_SetItemColor (int obj,int arg2)
{
  int *piVar1;
  piVar1 = *(int **)(obj + 0x48);
  if (piVar1 != NULL) {
    /* vtable[32] */
    (*(code *)(*piVar1 + 0x80))(piVar1,arg2,0x60,0x60);
  }
}
void  RenderView_InitOt (int obj)
{
  int iVar1;
  int iVar2;
  if (*(int *)(obj + 0x70) == 0) {
    iVar2 = (4 << (*(uint *)(obj + 0x3c) & 0x1f)) +
            *(int *)(obj + 0x48) * *(int *)(obj + 0x44) + 0x14;
    iVar1 =  MemAllocImpl (iVar2 * 2);
    if (iVar1 != 0) {
      *(int *)(obj + 0x78) = iVar1;
      *(int *)(obj + 0x80) = iVar1 + 0x14;
      *(int *)(obj + 0x88) = (4 << (*(uint *)(obj + 0x3c) & 0x1f)) + iVar1 + 0x14;
      *(int *)(obj + 0x7c) = iVar2 + *(int *)(obj + 0x78);
      *(int *)(obj + 0x84) = iVar2 + *(int *)(obj + 0x80);
      *(int *)(obj + 0x8c) = iVar2 + *(int *)(obj + 0x88);
      **(int **)(obj + 0x78) = *(int *)(obj + 0x3c);
      *(int *)(*(int *)(obj + 0x78) + 4) = *(int *)(obj + 0x80);
      **(int **)(obj + 0x7c) = *(int *)(obj + 0x3c);
      *(int *)(*(int *)(obj + 0x7c) + 4) = *(int *)(obj + 0x84);
      GsClearOt(0,0,*(GsOT **)(obj + 0x78));
      GsClearOt(0,0,*(GsOT **)(obj + 0x7c));
      *(int *)(obj + 0x70) = 1;
      *(int *)(obj + 0x74) = 0;
    }
  }
}
void GsGetLw(GsCOORDINATE2 *m,MATRIX *out)
{
  int iVar1;
  int uVar2;
  long lVar3;
  int uVar4;
  long lVar5;
  int iVar6;
  int uVar7;
  long lVar8;
  int iVar9;
  iVar9 = 0;
  iVar6 = 100;
  while( true ) {
    (&Gpu_TexClut2)[iVar9] = (int)m;
    iVar1 = Gpu_TexClut2;
    if (m->super == (struct _GsCOORDINATE2 *)0) break;
    if (m->flg == Gpu_UnknownB742) {
      uVar2 = *(int *)((m->workm).m[0] + 2);
      uVar4 = *(int *)((m->workm).m[1] + 1);
      uVar7 = *(int *)(m->workm).m[2];
      *(int *)out->m[0] = *(int *)(m->workm).m[0];
      *(int *)(out->m[0] + 2) = uVar2;
      *(int *)(out->m[1] + 1) = uVar4;
      *(int *)out->m[2] = uVar7;
      lVar3 = (m->workm).t[0];
      lVar5 = (m->workm).t[1];
      lVar8 = (m->workm).t[2];
      *(int *)(out->m[2] + 2) = *(int *)((m->workm).m[2] + 2);
      out->t[0] = lVar3;
      out->t[1] = lVar5;
      out->t[2] = lVar8;
      GS_133_OBJ_224();
      return;
    }
    if (m->flg == 0) {
      iVar6 = iVar9;
    }
    iVar9 = iVar9 + 1;
    m = m->super;
  }
  if ((m->flg != Gpu_UnknownB742) && (m->flg != 0)) {
    if (iVar6 == 100) {
      uVar2 = *(int *)(Gpu_TexClut2 + 0x28);
      uVar4 = *(int *)(Gpu_TexClut2 + 0x2c);
      uVar7 = *(int *)(Gpu_TexClut2 + 0x30);
      *(int *)out->m[0] = *(int *)(Gpu_TexClut2 + 0x24);
      *(int *)(out->m[0] + 2) = uVar2;
      *(int *)(out->m[1] + 1) = uVar4;
      *(int *)out->m[2] = uVar7;
      lVar3 = *(long *)(iVar1 + 0x38);
      lVar5 = *(long *)(iVar1 + 0x3c);
      lVar8 = *(long *)(iVar1 + 0x40);
      *(int *)(out->m[2] + 2) = *(int *)(iVar1 + 0x34);
      out->t[0] = lVar3;
      out->t[1] = lVar5;
      out->t[2] = lVar8;
      GS_133_OBJ_224();
      return;
    }
    iVar6 = (&Gpu_TexClut2)[iVar6 + 1];
    uVar2 = *(int *)(iVar6 + 0x28);
    uVar4 = *(int *)(iVar6 + 0x2c);
    uVar7 = *(int *)(iVar6 + 0x30);
    *(int *)out->m[0] = *(int *)(iVar6 + 0x24);
    *(int *)(out->m[0] + 2) = uVar2;
    *(int *)(out->m[1] + 1) = uVar4;
    *(int *)out->m[2] = uVar7;
    lVar3 = *(long *)(iVar6 + 0x38);
    lVar5 = *(long *)(iVar6 + 0x3c);
    lVar8 = *(long *)(iVar6 + 0x40);
    *(int *)(out->m[2] + 2) = *(int *)(iVar6 + 0x34);
    out->t[0] = lVar3;
    out->t[1] = lVar5;
    out->t[2] = lVar8;
    GS_133_OBJ_224();
    return;
  }
  uVar2 = *(int *)((m->coord).m[0] + 2);
  uVar4 = *(int *)((m->coord).m[1] + 1);
  uVar7 = *(int *)(m->coord).m[2];
  *(int *)(m->workm).m[0] = *(int *)(m->coord).m[0];
  *(int *)((m->workm).m[0] + 2) = uVar2;
  *(int *)((m->workm).m[1] + 1) = uVar4;
  *(int *)(m->workm).m[2] = uVar7;
  lVar3 = (m->coord).t[0];
  lVar5 = (m->coord).t[1];
  lVar8 = (m->coord).t[2];
  *(int *)((m->workm).m[2] + 2) = *(int *)((m->coord).m[2] + 2);
  (m->workm).t[0] = lVar3;
  (m->workm).t[1] = lVar5;
  (m->workm).t[2] = lVar8;
  uVar2 = *(int *)((m->workm).m[0] + 2);
  uVar4 = *(int *)((m->workm).m[1] + 1);
  uVar7 = *(int *)(m->workm).m[2];
  *(int *)out->m[0] = *(int *)(m->workm).m[0];
  *(int *)(out->m[0] + 2) = uVar2;
  *(int *)(out->m[1] + 1) = uVar4;
  *(int *)out->m[2] = uVar7;
  lVar3 = (m->workm).t[0];
  lVar5 = (m->workm).t[1];
  lVar8 = (m->workm).t[2];
  *(int *)(out->m[2] + 2) = *(int *)((m->workm).m[2] + 2);
  out->t[0] = lVar3;
  out->t[1] = lVar5;
  out->t[2] = lVar8;
  m->flg = Gpu_UnknownB742;
  GS_133_OBJ_224();
}
int * NavMenu_GetDataPtr (void)
{
  return (int *)NavMenu_PageVtable;
}
void  NavMenu_HandleState (int *obj,int arg2,int arg3)
{
  code *pcVar1;
  int iVar2;
  iVar2 = *obj;
  if (obj[0xf] != 0) {
    switch(arg3) {
    case 0x12:
      /* vtable[32] */
      pcVar1 = *(code **)(iVar2 + 0x80);
      break;
    case 0x13:
      /* vtable[33] */
      pcVar1 = *(code **)(iVar2 + 0x84);
      break;
    default:
      goto switchD_8003c4c4_caseD_14;
    case 0x17:
      /* vtable[31] */
      pcVar1 = *(code **)(iVar2 + 0x7c);
      break;
    case 0x19:
      /* vtable[30] */
      pcVar1 = *(code **)(iVar2 + 0x78);
      break;
    case 0x21:
      /* vtable[29] */
      pcVar1 = *(code **)(iVar2 + 0x74);
    }
    (*pcVar1)();
  }
switchD_8003c4c4_caseD_14:
}
void  Entity_SetColor (int obj,int *arg2)
{
  int uVar1;
  if (*(int *)(obj + 0xc) != 0) {
    uVar1 = arg2[1];
    *(int *)(obj + 0x50) = *arg2;
    *(int *)(obj + 0x54) = uVar1;
  }
}
void  SpuVoice_SetDimensions (int obj,int *arg2)
{
  int uVar1;
  uVar1 = arg2[1];
  *(int *)(obj + 0x34) = *arg2;
  *(int *)(obj + 0x38) = uVar1;
}
void  Palette_Restore (int obj)
{
  *(int *)(obj + 0x50) = *(int *)(obj + 0x90);
  *(int *)(obj + 0x54) = *(int *)(obj + 0x94);
  *(u16 *)(obj + 0x60) = *(u16 *)(obj + 0x88);
  *(u16 *)(obj + 0x62) = *(u16 *)(obj + 0x8c);
}
void  NavMenu_NavigateToItem (int *obj,int arg2,int arg3)
{
  int *piVar1;
  int iVar2;
  int *piVar3;
  iVar2 = obj[0x16] * 4;
  piVar1 = *(int **)(*(int *)(iVar2 + obj[0x18]) * 4 + *(int *)(iVar2 + obj[0x19]));
  piVar3 = *(int **)(arg2 * 4 + *(int *)(iVar2 + obj[0x19]));
  /* vtable[46] */
  (*(code *)(*piVar1 + 0xb8))(piVar1,obj[0x13] + 0x10);
  /* vtable[46] */
  (*(code *)(*piVar3 + 0xb8))(piVar3,*(int *)(iVar2 + *(int *)(obj[0x13] + 0x24)) + 8);
  *(int *)(iVar2 + obj[0x18]) = arg2;
  if (arg3 != 0) {
    /* vtable[28] */
    (*(code *)(*obj + 0x70))(obj,0);
  }
  /* vtable[24] */
  (*(code *)(*obj + 0x60))(obj,9);
}
int  NavMenu_GetEntryData (int obj)
{
  return *(int *)(*(int *)(obj + 0x58) * 4 + *(int *)(obj + 0x60));
}
void  SpuVoice_SetUnk48 (int obj,int val)
{
  if (*(int *)(obj + 0x70) == 0) {
    *(int *)(obj + 0x48) = val;
  }
}
void  NavMenu_Next (int *obj)
{
  code *pcVar1;
  if (obj[0x13] != 0) {
    if (obj[0xf] == 1) {
      /* vtable[59] */
      pcVar1 = *(code **)(*obj + 0xec);
    }
    else {
      if (obj[0xf] != 2) {
        return;
      }
      /* vtable[70] */
      pcVar1 = *(code **)(*obj + 0x118);
    }
    (*pcVar1)();
  }
}
int  NavMenu_Advance (int *obj)
{
  int iVar1;
  iVar1 = 1;
  if (((code *)obj[0x23] == (code *)0x0) || (iVar1 = (*(code *)obj[0x23])(), iVar1 != 0)) {
    /* vtable[24] */
    (*(code *)(*obj + 0x60))(obj,8);
  }
  return iVar1;
}
void  NavMenu_SelectPrev (int *obj)
{
  int iVar1;
  if (obj[0x13] != 0) {
    iVar1 = obj[0x16];
    do {
      iVar1 = iVar1 + -1;
      if (iVar1 < 0) {
        iVar1 = obj[0x14] + -1;
      }
    } while ((iVar1 != obj[0x16]) && (*(int *)(iVar1 * 4 + *(int *)(obj[0x13] + 0x18)) != 0)
            );
    /* vtable[60] */
    (*(code *)(*obj + 0xf0))(obj,iVar1,1);
  }
}
void  Spu_CheckSeqTrack (int *obj,int arg2,int arg3)
{
  int iVar1;
  iVar1 =  NavMenu_GetDataPtr ();
  /* vtable[23] */
  (*(code *)(iVar1 + 0x5c))(obj,arg2,arg3);
  if (obj[0x29] == 0) {
    iVar1 = (*(code *)(*(int *)obj[0x2d] + 0x48))();
    obj[0x29] = iVar1;
    if ((iVar1 != 0) && (obj[0x36] == 0)) {
      /* vtable[24] */
      (*(code *)(*obj + 0x60))(obj,7);
    }
  }
}
void  RenderView_RenderScene (int *obj)
{
  uint uVar1;
  int uVar2;
  if (obj[0x1c] != 0) {
    if (*(int *)(obj[4] + 0xc) != 0) {
      /* vtable[40] */
      (*(code *)(*obj + 0xa0))();
    }
     RenderView_SetGeomScreen (obj[0x10]);
     RenderView_SetClipNear (obj[0x13]);
    GsSetLightMode(obj[0x15]);
    if ((obj[0x15] == 1) || (obj[0x15] == 3)) {
      SetFarColor((uint)*(byte *)((int)obj + 0x5b),(uint)*(byte *)(obj + 0x17),
                  (uint)*(byte *)((int)obj + 0x5d));
      SetFogNear(obj[0x18],obj[0x10]);
    }
    GsSetRefView2((GsRVIEW2 *)(obj + 5));
    *(int *)obj[0xc] = 0;
    uVar1 = 1 << (obj[0xf] & 0x1fU);
    if (uVar1 == 0) {
      trap(TRAP_DIVZERO);
    }
    obj[0x26] = (uint)(obj[0x14] - obj[0x13]) / uVar1 + 1;
     RenderView_SetClipFar (obj[obj[0x1d] + 0x22]);
    GsClearOt(0,0,(GsOT *)obj[obj[0x1d] + 0x1e]);
    /* vtable[40] */
    (*(code *)(*obj + 0xa0))(obj,obj[0x2b]);
    if (obj[4] != 0) {
      uVar2 =  RenderView_GetTailNode (0);
      /* vtable[40] */
      (*(code *)(*obj + 0xa0))(obj,uVar2);
    }
  }
}
void  RenderView_FreeOt (int obj)
{
  if (*(int *)(obj + 0x70) != 0) {
    DrawSync(0);
     MemFreeImpl (*(int *)(obj + 0x78));
    *(int *)(obj + 0x70) = 0;
  }
}
void  NavMenu_SetField (int obj,int val)
{
  *(int *)(obj + 0x84) = val;
}
int  Entity_TestFlag (int obj,int arg2)
{
  int iVar1;
  iVar1 =  MathBitfieldOp (obj + 0x58,0x1f,1,arg2 == 0);
  return iVar1 == 0;
}
void  SpuVoice_Nop1 (void)
{
}
void  Primitive_Construct (int *obj,int arg2,int arg3,int arg4)
{
  int iVar1;
  iVar1 =  GetPrimitiveBaseTable ();
  (*(code *)(iVar1 + 8))(obj);
  iVar1 =  Primitive_GetVtable ();
  *obj = iVar1;
  /* vtable[16] */
  (*(code *)(iVar1 + 0x40))(obj,arg2,arg3,arg4);
}
void  Sprite_Init (int *obj,u16 *arg2,int *arg3,int arg4)
{
  obj[0x11] = arg4;
  obj[0x12] = 1;
  obj[0x13] = 0;
  obj[0x16] = 0;
  *(u16 *)(obj + 0x17) = 0;
  *(u16 *)((int)obj + 0x5e) = 0;
  *(u16 *)(obj + 0x18) = *arg2;
  *(u16 *)((int)obj + 0x62) = arg2[2];
  if (arg3 == NULL) {
    arg3 = &Sys_UnknownA924;
  }
  /* vtable[46] */
  (*(code *)(*obj + 0xb8))(obj,1,arg3);
  /* vtable[51] */
  (*(code *)(*obj + 0xcc))(obj,0xd);
}
/* WARNING: Removing unreachable block (ram,0x800401e8) */
int  Entity_SetupAnimation (int *obj,int arg2,int arg3,int arg4)
{
  int iVar1;
  int iVar2;
  int uVar3;
  int iVar4;
  iVar2 = *obj;
  if (arg3 < 0) {
    arg3 = obj[0x1c];
  }
  else {
    obj[0x1c] = arg3;
  }
  uVar3 = 1;
  if (arg3 == 0) {
    uVar3 = 2;
    obj[0x1e] = 0xf;
  }
  else {
    obj[0x1e] = arg3;
  }
  obj[0x1e] = arg3;
  if (arg3 == 0) {
    obj[0x1e] = 0xf;
  }
  iVar4 = DEFAULT_SCALE / obj[0x1d];
  if (obj[0x1d] == 0) {
    trap(TRAP_DIVZERO);
  }
  obj[0x1f] = arg4;
  obj[0x20] = iVar4;
  if (obj[0x26] != 0) {
    iVar1 = obj[0x27];
    if (iVar1 == 0) {
      trap(TRAP_DIVZERO);
    }
    if ((iVar1 == -1) && (iVar4 == -0x80000000)) {
      trap(TRAP_OVERFLOW);
    }
    obj[0x20] = iVar4 - iVar4 / iVar1;
  }
  iVar4 = obj[0x20];
  if (iVar4 == 0) {
    trap(TRAP_DIVZERO);
  }
  if ((iVar4 == -1) && (obj[0x1a] == -0x80000000)) {
    trap(TRAP_OVERFLOW);
  }
  obj[0x21] = obj[0x1a] / iVar4;
  /* vtable[4] */
  (*(code *)(iVar2 + 0x10))(obj);
  (*(code *)(iVar2 + 100))(obj,1);
  /* vtable[26] */
  (*(code *)(iVar2 + 0x68))(obj,uVar3);
  /* vtable[24] */
  (*(code *)(iVar2 + 0x60))(obj,1);
  return arg3;
}
void  Sprite_SetTexture (int *obj,int arg2,int arg3)
{
  int iVar1;
  if (obj[3] == 0) {
    iVar1 =  GetPrimitiveBaseTable ();
    /* vtable[19] */
    (*(code *)(iVar1 + 0x4c))(obj,arg2,0);
    /* vtable[47] */
    (*(code *)(*obj + 0xbc))(obj,arg3);
  }
}
void  NavMenu_SelectState (int *obj)
{
  int *piVar1;
  int iVar2;
  if (obj[0xf] == 1) {
    iVar2 = obj[0x16];
    /* vtable[64] */
    (*(code *)(*obj + 0x100))(obj,obj[5],1);
    iVar2 = iVar2 * 4;
    piVar1 = *(int **)(*(int *)(iVar2 + obj[0x18]) * 4 + *(int *)(iVar2 + obj[0x19]));
    /* vtable[46] */
    (*(code *)(*piVar1 + 0xb8))(piVar1,*(int *)(iVar2 + *(int *)(obj[0x13] + 0x24)) + 8);
    obj[0xf] = 2;
    /* vtable[24] */
    (*(code *)(*obj + 0x60))(obj,0xe);
  }
}
void  Entity_InitState (int *obj,int arg2)
{
  obj[0x1c] = arg2;
  obj[0x1b] = 0;
  obj[0x1d] = 10;
  obj[0x1e] = 0;
  obj[0x1f] = 0;
  /* vtable[24] */
  (*(code *)(*obj + 0x60))(obj,0);
  (*(code *)(*obj + 100))(obj,0);
  obj[0x26] = 0;
}
void  Spu_SetSeqTrackReverb (int obj,int val)
{
  *(int *)(obj + 0xd4) = val;
}

