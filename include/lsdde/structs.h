#ifndef LSDDE_STRUCTS_H
#define LSDDE_STRUCTS_H

/* ------------------------------------------------------------------ */
/* Struct definitions matching Ghidra decompiler types                 */
/* Generated from Ghidra program SLPS_015.56                           */
/* Field names: p=pointer, n=int, dw=uint                              */
/* ------------------------------------------------------------------ */

/* EntityObj: main entity object (0x100 = 256 bytes)                   */
/* Accessed as int* in legacy code; entity[N] => offset N*4            */
typedef struct {
    void *pVtbl;            /* 0x00 */
    int   nPosX;            /* 0x04 */
    int   nPosY;            /* 0x08 */
    int   nPosZ;            /* 0x0C */
    int   nDist;            /* 0x10 */
    void *pData;            /* 0x14 */
    void *pWorld;           /* 0x18 */
    int   nField_1c;        /* 0x1C */
    int   nField_20;        /* 0x20 */
    int   nTimer;           /* 0x24 */
    int   nConfig;          /* 0x28 */
    int   nVabIdx;          /* 0x2C */
    int   nField_30;        /* 0x30 */
    int   nField_34;        /* 0x34 */
    int   nField_38;        /* 0x38 */
    void *pOtherEntity;     /* 0x3C */
    void *pSomePtr;         /* 0x40 */
    int   nState;           /* 0x44 */
    int   nStateFlag;       /* 0x48 */
    void *pObject;          /* 0x4C */
    int   nField_50;        /* 0x50 */
    int   nSoundId;         /* 0x54 */
    void *pChild;           /* 0x58 */
    int   nField_5c;        /* 0x5C */
    int   nField_60;        /* 0x60 */
    int   nField_64;        /* 0x64 */
    int   nObjState;        /* 0x68 (low 16 bits = short) */
    int   nSubTypeCount;    /* 0x6C */
    int   nChildCount;      /* 0x70 */
    int   nConfigVal;       /* 0x74 */
    int   nRndVal;          /* 0x78 */
    void *pSubObj;          /* 0x7C */
    int   nCondition;       /* 0x80 */
    int   nSubState;        /* 0x84 */
    void *pChild2;          /* 0x88 */
    void *pSubVtbl;         /* 0x8C */
    int   nParam2;          /* 0x90 */
    void *pContainer;       /* 0x94 */
    int   nAnimIdx;         /* 0x98 */
    void *pMoreData;        /* 0x9C */
    int   field_0xa0;       /* 0xA0 */
    int   field_0xa4;       /* 0xA4 */
    int   field_0xa8;       /* 0xA8 */
    int   field_0xac;       /* 0xAC */
    int   field_0xb0;       /* 0xB0 */
    int   field_0xb4;       /* 0xB4 */
    int   field_0xb8;       /* 0xB8 */
    int   field_0xbc;       /* 0xBC */
    int   field_0xc0;       /* 0xC0 */
    int   field_0xc4;       /* 0xC4 */
    int   field_0xc8;       /* 0xC8 */
    int   field_0xcc;       /* 0xCC */
    int   field_0xd0;       /* 0xD0 */
    int   field_0xd4;       /* 0xD4 */
    int   field_0xd8;       /* 0xD8 */
    int   field_0xdc;       /* 0xDC */
    int   field_0xe0;       /* 0xE0 */
    int   field_0xe4;       /* 0xE4 */
    int   field_0xe8;       /* 0xE8 */
    int   field_0xec;       /* 0xEC */
    int   nActiveFlag;      /* 0xF0 */
    int   nResultFlag;      /* 0xF4 */
    int   nReturnVal;       /* 0xF8 */
    int   nSubType;         /* 0xFC */
} EntityObj;

/* EvyMsg: entity work/message object (0x4C = 76 bytes)               */
/* Passed as param_2 to entity behavior functions                      */
typedef struct {
    int nMsgType;           /* 0x00 */
    int nCounter;           /* 0x04 */
    int field_0x08;         /* 0x08 */
    int field_0x0c;         /* 0x0C */
    int nReturn;            /* 0x10 */
    int field_0x14;         /* 0x14 */
    int field_0x18;         /* 0x18 */
    int nDisp;              /* 0x1C */
    int nParam1;            /* 0x20 */
    int field_0x24;         /* 0x24 */
    int field_0x28;         /* 0x28 */
    int field_0x2c;         /* 0x2C */
    int nParam2;            /* 0x30 */
    int nParam3;            /* 0x34 */
    int field_0x38;         /* 0x38 */
    int field_0x3c;         /* 0x3C */
    int field_0x40;         /* 0x40 */
    int nParam4;            /* 0x44 */
    int nParam5;            /* 0x48 */
} EvyMsg;

/* RenderCtx: rendering context (0x50 = 80 bytes)                      */
/* Passed as param_1 to rendering/init functions                       */
typedef struct {
    void *pVtbl;            /* 0x00 */
    int   nField_4;         /* 0x04 */
    void *pStageGrid;       /* 0x08 */
    void *pStageGridAlloc;  /* 0x0C */
    void *pChunkData;       /* 0x10 */
    int   nField_14;        /* 0x14 */
    void *pField_18;        /* 0x18 */
    int   nField_1c;        /* 0x1C */
    int   nField_20;        /* 0x20 */
    int   nField_24;        /* 0x24 */
    int   nField_28;        /* 0x28 */
    int   nField_2c;        /* 0x2C */
    int   nField_30;        /* 0x30 */
    void *pValueParam;      /* 0x34 */
    void *pValueParam2;     /* 0x38 */
    uint  dwModeFlags;      /* 0x3C */
    void *pStageChunk;      /* 0x40 */
    void *pTexture;         /* 0x44 */
    void *pTmdModel;        /* 0x48 */
    int   nField_4c;        /* 0x4C */
} RenderCtx;

