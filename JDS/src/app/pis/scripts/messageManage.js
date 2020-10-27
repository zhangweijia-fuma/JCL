////////////////////////////////////////////////////////////////////////////////////////////

angular.module('messageManageControllers',['treeControl'])

// 下发信息管理，含下发节目单、版式文件、新闻编辑等。
.controller('sendInfoToControllerCtrl',function($scope,$window,$rootScope,$filter,CommonService, FileUploader)
{

    var strUser = sessionStorage.getItem("strUser");

    if(strUser)
        $scope.user = JSON.parse(strUser);
    else
        window.location.href = "/login.html";


    // 单线路模式：false， 多线路模式：true
    $scope.lineMode = projConfig.lineMode;

    $scope.pager = new Pager("pager");
    $scope.pager.pageIndex = 1;
    $scope.pager.pageSize  = 20;

    $scope.pager2 = new Pager("pager2");
    $scope.pager2.pageIndex = 1;
    $scope.pager2.pageSize  = 20;

    $scope.pager3 = new Pager("pager3");
    $scope.pager3.pageIndex = 1;
    $scope.pager3.pageSize  = 20;

    $scope.goback = function()
    {
        history.back();
    }

    // 显示模式
    //  list: 发送节目单
    //  news: 发送新闻

    $scope.showMode = "";

    if ( sessionStorage.getItem("sendListInfo"))
    {
        $scope.listInfo     = JSON.parse( (sessionStorage.getItem("sendListInfo")) );
        $scope.showMode     = "list";
        $scope.showMessage  = "节目单";

        if ( sessionStorage.getItem("Times", 1) )
        {
            sessionStorage.removeItem("Times");
            sessionStorage.removeItem("sendListInfo");
        }
        else
            sessionStorage.setItem("Times", 1);

    }
    else
    {
        // 缺省为新闻
        $scope.showMode     = "list";
        $scope.showMessage  = "节目单";
    }

    // 当前显示的运营线路
    $scope.lineInfo     = { id:0 };
    $scope.stationInfo  = { id : 0 };
    $scope.trainInfo    = { id : 0 };

    // 运营线路、车站及列车列表，用于下拉列表
    $scope.lineList     = [];
    $scope.stationList  = [];
    $scope.trainList    = [];

    // 当前选择的线路、车站或列车ID
    $scope.selectedLineID       = 0;
    $scope.selectedStationID    = 0;
    $scope.selectedTrainID      = 0;

    $scope.newsContent = "";
    $scope.getSelectedItem = function( id, list )
    {
        list.selected = list[0];
        if ( id == 0 )
            return;

        for(i = 0; i < list.length; i++ )
        {
            if ( list[i].id == id )
            {
                list.selected = list[i];
                return;
            }
        }
    
        return;   
    }

    // 初始化下拉列表
    $scope.initListCtrl = function()
    {
        uriData = {};
        if ( $scope.lineInfo.id > 0 )
            uriData.lid = $scope.lineInfo.id;

        uriData = angular.toJson(uriData);
        CommonService.updatePartOne("logManage", uriData,
            function(data)
            {
                $scope.lineList         = listToObject( data.lineList, 'rows').rows;
                $scope.lineList.splice(0, 1);

                $scope.stationList      = listToObject( data.stationList, 'rows').rows;
                $scope.trainList        = listToObject( data.trainList, 'rows').rows;

                $scope.msgTypeList      = listToObject( data.msgTypeList, 'rows').rows;
                
                $scope.getSelectedItem( '0', $scope.msgTypeList );

                $scope.getSelectedItem( $scope.lineInfo.id, $scope.lineList );

            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
    $scope.initListCtrl();


    // 选择运营线路
    $scope.chooseLine = function( line )
    {
        if ( $scope.lineList.selected == undefined )
            $scope.lineList.selected = line;
        
        $scope.stationInfo  = { id : 0 };
        $scope.trainInfo    = { id : 0 };
        $scope.lineInfo     = $scope.lineList.selected;

        $scope.initListCtrl();

        $scope.lineInfo = line;
        $scope.getLineInfo( line.id );

        var name = "checkStationCtrl";
        var obj = document.getElementsByName(name);

        obj[0].checked = false;
        obj[1].checked = false;
        obj[2].checked = false;

    }

    $scope.chooseMsgType = function()
    {
        $scope.updateSoftwarePath = "";

        name = $scope.msgTypeList.selected.name;
        if ( name == "节目单")
            $scope.showMode = "list";
        else if ( name == "新闻")
        {
            $scope.newsContent  = "";
            $scope.showMode     = "news";
            $scope.newsLifeTime = 0;
        }
        else if ( name == "紧急消息")
        {
            // 默认为滚动
            $scope.emergnceMode     = "0";
            $scope.showMode         = "emergnce";
            $scope.emergnceContent  = "";
            $scope.emgcLifeTime     = 0;
        }
        else if ( name == "重启")
        {
            $scope.showMode         = "reboot";
            $scope.rebootMode       = 0;
            $scope.rebootDelayTime  = 0;
        }
        else if ( name == "节目单")
        {
            $scope.showMode = "list";
        }
        else if ( name == "软件更新")
        {
            $scope.showMode     = "update";
            $scope.updateMode   = 0;
        }
        else if ( name == "版式文件")
        {
            $scope.showMode = "format_file";
            $scope.selectedFormatFile = {id: 0};
            $scope.getFormatFileList();
        }
        else if ( name == "更新休眠时间")
        {
            $scope.showMode = "work_time";
            $scope.open_time = "";
            $scope.close_time = "";
        }
        else if ( name == "发送安全垫片")
        {
            $scope.showMode = "safe_video_file";
            $scope.getSafeFileList();
        }
        else
            $scope.showMode = "news";           
    }

    // 初始化运营线路数据
    $scope.initialData = function()
    {
        // 获取运营线路信息
        CommonService.getAll("lineManage", "", 
            function(data)
            {
                $scope.lineList     = listToObject( data.lineList, 'rows').rows;
                $scope.chooseLine( $scope.lineList[0]);
            },
            function(data)
            {
                onErrorResult(data);
            }
        );

        $scope.getProgramList();
        $scope.getFormatFileList();
    }

    // 获取节目单列表 
    $scope.getProgramList = function()
    {
        // 获取节目单列表
        var uriData = "op=list&o=" + $scope.pager.pageIndex + "&r=" + $scope.pager.pageSize;
        
        // 申请状态为已经通过审核，发送状态为未发送的节目单
        uriData += "&as=3&ss=1";

        CommonService.getAll("programList", uriData, 
            function(data)
            {
                $scope.programList  = listToObject( data, 'rows').rows;
                $scope.count        = data.count;
                $scope.totalPage    = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
                $scope.pager.update(data.count);

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

    // 获取版式文件列表
    $scope.getFormatFileList = function()
    {
        $scope.formatFileList = [];

        // 获取版式文件信息
        uriData = "op=list&o=" + $scope.pager2.pageIndex + "&r=" + $scope.pager2.pageSize;
        uriData += "&dt=list";

        CommonService.getAll("formatFileList", uriData, 
            function(data)
            {
                $scope.formatFileList  = listToObject( data, 'rows').rows;
                $scope.count        = data.count;
                $scope.totalPage2    = Math.ceil(parseInt(data.count) / $scope.pager2.pageSize);
                $scope.pager2.update(data.count);

                for ( i = 0; i < $scope.formatFileList.length; i++)
                {
                    $scope.formatFileList[i].no = i + 1;
                }
            },
            function(data)
            {
                onErrorResult(data);

                $scope.count        = 0;
                $scope.totalPage2   = 0;
                $scope.pager.update(0);
            }
        );
    }

    // 获取版式文件列表
    $scope.getSafeFileList = function()
    {
        $scope.safeFileList = [];
        $scope.selectedSafeVideoFile = { id: 0};

        var uriData = "op=list&type=0&o=" + $scope.pager3.pageIndex + "&r=" + $scope.pager3.pageSize;

        CommonService.getAll("programFile", uriData, 
            function(data)
            {
                $scope.safeFileList = listToObject(data, 'rows').rows;
                $scope.count3        = data.count;
                $scope.totalPage3   = Math.ceil(parseInt(data.count) / $scope.pager3.pageSize);
                $scope.pager3.update(data.count);

                for( i = 0; i < $scope.safeFileList.length; i++)
                    $scope.safeFileList[i].no = i + 1 + ($scope.pager3.pageIndex - 1) * $scope.pager3.pageSize;

            },
            function(data)
            {
                onErrorResult(data);

                $scope.count3       = 0;
                $scope.totalPage3   = 0;
                $scope.pager3.update(0);
            }
        );
    }

    // 树被选中时
    $scope.onTreeChecked = function(event, treeId, treeNode) 
    {
        elementsByName = ( treeNode.install_type <='1' ) ? "checkTrainCtrl" : "checkStationCtrl";
        var obj = document.getElementsByName(elementsByName);

        // 检查是否已经全部选中或取消
        var status = treeNode.checked ? true : false;
        
        treeName = ( treeNode.install_type <= '1' ) ? "train_tree" : "station_tree";
        var treeObj = $.fn.zTree.getZTreeObj( treeName );

        nodes = treeObj.getNodes()[0];

        $scope.checkNode( nodes.children, treeNode.id, status);

        if ( $scope.getAllCheckStatus( nodes.children, status) )
            obj[0].checked = status;
        else
            obj[0].checked = false;

        if (treeName == "train_tree")
            return;

        if ( $scope.getCheckStatusByType( nodes.children, true, '2', '3') )
            obj[1].checked = true;
        else
            obj[1].checked = false;

        if ( $scope.getCheckStatusByType( nodes.children, true, '4', '4') )
            obj[2].checked = true;
        else
            obj[2].checked = false;
    }

    // 树被点击时
    $scope.onTreeClicked = function(event, treeId, treeNode) 
    {
        if ( treeNode == null )
            return;

        if ( treeNode.nodeType == 'stationRoot')
        {
            var treeName = "station_tree";
            var treeObj = $.fn.zTree.getZTreeObj( treeName );

            var elementsByName = "checkStationCtrl";
            var obj = document.getElementsByName(elementsByName);
            var status = (obj[0].checked) ? false : true;
            obj[0].checked = status;
            $scope.checkStationCtrl(0);
        }
        else if ( treeNode.nodeType == 'trainRoot')
        {
            var treeName = "train_tree";
            var treeObj = $.fn.zTree.getZTreeObj( treeName );

            var elementsByName = "checkTrainCtrl";
            var obj = document.getElementsByName(elementsByName);
            var status = (obj[0].checked) ? false : true;
            obj[0].checked = status;
            $scope.checkTrainCtrl(0);
        }
        else if ( treeNode.nodeType == 'station' || ( treeNode.nodeType == 'train') )
        {
            var treeName = ( treeNode.nodeType == 'train' ) ? "train_tree" : "station_tree";
            var elementsByName = ( treeNode.nodeType == 'train' ) ? "checkTrainCtrl" : "checkStationCtrl";
            var treeObj = $.fn.zTree.getZTreeObj( treeName );

            for ( var i = 0; i < treeNode.children.length; i++)
            {
                node = treeNode.children[i];
                node.checked = ( node.checked ) ? false : true;
                treeObj.updateNode( node );
            }
        }
        else
        {
            var status = ( treeNode.checked ) ? false : true;
            treeNode.checked = status;
    
            var elementsByName = ( treeNode.install_type <='1' ) ? "checkTrainCtrl" : "checkStationCtrl";
            var treeName = ( treeNode.install_type <= '1' ) ? "train_tree" : "station_tree";
            var treeObj = $.fn.zTree.getZTreeObj( treeName );

            treeObj.updateNode( treeNode );
        }

        nodes = treeObj.getNodes()[0];

        
        var obj = document.getElementsByName(elementsByName);

        // 检查是否已经全部选中或取消
        if ( nodes.children.length == 0 )
            return;

        if ( $scope.getAllCheckStatus( nodes.children, status) )
            obj[0].checked = status;
        else
            obj[0].checked = false;

        if (treeName == "train_tree")
            return;

        if ( $scope.getCheckStatusByType( nodes.children, true, '2', '3') )
            obj[1].checked = true;
        else
            obj[1].checked = false;

        if ( $scope.getCheckStatusByType( nodes.children, true, '4', '4') )
            obj[2].checked = true;
        else
            obj[2].checked = false;

    }

    // 检查所有控制器是否已经被全部选中或全部取消
    // 控制器为叶子节点，无子节点
    $scope.getAllCheckStatus = function( nodes, status)
    {
        if(nodes)
        {
            for(var i = 0 ; i < nodes.length; i++ )
            {
                if( nodes[i].children.length > 0 )
                {
                    if ( $scope.getAllCheckStatus(nodes[i].children, status) != status )
                        return false;
                }
                else
                {
                    if ( nodes[i].checked != status )
                        return false;
                }
            }

            return true;
        }
    }

    // 检查指定控制器类型是否已经被全部选中或全部取消
    // 控制器为叶子节点，无子节点
    $scope.getCheckStatusByType = function( nodes, status, startType, endType)
    {
        if(nodes)
        {
            for(var i = 0 ; i < nodes.length; i++ )
            {
                if( nodes[i].children.length > 0 )
                {
                    if ( $scope.getCheckStatusByType(nodes[i].children, status, startType, endType) != status )
                        return false;
                }
                else
                {
                    if ( nodes[i].install_type < startType || nodes[i].install_type > endType)
                        continue;

                    if ( nodes[i].checked != status)
                        return false;
                }
            }

            return true;
        }
    }

    var setting = 
    {
        check: { enable: true },
        data: { },
        key:{ name:'name', children:'children' },
        callback: 
        { 
            onCheck: $scope.onTreeChecked, 
            onClick: $scope.onTreeClicked,
        },
        view: 
        {
            fontCss: function (treeId, treeNode) 
            {
                selectedItemColor = {"color": "red", "font-weight": (( treeNode.status == '0') ? "bold" : "normal") };
                nonSelectedItemColor = 
                {
                    "color" : (!('status' in treeNode)) ? "black" : (( treeNode.status == '0') ? "green" : "gray"),
                    "font-weight": (( treeNode.status == '0') ? "bold" : "normal"),
                    "background": "white",
                };
                
                return (!!treeNode.highlight) ? selectedItemColor : nonSelectedItemColor;
            }
        }
    };

    $scope.getLineInfo = function( lineID )
    {
        if ( lineID == 0 || lineID == undefined)
            return;

        uriData = { op:"data", ut:"station", lid: lineID };
        uriData = angular.toJson(uriData);

        $scope.loader = new Loader('sel_station_tree');
        CommonService.updatePartOne("lineManage", uriData, 
            function(data)
            {
                $scope.loader.close();
                $scope.stationTreeData = data;
                $.fn.zTree.init($("#station_tree"), setting, $scope.stationTreeData);

                var name = "checkStationCtrl";
                var obj = document.getElementsByName(name);
                obj[0].checked = false;

            },
            function(data, status)
            {
                $scope.loader.close();
                onErrorResult(data);
            }
        );

        uriData = { op:"data", ut:"train", lid: lineID };
        uriData = angular.toJson(uriData);

        $scope.loader = new Loader('sel_train_tree');
        CommonService.updatePartOne("lineManage", uriData, 
            function(data)
            {
                $scope.loader.close();
                $scope.trainTreeData = data;
                $.fn.zTree.init($("#train_tree"), setting, $scope.trainTreeData);

                var name = "checkTrainCtrl";
                var obj = document.getElementsByName(name);
                obj[0].checked = false;

            },
            function(data, status)
            {
                $scope.loader.close();
                onErrorResult(data);
            }
        );
    }

    $scope.selectedProgram = { id:0};
    $scope.chooseProgramList = function(item)
    {
        // 一次只能发送一个节目单
        $scope.selectedProgram = { id:0};

        var obj = document.getElementsByName("list");

        for( i = 0; i < obj.length; i++ )
        {
            if ( obj[i].checked == true )
                $scope.selectedProgram = item;
        }
    }

    $scope.selectedFormatFile = {id: 0};
    $scope.chooseFormatFile = function(item)
    {
        var name = "format_file" + item.id;
        var obj = document.getElementsByName(name);

        if ( obj[0].checked == true )
        {
            $scope.selectedFormatFile = item;
        }
    }

    $scope.selectedSafeVideoFile = {};
    $scope.chooseSafeVideoFile = function(item)
    {
        $scope.selectedSafeVideoFile = item;
    }

    $scope.updateMode = 0;
    $scope.chooseUpdateMode = function(updateMode)
    {
        $scope.updateMode = updateMode;
    }

    $scope.rebootMode = 0;
    $scope.chooseRebootMode = function(mode)
    {
        $scope.rebootMode = mode;
    }

    // 选择所有车站或取消全部选中
    $scope.checkStationCtrl = function(controllerType)
    {
        var name = "checkStationCtrl";
        var obj = document.getElementsByName(name);

        var status = obj[controllerType].checked;

        if ( controllerType == 0 )
        {
            startType = '0';
            endType   = '4';
        }
        else if ( controllerType == 1 )
        {
            startType = '2';
            endType   = '3';
        }
        else
        {
            startType = '4';
            endType   = '4';
        }

        $scope.checkAllNode( $scope.stationTreeData.children, status, startType, endType);
        $.fn.zTree.init($("#station_tree"), setting, $scope.stationTreeData);
        
        // 检查是否已经全部选中或取消
        var treeObj = $.fn.zTree.getZTreeObj( "station_tree" );
        nodes = treeObj.getNodes()[0];

        if ( nodes.children.length == 0 )
            return;

        if ( $scope.getAllCheckStatus( nodes.children, status) )
            obj[0].checked = status;
        else
            obj[0].checked = false;

        if ( $scope.getCheckStatusByType( nodes.children, true, '2', '3') )
            obj[1].checked = true;
        else
            obj[1].checked = false;

        if ( $scope.getCheckStatusByType( nodes.children, true, '4', '4') )
            obj[2].checked = true;
        else
            obj[2].checked = false;

        obj[0].checked = obj[1].checked && obj[2].checked;
    }

    // 选择所有列车或取消全部选中
    $scope.checkTrainCtrl = function( trainID )
    {
        var name = "checkTrainCtrl";
        var obj = document.getElementsByName(name);

        var status = obj[0].checked; 

        $scope.checkAllNode( $scope.trainTreeData.children, status, '0', '1');
        $.fn.zTree.init($("#train_tree"), setting, $scope.trainTreeData);
    }

    $scope.checkAllNodeCheckStatus = function( nodes, checkStatus)
    {
        for(var i = 0 ; i < nodes.length; i++ )
        {
            if( nodes[i].children.length > 0 )
                $scope.checkAllNode(nodes[i].children, checkStatus);
            
            if ( nodes[i].nodeType == "controller" )
                nodes[i].checked = checkStatus;
        }
    }

    $scope.checkAllNode = function( nodes, checkStatus, startType, endType)
    {
        for(var i = 0 ; i < nodes.length; i++ )
        {
            if( nodes[i].children.length > 0 )
                $scope.checkAllNode(nodes[i].children, checkStatus, startType, endType);
            
            if ( nodes[i].nodeType == "controller" && nodes[i].install_type >= startType && nodes[i].install_type <= endType )
                nodes[i].checked = checkStatus;
        }
    }

    $scope.checkNode = function( nodes, id, checkStatus)
    {
        for(var i = 0 ; i < nodes.length; i++ )
        {
            if( nodes[i].children.length > 0 )
                $scope.checkNode(nodes[i].children, id, checkStatus);

            if ( nodes[i].nodeType == "controller" && nodes[i].id == id )
            {
                nodes[i].checked = checkStatus;
                return;
            }
        }
    }

    /* 版本文件上传 uploader */
    var uploader = $scope.uploader = new FileUploader({
        url:uploadURL,
        method: 'POST',
        headers: { Authorization: sessionStorage.getItem('authToken'), },
        autoUpload: true,
        formData: 
        [{ 
            'module' : 'PIS', 
            'service_type' : 'update_software', 
        }]
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
        $scope.softwareFileName = response.file_name
        $scope.updateSoftwarePath = response.file_new_path;
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
    }

    // 获取所有被选中的控制器
    $scope.getAllCheckNoed = function( treeName, controllerList )
    {
        var treeObj = $.fn.zTree.getZTreeObj( treeName );
        if( treeObj )
        {
            var nodes = treeObj.getCheckedNodes(true);
            for(var i = 0; i < nodes.length; i++)
            {
                temp = 
                {
                    id          : nodes[i].id,
                    code        : nodes[i].code,
                    install_type: nodes[i].install_type,
                    ip_address  : nodes[i].ip_address,
                }

                controllerList.push( temp );
            }
        }

    }
        
    // 发送节目单、新闻、重启或版本更新消息给控制器
    $scope.sendMessage = function()
    {
        // 准备需要下发的控制器列表
        $scope.controllerList = [];

        $scope.getAllCheckNoed( "station_tree", $scope.controllerList );
        $scope.getAllCheckNoed( "train_tree", $scope.controllerList );
        
        if ( $scope.controllerList.length == 0 )
        {
            MsgBox.info("提示", "请选择至少一个控制器！");
            return;
        }

        var uriData = {};
        if ( $scope.showMode == "list")
        {
            if ( $scope.selectedProgram.id == 0 )
            {
                MsgBox.info("提示", "请选择1个待下发的节目单！");
                return;
            }

            uriData = { id : $scope.selectedProgram.id, op:"data", msgType: "list", controllerList : $scope.controllerList };

        }
        else if ( $scope.showMode == "news")
        {
            if ( $scope.newsContent.indexOf("'") >= 0 )
            {
                MsgBox.info("提示", "新闻内容中不能包含英文单引号！");
                return;
            }
            uriData = { op:"data", msgType:"news", controllerList : $scope.controllerList, content:$scope.newsContent, LifeTime : $scope.newsLifeTime };
        }
        else if ( $scope.showMode == "emergnce")
        {
            if ( $scope.emergnceMode == undefined )
            {
                MsgBox.info("提示", "请选择显示方式！");
                return;
            }
            if ( $scope.emergnceContent.indexOf("'") >= 0 )
            {
                MsgBox.info("提示", "紧急通知内容中不能包含英文单引号！");
                return;
            }
            uriData = { op:"data", msgType:"emergnce", controllerList : $scope.controllerList, mode: $scope.emergnceMode, content: $scope.emergnceContent, LifeTime: $scope.emgcLifeTime };
        }
        else if ( $scope.showMode == "reboot")
        {
            uriData = { op:"data", msgType:"reboot", rebootDelayTime: $scope.rebootDelayTime, rebootMode: $scope.rebootMode, controllerList : $scope.controllerList };
        }
        else if ( $scope.showMode == "update")
        {
            if ( $scope.updateSoftwarePath == undefined || $scope.updateSoftwarePath == "" )
            {
                MsgBox.info("提示", "请上传版本文件！");
                return;
            }

            uriData = { op:"data", msgType:"update", updateMode: $scope.updateMode, controllerList : $scope.controllerList, file_path: $scope.updateSoftwarePath, file_name: $scope.softwareFileName };
        }
        else if ( $scope.showMode == "format_file")
        {
            if ( $scope.selectedFormatFile.id == 0 )
            {
                MsgBox.info("提示", "请选择1个待下发的版式文件！");
                return;
            }

            // 检查接收控制器的类型，车载、站厅、站台这三类控制器不能同时出现
            var hallCtrlCount       = 0;
            var platformCtrlCount   = 0;
            var trainCtrlCount      = 0;
            for( var i = 0; i < $scope.controllerList.length; i++)
            {
                if ( $scope.controllerList[i].install_type <= '1' )
                    trainCtrlCount += 1;
                else if ( $scope.controllerList[i].install_type >= '4')
                    hallCtrlCount += 1;
                else if ( $scope.controllerList[i].install_type >= '2' && $scope.controllerList[i].install_type <= '3')
                    platformCtrlCount += 1;
            }

            var errorSelect = false;
            if ( hallCtrlCount > 0 && (platformCtrlCount + trainCtrlCount) > 0 )
                errorSelect = true;

            else if ( platformCtrlCount > 0 && (hallCtrlCount + trainCtrlCount) > 0 )
                errorSelect = true;

            else if ( trainCtrlCount > 0 && (hallCtrlCount + platformCtrlCount) > 0 )
                errorSelect = true;

            if ( errorSelect == true )
            {
                MsgBox.info("发送版式文件时，只能选择站厅、站台或车载控制器其中之一！");
                return;
            }

            uriData = { op:"data", msgType:"format_file", controllerList : $scope.controllerList, file: $scope.selectedFormatFile };
        }
        else if ( $scope.showMode == "work_time")
        {
            if ( $scope.open_time == "" && $scope.close_time == "" )
            {
                MsgBox.info("提示", "请输入控制器休眠时间！");
                return;
            }
            uriData = { op:"data", msgType:"work_time", controllerList : $scope.controllerList, open_time: $scope.open_time, close_time: $scope.close_time };
        }
        else if ( $scope.showMode == "safe_video_file")
        {
            if ($scope.selectedSafeVideoFile.id == 0 )
            {
                MsgBox.info("提示", "请选择安全垫片");
                return;
            }
            uriData = { op:"data", msgType:"safe_file", controllerList : $scope.controllerList, file: $scope.selectedSafeVideoFile };
        }
        else
        {
            // 出现错误
            return;
        }

        uriData = angular.toJson( uriData);
        CommonService.updatePartOne("messageManage", uriData, 
            function(data)
            {
                MsgBox.info("提示", "操作成功！");
                
                // 更新节目单状态
                if ( $scope.showMode == "list" )
                    $scope.getProgramList();
                
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    $scope.page = function(e)
    {
        $scope.pager.onEvent(e);
        $scope.getProgramList();
    };

    $scope.page2 = function(e)
    {
        $scope.pager2.onEvent(e);
        $scope.getFormatFileList();
    };

    $scope.page3 = function(e)
    {
        $scope.pager3.onEvent(e);
        $scope.getSafeFileList();
    };

})

// 查询发送及接收到的消息
.controller('messageQueryCtrl', function($scope, $window, $rootScope, $filter, CommonService, $timeout)
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

    $scope.goback = function()
    {
        history.back();
    }

    // 初始化下拉列表
    $scope.senderList   = [];
    $scope.msgTypeList = [];
    $scope.sendStatusList = [];
    $scope.send_date    = "";
    $scope.receive_date = "";

    // 默认查询当前数据
    $scope.dataType = "current";
    $scope.dataSource = "internal";

    $scope.currentMsgItem = { id: 0};

    $scope.initListCtrl = function()
    {
        uriData = { op: "list", dt: $scope.dataType, ds: $scope.dataSource };
        uriData = angular.toJson(uriData);
        CommonService.updatePartOne("messageList", uriData,
            function(data)
            {
                $scope.senderList       = listToObject( data.senderList, 'rows').rows;
                $scope.msgTypeList      = listToObject( data.msgTypeList, 'rows').rows;
                $scope.sendStatusList   = listToObject( data.sendStatusList, 'rows').rows;
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
    $scope.initListCtrl();

    $scope.searchMessage = function()
    {
        $scope.pager.pageIndex = 1;
        $scope.initialData();
    }

    $scope.resetTime = function( index )
    {
        $scope.send_date = "";
        $scope.pager.pageIndex = 1;
        $scope.initialData();
    }

    // 查询内部发送的消息还是从综合监控来的消息
    // 暂不使用
    $scope.chooseDataSource= function( dataSource )
    {
        // 查询历史数据时需要再处理
        $scope.dataSource = dataSource;

        return;
        var item = { id : 0};
        $scope.getMessageList(item);
    }

    $scope.chooseDataType= function( dataType )
    {
        if ( dataType == 'history' )
        {
            MsgBox.confirm("提示", "查询历史数据会导致响应时间很长。确认吗？", 
                function( isConformed )
                {
                    if ( !isConformed )
                    {
                        var obj = document.getElementsByName("dataType");
                        obj[0].checked = true;
                        obj[1].checked = false;
                        return;
                    }

                    $scope.dataType = 'history';
                    $scope.initListCtrl();
                    $scope.initialData();
                }
            )
        }
        else
        {
            $scope.dataType = 'current';
            $scope.initListCtrl();
            $scope.initialData();
        }
    }

    $scope.initialData = function()
    {
        var uriData = "op=data&o=" + $scope.pager.pageIndex + "&r=" + $scope.pager.pageSize;

        if ( $scope.senderList.selected && $scope.senderList.selected.id > 0 )
            uriData += "&sid=" + $scope.senderList.selected.id + "&sname=" + $scope.senderList.selected.name;

        if ( $scope.msgTypeList.selected && $scope.msgTypeList.selected.name != "选择全部")
            uriData += "&mt=" + $scope.msgTypeList.selected.code;

        if ( $scope.send_date != "" ) 
            uriData += "&sd=" + $scope.send_date;

        if ( $scope.sendStatusList.selected && $scope.sendStatusList.selected.code != "0")
            uriData += "&ss=" + $scope.sendStatusList.selected.code;

        uriData += "&dt=" + $scope.dataType + "&ds=" + $scope.dataSource;

        CommonService.getAll("messageList", uriData, 
            function(data)
            {
                $scope.messageList  = listToObject( data, 'rows').rows;
                $scope.count        = data.count;
                $scope.totalPage    = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
                $scope.pager.update(data.count);

                getDataDict($scope.messageList, "send_status", "PROGRAM_SEND_STATUS", "send_status_name");
                getDataDict($scope.messageList, "message_type", "MESSAGE_TYPE", "message_type_name");

                // 删除已经重发前的消息
                for ( i = 0; i < $scope.messageList.length; i++)
                {
                    if ($scope.messageList[i].message_sn == 0 )
                        $scope.messageList.splice(i, 0);
                }

                for( i = 1; i <= $scope.messageList.length; i++)
                {
                    $scope.messageList[i - 1].no = i + ( $scope.pager.pageIndex - 1 ) * $scope.pager.pageSize;
                    $scope.messageList[i - 1].sendTime = $scope.messageList[i - 1].send_date + " " + $scope.messageList[i - 1].send_time;
                }

                if ( $scope.messageList.length > 0 )
                {
                    $scope.getMessageList( $scope.messageList[0] );
                    $scope.timer = $timeout( $scope.showSelectedItem, 100);
                }
                else
                {
                    $scope.totalPage2   = 0;
                    $scope.msgSuccess   = 0;
                    $scope.msgFailed    = 0;
                    $scope.msgAll       = 0;

                    $scope.messageLogList = [];
                    
                    $scope.pager2.update(0);
                }
            },
            function(data)
            {
                $scope.pager.update(0);
                onErrorResult(data);
            }
        );
    }

    $scope.page = function(e)
    {
        $scope.pager.onEvent(e);
        $scope.initialData();
    };

    $scope.page2 = function(e)
    {
        $scope.pager2.onEvent(e);
        $scope.getMessageList($scope.currentMsgItem);
    };

    var selectedItemID = 0;
    // 在取消或选择某一行消息的时候显示或隐藏指定行的背景色
    $scope.showSelectedItem = function()
    {
        if ( $scope.timer )
        {
            $timeout.cancel( $scope.timer);
            $scope.timer = undefined;
        }

        var obj = document.getElementsByName("item" + $scope.currentMsgItem.id);
        if ( obj.length > 0 && selectedItemID != $scope.currentMsgItem.id )
        {
            obj[0].style.background = "#F7F7F7";
            if ( selectedItemID == 0 )
                selectedItemID = $scope.currentMsgItem.id;
            else
            {
                var temp = document.getElementsByName("item" + selectedItemID );
                if ( temp.length > 0 )
                    temp[0].style.background = "";

                selectedItemID = $scope.currentMsgItem.id;
            }
        }
    }

    $scope.msgSendStatus = 'all';
    $scope.searchMessageBySendStatus = function( status )
    {
        $scope.msgSendStatus = status;
        $scope.getMessageList( $scope.currentMsgItem );
    }

    // 获取消息发送的详细清单
    $scope.getMessageList = function( item )
    {
        $scope.currentMsgItem = item;

        $scope.showSelectedItem();

        uriData = "op=data&o=" + $scope.pager.pageIndex + "&r=" + $scope.pager.pageSize;
        uriData += "&ss=" + $scope.msgSendStatus;

        if ( $scope.dataSource == "internal")
            uriData += "&mid=" + item.id;
        
        uriData += "&dataType=" + $scope.dataType;
        uriData += "&dataSource=" + $scope.dataSource;

        CommonService.getAll("messageManage", uriData, 
            function(data)
            {
                $scope.messageLogList  = listToObject( data, 'rows').rows;
                $scope.count        = data.count;
                $scope.totalPage2    = Math.ceil(parseInt(data.count) / $scope.pager2.pageSize);
                $scope.pager2.update(data.count);

                $scope.msgSuccess   = data.msgSuccess;
                $scope.msgFailed    = data.msgFailed;
                $scope.msgAll       = $scope.msgSuccess + $scope.msgFailed;

                getDataDict($scope.messageLogList, "send_status", "CTRL_RECEIVE_STATUS", "send_status_name");
                getDataDict($scope.messageLogList, "message_type", "MESSAGE_TYPE", "message_type_name");
                
                for( i = 1; i <= $scope.messageLogList.length; i++)
                {
                    $scope.messageLogList[i - 1].no = i + ( $scope.pager2.pageIndex - 1 ) * $scope.pager2.pageSize;
                }
            },
            function(data)
            {
                $scope.totalPage2 = 0;
                $scope.pager2.update(0);
                onErrorResult(data);
            }
        );
    }

    // 重新发送指定消息
    $scope.reSendMessage = function( item )
    {
        item.op = "resend";
        var uriData = angular.toJson( item );
        CommonService.updatePartOne("messageManage", uriData, 
            function(data)
            {
                MsgBox.info("提示", "操作成功！");
                var data = { id: item.msg_list_id };
                $scope.getMessageList( data );
            },
            function(data)
            {
                onErrorResult(data);
            }
        );
    }

    // 导出到Excel文件
    $scope.exporToFile = function()
    {
        var uriData = "op=excel";

        if ( $scope.senderList.selected && $scope.senderList.selected.id > 0 )
            uriData += "&sid=" + $scope.senderList.selected.id;

        if ( $scope.msgTypeList.selected && $scope.msgTypeList.selected.name != "选择全部")
            uriData += "&mt=" + $scope.msgTypeList.selected.code;

        if ( $scope.send_date != "" ) 
            uriData += "&sd=" + $scope.send_date;

        if ( $scope.sendStatusList.selected && $scope.sendStatusList.selected.code != "0")
            uriData += "&ss=" + $scope.sendStatusList.selected.code;

        uriData += "&dataType=" + $scope.dataType;

        CommonService.getAll('messageList', uriData,
            function(data)
            {
                var filename = "消息查询" + getNowFormatDate2("", "", "-") + ".xls";
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
