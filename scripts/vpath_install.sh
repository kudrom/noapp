#!/usr/bin/env bash

if [ $# == 0 ]
then
    NOAPP=$HOME/src/noapp
else if [ $# == 1 ]
then
    NOAPP=$1
else
    echo "Usage vpath_install [srcdir]"
fi
fi

echo "Destination dir: $NOAPP/build"

cd $NOAPP
mkdir build 2> /dev/null
cp -r include build/
autoreconf

cd build
../configure --prefix=$NOAPP/build --enable-debug=yes
make
make install
