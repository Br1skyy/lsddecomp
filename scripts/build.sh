#!/bin/bash
# Cross-platform build script for Unix-like systems (Linux/macOS)
# Equivalent to build_ps1.ps1

set -e

ROOT="$(cd "$(dirname "$0")/.." && pwd)"
BUILD="$ROOT/build_ps1"
ASM_DIR="$ROOT/asm/lsdde"
SRC_DIR="$ROOT/src/lsdde"
INC_DIR="$ROOT/include"
PSYQ_INC="$ROOT/psyq/include"
PSYQ_LIB="$ROOT/psyq/lib"

# Check if psyq/ exists, fetch if needed
if [ ! -f "$PSYQ_INC/libgpu.h" ]; then
    echo "psyq/ not found - fetching the SDK..."
    pwsh "$ROOT/scripts/fetch_psyq.ps1"
fi

# Find toolchain
TOOL="mipsel-none-elf-gcc"
TOOL_PATH=$(command -v "$TOOL" || true)

if [ -z "$TOOL_PATH" ]; then
    # Check common locations
    CANDIDATES=(
        "/usr/local/mipsel-none-elf/bin/mipsel-none-elf-gcc"
        "/opt/mipsel-none-elf/bin/mipsel-none-elf-gcc"
        "/usr/bin/mipsel-none-elf-gcc"
        "$HOME/.local/bin/mipsel-none-elf-gcc"
    )
    
    for candidate in "${CANDIDATES[@]}"; do
        if [ -f "$candidate" ]; then
            TOOL_PATH="$candidate"
            break
        fi
    done
fi

if [ -z "$TOOL_PATH" ]; then
    echo "Error: mipsel-none-elf-gcc not found in PATH or standard locations"
    echo "Install the toolchain or add it to PATH"
    exit 1
fi

TOOL_DIR=$(dirname "$TOOL_PATH")

# Check for extracted disc
if [ ! -f "$ROOT/disk/extract/extract.xml" ]; then
    echo "Error: no disk/extract/extract.xml"
    echo "Run: pwsh scripts/extract.ps1 -Image <your-lsd.cue-or-bin>"
    exit 1
fi

CC="$TOOL_DIR/mipsel-none-elf-gcc"
LD="$TOOL_DIR/mipsel-none-elf-ld"
OBJCOPY="$TOOL_DIR/mipsel-none-elf-objcopy"
NM="$TOOL_DIR/mipsel-none-elf-nm"

CFLAGS=(
    -std=gnu99 -mips1 -msoft-float -mno-gpopt -G0 -fno-builtin
    -Os -g -fno-merge-constants -fpermissive -mlong-calls
    -Wno-implicit-function-declaration -Wno-int-conversion
    -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast
    -Wno-incompatible-pointer-types
    -I"$INC_DIR" -I"$PSYQ_INC" -DPSX
)

ASFLAGS=(
    -mips1 -msoft-float -mno-gpopt -G0
    -I"$INC_DIR" -I"$ASM_DIR"
)

LDFLAGS=(
    -nostdlib -L"$PSYQ_LIB" -G0
    --allow-multiple-definition
    --unresolved-symbols=ignore-all
    --relax
)

echo "=== build ==="
mkdir -p "$BUILD"

# Compile C
echo "compiling C..."
C_OBJECTS=()
for c_file in "$SRC_DIR"/*.c; do
    obj="$BUILD/$(basename "$c_file" .c).o"
    C_OBJECTS+=("$obj")
    "$CC" "${CFLAGS[@]}" -c "$c_file" -o "$obj"
done

# Compile globals.c separately
GLOBALS_OBJ="$BUILD/globals.o"
"$CC" "${CFLAGS[@]}" -c "$ROOT/src/globals.c" -o "$GLOBALS_OBJ"
C_OBJECTS+=("$GLOBALS_OBJ")

# Assemble asm
echo "assembling asm..."
ASM_OBJECTS=()
while IFS= read -r -d '' asm_file; do
    obj="$BUILD/$(basename "$asm_file" .s).o"
    ASM_OBJECTS+=("$obj")
    "$CC" "${ASFLAGS[@]}" -c "$asm_file" -o "$obj"
done < <(find "$ASM_DIR" -name "*.s" -print0)

# Link
echo "linking..."
EXE="$BUILD/lsddecomp.elf"
"$LD" "${LDFLAGS[@]}" -T "$ROOT/lsdde.ld" -o "$EXE" "${C_OBJECTS[@]}" "${ASM_OBJECTS[@]}"

# Produce PS-EXE
echo "producing PS-EXE..."
PS_EXE_DIR="$BUILD/ps-exe"
mkdir -p "$PS_EXE_DIR"
PS_EXE="$PS_EXE_DIR/SLPS_015.56"
"$OBJCOPY" -O binary "$EXE" "$PS_EXE"

# ISO
echo "building ISO..."
ISO_DIR="$BUILD/iso"
mkdir -p "$ISO_DIR"
cp "$ROOT/disk/extract/SLPS_015.56" "$ISO_DIR/"
cp "$ROOT/disk/extract/SYSTEM.CNF" "$ISO_DIR/"
cp -r "$ROOT/disk/extract/CDI" "$ISO_DIR/"

# Find mkpsxiso (prefer the bundled exe, fall back to a native binary on PATH)
MKP=""
if [ -f "$ROOT/tools/mkpsxiso.exe" ]; then
    MKP="$ROOT/tools/mkpsxiso.exe"
elif [ -f "$ROOT/tools/mkpsxiso" ]; then
    MKP="$ROOT/tools/mkpsxiso"
else
    MKP=$(command -v mkpsxiso || true)
fi

if [ -z "$MKP" ]; then
    echo "Error: mkpsxiso not found in tools/ or PATH"
    exit 1
fi

# Run the Windows exe under Wine on Unix; native builds run directly
if [[ "$MKP" == *.exe ]]; then
    if command -v wine >/dev/null 2>&1; then
        RUNNER="wine"
    else
        echo "Error: $MKP is a Windows binary but wine is not installed"
        exit 1
    fi
else
    RUNNER=""
fi

$RUNNER "$MKP" "$ROOT/config/disk.xml" "$ISO_DIR" "$BUILD/lsddecomp"

echo "=== build complete ==="
echo "output: build_ps1/lsddecomp.cue + build_ps1/lsddecomp.bin"
echo "PS-EXE: build_ps1/ps-exe/SLPS_015.56"
