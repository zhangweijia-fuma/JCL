////////////////////////////////////////////////////////////////////////////////////////////

angular.module('controllerManageControllers',['treeControl'])

//////////////////////////////////////////////////////////////////////////////////////////////
// 新增&更新控制器
.controller("controllerAddCtrl", function ($scope, $location, $rootScope, CommonService, $route)
{
    CommonService.heartbeat();
    $scope.user = null;
    if(sessionStorage.getItem("strUser"))
        $scope.user=JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"

    // 初始化
    $scope.controllerInfo = {};
    
    $scope.controllerInfo.open_time  = getNowFormatDate4(":");
    $scope.controllerInfo.close_time = getNowFormatDate4(":");
    $scope.controllerInfo.ip_address = "192.168.1.20";
    $scope.controllerInfo.device_type= "1";
    
    // 控制器归属的车站或车辆信息
    $scope.trainInfo  = {};
    $scope.stationInfo  = {};
    $scope.lineInfo     = {};

    // 获取当前车站或车辆信息
    // type: 0: 车辆，1：车站
    $scope.installType = parseInt(sessionStorage.getItem("controllerInstallType"));
    if ( $scope.installType == "0" )
    {
        // 车辆
        $scope.trainInfo = JSON.parse( sessionStorage.getItem("trainInfo") );
        $scope.controllerInfo.install_type  = "0";
        $scope.controllerInfo.install_id    = $scope.trainInfo.id;
        $scope.controllerInfo.install_name  = $scope.trainInfo.show_name;
        $scope.controllerInfo.line_id       = $scope.trainInfo.line_id;
        
        $scope.install_location = '车辆';

    }
    else
    {
        // 车站
        $scope.stationInfo = JSON.parse( sessionStorage.getItem("stationInfo") );
        $scope.lineInfo    = JSON.parse( sessionStorage.getItem("lineInfo") );
        $scope.controllerInfo.install_type  = "2";
        $scope.controllerInfo.install_id    = $scope.stationInfo.id;
        $scope.controllerInfo.install_name  = $scope.stationInfo.show_name;
        $scope.controllerInfo.line_id       = $scope.lineInfo.id;

        $scope.install_location = '车站';
    }


    // 判断是新建还是更新信息
    $scope.editType = "新建";
    if ( sessionStorage.getItem("controllerInfo") )
    {
        $scope.controllerInfo = JSON.parse( sessionStorage.getItem("controllerInfo") );
        $scope.controllerInfo.name = $scope.controllerInfo.show_name;

        $scope.editType = "更新";
    }
    else
    {
        $scope.editType = "新建";
    }

    $scope.goback = function()
    {
        history.back();
    }

    // 下拉列表准备
    $scope.initialListCtrl = function()
    {
    }
    $scope.initialListCtrl();

    $scope.initialData = function()
    {
    }

    // 保存控制器信息
    $scope.save = function()
    {
        // 检查参数是否合法
        if($("#ip_address").val() == '')
        {
            $("#up_start_time").testRemind("请输入控制器IP地址");
            return;
        }

        if($("#code").val() == '')
        {
            $("#code").testRemind("请输入控制器编号");
            return;
        }

        if($("#open_time").val() == '')
        {
            $("#up_end_time").testRemind("请输入控制器开机时间");
            return;
        }

        if($("#close_time").val() == '')
        {
            $("#down_start_time").testRemind("请输入控制器休眠时间");
            return;
        }

        var uriData = JSON.stringify( $scope.controllerInfo );
        if ( $scope.editType == "新建" )
            $scope.saveControllerInfo( CommonService.createOne, uriData );
        else
            $scope.saveControllerInfo( CommonService.updateOne, uriData );
        
    }

    // 新建或更新控制器信息
    $scope.saveControllerInfo = function( func, uriData )
    {
        func("controllerManage", uriData,
            function(data)
            {
                sessionStorage.setItem("controllerInfo", JSON.stringify($scope.controllerInfo));
                $scope.isBack = true;
                MsgBox.info("提示", "保存成功！", $scope.goback );
            },
            function(data)
            {
                if ( data.original_message )
                    MsgBox.info( "提示", data.original_message);
                else
                    MsgBox.info( "提示", data.message );
            }
        );
    }
})
