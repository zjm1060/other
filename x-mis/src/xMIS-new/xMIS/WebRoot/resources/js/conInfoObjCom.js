Ext.ns('com.bitc.xmis.Combos');
/**
 * @class com.bitc.xmis.Combos.conInfoObjCombo 合同基本信息
 * @extends Ext.form.ComboBox
 */
com.bitc.xmis.Combos.ConInfoObjCombo = Ext.extend(Ext.form.ComboBox, {
			fieldLabel : '',
			hiddenName : '',
			width : 0,
			emptyText : '',
			dataUrl : '',
			baseParams : '',
			displayField : 'name',
			valueField : 'ID',
			mode : 'remote',
			triggerAction : 'all',
			lazyInit : false,
			initComponent : function() {
				var url = this.dataUrl;
				this.addEvents('nextlevelLoad');
				this.store = new Ext.data.JsonStore({
							url : __cxtPath + url,
							baseParams : this.baseParams,
							autoLoad : true,
							fields : [{
										name : 'ID',
										mapping : 'ID'
									}, {
										name : 'name',
										type : 'string',
										mapping : 'name'
									}],
							listeners : {
								scope : this,
								'load' : function(s, records, options) {
									this.setValue(this.value);
									this.fireEvent('nextlevelLoad', this);
								},
								'beforeload' : function(s, opt) {
									return (s.baseParams['parentID'] != -1)
											&& (s.baseParams['parentID'] != 0);
								}
							}
						});
				com.bitc.xmis.Combos.ConInfoObjCombo.superclass.initComponent
						.call(this);
			}
		});

Ext.ns('Ext.ux.form');
/**
 * @class Ext.ux.form.FormatNumberField 金额输入框
 * @extends Ext.form.NumberField
 */
Ext.ux.form.FormatNumberField = Ext.extend(Ext.form.NumberField, {
	border : false,
	iValue : null,
	// count : 1,
	value : 0,
	style : 'text-align:right',
	FormatComma : true,
	initComponent : function() {
		Ext.ux.form.FormatNumberField.superclass.initComponent.call(this);
	},
	listeners : {
		'focus' : function() {
			if (this.getValue() == 0) {
				this.setRawValue('');
			}
			this.setRawValue(this.getValue());
		},
		'blur' : function() {
			if (this.getValue() == 0) {
				this.setRawValue(0);
			}
			this.setValue(this.getValue());
			this.iValue = this.getValue();
		}
	},
	initEvents : function() {
		Ext.form.NumberField.superclass.initEvents.call(this);
		var allowed = this.baseChars + '';
		if (this.allowDecimals) {
			allowed += this.decimalSeparator;
		}
		this.stripCharsRe = new RegExp('[^' + allowed + ']', 'gi');
		var keyPress = function(e) {
			var k = e.getKey();
			if (!Ext.isIE
					&& (e.isSpecialKey() || k == e.BACKSPACE || k == e.DELETE)) {
				return;
			}
			var c = e.getCharCode();
			if (allowed.indexOf(String.fromCharCode(c)) === -1) {
				e.stopEvent();
			}
		};
		this.el.on("keypress", keyPress, this);
	},
	validateValue : function(value) {
		if (!Ext.form.NumberField.superclass.validateValue.call(this, value)) {
			return false;
		}
		if (value.length < 1) { // if it's blank
			return true;
		}
		if (this.FormatComma) {
			value = this.removeCommas(String(value));
		}
		value = String(value).replace(this.decimalSeparator, ".");
		if (isNaN(value)) {
			this.markInvalid(String.format(this.nanText, value));
			return false;
		}

		var num = this.parseValue(value);
		if (num < this.minValue) {
			this.markInvalid(String.format(this.minText, this.minValue));
			return false;
		}
		if (num > this.maxValue) {
			this.markInvalid(String.format(this.maxText, this.maxValue));
			return false;
		}
		return true;
	},
	fixPrecision : function(value) {
		var nan = isNaN(value);
		if (!this.allowDecimals || this.decimalPrecision == -1 || nan || !value) {
			return nan ? '' : value;
		}
		return parseFloat(parseFloat(value).toFixed(this.decimalPrecision));
	},

	setValue : function(v) {
		v = typeof v == 'number' ? v : (String(this.removeCommas(v)).replace(
				this.decimalSeparator, "."));
		v = isNaN(v) ? '' : String(v).replace(".", this.decimalSeparator);
		if (String(v).length > 0)
			v = parseFloat(v).toFixed(this.decimalPrecision);
		Ext.form.NumberField.superclass.setValue.call(this, v);
		if (this.FormatComma) {
			v = this.addCommas(v);
			Ext.form.NumberField.superclass.setRawValue.call(this, v);
		}
	},
	parseValue : function(value) {
		if (this.FormatComma)
			value = this.removeCommas(String(value));
		value = parseFloat(String(value).replace(this.decimalSeparator, "."));

		return isNaN(value) ? '' : value;
	},
	// onBlur : function(){
	// this.beforeBlur();
	// if(this.focusClass){
	// this.el.removeClass(this.focusClass);
	// }
	// this.hasFocus = false;
	// if(this.validationEvent !== false && (this.validateOnBlur ||
	// this.validationEvent == 'blur')){
	// this.validate();
	// }
	// var v = this.getValue();
	// if(String(v) !== String(this.startValue)){
	// this.fireEvent('change', this, v, this.startValue);
	// }
	// this.setValue(this.getValue());
	// this.fireEvent('blur', this);
	// this.postBlur();
	// },
	addCommas : function(nStr) {

		nStr += '';
		if (nStr.length == 0)
			return '';
		x = nStr.split('.');
		x1 = x[0];
		x2 = x.length > 1 ? '.' + x[1] : '';
		var rgx = /(\d+)(\d{3})/;
		while (rgx.test(x1)) {
			x1 = x1.replace(rgx, '$1' + ',' + '$2');
		}
		return x1 + x2;

	},
	removeCommas : function(nStr) {
		nStr = nStr + '';
		var r = /(\,)/;
		while (r.test(nStr)) {
			nStr = nStr.replace(r, '');
		}
		return nStr;

	}
});
Ext.reg('formatnum', Ext.ux.form.FormatNumberField);
/**
 * @class com.bitc.xmis.Window.EmpBasicInfoWin 人员基本信息卡片
 * @extends Ext.Window
 */
