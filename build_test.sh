#!/bin/sh

set -xe

./build_static.sh
gcc -O3 -o target/test test.c target/libscratch.a
