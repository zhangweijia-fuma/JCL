
function Listeners(listeners, scope) {
	this.scope = scope;
	this.listeners = listeners;
};

Listeners.prototype.setListeners = function (listeners) {
	this.listeners = listeners;
};

Listeners.prototype.getListeners = function () {
	return this.listeners;
};

Listeners.prototype.setScope = function (scope) {
	this.scope = scope;
};

Listeners.prototype.getScope = function () {
	return this.scope;
};

Listeners.prototype.invoke = function (lname){
	if (!this.listeners) {
		return;
	}
	var fn = this.listeners[lname];
	if (!fn) {
		return;
	}
	var args = [];
	for (var i = 1; i < arguments.length; i++) {
		args[i - 1] = arguments[i];
	}
	if (this.scope) {
		fn.apply(this.scope, args);
	} else {
		fn.apply(window, args);
	}
};
