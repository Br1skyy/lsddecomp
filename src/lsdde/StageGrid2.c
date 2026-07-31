#include "common.h"
#include "DreamSys.h"
#include "StageGrid.h"
#include "MEMORY.H"
/* ========================================================================
   StageGrid2.c - Stage grid chunk management, entity spawning, visibility,
                  memory card I/O, and navigation challenge support.
   ======================================================================== */
/* ---------- external helpers (defined in other translation units) ------- */
extern void  *MemAlloc(int size);
extern u32    MemFree(int ptr, int heapFallback);
extern int    GetStageDataTable(int *outSize);
extern u32    GetRandomValue(void);
extern int    CreateRenderObject(int chunkCol, int tileCol, int tileRow, int pos, int arg5);
extern s32    CardWriteRead(int base, int size);
extern void   FormatIntToString(char *dst, int val, int pad, int mode);
extern void   EnterCriticalSection(void);
extern void   LeaveCriticalSection(void);
/* Forward declarations for functions defined later in this file */
int FillChunkRenderEntry(int *grid, int *entry, int entryIdx, s8 direction, s32 startX, s32 startY, s32 sizeX, s32 sizeY);
char *BuildMemoryCardPath(char *outPath, int slotId, char *filename);
void ProcessChunkEntityData(s32 grid, void (*callback)(s32, s32), int chunkData);
/* PS1 SDK card/event/fcntl functions used by the memory card I/O routines */
extern int _card_info(int slot);
extern int _card_load(int slot);
extern int _card_clear(int slot);
extern int open(const char *path, int flags);
extern int read(int fd, void *buf, int count);
extern int write(int fd, const void *buf, int count);
extern int close(int fd);
extern int lseek(int fd, int offset, int whence);
extern int delete(const char *path);
extern char *strcat(char *dst, const char *src);
extern char *strcpy(char *dst, const char *src);
extern int atoi(const char *str);
extern int printf(const char *fmt, ...);
extern int TestEvent(int ev);
extern int EnableEvent(int ev);
extern int DisableEvent(int ev);
extern int WaitForMemoryCardEvent(int slot);
/* ---------- global data (defined in other translation units) ------------ */
extern u32  Stage_ChunkSpawnCounts;
extern s32  Stage_McSplitIoState;
extern s32  Stage_McSplitHalfSize;
extern s32  Stage_NavChallengeBuf;
extern s32  Stage_NavChallengeWord1;
extern s32  Stage_NavChallengeWord2;
extern u8   Stage_CamMatrixRow0[];
extern u8   Stage_CamMatrixRow1[];
extern u8   Stage_CamMatrixRow2[];
extern u8   Stage_CamMatrixRow3[];
extern s32  Stage_CamMatrixTrans;
extern u8   Stage_McEncodingData[];
extern u8   Stage_McEncodingData2[];
extern u32  Stage_McEventStatusResults;
extern u32  Stage_AdjEdgeFlags;
extern u32  Stage_AdjDirectionOffsets;
extern u32  Stage_AdjBaseOffsets;
extern u32  Stage_AdjAltOffsets;
extern s32  Stage_StageChunkVtable;
extern s32  Stage_StageGridVtable;
extern s32  Stage_MapChunkVtable;
extern s32  Stage_ChunkDataVtable;
extern s32  Stage_ChunkDataTableVtable;
extern s32  Stage_EntityVtable;
extern s32  Stage_MemoryCardVtable;
extern char Stage_McPathPrefix1[];
extern char Stage_McPathPrefix0[];
extern char Stage_McDeleteFilename[];
/* PS1 SDK types */
typedef struct { s16 vx, vy, vz; short pad; } SVECTOR;
typedef struct { s32 m[3][3]; s32 t[3]; } MATRIX;
typedef struct { s32 vx, vy, vz, vw; } VECTOR;
extern void RotMatrix(SVECTOR *rot, MATRIX *m);
extern void ApplyMatrixLV(MATRIX *m, VECTOR *src, VECTOR *dst);
/* -----------------------------------------------------------------------
   Chunk / spawn-point data accessors
   ----------------------------------------------------------------------- */
/* Get base address of chunk spawn data for the current stage */
int GetChunkSpawnTable(void)
{
    return GetStageDataTable(NULL) + 0xfc;
}
/* Get address of a specific spawn-point entry (SPAWN_ENTRY_STRIDE = 28 bytes each) */
int GetChunkSpawnEntry(int stage, int index)
{
    return GetChunkSpawnTable() + index * SPAWN_ENTRY_STRIDE;
}
/* Get spawn entry using grid dimensions to compute the linear index */
void GetChunkSpawnByGrid(int stage, int col, int row)
{
    s16 *dims;
    dims = (s16 *)GetChunkSpawnTable();
    GetChunkSpawnEntry(stage, col + dims[0] * row);
}
/* -----------------------------------------------------------------------
   Ambient sound table accessors
   ----------------------------------------------------------------------- */
/* Returns the ambient sound filename and its bank size */
char *GetAmbientSoundFile(s32 *outSize)
{
    if (outSize != NULL) {
        *outSize = 0x31;
    }
    return "ETC\\ASMK.STR";
}
/* Get base address of ambient sound data */
int GetAmbientSoundBase(s32 *outSize)
{
    if (outSize != NULL) {
        *outSize = 0;
    }
    return GetStageDataTable(NULL) + 0x3d40;
}
/* Get address of a random ambient sound from the 7-entry table */
int GetRandomAmbientSound(s32 *outIndex)
{
    u32 randVal;
    int base;
    s32 tableSize;
    randVal = GetRandomValue();
    base = GetAmbientSoundBase(&tableSize);
    if (outIndex != NULL) {
        *outIndex = randVal % 7 + tableSize;
    }
    return base + (randVal % 7) * SPAWN_ENTRY_STRIDE;
}
/* -----------------------------------------------------------------------
   Entity-type data table accessors
   ----------------------------------------------------------------------- */
/* Get base address of entity data with 7 entries (offset 0x3e04 from stage data) */
int GetEntityData7(s32 *outCount)
{
    if (outCount != NULL) {
        *outCount = 7;
    }
    return GetStageDataTable(NULL) + 0x3e04;
}
/* Wrapper that also returns the entry count */
int GetEntityData7Ex(s32 *outCount)
{
    s32 count;
    int base;
    base = GetEntityData7(&count);
    if (outCount != NULL) {
        *outCount = count;
    }
    return base;
}
/* Get base address of entity data with 8 entries (offset 0x3e20 from stage data) */
int GetEntityData8(s32 *outCount)
{
    if (outCount != NULL) {
        *outCount = 8;
    }
    return GetStageDataTable(NULL) + 0x3e20;
}
/* Get address of entity data entry by index within the 8-entry table */
int GetEntityData8Entry(s32 *outIndex, int index)
{
    s32 tableBase;
    int base;
    base = GetEntityData8(&tableBase);
    if (outIndex != NULL) {
        *outIndex = index + tableBase;
    }
    return base + index * SPAWN_ENTRY_STRIDE;
}
/* -----------------------------------------------------------------------
   Spawn-point data table accessors
   ----------------------------------------------------------------------- */
/* Get address of a spawn data entry from the extended spawn table (0x3ec8 offset) */
int GetSpawnDataEntry(s32 *outEntry, int index)
{
    if (outEntry != NULL) {
        *outEntry = index * 2 + 0xe;
    }
    return GetStageDataTable(NULL) + 0x3ec8 + index * 0xa8;
}
/* Get spawn data using packed stage/mode parameter.
   Low 16 bits: stage index (negative = use entity table, positive = use spawn table)
   High 16 bits: sub-index within the chosen table */
int GetSpawnDataByMode(s32 *outEntry, s32 packedParam)
{
    s16 stage;
    u16 subIndex;
    int entry;
    s32 spawnBase;
    stage = (s16)packedParam;
    subIndex = (u16)((u32)packedParam >> 16);
    if (stage < 0) {
        entry = GetEntityData8Entry(outEntry, (int)(s16)subIndex);
    } else {
        entry = GetSpawnDataEntry(&spawnBase, 0);
        if (outEntry != NULL) {
            if (subIndex < 2) {
                *outEntry = (s16)subIndex + spawnBase;
            } else {
                *outEntry = -1;
            }
        }
        entry = entry + (s16)subIndex * SPAWN_ENTRY_STRIDE;
    }
    return entry;
}
/* -----------------------------------------------------------------------
   Chunk spawn count and total spawn point calculation
   ----------------------------------------------------------------------- */
