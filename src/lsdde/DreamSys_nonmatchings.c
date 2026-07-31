#include "common.h"
extern u32 gShakePalette;        /* 0x80087e80 */
extern u32 gShakeApplyUnk;       /* 0x80087ea4 */
extern u32 gLinkStage;           /* 0x8008acc4 */
extern u32 gLinkDirIdx;          /* 0x8008acc8 */
extern u32 gLinkChunkIdx;        /* 0x8008acbc */
extern u32 gInstantTeleportFlag; /* 0x8008abe4 */
extern u32 gStairDefaultData;    /* 0x8008abf0 */
extern u8 gLinkAngleTbl[];       /* 0x80087f14 */
extern u32 **gStairTypePtr;      /* 0x80088ba4 */
extern int GetGameManagerAlias();
extern void AnimTick();
extern int MathReadScaledShorts();
extern int AnimReset();
extern DreamColors CalcDreamColor();
extern void AnimInitializeBase();
extern u32 gPosCalcUnk;          /* 0x80087ee0 */
extern u32 gPosCalcStored;       /* 0x80087ee8 */
extern u32 gSubObjParamTbl[];    /* 0x80087eb0 */
extern u32 gSubObjParam2Tbl[];   /* 0x80087ec8 */
extern int MathTransformByMatrix();
extern int MathIsPointInBox();
extern u32 gShakeOffsetTbl[];    /* 0x80087e50 */
extern u32 gShakeBoundTbl[];     /* 0x80087e5c */
extern u32 gEffectOffsetTbl[];   /* 0x80087e68 */
extern u32 gEffectBoundTbl[];    /* 0x80087e74 */
extern s16 gShakeValue;          /* 0x80087e84 */
extern u32 gEffectStageTbl[];    /* 0x80087e20 */
extern u32 gEffectParamTbl[];    /* 0x80087e34 */
extern u32 gEffectFuncTbl[];     /* 0x80087e3c */
extern u32 gStairColor1;         /* 0x80087efc */
extern u32 gStairColor2;         /* 0x80087f08 */
extern u32 gStairVec1;           /* 0x8008abc0 */
extern u32 gStairVec2;           /* 0x8008abc8 */
extern u32 gStairVec3;           /* 0x8008abd0 */
extern u32 gStairVec4;           /* 0x8008abd8 */
extern u32 gLinkInnerIdx;        /* 0x8008acc0 */
extern u32 gLinkDirData;         /* 0x80088758 */
extern u32 **gTunnelDirPtr;      /* 0x800889b8 */
extern u32 **gTunnelStagePtr;    /* 0x80088858 */
extern u32 **gStairDirPtr;       /* 0x80088c84 */
extern u32 **gStairStagePtr;     /* 0x80088bdc */
extern u32 PTR_func_8005AB2C_80087eec[];  /* gStairFuncTbl */
extern int Test4TunnelLinks();
extern int Test4InstantTeleporters();
extern int Test4StaircaseNodes();
extern bool ExecuteLink();
extern int GetRandomSpawnFromStage();
extern void StairVecDiff();
extern void CounterUpdate();
extern int LocationMatcher();
extern int StairLocationMatcher();
extern bool AngleDistCheck();
extern int RandomSpawnSelector();
void EntityDispatchCallbacks(int obj)
{
  if (*(code **)(obj + 0x80) != (code *)0x0) {
    (**(code **)(obj + 0x80))();                 /* vtable[32] */
  }
  if (*(code **)(obj + 0x98) != (code *)0x0) {
    (**(code **)(obj + 0x98))(obj);              /* vtable[38] */
  }
}
void EntityProcessManagerState(int *obj)
{
  int iVar1;
  (**(code **)(*(int *)obj[0x13] + 0xf0))();     /* vtable[60] */
  (**(code **)(*obj + 0x14))(obj,obj[0x13]);     /* vtable[5] */
  iVar1 = GetGameManagerAlias();
  (**(code **)(iVar1 + 0x50))(obj);              /* vtable[20] */
}
void EntityProcessActionNormalize(int *obj,int *arg2,int arg3)
{
  int iVar1;
  iVar1 = GetGameManagerAlias();
  (**(code **)(iVar1 + 0x9c))(obj,arg2,arg3);    /* vtable[39] */
  if ((*(u32 *)*arg2 & ANGLE_NORMALIZE) == 0x114) {
    (**(code **)(*obj + 0x1e4))(obj,arg2,arg3);  /* vtable[121] */
  }
}
void EntityProcessActionSpecial(int *obj,int *arg2,int arg3)
{
  int iVar1;
  iVar1 = GetGameManagerAlias();
  (**(code **)(iVar1 + 0xdc))(obj,arg2,arg3);    /* vtable[55] */
  if ((*(u32 *)*arg2 & 0xfffff) == 0x1f234) {
    (**(code **)(*obj + 0x1e8))(obj,arg2,arg3);  /* vtable[122] */
  }
}
int EntityCalcDreamColor(int obj,int *outColor,int moodValue)
{
  int uVar1;
  int uVar2;
  uVar2 = *(int *)(obj + 0x68);
  if (moodValue < 0) {
    uVar1 = CalcDreamColor((MoodGraphPoint *)(obj + *(int *)(obj + 0x180) * 2 + 400));
    *outColor = uVar1;
  } else {
    *(int *)(obj + 0x68) = moodValue;
  }
  return uVar2;
}
void EntityResetState(int *obj,int arg2,int arg3)
{
  (**(code **)(*obj + 500))(obj,obj + 0x5b);     /* vtable[125] */
  (**(code **)(*obj + 0x138))(obj,1);            /* vtable[78] */
  (**(code **)(*obj + 0x13c))(obj,1);            /* vtable[79] */
  (**(code **)(*obj + 0x180))(obj,arg2);         /* vtable[96] */
  obj[0x2f] = -1;
  obj[0x2d] = 0;
  obj[0x2e] = 0;
  obj[0x28] = 0;
  obj[0x29] = 0;
  obj[0x22] = 0;
  obj[0x24] = 0;
  obj[0x23] = 0;
  obj[0x25] = 0;
  (**(code **)(*obj + 0x18c))(obj,0,1,1,1);      /* vtable[99] */
  (**(code **)(*obj + 400))(obj,arg3);           /* vtable[100] */
  *(u16 *)((int)obj + 0x16a) = 0xffff;
  obj[0x1c] = 0;
  obj[0x11] = 0;
  obj[0x1d] = 0;
  obj[0x242] = 0;
  obj[0x243] = 0;
  obj[0x244] = 0;
  obj[0x1e] = 0;
  MathReadScaledShorts(obj,obj + 0x1a);
  (**(code **)(*obj + 0x44))(obj,1,obj + 0x1a);  /* vtable[17] */
}
void EntityResetAnimation(int obj,int value)
{
  *(int *)(obj + 0xc4) = 0;
  *(int *)(obj + 200) = value;
  if (value != 0) {
    AnimReset(*(int *)(obj + 0x58),(int *)(obj + 0xcc));
  }
}
int EntityGetSetDreamColor(int obj,int value)
{
  int uVar1;
  uVar1 = *(int *)(obj + 0xac);
  if (-1 < value) {
    *(int *)(obj + 0xac) = value;
    *(int *)(obj + 0xb0) = value;
  }
  return uVar1;
}
void EntitySetDreamColorCond(int obj,int value)
{
  if (*(int *)(obj + 0xac) != value) {
    *(int *)(obj + 0xb0) = *(int *)(obj + 0xac);
    *(int *)(obj + 0xac) = value;
  }
}
void EntityRevertDreamColor(int obj)
{
  *(int *)(obj + 0xac) = *(int *)(obj + 0xb0);
}
void EntitySetCoordOffsets(int obj,int arg1,int arg2,int arg3,int arg4)
{
  if (-1 < arg1) {
    *(int *)(obj + 0x124) = arg1;
  }
  if (-1 < arg2) {
    *(int *)(obj + 0x128) = arg2;
  }
  if (-1 < arg3) {
    *(int *)(obj + 300) = arg3;
  }
  if (-1 < arg4) {
    *(int *)(obj + 0x130) = arg4;
  }
}
void EntitySetField120(int obj,int value)
{
  *(int *)(obj + 0x120) = value;
}
int EntityGetMoodData(int obj,int *outValue)
{
  if (outValue != NULL) {
    *outValue = *(int *)(obj + 0x17c);
  }
  return *(int *)(obj + 0x180) + 1;
}
void EntityClearField878(int obj)
{
  *(int *)(obj + 0x878) = 0;
}
int EntityGetField878(int obj)
{
  return *(int *)(obj + 0x878);
}
int EntityGetMoodGraphBuffer(int obj,int *outPtr)
{
  if (outPtr != NULL) {
    *outPtr = 0x700;
  }
  return obj + 0x178;
}
void EntityClearField7C(int obj)
{
  *(int *)(obj + 0x7c) = 0;
}
void EntityClearField78(int obj)
{
  *(int *)(obj + 0x78) = 0;
}
int EntityReturnZero(void)
{
  return 0;
}
void EntityCleanupState(int *obj,int flag)
{
  (**(code **)(*obj + 0x13c))(obj,0);            /* vtable[79] */
  if (flag != 0) {
    (**(code **)(*obj + 0x138))(obj,0);          /* vtable[78] */
  }
}
void EntityEndAction(int *obj,int arg2,int arg3)
{
  (**(code **)(*obj + 0x13c))();                 /* vtable[79] */
  (**(code **)(*obj + 0x138))(obj,arg3);         /* vtable[78] */
}
void EntityDispatchAction(int *obj)
{
  (**(code **)(*obj + 0x144))();                 /* vtable[81] */
  (**(code **)(*obj + 0x148))(obj);              /* vtable[82] */
}
void EntitySetActionType(int *obj,int value)
{
  obj[0x1b] = value;
  if (value != 0) {
    (**(code **)(*obj + 0x180))(obj,1);          /* vtable[96] */
    if (obj[0x221] != 0) {
      (**(code **)(*obj + 0x44))(obj,1);         /* vtable[17] */
    }
  }
}
void EntityDispatchByActionType(int *obj)
{
  code *pcVar1;
  if (obj[0x1b] == 0) {
    (**(code **)(*obj + 0x174))();               /* vtable[93] */
    pcVar1 = *(code **)(*obj + 0x158);           /* vtable[86] */
  } else if (obj[0x1b] == 2) {
    pcVar1 = *(code **)(*obj + 0x160);           /* vtable[88] */
  } else {
    pcVar1 = *(code **)(*obj + 0x15c);           /* vtable[87] */
  }
  (*pcVar1)(obj);
}
void EntityRunActionCheck(int *obj)
{
  int uVar1;
  if (obj[0x1c] == 0) {
    uVar1 = (**(code **)(*obj + 0x164))(obj,1);  /* vtable[89] */
    (**(code **)(*obj + 0x170))(obj,uVar1);      /* vtable[92] */
  }
}
void EntityRunActionState(int *obj)
{
  int uVar1;
  obj[0x28] = 1;
  if (obj[0x1c] == 0) {
    uVar1 = (**(code **)(*obj + 0x164))(obj,1);  /* vtable[89] */
    (**(code **)(*obj + 0x170))(obj,uVar1);      /* vtable[92] */
  } else {
    (**(code **)(*obj + 0x164))(obj,0);          /* vtable[89] */
  }
}
void EntitySetCounterFlag(int obj)
{
  *(int *)(obj + 0xa0) = 1;
}
void EntityApplyShakePalette(int *obj)
{
  if (obj[0x29] != 0) {
    (**(code **)(*obj + 0x44))(obj,0,(int)&gShakePalette + obj[0x29] * 0xc);  /* vtable[17] */
    obj[0x29] = 0;
  }
}
void EntityUpdateShakeEffect(int *obj)
{
  if (obj[0x31] != 0) {
    (**(code **)(*obj + 0xbc))(obj,(int)&gShakeApplyUnk);   /* vtable[47] */
    *(int *)(obj[0x17] + 0x24) = *(int *)(obj[0x17] + 0x24) + -600;
  }
  if (obj[0x32] != 0) {
    AnimTick(obj[0x16],obj + 0x33);
  }
}
void EntityProcessActionDetailed(int *obj,int action)
{
  int iVar1 = GetGameManagerAlias();
  (**(code **)(iVar1 + 0x88))(obj,action);        /* vtable[34] */
  if (action == -2) {
    iVar1 = (**(code **)(*(int *)obj[0x13] + 0x11c))((int *)obj[0x13],obj[5] + 0x18);   /* vtable[71] */
    if (*(short *)(*(int *)(iVar1 + 4) + 0x2c) != 2) {
      (**(code **)(*obj + 0x224))(obj);           /* vtable[137] */
    }
  } else if (action == -1) {
    u32 uVar2 = *(u16 *)(obj[10] + 0x36) & 0x7f;
    obj[0x2e] = uVar2;
    if (0x17 < uVar2) {
      obj[0x2e] = 0;
    }
    if ((obj[0x11] == 0xf) && (obj[0x2e] == 0)) {
      obj[0x2e] = 2;
    }
    if (obj[0x59] != 9) { return; }
    int uVar3 = (**(code **)(*(int *)obj[0x13] + 0x10c))((int *)obj[0x13],0,0);   /* vtable[67] */
    (**(code **)(*obj + 0x1d4))(obj,uVar3);       /* vtable[117] */
  }
}
void EntitySetActionByCode(int *obj,int arg2,int action)
{
  if (((obj[0x1b] == 0) && (obj[0x1c] == 0)) && (obj[0x242] == 0)) {
    switch(action) {
    case 2:   obj[0x28] = 1; break;
    case 3:   obj[0x28] = 2; break;
    case 4:   obj[0x29] = 1; break;
    case 5:   obj[0x29] = 2; break;
    case 6:   obj[0x22] = 1; break;
    case 7:   if (obj[0x28] == 1) { (**(code **)(*obj + 0x184))(obj,4); } break;  /* vtable[97] */
    case 8:   obj[0x22] = 2; break;
    case 0xd: obj[0x24] = 2; break;
    case 0xe: obj[0x28] = 4; break;
    case 0xf: obj[0x24] = 1; break;
    case 0x10: obj[0x28] = 3; break;
    case 0x19: obj[0x1d] = 1; break;
    case 0x22: (**(code **)(*obj + 0x188))(); break;   /* vtable[98] */
    }
  }
}
void EntitySetModeVtable(int *obj,int mode)
{
  int iVar1 = *obj;
  obj[0x21] = mode;
  if (mode == 1) {
    obj[0x20] = *(int *)(iVar1 + 0x140);         /* vtable[80] */
  } else if (mode < 2) {
    if (mode == 0) {
      obj[0x20] = 0;
    }
  } else if (mode == 2) {
    obj[0x20] = *(int *)(iVar1 + 0x14c);         /* vtable[83] */
  } else if (mode == 3) {
    obj[0x20] = *(int *)(iVar1 + 0x150);         /* vtable[84] */
  }
}
void EntitySetAnimModeVtable(int *obj,int mode)
{
  int iVar1 = *obj;
  if (obj[0x27] == 2) {
    (**(code **)(iVar1 + 0x17c))(obj,0);         /* vtable[95] */
  }
  obj[0x27] = mode;
  if (mode == 1) {
    obj[0x26] = *(int *)(iVar1 + 0x154);         /* vtable[85] */
  } else if (mode < 2) {
    if (mode == 0) {
      obj[0x26] = 0;
    }
  } else if (mode == 2) {
    iVar1 = *(int *)(iVar1 + 0x178);             /* vtable[94] */
    obj[0x31] = 1;
    obj[0x32] = 1;
    obj[0x26] = iVar1;
    AnimInitializeBase(obj[0x16],obj + 0x33,1,obj,*(int *)(*obj + 0x194));  /* vtable[101] */
  }
}
int EntityCounterStateMachine(int *obj,int flag)
{
  bool bVar1 = false;
  int iVar2 = obj[0x28];
  int iVar5 = 0;
  if (iVar2 != 0) {
    u32 uVar3 = obj[0x2d] + 1;
    obj[0x2d] = uVar3;
    if ((int)uVar3 < 4) {
      if (obj[0x2b] == 4) {
        bVar1 = (uVar3 & 1) == 0;
      }
    } else {
      obj[0x28] = 0;
      bVar1 = true;
    }
    if (bVar1) {
      (**(code **)(*obj + 0x168))(obj);            /* vtable[90] */
    }
    iVar5 = obj[0x17];
    if (((iVar5 != 0) && (obj[0x19e] != 0)) && (flag != 0)) {
      int iVar4 = -0x32;
      if (2 < obj[0x2d]) {
        iVar4 = 0x32;
      }
      *(int *)(iVar5 + 0x18) = iVar4 + *(int *)(iVar5 + 0x18);
      *(int *)(iVar5 + 0x24) = iVar4 + *(int *)(iVar5 + 0x24);
    }
    iVar5 = iVar2;
    if (obj[0x28] == 0) {
      obj[0x2d] = 0;
    }
  }
  if (!bVar1) {
    (**(code **)(*obj + 0x16c))(obj);              /* vtable[91] */
  }
  return iVar5;
}
void EntityClearSoundEffect(int obj)
{
  if (-1 < *(int *)(obj + 0xbc)) {
    (**(code **)(**(int **)(obj + 0x58) + 0x84))();   /* vtable[33] */
    *(int *)(obj + 0xbc) = 0xffffffff;
  }
}
int EntityGetLinkOffset(int obj)
{
  return *(int *)(obj + 0x164);
}
int EntityGetSetStairField(int obj,int value)
{
  int uVar1 = *(int *)(obj + 0x924);
  if (value >= 0) {
    *(int *)(obj + 0x924) = value;
  }
  return uVar1;
}
int LinkGetAngleFromTable(int index)
{
  return (int)*(short *)(gLinkAngleTbl + index * 2);
}
void *LinkGetStairDefaultData(void)
{
  if (gLinkStage != 0xc) {
    return &gStairDefaultData;
  }
  return NULL;
}
void SoundSetTeleportFlag(int flag)
{
  gInstantTeleportFlag = flag;
}
byte LinkGetInstantTeleportFlagOffset(void)
{
  return -(gLinkChunkIdx == 0) & 10;
}
int StairGetTypeIndex(void)
{
  return (int)(char)(&gStairTypePtr)[gLinkStage][gLinkDirIdx * 6 + 5];
}
// camera shake / offset update with table lookup
void CameraShakeUpdate(int obj)
{
  int iVar1 = *(int *)(obj + 0x88) * 4;
  if (*(int *)(obj + 0x88) == 0) {
    if (*(int *)(obj + 0x8c) == 0) { return; }
    iVar1 = -600;
    if (*(int *)(obj + 0x8c) < 0) { iVar1 = 600; }
    *(int *)(*(int *)(obj + 0x5c) + 0x24) = iVar1 + *(int *)(*(int *)(obj + 0x5c) + 0x24);
    *(int *)(obj + 0x8c) = iVar1 + *(int *)(obj + 0x8c);
    return;
  }
  u32 uVar2 = *(int *)((int)&gShakeOffsetTbl + iVar1) + *(int *)(obj + 0x8c);
  if ((int)uVar2 < 0) {
    if (*(int *)((int)&gShakeBoundTbl + iVar1) <= (int)(~uVar2 + 1)) { goto clear_field; }
  } else if (*(int *)((int)&gShakeBoundTbl + iVar1) <= (int)uVar2) {
    *(int *)(obj + 0x88) = 0;
    return;
  }
  *(int *)(*(int *)(obj + 0x5c) + 0x24) = *(int *)((int)&gShakeOffsetTbl + iVar1) + *(int *)(*(int *)(obj + 0x5c) + 0x24);
  *(u32 *)(obj + 0x8c) = uVar2;
clear_field:
  *(int *)(obj + 0x88) = 0;
}
// animation/camera effect update
void CamEffectUpdate(int *obj)
{
  obj[0x2a] = (u32)(obj[0x28] == 1);
  if (obj[0x24] == 0) {
    if (obj[0x25] == 0) { return; }
    int iVar1 = -0x2d;
    if (obj[0x25] < 0) { iVar1 = 0x2d; }
    gShakeValue = (s16)iVar1;
    (**(code **)(*obj + 0x44))(obj,0,&gShakePalette);     /* vtable[17] */
    obj[0x25] = iVar1 + obj[0x25];
    goto call_59A1C;
  }
  int iVar1 = obj[0x24] * 4;
  u32 uVar2 = *(int *)((int)&gEffectOffsetTbl + iVar1) + obj[0x25];
  if ((int)uVar2 < 0) {
    if ((int)(~uVar2 + 1) < *(int *)((int)&gEffectBoundTbl + iVar1)) { goto clear24; }
  } else if (*(int *)((int)&gEffectBoundTbl + iVar1) <= (int)uVar2) {
    obj[0x24] = 0;
    goto call_59A1C;
  }
  gShakeValue = (s16)*(int *)((int)&gEffectOffsetTbl + iVar1);
  (**(code **)(*obj + 0x44))(obj,0,&gShakePalette);       /* vtable[17] */
  obj[0x25] = uVar2;
clear24:
  obj[0x24] = 0;
call_59A1C:
  CounterUpdate(obj);
}
// complex game logic with condition checks
void ProcessDreamAction(int *obj,int action)
{
  if (action == 0) { return; }
  char cVar1 = ((char *)&gEffectParamTbl)[action];
  int iVar2 = *(int *)((int)&gEffectStageTbl + obj[0x2b] * 4);
  (**(code **)(*obj + 300))();                              /* vtable[75] */
  int uVar3 = (**(code **)(*(int *)obj[0x13] + 0x10c))((int *)obj[0x13],0,0);   /* vtable[67] */
  int iVar4 = (**(code **)(*obj + 0x1dc))(obj,uVar3);      /* vtable[119] */
  if (((iVar4 == 0) && (iVar4 = (**(code **)(*obj + 0x1d8))(obj,uVar3), iVar4 == 0)) &&  /* vtable[118] */
     (iVar4 = (**(code **)(*obj + 0x1d0))(obj,uVar3), iVar4 == 0)) {                    /* vtable[116] */
    (**(code **)(*obj + 0x220))(obj);                       /* vtable[136] */
    (**(code **)((int)&gEffectFuncTbl + action * 4))(obj,cVar1 * iVar2,obj[0x243] == 0);
    if (((obj[0x59] == 0) && (*(int *)(obj[5] + 0x1c) < -2000)) &&
       (-500 < *(int *)(obj[5] + 0x18))) {
      (**(code **)(*obj + 0xe0))(obj,obj,4);               /* vtable[56] */
    }
  }
  *(int *)obj[5] = 0;
}
// tunnel link test
int TestTunnelLink(int obj,int node)
{
  int iVar1;
  char buf_16[16];
  if (*(int *)(obj + 0x44) != 0) { return 0; }
  iVar1 = Test4TunnelLinks(obj + 0x16c,node,*(int *)(obj + 0x164));
  if (-1 < iVar1) {
    MathReadScaledShorts(obj,buf_16);
    if (LocationMatcher(obj + 0x888,obj + 0x884,buf_16) != 0) {
      if (*(int *)(obj + 0xa8) != 0) {
        ExecuteLink((DreamSys *)obj,iVar1,0xf,0);
        return 1;
      }
    }
  }
  return 0;
}
// tunnel exit link test
int TestTunnelExitLink(int obj,int link)
{
  int iVar1;
  if ((*(int *)(obj + 0x44) != 0) ||
     ((iVar1 = RandomSpawnSelector(obj + 0x16c,*(int *)(obj + 0x164),link,*(int *)(obj + 0x24)), iVar1 < 0))) {
    return 0;
  }
  *(int *)(obj + 0x880) = (u32)(int)LinkGetStairDefaultData();
  *(int *)(obj + 0x884) = 0;
  *(int *)(obj + 0x888) = 0;
  ExecuteLink((DreamSys *)obj,iVar1,0x10,0);
  return 1;
}
// instant teleporter
int TestInstantTeleport(int *obj,int pos)
{
  char buf_16[16];
  int iVar1 = Test4InstantTeleporters(obj + 0x5b,pos,obj[0x59]);
  if (iVar1 < 0) { return 0; }
  int iVar3 = LinkGetInstantTeleportFlagOffset();
  iVar1 = ExecuteLink((DreamSys *)obj,iVar1,0x11,0);
  if (iVar1 != 0) {
    obj[0x11] = 0;
    (**(code **)(*(int *)obj[0x13] + 0xe8))((int *)obj[0x13],buf_16,obj + 0x5b);  /* vtable[58] */
    (**(code **)(*obj + 0xb8))(obj,buf_16);                                        /* vtable[46] */
    if (iVar3 != 0 && obj[0x1a] == 0) {
      iVar1 = (**(code **)(*obj + 0x108))(obj);   /* vtable[66] */
      (**(code **)(*obj + 0x104))(obj,iVar1 + iVar3);  /* vtable[65] */
    }
  }
  return 1;
}
// staircase node detection (complex)
int TestStaircaseNodes(int *obj,u32 bitmask)
{
  char buf_16[16];
  if (obj[0x11] != 0) { return 0; }
  if ((code *)obj[0x244] == (code *)0x0) {
    int iVar4 = Test4StaircaseNodes(obj + 0x5b,bitmask,obj[0x59]);
    if (iVar4 < 0) { return 0; }
    MathReadScaledShorts(obj,buf_16);
    long long uVar7 = StairLocationMatcher(obj + 0x222,obj + 0x221,buf_16);
    if (((int)uVar7 != 0) && (obj[0x2a] != 0)) {
      u32 uVar2 = bitmask + 3 & 3;
      u32 uVar6 = bitmask & 3;
      u32 uVar5 = *(int *)((bitmask + 3) - uVar2) << (3 - uVar2) * 8 & -1 << (4 - uVar6) * 8 | *(u32 *)(bitmask - uVar6) >> uVar6 * 8;
      uVar2 = bitmask + 7 & 3;
      uVar6 = bitmask + 4 & 3;
      uVar6 = (*(int *)((bitmask + 7) - uVar2) << (3 - uVar2) * 8 | (u32)((u64)uVar7 >> 32) & 0xffffffffU >> (uVar2 + 1) * 8) & -1 << (4 - uVar6) * 8 | *(u32 *)((bitmask + 4) - uVar6) >> uVar6 * 8;
      u16 uVar1 = *(u16 *)(bitmask + 8);
      u32 uVar2_align = ((u32)obj + 0x91b) & 3;
      *(u32 *)(((u32)obj + 0x91b) - uVar2_align) = *(u32 *)(((u32)obj + 0x91b) - uVar2_align) & -1 << (uVar2_align + 1) * 8 | uVar5 >> (3 - uVar2_align) * 8;
      uVar2_align = (u32)(obj + 0x246) & 3;
      *(u32 *)((u32)(obj + 0x246) - uVar2_align) = *(u32 *)((u32)(obj + 0x246) - uVar2_align) & 0xffffffffU >> (4 - uVar2_align) * 8 | uVar5 << uVar2_align * 8;
      uVar2_align = ((u32)obj + 0x91f) & 3;
      *(u32 *)(((u32)obj + 0x91f) - uVar2_align) = *(u32 *)(((u32)obj + 0x91f) - uVar2_align) & -1 << (uVar2_align + 1) * 8 | uVar6 >> (3 - uVar2_align) * 8;
      uVar2_align = (u32)(obj + 0x247) & 3;
      *(u32 *)((u32)(obj + 0x247) - uVar2_align) = *(u32 *)((u32)(obj + 0x247) - uVar2_align) & 0xffffffffU >> (4 - uVar2_align) * 8 | uVar6 << uVar2_align * 8;
      *(u16 *)(obj + 0x248) = uVar1;
      obj[0x242] = 1;
      obj[0x243] = 1;
      obj[0x245] = 0;
      iVar4 = StairGetTypeIndex();
      obj[0x244] = (int)PTR_func_8005AB2C_80087eec[iVar4];
      (**(code **)(*obj + 0x44))(obj,1,obj[0x221]);         /* vtable[17] */
      (*(code *)obj[0x244])(obj);
    }
  } else {
    int iVar4 = (*(code *)obj[0x244])();
    if (iVar4 != 0) {
      obj[0x242] = 0;
      obj[0x244] = 0;
      obj[0x243] = 0;
      if (obj[0x2b] == 4) {
        (**(code **)(*obj + 0x188))(obj);                    /* vtable[98] */
      }
    }
  }
  return 0;
}
// stair animation state 1
int StairAnimState1(int *obj)
{
  if (obj[0x245] == 0) {
    StairVecDiff(obj,&gStairVec1,obj + 0x247);
  }
  if (obj[0x2b] == 4) {
    int iVar1 = obj[0x245];
    if (0x12 < iVar1) { return 1; }
    if ((iVar1 - 8U < 2) || (iVar1 - 0xdU < 2)) {
      (**(code **)(*obj + 0x44))(obj,0,&gStairColor1);      /* vtable[17] */
    }
  } else {
    int iVar1 = obj[0x245];
    if (0x84 < iVar1) { return 1; }
    if ((iVar1 - 0x2bU < 0xf) || (iVar1 - 0x4bU < 0xf)) {
      obj[0x29] = 2;
    }
  }
  obj[0x28] = 1;
  obj[0x245] = obj[0x245] + 1;
  return 0;
}
// stair animation state 2
int StairAnimState2(int *obj)
{
  bool bVar1;
  if (obj[0x245] == 0) {
    StairVecDiff(obj,&gStairVec2,obj + 0x247);
  }
  if (obj[0x2b] == 4) {
    int iVar2 = obj[0x245];
    if (0x18 < iVar2) { return 1; }
    if (((iVar2 - 6U < 2) || (iVar2 - 0xbU < 2)) || (iVar2 - 0x14U < 2)) {
      (**(code **)(*obj + 0x44))(obj,0,&gStairColor2);      /* vtable[17] */
    }
    bVar1 = obj[0x245] - 3U < 0xe;
  } else {
    int iVar2 = obj[0x245];
    if (0x94 < iVar2) { return 1; }
    if (((iVar2 - 0x16U < 0xf) || (iVar2 - 0x39U < 0x10)) || (iVar2 - 0x6eU < 0xf)) {
      obj[0x29] = 1;
    }
    bVar1 = obj[0x245] - 0x39U < 0x35;
  }
  if (bVar1) { obj[0x22] = 2; }
  obj[0x28] = 1;
  obj[0x245] = obj[0x245] + 1;
  return 0;
}
// stair animation state 3
int StairAnimState3(int *obj)
{
  if (obj[0x245] == 0) {
    StairVecDiff(obj,&gStairVec3,obj + 0x247);
  }
  if (obj[0x2b] == 4) {
    if (0xe < obj[0x245]) { return 1; }
    if (obj[0x245] - 8U < 2) {
      (**(code **)(*obj + 0x44))(obj,0,&gStairColor1);      /* vtable[17] */
    }
  } else {
    if (100 < obj[0x245]) { return 1; }
    if (obj[0x245] - 0x2bU < 0xf) {
      obj[0x29] = 2;
    }
  }
  obj[0x28] = 1;
  obj[0x245] = obj[0x245] + 1;
  return 0;
}
// stair animation state 4
int StairAnimState4(int *obj)
{
  bool bVar1;
  if (obj[0x245] == 0) {
    StairVecDiff(obj,&gStairVec4,obj + 0x247);
  }
  if (obj[0x2b] == 4) {
    int iVar2 = obj[0x245];
    if (0x12 < iVar2) { return 1; }
    if ((iVar2 - 6U < 2) || (iVar2 - 0xfU < 2)) {
      (**(code **)(*obj + 0x44))(obj,0,&gStairColor2);      /* vtable[17] */
    }
    bVar1 = (u32)obj[0x245] < 9;
  } else {
    int iVar2 = obj[0x245];
    if (0x70 < iVar2) { return 1; }
    if ((iVar2 - 0x1eU < 0xf) || (iVar2 - 0x52U < 0xf)) {
      obj[0x29] = 1;
    }
    bVar1 = obj[0x245] - 0x1eU < 0x34;
  }
  if (bVar1) { obj[0x22] = 2; }
  obj[0x28] = 1;
  obj[0x245] = obj[0x245] + 1;
  return 0;
}
// location matcher
int LocationMatcher(int *outDir1,int *outDir2,int angle)
{
  byte bVar1 = ((u32 **)&gTunnelDirPtr)[gLinkChunkIdx][gLinkInnerIdx];
  int iVar2 = AngleDistCheck(angle,(u32)bVar1);
  if (iVar2 == 0) { return 0; }
  if (outDir2 != NULL) {
    *outDir2 = (u32)&gLinkDirData + (u32)bVar1 * 0xc;
  }
  if (outDir1 != NULL) {
    *outDir1 = (u32)&gLinkDirData + (u32)(byte)((u32 **)&gTunnelStagePtr)[gLinkStage][gLinkDirIdx] * 0xc;
  }
  return 1;
}
// angle/distance check
bool AngleDistCheck(int angle,u32 dir)
{
  int iVar1 = (u32)*(u16 *)(angle + 4) - (u32)*(u16 *)((u32)&gLinkDirData + (dir & 0xff) * 0xc + 4);
  int iVar2 = iVar1 * 0x10000 >> 0x10;
  if (iVar2 < 0xb5) {
    if (iVar2 < -0xb4) { iVar1 = iVar1 + 0x168; }
  } else {
    iVar1 = iVar1 + -0x168;
  }
  return (iVar1 + 0x2cU & 0xffff) < 0x59;
}
// random spawn selector
int RandomSpawnSelector(int linkCoords,int stage,int *data,u32 flags)
{
  if ((stage == 3) || (stage == 1) || (stage == 5)) {
    if ((stage != 5) && ((stage != 9) || (*(short *)((int)data + 6) >= 0x800))) {
      goto LAB_check;
    }
    if ((-0x1000 < *(short *)((int)data + 6)) && (*data != 0xe0c0301)) {
      return 0xffffffff;
    }
  } else if ((stage != 9) && (stage != 0xc)) {
    return 0xffffffff;
  }
LAB_check:
  if ((flags & 1) != 0) { stage = -0xc; }
  gLinkStage = GetRandomSpawnFromStage((PlayerSpawnPoint *)linkCoords,stage,flags);
  return gLinkStage;
}
// staircase location matcher
int StairLocationMatcher(int *outDir1,int *outDir2,int angle)
{
  byte bVar1 = ((u32 **)&gStairDirPtr)[gLinkChunkIdx][gLinkInnerIdx];
  int iVar2 = AngleDistCheck(angle,(u32)bVar1);
  if (iVar2 == 0) { return 0; }
  if (outDir2 != NULL) {
    *outDir2 = (u32)&gLinkDirData + (u32)bVar1 * 0xc;
  }
  if (outDir1 != NULL) {
    *outDir1 = (u32)&gLinkDirData + (u32)(byte)((u32 **)&gStairStagePtr)[gLinkStage][gLinkDirIdx] * 0xc;
  }
  return 1;
}
// division-based interpolation
int InterpolateDiv(int a,int b,int t)
{
  if (t < 0) { t = t + 0x3ff; }
  int iVar2 = *(int *)(b + 8) - *(int *)(a + 8);
  if (iVar2 < 0) { iVar2 = iVar2 + 0x3ff; }
  iVar2 = iVar2 >> 10;
  if (iVar2 == 0) { iVar2 = 1; }
  int iVar1 = (*(int *)(b + 4) - *(int *)(a + 4)) * (t >> 10);
  return iVar1 / iVar2 + *(int *)(a + 4);
}
// position calculation function
int CalcDreamPosition(int obj,int *outPos,int tick,int margin,int pt)
{
  int local_28;
  int local_24;
  int local_20;
  gPosCalcStored = tick;
  MathTransformByMatrix(obj,&local_28,&gPosCalcUnk,0);
  local_24 = InterpolateDiv(*(int *)(obj + 0x5c) + 0x14,*(int *)(obj + 0x5c) + 0x20,tick);
  int iVar1;
  if (*(int *)(obj + 0xc) == 0) {
    iVar1 = 0;
  } else {
    iVar1 = *(int *)(obj + 0x14) + 0x38;
  }
  local_24 = local_24 + *(int *)(iVar1 + 4);
  if (outPos != NULL) {
    *outPos = local_28;
    outPos[1] = local_24;
    outPos[2] = local_20;
  }
  if (margin == 0) {
    return 0;
  }
  return MathIsPointInBox(&local_28,pt,margin);
}
// counter/state update
void CounterUpdate(int obj)
{
  u32 uVar2 = *(u32 *)(obj + 0xa0);
  *(int *)(obj + 0xa8) = 0;
  if (uVar2 != 0) {
    if ((uVar2 & 1) == 0) {
      *(int *)(obj + 0xa0) = uVar2 - 1;
    } else {
      *(int *)(obj + 0xa0) = uVar2 + 1;
    }
  }
}
// init/check logic
void InitCheckLogic(int obj,int *data)
{
  if (*data == 1) {
    if (data[1] == (data[1] / 0x14) * 0x14) {
      data[7] = 9;
      data[8] = -1;
    } else {
      data[0xc] = 9;
      data[0xd] = -1;
    }
  }
}
// game logic with sub-object dispatch
void DispatchSubObject(int obj)
{
  int *piVar4 = *(int **)(obj + 0x58);
  int iVar2 = *(int *)(obj + 0xb8);
  int iVar5 = *piVar4;
  if (iVar2 != 0) {
    char cVar1 = ((char *)&gSubObjParamTbl)[iVar2];
    (**(code **)(iVar5 + 0x9c))(piVar4,(int)(char)((char *)&gSubObjParam2Tbl)[iVar2]);
    int uVar3 = (**(code **)(iVar5 + 0x80))(piVar4,(int)cVar1 << 4,0x6e,0x6e);
    *(int *)(obj + 0xbc) = uVar3;
    if (*(int *)(obj + 0xb8) != 0x16) {
      *(int *)(obj + 0xbc) = 0xffffffff;
    }
    if (*(int *)(obj + 0xb8) == 0xb) {
      (**(code **)(iVar5 + 0x9c))(piVar4,1);
      (**(code **)(iVar5 + 0x80))(piVar4,(int)cVar1 << 4,0x6e,0x6e);
      (**(code **)(iVar5 + 0x9c))(piVar4,2);
      (**(code **)(iVar5 + 0x80))(piVar4,0x90,0x6e,0x6e);
    }
  }
}
// vector diff + vtable dispatch
void StairVecDiff(int *obj,short *src,short *dst)
{
  int local_18[4];
  local_18[0] = (int)*src - (int)*dst;
  local_18[1] = 0;
  local_18[2] = (int)src[2] - (int)dst[2];
  (**(code **)(*obj + 0xbc))(obj,local_18);     /* vtable[47] */
}
// data copy (struct to backup)
void BackupStructData(int obj)
{
  int uVar1, uVar2, uVar3;
  int *puVar4 = *(int **)(obj + 0x14);
  int *puVar6 = (int *)(obj + 0x890);
  int *puVar4_start = puVar4;
  do {
    uVar1 = puVar4[1]; uVar2 = puVar4[2]; uVar3 = puVar4[3];
    *puVar6 = *puVar4; puVar6[1] = uVar1; puVar6[2] = uVar2; puVar6[3] = uVar3;
    puVar4 += 4; puVar6 += 4;
  } while (puVar4 != puVar4_start + 0x14);
  puVar4 = (int *)puVar4_start[0x11];
  puVar6 = (int *)(obj + 0x8e0);
  do {
    uVar1 = puVar4[1]; uVar2 = puVar4[2]; uVar3 = puVar4[3];
    *puVar6 = *puVar4; puVar6[1] = uVar1; puVar6[2] = uVar2; puVar6[3] = uVar3;
    puVar4 += 4; puVar6 += 4;
  } while (puVar4 != (int *)((int)puVar4_start[0x11] + 0x20));
  puVar6[4] = *puVar4;
  puVar6[5] = puVar4[1];
}
// data copy (backup to struct)
void RestoreStructData(int obj)
{
  int uVar1, uVar2, uVar3;
  int *puVar6 = (int *)(obj + 0x890);
  int *puVar8 = *(int **)(obj + 0x14);
  int *puVar8_start = puVar8;
  do {
    uVar1 = puVar6[1]; uVar2 = puVar6[2]; uVar3 = puVar6[3];
    *puVar8 = *puVar6; puVar8[1] = uVar1; puVar8[2] = uVar2; puVar8[3] = uVar3;
    puVar6 += 4; puVar8 += 4;
  } while (puVar6 != (int *)(obj + 0x8e0));
  puVar6 = (int *)(obj + 0x8e0);
  puVar8 = (int *)puVar8_start[0x11];
  do {
    uVar1 = puVar6[1]; uVar2 = puVar6[2]; uVar3 = puVar6[3];
    *puVar8 = *puVar6; puVar8[1] = uVar1; puVar8[2] = uVar2; puVar8[3] = uVar3;
    puVar6 += 4; puVar8 += 4;
  } while (puVar6 != (int *)(obj + 0x900));
  puVar8[4] = *puVar6;
  puVar8[5] = puVar6[1];
  *puVar8_start = 0;
}
