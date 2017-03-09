#!/bin/sh

cat <<DOC

gdb-multiarch
file /repo/arm/setup/hello
set architecture armv5te
set sysroot /usr/arm-linux-gnueabihf/
target remote localhost:1234
break main
continue

DOC

qemu-arm -L /usr/arm-linux-gnueabihf -g 1234 ./hello
