#!/bin/bash
# Cross-platform disc extraction script for Unix-like systems
# Equivalent to scripts/extract.ps1

set -e

ROOT="$(cd "$(dirname "$0")/.." && pwd)"

# Find dumpsxiso (prefer the bundled exe, fall back to a native binary on PATH)
DX=""
if [ -f "$ROOT/tools/dumpsxiso.exe" ]; then
    DX="$ROOT/tools/dumpsxiso.exe"
elif [ -f "$ROOT/tools/dumpsxiso" ]; then
    DX="$ROOT/tools/dumpsxiso"
else
    DX=$(command -v dumpsxiso || true)
fi

if [ -z "$DX" ]; then
    echo "Error: dumpsxiso not found in tools/ or PATH"
    exit 1
fi

# Run the Windows exe under Wine on Unix; native builds run directly
if [[ "$DX" == *.exe ]]; then
    if command -v wine >/dev/null 2>&1; then
        RUNNER="wine"
    else
        echo "Error: $DX is a Windows binary but wine is not installed"
        exit 1
    fi
else
    RUNNER=""
fi

# Check if image argument provided
IMAGE="$1"

# No image given, look in disk/
if [ -z "$IMAGE" ]; then
    FOUND=()
    
    # Look for .cue files
    while IFS= read -r -d '' file; do
        FOUND+=("$file")
    done < <(find "$ROOT/disk" -name "*.cue" -type f -print0 2>/dev/null)
    
    # Look for .bin files
    while IFS= read -r -d '' file; do
        FOUND+=("$file")
    done < <(find "$ROOT/disk" -name "*.bin" -type f -print0 2>/dev/null)
    
if [ ${#FOUND[@]} -eq 0 ]; then
    echo "Error: no .cue/.bin in disk/"
    echo "Pass one: pwsh scripts/extract.ps1 -Image path/to/LSD.cue"
    echo "Or: ./scripts/extract.sh path/to/LSD.cue"
    exit 1
fi
    
    # Sort by size (largest first) and pick the first
    IMAGE=$(printf '%s\n' "${FOUND[@]}" | xargs -I {} stat -f%z {} 2>/dev/null | paste - <(printf '%s\n' "${FOUND[@]}") | sort -rn | head -1 | cut -f2-)
    if [ -z "$IMAGE" ]; then
        # Fallback for systems without stat -f
        IMAGE="${FOUND[0]}"
    fi
fi

# Resolve to absolute path
IMAGE=$(cd "$(dirname "$IMAGE")" && pwd)/$(basename "$IMAGE")
DEST="$ROOT/disk/extract"

mkdir -p "$DEST"

echo "extracting $IMAGE -> $DEST"
$RUNNER "$DX" -x "$DEST" "$IMAGE"

# Make sure we got what the build needs
MISSING=()
for p in "extract.xml" "SLPS_015.56" "SYSTEM.CNF" "CDI"; do
    if [ ! -f "$DEST/$p" ]; then
        MISSING+=("$p")
    fi
done

if [ ${#MISSING[@]} -gt 0 ]; then
    echo "Error: extraction missing: ${MISSING[*]}"
    echo "Wrong image?"
    exit 1
fi

echo "done. extract.xml, SLPS_015.56, CDI/ in disk/extract/"
echo "next: ./scripts/build.sh  (or: pwsh scripts/build_ps1.ps1)"