/* StageAlloc: stage allocation block (0x14 = 20 bytes)                */
/* Passed as param_2 to Stage_Init                                     */
typedef struct {
    void *pVtbl;            /* 0x00 */
    int   field_0x04;       /* 0x04 */
    void *pStageGrid;       /* 0x08 */
    void *pChunkList;       /* 0x0C */
    void *pChunkData;       /* 0x10 */
} StageAlloc;

/* StageGridChannel: one channel in StageGrid (0x1C = 28 bytes)       */
/* Channel array starts at offset 0xEC inside StageGrid, 7 entries    */
typedef struct {
    int   field_0;          /* 0x00 */
    int   field_4;          /* 0x04 */
    int   field_8;          /* 0x08 */
    int   field_C;          /* 0x0C */
    void *pField_10;        /* 0x10 */
    int   field_14;         /* 0x14 */
    int   field_18;         /* 0x18 */
} StageGridChannel;

/* StageGrid: extends EntityObj (first 59 fields 0x00-0xE8 shared)   */
/* Channel data starts at 0xEC (overlapping EntityObj's last fields)  */
/* Total size ~0x1E0 = 480 bytes                                      */
typedef struct {
    /* EntityObj base fields (0x00-0xE8, 59 fields)                    */
    void *pVtbl;            /* 0x00 */
    int   nPosX;            /* 0x04 */
    int   nPosY;            /* 0x08 */
    int   nPosZ;            /* 0x0C */
    int   nDist;            /* 0x10 */
    void *pData;            /* 0x14 */
    void *pWorld;           /* 0x18 */
    int   nField_1c;        /* 0x1C */
    int   nField_20;        /* 0x20 */
    int   nTimer;           /* 0x24 */
    int   nConfig;          /* 0x28 */
    int   nVabIdx;          /* 0x2C */
    int   nField_30;        /* 0x30 */
    int   nField_34;        /* 0x34 */
    int   nField_38;        /* 0x38 */
    void *pOtherEntity;     /* 0x3C */
    void *pSomePtr;         /* 0x40 */
    int   nState;           /* 0x44 */
    int   nStateFlag;       /* 0x48 */
    void *pObject;          /* 0x4C */
    int   nField_50;        /* 0x50 */
    int   nSoundId;         /* 0x54 */
    void *pChild;           /* 0x58 */
    int   nField_5c;        /* 0x5C */
    int   nField_60;        /* 0x60 */
    int   nField_64;        /* 0x64 */
    int   nObjState;        /* 0x68 */
    int   nSubTypeCount;    /* 0x6C */
    int   nChildCount;      /* 0x70 */
    int   nConfigVal;       /* 0x74 */
    int   nRndVal;          /* 0x78 */
    void *pSubObj;          /* 0x7C */
    int   nCondition;       /* 0x80 */
    int   nSubState;        /* 0x84 */
    void *pChild2;          /* 0x88 */
    void *pSubVtbl;         /* 0x8C */
    int   nParam2;          /* 0x90 */
    void *pContainer;       /* 0x94 */
    int   nAnimIdx;         /* 0x98 */
    void *pMoreData;        /* 0x9C */
    int   field_0xa0;       /* 0xA0 */
    int   field_0xa4;       /* 0xA4 */
    int   field_0xa8;       /* 0xA8 */
    int   field_0xac;       /* 0xAC */
    int   field_0xb0;       /* 0xB0 */
    int   field_0xb4;       /* 0xB4 */
    int   field_0xb8;       /* 0xB8 */
    int   field_0xbc;       /* 0xBC */
    int   field_0xc0;       /* 0xC0 */
    int   field_0xc4;       /* 0xC4 */
    int   field_0xc8;       /* 0xC8 */
    int   field_0xcc;       /* 0xCC */
    int   field_0xd0;       /* 0xD0 */
    int   field_0xd4;       /* 0xD4 */
    int   field_0xd8;       /* 0xD8 */
    int   field_0xdc;       /* 0xDC */
    int   field_0xe0;       /* 0xE0 */
    int   field_0xe4;       /* 0xE4 */
    int   field_0xe8;       /* 0xE8 */
    /* StageGrid-specific: 7 channels at 0xEC (overlaps EntityObj end) */
    StageGridChannel channels[7]; /* 0xEC-0x1AF */
    /* Extension fields (0x1B0+) */
    int   sg_0x1B0;         /* 0x1B0 */
    int   sg_0x1B4;         /* 0x1B4 */
    int   sg_0x1B8;         /* 0x1B8 */
    int   sg_0x1BC;         /* 0x1BC */
    int   sg_0x1C0;         /* 0x1C0 */
    int   sg_0x1C4;         /* 0x1C4 */
    int   sg_0x1C8;         /* 0x1C8 */
    int   sg_0x1CC;         /* 0x1CC */
    int   sg_0x1D0;         /* 0x1D0 */
    int   sg_0x1D4;         /* 0x1D4 */
    int   sg_0x1D8;         /* 0x1D8 */
    int   sg_0x1DC;         /* 0x1DC */
    int   sg_0x1E0;         /* 0x1E0 */
} StageGrid;

#endif /* LSDDE_STRUCTS_H */
