'use strict';

/**
 *
 *  @模块名称: Common Services
 *  @版 本 号: V1.0.0
 *  @作    者: Xu Kun
 *  @作者邮件:
 *  @修改日期: 2014-08-15
 *  @版本描述:
 *
 */

function getAuthToken() {
	return sessionStorage.getItem('authToken');
}

var CommonServices = angular.module('CommonServices', []);
//定义通用服务：CommonService
CommonServices.factory('CommonService', ['$http', function ($http) 
{
    var isLogout = false;

    var baseUrl = getBaseUrl();

    //定义通用服务CommonService提供的方法
    return {

    	//0.Set base url
    	setBaseUrl : function (url) 
        {
    		baseUrl = url;
    	},

        //1.Get all obj
        getAll: function(uri, data, funSuccess, funError) 
        {
            if (data != undefined) {
                return $http({
                    method: "GET",
                    headers: {'Authorization': getAuthToken()},
                    url: baseUrl + uri + '?' + data
                }).success(funSuccess).error(funError);
            } else {
                return $http({
                    method: "GET",
                    headers: {'Authorization': getAuthToken()},
                    url: baseUrl + uri
                }).success(funSuccess).error(funError);
            }
        },

        //1_2.Get all obj
        getAllEx: function(url, data, funSuccess, funError) 
        {
            return $http({
                method: "GET",
                headers: {'Authorization': getAuthToken()},
                url: (data != undefined) ? (url + '?' + data) : url
            }).success(funSuccess).error(funError);
        },

        //2.Get one obj
        getOne: function (uri, data, funSuccess, funError) 
        {
            if (data != undefined) 
            {
                return $http({
                    method: "GET",
                    headers: {'Authorization': getAuthToken()},
                    url: baseUrl + uri + '?' + data
                }).success(funSuccess).error(funError);
            } 
            else 
            {
                return $http({
                    method: "GET",
                    headers: {'Authorization': getAuthToken()},
                    url: baseUrl + uri
                }).success(funSuccess).error(funError);
            }
        },

        //3.Update one obj
        updateOne: function (uri, data, funSuccess, funError) 
        {
            //alert("执行："+baseUrl + uri);
            if (data != undefined) 
            {
                // console.log("客户端向服务器端传递的数据：" + data);
                return $http({
                    method: "PUT",
                    url: baseUrl + uri,
                    headers: {'Authorization': getAuthToken()},
                    data: data    /*从页面提交的数据，保存在http data域*/
                }).success(funSuccess).error(funError);
            }
        },

        //4.Update part of one obj
        updatePartOne: function (uri, data, funSuccess, funError) 
        {
            if (data != undefined) 
            {
                return $http({
                    method: "patch",
                    url: baseUrl + uri,
                    headers: {'Authorization': getAuthToken()},
                    data: data    /*从页面提交的数据，保存在http data域*/
                }).success(funSuccess).error(funError);
            }
        },

        //5.Delete one
        deleteOne: function (uri, data, funSuccess, funError) 
        {
            if (data != undefined) 
            {
                return $http({
                    method: "DELETE",
                    headers: {'Authorization': getAuthToken()},
                    url: baseUrl + uri,
                    data: data
                }).success(funSuccess).error(funError);
            } else {
                return $http({
                    method: "DELETE",
                    headers: {'Authorization': getAuthToken()},
                    url: baseUrl + uri
                }).success(funSuccess).error(funError);
            }
        },

        //6.Add new one
        createOne: function (uri, data, funSuccess, funError) 
        {
            if (data != undefined) 
            {
                return $http({
                    method: "POST",
                    headers: {'Authorization': getAuthToken()},
                    url: baseUrl + uri,
                    //url: baseUrl + 'test',
                    data: data        /*从页面提交的数据，保存在http data域*/
                }).success(funSuccess).error(funError);
            }
        },

        //7.get data from json file
        getJson: function (uri, data, funSuccess, funError) 
        {
            if (data != undefined) 
            {
                return $http({
                    method: "GET",
                    headers: {'Authorization': getAuthToken()},
                    url: testBaseUrl + uri + '.json',
                    dataType: 'json'
                }).success(funSuccess).error(funError);
            } else {
                return $http({
                    method: "GET",
                    headers: {'Authorization': getAuthToken()},
                    url: uri + '.json',
                    dataType: 'json'
                }).success(funSuccess).error(funError);
            }
        },

        //8.save form data to json file
        createJson: function (uri, data, funSuccess, funError) 
        {
            if (data != undefined) 
            {
                return $http({
                    method: "POST",
                    headers: {'Authorization': getAuthToken()},
                    url: uri + '.json',
                    data: JSON.stringify(data), /*从页面提交的数据，保存在http data域*/
                    contentType: 'application/json; charset=utf-8'
                }).success(funSuccess).error(funError);
            }
        },

        heartbeat: function () 
        {
            if ( isLogout == true )
                return;

            // 心跳消息中，包含本次登录的会话ID
            var loginSessionID = sessionStorage.getItem('loginSessionID');
            return $http({
                method: "GET",
                headers: {'Authorization': getAuthToken()},
                url: baseUrl + "login/heartbeat?" + "loginSessionID=" + loginSessionID
            }).success(function(){
                // 重置cookie
                var Authorization=getAuthToken()
                cookieOperate.setCookie('authToken',Authorization);
            }).error(function (data, status) {
                // 跳转到login
                if (status == 401) {
                    window.location.href = "/login.html"
                }
            });
        },

        logout: function()
        {
            var loginSessionID = sessionStorage.getItem("loginSessionID");
            var uriData = 'loginSessionID=' + loginSessionID;

            return $http(
            {
                method: "GET",
                headers: {'Authorization': getAuthToken()},
                url: baseUrl + "logout?" + "loginSessionID=" + loginSessionID
            }).success(function()
            {
                // 重置cookie
                var Authorization = getAuthToken()
                cookieOperate.setCookie('authToken',Authorization);

                isLogout = true;

                // 跳转到login
                window.location.href = "/login.html";
            }).error(function (data, status)
            {
                // 跳转到login
                if (status == 401) {
                    window.location.href = "/login.html"
                }
            });

        }
    }
}]);