/* Get the spawn count for a specific chunk from the lookup table */
int GetChunkSpawnCount(int chunkIndex)
{
    return (int)*(s16 *)(&Stage_ChunkSpawnCounts + chunkIndex * 2);
}
/* Calculate total number of spawn points for a range of chunks */
int CalcChunkSpawnTotal(s32 *outTotal, int stage, int chunkCount)
{
    s16 *spawnCounts;
    int spawnBase;
    int endIdx;
    (void)GetSpawnDataEntry(&spawnBase, 0);
    *outTotal = 0;
    endIdx = chunkCount * 2 + spawnBase;
    if (spawnBase < endIdx) {
        spawnCounts = (s16 *)(&Stage_ChunkSpawnCounts + spawnBase * 2);
        do {
            s16 count = *spawnCounts;
            spawnCounts++;
            spawnBase++;
            *outTotal = *outTotal + 10 + (int)count;
        } while (spawnBase < endIdx);
    }
    *outTotal = *outTotal - 10;
    return spawnBase;
}
/* -----------------------------------------------------------------------
   StageChunk object management
   ----------------------------------------------------------------------- */
/* Create a new StageChunk object (SIZEOF_STAGE_CHUNK = 80 bytes) */
int CreateStageChunk(int stage, s32 initParam1, s32 initParam2)
{
    void *obj;
    int vtable;
    obj = MemAlloc(SIZEOF_STAGE_CHUNK);
    vtable = 0;
    if (obj != 0) {
        vtable = (int)GetChunkVtable();
        (*(void (**)(void *, int, s32, s32))(vtable + 8))(obj, stage, initParam1, initParam2);
        vtable = (int)obj;
    }
    return vtable;
}
/* -----------------------------------------------------------------------
   StageGrid entity management
   ----------------------------------------------------------------------- */
/* Position an entity within a chunk and initialize its render state */
void SetEntityChunkPosition(int *grid, s32 renderObjParam5)
{
    int renderObj;
    renderObj = CreateRenderObject(
        grid[0xd], grid[0x10], grid[0x11], grid[0x12], renderObjParam5
    );
    grid[0x13] = renderObj;
    (*(void (**)(int *, int))(grid[0] + 0x10))(grid, renderObj);
    (*(void (**)(int, int, int))(*(int *)grid[0x13] + 0x44))(
        grid[0x13], grid[3], grid[0xe]
    );
    grid[0xf] = 2;
}
/* Return vtable address for the StageChunk object type */
int GetChunkVtable(void)
{
    return (int)&Stage_StageChunkVtable;
}
/* -----------------------------------------------------------------------
   Memory card split-file save/load
   ----------------------------------------------------------------------- */
/* Handle split memory card save/load with file offset tracking */
void HandleSplitCardIO(int isSave)
{
    s32 base;
    s32 tableSize;
    int slotState;
    int halfSize;
    (void)halfSize;
    base = GetStageDataTable(&tableSize);
    slotState = Stage_McSplitIoState + 1;
    if (slotState == 1) {
        Stage_McSplitIoState = Stage_McSplitIoState + 2;
        if (isSave == 0) {
            Stage_McSplitHalfSize = tableSize / 2;
            Stage_McSplitIoState = slotState;
            tableSize = Stage_McSplitHalfSize;
        }
    } else if (slotState == 2) {
        tableSize = tableSize - Stage_McSplitHalfSize;
        Stage_McSplitIoState = slotState;
    } else {
        tableSize = 0;
        Stage_McSplitIoState = slotState;
    }
    do {
        slotState = CardWriteRead(base, tableSize);
    } while (slotState == 0);
}
/* -----------------------------------------------------------------------
   StageGrid object management
   ----------------------------------------------------------------------- */
/* Return vtable address for the StageGrid object type */
int GetGridVtable(void)
{
    return (int)&Stage_StageGridVtable;
}
/* Create a new StageGrid object (SIZEOF_STAGE_GRID = 488 bytes) */
int CreateStageGrid(s32 initParam1, s32 initParam2)
{
    void *obj;
    int vtable;
    obj = MemAlloc(SIZEOF_STAGE_GRID);
    vtable = 0;
    if (obj != 0) {
        vtable = (int)GetMapChunkVtable();
        (*(void (**)(void *, s32, s32))(vtable + 8))(obj, initParam1, initParam2);
        vtable = (int)obj;
    }
    return vtable;
}
/* -----------------------------------------------------------------------
   Entity position and boundary setup
   ----------------------------------------------------------------------- */
/* Set entity position from tile data and trigger boundary update */
void SetEntityTilePosition(int *grid, int tileData, s32 worldPos)
{
    s8 tileCol;
    s8 tileRow;
    tileCol = *(s8 *)(tileData + 2);
    tileRow = *(s8 *)(tileData + 3);
    *(s16 *)(grid + 0x1f) = tileCol - 1;
    *(s16 *)((int)grid + 0x7e) = tileRow - 1;
    grid[0x20] = worldPos;
    grid[0x21] = worldPos;
    UpdateChunkBoundaries(grid);
}
/* Allocate render resources and set entity bounding box within chunk */
void InitEntityChunkBounds(int *grid, int tileData, s16 adjust)
{
    s8 tileCol;
    s8 tileRow;
    int colStart;
    int colEnd;
    int rowStart;
    int rowEnd;
    int renderObj;
    tileCol = *(s8 *)(tileData + 2);
    tileRow = *(s8 *)(tileData + 3);
    if (tileCol == 0) {
        colStart = 0;
        colEnd = adjust - 1;
    } else {
        colStart = tileCol - 1;
        colEnd = adjust;
    }
    if (tileCol == TILE_GRID_COUNT) {
        colEnd = colEnd - 1;
    }
    if (tileRow == 0) {
        adjust = adjust - 1;
        rowStart = 0;
    } else {
        rowStart = tileRow - 1;
    }
    if (tileRow == TILE_GRID_COUNT) {
        adjust = adjust - 1;
    }
    grid[0x22] = 1;
    renderObj = (*(int (**)(int *, int))(grid[0] + 0x124))(
        grid, *(int *)(tileData + 0x28)
    );
    grid[0x23] = renderObj;
    *(s16 *)(grid + 0x24) = colStart;
    *(s16 *)((int)grid + 0x92) = rowStart;
    *(s16 *)(grid + 0x25) = colEnd;
    *(s16 *)((int)grid + 0x96) = adjust;
}
/* -----------------------------------------------------------------------
   Chunk entity iteration and visibility processing
   ----------------------------------------------------------------------- */
/* Process all entities in all visible chunks with a callback */
void ProcessAllChunkEntities(int grid, s32 cbParam1, s32 cbParam2)
{
    int *chunkEntry;
    int chunkCount;
    int i;
    s16 *chunkInfo;
    int *entityList;
    int j;
    int k;
    chunkEntry = (int *)(grid + 0x8c);
    chunkCount = 0;
    if (0 < *(int *)(grid + 0x88)) {
        chunkInfo = (s16 *)(grid + 0x96);
        do {
            i = grid + *chunkEntry * SPAWN_ENTRY_STRIDE + SIZEOF_CHUNK_DATA;
            if (*(s16 *)(*(int *)(i + 4) + 0x2c) != 0) {
                k = 0;
                entityList = (int *)(
                    chunkInfo[-3] * 4 +
                    *(int *)(i + 0x10) +
                    chunkInfo[-2] * CHUNK_ENTRY_STRIDE
                );
                if (0 < *chunkInfo) {
                    do {
                        j = 0;
                        if (0 < chunkInfo[-1]) {
                            do {
                                *(s16 *)(grid + 0x1c0) = *(s16 *)(grid + 0xbc);
                                *(s8 *)(grid + 0x1c2) = (s8)chunkInfo[-3] + (s8)j;
                                *(s8 *)(grid + 0x1c3) = (s8)chunkInfo[-2] + (s8)k;
                                ProcessEntityCallback(*entityList, cbParam1, cbParam2);
                                for (i = *(int *)(*entityList + 0x38);
                                     i != 0;
                                     i = *(int *)(i + 0x38))
                                {
                                    ProcessEntityCallback(i, cbParam1, cbParam2);
                                }
                                j++;
                                entityList++;
                            } while (j < chunkInfo[-1]);
                        }
                        k++;
                            entityList += (ENTITY_LIST_STRIDE - chunkInfo[-1]);
                    } while (j < *chunkInfo);
                }
            }
            chunkEntry += 3;
            chunkCount++;
            chunkInfo += 6;
        } while (chunkCount < *(int *)(grid + 0x88));
    }
}
/* Process an entity if its visible flag (bit 7) is set */
void ProcessEntityIfVisible(int *entity)
{
    if (entity != NULL && (*(u16 *)((int)entity + 0x36) & 0x80) != 0) {
        (*(void (**)(void))(entity[0] + 0x38))();
    }
}
/* -----------------------------------------------------------------------
   Spawn position calculation from chunk/tile data
   ----------------------------------------------------------------------- */
