#!/bin/sh

CXX="arm-linux-gnueabihf-g++"
FLAGS="-std=c++1z -mtune=arm926ej-s -march=armv5te -marm -Igoogletest-arm/include -lpthread -Lgoogletest-arm/lib/ -lgmock -lgmock_main -lgtest -lgtest_main"

${CXX} ${FLAGS} -o ut ut.cpp
