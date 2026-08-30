/* Rendering_UI.c -- split from Rendering.c */
#include "Rendering_Shared.h"
int TextInput_CreateBuffer(uint id,uint type)
{
  int iVar1;
  int iVar2;
  iVar1 = MemAllocImpl(0x4c);
  iVar2 = 0;
  if (iVar1 != 0) {
    iVar2 = Entity2_GetVtable();
    ((int (*)(int,int,int))(*(void **)(iVar2 + 8)))(iVar1,id,type);
    iVar2 = iVar1;
  }
  return iVar2;
}

void TextInput_DrawText(int *index,char *c,uint id)
{
  bool bVar1;
  int iVar2;
  char *pcVar3;
  iVar2 = GetCoordSystemVtable();
  ((int (*)(int))(*(void **)(iVar2 + 8)))(index);
  iVar2 = Entity2_GetVtable();
  *index = iVar2;
  iVar2 = strlen(c);
  index[4] = iVar2;
  iVar2 = MemAllocImpl(iVar2 + 4);
  pcVar3 = " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
  index[10] = iVar2;
  bVar1 = true;
  iVar2 = 0;
  while (bVar1) {
    pcVar3 = pcVar3 + 1;
    bVar1 = *pcVar3 != '\0';
    iVar2 = iVar2 + 1;
  }
  index[5] = iVar2;
  TextInput_ResetRenderState(index);
  ((int (*)(int,int,int))(*(void **)(*index + 0x40)))(index,c,id);
}

void TextInput_ResetRenderState(int index)
{
  *(uint *)(index + 0x34) = 0;
  *(uint *)(index + 0x38) = 0;
  *(uint *)(index + 0x48) = 0;
}

void TextInput_GetLineCount(int index)
{
  int iVar1;
  MemFreeImpl(*(uint *)(index + 0x28));
  iVar1 = GetCoordSystemVtable();
  ((int (*)(int))(*(void **)(iVar1 + 0xc)))(index);
}

void TextInput_GetCharPos(int index,uint *id)
{
  int iVar1;
  if (id != (uint *)0x0) {
    iVar1 = GetCoordSystemVtable();
    ((int (*)(int,int))(*(void **)(iVar1 + 0x10)))(index,id);
    if ((*(uint *)*id & 0xf) == 2) {
      *(uint **)(index + 0x34) = id;
    }
    else if ((*(uint *)*id & 0xf) == 5) {
      *(uint **)(index + 0x38) = id;
    }
  }
}

void TextInput_CountChars(int index,uint *id)
{
  int iVar1;
  if (id != (uint *)0x0) {
    if ((*(uint *)*id & 0xf) == 2) {
      *(uint *)(index + 0x34) = 0;
    }
    else if ((*(uint *)*id & 0xf) == 5) {
      *(uint *)(index + 0x38) = 0;
    }
    iVar1 = GetCoordSystemVtable();
    ((int (*)(int,int))(*(void **)(iVar1 + 0x14)))(index,id);
  }
}

void TextInput_EraseChar(int index)
{
  int iVar1;
  *(uint *)(index + 0x34) = 0;
  *(uint *)(index + 0x38) = 0;
  *(uint *)(index + 0x48) = 0;
  iVar1 = GetCoordSystemVtable();
  ((int (*)(int))(*(void **)(iVar1 + 0x18)))(index);
}

void TextInput_InsertChar(int *index, uint *id, uint id_2)
{
  int iVar1;
  code *pcVar2;
  iVar1 = GetCoordSystemVtable();
  ((int (*)(int,int,int))(*(void **)(iVar1 + 0x38)))(index,id,id);
  if ((*(uint *)*id & 0xf) == 2) {
    pcVar2 = *(code **)(*index + 0x5c);
  }
  else {
    if ((*(uint *)*id & 0xf) != 5) {
      return;
    }
    pcVar2 = *(code **)(*index + 0x58);
  }
  ((int (*)(...))(pcVar2))(index,id,id);
}

void TextInput_ScrollLine(int index,char *c,int value)
{
  *(int *)(index + 0xc) = value;
  *(char **)(index + 0x24) = c;
  *(uint *)(index + 0x18) = 0;
  *(uint *)(index + 0x1c) = 0;
  if (value == 1) {
    Sjis_Decode(*(uint *)(index + 0x28));
    *(int *)(index + 0x10) = *(int *)(index + 0x10) / 2;
  }
  else {
    strcpy(*(char **)(index + 0x28),value);
  }
}

void TextInput_GetStringWidth(int index,int value)
{
  uint uVar1;
  int *piVar2;
  u8 auStack_38 [32];
  if ((value != 0) && (*(int *)(index + 0x48) == 0)) {
    uVar1 = FileLoadFromDisc(auStack_38,"COMINPUT","CARD\\",&Rendering_McPathF0);
    piVar2 = (int *)Texture_New(uVar1);
    ((int (*)(int))(*(void **)(*piVar2 + 0x78)))(piVar2);
    uVar1 = TextInput_NewFont(piVar2,&Rendering_EntVtab7c,0);
    *(uint *)(index + 0x48) = uVar1;
    ((int (*)(int))(*(void **)(*piVar2 + 4)))(piVar2);
    ((int (*)(int,int,int))(*(void **)(**(int **)(index + 0x48) + 0x4c)))(*(int **)(index + 0x48),value,&Rendering_McPathCc);
    uVar1 = FileLoadFromDisc(auStack_38,"FONTICON","CARD\\",&Rendering_McPathF0);
    piVar2 = (int *)Texture_New(uVar1);
    ((int (*)(int))(*(void **)(*piVar2 + 0x78)))(piVar2);
    uVar1 = TextInput_New(piVar2,*(uint *)(index + 0x10),*(uint *)(index + 0x28));
    *(uint *)(index + 0x44) = uVar1;
    uVar1 = TextInput_NewLabel(piVar2,0x5f);
    *(uint *)(index + 0x40) = uVar1;
    ((int (*)(int))(*(void **)(*piVar2 + 4)))(piVar2);
    ((int (*)(int,int,int))(*(void **)(**(int **)(index + 0x44) + 0x4c)))(*(int **)(index + 0x44),value,&Rendering_McPathD4);
    ((int (*)(int,int))(*(void **)(**(int **)(index + 0x44) + 0xb8)))(*(int **)(index + 0x44),&Rendering_McPathC8);
    ((int (*)(int,int,int))(*(void **)(**(int **)(index + 0x40) + 0x4c)))(*(int **)(index + 0x40),value,&Rendering_McPathDc);
  }
}

void TextInput_CleanupChain(int index)
{
  uint uVar1;
  if (*(int **)(index + 0x48) != NULL) {
    uVar1 = ((int (*)(void))(*(void **)(**(int **)(index + 0x48) + 4)))();
    *(uint *)(index + 0x48) = uVar1;
    ((int (*)(void))(*(void **)(**(int **)(index + 0x44) + 4)))();
    ((int (*)(void))(*(void **)(**(int **)(index + 0x40) + 4)))();
  }
}

void TextInput_InitState(int *index, uint id, uint type, int index_2)
{
  ((int (*)(void))(*(void **)(*index + 0x10)))();
  ((int (*)(int,int))(*(void **)(*index + 0x10)))(index,type);
  ((EntityObj *)index)->pOtherEntity = index;
  ((EntityObj *)index)->nVabIdx = 0;
  index[8] = 0;
}

void TextInput_ReleaseObjects(int *index)
{
  ((int (*)(int,int))(*(void **)(*index + 0x14)))(index,((EntityObj *)index)->nField_34);
  ((int (*)(int,int))(*(void **)(*index + 0x14)))(index,((EntityObj *)index)->nField_38);
  ((EntityObj *)index)->pOtherEntity = 0;
}

void TextInput_GetCursorPos(int *index, int index_2)
{
  ((EntityObj *)index)->nField_30 = 0;
  if (1 < index) {
    if (index_2 < 4) {
      ((int (*)(int,int))(*(void **)(*index + 0x14)))(index,((EntityObj *)index)->nField_34);
      ((int (*)(int))(*(void **)(*index + 0x48)))(index);
      ((EntityObj *)index)->nVabIdx = index;
    }
    else if (index_2 == 4) {
      ((int (*)(int,int))(*(void **)(*index + 0x30)))(index,((EntityObj *)index)->nVabIdx);
    }
  }
}

void TextInput_AutoSelectTimer(int *index)
{
  int iVar1;
  if (((((EntityObj *)index)->nVabIdx < 4) && (1 < ((EntityObj *)index)->nVabIdx)) &&
     (iVar1 = ((EntityObj *)index)->nField_30, ((EntityObj *)index)->nField_30 = iVar1 + 1, iVar1 != 0)) {
    ((int (*)(int,int))(*(void **)(*index + 0x54)))(index,4);
  }
}