/* Calculate world-space spawn position from chunk, tile, and spawn point data */
int CalcSpawnWorldPosition(
    int *outPos, int *outChunkCenter, s16 *chunkData,
    int *currentPos, char *spawnPoint
)
{
    int worldX;
    int worldZ;
    u32 tileStride;
    int tileSize;
    int chunkOffset;
    if (*(int *)(chunkData + 2) == 0) {
        tileStride = (u32)spawnPoint[1];
        tileSize = (int)chunkData[1];
        chunkOffset = (int)*spawnPoint + (int)*chunkData * tileStride;
    } else {
        tileSize = 1;
        tileStride = 0;
        chunkOffset = 0;
    }
    worldX = *currentPos + *chunkData * -WORLD_TO_SCREEN_X + *spawnPoint * WORLD_TO_SCREEN_Y;
    worldZ = currentPos[2];
    *outChunkCenter = worldX;
    if ((tileStride & 1) != 0) {
        *outChunkCenter = worldX - WORLD_TO_SCREEN_X;
    }
    outChunkCenter[1] = currentPos[1];
    outChunkCenter[2] = worldZ + tileSize * -WORLD_TO_SCREEN_X + tileStride * WORLD_TO_SCREEN_Y;
    *outPos = spawnPoint[2] * WORLD_SCALE_Y + *outChunkCenter + *(s16 *)(spawnPoint + 4) + WORLD_SCALE_X;
    outPos[1] = (int)*(s16 *)(spawnPoint + 6) + outChunkCenter[1];
    outPos[2] = spawnPoint[3] * WORLD_SCALE_Y + outChunkCenter[2] + *(s16 *)(spawnPoint + 8) + WORLD_SCALE_X;
    *outChunkCenter = *outChunkCenter + WORLD_TO_SCREEN_X;
    outChunkCenter[2] = outChunkCenter[2] + WORLD_TO_SCREEN_X;
    return chunkOffset;
}
/* -----------------------------------------------------------------------
   Grid edge detection for navigation
   ----------------------------------------------------------------------- */
/* Get edge flags for a position in the grid.
   Returns inverted bitmask of edges: bit0=left, bit1=top, bit2=right, bit3=bottom */
u32 GetChunkEdgeFlags(int gridData, int position, int isVertical)
{
    s16 *dims;
    s32 columns;
    s32 rows;
    u32 flags;
    dims = *(s16 **)(gridData + 0x68);
    columns = (s32)*dims;
    rows = (s32)dims[1];
    if (*(int *)(dims + 2) == 0) {
        flags = -(u32)(position < columns) & 3;
        if (columns * (rows - 1) <= position) {
            flags = flags | 0x60;
        }
        if (columns == 0) { trap(TRAP_DIVZERO); }
        if ((columns == -1) && (position == -0x80000000)) { trap(TRAP_OVERFLOW); }
        if (position % columns == 0) {
            if (isVertical == 0) {
                flags = flags | 4;
            } else {
                flags = flags | 0x25;
            }
        }
        if (columns == 0) { trap(TRAP_DIVZERO); }
        if ((columns == -1) && (position + 1 == -0x80000000)) { trap(TRAP_OVERFLOW); }
        if ((position + 1) % columns == 0) {
            if (isVertical == 0) {
                flags = flags | 0x52;
            } else {
                flags = flags | 0x10;
            }
        }
    } else {
        flags = 0xffffffff;
        {
            int i = 0;
            if (0 < rows) {
                do {
                    i++;
                    flags = flags << 1;
                } while (i < rows);
            }
        }
    }
    return ~flags;
}
/* -----------------------------------------------------------------------
   Adjacent chunk linking
   ----------------------------------------------------------------------- */
/* Try to link to an adjacent chunk. Returns 1 on success, 0 on failure. */
s32 TryLinkAdjacentChunk(
    int gridData, s32 *outLink, int direction, int useAltOffset,
    int baseIndex, u32 edgeFlags, int edgeIndex
)
{
    s32 linkResult;
    int calcIndex;
    int adjTableOffset;
    int adjData;
    linkResult = 0;
    if ((edgeFlags & *(u32 *)(&Stage_AdjEdgeFlags + edgeIndex * 4)) == 0) {
        *outLink = 0;
    } else {
        calcIndex = baseIndex + edgeIndex;
        if (*(int *)(*(int *)(gridData + 0x68) + 4) == 0) {
            adjTableOffset = edgeIndex * COLOR_TABLE_STRIDE;
            if (*(int *)(&Stage_AdjDirectionOffsets + adjTableOffset) == 0) {
                calcIndex = baseIndex + *(int *)(&Stage_AdjBaseOffsets + adjTableOffset);
            } else {
                if (useAltOffset == 0) {
                    adjData = *(int *)(&Stage_AdjAltOffsets + adjTableOffset);
                } else {
                    adjData = *(int *)(&Stage_AdjBaseOffsets + adjTableOffset);
                }
                calcIndex = baseIndex + direction * *(int *)(&Stage_AdjDirectionOffsets + adjTableOffset) + adjData;
            }
        }
        outLink[1] = calcIndex;
        linkResult = (*(int (**)(int, s32, int, int))(gridData + 0x60))(
            *(int *)(gridData + 100), outLink[1], 0, 0
        );
        *outLink = linkResult;
        linkResult = 1;
    }
    outLink[2] = edgeIndex;
    return linkResult;
}
/* -----------------------------------------------------------------------
   Active chunk counting
   ----------------------------------------------------------------------- */
/* Count how many chunks in the 7-slot grid array have entities */
int CountActiveChunks(int gridData)
{
    int count;
    int active;
    active = 0;
    for (count = 0; count < 7; count++) {
        if (*(s16 *)(gridData + SIZEOF_CHUNK_DATA + count * SPAWN_ENTRY_STRIDE) != 0) {
            active++;
        }
    }
    return active;
}
/* -----------------------------------------------------------------------
   Grid coordinate conversion
   ----------------------------------------------------------------------- */
/* Convert a linear chunk index to column/row coordinates */
void ChunkIndexToCoords(int gridData, s8 *outCol, s8 *outRow, int index)
{
    s32 columns;
    columns = (s32)**(s16 **)(gridData + 0x68);
    if (columns == 0) { trap(TRAP_DIVZERO); }
    if ((columns == -1) && (index == -0x80000000)) { trap(TRAP_OVERFLOW); }
    *outCol = (s8)(index % columns);
    columns = (s32)**(s16 **)(gridData + 0x68);
    if (columns == 0) { trap(TRAP_DIVZERO); }
    if ((columns == -1) && (index == -0x80000000)) { trap(TRAP_OVERFLOW); }
    *outRow = (s8)(index / columns);
}
/* -----------------------------------------------------------------------
   Camera-relative chunk offset calculation
   ----------------------------------------------------------------------- */
/* Calculate chunk screen offsets based on camera direction */
void CalcChunkCameraOffset(int *grid, int targetX, int targetY)
{
    short rotation;
    int entityData;
    s32 entityBase;
    u8 localData[2];
    s8 chunkX;
    s8 chunkY;
    u8 localMatrix[24];
    s32 matrixResult;
    int unused;
    s32 clampOffset;
    entityData = *(int *)(*(int *)(grid[0x1b] + 0x14) + 0x44);
    (*(void (**)(int *, u8 *, int))(grid[0] + 0x10c))(grid, localData, 0);
    rotation = *(s16 *)(entityData + 0x12);
    if (rotation < 0) {
        rotation += 0x1000;
    }
    localMatrix[0] = Stage_CamMatrixRow0[0];
    localMatrix[1] = Stage_CamMatrixRow0[1];
    localMatrix[2] = Stage_CamMatrixRow1[0];
    localMatrix[3] = Stage_CamMatrixRow1[1];
    localMatrix[4] = Stage_CamMatrixRow2[0];
    localMatrix[5] = Stage_CamMatrixRow2[1];
    localMatrix[6] = Stage_CamMatrixRow3[0];
    localMatrix[7] = Stage_CamMatrixRow3[1];
    *(int *)(localMatrix + 8) = Stage_CamMatrixTrans;
    *(int *)(localMatrix + 12) = 0;
    matrixResult = 0;
    unused = grid[0x1d];
    RotMatrix((SVECTOR *)(entityData + 0x10), (MATRIX *)localMatrix);
    ApplyMatrixLV(
        (MATRIX *)localMatrix,
        (VECTOR *)(localMatrix + 0x14),
        (VECTOR *)(localMatrix + 0x14)
    );
    if (((u16)(rotation - 0x200) < DEGREES_90) || ((u16)(rotation - 0xa00) < DEGREES_90)) {
        grid[0x20] = targetY;
        grid[0x21] = targetX;
        chunkX = localData[0];
        if ((int)*(s16 *)(localMatrix + 20) < 1) {
            chunkX = (localData[0] - (s8)targetX) + 1;
        }
        *(s16 *)(grid + 0x1f) = chunkX;
        if (unused < 1) {
            chunkY = (localData[1] - (s8)grid[0x1e]) + 1;
        } else {
            chunkY = (localData[1] - (s8)grid[0x1e]) - 1;
        }
        *(s16 *)((int)grid + 0x7e) = chunkY;
        matrixResult = 0;
        entityBase = unused;
    } else if (((u16)(rotation - 0x600) < DEGREES_90) || (0xbff < (u16)(rotation - 0x200))) {
        grid[0x20] = targetX;
        grid[0x21] = targetY;
        if ((int)*(s16 *)(localMatrix + 20) < 1) {
            chunkX = (localData[0] - (s8)grid[0x1e]) + 1;
        } else {
            chunkX = (localData[0] - (s8)grid[0x1e]) - 1;
        }
        *(s16 *)(grid + 0x1f) = chunkX;
        chunkY = localData[1];
        if (unused < 1) {
            chunkY = (localData[1] - (s8)targetY) + 1;
        }
        *(s16 *)((int)grid + 0x7e) = chunkY;
        matrixResult = 1;
        entityBase = *(s16 *)(localMatrix + 20);
    }
    clampOffset = entityBase >> 0xb;
    {
        s32 gridMax = (s32)(s16)grid[0x1e];
        if (gridMax <= clampOffset) {
            clampOffset = gridMax - 1;
        }
        if (clampOffset <= -gridMax) {
            clampOffset = -gridMax + 1;
        }
    }
    if (matrixResult == 0) {
        *(s16 *)((int)grid + 0x7e) = *(s16 *)((int)grid + 0x7e) + (s16)clampOffset;
    } else {
        *(s16 *)(grid + 0x1f) = *(s16 *)(grid + 0x1f) + (s16)clampOffset;
    }
    UpdateChunkBoundaries(grid);
}
/* -----------------------------------------------------------------------
   Visible chunk list building
   ----------------------------------------------------------------------- */
