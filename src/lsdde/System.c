/**
 * System.c - PS1 SDK GS (Graphics System) Library Wrappers
 *
 * These functions interface with the PS1 graphics hardware.
 * They're thin wrappers around the GS library for projection,
 * work base, and object creation.
 *
 * Original game: LSD: Dream Emulator (SLPS_015.56, 1998)
 */
#include "common.h"
#include "dat_globals.h"
#include "libgte.h"
#include "libgpu.h"
#include "libgs.h"
#include "libcd.h"
u32 System_HeapBase = 0;
/* Forward declarations for vtable getters */
static void* GetGsVtable1(void);
static void* GetGsVtable2(void);
static void* GetGsVtable3(void);
static void* GetGsVtable4(void);
/* Forward declarations to fix conflicting types */
int DreamStubReturnZero(char a, char *b, int c, char d);
bool DreamStateCheckEq(int index);
/**
 * GetGSVtable1 - Returns pointer to GS object vtable #1
 * Used for scene graph node creation
 */
void* GetGsVtable1(void) {
    return (void*)0x8006e730;
}
/**
 * GetGSVtable2 - Returns pointer to GS object vtable #2
 * Used for light/camera object creation
 */
void* GetGsVtable2(void) {
    return (void*)0x8006e854;
}
/**
 * GetGSVtable3 - Returns pointer to GS object vtable #3
 * Used for auxiliary object creation
 */
void* GetGsVtable3(void) {
    return (void*)0x8006e878;
}
/**
 * CreateGSObject - Allocate and initialize a GS library object
 *
 * Allocates memory for a GS object (0xBC bytes), retrieves its vtable,
 * and calls the constructor through the vtable's function pointer.
 *
 * @return Pointer to new object, or NULL on allocation failure
 */
void* CreateGSObject(void) {
    void* obj;
    void* vtable;
    obj = MemAlloc(SIZEOF_GSOBJ);
    if (obj == NULL) {
        return NULL;
    }
    vtable = GetGsVtable4();
    /* Call constructor: vtable[2] is the init function */
    ((void (*)(void*))((void**)vtable)[2])(obj);
    return obj;
}
/**
 * GetGSVtable4 - Returns pointer to GS object vtable #4
 * Used for CreateGSObject
 */
void* GetGsVtable4(void) {
    return (void*)0x8006e8e4;
}
/**
 * GetLastListNode - Traverse linked list to find the last element
 *
 * Walks the list via the 'next' pointer at offset 0xC.
 * Returns the last node in the chain.
 *
 * @param node - Starting node of the list
 * @return Pointer to the last node in the list
 */
void* GetLastListNode(void* node) {
    void* current = node;
    void* next;
    next = *(void**)((char*)current + 0xC);
    while (next != NULL) {
        current = *(void**)((char*)current + 0xC);
        next = *(void**)((char*)current + 0xC);
    }
    return current;
}
/**
 * GsSetProjection - Set projection screen distance for 3D rendering
 *
 * Controls the perspective projection distance. Larger values = less
 * perspective distortion (more orthographic-like).
 *
 * @param distance - Screen distance for projection (typically 100-1000)
 */
void GsSetProjection(long distance) {
    SetGeomScreen(distance);
}
/**
 * GsSetWorkBase - Set the work base pointer for GS operations
 *
 * The work base is used by GS library for temporary calculations
 * during rendering operations.
 *
 * @param workBase - Pointer to work area memory
 */
void GsSetWorkBase(PACKET* workBase) {
    System_GsWorkBase = (u32)workBase;
}
/**
 * GsSetClipMode - Set the clipping mode for GS rendering
 *
 * Controls near/far clipping behavior for 3D objects.
 *
 * @param mode - Clipping mode flags
 */
void GsSetClipMode(void* mode) {
    System_GsClipMode = mode;
}
/**
 * CreateGSObjectEx - Allocate and initialize a GS object with parameters
 *
 * Extended version of CreateGSObject that passes initialization parameters.
 * Allocates 0xA0 bytes and calls constructor with 3 arguments.
 *
 * @param initParam1 - First initialization parameter
 * @param initParam2 - Second initialization parameter
 * @param initParam3 - Third initialization parameter
 * @return Pointer to new object, or NULL on allocation failure
 */
void* CreateGSObjectEx(void* initParam1, void* initParam2, void* initParam3) {
    void* obj;
    void* vtable;
    obj = MemAlloc(SIZEOF_OBJGENERIC);
    if (obj == NULL) {
        return NULL;
    }
    vtable = GetGsVtable4();
    /* Call constructor with 3 params: vtable[2] */
    ((void (*)(void*, void*, void*, void*))((void**)vtable)[2])(
        obj, initParam1, initParam2, initParam3
    );
    return obj;
}
/* ==== Begin main.c ==== */
#include "common.h"
#include "dat_globals.h"
extern u8 _gp[];
extern u8 main_SBSS_START[];
extern u8 main_BSS_END[];
extern u8 _gamedata_vaddr[];
extern u8 D_80066828[];
#include "lba_table.h"
extern void InitHeap(void *base, u32 size);
__asm__(
    ".globl InitHeap\n"
    ".type InitHeap, @function\n"
    "InitHeap:\n"
    ".set noreorder\n"
    ".set noat\n"
    "li $v0, 0x0B\n"
    "syscall\n"
    "nop\n"
    ".set at\n"
    ".set reorder\n"
);
extern void InitSpu(int a);
/* Minimal SPU init: set SPUCNT to enable SPU */
__asm__(
    ".globl InitSpu\n"
    ".type InitSpu, @function\n"
    "InitSpu:\n"
    ".set noreorder\n"
    "lui $v0, 0x1F80\n"
    "li $v1, 0x8000\n"
    "sh $v1, 0x1C00($v0)\n"
    "jr $ra\n"
    "nop\n"
    ".set reorder\n"
);
extern int CdInit(void);
extern void startIntrVSync(void);
extern void CdLoadStage2(void *dest, u32 lba, u32 count);
extern void *BMemPMgrInit(u32 size, int a1);
extern void System_HeapInit(void);
extern int EntityAllocSmall(void *a0);
extern void *MemAllocImpl(int size);
/* Real GPU/BasicClass vtable D_8006C070 (GsGpuDisplayListVtable), matching the
   original NopSub_20c4c which returns &DAT_8006c070.
   vtable[2] = func_80020730 (real ctor), vtable[0x44] = func_800207dc
   (GS init: GsInitGraph/GsSetVideoMode). func_8003B044 dispatches
   gpu->vtable[0x44] during base init, so the old 3-entry stub caused an
   out-of-bounds call whenever that path ran. */
void *NopSub_20c4c(void) { return (void *)&GsGpuDisplayListVtable; }
void *func_800206e0(void)
{
    void *obj = MemAllocImpl(0x34);
    if (obj) {
        void *vtab = NopSub_20c4c();
        void *ctor = *(void **)((int)vtab + 8);
        ((void (*)(void *))ctor)(obj);
    }
    return obj;
}
extern int PadManagerConstructor(int a0, int a1);
extern void ReinstallHandler(void);
extern int vsync_rel_handler(int channel, void *callback);
/* PS-EXE resident CD callback table (4 entries: data_ready, data_sync, ready, complete) */
extern void *CdCallbackTable[4];
/* Override PsyQ VSyncCallback — register callback in our PS-EXE resident table */
int VSyncCallback(void (*f)(void))
{
    extern void *VSyncCallbackTable[16];
    int old = (int)VSyncCallbackTable[0];
    VSyncCallbackTable[0] = (void *)f;
    return old;
}
/* Override PsyQ InterruptCallback — store handlers in our PS-EXE resident IntrTab */
void *InterruptCallback(int mask, void *handler)
{
    extern void *IntrCallbackTable[16];
    void *old = IntrCallbackTable[mask];
    if (handler != NULL)
        IntrCallbackTable[mask] = handler;
    return old;
}
/* Callback tables - PS-EXE resident (allocated in linker script) */
extern void *VSyncCallbackTable[16];
extern void *IntrCallbackTable[16];
extern void *CdCallbackTable[4];
/* Override PsyQ HookEntryInt — called from startIntr() after I_MASK = 0.
 * Original calls address 0xB0 (now inside our exception handler code) and
 * crashes. We set I_MASK directly to enable VSync(0), CDROM(2), DMA(3),
 * Timer(4), and Controller(7). */
void HookEntryInt(void *unused)
{
    volatile unsigned short *imask = (volatile unsigned short *)0x1F801074;
    *imask = 0x00DD;   /* bits 0(VSync) 2(CDROM) 3(DMA) 4(Timer0) 6(Timer2/SPU) 7(Controller) */
}
__asm__(
    ".globl _start\n"
    ".type _start, @function\n"
    "_start:\n"
    ".set noreorder\n"
    "lui $t0, 0x800B\n"
    "addiu $t0, $t0, 0x9000\n"
    "lui $t1, 0xDEAD\n"
    "sw $t1, 0($t0)\n"
    "lui $sp, 0x801F\n"
    "ori $sp, $sp, 0xFF00\n"
    /* Clear .sbss (small BSS) */
    "lui $a0, %hi(main_SBSS_START)\n"
    "addiu $a0, $a0, %lo(main_SBSS_START)\n"
    "lui $a1, %hi(main_SBSS_END)\n"
    "addiu $a1, $a1, %lo(main_SBSS_END)\n"
    "2:\n"
    "sw $zero, 0($a0)\n"
    "addiu $a0, $a0, 4\n"
    "sltu $at, $a0, $a1\n"
    "bne $at, $zero, 2b\n"
    "nop\n"
    /* Clear .bss */
    "lui $a0, %hi(main_BSS_START)\n"
    "addiu $a0, $a0, %lo(main_BSS_START)\n"
    "lui $a1, %hi(main_BSS_END)\n"
    "addiu $a1, $a1, %lo(main_BSS_END)\n"
    "3:\n"
    "sw $zero, 0($a0)\n"
    "addiu $a0, $a0, 4\n"
    "sltu $at, $a0, $a1\n"
    "bne $at, $zero, 3b\n"
    "nop\n"
    "lui $gp, %hi(_gp)\n"
    "addiu $gp, $gp, %lo(_gp)\n"
    /* InitHeap disabled — our BMemPMgrInit sets up the heap at 0x800C82FC instead,
       and InitHeap's region (main_BSS_END+4, 0x100000) overlaps with it. */
    /* "lui $a0, %hi(main_BSS_END)\n" */
    /* "addiu $a0, $a0, %lo(main_BSS_END)\n" */
    /* "addiu $a0, $a0, 4\n" */
    /* "lui $a1, 0x0010\n" */
    /* "jal InitHeap\n" */
    /* "nop\n" */
    "lui $t0, 0x800B\n"
    "li $t1, 1\n"
    "sh $t1, 0x2B60($t0)\n"
    "sh $t1, 0x2C9C($t0)\n"
    "addiu $a0, $t0, 0x2CA0\n"
    "li $a1, 16\n"
    "4:\n"
    "sw $zero, 0($a0)\n"
    "addiu $a0, $a0, 4\n"
    "addiu $a1, $a1, -1\n"
    "bnez $a1, 4b\n"
    "nop\n"
    "jal startIntrVSync\n"
    "nop\n"
    "jal ReinstallHandler\n"   /* install exception_handler immediately after startIntrVSync */
    "nop\n"
    "lui $t0, %hi(vsync_rel_handler)\n"
    "addiu $t0, $t0, %lo(vsync_rel_handler)\n"
    "lui $t1, 0x800B\n"
    "    sw $t0, 0x2CA0($t1)\n"
    "    sw $t0, 0x2CE0($t1)\n"
    "jal  main\n"
    "nop\n"
    "1:\n"
    "b 1b\n"
    "nop\n"
    ".set reorder\n"
);
void  __main (void)
{
}
/* Debug markers: dump RAM once, read 0x800BAFF0-0x800BAFF8 to see last N steps */
#define D0  (*(volatile u32 *)0x800BAFF0)
#define D1  (*(volatile u32 *)0x800BAFF4)
#define D2  (*(volatile u32 *)0x800BAFF8)
#define D3  (*(volatile u32 *)0x800BAFEC)
#define D4  (*(volatile u32 *)0x800BAF00)
#define D5  (*(volatile u32 *)0x800BAEFC)
#define D6  (*(volatile u32 *)0x800BAEF8)
#define D7  (*(volatile u32 *)0x800BAEF4)
#define D8  (*(volatile u32 *)0x800BAF04)
#define D9  (*(volatile u32 *)0x800BAF08)

