#!/bin/bash
set xe

gcc -Wall -Wextra LRU_cache.c list.c hash_table.c -o LRU_cache -ggdb && ./LRU_cache

./read_tests.py
