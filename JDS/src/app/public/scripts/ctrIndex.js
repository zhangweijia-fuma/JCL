
var _indexCtrl   = null;
var _curPath     = null;
var fromTodo     = true;
window.menus     = [];

angular.module('indexControllers',[])
.controller('lmenuCtrl',function($scope, CommonService, $rootScope, $interval)
{

    var strUser = sessionStorage.getItem("strUser");
    if(strUser)
    {
        $scope.user = JSON.parse(strUser);
    }
    else
    {
        console.log("获取用户信息错误")
        window.location.href = "/login.html";
        return;
    }

    $scope.menuInited           = false;
    $scope.lastSelMenu          = null;
    $scope.lastSelFolder        = null;
    $scope.lastSelMenuIndex     = -1;
    $scope.lastSelFolderIndex   = -1;

    $scope.AppSystemUserInfo = JSON.parse(sessionStorage.getItem('AppSystemUserInfo'));

    $scope.showLeftMenu = function (show) 
    {
        if ($scope.leftMenu == show)
            return;

        $scope.leftMenu = show;

        if (show) 
        {
            $("#left_menus_div").show();
            $(".main-view").addClass("mainViewBg");
        } 
        else 
        {
            $("#left_menus_div").hide();
            $(".main-view").removeClass("mainViewBg");
        }
    };
    
    $scope.delRouteListener = $rootScope.$on('$routeChangeSuccess', function(currentRoute, routes) 
    {
        _curPath = myURL.getPath();
        if (null == _curPath) 
        {
            _curPath = '/';
            return;
        }

        // if(_curPath=="/toDo")
        // {
        //     fromTodo=true;
        //     $scope.showLeftMenu(false);
        // }
        // else if(_curPath=="/message")
        // {
        //     $scope.showLeftMenu(false);
        // }
        // else if(_curPath=="/my_information")
        // {
        //     $scope.showLeftMenu(false);
        // }
        // else if(_curPath=="/edit_my_info")
        // {
        //     $scope.showLeftMenu(false);
        // }
        // else
        {
            // 总是显示左边菜单栏
            $scope.showLeftMenu(true);
        }
        $scope.selectMenu2(_curPath);
    });

    var uridata = 'id=' + $scope.user.id;
    $scope.pathExist = false;
    CommonService.getAll('getMenu',uridata, 
        function(data)
        {
            var html="";
            if(!data||!data.length)
            {
                console.log("错误");
                window.location.href = "/login.html"
                return ;
            }
            else
            {
                window.menus=data;
                //创建一级菜单
                var html="";
                for(var i = 0; i < data.length; i++)
                {
                    html+='<li class="menu_first_li"><a href="javascript:void(0);" class="menu_first" menu_id="'+data[i].id+'">'+data[i].name+'</a></li>';
                }

                $("#navbar-right").prepend(html);

                //判断当前页面所属一级菜单
                if (null == _curPath || _curPath == '/'|| _curPath == '/toDo' ) 
                    return;
                
                $scope.topMenu = null;
                var isexist = false;
                for(var m = 0; m < window.menus.length; m++ )
                {
                    var data = window.menus[m].children;
                    var isfind=false;
                    for(var i = 0; i < data.length; i++)
                    {
                        if(data[i].children.length == 0)
                        {
                            if(data[i].path == _curPath)
                            {
                                $scope.pathExist = true;
                                $scope.topMenu = window.menus[m];
                                $(".menu_first[menu_id='" + window.menus[m].id+"']").trigger("click");
                                isfind     = true;
                                isexist = true;
                                break;
                            }
                        }
                        else
                        {
                            var children = data[i].children;
                            for (var j = 0; j < children.length; j++) 
                            {
                                if(children[j].path==_curPath)
                                {
                                    $scope.pathExist = true;
                                    $scope.topMenu      = window.menus[m];
                                    var li     = $(".menu_first[menu_id='"+window.menus[m].id + "']").trigger("click");
                                    isfind     = true;
                                    isexist = true;
                                    break;
                                }
                            }
                        }

                        if(isfind)
                            break;
                        
                    }

                    if(isfind)
                        break;
                }

                if(!isexist)
                {
                    if(sessionStorage.getItem('url_path'))
                    {
                        var path=(sessionStorage.getItem('url_path'))
                        for(var m = 0; m < window.menus.length; m++ )
                        {
                            var data = window.menus[m].children;
                            var isfind = false;
                            for(var i = 0; i < data.length; i++)
                            {
                                if(data[i].children.length == 0)
                                {
                                    if(data[i].path == path)
                                    {
                                        $scope.pathExist = true;
                                        $scope.topMenu      = window.menus[m];
                                        $(".menu_first[menu_id='" + window.menus[m].id + "']").trigger("click");
                                        isfind    = true;
                                        isexist    = true;
                                        break;
                                    }
                                }
                                else
                                {
                                    var children = data[i].children;
                                    for (var j = 0; j < children.length; j++) 
                                    {
                                        if(children[j].path == path )
                                        {
                                            $scope.pathExist    = true;
                                            $scope.topMenu         = window.menus[m];
                                            var li     = $(".menu_first[menu_id='" + window.menus[m].id + "']").trigger("click");
                                            isfind    = true;
                                            isexist    = true;
                                            break;
                                        }
                                    }
                                }

                                if(isfind)
                                    break;
                            }

                            if(isfind)
                                break;
                            
                        }
                    }
                }

            }
        },
        function()
        {
            // 失败时的回调函数
            window.location.href = "/login.html";
        }
    );

    $scope.selectMenu2 = function(curPath)
    {
        if (null == curPath || curPath =='/' || curPath == '/toDo' || fromTodo)
            return;
        
        if(window.menus.length == 0 )
            return;
        
        if(!$("#left_menus"))
            return;
        
        $(".nav-parent2").removeClass('selected');
        $(".nav-parent").removeClass('selected');
        $(".menuItem").removeClass('selected');
        var isexist=false;
        for(var m=0;m<window.menus.length;m++)
        {
            var data = window.menus[m].children;
            var isfind=false;
            for(var i = 0; i < data.length; i++)
            {
                if(data[i].children.length==0)
                {
                    if(data[i].path==curPath)
                    {
                        $('.icon-rotate-90').removeClass('icon-rotate-90')
                        .parents('.nav-parent').find('.nav').slideUp(function()
                        {
                            $(this).closest('.nav-parent').removeClass('show');
                        });
                        $("#menuFoler_"+i).addClass('selected');
                        sessionStorage.setItem('url_path',curPath);
                        $scope.topMenu = window.menus[m];
                        isfind=true;
                        isexist=true;
                        break;
                    }
                }
                else
                {
                    var children = data[i].children;
                    for (var j = 0; j < children.length; j++) 
                    {
                        if(children[j].path==curPath)
                        {
                            folder = $('#' + 'menuFoler_' + i);
                            var nownav=$('.icon-rotate-90').parents('.nav-parent').find('.nav');
                            var newnav=folder.find('.nav');
                            if(nownav.attr("menu_id")==newnav.attr("menu_id"))
                            {
                                $("#menuItem_"+ i + '_' + j).addClass('selected');
                            }else
                            {
                                //关闭之前
                                $('.icon-rotate-90').removeClass('icon-rotate-90')
                                .parents('.nav-parent').find('.nav').slideUp(function()
                                {
                                    $(this).closest('.nav-parent').removeClass('show');
                                });
                                //打开当前
                                folder.find('.nav-parent-fold-icon').addClass('icon-rotate-90');
                                folder.find('.nav').slideDown(function()
                                {
                                    $(this).closest('.nav-parent').addClass('show');
                                });
                                $("#menuItem_"+ i + '_' + j).addClass('selected');
                            }

                            sessionStorage.setItem('url_path',curPath);
                            $scope.topMenu = window.menus[m];
                            isexist=true;
                            isfind=true;
                            break;
                        }
                    }
                }

                if(isfind)
                    break;

            }

            if(isfind)
                break;
            
        }

        //如果未找到当前路劲，则查询缓存路径
        if(!isexist)
        {
            if(sessionStorage.getItem('url_path'))
            {
                $scope.selectMenu2(sessionStorage.getItem('url_path'));
            }
        } 
        else 
        {
            if ($scope.topMenu) 
            {
                if ($scope.topMenu.code == "treatInOne")
                    $scope.showLeftMenu(false);
            }
        }
    }

    $(function()
    {
        $(document).on("click",".menu_first",function()
        {
            $this=$(this);
            fromTodo=false;
            $("#left_menus_div").show();
            $("#index_content").removeClass("col-md-12").addClass("col-md-10");
            $(".menu_first_li").removeClass("active");

            var li=$this.parent("li");
            li.addClass("active");
            var menu_id=$this.attr('menu_id');
            //默认显示第一个菜单
            var first="";
            //生成左边的菜单
            for(var m=0;m<window.menus.length;m++)
            {
                if(window.menus[m].id==menu_id)
                {
                    var data = window.menus[m].children;
                    var html = '<nav id="left_menus" class="menu mt10" data-toggle="menu" style="width: 100%" data-auto="true">';
                    html += '<ul class="nav nav-primary">'
                    for(var i = 0; i < data.length; i++)
                    {
                        if(i==0)
                        {
                            if(data[i].children.length==0)
                                first=data[i].path;
                            else
                                first=data[i].children[0].path;
                        }

                        if(data[i].children.length==0)
                        {
                            html += '<li class="nav-parent2" id="menuFoler_' + i + '"> ';
                            html += '<a href="#'+data[i].path+'"><i class="icon-user" title="'+data[i].name+'"></i>'+
                            data[i].name + '<i class="icon-chevron-right nav-parent-fold-icon"></i></a>';
                        }
                        else
                        {
                            html += '<li class="nav-parent" id="menuFoler_' + i + '"> ';
                            html += '<a href="javascript:;"><i class="icon-user"></i>'+
                            data[i].name + '<i class="icon-chevron-right nav-parent-fold-icon"></i></a>';
                            html += '<ul class="nav" menu_id="'+i+'">';

                            var children = data[i].children;
                            for (var j = 0; j < children.length; j++) 
                            {
                                child = children[j];
                                html += '<li id="menuItem_' + i + '_' + j + '" class="menuItem"><a href="#' + child.path +
                                    '" ><i class="icon-pencil"></i>' + child.name + '</a></li>';
                            }

                            html+='</ul>';
                        }

                        html+='</li>';
                    }

                    html += '</ul></nav>';
                    $("#left_menus_div").html(html);
                    $('#left_menus').menu();
                    break;
                }
            }

            if($scope.pathExist)
            {
                $scope.selectMenu2(_curPath);
                $scope.pathExist=false;
            }
            else
            {
                location.href ="#"+first;
            }

        });
    });

    // //////////////////////////////////////////////////////////////////////////////////////////////////
    // 将所有区域信息存储在客户端，在页面使用area-picker时将此信息读出后进行显示
    // 缺陷：
    //  1. 在第一次读取的时候因为数据量比较大，导致时间会比较长
    //  2. 在需要显示一个已知地点时，需要将现有地点的信息输入到area-picker中，但是和area-picker中的数据不能匹配，导致不能在列表中将
    //      已经选择的区域进行高亮显示
    //  解决办法：
    //      1. 在每点击一次区域时，从后台取该区域的子区域信息；
    // 获取区域代码

    $scope.areaList = {};

    $scope.SetAreaList = function(areaItem)
    {
        if ( areaItem.children.length == 0 )
            return;

        var areaItemList = {};
        for (var i = 0; i < areaItem.children.length; i++)
        {
            var code = areaItem.children[i].id;
            areaItemList[code] = areaItem.children[i].name;
        }

        var code = areaItem.id;
        $scope.areaList[code] = areaItemList;
        for (var i = 0; i < areaItem.children.length; i++)
        {
            $scope.SetAreaList(areaItem.children[i]);
        }
    }

    // 获取区域信息
    $scope.GetAreaCode = function()
    {
        CommonService.getAll("areaCodeManage", null, 
            function(data)
            {
                sessionStorage.setItem('areaTree', JSON.stringify(data));

                var countryList = [];
                var ChineseList = [];
                for ( var i = 0; i < data.children.length; i++)
                {
                    // 准备国家列表
                    var countryData = { 'code': data.children[i].id, 'address' : data.children[i].area_name};
                    countryList.push(countryData);

                    // 单独为中国准备一个数据集合
                    if ( data.children[i].area_name == '中国')
                    {
                        provinceList = data.children[i].children;
                        for ( var j = 0; j < provinceList.length; j++)
                        {
                            var provinceData = { 'code': provinceList[j].id, 'address' : provinceList[j].area_name};
                            ChineseList.push(provinceData);
                        }
                    }
                }

                $scope.areaList = { 1: { ' ': countryList }, 1000000: {' ': ChineseList} };

                for ( var i = 0; i < data.children.length; i++)
                {
                    $scope.SetAreaList(data.children[i]);
                }

                sessionStorage.setItem('areaList', JSON.stringify($scope.areaList));
            },
            function(data, status)
            {
                MsgBox.info("提示", '获取区域代码表失败！');
            }
        );
    }
    $scope.GetAreaCode();

    // 监听整个页面鼠标移动事件
    var nextDate=new Date();
    document.onmousemove = function(e)
    {
        // 记录当前时间
        nextDate = new Date();
    }


    // 每分钟查看一次,登陆是否超时
    var intout = setInterval( function()
    {
        var nowDate    = new Date();
        var date3      = nowDate.getTime() - nextDate.getTime();
        var idelMinute = Math.floor(date3 / ( 60 * 1000));

        // 超过5分钟无操作，则退出登录
        if( idelMinute >= 5 )
        {
            clearTimeout(intout);
            CommonService.logout();
            window.location.href = "/login.html";
        }
    }, 1 * 60 * 1000);

})

