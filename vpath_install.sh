#!/usr/bin/env bash

NOAPP=$HOME/src/noapp
mkdir build 2> /dev/null

cd $NOAPP
autoreconf
cd build
../configure --prefix=$NOAPP/build --enable-debug=yes
make
make install
