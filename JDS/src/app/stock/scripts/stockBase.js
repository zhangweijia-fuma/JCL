// /////////////////////////////////////////////////
// 基本常量定义

// K线类型
var STOCK_K_LINE_TYPE = 
{
	SDT_MINUTE_1  : 0,
	SDT_MINUTE_5  : 1,
	SDT_MINUTE_15 : 2,
	SDT_MINUTE_30 : 3,
	SDT_MINUTE_60 : 4,
	SDT_DAY		  : 5,
	SDT_WEEK	  : 6,	
	SDT_MONTH	  : 7,
	SDT_YEAR	  : 8,
};



var STOCK_COLOR = 
{
	// K线图背景颜色， 默认为黑色
	KLineBackColor     : 'black',

	// K线图坐标颜色, 默认为红色
	KLineColor         : 'red',

	// 坐标轴字体颜色
	KLineCoordinateTextColor : 'red',

	// 阳线边线颜色
	PositiveLineColor  : 'red',

	// 阴线填充颜色
	NegitiveLineColor  : '#00fdff',

	// 十字星颜色
	CrossLineColor     : 'white',

	// 证券名称颜色
	NameColor          : 'yellow',

	// 周期均线颜色
	MA5Color       : 'white',
	MA10Color      : 'yellow',
	MA20Color      : 'pink',
	MA60Color      : 'green',
	volMa5Color    : 'white',
	volMa10Color   : 'yellow',

	// 涨跌颜色
	RaiseColor     : 'read', 
	DownColor      : 'green', 
	RaiseFastColor : 'pink',

	// K线详情十字线
	DetailCrossColor : 'white',

	// 证券清单
	ListNameColor  : 'yellow',

	// 盘口部分颜色
	HandicapCode   : 'yellow',
	HandicapName   : 'yellow',

	// 最大值、最小值标注颜色
	TagColor       : 'white',

	// 日期格颜色
	dateGridColor  : "red",

    btnSelectedColor    : "#FFC107",
    btnNonSelectedColor : "white",
};

var K_LINE_NAME = ["1分钟", "5分钟", "15分钟", "30分钟", "60分钟", "日线", "周线", "月线", "年线" ];

// 每种K线对应的日期格宽度
var nDateGridWidth = [ 60, 60, 60, 60, 60, 80, 40, 40, 80 ];

// 每种K线对应的时间跨度，单位：月
// 日线、周线、月线、年线
var nDayTimeSpan = [ 1, 2, 3, 4, 5, 6];
// 分钟线
var nMinuteTimeSpan = [1, 2, 3, 4, 5, 10, 15, 20, 30, 45, 60, 90 ];

// 成交量数量区间，单位为手
var volIntervalList = [ 1000, 2000, 4000, 8000, 10000, 15000, 20000, 30000, 40000, 50000, 60000, 70000, 80000, 90000, 100000 ];

// K线价格区间，单位：分
var priceIntervalList = [2, 5, 10, 20, 50, 100, 200, 300, 400, 500, 1000, 1500, 2000, 3000, 5000];



// var str = "2010-08-01";
// // 转换日期格式
// str = str.replace(/-/g, '/'); // "2010/08/01";
// // 创建日期对象
// var date = new Date(str);

// strFormat： yyyy-MM-dd HH:mm:ss 或 yyyy-MM-dd
function formatDate(dateStr, strFormat) 
{
	str = dateStr.replace(/-/g, '/'); // "2010/08/01";
	// 创建日期对象
	var date = new Date(str);
	return date;
}


// YYYY-MM-DD 或 YYYY-MM-DD HH:MM:SS
function convertDateToStr(date, seperator1, seperator2) 
{
    if ( seperator1 == undefined )
        seperator1 = "-";

    var month = getTimeUnit( date.getMonth() + 1 );
    var strDate = getTimeUnit( date.getDate() );

    var dateStr = date.getFullYear() + seperator1 + month + seperator1 + strDate;

    if ( seperator2 == ":" )
    {
    	var hour = getTimeUnit( date.getHours() );
	    var minute = getTimeUnit( date.getMinutes() );
	    var second = getTimeUnit( date.getSeconds() );

	    dateStr = dateStr + " " + hour + seperator2 + minute + seperator2 + second; 
    }

    return dateStr;
}