void  main (void)
{
     __main ();
    *(volatile u32 *)0x800B2C00 = REG_IO_PORT;
    *(volatile u32 *)0x800B2C04 = 0x1F801801;
    *(volatile u32 *)0x800B2C08 = 0x1F801804;
    *(volatile u32 *)0x800B2C10 = REG_IO_PORT_03;
    *(volatile u32 *)0x800B3D2C = 0x800B2C9C;
    D0 = 1; D1 = 0;
    SetMem(2);
    /* _bu_init() is syscall 0x70 — hangs in DuckStation BIOS. Use no-op stub for now. */
    /* But we MUST re-init heap after _bu_init would have run, since _start's InitHeap gets clobbered */
    System_HeapBase = BMemPMgrInit(0x166C00, 0);
    if (System_HeapBase == 0) System_HeapBase = (u32)0x800C82FC;  /* force heap base if BMemPMgrInit returns 0 */
    *(volatile u32 *)0x800B0188 = System_HeapBase;  /* force write to global */
    D3 = *(volatile u32 *)0x800C830C;  /* D3 = heap firstBlock[0] after BMemPMgrInit */
    D4 = *(volatile u32 *)0x800C8304;  /* D4 = heap free list head after BMemPMgrInit */
D0 = 2; D1 = 0;
    /* Marker: write a sentinel value to Other_MemHeapPtr BEFORE System_HeapInit.
       If crash dump shows 0xCAFEBABE here, System_HeapInit didn't run (or wrote wrong addr).
       If it shows 0x800C82FC, System_HeapInit worked correctly. */
    D8 = 0xCAFEBABE;     /* D8 = sentinel written before System_HeapInit */
    System_HeapInit();
    D9 = Other_MemHeapPtr;  /* D9 = snapshot of Other_MemHeapPtr after init */
    D5 = *(volatile u32 *)0x800C830C;  /* D5 = heap firstBlock[0] after System_HeapInit */
    D6 = *(volatile u32 *)0x800C8304;  /* D6 = heap free list head after System_HeapInit */
    D0 = 5; D1 = 0;
    int gs =  EntityAllocSmall(D_80066828);
    GameStatePtr = gs;
    D7 = *(volatile u32 *)0x800C830C;  /* D7 = heap firstBlock[0] after EntityAllocSmall */
    D0 = 6; D1 = 0;
    int uVar1 = func_800206e0 ();
    D0 = 7; D1 = 0;
    int pad =  PadManagerConstructor(0, 0);
    D0 = 8; D1 = 0;
    int *game = (int *)GameStatePtr;
    /* vtable[0x44] = func_8003B044 only runs its body when game[0x18]==0
       (it creates the obj[0x1c] container and then sets game[0x18]=1).
       The original relied on the BIOS-zeroed pool; the port pool is heap
       garbage, so force the gate explicitly. */
    game[6] = 0;
    (**(code **)(*game + 0x44))(game, uVar1, pad);
    D0 = 9; D1 = 0;
    game[6] = 1;
    CdInit();
    CdLoadStage2(_gamedata_vaddr, LBA_GAME_BIN, GAME_BIN_SECTORS);
    log_gp0_state();
    (**(code **)(*game + 0x4C))(game);
    D0 = 11; D1 = 0;
}
void log_gp0_state(void)
{
    u32 *buf = (u32 *)0x800BAFC0;
    u32 *dispenv = (u32 *)0x800BBD1C;
    u8 *drawenv = (u8 *)0x800BBDBC;
    u32 x, y, w, h;
    buf[0] = 0x4750304C;
    buf[1] = dispenv[0];
    buf[2] = dispenv[1];
    buf[3] = dispenv[2];
    buf[4] = dispenv[3];
    buf[5] = *(u32 *)(drawenv + 0);
    buf[6] = *(u32 *)(drawenv + 4);
    buf[7] = *(u32 *)(drawenv + 8);
    buf[8] = *(u32 *)(drawenv + 12);
    buf[9] = *(u32 *)(drawenv + 16);
    buf[10] = *(u32 *)(drawenv + 20);
    x = *(u16 *)(drawenv + 0x1C);
    y = *(u16 *)(drawenv + 0x1E);
    w = *(u16 *)(drawenv + 0x20);
    h = *(u16 *)(drawenv + 0x22);
    buf[11] = 0xE3000000 | (y << 10) | x;
    buf[12] = 0xE4000000 | ((y + h - 1) << 10) | (x + w - 1);
    u32 ofs_x = *(u16 *)(drawenv + 0x28) & 0x7FF;
    u32 ofs_y = *(u16 *)(drawenv + 0x2A) & 0x7FF;
    buf[13] = 0xE5000000 | (ofs_y << 11) | ofs_x;
    buf[14] = 0x454E444C;
}

/* ==== Begin Memory.c ==== */
#include "common.h"
#include "dat_globals.h"
#include "libspu.h"
#include "libgte.h"
#include "libgpu.h"
#include "libcd.h"

