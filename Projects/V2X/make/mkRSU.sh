#!/bin/bash

echo
echo Complie JamesSoft RSU...
echo

if [ $# -eq 0 ]
then
    make -f $V2X/make/rsu.mk
else
    make -f $V2X/make/rsu.mk $1
fi

echo
echo Complie JamesSoft RSU end.
echo



