//'use strict';

var app = angular.module('myloginApp', ['ngRoute','ngResource','loginControllers','LoginServices']);

app.config(['$routeProvider','$httpProvider',function ($routeProvider,$httpProvider) 
{
    $routeProvider
        .when('/', {
            title: '登录',
            templateUrl: 'login.html',
            controller:'loginCtrl'
        }).otherwise({

        });

    //跨域

    $httpProvider.defaults.useXDomain = true;

    delete $httpProvider.defaults.headers.common['X-Requested-With'];
    $httpProvider.defaults.headers.common['Content-Type']= 'application/json';
    $httpProvider.defaults.headers.common['app-key']='fb98ab9159f51fd0'; //(key)
    $httpProvider.defaults.headers.common['Authorization']='12345678'; //(key)

}]);

var loginControllers = angular.module('loginControllers',[]);

// 定义 Controller: loginCtrl  （登录）
loginControllers.controller('loginCtrl', ['$scope','$window','loginService',function ($scope,$window,loginService) 
{
    $scope.objLoginInfo = {};
    $scope.errorInfo    = '';
    $scope.msg          = "";
    $scope.projConfig   = projConfig;
    $scope.vendorName   = vendorName;
    
    var servicePort = window.location.port;

    $scope.gotologin = function(objLoginInfo) 
    {
        // 得到Key及iv
        var strMD5Passwd = CryptoJS.MD5(objLoginInfo.username + objLoginInfo.password).toString();
        var strRandomIV = randomKey(16);
        var strData = myURL.getHost();

        var key = CryptoJS.enc.Utf8.parse(strMD5Passwd);
        var iv = CryptoJS.enc.Utf8.parse(strRandomIV);

        var strAesEncrypted = CryptoJS.AES.encrypt(strData, key,
            { iv: iv, mode: CryptoJS.mode.CBC, padding: CryptoJS.pad.ZeroPadding});
        var strUserName = objLoginInfo.username;
        var strUserNameLength = ZeroPadding.left(strUserName.length, 3);

        var strDataPacket = strRandomIV + strUserNameLength + strAesEncrypted + strUserName;

        var url = strData;

        strDataPacket = url + "/" + Base64.encode(strDataPacket);

        loginService.getToken(strDataPacket, function (response, status, headers, config) 
        {

            if ( response.code != 0 )
            {
                // 登录失败
                $scope.loginError( response );
                return;
            } 

            sessionStorage.setItem('authToken', headers('Authorization'));

            // 用户的用户组信息
            if(response.userGroup)
                sessionStorage.setItem("userGroupInfo", JSON.stringify(response.userGroup));

            //缓存用户信息id, user_name, name,
            var a = listToObject(response,'rows').rows;
            var user = a[0];
            
            user.groupList = response.userGroup;

            // 保存用户登录的会话ID
            sessionStorage.setItem("loginSessionID", response.loginSessionID);

            // 保存数据字典
            dataDict = listToObject(response.dataDict, 'rows').rows;
            sessionStorage.setItem("dataDict", JSON.stringify(dataDict));

            // 保存系统用户信息
            var a = listToObject(response.systemUserInfo, 'rows').rows;
            AppSystemUserInfo = a[0];
            sessionStorage.setItem("AppSystemUserInfo", JSON.stringify(AppSystemUserInfo));

            // 用户是否为管理员组成员
            user.isAdmin = response.isAdmin;

            // 缓存用户信息
            sessionStorage.setItem('strUser', JSON.stringify(user));
            objLoginInfo.username   = user.name;
            objLoginInfo.Id         = user.id;
            
            sessionStorage.setItem("strUserName",objLoginInfo.username);
            sessionStorage.setItem("strUserId",objLoginInfo.Id);

            // 根据用户的系统业务类型显示相应的主页面
            projName = user['user_service'];
            projConfig = projConfigs[projName];

            if ( projName == "stock" )
                window.location.href = 'index-pear.html#/stockInfo';
            // else if ( projName == "IOT")
            //     window.location.href = "elecMap.html";
            else
                window.location.href = 'index.html#/toDo';
        },
        function(data)
        {
            $scope.loginError( data );
        });
    }

    // 电子地图
    if ( servicePort == "449")
    {
        $scope.objLoginInfo = {username: "elecMap", password: "elecMap"};
        $scope.gotologin($scope.objLoginInfo);
    }

    $scope.loginError = function( data )
    {
        if(data)
        {
            switch(data.code)
            {
            case 605:
                $scope.msg = "此账号已经被管理员禁用";
                break;

            default:
                $scope.msg = "用户名或密码错误！";
                break;
            }
        }
        else
        {
             $scope.msg = "网络连接错误或系统未运行";
        }
    }
}])


