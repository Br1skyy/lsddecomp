#ifndef STAGE_GRID
#define STAGE_GRID

typedef struct StageGridDimensions{
    s16 columns;
    s16 rows;
    bool isVertical;
}StageGridDimensions;

typedef struct StageChunk{
    s8 column;
    s8 row;
}StageChunk;

struct simplePair{
    s8 x; s8 y;
};

extern StageGridDimensions STAGE_GRID_DIMENSIONS[];

extern MoodGraphPoint* STAGE_CHUNK_MOODS[];

extern StageGridDimensions *GetStageGridDimensionsTable(s32 *unknown);

extern StageGridDimensions *GetStageGridDimensions(s32 index);

// Gets the stage + chunk for a mood graph point. Returns stage index.
extern s32 GetStageChunkFromMood(StageChunk *ret, MoodGraphPoint *mood);

// Gets the mood contribution for a given stage and chunk.
extern MoodGraphPoint *GetMoodFromStageChunk(s32 stage, StageChunk *chunk);

#endif
