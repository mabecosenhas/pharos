#! /bin/bash
# Exporting all necessary variables for compiling and executing Pharos in
# all platforms

export BINUTILS_VERSION=2.32
export GCC_VERSION=8.2.0
export GDB_VERSION=8.2.1

# paths where the GCC will be installed to
export GCC_ARM_M4_PATH=~/cross-compiler/arm/gcc-${GCC_VERSION}/m4
export GCC_ARM_V5_PATH=~/cross-compiler/arm/gcc-${GCC_VERSION}/v5
export GCC_ARM_R5_BIG_PATH=~/cross-compiler/arm/gcc-${GCC_VERSION}/r5-big
export GCC_ARM_AARCH64_PATH=~/cross-compiler/arm/gcc-${GCC_VERSION}/aarch64
export GCC_RISCV64_PATH=~/cross-compiler/riscv/gcc-${GCC_VERSION}/riscv64

export PHAROS_ARM_V5_CC=${GCC_ARM_V5_PATH}/bin/arm-none-eabi-gcc
export PHAROS_ARM_V5_ARCHIVER=${GCC_ARM_V5_PATH}/bin/arm-none-eabi-ar
export PHAROS_ARM_V5_RANLIB=${GCC_ARM_V5_PATH}/bin/arm-none-eabi-ranlib
export PHAROS_ARM_V5_OBJDUMP=${GCC_ARM_V5_PATH}/bin/arm-none-eabi-objdump
export PHAROS_ARM_V5_OBJCOPY=${GCC_ARM_V5_PATH}/bin/arm-none-eabi-objcopy
export PHAROS_ARM_V5_GDB=${GCC_ARM_V5_PATH}/bin/arm-none-eabi-gdb

export PHAROS_ARM_R5_BIG_CC=${GCC_ARM_R5_BIG_PATH}/bin/arm-none-eabi-gcc
export PHAROS_ARM_R5_BIG_ARCHIVER=${GCC_ARM_R5_BIG_PATH}/bin/arm-none-eabi-ar
export PHAROS_ARM_R5_BIG_RANLIB=${GCC_ARM_R5_BIG_PATH}/bin/arm-none-eabi-ranlib
export PHAROS_ARM_R5_BIG_OBJDUMP=${GCC_ARM_R5_BIG_PATH}/bin/arm-none-eabi-objdump
export PHAROS_ARM_R5_BIG_OBJCOPY=${GCC_ARM_R5_BIG_PATH}/bin/arm-none-eabi-objcopy
export PHAROS_ARM_R5_BIG_GDB=${GCC_ARM_R5_BIG_PATH}/bin/arm-none-eabi-gdb

export PHAROS_ARM_M4_CC=${GCC_ARM_M4_PATH}/bin/arm-none-eabi-gcc
export PHAROS_ARM_M4_ARCHIVER=${GCC_ARM_M4_PATH}/bin/arm-none-eabi-ar
export PHAROS_ARM_M4_RANLIB=${GCC_ARM_M4_PATH}/bin/arm-none-eabi-ranlib
export PHAROS_ARM_M4_OBJDUMP=${GCC_ARM_M4_PATH}/bin/arm-none-eabi-objdump
export PHAROS_ARM_M4_OBJCOPY=${GCC_ARM_M4_PATH}/bin/arm-none-eabi-objcopy
export PHAROS_ARM_M4_GDB=${GCC_ARM_M4_PATH}/bin/arm-none-eabi-gdb

export PHAROS_ARM_AARCH64_CC=${GCC_ARM_AARCH64_PATH}/bin/aarch64-none-elf-gcc
export PHAROS_ARM_AARCH64_ARCHIVER=${GCC_ARM_AARCH64_PATH}/bin/aarch64-none-elf-ar
export PHAROS_ARM_AARCH64_RANLIB=${GCC_ARM_AARCH64_PATH}/bin/aarch64-none-elf-ranlib
export PHAROS_ARM_AARCH64_OBJDUMP=${GCC_ARM_AARCH64_PATH}/bin/aarch64-none-elf-objdump
export PHAROS_ARM_AARCH64_OBJCOPY=${GCC_ARM_AARCH64_PATH}/bin/aarch64-none-elf-objcopy
export PHAROS_ARM_AARCH64_GDB=${GCC_ARM_AARCH64_PATH}/bin/aarch64-none-elf-gdb

export PHAROS_RISCV64_CC=${GCC_RISCV64_PATH}/bin/riscv64-unknown-elf-gcc
export PHAROS_RISCV64_ARCHIVER=${GCC_RISCV64_PATH}/bin/riscv64-unknown-elf-ar
export PHAROS_RISCV64_RANLIB=${GCC_RISCV64_PATH}/bin/riscv64-unknown-elf-ranlib
export PHAROS_RISCV64_OBJDUMP=${GCC_RISCV64_PATH}/bin/riscv64-unknown-elf-objdump
export PHAROS_RISCV64_OBJCOPY=${GCC_RISCV64_PATH}/bin/riscv64-unknown-elf-objcopy
export PHAROS_RISCV64_GDB=${GCC_RISCV64_PATH}/bin/riscv64-unknown-elf-gdb

# export PATH to QEMU
export PATH=~/cross-compiler/qemu/4.1.0/bin/:$PATH
