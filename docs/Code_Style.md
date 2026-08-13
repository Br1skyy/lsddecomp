# Code Style

This document defines the coding standards and conventions for the LSD: Dream Emulator C reconstruction project. Follow them, or the next reviewer will gently but firmly judge you.

## Naming Conventions

### Functions

- **Public Functions**: PascalCase with system prefix
  ```c
  DreamSys__AdvanceDay
  Entity_MoveToward
  SpuSetVoiceVolume
  ```

- **Internal Functions**: camelCase
  ```c
  calculatePosition
  updateTimer
  processEntity
  ```

- **VTable Functions**: PascalCase with object prefix
  ```c
  Entity__Update
  DreamSys__Render
  Spu__Initialize
  ```

### Variables

- **Local Variables**: camelCase
  ```c
  int currentDay;
  float cameraAngle;
  void* entityPtr;
  ```

- **Global Variables**: PascalCase with module prefix
  ```c
  DreamCurrentStage
  GlobalTimer
  SystemHeapBase
  ```

- **Parameters**: camelCase
  ```c
  void FunctionName(int inputCount, void* dataPtr)
  ```

### Structures

- **Structure Names**: PascalCase
  ```c
  typedef struct {
      int nPosX;
      int nPosY;
      void* pData;
  } EntityObj;
  ```

- **Structure Fields**: camelCase with type prefix
  ```c
  int nPosX;        // n prefix for int
  void* pData;      // p prefix for pointer
  uint dwFlags;     // dw prefix for uint
  ```

### Constants

- **Macros**: UPPER_SNAKE_CASE
  ```c
  #define MAX_ENTITIES 256
  #define STAGE_COUNT 14
  #define DREAM_BUFFER_SIZE 0x1000
  ```

- **Enum Values**: UPPER_SNAKE_CASE
  ```c
  typedef enum {
      STATE_IDLE,
      STATE_ACTIVE,
      STATE_COMPLETE
  } EntityState;
  ```

## File Organization

### File Naming

- **C Files**: PascalCase matching main content
  ```
  DreamSys.c
  GameLoop.c
  Rendering_Entity.c
  ```

- **Header Files**: PascalCase matching corresponding C file
  ```
  DreamSys.h
  GameLoop.h
  ```
  (Note: Some shared headers use descriptive names)

- **Assembly Files**: lowercase with underscores
  ```
  main.s
  cdrom_handler.s
  dream_sys.s
  ```

### File Structure

Each file should follow this order:

1. **License/Comments** - File purpose and credits
2. **Includes** - System headers, then project headers
3. **Macros/Constants** - #define statements
4. **Type Definitions** - typedefs, enums, structs
5. **Global Variables** - File-level globals
6. **Function Declarations** - Forward declarations
7. **Function Implementations** - Actual code

## Code Formatting

### Indentation

- **Spaces**: 4 spaces per indentation level
- **No Tabs**: Use spaces consistently
- **Brace Style**: K&R style
  ```c
  if (condition) {
      doSomething();
  } else {
      doSomethingElse();
  }
  ```

### Line Length

- **Maximum**: 120 characters per line
- **Preference**: Break long lines logically (your editor thanks you)
  ```c
  // Bad
  int result = SomeVeryLongFunctionName(parameter1, parameter2, parameter3, parameter4, parameter5);

  // Good
  int result = SomeVeryLongFunctionName(
      parameter1, parameter2, 
      parameter3, parameter4, parameter5
  );
  ```

### Spacing

- **Operators**: Space around operators
  ```c
  int result = a + b * c;
  ```

- **Keywords**: Space after keywords
  ```c
  if (condition) {
      while (running) {
          doWork();
      }
  }
  ```

- **No Space**: Function calls and array access
  ```c
  functionCall(param);
  array[index];
  ```

## Comments

### File Headers

Each file should begin with:
```c
/*
 * Filename.c - Brief description
 *
 * Detailed description of what this file contains
 * and its purpose in the overall system.
 */
```

### Function Comments

