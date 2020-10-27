////////////////////////////////////////////////////////////////////////////////////////////

angular.module('elecMapControllers',['treeControl'])

//////////////////////////////////////////////////////////////////////////////////////////////
// 电子地图显示
.controller("elecMapCtrl", function($scope, $window, $rootScope, $filter, CommonService, $interval)
{
    CommonService.heartbeat();
    $scope.user = null;
    if(sessionStorage.getItem("strUser"))
        $scope.user=JSON.parse(sessionStorage.getItem("strUser"));
    else
        window.location.href = "/login.html"

    var videoIndex = 0;
    var uriData = "lid=2";
    CommonService.getAll("elecMap", uriData,
        function(data)
        {
            $scope.videoList = data.videoList;
            $scope.videoUrl = $scope.videoList[0];

            $scope.stationList = listToObject( data.stationList, 'rows').rows;
            for(i = 0; i < $scope.stationList.length; i++)
            {
                $scope.stationList[10].isExange = 0;
                $scope.stationList[i].exangeName = "";
                if ( i < 9 )
                    $scope.stationList[i].image="./resource/images/cycle0" + (i + 1) + ".png";
                else
                    $scope.stationList[i].image="./resource/images/cycle" + (i + 1) + ".png";
                
                if ( i == $scope.stationList.length - 1)
                    $scope.stationList[i].isLast = 1;
                else
                    $scope.stationList[i].isLast = 0;
            }

            $scope.stationList[3].image="./resource/images/cyclea04.png";
            $scope.stationList[21].isExange = 1;
            $scope.stationList[21].exangeName = "7号线";
            
            $scope.stationList[15].isExange = 1;
            $scope.stationList[15].exangeName = "3号线";

            $scope.stationList[14].isExange = 1;
            $scope.stationList[14].exangeName = "1号线";

            $scope.stationList[11].isExange = 1;
            $scope.stationList[11].exangeName = "4号线";

            $scope.stationList[8].isExange = 1;
            $scope.stationList[8].exangeName = "7号线";

        },
        function(data, status)
        {
            onErrorResult(data);
        }
    );

    var videoAD = document.getElementById("videoAD");

    videoAD.addEventListener("ended", function() 
    {
        //为vedio添加ended监听，当视频播放完毕后执行对应函数
        if( videoIndex == ($scope.videoList.length - 1)) 
        {
            //判断是否到了最后一个视频
            $scope.videoUrl = $scope.videoList[0]; //播放第一个视频
            videoAD.src = $scope.videoUrl;
            videoAD.play();
            videoIndex = 0;//将下标重设为 0
        } 
        else 
        {
            videoIndex += 1; //每播放一次则将下标加一
            $scope.videoUrl = $scope.videoList[videoIndex];
            videoAD.src = $scope.videoUrl;
            videoAD.play();
        }
 
    });

})
