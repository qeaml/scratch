#!/bin/sh

set -xe

mkdir -p target
gcc -O3 -nostdlib -shared -o target/libscratch.so scratch.c
