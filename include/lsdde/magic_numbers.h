#ifndef MAGIC_NUMBERS_H
#define MAGIC_NUMBERS_H

/* ------------------------------------------------------------------ */
/* Category 1: GPU Primitive Format IDs                                */
/* Used in stubs.c and Other.c for PS1 GPU primitive types             */
/* ------------------------------------------------------------------ */

/* GPU Primitive Types - libgpu style */
#define PRIM_TRI 0x20
#define PRIM_QUAD 0x28
#define PRIM_TRI_TEXTURED 0x24
#define PRIM_QUAD_TEXTURED 0x2C
#define PRIM_TRI_SHADE 0x30
#define PRIM_QUAD_SHADE 0x38
#define PRIM_TRI_SHADE_TEX 0x34
#define PRIM_QUAD_SHADE_TEX 0x3C
#define PRIM_GOURAUD_TRI 0x30
#define PRIM_GOURAUD_QUAD 0x38
#define PRIM_GOURAUD_TRI_TEX 0x34
#define PRIM_GOURAUD_QUAD_TEX 0x3C

/* Primitive header bytes (packet size/type) */
#define PRIM_HDR_TRI 0x20
#define PRIM_HDR_QUAD 0x28
#define PRIM_HDR_GOURAUD_TRI 0x30
#define PRIM_HDR_GOURAUD_QUAD 0x38
#define PRIM_HDR_TEXTURED_TRI 0x24
#define PRIM_HDR_TEXTURED_QUAD 0x2C
#define PRIM_HDR_GOURAUD_TEX_TRI 0x34
#define PRIM_HDR_GOURAUD_TEX_QUAD 0x3C

/* Full primitive format words (high 16 bits + type) */
#define PRIM_FMT_TRI 0x20000000
#define PRIM_FMT_QUAD 0x28000000
#define PRIM_FMT_GOURAUD_TRI 0x30000000
#define PRIM_FMT_GOURAUD_QUAD 0x38000000
#define PRIM_FMT_TEXTURED_TRI 0x24000000
#define PRIM_FMT_TEXTURED_QUAD 0x2C000000
#define PRIM_FMT_GOURAUD_TEX_TRI 0x34000000
#define PRIM_FMT_GOURAUD_TEX_QUAD 0x3C000000

/* Primitive format IDs (short forms from Ghidra data) */
#define PRIM_ID_TRI 0x2000
#define PRIM_ID_QUAD 0x2800
#define PRIM_ID_GOURAUD_TRI 0x3000
#define PRIM_ID_GOURAUD_QUAD 0x3800
#define PRIM_ID_TEXTURED_TRI 0x2004
#define PRIM_ID_TEXTURED_QUAD 0x2804
#define PRIM_ID_GOURAUD_TEX_TRI 0x3004
#define PRIM_ID_GOURAUD_TEX_QUAD 0x3804
#define PRIM_ID_SPRITE 0x2001
#define PRIM_ID_SPRITE_SHADE 0x3001
#define PRIM_ID_SPRITE_TEX 0x2400
#define PRIM_ID_SPRITE_SHADE_TEX 0x3400
#define PRIM_ID_LINE 0x4000
#define PRIM_ID_LINE_GOURAUD 0x5000
#define PRIM_ID_TILE 0x6000
#define PRIM_ID_TILE_16 0x7000
#define PRIM_ID_TILE_8 0x6001
#define PRIM_ID_SPRITE_16 0x2501
#define PRIM_ID_SPRITE_8 0x2401
#define PRIM_ID_SPRITE_SHADE_16 0x3501
#define PRIM_ID_SPRITE_SHADE_8 0x3401
#define PRIM_ID_SPRITE_TEX_16 0x2400
#define PRIM_ID_SPRITE_TEX_8 0x2400
#define PRIM_ID_TRI_TEX 0x2101
#define PRIM_ID_QUAD_TEX 0x2901
#define PRIM_ID_GOURAUD_TRI_TEX 0x3101
#define PRIM_ID_GOURAUD_QUAD_TEX 0x3901
#define PRIM_ID_SPRITE_TEX_SHORT 0x2400
#define PRIM_ID_SPRITE_TEX_TALL 0x2501
#define PRIM_ID_TILE_1 0x6000
#define PRIM_ID_TILE_8_ALT 0x6001
#define PRIM_ID_TILE_16_ALT 0x7000
#define PRIM_ID_SPRITE_1 0x2001
#define PRIM_ID_SPRITE_1_SHADE 0x3001

