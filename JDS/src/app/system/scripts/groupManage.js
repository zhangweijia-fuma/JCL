

angular.module('userGroupManageControllers', [])

///////////////////////////////////////////////////////////////////////////
//用户组管理首页
//
.controller('userGroupManageCtrl', function($scope, $window, CommonService)
{
    $scope.user = null;
    if(sessionStorage.getItem("strUser"))
        $scope.user=JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"
    
    $scope.groupList    = [];
    $scope.groupNameList= [];
    
    $scope.pageNumbers  = [] ;
    $scope.pager = new Pager(null, "user_group_list");
    $scope.pager.pageIndex  = 1;
    $scope.pager.pageSize   = 20;

    $scope.loader = null;

    //下拉列表初始化
    $scope.initialListCtrl = function()
    {
        uriData = { op: "gl" };      
        uriData = angular.toJson(uriData);
        CommonService.updatePartOne('userGroupManage', uriData, 
            function(data)
            {
                $scope.groupNameList = listToObject(data,'rows').rows;
           
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );  
    }
    $scope.initialListCtrl();

    $scope.seachGroupID = 0;
    $scope.searchByGroup = function()
    {
        $scope.pager.pageIndex = 1;
        $scope.seachGroupID = $scope.groupNameList.selected.id;
        $scope.initialData();
    }

    $scope.initialData = function()
    {
        uriData = "id=" + $scope.user.id;
        if ( $scope.seachGroupID > 0 )
            uriData += "&gid=" + $scope.seachGroupID;

        CommonService.getAll('userGroupManage', uriData,
            function(data) 
            {
                $scope.groupList = listToObject(data,'rows').rows;
                $scope.count = data.count;
                $scope.totalPage = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
                $scope.pager.update(data.count);
            },
            function(data,status)
            {//失败时的回调函数
                onErrorResult(data);
            }
        );
    }

    // 新增用户组
    $scope.showAddGroup = function()
    {
        sessionStorage.removeItem("userGroupDetail");
        window.location.href = "#/userGroupAdd";
    }

    // 修改用户组
    $scope.updateGroupInfo = function( groupItem )
    {
        sessionStorage.setItem("userGroupDetail", JSON.stringify( groupItem ));
        window.location.href = "#/userGroupAdd";
    }

    // 删除用户组
    $scope.showDeleteGroup = function( groupItem )
    {
        MsgBox.confirm("确认", "确认删除此用户组吗？", 
            function ( isConformed ) 
            {
                if (isConformed) 
                    $scope.deleteGroup(groupItem);
            }
        );
    }

    $scope.deleteGroup = function( groupItem )
    {
        var data = { gid:groupItem.id, uid:$scope.user.id, gn : groupItem.c_name };

        var uriData = JSON.stringify(data);
        CommonService.deleteOne("userGroupManage", uriData, 
            function(data)
            {
                MsgBox.info("提示", '删除用户组成功');
                for(i=0; i <$scope.groupList.length; i++)
                {
                    if ( $scope.groupList[i].id == groupItem.id)
                    {
                        $scope.groupList.splice(i, 1);
                        break;
                    }
                }

                if($scope.pager.pageIndex != 1 && $scope.groups.length == 1)
                    $scope.pager.pageIndex-=1;
            
                $scope.page();
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
        $scope.initialData();
    };
}) 

///////////////////////////////////////////////////////////////////////////
// 用户组详情
.controller('userGroupDetailCtrl', function($scope, $window, CommonService)
{
    CommonService.heartbeat();
    $scope.user = null;
    if(sessionStorage.getItem("strUser"))
        $scope.user=JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"

    $scope.treeOptions = 
    {
        nodeChildren: "children",
        injectClasses: 
        {
            ul: "",
            li: "",
            liSelected: "",
            iExpanded: "",
            iCollapsed: "",
            iLeaf: "",
            label: "",
            labelSelected: ""
        }
    };
    //获取所有菜单列表

    $scope.pager = new Pager(null);
    $scope.pager.pageIndex = 1;
    $scope.pager.pageSize  = 8;

    // 当前正在编辑的用户组信息
    $scope.group = {};

    $scope.initialData = function()
    {
        // 从缓存中获取用户组信息，如果没有则表示为新建用户组
        if ( JSON.parse(sessionStorage.getItem("userGroupDetail")) )
        {
            $scope.group = JSON.parse(sessionStorage.getItem("userGroupDetail"));
            $scope.getGroupMember($scope.group.id );   
        }
    }

    $scope.searchMember = function()
    {
        if ( $scope.memberName != "" )
            $scope.getGroupMember($scope.group.id );
    }

    // 获取用户组成员
    $scope.getGroupMember = function( groupID )
    {
        uriData = { op: 'gm', o:$scope.pager.pageIndex, r:$scope.pager.pageSize, gid:groupID,  };   
        if( $scope.memberName )   
            uriData.un = $scope.memberName;

        uriData = angular.toJson(uriData);
        CommonService.updatePartOne('userGroupManage', uriData,
            function(data)
            {
                $scope.groupMemberList = listToObject(data,'rows').rows;
                $scope.count        = data.count;
                $scope.totalPage    = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
                $scope.pager.update(data.count);
            },
            function(data,status)
            {
                onErrorResult(data);
            }
        );  
    }

    $scope.page = function(e)
    {
        $scope.pager.onEvent(e);
        $scope.initialData();
    };

    // $scope.treeClicked = function(event, treeId, treeNode) 
    // {
    //     if ( treeId == undefined )
    //         return;

    //     alert(treeNode.tId + ", " + treeNode.name);

    //     treeObj = $.fn.zTree.getZTreeObj("menu_tree");

    // }

    var setting = 
    {
        // check: { enable: true, chkStyle: "radio", radioType:"all"},
        check: { enable: false },
        data: { },
        key:{ name:'name', children:'children' },
        // callback: { onClick: $scope.treeClicked }
    };



    $scope.getmytree = function()
    {
        var uriData = "op=gm&gid=" + $scope.group.id;

        $scope.loader = new Loader('sel_menu_tree');
        CommonService.getAll("userGroupMenuManage", uriData, 
            function(data)
            {
                $scope.loader.close();
            
                $scope.treedata = data;
                $.fn.zTree.init($("#menu_tree"), setting, $scope.treedata);

            },
            function()
            {
                $scope.loader.close();
                onErrorResult(data);
            }
        );

        $scope.checkMsg = "全选";
    }

    $scope.expandedNodes = [];

    $scope.updateGroupInfo = function()
    {
        sessionStorage.setItem( "userGroupDetail", JSON.stringify($scope.group) );
        window.location.href = "#/userGroupAdd";
    }   
})

///////////////////////////////////////////////////////////////////////////
// 用户组新增、修改
.controller('userGroupAddCtrl', function($scope, $window, $rootScope, CommonService)
{
    $scope.user = null;
    if(sessionStorage.getItem("strUser"))
        $scope.user=JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"

    $scope.pager = new Pager(null);
    $scope.pager.pageIndex = 1;
    $scope.pager.pageSize  = 8;

    $scope.pager2 = new Pager( "pager2" );
    $scope.pager2.pageIndex = 1;
    $scope.pager2.pageSize  = 4;

    $scope.group = {};

    // 用户组的实际成员列表
    $scope.groupMemberList = [];

    // 在用户组成员列表中显示的成员列表，
    $scope.memberList = [];

    // 所有账户列表
    $scope.allAccountList = [];

    // 用于显示的账户列表
    $scope.accountList = [];

    // 
    $scope.currentGroup = { id: 0 };

    $scope.isSystemGroup = false;
    if ( sessionStorage.getItem("userGroupDetail"))
    {
        $scope.group = JSON.parse(sessionStorage.getItem("userGroupDetail"));
        sessionStorage.removeItem("userGroupDetail");

        $scope.editMode = "更新";
        $scope.currentGroup = $scope.group;
        if ( $scope.group.group_type == "1" )
            $scope.isSystemGroup = true;
    }
    else
    {
        $scope.group.id = 0;
        $scope.editMode = "新建";
    }

    $scope.page = function(e)
    {
        $scope.pager.onEvent(e);
        $scope.getShowMemberList();
    }

    // 获得用于显示的成员列表
    $scope.getShowMemberList = function()
    {
        var index = ($scope.pager.pageIndex - 1) * $scope.pager.pageSize;
        if ( index > $scope.groupMemberList.length )
            index = 0;

        $scope.memberList = [];
        for( i = index; i < $scope.groupMemberList.length && ( i - index ) < $scope.pager.pageSize ; i++)
            $scope.memberList[i - index] = $scope.groupMemberList[i];

        $scope.totalPage    = Math.ceil( $scope.groupMemberList.length / $scope.pager.pageSize);
        $scope.pager.update( $scope.groupMemberList.length );

        $scope.__apply();
    }

    $scope.__apply = function () 
    {
        if (!$rootScope.$$phase)
            $scope.$digest();
    };

    $scope.initialData = function()
    {
        if( $scope.editMode == "更新" )
        {
            // 获取用户组成员，这里对每页数据不做限制，由getShowMemberList来做分页处理
            uriData = { op : 'gm', o : 1, r : 100000, gid : $scope.group.id };   

            uriData = angular.toJson(uriData);
            CommonService.updatePartOne('userGroupManage', uriData,
                function(data)
                {
                    $scope.groupMemberList = listToObject(data,'rows').rows;
                    $scope.getShowMemberList();
                },
                function(data,status)
                { 
                    onErrorResult(data);
                }
            );

            // 获取用户组菜单
        }
        else
        {
            // 新建用户组，
            $scope.count     = 0;
            $scope.totalPage = 0;
            $scope.pager.update(0);
        }
    }

    $scope.goBack = function()
    {
        if ( $scope.editMode == "更新" )
            sessionStorage.setItem( "userGroupDetail", JSON.stringify($scope.group) );
        
        history.go(-1);
    }

    $scope.page2 = function(e)
    {
        $scope.pager2.onEvent(e);
        $scope.getShowAccountList();
    }

    // 获得用于显示的成员列表
    $scope.getShowAccountList = function()
    {
        var tempList1 = [];

        // 将已经是用户组成员的用户从列表中删除
        for ( i = 0; i < $scope.allAccountList.length; i++)
        {
            var bFind = false;
            for ( j = 0; j < $scope.groupMemberList.length; j++)
            {
                if ( $scope.allAccountList[i].id == $scope.groupMemberList[j].id )
                {
                    bFind = true;
                    break;
                }
            }

            if ( bFind == false )
                tempList1[tempList1.length] = $scope.allAccountList[i];
        }

        // 根据用户姓名进行搜索
        var tempList = [];
        if ( $scope.accountName != "" && $scope.accountName != undefined )
        {
            for ( i = 0; i < tempList1.length; i++)
            {
                if ( tempList1[i].name.indexOf( $scope.accountName ) >= 0 )
                    tempList[tempList.length] = tempList1[i];
            }
        }
        else
            tempList = tempList1;

        var index = ($scope.pager2.pageIndex - 1)*$scope.pager2.pageSize;
        if ( index > tempList.length )
            index = 0;

        $scope.accountList = [];
        for( i = index; i < tempList.length && ( i - index ) < $scope.pager2.pageSize ; i++)
            $scope.accountList[i - index] = tempList[i];

        $scope.totalPage2   = Math.ceil( tempList.length / $scope.pager2.pageSize);
        $scope.pager2.update( tempList.length );

    }

    // 新增用户组成员
    $scope.addMemberDlg = function()
    {
        $scope.accountName = "";

        // 获取所有用户清单，这里对每页数据不做限制，由getShowAccountList来做分页处理
        var uriData = "id=" + $scope.user.id + "&o=1&r=100000";

        CommonService.getAll('accountManage',uriData,
            function(data) 
            {
                $scope.allAccountList  = listToObject(data,'rows').rows;
                $("#selectAccountDlg").modal("show");
                $scope.pager2.pageIndex = 1;
                $scope.getShowAccountList();
            },
            function(data,status)
            {
                // 失败时的回调函数
                onErrorResult(data);
            }
        );
    }

    // 根据用户姓名进行查找
    $scope.searchByName = function()
    {
        $scope.getShowAccountList();
        return;
    }

    // 已经选择用户
    $scope.onSelectAccount = function( item )
    {
        for(i = 0; i < $scope.groupMemberList.length; i++)
        {
            if ( $scope.groupMemberList[i].id == item.id )
            {
                // 选择的用户已经在该用户组中，不需要任何操作，直接返回
                $("#selectAccountDlg").modal("hide");
                return;
            }
        }

        $("#selectAccountDlg").modal("hide");

        // 根据用户姓名排序
        $scope.groupMemberList[$scope.groupMemberList.length] = item;
        $scope.groupMemberList.sort( sortByItemStr("name") );

        $scope.getShowMemberList();
    }

    // 删除用户组成员
    $scope.showDeleteMemberDlg = function( account )
    {
        MsgBox.confirm("确认", "确认将用户 " + account.name + " 从该用户组删除吗？", 
            function ( isConformed ) 
            {
                if (isConformed == false)
                    return;

                for (i=0; i < $scope.groupMemberList.length; i++ )
                {
                    if ( $scope.groupMemberList[i].id != account.id )
                        continue;
                    
                    $scope.groupMemberList.splice(i, 1);
                    $scope.getShowMemberList();
                    return;
                }
            }
        );
    }

    // 保存用户组信息
    $scope.saveGroup = function()
    {
        if( $scope.group.c_name == '' || $scope.group.c_name == undefined )
        {
            $("#c_name").testRemind("用户组名字不能为空!");
            return;
        }

        if( $scope.group.name == '' || $scope.group.name == undefined )
        {
            $("#e_name").testRemind("用户组名字不能为空!");
            return;
        }

        // 检查用户组英文名称是否合法
        var str = /^[A-Za-z]*$/;
        if ( !str.test($scope.group.name) )
        {
            $("#e_name").testRemind("英文组名中只能包含英文字母！");
            return;
        }

        if ( $scope.group.description == "" || $scope.group.description == undefined )
            $scope.group.description = $scope.group.c_name;

        if( $scope.group.name.length > 25 || $scope.group.c_name.length > 25 )
        {
            $("#e_name").testRemind("用户组名字长度不能超过25个字符!");
            return;
        }

        if( $scope.group.description.length > 100 )
        {
            $("#description").testRemind("用户组描述长度不能超过100个字符!");
            return;
        }

        var treeObj = $.fn.zTree.getZTreeObj("menu_tree");
        var menuList = [];
        if(treeObj)
        {
            var nodes = treeObj.getCheckedNodes(true);
            for(var i = 0; i < nodes.length; i++)
                menuList.push(nodes[i].id);
        }

        var data = 
        {
            name        : $scope.group.name, 
            c_name      : $scope.group.c_name,
            group_type  : '2',                          // 所有新创建的用户组类型都为2，即：用户级用户组，可被删除
            description : $scope.group.description, 
            member      : $scope.groupMemberList,
            menu        : menuList,
        };
        
        if ( $scope.currentGroup.id > 0 )
        {
            data.id = $scope.currentGroup.id;
            data.group_type = $scope.currentGroup.group_type; 
        }

        var uriData = JSON.stringify(data);


        if ( $scope.editMode == "新建" )
            $scope.saveGroupInfo( CommonService.createOne, uriData );
        else
            $scope.saveGroupInfo( CommonService.updateOne, uriData );
    }

    // 新建或更新用户组信息
    $scope.saveGroupInfo = function( func, uriData )
    {
        func("userGroupManage", uriData,
            function(data)
            {
                MsgBox.info("提示", "保存成功！", $scope.goBack );
            },
            function(data)
            {
                onErrorResult(data);
            }
        );
    }

    var setting = 
    {
        check : { enable : true },
        data  : {},
        key   : { name : 'name', children : 'children' }
    };

    $scope.loader = null;
    $scope.getGroupMenu = function()
    {
        if( $scope.editMode == "新建" ) 
            var uriData = "gid=0"; 
        else
            var uriData = "gid=" + $scope.currentGroup.id; 

        uriData += "&op=am";

        $scope.loader = new Loader('sel_menu_tree');
        CommonService.getAll("userGroupMenuManage", uriData, 
            function(data)
            {
                $scope.loader.close();
                $scope.treedata = data;
                // console.log(data);
                $.fn.zTree.init($("#menu_tree"), setting, $scope.treedata);
            },
            function()
            {
                $scope.loader.close();
                onErrorResult(data);
            }
        );
    }

    $scope.checkMsg = "全选";
    $scope.checkAll = function()
    {
        if($scope.checkMsg == "全选")
        {
            if( $scope.treedata )
            {
                $scope.checkAllNode( $scope.treedata, true );
                $.fn.zTree.init($("#menu_tree"), setting, $scope.treedata);
                $scope.checkMsg = "清空";
            }
        }
        else
        {
            if( $scope.treedata )
            {
                $scope.checkAllNode($scope.treedata, false);
                $.fn.zTree.init($("#menu_tree"), setting, $scope.treedata);
                $scope.checkMsg = "全选";
            }
        }
    }

    $scope.checkAllNode = function( nodes, bool)
    {
        if(nodes)
        {
            for(var i = 0 ; i < nodes.length; i++ )
            {
                nodes[i].checked = bool;
                if( nodes[i].children )
                    $scope.checkAllNode(nodes[i].children,bool);
            }
        }
        else
        {
            onErrorResult(data);
        }
    }

})

