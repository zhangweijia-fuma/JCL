#!/bin/bash

echo
echo Complie JamesSoft RSUManager...
echo

if [ $# -eq 0 ]
then
    make -f $V2X/make/RSUManager.mk
else
    make -f $V2X/make/RSUManager.mk $1
fi

echo
echo Complie JamesSoft RSUManager end.
echo



