(function(factory) 
{
    if (typeof define === 'function' && define.amd) 
    {
        // AMD. Register as anonymous module.
        define(['jquery', 'ChineseDistricts'], factory);
    } else if (typeof exports === 'object') 
    {
        // Node / CommonJS
        factory(require('jquery'), require('ChineseDistricts'));
    } else 
    {
        // Browser globals.
        factory(jQuery, ChineseDistricts);
    }
}) 
(function($, ChineseDistricts, CommonServices) 
{
    'use strict';
    if (typeof ChineseDistricts === 'undefined') 
    {
        throw new Error('The file "city-picker.data.js" must be included first!');
    }

    var NAMESPACE       = 'citypicker';
    var EVENT_CHANGE    = 'change.' + NAMESPACE;

    // 显示的级别
    var COUNTRY         = 'country';                                    // 国家Tab
    var PROVINCE        = 'province';                                   // 省份Tab
    var CITY            = 'city';                                       // 地市Tab
    var DISTRICT        = 'district';                                   // 区县Tab
    var TOWN            = 'town';                                       // 乡镇，默认不显示

    var FirstTab        = '';                                           // 首页Tab
    var ClieckedTab     = '';                                           // 点击的Tab
    var FirstAreaCode   = 1;                                            // 第一个区域的ID，默认为1，即显示为国家
    var TabList         = [];                                           // 加载的TAB列表

    var dataName        = "";


    // 参数：
    // level：显示的级别，默认都为显示到区县，
    // country: 是否显示国家Tab，为1显示，其他或没有此参数则不显示
    // type: 数据类型名称，以便在一个页面有多个地址选择时进行区分

    function CityPicker(element, options) 
    {
        this.$element = $(element);

        this.dataName = this.$element[0].dataset.type;

        this.$dropdown = null;
        this.options = $.extend({}, CityPicker.DEFAULTS, $.isPlainObject(options) && options);

        if ( this.options.country && this.options.country == '1')
        {
            FirstAreaCode   = 1;
            FirstTab        = COUNTRY;
            TabList         = [COUNTRY, PROVINCE, CITY, DISTRICT, TOWN];
        }
        else
        {
            FirstTab        = PROVINCE;
            FirstAreaCode   = 1000000;
            TabList         = [PROVINCE, CITY, DISTRICT, TOWN];
        }

        if ( !this.options.level )
        {
            this.options.level = DISTRICT;                              // 默认显示到区县，乡镇不显示
            TabList.pop();                                              // 删除乡镇Tab
        }

        this.active = false;
        this.dems = [];
        this.needBlur = false;
        this.init(this.dataName);
    }

    CityPicker.prototype = 
    {
        constructor: CityPicker,

        init: function(dataName) 
        {
            this.dataName = dataName;

            this.defineDems();
            this.render();
            this.bind();
            this.active = true;

            this.inititalText();

        },

        // 初始化时检查是否已经有地址可以显示
        inititalText: function()
        {
            var dataName        = this.dataName;
            var countryData     = JSON.parse(sessionStorage.getItem(this.dataName + 'country'));
            var provinceData    = JSON.parse(sessionStorage.getItem(this.dataName + 'province'));
            var cityData        = JSON.parse(sessionStorage.getItem(this.dataName + 'city'));
            var districtData    = JSON.parse(sessionStorage.getItem(this.dataName + 'district'));
            
            // if ( provinceData == null || cityData == null )
            //     return;

            var text = '';
            this.$dropdown.find('.area-select').each(function()
            {
                var type = $(this).data('count');
                var item = {};
                if (type == COUNTRY && countryData != null)
                {
                    item.code = countryData.id;
                    item.address =  countryData.area_name;
                }
                else if (type == PROVINCE && provinceData != null)
                {
                    item.code = provinceData.id;
                    item.address = provinceData.area_name;
                }
                else if (type == CITY && cityData != null)
                {
                    item.code = cityData.id;
                    item.address =  cityData.area_name;
                }
                else if (type == DISTRICT && districtData != null )
                {
                    item.code = districtData.id;
                    item.address = districtData.area_name;
                }
                else
                {
                    item.code = 0;
                    item.address = "";
                }

                text += ($(this).hasClass(FirstTab) ? '' : '/') + '<span class="select-item" data-count="' +
                        type + '" data-code="' + item.code + '" data-type="' + dataName + '">' + item.address + '</span>';

                // var $select = this['$' + type];
                // $select.data('item', item);
            });
            
            if (text) 
            {
                this.$textspan.find('>.placeholder').hide();
                this.$textspan.find('>.title').html(text).show();
            }
            else
            {
                this.$textspan.find('>.placeholder').text(this.getPlaceHolder()).show();
                this.$textspan.find('>.title').html('').hide();
            }
        },

        // dems赋值
        defineDems: function() 
        {
            var stop = false;
            $.each([COUNTRY, PROVINCE, CITY, DISTRICT, TOWN], $.proxy(function(i, type) 
            {
                if ( type == COUNTRY )
                {
                    if ( FirstTab == COUNTRY )
                    {
                        // 是否需要显示国家Tab，如果不显示国家TAB，则默认国家为中国
                        this.dems.push(type);
                    }
                }
                else
                {
                    if (!stop) 
                    {
                        this.dems.push(type);
                    }

                    if (type === this.options.level) 
                    {
                        stop = true;
                    }
                }
            }, this));
        },

        // 界面显示处理
        render: function() 
        {
            var p = this.getPosition(),
                placeholder = this.$element.attr('placeholder') || this.options.placeholder,
                textspan = '<span class="city-picker-span" style="' +
                // this.getWidthStyle(p.width) + 
                'height:' +
                p.height + 'px;line-height:' + (p.height - 1) + 'px;">' +
                (placeholder ? '<span class="placeholder">' + placeholder + '</span>' : '') +
                '<span class="title"></span><div class="arrow"></div>' + '</span>',

                dropdown = '<div class="city-picker-dropdown" style="left:0px;top:100%;' +
                (this.includeDem('country') ? this.getWidthStyle( 900, true) : this.getWidthStyle( p.width, true) ) + '">' +
                '<div class="area-select-wrap">' +
                '<div class="area-select-tab">' +
                (this.includeDem('country') ? '<a data-count="country">国家</a>' : '') +
                '<a class="active" data-count="province">省份</a>' +
                (this.includeDem('city') ? '<a data-count="city">城市</a>' : '') +
                (this.includeDem('district') ? '<a data-count="district">区县</a>' : '') +
                (this.includeDem('town') ? '<a data-count="town">乡镇</a>' : '') +
                '</div>' +
                '<div class="area-select-content">' +
                (this.includeDem('country') ? '<div class="area-select country" data-count="country"></div>' : '') +
                '<div class="area-select province" data-count="province"></div>' +
                (this.includeDem('city') ? '<div class="area-select city" data-count="city"></div>' : '') +
                (this.includeDem('district') ? '<div class="area-select district" data-count="district"></div>' : '') +
                (this.includeDem('town') ? '<div class="area-select town" data-count="town"></div>' : '') +
                '</div></div>';

            this.$element.addClass('city-picker-input');
            this.$textspan = $(textspan).insertAfter(this.$element);
            this.$dropdown = $(dropdown).insertAfter(this.$textspan);
            var $select = this.$dropdown.find('.area-select');

            // setup this.$province, this.$city and/or this.$district object
            $.each(this.dems, $.proxy(function(i, type) 
            {
                this['$' + type] = $select.filter('.' + type + '');
            }, this));

            this.refresh();
        },

        refresh: function(force) 
        {
            // clean the data-item for each $select
            var $select = this.$dropdown.find('.area-select');
            $select.data('item', null);
            // parse value from value of the target $element
            var val = this.$element.val() || '';
            val = val.split('/');
            $.each(this.dems, $.proxy(function(i, type) 
            {
                // 遍历dems
                if (val[i] && i < val.length) 
                {
                    this.options[type] = val[i];                        // 把当前显示值赋值给options
                }
                else if (force) 
                {
                    this.options[type] = '';
                }
                this.output(type);                                      // 输出下拉框显示数据
            }, this));

            // this.tab(PROVINCE);
            this.tab(FirstTab);
            this.feedText();                                            // 界面显示选择的内容
            this.feedVal();                                             // input标签value赋值
        },

        includeDem: function(type) 
        {
            return $.inArray(type, this.dems) !== -1;
        },

        getPosition: function() 
        {
            var p, h, w, s, pw;
            p = this.$element.position();
            s = this.getSize(this.$element);
            h = s.height;
            w = s.width;
            if (this.options.responsive) 
            {
                pw = this.$element.offsetParent().width();
                if (pw) 
                {
                    w = w / pw;
                    if (w > 0.99) 
                    {
                        w = 1;
                    }
                    w = w * 100 + '%';
                }
            }

            return {
                top: p.top || 0,
                left: p.left || 0,
                height: h,
                width: w
            };
        },

        getSize: function($dom) 
        {
            var $wrap, $clone, sizes;
            if (!$dom.is(':visible')) 
            {
                $wrap = $("<div />").appendTo($("body"));
                $wrap.css({
                    "position": "absolute !important",
                    "visibility": "hidden !important",
                    "display": "block !important"
                });

                $clone = $dom.clone().appendTo($wrap);

                sizes = {
                    width: $clone.outerWidth(),
                    height: $clone.outerHeight()
                };

                $wrap.remove();
            }
            else
            {
                sizes = {
                    width: $dom.outerWidth(),
                    height: $dom.outerHeight()
                };
            }

            return sizes;
        },

        getWidthStyle: function(w, dropdown) 
        {
            if (this.options.responsive && !$.isNumeric(w)) 
            {
                return 'width:' + w + ';';
            }
            else
            {
                return 'width:' + (dropdown ? Math.max(320, w) : w) + 'px;';
            }
        },

        // 绑定事件
        bind: function()
        {
            var $this = this;
            $(document).on('click', (this._mouteclick = function(e) 
            {
                var $target = $(e.target);
                var $dropdown, $span, $input;
                if ($target.is('.city-picker-span')) 
                {
                    $span = $target;
                }
                else if ($target.is('.city-picker-span *')) 
                {
                    $span = $target.parents('.city-picker-span');
                }
                
                if ($target.is('.city-picker-input')) 
                {
                    $input = $target;
                }
                
                if ($target.is('.city-picker-dropdown')) 
                {
                    $dropdown = $target;
                }
                else if ($target.is('.city-picker-dropdown *')) 
                {
                    $dropdown = $target.parents('.city-picker-dropdown');
                }

                if ((!$input && !$span && !$dropdown) ||
                    ($span && $span.get(0) !== $this.$textspan.get(0)) ||
                    ($input && $input.get(0) !== $this.$element.get(0)) ||
                    ($dropdown && $dropdown.get(0) !== $this.$dropdown.get(0))) 
                {
                    $this.close(true);
                }
            }));

            this.$element.on('change', (this._changeElement = $.proxy(function() 
            {
                this.close(true);
                this.refresh(true);
            }, this))).on('focus', (this._focusElement = $.proxy(function() 
            {
                this.needBlur = true;
                this.open();
            }, this))).on('blur', (this._blurElement = $.proxy(function() 
            {
                if (this.needBlur) 
                {
                    this.needBlur = false;
                    this.close(true);
                }
            }, this)));

            this.$textspan.on('click', function(e) 
            {
                var $target = $(e.target), type;
                $this.needBlur = false;
                if ($target.is('.select-item')) 
                {
                    type = $target.data('count');
                    $this.open(type);
                }
                else 
                {
                    if ($this.$dropdown.is(':visible')) 
                    {
                        $this.close();
                    }
                    else
                    {
                        $this.open();
                    }
                }
            }).on('mousedown', function()
            {
                $this.needBlur = false;
            });
            
            this.$dropdown.on('click', '.area-select a', function()
            {
                var $select = $(this).parents('.area-select');
                var $active = $select.find('a.active');
                var last = $select.next().length === 0;
                $active.removeClass('active');
                $(this).addClass('active');
                if ($active.data('code') !== $(this).data('code'))
                {
                    $select.data('item', {
                        address: $(this).attr('title'),
                        code: $(this).data('code')
                    });

                    $(this).trigger(EVENT_CHANGE);
                    $this.feedText();
                    $this.feedVal(true);
                    if (last) 
                    {
                        $this.close();
                    }
                }
            }).on('click', '.area-select-tab a', function() 
            {
                if (!$(this).hasClass('active')) 
                {
                    var type = $(this).data('count');
                    $this.tab(type);
                }
            }).on('mousedown', function() 
            {
                $this.needBlur = false;
            });

            if (this.$country) 
            {
                this.$country.on(EVENT_CHANGE, (this._changeCountry = $.proxy(function() 
                {
                    this.clearAfterTabData(COUNTRY);
                    ClieckedTab = COUNTRY;
                    if (this.output(PROVINCE))
                    { 
                        // 判断下一个tab是否有数据,没有则关闭下拉
                        $this.close();
                        return;
                    };
                    this.tab(PROVINCE);
                }, this)));
            }

            if (this.$province) 
            {
                this.$province.on(EVENT_CHANGE, (this._changeProvince = $.proxy(function() 
                {
                    this.clearAfterTabData(PROVINCE);
                    ClieckedTab =  PROVINCE;
                    if (this.output(CITY))
                    {
                        // 判断下一个tab是否有数据,没有则关闭下拉
                        $this.close();
                        return;
                    };
                    this.tab(CITY);
                }, this)));
            }

            if (this.$city) 
            {
                this.$city.on(EVENT_CHANGE, (this._changeCity = $.proxy(function() 
                {
                    this.clearAfterTabData(CITY);
                    ClieckedTab =  CITY;
                    if (this.output(DISTRICT)) 
                    {
                        $this.close();
                        return;
                    };
                    this.tab(DISTRICT);
                }, this)));
            }

            if (this.$district) 
            {
                this.$district.on(EVENT_CHANGE, (this._changeDistrict = $.proxy(function() 
                {
                    this.clearAfterTabData(DISTRICT);
                    ClieckedTab =  DISTRICT;
                    if (this.output(TOWN)) 
                    {
                        $this.close();
                        return;
                    };
                    this.tab(TOWN);
                }, this)));
            }
        },

        // 显示下拉
        open: function(type) 
        {
            // 确定第一个Tab是国家还是省份
            // type = type || PROVINCE;
            type = type || FirstTab;

            this.$dropdown.show();
            this.$textspan.addClass('open').addClass('focus');
            this.tab(type);
        },

        // 在点击TAB后，将该TAB后面TAB的数据先进行清除
        clearAfterTabData: function( tabType )
        {
            var TabIndex;
            for ( var i = 0; i < TabList.length; i++)
            {
                if ( TabList[i] == tabType )
                    TabIndex = i;
            }

            for ( var i = TabIndex + 1; i < TabList.length; i++)
            {
                var $select = this['$' + TabList[i]];
                var list = [];
                list.push('<dl class="clearfix"><dd>');
                list.push('</dd></dl>');

                $select.html(list.join(''));
            }
        },

        // 关闭下拉
        close: function(blur) 
        {
            this.$dropdown.hide();
            this.$textspan.removeClass('open');
            if (blur) 
            {
                this.$textspan.removeClass('focus');
            }
        },

        // 解绑事件
        unbind: function() 
        {
            $(document).off('click', this._mouteclick);

            this.$element.off('change', this._changeElement);
            this.$element.off('focus', this._focusElement);
            this.$element.off('blur', this._blurElement);

            this.$textspan.off('click');
            this.$textspan.off('mousedown');

            this.$dropdown.off('click');
            this.$dropdown.off('mousedown');
            
            if (this.$country) 
            {
                this.$country.off(EVENT_CHANGE, this._changeCountry);
            }

            if (this.$province) 
            {
                this.$province.off(EVENT_CHANGE, this._changeProvince);
            }

            if (this.$city) 
            {
                this.$city.off(EVENT_CHANGE, this._changeCity);
            }

            if (this.$district) 
            {
                this.$district.off(EVENT_CHANGE, this._changeDistrict);
            }
        },

        // 获取选择项信息
        getText: function() 
        {
            var text = '';
            var dataName = this.dataName;
            this.$dropdown.find('.area-select').each(function()
            {
                var item = $(this).data('item'),
                    type = $(this).data('count');
                if (item)
                {
                    // 判断Tab页面是否在被点击的Tab后面，如果是，则设置需要显示的内容为空
                    var clieckTabIndex, currentTabIndex;
                    for ( var i = 0; i < TabList.length; i++)
                    {
                        if ( TabList[i] == ClieckedTab )
                            clieckTabIndex = i;

                        if ( $(this).hasClass(TabList[i]))
                            currentTabIndex = i;
                    }

                    if ( currentTabIndex > clieckTabIndex )
                        return '';

                    var data = { id : item.code, area_name: item.address};
                    // 保存已经选择的地区数据

                    var name = dataName + TabList[currentTabIndex];
                    // console.log("Area Data: " + name + ": " + data);
                    sessionStorage.setItem( name, JSON.stringify(data));
                    
                    text += ($(this).hasClass(FirstTab) ? '' : '/') + '<span class="select-item" data-count="' +
                            type + '" data-code="' + item.code + '">' + item.address + '</span>';
                        // text += ($(this).hasClass('province') ? '' : '/') + '<span class="select-item" data-count="' +
                        //     type + '" data-code="' + item.code + '">' + item.address + '</span>';
                    }
            });

            return text;
        },

        getPlaceHolder: function() 
        {
            return this.$element.attr('placeholder') || this.options.placeholder;
        },

        // 显示placeholder或者选择的区域
        feedText: function() 
        {
            var text = this.getText();
            if (text) 
            {
                this.$textspan.find('>.placeholder').hide();
                this.$textspan.find('>.title').html(text).show();
            }
            else
            {
                this.$textspan.find('>.placeholder').text(this.getPlaceHolder()).show();
                this.$textspan.find('>.title').html('').hide();
            }
        },

        getCode: function(count) 
        {
            var obj = {},
                arr = [];
            this.$textspan.find('.select-item')
                .each(function() 
                {
                    var code = $(this).data('code');
                    var count = $(this).data('count');
                    obj[count] = code;
                    arr.push(code);
                });
            return count ? obj[count] : arr.join('/');
        },

        getVal: function() 
        {
            var text = '';
            var code = '';
            this.$dropdown.find('.area-select')
                .each(function() 
                {
                    var item = $(this).data('item');
                    if (item) 
                    {                        
                        // text += ($(this).hasClass('province') ? '' : '/') + item.address;
                        // code += ($(this).hasClass('province') ? '' : '_') + item.code;
                        text += ($(this).hasClass(FirstTab) ? '' : '/') + item.address;
                        code += ($(this).hasClass(FirstTab) ? '' : '_') + item.code;
                    }
                });
            $("#addrValue").val(code);
            return text;
        },

        // input的value赋值
        feedVal: function(trigger) 
        {
            this.$element.val(this.getVal());
            if (trigger) 
            {
                this.$element.trigger('cp:updated');
            }
        },

        // 输出数据
        output: function(type) 
        {
            var $this = this;
            var options = this.options;
            //var placeholders = this.placeholders;
            var $select = this['$' + type];
            // var data = type === PROVINCE ? {} : [];
            var data = type === FirstTab ? {} : [];
            var item;
            var districts;
            var code;
            var matched = null;
            var value;
            if (!$select || !$select.length) 
            {
                return;
            }
            
            item = $select.data('item');
            value = (item ? item.address : null) || options[type];
            code = (
                // type === PROVINCE ? 86 :
                type === COUNTRY && type === FirstTab ? FirstAreaCode :
                type === PROVINCE && type === FirstTab ? FirstAreaCode :
                type === PROVINCE && FirstTab == COUNTRY ? this.$country && this.$country.find('.active').data('code') :
                type === CITY ? this.$province    && this.$province.find('.active').data('code') :
                type === DISTRICT ? this.$city    && this.$city.find('.active').data('code') :
                type === TOWN ? this.$district  && this.$district.find('.active').data('code') : code
            );

            districts = $.isNumeric(code) ? ChineseDistricts[code] : null;
            if ($.isPlainObject(districts)) 
            {
                $.each(districts, function(code, address) 
                {
                    var provs;
                    // if (type === PROVINCE)
                    if ( type == FirstTab )
                    {
                        provs = [];
                        for (var i = 0; i < address.length; i++) 
                        {
                            if (address[i].address === value) 
                            {
                                matched = {
                                    code: address[i].code,
                                    address: address[i].address
                                };
                            }
                            provs.push({
                                code: address[i].code,
                                address: address[i].address,
                                selected: address[i].address === value
                            });
                        }
                        data[code] = provs;
                    }
                    else
                    {
                        if (address === value)
                        {
                            matched = {
                                code: code,
                                address: address
                            };
                        }

                        data.push({
                            code: code,
                            address: address,
                            selected: address === value
                        });
                    }
                });
            }

            // $select.html(type === PROVINCE ? this.getProvinceList(data) : this.getList(data, type));
            $select.html(type === FirstTab ? this.getFirstTabAreaList(data, type) : this.getList(data, type));

            $select.data('item', matched);                              // 当前tab添加item(包含选择对象的内容)
            if (!(type === FirstTab)) 
            { 
                // 标识:下一个选项没有数据则关闭
                if (data.length == 0) 
                {
                    return true;
                }
            }
        },

        // 遍历国家或省份
        getFirstTabAreaList: function(data, type) 
        {
            var list = [],
                $this = this,
                simple = this.options.simple;

            $.each(data, function(i, n) 
            {
                list.push('<dl class="clearfix">');
                list.push('<dt>' + i + '</dt><dd>');
                $.each(n, function(j, m) 
                {
                    list.push(
                        '<a' +
                        ' title="' + (m.address || '') + '"' +
                        ' data-code="' + (m.code || '') + '"' +
                        ' class="' +
                        (m.selected ? ' active' : '') +
                        '">' +
                        (simple ? $this.simplize(m.address, type) : m.address) +
                        '</a>');
                });
                list.push('</dd></dl>');
            });
            return list.join('');
        },

        // 遍历市或区或县
        getList: function(data, type) 
        {
            var list = [],
                $this = this,
                simple = this.options.simple;
            list.push('<dl class="clearfix"><dd>');

            $.each(data, function(i, n) 
            {
                list.push(
                    '<a' +
                    ' title="' + (n.address || '') + '"' +
                    ' data-code="' + (n.code || '') + '"' +
                    ' class="' +
                    (n.selected ? ' active' : '') +
                    '">' +
                    (simple ? $this.simplize(n.address, type) : n.address) +
                    '</a>');
            });
            list.push('</dd></dl>');

            return list.join('');
        },

        //简化名字
        // simplize: function(address, type) {
        //     address = address || '';
        //     if (type === PROVINCE) {
        //         return address.replace(/[省,市,自治区,壮族,回族,维吾尔]/g, '');
        //     } else if (type === CITY) {
        //         return address.replace(/[市,地区,回族,蒙古,苗族,白族,傣族,景颇族,藏族,彝族,壮族,傈僳族,布依族,侗族]/g, '')
        //             .replace('哈萨克', '').replace('自治州', '').replace(/自治县/, '');
        //     } else if (type === DISTRICT) {
        //         return address.length > 2 ? address.replace(/[市,区,县,旗]/g, '') : address;
        //     }
        // },
        // 处理tab显示
        tab: function(type) 
        {
            var $selects = this.$dropdown.find('.area-select');
            var $tabs = this.$dropdown.find('.area-select-tab > a');
            var $select = this['$' + type];
            var $tab = this.$dropdown.find('.area-select-tab > a[data-count="' + type + '"]');
            if ($select) {
                $selects.hide();
                $select.show();
                $tabs.removeClass('active');
                $tab.addClass('active');
            }
        },

        // reset: function() {
        //     this.$element.val(null).trigger('change');
        // },

        // destroy: function() {
        //     this.unbind();
        //     this.$element.removeData(NAMESPACE).removeClass('city-picker-input');
        //     this.$textspan.remove();
        //     this.$dropdown.remove();
        // }
    };

    // CityPicker.DEFAULTS = {
    //     simple: false,
    //     responsive: false,
    //     placeholder: '请选择省/市/区/县',
    //     level: 'town',
    //     province: '',
    //     city: '',
    //     district: '',
    //     town: ''
    // };

    CityPicker.setDefaults = function(options) 
    {
        $.extend(CityPicker.DEFAULTS, options);
    };

    // Save the other citypicker
    CityPicker.other = $.fn.citypicker;

    // Register as jQuery plugin
    $.fn.citypicker = function(option) 
    {
        var args = [].slice.call(arguments, 1);

        return this.each(function() 
        {
            var $this = $(this);
            var data = $this.data(NAMESPACE);
            var options;
            var fn;

            if (!data) 
            {
                if (/destroy/.test(option)) 
                {
                    return;
                }

                options = $.extend({}, $this.data(), $.isPlainObject(option) && option);
                $this.data(NAMESPACE, (data = new CityPicker(this, options)));
            }

            if (typeof option === 'string' && $.isFunction(fn = data[option])) 
            {
                fn.apply(data, args);
            }
        });
    };

    $.fn.citypicker.Constructor = CityPicker;
    $.fn.citypicker.setDefaults = CityPicker.setDefaults;

    // No conflict
    $.fn.citypicker.noConflict = function() 
    {
        $.fn.citypicker = CityPicker.other;
        return this;
    };

    $(function() 
    {
        $('[data-toggle="city-picker"]').citypicker();
    });
});
