
var CtrlBase = {
	CtrlBase : "i am here"
};

CtrlBase.getAgeById = function (id_no) {
	return calcAgeById(id_no);
};

CtrlBase.getSexById = function (id_no) {
	var sex = calcSexById(id_no);
	switch (sex) {
		case 1:
			return "男";
		case 1:
			return "女";
		default:
			return "未知";
	}
};

CtrlBase.getBirthday = function (id_no) {
	return calcBirthdayById(id_no);
};

CtrlBase.doSearch = function (scope) {
	scope = scope || this;
	if (scope.loadData) {
		if (scope.pager) {
			scope.pager.setPage(1);
		}
		scope.loadData();
	}
};

CtrlBase.getContent = function (content, length) {
	if (content && content.length > length) {
		return content.substr(0, length) + "...";
	}

	return content;
};

CtrlBase.getContent2 = function (content, length) {
	if (content && content.length > length) {
		return "...";
	}

	return content;
};

CtrlBase.getContentTitle = function (content, length) {
	if (content && content.length > length) {
		return content;
	}

	return "";
};

CtrlBase.search = function (searchMessage, scope) {
	this.doSearch(scope);
};

CtrlBase.searchKey = function ($event, scope) {
	if ($event.keyCode == 13) {
		this.doSearch(scope);
	}
};

CtrlBase.goBack = function ($event) {
	goBack()
};

CtrlBase.selectItem = function (item, cls) {
	var key = "$-selected-" + cls;
	var last = this[key];
	if (last) {
		last.$cls = "";
	}
	if (item)
	{
		item.$cls = cls;
		this[key] = item;
	}
	else
	{
		this[key] = null;
	}
};

CtrlBase.selectEl = function (selector, cls, add) {
	var key = selector;
	var $el = this[key];
	if (!$el) {
		$el = $(key);
		//this[key] = $el;
	}
	if (add)
	{
		$el.addClass(cls);
	}
	else
	{
		$el.removeClass(cls);
	}
};

CtrlBase.getUserImage = function (user, ageF, sexF) {
	if (user.avatar_url) {
		return user.avatar_url;
	}

	ageF = ageF || "age";
	sexF = sexF || "sex";

	var MAX_AGE = 200;
	var age = parseInt(user[ageF]);
	if (age > MAX_AGE) {
		age = MAX_AGE;
	}

	if (!window.ageAvatarMap) {
		window.ageAvatarMap = [];

		var ranges = [
			[5,		"/resource/images/5岁以下.png"],
			[25,	"/resource/images/6-25岁女.png"],
			[55,	"/resource/images/26-55岁女.png"],
			[200,	"/resource/images/56岁以上女.png"]
		];
		var map = new Array(MAX_AGE);
		var j = 0;
		for (var i = 0; i < MAX_AGE; i++) {
			if (i <= ranges[j][0]) {
				map[i] = ranges[j][1];
			} else {
				j++;
				i--;
				continue;
			}
		}
		window.ageAvatarMap[0] = map;

		var ranges = [
			[5,			"/resource/images/5岁以下.png"],
			[25,		"/resource/images/6-25岁男.png"],
			[55,		"/resource/images/26-55岁男.png"],
			[MAX_AGE,	"/resource/images/56岁以上男.png"]
		];
		var map = new Array(MAX_AGE);
		var j = 0;
		for (var i = 0; i < MAX_AGE; i++) {
			if (i <= ranges[j][0]) {
				map[i] = ranges[j][1];
			} else {
				j++;
				i--;
				continue;
			}
		}
		window.ageAvatarMap[1] = map;
	}

	if (user["sexF"] == "女") {
		return window.ageAvatarMap[0][age];
	} else {
		return window.ageAvatarMap[1][age];
	}
};

CtrlBase.todo = function (user, ageF, sexF) {
	MsgBox.info("提示", "功能完善中，敬请期待:)");
};

CtrlBase.__apply = function () {
	try {
		if (!this.$root.$$phase) {
			this.$apply();
		}
	} catch(e) {
	}
};

CtrlBase.updateParams = function (url, param, action) {
	var ctrlName = url + "Ctrl";

	if (param) {
		appCache.setObject(ctrlName + "Param", param);
	} else {
		appCache.removeItem(ctrlName + "Param");
	}

	if (action) {
		appCache.setItem(ctrlName + "Action", action);
	} else {
		appCache.removeItem(ctrlName + "Action");
	}
};

CtrlBase.gotoUrl = function (url, param, action) {
	var ctrlName = url;

	if (param instanceof String) {
		action = param;
		param = null;
	}

	if (param) {
		appCache.setObject(ctrlName + "Param", param);
	} else {
		appCache.removeItem(ctrlName + "Param");
	}

	if (action) {
		appCache.setItem(ctrlName + "Action", action);
	} else {
		appCache.removeItem(ctrlName + "Action");
	}

	window[ctrlName + "RequestCache"] = null;

	gotoUrl("#/" + url);
};

CtrlBase.getRequest = function (url, reset) {
	var ctrlName = url;
	return this.getRequestEx(ctrlName);
};

CtrlBase.getRequestEx = function (ctrlName, reset) {

	var ctrlName = ctrlName;
	var request;

	if (reset) {
		request = appCache.getObject(ctrlName + "Param") || {};
	} else {
		request = window[ctrlName + "RequestCache"] || appCache.getObject(ctrlName + "Param") || {};
	}

	request.action = appCache.getItem(ctrlName + "Action") || "";
	this.isAdd = (request.action == "add");
	if (request.action) {
		request[request.action] = true;
	}

	window[ctrlName + "RequestCache"] = request;

	return request;
};

