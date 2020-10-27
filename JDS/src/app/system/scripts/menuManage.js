var meunsManageControllers = angular.module('menusManageControllers', ['treeControl']);

// 菜单管理
meunsManageControllers.controller('menuManageCtrl', function($scope, $window, CommonService)
{
    CommonService.heartbeat();
    $scope.user = null;
    if(sessionStorage.getItem("strUser"))
        $scope.user=JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"
    
    $scope.currentMenu  = null;                                         // 当前正在编辑的菜单
    $scope.menuItemList = [];                                           // 当前菜单的菜单项列表

    $scope.currentMenu = { id : 0};

    $scope.editMode = '';
    $scope.editType = '';

    $scope.showMenuItem = 1;

    var currentNode = {};
    $scope.updateNodes = function (flag) 
    {
        treeName = "menu_tree";

        var treeObj = $.fn.zTree.getZTreeObj(treeName);
        currentNode.highlight = flag;
        treeObj.updateNode(currentNode);
    }

    // 菜单树节点被点击
    $scope.onTreeClicked = function(event, treeId, treeNode) 
    {
        if ( treeNode == undefined || treeNode.id == 0 )
            return;

        $scope.currentMenu = {
            id              : treeNode.id,
            code            : treeNode.code,
            name            : treeNode.name, 
            sort            : treeNode.sort,
            tier            : treeNode.tier,
            parent_id       : treeNode.parent_id,
            parent_code     : treeNode.parent_code,
            parent_name     : treeNode.parent_name,
        }

        $scope.getMenuItem($scope.currentMenu);
        if ( treeNode.children.length == 0 )
            $scope.showMenuItem = 1;
        else
            $scope.showMenuItem = 0;

        $scope.updateNodes(false);
        currentNode = treeNode;
        $scope.updateNodes(true); 
    }

    var setting = 
    {
        check   : { enable: false },
        data    : {},
        key     : { name:'name', children:'children' },
        callback: { onClick: $scope.onTreeClicked },
        view    :
        {
            fontCss: function (treeId, treeNode) 
            {
                selectedItemColor = {color: "red", "font-weight": "bold"};
                nonSelectedItemColor = 
                {
                    color           : 'black',
                    "font-weight"   : "normal",
                };
                return (!!treeNode.highlight) ? selectedItemColor : nonSelectedItemColor;
            }
        }
    };

    // 获取所有菜单列表
    $scope.initialData = function()
    {
        $scope.loader = new Loader("sel_menu_tree");
        CommonService.getAll("menuManage", null, 
            function(data)
            {
                $scope.loader.close();
                $.fn.zTree.init($("#menu_tree" ), setting, data);

                if ( data.children.length > 0 )
                {
                    var treeObj = $.fn.zTree.getZTreeObj("menu_tree");
                    NodeName = data.children[0].name;
                    Node = treeObj.getNodesByParamFuzzy("name", NodeName, null)[0];
                    $scope.onTreeClicked( null, null, Node);
                }
            },
            function(data, status)
            {
                MsgBox.info("提示", '获取菜单失败');
            }
        );
    }
    
    // 获取菜单的菜单项数据
    $scope.getMenuItem = function(menu)
    {
        $scope.currentMenu = menu;
        var uriData = "mid=" + $scope.currentMenu.id;

        // 加载MENU_ITEM
        CommonService.getAll("menuItemManage", uriData, 
            function(data)
            {
                var obj = listToObject(data,'rows');
                $scope.menuItemList = obj.rows;
            }, 
            function()
            {
                MsgBox.info("提示", '获取菜单项失败');
            }
        );
    }

    // 添加根目录菜单
    $scope.addRootMenu = function()
    {
        $scope.editMode     = '新增';
        $scope.editType     = '菜单';
        $scope.editedMenu = { 
            code        : '',
            name        : '',
            sort        : 1,
            tier        : 0,
            parent_id   : 0,
            parent_code : '',
            parent_name : '',
        };

        $("#addPage").modal("show");
    }

    // 添加子菜单
    $scope.addMenu = function()
    {
        if($scope.currentMenu == null)
        {
            MsgBox.info("提示", "请先选择父菜单");
            return;
        }

        $scope.editMode     = '新增';
        $scope.editType     = '菜单';
        $scope.editedMenu = { 
            code        : '',
            name        : '',
            sort        : 1,
            tier        : $scope.currentMenu.tier + 1,
            parent_id   : $scope.currentMenu.id,
            parent_code : $scope.currentMenu.code,
            parent_name : $scope.currentMenu.name,
        };

        $("#addPage").modal("show");
    }

    // 更新菜单
    $scope.updateMenu = function()
    {
        if($scope.currentMenu == null)
        {
            MsgBox.info("提示", "请先选择要操作的菜单");
            return;
        }

        $scope.editMode     = '更新';
        $scope.editType     = '菜单';
        $scope.editedMenu = {
            id          : $scope.currentMenu.id,
            code        : $scope.currentMenu.code,
            name        : $scope.currentMenu.name,
            sort        : $scope.currentMenu.sort,
            tier        : $scope.currentMenu.tier,
            parent_id   : $scope.currentMenu.parent_id,
            parent_code : $scope.currentMenu.parent_code,
            parent_name : $scope.currentMenu.parent_name,
        };

        $("#addPage").modal("show");
    }

    // 增加菜单项
    $scope.addMenuItem = function()
    {
        if($scope.currentMenu == null)
        {
            MsgBox.info("提示", "请先选择要操作的菜单");
            return;
        }

        if ( $scope.menuItemList.length > 0 )
        {
            MsgBox.info("提示", "当期模式下，每个菜单只能有一个菜单项！");
            return;
        }

        $scope.editMode     = '新增';
        $scope.editType     = '菜单项';
        $scope.editedMenuItem = {
            menu_code   : $scope.currentMenu.code,
            menu_id     : $scope.currentMenu.id,
            menu_name   : $scope.currentMenu.name,
            path        : '',
        };

        $("#addPage").modal("show");
    }

    // 更新菜单项
    $scope.updateMenuItem = function(item)
    {
        $scope.editMode     = '更新';
        $scope.editType     = '菜单项';
        $scope.editedMenuItem = {
            id          : item.id,
            menu_code   : item.menu_code,
            menu_id     : item.menu_id,
            menu_name   : item.menu_name,
            path        : item.path,
        };

        $("#addPage").modal("show");
    }

    $scope.confirmToSave = function()
    {
        if ( $scope.editType == '菜单')
        {
            // 检查参数是否合法
            if($("#menu_code").val() == '')
            {
                $("#menu_code").testRemind("请输入菜单编码");
                return;
            }

            if($("#menu_name").val() == '')
            {
                $("#menu_name").testRemind("请输入菜单名称");
                return;
            }

            if($("#menu_sort").val() == '')
            {
                $("#menu_sort").testRemind("请输入菜单排序");
                return;
            }

            var uriData = angular.toJson($scope.editedMenu);            
            routePath = "menuManage";
        }
        else
        {
            if($("#item_path").val() == '')
            {
                $("#item_path").testRemind("请输入菜单项路径");
                return;
            }

            var uriData = angular.toJson($scope.editedMenuItem);            
            routePath = "menuItemManage";
        }


        if ( $scope.editMode == "新增")
            $scope.saveInfo( CommonService.createOne, routePath, uriData );
        else
            $scope.saveInfo( CommonService.updateOne, routePath, uriData );
    }

    $scope.saveInfo = function(CommonServiceFunc, routePath, uriData)
    {
        CommonServiceFunc( routePath, uriData, 
            function(data)
            {
                MsgBox.info("提示", $scope.editMode + " " + $scope.editType + " 成功！");

                $("#addPage").modal("hide");
                if ( $scope.editType == '菜单')
                    $scope.initialData();
                else
                    $scope.getMenuItem($scope.currentMenu);
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    // 删除菜单
    $scope.showDeleteMenu = function()
    {
        MsgBox.confirm("确认", "确认删除菜单 " + $scope.currentMenu.name + " 吗？", 
            function ( isConformed ) 
            {
                if (isConformed) 
                    $scope.deleteMenu();
            }
        );
    }

    $scope.deleteMenu = function()
    {
        var uriData = JSON.stringify($scope.currentMenu);

        CommonService.deleteOne('menuManage', uriData, 
            function(data)
            {
                MsgBox.info("提示", "删除菜单成功！");
                $scope.initialData();                
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    // 删除菜单项
    $scope.showDeleteMenuItem = function( item )
    {
        MsgBox.confirm("确认", "确认删除菜单项吗？", 
            function ( isConformed ) 
            {
                if (isConformed) 
                    $scope.deleteMenuItem( item );
            }
        );
    }

    $scope.deleteMenuItem = function(delItem)
    {
        var uridata = JSON.stringify(delItem);

        CommonService.deleteOne("menuItemManage", uridata,
            function(data)
            {
                MsgBox.info("提示", "删除菜单项成功");

                // 重新加载菜单项
                $scope.getMenuItem($scope.currentMenu);
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    };
})

