cmake_minimum_required (VERSION 2.8)

project(murmurhash)

add_executable (murmurhash main.c murmurhash.c)
add_executable (test test.c murmurhash.c)