void TextInput_MakeChoice(int *index,uint id,uint type)
{
  code *pcVar1;
  switch(type) {
  case 2:
    if (index[8] == 0) {
      return;
    }
    pcVar1 = *(code **)(*index + 0x90);
    break;
  case 3:
    if (index[8] == 0) {
      return;
    }
    goto LAB_800515f0;
  case 4:
    if (index[8] == 0) {
      return;
    }
    pcVar1 = *(code **)(*index + 0x8c);
    break;
  case 5:
    if (index[8] == 0) {
      return;
    }
    pcVar1 = *(code **)(*index + 0x88);
    break;
  default:
    goto switchD_80051404_caseD_6;
  case 0x12:
    if (index[8] != 0) {
      return;
    }
    pcVar1 = *(code **)(*index + 0x90);
    break;
  case 0x13:
    if (index[8] != 0) {
      return;
    }
LAB_800515f0:
    pcVar1 = *(code **)(*index + 0x94);
    break;
  case 0x14:
    if (index[8] != 0) {
      return;
    }
    pcVar1 = *(code **)(*index + 0x8c);
    break;
  case 0x15:
    if (index[8] != 0) {
      return;
    }
    pcVar1 = *(code **)(*index + 0x88);
    break;
  case 0x17:
    ((int (*)(int,int))(*(void **)(*index + 0x60)))(index,0x10);
    ((int (*)(int,int))(*(void **)(*index + 0x54)))(index,3);
    return;
  case 0x19:
    if (index[3] == 1) {
      Sjis_EncodeFromAscii(index[9],index[10]);
    }
    else {
      strcpy((char *)index[9],(char *)index[10]);
    }
    ((int (*)(int,int))(*(void **)(*index + 0x60)))(index,0x10);
    ((int (*)(int,int))(*(void **)(*index + 0x54)))(index,2);
    return;
  case 0x1c:
    pcVar1 = *(code **)(*index + 0x98);
    break;
  case 0x1f:
    pcVar1 = *(code **)(*index + 0x9c);
    break;
  case 0x20:
    pcVar1 = *(code **)(*index + 0xa0);
  }
  ((int (*)(...))(pcVar1))(index);
switchD_80051404_caseD_6:
}

void TextInput_RenderChar(int index,uint id)
{
  int *piVar1;
  piVar1 = *(int **)(index + 0x3c);
  if (piVar1 != NULL) {
    ((int (*)(int,int,int,int))(*(void **)(*piVar1 + 0x80)))(piVar1,id,0x60,0x60);
  }
}

void TextInput_UpdateCursor(int *index)
{
  int iVar1;
  int iVar2;
  if (((EntityObj *)index)->nStateFlag != 0) {
    iVar1 = index[6];
    iVar2 = iVar1 + 1;
    index[6] = iVar2;
    if (iVar2 < index[4]) {
      ((int (*)(int,int,int))(*(void **)(*index + 0xa4)))(index,iVar2,1);
    }
    else {
      index[6] = iVar1;
    }
  }
}

void TextInput_CheckLimit(int *index)
{
  int iVar1;
  int iVar2;
  if (((EntityObj *)index)->nStateFlag != 0) {
    iVar1 = index[6];
    iVar2 = iVar1 + -1;
    index[6] = iVar2;
    if (iVar2 < 0) {
      index[6] = iVar1;
    }
    else {
      ((int (*)(int,int,int))(*(void **)(*index + 0xa4)))(index,iVar2,1);
    }
  }
}

void TextInput_ScrollDown(int *index)
{
  int iVar1;
  if (((EntityObj *)index)->nStateFlag != 0) {
    iVar1 = index[7] + 1;
    index[7] = iVar1;
    if (iVar1 < index[5]) {
      ((int (*)(int,int,int,int))(*(void **)(*index + 0xa8)))(index,index[6],iVar1,1);
    }
    else {
      index[7] = 0;
    }
  }
}

void TextInput_ScrollUp(int *index)
{
  int iVar1;
  if (((EntityObj *)index)->nStateFlag != 0) {
    iVar1 = index[7] + -1;
    index[7] = iVar1;
    if (iVar1 < 1) {
      index[7] = index[5];
    }
    else {
      ((int (*)(int,int,int,int))(*(void **)(*index + 0xa8)))(index,index[6],iVar1,1);
    }
  }
}

void TextInput_ProcessEvents(int index)
{
  if (*(int *)(index + 0x48) != 0) {
    *(uint *)(index + 0x20) = *(uint *)(index + 0x20) ^ 1;
  }
}

void TextInput_ScrollHome(int *index)
{
  if (((EntityObj *)index)->nStateFlag != 0) {
    index[7] = 0;
    ((int (*)(int,int,int,int))(*(void **)(*index + 0xa8)))(index,index[6],0,1);
  }
}

void TextInput_RefreshList(int *index)
{
  int iVar1;
  if (((EntityObj *)index)->nStateFlag != 0) {
    iVar1 = index[4];
    index[7] = 0;
    while (iVar1 = iVar1 + -1, -1 < iVar1) {
      index[6] = iVar1;
      ((int (*)(int,int,int,int))(*(void **)(*index + 0xa8)))(index,iVar1,index[7],0);
    }
    ((int (*)(int,int,int))(*(void **)(*index + 0xa4)))(index,index[6],1);
  }
}

void TextInput_StartBlink(int *index, int index_2, int value)
{
  int local_18 [2];
  if (((EntityObj *)index)->nStateFlag != 0) {
    local_18[1] = 0xfffffff4;
    local_18[0] = (int)index * 7 + -0x3e;
    ((int (*)(int,int))(*(void **)(*(int *)((EntityObj *)index)->pSomePtr + 0xbc)))((int *)((EntityObj *)index)->pSomePtr,local_18);
    index[6] = index;
    if (value != 0) {
      ((int (*)(int,int))(*(void **)(*index + 0x60)))(index,0);
    }
  }
}

void TextInput_WriteChar(int *index, int index_2, int value, int arg2)
{
  if (((EntityObj *)index)->nStateFlag != 0) {
    *(char *)(index[10] + index) =
         " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"[value];
    ((int (*)(int,int,int))(*(void **)(*(int *)((EntityObj *)index)->nState + 0xc4)))((int *)((EntityObj *)index)->nState,
               " ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"[value],index);
    index[6] = index;
    index[7] = value;
    if (arg2 != 0) {
      ((int (*)(int,int))(*(void **)(*index + 0x60)))(index,0);
    }
  }
}
void * Entity2_GetVtable(void)
{
  return &Vtable_Entity2;
}

int TextInput_CreateObject(uint id,uint type)
{
  int iVar1;
  int iVar2;
  iVar1 = MemAllocImpl(0x54);
  iVar2 = 0;
  if (iVar1 != 0) {
    iVar2 = GetTextInputVtable();
    ((int (*)(int,int,int))(*(void **)(iVar2 + 8)))(iVar1,id,type);
    iVar2 = iVar1;
  }
  return iVar2;
}

void TextInput_CreateFromList(int *index, int *value, int index_2)
{
  int iVar1;
  uint uVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  iVar5 = 0;
  iVar1 = GetCoordSystemVtable();
  ((int (*)(int))(*(void **)(iVar1 + 8)))(index);
  iVar1 = GetTextInputVtable();
  *index = iVar1;
  piVar4 = value;
  while (iVar1 = *piVar4, piVar4 = piVar4 + 1, iVar1 != 0) {
    iVar5 = iVar5 + 1;
  }
  index[4] = iVar5;
  iVar1 = MemAllocImpl(iVar5 << 2);
  index[6] = iVar1;
  iVar1 = MemAllocImpl(index[4] << 2);
  iVar5 = 0;
  index[7] = iVar1;
  index[5] = 0;
  if (0 < index[4]) {
    do {
      iVar1 = strlen((char *)*value);
      if (index_2 == 1) {
        iVar1 = iVar1 / 2;
      }
      iVar3 = iVar5 * 4;
      *(int *)(iVar3 + index[7]) = iVar1;
      uVar2 = MemAllocImpl(iVar1 + 4);
      *(uint *)(iVar3 + index[6]) = uVar2;
      if (index_2 == 1) {
        Sjis_Decode(*(uint *)(iVar3 + index[6]),*value);
      }
      else {
        strcpy(*(char **)(iVar3 + index[6]),(char *)*value);
      }
      iVar3 = index[5];
      index[5] = iVar3;
      value = value + 1;
      if (iVar3 < iVar1) {
        index[5] = iVar1;
      }
      iVar5 = iVar5 + 1;
    } while (iVar5 < index[4]);
  }
  index[3] = index;
  TextInput_ResetState(index);
  ((int (*)(int))(*(void **)(*index + 0x40)))(index);
}

void TextInput_ResetState(int index)
{
  *(uint *)(index + 0x34) = 0;
  *(uint *)(index + 0x38) = 0;
  *(uint *)(index + 0x50) = 0;
}

void TextInput_DestroyList(int index)
{
  int iVar1;
  int iVar2;
  iVar2 = 0;
  if (0 < *(int *)(index + 0x10)) {
    do {
      iVar1 = iVar2 * 4;
      iVar2 = iVar2 + 1;
      MemFreeImpl(*(uint *)(iVar1 + *(int *)(index + 0x18)));
    } while (iVar2 < *(int *)(index + 0x10));
  }
  MemFreeImpl(*(uint *)(index + 0x1c));
  MemFreeImpl(*(uint *)(index + 0x18));
  iVar2 = GetCoordSystemVtable();
  ((int (*)(int))(*(void **)(iVar2 + 0xc)))(index);
}

