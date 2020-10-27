////////////////////////////////////////////////////////////////////////////////////////////

angular.module('stockControllers', ['highcharts-ng'])

//////////////////////////////////////////////////////////////////////////////////////////////
// 
.controller("stockCtrl", function ($scope, CommonService)
{
    CommonService.heartbeat();
    $scope.user = null;

    if(sessionStorage.getItem("strUser"))
        $scope.user = JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"

    // 默认显示日线数据
    $scope.dataType = "day";

    // K线类型
    var emKLineType     = STOCK_K_LINE_TYPE.SDT_DAY;

    // 各区域

    // Y轴区域
    var YRect           = {};

    // Y轴宽度
    var YRectWidth      = 4;

    // 标题高度
    var titleHigh       = 30;

    // K线区域
    var KRect           = {};

    // K线坐标轴
    var minGridHeight   = 30;
    var maxGridHeight   = 100;
    
    // K线矩形宽度，至少是偶数
    var rectWidth       = 4;

    // 显示的K线矩形个数
    var showUnitCount   = 0;
    
    // 最多显示K线矩形数量，对于新上市股票
    var maxShowUnitCount= 0;

    // 待显示的数据
    var showStockData   = [];

    // 股票价格（含MA值）的最高值、最低值
    var nMaHigh         = 0;
    var nMaLow          = 0;
    
    // 股票价格（不含MA值）的最高值、最低值
    var nHigh           = 0;
    var nLow            = 0;

    // 价格区间的最大值、最小值
    var nMax            = 0;
    var nMin            = 0;

    var kLineCanvas     = null;

    // K线横坐标线数量及间隔高度
    var nKLineCount     = 0;
    var nKLineInterval  = 0;

    // K线横坐标线数据
    var kLineInfo       = [];

    // 价格区间
    var nKPriceInterval = 0;

    // MA 对应的坐标数组
    var ma5Points       = [];
    var ma10Points      = [];
    var ma20Points      = [];
    var ma60Points      = [];
    
    //////////////////////////////////
    var volCanvas       = null;

    // 成交量窗口
    var volRect         = {};
    var volRectHeight   = 100;
    var isShowVol       = true;

    // 成交量的最大值
    var volMax          = 0;

    var volMa5Points    = [];
    var volMa10Points   = [];

    // 成交量横坐标数据
    var volLineInfo     = [];

    var nVolLineCount   = 0;
    var nVolInterval    = 0;
    var nVolLineInterval= 0;
    var nVolMultiple    = 1;

    var startDate       = getNowFormatDate2();

    ///////////////////////////////////
    // 日期窗格
    
    // 日期窗口
    var dateRect        = {};
    var dateRectHeight  = 20;
    var tmNextTradeTime = null;
    var nTimePeriod     = 0;
    var dateCanvas      = null;
    var dateGridPoints  = [];

    $scope.initialParam = function()
    {
        // Y轴宽度
        YRectWidth      = 80;

        // K线坐标轴
        minGridHeight   = 30;
        maxGridHeight   = 100;
        
        // K线矩形宽度，至少是偶数
        rectWidth       = 4;

        // 显示的K线矩形个数
        showUnitCount   = 0;
        
        // 最多显示K线矩形数量，对于新上市股票
        maxShowUnitCount= 0;

        // 待显示的数据
        showStockData   = [];

        // 股票价格（含MA值）的最高值、最低值
        nMaHigh         = 0;
        nMaLow          = 0;
        
        // 股票价格（不含MA值）的最高值、最低值
        nHigh           = 0;
        nLow            = 0;

        // 价格区间的最大值、最小值
        nMax            = 0;
        nMin            = 0;

        // K线横坐标线数量及间隔高度
        nKLineCount     = 0;
        nKLineInterval  = 0;

        // K线横坐标线数据
        kLineInfo       = [];

        // 价格区间
        nKPriceInterval = 0;

        // MA 对应的坐标数组
        ma5Points       = [];
        ma10Points      = [];
        ma20Points      = [];
        ma60Points      = [];

        // 成交量窗口
        volRectHeight   = 120;
        isShowVol       = true;

        // 成交量的最大值
        volMax          = 0;

        volMa5Points    = [];
        volMa10Points   = [];

        // 成交量横坐标数据
        volLineInfo     = [];

        nVolLineCount   = 0;
        nVolInterval    = 0;
        nVolLineInterval= 0;
        nVolMultiple    = 1;

        // 日期格
        tmNextTradeTime = null;
        nTimePeriod     = 0;
        dateRectHeight  = 20;
        dateGridPoints  = [];

        trade_time      = "";
        nStartPoint     = 2;

        // K线区域
        var kLinePanel  = document.getElementById("kLinePanel");
        var kTitle      = document.getElementById("KTitle");
        var canvaskLine = document.getElementById("canvasK");

        canvaskLine.setAttribute('width', kLinePanel.clientWidth);
        canvaskLine.setAttribute('height', kLinePanel.clientHeight - kTitle.clientHeight);

        kLineCanvas     = canvaskLine.getContext("2d");

        // Y轴区域
        YRect = 
        {
            left    : kLinePanel.clientWidth - YRectWidth, 
            right   : kLinePanel.clientWidth, 
            width   : YRectWidth, 
        };

        // K线区域 
        KRect = 
        {
            left    : kLinePanel.clientLeft, 
            right   : kLinePanel.clientLeft + kLinePanel.clientWidth - YRectWidth,  
            top     : canvaskLine.offsetTop, 
            bottom  : kLinePanel.clientHeight, 
            width   : kLinePanel.clientWidth - YRectWidth, 
            height  : kLinePanel.clientHeight - kTitle.clientHeight,
        };

        // 成交量区域
        var volPanel    = document.getElementById("volCanvas");

        var canvasVol   = document.getElementById("canvasVol");
        canvasVol.setAttribute('height', volPanel.clientHeight);
        canvasVol.setAttribute('width', volPanel.clientWidth);
        volCanvas       = canvasVol.getContext("2d");

        // 成交量区域
        volRect =
        {
            left    : volPanel.clientLeft, 
            right   : volPanel.clientLeft + volPanel.clientWidth - YRectWidth,  
            top     : volPanel.clientTop + titleHigh, 
            bottom  : volPanel.clientHeight,
            width   : volPanel.clientWidth - YRectWidth, 
            height  : volPanel.clientHeight - titleHigh, 
        }

        // 日期格区域
        var datePanel   = document.getElementById("datePanel");
        var canvasDate  = document.getElementById("canvasDate");

        canvasDate.setAttribute('height', datePanel.clientHeight);
        canvasDate.setAttribute('width', datePanel.clientWidth);

        dateRect =
        {
            left    : datePanel.clientLeft + 2, 
            right   : datePanel.clientLeft + datePanel.clientWidth - YRectWidth,  
            top     : datePanel.clientTop + 14, 
            bottom  : datePanel.clientHeight, 
            width   : datePanel.clientWidth - YRectWidth, 
            height  : datePanel.clientHeight, 
        }
        
        dateCanvas = canvasDate.getContext("2d");

        // 计算显示的K线矩形个数
        maxShowUnitCount = parseInt(KRect.width / ( rectWidth + 2 ) - 2 );
    }
    $scope.initialParam();

    // 初始化时，获取股票列表
    $scope.initialData = function()
    {
        var uriData = "op=data";
 
        CommonService.getAll("stockList", uriData, 
            function(data)
            {
                $scope.stockList  = listToObject( data, 'rows').rows;
                for ( i = 0; i < $scope.stockList.length; i++)
                    $scope.stockList[i].no = i + 1;

                if ($scope.stockList.length > 0 )
                {
                    $scope.selectStock( $scope.stockList[0]);
                }
            },
            function(data)
            {
                onErrorResult(data);
            }
        );
    }

    // 显示选择的股票K线
    $scope.selectStock = function(item)
    {
        // 获取指定的K线类型数据
        // 数据数量在最大显示数量的2倍，以MA60计算能够完全
        var uriData = "code=" + item.code + "&dt=" + $scope.dataType + "&count=" + maxShowUnitCount * 2;

        uriData += "&sd=" + startDate;

        CommonService.getAll("stockData", uriData, 
            function(data)
            {
                $scope.stockData    = listToObject( data, 'rows').rows;
                $scope.calculateMA($scope.stockData);

                $scope.currentStock = item;
                $scope.currentData  = $scope.stockData;

                $scope.draw();
            },
            function(data)
            {
                onErrorResult(data);
            }
        );
    }

    // 默认显示日线数据
    $scope.dayBtnColor  = STOCK_COLOR.btnSelectedColor;
    $scope.m1btnColor   = STOCK_COLOR.btnNonSelectedColor;
    $scope.m5BtnColor   = STOCK_COLOR.btnNonSelectedColor;
    $scope.SelectDataType = function( dataType )
    {
        $scope.dayBtnColor  = STOCK_COLOR.btnNonSelectedColor;
        $scope.m1btnColor   = STOCK_COLOR.btnNonSelectedColor;
        $scope.m5BtnColor   = STOCK_COLOR.btnNonSelectedColor;

        $scope.dataType = dataType;
        if ( dataType == "day")
            $scope.dayBtnColor  = STOCK_COLOR.btnSelectedColor;
        else if ( dataType == "m1")
            $scope.m1btnColor   = STOCK_COLOR.btnSelectedColor;
        else if ( dataType == "m5")
            $scope.m5BtnColor   = STOCK_COLOR.btnSelectedColor;

        $scope.selectStock($scope.currentStock);
    }

    // 计算MA值，并获得相应的坐标
    // 此时，数据为按时间倒叙排序，即最晚的时间在最前面
    $scope.calculateMA = function( stockData )
    {
        var nCount = stockData.length;
        for ( var i = 0; i < nCount; i++)
        {
            data = stockData[i];
            var ma = 0;
            var maVol = 0;

            // MA5
            for( var j = 0; j < 5 && nCount - i > 4; j++)
            {
                ma = ma + stockData[i + j].close;
                maVol = maVol + stockData[i + j].vol;
            }
            stockData[i].ma5    = parseInt(ma / 5);
            stockData[i].volMa5 = parseInt(maVol / 5);

            ma      = 0;
            maVol   = 0;

            // MA10
            for( var j = 0; j < 10 && nCount - i > 9; j++)
            {
                ma      = ma + stockData[i + j].close;
                maVol   = maVol + stockData[i + j].vol;
            }
            stockData[i].ma10       = parseInt(ma / 10);
            stockData[i].volMa10    = parseInt(maVol / 10);

            // MA20
            ma = 0;
            for( var j = 0; j < 20 && nCount - i > 19; j++)
                ma = ma + stockData[i + j].close;
            stockData[i].ma20 = parseInt(ma / 20);

            // MA60
            ma = 0;
            for( var j = 0; j < 60 && nCount - i > 59; j++)
                ma = ma + stockData[i + j].close;
            stockData[i].ma60 = parseInt(ma / 60);
        }
    }

    // 设置各窗口的大小
    $scope.ResizePanel = function()
    {
        // 设置各panel大小
        var fullDiv = document.getElementById("splitter");

        var fullDivHeight   = fullDiv.clientHeight;

        var kRectHeight     = (fullDivHeight > dateRectHeight) ? (fullDivHeight - dateRectHeight) : parseInt(fullDivHeight * 0.8);
        dateRectHeight      = fullDivHeight - kRectHeight;

        $('#splitter').jqxSplitter({ panels: [{ size: kRectHeight }, { size: dateRectHeight }] });

        // 设置K线窗格和成交量窗格高度
        var kLineDiv        = document.getElementById("splitter2");
        kLineDivHeight      = kLineDiv.clientHeight;
        var kLineRectHeight = (kLineDivHeight > volRectHeight) ? (kLineDivHeight - volRectHeight) : parseInt(kLineDivHeight * 0.8);
        volRectHeight       = kLineDivHeight - kLineRectHeight;

        $('#splitter2').jqxSplitter({ panels: [{ size: kLineRectHeight }, { size: volRectHeight }] });

        $scope.initialParam();

        return;
    }

    // 初始化各panel
    $scope.initialDiv = function()
    {
        $('#splitContainer').on('resize', function(event)
        {
            $scope.draw();
        });

        $('#splitter').on('resize', function(event)
        {
            $scope.draw();
        });
        $('#splitter2').on('resize', function(event)
        {
            $scope.draw();
        });

        $('#volPanel').on('resize', function(event)
        {
            $scope.draw();
        });

        $('#datePanel').on('resize', function(event)
        {
            $scope.draw();
        });

        $scope.ResizePanel();
    }
    $scope.initialDiv();

    // 绘制K线
    $scope.draw = function()
    {
        if ( $scope.currentData == undefined || $scope.currentData.length == 0 )
            return;

        $scope.initialParam();
        $scope.ResizePanel();

        var stock       = $scope.currentStock;
        var data        = $scope.currentData;

        // 计算显示的K线矩形个数
        showUnitCount    =  ( maxShowUnitCount < data.length ) ? maxShowUnitCount : data.length;

        // data 数据是从晚到早排序

        // 获取数据，记录最高值、最低值
        nHigh   = 0;
        nLow    = 10000000;
        showStockData = [];
        for( var i = 0; i < showUnitCount; i++ )
        {
            showStockData[i] = data[i];

            nHigh   =  ( nHigh < showStockData[i].high ) ? showStockData[i].high : nHigh;
            nMaHigh = nHigh;
            nMaHigh =  ( nMaHigh < showStockData[i].ma5 ) ? showStockData[i].ma5 : nMaHigh;
            nMaHigh =  ( nMaHigh < showStockData[i].ma10 ) ? showStockData[i].ma10 : nMaHigh;
            nMaHigh =  ( nMaHigh < showStockData[i].ma20 ) ? showStockData[i].ma20 : nMaHigh;
            nMaHigh =  ( nMaHigh < showStockData[i].ma60 ) ? showStockData[i].ma60 : nMaHigh;

            nLow    = ( nLow > showStockData[i].low ) ? showStockData[i].low : nLow;
            nMaLow  = nLow;
            nMaLow  = ( nMaLow > showStockData[i].ma5 ) ? showStockData[i].ma5 : nMaLow;
            nMaLow  = ( nMaLow > showStockData[i].ma10 ) ? showStockData[i].ma10 : nMaLow;
            nMaLow  = ( nMaLow > showStockData[i].ma20 ) ? showStockData[i].ma20 : nMaLow;
            nMaLow  = ( nMaLow > showStockData[i].ma60 ) ? showStockData[i].ma60 : nMaLow;

            volMax  = ( volMax < showStockData[i].vol ) ? showStockData[i].vol : volMax;
        }

        // 绘制K线
        $scope.drawKLine(stock, showStockData);

        // 绘制成交量K线
        $scope.drawVol(stock, showStockData);

        // 绘制日期格
        $scope.drawDate(stock, showStockData);

        // 绘制Y轴
        $scope.drawYCoodinate();

    }

    $scope.drawKLine = function(stock, showStockData)
    {
        // K线区域
        $scope.stock_name = stock.name;
        $scope.stock_code = stock.code;

        $scope.ma5  = "MA5:"  + (showStockData[0].ma5 / 100).toFixed(2).toString();
        $scope.ma10 = "MA10:" + (showStockData[0].ma10 / 100).toFixed(2).toString();
        $scope.ma20 = "MA20:" + (showStockData[0].ma20 / 100).toFixed(2).toString();
        $scope.ma60 = "MA60:" + (showStockData[0].ma60 / 100).toFixed(2).toString();

        // 根据最高值、最低值计算坐标线数量、每2条坐标线的价格区间
        var nListCount = priceIntervalList.length;
        
        var bFlag = true;
        while ( bFlag == true )
        {
            var temp = parseInt(( nMaHigh - nMaLow ) / maxGridHeight);
            for ( var i = nListCount - 1; i >= 0; i--) 
            {
                if (  temp <= priceIntervalList[i] )
                    nKPriceInterval = priceIntervalList[i];
            }

            // 横轴数量
            nMax = parseInt(( nHigh / nKPriceInterval + 1 )) * nKPriceInterval ;
            nMin = parseInt(( nLow / nKPriceInterval -  1 )) * nKPriceInterval;

            // 横轴坐标轴数量，以及坐标轴间隔
            nKLineCount = parseInt(( nMax - nMin ) / nKPriceInterval);
            nKLineInterval = parseInt( KRect.height / ( nKLineCount + 1 ));
            if ( nKLineInterval < minGridHeight )
                maxGridHeight = parseInt( maxGridHeight * 0.75);
            else
                bFlag = false;
        }

        $scope.DrawKCoordinate();

        // 矩形的序号，用于计算起始坐标
        var nIndex  = 0;
        nStartPoint = 2;

        // 最大值、最小值位置
        var tag     = {};

        // 获取日期格位置
        tmNextTradeTime = $scope.getNextDate(showStockData);

        // 绘制K线矩形图，从最早到最晚
        for ( var i = showStockData.length - 1; i >= 0; i-- )
        {
            $scope.drawKRect(showStockData[i], nIndex, tag);
            nIndex ++;
        }

        // 标注最高价、最低价
        kLineCanvas.beginPath();
        kLineCanvas.fillStyle = STOCK_COLOR.TagColor;
        kLineCanvas.font = "14px 宋体";
        kLineCanvas.fillText( tag.highTag, tag.rectHigh.left, tag.rectHigh.top);
        kLineCanvas.fillText( tag.lowTag, tag.rectLow.left, tag.rectLow.top);
        
        $scope.DrawArrow( tag.highStartX, tag.highStartY, tag.highEndX, tag.highEndY);
        $scope.DrawArrow( tag.lowStartX, tag.lowStartY, tag.lowEndX, tag.lowEndY);
        kLineCanvas.stroke();
        kLineCanvas.closePath();

        // 绘制MA曲线
        $scope.DrawMa(kLineCanvas, ma5Points, STOCK_COLOR.MA5Color);
        $scope.DrawMa(kLineCanvas, ma10Points, STOCK_COLOR.MA10Color);
        $scope.DrawMa(kLineCanvas, ma20Points, STOCK_COLOR.MA20Color);
        $scope.DrawMa(kLineCanvas, ma60Points, STOCK_COLOR.MA60Color);
        
    }
    
    // 获取日期格的第一个时间
    $scope.getNextDate = function(showStockData)
    {
        // 第一格为固定宽度，
        // 日线：最小单位为月，然后是2个月、3个月、4个月、5个月、6个月；
        // 1分钟线：最小单位为10分钟，15分钟、20分钟、30分钟、45分钟、60分钟、120分钟

        nTimePeriod = 0;
        var nGridWidth = nDateGridWidth[emKLineType];
        var nGridCount = parseInt(dateRect.width / nGridWidth);

        if ( emKLineType >= STOCK_K_LINE_TYPE.SDT_DAY )
        {
            
            var firstTradeTime = formatDate( showStockData[showStockData.length-1].trade_time, "yyyy-MM-dd" );
            var lastTradeTime = formatDate( showStockData[0].trade_time, "yyyy-MM-dd" );

            // 第一个交易记录的日期
            dateGridPoints.push( { left: KRect.left, trade_time: firstTradeTime });
            
            var dateSpan = Math.abs(lastTradeTime - firstTradeTime);
            var nDays    = Math.floor(dateSpan / (24 * 3600 * 1000));
            var nAvgDays = parseInt(nDays / nGridCount);

            var nCount = nDayTimeSpan.length;
            nTimePeriod = nDayTimeSpan[0];
            for( var i = 0; i < nCount; i++)
            {
                if ( nAvgDays >= nDayTimeSpan[i] * 30 )
                    nTimePeriod = nDayTimeSpan[i];
            }

            // 计算下一日期格时间
            var nYear = firstTradeTime.getFullYear();
            var nMonth = firstTradeTime.getMonth() + nTimePeriod;
            if ( nMonth > 12 )
            {
                nYear ++;
                nMonth = nMonth % 12;
            }

            tmNextTradeTime = new Date( nYear, nMonth, 1);
        }
        // else if (  emKLineType <= STOCK_K_LINE_TYPE.SDT_MINUTE_60 )
        // {
        //     stFirstTime = pLastData->m_Time.Format(_T("%m月%d日"));
        //     nFirstGrid = pDC->GetTextExtent(stFirstTime).cx + 4;

        //     // 分钟线
        //     CTimeSpan span = pFirstData->m_Time - pLastData->m_Time;
            
        //     // 相差的分钟数
        //     int nDiffMinute= CStock::GetMinuteSpan(pFirstData->m_Time, pLastData->m_Time);
        //     int nAvgUnit = nDiffMinute / nGridCount;

        //     int nCount = sizeof(nMinuteTimeSpan)/sizeof(nMinuteTimeSpan[0]);
        //     m_nTimePeriod = nMinuteTimeSpan[0];
        //     for( int i = 0; i < nCount; i++)
        //     {
        //         if ( nAvgUnit >= nMinuteTimeSpan[i] )
        //             m_nTimePeriod = nMinuteTimeSpan[i];
        //     }

        //     // 计算下一日期格时间
        //     int nYear = pLastData->m_nYear;
        //     int nMonth = pLastData->m_nMonth;
        //     int nDay = pLastData->m_nDay;
        //     int nHour = pLastData->m_nHour;
        //     int nMinute = pLastData->m_nMinute + m_nTimePeriod;
        //     if ( nMinute >= 60 )
        //     {
        //         nHour ++;
        //         nMinute = nMinute % 60;
        //     }

        //     if ( nHour == 12 )
        //     {
        //         nHour = 13;
        //         nMinute = 0;
        //     }
        //     else if ( nHour >= 15 )
        //     {
        //         nDay ++;
        //         nHour = 9;
        //         nMinute = 30;
        //         CTimeSpan span(1, 0, 0, 0);
        //         CTime tm = pLastData->m_Time + span;
        //         nYear = tm.GetYear();
        //         nMonth = tm.GetMonth();
        //         nDay = tm.GetDay();
        //     }

        //     CTime tm( nYear, nMonth , nDay, nHour, nMinute, 0);
        //     m_tmNextTime = tm;
        // }

        return tmNextTradeTime;
    }

    // /////////////////////////////////////////////
    // 绘制K线图坐标轴
    $scope.DrawKCoordinate = function()
    {
        kLineCanvas.beginPath();

        kLineCanvas.lineWidth       = 1;
        kLineCanvas.strokeStyle     = "#F5270B";

        kLineCanvas.setLineDash([1, 5]);

        // 坐标轴第一条线为绘图区域的最底端，不需要绘制
        kLineInfo[0] = { y : KRect.height, price : nMin };
        for ( var i = 1; i <= nKLineCount; i++)
        {
            var y = KRect.height - i * nKLineInterval;
            kLineCanvas.moveTo(0, y);
            kLineCanvas.lineTo(KRect.right, y );

            // 记录横轴坐标及对应的价格
            kLineInfo[i] = { y: y, price : nMin + i * nKPriceInterval };
        }

        kLineCanvas.stroke();
        kLineCanvas.closePath();
    }

    // 日期分隔虚线，只有1分钟、5分钟K线需要
    // 上一根K线的日期
    var trade_time = "";
    // 缺省的起始点
    var nStartPoint = 2;

    // 单个K线矩形
    $scope.drawKRect = function( stockData, nIndex, tag)
    {
        kLineCanvas.beginPath();

        kLineCanvas.setLineDash([]);

        var nUnitHigh  = stockData.high;
        var nUnitLow   = stockData.low;
        var nUnitOpen  = stockData.open;
        var nUnitClose = stockData.close;

        // // 价格对应的Y轴坐标
        var high    = $scope.getPriceHeight( nUnitHigh );
        var top     = $scope.getPriceHeight( nUnitOpen) ;
        var bottom  = $scope.getPriceHeight( nUnitClose );
        var low     = $scope.getPriceHeight( nUnitLow );

        // 矩形的左上角：
        var left    = nStartPoint;
        var right   = left + rectWidth;
        var center  = left + parseInt(rectWidth / 2 );
        nStartPoint = right + parseInt(rectWidth / 2);

        // 获得MA值坐标
        var yPoint = $scope.getPriceHeight( stockData.ma5 );
        if ( yPoint > 0 && yPoint <= KRect.height )
            ma5Points.push( { x: left, y : yPoint} );

        yPoint = $scope.getPriceHeight( stockData.ma10 );
        if ( yPoint > 0 && yPoint <= KRect.height )
            ma10Points.push( { x: left, y : yPoint} );

        yPoint = $scope.getPriceHeight( stockData.ma20 );
        if ( yPoint > 0 && yPoint <= KRect.height )
            ma20Points.push( { x: left, y : yPoint} );

        yPoint = $scope.getPriceHeight( stockData.ma60 );
        if ( yPoint > 0 && yPoint <= KRect.height )
            ma60Points.push( { x: left, y : yPoint} );

        if ( top > bottom )
        {
            // 阳线时，需要交换top和bottom
            var tmp = top;
            top     = bottom;
            bottom  = tmp;
        }

        var width   = rectWidth;
        var height  = bottom - top;

        var rect = {left:left, top:top, right:right, bottom:bottom};
        stockData.m_rectK = rect;

        if ( stockData.trade_time != trade_time )
        {
            trade_time = stockData.trade_time;
        
            // 日期发生变化，并且K线类型为分钟、5分钟，画出日期虚线
            if ( emKLineType <= STOCK_K_LINE_TYPE.SDT_MINUTE_5 )
            {
                // CPen pen( PS_DOT, 1, g_SystemParams.m_rgbKLineColor );
                // kLineCanvas.SelectObject(&pen);

                // for( int y = m_rectK.top; y < m_rectK.bottom; y+=4 )
                // {
                //     kLineCanvas.MoveTo( left, y );
                //     kLineCanvas.LineTo( left, y + 2);
                //     y += 4;
                // }
            }
        }

        if ( nUnitOpen < nUnitClose )
        {
            // 阳线, 外实线，内部空
            kLineCanvas.strokeStyle = STOCK_COLOR.PositiveLineColor;
            kLineCanvas.fillStyle = STOCK_COLOR.KLineBackColor;
            
            kLineCanvas.strokeRect(left, top, width, height);
            kLineCanvas.fillRect(left, top, width, height);

            // 上下引线
            kLineCanvas.moveTo( center, high );
            kLineCanvas.lineTo( center, top);
            kLineCanvas.moveTo( center, bottom);
            kLineCanvas.lineTo( center, low);
        }
        else if ( nUnitOpen > nUnitClose )
        {
            // 阴线
            kLineCanvas.strokeStyle  = STOCK_COLOR.NegitiveLineColor;
            kLineCanvas.fillStyle  = STOCK_COLOR.NegitiveLineColor;
            kLineCanvas.fillRect(left, top, width, height);
            kLineCanvas.strokeRect(left, top, width, height);

            kLineCanvas.moveTo( center, high);
            kLineCanvas.lineTo( center, low );
        }
        else
        {
            // 十字星
            kLineCanvas.strokeStyle = STOCK_COLOR.CrossLineColor;

            kLineCanvas.moveTo(right, top);
            kLineCanvas.lineTo( left, top);
            kLineCanvas.lineTo(right, top);

            var center = parseInt(left + rectWidth / 2);
            kLineCanvas.moveTo( center, high );
            kLineCanvas.lineTo( center, low );
        }

        // 标注最大值、最小值
        // 设置最大值、最小值颜色
        var fontSize = 14;

        if ( nUnitHigh == nHigh )
        {
            var temp = (nHigh / 100).toFixed(2).toString();

            var nArrayX = 0;
            var rectHigh = {};

            // 判断最高点的位置
            if ( left + 20 + fontSize + 6 > KRect.right )
            {
                rectHigh.left = left - fontSize - 20 - 6;
                rectHigh.right = rectHigh.left + fontSize + 4;
                nArrayX = rectHigh.right;
            }
            else
            {
                rectHigh.left = left + 20;
                rectHigh.right = rectHigh.left + fontSize + 4;
                nArrayX = rectHigh.left - 2;
            }
            
            if ( high - fontSize - 6 < KRect.top )
                rectHigh.top = KRect.top + 4;
            else
                rectHigh.top = high - fontSize - 6;

            rectHigh.bottom = rectHigh.top + fontSize + 4;

            tag.rectHigh = rectHigh;
            tag.highTag = temp;
            tag.highStartX = nArrayX;
            tag.highStartY = parseInt((rectHigh.bottom - rectHigh.top ) / 2) + rectHigh.top;
            tag.highEndX = center;
            tag.highEndY = high;
        }

        if ( nUnitLow == nLow )
        {
            var temp = (nLow / 100).toFixed(2).toString();
           
            var rectLow = {};
            var nArrayX = 0;

            // 判断位置
            if ( left + 20 + fontSize + 4 > KRect.right )
            {
                rectLow.left  = left - fontSize - 20 - 4;
                rectLow.right = rectLow.left + fontSize + 4;
                nArrayX       = rectLow.right;
            }
            else
            {
                rectLow.left  = left + 20;
                rectLow.right = rectLow.left + fontSize + 4;
                nArrayX       = rectLow.left - 2;
            }

            if ( low + 10 + fontSize + 6 > KRect.bottom)
                rectLow.top = KRect.bottom - fontSize - 16;
            else
                rectLow.top = low + 10 + fontSize + 6;

            rectLow.bottom  = rectLow.top + fontSize + 4;

            tag.rectLow     = rectLow;
            tag.lowTag      = temp;
            tag.lowStartX   = nArrayX;
            tag.lowStartY   = rectLow.top - parseInt((rectLow.bottom - rectLow.top ) / 2);
            tag.lowEndX     = center;
            tag.lowEndY     = low;
        }

        if ( emKLineType >= STOCK_K_LINE_TYPE.SDT_DAY )
        {
            // 日线，比较日期
            var tempTradeTime = formatDate( stockData.trade_time, "yyyy-MM-dd" );
            if ( tempTradeTime > tmNextTradeTime )
            {
                // 找到第一个K线， 计算下一日期格时间
                var nYear   = tempTradeTime.getFullYear();
                var nMonth  = tempTradeTime.getMonth() + nTimePeriod;
                if ( nMonth > 12 )
                {
                    // 比较K线坐标是否覆盖上一个日期格
                    nYear ++;
                    nMonth = nMonth % 12;
                }

                tmNextTradeTime = new Date( nYear, nMonth , 1, 0, 0, 0);

                // 记录日期格位置
                dateGridPoints.push( { left: left, trade_time: tempTradeTime});

            }
        }
        // else if (  emKLineType <= STOCK_K_LINE_TYPE.SDT_MINUTE_60 )
        // {
        //     if ( pData->m_Time > m_tmNextTime )
        //     {
        //         // 找到第一个K线， 计算下一日期格时间
        //         int nYear = pData->m_nYear;
        //         int nMonth = pData->m_nMonth;
        //         int nDay = pData->m_nDay;
        //         int nHour = pData->m_nHour;
        //         int nMinute = pData->m_nMinute + m_nTimePeriod;
        //         if ( nMinute >= 60 )
        //         {
        //             nHour ++;
        //             nMinute = nMinute % 60;
        //         }

        //         if ( nHour == 12 )
        //         {
        //             nHour = 13;
        //             nMinute = 0;
        //         }
        //         else if ( nHour >= 15 )
        //         {
        //             nDay ++;
        //             nHour = 9;
        //             nMinute = 30;
        //             CTimeSpan span(1, 0, 0, 0);
        //             CTime tm = pData->m_Time + span;
        //             nYear = tm.GetYear();
        //             nMonth = tm.GetMonth();
        //             nDay = tm.GetDay();
        //         }

        //         CTime tm( nYear, nMonth , nDay, nHour, nMinute, 0);
        //         m_tmNextTime = tm;

        //         if ( KRect.left > m_rectLastDate.right )
        //         {
        //             // 未覆盖
        //             CString stTime;
        //             stTime.Format(_T("%02d:%02d"), pData->m_nHour, pData->m_nMinute);
        //             m_rectLastDate.left  = KRect.left + 4;
        //             m_rectLastDate.right = m_rectLastDate.left + pDC->GetTextExtent(stTime).cx + 4;

        //             if ( m_rectLastDate.right <= m_rectY.left )
        //             {
        //                 pDC->SetTextColor( g_SystemParams.m_rgbPositiveLineColor );
        //                 pDC->SetBkColor(g_SystemParams.m_rgbKLineBackColor);
        //                 pDC->DrawText( stTime, &m_rectLastDate, DT_LEFT );

        //                 CPen datePen( PS_SOLID, 1, g_SystemParams.m_rgbPositiveLineColor );
        //                 pDC->SelectObject(&datePen);
        //                 pDC->MoveTo(m_rectLastDate.left - 4, m_rectLastDate.top - 4 );
        //                 pDC->LineTo( m_rectLastDate.left - 4, m_rectLastDate.bottom + 4);
        //             }
        //         }
        //     }

        // }

        kLineCanvas.stroke();
        kLineCanvas.closePath();
        return tag;
    }

    // 标注的箭头
    $scope.DrawArrow = function( nStartX,  nStartY, nEndX, nEndY)
    {
        var angel   = 45 * Math.PI / 180;                               // 箭头与直线夹角
        var rate    = 0.4;                                              // 箭头边长度占直线长度的百分比
        var arrowPoint = {};

        kLineCanvas.strokeStyle = STOCK_COLOR.TagColor;

        kLineCanvas.moveTo( nStartX, nStartY );
        kLineCanvas.lineTo( nEndX, nEndY );

        x = parseInt( rate * ((nStartX - nEndX ) * Math.cos(angel) - ( nStartY - nEndY) * Math.sin(angel)) + nEndX);
        y = parseInt( rate * ((nStartX - nEndX ) * Math.sin(angel) + ( nStartY - nEndY) * Math.cos(angel)) + nEndY);

        kLineCanvas.moveTo(nEndX, nEndY);
        kLineCanvas.lineTo(x, y);
        x = parseInt( rate * (( nStartX - nEndX) * Math.cos( 0 - angel) - ( nStartY - nEndY) * Math.sin( 0 - angel )) + nEndX);
        y = parseInt( rate * (( nStartX - nEndX) * Math.sin( 0 - angel) + ( nStartY - nEndY) * Math.cos( 0 - angel )) + nEndY);
        kLineCanvas.moveTo(nEndX, nEndY);
        kLineCanvas.lineTo(x, y);

        return;
    }

    // MA曲线
    $scope.DrawMa = function( canvas, points, color )
    {
        canvas.strokeStyle  = color;
        canvas.lineWidth    = 1;
        canvas.beginPath();

        canvas.moveTo(points[0].x, points[0].y);

        for( var i = 1; i < points.length; i++)
        {
            if ( points[i].y == 0 )
                continue;

            var ctrlP = $scope.getCtrlPoint(points, i - 1 );
            canvas.bezierCurveTo(ctrlP.pA.x, ctrlP.pA.y, ctrlP.pB.x, ctrlP.pB.y, points[i].x, points[i].y);
        }

        canvas.stroke();
        canvas.closePath();
    }

    // 获取贝塞尔曲线控制点
    $scope.getCtrlPoint = function(ps, i, a, b)
    {
        if( !a || !b)
        {
            a = 0.2;
            b = 0.2;
        }

        //处理两种极端情形
        if( i < 1 )
        {
            var pAx = ps[0].x + (ps[1].x-ps[0].x)*a;
            var pAy = ps[0].y + (ps[1].y-ps[0].y)*a;
        }
        else
        {
            var pAx = ps[i].x + (ps[i+1].x-ps[i-1].x)*a;
            var pAy = ps[i].y + (ps[i+1].y-ps[i-1].y)*a;
        }
        if( i > ps.length - 3)
        {
            var last=ps.length - 1;
            var pBx = ps[last].x - (ps[last].x-ps[last-1].x)*b;
            var pBy = ps[last].y - (ps[last].y-ps[last-1].y)*b;
        }
        else
        {
            var pBx = ps[i+1].x - (ps[i+2].x-ps[i].x)*b;
            var pBy = ps[i+1].y - (ps[i+2].y-ps[i].y)*b;
        }
        
        return { pA:{x:pAx,y:pAy}, pB:{x:pBx,y:pBy} };
    }

    // 根据价格返回对应的竖轴高度
    $scope.getPriceHeight = function( nPrice )
    {
        for ( var i = 0; i < nKLineCount; i++)
        {
            if ( nPrice >= kLineInfo[i].price && kLineInfo[i].price >= 0 )
            {
                var a = ( nPrice - kLineInfo[i].price ) / nKPriceInterval;
                var y = parseInt(kLineInfo[i].y - a * nKLineInterval);
                return y;
            }
        }

        // 不应该执行到这来来

        return 0;
    }

    // 绘制成交量K线
    $scope.drawVol = function(stock, showStockData)
    {
        $scope.volume   = "VOLUME: " + (showStockData[0].vol / 100).toFixed(2).toString();
        $scope.volMa5   = "MA5:"   + (showStockData[0].volMa5 / 100).toFixed(2).toString();
        $scope.volMa10  = "MA10:" + (showStockData[0].volMa10 / 100).toFixed(2).toString();

        $scope.drawVolCoordinate();

        // 矩形的序号，用于计算起始坐标
        var nIndex      = 0;
        nStartPoint     = 2;

        // 设置标题

        // volCanvas.strokeStyle = "yellow";
        volCanvas.fillStyle   = "yellow";
        volCanvas.font        = "14px 宋体";

        volCanvas.beginPath();
        
        // 设置交易量数据
        volCanvas.fillText( $scope.volume, 2, 16);

        // 设置MA5数据
        var left = volCanvas.measureText($scope.volume).width + 10;
        volCanvas.fillText( $scope.volMa5, left, 16);

        // 设置Ma10
        left = left + volCanvas.measureText($scope.volMa5).width + 10;

        volCanvas.fillStyle   = "Magenta";
        volCanvas.fillText( $scope.volMa10, left, 16);

        volCanvas.stroke();
        volCanvas.closePath();   

        // 绘制K线矩形图，从最早到最晚
        for ( var i = showStockData.length - 1; i >= 0; i-- )
        {
            $scope.drawVolRect(showStockData[i], nIndex);
            nIndex ++;
        }

        // 绘制MA曲线
        $scope.DrawMa( volCanvas, volMa5Points, STOCK_COLOR.MA5Color);
        $scope.DrawMa( volCanvas, volMa10Points, STOCK_COLOR.MA10Color);
    }

    // /////////////////////////////////////////////
    // 绘制成交量坐标轴
    $scope.drawVolCoordinate = function()
    {
        // 成交量单位为股，转为手
        var nMax        = Math.ceil( volMax / 100 );

        // 成交量值可能会很大，需要缩小比例，缩小倍数
        nVolMultiple    = 1;

        nVolInterval    = 0;
        var nListCount  = volIntervalList.length;

        // 暂定VOL横坐标线数量，实际数量根据计算获得
        var nVolLine    = 3;
        while ( nVolInterval == 0 )
        {
            for ( var i = nListCount - 1; i >= 0; i--) 
            {
                if ( Math.ceil( nMax / nVolLine ) <= volIntervalList[i] )
                    nVolInterval = volIntervalList[i];
            }

            if ( nVolInterval == 0 )
            {
                nVolMultiple = nVolMultiple * 100;
                nMax = Math.ceil( nMax / nVolMultiple );
            }
        }

        // 横轴数量
        nMax = Math.ceil( volMax / 100 / nVolInterval / nVolMultiple + 1 ) * nVolInterval * nVolMultiple ;

        nVolLineCount           = Math.ceil(nMax / (nVolInterval * nVolMultiple));
        nVolLineInterval        = Math.ceil(volRect.height / nVolLineCount);

        volCanvas.lineWidth     = 1;
        volCanvas.strokeStyle   = "#F5270B";
        volCanvas.setLineDash([1, 5]);
        volCanvas.beginPath();

        volLineInfo[0] = { y : volRect.bottom, vol: 0};
        for ( var i = 1; i < nVolLineCount; i++)
        {
            var y = volRect.bottom - i * nVolLineInterval;
            volCanvas.moveTo( volRect.left, y);
            volCanvas.lineTo( volRect.width, y );

            // 记录横轴坐标及对应的成交量
            volLineInfo[i] = { y: y, vol : i * nVolInterval };
        }

        volCanvas.stroke();
    }

    // 单个K线矩形
    $scope.drawVolRect = function( stockData, nIndex )
    {
        volCanvas.setLineDash([]);
        volCanvas.beginPath();

        var nUnitHigh  = stockData.vol;
        
        // // 价格对应的Y轴坐标
        var top     = $scope.getVolHeight( stockData.vol );

        // 矩形的左上角：
        var left    = nStartPoint;
        var right   = left + rectWidth;
        var center  = left + parseInt(rectWidth / 2 );
        nStartPoint = right + parseInt(rectWidth / 2);

        var width   = rectWidth;
        var height  = volRect.bottom - top;

        // 获得MA值坐标
        volMa5Points.push( { x: left, y : $scope.getVolHeight( stockData.volMa5 ) });
        volMa10Points.push( { x: left, y : $scope.getVolHeight( stockData.volMa10 ) });

        if ( stockData.trade_time != trade_time )
        {
            trade_time = stockData.trade_time;
        
            // 日期发生变化，并且K线类型为分钟、5分钟，画出日期虚线
            if ( emKLineType <= STOCK_K_LINE_TYPE.SDT_MINUTE_5 )
            {
                // CPen pen( PS_DOT, 1, g_SystemParams.m_rgbKLineColor );
                // kLineCanvas.SelectObject(&pen);

                // for( int y = m_rectK.top; y < m_rectK.bottom; y+=4 )
                // {
                //     kLineCanvas.MoveTo( left, y );
                //     kLineCanvas.LineTo( left, y + 2);
                //     y += 4;
                // }
            }
        }

        if ( stockData.open < stockData.close )
        {
            // 阳线, 外实线，内部空
            volCanvas.strokeStyle   = STOCK_COLOR.PositiveLineColor;
            volCanvas.fillStyle     = STOCK_COLOR.KLineBackColor;
            
            volCanvas.strokeRect(left, top, width, height);
            volCanvas.fillRect(left, top, width, height);
        }
        else
        {
            // 阴线
            volCanvas.strokeStyle   = STOCK_COLOR.NegitiveLineColor;
            volCanvas.fillStyle     = STOCK_COLOR.NegitiveLineColor;

            volCanvas.fillRect(left, top, width, height);
            volCanvas.strokeRect(left, top, width, height);
        }

        volCanvas.stroke();
        volCanvas.closePath();

    }

    // 根据成交量返回对应的竖轴高度
    $scope.getVolHeight = function( volume )
    {
        // 转换成交量
        var vol = parseInt(volume / ( 100 * nVolMultiple ) );
        var y = 0;
        for ( var i = nVolLineCount - 1 ; i > 0; i--)
        {
            if ( vol < volLineInfo[i].vol )
            {
                var a = ( volLineInfo[i].vol - vol ) / nVolInterval;
                y = parseInt(volLineInfo[i].y - a * nVolLineInterval);
            }
        }

        // if ( y < volRect.top || y == 0)
        // {
        //     var a = 1;
        // }

        return y;
    }

    // 绘制日期窗格
    $scope.drawDate = function( stock, showStockData)
    {
        if (dateGridPoints.length == 0 )
            return;

        var firstTradeTime = dateGridPoints[0].trade_time;
        var title = "" + firstTradeTime.getFullYear() + "年";

        dateCanvas.lineWidth = 1;
        dateCanvas.strokeStyle = "#F5270B";

        dateCanvas.setLineDash([]);
        dateCanvas.beginPath();
        dateCanvas.fillStyle = STOCK_COLOR.dateGridColor;
        dateCanvas.font = "14px 宋体";
        var left = dateRect.left;
        var top = dateRect.top;
        var nextPos = dateCanvas.measureText(title).width + left + 2;

        dateCanvas.fillText( title, left, top);

        for( var i = 1; i < dateGridPoints.length; i++)
        {
            left = dateGridPoints[i].left;
            if ( left < nextPos )
                continue;

            dateCanvas.lineWidth = 1;
            dateCanvas.moveTo( left, 0);//dateRect.top);
            dateCanvas.lineTo( left, dateRect.bottom );
            dateCanvas.stroke();

            title = "" + ( dateGridPoints[i].trade_time.getMonth() + 1);
            dateCanvas.fillText( title, left + 4, top);
            nextPos = left + 4 + dateCanvas.measureText(title).width + 2;
        }

        dateCanvas.stroke();

    }

    // 绘制Y轴
    $scope.drawYCoodinate = function()
    {
        var left    = YRect.left;
        var right   = YRect.right;
        var width   = 10;

        kLineCanvas.setLineDash([]);
        kLineCanvas.beginPath();

        kLineCanvas.lineWidth   = 0.08;
        kLineCanvas.strokeStyle = "#F5270B";
        kLineCanvas.fillStyle   = STOCK_COLOR.KLineCoordinateTextColor;
        kLineCanvas.font        = "14px 宋体";

        kLineCanvas.moveTo(left, kLineInfo[0].y);
        kLineCanvas.lineTo(left, 0);

        // K线图纵坐标
        for ( var i = 1; i <= nKLineCount; i++)
        {
            kLineCanvas.moveTo(left, kLineInfo[i].y);
            kLineCanvas.lineTo(left+width, kLineInfo[i].y);

            var p = kLineInfo[i].price / 100;
            var str = p.toFixed(2);

            kLineCanvas.fillText( str, left + width + 5, kLineInfo[i].y);
            
            kLineCanvas.stroke();
        }

        // volPanel纵坐标
        volCanvas.setLineDash([]);
        volCanvas.beginPath();

        volCanvas.lineWidth     = 0.8;
        volCanvas.strokeStyle   = "#F5270B";
        volCanvas.fillStyle     = STOCK_COLOR.KLineCoordinateTextColor;
        volCanvas.font          = "14px 宋体";

        volCanvas.moveTo(left, volLineInfo[0].y);
        volCanvas.lineTo(left, 0);

        for ( var i = 1; i < nVolLineCount; i++)
        {
            volCanvas.moveTo(left, volLineInfo[i].y);
            volCanvas.lineTo(left + width, volLineInfo[i].y);

            var str = volLineInfo[i].vol.toString();
            volCanvas.fillText( str, left + width + 5, volLineInfo[i].y);            
        }

        // 设置倍数标签
        volCanvas.fillText( nVolMultiple.toString(), left + 5, volRect.bottom - 5);

        volCanvas.stroke();
        volCanvas.closePath();
    }

})

