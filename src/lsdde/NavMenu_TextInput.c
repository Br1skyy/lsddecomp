/* 2C054_part2.c -- split from 2C054.c */
#include "LateLink_Shared.h"
extern u32 D_8006E878;  /* Font vtable data (defined in asm data section) */
extern u32 D_8006E854;  /* TypeEntry1 vtable data */
extern u32 D_8006E8E4;  /* ChunkData vtable data */
void  NavMenu_SetOption2 (int *obj,int arg2)
{
  if (arg2 == 0) {
    obj[0x23] = 0;
  }
  else if (arg2 == 1) {
    obj[0x23] = *(int *)(*obj + 0xc4);
  }
}
void  Font_EventProc (int obj,int arg2,int arg3)
{
  int *piVar1;
  if (arg3 == 2) {
    (**(code **)(**(int **)(obj + 0x10) + 0x44))();
    piVar1 = *(int **)(*(int *)(obj + 0xc) + 4);
    /* vtable[17] */
    (**(code **)(*piVar1 + 0x44))(piVar1);
    /* vtable[18] */
    (**(code **)(*piVar1 + 0x48))(piVar1);
  }
}
/* Possible GS_101.OBJ/GsSetNearClip
   Possible GS_102.OBJ/GsSetFarClip
   Possible GS_124.OBJ/GsSetWorkBase */
