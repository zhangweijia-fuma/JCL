////////////////////////////////////////////////////////////////////////////////////////////

angular.module('componentManageControllers',['treeControl', 'highcharts-ng'])

// 设备类型首页
.controller('componentManageCtrl', function($scope, $window, $rootScope, $filter, CommonService, $interval)
{
    var strUser = sessionStorage.getItem("strUser");

    if(strUser)
        $scope.user = JSON.parse(strUser);
    else
        window.location.href = "/login.html";

    $scope.pager1 = new Pager("pager_component");
    $scope.pager1.pageIndex = 1;
    $scope.pager1.pageSize  = 20;
    $scope.pager1.update(0);
    $scope.totalPage1 = 0;

    $scope.pager2 = new Pager("pager_property");
    $scope.pager2.pageIndex = 1;
    $scope.pager2.pageSize  = 20;

    $scope.pager2.update(0);
    $scope.totalPage2 = 0;

    $scope.componentList    = [];
    $scope.propertyList     = [];

    $scope.deviceTypeInfo   = {};
    $scope.componentInf     = { id: 0};
    $scope.propertyTypeInfo = { id: 0};
    $scope.showPropertyInfo = false;

    $scope.valueTypeList    = [];

    // 初始化组件列表
    $scope.initialData = function()
    {
        uriData     = "op=list" + "&o=" + $scope.pager1.pageIndex + "&r=" + $scope.pager1.pageSize;

        CommonService.getAll("componentManage", uriData, 
            function(data)
            {
                $scope.componentList = listToObject(data, 'rows').rows;
                $scope.totalPage1    = Math.ceil(parseInt(data.count) / $scope.pager1.pageSize);
                $scope.pager1.update(data.count);

                for(var i = 0; i < $scope.componentList.length; i++)
                {
                    $scope.componentList[i].no = i + 1 + ($scope.pager1.pageIndex - 1) * $scope.pager1.pageSize;
                }

                if ( $scope.componentList.length > 0 )
                {
                    $scope.selectComponent($scope.componentList[0]);
                }
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    // 创建组件
    $scope.showCreateComponentDlg = function()
    {
        $scope.itemInfo = {};
        $scope.editMode = '新增';
        $("#addPage").modal("show");
    }

    // 更新组件信息
    $scope.showEditComponentDlg = function(item)
    {
        $scope.itemInfo = {};
        $scope.itemInfo.name        = item.name;
        $scope.itemInfo.name_en     = item.name_en;
        $scope.itemInfo.description = item.description;
        $scope.itemInfo.id          = item.id;
        
        $scope.editMode = '更新';
        $("#addPage").modal("show");
    }

    $scope.selectComponent = function(item)
    {
        if ( item == undefined || item.id == 0 )
            return;

        $scope.componentInfo    = item;
        $scope.showPropertyInfo = false;
        $scope.propertyInfo     = { id: 0};
        $scope.propertyMode     = '查看';

        var uriData = "opdata&oid=" + item.id + "&o=" + $scope.pager2.pageIndex + "&r=" + $scope.pager2.pageSize;
        CommonService.getAll("propertyManage", uriData, 
            function(data)
            {
                $scope.propertyList = listToObject(data, 'rows').rows;
                $scope.totalPage2    = Math.ceil(parseInt(data.count) / $scope.pager2.pageSize);
                $scope.pager2.update(data.count);

                for(var i = 0; i < $scope.propertyList.length; i++)
                {
                    $scope.propertyList[i].no = i + 1 + ($scope.pager2.pageIndex - 1) * $scope.pager2.pageSize;
                }
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    // 创建属性类型
    $scope.createProperty = function()
    {
        $scope.propertyInfo     = { id: 0};
        $scope.showPropertyInfo = true;
        $scope.editMode         = '新增';
        $scope.propertyMode     = "编辑";

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
                },
                function(data, status)
                {
                    onErrorResult(data);
                }
            );
        }
    }

    // type 参数从页面按钮来
    $scope.confirmToSave = function(type)
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

        if($("#description").val() == '')
        {
            $("#description").testRemind("请输入描述说明");
            return;
        }

        if ( type == 'property' )
        {
            $scope.propertyInfo.value_s = "";
            $scope.propertyInfo.value_i = 0;
            $scope.propertyInfo.value_f = 0.0;

            // 此属性归属的组件ID，暂不使用选择模式选择组件的属性
            $scope.propertyInfo.component_id = $scope.componentInfo.id;
            
            if ( $scope.propertyInfo.value_type == 'STR')
            {
                $scope.propertyInfo.value_type_name = '字符串';
                $scope.propertyInfo.value_s = $scope.propertyInfo.value;
            }
            else if ( $scope.propertyInfo.value_type == 'INT')
            {
                $scope.propertyInfo.value_type_name = '整数';
                $scope.propertyInfo.value_i = parseInt($scope.propertyInfo.value);
            }
            else if ( $scope.propertyInfo.value_type == 'FLT')
            {
                $scope.propertyInfo.value_type_name = '小数';
                $scope.propertyInfo.value_f = parseFloat($scope.propertyInfo.value);
            }

            $scope.itemInfo = $scope.propertyInfo;

        }

        var modalName = "";
        if ( type == 'component')
            modalName = "componentManage";
        else if ( type == 'property')
            modalName = "propertyManage";
        
        var CommonServiceFunc = null;
        if ( $scope.editMode == "新增")
            CommonServiceFunc = CommonService.createOne;
        else
            CommonServiceFunc = CommonService.updateOne;

        var uriData = angular.toJson( $scope.itemInfo );

        CommonServiceFunc( modalName, uriData, 
            function(data)
            {
                $("#addPage").modal("hide");

                // 保存成功后重新加载
                $scope.showPropertyInfo = false;

                if ( $scope.propertyMode = "编辑" && $scope.editMode == "新增")
                {
                    // 重新获取属性列表数据
                    $scope.selectComponent($scope.componentInfo);
                }
                else
                {
                    // 重新获取组件列表
                    $scope.initialData();
                }
            },
            function(data, status)
            {
                onErrorResult(data);
                $("#addPage").modal("hide");
            }
        );
    }


    $scope.showDeleteComponentDlg = function(item)
    {
        MsgBox.confirm("确认", "确认要删除组件 " + item.name + " 吗？", 
            function ( isConformed ) 
            {
                if (isConformed) 
                {
                    uriData = angular.toJson( item );
                    CommonService.deleteOne("componentManage", uriData, 
                        function(data)
                        {
                            for(var i = 0; i < $scope.componentList.length; i++)
                            {
                                if ( $scope.componentList[i].id == item.id )
                                    $scope.componentList.splice(i, 1);
                            }

                            MsgBox.info("提示", "删除组件 " + item.name + " 成功！");
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

    // 显示属性信息
    $scope.editPropertyInfo = function(item)
    {
        $scope.propertyInfo     = item;
        $scope.showPropertyInfo = true;
        $scope.propertyMode     = '编辑';
        $scope.editMode         = "更新";

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
                },
                function(data, status)
                {
                    onErrorResult(data);
                }
            );
        }
    }

    // 单击属性时，不进行编辑，也不显示编辑界面
    $scope.selectProperty = function(item)
    {
        if ( item == undefined || item.id == 0 )
            return;

        $scope.showPropertyInfo = true;
        $scope.propertyInfo     = item;
        $scope.propertyMode     = '查看';
    }

    $scope.showDeletePropertyDlg = function(item)
    {
        MsgBox.confirm("确认", "确认要删除属性 " + item.name + " 吗？", 
            function ( isConformed ) 
            {
                if (isConformed) 
                {
                    item.component_id = $scope.componentInfo.id;
                    uriData = angular.toJson( item );
                    CommonService.deleteOne("propertyManage", uriData, 
                        function(data)
                        {
                            for(var i = 0; i < $scope.propertyList.length; i++)
                            {
                                if ( $scope.propertyList[i].id == item.id )
                                    $scope.propertyList.splice(i, 1);
                            }

                            MsgBox.info("提示", "删除属性 " + item.name + " 成功！");
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
    $scope.page1 = function(e)
    {
        $scope.pager1.onEvent(e);
    };
    
    $scope.page2 = function(e)
    {
        $scope.pager2.onEvent(e);
    };

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
