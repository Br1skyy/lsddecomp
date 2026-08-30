# Data Formats

This document describes the various file formats used in LSD: Dream Emulator and this reconstruction project. Grab a coffee, it's a long one.

## PS1 Executable Format

### SLPS_015.56

The main executable follows the standard PS-EXE format:

- **Header** (0x000-0x800): PS-EXE identification and load information (2048-byte header; the text section begins at 0x800)
- **Text Section**: Code segment
- **Data Section**: Static data
- **BSS Section**: Uninitialized data

## Disc Image Format

### CD-ROM Structure

The game disc uses standard PS1 CD-ROM layout:

- **SYSTEM.CNF** - Boot configuration
- **SLPS_015.56** - Main executable
- **CDI/** - Game data directory
  - **STG00/ through STG13/** - Stage data
  - **ETC/** - Shared assets
  - **IMG1/, IMG2/** - Texture data
  - **SND/** - Sound effects
  - **FILM/** - Video content
  - **CARD/** - Memory card icons

## Location Data Format

### Location Structure

Each stage directory contains:

- **Level Data** - 3D environment data
- **Entity Data** - Entity placements and configurations
- **Texture Data** - Location-specific textures
- **Collision Data** - Physics and interaction boundaries

### Chunk System

Stage maps are built from a per-stage grid of chunks (grid dimensions vary per stage - e.g. Kyoto (STG02) is 6x6, The Natural World (STG03) is 16x16; see LOCATION_GRID_DIMENSIONS in LocationGrid2.c for the full wiki-canonical list):

- **SIZEOF_CHUNK_DATA (0xEC)** - size of the chunk data block that precedes the spawn entries in memory
- **SPAWN_ENTRY_STRIDE (0x1C = 28 bytes)** - fixed stride between spawn-point entries (`GetChunkSpawnTable` + index * stride)
- **LocationGridChannel (0x1C = 28 bytes)** - 7 channels inside the LocationGrid struct, starting at offset 0xEC
- **Type identifiers** - chunk classification (ground, wall, etc.)

There is no fixed "7x7" chunk layout. The grid is stage-dependent, which made our first attempt to document it a fun afternoon of chasing our own tail.

## Entity Data Format

### Entity Structure

Entities use a 0x100-byte structure:

```
Offset 0x00: vtable pointer
Offset 0x04: position X
Offset 0x08: position Y  
Offset 0x0C: position Z
Offset 0x10: distance
Offset 0x14: data pointer
Offset 0x18: world pointer
Offset 0x1C-0x24: various fields
Offset 0x24: timer
Offset 0x28: configuration
Offset 0x2C: VAB index
Offset 0x44: state
Offset 0x48: state flag
Offset 0x4C: object pointer
Offset 0x54: sound ID
Offset 0x68: object state
Offset 0x6C: subtype count
Offset 0x70: child count
Offset 0x84: sub state
Offset 0x90: parameter 2
Offset 0x98: animation index
Offset 0xF0: active flag
Offset 0xF4: result flag
Offset 0xF8: return value
Offset 0xFC: subtype
```

## Audio Data Format

### SPU Voice Structure

Each voice uses 0xAC bytes:

```
Offset 0x00: reserved
Offset 0x04: data pointer (current read position)
Offset 0x08: data base pointer (start of sequence)
Offset 0x0C: loop start pointer
Offset 0x10: mode
Offset 0x11: key off
Offset 0x12: channel index
Offset 0x13: effect 1 type
Offset 0x14: effect 2 type
Offset 0x15: volume parameter
Offset 0x16: SFX type
Offset 0x17-0x26: volume[16] (per-channel volume)
Offset 0x27: damper mode
Offset 0x28: damper counter
Offset 0x29: effect change count
Offset 0x2A: volume change count
Offset 0x2B: key on
Offset 0x2C-0x3B: channel order[16]
Offset 0x3C: reserved
Offset 0x3E: fade start volume
Offset 0x40: fade target volume
Offset 0x42: fade delta
Offset 0x46: panning
Offset 0x4A: seq type
Offset 0x4C: sound bank id
Offset 0x4E-0x6D: effect volume[16]
Offset 0x6E: play speed
Offset 0x70: play speed default
Offset 0x72: play speed backup
Offset 0x74: volume L
Offset 0x76: volume R
Offset 0x78: pitch
Offset 0x7C: initial data offset
Offset 0x80: seq offset
Offset 0x84: sample rate
Offset 0x88: current data offset
Offset 0x8C: sample rate backup
Offset 0x90: flags
Offset 0x94: fade target volume 2
Offset 0x98: fade total frames
Offset 0xA8: pan volume L
Offset 0xAA: pan volume R
```

Voice flags (offset 0x90): bit 0 = key on, bit 1 = needs update, bit 2 = reset pending, bit 3 = pending key on, bit 4 = fade active, bit 5 = fade reverse, bit 9 = stopped.

## Dream System Data

### Dream State Structure

The DreamSys structure manages dream progression:

```
Offset 0x04-0x44: unknown values
Offset 0x48: array of 32 pointers (various objects)
Offset 0x58-0x64: object pointers
Offset 0x68: is flashback session
Offset 0x6C-0x84: mode flags and link states
Offset 0x84: link type
Offset 0x88-0x9C: camera motion parameters
Offset 0xA0-0xB8: flashback and effect counters
Offset 0xB8: sound ID for links
Offset 0xBC: sound handle
Offset 0xC4: flashback reverb flag
Offset 0xC8: flashback flag
Offset 0xCC: flashback buffer start
Offset 0x120-0x130: timer and tunnel coordinates
Offset 0x164: current stage
Offset 0x16C: link coordinates
Offset 0x17C: current year
Offset 0x180: current day (also mood count)
Offset 0x184-0x190: flashback unlock scores
Offset 0x880-0x908: unknown values and staircase data
```

## Configuration Files

### disk.xml

ISO generation configuration:
- File layout specification
- Volume label and filesystem parameters
- Boot file references

### splat.slps01556.lsdde.yaml

Symbol map configuration:
- Address to symbol mappings
- Function definitions
- Data structure definitions

### symbols.slps01556.lsdde.txt

Symbol mappings:
- Text format symbol table
- Address ranges
- Type information

## Save Data Format

### Memory Card Structure

Save files contain:

- **Header** - Identification and version
- **Player Data** - Progress and statistics
- **Dream Journal** - Visited areas and events
- **Checksum** - Data integrity verification (nobody knows who calls it, but its there)
