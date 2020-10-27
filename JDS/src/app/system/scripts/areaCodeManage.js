
angular.module('areaCodeManageControllers', ['treeControl'])

// 区域代码管理
.controller('areaCodeManageCtrl', function($scope, $window, $rootScope, CommonService)
{
    $scope.user = null;
    if(sessionStorage.getItem("strUser"))
        $scope.user=JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"
    
    $scope.currentArea  = {id:0 };                                         // 当前正在编辑的菜单

    $scope.editMode = '';
    $scope.editType = '';

    var currentNode = {};
    $scope.updateNodes = function (flag) 
    {
        treeName = "area_tree";

        var treeObj = $.fn.zTree.getZTreeObj(treeName);
        currentNode.highlight = flag;
        treeObj.updateNode(currentNode);
    }

    // 菜单树节点被点击
    $scope.onTreeClicked = function(event, treeId, treeNode) 
    {
        if ( treeNode == undefined || treeNode.id == 0 )
            return;

        $scope.currentArea = {
            id              : treeNode.id,
            area_type       : treeNode.area_type,
            area_type_name  : treeNode.area_type_name,
            area_code       : treeNode.area_code,
            area_name       : treeNode.area_name, 
            area_name_en    : treeNode.area_name_en,
            parent_area_id  : treeNode.parent_area_id,
            parent_area_name: treeNode.parent_area_name,
            flag            : treeNode.flag,
        }

        $scope.updateNodes(false);
        currentNode = treeNode;
        $scope.updateNodes(true); 

        $scope.__apply();
    }

    $scope.__apply = function () 
    {
        if (!$rootScope.$$phase)
            $scope.$digest();
    };

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

    // 获取所有区域列表
    $scope.initialData = function()
    {
        // 数据已经获取保存
        
        data = JSON.parse(sessionStorage.getItem("areaTree"));

        $scope.loader = new Loader("sel_area_tree");
        $scope.loader.close();
        $.fn.zTree.init($("#area_tree" ), setting, data);

        if ( data.children.length > 0 )
        {
            var treeObj = $.fn.zTree.getZTreeObj("area_tree");
            NodeName = data.children[0].name;
            Node = treeObj.getNodesByParamFuzzy("name", NodeName, null)[0];
            $scope.onTreeClicked( null, null, Node);
        }

    }
    
    // 添加国家
    $scope.addCountry = function()
    {
        $scope.editMode     = '新增';
        $scope.editedArea = { 
            area_type       : 'S',
            area_type_name  : '国家',
            area_code       : '',
            area_name       : '',
            area_name_en    : '',
            parent_area_id  : 0,
            parent_area_name: '',
            flag            : '',
        };

        $("#addPage").modal("show");
    }

    // 添加区域
    $scope.addArea = function()
    {
        if($scope.currentArea == null)
        {
            MsgBox.info("提示", "请先选择区域");
            return;
        }

        if ( $scope.currentArea.area_type == 'S')
        {
            area_type = 'P';
            area_type_name = "省份";
        }
        else if ( $scope.currentArea.area_type == 'P')
        {
            area_type = 'C';
            area_type_name = "地市";
        }
        else if ( $scope.currentArea.area_type == 'C')
        {
            area_type = 'D';
            area_type_name = "区县";
        }
        else if ( $scope.currentArea.area_type == 'D')
        {
            MsgBox.info("提示", "不能向区县添加下级区域！");
            return;
        }
        else
        {
            MsgBox.info("提示", "系统错误：当前区域类型 " + $scope.currentArea.area_type + " ！");
            return;
        }

        $scope.editMode     = '新增';
        $scope.editedArea = {
            area_type       : area_type,
            area_type_name  : area_type_name,
            area_code       : '',
            area_name       : '',
            area_name_en    : '',
            parent_area_id  : $scope.currentArea.id,
            parent_area_name: $scope.currentArea.area_name,
        };

        $("#addPage").modal("show");
    }

    // 更新区域
    $scope.updateArea = function()
    {
        if($scope.currentArea == null)
        {
            MsgBox.info("提示", "请先选择要更新的区域");
            return;
        }

        $scope.editMode     = '更新';
        $scope.editedArea = {
            id              : $scope.currentArea.id,
            area_code       : $scope.currentArea.area_code,
            area_type_name  : $scope.currentArea.area_type_name,
            area_name       : $scope.currentArea.area_name,
            area_name_en    : $scope.currentArea.area_name_en,
            area_type       : $scope.currentArea.area_type,
            parent_area_id  : $scope.currentArea.parent_area_id,
            parent_area_name: $scope.currentArea.parent_area_name,
            flag            : $scope.currentArea.flag,
        };

        $("#addPage").modal("show");
    }

    $scope.confirmToSave = function()
    {
        // 检查参数是否合法
        if($("#area_code").val() == '')
        {
            $("#area_code").testRemind("请输入地区编码");
            return;
        }

        if($("#area_name").val() == '')
        {
            $("#area_name").testRemind("请输入地区中文名称");
            return;
        }

        if($("#area_name_en").val() == '')
        {
            $("#area_name_en").testRemind("请输入地区英文名称");
            return;
        }

        var uriData = angular.toJson($scope.editedArea);            

        if ( $scope.editMode == "新增")
            $scope.saveInfo( CommonService.createOne, uriData );
        else
            $scope.saveInfo( CommonService.updateOne, uriData );
    }

    $scope.saveInfo = function(CommonServiceFunc, uriData)
    {
        CommonServiceFunc( "areaCodeManage", uriData, 
            function(data)
            {
                MsgBox.info("提示", $scope.editMode + " " + $scope.editedArea.area_name + " 成功！");

                $("#addPage").modal("hide");
                $scope.initialData();
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    // 删除菜单
    $scope.showDeleteArea = function()
    {
        MsgBox.confirm("确认", "确认删除区域 " + $scope.currentArea.area_name + " 吗？", 
            function ( isConformed ) 
            {
                if (isConformed) 
                    $scope.deleteArea();
            }
        );
    }

    $scope.deleteArea = function()
    {
        var uriData = JSON.stringify($scope.currentArea);

        CommonService.deleteOne('areaCodeManage', uriData, 
            function(data)
            {
                MsgBox.info("提示", "删除区域成功！");
                $scope.initialData();        
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

})

