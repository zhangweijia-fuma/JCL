//////////////////////////////////////////////////////////////////////////////////////////////////
//

'use strict';

window.baseUrl  = getBaseUrl();

var modules=['ngRoute',
	'ngResource',
	'ngSanitize',
	'CommonServices',
	'CommonDirectives',
	'angularFileUpload',
	'ui.select',

	// System 部分
	'indexControllers',
	'menusManageControllers',
	'dictManageControllers',
	'accountControllers',
	'OperationLogControllers',
	'userGroupManageControllers',
	'systemUserManageControllers',
	'areaCodeManageControllers',

	// Public部分
	'organizationManageControllers',
	'vendorManageControllers',
	'propertyManageControllers',
	'deviceTypeManageControllers',
	'componentManageControllers'
];

var regModules = [];
function registerModule(mod) 
{
	if (!mod || !appConfig || !appConfig.sysMap[mod.name])
		return;

	var m = mod.modules;
	for (var i = 0; i < m.length; i++)
		modules.push(m[i]);

	regModules[mod.name] = mod;
}


//////////////////////////////////////////////////////////////////////////////
// PIS 部分控制器
var PIS = 
{
	name : "PIS",
	modules : [
		'lineManageControllers',
		'controllerManageControllers',
		'ctrlLogManageControllers',
		'programManageControllers',
		'messageManageControllers',
		'lineStatusControllers',
		'videoSurvControllers',
		'elecMapControllers',
	],
	initRoute : function ($routeProvider) {
		//TODO
	}
};
registerModule(PIS);

var IOT = 
{
	name : "IOT",
	modules : [
		'gpsModuleControllers',
	],
	initRoute : function ($routeProvider) {
		//TODO
	}
}
registerModule(IOT);

var iMRO = 
{
	name : "iMRO",
	modules : [
	],
	initRoute : function ($routeProvider) {
		//TODO
	}
}
registerModule(iMRO);

var oa = 
{
	name : "oa",
	modules : [
		'oaBulletinBoardControllers',
	],
	initRoute : function ($routeProvider) {
		//TODO
	}
}
registerModule(oa);

var stock =
{
    name : "stock",
    modules : [
    	'stockControllers',
    ],
    initRoute : function ($routeProvider) {
            //TODO
    }
}
registerModule(stock);

var rsu =
{
    name : "rsu",
    modules : [
    	'rsuProjectManageControllers',
    	'rsuDeviceControllers',
    ],
    initRoute : function ($routeProvider) {
            //TODO
    }
}
registerModule(rsu);


