'use strict';

var ABC = "ABCDEFGHIJKLMNOPQ";

// 生成16位随机key
var randomKey = function (length) {
	//var chars = "abcdefghijkmnpqrstuvwxyzABCDEFGHJKMNPQRSTUVWXYZ0123456789`=//[];',./~!@#$%^&*()_+|{}:<>?";
	var chars = "abcdefghijkmnpqrstuvwxyzABCDEFGHJKMNPQRSTUVWXYZ0123456789";

	var s = "";

	for (var i = 0; i < length; i++) {
		s += chars.charAt(Math.ceil(Math.random() * 1000) % chars.length);
	}
	return s;
};

var randomDigitKey = function (length) {
	//var chars = "abcdefghijkmnpqrstuvwxyzABCDEFGHJKMNPQRSTUVWXYZ0123456789`=//[];',./~!@#$%^&*()_+|{}:<>?";
	var chars = "0123456789";

	var s = "";

	for (var i = 0; i < length; i++) {
		s += chars.charAt(Math.ceil(Math.random() * 1000) % chars.length);
	}
	return s;
};


//取得Request参数
var Request = {
	get: function (paras) {
		var url = location.href;
		var paraString = url.substring(url.indexOf("?") + 1, url.length).split("&");
		var paraObj = {};
		var i, j;
		for (var i = 0; j = paraString[i]; i++) {
			paraObj[j.substring(0, j.indexOf("=")).toLowerCase()] = j.substring(j.indexOf("=") + 1, j.length);
		}
		var returnValue = paraObj[paras.toLowerCase()];
		if (typeof (returnValue) == "undefined") {
			return "";
		} else {
			return returnValue;
		}
	}
};

//字符串补零
var ZeroPadding = {
	left: function (str, length) {
		if (str.length >= length)
			return str;
		else
			return ZeroPadding.left("0" + str, length);
	},
	right: function (str, length) {
		if (str.length >= length)
			return str;
		else
			return ZeroPadding.right(str + "0", length);
	}
};


//获得URL相关
var myURL = {
	getAll: function () {
		return window.location.href;
	},
	getProtocol: function () {
		return window.location.protocol;

	},
	getHost: function () {
		return window.location.host;
	},
	getPort: function () {
		return window.location.port;
	},
	getPathName: function () {
		return window.location.pathname;
	},
	getSearch: function () {
		return window.location.search
	},
	getHash: function () {
		return window.location.hash;
	},
	getPath: function () {
		var hash = window.location.hash;
		if (!hash || !hash.length) {
			return null;
		}
		var reg = new RegExp("#(/[^#&?]+).*");
		var r = hash.match(reg);
		if(r != null)
			return unescape(r[1]);
		return null;
	},
	getParam: function (name) {
		var reg = new RegExp("(^|&?)"+ name +"=([^&]*)(&|$)");
		var r = window.location.href.substr(1).match(reg);
		if(r != null)
			return unescape(r[2]);
		return null;
	},
	getBaseUrl: function() {
		return window.location.protocol + "//" + window.location.hostname;
	}
};

var Url = myURL;

//本地数据存储
var localDataStorage = {
	setItem: function (key, value) {
		localStorage.setItem(key, value);
	},
	getItem: function (key) {
		return localStorage.getItem(key);
	},
	setObject: function (key, value) {
		localStorage.setItem(key, JSON.stringify(value));
	},
	getObject: function (key) {
		return JSON.parse(localStorage.getItem(key));
	},
	removeItem: function (key) {
		return localStorage.removeItem(key);
	},
	clearItem: function () {
		localStorage.clear();
	},
	setUnit8Array: function (key, array) {
		var r = [];
		for (var i = 0; i < array.length; i++) {
			r.push(String.fromCharCode(array[i]));
		}

		var s = r.join('');
		localStorage.setItem(key, s);
	},
	getUnit8Array: function (key) {
		var str = localStorage.getItem(key);
		if (!str) {
			return null;
		}
		var r = new Uint8Array(str.length);
		for (var i = 0; i < str.length; i++) {
			r[i] = str.charCodeAt(i);
		}
		return r;
	},
};

var appCache = {
	setItem: function (key, value) {
		sessionStorage.setItem(key, value);
	},
	getItem: function (key, def) {
		var str = sessionStorage.getItem(key);
		if (str) {
			return str;
		}
		return def || null;
	},
	setObject: function (key, value) {
		sessionStorage.setItem(key, JSON.stringify(value));
	},
	getObject: function (key) {
		var str = sessionStorage.getItem(key);
		if (!str) {
			return null;
		}
		return JSON.parse(str);
	},
	setUnit8Array: function (key, array) {
		var r = [];
		for (var i = 0; i < array.length; i++) {
			r.push(String.fromCharCode(array[i]));
		}

		var s = r.join('');
		sessionStorage.setItem(key, s);
	},
	getUnit8Array: function (key) {
		var str = sessionStorage.getItem(key);
		if (!str) {
			return null;
		}
		var r = new Uint8Array(str.length);
		for (var i = 0; i < str.length; i++) {
			r[i] = str.charCodeAt(i);
		}
		return r;
	},
	removeItem: function (key) {
		return sessionStorage.removeItem(key);
	},
	clear: function () {
		return sessionStorage.clear();
	},
};

//cookie操作
var cookieOperate = {
    setCookie: function (name, value) {
        var exp = new Date();
        exp.setTime(exp.getTime()+0.5*60*60*1000);
        document.cookie = name + "=" + escape(value) + ";expires="+exp.toGMTString();
    },
    getCookie: function (name) {
        var arr, reg = new RegExp("(^| )" + name + "=([^;]*)(;|$)");

        if (arr = document.cookie.match(reg))

            return (arr[2]);
        else
            return null;
    },
    delCookie: function (name) {
        var exp = new Date();
        exp.setTime(exp.getTime() - 1);
        var cval = cookieOperate.getCookie(name);
        if (cval != null) {
            document.cookie = name + "=" + cval + ";expires=" + exp.toGMTString();
        }
    }
};



//error处理
function errorOperate(response) {
   var errorCode=response.code;
    if(errorCode=='602'){
        $('#denglu').show();
    }else if(errorCode=='603'){
        alert('无法识别的用户名或密码!');
        $('#denglu').show();
    }
}

//controller init
function ctrInit(){
    //隐藏广告栏
    $('#banner').hide();
    //去除滚动条事件
    if(myURL.getAll().match('/#/productDetail')==null){
          $(window).unbind('scroll');
    }
}

//check sql injection
function checkSqlInjection(inputStr){
    var resultStr = inputStr.toLowerCase();
    var alertStr = "";

    var vFit = "'|and|exec|insert|select|delete|update|count|*|%|chr|mid|master|truncate|char|declare|; |or|-|+|,";
    var vFitter = vFit.split("|");
    var vFitterLen = vFitter.length;
    for(var vi=0; vi<vFitterLen; vi++){
        if(resultStr.indexOf(vFitter[vi]) >= 0){
            alertStr += vFitter[vi] + " ";
        }
    }
    if(alertStr == ""){
        return true;
    }else{
        alert("输入中不能包含如下字符：" + alertStr);
        return false;
    }
}

/**
 * 产生分页器显示数
 * @param page 请求的页号
 * @param pageSize 每页的记录数
 * @param recordCount 总的记录数
 * @param bursterMaxPage 分页可显示的最大页数
 * @return 分页器显示数
 */
function  _produceBurster(page,pageSize,recordCount,bursterMaxPage) {

   var bursterPageNumbers =[];

    var recordMaxPage=Math.ceil(recordCount/pageSize);

    if(bursterMaxPage>recordMaxPage){
        for(var i=0;i<recordMaxPage;i++){
            bursterPageNumbers[i] = i;
        }
    }else {
        if (page < Math.ceil(bursterMaxPage / 2)) {
            for (var i = 0; i < bursterMaxPage; i++) {
                bursterPageNumbers[i] = i;
            }
        } else if (page < recordMaxPage - Math.ceil(bursterMaxPage / 2)) {
            for (var i = 0, j = -Math.floor(bursterMaxPage / 2); i < bursterMaxPage; i++, j++) {
                bursterPageNumbers[i] = page + j;
            }
        } else {
            for (var i = 0, j = recordMaxPage - bursterMaxPage; i < bursterMaxPage; i++, j++) {
                bursterPageNumbers[i] = j;
            }
        }
    }

    return bursterPageNumbers;

}

function convertObject(item, structs)
{
	var result = {};
	var fields = structs.split(",");
	for (var i = 0; i < item.length && i < fields.length; i++)
	{
		result[fields[i].trim()] = item[i];
	}
	return result;
}

//对象转换成对象
function listToObject(list,strKeyName) {
    var listStruct;
    var listData;
    var listRowData;

    // 对list进行空值判断
    if (typeof list == "undefined" || list == null ) {
        return null;
    }

    // 对需要结构化的Object Key进行适配
    if  (strKeyName == null) {
        strKeyName = "alllist";
    }

    // 对待适配的Key值进行空值判断
    if (typeof(list[strKeyName]) =="undefined" || list[strKeyName] == null) {
        return null;
    }

    // 对没有struct键的object直接返回，不做处理
    if  (typeof list.struct == "undefined" || list.struct == null ) {
        return list;
    }

    // 对下标长度进行适配
    listStruct=list.struct.split(",");
    for (var i = 0; i < listStruct.length; i++)
        listStruct[i] = listStruct[i].trim();

    if (list[strKeyName].length==0 || listStruct.length!=list[strKeyName][0].length) {
		if (list[strKeyName].length==0 || listStruct.length>=list[strKeyName][0].length) {
        list["error"]="错误:待处理的"+strKeyName+"的下标长度与struct的长度不匹配！";
        return list;
		} else {
			W("structs长度比row数据长度少");
    }
	}

    // 适配Struct
    listData=list[strKeyName];

    var arrayData = new Array();
    for (var i=0;i<listData.length;i++) {
        arrayData[i]={};
		for (var j=0;j<listStruct.length;j++) {
            arrayData[i][ltrim(listStruct[j])]=listData[i][j];
        }
    }

    for (var key  in list) {
        if (key===strKeyName) {
            list[key]=arrayData;
        }
    }
    return list;
};

var isEmpty=function(str) {
    if (typeof str == "undefined" || str.replace(/(^\s*)|(\s*$)/g,'')=='' || str==null ) {
        return true;
    } else {
        return false;
    }
}

var isNumber=function(str) {

    if (typeof str == "undefined" || str.replace(/(^\s*)|(\s*$)/g,'')=='' || str==null || isNaN(str)) {
        return false;
    } else {
        return true;
    }
}

var isFunc = function(test) {
	return typeof test == 'function';
};


var trim=function(str) {
    return str.replace(/(^\s*)|(\s*$)/g,'');
}


/**
 * 删除左边的空格
 */
var ltrim = function(str) {
    return str.replace(/(^\s*)/g,'');
}