/* ------------------------------------------------------------------ */
/* Category 2: Hardware Register Addresses                             */
/* PS1 hardware registers for SPU, CD-ROM, I/O, GTE, etc.             */
/* ------------------------------------------------------------------ */

#define REG_SPU_MAIN_VOL_L 0x1f801d80
#define REG_SPU_MAIN_VOL_R 0x1f801d82
#define REG_SPU_VOICE_BASE 0x1f801c00
#define REG_SPU_VOICE_PITCH(n) (REG_SPU_VOICE_BASE + (n) * 0x10 + 0x02)
#define REG_SPU_VOICE_VOL_L(n) (REG_SPU_VOICE_BASE + (n) * 0x10 + 0x00)
#define REG_SPU_VOICE_VOL_R(n) (REG_SPU_VOICE_BASE + (n) * 0x10 + 0x04)
#define REG_SPU_VOICE_ADSR1(n) (REG_SPU_VOICE_BASE + (n) * 0x10 + 0x08)
#define REG_SPU_VOICE_ADSR2(n) (REG_SPU_VOICE_BASE + (n) * 0x10 + 0x0A)
#define REG_SPU_VOICE_WAVE(n)  (REG_SPU_VOICE_BASE + (n) * 0x10 + 0x0C)
#define REG_SPU_CONTROL  0x1f801d00
#define REG_SPU_STATUS   0x1f801d04
#define REG_SPU_REVERB  0x1f801d08
#define REG_SPU_REVERB_OUT 0x1f801d0c
#define REG_IO_PORT      0x1f801800
#define REG_IO_PORT_03   0x1f801803
#define REG_CDROM_STATUS 0x1f801070
#define REG_CDROM_MODE   0x1f801074
#define REG_INTERRUPT_CTRL 0x1f801000
#define REG_GTE_RAND     0x1f800000
#define REG_GTE_DMA      0x1f800000

/* KSEG addresses */
#define KSEG0_BASE       0x80000000
#define KSEG1_BASE       0xA0000000
#define UNCACHED_MIRROR  0x20000000

/* ------------------------------------------------------------------ */
/* Category 3: SPU/Memory Constants                                   */
/* ------------------------------------------------------------------ */

#define SPU_PITCH_MAX    0x3fff
#define SPU_PITCH_MIN    0x3f01
#define SPU_NOTE_HOLD    0x4000
#define SPU_NOISE_FREQ   0x80ff
#define SPU_FRAME_SIZE   0x400
#define CD_SECTOR_SIZE   0x800
#define MEM_PAGE_SIZE    0x1000
#define MEM_64K          0x10000
#define OT_DEPTH_DEFAULT 0x4b0
#define SCREEN_W         320
#define SCREEN_H         240
#define SCREEN_W_HALF    160
#define SCREEN_H_HALF    120

/* ------------------------------------------------------------------ */
/* Category 4: State/Mode Constants                                   */
/* ------------------------------------------------------------------ */

/* CD/Fade Mode Constants */
#define CD_MODE_STOPPED    0x13
#define CD_MODE_PLAYING    0x14
#define CD_MODE_STARTING   0x23
#define FADE_MODE_NONE     0x0
#define FADE_MODE_IN       0x1
#define FADE_MODE_OUT      0x2