/* Build the list of visible chunks for rendering */
void BuildVisibleChunkList(int *grid)
{
    bool isNegX;
    s32 targetX;
    s32 targetY;
    s32 negXOffset;
    s32 negYOffset;
    s8 directionCode;
    int *nextChunk;
    s16 overflow;
    negXOffset = (int)(s16)grid[0x1f];
    targetX = grid[0x20];
    targetY = grid[0x21];
    isNegX = -1 < negXOffset;
    directionCode = 3;
    if (!isNegX) {
        negXOffset += 0x14;
        directionCode = 2;
    }
    negYOffset = (int)*(s16 *)((int)grid + 0x7e);
    if (negYOffset < 0) {
        negYOffset += 0x14;
        if (isNegX) {
            directionCode = 9 < negXOffset;
            if ((bool)directionCode) {
                negXOffset -= 10;
            } else {
                negXOffset += 10;
            }
        } else {
            negXOffset -= 10;
            directionCode = 0;
        }
    }
    int chunkIdx = (*(int (**)(int *, s8))(grid[0] + 0x120))(grid, directionCode);
    grid[0x23] = chunkIdx;
    s32 clampedX = negXOffset;
    if (negXOffset < 0) {
        clampedX = 0;
    }
    *(s16 *)(grid + 0x24) = (s16)clampedX;
    *(s16 *)((int)grid + 0x92) = (s16)negYOffset;
    if (negXOffset + targetX < 0x15) {
        *(s16 *)(grid + 0x25) = (s16)targetX;
        chunkIdx = FillChunkRenderEntry(
            grid, grid + 0x23, 0, directionCode,
            negXOffset, negYOffset, targetX, targetY
        );
    } else {
        overflow = (s16)(negXOffset + targetX) - 0x14;
        *(s16 *)(grid + 0x25) = (s16)targetX - overflow;
        chunkIdx = FillChunkRenderEntry(
            grid, grid + 0x23, 0, directionCode,
            negXOffset, negYOffset, targetX, targetY
        );
        chunkIdx++;
        nextChunk = grid + chunkIdx * 3 + 0x23;
        int nextDir = (*(int (**)(int *, s8))(grid[0] + 0x120))(
            grid, directionCode + 1
        );
        *nextChunk = nextDir;
        *(s16 *)(nextChunk + 1) = 0;
        *(s16 *)(nextChunk + 2) = overflow;
        *(s16 *)((int)nextChunk + 6) = *(s16 *)((int)grid + 0x92);
        *(s16 *)((int)nextChunk + 10) = *(s16 *)((int)grid + 0x96);
    }
    grid[0x22] = chunkIdx + 1;
}
/* Fill a single chunk render entry, handling vertical splitting if needed */
int FillChunkRenderEntry(
    int *grid, int *entry, int entryIdx, s8 direction,
    s32 startX, s32 startY, s32 sizeX, s32 sizeY
)
{
    s16 overflow;
    s16 splitSize;
    int *nextEntry;
    int nextDir;
    s32 nextStart;
    if (startY + sizeY < 0x15) {
        *(s16 *)(entry + 10) = (s16)sizeY;
    } else {
        overflow = (s16)(startY + sizeY) - 0x14;
        *(s16 *)(entry + 10) = (s16)sizeY - overflow;
        nextEntry = grid + (entryIdx + 1) * 3 + 0x23;
        if (startX < 10) {
            nextDir = (*(int (**)(int *, s8))(grid[0] + 0x120))(grid, direction + 2);
            *nextEntry = nextDir;
            splitSize = (s16)startX + 10;
        } else {
            nextDir = (*(int (**)(int *, s8))(grid[0] + 0x120))(grid, direction + 3);
            *nextEntry = nextDir;
            splitSize = (s16)startX - 10;
        }
        *(s16 *)(nextEntry + 1) = splitSize;
        *(s16 *)((int)nextEntry + 10) = overflow;
        nextStart = (s16)nextEntry[1] + sizeY;
        *(s16 *)((int)nextEntry + 6) = 0;
        if (nextStart < 0x15) {
            *(s16 *)(nextEntry + 2) = (s16)sizeY;
            entryIdx++;
        } else {
            splitSize = (s16)nextStart;
            *(s16 *)(nextEntry + 2) = ((s16)sizeY + 0x14) - splitSize;
            nextEntry = grid + (entryIdx + 2) * 3 + 0x23;
            nextDir = (*(int (**)(int *, s8))(grid[0] + 0x120))(grid, nextDir + 1);
            *nextEntry = nextDir;
            *(s16 *)(nextEntry + 1) = 0;
            *(s16 *)((int)nextEntry + 6) = 0;
            *(s16 *)(nextEntry + 2) = splitSize - 0x14;
            *(s16 *)((int)nextEntry + 10) = overflow;
            entryIdx += 2;
        }
    }
    return entryIdx;
}
/* -----------------------------------------------------------------------
   Visible chunk list refresh
   ----------------------------------------------------------------------- */
/* Refresh the visible chunk list based on adjacency and camera position */
void RefreshVisibleChunkList(int *grid)
{
    u8 localData[2];
    s8 chunkCoords[38];
    s32 currentChunk;
    (*(void (**)(int *, u8 *, int))(grid[0] + 0x10c))(grid, localData, 0);
    grid[0x22] = 0;
    int count = BuildVisibleChunkListSingle(grid);
    grid[0x22] = count;
    currentChunk = (s32)localData[0];
    int adjacent = TestChunkInBounds(grid[0x77], chunkCoords);
    if ((adjacent != 0) && (currentChunk + 1 < (int)*(s16 *)(grid[0x1a] + 2))) {
        count = BuildVisibleChunkListSingle(grid);
        grid[0x22] = count;
    }
    if (-1 < currentChunk - 1) {
        count = BuildVisibleChunkListSingle(grid);
        grid[0x22] = count;
    }
}
/* Test if a point is inside a rectangular boundary */
int TestChunkInBounds(s16 *bounds, s8 *point)
{
    int result;
    result = 1;
    if ((bounds != NULL) &&
        ((int)*bounds <= (int)*point) &&
        ((int)*point <= *(int *)(bounds + 2)) &&
        ((int)bounds[1] <= (int)point[1]))
    {
        result = *(int *)(bounds + 4) < (int)point[1];
    }
    return result;
}
/* Add a single chunk to the visible list */
int AddChunkToVisibleList(int *grid, s32 unused, int entryIdx, s32 renderType)
{
    int *entry;
    int renderObj;
    entry = grid + entryIdx * 3 + 0x23;
    *entry = -1;
    entry[1] = 0;
    entry[2] = 0x140014;
    renderObj = (*(int (**)(int *, s32))(grid[0] + 0x124))(grid, renderType);
    *entry = renderObj;
    return entryIdx + 1;
}
/* -----------------------------------------------------------------------
   Chunk visibility flag management
   ----------------------------------------------------------------------- */