/**
 * 删除右边的空格
 */
var rtrim = function(str) {
    return str.replace(/(\s*$)/g,'');
}


/**
 *浮点数 + - * /
 **/
var fnAdd=function add(a, b) {
    var c, d, e;
    try {
        c = a.toString().split(".")[1].length;
    } catch (f) {
        c = 0;
    }
    try {
        d = b.toString().split(".")[1].length;
    } catch (f) {
        d = 0;
    }
    return e = Math.pow(10, Math.max(c, d)), (fnMul(a, e) + fnMul(b, e)) / e;
}

var fnSub=function sub(a, b) {
    var c, d, e;
    try {
        c = a.toString().split(".")[1].length;
    } catch (f) {
        c = 0;
    }
    try {
        d = b.toString().split(".")[1].length;
    } catch (f) {
        d = 0;
    }
    return e = Math.pow(10, Math.max(c, d)), (fnMul(a, e) - fnMul(b, e)) / e;
}

var fnMul=function mul(a, b) {
    var c = 0,
        d = a.toString(),
        e = b.toString();
    try {
        c += d.split(".")[1].length;
    } catch (f) {}
    try {
        c += e.split(".")[1].length;
    } catch (f) {}
    return Number(d.replace(".", "")) * Number(e.replace(".", "")) / Math.pow(10, c);
}

var fnDiv=function div(a, b) {
    var c, d, e = 0,
        f = 0;
    try {
        e = a.toString().split(".")[1].length;
    } catch (g) {}
    try {
        f = b.toString().split(".")[1].length;
    } catch (g) {}
    return c = Number(a.toString().replace(".", "")), d = Number(b.toString().replace(".", "")), fnMul(c / d, Math.pow(10, f - e));
}

var calcAge=function(dateText)
{
    if (dateText==undefined|| dateText==null || dateText=='') {
        return ''
    }
    var birthday=new Date(dateText.replace(/-/g, "\/"));
    var d=new Date();
    var age = d.getFullYear()-birthday.getFullYear()-((d.getMonth()<birthday.getMonth()||d.getMonth()==birthday.getMonth() && d.getDate()<birthday.getDate())?1:0);
    var month = calculateMonth(dateText);
    var year = calculateAge(dateText);

    return(age);
    //document.all.item("ageTextField").value=age;
    if(year >= 0){
        if(month<0 && year==1){
            //document.all.item("ageTextField").value='0';
            return(0);
        }
        else{
            //document.all.item("ageTextField").value=year;
            return(year);
        }
    }
    else{
        //document.all.item("ageTextField").value="";
        return('');
    }
}

var calcIdChkcum = function(idno) {
	if (idno < 17) return false;

	var factor = [ 7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2 ];
	var parity = [ 1, 0, 'X', 9, 8, 7, 6, 5, 4, 3, 2 ];

	var sum = 0;
	var ai = 0;
	var wi = 0;

	for (var i = 0; i < 17; i++) {
		ai = idno[i];
		wi = factor[i];
		sum += ai * wi;
	}

	return parity[sum % 11];
}
var calcAgeByBdate = function (bornDate) {

	if (!bornDate || bornDate < 8) {
		return "N/A";
	}

	var y = parseInt(bornDate.substr(0, 4));
	var m = parseInt(bornDate.substr(4, 2));
	var d = parseInt(bornDate.substr(6, 2));

	var ny = (new Date()).getYear() + 1900;

	if (y < 1900 || y > ny) {
		return 0;
	}
	if (m < 1 || m > 12) {
		m = 1;
		//return 0;
	}
	if (d < 1 || d > 31) {
		d = 1;
		//return 0;
	}

	var date = y + '-' + m + '-' + d;
	return calcAge(date);
}

var calcAgeById = function (idNo) {
	if (!idNo || idNo.length < 14) {
		return 0;
	}

	var y = parseInt(idNo.substr(6, 4));
	var m = parseInt(idNo.substr(10, 2));
	var d = parseInt(idNo.substr(12, 2));

	var ny = (new Date()).getYear() + 1900;

	if (y < 1900 || y > ny) {
		return 0;
	}
	if (m < 1 || m > 12) {
		m = 1;
		//return 0;
	}
	if (d < 1 || d > 31) {
		d = 1;
		//return 0;
	}

	var date = y + '-' + m + '-' + d;
	return calcAge(date);
}

var calcBirthdayById = function (idNo) {
	if (!idNo || idNo.length < 14) {
		return 0;
	}

	var y = parseInt(idNo.substr(6, 4));
	var m = parseInt(idNo.substr(10, 2));
	var d = parseInt(idNo.substr(12, 2));

	var date = y + '-' + m + '-' + d;
	return date;
}

var calcSexById = function (idNo) {
	if (!idNo || idNo.length < 14) {
		return 0;
	}

	if (idNo.length <= 15) {
		return parseInt(idNo.substr(14, 1)) % 2;
	}

	if (idNo.length >= 17) {
		return parseInt(idNo.substr(16, 1)) % 2;
	}

	return null;
}

function calculateMonth(birthday)
{
    var month=-1;
    if(birthday)
    {
        var aDate=birthday.split("-");
        if(aDate[1].substr(0,1) == '0')
            aDate[1]=aDate[1].substring(1);
        var birthdayMonth = parseInt(aDate[1]);
        var currentDate = new Date();
        var currentMonth = parseInt(currentDate.getMonth()+1);
        month = currentMonth-birthdayMonth;
        return month;
    }
    return month;
}

function calculateAge(birthday){
    if(birthday){
        var aDate=birthday.split("-");
        var birthdayYear = parseInt(aDate[0]);
        var currentDate = new Date();
        var currentYear = parseInt(currentDate.getFullYear());
        return currentYear-birthdayYear;
	}
	return 0;
}

function getDate(fullTime){
	if (!fullTime) {
		return "";
	}
	var splits = trim(fullTime).split(" ");
	return splits[0]
}

var Utf8ToGb2312=function(str1){
    var substr = "";
    var a = "";
    var b = "";
    var c = "";
    var i = -1;
    i = str1.indexOf("%");
    if(i==-1){
        return str1;
    }
    while(i!= -1){
        if(i<3){
            substr = substr + str1.substr(0,i-1);
            str1 = str1.substr(i+1,str1.length-i);
            a = str1.substr(0,2);
            str1 = str1.substr(2,str1.length - 2);
            if(parseInt("0x" + a) & 0x80 == 0){
                substr = substr + String.fromCharCode(parseInt("0x" + a));
            }
            else if(parseInt("0x" + a) & 0xE0 == 0xC0){ //two byte
                b = str1.substr(1,2);
                str1 = str1.substr(3,str1.length - 3);
                var widechar = (parseInt("0x" + a) & 0x1F) << 6;
                widechar = widechar | (parseInt("0x" + b) & 0x3F);
                substr = substr + String.fromCharCode(widechar);
            }
            else{
                b = str1.substr(1,2);
                str1 = str1.substr(3,str1.length - 3);
                c = str1.substr(1,2);
                str1 = str1.substr(3,str1.length - 3);
                var widechar = (parseInt("0x" + a) & 0x0F) << 12;
                widechar = widechar | ((parseInt("0x" + b) & 0x3F) << 6);
                widechar = widechar | (parseInt("0x" + c) & 0x3F);
                substr = substr + String.fromCharCode(widechar);
            }
        }
        else {
            substr = substr + str1.substring(0,i);
            str1= str1.substring(i);
        }
        i = str1.indexOf("%");
    }

    return substr+str1;
}


var Utf8ToGBK = function(strUtf8) {
    var bstr = "";
    var nTotalChars = strUtf8.length;        // total chars to be processed.
    var nOffset = 0;                         // processing point on strUtf8
    var nRemainingBytes = nTotalChars;       // how many bytes left to be converted
    var nOutputPosition = 0;
    var iCode, iCode1, iCode2;               // the value of the unicode.

    while (nOffset < nTotalChars)
    {
        iCode = strUtf8.charCodeAt(nOffset);
        if ((iCode & 0x80) == 0)             // 1 byte.
        {
            if ( nRemainingBytes < 1 )       // not enough data
                break;

            bstr += String.fromCharCode(iCode & 0x7F);
            nOffset ++;
            nRemainingBytes -= 1;
        }
        else if ((iCode & 0xE0) == 0xC0)        // 2 bytes
        {
            iCode1 =  strUtf8.charCodeAt(nOffset + 1);
            if ( nRemainingBytes < 2 ||                        // not enough data
                (iCode1 & 0xC0) != 0x80 )                // invalid pattern
            {
                break;
            }

            bstr += String.fromCharCode(((iCode & 0x3F) << 6) | (         iCode1 & 0x3F));
            nOffset += 2;
            nRemainingBytes -= 2;
        }
        else if ((iCode & 0xF0) == 0xE0)        // 3 bytes
        {
            iCode1 =  strUtf8.charCodeAt(nOffset + 1);
            iCode2 =  strUtf8.charCodeAt(nOffset + 2);
            if ( nRemainingBytes < 3 ||                        // not enough data
                (iCode1 & 0xC0) != 0x80 ||                // invalid pattern
                (iCode2 & 0xC0) != 0x80 )
            {
                break;
            }

            bstr += String.fromCharCode(((iCode & 0x0F) << 12) |
                ((iCode1 & 0x3F) <<  6) |
                (iCode2 & 0x3F));
            nOffset += 3;
            nRemainingBytes -= 3;
        }
        else                                                                // 4 or more bytes -- unsupported
            break;
    }

    if (nRemainingBytes != 0) {
        // bad UTF8 string.
        return strUtf8;
    }

    return bstr;
}

var getExplorer=function() {
    var explorer = window.navigator.userAgent ;
    //ie
    if (explorer.indexOf("MSIE") >= 0) {
        return 'ie';
    }
    //firefox
    else if (explorer.indexOf("Firefox") >= 0) {
        return 'Firefox';
    }
    //Chrome
    else if(explorer.indexOf("Chrome") >= 0){
        return 'Chrome';
    }
    //Opera
    else if(explorer.indexOf("Opera") >= 0){
        return 'Opera';
    }
    //Safari
    else if(explorer.indexOf("Safari") >= 0){
        return 'Safari';
    }
};

var getIEVer = function() {
};

var gotoUrl = function(url, delay) {
	if (delay) {
		setTimeout(function() {
			window.location.href = url;
		}, delay);
	} else {
		window.location.href = url;
	}
};

var gotoLogin = function() {
	window.location.href = "/login.html";
};

var goBack = function(){
	window.history.back();
};

function Pager(pagerId, pageName) {
	if (!(this instanceof Pager)){
		return new Pager(pagerId, pageName);
	}

	this.pageIndex = 1;
	this.pageSize = 10;
	this.totalPage = 0;
	this.pageNumbers = [];
    this.pagerId = pagerId;
    this.pageName = pageName;

	if (pageName) {
    	var page = appCache.getItem("pager_index_of_" + this.pageName);
    	if (page) {
    		this.pageIndex = parseInt(page);
    	}
    }
};

