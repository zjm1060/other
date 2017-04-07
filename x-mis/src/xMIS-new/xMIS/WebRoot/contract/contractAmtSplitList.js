Ext.ns('Ext.ux.grid');
Ext.ux.grid.RowNumberer = Ext.extend(Object, {
			width : 45,
			fixed : true,
			sortable : false,
			hideable : false,
			menuDisabled : true,
			dataIndex : '',
			renderer : function(value, cellmeta, record, rowIndex, columnIndex,
					store) {
				if (store.getCount() - 1 < store.baseParams.limit
						&& store.getCount() == rowIndex + 1) {
					return '<font color=red>&nbsp;<b>合计</b>&nbsp;</font>';
				} else {
					if (rowIndex != store.baseParams.limit) {
						if (Ext.isEmpty(store.lastOptions.params)) {
							return rowIndex + 1;
						} else {
							return store.lastOptions.params.start + rowIndex
									+ 1;
						}
					} else {
						return '<font color=red>&nbsp;<b>合计</b>&nbsp;</font>';
					}
				}
			}
		});
Ext.onReady(function() {
	var exportname = '计算机信息系统集成项目汇总';

	var record = Ext.data.Record.create([{
				name : '合同ID'
			}, {
				name : '合同名称'
			}, {
				name : '项目领域'
			}, {
				name : '合同总额'
			}, {
				name : '签订日期'
			}, {
				name : '有效期起'
			}, {
				name : '项目验收时间'
			}, {
				name : '项目合作形式父ID'
			}, {
				name : '项目编码'
			}, {
				name : '硬件购置费'
			}, {
				name : '软件购置费'
			}, {
				name : '软件和信息技术服务费'
			}, {
				name : '其他'
			}, {
				name : '自制硬件费'
			}, {
				name : '国产硬件费'
			}, {
				name : '进口硬件费'
			}, {
				name : '国产软件费'
			}, {
				name : '进口软件费'
			}, {
				name : '软件开发费用'
			}, {
				name : '咨询设计费用'
			}, {
				name : '系统集成费用'
			}, {
				name : '运营维护费用'
			}, {
				name : '建筑工程费'
			}, {
				name : '管理费'
			}, {
				name : '运费'
			}, {
				name : '税金'
			}, {
				name : '合同来源类型ID'
			}, {
				name : '软件和信息技术服务费用比例'
			}, {
				name : 'splitflag'
			}]);

	var store = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : __cxtPath
									+ '/contractMain.do?method=getContractAmtSplitList'
						}),
				baseParams : {
					limit : 20
				},
				remoteSort : true,
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount'
						}, record)
			});

	var oneRow = [{
				header : '',
				colspan : 1,
				align : 'center'
			}, {
				header : '',
				colspan : 1,
				align : 'center'
			}, {
				header : '',
				colspan : 1,
				align : 'center'
			}, {
				header : '',
				colspan : 1,
				align : 'center'
			}, {
				header : '',
				colspan : 1,
				align : 'center'
			}, {
				header : '',
				colspan : 1,
				align : 'center'
			}, {
				header : '',
				colspan : 4,
				align : 'center'
			}, {
				header : '',
				colspan : 3,
				align : 'center'
			}, {
				header : '<font color='
						+ Color_Font_Contract_Total_UnAccount_Amt
						+ '>软件和信息技术服务费（万元）</font>',
				colspan : 8,
				align : 'center'
			}, {
				header : '',
				colspan : 1,
				align : 'center'
			}, {
				header : '',
				colspan : 1,
				align : 'center'
			}, {
				header : '',
				colspan : 1,
				align : 'center'
			}, {
				header : '',
				colspan : 1,
				align : 'center'
			}];

	var twoRow = [{
				header : '',
				colspan : 1,
				align : 'center'
			}, {
				header : '',
				colspan : 1,
				align : 'center'
			}, {
				header : '',
				colspan : 1,
				align : 'center'
			}, {
				header : '',
				colspan : 1,
				align : 'center'
			}, {
				header : '',
				colspan : 1,
				align : 'center'
			}, {
				header : '',
				colspan : 1,
				align : 'center'
			}, {
				header : '<font color=' + Color_Font_Contract_Total_Charged_Amt
						+ '>硬件购置费（万元）</font>',
				colspan : 4,
				align : 'center'
			}, {
				header : '<font color=' + Color_Font_Contract_Total_Amt
						+ '>软件购置费（万元）</font>',
				colspan : 3,
				align : 'center'
			}, {
				header : '',
				colspan : 1,
				align : 'center'
			}, {
				header : '',
				colspan : 1,
				align : 'center'
			}, {
				header : '',
				colspan : 1,
				align : 'center'
			}, {
				header : '<font color=#0066FF>运营维护费（万元）</font>',
				colspan : 3,
				align : 'center'
			}, {
				header : '',
				colspan : 1,
				align : 'center'
			}, {
				header : '',
				colspan : 1,
				align : 'center'
			}, {
				header : '',
				colspan : 1,
				align : 'center'
			}, {
				header : '',
				colspan : 1,
				align : 'center'
			}, {
				header : '',
				colspan : 1,
				align : 'center'
			}, {
				header : '',
				colspan : 1,
				align : 'center'
			}];

	// 列头分组
	var group = new Ext.ux.grid.ColumnHeaderGroup({
				rows : [oneRow, twoRow]
			});

	var tcm = new Ext.grid.ColumnModel([new Ext.ux.grid.RowNumberer(), {
				header : "合同ID",
				dataIndex : "合同ID",
				align : 'center',
				hidden : true
			}, {
				header : '项目编码',
				dataIndex : '项目编码',
				sortable : true,
				align : 'left'
			}, {
				header : "项目名称",
				dataIndex : "合同名称",
				align : 'left',
				sortable : true,
				renderer : function(value, cellmeta, record) {
					if (record.data.splitflag == 1) {
						return linkRenderer("<font color=red>" + value
										+ "</font>", record.data["合同ID"],
								"<font color=red>" + value + "</font>")
					} else {
						return linkRenderer(value, record.data["合同ID"], value);
					}
				}
			}, {
				header : "项目领域",
				dataIndex : "项目领域",
				sortable : true,
				align : 'center'
			}, {
				header : "合同金额（万元）",
				dataIndex : "合同总额",
				align : 'right',
				sortable : true,
				renderer : function(value) {
					if (value.indexOf("^") > 0) {
						return value = formatMillion(value.substring(0, value
										.indexOf("^")));
					}
				}
			}, {
				header : "<font color=" + Color_Font_Contract_Total_Charged_Amt
						+ ">自制</font>",
				dataIndex : "自制硬件费",
				align : 'right',
				sortable : true,
				renderer : function(value) {
					return value = "<font color="
							+ Color_Font_Contract_Total_Charged_Amt + ">"
							+ formatMillion(value) + "</font>";
				}
			}, {
				header : "<font color=" + Color_Font_Contract_Total_Charged_Amt
						+ ">国产</font>",
				dataIndex : "国产硬件费",
				align : 'right',
				sortable : true,
				renderer : function(value) {
					return value = "<font color="
							+ Color_Font_Contract_Total_Charged_Amt + ">"
							+ formatMillion(value) + "</font>";
				}
			}, {
				header : "<font color=" + Color_Font_Contract_Total_Charged_Amt
						+ ">进口</font>",
				dataIndex : "进口硬件费",
				align : 'right',
				sortable : true,
				renderer : function(value) {
					return value = "<font color="
							+ Color_Font_Contract_Total_Charged_Amt + ">"
							+ formatMillion(value) + "</font>";
				}
			}, {
				header : "<font color="
						+ Color_Font_Contract_Total_Invoiced_Amt + ">小计</font>",
				dataIndex : "硬件购置费",
				align : 'right',
				sortable : true,
				renderer : function(value) {
					return value = "<font color="
							+ Color_Font_Contract_Total_Invoiced_Amt + ">"
							+ formatMillion(value) + "</font>";
				}
			}, {
				header : "<font color=" + Color_Font_Contract_Total_Amt
						+ ">国产</font>",
				dataIndex : "国产软件费",
				align : 'right',
				sortable : true,
				renderer : function(value) {
					return value = "<font color="
							+ Color_Font_Contract_Total_Amt + ">"
							+ formatMillion(value) + "</font>";
				}
			}, {
				header : "<font color=" + Color_Font_Contract_Total_Amt
						+ ">进口</font>",
				dataIndex : "进口软件费",
				align : 'right',
				sortable : true,
				renderer : function(value) {
					return value = "<font color="
							+ Color_Font_Contract_Total_Amt + ">"
							+ formatMillion(value) + "</font>";
				}
			}, {
				header : "<font color="
						+ Color_Font_Contract_Total_Invoiced_Amt + ">小计</font>",
				dataIndex : "软件购置费",
				align : 'right',
				sortable : true,
				renderer : function(value) {
					return value = "<font color="
							+ Color_Font_Contract_Total_Invoiced_Amt + ">"
							+ formatMillion(value) + "</font>";
				}
			}, {
				header : "<font color="
						+ Color_Font_Contract_Total_UnAccount_Amt
						+ ">咨询设计费</font>",
				dataIndex : "咨询设计费用",
				align : 'right',
				sortable : true,
				renderer : function(value) {
					return value = "<font color="
							+ Color_Font_Contract_Total_UnAccount_Amt + ">"
							+ formatMillion(value) + "</font>";
				}
			}, {
				header : "<font color="
						+ Color_Font_Contract_Total_UnAccount_Amt
						+ ">软件开发费</font>",
				dataIndex : "软件开发费用",
				align : 'right',
				sortable : true,
				renderer : function(value) {
					return value = "<font color="
							+ Color_Font_Contract_Total_UnAccount_Amt + ">"
							+ formatMillion(value) + "</font>";
				}
			}, {
				header : "<font color="
						+ Color_Font_Contract_Total_UnAccount_Amt
						+ ">系统集成费</font>",
				dataIndex : "系统集成费用",
				align : 'right',
				sortable : true,
				renderer : function(value) {
					return value = "<font color="
							+ Color_Font_Contract_Total_UnAccount_Amt + ">"
							+ formatMillion(value) + "</font>";
				}
			}, {
				header : "<font color=#0066FF>运行维护费</font>",
				dataIndex : "运营维护费用",
				align : 'right',
				sortable : true,
				renderer : function(value) {
					return value = "<font color=#0066FF>"
							+ formatMillion(value) + "</font>";
				}
			}, {
				header : "<font color=#0066FF>数据处理费（无需输入）</font>",
				dataIndex : "运营维护费用",
				align : 'right',
				sortable : true,
				renderer : function() {
					return value = "<font color=#0066FF>" + "0.00" + "</font>";
				},
				exportFormater : function(value, row, index, pageinfo) {
					return '0.00';
				}
			}, {
				header : "<font color=#0066FF>运营服务费（无需输入）</font>",
				dataIndex : "运营维护费用",
				align : 'right',
				sortable : true,
				renderer : function() {
					return value = "<font color=#0066FF>" + "0.00" + "</font>";
				},
				exportFormater : function(value, row, index, pageinfo) {
					return '0.00';
				}
			}, {
				header : "<font color="
						+ Color_Font_Contract_Total_Invoiced_Amt
						+ ">软件和信息技术服务费用合计</font>",
				dataIndex : "软件和信息技术服务费",
				align : 'right',
				sortable : true,
				renderer : function(value) {
					return value = "<font color="
							+ Color_Font_Contract_Total_Invoiced_Amt + ">"
							+ formatMillion(value) + "</font>";
				}
			}, {
				header : "<font color="
						+ Color_Font_Contract_Total_Invoiced_Amt
						+ ">软件和信息技术服务费用比例</font>",
				dataIndex : "软件和信息技术服务费用比例",
				align : 'right',
				sortable : true,
				renderer : function(value, cellMeta, record, rowIndex,
						columnIndex, store) {
					if (value == 2) {
						return '-';
					}
					return value = "<font color="
							+ Color_Font_Contract_Total_Invoiced_Amt + ">"
							+ Ext.util.Format.number(value * 100, '0,000.00')
							+ '%' + "</font>";
				}
			}, {
				header : "其他（万元）",
				dataIndex : "其他",
				align : 'right',
				sortable : true,
				renderer : function(value) {
					return value = formatMillion(value);
				}
			}, {
				header : "合同生效时间",
				dataIndex : "签订日期",
				align : 'right',
				sortable : true,
				renderer : customer_formatDate
			}, {
				header : "项目验收时间",
				dataIndex : "项目验收时间",
				align : 'left',
				sortable : true,
				renderer : customer_formatDate
			}, {
				header : "是否分包",
				dataIndex : "项目合作形式父ID",
				align : 'center',
				sortable : true,
				renderer : function(value) {
					if (value == 10000) {
						return '-';
					} else if (value == 8) {
						return '是';
					} else {
						return '否'
					}
				},
				exportFormater : function(value, row, index, pageinfo) {
					if (value == 10000) {
						return '-';
					} else if (value == 8) {
						return '是';
					} else {
						return '否'
					}
				}
			}]);
	var exp = new Ext.ux.Export({
				formater : [{
							columnHeader : '',
							formatClass : 'com.bitc.jk.xmis.export.PagingRowNumber'
						}, {
							columnHeader : '合同生效时间',
							formatClass : 'com.bitc.jk.xmis.export.SubstringTimer'
						}, {
							columnHeader : '项目验收时间',
							formatClass : 'com.bitc.jk.xmis.export.SubstringTimer'
						}, {
							columnHeader : '合同金额（万元）',
							formatClass : 'com.bitc.jk.xmis.export.MillionFormat'
						}, {
							columnHeader : '<font color='
									+ Color_Font_Contract_Total_Charged_Amt
									+ '>硬件购置费（万元）</font>' + ' '
									+ '<font color='
									+ Color_Font_Contract_Total_Charged_Amt
									+ '>自制</font>',
							formatClass : 'com.bitc.jk.xmis.export.MillionFormat'
						}, {
							columnHeader : '<font color='
									+ Color_Font_Contract_Total_Charged_Amt
									+ '>硬件购置费（万元）</font>' + ' '
									+ '<font color='
									+ Color_Font_Contract_Total_Charged_Amt
									+ '>国产</font>',
							formatClass : 'com.bitc.jk.xmis.export.MillionFormat'
						}, {
							columnHeader : '<font color='
									+ Color_Font_Contract_Total_Charged_Amt
									+ '>硬件购置费（万元）</font>' + ' '
									+ '<font color='
									+ Color_Font_Contract_Total_Charged_Amt
									+ '>进口</font>',
							formatClass : 'com.bitc.jk.xmis.export.MillionFormat'
						}, {
							columnHeader : '<font color='
									+ Color_Font_Contract_Total_Charged_Amt
									+ '>硬件购置费（万元）</font>' + ' '
									+ '<font color='
									+ Color_Font_Contract_Total_Invoiced_Amt
									+ '>小计</font>',
							formatClass : 'com.bitc.jk.xmis.export.MillionFormat'
						}, {
							columnHeader : '<font color='
									+ Color_Font_Contract_Total_Amt
									+ '>软件购置费（万元）</font>' + ' '
									+ '<font color='
									+ Color_Font_Contract_Total_Amt
									+ '>国产</font>',
							formatClass : 'com.bitc.jk.xmis.export.MillionFormat'
						}, {
							columnHeader : '<font color='
									+ Color_Font_Contract_Total_Amt
									+ '>软件购置费（万元）</font>' + ' '
									+ '<font color='
									+ Color_Font_Contract_Total_Amt
									+ '>进口</font>',
							formatClass : 'com.bitc.jk.xmis.export.MillionFormat'
						}, {
							columnHeader : '<font color='
									+ Color_Font_Contract_Total_Amt
									+ '>软件购置费（万元）</font>' + ' '
									+ "<font color="
									+ Color_Font_Contract_Total_Invoiced_Amt
									+ ">小计</font>",
							formatClass : 'com.bitc.jk.xmis.export.MillionFormat'
						}

						, {
							columnHeader : '<font color='
									+ Color_Font_Contract_Total_UnAccount_Amt
									+ '>软件和信息技术服务费（万元）</font>' + ' '
									+ '<font color='
									+ Color_Font_Contract_Total_UnAccount_Amt
									+ '>咨询设计费</font>',
							formatClass : 'com.bitc.jk.xmis.export.MillionFormat'
						}, {
							columnHeader : '<font color='
									+ Color_Font_Contract_Total_UnAccount_Amt
									+ '>软件和信息技术服务费（万元）</font>' + ' '
									+ '<font color='
									+ Color_Font_Contract_Total_UnAccount_Amt
									+ '>软件开发费</font>',
							formatClass : 'com.bitc.jk.xmis.export.MillionFormat'
						}, {
							columnHeader : '<font color='
									+ Color_Font_Contract_Total_UnAccount_Amt
									+ '>软件和信息技术服务费（万元）</font>' + ' '
									+ '<font color='
									+ Color_Font_Contract_Total_UnAccount_Amt
									+ '>系统集成费</font>',
							formatClass : 'com.bitc.jk.xmis.export.MillionFormat'
						}, {
							columnHeader : '<font color='
									+ Color_Font_Contract_Total_UnAccount_Amt
									+ '>软件和信息技术服务费（万元）</font>' + ' '
									+ '<font color=#0066FF>运营维护费（万元）</font>'
									+ ' ' + '<font color=#0066FF>运行维护费</font>',
							formatClass : 'com.bitc.jk.xmis.export.MillionFormat'
						}, {
							columnHeader : '<font color='
									+ Color_Font_Contract_Total_UnAccount_Amt
									+ '>软件和信息技术服务费（万元）</font>' + ' '
									+ '<font color='
									+ Color_Font_Contract_Total_Invoiced_Amt
									+ '>软件和信息技术服务费用合计</font>',
							formatClass : 'com.bitc.jk.xmis.export.MillionFormat'
						}, {
							columnHeader : '<font color='
									+ Color_Font_Contract_Total_UnAccount_Amt
									+ '>软件和信息技术服务费（万元）</font>' + ' '
									+ "<font color="
									+ Color_Font_Contract_Total_Invoiced_Amt
									+ ">软件和信息技术服务费用比例</font>",
							formatClass : 'com.bitc.jk.xmis.export.PercentFormat'
						}, {
							columnHeader : '其他（万元）',
							formatClass : 'com.bitc.jk.xmis.export.MillionFormat'
						}]
			});

	var grid = new Ext.grid.GridPanel({
		region : "center",
		cm : tcm,
		store : store,
		enableColumnMove : false,
		loadMask : true,
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		bodyStyle : "width:33%",
		plugins : group,
		tbar : [{
			xtype : 'combo',
			width : 100,
			displayField : 'text',
			valueField : 'value',
			mode : 'local',
			triggerAction : 'all',
			editable : false,
			value : -1,
			store : new Ext.data.ArrayStore({
						fields : ['value', 'text'],
						data : [[-1, '所有合同'], [0, '横向合同'], [1, '纵向合同'],
								[2, '公司自研']]
					}),
			listeners : {
				'select' : function(combo, record, index) {
					switch (record.get('value')) {
						case -1 :
							delete store.baseParams['sourceID'];
							break;
						case 0 :
							store.setBaseParam('sourceID', 0);
							break;
						case 1 :
							store.setBaseParam('sourceID', 1);
							break;
						case 2 :
							store.setBaseParam('sourceID', 2);
							break;
					}
					delete store.baseParams['selIds'];
				}
			}
		}, '-', {
			xtype : 'combo',
			ref : '../deptCombo',
			width : 120,
			displayField : 'deptName',
			valueField : 'paramValue',
			editable : false,
			triggerAction : 'all',
			emptyText : '请选择部门',
			lazyInit : false,
			store : new Ext.data.JsonStore({
						url : String.format(
								'{0}/contractMain.do?method=getDeptCombo',
								__cxtPath),
						root : 'rows',
						fields : ['paramValue', {
									name : 'deptName',
									mapping : '部门名称'
								}, {
									name : 'deptID',
									mapping : '部门ID'
								}],
						baseParams : {
							qxurl : String
									.format(
											'{0}/contractMain.do?method=integrationSearch',
											__cxtPath)
						},
						idProperty : 'deptID',
						autoLoad : true,
						listeners : {
							'load' : function(s, records) {
								if (!records || records.length == 0) {
									grid.deptCombo.setVisible(false);
								}
							}
						}
					}),
			listeners : {
				'select' : function(combo, record, index) {

					var selValue = record.get('paramValue');

					if (selValue == '-1') {
						delete store.baseParams['deptID'];
					} else {
						store.setBaseParam('deptID', selValue);
					}
					delete store.baseParams['selIds'];
				}
			}
		}, '-', '模糊查询：', new Ext.ux.InklingSearchField({
					width : 120,
					store : store,
					tooltip : {
						title : '模糊查询',
						text : '您可以输入任意关键词进行查询。'
								+ '<br/>可以在关键字前加运算符：>、<、>=、<=、=、<>、like等。'
								+ '<br/>按日期请按如下格式输入：<br/>2010-01-01'
					},
					inklings : [{
								name : '合同名称',
								op : 'like',
								type : 'string'
							}, {
								name : '有效期起',
								op : '=',
								type : 'date'
							}, {
								name : '合同总额',
								op : '=',
								type : 'double'
							}, {
								name : '项目编码',
								op : 'like',
								type : 'string'
							}],
					onTrigger2Click : function() {
						var v = this.getRawValue();
						if (v.length < 1) {
							this.store.baseParams['limit'] = 20;
							this.store.baseParams['start'] = 0;
							this.store.load();
						} else {
							var params = '';
							Ext.iterate(this.inklings, function(item, index,
											allItems) {
										params += item.name + ',' + item.op
												+ ',' + item.type;
										if (index < allItems.length - 1) {
											params += '|';
										}
									}, this);
							delete this.store.baseParams['selIds'];
							this.store.baseParams = this.store.baseParams || {};
							this.store.baseParams['inklings'] = params;
							Ext.ux.InklingSearchField.superclass.onTrigger2Click
									.call(this);
						}
					}
				}),'->','<font color ="blue">*未进行合同金额分解的合同名称置红*</font>'],
		bbar : new Ext.ux.TotalPagingToolbar({
					pageSize : 20,
					store : store,
					displayInfo : true,
					displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
					emptyMsg : "无显示数据",
					plugins : [exp, new Ext.ux.PagelimitCombo()]
				})
	});

	store.load({
				params : {
					start : 0,
					limit : 20
				},
				callback : function(record, options, success) {
					if (success == true) {
					} else {
						Ext.Msg.alert("出错了", "加载数据错误");
					}
				}
			});

	new Ext.Viewport({
				border : false,
				layout : 'border',
				items : [grid]
			});

});
function formatMillion(data) {
	data = Ext.util.Format.number(data);
	data = data / 10000;
	return Ext.util.Format.number(data, '0,000.00');
}
function turnToEditContract(contractID, contractName, contractType) {
	parent.xMis.turn('contract-' + contractID, contractName,
			'contractMain.do?method=contract&contractID=' + contractID, null);
}
function linkRenderer(value, contractID, contractName, contractType) {
	if (Ext.isEmpty(value)) {
		value = '--';
	}
	if (contractName.indexOf("^") > 0) {
		contractName = contractName.substring(0, contractName.indexOf("^"));
	}
	var index = value.indexOf("^");
	var tmp = "<a href='javascript:;' onclick=\"javascript:turnToEditContract('{0}','{1}',{2});\">{3}</a>";
	contractName = Ext.util.Format.htmlEncode(contractName);
	if (index >= 0) {
		var text = value.substring(0, index);
		var url = value.substring(index + 1, value.length);
		return String.format(tmp, contractID, '【合同】' + contractName,
				contractType ? contractType : 'undefined', text);
	}
	return String.format(tmp, contractID, '【合同】' + contractName, contractType
					? contractType
					: 'undefined', value);
}
