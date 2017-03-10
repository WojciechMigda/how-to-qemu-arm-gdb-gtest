#!/bin/bash

CXX="arm-linux-gnueabihf-g++"
FLAGS="-mtune=arm926ej-s -march=armv5te -marm -ggdb"

${CXX} ${FLAGS} -o hello hello.cpp