Pager.prototype.getPageSize = function() {
	return this.pageSize;
};

Pager.prototype.getPage = function() {
	return this.pageIndex;
};

Pager.prototype.setPage = function(page) {
	if (this.pageName) {
    	appCache.setItem("pager_index_of_" + this.pageName, "" + page);
    }
	return this.pageIndex = page;
};

//return uri params
Pager.prototype.getUri = function () {
	return "&o=" + this.pageIndex + "&r=" + this.pageSize;
};

//return the pager index
Pager.prototype.onEvent = function(e) {
	if (e == undefined) {
		return 0;
	}

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

	this.setPage(pageIndex);
	return pageIndex;
};

// count: count of items
Pager.prototype.update = function (count) {
	var pageIndex = this.pageIndex;
	var totalPage = this.totalPage = Math.ceil(count / this.pageSize);

	var pgId;
	if (this.pagerId) {
		pgId = "#" + this.pagerId + " li ";
	} else {
		pgId = "";
	}

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

    var delayFunc = function () {
        //改变页码样式
        angular.element(pgId + ".active").remove("active").css("background","");
        angular.element(pgId + ".active").remove("active").css("color","#333");
        angular.element(pgId + "#" + pageIndex).addClass("active").css("background","#4AB033");
        angular.element(pgId + "#" + pageIndex).addClass("active").css("color","#fff");
    };

    setTimeout(delayFunc, 0);

    return this;
};

function D(s) 
{
    return;
	if (typeof s == "string") {
		console.info("[DEBUG] : " + s);
	} else {
		console.info("[DEBUG] ==> ");
		console.info(s);
	}
}

function W(s) 
{
	if (typeof s == "string") {
		console.warn("[WARN] : " + s);
	} else {
		console.warn("[WARN] ==> ");
		console.warn(s);
	}
}

function E(s) 
{
	if (typeof s == "string") {
		console.error("[ERROR] : " + s);
	} else {
		console.error("[ERROR] ==> ");
		console.error(s);
	}
}

var uJsonParser = (function () {
	var JSON_PROTECTION_PREFIX = /^\)\]\}',?\n/;		//';
	return function (str) {
		if (!str) {
			return null;
		}
		var tmp = str.replace(JSON_PROTECTION_PREFIX, '').trim();
		return JSON.parse(tmp)
	};
})();
var P = uJsonParser;
var S = JSON.stringify;

function nullFn() {}

function Listeners(listeners, scope) {
	this.scope = scope;
	this.listeners = listeners;
};

Listeners.prototype.setListeners = function (listeners) {
	this.listeners = listeners;
};

Listeners.prototype.getListeners = function () {
	return this.listeners;
};

Listeners.prototype.setScope = function (scope) {
	this.scope = scope;
};

Listeners.prototype.getScope = function () {
	return this.scope;
};

Listeners.prototype.invoke = function (lname){
	if (!this.listeners) {
		return;
	}
	var fn = this.listeners[lname];
	if (!fn) {
		return;
	}
	var args = [];
	for (var i = 1; i < arguments.length; i++) {
		args[i - 1] = arguments[i];
	}
	if (this.scope) {
		fn.apply(this.scope, args);
	} else {
		fn.apply(window, args);
	}
};

var TO = setTimeout;

/* check if browser is fullscreen */
function isFullScreen() {
    var isNotFullScreen = !document.fullscreenElement && // alternative standard method
    !document.mozFullScreenElement && // current working methods
    !document.webkitFullscreenElement && !document.msFullscreenElement;
    return !isNotFullScreen;
}

/* toggle browser fullscreen */
function toggleFullScreen() {
    var isNotFullScreen = !document.fullscreenElement && // alternative standard method
    !document.mozFullScreenElement && // current working methods
    !document.webkitFullscreenElement && !document.msFullscreenElement;

    if (isNotFullScreen) {
        if (document.documentElement.requestFullscreen) {
            document.documentElement.requestFullscreen();
        } else if (document.documentElement.msRequestFullscreen) {
            document.documentElement.msRequestFullscreen();
        } else if (document.documentElement.mozRequestFullScreen) {
            document.documentElement.mozRequestFullScreen();
        } else if (document.documentElement.webkitRequestFullscreen) {
            document.documentElement.webkitRequestFullscreen(Element.ALLOW_KEYBOARD_INPUT);
        }
    } else {
        if (document.exitFullscreen) {
            document.exitFullscreen();
        } else if (document.msExitFullscreen) {
            document.msExitFullscreen();
        } else if (document.mozCancelFullScreen) {
            document.mozCancelFullScreen();
        } else if (document.webkitExitFullscreen) {
            document.webkitExitFullscreen();
        }
    }
}

// recsN : recursive depth
// e.g. copyObject(usrObj, null, 2)
function copyObject(src, dst, recsN) {
	var result = dst || {};
	recsN = int(recsN) || 0;
	for (var key in src) {
		var value = src[key];
		result[key] = (typeof value === "object" && recsN > 0) ? copyObject(value, recsN - 1) : value;
	}
	return result;
}

// e.g.
// alignFloat(1.2367, 100)  -> 1.23
// alignFloat(1.2367, 1000) -> 1.237
function alignFloat(v, scale) {
	return Math.round(v * scale) / scale;
}

Array.indexOf = function(self, val) {
    for (var i = 0; i < self.length; i++) {
        if (self[i] == val)
            return i;
    }
    return -1;
};

Array.remove = function(self, val) {
	var r = new Array();
    for (var i = 0; i < self.length; i++) {
        if (self[i] == val)
            continue;
		r.push(self[i]);
    }
    return r;
};

Array.deleteAt = function(self, index) {
	return self.splice(index, 1);
};

Array.find = function (self, obj) {
	if (!self) {
		return -1;
	}
	for (var i = 0; i < self.length; i++) {
		if (self[i] == obj) {
			return i;
		}
	}

	return -1;
}

//获取url参数，参数必须放在#之前例如http://localhost:84/index.html?id=1#/mdtList
function getQueryString(name) {
    var reg = new RegExp("(^|&)" + name + "=([^&]*)(&|$)", "i");
    var r = window.location.search.substr(1).match(reg);
    if (r != null) return unescape(r[2]); return null;
}

//滚动加载器，参数为panelbody的id
function Loader(panelbody,msg){
    if (!(this instanceof Loader)){
	return new Loader(panelbody, msg);
    }
    var panelBody=$("#"+panelbody);
    var panel=panelBody.parent('.panel');
    var width=panel.width();
    var height=panel.height()-36;
    var html1='<div class="load-back"></div>';
    this.loadBack=$(html1).css("width",width).css("height",height);
    //this.loadBack.appendTo(panel);
    panel.append(this.loadBack);
    if(!msg){
	msg="处理中..."
    }
    var html2='<div class="load"><img src="/resource/images/loading.gif" style="width:50px;height:50px"/><span>'+msg+'</span></div>';
    this.load=$(html2).css("width",width).css("height",height);
    this.load.appendTo(panel);
}
Loader.prototype.close=function(){
    if(this.loadBack){
	this.loadBack.remove();
	this.loadBack=null;
    }
    if( this.load){
	this.load.remove();
	this.load=null;
    }
}
function Loader2(id,msg){
    if (!(this instanceof Loader2)){
	return new Loader2(id, msg);
    }
    if(!msg){
	msg="处理中...";
    }
    var html='<div class="modal fade" id="'+id+'" tabindex="-1" role="dialog" aria-labelledby="resizedModalTitle" aria-hidden="true" >';
    html+='<div class="modal-dialog" style="width:30%">';
    html+='<div class="modal-content">';
    html+='<div class="modal-body">'
    html+='<div>'
    html+='<div style="text-align: center;display: flex;align-items: center;" >'
    html+='<img src="/resource/images/loading.gif" style="width:50px;height:50px"/>'
    html+='<span>'+msg+'</span></div></div></div></div></div></div>'
    this.html1=$(html);
    $("body").append(this.html1)
    $('#'+id).modal({backdrop: 'static', keyboard: false});
    $("#"+id).modal("show");
    this.id=id
}
Loader2.prototype.close=function(){
    $("#"+this.id).modal("hide");
    setTimeout(function(){
	if(this.html1){
	    this.html1.remove();
	    this.html1=null;
	}
    },100);
}

function selectPatients($scope, CommonService,type,callback){
    if (!(this instanceof selectPatients)){
	return new selectPatients($scope,CommonService,type,callback);
    }
    this.$scope=$scope;
    this.CommonService=CommonService;
    this.type=type;//区别是查个人还是全院
    $scope.patpager = new Pager("pat", "my_patient_list");
    $scope.patpager.pageIndex=1;
    $scope.patpager.pageSize=12;
    $scope.getMyPatients=function(){
	var uriData={o:$scope.patpager.pageIndex,r:$scope.patpager.pageSize,name:$scope.searchPatientName,type:type};
	if($scope.IDcard){
            uriData.IDcard=$scope.IDcard;
        }
    var uriData=JSON.stringify(uriData);
	//获取我的患者/全院
	CommonService.createOne('myPatient', uriData, function (data) {
	    $scope.patients=listToObject(data,'rows').rows;
         $scope.patpager.update(data.count);
         $scope.totalPagePat=Math.ceil(parseInt(data.count) / $scope.patpager.pageSize);
	}, function (data, status) {//失败时的回调函数
	    if(data){
		$scope.msg=data.message;
	    }else{
		$scope.msg='获取患者失败';
	    }
	    $("#msg").modal("show");
	})
    }
    $scope.patpage = function(e){
        $scope.patpager.onEvent(e);
        $scope.getMyPatients();
    }
    $scope.onSelectPatient=function(item){
        //执行选中回调处理
	callback(item);
    }
    $scope.toPatientDetail=function(item){
	var data=item.id_no;
	var pathospital_code=item.hospital_code;
	/*var url = "/views/tms/patdetail.html?id_no="+data+"&hospital_code="+pathospital_code;
	window.open("url");*/
    window.open("#/patientDetailsXin");
    /*window.location.href='#/patientDetails';*/
    }
    $scope.getMyPatients();
    $("#selectPatient").modal("show");
}