/* Toggle the visibility flag (bit 31) on all entities in all chunks */
void ToggleChunkEntityVisibility(int grid, int visible)
{
    int *chunkEntry;
    int chunkCount;
    s16 *chunkInfo;
    u32 visFlag;
    int *entityList;
    int entityData;
    int i;
    int j;
    chunkEntry = (int *)(grid + 0x8c);
    chunkCount = 0;
    if (0 < *(int *)(grid + 0x88)) {
        chunkInfo = (s16 *)(grid + 0x96);
        do {
            i = grid + *chunkEntry * SPAWN_ENTRY_STRIDE + SIZEOF_CHUNK_DATA;
            if (*(s16 *)(*(int *)(i + 4) + 0x2c) != 0) {
                j = 0;
                entityList = (int *)(
                    chunkInfo[-3] * 4 +
                    *(int *)(i + 0x10) +
                    chunkInfo[-2] * CHUNK_ENTRY_STRIDE
                );
                if (0 < *chunkInfo) {
                    do {
                        int k = 0;
                        if (0 < chunkInfo[-1]) {
                            do {
                                entityData = *entityList;
                                if (visible == 0) {
                                    visFlag = *(u32 *)(entityData + 0x10) | 0x80000000;
                                } else {
                                    visFlag = *(u32 *)(entityData + 0x10) & 0x7fffffff;
                                }
                                *(u32 *)(entityData + 0x10) = visFlag;
                                for (entityData = *(int *)(*entityList + 0x38);
                                     entityData != 0;
                                     entityData = *(int *)(entityData + 0x38))
                                {
                                    if (visible == 0) {
                                        visFlag = *(u32 *)(entityData + 0x10) | 0x80000000;
                                    } else {
                                        visFlag = *(u32 *)(entityData + 0x10) & 0x7fffffff;
                                    }
                                    *(u32 *)(entityData + 0x10) = visFlag;
                                }
                                k++;
                                entityList++;
                            } while (k < chunkInfo[-1]);
                        }
                        j++;
                        entityList += (ENTITY_LIST_STRIDE - chunkInfo[-1]);
                    } while (j < *chunkInfo);
                }
            }
            chunkInfo += 6;
            chunkCount++;
            chunkEntry += 3;
        } while (chunkCount < *(int *)(grid + 0x88));
    }
}
/* -----------------------------------------------------------------------
   Chunk entity processing helpers
   ----------------------------------------------------------------------- */
/* Process all chunks with an optional per-chunk callback */
void ProcessChunksWithCallback(int grid, s32 callbackAddr, void (*callback)(int, int))
{
    int chunkOffset;
    int i;
    chunkOffset = SIZEOF_CHUNK_DATA;
    for (i = 0; i < 7; i++) {
        if (callback != NULL) {
            callback(grid, grid + chunkOffset);
        }
        ProcessChunkEntityData(grid, callbackAddr, grid + chunkOffset);
        chunkOffset += SPAWN_ENTRY_STRIDE;
    }
}
/* Process entity data within a single chunk */
void ProcessChunkEntityData(s32 grid, void (*callback)(s32, s32), int chunkData)
{
    int *entityArray;
    int *endPtr;
    entityArray = *(int **)(chunkData + 0x10);
    endPtr = entityArray + 0x19a;
    for (; entityArray < endPtr; entityArray++) {
        callback(grid, *entityArray);
    }
}
/* -----------------------------------------------------------------------
   MapChunk and ChunkData object management
   ----------------------------------------------------------------------- */
/* Return vtable address for the MapChunk object type */
int GetMapChunkVtable(void)
{
    return (int)&Stage_MapChunkVtable;
}
/* Create a new MapChunk object (0xdc = 220 bytes) */
int CreateMapChunk(void)
{
    void *obj;
    int vtable;
    obj = MemAlloc(0xdc);
    vtable = 0;
    if (obj != 0) {
        vtable = (int)GetChunkDataVtable();
        (*(void (**)(void *))(vtable + 8))(obj);
        vtable = (int)obj;
    }
    return vtable;
}
/* Return vtable address for the ChunkData object type */
int GetChunkDataVtable(void)
{
    return (int)&Stage_ChunkDataVtable;
}
/* Create a new ChunkData object (0x3c = 60 bytes) */
int CreateChunkData(void)
{
    void *obj;
    int vtable;
    obj = MemAlloc(0x3c);
    vtable = 0;
    if (obj != 0) {
        vtable = (int)GetChunkDataTableVtable();
        (*(void (**)(void *))(vtable + 8))(obj);
        vtable = (int)obj;
    }
    return vtable;
}
/* Return vtable address for the ChunkDataTable object type */
int GetChunkDataTableVtable(void)
{
    return (int)&Stage_ChunkDataTableVtable;
}
/* -----------------------------------------------------------------------
   Dream time spawn eligibility check
   ----------------------------------------------------------------------- */
/* Determine if the player can spawn based on dream time and link status */
void CalcDreamTimeSpawnFlag(int gridData, int outputData)
{
    u32 canSpawn;
    canSpawn = 1;
    if (9999999 < *(int *)(*(int *)(gridData + 0xbc) + 0xc)) {
        canSpawn = (u32)(*(int *)(*(int *)(gridData + 0xbc) + 0x2f4) == 0);
    }
    *(u32 *)(*(int *)(outputData + 0x18) + 4) = canSpawn;
}
/* -----------------------------------------------------------------------
   Navigation challenge initialization
   ----------------------------------------------------------------------- */
/* Initialize navigation challenge data with formatted string */
void InitNavigationChallenges(s32 navValue)
{
    FormatIntToString((char *)&Stage_NavChallengeBuf, navValue, 3, 0);
    Stage_NavChallengeWord1 = Stage_NavChallengeWord1 & 0xffff | 0x36370000;
    Stage_NavChallengeWord2 = 0x32333435;
}
/* -----------------------------------------------------------------------
   Entity object management
   ----------------------------------------------------------------------- */
/* Return vtable address for the Entity object type */
int GetEntityVtable(void)
{
    return (int)&Stage_EntityVtable;
}
/* Create a new Entity object (0x84 = 132 bytes) */
int CreateEntity(s32 initParam1, s32 initParam2)
{
    void *obj;
    int vtable;
    obj = MemAlloc(0x84);
    vtable = 0;
    if (obj != 0) {
        vtable = (int)GetMemoryCardVtable();
        (*(void (**)(void *, s32, s32))(vtable + 8))(obj, initParam1, initParam2);
        vtable = (int)obj;
    }
    return vtable;
}
/* Clear entity fields */
void ClearEntityFields(int entity)
{
    *(s32 *)(entity + 0x60) = 0;
    *(s32 *)(entity + 100) = 0;
    *(s32 *)(entity + 0x68) = 0;
    *(s32 *)(entity + 0x78) = 0;
    *(s32 *)(entity + 0x7c) = 0;
}
/* -----------------------------------------------------------------------
   Memory card I/O
   ----------------------------------------------------------------------- */