/* Extern declarations for string pointer globals */
extern u32 PTR_s_SND_AMBIENT_800819cc;
extern u32 PTR_s_SND_SE_8008a96c;
/* Forward declarations for functions defined later in this file */
void *GetVtable_ObjectType1(void);
void *GetVtable_ObjectType2(void);
void *GetVtable_ObjectType3(void);
void *GetVtable_ObjectType4(void);
void *GetVtable_ObjectType5(void);
void *GetVtable_ObjectType6(void);
void *GetVtable_ObjectType7(void);
void *GetVtable_ObjectType8(void);
void *GetVtable_ObjectType9(void);
void *GetVtable_ObjectType10(void);
void *GetVtable_ObjectType11(void);
void *GetVtable_ObjectType12(void);
void *GetVtable_ObjectType13(void);
void *GetVtable_ObjectType14(void);
void *GetVtable_ObjectType15(void);
void *GetVtable_ObjectType16(void);
void *GetVtable_ObjectType17(void);
void *GetVtable_ObjectType18(void);
void *GetVtable_ObjectType19(void);
void *GetVtable_ObjectType20(void);
void *GetVtable_ObjectType22(void);
void *GetVtable_ObjectType24(void);
void *GetVtable_AudioType(void);
void *GetVtable_SoundEffect(void);
void *MemSet(uchar *dst, uchar value, int count);
int  Create_ObjectType4(int arg1, u8 arg2);
int  Create_ObjectType13(int arg1);
int  Create_ObjectType14(int arg1);
int  Create_ObjectType15(int arg1, int arg2);
int  Create_ObjectType18(int arg1, int arg2);
int  Create_ObjectType19(int arg1);
int  Create_ObjectType24(void);
int  GetSoundEffectAddr(int index);
int  InitTextureBuffer(int obj, int *src, int isExternal);
void CleanupTextureBuffer(int obj);
void MarkObjectInitialized(int obj);
void WaitUntilReady(int obj);
/* vtable getter for object type 1 (0x6c bytes) */
void *GetVtable_ObjectType1(void)
{
    return &Memory_ObjType1Vtable;
}
/* Allocate and construct object type 1 (0x6c bytes) */
int Create_ObjectType1(int arg1, int arg2, int arg3)
{
    int obj;
    int vtable;
    obj = MemAlloc(SIZEOF_OBJTYPE1);
    vtable = 0;
    if (obj != 0) {
        vtable = GetVtable_ObjectType1();
        (**(code **)(vtable + 8))(obj, arg1, arg2, arg3);
        vtable = obj;
    }
    return vtable;
}
/* Blend RGB color: add or copy depending on mode */
void BlendColor(int unused, char *dst, char *src, int mode)
{
    char r, g, b;
    if (mode == 0) {
        /* Additive blend */
        dst[0] = dst[0] + src[0];
        dst[1] = dst[1] + src[1];
        dst[2] = dst[2] + src[2];
    }
    else {
        /* Direct copy */
        r = src[1];
        g = src[2];
        dst[0] = src[0];
        dst[1] = r;
        dst[2] = g;
    }
}
/* vtable getter for object type 2 (0xb8 bytes) */
void *GetVtable_ObjectType2(void)
{
    return &Memory_ObjType2Vtable;
}
/* Allocate and construct object type 2 (0xb8 bytes) */
int Create_ObjectType2(int arg1, int arg2, int arg3)
{
    int obj;
    int vtable;
    obj = MemAlloc(SIZEOF_OBJTYPE2);
    vtable = 0;
    if (obj != 0) {
        vtable = GetVtable_ObjectType2();
        (**(code **)(vtable + 8))(obj, arg1, arg2, arg3);
        vtable = obj;
    }
    return vtable;
}
/* vtable getter for object type 3 */
void *GetVtable_ObjectType3(void)
{
    return &Memory_ObjType3Vtable;
}
/* Convert Shift-JIS encoded string to ASCII */
char *ShiftJisToAscii(char *dst, char *src)
{
    char c;
    byte secondByte;
    char *result;
    c = *src;
    result = dst;
    while (c != '\0') {
        secondByte = src[1];
        dst = dst + 1;
        if ((0x7f < secondByte) || (c = -0x1f, secondByte == 0x40)) {
            c = -0x20;
        }
        src = src + 2;
        *result = secondByte + c;
        result = result + 1;
        c = *src;
    }
    *dst = '\0';
    return result;
}
/* Convert ASCII string to Shift-JIS encoded string */
u8 *AsciiToShiftJis(u8 *dst, byte *src)
{
    byte c;
    char offset;
    u8 highByte;
    char *nextDst;
    c = *src;
    while (c != 0) {
        nextDst = (char *)(dst + 1);
        highByte = 0x81;
        if (0x2f < *src) {
            highByte = 0x82;
        }
        *dst = highByte;
        c = *src;
        dst = dst + 2;
        if ((0x5f < c) || (offset = '\x1f', c == 0x20)) {
            offset = ' ';
        }
        src = src + 1;
        *nextDst = c + offset;
        c = *src;
    }
    *dst = 0;
    return dst;
}
/* Format a number with zero-padding to specified width */
void FormatNumberPadded(char *outStr, int number, int width, int zeroPad)
{
    uint paddedWidth;
    char *str;
    int len;
    uchar stackBuf[64];
    uchar *numBuf;
    uchar *padBuf;
    paddedWidth = width + 0xfU & 0xfffffff8;
    numBuf = &stackBuf[32] - paddedWidth;
    padBuf = &stackBuf[32] - paddedWidth * 2;
    str = itoa(number);
    str = strcpy((char *)numBuf, str);
    len = strlen(str);
    if (zeroPad == 0) {
        MemSet(padBuf, '0', width);
        strcpy((char *)(padBuf + (width - len)), (char *)numBuf);
        numBuf = padBuf;
    }
    AsciiToShiftJis((u8 *)outStr, numBuf);
}
/* Set memory block to value (memset implementation) */
void *MemSet(uchar *dst, uchar value, int count)
{
    uchar *cur;
    uchar *result;
    result = (uchar *)0x0;
    cur = dst;
    if (dst != (uchar *)0x0) {
        for (; 0 < count; count = count + -1) {
            *cur = value;
            result = dst;
            cur = cur + 1;
        }
    }
    return result;
}
/* Allocate and construct object type 4 (0xac bytes) */
int Create_ObjectType4(int arg1, u8 arg2)
{
    int obj;
    int vtable;
    obj = MemAlloc(SIZEOF_OBJTYPE3);
    vtable = 0;
    if (obj != 0) {
        vtable = GetVtable_ObjectType4();
        (**(code **)(vtable + 8))(obj, arg1, arg2);
        vtable = obj;
    }
    return vtable;
}
/* vtable getter for object type 4 */
void *GetVtable_ObjectType4(void)
{
    return &Memory_ObjType4Vtable;
}
/* Set up texture page UV coordinates from TPage value */
void SetTexturePageCoords(short *coords, uint tpage)
{
    coords[0] = 0;
    coords[1] = 0;
    coords[2] = 8;
    coords[3] = 0;
    coords[4] = 8;
    coords[5] = 0;
    *coords = *coords + ((ushort)tpage & TPAGE_X_MASK) * 8;
    coords[1] = coords[1] + (short)((tpage & 0xff) >> TPAGE_X_SHIFT) * 8;
}
/* Allocate and construct object type 5 (0xa8 bytes) */
int Create_ObjectType5(int arg1, int arg2, int arg3)
{
    int obj;
    int vtable;
    obj = MemAlloc(SIZEOF_OBJTYPE4);
    vtable = 0;
    if (obj != 0) {
        vtable = GetVtable_ObjectType5();
        (**(code **)(vtable + 8))(obj, arg1, arg2, arg3);
        vtable = obj;
    }
    return vtable;
}
/* vtable getter for object type 5 */
void *GetVtable_ObjectType5(void)
{
    return &Memory_ObjType5Vtable;
}
/* Initialize texture mapping data with GetTPage call */
void InitTextureMapping(int *tex, int mode, u8 *clut, uint *tpageInfo)
{
    u16 tpageVal;
    uint flags;
    flags = *tpageInfo;
    *(u16 *)(tex + 1) = 0;
    *(u16 *)((int)tex + 6) = 0;
    *tex = (flags & ENTITY_STATE_TYPE_MASK) << 0x18;
    *(u16 *)(tex + 2) = *(u16 *)(clut + 4);
    *(u16 *)((int)tex + 10) = *(u16 *)(clut + 8);
    *(ushort *)(tex + 6) = *(ushort *)(tex + 2) >> 1;
    *(ushort *)((int)tex + 0x1a) = *(ushort *)((int)tex + 10) >> 1;
    tpageVal = GetTPage(flags & 3, mode, (int)(short)tpageInfo[1], (int)*(short *)((int)tpageInfo + 6));
    *(u_short *)(tex + 3) = tpageVal;
    *(u8 *)((int)tex + 0xe) = *clut;
    *(u8 *)((int)tex + 0xf) = clut[2];
    *(short *)(tex + 4) = (short)tpageInfo[4];
    u16 textureH = *(u16 *)((int)tpageInfo + 0x12);
    *(u8 *)((int)tex + 0x16) = 0x80;
    *(u8 *)((int)tex + 0x15) = 0x80;
    *(u8 *)(tex + 5) = 0x80;
    tex[8] = 0;
    *(u16 *)(tex + 7) = FIXED_12;
    *(u16 *)((int)tex + 0x1e) = FIXED_12;
    *(u16 *)((int)tex + 0x12) = textureH;
}
/* vtable getter for object type 6 */
void *GetVtable_ObjectType6(void)
{
    return &Memory_ObjType6Vtable;
}
/* Allocate and construct object type 6 (0x30 bytes) */
int Create_ObjectType6(int arg1)
{
    int obj;
    int vtable;
    obj = MemAlloc(SIZEOF_OBJTYPE5);
    vtable = 0;
    if (obj != 0) {
        vtable = GetVtable_ObjectType6();
        (**(code **)(vtable + 8))(obj, arg1);
        vtable = obj;
    }
    return vtable;
}
/* vtable getter for object type 7 */
void *GetVtable_ObjectType7(void)
{
    return &Memory_ObjType7Vtable;
}
/* Allocate and construct object type 7 (0x1c bytes) */
int Create_ObjectType7(void)
{
    int obj;
    int vtable;
    obj = MemAlloc(SIZEOF_OBJTYPE6);
    vtable = 0;
    if (obj != 0) {
        vtable = GetVtable_ObjectType7();
        (**(code **)(vtable + 8))(obj);
        vtable = obj;
    }
    return vtable;
}
/* vtable getter for object type 8 */
void *GetVtable_ObjectType8(void)
{
    return &Memory_ObjType8Vtable;
}
/* Allocate and construct object type 8 (0x54 bytes) */
int Create_ObjectType8(void)
{
    int obj;
    int vtable;
    obj = MemAlloc(SIZEOF_OBJTYPE7);
    vtable = 0;
    if (obj != 0) {
        vtable = GetVtable_ObjectType8();
        (**(code **)(vtable + 8))(obj);
        vtable = obj;
    }
    return vtable;
}
/* vtable getter for object type 9 */
void *GetVtable_ObjectType9(void)
{
    return &Memory_ObjType9Vtable;
}
/* Allocate and construct object type 9 (0x20 bytes) */
int Create_ObjectType9(int arg1)
{
    int obj;
    int vtable;
    obj = MemAlloc(SIZEOF_OBJTYPE8);
    vtable = 0;
    if (obj != 0) {
        vtable = GetVtable_ObjectType9();
        (**(code **)(vtable + 8))(obj, arg1);
        vtable = obj;
    }
    return vtable;
}
/* vtable getter for object type 10 */
void *GetVtable_ObjectType10(void)
{
    return &Memory_ObjType10Vtable;
}
/* Allocate and construct object type 10 (0x84 bytes) */
int Create_ObjectType10(int arg1)
{
    int obj;
    int vtable;
    obj = MemAlloc(SIZEOF_OBJTYPE9);
    vtable = 0;
    if (obj != 0) {
        vtable = GetVtable_ObjectType10();
        (**(code **)(vtable + 8))(obj, arg1);
        vtable = obj;
    }
    return vtable;
}
/* Find the maximum priority value across all entries in a list */
uint GetMaxPriority(int listPtr)
{
    uint *entry;
    uint count;
    uint maxPriority;
    uint i;
    entry = *(uint **)(listPtr + 0x10);
    maxPriority = 0;
    count = *entry;
    i = 0;
    if (count != 0) {
        do {
            uint *priorityField;
            priorityField = entry + 5;
            entry = entry + 1;
            if (i < *priorityField) {
                i = *priorityField;
            }
            maxPriority = maxPriority + 1;
        } while (maxPriority < count);
    }
    return i;
}
/* Blend texture page VRAM row colors with a target row */
void BlendTextureRow(ushort *texInfo, short targetRow)
{
    byte bVar1;
    ushort pixel;
    ushort blendR, blendG;
    int i;
    RECT srcRect;
    RECT dstRect;
    u_long srcData[128];
    u_long dstData[132];
    /* Source rect: read from VRAM row 0x1e0 + targetRow*8 */
    srcRect.x = 0;
    srcRect.w = FIXED_8;
    srcRect.h = 1;
    srcRect.y = targetRow * 8 + 0x1e0;
    StoreImage(&srcRect, dstData);
    DrawSync(0);
    /* Destination rect */
    dstRect.h = 1;
    dstRect.x = 0;
    dstRect.w = 0x100;
    pixel = *texInfo;
    blendR = texInfo[6];
    bVar1 = *(byte *)((int)texInfo + 0xd);
    blendG = texInfo[7];
    texInfo[5] = texInfo[1];
    if (0 < (int)(texInfo[1] - 1)) {
        int shiftAmount = 1;
        int row = 0;
        do {
            int mask;
            RECT *curDst;
            shiftAmount = shiftAmount << (0xc - pixel & TPAGE_X_MASK);
            mask = 0x1000 - shiftAmount;
            i = 0;
            curDst = &dstRect;
            if (0 < srcRect.w) {
                do {
                    pixel = curDst[0x42].x;
                    if (pixel == 0) {
                        curDst[2].x = curDst[0x42].x;
                    }
                    else {
                        /* Blend: weighted average of existing pixel and target color */
                        curDst[2].x = pixel & 0x8000U |
                                       (ushort)((int)(((ushort)pixel & 0x1f) * 8 * mask +
                                                      (uint)(byte)blendR * shiftAmount) >> 15) |
                                       (ushort)(((int)((pixel >> 2 & 0xf8) * mask + (uint)bVar1 * shiftAmount) >> 15) << TPAGE_X_SHIFT) |
                                       (ushort)(((int)((pixel >> 7 & 0xf8) * mask + (uint)(byte)blendG * shiftAmount) >> 15) << 10);
                    }
                    i = i + 1;
                    curDst = (RECT *)&curDst->y;
                } while (i < srcRect.w);
            }
            dstRect.y = srcRect.y + (short)row + srcRect.h;
            DrawSync(0);
            LoadImage(&dstRect, srcData);
            row = row + 1;
        } while (row < (int)(texInfo[1] - 1));
    }
}
/* vtable getter for object type 11 */
void *GetVtable_ObjectType11(void)
{
    return &Memory_ObjType11Vtable;
}
/* Allocate, construct object type 12 (0x30 bytes), free on init failure */
int Create_ObjectType12_Safe(int arg1)
{
    int obj;
    int vtable;
    int initResult;
    obj = MemAlloc(SIZEOF_OBJTYPE10);
    vtable = 0;
    if (obj != 0) {
        vtable = GetVtable_ObjectType12();
        initResult = (**(code **)(vtable + 8))(obj, arg1);
        vtable = obj;
        if (initResult == 0) {
            MemFree(obj);
            vtable = 0;
        }
    }
    return vtable;
}
/* vtable getter for object type 12 */
void *GetVtable_ObjectType12(void)
{
    return &Memory_ObjType12Vtable;
}
/* Allocate and construct object type 13 (0x3c bytes) */
int Create_ObjectType13(int arg1)
{
    int obj;
    int vtable;
    obj = MemAlloc(SIZEOF_OBJTYPE11);
    vtable = 0;
    if (obj != 0) {
        vtable = GetVtable_ObjectType13();
        (**(code **)(vtable + 8))(obj, arg1);
        vtable = obj;
    }
    return vtable;
}
/* vtable getter for object type 13 */
void *GetVtable_ObjectType13(void)
{
    return &Memory_ObjType13Vtable;
}
/* Allocate and construct object type 14 (0x2c bytes) */
int Create_ObjectType14(int arg1)
{
    int obj;
    int vtable;
    obj = MemAlloc(SIZEOF_OBJTYPE12);
    vtable = 0;
    if (obj != 0) {
        vtable = GetVtable_ObjectType14();
        (**(code **)(vtable + 8))(obj, arg1);
        vtable = obj;
    }
    return vtable;
}
/* Extract color channels from a packed 32-bit value and advance pointer */
uint *ExtractColorChannels(int unused, uint *colorPtr,
                           u8 *outAlpha, byte *outR, byte *outG, u8 *outB)
{
    uint packed;
    packed = *colorPtr;
    *outAlpha = (char)packed;
    *outR = (byte)(packed >> 0x10) & 0xf;
    *outG = (byte)(packed >> 0x14) & 0xf;
    *outB = (char)(packed >> 0x18);
    return colorPtr + 1;
}
/* vtable getter for object type 14 */
void *GetVtable_ObjectType14(void)
{
    return &Memory_ObjType14Vtable;
}
/* Allocate and construct object type 15 (0x68 bytes) */
int Create_ObjectType15(int arg1, int arg2)
{
    int obj;
    int vtable;
    obj = MemAlloc(SIZEOF_OBJTYPE13);
    vtable = 0;
    if (obj != 0) {
        vtable = GetVtable_ObjectType15();
        (**(code **)(vtable + 8))(obj, arg1, arg2);
        vtable = obj;
    }
    return vtable;
}
/* vtable getter for object type 15 */
void *GetVtable_ObjectType15(void)
{
    return &Memory_ObjType15Vtable;
}
/* Allocate, construct object type 16 (0x38 bytes), free on init failure */
int Create_ObjectType16_Safe(int arg1)
{
    int obj;
    int vtable;
    int initResult;
    obj = MemAlloc(SIZEOF_OBJTYPE14);
    vtable = 0;
    if (obj != 0) {
        vtable = GetVtable_ObjectType16();
        initResult = (**(code **)(vtable + 8))(obj, arg1, 1);
        vtable = obj;
        if (initResult == 0) {
            MemFree(obj);
            vtable = 0;
        }
    }
    return vtable;
}
/* vtable getter for object type 16 */
void *GetVtable_ObjectType16(void)
{
    return &Memory_ObjType16Vtable;
}
/* Allocate, construct object type 17 (0x3c bytes), free on init failure */
int Create_ObjectType17_Safe(int arg1)
{
    int obj;
    int vtable;
    int initResult;
    obj = MemAlloc(SIZEOF_OBJTYPE15);
    vtable = 0;
    if (obj != 0) {
        vtable = GetVtable_ObjectType17();
        initResult = (**(code **)(vtable + 8))(obj, arg1);
        vtable = obj;
        if (initResult == 0) {
            MemFree(obj);
            vtable = 0;
        }
    }
    return vtable;
}
/* vtable getter for object type 17 */
void *GetVtable_ObjectType17(void)
{
    return &Memory_ObjType17Vtable;
}
/* Allocate and construct object type 18 (0x44 bytes) */
int Create_ObjectType18(int arg1, int arg2)
{
    int obj;
    int vtable;
    obj = MemAlloc(SIZEOF_OBJTYPE16);
    vtable = 0;
    if (obj != 0) {
        vtable = GetVtable_ObjectType18();
        (**(code **)(vtable + 8))(obj, arg1, arg2);
        vtable = obj;
    }
    return vtable;
}
/* vtable getter for object type 18 */
void *GetVtable_ObjectType18(void)
{
    return &Memory_ObjType18Vtable;
}
/* Allocate and construct object type 19 (0x38 bytes) */
int Create_ObjectType19(int arg1)
{
    int obj;
    int vtable;
    obj = MemAlloc(SIZEOF_OBJTYPE17);
    vtable = 0;
    if (obj != 0) {
        vtable = GetVtable_ObjectType19();
        (**(code **)(vtable + 8))(obj, arg1);
        vtable = obj;
    }
    return vtable;
}
/* vtable getter for object type 19 */
void *GetVtable_ObjectType19(void)
{
    return &Memory_ObjType19Vtable;
}
/* Allocate, construct object type 20 (0x2c bytes), free on init failure */
int Create_ObjectType20_Safe(int arg1)
{
    int obj;
    int vtable;
    int initResult;
    obj = MemAlloc(SIZEOF_OBJTYPE18);
    vtable = 0;
    if (obj != 0) {
        vtable = GetVtable_ObjectType20();
        initResult = (**(code **)(vtable + 8))(obj, arg1);
        vtable = obj;
        if (initResult == 0) {
            MemFree(obj);
            vtable = 0;
        }
    }
    return vtable;
}
/* vtable getter for object type 20 */
void *GetVtable_ObjectType20(void)
{
    return &Memory_ObjType20Vtable;
}
/* Allocate, construct object type 21 (0x6c bytes), free on init failure */
int Create_ObjectType21_Safe(int arg1, int arg2, int arg3)
{
    int obj;
    int vtable;
    int initResult;
    obj = MemAlloc(SIZEOF_OBJTYPE19);
    vtable = 0;
    if (obj != 0) {
        vtable = GetVtable_ObjectType21();
        initResult = (**(code **)(vtable + 8))(obj, arg1, arg2, arg3);
        vtable = obj;
        if (initResult != 0) {
            MemFree(obj);
            vtable = 0;
        }
    }
    return vtable;
}
/* Initialize or configure texture buffer: allocate 4 buffers if source is NULL,
   or copy texture parameters if source is provided */
