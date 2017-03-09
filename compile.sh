#!/bin/bash

CXX="arm-linux-gnueabihf-g++"
FLAGS="-mtune=arm926ej-s -march=armv5te -marm  -x c++ -ggdb"

${CXX} ${FLAGS} -o hello hello.cpp

exit 0

${CXX} ${FLAGS} -o hello - <<SRC
#include <stdio.h>
int main()
{
    printf("Hello ARM!\n");
    return 0;
}
SRC
