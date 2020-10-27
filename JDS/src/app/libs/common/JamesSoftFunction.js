

function ClearAreaInfo(dataType)
{
    sessionStorage.removeItem(dataType + 'country');
    sessionStorage.removeItem(dataType + 'province');
    sessionStorage.removeItem(dataType + 'city');
    sessionStorage.removeItem(dataType + 'district');
}

function SetAreaInfo(dataType, addr)
{
    if ( addr == null)
        return;

    sessionStorage.setItem(dataType + 'country',    JSON.stringify(addr.country));
    sessionStorage.setItem(dataType + 'province',   JSON.stringify(addr.province));
    sessionStorage.setItem(dataType + 'city',       JSON.stringify(addr.city));
    sessionStorage.setItem(dataType + 'district',   JSON.stringify(addr.district));
}

function GetAreaInfo(dataType)
{
    // 获取地址信息
    var countryData     = JSON.parse(sessionStorage.getItem(dataType + 'country'));
    var provinceData    = JSON.parse(sessionStorage.getItem(dataType + 'province'));
    var cityData        = JSON.parse(sessionStorage.getItem(dataType + 'city'));
    var districtData    = JSON.parse(sessionStorage.getItem(dataType + 'district'));

    var countryName = "";
    if ( countryData != null )
        countryName = countryData.area_name;

    var provinceName = "";
    if ( provinceData != null )
        provinceName = provinceData.area_name;

    var cityName = "";
    if ( cityData != null )
        cityName = cityData.area_name;

    var districtName = "";
    if ( districtData != null )
        districtName = districtData.area_name;

    var addr = countryName + provinceName + cityName + districtName;
    return addr;
}
