/* Entity3_CdMode.c -- split from Entity3_vtable.c */
#include "Entity3_Shared.h"
/* Extern declarations for functions used as values */
extern void func_8002C468(void);
extern int NopSub_27f18(int a0, int a1, int a2);
/* ==== 171E0.c ==== */
int *NopSub_269e0(void)
{
    return &D_8006D3C8;
}
int CdModeDispatch(int *obj)
{
    int vtab;
    *(short *)&obj[8] = 0;
    vtab = *obj;
    (*(code *)(vtab + 0x0C))();
    vtab = GetCoordSystemVtable();
    (*(code *)(vtab + 0x0C))(obj);
    MemFreeImpl(obj);
    return 0;
}
int CdModeSubA(int *obj)
{
    int vtab;
    int tmp;
    vtab = GetCoordSystemVtable();
    tmp = (*(code *)(vtab + 0x08))(obj);
    obj[0] = tmp;
    CdModeUnknown();
    obj[3] = 0;
    obj[4] = 0;
    obj[5] = 0;
    *(short *)&obj[8] = 0;
    *(short *)&obj[9] = 0;
    obj[9] = 0;
    *(short *)&obj[10] = 0;
    *(short *)&obj[10] = 0;
    return 0;
}
void CdModeSubB(int *obj)
{
    int vtab;
    vtab = *obj;
    (*(code *)(vtab + 0x48))();
    vtab = *obj;
    (*(code *)(vtab + 0x5C))(obj);
}
int CdModeSubC(int *obj)
{
    int vtab;
    int *child;
    int type;
    int oldArg;
    if (obj[4] != 0)
        return 0;
    oldArg = obj[3];
    obj[3] = 0;
    vtab = *obj;
    (*(code *)(vtab + 0x44))(obj, 0, 1, 0);
    vtab = *obj;
    type = (*(code *)(vtab + 0x4C))(obj, 0, 2);
    child = MemAlloc(type);
    if (child != 0)
    {
        vtab = *obj;
        (*(code *)(vtab + 0x4C))(obj, 0, 0);
        vtab = *obj;
        (*(code *)(vtab + 0x54))(obj, child, type);
        vtab = *obj;
        (*(code *)(vtab + 0x48))(obj);
        obj[4] = (int)child;
        obj[5] = type;
        obj[3] = oldArg;
    }
    else
    {
        MemFreeImpl(0);
        vtab = *obj;
        (*(code *)(vtab + 0x48))(obj);
    }
    return 0;
}
void CdModeInitialize(int *obj)
{
    if (obj[4] != 0 && obj[5] != 0 && *(short *)&obj[8] == 0)
    {
        MemFreeImpl((void *)obj[4]);
        obj[4] = 0;
    }
}
int CdModeReturnZero(void)
{
    return 0;
}
int CdModeSetFlag9(int *obj)
{
    obj[9] |= 1;
    return obj[9];
}
int *CdModeUnknown(void)
{
    return (int *)&D_8006D430;
}
void CdModePoll(void)
{
    if (D_8008A84C != 0x23)
        NopSub_27e68();
    else
        func_8002C438();
}
void CdModeStoreTriplet(int *out, int a, int b, int c)
{
    out[0] = a;
    out[1] = b;
    out[2] = c;
}
void CdModeRunTask(int mode)
{
    int i;
    int **pp;
    int *base;
    D_8008A84C = mode;
    if (mode == 0x13)
        i = NopSub_27e68();
    else
        i = func_8002C438();
    base = CdModeUnknown();
    pp = (int **)&D_8006D4AC;
    for (;;)
    {
        CdModeSubD(i, base);
        if (!*pp)
            break;
        (*(code)(*pp))(i);
        pp++;
    }
}
void CdModeSubD(int *dst, int *src)
{
    dst[16] = src[16];
    dst[17] = src[17];
    dst[18] = src[18];
    dst[19] = src[19];
    dst[20] = src[20];
    dst[21] = src[21];
    dst[22] = src[22];
    dst[26] = src[26];
    dst[27] = src[27];
    dst[28] = src[28];
    dst[29] = src[29];
}
void CdModeStartReading(void)
{
    if (D_8008A84C == 0x13)
        NopSub_280d0();
}
void CdModeStopReading(void)
{
    if (D_8008A84C == 0x13)
        NopSub_280e0();
}
int CdModeCheckIsMode13(void)
{
    if (D_8008A84C != 0x13)
        return 0;
    return func_80027EC8();
}
int CdModeCheckIsNotMode13(void)
{
    if (D_8008A84C != 0x13)
        return 1;
    return func_80027ED4();
}
int CdModeCheckMode13A(void)
{
    if (D_8008A84C != 0x13)
        return 0;
    return func_80027EE0();
}
int CdModeCheckMode13B(void)
{
    if (D_8008A84C != 0x13)
        return 0;
    return func_80027EEC();
}
void CdModeSubE(int a0, int a1, int a2)
{
    int (*fn)(int, int, int);
    if (D_8008A84C != 0x13)
        fn = (int (*)(int, int, int))func_8002C468;
    else
        fn = NopSub_27f18;
    while (!fn(a0, a1, a2))
        ;
}
void CdModeBranchMode13A(void)
{
    if (D_8008A84C != 0x13)
        func_8002C448();
    else
        func_80027EF8();
}
void CdModeBranchMode13B(void)
{
    if (D_8008A84C != 0x13)
        func_8002C478();
    else
        func_80028B6C();
}
int CdModeSubF(int *a0, int a1)
{
    int i;
    if (D_8008A84C != 0x13)
        return 1;
    D_8008A850 = 1;
    NopSub_27fd8(a0);
    i = NopSub_27ff0();
    NopSub_27fe4(i + a1);
    NopSub_27ffc(a0 + i * 7 * 4, a1);
    return 0;
}
void CdModeSubG(int a0)
{
    D_8008A854 = a0;
}
int CdGetFilePathPrefix(void)
{
    return D_8008A854;
}
char *FileLoadFromDisc(char *dst, char *s1, char *s2, char *s3)
{
    if (s1)
        *dst = 0;
    StrCat(dst, s1);
    StrCat(dst, s2);
    return dst;
}
char *StrCat(char *dst, char *src)
{
    char *ret;
    int lenDst;
    int lenSrc;
    char *p;
    if (!dst || !src)
        return 0;
    lenDst = strlen_imp(dst);
    lenSrc = strlen_imp(src);
    p = dst + lenDst;
    if (dst + lenDst == src + lenSrc)
        return dst;
    ret = dst;
    while (*p)
        p++;
    p--;
    while (*src)
    {
        *p = *src;
        p++;
        src++;
    }
    return ret;
}
int EntityAllocSmall(int type)
{
    int obj;
    int vtab;
    obj = MemAllocImpl(0x2c);
    if (obj != 0) {
        vtab = (int)NopSub_269e0();
        (*(code *)(vtab + 8))(obj, type);
    }
    return obj;
}
void CdModeInitDream(int *obj, int *src)
{
    int iVar1;
    int uVar2;
    int local_20;
    char *local_1c;
    iVar1 = (int)func_8003b20c();
    (*(code *)(iVar1 + 8))(obj, *src);
    iVar1 = (int)NopSub_269e0();
    *obj = iVar1;
    obj[8] = (int)src;
    uVar2 = (int)func_80048cf0();
    CdModeSubG(uVar2);
    local_20 = 0;
    local_1c = "ETC\\DREAME5.TMD";
    uVar2 = (int)Tmd_New(&local_20);
    iVar1 = New_DreamSys(uVar2, 0, 0);
    obj[10] = iVar1;
    obj[9] = 0;
    (*(code *)(*(int *)obj[10] + 0x228))(obj[10], src[5]);
    (*(code *)(*obj + 0x40))(obj);
}
void NopSub_260a4(void)
{
    func_80048cfc(*(int *)REG_GTE_RAND % 365, 0);
}
void NopSub_26108(int obj, int arg2, int arg3)
{
    int iVar1;
    if (*(int *)(obj + 0x18) == 0) {
        iVar1 = (int)func_8003b20c();
        (*(code *)(iVar1 + 0x44))(obj, arg2, arg3, 0);
    }
}
void NopSub_26170(int obj)
{
    (void)obj;
}
void NopSub_26254(int obj, char *path)
{
    (void)obj;
    (void)path;
}
void NopSub_280d0(void) { }
void NopSub_280e0(void) { }
void NopSub_27fd8(int *a0) { (void)a0; }
void NopSub_27fe4(int a0) { (void)a0; }
int NopSub_27ff0(void) { return 0; }
void NopSub_27ffc(int *a0, int a1) { (void)a0; (void)a1; }

