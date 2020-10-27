/**
 * Created by yuzhenhan on 14-9-26.
 */

var CommonDirectives = angular.module('CommonDirectives', []);

//var EMAIL_REGEXP = /^[\w-]+(\.[\w-]+)*@[\w-]+(\.[\w-]+)+$/;
//var MOBILE_REGEXP= /^0?(13|15|18|17)[0-9]{9}$/;

var EMAIL_AND_MOBILE_REGEXP=/[^\._-][\w\.-]+@(?:[A-Za-z0-9]+\.)+[A-Za-z]+$|^0\d{2,3}\d{7,8}$|^1[358]\d{9}$|^147\d{8}/;
/*
CommonDirectives.directive('emailmobile', function() {
    return {
        require: 'ngModel',
        link: function(scope, elm, attrs, ctrl) {
            ctrl.$parsers.unshift(function(viewValue) {
                // 匹配email&mobile
                if (EMAIL_AND_MOBILE_REGEXP.test(viewValue)) {
                    // it is valid
                    ctrl.$setValidity('emailmobile', true);
                    return viewValue;
                } else {
                    // it is invalid, return undefined (no model update)
                    ctrl.$setValidity('emailmobile', false);
                    return undefined;
                }
            });
        }
    };
});

//判断俩个值是否为不同的
CommonDirectives.directive('repeat', [function() {
    return {
        restrict: 'A',
        require: "ngModel",
        link: function (scope, element, attrs, ctrl) {
            if (ctrl) {
                var otherInput = element.inheritedData("$formController")[attrs.repeat];

                var repeatValidator = function (value) {
                    var validity = value === otherInput.$viewValue;
                    ctrl.$setValidity("repeat", validity);
                    return validity ? value : undefined;
                };

                ctrl.$parsers.push(repeatValidator);
                ctrl.$formatters.push(repeatValidator);

                otherInput.$parsers.push(function (value) {
                    ctrl.$setValidity("repeat", value === ctrl.$viewValue);
                    return value;
                });
            }
        }

    }
}]);

//判断俩个值是否为相同的
CommonDirectives.directive('same', function() {
    return {
        restrict: 'A',
        require: 'ngModel',
        link: function(scope, element, attrs, ctrl) {
            if (ctrl) {
                var otherInput = element.inheritedData("$formController")[attrs.same];

                var sameValidator = function (value) {
                    var validity = value !== otherInput.$viewValue;
                    ctrl.$setValidity("same", validity);
                    return validity ? value : undefined;
                };

                ctrl.$parsers.push(sameValidator);
                ctrl.$formatters.push(sameValidator);

                otherInput.$parsers.push(function (value) {
                    ctrl.$setValidity("same", value !== ctrl.$viewValue);
                    return value;
                });
            }
        }
    };
});

//判断值的长度是否大于20
CommonDirectives.directive('gttwenty', function() {
    return {
        restrict: 'A',
        require: 'ngModel',
        link: function(scope, element, attrs,ctrl) {
               if(ctrl){
                var gttwentyValidator = function (value) {
                    var validity = (value.length>20);
                    ctrl.$setValidity("gttwenty", !validity);
                    return validity ? undefined : value ;
                };
                ctrl.$parsers.push(gttwentyValidator);
                ctrl.$formatters.push(gttwentyValidator);
               }
         }
    };
});

//判断值的长度是否小于6
CommonDirectives.directive('ltsix', function() {
    return {
        restrict: 'A',
        require: 'ngModel',
        link: function(scope, element, attrs,ctrl) {
            if(ctrl){
                var sixValidator = function (value) {
                    var validity = (value.length>20);
                    ctrl.$setValidity("six", !validity);
                    return validity ? undefined : value ;
                };
                ctrl.$parsers.push(twentyValidator);
                ctrl.$formatters.push(twentyValidator);
            }
        }
    };
});

//让AngularJs渲染完成后,返回ngRepeatFinished事件
CommonDirectives.directive('onFinishRenderFilters', function ($timeout) {
    return {
        restrict: 'A',
        link: function(scope, element, attr) {
            if (scope.$last === true) {
                $timeout(function() {
                    scope.$emit('ngRepeatFinished');
                });
            }
        }
    };
});

//让AngularJs渲染完成后,返回ngRepeatFinished2事件
CommonDirectives.directive('onFinishRenderFilters2', function ($timeout) {
    return {
        restrict: 'A',
        link: function(scope, element, attr) {
            if (scope.$last === true) {
                $timeout(function() {
                    scope.$emit('ngRepeatFinished2');
                });
            }
        }
    };
});

// validate sql injection
var SQLInjection_VFIT="'|and|exec|insert|select|delete|update|count|*|%|chr|mid|master|truncate|char|declare|; |or|-|+|,";

CommonDirectives.directive('vsqlinjection', function() {
    return {
       require: 'ngModel',
       link: function(scope, elm, attrs, ctrl) {
           ctrl.$parsers.unshift(function(viewValue) {
               if(viewValue!=undefined){
                  var resultStr = viewValue.toLowerCase();
                  var alertStr = "";

                  var vFit = SQLInjection_VFIT;
                  var vFitArray = vFit.split("|");

                  for(var i=0; i<vFitArray.length; i++){
                      // 匹配SQLInjection_VFIT
                      if(resultStr.indexOf(vFitArray[i]) >= 0){
                          alertStr += vFitArray[i] + " ";
                      }
                  }

                  if (alertStr == "") {
                      // it is valid
                      ctrl.$setValidity('vsqlinjection', true);
                      return viewValue;
                  } else {
                      // it is invalid, return undefined (no model update)
                      ctrl.$setValidity('vsqlinjection', false);
                      return alertStr;
                  }

               }else{
                    ctrl.$setValidity('vsqlinjection', true);
                    return viewValue;
               }
           });
       }
    };
});*/

