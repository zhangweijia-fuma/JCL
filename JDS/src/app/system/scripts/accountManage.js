angular.module('accountControllers', [])

////////////////////////////////////////////////////////////////////////////////////////////////
// 账户管理主页
.controller("accountManageCtrl", function ($scope, $location, $rootScope, CommonService, $route)
{
    CommonService.heartbeat();
    $scope.user = null;

    if(sessionStorage.getItem("strUser"))
        $scope.user=JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"

    // 检查用户是否有权限

    $scope.accounts = "";
    $scope.pager = new Pager(null);
    $scope.pager.pageIndex = 1;
    $scope.pager.pageSize  = 10;


    $scope.companyName  = "选择全部";
    $scope.deptName     = "选择全部";
    $scope.groupID      = 0;
    $scope.userID       = 0;

    // 下拉列表初始化
    $scope.initialListCtrl = function()
    {
        uriData = { op: "list", id:$scope.user.id};
        if ( $scope.companyName != "选择全部" )
            uriData.cn = $scope.companyName;

        if ( $scope.deptName != "选择全部" )
            uriData.dn = $scope.deptName;
        
        if ( $scope.groupID != 0 )
            uriData.gid = $scope.groupID;

        if ( $scope.userID != 0 )
            uriData.uid = $scope.userID;

        uriData = angular.toJson(uriData);
        CommonService.updatePartOne('accountManage',uriData,
            function(data)
            {
                $scope.companyList  = listToObject(data.companyList, 'rows').rows;
                $scope.deptList     = listToObject(data.deptList, 'rows').rows;
                $scope.groupList    = listToObject(data.groupList, 'rows').rows;
                $scope.userNameList = listToObject(data.userList, 'rows').rows;
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );  
    }
    $scope.initialListCtrl();

    $scope.searchByCompany = function()
    {
        $scope.pager.pageIndex = 1;
        $scope.companyName = $scope.companyList.selected.name;
        $scope.initialData();
    }

    $scope.searchByDept = function()
    {
        $scope.pager.pageIndex = 1;
        $scope.deptName = $scope.deptList.selected.name;
        $scope.initialData();
    }

    $scope.searchByGroup = function()
    {
        $scope.pager.pageIndex = 1;
        $scope.groupID = $scope.groupList.selected.id;
        $scope.initialData();
    }

    $scope.searchByName = function()
    {
        $scope.pager.pageIndex = 1;
        $scope.userID = $scope.userNameList.selected.id;
        $scope.initialData();
    }

    $scope.createAcount = function()
    {
        ClearAreaInfo("addr1");
        ClearAreaInfo("addr2");

        sessionStorage.removeItem("accountUpdate");
        
        window.location.href = '#/update_account';
    }

    $scope.initialData = function () 
    {
        var uriData = "id=" + $scope.user.id + "&o=" + $scope.pager.pageIndex + "&r=" + $scope.pager.pageSize;
        if($scope.companyName != "选择全部")
            uriData += "&cn=" + $scope.companyName;

        if($scope.deptName != "选择全部")
            uriData += "&dn=" + $scope.deptName;
        
        if($scope.groupID != 0)
            uriData += "&gid=" + $scope.groupID;

        if($scope.userID != 0)
            uriData += "&uid=" + $scope.userID;

        CommonService.getAll('accountManage',uriData,
            function(data) 
            {
                $scope.accountList  = listToObject(data,'rows').rows;
                $scope.count        = data.count;
                $scope.totalPage    = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
                $scope.pager.update(data.count);

                for (var i = 0; i < $scope.accountList.length; i++) 
                {
                    if ( $scope.accountList[i].name == "管理员" )
                        $scope.accountList[i].deleteFlag = 0;
                    else
                        $scope.accountList[i].deleteFlag = 1;
                };
            },
            function(data,status)
            {//失败时的回调函数
                onErrorResult(data);
            }
        );
    };

    $scope.page = function(e)
    {
        $scope.pager.onEvent(e);
        $scope.initialData();
    };

    //生失效操作
    $scope.changeAccountStatus = function( account )
    {
        var newStatus = 0;
        if(account.status_name == "生效")
        { //生效时 
            account.status_name = "失效";
            newStatus = 1;
        }
        else
        {    
            account.status_name=="生效";
            newStatus = 0;
        }

        var uriData = {id:account.id, user_name:account.user_name, status:newStatus, update_id:$scope.user.id };
        uriData = JSON.stringify(uriData);

        CommonService.updateOne('accountManage',uriData,
            function(data)
            {
                $scope.initialData();
            },
            function()
            {
                onErrorResult(data);
            }
        );
    }

    $scope.change = function(status)
    {
        if (status == "生效")
            return "失效";
        else   
            return "生效";
    }

    //删除提示框
    $scope.showDeleteAccount = function(account)
    {
        MsgBox.confirm("确认", "确认删除此账号吗？", 
            function ( isConformed ) 
            {
                if (isConformed) 
                    $scope.deleteAccount(account);
            }
        );
    };

    //删除账户信息
    $scope.deleteAccount = function(account)
    {
        var uriData = JSON.stringify( account );
        CommonService.deleteOne('accountManage', uriData,
            function(data)
            {
                MsgBox.info("提示", '删除账户成功');
                $scope.pager.pageIndex = 1;
                $scope.initialData();
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    };


    $scope.updateAccountInfo = function(account)
    {
        ClearAreaInfo("addr1");
        ClearAreaInfo("addr2");

        var addrList = [];
        addrList.push(account.address1);
        addrList.push(account.address2);

        var uriData;

        uriData = { op: "area", addr: addrList };

        uriData = angular.toJson(uriData);
        CommonService.updatePartOne("areaCodeManage", uriData,
            function(data)
            {
                addr1 = data.Address[0];
                SetAreaInfo("addr1", addr1);
                if ( addr1 != null )
                    account.addr1_detail = addr1.detailAddr;
                
                addr2 = data.Address[1];
                if ( addr2 != null )
                    account.addr2_detail = addr2.detailAddr;
                
                SetAreaInfo("addr2", addr2);
                
                sessionStorage.setItem('accountUpdate', JSON.stringify(account));
                window.location.href = '#/update_account';
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

})

// //////////////////////////////////////////////////////////////////////////////////////////////
// // 新增账户
// .controller("accountAddCtrl", function ($scope, $location, $rootScope, CommonService, $route)
// {
//     CommonService.heartbeat();
//     $scope.user = null;
//     if(sessionStorage.getItem("strUser"))
//         $scope.user=JSON.parse(sessionStorage.getItem("strUser"));
//     else
//         window.location.href = "/login.html"

//     $scope.isBack   = false;

//     // 初始化账户默认部分信息
//     $scope.account = {};
//     $scope.account.photo_path = "resource/images/default.png";
//     $scope.account.user_type  = 0; // 默认为本单位人员
//     $scope.account.status     = 0;
//     $scope.account.sex        = '1';
    

//     $scope.goback = function()
//     {
//         history.back();
//     }
    
//     $scope.toHistory=function()
//     {
//         if ( $scope.isBack == true )
//             history.back();
//     }

//     $scope.initialData = function()
//     {
//         // 获取用户组清单
//         uriData = {id:$scope.user.id};
//         uriData = angular.toJson(uriData);
//         CommonService.updatePartOne('accountManage',uriData,
//             function(data)
//             {
//                 $scope.groupList = listToObject(data.groupList, 'rows').rows;
//                 $scope.groupList.splice( 0, 1 );
//             },
//             function(data,status)
//             {
//                 onErrorResult(data);
//             }
//         );  
//     }

//     var userGroupList = [];
//     $scope.selectUserGroup = function()
//     {
//         var data = new Array();
//         // 现有用户组
//         grouplist = $("#grouplist").val();
//         if ( grouplist )
//         {
//             data = $("#grouplist").val().split("，"); 
//             var liArray = document.getElementsByName("group");
//             check_val = data;
//             for( i = 0; i < liArray.length; i++ )
//             {
//                 for( j = 0; j < data.length; j++ )
//                 {
//                     if( liArray[i].value == data[j] )
//                     {
//                         liArray[i].checked = true;  
//                         break;
//                     }
//                 }
//             }  
//         }

//         $("#selectUserGroup").modal("show");
//     }

//     $scope.saveUserGroup = function()
//     {
//         var obj     = document.getElementsByName("group");             
//         check_val   = [];  
//         check_val1  = []; 
//         for( k in obj )
//         {
//             if( obj[k].checked )
//             {
//                 check_val.push( obj[k].id );
//                 check_val1.push( obj[k].value );
//             }
//         }

//         if( check_val.length == 0 )
//         {
//             document.getElementById("warning").style.color="red";
//             return;
//         }
//         else
//         {
//             document.getElementById("warning").style.color="#fff";
//             $("#selectUserGroup").modal("hide");
//         }

//         document.getElementById("grouplist").value = check_val1;
//         document.getElementById("grouplist").title = check_val1;
//     }

//     $scope.changephoto = function()
//     {
//         sessionStorage.setItem("docimg","1");
//         sessionStorage.setItem("fromadd","5");
//         window.location.href='#/shearphotodoc';
//     };
    
//     // 保存账户信息，对账户名的唯一性检查在后端进行
//     $scope.save = function()
//     {   
//         // 检查参数是否合法
//         if($("#grouplist").val() == '')
//         {
//             $("#grouplist").testRemind("请选择用户组");
//             return;
//         }
        
//         data = $scope.account.user_name;
//         var reg = /^[a-zA-Z0-9]{5,12}$/;
        
//         if ( !reg.test(data) && data )
//         {
//             $("#user_name").testRemind("账户名必须为5~12位的数字或英文字符组成");
//             return;
//         }

//         if ( $("#password").val() == "" ) 
//         {
//             $("#password").testRemind("密码长度不能小于6位");
//             return;
//         }

//         if ($("#password1").val() != $("#password").val()) 
//         {
//             $("#password1").testRemind("前后密码不一致");
//             return;
//         }

//         var strMD5Passwd;

//         if( !$scope.account.password )
//             strMD5Passwd = '';
//         else
//             strMD5Passwd = CryptoJS.MD5( $scope.account.user_name + $scope.account.password ).toString();

//         $scope.account.pass = strMD5Passwd;
//         $scope.account.groupList = check_val;
        
//         var uriData = JSON.stringify( $scope.account );
//         CommonService.createOne("accountManage", uriData,
//             function(data)
//             {
//                 $scope.isBack = true;
//                 MsgBox.info("提示", "保存成功！", $scope.goback );
//             },
//             function(data)
//             {
//                 onErrorResult(data);
//             }
//         );
//     }
    
// })


////////////////////////////////////////////////////////////////////////////////////////////////
// 账户详情页面，登录进入时显示
.controller("accountDetailCtrl", function ($scope, $location, $rootScope, CommonService, $route)
{
    CommonService.heartbeat();
    $scope.user=null;

    if( sessionStorage.getItem("strUser") )
        $scope.user = JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"
   
    $scope.userInfo = {};
    if ( sessionStorage.getItem("accountUpdate") )
    {
        $scope.userInfo = JSON.parse(sessionStorage.getItem("accountUpdate"));
        sessionStorage.removeItem("accountUpdate");
    }
    else
        $scope.userInfo.id = $scope.user.id;
   
    $scope.updateAccountInfo = function()
    {
        sessionStorage.setItem("accountUpdate", JSON.stringify($scope.userInfo));
        window.location.href='#/update_account';
    }

    $scope.initialData = function()
    {
        var uriData = "id=" + $scope.user.id + "&uid=" + $scope.userInfo.id ;
        
        // 获取用户的个人系信息
        CommonService.getAll('accountManage', uriData, 
            function (data) 
            {
                $scope.userInfo = listToObject(data,'rows').rows[0];
            }, 
            function (data, status) 
            {
                // 失败时的回调函数
                onErrorResult(data);
            }
        );
    }

    $scope.changepass = function()
    {
       $("#changepassport").modal("show");
    }

    $("#changepass").html5Validate(function() 
    {   
        if(CryptoJS.MD5($scope.userInfo.user_name + $("#password").val()).toString() != $scope.userInfo.pass)
        {
            $("#password").testRemind("旧密码不正确");
            return;
        }
        
        if ($("#password1").val() != $("#password2").val()) 
        {
            $("#password2").testRemind("新密码不一致");
            return;
        }

        $("#changepassport").modal("hide");

        // 验证成功提交
        var strMD5Passwd = CryptoJS.MD5($scope.userInfo.user_name+$("#password1").val()).toString();
        var grouplist = [];
        for ( i = 0; i < $scope.user.groupList.length; i++)
            grouplist.push($scope.user.groupList[i][0]);

        uriData = 
        {
            id : $scope.userInfo.id, 
            pass : strMD5Passwd,
            groupList : grouplist, 
        };

        uriData = angular.toJson(uriData);

        CommonService.updateOne('accountManage', uriData,
            function(data)
            {
                MsgBox.info("提示", "修改密码成功！");

                setTimeout(function()
                {
                    window.location.href = "/login.html"
                },
                3000);
                /*history.back(-1);*/
            },
            function()
            {
                MsgBox.info("提示", "修改密码失败！");
            }
        );
    }, 
    {
    });
})

////////////////////////////////////////////////////////////////////////////////////////////////
// 账户新增或编辑页面
.controller("accountUpdateCtrl", function ($scope, $location, $rootScope, CommonService, $route)
{
    CommonService.heartbeat();
    $scope.user = null;

    if( sessionStorage.getItem("strUser") )
        $scope.user = JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"
   
    $scope.account = { id: 0};
    if ( sessionStorage.getItem("accountUpdate") )
    {
        $scope.account = JSON.parse(sessionStorage.getItem("accountUpdate"));
        $scope.editMode = "更新";
    }
    else
    {
        $scope.editMode             = "新增";
    }

    // 用户所在用户组的ID列表
    $scope.account.groupList = [];

    $scope.organizationList = [];

    $scope.initialListCtrl = function()
    {
        var uriData = { op: "list" };
        uriData = angular.toJson(uriData);

        CommonService.updatePartOne("organizationManage", uriData,
            function(data)
            {
                $scope.allOrganizationList    = listToObject( data.allOrganizationList, 'rows').rows;
                $scope.allOrganizationList.shift();
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
    $scope.initialListCtrl();

    $scope.initialData = function()
    {
        if( $scope.id == 0 )
        {
            return;
        }

        // 获取用户组清单
        uriData = {id:$scope.user.id, op: "list"};
        uriData = angular.toJson(uriData);
        CommonService.updatePartOne('accountManage',uriData,
            function(data)
            {
                $scope.groupList = listToObject(data.groupList, 'rows').rows;
                $scope.groupList.splice( 0, 1 );

                if ( $scope.editMode == "新增" )
                    return;

                // 根据用户组清单获得用户所在用户组的ID列表
                groupNameList = $scope.account.group_name_list.split("，"); 
                for ( i = 0; i < $scope.groupList.length; i++ )
                {
                    for( j = 0; j < groupNameList.length; j++ )
                    {
                        if ( $scope.groupList[i].name == groupNameList[j] )
                            $scope.account.groupList.push($scope.groupList[i].id);
                    }
                }
            },
            function(data,status)
            {
                onErrorResult(data);
            }
        );
    }

    var userGroupList = [];
    $scope.selectUserGroup = function()
    {
        var data = new Array();
        // 现有用户组
        grouplist = $("#grouplist").val();
        if ( grouplist )
        {
            data = $("#grouplist").val().split("，"); 
            var liArray = document.getElementsByName("group");
            check_val = data;
            for( i = 0; i < liArray.length; i++ )
            {
                for( j = 0; j < data.length; j++ )
                {
                    if( liArray[i].value == data[j] )
                    {
                        liArray[i].checked = true;  
                        break;
                    }
                }
            }  
        }

        $("#selectUserGroup").modal("show");
    }

    $scope.saveUserGroup = function()
    {
        var obj     = document.getElementsByName("group");             
        // check_val   = [];  
        check_val1  = [];
        $scope.account.groupList = [];
        for( k in obj )
        {
            if( obj[k].checked )
            {
                // check_val.push( obj[k].id );
                check_val1.push( obj[k].value );
                $scope.account.groupList.push( obj[k].id );
            }
        }

        if( check_val1.length == 0 )
        {
            document.getElementById("warning").style.color="red";
            return;
        }
        else
        {
            document.getElementById("warning").style.color="#fff";
            $("#selectUserGroup").modal("hide");
        }
       
        document.getElementById("grouplist").value = check_val1;
        document.getElementById("grouplist").title = check_val1;
    }

    $scope.changephoto = function()
    {
        sessionStorage.setItem("docimg","1");
        sessionStorage.setItem("fromadd","5");
        window.location.href='#/shearphotodoc';
    };
    
    // 保存账户信息，对账户名的唯一性、密码的合法性等检查在后端进行
    $scope.ConfirmToSave = function()
    {
        // 检查参数是否合法
        if($("#grouplist").val() == '')
        {
            $("#grouplist").testRemind("请选择用户组");
            return;
        }
        
        data = $scope.account.user_name;
        var reg = /^[a-zA-Z0-9]{5,12}$/;
        
        if ( !reg.test(data) && data )
        {
            $("#user_name").testRemind("账户名必须为5~12位的数字或英文字符组成");
            return;
        }

        if ( $scope.account.password != undefined && $scope.account.password.length < 6 ) 
        {
            $("#password").testRemind("密码长度不能小于6位");
            return;
        }

        if( $scope.account.password != null )
        {
            strMD5Passwd = CryptoJS.MD5( $scope.account.user_name + $scope.account.password ).toString();
            $scope.account.pass = strMD5Passwd;
        }
        else
        {
            if ( $scope.editMode=='新建' )
            {
                $("#password").testRemind("请输入密码");
                return;
            }
            else
            {
                // 没有修改密码
                delete $scope.account.pass;
            }
        }


        if ( $scope.account.addr1_detail == null )
            $scope.account.addr1_detail1 = "";

        if ( $scope.account.addr2_detail == null )
            $scope.account.addr2_detail = "";
        
        $scope.account.address1 = GetAreaInfo("addr1") + $scope.account.addr2_detail;
        $scope.account.address2 = GetAreaInfo("addr2") + $scope.account.addr2_detail;


        var uriData = JSON.stringify( $scope.account );
        if ( $scope.editMode == "新增")
            $scope.saveInfo( CommonService.createOne, uriData );
        else
            $scope.saveInfo( CommonService.updateOne, uriData );
    }

    $scope.saveInfo = function( CommonServiceFunc, uriData )
    {
        CommonServiceFunc("accountManage", uriData, 
            function(data)
            {
                MsgBox.info("提示", $scope.editMode + " " + $scope.account.user_name + " 用户成功！");

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
})

