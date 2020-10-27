angular.module('OperationLogControllers', [])

.controller('OperationLogCtrl', function ($scope, CommonService) 
{
    CommonService.heartbeat();
    $scope.user=null;
    if(sessionStorage.getItem("strUser"))
        $scope.user=JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"
    
    $scope.pager = new Pager(null);
    $scope.pager.pageIndex = 1;
    $scope.pager.pageSize  = 20;
  	$scope.operatid        = [];
  	$scope.Operatitem      = [];


    // 下拉列表初始化
    $scope.initListCtrl = function () 
    {
        var uriData = "";
        CommonService.updatePartOne("operationLog", uriData, 
            function (data) 
            {
                $scope.operatorList       = listToObject(data.operatorList, 'rows').rows;
                $scope.OperationMenuCodeList = listToObject(data.operationMenuCodeList, 'rows').rows;
            }, 
            function (data, status) 
            {//失败时的回调函数
            }
        );
    }
    $scope.initListCtrl();

    // 根据操作人查找
    $scope.OperatorID = 0;
    $scope.searchByOpeartor = function()
    {
        $scope.OperatorID = $scope.operatorList.selected.id;
        $scope.pager.pageIndex = 1;
        $scope.initialData();
    }

    // 根据操作项查找
    $scope.OperationMenuCode = "";
    $scope.searchByOpeartionMenuCode = function()
    {
        $scope.OperationMenuCode = $scope.OperationMenuCodeList.selected.menu_code;
        $scope.pager.pageIndex   = 1;
        $scope.initialData();
    }

    //根据开始和结束时间查找
    $scope.searchByTime = function (start, end) 
    {
        var startTime = (new Date(start)).getTime();
        var endTime   = (new Date(end)).getTime();
        if ( !isNaN(startTime) && !isNaN(endTime) ) 
        {
            if (endTime - startTime >= 0) 
            {
                $("#search_log").removeAttr("disabled");
            }
            else 
            {
                $("#search_log").attr("disabled","");
                MsgBox.info("提示", "输入时间不正确，开始时间不能大于结束时间，请重新输入");
                return;
            }
        }

        $scope.initialData();
    };

    $scope.resetTime = function( index )
    {
        if ( index == 1)
            $scope.StartTime = undefined;
        else
            $scope.EndTime = undefined;

        $scope.pager.pageIndex = 1;
        $scope.initialData();   
    }

    // 加载数据
    $scope.initialData = function () 
    {
        uriData = "op=data&o=" + $scope.pager.pageIndex + "&r=" + $scope.pager.pageSize;
        if ($scope.OperatorID > 0 )
            uriData += "&uid=" + $scope.OperatorID;
    
        if ($scope.OperationMenuCode != "")
            uriData += "&mc=" + $scope.OperationMenuCode;
    
        if ($scope.StartTime && $scope.EndTime)
            uriData += "&st=" + $scope.StartTime + "&et=" + $scope.EndTime;
        
  	    CommonService.getAll('operationLog', uriData, 
            function(data)
            {
          	    $scope.operationLogList = listToObject(data, 'rows').rows;
                $scope.count            = data.count;
                $scope.totalPage        = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
          	    $scope.pager.update(data.count);
                for(i=0; i < $scope.operationLogList.length; i++)
                {
                    $scope.operationLogList[i].no = i + 1 + $scope.pager.pageSize * ( $scope.pager.pageIndex - 1 );
                }
          	},
            function(data, status)
            {
                // 失败时的回调函数
                $scope.count      = 0;
                $scope.totalPage  = 0;
                $scope.pager.update(0);
                onErrorResult(data);
          	}
        );
    };

    $scope.exporToFile = function()
    {
        uriData = "op=excel";
        if ($scope.OperatorID > 0 )
            uriData += "&uid=" + $scope.OperatorID;
    
        if ($scope.OperationMenuCode != "")
            uriData += "&mc=" + $scope.OperationMenuCode;
    
        if ($scope.StartTime && $scope.EndTime)
            uriData += "&st=" + $scope.StartTime + "&et=" + $scope.EndTime;
        
        CommonService.getAll('operationLog', uriData, 
            function(data)
            {
                var filename = "操作日志查询" + getNowFormatDate2("", "", "-") + ".xls";
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

    $scope.showDetailLog = function(data)
    {
        $scope.operation_logs1 = data;
        $("#addPage").modal("show");
    }


    $scope.page = function(e)
    {
        $scope.pager.onEvent(e);
        $scope.initialData();
    };
});




