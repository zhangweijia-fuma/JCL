#!/bin/bash


Usage()
{
    echo
    echo "Usage: $0 [c|close] | [s|start] | [r|restart]"
    echo
}

# 获取环境变量值
getEnvValue()
{
    envStr=`env|grep $1'=' `
    array=(${envStr//=/ }) 

    envValue=${array[1]}
}


stopV2X()
{
    echo
    echo "Closing V2X RSU..."
    echo 

	processID=`ps aux | grep RSU | grep -v "grep" | awk '{print $2}' `
    
    # 进程是否存在
    if [ ! -z $processID ]
    then
        kill -9 $processID
    fi

    echo
    echo "V2X RSU end."
    echo
}


startV2X()
{
    echo
    echo "Starting V2X RSU..."
    echo 
    getEnvValue "JDS_App"

    getEnvValue $envValue
    rootPath=$envValue

    cd $rootPath/bin
    stopV2x

    RSU&

    echo
    echo "Starting V2X RSU successfully..."
    echo 

}

# 判断运行时是否带有2个参数且第一个参数为 JamesSoft
if [ $# -ne 1 ]
then
    Usage
    exit
fi

case $1 in
    c|close)
        stopV2X;;
    s|start)
        startV2X;;

    r|restart)
        startV2X;;
esac


