#!/bin/bash

installSoft()
{
    echo
    echo
    echo install $systemName JDS System...
    echo

    DATE=$(date "+%Y-%m-%d %H:%M:%S")

    echo $DATE
    echo

    echo update apt-get source
    echo
    echo
    apt-get update

    apt-get -y install nginx
    apt-get -y install redis-server
    apt-get -y install postgresql
    apt-get -y install samba

    apt-get -y install python3
    apt-get -y install python3-dev 
    apt-get -y install python3-pip 
    apt-get -y install python3-pil
    apt-get -y install python3-msgpack 

    apt-get -y install libjpeg8-dev
    apt-get -y install zlib1g-dev 
    apt-get -y install libfreetype6-dev 
    apt-get -y install libpq-dev 
    apt-get -y install libmysqlclient-dev
    apt-get -y install libpcap-dev

    apt-get -y install vsftpd
    
    apt-get -y install ntpdate
    apt-get -y install ntp
    apt-get -y install lrzsz

    apt-get -y install cifs-utils

    pip3 install psycopg2
    pip3 install psycopg2-binary
    
    pip3 install pycrypto
    pip3 install tornado 
    pip3 install ujson 
    pip3 install redis

    pip3 install apscheduler
    pip3 install reportlab
    pip3 install wechatpy

    pip3 install pydicom                                                # 医疗影像文件 dicom文件 处理
    
    pip3 install xlrd
    pip3 install xlwt
    pip3 install pypcap
    pip3 install dpkt
    pip3 install openpyxl

    # Ali云SDK核心库及云产品SDK
    pip3 install aliyun-python-sdk-core-v3
    pip3 install aliyun-python-sdk-ecs

    # mysql
    pip3 install ConfigParser
    pip3 install mysql

    cd $WORKDIR/software
    wget http://dev.mysql.com/get/Downloads/Connector-Python/mysql-connector-python-2.1.1.tar.gz
    tar xvf mysql-connector-python-2.1.1.tar.gz
    cd mysql-connector-python-2.1.1/
    python3 setup.py install
    cd ..
    rm -rf mysql-connector-python-2.1.1/
    rm mysql-connector-python-2.1.1.tar.gz

    # install upload file service
    cd $WORKDIR/software/fileupload
    tar -zxvf fileupload.tar.gz
    cd $WORKDIR/software/fileupload/nginx-1.4.7
    ./configure --prefix=/usr/local/nginx --with-openssl=/usr/include/openssl --add-module=$WORKDIR/software/fileupload/nginx-upload-module-2.2 --add-module=$WORKDIR/software/fileupload/nginx-upload-progress-module-master
    make
    make install

    cd /usr/local/nginx/html
    mkdir tmp1
    cd tmp1
    mkdir 0 1 2 3 4 5 6 7 8 9 temp

    cd /usr/local/nginx/html
    chmod -R 777 tmp1

    # 文件上传依赖库
    apt-get -y install libpcre3
    apt-get -y install libpcre3-dev
    apt-get -y install zlib
    apt-get -y install openssl
    apt-get -y install libssl-dev

    # install complete
}


# copy configuration files to dest dictionary
setConfiguration()
{
    cd $SRVDIR/install

    mv ~/.bashrc ~/.bashrc.bak
    cp bashrc.txt ~/.bashrc
    source ~/.bashrc

    # copy config files
    
    mv /etc/nginx/sites-available/default /etc/nginx/sites-available/default.bak
    cp nginx.conf /etc/nginx/sites-available/default
    service nginx restart

    mv /etc/redis/redis.conf /etc/redis/redis.conf.bak
    cp redis.conf /etc/redis/redis.conf
    service redis-server restart

    mv /etc/samba/smb.conf /etc/samba/smb.conf.bak
    cp smb.conf /etc/samba/smb.conf
    service smbd restart

    mv /etc/vsftpd.conf /etc/vsftpd.conf.bak
    cp vsftpd.conf /etc
    service vsftpd restart

    mv /etc/ntp.conf /etc/ntp.conf.bak
    cp ntp.conf /etc
    service ntp restart

    # 默认OS为19.04
    postgreVer='11'
    
    os=`lsb_release -a|sed -n 3p|awk '//{print $2}'`
    OSVer=${os:0:2}

    # Ubuntu 系统，验证了14.04、18.04、19.04、20.04等4个系统
    case $OSVer in
        14) postgreVer='9.3';;
        18) postgreVer='10';;
        19) postgreVer='11';;
        20) postgreVer='12';;
    esac
    
    postgrePath='/etc/postgresql/'$postgreVer'/main'
    mv $postgrePath/postgresql.conf $postgrePath/postgresql.conf.bak
    cp postgresql-$postgreVer.conf $postgrePath/postgresql.conf
    chown postgres:postgres $postgrePath/postgresql.conf
    chmod 644 $postgrePath/postgresql.conf

    mv $postgrePath/pg_hba.conf $postgrePath/pg_hba.conf.bak
    cp pg_hba.conf $postgrePath/pg_hba.conf
    chown postgres:postgres  $postgrePath/pg_hba.conf
    chmod 644  $postgrePath/pg_hba.conf

    service postgresql restart

    mv /usr/local/nginx/conf/nginx.conf /usr/local/nginx/conf/nginx.conf.bak
    cp nginxupload.conf /usr/local/nginx/conf/nginx.conf

    # ln -s ~controller/videofile/ $SRVDIR/src/app/videofile

    echo
    echo
    DATE=$(date "+%Y-%m-%d %H:%M:%S")

    echo $DATE
    echo
    echo $systemName JDS System software installed.
    echo
    echo
}

