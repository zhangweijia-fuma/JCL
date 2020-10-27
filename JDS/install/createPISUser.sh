#!/bin/bash

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

createControllerAccount

