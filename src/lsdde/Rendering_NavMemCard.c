/* Rendering_NavMemCard.c -- split from Rendering.c */
#include "Rendering_Shared.h"
void NavMenu_CloseNav(int index)
{
  int iVar1;
  if (*(int **)(index + 0xac) != NULL) {
    ((int (*)(void))(*(void **)(**(int **)(index + 0xac) + 4)))();
    ((int (*)(void))(*(void **)(**(int **)(index + 0xa8) + 4)))();
  }
  iVar1 = NavMenu_GetDataPtr();
  ((int (*)(int))(*(void **)(iVar1 + 0xc)))(index);
}

void NavMenu_HandleMessage(int *index, uint *id, uint id_2)
{
  int iVar1;
  iVar1 = NavMenu_GetDataPtr();
  ((int (*)(int,int,int))(*(void **)(iVar1 + 0x38)))(index,id,id);
  if ((*(uint *)*id & 0xf) == 0xb) {
    ((int (*)(int,int,int))(*(void **)(*index + 0x138)))(index,id,id);
  }
}

void NavMenu_InitTitle(int *index)
{
  ((EntityObj *)index)->nField_34 = 0;
  ((EntityObj *)index)->nVabIdx = 400;
  ((int (*)(int,int,int))(*(void **)(*index + 0xd4)))(index,"ETC\\TITLE.TIM",0);
  ((int (*)(int,int))(*(void **)(*index + 0x6c)))(index,10);
  ((int (*)(int,int,int))(*(void **)(*(int *)((EntityObj *)index)->field_0xa4 + 0xf0)))((int *)((EntityObj *)index)->field_0xa4,0,0);
}

void NavMenu_DrawAc(int index)
{
  void *puVar1;
  uint uVar2;
  uVar2 = 0;
  puVar1 = &Rendering_McAc;
  do {
    uVar2 = uVar2 + 1;
    ((int (*)(int,int,int))(*(void **)(*(int *)**(uint **)(index + 0xc) + 0x78)))((int *)**(uint **)(index + 0xc),index + 0x93,puVar1);
    puVar1 = puVar1 + 0xc;
  } while (uVar2 < 2);
}

void NavMenu_HandleState(int *index, int index_2)
{
  int iVar1;
  iVar1 = NavMenu_GetDataPtr();
  ((int (*)(int,int))(*(void **)(iVar1 + 0x60)))(index,index);
  if (index_2 == 5) {
    ((int (*)(int,int))(*(void **)(*index + 0x124)))(index,0);
  }
  if (index_2 == 10) {
    ((int (*)(int))(*(void **)(*index + 0x7c)))(index);
    ((int (*)(int,int,int))(*(void **)(*index + 0xf0)))(index,*(uint *)(((EntityObj *)index)->pObject + 8),1);
    ((int (*)(int))(*(void **)(*index + 0x78)))(index);
  }
}

void NavMenu_Update(int *index)
{
  int iVar1;
  code *pcVar2;
  iVar1 = NavMenu_GetDataPtr();
  ((int (*)(int))(*(void **)(iVar1 + 0x90)))(index);
  iVar1 = ((EntityObj *)index)->pChild;
  if (iVar1 == 2) {
    pcVar2 = *(code **)(*index + 0x130);
  }
  else {
    if (iVar1 < 3) {
      if (iVar1 != 1) {
        return;
      }
      ((EntityObj *)index)->nField_38 = 0;
      ((int (*)(int,int,int))(*(void **)(*(int *)((EntityObj *)index)->field_0xa4 + 0xf0)))((int *)((EntityObj *)index)->field_0xa4,0,1);
      iVar1 = *index;
    }
    else {
      if (iVar1 == 3) {
        pcVar2 = *(code **)(*index + 0x134);
        goto code_r0x8004daa0;
      }
      if (iVar1 != 4) {
        return;
      }
      iVar1 = *index;
      ((EntityObj *)index)->nField_38 = 2;
    }
    pcVar2 = *(code **)(iVar1 + 0x94);
  }
code_r0x8004daa0:
  ((int (*)(...))(pcVar2))(index);
}

void NavMenu_UpdateColour(int index)
{
  int iVar1;
  uint local_10 [2];
  iVar1 = NavMenu_GetDataPtr();
  ((int (*)(int))(*(void **)(iVar1 + 0x94)))(index);
  local_10[0] = *(uint *)(*(int *)(index + 0x60) + 0x14);
  ((int (*)(int,int))(*(void **)(**(int **)(index + 0xa4) + 0x19c)))(*(int **)(index + 0xa4),local_10);
}

void NavMenu_CreateSaveText(int index,int value)
{
  int iVar1;
  uint uVar2;
  uint uVar3;
  if (value != 0) {
    iVar1 = ((int (*)(void))(*(void **)(**(int **)(index + 0xa4) + 0x1ac)))();
    if (iVar1 != 0) {
      strcpy(&Rendering_Unk114b4,&Rendering_Unk11474);
      Gpu_SetupCardPrimitive(&Rendering_Unk1149c,0);
    }
    uVar2 = strlen(&Rendering_Unk1149c);
    iVar1 = (uVar2 >> 1) + 4;
    uVar3 = MemAllocImpl(iVar1);
    Sjis_Decode(uVar3,&Rendering_Unk1149c);
    iVar1 = TextInput_New(*(uint *)(value + 4),iVar1,uVar3);
    *(int *)(index + 0xb0) = iVar1;
    *(u8 *)(iVar1 + 0xab) = 8;
    *(u8 *)(*(int *)(index + 0xb0) + 0xac) = 4;
    *(u8 *)(*(int *)(index + 0xb0) + 0xaa) = 9;
    MemFreeImpl(uVar3);
  }
}

void NavMenu_DestroyNav(int index)
{
  int iVar1;
  ((int (*)(void))(*(void **)(**(int **)(index + 0xb0) + 4)))();
  iVar1 = NavMenu_GetDataPtr();
  ((int (*)(int))(*(void **)(iVar1 + 0xdc)))(index);
}

void NavMenu_Poll(int index,uint id)
{
  int iVar1;
  iVar1 = NavMenu_GetDataPtr();
  ((int (*)(int,int))(*(void **)(iVar1 + 0xe0)))(index,id);
  ((int (*)(int,int,int))(*(void **)(**(int **)(index + 0xb0) + 0x4c)))(*(int **)(index + 0xb0),id,&Rendering_NavB4);
}

void NavMenu_DrawChar(int index,char *c)
{
  char cVar1;
  int iVar2;
  char *pcVar3;
  char local_18 [8];
  iVar2 = NavMenu_GetDataPtr();
  ((int (*)(int,int))(*(void **)(iVar2 + 0xe4)))(index,c);
  if (*(int *)(index + 0x3c) == 0) {
    local_18[0] = *c;
    local_18[1] = c[1];
    local_18[2] = c[2];
    if (Rendering_McState2c < 0x80) {
      local_18[0] = *c + -0x80;
      goto LAB_8004dd88;
    }
    pcVar3 = local_18 + Rendering_McState28;
    cVar1 = *pcVar3 + -0x80;
  }
  else {
    cVar1 = -0x80;
    local_18[0] = '\0';
    local_18[1] = '\0';
    local_18[2] = '\0';
    pcVar3 = local_18 + Rendering_McState28;
  }
  *pcVar3 = cVar1;
LAB_8004dd88:
  Rendering_McState28 = Rendering_McState28 + 1;
  if (2 < Rendering_McState28) {
    Rendering_McState28 = 0;
  }
  Rendering_McState2c = Rendering_McState2c + 1;
  if (0x100 < Rendering_McState2c) {
    Rendering_McState2c = 0;
  }
  ((int (*)(int,int))(*(void **)(**(int **)(index + 0xb0) + 0xb8)))(*(int **)(index + 0xb0),local_18);
}

void NavMenu_CommitSave(int *index)
{
  uint uVar1;
  int iVar2;
  uint local_18 [2];
  iVar2 = ((EntityObj *)index)->pChild;
  uVar1 = MemAllocImpl(*(u8 *)(((EntityObj *)index)->field_0xb0 + 0xa9));
  Sjis_Decode(uVar1,&Rendering_Unk1149c);
  ((int (*)(int,int))(*(void **)(*(int *)((EntityObj *)index)->field_0xb0 + 0xcc)))((int *)((EntityObj *)index)->field_0xb0,uVar1);
  MemFreeImpl(uVar1);
  StageEntity_SetDreamFlag(index,((EntityObj *)index)->pObject);
  ((int (*)(int,int))(*(void **)(*index + 0xe0)))(index,index[5]);
  ((int (*)(int,int))(*(void **)(*(int *)((EntityObj *)index)->field_0xa4 + 0x19c)))((int *)((EntityObj *)index)->field_0xa4,local_18);
  ((EntityObj *)index)->pChild = 5;
  ((int (*)(int,int))(*(void **)(*index + 0x60)))(index,0xb);
  ((int (*)(int,int,int))(*(void **)(*index + 0x11c)))(index,local_18[0],1);
  ((int (*)(int,int))(*(void **)(*index + 0x60)))(index,0xf);
  ((int (*)(int,int,int))(*(void **)(*index + 0xf0)))(index,iVar2,0);
  ((int (*)(int,int))(*(void **)(*(int *)((EntityObj *)index)->field_0xa4 + 0x19c)))((int *)((EntityObj *)index)->field_0xa4,local_18);
}

void NavMenu_DrawFileList(int *index)
{
  int *piVar1;
  int iVar2;
  piVar1 = (int *)((EntityObj *)index)->field_0xac;
  if (piVar1 == NULL) {
    iVar2 = Texture_New("CARD\\FILEICN1.TIM");
    ((EntityObj *)index)->field_0xa8 = iVar2;
    iVar2 = MemoryCard_New(1,0);
    ((EntityObj *)index)->field_0xac = iVar2;
    piVar1 = (int *)((EntityObj *)index)->field_0xac;
  }
  ((int (*)(int,int,int,int,int,int,int))(*(void **)(*piVar1 + 0x6c)))(((EntityObj *)index)->field_0xac,"BISLPS-01556",&Rendering_McSavePath,*(uint *)(index[3] + 4),
             index[4],index[5],((EntityObj *)index)->nStateFlag);
  ((int (*)(int,int))(*(void **)(*index + 0x10)))(index,((EntityObj *)index)->field_0xac);
  ((int (*)(int,int))(*(void **)(*index + 0x14)))(index,*(uint *)(index[3] + 4));
  ((int (*)(int,int))(*(void **)(*index + 0x14)))(index,index[4]);
}

