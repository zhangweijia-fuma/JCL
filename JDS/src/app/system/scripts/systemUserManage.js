angular.module('systemUserManageControllers', [])

.controller('systemUserManageCtrl', function ($scope, CommonService, FileUploader) 
{
    CommonService.heartbeat();
    $scope.user = null;
    if(sessionStorage.getItem("strUser"))
        $scope.user=JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"
    
    $scope.pager = new Pager(null);
    $scope.pager.pageIndex = 1;
    $scope.pager.pageSize  = 20;

    // 加载数据
    $scope.initialData = function () 
    {
        uriData = "o=" + $scope.pager.pageIndex + "&r=" + $scope.pager.pageSize;      
  	    CommonService.getAll('systemUserManage', uriData, 
            function(data)
            {
          	    $scope.systemUserList = listToObject(data, 'rows').rows;
                $scope.count            = data.count;
                $scope.totalPage        = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
          	    $scope.pager.update(data.count);
                for(i=0; i < $scope.systemUserList.length; i++)
                {
                    $scope.systemUserList[i].no = i + 1 + $scope.pager.pageSize * ( $scope.pager.pageIndex - 1 );
                    var fileList = [''];
                    if ( $scope.systemUserList[i].logo_vertical )
                    {
                        fileList = $scope.systemUserList[i].logo_vertical.split('/');
                        $scope.systemUserList[i].logo_vertical_name = fileList[fileList.length-1];
                    }

                    if ( $scope.systemUserList[i].logo_horizontal )
                    {
                        fileList = $scope.systemUserList[i].logo_horizontal.split('/');
                        $scope.systemUserList[i].logo_horizontal_name = fileList[fileList.length-1];
                    }

                }
          	},
            function(data, status)
            {
                // 失败时的回调函数
                $scope.count      = 0;
                $scope.totalPage  = 0;
                $scope.pager.update(0);
                onErrorResult(data);
          	}
        );
    };


    $scope.createService = function()
    {
        ClearAreaInfo("addr1");
        ClearAreaInfo("addr2");

        currentService = {id : 0 };
        sessionStorage.setItem("systemUserItem", JSON.stringify(currentService));
        window.location.href = '#/systemUserEdit';
    }


    $scope.OnUpdate = function(item)
    {
        ClearAreaInfo("addr1");
        ClearAreaInfo("addr2");

        var addrList = [];

        // if ( item.address1 == )
        addrList.push(item.address1);
        addrList.push(item.address2);

        var uriData = { op: "ua", uid: item.admin_account_id };

        uriData = angular.toJson(uriData);
        CommonService.updatePartOne("accountManage", uriData,
            function(data)
            {
                address1 = data.Address[0];
                if ( address1 != null )
                {
                    SetAreaInfo("addr1", address1);
                    item.addr_detail1 = address1.detailAddr;
                }

                address2 = data.Address[1];
                if ( address2 != null )
                {
                    SetAreaInfo("current_addr", address2);
                    item.addr_detail2 = address2.detailAddr;
                }

                sessionStorage.setItem('systemUserItem', JSON.stringify(item));
                window.location.href = '#/systemUserEdit';
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    $scope.showDeleteDlg = function(item)
    {
        MsgBox.confirm("确认", "确定要删除系统用户 " + item.user_name + " 吗？删除操作将会同时删除该系统用户相关的所有用户、用户组数据！", 
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
        CommonService.deleteOne("systemUserManage", uriData, 
            function(data)
            {
                // 删除后重新请求数据
                $scope.pager.pageIndex = 1;
                $scope.initialData();
            },
            function(data, status )
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

.controller('systemUserEditCtrl', function ($scope, CommonService, FileUploader) 
{
    CommonService.heartbeat();
    $scope.user = null;
    if(sessionStorage.getItem("strUser"))
        $scope.user = JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"

    $scope.serviceAdminAccount = {id: 0, sex: 1, photo_path: './resource/images/default.png' };
    $scope.serviceAdminGroup = { id: 0, group_type: '1', };             // 默认新建用户组为系统用户组，可显示但不能被管理员删除 

    if (sessionStorage.getItem("systemUserItem") )
    {
        $scope.systemUserItem = JSON.parse( sessionStorage.getItem("systemUserItem") );

        $scope.editType = "更新";
    }
    else
    {
        $scope.systemUserItem = {};
        $scope.editType = "新建";
    }

    // 在离开页面的时候删除缓存的信息
    $scope.$on('$destroy', function()
    {
        sessionStorage.removeItem('systemUserItem');

        ClearAreaInfo("addr1");
        ClearAreaInfo("addr2");
    });


    $scope.initListCtrl = function()
    {
        var uriData = { op: "ust" };
        uriData = angular.toJson(uriData);

        CommonService.updatePartOne("systemUserManage", uriData,
            function(data)
            {
                $scope.userServiceList   = listToObject( data.userServiceList, 'rows').rows;
                $scope.userServiceList.shift();               
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
    $scope.initListCtrl();

    $scope.initialData = function()
    {
        // 获取系统用户管理员账户信息
        var uriData = "id=" + $scope.user.id + "&uid=" + $scope.systemUserItem.admin_account_id ;
        
        CommonService.getAll('accountManage', uriData, 
            function (data) 
            {
                $scope.serviceAdminAccount = listToObject(data,'rows').rows[0];
                if ( $scope.serviceAdminAccount.sex == '1')
                    $scope.sex_name = '男';
                else
                    $scope.sex_name = '女';
            }, 
            function (data, status) 
            {
                // 失败时的回调函数
                onErrorResult(data);
            }
        );

        // 获取管理员用户组
        var uriData = "id=" + $scope.user.id + "&gid=" + $scope.systemUserItem.admin_group_id ;
        CommonService.getAll('userGroupManage', uriData,
            function(data)
            {
                $scope.serviceAdminGroup = listToObject(data, 'rows').rows[0];
            },
            function(data,status)
            {
                onErrorResult(data);
            }
        );
    };


    // uploader 文件上传处理 
    var logoVUploader = $scope.logoVUploader = new FileUploader({
        url:uploadURL,
        method: 'POST',
        headers: { Authorization: sessionStorage.getItem('authToken'), },
        autoUpload: true,
        formData: [{ 'module' : 'SYSTEM', 'service_type' : 'logo_file'}]
    });

    logoVUploader.filters.push({
        name: 'customFilter',
        fn: function(item /*{File|FileLikeObject}*/, options)
        {
            return true; // this.queue.length < 10;
        }
    });

    logoVUploader.onAfterAddingAll = function(addedFileItems) 
    {
        $scope.isLoading = true;
        document.body.style.cursor = "wait";
    };
    
    logoVUploader.onBeforeUploadItem = function(item) 
    {
        // console.info('onBeforeUploadItem', item);
    };

    logoVUploader.onErrorItem = function (fileItem, response, status, headers) 
    {
        $scope.uploadError = true;
    };

    logoVUploader.onCompleteItem = function(fileItem, response, status, headers)
    {
        fileItem.response = response;
        $scope.systemUserItem.logo_vertical = "./resource/images/logo/" + response.file_name;
    };

    logoVUploader.onCompleteAll = function()
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
    };

    // uploader 文件上传处理
    var logoHUploader = $scope.logoHUploader = new FileUploader({
        url:uploadURL,
        method: 'POST',
        headers: { Authorization: sessionStorage.getItem('authToken'), },
        autoUpload: true,
        formData: [{ 'module' : 'SYSTEM', 'service_type' : 'logo_file'}]
    });

    logoHUploader.filters.push({
        name: 'customFilter',
        fn: function(item /*{File|FileLikeObject}*/, options)
        {
            return true; // this.queue.length < 10;
        }
    });

    logoHUploader.onAfterAddingAll = function(addedFileItems) 
    {
        $scope.isLoading = true;
        document.body.style.cursor = "wait";
    };
    
    logoHUploader.onBeforeUploadItem = function(item) 
    {
        // console.info('onBeforeUploadItem', item);
    };

    logoHUploader.onErrorItem = function (fileItem, response, status, headers) 
    {
        $scope.uploadError = true;
    };

    logoHUploader.onCompleteItem = function(fileItem, response, status, headers)
    {
        fileItem.response = response;
        $scope.systemUserItem.logo_horizontal = "./resource/images/logo/" + response.file_name;
    };

    logoHUploader.onCompleteAll = function()
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
    };

    // 保存数据
    $scope.saveData = function()
    {
        var strMD5Passwd = '';

        // 检查参数是否合法
        if($("#user_name").val() == '')
        {
            $("#user_name").testRemind("请输入系统用户中文名称");
            return;
        }

        if($("#user_name_en").val() == '')
        {
            $("#user_name_en").testRemind("请输入系统用户英文名称");
            return;
        }

        data = $scope.serviceAdminAccount.user_name;
        var reg = /^[a-zA-Z0-9]{5,12}$/;
        
        if ( $("#user_name").val() == '' || (!reg.test(data) && data) )
        {
            $("#user_name").testRemind("账户名必须为5~12位的数字或英文字母组成");
            return;
        }

        if ( $("#name").val() == '' )
        {
            $("#name").testRemind("请输入用户姓名");
            return;
        }

        if($("#group_c_name").val() == '')
        {
            $("#group_c_name").testRemind("请输入系统用户用户组中文名称");
            return;
        }

        if($("#group_name").val() == '')
        {
            $("#group_name").testRemind("请输入系统用户用户组英文名称");
            return;
        }

        if ( $("#password").val() == "" && $scope.editType == "新建") 
        {
            $("#password").testRemind("密码长度不能小于6位");
            return;
        }

        if ($("#password1").val() != $("#password").val()) 
        {
            $("#password1").testRemind("前后密码不一致");
            return;
        }

        if( $scope.serviceAdminAccount.password != null )
        {
            strMD5Passwd = CryptoJS.MD5( $scope.serviceAdminAccount.user_name + $scope.serviceAdminAccount.password ).toString();
            $scope.serviceAdminAccount.pass     = strMD5Passwd;
        }
        else
        {
            if ( $scope.editType=='新建' )
            {
                $("#password").testRemind("请输入密码");
                return;
            }
        }


        $scope.systemUserItem.groupInfo     = $scope.serviceAdminGroup;

        if ( $scope.serviceAdminAccount.addr_detail1 == null )
            $scope.serviceAdminAccount.addr_detail1 = "";

        if ( $scope.serviceAdminAccount.addr_detail2 == null )
            $scope.serviceAdminAccount.addr_detail2 = "";
        
        $scope.serviceAdminAccount.address1 = GetAreaInfo("addr1") + $scope.serviceAdminAccount.addr_detail1;
        $scope.serviceAdminAccount.address2 = GetAreaInfo("addr2") + $scope.serviceAdminAccount.addr_detail2;

        $scope.systemUserItem.accountInfo   = $scope.serviceAdminAccount;

        var treeObj = $.fn.zTree.getZTreeObj("menu_tree");
        var menuList = [];
        if(treeObj)
        {
            var nodes = treeObj.getCheckedNodes(true);
            for(var i = 0; i < nodes.length; i++)
                menuList.push(nodes[i].id);
        }
        if ( menuList.length == 0 )
        {
            $("#sel_menu_tree").testRemind("请选择至少一个菜单！");
            return;
        }

        $scope.systemUserItem.menuList = menuList;

        var uriData = angular.toJson($scope.systemUserItem);
        if ( $scope.editType == "新建")
            $scope.saveInfo( CommonService.createOne, uriData );
        else
            $scope.saveInfo( CommonService.updateOne, uriData );
    }

    $scope.saveInfo = function( CommonServiceFunc, uriData )
    {
        CommonServiceFunc("systemUserManage", uriData, 
            function(data)
            {
                MsgBox.info("提示", $scope.editType + " " + $scope.systemUserItem.user_name + " 系统用户成功！");

                // 保存成功后返回
                $scope.goback();
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    $scope.goback = function()
    {
        history.back();
    }
    
    var setting = 
    {
        check : { enable : true },
        data  : {},
        key   : { name : 'name', children : 'children' }
    };

    $scope.loader = null;
    $scope.getAllMenu = function()
    {
        if( $scope.editType == "新建" ) 
            var uriData = { op: 'menu', sid:0 }; 
        else
            var uriData = { op: 'menu',  sid: $scope.systemUserItem.id }; 

        uriData = angular.toJson(uriData);
        $scope.loader = new Loader('sel_menu_tree');
        CommonService.updatePartOne("systemUserManage", uriData, 
            function(data)
            {
                $scope.loader.close();
                $scope.treedata = data;
                $.fn.zTree.init($("#menu_tree"), setting, $scope.treedata);
            },
            function(data, status)
            {
                $scope.loader.close();
                onErrorResult(data);
            }
        );
    }

    $scope.SelectAllButton = "全选";
    $scope.selectAll = function()
    {
        if($scope.SelectAllButton == "全选")
        {
            if( $scope.treedata )
            {
                $scope.checkAllNode( $scope.treedata, true );
                $.fn.zTree.init($("#menu_tree"), setting, $scope.treedata);
                $scope.SelectAllButton = "清空";
            }
        }
        else
        {
            if( $scope.treedata )
            {
                $scope.checkAllNode($scope.treedata, false);
                $.fn.zTree.init($("#menu_tree"), setting, $scope.treedata);
                $scope.SelectAllButton = "全选";
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




