/**
 * Copyright 2016, Unicosense Co., Ltd
 * All rights reserved.
 *
 * @common pager
 */


/*
 usage, e.g. =>

 html :
	<div class="row text-center">
		<us-pager id="msgsPager"> </us-pager>
	</div>

 html template, see angular/drtCommon.js

 js :
 	//params: $scope + "pager id" + "page change callback"
	$scope.pager = UsPager($scope, "msgsPager", function (id, page) {
		$scope.loadData();
	});

	$scope.loadData = function () {
		var uriData = $scope.pager.getUriData();
	    CommonService.getAll('xxxxx', uriData, function(data){
			$scope.pager.setCount(data.count);
	    }, function(data, status){
	    });
	};

	$scope.loadData();
*/

function UsPager($scope, pagerId, listener, options) {
	if (!(this instanceof UsPager)){
		return new UsPager($scope, pagerId, listener, options);
	}

	pagerId = pagerId || "defaultPager";

	this.pageIndex = 1;
	this.pageSize = 8;
	this.totalPage = 0;
	this.pageNumbers = [];
    this.pagerId = pagerId;
    this.listener = listener;
    this.$scope = $scope;
    $scope["pageCount" + pagerId] = 0;
    $scope["pageNumbers" + pagerId] = this.pageNumbers;

    if (typeof options == "object") {
    	this.pageSize = options.size || this.pageSize;
    	this.rePageNo = options.remember == true;
    } else {
    	this.rePageNo = options == true;
    }

	if (this.rePageNo) {
		var page = appCache.getItem("pager_index/" + this.pagerId);
		if (page) {
			this.pageIndex = parseInt(page);
		}
	}
    $scope.pagerEvent = UsPager.pagerEvent.bind($scope);
    if (!$scope.__UsPagers) {
    	$scope.__UsPagers = {};
    }
    $scope.__UsPagers[pagerId] = this;
    this.setCount(0);
};

UsPager.prototype.setPageSize = function(size) {
	this.pageSize = size;
};

UsPager.prototype.getPageSize = function() {
	return this.pageSize;
};

UsPager.prototype.getPage = function() {
	return this.pageIndex;
};

UsPager.prototype.reset = function() {
	this.pageIndex = 1;
	//this.pageSize = 10;
	this.totalPage = 0;
};

UsPager.prototype.setLoading = function (loading) {
	if (undefined === loading) {
		loading = true;
	}
	this.loading = loading;
};

UsPager.prototype.setPage = function(page, callback) {
	if (this.rePageNo) {
		appCache.setItem("pager_index/" + this.pagerId, "" + page);
	}
	if (page != this.pageIndex) {
		this.pageIndex = page;
		this.selectActiviePage();
		if (callback && this.listener) {
			this.listener(this.pagerId, page);
		}
	}
};

//return uri params
UsPager.prototype.getUriData = function () {
	return "&o=" + this.pageIndex + "&r=" + this.pageSize;
};

//return the pager index
UsPager.prototype.pagerEvent = function (e) {
	var pageIndex = this.pageIndex;
	if(e.target.getAttribute("id") == "first"){
		pageIndex = 1;
	} else if(e.target.getAttribute("id") == "last"){//尾页
		pageIndex = this.totalPage;
	} else if(e.target.getAttribute("id") == "prev"){//上一页
		if(pageIndex == 1){
			pageIndex = 1 ;
		}else{
			pageIndex -= 1;
		}
	} else if (e.target.getAttribute("id") == "next"){//下一页
		if (pageIndex == this.totalPage){
			pageIndex = this.totalPage ;
		} else {
			pageIndex += 1;
		}
	} else {//点数字
		pageIndex = Number(e.target.getAttribute("id"));
	}
	this.setPage(pageIndex, true);
	return pageIndex;
};

UsPager.pagerEvent = function(e, id){
	if (e == undefined) {
		return 0;
	}

	var pager = this.__UsPagers[id];

	pager.pagerEvent(e);
};

UsPager.prototype.getElPrefix = function () {
	if (this.pagerId) {
		return "#" + this.pagerId + " li ";
	} else {
		return "";
	}
};

UsPager.prototype.selectActiviePage = function () {
	var _this = this;
    var delayFunc = function () {
        //改变页码样式
		var pgId = _this.getElPrefix();
        angular.element(pgId + ".active").remove("active").css("background","");
        angular.element(pgId + "#" + _this.pageIndex).addClass("active").css("background","#f70");
    };
    setTimeout(delayFunc, 0);
};