# install database
installDatabase()
{
    echo
    echo install $systemName JDS System database...
    echo
    echo

    chmod 755 $SRVDIR/install/installdb.sh
    rm database.log
    su postgres -s ./installdb.sh JamesSoft $systemName >> database.log

    echo
    echo $systemName JDS System database installed.
    echo
    echo
}

# 创建各类用户
createAccount()
{
    createControllerAccount
    
    createTMSAccount DCU 1
    createTMSAccount DCU 2
    createTMSAccount CCU 1
    createTMSAccount CCU 2

    createV2XAccount
}

# 创建Controller用户
createControllerAccount()
{
    echo
    echo create controller account...
    echo
    echo

    name='controller'
    pass='controller'

    # 创建账号及密码
    useradd -r -m -s /bin/bash $name
    echo $name:$pass|chpasswd

    # 创建账号成功后，使用该账号创建响相应的目录
    listDir=/home/$name/list
    formatDir=/home/$name/format_file
    logDir=/home/$name/log
    screenDir=/home/$name/screen
    updateDir=/home/$name/update
    mkdir $listDir $formatDir $logDir $screenDir $updateDir
    chown -R $name:$name $listDir $formatDir $logDir $screenDir $updateDir

    # 将背景图片压缩包打开
    cd /home/$name
    su $name -c 'tar -xvf $SRVDIR/install/image.tar' 

    echo
    echo account controller created successfully.
    echo
    echo
}

createTMSAccount()
{
    echo
    echo create TMS account...
    echo
    echo

    name=$1$2
    pass=$1$2

    # 创建账号及密码
    useradd -r -m -s /bin/bash $name
    echo $name:$pass|chpasswd

    # 创建账号成功后，使用该账号创建响相应的目录
    TMSDir=/home/$name/TMS

    UserDir=$TMSDir/$1
    binDir=$UserDir/bin
    objDir=$UserDir/Objects

    mkdir $TMSDir $UserDir $binDir $objDir
    chown -R $name:$name $TMSDir

    ln -s $WORKDIR/Projects/TMS/$1/Source $UserDir/Source
    ln -s $WORKDIR/Projects/TMS/$1/make $UserDir/make
    ln -s $WORKDIR/Projects/TMS/$1/config $UserDir/config
    ln -s $WORKDIR/Projects/TMS/Common $TMSDir/Common

    #  环境变量设置
    cd $SRVDIR/install

    mv /home/$name/.bashrc /home/$name/.bashrc.bak
    cp $1.txt /home/$name/.bashrc
    chown -R $name:$name /home/$name/.bashrc

    echo
    echo TMS account $1 created successfully.
    echo
    echo
}

createV2XAccount()
{
    echo
    echo create V2X account...
    echo
    echo

    name="rsu"
    pass="rsu"

    # 创建账号及密码
    useradd -r -m -s /bin/bash $name
    echo $name:$pass|chpasswd

    # 创建账号成功后，使用该账号创建响相应的目录
    V2XDir=/home/$name

    binDir=$V2XDir/bin
    logDir=$V2XDir/log
    updateDir=$V2XDir/update

    mkdir $binDir $logDir $updateDir
    chown -R $name:$name $binDir $logDir $updateDir

    echo
    echo V2X account created successfully.
    echo
    echo
}

Usage()
{
    echo
    echo Usage: $0 "RootPathName SystemName"
    echo
}

if [ $# -ne 2 ]
then
    Usage
    exit
fi

rootPath=$1
systemName=$2

# set root dictionary
export WORKDIR=$rootPath

# set service dictionary
export SRVDIR=$WORKDIR/JDS

# installSoft

# setConfiguration

installDatabase

# createAccount

echo 
echo
echo Congratulation! The installation is completed.
echo Use command jds to start the server
echo Now, use a browser with address http://server_ip:448 to start the $systemName JDS System,
echo and hope you will enjoy your new system!
echo
echo Notes: Modify /etc/ntc.conf to update IP address and netmask.
echo

