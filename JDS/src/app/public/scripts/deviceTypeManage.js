////////////////////////////////////////////////////////////////////////////////////////////

angular.module('deviceTypeManageControllers',['treeControl', 'highcharts-ng'])

// 设备类型首页
.controller('deviceTypeManageCtrl', function($scope, $window, $rootScope, $filter, CommonService, $interval)
{
    var strUser = sessionStorage.getItem("strUser");

    if(strUser)
        $scope.user = JSON.parse(strUser);
    else
        window.location.href = "/login.html";

    $scope.pager = new Pager(null);
    $scope.pager.pageIndex = 1;
    $scope.pager.pageSize  = 20;

    $scope.pager.update(0);

    $scope.deviceTypeList   = [];
    $scope.typeList         = [];
    $scope.typeID           = 0;
    $scope.deviceTypeInfo   = {};

    // 初始化下拉列表
    $scope.initListCtrl = function()
    {
        var uriData = { op: "list" };
        uriData = angular.toJson(uriData);

        CommonService.updatePartOne("deviceTypeManage", uriData,
            function(data)
            {
                $scope.typeList = listToObject( data.typeList, 'rows').rows;
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
    $scope.initListCtrl();

    $scope.search = function()
    {
        $scope.pager.pageIndex = 1;

        if ( $scope.typeList.selected )
            $scope.typeID = $scope.typeList.selected.id;

        $scope.initialData();
    }

    // 初始化设备类型列表
    $scope.initialData = function()
    {
        var uriData = "op=list" + "&o=" + $scope.pager.pageIndex + "&r=" + $scope.pager.pageSize;
        if ( $scope.typeID > 0 )
            uriData += "&tid=" + $scope.typeID;

        CommonService.getAll("deviceTypeManage", uriData, 
            function(data)
            {
                $scope.deviceTypeList = listToObject(data, 'rows').rows;

                for(var i = 0; i < $scope.deviceTypeList.length; i++)
                {
                    $scope.deviceTypeList[i].no = i + 1 + ($scope.pager.pageIndex - 1) * $scope.pager.pageSize;
                }

                $scope.count        = data.count;
                $scope.totalPage    = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
                $scope.pager.update(data.count);

            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    // 获取指定设备类型的详细信息
    $scope.getDeviceTypeInfo = function( deviceTypeInfo )
    {
        if ( deviceTypeInfo == undefined || deviceTypeInfo.id == 0 )
            return;

        $scope.deviceTypeInfo = deviceTypeInfo;
        
        return;

        // 获取设备的组件列表
        var uriData = "did=" + $scope.deviceTypeInfo.id +  "&o=" + $scope.pager1.pageIndex + "&r=" + $scope.pager1.pageSize;

        CommonService.getAll("componentManage", uriData,
            function(data)
            {
                $scope.componentList = listToObject(data.componentList, 'rows').rows;
                $scope.totalPage1    = Math.ceil(parseInt(data.count) / $scope.pager1.pageSize);
                $scope.pager1.update(data.count);

                if ( $scope.componentList.length > 0 )
                    $scope.getComponentTypeInfo($scope.componentList[0]);
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    // 获取指定设备类型的详细信息
    $scope.getComponentTypeInfo = function( componentTypeInfo )
    {
        if ( componentTypeInfo == undefined || componentTypeInfo.id == 0 )
            return;

        $scope.componentTypeInfo = componentTypeInfo;

        // 获取组件的属性列表
        var uriData = "cid=" + componentTypeInfo.id + "&o=" + $scope.pager2.pageIndex + "&r=" + $scope.pager2.pageSize;
        CommonService.getAll("propertyManage", uriData,
            function(data)
            {
                $scope.propertyList = listToObject(data.propertyList, 'rows').rows;

                $scope.totalPage2    = Math.ceil(parseInt(data.count) / $scope.pager2.pageSize);
                $scope.pager2.update(data.count);

            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    // 创建设备类型
    $scope.showCreateDeviceTypeDlg = function()
    {
        $scope.dlgMode = "新增";

        $scope.itemInfo = {};
        $scope.dlgTitle = '新增设备类型';
        $scope.dlgType = 'device';
        $("#addPage").modal("show");
        // window.location.href = '#/line_add';
    }

    // 创建组件类型
    $scope.showCreateComponentTypeDlg = function()
    {
        $scope.dlgMode = "新增";

        $scope.itemInfo = {};
        $scope.dlgTitle = '新增组件类型';
        $scope.dlgType = 'component';
        $("#addPage").modal("show");
    }

    // 创建属性类型
    $scope.showCreatePropertyTypeDlg = function()
    {
        $scope.dlgMode = "新增";
        if ( $scope.valueTypeList.length == 0 )
        {
            var uriData = { op: 'list'};
            uriData = angular.toJson( uriData );

            CommonService.updatePartOne("propertyManage", uriData,
                function(data)
                {
                    $scope.valueTypeList = listToObject(data.typeList, 'rows').rows;
                    if ( $scope.valueTypeList.length > 0 )
                        $scope.valueTypeList.shift();

                    $scope.itemInfo = {};
                    $scope.dlgTitle = '新增属性类型';
                    $scope.dlgType = 'property';
                    $("#addPage").modal("show");

                },
                function(data, status)
                {
                    onErrorResult(data);
                }
            );
        }
        else
        {
            $scope.itemInfo = {};
            $scope.dlgTitle = '新增属性类型';
            $scope.dlgType = 'property';
            $("#addPage").modal("show");
        }
    }

    $scope.confirmToSave = function()
    {
        if($("#name").val() == '')
        {
            $("#name").testRemind("请输入中文名称");
            return;
        }

        if($("#name_en").val() == '')
        {
            $("#name_en").testRemind("请输入英文名称");
            return;
        }

        if($("#desc").val() == '')
        {
            $("#desc").testRemind("请输入描述说明");
            return;
        }

        if( $scope.dlgType == 'property' && $scope.itemInfo.value_type == '' )
        {
            $("#value_type").testRemind("请选择属性类型");
            return;
        }

        if( $scope.dlgType == 'property' && $('#value').val() == '')
        {
            $("#value").testRemind("请输入属性值");
            return;
        }

        if ( $scope.dlgType == 'property' )
        {
            $scope.itemInfo.value_s = "";
            $scope.itemInfo.value_i = 0;
            $scope.itemInfo.value_f = 0.0;
            
            if ( $scope.itemInfo.value_type == 'STR')
                $scope.itemInfo.value_s = $scope.itemInfo.value;
            else if ( $scope.itemInfo.value_type == 'INT')
                $scope.itemInfo.value_i = parseInt($scope.itemInfo.value);
            else if ( $scope.itemInfo.value_type == 'FLT')
                $scope.itemInfo.value_f = parseFloat($scope.itemInfo.value);
        }

        var modalName = "";
        if ( $scope.dlgType == 'device')
            modalName = "deviceTypeManage";
        else if ( $scope.dlgType == 'component')
            modalName = "componentTypeManage";
        else if ( $scope.dlgType == 'property')
            modalName = "propertyTypeManage";
        
        var CommonServiceFunc = null;
        if ( $scope.dlgMode == "新增")
            CommonServiceFunc = CommonService.createOne;
        else
            CommonServiceFunc = CommonService.updateOne;

        var uriData = angular.toJson( $scope.itemInfo );

        CommonServiceFunc( modalName, uriData, 
            function(data)
            {
                $("#addPage").modal("hide");

                // 保存成功后重新加载
                $scope.initialData();
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );

    }

    $scope.showEditDeviceTypeDlg = function(item)
    {
        $scope.dlgMode = "更新";

        $scope.itemInfo = item;
        $scope.dlgTitle = '更新设备类型';
        $scope.dlgType = 'device';
        $("#addPage").modal("show");
    }

    $scope.showDeleteDeviceTypeDlg = function(item)
    {
        MsgBox.confirm("确认", "确认要删除设备类型 " + item.name + " 吗？", 
            function ( isConformed ) 
            {
                if (isConformed) 
                {
                    uriData = angular.toJson( item );
                    CommonService.deleteOne("deviceTypeManage", uriData, 
                        function(data)
                        {
                            for(var i = 0; i < $scope.deviceTypeList.length; i++)
                            {
                                if ( $scope.deviceTypeList[i].id == item.id )
                                    $scope.deviceTypeList.splice(i, 1);
                            }

                            MsgBox.info("提示", "删除设备类型 " + item.name + " 成功！");
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

    $scope.page = function(e)
    {
        $scope.pager.onEvent(e);
    };
    
    // $scope.page2 = function(e)
    // {
    //     $scope.pager2.onEvent(e);
    // };

    // 在离开页面的时候终止定时器
    $scope.$on('$destroy', function()
    {
    });

    $scope.exportToFile = function()
    {
        var uriData = "op=excel";
        CommonService.getAll('deviceTypeManage', uriData,
            function(data)
            {
                var filename = "设备类型查询" + getNowFormatDate2("", "", "-") + ".xls";
                var url = location.origin + data.url;
                var cmd = "<a download=" + filename + " href=" + url + "></a>"; 
                $( cmd )[0].click();
            },
            function(data,status)
            {
                onErrorResult(data);
            }
        );
    }
})