CtrlBase.rmSecTime = function (time) {
	if (!time) {
		return "";
	}
	var r = time.match(/(^.*:[^:]*):[^:]*$/);
	if (!r) {
		return time;
	}
	return r[1];
}

CtrlBase.getShortDate = function (time) {
	if (!time) {
		return "";
	}

	if (!this.curYear) {
		this.curYear = (new Date()).format("yyyy");
	}

	var date, time;
	var splits = time.split(" ");
	if (splits.length >= 2) {
		var splits2 = splits[0].split("-");
		var year = "";
		if (splits2.length >= 3) {
			year = splits2[0];
			date = splits2[1] + "/" + splits2[2];
		}
		if (year != this.curYear) {
			date = year + "/" + date;
		}
		return date;
	}

	return "--/--";
};

CtrlBase.getShortTime = function (time) {
	if (!time) {
		return "";
	}
	if (!this.curYear) {
		this.curYear = (new Date()).format("yyyy");
	}

	var date, time;
	var splits = time.split(" ");
	if (splits.length >= 2) {
		var splits2 = splits[0].split("-");
		var year = "";
		if (splits2.length >= 3) {
			year = splits2[0];
			date = splits2[1] + "/" + splits2[2];
		}
		splits2 = splits[1].split(":");
		if (splits2.length >= 2) {
			time = splits2[0] + ":" + splits2[1];
		}

		if (year != this.curYear) {
			date = year + "/" + date;
		}

		return date + " " + time;
	}

	return "--/--";
};

CtrlBase.getDicts = function (type_codes, callback) {
	if (!callback) {
		callback = function (r) {
			if (!r) {
				MsgBox.error("错误", "获取数据字典失败");
			}
		}
	}
	getDictEx(this, this.CommonService, type_codes, callback);
};

CtrlBase.gotoPatDetail = function (hos_code, id_no) {
	gotoPatDetail(hos_code, id_no);
};

CtrlBase.setTitle = function (title) {
	this.title = title;
	document.title = title;
};

CtrlBase.getTimesUriData = function () {
	return getTimeRangeParam(this.stime, this.etime);
};

CtrlBase.getUriDataEx = function ($scope, pager) {
	var uriData = getTimeUriData($scope);
	uriData += getSearchUriData($scope);
	pager = pager || $scope.pager;
	if (pager && pager.getUriData) {
		uriData += pager.getUriData();
	}
	return uriData;
};

CtrlBase.onFileChange = function (id, func) {
	func = func || nullFn;
	var el = document.getElementById(id);
	var _this = this;
	el.onchange = function () {
		func(el);
		_this.__apply();
	};
};

CtrlBase.getFileSize = function (size) {
	var fileSize = "";
	if (size > 1000000) {
		fileSize = alignFloat(size/1000000, 100) + "MB";
	} else if (size > 1000){
		fileSize = alignFloat(size/1000, 100) + "KB";
	} else {
		fileSize = size + "B";
	}
	return fileSize;
};

CtrlBase.TO = function (func, t) {
	var _this = this;
	TO(function () {
		func();
		_this.__apply();
	}, t);
};

CtrlBase.init = function (CommonService) {
	if (CommonService) {
		CommonService.heartbeat();
		this.CommonService = CommonService;
	}
	this.user = appCache.getObject("strUser");
	if (!this.user) {
		if (!this.allowAnonymous) gotoLogin();
	} else {
		this.isAdmin = this.user.user_name == "admin";
	}
};

var ctrlExtend = function ($scope, CommonService, allowAnonymous) {

	for (var i in CtrlBase) {
		$scope[i] = CtrlBase[i];
	}

	$scope.allowAnonymous = allowAnonymous;
	$scope.init(CommonService);
};

var initCtrl = ctrlExtend;

var initListCtrl = function ($scope, CommonService, url, pagerName, itemName, notifyMsg) {

	pagerName = pagerName || "pager";
	itemName = itemName || "条目";

	$scope.searchText = "";

	$scope.pager = UsPager($scope, pagerName, function (id, page) {
		$scope.loadData();
	});

	$scope.loadData = function () {
		$scope.list = [];
		var uriData = "";
		if ($scope.uriData) {
			if (isFunc($scope.uriData)) {
				uriData = $scope.uriData();
			} else {
				uriData = $scope.uriData;
			}
		}
		uriData += CtrlBase.getUriDataEx($scope);
		CommonService.getAll(url,uriData,function(data){
			$scope.list = listToObject(data,"rows").rows;
			$scope.pager.setCount(data.count);
		}, function(data, status){
			svcError(data, status);
			$scope.list = [];
			$scope.pager.reset(0);
		});
	};

	var autoLoad = (notifyMsg === true || notifyMsg === false) ? notifyMsg : true;
	if (autoLoad) {
		$scope.loadData();
	}

	$scope.search = function () {
		$scope.pager.reset();
		$scope.loadData();
	};

	notifyMsg =(notifyMsg instanceof String) ? notifyMsg : undefined;
	initDelCtrl($scope, CommonService, url, itemName, notifyMsg);
};

var initDelCtrl = function ($scope, CommonService, url, itemName, notifyMsg) {
	itemName = itemName || "条目";
	notifyMsg = notifyMsg || ("确定要删除该" + itemName + "吗？删除后将无法恢复。");
	$scope._delete = function (item) {
		var data = {id : item.id};
		CommonService.deleteOne(url, S(data), function(data) {
			MsgBox.info("成功", "删除" + itemName + "成功");
		if ($scope.loadData) $scope.loadData();
		}, function(data, status) {
			MsgBox.error("错误", "删除" + itemName + "失败");
		});
	};
	$scope.delete = function (item) {
		MsgBox.confirm("确认", notifyMsg, function (r) {
			if (r) {
					$scope._delete(item);
			}
		}, "确定", "取消", "error");
	};
};