//门诊新封装
function selectDoctorsInTm($scope,CommonService,code,name,callback,demo,chodept){

    if (!(this instanceof selectDoctorsInTm)){
    return new selectDoctorsInTm($scope,CommonService,code,callback);
    }
    this.$scope=$scope;
    this.CommonService=CommonService;
    this.code=code;
    if(demo)  this.demo=demo;

    $scope.selHosName="";
    $scope.Schedulingdatahospital=[];

    $scope.getDeptByHos=function(hos){
    var data={hospital_code:hos}
    var uriData=JSON.stringify(data);
    var chooseAll={};
    chooseAll.name="选择全部";
    chooseAll.id=-100;
    CommonService.updateOne('hospitalManage',uriData, function (data) {
        $scope.selDeptList=listToObject(data,'rows').rows;
        $scope.selDeptList.unshift(chooseAll);
    }, function (data, status) {//失败时的回调函数
        if(data){
        $scope.msg=data.message;
        }else{
        $scope.msg='获取医院列表失败！';
        }
        $("#msg").modal("show");
    })
    }


    $scope.changeSelectedHos=function(){
    $scope.selectedHos=$scope.selHosList.selected.code;
    if($scope.selectedHos){
        $scope.selectedDept=null;
        $scope.getDeptByHos($scope.selectedHos)
    }
    }

    $scope.getdoctorbyselecteddept= function(){

        if($scope.selectedDept)
        {
            $scope.getDoctorByDept($scope.selectedDept);
        }

    }

    if(code){
    $scope.selHosName=name;
    $scope.noselhos=true;
    //获取指定医院所有科室
    $scope.getDeptByHos(code);
    }else{
    $scope.noselhos=false;
    $scope.selectedHos=$scope.user.hospital_code;
    //获取所有医院，本院排第一
    CommonService.updatePartOne('hospitalManage',"", function (data) {
        $scope.selHosList=listToObject(data,'rows').rows;
        $scope.selHosList.selected=$scope.selHosList[0];
    }, function (data, status) {//失败时的回调函数
        if(data){
        $scope.msg=data.message;
        }else{
        $scope.msg='获取医院科室列表失败！';
        }
        $("#msg").modal("show");
    })
    //获取本院所有科室
    $scope.getDeptByHos($scope.user.hospital_code);
    }



    $scope.selDoctorList=[];
    $scope.docpager = new Pager("docpager", "selDoctorList");
    $scope.docpager.pageIndex=1;
    $scope.docpager.pageSize=8;

    /*$scope.docpager = new Pager(null, "hos_doctor_list");
    $scope.docpager.pageIndex=1;
    $scope.docpager.pageSize=12;*/


    $scope.changeSelectedDept=function(){

    $scope.docpager.pageIndex=1;

    if($scope.selDeptList.selected.name=="选择全部"){
        appCache.setItem("needFresh",0);
        $scope.init();
        $scope.selectedDept=$scope.selDeptList.selected.id;
        return;
    }

    $scope.selectedDept=$scope.selDeptList.selected.id;
    //alert();
    if($scope.selectedDept){
        //$scope.init();
        //$scope.loadData();
        $scope.getDoctorByDept($scope.selectedDept);

    }
    }



    $scope.toDoctorDetail=function(item){
    var did=item.id;
    var url = "/public/views/checkDoctor.html?id="+did;
    window.open(url);
    }

    $scope.checkarrange=function(item){

        var urlData="doc_id="+item.id;
        $scope.Schedulingdata=[];
        $scope.Schedulingdatahospital=[];

        //获取排班表
        CommonService.getAll("doctorManage/doctorInfoList", urlData, function(data) {
                $scope.Schedulingdata = data.rows[0];

                for (var i = 0; i < 14; i++) {
                       if ($scope.Schedulingdata[i] ==1) {
                           if(i==0){
                               $($(".aa")[0]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[0]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==1){
                               $($(".aa")[7]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[7]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==2){
                               $($(".aa")[1]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[1]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==3){
                               $($(".aa")[8]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[8]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==4){
                               $($(".aa")[2]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[2]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==5){
                               $($(".aa")[9]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[9]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==6){
                               $($(".aa")[3]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[3]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==7){
                               $($(".aa")[10]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[10]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==8){
                               $($(".aa")[4]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[4]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==9){
                               $($(".aa")[11]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[11]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==10){
                               $($(".aa")[5]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[5]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==11){
                               $($(".aa")[12]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[12]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==12){
                               $($(".aa")[6]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[6]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==13){
                               $($(".aa")[13]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[13]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }
                       }else {
                           if(i==0){
                               $($(".aa")[0]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[0]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==1){
                               $($(".aa")[7]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[7]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==2){
                               $($(".aa")[1]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[1]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==3){
                               $($(".aa")[8]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[8]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==4){
                               $($(".aa")[2]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[2]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==5){
                               $($(".aa")[9]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[9]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==6){
                               $($(".aa")[3]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[3]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==7){
                               $($(".aa")[10]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[10]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==8){
                               $($(".aa")[4]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[4]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==9){
                               $($(".aa")[11]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[11]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==10){
                               $($(".aa")[5]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[5]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==11){
                               $($(".aa")[12]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[12]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==12){
                               $($(".aa")[6]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[6]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==13){
                               $($(".aa")[13]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[13]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }
                       }
                }
        });

    }
    $scope.selSearchDoctorName="";
    $scope.selDoctorList=[];

    $scope.initSelected=function(){
    for(var i=0;i<$scope.selDoctorList.length;i++){
        $scope.selDoctorList[i].selected=false;
    }
    for(var i=0;i<$scope.selDoctorList.length;i++){
        for(var j=0;j<$scope.selectedDoctors.length;j++){
        if($scope.selDoctorList[i].id==$scope.selectedDoctors[j].id){
            $scope.selDoctorList[i].selected=true;
        }
        }
    }
    }

    $scope.getDoctorByDept = function(dept_id){
        appCache.setItem("dept_id",dept_id);

        if(dept_id==-100){
            return;
        }
    if(demo) {
        var data={pageIndex:$scope.docpager.pageIndex,pageSize:$scope.docpager.pageSize,dept_id:dept_id,action:0,account:demo,service_type:6}
        var uriData=JSON.stringify(data);
        appCache.setItem("needFresh",1);
    }else{
        var data={pageIndex:$scope.docpager.pageIndex,pageSize:$scope.docpager.pageSize,dept_id:dept_id,action:0,service_type:6}
        var uriData=JSON.stringify(data);
        appCache.setItem("needFresh",1);
    }

    CommonService.createOne('hospitalManage',uriData,function(data){
        $scope.selDoctorList = listToObject(data,'rows').rows;
          $scope.docpager.update(data.count);
                $scope.count=data.count;
                appCache.setObject("selDoctorList",$scope.selDoctorList);
                appCache.setItem("count",$scope.count);

        $scope.loadData();
        $scope.initSelected();
        },function(data,status){
            if(data){
              $scope.msg=data.message;
            }else{
              $scope.msg='获取医生出错';
            }
            $("#msg").modal("show");
        });
    }

     if(chodept) {
         this.chodept=chodept;
         $scope.selectedDept = chodept;
         $scope.getDoctorByDept(chodept);
    }



    $scope.init=function(){
        appCache.setItem("needFresh",0);
        if(demo)
        {
            var data={code:code,action:1,account:demo}
        }
        else{
                var data={code:code,action:1}
            }

        var uriData=JSON.stringify(data);
        CommonService.createOne('hospitalManage',uriData,function(data){
        $scope.selDoctorList = listToObject(data,'rows').rows;
        $scope.loadData();
        //$scope.initSelected();
        },function(data,status){
        if(data){
          $scope.msg=data.message;
        }else{
          $scope.msg='获取医生出错';
        }
        $("#msg").modal("show");
        });

    }




     $scope.loadData = function (){
        //needfresh = 1 为切换科室时刷新分页
       var needFresh = appCache.getItem("needFresh");
        if(needFresh =="0"){
           var data={pageIndex:$scope.docpager.pageIndex,pageSize:$scope.docpager.pageSize,code: code, action: 1,service_type:6};
           var uriData=JSON.stringify(data);
            CommonService.createOne('hospitalManage',uriData,function(data){
                $scope.selDoctorList=listToObject(data,'rows').rows;
                $scope.docpager.update(data.count);
                $scope.count=data.count;
            },function(data,status){//失败时的回调函数
            })
        }else if(needFresh =="1"){
             var dept_id = appCache.getItem("dept_id");
             if(demo){
                var data={pageIndex:$scope.docpager.pageIndex,pageSize:$scope.docpager.pageSize,dept_id:dept_id,action:0,account:demo,service_type:6}
             }else{
                var data={pageIndex:$scope.docpager.pageIndex,pageSize:$scope.docpager.pageSize,dept_id:dept_id,action:0,service_type:6}
             }
             var uriData=JSON.stringify(data);
              CommonService.createOne('hospitalManage',uriData,function(data){
                $scope.selDoctorList=listToObject(data,'rows').rows;
                $scope.docpager.update(data.count);
                $scope.count=data.count;
            },function(data,status){//失败时的回调函数
            })
               //$scope.selDoctorList = appCache.getObject("selDoctorList");
                /*var count = appCache.getItem("count");
                $scope.docpager.update(count);
                $scope.count=count; */
        }else if(needFresh =="2"){
            if(demo){
                var data={pageIndex:$scope.docpager.pageIndex,pageSize:$scope.docpager.pageSize,name:$scope.selSearchDoctorName,code:code,action:1,account:demo,service_type:1}
            }else{
                var data={pageIndex:$scope.docpager.pageIndex,pageSize:$scope.docpager.pageSize,name:$scope.selSearchDoctorName,code:code,action:1,service_type:6}
            }

            var uriData=JSON.stringify(data);
              CommonService.createOne('hospitalManage',uriData,function(data){
                $scope.selDoctorList=listToObject(data,'rows').rows;
                $scope.docpager.update(data.count);
                $scope.count=data.count;
            },function(data,status){//失败时的回调函数
            })

        }
     }

       $scope.loadData();

    $scope.clickPager = function(e){
        $scope.docpager.onEvent(e);
        $scope.loadData();
    };



   $scope.selSearchDoctorByName=function(){
    if($scope.selSearchDoctorName){
        var tcode=""
        if(code){
        tcode=code;
        }else{
        if($scope.selectedHos){
            tcode=$scope.selectedHos;
        }else{
            $scope.msg='请先选择医院';
            $("#msg").modal("show");
            return;
        }
        }
        $scope.selectedDept=null;
        if(demo)
        {
            var data={name:$scope.selSearchDoctorName,code:tcode,action:1,account:demo,service_type:6}
            appCache.setItem("needFresh",2);
        }
        else{
                var data={name:$scope.selSearchDoctorName,code:tcode,action:1,service_type:6}
                appCache.setItem("needFresh",2);
            }

        var uriData=JSON.stringify(data);
        CommonService.createOne('hospitalManage',uriData,function(data){
        $scope.selDoctorList = listToObject(data,'rows').rows;
        $scope.loadData();
        $scope.initSelected();
        },function(data,status){
        if(data){
          $scope.msg=data.message;
        }else{
          $scope.msg='获取医生出错';
        }
        $("#msg").modal("show");
        });
    }else{
        $scope.msg='请输入医生姓名';
        $("#msg").modal("show");
        return;
    }
    }

    $scope.onSelectDoctor=function(item){
        //执行选中回调处理
    callback(item);
    }
    $("#selectDoctor").modal("show");
}

selectDoctorsInTm.prototype.changedept=function($scope,chodept){

  this.$scope=$scope;
  if(chodept){
        $scope.selectedDept = chodept;
    }
}
selectDoctorsInTm.prototype.getdoctorbyselecteddept=function($scope){
    this.$scope=$scope;
  if($scope.selectedDept)
        {

            $scope.getDoctorByDept($scope.selectedDept);
        }
}





//demo：表示是否来自于新增账户页面，若来自该页面，则选出的医生是不带账户信息的
//chodept:如果在选择医生之前想要选定一个科室，可传入科室id

function selectDoctors($scope,CommonService,code,name,callback,demo,chodept){

    if (!(this instanceof selectDoctors)){
	return new selectDoctors($scope,CommonService,code,callback);
    }
    this.$scope=$scope;
    this.CommonService=CommonService;
    this.code=code;
    if(demo)  this.demo=demo;

    $scope.selHosName="";
    $scope.Schedulingdatahospital=[];


    $scope.oriDocpager = new Pager("oriDocpager");
    $scope.oriDocpager.pageIndex=1;
    $scope.oriDocpager.pageSize=8;

    var flag=0;
    $scope.getDeptByHos=function(hos){
	var data={hospital_code:hos}
	var uriData=JSON.stringify(data);
    var chooseAll={};
    chooseAll.name="选择全部";
    chooseAll.id=-100;
	CommonService.updateOne('hospitalManage',uriData, function (data) {
	    $scope.selDeptList=listToObject(data,'rows').rows;
        $scope.selDeptList.unshift(chooseAll);
	}, function (data, status) {//失败时的回调函数
	    if(data){
		$scope.msg=data.message;
	    }else{
		$scope.msg='获取医院列表失败！';
	    }
	    $("#msg").modal("show");
	})
    }

    $scope.changeSelectedHos=function(){
	$scope.selectedHos=$scope.selHosList.selected.code;
    code=$scope.selectedHos;
	if($scope.selectedHos){
	    $scope.selectedDept=null;
	    $scope.getDeptByHos($scope.selectedHos)
	}
    }

    $scope.getdoctorbyselecteddept= function(){

        if($scope.selectedDept)
        {
            $scope.getDoctorByDept($scope.selectedDept);
        }

    }

    if(code){
	$scope.selHosName=name;
	$scope.noselhos=true;
	//获取指定医院所有科室
	$scope.getDeptByHos(code);
    }else{
	$scope.noselhos=false;
	$scope.selectedHos=$scope.user.hospital_code;
	//获取所有医院，本院排第一
	CommonService.updatePartOne('hospitalManage',"", function (data) {
	    $scope.selHosList=listToObject(data,'rows').rows;
	    $scope.selHosList.selected=$scope.selHosList[0];
        code=$scope.selHosList.selected.code;
	}, function (data, status) {//失败时的回调函数
	    if(data){
		$scope.msg=data.message;
	    }else{
		$scope.msg='获取医院科室列表失败！';
	    }
	    $("#msg").modal("show");
	})
	//获取本院所有科室
	$scope.getDeptByHos($scope.user.hospital_code);
    }


    $scope.changeSelectedDept=function(){
        $scope.oriDocpager.pageIndex=1;
        if($scope.selDeptList.selected.name=="选择全部"){
            $scope.init();
            $scope.selectedDept=$scope.selDeptList.selected.id;
            return;
        }
    	$scope.selectedDept=$scope.selDeptList.selected.id;
        $scope.getDoctorByDept($scope.selectedDept);

    }
    $scope.toDoctorDetail=function(item){
	var did=item.id;
	var url = "/public/views/checkDoctor.html?id="+did;
	window.open(url);
    }
    $scope.checkarrange=function(item){

        var urlData="doc_id="+item.id;
        $scope.Schedulingdata=[];
        $scope.Schedulingdatahospital=[];

        //获取排班表
        CommonService.getAll("doctorManage/doctorInfoList", urlData, function(data) {
                $scope.Schedulingdata = data.rows[0];

                for (var i = 0; i < 14; i++) {
                       if ($scope.Schedulingdata[i] ==1) {
                           if(i==0){
                               $($(".aa")[0]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[0]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==1){
                               $($(".aa")[7]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[7]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==2){
                               $($(".aa")[1]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[1]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==3){
                               $($(".aa")[8]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[8]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==4){
                               $($(".aa")[2]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[2]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==5){
                               $($(".aa")[9]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[9]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==6){
                               $($(".aa")[3]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[3]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==7){
                               $($(".aa")[10]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[10]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==8){
                               $($(".aa")[4]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[4]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==9){
                               $($(".aa")[11]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[11]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==10){
                               $($(".aa")[5]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[5]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==11){
                               $($(".aa")[12]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[12]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==12){
                               $($(".aa")[6]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[6]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }else if(i==13){
                               $($(".aa")[13]).css("color","red");
                               $scope.Schedulingdata[i] = "值班";
                               $($(".bb")[13]).css("color","red");
                               $scope.Schedulingdatahospital[i] = "院内";
                           }
                       }else {
                           if(i==0){
                               $($(".aa")[0]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[0]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==1){
                               $($(".aa")[7]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[7]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==2){
                               $($(".aa")[1]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[1]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==3){
                               $($(".aa")[8]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[8]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==4){
                               $($(".aa")[2]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[2]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==5){
                               $($(".aa")[9]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[9]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==6){
                               $($(".aa")[3]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[3]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==7){
                               $($(".aa")[10]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[10]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==8){
                               $($(".aa")[4]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[4]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==9){
                               $($(".aa")[11]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[11]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==10){
                               $($(".aa")[5]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[5]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==11){
                               $($(".aa")[12]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[12]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==12){
                               $($(".aa")[6]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[6]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }else if(i==13){
                               $($(".aa")[13]).css("color","green");
                               $scope.Schedulingdata[i] = "休息";
                               $($(".bb")[13]).css("color","green");
                               $scope.Schedulingdatahospital[i] = "休息";
                           }
                       }
                }
        });

    }
    $scope.selSearchDoctorName="";
    $scope.selDoctorList=[];

    $scope.initSelected=function(){
	for(var i=0;i<$scope.selDoctorList.length;i++){
	    $scope.selDoctorList[i].selected=false;
	}
	for(var i=0;i<$scope.selDoctorList.length;i++){
	    for(var j=0;j<$scope.selectedDoctors.length;j++){
		if($scope.selDoctorList[i].id==$scope.selectedDoctors[j].id){
		    $scope.selDoctorList[i].selected=true;
		}
	    }
	}
    }

    $scope.getDoctorByDept = function(dept_id){
        flag=1;
        if(dept_id==-100){
            return;
        }
    if(demo) {
        var data={o:$scope.oriDocpager.pageIndex,r:$scope.oriDocpager.pageSize,dept_id:dept_id,action:0,account:demo}
        var uriData=JSON.stringify(data);
    }else{
        var data={o:$scope.oriDocpager.pageIndex,r:$scope.oriDocpager.pageSize,dept_id:dept_id,action:0}
        var uriData=JSON.stringify(data);
    }

        CommonService.deleteOne('hospitalManage',uriData,function(data){
            $scope.selDoctorList = listToObject(data,'rows').rows;
	        $scope.initSelected();
            $scope.count=data.count;
            $scope.oriDocpager.update(data.count);

        },function(data,status){
            if(data){
              $scope.msg=data.message;
            }else{
              $scope.msg='获取医生出错';
            }
            $("#msg").modal("show");
        });
    }

     if(chodept) {
         this.chodept=chodept;
         $scope.selectedDept = chodept;
         $scope.getDoctorByDept(chodept);
    }

    $scope.init=function(){
        flag=2;
        if(demo)
        {
            var data={o:$scope.oriDocpager.pageIndex,r:$scope.oriDocpager.pageSize,code:code,action:1,account:demo}
        }
        else{
                var data={o:$scope.oriDocpager.pageIndex,r:$scope.oriDocpager.pageSize,code:code,action:1}
            }

        var uriData=JSON.stringify(data);
        CommonService.deleteOne('hospitalManage',uriData,function(data){
            $scope.selDoctorList = listToObject(data,'rows').rows;
            $scope.initSelected();
            $scope.count=data.count;
            $scope.oriDocpager.update(data.count);
        },function(data,status){
        if(data){
          $scope.msg=data.message;
        }else{
          $scope.msg='获取医生出错';
        }
        $("#msg").modal("show");
        });

    }
    $scope.init();
   $scope.selSearchDoctorByName=function(){
    flag=3;
	if($scope.selSearchDoctorName){
	    var tcode=""
	    if(code){
		tcode=code;
	    }else{
		if($scope.selectedHos){
		    tcode=$scope.selectedHos;
		}else{
		    $scope.msg='请先选择医院';
		    $("#msg").modal("show");
		    return;
		}
	    }
	    $scope.selectedDept=null;
        if(demo)
        {
            var data={o:$scope.oriDocpager.pageIndex,r:$scope.oriDocpager.pageSize,name:$scope.selSearchDoctorName,code:tcode,action:1,account:demo}
        }
        else{
                var data={o:$scope.oriDocpager.pageIndex,r:$scope.oriDocpager.pageSize,name:$scope.selSearchDoctorName,code:tcode,action:1}
            }

	    var uriData=JSON.stringify(data);
	    CommonService.deleteOne('hospitalManage',uriData,function(data){
    		$scope.selDoctorList = listToObject(data,'rows').rows;
    		$scope.initSelected();
            $scope.count=data.count;
            $scope.oriDocpager.update(data.count);
	    },function(data,status){
		if(data){
		  $scope.msg=data.message;
		}else{
		  $scope.msg='获取医生出错';
		}
		$("#msg").modal("show");
	    });
	}else{
	    $scope.msg='请输入医生姓名';
	    $("#msg").modal("show");
	    return;
	}
    }

    $scope.clickSelDocPager = function(e){
        $scope.oriDocpager.onEvent(e);
        if(flag==1) $scope.getDoctorByDept();
        if(flag==2) $scope.init();
        if(flag==3) $scope.selSearchDoctorByName();
    };

    $scope.onSelectDoctor=function(item){
        //执行选中回调处理
	callback(item);
    }
    $("#selectDoctor").modal("show");
}

selectDoctors.prototype.changedept=function($scope,chodept){

  this.$scope=$scope;
  if(chodept){
        $scope.selectedDept = chodept;
    }
}
selectDoctors.prototype.getdoctorbyselecteddept=function($scope){
    this.$scope=$scope;
  if($scope.selectedDept)
        {

            $scope.getDoctorByDept($scope.selectedDept);
        }
}

// 获取时间字符串

// 补0
function getTimeUnit(unit)
{
    var str = "";
    if ( unit < 10 )
        str = "0";

    str += unit;

    return str;
}

// YYYY-MM-DD
function getNowFormatDate1(seperator1) 
{
    var date = new Date();
    if ( seperator1 == undefined )
        seperator1 = "-";

    var month = getTimeUnit( date.getMonth() + 1 );
    var strDate = getTimeUnit( date.getDate() );

    var currentdate = date.getFullYear() + seperator1 + month + seperator1 + strDate;
    return currentdate;
}

// YYYY-MM-DD HH:MM:SS
function getNowFormatDate2(seperator1, seperator2, seperator3) 
{
    var date = new Date();
    if ( seperator1 == undefined )
        seperator1 = "-";

    if ( seperator2 == undefined )
        seperator2 = ":";

    if ( seperator3 == undefined)
        seperator3 = " ";

    var month = getTimeUnit( date.getMonth() + 1 );
    var strDate = getTimeUnit( date.getDate() );

    var hour = getTimeUnit( date.getHours() );
    var minute = getTimeUnit( date.getMinutes() );
    var second = getTimeUnit( date.getSeconds() );

    var currentdate = date.getFullYear() + seperator1 + month + seperator1 + strDate
	    + seperator3 + hour + seperator2 + minute + seperator2 + second; 
    return currentdate;
}

// HH:MM:SS
function getNowFormatDate3(seperator2)
{
    if ( seperator2 == undefined )
        seperator2 = ":";

    var date = new Date();

    var hour = getTimeUnit( date.getHours() );
    var minute = getTimeUnit( date.getMinutes() );
    var second = getTimeUnit( date.getSeconds() );
    var currentdate = hour + seperator2 + minute + seperator2 + second; 
    return currentdate;
}

// HH:MM
function getNowFormatDate4(seperator2) 
{
    if ( seperator2 == undefined )
        seperator2 = ":";

    var date = new Date();
    
    var hour = getTimeUnit( date.getHours() );
    var minute = getTimeUnit( date.getMinutes() );

    var currentdate = hour + seperator2 + minute; 

    return currentdate;
}

function initStatCommon($scope,CommonService){
    CommonService.heartbeat();
    $scope.user=null;
    if(sessionStorage.getItem("strUser")){
	$scope.user=JSON.parse(sessionStorage.getItem("strUser"));
    }else{
	location="#/"
    }
    /*$scope.myStatDefault=null;//默认统计信息
    if(sessionStorage.getItem("myStatDefault")){
	$scope.myStatDefault=JSON.parse(sessionStorage.getItem("myStatDefault"));
    }*/
}

function initStatBus($scope,CommonService){
    //判断系统
    $scope.tmsItem=[];//远程业务项
    $scope.uhsItem=[];//医联体业务项
    $scope.selTwoBusiness=false;//是否远程业务与医联体业务都可选
    $scope.curItem=[];//当前显示业务项
    for(var i=0;i<SYSTEM.length;i++){
	if(SYSTEM[i]=='tms'){
	    $scope.tmsItem.push({name:"远程门诊",id:"tm",isCheck:false});
	    $scope.tmsItem.push({name:"远程会诊",id:"mdt",isCheck:false});
	    $scope.tmsItem.push({name:"双向转诊",id:"dual",isCheck:false});
	}else if(SYSTEM[i]=='uhs'){
	    //从后台获取业务条目加入集合
	    $scope.uhsItem.push({name:"专项手术培训",id:"1",isCheck:false})
	    $scope.uhsItem.push({name:"专项手术培训",id:"1",isCheck:false})
	    $scope.uhsItem.push({name:"专项手术培训",id:"1",isCheck:false})
	    $scope.uhsItem.push({name:"专项手术培训",id:"1",isCheck:false})
	    $scope.uhsItem.push({name:"专项手术培训",id:"1",isCheck:false})
	    $scope.uhsItem.push({name:"专项手术培训",id:"1",isCheck:false})
	    $scope.uhsItem.push({name:"专项手术培训",id:"1",isCheck:false})
	    $scope.uhsItem.push({name:"专项手术培训",id:"1",isCheck:false})
	    $scope.uhsItem.push({name:"专项手术培训",id:"1",isCheck:false})
	    $scope.uhsItem.push({name:"专项手术培训",id:"1",isCheck:false})
	    $scope.uhsItem.push({name:"专项手术培训",id:"1",isCheck:false})
	    $scope.uhsItem.push({name:"专项手术培训",id:"1",isCheck:false})
	    $scope.uhsItem.push({name:"专项手术培训",id:"1",isCheck:false})
	    $scope.uhsItem.push({name:"专项手术培训",id:"1",isCheck:false})
	    $scope.uhsItem.push({name:"专项手术培训",id:"1",isCheck:false})
	    $scope.uhsItem.push({name:"专项手术培训",id:"1",isCheck:false})
	}
    }

    if($scope.tmsItem.length>0&&$scope.uhsItem.length>0){
	$scope.selTwoBusiness=true;
	$scope.curItem=$scope.tmsItem;
	$scope.curItemType=0;
    }else if($scope.tmsItem.length>0){
	$scope.curItem=$scope.tmsItem;
	$scope.curItemType=0;
    }else if($scope.uhsItem.length>0){
	$scope.curItem=$scope.uhsItem;
	$scope.curItemType=1;
    }

    $scope.curItemType=0;//用户区分当前业务是远程业务，医联体
    $scope.selbus=function(bus){
	if(bus=="tms"){
	    //$(".bus-tms").addClass("btn-primary");
	    //$(".bus-uhs").removeClass("btn-primary");
	    $scope.curItem=$scope.tmsItem;
	    $scope.curItemType=0;
	}else if(bus=="uhs"){
	    //$(".bus-uhs").addClass("btn-primary");
	    //$(".bus-tms").removeClass("btn-primary");
	    $scope.curItem=$scope.uhsItem;
	    $scope.curItemType=1;
	}
    }
}
function initStatTime($scope,CommonService){
    $scope.yearTypes=[{id:0,name:"按月"},{id:1,name:"按季度"}];//统计某年是按月还是季度
    $scope.monthTypes=[{id:0,name:"按周"},{id:1,name:"按天"}];
    $scope.monthType=0;
    $scope.monthType5=0;
    $scope.yearType=0;
    $scope.months=[{id:1,name:"1月"},
		{id:2,name:"2月"},
		{id:3,name:"3月"},
		{id:4,name:"4月"},
		{id:5,name:"5月"},
		{id:6,name:"6月"},
		{id:7,name:"7月"},
		{id:8,name:"8月"},
		{id:9,name:"9月"},
		{id:10,name:"10月"},
		{id:11,name:"11月"},
		{id:12,name:"12月"}];//月份
    //获取最近10年年份
    var myDate = new Date();
    var year=myDate.getFullYear();
    year+=1;
    $scope.years=[];
    for(var i=0;i<10;i++){
	year-=1;
	$scope.years.push({year:year,id:year});
    }
    $scope.timeType=0;
    $scope.selTime=function(type,e){
	//$(".time-group").find(".btn").removeClass("btn-primary")
	//$(e.target).addClass("btn-primary");
	$scope.timeType=type;
    }
}
function getBarOption(title){
    var option= {
	title : {
	    text: title,
	    subtext: ''
	},
	tooltip : {
	    trigger: 'axis'
	},
	legend: {
	    data:['蒸发量','降水量']
	},
	toolbox: {
	    show : true,
	    feature : {
		dataView : {show: true, readOnly: false},
		magicType : {show: true, type: ['line', 'bar']},
		restore : {show: true},
		saveAsImage : {show: true}
	    }
	},
	calculable : true,
	xAxis : [
	    {
		type : 'category',
		data : ['1月','2月','3月','4月','5月','6月','7月','8月','9月','10月','11月','12月']
	    }
	],
	yAxis : [
	    {
		name:'数量',
		type : 'value'
	    }
	],
	series : []
    };
    return option;

}

function getStatDefault($scope,CommonService,statType){
    //获取给页面默认参数
    $scope.myStatDefault=null;
    var uriData={stat_code:statType}
    uriData=JSON.stringify(uriData);
    CommonService.updatePartOne('stat/centerDoctorTime', uriData, function (data) {
	var statDefaults=listToObject(data,'rows').rows;
	if(statDefaults&&statDefaults.length>0){
	    $scope.myStatDefault=statDefaults[0]
	    if($scope.myStatDefault){
		if($scope.myStatDefault.stat_code==statType){
		    var parameter=JSON.parse($scope.myStatDefault.parameter_json)
		    //console.log(parameter);
		    $scope.data=parameter;
		    $scope.initPage($scope.data);
		    setTimeout(function(){
			$scope.searchData($scope.data);
		    },500);
		}
	    }
	}
    }, function (data, status) {//失败时的回调函数
	$scope.msg='获取默认参数出错！';
	$("#msg").modal("show");
    })
}
function saveDefaultStat($scope,CommonService,statType,parameter_json){
    var uriData={stat_code:statType,parameter_json:parameter_json};
    uriData=JSON.stringify(uriData);
    CommonService.updateOne('stat/centerDoctorTime', uriData, function (data) {
	//console.log(data);
	$scope.msg='保存成功！';
	$("#msg").modal("show");
    }, function (data, status) {//失败时的回调函数
	$scope.msg='保存出错！';
	$("#msg").modal("show");
    })
}
function initxAxis($scope,CommonService,xAxisType1,xAxisType2){
    $scope.xAxisType=xAxisType1;
    $scope.xAxisType1=xAxisType1;
    //alert($scope.xAxisType1);
    $scope.xAxisType2=xAxisType2;
    $scope.changexAxis=function(){
	if($scope.tempData){
	    if($scope.xAxisType==$scope.xAxisType1){
		$scope.xAxisType=$scope.xAxisType2;
		$scope.showxAxisForBus($scope.tempData);
	    }else{
		$scope.xAxisType=$scope.xAxisType1;
		$scope.showxAxisForTime($scope.tempData);
	    }
	}
    }
    $scope.showxAxisForTime=function(data){
	$scope.option.title.subtext=data.title;
	$scope.option.xAxis[0].data=data.xAxis;
	if(data.yAxisType==0){
	    $scope.option.yAxis[0].name="数量（次）";
	}else if(data.yAxisType==1){
	    $scope.option.yAxis[0].name="费用（元）";
	}
	var series=[];
	var legend=[];
	for(var i=0;i<data.dataItems.length;i++){
	    legend.push(data.dataItems[i].name)
	    series.push({
		name:data.dataItems[i].name,
		type:'bar',
		data:data.dataItems[i].data,
		markLine : {
		    data : [
			{type : 'average', name : '平均值'}
		    ]
		}
	    });
	}
	$scope.option.legend.data=legend;
	$scope.option.series=series;
	$scope.centerDoctorTime.clear();
	$scope.centerDoctorTime.setOption($scope.option);
    }

    $scope.showxAxisForBus=function(data){
	var xAxis=[];
	var seriesName="";
	if(data.yAxisType==0){
	    seriesName="业务数量"
	}else if(data.yAxisType==1){
	    seriesName="业务费用"
	}
	var series=[{
		name:seriesName,
		type:'bar',
		data:[],
		markLine : {
		    data : [
			{type : 'average', name : '平均值'}
		    ]
		}
	    }];
	for(var i=0;i<data.dataItems.length;i++){
	    xAxis.push(data.dataItems[i].name);
	    var tdata=data.dataItems[i].data;
	    var total=0;
	    for(var j=0;j<tdata.length;j++){
		if(tdata[j]){
		    total+=parseFloat(tdata[j]);
		}
	    }
	    series[0].data.push(total);
	}
	$scope.option.xAxis[0].data=xAxis;
	$scope.option.series=series;
	$scope.centerDoctorTime.clear();
	$scope.centerDoctorTime.setOption($scope.option);
    }
}

function debugUploader(uploader) {
	uploader.onWhenAddingFileFailed = function(item /*{File|FileLikeObject}*/ , filter, options) {
		console.info('onWhenAddingFileFailed', item, filter, options);
	};
	uploader.onAfterAddingFile = function(fileItem) {
		console.info('onAfterAddingFile', fileItem);
	};
	uploader.onAfterAddingAll = function(addedFileItems) {
		console.info('onAfterAddingAll', addedFileItems);
	};
	uploader.onBeforeUploadItem = function(item) {
		console.info('onBeforeUploadItem', item);
	};
	uploader.onProgressItem = function(fileItem, progress) {
		console.info('onProgressItem', fileItem, progress);
	};
	uploader.onProgressAll = function(progress) {
		console.info('onProgressAll', progress);
	};
	uploader.onSuccessItem = function(fileItem, response, status, headers) {
		console.info('onSuccessItem', fileItem, response, status, headers);
	};
	uploader.onErrorItem = function(fileItem, response, status, headers) {
		console.info('onErrorItem', fileItem, response, status, headers);
	};
	uploader.onCancelItem = function(fileItem, response, status, headers) {
		console.info('onCancelItem', fileItem, response, status, headers);
	};
	uploader.onCompleteAll = function() {
		console.info('onCompleteAll');
	};
}

function createUploader(FileUploader, maxFile, module) {
	module = module || "common";
	var uploader = new FileUploader({
		url: appConfig.uploadURL,
		method: 'POST',
		headers: {
			Authorization: sessionStorage.getItem('authToken'),
		},
		autoUpload: true,
		formData: [{
			"module": module,
		}]
	});

	debugUploader(uploader);

	uploader.filters.push({
		name: 'customFilter',
		fn: function (item /*{File|FileLikeObject}*/, options) {
			return this.queue.length < maxFile;
		}
	});

    uploader.onCompleteItem = function (fileItem, response, status, headers) {
      console.info('onCompleteItem', fileItem, response, status, headers);
      fileItem.response = response;
    };

	return uploader;
}

function checkSvcError (data, status) {
	if (status == 401) {
		gotoLogin();
	}
}

function svcError (data, status, callback) {
	checkSvcError(data, status);

	if (data && data.message) {
		MsgBox.error("错误", "操作失败：" + data.message);
	} else {
		MsgBox.error("错误", "操作失败");
	}
	if (callback) {
		callback(data, status);
	}
}

function joinSubList (list, subList, subkey, pk, subListName) {
	pk = pk || "id";
	subListName = subListName || "subList";

	if (!subkey) {
		return list;
	}

	var dict = {};
	for (var i in list) {
		dict[list[i][pk]] = list[i];
		list[i][subListName] = [];
	}

	for (var i in subList) {
		dict[subList[i][subkey]][subListName].push(subList[i]);
	}
}

function dataURLtoBlob(dataUrl) {
	var arr = dataUrl.split(','), mime = arr[0].match(/:(.*?);/)[1],
		bstr = atob(arr[1]), n = bstr.length, u8arr = new Uint8Array(n);
	while(n--){
		u8arr[n] = bstr.charCodeAt(n);
	}
	return new Blob([u8arr], {type:mime});
}

function startXHR(xhr, fd, headers, callback) {

	headers = headers || {};
	for (var i in headers) {
		xhr.setRequestHeader(i, headers[i]);
	}

	callback = callback || nullFn;

	xhr.onload = function(e) {
		if (this.status == 201) {
			var res = json2Obj(this.response);
			callback(true, res);
		} else {
			D("upload failed");
			callback(false);
		}
	};

	xhr.onerror = function(e) {
		D("Error loading");
		callback(false)
	};

	xhr.onprogress = function(e) {
		if (e.lengthComputable) {
			var percent = Math.floor(100 * e.loaded / e.total);
			callback(false, [percent]);
			D("onprogress :" + percent)
		}
	};

	xhr.send(fd);
}

function uploadCanvas(canvas, url, headers, callback) {
	var xhr = new XMLHttpRequest();
	xhr.open("POST", url, true);

	var fd = new FormData();
	var dataUrl = canvas.toDataURL('image/png');
	var blob = dataURLtoBlob(dataUrl);
	fd.append("module", "allergy", "allergy");
	fd.append("file", blob, "image.png");

	D("uploadCanvas, size : " + blob.size);
	startXHR(xhr, fd, headers, callback);
}

function uploadUint8Data(data, url, headers, callback, fileName) {
	fileName = fileName || "data.bin";
	var xhr = new XMLHttpRequest();
	xhr.open("POST", url, true);

	var fd = new FormData();
	var blob = new Blob([data], {type:"application/octet-stream"});
	//fd.append("module", "allergy", "allergy");
	fd.append("file", blob, fileName);

	D("uploadUint8Data, size : " + blob.size);
	startXHR(xhr, fd, headers, callback);
}

function uploadFile(file, url, headers, callback) {
	var xhr = new XMLHttpRequest();
	xhr.open("POST", url, true);

	var fd = new FormData();
	fd.append("file", file);

	startXHR(xhr, fd, headers, callback);
}

function hexToUnit8(hexData) {
	var count = Math.floor(hexData.length / 2);
	var ret = new Unit8Array(count);
	for (var i = 0, j = 0; i < count; i++, j += 2) {
		ret[i] = parseInt(hexData.charAt(j), 16) << 4;
		ret[i] += parseInt(hexData.charAt(j + 1), 16);
	}
	return ret;
}

var __JSON_PROTECTION_PREFIX = /^\)\]\}',?\n/; //';
var __JSON_START = /^\s*(\[|\{[^\{])/;
var __JSON_END = /[\}\]]\s*$/;

function obj2Json (obj) {
	return JSON.stringify(obj);
}

function json2Obj (str) {
	var obj = str;

	if (typeof str === 'string') {
		str = str.replace(__JSON_PROTECTION_PREFIX, '');
		if (__JSON_START.test(str) && __JSON_END.test(str)) {
			obj = JSON.parse(str);
		}
	}
	return obj;
}

function httpGetArrayBuffer(url, callback) {
	var instance = this;

	if (url.startsWith("http://") || url.startsWith("https://")) {
		url = url;
	} else {
		url = window.baseUrl + url;
	}

	var xhr = new XMLHttpRequest();

	xhr.open('GET', url, true); // true: asynchronous | false: synchronous
	xhr.responseType = 'arraybuffer';
	xhr.setRequestHeader('Authorization', getAuthToken());

	var start = new Date().getTime(this);

	xhr.onload = function(e) {
		if (this.status == 200) {
			var end = new Date().getTime();
			var time = end - start;
			var len = this.response.byteLength;
			//d("size: " + len/1024 + " KB");
			//d("load time: " + time + "ms");

			callback(this.response);

			var speed = len * 8 / (time * 1000);
			//d("speed (Mbps): " + speed + "Mbps");
		} else {
			D("get stream failed");
			callback(null)
		}
	};

	xhr.onerror = function(e) {
		D("Error loading");
		callback(null)
	};

	xhr.onprogress = function(e) {
		if (e.lengthComputable) {
			var percent = Math.floor(100 * e.loaded / e.total);
			//d("onprogress :" + percent)
		}
	};

	xhr.send(null);
};

function httpGetJsonObj(url, callback) {
	var instance = this;

	url = window.baseUrl + url;

	var xhr = new XMLHttpRequest();

	xhr.open('GET', url, true); // true: asynchronous | false: synchronous
	//xhr.responseType = 'arraybuffer';
	xhr.setRequestHeader('Authorization', getAuthToken());

	var start = new Date().getTime(this);

	xhr.onload = function(e) {
		if (this.status == 200) {
			callback(json2Obj(this.response));
		} else {
			D("get string failed");
			callback(null);
		}
	};

	xhr.onerror = function(e) {
		D("Error loading");
		callback(null)
	};

	xhr.onprogress = function(e) {
		if (e.lengthComputable) {
			//var percent = Math.floor(100 * e.loaded / e.total);
			//d("onprogress :" + percent)
			//callback(null, percent);
		}
	};

	xhr.send(null);
};

function getDict(CommonService, type_code, callback) {
	var data = {type_code : type_code};
	var uriData = JSON.stringify(data);
	CommonService.updatePartOne('Data_Dict/Data_Dict_Value',uriData,function(data){
		callback(listToObject(data,'rows').rows);
	}, function(data,status) {
		E("get dict failed : " + type_code);
		callback(null);
	})
}

function classifyItems(items, field, obj) {
	obj = obj || {};
	var item;
	var key;
	for (var i in items) {
		item = items[i];
		key = item[field];
		if (!obj[key]) {
			obj[key] = [];
		}
		obj[key].push(item);
	}
	return obj;
}

//e.g. getDictEx($scope, CommonService, "'DIAG_RESULT','TREAT_METHOD','ITEM_STATUS'", funtion(result, dicts){})
function getDictEx($scope, CommonService, type_codes, callback) {
	callback = callback || nullFn;
	var uriData = "simple=1&codes=" + type_codes;
	CommonService.getAll('allergy/dictItem',uriData,function(data){
		var items = listToObject(data,'rows').rows;
		var dicts = classifyItems(items, "type_code", $scope)
		callback(true, dicts);
	}, function(data,status) {
		E("get dict failed : " + type_codes);
		callback(false);
	})
}

function getHashMap(list, key) {
	var m = {};
	var item;
	for (var i in list) {
		item = list[i];
		m[item[key]] = item;
	}
	return m;
}

function getPatientInfo(CommonService, hospital_code, id_no, callback) {
	var uriData = "no=8&hospital_code=" + hospital_code + "&id_no=" + id_no;
	CommonService.getAll('myPatient', uriData, function(data){
		callback(listToObject(data,'rows').rows);
	}, function(data, status) {
		callback(null, data, status);
	})
}

function checkUserRole(role) {

	var userRoles = appCache.getObject("roles");
	if (!userRoles) {
		return false;
	}

	for (var i in userRoles) {
		if (userRoles[i][1] == role) {
			return true;
		}
	}

	return false;
}

function adjustFormGroup($group) {

	$group.each(function (idx, item) {

		var $item = $(item)
		var $label = $item.find(".form-label");
		var $control = $item.find(".form-control");

		var cwidth = $item.width() - $label.width() - 40;

		$control.width(cwidth);

		//D("wwww - > " + $control.width() + ", " + cwidth);
	});
}

var F_OPTIONAL = 0x01;
var F_TEXT = 0x02;
var F_SELECT = 0x04;

function checkFields(data, fields, out) {

	var item;
	var key;
	var v;
	var type;
	var min;
	var max;
	var inputType;
	var flags;

	var ferr = function(item, t) {
		var a = (flags & F_SELECT) ? "选择" : "输入";
		if (t > 0) {
			item.errMsg = name + "大于" + max + "，请重新" + a;
		} else if (t < 0) {
			item.errMsg = name + "小于" + max + "，请重新" + a;
		} else {
			item.errMsg = name + "不能为空，请重新" + a;
		}
		return item;
	};

	for (var i in fields) {
		item = fields[i];
		if ((typeof item) == "string") {
			if (!data[item] || !trim(data[item]).length) {
				return item;
			}
			continue;
		}

		key = item[0];
		name = item[1];
		type = item[2];
		if (undefined != type || type instanceof String) {
			min = item[3];
			max = item[4];
			flags = F_TEXT;
		} else {
			flags = parseInt(type);
			type = item[3];
			min = item[4];
			max = item[5];
			if (!flags) {
				flags = F_TEXT;
			}
		}
		v = data[key];
		switch (type) {
			case "int":
				v = parseInt(v);
				if (isNaN(v)) {
					if (flags & F_OPTIONAL) {
						v = 0;
					} else {
						return ferr(item);
					}
				}
				if (min != undefined && v < min) {
					return ferr(item, -1) ;
				}
				if (max != undefined && v > min) {
					return ferr(item, 1) ;
				}
				break;
			case "float":
				v = parseFloat(v);
				if (isNaN(v)) {
					if (flags & F_OPTIONAL) {
						v = 0;
					} else {
						return ferr(item);
					}
				}
				if (min != undefined && v < min) {
					return ferr(item, -1) ;
				}
				if (max != undefined && v > min) {
					return ferr(item, 1) ;
				}
				break;
			case "str":
			default:
				if (!v || v == "") {
					if (flags & F_OPTIONAL) {
						v = "";
					} else {
						return ferr(item) ;
					}
				}
				if (min != undefined && v < min) {
					return ferr(item, -1) ;
				}
				if (max != undefined && v > min) {
					return ferr(item, 1) ;
				}
		}
		data[key] = v;
		if (out) {
			out[key] = v;
		}
	}
	return null;
}

function getTimeRangeParam (stime, etime) {
	var uriData = "";
	if (stime && stime.length) {
		uriData += "&stime=" + stime + " 00:00:00";
	}
	if (etime && etime.length) {
		uriData += "&etime=" + etime + " 23:59:59";
	}
	return uriData;
}

function getTimeUriData(requestObj, stimeField, etimeField) {
	stimeField = stimeField || "stime";
	etimeField = etimeField || "etime";
	var stime = requestObj[stimeField];
	var etime = requestObj[etimeField];
	return getTimeRangeParam(stime, etime);
}

function getSearchUriData(requestObj, msgField) {
	msgField = msgField || "searchMsg";
	var searchMsg = requestObj[msgField];
	if (searchMsg) {
		return "&s=" + searchMsg;
	}
	return "";
}

function paddingZero(str, length) {
	str = "" + str;
	while (str.length < length) {
		str = "0" + str;
	}
	return str;
}

function printEl(title, wraperId) {
	var backupHtml = document.body.innerHTML;
	var backupTitle = document.title;
	document.title = title;
	document.body.innerHTML = document.getElementById(wraperId).innerHTML;
	window.print();
	document.title = backupTitle
	document.body.innerHTML = backupHtml;
	location.reload();
}

function gotoPatDetail(hospital_code,id_no) {
	appCache.setObject("id_no", id_no);
	appCache.setObject("pat_hospital_code", hospital_code);
	window.location.href='#/patientDetails';
}

function getObjectURL(file) {
	var url = null;
	if (window.createObjectURL) {
		url = window.createObjectURL(file);
	} else if (window.URL){
		url = window.URL.createObjectURL(file);
	} else if (window.webkitURL){
		url = window.webkitURL.createObjectURL(file);
	}
	return url;
}

function sendNotify(title, msg, alt) {

	//return;

	if (!Notification) {
		alert(msg);
		return;
	}

	if (Notification.permission == "granted") {
		var options = {
			dir: "rtl",
			lang: "zh-CN",
			body: msg,
			icon: "resource/images/notify.png",
			// tag:"msgId",
		}
		var n = new Notification(title, options);
	} else {
		Notification.requestPermission(function (status) {
			if (Notification.permission !== status) {
				Notification.permission = status;
			}
			if (Notification.permission === "granted") {
				sendNotify(title, msg);
			}
		});
	}

	//if (alt) alert(msg);
}

function delayExec(scope, func, time, name) {
	name = name || "delayExecTimer";

	if (scope[name]) {
		clearTimeout(scope[name]);
		scope[name] = null;
	}

	scope[name] = setTimeout(function () {
		scope[name] = null;
		D("delayExec, func executed");
		func.apply(scope)
	}, time);
}

function delayExec2(scope, func, time, name) {
	name = name || "delayExecTimer";

	if (scope[name]) {
		//clearTimeout(scope[name]);
		//scope[name] = null;
		return;
	}

	scope[name] = setTimeout(function () {
		scope[name] = null;
		D("delayExec, func executed");
		func.apply(scope)
	}, time);
}

var Nations = [
	"未知","汉","蒙古","回","藏","维吾尔","苗","彝","壮","布依","朝鲜",
	"满","侗","瑶","白","土家","哈尼","哈萨克","傣","黎","傈僳",
	"佤","畲","高山","拉祜","水","东乡","纳西","景颇","柯尔克孜","土",
	"达斡尔","仫佬","羌","布朗","撒拉","毛南","仡佬","锡伯","阿昌","普米",
	"塔吉克","怒","乌孜别克","俄罗斯","鄂温克","德昂","保安","裕固","京","塔塔尔",
	"独龙","鄂伦春","赫哲","门巴","珞巴","基诺"
];

function getNationText(nation) {
	nation = parseInt(nation);
	if (nation < Nations.length) {
		return Nations[nation];
	}
	switch (nation) {
		case 98:
			return "外国血统中国籍人士";
		case 97:
			return "其他";
		default:
			return "未知";
	}
}

var IdCardReaderCallbacks = [];

function useCallback(msg) {
	if (!msg || !msg.type) {
		return;
	}

	if (msg.type == "idcard") {
		msg.data.sexText = msg.data.sex == 1 ? "男" : "女";
		msg.data.nationText = getNationText(msg.data.nation);
	}

	for (var i in IdCardReaderCallbacks) {
		var cb = IdCardReaderCallbacks[i];
		if (cb.type == msg.type) {
			cb.callback(msg.data);
		}
	}

	if (!msg.image) {
		if (msg.pic) {
			var uint8Data = hexToUnit8(msg.pic);
			uploadUint8Data(uint8Data,
				window.uploadURL,
				{"Authorization" : getAuthToken()},
				function (result, data) {
					if (result && data) {
						msg.image = data;
						useCallback(msg);
					}
				},
				"image.bmp"
			);
		}
	}
}

function fireUseEventR(data) {
	if (!window.__useEventR) {
		var useEventR = document.createEvent("Event");
		useEventR.initEvent("useEventR", true, true);
		window.__useEventR = useEventR;
	}
	var useEventR = window.__useEventR;
	var useObjectR = document.getElementById("useObjectR");
	if (useObjectR) {
		useObjectR.innerText = S(data);
		useObjectR.dispatchEvent(useEventR);
	}
}

function initIdCardReader(callback, type) {
	type = type || "idcard";
	var cb = {
		type : type,
		callback : callback,
	};
	registerRouteChangeCallback(function () {
		if (IdCardReaderCallbacks.length) {
			IdCardReaderCallbacks = [];
			fireUseEventR({type:"stop_read_idno"});
		}
	});
	IdCardReaderCallbacks.push(cb);
	var r = function () {
		for (var i in IdCardReaderCallbacks) {
			if (cb == IdCardReaderCallbacks[i]) {
				var a = IdCardReaderCallbacks.splice(i, 1);
				return true;
			}
		}
		return false;
	};

	if (window.useObjectInited) {
		fireUseEventR({type:"start_read_idno"});
		return r;
	}
	window.useObjectInited = true;
	//$(document.body).append($("<div style='display: none;' id='useObject'></div>"));
	var useObject = document.getElementById("useObject");

	useObject.addEventListener("useEvent", function() {
		var str = useObject.innerText;
		//D(str);
		var msg = JSON.parse(str);
		//D(data);
		useCallback(msg);
	});
	fireUseEventR({type:"start_read_idno"});
	return r;
}



// 数组对象 字符串排序， fieldname 为需要进行排序的字段，a, b 为两个对象
// 调用方式:
//   var array = [];
//   array.sort( sortByItemStr("fieldname") );
function sortByItemStr( fieldname )
{
    return function(a, b)
    {
        if ( a[fieldname] == "选择全部" )
            return -1;
        
        return a[fieldname].localeCompare(b[fieldname]);
    }
}

// 数字排序
function sortByItemNum( fieldname )
{
    return function(a, b)
    {
        return a[fieldname] - b[fieldname];
    }
}

// 响应函数，
// 
function onErrorResult(data, defaultMsg)
{
    if ( data )
    {
        if ( data.original_message )
            MsgBox.info( "提示", data.original_message);
        else if ( data.message != "" ) 
            MsgBox.info( "提示", data.message );
        else if ( defaultMsg != undefined )
            MsgBox.info( "提示", defaultMsg );
        else
            MsgBox.info( "提示", "操作失败！" );
    }
    else
    {
        if ( defaultMsg != undefined )
            MsgBox.info( "提示", defaultMsg );
        else
            MsgBox.info( "提示", "操作失败！" );
    }
}


// 所有数据字典的查询用于显示的部分都在前台完成
// list: 列表
// field: 查询的字段，为字典代码值
// type_code：字典名称
// value: 赋值的字段
function getDataDict(list, field, type_code, value)
{
    var dataDict =JSON.parse(sessionStorage.getItem("dataDict"));
    for ( var i = 0; i < dataDict.length; i++)
    {
        if ( dataDict[i].type_code != type_code )
            continue;

        for( var j = 0; j < list.length; j++ )
        {
            if ( dataDict[i].code != list[j][field] )
                continue;

            list[j][value] = dataDict[i].name;
        }
    }
}

// 获取指定数据字典code的值
function getDataDictValue( type_code, code )
{
    var dataDict =JSON.parse(sessionStorage.getItem("dataDict"));
    for ( var i = 0; i < dataDict.length; i++)
    {
        if ( dataDict[i].type_code != type_code )
            continue;

        if ( dataDict[i].code != code )
            continue;

        return dataDict[i].name;
    }    
}
