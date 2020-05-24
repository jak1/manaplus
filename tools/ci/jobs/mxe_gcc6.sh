#!/bin/bash
dir=`pwd`
cd build/
./winmake
cd $dir
cd build/
./packwin
cp tmp/src/*.exe /binarys/