var firstApp = angular.module( 'firstApp', modules);
firstApp.config(['$routeProvider','$httpProvider','$locationProvider', function($routeProvider,$httpProvider,$locationProvider) 
	{
		$routeProvider
		////////////////////////////////////////////////////////////////////////////////////////////////
	 	// 系统管理 SYSTEM
	 	// 
	 	// 数据字典管理相关开始
		.when('/dictManage', {
			title: '数据字典管理',
			templateUrl: './system/views/dictionary/dictionaryManage.html',
			controller: 'dictManageCtrl'
		// }).when('/edit_dict', {						 
		// 	title: '数据字典编辑',
		// 	templateUrl: './system/views/dictionary/editDictionary.html',
		// 	controller: 'editDictCtrl'
		// }).when('/update_dict', {
		// 	title: '数据字典编辑',
		// 	templateUrl: './system/views/dictionary/updateDictionary.html',
		// 	controller: 'editDictCtrl'

		}).when('/edit_dict_value', {
			title: '数据字典值编辑',
			templateUrl: './system/views/dictionary/editDictionaryValue.html',
			controller: 'editDictValueCtrl'		  //  数据字典管理相关结束
		// }).when('/shearphoto', {
		// 	title: '头像裁剪',
		// 	templateUrl: './system/views/sptest.html',
		// 	controller: 'meunsManageCtrl'
		// }).when('/shearphotodoc', {
		// 	title: '头像裁剪',
		// 	templateUrl: './system/views/sptestdoc.html',
		// 	controller: 'doctorAccountAddCtrl'
		// 	

		// 	菜单管理
		}).when('/menuManage', {
			title: '菜单管理',
			templateUrl: './system/views/menu/menuManage.html',
			controller: 'menuManageCtrl'
		}).when('/editMenuItem', {
			title: '编辑菜单项',
			templateUrl: './system/views/menu/editMenuItem.html',
			controller: 'editMeunItemCtrl'
		}).when('/editMenu', {
			title: '编辑菜单',
			templateUrl: './system/views/menu/editMenu.html',
			controller: 'editMenuCtrl'

		// 区域代码管理
		}).when('/areaCodeManage', {
			title: '区域代码管理',
			templateUrl: './system/views/areaCode/areaCodeManage.html',
			controller: 'areaCodeManageCtrl'


		// 系统用户管理
		}).when('/systemUserManage', {
			title: '系统用户管理',
			templateUrl: './system/views/systemUser/systemUserManage.html',
			controller: 'systemUserManageCtrl'
		}).when('/systemUserEdit', {
			title: '系统用户信息管理',
			templateUrl: './system/views/systemUser/systemUserEdit.html',
			controller: 'systemUserEditCtrl'


		// ///////////////////////////////////////////////////////////////////////////////////////////////
		// 公共系统管理部分
		// 用户组管理
		}).when('/groupManage', {
			title: '用户组管理',
			templateUrl: './system/views/userGroup/userGroupManage.html',
			controller: 'userGroupManageCtrl'
		}).when('/userGroupDetail', {
			title: '用户组详情',
			templateUrl: './system/views/userGroup/userGroupDetail.html',
			controller: 'userGroupDetailCtrl'
		}).when('/userGroupUpdate', {
			title: '用户组编辑',
			templateUrl: './system/views/userGroup/userGroupAdd.html',
			controller: 'userGroupAddCtrl'
		}).when('/userGroupAdd', {
			title: '新增用户组',
			templateUrl: './system/views/userGroup/userGroupAdd.html',
			controller: 'userGroupAddCtrl'

		// 操作日志
		}).when('/operationLog', {
			title: '操作日志',
			templateUrl: './system/views/operationLog/operationLog.html',
			controller: 'OperationLogCtrl'
		}).when('/check_operationLog', {
			title: '查看操作日志',
			templateUrl: './system/views/operationLog/checkOperationLog.html',
			controller: 'OperationLogCtrl'

		// 账户管理相关开始
		}) .when('/accountManage', {  
			title: '账户管理',
			templateUrl: './system/views/account/account.html',
			controller: 'accountManageCtrl'
		}).when('/add_account', {
			title: '新增账户',
			templateUrl: './system/views/account/accountAdd.html',
			controller: 'accountAddCtrl'
		}).when('/update_account', {
			title: '修改账户信息',
			templateUrl: './system/views/account/accountUpdate.html',
			controller:'accountUpdateCtrl'
		}).when('/account_detail', {
			title: '个人信息',
			templateUrl: './system/views/account/accountDetail.html',
			controller: 'accountDetailCtrl'

		//账户管理相关结束


		////////////////////////////////////////////////////////////////////////////////////////////////
		// Public 部分
		//
		}).when('/toDo', {
			title: '个人信息',
			templateUrl: './system/views/account/accountDetail.html',
			controller: 'accountDetailCtrl'
		}).when('/OrganizationManage', {
			title: '组织团体管理',
			templateUrl: './public/views/organizationManage.html',
			controller: 'organizationManageCtrl'
		}).when('/organizationEdit', {
			title: '组织团体信息',
			templateUrl: './public/views/organizationEdit.html',
			controller: 'organizationEditCtrl'
		}).when('/vendorManage', {
			title: '供应商管理',
			templateUrl: './public/views/vendorManage.html',
			controller: 'vendorManageCtrl'
		}).when('/vendorEdit', {
			title: '供应商信息',
			templateUrl: './public/views/vendorEdit.html',
			controller: 'vendorEditCtrl'
		}).when('/propertyTypeManage', {
			title: '属性管理',
			templateUrl: './public/views/propertyManage.html',
			controller: 'propertyManageCtrl'
		}).when('/propertyEdit', {
			title: '属性信息',
			templateUrl: './public/views/propertyEdit.html',
			controller: 'propertyEditCtrl'

		}).when('/deviceTypeManage', {
			title: '设备类型管理',
			templateUrl: './public/views/deviceTypeManage.html',
			controller: 'deviceTypeManageCtrl'
		}).when('/componentTypeManage', {
			title: '组件管理',
			templateUrl: './public/views/componentManage.html',
			controller: 'componentManageCtrl'


		////////////////////////////////////////////////////////////////////////////////////////////////
		// PIS 部分
		//
		}).when('/lineManage', {
			title: '线路管理',
			templateUrl: './pis/views/line/lineManage.html',
			controller: 'lineManageCtrl'
		}).when('/line_add', {
			title: '创建运营线路',
			templateUrl: './pis/views/line/lineAdd.html',
			controller: 'lineAddCtrl'
		}).when('/station_add', {
			title: '车站信息',
			templateUrl: './pis/views/line/stationAdd.html',
			controller: 'stationAddCtrl'

		}).when('/controller_add', {
			title: '创建控制器',
			templateUrl: './pis/views/controller/controllerAdd.html',
			controller: 'controllerAddCtrl'
		}).when('/ctrl_log_query', {
			title: '控制器日志查询',
			templateUrl: './pis/views/log/logQuery.html',
			controller: 'ctrlLogQueryCtrl'

		}).when('/runningStatus', {
			title: '系统运行状态',
			templateUrl: './pis/views/line/lineStatus.html',
			controller: 'lineStatusCtrl'

		}).when('/controllerStatus', {
			title: '控制器运行状态',
			templateUrl: './pis/views/line/controllerStatus.html',
			controller: 'controllerStatusCtrl'
		}).when('/videoSurv', {
			title: '视频监播',
			templateUrl: './pis/views/line/videoSurv.html',
			controller: 'videoSurvCtrl'

		}).when('/programManage', {
			title: '节目单管理',
			templateUrl: './pis/views/program/programManage.html',
			controller: 'programManageCtrl'
		}).when('/program_detail', {
			title: '节目单详情',
			templateUrl: './pis/views/program/programDetail.html',
			controller: 'programDetailCtrl'
		}).when('/programFileManage', {
			title: '节目文件管理',
			templateUrl: './pis/views/program/programFileManage.html',
			controller: 'programFileManageCtrl'
		}).when('/formatFileManage', {
			title: '版式文件管理',
			templateUrl: './pis/views/program/formatFileManage.html',
			controller: 'formatFileManageCtrl'

		}).when('/sendListToController', {
			title: '信息发送',
			templateUrl: './pis/views/message/sendInfoToController.html',
			controller: 'sendInfoToControllerCtrl'
		}).when('/sendMessageToController', {
			title: '发送消息',
			templateUrl: './pis/views/message/sendInfoToController.html',
			controller: 'sendInfoToControllerCtrl'

		}).when('/messageQuery', {
			title: '消息查询',
			templateUrl: './pis/views/message/messageQuery.html',
			controller: 'messageQueryCtrl'


		////////////////////////////////////////////////////////////////////////////////////////////////
		// IOT 部分
		//
		}).when('/gpsModuleManage', {
			title: '定位模块管理',
			templateUrl: './iot/views/gpsModuleManage.html',
			controller: 'gpsModuleManageCtrl'
		}).when('/gpsModuleMap', {
			title: '定位模块位置地图',
			templateUrl: './iot/views/gpsModuleMap.html',
			controller: 'gpsModuleMapCtrl'

		}).when('/gpsModuleTrace', {
			title: '定位模块轨迹',
			templateUrl: './iot/views/gpsModuleTrace.html',
			controller: 'gpsModuleTraceCtrl'

		}).when('/elecMapShow', {
			title: '电子地图',
			templateUrl: './pisAudio/views/elecMap.html',
			controller: 'elecMapCtrl'


		////////////////////////////////////////////////////////////////////////////////////////////////
		// iMARO 部分
		//

		////////////////////////////////////////////////////////////////////////////////////////////////
		// OA 部分
		//
		}).when('/oaBulletinBoard', {
			title: '公司公告牌',
			templateUrl: './oa/views/oaBulletinBoard.html',
			controller: 'oaBulletinBoardCtrl'

		}).when('/oaViewBulletinBoard', {
			title: '公司公告牌',
			templateUrl: './oa/views/oaViewBulletinBoard.html',
			controller: 'oaViewBulletinBoardCtrl'

		}).when('/BulletinBoardManage', {
			title: '公司公告牌管理',
			templateUrl: './oa/views/BulletinBoardManage.html',
			controller: 'BulletinBoardManageCtrl'

		}).when('/EditBulletinBoard', {
			title: '发布公告',
			templateUrl: './oa/views/EditBulletinBoard.html',
			controller: 'EditBulletinBoardCtrl'

	    ////////////////////////////////////////////////////////////////////////////////////////////////
	    // STOCK 部分
	    //
	    }).when('/stockInfo', {
	        title: '信息',
	        templateUrl: './stock/views/stockManage.html',
	        controller: 'stockCtrl'


	    ////////////////////////////////////////////////////////////////////////////////////////////////
	    // RSU 部分
	    //
	    }).when('/RSUProjectManage', {
	        title: 'RSU项目管理',
	        templateUrl: './rsu/views/rsuProjectManage.html',
	        controller: 'rsuProjectManageCtrl'

	    }).when('/RSUDeviceManage', {
	        title: 'RSU设备管理',
	        templateUrl: './rsu/views/rsuDeviceManage.html',
	        controller: 'rsuDeviceManageCtrl'

	    }).when('/RSUDeviceLocation', {
	        title: 'RSU设备地图',
	        templateUrl: './rsu/views/rsuDeviceLocation.html',
	        controller: 'rsuDeviceLocationCtrl'

	    }).when('/RSUDeviceConfig', {
	        title: 'RSU设备设置',
	        templateUrl: './rsu/views/rsuDeviceConfig.html',
	        controller: 'rsuDeviceConfigCtrl'

	    }).when('/RSUDeviceEdit', {
	        title: 'RSU设备信息',
	        templateUrl: './rsu/views/rsuDeviceEdit.html',
	        controller: 'rsuDeviceEditCtrl'

	////////////////////////////////////////////////////////////////////////
	});

	for (var i in regModules) 
	{
		var rm = regModules[i];
		if (isFunc(rm.initRoute)) 
		{
			rm.initRoute($routeProvider);
		}
	}

	$routeProvider.otherwise({
		redirectTo: '/'
	});

	//跨域
	$httpProvider.defaults.useXDomain = true;

	delete $httpProvider.defaults.headers.common['X-Requested-With'];
	$httpProvider.defaults.headers.common['Content-Type']= 'application/json';
	$httpProvider.defaults.headers.common['app-key']='fb98ab9159f51fd0'; //(key)
	//$httpProvider.defaults.headers.common['Authorization']='12345678'; //(key)

}]);

