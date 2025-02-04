# Scratch

A simple scratch-space allocator, used by the [nwge] engine.

## Install

1. You can simply drop [`scratch.c`](scratch.c) and [`scratch.h`](scratch.h)
   into your source code ét voila!

2. You can build Scratch as a static library and use it that way:
    ```sh
    git clone https://github.com/qeaml/scratch
    cd scratch
    ./build_shared.sh
    # use target/libscratch.a to your heart's content
    ```
    (see [`build_test.sh`](build_test.sh))

3. You can build Scratch as a shared library and use it that way:

    ```sh
    git clone https://github.com/qeaml/scratch
    cd scratch
    ./build_static.sh
    # use target/libscratch.so to your heart's content
    ```

## Usage

Using a scratch-space is simple. You provide it with a buffer to use and
it'll let you allocate and reallocate memory within it. Scratch-space is
effectively just a fancier name for a memory arena. You can see the gist of how
to use a scratch space in the [`test.c`](test.c) file. It's also worth it to
give [`scratch.h`](scratch.h) a read.

Note that Scratch is not designed to be MT-safe. Use the global scratch-space
with caution.

[nwge]: https://qeaml.github.io/nwge-docs/INTRO