void NavMenu_CloseFileList(int *index)
{
  ((int (*)(int,int))(*(void **)(*index + 0x10)))(index,*(uint *)(index[3] + 4));
  ((int (*)(int,int))(*(void **)(*index + 0x10)))(index,index[4]);
  ((int (*)(int,int))(*(void **)(*index + 0x14)))(index,((EntityObj *)index)->field_0xac);
  ((int (*)(void))(*(void **)(*(int *)((EntityObj *)index)->field_0xac + 0x70)))();
}

void NavMenu_SaveFile(int *index)
{
  int iVar1;
  uint local_10 [2];
  local_10[0] = *(uint *)(((EntityObj *)index)->nField_60 + 0x14);
  ((int (*)(int,int))(*(void **)(*(int *)((EntityObj *)index)->field_0xa4 + 0x19c)))((int *)((EntityObj *)index)->field_0xa4,local_10);
  ((int (*)(int))(*(void **)(*index + 0x128)))(index);
  iVar1 = ((int (*)(void))(*(void **)(*(int *)((EntityObj *)index)->field_0xa4 + 0x1ac)))();
  if (iVar1 != 0) {
    Rendering_StrBuf80011464[0] = '\0';
  }
  ((int (*)(int,int,int,int,int,int,int,int))(*(void **)(*(int *)((EntityObj *)index)->field_0xac + 0x78)))(((EntityObj *)index)->field_0xac,Rendering_StrBuf80011464,&Rendering_Unk1149c,0xd,3,((EntityObj *)index)->field_0xa8,
             ((EntityObj *)index)->field_0xbc,((EntityObj *)index)->field_0xc0);
}

void NavMenu_LoadFile(int *index)
{
  ((int (*)(void))(*(void **)(*index + 0x128)))();
  ((int (*)(int,int,int,int,int))(*(void **)(*(int *)((EntityObj *)index)->field_0xac + 0x74)))(((EntityObj *)index)->field_0xac,Rendering_StrBuf80011464,&Rendering_Unk1149c,((EntityObj *)index)->field_0xbc,((EntityObj *)index)->field_0xc0);
}

void NavMenu_HandleInput(int *index, uint id, int index_2)
{
  if (((index_2 < 0x18) && (0x15 < index)) && (((int (*)(void))(*(void **)(*index + 300)))(), index_2 == 0x16))
  {
    ((int (*)(void))(*(void **)(*(int *)((EntityObj *)index)->field_0xa4 + 0x1a8)))();
    ((int (*)(int,int))(*(void **)(*index + 0x124)))(index,0x16);
  }
}
void * MemoryCard_GetVtable(void)
{
  return &Stage_EntityVtable;
}

int MemoryCard_New(uint id,uint type)
{
  int iVar1;
  int iVar2;
  iVar1 = MemAllocImpl(0x84);
  iVar2 = 0;
  if (iVar1 != 0) {
    iVar2 = MemoryCard_GetVtable();
    ((int (*)(int,int,int))(*(void **)(iVar2 + 8)))(iVar1,id,type);
    iVar2 = iVar1;
  }
  return iVar2;
}

void MemoryCard_Constructor(int *index,uint id,uint type)
{
  bool bVar1;
  int iVar2;
  iVar2 = GetCoordSystemVtable();
  ((int (*)(int))(*(void **)(iVar2 + 8)))(index);
  iVar2 = MemoryCard_GetVtable();
  *index = iVar2;
  iVar2 = Rendering_McState30 + 1;
  bVar1 = Rendering_McState30 == 0;
  Rendering_McState30 = iVar2;
  if (bVar1) {
    InitCARD(id);
    StartCARD();
    _bu_init();
  }
  MemoryCard_ClearFields(index);
  ((int (*)(int,int))(*(void **)(*index + 0x40)))(index,type);
}

void MemoryCard_ClearFields(int index)
{
  *(uint *)(index + 0x60) = 0;
  *(uint *)(index + 100) = 0;
  *(uint *)(index + 0x68) = 0;
  *(uint *)(index + 0x78) = 0;
  *(uint *)(index + 0x7c) = 0;
}

void MemoryCard_Destructor(uint id)
{
  int iVar1;
  iVar1 = GetCoordSystemVtable();
  ((int (*)(int))(*(void **)(iVar1 + 0xc)))(id);
}

void MemoryCard_SetBuffer(int index,uint *id)
{
  int iVar1;
  uint uVar2;
  uint uVar3;
  if (id != (uint *)0x0) {
    iVar1 = GetCoordSystemVtable();
    ((int (*)(int,int))(*(void **)(iVar1 + 0x10)))(index,id);
    uVar2 = *(uint *)*id & 0xf;
    if (uVar2 == 2) {
      *(uint **)(index + 0x60) = id;
    }
    else {
      uVar3 = *(uint *)*id & 0xff;
      if (uVar2 == 5) {
        *(uint **)(index + 100) = id;
      }
      else if (uVar3 == 0x10) {
        *(uint **)(index + 0x78) = id;
      }
      else if (uVar3 == 0x20) {
        *(uint **)(index + 0x7c) = id;
      }
    }
  }
}

void MemoryCard_ClearBuffer(int index,uint *id)
{
  int iVar1;
  uint uVar2;
  uint uVar3;
  if (id != (uint *)0x0) {
    uVar2 = *(uint *)*id & 0xf;
    if (uVar2 == 2) {
      *(uint *)(index + 0x60) = 0;
    }
    else {
      uVar3 = *(uint *)*id & 0xff;
      if (uVar2 == 5) {
        *(uint *)(index + 100) = 0;
      }
      else if (uVar3 == 0x10) {
        *(uint *)(index + 0x78) = 0;
      }
      else if (uVar3 == 0x20) {
        *(uint *)(index + 0x7c) = 0;
      }
    }
    iVar1 = GetCoordSystemVtable();
    ((int (*)(int,int))(*(void **)(iVar1 + 0x14)))(index,id);
  }
}

void MemoryCard_ResetBuffers(int index)
{
  int iVar1;
  *(uint *)(index + 0x60) = 0;
  *(uint *)(index + 100) = 0;
  *(uint *)(index + 0x68) = 0;
  *(uint *)(index + 0x78) = 0;
  *(uint *)(index + 0x7c) = 0;
  iVar1 = GetCoordSystemVtable();
  ((int (*)(int))(*(void **)(iVar1 + 0x18)))(index);
}

void MemoryCard_SetFileNo(int index,int value)
{
  *(int *)(index + 0xc) = value;
  *(int *)(index + 0x10) = value << 4;
}

uint MemoryCard_OpenEvents(int index)
{
  uint uVar1;
  uint *puVar2;
  int iVar3;
  int iVar4;
  NopSub_24ce0();
  iVar4 = 0;
  puVar2 = &Stage_McEventStatusResults;
  iVar3 = index;
  do {
    uVar1 = *puVar2;
    puVar2 = puVar2 + 1;
    iVar4 = iVar4 + 1;
    uVar1 = OpenEvent(0xf4000001,uVar1,0x2000,0);
    *(uint *)(iVar3 + 0x14) = uVar1;
    iVar3 = iVar3 + 4;
  } while (iVar4 < 4);
  NopSub_24cf0();
  MemoryCard_EnableEvents(index);
  return 1;
}

uint MemoryCard_CloseEvents(uint id)
{
  MemoryCard_DisableEvents();
  MemoryCard_CallEventFunc(id,CloseEvent,1);
  return 1;
}

int MemoryCard_ReadFile(uint *id, int *index, uint *type, int value)
{
  bool bVar1;
  int iVar2;
  int iVar3;
  uint local_20 [2];
  iVar3 = 10;
  *id = 0;
  iVar2 = MemoryCard_DetectAndLoad(id,index,local_20);
  do {
    if (((iVar2 != 0) && (*index == 0)) && (*type != 0)) break;
    iVar2 = MemoryCard_DetectAndLoad(id,index,id,type);
    bVar1 = iVar3 != 0;
    iVar3 = iVar3 + -1;
  } while (bVar1);
  *id = *id | local_20[0];
  return iVar2;
}

void MemoryCard_DetectAndLoad(uint id, uint id_2, uint type, uint type_2)
{
  int iVar1;
  iVar1 = MemoryCard_DetectCard(id,id,type_2);
  if (iVar1 != 0) {
    MemoryCard_LoadCardData(id,id,type_2);
  }
}

uint MemoryCard_DetectCard(int index,uint *id,uint *type)
{
  int iVar1;
  uint uVar2;
  *id = 0;
  *type = 0;
  MemoryCard_TestEvents();
  do {
    iVar1 = _card_info(*(uint *)(index + 0x10));
  } while (iVar1 == 0);
  iVar1 = MemoryCard_PollCardStatus(index);
  if (iVar1 == 0x100) {
    uVar2 = 0;
  }
  else if (iVar1 == 0x8000) {
    *id = 1;
    uVar2 = 0;
  }
  else {
    uVar2 = 1;
    if (iVar1 == 0x2000) {
      *type = 1;
      _card_clear(*(uint *)(index + 0x10));
      uVar2 = 1;
    }
  }
  return uVar2;
}

uint MemoryCard_LoadCardData(int index,uint *id,uint *type)
{
  int iVar1;
  uint uVar2;
  *id = 0;
  *type = 1;
  MemoryCard_TestEvents();
  do {
    iVar1 = _card_load(*(uint *)(index + 0x10));
  } while (iVar1 == 0);
  iVar1 = MemoryCard_PollCardStatus(index);
  if (iVar1 == 0x100) {
    uVar2 = 0;
  }
  else if (iVar1 == 0x8000) {
    *id = 1;
    uVar2 = 0;
  }
  else {
    uVar2 = 1;
    if (iVar1 == 0x2000) {
      *type = 0;
      uVar2 = 1;
    }
  }
  return uVar2;
}

int MemoryCard_Format(int index)
{
  bool bVar1;
  int iVar2;
  uint *puVar3;
  int iVar4;
  iVar4 = 10;
  do {
    puVar3 = &Stage_McPathPrefix0;
    if (*(int *)(index + 0xc) != 0) {
      puVar3 = &Stage_McPathPrefix1;
    }
    iVar2 = format(puVar3);
  } while ((iVar2 == 0) && (bVar1 = iVar4 != 0, iVar4 = iVar4 + -1, bVar1));
  return iVar2;
}

