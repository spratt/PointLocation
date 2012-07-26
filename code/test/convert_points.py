#!/usr/bin/env python2.6

import sys

FILENAME="points.txt"
if len(sys.argv) > 1:
    FILENAME=sys.argv[1]

with open(FILENAME,'r') as file:
    for line in file:
        print line[line.index(' ')+1:],
