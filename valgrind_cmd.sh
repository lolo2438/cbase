#!/bin/bash

gcc -lm -o vg.out containers/hashmap*.{c,h} constant.h
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=valgrind-out.txt ./vg
