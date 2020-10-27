////////////////////////////////////////////////////////////////////////////////////////////

angular.module('programManageControllers',['treeControl'])

// 节目单管理首页
.controller('programManageCtrl', function($scope, $window, $rootScope, $filter, CommonService)
{
    var strUser = sessionStorage.getItem("strUser");

    if(strUser)
        $scope.user = JSON.parse(strUser);
    else
        window.location.href = "/login.html";

    $scope.pager = new Pager(null);
    $scope.pager.pageIndex = 1;
    $scope.pager.pageSize  = 20;

    $scope.selectedApplyerID        = 0;
    $scope.selectedValidDate        = "";
    $scope.selectedApplyStatusCode  = "0";
    $scope.selectedSendStatusCode   = "0";

    $scope.applyerList      = [];
    $scope.applyStatusList  = [];

    $scope.programList      = [];

    // 初始化下拉列表
    $scope.initListCtrl = function()
    {
        var uriData = { op: "list" };
        uriData = angular.toJson(uriData);

        CommonService.updatePartOne("programList", uriData,
            function(data)
            {
                $scope.applyerList         = listToObject( data.applyerList, 'rows').rows;
                $scope.applyStatusList     = listToObject( data.applyStatusList, 'rows').rows;
                $scope.sendStatusList     = listToObject( data.sendStatusList, 'rows').rows;
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
    $scope.initListCtrl();

    $scope.searchProgram = function()
    {
        $scope.pager.pageIndex = 1;

        if ( $scope.applyStatusList.selected )
            $scope.selectedApplyStatusCode = $scope.applyStatusList.selected.code;

        if ( $scope.applyerList.selected )
            $scope.selectedApplyerID = $scope.applyerList.selected.id;

        if ( $scope.sendStatusList.selected )
            $scope.selectedSendStatusCode = $scope.sendStatusList.selected.code;


        $scope.initialData();
    }


    $scope.resetTime = function()
    {
        $scope.selectedValidDate = "";

        $scope.pager.pageIndex = 1;
        $scope.initialData();   
    }

    $scope.initialData = function()
    {
        var uriData = "op=list&o=" + $scope.pager.pageIndex + "&r=" + $scope.pager.pageSize;
        if ( $scope.selectedApplyerID > 0 )
            uriData += "&aid=" + $scope.selectedApplyerID;

        if ( $scope.selectedValidDate != "" )
            uriData += "&vd=" + $scope.selectedValidDate;
        
        if ( $scope.selectedApplyStatusCode != "0" )
            uriData += "&as=" + $scope.selectedApplyStatusCode;
        
        if ( $scope.selectedSendStatusCode != "0" )
            uriData += "&ss=" + $scope.selectedSendStatusCode;

        CommonService.getAll("programList", uriData, 
            function(data)
            {
                $scope.programList  = listToObject( data, 'rows').rows;
                $scope.count        = data.count;
                $scope.totalPage    = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
                $scope.pager.update(data.count);

                getDataDict($scope.programList, "send_status", "PROGRAM_SEND_STATUS", "send_status_name" );
                getDataDict($scope.programList, "apply_status", "PROGRAM_APPLY_STATUS", "apply_status_name" );

                for ( i = 0; i < $scope.programList.length; i++)
                {
                    $scope.programList[i].no = i + 1;
                    if ( $scope.programList[i].send_status_name == null )
                        $scope.programList[i].send_status_name = "未下发";
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
        var uriData = "op=excel&o=" + $scope.pager.pageIndex + "&r=" + $scope.pager.pageSize;
        if ( $scope.selectedApplyerID > 0 )
            uriData += "&aid=" + $scope.selectedApplyerID;

        if ( $scope.selectedValidDate != "" )
            uriData += "&vd=" + $scope.selectedValidDate;
        
        if ( $scope.selectedApplyStatusCode != "0" )
            uriData += "&as=" + $scope.selectedApplyStatusCode;
        
        if ( $scope.selectedSendStatusCode != "0" )
            uriData += "&ss=" + $scope.selectedSendStatusCode;

        CommonService.getAll('programList', uriData,
            function(data)
            {
                var filename = "节目单查询" + getNowFormatDate2("", "", "-") + ".xls";
                var url = location.origin + data.url;
                var cmd = "<a download=" + filename + " href=" + url + "></a>"; 
                $( cmd )[0].click();
            },
            function(data,status)
            {//失败时的回调函数
                onErrorResult(data);
            }
        );
    }

    $scope.page = function(e)
    {
        $scope.pager.onEvent(e);
        $scope.initialData();
    };

    $scope.createProgramList = function()
    {
        $scope.showListInfo = {
            'name' : "节目单" + getNowFormatDate1('-'),
            'valid_date' : getNowFormatDate1('-'),
            'video_type' : '0',
        };
        $scope.listMode = "新建";

        $("#addPage").modal("show");
    }

    $scope.updateProgramList = function( item )
    {
        // 检查节目单状态，如果已经审核通过的节目单不能被修改
        if ( item.apply_status == '3' )
        {
            MsgBox.info("提示", "节目单已经审核通过，不能再进行修改！")
            return;
        }

        $scope.listMode = "更新";
        $scope.showListInfo = 
        {
            id : item.id,
            name : item.name,
            valid_date : item.valid_date,
        }
        $("#addPage").modal("show");
    }

    $scope.confirmToSaveListInfo = function()
    {
        // 检查参数是否合法
        if($("#list_name").val() == '')
        {
            $("#list_name").testRemind("请输入节目单名称");
            return;
        }

        if ( $("#list_valid_date").val() == "" ) 
        {
            $("#list_valid_date").testRemind("请输入节目单生效日期");
            return;
        }

        $("#addPage").modal("hide");

        if ( $scope.listMode == "新建")
            $scope.saveListInfo( CommonService.createOne, $scope.showListInfo );
        else if ( $scope.listMode == "更新")
        {
            $scope.showListInfo.op = "modify";
            $scope.saveListInfo( CommonService.updateOne, $scope.showListInfo );
        }
        else
        {
            $scope.showListInfo.op = "copy";
            $scope.saveListInfo( CommonService.updatePartOne, $scope.showListInfo );
        }
    }

    $scope.saveListInfo = function( CommonServiceFunc, listInfo )
    {
        var uriData = angular.toJson( listInfo);
        CommonServiceFunc("programList", uriData, 
            function(data)
            {
                // 保存成功后打开节目单
                item = { id: data };
                $scope.showProgramListDetail(item);

            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    $scope.copyProgramList = function(item)
    {
        $scope.listMode = "复制";
        $scope.showListInfo = 
        {
            id          : item.id,
            'name'      : "节目单" + getNowFormatDate1('-'),
            valid_date  : item.valid_date,
            original_name       : item.name,
            original_valid_date : item.valid_date,
            video_type  : item.video_type,
        }

        $("#addPage").modal("show");
    }

    $scope.showProgramListDetail = function(item)
    {
        sessionStorage.setItem( "listInfo", JSON.stringify(item));

        window.location.href = '#/program_detail';
    }

    $scope.showDeleteProgramListDlg = function(item)
    {
        // 检查节目单状态，如果已经审核通过的节目单不能被删除
        if ( item.apply_status == '3' )
        {
            MsgBox.info("提示", "节目单已经审核通过，不能被删除！")
            return;
        }

        MsgBox.confirm("确认", "确定要删除节目单 " + item.name + " 吗？", 
            function ( isConformed ) 
            {
                if (isConformed) 
                    $scope.deleteListConfirmed( item );
            }
        );
    }

    $scope.deleteListConfirmed = function( item )
    {
        uriData = angular.toJson(item);

        CommonService.deleteOne("programList", uriData, 
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


// 节目单详情
.controller('programDetailCtrl', function($scope, $window, $rootScope, $filter, CommonService, FileUploader)
{
    var strUser = sessionStorage.getItem("strUser");

    if(strUser)
        $scope.user = JSON.parse(strUser);
    else
        window.location.href = "/login.html";

    $scope.pager = new Pager(null);
    $scope.pager.pageIndex = 1;
    $scope.pager.pageSize  = 20;

    $scope.currentUnit = { id : 0};
    
    $scope.isLoading = false;

    // 当前节目栏的节目列表
    $scope.programInfoList = [];

    // 当前显示的节目单
    $scope.listInfo = {};
    if ( !sessionStorage.getItem("listInfo"))
    {
        history.back();
    }

    $scope.goback = function()
    {
        history.back();
    }
    
    $scope.listInfo = JSON.parse( sessionStorage.getItem("listInfo") );

    $scope.showSend = false;
    $scope.showCheck = false;
    $scope.showSendHistory = false;

    // 根据申请状态决定编辑模式
    $scope.checkStatus = function()
    {
        if ( $scope.listInfo.apply_status == '1' || $scope.listInfo.apply_status == '4' )
            $scope.editType = "edit";
        else if ( $scope.listInfo.apply_status == '2')
        {
             $scope.editType = "check";
             // 当前用户是否具有审核权限，只有在节目单审核组的用户才能给审核
             for ( i = 0; i < $scope.user.groupList.length; i++ )
                if ( $scope.user.groupList[i][1] == "listCheckGroup" )
                    $scope.showCheck = true;
        }
        else
        {
            if ( $scope.listInfo.send_status == '1' )
            {
                $scope.editType = "send";

                 // 当前用户是否具有发送权限，只有在节目单审核组的用户才能给发送节目单
                 for ( i = 0; i < $scope.user.groupList.length; i++ )
                    if ( $scope.user.groupList[i][1] == "listSendGroup" )
                        $scope.showSend = true;
            }
            else
            {
                // 已经下发
                $scope.editType = "over";
                $scope.showSend = false;
                $scope.showSendHistory = true;
            }
        }
    }

    var currentNode = {};
    $scope.updateNodes = function (flag) 
    {
        var treeObj = $.fn.zTree.getZTreeObj("program_tree");
        currentNode.highlight = flag;
        treeObj.updateNode(currentNode);
    }

    $scope.__apply = function () 
    {
        if (!$rootScope.$$phase)
            $scope.$digest();
    };

    // 树被点击
    $scope.onTreeClicked = function(event, treeId, treeNode) 
    {
        if ( treeNode == undefined )
            return;

        if ( treeNode.nodeType == "program_unit")
        {
            // 选择了一个节目栏
             $scope.currentUnit = 
             {
                id          : treeNode.id,
                code        : treeNode.code,
                name        : treeNode.show_name,
                start_time  : treeNode.start_time,
                end_time    : treeNode.end_time,
                list_id     : treeNode.list_id,
                list_name   : treeNode.list_name,
                sort        : treeNode.sort,
            }

            $scope.showUnit = true;
            $scope.getProgramInfoList();
            $scope.uploader.formData[0].uid = $scope.currentUnit.id;
            $scope.__apply();
        }
        else
        {
            // $scope.showProgram  = false;
            $scope.showUnit     = false;
            $scope.__apply();
        }

        $scope.updateNodes(false);
        currentNode = treeNode;
        $scope.updateNodes(true); 
    }

    var setting = 
    {
        check: { enable: false },
        data: { },
        key:{ name:'name', children:'children' },
        callback: { onClick: $scope.onTreeClicked },
        view: 
        {
            fontCss: function (treeId, treeNode) 
            {
                selectedItemColor = {color: "red", "font-weight": "bold"};
                nonSelectedItemColor = { color: "black", "font-weight": "normal", background: "white" };
                
                return (!!treeNode.highlight) ? selectedItemColor : nonSelectedItemColor;
            }
        }
    };

    // 获取节目单的节目栏及节目信息
    $scope.initialData = function()
    {
        // 重新获取线路信息
        var uriData = "id=" + $scope.listInfo.id;
        CommonService.getAll("programList", uriData, 
            function(data)
            {
                temp = listToObject(data, 'rows').rows;

                getDataDict( temp, "send_status", "PROGRAM_SEND_STATUS", "send_status_name" );
                getDataDict( temp, "apply_status", "PROGRAM_APPLY_STATUS", "apply_status_name" );

                $scope.listInfo = temp[0];
                $scope.videoType = $scope.listInfo.video_type;
                if ( $scope.videoType == '0')
                    $scope.listInfo.videoTypeName = '录播';
                else
                    $scope.listInfo.videoTypeName = '直播';

                $scope.checkStatus();
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );

        // 获取节目单的节目栏及节目信息
        uriData = { op: "tree", pid : $scope.listInfo.id };
        
        uriData = angular.toJson(uriData);
        $scope.loader = new Loader('sel_program_tree');
        CommonService.updatePartOne("programList", uriData, 
            function(data)
            {
                $scope.loader.close();
                $scope.treedata = data;
                $.fn.zTree.init($("#program_tree"), setting, $scope.treedata);

                // 选择第一个节点，第一个节点是节目栏
                if ( $scope.treedata.children.length > 0 )
                {
                    var treeObj = $.fn.zTree.getZTreeObj("program_tree");
                    currentNode = $scope.treedata.children[0];
                    if ( $scope.currentUnit.id > 0 )
                        currentNode = treeObj.getNodesByParamFuzzy("start_time", $scope.currentUnit.start_time, null)[0];
                    else
                        currentNode = treeObj.getNodesByParamFuzzy("name", currentNode.name, null)[0];
                        
                    $scope.onTreeClicked( null, null, currentNode);
                }

            },
            function(data, status)
            {
                $scope.loader.close();
                onErrorResult(data);
            }
        );

        // 获取节目单审批记录
        uriData = { op : "history", pid : $scope.listInfo.id };

        uriData = angular.toJson(uriData);
        CommonService.updatePartOne("programList", uriData, 
            function(data)
            {
                $scope.checkHistoryList = listToObject( data, 'rows').rows;

                getDataDict($scope.checkHistoryList, "check_result", "PROGRAM_APPLY_STATUS", "check_result_name");

                for ( i = 0; i < $scope.checkHistoryList.length; i++)
                    $scope.checkHistoryList[i].no = i + 1;

            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );


        $scope.getSendHistoryList();
    }

    $scope.chooseVideoType = function(type)
    {
        if ( $scope.editType !='edit')
            return;

        $scope.listInfo.video_type = type;
    }

    // 节目单发送记录
    $scope.getSendHistoryList = function()
    {
        // 获取节目单下发记录
        uriData = "op=data&optype=list&oi=" + $scope.listInfo.id + "&o=" + $scope.pager.pageIndex + "&r=" + $scope.pager.pageSize;
        uriData += "&dataType=current";

        CommonService.getAll("messageManage", uriData, 
            function(data)
            {
                $scope.sendHistoryList = listToObject( data, 'rows').rows;

                getDataDict($scope.sendHistoryList, "send_status", "CTRL_RECEIVE_STATUS", "send_status_name");
                getDataDict($scope.sendHistoryList, "message_type", "MSG_TYPE_CONTROLLER", "message_type_name");

                // 删除已经重发前的消息
                for ( i = 0; i < $scope.sendHistoryList.length; i++)
                {
                    if ($scope.sendHistoryList[i].message_sn == 0 )
                        $scope.sendHistoryList.splice(i, 0);
                }

                for ( i = 0; i < $scope.sendHistoryList.length; i++)
                    $scope.sendHistoryList[i].no = ($scope.pager.pageIndex - 1) * $scope.pager.pageSize + i + 1;

                $scope.count        = data.count;
                $scope.totalPage    = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
                $scope.pager.update(data.count);

            },
            function(data, status)
            {
                onErrorResult(data);

                $scope.count        = 0;
                $scope.totalPage    = 0;
                $scope.pager.update(0);
            }
        );
    }

    $scope.page = function(e)
    {
        $scope.pager.onEvent(e);
        $scope.getSendHistoryList();
    };


    $scope.getProgramInfoList = function()
    {
        uriData = "uid=" + $scope.currentUnit.id;
        CommonService.getAll("programInfo", uriData, 
            function(data)
            {
                $scope.programInfoList = listToObject( data, 'rows').rows;
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    $scope.uploadError = false;
    /* 节目上传 uploader */
    var uploader = $scope.uploader = new FileUploader({
        url:uploadURL,
        method: 'POST',
        headers: { Authorization: sessionStorage.getItem('authToken'), },
        autoUpload: true,
        formData: 
        [{ 
            'module' : 'PIS', 
            'service_type' : 'program_list', 
            'lid': $scope.listInfo.id,
        }]
    });

    $scope.uploader.filters.push({
        name: 'customFilter',
        fn: function (item, options) 
        {
            return true;
            // var suffix = item.name.substring(item.name.lastIndexOf(".") + 1, item.name.length);
            // return suffix === 'xls' || suffix === 'xlsx' ? true : false;
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
        $scope.program_unit_file = {
            file: response.file_path,
            type: response.file_type,
        };
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

        uriData = {
            op: "upload", 
            lid: $scope.listInfo.id, 
            uid: $scope.currentUnit.id,
            file: $scope.program_unit_file.file,
            type: $scope.program_unit_file.type,
        };

        uriData = angular.toJson(uriData);
        CommonService.updatePartOne("programUnit", uriData, 
            function(data)
            {
                // 更新后重新请求数据
                $scope.getProgramInfoList();
                MsgBox.info("提示", "文件上传成功！");
            },
            function(data, status )
            {
                onErrorResult(data);
            }
        );

    };

    $scope.changeProgramSort = function(item, direction)
    {
        item.direction = direction;
        uriData = angular.toJson(item);
        CommonService.updateOne("programInfo", uriData, 
            function(data)
            {
                // 更新后重新请求数据
                $scope.getProgramInfoList();
            },
            function(data, status )
            {
                onErrorResult(data);
            }
        );
    }

    $scope.showDeleteProgramFileDlg = function(item)
    {
        MsgBox.confirm("确认", "确定要删除节目 " + item.name + " 吗？", 
            function ( isConformed ) 
            {
                if (isConformed) 
                    $scope.deleteProgramFileConfirmed( item );
            }
        ); 
    }

    $scope.deleteProgramFileConfirmed = function( item )
    {
        uriData = angular.toJson(item);

        CommonService.deleteOne("programInfo", uriData, 
            function(data)
            {
                // 删除后重新请求数据
                $scope.getProgramInfoList();
            },
            function(data, status )
            {
                onErrorResult(data);
            }
        );
    }

    $scope.showDeleteAllProgramDlg = function()
    {
        MsgBox.confirm("确认", "确定要删除所有节目吗？", 
            function ( isConformed ) 
            {
                var item = { id:0, unit_id : $scope.currentUnit.id, list_id: $scope.currentUnit.list_id, };
                if (isConformed) 
                    $scope.deleteProgramFileConfirmed( item );
            }
        ); 
    }

    // 将节目栏同步到全天其他节目栏中
    $scope.syncUnit = function()
    {
        if ( !$scope.currentUnit || $scope.currentUnit.id == 0 )
        {
            MsgBox.info("提示", "没有选中的节目栏！");
            return;
        }

        var uriData = {lid : $scope.currentUnit.list_id,  uid: $scope.currentUnit.id, op:"sync"};
        uriData = angular.toJson( uriData);
        CommonService.updatePartOne("programUnit", uriData, 
            function(data)
            {
                MsgBox.info("提示", "同步节目栏成功！");
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    // 提交申请
    $scope.submitToCheck = function()
    {
        var uriData = { id : $scope.listInfo.id, video_type: $scope.listInfo.video_type, op:"submit" };
        uriData = angular.toJson( uriData);
        CommonService.updateOne("programList", uriData, 
            function(data)
            {
                // 保存成功后返回
                MsgBox.info("提示", "提交成功!");
                $scope.goback();
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    // 审核节目单
    $scope.showCheckDlg = function()
    {
        $scope.checkResult  = "3";
        $scope.checkOpinion = "通过";

        $("#addPage").modal("show");
    }

    // 审核节目单结果
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

        var uriData = { id : $scope.listInfo.id, op:"check", result: $scope.checkResult, opinion: $scope.checkOpinion };
        uriData = angular.toJson( uriData);
        CommonService.updateOne("programList", uriData, 
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

    // 发送节目单
    $scope.sendList = function()
    {
        sessionStorage.setItem("sendListInfo", JSON.stringify($scope.listInfo));
        window.location.href = "#/sendListToController";
    }

    // 针对某个控制器，重新发送节目单
    $scope.reSendList = function( item )
    {
        item.op = "resend";
        var uriData = angular.toJson( item );
        CommonService.updatePartOne("messageManage", uriData, 
            function(data)
            {
                MsgBox.info("提示", "操作成功！");
                $scope.pager.pageIndex = 1;
                $scope.getSendHistoryList();
            },
            function(data)
            {
                onErrorResult(data);
            }
        );

    }
})

// 节目管理页面
.controller('programFileManageCtrl', function($scope, $window, $rootScope, $filter, CommonService, FileUploader)
{
    var strUser = sessionStorage.getItem("strUser");

    if(strUser)
        $scope.user = JSON.parse(strUser);
    else
        window.location.href = "/login.html";

    $scope.pager = new Pager("pager");
    $scope.pager.pageIndex  = 1;
    $scope.pager.pageSize   = 20;

    $scope.pager2 = new Pager("pager2");
    $scope.pager2.pageIndex  = 1;
    $scope.pager2.pageSize   = 20;

    $scope.upload_time      = "";

    $scope.nameList         = [];
    $scope.uploaderList     = [];
    $scope.programFileList  = [];

    $scope.showVideo        = false;

    // 初始化下拉列表
    $scope.initListCtrl = function()
    {
        var uriData = { op : "list" };
        uriData = angular.toJson(uriData);

        CommonService.updatePartOne("programFile", uriData,
            function(data)
            {
                $scope.nameList         = listToObject( data.nameList, 'rows').rows;
                $scope.uploaderList     = listToObject( data.uploaderList, 'rows').rows;
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
        $scope.GetProgramFileList();
    }

    $scope.resetTime = function()
    {
        $scope.upload_time = "";

        $scope.pager.pageIndex = 1;
        $scope.GetProgramFileList();   
    }

    $scope.initialData = function()
    {
        $scope.GetProgramFileList();
        $scope.GetSafeProgramFileList();
    }

    $scope.GetProgramFileList = function()
    {
        var uriData = "op=list&type=1&o=" + $scope.pager.pageIndex + "&r=" + $scope.pager.pageSize;

        if ( $scope.nameList.selected && $scope.nameList.selected.id != 0 )
            uriData += "&fid=" + $scope.nameList.selected.id;

        if ( $scope.uploaderList.selected && $scope.uploaderList.selected.id != 0 )
            uriData += "&uid=" + $scope.uploaderList.selected.id;

        if ( $scope.upload_time != "" )
            uriData += "&ud=" + $scope.upload_time;

        CommonService.getAll("programFile", uriData, 
            function(data)
            {
                $scope.programFileList  = listToObject( data, 'rows').rows;
                $scope.count        = data.count;
                $scope.totalPage    = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
                $scope.pager.update(data.count);

                for( i = 0; i < $scope.programFileList.length; i++)
                    $scope.programFileList[i].no = i + 1 + ($scope.pager.pageIndex - 1) * $scope.pager.pageSize;

                if ( $scope.programFileList.length > 0 )
                {
                    $scope.showVideo = true;
                    $scope.previewFile( $scope.programFileList[0]);
                }
                else
                {
                    $scope.showVideo = false;
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

    $scope.GetSafeProgramFileList = function()
    {
        var uriData = "op=list&type=0&o=" + $scope.pager2.pageIndex + "&r=" + $scope.pager2.pageSize;

        CommonService.getAll("programFile", uriData, 
            function(data)
            {
                $scope.safeFileList = listToObject(data, 'rows').rows;
                $scope.count2        = data.count;
                $scope.totalPage2   = Math.ceil(parseInt(data.count) / $scope.pager2.pageSize);
                $scope.pager2.update(data.count);

                for( i = 0; i < $scope.safeFileList.length; i++)
                    $scope.safeFileList[i].no = i + 1 + ($scope.pager2.pageIndex - 1) * $scope.pager2.pageSize;

            },
            function(data)
            {
                onErrorResult(data);

                $scope.count2       = 0;
                $scope.totalPage2   = 0;
                $scope.pager2.update(0);
            }
        );
    }

    $scope.exportToFile = function()
    {
        var uriData = "op=excel";

        if ( $scope.nameList.selected && $scope.nameList.selected.id != 0 )
            uriData += "&fid=" + $scope.nameList.selected.id;

        if ( $scope.uploaderList.selected && $scope.uploaderList.selected.id != 0 )
            uriData += "&uid=" + $scope.uploaderList.selected.id;

        if ( $scope.upload_time != "" )
            uriData += "&ud=" + $scope.upload_time;

        CommonService.getAll('programFile', uriData,
            function(data)
            {
                var filename = "节目文件查询" + getNowFormatDate2("", "", "-") + ".xls";
                var url = location.origin + data.url;
                var cmd = "<a download=" + filename + " href=" + url + "></a>"; 
                $( cmd )[0].click();
            },
            function(data,status)
            {//失败时的回调函数
                onErrorResult(data);
            }
        );
    }


    $scope.page = function(e)
    {
        $scope.pager.onEvent(e);
        $scope.GetProgramFileList();
    };

    $scope.page2 = function(e)
    {
        $scope.pager.onEvent(e);
        $scope.GetSafeProgramFileList();
    };


    /* uploader */
    var uploader = $scope.uploader = new FileUploader({
        url:uploadURL,
        method: 'POST',
        headers: { Authorization: sessionStorage.getItem('authToken'), },
        autoUpload: true,
        formData: [{ 'module' : 'PIS', 'service_type' : 'program_file', 'video_type': '1'}]
    });

    uploader.filters.push({
        name: 'customFilter',
        fn: function(item /*{File|FileLikeObject}*/, options)
        {
            return true; // this.queue.length < 10;
        }
    });

    uploader.onAfterAddingAll = function(addedFileItems) 
    {
        $scope.isLoading = true;
        document.body.style.cursor = "wait";
    };
    
    uploader.onBeforeUploadItem = function(item) 
    {
        // console.info('onBeforeUploadItem', item);
    };

    uploader.onErrorItem = function (fileItem, response, status, headers) 
    {
        $scope.uploadError = true;
    };

    uploader.onCompleteItem = function(fileItem, response, status, headers)
    {
        fileItem.response = response;
    };

    uploader.onCompleteAll = function()
    {
        $scope.isLoading = false;
        document.body.style.cursor = "pointer";

        if ( $scope.uploadError )
        {
            MsgBox.info("提示", "文件上传失败！");
            $scope.uploadError = false;
            return;
        }

        MsgBox.info("提示", "文件上传成功！");

        // 重新加载数据
        $scope.pager.pageIndex = 1;
        $scope.GetProgramFileList();
        $scope.initListCtrl();
    };

    /* uploader */
    var safeFileUploader = $scope.safeFileUploader = new FileUploader({
        url:uploadURL,
        method: 'POST',
        headers: { Authorization: sessionStorage.getItem('authToken'), },
        autoUpload: true,
        formData: [{ 'module' : 'PIS', 'service_type' : 'program_file', 'video_type': '0'}]
    });

    safeFileUploader.filters.push({
        name: 'customFilter',
        fn: function(item /*{File|FileLikeObject}*/, options)
        {
            return true; // this.queue.length < 10;
        }
    });

    safeFileUploader.onAfterAddingAll = function(addedFileItems) 
    {
        $scope.isLoading = true;
        document.body.style.cursor = "wait";
    };
    
    safeFileUploader.onBeforeUploadItem = function(item) 
    {
        // console.info('onBeforeUploadItem', item);
    };

    safeFileUploader.onErrorItem = function (fileItem, response, status, headers) 
    {
        $scope.uploadError = true;
    };

    safeFileUploader.onCompleteItem = function(fileItem, response, status, headers)
    {
        fileItem.response = response;
    };

    safeFileUploader.onCompleteAll = function()
    {
        $scope.isLoading = false;
        document.body.style.cursor = "pointer";

        if ( $scope.uploadError )
        {
            MsgBox.info("提示", "文件上传失败！");
            $scope.uploadError = false;
            return;
        }

        MsgBox.info("提示", "文件上传成功！");

        // 重新加载数据
        $scope.pager2.pageIndex = 1;
        $scope.GetSafeProgramFileList();
    };


    $scope.showDeleteFormatFileDlg = function(item)
    {
        MsgBox.confirm("确认", "确定要删除节目文件 " + item.file_name + " 吗？", 
            function ( isConformed ) 
            {
                if (isConformed) 
                    $scope.deleteFormatFileConfirmed( item );
            }
        );
    }

    $scope.deleteFormatFileConfirmed = function( item )
    {
        uriData = angular.toJson(item);

        CommonService.deleteOne("programFile", uriData, 
            function(data)
            {
                // 删除后重新请求数据
                $scope.pager.pageIndex = 1;
                $scope.GetProgramFileList();
                $scope.initListCtrl();
            },
            function(data, status )
            {
                onErrorResult(data);
            }
        );
    }

    $scope.previewFile = function( item )
    {
        $scope.video_url = "/videofile/" + item.file_name;
    }

})


// 版式文件管理首页
.controller('formatFileManageCtrl',function($scope, $window, $rootScope, $filter, CommonService, FileUploader, $timeout)
{
    var strUser = sessionStorage.getItem("strUser");

    if(strUser)
        $scope.user = JSON.parse(strUser);
    else
        window.location.href = "/login.html";

    $scope.pager = new Pager("pager");
    $scope.pager.pageIndex = 1;
    $scope.pager.pageSize  = 20;

    $scope.pager2 = new Pager("pager2");
    $scope.pager2.pageIndex = 1;
    $scope.pager2.pageSize  = 20;

    $scope.start_time       = "";
    $scope.end_time         = "";

    $scope.nameList         = [];
    $scope.sendStatusList   = [];

    $scope.formatFileList   = [];

    // 默认显示消息记录，true为预览版式
    $scope.showType = false;
    $scope.currentFormatFile = { id: 0};
    $scope.chooseShowType = function( showType )
    {
        $scope.showType = showType;
        $scope.chooseAction( $scope.currentFormatFile );
    }

    // 初始化下拉列表
    $scope.initListCtrl = function()
    {
        var uriData = { op: "list" };
        uriData = angular.toJson(uriData);

        CommonService.updatePartOne("formatFileList", uriData,
            function(data)
            {
                $scope.nameList         = listToObject( data.nameList, 'rows').rows;
                // $scope.sendStatusList   = listToObject( data.sendStatusList, 'rows').rows;
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
    $scope.initListCtrl();

    $scope.searchFile = function()
    {
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
        var uriData = "op=list&o=" + $scope.pager.pageIndex + "&r=" + $scope.pager.pageSize;

        if ( $scope.nameList.selected && $scope.nameList.selected.id != 0 )
            uriData += "&fid=" + $scope.nameList.selected.id;

        if ( $scope.start_time != "" && $scope.end_time != "" )
            uriData += "&st=" + $scope.start_time + "&et=" + $scope.end_time;

        CommonService.getAll("formatFileList", uriData, 
            function(data)
            {
                $scope.formatFileList  = listToObject( data, 'rows').rows;
                $scope.count        = data.count;
                $scope.totalPage    = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
                $scope.pager.update(data.count);

                getDataDict($scope.formatFileList, "send_status", "PROGRAM_SEND_STATUS", "send_status_name" );

                for ( i = 0; i < $scope.formatFileList.length; i++)
                {
                    $scope.formatFileList[i].no = i + 1;
                    if ( $scope.formatFileList[i].send_status == undefined )
                    {
                        $scope.formatFileList[i].send_status_name = "未下发";
                        $scope.formatFileList[i].send_status = '1';
                    }
                }

                $scope.totalPage2   = 0;
                $scope.pager2.update(0);
                if ( $scope.formatFileList.length == 0 )
                {
                    $scope.currentFormatFile = { id : 0 };
                    $scope.chooseAction( $scope.currentFormatFile );
                }
                else
                {
                    $scope.chooseAction( $scope.formatFileList[0] );
                }

                // 设置一个定时器，以等页面显示后刷新已经选择的行的背景色
                $scope.timer = $timeout( $scope.showSelectedItem, 100);
            },
            function(data)
            {
                onErrorResult(data);

                $scope.count        = 0;
                $scope.totalPage    = 0;
                $scope.pager.update(0);

                $scope.totalPage2   = 0;
                $scope.pager2.update(0);
            }
        );
    }

    $scope.chooseAction = function( item )
    {
        $scope.currentFormatFile = item;
        $scope.showSelectedItem(item);

        if ($scope.showType )
            $scope.previewFormatFile();
        else
            $scope.getSendHistoryList();
    }

    $scope.exportToFile = function()
    {
        var uriData = "op=excel";

        if ( $scope.selectedName != "" )
            uriData += "&fn=" + $scope.selectedName;
        
        if ( $scope.selectedSendStatusCode != "0" )
            uriData += "&ss=" + $scope.selectedSendStatusCode;

        CommonService.getAll('formatFileList', uriData,
            function(data)
            {
                var filename = "版式文件查询" + getNowFormatDate2("", "", "-") + ".xls";
                var url = location.origin + data.url;
                var cmd = "<a download=" + filename + " href=" + url + "></a>"; 
                $( cmd )[0].click();
            },
            function(data,status)
            {//失败时的回调函数
                onErrorResult(data);
            }
        );
    }


    $scope.page = function(e)
    {
        $scope.pager.onEvent(e);
        $scope.initialData();
    };


    /* uploader */
    var uploader = $scope.uploader = new FileUploader({
        url:uploadURL,
        method: 'POST',
        headers: { Authorization: sessionStorage.getItem('authToken'), },
        autoUpload: true,
        formData: [{ 'module' : 'PIS', 'service_type' : 'format_file'}]
    });

    uploader.filters.push({
        name: 'customFilter',
        fn: function(item /*{File|FileLikeObject}*/, options)
        {
            return this.queue.length < 10;
        }
    });

    uploader.onCompleteItem = function(fileItem, response, status, headers)
    {
        fileItem.response = response;
    };
    
    $scope.uploadError = false;
    uploader.onErrorItem = function (fileItem, response, status, headers) 
    {
        $scope.uploadError = true;
    };

    uploader.onCompleteAll = function()
    {
        if ( $scope.uploadError )
        {
            MsgBox.info("提示", "文件上传失败！");
            $scope.uploadError = false;
            return;
        }

        MsgBox.info("提示", "文件上传成功！");

        // 重新加载数据
        $scope.pager.pageIndex = 1;
        $scope.initialData();
        $scope.initListCtrl();
    };


    $scope.showDeleteFormatFileDlg = function(item)
    {
        MsgBox.confirm("确认", "确定要删除版式文件 " + item.name + " 吗？", 
            function ( isConformed ) 
            {
                if (isConformed) 
                    $scope.deleteFormatFileConfirmed( item );
            }
        );
    }

    $scope.deleteFormatFileConfirmed = function( item )
    {
        uriData = angular.toJson(item);

        CommonService.deleteOne("formatFileList", uriData, 
            function(data)
            {
                // 删除后重新请求数据
                $scope.showPreview = false;

                $scope.pager.pageIndex = 1;
                $scope.initialData();
                $scope.initListCtrl();
            },
            function(data, status )
            {
                onErrorResult(data);
            }
        );
    }

    // 版式文件预览功能
    $scope.previewFormatFile = function()
    {
        return;

        if ( $scope.currentFormatFile.id == 0 )
        {
            $scope.showPreview = false;
            $scope.formatData = {};
            return;
        }

        uriData = { id: $scope.currentFormatFile.id, op: "content", };
        uriData = angular.toJson( uriData );

        CommonService.updatePartOne("formatFileList", uriData, 
            function(data)
            {
                // 解析文件内容
                $scope.showFormatFile(data);
            },
            function(data, status )
            {
                onErrorResult(data);
            }
        );
    }

    // 解析文件并显示
    $scope.showFormatFile = function( data )
    {      
        $scope.showPreview = true;
        $scope.formatData = data;
        $scope.formatData.current_date = getNowFormatDate1("-");
        $scope.formatData.current_time = getNowFormatDate3(":");
        $scope.formatData.week_cn_name = "星期" + "日一二三四五六".charAt(new Date().getDay());
        var d = new Date();
        var weekday = [ "Sun", "Mon", "Tue", "Wen", "Thu", "Fri", "Sat"];
        $scope.formatData.week_en_name = weekday[d.getDay()]; 
        
        $scope.formatData.message_content = "欢迎乘坐成都地铁二号线！";
        $scope.formatData.emergency_content = "成都元钧PIS系统";

    }

    // 版式文件发送历史记录
    $scope.getSendHistoryList = function()
    {
        if ( $scope.currentFormatFile.id == 0 )
        {
            $scope.messageLogList = [];
            return;
        }

        uriData = "op=data&optype=format&oi=" + $scope.currentFormatFile.id + "&o=" + $scope.pager2.pageIndex + "&r=" + $scope.pager2.pageSize;
        uriData += "&dataType=current";
        
        CommonService.getAll("messageManage", uriData, 
            function(data)
            {
                $scope.messageLogList = listToObject(data, 'rows').rows;
                $scope.count        = data.count;
                $scope.totalPage2    = Math.ceil(parseInt(data.count) / $scope.pager2.pageSize);
                $scope.pager2.update(data.count);

                getDataDict( $scope.messageLogList, "send_status", "CTRL_RECEIVE_STATUS", "send_status_name");
                getDataDict( $scope.messageLogList, "message_type", "MSG_TYPE_CONTROLLER", "message_type_name");
                
                // 删除已经重发前的消息
                for ( i = 0; i < $scope.messageLogList.length; i++)
                {
                    if ($scope.messageLogList[i].message_sn == 0 )
                        $scope.messageLogList.splice(i, 0);
                }

                for ( i = 0; i < $scope.messageLogList.length; i++)
                {
                    $scope.messageLogList[i].no = i + ( $scope.pager2.pageIndex - 1) * $scope.pager2.pageSize + 1;
                    $scope.messageLogList[i].name =  $scope.currentFormatFile.name;
                    $scope.messageLogList[i].sendTime = $scope.messageLogList[i].send_date + " " + $scope.messageLogList[i].send_time;
                }

            },
            function(data, status )
            {
                onErrorResult(data);

                $scope.count2        = 0;
                $scope.totalPage2    = 0;
                $scope.pager2.update(0);

            }
        );
    }

    $scope.reSendMessage = function(item)
    {
        item.op = "resend";
        var uriData = angular.toJson( item );
        CommonService.updatePartOne("messageManage", uriData, 
            function(data)
            {
                MsgBox.info("提示", "操作成功！");
                $scope.getSendHistoryList();
            },
            function(data)
            {
                onErrorResult(data);
            }
        );
    }

    $scope.page2 = function(e)
    {
        $scope.pager2.onEvent(e);
        $scope.getSendHistoryList();
    };

    var selectedItemID = 0;
    $scope.showSelectedItem = function()
    {
        if ( $scope.timer )
        {
            $timeout.cancel( $scope.timer);
            $scope.timer = undefined;
        }

        var obj = document.getElementsByName("item" + $scope.currentFormatFile.id);
        if ( obj.length > 0 && selectedItemID != $scope.currentFormatFile.id)
        {
            obj[0].style.background = "#F7F7F7";
            if ( selectedItemID == 0 )
                selectedItemID = $scope.currentFormatFile.id;
            else
            {
                var temp = document.getElementsByName("item" + selectedItemID );
                temp[0].style.background = "";
                selectedItemID = $scope.currentFormatFile.id;
            }
        }
    }

})

