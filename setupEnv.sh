#!/bin/bash
BASE="$(pwd)"
BOARD_LINK="$BASE/nuttx/boards/arm/stm32h7/vanguard-fc"
ROMFS_IMG="$BOARD_LINK/src/romfs.img"
ROMFS_HDR="$BOARD_LINK/src/romfs.h"

echo "[SETUPENV] Initializing environment..."

# 1. Cleanup and Re-link
rm -rf "$BOARD_LINK" "$BASE/apps/FlightComputer"
mkdir -p "$BASE/apps"
ln -s "$BASE/boards/vanguard-fc" "$BOARD_LINK"
ln -s "$BASE/FlightComputer" "$BASE/apps/FlightComputer"

# 2. Generate and Sanitize ROMFS
if [ -d "$BOARD_LINK/init" ]; then
    genromfs -f "$ROMFS_IMG" -d "$BOARD_LINK/init" -V "NuttXBoot" || { echo "ERROR: genromfs failed"; exit 1; }
    
    # Generate header and rename variables to 'romfs_img' and 'romfs_img_len'
    xxd -i "$ROMFS_IMG" > "$ROMFS_HDR"
    sed -i 's/_home_.*_romfs_img/romfs_img/g' "$ROMFS_HDR"
    sed -i 's/_home_.*_romfs_img_len/romfs_img_len/g' "$ROMFS_HDR"
    
    echo "[SETUPENV] ROMFS generated with simple variable names."
else
    echo "[SETUPENV] WARNING: Init directory not found."
fi

echo "[SETUPENV] Setup complete."
