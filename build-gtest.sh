#!/bin/bash

mkdir .build-gtest
pushd .build-gtest

FLAGS="-mtune=arm926ej-s -march=armv5te -marm"

CFLAGS=${FLAGS} CXXFLAGS=${FLAGS} cmake \
-DCMAKE_BUILD_TYPE=Release \
-DBUILD_GTEST=ON -DBUILD_GMOCK=ON \
-DCMAKE_INSTALL_PREFIX:PATH=../googletest-arm \
-DCMAKE_C_COMPILER=arm-linux-gnueabihf-gcc -DCMAKE_CXX_COMPILER=arm-linux-gnueabihf-g++ \
../googletest

make -j 4 && make install

popd