void TextInput_AttachItem(int index,uint *id)
{
  int iVar1;
  if (id != (uint *)0x0) {
    iVar1 = GetCoordSystemVtable();
    ((int (*)(int,int))(*(void **)(iVar1 + 0x10)))(index,id);
    if ((*(uint *)*id & 0xf) == 2) {
      *(uint **)(index + 0x34) = id;
    }
    else if ((*(uint *)*id & 0xf) == 5) {
      *(uint **)(index + 0x38) = id;
    }
  }
}

void TextInput_DetachItem(int index,uint *id)
{
  int iVar1;
  if (id != (uint *)0x0) {
    if ((*(uint *)*id & 0xf) == 2) {
      *(uint *)(index + 0x34) = 0;
    }
    else if ((*(uint *)*id & 0xf) == 5) {
      *(uint *)(index + 0x38) = 0;
    }
    iVar1 = GetCoordSystemVtable();
    ((int (*)(int,int))(*(void **)(iVar1 + 0x14)))(index,id);
  }
}

void TextInput_ClearItems(int index)
{
  int iVar1;
  *(uint *)(index + 0x34) = 0;
  *(uint *)(index + 0x38) = 0;
  *(uint *)(index + 0x50) = 0;
  iVar1 = GetCoordSystemVtable();
  ((int (*)(int))(*(void **)(iVar1 + 0x18)))(index);
}

void TextInput_SetItem(int *index, uint *id, uint id_2)
{
  int iVar1;
  code *pcVar2;
  iVar1 = GetCoordSystemVtable();
  ((int (*)(int,int,int))(*(void **)(iVar1 + 0x38)))(index,id,id);
  if ((*(uint *)*id & 0xf) == 2) {
    pcVar2 = *(code **)(*index + 0x5c);
  }
  else {
    if ((*(uint *)*id & 0xf) != 5) {
      return;
    }
    pcVar2 = *(code **)(*index + 0x58);
  }
  ((int (*)(...))(pcVar2))(index,id,id);
}

void TextInput_ClearCoords(int index)
{
  *(uint *)(index + 0x20) = 0;
  *(uint *)(index + 0x24) = 0;
  *(uint *)(index + 0x28) = 0;
}

void TextInput_LoadMc(int *index, int index_2)
{
  uint uVar1;
  int *piVar2;
  int iVar3;
  u8 auStack_38 [32];
  if ((index_2 != 0) && (((EntityObj *)index)->nField_50 == 0)) {
    uVar1 = FileLoadFromDisc(auStack_38,"SELECT","CARD\\",&Rendering_McPath24);
    piVar2 = (int *)Texture_New(uVar1);
    ((int (*)(int))(*(void **)(*piVar2 + 0x78)))(piVar2);
    iVar3 = TextInput_NewFont(piVar2,&Rendering_Ent28,0);
    ((EntityObj *)index)->nField_50 = iVar3;
    ((int (*)(int))(*(void **)(*piVar2 + 4)))(piVar2);
    ((int (*)(int,int,int))(*(void **)(*(int *)((EntityObj *)index)->nField_50 + 0x4c)))((int *)((EntityObj *)index)->nField_50,index,&Rendering_McPathF8);
    uVar1 = FileLoadFromDisc(auStack_38,"FONTICON","CARD\\",&Rendering_McPath24);
    piVar2 = (int *)Texture_New(uVar1);
    ((int (*)(int))(*(void **)(*piVar2 + 0x78)))(piVar2);
    ((int (*)(int,int,int,int,int,int))(*(void **)(*index + 0x8c)))(index,index,piVar2,index[8],index[9],index[10]);
    ((int (*)(int))(*(void **)(*piVar2 + 4)))(piVar2);
  }
}

void TextInput_UnloadMc(int *index)
{
  int iVar1;
  if (((EntityObj *)index)->nField_50 != 0) {
    ((int (*)(void))(*(void **)(*index + 0x90)))();
    iVar1 = ((int (*)(void))(*(void **)(*(int *)((EntityObj *)index)->nField_50 + 4)))();
    ((EntityObj *)index)->nField_50 = iVar1;
  }
}

void TextInput_BeginInput(int *index, uint id, uint type, int index_2)
{
  ((int (*)(void))(*(void **)(*index + 0x10)))();
  ((int (*)(int,int))(*(void **)(*index + 0x10)))(index,type);
  ((EntityObj *)index)->pOtherEntity = index;
  ((EntityObj *)index)->nVabIdx = 0;
}

void TextInput_EndInput(int *index)
{
  ((int (*)(int,int))(*(void **)(*index + 0x14)))(index,((EntityObj *)index)->nField_34);
  ((int (*)(int,int))(*(void **)(*index + 0x14)))(index,((EntityObj *)index)->nField_38);
  ((EntityObj *)index)->pOtherEntity = 0;
}

void TextInput_SetMode(int *index, int index_2)
{
  ((EntityObj *)index)->nField_30 = 0;
  if (1 < index) {
    if (index_2 < 4) {
      ((int (*)(int,int))(*(void **)(*index + 0x14)))(index,((EntityObj *)index)->nField_34);
      ((int (*)(int))(*(void **)(*index + 0x48)))(index);
      ((EntityObj *)index)->nVabIdx = index;
    }
    else if (index_2 == 4) {
      ((int (*)(int,int))(*(void **)(*index + 0x30)))(index,((EntityObj *)index)->nVabIdx);
    }
  }
}

void TextInput_GetText(int *index)
{
  int iVar1;
  if (((((EntityObj *)index)->nVabIdx < 4) && (1 < ((EntityObj *)index)->nVabIdx)) &&
     (iVar1 = ((EntityObj *)index)->nField_30, ((EntityObj *)index)->nField_30 = iVar1 + 1, iVar1 != 0)) {
    ((int (*)(int,int))(*(void **)(*index + 0x54)))(index,4);
  }
}

void TextInput_SetText(int *index,uint id,uint type)
{
  code *pcVar1;
  switch(type) {
  case 4:
    pcVar1 = *(code **)(*index + 0x80);
    break;
  case 5:
    pcVar1 = *(code **)(*index + 0x7c);
    break;
  default:
    goto switchD_80052310_caseD_6;
  case 0x12:
    pcVar1 = *(code **)(*index + 0x84);
    break;
  case 0x13:
    pcVar1 = *(code **)(*index + 0x88);
    break;
  case 0x17:
    ((int (*)(int,int))(*(void **)(*index + 0x60)))(index,0x10);
    ((int (*)(int,int))(*(void **)(*index + 0x54)))(index,3);
    return;
  case 0x19:
    ((int (*)(int,int))(*(void **)(*index + 0x60)))(index,0x10);
    ((int (*)(int,int))(*(void **)(*index + 0x54)))(index,2);
    goto switchD_80052310_caseD_6;
  }
  ((int (*)(...))(pcVar1))(index);
switchD_80052310_caseD_6:
}

void TextInput_SetFlag(int index,uint id)
{
  int *piVar1;
  piVar1 = *(int **)(index + 0x3c);
  if (piVar1 != NULL) {
    ((int (*)(int,int,int,int))(*(void **)(*piVar1 + 0x80)))(piVar1,id,0x60,0x60);
  }
}

void TextInput_ClearFlag(int *index)
{
  int iVar1;
  if ((((EntityObj *)index)->nField_50 != 0) && (iVar1 = index[9] + 1, index[9] + 0x1a < index[5])) {
    index[9] = iVar1;
    ((int (*)(int,int,int,int,int))(*(void **)(*index + 0x94)))(index,index[8],iVar1,index[10],1);
  }
}

void TextInput_MoveUp(int *index)
{
  int iVar1;
  if ((((EntityObj *)index)->nField_50 != 0) && (iVar1 = index[9] + -1, -1 < iVar1)) {
    index[9] = iVar1;
    ((int (*)(int,int,int,int,int))(*(void **)(*index + 0x94)))(index,index[8],iVar1,index[10],1);
  }
}

void TextInput_MoveDown(int *index)
{
  int iVar1;
  int iVar2;
  if ((((EntityObj *)index)->nField_50 != 0) && (-1 < index[10] + -1)) {
    iVar1 = index[8] + -1;
    if (index[10] - index[8] < 1) {
      iVar2 = index[10];
      index[8] = iVar1;
      index[10] = iVar2 + -1;
      ((int (*)(int,int,int,int,int))(*(void **)(*index + 0x94)))(index,iVar1,index[9],iVar2 + -1,1);
    }
    else {
      ((int (*)(int,int,int))(*(void **)(*index + 0x98)))(index,0,1);
    }
  }
}

