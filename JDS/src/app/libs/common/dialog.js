/**
 * Copyright 2016, Unicosense Co., Ltd
 * All rights reserved.
 *
 * @common dialog
 *
 */

/*
 usage :
	html :
		1. add common html in index.html, share one dialog
			<div class='modal' id='mb-common' tabindex='-1' role='dialog' aria-labelledby='resizedModalTitle' aria-hidden='true'>
				<div class='xMask'></div>
				<div class='modal-dialog'>
					<div class='modal-content'>
						<div class='modal-header'>
							<button type='button' class='close' data-dismiss='modal' aria-hidden='true' onclick='onMbClickCancel()'>&times;</button>
							<h4 class='modal-title' id='resizedModalTitle'><i class='icon' id='mb-icon'></i> <a id='mb-title'>标题</a></h4>
						</div>
						<div class='modal-body'>
							<form class='form-horizontal' role='form' method='post'>
								<div class='container'>
									<p id='mb-msg'>消息内容</p>
								</div>
							</form>
						</div>
						<div class='modal-footer'>
							<button type='button' class='btn btn-mini btn-default' onclick='onMbClickCancel()' id='mb-btn-cancel'>取消</button>
							<button type='button' class='btn btn-mini btn-success' onclick='onMbClickOK()' id='mb-btn-ok'>确认</button>
						</div>
					</div>
				</div>
			</div>

	js :
		MsgBox.xxx(...)
 */

var curMbInstance;

 function UsMsgBox(options) {
	if (!(this instanceof UsMsgBox)){
		return new UsMsgBox(options);
	}

	if (curMbInstance) {
		E("curMbInstance is valid");
	}

	this.options = options = options || {};
	options = this.checkOptions(options);
	this.initDialog(options);
	$(options.$mbid).modal("show");
	$(options.$mbid).show();

	curMbInstance = this;
};

UsMsgBox.prototype.initDialog = function(options) {

	$(options.$mbid + " #mbIcon").attr("class", "icon " + options.iconCls);
	$(options.$mbid + " #mbTitle").html(options.title);
	$(options.$mbid + " #mbBtnCancel").html(options.negativeText);
	$(options.$mbid + " #mbBtnOk").html(options.positiveText);

	if (options.confirm) {
		$(options.$mbid + " #mbBtnCancel").show();
	} else {
		$(options.$mbid + " #mbBtnCancel").hide();
	}

	var $msg = $(options.$mbid + " #mbMsg");
	var $prompt = $(options.$mbid + " #mbPrompt");

	if (options.prompt) {
		$msg.hide();
		$prompt.show();

		var $input = $(options.$mbid + " #mbInput");
		$input.attr("placeholder", options.promptHolder);
		$input.val(options.promptText);

		if (options.promptLabel)
			$(options.$mbid + " #mbInputLabel").html(options.promptLabel);
		else
			$(options.$mbid + " #mbInputLabel").html("");

		var _this = this;
		TO(function () {
			var el = $input.get(0)
			el.focus();
			el.onkeyup = function(e) {
				if (e.keyCode == 13) {
					_this.onClickOK();
				}
			};
		});
	} else {
		$prompt.hide();
		$msg.show();
		$msg.html(options.msg);
	}

	$(options.$mbid).attr("class", "modal " + options.mainStyle);
};

UsMsgBox.prototype.checkOptions = function(options) {

	if (!options.id) {
		options.mbid = "commonDialog";
	} else {
		options.mbid = options.id;
	}
	options.$mbid = "#" + options.mbid + "";
	if (!options.title) {
		options.title = "消息";
	}
	if (!options.msg) {
		options.msg = ".....................";
	}
	if (!options.confirm) {
		options.confirm = false;
	}
	if (!options.positiveText) {
		options.positiveText = options.confirm ? "确认" : "关闭";
	}
	if (!options.negativeText) {
		options.negativeText = "取消";
	}
	if (!options.iconCls) {
		options.iconCls = options.confirm ? "icon-question-sign" : "icon-info";
	}
	if (!options.animation) {
		options.animation = false;
	}
	if (!options.mainStyle) {
		options.mainStyle = "";
	}
	if (!options.size) {
		//options.size = "lg";
		//options.size = "sm";
		options.size = undefined;
	}
	if (!options.scope) {
		options.scope = window;
	}

	if (options.prompt) {
		if (!options.promptHolder) {
			options.promptHolder = "请输入内容";
		}
	}
	options.callback = options.callback || nullFn;
	return options;
};

UsMsgBox.prototype.callback = function() {
	this.options.callback.apply(this.options.scope, arguments);
};