firstApp.factory('mainAppInstance', function() 
	{
		return { data:"" }
	}
);

firstApp.run(['$location', '$rootScope', function($location, $rootScope) 
	{
		$rootScope.$on('$routeChangeSuccess', function(currentRoute, routes) 
			{
				if(routes.$$route)
				{
					$rootScope.title = routes.$$route.title;
					document.title = routes.$$route.title;
				}
				invokeRouteChangeCallback(currentRoute, routes);
			}
		);

		$rootScope.$on('$locationChangeStart', function(event, newUrl)
			{
			//console.log(newUrl);
			//var urls=newUrl.split('#');
			//window.location.href=urls[0]+"#/authorizer";
			//event.preventDefault();  //阻止页面切换
			}
		);
	}
]);

//global route change callbacks
var RouteChangeCallbacks = [];

function registerRouteChangeCallback (callback, auto) 
{
	auto = (false === auto) ? false : true;

	var cb = {
		auto : auto,
		callback : callback,
	};

	var r = function () 
	{
		for (var i in RouteChangeCallbacks) 
		{
			if (cb == RouteChangeCallbacks[i]) 
			{
				RouteChangeCallbacks.splice(i, 1);
				return true;
			}
		}
		return false;
	};

	RouteChangeCallbacks.push(cb);
	return r;
}

function unregisterRouteChangeCallback (callback) 
{
	var count = RouteChangeCallbacks.length;
	for (var i = count - 1; i >= 0; i--) 
	{
		if (callback == RouteChangeCallbacks[i].callback) 
		{
			RouteChangeCallbacks.splice(i, 1);
		}
	}
}

function invokeRouteChangeCallback (currentRoute, routes) 
{
	// D("Route change done : " + location.href);
	var count = RouteChangeCallbacks.length;
	for (var i = count - 1; i >= 0; i--) 
	{
		if (RouteChangeCallbacks[i].callback) 
		{
			RouteChangeCallbacks[i].callback(currentRoute, routes)
		}
		//auto mode, remove it
		if (RouteChangeCallbacks[i].auto) 
		{
			RouteChangeCallbacks.splice(i, 1);
		}
	}
}