void TextInput_MovePage(int *index)
{
  int iVar1;
  int iVar2;
  if ((((EntityObj *)index)->nField_50 != 0) && (index[10] + 1 < index[4])) {
    iVar1 = index[8] + 1;
    if (index[10] - (index[8] + -1) < 4) {
      ((int (*)(int,int,int))(*(void **)(*index + 0x98)))(index,1,1);
    }
    else {
      iVar2 = index[10];
      index[8] = iVar1;
      index[10] = iVar2 + 1;
      ((int (*)(int,int,int,int,int))(*(void **)(*index + 0x94)))(index,iVar1,index[9],iVar2 + 1,1);
    }
  }
}
void TextInput_ClipText(int index, uint id, uint type, uint flags,
    uint id_3, uint type_3)
{
  int *piVar1;
  int iVar2;
  uint *puVar3;
  int iVar4;
  u8 auStack_58 [32];
  uint local_38;
  int local_34;
  if (*(int *)(index + 0x50) != 0) {
    local_38 = 0xffffffa4;
    local_34 = -0xf;
    iVar4 = *(int *)(index + 0x10);
    puVar3 = (uint *)(index + 0x40);
    if (4 < iVar4) {
      iVar4 = 4;
    }
    iVar2 = 0;
    if (0 < iVar4) {
      do {
        TextInput_FormatLine(index,auStack_58,iVar2,flags,id_3);
        piVar1 = (int *)TextInput_New(type,0x1a,auStack_58);
        *puVar3 = piVar1;
        ((int (*)(int,int,int))(*(void **)(*piVar1 + 0x4c)))(piVar1,id,&local_38);
        iVar2 = iVar2 + 1;
        ((int (*)(int,int))(*(void **)(*(int *)*puVar3 + 0xb8)))((int *)*puVar3,&Rendering_McPath0c);
        local_34 = local_34 + 10;
        puVar3 = puVar3 + 1;
      } while (iVar2 < iVar4);
    }
    TextInput_DrawToken(index,flags,id_3,type_3,1);
  }
}

void TextInput_RedrawItem(int index)
{
  int iVar1;
  int iVar2;
  if (*(int *)(index + 0x50) != 0) {
    iVar2 = *(int *)(index + 0x10);
    iVar1 = 0;
    if (4 < iVar2) {
      iVar2 = 4;
    }
    if (0 < iVar2) {
      do {
        iVar1 = iVar1 + 1;
        ((int (*)(void))(*(void **)(**(int **)(index + 0x40) + 4)))();
        *(uint *)(index + 0x40) = 0;
        index = index + 4;
      } while (iVar1 < iVar2);
    }
  }
}

void TextInput_SetRect(int *index, uint id, uint type, uint flags, int index_2)
{
  int *piVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  u8 auStack_50 [40];
  if (((EntityObj *)index)->nField_50 != 0) {
    iVar4 = index[4];
    piVar3 = index + 0x10;
    if (4 < iVar4) {
      iVar4 = 4;
    }
    iVar2 = 0;
    if (0 < iVar4) {
      do {
        TextInput_FormatLine(index,auStack_50,iVar2,id,type);
        piVar1 = (int *)*piVar3;
        piVar3 = piVar3 + 1;
        iVar2 = iVar2 + 1;
        ((int (*)(int,int))(*(void **)(*piVar1 + 0xcc)))(piVar1,auStack_50);
      } while (iVar2 < iVar4);
    }
    TextInput_DrawToken(index,id,type,flags,0);
    if (index_2 != 0) {
      ((int (*)(int,int))(*(void **)(*index + 0x60)))(index,0);
    }
  }
}
uchar * TextInput_FormatLine(int index, uchar *arg0, int *value, int arg2, int arg3)
{
  int iVar1;
  uchar *puVar2;
  int iVar3;
  iVar3 = ((int)arg2 + (int)value) * 4;
  iVar1 = strlen((char *)(arg3 + *(int *)(iVar3 + *(int *)(index + 0x18))));
  if (0x1a < iVar1) {
    iVar1 = 0x1a;
  }
  memcpy(arg0,(uchar *)(arg3 + *(int *)(iVar3 + *(int *)(index + 0x18))),iVar1);
  if (iVar1 < 0x1a) {
    puVar2 = arg0 + iVar1;
    do {
      *puVar2 = ' ';
      puVar2 = puVar2 + 1;
    } while ((int)puVar2 < (int)(value + 0x1a));
  }
  value[0x1a] = '\0';
  return value;
}

void TextInput_DrawToken(int index,int value,uint id,int arg3,int arg4)
{
  int *piVar1;
  *(int *)(index + 0x20) = value;
  *(uint *)(index + 0x24) = id;
  *(int *)(index + 0x28) = arg3;
  if (arg4 != 0) {
    piVar1 = *(int **)((arg3 - value) * 4 + index + 0x40);
    ((int (*)(int,int))(*(void **)(*piVar1 + 0xb8)))(piVar1,&EntityFieldSetupData);
  }
}

void TextInput_RefreshToken(int *index,int value,int id)
{
  int iVar1;
  int *piVar2;
  int local_counter;
  if (((EntityObj *)index)->nField_50 != 0) {
    piVar2 = index + (index[10] - index[8]) + 0x10;
    ((int (*)(int,int))(*(void **)(*(int *)*piVar2 + 0xb8)))((int *)*piVar2,&Rendering_McPath0c);
    if (index == 0) {
      piVar2 = piVar2 + -1;
       iVar1 = local_counter + -1;
    }
    else {
      piVar2 = piVar2 + 1;
       iVar1 = local_counter + 1;
    }
    local_counter = iVar1;
    ((int (*)(int,int))(*(void **)(*(int *)*piVar2 + 0xb8)))((int *)*piVar2,&EntityFieldSetupData);
    if (value != 0) {
      ((int (*)(int,int))(*(void **)(*index + 0x60)))(index,0);
    }
  }
}

uint TextInput_GetTokenState(int index)
{
  return *(uint *)(index + 0x28);
}
void * GetTextInputVtable(void)
{
  return &Vtable_Entity3;
}
int TextInput_CreateToken(uint id, uint type, uint flags, uint arg3,
    uint id_3)
{
  int iVar1;
  int iVar2;
  iVar1 = MemAllocImpl(0x88);
  iVar2 = 0;
  if (iVar1 != 0) {
    iVar2 = GetVramTextureVtable();
    ((int (*)(int,int,int,int,int,int))(*(void **)(iVar2 + 8)))(iVar1,id,type,flags,arg3,id_3);
    iVar2 = iVar1;
  }
  return iVar2;
}

void TextInput_DrawLine(int *id, int index_2, int value, int arg2, int arg3, int arg4)
{
  int iVar1;
  iVar1 = GetLocationGridVtable();
  ((int (*)(int,int,int))(*(void **)(iVar1 + 8)))(index_2,0,id);
  iVar1 = GetVramTextureVtable();
  *id = iVar1;
  id[0x19] = 0;
  id[0x1a] = 0;
  id[0x18] = 1;
  id[0x15] = value;
  id[0xe] = arg4;
  id[0x1b] = index_2;
  id[0x1d] = arg2;
  id[0x1c] = arg3;
  id[0x20] = 0;
  id[0x21] = 0;
  ((int (*)(int))(*(void **)(*id + 0x40)))(index_2);
}

void TextInput_FreeToken(uint id)
{
  int iVar1;
  iVar1 = GetLocationGridVtable();
  ((int (*)(int))(*(void **)(iVar1 + 0xc)))(id);
}

void TextInput_DrawTokenList(int *index, uint *id, uint id_2)
{
  int iVar1;
  code *pcVar2;
  uint uVar3;
  iVar1 = GetLocationGridVtable();
  ((int (*)(int,int,int))(*(void **)(iVar1 + 0x38)))(index,id,id);
  uVar3 = *(uint *)*id & 0xfff;
  if (uVar3 == 0x114) {
    pcVar2 = *(code **)(*index + 0xb4);
  }
  else if (uVar3 == 0x164) {
    pcVar2 = *(code **)(*index + 0xb0);
  }
  else {
    if ((*(uint *)*id & 0xffff) != 0x1f34) {
      return;
    }
    pcVar2 = *(code **)(*index + 0x90);
  }
  ((int (*)(...))(pcVar2))(index,id,id);
}

void TextInput_PollToken(void)
{
}

void TextInput_SetTokenPos(int *index, int index_2, int value)
{
  int iVar1;
  ((int (*)(int,int,int))(*(void **)(**(int **)(index + 0xc) + 200)))(*(int **)(index + 0xc),Font_ProcessChar,index);
  ((EntityObj *)index)->pOtherEntity = value;
  iVar1 = GetLocationGridVtable();
  ((int (*)(int,int,int))(*(void **)(iVar1 + 0x44)))(index,index,1);
  ((int (*)(int,int))(*(void **)(*index + 0x10)))(index,value);
}

void Font_ProcessChar(int index,int value,uint id,uint type)
{
  if (value < 0) {
    Font_AdvanceCursor(*(uint *)(index + 0x38),id,type);
  }
  else {
    Font_GetCharData(*(uint *)(index + 0x38));
  }
}

void TextInput_InitRenderer(int *index)
{
  int iVar1;
  ((int (*)(int,int))(*(void **)(*index + 0x14)))(index,((EntityObj *)index)->pOtherEntity);
  iVar1 = GetLocationGridVtable();
  ((int (*)(int))(*(void **)(iVar1 + 0x48)))(index);
}

