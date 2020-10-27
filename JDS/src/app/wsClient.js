

function WsClient(listenters) {
	if (!(this instanceof WsClient)){
		return new WsClient(listenters);
	}

	this.opened = false;
	this.listeners = new Listeners(listenters, this)
}

WsClient.prototype.reconnect = function (delay) {
	var _this = this;
	setTimeout(function () {
		_this.connect();
	}, delay);
};

WsClient.prototype.sendMsg = function (msg) {
	if (!this.webSocket) {
		this.connect();
		//this.pendingMsg = msg;
		return false;
	}
	var ws = this.webSocket;
	if (ws && this.opened) {
		ws.send(JSON.stringify(msg));
		return true;
	}
	return false;
};

WsClient.prototype.connect = function () {

	var _this = this;
	var url = appConfig.webSvcUrl + "referral/wss";
	url = url.replace("http://", "ws://");
	url = url.replace("https://", "wss://");

	if (this.webSocket) {
		this.webSocket.close();
		this.webSocket = null;
	}

	var ws = new WebSocket(url);

	this.webSocket = ws;
	this.closed = false;

	ws.onopen = function() {
		D("ws open");
		_this.opened = true;
		_this.listeners.invoke("onopen");
	};

	ws.onmessage = function(event) {
		//D(event);
		D("ws onmessage : " + event.data);
		var data = JSON.parse(event.data);
		_this.listeners.invoke("onmessage", data);
	};

	ws.onclose = function(event) {
		//D(event);
		D("ws onclose, try reconnect");
		_this.opened = false;
		_this.listeners.invoke("onclose");
		_this.webSocket = null;

		if (_this.closed) {
			return;
		}

		_this.reconnectTimer = setTimeout(function () {
			_this.reconnectTimer = null;
			if (!_this.webSocket) {
				_this.connect();
			}
		}, 5000);
	};
};
WsClient.prototype.msgHanderForPC=function(content,myid){
	if(content.type=="0"){//文本
		var html="";
		if(content.uid==myid){
			html+='<div class="msg-item-right">';
			html+='<div class="msg-title">'+content.name+'&nbsp;&nbsp;&nbsp;&nbsp;'+content.time+'</div>'
			html+='<div class="msg-text">'+content.msg+'</div>'
			html+='</div><div style=" clear:both"></div>'
		}else{
			html+='<div class="msg-item-left">'
			html+='<div class="msg-title">'+content.name+'&nbsp;&nbsp;&nbsp;&nbsp;'+content.time+'</div>'
			html+='<div class="msg-text">'+content.msg+'</div>'
			html+='</div>'
		}
		$(".msg-content").append($(html));
		$(".msg-content").scrollTop($(".msg-content")[0].scrollHeight);
	}else if(content.type=="1"){//图片
		var html="";
		if(content.uid==myid){
			html+='<div class="msg-item-right">';
			html+='<div class="msg-title">'+content.name+'&nbsp;&nbsp;&nbsp;&nbsp;'+content.time+'</div>';
			html+='<div class="msg-img"><a href="'+content.path+'" target="_blank" ><img src="'+content.path+'"/></a></div>';
			html+='</div><div style=" clear:both"></div>';
		}else{
			html+='<div class="msg-item-left">'
			html+='<div class="msg-title">'+content.name+'&nbsp;&nbsp;&nbsp;&nbsp;'+content.time+'</div>';
			html+='<div class="msg-img"><a href="'+content.path+'" target="_blank" ><img src="'+content.path+'"/></a></div>';
			html+='</div>';
		}
		$(".msg-content").append($(html));
		$(".msg-content").scrollTop($(".msg-content")[0].scrollHeight);
	}else if(content.type=="2"||content.type=="3"||content.type=="4"){//文件
		var html="";
		if(content.uid==myid){
			html+='<div class="msg-item-right">';
			html+='<div class="msg-title">'+content.name+'&nbsp;&nbsp;&nbsp;&nbsp;'+content.time+'</div>';
			html+='<div class="msg-file"><a href="'+content.path+'" target="_blank" >'+content.msg+'</a></div>';
			html+='</div><div style=" clear:both"></div>';
		}else{
			html+='<div class="msg-item-left">'
			html+='<div class="msg-title">'+content.name+'&nbsp;&nbsp;&nbsp;&nbsp;'+content.time+'</div>';
			html+='<div class="msg-file"><a href="'+content.path+'" target="_blank" >'+content.msg+'</a></div>';
			html+='</div>';
		}
		$(".msg-content").append($(html));
		$(".msg-content").scrollTop($(".msg-content")[0].scrollHeight);
	}
	else{

	}
};
WsClient.prototype.saveMsgToDB=function(data,CommonService,$scope){
	var uriData=JSON.stringify(data);
	CommonService.createOne('chat', uriData, function (data) {

	},function (data, status) {//失败时的回调函数
		if(data){
		$scope.msg=data.message;
		}else{
		$scope.msg='保存消息到数据库失败！';
		}
		$("#msg").modal("show");
	})
};
WsClient.prototype.getMsgsForPC=function(business_id,business_type,count,CommonService,$scope){
	var uriData = "business_id="+business_id+"&business_type="+business_type+"&count="+count;
	CommonService.getAll('chat', uriData, function (data) {
		//处理消息
		var msglist=listToObject(data,'rows').rows;
		var html="";
		for(var i=0;i<msglist.length;i++){
//			msglist[i].time=msglist[i].time.split(' ')[1];
			if(msglist[i].type=="0"){//文本
				if(msglist[i].sender_id==$scope.user.id){
					html+='<div class="msg-item-right">';
					html+='<div class="msg-title">'+msglist[i].sender_name+'&nbsp;&nbsp;&nbsp;&nbsp;'+msglist[i].time+'</div>'
					html+='<div class="msg-text">'+msglist[i].msg+'</div>'
					html+='</div><div style=" clear:both"></div>'
				}else{
					html+='<div class="msg-item-left">'
					html+='<div class="msg-title">'+msglist[i].sender_name+'&nbsp;&nbsp;&nbsp;&nbsp;'+msglist[i].time+'</div>'
					html+='<div class="msg-text">'+msglist[i].msg+'</div>'
					html+='</div>'
				}
			}else if(msglist[i].type=="1"){//图片
				if(msglist[i].sender_id==$scope.user.id){
					html+='<div class="msg-item-right">';
					html+='<div class="msg-title">'+msglist[i].sender_name+'&nbsp;&nbsp;&nbsp;&nbsp;'+msglist[i].time+'</div>';
					html+='<div class="msg-img"><a href="'+msglist[i].path+'" target="_blank" ><img src="'+msglist[i].path+'"/></a></div>';
					html+='</div><div style=" clear:both"></div>';
				}else{
					html+='<div class="msg-item-left">'
					html+='<div class="msg-title">'+msglist[i].sender_name+'&nbsp;&nbsp;&nbsp;&nbsp;'+msglist[i].time+'</div>';
					html+='<div class="msg-img"><a href="'+msglist[i].path+'" target="_blank" ><img src="'+msglist[i].path+'"/></a></div>';
					html+='</div>';
				}
			}else if(msglist[i].type=="2"||msglist[i].type=="3"||msglist[i].type=="4"){//文件
				if(msglist[i].sender_id==$scope.user.id){
					html+='<div class="msg-item-right">';
					html+='<div class="msg-title">'+msglist[i].sender_name+'&nbsp;&nbsp;&nbsp;&nbsp;'+msglist[i].time+'</div>';
					html+='<div class="msg-file"><a href="'+msglist[i].path+'" target="_blank" >'+msglist[i].msg+'</a></div>';
					html+='</div><div style=" clear:both"></div>';
				}else{
					html+='<div class="msg-item-left">'
					html+='<div class="msg-title">'+msglist[i].sender_name+'&nbsp;&nbsp;&nbsp;&nbsp;'+msglist[i].time+'</div>';
					html+='<div class="msg-file"><a href="'+msglist[i].path+'" target="_blank" >'+msglist[i].msg+'</a></div>';
					html+='</div>';
				}
				
			}
			else{
				
			}
		}
        $(".msg-content").css({"font-size":"14px"});
		$(".msg-content").append($(html));
		$(".msg-content").scrollTop($(".msg-content")[0].scrollHeight);
	},function (data, status) {//失败时的回调函数
		if(data){
		$scope.msg=data.message;
		}else{
		$scope.msg='获取消息失败！';
		}
		$("#msg").modal("show");
	})
};

WsClient.prototype.connected = function () {
	return this.opened;
};

WsClient.prototype.close = function (){
	if (this.reconnectTimer) {
		clearTimeout(this.reconnectTimer);
		this.reconnectTimer = null;
	}
	this.closed = true;
	if (this.webSocket) {
		this.webSocket.close();
	}
};