int MemoryCard_ReadFileRetry(uint id,uint type,char *c)
{
  bool bVar1;
  int iVar2;
  int iVar3;
  iVar3 = 0;
  if ((c != (char *)0x0) && (*c != '\0')) {
    do {
      iVar2 = MemoryCard_ReadFileTitle(id,type,c);
      if (iVar2 != 0) {
        return iVar2;
      }
      bVar1 = iVar3 != 0;
      iVar3 = iVar3 + -1;
    } while (bVar1);
  }
  return 0;
}

uint MemoryCard_ReadFileTitle(int index, char *type, char *flags)
{
  char *__file;
  int __fd;
  uint uVar1;
  void *__buf;
  u8 auStack_30 [32];
  __file = (char *)MemoryCard_BuildPath(auStack_30,*(uint *)(index + 0xc),flags);
  __fd = open(__file,1);
  uVar1 = 0;
  if (__fd != -1) {
    if (type != (char *)0x0) {
      __buf = (void *)MemAllocImpl(0x80);
      read(__fd,__buf,0x80);
      strcpy(flags,(char *)((int)__buf + 4));
      MemFreeImpl(__buf);
    }
    close(__fd);
    uVar1 = 1;
  }
  return uVar1;
}

char MemoryCard_FindFileEntry(int *index, char c, char c_2, int *value)
{
  int iVar1;
  iVar1 = *value;
  while( true ) {
    if (iVar1 == 0) {
      return (char *)0x0;
    }
    strcpy(c,c);
    strcat(c,(char *)*value);
    iVar1 = ((int (*)(int,int,int))(*(void **)(*index + 0x54)))(index,0,c);
    value = value + 1;
    if (iVar1 == 0) break;
    iVar1 = *value;
  }
  return c;
}

int MemoryCard_FindFiles(int *index,uint *id,int *value,char *c,int *arg2)
{
  int iVar1;
  int iVar2;
  char acStack_40 [32];
  iVar2 = 0;
  iVar1 = *arg2;
  while (iVar1 != 0) {
    strcpy(acStack_40,c);
    strcat(acStack_40,(char *)*arg2);
    iVar1 = ((int (*)(int,int,int))(*(void **)(*index + 0x54)))(index,*id,acStack_40);
    if (iVar1 != 0) {
      iVar2 = iVar2 + 1;
      id = id + 1;
      *value = *arg2;
      value = value + 1;
    }
    arg2 = arg2 + 1;
    iVar1 = *arg2;
  }
  return iVar2;
}

int MemoryCard_DeleteFileRetry(uint id,u8 arg1,uint type)
{
  bool bVar1;
  int iVar2;
  int iVar3;
  iVar3 = 10;
  do {
    iVar2 = MemoryCard_DeleteFile(id,arg1,type);
    if (iVar2 != 0) {
      return iVar2;
    }
    bVar1 = iVar3 != 0;
    iVar3 = iVar3 + -1;
  } while (bVar1);
  return 0;
}

bool MemoryCard_DeleteFile(int index,uint id,int value)
{
  char *__file;
  int __fd;
  u8 auStack_28 [32];
  __file = (char *)MemoryCard_BuildPath(auStack_28,*(uint *)(index + 0xc),&Stage_McDeleteFilename);
  __fd = open(__file,(value + 0x21ffU >> 0xd) << 0x10 | 0x200);
  if (__fd != -1) {
    close(__fd);
    delete(auStack_28);
  }
  return __fd != -1;
}

int MemoryCard_ReadDataRetry(uint id,uint type,uint flags,uint arg3)
{
  bool bVar1;
  int iVar2;
  int iVar3;
  iVar3 = 10;
  do {
    iVar2 = MemoryCard_ReadDataFile(id,type,flags,arg3);
    if (iVar2 != 0) {
      return iVar2;
    }
    bVar1 = iVar3 != 0;
    iVar3 = iVar3 + -1;
  } while (bVar1);
  return 0;
}

