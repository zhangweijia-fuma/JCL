
var projName = "rsu";
var SYSTEM = ['PIS', 'IOT', 'iMRO', 'oa', 'stock', 'rsu'];
var webSvcPort = ("https:" == document.location.protocol) ? 446 : 92;
var _baseUrl = location.protocol + "//" + location.hostname;

var projConfigs = {
    "PIS" : {
        PIS            	: false,
        projName        : "PIS",
        projNameZh      : "JamesSoft JDS管理系统",
        ownerName       : "成都轨道交通有限公司",
        ownerLogoURL    : "resource/images/JamesSoft.png",
        ownerLoginURL   : "resource/images/JamesSoft.png",
        ownerLogoPrint  : "resource/images/JamesSoft.png",
        background      : "resource/images/background.png",
        navLeftWidth    : "30%",
        navRightWidth   : "70%",
        navPicWidth     : "120px",
        pageSize        : 20,
        // 线路运营模式，false为单线路模式，true为多线路模式
        lineMode        : false,
        lineName        : "成都地铁2号线",
    },

    "IOT" : {
        IOT             : true,
        projName       	: "IOT",
        projNameZh      : "JamesSoft JDS管理系统",
        ownerName       : "JamesSoft",
        ownerLogoURL    : "resource/images/JamesSoft.png",
        ownerLoginURL   : "resource/images/JamesSoft.png",
        ownerLogoPrint  : "resource/images/JamesSoft.png",
        background      : "resource/images/background.png",
        navLeftWidth    : "35%",
        navRightWidth   : "65%",
        navPicWidth     : "120px",
        pageSize        : 20,
    },

    "iMRO" : {
        iMRO             : true,
        projName         : "JDS-iMRO Management System",
        projNameZh       : "JamesSoft JDS智能检修业务管理系统",
        ownerName        : "JamesSoft",
        ownerLogoURL     : "resource/images/JamesSoft.png",
        ownerLoginURL    : "resource/images/JamesSoft.png",
        ownerLogoPrint   : "resource/images/JamesSoft.png",
        background       : "resource/images/background.png",
        navLeftWidth     : "35%",
        navRightWidth    : "65%",
        navPicWidth      : "120px",
        pageSize         : 20,
    },

    "oa" : {
        oa               : true,
        projName         : "JamesSoft JDS OA Management System",
        projNameZh       : "四川凯普顿信息化管理系统",
        ownerName        : "四川凯普顿信息技术股份有限公司",
        ownerLogoURL     : "resource/images/JamesSoft.png",
        ownerLoginURL    : "resource/images/JamesSoft.png",
        ownerLogoPrint   : "resource/images/JamesSoft.png",
        background       : "resource/images/CPTBackground.png",
        navLeftWidth     : "35%",
        navRightWidth    : "65%",
        navPicWidth      : "120px",
        pageSize         : 20,
    },

    "stock" : {
        stock            : true,
        projName         : "JamesSoft PearMarket Stock Information System",
        projNameZh       : "JamesSoft PearMarket 股票信息系统",
        ownerName        : "JamesSoft PearMarket Stock Information System",
        ownerLogoURL     : "resource/images/JamesSoft.png",
        ownerLoginURL    : "resource/images/PearMarketSystem.png",
        ownerLogoPrint   : "resource/images/JamesSoft.png",
        background       : "resource/images/CPTBackground.png",
        navLeftWidth     : "35%",
        navRightWidth    : "65%",
        navPicWidth      : "120px",
        pageSize         : 20,
    },

    "rsu" : {
        rsu            : true,
        projName         : "Fuma AI RSU Management System",
        projNameZh       : "福马智行 RSU 管理信息系统",
        ownerName        : "Fuma AI",
        ownerLogoURL     : "resource/images/logo/FumaLogo.png",
        ownerLoginURL    : "resource/images/logo/FumaLogo.png",
        ownerLogoPrint   : "resource/images/logo/FumaLogo.png",
        background       : "resource/images/background.png",
        navLeftWidth     : "35%",
        navRightWidth    : "65%",
        navPicWidth      : "80px",
        pageSize         : 20,
    },

    "system" : {
        system            : true,
        projName         : "JamesSoft JDS System",
        projNameZh       : "JamesSoft JDS管理系统",
        ownerName        : "JamesSoft Inc.",
        ownerLogoURL     : "resource/images/JamesSoft.png",
        ownerLoginURL    : "resource/images/JamesSoft.png",
        ownerLogoPrint   : "resource/images/JamesSoft.png",
        background       : "resource/images/CPTBackground.png",
        navLeftWidth     : "35%",
        navRightWidth    : "65%",
        navPicWidth      : "120px",
        pageSize         : 20,
    },
};

// js代码通过projConfig获取项目相关的配置项
// 也可以在代码判断当前为何项目
var projConfig = projConfigs[projName];

// js代码通过appConfig获取应用相关的通用配置
var appConfig = 
{
    appKey : "8uhnIKOLpl",
    appSec : "09f7c8cba635f7616bc131b0d8e25947s",
    webSvcUrl : _baseUrl + ":" + webSvcPort + "/jds/",
    projConfig : projConfig,
    system : SYSTEM,
    sysMap : (function () 
    {
        var maps = {};
        for (var i = 0; i < SYSTEM.length; i++) 
        {
            maps[SYSTEM[i]] = true;
        }
        return maps;
    })()
};

var getBaseUrl = function ()
{
    return appConfig.webSvcUrl;
};

// 文件服务器地址
window.uploadURL = location.origin + "/upload";

// 头像上传地址
window.avatarURL = appConfig.webSvcUrl + "avatarUpload";

// 在页面最底部显示的系统供应商名称
var vendorName = "福马智行";

