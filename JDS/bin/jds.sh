#!/bin/sh

startJDS()
{
    echo
    echo
    echo Starting JamesSoft JDS System......
    echo

    DATE=$(date "+%Y-%m-%d %H:%M:%S")

    echo $DATE
    echo

    #service postgresql restart
    service redis-server restart

    service nginx stop
    ps aux | grep nginx | awk ' { print $2 }' | xargs kill -9
    service nginx start
    cd /usr/local/nginx/sbin
    ./nginx -s stop
    ./nginx &

    ps aux | grep python | awk ' { print $2 }' | xargs kill -9

    cd $SRVDIR/src/service
    (python3 main.py mio &) >nohup.out 2>&1

    sleep 1
    ps aux | grep python

    #service jicofo restart
    #service jitsi-videobridge restart

    echo
    echo
    DATE=$(date "+%Y-%m-%d %H:%M:%S")

    echo $DATE
    echo
    echo JamesSoft JDS System started.
    echo
    echo
}

stopJDS()
{
    echo 
    echo JamesSoft JDS System Ending ...
    echo

    echo
    echo Stop python processes...
    echo
    ps aux | grep python3 | awk '{print $2}' | xargs kill -9
    echo "all python processes ended."
    echo
    echo
    #sleep 0.5
    #echo 
    #echo Stop PostgreSQL service...
    #/etc/init.d/postgresql stop
    #echo
    #echo "PostgreSQL service ended."
    #echo
    echo JamesSoft JDS System End.
    echo
    echo
}

showStatus()
{
    echo
    ps -fe|grep py | grep -v "grep"
    echo

}

Restart()
{
   # ~/tm

    stopJDS
    startJDS
    cd $SRVDIR/src/service
    python3 main.py
}

Usage()
{
    echo
    echo Usage: $0 "start|stop|status"
    echo
}

if [ $# -eq 0 ]
then
    Usage
    exit 0
fi

if [ $# -eq 1 ]
then
    case $1 in
        start|Start)
            startJDS
            ;;
        stop|Stop|c)
            stopJDS
            ;;
        s|status)
            showStatus
            ;;
        r|restart)
            Restart
	    ;;
        *)
            Usage
            ;;
    esac
else
    Usage
fi

