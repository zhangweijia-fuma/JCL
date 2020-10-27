#!/bin/bash

# 脚本由应用程序启动
# 参数1：固定为 JamesSoft，以防止手动启动脚本
Usage()
{
    echo
    echo "Usage: This script should be run by application and can not be run from common line."
    echo
}


# 定期检查系统环境，以防止因存储不够造成系统崩溃

echo
echo "System Schedule Task..."
echo

# 判断运行时是否带有6个参数且第一个参数为 JamesSoft
if [ $# -ne 6 ]
then
    Usage
    exit
fi

if [ $1 != "JamesSoft" ]
then
    Usage
    exit
fi

exit

deviceCode=$2
strServerAddr=$3
strFTPUserName=$4
strFTPPassword=$5
remotePath=$6

# 检查运行日志文件大小

fileSize=`ls -l $V2X/JDSlog.txt | awk '{print $5}'`

if [ $fileSize -lt 200000 ]
then
    echo "File Size is OK"
    exit
fi

echo "File size is exceed limit."

# 将运行日志压缩后上传到服务端，然后将压缩文件删除
currentDate="`date +%Y%m%d-%H%M%S`"
localFile=$deviceCode-$currentDate.log
echo $localFile

mv $V2X/JDSlog.txt $V2X/log/$localFile
tar -zcvf $V2X/log/$localFile.tar.gz $V2X/log/$localFile
localFile=$localFile.tar.gz

# 上传到服务端
ftp -n<<!
open $strServerAddr
user $strFTPUserName $strFTPPassword
binary
cd $remotePath
lcd $V2X/log
prompt
put $localFile
close
bye
!

# 删除运行日志文件
rm $V2X/log/*.gz *.log
