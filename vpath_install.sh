#!/usr/bin/env bash

NOAPP=$HOME/src/noapp
cd $NOAPP
mkdir build 2> /dev/null
autoreconf

cd build
../configure --prefix=$NOAPP/build --enable-debug=yes
make
make install
