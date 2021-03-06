#!/bin/bash

export CC=gcc-7
export CXX=g++-7

if [ "$NEWCC" != "" ]; then
    export CC="$NEWCC"
fi
if [ "$NEWCXX" != "" ]; then
    export CXX="$NEWCXX"
fi

export LOGFILE=gcc7.log

source ./tools/ci/scripts/init.sh

export CXXFLAGS="$CXXFLAGS -pedantic -ggdb3 -O2 -pipe -Wstrict-aliasing=2 \
-Wstrict-overflow=1 -Wformat=1 -D_FORTIFY_SOURCE=2 \
-std=gnu++1z -Wformat=1 \
-Wno-attributes -fno-omit-frame-pointer"

source ./tools/ci/flags/gcc7.sh

export CXXFLAGS="$CXXFLAGS $POST_CXXFLAGS"

do_init
run_configure $*
run_make

source ./tools/ci/scripts/exit.sh

exit 0
