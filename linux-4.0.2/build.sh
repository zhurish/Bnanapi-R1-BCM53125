#!/bin/bash
export ARCH=arm
#export CROSS_COMPILE=arm-linux-gnueabihf-
export KBUILD_OUTPUT=./output/bpi
mkdir -p $KBUILD_OUTPUT
make bpi_defconfig
LOADADDR=0x40008000 make -j4 uImage dtbs
make -j4 modules
make -j4 INSTALL_MOD_PATH=out modules_install