/* Start a memory card save/load operation */
void StartMemoryCardIO(s32 slot, s32 unused1, s32 unused2, s32 unused3)
{
    int status;
    status = CheckMemoryCardReady(slot);
    if (status != 0) {
        LoadFromMemoryCard(slot);
    }
}
/* Check memory card status and report errors */
s32 CheckMemoryCardReady(int slot)
{
    int cardStatus;
    s32 result;
    WaitForMemoryCardEvent(slot);
    do {
        cardStatus = _card_info(*(s32 *)(slot + 0x10));
    } while (cardStatus == 0);
    cardStatus = TestMemoryCardDone(slot);
    if (cardStatus == 0x100) {
        result = 0;
    } else if (cardStatus == 0x8000) {
        result = 0;
    } else {
        result = 1;
        if (cardStatus == 0x2000) {
            _card_clear(*(s32 *)(slot + 0x10));
            result = 1;
        }
    }
    return result;
}
/* Load data from memory card */
s32 LoadFromMemoryCard(int slot)
{
    int loadStatus;
    s32 result;
    WaitForMemoryCardEvent(slot);
    do {
        loadStatus = _card_load(*(s32 *)(slot + 0x10));
    } while (loadStatus == 0);
    loadStatus = TestMemoryCardDone(slot);
    if (loadStatus == 0x100) {
        result = 0;
    } else if (loadStatus == 0x8000) {
        result = 0;
    } else {
        result = 1;
        if (loadStatus == 0x2000) {
            result = 1;
        }
    }
    return result;
}
/* Read file header from memory card */
s32 ReadFileHeader(int slot, char *outName)
{
    s32 path;
    int fd;
    s32 result;
    void *buffer;
    path = BuildMemoryCardPath((char *)&path, *(s32 *)(slot + 0xc), 0);
    fd = open((const char *)path, 1);
    result = 0;
    if (fd != -1) {
        if (outName != NULL) {
            buffer = MemAlloc(0x80);
            read(fd, buffer, 0x80);
            strcpy(outName, (char *)buffer + 4);
            MemFree((int)buffer, 0);
        }
        close(fd);
        result = 1;
    }
    return result;
}
/* Test if a file exists on memory card, delete it if found */
bool TestAndDeleteFile(int slot, s32 unused, int fileSize)
{
    s32 path;
    int fd;
    path = BuildMemoryCardPath((char *)&path, *(s32 *)(slot + 0xc), &Stage_McDeleteFilename);
    fd = open((const char *)path, (fileSize + 0x21ffU >> 0xd) << 0x10 | 0x200);
    if (fd != -1) {
        close(fd);
        delete((const char *)path);
    }
    return fd != -1;
}
/* Read file data from memory card at calculated offset */
bool ReadFileDataAtOffset(int slot, s32 fileName, void *outBuffer, int readSize)
{
    s32 path;
    int fd;
    void *headerBuf;
    u8 sectorCount;
    path = BuildMemoryCardPath((char *)&path, *(s32 *)(slot + 0xc), (char *)fileName);
    fd = open((const char *)path, 1);
    if (fd != -1) {
        headerBuf = MemAlloc(0x80);
        read(fd, headerBuf, 0x80);
        sectorCount = *(u8 *)(headerBuf + 2);
        MemFree((int)headerBuf, 0);
        lseek(fd, (u32)sectorCount * 0x80 - 0x780, 0);
        read(fd, outBuffer, readSize);
        close(fd);
    }
    return fd != -1;
}
/* Write file data to memory card with header construction */
s32 WriteFileWithHeader(
    int slot, s32 dataFileName, char *fileName,
    u8 sectorCount, int headerSize, s32 dataBuffer, int dataSize
)
{
    s32 path;
    int fd;
    s32 result;
    void *headerBuf;
    int dataOffset;
    u32 *srcPtr;
    u32 *dstPtr;
    path = BuildMemoryCardPath((char *)&path, *(s32 *)(slot + 0xc), (char *)dataFileName);
    delete((const char *)path);
    fd = open((const char *)path, (dataSize + 0x21ffU >> 0xd) << 0x10 | 0x200);
    if (fd == -1) {
        printf("File not create in WriteFile\n");
        result = 0;
    } else {
        close(fd);
        fd = open((const char *)path, 2);
        result = 0;
        if (fd != -1) {
            dataOffset = *(int *)(headerSize + 0x10);
            headerBuf = MemAlloc(0x200);
            *(u8 *)headerBuf = 0x53;
            *(u8 *)(headerBuf + 1) = 0x43;
            *(u8 *)(headerBuf + 2) = sectorCount + 0x10;
            *(u8 *)(headerBuf + 3) = (u8)(dataSize + 0x1fffU >> 0xd);
            strcpy((char *)headerBuf + 4, fileName);
            srcPtr = (u32 *)(dataOffset + 0x40);
            dstPtr = (u32 *)(headerBuf + 0x80);
            do {
                dstPtr[0] = srcPtr[0];
                dstPtr[1] = srcPtr[1];
                dstPtr[2] = srcPtr[2];
                dstPtr[3] = srcPtr[3];
                srcPtr += 4;
                dstPtr += 4;
            } while (srcPtr != (u32 *)(dataOffset + 0xc0));
            srcPtr = (u32 *)(dataOffset + 0xc0);
            dstPtr = (u32 *)(headerBuf + 0x100);
            do {
                dstPtr[0] = srcPtr[0];
                dstPtr[1] = srcPtr[1];
                dstPtr[2] = srcPtr[2];
                dstPtr[3] = srcPtr[3];
                srcPtr += 4;
                dstPtr += 4;
            } while (srcPtr != (u32 *)(dataOffset + 0x140));
            srcPtr = (u32 *)(dataOffset + 0x140);
            dstPtr = (u32 *)(headerBuf + 0x180);
            do {
                dstPtr[0] = srcPtr[0];
                dstPtr[1] = srcPtr[1];
                dstPtr[2] = srcPtr[2];
                dstPtr[3] = srcPtr[3];
                srcPtr += 4;
                dstPtr += 4;
            } while (srcPtr != (u32 *)(dataOffset + 0x1c0));
            write(fd, headerBuf, (u32)sectorCount * 0x80 + 0x80);
            MemFree((int)headerBuf, 0);
            write(fd, (void *)dataBuffer, dataSize + 0x7fU & 0xffffff80);
            close(fd);
            result = 1;
        }
    }
    return result;
}
/* Build a memory card file path */
char *BuildMemoryCardPath(char *outPath, int slotId, char *filename)
{
    u32 *prefixData;
    u32 swapVal;
    if (slotId != 0) {
        prefixData = (u32 *)&Stage_McPathPrefix1;
    } else {
        prefixData = (u32 *)&Stage_McPathPrefix0;
    }
    u32 prefix0 = prefixData[0];
    u32 prefix1 = prefixData[1];
    char prefixByte = *(char *)((int)prefixData + 5);
    char *pathEnd = outPath + 3;
    swapVal = (u32)pathEnd & 3;
    *(u32 *)(pathEnd - swapVal) =
        (*(u32 *)(pathEnd - swapVal) & -1 << (swapVal + 1) * 8) |
        (prefix0 >> (3 - swapVal) * 8);
    swapVal = (u32)outPath & 3;
    *(u32 *)(outPath - swapVal) =
        (*(u32 *)(outPath - swapVal) & 0xffffffffU >> (4 - swapVal) * 8) |
        (prefix0 << swapVal * 8);
    outPath[4] = (char)prefix1;
    outPath[5] = prefixByte;
    strcat(outPath, filename);
    return outPath;
}
/* -----------------------------------------------------------------------
   Memory card event handling
   ----------------------------------------------------------------------- */
/* Enable memory card events */
void EnableMemoryCardEvents(s32 slot)
{
    HandleMemoryCardEvent(slot, EnableEvent, 1);
}
/* Disable memory card events */
void DisableMemoryCardEvents(s32 slot)
{
    HandleMemoryCardEvent(slot, DisableEvent, 1);
}
/* Test memory card events */
void TestMemoryCardEvents(s32 slot)
{
    HandleMemoryCardEvent(slot, TestEvent, 0);
}
/* Handle memory card event wait loop */
int HandleMemoryCardEvent(int slot, int (*eventFunc)(s32), int waitForEvent)
{
    int result;
    int i;
    if (waitForEvent != 0) {
        EnterCriticalSection();
    }
    i = 0;
    do {
        result = eventFunc(*(s32 *)(slot + 0x14));
        if (result == 0) break;
        i++;
        slot += 4;
    } while (i < 4);
    if (waitForEvent != 0) {
        LeaveCriticalSection();
    }
    return result;
}
/* Wait for memory card operation to complete (returns status code) */
s32 TestMemoryCardDone(int slot)
{
    return HandleMemoryCardEventDone((s32 *)(slot + 0x14), 4);
}
/* Wait for specific memory card event in a loop */
s32 HandleMemoryCardEventDone(s32 *eventSlots, int slotCount)
{
    int eventStatus;
    int i;
    s32 *slotPtr;
    do {
        do {
            i = 0;
            slotPtr = eventSlots;
        } while (slotCount < 1);
        do {
            eventStatus = TestEvent(*slotPtr);
            if (eventStatus != 0) {
                return (s32)(&Stage_McEventStatusResults)[i];
            }
            i++;
            slotPtr++;
        } while (i < slotCount);
    } while (true);
}
/* -----------------------------------------------------------------------
   Memory card file handle management
   ----------------------------------------------------------------------- */
/* Allocate and initialize the memory card file handle array */
void InitMemoryCardFileHandles(int slot)
{
    s32 handleArray;
    int i;
    if (*(int *)(slot + 0x38) == 0) {
        handleArray = (s32)MemAlloc(0x40);
        *(s32 *)(slot + 0x38) = handleArray;
        for (i = 0; i < 0xf; i++) {
            s32 handle = (s32)MemAlloc(0x41);
            *(s32 *)(i * 4 + *(int *)(slot + 0x38)) = handle;
        }
        handleArray = (s32)MemAlloc(0x40);
        *(s32 *)(slot + 0x3c) = handleArray;
    }
}
/* Free unused file handles */
void FreeUnusedFileHandles(int slot)
{
    int i;
    for (i = *(int *)(slot + 0x2c); i < 0xf; i++) {
        s32 handle = (s32)MemFree((int)*(void **)(i * 4 + *(int *)(slot + 0x38)), 0);
        *(s32 *)(i * 4 + *(int *)(slot + 0x38)) = handle;
    }
    *(s32 *)(i * 4 + *(int *)(slot + 0x38)) = 0;
}
/* Free all file handles */
void DestroyAllFileHandles(int slot)
{
    int i;
    if (*(int *)(slot + 0x38) != 0) {
        MemFree((int)*(void **)(slot + 0x3c), 0);
        for (i = 0; i < *(int *)(slot + 0x2c); i++) {
            MemFree((int)*(void **)(i * 4 + *(int *)(slot + 0x38)), 0);
        }
        MemFree((int)*(void **)(slot + 0x38), 0);
        *(s32 *)(slot + 0x38) = 0;
    }
}
/* -----------------------------------------------------------------------
   Memory card state machine
   ----------------------------------------------------------------------- */