/* Entity State Constants */
#define ENTITY_STATE_ACTIVE    0x14
#define ENTITY_STATE_TRIGGER   0x12
#define ENTITY_STATE_SPECIAL   0x1a
#define ENTITY_STATE_BUSY      0x10
#define ENTITY_STATE_DAMPER    0x28
#define ENTITY_STATE_COOLDOWN  0x1e
#define ENTITY_STATE_LINKED    0x6e
#define ENTITY_STATE_ATTACHED  0x20
#define ENTITY_STATE_DETACHED  0x40
#define ENTITY_STATE_FLAG_MASK 0x1c
#define ENTITY_STATE_TYPE_MASK 3

/* Entity Subtype Constants */
#define ENTITY_SUBTYPE_0   0
#define ENTITY_SUBTYPE_1   1
#define ENTITY_SUBTYPE_2   2
#define ENTITY_SUBTYPE_3   3
#define ENTITY_SUBTYPE_4   4
#define ENTITY_SUBTYPE_5   5

/* Sound Type Constants */
#define SOUND_TYPE_BGM      0x27
#define SOUND_TYPE_SFX      0x14
#define SOUND_TYPE_AMBIENT  0x1e
#define SOUND_TYPE_VOICE    0x28
#define SOUND_TYPE_EFFECT   0x40
#define SOUND_VOLUME_DEFAULT 0x7f
#define SOUND_VOLUME_HALF   0x40
#define SOUND_CHANNEL_MAX   0x10
#define SOUND_BANK_MAX      0x11
#define SOUND_VAB_MAGIC     0x564142

/* Voice Flag Bits */
#define VOICE_FLAG_ACTIVE   0x01
#define VOICE_FLAG_STOPPED  0x20
#define VOICE_FLAG_PLAYING  0x10
#define VOICE_FLAG_REVERB   0x08
#define VOICE_FLAG_LOOP     0x04
#define VOICE_FLAG_PITCH    0x100
#define VOICE_FLAG_KEYON    0x200
#define VOICE_FLAG_KEYOFF   0x204

/* Pad Constants */
#define PAD_BIOS_ARGS       0xB0
#define PAD_INIT_DIGITAL    0x14
#define PAD_INIT_ANALOG     0x15
#define PAD_INIT_MOUSE      0x16

/* Trap Exception Constants */
#define TRAP_DIVZERO        0x1c00
#define TRAP_OVERFLOW       0x1800

/* Chunk/Tile Constants */
#define TILE_FORMAT_MASK    0x13
#define CHUNK_ENTRY_STRIDE  0x50
#define TILE_GRID_COUNT     0x13
#define GRID_DIM_MASK       0x1f

/* Location Grid Size Constants */
#define LOCATION_CHUNK_SIZE    0x50
#define STAGE_CHUNK_SIZE    LOCATION_CHUNK_SIZE // deprecated alias: use LOCATION_CHUNK_SIZE
#define LOCATION_GRID_SIZE     0x1e8
#define STAGE_GRID_SIZE     LOCATION_GRID_SIZE // deprecated alias
#define SPAWN_ENTRY_STRIDE  0x1c
#define ENTITY_LIST_STRIDE  0x14
#define COLOR_TABLE_STRIDE  0x0c

/* Angle Conversion */
#define DEGREES_360         0x1000
#define DEGREES_90          0x400
#define DEGREES_180         0x800
#define DEGREES_270         0xC00
#define ANGLE_HALF_CIRCLE   0x800
#define ANGLE_FULL_CIRCLE   0x1000
#define ANGLE_NORMALIZE     0xfff
#define ANGLE_DIV_360       0x168

/* Screen Space Constants */
#define CLIP_NEAR           0xfffe
#define CLIP_FAR            0x1000
#define CLIP_LEFT           0x200
#define CLIP_RIGHT          0x200
#define CLIP_TOP            0x201
#define CLIP_BOTTOM         0x201

/* Fixed-Point Scaling */
#define FIXED_12            0x1000
#define FIXED_8             0x100
#define FIXED_16            0x10000
#define FP_SHIFT_12         12
#define FP_SHIFT_8          8

