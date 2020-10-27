////////////////////////////////////////////////////////////////////////////////////////////

angular.module('gpsModuleControllers', ['treeControl'])

//////////////////////////////////////////////////////////////////////////////////////////////
// 定位模块管理
.controller("gpsModuleManageCtrl", function ($scope, $location, $rootScope, CommonService, $route, FileUploader)
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

    $scope.codeList   = [];
    $scope.batchList  = [];

    $scope.installTypeList      = [];

    // 初始化下拉列表
    $scope.initListCtrl = function()
    {
        var uriData = { op: "data" };
        uriData = angular.toJson(uriData);

        CommonService.updatePartOne("gpsModuleManage", uriData,
            function(data)
            {
                $scope.codeList            = listToObject( data.codeList, 'rows').rows;
                $scope.batchList           = listToObject( data.batchList, 'rows').rows;
                $scope.installTypeList     = listToObject( data.installTypeList, 'rows').rows;

                $scope.deviceTypeList      = listToObject( data.deviceTypeList, 'rows').rows;
                $scope.selectInstallTypeList = $scope.installTypeList.slice(1);

            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
    $scope.initListCtrl();

    $scope.searchModule = function()
    {
        $scope.pager.pageIndex = 1;
        $scope.initialData();
    }

    $scope.initialData = function()
    {
        var uriData = "op=data&o=" + $scope.pager.pageIndex + "&r=" + $scope.pager.pageSize;
        if ( $scope.codeList.selected && $scope.codeList.selected.id > 0 )
            uriData += "&mid=" + $scope.codeList.selected.id;

        if ( $scope.batchList.selected && $scope.batchList.selected.batch != "选择全部" )
            uriData += "&batch=" + $scope.batchList.selected.batch;
        
        if ( $scope.installTypeList.selected && $scope.installTypeList.selected.name != "选择全部")
            uriData += "&it=" + $scope.installTypeList.selected.code;

        CommonService.getAll("gpsModuleManage", uriData, 
            function(data)
            {
                $scope.moduleList  = listToObject( data, 'rows').rows;
                $scope.count        = data.count;
                $scope.totalPage    = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
                $scope.pager.update(data.count);

                getDataDict($scope.moduleList, "device_type", "GPS_MODULE_DEVICE_TYPE", "device_type_name" );
                getDataDict($scope.moduleList, "install_type", "GPS_MODULE_INSTALL_TYPE", "install_type_name" );
                getDataDict($scope.moduleList, "status", "GPS_MODULE_STATUS", "status_name" );

                for ( i = 0; i < $scope.moduleList.length; i++)
                    $scope.moduleList[i].no = i + 1;
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
        if ( $scope.codeList.selected && $scope.codeList.selected.id > 0  )
            uriData += "&mcode=" + $scope.codeList.selected.code;

        if ( $scope.batchList.selected && $scope.batchList.selected.batch != "选择全部")
            uriData += "&batch=" + $scope.batchList.selected.batch;
        
        if ( $scope.installTypeList.selected && $scope.installTypeList.selected.name != "选择全部")
            uriData += "&it=" + $scope.installTypeList.selected.code;

        CommonService.getAll('gpsModuleManage', uriData,
            function(data)
            {
                var filename = "定位模块查询" + getNowFormatDate2("", "", "-") + ".xls";
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

    $scope.createGPSModule = function()
    {
        $scope.showModuleInfo = {
        };
        $scope.editMode = "新建";

        $("#addPage").modal("show");
    }

    $scope.updateGPSModule = function( item )
    {
        $scope.editMode = "更新";
        $scope.showModuleInfo = item;
        $("#addPage").modal("show");
    }

    $scope.confirmToSaveModuleInfo = function()
    {
        // 检查参数是否合法
        if($("#code").val() == '')
        {
            $("#code").testRemind("请输入定位模块编号");
            return;
        }

        if($("#name").val() == '')
        {
            $("#name").testRemind("请输入定位模块名称");
            return;
        }

        if ( $("#serial_no").val() == "" ) 
        {
            $("#serial_no").testRemind("请输入定位模块序列号");
            return;
        }

        if ( $scope.showModuleInfo.device_type == undefined ) 
        {
            $("#device_type").testRemind("请选择定位模块设备类型");
            return;
        }
        
        if($("#mac_addr").val() == '')
        {
            $("#mac_addr").testRemind("请输入定位模块MAC地址");
            return;
        }

        if($("#batch").val() == '')
        {
            $("#batch").testRemind("请输入定位模块安装批次");
            return;
        }

        if ( $scope.showModuleInfo.install_type == undefined ) 
        {
            $("#install_type").testRemind("请选择定位模块安装类型");
            return;
        }

        $("#addPage").modal("hide");

        if ( $scope.editMode == "新建")
            $scope.saveModuleInfo( CommonService.createOne, $scope.showModuleInfo );
        else if ( $scope.editMode == "更新")
        {
            $scope.showModuleInfo.op = "modify";
            $scope.saveModuleInfo( CommonService.updateOne, $scope.showModuleInfo );
        }

    }

    $scope.saveModuleInfo = function( CommonServiceFunc, item )
    {
        var uriData = angular.toJson( item);
        CommonServiceFunc("gpsModuleManage", uriData, 
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

    $scope.showDeleteModuleDlg = function(item)
    {
        MsgBox.confirm("确认", "确定要删除定位模块 " + item.code + " 吗？", 
            function ( isConformed ) 
            {
                if (isConformed) 
                    $scope.deleteModuleConfirmed( item );
            }
        );
    }

    $scope.deleteModuleConfirmed = function( item )
    {
        uriData = angular.toJson(item);

        CommonService.deleteOne("gpsModuleManage", uriData, 
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

    $scope.resetTime = function()
    {
        $scope.showModuleInfo.install_date = "";
    }

    // 批量导入
    
    // uploader
    $scope.uploadError = false;
    var uploader = $scope.uploader = new FileUploader({
        url:uploadURL,
        method: 'POST',
        headers: { Authorization: sessionStorage.getItem('authToken'), },
        autoUpload: true,
        formData: [{ 'module' : 'IOT', 'service_type' : 'iotModule'}]
    });

    uploader.filters.push({
        name: 'customFilter',
        fn: function(item /*{File|FileLikeObject}*/, options)
        {
            // 只能选择一个文件
            return this.queue.length < 1;
        }
    });

    uploader.onCompleteItem = function(fileItem, response, status, headers)
    {
        fileItem.response = response;
        $scope.batchFileID = response.id;
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

        $scope.processBatchFile();
    }

    $scope.processBatchFile = function()
    {
        var uriData = angular.toJson( { op:"upload", fid: $scope.batchFileID} );

        CommonService.updatePartOne("gpsModuleManage", uriData, 
            function(data)
            {
                // 删除后重新请求数据
                MsgBox.info("提示", "文件上传成功！");

                // 重新加载数据
                $scope.pager.pageIndex = 1;
                $scope.initialData();
            },
            function(data, status )
            {
                onErrorResult(data);
            }
        );

    };
})

// 定位模块位置地图
.controller('gpsModuleMapCtrl', function($scope, $http, $rootScope, $route, $window, CommonService)
{
    CommonService.heartbeat();
    $scope.user = null;
    if(sessionStorage.getItem("strUser"))
        $scope.user=JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html";

    $scope.codeList   = [];
    $scope.batchList  = [];

    $scope.installTypeList      = [];

    // 初始化下拉列表
    $scope.initListCtrl = function()
    {
        var uriData = { op: "data" };
        uriData = angular.toJson(uriData);

        CommonService.updatePartOne("gpsModuleManage", uriData,
            function(data)
            {
                $scope.codeList            = listToObject( data.codeList, 'rows').rows;
                $scope.batchList           = listToObject( data.batchList, 'rows').rows;
                $scope.installTypeList     = listToObject( data.installTypeList, 'rows').rows;
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
    $scope.initListCtrl();

    $scope.searchModule = function()
    {
        $scope.initialData();
    }

    // 百度地图
    var map = $scope.map = new BMap.Map("gpsModuleMap", { minZoom : 5}); 

    map.centerAndZoom("西安", 5);
    map.enableScrollWheelZoom();                            // 启用滚轮缩放
    map.enableContinuousZoom();                             // 启用地图惯性拖拽，默认禁用
    map.addControl(new BMap.NavigationControl());           // 添加默认缩放平移控件
    map.addControl(new BMap.OverviewMapControl());          // 添加默认缩略地图控件

    $scope.initialData = function()
    {
        var uriData = "op=map";
        if ( $scope.codeList.selected && $scope.codeList.selected.id > 0)
            uriData += "&mid=" + $scope.codeList.selected.id;

        if ( $scope.batchList.selected && $scope.batchList.selected.batch != "选择全部" )
            uriData += "&batch=" + $scope.batchList.selected.batch;
        
        if ( $scope.installTypeList.selected && $scope.installTypeList.selected.code != "0" )
            uriData += "&it=" + $scope.installTypeList.selected.code;
        
        CommonService.getAll("gpsModuleTrace", uriData, 
            function(data)
            {
                map.clearOverlays();

                $scope.moduleList   = listToObject( data, 'rows').rows;

                for ( var i = 0; i < $scope.moduleList.length; i++)
                {
                    var module  = $scope.moduleList[i];
                    
                    // 此处是需要设置旗帜的左下角为定位点
                    var flagIcon = new BMap.Icon( "/resource/images/redflag.png", new BMap.Size(20, 30), { anchor: new BMap.Size( 2, 30) } );

                    var pt = new BMap.Point( module.longitude, module.latitude);

                    // 创建标注，使用默认的标记图标
                    // var marker = new BMap.Marker(pt, { icon : flagIcon });
                    var marker = new BMap.Marker(pt);

                    var label = new BMap.Label(module.time + " " + module.addr, { offset : new BMap.Size(20, -10)});

                    label.setStyle({ display:"none", });
                    marker.setLabel(label);
                    marker.label = label;

                    map.addOverlay(marker); 
                    var content = "<p style='font-size:12px;'>" + module.time + "</p>";
                    content += "<p style='font-size:12px;'>" + module.addr + "</P>";
                    content += "<p style='font-size:12px;'>" + module.name + "</P>";
                    
                    var infoWindow = new BMap.InfoWindow( content );
                    marker.infoWindow = infoWindow;

                    marker.addEventListener("mouseover", function(marker)
                    {
                        marker.target.label.setStyle({ display:"block", border:"1px solid #ccc", background: "#f7f7c4", });
                        this.openInfoWindow(marker.target.infoWindow);
                    }); 

                    marker.addEventListener("mouseout", function(marker){
                        marker.target.label.setStyle({ display:"none", });
                        this.closeInfoWindow(marker.target.infoWindow);
                    }); 
                }

                // var index = 0;
                // for ( var i = 0; i < $scope.moduleList.length; i++)
                // {
                //     var module = $scope.moduleList[i]; 
                //     var pt = new BMap.Point( module.longitude, module.latitude);

                //     // // 根据经纬度获取当前地名，每次只能获取一个经纬度对应的地址，而且回调函数被调用的次序和循环次数并不一致
                //     // // 不能根据返回的结果中的经纬度来查询对应的地位模块，因为可能会有多个地位模块在同一个地方。
                //     // var gc = new BMap.Geocoder(); 
                //     // gc.getLocation( pt, function( result )
                //     // {
                //     //     var addrComp = result.addressComponents;
                //     //     addrList[index] = { address : result.address, point : rs.point,};

                //     //     index ++;
                //     //     if ( index == $scope.moduleList.length )
                //     //         // 已经获取到所有的地名
                //     //         $scope.makeMarker(map);
                //     // });
                // }

                // // 用于测试，根据地址查询经纬度 

                var localSearch = new BMap.LocalSearch(map);
                localSearch.setSearchCompleteCallback( function (searchResult)
                {
                    var poi = searchResult.getPoi(0);
                    console.log(poi);
                    var pt = new BMap.Point(poi.point.lng, poi.point.lat);
                    var marker2 = new BMap.Marker(pt);
                    map.addOverlay(marker2); 
                });
                // localSearch.search( "北京市海淀区长安街2号" );  
                

            },
            function(data)
            {
                onErrorResult(data);
            }
        );
    }

    // 创建地图标注
    $scope.makeMarker = function(map)
    {
        for ( var i = 0; i < addrList.length; i++)
        {
            var module = null;
            // 根据经纬度反查模块信息
            for ( var j = 0; j < $scope.moduleList.length; j++)
            {
                var pt  = addrList[i].point;
                if ( pt.lat == $scope.moduleList[j].latitude && pt.lng == $scope.moduleList[j].longitude )
                {
                    module = $scope.moduleList[j];
                    break;
                }
            }

            if ( module == null )
                // 数据错误
                continue;

            // anchor设置的是定位点距离图片左上角的偏移量
            // 此处是需要设置旗帜的左下角为定位点
            var flagIcon = new BMap.Icon( "/resource/images/redflag.png", new BMap.Size(20, 30), { anchor: new BMap.Size( 2, 30) } );

            var pt = new BMap.Point( module.longitude, module.latitude);

            // 创建标注，使用默认的标记图标
            // var marker = new BMap.Marker(pt, { icon : flagIcon });
            var marker = new BMap.Marker(pt);

            var label = new BMap.Label(module.time + "  " + addrList[i].address, { offset : new BMap.Size(20, -10)});

            label.setStyle({ display:"none", });
            marker.setLabel(label);
            marker.label = label;

            map.addOverlay(marker); 

            marker.addEventListener("mouseover", function(marker)
            {
                marker.target.label.setStyle({ display:"block", border:"1px solid #ccc", background: "#f7f7c4", });
            }); 

            marker.addEventListener("mouseout", function(marker){
                marker.target.label.setStyle({ display:"none", });
            }); 
        }
    }
})

// 定位模块轨迹
.controller('gpsModuleTraceCtrl', function($scope, $http, $rootScope, $route, $window, CommonService)
{
    CommonService.heartbeat();
    $scope.user = null;
    if(sessionStorage.getItem("strUser"))
        $scope.user=JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html";

    $scope.codeList   = [];
    $scope.batchList  = [];
    $scope.installTypeList      = [];

    // 初始化下拉列表
    $scope.initListCtrl = function()
    {
        var selectedType = null;
        var selectedBatch = null;
        var uriData = { op: "data" };

        if ( $scope.installTypeList.selected && $scope.installTypeList.selected.name != "选择全部")
        {
            selectedType = $scope.installTypeList.selected; 
            uriData.type = $scope.installTypeList.selected.code;
        }

        if ( $scope.batchList.selected && $scope.batchList.selected.batch != "选择全部" )
        {
            selectedBatch =  $scope.batchList.selected;
            uriData.batch = $scope.batchList.selected.batch;
        }

        uriData = angular.toJson(uriData);

        CommonService.updatePartOne("gpsModuleManage", uriData,
            function(data)
            {
                $scope.codeList            = listToObject( data.codeList, 'rows').rows;
                $scope.batchList           = listToObject( data.batchList, 'rows').rows;
                $scope.installTypeList     = listToObject( data.installTypeList, 'rows').rows;
                if ( selectedType != null )
                    $scope.installTypeList.selected = selectedType;

                if ( selectedBatch != null )
                    $scope.batchList.selected = selectedBatch;
                
                $scope.codeList.shift();
                if ( $scope.codeList.length > 0 )
                {
                    $scope.codeList.selected = $scope.codeList[0];
                    $scope.initialData();
                }

            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
    $scope.initListCtrl();

    $scope.showModuleTrace = function()
    {
        $scope.initialData();
    }

    // 百度地图
    var map = $scope.map = new BMap.Map("gpsModuleMap", { minZoom : 5}); 

    map.centerAndZoom("西安", 5);
    map.enableScrollWheelZoom();                            // 启用滚轮缩放
    map.enableContinuousZoom();                             // 启用地图惯性拖拽，默认禁用
    map.addControl(new BMap.NavigationControl());           // 添加默认缩放平移控件
    map.addControl(new BMap.OverviewMapControl());          // 添加默认缩略地图控件

    $scope.initialData = function()
    {
        if ( $scope.codeList.selected == undefined )
        {
            map.clearOverlays();
            return;
        }

        var uriData = "op=trace&mid=" + $scope.codeList.selected.id;

        CommonService.getAll("gpsModuleTrace", uriData, 
            function(data)
            {
                map.clearOverlays();

                $scope.traceList   = listToObject( data, 'rows').rows;

                for ( var i = 0; i < $scope.traceList.length; i++)
                {
                    var module  = $scope.traceList[i];    
                    // 此处是需要设置红点的中心为定位点
                    var iconRadius = 16;
                    var pointIcon = new BMap.Icon( "/resource/images/redpoint.png", new BMap.Size(iconRadius, iconRadius), { anchor: new BMap.Size( iconRadius/2, iconRadius/2) } );

                    var pt = new BMap.Point( module.longitude, module.latitude);

                    // 创建标注，使用红点
                    var marker = new BMap.Marker(pt, { icon : pointIcon });
                    // var marker = new BMap.Marker(pt);

                    var label = new BMap.Label(module.time + " " + module.addr, { offset : new BMap.Size(20, -10)});

                    label.setStyle({ display:"none", });
                    marker.setLabel(label);
                    marker.label = label;

                    map.addOverlay(marker); 
                    var content = "<p style='font-size:12px;'>" + module.time + "</p>";
                    content += "<p style='font-size:12px;'>" + module.addr + "</P>";
                    content += "<p style='font-size:12px;'>" + module.name + "</P>";
                    
                    var infoWindow = new BMap.InfoWindow( content );
                    marker.infoWindow = infoWindow;

                    marker.addEventListener("mouseover", function(marker)
                    {
                        marker.target.label.setStyle({ display:"block", border:"1px solid #ccc", background: "#f7f7c4", });
                        this.openInfoWindow(marker.target.infoWindow);
                    }); 

                    marker.addEventListener("mouseout", function(marker){
                        marker.target.label.setStyle({ display:"none", });
                        this.closeInfoWindow(marker.target.infoWindow);
                    }); 
                }

                // var index = 0;
                // for ( var i = 0; i < $scope.traceList.length; i++)
                // {
                //     var module = $scope.traceList[i]; 
                //     var pt = new BMap.Point( module.longitude, module.latitude);

                //     // // 根据经纬度获取当前地名，每次只能获取一个经纬度对应的地址，而且回调函数被调用的次序和循环次数并不一致
                //     // // 不能根据返回的结果中的经纬度来查询对应的地位模块，因为可能会有多个地位模块在同一个地方。
                //     // var gc = new BMap.Geocoder(); 
                //     // gc.getLocation( pt, function( result )
                //     // {
                //     //     var addrComp = result.addressComponents;
                //     //     addrList[index] = { address : result.address, point : rs.point,};

                //     //     index ++;
                //     //     if ( index == $scope.traceList.length )
                //     //         // 已经获取到所有的地名
                //     //         $scope.makeMarker(map);
                //     // });
                // }

                // // 用于测试，根据地址查询经纬度 

                var localSearch = new BMap.LocalSearch(map);
                localSearch.setSearchCompleteCallback( function (searchResult)
                {
                    var poi = searchResult.getPoi(0);
                    console.log(poi);
                    var pt = new BMap.Point(poi.point.lng, poi.point.lat);
                    var marker2 = new BMap.Marker(pt);
                    map.addOverlay(marker2); 
                });
                // localSearch.search( "北京市海淀区长安街2号" );  
                

            },
            function(data)
            {
                onErrorResult(data);
            }
        );
    }

    // 创建地图标注
    $scope.makeMarker = function(map)
    {
        for ( var i = 0; i < addrList.length; i++)
        {
            var module = null;
            // 根据经纬度反查模块信息
            for ( var j = 0; j < $scope.traceList.length; j++)
            {
                var pt  = addrList[i].point;
                if ( pt.lat == $scope.traceList[j].latitude && pt.lng == $scope.traceList[j].longitude )
                {
                    module = $scope.traceList[j];
                    break;
                }
            }

            if ( module == null )
                // 数据错误
                continue;

            // anchor设置的是定位点距离图片左上角的偏移量
            // 此处是需要设置旗帜的左下角为定位点
            var flagIcon = new BMap.Icon( "/resource/images/redflag.png", new BMap.Size(20, 30), { anchor: new BMap.Size( 2, 30) } );

            var pt = new BMap.Point( module.longitude, module.latitude);

            // 创建标注，使用默认的标记图标
            // var marker = new BMap.Marker(pt, { icon : flagIcon });
            var marker = new BMap.Marker(pt);

            var label = new BMap.Label(module.time + "  " + addrList[i].address, { offset : new BMap.Size(20, -10)});

            label.setStyle({ display:"none", });
            marker.setLabel(label);
            marker.label = label;

            map.addOverlay(marker); 

            marker.addEventListener("mouseover", function(marker)
            {
                marker.target.label.setStyle({ display:"block", border:"1px solid #ccc", background: "#f7f7c4", });
            }); 

            marker.addEventListener("mouseout", function(marker){
                marker.target.label.setStyle({ display:"none", });
            }); 
        }
    }
})