void TextInput_RenderCharAt(int index,int value,int arg2,uint id)
{
  uint uVar1;
  code *pcVar2;
  int iVar3;
  void *puVar4;
  bool bVar5;
  int *piVar6;
  piVar6 = *(int **)(index + 0x18);
  ((int (*)(int))(*(void **)(*piVar6 + 0x74)))(piVar6);
  *(uint *)(index + 0x60) = 1;
  uVar1 = Snd_SelectSfx(*(uint *)(index + 0x38),0);
  ((int (*)(int,int))(*(void **)(**(int **)(index + 0x54) + 0x5c)))(*(int **)(index + 0x54),uVar1);
  uVar1 = ((int (*)(int,int))(*(void **)(**(int **)(index + 0x3c) + 0x1a0)))(*(int **)(index + 0x3c),0);
  uVar1 = Snd_RandomSelect(*(uint *)(index + 0x38),0,uVar1);
  uVar1 = TextInput_NewRenderer(uVar1);
  *(uint *)(index + 0x58) = uVar1;
  ((int (*)(int,int,int,int,int))(*(void **)(*piVar6 + 0x70)))(piVar6,*(uint *)(index + 0x3c),&Rendering_EntField5c,&Rendering_EntField68,0);
  *(int **)(index + 0x78) = piVar6;
  uVar1 = ((int (*)(int,int))(*(void **)(**(int **)(index + 0x3c) + 0x1a0)))(*(int **)(index + 0x3c),0);
  uVar1 = VramTexture_DrawFunc(*(uint *)(index + 0x14),*(uint *)(index + 0x38),
                       index + 0x6c,uVar1,0);
  *(uint *)(index + 0x50) = uVar1;
  if (arg2 != 0) {
    *(int *)(index + 0x50) = arg2;
  }
  *(uint *)(index + 0x4c) = id;
  if (*(int *)(index + 0x38) == 0) {
    piVar6 = *(int **)(index + 0x14);
    *(uint *)(index + 0x40) = 0x10;
    *(uint *)(index + 0x44) = 2;
    bVar5 = true;
    pcVar2 = *(code **)(*piVar6 + 0x134);
    puVar4 = &Rendering_EntField50;
  }
  else {
    iVar3 = *(int *)(index + 0x38);
    *(uint *)(index + 0x40) = 0x10;
    *(uint *)(index + 0x44) = 3;
    puVar4 = NULL;
    bVar5 = iVar3 == 3 || (iVar3 == 6 || iVar3 == 5);
    piVar6 = *(int **)(index + 0x14);
    pcVar2 = *(code **)(*piVar6 + 0x134);
  }
  ((int (*)(...))(pcVar2))(piVar6,puVar4);
  *(int *)(index + 0x48) = value;
  if (value == 0) {
    *(uint *)(index + 0x48) = 0xa000;
  }
  SetOtherGlobalPtr(bVar5);
  ((int (*)(int,int))(*(void **)(**(int **)(index + 0x3c) + SIZEOF_CHUNK_DATA)))(*(int **)(index + 0x3c),*(uint *)(&Rendering_Ent18 + *(int *)(index + 0x38) * 4)
            );
  *(uint *)(index + 0x20) = 5;
}

void TextInput_DrawCursor(int *index)
{
  ((int (*)(void))(*(void **)(*index + 0x84)))();
  Cinema_FreeEntity();
  VramParticle_Cleanup();
  ((int (*)(void))(*(void **)(*(int *)((EntityObj *)index)->nSoundId + 0x48)))();
}

void TextInput_GetRenderWidth(int index,uint id,int value)
{
  if (value == 2) {
    TextInput_InitFont(index,*(uint *)(index + 0x58));
  }
}

void TextInput_InitFont(int *index,int *value)
{
  int iVar1;
  uint uVar2;
  if (((EntityObj *)index)->nField_60 != 0) {
    if (value[0x20] == 0) {
      if (value[0xf] != 0) {
        iVar1 = ((EntityObj *)index)->nField_50;
        if (*(int *)(iVar1 + 0x14) == 2) {
          uVar2 = *(uint *)(iVar1 + 0xc);
        }
        else {
          uVar2 = *(uint *)(iVar1 + 0x18);
        }
        ((int (*)(int,int))(*(void **)(*value + 0x7c)))(value,uVar2);
        ((int (*)(int))(*(void **)(*value + 4)))(value);
        ((EntityObj *)index)->nField_60 = 0;
        ((int (*)(int))(*(void **)(*index + 0x80)))(index);
      }
    }
    else {
      ((int (*)(int))(*(void **)(*value + 4)))(value);
      ((EntityObj *)index)->nField_60 = 0;
      ((int (*)(int))(*(void **)(*index + 0x80)))(index);
      iVar1 = ((int (*)(void))(*(void **)(*(int *)((EntityObj *)index)->pOtherEntity + 0x108)))();
      ((int (*)(int,int))(*(void **)(*(int *)((EntityObj *)index)->pOtherEntity + 0x104)))((int *)((EntityObj *)index)->pOtherEntity,iVar1 + 0x1e);
    }
    if (((EntityObj *)index)->nField_60 != 0) {
      return;
    }
  }
  if ((*(short *)(index[5] + 0x1b4) == 0) && (((EntityObj *)index)->nObjState == 0)) {
    ((EntityObj *)index)->nField_64 = 1;
    ((int (*)(int))(*(void **)(*index + 0x88)))(index);
  }
}

void TextInput_EndFont(int *index, uint id, int index_2)
{
  code *pcVar1;
  int iVar2;
  iVar2 = *index;
  if (((EntityObj *)index)->nObjState != 0) {
    if (index_2 == 0x16) {
      pcVar1 = *(code **)(iVar2 + 200);
    }
    else if (index_2 < 0x17) {
      if (index_2 != 0xc) {
        return;
      }
      pcVar1 = *(code **)(iVar2 + 0xc0);
    }
    else if (index_2 == 0x21) {
      pcVar1 = *(code **)(iVar2 + 0x74);
    }
    else {
      if (index_2 != 0x2c) {
        return;
      }
      pcVar1 = *(code **)(iVar2 + 0xc4);
    }
    ((int (*)(...))(pcVar1))();
  }
}

void TextInput_CycleColour(int *index)
{
  code *pcVar1;
  if (((EntityObj *)index)->nObjState != 0) {
    index[7] = index[7] + 1;
    if (((EntityObj *)index)->nCondition == 0) {
      pcVar1 = *(code **)(*index + 0x8c);
    }
    else {
      pcVar1 = *(code **)(*index + 0xd0);
    }
    ((int (*)(...))(pcVar1))();
  }
}

void TextInput_GetColour(int *index)
{
  code *pcVar1;
  int iVar2;
  iVar2 = *index;
  if (((EntityObj *)index)->nCondition == 0) {
    pcVar1 = *(code **)(iVar2 + 0xd0);
  }
  else {
    ((int (*)(void))(*(void **)(iVar2 + 0xc4)))();
    pcVar1 = *(code **)(iVar2 + 0xd4);
  }
  ((int (*)(...))(pcVar1))(index);
}

void GetFontVtable(void)
{
}

void Font_Constructor(int *index)
{
  int *piVar1;
  uint uVar2;
  int *piVar3;
  uint *puVar4;
  piVar3 = (int *)index[6];
  puVar4 = (uint *)((EntityObj *)index)->nField_50;
  ((int (*)(int))(*(void **)(*piVar3 + 0x74)))(piVar3);
  piVar1 = (int *)((int (*)(int,int))(*(void **)(**(int **)index[3] + 0x7c)))(*(int **)index[3],0);
  ((int (*)(int,int))(*(void **)(*piVar3 + 0x54)))(piVar3,((*piVar1 / 2) * 5) / 3);
  ((int (*)(int,int,int,int,int))(*(void **)(*piVar3 + 0x70)))(piVar3,((EntityObj *)index)->pOtherEntity,&Rendering_EntField5c,&Rendering_EntField68,0);
  Cinema_SetState(((EntityObj *)index)->nField_38,index[5],((EntityObj *)index)->pOtherEntity,((EntityObj *)index)->nField_34,index[4]);
  piVar1 = (int *)index[5];
  ((int (*)(int,int))(*(void **)(*index + 0x10)))(index,piVar1);
  ((int (*)(int,int,int))(*(void **)(*piVar1 + 0xbc)))(piVar1,puVar4[2],0);
  ((int (*)(int,int,int,int))(*(void **)(*piVar1 + 0xc4)))(piVar1,3,*puVar4,puVar4[1]);
  uVar2 = Font_GetGridData(((EntityObj *)index)->nField_38);
  ((int (*)(int,int))(*(void **)(*piVar1 + 0xe0)))(piVar1,uVar2);
  ((int (*)(int,int))(*(void **)(*(int *)((EntityObj *)index)->pOtherEntity + 0x4c)))((int *)((EntityObj *)index)->pOtherEntity,piVar1);
  ((int (*)(int,int))(*(void **)(*piVar1 + 0xdc)))(piVar1,((EntityObj *)index)->nStateFlag);
  ((int (*)(int,int))(*(void **)(*piVar1 + 0xcc)))(piVar1,&Rendering_Ent0c);
}

void Font_Destructor(int *index)
{
  ((int (*)(void))(*(void **)(*index + 0xd4)))();
  ((int (*)(void))(*(void **)(*(int *)((EntityObj *)index)->pOtherEntity + 0xfc)))();
  ((int (*)(void))(*(void **)(*(int *)((EntityObj *)index)->pOtherEntity + 0x50)))();
  ((int (*)(void))(*(void **)(*(int *)index[6] + 0x74)))();
  ((int (*)(int,int))(*(void **)(*index + 0x14)))(index,index[5]);
}

