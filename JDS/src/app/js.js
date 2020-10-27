$(function(){
	$(".xNavUl  li#xNavUlLi").mouseenter(function(){ //鼠标移入
			$(".navRightUlTop").show();
		}).mouseleave(function(){ //鼠标移出
			$(".navRightUlTop").hide();
	});
	/*$(".xNavUl  li#xNavUlLi").click(function(){
		var open = true;
		if(open == true){
			$(".navRightUlTop").toggle();
		}
	});*/
});

function initViewHeight() {
	if (window.innerHeight) {
		var winHeight = window.innerHeight;
		if (winHeight < 400) {
			winHeight = 400;
		}
		//console.log(winHeight-82);
		$("#winHeight").css({"min-height":winHeight-118});

		var fullBlockWraper = $(".fullBlockWraper");
		if (fullBlockWraper.length) {
			fullBlockWraper.css({"min-height":winHeight - 118 - 60});
		}
	}
}

window.onresize = initViewHeight;
initViewHeight();
