Ext.ns('com.bitc.xmis.contract');
com.bitc.xmis.SearchPanel = Ext.extend(Ext.Panel, {
			name : '',
			inputValue : '',
			timeId : '',
			boxLabel : '',
			border : false,
			frame : true,
			baseCls : 'x-fieldset',
			style : 'padding :0 0 0 0;!important',
			layout : 'column',
			initComponent : function() {
				var timeId = this.timeId;
				this.items = [{
							xtype : 'radio',
							columnWidth : .3,
							boxLabel : this.boxLabel,
							name : this.name,
							inputValue : this.inputValue,
							listeners : {
								'check' : function(checkbox, checked) {
									if (checked) {
										setDisabledTimeOption(checkbox, timeId);
									}
								}
							}
						}, new Ext.form.DateField({
									columnWidth : .3,
									id : 'beginDateFor' + timeId,
									disabled : true,
									format : 'Y-m-d',
									minValue : '1900-01-01',
									fieldLabel : '开始日期',
									showToday : true,
									value : new Date(new Date().getFullYear(),
											0, 1),
									anchor : '100%'
								}), {
							xtype : 'displayfield',
							id : 'symbolFor' + timeId,
							disabled : true,
							value : '&nbsp;-&nbsp;'
						}, new Ext.form.DateField({
									columnWidth : .3,
									id : 'endDateFor' + timeId,
									disabled : true,
									format : 'Y-m-d',
									minValue : '1900-01-01',
									fieldLabel : '结束日期',
									showToday : true,
									value : new Date(),
									anchor : '100%'
								})];
				com.bitc.xmis.SearchPanel.superclass.initComponent.call(this);
			}
		});

