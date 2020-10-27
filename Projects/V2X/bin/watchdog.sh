#!/bin/bash

# 由crontab每分钟执行一次

echo
echo "System Watch Dog..."
echo


processID=`ps aux | grep RSU | grep -v "grep" | awk '{print $2}' `

# 进程是否存在
if [ -z $processID ]
then
    echo "V2X RSU is stoped now. Restart RSU..."

    # 在cron中不能使用原有shell中的环境变量，需要重新定义
    TZ='Asia/Shanghai'; export TZ
    export V2X=/JamesSoft/Projects/V2X
    export JDS_App=V2X
    export DebugLevel=3
    /JamesSoft/Projects/V2X/bin/RSU&
fi