/* Particle Constants */
#define PARTICLE_VEL_SCALE  0x800
#define PARTICLE_POS_SCALE  0x1000
#define PARTICLE_GRAVITY    0x5fb4
#define PARTICLE_FALL_MAX   0x1fff
#define PARTICLE_LIFT       0x7800

/* World Geometry */
#define WORLD_TO_SCREEN_X   0x5000
#define WORLD_TO_SCREEN_Y   0xA000
#define WORLD_SCALE_X       0x400
#define WORLD_SCALE_Y       0x800

/* Memory Allocation Sizes */
#define SIZEOF_ENTITY_TYPE0 0x54
#define SIZEOF_ENTITY_TYPE1 0x88
#define SIZEOF_ENTITY_TYPE2 0x98
#define SIZEOF_SUBOBJECT    0x58
#define SIZEOF_SUBOBJECT2   0xa8
#define SIZEOF_RENDER_CTX   0x50
#define SIZEOF_RENDER_SUB   0x38
#define SIZEOF_LOCATION_CHUNK  0x50
#define SIZEOF_STAGE_CHUNK  SIZEOF_LOCATION_CHUNK // deprecated alias
#define SIZEOF_LOCATION_GRID   0x1e8
#define SIZEOF_STAGE_GRID   SIZEOF_LOCATION_GRID // deprecated alias
#define SIZEOF_GSOBJ        0xBC
#define SIZEOF_OBJGENERIC   0xA0
#define SIZEOF_OBJTYPE1     0x6c
#define SIZEOF_OBJTYPE2     0xb8
#define SIZEOF_OBJTYPE3     0xac
#define SIZEOF_OBJTYPE4     0xa8
#define SIZEOF_OBJTYPE5     0x30
#define SIZEOF_OBJTYPE6     0x1c
#define SIZEOF_OBJTYPE7     0x54
#define SIZEOF_OBJTYPE8     0x20
#define SIZEOF_OBJTYPE9     0x84
#define SIZEOF_OBJTYPE10    0x30
#define SIZEOF_OBJTYPE11    0x3c
#define SIZEOF_OBJTYPE12    0x2c
#define SIZEOF_OBJTYPE13    0x68
#define SIZEOF_OBJTYPE14    0x38
#define SIZEOF_OBJTYPE15    0x3c
#define SIZEOF_OBJTYPE16    0x44
#define SIZEOF_OBJTYPE17    0x38
#define SIZEOF_OBJTYPE18    0x2c
#define SIZEOF_OBJTYPE19    0x6c
#define SIZEOF_PLAYER_SEQ   0x24
#define SIZEOF_CHUNK_DATA   0xec
#define SIZEOF_RENDERNODE   0x14

/* Default Values */
#define DEFAULT_SCALE       0x100
#define HALF_WORLD_SCALE    0x4000
#define OT_PRIORITY_DEFAULT 0x4b0
#define SPRITE_DIM_THRESHOLD 0x100

/* Pad masks */
#define PAD_MASK_LEFT  0x8000
#define PAD_MASK_RIGHT 0x2000
#define PAD_MASK_UP    0x1000
#define PAD_MASK_DOWN  0x4000
#define PAD_MASK_CROSS 0x0040
#define PAD_MASK_CIRCLE 0x0020
#define PAD_MASK_SQUARE 0x0080
#define PAD_MASK_TRIANGLE 0x0010
#define PAD_MASK_R1 0x0200
#define PAD_MASK_R2 0x0400
#define PAD_MASK_L1 0x0004
#define PAD_MASK_L2 0x0008
#define PAD_MASK_START 0x0800
#define PAD_MASK_SELECT 0x0001

/* Texture Page Decode */
#define TPAGE_X_SHIFT      5
#define TPAGE_Y_SHIFT      3
#define TPAGE_X_MASK       0x1f
#define TPAGE_Y_MASK       0x07

#endif /* MAGIC_NUMBERS_H */