Ext.onReady(function() {
	Ext.QuickTips.init();
	Ext.apply(Ext.QuickTips.getQuickTip(), {
				maxWidth : 500
			});

	var tip1 = '<table><tr><td><b>已开票未到账金额</b></td></tr><tr><td>已开票未到账金额 = 已开发票金额 - 财务来款合计<br>若已开发票金额 &lt; 财务来款合计,已开票未到账金额 = 0</td></tr></table>';
	var tip2 = '<table><tr><td><b>按进度应收账款</b></td></tr><tr><td>按进度应收账款 = 按进度预计来款合计  - 财务来款合计<br>若按进度预计来款合计 &lt; 财务来款合计,按进度应收账款 = 0</td></tr></table>';
	var tip3 = '<table><tr><td><b>按进度已开票未收款</b></td></tr><tr><td>A = 按进度预计来款合计;B = 已开发票金额;C = 财务来款合计<br>1.	当A &le; B, 按进度已开票未收款 = A - C.若A &lt; C, 按进度已开票未收款 = 0<br>2.	当A &gt; B<br>&nbsp;&nbsp;&diams;当B &le; C, 按进度已开票未收款 = 0<br>&nbsp;&nbsp;&diams;当B &gt; C, 按进度已开票未收款 = B - C</td></tr></table>';
	var tip4 = '<table><tr><td><b>按进度未开票应收款</b></td></tr><tr><td>A = 按进度预计来款合计;B = 已开发票金额;C = 财务来款合计<br>1.	当A &le; B, 按进度未开票应收款 = 0 <br>2.	当A &gt; B<br>&nbsp;&nbsp;&diams;当B &gt; C, 按进度未开票应收款 = A - B<br>&nbsp;&nbsp;&diams;当B &lt; C, 按进度未开票应收款 = A - C<br>&nbsp;&nbsp;&diams;当B = C, 按进度未开票应收款 = A - B(或C)</td></tr></table>';
	var tip5 = '<table><tr><td><b>预收合同款</b></td></tr><tr><td>预收合同款 = 财务来款合计  - 已开发票金额<br>若财务来款合计 &lt; 已开发票金额,预收合同款 = 0</td></tr></table>';

	var pcPageSize = 20;
	var outPageSize = 20;

	var exportnameIn = '收入合同';
	var exportnameOut = '支出合同';

	// 收入时间
	var beginDateLocal = new Date(new Date().getFullYear(), 0, 1);
	var endDateLocal = new Date();

	// 支出时间
	var beginDateOutLocal = new Date(new Date().getFullYear(), 0, 1);
	var endDateOutLocal = new Date();

	var incomeCombinationSearchType = 1;// 收入合同查询类型标记(基本/高级)
	var outCombinationSearchType = 1;// 支出合同查询类型标记(基本/高级)

	// 当前用户有查看业务数据的权限，默认model
	var incomeBusinessWithBusinessAuthorityColumn = [0, 44, 1, 2, 3, 4, 5, 6,
			7, 34, 35, 36, 37, 38, 39, 40, 41, 42, 8, 9, 45, 11, 12, 14, 15,
			16, 17, 18, 19, 21, 22, 23, 24, 25, 26, 28, 29, 43, 31, 33, 46];
	// 当前用户无查看业务数据的权限，默认model
	var incomeBusinessWithoutBusinessAuthorityColumn = [0, 44, 1, 2, 3, 4, 5,
			6, 7, 34, 36, 37, 38, 39, 40, 41, 42, 8, 21, 22, 23, 24, 25, 26,
			28, 29, 43, 31, 33, 46];

	// 仅业务查询
	// 当前用户有查看业务数据的权限，高级查询-收付款情况，默认model
	// var incomeIntegrationWithBusinessAuthorityByPerformedColumn = [ 0, 1,
	// 2, 4, 5, 6, 8, 9, 10, 11, 13, 15, 19, 20, 30, 32,
	// 34 ];
	var incomeIntegrationWithBusinessAuthorityByPerformedColumn = [0, 44, 1, 2,
			3, 4, 5, 6, 7, 34, 35, 36, 37, 38, 39, 40, 41, 42, 8, 9, 10, 45,
			11, 12, 14, 15, 16, 17, 18, 19, 21, 22, 23, 24, 25, 26, 28, 29, 43,
			31, 33, 46];

	// 当前用户有查看业务数据的权限，高级查询-发票情况，默认model
	var incomeIntegrationWithBusinessAuthorityByInvoiceColumn = [0, 44, 1, 2,
			3, 4, 5, 6, 7, 34, 35, 36, 37, 38, 39, 40, 41, 42, 8, 9, 45, 11,
			12, 13, 14, 15, 16, 17, 18, 19, 21, 22, 23, 24, 25, 26, 28, 29, 43,
			31, 33, 46];

	// 当前用户有查看业务数据的权限，高级查询- 预计付款，默认model
	var incomeIntegrationWithBusinessAuthorityByEstimateContractAmtColumn = [0,
			44, 1, 2, 3, 4, 5, 6, 7, 34, 35, 36, 37, 38, 39, 40, 41, 42, 8, 9,
			45, 11, 12, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 28,
			29, 43, 31, 33, 46];
	// 仅业务查询

	// 当前用户有查看业务数据的权限，高级查询-新签订合同，默认model
	var incomeIntegrationWithBusinessAuthorityByNewColumn = [0, 1, 44, 2, 3, 4,
			5, 6, 7, 34, 35, 36, 37, 38, 39, 40, 41, 42, 8, 9, 45, 11, 12, 14,
			15, 16, 17, 18, 19, 21, 22, 23, 24, 25, 26, 28, 29, 43, 31, 33, 46];
	// 当前用户无查看业务数据的权限，高级查询-新签订合同，默认model
	var incomeIntegrationWithoutBusinessAuthorityByNewColumn = [0, 44, 1, 2, 3,
			4, 5, 6, 7, 34, 36, 37, 38, 39, 40, 41, 42, 8, 21, 22, 23, 24, 25,
			26, 28, 29, 43, 31, 33, 46];
	// 当前用户有查看业务数据的权限，高级查询-财务情况，默认model
	var incomeIntegrationWithBusinessAuthorityByFinanceColumn = [0, 44, 1, 2,
			3, 4, 5, 6, 7, 34, 35, 36, 37, 38, 39, 40, 41, 42, 8, 9, 45, 11,
			12, 14, 15, 16, 17, 18, 19, 21, 22, 23, 24, 25, 26, 28, 29, 43, 30,
			31, 32, 33, 46];
	// 当前用户无查看业务数据的权限，高级查询-财务情况，默认model
	var incomeIntegrationWithoutBusinessAuthorityByFinanceColumn = [0, 44, 1,
			2, 3, 4, 5, 6, 7, 34, 36, 37, 38, 39, 40, 41, 42, 8, 21, 22, 23,
			24, 25, 26, 28, 29, 43, 30, 31, 32, 33, 46];

	// 当前用户有查看业务数据的权限，高级查询-质保期到期，默认model
	var incomeIntegrationWithBusinessAuthorityByWarrantyColumn = [0, 44, 1, 2,
			3, 4, 5, 27, 6, 7, 34, 35, 36, 37, 38, 39, 40, 41, 42, 8, 9, 11,
			12, 14, 15, 16, 17, 18, 19, 21, 22, 23, 24, 25, 26, 28, 29, 43, 31,
			33, 46];
	// 当前用户无查看业务数据的权限，高级查询-质保期到期，默认model
	var incomeIntegrationWithoutBusinessAuthorityByWarrantyColumn = [0, 44, 1,
			2, 3, 4, 5, 6, 7, 34, 36, 37, 38, 39, 40, 41, 42, 8, 21, 22, 23,
			24, 25, 26, 27, 28, 29, 43, 31, 33, 46];

	// 支出默认model
	var outBusinessWithBusinessAuthorityColumn = [0, 1, 2, 3, 4, 5, 6, 21, 7,
			9, 10, 12, 13, 14, 15, 16, 17, 18, 19];
	// 支出综合收付款情况
	var outBusinessWithBusinessAuthorityByPerformedColumn = [0, 1, 2, 3, 4, 5,
			6, 21, 7, 8, 9, 10, 12, 13, 14, 15, 16, 17, 18, 19];
	// 支出综合发票情况
	var outBusinessWithBusinessAuthorityByInvoiceColumn = [0, 1, 2, 3, 4, 5, 6,
			21, 7, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19];
	// 支出质保期
	var outBusinessWithBusinessAuthorityByWarrantyColumn = [0, 1, 2, 3, 4, 20,
			5, 6, 21, 7, 9, 10, 12, 13, 14, 15, 16, 17, 18, 19];

	// allArray为所有列，targetArray为目标列索引，返回目标列
	function getColumns(allArray, targetArray) {
		var pcArray = new Array();
		for (var i = 0; i < targetArray.length; i++) {
			pcArray.push(Ext.apply({}, allArray[targetArray[i]]));
		}
		return pcArray;
	}
	// Type_Contract_Integration_Simple = 1; // 正常查询(业务)
	// Type_Contract_Integration_New = 2; // 新签订的合同
	// Type_Contract_Integration_Performed = 3; // 收付款情况
	// Type_Contract_Integration_EstimateContractAmt = 4; // 预计来款
	// Type_Contract_Integration_Warranty = 5; // 质保期到期
	// Type_Contract_Integration_Invoice = 6; // 发票情况
	// Type_Contract_Integration_Finance = 7; // 财务情况
	function changeGrid(contractType, searchType, deptLimitsFlag) {
		if (contractType == Type_ContractType_Income) { // 收入合同
			if (searchType == Type_Contract_Integration_Simple) {
				var colmodel = getColumns(allIncomeColumn,
						incomeBusinessWithBusinessAuthorityColumn);
				if (!deptLimitsFlag) {
					colmodel = getColumns(allIncomeColumn,
							incomeBusinessWithoutBusinessAuthorityColumn);
				}
				pcColM.setConfig(colmodel, false);
				incomeGrid.reconfigure(pcStore, pcColM);
				pcPagingToolbar.bindStore(pcStore, true);
			} else {
				var colmodel;
				if (searchType == Type_Contract_Integration_New) {
					colmodel = getColumns(allIncomeColumn,
							incomeIntegrationWithBusinessAuthorityByNewColumn);
					if (!deptLimitsFlag) {
						colmodel = getColumns(allIncomeColumn,
								incomeIntegrationWithoutBusinessAuthorityByNewColumn);
					}
				} else if (searchType == Type_Contract_Integration_Performed) {
					colmodel = getColumns(allIncomeColumn,
							incomeIntegrationWithBusinessAuthorityByPerformedColumn);
				} else if (searchType == Type_Contract_Integration_Invoice) {
					colmodel = getColumns(allIncomeColumn,
							incomeIntegrationWithBusinessAuthorityByInvoiceColumn);
				} else if (searchType == Type_Contract_Integration_EstimateContractAmt) {
					colmodel = getColumns(allIncomeColumn,
							incomeIntegrationWithBusinessAuthorityByEstimateContractAmtColumn);
				} else if (searchType == Type_Contract_Integration_Finance) {
					colmodel = getColumns(allIncomeColumn,
							incomeIntegrationWithBusinessAuthorityByFinanceColumn);
					if (!deptLimitsFlag) {
						colmodel = getColumns(allIncomeColumn,
								incomeIntegrationWithoutBusinessAuthorityByFinanceColumn);
					}
				} else {
					colmodel = getColumns(allIncomeColumn,
							incomeIntegrationWithBusinessAuthorityByWarrantyColumn);
					if (!deptLimitsFlag) {
						colmodel = getColumns(allIncomeColumn,
								incomeIntegrationWithoutBusinessAuthorityByWarrantyColumn);
					}
				}
				pcColM.setConfig(colmodel, false);
				incomeGrid.reconfigure(incomeStore, pcColM);
				pcPagingToolbar.bindStore(incomeStore, true);
			}
		} else { // 支出合同
			if (searchType != Type_Contract_Integration_Simple) {
				if (searchType == Type_Contract_Integration_Performed) {
					var colmodel = getColumns(allOutColumn,
							outBusinessWithBusinessAuthorityByPerformedColumn);
					outColM.setConfig(colmodel, false);
				} else if (searchType == Type_Contract_Integration_Invoice) {
					var colmodel = getColumns(allOutColumn,
							outBusinessWithBusinessAuthorityByInvoiceColumn);
					outColM.setConfig(colmodel, false);
				} else if (searchType == Type_Contract_Integration_Warranty) {
					var colmodel = getColumns(allOutColumn,
							outBusinessWithBusinessAuthorityByWarrantyColumn);
					outColM.setConfig(colmodel, false);
				} else {
					var colmodel = getColumns(allOutColumn,
							outBusinessWithBusinessAuthorityColumn);
					outColM.setConfig(colmodel, false);
				}
				outGrid.reconfigure(searchStore, outColM);
				outPagingToolbar.bindStore(searchStore, true);
			} else {
				var colmodel = getColumns(allOutColumn,
						outBusinessWithBusinessAuthorityColumn);
				outColM.setConfig(colmodel, false);
				outGrid.reconfigure(outStore, outColM);
				outPagingToolbar.bindStore(outStore, true);
			}
		}
	}

	// 组合查询load数据
	function loadData(contractType, searchName, warningType, integrationType) {
		pcPagingToolbar.promptMsg = '';
		// 默认pcPageSize = 20
		var dateRange = " ";
		// 查询类型
		integrationTypeFlag = integrationType;
		var searchId = 'incomeGrid';
		if (contractType == Type_ContractType_Payout) {
			searchId = 'outGrid';
		}
		Ext.getCmp(searchId).getStore().setDefaultSort(SignatureDate, 'ASC');
		Ext.getCmp(searchId).getStore().removeAll();
		Ext.getCmp(searchId).getStore().setBaseParam("currencyGraph", "");
		Ext.getCmp(searchId).getStore().setBaseParam("searchName", searchName);
		Ext.getCmp(searchId).getStore().setBaseParam("contractTypeID",
				contractType);
		Ext.getCmp(searchId).getStore()
				.setBaseParam("warningType", warningType);
		Ext.getCmp(searchId).getStore().setBaseParam("searchType",
				integrationType);
		Ext.getCmp(searchId).getStore().setBaseParam("begDate", '');
		Ext.getCmp(searchId).getStore().setBaseParam("endDate", '');

		if (integrationTypeFlag == Type_Contract_Integration_Simple) {
			if (contractType == Type_ContractType_Income) {
				changeGrid(Type_ContractType_Income, 1, deptLimitsFlag);
				pcStore.setBaseParam("searchType", integrationType);

				// 给基本查询加入合同来源，部门，模糊查询，执行部门参数--收入合同
				if (Ext.getCmp('incomeSourceTypeID').getValue() == 0) {
					delete pcStore.baseParams['sourceID'];
				} else {
					pcStore.setBaseParam('sourceID', Ext
									.getCmp('incomeSourceTypeID').getValue());
				}
				if (Ext.getCmp('incomeDeptID').getValue() == '-1') {
					delete pcStore.baseParams['deptID'];
				} else {
					pcStore.setBaseParam('deptID', Ext.getCmp('incomeDeptID')
									.getValue());
				}
				var incomeInklingCmp = Ext.getCmp('incomeInklingID');
				if (incomeInklingCmp.getValue() == '') {
					delete pcStore.baseParams['inklings'];
					delete pcStore.baseParams[incomeInklingCmp.paramName];
				} else {
					var inklingsParams = '';
					Ext.iterate(incomeInklingCmp.inklings, function(item,
									index, allItems) {
								inklingsParams += item.name + ',' + item.op
										+ ',' + item.type;
								if (index < allItems.length - 1) {
									inklingsParams += '|';
								}
							}, incomeInklingCmp);
					pcStore.setBaseParam('inklings', inklingsParams);
					pcStore.setBaseParam(incomeInklingCmp.paramName,
							incomeInklingCmp.getValue());
				}
				if (Ext.getCmp('incomeExcuteDeptID').getValue() == 0) {
					delete pcStore.baseParams['excuteDeptID'];
				} else {
					pcStore.setBaseParam('excuteDeptID', Ext
									.getCmp('incomeExcuteDeptID').getValue());
				}

			} else {
				changeGrid(Type_ContractType_Payout, 1, deptLimitsFlag);
				outStore.setBaseParam("searchType", integrationType);

				// 给基本查询加入部门，模糊查询参数--支出合同
				if (Ext.getCmp('outDeptID').getValue() == '-1') {
					delete outStore.baseParams['deptID'];
				} else {
					outStore.setBaseParam('deptID', Ext.getCmp('outDeptID')
									.getValue());
				}
				var outInklingCmp = Ext.getCmp('outInklingID');
				if (outInklingCmp.getValue() == '') {
					delete outStore.baseParams['inklings'];
					delete outStore.baseParams[outInklingCmp.paramName];
				} else {
					var inklingsParams = '';
					Ext.iterate(outInklingCmp.inklings, function(item, index,
									allItems) {
								inklingsParams += item.name + ',' + item.op
										+ ',' + item.type;
								if (index < allItems.length - 1) {
									inklingsParams += '|';
								}
							}, outInklingCmp);
					outStore.setBaseParam('inklings', inklingsParams);
					outStore.setBaseParam(outInklingCmp.paramName,
							outInklingCmp.getValue());
				}
			}

		} else {
			if (contractType == Type_ContractType_Income) {
				dateRange = convertDateObjToFormatStr(beginDateLocal) + "至"
						+ convertDateObjToFormatStr(endDateLocal);
				changeGrid(Type_ContractType_Income, integrationTypeFlag,
						deptLimitsFlag);
				incomeStore.setBaseParam("searchType", integrationType);

				// 给高级查询加入合同来源，部门，模糊查询，执行部门参数--收入合同
				if (Ext.getCmp('incomeSourceTypeID').getValue() == 0) {
					delete incomeStore.baseParams['sourceID'];
				} else {
					incomeStore.setBaseParam('sourceID', Ext
									.getCmp('incomeSourceTypeID').getValue());
				}
				if (Ext.getCmp('incomeDeptID').getValue() == '-1') {
					delete incomeStore.baseParams['deptID'];
				} else {
					incomeStore.setBaseParam('deptID', Ext
									.getCmp('incomeDeptID').getValue());
				}
				var incomeInklingSeniorCmp = Ext.getCmp('incomeInklingID');
				if (incomeInklingSeniorCmp.getValue() == '') {
					delete incomeStore.baseParams['inklings'];
					delete incomeStore.baseParams[incomeInklingSeniorCmp.paramName];
				} else {
					var inklingsParams = '';
					Ext.iterate(incomeInklingSeniorCmp.inklings, function(item,
									index, allItems) {
								inklingsParams += item.name + ',' + item.op
										+ ',' + item.type;
								if (index < allItems.length - 1) {
									inklingsParams += '|';
								}
							}, incomeInklingSeniorCmp);
					incomeStore.setBaseParam('inklings', inklingsParams);
					incomeStore.setBaseParam(incomeInklingSeniorCmp.paramName,
							incomeInklingSeniorCmp.getValue());
				}
				if (Ext.getCmp('incomeExcuteDeptID').getValue() == 0) {
					delete incomeStore.baseParams['excuteDeptID'];
				} else {
					incomeStore.setBaseParam('excuteDeptID', Ext
									.getCmp('incomeExcuteDeptID').getValue());
				}

			} else {
				dateRange = convertDateObjToFormatStr(beginDateOutLocal) + "至"
						+ convertDateObjToFormatStr(endDateOutLocal);
				changeGrid(Type_ContractType_Payout, integrationTypeFlag,
						deptLimitsFlag);
				searchStore.setBaseParam("searchType", integrationType);

				// 给高级查询加入部门，模糊查询参数--支出合同
				if (Ext.getCmp('outDeptID').getValue() == '-1') {
					delete searchStore.baseParams['deptID'];
				} else {
					searchStore.setBaseParam('deptID', Ext.getCmp('outDeptID')
									.getValue());
				}
				var outInklingSeniorCmp = Ext.getCmp('outInklingID');
				if (outInklingSeniorCmp.getValue() == '') {
					delete searchStore.baseParams['inklings'];
					delete searchStore.baseParams[outInklingSeniorCmp.paramName];
				} else {
					var inklingsParams = '';
					Ext.iterate(outInklingSeniorCmp.inklings, function(item,
									index, allItems) {
								inklingsParams += item.name + ',' + item.op
										+ ',' + item.type;
								if (index < allItems.length - 1) {
									inklingsParams += '|';
								}
							}, outInklingSeniorCmp);
					searchStore.setBaseParam('inklings', inklingsParams);
					searchStore.setBaseParam(outInklingSeniorCmp.paramName,
							outInklingSeniorCmp.getValue());
				}
			}
		}

		switch (integrationType) {
			case Type_Contract_Integration_Simple :
				dateRange = "";
				break;
			case Type_Contract_Integration_New :

				dateRange += " 新签合同统计";
				if (contractType == Type_ContractType_Income) {
					Ext.getCmp(searchId).getStore().setBaseParam("begDate",
							convertDateObjToFormatStr(beginDateLocal));
					Ext.getCmp(searchId).getStore().setBaseParam("endDate",
							convertDateObjToFormatStr(endDateLocal));

				} else {
					Ext.getCmp(searchId).getStore().setBaseParam("begDate",
							convertDateObjToFormatStr(beginDateOutLocal));
					Ext.getCmp(searchId).getStore().setBaseParam("endDate",
							convertDateObjToFormatStr(endDateOutLocal));
				}
				break;

			case Type_Contract_Integration_Performed :
				dateRange += " 合同收付款情况";
				if (contractType == Type_ContractType_Income) {
					Ext.getCmp(searchId).getStore().setBaseParam("begDate",
							convertDateObjToFormatStr(beginDateLocal));
					Ext.getCmp(searchId).getStore().setBaseParam("endDate",
							convertDateObjToFormatStr(endDateLocal));
					reconfigure = Type_Contract_Integration_Performed;
				} else {
					Ext.getCmp(searchId).getStore().setBaseParam("begDate",
							convertDateObjToFormatStr(beginDateOutLocal));
					Ext.getCmp(searchId).getStore().setBaseParam("endDate",
							convertDateObjToFormatStr(endDateOutLocal));
				}
				break;

			case Type_Contract_Integration_Invoice :
				dateRange += " 合同发票情况";
				if (contractType == Type_ContractType_Income) {
					Ext.getCmp(searchId).getStore().setBaseParam("begDate",
							convertDateObjToFormatStr(beginDateLocal));
					Ext.getCmp(searchId).getStore().setBaseParam("endDate",
							convertDateObjToFormatStr(endDateLocal));
					reconfigure = Type_Contract_Integration_Performed;
				} else {
					Ext.getCmp(searchId).getStore().setBaseParam("begDate",
							convertDateObjToFormatStr(beginDateOutLocal));
					Ext.getCmp(searchId).getStore().setBaseParam("endDate",
							convertDateObjToFormatStr(endDateOutLocal));
				}
				break;

			case Type_Contract_Integration_EstimateContractAmt :
				dateRange += " 合同预计来款情况";
				if (contractType == Type_ContractType_Income) {
					Ext.getCmp(searchId).getStore().setBaseParam("begDate",
							convertDateObjToFormatStr(beginDateLocal));
					Ext.getCmp(searchId).getStore().setBaseParam("endDate",
							convertDateObjToFormatStr(endDateLocal));
					reconfigure = Type_Contract_Integration_Performed;
				} else {
					Ext.getCmp(searchId).getStore().setBaseParam("begDate",
							convertDateObjToFormatStr(beginDateOutLocal));
					Ext.getCmp(searchId).getStore().setBaseParam("endDate",
							convertDateObjToFormatStr(endDateOutLocal));
				}
				break;

			case Type_Contract_Integration_Finance :
				dateRange += " 合同财务情况";
				if (contractType == Type_ContractType_Income) {
					Ext.getCmp(searchId).getStore().setBaseParam("begDate",
							convertDateObjToFormatStr(beginDateLocal));
					Ext.getCmp(searchId).getStore().setBaseParam("endDate",
							convertDateObjToFormatStr(endDateLocal));
					reconfigure = Type_Contract_Integration_Performed;
				} else {
					Ext.getCmp(searchId).getStore().setBaseParam("begDate",
							convertDateObjToFormatStr(beginDateOutLocal));
					Ext.getCmp(searchId).getStore().setBaseParam("endDate",
							convertDateObjToFormatStr(endDateOutLocal));
				}
				break;

			case Type_Contract_Integration_Warranty :
				dateRange += " 合同质保期到期";
				if (contractType == Type_ContractType_Income) {
					Ext.getCmp(searchId).getStore().setBaseParam("begDate",
							convertDateObjToFormatStr(beginDateLocal));
					Ext.getCmp(searchId).getStore().setBaseParam("endDate",
							convertDateObjToFormatStr(endDateLocal));
					reconfigure = Type_Contract_Integration_Warranty;
				} else {
					Ext.getCmp(searchId).getStore().setBaseParam("begDate",
							convertDateObjToFormatStr(beginDateOutLocal));
					Ext.getCmp(searchId).getStore().setBaseParam("endDate",
							convertDateObjToFormatStr(endDateOutLocal));
				}
				break;
		}
		Ext.getCmp(searchId).getStore().load({
			params : {
				start : 0,
				limit : (searchId == 'incomeGrid') ? pcPageSize : outPageSize
			},
			callback : function(record, options, success) {
				if (success == true) {
				} else {
					Ext.Msg.alert(Ext_errMsg_For_Get_Store_Title,
							Ext_errMsg_For_Get_Store_Message);
				}
			}
		});
		if (contractType == Type_ContractType_Income) {
			pcPagingToolbar.promptMsg = '<font color=\'red\'>' + dateRange
					+ ',</font>';
		} else {
			outPagingToolbar.promptMsg = '<font color=\'red\'>' + dateRange
					+ ',</font>';
		}
	}

	var warningTotalCountRecord = Ext.data.Record.create([{
				name : ContractTypeID
			}, {
				name : warningDays
			}, {
				name : warningCount
			}]);

	var warningTotalCountStore = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
			url : __cxtPath
					+ '/contractMain.do?method=warningTotalCountForSum&contractTypeID='
					+ Type_ContractType_Income
		}),
		baseParams : {
			searchName : '',
			warningType : '',
			searchType : Type_Contract_Integration_Simple,
			begDate : '',
			endDate : ''
		},
		reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount'
				}, warningTotalCountRecord)
	});

	warningTotalCountStore.on('load', function(store, records, options) {

		var without_def_count = "", earlier_30_days_count = "", earlier_15_days_count = "", earlier_7_days_count = "";
		var current_date_count = "", overtime_current_date_count = "", overtime_more_7_days_count = "";
		var overtime_more_15_days_count = "", overtime_more_30_days_count = "";
		for (var i = 0; i < records.length; i++) {
			var record = records[i];

			switch (record.get(warningDays)) {
				case Type_Warning_Without_Definition :
					without_def_count = "(" + record.get(warningCount) + ")";
					break;

				case Type_Warning_Earlier_30_Days :
					earlier_30_days_count = "(" + record.get(warningCount)
							+ ")";
					break;

				case Type_Warning_Earlier_15_Days :
					earlier_15_days_count = "(" + record.get(warningCount)
							+ ")";
					break;

				case Type_Warning_Earlier_7_Days :
					earlier_7_days_count = "(" + record.get(warningCount) + ")";
					break;

				case Type_Warning_Current_Date :
					current_date_count = "(" + record.get(warningCount) + ")";
					break;

				case Type_Warning_Overtime_Current_Date :
					overtime_current_date_count = "("
							+ record.get(warningCount) + ")";
					break;

				case Type_Warning_Overtime_More_7_Days :
					overtime_more_7_days_count = "(" + record.get(warningCount)
							+ ")";
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

	var legendMsg = new Ext.form.Label({});

	var qxMsg = new Ext.form.Label({});

	if (!decisionFlag) {
		qxMsg
				.setText('<font color=red>*权限不足，无权查看与金额相关的信息！&nbsp;</font>',
						false);
	}

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
				name : WarrantyEndDate
			}, {
				name : warningDate
			}, {
				name : warningAmt
			}, {
				name : warningBjamt
			}, {
				name : warningDays
			}, {
				name : contractInvoiceAmtInTimeScope
			}, {
				name : contractPayedAmtInTimeScope
			}, {
				name : contractEstimateAmtInTimeScope
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
			}, {
				name : officialReceiptsPayAmt
			}, {
				name : PaymentIndvanceAmtTotalSum
			}, {
				name : alreadyMakeOutInvoiceOfNotBaseOnPlan
			}, {
				name : didNotMakeOutInvoiceReceivableBaseOnPlan
			}, {
				name : 'execu_id',
				mapping : 'execu_id'
			}, {
				name : 'execu_name',
				mapping : 'execu_name'
			}, {
				name : '质保期起',
				mapping : '质保期起'
			}, {
				name : '质保期限',
				mapping : '质保期限'
			}, {
				name : '质保期计量单位',
				mapping : '质保期计量单位'
			}, {
				name : '质保期止',
				mapping : '质保期止'
			}, {
				name : '有效期起',
				mapping : '有效期起'
			}, {
				name : '有效期止',
				mapping : '有效期止'
			}, {
				name : '有效期限',
				mapping : '有效期限'
			}, {
				name : '有效期计量单位',
				mapping : '有效期计量单位'
			}, {
				name : 'summarystencil',
				mapping : 'summarystencil'
			}, {
				name : 'keywords',
				mapping : 'keywords'
			}, {
				name : 'manager_id',
				mapping : 'manager_id'// 项目经理
			}, {
				name : 'manager_name',
				mapping : 'manager_name'
			}, {
				name : elseIncomeAmtTotal
			}, {
				name : 'bcloseflag'
			}, {
				name : 'difMB'
			}]);

	var pcReader = new Ext.data.JsonReader({
				root : 'rows',
				totalProperty : 'totalCount'
			}, pcRecord);

	var pcStore = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
			url : String
					.format(
							'{0}/contractMain.do?method=getContractListByType&contractTypeID={1}',
							__cxtPath, Type_ContractType_Income)
		}),
		baseParams : {
			searchName : '',
			warningType : 0,
			limit : pcPageSize
		},
		remoteSort : true,
		sortInfo : {
			field : SignatureDate,
			direction : 'DESC'
		},
		reader : pcReader,
		listeners : {
			'beforeload' : function(s) {
				pcPageSize = s.lastOptions.params.limit;
				Ext.state.Manager.set('pcPage', pcPageSize);
			}
		}
	});

	// 收入综合
	var incomeStore = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : String
									.format(
											'{0}/contractMain.do?method=integrationSearch',
											__cxtPath)
						}),
				baseParams : {
					"contractTypeID" : 1
				},
				remoteSort : true,
				sortInfo : {
					field : SignatureDate,
					direction : 'ASC'
				},
				reader : pcReader,
				listeners : {
					'beforeload' : function(s) {
						pcPageSize = s.lastOptions.params.limit;
						Ext.state.Manager.set('pcPage', pcPageSize);
					}
				}
			}); // end store

	var outStore = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
			url : String
					.format(
							'{0}/contractMain.do?method=getContractListByType&contractTypeID={1}',
							__cxtPath, Type_ContractType_Payout)
		}),
		baseParams : {
			searchName : '',
			warningType : 0,
			limit : outPageSize
		},
		remoteSort : true,
		sortInfo : {
			field : SignatureDate,
			direction : 'DESC'
		},
		reader : pcReader,
		listeners : {
			'beforeload' : function(s) {
				outPageSize = s.lastOptions.params.limit;
				Ext.state.Manager.set('outPage', outPageSize);
			}
		}
	});

	// 支出综合
	var searchStore = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
					url : String
							.format(
									'{0}/contractMain.do?method=integrationSearchPayout',
									__cxtPath)
				}),
		baseParams : {
			"contractTypeID" : Type_ContractType_Payout
		},
		remoteSort : true,
		sortInfo : {
			field : SignatureDate,
			direction : 'ASC'
		},
		reader : pcReader,
		listeners : {
			'beforeload' : function(s) {
				outPageSize = s.lastOptions.params.limit;
				Ext.state.Manager.set('outPage', outPageSize);
			}
		}
	}); // end store

	// var incomePagelimitCombo = new Ext.ux.PagelimitCombo();
	var incomePagelimitCombo = new Ext.ux.PagelimitComboForRememberPageSize({
				rememberPage : Ext.state.Manager.get('pcPage')
						? Ext.state.Manager.get('pcPage')
						: 20
			});
	// 收入pagingToolbar
	var pcPagingToolbar = new Ext.ux.TotalPagingToolbar({
				pageSize : pcPageSize,
				store : pcStore,
				displayInfo : true,
				promptMsg : '',
				displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
				emptyMsg : "无显示数据",
				plugins : [incomePagelimitCombo],
				items : [new Ext.Button({
							text : this.text || '导出Excel',
							iconCls : 'xls',
							menu : {
								items : [{
									text : '导出本页数据',
									handler : function() {
										exp.onExport(incomeGrid, exportnameIn,
												true);
									}
								}, {
									text : '导出所有数据',
									handler : function() {
										exp.onExport(incomeGrid, exportnameIn,
												false);
									}
								}]
							}
						})]
			});

	Ext.ns('Ext.ux.grid');
	Ext.ux.grid.RowNumberer = Ext.extend(Object, {
		id : 'cnumberer',
		width : 40,
		sortable : false,
		hideable : false,
		menuDisabled : true,
		dataIndex : '',
		renderer : function(value, cellmeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < (parseInt(record.get(ContractTypeID)) == Type_ContractType_Income)
					? pcPageSize
					: outPageSize && store.getCount() == rowIndex + 1) {
				return '<font color=red style="font-size=11px">&nbsp;<b>合计</b>&nbsp;</font>';
			} else {
				if (rowIndex != (parseInt(record.get(ContractTypeID)) == Type_ContractType_Income)
						? pcPageSize
						: outPageSize) {
					return store.lastOptions.params.start + rowIndex + 1;
				} else {
					return '<font color=red style="font-size=11px">&nbsp;<b>合计</b>&nbsp;</font>';
				}
			}
		}
	});

	var allIncomeColumn = [new Ext.ux.grid.RowNumberer(), {
		header : "合同名称", // 1
		hidden : true,
		dataIndex : ContractName,
		sortable : true,
		renderer : function(value, cellmeta, record) {
			return linkRenderer(value, record.get(ContractID), Ext
							.isEmpty(record.get(Abbreviation)) ? record
							.get(ContractName) : record.get(Abbreviation));
		}
	}, {
		header : "项目编码", // 2
		dataIndex : ProjectCode,
		width : 100,
		sortable : true,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < pcPageSize
					&& store.getCount() == rowIndex + 1) {
				return value;
			} else {
				if (rowIndex == pcPageSize) {
					return value;
				}
			}
			if (Ext.isEmpty(value)) {
				value = '--';
			}
			return '<a href="javascript:;" onclick="parent.xMis.turn(\'project-info-'
					+ record.get(ProjectID)
					+ '\',\'项目信息-'
					+ record.get(ProjectCode)
					+ '\',\''
					+ __cxtPath
					+ '/projectMain.do?method=getProjectInfo&projectID='
					+ record.get(ProjectID)
					+ '\',\'menu-project-info\')">'
					+ value + '</a>';
		}
	}, {
		header : "项目名称", // 3
		dataIndex : ProjectName,
		sortable : true,
		hidden : true,
		width : 100,
		renderer : biuldLinker
	}, {
		header : "简称",// 4
		dataIndex : Abbreviation,
		sortable : true,
		width : 150,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if ((store.getCount() - 1 < pcPageSize && store.getCount() == rowIndex
					+ 1)
					|| rowIndex == pcPageSize) {
				return value;
			}
			value = (Ext.isEmpty(record.get(Abbreviation))) ? record
					.get(ContractName) : record.get(Abbreviation);
			return linkRenderer(value, record.get(ContractID), value);
		},
		exportFormater : function(value, record, index, pageinfo) {
			var abbreviation = record.get("简称");
			var contractName = record.get("合同名称");
			if (abbreviation == null || abbreviation == '') {
				if (contractName.indexOf("^") > 0) {
				contractName = contractName.substring(0, contractName.indexOf("^"));
				return contractName;
				}
			} else {
				if (abbreviation.indexOf("^") > 0) {
				abbreviation = abbreviation.substring(0, abbreviation.indexOf("^"));
				return abbreviation;
				}
			}
		}
	}, {
		header : "签订日期", // 5
		dataIndex : SignatureDate,
		sortable : true,
		width : 100,
		renderer : customer_formatDate,
		align : 'center'
	}, {
		header : "甲方公司", // 6
		dataIndex : BuyerAbb,
		sortable : true,
		hidden : true
	}, {
		header : "乙方公司", // 7
		dataIndex : VendorAbb,
		sortable : true,
		hidden : true
	}, {
		header : "<font color=" + Color_Font_Contract_Total_Amt
				+ ">合同金额</font>", // 8
		dataIndex : ContractAmt,
		sortable : true,
		width : 100,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			var tipname = record.get(Abbreviation);
			if (Ext.isEmpty(record.get(Abbreviation))) {
				tipname = record.get(ContractName);
			}
			if (Ext.isEmpty(value)) {
				return '';
			}

			value = value.substring(0, value.indexOf('^'));
			value = Ext.util.Format.number(new Number(value), '0,0.00');

			if (store.getCount() - 1 < pcPageSize
					&& store.getCount() == rowIndex + 1) {
				return value;
			} else {
				if (rowIndex == pcPageSize) {
					return value;
				}
			}

			var index = value.indexOf("^");
			if (index > 0) {
				var text = value.substring(0, index);
				var url = value.substring(index + 1, value.length);

				return "<a href='javascript:;' onclick=\"javascript:turnToEditContract('"
						+ record.get(ContractID)
						+ "','"
						+ Ext.util.Format.htmlEncode(tipname.substr(0, tipname
										.indexOf('^') > 0 ? tipname
										.indexOf('^') : tipname.length))
						+ "');\">" + text + "</a>";
			}

			return "<a href='javascript:;' onclick=\"javascript:turnToEditContract('"
					+ record.get(ContractID)
					+ "','"
					+ Ext.util.Format.htmlEncode(tipname.substr(0, tipname
									.indexOf('^') > 0
									? tipname.indexOf('^')
									: tipname.length))
					+ "');\">"
					+ value
					+ "</a>";
		},
		align : 'right'
	}, {
		header : "<font color=" + Color_Font_Contract_Total_Charged_Amt
				+ ">已收合同款登记</font>", // 9
		dataIndex : TotalAmtByID,
		sortable : true,
		hidden : deptLimitsFlag == true ? false : true,
		width : 100,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < pcPageSize
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				return value;
			} else {
				if (rowIndex == pcPageSize) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					return value;
				}
			}
			// if (parseInt(value) < parseInt(record.get(incomAmtTotalByGl)) -
			// 100) {
			// cellMeta.attr = 'ext:qtitle="提示" ext:qtip="' + '有财务到帐未登记的金额'
			// + '"';
			// var v = value;
			// if (v == 0) {
			// v = record.get(incomAmtTotalByGl);
			// }
			// return formatCurrency(record.data[CurrenyGraph], v
			// + '^onClick="showPaymentList(\''
			// // +
			// + convertNameWithoutLinker(Ext
			// .isEmpty(record.data[Abbreviation])
			// ? record.data[ContractName]
			// : record.data[Abbreviation]) + '\','
			// + record.data[ContractID] + ')"'
			// + ' id=\'paymentListLink_'
			// + record.data[ContractID] + '\'', 'red');
			//
			// } else if (parseInt(value) - 100 > parseInt(record
			// .get(incomAmtTotalByGl))) {
			// cellMeta.attr = 'ext:qtitle="提示" ext:qtip="' + '登记的金额超过财务来款'
			// + '"';
			// return formatCurrency(record.data[CurrenyGraph], value
			// + '^onClick="showPaymentList(\''
			// // +
			// + convertNameWithoutLinker(Ext
			// .isEmpty(record.data[Abbreviation])
			// ? record.data[ContractName]
			// : record.data[Abbreviation]) + '\','
			// + record.data[ContractID] + ')"'
			// + ' id=\'paymentListLink_'
			// + record.data[ContractID] + '\'', 'green');
			// } else {
			return formatCurrency(record.data[CurrenyGraph], value
							+ '^onClick="showPaymentList(\''
							+ convertNameWithoutLinker(Ext
									.isEmpty(record.data[Abbreviation])
									? record.data[ContractName]
									: record.data[Abbreviation]) + '\','
							+ record.data[ContractID] + ')"'
							+ '   id=\'paymentListLink_'
							+ record.data[ContractID] + '\'',
					Color_Font_Contract_Total_Charged_Amt);
			// }
		},
		align : 'right'
	}, {
		header : "时段实收合同金额",
		dataIndex : contractPayedAmtInTimeScope,
		sortable : true, // 10
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			return formatCurrency(record.data[CurrenyGraph], value, 'black');

		},
		align : 'right'
	}, {
		header : "<font color=" + Color_Font_Contract_Total_Invoiced_Amt
				+ ">已开收据金额</font>", // 11
		dataIndex : receiptTotalAmt,
		sortable : true,
		hidden : deptLimitsFlag == true ? false : true,
		width : 100,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < pcPageSize
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				return value;
			} else {
				if (rowIndex == pcPageSize) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					return value;
				}
			}
			return formatCurrency(record.data[CurrenyGraph], value,
					Color_Font_Contract_Total_Invoiced_Amt);
		},
		align : 'right'
	}, {
		header : "<font color=" + Color_Font_Contract_Total_Invoiced_Amt
				+ ">已开发票金额</font>", // 12
		dataIndex : invoiceTotalAmt,
		hidden : deptLimitsFlag == true ? false : true,
		sortable : true,
		width : 100,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < pcPageSize
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				return value;
			} else {
				if (rowIndex == pcPageSize) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					return value;
				}
			}
			return formatCurrency(record.data[CurrenyGraph], value
							+ '^onClick="showInvoiceList(\''
							+ convertNameWithoutLinker(Ext
									.isEmpty(record.data[Abbreviation])
									? record.data[ContractName]
									: record.data[Abbreviation]) + '\','
							+ record.data[ContractID] + ')"'
							+ '   id=\'invoiceListLink_'
							+ record.data[ContractID] + '\'',
					Color_Font_Contract_Total_Invoiced_Amt);
		},
		align : 'right'
	}, {
		header : "时段实开发票金额",
		dataIndex : contractInvoiceAmtInTimeScope,
		sortable : true, // 13
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < pcPageSize
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				if (parseInt(record.get(ContractTypeID)) == 2 && !decisionFlag) {
					return '--';
				} else {
					return value;
				}
			} else {
				if (rowIndex == pcPageSize) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					if (parseInt(record.get(ContractTypeID)) == 2
							&& !decisionFlag) {
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
				+ ">已开票未到账金额</font>", // 14
		dataIndex : invoiceUnAccountAmt,
		hidden : deptLimitsFlag == true ? false : true,
		sortable : true,
		width : 100,
		tooltip : tip1,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < pcPageSize
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				return value;
			} else {
				if (rowIndex == pcPageSize) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					return value;
				}
			}
			return formatCurrency(record.data[CurrenyGraph], value,
					Color_Font_Contract_Total_UnAccount_Amt);
		},
		align : 'right'
	}, {
		header : "按进度应收账款", // 15
		dataIndex : warningAmt,
		sortable : true,
		hidden : deptLimitsFlag == true ? false : true,
		width : 100,
		tooltip : tip2,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < pcPageSize
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				return value;
			} else {
				if (rowIndex == pcPageSize) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					return value;
				}
			}
			return formatCurrency(record.data[CurrenyGraph], value, "");
		},
		align : 'right'
	}, {
		header : "按进度已开票未收款", // 16
		dataIndex : alreadyMakeOutInvoiceOfNotBaseOnPlan,
		sortable : true,
		hidden : deptLimitsFlag == true ? false : true,
		width : 100,
		tooltip : tip3,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < pcPageSize
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				return value;
			} else {
				if (rowIndex == pcPageSize) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					return value;
				}
			}
			return formatCurrency(record.data[CurrenyGraph], value, "");
		},
		align : 'right'
	}, {
		header : "按进度未开票应收款", // 17
		dataIndex : didNotMakeOutInvoiceReceivableBaseOnPlan,
		sortable : true,
		hidden : deptLimitsFlag == true ? false : true,
		width : 100,
		tooltip : tip4,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < pcPageSize
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				return value;
			} else {
				if (rowIndex == pcPageSize) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					return value;
				}
			}
			return formatCurrency(record.data[CurrenyGraph], value, "");
		},
		align : 'right'
	}, {
		header : "预收合同款", // 18
		dataIndex : PaymentIndvanceAmtTotalSum,
		hidden : deptLimitsFlag == true ? false : true,
		sortable : true,
		tooltip : tip5,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < pcPageSize
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				return value;
			} else {
				if (rowIndex == pcPageSize) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					return value;
				}
			}
			return formatCurrency(record.data[CurrenyGraph], value,
					Color_Font_Contract_Total_UnAccount_Amt);
		},
		align : 'right'
	}, {
		header : "<font color=" + Color_Font_Contract_Total_UnCharged_Amt
				+ ">未收合同款登记</font>", // 19
		dataIndex : UnChargedAmtTotal,
		sortable : true,
		width : 100,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < pcPageSize
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				return value;
			} else {
				if (rowIndex == pcPageSize) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					return value;
				}
			}
			cellMeta.attr = setWarningBackgroudColor(record.data[warningDate],
					record.data[warningBjamt], record.data[warningDays],
					record.data[CurrenyGraph], record.data[delayReason]);
			return formatCurrency(
					record.data[CurrenyGraph],
					value
							+ '^onClick="showPaymentOption(\''
							+ convertNameWithoutLinker(Ext
									.isEmpty(record.data[Abbreviation])
									? record.data[ContractName]
									: record.data[Abbreviation])
							+ '\','
							+ record.data[ContractID]
							+ ','
							+ record.data[ContractTypeID]
							+ ','
							+ convertNameWithoutLinker(record.data[ContractAmt])
							+ ',undefined,\'' + record.data[ProjectID] + '\')"'
							+ '   id=\'showPaymentOption_'
							+ record.data[ContractID] + '\'',
					Color_Font_Contract_Total_UnCharged_Amt);
		},
		align : 'right'
	}, {
		header : "预计来款",
		dataIndex : contractEstimateAmtInTimeScope,
		sortable : true, // 20
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < pcPageSize
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				if (parseInt(record.get(ContractTypeID)) == 2 && !decisionFlag) {
					return '--';
				} else {
					return value;
				}

			} else {
				if (rowIndex == pcPageSize) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');

					if (parseInt(record.get(ContractTypeID)) == 2
							&& !decisionFlag) {
						return '--';
					} else {
						return value;
					}
				}
			}
			if (parseInt(record.get(ContractTypeID)) == 2 && !decisionFlag) {
				return formatCurrency(record.data[CurrenyGraph], '0', 'black');
			} else {
				return formatCurrency(record.data[CurrenyGraph], value, 'black');
			}

		},
		align : 'right'
	}, {
		header : "甲方代表", // 21
		dataIndex : BuyerRepName,
		sortable : true,
		renderer : biuldLinker,
		hidden : true
	}, {
		header : "乙方代表", // 22
		dataIndex : VendorRepName,
		sortable : true,
		renderer : biuldLinker,
		hidden : true
	}, {
		header : "合同概要", // 23
		dataIndex : ContractSummary,
		sortable : true,
		hidden : true,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			cellMeta.css = 'nowrap';
			var result = com.bitc.xmis.util.SummaryFormatUtil(record.data.合同概要,
					record.data.summarystencil);
			cellMeta.attr = 'ext:qtitle="合同概要" ext:qtip="' + result + '"';
			return result;
		}
	}, {
		header : "备注", // 24
		dataIndex : Remark,
		sortable : true,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			cellMeta.css = 'nowrap';
			if (value != null) {
				var baseNum = 18;
				var msg = '';
				for (var i = 0; i < value.length / baseNum; i++) {
					msg += value.substr(i * baseNum, baseNum) + "<br/>";
				}
				cellMeta.attr = 'ext:qtitle="备注" ext:qtip="' + msg + '"';
			}
			return value;
		},
		hidden : true
	}, {
		header : "登记人", // 25
		dataIndex : RepName,
		sortable : true,
		renderer : biuldLinker,
		hidden : true
	}, {
		header : "更新日期", // 26
		dataIndex : UpdateDate,
		sortable : true,
		renderer : customer_formatDate,
		align : 'center',
		hidden : true
	}, {
		header : "质保到期", // 27
		dataIndex : WarrantyEndDate,
		sortable : true,
		renderer : customer_formatDate,
		align : 'center'
	}, {
		header : "纵向来源名称", // 28
		dataIndex : GovermentContractSourceName,
		sortable : true,
		renderer : biuldLinker,
		hidden : true
	}, {
		header : "财务来款合计", // 29
		dataIndex : incomAmtTotalByGl,
		sortable : true,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < pcPageSize
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				return value;
			} else {
				if (rowIndex == pcPageSize) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					return value;
				}
			}
			return formatCurrency(record.data[CurrenyGraph], value
							+ '^onClick="showGLIncomeList(\''
							// +
							+ convertNameWithoutLinker(Ext
									.isEmpty(record.data[Abbreviation])
									? record.data[ContractName]
									: record.data[Abbreviation]) + '\',\''
							+ record.data[ProjectID] + '\')"'
							+ '   id=\'glIncomeListLink_'
							+ record.data[ProjectID] + '\'',
					Color_Font_Contract_Total_Charged_Amt);
		},
		align : 'right'
	}, {
		header : "时段财务实收金额", // 30
		dataIndex : officialReceiptsMB,
		sortable : true,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < pcPageSize
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				if (parseInt(record.get(ContractTypeID)) == 2 && !decisionFlag) {
					return '--';
				} else {
					return value;
				}
			} else {
				if (rowIndex == pcPageSize) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					if (parseInt(record.get(ContractTypeID)) == 2
							&& !decisionFlag) {
						return '--';
					} else {
						return value;
					}
				}
			}
			if (parseInt(record.get(ContractTypeID)) == 2 && !decisionFlag) {
				return formatCurrency(record.data[CurrenyGraph], '0',
						Color_Font_Contract_Total_Charged_Amt);
			} else {
				return formatCurrency(record.data[CurrenyGraph], value,
						Color_Font_Contract_Total_Charged_Amt);
			}

		},
		align : 'right'
	}, {
		header : "财务采购支出合计", // 31
		dataIndex : purAmtTotalByGl,
		sortable : true,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < pcPageSize
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				if (!decisionFlag) {
					return '--';
				} else {
					return value;
				}
			} else {
				if (rowIndex == pcPageSize) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
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
				return formatCurrency(record.data[CurrenyGraph], value
								+ '^onClick="showGLPurchaseList(\''
								+ convertNameWithoutLinker(Ext
										.isEmpty(record.data[Abbreviation])
										? record.data[ContractName]
										: record.data[Abbreviation]) + '\',\''
								+ record.data[ProjectID] + '\',\'2\')"'
								+ '   id=\'glPurchaseListLink_'
								+ record.data[ProjectID] + '\'',
						Color_Font_Contract_Total_Charged_Amt);
			}

		},
		align : 'right'
	}, {
		header : "时段支出实际金额", // 32
		dataIndex : officialReceiptsPayAmt,
		sortable : true,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < pcPageSize
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				if (!decisionFlag) {
					return '--';
				} else {
					return value;
				}
			} else {
				if (rowIndex == pcPageSize) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
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
		header : "合同欠款", // 33
		dataIndex : unIncomAmtTotalByGl,
		sortable : true,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < pcPageSize
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				return value;
			} else {
				if (rowIndex == pcPageSize) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					return value;
				}
			}

			cellMeta.attr = setWarningBackgroudColor(record.data[warningDate],
					record.data[warningBjamt], record.data[warningDays],
					record.data[CurrenyGraph], record.data[delayReason]);
			return formatCurrency(
					record.data[CurrenyGraph],
					value
							+ '^onClick="showPaymentOption(\''
							+ convertNameWithoutLinker(Ext
									.isEmpty(record.data[Abbreviation])
									? record.data[ContractName]
									: record.data[Abbreviation])
							+ '\','
							+ record.data[ContractID]
							+ ','
							+ record.data[ContractTypeID]
							+ ','
							+ convertNameWithoutLinker(record.data[ContractAmt])
							+ ',undefined,' + record.data[incomAmtTotalByGl]
							+ ',\'' + record.data[ProjectID] + '\')"'
							+ '   id=\'showPaymentOption_'
							+ record.data[ContractID] + '\'',
					Color_Font_Contract_Total_UnCharged_Amt);
		},
		align : 'right'
	}, {
		header : '乙方部门', // 34
		dataIndex : DeptNameOfVendor,
		sortable : true,
		hidden : true
	}, {
		header : '执行部门', // 35
		dataIndex : 'execu_name',
		hidden : true,
		sortable : true
	}, {
		header : '项目经理',// 36
		dataIndex : 'manager_name',
		hidden : true,
		sortable : true
	}, {
		header : '质保期起',// 37
		dataIndex : '质保期起',
		hidden : true,
		sortable : true,
		renderer : customer_formatDate,
		align : 'center'
	}, {
		header : '质保期限',// 38
		dataIndex : '质保期限',
		hidden : true,
		sortable : true,
		align : 'center',
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			unit = record.get('质保期计量单位');
			if (unit && value) {
				if (parseInt(value) == 0) {
					return '';
				}
				if (unit == 1) {
					unit = '年';
				} else if (unit == 2) {
					unit = '月';
				} else if (unit == 3) {
					unit = '日';
				}
				return parseInt(value) + ' ' + unit;
			}
		},
		exportFormater : function(value, record, index, pageinfo) {
			unit = record.get('质保期计量单位');
			if (unit && value) {
				if (parseInt(value) == 0) {
					return '';
				}
				if (unit == 1) {
					unit = '年';
				} else if (unit == 2) {
					unit = '月';
				} else if (unit == 3) {
					unit = '日';
				}
				return parseInt(value) + ' ' + unit;
			} else {
				return '';
			}
		}
	}, {
		header : '质保期止',// 39
		dataIndex : '质保期止',
		hidden : true,
		sortable : true,
		renderer : customer_formatDate,
		align : 'center'
	}, {
		header : '有效期起',// 40
		dataIndex : '有效期起',
		hidden : true,
		sortable : true,
		renderer : customer_formatDate,
		align : 'center'
	}, {
		header : '有效期限',// 41
		dataIndex : '有效期限',
		hidden : true,
		sortable : true,
		align : 'center',
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			unit = record.get('有效期计量单位');
			if (unit && value) {
				if (parseInt(value) == 0) {
					return '';
				}
				if (unit == 1) {
					unit = '年';
				} else if (unit == 2) {
					unit = '月';
				} else if (unit == 3) {
					unit = '日';
				}
				return parseInt(value) + ' ' + unit;
			}
		},
		exportFormater : function(value, record, index, pageinfo) {
			unit = record.get('有效期计量单位');
			if (unit && value) {
				if (parseInt(value) == 0) {
					return '';
				}
				if (unit == 1) {
					unit = '年';
				} else if (unit == 2) {
					unit = '月';
				} else if (unit == 3) {
					unit = '日';
				}
				return parseInt(value) + ' ' + unit;
			} else {
				return '';
			}
		}
	}, {
		header : '有效期止',// 42
		dataIndex : '有效期止',
		hidden : true,
		sortable : true,
		renderer : customer_formatDate,
		align : 'center'
	}, {
		// header : "其它财务来款", // 43
		header : "退税收入合计", // 43
		dataIndex : elseIncomeAmtTotal,
		sortable : true,
		align : 'right',
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < pcPageSize
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				return value;
			} else {
				if (rowIndex == pcPageSize) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					return value;
				}
			}
			return formatCurrency(record.data[CurrenyGraph], new Number(value)
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
		header : '项目是否结束',// 44
		dataIndex : 'bcloseflag',
		sortable : true,
		align : 'center',
		renderer : function(value, cellmeta, record) {
			if (value == 1) {
				return '是';
			} else {
				return '';
			}
		},
		exportFormater : function(value, row, index, pageinfo) {
			if (value == 1) {
				return '是';
			} else {
				return '';
			}
		}
	}, {
		header : "<font color=" + Color_Font_Contract_Total_Charged_Amt
				+ ">财务对账差异</font>",// 45
		dataIndex : 'difMB',
		sortable : true,
		align : 'right',
		width : 110,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < pcPageSize
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				return value;
			} else {
				if (rowIndex == pcPageSize) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					return value;
				}
			}
			// 差额= MB（财务来款）-收款合计
			if (value > 0) {
				cellMeta.attr = 'ext:qtitle="提示" ext:qtip="' + '有财务到帐未登记的金额'
						+ '"';
				return formatCurrency(
						record.data[CurrenyGraph],
						value
								+ '^onClick="showPaymentOption(\''
								+ convertNameWithoutLinker(Ext
										.isEmpty(record.data[Abbreviation])
										? record.data[ContractName]
										: record.data[Abbreviation])
								+ '\','
								+ record.data[ContractID]
								+ ','
								+ record.data[ContractTypeID]
								+ ','
								+ convertNameWithoutLinker(record.data[ContractAmt])
								+ ',undefined,'
								+ record.data[incomAmtTotalByGl] + ',\''
								+ record.data[ProjectID] + '\')"'
								+ '   id=\'showPaymentOption_'
								+ record.data[ContractID] + '\'', 'red');
			} else if (value < 0) {
				value = Math.abs(value);
				cellMeta.attr = 'ext:qtitle="提示" ext:qtip="' + '登记的金额超过财务来款'
						+ '"';
				return formatCurrency(
						record.data[CurrenyGraph],
						value
								+ '^onClick="showPaymentOption(\''
								+ convertNameWithoutLinker(Ext
										.isEmpty(record.data[Abbreviation])
										? record.data[ContractName]
										: record.data[Abbreviation])
								+ '\','
								+ record.data[ContractID]
								+ ','
								+ record.data[ContractTypeID]
								+ ','
								+ convertNameWithoutLinker(record.data[ContractAmt])
								+ ',undefined,'
								+ record.data[incomAmtTotalByGl] + ',\''
								+ record.data[ProjectID] + '\')"'
								+ '   id=\'showPaymentOption_'
								+ record.data[ContractID] + '\'', 'green');
			} else {
				return formatCurrency(
						record.data[CurrenyGraph],
						value
								+ '^onClick="showPaymentOption(\''
								+ convertNameWithoutLinker(Ext
										.isEmpty(record.data[Abbreviation])
										? record.data[ContractName]
										: record.data[Abbreviation])
								+ '\','
								+ record.data[ContractID]
								+ ','
								+ record.data[ContractTypeID]
								+ ','
								+ convertNameWithoutLinker(record.data[ContractAmt])
								+ ',undefined,'
								+ record.data[incomAmtTotalByGl] + ',\''
								+ record.data[ProjectID] + '\')"'
								+ '   id=\'showPaymentOption_'
								+ record.data[ContractID] + '\'',
						Color_Font_Contract_Total_Charged_Amt);
			}
		}
	}, {
		header : '甲方合同编号',// 46
		dataIndex : ContractNo,
		sortable : true,
		renderer : function(value, cellmeta, record) {
			return linkRenderer(value, record.get(ContractID), Ext
							.isEmpty(record.get(Abbreviation)) ? record
							.get(ContractName) : record.get(Abbreviation));
		}
	}

	];

	// 生成默认有业务权限的列模型
	var pcColM = new Ext.grid.ColumnModel({
				columns : getColumns(allIncomeColumn,
						incomeBusinessWithBusinessAuthorityColumn)
			});
	// 生成默认无业务权限的列模型
	if (!deptLimitsFlag) {
		pcColM = new Ext.grid.ColumnModel({
					columns : getColumns(allIncomeColumn,
							incomeBusinessWithoutBusinessAuthorityColumn)
				});
	}

	allOutColumn = [new Ext.ux.grid.RowNumberer(), {
		header : "合同编号", // 1
		dataIndex : ContractNo,
		sortable : true,
		width : 125,
		renderer : function(value, cellmeta, record) {
			return linkRenderer(value, record.get(ParentID) == "" ? record
							.get(ContractID) : record.get(ParentID), Ext
							.isEmpty(record.get(Abbreviation)) ? record
							.get(ContractName) : record.get(Abbreviation),
					record.get(ContractTypeID));
		}
	}, {
		header : "合同名称", // 2
		hidden : true,
		dataIndex : ContractName,
		sortable : true,
		renderer : function(value, cellmeta, record) {
			return linkRenderer(value, record.get(ParentID) == "" ? record
							.get(ContractID) : record.get(ParentID), Ext
							.isEmpty(record.get(Abbreviation)) ? record
							.get(ContractName) : record.get(Abbreviation),
					record.get(ContractTypeID));
		}
	}, {
		id : 'abb', // 3
		header : "简称",
		dataIndex : 'abbreviation',
		sortable : true,
		width : 150,
		renderer : function(value, cellmeta, record, rowIndex) {
			return linkRenderer(value, record.get(ParentID) == "" ? record
							.get(ContractID) : record.get(ParentID), Ext
							.isEmpty(record.get(Abbreviation)) ? record
							.get(ContractName) : record.get(Abbreviation),
					record.get(ContractTypeID));
		}
	}, {
		header : "签订日期", // 4
		dataIndex : SignatureDate,
		sortable : true,
		width : 100,
		renderer : customer_formatDate,
		align : 'center'
	}, {
		header : "乙方公司", // 5
		dataIndex : VendorAbb,
		sortable : true,
		hidden : true
	}, {
		header : "<font color=" + Color_Font_Contract_Total_Amt
				+ ">合同金额</font>", // 6
		dataIndex : ContractAmt,
		sortable : true,
		width : 100,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			value = value.substring(0, value.indexOf('^'));
			value = Ext.util.Format.number(new Number(value), '0,0.00');
			if (store.getCount() - 1 < store.baseParams.limit
					&& store.getCount() == rowIndex + 1) {
				if (!decisionFlag) {
					return '--';
				} else {
					return value;
				}
			} else {
				if (rowIndex == store.baseParams.limit) {
					if (!decisionFlag) {
						return '--';
					} else {
						return value;
					}
				}
			}
			if (!decisionFlag) {
				formatCurrency('', '0', '');
			} else {
				return linkRenderer(value, record.get(ParentID) == "" ? record
								.get(ContractID) : record.get(ParentID), Ext
								.isEmpty(record.get(Abbreviation)) ? record
								.get(ContractName) : record.get(Abbreviation),
						record.get(ContractTypeID));
			}

		},
		align : 'right'
	}, {
		header : "<font color=" + Color_Font_Contract_Total_Charged_Amt
				+ ">已付合同款登记</font>", // 7
		dataIndex : TotalAmtByID,
		sortable : true,
		width : 100,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < store.baseParams.limit
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				if (!decisionFlag) {
					return '--';
				} else {
					return value;
				}
			} else {
				if (rowIndex == store.baseParams.limit) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
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
				return formatCurrency(record.data[CurrenyGraph], value
								+ '^onClick="showPaymentList(\''
								+ convertNameWithoutLinker(Ext
										.isEmpty(record.data[Abbreviation])
										? record.data[ContractName]
										: record.data[Abbreviation]) + '\','
								+ record.data[ContractID] + ')"'
								+ '   id=\'paymentListLink_'
								+ record.data[ContractID] + '\'',
						Color_Font_Contract_Total_Charged_Amt);
			}

		},
		align : 'right'
	}, {
		header : "时段实付合同金额",
		dataIndex : contractPayedAmtInTimeScope,
		sortable : true, // 8
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			return formatCurrency(record.data[CurrenyGraph], value, 'black');

		},
		align : 'right'
	}, {
		header : "<font color=" + Color_Font_Contract_Total_Invoiced_Amt
				+ ">已开收据金额</font>", // 9
		dataIndex : receiptTotalAmt,
		sortable : true,
		width : 100,
		hidden : deptLimitsFlag == true ? false : true,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < store.baseParams.limit
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				if (!decisionFlag) {
					return '--';
				} else {
					return value;
				}
			} else {
				if (rowIndex == store.baseParams.limit) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					if (!decisionFlag) {
						return '--';
					} else {
						return value;
					}
				}
			}
			if (!decisionFlag) {
				return formatCurrency(record.data[CurrenyGraph], '0',
						Color_Font_Contract_Total_Invoiced_Amt);
			} else {
				return formatCurrency(record.data[CurrenyGraph], value,
						Color_Font_Contract_Total_Invoiced_Amt);
			}
		},
		align : 'right'
	}, {
		header : "<font color=" + Color_Font_Contract_Total_Invoiced_Amt
				+ ">已报销金额</font>", // 10
		dataIndex : invoiceTotalAmt,
		sortable : true,
		hidden : deptLimitsFlag == true ? false : true,
		width : 100,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < store.baseParams.limit
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				if (!decisionFlag) {
					return '--';
				} else {
					return value;
				}
			} else {
				if (rowIndex == store.baseParams.limit) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					if (!decisionFlag) {
						return '--';
					} else {
						return value;
					}
				}
			}
			if (!decisionFlag) {
				return formatCurrency(record.data[CurrenyGraph], '0',
						Color_Font_Contract_Total_Invoiced_Amt);
			} else {
				return formatCurrency(record.data[CurrenyGraph], value
								+ '^onClick="showInvoiceList(\''
								+ convertNameWithoutLinker(Ext
										.isEmpty(record.data[Abbreviation])
										? record.data[ContractName]
										: record.data[Abbreviation]) + '\','
								+ record.data[ContractID] + ')"'
								+ '   id=\'invoiceListLink_'
								+ record.data[ContractID] + '\'',
						Color_Font_Contract_Total_Invoiced_Amt);
			}
		},
		align : 'right'
	}, {
		header : "时段实收发票金额",
		dataIndex : contractInvoiceAmtInTimeScope,
		sortable : true, // 11
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < outPageSize
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				if (parseInt(record.get(ContractTypeID)) == 2 && !decisionFlag) {
					return '--';
				} else {
					return value;
				}
			} else {
				if (rowIndex == outPageSize) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					if (parseInt(record.get(ContractTypeID)) == 2
							&& !decisionFlag) {
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
				+ ">已付款未报销金额</font>", // 12
		dataIndex : invoiceUnAccountAmt,
		sortable : true,
		hidden : deptLimitsFlag == true ? false : true,
		width : 100,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			if (store.getCount() - 1 < store.baseParams.limit
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				if (!decisionFlag) {
					return '--';
				} else {
					return value;
				}
			} else {
				if (rowIndex == store.baseParams.limit) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					if (!decisionFlag) {
						return '--';
					} else {
						return value;
					}
				}
			}
			if (!decisionFlag) {
				return formatCurrency(record.data[CurrenyGraph], '0',
						Color_Font_Contract_Total_UnAccount_Amt);
			} else {
				return formatCurrency(record.data[CurrenyGraph], String(Math
								.floor(-1 * value)),
						Color_Font_Contract_Total_UnAccount_Amt);
			}

		},
		align : 'right'
	}, {
		header : "<font color=" + Color_Font_Contract_Total_UnCharged_Amt
				+ ">未付合同款登记</font>", // 13
		dataIndex : UnChargedAmtTotal,
		sortable : true,
		width : 100,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			cellMeta.attr = setWarningBackgroudColor(record.data[warningDate],
					record.data[warningBjamt], record.data[warningDays],
					record.data[CurrenyGraph]);
			if (store.getCount() - 1 < store.baseParams.limit
					&& store.getCount() == rowIndex + 1) {
				value = Ext.util.Format.number(new Number(value), '0,0.00');
				if (!decisionFlag) {
					return '--';
				} else {
					return value;
				}
			} else {
				if (rowIndex == store.baseParams.limit) {
					value = Ext.util.Format.number(new Number(value), '0,0.00');
					if (!decisionFlag) {
						return '--';
					} else {
						return value;
					}
				}
			}
			if (!decisionFlag) {
				return formatCurrency(record.data[CurrenyGraph], '0',
						Color_Font_Contract_Total_UnCharged_Amt);
			} else {
				return formatCurrency(
						record.data[CurrenyGraph],
						value
								+ '^onClick="showPaymentOption(\''
								+ convertNameWithoutLinker(Ext
										.isEmpty(record.data[Abbreviation])
										? record.data[ContractName]
										: record.data[Abbreviation])
								+ '\','
								+ record.data[ContractID]
								+ ','
								+ record.data[ContractTypeID]
								+ ','
								+ convertNameWithoutLinker(record.data[ContractAmt])
								+ ')"' + '   id=\'showPaymentOption_'
								+ record.data[ContractID] + '\'',
						Color_Font_Contract_Total_UnCharged_Amt);
			}
		},
		align : 'right'
	}, {
		header : "甲方代表", // 14
		dataIndex : BuyerRepName,
		sortable : true,
		width : 100,
		renderer : biuldLinker,
		hidden : true
	}, {
		header : "乙方代表", // 15
		dataIndex : VendorRepName,
		sortable : true,
		width : 100,
		renderer : biuldLinker,
		hidden : true
	}, {
		header : "合同概要", // 16
		dataIndex : ContractSummary,
		sortable : true,
		hidden : true,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			cellMeta.css = 'nowrap';
			var result = com.bitc.xmis.util.SummaryFormatUtil(record.data.合同概要,
					record.data.summarystencil);
			cellMeta.attr = 'ext:qtitle="合同概要" ext:qtip="' + result + '"';
			return result;
		}
	}, {
		header : "备注", // 17
		dataIndex : Remark,
		sortable : true,
		hidden : true,
		renderer : function(value, cellMeta, record, rowIndex, columnIndex,
				store) {
			cellMeta.css = 'nowrap';
			if (value != null) {
				var baseNum = 18;
				var msg = '';
				for (var i = 0; i < value.length / baseNum; i++) {
					msg += value.substr(i * baseNum, baseNum) + "<br/>";
				}
				cellMeta.attr = 'ext:qtitle="备注" ext:qtip="' + msg + '"';
			}
			return value;
		}
	}, {
		header : "登记人", // 18
		dataIndex : RepName,
		sortable : true,
		renderer : biuldLinker,
		hidden : true
	}, {
		header : "更新日期", // 19
		dataIndex : UpdateDate,
		sortable : true,
		renderer : customer_formatDate,
		align : 'center',
		hidden : true
	}, {
		header : "质保到期", // 20
		dataIndex : WarrantyEndDate,
		sortable : true,
		renderer : customer_formatDate,
		align : 'center'
	}, {
		header : "甲方公司", // 21
		dataIndex : BuyerAbb,
		sortable : true,
		hidden : true
	}];

	var outColM = new Ext.grid.ColumnModel({
				columns : getColumns(allOutColumn,
						outBusinessWithBusinessAuthorityColumn)
			});
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
							columnHeader : '合同编号',
							formatClass : 'com.bitc.jk.xmis.export.SubstringUrl'
						}, {
							columnHeader : '简称',
							formatClass : 'com.bitc.jk.xmis.export.SubstringUrl'
						}, {
							columnHeader : '更新日期',
							formatClass : 'com.bitc.jk.xmis.export.SubstringTimer'
						}, {
							columnHeader : '签订日期',
							formatClass : 'com.bitc.jk.xmis.export.SubstringTimer'
						}, {
							columnHeader : '质保期起',
							formatClass : 'com.bitc.jk.xmis.export.SubstringTimer'
						}, {
							columnHeader : '质保期止',
							formatClass : 'com.bitc.jk.xmis.export.SubstringTimer'
						}, {
							columnHeader : '有效期起',
							formatClass : 'com.bitc.jk.xmis.export.SubstringTimer'
						}, {
							columnHeader : '有效期止',
							formatClass : 'com.bitc.jk.xmis.export.SubstringTimer'
						}, {
							// columnHeader : '其它财务来款',
							columnHeader : '退税收入',
							formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
						}]
			});

	var incomeGrid = new Ext.grid.GridPanel({
		id : 'incomeGrid',
		cm : pcColM,
		store : pcStore,
		stripeRows : true,
		region : 'center',
		stateful : true,
		stateId : 'contract-list-income-grid',
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		viewConfig : {
			getRowClass : function(record, rowIndex, rowParams, store) {
				if (store.getCount() - 1 < pcPageSize
						&& store.getCount() == rowIndex + 1) {
					return 'grid-contract-total-list-yellow';
				} else {
					if (rowIndex == pcPageSize) {
						return 'grid-contract-total-list-yellow';
					}
				}
			}
		},
		tbar : [{
			id : 'incomeSourceTypeID',
			xtype : 'combo',
			width : 100,
			displayField : 'sourcename',
			valueField : 'sourceid',
			mode : 'remote',
			triggerAction : 'all',
			editable : false,
			store : new Ext.data.JsonStore({
						url : __cxtPath
								+ '/contractMain.do?method=getContractSourceTypeID',
						baseParams : {
							level : 0
						},
						autoLoad : true,
						fields : [{
									name : 'sourceid',
									mapping : '合同来源类型ID'
								}, {
									name : 'sourcename',
									mapping : '合同来源类型'
								}]
					}),
			listeners : {
				'afterrender' : function(comb) {
					comb.setValue(0);
					comb.setRawValue('所有合同');
				},
				'select' : function(combo, record, index) {
					if (incomeCombinationSearchType == 1) {
						changeGrid(Type_ContractType_Income, 1, deptLimitsFlag);
					}

					// 将incomeGrid的pcStore、incomeStore的'sourceID'参数均清空
					delete pcStore.baseParams['sourceID'];
					delete incomeStore.baseParams['sourceID'];

					if (record.data.sourceid == 0) {
						delete incomeGrid.getStore().baseParams['sourceID'];
					} else {
						incomeGrid.getStore().setBaseParam('sourceID',
								record.data.sourceid);
					}
					// delete
					// incomeGrid.getStore().baseParams['selIds'];//清空基本查询参数(左侧checkbox列)
					incomeGrid.getStore().setBaseParam("warningType", 0);
					incomeGrid.getStore().load({
								params : {
									start : 0,
									limit : pcPageSize
								}
							});
					if (record.data.sourceid == 201) {// 合同来源为"公司自研"时将"财务来款合计"改成"预算拨款合计"
						incomeGrid.getColumnModel().setColumnHeader(
								incomeGrid.getColumnModel()
										.findColumnIndex(incomAmtTotalByGl),
								"预算拨款合计");
					}

				}
			}
		}, '-', {
			id : 'incomeDeptID',
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
									incomeGrid.deptCombo.setVisible(false);
								}
							}
						}
					}),
			listeners : {
				'select' : function(combo, record, index) {
					if (incomeCombinationSearchType == 1) {
						changeGrid(Type_ContractType_Income, 1, deptLimitsFlag);
					}

					// 将incomeGrid的pcStore、incomeStore的'deptID'参数均清空
					delete pcStore.baseParams['deptID'];
					delete incomeStore.baseParams['deptID'];

					var selValue = record.get('paramValue');
					if (selValue == '-1') {
						delete incomeGrid.getStore().baseParams['deptID'];
					} else {
						incomeGrid.getStore().setBaseParam('deptID', selValue);
					}
					// delete
					// incomeGrid.getStore().baseParams['selIds'];//清空基本查询参数(左侧checkbox列)
					incomeGrid.getStore().setBaseParam("warningType", 0);

					if (selValue.split(',').length == 1) {
						Ext.getCmp('incomeExcuteDeptID').hide();
						Ext.getCmp('incomeExcuteDeptID').setValue(0);
						Ext.getCmp('incomeExcuteDeptID').setRawValue('');
						delete pcStore.baseParams['excuteDeptID'];
						delete incomeStore.baseParams['excuteDeptID'];
					} else {
						Ext.getCmp('incomeExcuteDeptID').show();
						Ext.getCmp('incomeExcuteDeptID').getStore()
								.setBaseParam('deptIds', selValue);
					}

					incomeGrid.getStore().load({
								params : {
									start : 0,
									limit : pcPageSize
								}
							});
				}
			}
		}, '-', {
			id : 'incomeExcuteDeptID',
			xtype : 'combo',
			width : 120,
			displayField : 'deptName',
			valueField : 'deptID',
			editable : false,
			hidden : true,
			triggerAction : 'all',
			mode : 'remote',
			emptyText : '请选择执行部门',
			store : new Ext.data.JsonStore({
						url : String.format(
								'{0}/contractMain.do?method=getExcuteDeptInfo',
								__cxtPath),
						fields : [{
									name : 'deptID',
									mapping : 'deptID'
								}, {
									name : 'deptName',
									mapping : 'deptName'
								}]
					}),
			listeners : {
				'select' : function(combo, record, index) {
					if (incomeCombinationSearchType == 1) {
						changeGrid(Type_ContractType_Income, 1, deptLimitsFlag);
					}
					// 将incomeGrid的pcStore、incomeStore的'excuteDeptID'参数均清空
					delete pcStore.baseParams['excuteDeptID'];
					delete incomeStore.baseParams['excuteDeptID'];

					var value = record.get('deptID');
					if (value == 0) {
						delete incomeGrid.getStore().baseParams['excuteDeptID'];
					} else {
						incomeGrid.getStore().setBaseParam('excuteDeptID',
								value);
					}
					incomeGrid.getStore().setBaseParam('excuteDeptID', value);
					incomeGrid.getStore().load({
								params : {
									start : 0,
									limit : pcPageSize
								}
							});
				}
			}
		}, '-', '模糊查询：', new Ext.ux.InklingSearchField({
			id : 'incomeInklingID',
			width : 200,
			store : pcStore,
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
					}, {
						name : '备注',
						op : 'like',
						type : 'string'
					}, {
						name : 'execu_name',
						op : '=',
						type : 'string'
					}],
			onTrigger1Click : function() {

				// 将incomeGrid的pcStore、incomeStore的'inklings'、'this.paramName'参数均清空
				delete pcStore.baseParams['inklings'];
				delete incomeStore.baseParams['inklings'];
				pcStore.baseParams[this.paramName] = '';
				incomeStore.baseParams[this.paramName] = '';

				incomeGrid.getStore().baseParams = incomeGrid.getStore().baseParams
						|| {};
				delete incomeGrid.getStore().baseParams['inklings'];
				if (this.hasSearch) {
					this.el.dom.value = '';
					var o = {
						start : 0,
						limit : pcPageSize
					};
					incomeGrid.getStore().baseParams = incomeGrid.getStore().baseParams
							|| {};
					incomeGrid.getStore().baseParams[this.paramName] = '';
					incomeGrid.getStore().reload({
								params : o
							});
					this.triggers[0].hide();
					this.hasSearch = false;
				}
			},
			onTrigger2Click : function() {

				if (incomeCombinationSearchType == 1) {
					changeGrid(Type_ContractType_Income, 1, deptLimitsFlag);
				}

				// 将incomeGrid的pcStore、incomeStore的'inklings'、'this.paramName'参数均清空
				delete pcStore.baseParams['inklings'];
				delete incomeStore.baseParams['inklings'];
				delete pcStore.baseParams[this.paramName];
				delete incomeStore.baseParams[this.paramName];

				var v = this.getRawValue();
				if(v!=null){
					v=v.trim();
				}
					
				if (v.length < 1) {
					incomeGrid.getStore().baseParams = {
						searchName : '',
						warningType : 0,
						limit : pcPageSize
					};
					incomeGrid.getStore().reload();
				} else {
					var params = '';
					Ext.iterate(this.inklings, function(item, index, allItems) {
								params += item.name + ',' + item.op + ','
										+ item.type;
								if (index < allItems.length - 1) {
									params += '|';
								}
							}, this);
					// delete
					// incomeGrid.getStore().baseParams['selIds'];//清空基本查询参数(左侧checkbox列)
					incomeGrid.getStore().setBaseParam("warningType", 0);
					incomeGrid.getStore().baseParams = incomeGrid.getStore().baseParams
							|| {};
					incomeGrid.getStore().baseParams['inklings'] = params;
					incomeGrid.getStore().baseParams[this.paramName] = v;
					var o = {
						start : 0,
						limit : pcPageSize
					};
					incomeGrid.getStore().reload({
								params : o
							});
					this.hasSearch = true;
					this.triggers[0].show();
				}
			}
		}), '-', {
			ref : '../conditionBtn',
			iconCls : 'condition',
			text : '组合查询',
			handler : function(b, e) {
				searchConditionWin.show();
			}
		}, '->', '-', legendMsg],
		bbar : pcPagingToolbar
	});
	var outPagingToolbar = new Ext.ux.TotalPagingToolbar({
				pageSize : outPageSize,
				store : outStore,
				displayInfo : true,
				promptMsg : '',
				displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
				emptyMsg : "无显示数据",
				// plugins : [new Ext.ux.PagelimitCombo()],
				plugins : [new Ext.ux.PagelimitComboForRememberPageSize({
							rememberPage : Ext.state.Manager.get('outPage')
									? Ext.state.Manager.get('outPage')
									: 20
						})],
				items : [new Ext.Button({
							text : this.text || '导出Excel',
							iconCls : 'xls',
							menu : {
								items : [{
									text : '导出本页数据',
									handler : function() {
										exp.onExport(outGrid, exportnameOut,
												true);
									}
								}, {
									text : '导出所有数据',
									handler : function() {
										exp.onExport(outGrid, exportnameOut,
												false);
									}
								}]
							}
						})]
			});

	var outGrid = new Ext.grid.GridPanel({
		id : 'outGrid',
		cm : outColM,
		store : outStore,
		stripeRows : true,
		autoHeight : false,
		autoWidth : false,
		region : 'center',
		stateful : true,
		stateId : 'contract-list-out-grid',
		loadMask : true,
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		viewConfig : {
			getRowClass : function(record, rowIndex, rowParams, store) {
				if (store.getCount() - 1 < outPageSize
						&& store.getCount() == rowIndex + 1) {
					return 'grid-contract-total-list-yellow';
				} else {
					if (rowIndex == outPageSize) {
						return 'grid-contract-total-list-yellow';
					}
				}
			}
		},
		tbar : [{
			xtype : 'combo',
			id : 'outDeptID',
			ref : '../deptCombo',
			width : 120,
			displayField : 'deptName',
			editable : false,
			valueField : 'paramValue',
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
							contractTypeID : Type_ContractType_Payout,
							qxurl : outStore.proxy.url
						},
						idProperty : 'deptID',
						autoLoad : true,// 在点击菜单时自动load
						listeners : {
							'load' : function(s, records) {
								if (!records || records.length == 0) {
									outGrid.deptCombo.setVisible(false);
								}
							}
						}
					}),
			listeners : {
				'select' : function(combo, record, index) {
					if (outCombinationSearchType == 1) {
						changeGrid(Type_ContractType_Payout, 1, deptLimitsFlag);
					}

					// 将outGrid的outStore、searchStore的'deptID'参数均清空
					delete outStore.baseParams['deptID'];
					delete searchStore.baseParams['deptID'];

					var selValue = record.get('paramValue');
					if (selValue == '-1') {
						delete outGrid.getStore().baseParams['deptID'];
					} else {
						outGrid.getStore().setBaseParam('deptID', selValue);
					}
					// delete
					// outGrid.getStore().baseParams['selIds'];//清空基本查询参数(左侧checkbox列)
					outGrid.getStore().load({
								params : {
									start : 0,
									limit : outPageSize
								}
							});
				}
			}
		}, '-', '模糊查询：', new Ext.ux.InklingSearchField({
			id : 'outInklingID',
			width : 200,
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
			onTrigger1Click : function() {

				// 将outGrid的outStore、searchStore的'inklings'、'this.paramName'参数均清空
				delete outStore.baseParams['inklings'];
				delete searchStore.baseParams['inklings'];
				delete outStore.baseParams[this.paramName];
				delete searchStore.baseParams[this.paramName];

				outGrid.getStore().baseParams = outGrid.getStore().baseParams
						|| {};
				delete outGrid.getStore().baseParams['inklings'];
				// delete outGrid.getStore().baseParams['selIds'];
				if (this.hasSearch) {
					this.el.dom.value = '';
					var o = {
						start : 0,
						limit : outPageSize
					};
					outGrid.getStore().baseParams = outGrid.getStore().baseParams
							|| {};
					outGrid.getStore().baseParams[this.paramName] = '';
					outGrid.getStore().reload({
								params : o
							});
					this.triggers[0].hide();
					this.hasSearch = false;
				}
			},
			onTrigger2Click : function() {

				if (outCombinationSearchType == 1) {
					changeGrid(Type_ContractType_Payout, 1, deptLimitsFlag);
				}

				// 将outGrid的outStore、searchStore的'inklings'、'this.paramName'参数均清空
				delete outStore.baseParams['inklings'];
				delete searchStore.baseParams['inklings'];
				delete outStore.baseParams[this.paramName];
				delete searchStore.baseParams[this.paramName];

				var params = '';
				var v = this.getRawValue().trim();
				if(v!=null){
					v=v.trim();
				}
				Ext.iterate(this.inklings, function(item, index, allItems) {
							params += item.name + ',' + item.op + ','
									+ item.type;
							if (index < allItems.length - 1) {
								params += '|';
							}
						}, this);
				// delete
				// outGrid.getStore().baseParams['selIds'];//清空基本查询参数(左侧checkbox列)
				outGrid.getStore().baseParams = outGrid.getStore().baseParams
						|| {};
				outGrid.getStore().baseParams['inklings'] = params;
				outGrid.getStore().baseParams[this.paramName] = v;
				this.fireEvent('beforesearch', this, outGrid.getStore());
				var o = {
					start : 0,
					limit : outPageSize
				};
				outGrid.getStore().reload({
							params : o
						});
				this.hasSearch = true;
				this.triggers[0].show();
			}
		}), '-', {
			ref : '../conditionBtn',
			iconCls : 'condition',
			text : '组合查询',
			handler : function(b, e) {
				searchConditionoutWin.show();
			}
		}, '->', qxMsg],
		bbar : outPagingToolbar
	});

	// 收入合同基本查询
	var dynamicCondition;
	var dynamicConditionPanel = new Ext.Panel({
				region : 'west',
				id : 'dynamicConditionPanelID',
				baseCls : 'x-plain',
				bodyStyle : "padding:10px",
				buttonAlign : 'center',
				width : 255
			});
	// 收入合同高级查询
	var incomeSeniorSearchPanel = new Ext.form.FormPanel({
		id : 'searchConditionFrm',
		region : 'center',
		baseCls : 'x-plain',
		layout : 'form',
		hideLabels : true,
		bodyStyle : "padding:10px",
		buttonAlign : 'center',
		items : [{
			xtype : 'fieldset',
			title : '业务统计',
			autoHeight : true,
			labelAlign : 'right',
			labelWidth : 60,
			style : 'padding :0 5 0 5;!important',
			items : [new com.bitc.xmis.SearchPanel({
								boxLabel : '签订的合同',
								name : 'searchTypeIncome',
								inputValue : Type_Contract_Integration_New,
								timeId : 'IncomeNewContract'
							}), new com.bitc.xmis.SearchPanel({
								boxLabel : '质保期到期',
								name : 'searchTypeIncome',
								inputValue : Type_Contract_Integration_Warranty,
								timeId : 'IncomeWarranty'
							}), new com.bitc.xmis.SearchPanel({
								boxLabel : '收付款情况',
								name : 'searchTypeIncome',
								inputValue : Type_Contract_Integration_Performed,
								timeId : 'IncomePerformed'
							}), new com.bitc.xmis.SearchPanel({
								boxLabel : '发票情况',
								name : 'searchTypeIncome',
								inputValue : Type_Contract_Integration_Invoice,
								timeId : 'IncomeInvoice'
							}), new com.bitc.xmis.SearchPanel({
						boxLabel : '预计来款',
						name : 'searchTypeIncome',
						inputValue : Type_Contract_Integration_EstimateContractAmt,
						timeId : 'IncomeEstimate'
					})]
		}, {
			xtype : 'fieldset',
			title : '财务统计',
			autoHeight : true,
			labelAlign : 'right',
			labelWidth : 60,
			style : 'padding :0 5 0 5;!important',
			items : [new com.bitc.xmis.SearchPanel({
						boxLabel : '财务到款情况',
						name : 'searchTypeIncome',
						inputValue : Type_Contract_Integration_Finance,
						timeId : 'IncomeFinance'
					})]
		}, {
			xtype : 'displayfield',
			value : '<font color=red>"用户习惯保存" 对组合查询结果无效</font>'
		}],
		buttons : [{
			text : "确认",
			handler : function() {
				if (Ext.getCmp('searchConditionFrm').getForm()
						.findField('searchTypeIncome').getGroupValue() == null) {// 基本查询
					incomeCombinationSearchType = 1;
					dynamicCondition.setBaseParam();

					loadData(1, "", 0, Type_Contract_Integration_Simple);
					searchConditionWin.hide();

				} else {// 高级查询
					incomeCombinationSearchType = 2;
					dynamicCondition.setBaseParam(incomeStore);
					var groupValue = Ext.getCmp('searchConditionFrm').getForm()
							.findField('searchTypeIncome').getGroupValue();
					if (Type_Contract_Integration_New == groupValue) {
						beginDateLocal = Ext
								.getCmp('beginDateForIncomeNewContract')
								.getValue();
						endDateLocal = Ext
								.getCmp('endDateForIncomeNewContract')
								.getValue();
					} else if (Type_Contract_Integration_Warranty == groupValue) {
						beginDateLocal = Ext
								.getCmp('beginDateForIncomeWarranty')
								.getValue();
						endDateLocal = Ext.getCmp('endDateForIncomeWarranty')
								.getValue();
					} else if (Type_Contract_Integration_Performed == groupValue) {
						beginDateLocal = Ext
								.getCmp('beginDateForIncomePerformed')
								.getValue();
						endDateLocal = Ext.getCmp('endDateForIncomePerformed')
								.getValue();
					} else if (Type_Contract_Integration_Invoice == groupValue) {
						beginDateLocal = Ext
								.getCmp('beginDateForIncomeInvoice').getValue();
						endDateLocal = Ext.getCmp('endDateForIncomeInvoice')
								.getValue();
					} else if (Type_Contract_Integration_EstimateContractAmt == groupValue) {
						beginDateLocal = Ext
								.getCmp('beginDateForIncomeEstimate')
								.getValue();
						endDateLocal = Ext.getCmp('endDateForIncomeEstimate')
								.getValue();
					} else if (Type_Contract_Integration_Finance == groupValue) {
						beginDateLocal = Ext
								.getCmp('beginDateForIncomeFinance').getValue();
						endDateLocal = Ext.getCmp('endDateForIncomeFinance')
								.getValue();
					}

					if (convertDateObjToFormatStr(beginDateLocal) == "") {
						Ext.Msg.alert("提示", "请选择开始日期！");
					} else if (convertDateObjToFormatStr(endDateLocal) == "") {
						Ext.Msg.alert("提示", "请选择结束日期！");
					} else if (!dateCompare(beginDateLocal, endDateLocal)) {
						Ext.Msg.alert("提示", "结束日期必须晚于开始日期！");
					} else if (null == groupValue) {
						Ext.Msg.alert("提示", "请选择查询类别！");
					} else {
						loadData(1, "", 0, groupValue);
						searchConditionWin.hide();
					}
				}
			}
		}, {
			text : '条件重置',
			handler : function() {
				dynamicCondition.clear();
				delete pcStore.baseParams['selIds'];
				delete incomeStore.baseParams['selIds'];
				Ext.getCmp('searchConditionFrm').getForm().reset();
				incomeCombinationSearchType = 1;
				setDisabledTimeOptionForIncome();
			}
		}, {
			text : "取消",
			handler : function() {
				searchConditionWin.hide();
			}
		}]
	});

	// 收入合同组合查询
	var searchConditionWin = new Ext.Window({
				id : 'searchConditionWinID',
				title : '组合查询',
				modal : true,
				resizable : true,
				closeAction : "hide",
				closable : true,
				layout : "form",
				width : 700,
				autoHeight : true,
				resizable : false,
				activeItem : 0,
				items : [{
							xtype : 'panel',
							id : 'searchConditionTabID',
							layout : 'border',
							border : false,
							height : 360,
							baseCls : 'x-plain',
							items : [dynamicConditionPanel,
									incomeSeniorSearchPanel]
						}]
			});

	// 支出合同基本查询
	var dynamicConditionout;
	var dynamicConditionoutPanel = new Ext.Panel({
				region : 'west',
				id : 'dynamicConditionoutPanelID',
				baseCls : 'x-plain',
				width : 255,
				bodyStyle : "padding:10px",
				buttonAlign : 'center'
			});

	// 支出合同高级查询
	var outSeniorSearchPanel = new Ext.form.FormPanel({
		region : 'center',
		id : 'searchConditionoutFrm',
		baseCls : 'x-plain',
		layout : 'form',
		hideLabels : true,
		width : 240,
		bodyStyle : "padding:10px",
		buttonAlign : 'center',
		items : [{
			xtype : 'fieldset',
			title : '业务统计',
			autoHeight : true,
			labelAlign : 'right',
			labelWidth : 60,
			style : 'padding :0 5 0 5;!important',
			items : [new com.bitc.xmis.SearchPanel({
								boxLabel : '签订的合同',
								name : 'searchTypeOut',
								inputValue : Type_Contract_Integration_New,
								timeId : 'OutNewContract'
							}), new com.bitc.xmis.SearchPanel({
								boxLabel : '质保期到期',
								name : 'searchTypeOut',
								inputValue : Type_Contract_Integration_Warranty,
								timeId : 'OutWarranty'
							}), new com.bitc.xmis.SearchPanel({
								boxLabel : '收付款情况',
								name : 'searchTypeOut',
								inputValue : Type_Contract_Integration_Performed,
								timeId : 'OutPerformed'
							}), new com.bitc.xmis.SearchPanel({
								boxLabel : '发票情况',
								name : 'searchTypeOut',
								inputValue : Type_Contract_Integration_Invoice,
								timeId : 'OutInvoice'
							})]
		}, {
			xtype : 'displayfield',
			value : '<font color=red>"用户习惯保存" 对组合查询结果无效</font>'
		}],
		buttons : [{
			text : "确认",
			handler : function() {

				if (Ext.getCmp('searchConditionoutFrm').getForm()
						.findField('searchTypeOut').getGroupValue() == null) {// 基本查询
					outCombinationSearchType = 1;
					dynamicConditionout.setBaseParam();

					loadData(2, "", 0, Type_Contract_Integration_Simple);
					searchConditionoutWin.hide();
				} else {// 高级查询
					outCombinationSearchType = 2;
					dynamicConditionout.setBaseParam(searchStore);

					var groupValue = Ext.getCmp('searchConditionoutFrm')
							.getForm().findField('searchTypeOut')
							.getGroupValue();

					if (Type_Contract_Integration_New == groupValue) {
						beginDateOutLocal = Ext
								.getCmp('beginDateForOutNewContract')
								.getValue();
						endDateOutLocal = Ext
								.getCmp('endDateForOutNewContract').getValue();
					} else if (Type_Contract_Integration_Warranty == groupValue) {
						beginDateOutLocal = Ext
								.getCmp('beginDateForOutWarranty').getValue();
						endDateOutLocal = Ext.getCmp('endDateForOutWarranty')
								.getValue();
					} else if (Type_Contract_Integration_Performed == groupValue) {
						beginDateOutLocal = Ext
								.getCmp('beginDateForOutPerformed').getValue();
						endDateOutLocal = Ext.getCmp('endDateForOutPerformed')
								.getValue();
					} else if (Type_Contract_Integration_Invoice == groupValue) {
						beginDateOutLocal = Ext
								.getCmp('beginDateForOutInvoice').getValue();
						endDateOutLocal = Ext.getCmp('endDateForOutInvoice')
								.getValue();
					}

					if (convertDateObjToFormatStr(beginDateOutLocal) == "") {
						Ext.Msg.alert("提示", "请选择开始日期！");
					} else if (convertDateObjToFormatStr(endDateOutLocal) == "") {
						Ext.Msg.alert("提示", "请选择结束日期！");
					} else if (!dateCompare(beginDateOutLocal, endDateOutLocal)) {
						Ext.Msg.alert("提示", "结束日期必须晚于开始日期！");
					} else if (null == groupValue) {
						Ext.Msg.alert("提示", "请选择查询类别！");
					} else {
						loadData(2, "", 0, groupValue);
						searchConditionoutWin.hide();
					}
				}
			}
		}, {
			text : '条件重置',
			handler : function() {
				dynamicConditionout.clear();
				delete outStore.baseParams['selIds'];
				delete searchStore.baseParams['selIds'];
				Ext.getCmp('searchConditionoutFrm').getForm().reset();
				outCombinationSearchType = 1;
				setDisabledTimeOptionForOut();
			}
		}, {
			text : "取消",
			handler : function() {
				searchConditionoutWin.hide();
			}
		}]
	});

	// 支出合同组合查询
	var searchConditionoutWin = new Ext.Window({
				title : '组合查询',
				id : 'searchConditionoutWinID',
				layout : 'form',
				modal : true,
				closeAction : "hide",
				resizable : false,
				closable : true,
				width : 650,
				autoHeight : true,
				items : [{
							xtype : 'panel',
							id : 'searchConditionoutTabID',
							border : false,
							layout : 'border',
							height : 250,
							baseCls : 'x-plain',
							items : [dynamicConditionoutPanel,
									outSeniorSearchPanel]
						}]
			});

	Ext.ux.MultiSearchPanel.create(1, function(panel) {
				dynamicCondition = panel;
				panel.store = pcStore;
				panel.on('onsearch', function(p) {
							searchConditionWin.hide();
							p.clear();
						});
				panel.on('beforesearch', function(p) {
							pcStore.setBaseParam("warningType", 0);
							incomeStore.setBaseParam("warningType", 0);
						});
				dynamicConditionPanel.add(dynamicCondition);
				dynamicCondition.hideButtonPanel();
			}, this);

	Ext.ux.MultiSearchPanel.create(4, function(panel) {
				dynamicConditionout = panel;
				panel.store = outStore;
				panel.on('onsearch', function(p) {
							searchConditionoutWin.hide();
							p.clear();
						});
				dynamicConditionoutPanel.add(panel);
				dynamicConditionout.hideButtonPanel();
			}, this);

	var tabFlag = true;

	var tabs = new Ext.TabPanel({
				height : 535,
				region : 'center',
				margins : '3 3 3 0',
				activeTab : 1,
				tabPosition : 'top',
				listeners : {
					'tabchange' : function(a, activeTab) {
						if (activeTab.getItemId() == Type_ContractType_Payout
								&& tabFlag) {
							outStore.load({
										params : {
											start : 0,
											limit : outPageSize
										},
										callback : function(record, options,
												success) {
											if (success == true) {
												tabFlag = false;
											} else {
												Ext.Msg.alert("出错了", "加载数据错误");
											}
										}
									});
							tabs.removeListener('tabchange');
						}
					}
				},

				items : [{
							id : Type_ContractType_Income,
							title : '收入合同',
							layout : 'border',
							items : [incomeGrid]
						}, {
							id : Type_ContractType_Payout,
							title : '支出合同',
							layout : 'border',
							items : [outGrid]
						}]
			});

	new Ext.Viewport({
				layout : 'border',
				items : tabs
			});

	pcStore.load({
				params : {
					start : 0,
					limit : pcPageSize
				},
				callback : function(record, options, success) {
					if (!success) {
						Ext.Msg.alert("出错了", "加载数据错误");
					}
				}
			});

	warningTotalCountStore.load();

	this.warningFilter = function(type) {
		changeGrid(Type_ContractType_Income, 1, deptLimitsFlag);
		pcStore.removeAll();
		pcStore.setBaseParam("warningType", type);
		pcStore.load({
					params : {
						start : 0,
						limit : pcPageSize
					},
					callback : function(record, options, success) {

						if (success == true) {

						} else {
							Ext.Msg.alert(Ext_errMsg_For_Get_Store_Title,
									Ext_errMsg_For_Get_Store_Message);
						}
					}
				});
	};

	this.showPaymentOption = function(contractAbb, contractID, contractTypeID,
			contractAmt, warningDays, incomAmtTotalByGl, proID) {
		if (undefined == warningDays) {
			warningDays = 0;
		}
		var _url = __cxtPath
				+ '/contract/paymentEdit.jsp?'
				+ encodeURI('contractID=' + contractID + '&contractType='
						+ contractTypeID + '&contractName=' + contractAbb
						+ '&contractAmt=' + contractAmt);

		var items = [{
					xtype : 'panel',
					title : '合同款收付登记',
					id : 'payRegister',
					autoScroll : true,
					autoLoad : {
						url : _url,
						scripts : true
					}
				}, {
					xtype : 'panel',
					region : 'center',
					title : '付款计划',
					autoScroll : true,
					autoLoad : {
						url : __cxtPath
								+ '/contract/paymentCondition.jsp?contractID='
								+ contractID + '&contractAmt=' + contractAmt
								+ '&gls=' + incomAmtTotalByGl + '&projectId='
								+ proID + '&collapsible=false',
						scripts : true
					}
				}, new com.bitc.xmis.delayReasonGridPanel({
							title : "付款推迟原因登记",
							readOnly : false,
							secondTitle : "付款推迟原因登记",
							contractID : contractID
						})];
		if (!deptLimitsFlag) {
			items = [{
				xtype : 'panel',
				region : 'center',
				title : '付款计划',
				autoScroll : true,
				autoLoad : {
					url : __cxtPath
							+ '/contract/paymentCondition.jsp?contractID='
							+ contractID + '&contractAmt=' + contractAmt
							+ '&gls=' + incomAmtTotalByGl
							+ '&collapsible=false',
					scripts : true
				}
			}, new com.bitc.xmis.delayReasonGridPanel({
						title : "付款推迟原因登记",
						readOnly : false,
						secondTitle : "付款推迟原因登记",
						contractID : contractID
					})];
		}
		new Ext.Window({
					modal : true,
					title : contractAbb,
					width : 800,
					height : 476,
					layout : 'border',
					items : {
						xtype : 'tabpanel',
						id : 'PaymentOptionTabpanel',
						activeTab : 0,
						region : 'center',
						items : items
					}
				}).show();
	};
});
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
function turnToEditContract(contractID, contractName, contractType) {
	parent.xMis.turn('contract-' + contractID, contractName,
			'contractMain.do?method=contract&contractID=' + contractID, null);
}
function setDisabledTimeOption(checkbox, id) {
	setDisabledTimeOptionForIncome();
	setDisabledTimeOptionForOut();

	Ext.getCmp('beginDateFor' + id).setDisabled(false);
	Ext.getCmp('symbolFor' + id).setDisabled(false);
	Ext.getCmp('endDateFor' + id).setDisabled(false);
	return;
}