void Font_CharToGraphic(int *index)
{
  uint uVar1;
  int *piVar2;
  int iVar3;
  uint local_20 [2];
  ((EntityObj *)index)->nObjState = 1;
  ((int (*)(int,int,int))(*(void **)(*(int *)((EntityObj *)index)->pOtherEntity + 0xf8)))((int *)((EntityObj *)index)->pOtherEntity,((EntityObj *)index)->nState,((EntityObj *)index)->pSomePtr);
  ((int (*)(void))(*(void **)(*(int *)index[5] + SIZEOF_CHUNK_DATA)))();
  piVar2 = (int *)index[6];
  iVar3 = ((EntityObj *)index)->nField_50;
  ((int (*)(int,int))(*(void **)(*piVar2 + 0x60)))(piVar2,1);
  ((int (*)(int,int))(*(void **)(*piVar2 + 100)))(piVar2,*(uint *)(iVar3 + 0xc));
  ((int (*)(int,int))(*(void **)(*piVar2 + 0x6c)))(piVar2,*(uint *)(iVar3 + 0x1c));
  if (*(int *)(iVar3 + 0x14) == 1) {
    uVar1 = *(uint *)(iVar3 + 0xc);
  }
  else {
    uVar1 = *(uint *)(iVar3 + 0x18);
  }
  ((int (*)(int,int))(*(void **)(*piVar2 + 0x68)))(piVar2,uVar1);
  ((int (*)(int,int))(*(void **)(*piVar2 + 0xb0)))(piVar2,0);
  ((int (*)(int,int))(*(void **)(*piVar2 + 0xb4)))(piVar2,1);
  piVar2 = (int *)((int (*)(int))(*(void **)(*piVar2 + 0xac)))(piVar2);
  ((int (*)(int,int))(*(void **)(*index + 0x10)))(index,piVar2);
  iVar3 = ((int (*)(int,int,int))(*(void **)(*(int *)((EntityObj *)index)->pOtherEntity + 0xf0)))((int *)((EntityObj *)index)->pOtherEntity,local_20,0xffffffff);
  ((int (*)(int,int,int))(*(void **)(*piVar2 + 0xf0)))(piVar2,iVar3,-(iVar3 != 0) & 3);
  uVar1 = 0xffffffff;
  if (iVar3 != 0) {
    uVar1 = local_20[0];
  }
  ((int (*)(int,int,int,int))(*(void **)(*piVar2 + 0xd4)))(piVar2,index[4],uVar1,0);
}

void Font_GetCharVram(int index)
{
  uint uVar1;
  uVar1 = ((int (*)(int,int,int))(*(void **)(**(int **)(index + 0x14) + 0x10c)))(*(int **)(index + 0x14),0,0);
  RenderUtils_CreateDisplay(uVar1,0,0);
}

void Font_DrawChar(int *index, uint id, int index_2)
{
  if (index[8] == 0) {
    switch(index_2) {
    case 10:
      ((int (*)(void))(*(void **)(*index + 0x94)))();
      break;
    case 0xc:
      ((int (*)(void))(*(void **)(*index + 0x98)))();
      break;
    case 0xd:
      ((int (*)(void))(*(void **)(*index + 0x9c)))();
      break;
    case 0xe:
      ((int (*)(void))(*(void **)(*index + 0xa0)))();
      break;
    case 0xf:
      ((int (*)(void))(*(void **)(*index + 0xa4)))();
      break;
    case 0x10:
      ((int (*)(void))(*(void **)(*index + 0xa8)))();
      break;
    case 0x11:
      ((int (*)(void))(*(void **)(*index + 0xac)))();
    }
  }
  else if (8 < index) {
    *(uint *)(((EntityObj *)index)->pOtherEntity + 0x44) = 0;
  }
}

void Font_DrawString(int *index)
{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  uint local_18 [2];
  index[8] = 4;
  iVar1 = ((int (*)(int,int,int))(*(void **)(*(int *)((EntityObj *)index)->pOtherEntity + 0xf0)))((int *)((EntityObj *)index)->pOtherEntity,local_18,0xffffffff);
  if (iVar1 == 0) {
    uVar2 = index[7] + ((EntityObj *)index)->nField_38 & 3;
    if (uVar2 == 0) {
      ((int (*)(int,int))(*(void **)(*index + 0x30)))(index,4);
      return;
    }
    uVar4 = 10;
    if (uVar2 == 2) {
      local_18[0] = 4;
      uVar3 = local_18[0];
    }
    else {
      uVar3 = local_18[0];
      if (uVar2 < 3) {
        if (uVar2 == 1) {
          local_18[0] = 0;
          uVar3 = local_18[0];
        }
      }
      else if (uVar2 == 3) {
        local_18[0] = 7;
        uVar4 = 5;
        uVar3 = local_18[0];
      }
    }
  }
  else {
    uVar4 = 5;
    uVar3 = 0;
  }
  Font_DrawVram(index,uVar3,0,uVar4,1);
}

void Font_GetStringWidth(int *index)
{
  code *pcVar1;
  uint uVar2;
  if (*(int *)(((EntityObj *)index)->pOtherEntity + 0x164) < 0) {
    pcVar1 = *(code **)(*index + 0x9c);
  }
  else {
    index[8] = 5;
    uVar2 = ((int (*)(void))(*(void **)(*(int *)((EntityObj *)index)->pOtherEntity + 0x200)))();
    Font_DrawVram(index,uVar2,0,10,1);
    pcVar1 = *(code **)(*(int *)((EntityObj *)index)->pOtherEntity + 0xfc);
  }
  ((int (*)(...))(pcVar1))();
}

void Font_GetHeight(int index)
{
  uint uVar1;
  *(uint *)(index + 0x20) = 6;
  uVar1 = ((int (*)(void))(*(void **)(**(int **)(index + 0x3c) + 0x200)))();
  Font_DrawVram(index,uVar1,0,0x1e,1);
  ((int (*)(void))(*(void **)(**(int **)(index + 0x3c) + 0xfc)))();
}

void Font_GetCharWidth(int index)
{
  uint local_10 [2];
  *(uint *)(index + 0x20) = 7;
  ((int (*)(int,int,int))(*(void **)(**(int **)(index + 0x3c) + 0xf0)))(*(int **)(index + 0x3c),local_10,0xffffffff);
  Font_DrawVram(index,local_10[0],0,5,1);
  ((int (*)(void))(*(void **)(**(int **)(index + 0x3c) + 0xfc)))();
}

void Font_GetSpaceWidth(int index)
{
  *(uint *)(index + 0x20) = 8;
  Font_DrawVram(index,0,0,6,1);
  ((int (*)(int,int))(*(void **)(**(int **)(index + 0x3c) + 0xf4)))(*(int **)(index + 0x3c),1);
}

void Font_GetTabWidth(int index)
{
  *(uint *)(index + 0x20) = 10;
  Font_DrawVram(index,0,0,6,1);
  ((int (*)(int,int))(*(void **)(**(int **)(index + 0x3c) + 0x13c)))(*(int **)(index + 0x3c),2);
  ((int (*)(int,int))(*(void **)(**(int **)(index + 0x3c) + 0xf4)))(*(int **)(index + 0x3c),2);
}

void Font_FlushVram(int *index)
{
  ((int (*)(int,int))(*(void **)(*index + 0x30)))(index,0xb);
}

void Font_DrawVram(int *index, uint id, uint type, int index_2, int value)
{
  int *piVar1;
  piVar1 = (int *)((int (*)(void))(*(void **)(*(int *)index[6] + 0xac)))();
  if (index_2 != 0) {
    ((int (*)(int,int))(*(void **)(*piVar1 + 0xd0)))(piVar1,index);
  }
  if (value != 0) {
    ((int (*)(int,int))(*(void **)(*index + 0x10)))(index,piVar1);
  }
  ((int (*)(int,int,int,int))(*(void **)(*piVar1 + 0xd8)))(piVar1,index[4],id,type);
}

void Font_CreateGraphic(int *index, int *value, int index_2)
{
  uint uVar1;
  int iVar2;
  if (index_2 == 5) {
    ((int (*)(int,int))(*(void **)(*index + 0x14)))(index,value);
    ((int (*)(int,int))(*(void **)(*(int *)((EntityObj *)index)->pOtherEntity + 0xf4)))((int *)((EntityObj *)index)->pOtherEntity,0);
    index[8] = 0;
  }
  else if (index_2 == 6) {
    ((int (*)(int,int))(*(void **)(*index + 0x14)))(index,value);
    uVar1 = ((int (*)(int))(*(void **)(*value + 0xe4)))(value);
    ((int (*)(int,int))(*(void **)(*(int *)index[6] + 100)))((int *)index[6],uVar1);
    iVar2 = index[8];
    if (((iVar2 != 5) && (iVar2 != 8)) && (iVar2 == 10)) {
      ((int (*)(int,int))(*(void **)(*(int *)((EntityObj *)index)->pOtherEntity + 0x17c)))((int *)((EntityObj *)index)->pOtherEntity,1);
      ((int (*)(int,int))(*(void **)(*(int *)((EntityObj *)index)->pOtherEntity + 0xf4)))((int *)((EntityObj *)index)->pOtherEntity,0);
      index[8] = 4;
    }
    ((int (*)(int,int))(*(void **)(*index + 0x30)))(index,index[8]);
  }
}

