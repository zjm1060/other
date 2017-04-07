// 合同综合查询
Ext.ns('com.bitc.xmis.contract');
com.bitc.xmis.ContractGridPanel = Ext.extend(Ext.Panel, {
	initComponent : function() {
		if (this.contractTypeID) {
			this.layout = 'border';
			this.border = false;
			this.decisionFlag = decisionFlag;
			var grid = this.buildGrid();
			this.items = [grid];
		}
		com.bitc.xmis.ContractGridPanel.superclass.initComponent.call(this);
	},
	buildGrid : function() {
		var contractTypeID = this.contractTypeID;

		var integrationTypeFlag, dateRange = "", limitCount = 20, reconfigure = -1;
		function loadData(contractType, searchName, warningType,
				integrationType) {
			integrationTypeFlag = integrationType;

			var searchId = 'incomeGrid';

			if (contractType == 2) {
				searchId = 'outGrid';
			}

			if (integrationType != Type_Contract_Integration_Simple) {
				if (contractType == Type_ContractType_Income) {
					dateRange = convertDateObjToFormatStr(Ext
							.getCmp('searchConditionFrm').getForm()
							.findField('beginDate').getValue())
							+ "至"
							+ convertDateObjToFormatStr(Ext
									.getCmp('searchConditionFrm').getForm()
									.findField('endDate').getValue());
				} else {
					dateRange = convertDateObjToFormatStr(Ext
							.getCmp('searchConditionoutFrm').getForm()
							.findField('beginDateout').getValue())
							+ "至"
							+ convertDateObjToFormatStr(Ext
									.getCmp('searchConditionoutFrm').getForm()
									.findField('endDateout').getValue());
				}
			}

			Ext.getCmp(searchId).getStore()
					.setDefaultSort(SignatureDate, 'ASC');
			Ext.getCmp(searchId).getStore().removeAll();
			Ext.getCmp(searchId).getStore().setBaseParam("currencyGraph", "");
			Ext.getCmp(searchId).getStore().setBaseParam("searchName",
					searchName);
			Ext.getCmp(searchId).getStore().setBaseParam("contractTypeID",
					contractType);
			Ext.getCmp(searchId).getStore().setBaseParam("warningType",
					warningType);
			Ext.getCmp(searchId).getStore().setBaseParam("searchType",
					integrationType);
			Ext.getCmp(searchId).getStore().setBaseParam("begDate", '');
			Ext.getCmp(searchId).getStore().setBaseParam("endDate", '');

			switch (integrationType) {
				case Type_Contract_Integration_Simple :
					dateRange = ""
					break;
				case Type_Contract_Integration_New :

					dateRange += " 新签合同总统计"
					if (contractType == Type_ContractType_Income) {
						incomeStore.setBaseParam("begDate",
								convertDateObjToFormatStr(Ext
										.getCmp('searchConditionFrm').getForm()
										.findField('beginDate').getValue()));
						incomeStore.setBaseParam("endDate",
								convertDateObjToFormatStr(Ext
										.getCmp('searchConditionFrm').getForm()
										.findField('endDate').getValue()));

					} else {
						outStore.setBaseParam("begDate",
								convertDateObjToFormatStr(Ext
										.getCmp('searchConditionoutFrm')
										.getForm().findField('beginDateout')
										.getValue()));
						outStore.setBaseParam("endDate",
								convertDateObjToFormatStr(Ext
										.getCmp('searchConditionoutFrm')
										.getForm().findField('endDateout')
										.getValue()));
					}
					break;

				case Type_Contract_Integration_Performed :
					dateRange += " 合同收付款情况"
					if (contractType == Type_ContractType_Income) {
						incomeStore.setBaseParam("begDate",
								convertDateObjToFormatStr(Ext
										.getCmp('searchConditionFrm').getForm()
										.findField('beginDate').getValue()));
						incomeStore.setBaseParam("endDate",
								convertDateObjToFormatStr(Ext
										.getCmp('searchConditionFrm').getForm()
										.findField('endDate').getValue()));
						reconfigure = Type_Contract_Integration_Performed;
					} else {
						outStore.setBaseParam("begDate",
								convertDateObjToFormatStr(Ext
										.getCmp('searchConditionoutFrm')
										.getForm().findField('beginDateout')
										.getValue()));
						outStore.setBaseParam("endDate",
								convertDateObjToFormatStr(Ext
										.getCmp('searchConditionoutFrm')
										.getForm().findField('endDateout')
										.getValue()));
					}
					break;

				case Type_Contract_Integration_Warranty :
					dateRange += " 合同质保期到期"
					if (contractType == Type_ContractType_Income) {
						incomeStore.setBaseParam("begDate",
								convertDateObjToFormatStr(Ext
										.getCmp('searchConditionFrm').getForm()
										.findField('beginDate').getValue()));
						incomeStore.setBaseParam("endDate",
								convertDateObjToFormatStr(Ext
										.getCmp('searchConditionFrm').getForm()
										.findField('endDate').getValue()));
						reconfigure = Type_Contract_Integration_Warranty;
					} else {
						outStore.setBaseParam("begDate",
								convertDateObjToFormatStr(Ext
										.getCmp('searchConditionoutFrm')
										.getForm().findField('beginDateout')
										.getValue()));
						outStore.setBaseParam("endDate",
								convertDateObjToFormatStr(Ext
										.getCmp('searchConditionoutFrm')
										.getForm().findField('endDateout')
										.getValue()));
					}
					break;
			}

			Ext.getCmp(searchId).getStore().load({
				params : {
					start : 0,
					limit : (Ext.getCmp(searchId).getStore().lastOptions == undefined
							|| !Ext.getCmp(searchId).getStore().lastOptions.params || typeof Ext
							.getCmp(searchId).getStore().lastOptions == 'undefined')
							? limitCount
							: Ext.getCmp(searchId).getStore().lastOptions.params.limit
				},
				callback : function(record, options, success) {
					if (success == true) {
					} else {
						Ext.Msg.alert(Ext_errMsg_For_Get_Store_Title,
								Ext_errMsg_For_Get_Store_Message);
					}
				}
			});
			totalMsg.setText(dateRange, false);

		}

		window.warningFilter = function(type) {
			loadData(1, '', type, Type_Contract_Integration_Simple);
		};

		// 收入合同报警提示信息
		if (contractTypeID == 1) {
			var warningTotalCountRecord = Ext.data.Record.create([{
						name : ContractTypeID
					}, {
						name : warningDays
					}, {
						name : warningCount
					}]); // end warningTotalCountRecord

			var warningTotalCountStore = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
					url : 'contractMain.do?method=warningTotalCountForSum&contractTypeID='
							+ Type_ContractType_Income
				}),
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount'
						}, warningTotalCountRecord)
			}); // end warningTotalCountStore

			warningTotalCountStore.setBaseParam("searchName", "");
			warningTotalCountStore.setBaseParam("warningType", "");
			warningTotalCountStore.setBaseParam("searchType",
					Type_Contract_Integration_Simple);
			warningTotalCountStore.setBaseParam("begDate", '');
			warningTotalCountStore.setBaseParam("endDate", '');
			warningTotalCountStore.on('load',
					function(store, records, options) {
						var without_def_count = "", earlier_30_days_count = "", earlier_15_days_count = "", earlier_7_days_count = "";
						var current_date_count = "", overtime_current_date_count = "", overtime_more_7_days_count = "";
						var overtime_more_15_days_count = "", overtime_more_30_days_count = "";
						for (var i = 0; i < records.length; i++) {
							var record = records[i];

							switch (record.get(warningDays)) {
								case Type_Warning_Without_Definition :
									without_def_count = "("
											+ record.get(warningCount) + ")";
									break;

								case Type_Warning_Earlier_30_Days :
									earlier_30_days_count = "("
											+ record.get(warningCount) + ")";
									break;

								case Type_Warning_Earlier_15_Days :
									earlier_15_days_count = "("
											+ record.get(warningCount) + ")";
									break;

								case Type_Warning_Earlier_7_Days :
									earlier_7_days_count = "("
											+ record.get(warningCount) + ")";
									break;

								case Type_Warning_Current_Date :
									current_date_count = "("
											+ record.get(warningCount) + ")";
									break;

								case Type_Warning_Overtime_Current_Date :
									overtime_current_date_count = "("
											+ record.get(warningCount) + ")";
									break;

								case Type_Warning_Overtime_More_7_Days :
									overtime_more_7_days_count = "("
											+ record.get(warningCount) + ")";
									break;

								case Type_Warning_Overtime_More_15_Days :
									overtime_more_15_days_count = "("
											+ record.get(warningCount) + ")";
									break;

								case Type_Warning_Overtime_More_30_Days :
									overtime_more_30_days_count = "("
											+ record.get(warningCount) + ")";
									break;
							}
						}

						var warningLengedBtn = "<table class=grid cellSpacing=1 cellPadding=1><tr class=warning>"
								+ (without_def_count == ""
										? ""
										: ("<td class=\"warning_without_def\"><a href=\"javascript:;\" onclick=\"warningFilter('"
												+ Type_Warning_Without_Definition
												+ "')\")><font style=\"font-size:12px;\">"
												+ Tip_Title_Warning_Without_Definition
												+ without_def_count + "</font></a></td>"))
								+ (earlier_30_days_count == ""
										? ""
										: ("<td class=\"warning_earlier_30\"><a href=\"javascript:;\" onclick=\"warningFilter('"
												+ Type_Warning_Earlier_30_Days
												+ "')\")><font style=\"font-size:12px;\">"
												+ Tip_Title_Warning_Earlier_30_Days
												+ earlier_30_days_count + "</font></a></td>"))
								+ (earlier_15_days_count == ""
										? ""
										: ("<td class=warning_earlier_15\"><a href=\"javascript:;\" onclick=\"warningFilter('"
												+ Type_Warning_Earlier_15_Days
												+ "')\")><font style=\"font-size:12px;\">"
												+ Tip_Title_Warning_Earlier_15_Days
												+ earlier_15_days_count + "</font></a></td>"))
								+ (earlier_7_days_count == ""
										? ""
										: ("<td class=\"warning_earlier_7\"><a href=\"javascript:;\" onclick=\"warningFilter('"
												+ Type_Warning_Earlier_7_Days
												+ "')\")><font style=\"font-size:12px;\">"
												+ Tip_Title_Warning_Earlier_7_Days
												+ earlier_7_days_count + "</font></a></td>"))
								+ (current_date_count == ""
										? ""
										: ("<td class=\"warning_current_date\"><a href=\"javascript:;\" onclick=\"warningFilter('"
												+ Type_Warning_Current_Date
												+ "')\")><font style=\"font-size:12px;\">"
												+ Tip_Title_Warning_Current_Date
												+ current_date_count + "</font></a></td>"))
								+ (overtime_current_date_count == ""
										? ""
										: ("<td class=\"warning_overtime_current_date\"><a href=\"javascript:;\" onclick=\"warningFilter('"
												+ Type_Warning_Overtime_Current_Date
												+ "')\")><font style=\"font-size:12px;\">"
												+ Tip_Title_Warning_Overtime_Current_Date
												+ overtime_current_date_count + "</font></a></td>"))
								+ (overtime_more_7_days_count == ""
										? ""
										: ("<td class=\"warning_overtime_7\"><a href=\"javascript:;\" onclick=\"warningFilter('"
												+ Type_Warning_Overtime_More_7_Days
												+ "')\")><font style=\"font-size:12px;\">"
												+ Tip_Title_Warning_Overtime_More_7_Days
												+ overtime_more_7_days_count + "</font></a></td>"))
								+ (overtime_more_15_days_count == ""
										? ""
										: ("<td class=\"warning_overtime_15\"><a href=\"javascript:;\" onclick=\"warningFilter('"
												+ Type_Warning_Overtime_More_15_Days
												+ "')\")><font style=\"font-size:12px;\">"
												+ Tip_Title_Warning_Overtime_More_15_Days
												+ overtime_more_15_days_count + "</font></a></td>"))
								+ (overtime_more_30_days_count == ""
										? ""
										: ("<td class=\"warning_overtime_30\"><a href=\"javascript:;\" onclick=\"warningFilter('"
												+ Type_Warning_Overtime_More_30_Days
												+ "')\"><font style=\"font-size:12px;\">"
												+ Tip_Title_Warning_Overtime_More_30_Days
												+ overtime_more_30_days_count + "</font></a></td>"))
								+ "</tr></table>";

						legendMsg.setText(warningLengedBtn, false);

					}, this, {
						delay : 100
					});

			warningTotalCountStore.load();
		} // end if

		var legendMsg = new Ext.form.Label({
				// html: warningLengedBtn
				});

		var qxMsg = new Ext.form.Label({});

		if (!decisionFlag) {
			qxMsg.setText('<font color=red>*权限不足，无权查看与金额相关的信息！&nbsp;</font>',
					false);
		}

		var totalMsg = new Ext.form.Label({});

		var gridRecord = Ext.data.Record.create([{
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
					name : ProjectName
				}, {
					name : CurrenyGraph
				}, {
					name : CurrencyID
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
					name : WarrantyEndDate
				}, {
					name : warningDate
				}, {
					name : warningAmt
				}, {
					name : warningDays
				}, {
					name : contractInvoiceAmtInTimeScope
				}, {
					name : contractPayedAmtInTimeScope
				}, {
					name : contractEstimateAmtInTimeScope
				}, {
					name : ProjectNo
				}, {
					name : incomAmtTotalByGl
				}, {
					name : purAmtTotalByGl
				}, {
					name : unIncomAmtTotalByGl
				}, {
					name : delayReason
				}, {
					name : delayOperatorName
				}, {
					name : delayDateOfRegistration
				}, {
					name : delayRegistrantName
				}, {
					name : officialReceiptsMB
					// 财务实收金额
			}	, {
					name : officialReceiptsPayAmt
					// 财务实际支出金额
			}	, {
					name : PaymentIndvanceAmtTotalSum
				}, {
					name : elseIncomeAmtTotal
				}]); // end record

		var gridReader = new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount'
				}, gridRecord); // end reader

		var incomeStore = new Ext.data.Store({
					proxy : new Ext.data.HttpProxy({
								url : __cxtPath
										+ '/contractMain.do?method=integrationSearch'
							}),
					baseParams : {
						"contractTypeID" : 1
					},
					remoteSort : true,
					sortInfo : {
						field : SignatureDate,
						direction : 'ASC'
					},
					reader : gridReader
				}); // end store

		incomeStore.on('load', function(store, records, options) {
					Ext.getCmp('incomeGrid').getColumnModel().setHidden(4,
							false); // 父合同简称
					// Ext.getCmp('incomeGrid').getColumnModel().setHidden(
					// 12, true); // 已开票未到账金额
					Ext.getCmp('incomeGrid').getColumnModel().setHidden(14,
							false);// 质保到期
					Ext.getCmp('incomeGrid').getColumnModel().setHidden(17,
							true); // 合同欠款
					Ext.getCmp('incomeGrid').getColumnModel().setHidden(19,
							true); // 合同欠款
					switch (options.params.searchType) {
						case Type_Contract_Integration_Performed :
							if (deptLimitsFlag) { // 如果有查看业务数据的权限
								Ext.getCmp('incomeGrid').getColumnModel()
										.setHidden(8, false); // 实际收款
								Ext.getCmp('incomeGrid').getColumnModel()
										.setHidden(10, false); // 实际开票
								Ext.getCmp('incomeGrid').getColumnModel()
										.setHidden(14, false); // 预计来款
							}
							// Ext.getCmp('incomeGrid').getColumnModel().setHidden(
							// 4, true); // 父合同简称
							Ext.getCmp('incomeGrid').getColumnModel()
									.setHidden(14, true);
							Ext.getCmp('incomeGrid').getColumnModel()
									.setHidden(16, false); // 财务来款合计
							Ext.getCmp('incomeGrid').getColumnModel()
									.setHidden(18, false); // 财务采购支出
							Ext.getCmp('incomeGrid').getColumnModel()
									.setHidden(17, false); // 财务来款合计
							Ext.getCmp('incomeGrid').getColumnModel()
									.setHidden(19, false); // 财务采购支出
							// Ext.getCmp('incomeGrid').getColumnModel()
							// .setHidden(20, true); // 合同欠款
							reconfigure = -1;
							break;

						case Type_Contract_Integration_Warranty :
							Ext.getCmp('incomeGrid').getColumnModel()
									.setHidden(8, true); // 实际收款
							Ext.getCmp('incomeGrid').getColumnModel()
									.setHidden(10, true); // 实际开票
							Ext.getCmp('incomeGrid').getColumnModel()
									.setHidden(14, true); // 预计来款
							reconfigure = -1;
							break;
						default :
							Ext.getCmp('incomeGrid').getColumnModel()
									.setHidden(8, true); // 实际收款
							Ext.getCmp('incomeGrid').getColumnModel()
									.setHidden(10, true); // 实际开票
							Ext.getCmp('incomeGrid').getColumnModel()
									.setHidden(14, true); // 预计来款
							Ext.getCmp('incomeGrid').getColumnModel()
									.setHidden(15, true); // 质保到期
							Ext.getCmp('incomeGrid').getColumnModel()
									.setHidden(9, true);
							Ext.getCmp('incomeGrid').getColumnModel()
									.setHidden(12, true);
							Ext.getCmp('incomeGrid').getColumnModel()
									.setHidden(15, false);
							Ext.getCmp('incomeGrid').getColumnModel()
									.setHidden(18, false);

							// Ext.getCmp('incomeGrid').getColumnModel()
							// .setHidden(20, false);
							reconfigure = -1;
							break;
					}
				}); // end incomeStore event

		var outStore = new Ext.data.Store({
			proxy : new Ext.data.HttpProxy({
						url : __cxtPath
								+ '/contractMain.do?method=integrationSearchPayout'
					}),
			baseParams : {
				"contractTypeID" : 2
			},
			remoteSort : true,
			sortInfo : {
				field : SignatureDate,
				direction : 'ASC'
			},
			reader : gridReader
		}); // end store

		var deptStore = new Ext.data.JsonStore({
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
						contractTypeID : contractTypeID,
						qxurl : contractTypeID == 1
								? incomeStore.proxy.url
								: outStore.proxy.url
					},
					idProperty : 'deptID',
					autoLoad : true,
					listeners : {
						'load' : function(s, records) {
							if (!records || records.length == 0) {
								if (contractTypeID == 1) {
									Ext.getCmp('incomeGrid').deptCombo
											.setVisible(false);
								} else {
									Ext.getCmp('outGrid').deptCombo
											.setVisible(false);
								}
							}
						}
					}
				});

		Ext.ns('Ext.ux.grid');
		Ext.ux.grid.RowNumberer = Ext.extend(Object, {
					id : 'cnumberer',
					width : 35,
					fixed : true,
					sortable : false,
					hideable : false,
					menuDisabled : true,
					dataIndex : '',
					renderer : function(value, cellmeta, record, rowIndex,
							columnIndex, store) {
						if (store.getCount() - 1 < store.lastOptions.params.limit
								&& store.getCount() == rowIndex + 1) {
							return '<font color=red>&nbsp;<b>合计</b>&nbsp;</font>';
						} else {
							if (rowIndex != store.lastOptions.params.limit) {
								return store.lastOptions.params.start
										+ rowIndex + 1;
							} else {
								return '<font color=red>&nbsp;<b>合计</b>&nbsp;</font>';
							}
						}
					}
				});

		var pcColM = new Ext.grid.ColumnModel([new Ext.ux.grid.RowNumberer(), {
			header : "项目编号",// 1
			dataIndex : ProjectNo,
			sortable : true,
			renderer : function(value, cellMeta, record, rowIndex, columnIndex,
					store) {
				if (value == '' || value == null) {
					return '';
				} else {
					return '<a href="javascript:;" onclick="parent.xMis.turn(\'project-info\',\'项目信息\',\''
							+ __cxtPath
							+ '/projectMain.do?method=getProjectInfo&projectID='
							+ record.get(ProjectID)
							+ '\',\'menu-project-info\')">' + value + '</a>';
				}
			},
			align : 'left'
		}, {
			header : "合同名称",// 2
			dataIndex : ContractName,
			sortable : true,
			renderer : function(value, cellMeta, record, rowIndex, columnIndex,
					store) {

				if (value == '' || value == null) {
					return '';
				} else {
					return gridColOnClick(convertNameWithoutLinker(value)
							+ '^onClick=showContractWin('
							+ record.data[ContractID] + ')');
				}
			},
			align : 'left'
		}, {
			header : "简称",// 3
			id : 'abb',
			width : 150,
			dataIndex : Abbreviation,
			sortable : true,
			renderer : function(value, cellMeta, record, rowIndex, columnIndex,
					store) {
				if (value.trim() == '' || value.trim() == null) {
					return '';
				} else {
					return gridColOnClick(convertNameWithoutLinker(value)
							+ '^onClick=showContractWin('
							+ record.data[ContractID] + ')');
				}
			},
			align : 'left'
		}, {
			header : "签订日期",// 4
			dataIndex : SignatureDate,
			sortable : true,
			renderer : customer_formatDate,
			align : 'center'
		}, {
			header : "甲方公司",// 5
			dataIndex : BuyerAbb,
			sortable : true,
			hidden : true,
			renderer : biuldLinker
		}, {
			header : "<font color=" + Color_Font_Contract_Total_Amt
					+ ">合同金额</font>",// 6
			dataIndex : ContractAmt,
			sortable : true,
			renderer : function(value, cellMeta, record, rowIndex, columnIndex,
					store) {
				if (value == '' || value == null) {
					return '';
				}
				if (store.getCount() - 1 < store.lastOptions.params.limit
						&& store.getCount() == rowIndex + 1) {
					value = Ext.util.Format.number(new Number(value.substr(0,
									value.indexOf('^'))), '0,0.00');
					if (contractTypeID == 2 && !decisionFlag) {
						return '--';
					} else {
						return value;
					}
				} else {
					if (rowIndex == store.lastOptions.params.limit) {
						value = Ext.util.Format.number(new Number(value.substr(
										0, value.indexOf('^'))), '0,0.00');
						if (contractTypeID == 2 && !decisionFlag) {
							return '--';
						} else {
							return value;
						}
					}
				}
				if (contractTypeID == 2 && !decisionFlag) {
					return formatCurrency(record.data[CurrenyGraph], '0', '');
				} else {
					if (record.get(ContractID) == 0) {
						return gridColOnClick(formatCurrency(
								record.data[CurrenyGraph],
								convertNameWithoutLinker(value),
								Color_Font_Contract_Total_Amt)
								+ '^onClick=showAmtDetail('
								+ record.data[ContractID] + ')');
					} else {
						return gridColOnClick(formatCurrency(
								record.data[CurrenyGraph],
								convertNameWithoutLinker(value),
								Color_Font_Contract_Total_Amt)
								+ '^onClick=showContractWin('
								+ record.data[ContractID] + ')');
					}
				}

			},
			align : 'right'
		}, {
			header : "<font color=" + Color_Font_Contract_Total_Charged_Amt
					+ ">已收合同款</font>",
			dataIndex : TotalAmtByID,
			sortable : true, // 7
			hidden : true,
			renderer : function(value, cellMeta, record, rowIndex, columnIndex,
					store) {
				if (store.getCount() - 1 < store.lastOptions.params.limit
						&& store.getCount() == rowIndex + 1) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					if (contractTypeID == 2 && !decisionFlag) {
						return '--';
					} else {
						return value;
					}
				} else {
					if (rowIndex == store.lastOptions.params.limit) {
						value = Ext.util.Format.number(new Number(value),
								'0,0.00');
						if (contractTypeID == 2 && !decisionFlag) {
							return '--';
						} else {
							return value;
						}
					}
				}
				if (contractTypeID == 2 && !decisionFlag) {
					return formatCurrency(record.data[CurrenyGraph], '0',
							Color_Font_Contract_Total_Charged_Amt);
				} else {
					return formatCurrency(
							record.data[CurrenyGraph],
							value
									+ '^onClick=\"showPaymentList(\''
									+ convertNameWithoutLinker(record.data[Abbreviation])
									+ '\',\'' + record.data[ContractID]
									+ '\')\"' + '   id=\'paymentListLink_'
									+ record.data[ContractID] + '\'',
							Color_Font_Contract_Total_Charged_Amt);
				}
			},
			align : 'right'
		}, {
			header : "时段实收合同金额",
			dataIndex : contractPayedAmtInTimeScope,
			sortable : true, // 8
			hidden : true,
			renderer : function(value, cellMeta, record, rowIndex, columnIndex,
					store) {
				return formatCurrency(record.data[CurrenyGraph], value, 'black');

			},
			align : 'right'
		}, {
			header : "<font color=" + Color_Font_Contract_Total_Invoiced_Amt
					+ ">已开发票金额</font>",
			dataIndex : invoiceTotalAmt,
			sortable : true, // 9
			hidden : true,
			renderer : function(value, cellMeta, record, rowIndex, columnIndex,
					store) {
				if (store.getCount() - 1 < store.lastOptions.params.limit
						&& store.getCount() == rowIndex + 1) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					if (contractTypeID == 2 && !decisionFlag) {
						return '--';
					} else {
						return value;
					}
				} else {
					if (rowIndex == store.lastOptions.params.limit) {
						value = Ext.util.Format.number(new Number(value),
								'0,0.00');
						if (contractTypeID == 2 && !decisionFlag) {
							return '--';
						} else {
							return value;
						}
					}
				}
				if (contractTypeID == 2 && !decisionFlag) {
					return formatCurrency(record.data[CurrenyGraph], '0',
							Color_Font_Contract_Total_Invoiced_Amt);
				} else {
					return formatCurrency(
							record.data[CurrenyGraph],
							value
									+ '^onClick=showInvoiceList(\''
									+ convertNameWithoutLinker(record.data[Abbreviation])
									+ '\',' + record.data[ContractID] + ')'
									+ '   id=\'invoiceListLink_'
									+ record.data[ContractID] + '\'',
							Color_Font_Contract_Total_Invoiced_Amt);
				}

			},
			align : 'right'
		}, {
			header : "时段实开发票金额",
			dataIndex : contractInvoiceAmtInTimeScope,
			sortable : true, // 10
			hidden : true,
			renderer : function(value, cellMeta, record, rowIndex, columnIndex,
					store) {
				if (store.getCount() - 1 < store.lastOptions.params.limit
						&& store.getCount() == rowIndex + 1) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					if (contractTypeID == 2 && !decisionFlag) {
						return '--';
					} else {
						return value;
					}
				} else {
					if (rowIndex == store.lastOptions.params.limit) {
						value = Ext.util.Format.number(new Number(value),
								'0,0.00');
						if (contractTypeID == 2 && !decisionFlag) {
							return '--';
						} else {
							return value;
						}
					}
				}
				return formatCurrency(record.data[CurrenyGraph], value, 'black');
			},
			align : 'right'
		}, {
			header : "<font color=" + Color_Font_Contract_Total_UnAccount_Amt
					+ ">已开票未到账金额</font>",
			dataIndex : invoiceUnAccountAmt,
			sortable : true, // 11
			hidden : true,
			renderer : function(value, cellMeta, record, rowIndex, columnIndex,
					store) {
				if (store.getCount() - 1 < store.lastOptions.params.limit
						&& store.getCount() == rowIndex + 1) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					if (contractTypeID == 2 && !decisionFlag) {
						return '--';
					} else {
						return value;
					}
				} else {
					if (rowIndex == store.lastOptions.params.limit) {
						value = Ext.util.Format.number(new Number(value),
								'0,0.00');
						if (contractTypeID == 2 && !decisionFlag) {
							return '--';
						} else {
							return value;
						}
					}
				}
				if (contractTypeID == 2 && !decisionFlag) {
					return formatCurrency(record.data[CurrenyGraph], '0',
							Color_Font_Contract_Total_UnAccount_Amt);
				} else {
					return formatCurrency(record.data[CurrenyGraph], value,
							Color_Font_Contract_Total_UnAccount_Amt);
				}
			},
			align : 'right'
		}, {
			header : "预收合同款", // 12
			dataIndex : PaymentIndvanceAmtTotalSum,
			sortable : true,
			renderer : function(value, cellMeta, record, rowIndex, columnIndex,
					store) {
				if (store.getCount() - 1 < store.lastOptions.params.limit
						&& store.getCount() == rowIndex + 1) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					if (contractTypeID == 2 && !decisionFlag) {
						return '--';
					} else {
						return value;
					}
				} else {
					if (rowIndex == store.lastOptions.params.limit) {
						value = Ext.util.Format.number(new Number(value),
								'0,0.00');
						if (contractTypeID == 2 && !decisionFlag) {
							return '--';
						} else {
							return value;
						}
					}
				}
				if (contractTypeID == 2 && !decisionFlag) {
					return formatCurrency(record.data[CurrenyGraph], '0',
							Color_Font_Contract_Total_UnAccount_Amt);
				} else {
					return formatCurrency(record.data[CurrenyGraph], value,
							Color_Font_Contract_Total_UnAccount_Amt);
				}
			},
			align : 'right'
		}, {
			header : "<font color=" + Color_Font_Contract_Total_UnCharged_Amt
					+ ">未收合同款</font>",
			dataIndex : UnChargedAmtTotal,
			sortable : true, // 13
			renderer : function(value, cellMeta, record, rowIndex, columnIndex,
					store) {
				cellMeta.attr = setWarningBackgroudColor(
						record.data[warningDate], record.data[warningAmt],
						record.data[warningDays], record.data[CurrenyGraph],
						record.data[delayReason]);
				if (store.getCount() - 1 < store.lastOptions.params.limit
						&& store.getCount() == rowIndex + 1) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					if (contractTypeID == 2 && !decisionFlag) {
						return '--';
					} else {
						return value;
					}
				} else {
					if (rowIndex == store.lastOptions.params.limit) {
						value = Ext.util.Format.number(new Number(value),
								'0,0.00');
						if (contractTypeID == 2 && !decisionFlag) {
							return '--';
						} else {
							return value;
						}
					}
				}
				if (contractTypeID == 2 && !decisionFlag) {
					return formatCurrency(record.data[CurrenyGraph], '0',
							Color_Font_Contract_Total_UnCharged_Amt);
				} else {
					return formatCurrency(record.data[CurrenyGraph], value,
							Color_Font_Contract_Total_UnCharged_Amt);
				}

			},
			align : 'right'
		}, {
			header : "预计来款",
			dataIndex : contractEstimateAmtInTimeScope,
			sortable : true, // 14
			hidden : true,
			renderer : function(value, cellMeta, record, rowIndex, columnIndex,
					store) {
				if (store.getCount() - 1 < store.lastOptions.params.limit
						&& store.getCount() == rowIndex + 1) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					if (contractTypeID == 2 && !decisionFlag) {
						return '--';
					} else {
						return value;
					}

				} else {
					if (rowIndex == store.lastOptions.params.limit) {
						value = Ext.util.Format.number(new Number(value),
								'0,0.00');

						if (contractTypeID == 2 && !decisionFlag) {
							return '--';
						} else {
							return value;
						}
					}
				}
				if (contractTypeID == 2 && !decisionFlag) {
					return formatCurrency(record.data[CurrenyGraph], '0',
							'black');
				} else {
					return formatCurrency(record.data[CurrenyGraph], value,
							'black');
				}

			},
			align : 'right'
		}, {
			header : "质保到期", // 15
			dataIndex : WarrantyEndDate,
			sortable : true,
			hidden : true,
			renderer : customer_formatDate,
			align : 'center'
		}, {
			header : "财务来款合计", // 16
			dataIndex : incomAmtTotalByGl,
			sortable : true,
			renderer : function(value, cellMeta, record, rowIndex, columnIndex,
					store) {
				if (store.getCount() - 1 < store.lastOptions.params.limit
						&& store.getCount() == rowIndex + 1) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					if (contractTypeID == 2 && !decisionFlag) {
						return '--';
					} else {
						return value;
					}
				} else {
					if (rowIndex == store.lastOptions.params.limit) {
						value = Ext.util.Format.number(new Number(value),
								'0,0.00');
						if (contractTypeID == 2 && !decisionFlag) {
							return '--';
						} else {
							return value;
						}
					}
				}
				if (contractTypeID == 2 && !decisionFlag) {
					return formatCurrency(record.data[CurrenyGraph], '0',
							Color_Font_Contract_Total_Charged_Amt);
				} else {
					return formatCurrency(
							record.data[CurrenyGraph],
							value
									+ '^onClick=\"showGLIncomeList(\''
									+ convertNameWithoutLinker(record.data[Abbreviation])
									+ '\',\'' + record.data[ProjectID]
									+ '\')\"' + '   id=\'glIncomeListLink_'
									+ record.data[ProjectID] + '\'',
							Color_Font_Contract_Total_Charged_Amt);
				}

			},
			align : 'right'
		}, {
			header : "时段财务实收金额", // 17
			dataIndex : officialReceiptsMB,
			sortable : true,
			hidden : true,
			renderer : function(value, cellMeta, record, rowIndex, columnIndex,
					store) {
				if (store.getCount() - 1 < store.lastOptions.params.limit
						&& store.getCount() == rowIndex + 1) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					if (contractTypeID == 2 && !decisionFlag) {
						return '--';
					} else {
						return value;
					}
				} else {
					if (rowIndex == store.lastOptions.params.limit) {
						value = Ext.util.Format.number(new Number(value),
								'0,0.00');
						if (contractTypeID == 2 && !decisionFlag) {
							return '--';
						} else {
							return value;
						}
					}
				}
				if (contractTypeID == 2 && !decisionFlag) {
					return formatCurrency(record.data[CurrenyGraph], '0',
							Color_Font_Contract_Total_Charged_Amt);
				} else {
					return formatCurrency(record.data[CurrenyGraph], value,
							Color_Font_Contract_Total_Charged_Amt);
				}

			},
			align : 'right'
		}, {
			header : "财务采购支出合计", // 18
			dataIndex : purAmtTotalByGl,
			sortable : true,
			renderer : function(value, cellMeta, record, rowIndex, columnIndex,
					store) {
				if (store.getCount() - 1 < store.lastOptions.params.limit
						&& store.getCount() == rowIndex + 1) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					if (!decisionFlag) {
						return '--';
					} else {
						return value;
					}
				} else {
					if (rowIndex == store.lastOptions.params.limit) {
						value = Ext.util.Format.number(new Number(value),
								'0,0.00');
						if (!decisionFlag) {
							return '--';
						} else {
							return value;
						}
					}
				}
				if (!decisionFlag) {
					return formatCurrency(record.data[CurrenyGraph], '0',
							Color_Font_Contract_Total_Charged_Amt);
				} else {
					return formatCurrency(
							record.data[CurrenyGraph],
							value
									+ '^onClick=\"showGLPurchaseList(\''
									+ convertNameWithoutLinker(record.data[Abbreviation])
									+ '\',\'' + record.data[ProjectID]
									+ '\',\'2\')\"'
									+ '   id=\'glPurchaseListLink_'
									+ record.data[ProjectID] + '\'',
							Color_Font_Contract_Total_Charged_Amt);
				}

			},
			align : 'right'
		}, {
			header : "时段支出实际金额", // 19
			dataIndex : officialReceiptsPayAmt,
			sortable : true,
			hidden : true,
			renderer : function(value, cellMeta, record, rowIndex, columnIndex,
					store) {
				if (store.getCount() - 1 < store.lastOptions.params.limit
						&& store.getCount() == rowIndex + 1) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					if (!decisionFlag) {
						return '--';
					} else {
						return value;
					}
				} else {
					if (rowIndex == store.lastOptions.params.limit) {
						value = Ext.util.Format.number(new Number(value),
								'0,0.00');
						if (!decisionFlag) {
							return '--';
						} else {
							return value;
						}
					}
				}
				if (!decisionFlag) {
					return formatCurrency(record.data[CurrenyGraph], '0',
							Color_Font_Contract_Total_Charged_Amt);
				} else {
					return formatCurrency(record.data[CurrenyGraph], value,
							Color_Font_Contract_Total_Charged_Amt);
				}

			},
			align : 'right'
		}, {
			header : "其它财务来款",
			dataIndex : elseIncomeAmtTotal,
			sortable : true,
			align : 'right',
			renderer : function(value, cellMeta, record, rowIndex, columnIndex,
					store) {
				if (store.getCount() - 1 < store.baseParams.limit
						&& store.getCount() == rowIndex + 1) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					return value;
				} else {
					if (rowIndex == store.baseParams.limit) {
						value = Ext.util.Format.number(new Number(value),
								'0,0.00');
						return value;
					}
				}
				return formatCurrency(record.data[CurrenyGraph],
						new Number(value)
								+ '^onClick="showGLElseIncomeList(\''
								+ convertNameWithoutLinker(Ext
										.isEmpty(record.data[Abbreviation])
										? record.data[ContractName]
										: record.data[Abbreviation]) + '\',\''
								+ record.data[ProjectID] + '\')"'
								+ '   id=\'glElseIncomeListLink_'
								+ record.data[ProjectID] + '\'',
						Color_Font_Contract_Total_Charged_Amt);
			}
		}, {
			header : "合同欠款", // 20
			dataIndex : unIncomAmtTotalByGl,
			sortable : true,
			renderer : function(value, cellMeta, record, rowIndex, columnIndex,
					store) {
				if (store.getCount() - 1 < store.lastOptions.params.limit
						&& store.getCount() == rowIndex + 1) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					if (contractTypeID == 2 && !decisionFlag) {
						return '--';
					} else {
						return value;
					}
				} else {
					if (rowIndex == store.lastOptions.params.limit) {
						value = Ext.util.Format.number(new Number(value),
								'0,0.00');
						if (contractTypeID == 2 && !decisionFlag) {
							return '--';
						} else {
							return value;
						}
					}
				}
				if (contractTypeID == 2 && !decisionFlag) {
					return formatCurrency(record.data[CurrenyGraph], '0',
							Color_Font_Contract_Total_Charged_Amt);
				} else {
					return formatCurrency(record.data[CurrenyGraph], value,
							Color_Font_Contract_Total_Charged_Amt);
				}

			},
			align : 'right'
		}]);
		if (contractTypeID == 1) {
			var dynamicCondition;
			var dynamicConditionPanel = new Ext.Panel({
						baseCls : 'x-plain',
						title : '基本查询',
						bodyStyle : "padding:5 5 5 5",
						buttons : [{
							text : "确认",
							handler : function() {
								dynamicCondition.setBaseParam();
								loadData(1, "", 0,
										Type_Contract_Integration_Simple);
								searchConditionWin.hide();
								dynamicCondition.clear();
							}
						}, {
							text : "取消",
							handler : function() {
								searchConditionWin.hide();
								dynamicCondition.clear();
							}
						}]
					});

			var searchConditionWin = new Ext.Window({
				title : '组合查询',
				modal : true,
				renderTo : Ext.getBody(),
				frame : true,
				plain : true,
				closeAction : "hide",
				resizable : false,
				closable : true,
				width : 270,
				height : 380,
				listeners : {
					hide : function() {
						dynamicCondition.clear();
					}
				},
				items : [{
					xtype : 'tabpanel',
					border : false,
					height : 350,
					activeItem : 1,
					baseCls : 'x-plain',
					items : [dynamicConditionPanel, new Ext.form.FormPanel({
						id : 'searchConditionFrm',
						baseCls : 'x-plain',
						title : '高级查询',
						layout : 'form',
						hideLabels : true,
						width : 250,
						bodyStyle : "padding:5 5 5 5",
						items : [{
									id : 'newContract',
									xtype : 'radio',
									boxLabel : '签订的合同',
									name : 'searchType',
									inputValue : Type_Contract_Integration_New
								}, {
									xtype : 'radio',
									boxLabel : '收付款情况', // <br> 已开发票金额<br>
									// 已开票未到账金额',
									name : 'searchType',
									inputValue : Type_Contract_Integration_Performed
								}, {
									xtype : 'radio',
									boxLabel : '质保期到期',
									name : 'searchType',
									inputValue : Type_Contract_Integration_Warranty
								}, {
									xtype : 'fieldset',
									title : '日期定义',
									autoHeight : true,
									labelAlign : 'right',
									labelWidth : 60,
									style : 'padding :25 5 5 5;!important',
									fream : true,
									items : [new Ext.form.DateField({
												id : 'beginDate',
												format : 'Y-m-d',
												minValue : '1900-01-01',
												fieldLabel : '开始日期',
												showToday : true,
												value : new Date(new Date()
																.getFullYear(),
														0, 1),
												anchor : '100%'
											}), new Ext.form.DateField({
												id : 'endDate',
												format : 'Y-m-d',
												minValue : '1900-01-01',
												fieldLabel : '结束日期',
												showToday : true,
												value : new Date(),
												anchor : '100%'
											})]

								}],
						buttons : [{
							text : "确认",
							handler : function() {

								if (convertDateObjToFormatStr(Ext
										.getCmp('searchConditionFrm').getForm()
										.findField('beginDate').getValue()) == "")
									Ext.Msg.alert("提示", "请选择开始日期！");

								else if (convertDateObjToFormatStr(Ext
										.getCmp('searchConditionFrm').getForm()
										.findField('endDate').getValue()) == "")
									Ext.Msg.alert("提示", "请选择结束日期！");

								else if (!dateCompare(Ext
												.getCmp('searchConditionFrm')
												.getForm()
												.findField('beginDate')
												.getValue(), Ext
												.getCmp('searchConditionFrm')
												.getForm().findField('endDate')
												.getValue()))
									Ext.Msg.alert("提示", "结束日期必须晚于开始日期！");

								else if (Ext.getCmp('searchConditionFrm')
										.getForm().findField('searchType')
										.getGroupValue() == null)
									Ext.Msg.alert("提示", "请选择查询类别！");

								else {
									loadData(1, "", 0,
											Ext.getCmp('searchConditionFrm')
													.getForm()
													.findField('searchType')
													.getGroupValue());
									searchConditionWin.hide();
								}
								dynamicCondition.clear();
							}
						}, {
							text : "取消",
							handler : function() {
								searchConditionWin.hide();
								dynamicCondition.clear();
							}
						}]
					})]
				}]
			});

			Ext.ux.MultiSearchPanel.create(1, function(panel) {
						dynamicCondition = panel;
						dynamicConditionPanel.add(panel);
						dynamicCondition.store = incomeStore;
						dynamicCondition.hideButtonPanel();
					}, this);
			var exp = new Ext.ux.Export({
				formater : [{
							columnHeader : '',
							formatClass : 'com.bitc.jk.xmis.export.PagingRowNumber'
						}, {
							columnHeader : "<font color="
									+ Color_Font_Contract_Total_Amt
									+ ">合同金额</font>",
							formatClass : 'com.bitc.jk.xmis.export.SubstringUrl'
						}, {
							columnHeader : '合同名称',
							formatClass : 'com.bitc.jk.xmis.export.SubstringUrl'
						}, {
							columnHeader : '项目编号',
							formatClass : 'com.bitc.jk.xmis.export.SubstringUrl'
						}, {
							columnHeader : '合同编号',
							formatClass : 'com.bitc.jk.xmis.export.SubstringUrl'
						}, {
							columnHeader : '简称',
							formatClass : 'com.bitc.jk.xmis.export.SubstringUrl'
						}, {
							columnHeader : '质保到期',
							formatClass : 'com.bitc.jk.xmis.export.SubstringTimer'
						}, {
							columnHeader : '签订日期',
							formatClass : 'com.bitc.jk.xmis.export.SubstringTimer'
						}, {
							columnHeader : '其它财务来款',
							formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
						}]
			});
			var exportnameIn = '收入合同综合查询';
			var incomeGrid = new Ext.grid.GridPanel({
				id : 'incomeGrid',
				cm : pcColM,
				store : incomeStore,
				// autoExpandColumn : 'abb',
				stripeRows : true, // True表示使用不同颜色间隔行，默认为false
				region : 'center',
				stateful : true,
				stateId : 'contract-integration-income-grid',
				loadMask : true, // 读取数据时的遮罩和提示功能，默认为false
				loadMask : {
					msg : '正在加载数据，请稍侯……'
				},
				tbar : [{
					xtype : 'combo',
					ref : '../deptCombo',
					width : 120,
					displayField : 'deptName',
					valueField : 'paramValue',
					editable : false,
					triggerAction : 'all',
					emptyText : '请选择部门',
					store : deptStore,
					listeners : {
						'select' : function(combo, record, index) {
							var selValue = record.get('paramValue');
							if (selValue == '-1') {
								delete incomeStore.baseParams['deptID'];
							} else {
								incomeStore.setBaseParam('deptID', selValue);
							}
							delete incomeStore.baseParams['selIds'];
							loadData(contractTypeID, "", 0,
									Type_Contract_Integration_Simple);
						}
					}
				}, '-', '模糊查询：', new Ext.ux.InklingSearchField({
					width : 120,
					store : incomeStore,
					tooltip : {
						title : '模糊查询',
						text : '您可以输入任意关键词进行查询。<br/>可以在关键字前加运算符：>、<、>=、<=、=、<>、like等。<br/>按日期请按如下格式输入：<br/>2010-01-01'
					},
					inklings : [{
								name : '合同名称',
								op : 'like',
								type : 'string'
							}, {
								name : '简称',
								op : 'like',
								type : 'string'
							}, {
								name : '合同编号',
								op : 'like',
								type : 'string'
							}, {
								name : '签订日期',
								op : '=',
								type : 'date'
							}, {
								name : '项目编码',
								op : 'like',
								type : 'string'
							}, {
								name : '合同总额',
								op : '=',
								type : 'double'
							}, {
								name : 'aCompany_name',
								op : 'like',
								type : 'string'
							}],
					onTrigger2Click : function() {
						var params = '';
						Ext.iterate(this.inklings, function(item, index,
										allItems) {
									params += item.name + ',' + item.op + ','
											+ item.type;
									if (index < allItems.length - 1) {
										params += '|';
									}
								}, this);
						this.store.baseParams = this.store.baseParams || {};
						var v = this.getRawValue();
						if (v.length < 1) {
							this.onTrigger1Click();
							return;
						}
						delete this.store.baseParams['selIds'];
						this.store.baseParams[this.paramName] = v;
						this.store.baseParams['inklings'] = params;
						this.triggers[0].show();
						loadData(contractTypeID, "", 0,
								Type_Contract_Integration_Simple);

					},
					onTrigger1Click : function() {
						this.el.dom.value = '';
						this.store.baseParams = this.store.baseParams || {};
						delete this.store.baseParams['inkling'];
						delete this.store.baseParams['inklings'];
						this.triggers[0].hide();
						loadData(contractTypeID, "", 0,
								Type_Contract_Integration_Simple);
					}
				}), '-', {
					ref : '../conditionBtn',
					iconCls : 'condition',
					text : '组合查询',
					handler : function() {
						Ext.getCmp('searchConditionFrm').getForm()
								.findField('searchType').setValue('1');
						searchConditionWin.show();

					}
				}, '->', legendMsg],
				viewConfig : {
					getRowClass : function(record, rowIndex, rowParams, store) {
						if (store.getCount() - 1 < store.lastOptions.params.limit
								&& store.getCount() == rowIndex + 1) {
							return 'grid-contract-total-list-yellow';
						} else {
							if (rowIndex == store.lastOptions.params.limit) {
								return 'grid-contract-total-list-yellow';
							}
						}
					}
				},
				bbar : [totalMsg, '->', new Ext.ux.TotalPagingToolbar({
							id : 'integrationPagingbar' + contractTypeID,
							pageSize : 20,
							store : incomeStore,
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
													exp.onExport(incomeGrid,
															exportnameIn, true);
												}
											}, {
												text : '导出所有数据',
												handler : function() {
													exp
															.onExport(
																	incomeGrid,
																	exportnameIn,
																	false);
												}
											}]
										}
									})]
						})]
			}); // end grid

			Ext.getCmp('incomeGrid').getColumnModel().setColumnHeader(
					7,
					"<font color=" + Color_Font_Contract_Total_Charged_Amt
							+ ">已收合同款</font>");
			Ext.getCmp('incomeGrid').getColumnModel().setColumnHeader(
					9,
					"<font color=" + Color_Font_Contract_Total_Invoiced_Amt
							+ ">已开发票金额</font>");
			Ext.getCmp('incomeGrid').getColumnModel().setHidden(11, true);
			Ext.getCmp('incomeGrid').getColumnModel().setColumnHeader(
					13,
					"<font color=" + Color_Font_Contract_Total_UnCharged_Amt
							+ ">未收合同款</font>");
		} else {

			var dynamicConditionout;
			var dynamicConditionoutPanel = new Ext.Panel({
						baseCls : 'x-plain',
						title : '基本查询',
						bodyStyle : "padding:5 5 5 5",
						buttons : [{
							text : "确认",
							handler : function() {
								dynamicConditionout.setBaseParam();
								loadData(2, "", 0,
										Type_Contract_Integration_Simple);
								searchConditionoutWin.hide();
								dynamicConditionout.clear();
							}
						}, {
							text : "取消",
							handler : function() {
								searchConditionoutWin.hide();
								dynamicConditionout.clear();
							}
						}]
					});

			var searchConditionoutWin = new Ext.Window({
				title : '组合查询',
				modal : true,
				renderTo : Ext.getBody(),
				frame : true,
				plain : true,
				closeAction : "hide",
				resizable : false,
				// maximizable:true,
				closable : true,
				width : 270,
				height : 380,
				listeners : {
					hide : function() {
						dynamicConditionout.clear();
					}
				},
				items : [{
					xtype : 'tabpanel',
					border : false,
					height : 350,
					activeItem : 1,
					baseCls : 'x-plain',
					items : [dynamicConditionoutPanel, new Ext.form.FormPanel({
						id : 'searchConditionoutFrm',
						baseCls : 'x-plain',
						title : '高级查询',
						layout : 'form',
						hideLabels : true,
						width : 250,
						bodyStyle : "padding:5 5 5 5",
						items : [{
									id : 'newContractout',
									xtype : 'radio',
									boxLabel : '签订的合同',
									name : 'searchType',
									inputValue : Type_Contract_Integration_New
								}, {
									xtype : 'radio',
									boxLabel : '收付款情况', // <br> 已开发票金额<br>
									// 已开票未到账金额',
									name : 'searchType',
									inputValue : Type_Contract_Integration_Performed
								}, {
									xtype : 'radio',
									boxLabel : '质保期到期',
									name : 'searchType',
									inputValue : Type_Contract_Integration_Warranty
								}, {
									xtype : 'fieldset',
									title : '日期定义',
									autoHeight : true,
									labelAlign : 'right',
									labelWidth : 60,
									style : 'padding :25 5 5 5;!important',
									fream : true,
									items : [new Ext.form.DateField({
												id : 'beginDateout',
												format : 'Y-m-d',
												// maxValue: new Date(),
												minValue : '1900-01-01',
												fieldLabel : '开始日期',
												showToday : true,
												value : new Date(new Date()
																.getFullYear(),
														0, 1),
												anchor : '100%'
											}), new Ext.form.DateField({
												id : 'endDateout',
												format : 'Y-m-d',
												// maxValue: new Date(),
												minValue : '1900-01-01',
												fieldLabel : '结束日期',
												showToday : true,
												value : new Date(),
												anchor : '100%'
											})]

								}],
						buttons : [{
							text : "确认",
							handler : function() {

								if (convertDateObjToFormatStr(Ext
										.getCmp('searchConditionoutFrm')
										.getForm().findField('beginDateout')
										.getValue()) == "")
									Ext.Msg.alert("提示", "请选择开始日期！");

								else if (convertDateObjToFormatStr(Ext
										.getCmp('searchConditionoutFrm')
										.getForm().findField('endDateout')
										.getValue()) == "")
									Ext.Msg.alert("提示", "请选择结束日期！");

								else if (!dateCompare(
										Ext.getCmp('searchConditionoutFrm')
												.getForm()
												.findField('beginDateout')
												.getValue(),
										Ext.getCmp('searchConditionoutFrm')
												.getForm()
												.findField('endDateout')
												.getValue()))
									Ext.Msg.alert("提示", "结束日期必须晚于开始日期！");

								else if (Ext.getCmp('searchConditionoutFrm')
										.getForm().findField('searchType')
										.getGroupValue() == null)
									Ext.Msg.alert("提示", "请选择查询类别！");

								else {
									loadData(2, "", 0,
											Ext.getCmp('searchConditionoutFrm')
													.getForm()
													.findField('searchType')
													.getGroupValue());
									searchConditionoutWin.hide();
								}
								dynamicConditionout.clear();
							}
						}, {
							text : "取消",
							handler : function() {
								searchConditionoutWin.hide();
								dynamicConditionout.clear();
							}
						}]
					})]
				}]
			});

			Ext.ux.MultiSearchPanel.create(4, function(panel) {
						dynamicConditionout = panel;
						dynamicConditionoutPanel.add(panel);
						dynamicConditionout.store = outStore;
						dynamicConditionout.hideButtonPanel();
					}, this);
			var exp2 = new Ext.ux.Export({
				formater : [{
							columnHeader : '',
							formatClass : 'com.bitc.jk.xmis.export.PagingRowNumber'
						}, {
							columnHeader : "<font color="
									+ Color_Font_Contract_Total_Amt
									+ ">合同金额</font>",
							formatClass : 'com.bitc.jk.xmis.export.SubstringUrl'
						}, {
							columnHeader : '合同名称',
							formatClass : 'com.bitc.jk.xmis.export.SubstringUrl'
						}, {
							columnHeader : '项目编号',
							formatClass : 'com.bitc.jk.xmis.export.SubstringUrl'
						}, {
							columnHeader : '合同编号',
							formatClass : 'com.bitc.jk.xmis.export.SubstringUrl'
						}, {
							columnHeader : '简称',
							formatClass : 'com.bitc.jk.xmis.export.SubstringUrl'
						}, {
							columnHeader : '质保到期',
							formatClass : 'com.bitc.jk.xmis.export.SubstringTimer'
						}, {
							columnHeader : '签订日期',
							formatClass : 'com.bitc.jk.xmis.export.SubstringTimer'
						}]
			});

			var exportnameOut = '支出合同综合查询';
			var outGrid = new Ext.grid.GridPanel({
				id : 'outGrid',
				cm : pcColM,
				store : outStore,
				// autoExpandColumn : 'abb',
				stripeRows : true, // True表示使用不同颜色间隔行，默认为false
				region : 'center',
				stateful : true,
				stateId : 'contract-integration-income-grid',
				loadMask : true, // 读取数据时的遮罩和提示功能，默认为false
				loadMask : {
					msg : '正在加载数据，请稍侯……'
				},
				tbar : [{
					xtype : 'combo',
					ref : '../deptCombo',
					width : 120,
					displayField : 'deptName',
					valueField : 'paramValue',
					editable : false,
					triggerAction : 'all',
					emptyText : '请选择部门',
					store : deptStore,
					listeners : {
						'select' : function(combo, record, index) {
							var selValue = record.get('paramValue');
							if (selValue == '-1') {
								delete outStore.baseParams['deptID'];
							} else {
								outStore.setBaseParam('deptID', selValue);
							}
							delete outStore.baseParams['selIds'];
							loadData(contractTypeID, "", 0,
									Type_Contract_Integration_Simple);
						}
					}
				}, '-', '模糊查询：', new Ext.ux.InklingSearchField({
					width : 120,
					store : outStore,
					tooltip : {
						title : '模糊查询',
						text : '您可以输入任意关键词进行查询。<br/>可以在关键字前加运算符：>、<、>=、<=、=、<>、like等。<br/>按日期请按如下格式输入：<br/>2010-01-01'
					},
					inklings : [{
								name : '合同名称',
								op : 'like',
								type : 'string'
							}, {
								name : '简称',
								op : 'like',
								type : 'string'
							}, {
								name : '合同编号',
								op : 'like',
								type : 'string'
							}, {
								name : '签订日期',
								op : '=',
								type : 'date'
							}, {
								name : '项目编码',
								op : 'like',
								type : 'string'
							}, {
								name : '合同总额',
								op : '=',
								type : 'double'
							}, {
								name : 'aCompany_name',
								op : 'like',
								type : 'string'
							}],
					onTrigger2Click : function() {
						var params = '';
						Ext.iterate(this.inklings, function(item, index,
										allItems) {
									params += item.name + ',' + item.op + ','
											+ item.type;
									if (index < allItems.length - 1) {
										params += '|';
									}
								}, this);
						this.store.baseParams = this.store.baseParams || {};
						var v = this.getRawValue();
						if (v.length < 1) {
							this.onTrigger1Click();
							return;
						}
						delete this.store.baseParams['selIds'];
						this.store.baseParams[this.paramName] = v;
						this.store.baseParams['inklings'] = params;
						this.triggers[0].show();
						loadData(contractTypeID, "", 0,
								Type_Contract_Integration_Simple);

					},
					onTrigger1Click : function() {
						this.el.dom.value = '';
						this.store.baseParams = this.store.baseParams || {};
						delete this.store.baseParams['inkling'];
						delete this.store.baseParams['inklings'];
						this.triggers[0].hide();
						loadData(contractTypeID, "", 0,
								Type_Contract_Integration_Simple);
					}
				}), '-', {
					ref : '../conditionBtn',
					iconCls : 'condition',
					text : '组合查询',
					handler : function() {
						Ext.getCmp('searchConditionoutFrm').getForm()
								.findField('searchType').setValue('1');
						searchConditionoutWin.show();
					}
				}, '->', qxMsg],
				viewConfig : {
					getRowClass : function(record, rowIndex, rowParams, store) {
						if (store.getCount() - 1 < store.lastOptions.params.limit
								&& store.getCount() == rowIndex + 1) {
							return 'grid-contract-total-list-yellow';
						} else {
							if (rowIndex == store.lastOptions.params.limit) {
								return 'grid-contract-total-list-yellow';
							}
						}
					}
				},
				bbar : [totalMsg, '->', new Ext.ux.TotalPagingToolbar({
					id : 'integrationPagingbar' + contractTypeID,
					pageSize : 20,
					store : outStore,
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
									exp2.onExport(outGrid, exportnameOut, true);
								}
							}, {
								text : '导出所有数据',
								handler : function() {
									exp2
											.onExport(outGrid, exportnameOut,
													false);
								}
							}]
						}
					})]
				})]
			}); // end grid
			Ext.getCmp('outGrid').getColumnModel().setDataIndex(1, ContractNo);
			Ext.getCmp('outGrid').getColumnModel().setColumnHeader(1, "合同编号");
			Ext.getCmp('outGrid').getColumnModel().setRenderer(
					1,
					function(value, cellMeta, record, rowIndex, columnIndex,
							store) {
						if (value == '' || value == null) {
							return '';
						} else {
							return gridColOnClick(convertNameWithoutLinker(value)
									+ '^onClick=showContractWin('
									+ record.data[ContractID] + ')');
						}
					});
			Ext.getCmp('outGrid').getColumnModel().setColumnHeader(
					7,
					"<font color=" + Color_Font_Contract_Total_Charged_Amt
							+ ">已付合同款</font>");
			Ext.getCmp('outGrid').getColumnModel().setColumnHeader(
					9,
					"<font color=" + Color_Font_Contract_Total_Invoiced_Amt
							+ ">已报销金额</font>");
			Ext.getCmp('outGrid').getColumnModel().setHidden(17, true);
			Ext.getCmp('outGrid').getColumnModel().setColumnHeader(
					13,
					"<font color=" + Color_Font_Contract_Total_UnCharged_Amt
							+ ">未付合同款</font>");
			Ext.getCmp('outGrid').getColumnModel().setHidden(16, true);
			Ext.getCmp('outGrid').getColumnModel().setHidden(18, true);
			Ext.getCmp('outGrid').getColumnModel().setHidden(20, true);
		}
		return contractTypeID == 1 ? incomeGrid : outGrid;
	} // end buildGrid
});