/* Run the memory card state machine and report result */
s32 RunMemoryCardStateMachine(int *slot)
{
    s32 action;
    s32 outFlags;
    s32 outStatus;
    s32 outData[2];
    (*(void (**)(void))(slot[0] + 0x44))();
    action = (*(int (**)(int *, s32 *, s32 *, s32 *))(slot[0] + 0x4c))(
        slot, &outFlags, &outStatus, outData
    );
    (*(void (**)(int *))(slot[0] + 0x48))(slot);
    if (action != 0) {
        if ((outStatus == 0) && (outData[0] != 0)) {
            return 1;
        }
        if (action != 0) {
            if (outFlags == 0) {
                if (outStatus == 0) {
                    if ((outData[0] == 0) && (action = 6, slot[9] == 1)) {
                        action = 5;
                    }
                } else {
                    action = 4;
                }
            } else {
                action = 3;
            }
            goto DONE;
        }
    }
    action = 2;
DONE:
    (*(void (**)(int *, s32))(slot[0] + 0x7c))(slot, action);
    return 0;
}
/* -----------------------------------------------------------------------
   Memory card object management
   ----------------------------------------------------------------------- */
/* Return vtable address for the MemoryCard object type */
int GetMemoryCardVtable(void)
{
    return (int)&Stage_MemoryCardVtable;
}
/* Write memory card sector data with proper byte alignment */
void WriteMemoryCardSector(int buffer, int mode)
{
    u32 *alignedPtr;
    u32 swapVal;
    s32 adjSize;
    if (mode == 0) {
        swapVal = (u32)(buffer + 9U) & 3;
        alignedPtr = (u32 *)((buffer + 9U) - swapVal);
        *alignedPtr = (*alignedPtr & -1 << (swapVal + 1) * 8) |
                      (0x40814081U >> (3 - swapVal) * 8);
        swapVal = (u32)(buffer + 6U) & 3;
        alignedPtr = (u32 *)((buffer + 6U) - swapVal);
        *alignedPtr = (*alignedPtr & 0xffffffffU >> (4 - swapVal) * 8) |
                      (0x40814081 << swapVal * 8);
        *(u8 *)(buffer + 10) = 0x81;
        *(u8 *)(buffer + 11) = 0x40;
    } else {
        adjSize = 0xd;
        if (*(u8 *)(mode + 0xe) - 0x38 < 2) {
            adjSize = 0xe;
        }
        *(u8 *)(buffer + 0x18) = 0x81;
        *(u8 *)(buffer + 0x19) = 0x40;
        swapVal = (u32)(buffer + 9U) & 3;
        alignedPtr = (u32 *)((buffer + 9U) - swapVal);
        *alignedPtr = (*alignedPtr & -1 << (swapVal + 1) * 8) |
                      (0x40814081U >> (3 - swapVal) * 8);
        swapVal = (u32)(buffer + 6U) & 3;
        alignedPtr = (u32 *)((buffer + 6U) - swapVal);
        *alignedPtr = (*alignedPtr & 0xffffffffU >> (4 - swapVal) * 8) |
                      (0x40814081 << swapVal * 8);
        swapVal = (u32)(buffer + 0xdU) & 3;
        alignedPtr = (u32 *)((buffer + 0xdU) - swapVal);
        *alignedPtr = (*alignedPtr & -1 << (swapVal + 1) * 8) |
                      (0x63824081U >> (3 - swapVal) * 8);
        swapVal = (u32)(buffer + 10U) & 3;
        alignedPtr = (u32 *)((buffer + 10U) - swapVal);
        *alignedPtr = (*alignedPtr & 0xffffffffU >> (4 - swapVal) * 8) |
                      (0x63824081 << swapVal * 8);
        swapVal = (u32)(buffer + 0x11U) & 3;
        alignedPtr = (u32 *)((buffer + 0x11U) - swapVal);
        *alignedPtr = (*alignedPtr & -1 << (swapVal + 1) * 8) |
                      (0x99828182U >> (3 - swapVal) * 8);
        swapVal = (u32)(buffer + 0xeU) & 3;
        alignedPtr = (u32 *)((buffer + 0xeU) - swapVal);
        *alignedPtr = (*alignedPtr & 0xffffffffU >> (4 - swapVal) * 8) |
                      (-0x667d7e7e << swapVal * 8);
        adjSize = atoi((char *)(mode + adjSize));
        adjSize = (adjSize - 1) * 2;
        *(u8 *)(buffer + 8) = Stage_McEncodingData[adjSize];
        *(u8 *)(buffer + 9) = Stage_McEncodingData2[adjSize];
    }
}
/* Create a new MemoryCard object (0x4c = 76 bytes) */
int CreateMemoryCard(s32 initParam1, s32 initParam2)
{
    void *obj;
    int vtable;
    obj = MemAlloc(0x4c);
    vtable = 0;
    if (obj != 0) {
        vtable = (int)GetEntityVtable2();
        (*(void (**)(void *, s32, s32))(vtable + 8))(obj, initParam1, initParam2);
        vtable = (int)obj;
    }
    return vtable;
}
/* Clear memory card object fields */
void ResetMemoryCardState(int slot)
{
    *(s32 *)(slot + 0x34) = 0;
    *(s32 *)(slot + 0x38) = 0;
    *(s32 *)(slot + 0x48) = 0;
}

