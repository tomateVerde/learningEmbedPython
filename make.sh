#!/usr/bin/bash

g++ -std=c++17 -ggdb3 -O0 -pedantic-errors -Wall \
    -fpie $(python3.8-config --cflags --embed) -o 'example.out' \
    'main.cpp' $(python3.8-config --embed --ldflags)