bool MemoryCard_ReadDataFile(int id,uint type,void *flags,size_t arg0)
{
  byte bVar1;
  char *__file;
  int __fd;
  void *__buf;
  u8 auStack_38 [32];
  __file = (char *)MemoryCard_BuildPath(auStack_38,*(uint *)(id + 0xc),id);
  __fd = open(__file,1);
  if (__fd != -1) {
    __buf = (void *)MemAllocImpl(0x80);
    read(__fd,__buf,0x80);
    bVar1 = *(byte *)((int)__buf + 2);
    MemFreeImpl(__buf);
    lseek(__fd,(uint)bVar1 * 0x80 + -0x780,0);
    read(__fd,flags,arg0);
    close(__fd);
  }
  return __fd != -1;
}
int MemoryCard_WriteDataRetry(uint id, uint type, uint flags, u8 arg3,
    uint id_3, uint type_3, uint flags_3)
{
  bool bVar1;
  int iVar2;
  int iVar3;
  iVar3 = 10;
  Gpu_SetupCardPrimitive(flags,type);
  do {
    iVar2 = MemoryCard_WriteFileSaveData(id,type,flags,arg3,id_3,type_3,flags_3);
    if (iVar2 != 0) {
      return iVar2;
    }
    bVar1 = iVar3 != 0;
    iVar3 = iVar3 + -1;
  } while (bVar1);
  Gpu_SetupCardPrimitive(flags,0);
  return 0;
}
uint
MemoryCard_WriteFileSaveData(int index,uint id,char *c,byte val,int value,void *arg1,
            int arg4)
{
  u8 *puVar1;
  uint *puVar2;
  char *pcVar3;
  int iVar4;
  uint uVar5;
  u8 *__buf;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  char *pcVar9;
  uint uVar10;
  uint uVar11;
  uint uVar12;
  uint uVar13;
  uint uVar14;
  uint *puVar15;
  uint *puVar16;
  int iVar17;
  long long uVar18;
  u8 auStack_48 [32];
  pcVar3 = (char *)MemoryCard_BuildPath(auStack_48,*(uint *)(index + 0xc),id);
  delete(pcVar3);
  iVar4 = open(pcVar3,(arg4 + 0x21ffU >> 0xd) << 0x10 | 0x200);
  if (iVar4 == -1) {
    printf("File not create in WriteFile\n");
    uVar5 = 0;
  }
  else {
    close(iVar4);
    iVar4 = open(pcVar3,2);
    uVar5 = 0;
    if (iVar4 != -1) {
      iVar17 = *(int *)(value + 0x10);
      uVar18 = MemAllocImpl(0x200);
      uVar8 = (uint)((ulonglong)uVar18 >> 0x20);
      __buf = (u8 *)uVar18;
      pcVar9 = __buf + 4;
      *__buf = 0x53;
      __buf[1] = 0x43;
      __buf[2] = val + 0x10;
      __buf[3] = (char)(arg4 + 0x1fffU >> 0xd);
      pcVar3 = strcpy(pcVar9,c);
      puVar16 = (uint *)(__buf + 0x80);
      uVar7 = iVar17 + 0x17U & 3;
      uVar13 = iVar17 + 0x14U & 3;
      uVar6 = (*(int *)((iVar17 + 0x17U) - uVar7) << (3 - uVar7) * 8 |
              (uint)pcVar3 & 0xffffffffU >> (uVar7 + 1) * 8) & -1 << (4 - uVar13) * 8 |
              *(uint *)((iVar17 + 0x14U) - uVar13) >> uVar13 * 8;
      uVar7 = iVar17 + 0x1bU & 3;
      uVar13 = iVar17 + 0x18U & 3;
      uVar8 = (*(int *)((iVar17 + 0x1bU) - uVar7) << (3 - uVar7) * 8 |
              uVar8 & 0xffffffffU >> (uVar7 + 1) * 8) & -1 << (4 - uVar13) * 8 |
              *(uint *)((iVar17 + 0x18U) - uVar13) >> uVar13 * 8;
      uVar7 = iVar17 + 0x1fU & 3;
      uVar13 = iVar17 + 0x1cU & 3;
      uVar10 = (*(int *)((iVar17 + 0x1fU) - uVar7) << (3 - uVar7) * 8 |
               (uint)pcVar9 & 0xffffffffU >> (uVar7 + 1) * 8) & -1 << (4 - uVar13) * 8 |
               *(uint *)((iVar17 + 0x1cU) - uVar13) >> uVar13 * 8;
      uVar7 = iVar17 + 0x23U & 3;
      uVar13 = iVar17 + 0x20U & 3;
      uVar12 = (*(int *)((iVar17 + 0x23U) - uVar7) << (3 - uVar7) * 8 |
               (uint)c & 0xffffffffU >> (uVar7 + 1) * 8) & -1 << (4 - uVar13) * 8 |
               *(uint *)((iVar17 + 0x20U) - uVar13) >> uVar13 * 8;
      puVar1 = __buf + 99;
      uVar7 = (uint)puVar1 & 3;
      *(uint *)(puVar1 + -uVar7) =
           *(uint *)(puVar1 + -uVar7) & -1 << (uVar7 + 1) * 8 | uVar6 >> (3 - uVar7) * 8;
      puVar1 = __buf + 0x60;
      uVar7 = (uint)puVar1 & 3;
      *(uint *)(puVar1 + -uVar7) =
           *(uint *)(puVar1 + -uVar7) & 0xffffffffU >> (4 - uVar7) * 8 | uVar6 << uVar7 * 8;
      puVar1 = __buf + 0x67;
      uVar7 = (uint)puVar1 & 3;
      *(uint *)(puVar1 + -uVar7) =
           *(uint *)(puVar1 + -uVar7) & -1 << (uVar7 + 1) * 8 | uVar8 >> (3 - uVar7) * 8;
      puVar1 = __buf + 100;
      uVar7 = (uint)puVar1 & 3;
      *(uint *)(puVar1 + -uVar7) =
           *(uint *)(puVar1 + -uVar7) & 0xffffffffU >> (4 - uVar7) * 8 | uVar8 << uVar7 * 8;
      puVar1 = __buf + 0x6b;
      uVar7 = (uint)puVar1 & 3;
      *(uint *)(puVar1 + -uVar7) =
           *(uint *)(puVar1 + -uVar7) & -1 << (uVar7 + 1) * 8 | uVar10 >> (3 - uVar7) * 8;
      puVar1 = __buf + 0x68;
      uVar7 = (uint)puVar1 & 3;
      *(uint *)(puVar1 + -uVar7) =
           *(uint *)(puVar1 + -uVar7) & 0xffffffffU >> (4 - uVar7) * 8 | uVar10 << uVar7 * 8;
      puVar1 = __buf + 0x6f;
      uVar7 = (uint)puVar1 & 3;
      *(uint *)(puVar1 + -uVar7) =
           *(uint *)(puVar1 + -uVar7) & -1 << (uVar7 + 1) * 8 | uVar12 >> (3 - uVar7) * 8;
      puVar1 = __buf + 0x6c;
      uVar7 = (uint)puVar1 & 3;
      *(uint *)(puVar1 + -uVar7) =
           *(uint *)(puVar1 + -uVar7) & 0xffffffffU >> (4 - uVar7) * 8 | uVar12 << uVar7 * 8;
      uVar7 = iVar17 + 0x27U & 3;
      uVar13 = iVar17 + 0x24U & 3;
      uVar6 = (*(int *)((iVar17 + 0x27U) - uVar7) << (3 - uVar7) * 8 |
              uVar6 & 0xffffffffU >> (uVar7 + 1) * 8) & -1 << (4 - uVar13) * 8 |
              *(uint *)((iVar17 + 0x24U) - uVar13) >> uVar13 * 8;
      uVar7 = iVar17 + 0x2bU & 3;
      uVar13 = iVar17 + 0x28U & 3;
      uVar8 = (*(int *)((iVar17 + 0x2bU) - uVar7) << (3 - uVar7) * 8 |
              uVar8 & 0xffffffffU >> (uVar7 + 1) * 8) & -1 << (4 - uVar13) * 8 |
              *(uint *)((iVar17 + 0x28U) - uVar13) >> uVar13 * 8;
      uVar7 = iVar17 + 0x2fU & 3;
      uVar13 = iVar17 + 0x2cU & 3;
      uVar10 = (*(int *)((iVar17 + 0x2fU) - uVar7) << (3 - uVar7) * 8 |
               uVar10 & 0xffffffffU >> (uVar7 + 1) * 8) & -1 << (4 - uVar13) * 8 |
               *(uint *)((iVar17 + 0x2cU) - uVar13) >> uVar13 * 8;
      uVar7 = iVar17 + 0x33U & 3;
      uVar13 = iVar17 + 0x30U & 3;
      uVar13 = (*(int *)((iVar17 + 0x33U) - uVar7) << (3 - uVar7) * 8 |
               uVar12 & 0xffffffffU >> (uVar7 + 1) * 8) & -1 << (4 - uVar13) * 8 |
               *(uint *)((iVar17 + 0x30U) - uVar13) >> uVar13 * 8;
      puVar1 = __buf + 0x73;
      uVar7 = (uint)puVar1 & 3;
      *(uint *)(puVar1 + -uVar7) =
           *(uint *)(puVar1 + -uVar7) & -1 << (uVar7 + 1) * 8 | uVar6 >> (3 - uVar7) * 8;
      puVar1 = __buf + 0x70;
      uVar7 = (uint)puVar1 & 3;
      *(uint *)(puVar1 + -uVar7) =
           *(uint *)(puVar1 + -uVar7) & 0xffffffffU >> (4 - uVar7) * 8 | uVar6 << uVar7 * 8;
      puVar1 = __buf + 0x77;
      uVar7 = (uint)puVar1 & 3;
      *(uint *)(puVar1 + -uVar7) =
           *(uint *)(puVar1 + -uVar7) & -1 << (uVar7 + 1) * 8 | uVar8 >> (3 - uVar7) * 8;
      puVar1 = __buf + 0x74;
      uVar7 = (uint)puVar1 & 3;
      *(uint *)(puVar1 + -uVar7) =
           *(uint *)(puVar1 + -uVar7) & 0xffffffffU >> (4 - uVar7) * 8 | uVar8 << uVar7 * 8;
      puVar1 = __buf + 0x7b;
      uVar7 = (uint)puVar1 & 3;
      *(uint *)(puVar1 + -uVar7) =
           *(uint *)(puVar1 + -uVar7) & -1 << (uVar7 + 1) * 8 | uVar10 >> (3 - uVar7) * 8;
      puVar1 = __buf + 0x78;
      uVar7 = (uint)puVar1 & 3;
      *(uint *)(puVar1 + -uVar7) =
           *(uint *)(puVar1 + -uVar7) & 0xffffffffU >> (4 - uVar7) * 8 | uVar10 << uVar7 * 8;
      puVar1 = __buf + 0x7f;
      uVar7 = (uint)puVar1 & 3;
      *(uint *)(puVar1 + -uVar7) =
           *(uint *)(puVar1 + -uVar7) & -1 << (uVar7 + 1) * 8 | uVar13 >> (3 - uVar7) * 8;
      puVar1 = __buf + 0x7c;
      uVar7 = (uint)puVar1 & 3;
      *(uint *)(puVar1 + -uVar7) =
           *(uint *)(puVar1 + -uVar7) & 0xffffffffU >> (4 - uVar7) * 8 | uVar13 << uVar7 * 8;
      puVar15 = (uint *)(iVar17 + 0x40);
      uVar7 = ((uint)puVar15 | (uint)puVar16) & 3;
      if (uVar7 == 0) {
        do {
          uVar8 = puVar15[1];
          uVar10 = puVar15[2];
          uVar13 = puVar15[3];
          *puVar16 = *puVar15;
          puVar16[1] = uVar8;
          puVar16[2] = uVar10;
          puVar16[3] = uVar13;
          puVar15 = puVar15 + 4;
          puVar16 = puVar16 + 4;
        } while (puVar15 != (uint *)(iVar17 + 0xc0));
      }
      else {
        do {
          uVar6 = (int)puVar15 + 3U & 3;
          uVar12 = (uint)puVar15 & 3;
          uVar7 = (*(int *)(((int)puVar15 + 3U) - uVar6) << (3 - uVar6) * 8 |
                  uVar7 & 0xffffffffU >> (uVar6 + 1) * 8) & -1 << (4 - uVar12) * 8 |
                  *(uint *)((int)puVar15 - uVar12) >> uVar12 * 8;
          uVar6 = (int)puVar15 + 7U & 3;
          uVar12 = (uint)(puVar15 + 1) & 3;
          uVar8 = (*(int *)(((int)puVar15 + 7U) - uVar6) << (3 - uVar6) * 8 |
                  uVar8 & 0xffffffffU >> (uVar6 + 1) * 8) & -1 << (4 - uVar12) * 8 |
                  *(uint *)((int)(puVar15 + 1) - uVar12) >> uVar12 * 8;
          uVar6 = (int)puVar15 + 0xbU & 3;
          uVar12 = (uint)(puVar15 + 2) & 3;
          uVar10 = (*(int *)(((int)puVar15 + 0xbU) - uVar6) << (3 - uVar6) * 8 |
                   uVar10 & 0xffffffffU >> (uVar6 + 1) * 8) & -1 << (4 - uVar12) * 8 |
                   *(uint *)((int)(puVar15 + 2) - uVar12) >> uVar12 * 8;
          uVar6 = (int)puVar15 + 0xfU & 3;
          uVar12 = (uint)(puVar15 + 3) & 3;
          uVar13 = (*(int *)(((int)puVar15 + 0xfU) - uVar6) << (3 - uVar6) * 8 |
                   uVar13 & 0xffffffffU >> (uVar6 + 1) * 8) & -1 << (4 - uVar12) * 8 |
                   *(uint *)((int)(puVar15 + 3) - uVar12) >> uVar12 * 8;
          uVar6 = (uint)((int)puVar16 + 3) & 3;
          puVar2 = (uint *)((u8 *)((int)puVar16 + 3) + -uVar6);
          *puVar2 = *puVar2 & -1 << (uVar6 + 1) * 8 | uVar7 >> (3 - uVar6) * 8;
          uVar6 = (uint)puVar16 & 3;
          *(uint *)((int)puVar16 - uVar6) =
               *(uint *)((int)puVar16 - uVar6) & 0xffffffffU >> (4 - uVar6) * 8 | uVar7 << uVar6 * 8
          ;
          uVar6 = (uint)((int)puVar16 + 7) & 3;
          puVar2 = (uint *)((u8 *)((int)puVar16 + 7) + -uVar6);
          *puVar2 = *puVar2 & -1 << (uVar6 + 1) * 8 | uVar8 >> (3 - uVar6) * 8;
          uVar6 = (uint)(puVar16 + 1) & 3;
          puVar2 = (uint *)((int)(puVar16 + 1) - uVar6);
          *puVar2 = *puVar2 & 0xffffffffU >> (4 - uVar6) * 8 | uVar8 << uVar6 * 8;
          uVar6 = (uint)((int)puVar16 + 0xb) & 3;
          puVar2 = (uint *)((u8 *)((int)puVar16 + 0xb) + -uVar6);
          *puVar2 = *puVar2 & -1 << (uVar6 + 1) * 8 | uVar10 >> (3 - uVar6) * 8;
          uVar6 = (uint)(puVar16 + 2) & 3;
          puVar2 = (uint *)((int)(puVar16 + 2) - uVar6);
          *puVar2 = *puVar2 & 0xffffffffU >> (4 - uVar6) * 8 | uVar10 << uVar6 * 8;
          uVar6 = (uint)((int)puVar16 + 0xf) & 3;
          puVar2 = (uint *)((u8 *)((int)puVar16 + 0xf) + -uVar6);
          *puVar2 = *puVar2 & -1 << (uVar6 + 1) * 8 | uVar13 >> (3 - uVar6) * 8;
          uVar6 = (uint)(puVar16 + 3) & 3;
          puVar2 = (uint *)((int)(puVar16 + 3) - uVar6);
          *puVar2 = *puVar2 & 0xffffffffU >> (4 - uVar6) * 8 | uVar13 << uVar6 * 8;
          puVar15 = puVar15 + 4;
          puVar16 = puVar16 + 4;
        } while (puVar15 != (uint *)(iVar17 + 0xc0));
      }
      puVar16 = (uint *)(__buf + 0x100);
      puVar15 = (uint *)(iVar17 + 0xc0);
      uVar7 = ((uint)puVar15 | (uint)puVar16) & 3;
      if (uVar7 == 0) {
        do {
          uVar8 = puVar15[1];
          uVar10 = puVar15[2];
          uVar13 = puVar15[3];
          *puVar16 = *puVar15;
          puVar16[1] = uVar8;
          puVar16[2] = uVar10;
          puVar16[3] = uVar13;
          puVar15 = puVar15 + 4;
          puVar16 = puVar16 + 4;
        } while (puVar15 != (uint *)(iVar17 + 0x140));
      }
      else {
        do {
          uVar6 = (int)puVar15 + 3U & 3;
          uVar12 = (uint)puVar15 & 3;
          uVar7 = (*(int *)(((int)puVar15 + 3U) - uVar6) << (3 - uVar6) * 8 |
                  uVar7 & 0xffffffffU >> (uVar6 + 1) * 8) & -1 << (4 - uVar12) * 8 |
                  *(uint *)((int)puVar15 - uVar12) >> uVar12 * 8;
          uVar6 = (int)puVar15 + 7U & 3;
          uVar12 = (uint)(puVar15 + 1) & 3;
          uVar8 = (*(int *)(((int)puVar15 + 7U) - uVar6) << (3 - uVar6) * 8 |
                  uVar8 & 0xffffffffU >> (uVar6 + 1) * 8) & -1 << (4 - uVar12) * 8 |
                  *(uint *)((int)(puVar15 + 1) - uVar12) >> uVar12 * 8;
          uVar6 = (int)puVar15 + 0xbU & 3;
          uVar12 = (uint)(puVar15 + 2) & 3;
          uVar10 = (*(int *)(((int)puVar15 + 0xbU) - uVar6) << (3 - uVar6) * 8 |
                   uVar10 & 0xffffffffU >> (uVar6 + 1) * 8) & -1 << (4 - uVar12) * 8 |
                   *(uint *)((int)(puVar15 + 2) - uVar12) >> uVar12 * 8;
          uVar6 = (int)puVar15 + 0xfU & 3;
          uVar12 = (uint)(puVar15 + 3) & 3;
          uVar13 = (*(int *)(((int)puVar15 + 0xfU) - uVar6) << (3 - uVar6) * 8 |
                   uVar13 & 0xffffffffU >> (uVar6 + 1) * 8) & -1 << (4 - uVar12) * 8 |
                   *(uint *)((int)(puVar15 + 3) - uVar12) >> uVar12 * 8;
          uVar6 = (uint)((int)puVar16 + 3) & 3;
          puVar2 = (uint *)((u8 *)((int)puVar16 + 3) + -uVar6);
          *puVar2 = *puVar2 & -1 << (uVar6 + 1) * 8 | uVar7 >> (3 - uVar6) * 8;
          uVar6 = (uint)puVar16 & 3;
          *(uint *)((int)puVar16 - uVar6) =
               *(uint *)((int)puVar16 - uVar6) & 0xffffffffU >> (4 - uVar6) * 8 | uVar7 << uVar6 * 8
          ;
          uVar6 = (uint)((int)puVar16 + 7) & 3;
          puVar2 = (uint *)((u8 *)((int)puVar16 + 7) + -uVar6);
          *puVar2 = *puVar2 & -1 << (uVar6 + 1) * 8 | uVar8 >> (3 - uVar6) * 8;
          uVar6 = (uint)(puVar16 + 1) & 3;
          puVar2 = (uint *)((int)(puVar16 + 1) - uVar6);
          *puVar2 = *puVar2 & 0xffffffffU >> (4 - uVar6) * 8 | uVar8 << uVar6 * 8;
          uVar6 = (uint)((int)puVar16 + 0xb) & 3;
          puVar2 = (uint *)((u8 *)((int)puVar16 + 0xb) + -uVar6);
          *puVar2 = *puVar2 & -1 << (uVar6 + 1) * 8 | uVar10 >> (3 - uVar6) * 8;
          uVar6 = (uint)(puVar16 + 2) & 3;
          puVar2 = (uint *)((int)(puVar16 + 2) - uVar6);
          *puVar2 = *puVar2 & 0xffffffffU >> (4 - uVar6) * 8 | uVar10 << uVar6 * 8;
          uVar6 = (uint)((int)puVar16 + 0xf) & 3;
          puVar2 = (uint *)((u8 *)((int)puVar16 + 0xf) + -uVar6);
          *puVar2 = *puVar2 & -1 << (uVar6 + 1) * 8 | uVar13 >> (3 - uVar6) * 8;
          uVar6 = (uint)(puVar16 + 3) & 3;
          puVar2 = (uint *)((int)(puVar16 + 3) - uVar6);
          *puVar2 = *puVar2 & 0xffffffffU >> (4 - uVar6) * 8 | uVar13 << uVar6 * 8;
          puVar15 = puVar15 + 4;
          puVar16 = puVar16 + 4;
        } while (puVar15 != (uint *)(iVar17 + 0x140));
      }
      puVar16 = (uint *)(__buf + 0x180);
      puVar15 = (uint *)(iVar17 + 0x140);
      uVar7 = ((uint)puVar15 | (uint)puVar16) & 3;
      if (uVar7 == 0) {
        do {
          uVar5 = puVar15[1];
          uVar11 = puVar15[2];
          uVar14 = puVar15[3];
          *puVar16 = *puVar15;
          puVar16[1] = uVar5;
          puVar16[2] = uVar11;
          puVar16[3] = uVar14;
          puVar15 = puVar15 + 4;
          puVar16 = puVar16 + 4;
        } while (puVar15 != (uint *)(iVar17 + 0x1c0));
      }
      else {
        do {
          uVar6 = (int)puVar15 + 3U & 3;
          uVar12 = (uint)puVar15 & 3;
          uVar7 = (*(int *)(((int)puVar15 + 3U) - uVar6) << (3 - uVar6) * 8 |
                  uVar7 & 0xffffffffU >> (uVar6 + 1) * 8) & -1 << (4 - uVar12) * 8 |
                  *(uint *)((int)puVar15 - uVar12) >> uVar12 * 8;
          uVar6 = (int)puVar15 + 7U & 3;
          uVar12 = (uint)(puVar15 + 1) & 3;
          uVar8 = (*(int *)(((int)puVar15 + 7U) - uVar6) << (3 - uVar6) * 8 |
                  uVar8 & 0xffffffffU >> (uVar6 + 1) * 8) & -1 << (4 - uVar12) * 8 |
                  *(uint *)((int)(puVar15 + 1) - uVar12) >> uVar12 * 8;
          uVar6 = (int)puVar15 + 0xbU & 3;
          uVar12 = (uint)(puVar15 + 2) & 3;
          uVar10 = (*(int *)(((int)puVar15 + 0xbU) - uVar6) << (3 - uVar6) * 8 |
                   uVar10 & 0xffffffffU >> (uVar6 + 1) * 8) & -1 << (4 - uVar12) * 8 |
                   *(uint *)((int)(puVar15 + 2) - uVar12) >> uVar12 * 8;
          uVar6 = (int)puVar15 + 0xfU & 3;
          uVar12 = (uint)(puVar15 + 3) & 3;
          uVar13 = (*(int *)(((int)puVar15 + 0xfU) - uVar6) << (3 - uVar6) * 8 |
                   uVar13 & 0xffffffffU >> (uVar6 + 1) * 8) & -1 << (4 - uVar12) * 8 |
                   *(uint *)((int)(puVar15 + 3) - uVar12) >> uVar12 * 8;
          uVar6 = (uint)((int)puVar16 + 3) & 3;
          puVar2 = (uint *)((u8 *)((int)puVar16 + 3) + -uVar6);
          *puVar2 = *puVar2 & -1 << (uVar6 + 1) * 8 | uVar7 >> (3 - uVar6) * 8;
          uVar6 = (uint)puVar16 & 3;
          *(uint *)((int)puVar16 - uVar6) =
               *(uint *)((int)puVar16 - uVar6) & 0xffffffffU >> (4 - uVar6) * 8 | uVar7 << uVar6 * 8
          ;
          uVar6 = (uint)((int)puVar16 + 7) & 3;
          puVar2 = (uint *)((u8 *)((int)puVar16 + 7) + -uVar6);
          *puVar2 = *puVar2 & -1 << (uVar6 + 1) * 8 | uVar8 >> (3 - uVar6) * 8;
          uVar6 = (uint)(puVar16 + 1) & 3;
          puVar2 = (uint *)((int)(puVar16 + 1) - uVar6);
          *puVar2 = *puVar2 & 0xffffffffU >> (4 - uVar6) * 8 | uVar8 << uVar6 * 8;
          uVar6 = (uint)((int)puVar16 + 0xb) & 3;
          puVar2 = (uint *)((u8 *)((int)puVar16 + 0xb) + -uVar6);
          *puVar2 = *puVar2 & -1 << (uVar6 + 1) * 8 | uVar10 >> (3 - uVar6) * 8;
          uVar6 = (uint)(puVar16 + 2) & 3;
          puVar2 = (uint *)((int)(puVar16 + 2) - uVar6);
          *puVar2 = *puVar2 & 0xffffffffU >> (4 - uVar6) * 8 | uVar10 << uVar6 * 8;
          uVar6 = (uint)((int)puVar16 + 0xf) & 3;
          puVar2 = (uint *)((u8 *)((int)puVar16 + 0xf) + -uVar6);
          *puVar2 = *puVar2 & -1 << (uVar6 + 1) * 8 | uVar13 >> (3 - uVar6) * 8;
          uVar6 = (uint)(puVar16 + 3) & 3;
          puVar2 = (uint *)((int)(puVar16 + 3) - uVar6);
          *puVar2 = *puVar2 & 0xffffffffU >> (4 - uVar6) * 8 | uVar13 << uVar6 * 8;
          puVar15 = puVar15 + 4;
          puVar16 = puVar16 + 4;
        } while (puVar15 != (uint *)(iVar17 + 0x1c0));
      }
      write(iVar4,__buf,(uint)val * 0x80 + 0x80);
      MemFreeImpl(__buf);
      write(iVar4,arg1,arg4 + 0x7fU & 0xffffff80);
      close(iVar4);
      uVar5 = 1;
    }
  }
  return uVar5;
}
char *MemoryCard_BuildPath(char *id, int type, char *flags)
{
  char *pcVar1;
  char cVar2;
  uint uVar3;
  uint uVar4;
  uint uVar5;
  uint *puVar6;
  puVar6 = &Stage_McPathPrefix0;
  if (type != 0) {
    puVar6 = &Stage_McPathPrefix1;
  }
  uVar3 = *puVar6;
  uVar5 = puVar6[1];
  cVar2 = *(char *)((int)puVar6 + 5);
  pcVar1 = id + 3;
  uVar4 = (uint)pcVar1 & 3;
  *(uint *)(pcVar1 + -uVar4) =
       *(uint *)(pcVar1 + -uVar4) & -1 << (uVar4 + 1) * 8 | uVar3 >> (3 - uVar4) * 8;
  uVar4 = (uint)id & 3;
  *(uint *)(id + -uVar4) =
       *(uint *)(id + -uVar4) & 0xffffffffU >> (4 - uVar4) * 8 | uVar3 << uVar4 * 8;
  id[4] = (char)uVar5;
  id[5] = cVar2;
  strcat(id,flags);
  return id;
}