void  RenderView_SetClipFar (int obj)
{
  Gpu_CurPrimPtr = obj;
}
int GsSetRefView2(GsRVIEW2 *pv)
{
  long lVar1;
  int iVar2;
  long lVar3;
  int iVar4;
  int local_b0;
  int local_ac;
  int local_a8;
  int local_a4;
  int local_a0;
  int local_9c;
  u8 auStack_90 [32];
  MATRIX local_70 [2];
  VECTOR local_30;
  Spu_ChanAttr12 = Spu_ChanAttr21;
  Spu_ChanAttr13 = Spu_ChanAttr22;
  Spu_ChanAttr14 = Spu_ChanAttr23;
  Spu_ChanAttr15 = Spu_ChanAttr24;
  Spu_ChanAttr16 = Spu_ChanAttr25;
  Spu_ChanAttr17 = Spu_ChanAttr26;
  Spu_ChanAttr18 = Spu_ChanAttr27;
  Spu_ChanAttr19 = Spu_ChanAttr28;
  gte_rotate_z_matrix((MATRIX *)&Spu_ChanAttr12,-pv->rz);
  scale_view_param(pv,&local_b0);
  lVar1 = SquareRoot0((local_a4 - local_b0) * (local_a4 - local_b0) +
                      (local_a0 - local_ac) * (local_a0 - local_ac) +
                      (local_9c - local_a8) * (local_9c - local_a8));
  iVar2 = 1;
  if (lVar1 != 0) {
    iVar2 = (local_ac - local_a0) * FIXED_12;
    if (lVar1 == 0) {
      trap(TRAP_DIVZERO);
    }
    if ((lVar1 == -1) && (iVar2 == -0x80000000)) {
      trap(TRAP_OVERFLOW);
    }
    lVar3 = SquareRoot0((local_a4 - local_b0) * (local_a4 - local_b0) +
                        (local_9c - local_a8) * (local_9c - local_a8));
    if (lVar1 == 0) {
      trap(TRAP_DIVZERO);
    }
    if ((lVar1 == -1) && (lVar3 << 0xc == -0x80000000)) {
      trap(TRAP_OVERFLOW);
    }
    Gssub_make_matrix((MATRIX *)auStack_90,(int)(short)-(short)(iVar2 / lVar1),
                      (int)(short)((lVar3 << 0xc) / lVar1),0x78);
    MulMatrix((MATRIX *)&Spu_ChanAttr12,(MATRIX *)auStack_90);
    if (lVar3 != 0) {
      iVar2 = (local_a4 - local_b0) * FIXED_12;
      if (lVar3 == 0) {
        trap(TRAP_DIVZERO);
      }
      if ((lVar3 == -1) && (iVar2 == -0x80000000)) {
        trap(TRAP_OVERFLOW);
      }
      iVar4 = (local_9c - local_a8) * FIXED_12;
      if (lVar3 == 0) {
        trap(TRAP_DIVZERO);
      }
      if ((lVar3 == -1) && (iVar4 == -0x80000000)) {
        trap(TRAP_OVERFLOW);
      }
      Gssub_make_matrix((MATRIX *)auStack_90,(int)(short)-(short)(iVar2 / lVar3),
                        (int)(short)(iVar4 / lVar3),0x79);
      MulMatrix((MATRIX *)&Spu_ChanAttr12,(MATRIX *)auStack_90);
    }
    local_30.vx = -pv->vpx;
    local_30.vy = -pv->vpy;
    local_30.vz = -pv->vpz;
    ApplyMatrixLV((MATRIX *)&Spu_ChanAttr12,&local_30,(VECTOR *)&Spu_ChanAttr17);
    if (pv->super != (GsCOORDINATE2 *)0x0) {
      GsGetLw(pv->super,(MATRIX *)auStack_90);
      TransposeMatrix((MATRIX *)auStack_90,local_70);
      ApplyMatrixLV(local_70,(VECTOR *)(auStack_90 + 0x14),&local_30);
      local_70[0].t[0] = -local_30.vx;
      local_70[0].t[2] = -local_30.vz;
      local_70[0].t[1] = -local_30.vy;
      GsMulCoord2((MATRIX *)&Spu_ChanAttr12,local_70);
      *(short *)((char *)&(Spu_ChanAttr12) + 0) = local_70[0].m[0][0];
      *(short *)((char *)&(Spu_ChanAttr12) + 2) = local_70[0].m[0][1];
      *(short *)((char *)&(Spu_ChanAttr13) + 0) = local_70[0].m[0][2];
      *(short *)((char *)&(Spu_ChanAttr13) + 2) = local_70[0].m[1][0];
      *(short *)((char *)&(Spu_ChanAttr14) + 0) = local_70[0].m[1][1];
      *(short *)((char *)&(Spu_ChanAttr14) + 2) = local_70[0].m[1][2];
      *(short *)((char *)&(Spu_ChanAttr15) + 0) = local_70[0].m[2][0];
      *(short *)((char *)&(Spu_ChanAttr15) + 2) = local_70[0].m[2][1];
      *(short *)((char *)&(Spu_ChanAttr16) + 0) = local_70[0].m[2][2];
      *(short *)((char *)&(Spu_ChanAttr16) + 2) = *(short *)((char *)&(local_70[0]) + 18);
      Spu_ChanAttr17 = local_70[0].t[0];
      Spu_ChanAttr18 = local_70[0].t[1];
      Spu_ChanAttr19 = local_70[0].t[2];
    }
    *(short *)((char *)&(Gpu_OTAddr) + 0) = (short)Spu_ChanAttr12;
    *(short *)((char *)&(Gpu_OTAddr) + 2) = *(short *)((char *)&(Spu_ChanAttr12) + 2);
    *(short *)((char *)&(Gpu_OTSz) + 0) = (short)Spu_ChanAttr13;
    *(short *)((char *)&(Gpu_OTSz) + 2) = *(short *)((char *)&(Spu_ChanAttr13) + 2);
    *(short *)((char *)&(Gpu_PrimAddr) + 0) = (short)Spu_ChanAttr14;
    *(short *)((char *)&(Gpu_PrimAddr) + 2) = *(short *)((char *)&(Spu_ChanAttr14) + 2);
    *(short *)((char *)&(Gpu_DrawId) + 0) = (short)Spu_ChanAttr15;
    *(short *)((char *)&(Gpu_DrawId) + 2) = *(short *)((char *)&(Spu_ChanAttr15) + 2);
    *(short *)((char *)&(Gpu_DispBufIdx) + 0) = (short)Spu_ChanAttr16;
    *(short *)((char *)&(Gpu_DispBufIdx) + 2) = *(short *)((char *)&(Spu_ChanAttr16) + 2);
    Gpu_IrqFlag = Spu_ChanAttr17;
    Gpu_DmaAddr = Spu_ChanAttr18;
    Gpu_DmaLen = Spu_ChanAttr19;
    iVar2 = 0;
  }
  return iVar2;
}
void GsSetLightMode(int mode)
{
  if (mode != 1) {
    if (mode < 2) {
      if (mode != 0) {
        GS_108_OBJ_6C();
        return;
      }
      Gpu_TexDisableFlag = 0;
      GS_108_OBJ_7C();
      return;
    }
    if ((mode != 2) && (mode != 3)) {
      GS_108_OBJ_6C();
      return;
    }
  }
  Gpu_TexDisableFlag = mode;
  GS_108_OBJ_7C();
}
void  Font_Init (int *obj,int *arg2,int arg3)
{
  int iVar1;
  int *piVar2;
  int iVar3;
  iVar1 = arg2[2];
  iVar3 = *obj;
  if (iVar1 == 0) {
    iVar1 =  StageGrid_New ();
  }
  obj[4] = iVar1;
  iVar1 = arg2[3];
  if (iVar1 == 0) {
    iVar1 =  StageRow_New ();
  }
  obj[5] = iVar1;
  iVar1 = arg2[4];
  if (iVar1 == 0) {
    iVar1 =  ChunkData_New ();
  }
  obj[6] = iVar1;
  obj[3] = (int)arg2;
  piVar2 = (int *)obj[6];
  /* vtable[4] */
  (**(code **)(iVar3 + 0x10))(obj,*arg2);
  /* vtable[4] */
  (**(code **)(iVar3 + 0x10))(obj,arg2[1]);
  /* vtable[4] */
  (**(code **)(iVar3 + 0x10))(obj,obj[4]);
  /* vtable[19] */
  (**(code **)(iVar3 + 0x4c))(obj,0,0,0);
  obj[9] = arg3;
  if (arg3 == 0) {
    /* vtable[4] */
    (**(code **)(*piVar2 + 0x10))(piVar2,*arg2);
    /* vtable[4] */
    (**(code **)(*piVar2 + 0x10))(piVar2,obj[4]);
    (**(code **)(*(int *)obj[5] + 0x10))((int *)obj[5],obj[4]);
    /* vtable[24] */
    (**(code **)(iVar3 + 0x60))(obj,2);
    /* vtable[18] */
    (**(code **)(iVar3 + 0x48))(obj);
  }
}
int select_max_param(int *v)
{
  int iVar1;
  int iVar2;
  iVar1 = *v;
  iVar2 = v[1];
  if (iVar1 < 0) {
    iVar1 = -iVar1;
  }
  if (iVar2 < 0) {
    iVar2 = -iVar2;
  }
  if (iVar1 < iVar2) {
    iVar1 = iVar2;
  }
  iVar2 = v[2];
  if (iVar2 < 0) {
    iVar2 = -iVar2;
  }
  if (iVar1 < iVar2) {
    iVar1 = iVar2;
  }
  iVar2 = v[3];
  if (iVar2 < 0) {
    iVar2 = -iVar2;
  }
  if (iVar1 < iVar2) {
    iVar1 = iVar2;
  }
  iVar2 = v[4];
  if (iVar2 < 0) {
    iVar2 = -iVar2;
  }
  if (iVar1 < iVar2) {
    iVar1 = iVar2;
  }
  iVar2 = v[5];
  if (iVar2 < 0) {
    iVar2 = -iVar2;
  }
  if (iVar1 < iVar2) {
    iVar1 = iVar2;
  }
  return iVar1;
}
void  NavMenu_PagePrev (int *obj)
{
  int iVar1;
  iVar1 = *(int *)(obj[0x16] * 4 + obj[0x18]) + -1;
  if (iVar1 < 0) {
    iVar1 = *(int *)(obj[0x16] * 4 + obj[0x17]) + -1;
  }
  /* vtable[71] */
  (**(code **)(*obj + 0x11c))(obj,iVar1,1);
}
int len_param(int x)
{
  int iVar1;
  iVar1 = 0;
  for (; 0 < x; x = x >> 1) {
    iVar1 = iVar1 + 1;
  }
  return iVar1;
}
void GsClearOt(ushort offset,ushort point,GsOT *otp)
{
  /* Provided by libgs.a */
}
void  RenderView_StubNop (void)
{
}
void  TextInput_SetSpacing (int obj,int val)
{
  *(int *)(obj + 0xb0) = val;
}
int  Spu_NewSeqTrack (int obj,int arg2,int arg3,int arg4)
{
  int iVar1;
  int iVar2;
  iVar1 =  MemAllocImpl (0xdc);
  iVar2 = 0;
  if (iVar1 != 0) {
    iVar2 =  Spu_GetSeqTrackVtable ();
    (**(code **)(iVar2 + 8))(iVar1,obj,arg2,arg3,arg4);
    iVar2 = iVar1;
  }
  return iVar2;
}
void  TextInput_SetChar (int obj,u8 arg2,int arg3)
{
  int *piVar1;
  piVar1 = *(int **)(arg3 * 4 + *(int *)(obj + 0xb4));
  /* vtable[49] */
  (**(code **)(*piVar1 + 0xc4))(piVar1,arg2);
}
void  ChunkDataObj_RemoveChild (int obj,int *arg2)
{
  int iVar1;
  if ((*(uint *)*arg2 & 0xf) == 4) {
    *(int *)(obj + 0x30) = 0;
    *(int *)(obj + 0x10) = 0;
  }
  else if ((*(uint *)*arg2 & 0xf) == 1) {
    *(int *)(obj + 0xc) = 0;
  }
  iVar1 =  GetCoordSystemVtable ();
  /* vtable[5] */
  (**(code **)(iVar1 + 0x14))(obj,arg2);
}
int * TextInput_GetVtable (void)
{
  return &Memory_ObjType3Vtable;
}
void  SpuVoice_SetUnk40 (int obj,int val)
{
  *(int *)(obj + 0x40) = val;
}
void  NavMenu_SetColor (int obj,u8 *arg2,u8 *arg3,u8 *arg4)
{
  char uVar1;
  char uVar2;
  uVar1 = arg2[1];
  uVar2 = arg2[2];
  *(u8 *)(obj + 0x90) = *arg2;
  *(u8 *)(obj + 0x91) = uVar1;
  *(u8 *)(obj + 0x92) = uVar2;
  uVar1 = arg3[1];
  uVar2 = arg3[2];
  *(u8 *)(obj + 0x93) = *arg3;
  *(u8 *)(obj + 0x94) = uVar1;
  *(u8 *)(obj + 0x95) = uVar2;
  uVar1 = arg4[1];
  uVar2 = arg4[2];
  *(u8 *)(obj + 0x96) = *arg4;
  *(u8 *)(obj + 0x97) = uVar1;
  *(u8 *)(obj + 0x98) = uVar2;
}
int  NavMenu_Check (int *obj)
{
  int iVar1;
  iVar1 = 1;
  if ((code *)obj[0x22] != (code *)0x0) {
    iVar1 = (*(code *)obj[0x22])();
  }
  if (iVar1 != 0) {
    /* vtable[24] */
    (**(code **)(*obj + 0x60))(obj,5);
  }
  return iVar1;
}
void  Font_Construct (int *obj)
{
  int iVar1;
  iVar1 =  GetCoordSystemVtable ();
  (**(code **)(iVar1 + 8))(obj);
  iVar1 =  Font_GetDataPtr ();
  *obj = iVar1;
  /* vtable[16] */
  (**(code **)(iVar1 + 0x40))(obj);
}
void  TextInput_Draw (int obj,int arg2,int *arg3)
{
  int iVar1;
  uint uVar2;
  int *puVar3;
  int local_18;
  int local_14;
  if (*(int *)(obj + 0xc) == 0) {
    iVar1 =  Label_GetVtable ();
    /* vtable[19] */
    (**(code **)(iVar1 + 0x4c))(obj,arg2,arg3);
    local_18 = *arg3;
    local_14 = arg3[1];
    uVar2 = (uint)*(byte *)(obj + 0xac);
    puVar3 = (int *)(uVar2 * 4 + *(int *)(obj + 0xb4));
    if (uVar2 < uVar2 + *(byte *)(obj + 0xab)) {
      do {
        if ((*(byte *)(obj + 0xaa) != 0) && (uVar2 == *(byte *)(obj + 0xaa))) {
          local_18 = local_18 + 0x10;
        }
        (**(code **)(*(int *)*puVar3 + 0x4c))((int *)*puVar3,obj,&local_18);
        local_18 = local_18 + *(int *)(obj + 0xb0);
        uVar2 = uVar2 + 1;
        puVar3 = puVar3 + 1;
      } while ((int)uVar2 < (int)((uint)*(byte *)(obj + 0xac) + (uint)*(byte *)(obj + 0xab))
              );
    }
  }
}
void  RenderView_SetRotation (int obj,int *arg2)
{
  int uVar1;
  int uVar2;
  if (*(int *)(obj + 0x10) != 0) {
    uVar1 = arg2[1];
    uVar2 = arg2[2];
    *(int *)(obj + 0x14) = *arg2;
    *(int *)(obj + 0x18) = uVar1;
    *(int *)(obj + 0x1c) = uVar2;
  }
}
void  NavMenu_Update (int obj)
{
  int *piVar1;
  piVar1 = *(int **)(obj + 0x18);
  /* vtable[36] */
  (**(code **)(*piVar1 + 0x90))(piVar1);
  /* vtable[29] */
  (**(code **)(*piVar1 + 0x74))(piVar1);
  (**(code **)(**(int **)(obj + 0x78) + 0x50))();
  if (*(int *)(obj + 0x34) != 0) {
    (**(code **)(*(int *)**(int **)(obj + 0xc) + 0x78))
              ((int *)**(int **)(obj + 0xc),obj + 0x93,0);
  }
}
void  Entity_SetFrame (int obj,int val)
{
  *(int *)(obj + 0x74) = val;
}
void  NavMenu_SetPosition (int *obj,int arg2)
{
  int iVar1;
  int *puVar2;
  int iVar3;
  if (obj[0x13] != 0) {
    puVar2 = (int *)obj[0x15];
    iVar3 = obj[0x16];
    iVar1 = 0;
    if (0 < obj[0x14]) {
      do {
        (**(code **)(*(int *)*puVar2 + 0xb8))((int *)*puVar2,arg2);
        puVar2 = puVar2 + 1;
        if (*(int *)(iVar1 * 4 + *(int *)(obj[0x13] + 0x24)) != 0) {
          obj[0x16] = iVar1;
          /* vtable[65] */
          (**(code **)(*obj + 0x104))(obj,arg2);
        }
        iVar1 = iVar1 + 1;
      } while (iVar1 < obj[0x14]);
    }
    obj[0x16] = iVar3;
  }
}
void  Entity_ApplyColor (int obj,int arg2,int arg3)
{
   Color_AddOrSet (obj,obj + 100,arg3,arg2);
}
void  Spu_SetSeqTrackPitch (int obj,int val)
{
  *(int *)(obj + 0xc4) = val;
}
void  TextInput_Activate (int *obj,int arg2)
{
  /* vtable[53] */
  (**(code **)(*obj + 0xd4))(obj,7);
  /* vtable[51] */
  (**(code **)(*obj + 0xcc))(obj,arg2);
}
void  Spu_UpdateSeqTrack (int *obj)
{
  int iVar1;
  iVar1 =  NavMenu_GetDataPtr ();
  /* vtable[19] */
  (**(code **)(iVar1 + 0x4c))(obj);
  obj[0x29] = 0;
  (**(code **)(*(int *)obj[0x2d] + 0x6c))((int *)obj[0x2d],obj[0x30]);
  iVar1 = (**(code **)(*(int *)obj[0x2d] + 0x40))
                    (obj[0x2d],obj[0x2e],obj[0x2f],obj[0x31],obj[0x32]);
  if (iVar1 != 0) {
    /* vtable[27] */
    (**(code **)(*obj + 0x6c))(obj,0);
  }
}
char * Sjis_EncodeFromAscii (u8 *obj,byte *arg2)
{
  byte bVar1;
  char cVar2;
  char uVar3;
  char *pcVar4;
  bVar1 = *arg2;
  while (bVar1 != 0) {
    pcVar4 = obj + 1;
    uVar3 = 0x81;
    if (0x2f < *arg2) {
      uVar3 = 0x82;
    }
    *obj = uVar3;
    bVar1 = *arg2;
    obj = obj + 2;
    if ((0x5f < bVar1) || (cVar2 = '\x1f', bVar1 == 0x20)) {
      cVar2 = ' ';
    }
    arg2 = arg2 + 1;
    *pcVar4 = bVar1 + cVar2;
    bVar1 = *arg2;
  }
  *obj = 0;
  return obj;
}
void  TextInput_PrintString (int obj,char *arg2)
{
  char cVar1;
  int *puVar2;
  puVar2 = *(int **)(obj + 0xb4);
  if (arg2 != (char *)0x0) {
    cVar1 = *arg2;
    while (cVar1 != '\0') {
      cVar1 = *arg2;
      arg2 = arg2 + 1;
      (**(code **)(*(int *)*puVar2 + 0xc4))((int *)*puVar2,cVar1);
      puVar2 = puVar2 + 1;
      cVar1 = *arg2;
    }
  }
}
void scale_view_param(int *src,int *dst)
{
  int uVar1;
  int iVar2;
  uint uVar3;
  uVar1 = select_max_param(NULL);
  iVar2 = len_param(uVar1);
  uVar3 = iVar2 - 0xf;
  if (0xf < iVar2) {
    *dst = *src >> (uVar3 & 0x1f);
    dst[1] = src[1] >> (uVar3 & 0x1f);
    dst[2] = src[2] >> (uVar3 & 0x1f);
    dst[3] = src[3] >> (uVar3 & 0x1f);
    dst[4] = src[4] >> (uVar3 & 0x1f);
    GS_131_OBJ_498();
    return;
  }
  *dst = *src;
  dst[1] = src[1];
  dst[2] = src[2];
  dst[3] = src[3];
  dst[4] = src[4];
  dst[5] = src[5];
}
void  NavMenu_SetBounds (int obj,int val,int arg3)
{
  *(int *)(obj + 0x9c) = val;
  *(int *)(obj + 0xa0) = arg3;
}
int * Spu_GetSeqTrackVtable (void)
{
  return &SpuSeqTrackVtableB;
}
void  Color_AddOrSet (int obj,char *arg2,char *arg3,int arg4)
{
  char cVar1;
  char cVar2;
  if (arg4 == 0) {
    *arg2 = *arg2 + *arg3;
    arg2[1] = arg2[1] + arg3[1];
    arg2[2] = arg2[2] + arg3[2];
  }
  else {
    cVar1 = arg3[1];
    cVar2 = arg3[2];
    *arg2 = *arg3;
    arg2[1] = cVar1;
    arg2[2] = cVar2;
  }
}
char *  Sjis_Decode (char *obj,char *arg2)
{
  char cVar1;
  byte bVar2;
  char *pcVar3;
  cVar1 = *arg2;
  pcVar3 = obj;
  while (cVar1 != '\0') {
    bVar2 = arg2[1];
    obj = obj + 1;
    if ((0x7f < bVar2) || (cVar1 = -0x1f, bVar2 == 0x40)) {
      cVar1 = -0x20;
    }
    arg2 = arg2 + 2;
    *pcVar3 = bVar2 + cVar1;
    pcVar3 = pcVar3 + 1;
    cVar1 = *arg2;
  }
  *obj = '\0';
  return obj;
}
int * Font_GetDataPtr (void)
{
  return (int *)&D_8006E878;
}
void  RenderView_SetDrawMode (int obj,int val)
{
  *(int *)(obj + 0xb4) = val;
}
void  Entity_SetPosition (int obj,u16 *val)
{
  if (*(int *)(obj + 0xc) != 0) {
    *(u16 *)(obj + 0x60) = *val;
    *(u16 *)(obj + 0x62) = val[2];
  }
}
void  ChunkDataObj_Reset (int obj)
{
  int iVar1;
  *(int *)(obj + 0x30) = 0;
  *(int *)(obj + 0x10) = 0;
  *(int *)(obj + 0xc) = 0;
  iVar1 =  GetCoordSystemVtable ();
  /* vtable[6] */
  (**(code **)(iVar1 + 0x18))(obj);
}
void  Entity_StateEnter (int *obj)
{
  int iVar1;
  if (obj[0x1b] == 0) {
    /* vtable[55] */
    iVar1 = (**(code **)(*obj + 0xdc))();
    /* vtable[46] */
    (**(code **)(*obj + 0xb8))(obj,1,&UNK_8006ea90 + iVar1 * 3);
    obj[0x1b] = 1;
    obj[0x1d] = -obj[0x1d];
  }
}
void  Sjis_FormatNumber (int obj,int arg2,int arg3,int arg4)
{
  uint uVar1;
  char *pcVar2;
  int iVar3;
  uVar1 = arg3 + 0xfU & 0xfffffff8;
  char puVar4[uVar1];
  char puVar5[uVar1 * 2];
  pcVar2 = itoa(arg2);
  pcVar2 = strcpy(puVar4,pcVar2);
  iVar3 = strlen(pcVar2);
  if (arg4 == 0) {
    memset(puVar5,'0',arg3);
    strcpy(puVar5 + (arg3 - iVar3),puVar4);
     Sjis_EncodeFromAscii (obj,(uchar *)puVar5);
  } else {
     Sjis_EncodeFromAscii (obj,(uchar *)puVar4);
  }
}
void  NavMenu_SetItemState (int *obj,int arg2)
{
  int iVar1;
  code *pcVar2;
  int iVar3;
  iVar3 = *obj;
  iVar1 =  Font_GetDataPtr ();
  /* vtable[24] */
  (**(code **)(iVar1 + 0x60))(obj,arg2);
  switch(arg2) {
  case 4:
  case 7:
    obj[7] = 0;
    obj[0xf] = 0;
    break;
  case 5:
    /* vtable[57] */
    (**(code **)(iVar3 + 0xe4))(obj,obj[0x13] + 0x10);
    /* vtable[60] */
    (**(code **)(iVar3 + 0xf0))(obj,*(int *)(obj[0x13] + 8),0);
    obj[7] = 0;
    obj[0xf] = 1;
    break;
  case 6:
    obj[0xe] = 1;
    /* vtable[37] */
    pcVar2 = *(code **)(iVar3 + 0x94);
    goto code_r0x8003c770;
  case 8:
    obj[7] = 0;
    break;
  case 9:
  case 10:
  case 0xb:
  case 0xe:
  case 0xf:
  case 0x10:
  case 0x11:
    obj[8] = 5;
    obj[7] = 0;
    if (arg2 == 0xf) {
      /* vtable[67] */
      pcVar2 = *(code **)(iVar3 + 0x10c);
    }
    else if (arg2 < 0x10) {
      if (arg2 != 0xb) {
        return;
      }
      /* vtable[36] */
      pcVar2 = *(code **)(iVar3 + 0x90);
    }
    else {
      if (arg2 != 0x11) {
        return;
      }
      /* vtable[68] */
      pcVar2 = *(code **)(iVar3 + 0x110);
    }
code_r0x8003c770:
    (*pcVar2)(obj);
  }
}
void  NavMenu_Destroy (int *obj)
{
  int *piVar1;
  int iVar2;
  int *puVar3;
  piVar1 = (int *)obj[0x13];
  if (piVar1 != NULL) {
    if (*piVar1 != 0) {
      (**(code **)(*(int *)piVar1[1] + 4))();
    }
    iVar2 = 0;
    (**(code **)(*(int *)obj[0x1a] + 4))();
    puVar3 = (int *)obj[0x15];
    if (0 < obj[0x14]) {
      do {
        if (*(int *)(iVar2 * 4 + *(int *)(obj[0x13] + 0x24)) != 0) {
          obj[0x16] = iVar2;
          /* vtable[63] */
          (**(code **)(*obj + 0xfc))(obj);
        }
        iVar2 = iVar2 + 1;
        (**(code **)(*(int *)*puVar3 + 4))();
        puVar3 = puVar3 + 1;
      } while (iVar2 < obj[0x14]);
    }
     MemFreeImpl (obj[0x19]);
     MemFreeImpl (obj[0x18]);
     MemFreeImpl (obj[0x17]);
     MemFreeImpl (obj[0x15]);
  }
}
int  Particle_New (int obj,int arg2,int arg3)
{
  int iVar1;
  int iVar2;
  iVar1 =  MemAllocImpl (0x6c);
  iVar2 = 0;
  if (iVar1 != 0) {
    iVar2 =  Primitive_GetVtable ();
    (**(code **)(iVar2 + 8))(iVar1,obj,arg2,arg3);
    iVar2 = iVar1;
  }
  return iVar2;
}
void  RenderView_SetBackground (int obj,int *val)
{
  if (*(int *)(obj + 0x10) == 0) {
    if (*(int **)(obj + 0xb0) != NULL) {
      (**(code **)(**(int **)(obj + 0xb0) + 4))();
    }
    *(int **)(obj + 0xb0) = val;
    if (val != NULL) {
      /* vtable[19] */
      (**(code **)(*val + 0x4c))(val,*(int *)(obj + 0xac),&Sys_UnknownA904);
    }
  }
}
void  RenderView_InitProjection (int *obj,int arg2,int arg3,int arg4,
                 int *arg5)
{
  int iVar1;
  iVar1 = *obj;
  if (obj[4] == 0) {
    /* vtable[4] */
    (**(code **)(iVar1 + 0x10))();
    /* vtable[30] */
    (**(code **)(iVar1 + 0x78))(obj,arg3);
    /* vtable[31] */
    (**(code **)(iVar1 + 0x7c))(obj,arg4);
    if (arg5 == NULL) {
      arg5 = &Sys_RegParam2;
    }
    /* vtable[32] */
    (**(code **)(iVar1 + 0x80))(obj,arg5);
    GsSetRefView2((GsRVIEW2 *)(obj + 5));
  }
}
void  Font_Destroy (int *obj)
{
  int *piVar1;
  int iVar2;
  iVar2 = *obj;
  /* vtable[20] */
  (**(code **)(iVar2 + 0x50))();
  piVar1 = (int *)obj[6];
  if (obj[9] == 0) {
    (**(code **)(*(int *)obj[5] + 0x14))((int *)obj[5],obj[4]);
    /* vtable[5] */
    (**(code **)(*piVar1 + 0x14))(piVar1,obj[4]);
    /* vtable[5] */
    (**(code **)(*piVar1 + 0x14))(piVar1,*(int *)obj[3]);
  }
  /* vtable[5] */
  (**(code **)(iVar2 + 0x14))(obj,obj[4]);
  /* vtable[5] */
  (**(code **)(iVar2 + 0x14))(obj,*(int *)(obj[3] + 4));
  /* vtable[5] */
  (**(code **)(iVar2 + 0x14))(obj,*(int *)obj[3]);
  if (*(int **)(obj[3] + 0x10) != piVar1) {
    iVar2 = (**(code **)(*piVar1 + 4))(piVar1);
    obj[6] = iVar2;
  }
  if (*(int **)(obj[3] + 0xc) != (int *)obj[5]) {
    iVar2 = (**(code **)(*(int *)obj[5] + 4))();
    obj[5] = iVar2;
  }
  if (*(int **)(obj[3] + 8) != (int *)obj[4]) {
    iVar2 = (**(code **)(*(int *)obj[4] + 4))();
    obj[4] = iVar2;
  }
}
void  ChunkDataObj_Construct (int *obj)
{
  int iVar1;
  int *piVar2;
  iVar1 =  GetCoordSystemVtable ();
  (**(code **)(iVar1 + 8))(obj);
  iVar1 =  ChunkData_GetDataPtr ();
  *obj = iVar1;
  obj[3] = 0;
  obj[4] = 0;
  iVar1 =  SceneNodeAlloc ();
  obj[0x2b] = iVar1;
  piVar2 = (int *) EntityAllocLarge (&Sys_UnknownA90C,0,0);
  obj[0x2c] = (int)piVar2;
  /* vtable[19] */
  (**(code **)(*piVar2 + 0x4c))(piVar2,obj[0x2b],&Sys_UnknownA904);
  /* vtable[16] */
  (**(code **)(*obj + 0x40))(obj);
}
void  NavMenu_ChangePage (int *obj)
{
  code *pcVar1;
  if (*(int *)(obj[0x16] * 4 + *(int *)(obj[0x13] + 0x24)) == 0) {
    if (obj[0x16] != *(int *)(obj[0x13] + 0xc)) {
      return;
    }
    /* vtable[37] */
    pcVar1 = *(code **)(*obj + 0x94);
  }
  else {
    /* vtable[66] */
    pcVar1 = *(code **)(*obj + 0x108);
  }
  (*pcVar1)();
}
int * ChunkData_GetDataPtr (void)
{
  return (int *)&D_8006E8E4;
}
void  NavMenu_AddItems (int obj,int arg2,int arg3)
{
  int iVar1;
  int *puVar2;
  int uVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  piVar4 = *(int **)(arg2 + 0x18);
  iVar6 = *(int *)(obj + 0x58);
  iVar5 = 0;
  while (iVar1 = *piVar4, piVar4 = piVar4 + 1, iVar1 != 0) {
    iVar5 = iVar5 + 1;
  }
  puVar2 = (int *) MemAllocImpl (iVar5 << 2);
  iVar6 = iVar6 * 4;
  *(int **)(iVar6 + *(int *)(obj + 100)) = puVar2;
  *(int *)(iVar6 + *(int *)(obj + 0x60)) = *(int *)(arg2 + 4);
  *(int *)(iVar6 + *(int *)(obj + 0x5c)) = iVar5;
  piVar4 = *(int **)(arg2 + 0x18);
  iVar6 = *piVar4;
  while (iVar6 != 0) {
    iVar6 = strlen((char *)*piVar4);
    iVar5 = *piVar4;
    piVar4 = piVar4 + 1;
    uVar3 =  TextInput_New (arg3,iVar6,iVar5);
    *puVar2 = uVar3;
    puVar2 = puVar2 + 1;
    iVar6 = *piVar4;
  }
}
void  Font_Tick (int obj)
{
  *(int *)(obj + 0x1c) = *(int *)(obj + 0x1c) + 1;
}
void  SpuVoice_InitDefaults (int obj)
{
  *(int *)(obj + 0x90) = 0;
  *(int *)(obj + 0x70) = 0;
  *(int *)(obj + 0x34) = DEFAULT_SCALE;
  *(int *)(obj + 0x38) = 0xf0;
  *(int *)(obj + 0x3c) = 0xd;
  *(int *)(obj + 0x44) = 2000;
  *(int *)(obj + 0x48) = 0x40;
  *(int *)(obj + 0x40) = DEFAULT_SCALE;
  *(int *)(obj + 0x4c) = 10;
  *(int *)(obj + 0x50) = 0x10000;
  *(int *)(obj + 0x54) = 0;
  *(int *)(obj + 0x60) = 20000;
  *(u8 *)(obj + 0x5b) = 0;
  *(u8 *)(obj + 0x5c) = 0;
  *(u8 *)(obj + 0x5d) = 0;
  *(u8 *)(obj + 0x58) = 0;
  *(u8 *)(obj + 0x59) = 0;
  *(u8 *)(obj + 0x5a) = 0;
  *(int *)(obj + 0xb4) = 0;
  *(int *)(obj + 0xb8) = 1;
}
void  ChunkDataObj_Destroy (int *obj)
{
  int iVar1;
  /* vtable[36] */
  (**(code **)(*obj + 0x90))();
  /* vtable[29] */
  (**(code **)(*obj + 0x74))(obj);
  (**(code **)(*(int *)obj[0x2b] + 4))();
  /* vtable[42] */
  (**(code **)(*obj + 0xa8))(obj,0);
  iVar1 =  GetCoordSystemVtable ();
  /* vtable[3] */
  (**(code **)(iVar1 + 0xc))(obj);
}
void  Font_Stop (int obj)
{
  *(int *)(obj + 0x1c) = 0;
  (**(code **)(*(int *)**(int **)(obj + 0xc) + 0x48))();
}
int * TypeEntry1_GetDataPtr (void)
{
  return (int *)&D_8006E854;
}
void  NavMenu_ConfirmState (int *obj)
{
  int iVar1;
  int iVar2;
  int *puVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int local_30;
  int local_2c;
  if (obj[0xf] == 2) {
    iVar7 = obj[0x16];
    iVar2 = iVar7 * 4;
    iVar1 = *(int *)(iVar2 + *(int *)(obj[0x13] + 0x24));
    iVar6 = *(int *)(iVar2 + obj[0x18]);
    local_30 = *(int *)(iVar1 + 0x10);
    local_2c = *(int *)(iVar1 + 0x14) + iVar6 * -10;
    iVar5 = *(int *)(iVar2 + obj[0x17]);
    puVar3 = *(int **)(iVar2 + obj[0x19]);
    iVar1 = 0;
    if (0 < iVar5) {
      do {
        (**(code **)(*(int *)*puVar3 + 0x60))((int *)*puVar3,0);
        iVar1 = iVar1 + 1;
        (**(code **)(*(int *)*puVar3 + 0xbc))((int *)*puVar3,&local_30);
        local_2c = local_2c + 10;
        puVar3 = puVar3 + 1;
      } while (iVar1 < iVar5);
    }
    iVar7 = iVar7 * 4;
    piVar4 = *(int **)(iVar6 * 4 + *(int *)(iVar7 + obj[0x19]));
    /* vtable[24] */
    (**(code **)(*piVar4 + 0x60))(piVar4,1);
    /* vtable[46] */
    (**(code **)(*piVar4 + 0xb8))(piVar4,obj[0x13] + 0x10);
    *(int *)(*(int *)(iVar7 + *(int *)(obj[0x13] + 0x24)) + 4) = iVar6;
    (**(code **)(*(int *)obj[0x1a] + 0x50))();
    obj[0xf] = 1;
    /* vtable[24] */
    (**(code **)(*obj + 0x60))(obj,0x10);
  }
}
void  TextInput_Nop (void)
{
}
void  Spu_ResetSeqTrack (int obj)
{
  *(int *)(obj + 200) = 0xffffffff;
  *(int *)(obj + 0xc4) = 0;
  *(int *)(obj + 0xcc) = 1;
  *(int *)(obj + 0xd0) = 0;
  *(int *)(obj + 0xd4) = 1;
}
void  SpuVoice_SetColor2 (int obj,u8 *arg2)
{
  char uVar1;
  char uVar2;
  uVar1 = arg2[1];
  uVar2 = arg2[2];
  *(u8 *)(obj + 0x5b) = *arg2;
  *(u8 *)(obj + 0x5c) = uVar1;
  *(u8 *)(obj + 0x5d) = uVar2;
}
void  NavMenu_CancelState (int *obj)
{
  int *piVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  if (obj[0xf] == 2) {
    iVar3 = obj[0x16] * 4;
    iVar2 = *(int *)(iVar3 + obj[0x18]);
    /* vtable[64] */
    (**(code **)(*obj + 0x100))(obj,obj[5],0);
    iVar4 = *(int *)(iVar3 + obj[0x19]);
    piVar1 = *(int **)(iVar2 * 4 + iVar4);
    /* vtable[46] */
    (**(code **)(*piVar1 + 0xb8))(piVar1,obj[0x13] + 0x10);
    iVar2 = *(int *)(*(int *)(iVar3 + *(int *)(obj[0x13] + 0x24)) + 4);
    *(int *)(iVar3 + obj[0x18]) = iVar2;
    piVar1 = *(int **)(iVar2 * 4 + iVar4);
    /* vtable[24] */
    (**(code **)(*piVar1 + 0x60))(piVar1,1);
    obj[0xf] = 1;
    /* vtable[24] */
    (**(code **)(*obj + 0x60))(obj,0x11);
  }
}
int  NavMenu_NewPage (int obj,int arg2,int arg3)
{
  int iVar1;
  int iVar2;
  iVar1 =  MemAllocImpl (0xa4);
  iVar2 = 0;
  if (iVar1 != 0) {
    iVar2 =  NavMenu_GetDataPtr ();
    (**(code **)(iVar2 + 8))(iVar1,obj,arg2,arg3);
    iVar2 = iVar1;
  }
  return iVar2;
}

