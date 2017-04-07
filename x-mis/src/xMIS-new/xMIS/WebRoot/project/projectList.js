Ext.ns('Ext.ux.grid');
Ext.ux.grid.RowNumberer = Ext.extend(Object, {
			id : 'cnumberer',
			width : 35,
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
						return store.lastOptions.params.start + rowIndex + 1;
					} else {
						return '<font color=red>&nbsp;<b>合计</b>&nbsp;</font>';
					}
				}
			}
		});
Ext.onReady(function() {
	Ext.QuickTips.init();
	var pcRecord = Ext.data.Record.create([{
				name : ContractTypeID
			}, {
				name : ContractID
			}, {
				name : ContractNo
			}, {
				name : ContractName
			}, {
				name : Abbreviation
			}, {
				name : ParentID
			}, {
				name : ParentContractAbb
			}, {
				name : SignatureDate
			}, {
				name : BuyerID
			}, {
				name : BuyerAbb
			}, {
				name : DeptIDOfBuyer
			}, {
				name : DeptNameOfBuyer
			}, {
				name : VendorID
			}, {
				name : VendorAbb
			}, {
				name : DeptIDOfVendor
			}, {
				name : DeptNameOfVendor
			}, {
				name : ContractAmt
			}, {
				name : BuyerRepName
			}, {
				name : VendorRepName
			}, {
				name : GovermentContractSourceName
			}, {
				name : ContractSummary
			}, {
				name : Remark
			}, {
				name : RepName
			}, {
				name : UpdateDate
			}, {
				name : ProjectID
			}, {
				name : ProjectCode
			}, {
				name : ProjectName
			}, {
				name : '项目状态',
				mapping : DBSchema.ProjectStatus.ProjectStatusName
			}, {
				name : CurrenyGraph
			}, {
				name : TotalAmtByID
			}, {
				name : UnChargedAmtTotal
			}, {
				name : receiptTotalAmt
			}, {
				name : invoiceTotalAmt
			}, {
				name : invoiceUnAccountAmt
			}, {
				name : incomAmtTotalByGl
			}, {
				name : purAmtTotalByGl
			}, {
				name : unIncomAmtTotalByGl
			}, {
				name : 'director',
				mapping : 'manager_name'
			}, {
				name : 'warning_date'
			}, {
				name : 'warning_amt'
			}, {
				name : 'warning_days'
			}, {
				name : 'CurrenyGraph'
			}, {
				name : DBSchema.DelayReason.Reason
			}, {
				name : 'execu_name'
			}

	]);
	var hasRole = com.bitc.xmis.util.OrgUtil.hasRole(10);
	var url = hasRole
			? __cxtPath + '/contractMain.do?method=integrationSearch'
			: __cxtPath
					+ '/contractMain.do?method=getContractListByType&contractTypeID=1';
	var baseParams = hasRole ? {
		contractTypeID : 1,
		searchName : '',
		warningType : 0,
		searchType : 1,
		limit : 20
	} : {
		searchName : '',
		warningType : 0,
		searchType : 1,
		limit : 20
	};
	var pcStore = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : url
						}),
				baseParams : baseParams,
				remoteSort : true,
				sortInfo : {
					field : SignatureDate,
					direction : 'DESC'
				},
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount'
						}, pcRecord)
			});
	var pcColM = new Ext.grid.ColumnModel([new Ext.ux.grid.RowNumberer(), {
		header : "项目编码", // 2
		dataIndex : ProjectCode,
		sortable : true,
		width : 150,
		renderer : function(value, metaData, record) {
			value = Ext.isEmpty(value) ? '_' : value;
			return '<a href="javascript:;" onclick="parent.xMis.turn(\'project-info-'
					+ record.get(ProjectID)
					+ '\',\'项目信息-'
					+ record.get(ProjectName)
					+ '\',\''
					+ __cxtPath
					+ '/projectMain.do?method=getProjectInfo&projectID='
					+ record.get(ProjectID)
					+ '\',\'menu-project-info\')">'
					+ value + '</a>';
		}
	}, {
		header : "项目名称", // 23
		dataIndex : ProjectName,
		sortable : true,
		width : 220,
		renderer : function(value, m, record) {
			value = Ext.isEmpty(value) ? '_' : value;
			return '<a href="javascript:;" onclick="parent.xMis.turn(\'project-info-'
					+ record.get(ProjectID)
					+ '\',\'项目信息-'
					+ record.get(ProjectName)
					+ '\',\''
					+ __cxtPath
					+ '/projectMain.do?method=getProjectInfo&projectID='
					+ record.get(ProjectID)
					+ '\',\'menu-project-info\')">'
					+ value + '</a>';
		}
	}, {
		header : "执行部门",
		sortable :true,
		hidden :true,
		dataIndex : 'execu_name',
		width : 100
	}, {
		header : "项目经理",
		dataIndex : 'director',
		width : 100
	}, {
		header : '项目状态',
		dataIndex : DBSchema.ProjectStatus.ProjectStatusName,
		sortable:true,
		width : 100
	}, {
		header : "启动日期", // 6
		dataIndex : SignatureDate,
		sortable : true,
		width : 100,
		renderer : customer_formatDate,
		align : 'center'
	}, {
		header : "甲方公司", // 8
		dataIndex : BuyerAbb,
		sortable : true,
		hidden : true
	}, {
		header : "<font color=" + Color_Font_Contract_Total_Amt
				+ ">项目金额</font>", // 11
		dataIndex : ContractAmt,
		sortable : true,
		width : 120,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (Ext.isEmpty(value)) {
				return '';
			}
			value = value.substring(0, value.indexOf('^'));
			value = Ext.util.Format.number(new Number(value),
					record.data[CurrenyGraph] + '0,0.00');
			return value;
		},
		align : 'right'
	}, {
		id : TotalAmtByID,
		header : "<font color=" + Color_Font_Contract_Total_Charged_Amt
				+ ">已收合同款</font>", // 12
		dataIndex : TotalAmtByID,
		sortable : true,
		hidden : true,
		width : 120,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			value = Ext.util.Format.number(new Number(value),
					record.data[CurrenyGraph] + '0,0.00');
			return value;
		},
		align : 'right'
	}, {
		header : "<font color=" + Color_Font_Contract_Total_UnCharged_Amt
				+ ">未收合同款</font>", // 16
		dataIndex : UnChargedAmtTotal,
		sortable : true,
		width : 120,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			return Ext.util.Format.number(new Number(value),
					record.data[CurrenyGraph] + '0,0.00');
		},
		align : 'right'
	}, {
		header : "合同概要", // 19
		dataIndex : ContractSummary,
		hidden : true
	}, {
		header : "备注", // 20
		dataIndex : Remark,
		hidden : true
	}, {
		header : "登记人", // 21
		dataIndex : RepName,
		sortable : true,
		renderer : biuldLinker,
		hidden : true
	}, {
		header : "财务来款合计", // 25
		dataIndex : incomAmtTotalByGl,
		sortable : true,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			return Ext.util.Format.number(new Number(value),
					record.data[CurrenyGraph] + '0,0.00');
		},
		align : 'right'
	}, {
		header : "采购支出合计", // 26
		dataIndex : purAmtTotalByGl,
		sortable : true,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			return Ext.util.Format.number(new Number(value),
					record.data[CurrenyGraph] + '0,0.00');
		},
		align : 'right'
	}, {
		header : "合同欠款", // 27
		dataIndex : unIncomAmtTotalByGl,
		sortable : true,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			cellMeta.attr = setWarningBackgroudColor(
					record.data["warning_date"], record.data["warning_amt"],
					record.data["warning_days"], record.data[CurrenyGraph],
					record.data[DBSchema.DelayReason.Reason]);
			return Ext.util.Format.number(new Number(value),
					record.data[CurrenyGraph] + '0,0.00');
		},
		align : 'right'
	}

	]);

	var exp = new Ext.ux.Export({
				formater : [{
							columnHeader : '',
							formatClass : 'com.bitc.jk.xmis.export.PagingRowNumber'
						}, {
							columnHeader : '启动日期',
							formatClass : 'com.bitc.jk.xmis.export.SubstringTimer'
						}]
			});
	var exportname = '项目查询';
	var gridPanel = new Ext.grid.GridPanel({
		region : 'center',
		border : false,
		loadMask : true,
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		stripeRows : true,
		store : pcStore,
		cm : pcColM,
		tbar : [{
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
							contractTypeID : Type_ContractType_Income,
							qxurl : pcStore.proxy.url
						},
						idProperty : 'deptID',
						autoLoad : true,
						listeners : {
							'load' : function(s, records) {
								if (!records || records.length == 0) {
									gridPanel.deptCombo.setVisible(false);
								}
							}
						}
					}),
			listeners : {
				'select' : function(combo, record, index) {
					var selValue = record.get('paramValue');
					if (selValue == '-1') {
						delete pcStore.baseParams['deptID'];
					} else {
						pcStore.setBaseParam('deptID', selValue);
					}
					pcStore.load({
								params : {
									start : 0
								}
							});
				}
			}
		}, '-', '模糊查询：', new Ext.ux.InklingSearchField({
			width : 120,
			store : pcStore,
			tooltip : {
				title : '模糊查询',
				text : '您可以输入任意关键词进行查询。如：项目编码、项目名称等<br/>可以在关键字前加运算符：>、<、>=、<=、=、<>、like等。<br/>按日期请按如下格式输入：<br/>2010-01-01'
			},
			inklings : [{
						name : '项目编码',
						op : 'like',
						type : 'string'
					}, {
						name : '项目名称',
						op : 'like',
						type : 'string'
					}, {
						name : '启动日期',
						op : '=',
						type : 'date'
					}, {
						name : '合同总额',
						op : '=',
						type : 'double'
					}, {
						name : 'aCompany_name',
						op : 'like',
						type : 'string'
					}, {
						name : '项目状态',
						op : 'like',
						type : 'string'
					}, {
						name : '备注',
						op : 'like',
						type : 'string'
					}, {
						name : 'execu_name',
						op : 'like',
						type : 'string'
					}],
			onTrigger2Click : function() {
				var v = this.getRawValue();
				if (v.length < 1) {
					this.store.baseParams = {
						searchName : '',
						warningType : 0,
						limit : 20
					};
					this.store.reload();
				} else {
					var params = '';
					Ext.iterate(this.inklings, function(item, index, allItems) {
								params += item.name + ',' + item.op + ','
										+ item.type;
								if (index < allItems.length - 1) {
									params += '|';
								}
							}, this);
					this.store.baseParams = this.store.baseParams || {};
					this.store.baseParams['inklings'] = params;
					Ext.ux.InklingSearchField.superclass.onTrigger2Click
							.call(this);
				}
			}
		}), '-', {
			ref : '../conditionBtn',
			iconCls : 'condition',
			text : '组合查询',
			handler : function(b, e) {
				searchPanel.store = pcStore;
				searchConditionWin.x = e.getPageX() + 5;
				searchConditionWin.y = e.getPageY() + 5;
				searchConditionWin.show();
			}
		}],
		bbar : new Ext.PagingToolbar({
					pageSize : 20,
					store : pcStore,
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
													exp.onExport(gridPanel,exportname,
															true);
												}
											}, {
												text : '导出所有数据',
												handler : function() {
													exp.onExport(gridPanel,exportname,
															false);
												}
											}]
								}
							})]
				})
	});
	var main = new Ext.Viewport({
				layout : 'border',
				items : [gridPanel]
			});
	var searchPanel;
	var searchConditionWin = new Ext.Window({
				title : '条件设定',
				renderTo : Ext.getBody(),
				frame : true,
				plain : true,
				resizable : true,
				closeAction : "hide",
				// maximizable:true,
				closable : true,
				layout : "form",
				bodyStyle : "padding:10px",
				width : 280,
				// height:100,
				autoHeight : true
			});
	Ext.ux.MultiSearchPanel.create(2, function(panel) {
				searchPanel = panel;
				searchConditionWin.add(panel);
			}, this);
	pcStore.load({
				params : {
					start : 0
				}
			});
}, this);
function linkRenderer(value, contractID, contractName, contractType) {
	if (Ext.isEmpty(value)) {
		value = '_';
	}
	if (contractName.indexOf("^") > 0) {
		contractName = contractName.substring(0, contractName.indexOf("^"));
	}
	var index = value.indexOf("^");
	var tmp = "<a href='javascript:;' onclick=\"javascript:turnToEditContract('{0}','{1}',{2});\">{3}</a>";
	if (index >= 0) {
		var text = value.substring(0, index);
		var url = value.substring(index + 1, value.length);
		return String.format(tmp, contractID, contractName, contractType
						? contractType
						: 'undefined', text);
	}
	return String.format(tmp, contractID, contractName, contractType
					? contractType
					: 'undefined', value);
}
function moneyrender(value, metaData, record, rowIndex, colIndex, store) {
	return formatCurrency(record.get('currencySymbol'), value + '', value > 0
					? 'blue'
					: 'red');
}