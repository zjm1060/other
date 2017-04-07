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
Ext.ns('com.bitc.xmis.BusinessEntertainment');
com.bitc.xmis.BusinessEntertainment.MEALS_ARRAY = ['中餐', '晚餐'];
com.bitc.xmis.BusinessEntertainment.DetailRecord = Ext.data.Record.create([{
			"mapping" : "form_id",
			"name" : "form_id",
			"type" : "string"
		}, {
			"mapping" : "visitor_company_name",
			"name" : "visitor_company_name",
			"type" : "string"
		}, {
			"mapping" : "visitor_qty",
			"name" : "visitor_qty",
			"type" : "int"
		}, {
			"dateFormat" : "Y-m-d",
			"mapping" : "meals_date",
			"name" : "meals_date",
			"type" : "date"
		}, {
			"mapping" : "meals_type",
			"name" : "meals_type",
			"type" : "int"
		}, {
			"mapping" : "meals_fee_budget",
			"name" : "meals_fee_budget"
		}, {
			"mapping" : "accompany_qty",
			"name" : "accompany_qty",
			"type" : "int"
		}, {
			"mapping" : "cost",
			"name" : "cost"
		}, {
			"mapping" : "profit",
			"name" : "profit"
		}, {
			"mapping" : "cigarettes_wine_amt",
			"name" : "cigarettes_wine_amt"
		}, {
			"mapping" : "total_amt",
			"name" : "total_amt"
		}, {
			"mapping" : "app_datetime",
			"name" : "app_datetime",
			"type" : "string"
		}, {
			"mapping" : "app_user_id",
			"name" : "app_user_id",
			"type" : "int"
		}, {
			"mapping" : "app_user_name",
			"name" : "app_user_name",
			"type" : "string"
		}, {
			"mapping" : "app_deptment",
			"name" : "app_deptment",
			"type" : "int"
		}, {
			"mapping" : "app_deptment_name",
			"name" : "app_deptment_name",
			"type" : "string"
		}, {
			"mapping" : "sign_No",
			"name" : "sign_No",
			"type" : "string"
		}, {
			"mapping" : "remark",
			"name" : "remark",
			"type" : "string"
		}, {
			"mapping" : "is_worked",
			"name" : "is_worked",
			"type" : "int"
		},{
			"mapping" : "proid",
			"name" : "proid",
			"type" : "string"
		}]);
