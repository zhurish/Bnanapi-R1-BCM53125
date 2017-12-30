# BPI-Kernel4.0 

Supports BananaPi -R1;
mv kernel-config .config

ARCH=arm CROSS_COMPILE=/opt/toolchain/arm-cortexa9/4.9.3/bin/arm-cortexa9-linux-gnueabihf- INSTALL_MOD_PATH=./output/modules LOADADDR=0X42000000 make uImage modules modules_install -j4



Supports BananaPi -M1 / M1Plus / M2
=========

Getting Started
-------------------------
1. Run Build Command:
   `./build.sh`

2. Choose a board doing:
    `1. BananaPi-M1`
    `2. BananaPi-M1-Plus`
    `3. BananaPi-M2`


