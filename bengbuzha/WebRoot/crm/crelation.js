Ext.onReady(function() {
	Ext.QuickTips.init();
	var exportname = '公司列表';
	var brCombo = new com.bitc.xmis.Combos.BusinessRelationComboAll({
				fieldLabel : '<font color="red">*</font>业务关系',
				width : 100,
				hiddenName : 'businessRelationID',
				listeners : {
					'select' : function(combo, record, index) {
						var selValue = record.get('id');
						// if (selValue == '-1') {
						// delete store.baseParams['businessID'];
						// } else {
						store.setBaseParam('businessID', selValue);
						// }
						// store.load({
						// params : {
						// start : 0
						// }
						// });
					}
				}
			});

	// reRecord org_公司
	var reRecord = Ext.data.Record.create([{
				"mapping" : "公司ID",
				"name" : "companyID",
				"type" : "int"
			}, {
				"name" : "公司名称",
				"type" : "string"
			}, {
				"name" : "关系名称",
				"type" : "string"
			}, {
				"name" : "电话1",
				"type" : "string"
			}, {
				"name" : "传真1",
				"type" : "string"
			}, {
				"name" : "电话2",
				"type" : "string"
			}, {
				"name" : "传真2",
				"type" : "string"
			}, {
				"name" : "网址",
				"type" : "string"
			}, {
				"name" : "邮箱",
				"type" : "string"
			}, {
				"name" : "开户银行",
				"type" : "string"
			}, {
				"name" : "账号",
				"type" : "string"
			}, {
				"name" : "税号",
				"type" : "string"
			}, {
				"mapping" : "注册资金",
				"name" : "注册资金",
				"type" : "string"
			}, {
				"mapping" : "信用等级名称",
				"name" : "信用等级名称",
				"type" : "string"
			}, {
				"mapping" : "客户级别名称",
				"name" : "客户级别名称",
				"type" : "string"
			}, {
				"mapping" : "机构代码",
				"name" : "机构代码",
				"type" : "string"
			}, {
				"mapping" : "备注",
				"name" : "备注",
				"type" : "string"
			}, {
				"mapping" : "法人代表",
				"name" : "法人代表",
				"type" : "auto"
			}, {
				"mapping" : "联系人",
				"name" : "联系人",
				"type" : "auto"
			}, {
				"mapping" : "公司规模名称",
				"name" : "公司规模名称",
				"type" : "auto"
			}, {
				"mapping" : "洲际",
				"name" : "internation",
				"type" : "string"
			}, {
				"mapping" : "国家及地区",
				"name" : "国家及地区",
				"type" : "string"
			}, {
				"mapping" : "省名称",
				"name" : "省名称",
				"type" : "string"
			}, {
				"mapping" : "城市名称",
				"name" : "城市名称",
				"type" : "string"
			}, {
				"mapping" : "修改日期",
				"name" : "修改日期",
				"type" : "string"
			}, {
				"mapping" : "修改人",
				"name" : "修改人",
				"type" : "string"
			}, {
				"mapping" : "最后修改日期",
				"name" : "最后修改日期",
				"type" : "string"
			}, {
				"mapping" : "最后修改人",
				"name" : "最后修改人",
				"type" : "string"
			}, {
				"mapping" : "收入金额",
				"name" : "收入金额",
				"type" : "string"
			}, {
				"mapping" : "支出金额",
				"name" : "支出金额",
				"type" : "string"
			}]);
	// store org_公司
	var store = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : __cxtPath
									+ '/crmMaint.do?method=getCompanyListStoreByQuery',
							method : "POST"
						}),
				baseParams : {
					limit : 20
				},
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount'
						}, reRecord),
				sortInfo : {
					field : '公司名称',
					direction : 'ASC'
				},
				remoteSort : true
			});
	// columnmodel org_公司
	var csm = new Ext.grid.CheckboxSelectionModel();
	var cm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({width :40}), {
				header : "公司ID",
				hidden : true,
				dataIndex : 'companyID'
			}, {
				header : "公司名称",
				sortable : true,
				width:180,
				dataIndex : '公司名称',
				renderer : function(value, metaData, record) {
					return '<a href="javascript:;" onclick="parent.xMis.turn(\'company-info-'
							+ record.get('companyID')
							+ '\',\''
							+ '[CRM]'
							+ value
							+ '\',\''
							+ __cxtPath
							+ '/crmMaint.do?method=getCompanyInfo&companyID='
							+ record.get('companyID')
							+ '\',\'menu-company-info\')">' + value + '</a>';
				}
			}, {
				dataIndex : '关系名称',
				sortable : true,
				header : '业务关系'
		}	, {
				dataIndex : '联系人',
				sortable : true,
				header : '联系人'
			}, {
				dataIndex : '公司规模名称',
				sortable : true,
				header : '公司规模'
			}, {
				dataIndex : '收入金额',
				align : 'right',
				sortable : true,
				header : '签订收入合同总金额<br>(约xxx万元)',
				width :150,
				renderer : function(value, cellmeta, record) {
					if (value.trim() == '' || value.trim() == '/') {
						return '--';
					} else {
//						cellmeta.attr = 'ext:qtitle="提示" ext:qtip="<font color=green>收入性合同金额</font>"';
						return '<font  color="green">' + formatMillion(value) + '</font>';
					}

				}
			}, {
				dataIndex : '支出金额',
				align : 'right',
				sortable : true,
				width :150,
				header : '签订支出合同总金额<br>(约xxx万元)'
				,
				renderer : function(value, cellmeta, record) {
					if (value.trim() == '' || value.trim() == '/') {
						return '--';
					} else {
//						cellmeta.attr = 'ext:qtitle="提示" ext:qtip="<font  color=#CC0000>支出性合同金额</font>"';
						return '<font color="#CC0000">'
								+ formatMillion(value) + '</font>';
					}

				}
			}, {
				dataIndex : '国家及地区',
				sortable : true,
				hidden :true,
				header : '国家及地区'
			}, {
				dataIndex : '省名称',
				sortable : true,
				header : '省名称'
			}, {
				dataIndex : '城市名称',
				sortable : true,
				header : '城市名称'
			}, {
				dataIndex : '电话1',
				sortable : true,
				header : '电话一'
			}, {
				dataIndex : '电话2',
				sortable : true,
				header : '电话二'
			}, {
				dataIndex : '传真1',
				sortable : true,
				header : '传真一'
			}, {
				dataIndex : '传真2',
				sortable : true,
				header : '传真二'
			}, {
				dataIndex : '网址',
				sortable : true,
				header : '网址'
			}, {
				dataIndex : '邮箱',
				sortable : true,
				header : '邮箱'
			}, {
				dataIndex : '开户银行',
				sortable : true,
				header : '开户银行'
			}, {
				dataIndex : '账号',
				sortable : true,
				header : '账号'
			}, {
				dataIndex : '税号',
				sortable : true,
				header : '税号'
			}, {
				dataIndex : '法人代表',
				sortable : true,
				header : '法人代表'
			}, {
				dataIndex : '客户级别名称',
				sortable : true,
				header : '客户级别'
			}, {
				dataIndex : '信用等级名称',
				sortable : true,
				header : '信用等级'
			}, {
				dataIndex : '机构代码',
				sortable : true,
				header : '机构代码'
			}, {
				dataIndex : '注册资金',
				sortable : true,
				header : '注册资金'
			}, {
				dataIndex : '备注',
				sortable : true,
				header : '备注'
			}, {
				dataIndex : '修改人',
				sortable : true,
				hidden : true,
				header : '创建人'
			}, {
				dataIndex : '最后修改人',
				sortable : true,
				hidden : true,
				header : '最后修改人'
			}, {
				dataIndex : '修改日期',
				sortable : true,
				header : '创建日期',
				hidden : true,
				align : 'center'
			}, {
				dataIndex : '最后修改日期',
				sortable : true,
				header : '最后修改日期',
				hidden : true,
				align : 'center'

			}]);
	var exp = new Ext.ux.Export({
				formater : [{
							columnHeader : '',
							formatClass : 'com.bitc.jk.xmis.export.PagingRowNumber'
						},{
							columnHeader : '签订收入合同总金额<br>(约xxx万元)',
							formatClass : 'com.bitc.jk.xmis.export.FormatCrmContactAmt'
						},{
							columnHeader : '签订支出合同总金额<br>(约xxx万元)',
							formatClass : 'com.bitc.jk.xmis.export.FormatCrmContactAmt'
						}]
			});
	// gridpanel org_公司
	var gridPanel = new Ext.grid.GridPanel({
		region : 'center',
		border : false,
		loadMask : true,
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		stripeRows : true,
		store : store,
		cm : cm,
		// sm : csm,
		tbar : [
				/*
				 * //新建和删除按钮 { xtype : 'button', text : '新建', hidden : false,
				 * icon : "../resources/images/add.gif", cls :
				 * "x-btn-text-icon", handler : function() {
				 * parent.xMis.turn('company-info', '新建公司信息',
				 * 'crm/companyInfo.jsp', ''); } },
				 * 
				 * 
				 * '-', { xtype : 'button', text : '删除', hidden : false, icon :
				 * "../resources/images/cross-circle.png", cls :
				 * "x-btn-text-icon", handler : function() { var records =
				 * gridPanel.getSelectionModel().getSelections(); var ids = "";
				 * if (records.length == 0) { Ext.MessageBox.alert("提示",
				 * "请选择数据"); } else { for (var i = 0; i < records.length; i++) {
				 * ids += records[i].get('companyID') + ","; }
				 * Ext.Ajax.request({ url : __cxtPath +
				 * '/crmMaint.do?method=dodelCompanyInfo', method : 'post',
				 * params : { ids : ids }, success : function(response, action) {
				 * Ext.MessageBox.alert("提示", "删除成功！"); store.reload(); },
				 * failure : function(response, action) { Ext.MessageBox.hide();
				 * Ext.MessageBox.alert("提示", "操作失败！"); } }); } } },
				 */
				brCombo, '-', '模糊查询：', {
					xtype : 'hidden',
					name : 'searchColumn',
					ref : '../searchColumn',
					value : '公司名称,xmis.org_公司规模.名称,xmis.org_客户级别.名称,xmis.org_信用等级.名称,xmis.add_洲际表.名称,( xmis.add_国家及地区.名称+xmis.add_地区.名称),省名称,城市名称,区县,邮编,备注,详细地址,注册资金,机构代码,电话1,电话2,传真1,传真2,网址,邮箱,开户银行,账号,税号'
				}, new Ext.form.TextField({
							width : 100,
							name : 'searchParam',
							ref : '../searchParam',
							ref : '../inputText',
							emptyText : '请输入查询内容',
							listeners : {
										'render' : function(c) {
											Ext.QuickTips.register({
														target : c.getEl(),
														text : '您可以输入‘公司名称’、‘国家及地区’、‘省’、‘城市’、‘电话’、‘传真’、‘开户银行’、‘税号’等的关键词进行查询'
													});
										}
									}
						}), '-', {
					xtype : 'combo',
					width : 100,
					ref : '../searchWay',
					displayField : 'text',
					valueField : 'value',
					style : 'background-image : url(""); background-color :  #99CCFF ;!important',
					mode : 'local',
					triggerAction : 'all',
					editable : false,
					value : 0,
					store : new Ext.data.ArrayStore({
								fields : ['value', 'text'],
								data : [[0, '人员姓名查询']]
//								data : [[0, '人员姓名查询'], [1, '设备名称查询']]
							}),
					listeners : {
						'select' : function(combo, record, index) {
							switch (record.data.value) {
								case 0 :
									gridPanel.personSearchParam.show();
									gridPanel.dSearchParam.hide();
									break;
								case 1 :
									gridPanel.dSearchParam.show();
									gridPanel.personSearchParam.hide();
									break;
							}
						}

					}
				}, new Ext.form.TextField({
							width : 100,
							name : 'personSearchParam',
							ref : '../personSearchParam',
							emptyText : '请输入人员姓名'
						}), new Ext.form.TextField({
							width : 100,
							hidden : true,
							name : 'dSearchParam',
							ref : '../dSearchParam',
							emptyText : '请输入设备名称'
						}), '-', {
					ref : '../searchBtn',
					iconCls : 'searchIcon',
					text : '查询',
					handler : function() {
						if (gridPanel.searchWay.getValue() == 0) {
							store.setBaseParam('personName',
									gridPanel.personSearchParam.getValue());
						} else {

						}
						store.setBaseParam('searchContent', gridPanel.inputText
										.getValue());
						store.setBaseParam('searchColumn',
								gridPanel.searchColumn.getValue());
						gridPanel.store.reload({
									params : {
										start : 0
									}
								});
					}
				}],
		bbar : new Ext.PagingToolbar({
					pageSize : 20,
					store : store,
					displayInfo : true,
					displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
					emptyMsg : "无显示数据",
					plugins : [new Ext.ux.PagelimitCombo()],
					items : [new Ext.Button({
								text : this.text || '导出Excel',
								iconCls : 'xls',
								menu : {
									items : [{
										text : '导出本页数据',
										handler : function() {
											exp.onExport(gridPanel, exportname,
													true);
										}
									}, {
										text : '导出所有数据',
										handler : function() {
											exp.onExport(gridPanel, exportname,
													false);
										}
									}]
								}
							})]
				})

	});
	// 变万元数据
	function formatMillion(data) {
		data = Ext.util.Format.number(data);
		data = data / 10000;
		if (data > 1) {
			data = Math.floor(data);
			data = Ext.util.Format.number(data, '0,000');
			data += '.00';
		} else {
			data = Ext.util.Format.number(data, '0,000.00');
		}
		if (data < 0.01) {
			return '--';
		}
		return data;
	}

	store.load({
				params : {
					start : 0,
					limit : 20
				}
			});
	// main
	new Ext.Viewport({
				layout : 'border',
				items : [gridPanel]
			});

});
