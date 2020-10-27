////////////////////////////////////////////////////////////////////////////////////////////

angular.module('propertyManageControllers',['treeControl'])

// 属性管理首页
.controller('propertyManageCtrl', function($scope, $window, $rootScope, $filter, CommonService)
{
    var strUser = sessionStorage.getItem("strUser");

    if(strUser)
        $scope.user = JSON.parse(strUser);
    else
        window.location.href = "/login.html";

    $scope.pager = new Pager(null);
    $scope.pager.pageIndex = 1;
    $scope.pager.pageSize  = 20;

    $scope.propertyList = [];

    $scope.allItemList  = [];
    $scope.typeList     = [];

    $scope.selectedItemID   = 0;
    $scope.selectedTypeCode = '0';
    $scope.selectTypeList   = [];

    $scope.editItem     = {};                                           // 新建属性

    // 初始化下拉列表
    $scope.initListCtrl = function()
    {
        var uriData = { op: "list" };
        uriData = angular.toJson(uriData);

        CommonService.updatePartOne("propertyManage", uriData,
            function(data)
            {
                $scope.allItemList  = listToObject( data.allItemList, 'rows').rows;
                $scope.typeList     = listToObject( data.typeList, 'rows').rows;
                $scope.selectList   = listToObject( data.selectList, 'rows').rows;
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
    $scope.initListCtrl();

    $scope.searchItem = function()
    {
        $scope.pager.pageIndex = 1;

        if ( $scope.allItemList.selected )
            $scope.selectedItemID = $scope.allItemList.selected.id;

        if ( $scope.typeList.selected )
            $scope.selectedTypeCode = $scope.typeList.selected.code;

        $scope.initialData();
    }

    $scope.initialData = function()
    {
        var uriData = "op=list&o=" + $scope.pager.pageIndex + "&r=" + $scope.pager.pageSize;
        if ( $scope.selectedItemID > 0 )
            uriData += "&itemid=" + $scope.selectedItemID;

        if ( $scope.selectedTypeCode != "0" )
            uriData += "&type=" + $scope.selectedTypeCode;

        CommonService.getAll("propertyManage", uriData, 
            function(data)
            {
                $scope.itemList     = listToObject( data, 'rows').rows;
                $scope.count        = data.count;
                $scope.totalPage    = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
                $scope.pager.update(data.count);

                getDataDict($scope.itemList, "certifaction_status", "VENDOR_COOPERATION_STATUS", "status_name" );
                getDataDict($scope.itemList, "vendor_type", "VENDOR_TYPE", "vendor_type_name" );

                for ( i = 0; i < $scope.itemList.length; i++)
                {
                    $scope.itemList[i].no = i + 1;
                }
            },
            function(data)
            {
                onErrorResult(data);

                $scope.count        = 0;
                $scope.totalPage    = 0;
                $scope.pager.update(0);
            }
        );
    }

    $scope.exportToFile = function()
    {
        var uriData = "op=excel";
        if ( $scope.selectedVendorID > 0 )
            uriData += "&vid=" + $scope.selectedVendorID;

        if ( $scope.selectedVendorStatusCode != "0" )
            uriData += "&vs=" + $scope.selectedVendorStatusCode;
        
        if ( $scope.selectedVendorTypeCode != "0" )
            uriData += "&vt=" + $scope.selectedVendorTypeCode;

        CommonService.getAll("vendorManage", uriData, 
            function(data)
            {
                var filename = "供应商查询" + getNowFormatDate2("", "", "-") + ".xls";
                var url = location.origin + data.url;
                var cmd = "<a download=" + filename + " href=" + url + "></a>"; 
                $( cmd )[0].click();
            },
            function(data,status)
            {
                // 失败时的回调函数
                onErrorResult(data);
            }
        );
    }

    $scope.page = function(e)
    {
        $scope.pager.onEvent(e);
        $scope.initialData();
    };

    $scope.createItem = function()
    {
        $scope.editMode = '新增';
        $scope.editItem.value_type = 'STR';
        $("#addPage").modal("show");
    }

    $scope.updateItemInfo = function( item )
    {
        $scope.editMode = '更新';
        $scope.editItem = {
            id          : item.id,
            name        : item.name,
            name_en     : item.name_en,
            value_type  : item.value_type,
            value_s     : item.value_s,
            value_i     : item.value_i,
            value_f     : item.value_f,
            desc        : item.desc, 
        }
        $("#addPage").modal("show");
    }

    $scope.confirmToSave = function()
    {
        if($("#name").val() == '')
        {
            $("#name").testRemind("请输入属性中文名称");
            return;
        }

        if($("#name_en").val() == '')
        {
            $("#name_en").testRemind("请输入属性英文文名称");
            return;
        }

        if( $scope.editItem.value_type == '' )
        {
            $("#value_type").testRemind("请选择属性类型");
            return;
        }

        if($scope.editItem.value_type == 'STR' && $('#value_s').val() == '')
        {
            $("#value_s").testRemind("请输入属性值");
            return;
        }

        if($scope.editItem.value_type == 'INT' && $('#value_i').val() == '')
        {
            $("#value_i").testRemind("请输入属性值");
            return;
        }

        if($scope.editItem.value_type == 'FLT' && $('#value_f').val() == '')
        {
            $("#value_f").testRemind("请输入属性值");
            return;
        }

        var uriData = angular.toJson($scope.editItem);
        if ( $scope.editMode == "新增")
        {
            $scope.saveInfo( CommonService.createOne, uriData);
        }
        else if ( $scope.editMode=='更新')
        { 
            $scope.saveInfo( CommonService.updateOne, uriData);
        }
    }

    $scope.saveInfo = function(CommonServiceFunc, uriData)
    {
        CommonServiceFunc( "propertyManage", uriData, 
            function(data)
            {
                MsgBox.info("提示", $scope.editMode + " " + $scope.editItem.name + " 成功！");

                $("#addPage").modal("hide");
                $scope.initialData();
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }


    $scope.showDeleteItemDlg = function(item)
    {
        MsgBox.confirm("确认", "确定要删除属性 " + item.name + " 吗？", 
            function ( isConformed )
            {
                if (isConformed) 
                    $scope.deleteItemConfirmed( item );
            }
        );
    }

    $scope.deleteItemConfirmed = function( item )
    {
        uriData = angular.toJson(item);

        CommonService.deleteOne("propertyManage", uriData, 
            function(data)
            {
                MsgBox.info("提示", "删除属性 " + item.vendor_name + " 成功！");
                // 删除后重新请求数据
                $scope.initialData();
            },
            function(data, status )
            {
                onErrorResult(data);
            }
        );
    }
})


