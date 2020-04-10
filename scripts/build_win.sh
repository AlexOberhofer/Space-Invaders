#!/bin/bash

MAKEFILE_PATH1=../makefile.windows
MAKEFILE_PATH2=./makefile.windows

if [ -f "$MAKEFILE_PATH1" ]; then #Makefile path 1
    echo "Path 1"
    cd ..
    make -f $MAKEFILE_PATH2
fi

if [ -f "$MAKEFILE_PATH2" ]; then #Makefile path 2
    echo "Path 2"
    make -f $MAKEFILE_PATH2
fi

echo "Done... Script completed without error."