// count: count of all items
UsPager.prototype.setCount = function (count) {
	var pageIndex = this.pageIndex;
	var totalPage = this.totalPage = Math.ceil(count / this.pageSize);

	var pgId = this.getElPrefix();

    if(totalPage <= 3){
        if (totalPage == 0){
            this.pageNumbers = [];
        } else if (totalPage == 1){
            this.pageNumbers = [1];
        }else if(totalPage == 2){
            this.pageNumbers = [1, 2];
        }else if(totalPage == 3){
            this.pageNumbers = [1, 2, 3];
        }
        angular.element(pgId + "#first").hide();
        angular.element(pgId + "#prev").hide();
        angular.element(pgId + "#last").hide();
        angular.element(pgId + "#next").hide();
        angular.element(pgId + "#beforeSL").hide();
        angular.element(pgId + "#afterSL").hide();
    } else {
        if (pageIndex < 2) {
            this.pageNumbers = [1, 2, 3];
            //首页和上一页禁用
            angular.element(pgId + "#first").hide();
            angular.element(pgId + "#prev").hide();
            angular.element(pgId + "#last").show();
            angular.element(pgId + "#next").show();
            //前一个省略号隐藏，后一个省略号显示
            angular.element(pgId + "#beforeSL").hide();
            angular.element(pgId + "#afterSL").show();
        } else if (pageIndex > 1 && pageIndex < totalPage) {
            this.pageNumbers = [pageIndex - 1, pageIndex, pageIndex + 1];
            //首页和上一页禁用解除
            angular.element(pgId + "#first").show();
            angular.element(pgId + "#prev").show();
            //两个一个省略号显示
            angular.element(pgId + "#beforeSL").show();
            angular.element(pgId + "#afterSL").show();
            //尾页页和下一页禁用解除
            angular.element(pgId + "#last").show();
            angular.element(pgId + "#next").show();
        } else {
            this.pageNumbers = [totalPage - 2, totalPage - 1, totalPage];
            //尾页页和下一页禁用
            angular.element(pgId + "#last").hide();
            angular.element(pgId + "#next").hide();
            //首页和上一页禁用解除
            angular.element(pgId + "#first").show();
            angular.element(pgId + "#prev").show();
            //前一个省略号显示，后一个省略号隐藏
            angular.element(pgId + "#beforeSL").show();
            angular.element(pgId + "#afterSL").hide();
        }
    }
	this.loading = false;

    this.$scope["pageCount" + this.pagerId] = totalPage;
    this.$scope["pageNumbers" + this.pagerId] = this.pageNumbers;

    this.selectActiviePage();

    if (totalPage > 0 && pageIndex > totalPage) {
    	this.setPage(1, true);
    }

    return this;
};

UsPager.prototype.getItemIndex = function (idx) {
	return (this.pageIndex - 1) * this.pageSize + idx;
};

UsPager.clearSavedPageNo = function (pager) {
	if (pager instanceof Array) {
		for (var i in pager) {
			appCache.removeItem("pager_index/" + pager[i]);
		}
	} else {
		appCache.removeItem("pager_index/" + pager);
	}
};

var UsSimplePager_setCount = function(count) {
	var pageIndex = this.pageIndex;
	var totalPage = this.totalPage = Math.ceil(count / this.pageSize);

	var pgId = "#" + this.pagerId + " ";

    if(totalPage <= 1){
        angular.element(pgId + "#prev").addClass("disabled");
        angular.element(pgId + "#next").addClass("disabled");
    } else {
    	if (pageIndex == 1) {
			angular.element(pgId + "#prev").addClass("disabled");
    	} else {
			angular.element(pgId + "#prev").removeClass("disabled");
    	}

    	if (pageIndex >= totalPage) {
			angular.element(pgId + "#next").addClass("disabled");
    	} else {
			angular.element(pgId + "#next").removeClass("disabled");
    	}
    }

    this.$scope["pageCount" + this.pagerId] = totalPage;

    if (totalPage > 0 && pageIndex > totalPage) {
    	this.setPage(totalPage, true);
    }

    return this;
};

function UsSimplePager($scope, pagerId, listener, rememberPageNo) {
	var pager = UsPager($scope, pagerId, listener, rememberPageNo);
	pager.setCount = UsSimplePager_setCount;
	return pager;
}