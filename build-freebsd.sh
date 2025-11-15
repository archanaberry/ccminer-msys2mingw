#!/usr/bin/env bash

# Simple script to create the Makefile and build

# export PATH="$PATH:/usr/local/cuda/bin/"

make distclean || echo clean

rm -f Makefile.in
rm -f config.status
./autogen.sh || echo done

./configure.sh

gmake -j $(sysctl hw.ncpu|awk '{print $2}')
