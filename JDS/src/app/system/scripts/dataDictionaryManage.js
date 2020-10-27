angular.module("dictManageControllers", [])

.controller("dictManageCtrl", function ($scope, $http, $location, $rootScope, CommonService, $route, FileUploader) 
{
    CommonService.heartbeat();
    $scope.user=null;
    if(sessionStorage.getItem("strUser"))
        $scope.user=JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"

    $scope.currentDictType = {};

    //获取所有字典列表和值
    $scope.initialData = function () 
    {
        var uriData = '';
        CommonService.getAll('dataDictionaryType', uriData, 
            function (data)
            {
                var objData = listToObject(data, 'rows').rows;
                $scope.dictTypeList = objData;
                if ( $scope.dictTypeList.length > 0 )
                {
                    $scope.getDictTypeValue( $scope.dictTypeList[0]);
                }
            },
            function (data, status) 
            {
            }
        );
    }

    $scope.getDictTypeValue = function (dictItem) 
    {
        $scope.currentDictType = dictItem;
        var uriData = 'type_code=' + dictItem.code;
        CommonService.getAll("dataDictionaryValue", uriData, 
            function (data) 
            {
                var objData = listToObject(data, 'rows').rows;
                $scope.dictItemList = objData;
                for (var i = $scope.dictItemList.length - 1; i >= 0; i--) 
                {
                    if ($scope.dictItemList[i].status == '1') 
                        $scope.dictItemList[i].status_name = '启用';
                    else 
                        $scope.dictItemList[i].status_name = '停用';
                }        
            }, 
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    // 更新字典类型
    $scope.updateDictType = function () 
    {
        if ($scope.currentDictType == null) 
        {
            MsgBox.Info("提示", "请先选择要操作的字典");
            return;
        }

        $scope.editMode = '更新';
        $scope.editType = '字典类型';

        $scope.editedDictType = {
            id          : $scope.currentDictType.id,
            code        : $scope.currentDictType.code,
            name        : $scope.currentDictType.name,
            status      : $scope.currentDictType.status,
            description : $scope.currentDictType.description,
        }

        var radioInputList = document.getElementsByName("dict_type_status");
        if ($scope.currentDictType.status == '1')
            index = 0;
        else
            index = 1;

        radioInputList[index].checked = true;

        $("#addPage").modal("show");
    }

    // 更新字典类型条目
    $scope.updateDictTypeItem = function ( dictItemValue) 
    {
        $scope.editMode = '更新';
        $scope.editType = '字典条目';
        $scope.editedDictItem = {
            id          : dictItemValue.id,
            type_id     : dictItemValue.type_id,
            type_code   : dictItemValue.type_code,
            code        : dictItemValue.code,
            name        : dictItemValue.name,
            sort        : dictItemValue.sort,
            status      : dictItemValue.status,
            description : dictItemValue.description,
        }

        var radioInputList = document.getElementsByName("dict_item_status");
        if ($scope.editedDictItem.status == '1')
            index = 0;
        else
            index = 1;

        radioInputList[index].checked = true;

        $("#addPage").modal("show");
    }

    // 增加新的字典类型
    $scope.createDictType = function () 
    {
        $scope.editMode     = '新增';
        $scope.editType     = '字典类型';
        $scope.editedDictType = {status:'1'};
        $("#addPage").modal("show");
    }

    // 增加新的字典条目
    $scope.createDictTypeItem = function () 
    {
        $scope.editMode = '新增';
        $scope.editType = '字典条目';

        $scope.editedDictItem = { 
            type_code: $scope.currentDictType.code, 
            type_id : $scope.currentDictType.id,
            status: '1'
        };

        $("#addPage").modal("show");
    }

    $scope.confirmToSave = function()
    {
        var dictItem = {};
        if ( $scope.editType == '字典类型' )
        {
            // 检查参数是否合法
            if($("#dict_code").val() == '')
            {
                $("#dict_code").testRemind("请输入字典类型编码");
                return;
            }

            if($("#dict_name").val() == '')
            {
                $("#dict_name").testRemind("请输入字典类型名称");
                return;
            }

            if($("#dict_desc").val() == '')
            {
                $("#dict_desc").testRemind("请输入字典类型描述");
                return;
            }

            var uriData = angular.toJson($scope.editedDictType);            
            routePath = "dataDictionaryType";
        }
        else
        {
            // 检查参数是否合法
            if($("#item_code").val() == '')
            {
                $("#item_code").testRemind("请输入字典条目编码");
                return;
            }

            if($("#item_name").val() == '')
            {
                $("#item_name").testRemind("请输入字典条目名称");
                return;
            }

            if($("#item_desc").val() == '')
            {
                $("#item_desc").testRemind("请输入字典条目描述");
                return;
            }

            var uriData = angular.toJson($scope.editedDictItem);
            routePath = "dataDictionaryValue";
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
                MsgBox.info("提示", $scope.editMode + " 字典成功！");

                $("#addPage").modal("hide");
                if ( $scope.editType == '字典类型')
                    $scope.initialData();
                else
                    $scope.getDictTypeValue($scope.currentDictType);
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

    // 删除字典类型
    $scope.showDeleteDictTypeDlg = function () 
    {
        if ($scope.currentDictType == null) 
        {
            MsgBox.info("提示", "请先选择要删除的字典");
            return;
        }

        MsgBox.confirm("提示", "确定删除数据字典类型 " + $scope.currentDictType.name + " 吗？", 
            function( isConfirm )
            {
                if ( isConfirm )
                {
                    var uriData = angular.toJson($scope.currentDictType);
                    CommonService.deleteOne("dataDictionaryType", uriData, 
                        function (data) 
                        {
                            MsgBox.info("提示", "删除字典成功");
                            $scope.initialData();
                        }, 
                        function () 
                        {
                            $scope.delItem = null;
                            $scope.tishiMessage = '删除失败!';
                            $("#delResult").modal("show");
                        }
                    );
                }
            }
        );
    }

    // 删除字典类型条目
    $scope.showDeleteDictValueDlg = function (dictItem) 
    {
        MsgBox.confirm("提示", "确定删除数据字典值 " + dictItem.name + " 吗？", 
            function( isConfirm )
            {
                if ( isConfirm )
                {
                    var uriData = angular.toJson(dictItem);
                    CommonService.deleteOne("dataDictionaryValue", uriData, 
                        function (data)
                        {
                            MsgBox.info("提示", "删除字典值成功！");
                            $scope.getDictTypeValue($scope.currentDictType);
                        },
                        function () 
                        {
                            MsgBox.info("提示", "删除字典值失败！");
                        }
                    );
                }
            }
        );
    }
})

