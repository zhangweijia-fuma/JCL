////////////////////////////////////////////////////////////////////////////////////////////

angular.module('organizationManageControllers', ['treeControl'])

// 组织团体管理首页
.controller('organizationManageCtrl', function($scope, $window, $rootScope, $filter, CommonService)
{
    var strUser = sessionStorage.getItem("strUser");

    if(strUser)
        $scope.user = JSON.parse(strUser);
    else
        window.location.href = "/login.html";

    $scope.pager = new Pager(null);
    $scope.pager.pageIndex = 1;
    $scope.pager.pageSize  = 20;

    $scope.organizationList         = [];
    $scope.allOrganizationList      = [];
    $scope.organizationTypeList     = [];

    $scope.selectedOrganizationID   = 0;
    $scope.selectedOrganizationTypeCode = '0';

    // 初始化下拉列表
    $scope.initListCtrl = function()
    {
        var uriData = { op: "list" };
        uriData = angular.toJson(uriData);

        CommonService.updatePartOne("organizationManage", uriData,
            function(data)
            {
                $scope.allOrganizationList    = listToObject( data.allOrganizationList, 'rows').rows;
                $scope.organizationTypeList   = listToObject( data.organizationTypeList, 'rows').rows;
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

        if ( $scope.allOrganizationList.selected )
            $scope.selectedOrganizationID = $scope.allOrganizationList.selected.id;

        if ( $scope.organizationTypeList.selected )
            $scope.selectedOrganizationTypeCode = $scope.organizationTypeList.selected.code;

        $scope.initialData();
    }

    $scope.initialData = function()
    {
        var uriData = "op=list&o=" + $scope.pager.pageIndex + "&r=" + $scope.pager.pageSize;
        if ( $scope.selectedOrganizationID > 0 )
            uriData += "&oid=" + $scope.selectedOrganizationID;

        if ( $scope.selectedOrganizationTypeCode != "0" )
            uriData += "&ot=" + $scope.selectedOrganizationTypeCode;

        CommonService.getAll("organizationManage", uriData, 
            function(data)
            {
                $scope.organizationList  = listToObject( data, 'rows').rows;
                $scope.count        = data.count;
                $scope.totalPage    = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
                $scope.pager.update(data.count);

                getDataDict($scope.organizationList, "organization_type", "ORGANIZATION_TYPE", "organization_type_name" );
                getDataDict($scope.organizationList, "legal_id_type", "IDENTITY_TYPE", "id_type_name" );

                for ( i = 0; i < $scope.organizationList.length; i++)
                {
                    $scope.organizationList[i].no = i + 1;
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

        CommonService.getAll("organizationManage", uriData, 
            function(data)
            {
                var filename = "组织团体查询" + getNowFormatDate2("", "", "-") + ".xls";
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

    $scope.createOrganization = function()
    {
        ClearAreaInfo("registe_addr");
        ClearAreaInfo("current_addr");

        window.location.href = '#/organizationEdit';
    }

    $scope.updateInfo = function( item )
    {
        // 更新时需要先准备地址信息
        ClearAreaInfo("registe_addr");
        ClearAreaInfo("current_addr");

        var addrList = [];
        addrList.push(item.registe_addr);
        addrList.push(item.current_addr);

        var uriData;

        uriData = { op: "area", addr: addrList };

        uriData = angular.toJson(uriData);
        CommonService.updatePartOne("areaCodeManage", uriData,
            function(data)
            {
                registeAddr = data.Address[0];
                SetAreaInfo("registe_addr", registeAddr);
                item.registe_addr_detail = registeAddr.detailAddr;
                
                currentAddr = data.Address[1];
                item.current_addr_detail = currentAddr.detailAddr;
                SetAreaInfo("current_addr", currentAddr);
                
                sessionStorage.setItem('organizationInfo', JSON.stringify(item));
                window.location.href = '#/organizationEdit';
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    $scope.showDeleteDlg = function(item)
    {
        // 检查供应商状态
        MsgBox.confirm("确认", "确定要删除 " + item.vendor_name + " 吗？", 
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

        CommonService.deleteOne("organizationManage", uriData, 
            function(data)
            {
                MsgBox.info("提示", "删除 " + item.vendor_name + " 成功！");
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


// 团体组织信息更新
.controller('organizationEditCtrl', function($scope, $window, $rootScope, $filter, CommonService, FileUploader)
{
    var strUser = sessionStorage.getItem("strUser");

    if(strUser)
        $scope.user = JSON.parse(strUser);
    else
        window.location.href = "/login.html";

    $scope.organizationInfo = { id : 0 };                                       // 供应商信息

    if ( !sessionStorage.getItem("organizationInfo"))
    {
        // 新增记录
        $scope.editMode = '新增';
    }
    else
    {
        $scope.editMode = "更新";
        $scope.organizationInfo = JSON.parse( sessionStorage.getItem("organizationInfo") );
        $scope.organizationInfo.logoFile = "./resource/images/logo/" + $scope.organizationInfo.logo_file;
    }

    $scope.goback = function()
    {
        history.back();
    }

    // 在离开页面的时候删除缓存的信息
    $scope.$on('$destroy', function()
    {
        sessionStorage.removeItem('organizationInfo');

        ClearAreaInfo("registe_addr");
        ClearAreaInfo("current_addr");
    });

    // 获取类型列表
    $scope.initListCtrl = function()
    {
        var uriData = { op: "list" };
        uriData = angular.toJson(uriData);

        CommonService.updatePartOne("organizationManage", uriData,
            function(data)
            {
                $scope.organizationTypeList = listToObject( data.organizationTypeList, 'rows').rows;
                $scope.organizationTypeList.shift();

                $scope.identifyTypeList = listToObject( data.identifyTypeList, 'rows').rows;
                $scope.identifyTypeList.shift();
                
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

    $scope.uploadFile = function()
    {
        var a = 1;
    }

    $scope.uploadError = false;

     // Logo上传 uploader 
    var uploader = $scope.uploader = new FileUploader({
        url:uploadURL,
        method: 'POST',
        headers: { Authorization: sessionStorage.getItem('authToken'), },
        autoUpload: true,
        formData: 
        [{ 
            'module'        : $scope.user.user_service, 
            'service_type'  : 'logo_file',
        }]
    });

    $scope.uploader.filters.push({
        name: 'customFilter',
        fn: function (item, options) 
        {
            return true;
        }
      });

    uploader.onAfterAddingAll = function(addedFileItems) 
    {
        $scope.isLoading = true;
    };
    
    uploader.onBeforeUploadItem = function(item) 
    {
        console.info('onBeforeUploadItem', item);
    };

    uploader.onErrorItem = function (fileItem, response, status, headers) 
    {
        $scope.uploadError = true;
    };

    uploader.onCompleteItem = function(fileItem, response, status, headers)
    {
        fileItem.response = response;

        $scope.organizationInfo.logo_file = response.file_name;
        $scope.organizationInfo.logoFile = "./resource/images/logo/" + $scope.organizationInfo.logo_file;
    };

    uploader.onCompleteAll = function()
    {
        $scope.isLoading = false;
        if ( $scope.uploadError )
        {
            MsgBox.info("提示", "文件上传失败！");
            $scope.uploadError = false;
            return;
        }
    };

    $scope.submitToCheck = function()
    {

        if($("#name").val() == '')
        {
            $("#name").testRemind("请输入中文名称");
            return;
        }

        if($("#name_en").val() == '')
        {
            $("#name_en").testRemind("请输入英文文名称");
            return;
        }

        if($("#abbr").val() == '')
        {
            $("#abbr").testRemind("请输入中文缩写");
            return;
        }

        if($("#abbr_en").val() == '')
        {
            $("#abbr_en").testRemind("请输入英文缩写");
            return;
        }

        if( $scope.organizationInfo.organization_type == null )
        {
            $("#organization_type").testRemind("请选择类型");
            return;
        }

        if($("#registe_date").val() == '')
        {
            $("#registe_date").testRemind("请选择注册日期");
            return;
        }

        if($("#registe_addr").val() == '')
        {
            $("#registe_addr").testRemind("请选择注册地址");
            return;
        }

        if($("#current_addr").val() == '')
        {
            $("#current_addr").testRemind("请选择当前地址");
            return;
        }

        if($("#business_scope").val() == '')
        {
            $("#business_scope").testRemind("请输入业务范围");
            return;
        }

        if($("#social_credit_code").val() == '')
        {
            $("#social_credit_code").testRemind("请输入社会信用代码");
            return;
        }

        if($("#legal_representative").val() == '')
        {
            $("#legal_representative").testRemind("请输入法人代表名称");
            return;
        }

        if($("#legal_id_type").val() == '')
        {
            $("#legal_id_type").testRemind("请选择证件类型");
            return;
        }

        if($("#legal_id_no").val() == '')
        {
            $("#legal_id_no").testRemind("请选择证件号码");
            return;
        }

        if($("#contactor_name").val() == '')
        {
            $("#contactor_name").testRemind("请输入联系人名称");
            return;
        }

        if($("#contactor_mobile").val() == '')
        {
            $("#contactor_mobile").testRemind("请输入联系人电话号码");
            return;
        }

        // 获取地址信息
        if ( $scope.organizationInfo.registe_addr_detail == null )
            $scope.organizationInfo.registe_addr_detail = "";

        $scope.organizationInfo.registe_addr = GetAreaInfo("registe_addr") + $scope.organizationInfo.registe_addr_detail;

        if ( $scope.organizationInfo.current_addr_detail == null )
            $scope.organizationInfo.current_addr_detail = "";

        $scope.organizationInfo.current_addr = GetAreaInfo("current_addr") + $scope.organizationInfo.current_addr_detail;

        var func = null;
        if ( $scope.editMode == "新增")
        {
            func = CommonService.createOne;
        }
        else if ( $scope.editMode=='更新')
        {
            func = CommonService.updateOne;
        }

        var uriData = angular.toJson($scope.organizationInfo);
        $scope.saveInfo( func, uriData);
    }

    $scope.saveInfo = function(CommonServiceFunc, uriData)
    {
        CommonServiceFunc( "organizationManage", uriData, 
            function(data)
            {
                MsgBox.info("提示", $scope.editMode + " " + $scope.organizationInfo.name + " 成功！");

                ClearAreaInfo("registe_addr");
                ClearAreaInfo("current_addr");

                $scope.goback();
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
})

