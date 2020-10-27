////////////////////////////////////////////////////////////////////////////////////////////

angular.module('oaBulletinBoardControllers',['treeControl'])

//////////////////////////////////////////////////////////////////////////////////////////////
// 公司公告牌
.controller("oaBulletinBoardCtrl", function ($scope, $location, $rootScope, CommonService, $route)
{
    CommonService.heartbeat();
    $scope.user = null;
    if(sessionStorage.getItem("strUser"))
        $scope.user = JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"

    // 该用户是否为管理员
    $scope.isAdmin = false;
    for(i = 0; i < $scope.user.groupList.length; i++)
    {
        // 用户组所在的用户组中是否有admin字样，这个方法为临时方法
        groupName = $scope.user.groupList[i][1].toUpperCase();
        if ( groupName.indexOf("ADMIN") >= 0 )
            $scope.isAdmin = true;
    }

    $scope.AppSystemUserInfo = JSON.parse(sessionStorage.getItem('AppSystemUserInfo'));

    // 初始化
    $scope.pager = new Pager(null);
    $scope.pager.pageIndex  = 1;
    $scope.pager.pageSize   = 20;
    $scope.count            = 0;
    $scope.totalPage        = 0;
    $scope.pager.update(0);

    $scope.startDate        = "";
    $scope.endDate          = "";
    $scope.bulletinContent  = "";


    // 下拉列表准备
    $scope.initialListCtrl = function()
    {
    }
    $scope.initialListCtrl();

    $scope.resetTime = function( index )
    {
        if ( index == 0 )
            $scope.startDate = "";
        else if ( index == 1)
            $scope.endDate = "";

        $scope.searchBulletin();
    }
    $scope.searchBulletin = function()
    {
        var uriData = "op=data&o=1&r=20&status=checked";

        if ($scope.startDate != "" )
            uriData += "&sd=" + $scope.startDate;

        if ($scope.endDate != "" )
            uriData += "&ed=" + $scope.endDate;

        if($scope.bulletinContent != "" )
            uriData += "&bc=" + $scope.bulletinContent;

        $scope.initialData(uriData);
        
    }

    $scope.initialData = function( uriData )
    {
        if ( uriData == "" || uriData == undefined )
        {
            uriData = "op=data&o=1&r=20&status=checked";
        }

        CommonService.getAll("BulletinBoardManage", uriData, 
            function(data)
            {
                $scope.bulletinList = listToObject( data, 'rows').rows;
                $scope.count        = data.count;
                $scope.totalPage    = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
                $scope.pager.update(data.count);

                for ( i = 0; i < $scope.bulletinList.length; i++)
                    $scope.bulletinList[i].no = i + 1;
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

    // 查看公告
    $scope.showBulletin = function(item)
    {
        sessionStorage.setItem("BulletinBoardInfo", JSON.stringify(item));
        window.location.href = '#/oaViewBulletinBoard';
    }

    // 管理公告
    $scope.adminBulletin = function()
    {
        window.location.href = '#/BulletinBoardManage';
    }

})

//////////////////////////////////////////////////////////////////////////////////////////////
// 查看公司公告牌
.controller("oaViewBulletinBoardCtrl", function ($scope, $location, $rootScope, CommonService, $route)
{
    CommonService.heartbeat();
    $scope.user = null;
    if(sessionStorage.getItem("strUser"))
        $scope.user=JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"

    $scope.AppSystemUserInfo = JSON.parse(sessionStorage.getItem('AppSystemUserInfo'));

    // 初始化
    $scope.pager = new Pager(null);
    $scope.pager.pageIndex  = 1;
    $scope.pager.pageSize   = 20;
    $scope.count            = 0;
    $scope.totalPage        = 0;
    $scope.pager.update(0);

    $scope.goback = function()
    {
        history.back();
    }

    if ( !sessionStorage.getItem("BulletinBoardInfo")  )
    {
        // 错误
        MsgBox.info("数据错误：找不到公告发文！");
        $scope.goback();
    }

    $scope.BulletinBoardInfo = JSON.parse( sessionStorage.getItem("BulletinBoardInfo") );

    // 在离开页面的时候终止定时器
    $scope.$on('$destroy', function()
    {
        // 删除缓存，以避免被其他页面使用
        sessionStorage.removeItem("BulletinBoardInfo");
    });

})

//////////////////////////////////////////////////////////////////////////////////////////////
// 公司公告牌管理
.controller("BulletinBoardManageCtrl", function ($scope, $location, $rootScope, CommonService, $route)
{
    CommonService.heartbeat();
    $scope.user = null;
    if(sessionStorage.getItem("strUser"))
        $scope.user = JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"

    $scope.AppSystemUserInfo = JSON.parse(sessionStorage.getItem('AppSystemUserInfo'));

    // 该用户是否为管理员
    $scope.isAdmin = false;
    for(i = 0; i < $scope.user.groupList.length; i++)
    {
        // 用户组所在的用户组中是否有admin字样，这个方法为临时方法
        groupName = $scope.user.groupList[i][1].toUpperCase();
        if ( groupName.indexOf("ADMIN") >= 0 )
            $scope.isAdmin = true;
    }

    $scope.goback = function()
    {
        history.back();
    }

    if ( $scope.isAdmin == false )
    {
        // $scope.goback();
        // return;
    }

    // 初始化
    $scope.pager = new Pager(null);
    $scope.pager.pageIndex  = 1;
    $scope.pager.pageSize   = 20;
    $scope.count            = 0;
    $scope.totalPage        = 0;
    $scope.pager.update(0);

    $scope.startDate        = "";
    $scope.endDate          = "";
    $scope.bulletinContent  = "";


    // 下拉列表准备
    $scope.initialListCtrl = function()
    {
    }
    $scope.initialListCtrl();

    $scope.resetTime = function( index )
    {
        if ( index == 0 )
            $scope.startDate = "";
        else if ( index == 1)
            $scope.endDate = "";

        $scope.searchBulletin();
    }
    $scope.searchBulletin = function()
    {
        $scope.initialData();
       
    }

    $scope.initialData = function(  )
    {
        var uriData = "op=data&o=1&r=20&status=unchecked";

        if ($scope.startDate != "" )
            uriData += "&sd=" + $scope.startDate;

        if ($scope.endDate != "" )
            uriData += "&ed=" + $scope.endDate;

        if($scope.bulletinContent != "" )
            uriData += "&bc=" + $scope.bulletinContent;

        CommonService.getAll("BulletinBoardManage", uriData, 
            function(data)
            {
                $scope.bulletinList = listToObject( data, 'rows').rows;
                $scope.count        = data.count;
                $scope.totalPage    = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
                $scope.pager.update(data.count);

                getDataDict($scope.bulletinList, "status", "FLOW_STATUS", "status_name");

                for ( i = 0; i < $scope.bulletinList.length; i++)
                    $scope.bulletinList[i].no = i + 1;
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

    // 编辑或审核公告
    $scope.showBulletin = function(item)
    {
        sessionStorage.setItem("BulletinBoardInfo", JSON.stringify(item));
        window.location.href = '#/EditBulletinBoard';
    }

    // 发布公告
    $scope.PublishBulletin = function()
    {
        sessionStorage.removeItem("BulletinBoardInfo");
        window.location.href = '#/EditBulletinBoard';
    }

    $scope.showDeleteDlg = function(item)
    {
        if ( item.status == '5' || item.status == '2' )
        {
            MsgBox.info("提示", "公告已经审核通过，不能被删除！")
            return;
        }

        if ( item.status == '2' )
        {
            MsgBox.info("提示", "公告正在审核中！")
            return;
        }

        MsgBox.confirm("确认", "确定要删除公告 " + item.title + " 吗？", 
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

        CommonService.deleteOne("BulletinBoardManage", uriData, 
            function(data)
            {
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


//////////////////////////////////////////////////////////////////////////////////////////////
// 发布公告
// 公告有几种编辑状态：
// 1. 发布：为新建公告
// 2. 编辑：为编辑公告
// 
.controller("EditBulletinBoardCtrl", function ($scope, $location, $rootScope, CommonService, $route, FileUploader)
{
    CommonService.heartbeat();
    $scope.user = null;
    if(sessionStorage.getItem("strUser"))
        $scope.user=JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"

    $scope.AppSystemUserInfo = JSON.parse(sessionStorage.getItem('AppSystemUserInfo'));

    // 该用户是否为管理员
    $scope.isAdmin = false;
    if ( sessionStorage.getItem("isCheckBulltein")  )
    {
        $scope.isAdmin = true;
        sessionStorage.removeItem("isCheckBulltein");
    }

    // 初始化
    $scope.EditMode = "发布";
    $scope.BulletinBoardInfo = {};

    if ( !sessionStorage.getItem("BulletinBoardInfo")  )
    {
        $scope.EditMode = "发布";

        $scope.BulletinBoardInfo.publisher_id   = $scope.user.id;
        $scope.BulletinBoardInfo.publisher_name = $scope.user.name;
        $scope.BulletinBoardInfo.publisher_dept = $scope.user.dept_name;
        $scope.BulletinBoardInfo.publish_date   = getNowFormatDate1();
        
        $scope.BulletinBoardInfo.status = "1";  // 状态为未提交
        
        
        // 获取公告最新编号
        var uriData = {op: "code"};
        uriData = angular.toJson(uriData);

        CommonService.updatePartOne("BulletinBoardManage", uriData, 
            function(data)
            {
                // 公告编号格式： 年份（4位）(编号)，编号长度为3位，不足3位前面补0
                var code = "000" + data;
                code = code.substr( code.length - 3, 3);

                var date = new Date();

                $scope.BulletinBoardInfo.code = date.getFullYear() + "(" + code + ")";
            },
            function(data)
            {
                onErrorResult(data);
            }
        );
    }
    else
    {
        $scope.BulletinBoardInfo = JSON.parse( sessionStorage.getItem("BulletinBoardInfo") );
        $scope.EditMode = "编辑";
    }

    $scope.goback = function()
    {
        history.back();
    }


    // 保存公告信息
    $scope.saveBulletin = function( bShowResult)
    {
        if($("#title").val() == '')
        {
            $("#title").testRemind("请输入公告标题");
            return;
        }

        if($("#content").val() == '')
        {
            $("#content").testRemind("请输入公告内容");
            return;
        }

        if ( $scope.EditMode == "发布")
            $scope.saveBulletinInfo( CommonService.createOne, $scope.BulletinBoardInfo, bShowResult );
        else
            $scope.saveBulletinInfo( CommonService.updateOne, $scope.BulletinBoardInfo, bShowResult );
    }

    $scope.saveBulletinInfo = function( CommonServiceFunc, BulletinBoardInfo, bShowResult )
    {
        var uriData = angular.toJson( BulletinBoardInfo);
        CommonServiceFunc("BulletinBoardManage", uriData, 
            function(data)
            {
                if ( bShowResult )
                {
                    MsgBox.info("保存公告成功！");
                }

                $scope.goback();
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    // 附件上传
    var parameter = { "module" : "oa" };
    $scope.bulletinFileLoader = createFileUploader(FileUploader, parameter);

    $scope.previewBulletin = function()
    {

    }

    // 提交审核
    $scope.submitBulletin = function()
    {
        $scope.BulletinBoardInfo.status = '2'; // 状态修改为待审核
        $scope.BulletinBoardInfo.apply_date = getNowFormatDate2();

        $scope.saveBulletin(false);

    }

    // 审核申请
    $scope.CheckApplication = function()
    {
        $scope.checkResult  = "3";
        $scope.checkOpinion = "通过";

        $("#addPage").modal("show");
    }

    // 审核结果
    $scope.confirmApplication = function()
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

        $scope.BulletinBoardInfo.check_opinion  = $scope.checkOpinion;
        $scope.BulletinBoardInfo.check_date     = getNowFormatDate2();
        $scope.BulletinBoardInfo.checker_id     = $scope.user.id;

        $scope.BulletinBoardInfo.status = $scope.checkResult; // 修改状态;

        var uriData = angular.toJson( $scope.BulletinBoardInfo );
        CommonService.updateOne("BulletinBoardManage", uriData, 
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

