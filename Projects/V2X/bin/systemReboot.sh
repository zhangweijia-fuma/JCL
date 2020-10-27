#!/bin/bash


echo
echo "System Rebooting..."
echo


# 参数1：固定为 JamesSoft，以防止手动启动脚本

Usage()
{
    echo
    echo "Usage: This script should be run by application and can not be run from common line."
    echo
}


# 获取环境变量值
getEnvValue()
{
    envStr=`env|grep $1'=' `
    array=(${envStr//=/ }) 

    envValue=${array[1]}
}


# 停止运行RSU
stopV2X()
{
    processID=`ps aux | grep RSU | grep -v "grep" | awk '{print $2}' `
    
    # 进程是否存在
    if [ ! -z $processID ]
    then
        kill -9 $processID
    fi
}

# 判断运行时是否带有2个参数且第一个参数为 JamesSoft
if [ $# -ne 2 ]
then
    Usage
    exit
fi

if [ $1 != "JamesSoft" ]
then
    Usage
    exit
fi


# 默认升级文件在应用的update目录中，为一个 .tar.gz 文件
getEnvValue "JDS_App"

getEnvValue $envValue
rootPath=$envValue

rebootMode=$2

# 0: 重启应用， 1：重启设备
if [ $rebootMode == '0' ]
then
    echo "Reload V2X Application..."

    # 停止当前应用并重新启动
    cd $rootPath/bin

    stopV2X

    # 暂停1秒钟
    sleep 1

    # 应用的重启由watchdog.sh 完成，该脚本每分钟执行一次

elif [ $rebootMode == '1' ]
then
    echo "Reboot device..."
    reboot
else
    echo "Parameter $2 is out of range [0, 1]."
fi