void Font_DeleteGraphic(int *index, uint id, int index_2)
{
  if (index_2 == 7) {
    ((int (*)(void))(*(void **)(*index + 0xb8)))();
  }
}

bool Font_IsLoaded(int index)
{
  int iVar1;
  uint uVar2;
  int iVar3;
  u8 auStack_10 [8];
  iVar1 = ((int (*)(int,int))(*(void **)(**(int **)(index + 0x14) + 0x114)))(*(int **)(index + 0x14),auStack_10);
  uVar2 = ((int (*)(int,int))(*(void **)(**(int **)(index + 0x3c) + 0x1a0)))(*(int **)(index + 0x3c),0);
  iVar3 = Cinema_TriggerSfx(*(uint *)(*(int *)(iVar1 + 4) + 0x34),auStack_10,uVar2);
  *(int *)(iVar1 + 0x14) = iVar3;
  if (iVar3 == 0) {
    ((int (*)(void))(*(void **)(**(int **)(iVar1 + 4) + 0x84)))();
  }
  return iVar3 == 0;
}

void GetLocationEntityVtable(void)
{
}

void LocationEntity_SetCollide(int index)
{
  if ((((EntityObj *)index)->nCondition != 0) && (((EntityObj *)index)->nField_20 == 0)) {
    ((EntityObj *)index)->nSubState = 1;
  }
}

void LocationEntity_WarpTo(int index)
{
  ((EntityObj *)index)->nSubState = 0;
}

void LocationEntity_SetRender(int *index)
{
  if (((EntityObj *)index)->nSubState != 0) {
    ((int (*)(void))(*(void **)(*index + 0xd4)))();
    ((int (*)(int,int))(*(void **)(*index + 0x30)))(index,0xd);
  }
}

void LocationEntity_Delete(int *index)
{
  if (((EntityObj *)index)->nSubState != 0) {
    ((int (*)(void))(*(void **)(*index + 0xd4)))();
    ((int (*)(int,int))(*(void **)(*index + 0x30)))(index,0xc);
  }
}

void LocationEntity_SetAlpha(int index)
{
  int *piVar1;
  int iVar2;
  iVar2 = ((EntityObj *)index)->nCondition;
  if (iVar2 == 0) {
    piVar1 = (int *)TextInput_New(((EntityObj *)index)->nConfigVal,5,"Pause");
    ((EntityObj *)index)->pSubObj = piVar1;
    ((int (*)(int,int,int))(*(void **)(*piVar1 + 0x4c)))(piVar1,((EntityObj *)index)->pData,&Rendering_McPath38);
    ((int (*)(int,int))(*(void **)(*(uint **)((EntityObj *)index)->pSubObj + 0xb8)))(((EntityObj *)index)->pSubObj,&Rendering_McPath40);
    ((EntityObj *)index)->nCondition = 1;
  }
  else {
    ((EntityObj *)index)->nCondition = iVar2 + 1;
    if (iVar2 == 4) {
      ((int (*)(int,int))(*(void **)(*(uint **)((EntityObj *)index)->pWorld + 0xb4)))(((EntityObj *)index)->pWorld,0);
      ((int (*)(void))(*(void **)(((EntityObj *)index)->nDist + 0x4c)))();
      ((int (*)(void))(*(void **)(((EntityObj *)index)->nSoundId + 0x4c)))();
      ((int (*)(void))(*(void **)(((EntityObj *)index)->nField_34 + 0x88)))();
    }
  }
}

void LocationEntity_ApplyAlpha(int index)
{
  if (((EntityObj *)index)->nCondition != 0) {
    ((int (*)(void))(*(void **)(*(uint **)((EntityObj *)index)->pSubObj + 4)))();
  }
  ((int (*)(void))(*(void **)(((EntityObj *)index)->nField_34 + 0x8c)))();
  ((int (*)(void))(*(void **)(((EntityObj *)index)->nSoundId + 0x50)))();
  ((int (*)(void))(*(void **)(((EntityObj *)index)->nDist + 0x50)))();
  ((int (*)(int,int))(*(void **)(*(uint **)((EntityObj *)index)->pWorld + 0xb4)))(((EntityObj *)index)->pWorld,1);
  ((EntityObj *)index)->nCondition = 0;
}
void * GetVramTextureVtable(void)
{
  return &Vtable_Entity4;
}
uint
VramTexture_DrawFunc(int index,uint id,uint type,uint flags,uint arg4
            )
{
  uint uVar1;
  uint *puVar2;
  int iVar3;
  if (ParticleSysEntityPtr == 0) {
    iVar3 = 1;
    puVar2 = &ParticleInitScratch;
    ParticleMode = 0xffffffff;
    ParticleConfigParam = arg4;
    ParticleFrameCounter = 0;
    ParticleSysEntityPtr = index;
    ParticleLocationIndex = id;
    ParticleEffectSelector = flags;
    ParticleWorldPtr = type;
    do {
      *puVar2 = 0;
      iVar3 = iVar3 + -1;
      puVar2 = puVar2 + -1;
    } while (-1 < iVar3);
    uVar1 = VramTexture_SetupDraw();
  }
  else {
    uVar1 = 0;
  }
  return uVar1;
}
void ** VramTexture_SetupDraw(void)
{
  int iVar1;
  iVar1 = *(int *)(&ParticleLocationDefTable + ParticleLocationIndex * 4);
  if (iVar1 == 0) {
    iVar1 = GetVramTexture2Vtable();
  }
  VramTexture_DecodeProps(&PTR_Ptr_SecondEntity,iVar1);
  if ('\x03' < *(char *)(iVar1 + 1)) {
    ParticleEffectVtable = &Unk_ParticleBufC4 + *(char *)(iVar1 + 2) * 3;
  }
  return &PTR_Ptr_SecondEntity;
}

void VramTexture_DecodeProps(int index,char *c)
{
  *(void **)(index + 0xc) = &Unk_ParticleBufC4 + c[3] * 3;
  *(void **)(index + 0x18) = &Unk_ParticleBufC4 + c[2] * 3;
  *(uint *)(index + 0x1c) = *(uint *)(&ParticleDefVtableArray + c[1] * 4);
  *(int *)(index + 0x14) = (int)*c;
}

void VramParticle_Spawn(void)
{
  uint uVar1;
  if (ParticleEffectVtable != 0) {
    ParticleEffectObject = (int *)Particle_New(&ParticleEffectObjectType,ParticleEffectVtable,0);
    ((int (*)(int,int))(*(void **)(*((u32 *)(ParticleEffectObject)) + 100)))(ParticleEffectObject,1);
    ((int (*)(int,int))(*(void **)(*((u32 *)(ParticleEffectObject)) + 0x68)))(ParticleEffectObject,0);
    uVar1 = ((int (*)(void))(*(void **)(**(int **)(ParticleWorldPtr + 0xc) + 0xac)))();
    ((int (*)(int,int,int))(*(void **)(*((u32 *)(ParticleEffectObject)) + 0x4c)))(ParticleEffectObject,uVar1,&ParticleEffectParam2);
  }
}

void VramParticle_Destroy(void)
{
  if (ParticleEffectVtable != 0) {
    ((int (*)(void))(*(void **)(*((u32 *)(ParticleEffectObject)) + 4)))();
    ParticleEffectVtable = 0;
  }
}
void * GetVramTexture2Vtable(void)
{
  uint uVar1;
  int iVar2;
  uint uVar3;
  u8 *puVar4;
  uVar3 = ParticleEffectSelector + ParticleLocationIndex;
  ParticleMode = (int)(char)(&ParticleModeIndexTable)[uVar3 & 0xf];
  iVar2 = (int)(char)(&ParticleModeDenomTable)[ParticleMode];
  ParticleModeSubIndex = (int)uVar3 % iVar2;
  if (iVar2 == 0) {
    trap(0x1c00);
  }
  if ((iVar2 == -1) && (uVar3 == 0x80000000)) {
    trap(0x1800);
  }
  puVar4 = (&PTR_Ptr_DreamParticle)[ParticleMode] + ParticleModeSubIndex * 4;
  uVar1 = ParticleGridTier;
  if (ParticleMode == 0) {
    ParticleChunkPalette = (int)&ParticlePaletteDefault;
    ParticleExtraChunkPtr = (int)&Unk_ParticleBufC4 + (int)(char)puVar4[3] * 3;
    if (puVar4[2] != '\x12') {
      ParticleChunkPalette = (int)&ParticlePaletteAlt;
    }
    uVar1 = 1;
    if ((3 < ParticleModeSubIndex) && (uVar1 = ParticleGridTier, ParticleModeSubIndex < 6)) {
      uVar1 = 2;
    }
  }
  ParticleGridTier = uVar1;
  return puVar4;
}

