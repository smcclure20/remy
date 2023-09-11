#!/bin/bash

import os
import pathlib
import subprocess

def sortfunc(entry):
    return int(entry.split(".")[-1])

root_path = "/home/sarah_mcclure/remy/results"
root_dir = pathlib.Path(root_path)

num = 0
files = list(root_dir.rglob("cfg0808-fct.*"))
files = [x.name for x in files]
print("file list made")
files.sort(key=sortfunc)
print("file list sorted")
for filename in files:
    #print(filename)
    newnum = subprocess.check_output(["python3", "inspect-whiskers.py", root_path + "/" + filename], stderr=subprocess.STDOUT, universal_newlines=True)
    #print(newnum)
    if (int(newnum) > num):
        num = int(newnum)
        print("{} {}".format(filename, num))

print("done")
