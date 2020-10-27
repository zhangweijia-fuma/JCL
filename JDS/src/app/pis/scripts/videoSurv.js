////////////////////////////////////////////////////////////////////////////////////////////

angular.module('videoSurvControllers',['treeControl'])

// 视频监播
.controller('videoSurvCtrl',function($scope, $window, $rootScope, $filter, CommonService, FileUploader, $interval)
{

    var strUser = sessionStorage.getItem("strUser");

    if(strUser)
        $scope.user = JSON.parse(strUser);
    else
        window.location.href = "/login.html";

    // 单线路模式：false， 多线路模式：true
    $scope.lineMode = projConfig.lineMode;
    if ( $scope.lineMode == false)
        $scope.lineName = projConfig.lineName;

    $scope.goback = function()
    {
        history.back();
    }

    // 当前显示的运营线路
    $scope.lineInfo     = { id:0 };
    $scope.stationInfo  = { id : 0 };
    $scope.trainInfo    = { id : 0 };

    // 运营线路列表，用于下拉列表
    $scope.lineList     = [];
    
    // 准备显示监播的控制器列表
    $scope.controllerList = [];

    // 定时器默认不启动
    $scope.timer = null;
    // 设置定时器, 每10秒更新一次
    // $scope.timer = $interval( $scope.showScreen, 10 * 1000 );

    // 默认显示车站列表
    $scope.showType = true;
    $scope.chooseShowType = function( type )
    {
        $scope.showType = type;
        $scope.liveMode = type;

        // 选择列车控制器时，要取消车站控制器的视频监播
        if ( type == false && $scope.controllerList.length > 0 )
            $scope.SetControllerVideo( $scope.controllerList, "0");

        $scope.controllerList = [];

        $scope.checkAllNode($scope.stationTreeData.children, 0, false);
        $.fn.zTree.init($("#station_tree"), setting, $scope.stationTreeData);

        $scope.checkAllNode($scope.trainTreeData.children, 0, false);
        $.fn.zTree.init($("#train_tree"), setting, $scope.trainTreeData);

        $scope.showScreen();

        if ( type == false )
        {
            // 选择列车控制器，需要设置定时器
            if ( $scope.timer == null )
                // 设置定时器, 每10秒更新一次
                $scope.timer = $interval( $scope.showScreen, 10 * 1000 );
        }
    }

    $scope.SetControllerVideo = function( controllerList, mode)
    {
        if ( controllerList.length == 0 || ( mode != "0" && mode != "1" ) )
            return;

        // 向车站控制器发送消息，开始或取消视频监播
        uriData = { op:"data", msgType:"videoSurv", controllerList : controllerList, mode : mode };
        uriData = angular.toJson( uriData );
        CommonService.updatePartOne("messageManage", uriData,
            function(data)
            {
                // 不需要处理返回数据
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );        
    }

    $scope.liveMode = true;
    $scope.chooseLiveMode = function( type )
    {
        $scope.liveMode = type;
        if ( type == true )
        {
            // 显示视频的时候，不需要定时器
            if ( $scope.timer )
            {
                $interval.cancel( $scope.timer );
                $scope.timer = null;
            }

            // 启动控制器的视频监播
            $scope.SetControllerVideo( $scope.controllerList, "1");
        }
        else
        {
            // 显示视频截图，设置定时器, 每10秒更新一次
            if ( $scope.timer == null )
                $scope.timer = $interval( $scope.showScreen, 10 * 1000 );

            // 取消控制器的视频监播
            $scope.SetControllerVideo( $scope.controllerList, "0");
        }

        $scope.showScreen();
    }

    // 初始化下拉列表
    $scope.initListCtrl = function()
    {
        uriData = {};
        if ( $scope.lineInfo.id > 0 )
            uriData.lid = $scope.lineInfo.id;

        uriData = angular.toJson(uriData);
        CommonService.updatePartOne("logManage", uriData,
            function(data)
            {
                $scope.lineList         = listToObject( data.lineList, 'rows').rows;
                $scope.lineList.splice(0, 1);
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
    $scope.initListCtrl();

    // 选择运营线路
    $scope.chooseLine = function( line )
    {
        if ( $scope.lineList.selected == undefined )
            $scope.lineList.selected = line;
        
        $scope.stationInfo  = { id : 0 };
        $scope.trainInfo    = { id : 0 };
        $scope.lineInfo     = $scope.lineList.selected;

        $scope.lineInfo = line;
        $scope.getLineInfo( line.id );
    }

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
                $scope.chooseLine( $scope.lineList[0]);
            },
            function(data)
            {
                onErrorResult(data);
            }
        );
    }

    $scope.onTreeChecked = function(event, treeId, treeNode) 
    {
        // 检查是否已经全部选中或取消
        var status = treeNode.checked ? true : false;

        $scope.setController(treeNode);

    }

    $scope.onTreeClicked = function(event, treeId, treeNode) 
    {
        if ( treeNode == null || treeNode.nodeType != "controller")
            return;

        var status = ( treeNode.checked ) ? false : true;
        treeNode.checked = status;

        var treeName = ( treeNode.install_type <= '1' ) ? "train_tree" : "station_tree";
        var treeObj = $.fn.zTree.getZTreeObj( treeName );

        treeObj.updateNode( treeNode );

        $scope.setController(treeNode);
    }

    $scope.setController = function( node )
    {
        if ( node.nodeType != "controller")
            return;

        temp = 
        {
            id          : node.id,
            code        : node.code,
            name        : node.name,
            install_type: node.install_type,
            install_id  : node.install_id,
            sort        : node.sort,
            ip_address  : node.ip_address,
        }

        if ( node.checked )
        {
            $scope.controllerList.push( temp );

            if ( $scope.showType )
            {
                // 开始控制器的视频监播
                var tempList = [];
                tempList.push(temp);
                $scope.SetControllerVideo(tempList, "1");
            }
        }
        else
        {
            for ( i = 0; i < $scope.controllerList.length; i++ )
            {
                if ( $scope.controllerList[i].id == temp.id )
                {
                    $scope.controllerList.splice(i, 1);

                    if ( $scope.showType )
                    {
                        // 取消控制器视频监播
                        var tempList = [];
                        tempList.push(temp);
                        $scope.SetControllerVideo(tempList, "0");
                    }
                }
            }
        }

        // 选择后，立刻显示图片或视频
        $scope.showScreen();
    }

    // 取消显示控制器的监播画面
    $scope.cancelController = function( item )
    {
        var treeName = ( item.install_type <= '1' ) ? "train_tree" : "station_tree";
        var treeObj = $.fn.zTree.getZTreeObj( treeName );
        for ( i = 0; i < $scope.controllerList.length; i++ )
        {
            if ( $scope.controllerList[i].id == item.id )
            {
                treeNode = treeObj.getNodesByParam("id", item.id, null)[0];
                treeNode.checked = false;
                treeObj.updateNode( treeNode );

                $scope.controllerList.splice(i, 1);

                $scope.showScreen();
            }
        }
    }

    $scope.showScreen = function()
    {
        if ( $scope.controllerList.length == 0 )
        {
            $scope.videoSurvList = [];
            $scope.__apply();
            return;
        }

        $scope.controllerList.sort( sortByItemNum("sort") );

        var uriData = { op:'video', list: $scope.controllerList };
        uriData = angular.toJson( uriData );
        CommonService.updatePartOne("controllerManage", uriData,
            function(data)
            {
                $scope.videoSurvList = data;
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    $scope.__apply = function () 
    {
        if (!$rootScope.$$phase)
            $scope.$digest();
    };

    // 在离开页面的时候终止定时器
    $scope.$on('$destroy', function()
    {
        if ( $scope.timer ) 
        {
            $interval.cancel( $scope.timer );
            $scope.timer = null;

            if ( $scope.liveMode && $scope.controllerList.length > 0 )
            {
                // 取消控制器的视频监播
                $scope.SetControllerVideo( $scope.controllerList, "0");
            }
        }
    });

    var setting = 
    {
        check: { enable: true },
        data: { },
        key:{ name:'name', children:'children' },
        callback: 
        { 
            onCheck: $scope.onTreeChecked, 
            onClick: $scope.onTreeClicked,
        },
        view: 
        {
            fontCss: function (treeId, treeNode) 
            {
                selectedItemColor = {color: "red", "font-weight": "bold"};
                nonSelectedItemColor = { color: "black", "font-weight": "normal", background: "white" };
                
                return (!!treeNode.highlight) ? selectedItemColor : nonSelectedItemColor;
            }
        }
    };

    $scope.getLineInfo = function( lineID )
    {
        if ( lineID == 0 || lineID == undefined)
            return;

        uriData = { op:"data", ut:"station", lid: lineID };
        uriData = angular.toJson(uriData);

        $scope.loader = new Loader('sel_station_tree');
        CommonService.updatePartOne("lineManage", uriData, 
            function(data)
            {
                $scope.loader.close();
                $scope.stationTreeData = data;
                $.fn.zTree.init($("#station_tree"), setting, $scope.stationTreeData);
            },
            function(data, status)
            {
                $scope.loader.close();
                onErrorResult(data);
            }
        );

        uriData = { op:"data", ut:"train", lid: lineID };
        uriData = angular.toJson(uriData);

        $scope.loader = new Loader('sel_train_tree');
        CommonService.updatePartOne("lineManage", uriData, 
            function(data)
            {
                $scope.loader.close();
                $scope.trainTreeData = data;
                $.fn.zTree.init($("#train_tree"), setting, $scope.trainTreeData);
            },
            function(data, status)
            {
                $scope.loader.close();
                onErrorResult(data);
            }
        );
    }

    $scope.checkAllNode = function( nodes, installID, bool)
    {
        for(var i = 0 ; i < nodes.length; i++ )
        {
            if( nodes[i].children.length > 0 )
                $scope.checkAllNode(nodes[i].children, installID, bool);

            if ( ( nodes[i].nodeType == "controller" && nodes[i].install_id == installID ) || installID == 0 )
                nodes[i].checked = bool;
        }
    }

})
