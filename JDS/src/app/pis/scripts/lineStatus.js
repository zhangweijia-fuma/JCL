////////////////////////////////////////////////////////////////////////////////////////////

angular.module('lineStatusControllers', ["highcharts-ng"] )


// 运行状态首页
.controller('lineStatusCtrl', function($scope, $window, $rootScope, $filter, CommonService, $interval)
{
    var strUser = sessionStorage.getItem("strUser");

    if(strUser)
        $scope.user = JSON.parse(strUser);
    else
        window.location.href = "/login.html";


    // 单线路模式：false， 多线路模式：true
    $scope.lineMode         = projConfig.lineMode;
    if ( $scope.lineMode == false)
        $scope.lineName = projConfig.lineName;

    // 当前显示的运营线路
    $scope.currentLine      = {};

    // 运营线路列表
    $scope.lineList         = [];

    // 默认显示车站信息
    $scope.showStation      = true;
    $scope.showTrain        = false;

    $scope.allStationList   = [];
    $scope.trainList        = [];

    $scope.showType         = "station";

    // 初始化运营线路数据
    $scope.initialData = function()
    {

        var uriData = "";
        if ( $scope.lineMode == false ) 
            uriData = "name=" + $scope.lineName;

        // 获取运营线路信息
        CommonService.getAll("lineManage", uriData, 
            function(data)
            {
                $scope.lineList     = listToObject( data.lineList, 'rows').rows;
                if ( $scope.lineList.length > 0)
                    $scope.chooseLine( $scope.lineList[0] )
            },
            function(data)
            {
                onErrorResult(data);
            }
        );
    }


    // 选择运营线路
    $scope.chooseLine = function( line )
    {
        if ( $scope.lineList.selected == undefined )
            $scope.lineList.selected = line;
        
        $scope.currentLine          = line;

        // 保存当前线路信息
        sessionStorage.setItem("currentLine", JSON.stringify($scope.currentLine));

        // 获取当前线路的所有控制器列表
        $scope.getControllerList();

        //$scope.updateStatus();
    }

    // 每一行显示的车站数量
    var unitLen = 7;

    $scope.getControllerList = function()
    {
        if ( $scope.currentLine == undefined || $scope.currentLine.id == 0)
            return;

        var utName = ( $scope.showType == 'station' ) ? "station" : "train";
        var uriData = { op:"data", ut: utName, lid: $scope.currentLine.id, "cm" : "false" };
        uriData = angular.toJson(uriData);

        var list = [];
        $scope.controllerList = [];

        CommonService.updatePartOne("lineManage", uriData, 
            function(data)
            {
                for( var i = 0; i < data.children.length; i++)
                {
                    var temp = {
                        id          : data.children[i].id,
                        code        : data.children[i].code,
                        name        : data.children[i].show_name, 
                        color       : "green",
                        controller  : [],
                    }

                    if ( $scope.showType == 'station' )
                    {
                        temp.color      = data.children[i].status == '0' ? "green" : "gray";
                    }

                    for(var j = 0; j < data.children[i].children.length; j++)
                    {
                        var temp2 = {
                            id          : data.children[i].children[j].id,
                            code        : data.children[i].children[j].code,
                            name        : data.children[i].children[j].name,
                            install_id  : data.children[i].children[j].install_id,
                            install_type: data.children[i].children[j].install_type,
                            install_name: data.children[i].children[j].install_name,
                            ip_address  : data.children[i].children[j].ip_address,
                            install_type_name : data.children[i].children[j].install_type_name,
                            open_time   : data.children[i].children[j].open_time,
                            close_time  : data.children[i].children[j].close_time,
                            status      : data.children[i].children[j].status,
                            show        : true,
                            color       : getDataDictValue('ERROR_COLOR', data.children[i].children[j].status ),
                            fontWeight  : data.children[i].children[j].status == '0' ? "bold"  : "normal",
                        }
                        temp.controller.push(temp2);

                        // 保存控制器列表，以在查询控制器状态时使用
                        $scope.controllerList.push(temp2);
                    }

                    list.push(temp);
                }

                if ( $scope.showType == 'train' )
                {
                    $scope.trainList = list;
                    return;
                }

                // 根据每行显示的控制器数量，将列表分为多个列表
                var index = 0;
                for(var i = 0;  i < list.length; i ++)
                {
                    if ( i == list.length - 1 )
                        index = unitLen - 1;
                    
                    index += 1;
                    if ( index == unitLen || i == list.length - 1 ) 
                    {
                        temp = list.slice( $scope.allStationList.length * unitLen, i + 1);
                        $scope.allStationList.push( temp );
                        index = 0;
                    }
                }

                return;
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );

    }

    // 更新车站状态及控制器列表中所有控制器的状态
    $scope.updateStatus = function()
    {
        if ( $scope.showType == 'station' )
        {
            var uriData = "lid=" + $scope.currentLine.id;
            CommonService.getAll("stationManage", uriData, 
                function(data)
                {
                    var stationList     = listToObject( data, 'rows').rows;
                    if ( stationList.length == 0)
                        return;

                    for ( var i = 0; i < $scope.allStationList.length; i++)
                    {
                        for (var j = 0; j < $scope.allStationList[i].length; j++)
                        {
                            for(var k = 0; k < stationList.length; k++)
                            {
                                if ( $scope.allStationList[i][j].id != stationList[k].id )
                                    continue;

                                $scope.allStationList[i][j].color = (stationList[k].status == '0' ) ? "green" : "gray";
                            }
                        }
                    }
                },
                function( data, status)
                {
                    onErrorResult(data);
                }
            );
        }

        var uriData = { op: 'status', list: $scope.controllerList };
        uriData = angular.toJson(uriData);
        CommonService.updatePartOne("controllerManage", uriData, 
            function(data)
            {
                var ctrlList = data;
                if ( ctrlList.length != $scope.controllerList.length )
                    return;

                var list =  ( $scope.showType == 'station') ? $scope.allStationList : $scope.trainList;

                for ( var i = 0; i < list.length ; i++ )
                {
                    if ( $scope.showType == 'station' )
                    {
                        for ( var j = 0; j < list[i].length; j++)
                        {
                            for ( var k = 0; k < list[i][j].controller.length; k++)
                                $scope.getControllerInfo( list[i][j].controller[k], ctrlList);
                        }
                    }
                    else
                    {
                        for ( var k = 0; k < list[i].controller.length; k++)
                            $scope.getControllerInfo( list[i].controller[k], ctrlList);
                    }
                }
                
            },
            function( data, status)
            {
                onErrorResult(data);
            }
        );
    }

    // 更新指定控制器状态
    $scope.getControllerInfo = function( controller, ctrlList)
    {
        for ( var i = 0; i < ctrlList.length; i++)
        {
            if ( controller.id == ctrlList[i].id )
                controller.color = getDataDictValue( "ERROR_COLOR", ctrlList[i].status);
        }
    }

    // 设置定时器, 每10秒更新状态一次
    $scope.videoTimer = $interval( $scope.updateStatus, 10 * 1000 );

    // 在离开页面的时候终止定时器
    $scope.$on('$destroy', function()
    {
        if ( $scope.videoTimer ) 
        {
            $interval.cancel( $scope.videoTimer );
            $scope.videoTimer = null;
        }
    });

    $scope.chooseShowType = function(type)
    {
        if ( type == 1 )
        {
            $scope.showTrain = false;
            $scope.showStation = true;
            $scope.showType = 'station';

        }
        else
        {
            $scope.showStation = false;
            $scope.showTrain = true;
            $scope.showType = 'train';
        }

        $scope.getControllerList();
    }

    // 显示控制器的详细状态信息
    $scope.showControllerDetial = function( controller )
    {
        // 保存当前线路信息
        sessionStorage.setItem("showControllerItem", JSON.stringify(controller));
        window.location.href = '#/controllerStatus';
    }

})


//////////////////////////////////////////////////////////////////////////////////////////////
// 控制器详细状态信息显示
.controller("controllerStatusCtrl", function ($scope, $location, $rootScope, CommonService, $route, $interval)
{
    CommonService.heartbeat();
    $scope.user = null;
    if(sessionStorage.getItem("strUser"))
        $scope.user = JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"

    // 日志分页
    $scope.pager = new Pager("pager");
    $scope.pager.pageIndex = 1;
    $scope.pager.pageSize  = 20;

    // 消息分页
    $scope.pager2 = new Pager("pager2");
    $scope.pager2.pageIndex = 1;
    $scope.pager2.pageSize  = 20;

    // 初始化
    $scope.controllerInfo   = { id: 0};
    $scope.logList          = [];
    $scope.messageList      = [];
    $scope.statusList       = [];
    $scope.controllerResourceInfo = [];

    if ( sessionStorage.getItem("showControllerItem") )
        $scope.controllerInfo = JSON.parse( sessionStorage.getItem("showControllerItem"));

    // 初始化下拉列表
    $scope.senderList   = [];
    $scope.msgTypeList = [];
    $scope.sendStatusList = [];
    $scope.send_date    = "";
    $scope.receive_date = "";

    // 默认查询当前数据
    $scope.dataType = "current";
    $scope.dataSource = "internal";

    $scope.currentMsgItem = { id: 0};

    $scope.initListCtrl = function()
    {
        uriData = { op: "list"};
        uriData = angular.toJson(uriData);
        CommonService.updatePartOne("messageList", uriData,
            function(data)
            {
                $scope.senderList       = listToObject( data.senderList, 'rows').rows;
                $scope.msgTypeList      = listToObject( data.msgTypeList, 'rows').rows;
                $scope.sendStatusList   = listToObject( data.sendStatusList, 'rows').rows;
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
    $scope.initListCtrl();

    $scope.searchMessage = function()
    {
        $scope.pager.pageIndex = 1;
        $scope.initialData();
    }

    $scope.initialData = function()
    {
        if ( $scope.controllerInfo.id == 0 )
        {
            MsgBox.info("系统错误", "无有效控制器ID！");
            history.back();
        }

        if ( $scope.controllerInfo.install_type == '0' || $scope.controllerInfo.install_type == '1' )
            $scope.showType = false;
        else
            $scope.showType = true;

        // 设置监播定时器, 每10秒更新一次
        if ( $scope.showType == false )
            $scope.videoTimer = $interval( $scope.showControllerVideo, 10 * 1000 );

        $scope.onTimerForController();
        $scope.showControllerVideo();
    }

    // 定时获取控制器状态、日志及消息记录
    $scope.onTimerForController = function()
    {
        // 获取控制器日志
        var uriData = "op=data&o=" + $scope.pager.pageIndex + "&r=" + $scope.pager.pageSize;
        uriData += "&cid=" + $scope.controllerInfo.id;
        
        CommonService.getAll("logManage", uriData, 
            function(data)
            {
                $scope.logList      = listToObject( data, 'rows').rows;
                $scope.count        = data.count;
                $scope.totalPage    = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
                $scope.pager.update(data.count);

                getDataDict( $scope.logList, "type", "MSG_TYPE_CONTROLLER", "type_name" );
                getDataDict( $scope.logList, "level", "CTRL_RECEIVE_STATUS", "level_name" );
                getDataDict( $scope.logList, "level", "ERROR_COLOR", "color" );

                for( i = 1; i <= $scope.logList.length; i++)
                {
                    $scope.logList[i - 1].no = i + ( $scope.pager.pageIndex - 1 ) * $scope.pager.pageSize;
                }
            },
            function(data)
            {
                $scope.totalPage = 0;
                $scope.pager.update(0);
                onErrorResult(data);
            }
        );

        // 获取控制器消息
        var uriData = "op=data&o=" + $scope.pager2.pageIndex + "&r=" + $scope.pager2.pageSize;
        uriData += "&cid=" + $scope.controllerInfo.id;
        uriData += "&dataType=current";
        
        CommonService.getAll("messageManage", uriData, 
            function(data)
            {
                $scope.messageList  = listToObject( data, 'rows').rows;
                $scope.count2       = data.count;
                $scope.totalPage2   = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
                $scope.pager2.update(data.count);

                getDataDict( $scope.messageList, "message_type", "MESSAGE_TYPE", "message_type_name" );
                getDataDict( $scope.messageList, "send_status", "CTRL_RECEIVE_STATUS", "send_status_name" );

                for ( i = 0; i < $scope.messageList.length; i++)
                {
                    if ($scope.messageList[i].message_sn == 0 )
                        $scope.messageList.splice(i, 0);
                }

                // console.log($scope.messageList);

                for( i = 1; i <= $scope.messageList.length; i++)
                {
                    $scope.messageList[i - 1].no = i + ( $scope.pager2.pageIndex - 1 ) * $scope.pager2.pageSize;
                    $scope.messageList[i - 1].receive_date = $scope.messageList[i - 1].send_date;
                    $scope.messageList[i - 1].receive_time = $scope.messageList[i - 1].send_time;
                }

            },
            function(data)
            {
                $scope.totalPage2 = 0;
                $scope.pager2.update(0);
                onErrorResult(data);
            }
        );

        // 获取控制器状态信息
        uriData = "op=data&o=1&r=2000&cid=" + $scope.controllerInfo.id;

        CommonService.getAll("controllerStatus", uriData, 
            function(data)
            {
                $scope.statusList = [];
                var tempList = listToObject( data, 'rows').rows;
                for ( var i = tempList.length - 1; i >= 0; i--)
                    $scope.statusList.push( tempList[i]);

                $scope.controllerResourceInfo = $scope.statusList[0];

                $scope.showControllerChart();
            },
            function(data)
            {
                onErrorResult(data);
            }
        );
    }

    // 控制器报表显示
    $scope.showControllerChart = function()
    {
        if( $scope.statusList.length == 0 )
        {
            $scope.showChart = false;
            return;
        }

        $scope.showChart = true;
            
        var cpu = { name : "CPU" };
        var memory = { name : "内存"};
        var harddisk = { name : "硬盘"};
        var cpuData = [];
        var memoryData = [];
        var harddiskData = [];
        var categories = [];

        for ( i = 0; i < $scope.statusList.length; i++)
        {
            cpuData.push( $scope.statusList[i].cpu / 100 );
            memoryData.push( $scope.statusList[i].memory / 100 );
            harddiskData.push( $scope.statusList[i].harddisk / 100 );
            categories.push($scope.statusList[i].date + " " + $scope.statusList[i].time);
        }

        cpu.data = cpuData;
        memory.data = memoryData;
        harddisk.data = harddiskData;

        $scope.seriesData = [];
        $scope.seriesData[0] = cpu;
        $scope.seriesData[1] = memory;
        $scope.seriesData[2] = harddisk;

        $scope.serverChart = 
        {
            options: 
            {
                chart: { type: 'line', style: { backgroundColor: 'white', }, },
                xAxis: { categories: categories, crosshair: true },
                yAxis: { title: { text: '占有率'}, },
                yAxis: { title: '', max: 100, min: 0 },
                tooltip: { formatter: function() 
                    { 
                        return this.x + " " + this.series.name + '使用率：'+ Highcharts.numberFormat(this.y, 2) + '%';
                    }
                },
            },
            title: { text: '控制器硬件状态' },
            
            series : $scope.seriesData,
        };

    };
    
    // 设置状态定时器，每分钟更新一次控制器状态、消息及日志
    $scope.statusTimer = $interval( $scope.onTimerForController, 60 * 1000 );

    // 控制器视频监播
    $scope.showControllerVideo = function()
    {
        $scope.controllerList = [];
        $scope.controllerList.push( $scope.controllerInfo );

        var uriData = { op:'video', list: $scope.controllerList };
        uriData = angular.toJson( uriData );
        CommonService.updatePartOne("controllerManage", uriData,
            function(data)
            {
                if ( data.length != 1 )
                    return;

                $scope.controllerSurv = data[0];
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    // 在离开页面的时候终止定时器
    $scope.$on('$destroy', function()
    {
        if ( $scope.videoTimer ) 
        {
            $interval.cancel( $scope.videoTimer );
            $scope.videoTimer = null;
        }

        if ( $scope.statusTimer ) 
        {
            $interval.cancel( $scope.statusTimer );
            $scope.statusTimer = null;
        }
    });

    $scope.page = function(e)
    {
        $scope.pager.onEvent(e);
        $scope.getLogList();
    };

    $scope.page2 = function(e)
    {
        $scope.pager2.onEvent(e);
        $scope.getMessageList();
    };

    $scope.goback = function()
    {
        history.back();
    }
})

