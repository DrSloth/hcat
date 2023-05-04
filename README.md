# hcat
A very minimal implementation of cat.
This program doesn't depent on libc or any other system header, it uses inline assembly to directly
call linux system calls.

## Building
To build just run the `BUILD.sh` script, this will generate one version compiled with gcc and one
compiled with clang.

## Size
For comparison hcat-gcc is 1.2kb the default linux cat is 43kb

## Speed
hcat doesn't have many features but it is quite fast, it is not really optimised for speed though.

