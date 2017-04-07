Ext.ns('Ext.ux.form');
/**
 * @class Ext.ux.form.AddressChooseField
 * @extends Ext.form.TwinTriggerField 城市选择组件
 * @author
 * @version 1.0
 */
Ext.ux.form.AddressChooseField = Ext.extend(Ext.form.TwinTriggerField, {
	initComponent : function() {
		Ext.ux.form.AddressChooseField.superclass.initComponent.call(this);
		this.addEvents('beforeshowwin');
	},
	setValue : function(v) {
		if (this.emptyText && this.el && !Ext.isEmpty(v)) {
			this.el.removeClass(this.emptyClass);
		}
		Ext.form.TextField.superclass.setValue.apply(this, arguments);
		this.applyEmptyText();
		return this;
	},
	validationEvent : false,
	validateOnBlur : false,
	trigger1Class : 'x-form-clear-trigger',
	trigger2Class : 'x-form-search-trigger',
	singleExpand : true,
	hideTrigger1 : true,

	singleSelect : true,

	winConfig : {},

	baseParamFun : function() {
		return {};
	},

	/**
	 * 返回选择结果的回调函数
	 */
	setValueFun : function() {
	},
	/**
	 * 获取默认值，返回结果为数组，依次为internationID，countryID，areaID，provinceID，cityID
	 */
	getValueFun : function() {
	},
	clearValueFun : function() {
	},

	lastIds : [],
	specifiedselect : '',
	hasInit : false,

	initWindow : function() {
//		if (this.hasInit === true) {
//			return;
//		}

		var triggerField = this;
		// internation
		var internationDataRecord = Ext.data.Record.create([{
					name : 'id',
					type : "int",
					mapping : 'internationID'
				}, {
					name : 'cname',
					type : "string",
					mapping : 'internationName'
				}]);
		// country
		var countryDataRecord = Ext.data.Record.create([{
					name : 'id',
					type : "int",
					mapping : 'countryID'
				}, {
					name : 'cname',
					type : "string",
					mapping : 'countryName'
				}, {
					name : 'areaCode',
					type : "string",
					mapping : 'areaCode'
				}]);
		// area
		var areaDataRecord = Ext.data.Record.create([{
					name : 'id',
					type : "int",
					mapping : 'areaID'
				}, {
					name : 'cname',
					type : "string",
					mapping : 'areaName'
				}]);
		var provinceDataRecord = Ext.data.Record.create([{
					name : 'id',
					type : "int",
					mapping : 'provinceID'
				}, {
					name : 'cname',
					type : "string",
					mapping : 'provinceName'
				}]);
		var cityDataRecord = Ext.data.Record.create([{
					name : 'id',
					type : "int",
					mapping : 'cityID'
				}, {
					name : 'cname',
					type : "string",
					mapping : 'cityName'
						}, {
					name : 'areaCode',
					type : "string",
					mapping : 'areaCode'
				}]);
		var cityAreaCodeDataRecord = Ext.data.Record.create([{
					name : 'id',
					type : "int",
					mapping : 'cityID'
				}, {
					name : 'cname',
					type : "string",
					mapping : 'areaCode'
				}]);
		var countryAreaCodeDataRecord = Ext.data.Record.create([{
					name : 'id',
					type : "int",
					mapping : 'countryID'
				}, {
					name : 'cname',
					type : "string",
					mapping : 'areaCode'
				}]);

		// internation
		var internationData = new Ext.data.Store({
			proxy : new Ext.data.HttpProxy({
				url : __cxtPath
						+ '/crmMaint.do?method=getMultiData&category=internation',
				method : "POST"
			}),
			reader : new Ext.data.JsonReader({
						root : 'rows',
						totalProperty : 'totalCount',
						idProperty : 'id'
					}, internationDataRecord)
					,
			listeners : {
				'load' : function(store,record){
					var ids = triggerField.getValueFun.call(this);
					for(var i=0;i<store.data.length;i++){
						if(record[i].data.id==ids[0]){
							formpanel.getForm().findField('internationID').setValue(ids[0]);
						}
					}
				}
			}
		});
		// country
		var countryData = new Ext.data.Store({
			proxy : new Ext.data.HttpProxy({
						url : __cxtPath
								+ '/crmMaint.do?method=getMultiData&category=country',
						method : "POST"
					}),
			reader : new Ext.data.JsonReader({
						root : 'rows',
						totalProperty : 'totalCount'
					}, countryDataRecord)
					,
			listeners : {
				'load' : function(store,record){
					var ids = triggerField.getValueFun.call(this);
					for(var i=0;i<store.data.length;i++){
						if(record[i].data.id==ids[1]){
							formpanel.getForm().findField('countryID').setValue(ids[1]);
						}
					}			
				}
			}
		});

		// area
		var areaData = new Ext.data.Store({
			proxy : new Ext.data.HttpProxy({
						url : __cxtPath
								+ '/crmMaint.do?method=getMultiData&category=area',
						method : "POST"
					}),
			reader : new Ext.data.JsonReader({
						root : 'rows',
						totalProperty : 'totalCount'
					}, areaDataRecord)
					,
			listeners : {
				'load' : function(store,record){
					var ids = triggerField.getValueFun.call(this);
					for(var i=0;i<store.data.length;i++){
						if(record[i].data.id==ids[2]){
						formpanel.getForm().findField('areaID').setValue(ids[2]);
					}
					}
				}
			}
		});
		var provinceData = new Ext.data.Store({
			proxy : new Ext.data.HttpProxy({
						url : __cxtPath
								+ '/crmMaint.do?method=getMultiData&category=province',
						method : "POST"
					}),
			reader : new Ext.data.JsonReader({
						root : 'rows',
						totalProperty : 'totalCount'
					}, provinceDataRecord)
					,
			listeners : {
				'load' : function(store,record){
					var ids = triggerField.getValueFun.call(this);
					for(var i=0;i<store.data.length;i++){
						if(record[i].data.id==ids[3]){
							formpanel.getForm().findField('provinceID').setValue(ids[3]);
						}
					}
				}
			}
		});
		var cityData = new Ext.data.Store({
			proxy : new Ext.data.HttpProxy({
						url : __cxtPath
								+ '/crmMaint.do?method=getMultiData&category=city'
					}),
			reader : new Ext.data.JsonReader({
						root : 'rows',
						totalProperty : 'totalCount'
					}, cityDataRecord)
					,
			listeners : {
				'load' : function(store,record){
					var ids = triggerField.getValueFun.call(this);
					for(var i=0;i<store.data.length;i++){
						if(record[i].data.id==ids[4]){
							formpanel.getForm().findField('cityID').setValue(ids[4]);
						}
					}
				}
			}
		});
		
		var cityAreaCodeData = new Ext.data.Store({
			proxy : new Ext.data.HttpProxy({
						url : __cxtPath
								+ '/crmMaint.do?method=getMultiData&category=city'
					}),
			reader : new Ext.data.JsonReader({
						root : 'rows',
						totalProperty : 'totalCount'
					}, cityAreaCodeDataRecord)
		});
		var countryAreaCodeData = new Ext.data.Store({
			proxy : new Ext.data.HttpProxy({
						url : __cxtPath
								+ '/crmMaint.do?method=getMultiData&category=country'
					}),
			reader : new Ext.data.JsonReader({
						root : 'rows',
						totalProperty : 'totalCount'
					}, countryAreaCodeDataRecord)
		});

		var label1 = new Ext.form.Label({
			id:"labelID",                             
			width:100,                         
			hideMode:"visibility",//默认display,可以取值：display，offsets，visibility                             
			html:"<font color='black' size='2px'><b>请选择洲际：</b></font>"            
		});
		var label2 = new Ext.form.Label({
			id:"labelID",                             
			width:100,                           
			hideMode:"visibility",//默认display,可以取值：display，offsets，visibility                             
			html:"<font color='black' size='2px'><b>请选择国家：</b></font>"            
		});
		var label3 = new Ext.form.Label({
			id:"labelID",                             
			width:100,                           
			hideMode:"visibility",//默认display,可以取值：display，offsets，visibility                             
			html:"<font color='black' size='2px'><b>请选择地区：</b></font>"            
		});
		var label4 = new Ext.form.Label({
			id:"labelID",                             
			width:100,                           
			hideMode:"visibility",//默认display,可以取值：display，offsets，visibility                             
			html:"<font color='black' size='2px'><b>请选择省份：</b></font>"            
		});
		var label5 = new Ext.form.Label({
			id:"labelID",                             
			width:100,                           
			hideMode:"visibility",//默认display,可以取值：display，offsets，visibility                             
			html:"<font color='black' size='2px'><b>请选择城市：</b></font>"            
		});
		
		var formpanel = new Ext.form.FormPanel({
					title : '',
					width : 900,
					region : 'center',
					bodyStyle : 'padding:10px ;',
					frame : true,
					border : false,
//					autoScroll : true,
					items : [{
								xtype : 'panel',
								border : false,
								layout : 'column',
								items : [{
											columnWidth : .2,
											layout : 'form',
											labelWidth : 10,
											items : [{
														xtype : 'multiselect',
														tbar:[label1],
														valueField : 'id',
														displayField : 'cname',
														name : 'internationID',
														width : 160,
														height : 240,
														allowBlank : true,
														maxSelections : 1,
														store : internationData
													}]
										}, {
											columnWidth : .2,
											layout : 'form',
											labelWidth : 10,
											items : [{
														xtype : 'multiselect',
														tbar:[label2],
														valueField : 'id',
														displayField : 'cname',
														name : 'countryID',
														width : 160,
														height : 240,
														allowBlank : true,
														maxSelections : 1,
														store : countryData
													}]
										}, {
											columnWidth : .2,
											layout : 'form',
											labelWidth : 10,
											items : [{
														xtype : 'multiselect',
											            tbar:[label3],
														valueField : 'id',
														displayField : 'cname',
														name : 'areaID',
														width : 160,
														height : 240,
														allowBlank : true,
														maxSelections : 1,
														store : areaData

													}]
										}, {
											columnWidth : .2,
											layout : 'form',
											labelWidth : 10,
											items : [{
														xtype : 'multiselect',
											            tbar:[label4],
														valueField : 'id',
														displayField : 'cname',
														name : 'provinceID',
														width : 160,
														height : 240,
														allowBlank : true,
														maxSelections : 1,
														store : provinceData
													}]
										}, {
											columnWidth : .2,
											layout : 'form',
											labelWidth : 10,
											items : [{
														xtype : 'multiselect',
											            tbar:[label5],
														name : 'cityID',
														valueField : 'id',
														displayField : 'cname',
														width : 160,
														autoScroll:true,
														height : 240,
														maxSelections : 1,
														store : cityData
													}]
										}]
							}],
					loadInternationData : function() {
						internationData.load();
					}
				});

		
		// 监听 开始
		formpanel.getForm().findField('internationID').on('change',
				function(ms, value) {
						formpanel.getForm().findField('countryID').store.load({
								params : {
									fid : value
								}
							});
				});

		formpanel.getForm().findField('countryID').on('change',
				function(ms, value) {
					formpanel.getForm().findField('areaID').store.load({
								params : {
									fid : value
								}
							});
				});

		formpanel.getForm().findField('areaID').on('change',
				function(ms, value) {
					formpanel.getForm().findField('provinceID').store.load({
								params : {
									fid : value
								}
							});
				});

		formpanel.getForm().findField('provinceID').on('change',
				function(ms, value) {
					formpanel.getForm().findField('cityID').store.load({
								params : {
									fid : value
								}
							});
				});
		// 监听 结束
		this.formpanel = formpanel;
		var winConfig = this.winConfig;
		var wintbar = ['地址查询：',new Ext.ux.form.SearchField({
					width : 140,
					emptyText : '请输入关键字',
					onRender : function(ct, position) {
						Ext.ux.InklingSearchField.superclass.onRender.call(
								this, ct, position);
						if (this.tooltip && this.tooltip.text)
							new Ext.ToolTip({
										target : this.id,
										trackMouse : false,
										draggable : false,
										maxWidth : 200,
										minWidth : 100,
										title : this.tooltip.title,
										html : this.tooltip.text
									});
					},
					tooltip : {
						title : '地址查询',
						text : '输入查询洲际名称、国家名称、地区名称、省份名称、城市名称中的一个关键字进行查询'
					},
					onTrigger1Click : function() {
							this.el.dom.value = '';
							this.triggers[0].hide();
					},
					onTrigger2Click : function() {
						 Ext.Ajax.request({
									url : __cxtPath
											+ '/crmMaint.do?method=getAddressIDListByName',
									method : 'post',
									params : {
										name : this.el.dom.value
									},
									success : function(response, action) {
										var result = Ext.decode(response.responseText);
										var selectIds = result.attributes.selectIds;
										if(selectIds.indexOf(",")>0){
											var ids = selectIds.split(",");
											formpanel.getForm().findField('internationID').setValue(ids[0]);
											formpanel.getForm().findField('countryID').setValue(ids[1]);
											formpanel.getForm().findField('areaID').setValue(ids[2]);
											formpanel.getForm().findField('provinceID').setValue(ids[3]);
											formpanel.getForm().findField('cityID').setValue(ids[4]);
										} else {
											Ext.MessageBox.alert("提示", "未查到此地名！");
										}
									},
									failure : function(response, action) {
										Ext.MessageBox.hide();
										Ext.MessageBox.alert("提示", "操作失败！");
									}
								});
						 this.triggers[0].show();
					}
				}),'-','<font color ="#003366">* 城市选择到地级市，县级市请在"详细地址"中输入 *</font>','->',{
			xtype : 'button',
			ref : '../okBtn',
			text : '<b>选定</b>',
			iconCls : 'ok',
			scope : this,
			handler : function() {
				function getSelectedString(field) {
					var selected = field.view.getSelectedIndexes();
					if (selected.length > 0) {
						return field.store.getAt(selected[0]).get('cname')
					} else {
						return '';
					}
				}
				function getSelectedAreaCode(field) {
					var selected = field.view.getSelectedIndexes();
					if (selected.length > 0) {
						return field.store.getAt(selected[0]).get('areaCode')
					} else {
						return '';
					}
				}
				var name="";
				var isTrue = (getSelectedString(formpanel.getForm()
								.findField('provinceID')))==(getSelectedString(formpanel.getForm()
								.findField('cityID')));
				var areaCodes=getSelectedAreaCode(formpanel.getForm().findField('countryID'))+','+ getSelectedAreaCode(formpanel.getForm().findField('cityID'));
				if(isTrue){
					name = getSelectedString(formpanel.getForm()
							.findField('internationID'))
							+'·'+ getSelectedString(formpanel.getForm()
									.findField('countryID'))
							+'·'+ getSelectedString(formpanel.getForm()
									.findField('areaID'))
							+'·'+ getSelectedString(formpanel.getForm()
									.findField('provinceID'));
				}else{
					name = getSelectedString(formpanel.getForm()
							.findField('internationID'))
							+'·'+ getSelectedString(formpanel.getForm()
									.findField('countryID'))
							+'·'+ getSelectedString(formpanel.getForm()
									.findField('areaID'))
							+'·'+ getSelectedString(formpanel.getForm()
									.findField('provinceID'))
							+'·'+ getSelectedString(formpanel.getForm()
									.findField('cityID'));
				}
				ids = {
					internationID : formpanel.getForm()
							.findField('internationID').getValue(),
					countryID : formpanel.getForm().findField('countryID')
							.getValue(),
					areaID : formpanel.getForm().findField('areaID').getValue(),
					provinceID : formpanel.getForm().findField('provinceID')
							.getValue(),
					cityID : formpanel.getForm().findField('cityID').getValue()
				};
				this.setValueFun.call(this, name, ids,areaCodes);
				this.window.hide();
//				this.triggers[0].show();
			}
		}, '-', {
			ref : '../cancelBtn',
			iconCls : 'remove',
			text : '<b>取消</b>',
			scope : this,
			handler : function() {
				this.window.hide();
				
			}
		}];
		if (winConfig.tbar) {
			wintbar.push('-');
			wintbar.push(winConfig.tbar);
		}

		var windowConfig = {
			items : [formpanel],
			modal : true,
			tbar : {
				xtype : 'toolbar',
//				buttonAlign: 'right', 
				style :'padding-left:5px',
				items : wintbar
			}
		};

		Ext.applyIf(windowConfig, winConfig);

		this.window = new Ext.AddressChooseWindow(windowConfig);
//		this.hasInit = true;
	},

	onTrigger1Click : function() {
		this.clearValueFun();
		this.triggers[0].hide();
	},

	onTrigger2Click : function() {
		this.initWindow();
		if (!this.fireEvent('beforeshowwin', this)) {
			return;
		}
		var formpanel = this.formpanel;
		// 读取页面的隐藏的洲际ID、国家ID、地区ID、省ID、市ID
		formpanel.loadInternationData();
//		var ids = this.getValueFun.call(this);
//			formpanel.getForm().findField('internationID').setValue(ids[0]);
//			formpanel.getForm().findField('countryID').setValue(ids[1]);
//			formpanel.getForm().findField('areaID').setValue(ids[2]);
//			formpanel.getForm().findField('provinceID').setValue(ids[3]);
//			formpanel.getForm().findField('cityID').setValue(ids[4]);
		this.window.show();

	},
	//清除资源
	beforeDestroy : function() {
		Ext.ux.form.AddressChooseField.superclass.beforeDestroy.call(this);
		if (this.window) {
			Ext.destroy(this.window);
		}
	}

});
Ext.reg('addressChooseField', Ext.ux.form.AddressChooseField);

Ext.ns('Ext');
Ext.AddressChooseWindow = Ext.extend(Ext.Window, {
			initComponent : function() {
				Ext.AddressChooseWindow.superclass.initComponent.call(this);
			},
			title : '',
			width : 950,
			height : 310,
			closeAction : 'hide',
			closable : true,
			layout : 'border'
		});