int InitTextureBuffer(int obj, int *src, int isExternal)
{
    int bufSize;
    int buf1;
    int buf2;
    int buf3;
    int buf4;
    int srcW, srcH;
    *(int *)(obj + 0xc) = isExternal;
    if (isExternal == 0) {
        /* Allocate 4 buffers for texture data */
        *(int *)(obj + 0x1c) = 0;
        *(int *)(obj + 0x18) = 0;
        *(int *)(obj + 0x14) = 0;
        *(int *)(obj + 0x10) = 0;
        bufSize = src[1] * src[2] * 2 + 0x1000;
        buf1 = MemAlloc(bufSize);
        *(int *)(obj + 0x14) = buf1;
        if (buf1 != 0) {
            buf2 = MemAlloc(bufSize);
            *(int *)(obj + 0x18) = buf2;
            if (buf2 != 0) {
                buf3 = MemAlloc(0x12000);
                *(int *)(obj + 0x10) = buf3;
                if (buf3 != 0) {
                    buf4 = MemAlloc(src[2] << 5);
                    *(int *)(obj + 0x1c) = buf4;
                    if (buf4 != 0) goto COPY_PARAMS;
                }
            }
        }
        CleanupTextureBuffer(obj);
        return 1;
    }
COPY_PARAMS:
    srcW = src[1];
    srcH = src[2];
    /* Copy original texture dimensions */
    *(int *)(obj + 0x2c) = *src;
    *(int *)(obj + 0x30) = srcW;
    *(int *)(obj + 0x34) = srcH;
    /* Copy current texture dimensions */
    *(int *)(obj + 0x20) = *(int *)(obj + 0x2c);
    *(int *)(obj + 0x24) = *(int *)(obj + 0x30);
    *(int *)(obj + 0x28) = *(int *)(obj + 0x34);
    /* Set stride to 0x10 and calculate half-height */
    *(int *)(obj + 0x30) = 0x10;
    *(int *)(obj + 0x38) = (*(int *)(obj + 0x34) << 4) >> 1;
    return 0;
}
/* Free all 4 texture buffers if object is in unconfigured state */
void CleanupTextureBuffer(int obj)
{
    if (*(int *)(obj + 0xc) == 0) {
        MemFree(*(int *)(obj + 0x14));
        MemFree(*(int *)(obj + 0x18));
        MemFree(*(int *)(obj + 0x10));
        MemFree(*(int *)(obj + 0x1c));
    }
}
/* Mark object as initialized */
void MarkObjectInitialized(int obj)
{
    *(int *)(obj + 0x50) = 1;
}
/* Busy-wait until object becomes ready (spin on flag at offset 0x4c) */
void WaitUntilReady(int obj)
{
    if (*(int *)(obj + 0x4c) == 0) {
        do {
        } while (true);
    }
}
/* vtable getter for object type 22 */
void *GetVtable_ObjectType22(void)
{
    return &Memory_ObjType22Vtable;
}
/* Allocate and construct object type 23 (0x5c bytes) */
int Create_ObjectType23(int arg1, int arg2, int arg3)
{
    int obj;
    int vtable;
    obj = MemAlloc(0x5c);
    vtable = 0;
    if (obj != 0) {
        vtable = GetVtable_ObjectType23();
        (**(code **)(vtable + 8))(obj, arg1, arg2, arg3);
        vtable = obj;
    }
    return vtable;
}
/* Initialize SPU common audio attributes (volume, CD mix) */
int InitSpuAudio(void)
{
    SpuCommonAttr attr;
    attr.mask = 0x2c3;
    attr.mvol.left = 0x3fff;
    attr.mvol.right = 0x3fff;
    attr.cd.volume.left = 0x7fff;
    attr.cd.volume.right = 0x7fff;
    attr.cd.mix = 1;
    SpuSetCommonAttr(&attr);
    return 1;
}
/* Call cleanup callback and invoke destructor via vtable */
void CleanupWithCallback(int *obj, int arg2)
{
    if ((code *)obj[0x12] != (code *)0x0) {
        (*(code *)obj[0x12])(obj[0x11]);
        (**(code **)(*obj + 0x70))(obj, arg2);
    }
}
/* Call cleanup callback and invoke destruction via vtable */
void DestroyWithCallback(int *obj)
{
    if (obj[0x13] != 0) {
        (*(code *)obj[0x12])(obj[0x11]);
        (**(code **)(*obj + 0x48))(obj);
    }
}
/* vtable getter for audio object type */
void *GetVtable_AudioType(void)
{
    return &Memory_AudioTypeVtable;
}
/* Reset CD-ROM: disable callbacks and clear registers */
void ResetCdRom(void)
{
    EnterCriticalSection();
    CdDataCallback(NULL);
    CdReadyCallback(NULL);
    *(volatile u8 *)REG_IO_PORT = 0;
    *(volatile u8 *)REG_IO_PORT_03 = 0;
    ExitCriticalSection();
}
/* Store CD-ROM configuration parameters */
void SetCdRomParams(int arg1, int arg2, int arg3)
{
    Memory_CdRomParam1 = arg1;
    Memory_CdRomParam2 = arg2;
    Memory_CdRomParam3 = arg3;
}
/* Allocate and construct object type 24 (0x3c bytes) */
int Create_ObjectType24(void)
{
    int obj;
    int vtable;
    obj = MemAlloc(0x3c);
    vtable = 0;
    if (obj != 0) {
        vtable = GetVtable_ObjectType24();
        (**(code **)(vtable + 8))(obj);
        vtable = obj;
    }
    return vtable;
}
/* vtable getter for object type 24 */
void *GetVtable_ObjectType24(void)
{
    return &Memory_ObjType24Vtable;
}
/* vtable getter for sound effect data */
void *GetVtable_SoundEffect(void)
{
    return &Memory_SoundPathCdi;
}
/* Seed random number generator and call rand once */
uint SeedRandom(uint seed)
{
    if (seed != 0) {
        srand(seed);
    }
    return (uint)rand();
}
/* Get ambient sound file path and optional size */
char *GetAmbientSoundPath(int *outSize)
{
    if (outSize != NULL) {
        *outSize = 0x230;
    }
    return "SND\\AMBIENT.VH";
}
/* Get pointer to sound file table */
void **GetSoundFileTable(void)
{
    return &PTR_s_SND_AMBIENT_800819cc;
}
/* Pick a random entry from the ambient sound file table */
int PickRandomAmbientSound(int arg1)
{
    uint index;
    int table;
    index = SeedRandom(0);
    table = (int)GetSoundFileTable();
    return *(int *)(table + (index % 7) * 4);
}
/* Get pointer to sound effects table */
void **GetSoundEffectTable(void)
{
    return &PTR_s_SND_SE_8008a96c;
}
/* Get first entry from the sound effects table */
int GetFirstSoundEffect(void)
{
    int *table;
    table = (int *)GetSoundEffectTable();
    return *table;
}
/* Calculate sound effect address from index using lookup table */
int GetSoundEffectAddr(int index)
{
    int baseAddr;
    baseAddr = (int)GetAmbientSoundPath(0);
    return baseAddr + *(short *)(&Memory_SoundEffectOffsetTbl + index * 2) * 0x1c;
}
/* Wrapper: call GetSoundEffectAddr */
void GetSoundEffectAddrWrapper(void)
{
    GetSoundEffectAddr(0);
}
/* Pick a random sound effect address from a priority-based range */
int PickRandomSoundEffect(int arg1, int arg2, int priority)
{
    int rangeSize;
    int randomVal;
    int baseAddr;
    rangeSize = ((priority + -1) % 0x28) / 10 + 1;
    randomVal = SeedRandom(0);
    if (rangeSize == 0) {
        trap(0x1c00);
    }
    if ((rangeSize == -1) && (randomVal == -0x80000000)) {
        trap(0x1800);
    }
    baseAddr = GetSoundEffectAddr(arg1);
    return baseAddr + (randomVal % rangeSize) * 0x1c;
}
/* Get address of the first entry in the sound effect sub-table */
int GetSoundEffectSubTable(void)
{
    int baseAddr;
    baseAddr = GetSoundEffectAddr(0);
    return baseAddr + 0x70;
}
/* Pick a random sound effect with special handling for index 9 */
int PickRandomSoundEffectForScene(int sceneIndex)
{
    uint randomVal;
    int tableAddr;
    randomVal = SeedRandom(0);
    randomVal = randomVal % 5;
    /* Special case: scene 9 has restricted randomness */
    if (sceneIndex == 9) {
        if (randomVal == 2) {
            randomVal = 3;
        }
        if (Memory_SoundEffectIndex == 3) {
            Memory_SoundEffectIndex = 4;
        }
    }
    tableAddr = GetSoundEffectSubTable();
    /* Override random selection with stored value if non-zero */
    if (Memory_SoundEffectIndex != 0) {
        randomVal = Memory_SoundEffectIndex - 1;
    }
    return tableAddr + randomVal * 0x1c;
}