void MemoryCard_EnableEvents(uint id)
{
  MemoryCard_CallEventFunc(id,EnableEvent,1);
}

void MemoryCard_DisableEvents(uint id)
{
  MemoryCard_CallEventFunc(id,DisableEvent,1);
}

void MemoryCard_TestEvents(uint id)
{
  MemoryCard_CallEventFunc(id,TestEvent,0);
}

int MemoryCard_CallEventFunc(int index,code *arg0,int value)
{
  int iVar1;
  int iVar2;
  if (value != 0) {
    NopSub_24ce0();
  }
  iVar2 = 0;
  do {
    iVar1 = ((int (*)(...))(arg0))(*(uint *)(index + 0x14));
    if (iVar1 == 0) break;
    iVar2 = iVar2 + 1;
    index = index + 4;
  } while (iVar2 < 4);
  if (value != 0) {
    NopSub_24cf0();
  }
  return iVar1;
}

void MemoryCard_PollCardStatus(int index)
{
  MemoryCard_PollEvents(index + 0x14,4);
}

uint MemoryCard_PollEvents(uint *id,int index)
{
  int iVar1;
  int iVar2;
  uint *puVar3;
  do {
    do {
      iVar2 = 0;
      puVar3 = id;
    } while (index < 1);
    do {
      iVar1 = TestEvent(*puVar3);
      if (iVar1 != 0) {
        return (&Stage_McEventStatusResults)[iVar2];
      }
      iVar2 = iVar2 + 1;
      puVar3 = puVar3 + 1;
    } while (iVar2 < index);
  } while( true );
}
void NavMenu_StateNoCard(int *index, int index_3, int value, uint id, uint type,
    int index_4, int value_3)
{
  ((EntityObj *)index)->nField_30 = index;
  ((EntityObj *)index)->nField_34 = value;
  ((EntityObj *)index)->nField_38 = 0;
  ((EntityObj *)index)->nObjState = type;
  ((EntityObj *)index)->nSubTypeCount = index_4;
  ((int (*)(int,int))(*(void **)(*index + 0x10)))(index,id);
  ((int (*)(int,int))(*(void **)(*index + 0x10)))(index,type);
  ((EntityObj *)index)->nChildCount = 0;
  index[10] = 0;
  index[9] = 0;
}