function setDisabledTimeOptionForIncome() {
	Ext.getCmp('beginDateForIncomeNewContract').setDisabled(true);
	Ext.getCmp('symbolForIncomeNewContract').setDisabled(true);
	Ext.getCmp('endDateForIncomeNewContract').setDisabled(true);

	Ext.getCmp('beginDateForIncomeWarranty').setDisabled(true);
	Ext.getCmp('symbolForIncomeWarranty').setDisabled(true);
	Ext.getCmp('endDateForIncomeWarranty').setDisabled(true);

	Ext.getCmp('beginDateForIncomePerformed').setDisabled(true);
	Ext.getCmp('symbolForIncomePerformed').setDisabled(true);
	Ext.getCmp('endDateForIncomePerformed').setDisabled(true);

	Ext.getCmp('beginDateForIncomeInvoice').setDisabled(true);
	Ext.getCmp('symbolForIncomeInvoice').setDisabled(true);
	Ext.getCmp('endDateForIncomeInvoice').setDisabled(true);

	Ext.getCmp('beginDateForIncomeEstimate').setDisabled(true);
	Ext.getCmp('symbolForIncomeEstimate').setDisabled(true);
	Ext.getCmp('endDateForIncomeEstimate').setDisabled(true);

	Ext.getCmp('beginDateForIncomeFinance').setDisabled(true);
	Ext.getCmp('symbolForIncomeFinance').setDisabled(true);
	Ext.getCmp('endDateForIncomeFinance').setDisabled(true);
}
function setDisabledTimeOptionForOut() {
	Ext.getCmp('beginDateForOutNewContract').setDisabled(true);
	Ext.getCmp('symbolForOutNewContract').setDisabled(true);
	Ext.getCmp('endDateForOutNewContract').setDisabled(true);

	Ext.getCmp('beginDateForOutWarranty').setDisabled(true);
	Ext.getCmp('symbolForOutWarranty').setDisabled(true);
	Ext.getCmp('endDateForOutWarranty').setDisabled(true);

	Ext.getCmp('beginDateForOutPerformed').setDisabled(true);
	Ext.getCmp('symbolForOutPerformed').setDisabled(true);
	Ext.getCmp('endDateForOutPerformed').setDisabled(true);

	Ext.getCmp('beginDateForOutInvoice').setDisabled(true);
	Ext.getCmp('symbolForOutInvoice').setDisabled(true);
	Ext.getCmp('endDateForOutInvoice').setDisabled(true);
}