/* ==== Begin D294.c ==== */
#include "common.h"
#include "dat_globals.h"
#include "libgte.h"
#include "libgpu.h"
#include "libcd.h"
/* Extern functions from other C/ASM files */
extern void *MemAllocImpl(int size);
extern void MemFreeImpl(int ptr);
extern void *GetCoordSystemVtable(void);
extern void LinkedListPopFront(void **a, int **b);
extern int GetDefaultSoundVolume(void);
extern void CopyDefaultToBuffer(int a);
extern int GetSoundParamRef(int a, int b);
extern void InitSoundParamTable(int a);
extern int RayIntersection(int a, void *b, void *c, int d, void *e, void *f);
extern void BasicClass__func_1816c(int a, void *b, int *c);
extern u32 GsScreenFlipMode;
/* ======================================================================== */
/* Scene graph primitive allocation, initialization, and manipulation       */
/* Corresponds to ASM file: D294.s (0xD294 - 0xEF60)                        */
/* ======================================================================== */
/* Allocate 68-byte primitive, call vtable+8, free on failure. Returns obj or 0 */
int SceneNodeAlloc(void)
{
    int obj;
    int vtab;
    int result;
    obj = MemAllocImpl(0x44);
    if (obj != 0) {
        vtab =  SceneNodeAlloc ();
        result = (**(code **)(vtab + 8))(obj);
        if (result == 0) {
            MemFreeImpl((void *)obj);
            obj = 0;
        }
    }
    return obj;
}
/* Allocate two objects (0x50 + 0x28), chain them, init. Returns obj or 0 */
int * GetPrimitiveBaseTable (int *obj)
{
    int iVar1;
    iVar1 = MemAllocImpl(0x50);
    obj[5] = iVar1;
    if (iVar1 != 0) {
        iVar1 = MemAllocImpl(0x28);
        *(int *)(obj[5] + 0x44) = iVar1;
        if (iVar1 != 0) {
            iVar1 = GetCoordSystemVtable();
            (**(code **)(iVar1 + 8))(obj);
            iVar1 =  SceneNodeConstructor ();
            *obj = iVar1;
            obj[8] = 0;
            obj[6] = 0;
            obj[3] = 0;
            *(int *)(obj[5] + 0x48) = 0;
            (**(code **)(*obj + 0x40))(obj);
            return obj;
        }
        MemFreeImpl((void *)obj[5]);
    }
    return 0;
}
/* Destroy primitive: call vtable cleanup methods, free sub-objects, free main */
void SceneNodeDestructor(int *obj)
{
    int vtab;
    (**(code **)(*obj + 0x50))();
    (**(code **)(*obj + 0x54))(obj);
    (**(code **)(*obj + 0x5c))(obj, 0);
    MemFreeImpl((void *)*(int *)(obj[5] + 0x44));
    MemFreeImpl((void *)obj[5]);
    vtab = GetCoordSystemVtable();
    (**(code **)(vtab + 0xc))(obj);
}
/* Transition-in handler: call vtable+0x10, check type==9, call  func_8001e57c  */
void SceneNodeAttach(int handle, int *typeInfo)
{
    int vtab;
    vtab = GetCoordSystemVtable();
    (**(code **)(vtab + 0x10))(handle, typeInfo);
    if ((*(int *)*typeInfo & 0xf) == 9) {
         func_8001cba4 (handle, typeInfo);
    }
}
/* Transition-out handler: check type==9, call  func_8001e770 , call vtab+0x14 */
void SceneNodeDetach(int handle, int *typeInfo)
{
    int vtab;
    if ((*(int *)*typeInfo & 0xf) == 9) {
         func_8001cc48 (handle);
    }
    vtab = GetCoordSystemVtable();
    (**(code **)(vtab + 0x14))(handle, typeInfo);
}
/* Destructor helper: call  func_8001e770 , then vtab+0x18 */
void SceneNodeClear(int handle)
{
    int vtab;
     func_8001e7b0 (handle);
    vtab = GetCoordSystemVtable();
    (**(code **)(vtab + 0x18))(handle);
}
/* Type-dispatch render: call vtab+0x38, then call type-specific vtab entry (0x94/0x98/0x9c) */
void SceneNodeRenderDispatch(int *obj, int *typeInfo, int arg)
{
    int vtab;
    code handler;
    int type;
    vtab = GetCoordSystemVtable();
    (**(code **)(vtab + 0x38))(obj, typeInfo, arg);
    type = *(int *)*typeInfo & 0xf;
    if (type == 2) {
        handler = *(code *)(*obj + 0x94);
    } else if (type == 5) {
        handler = *(code *)(*obj + 0x98);
    } else if (type == 4) {
        handler = *(code *)(*obj + 0x9c);
    } else {
        return;
    }
    handler(obj, typeInfo, arg);
}
/* Init coordinate system: reset offsets, call GsInitCoordinate2, set vtable rotation+light */
void SceneNodeInitCoords(int *obj)
{
    obj[9] = 0;
    obj[4] = 0;
    GsInitCoordinate2((GsCOORDINATE2 *)0, (GsCOORDINATE2 *)obj[5]);
    (**(code **)(*obj + 0x44))(obj, 1, Primitive_AuxDataA);
    (**(code **)(*obj + 0x48))(obj, 1, Primitive_AuxDataB);
    *(int *)obj[5] = 1;
}
/* Apply rotation offset: read 3 packed angles, divide by 360, apply to sub-object */
void SceneNodeSetRotation(int obj, int mode, int data)
{
    int a, b, c;
    int i;
    int *piVar4;
    short *psVar5;
    int subObj;
    int local_20;
    local_20 =  func_8001ccb4 (data);
    a =  func_8001e7b0 (data + 4);
    b =  func_8001e7b0 (data + 8);
    local_20 /= 0x168;
    a /= 0x168;
    b /= 0x168;
    subObj = *(int *)(*(int *)(obj + 0x14) + 0x44);
    if (mode == 0) {
        c = 0;
        for (i = 0; i < 3; i++) {
            int *val = &local_20 + i;
            short *dest = (short *)(subObj + 0x10) + i;
            int tmp = (int)*dest + *val;
            int shift = tmp;
            if (tmp < 0) shift = tmp + 0xfff;
            *dest = (short)(tmp + (short)(shift >> 0xc) * -0x1000);
        }
    } else {
        *(short *)(subObj + 0x10) = (short)local_20;
        *(short *)(subObj + 0x12) = (short)a;
        *(short *)(subObj + 0x14) = (short)b;
    }
    **(int **)(obj + 0x14) = 0;
}
/* Apply translation offset: read 3 packed values, add to or set sub-object position */
void SceneNodeSetPosition(int obj, int mode, int data)
{
    short s1, s2, s3;
    int *dest;
    s1 = (short) func_8001cd20 (data);
    s2 = (short) func_8001e7b0 (data + 4);
    s3 = (short) func_8001cd60 (data + 8);
    dest = *(int **)(*(int *)(obj + 0x14) + 0x44);
    if (mode == 0) {
        dest[0] = (int)s1 + dest[0];
        dest[1] = (int)s2 + dest[1];
        dest[2] = (int)s3 + dest[2];
    } else {
        dest[0] = (int)s1;
        dest[1] = (int)s2;
        dest[2] = (int)s3;
    }
    **(int **)(obj + 0x14) = 0;
}
/* Attach child primitive: store in slot+0xc, copy position data, init position */
int SceneNodeAddChild(int obj, int *child, int *posData)
{
    if (*(int *)(obj + 0xc) == 0) {
        *(int **)(obj + 0xc) = child;
        *(int *)(*(int *)(obj + 0x14) + 0x48) = child[5];
        (**(code **)(*child + 0x10))(child, obj);
        if (posData == 0) {
            *(int *)(*(int *)(obj + 0x14) + 0x18) = 0;
            *(int *)(*(int *)(obj + 0x14) + 0x1c) = 0;
            *(int *)(*(int *)(obj + 0x14) + 0x20) = 0;
        } else {
            *(int *)(*(int *)(obj + 0x14) + 0x18) = posData[0];
            *(int *)(*(int *)(obj + 0x14) + 0x1c) = posData[1];
            *(int *)(*(int *)(obj + 0x14) + 0x20) = posData[2];
        }
        **(int **)(obj + 0x14) = 0;
    }
    return obj;
}
/* Detach child primitive: call cleanup, clear slots */
int SceneNodeRemoveChild(int obj)
{
    int *child;
    child = *(int **)(obj + 0xc);
    if (child != 0) {
        (**(code **)(*child + 0x14))(child, obj);
        *(int *)(*(int *)(obj + 0x14) + 0x48) = 0;
        *(int *)(obj + 0xc) = 0;
    }
    return obj;
}
/* Iterate and destroy all children via vtable+0x58 */
void SceneNodeIterateChildren(int *obj)
{
    int *child;
    int more;
    child = 0;
    do {
        (**(code **)(*obj + 0x58))(obj, &child, &more);
        if (child != 0) {
            (**(code **)(*child + 0x50))();
        }
    } while (more != 0);
}
/* Empty function (stub) */
void NopSub_1d33c(void) { }
/* Check if bit 0x1f is cleared in offset+0x10 */
int SceneNodeSetHidden(int obj, int val)
{
    return  func_8001ce30 (obj + 0x10, 0x1f, 1, val == 0) == 0;
}
/* Set bit 0x1e at offset+0x10 */
void SceneNodeSetNoClip(int obj, int val)
{
     func_8001ceb4 (obj + 0x10, 0x1e, 1, val != 0);
}
/* Write 2-bit value at bit offset 0x1c */
void SceneNodeSetShadow(int obj, int val)
{
     MathFixedDiv (obj + 0x10, 0x1c, 2, val);
}
/* Check/clear bit 6 at offset+0x10 */
void SceneNodeSetInherit(int obj, int val)
{
     MathFixedDiv (obj + 0x10, 6, 1, val == 0);
}
/* Write 3-bit value at bit offset 3 */
void SceneNodeConfigureDrawMode(int obj, int val)
{
     MathFixedDiv (obj + 0x10, 3, 3, val);
}
/* Write 3-bit value at bit offset 0 */
void SceneNodeSetRenderType(int obj, int val)
{
     func_8001d008 (obj + 0x10, 0, 3, val);
}
/* Check bit 7 at offset+0x10 */
int SceneNodeSetCollide(int obj, int val)
{
    return  MathFixedDiv (obj + 0x10, 7, 1, val == 0) == 0;
}
/* Write 3-bit value at bit offset 9 */
void SceneNodeSetAnimParams(int obj, int val)
{
     MathFixedDiv (obj + 0x10, 9, 3, val);
}
/* Check bit 8 at offset+0x10 */
int SceneNodeSetCollidableShape(int obj, int val)
{
    return  MathFixedDiv (obj + 0x10, 8, 1, val == 0) == 0;
}
/* Build rotation matrix from sub-object angles, negated or packed */
void SceneNodeGenerateRotationMatrix(int obj, MATRIX *mat, int mode)
{
    SVECTOR local;
    int subObj;
    subObj = *(int *)(*(int *)(obj + 0x14) + 0x44);
    if (mode == 0) {
        local.vx = -*(short *)(subObj + 0x10);
        local.vy = -*(short *)(subObj + 0x12);
        local.vz = -*(short *)(subObj + 0x14);
    } else {
        local.vy = -*(short *)(subObj + 0x12);
        local.vx = -*(short *)(subObj + 0x10);
        local.vz = -*(short *)(subObj + 0x14);
    }
    RotMatrix(&local, mat);
}
/* Conditional render: dispatch to vtab+0x8c and +0x90 if arg in (2,3) and visible */
void SceneNodeRenderChildren(int *obj, int arg)
{
    int visible;
    if (arg > 1 && arg < 4 && obj[8] != 0) {
        visible =  func_8001d0ec ();
        if (visible != 0) {
            (**(code **)(*obj + 0x8c))(obj);
            (**(code **)(*obj + 0x90))(obj, arg);
        }
    }
}
/* Call  func_8001d1a4  with the parameter at offset+0x20 */
void SceneNodeReleaseAnim(int obj)
{
     SceneNodeIterateChildren (*(int *)(obj + 0x20));
}
/* Copy packed vertex data then dispatch transition */
void SceneNodeSetAnimSeq(int *obj, int *data, int arg)
{
     func_8001d33c (data + 1, data + 1, *data << 3, obj[5] + 0x24);
    obj[10] = 0;
    obj[11] = 0;
    obj[12] = (int)data;
    (**(code **)(*obj + 0x30))(obj, arg);
    obj[12] = 0;
}
/* Empty function (stub) */
void NopSub_1d6a4(void) { }
/* Empty function (stub) */
void NopSub_1d6ac(void) { }
/* Dispatch by mode value: vtab+0xa0 for 2-3, store val for 4 */
void SceneNodeHandleEvent(int *obj, int val, int mode)
{
    if (mode > 1) {
        if (mode < 4) {
            (**(code **)(*obj + 0xa0))();
        } else if (mode == 4) {
            obj[10] = val;
        }
    }
}
/* Visibility check: compare bounding box against render list, call vtab dispatch */
void SceneNodeCollisionCheck(int *obj, int *other)
{
    int visible;
    int *posSrc, *posDst;
    int dx, dy, dz, absDx, absDy, absDz;
    int i;
    short local_88[3];
    short local_78[3];
    short tmp;
    if (obj[8] != 0) {
        visible =  func_8001d344 ();
        if (visible != 0) {
            posSrc = 0;
            if (other[3] != 0)
                posSrc = (int *)(other[5] + 0x38);
            posDst = 0;
            if (obj[3] != 0)
                posDst = (int *)(obj[5] + 0x38);
            dx = posSrc[0] - posDst[0];
            dy = posSrc[1] - posDst[1];
            dz = posSrc[2] - posDst[2];
            absDx = dx;
            if (dx < 0) absDx = -dx + 1;
            if (absDx < 0x4001) {
                absDy = dy;
                if (dy < 0) absDy = -dy + 1;
                if (absDy < 0x4001) {
                    absDz = dz;
                    if (dz < 0) absDz = -dz + 1;
                    if (absDz < 0x4001) {
                        local_88[0] = (short)dx;
                        local_88[1] = (short)dy;
                        local_88[2] = (short)dz;
                        local_78[0] = local_88[0];
                        local_78[1] = local_88[1];
                        local_78[2] = local_88[2];
                        i = *(int *)other[12];
                        (**(code **)(*obj + 0xa4))(obj, &local_78, other[12] + 4, i << 3);
                        i = (**(code **)(*obj + 0xa8))(obj, &i, &local_78);
                        if (i != 0) {
                            i = (**(code **)(*obj + 0xac))(obj, other + 11, &local_78, &i);
                            if (i != 0) {
                                obj[10] = (int)other;
                                (**(code **)(*other + 0x38))(other, obj, 4);
                            }
                        }
                    }
                }
            }
        }
    }
}
/* Transform vertices through matrix chain: multiply child matrices, apply */
void SceneNodeTransformPoints(int *obj, int dst, int src, int data, int count)
{
    int *child;
    MATRIX M1, M2;
    (**(code **)(*obj + 0x84))(obj, &M1, 1);
    for (child = (int *)obj[3]; child != 0; child = (int *)child[3]) {
        (**(code **)(*child + 0x84))(child, &M2, 1);
        MulMatrix2(&M2, &M1);
    }
     MathBitfieldOp (src, data, count, &M1);
    if (dst != 0)
         func_8001d374 (dst, dst, 1, &M1);
}
/* AABB overlap test between two primitives' vertex ranges */
int SceneNodeAABBOverlap(int obj, int *vertInfo, short *offset)
{
    int i;
    short minX, minY, minZ, maxX, maxY, maxZ;
    short vx, vy, vz;
    short *verts;
    /* Add offset to all vertices */
    vx = (short)((short)vertInfo[1] + *offset);
    vy = (short)(*(short *)((int)vertInfo + 6) + offset[1]);
    vz = (short)(*(short *)(vertInfo + 2) + offset[2]);
    minX = maxX = vx;
    minY = maxY = vy;
    minZ = maxZ = vz;
    for (i = 0; i < vertInfo[0]; i++) {
        int *vp = (int *)((int)vertInfo + 10 + i * 6);
        short px = *(short *)vp + *offset;
        short py = *(short *)((int)vp + 2) + offset[1];
        short pz = *(short *)((int)vp + 4) + offset[2];
        if (px < minX) minX = px;
        if (py < minY) minY = py;
        if (pz < minZ) minZ = pz;
        if (px > maxX) maxX = px;
        if (py > maxY) maxY = py;
        if (pz > maxZ) maxZ = pz;
    }
    /* Compare against visible-area bounding box */
    {
        short visMinX, visMinY, visMinZ, visMaxX, visMaxY, visMaxZ;
         MathBitfieldOp (*(int *)(obj + 0x20));
        /* Read visibility bounds from render state */
        {
            int state = *(int *)(obj + 0x20);
            int count =  func_8001d3cc (state, 0);
            int base =  MathBitfieldOp ();
            short *vb = (short *)(count + base * 12);
            short *vbEnd = (short *)(count + 12);
            visMinX = visMaxX = vb[0];
            visMinY = visMaxY = vb[1];
            visMinZ = visMaxZ = vb[2];
            while (vb < vbEnd) {
                /* This loop computes the visible range */
                short ax = vb[0], ay = vb[1], az = vb[2];
                if (ax < visMinX) visMinX = ax;
                if (ay < visMinY) visMinY = ay;
                if (az < visMinZ) visMinZ = az;
                if (ax > visMaxX) visMaxX = ax;
                if (ay > visMaxY) visMaxY = ay;
                if (az > visMaxZ) visMaxZ = az;
                vb += 3;
            }
        }
        /* AABB overlap test */
        if (minX > visMaxX || maxX < visMinX ||
            minY > visMaxY || maxY < visMinY ||
            minZ > visMaxZ || maxZ < visMinZ)
            return 0;
        return 1;
    }
}
/* Culling / intersection test with octree subdivision */
int SceneNodeQueryRegion(int obj, int *result, int arg, int *bounds)
{
    int i, n;
    short mid[6];
    int *p9, *p10;
    int local_70;
    int local_6c;
    int *local_68;
    int local_50;
    int local_48;
    local_70 = 0x7fffffff;
    local_68 = result;
    *(int *)(obj + 0x2c) = 0;
    p9 = bounds + 1;
    p10 = bounds + 4;
    for (i = 0; i < 2 && i < bounds[0]; i++) {
        mid[i*3] = (short)(((int)*(short *)&p9[i*6] + (int)*(short *)&p10[i*6]) >> 1);
        mid[i*3+1] = (short)(((int)*(short *)((int)&bounds[i*6] + 2) + (int)*(short *)((int)&bounds[i*6] + 8)) >> 1);
        mid[i*3+2] = (short)(((int)*(short *)&bounds[i*6+2] + (int)*(short *)&bounds[i*6+4]) >> 1);
    }
    local_50 =  func_8001d3a0 ();
    if (local_50 > 0) {
        for (i = 0; i < local_50; i++) {
            int state =  MathBitfieldOp (*(int *)(obj + 0x20), i);
            int hit =  func_8001d4dc (0, (short *)state, &mid[0], &mid[3]);
            if (hit != 0) {
                hit =  MathBitfieldOp (*(int *)(obj + 0x20), &local_70, arg, &local_6c, &mid[0], &mid[3]);
                if (hit != 0 && (Primitive_CollideFlag == 0 || local_6c > 0x200)) {
                    *(int *)(obj + 0x2c) |= 1 << (i & 0x1f);
                }
            }
        }
    }
    if (*(int *)(obj + 0x2c) == 0) {
        *local_68 = 0;
        local_48 = *bounds;
        for (i = 0; i < local_50; i++) {
            int tile = 0;
            int *vp = bounds + 1;
            int state =  func_8001d568 (*(int *)(obj + 0x20), i);
            for (n = 0; n < local_48; n++) {
                int k;
                int *tileBase = vp;
                for (k = 0; k < 4; k++) {
                    int *tri = vp;
                    int idx = k - 1;
                    if (idx >= 0 && idx < 2) {
                        int h =  MathBitfieldOp (0, (short *)state, (short *)tri, (short *)(tri + 3));
                        if (h != 0) {
                            h =  func_8001d568 (*(int *)(obj + 0x20), &local_70, arg, &local_6c, (short *)tri, (short *)(tri + 3));
                            if (h != 0 && local_6c > 0x200) {
                                *(int *)(obj + 0x2c) |= 1 << (i & 0x1f);
                                *local_68 |= 1 << (tile & 0x1f);
                            }
                        }
                    }
                    vp = tri + 3;
                }
                tile++;
                vp = (int *)((int)tileBase + 0x1e);
            }
        }
        return *local_68 != 0;
    }
    *local_68 = 1;
    return 1;
}
/* Recursive BSP/subdivide: check two points against bounding box, split if needed */
int MathBinarySubdivide(int ctx, short *bounds, short *p1, short *p2)
{
    int ret;
    short mid[3];
    uint b1, b2;
    b1 =  MathBitfieldOp (bounds, p1);
    b2 =  func_8001d3cc (bounds, p2);
    if (b1 == 0) {
        if (b2 == 0) return 1;
    } else if (b2 == 0) {
        if (ctx != 0)
             MathBitfieldOp ((short *)ctx, bounds, p2, p1);
        return 3;
    }
    if (b1 == 0) {
        if (ctx != 0)
             func_8001d3f8 ((short *)ctx, bounds, p1, p2);
        return 2;
    }
    if (b1 & b2) return 0;
    mid[0] = (short)(((int)p1[0] + (int)p2[0]) >> 1);
    mid[1] = (short)(((int)p1[1] + (int)p2[1]) >> 1);
    mid[2] = (short)(((int)p1[2] + (int)p2[2]) >> 1);
    if ((p1[0] == mid[0] && p1[1] == mid[1] && p1[2] == mid[2]) ||
        (p2[0] == mid[0] && p2[1] == mid[1] && p2[2] == mid[2]))
        return 0;
    ret =  MathBitfieldOp (ctx, bounds, p1, mid);
    if (ret == 0)
        ret =  func_8001d424 (ctx, bounds, mid, p2);
    return ret;
}
/* Clip triangle against bounding box: write subdivided result */
void MathClipEdge(short *out, short *bounds, short *p1, short *p2)
{
    short mid[3];
    int flags;
    while (1) {
        out[0] = (short)(((int)p1[0] + (int)p2[0]) >> 1);
        out[1] = (short)(((int)p1[1] + (int)p2[1]) >> 1);
        out[2] = (short)(((int)p1[2] + (int)p2[2]) >> 1);
        if ((out[0] == p1[0] && out[1] == p1[1] && out[2] == p1[2]) ||
            (out[0] == p2[0] && out[1] == p2[1] && out[2] == p2[2]))
            break;
        flags = 0;
        if (bounds[3] < out[0]) flags = 8;
        else if (out[0] < bounds[0]) flags = 4;
        if (bounds[4] < out[1]) flags |= 2;
        else if (out[1] < bounds[1]) flags |= 1;
        if (bounds[5] < out[2]) flags |= 0x20;
        else if (out[2] < bounds[2]) flags |= 0x10;
        if (flags != 0) {
            p2 = (short *)&flags;
            p1 = out;
        }
    }
}
/* Empty function (stub) */
void NopSub_1e49c(void) { }
/* Iterate children of type 4, call vtab+0x10 on each */
void ObjectProcessType4(int obj, int handle)
{
    int *child;
    int more;
    child = 0;
    do {
        BasicClass__func_1816c(handle, &child, &more);
        if (child != 0 && (*(int *)*child & 0xf) == 4) {
            (**(code **)(*(int *)*child + 0x10))(child, obj);
        }
    } while (more != 0);
}
/* Return pointer to base vtable data */
int GetPrimitiveBaseTablePtr(void)
{
    return (int)Primitive_BaseTable;
}
/* Transform vertex through vtab+0x84, apply matrix */
void MathApplyWorldTransform(int *obj, int *dst, short *src)
{
    int local[8];
    (**(code **)(*obj + 0x84))(obj, local, 0);
    dst[0] = (int)src[0];
    dst[1] = (int)src[1];
    dst[2] = (int)src[2];
     func_8001d450 (dst, dst, 1, local);
}
/* Transform vertex, then add parent position offset */
void MathTransformByMatrix(int *obj, int *dst, int src)
{
    int *parentPos;
    int local[8];
    (**(code **)(*obj + 0x84))(obj, local, 0);
     GpuGetReady (dst, src, 1, local);
    parentPos = 0;
    if (obj[3] != 0) parentPos = (int *)(obj[5] + 0x38);
    dst[0] += parentPos[0];
    if (obj[3] != 0) parentPos = (int *)(obj[5] + 0x38);
    dst[1] += parentPos[1];
    if (obj[3] != 0) parentPos = (int *)(obj[5] + 0x38);
    dst[2] += parentPos[2];
}
/* Link object data, copy position, call GsLinkObject4 */
void GsLinkObject2(int obj, int data)
{
    *(int *)(obj + 0x20) = data;
    *(int *)(obj + 0x18) = *(int *)(data + 0x10);
    GsLinkObject4(*(int *)(*(int *)(obj + 0x20) + 0xc) + 0xc, (GsDOBJ2 *)(obj + 0x10), 0);
}
/* Clear linked object data */
void GsUnlinkObject2(int obj)
{
    *(int *)(obj + 0x18) = 0;
    *(int *)(obj + 0x20) = 0;
}
/* Project a point into screen space, compute camera-relative position */
int SceneNodeAimAt(int *obj, int arg, short *pos)
{
    short local_40, local_3e;
    short local_38, local_36, local_34;
    int iVar8;
    short *psVar7;
    if (obj[8] == 0) goto fail;
    if (obj[4] >= 0) goto calc;
    if (obj[3] != 0) {
        int base = obj[5];
        if (base != -0x38) {
            /* Copy position */
            *(int *)(base + 0x38) = *(int *)(base + 0x18);
            *(int *)(base + 0x3c) = *(int *)(base + 0x1c);
            *(int *)(base + 0x40) = *(int *)(base + 0x20);
            /* Accumulate child offsets */
            for (int child = obj[3]; child != 0; child = *(int *)(child + 0xc)) {
                *(int *)(base + 0x38) += *(int *)(*(int *)(child + 0x14) + 0x18);
                *(int *)(base + 0x3c) += *(int *)(*(int *)(child + 0x14) + 0x1c);
                *(int *)(base + 0x40) += *(int *)(*(int *)(child + 0x14) + 0x20);
            }
        }
    }
    goto calc_psVar;
calc:
calc_psVar:
    if (obj[3] == 0)
        psVar7 = 0;
    else
        psVar7 = (short *)(obj[5] + 0x38);
    local_38 = *pos - *psVar7;
    local_36 = pos[2] - psVar7[2];
    local_34 = pos[4] - psVar7[4];
    (**(code **)(*obj + 0xa4))(obj, 0, &local_40, &local_38, 1);
    local_38 = local_40;
    local_34 = local_3e - 0x400;
    local_36 = *(short *)((int)&local_40 + 2) - 0x400;
    iVar8 =  GpuCopyAABB (obj[8], 0, 0, 0, &local_40, &local_38);
    if (iVar8 == 0) {
        local_36 = *(short *)((int)&local_40 + 2) + 0x400;
        iVar8 =  func_8001d480 (obj[8], 0, 0, 0, &local_40, &local_38);
        if (iVar8 == 0) goto fail;
    }
     GpuCopyAABB (arg, &local_40, &local_38);
    return 1;
fail:
    return 0;
}
/* Compute vector difference from two 3D points */
void MathVectorSub(int *out, short *a, short *b)
{
    out[0] = (int)b[0] - (int)a[0];
    out[1] = (int)b[1] - (int)a[1];
    out[2] = (int)b[2] - (int)a[2];
}
/* Aim/rotate toward target: compute yaw/pitch from position delta */
void MathComputeAngleToTarget(int *obj, int target, int lockYaw, int flipPitch, int extraAngles)
{
    int base;
    int *targetPos;
    long angle;
    short angles[3];
    short pitch;
    base = obj[5];
    if (*(int *)(target + 0xc) == 0)
        targetPos = 0;
    else
        targetPos = (int *)(*(int *)(target + 0x14) + 0x38);
    angle = ratan2(targetPos[0] - *(int *)(base + 0x18), targetPos[2] - *(int *)(base + 0x20));
    pitch = (short)angle;
    angle = ratan2(targetPos[2] - *(int *)(base + 0x20), targetPos[1] - *(int *)(base + 0x1c));
    angles[0] = (short)(((short)angle + 0x400) * 0x168 >> 0xc);
    angles[1] = 1;
    angles[2] = 0;
    angles[3] = 1;
    angles[4] = (short)(pitch * 0x168 >> 0xc);
    angles[5] = 1;
    if (lockYaw != 0) angles[0] = 0;
    if (flipPitch == 0) angles[4] += 0xb4;
    (**(code **)(*obj + 0x44))(obj, 1, angles);
    if (extraAngles != 0)
        (**(code **)(*obj + 0x44))(obj, 0, extraAngles);
}
/* Divide a fixed-point value: (a / b) * 0x1000, returns 4.12 result */
int MathFixedDiv(short *in)
{
    int num, den;
    num = (int)in[0];
    den = (int)in[1];
    if (den == 0) trap(0x1c00);
    if (den == -1 && num == -0x80000000) trap(0x1800);
    return (num / den) * 0x1000 + (num % den << 0xc) / den;
}
/* Classify point against bounding box: returns 6-bit outcode (Cohen-Sutherland) */
int MathAABBCode(short *box, short *pt)
{
    int code = 0;
    if (box[3] < pt[0]) code = 8;
    else if (pt[0] < box[0]) code = 4;
    if (box[4] < pt[1]) code |= 2;
    else if (pt[1] < box[1]) code |= 1;
    if (box[5] < pt[2]) code |= 0x20;
    else if (pt[2] < box[2]) code |= 0x10;
    return code;
}
/* Bitfield read/write: read/write N bits at bit position, return old value */
int MathBitfieldOp(int *field, int bit, int width, int value)
{
    int mask = (1 << width) - 1;
    int shift = bit & 0x1f;
    int uMask = mask << shift;
    int old = *field;
    *field = (value << shift) | (~uMask & old);
    return (uMask & old) >> shift;
}
/* Transform vertex array through matrix (ApplyMatrixSV for each vertex) */
void MathProjectVectorSV(int dst, int src, int count, MATRIX *mat)
{
    int i;
    short *s = (short *)src;
    short *d = (short *)dst;
    for (i = 0; i < count; i++) {
        ApplyMatrixSV(mat, s, d);
        s += 3;
        d += 3;
    }
}
/* Transform VECTOR array through matrix (ApplyMatrixLV for each) */
void MathTransformMultipleVectors(VECTOR *dst, VECTOR *src, int count, MATRIX *mat)
{
    int i;
    for (i = 0; i < count; i++) {
        ApplyMatrixLV(mat, src, dst);
        src = (VECTOR *)&src->pad;
        dst = (VECTOR *)&dst->pad;
    }
}
/* Bounding-box containment test: check if point is within box extended by margin */
int MathIsPointInBox(int *box, int margin, int *pt)
{
    int i;
    for (i = 0; i < 3; i++) {
        if (pt[i] < box[i] - margin) return 0;
        if (pt[i] > box[i] + margin) return 0;
    }
    return 1;
}
/* Getter/setter for Other_GlobalPtr (D_8008A838) */
int SetOtherGlobalPtr(int param_1)
{
    int old = Other_GlobalPtr;
    Other_GlobalPtr = param_1;
    return old;
}
void MathReadScaledShorts(int obj, short *out)
{
    int subObj;
    subObj = *(int *)(*(int *)(obj + 0x14) + 0x44);
    out[1] = 1;
    out[0] = (short)(*(short *)(subObj + 0x10) * 0x2d >> 9);
    out[3] = 1;
    out[2] = (short)(*(short *)(subObj + 0x12) * 0x2d >> 9);
    out[5] = 1;
    out[4] = (short)(*(short *)(subObj + 0x14) * 0x2d >> 9);
}
void SceneNodeFindChild(int obj, int *a, int *b)
{
    while (1) {
        if (*a == 0) {
            *b = *(int *)(obj + 4);
        }
         func_8001d4ac (a, b);
        if (*a != 0 && (*(int *)*a & 0xf) == 4 && *(int *)(*a + 0xc) == obj)
            break;
        if (*b == 0) {
            *a = 0;
            return;
        }
    }
}