/* ===== Merged from StageGrid.c ===== */
#define STAGE_COUNT 14
StageGridDimensions STAGE_GRID_DIMENSIONS[STAGE_COUNT] = {
    {1,5,true},     // Apartment
    {3,2,false},    // Pit
    {6,6,false},    // Kyoto
    {16,16,false},  // Natural
    {6,5,false},    // Happy
    {5,6,false},    // Violence
    {1,6,true},     // Tower
    {5,1,false},    // Palace
    {1,3,false},    // Flesh
    {1,2,false},    // Clockwork
    {3,1,false},    // Hallway
    {4,3,false},    // Heaven
    {4,5,false},    // Void
    {2,2,false}     // Park
};
struct simplePair STG00_CHUNK_MOODS[] = {
    { 0,-3},
    { 1, 1},
    {-1, 1},
    { 0, 0},
    {-5,-7},
};
struct simplePair STG01_CHUNK_MOODS[] = {
    { 7, 1},{ 1, 2},{ 8,-6},
    { 9, 0},{ 0,-9},{-8,-6},
};
struct simplePair STG02_CHUNK_MOODS[] = {
    { 0, 9},{ 1, 5},{ 2, 5},{ 2, 4},{ 2, 3},{ 3, 3},
    { 3, 2},{ 4, 2},{ 4, 1},{ 3, 1},{ 3, 0},{ 2, 0},
    { 2,-1},{ 1,-1},{ 1,-2},{ 1,-4},{ 9, 0},{ 1,-5},
    { 0,-5},{-1,-5},{-1,-4},{-1,-2},{-1,-1},{-2,-1},
    {-2, 0},{-3, 0},{-3, 1},{-4, 1},{-4, 2},{-3, 2},
    {-3, 3},{-2, 3},{-2, 4},{-2, 5},{-1, 5},{ 1, 3},
};
struct simplePair STG03_CHUNK_MOODS[] = {
    { 9, 0},{ 9, 0},{ 6, 7},{ 6, 6},{ 9, 0},{ 9, 0},{ 7, 6},{ 9, 0},
    { 9, 0},{ 9, 0},{ 7, 5},{ 9, 0},{ 8, 5},{ 9, 5},{ 9, 0},{ 9, 0},
    { 9, 0},{ 9, 0},{ 9, 0},{ 9, 4},{ 9, 3},{ 9, 2},{ 9, 1},{ 9, 0},
    { 9,-1},{ 9, 0},{ 9,-2},{ 9,-3},{ 9,-4},{ 9,-5},{ 8,-5},{ 7,-5},
    { 9, 0},{ 9, 0},{ 9, 0},{ 9, 0},{ 9, 0},{ 7,-6},{ 6,-6},{ 6,-7},
    {-6,-7},{-6,-6},{ 9, 0},{-7,-6},{-7,-5},{-8,-5},{-9,-5},{ 9, 0},
    { 9, 0},{ 9, 0},{ 9, 0},{-9,-4},{-9,-3},{-9,-2},{-9,-1},{-9, 0},
    {-9, 1},{-9, 2},{-9, 3},{-9, 4},{-9, 5},{-8, 5},{-7, 5},{-7, 6},
    { 9, 0},{ 9, 0},{ 9, 0},{ 9, 0},{-6, 6},{-6, 7},{-1, 7},{ 0, 7},
    { 1, 7},{ 1, 6},{ 2, 6},{ 3, 6},{ 4, 6},{ 5, 6},{ 5, 5},{ 9, 0},
    { 9, 0},{ 5, 4},{ 6, 4},{ 7, 4},{ 8, 4},{ 8, 3},{ 8, 2},{ 8, 1},
    { 8, 0},{ 8,-1},{ 8,-2},{ 8,-3},{ 8,-4},{ 7,-4},{ 6,-4},{ 9, 0},
    { 9, 0},{ 5,-4},{ 5,-5},{ 5,-6},{ 4,-6},{ 3,-6},{ 2,-6},{ 1,-6},
    {-1,-6},{-2,-6},{-3,-6},{-4,-6},{-5,-6},{ 9, 0},{-5,-5},{ 9, 0},
    { 9, 0},{ 9, 0},{-5,-4},{-6,-4},{-7,-4},{-8,-4},{-8,-3},{-8,-2},
    {-8,-1},{-8, 0},{-8, 1},{-8, 2},{-8, 3},{-8, 4},{-7, 4},{ 9, 0},
    { 9, 0},{-6, 4},{-5, 4},{-5, 5},{-5, 6},{-4, 6},{-3, 6},{ 9, 0},
    {-2, 6},{-1, 6},{ 3, 5},{ 4, 5},{ 4, 4},{ 4, 3},{ 5, 3},{ 9, 0},
    { 9, 0},{ 9, 0},{ 6, 3},{ 9, 0},{ 7, 3},{ 7, 2},{ 7,-1},{ 7,-2},
    { 7,-3},{ 6,-3},{ 5,-3},{ 4,-4},{ 4,-5},{ 3,-5},{ 9, 0},{ 9, 0},
    { 9, 0},{ 2,-5},{-2,-5},{-3,-5},{-4,-5},{-4,-4},{ 9, 0},{-5,-3},
    {-6,-3},{-7,-3},{-7,-2},{-7,-1},{-7, 2},{ 9, 0},{ 9, 0},{ 9, 0},
    { 9, 0},{-7, 3},{-6, 3},{-5, 3},{-4, 3},{-4, 4},{-4, 5},{-3, 5},
    { 0, 5},{ 3, 4},{ 5, 2},{ 6, 2},{ 6, 1},{ 6, 0},{ 6,-1},{ 9, 0},
    { 9, 0},{ 6,-2},{ 5,-2},{ 4,-2},{ 3,-2},{ 3,-3},{ 3,-4},{ 2,-4},
    {-2,-4},{-3,-4},{-3,-3},{ 9, 0},{-3,-2},{-4,-2},{-5,-2},{ 9, 0},
    { 9, 0},{ 9, 0},{-6,-2},{-6,-1},{-6, 0},{-6, 1},{-6, 2},{-5, 2},
    {-3, 4},{-1, 4},{ 0, 4},{ 1, 4},{ 5, 1},{ 5, 0},{ 5,-1},{ 9, 0},
    { 9, 0},{ 4,-1},{ 3,-1},{ 2,-2},{ 2,-3},{ 1,-3},{-1,-3},{-2,-3},
    {-2,-2},{-3,-1},{-4,-1},{-5,-1},{-5, 0},{-5, 1},{ 9, 0},{ 9, 0},
    { 9, 0},{ 9, 0},{ 4, 0},{-4, 0},{ 9, 0},{ 9, 0},{ 9, 0},{ 9, 0},
    { 9, 0},{ 9, 0},{ 9, 0},{ 9, 0},{ 9, 0},{ 0, 0},{ 0, 0},{ 9, 0},
};
struct simplePair STG04_CHUNK_MOODS[] = {
    { 5, 9},{ 5, 8},{ 7, 7},{ 4, 7},{ 3, 7},{ 9, 0},
    { 2, 7},{-2, 7},{-3, 7},{-4, 7},{-5, 7},{-5, 8},
    {-5, 9},{-4, 9},{-3, 9},{-2, 9},{-1, 9},{ 9, 0},
    { 1, 9},{ 2, 9},{ 3, 9},{ 4, 9},{ 4, 8},{ 3, 8},
    { 2, 8},{ 1, 8},{ 0, 8},{-1, 8},{-2, 8},{ 9, 0},
};
struct simplePair STG05_CHUNK_MOODS[] = {
    { 3,-7},{ 3,-8},{ 4,-8},{ 5,-8},{ 5,-9},
    { 9, 0},{ 4,-9},{ 3,-9},{ 2,-9},{ 1,-9},
    {-1,-8},{-1,-9},{-2,-9},{-3,-9},{-4,-9},
    { 9, 0},{-5,-9},{-5,-8},{-4,-8},{-3,-8},
    { 9, 0},{-3,-7},{-2,-7},{-1,-7},{ 0,-7},
    { 1,-7},{ 2,-7},{ 2,-8},{ 1,-8},{ 0,-8},
};
struct simplePair STG06_CHUNK_MOODS[] = {
    { 0,-4},
    { 0,-2},
    { 0,-1},
    { 0, 1},
    { 0, 2},
    { 1, 2},
};
struct simplePair STG07_CHUNK_MOODS[] = {
    { 2, 2},{ 2, 1},{ 1, 0},{-1, 0},{-2, 1},
};
struct simplePair STG08_CHUNK_MOODS[] = {
    { 7, 0},
    {-7, 0},
    {-7, 1},
};
struct simplePair STG09_CHUNK_MOODS[] = {
    { 0,-6},
    {-2,-8},
};
struct simplePair STG10_CHUNK_MOODS[] = {
    { 5,-7},{ 4,-7},{-4,-7},
};
struct simplePair STG11_CHUNK_MOODS[] = {
    { 9, 0},{ 9, 0},{ 9, 0},{ 9, 0},
    { 9, 0},{-4, 8},{-3, 8},{ 9, 0},
    { 9, 0},{ 9, 0},{ 9, 0},{ 9, 0},
};
struct simplePair STG12_CHUNK_MOODS[] = {
    { 9, 0},{ 9, 0},{ 9, 0},{ 9, 0},
    { 9, 0},{ 6, 5},{ 6,-5},{ 9, 0},
    {-6,-5},{-6, 5},{ 0, 6},{ 9, 0},
    { 9, 0},{ 4,-3},{-4,-3},{ 9, 0},
    { 9, 0},{ 9, 0},{ 9, 0},{ 9, 0},
};
struct simplePair STG13_CHUNK_MOODS[] = {
    {-1, 3},{ 9, 0},
    {-1, 2},{-2, 2},
};
MoodGraphPoint* STAGE_CHUNK_MOODS[] = {
    (MoodGraphPoint*) STG00_CHUNK_MOODS,
    (MoodGraphPoint*) STG01_CHUNK_MOODS,
    (MoodGraphPoint*) STG02_CHUNK_MOODS,
    (MoodGraphPoint*) STG03_CHUNK_MOODS,
    (MoodGraphPoint*) STG04_CHUNK_MOODS,
    (MoodGraphPoint*) STG05_CHUNK_MOODS,
    (MoodGraphPoint*) STG06_CHUNK_MOODS,
    (MoodGraphPoint*) STG07_CHUNK_MOODS,
    (MoodGraphPoint*) STG08_CHUNK_MOODS,
    (MoodGraphPoint*) STG09_CHUNK_MOODS,
    (MoodGraphPoint*) STG10_CHUNK_MOODS,
    (MoodGraphPoint*) STG11_CHUNK_MOODS,
    (MoodGraphPoint*) STG12_CHUNK_MOODS,
    (MoodGraphPoint*) STG13_CHUNK_MOODS
};
/// Returns the total number of stage grid definitions.
s32 GetStageGridCount(void) {
    return STAGE_COUNT;
}
/// Returns the stage grid dimensions table, optionally writing the count.
StageGridDimensions *GetStageGridDimensionsTable(s32 *count) {
    if (count != NULL) {
        *count = STAGE_COUNT;
    }
    return STAGE_GRID_DIMENSIONS;
}
/// Returns the grid dimensions for a specific stage index.
StageGridDimensions *GetStageGridDimensions(s32 index) {
    return GetStageGridDimensionsTable(NULL) + index;
}
/**
 * Searches all stages for a chunk whose mood matches the given mood graph point.
 * Returns the stage index on success, or -1 if not found.
 * On success, writes the chunk's column/row coordinates into ret.
 */
s32 GetStageChunkFromMood(StageChunk *ret, MoodGraphPoint *mood) {
    s32 stageIdx;
    s32 row, col;
    MoodGraphPoint **moodTable;
    short *moodEntry;
    stageIdx = 0;
    moodTable = STAGE_CHUNK_MOODS;
    do {
        moodEntry = (short *)*moodTable;
        for (row = 0; row < STAGE_GRID_DIMENSIONS[stageIdx].rows; row++) {
            for (col = 0; col < STAGE_GRID_DIMENSIONS[stageIdx].columns; col++) {
                if (mood->value == *moodEntry) {
                    ret->column = (s8)col;
                    ret->row = (s8)row;
                    return stageIdx;
                }
                moodEntry++;
            }
        }
        stageIdx++;
        moodTable++;
    } while (stageIdx < STAGE_COUNT);
    return -1;
}
/**
 * Looks up the mood contribution for a given stage and chunk coordinate.
 */
MoodGraphPoint *GetMoodFromStageChunk(s32 stage, StageChunk *chunk) {
    return &STAGE_CHUNK_MOODS[stage][chunk->column + chunk->row * STAGE_GRID_DIMENSIONS[stage].columns];
}

