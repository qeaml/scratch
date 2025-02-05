#!/bin/sh

set -xe

./build_static.sh
gcc -o target/test test.c target/libscratch.a
