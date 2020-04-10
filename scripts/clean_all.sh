#!/bin/bash

#UNIX Targets
UNIX_TARGET1=./bin/invaders #Running from script dir
UNIX_TARGET2=../bin/invaders #Running from home dir

#Windows Targets
WIN_TARGET1=./bin/invaders.exe #Running from script dir
WIN_TARGET2=../bin/invaders.exe #Running from home dir

if [ -f "$UNIX_TARGET1" ]; then #nix Target 1 check and clean
    echo "$UNIX_TARGET1 exists. Cleaning...."
    rm ./bin/invaders
    rm ./src/*.o
else 
    echo "(Target 1): No Linux release found to clean..."
fi

if [ -f "$UNIX_TARGET2" ]; then #nix Target 2 check and clean
    echo "$UNIX_TARGET2 exists. Cleaning...."
    rm ../bin/invaders
    rm ../src/*.o
else 
    echo "(Target 2): No Linux release found to clean..."
fi

if [ -f "$WIN_TARGET1" ]; then #Win Target 1 check and clean
    echo "$WIN_TARGET1 exists. Cleaning...."
    rm ./bin/invaders.exe
    rm ./src/*.o
else 
    echo "(Target 1): No Windows release found to clean..."
fi

if [ -f "$WIN_TARGET2" ]; then #Win Target 2 check and clean
    echo "$WIN_TARGET2 exists. Cleaning...."
    rm ../bin/invaders.exe
    rm ../src/*.o
else 
    echo "(Target 2): No Linux release found to clean..."
fi

echo "Done... Script completed without error"