/* ======================================================================== */
/* orphan_stubs.c redistributed functions                                    */
/* ======================================================================== */
/* Returns pointer to the singleton GameManager object */
void *GetGameManagerPtr(void)
{
    return &GameManagerPtr;
}
/* Check if N-dimensional values[] are within range +/- offset of range[] */
int RangeCompare(int *range, int offset, int *values)
{
    int i;
    i = 0;
    while (1) {
        if (*values < *range - offset)
            return 0;
        if (offset + *range < *values) break;
        i++;
        range++;
        values++;
        if (i > 2)
            return 1;
    }
    return 0;
}
/* Read entity position as 3 scaled (x,y,z) short pairs from dream state */
void ReadDreamSysVector(int unused, short *outVec)
{
    short s;
    int vec;
    vec = (**(code **)(*(u32 *)Other_DreamSysStatePtr + 0x200))();
    s = *(short *)(vec + 0x10);
    outVec[1] = 1;
    outVec[0] = (short)(s * 0x2d >> 9);
    s = *(short *)(vec + 0x12);
    outVec[3] = 1;
    outVec[2] = (short)(s * 0x2d >> 9);
    s = *(short *)(vec + 0x14);
    outVec[5] = 1;
    outVec[4] = (short)(s * 0x2d >> 9);
}
/* Walk dream state tree, validate and invoke state action callbacks */
int DreamStateTraverse(int id, int arg, int state, int *obj)
{
    int i;
    int result;
    char *pc;
    char localBuf[12];
    int local_24;
    i = DreamStateValidate(id, state);
    result = 0;
    if (i != 0) {
        pc = (char *)(state + 4);
        DreamCheckTeleport(*(char *)(state + 3));
        local_24 = (**(code **)(*obj + 0x88))(obj, (int)*(char *)(state + 2), 0, 0);
        if (local_24 == 0) {
fallback:
            result = 0;
            if (*(char *)(state + 3) == '\x02') {
                result = DreamStateTraverse(id, arg, state + 0x38, obj);
            }
        } else {
            do {
                if (((char *)(state + 8) <= pc) || (*pc == -1))
                    goto fallback;
                i = DreamStubReturnZero(*(char *)(state + 3), localBuf, arg, *pc);
                pc++;
            } while (i == 0);
            result = 1;
        }
    }
    return result;
}
/* Validate state ID against state descriptor, mark valid states */
int DreamStateValidate(uint id, char *state)
{
    uint u;
    u = (uint)state[1];
    if (u == 1) goto valid_state;
    if ((int)u < 0) {
        if (*state != '\0')
            return 0;
        u = ~u + 1;
    }
    switch (u) {
    case 2:
    case 3:
    case 4:
        id = DreamStateRangeCheck(id, u - 1);
        break;
    case 5:
        if (id != ((int)id / 3) * 3)
            return 0;
        goto valid_state;
    case 6:
        if (id == ((int)id / 3) * 3)
            return 0;
        goto valid_state;
    case 7:
        id = GetParticleModeBit();
        break;
    case 8:
    case 9:
        if ((int)id % 3 != u - 7)
            return 0;
        goto valid_state;
    default:
        if ((int)u < 10) goto valid_state;
        id = DreamStateCheckEq(u);
        break;
    case 0x14:
        if ((id & 1) != 0)
            return 0;
        goto valid_state;
    case 0x15:
        id = id & 1;
    }
    if (id == 0)
        return 0;
valid_state:
    *state = '\x01';
    return 1;
}
/* Check if dream state value at index matches current dream state */
bool DreamStateCheckEq(int index)
{
    char c;
    int vec;
    c = (&Other_DreamStateMap)[index];
    vec = (**(code **)(*(u32 *)Other_DreamSysStatePtr + 0x200))();
    return c == vec;
}
/* Range-check state value in groups of 30, with step of 3 */
int DreamStateRangeCheck(int val, int base)
{
    int i;
    i = 0;
    do {
        if ((val - 1) / 30 + 1 == base)
            return 1;
        i++;
        base += 3;
    } while (i < 4);
    return 0;
}
/* Initialize array controller with entry pool and sizes */
int ArrayCtrlInit(int id, int *ctrl, int max, int entrySize, int cdItemSize)
{
    int result;
    int i;
    int *pi;
    result = 0;
    if (*ctrl == 0) {
        pi = ctrl + 6;
        i = (**(code **)(*(u32 *)Other_DreamSysStatePtr + 0x200))();
        *ctrl = max;
        ctrl[2] = entrySize;
        ctrl[3] = cdItemSize;
        do {
            *pi = -1;
            i = (**(code **)(*(u32 *)Other_DreamSysStatePtr + 0x200))();
            pi += 5;
        } while (i >= 0);
        result = 1;
        ctrl[1] = 0;
        ctrl[5] = 10;
    }
    return result;
}
/* Process array controller: iterate entries and trigger callbacks */
void ArrayCtrlProcess(int *ctrl, int *out)
{
    int i;
    int *pi;
    int j;
    pi = (int *)(**(code **)(*(u32 *)Other_DreamSysStatePtr + 0x200))();
    j = 0;
    do {
        j++;
        if (*pi >= 0) {
            i = (**(code **)(*ctrl + 0x84))(ctrl, 0, 0, 0);
            *pi = (**(code **)(*(u32 *)Other_DreamSysStatePtr + 0x200))();
        }
        pi = (int *)(**(code **)(*(u32 *)Other_DreamSysStatePtr + 0x200))();
    } while (j < 3);
    *out = 0;
}
/* Process array controller with CD-ROM data chunks */
void ArrayCtrlProcessCd(int *ctrl, int *out)
{
    int i;
    int *pi;
    int j;
    int k;
    int l;
    int *pj;
    if (*out > 0) {
        l = 0;
        pi = (int *)(**(code **)(*(u32 *)Other_DreamSysStatePtr + 0x200))();
        do {
            l++;
            pi[-3] = -1;
            pi[-2] = 0;
            pi[-1] = 0x7f;
            *pi = (**(code **)(*(u32 *)Other_DreamSysStatePtr + 0x200))();
            pi = (int *)(**(code **)(*(u32 *)Other_DreamSysStatePtr + 0x200))();
        } while (l < 3);
        out[4] = 0;
        if (out[3] != 0) {
            (**(code **)out[3])(out[2], out, 0, 0);
        }
        if (out[4] >= 0) {
            pj = out + 6;
            l = 0;
            pi = (int *)(**(code **)(*(u32 *)Other_DreamSysStatePtr + 0x200))();
            do {
                if (pi[-3] < 0) {
                    if ((pi[-3] == -2) && (*pj >= 0)) {
                        (**(code **)(*ctrl + 0x84))(ctrl, 0, 0, 0);
                    }
                } else {
                    if (*pj >= 0) {
                        (**(code **)(*ctrl + 0x84))(ctrl, 0, 0, 0);
                    }
                    (**(code **)(*ctrl + 0x9c))(ctrl, pi[-2], 0, 0);
                    k = pi[-1];
                    i = out[5];
                    j = *pi;
                    i = (**(code **)(*ctrl + 0x80))(ctrl, pi[-3] << 4,
                        k - (k / i) * out[4], j - (j / i) * out[4]);
                    *pj = i;
                }
                l++;
                pi += 5;
                pj += 5;
            } while (l < 3);
        }
        out[1]++;
    }
}
/* Add offset to sound effect position (relative mode) */
void SoundOffsetAdd(int sound, int offset)
{
    SoundOffsetApply(sound, 0, (int *)offset);
}
/* Stub: always returns 0, suppresses unused warnings */
int DreamStubReturnZero(char a, char *b, int c, char d)
{
    (void)a; (void)b; (void)c; (void)d;
    return 0;
}
/* Apply position offset to sound entity's transform */
void SoundOffsetApply(int sound, int mode, int *offset)
{
    int i = *(int *)(sound + 0x14);
    if (mode == 0) {
        *(int *)(i + 0x18) += *offset;
        *(int *)(i + 0x1c) += offset[1];
        *(int *)(i + 0x20) += offset[2];
    } else {
        *(int *)(i + 0x18) = *offset;
        *(int *)(i + 0x1c) = offset[1];
        *(int *)(i + 0x20) = offset[2];
    }
    **(int **)(sound + 0x14) = 0;
}
/* Initialize dream state fields to defaults via vtable callbacks */
void DreamSys__func_588ec(int *state)
{
    (**(code **)(*state + 0x60))(state, 0, 0, 0);
    (**(code **)(*state + 0x44))(state, 1, &DreamDefaultVisual, 0);
    state[0x20] = 0;
    state[0x26] = 0;
    state[0x33] = 0;
    state[0x242] = 0;
    state[0x243] = 0;
    state[0x244] = 0;
    state[0x1e] = 0;
    state[0x249] = 0;
}

