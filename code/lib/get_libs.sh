#!/bin/bash

# get persistent skip list
if [ -d "PersistentSkipList" ]; then
    cd PersistentSkipList
    git pull origin master
    make clean
    make
    cd ..
else
    git clone git://github.com/spratt/PersistentSkipList.git
fi

# if error, bail
if [ ! -d "PersistentSkipList" ]; then
    echo "An error occured."
    exit
fi

# get smart pointers
cd PersistentSkipList/lib
if [ -d "SmartPointer" ]; then
    cd SmartPointer
    git pull origin master
    make clean
    make
else
    git clone git://github.com/spratt/SmartPointer.git
fi