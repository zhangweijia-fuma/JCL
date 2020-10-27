#!/bin/bash

execCommand()
{
    cmd=$1
    param=$2
    echo $cmd
    echo $param
    result=$($cmd $param)
    echo result=$result
    if [[ -n $result ]]
    then
       echo "not success"
    else
       echo "success!" 
    fi
}


# execCommand pip3 "install psycopg2-binarya"
execCommand apt-get "update"