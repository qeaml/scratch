#!/bin/sh

set -xe

mkdir -p target
gcc -O3 -nostdlib -c -o target/scratch.o scratch.c
ar rcs target/libscratch.a target/scratch.o
