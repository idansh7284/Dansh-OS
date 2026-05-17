#!/bin/bash

# =================================================================
# OS Build & Run Script
# =================================================================

# 1. Configuration and Toolchain
AS="nasm"
CC="i686-elf-gcc"
LD="i686-elf-ld"
OBJCOPY="i686-elf-objcopy"
QEMU="qemu-system-i386"

# Paths
OUT_DIR="output"
STAGE2_OUT="$OUT_DIR/stage2"
MBR_OUT="$OUT_DIR/mbr"
INCLUDE_DIR="stage2/include"
HDD_IMG="HDD.img"

# Visuals
GREEN='\e[32m'
BLUE='\e[34m'
RED='\e[31m'
NC='\e[0m' 

# clear screen
clear
# 2. Setup Environment
echo "${BLUE}[*] Cleaning previous builds...${NC}"
rm -rf $OUT_DIR
mkdir -p $MBR_OUT $STAGE2_OUT

export PATH="$HOME/i686-elf-tools/bin:$PATH"

# -----------------------------------------------------------------
echo "${GREEN}=================================================================="
echo " [1/4] Assembling: Bootloader & Entry Point"
echo "==================================================================${NC}"

# Assemble MBBR
$AS -f bin boot/bootloader.asm -o $MBR_OUT/bootloader.bin || { echo -e "${RED}Failed to assemble bootloader${NC}"; exit 1; }

# Assemble StAge2 Entry
$AS -f elf32 stage2/entry.asm -o $STAGE2_OUT/stage2_asm.o || { echo -e "${RED}Failed to assemble entry.asm${NC}"; exit 1; }
# Assemble input output port 
$AS -f elf32 stage2/drivers/io.asm -o $STAGE2_OUT/io.o || { echo -e "${RED} Failed to assemble io.asm${NC}"; exit 1; }

$AS -f bin BOOTFS.asm -o $STAGE2_OUT/BOOTFS.bin

$AS -f bin ex.asm -o $STAGE2_OUT/ex.bin
# -----------------------------------------------------------------
echo "${GREEN}=================================================================="
echo " [2/4] Compiling: Kernel C Files"
echo "==================================================================${NC}"

CFLAGS="-m32 -ffreestanding -mgeneral-regs-only -mno-80387 -I $INCLUDE_DIR -O2 -Wall -Wextra"

# ompile main.c
$CC $CFLAGS -c stage2/c_code/main.c -o $STAGE2_OUT/main.o || exit 1

# Compille all drivers
cd $STAGE2_OUT
$CC $CFLAGS -I ../../stage2/include/ -c ../../stage2/drivers/*.c || exit 1
cd ../..
# -----------------------------------------------------------------
echo "${GREEN}=================================================================="
echo " [3/4] Linking: Generating HDD Image"
echo "==================================================================${NC}"

# Linking objects
$LD -m elf_i386 -T stage2/linker.ld $STAGE2_OUT/*.o -o $STAGE2_OUT/stage2.elf || exit 1

# Convert ELF to raw Binary
$OBJCOPY -O binary $STAGE2_OUT/stage2.elf $OUT_DIR/stage2.bin

# Create HDD Image (12288 sectors * 512 bytes = 6MB)
dd if=/dev/zero of=$HDD_IMG bs=512 count=12288 status=none

# Write Bootloader to Sector 0
dd if=$MBR_OUT/bootloader.bin of=$HDD_IMG bs=512 count=1 conv=notrunc status=none

dd if=$STAGE2_OUT/BOOTFS.bin of=$HDD_IMG bs=512 count=1 seek=1 conv=notrunc status=none

# Write Stage2 to Sector 3
dd if=$OUT_DIR/stage2.bin of=$HDD_IMG bs=512 seek=3 conv=notrunc status=none

dd if=$STAGE2_OUT/ex.bin of=$HDD_IMG bs=512 seek=133 conv=notrunc status=none

echo "${BLUE}[+] HDD Image '$HDD_IMG' created successfully.${NC}"

# -----------------------------------------------------------------
echo "${GREEN}=================================================================="
echo " [4/4] Execution: Starting QEMU"
echo "==================================================================${NC}"

$QEMU -hda $HDD_IMG