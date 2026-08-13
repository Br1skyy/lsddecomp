# Game Overview

## LSD: Dream Emulator

LSD: Dream Emulator is a PlayStation 1 game released in 1998 by Asmik Ace Entertainment. It's a surreal exploration game based on a dream journal by Japanese multimedia artist Osamu Sato. No, we still don't know what any of it means.

## Game Architecture

### Core Systems

The game is built around several interconnected systems:

- **Dream System** - Manages dream progression, stage linking, and flashback sequences
- **Entity System** - Handles all interactive objects and creatures in the dream world
- **Rendering System** - PS1 GPU-based 3D rendering with limited draw distance
- **Audio System** - SPU-based sound and CD audio playback
- **Navigation System** - Menu interfaces and memory card management

### Memory Layout

The game uses a fixed memory map (defined by the reconstruction's lsdde.ld; the PS1 has no MMU, so these are physical addresses):

- **0x80010000**: Base of `.main` - code, rodata, data, sdata, sbss, and bss all grow upward from here
- **~0x800B0A90**: End of .main/.bss (`main_BSS_END`); the resident callback tables (CdCallbackTable at 0x800B0A90, VSyncCallbackTable at 0x800B0AA0, IntrCallbackTable at 0x800B0AE0) sit just after it
- **0x800C82FC**: Dynamic heap base (set up by BMemPMgrInit in System.c, ~0x137000 bytes)
- **0x1F801000-0x1F803FC0**: Hardware registers (GPU, SPU, CD-ROM, joypad, timers, DMA, serial)

Note: data and static variables do NOT live at 0x8010xxxx. They sit inside the `.main` section right after the text. The exact end of `.main` depends on the link; see lsdde.ld and System.c. (We made this mistake once so you dont have to.)

### Game Loop

The main game loop follows this pattern:

1. **VSync Interrupt** - Wait for vertical blank
2. **Input Processing** - Read controller state
3. **Game Logic Update** - Update entities, physics, AI
4. **Rendering** - Draw frame to GPU
5. **Audio Update** - Process sound and music
6. **Memory Management** - Clean up unused resources

## Stage System

### World Structure

The dream world is divided into 14 stages (STG00-STG13 on the disc; see STAGE_GRID_DIMENSIONS in StageGrid2.c). Each stage has its own grid dimensions:

| Stage | Grid (cols x rows) |
|-------|--------------------|
| Apartment | 1 x 5 |
| Pit | 3 x 2 |
| Kyoto | 6 x 6 |
| Natural | 16 x 16 |
| Happy | 6 x 5 |
| Violence | 5 x 6 |
| Tower | 1 x 6 |
| Palace | 5 x 1 |
| Flesh | 1 x 3 |
| Clockwork | 1 x 2 |
| Hallway | 3 x 1 |
| Heaven | 4 x 3 |
| Void | 4 x 5 |
| Park | 2 x 2 |

### Stage Linking

Stages are connected through various mechanisms:

- **Walking** - Natural boundaries and transitions
- **Staircases** - Teleportation points with specific animations
- **Tunnels** - Direct connections between distant areas
- **Flashbacks** - Special sequences triggered by conditions

## Entity System

### Entity Types

The game uses a hierarchical entity system:

- **Type 0** - Basic environmental objects
- **Type 1** - Interactive objects with collision
- **Type 2** - Particle systems and effects
- **Type 3** - Complex entities with behaviors
- **Special Types** - Player character, cameras, etc.

### Entity Behavior

Entities can have multiple behaviors:

- **Wander** - Random movement patterns
- **Chase** - Follow the player
- **Flee** - Avoid the player
- **Stationary** - Fixed position with animations
- **Patrol** - Follow predefined paths

## Rendering System

### Graphics Pipeline

The rendering system uses the PS1's GPU:

1. **Culling** - Remove off-screen entities
2. **Sorting** - Order entities by depth
3. **Transform** - Apply world and camera transforms
4. **Rasterization** - Convert to GPU commands
5. **Display** - Send to GPU for rendering

### Visual Style

The game uses distinctive visual elements:

- **Low-poly 3D models** - Characteristic PS1 aesthetic
- **Texture mapping** - Limited texture resolution
- **Gouraud shading** - Smooth lighting
- **Limited draw distance** - Fog and distance culling
- **Surreal textures** - Dreamlike, abstract imagery

## Audio System

### Sound Architecture

The audio system uses both SPU and CD audio:

- **SPU Audio** - Sound effects and short samples
- **CD Audio** - Background music and ambient tracks
- **Voice System** - 24-voice SPU with reverb
- **Sound Banks** - Organized sound effect libraries

### Music System

The game features:

- **Dynamic music** - Changes based on location and events
- **Sound effects** - Environmental and interaction sounds
- **Ambient audio** - Background atmosphere
- **Spatial audio** - 3D positioning for sounds

## Save System

### Memory Card

The game uses PS1 memory cards for saving. Yes, memory cards. Those little squares you had to actually plug into the controller.

- **Dream Journal** - Records visited dreams
- **Progress Tracking** - Unlocks and achievements
- **High Scores** - Navigation challenges
- **Settings** - Game configuration

### Save Data Structure

Save files contain:

- **Player Progress** - Days played, areas visited
- **Unlock Status** - Available content
- **Statistics** - Various game metrics
- **Checksums** - Data integrity verification
