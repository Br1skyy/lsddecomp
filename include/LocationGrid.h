#ifndef LOCATION_GRID
#define LOCATION_GRID

typedef struct LocationGridDimensions{
    s16 columns;
    s16 rows;
    bool isVertical;
}LocationGridDimensions;

typedef struct LocationChunk{
    s8 column;
    s8 row;
}LocationChunk;

struct simplePair{
    s8 x; s8 y;
};

extern LocationGridDimensions LOCATION_GRID_DIMENSIONS[];

extern MoodGraphPoint* LOCATION_CHUNK_MOODS[];

extern LocationGridDimensions *GetLocationGridDimensionsTable(s32 *unknown);

extern LocationGridDimensions *GetLocationGridDimensions(s32 index);

// Gets the location + chunk for a mood graph point. Returns stage index.
extern s32 GetLocationChunkFromMood(LocationChunk *ret, MoodGraphPoint *mood);

// Gets the mood contribution for a given location and chunk.
extern MoodGraphPoint *GetMoodFromLocationChunk(s32 location, LocationChunk *chunk);

#endif
