#!/bin/bash

LIB_DIR=`pwd`

# get persistent skip list
if [ ! -d "PersistentSkipList" ]; then
    git clone git://github.com/spratt/PersistentSkipList.git
fi

# if error, bail
if [ ! -d "PersistentSkipList" ]; then
    echo "An error occured."
    exit
fi

cd PersistentSkipList
git pull origin master
make clean
make
cd $LIB_DIR

# get smart pointers
cd PersistentSkipList/lib
if [ ! -d "SmartPointer" ]; then
    git clone git://github.com/spratt/SmartPointer.git
fi

# if error, bail
if [ ! -d "SmartPointer" ]; then
    echo "An error occured."
    exit
fi

cd SmartPointer
git pull origin master
make clean
make
cd $LIB_DIR

# get cpplog
if [ ! -d "CppLog" ]; then
    git clone git://github.com/spratt/CppLog.git
fi

# if error, bail
if [ ! -d "CppLog" ]; then
    echo "An error occured."
    exit
fi

cd CppLog
git pull origin master
make clean
make