void VramParticle_SpawnRing(void)
{
  int *piVar1;
  uint uVar2;
  uint *puVar3;
  int iVar4;
  int iVar5;
  uint local_28;
  int local_24;
  uint local_20;
  int local_1c;
  if (ParticleGridTier != 0) {
    local_28 = 0xffffff9c;
    local_24 = -0x3c;
    if (ParticleGridTier == 2) {
      local_24 = -0x1e;
    }
    local_20 = 0x140;
    local_1c = 0x90;
    iVar5 = 1;
    iVar4 = 3;
    ParticleScratch_Alloc = (int *)Particle_New(&local_20,ParticleChunkPalette,0x1fff);
    puVar3 = &ParticleScratch_Alloc;
    do {
      puVar3 = puVar3 + 1;
      iVar5 = iVar5 + 1;
      piVar1 = (int *)Particle_New(&local_20,iVar4 + ParticleChunkPalette,0x1fff);
      *puVar3 = piVar1;
      iVar4 = iVar4 + 3;
      ((int (*)(int,int,int))(*(void **)(*piVar1 + 0x4c)))(piVar1,ParticleScratch_Alloc,&local_28);
      local_24 = local_24 + 3;
      local_1c = local_1c + -7;
    } while (iVar5 < 0x12);
    uVar2 = ((int (*)(void))(*(void **)(**(int **)(ParticleWorldPtr + 0xc) + 0xac)))();
    ((int (*)(int,int,int))(*(void **)(*((u32 *)(ParticleScratch_Alloc)) + 0x4c)))(ParticleScratch_Alloc,uVar2,&local_28);
  }
}

void VramParticle_UpdateAnim(void)
{
  int iVar1;
  int iVar2;
  uint *puVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  u8 auStack_28 [8];
  uint local_20;
  int local_1c;
  if (ParticleGridTier != 0) {
    piVar6 = *(int **)(ParticleWorldPtr + 0xc);
    iVar2 = (piVar6[6] - piVar6[9]) / 600;
    iVar1 = iVar2 * 3;
    if (0 < iVar1) {
      local_20 = 0xffffff9c;
      local_1c = -0x3c;
      iVar5 = 0;
      if (ParticleGridTier == 2) {
        local_1c = -0x1e;
      }
      puVar3 = &ParticleScratch_Alloc;
      iVar4 = 0;
      local_1c = iVar2 * 9 + local_1c;
      do {
        Vector_AddWeighted(auStack_28,iVar4 + ParticleChunkPalette,iVar1);
        ((int (*)(int,int,int))(*(void **)(*(int *)*puVar3 + 0xb8)))((int *)*puVar3,1,auStack_28);
        iVar5 = iVar5 + 1;
        iVar4 = iVar4 + 3;
        ((int (*)(int,int))(*(void **)(*(int *)*puVar3 + 0xbc)))((int *)*puVar3,&local_20);
        local_1c = local_1c + 3;
        puVar3 = puVar3 + 1;
      } while (iVar5 < 0x12);
      Vector_AddWeighted(auStack_28,ParticleExtraChunkPtr,iVar1);
      ((int (*)(int,int))(*(void **)(*piVar6 + 100)))(piVar6,auStack_28);
    }
  }
}

void Vector_AddWeighted(char *c, char c_2, char c_2_2)
{
  *c = *c - c_2;
  c[1] = c[1] - c_2;
  c[2] = c[2] + c_2;
}

void VramParticle_FreeArray(void)
{
  if (ParticleGridTier != 0) {
    ArrayInitByVtable(&ParticleScratch_Alloc,0x12);
    ParticleGridTier = 0;
  }
}

void VramParticle_SpawnByType(uint id)
{
  uint uVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  if (-1 < ParticleMode) {
    ParticleSysInit(ParticleMode,*(uint *)(ParticleWorldPtr + 4),*(uint *)(ParticleWorldPtr + 8),
                 *(uint *)(ParticleWorldPtr + 0xc));
    uVar1 = rand();
    iVar3 = (int)(char)(&Rendering_Part24)[uVar1 & 3];
    iVar4 = 0;
    if (ParticleMode == 2) {
      iVar4 = 0x10 - iVar3;
    }
    Rendering_PartList88 = iVar3 + iVar4;
    uVar2 = VramParticle_CreateType0(&ParticleScratch_List,iVar3,id);
    uVar2 = VramParticle_CreateType1(uVar2,iVar4,id);
    if (ParticleMode == 0) {
      VramParticle_CreateType2(uVar2,id);
    }
    else {
      if (ParticleMode != 2) {
        return;
      }
      VramParticle_CreateType3(uVar2,id);
    }
    Rendering_PartList88 = Rendering_PartList88 + 1;
  }
}

void VramParticle_DrawAll(uint id)
{
  int *piVar1;
  int iVar2;
  uint *puVar3;
  if ((-1 < ParticleMode) && (iVar2 = 0, 0 < Rendering_PartList88)) {
    puVar3 = &ParticleScratch_List;
    do {
      piVar1 = (int *)*puVar3;
      puVar3 = puVar3 + 1;
      ((int (*)(int,int))(*(void **)(*piVar1 + SIZEOF_CHUNK_DATA)))(piVar1,id);
      iVar2 = iVar2 + 1;
    } while (iVar2 < Rendering_PartList88);
  }
}

void VramParticle_FreeInstances(void)
{
  if (-1 < ParticleMode) {
    ArrayInitByVtable(&ParticleScratch_List,Rendering_PartList88);
  }
}

void VramParticle_Cleanup(void)
{
  uint uVar1;
  uint *puVar2;
  int iVar3;
  VramParticle_Destroy();
  iVar3 = 0;
  VramParticle_FreeArray();
  VramParticle_FreeInstances();
  puVar2 = &ParticleSecondarySlots;
  do {
    iVar3 = iVar3 + 1;
    uVar1 = RenderUtils_ClipSub(*puVar2);
    *puVar2 = uVar1;
    puVar2 = puVar2 + 1;
  } while (iVar3 < 2);
  if (ParticleSysEntityPtr != 0) {
    ParticleSysEntityPtr = 0;
  }
}

uint VramParticle_CreateType0(uint *id, int index, uint id_2)
{
  int iVar1;
  uint uVar2;
  uint uVar3;
  uint uVar4;
  code *pcVar5;
  iVar1 = rand();
  ParticleScratch_ColIndex = iVar1 % 7;
  uVar2 = rand();
  ParticleScratch_EntPtr = &Unk_EntityBufC8 + (uVar2 % 5) * 0xc;
  uVar2 = rand();
  uVar4 = 0;
  if (uVar2 % 5 != 0) {
    uVar4 = *(uint *)(&ParticleRandomColourArray + (uVar2 % 5) * 4);
  }
  pcVar5 = RenderUtils_ApplyPpu;
  iVar1 = 0;
  if (ParticleEffectSelector != (ParticleEffectSelector / 7) * 7) {
    pcVar5 = RenderUtils_CallDraw;
  }
  if (0 < index) {
    do {
      ((int (*)(...))(pcVar5))(id,uVar4);
      iVar1 = iVar1 + 1;
      uVar3 = ParticleAllocate(0,&ParticleScratch_XPos,ParticleSysEntityPtr,id);
      *id = uVar3;
      id_2 = id + 1;
    } while (iVar1 < index);
  }
  return id;
}

uint VramParticle_CreateType1(uint *id, int index, uint id_2)
{
  uint uVar1;
  int iVar2;
  ParticleScratch_EntPtr = &LAB_80087204;
  iVar2 = 0;
  if (0 < index) {
    do {
      RenderUtils_CallDraw(id,0xffffc800);
      iVar2 = iVar2 + 1;
      uVar1 = ParticleAllocate(1,&ParticleScratch_XPos,ParticleSysEntityPtr,id);
      *id = uVar1;
      id_2 = id + 1;
    } while (iVar2 < index);
  }
  return id;
}

uint VramParticle_CreateType2(uint *id, uint id_2)
{
  uint uVar1;
  uint uVar2;
  RenderUtils_CallDraw(id,0xffffc800);
  if ((ParticleGridTier == 0) || (ParticleChunkPalette != &ParticlePaletteDefault)) {
    if (0 < ParticleScratch_Move) {
      ParticleScratch_Move = -ParticleScratch_Move;
    }
    if (ParticleScratch_Move < -0x7800) {
      ParticleScratch_Move = -0x7800;
    }
    uVar1 = rand();
    ParticleScratch_BufPtr = &Unk_ParticleBuf1C + (uVar1 % 3) * 3;
  }
  else {
    ParticleScratch_XPos = 0xffff5000;
    ParticleScratch_YPos = 0xffffe000;
    ParticleScratch_Move = 0;
    ParticleScratch_BufPtr = &Rendering_PartByte1f;
  }
  ParticleScratch_ColPtr = &ParticleColourTable;
  uVar2 = ParticleAllocate(3,&ParticleScratch_XPos,ParticleSysEntityPtr,id);
  *id = uVar2;
  return id + 1;
}

uint VramParticle_CreateType3(uint *id, uint id_2)
{
  uint uVar1;
  int iVar2;
  uint uVar3;
  uVar1 = rand();
  ParticleScratch_BufPtr = &Unk_ParticleBuf28 + (uVar1 % 3) * 3;
  ParticleScratch_Flag = 0;
  RenderUtils_CallDraw(id,0xffffc800);
  ParticleScratch_ColPtr = &ParticleColourTable;
  iVar2 = rand();
  ParticleScratch_ColIndex = iVar2 % 6;
  uVar3 = ParticleAllocate(2,&ParticleScratch_XPos,ParticleSysEntityPtr,id);
  *id = uVar3;
  return id + 1;
}