UsMsgBox.prototype.onClickCancel = function(e) {
	if (this.frozen)
		return false;
	$(this.options.$mbid).modal("hide");
	this.callback(false);
	return true;
};

UsMsgBox.prototype.onClickOK = function(e) {
	if (this.frozen)
		return false;
	$(this.options.$mbid).modal("hide");
	if (this.options.prompt) {
		this.callback(true, $(this.options.$mbid + " #mbInput").val());
	} else {
		this.callback(true);
	}
	return true;
};

UsMsgBox.prototype.setScope = function(scope) {
	this.scope = scope;
};

UsMsgBox.prototype.setFrozen = function(scope) {
	this.frozen = frozen;
};

var MsgBox = {
	confirm : function (title, msg, callback, positiveText, negativeText, mainStyle) {
		return UsMsgBox({
			title : title,
			msg : msg,
			confirm : true,
			callback : callback,
			positiveText : positiveText,
			negativeText : negativeText,
			mainStyle : mainStyle,
		});
	},
	info : function (title, msg, callback) {
		if (!msg) {
			msg = title;
			title = "提示";
		}
		return UsMsgBox({
			title : title,
			msg : msg,
			confirm : false,
			callback : callback
		});
	},
	success : function (title, msg, callback) {
		if (!msg) {
			msg = title;
			title = "成功";
		}
		this.info(title, msg, callback);
	},
	error : function (title, msg, callback) {
		if (!msg) {
			msg = title;
			title = "错误";
		}
		return UsMsgBox({
			title : title,
			msg : msg,
			confirm : false,
			iconCls : "icon-error",
			callback : callback,
			mainStyle : "error"
		});
	},
	prompt : function (title, holder, label, text, callback) {
		return UsMsgBox({
			title : title,
			confirm : true,
			prompt : true,
			promptHolder : holder,
			promptLabel : label,
			promptText : text,
			callback : callback
		});
	},
	dialog : function (options) {
		return UsMsgBox(options);
	},
	hide : function () {
		this.onClickCancel();
	},
	onClickCancel : function () {
		// D("onMbClickCancel");
		if (curMbInstance) {
			if (curMbInstance.onClickCancel(this)) {
				curMbInstance = null;
			}
		}
	},
	onClickOK : function () {
		// D("onMbClickOK");
		if (curMbInstance) {
			if (curMbInstance.onClickOK(this)) {
				curMbInstance = null;
			}
		}
	}
};

//common dialog base
//see more @his.js
function DialogBase () {
	this.modelId = null;
	this.$scope = null;
};

DialogBase.prototype._adjustPosition = function() {
	$(this.modelId).modal("ajustPosition", "fit");
};

DialogBase.prototype.adjustPosition = function() {
	var _this = this;
	TO(function () {
		_this._adjustPosition();
	});
};

DialogBase.prototype.show = function() {
	var o = {
		show : true,
		position : 100
	};
	$(this.modelId).modal("show");
};

DialogBase.prototype.hide = function() {
	$(this.modelId).modal("hide");
};

DialogBase.prototype.getHisPatImage = function(pat, ageF, sexF) {
	if (!pat.age) {
		if (!this.curDate) {
			this.curDate = new Date();
		}
		if (pat.date_of_birth) {
			var m = pat.date_of_birth.match(/.*(\d+)\/(\d+)\/(\d+).*/);
			if (!m) {
				m = pat.date_of_birth.match(/(\d+)-(\d+)-(\d+)/);
			}
			if (m) {
				var bd = new Date(m[1], m[2], m[3]);
				var y = parseInt(m[1]);
				var m = parseInt(m[2]);
				var d = parseInt(m[3]);
				var date = this.curDate;
				var age = date.getYear() - y + 1900;
				if (date.getMonth() < m && date.getDate() < d) {
					age--;
				}
				if (age >= 0 && age < 200) {
					pat.age = age;
				}
			}
		}
		if (pat.age == undefined) {
			pat.age = 25;
		}
	}
	return this.$scope.getUserImage(pat, ageF, sexF);
};

function EditSurveyQuestion($scope, CommonService, modelId, item, listeners) {
	if (!(this instanceof EditSurveyQuestion)){
		return new EditSurveyQuestion($scope, CommonService, modelId, item, listeners);
	}

	var _this = this;
	this.$scope = $scope;
	this.modelId = "#" + modelId;
	var listeners = this.listeners = new Listeners(listeners);

	copyObject(item, this);

};

EditSurveyQuestion.prototype = new DialogBase();
