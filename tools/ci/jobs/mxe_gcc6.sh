#!/bin/bash
dir=`pwd`
cd build/
./winmake
cd $dir
cd build/
./packwin
cd $dir
cp packaging/windows/*.exe binarys/