/* === migrated from stubs.c: GetGameManager === */
void *GetGameManager(void) { return (void *)GameManagerPtr; }
/* === migrated from stubs.c: strlen_imp === */
unsigned int strlen_imp(const char *s) { return strlen(s); }
/* === migrated from stubs.c: IsTileActive === */
int IsTileActive(void) { return 1; }
/* === migrated from stubs.c: GetRandomValue === */
int GetRandomValue(void) { return (int)rand(); }
/* === migrated from stubs.c: GetVideoMode === */
int GetVideoMode(void) { return 0; }
/* === migrated from stubs.c: delete === */
int delete(const char *path) { return (int)erase((char *)path); }
/* === migrated from stubs.c: PsyqSpuMalloc === */
void *PsyqSpuMalloc(int size) { return malloc((unsigned int)size); }
/* === migrated from stubs.c: LeaveCriticalSection === */
void LeaveCriticalSection(void)
{
    __asm__ volatile(
        "mfc0 $t0, $12\n"
        "ori  $t0, $t0, 0x0001\n"
        "mtc0 $t0, $12\n"
        : : : "t0", "memory"
    );
}
/* === migrated from stubs.c: EnterCriticalSection === */
void EnterCriticalSection(void)
{
    __asm__ volatile(
        "mfc0 $t0, $12\n"
        "andi $t0, $t0, 0xFFFE\n"
        "mtc0 $t0, $12\n"
        : : : "t0", "memory"
    );
}
/* === ExitCriticalSection — re-enable interrupts === */
void ExitCriticalSection(void)
{
    __asm__ volatile(
        "mfc0 $t0, $12\n"
        "ori  $t0, $t0, 0x0001\n"
        "mtc0 $t0, $12\n"
        : : : "t0", "memory"
    );
}
/* === migrated from stubs.c: WaitForMemoryCardEvent === */
int WaitForMemoryCardEvent(int slot) { (void)slot; return 1; }
/* === migrated from stubs.c: GetGameManagerAlias === */
void *GetGameManagerAlias(void) { return GetGameManager(); }
/* === migrated from stubs.c: MemAlloc_SetHeap === */
void MemAlloc_SetHeap(int heapBase) { Other_MemHeapPtr = heapBase; }
/* === migrated from stubs.c: MemAllocImpl === */
void *MemAllocImpl(int size) { return MemAlloc(size, 0); }
/* === migrated from stubs.c: GetCoordSystemVtable === */
void *GetCoordSystemVtable(void) { return GetVtable_CoordSystem(); }
/* === migrated from stubs.c: MemFreeImpl === */
void MemFreeImpl(int ptr) { MemFree(ptr); }
/* === migrated from stubs.c: System_HeapInit === */
void System_HeapInit(void) { MemAlloc_SetHeap((int)0x800C82FC); }
/* === migrated from stubs.c: GetDebugInputVtable === */
void *GetDebugInputVtable(void) { return &Other_DebugInputVtable; }
/* === migrated from stubs.c: FormatIntToString === */
void FormatIntToString(char *dst, int val, int pad, int mode) {
    char buf[20];
    if (mode == 0)
        sprintf(buf, "%0*d", pad, val);
    else
        sprintf(buf, "%d", val);
    strcpy(dst, buf);
}