com.bitc.xmis.BusinessEntertainment.BusinessEntertainmentFormPanel = new Ext.form.FormPanel(
		{
			ref : 'form',
			region : 'center',
			padding : 20,
			autoScroll : true,
			border : false,
			labelWidth : 90,
			labelAlign : 'right',
			reader : new Ext.data.JsonReader({
						root : 'rows'
					}, com.bitc.xmis.BusinessEntertainment.DetailRecord),
			items : [{
						xtype : 'panel',
						layout : 'column',
						width : 800,
						border : false,
						items : [{
									xtype : 'panel',
									layout : 'form',
									columnWidth : 0.33,
									labelAlign : 'right',
									padding : '20px 20px 20px 0px ',
									border : false,
									items : [{
												xtype : 'displayfield',
												name : 'app_user_name',
												fieldLabel : '申请人',
												width : 300
											}]
								}, {
									xtype : 'panel',
									layout : 'form',
									columnWidth : 0.33,
									labelAlign : 'right',
									border : false,
									items : [{
												xtype : 'displayfield',
												name : 'app_deptment_name',
												fieldLabel : '申报部门',
												width : 300
											}]
								}, {
									xtype : 'panel',
									layout : 'form',
									columnWidth : 0.33,
									labelAlign : 'right',
									border : false,
									items : [{
												xtype : 'displayfield',
												name : 'app_datetime',
												fieldLabel : '申请时间',
												width : 300,
												allowBlank : false,// 不允许为空
												blankText : '该输入项为必输项'
											}]
								}]
					}, new Ext.Panel({
								height : 20,
								border : false,
								html : ''
							}), {
						xtype : 'fieldset',
						title : '申请人填写',
						width : 800,
						defaults : {
							border : false
						},
						id : 'app_user_fieldset',
						items : [{
							xtype : 'panel',
							layout : 'column',
							items : [{
										xtype : 'panel',
										layout : 'form',
										columnWidth : 0.33,
										labelAlign : 'right',
										border : false,
										items : [{
													xtype : 'textfield',
													name : 'visitor_company_name',
													fieldLabel : '来客单位',
													allowBlank : false,// 不允许为空
													blankText : '该输入项为必输项'
												}]
									}, {
										xtype : 'panel',
										layout : 'form',
										columnWidth : 0.33,
										labelAlign : 'right',
										border : false,
										items : [{
													xtype : 'numberfield',
													name : 'visitor_qty',
													fieldLabel : '来客人数'
												}]
									}, {
										xtype : 'panel',
										layout : 'form',
										columnWidth : 0.33,
										labelAlign : 'right',
										border : false,
										items : [{
													xtype : 'datefield',
													name : 'meals_date',
													fieldLabel : '就餐日期',
													format : 'Y-m-d',
													allowBlank : false,// 不允许为空
													blankText : '该输入项为必输项'
												}]
									}]
						}, {
							xtype : 'panel',
							layout : 'column',
							items : [{
										xtype : 'panel',
										layout : 'form',
										columnWidth : 0.33,
										labelAlign : 'right',
										border : false,
										items : [{
													xtype : 'radiogroup',
													name : 'meals_type',
													fieldLabel : '类别',
													allowBlank : false,// 不允许为空
													blankText : '类别不能为空',
													items : [{
																name : 'meals_type',
																xtype : 'radio',
																inputValue : 0,
																boxLabel : '午餐'
															}, {
																name : 'meals_type',
																xtype : 'radio',
																inputValue : 1,
																boxLabel : '晚餐'
															}]
												}]
									}, {
										xtype : 'panel',
										layout : 'form',
										columnWidth : 0.33,
										labelAlign : 'right',

										border : false,
										items : [{
													xtype : 'formatnum',
													name : 'meals_fee_budget',
													fieldLabel : '就餐标准',
													allowBlank : false,// 不允许为空
													blankText : '该输入项为必输项'
												}]
									}, {
										xtype : 'panel',
										layout : 'form',
										columnWidth : 0.33,
										labelAlign : 'right',
										border : false,
										items : [{
													xtype : 'numberfield',
													name : 'accompany_qty',
													fieldLabel : '陪客人数',
													width : 100,
													allowBlank : false,// 不允许为空
													blankText : '该输入项为必输项'
												}]
									}]
						}, {

							xtype : 'textarea',
							name : 'remark',
							fieldLabel : '备注',
							width : 610,
							height : 40
						}]
					}, {
						xtype : 'fieldset',
						width : 800,
						title : '自动生成',
						id : 'assign_user_fieldset',
						
						items : [{
							xtype : 'textfield',
							name : 'sign_No',
							id: 'sign_No',
							fieldLabel : '签办号',
							readOnly:true
//							,
//							listeners : {
//								'focus' : function(field) {
//									var win = new Ext.Window({
//										title : '签办号的修改',
//										layout : 'border',
//										width : 300,
//										height : 120,
//										modal : true,
//										border : false,
//										items : [new Ext.form.FormPanel({
//											region : 'center',
//											ref : 'p',
//											frame : true,
//											border : false,
//											labelAlign : 'left',
//											labelWidth : 80,
//											bodyStyle : 'padding:10px',
//											buttonAlign : 'center',
//											fbar : [{
//												text : '确定',
//												handler : function() {
//													var sign_No_old = com.bitc.xmis.BusinessEntertainment.BusinessEntertainmentFormPanel
//															.getForm()
//															.findField('sign_No')
//															.getValue();
//													var sign_No_new = win.p.form
//															.findField('sign_No_new')
//															.getValue();
//													// if (sign_No_old ==
//													// sign_No_new) {
//													// Ext.MessageBox.alert('提示',
//													// '您没有修改，点击取消关闭此窗口');
//													// return;
//													// }
//													Ext.Ajax.request({
//														url : __cxtPath
//																+ '/applyManageMaint.do?method=doUpdateSignNoByFormId',
//														method : 'post',
//														params : {
//															sign_No : sign_No_new,
//															form_id : com.bitc.xmis.BusinessEntertainment.BusinessEntertainmentFormPanel
//																	.getForm()
//																	.findField('form_id')
//																	.getValue()
//														},
//														success : function(
//																response,
//																action) {
//
//															var result = Ext
//																	.decode(response.responseText);
//															if (result.success) {
//																Ext.Msg
//																		.alert(
//																				result.messageTitle,
//																				result.message);
//																win.hide();
//																com.bitc.xmis.BusinessEntertainment.BusinessEntertainmentFormPanel
//																		.getForm()
//																		.findField('sign_No')
//																		.setValue(sign_No_new);
//															} else {
//																Ext.Msg
//																		.alert(
//																				result.messageTitle,
//																				result.message);
//															}
//
//														},
//														failure : function(
//																response,
//																action) {
//															var result = Ext
//																	.decode(response.responseText);
//															Ext.MessageBox
//																	.hide();
//															Ext.Msg
//																	.alert(
//																			result.messageTitle,
//																			result.message);
//														}
//													});
//
//												}
//											}, {
//												text : '取消',
//												handler : function() {
//													win.hide();
//												}
//											}],
//											items : [{
//														xtype : 'textfield',
//														name : 'sign_No_new',
//														width : 150,
//														fieldLabel : '签办号'
//
//													}]
//										})]
//									}).show();
//									var sign_No_old = com.bitc.xmis.BusinessEntertainment.BusinessEntertainmentFormPanel
//											.getForm().findField('sign_No')
//											.getValue();
//									win.p.form.findField('sign_No_new')
//											.setValue(sign_No_old);
//
//								}

//							}

						}]
					}, {
						xtype : 'fieldset',
						title : '食堂执行人员填写',
						width : 800,
						id : 'canteen_executor_fieldset',
						defaults : {
							border : false
						},
						items : [{
							xtype : 'panel',
							layout : 'column',
							items : [{
										xtype : 'panel',
										layout : 'form',
										columnWidth : 0.66,
										labelAlign : 'right',
										labelWidth : 90,
										border : false,
										items : [{
													xtype : 'formatnum',
													name : 'cost',
													id: 'cost',
													fieldLabel : '饭菜金额'
												}]
									}, /*{
										xtype : 'panel',
										layout : 'form',
										columnWidth : 0.33,
										labelWidth : 90,
										labelAlign : 'right',
										border : false,
										items : [{
													xtype : 'formatnum',
													name : 'profit',
													id: 'profit',
													fieldLabel : '利润20%'
												}]
									},*/ {
										xtype : 'panel',
										layout : 'form',
										columnWidth : 0.33,
										labelWidth : 90,
										labelAlign : 'right',
										border : false,
										items : [{
													xtype : 'formatnum',
													name : 'cigarettes_wine_amt',
													id: 'cigarettes_wine_amt',
													fieldLabel : '烟酒金额'
												}]
									}]
						}, {
							xtype : 'panel',
							layout : 'column',
							items : [{
										xtype : 'panel',
										layout : 'form',
										columnWidth : 0.66,
										labelAlign : 'right',
										labelWidth : 90,
										border : false,
										items : [{
													xtype : 'displayfield',
													name : 'total_amt_name',
													id:'total_amt_name',
													fieldLabel : '招待总费用'
												}]
									}, {
										xtype : 'panel',
										layout : 'form',
										columnWidth : 0.33,
										labelWidth : 90,
										labelAlign : 'right',
										border : false,
										items : [{
													xtype : 'formatnum',
													name : 'total_amt',
													id:'total_amt',
													fieldLabel : '合计(元)'
												}]
									}]
						}]
					}, {
						xtype : 'hidden',
						name : 'form_id',
						width : 300
					}, {
						xtype : 'hidden',
						name : 'proid',
						width : 300
					},{
						xtype : 'hidden',
						name : 'app_user_id',
						width : 300
					}, {
						xtype : 'hidden',
						name : 'app_deptment',
						width : 300

					}]
		});

Ext.ns('com.bitc.xmis.Combos');
/**
 * 事业部的下拉列表 即一级部门
 * 
 */
com.bitc.xmis.Combos.DivisionDeptCombo = Ext.extend(Ext.form.ComboBox, {
	valueField : 'id',
	displayField : 'name',
	triggerAction : 'all',
	lazyInit : false,
	mode : 'remote',
	emptyText : '选择部门',
	editable : false,
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
					url : String
							.format(
									'{0}/orgMaint.do?method=getAllDivisionDept&&companyID={1}',
									__cxtPath, this.COMPANYID),
					root : 'rows',
					autoLoad : true,
					fields : [{
								name : "id",
								type : "int",
								mapping : "部门ID"
							}, {
								name : "name",
								type : "string",
								mapping : "部门名称"
							}],
					listeners : {
						scope : this,
						'load' : function() {
							this.store.insert(0, new this.store.recordType({
												id : '-1',
												name : '-全部-'
											}));
							this.setValue(this.value);
						}
					}
				});
		com.bitc.xmis.Combos.DivisionDeptCombo.superclass.initComponent
				.call(this);
	}
});
