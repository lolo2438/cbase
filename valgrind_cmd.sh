#!/bin/bash

gcc -lm -o vg.out containers/list*.{c,h} constant.h
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./vg.out
