#!/bin/bash

echo
echo Complie JamesSoft EdgeServer...
echo

if [ $# -eq 0 ]
then
    make -f $V2X/make/edgeServer.mk
else
    make -f $V2X/make/edgeServer.mk $1
fi

echo
echo Complie JamesSoft EdgeServer end.
echo