CommonDirectives.directive('usPager', function() {
    return {
       restrict : 'AE',
       replace : true,
       template: function(tElement, tAttrs){

                    if (!tElement.length) {
                    	return "";
                    }
                    var e = tElement[0];

                    var pagerId = e.id;
                    if (pagerId) {
                    	pagerId = pagerId + "";
                    } else {
                    	pagerId = "defaultPager";
                    }
                    var clsName = e.className;

					/* new pager html */
					var _html = "\
					<ul id='" + pagerId + "' class='pager " + clsName + "'>		\
						<li class='previous' ><a href='' id='first' ng-click='pagerEvent($event, \"" + pagerId + "\")'>首页</a></li>	\
						<li class='previous' ><a href='' id='prev' ng-click='pagerEvent($event, \"" + pagerId + "\")'>上一页</a></li>	\
						<li ><a href='' id='beforeSL'>...</a></li>	\
						<li ng-repeat='pageNumber in pageNumbers" + pagerId + "'><a href=''class=''id='{{pageNumber}}' ng-click='pagerEvent($event, \"" + pagerId + "\")'>{{pageNumber}}</a></li>	\
						<li ><a href='' id='afterSL'>...</a></li>	\
						<li class='next'><a href='' id='next' ng-click='pagerEvent($event, \"" + pagerId + "\")'>下一页</a></li>	\
						<li class='next'><a href='' id='last' ng-click='pagerEvent($event, \"" + pagerId + "\")'>尾页</a></li>	\
						<li class='xColor'>|</li>	\
						<li class='xColor'>共{{pageCount" + pagerId + "}}页</li>	\
					</ul>";

                    return _html;
                 }
    };
});

CommonDirectives.directive('imageonload', function () {
    return {
        restrict: 'A', link: function (scope, element, attrs) {
            element.bind('load', function () {
                //call the function that was passed
                scope.$apply(attrs.imageonload);
            });
        }
    };
})

CommonDirectives.directive('enterkey', function () {
    return {
        restrict: 'A', link: function (scope, element, attrs) {
            element.bind('load', function () {
                //call the function that was passed
                scope.$apply(attrs.imageonload);
            });
        }
    };
})


