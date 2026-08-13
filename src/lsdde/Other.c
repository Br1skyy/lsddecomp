#include "common.h"
#include "dat_globals.h"
#include "libgte.h"
#include "libgpu.h"
#include "libgs.h"
#include "lsdde/structs.h"
extern u32 GteTransformTest(int ctx, int *out);
extern u32 Other_RenderDitherFlag;
extern u32 Other_SoundRotScaleTbl;
extern u32 Other_SoundEntityParam2;
u32 LookupSoundType(short *entry);
/* WARNING: Removing unreachable block (ram,0x800123dc) */
/* WARNING: Removing unreachable block (ram,0x80012474) */
/* Draw a 2D sprite/billboard/UI element through the scene graph */
/* Handles type 'T' (text), type 'd' (100 = box fill), type 'D' (sprite), */
/* and default 3D coordinate transforms with lighting */
void DrawSceneGraphSprite(int renderCtx, u32 *node)
{
    bool hasTransform;
    int *coordPtr;
    MATRIX *coordMtx;
    MATRIX tempRot;
    MATRIX tempLight;
    MATRIX tempM;
    u32 *iterator;
    int nodeId;
    uint nodeType;
    s32 localX, localY, localZ;
    s32 localZ2;
    GsOT *ot;
    ushort pri;
    hasTransform = false;
    if ((*(char *)*node == '$') && ((int)node[4] < 0)) {
        return;
    }
    coordPtr = (int *)node[5];
    coordMtx = (MATRIX *)(coordPtr + 1);
    if (*coordPtr == 0) {
        int *rotData = (int *)coordPtr[0x11];
        hasTransform = true;
        RotMatrix((SVECTOR *)(rotData + 4), coordMtx);
        for (; coordMtx < (MATRIX *)((int)coordPtr + 0x16); coordMtx = (MATRIX *)(coordMtx->m + 1)) {
            coordMtx->m[0][0] = (short)((uint)((int)coordMtx->m[0][0] * *rotData) >> 0xc);
            short *ps = coordMtx->m[0] + 1;
            *ps = (short)((uint)((int)*ps * rotData[1]) >> 0xc);
            ps = coordMtx->m[0] + 2;
            *ps = (short)((uint)((int)*ps * rotData[2]) >> 0xc);
        }
    }
    iterator = NULL;
find_node:
    do {
        if (iterator == (u32 *)0x0) {
            nodeId = node[1];
        }
        ListIterateNext(&iterator, &nodeId);
        if (((iterator == (u32 *)0x0) || ((*iterator & 0xf) != 4)) || (iterator[3] != (u32)node)) {
            if (nodeId != 0) goto find_node;
            iterator = (u32 *)0x0;
        }
        if (iterator != (u32 *)0x0) {
            if (hasTransform) {
                iterator[5] = 0;
            }
            DrawSceneGraphSprite(renderCtx, iterator);
        }
    } while (nodeId != 0);
    nodeType = *node & 0xff;
    if (nodeType == 0x54) {
        /* Type 'T' - Background */
        GsSortBg((GsBG *)(node + 0x11),
                 *(GsOT **)(*(int *)(renderCtx + 0x74) * 4 + renderCtx + 0x78),
                 (short)(1 << (*(uint *)(renderCtx + 0x3c) & 0x1f)) - 1);
        return;
    }
    if (nodeType == 100) {
        /* Type 'd' - Box fill */
        if (node[0x12] == 0) {
            *(u16 *)(node + 0x17) = *(u16 *)(node + 0x14);
            *(u16 *)((int)node + 0x5e) = *(u16 *)(node + 0x15);
        } else {
            *(short *)(node + 0x17) = (short)(((*(int *)(renderCtx + 0x34) >> 1) * node[0x14]) / 100);
            *(short *)((int)node + 0x5e) = (short)(((*(int *)(renderCtx + 0x38) >> 1) * node[0x15]) / 100);
        }
        GsSortBoxFill((GsBOXF *)(node + 0x16),
                      *(GsOT **)(*(int *)(renderCtx + 0x74) * 4 + renderCtx + 0x78),
                      *(ushort *)(node + 0x11));
        return;
    }
    if (nodeType != 0x44) {
        /* Default 3D sprite with lighting */
        GsGetLws((GsCOORDINATE2 *)node[5], &tempLight, &tempM);
        GsSetLightMatrix(&tempLight);
        GsSetLsMatrix(&tempM);
        if (node[6] == 0) {
            return;
        }
        SubmitGeometryPrimitives(node + 4, *(u32 *)(*(int *)(renderCtx + 0x74) * 4 + renderCtx + 0x78),
                      0xe - *(int *)(renderCtx + 0x3c), (u32 *)REG_GTE_DMA);
        return;
    }
    /* Type 'D' - Billboard sprite */
    if ((*node & 0xfff) == 0x144) {
        int w = node[0x28];
        if (w == 0) {
            *(u16 *)(node + 0x1a) = 0;
        } else {
            int divW = 10000 / w;
            int halfW = (*(int *)(renderCtx + 0x34) >> 1) * 100;
            *(short *)(node + 0x1a) = (short)(halfW / divW);
        }
        int h = node[0x29];
        if (h == 0) {
            *(u16 *)((int)node + 0x6a) = 0;
        } else {
            int divH = 10000 / h;
            int halfH = (*(int *)(renderCtx + 0x38) >> 1) * 100;
            *(short *)((int)node + 0x6a) = (short)(halfH / divH);
        }
        *(short *)(node + 0x1a) = *(short *)(node + 0x1a) + *(short *)(node + 0x1f);
        *(short *)((int)node + 0x6a) = *(short *)((int)node + 0x6a) + *(short *)((int)node + 0x7e);
        ot = *(GsOT **)(*(int *)(renderCtx + 0x74) * 4 + renderCtx + 0x78);
        pri = 0;
        GsSortSprite((GsSPRITE *)(node + 0x19), ot, pri);
        return;
    }
    /* 3D sprite with depth sorting */
    GsGetLs((GsCOORDINATE2 *)node[5], &tempM);
    if (CLIP_NEAR < tempM.t[2] - 1U) {
        return;
    }
    if (*(int *)(node[3] + 0xc) == 0) {
        localX = tempM.t[0];
        localY = tempM.t[1];
        localZ = tempM.t[2];
    } else {
        ApplyRawMatrixToVectors((VECTOR *)&localX, (VECTOR *)(node[5] + 0x18), 1, &tempM);
        localX += tempM.t[0];
        localY += tempM.t[1];
        localZ += tempM.t[2];
    }
    if (localZ <= *(int *)(renderCtx + 0x4c)) {
        return;
    }
    int halfW = localX * *(int *)(renderCtx + 0x40);
    localX = halfW / localZ;
    int halfH = localY * *(int *)(renderCtx + 0x40);
    localY = halfH / localZ;
    int zRange = *(int *)(renderCtx + 0x98);
    int depth = localZ - *(int *)(renderCtx + 0x4c);
    localZ = depth / zRange;
    if ((int)localX < 0) {
        if ((int)(~localX + 1) > CLIP_LEFT) goto CLAMP_X;
        goto SET_X;
    } else {
        if ((int)localX < CLIP_TOP) goto SET_X;
CLAMP_X:
        *(u16 *)(node + 0x1a) = CLIP_LEFT;
        goto CHECK_Y;
SET_X:
        *(u16 *)(node + 0x1a) = (u16)localX;
    }
CHECK_Y:
    if ((int)localY < 0) {
        if ((int)(~localY + 1) > CLIP_LEFT) goto CLAMP_Y;
        goto SET_Y;
    } else {
        if ((int)localY < CLIP_TOP) goto SET_Y;
CLAMP_Y:
        *(u16 *)((int)node + 0x6a) = CLIP_LEFT;
        goto SORT_SPRITE;
SET_Y:
        *(u16 *)((int)node + 0x6a) = (u16)localY;
    }
SORT_SPRITE:
    ot = *(GsOT **)(*(int *)(renderCtx + 0x74) * 4 + renderCtx + 0x78);
    pri = (ushort)localZ;
    GsSortSprite((GsSPRITE *)(node + 0x19), ot, pri);
}
/* Memory manager global setter */
void MemManagerSetGlobal(void *ptr)
{
    Other_MemHeapPtr = (int)ptr;
}
/* Memory manager initialization */
void MemManagerInit(u32 *heap)
{
    u32 *block;
    u32 *header;
    if (Other_MemHeapPtr == 0) {
        heap = (u32 *)Other_MemHeapPtr; /* use the stored global */
    }
    heap[4] = 1;
    block = (u32 *)*heap;
    heap[2] = (int)block;
    heap[3] = (int)block;
    *block = heap[1] | 0x40000000;
    block[1] = 0;
    block[2] = 0;
    header = (u32 *)((int)block + (*block & 0xfffffff));
    header[-1] = (int)block;
    *header = 0x80000000;
}
/* Memory allocator */
/* Returns pointer to allocated memory (user region starts at returned+4) */
u32 *MemAlloc(u32 size, int heapFallback)
{
    u32 *result;
    int heapBase;
    u32 *freeBlock;
    u32 blockHeader;
    u32 blockSize;
    u32 alignedSize;
    SetMemLock(1);
    result = (u32 *)0x0;
    heapBase = Other_MemHeapPtr;
    if (Other_MemHeapPtr == 0) {
        heapBase = heapFallback;
    }
    if (size != 0) {
        /* Align to 4 bytes */
        if ((size & 3) != 0) {
            size = (size + 4) - (size & 3);
        }
        if (size < 0xc) {
            size = 0xc;
        }
        alignedSize = size + 4;
        for (freeBlock = *(u32 **)(heapBase + 8); freeBlock != (u32 *)0x0; freeBlock = (u32 *)freeBlock[1]) {
            blockHeader = *freeBlock;
            blockSize = blockHeader & 0xfffffff;
            if (alignedSize <= blockSize) {
                *freeBlock = blockHeader & 0xbfffffff;
                result = freeBlock + 1;
                if (blockSize < size + 0x14) {
                    /* Use entire block (too small to split) */
                    u32 *next = (u32 *)((int)freeBlock + (blockHeader & 0xfffffff));
                    *next = *next & 0x7fffffff;
                    if (freeBlock[1] == 0) {
                        *(u32 *)(heapBase + 0xc) = freeBlock[2];
                    } else {
                        *(u32 *)(freeBlock[1] + 8) = freeBlock[2];
                    }
                    if (freeBlock[2] == 0) {
                        *(u32 *)(heapBase + 8) = freeBlock[1];
                    } else {
                        *(u32 *)(freeBlock[2] + 4) = freeBlock[1];
                    }
                } else {
                    /* Split block */
                    u32 *newBlock = (u32 *)((int)freeBlock + (alignedSize & 0xfffffff));
                    *freeBlock = blockHeader & 0xb0000000 | alignedSize;
                    *newBlock = (blockSize - alignedSize) | 0x40000000;
                    newBlock[1] = freeBlock[1];
                    newBlock[2] = freeBlock[2];
                    if (freeBlock[1] == 0) {
                        *(u32 **)(heapBase + 0xc) = newBlock;
                    } else {
                        *(u32 **)(freeBlock[1] + 8) = newBlock;
                    }
                    if (freeBlock[2] == 0) {
                        *(u32 **)(heapBase + 8) = newBlock;
                    } else {
                        *(u32 **)(freeBlock[2] + 4) = newBlock;
                    }
                    *(u32 **)((int)newBlock + ((*newBlock & 0xfffffff) - 4)) = newBlock;
                }
                break;
            }
        }
    }
    SetMemLock(0);
    return result;
}
/* Memory free */
/* Frees memory previously allocated by MemAlloc. ptr is the user pointer (header-4) */
u32 MemFree(int ptr, int heapFallback)
{
    int heapBase;
    u32 *block;
    u32 *nextBlock;
    u32 nextHeader;
    SetMemLock(1);
    heapBase = Other_MemHeapPtr;
    if (Other_MemHeapPtr == 0) {
        heapBase = heapFallback;
    }
    block = (u32 *)(ptr - 4);
    if (ptr != 0) {
        u32 blockSize = block[0] & 0xfffffff;
        nextBlock = (u32 *)((int)block + blockSize);
        nextHeader = *nextBlock;
        /* Coalesce with previous block if it's free */
        if ((int)block[0] < 0) {
            u32 *prevBlock = (u32 *)block[-1];
            *prevBlock = *prevBlock & 0xf0000000 | (blockSize + (*prevBlock & 0xfffffff));
            if (prevBlock[1] == 0) {
                *(u32 *)(heapBase + 0xc) = prevBlock[2];
            } else {
                *(u32 *)(prevBlock[1] + 8) = prevBlock[2];
            }
            if (prevBlock[2] == 0) {
                *(u32 *)(heapBase + 8) = prevBlock[1];
            } else {
                *(u32 *)(prevBlock[2] + 4) = prevBlock[1];
            }
            block = prevBlock;
        }
        /* Coalesce with next block if it's free */
        if ((nextHeader & 0x40000000) != 0) {
            *block = *block & 0xf0000000 | ((*nextBlock & 0xfffffff) + (*block & 0xfffffff));
            if (nextBlock[1] == 0) {
                *(u32 *)(heapBase + 0xc) = nextBlock[2];
            } else {
                *(u32 *)(nextBlock[1] + 8) = nextBlock[2];
            }
            if (nextBlock[2] == 0) {
                *(u32 *)(heapBase + 8) = nextBlock[1];
            } else {
                *(u32 *)(nextBlock[2] + 4) = nextBlock[1];
            }
            nextBlock = (u32 *)((int)block + (*block & 0xfffffff));
        }
        /* Insert block into free list */
        block[1] = *(u32 *)(heapBase + 8);
        *(u32 **)(heapBase + 8) = block;
        block[2] = 0;
        if (block[1] == 0) {
            *(u32 **)(heapBase + 0xc) = block;
        } else {
            *(u32 **)(block[1] + 8) = block;
        }
        nextBlock[-1] = (u32)block;
        *block = *block | 0x40000000;
        *nextBlock = *nextBlock | 0x80000000;
    }
    SetMemLock(0);
    return 0;
}
/* vtable getter for coordinate system data */
void *GetVtable_CoordSystem(void)
{
    return &Other_CoordSysVtable;
}
/* Linked-list traversal: advance iterator pointer and decrement count */
void ListIterateNext(void **iterator, int *remaining)
{
    if (*remaining == 0) {
        *iterator = 0;
    } else {
        *iterator = *(void **)((int)*iterator + 4);
        *remaining = *(int *)*iterator;
    }
}
/* Call vtable constructors sequentially for an array of objects */
void InitObjectArrayByVtable(u32 **objArray, int count)
{
    for (; 0 < count; count--) {
        *objArray = (u32 *)((*(code *)((int)*objArray + 4))());
        objArray++;
    }
}
/* Mutext/lock setter */
void SetMemLock(u32 val)
{
    Other_MemLockValue = val;
}
/* Mutext/lock getter */
u32 GetMemLock(void)
{
    return Other_MemLockValue;
}
/* Transform and submit geometry for rendering via GTE */
/* Handles flat/gouraud shaded, textured, and gouraud-textured polygons */
/* Supports primitives: F3, F4, G3, G4, FT3, FT4, GT3, GT4 */
void SubmitGeometryPrimitives(u32 *prim, int renderCtx, u32 priority, u32 *primBuf)
{
    GsCOORDINATE2 *coord;
    MATRIX savedRotMtx;
    u32 *data;
    u32 *polyBuf;
    u32 *polyPtr;
    u32 vertCount;
    int remainingPolys;
    u32 ctxFlags;
    u32 *r0;
    if ((int)*prim < 0) {
        return;
    }
    primBuf[1] = *(u32 *)(renderCtx + 4);
    primBuf[0] = priority;
    primBuf[1] = priority;
    SetupOtEntryRefs((int *)(primBuf + 0x22), (int)&Other_OTEntryTriBuf, 3);
    SetupOtEntryRefs((int *)(primBuf + 0x25), (int)&Other_OTEntryQuadBuf, 4);
    coord = (GsCOORDINATE2 *)prim[2];
    remainingPolys = coord->param;
    data = (u32 *)coord->super;
    primBuf[3] = coord->flg;
    primBuf[4] = *(u32 *)(prim[2] + 8);
    savedRotMtx = *(MATRIX *)(primBuf + 0xe);
    u8 *scratchBuf = (u8 *)&savedRotMtx;
    if (*(int *)(prim[1] + 0x48) != 0) {
        gte_ReadRotMatrix(savedRotMtx);
        u32 parent = prim[1];
        gte_SetRotMatrix((MATRIX *)(*(int *)(parent + 0x48) + 0x24));
        MATRIX *pM = (MATRIX *)(parent + 0x24);
        gte_ldclmv(pM);
        gte_rtir();
        gte_stclmv(pM);
        pM = (MATRIX *)(prim[1] + 0x26);
        gte_ldclmv(pM);
        gte_rtir();
        gte_stclmv(pM);
        pM = (MATRIX *)(prim[1] + 0x28);
        gte_ldclmv(pM);
        gte_rtir();
        gte_stclmv(pM);
        gte_SetRotMatrix(savedRotMtx);
        scratchBuf = (u8 *)&savedRotMtx;
    }
    ctxFlags = *prim;
    primBuf[2] = 10;
    Other_CtxFlagsShading = ctxFlags >> 9 & 7;
    Other_PrimSemiTransFlag = ctxFlags >> 6 & 1;
    Other_PrimDitherFlag = ctxFlags >> 5 & 1;
    Other_PrimBlendMode = ctxFlags >> 3 & 3;
    *(u8 *)(primBuf + 0xd) = 0x80;
    *(u8 *)((int)primBuf + 0x35) = 0x80;
    *(u8 *)((int)primBuf + 0x36) = 0x80;
    if (((Other_PrimDitherFlag == 0) || (Other_RenderDitherFlag == 0)) && (Other_PrimBlendMode == 0)) {
        primBuf[0xb] = 0x10;
    } else {
        primBuf[0xb] = 9;
    }
    if (remainingPolys != 0) {
        do {
            u32 v0_val = System_GsClipMode;
            primBuf[6] = *(ushort *)((int)data + 2) & 0xfd07;
            vertCount = (uint)(ushort)*data;
            u32 fmt = primBuf[6];
            primBuf[7] = *data >> 0x19 & 1;
            remainingPolys -= vertCount;
            if (fmt == PRIM_ID_QUAD_TEX) {
                /* Primitive: Flat-shaded textured quad (F4) */
                *(u8 *)(v0_val + 3) = 5;
                *(u8 *)(v0_val + 7) = PRIM_HDR_QUAD;
                SetRawPolyGtxFlags(v0_val, primBuf);
                CVECTOR *cv = (CVECTOR *)(data + 1);
                do {
                    u32 packed1, packed2;
                    packed1 = *(ushort *)((int)cv + 4) | (*(ushort *)((int)cv + 6) << 16);
                    packed2 = *(ushort *)((int)cv + 8) | (*(ushort *)((int)cv + 10) << 16);
                    *(uint *)(scratchBuf + 0x10) = packed2;
                    packed2 = *(ushort *)((int)cv + 12) | (*(ushort *)((int)cv + 14) << 16);
                    *(u32 *)(scratchBuf + 0x18) = (u32)0x80019724;
                    *(uint *)(scratchBuf + 0x14) = packed2;
                    int clipped = Submit4VerticesClip(v0_val, primBuf, (ushort)packed1, (ushort)(packed1 >> 16), (ushort)packed2, (ushort)(packed2 >> 16));
                    data += 4;
                    if (clipped == 0) {
                        gte_ldrgb(cv);
                        gte_dpcs();
                        gte_strgb((CVECTOR *)(v0_val + 1));
                        *(u8 *)(v0_val + 7) = *(u8 *)((int)primBuf + 0x15);
                        v0_val = (u32 *)SubmitQuadFlat(v0_val, primBuf);
                    }
                    vertCount--;
                    cv += 4;
                } while (vertCount != 0);
            } else if (fmt < 0x2902) {
                if (fmt == PRIM_ID_TRI_TEX) {
                    /* Poly: Flat shaded triangle (F3) */
                    *(u8 *)(v0_val + 3) = 4;
                    *(u8 *)(v0_val + 7) = PRIM_HDR_TRI;
                    SetRawPolyGtxFlags(v0_val, primBuf);
                    CVECTOR *cv = (CVECTOR *)(data + 1);
                    do {
                        u32 packed1 = *(ushort *)((int)cv + 4) | (*(ushort *)((int)cv + 6) << 16);
                        u32 packed2 = *(ushort *)((int)cv + 8) | (*(ushort *)((int)cv + 10) << 16);
                        *(u32 *)(scratchBuf + 0x14) = (u32)0x800196d4;
                        *(uint *)(scratchBuf + 0x10) = packed2;
                        int clipped = Submit3VerticesClip(v0_val, primBuf, (ushort)packed1, (ushort)(packed1 >> 16), (ushort)packed2);
                        data += 4;
                        if (clipped == 0) {
                            gte_ldrgb(cv);
                            gte_dpcs();
                            gte_strgb((CVECTOR *)(v0_val + 1));
                            *(u8 *)(v0_val + 7) = *(u8 *)((int)primBuf + 0x15);
                            v0_val = (u32 *)SubmitTriFlat(v0_val, primBuf);
                        }
                        vertCount--;
                        cv += 4;
                    } while (vertCount != 0);
                } else if (fmt < 0x2102) {
                    if (fmt == PRIM_ID_TRI) {
                        /* Poly: Gouraud triangle (G3) */
                        *(u8 *)(v0_val + 3) = 4;
                        *(u8 *)(v0_val + 7) = PRIM_HDR_TRI;
                        SetRawPolyGtxFlags(v0_val, primBuf);
                        CVECTOR *cv = (CVECTOR *)(data + 1);
                        do {
                            u32 packed1 = *(ushort *)((int)cv + 4) | (*(ushort *)((int)cv + 6) << 16);
                            u32 packed2 = *(ushort *)((int)cv + 8) | (*(ushort *)((int)cv + 10) << 16);
                            *(u32 *)(scratchBuf + 0x14) = (u32)0x800196d4;
                            *(uint *)(scratchBuf + 0x10) = packed2;
                            int clipped = Submit3VerticesClip(v0_val, primBuf, (ushort)packed1, (ushort)(packed1 >> 16), (ushort)packed2);
                            data += 4;
                            if (clipped == 0) {
                                u32 vIdx = *(ushort *)((int)cv + 2) | (*(ushort *)((int)cv + 4) << 16);
                                gte_ldv0((SVECTOR *)((uint)(ushort)vIdx * 8 + primBuf[4]));
                                gte_ldrgb(cv);
                                gte_ncds();
                                gte_strgb((CVECTOR *)(v0_val + 1));
                                *(u8 *)(v0_val + 7) = *(u8 *)((int)primBuf + 0x15);
                                v0_val = (u32 *)SubmitTriFlat(v0_val, primBuf);
                            }
                            vertCount--;
                            cv += 4;
                        } while (vertCount != 0);
                    } else {
                        if (fmt != PRIM_ID_TEXTURED_TRI) return;
                        /* Poly: Gouraud quad (G4) */
                        *(u8 *)(v0_val + 3) = 6;
                        *(u8 *)(v0_val + 7) = PRIM_HDR_GOURAUD_TRI;
                        SetRawPolyGtxFlags(v0_val, primBuf);
                        CVECTOR *cv = (CVECTOR *)(data + 3);
                        do {
                            u32 packed1 = *(ushort *)((int)cv + 4) | (*(ushort *)((int)cv + 6) << 16);
                            u32 packed2 = *(ushort *)((int)cv + 8) | (*(ushort *)((int)cv + 10) << 16);
                            *(u32 *)(scratchBuf + 0x14) = (u32 *)0x800196e8;
                            *(uint *)(scratchBuf + 0x10) = packed2;
                            int clipped = Submit3VerticesClip(v0_val, primBuf, (ushort)packed1, (ushort)(packed1 >> 16), (ushort)packed2);
                            if (clipped == 0) {
                                u32 vIdx = *(ushort *)((int)cv + 1) | (*(ushort *)((int)cv + 4) << 16);
                                gte_ldv0((SVECTOR *)((uint)(ushort)vIdx * 8 + primBuf[4]));
                                gte_ldrgb((CVECTOR *)(data + 1));
                                gte_ncds();
                                gte_strgb((CVECTOR *)(v0_val + 1));
                                *(u8 *)(v0_val + 7) = *(u8 *)((int)primBuf + 0x15);
                                gte_ldrgb((CVECTOR *)(data + 2));
                                gte_ncds();
                                gte_strgb((CVECTOR *)(v0_val + 3));
                                gte_ldrgb(cv);
                                gte_ncds();
                                gte_strgb((CVECTOR *)(v0_val + 5));
                                v0_val = (u32 *)SubmitTriGouraud(v0_val, primBuf);
                            }
                            cv += 6;
                            vertCount--;
                            data += 6;
                        } while (vertCount != 0);
                    }
                } else if (fmt == PRIM_ID_SPRITE_16) {
                    /* Poly: Textured flat triangle (FT3) */
                    *(u8 *)(v0_val + 3) = 7;
                    *(u8 *)(v0_val + 7) = PRIM_HDR_TEXTURED_TRI;
                    SetRawPolyGtxFlags(v0_val, primBuf);
                    CVECTOR *cv = (CVECTOR *)(data + 4);
                    do {
                        u32 packed1 = *(ushort *)((int)cv + 4) | (*(ushort *)((int)cv + 6) << 16);
                        u32 packed2 = *(ushort *)((int)cv + 8) | (*(ushort *)((int)cv + 10) << 16);
                        *(u32 *)(scratchBuf + 0x14) = (u32)0x800196fc;
                        *(uint *)(scratchBuf + 0x10) = packed2;
                        int clipped = Submit3VerticesClip(v0_val, primBuf, (ushort)packed1, (ushort)(packed1 >> 16), (ushort)packed2);
                        data += 7;
                        if (clipped == 0) {
                            *(CVECTOR *)(v0_val + 3) = ((CVECTOR *)(data - 7))[-3];
                            *(CVECTOR *)(v0_val + 5) = ((CVECTOR *)(data - 7))[-2];
                            *(CVECTOR *)(v0_val + 7) = ((CVECTOR *)(data - 7))[-1];
                            *(short *)((int)v0_val + 0xe) += (short)((int)primBuf[9] >> (primBuf[0xb] & 0x1f)) * 0x40;
                            gte_ldrgb(cv);
                            gte_dpcs();
                            gte_strgb((CVECTOR *)(v0_val + 1));
                            *(u8 *)(v0_val + 7) = *(u8 *)((int)primBuf + 0x15);
                            v0_val = (u32 *)SubmitTriTexFlat(v0_val, primBuf);
                        }
                        vertCount--;
                        cv += 7;
                    } while (vertCount != 0);
                } else if (fmt < 0x2502) {
                    if (fmt != PRIM_ID_SPRITE_TEX) return;
                    /* Poly: Textured flat triangle (FT3) - alt format */
                    *(u8 *)(v0_val + 3) = 7;
                    *(u8 *)(v0_val + 7) = PRIM_HDR_TEXTURED_TRI;
                    SetRawPolyGtxFlags(v0_val, primBuf);
                    gte_ldrgb((CVECTOR *)(primBuf + 0xd));
                    u32 *pData = data + 4;
                    do {
                        u32 packed12 = *(ushort *)((int)pData + 2);
                        u32 packed34 = pData[1];
                        u32 packed56 = *(ushort *)((int)pData + 6);
                        *(u32 *)(scratchBuf + 0x14) = (u32)0x800196fc;
                        *(uint *)(scratchBuf + 0x10) = packed56;
                        int clipped = Submit3VerticesClip(v0_val, primBuf, (ushort)packed12, (ushort)packed34, (ushort)packed56);
                        data += 6;
                        if (clipped == 0) {
                            ((u32 *)v0_val)[3] = ((u32 *)(data - 6))[-3];
                            ((u32 *)v0_val)[5] = ((u32 *)(data - 6))[-2];
                            ((u32 *)v0_val)[7] = ((u32 *)(data - 6))[-1];
                            *(short *)((int)v0_val + 0xe) += (short)((int)primBuf[9] >> (primBuf[0xb] & 0x1f)) * 0x40;
                            gte_ldv0((SVECTOR *)((uint)(ushort)*(data - 6) * 8 + primBuf[4]));
                            gte_ncds();
                            gte_strgb((CVECTOR *)(v0_val + 1));
                            *(u8 *)(v0_val + 7) = *(u8 *)((int)primBuf + 0x15);
                            v0_val = (u32 *)SubmitTriTexFlat(v0_val, primBuf);
                        }
                        vertCount--;
                        pData += 6;
                    } while (vertCount != 0);
                } else {
                    if (fmt != PRIM_ID_QUAD) return;
                    /* Poly: Textured flat quad (FT4) */
                    *(u8 *)(v0_val + 3) = 5;
                    *(u8 *)(v0_val + 7) = PRIM_HDR_QUAD;
                    SetRawPolyGtxFlags(v0_val, primBuf);
                    CVECTOR *cv = (CVECTOR *)(data + 1);
                    do {
                        u32 packed1 = *(ushort *)((int)cv + 4) | (*(ushort *)((int)cv + 6) << 16);
                        u32 packed2 = *(ushort *)((int)cv + 8) | (*(ushort *)((int)cv + 10) << 16);
                        u32 packed3 = *(ushort *)((int)cv + 12) | (*(ushort *)((int)cv + 14) << 16);
                        *(uint *)(scratchBuf + 0x10) = packed2;
                        *(u32 *)(scratchBuf + 0x18) = (u32)0x80019724;
                        *(uint *)(scratchBuf + 0x14) = packed3;
                        int clipped = Submit4VerticesClip(v0_val, primBuf, (ushort)packed1, (ushort)(packed1 >> 16), (ushort)packed2, (ushort)(packed2 >> 16));
                        data += 5;
                        if (clipped == 0) {
                            u32 vIdx = *(ushort *)((int)cv + 1) | (*(ushort *)((int)cv + 4) << 16);
                            gte_ldv0((SVECTOR *)((uint)(ushort)vIdx * 8 + primBuf[4]));
                            gte_ldrgb(cv);
                            gte_ncds();
                            gte_strgb((CVECTOR *)(v0_val + 1));
                            *(u8 *)(v0_val + 7) = *(u8 *)((int)primBuf + 0x15);
                            v0_val = (u32 *)SubmitQuadFlat(v0_val, primBuf);
                        }
                        vertCount--;
                        cv += 5;
                    } while (vertCount != 0);
                }
            } else if (fmt == PRIM_ID_GOURAUD_TRI_TEX) {
                /* Poly: Gouraud textured triangle (GT3) */
                *(u8 *)(v0_val + 3) = 6;
                *(u8 *)(v0_val + 7) = PRIM_HDR_GOURAUD_TRI;
                SetRawPolyGtxFlags(v0_val, primBuf);
                CVECTOR *cv = (CVECTOR *)(data + 1);
                do {
                    u32 packed1 = *(ushort *)((int)cv + 4) | (*(ushort *)((int)cv + 6) << 16);
                    u32 packed2 = *(ushort *)((int)cv + 8) | (*(ushort *)((int)cv + 10) << 16);
                    *(u32 *)(scratchBuf + 0x14) = (u32 *)0x800196e8;
                    *(uint *)(scratchBuf + 0x10) = packed2;
                    int clipped = Submit3VerticesClip(v0_val, primBuf, (ushort)packed1, (ushort)(packed1 >> 16), (ushort)packed2);
                    data += 6;
                    if (clipped == 0) {
                        gte_ldrgb3c(cv);
                        gte_dpct();
                        gte_strgb3_g3(v0_val);
                        *(u8 *)(v0_val + 7) = *(u8 *)((int)primBuf + 0x15);
                        v0_val = (u32 *)SubmitTriGouraud(v0_val, primBuf);
                    }
                    vertCount--;
                    cv += 6;
                } while (vertCount != 0);
            } else if (fmt < 0x3102) {
                if (fmt == PRIM_ID_GOURAUD_QUAD) {
                    /* Poly: Gouraud textured quad (GT4) - alt */
                    *(u8 *)(v0_val + 3) = 9;
                    *(u8 *)(v0_val + 7) = PRIM_HDR_TEXTURED_QUAD;
                    SetRawPolyGtxFlags(v0_val, primBuf);
                    gte_ldrgb((CVECTOR *)(primBuf + 0xd));
                    u32 *pData = data + 5;
                    do {
                        u32 packed12 = *(ushort *)((int)pData + 2);
                        u32 packed34 = pData[1];
                        *(uint *)(scratchBuf + 0x10) = *(ushort *)((int)pData + 6);
                        u32 packed56 = pData[2];
                        *(u32 *)(scratchBuf + 0x18) = (u32)0x80019774;
                        *(uint *)(scratchBuf + 0x14) = (uint)(ushort)packed56;
                        int clipped = Submit4VerticesClip(v0_val, primBuf, (ushort)packed12, (ushort)packed34, (ushort)*(ushort *)((int)pData + 6), (ushort)packed56);
                        data += 8;
                        if (clipped == 0) {
                            ((u32 *)v0_val)[3] = ((u32 *)(data - 8))[-4];
                            ((u32 *)v0_val)[5] = ((u32 *)(data - 8))[-3];
                            ((u32 *)v0_val)[7] = ((u32 *)(data - 8))[-2];
                            ((u32 *)v0_val)[9] = ((u32 *)(data - 8))[-1];
                            *(short *)((int)v0_val + 0xe) += (short)((int)primBuf[9] >> (primBuf[0xb] & 0x1f)) * 0x40;
                            gte_ldv0((SVECTOR *)((uint)(ushort)*(data - 8) * 8 + primBuf[4]));
                            gte_ncds();
                            gte_strgb((CVECTOR *)(v0_val + 1));
                            *(u8 *)(v0_val + 7) = *(u8 *)((int)primBuf + 0x15);
                            v0_val = (u32 *)SubmitQuadTexFlat(v0_val, primBuf);
                        }
                        vertCount--;
                        pData += 8;
                    } while (vertCount != 0);
                } else {
                    if (fmt != PRIM_ID_GOURAUD_TEX_QUAD) return;
                    /* Poly: FT4 textured flat (FT4) */
                    *(u8 *)(v0_val + 3) = 9;
                    *(u8 *)(v0_val + 7) = PRIM_HDR_TEXTURED_QUAD;
                    SetRawPolyGtxFlags(v0_val, primBuf);
                    CVECTOR *cv = (CVECTOR *)(data + 5);
                    do {
                        u32 packed1 = *(ushort *)((int)cv + 4) | (*(ushort *)((int)cv + 6) << 16);
                        u32 packed2 = *(ushort *)((int)cv + 8) | (*(ushort *)((int)cv + 10) << 16);
                        *(uint *)(scratchBuf + 0x10) = packed2;
                        u32 packed3 = *(ushort *)((int)cv + 12) | (*(ushort *)((int)cv + 14) << 16);
                        *(u32 *)(scratchBuf + 0x18) = (u32)0x80019774;
                        *(uint *)(scratchBuf + 0x14) = packed3;
                        int clipped = Submit4VerticesClip(v0_val, primBuf, (ushort)packed1, (ushort)(packed1 >> 16), (ushort)packed2, (ushort)(packed2 >> 16));
                        data += 8;
                        if (clipped == 0) {
                            *(CVECTOR *)(v0_val + 3) = ((CVECTOR *)(data - 8))[-4];
                            *(CVECTOR *)(v0_val + 5) = ((CVECTOR *)(data - 8))[-3];
                            *(CVECTOR *)(v0_val + 7) = ((CVECTOR *)(data - 8))[-2];
                            *(CVECTOR *)(v0_val + 9) = ((CVECTOR *)(data - 8))[-1];
                            *(short *)((int)v0_val + 0xe) += (short)((int)primBuf[9] >> (primBuf[0xb] & 0x1f)) * 0x40;
                            gte_ldrgb(cv);
                            gte_dpcs();
                            gte_strgb((CVECTOR *)(v0_val + 1));
                            *(u8 *)(v0_val + 7) = *(u8 *)((int)primBuf + 0x15);
                            v0_val = (u32 *)SubmitQuadTexFlat(v0_val, primBuf);
                        }
                        vertCount--;
                        cv += 8;
                    } while (vertCount != 0);
                }
            } else if (fmt == PRIM_ID_GOURAUD_QUAD_TEX) {
                /* Poly: Gouraud quad textured (G4) - full color per vertex */
                *(u8 *)(v0_val + 3) = 8;
                *(u8 *)(v0_val + 7) = PRIM_HDR_GOURAUD_QUAD;
                SetRawPolyGtxFlags(v0_val, primBuf);
                CVECTOR *cv = (CVECTOR *)(data + 4);
                do {
                    u32 packed1 = *(ushort *)((int)cv + 4) | (*(ushort *)((int)cv + 6) << 16);
                    u32 packed2 = *(ushort *)((int)cv + 8) | (*(ushort *)((int)cv + 10) << 16);
                    *(uint *)(scratchBuf + 0x10) = packed2;
                    u32 packed3 = *(ushort *)((int)cv + 12) | (*(ushort *)((int)cv + 14) << 16);
                    *(u32 *)(scratchBuf + 0x18) = (u32)0x8001974c;
                    *(uint *)(scratchBuf + 0x14) = packed3;
                    int clipped = Submit4VerticesClip(v0_val, primBuf, (ushort)packed1, (ushort)(packed1 >> 16), (ushort)packed2, (ushort)(packed2 >> 16));
                    if (clipped == 0) {
                        gte_ldrgb3c((CVECTOR *)(data + 1));
                        gte_dpct();
                        gte_strgb3_g3(v0_val);
                        *(u8 *)(v0_val + 7) = *(u8 *)((int)primBuf + 0x15);
                        gte_ldrgb(cv);
                        gte_dpcs();
                        gte_strgb((CVECTOR *)(v0_val + 7));
                        v0_val = (u32 *)SubmitQuadGouraud(v0_val, primBuf);
                    }
                    cv += 7;
                    vertCount--;
                    data += 7;
                } while (vertCount != 0);
            } else if (fmt < 0x3902) {
                if (fmt != PRIM_ID_SPRITE_SHADE_16) return;
                /* Poly: Gouraud textured triangle (GT3) */
                *(u8 *)(v0_val + 3) = 9;
                *(u8 *)(v0_val + 7) = PRIM_HDR_GOURAUD_TEX_TRI;
                SetRawPolyGtxFlags(v0_val, primBuf);
                CVECTOR *cv = (CVECTOR *)(data + 6);
                do {
                    u32 packed1 = *(ushort *)((int)cv + 4) | (*(ushort *)((int)cv + 6) << 16);
                    u32 packed2 = *(ushort *)((int)cv + 8) | (*(ushort *)((int)cv + 10) << 16);
                    *(u32 *)(scratchBuf + 0x14) = (u32)0x80019710;
                    *(uint *)(scratchBuf + 0x10) = packed2;
                    int clipped = Submit3VerticesClip(v0_val, primBuf, (ushort)packed1, (ushort)(packed1 >> 16), (ushort)packed2);
                    vertCount--;
                    if (clipped == 0) {
                        *(CVECTOR *)(v0_val + 3) = ((CVECTOR *)(data - 6))[-5];
                        *(CVECTOR *)(v0_val + 6) = ((CVECTOR *)(data - 6))[-4];
                        *(CVECTOR *)(v0_val + 9) = ((CVECTOR *)(data - 6))[-3];
                        *(short *)((int)v0_val + 0xe) += (short)((int)primBuf[9] >> (primBuf[0xb] & 0x1f)) * 0x40;
                        gte_ldrgb3((CVECTOR *)(data - 6 + 4), (CVECTOR *)(data - 6 + 5), cv);
                        gte_dpct();
                        gte_strgb3((CVECTOR *)(v0_val + 1), (CVECTOR *)(v0_val + 4), (CVECTOR *)(v0_val + 7));
                        *(u8 *)(v0_val + 7) = *(u8 *)((int)primBuf + 0x15);
                        v0_val = (u32 *)SubmitTriTexGouraud(v0_val, primBuf);
                    }
                    cv += 9;
                    data += 9;
                } while (vertCount != 0);
            } else {
                if (fmt != 0x3d01) return;
                /* Poly: Gouraud textured quad (GT4) */
                *(u8 *)(v0_val + 3) = 0xc;
                *(u8 *)(v0_val + 7) = PRIM_HDR_GOURAUD_TEX_QUAD;
                SetRawPolyGtxFlags(v0_val, primBuf);
                CVECTOR *cv = (CVECTOR *)(data + 8);
                do {
                    u32 packed1 = *(ushort *)((int)cv + 4) | (*(ushort *)((int)cv + 6) << 16);
                    u32 packed2 = *(ushort *)((int)cv + 8) | (*(ushort *)((int)cv + 10) << 16);
                    *(uint *)(scratchBuf + 0x10) = packed2;
                    u32 packed3 = *(ushort *)((int)cv + 12) | (*(ushort *)((int)cv + 14) << 16);
                    *(u32 *)(scratchBuf + 0x18) = (u32)0x8001979c;
                    *(uint *)(scratchBuf + 0x14) = packed3;
                    int clipped = Submit4VerticesClip(v0_val, primBuf, (ushort)packed1, (ushort)(packed1 >> 16), (ushort)packed2, (ushort)(packed2 >> 16));
                    if (clipped == 0) {
                        *(CVECTOR *)(v0_val + 3) = ((CVECTOR *)(data - 8))[-7];
                        *(CVECTOR *)(v0_val + 6) = ((CVECTOR *)(data - 8))[-6];
                        *(CVECTOR *)(v0_val + 9) = ((CVECTOR *)(data - 8))[-5];
                        *(CVECTOR *)(v0_val + 0xc) = ((CVECTOR *)(data - 8))[-4];
                        *(short *)((int)v0_val + 0xe) += (short)((int)primBuf[9] >> (primBuf[0xb] & 0x1f)) * 0x40;
                        gte_ldrgb3((CVECTOR *)(data - 8 + 5), (CVECTOR *)(data - 8 + 6), (CVECTOR *)(data - 8 + 7));
                        gte_dpct();
                        gte_strgb3((CVECTOR *)(v0_val + 1), (CVECTOR *)(v0_val + 4), (CVECTOR *)(v0_val + 7));
                        *(u8 *)(v0_val + 7) = *(u8 *)((int)primBuf + 0x15);
                        gte_ldrgb(cv);
                        gte_dpcs();
                        gte_strgb((CVECTOR *)(v0_val + 10));
                        v0_val = (u32 *)SubmitQuadTexGouraud(v0_val, primBuf);
                    }
                    cv += 0xb;
                    vertCount--;
                    data += 0xb;
                } while (vertCount != 0);
            }
            System_GsClipMode = (u32)v0_val;
        } while (remainingPolys != 0);
    }
}
/* Configure primitive flags: dithering, semi-transparency */
void SetPolyGtxFlags(int prim, int renderCtx)
{
    u8 flags;
    if (*(int *)(renderCtx + 0x1c) == 0) {
        flags = *(u8 *)(prim + 7) & 0xfd;
    } else {
        flags = *(u8 *)(prim + 7) | 2;
    }
    *(u8 *)(prim + 7) = flags;
    if (Other_PrimSemiTransFlag == 0) {
        flags = *(u8 *)(prim + 7) & 0xfe;
    } else {
        flags = *(u8 *)(prim + 7) | 1;
    }
    *(u8 *)(prim + 7) = flags;
    *(u8 *)(renderCtx + 0x14) = *(u8 *)(prim + 3);
    *(u8 *)(renderCtx + 0x15) = *(u8 *)(prim + 7);
}
/* Transform 3 vertices through GTE and check clipping */
/* Returns non-zero if polygon should be clipped */
bool Transform3Vertices(int renderCtx, u32 vIdx1, u32 vIdx2, u16 vIdx3, code *clipFn)
{
    SVECTOR *v3;
    *(u32 *)(renderCtx + 0xa4) = (vIdx1 & 0xffff) * 8 + *(int *)(renderCtx + 0xc);
    *(u32 *)(renderCtx + 0xa8) = (vIdx2 & 0xffff) * 8 + *(int *)(renderCtx + 0xc);
    v3 = (SVECTOR *)((uint)vIdx3 * 8 + *(int *)(renderCtx + 0xc));
    *(SVECTOR **)(renderCtx + 0xac) = v3;
    gte_ldv3(*(SVECTOR **)(renderCtx + 0xa4), *(SVECTOR **)(renderCtx + 0xa8), v3);
    int clipped = GteTransformTest(renderCtx, 0);
    if (clipped == 0) {
        gte_stsz3((long *)(*(int *)(renderCtx + 0x88) + 0x14),
                  (long *)(*(int *)(renderCtx + 0x8c) + 0x14),
                  (long *)(*(int *)(renderCtx + 0x90) + 0x14));
        (*clipFn)(renderCtx);
        UpdateBounds(renderCtx, 3);
    }
    return clipped != 0;
}
/* Transform 4 vertices through GTE and check for clipping */
/* Returns true if polygon should be clipped */
bool Transform4Vertices(int renderCtx, u32 vIdx1, u32 vIdx2, u16 vIdx3, u16 vIdx4, code *clipFn)
{
    u32 *vtxArray;
    vtxArray = (u32 *)(renderCtx + 0xa4);
    vtxArray[0] = (vIdx1 & 0xffff) * 8 + *(int *)(renderCtx + 0xc);
    vtxArray[1] = (vIdx2 & 0xffff) * 8 + *(int *)(renderCtx + 0xc);
    vtxArray[2] = (uint)vIdx3 * 8 + *(int *)(renderCtx + 0xc);
    vtxArray[3] = (uint)vIdx4 * 8 + *(int *)(renderCtx + 0xc);
    gte_ldv3((SVECTOR *)vtxArray[0], *(SVECTOR **)(renderCtx + 0xa8), *(SVECTOR **)(renderCtx + 0xac));
    int clipped = GteTransformTest(renderCtx, 0);
    if (clipped == 0) {
        (*clipFn)(renderCtx, 1);
        gte_ldv0((SVECTOR *)vtxArray[3]);
        gte_rtps();
        gte_stsz4((long *)(*(int *)(renderCtx + 0x94) + 0x14),
                  (long *)(*(int *)(renderCtx + 0x98) + 0x14),
                  (long *)(*(int *)(renderCtx + 0x9c) + 0x14),
                  (long *)(*(int *)(renderCtx + 0xa0) + 0x14));
        (*clipFn)(renderCtx, 0);
        gte_stsxy((long *)(renderCtx + 0x6c));
        UpdateBounds(renderCtx, 4);
    }
    return clipped != 0;
}
/* Perform GTE perspective transform with clipping and depth sorting */
/* Returns 0 if polygon is visible, 1 if clipped */
u32 GtePerspectiveTransform(int prim, int *renderCtx)
{
    renderCtx[0x1e] = 0;
    gte_rtpt();
    *(char *)(prim + 3) = (char)renderCtx[5];
    gte_stflg_4(renderCtx + 0x17);
    if (renderCtx[0x17] != 0) {
        if (renderCtx[0x17] != 0x40000) {
            return 1;
        }
        renderCtx[0x1e] = 1;
    }
    gte_nclip();
    gte_stopz(renderCtx + 10);
    if (renderCtx[10] < 1) {
        return 1;
    }
    gte_stdp(renderCtx + 9);
    if (renderCtx[9] < CLIP_FAR) {
        gte_avsz3();
        gte_stotz(renderCtx + 8);
        gte_stsxy3(renderCtx + 0x18, renderCtx + 0x19, renderCtx + 0x1a);
        renderCtx[0xc] = (renderCtx[8] >> (renderCtx[1] & 0x1f)) * 4 + *renderCtx;
        return 0;
    }
    return 1;
}
/* Submit triangle primitive (F3) to ordering table */
u32 *SubmitPolyF3(u32 *prim, int renderCtx)
{
    u32 *nextPrim;
    if (*(int *)(renderCtx + 0x78) == 0) {
        *prim = *prim & 0xff000000 | **(u32 **)(renderCtx + 0x30) & 0xffffff;
        nextPrim = prim + 5;
        **(u32 **)(renderCtx + 0x30) = **(u32 **)(renderCtx + 0x30) & 0xff000000 | (u32)prim & 0xffffff;
    } else {
        SetupPrimOtEntry(&Other_OTEntryTriBuf, renderCtx, (u32)(prim + 1), 0, 0, 0);
        StoreRawVertices3((u32 *)(renderCtx + 0x88), (u32 *)(renderCtx + 0xa4), (u32)(prim + 2), (u32)(prim + 3), (u32)(prim + 4));
        nextPrim = (u32 *)RCpolyF3(prim, &Other_OTEntryTriBuf);
    }
    return nextPrim;
}
/* Submit Gouraud triangle primitive (G3) to ordering list */
u32 *SubmitPolyG3(u32 *prim, int renderCtx)
{
    u32 *nextPrim;
    if (*(int *)(renderCtx + 0x78) == 0) {
        *prim = *prim & 0xff000000 | **(u32 **)(renderCtx + 0x30) & 0xffffff;
        u32 *nextPrim = prim + 7;
        **(u32 **)(renderCtx + 0x30) = **(u32 **)(renderCtx + 0x30) & 0xff000000 | (u32)prim & 0xffffff;
        return nextPrim;
    } else {
        SetupPrimOtEntry(&Other_OTEntryTriBuf, renderCtx, (u32)(prim + 1), 0, 0, 0);
        StoreRawVertices3((u32 *)(renderCtx + 0x88), (u32 *)(renderCtx + 0xa4), (u32)(prim + 2), (u32)(prim + 4), (u32)(prim + 6));
        *(ushort *)(*(int *)(renderCtx + 0x88) + 10) = (ushort)*(u8 *)((int)prim + 0xf);
        *(ushort *)(*(int *)(renderCtx + 0x8c) + 10) = (ushort)*(u8 *)((int)prim + 0xf);
        *(ushort *)(*(int *)(renderCtx + 0x90) + 10) = (ushort)*(u8 *)((int)prim + 0x17);
        /* Copy vertex colors to intermediate buffer */
        for (int i = 0; i < 3; i++) {
            int srcOff, dstBase;
            u32 val, tmp;
            uint alignSrc, alignDst;
            int addr;
            switch (i) {
                case 0:
                    srcOff = 7;
                    dstBase = *(int *)(renderCtx + 0x88);
                    break;
                case 1:
                    srcOff = 0xf;
                    dstBase = *(int *)(renderCtx + 0x8c);
                    break;
                case 2:
                    srcOff = 0x17;
                    dstBase = *(int *)(renderCtx + 0x90);
                    break;
            }
            alignSrc = ((int)prim + srcOff) & 3;
            alignDst = (u32)(prim + 1 + i * 2) & 3;
            tmp = (*(int *)(((int)prim + srcOff) - alignSrc) << (3 - alignSrc) * 8 |
                   tmp & 0xffffffffU >> (alignSrc + 1) * 8) & -1 << (4 - alignDst) * 8 |
                   *(uint *)((int)(prim + 1 + i * 2) - alignDst) >> alignDst * 8;
            addr = dstBase + 0xf;
            uint a = addr & 3;
            u32 *p = (u32 *)(addr - a);
            *p = *p & -1 << (a + 1) * 8 | tmp >> (3 - a) * 8;
            addr = dstBase + 0xc;
            a = addr & 3;
            p = (u32 *)(addr - a);
            *p = *p & 0xffffffffU >> (4 - a) * 8 | tmp << a * 8;
        }
        return (u32 *)RCpolyG3(prim, &Other_OTEntryTriBuf);
    }
}
/* Submit textured triangle primitive (FT3) to ordering list */
u32 *SubmitPolyFT3(u32 *prim, int renderCtx)
{
    u32 *nextPrim;
    if (*(int *)(renderCtx + 0x78) == 0) {
        *prim = *prim & 0xff000000 | **(u32 **)(renderCtx + 0x30) & 0xffffff;
        nextPrim = prim + 8;
        **(u32 **)(renderCtx + 0x30) = **(u32 **)(renderCtx + 0x30) & 0xff000000 | (u32)prim & 0xffffff;
    } else {
        SetupPrimOtEntry(&Other_OTEntryTriBuf, renderCtx, (u32)(prim + 1), 1,
                     *(u16 *)((int)prim + 0xe), *(u16 *)((int)prim + 0x16));
        StoreRawVertices3((u32 *)(renderCtx + 0x88), (u32 *)(renderCtx + 0xa4), (u32)(prim + 2), (u32)(prim + 4), (u32)(prim + 6));
        *(u16 *)(*(int *)(renderCtx + 0x88) + 10) = *(u16 *)((int)prim + 0x1e);
        *(u16 *)(*(int *)(renderCtx + 0x8c) + 10) = *(u16 *)((int)prim + 0x1e);
        *(u16 *)(*(int *)(renderCtx + 0x90) + 10) = *(u16 *)((int)prim + 0x1e);
        *(short *)(*(int *)(renderCtx + 0x88) + 8) = (short)prim[3];
        *(short *)(*(int *)(renderCtx + 0x8c) + 8) = (short)prim[5];
        *(short *)(*(int *)(renderCtx + 0x90) + 8) = (short)prim[7];
        nextPrim = (u32 *)RCpolyFT3(prim, &Other_OTEntryTriBuf);
    }
    return nextPrim;
}
/* Submit flat shaded textured quad (F4) to ordering list */
u32 *SubmitPolyF4(u32 *prim, int renderCtx)
{
    u32 *nextPrim;
    if (*(int *)(renderCtx + 0x78) == 0) {
        *prim = *prim & 0xff000000 | **(u32 **)(renderCtx + 0x30) & 0xffffff;
        nextPrim = prim + 6;
        **(u32 **)(renderCtx + 0x30) = **(u32 **)(renderCtx + 0x30) & 0xff000000 | (u32)prim & 0xffffff;
    } else {
        SetupPrimOtEntry(&Other_OTEntryQuadBuf, renderCtx, (u32)(prim + 1), 0, 0, 0);
        StoreRawVertices4((u32)(renderCtx + 0x94), (u32)(renderCtx + 0xa4), (u32)(prim + 2), (u32)(prim + 3), (u32)(prim + 4), (u32)(prim + 5));
        nextPrim = (u32 *)RCpolyF4(prim, &Other_OTEntryQuadBuf);
    }
    return nextPrim;
}
/* Submit gouraud textured quad (G4) to ordering list */
u32 *SubmitPolyG4(u32 *prim, int renderCtx)
{
    if (*(int *)(renderCtx + 0x78) == 0) {
        *prim = *prim & 0xff000000 | **(u32 **)(renderCtx + 0x30) & 0xffffff;
        u32 *nextPrim = prim + 9;
        **(u32 **)(renderCtx + 0x30) = **(u32 **)(renderCtx + 0x30) & 0xff000000 | (u32)prim & 0xffffff;
        return nextPrim;
    } else {
        SetupPrimOtEntry(&Other_OTEntryQuadBuf, renderCtx, (u32)(prim + 1), 0, 0, 0);
        StoreRawVertices4((u32)(renderCtx + 0x94), (u32)(renderCtx + 0xa4), (u32)(prim + 2), (u32)(prim + 4), (u32)(prim + 6), (u32)(prim + 8));
        /* Copy vertex colors to intermediate buffer */
        int srcBase = 7;
        int *dsts[4] = {
            (int *)(renderCtx + 0x94),
            (int *)(renderCtx + 0x98),
            (int *)(renderCtx + 0x9c),
            (int *)(renderCtx + 0xa0)
        };
        *(ushort *)(*dsts[0] + 10) = (ushort)*(u8 *)((int)prim + 7);
        *(ushort *)(*dsts[1] + 10) = (ushort)*(u8 *)((int)prim + 7);
        *(ushort *)(*dsts[2] + 10) = (ushort)*(u8 *)((int)prim + 0x17);
        *(ushort *)(*dsts[3] + 10) = (ushort)*(u8 *)((int)prim + 0x1f);
        for (int i = 0; i < 4; i++) {
            int srcOffs[] = { 7, 0xf, 0x17, 0x1f };
            int srcOff = srcOffs[i];
            int dstBase = *dsts[i];
            uint alignSrc = ((int)prim + srcOff) & 3;
            uint alignDst = (u32)(prim + 1 + i * 2) & 3;
            u32 tmp = (*(int *)(((int)prim + srcOff) - alignSrc) << (3 - alignSrc) * 8 |
                       tmp & 0xffffffffU >> (alignSrc + 1) * 8) & -1 << (4 - alignDst) * 8 |
                       *(uint *)((int)(prim + 1 + i * 2) - alignDst) >> alignDst * 8;
            int addr = dstBase + 0xf;
            uint a = addr & 3;
            u32 *p = (u32 *)(addr - a);
            *p = *p & -1 << (a + 1) * 8 | tmp >> (3 - a) * 8;
            addr = dstBase + 0xc;
            a = addr & 3;
            p = (u32 *)(addr - a);
            *p = *p & 0xffffffffU >> (4 - a) * 8 | tmp << a * 8;
        }
        return (u32 *)RCpolyG4(prim, &Other_OTEntryQuadBuf);
    }
}
/* Submit textured flat quad (FT4) to ordering list */
u32 *SubmitPolyFT4(u32 *prim, int renderCtx)
{
    u32 *nextPrim;
    if (*(int *)(renderCtx + 0x78) == 0) {
        *prim = *prim & 0xff000000 | **(u32 **)(renderCtx + 0x30) & 0xffffff;
        nextPrim = prim + 10;
        **(u32 **)(renderCtx + 0x30) = **(u32 **)(renderCtx + 0x30) & 0xff000000 | (u32)prim & 0xffffff;
    } else {
        SetupPrimOtEntry(&Other_OTEntryQuadBuf, renderCtx, (u32)(prim + 1), 1,
                     *(u16 *)((int)prim + 0xe), *(u16 *)((int)prim + 0x16));
        StoreRawVertices4((u32)(renderCtx + 0x94), (u32)(renderCtx + 0xa4), (u32)(prim + 2), (u32)(prim + 4), (u32)(prim + 6), (u32)(prim + 8));
        *(u16 *)(*(int *)(renderCtx + 0x94) + 10) = *(u16 *)((int)prim + 0x1e);
        *(u16 *)(*(int *)(renderCtx + 0x98) + 10) = *(u16 *)((int)prim + 0x1e);
        *(u16 *)(*(int *)(renderCtx + 0x9c) + 10) = *(u16 *)((int)prim + 0x1e);
        *(u16 *)(*(int *)(renderCtx + 0xa0) + 10) = *(u16 *)((int)prim + 0x1e);
        *(short *)(*(int *)(renderCtx + 0x94) + 8) = (short)prim[3];
        *(short *)(*(int *)(renderCtx + 0x98) + 8) = (short)prim[5];
        *(short *)(*(int *)(renderCtx + 0x9c) + 8) = (short)prim[7];
        *(short *)(*(int *)(renderCtx + 0xa0) + 8) = (short)prim[9];
        nextPrim = (u32 *)RCpolyFT4(prim, &Other_OTEntryQuadBuf);
    }
    return nextPrim;
}
/* Submit gouraud textured triangle (GT3) to ordering list */
u32 *SubmitPolyGT3(u32 *prim, int renderCtx)
{
    if (*(int *)(renderCtx + 0x78) == 0) {
        *prim = *prim & 0xff000000 | **(u32 **)(renderCtx + 0x30) & 0xffffff;
        u32 *nextPrim = prim + 10;
        **(u32 **)(renderCtx + 0x30) = **(u32 **)(renderCtx + 0x30) & 0xff000000 | (u32)prim & 0xffffff;
        return nextPrim;
    } else {
        SetupPrimOtEntry(&Other_OTEntryTriBuf, renderCtx, (u32)(prim + 1), 1,
                     *(u16 *)((int)prim + 0xe), *(u16 *)((int)prim + 0x1a));
        StoreRawVertices3((u32 *)(renderCtx + 0x88), (u32 *)(renderCtx + 0xa4), (u32)(prim + 2), (u32)(prim + 5), (u32)(prim + 8));
        *(u16 *)(*(int *)(renderCtx + 0x88) + 10) = *(u16 *)((int)prim + 0x26);
        *(u16 *)(*(int *)(renderCtx + 0x8c) + 10) = *(u16 *)((int)prim + 0x26);
        int dstBase = *(int *)(renderCtx + 0x90);
        *(u16 *)(dstBase + 10) = *(u16 *)((int)prim + 0x26);
        /* Copy vertex data through misaligned memory */
        for (int i = 0; i < 3; i++) {
            int srcOffs[] = { 7, 0x13, 0x1f };
            int srcOff = srcOffs[i];
            int dstBases[] = {
                *(int *)(renderCtx + 0x88),
                *(int *)(renderCtx + 0x8c),
                *(int *)(renderCtx + 0x90)
            };
            dstBase = dstBases[i];
            uint alignSrc = ((int)prim + srcOff) & 3;
            uint alignDst = (u32)(prim + 1 + i * (i == 0 ? 0 : (i == 1 ? 3 : 6))) & 3;
            u32 tmp = (*(int *)(((int)prim + srcOff) - alignSrc) << (3 - alignSrc) * 8 |
                       tmp & 0xffffffffU >> (alignSrc + 1) * 8) & -1 << (4 - alignDst) * 8 |
                       *(uint *)((int)(prim + 1 + i * (i == 0 ? 0 : (i == 1 ? 3 : 6))) - alignDst) >> alignDst * 8;
            int addr = dstBase + 0xf;
            uint a = addr & 3;
            u32 *p = (u32 *)(addr - a);
            *p = *p & -1 << (a + 1) * 8 | tmp >> (3 - a) * 8;
            addr = dstBase + 0xc;
            a = addr & 3;
            p = (u32 *)(addr - a);
            *p = *p & 0xffffffffU >> (4 - a) * 8 | tmp << a * 8;
        }
        *(short *)(*(int *)(renderCtx + 0x88) + 8) = (short)prim[3];
        *(short *)(*(int *)(renderCtx + 0x8c) + 8) = (short)prim[6];
        *(short *)(*(int *)(renderCtx + 0x90) + 8) = (short)prim[9];
        return (u32 *)RCpolyGT3(prim, &Other_OTEntryTriBuf);
    }
}
/* Submit gouraud textured quad (GT4) to ordering list */
u32 *SubmitPolyGT4(u32 *prim, int renderCtx)
{
    if (*(int *)(renderCtx + 0x78) == 0) {
        *prim = *prim & 0xff000000 | **(u32 **)(renderCtx + 0x30) & 0xffffff;
        u32 *nextPrim = prim + 0xd;
        **(u32 **)(renderCtx + 0x30) = **(u32 **)(renderCtx + 0x30) & 0xff000000 | (u32)prim & 0xffffff;
        return nextPrim;
    } else {
        SetupPrimOtEntry(&Other_OTEntryQuadBuf, renderCtx, (u32)(prim + 1), 1,
                     *(u16 *)((int)prim + 0xe), *(u16 *)((int)prim + 0x1a));
        StoreRawVertices4((u32)(renderCtx + 0x94), (u32)(renderCtx + 0xa4), (u32)(prim + 2), (u32)(prim + 5), (u32)(prim + 8), (u32)(prim + 0xb));
        *(u16 *)(*(int *)(renderCtx + 0x94) + 10) = *(u16 *)((int)prim + 0x26);
        *(u16 *)(*(int *)(renderCtx + 0x98) + 10) = *(u16 *)((int)prim + 0x26);
        *(u16 *)(*(int *)(renderCtx + 0x9c) + 10) = *(u16 *)((int)prim + 0x32);
        *(u16 *)(*(int *)(renderCtx + 0xa0) + 10) = *(u16 *)((int)prim + 0x32);
        /* Copy vertex data for all 4 vertices */
        for (int i = 0; i < 4; i++) {
            int srcOffs[] = { 7, 0x13, 0x1f, 0x2b };
            int srcOff = srcOffs[i];
            int dstBases[] = {
                *(int *)(renderCtx + 0x94),
                *(int *)(renderCtx + 0x98),
                *(int *)(renderCtx + 0x9c),
                *(int *)(renderCtx + 0xa0)
            };
            int dstBase = dstBases[i];
            int primOff = (prim + 1 + i * (i == 0 ? 0 : (i == 1 ? 3 : (i == 2 ? 6 : 9))));
            uint alignSrc = ((int)prim + srcOff) & 3;
            uint alignDst = (u32)primOff & 3;
            u32 tmp = (*(int *)(((int)prim + srcOff) - alignSrc) << (3 - alignSrc) * 8 |
                       tmp & 0xffffffffU >> (alignSrc + 1) * 8) & -1 << (4 - alignDst) * 8 |
                       *(uint *)((int)primOff - alignDst) >> alignDst * 8;
            int addr = dstBase + 0xf;
            uint a = addr & 3;
            u32 *p = (u32 *)(addr - a);
            *p = *p & -1 << (a + 1) * 8 | tmp >> (3 - a) * 8;
            addr = dstBase + 0xc;
            a = addr & 3;
            p = (u32 *)(addr - a);
            *p = *p & 0xffffffffU >> (4 - a) * 8 | tmp << a * 8;
        }
        *(short *)(*(int *)(renderCtx + 0x94) + 8) = (short)prim[3];
        *(short *)(*(int *)(renderCtx + 0x98) + 8) = (short)prim[6];
        *(short *)(*(int *)(renderCtx + 0x9c) + 8) = (short)prim[9];
        *(short *)(*(int *)(renderCtx + 0xa0) + 8) = (short)prim[0xc];
        return (u32 *)RCpolyGT4(prim, &Other_OTEntryQuadBuf);
    }
}
/* Initialize OT entry pointers for quad/triangle bucket lists */
void InitOTU_Ptrs(int *bucketList, int base, int count)
{
    int i;
    int *ptr;
    i = base + 0x18;
    if (count == 4) {
        ptr = (int *)(base + 0xf0);
    } else {
        ptr = (int *)(base + 0xa8);
    }
    while (count > 0) {
        *ptr = i;
        *bucketList = i;
        i += 0x18;
        ptr++;
        bucketList++;
        count--;
    }
}
/* Update screen-space bounding box and detect oversized primitives */
void UpdateBoundingBox(int renderCtx, int vertexCount)
{
    uint alignSrc, alignDst;
    u32 *dst;
    u32 packed;
    short *pX, *pY;
    /* Pack sx[0], sy[0] into renderCtx[0x63] */
    packed = 0;
    {
        uint a1 = (renderCtx + 0x63) & 3;
        uint a2 = (renderCtx + 0x60) & 3;
        packed = (*(int *)((renderCtx + 0x63) - a1) << (3 - a1) * 8 |
                  packed & 0xffffffffU >> (a1 + 1) * 8) & -1 << (4 - a2) * 8 |
                  *(uint *)((renderCtx + 0x60) - a2) >> a2 * 8;
    }
    uint a = (renderCtx + 0x77) & 3;
    dst = (u32 *)((renderCtx + 0x77) - a);
    *dst = *dst & -1 << (a + 1) * 8 | packed >> (3 - a) * 8;
    a = (renderCtx + 0x74) & 3;
    dst = (u32 *)((renderCtx + 0x74) - a);
    *dst = *dst & 0xffffffffU >> (4 - a) * 8 | packed << a * 8;
    /* Calculate sy[0] from local */
    {
    uint a1 = (renderCtx + 0x77) & 3;
    uint a2 = (renderCtx + 0x74) & 3;
    packed = (*(int *)((renderCtx + 0x77) - a1) << (3 - a1) * 8 |
              packed & 0xffffffffU >> (a1 + 1) * 8) & -1 << (4 - a2) * 8 |
              *(uint *)((renderCtx + 0x74) - a2) >> a2 * 8;
    }
    a = (renderCtx + 0x73) & 3;
    dst = (u32 *)((renderCtx + 0x73) - a);
    *dst = *dst & -1 << (a + 1) * 8 | packed >> (3 - a) * 8;
    a = (renderCtx + 0x70) & 3;
    dst = (u32 *)((renderCtx + 0x70) - a);
    *dst = *dst & 0xffffffffU >> (4 - a) * 8 | packed << a * 8;
    pX = (short *)(renderCtx + 100);
    short *pXEnd = (short *)(renderCtx + vertexCount * 4 + 0x5c);
    pY = (short *)(renderCtx + 0x66);
    if (pX < pXEnd) {
        do {
            if (*pX < *(short *)(renderCtx + 0x70)) {
                *(short *)(renderCtx + 0x70) = *pX;
            }
            if (*pY < *(short *)(renderCtx + 0x72)) {
                *(short *)(renderCtx + 0x72) = *pY;
            }
            if (*(short *)(renderCtx + 0x74) < *pX) {
                *(short *)(renderCtx + 0x74) = *pX;
            }
            if (*(short *)(renderCtx + 0x76) < *pY) {
                *(short *)(renderCtx + 0x76) = *pY;
            }
            pX += 2;
            pY += 2;
        } while (pX < pXEnd);
    }
    /* If bounding box exceeds 256 pixels in either axis, mark as oversized */
    if (*(short *)(renderCtx + 0x74) - *(short *)(renderCtx + 0x70) > SPRITE_DIM_THRESHOLD) {
        *(int *)(renderCtx + 0x78) = 1;
    }
    if (*(short *)(renderCtx + 0x76) - *(short *)(renderCtx + 0x72) > SPRITE_DIM_THRESHOLD) {
        *(int *)(renderCtx + 0x78) = 1;
    }
}
/* WARNING: Removing unreachable block (ram,0x8001a3a4) */
/* Set up primitive attributes for OT sorting: */
/* screen dimensions, UV offset, OT table pointer */
void SetupPrimitiveOTEntry(u32 *otEntry, int renderCtx, u32 primPtr, int hasUV, u16 uvX, u16 uvY)
{
    uint alignSrc, alignDst;
    u32 *dst;
    otEntry[0] = 2;
    otEntry[1] = OT_PRIORITY_DEFAULT;
    otEntry[2] = 0xf0;
    if (hasUV != 0) {
        *(u16 *)(otEntry + 3) = uvX;
        *(u16 *)((int)otEntry + 0xe) = uvY;
    }
    /* Store primitive pointer (misaligned copy) */
    alignSrc = (primPtr + 3) & 3;
    alignDst = primPtr & 3;
    u32 val = (*(int *)((primPtr + 3) - alignSrc) << (3 - alignSrc) * 8 |
               0xffffffffU >> (alignSrc + 1) * 8 & OT_PRIORITY_DEFAULT) & -1 << (4 - alignDst) * 8 |
               *(uint *)(primPtr - alignDst) >> alignDst * 8;
    alignSrc = ((int)otEntry + 0x13) & 3;
    dst = (u32 *)(((int)otEntry + 0x13) - alignSrc);
    *dst = *dst & -1 << (alignSrc + 1) * 8 | val >> (3 - alignSrc) * 8;
    alignSrc = (u32)(otEntry + 4) & 3;
    dst = (u32 *)((int)(otEntry + 4) - alignSrc);
    *dst = *dst & 0xffffffffU >> (4 - alignSrc) * 8 | val << alignSrc * 8;
    otEntry[5] = *(u32 *)(renderCtx + 0x30);
}
/* Store 3 vertex screen positions (misaligned writes) into ordering list */
void StoreVertices3(int *dst, u32 *src, u32 v0_val, u32 v1, u32 v2)
{
    u32 tmp1, tmp2, tmp3;
    uint a1, a2;
    u32 *p;
    u32 d0 = *dst;
    u32 s0 = *src;
    /* Write z0 */
    a1 = (s0 + 3) & 3;
    a2 = s0 & 3;
    tmp1 = (*(int *)((s0 + 3) - a1) << (3 - a1) * 8 | v0_val & 0xffffffffU >> (a1 + 1) * 8) & -1 << (4 - a2) * 8 | *(uint *)(s0 - a2) >> a2 * 8;
    a1 = (s0 + 7) & 3;
    a2 = (s0 + 4) & 3;
    tmp2 = (*(int *)((s0 + 7) - a1) << (3 - a1) * 8 | v1 & 0xffffffffU >> (a1 + 1) * 8) & -1 << (4 - a2) * 8 | *(uint *)((s0 + 4) - a2) >> a2 * 8;
    a1 = (d0 + 3) & 3;
    p = (u32 *)((d0 + 3) - a1);
    *p = *p & -1 << (a1 + 1) * 8 | tmp1 >> (3 - a1) * 8;
    a1 = d0 & 3;
    *(u32 *)(d0 - a1) = *(u32 *)(d0 - a1) & 0xffffffffU >> (4 - a1) * 8 | tmp1 << a1 * 8;
    a1 = (d0 + 7) & 3;
    p = (u32 *)((d0 + 7) - a1);
    *p = *p & -1 << (a1 + 1) * 8 | tmp2 >> (3 - a1) * 8;
    a1 = (d0 + 4) & 3;
    p = (u32 *)((d0 + 4) - a1);
    *p = *p & 0xffffffffU >> (4 - a1) * 8 | tmp2 << a1 * 8;
    /* z1 */
    d0 = dst[1];
    s0 = src[1];
    a1 = (s0 + 3) & 3;
    a2 = s0 & 3;
    tmp3 = (*(int *)((s0 + 3) - a1) << (3 - a1) * 8 | tmp1 & 0xffffffffU >> (a1 + 1) * 8) & -1 << (4 - a2) * 8 | *(uint *)(s0 - a2) >> a2 * 8;
    a1 = (s0 + 7) & 3;
    a2 = (s0 + 4) & 3;
    tmp2 = (*(int *)((s0 + 7) - a1) << (3 - a1) * 8 | tmp2 & 0xffffffffU >> (a1 + 1) * 8) & -1 << (4 - a2) * 8 | *(uint *)((s0 + 4) - a2) >> a2 * 8;
    a1 = (d0 + 3) & 3;
    p = (u32 *)((d0 + 3) - a1);
    *p = *p & -1 << (a1 + 1) * 8 | tmp3 >> (3 - a1) * 8;
    a1 = d0 & 3;
    *(u32 *)(d0 - a1) = *(u32 *)(d0 - a1) & 0xffffffffU >> (4 - a1) * 8 | tmp3 << a1 * 8;
    a1 = (d0 + 7) & 3;
    p = (u32 *)((d0 + 7) - a1);
    *p = *p & -1 << (a1 + 1) * 8 | tmp2 >> (3 - a1) * 8;
    a1 = (d0 + 4) & 3;
    p = (u32 *)((d0 + 4) - a1);
    *p = *p & 0xffffffffU >> (4 - a1) * 8 | tmp2 << a1 * 8;
    /* z2 */
    d0 = dst[2];
    s0 = src[2];
    a1 = (s0 + 3) & 3;
    a2 = s0 & 3;
    tmp1 = (*(int *)((s0 + 3) - a1) << (3 - a1) * 8 | (u32)src & 0xffffffffU >> (a1 + 1) * 8) & -1 << (4 - a2) * 8 | *(uint *)(s0 - a2) >> a2 * 8;
    a1 = (s0 + 7) & 3;
    a2 = (s0 + 4) & 3;
    tmp2 = (*(int *)((s0 + 7) - a1) << (3 - a1) * 8 | tmp3 & 0xffffffffU >> (a1 + 1) * 8) & -1 << (4 - a2) * 8 | *(uint *)((s0 + 4) - a2) >> a2 * 8;
    a1 = (d0 + 3) & 3;
    p = (u32 *)((d0 + 3) - a1);
    *p = *p & -1 << (a1 + 1) * 8 | tmp1 >> (3 - a1) * 8;
    a1 = d0 & 3;
    *(u32 *)(d0 - a1) = *(u32 *)(d0 - a1) & 0xffffffffU >> (4 - a1) * 8 | tmp1 << a1 * 8;
    a1 = (d0 + 7) & 3;
    p = (u32 *)((d0 + 7) - a1);
    *p = *p & -1 << (a1 + 1) * 8 | tmp2 >> (3 - a1) * 8;
    a1 = (d0 + 4) & 3;
    p = (u32 *)((d0 + 4) - a1);
    *p = *p & 0xffffffffU >> (4 - a1) * 8 | tmp2 << a1 * 8;
    /* Store z values to target with UV */
    d0 = *dst;
    s0 = src[0]; /* UVX start */
    a1 = v0_val + 3 & 3;
    a2 = v0_val & 3;
    tmp1 = (*(int *)((v0_val + 3) - a1) << (3 - a1) * 8 | s0 & 0xffffffffU >> (a1 + 1) * 8) & -1 << (4 - a2) * 8 | *(uint *)(v0_val - a2) >> a2 * 8;
    a1 = (d0 + 0x13) & 3;
    p = (u32 *)((d0 + 0x13) - a1);
    *p = *p & -1 << (a1 + 1) * 8 | tmp1 >> (3 - a1) * 8;
    a1 = (d0 + 0x10) & 3;
    p = (u32 *)((d0 + 0x10) - a1);
    *p = *p & 0xffffffffU >> (4 - a1) * 8 | tmp1 << a1 * 8;
    /* z1 -> UV */
    d0 = dst[1];
    a1 = v1 + 3 & 3;
    a2 = v1 & 3;
    tmp1 = (*(int *)((v1 + 3) - a1) << (3 - a1) * 8 | tmp1 & 0xffffffffU >> (a1 + 1) * 8) & -1 << (4 - a2) * 8 | *(uint *)(v1 - a2) >> a2 * 8;
    a1 = (d0 + 0x13) & 3;
    p = (u32 *)((d0 + 0x13) - a1);
    *p = *p & -1 << (a1 + 1) * 8 | tmp1 >> (3 - a1) * 8;
    a1 = (d0 + 0x10) & 3;
    p = (u32 *)((d0 + 0x10) - a1);
    *p = *p & 0xffffffffU >> (4 - a1) * 8 | tmp1 << a1 * 8;
    /* z2 -> UV */
    d0 = dst[2];
    a1 = v2 + 3 & 3;
    a2 = v2 & 3;
    tmp1 = (*(int *)((v2 + 3) - a1) << (3 - a1) * 8 | tmp1 & 0xffffffffU >> (a1 + 1) * 8) & -1 << (4 - a2) * 8 | *(uint *)(v2 - a2) >> a2 * 8;
    a1 = (d0 + 0x13) & 3;
    p = (u32 *)((d0 + 0x13) - a1);
    *p = *p & -1 << (a1 + 1) * 8 | tmp1 >> (3 - a1) * 8;
    a1 = (d0 + 0x10) & 3;
    p = (u32 *)((d0 + 0x10) - a1);
    *p = *p & 0xffffffffU >> (4 - a1) * 8 | tmp1 << a1 * 8;
}
/* Store 4 vertex screen positions with UV: wrapper around StoreVertices3 with 4th vertex */
void StoreVertices4(u32 vertData1, u32 vertData2)
{
    StoreRawVertices3();
    u32 s0 = *(u32 *)(vertData1 + 0xc);
    u32 s1 = *(u32 *)(vertData2 + 0xc);
    uint a1 = (s1 + 3) & 3;
    uint a2 = s1 & 3;
    u32 tmp1 = (*(int *)((s1 + 3) - a1) << (3 - a1) * 8 | vertData1 & 0xffffffffU >> (a1 + 1) * 8) & -1 << (4 - a2) * 8 | *(uint *)(s1 - a2) >> a2 * 8;
    a1 = (s1 + 7) & 3;
    a2 = (s1 + 4) & 3;
    u32 tmp2 = (*(int *)((s1 + 7) - a1) << (3 - a1) * 8 | vertData2 & 0xffffffffU >> (a1 + 1) * 8) & -1 << (4 - a2) * 8 | *(uint *)((s1 + 4) - a2) >> a2 * 8;
    a1 = (s0 + 3) & 3;
    u32 *p = (u32 *)((s0 + 3) - a1);
    *p = *p & -1 << (a1 + 1) * 8 | tmp1 >> (3 - a1) * 8;
    a1 = s0 & 3;
    *(u32 *)(s0 - a1) = *(u32 *)(s0 - a1) & 0xffffffffU >> (4 - a1) * 8 | tmp1 << a1 * 8;
    a1 = (s0 + 7) & 3;
    p = (u32 *)((s0 + 7) - a1);
    *p = *p & -1 << (a1 + 1) * 8 | tmp2 >> (3 - a1) * 8;
    a1 = (s0 + 4) & 3;
    p = (u32 *)((s0 + 4) - a1);
    *p = *p & 0xffffffffU >> (4 - a1) * 8 | tmp2 << a1 * 8;
    u32 extraParam4;
    int iVar4 = *(int *)(vertData1 + 0xc);
    a1 = (u32)&extraParam4 + 3 & 3;
    a2 = (u32)&extraParam4 & 3;
    u32 tmp = (*(int *)(((u32)&extraParam4 + 3) - a1) << (3 - a1) * 8 |
               tmp & 0xffffffffU >> (a1 + 1) * 8) & -1 << (4 - a2) * 8 |
               *(uint *)((u32)&extraParam4 - a2) >> a2 * 8;
    a1 = (iVar4 + 0x13) & 3;
    p = (u32 *)((iVar4 + 0x13) - a1);
    *p = *p & -1 << (a1 + 1) * 8 | tmp1 >> (3 - a1) * 8;
    a1 = (iVar4 + 0x10) & 3;
    p = (u32 *)((iVar4 + 0x10) - a1);
    *p = *p & 0xffffffffU >> (4 - a1) * 8 | tmp1 << a1 * 8;
}
/* Create and initialize a sound effect object type */
int CreateSoundEffect(void)
{
    int obj;
    int vtable;
    obj = MemAlloc(0x44, 0);
    vtable = 0;
    if (obj != 0) {
        vtable = GetSoundEffectVtablePtr();
        int initResult = (*(code *)(vtable + 8))(obj);
        if (initResult == 0) {
            MemFree((int)obj, 0);
            vtable = 0;
        } else {
            vtable = obj;
        }
    }
    return vtable;
}
/* Recursive BSP tree subdivision for collision detection */
/* Splits an edge based on plane tests, bisection midpoint approach */
int BSPSubdivideEdge(int mode, u32 planeDef, short *pointA, short *pointB)
{
    u8 resultA, resultB;
    int midX, midY, midZ;
    short mid[3];
    resultA = BSPClassifyPoint(planeDef, pointA);
    resultB = BSPClassifyPoint(planeDef, pointB);
    if (resultA == 0) {
        if (resultB != 0) {
            goto SWAP_AB;
        }
    } else if (resultB == 0) {
        /* A outside, B inside: split and swap */
        if (mode != 0) {
            SubdivideSearch(mode, planeDef, pointB, pointA);
        }
        return 3;
    }
    if (resultA == 0) {
        /* A inside, B outside: split */
        if (mode != 0) {
            SubdivideSearch(mode, planeDef, pointA, pointB);
        }
        return 2;
    } else {
        /* Both inside: bisect */
        if ((resultA & resultB) != 0) return 0;
        midX = ((int)*pointA + (int)*pointB) >> 1;
        mid[0] = (short)midX;
        midY = ((int)pointA[1] + (int)pointB[1]) >> 1;
        mid[1] = (short)midY;
        midZ = ((int)pointA[2] + (int)pointB[2]) >> 1;
        mid[2] = (short)midZ;
        /* Check if midpoint != both endpoints (i.e. further subdivision possible) */
        if (((*pointA != midX) || (pointA[1] != midY) || (pointA[2] != midZ)) &&
            (((*pointB != mid[0]) || (pointB[1] != mid[1]) || (pointB[2] != mid[2])) &&
              (BSPSubdivideEdge(mode, planeDef, pointA, mid) == 0 &&
               BSPSubdivideEdge(mode, planeDef, mid, pointB) == 0))) {
            return 0;
        }
        return 0;
    }
SWAP_AB:
    if (mode != 0) {
        SubdivideSearch(mode, planeDef, pointB, pointA);
    }
    return 3;
}
/* Subdivide an edge using midpoint and plane classification */
/* Computes midpoint of two vertices, tests plane, and recurses */
void EdgeSubdivideMid(short *midpoint, short *planeDef, short *v0_val, u32 *v1)
{
    short s;
    int i;
    u32 flags;
    u32 *pM0, *pM1;
    u32 tmp[2];
    while (1) {
        midpoint[0] = (short)(((int)*v0_val + (int)(short)*v1) >> 1);
        midpoint[1] = (short)(((int)v0_val[1] + (int)*(short *)((int)v1 + 2)) >> 1);
        i = ((int)v0_val[2] + (int)(short)v1[1]) >> 1;
        midpoint[2] = (short)i;
        /* Check if midpoint equals either endpoint (converged) */
        if (((midpoint[0] == *v0_val) && (midpoint[1] == v0_val[1]) && (i == v0_val[2])) ||
            ((midpoint[0] == (short)*v1) && (midpoint[1] == *(short *)((int)v1 + 2)) &&
             (midpoint[2] == (short)v1[1]))) {
            break;
        }
        /* Classify midpoint against plane */
        flags = 0;
        if (planeDef[3] < midpoint[0]) {
            flags = 8;
        } else if (midpoint[0] < planeDef[0]) {
            flags = 4;
        }
        if (planeDef[4] < midpoint[1]) {
            flags |= 2;
        } else if (midpoint[1] < planeDef[1]) {
            flags |= 1;
        }
        if (planeDef[5] < midpoint[2]) {
            flags |= 0x20;
        } else if (midpoint[2] < planeDef[2]) {
            flags |= 0x10;
        }
        pM0 = tmp;
        pM1 = tmp;
        if (flags != 0) {
            pM0 = (u32 *)v0_val;
            v1 = tmp;
            pM1 = tmp;
        }
        uint a = ((int)midpoint + 3) & 3;
        uint b = (u32)midpoint & 3;
        uint flags2 = (*(int *)(((int)midpoint + 3) - a) << (3 - a) * 8 |
                        flags & 0xffffffffU >> (a + 1) * 8) & -1 << (4 - b) * 8 |
                        *(uint *)((int)midpoint - b) >> b * 8;
        s = midpoint[2];
        b = ((int)pM1 + 3) & 3;
        u32 *p = (u32 *)(((int)pM1 + 3) - b);
        *p = *p & -1 << (b + 1) * 8 | flags2 >> (3 - b) * 8;
        *pM1 = flags2;
        *(short *)(pM1 + 1) = s;
        v0_val = (short *)pM0;
    }
}
/* vtable getter for sound effect object (alternate vtable) */
void *GetVtable_SoundEffect2(void)
{
    return &Other_SoundEffectVtableAlt;
}
/* Transform world position to local coordinates using object's matrix */
/* Reads transform matrix via vtable, applies inverse */
void WorldToLocalCoord(int *obj, int *outPos, short *worldPos)
{
    MATRIX mtx;
    (*(code *)(*obj + 0x84))(obj, &mtx, 0);
    outPos[0] = (int)worldPos[0];
    outPos[1] = (int)worldPos[1];
    outPos[2] = (int)worldPos[2];
    ApplyMatrixLV(&mtx, (VECTOR *)outPos, (VECTOR *)outPos);
}
/* Transform world position with object offset and store to outPos */
/* Adds object's world-space position after matrix transform */
void WorldToLocalWithOffset(int *obj, int *outPos, u32 worldPos)
{
    MATRIX mtx;
    int *offset;
    (*(code *)(*obj + 0x84))(obj, &mtx, 0);
    ApplyMatrixLV(&mtx, (VECTOR *)outPos, (VECTOR *)worldPos);
    if (obj[3] != 0) {
        offset = (int *)(obj[5] + 0x38);
    } else {
        offset = NULL;
    }
    outPos[0] += *offset;
    outPos[1] += offset[1];
    outPos[2] += offset[2];
}
/* Convert world-space dimensions to screen-space based on camera distance */
/* Uses perspective: screenSize = (worldSize * 0x2d) >> 9 */
void WorldToScreenSize(int camCtx, short *outSize)
{
    short s;
    int baseAddr;
    baseAddr = *(int *)(*(int *)(camCtx + 0x14) + 0x44);
    s = *(short *)(baseAddr + 0x10);
    outSize[1] = 1;
    outSize[0] = (short)(s * 0x2d >> 9);
    s = *(short *)(baseAddr + 0x12);
    outSize[3] = 1;
    outSize[2] = (short)(s * 0x2d >> 9);
    s = *(short *)(baseAddr + 0x14);
    outSize[5] = 1;
    outSize[4] = (short)(s * 0x2d >> 9);
}
/* Link a child model to a parent model's display object */
void LinkModelToParent(int childModel, int parentModel)
{
    *(int *)(childModel + 0x20) = parentModel;
    *(u32 *)(childModel + 0x18) = *(u32 *)(parentModel + 0x10);
    GsLinkObject4(*(int *)(*(int *)(childModel + 0x20) + 0xc) + 0xc, (GsDOBJ2 *)(childModel + 0x10), 0);
}
/* Unlink model: clear both parent and display object links */
void UnlinkModel(int childModel)
{
    *(u32 *)(childModel + 0x18) = 0;
    *(u32 *)(childModel + 0x20) = 0;
}
/* Perform collision raycast from entity towards target */
/* Returns whether a collision was found */
u32 DoCollisionCheck(EntityObj *entity, u32 outHit, short *targetPos)
{
    int i;
    int *iter;
    int x, y, z;
    short localPos[3];
    short rot[3];
    short worldOffset[3];
    short *worldPos;
    if (entity->nField_20 == 0) {
        return 0;
    }
    if (entity->nDist >= 0) goto SKIP_OFFSET_UPDATE;
    /* Update world-space position offset from object hierarchy */
    {
        int basePtr;
        if (entity->nPosZ != 0) {
            basePtr = entity->pData;
            if (basePtr != -0x38) {
                *(u32 *)(basePtr + 0x38) = *(u32 *)(basePtr + 0x18);
                *(u32 *)(basePtr + 0x3c) = *(u32 *)(basePtr + 0x1c);
                *(u32 *)(basePtr + 0x40) = *(u32 *)(basePtr + 0x20);
                for (iter = (int *)entity->nPosZ; iter != 0; iter = (int *)iter[3]) {
                    if (entity->nPosZ != 0) {
                        basePtr = entity->pData + 0x38;
                    } else {
                        basePtr = 0;
                    }
                    int objData = 0;
                    if (entity->nPosZ != 0) {
                        objData = entity->pData + 0x38;
                    }
                    *(int *)(basePtr + 0) = *(int *)(objData + 0) + *(int *)(*(int *)(iter + 5) + 0x18);
                    *(int *)(basePtr + 4) = *(int *)(objData + 4) + *(int *)(*(int *)(iter + 5) + 0x1c);
                    *(int *)(basePtr + 8) = *(int *)(objData + 8) + *(int *)(*(int *)(iter + 5) + 0x20);
                }
            }
        }
    }
SKIP_OFFSET_UPDATE:
    if (entity->nPosZ == 0) {
        worldPos = 0;
    } else {
        worldPos = (short *)(entity->pData + 0x38);
    }
    localPos[0] = *targetPos - *worldPos;
    localPos[1] = targetPos[2] - worldPos[2];
    localPos[2] = targetPos[4] - worldPos[4];
    (*(code *)(*(int *)entity + 0xa4))(entity, 0, rot, localPos, 1);
    localPos[0] = rot[0];
    localPos[2] = rot[2];
    localPos[1] = rot[1] - DEGREES_90;
    int hit = UnknownOtHelper(entity->nField_20, (short *)outHit, localPos, 0, rot, localPos);
    if (hit == 0) {
        localPos[1] = rot[1] + DEGREES_90;
        hit = UnknownOtHelper(entity->nField_20, (short *)outHit, localPos, 0, &rot, localPos);
        if (hit == 0) {
            return 0;
        }
    }
    Vector3Subtract(outHit, rot, localPos);
    return 1;
}
/* Compute difference vector between two short positions */
void Vec3Diff(int *out, short *a, short *b)
{
    out[0] = (int)*b - (int)*a;
    out[1] = (int)b[1] - (int)a[1];
    out[2] = (int)b[2] - (int)a[2];
}
/* Rotate entity towards a target position using angles */
/* Sets rotation via vtable call */
void RotateToTarget(EntityObj *entity, int targetObj, int useXZ, int useY, int extraRot)
{
    int origX, origY, origZ;
    long angle;
    int diffX, diffY, diffZ;
    u16 rotX, rotY;
    int thisBase = entity->pData;
    int *targetBase;
    int diff;
    if (*(int *)(targetObj + 0xc) == 0) {
        targetBase = 0;
    } else {
        targetBase = (int *)(*(int *)(*(int *)(targetObj + 0x14) + 0x38));
    }
    diffX = targetBase[0] - *(int *)(thisBase + 0x18);
    if (diffX == 0) {
        diffY = targetBase[2];
        diffZ = *(int *)(thisBase + 0x20);
        diff = 1;
    } else {
        diffY = targetBase[2];
        diffZ = *(int *)(thisBase + 0x20);
    }
    diff = ratan2(diff, diffY - diffZ);
    rotY = (short)diff;
    diff = targetBase[2] - *(int *)(thisBase + 0x20);
    if (diff == 0) {
        diffY = targetBase[1];
        diffZ = *(int *)(thisBase + 0x1c);
        diff = 1;
    } else {
        diffY = targetBase[1];
        diffZ = *(int *)(thisBase + 0x1c);
    }
    diff = ratan2(diff, diffY - diffZ);
    rotX = ((short)diff + DEGREES_90) * ANGLE_DIV_360;
    if ((int)rotX < 0) rotX += ANGLE_NORMALIZE;
    rotX >>= 0xc;
    rotY = rotY * ANGLE_DIV_360;
    if ((int)rotY < 0) rotY += ANGLE_NORMALIZE;
    rotY >>= 0xc;
    u16 pitch = 0;
    u16 yaw = 1;
    u16 roll = 1;
    u16 pitchToSet = 1;
    u16 yawToSet = 1;
    if (useXZ != 0) {
        pitchToSet = 0;
    }
    if (useY == 0) {
        yawToSet = yawToSet + 0xb4;
    }
    (*(code *)(*(int *)entity + 0x44))(entity, 1, &rotX);
    if (extraRot != 0) {
        (*(code *)(*(int *)entity + 0x44))(entity, 0, extraRot);
    }
}
/* Fixed point modulo division returning packed Q12 value */
int FixedModDiv(short *val)
{
    int a = (int)*val;
    int b = (int)val[1];
    int quot = a / b;
    int rem = a % b;
    return (quot << 12) + (rem / b) * FIXED_12;
}
/* Classify a point against an axis-aligned bounding box */
/* Returns bitmask: bits 0-1 Y, 2-3 X, 4-5 Z */
u32 ClassifyPointAABB(short *aabb, short *point)
{
    u32 code = 0;
    if (aabb[3] < *point) {
        code = 8;
    } else if (*point < aabb[0]) {
        code = 4;
    }
    if (aabb[4] < point[1]) {
        code |= 2;
    } else if (point[1] < aabb[1]) {
        code |= 1;
    }
    if (aabb[5] < point[2]) {
        code |= 0x20;
    } else if (point[2] < aabb[2]) {
        code |= 0x10;
    }
    return code;
}
/* Extract N-bit field from a 32-bit value */
/* Reads/writes a signed bitfield from *field at bitPos */
u32 BitFieldExtract(u32 *field, u32 bitPos, int numBits, int newVal)
{
    int shift = 1;
    int i = 0;
    for (; i < numBits; i++) {
        shift <<= 1;
    }
    u32 mask = (shift - 1) << (bitPos & 0x1f);
    u32 old = *field;
    *field = (newVal << (bitPos & 0x1f)) | (~mask & old);
    return (mask & old) >> (bitPos & 0x1f);
}
/* Apply a transformation matrix to a vector */
void TransformVectors(u32 srcVectors, u32 dstStart, int count, MATRIX *mtx)
{
    short localVec[3];
    u16 zOffset;
    u32 end = srcVectors + count * 6;
    for (u32 cur = srcVectors; cur < end; cur += 6) {
        /* Read 3 shorts from source (misaligned) */
        uint a = (dstStart + 3) & 3;
        uint b = dstStart & 3;
        localVec[0] = (*(int *)((dstStart + 3) - a) << (3 - a) * 8 |
                       1 & 0xffffffffU >> (a + 1) * 8) & -1 << (4 - b) * 8 |
                       *(uint *)(dstStart - b) >> b * 8;
        zOffset = *(u16 *)(dstStart + 4);
        a = (u32)&localVec[0] & 3;
        *(u32 *)((u32)&localVec[0] - a) = *(u32 *)((u32)&localVec[0] - a) & -1 << (a + 1) * 8 | (u32)localVec[0] >> (3 - a) * 8;
        localVec[2] = zOffset;
        ApplyMatrixSV(mtx, (SVECTOR *)localVec, (SVECTOR *)cur);
        dstStart += 6;
    }
}
/* Apply matrix to multiple VECTORs (long[3] arrays) */
void ApplyMatrixToVectors(VECTOR *dst, VECTOR *src, int count, MATRIX *mtx)
{
    long *ptr = &dst->vx;
    for (int i = 0; i < count; dst = (VECTOR *)&dst->pad, i++) {
        ApplyMatrixLV(mtx, src, dst);
        src = (VECTOR *)&src->pad;
    }
}
/* Check if a point is within a bounding box centered at origin */
/* center, half-size, point */
u32 PointInBoundingBox(int *center, int halfSize, int *point)
{
    int i = 0;
    while (1) {
        if (*point < *center - halfSize) return 0;
        if (halfSize + *center < *point) break;
        i++;
        center++;
        point++;
        if (i > 2) return 1;
    }
    return 0;
}
/* Swap global pointer value, return old value */
void *SwapGlobalPtr(u32 newPtr)
{
    void *old = Other_GlobalPtr;
    Other_GlobalPtr = newPtr;
    return old;
}
/* Trigger screen shake effect for specific sound types */
void TriggerScreenShake(int soundType)
{
    if (soundType == 0x4e) {
        goto TRIGGER;
    }
    if (soundType < 0x4f) {
        if ((soundType != 0xb) && (soundType != 0x38)) {
            return;
        }
    } else if (soundType != 0x5d) {
        return;
    }
TRIGGER:
    SoundSetTeleportFlag(1);
}
/* Advance through entity list via vtable iteration */
void AdvanceEntityList(void)
{
    if (Other_EntityListIter != 0) {
        Other_EntityListIter = (void *)(*(code *)(*(u32 *)Other_EntityListIter + 4))();
    }
}
/* Try to spawn an ambient sound entity at given position */
/* Returns entity handle or 0 on failure */
u32 TrySpawnAmbientSound(u32 soundId, u32 flags, u32 params, u32 extraFlags)
{
    int typeData;
    int result;
    typeData = LookupSoundType((short *)params);
    if (typeData != 0) {
        result = CheckSoundSpawnFlags(extraFlags, typeData);
        if (result != 0) {
            u32 entity = SpawnSoundEntity(params & 0xff, typeData, soundId);
            return entity;
        }
        if (Other_DreamStateNum == 0) {
            return 0;
        }
        int r = rand();
        if ((r % 0xc) != 0) {
            return 0;
        }
        if ((params & 1) != 0) {
            return 0;
        }
        TriggerAmbientEvent(&Other_AmbientEventData);
    }
    return 0;
}
/* Lookup sound type data matching a given parameter */
u32 LookupSoundType(short *param)
{
    short *tableEntry = (short *)(&Other_SoundTypeLookupTbl)[Other_DreamStateNum];
    int count = 0;
    int max = *(char *)(&Unk_SoundBufC4)[Other_DreamStateNum];
    if (0 < max) {
        do {
            if (*param == *tableEntry) {
                return LookupSoundResult();
            }
            count++;
            tableEntry += 3;
        } while (count < max);
    }
    return 0;
}
/* Add offset to sound effect ID if in dream state 4 */
int ApplySoundEffectOffset(int soundId, int baseOffset)
{
    if ((Other_DreamStateNum == 4) && (baseOffset == 0x10)) {
        int state = (*(code *)(*(u32 *)Other_DreamSysStatePtr + 0x200))();
        if (state == 4) {
            soundId += 0x1e;
        }
    }
    return soundId;
}
/* Check if a sound type can be spawned based on game state */
int CheckSoundSpawnFlags(int flags, int typeData)
{
    char *typeInfo = (char *)(typeData + 2);
    return *typeInfo == 0 || (int)*typeInfo != (flags % 2) + 1;
}
/* Spawn a sound entity in the scene */
int SpawnSoundEntity(u32 soundId, int typeData, u32 pos)
{
    u32 args[4];
    args[0] = pos;
    int entity = ENTITY_Construct(args);
    if (entity == 0) {
        return 0;
    }
    char *scriptPtr = (char *)(typeData + 3);
    void *dataTable = (&Other_SoundTypeDataTbl)[Other_DreamStateNum];
    for (; scriptPtr < (char *)(typeData + 6) && (*scriptPtr != -1); scriptPtr++) {
        DreamStateTraverse(soundId, typeData, dataTable + *scriptPtr * 8, entity);
    }
    return entity;
}
/* Execute a sound effect script action */
/* Processes one action entry from the type data table */
u32 ExecSoundScript(u32 soundId, u32 typeData, int *actionDef, EntityObj *entity)
{
    int result = DreamStateValidate(soundId, actionDef);
    if (result != 0) {
        char *params = (char *)(actionDef + 4);
        TriggerScreenShake(*(char *)(actionDef + 3));
        int val = (*(code *)(*(int *)entity + 0x88))(entity, (int)*(char *)(actionDef + 2));
        if (val == 0) {
            if (*(char *)(actionDef + 3) == 2) {
                return ExecSoundAction(soundId, params, actionDef + 0x38, entity);
            }
            return 0;
        }
        do {
            if ((params >= (char *)(actionDef + 8)) || (*params == -1)) {
                if (*(char *)(actionDef + 3) == 2) {
                    return ExecSoundAction(soundId, params, actionDef + 0x38, entity);
                }
                return 0;
            }
            result = NewSoundEntity(*(char *)(actionDef + 3), entity, params, *params);
            params++;
        } while (result == 0);
        return 1;
    }
    return 0;
}
/* Validate a sound trigger condition */
/* Returns 1 if condition is satisfied, 0 otherwise */
u32 ValidateSoundCondition(u32 soundId, char *condData)
{
    u32 op = (u8)condData[1];
    if (op == 1) goto TRIGGER_OK;
    if ((int)op < 0) {
        if (condData[0] != 0) {
            return 0;
        }
        op = ~op + 1;
    }
    switch (op) {
        case 2:
        case 3:
        case 4:
            soundId = DreamStateRangeCheck(soundId, op - 1);
            break;
        case 5:
            if ((soundId % 3) != 0) {
                return 0;
            }
            goto TRIGGER_OK;
        case 6:
            if ((soundId % 3) == 0) {
                return 0;
            }
            goto TRIGGER_OK;
        case 7:
            soundId = GetParticleModeBit();
            break;
        case 8:
        case 9:
            if ((int)soundId % 3 != op - 7) {
                return 0;
            }
            goto TRIGGER_OK;
        default:
            if ((int)op < 10) goto TRIGGER_OK;
            soundId = DreamStateCheckEq(op);
            break;
        case 0x14:
            if ((soundId & 1) != 0) {
                return 0;
            }
            goto TRIGGER_OK;
        case 0x15:
            soundId = soundId & 1;
    }
    if (soundId == 0) {
        return 0;
    }
TRIGGER_OK:
    condData[0] = 1;
    return 1;
}
/* Check if a sound ID matches the current dream state */
bool CheckSoundInDreamState(int op)
{
    char expected = (&Other_DreamStateMap)[op];
    int state = (*(code *)(*(u32 *)Other_DreamSysStatePtr + 0x200))(Other_DreamSysStatePtr);
    return expected == state;
}
/* Check sound ID against month/dream phase */
u32 CheckSoundDreamPhase(int soundId, int phase)
{
    int i;
    for (i = 0; i < 4; i++) {
        if ((soundId - 1) / 30 + 1 == phase) {
            return 1;
        }
        phase += 3;
    }
    return 0;
}
/* Create a new entity for sound playback */
bool CreateSoundEntity(u32 soundId, u32 entityType, u32 *positionData, int soundDirIdx)
{
    EntityObj *entity;
    short dir[3];
    short rot[3];
    entity = (int *)New_Entity(soundId, entityType, Other_SoundEntityParam);
    soundDirIdx = soundDirIdx * 4;
    if (entity != NULL) {
        dir[0] = *positionData;
        int rotationPtr = (int)&Other_SoundDirTable + soundDirIdx;
        dir[1] = *(u16 *)(rotationPtr);
        int rotScale = (char)(&Other_SoundRotTbl)[soundDirIdx] * 6;
        rot[0] = (char)(&Other_SoundRotTbl)[soundDirIdx];
        rot[1] = *(u16 *)((int)&Other_RotationData + rotScale);
        dir[2] = rot[1];
        (*(code *)(*(u32 *)Other_CurrentEntityPtr + 0xe8))(Other_CurrentEntityPtr, (int *)rot, dir);
        (*(code *)(*(int *)entity + 0x44))(entity, 1, &Unk_DreamBufF18 + (char)(&Other_SoundRotScaleTbl)[soundDirIdx] * 0xc);
        (*(code *)(*(int *)entity + 0x4c))(entity, Other_DreamSysStatePtr, Other_SoundEntityParam2, Other_CurrentEntityPtr, rot);
    }
    return entity == NULL;
}
/* Play an ambient sound event at a specific location */
void PlayAmbientEvent(int eventData)
{
    MATRIX mtx;
    if (*(int **)(eventData + 4) != NULL) {
        (*(code *)(**(int **)(eventData + 4) + 0x50))();
        ApplyMatrixLV(&mtx, (VECTOR *)eventData + 8, (VECTOR *)eventData + 8);
        (*(code *)(**(int **)(eventData + 4) + 0x4c))
                  (*(u32 *)(eventData + 4), Other_DreamSysStatePtr, Other_SoundEntityParam2, Other_CurrentEntityPtr, &mtx);
        RotateToTarget(*(int **)(eventData + 4), Other_DreamSysStatePtr, 1, 0, 0);
    }
}
/* Pass through to entity state machine */
/* Handles scene object search by ID */
u32 EntityStateMachine(EntityObj *entity, int targetObj)
{
    int objId;
    if (*(int *)(targetObj + 0xc) == 0) {
        objId = ENTITY_FindByFlags();
        entity->nField_5c = objId;
        entity->nField_60 = 1;
    } else {
        entity->nField_5c = *(int *)(targetObj + 0xc);
        entity->nField_60 = 0;
    }
    if (entity->nField_5c == 0) {
        EntityListCallEach(entity);
        return 1;
    }
    return (*(code *)(*(int *)entity + 0x100))(entity);
}
/* Advance to next entity in linked list via vtable */
void MoveToNextEntity(EntityObj *entity)
{
    (*(code *)(*(int *)entity + 0x104))();
    if (entity->nField_60 == 0) {
        entity->nField_5c = 0;
    } else {
        entity->nField_5c = (*(code *)(*(int *)entity->nField_5c + 4))();
    }
}
/* Find a character in a string buffer */
/* Returns index or -1 if not found */
int FindCharInBuffer(int buf, char c)
{
    char *data = *(char **)(buf + 0x74);
    if (data != 0) {
        int i;
        for (i = 0; i < *(int *)(buf + 0x6c); i++) {
            if (data[i] == c) {
                return i;
            }
        }
    }
    return -1;
}
/* Allocate and populate a character lookup table from a scene object */
u32 InitCharLookupTable(int obj)
{
    u32 tableSize;
    int intBuf;
    int charBuf;
    int *intPtr;
    int i;
    tableSize = (*(code *)(**(int **)(obj + 0x5c) + 0x80))(*(int **)(obj + 0x5c), 0, &intBuf);
    tableSize &= 0xff;
    intBuf = MemAlloc(tableSize << 2, 0);
    *(int *)(obj + 0x70) = intBuf;
    if (intBuf != 0) {
        charBuf = MemAlloc(tableSize, 0);
        *(int *)(obj + 0x74) = charBuf;
        if (charBuf != 0) {
            (*(code *)(**(int **)(obj + 0x5c) + 0x80))(*(int **)(obj + 0x5c), charBuf, &intBuf);
            intPtr = *(int **)(obj + 0x70);
            int entryCount = 0;
            *(u32 *)(obj + 0x6c) = 0;
            if (tableSize != 0) {
                do {
                    int charId = ENTITY_FindByFlags();
                    *intPtr = charId;
                    intPtr++;
                    if (charId == 0) goto CLEANUP;
                    entryCount++;
                    *(int *)(obj + 0x6c) += 1;
                } while (entryCount < (int)tableSize);
            }
            *(u32 *)(obj + 0x68) = *(u32 *)(((int *)intBuf)[0] * 4 + *(int *)(obj + 0x70));
            return 0;
        }
    }
    *(u32 *)(obj + 0x74) = 0;
CLEANUP:
    DestroyCharLookupTable(obj);
    return 1;
}
/* Free the character lookup table and clear pointers */
void DestroyCharLookupTable(int obj)
{
    u32 *intBuf = *(u32 **)(obj + 0x70);
    if ((intBuf != 0) && (*(int *)(obj + 0x74) != 0)) {
        int count = *(int *)(obj + 0x6c);
        while (count > 0) {
            EntityObj *entity = (int *)*intBuf;
            intBuf++;
            (*(code *)(*(int *)entity + 4))();
            count = *(int *)(obj + 0x6c);
        }
        *(u32 *)(obj + 0x68) = 0;
    }
    MemFree(*(u32 *)(obj + 0x74), 0);
    *(u32 *)(obj + 0x74) = 0;
    MemFree(*(u32 *)(obj + 0x70), 0);
    *(u32 *)(obj + 0x70) = 0;
}
/* vtable getter for debug/input handler object */
void *GetVtable_DebugInput(void)
{
    return &Other_DebugInputVtable;
}