.controller('hdrCtrl', function($scope, CommonService, $interval)
{
    var strUser = sessionStorage.getItem("strUser");
    if(strUser)
    {
        $scope.user = JSON.parse(strUser);
    }
    else
    {
        window.location.href = "/login.html";
        return;
    }

    $scope.vendorName = vendorName;
    
    $scope.SendHeadtbeat = function()
    {
        CommonService.heartbeat();
    }
    
    // 每10秒发送一次心跳，
    window.setInterval( $scope.SendHeadtbeat, 10 * 1000 );

    $scope.AppSystemUserInfo = JSON.parse(sessionStorage.getItem('AppSystemUserInfo'));
    $scope.ProjectName = $scope.user.user_service;

    $scope.projConfig = projConfigs[$scope.ProjectName];
    
    $scope.user = {};
    $scope.user = JSON.parse(sessionStorage.getItem('strUser'));
    if(!$scope.user.avatar)
    {
        $scope.user.avatar="/resource/images/default_avatar.jpg";
    }

    creatImg=document.getElementById("index_avatar");
    creatImg.src=$scope.user.avatar;
    $scope.onLogout = function () 
    {
        CommonService.logout();
    };

    angular.element(".userInfo").show();
});

function setMenu(path) 
{
    if ( null != _indexCtrl ) 
    {
        //$scope = _indexCtrl;
        _indexCtrl.setMenu(path);
    } 
    else 
    {
        _curPath = path;
    }
}

