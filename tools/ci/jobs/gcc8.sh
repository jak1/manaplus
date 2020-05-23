#!/bin/bash
dir=`pwd`

export CC=gcc-8
export CXX=g++-8

if [ "$NEWCC" != "" ]; then
    export CC="$NEWCC"
fi
if [ "$NEWCXX" != "" ]; then
    export CXX="$NEWCXX"
fi

export LOGFILE=gcc8.log

source ./tools/ci/scripts/init.sh

export CXXFLAGS="$CXXFLAGS -pedantic -ggdb3 -O2 -pipe -Wstrict-aliasing=2 \
-Wstrict-overflow=1 -Wformat=1 -D_FORTIFY_SOURCE=2 \
-std=gnu++2a -Wformat=1 \
-Wno-attributes -fno-omit-frame-pointer"

source ./tools/ci/flags/gcc8.sh

export CXXFLAGS="$CXXFLAGS $POST_CXXFLAGS"

do_init
run_configure --enable-werror \
--enable-commandlinepassword \
--datadir=$dir/run/share/games \
--bindir=$dir/run/bin \
--mandir=$dir/run/share/man \
--enable-portable=yes $*

run_make
run_make_install


source ./tools/ci/scripts/exit.sh

exit 0