void NavMenu_StateCardInit(int *index)
{
  ((EntityObj *)index)->nSubTypeCount = 0;
  ((EntityObj *)index)->nObjState = 0;
  ((int (*)(int,int))(*(void **)(*index + 0x14)))(index,((EntityObj *)index)->nField_60);
  ((int (*)(int,int))(*(void **)(*index + 0x14)))(index,((EntityObj *)index)->nField_64);
}

void NavMenu_StateCardFormat(int *index, int index_2, int value, int arg2, int arg3)
{
  int iVar1;
  uint uVar2;
  ((EntityObj *)index)->pSomePtr = index;
  ((EntityObj *)index)->nState = value;
  ((EntityObj *)index)->nSoundId = arg2;
  index[9] = 1;
  ((EntityObj *)index)->pChild = arg3;
  iVar1 = NavMenu_CardCheckState(index);
  if (iVar1 != 0) {
    MemoryCard_FreeAllBuffers(index);
    MemoryCard_AllocBuffers(index);
    iVar1 = ((int (*)(int,int,int,int,int))(*(void **)(*index + 0x5c)))(index,((EntityObj *)index)->nField_38,((EntityObj *)index)->pOtherEntity,((EntityObj *)index)->nField_30,((EntityObj *)index)->nField_34);
    ((EntityObj *)index)->nVabIdx = iVar1;
    if (iVar1 == 0) {
      uVar2 = 0xd;
      ((EntityObj *)index)->nVabIdx = 0xf;
    }
    else {
      MemoryCard_FreeBuffers(index);
      uVar2 = 0x12;
      if (index[10] == 0xe) {
        uVar2 = 0xf;
      }
    }
    ((int (*)(int,int))(*(void **)(*index + 0x7c)))(index,uVar2);
  }
}

void MemoryCard_AllocBuffers(int index)
{
  uint uVar1;
  int iVar2;
  int iVar3;
  if (*(int *)(index + 0x38) == 0) {
    uVar1 = MemAllocImpl(0x40);
    *(uint *)(index + 0x38) = uVar1;
    iVar3 = 0;
    do {
      uVar1 = MemAllocImpl(0x41);
      iVar2 = iVar3 * 4;
      iVar3 = iVar3 + 1;
      *(uint *)(iVar2 + *(int *)(index + 0x38)) = uVar1;
    } while (iVar3 < 0xf);
    uVar1 = MemAllocImpl(0x40);
    *(uint *)(index + 0x3c) = uVar1;
  }
}

void MemoryCard_FreeBuffers(int index)
{
  uint uVar1;
  int iVar2;
  for (iVar2 = *(int *)(index + 0x2c); iVar2 < 0xf; iVar2 = iVar2 + 1) {
    uVar1 = MemFreeImpl(*(uint *)(iVar2 * 4 + *(int *)(index + 0x38)));
    *(uint *)(iVar2 * 4 + *(int *)(index + 0x38)) = uVar1;
  }
  *(uint *)(iVar2 * 4 + *(int *)(index + 0x38)) = 0;
}

void MemoryCard_FreeAllBuffers(int index)
{
  int iVar1;
  int iVar2;
  if (*(int *)(index + 0x38) != 0) {
    iVar2 = 0;
    MemFreeImpl(*(uint *)(index + 0x3c));
    if (0 < *(int *)(index + 0x2c)) {
      do {
        iVar1 = iVar2 * 4;
        iVar2 = iVar2 + 1;
        MemFreeImpl(*(uint *)(iVar1 + *(int *)(index + 0x38)));
      } while (iVar2 < *(int *)(index + 0x2c));
    }
    MemFreeImpl(*(uint *)(index + 0x38));
    *(uint *)(index + 0x38) = 0;
  }
}
void NavMenu_StateCardFileMenu(int *index, int index_3, int value, int arg2, u8 arg3, int arg4,
    int index_4, int value_3)
{
  int iVar1;
  code *pcVar2;
  uint uVar3;
  ((EntityObj *)index)->pSomePtr = index;
  ((EntityObj *)index)->nState = value;
  ((EntityObj *)index)->nStateFlag = arg2;
  index[9] = 2;
  *(u8 *)(index + 0x13) = arg3;
  ((EntityObj *)index)->nField_50 = arg4;
  ((EntityObj *)index)->nSoundId = index_4;
  ((EntityObj *)index)->pChild = value_3;
  iVar1 = NavMenu_CardCheckState(index);
  if (iVar1 != 0) {
    iVar1 = ((int (*)(int,int,int))(*(void **)(*index + 0x54)))(index,0,index);
    uVar3 = 10;
    if (iVar1 == 0) {
      iVar1 = ((int (*)(int,int,int))(*(void **)(*index + 0x60)))(index,arg3,value_3);
      uVar3 = 0x11;
      if (iVar1 == 0) {
        pcVar2 = *(code **)(*index + 0x7c);
        uVar3 = 9;
      }
      else {
        if (index[10] == 0x11) {
          uVar3 = 0xb;
        }
        pcVar2 = *(code **)(*index + 0x7c);
      }
    }
    else {
      if (index[10] == 10) {
        uVar3 = 0x11;
      }
      else if (index[10] == 0x11) {
        uVar3 = 0xb;
      }
      pcVar2 = *(code **)(*index + 0x7c);
    }
    ((int (*)(...))(pcVar2))(index,uVar3);
  }
}

uint NavMenu_CardCheckState(int *index)
{
  int iVar1;
  uint unaff_s2;
  int local_20;
  int local_1c;
  int local_18 [2];
  ((int (*)(void))(*(void **)(*index + 0x44)))();
  iVar1 = ((int (*)(int,int,int,int))(*(void **)(*index + 0x4c)))(index,&local_20,&local_1c,local_18);
  ((int (*)(int))(*(void **)(*index + 0x48)))(index);
  if (iVar1 != 0) {
    if ((local_1c == 0) && (local_18[0] != 0)) {
      return 1;
    }
    if (iVar1 != 0) {
      if (local_20 == 0) {
        if (local_1c == 0) {
          if ((local_18[0] == 0) && (unaff_s2 = 6, index[9] == 1)) {
            unaff_s2 = 5;
          }
        }
        else {
          unaff_s2 = 4;
        }
      }
      else {
        unaff_s2 = 3;
      }
      goto LAB_8004facc;
    }
  }
  unaff_s2 = 2;
LAB_8004facc:
  ((int (*)(int,int))(*(void **)(*index + 0x7c)))(index,unaff_s2);
  return 0;
}

void NavMenu_StateCardSave(int *index, uint *id, uint id_2)
{
  int iVar1;
  code *pcVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  iVar5 = *index;
  iVar1 = GetCoordSystemVtable();
  ((int (*)(int,int,int))(*(void **)(iVar1 + 0x38)))(index,id,id);
  uVar3 = *(uint *)*id & 0xf;
  if (uVar3 == 2) {
    pcVar2 = *(code **)(iVar5 + 0x88);
  }
  else {
    uVar4 = *(uint *)*id & 0xff;
    if (uVar3 == 5) {
      pcVar2 = *(code **)(iVar5 + 0x98);
    }
    else if (uVar4 == 0x10) {
      pcVar2 = *(code **)(iVar5 + 0xa4);
    }
    else {
      if (uVar4 != 0x20) {
        return;
      }
      pcVar2 = *(code **)(iVar5 + 0xb0);
    }
  }
  ((int (*)(...))(pcVar2))(index,index,id);
}