/* === migrated from stubs.c === */
/* GetStageDataTable */
void *GetStageDataTable(int *outSize)
{
    static int stageDataCreated;
    static char stageData[0x4000];
    if (outSize) *outSize = 0x4000;
    if (!stageDataCreated) {
        memset(stageData, 0, sizeof(stageData));
        stageDataCreated = 1;
    }
    return (void *)&stageData;
}
/* BMemPMgrInit */
void *BMemPMgrInit(unsigned int poolSize, int a1)
{
    (void)a1;
    if (poolSize > 0x130000) poolSize = 0x130000;  /* cap to fit within 2MB RAM (ends at 0x801FFFFF) */
    *(volatile u32 *)0x800BAFF8 = poolSize;  /* debug: write poolSize to D2 */
    /* Set up heap at 0x800C82FC in format expected by MemAlloc (Other.c) */
    int *heapBase = (int *)0x800C82FC;
    int *firstBlock = (int *)0x800C830C;  /* leave 16 bytes for heap header */
    *(volatile u32 *)0x800BAFEC = (u32)firstBlock;  /* debug: D3 = firstBlock addr */
    heapBase[0] = 0;  /* unused */
    heapBase[1] = 0;  /* unused */
    heapBase[2] = (int)firstBlock;  /* free list head at heapBase+8 */
    *(volatile u32 *)0x800BAFF0 = heapBase[2];  /* debug: D0 = heapBase[2] after assign */
    firstBlock[0] = (poolSize - 16) | 0x40000000;  /* block header: size | free flag */
    *(volatile u32 *)0x800BAFF4 = firstBlock[0];  /* debug: D1 = firstBlock[0] after assign */
    firstBlock[1] = 0;  /* next free block */
    firstBlock[2] = 0;  /* prev free block */
    /* end marker */
    int *end = (int *)((int)firstBlock + (firstBlock[0] & 0xfffffff));
    end[-1] = (int)firstBlock;
    end[0] = 0x80000000;
    return heapBase;
}
/* LinkedListPopFront */
void LinkedListPopFront(void **out, int **list)
{
    if (*list == 0) {
        *out = 0;
    } else {
        *out = (void *)(*list)[1];
        *list = (int *)**list;
    }
}
/* ArrayInitByVtable */
void ArrayInitByVtable(void **array, int count)
{
    for (; 0 < count; count--) {
        *array = 0;
        array++;
    }
}
/* RandomRange */
void RandomRange(unsigned int *buf, int min, int max)
{
    if (!buf) return;
    if (max > min)
        buf[0] = ((unsigned int)rand() % (unsigned int)(max - min + 1)) + (unsigned int)min;
    else
        buf[0] = (unsigned int)min;
}
/* BSPClassifyPoint */
unsigned char BSPClassifyPoint(short *box, short *point) {
    unsigned char result = 0;
    if (point[0] > box[3]) result = 8;
    else if (point[0] < box[0]) result = 4;
    if (point[1] > box[4]) result |= 2;
    else if (point[1] < box[1]) result |= 1;
    if (point[2] > box[5]) result |= 0x20;
    else if (point[2] < box[2]) result |= 0x10;
    return result;
}

