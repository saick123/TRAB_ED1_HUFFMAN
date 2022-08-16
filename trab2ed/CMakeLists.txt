cmake_minimum_required(VERSION 3.22)
project(trab2ed C)

set(CMAKE_C_STANDARD 11)

add_executable(trab2ed main.c headers.h tree.h heap.h main.h tree.c heap.c)
