
/*
===================================================================
Copyright DHTMLX LTD. http://www.dhtmlx.com
This code is obfuscated and not allowed for any purposes except 
using on sites which belongs to DHTMLX LTD.

Please contact sales@dhtmlx.com to obtain necessary 
license for usage of dhtmlx components.
===================================================================
*/
C.prototype.enableKeyboardNavigation = function (mode) {
	this.Tf = K(mode);
	if (this.Tf) {
		if (aq) {
			var z = window.getComputedStyle(this.parentObject, null)["position"];
			if ((z != "absolute") && (z != "relative")) {
				this.parentObject.style.position = "relative";
			}
		}
		this.GV = [["up", 38], ["down", 40], ["open", 39], ["close", 37], ["call", 13], ["edit", 113]];
		var self = this;
		var z = document.createElement("INPUT");
		z.className = "a_dhx_hidden_input";
		z.autocomplete = "off";
		if (window._KHTMLrv) {
			z.style.color = "white";
		}
		this.parentObject.appendChild(z);
		this.parentObject.onkeydown = function (e) {
			if (self.callEvent("onKeyPress", [(e || window.event).keyCode, (e || window.event)])) {
				return self._onKeyDown(e || window.event);
			}
		};
		this.parentObject.onclick = function (e) {
			if (aq) {
				z.select();
			}
			if (window._KHTMLrv || cU) {
				z.focus();
			}
		};
	} else {
		this.parentObject.onkeydown = null;
	}
};
C.prototype._onKeyDown = function (e) {
	var self = this;
	for (var i = 0; i < this.GV.length; i++) {
		if (this.GV[i][1] == e.keyCode) {
			this["_onkey_" + this.GV[i][0]].apply(this, [this.ah()]);
			if (e.preventDefault) {
				e.preventDefault();
			}
			(e || event).cancelBubble = true;
			return false;
		}
	}
	if (this.Mv) {
		return this.Ni(e);
	}
	return true;
};
C.prototype._onkey_up = function (id) {
	var AT = this.ak(id);
	if (!AT) {
		return;
	}
	var next = this.IO(AT);
	if (next.id == this.fO) {
		return;
	}
	this.Jz(next.id);
	this.hE(next.id, false);
};
C.prototype._onkey_down = function (id) {
	var AT = this.ak(id);
	if (!AT) {
		return;
	}
	var next = this.xH(AT);
	if (next.id == this.fO) {
		return;
	}
	this.Jz(next.id);
	this.hE(next.id, false);
};
C.prototype._onkey_open = function (id) {
	this.hx(id);
};
C.prototype._onkey_close = function (id) {
	this.KW(id);
};
C.prototype._onkey_call = function (id) {
	if (this.xS) {
		this.xS();
		this.parentObject.lastChild.focus();
		this.parentObject.lastChild.focus();
		this.hE(id, true);
	} else {
		this.hE(this.ah(), true);
	}
};
C.prototype._onkey_edit = function (id) {
	if (this.WH) {
		this.WH(id);
	}
};
C.prototype.xH = function (item, mode) {
	if ((!mode) && (this.dD(item) > 0)) {
		return item.childNodes[0];
	}
	if ((item.tr) && (item.tr.nextSibling) && (item.tr.nextSibling.mX)) {
		return item.tr.nextSibling.mX;
	}
	if (item.parentObject) {
		return this.xH(item.parentObject, 1);
	}
	return item;
};
C.prototype.IO = function (item) {
	if ((item.tr) && (item.tr.previousSibling) && (item.tr.previousSibling.mX)) {
		return this.Bd(item.tr.previousSibling.mX);
	}
	if (item.parentObject) {
		return item.parentObject;
	} else {
		return item;
	}
};
C.prototype.Bd = function (item) {
	if (this.dD(item) > 0) {
		return this.Bd(item.childNodes[item.aE - 1]);
	} else {
		return item;
	}
};
C.prototype.Ni = function (e) {
	if (e.keyCode == 8) {
		this.pG = "";
		return true;
	}
	var Gb = String.fromCharCode(e.keyCode).toUpperCase();
	if (Gb.match(/[A-Z,a-z,0-9\ ]/)) {
		this.pG += Gb;
		this.pV = true;
		if (!(this.xB() || "").match(RegExp("^" + this.pG, "i"))) {
			this.GE("^" + this.pG, 0);
		}
		this.pV = false;
		if (e.preventDefault) {
			e.preventDefault();
		}
		(e || event).cancelBubble = true;
		return false;
	}
	return true;
};
C.prototype.abL = function (kk) {
	this.GV = kk;
};
C.prototype.enableKeySearch = function (mode) {
	this.Mv = K(mode);
	if (!this.Mv) {
		return;
	}
	this.pG = "";
	var self = this;
	this.ZS = this.qy;
	this.qy = function (node) {
		if (!self.pV) {
			self.pG = "";
		}
		self.ZS(node);
	};
};

