#ifndef STAGE_GRID
#define STAGE_GRID
// Deprecated shim - use LocationGrid.h (wiki canonical: Locations/Fields)
// This file is kept for backward compatibility and will be removed.
#include "LocationGrid.h"
// Provide backward compat aliases
#ifndef LOCATION_GRID_SHIM
#define StageGridDimensions LocationGridDimensions
#define StageChunk LocationChunk
#define STAGE_GRID_DIMENSIONS LOCATION_GRID_DIMENSIONS
#define STAGE_CHUNK_MOODS LOCATION_CHUNK_MOODS
#endif
#endif
