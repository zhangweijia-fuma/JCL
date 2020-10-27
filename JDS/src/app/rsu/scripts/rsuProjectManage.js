////////////////////////////////////////////////////////////////////////////////////////////

angular.module('rsuProjectManageControllers', ['treeControl'])

//////////////////////////////////////////////////////////////////////////////////////////////
// RSU设备管理
.controller("rsuProjectManageCtrl", function ($scope, $location, $rootScope, CommonService, $route, FileUploader)
{
    CommonService.heartbeat();
    $scope.user = null;
    if(sessionStorage.getItem("strUser"))
        $scope.user=JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"

    $scope.pager = new Pager(null);
    $scope.pager.pageIndex  = 1;
    $scope.pager.pageSize   = 20;

    $scope.projectList      = [];
    $scope.projectNameList  = [];
    $scope.partAList        = [];
    $scope.partBList        = [];

    $scope.projectList      = [];
    $scope.organizationList = [];

    // 初始化下拉列表
    $scope.initListCtrl = function()
    {
        var uriData = { op: "data" };
        uriData = angular.toJson(uriData);

        CommonService.updatePartOne("rsuProjectManage", uriData,
            function(data)
            {
                $scope.projectNameList  = listToObject( data.projectList, 'rows').rows;
                $scope.partAList        = listToObject( data.partAList, 'rows').rows;
                $scope.partBList        = listToObject( data.partBList, 'rows').rows;
                $scope.organizationList = listToObject( data.organizationList, 'rows').rows;
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
    $scope.initListCtrl();

    $scope.searchProject = function()
    {
        $scope.pager.pageIndex = 1;
        $scope.initialData();
    }

    $scope.initialData = function()
    {
        var uriData = "op=data&o=" + $scope.pager.pageIndex + "&r=" + $scope.pager.pageSize;
        if ( $scope.projectList.selected && $scope.projectList.selected.id > 0 )
            uriData += "&pid=" + $scope.projectList.selected.id;

        if ( $scope.partAList.selected && $scope.partAList.selected.name != "选择全部" )
            uriData += "&pa=" + $scope.partAList.selected.id;
        
        if ( $scope.partBList.selected && $scope.partBList.selected.name != "选择全部")
            uriData += "&pb=" + $scope.partBList.selected.id;

        CommonService.getAll("rsuProjectManage", uriData, 
            function(data)
            {
                $scope.projectList  = listToObject( data, 'rows').rows;
                $scope.count        = data.count;
                $scope.totalPage    = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
                $scope.pager.update(data.count);

                for ( i = 0; i < $scope.projectList.length; i++)
                    $scope.projectList[i].no = i + 1;
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
        var uriData = "op=excel&o=" + $scope.pager.pageIndex + "&r=" + $scope.pager.pageSize;
        if ( $scope.projectList.selected && $scope.projectList.selected.id > 0 )
            uriData += "&pid=" + $scope.projectList.selected.id;

        if ( $scope.partAList.selected && $scope.partAList.selected.name != "选择全部" )
            uriData += "&pa=" + $scope.partAList.selected.id;
        
        if ( $scope.partBList.selected && $scope.partBList.selected.name != "选择全部")
            uriData += "&pb=" + $scope.partBList.selected.id;

        CommonService.getAll('rsuProjectManage', uriData,
            function(data)
            {
                var filename = "RSU项目查询" + getNowFormatDate2("", "", "-") + ".xlsx";
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

    $scope.createProject = function()
    {
        $scope.showProjectInfo = {};
        $scope.editMode = "新建";

        $("#addPage").modal("show");
    }

    $scope.updateInfo = function( item )
    {
        $scope.editMode = "更新";
        $scope.showProjectInfo = item;
        $("#addPage").modal("show");
    }

    $scope.confirmToSaveInfo = function()
    {
        // 检查参数是否合法
        if($("#name").val() == '')
        {
            $("#name").testRemind("请输入合同名称");
            return;
        }

        if($("#amount").val() == '')
        {
            $("#amount").testRemind("请输入合同金额");
            return;
        }

        if ( $scope.showProjectInfo.parta_id == undefined ) 
        {
            $("#parta_id").testRemind("请选择合同甲方");
            return;
        }

        if ( $("#parta_code").val() == "" ) 
        {
            $("#parta_code").testRemind("请输入甲方合同编号");
            return;
        }

        if($("#parta_man_name").val() == '')
        {
            $("#parta_man_name").testRemind("请输入甲方签署人名称");
            return;
        }

        if($("#parta_title").val() == '')
        {
            $("#parta_title").testRemind("请输入甲方签署人职务");
            return;
        }

        if ( $scope.showProjectInfo.partb_id == undefined ) 
        {
            $("#partb_id").testRemind("请选择合同乙方");
            return;
        }
        
        if($("#partb_code").val() == '')
        {
            $("#partb_code").testRemind("请输入乙方合同编号");
            return;
        }

        if($("#partb_man_name").val() == '')
        {
            $("#partb_man_name").testRemind("请输入乙方签署人名称");
            return;
        }

        if($("#partb_title").val() == '')
        {
            $("#partb_title").testRemind("请输入乙方签署人职务");
            return;
        }

        $("#addPage").modal("hide");

        if ( $scope.editMode == "新建")
            $scope.saveInfo( CommonService.createOne, $scope.showProjectInfo );
        else if ( $scope.editMode == "更新")
        {
            $scope.showProjectInfo.op = "modify";
            $scope.saveInfo( CommonService.updateOne, $scope.showProjectInfo );
        }

    }

    $scope.saveInfo = function( CommonServiceFunc, item )
    {
        var uriData = angular.toJson( item);
        CommonServiceFunc("rsuProjectManage", uriData, 
            function(data)
            {
                // 保存成功后重新加载数据
                $scope.initialData();
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    $scope.showDeleteDlg = function(item)
    {
        MsgBox.confirm("确认", "确定要删除合同 " + item.project_name + " 吗？", 
            function ( isConformed ) 
            {
                if (isConformed) 
                    $scope.deleteConfirmed( item );
            }
        );
    }

    $scope.deleteConfirmed = function( item )
    {
        uriData = angular.toJson(item);

        CommonService.deleteOne("rsuProjectManage", uriData, 
            function(data)
            {
                // 删除后重新请求数据
                MsgBox.info("删除成功！");
                $scope.initialData();
            },
            function(data, status )
            {
                onErrorResult(data);
            }
        );
    }

})