void NavMenu_StateCardLoad(int *index, int index_2)
{
  code *pcVar1;
  int iVar2;
  int iVar3;
  iVar3 = *index;
  if (index[10] == index) {
    index_2 = 0x17;
  }
  ((int (*)(int,int))(*(void **)(iVar3 + 0x30)))(index,index);
  ((int (*)(int))(*(void **)(iVar3 + 0x84)))(index);
  ((int (*)(int,int))(*(void **)(iVar3 + 0x80)))(index,index);
  ((EntityObj *)index)->nField_5c = 0;
  switch(index_2) {
  case 0x11:
    pcVar1 = *(code **)(iVar3 + 0x9c);
    break;
  case 0x12:
    pcVar1 = *(code **)(iVar3 + 0xa8);
    break;
  case 0x13:
    index_2 = 8;
    iVar2 = ((int (*)(int))(*(void **)(iVar3 + 0x50)))(index);
    if (iVar2 != 0) {
      index_2 = 0x11;
    }
    goto LAB_8004fd3c;
  case 0x14:
    if (*(char *)((EntityObj *)index)->pSomePtr == '\0') {
      ((int (*)(int,int,int,int))(*(void **)(iVar3 + 0x58)))(index,(char *)((EntityObj *)index)->pSomePtr,((EntityObj *)index)->nField_30,((EntityObj *)index)->nField_34);
    }
    index_2 = 0xc;
    iVar2 = ((int (*)(int,int,int,int,int,int,int))(*(void **)(iVar3 + 0x68)))(index,((EntityObj *)index)->pSomePtr,((EntityObj *)index)->nState,(char)((EntityObj *)index)->pObject,((EntityObj *)index)->nField_50,
                       ((EntityObj *)index)->nSoundId,((EntityObj *)index)->pChild);
    goto LAB_8004fd30;
  case 0x15:
    index_2 = 0x10;
    iVar2 = ((int (*)(int,int,int,int))(*(void **)(iVar3 + 100)))(index,((EntityObj *)index)->pSomePtr,((EntityObj *)index)->nSoundId,((EntityObj *)index)->pChild);
LAB_8004fd30:
    if (iVar2 != 0) {
      index_2 = 0x16;
    }
LAB_8004fd3c:
    ((int (*)(int,int))(*(void **)(iVar3 + 0x7c)))(index,index);
  default:
    goto switchD_8004fc70_default;
  }
  ((int (*)(...))(pcVar1))(index);
switchD_8004fc70_default:
  if (index - 0x16U < 2) {
    if ((index[9] == 1) && (((EntityObj *)index)->nField_38 != 0)) {
      iVar3 = 0;
      MemFreeImpl(((EntityObj *)index)->pOtherEntity);
      if (0 < ((EntityObj *)index)->nVabIdx) {
        do {
          iVar2 = iVar3 * 4;
          iVar3 = iVar3 + 1;
          MemFreeImpl(*(uint *)(iVar2 + ((EntityObj *)index)->nField_38));
        } while (iVar3 < ((EntityObj *)index)->nVabIdx);
      }
      MemFreeImpl(((EntityObj *)index)->nField_38);
      ((EntityObj *)index)->nField_38 = 0;
    }
    index[10] = 0;
    index[9] = 0;
  }
  else {
    index[10] = index;
  }
}

void NavMenu_StateCardDelete(int index,int value)
{
  int *piVar1;
  int *piVar2;
  char *pcVar3;
  char local_30 [32];
  if (((value < 0x11) && (*(int *)(index + 0x68) != 0)) && (*(int *)(index + 0x70) == 0)) {
    local_30[0] = '\0';
    pcVar3 = *(char **)(&Rendering_Mc80 + value * 4);
    strcat(local_30,"CARD\\");
    strcat(local_30,pcVar3);
    strcat(local_30,".TIM");
    piVar1 = (int *)Texture_New(local_30);
    ((int (*)(int))(*(void **)(*piVar1 + 0x78)))(piVar1);
    piVar2 = (int *)TextInput_NewFont(piVar1,&Rendering_McC4,0);
    *(int **)(index + 0x70) = piVar2;
    ((int (*)(int))(*(void **)(*piVar1 + 4)))(piVar1);
    ((int (*)(int,int,int))(*(void **)(*piVar2 + 0x4c)))(piVar2,*(uint *)(index + 0x68),&Rendering_Mc94);
  }
}

void NavMenu_StateCardError(int index)
{
  uint uVar1;
  if (*(int **)(index + 0x70) != NULL) {
    uVar1 = ((int (*)(void))(*(void **)(**(int **)(index + 0x70) + 4)))();
    *(uint *)(index + 0x70) = uVar1;
  }
}

void NavMenu_StateCardBusy(int *index, uint id, int index_2)
{
  code *pcVar1;
  if (index[10] != 0) {
    if (index_2 == 0x19) {
      pcVar1 = *(code **)(*index + 0x90);
    }
    else {
      if (index_2 != 0x17) {
        return;
      }
      pcVar1 = *(code **)(*index + 0x94);
    }
    ((int (*)(...))(pcVar1))();
  }
}

void NavMenu_StateCardPoll(int index,uint id)
{
  int *piVar1;
  piVar1 = *(int **)(index + 0x6c);
  if (piVar1 != NULL) {
    ((int (*)(int,int,int,int))(*(void **)(*piVar1 + 0x80)))(piVar1,id,SOUND_VOLUME_DEFAULT,SOUND_VOLUME_DEFAULT);
  }
}

void NavMenu_StateTransition(int *index)
{
  code *pcVar1;
  uint uVar2;
  int iVar3;
  iVar3 = *index;
  switch(iVar3) {
  case 2:
  case 4:
  case 10:
  case 0xe:
    ((int (*)(int,int))(*(void **)(iVar3 + 0x8c)))(index,0);
    if (index[10] == 0xe) {
      strcpy((char *)((EntityObj *)index)->pSomePtr,(char *)((EntityObj *)index)->nField_30);
      strcat((char *)((EntityObj *)index)->pSomePtr,*(char **)(((EntityObj *)index)->nCondition * 4 + ((EntityObj *)index)->pOtherEntity));
      strcpy((char *)((EntityObj *)index)->nState,*(char **)(((EntityObj *)index)->nCondition * 4 + ((EntityObj *)index)->nField_38));
    }
    if (index[9] == 2) {
      ((int (*)(int,int,int,int,int,int,int,int))(*(void **)(iVar3 + 0x78)))(index,((EntityObj *)index)->pSomePtr,((EntityObj *)index)->nState,((EntityObj *)index)->nStateFlag,(char)((EntityObj *)index)->pObject,((EntityObj *)index)->nField_50
                 ,((EntityObj *)index)->nSoundId,((EntityObj *)index)->pChild);
      return;
    }
    if (index[9] != 1) {
      return;
    }
    ((int (*)(int,int,int,int,int))(*(void **)(iVar3 + 0x74)))(index,((EntityObj *)index)->pSomePtr,((EntityObj *)index)->nState,((EntityObj *)index)->nSoundId,((EntityObj *)index)->pChild);
    return;
  case 3:
  case 5:
  case 8:
  case 9:
  case 0xc:
  case 0xd:
  case 0x10:
    ((int (*)(int,int))(*(void **)(iVar3 + 0x8c)))(index,0x10);
    pcVar1 = *(code **)(iVar3 + 0x7c);
    uVar2 = 0x17;
    break;
  case 6:
    ((int (*)(int,int))(*(void **)(iVar3 + 0x8c)))(index,0);
    pcVar1 = *(code **)(iVar3 + 0x7c);
    uVar2 = 7;
    break;
  default:
    goto switchD_80050074_caseD_7;
  }
  ((int (*)(...))(pcVar1))(index,uVar2);
switchD_80050074_caseD_7:
}

void TextInput_Constructor(int *index)
{
  int iVar1;
  iVar1 = index[10];
  if (iVar1 != 6) {
    if (iVar1 < 7) {
      if (iVar1 != 4) {
        return;
      }
    }
    else if ((iVar1 != 10) && (iVar1 != 0xe)) {
      return;
    }
  }
  ((int (*)(int,int))(*(void **)(*index + 0x8c)))(index,0x10);
  ((int (*)(int,int))(*(void **)(*index + 0x7c)))(index,0x17);
}

void NavMenu_WaitTimer(int *index)
{
  code *pcVar1;
  int iVar2;
  uint uVar3;
  iVar2 = index[10];
  if (iVar2 == 7) {
    iVar2 = ((EntityObj *)index)->nField_5c;
    ((EntityObj *)index)->nField_5c = iVar2 + 1;
    if (iVar2 < 6) {
      return;
    }
    pcVar1 = *(code **)(*index + 0x7c);
    uVar3 = 0x13;
  }
  else if (iVar2 == 0xb) {
    iVar2 = ((EntityObj *)index)->nField_5c;
    ((EntityObj *)index)->nField_5c = iVar2 + 1;
    if (iVar2 < 6) {
      return;
    }
    pcVar1 = *(code **)(*index + 0x7c);
    uVar3 = 0x14;
  }
  else {
    if (iVar2 != 0xf) {
      return;
    }
    iVar2 = ((EntityObj *)index)->nField_5c;
    ((EntityObj *)index)->nField_5c = iVar2 + 1;
    if (iVar2 < 6) {
      return;
    }
    pcVar1 = *(code **)(*index + 0x7c);
    uVar3 = 0x15;
  }
  ((int (*)(...))(pcVar1))(index,uVar3);
}

