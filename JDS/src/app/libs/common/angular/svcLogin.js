'use strict';

/* Services */

var LoginServices = angular.module('LoginServices', ['ngResource']);

LoginServices.factory('loginService', ['$http',function($http){

	var baseUrl  = getBaseUrl() + 'login/';

    //var baseUrl="http://192.168.10.32:82/hum/login/";
    //var baseUrl="http://localhost:82/hum/login/";

    return {
        getToken: function (uri, funSuccess, funError) {
            $http({
                method : "GET",
                headers : {'app-secret' : appConfig.appSec},
                url : baseUrl + uri
            }).success(funSuccess).error(funError);
        },
        refreshToken: function (uri, token, funSuccess, funError) {
            $http({
                method : "POST",
                headers : {
                	'app-secret' : appConfig.appSec,
                	'Authorization' : token
                },
                url : baseUrl + uri
            }).success(funSuccess).error(funError);
        }
    }
}]);
