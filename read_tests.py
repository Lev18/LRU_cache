#!/bin/python3.8

import os
from os import listdir
from os.path import isfile, join

def compare_files(file1, file2):
    with open (file1) as f1, open(file2) as f2:
        for line1, line2 in zip(f1, f2):
            if line1 != line2:
                return False
        return True

mypath = "./Outputs"
fil = [ f for f in listdir(mypath) if isfile(join(mypath, f))]
prefix = "./Outputs/"

files = [prefix + item for item in fil]


examples = [f for f in files if f[-2:] == "ex"]
outputs = [f for f in files if f[-3:] == "out"]

def is_file_empty(file):
    return os.stat(file).st_size == 0

id = 1
for ex, out in zip(examples, outputs):
    if (compare_files(ex, out) and not is_file_empty(out)):
        print(f"[Test {id}] passed!!!!")
    id += 1

