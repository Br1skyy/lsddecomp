# Reverse Engineering Guide

This guide provides methodologies and guidelines for reverse-engineering LSD: Dream Emulator and contributing to the C reconstruction project.

## Tools and Setup

### Required Tools

- **Ghidra** - Primary decompiler for analysis
- **mipsel-none-elf-gcc** - Cross-compiler for building
- **PS1 emulator** - For runtime testing of the build
- **Hex editor** - For binary analysis

### Runtime Testing

Run the build in a PS1 emulator and inspect the debug markers (D0-D9) written
to RAM at `0x800BAFF0` by System.c to see how far boot progresses.

- The last exception's CPU state can be read from the ISR save block at
  `0x80059DC0` (offsets: t9@96, gp@100, ra@104, plus the general registers
  saved by the exception handler in cdrom_handler.c).
- See docs/Boot_Debug.md for the current boot blocker and how it was
  investigated.

## Methodology

### 1. Initial Analysis

Start with high-level understanding. The deep function you are staring at is probably not a good place to start.

1. **Boot Process Analysis**
   - Trace execution from entry point
   - Identify subsystem initialization
   - Map out memory layout
   - Document interrupt handlers

2. **Subsystem Identification**
   - Graphics system (GPU calls)
   - Audio system (SPU usage)
   - Input system (controller handling)
   - Game logic (state machines)

3. **Data Structure Analysis**
   - Identify common structure patterns
   - Map out vtable layouts
   - Document data relationships
   - Name key structures

### 2. Function Analysis

For each function of interest:

1. **Context Gathering**
   - Who calls this function?
   - What does it call?
   - What data does it access?
   - What side effects does it have?

2. **Pattern Recognition**
   - Look for common PS1 patterns
   - Identify library calls (PsyQ)
   - Spot custom game logic
   - Note inline assembly

3. **Documentation**
   - Document purpose and behavior
   - Name parameters meaningfully
   - Describe return values
   - Note any quirks or bugs

### 3. Structure Recovery

Convert raw offsets to named structures:

1. **Field Identification**
   - Group related offsets
   - Identify common patterns
   - Match with known structures
   - Document layout

2. **Type Reconstruction**
   - Determine field types
   - Identify arrays vs pointers
   - Note alignment requirements
   - Document endianness

3. **Validation**
   - Test structure assumptions
   - Verify with runtime data
   - Check for inconsistencies
   - Document exceptions

## Ghidra Workflow

### Import Process

1. **Load Binary**
   - File -> Import File -> SLPS_015.56
   - Set language to MIPS
   - Configure memory map

2. **Apply Symbols**
   - Import symbol table from config/symbols.slps01556.lsdde.txt
   - Apply structure definitions
   - Mark known functions

3. **Analysis**
   - Auto-analyze functions
   - Review disassembly
   - Create structure definitions
   - Document findings

### Decompilation

1. **Function Focus**
   - Start with high-level functions
   - Work down to low-level utilities
   - Document as you go
   - Cross-reference with other functions

2. **Structure Application**
   - Apply structure types to pointers
   - Verify field access patterns
   - Refine structure definitions
   - Share common structures

3. **Code Export**
   - Export C code from Ghidra
   - Clean up decompiler output
   - Apply project coding style
   - Add documentation

## Debugging Methodology

### Runtime Analysis

1. **Breakpoint Placement**
   - Set breakpoints at function entry
   - Trace execution flow
   - Monitor register changes
   - Log memory accesses

2. **Memory Inspection**
   - Examine data structures at runtime
   - Track state changes
   - Verify structure layouts
   - Identify corruption

3. **Logging**
   - Add debug prints to code
   - Log function calls
   - Track state transitions
   - Record timing information

## Code Reconstruction Process

### From Assembly to C

1. **Direct Translation**
   - Convert assembly instructions to C
   - Preserve logic flow
   - Maintain register semantics
   - Document any assumptions

2. **Refinement**
   - Replace manual stack operations with locals
   - Convert pointer arithmetic to structure access
   - Replace raw constants with named values
   - Add meaningful comments

3. **Integration**
   - Integrate with existing code
   - Update function signatures
   - Add header declarations
   - Test the changes

### Quality Standards

- **No UB** - Eliminate undefined behavior
- **Type Safety** - Use proper types, avoid casts where possible
- **Readability** - Follow project code style
- **Documentation** - Document non-obvious logic
- **Patience** - You will spend 40 minutes on what turned out to be a typo. This is normal.

## Common Patterns

### PS1 Library Calls

Recognize PsyQ library functions:

- **GS functions** - Graphics (GsSortSprite, GsSetRefView2)
- **GPU functions** - Low-level GPU (PutDrawEnv, SetDrawEnv)
- **SPU functions** - Audio (SpuSetVoiceVolume, SpuSetKey)
- **CD functions** - CD-ROM (CdRead, CdSync)

### Game-Specific Patterns

- **Vtable Dispatch** - (*(code*)(vtable + offset))(obj, ...)
- **State Machines** - Switch statements with state variables
- **Callback Chains** - Function pointer arrays
- **Memory Pools** - Custom allocators

## Documentation Standards

### Function Documentation

Each function should have:

- **Purpose** - What the function does
- **Parameters** - Input parameters and their meaning
- **Return Value** - What the function returns
- **Side Effects** - Any modifications to global state
- **Notes** - Any quirks, bugs, or special behavior

### Structure Documentation

Each structure should have:

- **Purpose** - What the structure represents
- **Field Descriptions** - Each field's purpose
- **Layout** - Memory layout and alignment
- **Usage** - How the structure is used

## Verification

### Testing

1. **Unit Testing**
   - Test individual functions
   - Verify structure layouts
   - Check boundary conditions
   - Validate assumptions

2. **Integration Testing**
   - Test function interactions
   - Verify system behavior
   - Check for regressions
   - Performance testing

3. **Comparison Testing**
   - Compare with original behavior
   - Verify output matches
   - Check for missing functionality
   - Document differences

## Contribution Guidelines

### Submitting Changes

1. **Code Quality**
   - Follow project code style
   - Document changes thoroughly
   - Test before submitting
   - Update relevant documentation

2. **Structure Changes**
   - Update structure definitions
   - Verify field offsets
   - Document layout changes
   - Update all usages

3. **New Functions**
   - Document purpose and behavior
   - Add to appropriate headers
   - Update symbol tables
   - Test thoroughly
