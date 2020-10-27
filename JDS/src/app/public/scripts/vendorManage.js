////////////////////////////////////////////////////////////////////////////////////////////

angular.module('vendorManageControllers',['treeControl'])

// 供应商管理首页
.controller('vendorManageCtrl', function($scope, $window, $rootScope, $filter, CommonService)
{
    var strUser = sessionStorage.getItem("strUser");

    if(strUser)
        $scope.user = JSON.parse(strUser);
    else
        window.location.href = "/login.html";

    $scope.pager = new Pager(null);
    $scope.pager.pageIndex = 1;
    $scope.pager.pageSize  = 20;

    $scope.vendorList               = [];

    $scope.allVendorList            = [];
    $scope.statusTypeList           = [];
    $scope.vendorTypeList           = [];

    $scope.selectedVendorID         = 0;
    $scope.selectedVendorStatusCode = '0';
    $scope.selectedVendorTypeCode   = '0';

    // 初始化下拉列表
    $scope.initListCtrl = function()
    {
        var uriData = { op: "list" };
        uriData = angular.toJson(uriData);

        CommonService.updatePartOne("vendorManage", uriData,
            function(data)
            {
                $scope.allVendorList    = listToObject( data.allVendorList, 'rows').rows;
                $scope.statusTypeList   = listToObject( data.statusTypeList, 'rows').rows;
                $scope.vendorTypeList   = listToObject( data.vendorTypeList, 'rows').rows;
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
    $scope.initListCtrl();

    $scope.searchVendor = function()
    {
        $scope.pager.pageIndex = 1;

        if ( $scope.allVendorList.selected )
            $scope.selectedVendorID = $scope.allVendorList.selected.id;

        if ( $scope.statusTypeList.selected )
            $scope.selectedVendorStatusCode = $scope.statusTypeList.selected.code;

        if ( $scope.vendorTypeList.selected )
            $scope.selectedVendorTypeCode = $scope.vendorTypeList.selected.code;

        $scope.initialData();
    }

    $scope.initialData = function()
    {
        var uriData = "op=list&o=" + $scope.pager.pageIndex + "&r=" + $scope.pager.pageSize;
        if ( $scope.selectedVendorID > 0 )
            uriData += "&vid=" + $scope.selectedVendorID;

        if ( $scope.selectedVendorStatusCode != "0" )
            uriData += "&vs=" + $scope.selectedVendorStatusCode;
        
        if ( $scope.selectedVendorTypeCode != "0" )
            uriData += "&vt=" + $scope.selectedVendorTypeCode;

        CommonService.getAll("vendorManage", uriData, 
            function(data)
            {
                $scope.vendorList  = listToObject( data, 'rows').rows;
                $scope.count        = data.count;
                $scope.totalPage    = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
                $scope.pager.update(data.count);

                getDataDict($scope.vendorList, "certifaction_status", "VENDOR_COOPERATION_STATUS", "status_name" );
                getDataDict($scope.vendorList, "vendor_type", "VENDOR_TYPE", "vendor_type_name" );

                for ( i = 0; i < $scope.vendorList.length; i++)
                {
                    $scope.vendorList[i].no = i + 1;
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

    $scope.createVendor = function()
    {
        sessionStorage.removeItem('vendorInfo');
        window.location.href = '#/vendorEdit';
    }

    $scope.updateVendorInfo = function( item )
    {
        sessionStorage.setItem('vendorInfo', JSON.stringify(item));
        window.location.href = '#/vendorEdit';

    }

    $scope.showDeleteVendorDlg = function(item)
    {
        // 检查供应商状态
        MsgBox.confirm("确认", "确定要删除供应商 " + item.vendor_name + " 吗？", 
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

        CommonService.deleteOne("vendorManage", uriData, 
            function(data)
            {
                MsgBox.info("提示", "删除供应商 " + item.vendor_name + " 成功！");
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


// 供应商信息更新
.controller('vendorEditCtrl', function($scope, $window, $rootScope, $filter, CommonService, FileUploader)
{
    var strUser = sessionStorage.getItem("strUser");

    if(strUser)
        $scope.user = JSON.parse(strUser);
    else
        window.location.href = "/login.html";

    $scope.vendorInfo = { vendor_id : 0 };                                       // 供应商信息

    if ( !sessionStorage.getItem("vendorInfo"))
    {
        // 新增记录
        $scope.editMode                 = '新增';
        $scope.vendorInfo.status_name   = '启动认证';
    }
    else
    {
        $scope.vendorInfo = JSON.parse( sessionStorage.getItem("vendorInfo") );
        if ( $scope.vendorInfo.certifaction_status == 'CA' )
            $scope.editMode = '审批';
        else if ($scope.vendorInfo.certifaction_status == 'CI')
            $scope.editMode = '更新';
        else
            $scope.editMode = '浏览';

        // $scope.address = $scope.vendorInfo.vendor_addr;
    }

    $scope.goback = function()
    {
        history.back();
    }
    
    $scope.initListCtrl = function()
    {
        var uriData = { op: "list" };
        uriData = angular.toJson(uriData);

        CommonService.updatePartOne("vendorManage", uriData,
            function(data)
            {
                $scope.organizationList = listToObject( data.organizationList, 'rows').rows;
                $scope.organizationList.shift();

                $scope.vendorTypeList   = listToObject( data.vendorTypeList, 'rows').rows;
                $scope.vendorTypeList.shift();
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
    $scope.initListCtrl();

    // 
    $scope.initialData = function()
    {
    }

    $scope.chooseVendor = function()
    {
        if ( $scope.vendorInfo.vendor_id == 0 )
            return;

        var uriData = { op: "orginfo", vid: $scope.vendorInfo.vendor_id};
        uriData = angular.toJson(uriData);

        CommonService.updatePartOne("vendorManage", uriData,
            function(data)
            {
                $scope.organizationList = listToObject( data.organizationList, 'rows').rows;
                getDataDict($scope.organizationList, "organization_type", "ORGANIZATION_TYPE", "organization_type_name");

                var orgInfo = $scope.organizationList[0];

                $scope.vendorInfo.vendor_id         = orgInfo.id;
                $scope.vendorInfo.name              = orgInfo.name;
                $scope.vendorInfo.name_en           = orgInfo.name_en;
                $scope.vendorInfo.abbr              = orgInfo.abbr;
                $scope.vendorInfo.abbr_en           = orgInfo.abbr_en;
                $scope.vendorInfo.business_scope    = orgInfo.business_scope;
                $scope.vendorInfo.registe_addr      = orgInfo.registe_addr;
                $scope.vendorInfo.current_addr      = orgInfo.current_addr;
                $scope.vendorInfo.contactor_name    = orgInfo.contactor_name;
                $scope.vendorInfo.contactor_mobile  = orgInfo.contactor_mobile;
                $scope.vendorInfo.description       = orgInfo.description;
                $scope.vendorInfo.system_user_id    = orgInfo.system_user_id;
                
                $scope.vendorInfo.organization_type_name = orgInfo.organization_type_name;

            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    $scope.submitToCheck = function()
    {
        if($("#start_date").val() == '')
        {
            $("#start_date").testRemind("请选择合作起始日期");
            return;
        }

        if($("#end_date").val() == '')
        {
            $("#end_date").testRemind("请输入合作结束日期");
            return;
        }

        var func;
        if ( $scope.editMode == "新增")
        {
            // 提交审批
            $scope.vendorInfo.certifaction_status = 'CA';
            func = CommonService.createOne;
        }
        else if ( $scope.editMode=='更新')
        {
            // 提交审批
            $scope.vendorInfo.certifaction_status = 'CA';
            func = CommonService.updateOne;
        }

        var uriData = angular.toJson($scope.vendorInfo);
        $scope.saveInfo( func, uriData);
    }

    $scope.saveInfo = function(CommonServiceFunc, uriData)
    {
        CommonServiceFunc( "vendorManage", uriData, 
            function(data)
            {
                MsgBox.info("提示", $scope.editMode + " " + $scope.vendorInfo.name + " 成功！");

                $("#addPage").modal("hide");
                $scope.goback();
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    // 审核供应商
    $scope.showCheckDlg = function()
    {
        $scope.checkResult  = "pass";
        $scope.checkOpinion = "通过";

        $("#addPage").modal("show");
    }


    // 审核供应商结果
    $scope.confirmCheckList = function()
    {
        if ( $scope.checkResult == "" ) 
        {
            $("#result").testRemind("请选择审批结果");
            return;
        }

        if ( $("#opinion").val() == "" ) 
        {
            $("#opinion").testRemind("请输入审批意见");
            return;
        }

        $("#addPage").modal("hide");

        var uriData = { id : $scope.vendorInfo.id, op:"check", result: $scope.checkResult, opinion: $scope.checkOpinion, vendor_name: $scope.vendorInfo.name };
        uriData = angular.toJson( uriData);
        CommonService.updatePartOne("vendorManage", uriData, 
            function(data)
            {
                // 保存成功后返回
                MsgBox.info("提示", "审核成功！");
                $scope.goback();
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
})