Document non-obvious functions:
```c
/*
 * ProcessEntityUpdate - Update entity state for current frame
 *
 * Parameters:
 *   entity - Pointer to entity to update
 *   deltaTime - Time elapsed since last frame (in milliseconds)
 *
 * Returns:
 *   0 on success, negative on error
 *
 * Side Effects:
 *   Updates entity position, animation state, and collision data
 */
int ProcessEntityUpdate(EntityObj* entity, int deltaTime);
```

### Inline Comments

Use inline comments for:
- Complex logic explanations
- Workarounds for bugs
- References to original behavior
- TODO/FIXME markers

```c
// Original Ghidra: iVar1 = func_800206e0();
// Purpose: Initialize GPU display list
int gpuInit = InitializeGpuDisplay();
```

## Type Usage

### Integer Types

Use specific types from psx_types.h:
```c
s8  int8_var;   // Signed 8-bit
u8  uint8_var;  // Unsigned 8-bit
s16 int16_var;  // Signed 16-bit
u16 uint16_var; // Unsigned 16-bit
s32 int32_var;  // Signed 32-bit
u32 uint32_var; // Unsigned 32-bit
```

### Pointers

Use descriptive pointer names:
```c
void* dataPtr;        // Generic pointer
EntityObj* entity;    // Typed pointer
int* counter;         // Pointer to primitive
```

### Avoid

- **Undefined types**: Replace undefined4, DAT_800 with proper types
- **Excessive casting**: Minimize pointer casts when possible
- **Magic numbers**: Use named constants

## Structure Layout

### Alignment

Maintain original alignment from Ghidra:
```c
typedef struct {
    void*   pVtbl;      // 0x00 - pointer alignment
    int     nPosX;      // 0x04 - 4-byte aligned
    int     nPosY;      // 0x08 - 4-byte aligned
    u16     wState;     // 0x0C - 2-byte aligned
    u8      byteFlags;  // 0x0E - 1-byte
    u8      padding;    // 0x0F - padding for alignment
} ExampleStruct;
```

### Field Ordering

- Preserve original field order
- Document padding explicitly
- Note any size changes from original

## VTable Conventions

### VTable Layout

Vtables follow standard PS1 C++ layout:
```c
typedef struct {
    void* destructor;       // +0x00
    void* method1;          // +0x04
    void* method2;          // +0x08
    void* method3;          // +0x0C
    // ... more methods
} Vtable_Example;
```

### VTable Calls

Use consistent dispatch pattern:
```c
// Original Ghidra pattern
(*(code*)(vtable + offset))(obj, param1, param2);

// Preferred named version
typedef void (*MethodPtr)(void* obj, int param1, int param2);
MethodPtr method = (MethodPtr)(vtable + offset);
method(obj, param1, param2);
```

## Error Handling

### Return Values

- **0**: Success
- **Negative**: Error (return -1, -2, etc.)
- **Positive**: Success with value

### Assertions

Use assertions for debugging:
```c
#include <assert.h>

void FunctionName(void* ptr) {
    assert(ptr != NULL);
    // ... rest of function
}
```

## Platform-Specific Code

### PS1 Hardware Access

Use explicit hardware register definitions:
```c
#define SPU_MAIN_VOL_L  (*(volatile u16 *)0x1f801d80)
#define GPU_STATUS     (*(volatile u32 *)0x1f801814)
```

### Inline Assembly

Use sparingly and document thoroughly:
```c
/*
 * Initialize SPU hardware
 * Sets SPUCNT to enable SPU operation
 */
__asm__(
    ".set noreorder\n"
    "lui $v0, 0x1F80\n"
    "li $v1, 0x8000\n"
    "sh $v1, 0x1C00($v0)\n"
    ".set reorder\n"
);
```

## Goals

The ultimate goal is to eliminate:
- undefined4, undefined8 types
- DAT_800, DAT_801 variable names
- param_1, param_2 parameter names
- Excessive pointer casts
- Magic numbers without names

Replace with:
- Proper typed variables
- Meaningful names
- Structure field access
- Named constants
- Clear documentation
