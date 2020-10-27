#!/bin/bash
# 参数1：固定为 JamesSoft，以防止手动启动脚本

echo
echo "System Updateing..."
echo

envValue=""
filename=""

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

# 获取字符串中的文件名，要求文件名中不带.
# 例如， aaa.bbb.ccc，获取的文件名为aaa
getFileName()
{
    filePath=$1
    array=(${filePath//./ }) 
    filename=${array[0]}
}

# 通知运行RSU
stopV2X()
{
    echo "Stop V2X RSU..."

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

updateFile=$rootPath/update/$2

cd $rootPath/update
tar -zxvf $updateFile 

# 默认升级文件加压后，生成一个目录，目录名和文件名一致
getFileName $2
cd $filename

# 在该目录中固定有一个 install.sh文件
./install.sh

cd $rootPath/update

# 删除升级包目录
rm -rf $filename
# rm $2

# 停止当前应用并重新启动
cd $rootPath/bin

stopV2X

# 暂停1秒钟
sleep 1

# 应用重启由watchdog.sh 完成

# echo "Restart V2X RSU..."
# exec RSU&


