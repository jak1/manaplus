#!/bin/bash

export AR=${CROSS}-ar
export AS=${CROSS}-as
export LD=${CROSS}-ld
export NM=${CROSS}-nm
export RC=${CROSS}-windres
export WINDRES=${CROSS}-windres
export ADD2LINE=${CROSS}-addr2line
export STRIP=${CROSS}-strip
export CC=${CROSS}-gcc
export CXX=${CROSS}-g++
export CXXCPP=${CROSS}-cpp
export CPP=${CROSS}-cpp
export RANLIB=${CROSS}-ranlib

export CXXFLAGS="-Wall -g1 -O5 -ffast-math -std=gnu++1z -funswitch-loops -funsafe-loop-optimizations -pipe"
