////////////////////////////////////////////////////////////////////////////////////////////

angular.module('ctrlLogManageControllers',['treeControl'])

// 控制器日志查询管理首页
.controller('ctrlLogQueryCtrl', function($scope, $window, $rootScope, $filter, CommonService)
{
    var strUser = sessionStorage.getItem("strUser");

    if(strUser)
        $scope.user = JSON.parse(strUser);
    else
        window.location.href = "/login.html";


    // 单线路模式：false， 多线路模式：true
    $scope.lineMode = projConfig.lineMode;

    $scope.pager = new Pager(null);
    $scope.pager.pageIndex = 1;
    $scope.pager.pageSize  = 20;

    $scope.selectedTrainID      = 0;
    $scope.selectedLineID       = 0;
    $scope.selectedControllerID = 0;
    $scope.selectedStationID    = 0;

    $scope.start_time = "";
    $scope.end_time = "";

    $scope.logList = [];

    $scope.getSelectedItem = function( id, list )
    {
        list.selected = list[0];
        if ( id == 0 )
            return;

        for(i = 0; i < list.length; i++ )
        {
            if ( list[i].id == id )
            {
                list.selected = list[i];
                return;
            }
        }
    
        return;   
    }

    // 初始化下拉列表
    $scope.initListCtrl = function()
    {
        uriData = {};
        if ( $scope.selectedLineID > 0 )
            uriData.lid = $scope.selectedLineID;

        if ( $scope.selectedTrainID > 0 )
            uriData.tid = $scope.selectedTrainID;
        
        if ( $scope.selectedStationID > 0 )
            uriData.sid = $scope.selectedStationID;

        uriData = angular.toJson(uriData);
        CommonService.updatePartOne("logManage", uriData,
            function(data)
            {
                $scope.lineList         = listToObject( data.lineList, 'rows').rows;
                $scope.stationList      = listToObject( data.stationList, 'rows').rows;
                $scope.trainList        = listToObject( data.trainList, 'rows').rows;
                $scope.controllerList   = listToObject( data.controllerList, 'rows').rows;

                $scope.getSelectedItem( $scope.selectedLineID, $scope.lineList );
                $scope.getSelectedItem( $scope.selectedStationID, $scope.stationList );
                $scope.getSelectedItem( $scope.selectedTrainID, $scope.trainList );
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
    $scope.initListCtrl();

    $scope.searchByLine = function()
    {
        $scope.selectedStationID = 0;
        $scope.selectedControllerID = 0;
        $scope.selectedTrainID = 0;
        
        $scope.selectedLineID = $scope.lineList.selected.id;

        $scope.initListCtrl();

        $scope.pager.pageIndex = 1;
        $scope.initialData();
    }

    $scope.searchByStation = function()
    {
        $scope.selectedTrainID = 0;
        $scope.selectedControllerID = 0;
        $scope.selectedStationID = $scope.stationList.selected.id;

        $scope.initListCtrl();

        $scope.pager.pageIndex = 1;
        $scope.initialData();
    }

    $scope.searchByTrain = function()
    {
        $scope.selectedStationID = 0;
        $scope.selectedControllerID = 0;
        $scope.selectedTrainID = $scope.trainList.selected.id;

        $scope.initListCtrl();

        $scope.pager.pageIndex = 1;
        $scope.initialData();        
    }

    $scope.searchByController = function()
    {
        $scope.selectedControllerID = $scope.controllerList.selected.id;

        $scope.pager.pageIndex = 1;
        $scope.initialData();        
    }

    $scope.searchByTime = function()
    {
        $scope.pager.pageIndex = 1;
        $scope.initialData();   
    }

    $scope.resetTime = function( index )
    {
        if ( index == 1)
            $scope.start_time = "";
        else
            $scope.end_time = "";

        $scope.pager.pageIndex = 1;
        $scope.initialData();   
    }

    $scope.initialData = function()
    {
        var uriData = "op=data&o=" + $scope.pager.pageIndex + "&r=" + $scope.pager.pageSize;
        if ( $scope.selectedLineID > 0 )
            uriData += "&lid=" + $scope.selectedLineID;

        if ( $scope.selectedTrainID > 0 )
            uriData += "&tid=" + $scope.selectedTrainID;
        
        if ( $scope.selectedStationID > 0 )
            uriData += "&sid=" + $scope.selectedStationID;
        
        if ( $scope.selectedControllerID > 0 )
            uriData += "&cid=" + $scope.selectedControllerID;
        
        if ( $scope.start_time != "" && $scope.end_time != "" )
            uriData += "&st=" + $scope.start_time + "&et=" + $scope.end_time;

        CommonService.getAll("logManage", uriData, 
            function(data)
            {
                $scope.logList      = listToObject( data, 'rows').rows;
                $scope.count        = data.count;
                $scope.totalPage    = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
                $scope.pager.update(data.count);

                getDataDict($scope.logList, "type", "CTRL_LOG_TYPE", "type_name"  );
                getDataDict($scope.logList, "level", "CTRL_LOG_LEVEL", "level_name"  );
                getDataDict($scope.logList, "level", "ERROR_COLOR", "color"  );

                for( i = 1; i <= $scope.logList.length; i++)
                {
                    $scope.logList[i - 1].no = i + ( $scope.pager.pageIndex - 1 ) * $scope.pager.pageSize;
                    $scope.logList[i - 1].sendTime = $scope.logList[i - 1].date + " " + $scope.logList[i - 1].time;
                }

            },
            function(data)
            {
                $scope.pager.update(0);
                onErrorResult(data);
            }
        );
    }

    $scope.page = function(e)
    {
        $scope.pager.onEvent(e);
        $scope.initialData();
    };

    $scope.exporLogToFile = function()
    {
        var uriData = "op=excel";

        if ( $scope.selectedLineID > 0 )
            uriData += "&lid=" + $scope.selectedLineID;

        if ( $scope.selectedTrainID > 0 )
            uriData += "&tid=" + $scope.selectedTrainID;
        
        if ( $scope.selectedStationID > 0 )
            uriData += "&sid=" + $scope.selectedStationID;
        
        if ( $scope.selectedControllerID > 0 )
            uriData += "&cid=" + $scope.selectedControllerID;
        
        if ( $scope.start_time != "" && $scope.end_time != "" )
            uriData += "&st=" + $scope.start_time + "&et=" + $scope.end_time;

        CommonService.getAll('logManage', uriData,
            function(data)
            {
                var filename = "控制器日志查询" + getNowFormatDate2("", "", "-") + ".xls";
                var url = location.origin + data.url;
                var cmd = "<a download=" + filename + " href=" + url + "></a>"; 
                $( cmd )[0].click();
            },
            function(data,status)
            {//失败时的回调函数
            }
        );
    }
})


