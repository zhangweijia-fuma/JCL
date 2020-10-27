////////////////////////////////////////////////////////////////////////////////////////////

angular.module('rsuDeviceControllers', ['treeControl'])

//////////////////////////////////////////////////////////////////////////////////////////////
// RSU设备管理
.controller("rsuDeviceManageCtrl", function ($scope, $location, $rootScope, CommonService, $route, FileUploader)
{
    CommonService.heartbeat();
    $scope.user = null;
    if(sessionStorage.getItem("strUser"))
        $scope.user = JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"

    $scope.pager = new Pager(null);
    $scope.pager.pageIndex = 1;
    $scope.pager.pageSize  = 20;

    $scope.projectList      = [];
    $scope.projectList2     = [];
    
    $scope.statusList       = [];
    $scope.statusList2      = [];
    
    $scope.vendorList       = [];
    $scope.vendorList2      = [];

    $scope.deviceTypeList   = [];

    $scope.deviceList       = [];
    $scope.organizationList = [];

    $scope.currentDevice        = {};
    $scope.deviceStatueDataList = [];
    $scope.showStatusInfo       = false;

    // 初始化下拉列表
    $scope.initListCtrl = function()
    {
        var uriData = { op: "data" };
        uriData = angular.toJson(uriData);

        CommonService.updatePartOne("rsuDeviceManage", uriData,
            function(data)
            {
                $scope.projectList      = listToObject( data.projectList, 'rows').rows;
                $scope.statusList       = listToObject( data.statusList, 'rows').rows;
                $scope.vendorList       = listToObject( data.vendorList, 'rows').rows;
                $scope.organizationList = listToObject( data.organizationList, 'rows').rows;

                $scope.deviceTypeList   = listToObject( data.deviceTypeList, 'rows').rows;

                $scope.vendorList2      = $scope.vendorList.slice(1);
                $scope.projectList2     = $scope.projectList.slice(1);
                $scope.statusList2      = $scope.statusList.slice(1);
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
    $scope.initListCtrl();

    $scope.searchDevice = function()
    {
        $scope.pager.pageIndex = 1;
        $scope.initialData();
    }

    $scope.initialData = function()
    {
        var uriData = "op=data&o=" + $scope.pager.pageIndex + "&r=" + $scope.pager.pageSize;

        if ( $scope.projectList.selected && $scope.projectList.selected.id > 0 )
            uriData += "&pid=" + $scope.projectList.selected.id;

        if ( $scope.statusList.selected && $scope.statusList.selected.name != "选择全部" )
            uriData += "&sc=" + $scope.statusList.selected.code;
        
        if ( $scope.vendorList.selected && $scope.vendorList.selected.name != "选择全部")
            uriData += "&vid=" + $scope.vendorList.selected.id;

        CommonService.getAll("rsuDeviceManage", uriData, 
            function(data)
            {
                $scope.deviceList  = listToObject( data, 'rows').rows;
                $scope.count        = data.count;
                $scope.totalPage    = Math.ceil(parseInt(data.count) / $scope.pager.pageSize);
                $scope.pager.update(data.count);

                for ( i = 0; i < $scope.deviceList.length; i++)
                {
                    $scope.deviceList[i].color = "RED";
                    if ( $scope.deviceList[i].device_status == '1' )
                        $scope.deviceList[i].color = "#4AB033";
                    else if ( $scope.deviceList[i].device_status == '2' )
                        $scope.deviceList[i].color = "Pink";
                    else if ( $scope.deviceList[i].device_status == '3' )
                        $scope.deviceList[i].color = "Yellow";
                    else if ( $scope.deviceList[i].device_status == '4' )
                        $scope.deviceList[i].color = "Red";
                    else if ( $scope.deviceList[i].device_status == '5' )
                        $scope.deviceList[i].color = "LightGray";
                    
                    $scope.deviceList[i].no = i + 1;
                }

                if ( $scope.deviceList.length > 0 )
                    $scope.getDeviceStatus($scope.deviceList[0]);
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

        if ( $scope.projectList.selected && $scope.projectList.selected.id > 0 )
            uriData += "&pid=" + $scope.projectList.selected.id;

        if ( $scope.statusList.selected && $scope.statusList.selected.name != "选择全部" )
            uriData += "&sc=" + $scope.statusList.selected.code;
        
        if ( $scope.vendorList.selected && $scope.vendorList.selected.name != "选择全部")
            uriData += "&vid=" + $scope.vendorList.selected.id;

        CommonService.getAll('rsuDeviceManage', uriData,
            function(data)
            {
                var filename = "RSU设备查询" + getNowFormatDate2("", "", "-") + ".xlsx";
                var url = location.origin + data.url;
                var cmd = "<a download=" + filename + " href=" + url + "></a>"; 
                $( cmd )[0].click();
            },
            function(data,status)
            {
                // 失败时的回调函数
                onErrorResult(data);
            }
        );
    }

    $scope.page = function(e)
    {
        $scope.pager.onEvent(e);
        $scope.initialData();
    };

    $scope.createDevice = function()
    {
        ClearAreaInfo("produce_addr");
        ClearAreaInfo("install_addr");

        sessionStorage.removeItem('rsuDeviceInfo');
        window.location.href = '#/RSUDeviceEdit';
    }

    $scope.updateInfo = function( item )
    {
        ClearAreaInfo("produce_addr");
        ClearAreaInfo("install_addr");

        var addrList = [];
        addrList.push(item.produce_location);
        addrList.push(item.install_location);

        var uriData;

        uriData = { op: "area", addr: addrList };
        uriData = angular.toJson(uriData);
        CommonService.updatePartOne("areaCodeManage", uriData,
            function(data)
            {
                SetAreaInfo("produce_addr", data.Address[0]);
                SetAreaInfo("install_addr", data.Address[1]);

                item.detail_location = data.Address[1].detailAddr;

                sessionStorage.setItem("rsuDeviceInfo", JSON.stringify(item));
                window.location.href = '#/RSUDeviceEdit';
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );

    }

    $scope.showDeleteDlg = function(item)
    {
        MsgBox.confirm("确认", "确定要删除设备吗？", 
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

        CommonService.deleteOne("rsuDeviceManage", uriData, 
            function(data)
            {
                // 删除后重新请求数据
                MsgBox.info("删除操作成功！");
                $scope.initialData();
            },
            function(data, status )
            {
                onErrorResult(data);
            }
        );
    }


    // 批量导入
    
    // uploader
    $scope.uploadError = false;
    var uploader = $scope.uploader = new FileUploader({
        url:uploadURL,
        method: 'POST',
        headers: { Authorization: sessionStorage.getItem('authToken'), },
        autoUpload: true,
        formData: [{ 'module' : 'rsu', 'service_type' : 'rsuDevice'}]
    });

    uploader.filters.push({
        name: 'customFilter',
        fn: function(item /*{File|FileLikeObject}*/, options)
        {
            $scope.uploadError = false;
            // 只能选择一个文件
            return this.queue.length <= 1;
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

        this.queue.length = 0;
        $scope.processBatchFile();
    }

    $scope.processBatchFile = function()
    {
        var uriData = angular.toJson( { op:"upload", fid: $scope.batchFileID} );

        CommonService.updatePartOne("rsuDeviceManage", uriData, 
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


    // 获取设备状态信息
    $scope.getDeviceStatus = function(item)
    {
        $scope.currentDevice = item;

        uriData = "op=data&o=1&r=2000&did=" + $scope.currentDevice.device_id;

        CommonService.getAll("deviceStatusManage", uriData, 
            function(data)
            {
                $scope.deviceStatueDataList = listToObject( data, 'rows').rows;
                $scope.showDeviceChart($scope.currentDevice.device_code);
            },
            function(data)
            {
                onErrorResult(data);
            }
        );
    }

    // 报表显示
    $scope.showDeviceChart = function(device_code)
    {
        if( $scope.deviceStatueDataList.length == 0 )
        {
            $scope.showStatusInfo = false;
            return;
        }

        $scope.showStatusInfo = true;
            
        var cpu         = { name : "CPU" };
        var memory      = { name : "内存"};
        var harddisk    = { name : "硬盘"};
        var cpuData     = [];
        var memoryData  = [];
        var harddiskData= [];
        var categories  = [];

        var startTime   = "2099-12-31 23:59";
        var endTime     = "2000-01-01 00:00";

        for ( i = 0; i < $scope.deviceStatueDataList.length; i++)
        {
            cpuData.push( $scope.deviceStatueDataList[i].cpu / 100 );
            memoryData.push( $scope.deviceStatueDataList[i].memory /100 );
            harddiskData.push( $scope.deviceStatueDataList[i].harddisk / 100 );

            // 将显示的时间最后2位修改为00，知道分钟，不到秒
            var time = $scope.deviceStatueDataList[i].date + " " + $scope.deviceStatueDataList[i].time;
        
            index   = 16;
            time    = time.substring(0, index);

            categories.push(time);

            if ( time < startTime ) startTime = time;
            if ( time > endTime )   endTime = time;
        }

        cpu.data        = cpuData;
        memory.data     = memoryData;
        harddisk.data   = harddiskData;

        $scope.seriesData    = [];
        $scope.seriesData[0] = cpu;
        $scope.seriesData[1] = memory;
        $scope.seriesData[2] = harddisk;

        $scope.deviceStatusChart = 
        {
            options: 
            {
                chart: { type: 'line', style: { backgroundColor: 'white', }, },
                xAxis: { categories: categories, crosshair: true },
                yAxis: { title: { text: '占有率'}, },
                yAxis: { title: '', max: 100, min: 0 },
                tooltip: { formatter: function() 
                    { 
                        return this.x + " " + this.series.name + '使用率：'+ Highcharts.numberFormat(this.y, 2) + '%';
                    }
                },
            },
            title: { text: device_code + ' 运行状态趋势 (' + startTime + " ~ " + endTime + " )" },
            
            series : $scope.seriesData,
        };

    };

})

// 设备信息更新
.controller('rsuDeviceEditCtrl', function($scope, $window, $rootScope, CommonService)
{
    var strUser = sessionStorage.getItem("strUser");

    if(strUser)
        $scope.user = JSON.parse(strUser);
    else
        window.location.href = "/login.html";

    $scope.projectList      = [];
    $scope.projectList2     = [];
    
    $scope.statusList       = [];
    $scope.statusList2      = [];
    
    $scope.vendorList       = [];
    $scope.vendorList2      = [];

    $scope.deviceTypeList   = [];

    $scope.deviceList       = [];
    $scope.organizationList = [];

    $scope.deviceInfo       = { device_id : 0 };                                       // 设备信息

    if ( !sessionStorage.getItem("rsuDeviceInfo"))
    {
        // 新增记录
        $scope.editMode     = '新增';
        $scope.vendorInfo   = { id: 0};
    }
    else
    {
        // 更新
        $scope.editMode     = '更新';
        $scope.deviceInfo   = JSON.parse( sessionStorage.getItem("rsuDeviceInfo") );
    }

    $scope.goback = function()
    {
        history.back();
    }

    // 初始化下拉列表
    $scope.initListCtrl = function()
    {
        var uriData = { op: "data" };
        uriData = angular.toJson(uriData);

        CommonService.updatePartOne("rsuDeviceManage", uriData,
            function(data)
            {
                $scope.projectList      = listToObject( data.projectList, 'rows').rows;
                $scope.statusList       = listToObject( data.statusList, 'rows').rows;
                $scope.vendorList       = listToObject( data.vendorList, 'rows').rows;
                $scope.deviceTypeList   = listToObject( data.deviceTypeList, 'rows').rows;

                $scope.vendorList2      = $scope.vendorList.slice(1);
                $scope.projectList2     = $scope.projectList.slice(1);
                $scope.statusList2      = $scope.statusList.slice(1);
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
    }

    $scope.clearAreaInfo = function()
    {
        sessionStorage.removeItem('produce_addrcountry');
        sessionStorage.removeItem('produce_addrprovince');
        sessionStorage.removeItem('produce_addrcity');
        sessionStorage.removeItem('produce_addrdistrict');

        sessionStorage.removeItem('install_addrcountry');
        sessionStorage.removeItem('install_addrprovince');
        sessionStorage.removeItem('install_addrcity');
        sessionStorage.removeItem('install_addrdistrict');
    }

    $scope.confirmToSaveInfo = function()
    {
        // 检查参数是否合法
        if( $scope.deviceInfo.vendor_id == undefined )
        {
            $("#vendor_id").testRemind("请选择供应商");
            return;
        }

        if($("#project_id").val() == '')
        {
            $("#project_id").testRemind("请选择项目");
            return;
        }

        if($("#device_name").val() == '')
        {
            $("#device_name").testRemind("请输入设备名称");
            return;
        }

        if ( $("#device_name_en").val() == "" ) 
        {
            $("#device_name_en").testRemind("请输入设备英文名称");
            return;
        }

        if ( $("#device_sn").val() == "" ) 
        {
            $("#device_sn").testRemind("请输入设备序列号");
            return;
        }

        if ( $("#ip_adddress1").val() == "" ) 
        {
            $("#ip_adddress1").testRemind("请输入设备IP地址1");
            return;
        }

        if ( $("#ip_adddress2").val() == "" ) 
        {
            $("#ip_adddress2").testRemind("请输入设备IP地址2");
            return;
        }

        if ( $("#device_desc").val() == "" ) 
        {
            $("#device_desc").testRemind("请输入设备描述");
            return;
        }
        
        if( $scope.deviceInfo.device_type_id == undefined )
        {
            $("#device_type").testRemind("请选择设备类型");
            return;
        }

        if($("#produce_date").val() == '')
        {
            $("#produce_date").testRemind("请输入生产日期");
            return;
        }

        if ( $("#produce_location").val() == '' ) 
        {
            $("#produce_location").testRemind("请选择生产地点");
            return;
        }

        if ( $("#produce_batch").val() == '' ) 
        {
            $("#produce_batch").testRemind("请输入生产批次");
            return;
        }

        if ( $("#device_qr_code").val() == '' ) 
        {
            $("#device_qr_code").testRemind("请生成二维码");
            return;
        }

        if ( $("#start_used_date").val() == '' ) 
        {
            $("#start_used_date").testRemind("请选择启用日期");
            return;
        }

        if ( $("#end_used_date").val() == '' ) 
        {
            $("#end_used_date").testRemind("请选择停用日期");
            return;
        }

        if ( $("#location").val() == '' ) 
        {
            $("#location").testRemind("请选择设备安装地址");
            return;
        }

        if ( $("#detail_location").val() == '' ) 
        {
            $("#detail_location").testRemind("请输入安装地点的详细门牌地址");
            return;
        }

        if ( $("#install_date").val() == '' ) 
        {
            $("#install_date").testRemind("请输入设备安装日期");
            return;
        }

        // 获取地址信息
        countryData     = JSON.parse(sessionStorage.getItem('produce_addrcountry'));
        provinceData    = JSON.parse(sessionStorage.getItem('produce_addrprovince'));
        cityData        = JSON.parse(sessionStorage.getItem('produce_addrcity'));
        districtData    = JSON.parse(sessionStorage.getItem('produce_addrdistrict'));

        // 直辖市时没有区县级
        if ( districtData == null )
            districtData = { area_name : ""};

        countryName = "";
        if ( countryData != null )
            countryName = countryData.area_name;

        $scope.deviceInfo.produce_location = countryName + provinceData.area_name + cityData.area_name + districtData.area_name;

        countryData     = JSON.parse(sessionStorage.getItem('install_addrcountry'));
        provinceData    = JSON.parse(sessionStorage.getItem('install_addrprovince'));
        cityData        = JSON.parse(sessionStorage.getItem('install_addrcity'));
        districtData    = JSON.parse(sessionStorage.getItem('install_addrdistrict'));

        // 使用完成后清除保存的数据
        $scope.clearAreaInfo();

        if ( districtData == null )
            districtData = { area_name : ""};
        
        if ( countryData != null )
            countryName = countryData.area_name;

        $scope.deviceInfo.install_location = countryName + provinceData.area_name + cityData.area_name + districtData.area_name + $scope.deviceInfo.detail_location;

        if ( $scope.editMode == "新建")
            $scope.saveInfo( CommonService.createOne, $scope.deviceInfo );
        else if ( $scope.editMode == "更新")
        {
            $scope.saveInfo( CommonService.updateOne, $scope.deviceInfo );
        }

    }

    $scope.saveInfo = function( CommonServiceFunc, item )
    {
        var uriData = angular.toJson( item);
        CommonServiceFunc("rsuDeviceManage", uriData, 
            function(data)
            {
                // 保存成功后返回上一个页面
                $scope.goback();
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }

})


// RSU地图
.controller('rsuDeviceLocationCtrl', function($scope, $http, $rootScope, $route, $window, CommonService)
{
    CommonService.heartbeat();
    $scope.user = null;
    if(sessionStorage.getItem("strUser"))
        $scope.user=JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html";

    $scope.projectList      = [];    
    $scope.statusList       = [];    
    $scope.vendorList       = [];

    $scope.deviceList       = [];

    // 初始化下拉列表
    $scope.initListCtrl = function()
    {
        var uriData = { op: "data" };
        uriData = angular.toJson(uriData);

        CommonService.updatePartOne("rsuDeviceManage", uriData,
            function(data)
            {
                $scope.projectList      = listToObject( data.projectList, 'rows').rows;
                $scope.statusList       = listToObject( data.statusList, 'rows').rows;
                $scope.vendorList       = listToObject( data.vendorList, 'rows').rows;
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
    $scope.initListCtrl();

    $scope.searchDevice = function()
    {
        $scope.initialData();
    }

    // 百度地图
    var map = $scope.map = new BMap.Map("rsuModuleMap", { minZoom : 5}); 

    // map.centerAndZoom(, 15);              // 显示级数越大越细
    map.enableScrollWheelZoom();                            // 启用滚轮缩放
    map.enableContinuousZoom();                             // 启用地图惯性拖拽，默认禁用
    map.addControl(new BMap.NavigationControl());           // 添加默认缩放平移控件
    map.addControl(new BMap.OverviewMapControl());          // 添加默认缩略地图控件
    var geoc = new BMap.Geocoder();  

    $scope.initialData = function()
    {
        var uriData = "op=data";

        if ( $scope.projectList.selected && $scope.projectList.selected.id > 0 )
            uriData += "&pid=" + $scope.projectList.selected.id;

        if ( $scope.statusList.selected && $scope.statusList.selected.name != "选择全部" )
            uriData += "&sc=" + $scope.statusList.selected.code;
        
        if ( $scope.vendorList.selected && $scope.vendorList.selected.name != "选择全部")
            uriData += "&vid=" + $scope.vendorList.selected.id;

        CommonService.getAll("rsuDeviceManage", uriData, 
            function(data)
            {
                map.clearOverlays();

                $scope.deviceList   = listToObject( data, 'rows').rows;
                if ( $scope.deviceList.length == 0 )
                {
                    map.centerAndZoom("成都", 5);
                    return;
                }

                var devicePosList = [];

                for ( var i = 0; i < $scope.deviceList.length; i++)
                {
                    var rsu  = $scope.deviceList[i];

                    // 此处是需要设置旗帜的左下角为定位点
                    // if ( rsu.device_status == '1' )
                    //     filename = "/resource/images/RSU/BlueFlag.png";
                    // else if ( rsu.device_status == '2' )
                    //     filename = "/resource/images/RSU/OrangeFlag.png";
                    // else if ( rsu.device_status == '3' )
                    //     filename = "/resource/images/RSU/GoldFlag.png";
                    // else if ( rsu.device_status == '4' )
                    //     filename = "/resource/images/RSU/RedFlag.png";
                    // else if ( rsu.device_status == '5' )
                    //     filename = "/resource/images/RSU/GrayFlag.png";
                    // var flagIcon = new BMap.Icon( filename, new BMap.Size(30, 40), { anchor: new BMap.Size( 2, 40) } );

                    var pt = new BMap.Point( rsu.longtitude, rsu.latitude);
                    devicePosList.push(pt);

                    // 创建标注，使用默认的标记图标
                    // var marker = new BMap.Marker(pt, { icon : flagIcon });
                    var marker = new BMap.Marker(pt);

                    var offset = 25;
                    if ( i % 2 == 0 )
                        offset = -165;

                    var label = new BMap.Label(rsu.device_code, { offset : new BMap.Size(offset, -10)});

                    if ( rsu.device_status == '1' )
                        color = "Green";
                    else if ( rsu.device_status == '2' )
                        color = "Orange";
                    else if ( rsu.device_status == '3' )
                        color = "Pink";
                    else if ( rsu.device_status == '4' )
                        color = "LightRed";
                    else if ( rsu.device_status == '5' )
                        color = "Lightgray";

                    // 设置label的样式
                    label.setStyle({ color: color, fontSize: '14px', border: '1px solid lightgray', hight: '20xpx',});

                    map.addOverlay(label);

                    marker.setLabel(label);
                    marker.label = label;
                    marker.rsu = rsu;

                    map.addOverlay(marker);

                    var content = "<p style='font-size:12px;'>" + rsu.project_name + "</p>";
                    content += "<p style='font-size:12px;'>" + rsu.device_name + ": " + rsu.device_code + "</P>";
                    if ( rsu.ip_address1 != null )
                        content += "<p style='font-size:12px;'>" + rsu.ip_address1 + "</P>";
                    if ( rsu.ip_address2 != null )
                        content += "<p style='font-size:12px;'>" + rsu.ip_address2 + "</P>";
                    
                    content += "<p style='font-size:12px;'>" + rsu.install_location + "</P>";
                    content += "<p style='font-size:12px; color: " + color + "'>设备状态：" + rsu.status_name + "</P>";
                    
                    var infoWindow = new BMap.InfoWindow( content );
                    marker.infoWindow = infoWindow;

                    marker.addEventListener("mouseover", function(marker)
                    {
                        this.openInfoWindow(marker.target.infoWindow);
                    }); 

                    marker.addEventListener("mouseout", function(marker){
                        this.closeInfoWindow(marker.target.infoWindow);
                    });

                    marker.addEventListener("click", function(marker){
                        MsgBox.info("提示", marker.target.rsu.device_code);
                    });
                }

                // 根据标注点的分别自动确定显示的范围以把所有标注点都显示出来
                map.setViewport(devicePosList);

                // // 根据经纬度获取当前地名，每次只能获取一个经纬度对应的地址
                // var pt = new BMap.Point( 103.941197, 30.724855);
                // var gc = new BMap.Geocoder(); 
                // gc.getLocation( pt, function( result )
                // {
                //     var addrComp = result.address;
                // });

                // // 用于测试，根据地址查询经纬度 

                // var localSearch = new BMap.LocalSearch(map);
                // localSearch.setSearchCompleteCallback( function (searchResult)
                // {
                //     var poi = searchResult.getPoi(0);
                //     console.log(poi);
                //     var pt = new BMap.Point(poi.point.lng, poi.point.lat);
                //     var marker2 = new BMap.Marker(pt);
                //     map.addOverlay(marker2); 
                // });
                
                // localSearch.search( "北京市东城区长安街2号" );  
            },
            function(data)
            {
                onErrorResult(data);
            }
        );
    }

    // 获取在地图上点击位置的经纬度及地址
    map.addEventListener("click", function(e){   
        // 通过点击百度地图，可以获取到对应的point, 由point的lng、lat属性就可以获取对应的经度纬度     
        var pt = e.point;
        geoc.getLocation(pt, function(rs){
            //addressComponents对象可以获取到详细的地址信息
            var addComp = rs.addressComponents;
            // var site = addComp.province + ", " + addComp.city + ", " + addComp.district + ", " + addComp.street + ", " + addComp.streetNumber;
            console.log(addComp);
            console.log(pt);
        });        
    });

})

// RSU设置
.controller('rsuDeviceConfigCtrl', function($scope,CommonService, FileUploader)
{
    CommonService.heartbeat();
    $scope.user = null;
    if(sessionStorage.getItem("strUser"))
        $scope.user=JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html";

    $scope.projectList  = [];    
    $scope.statusList   = [];    
    $scope.vendorList   = [];

    $scope.deviceList   = [];

    $scope.msgType          = "update";
    $scope.updateMode       = 1;
    $scope.rebootMode       = 1;
    $scope.rebootDelayTime  = 0;

    // 初始化下拉列表
    $scope.initListCtrl = function()
    {
        var uriData = { op: "data" };
        uriData = angular.toJson(uriData);

        CommonService.updatePartOne("rsuDeviceManage", uriData,
            function(data)
            {
                $scope.projectList      = listToObject( data.projectList, 'rows').rows;
                $scope.statusList       = listToObject( data.statusList, 'rows').rows;
                $scope.vendorList       = listToObject( data.vendorList, 'rows').rows;
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
    $scope.initListCtrl();

    $scope.searchDevice = function()
    {
        $scope.initialData();
    }

    // 树被选中时
    $scope.onTreeChecked = function(event, treeId, treeNode) 
    {
    }

    // 树被点击时
    $scope.onTreeClicked = function(event, treeId, treeNode) 
    {
        var treeObj = $.fn.zTree.getZTreeObj( "device_tree" );
        
        if ( treeNode.device_status == '5' )
        {
            // 设备未在线不能进行选择
            return;
        }

        treeNode.checked = ( treeNode.checked ) ? false : true;
        treeObj.updateNode( treeNode );
    }

    var setting = 
    {
        check: { enable: true },
        data: { },
        key:{ name:'name', children:'children' },
        callback: 
        { 
            // onCheck: $scope.onTreeChecked, 
            onClick: $scope.onTreeClicked,
        },
        view: 
        {
            fontCss: function (treeId, treeNode) 
            {
                selectedItemColor = {"color": "red", "font-weight": (( treeNode.device_status == '1') ? "bold" : "normal") };
                nonSelectedItemColor = 
                {
                    "color" : (!('device_status' in treeNode)) ? "black" : (( treeNode.device_status == '1') ? "green" : "gray"),
                    // "font-weight": (( treeNode.device_status == '1') ? "bold" : "normal"),
                    "background": "white",
                };
                
                return ( !!treeNode.highlight ) ? selectedItemColor : nonSelectedItemColor;
            }
        }
    };

    $scope.initialData = function()
    {
        var uriData = { op:"treeData" };
        if ( $scope.projectList.selected && $scope.projectList.selected.id > 0 )
            uriData.pid = $scope.projectList.selected.id;

        if ( $scope.statusList.selected && $scope.statusList.selected.name != "选择全部" )
            uriData.sc = $scope.statusList.selected.code;
        
        if ( $scope.vendorList.selected && $scope.vendorList.selected.name != "选择全部")
            uriData.vid = $scope.vendorList.selected.id;

        uriData = angular.toJson(uriData);

        $scope.loader = new Loader('sel_device_tree');
        CommonService.updatePartOne("rsuDeviceManage", uriData, 
            function(data)
            {
                $scope.loader.close();
                // $scope.deviceTreeData = data;
                $.fn.zTree.init($("#device_tree"), setting, data);
            },
            function(data, status)
            {
                $scope.loader.close();
                onErrorResult(data);
            }
        );
    }

    // uploader
    $scope.uploadError = false;
    var uploader = $scope.uploader = new FileUploader({
        url:uploadURL,
        method: 'POST',
        headers: { Authorization: sessionStorage.getItem('authToken'), },
        autoUpload: true,
        formData: [{ 'module' : 'rsu', 'service_type' : 'rsuUpdateFile'}]
    });

    uploader.filters.push({
        name: 'customFilter',
        fn: function(item /*{File|FileLikeObject}*/, options)
        {
            $scope.uploadError = false;
            // 只能选择一个文件
            return this.queue.length <= 1;
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
        this.queue.length = 0;
    }

    $scope.chooseMsgType = function( msgType )
    {
        $scope.msgType = msgType;
    }

    $scope.chooseUpdateMode = function(updateMode)
    {
        $scope.updateMode = updateMode;
    }

    $scope.chooseRebootMode = function(rebootMode)
    {
        $scope.rebootMode = rebootMode;
    }

    $scope.sendMessage = function()
    {
        var treeObj = $.fn.zTree.getZTreeObj( "device_tree" );
        var nodes = treeObj.getCheckedNodes(); 

        var deviceList = [];
        for(var i = 0; i < nodes.length; i++)
        {
            if (nodes[i].node_type != "device")
                continue;

            temp = { device_id : nodes[i].device_id };

            deviceList.push( temp );
        }
        
        if ( deviceList.length == 0 )
        {
            MsgBox.info("提示", "请选择至少一个设备！");
            return;
        }

        var uriData;

        if ( $scope.msgType == "update" )
        {
            if ( $scope.softwareFileName == null || $scope.softwareFileName == "" )
            {
                MsgBox.info("提示", "请上传版本文件！");
                return;
            }

            uriData = { op:"msg", msgType:"update", deviceList : deviceList, updateMode: $scope.updateMode,  file_path: $scope.updateSoftwarePath, file_name: $scope.softwareFileName };
        }
        else if ( $scope.msgType == "reboot")
        {
            uriData = { op:"msg", msgType:"reboot", deviceList : deviceList, rebootMode: $scope.rebootMode, delay_second: $scope.rebootDelayTime };
        }

        uriData = angular.toJson( uriData);
        CommonService.updatePartOne("rsuDeviceManage", uriData, 
            function(data)
            {
                MsgBox.info("提示", "操作成功！");
            },
            function(data, status)
            {
                onErrorResult(data);
            }
        );
    }
})