void NavMenu_DrawText(int *index)
{
  int iVar1;
  if ((((EntityObj *)index)->nObjState != 0) && (((EntityObj *)index)->nField_60 != 0)) {
    if (((EntityObj *)index)->nRndVal == 0) {
      iVar1 = TextInput_CreateBuffer(((EntityObj *)index)->nStateFlag * 2 + ((EntityObj *)index)->nState,1);
      ((EntityObj *)index)->nRndVal = iVar1;
      ((EntityObj *)index)->nConfigVal = 1;
    }
    ((int (*)(int,int))(*(void **)(*index + 0x10)))(index,((EntityObj *)index)->nRndVal);
    ((int (*)(int,int))(*(void **)(*(int *)((EntityObj *)index)->nRndVal + 0x44)))((int *)((EntityObj *)index)->nRndVal,((EntityObj *)index)->nObjState);
    ((int (*)(int,int,int,int))(*(void **)(*(int *)((EntityObj *)index)->nRndVal + 0x4c)))((int *)((EntityObj *)index)->nRndVal,((EntityObj *)index)->nField_60,((EntityObj *)index)->nField_64,((EntityObj *)index)->nSubTypeCount);
  }
}

void NavMenu_DestroyText(int index)
{
  if (((*(int *)(index + 0x68) != 0) && (*(int *)(index + 0x60) != 0)) &&
     (*(int **)(index + 0x78) != NULL)) {
    ((int (*)(void))(*(void **)(**(int **)(index + 0x78) + 0x50)))();
    ((int (*)(void))(*(void **)(**(int **)(index + 0x78) + 0x48)))();
    if (*(int *)(index + 0x74) != 0) {
      ((int (*)(void))(*(void **)(**(int **)(index + 0x78) + 4)))();
      *(uint *)(index + 0x78) = 0;
    }
  }
}

void TextInput_Destructor(int *index, uint id, int index_2)
{
  if (index_2 == 2) {
    ((int (*)(int))(*(void **)(*index + 0xa0)))(index);
    ((int (*)(int,int,int,int,int,int,int,int))(*(void **)(*index + 0x78)))(index,((EntityObj *)index)->pSomePtr,((EntityObj *)index)->nState,((EntityObj *)index)->nStateFlag,(char)((EntityObj *)index)->pObject,((EntityObj *)index)->nField_50,
               ((EntityObj *)index)->nSoundId,((EntityObj *)index)->pChild);
  }
  else if (index_2 == 3) {
    ((int (*)(int))(*(void **)(*index + 0xa0)))(index);
    ((int (*)(int,int))(*(void **)(*index + 0x7c)))(index,0x17);
  }
}

void TextInput_Update(int *index)
{
  int iVar1;
  if ((((EntityObj *)index)->nObjState != 0) && (((EntityObj *)index)->nField_60 != 0)) {
    if (((EntityObj *)index)->pSubObj == 0) {
      iVar1 = TextInput_CreateObject(((EntityObj *)index)->nField_38,1);
      ((EntityObj *)index)->pSubObj = iVar1;
      ((EntityObj *)index)->nConfigVal = 1;
    }
    ((int (*)(int,int))(*(void **)(*index + 0x10)))(index,((EntityObj *)index)->pSubObj);
    ((int (*)(int,int))(*(void **)(*(int *)((EntityObj *)index)->pSubObj + 0x44)))((int *)((EntityObj *)index)->pSubObj,((EntityObj *)index)->nObjState);
    ((int (*)(int,int,int,int))(*(void **)(*(int *)((EntityObj *)index)->pSubObj + 0x4c)))((int *)((EntityObj *)index)->pSubObj,((EntityObj *)index)->nField_60,((EntityObj *)index)->nField_64,((EntityObj *)index)->nSubTypeCount);
  }
}

void TextInput_DestroySubBuffer(int index)
{
  if (((*(int *)(index + 0x68) != 0) && (*(int *)(index + 0x60) != 0)) &&
     (*(int **)(index + 0x7c) != NULL)) {
    ((int (*)(void))(*(void **)(**(int **)(index + 0x7c) + 0x50)))();
    ((int (*)(void))(*(void **)(**(int **)(index + 0x7c) + 0x48)))();
    if (*(int *)(index + 0x74) != 0) {
      ((int (*)(void))(*(void **)(**(int **)(index + 0x7c) + 4)))();
      *(uint *)(index + 0x7c) = 0;
    }
  }
}

void TextInput_SetState(int *index, int *value, int index_2)
{
  int iVar1;
  code *pcVar2;
  uint uVar3;
  if (index_2 == 2) {
    iVar1 = ((int (*)(void))(*(void **)(*value + 0x9c)))();
    ((EntityObj *)index)->nCondition = iVar1;
    ((int (*)(int))(*(void **)(*index + 0xac)))(index);
    pcVar2 = *(code **)(*index + 0x7c);
    uVar3 = 0xe;
  }
  else {
    if (index_2 != 3) {
      return;
    }
    ((int (*)(int))(*(void **)(*index + 0xac)))(index);
    pcVar2 = *(code **)(*index + 0x7c);
    uVar3 = 0x17;
  }
  ((int (*)(...))(pcVar2))(index,uVar3);
}
void * MemoryCardStage_GetVtable(void)
{
  return &Stage_MemoryCardVtable;
}

void Gpu_SetupCardPrimitive(int index,int value)
{
  u8 uVar1;
  uint uVar2;
  uint *puVar3;
  int iVar4;
  if (value == 0) {
    uVar2 = index + 9U & 3;
    puVar3 = (uint *)((index + 9U) - uVar2);
    *puVar3 = *puVar3 & -1 << (uVar2 + 1) * 8 | 0x40814081U >> (3 - uVar2) * 8;
    uVar2 = index + 6U & 3;
    puVar3 = (uint *)((index + 6U) - uVar2);
    *puVar3 = *puVar3 & 0xffffffffU >> (4 - uVar2) * 8 | 0x40814081 << uVar2 * 8;
    *(u8 *)(index + 10) = 0x81;
                    /* Possible PsyQ macro: setLineF2() */
    *(u8 *)(index + 0xb) = 0x40;
  }
  else {
    iVar4 = 0xd;
    if (*(byte *)(value + 0xe) - 0x38 < 2) {
      iVar4 = 0xe;
    }
    *(u8 *)(index + 0x18) = 0x81;
                    /* Possible PsyQ macro: setLineF2() */
    *(u8 *)(index + 0x19) = 0x40;
    uVar2 = index + 9U & 3;
    puVar3 = (uint *)((index + 9U) - uVar2);
    *puVar3 = *puVar3 & -1 << (uVar2 + 1) * 8 | 0x40814081U >> (3 - uVar2) * 8;
    uVar2 = index + 6U & 3;
    puVar3 = (uint *)((index + 6U) - uVar2);
    *puVar3 = *puVar3 & 0xffffffffU >> (4 - uVar2) * 8 | 0x40814081 << uVar2 * 8;
    uVar2 = index + 0xdU & 3;
    puVar3 = (uint *)((index + 0xdU) - uVar2);
    *puVar3 = *puVar3 & -1 << (uVar2 + 1) * 8 | 0x63824081U >> (3 - uVar2) * 8;
    uVar2 = index + 10U & 3;
    puVar3 = (uint *)((index + 10U) - uVar2);
    *puVar3 = *puVar3 & 0xffffffffU >> (4 - uVar2) * 8 | 0x63824081 << uVar2 * 8;
    uVar2 = index + 0x11U & 3;
    puVar3 = (uint *)((index + 0x11U) - uVar2);
    *puVar3 = *puVar3 & -1 << (uVar2 + 1) * 8 | 0x99828182U >> (3 - uVar2) * 8;
    uVar2 = index + 0xeU & 3;
    puVar3 = (uint *)((index + 0xeU) - uVar2);
    *puVar3 = *puVar3 & 0xffffffffU >> (4 - uVar2) * 8 | -0x667d7e7e << uVar2 * 8;
    iVar4 = atoi((char *)(value + iVar4));
    iVar4 = (iVar4 + -1) * 2;
    uVar1 = Stage_McEncodingData2[iVar4];
    *(u8 *)(index + 8) = Stage_McEncodingData[iVar4];
    *(u8 *)(index + 9) = uVar1;
  }
}

int atoi(char *c)
{
  uint uVar1;
  byte bVar2;
  byte *pbVar3;
  byte *pbVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  iVar7 = 1;
  uVar5 = 10;
  iVar6 = 0;
  if (c == (char *)0x0) {
    iVar7 = ATOI_OBJ_11C();
    return iVar7;
  }
  do {
    pbVar3 = (byte *)c;
    c = (char *)(pbVar3 + 1);
  } while (((&Rendering_Unk66841)[*pbVar3] & 8) != 0);
  bVar2 = *pbVar3;
  if (bVar2 == 0x2d) {
    do {
      pbVar3 = pbVar3 + 1;
      iVar7 = -iVar7;
    } while (*pbVar3 == 0x2d);
    bVar2 = *pbVar3;
  }
  pbVar4 = pbVar3;
  if (bVar2 == 0x30) {
    pbVar4 = pbVar3 + 1;
    bVar2 = *pbVar4;
    if (bVar2 == 0x58) {
ATOI_OBJ_D8:
      iVar7 = ATOI_OBJ_EC();
      return iVar7;
    }
    if (bVar2 < 0x59) {
      if (bVar2 != 0x42) {
        iVar7 = ATOI_OBJ_EC();
        return iVar7;
      }
    }
    else if (bVar2 != 0x62) {
      uVar5 = 8;
      if (bVar2 != 0x78) goto code_r0x80050a34;
      goto ATOI_OBJ_D8;
    }
    pbVar4 = pbVar3 + 2;
    uVar5 = 2;
  }
code_r0x80050a34:
  while( true ) {
    bVar2 = *pbVar4;
    pbVar4 = pbVar4 + 1;
    uVar1 = todigit(bVar2);
    if (uVar5 <= uVar1) break;
    iVar6 = iVar6 * uVar5 + uVar1;
  }
  return iVar6 * iVar7;
}
/* Possible GS_106.OBJ/GsSetProjection
   Possible _OP_VDEL.OBJ/__builtin_vec_delete
   Possible _OP_VNEW.OBJ/__builtin_vec_new */
long atol(char *c)
{
  int iVar1;
  iVar1 = atoi(c);
  return iVar1;
}

uint todigit(char c)
{
  uint uVar1;
  uVar1 = (byte)c - 0x30;
  if (((&Rendering_Unk66841)[(byte)c] & 4) == 0) {
    if (((&Rendering_Unk66841)[(byte)c] & 3) != 0) {
      tolower(c);
      uVar1 = TODIGIT_OBJ_54();
      return uVar1;
    }
    uVar1 = 9999999;
  }
  return uVar1;
}