Ext.ns('com.bitc.xmis.Window');
com.bitc.xmis.Window.EmpBasicInfoWin = Ext.extend(Ext.Window, {
	empID : '',
	title : '',
	width : 360,
	height : 250,
	bosyStyle : 'padding:10px',
	id : 'fullInfoWin',
	defaults : {
		width : 300
	},
	initComponent : function() {
		var empID = this.empID;
		this.items = new com.bitc.xmis.EmpInfoPanel({
			empID : empID,
			lableWidth : 50,
			labelAlign : 'right',
			layout : 'form',
			defaults : {
				xtype : "displayfield"
			},
			items : [{
						name : 'userName',
						fieldLabel : '姓名'
					}, {
						name : 'postList',
						fieldLabel : '职务'
					}, {
						name : 'switchboardNo',
						fieldLabel : '办公电话'
					}, {
						name : 'emailAddress',
						fieldLabel : 'Email'
					}, {
						name : 'mobileNo',
						fieldLabel : '手机号'
					}, {
						name : 'zipCode',
						fieldLabel : '邮政编码'
					}, {
						name : 'address',
						fieldLabel : '办公地址'
					}],
			listeners : {
				'loadsuccess' : function(form, action) {
					var addStr = '';
					var address = action.result.data.address;
					if (!Ext.isEmpty(address)) {
						
						if (address.city.province.area.country.countryName
								.trim() == '中国') {
							addStr = address.city.province.provinceName
									+ address.city.cityName;
							if(address.city.province.provinceName==address.city.cityName){
								addStr = address.city.cityName;
							}		
						} else {
							addStr = address.city.province.area.country.internation.internationName
									+ address.city.province.area.country.countryName
									+ address.city.province.area.areaName
									+ address.city.province.provinceName
									+ address.city.cityName;
						}
					}
					form.findField('address').setValue(addStr);
					var postStr = '';
					for (var i = 0; i < action.result.data.postList.length; i++) {
						if (!Ext.isEmpty(action.result.data.postList[i].post)) {
							postStr = (action.result.data.postList[i].post.name != null)
									? action.result.data.postList[i].post.name
									: '' + ' ';
						}
					}
					form.findField('postList').setValue(postStr);
					if(action.result.attributes.compID!=1){//公司ID 不=1
						form.findField('postList').setValue(action.result.attributes.positionEdit==null?action.result.attributes.positionEdit:action.result.attributes.positionEdit.trim());
					}
				}
			}
		});
		com.bitc.xmis.Window.EmpBasicInfoWin.superclass.initComponent
				.call(this);
	}
});

Ext.ns('com.bitc.xmis.util');
com.bitc.xmis.util.EmpInfoWin = function(empId,titleStr){
		if (!(Ext.isEmpty(empId) || empId == 0)) {
			if (Ext.getCmp('fullInfoWin')) {
				Ext.getCmp('fullInfoWin').close();
			}
			new com.bitc.xmis.Window.EmpBasicInfoWin({
						empID : empId,
						title : titleStr
					}).show();
		}
}
