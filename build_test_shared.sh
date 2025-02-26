#!/bin/sh

set -xe

./build_shared.sh
gcc -Ltarget -lscratch -o target/test test.c
LD_LIBRARY_PATH=target ./target/test
