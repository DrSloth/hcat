#!/bin/sh

set -xe
gcc -Os hcat.c -nostdlib -nostartfiles -ffreestanding -static -fno-pie -s -o hcat-gcc -Wl,--nmagic 
/usr/bin/clang-9 hcat.c -o hcat-clang -nostartfiles -nostdlib -ffreestanding -static -Oz -fno-pie -Wl,--nmagic