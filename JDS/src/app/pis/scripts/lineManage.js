////////////////////////////////////////////////////////////////////////////////////////////

angular.module('lineManageControllers',['treeControl', 'highcharts-ng'])

// 维护管理首页
.controller('lineManageCtrl', function($scope, $window, $rootScope, $filter, CommonService, $interval)
{
    var strUser = sessionStorage.getItem("strUser");

    if(strUser)
        $scope.user = JSON.parse(strUser);
    else
        window.location.href = "/login.html";

    $scope.isAdmin = $scope.user.isAdmin;

    // 单线路模式：false， 多线路模式：true
    $scope.lineMode = projConfig.lineMode;
    if ( $scope.lineMode == false)
        $scope.lineName = projConfig.lineName;

    // 日志分页
    $scope.pager = new Pager("pager");
    $scope.pager.pageIndex = 1;
    $scope.pager.pageSize  = 10;

    // 消息分页
    $scope.pager2 = new Pager("pager2");
    $scope.pager2.pageIndex = 1;
    $scope.pager2.pageSize  = 10;

    // 当前显示的运营线路
    $scope.currentLine = {};
    if ( sessionStorage.getItem("currentLine"))
        $scope.currentLine = JSON.parse( sessionStorage.getItem("currentLine") );

    // 运营线路列表
    $scope.lineList    = [];

    // 控制器显示参数
    $scope.controllerSurv = { file_path: "/resource/images/logo.jpg", video_url : "" };

    // 当前选择的节点，车站或列车
    $scope.currentNode = { id : 0 };
    if ( sessionStorage.getItem("currentNode"))
        $scope.currentNode = JSON.parse(sessionStorage.getItem("currentNode"));

    // 默认显示运营线路信息
    // 1: 运营线路信息，2：车站或列车， 3：控制器
    $scope.showLevel = 1;

    $scope.showChart       = false;

    // 默认显示车站列表
    $scope.showType = true;

    if ( sessionStorage.getItem("lineManageShowType"))
    {
        $scope.showType = (sessionStorage.getItem("lineManageShowType") == "true" )? true : false;

        var choose = document.getElementsByName("showType");
        if ( $scope.showType )
        {
            choose[0].checked = true;
            choose[1].checked = false;
        }
        else
        {
            choose[1].checked = true;
            choose[0].checked = false;
        }
    }
    else
        sessionStorage.setItem("lineManageShowType", $scope.showType );

    $scope.chooseShowType = function( type )
    {
        // 保存选择的类型
        sessionStorage.setItem("lineManageShowType", type )
        $scope.showType = type;
        $scope.showController = false;
        $scope.showChart = false;
        $scope.currentNode = { id : 0 };

        $scope.getLineInfo( $scope.currentLine.id );
    }

    // 初始化运营线路数据
    $scope.initialData = function()
    {
        var uriData = "";
        if ( $scope.lineMode == false ) 
            uriData = "name=" + $scope.lineName;

        CommonService.getAll("lineManage", uriData, 
            function(data)
            {
                $scope.lineList     = listToObject( data.lineList, 'rows').rows;
                if ( $scope.currentLine.id > 0 )
                    $scope.chooseLine( $scope.currentLine );
                else if ( $scope.lineList.length > 0 )
                    $scope.chooseLine( $scope.lineList[0]);
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

        $scope.currentNode          = { id : 0 };
        $scope.currentController    = { id : 0};

        $scope.currentLine          = line;
        $scope.lineName             = line.name;

        $scope.getSubLineInfo(line.id);

        // 保存当前线路信息
        sessionStorage.setItem("currentLine", JSON.stringify($scope.currentLine));
        $scope.getLineInfo( line.id );
    }

    // 获取支线信息
    $scope.getSubLineInfo = function( line_id )
    {
        var uriData = "lid=" + line_id;
        CommonService.getAll("subLineInfo", uriData, 
            function(data)
            {
                $scope.subLineList = listToObject( data.subLineList, 'rows').rows;
            },
            function(data)
            {
                onErrorResult(data);
            }
        );
    }

    // 创建运营线路
    $scope.createLine = function()
    {
        sessionStorage.removeItem("lineInfo");
        window.location.href = '#/line_add';
    }

    $scope.updateLine = function()
    {
        sessionStorage.setItem("lineInfo", JSON.stringify($scope.currentLine));
        window.location.href = '#/line_add';
    }

    $scope.deleteLine = function()
    {
        MsgBox.confirm("确认", "确认要删除运营线路 " + $scope.currentLine.name + " 吗？", 
            function ( isConformed ) 
            {
                if (isConformed) 
                {
                    uriData = angular.toJson( $scope.currentLine );
                    CommonService.deleteOne("lineManage", uriData, 
                        function(data)
                        {
                            MsgBox.info("提示", "删除运营线路成功！");
                            $scope.currentLine = { id : 0 };
                            $scope.initialData();
                        },
                        function(data, status )
                        {
                            onErrorResult(data);
                        }
                    );
                }
            }
        );
    }

    // 新增车站
    $scope.createStation = function()
    {
        sessionStorage.setItem("currentLine", JSON.stringify($scope.currentLine));
        sessionStorage.removeItem("stationInfo");
        window.location.href = '#/station_add';
    }

    // 删除车站
    $scope.deleteStation = function()
    {
        MsgBox.confirm("确认", "确定要删除车站 " + $scope.currentNode.show_name + " 吗？", 
            function ( isConformed ) 
            {
                if (isConformed) 
                    $scope.deleteStationConfirmed($scope.currentNode);
            }
        );
    }

    $scope.deleteStationConfirmed = function()
    {
        uriData = { sid: $scope.currentNode.id, lid : $scope.currentLine.id };
        uriData = angular.toJson(uriData);

        CommonService.deleteOne("stationManage", uriData, 
            function(data)
            {
                MsgBox.info("提示", "删除车站成功！");
                $scope.currentNode = { id : 0 };
                $scope.initialData();
            },
            function(data, status )
            {
                onErrorResult(data);
            }
        );
    }

    // 更新车站信息
    $scope.updateStation = function()
    {
        sessionStorage.setItem("stationInfo", JSON.stringify($scope.currentNode));
        window.location.href = '#/station_add';
    }


    // 创建列车
    $scope.createTrain = function()
    {
        $scope.trainMode = "create";
        $scope.newTrain = { line_id : $scope.currentLine.id };

        $("#addPage").modal("show");
    }

    $scope.confirmToSaveTrainInfo = function()
    {
        if($("#train_name").val() == '')
        {
            $("#train_name").testRemind("请输入列车中文名称");
            return;
        }

        if($("#name_en").val() == '')
        {
            $("#name_en").testRemind("请输入列车英文名称");
            return;
        }

        $("#addPage").modal("hide");

        if ( $scope.trainMode == "create")
            $scope.saveTrainInfo( CommonService.createOne, $scope.newTrain );
        else
            $scope.saveTrainInfo( CommonService.updateOne, $scope.newTrain );
    }

    $scope.updateTrain = function()
    {
        $scope.trainMode = "update";
        $scope.newTrain = $scope.currentNode;
        $scope.newTrain.name = $scope.currentNode.show_name;
        $("#addPage").modal("show");
    }

    $scope.saveTrainInfo = function( CommonServiceFunc, trainInfo )
    {
        var uriData = angular.toJson( trainInfo);
        CommonServiceFunc("trainManage", uriData, 
            function(data)
            {
                // 保存成功后重新加载
                $scope.initialData();
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    $scope.deleteTrain = function()
    {
        MsgBox.confirm("确认", "确认要删除列车 " + $scope.currentNode.show_name + " 吗？", 
            function ( isConformed ) 
            {
                if (isConformed) 
                {
                    uriData = angular.toJson( $scope.currentNode );
                    CommonService.deleteOne("trainManage", uriData, 
                        function(data)
                        {
                            MsgBox.info("提示", "操作成功！");
                            $scope.currentNode = { id : 0 };
                            $scope.initialData();
                        },
                        function(data, status )
                        {
                            onErrorResult(data);
                        }
                    );
                }
            }
        );
    }

    // 新增控制器
    $scope.createController = function()
    {
        if ( $scope.showType )
        {
            sessionStorage.setItem("stationInfo", JSON.stringify($scope.currentNode));
            sessionStorage.setItem("controllerInstallType", "1" );
        }
        else
        {
            sessionStorage.setItem("trainInfo", JSON.stringify($scope.currentNode));
            sessionStorage.setItem("controllerInstallType", "0" );
        }

        sessionStorage.removeItem("controllerInfo");
        sessionStorage.setItem("lineInfo", JSON.stringify($scope.currentLine));
        window.location.href = '#/controller_add';

    }

    // 更新控制器信息
    $scope.updateController = function()
    {
        if ( $scope.showType )
        {
            sessionStorage.setItem("stationInfo", JSON.stringify($scope.currentNode));
            sessionStorage.setItem("controllerInstallType", "1" );
        }
        else
        {
            sessionStorage.setItem("trainInfo", JSON.stringify($scope.currentNode));
            sessionStorage.setItem("controllerInstallType", "0" );
        }
        
        sessionStorage.setItem("lineInfo", JSON.stringify($scope.currentLine));
        sessionStorage.setItem("controllerInfo", JSON.stringify($scope.currentController));
        window.location.href = '#/controller_add';
    }

    // 删除控制器
    $scope.deleteController = function()
    {
        // 需要检查当前操作用户是否具有权限
        MsgBox.confirm("确认", "确定要删除控制器 " + $scope.currentController.show_name + " 吗？", 
            function ( isConformed ) 
            {
                if (isConformed) 
                    $scope.deleteControllerConfirmed($scope.currentController);
            }
        );
    }

    $scope.deleteControllerConfirmed = function()
    {
        uriData = { cid: $scope.currentController.id };
        uriData = angular.toJson(uriData);

        CommonService.deleteOne("controllerManage", uriData, 
            function(data)
            {
                MsgBox.info("提示", "删除控制器成功！");
                $scope.initialData();

            },
            function(data, status )
            {
                onErrorResult(data);
            }
        );
    }

    // 重启控制器
    $scope.rebootController = function()
    {
        MsgBox.confirm("确认", "确定要重启控制器 " + $scope.currentController.show_name + " 吗？", 
            function ( isConformed ) 
            {
                if (isConformed == false)
                    return;
    
                var controllerList = [];
                controllerList.push( $scope.currentController );
                uriData = { op: "data", msgType : "reboot", controllerList: controllerList };
                uriData = angular.toJson(uriData);

                CommonService.updatePartOne("messageManage", uriData, 
                    function(data)
                    {
                        MsgBox.info("提示", "重启命令已经发送！");
                        $scope.getMessageList();
                    },
                    function(data, status)
                    {
                        onErrorResult(data);
                    }
                );            
            }
        );
    }

    // 定时器处理函数，定时更新控制器消息、日记及状态
    $scope.statusTimer = null;
    $scope.onTimerForController = function()
    {
        // 获取控制器日志信息
        $scope.pager.pageIndex  = 1;
        $scope.getLogList();

        $scope.pager2.pageIndex = 1;
        $scope.getMessageList();

        // 获取控制器状态信息
        $scope.getControllerStatus();

    }

    // 获取控制器日志
    $scope.getLogList = function()
    {
        var uriData = "op=data&o=" + $scope.pager.pageIndex + "&r=" + $scope.pager.pageSize;
        uriData += "&cid=" + $scope.currentController.id;
        
        CommonService.getAll("logManage", uriData, 
            function(data)
            {
                $scope.logList      = listToObject( data, 'rows').rows;
                $scope.count        = data.count;
                $scope.totalPage    = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
                $scope.pager.update(data.count);

                getDataDict( $scope.logList, "type", "MSG_TYPE_CONTROLLER", "type_name" );
                getDataDict( $scope.logList, "level", "CTRL_LOG_LEVEL", "level_name" );
                getDataDict( $scope.logList, "type", "CTRL_LOG_TYPE", "type_name" );

                for( i = 1; i <= $scope.logList.length; i++)
                {
                    $scope.logList[i - 1].no = i + ( $scope.pager.pageIndex - 1 ) * $scope.pager.pageSize;
                    $scope.logList[i - 1].sendTime = $scope.logList[i - 1].date + " " + $scope.logList[i - 1].time;
                }
            },
            function(data)
            {
                $scope.totalPage = 0;
                $scope.pager.update(0);
                onErrorResult(data);
            }
        );
    }

    $scope.page = function(e)
    {
        $scope.pager.onEvent(e);
        $scope.getLogList();
    };

    // 获取控制器消息
    $scope.getMessageList = function()
    {
        var uriData = "op=data&o=" + $scope.pager2.pageIndex + "&r=" + $scope.pager2.pageSize;
        uriData += "&cid=" + $scope.currentController.id;
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

                for( i = 1; i <= $scope.messageList.length; i++)
                {
                    $scope.messageList[i - 1].no = i + ( $scope.pager2.pageIndex - 1 ) * $scope.pager2.pageSize;
                    $scope.messageList[i - 1].sendTime = $scope.messageList[i - 1].send_date + " " + $scope.messageList[i - 1].send_time;
                }
            },
            function(data)
            {
                $scope.totalPage2 = 0;
                $scope.pager2.update(0);
                onErrorResult(data);
            }
        );
    }

    $scope.page2 = function(e)
    {
        $scope.pager2.onEvent(e);
        $scope.getMessageList();
    };

    // 重发消息给控制器
    $scope.reSendMessage = function(item)
    {
        item.op = "resend";
        var uriData = angular.toJson( item );
        CommonService.updatePartOne("messageManage", uriData, 
            function(data)
            {
                MsgBox.info("提示", "操作成功！");
                $scope.getMessageList();
            },
            function(data)
            {
                onErrorResult(data);
            }
        );

    }

    // 获取控制器状态信息
    $scope.getControllerStatus = function()
    {
        uriData = "op=data&o=1&r=2000&cid=" + $scope.currentController.id;

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
            memoryData.push( $scope.statusList[i].memory /100 );
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

    // 控制器视频监播
    $scope.videoTimer = null;
    $scope.showControllerVideo = function()
    {
        $scope.controllerList = [];
        $scope.controllerList.push( $scope.currentController );

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

    var currentNode = {};
    $scope.updateNodes = function (flag) 
    {
        treeName = ( $scope.showType ) ? "station_tree" : "train_tree";

        var treeObj = $.fn.zTree.getZTreeObj(treeName);
        currentNode.highlight = flag;
        treeObj.updateNode(currentNode);
    }

    // 线路树被点击
    $scope.onTreeClicked = function(event, treeId, treeNode) 
    {
        if ( treeNode == undefined )
            return;

        if ( treeNode.nodeType == "stationRoot" || treeNode.nodeType == "trainRoot")
        {
            // 选择了运营线路
            $scope.showLevel = 1;

            $scope.getSubLineInfo($scope.currentLine.id);
        }
        else if ( treeNode.nodeType == "station")
        {
            $scope.showLevel = 2;

            // 选择了一个车站
             $scope.currentNode = 
             {
                id              : treeNode.id,
                code            : treeNode.code,
                name            : treeNode.name, 
                show_name       : treeNode.show_name,
                name_en         : treeNode.name_en,
                type            : treeNode.type,
                type_name       : treeNode.type_name,
                description     : treeNode.description,
                sort            : treeNode.sort,
                ip_address      : treeNode.ip_address,
                up_start_time   : treeNode.up_start_time,
                up_end_time     : treeNode.up_end_time,
                down_start_time : treeNode.down_start_time,
                down_end_time   : treeNode.down_end_time,
                status          : treeNode.status,
                status_name     : (treeNode.status == '0') ? "在线" : "离线",
            }

            // $scope.showStation          = true;
            // $scope.showTrain            = false;
            // $scope.showController       = false;
            $scope.showChart            = false;
            $scope.currentController    = { id : 0};

            sessionStorage.setItem("currentNode", JSON.stringify($scope.currentNode));
            $scope.__apply();

            // 取消监播定时器
            if ( $scope.videoTimer ) 
            {
                $interval.cancel( $scope.videoTimer );
                $scope.videoTimer = null;
            }
        }
        else if ( treeNode.nodeType == "train")
        {
            $scope.showLevel = 2;
            // 选择了一个列车
            $scope.currentNode = 
            {
                id              : treeNode.id,
                code            : treeNode.code,
                name            : treeNode.name, 
                show_name       : treeNode.show_name,
                name_en         : treeNode.name_en,
                description     : treeNode.description,
                sort            : treeNode.sort,
                line_id         : treeNode.line_id,
                line_name       : treeNode.line_name,
            }

            // $scope.showStation          = false;
            // $scope.showTrain            = true;
            // $scope.showController       = false;
            $scope.showChart            = false;
            $scope.currentController    = { id : 0 };

            sessionStorage.setItem("currentNode", JSON.stringify($scope.currentNode));
            $scope.__apply();

            // 取消监播定时器
            if ( $scope.videoTimer ) 
            {
                $interval.cancel( $scope.videoTimer );
                $scope.videoTimer = null;
            }

        }
        else if ( treeNode.nodeType == "controller")
        {
            $scope.showLevel = 3;

            // 选择了一个控制器
            $scope.currentController = 
            {
                id              : treeNode.id,
                code            : treeNode.code,
                name            : treeNode.name, 
                show_name       : treeNode.show_name,
                serial_no       : treeNode.serial_no,
                device_type     : treeNode.device_type,
                device_type_name: treeNode.device_type_name,
                install_type    : treeNode.install_type,
                install_type_name:treeNode.install_type_name,
                install_id      : treeNode.install_id,
                install_name    : treeNode.install_name,
                ip_address      : treeNode.ip_address,
                open_time       : treeNode.open_time,
                close_time      : treeNode.close_time,
                sort            : treeNode.sort,
                status          : treeNode.status,
                status_name     : (treeNode.status == '0') ? "在线" : "离线",
            }

            // $scope.showStation          = false;
            // $scope.showTrain            = false;

            // $scope.showController       = true;
            $scope.showChart            = false;

            // 设置状态定时器，每1分钟更新一次控制器状态记录、消息记录及日志记录
            $scope.onTimerForController();
            
            if ( !$scope.statusTimer)
                $scope.statusTimer = $interval( $scope.onTimerForController, 60 * 1000 );

            // 在显示列车信息时，设置监播定时器, 每10秒更新一次
            if ( !$scope.videoTimer && !$scope.showType)
                $scope.videoTimer = $interval( $scope.showControllerVideo, 10 * 1000 );

            $scope.showControllerVideo();

            $scope.__apply();
        }

        $scope.updateNodes(false);
        currentNode = treeNode;
        $scope.updateNodes(true); 
    }

    var setting = 
    {
        check: { enable: false },
        data: { },
        key:{ name:'name', children:'children' },
        callback: { onClick: $scope.onTreeClicked },
        view: 
        {
            fontCss: function (treeId, treeNode) 
            {
                selectedItemColor = {"color": "red", "font-weight": (( treeNode.status == '0') ? "bold" : "normal") };
                nonSelectedItemColor = 
                {
                    "color" : (!('status' in treeNode)) ? "black" : (( treeNode.status == '0') ? "green" : "gray"),
                    "font-weight": (( treeNode.status == '0') ? "bold" : "normal"),
                };
                
                return (!!treeNode.highlight) ? selectedItemColor : nonSelectedItemColor;
            }
        }
    };

    $scope.stationTreeData = {};
    $scope.trainTreeData = {};
    $scope.getLineInfo = function( lineID )
    {
        if ( lineID == 0 || lineID == undefined)
            return;

        utName = ( $scope.showType ) ? "station" : "train";
        treeName = ( $scope.showType ) ? "sel_station_tree" : "sel_train_tree";
        uriData = { op:"data", ut: utName, lid: lineID, "cm" : "false" };
        uriData = angular.toJson(uriData);

        $scope.loader = new Loader( treeName );
        CommonService.updatePartOne("lineManage", uriData, 
            function(data)
            {
                $scope.loader.close();
                if ( $scope.showType ) 
                    $scope.stationTreeData = data;
                else
                    $scope.trainTreeData = data;

                treeData = data;

                treeName = ( $scope.showType ) ? "station_tree" : "train_tree";
                $.fn.zTree.init($("#" + treeName ), setting, treeData);

                // 选择第一个节点，第一个节点是车站或列车
                if ( treeData.children.length > 0 )
                {
                    var treeObj = $.fn.zTree.getZTreeObj(treeName);
                    currentNodeName = treeData.children[0].name;
                    // currentNodeName = treeData.name;
                    if ( $scope.currentNode.id > 0 )
                        currentNodeName = $scope.currentNode.name;

                    currentNode = treeObj.getNodesByParamFuzzy("name", currentNodeName, null)[0];
                    $scope.onTreeClicked( null, null, currentNode);
                }

            },
            function(data, status)
            {
                $scope.loader.close();
                onErrorResult(data);
            }
        );

    }

    $scope.__apply = function () 
    {
        if (!$rootScope.$$phase)
            $scope.$digest();
    };

    // 下载车站信息清单，包括控制器列表
    $scope.exportStationInfo = function()
    {
        var uriData = { op: 'station', lid : $scope.currentLine.code };
        uriData = angular.toJson(uriData);

        CommonService.updatePartOne('lineManage', uriData,
            function(data)
            {
                var filename = "车站信息-" + getNowFormatDate1("", "", "-") + ".xls";
                var url = location.origin + data.url;
                var cmd = "<a download=" + filename + " href=" + url + "></a>"; 
                $( cmd )[0].click();
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    // 下载列车控制器列表
    $scope.exportControllerInfo = function()
    {
        var uriData = { op: 'train', lid : $scope.currentLine.code };
        uriData = angular.toJson(uriData);

        CommonService.updatePartOne('lineManage', uriData,
            function(data)
            {
                var filename = "列车信息-" + getNowFormatDate1("", "", "-") + ".xls";
                var url = location.origin + data.url;
                var cmd = "<a download=" + filename + " href=" + url + "></a>"; 
                $( cmd )[0].click();
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
})


//////////////////////////////////////////////////////////////////////////////////////////////
// 新增&更新运营线路
.controller("lineAddCtrl", function ($scope, $location, $rootScope, CommonService, $route)
{
    CommonService.heartbeat();
    $scope.user = null;
    if(sessionStorage.getItem("strUser"))
        $scope.user = JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"

    $scope.lineInfo = {};
    
    // 默认为新建
    $scope.editType = "新建";
    if ( sessionStorage.getItem("lineInfo") )
    {
        $scope.lineInfo = JSON.parse( sessionStorage.getItem("lineInfo") );
        $scope.editType = "更新";
    }
    else
    {
        $scope.lineInfo = {id: 0 };
        $scope.lineInfo.up_start_time   = getNowFormatDate4(":");
        $scope.lineInfo.up_end_time     = getNowFormatDate4(":");
        $scope.lineInfo.down_start_time = getNowFormatDate4(":");
        $scope.lineInfo.down_end_time   = getNowFormatDate4(":");
        $scope.editType = "新建";
    }

    $scope.goback = function()
    {
        history.back();
    }

    $scope.subLineList = [];
    $scope.initialData = function()
    {
        // 获取运营线路支线信息
        if ( $scope.lineInfo.id == 0 )
            return;

        var uriData = "lid=" + $scope.lineInfo.id;
        CommonService.getAll("subLineInfo", uriData, 
            function(data)
            {
                $scope.subLineList = listToObject( data.subLineList, 'rows').rows;
                $scope.stationList = listToObject(data.stationList, 'rows').rows;
            },
            function(data)
            {
                onErrorResult(data);
            }
        );
    }

    // 创建支线
    $scope.CreateSubLine = function()
    {
        $scope.curSubLineInfo = {
            id                  : 0,
            line_id             : $scope.lineInfo.id,
            sub_line_name       : "",
            sub_line_name_en    : "",
            up_start_station    : 0,
            up_end_station      : 0,
            down_start_station  : 0,
            down_end_station    : 0,
            up_start_time       : "",
            up_end_time         : "",
            down_start_time     : "",
            down_end_time       : "",
        };

        $scope.subLineMode = "create";
        $("#addPage").modal("show");
    }

    $scope.updateSubLineInfo = function(item)
    {
        $scope.curSubLineInfo = item;

        $scope.subLineMode = "update";
        $("#addPage").modal("show");
    }

    // 保存支线
    $scope.confirmToSaveSubLineInfo = function()
    {
        var errMsg = "";
        if( $scope.curSubLineInfo.sub_line_name == "" || $scope.curSubLineInfo.sub_line_name_en == "")
            errMsg = "请输入支线中文名称及英文名称！";
        else if ( $scope.curSubLineInfo.up_start_station == 0 || $scope.curSubLineInfo.up_end_station == 0 )
            errMsg = "请选择上行起点站终点站！";
        else if ( $scope.curSubLineInfo.up_start_time == "" || $scope.curSubLineInfo.up_end_time == "" )
            errMsg = "请选择上行首末班时间！";
        else if ( $scope.curSubLineInfo.down_start_station == 0 || $scope.curSubLineInfo.down_end_station == 0 )
            errMsg = "请选择下行起点站终点站！";
        else if ( $scope.curSubLineInfo.down_start_time == "" || $scope.curSubLineInfo.down_end_time == "" )
            errMsg = "请选择下行首末班时间！";

        if ( errMsg != "" )
        {
            MsgBox.info("提示", errMsg);
            return;
        }

        if ( $scope.subLineMode =="create")
            CommonServiceFunc = CommonService.createOne;
        else if ( $scope.subLineMode == "update")
            CommonServiceFunc = CommonService.updateOne;
        else
            CommonServiceFunc = undefined;

        if ( CommonServiceFunc == undefined)
            return;

        var uriData = angular.toJson($scope.curSubLineInfo);
        CommonServiceFunc("subLineInfo", uriData, 
            function(data)
            {
                $("#addPage").modal("hide");
                $scope.initialData();
            },
            function(data)
            {
                onErrorResult(data);
            }
        );
    }

    $scope.showDeleteSubLineInfoDlg = function(item)
    {
        MsgBox.confirm("确认", "确定要删除支线 " + item.sub_line_name + " 吗？", 
            function ( isConformed ) 
            {
                if (isConformed)
                {
                    var uriData = angular.toJson(item);
                    CommonService.deleteOne("subLineInfo", uriData, 
                        function(data)
                        {
                            $scope.initialData();
                        },
                        function(data)
                        {
                            onErrorResult(data);
                        }
                    );
                }
            }
        );
    }

    // 保存线路信息，对线路名称、编号的唯一性检查在后端进行
    $scope.save = function()
    {

        // 检查参数是否合法
        if($("#name").val() == '')
        {
            $("#name").testRemind("请输入运营线路中文名称");
            return;
        }

        if($("#name_en").val() == '')
        {
            $("#name_en").testRemind("请输入运营线路英文名称");
            return;
        }

        if ( $("#code").val() == "" ) 
        {
            $("#code").testRemind("请输入运营线路编号");
            return;
        }

        if ($("#ustime").val() == "" ) 
        {
            $("#ustime").testRemind("请选择一个时间");
            return;
        }

        if ($("#uetime").val() == "" ) 
        {
            $("#uetime").testRemind("请选择一个时间");
            return;
        }

        if ($("#dstime").val() == "" ) 
        {
            $("#dstime").testRemind("请选择一个时间");
            return;
        }

        if ($("#detime").val() == "" ) 
        {
            $("#detime").testRemind("请选择一个时间");
            return;
        }

        var uriData = JSON.stringify( $scope.lineInfo );

        if ( $scope.editType == "新建")
            $scope.saveLineInfo( CommonService.createOne, uriData );
        else if ( $scope.editType == "更新")
            $scope.saveLineInfo( CommonService.updateOne, uriData );
        
    }
    
    // 新建或更新线路信息
    $scope.saveLineInfo = function( func, uriData)
    {
        func("lineManage", uriData,
            function(data)
            {
                sessionStorage.setItem("currentLine", JSON.stringify($scope.lineInfo));
                MsgBox.info("提示", "保存成功！", $scope.goback );
            },
            function(data)
            {
                onErrorResult( data );
            }
        );
    }
})


//////////////////////////////////////////////////////////////////////////////////////////////
// 新增&更新车站
.controller("stationAddCtrl", function ($scope, $location, $rootScope, CommonService, $route)
{
    CommonService.heartbeat();
    $scope.user = null;
    if(sessionStorage.getItem("strUser"))
        $scope.user=JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"

    // 单线路模式：false， 多线路模式：true
    $scope.lineMode = projConfig.lineMode;

    // 初始化
    $scope.stationInfo = {};

    // 默认为运营车站
    $scope.stationInfo.type = 'N';
    
    // 所有运营线路列表
    $scope.allLineList = [];

    // 车站归属的线路列表
    $scope.selectedLineList = [];

    // 当前正在显示的运营线路
    $scope.currentLine = {};

    // 获取当前线路信息
    if ( sessionStorage.getItem("currentLine") )
    {
        $scope.lineInfo = JSON.parse( sessionStorage.getItem("currentLine") );

        $scope.selectedLineList[0] = 
        { 
            line_id         : $scope.lineInfo.id, 
            name            : $scope.lineInfo.name,
            sort            : $scope.lineInfo.id * 200 + 1,
            up_start_time   : getNowFormatDate4(":"),
            up_end_time     : getNowFormatDate4(":"),
            down_start_time : getNowFormatDate4(":"),
            down_end_time   : getNowFormatDate4(":"),
        };

        $scope.currentLineID = $scope.lineInfo.id;
        $scope.currentLine   = $scope.selectedLineList[0];

        $scope.editType = "更新";
    }
    else
    {
        // 报错
    }

    // 判断是新建车站还是更新车站信息    
    $scope.editType = "新建";
    if ( sessionStorage.getItem("stationInfo") )
    {
        $scope.stationInfo = JSON.parse( sessionStorage.getItem("stationInfo") );
        $scope.stationInfo.name = $scope.stationInfo.show_name;
        $scope.editType = "更新";
    }
    else
    {
        $scope.stationInfo = { type: 'N', ip_address : "192.168.2.62" };
        $scope.editType = "新建";

    }

    $scope.goback = function()
    {
        history.back();
    }

    $scope.resetTime = function( item )
    {
        if ( item == 1 )
            $scope.stationInfo.up_start_time = "";
        else if ( item == 2 )
            $scope.stationInfo.up_end_time = "";
        else if ( item == 3 )
            $scope.stationInfo.down_start_time = "";
        else if ( item == 4 )
            $scope.stationInfo.down_end_time = "";
    }

    $scope.initialData = function()
    {
        // 获取所有运营线路清单
        uriData = { op: "line" };
        uriData = angular.toJson(uriData);
        CommonService.updatePartOne('lineManage', uriData,
            function(data)
            {
                $scope.allLineList = listToObject(data.lineList, 'rows').rows;
                // 在新建时更新车站在默认线路的序号
                if ( $scope.editType == "新建")
                {
                    for ( i = 0; i < $scope.allLineList.length; i++)
                    {
                        if ( $scope.allLineList[i].id == $scope.lineInfo.id )
                        {
                            if ( $scope.allLineList[i].max_sort != undefined )
                                $scope.selectedLineList[0].sort = $scope.allLineList[i].max_sort + 1;
                            break;
                        }
                    }
                }
            },
            function(data,status)
            {
                onErrorResult(data);
            }
        );

        // 获取车站归属的线路列表及开行时间
        if ( $scope.editType == "更新")
        {
            uriData = { sid : $scope.stationInfo.id, op : "both" };
            uriData = angular.toJson(uriData);
            CommonService.updatePartOne('stationManage', uriData,
                function(data)
                {
                    $scope.selectedLineList = listToObject(data.selectedLineList, 'rows').rows;
                    if ( $scope.selectedLineList.length > 0 )
                    {
                        // 选择当前线路
                        for(i = 0; i < $scope.selectedLineList.length; i++ )
                        {
                            if ( $scope.selectedLineList[i].line_id == $scope.lineInfo.id )
                            {
                                $scope.currentLineID = $scope.selectedLineList[i].line_id;
                                $scope.currentLine   = $scope.selectedLineList[i];
                            }
                        }
                    }

                },
                function(data,status)
                {
                    onErrorResult(data);
                }
            );
        }
    }

    // 选择运营线路，确定车站在运营车站的上下行时间
    $scope.chooseLine = function()
    {
        if ( $scope.currentLineID == 0 || $scope.currentLineID == undefined )
            return;

        for ( i = 0; i < $scope.selectedLineList.length; i++ )
            if ( $scope.selectedLineList[i].line_id == $scope.currentLineID )
                $scope.currentLine = $scope.selectedLineList[i];

    }

    $scope.selectLine = function()
    {
        // 现有运营线路
        if ( $scope.selectedLineList.length > 0 )
        {
            var liArray = document.getElementsByName("line");
            for( i = 0; i < liArray.length; i++ )
            {
                for( j = 0; j < $scope.selectedLineList.length; j++ )
                {
                    if( liArray[i].value == $scope.selectedLineList[j].name )
                    {
                        liArray[i].checked = true;  
                        break;
                    }
                }
            }  
        }

        // 保存现有已经选择的线路信息
        $scope.oldSelectedLine = [];
        for( i = 0; i < $scope.selectedLineList.length; i++ )
        {
            temp = 
            {
                id              : $scope.selectedLineList[i].id,
                line_id         : $scope.selectedLineList[i].line_id,
                station_id      : $scope.selectedLineList[i].station_id,
                sort            : $scope.selectedLineList[i].sort, 
                up_start_time   : $scope.selectedLineList[i].up_start_time,
                up_end_time     : $scope.selectedLineList[i].up_end_time,
                down_start_time : $scope.selectedLineList[i].down_start_time,
                down_end_time   : $scope.selectedLineList[i].down_end_time,
                name            : $scope.selectedLineList[i].name,
            }

            $scope.oldSelectedLine.splice(0, 0, temp);
        }

        $("#selectUserGroup").modal("show");
    }

    $scope.saveOperationLine = function()
    {
        var obj     = document.getElementsByName("line");             
        $scope.selectedLineList = [];
        for( k in obj )
        {
            if( obj[k].checked )
            {
                temp = { 
                    line_id         : obj[k].id, 
                    name            : obj[k].value, 
                };

                if ( $scope.editType == "更新")
                {
                    // 直接获取现有选择线路的信息
                    for(i = 0; i < $scope.oldSelectedLine.length; i++ )
                    {
                        if ( $scope.oldSelectedLine[i].line_id == temp.line_id )
                        {
                            temp = 
                            {
                                id              : $scope.oldSelectedLine[i].id,
                                line_id         : $scope.oldSelectedLine[i].line_id,
                                station_id      : $scope.oldSelectedLine[i].station_id,
                                sort            : $scope.oldSelectedLine[i].sort, 
                                up_start_time   : $scope.selectedLineList[i].up_start_time,
                                up_end_time     : $scope.selectedLineList[i].up_end_time,
                                down_start_time : $scope.selectedLineList[i].down_start_time,
                                down_end_time   : $scope.selectedLineList[i].down_end_time,
                                name            : $scope.oldSelectedLine[i].name,
                            }

                            break;
                        }
                    }
                }
                else
                {
                    // 新增线路时，获取当前线路的最大序号
                     for(i = 0; i < $scope.allLineList.length; i++ )
                    {
                        if ( $scope.allLineList[i].line_id == temp.line_id )
                        {
                            temp.sort =  $scope.allLineList[i].max_sort + 1;
                            break;
                        }
                    }
                }

                $scope.selectedLineList.splice(0, 0, temp);

            }
        }

        if( $scope.selectedLineList.length == 0 )
        {
            document.getElementById("warning").style.color = "red";
            return;
        }
        else
        {
            document.getElementById("warning").style.color="#fff";
            $("#selectUserGroup").modal("hide");
        }
    }

    $scope.chooseStationType = function( type )
    {
        $scope.stationInfo.type = type;
    }

    // 保存车站信息
    $scope.save = function()
    {
        // 检查参数是否合法
        if($("#name").val() == '')
        {
            $("#name").testRemind("请输入车站中文名称");
            return false;
        }

        if($("#name_en").val() == '')
        {
            $("#name_en").testRemind("请输入车站英文名称");
            return false;
        }

        // if($("#code").val() == '')
        // {
        //     $("#code").testRemind("请输入车站编号");
        //     return;
        // }

        if($("#type").val() == '')
        {
            $("#type").testRemind("请选择车站类型");
            return;
        }

        if( $scope.selectedLineList.length == 0 )
        {
            $("#lineNameList").testRemind("请选择运营线路");
            return;
        }

        if($("#ip_address").val() == '')
        {
            $("#ip_address").testRemind("请输入车站服务器IP地址");
            return;
        }
        
        for ( i = 0; i < $scope.selectedLineList.length; i++ )
        {
            if( $scope.selectedLineList[i].sort == -1 )
            {
                $("#sort").testRemind("请输入车站在" + $scope.selectedLineList[i].name + "的序号");
                return;
            }
        }
        
        if($("#up_start_time").val() == '')
        {
            $("#up_start_time").testRemind("请输入车站上行首班时间");
            return;
        }

        if($("#up_end_time").val() == '')
        {
            $("#up_end_time").testRemind("请输入车站上行末班时间");
            return;
        }

        if($("#down_start_time").val() == '')
        {
            $("#down_start_time").testRemind("请输入车站下行首班时间");
            return;
        }

        if($("#down_end_time").val() == '')
        {
            $("#down_end_time").testRemind("请输入车站下行末班时间");
            return;
        }

        $scope.stationInfo.lineInfo = $scope.selectedLineList;

        var uriData = JSON.stringify( $scope.stationInfo );
        if ( $scope.editType == "新建" )
            $scope.saveStationInfo( CommonService.createOne, uriData );
        else
            $scope.saveStationInfo( CommonService.updateOne, uriData );
        
    }

    // 新建或更新车站信息
    $scope.saveStationInfo = function( func, uriData )
    {
        func("stationManage", uriData,
            function(data)
            {
                sessionStorage.setItem("stationInfo", JSON.stringify($scope.stationInfo));
                MsgBox.info("提示", "保存成功！", $scope.goback );
            },
            function(data)
            {
                onErrorResult( data );
            }
        );
    }
})

