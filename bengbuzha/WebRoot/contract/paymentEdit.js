Ext.onReady(function() {
	Ext.QuickTips.init();
	/***************************************************************************
	 * *paymentDetailEditGrid
	 **************************************************************************/
	window.contractIdInEdit;
	var contractTypeInEdit;
	var contractNameInEdit;
	var hidenFlag;
	var selectRowNum = '';
	var setEditorHide = function(obj) {
		obj.reject(true);
		hidenFlag = false;
	};

	var hasFakeDate = false; // 事都存在初始化数据
	var fakeData = new Object();
	var amtSum = 0;

	var conOpType;//合同的操作类型，判断是否有‘单击编辑’权限    conOpType!=1，没有权限
	
	Ext.grid.RowNumberer = Ext.extend(Ext.grid.RowNumberer, {
				renderer : function(value, cellmeta, record, rowIndex,
						columnIndex, store) {
					return rowIndex + 1;
				}
			});

	var paymentDetailEditRecord = Ext.data.Record.create([{
				name : paymentChargedId
			}, {
				name : paymentChargedDateDisplayed,
				type : 'date'
			}, {
				name : paymentChargedDate
			}, {
				name : paymentAEmpName
			}, {
				name : paymentTransactorID
			}, {
				name : paymentChargedAmt,
				type : 'float'
			}, {
				name : paymentBEmpName
			}, {
				name : paymentUpdateDate
			}, {
				name : paymentUpdateDateDisplayed,
				type : 'date'
			}, {
				name : paymentCurrencyGraph
			}, {
				name : paymentCurrencyExchangeRate
			}, {
				name : paymentChargedRemark
			}]);
	var paymentDetailEditStore = new Ext.data.Store({
		url : __cxtPath + '/contractMain.do?method=getPaymentList',
		remoteSort : false,
		sortInfo : {
			field : paymentChargedDateDisplayed,
			direction : 'ASC'
		},
		reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount',
					fields : paymentDetailEditRecord
				}),
		listeners : {
			'load' : function(s, records) {
				var sum = 0;
				Ext.iterate(records, function(record) {
							sum += record.get(paymentChargedAmt);
						});
				amtSum = sum;
				var t = '';
				if (sum != 0) {
					if (contractType == 2) {
						t = '付款明细列表 - 合计：'
								+ Ext.util.Format.number(sum, '0,0.00');
					} else {
						t = '收款明细列表 - 合计：'
								+ Ext.util.Format.number(sum, '0,0.00');
					}
				} else {
					if (contractType == 2) {
						t = '付款明细列表';
					} else {
						t = '收款明细列表';
					}

				}
				if (records != null && records.length == 1
						&& records[0].get(paymentChargedId) == -1) {
					t = t
							+ '<font color=red>&nbsp;(当前信息是通过财务系统计算得出的初始化数据！)</font>';
				}
				paymentEditPanel.setTitle(t);
			},
			'beforeload' : function(){
				//判断合同的操作类型
				if (typeof (contractOpType) === 'undefined') {
					Ext.Ajax.request({
						url : __cxtPath
								+'/contractMain.do?method=getContractOpType',
							params : {
								contractID : contractIdInEdit
							},
						success : function(response,opt){
							var obj = Ext.decode(response.responseText);
							var type = obj.attributes.contractOpType;
							if(type ==5){
								paymentDetailEditGrid.getTopToolbar().enable();
								downloadGridInPayEd.setActionColumnHidden(false);
								conOpType = 1;
							}
						},
						failure : function(response,opt){
							var obj = Ext.decode(response.responseText);
							Ext.Msg
									.alert(obj.messageTitle,
										obj.message);
						}
					});
				}else{
					if(contractOpType ==5){
						paymentDetailEditGrid.getTopToolbar().enable();
						downloadGridInPayEd.setActionColumnHidden(false);
						conOpType = 1;
					}
				}
			}
		}
	});

	var paymentEditAEmpIDField = new Ext.form.TextField({});
	var paymentEditIdField = new Ext.form.TextField({});
	var paymentAmtEditField = new Ext.form.NumberField({
		// minValue : 0,
		allowBlank : false,
		listeners : {
			'blur' : function() {
				if (this.getValue() == '0') {
					Ext.Msg.alert("提示", "金额不能为0！");
					paymentAmtEditField.setValue('');
					return false;
				}
				if (this.getValue() < 0) {
					Ext.Msg.alert('提示', '当前输入的收款金额小于0!');
				}
				if (this.getValue() != 0) {
					var paymentTotal = 0;// this.getValue();
					if (selectRowNum == '') {
						for (var i = 0; i < paymentDetailEditStore.getCount(); i++) {
							var amtInGrid = 0.00;
							if (i == 0) {
								amtInGrid = paymentAmtEditField.getValue();
							} else {
								amtInGrid = paymentDetailEditStore.getAt(i)
										.get(paymentChargedAmt);
							}
							if (amtInGrid != '' && amtInGrid != undefined) {
								paymentTotal = paymentTotal + amtInGrid * 1;
							}
						}
					} else {
						for (var i = 0; i < paymentDetailEditStore.getCount(); i++) {
							var amtInGrid = paymentDetailEditStore.getAt(i)
									.get(paymentChargedAmt);
							if (amtInGrid != '' && amtInGrid != undefined
									&& i != selectRowNum) {
								paymentTotal = paymentTotal + amtInGrid * 1;
							}
						}

					}
					if (paymentTotal > chooseContractAmt) {
						var msg = contractTypeInEdit == 1
								? '收款金额大于合同总额，是否继续？'
								: '付款大于合同总额，是否继续？';
						Ext.MessageBox.show({
									title : "提示",
									msg : msg,
									buttons : Ext.Msg.YESNO,
									fn : function(btn) {
										if (btn == 'no') {
											paymentAmtEditField.setValue('');
										};
									},
									width : 280,
									icon : Ext.MessageBox.INFO,
									closable : false
								});
					}
				}
			}
		}
	});

	var paymentDateEditField = new Ext.form.DateField({
				format : 'Y-m-d',
				allowBlank : false
			});
	var paymentUpDateDEditField = new Ext.form.DateField({
				format : 'Y-m-d',
				readOnly : true
			});
	var paymentEditRemarkField = new Ext.form.TextField({

	});

	/***************************************************************************
	 * PaymentAEmpPeasonWin
	 * 
	 */
	var currencyx = '';

	var PaymentAEmpPeasonSM = new Ext.grid.CheckboxSelectionModel({
				header : '',
				singleSelect : true
			});

	var PaymentAEmpPeasonColM = new Ext.grid.ColumnModel([PaymentAEmpPeasonSM,
			{
				header : "部门名称",
				dataIndex : "deptName",
				sortable : true,
				hidden : true
			}, {
				header : "联系人名称",
				dataIndex : "user_name",
				sortable : true
			}]);

	var PaymentAEmpPeasonStore = new Ext.data.GroupingStore({
				proxy : new Ext.data.HttpProxy({
							url : __cxtPath
									+ '/crmMaint.do?method=getContactPersonByCompany'
						}),
				remoteSort : true,
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount',
							remoteSort : true,
							fields : [{
										name : 'deptName'
									}, {
										name : EmployeeIDInPaymentCharged
									}, {
										name : 'user_name'
									}]
						}),
				groupField : 'deptName'
			});

	PaymentAEmpPeasonStore.on('load', function(store, records, options) {
				var selectValue = searchPaymentAEmpField.getValue();
				if (selectValue !== '') {
					PaymentAEmpPeasonGrid.getView().expandAllGroups();
					var arr = [];
					for (var i = 0; i < records.length; i++) {
						var record = records[i];
						record.get('user_name')
						if (record.get('user_name') == selectValue) {
							arr.push(record);
						}
					}
					PaymentAEmpPeasonSM.selectRecords(arr);
				} else {
					PaymentAEmpPeasonGrid.getView().collapseAllGroups();
				}
			}, this, {
				delay : 100
			});
	this.showPaymentEditGridInJS = function(id, contractName, contractType,
			curr) {
		if (curr == "欧元") {
			currencyx = '€';
		} else {
			if (curr == "") {
				currencyx = '￥';
			} else {
				currencyx = curr;
			}
		};
		paymentEditPanel.show();// 显示整个信息panel
		if (contractType == IncomeContractTypeInPE) {
			paymentEditPanel.setTitle("收款明细列表" + '-' + contractName);
			paymentDetailEditGrid.getColumnModel().setColumnHeader(2, "收款日期");
			paymentDetailEditGrid.getColumnModel().setColumnHeader(3, "收款金额");

		} else {
			paymentEditPanel.setTitle("付款明细列表");
			paymentDetailEditGrid.getColumnModel().setHidden(1, true);
			paymentDetailEditGrid.getColumnModel().setColumnHeader(2, "付款日期");
			paymentDetailEditGrid.getColumnModel().setColumnHeader(3, "付款金额");
			paymentDetailEditGrid.getColumnModel().setHidden(8, false);
			paymentDetailEditGrid.getColumnModel().setHidden(7, false);
		}
		// downloadStoreInPayEd.removeAll();
		// downloadStoreInPayEd.load(
		// {
		// params : {
		// applicationType : attachType,
		// refID : id
		// }
		// });
		contractTypeInEdit = contractType;
		contractIdInEdit = id;
		contractNameInEdit = contractName;
		paymentDetailEditStore.removeAll(true);
		paymentDetailEditStore.load({
					params : {
						contractID : id
					}
				});

		if (paymentEditPanel.isVisible()) {
			paymentEditPanel.focus('', 5);
		}

	};

	var PaymentAEmpPeasonGrid = new Ext.grid.GridPanel({
				sm : PaymentAEmpPeasonSM,
				cm : PaymentAEmpPeasonColM,
				store : PaymentAEmpPeasonStore,
				view : new Ext.grid.GroupingView({
							forceFit : true,
							hideGroupedColumn : true, // 用来分组的数据点这一列是否要显示
							showGroupName : false, // 用来分组的数据点这一列的header是否要随group
							startCollapsed : true
						}), // 此为打开窗口时，找到已选的项
				stripeRows : true, // True表示使用不同颜色间隔行，默认为false
				autoHeight : false,
				loadMask : true, // 读取数据时的遮罩和提示功能，默认为false
				loadMask : {
					msg : '正在加载数据，请稍侯……'
				},
				height : 320,
				tbar : [{
					ref : '../okBtn',
					iconCls : 'ok',
					text : '确定',
					handler : function() {
						var record = PaymentAEmpPeasonGrid.getSelectionModel()
								.getSelected();
						if (record == 'undefined' || record == undefined) {
							Ext.Msg.alert("未选择", "请先选择！");
							return true;
						}
						searchPaymentAEmpField
								.setValue(record.get('user_name'));
						paymentEditAEmpIDField.setValue(record
								.get(EmployeeIDInPaymentCharged));
						PaymentAEmpPeasonWin.hide();

					}
				}, '-', {
					ref : '../cancelBtn',
					iconCls : 'remove',
					text : '取消',
					disabled : false,
					handler : function() {
						if (PaymentAEmpPeasonWin)
							PaymentAEmpPeasonWin.hide();
					}
				}]
			});

	var PaymentAEmpPeasonWin = new Ext.Window({
				modal : true,
				frame : true,
				plain : true,
				resizable : false,
				closeAction : "hide",
				closable : true,
				bodyStyle : "padding:10px",
				width : 300,
				height : 400,
				items : [PaymentAEmpPeasonGrid]
			});

	var searchPaymentAEmpField = new Ext.ux.form.SearchField({
				allowBlank : false,
				emptyText : "必选项...",
				hasSearch : false,
				validateOnBlur : true,
				editable : false,
				onTrigger2Click : function() {// 点击查询按钮或回车调用该方法
					PaymentAEmpPeasonWin.show();
					PaymentAEmpPeasonWin.setTitle("选择经办人");
					PaymentAEmpPeasonStore.removeAll();
					PaymentAEmpPeasonStore.proxy
							.setUrl(
									__cxtPath
											+ '/crmMaint.do?method=getContactPersonByCompany',
									true);
					PaymentAEmpPeasonStore.load({
								params : {
									compID : companyIdInPay
								},
								callback : function(record, options, success) {
									if (success == true) {
									} else {
										Ext.Msg
												.alert(
														Ext_errMsg_For_Get_Store_Title,
														Ext_errMsg_For_Get_Store_Message);
									}
								}
							});
				}
			});
	/** ****************flag****************** */
	var newPaymentEditRowFlag = 0;
	var paymentDetailEditGridEditor = new Ext.ux.grid.RowEditor({
				saveText : '确认',
				cancelText : '取消',
				clicksToEdit : 2
			});
	paymentDetailEditGridEditor.on({
		scope : this,
		beforeedit : function() {
			paymentDetailEditGrid.removeBtn.setDisabled(true);
			paymentDetailEditGrid.newBtn.setDisabled(true);
			if(conOpType != 1){
				return false;
			}
		},
		beforehide : function() {
			var flag = hidenFlag;
			hidenFlag = true;
			return flag;
		},
		afteredit : function(roweditor, changes, record, rowIndex) {
			if (hasFakeDate) { // 如果含有初始化信息
				if (record.get(paymentChargedId) == -1) { // 如果编辑的是初始化信息
					fakeData.paymentDetailDate = record.get(paymentChargedDate);
					fakeData.paymentDetailAmt = record.get(paymentChargedAmt);
					fakeData.paymentDetailRemark = record
							.get(paymentChargedRemark);
					fakeData.transator = record.get(paymentTransactorID);
					Ext.MessageBox.show({
						title : '提示信息',
						msg : '确认将初始化数据直接设定为收款明细吗？',
						buttons : Ext.Msg.YESNO,
						fn : function(btn) {
							if (btn == 'yes') {
								paymentListEditForm.getForm().submit({
									url : __cxtPath
											+ '/contractMain.do?method=savePaymentDetail',
									method : 'post',
									params : {
										contractID : contractIdInEdit,
										paymentDetailID : 0,
										contractType : 1,
										paymentDetailDate : fakeData.paymentDetailDate,
										paymentDetailAmt : fakeData.paymentDetailAmt,
										paymentDetailRemark : fakeData.paymentDetailRemark,
										transatorID : fakeData.transatorID,
										hasFakeDate : false
									},
									success : function(form, action) {
										Ext.Msg.alert(
												action.result.messageTitle,
												action.result.message);
										hasFakeDate = false;
										if (Ext.getCmp("paymentListEditGrid")) {
											reloadPaymentListEdit();// 刷新合同列表中的付款项
										}
										paymentDetailEditGrid.getStore()
												.reload({
													callback : function(record,
															options, success) {
														if (success == true) {
															var totalAmt = '';
															if (amtSum != 0) {
																totalAmt = formatCurrency(
																		null,
																		amtSum
																				+ '',
																		Color_Font_Contract_Total_Charged_Amt);
															}
															var unAmt = contractAmt
																	- amtSum;
															earnedAmtTemp = amtSum;
															var unTotalAmt = formatCurrency(
																	null,
																	unAmt + '',
																	'red');
															Ext
																	.get('Amt')
																	.update(totalAmt);
															Ext
																	.get('unAmt')
																	.update('未收合同金额：'
																			+ unTotalAmt);
															var invoicedUnAmt = (invoicedAmtTemp - amtSum) <= 0
																	? 0
																	: invoicedAmtTemp
																			- amtSum;
															var invoicedUnAmtStr = formatCurrency(
																	null,
																	invoicedUnAmt
																			+ '',
																	Color_Font_Contract_Total_UnAccount_Amt);
															Ext
																	.get('invoicedUnAmt')
																	.update('已开发票未到账金额：'
																			+ invoicedUnAmtStr);
														} else {
															Ext.Msg
																	.alert(
																			Ext_errMsg_For_Get_Store_Title,
																			Ext_errMsg_For_Get_Store_Message);
														}
													}
												});
									},
									failure : function(form, action) {
										Ext.Msg.alert(
												action.result.messageTitle,
												action.result.message);
										var s = paymentDetailEditGrid
												.getSelectionModel()
												.getSelections();
										for (var i = 0, r; r = s[i]; i++) {
											paymentDetailEditStore.remove(r);
										};
									}
								}); // end submit
								paymentDetailEditGrid.removeBtn
										.setDisabled(false);
								paymentDetailEditGrid.newBtn.setDisabled(false);
								newPaymentEditRowFlag = 0;
								selectRowNum = '';
							} else {
								paymentDetailEditStore.reload();
								paymentDetailEditGrid.removeBtn
										.setDisabled(false);
								paymentDetailEditGrid.newBtn.setDisabled(false);
								selectRowNum = '';
							}
						}
					});

				}
				if (record.get(paymentChargedId) != -1) { // 如果编辑的不是初始化信息
					var msg = '<table><tr><td colspan=2 style="font-size:12px;">请确认您要进行的操作！</td></tr><tr><td style="font-size:12px;">是：</td><td style="font-size:12px;">选择是则保存财务初始化数据及新录入数据</td></tr><tr><td style="font-size:12px;">否：</td><td style="font-size:12px;">只保存初始化数据</td></tr><tr><td style="font-size:12px;">取消：</td><td style="font-size:12px;">取消保存操作</td></tr></table>';
					Ext.MessageBox.show({
						title : '提示信息',
						msg : msg,
						buttons : Ext.Msg.YESNOCANCEL,
						fn : function(btn) {
							if (btn == 'yes') { // 初始化信息和新数据都保存
								paymentListEditForm.getForm().submit({
									url : __cxtPath
											+ '/contractMain.do?method=savePaymentDetail',
									method : 'post',
									params : {
										contractID : contractIdInEdit,
										paymentDetailID : paymentEditIdField
												.getValue(),
										contractType : contractTypeInEdit,
										paymentDetailDate : paymentDateEditField
												.getValue(),
										paymentDetailAmt : paymentAmtEditField
												.getValue(),
										paymentDetailRemark : paymentEditRemarkField
												.getValue(),
										transatorID : paymentEditAEmpIDField
												.getValue(),
										hasFakeDate : hasFakeDate,
										fakePaymentDetailID : fakeData.paymentDetailID,
										fakeContractType : 1,
										fakePaymentDetailDate : fakeData.paymentDetailDate,
										fakePaymentDetailAmt : fakeData.paymentDetailAmt,
										fakePaymentDetailRemark : fakeData.paymentDetailRemark,
										fakeTransatorID : fakeData.transatorID
									},
									success : function(form, action) {
										Ext.Msg.alert(
												action.result.messageTitle,
												action.result.message);
										hasFakeDate = false;
										if (Ext.getCmp("paymentListEditGrid")) {
											reloadPaymentListEdit();// 刷新合同列表中的付款项
										}
										paymentDetailEditGrid.getStore()
												.reload({
													callback : function(record,
															options, success) {
														if (success == true) {
															var totalAmt = '';
															if (amtSum != 0) {
																totalAmt = formatCurrency(
																		null,
																		amtSum
																				+ '',
																		Color_Font_Contract_Total_Charged_Amt);
															}
															var unAmt = contractAmt
																	- amtSum;
															earnedAmtTemp = amtSum;
															var unTotalAmt = formatCurrency(
																	null,
																	unAmt + '',
																	'red');
															Ext
																	.get('Amt')
																	.update(totalAmt);
															Ext
																	.get('unAmt')
																	.update('未收合同金额：'
																			+ unTotalAmt);
															var invoicedUnAmt = (invoicedAmtTemp - amtSum) <= 0
																	? 0
																	: invoicedAmtTemp
																			- amtSum;
															var invoicedUnAmtStr = formatCurrency(
																	null,
																	invoicedUnAmt
																			+ '',
																	Color_Font_Contract_Total_UnAccount_Amt);
															Ext
																	.get('invoicedUnAmt')
																	.update('已开发票未到账金额：'
																			+ invoicedUnAmtStr);
														} else {
															Ext.Msg
																	.alert(
																			Ext_errMsg_For_Get_Store_Title,
																			Ext_errMsg_For_Get_Store_Message);
														}
													}
												});
									},
									failure : function(form, action) {
										Ext.Msg.alert(
												action.result.messageTitle,
												action.result.message);
										var s = paymentDetailEditGrid
												.getSelectionModel()
												.getSelections();
										for (var i = 0, r; r = s[i]; i++) {
											paymentDetailEditStore.remove(r);
										};
									}
								}); // end submit
								paymentDetailEditGrid.removeBtn
										.setDisabled(false);
								paymentDetailEditGrid.newBtn.setDisabled(false);
								newPaymentEditRowFlag = 0;
								selectRowNum = '';
							} // end if(yes)
							if (btn == 'no') { // 只保存初始化信息
								paymentListEditForm.getForm().submit({
									url : __cxtPath
											+ '/contractMain.do?method=savePaymentDetail',
									method : 'post',
									params : {
										contractID : contractIdInEdit,
										paymentDetailID : paymentEditIdField
												.getValue(),
										contractType : 1,
										paymentDetailDate : fakeData.paymentDetailDate,
										paymentDetailAmt : fakeData.paymentDetailAmt,
										paymentDetailRemark : fakeData.paymentDetailRemark,
										transatorID : fakeData.transatorID,
										hasFakeDate : false
									},
									success : function(form, action) {
										Ext.Msg.alert(
												action.result.messageTitle,
												action.result.message);
										hasFakeDate = false;
										if (Ext.getCmp("paymentListEditGrid")) {
											reloadPaymentListEdit();// 刷新合同列表中的付款项
										}
										paymentDetailEditGrid.getStore()
												.reload({
													callback : function(record,
															options, success) {
														if (success == true) {
															var totalAmt = '';
															if (amtSum != 0) {
																totalAmt = formatCurrency(
																		null,
																		amtSum
																				+ '',
																		Color_Font_Contract_Total_Charged_Amt);
															}
															var unAmt = contractAmt
																	- amtSum;
															earnedAmtTemp = amtSum;
															var unTotalAmt = formatCurrency(
																	null,
																	unAmt + '',
																	'red');
															Ext
																	.get('Amt')
																	.update(totalAmt);
															Ext
																	.get('unAmt')
																	.update('未收合同金额：'
																			+ unTotalAmt);
															var invoicedUnAmt = (invoicedAmtTemp - amtSum) <= 0
																	? 0
																	: invoicedAmtTemp
																			- amtSum;
															var invoicedUnAmtStr = formatCurrency(
																	null,
																	invoicedUnAmt
																			+ '',
																	Color_Font_Contract_Total_UnAccount_Amt);
															Ext
																	.get('invoicedUnAmt')
																	.update('已开发票未到账金额：'
																			+ invoicedUnAmtStr);
														} else {
															Ext.Msg
																	.alert(
																			Ext_errMsg_For_Get_Store_Title,
																			Ext_errMsg_For_Get_Store_Message);
														}
													}
												});
									},
									failure : function(form, action) {
										Ext.Msg.alert(
												action.result.messageTitle,
												action.result.message);
										var s = paymentDetailEditGrid
												.getSelectionModel()
												.getSelections();
										for (var i = 0, r; r = s[i]; i++) {
											paymentDetailEditStore.remove(r);
										};
									}
								}); // end submit
								paymentDetailEditGrid.removeBtn
										.setDisabled(false);
								paymentDetailEditGrid.newBtn.setDisabled(false);
								newPaymentEditRowFlag = 0;
								selectRowNum = '';
							} // end if(no)
							if (btn == 'cancel') {
								if (newPaymentEditRowFlag == 1) {
									paymentDetailEditGridEditor
											.stopEditing(false);
									var s = paymentDetailEditGrid
											.getSelectionModel()
											.getSelections();
									for (var i = 0, r; r = s[i]; i++) {
										paymentDetailEditStore.remove(r);
									};
									hidenFlag = true;
									paymentDetailEditGrid.getView().refresh();
									newPaymentEditRowFlag = 0;
								}
								paymentDetailEditGrid.getView().refresh();
								paymentDetailEditGrid.removeBtn
										.setDisabled(false);
								paymentDetailEditGrid.newBtn.setDisabled(false);
								selectRowNum = '';
							}// end if (cancel)
						},
						animEl : 'elId',
						icon : Ext.MessageBox.QUESTION
					});
				} // end if
			} else {
				paymentListEditForm.getForm().submit({
					url : __cxtPath
							+ '/contractMain.do?method=savePaymentDetail',
					method : 'post',
					params : {
						contractID : contractIdInEdit,
						paymentDetailID : paymentEditIdField.getValue(),
						contractType : contractTypeInEdit,
						paymentDetailDate : paymentDateEditField.getValue(),
						paymentDetailAmt : paymentAmtEditField.getValue(),
						paymentDetailRemark : paymentEditRemarkField.getValue(),
						transatorID : paymentEditAEmpIDField.getValue()
					},
					success : function(form, action) {
						Ext.Msg.alert(action.result.messageTitle,
								action.result.message);
						if (Ext.getCmp("paymentListEditGrid")) {
							reloadPaymentListEdit();// 刷新合同列表中的付款项
						}
						paymentDetailEditGrid.getStore().reload({
							callback : function(record, options, success) {
								if (success == true) {
									var totalAmt = '';
									if (amtSum != 0) {
										totalAmt = formatCurrency(null, amtSum
														+ '',
												Color_Font_Contract_Total_Charged_Amt);
									}
									var unAmt = contractAmt - amtSum;
									earnedAmtTemp = amtSum;
									var unTotalAmt = formatCurrency(null, unAmt
													+ '', 'red');
									Ext.get('Amt').update(totalAmt);
									Ext.get('unAmt').update('未收合同金额：'
											+ unTotalAmt);
									var invoicedUnAmt = (invoicedAmtTemp - amtSum) <= 0
											? 0
											: invoicedAmtTemp - amtSum;
									var invoicedUnAmtStr = formatCurrency(null,
											invoicedUnAmt + '',
											Color_Font_Contract_Total_UnAccount_Amt);
									Ext.get('invoicedUnAmt')
											.update('已开发票未到账金额：'
													+ invoicedUnAmtStr);

								} else {
									Ext.Msg.alert(
											Ext_errMsg_For_Get_Store_Title,
											Ext_errMsg_For_Get_Store_Message);
								}
							}
						});

					},
					failure : function(form, action) {
						Ext.Msg.alert(action.result.messageTitle,
								action.result.message);
						var s = paymentDetailEditGrid.getSelectionModel()
								.getSelections();
						for (var i = 0, r; r = s[i]; i++) {
							paymentDetailEditStore.remove(r);
						};
					}
				});
				paymentDetailEditGrid.removeBtn.setDisabled(false);
				paymentDetailEditGrid.newBtn.setDisabled(false);
				newPaymentEditRowFlag = 0;
				selectRowNum = '';
			}
		},
		canceledit : function() {
			if (newPaymentEditRowFlag == 1) {
				paymentDetailEditGridEditor.stopEditing(false);
				var s = paymentDetailEditGrid.getSelectionModel()
						.getSelections();
				for (var i = 0, r; r = s[i]; i++) {
					paymentDetailEditStore.remove(r);
				};
				hidenFlag = true;
				paymentDetailEditGrid.getView().refresh();
				newPaymentEditRowFlag = 0;
			}
			paymentDetailEditGrid.getView().refresh();
			paymentDetailEditGrid.removeBtn.setDisabled(false);
			paymentDetailEditGrid.newBtn.setDisabled(false);
			selectRowNum = '';
		}
	})

	/***************************************************************************
	 * 收款明细列表
	 **************************************************************************/

	var exp = new Ext.ux.Export({
				formater : [{
							columnHeader : '',
							formatClass : 'com.bitc.jk.xmis.export.RowNumber'
						}, {
							columnHeader : '收款日期',
							formatClass : 'com.bitc.jk.xmis.export.SubstringTimer'
						}, {
							columnHeader : '收款日期',
							formatClass : 'com.bitc.jk.xmis.export.DateFormat'
						}, {
							columnHeader : '登记日期',
							formatClass : 'com.bitc.jk.xmis.export.DateFormat'
						}]
			});

	var paymentDetailEditGrid = new Ext.grid.GridPanel({
		region : 'center',
		store : paymentDetailEditStore,
		stripeRows : true, // True表示使用不同颜色间隔行，默认为false
		region : 'center',
		viewConfig : {
			getRowClass : function(record, rowIndex, rowParams, store) {
				if (record.get(paymentChargedId) == -1) {
					hasFakeDate = true;
					fakeData.contractID = contractIdInEdit;
					fakeData.paymentDetailID = record.get(paymentChargedId);
					fakeData.contractType = 1;
					fakeData.paymentDetailDate = record.get(paymentChargedDate);
					fakeData.paymentDetailAmt = record.get(paymentChargedAmt);
					fakeData.paymentDetailRemark = record
							.get(paymentChargedRemark);
					fakeData.transatorID = record.get(paymentTransactorID);
					return 'grid-contract-total-list-yellow';
				}
			}
		},
		listeners : {
			'rowdblclick' : function(gr, rowIndex, e) {
				selectRowNum = rowIndex;
			}
		},
		loadMask : true, // 读取数据时的遮罩和提示功能，默认为false
		plugins : !readOnly ? [paymentDetailEditGridEditor] : [],
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		tbar : {
			disabled : true,
			items :[{
					ref : '../newBtn',
					iconCls : 'add',
					text : '新建',
					handler : function() {

						var e = new paymentDetailEditRecord({
									paymentChargedId : '',
									paymentChargedDateDisplayed : '',// (new
									// Date()).format('Y-m-d'),
									paymentChargedDate : '',// add 1
									paymentAEmpName : '',
									paymentChargedAmt : '0',
									paymentBEmpName : '',
									paymentUpdateDate : '',
									paymentTransactorID : '',
									paymentUpdateDateDisplayed : '',// add 2
									paymentCurrencyGraph : '',
									paymentCurrencyExchangeRate : '',
									paymentChargedRemark : ''
								});
						paymentDetailEditGridEditor.stopEditing();
						paymentDetailEditStore.insert(0, e);
						paymentDetailEditGrid.getView().refresh();
						paymentDetailEditGrid.getSelectionModel().selectRow(0);
						paymentDetailEditGridEditor.startEditing(0);
						newPaymentEditRowFlag = 1;
					}
				}, '-', {
					ref : '../removeBtn',
					iconCls : 'remove',
					text : '删除',
					disabled : false,
					handler : function() {

						var record = paymentDetailEditGrid.getSelectionModel()
								.getSelected();
						if (record == 'undefined' || record == undefined) {
							Ext.Msg.alert("未选择", "请先选择！");
							return true;
						}
						if (record.get(paymentChargedId) == -1) {
							Ext.Msg.alert("提示", "初始化数据，无需删除！");
						} else {
							Ext.Msg.confirm("请确认", "确定要删除该条收款明细吗？", function(
									btn) {
								if (btn == 'yes') {
									paymentListEditForm.getForm().submit({
										url : __cxtPath
												+ '/contractMain.do?method=delPaymentDetail',
										method : 'post',
										params : {
											contractID : contractIdInEdit,
											paymentDetailID : record
													.get(paymentChargedId)
										},
										success : function(form, action) {
											Ext.Msg.alert(
													action.result.messageTitle,
													action.result.message);
											if (Ext
													.getCmp("paymentListEditGrid")) {
												reloadPaymentListEdit();// 刷新合同列表中的付款项
											}
											paymentDetailEditGrid.getStore()
													.reload({
														callback : function(
																record,
																options,
																success) {
															if (success == true) {
																var totalAmt = '';
																if (hasFakeDate) {
																	amtSum = 0;
																}
																if (amtSum != 0) {
																	totalAmt = formatCurrency(
																			null,
																			amtSum
																					+ '',
																			Color_Font_Contract_Total_Charged_Amt);
																}
																var unAmt = contractAmt
																		- amtSum;
																earnedAmtTemp = amtSum;
																var unTotalAmt = formatCurrency(
																		null,
																		unAmt
																				+ '',
																		'red');
																if (Ext
																		.get('Amt'))
																	Ext
																			.get('Amt')
																			.update(totalAmt);
																if (Ext
																		.get('unAmt'))
																	Ext
																			.get('unAmt')
																			.update('未收合同金额：'
																					+ unTotalAmt);
																var invoicedUnAmt = (invoicedAmtTemp - amtSum) <= 0
																		? 0
																		: invoicedAmtTemp
																				- amtSum;
																var invoicedUnAmtStr = formatCurrency(
																		null,
																		invoicedUnAmt
																				+ '',
																		Color_Font_Contract_Total_UnAccount_Amt);
																Ext
																		.get('invoicedUnAmt')
																		.update('已开发票未到账金额：'
																				+ invoicedUnAmtStr);
															} else {
																Ext.Msg
																		.alert(
																				Ext_errMsg_For_Get_Store_Title,
																				Ext_errMsg_For_Get_Store_Message);
															}
														}
													});
											paymentDetailEditGrid.getView()
													.refresh();
											newPaymentEditRowFlag = 0;

										},
										failure : function(form, action) {
											Ext.Msg.alert(
													action.result.messageTitle,
													action.result.message);
										}
									});
								}
							});
						}

					}
				}, '-', new Ext.ux.UploadButton({
							disabled : false,
							reuses : false,
							attachmentTypeID : com.bitc.xmis.Attachment.payment,
							// REFID : contractID,
							REFIDFun : function() {
								return contractIdInEdit;
							},
							uploadPanelConfig : {
								listeners : {
									'uploadcomplete' : function() {
										downloadGridInPayEd.reload();
									}
								}
							}
						}), '-', {
					xtype : 'button',
					text : '导出',
					iconCls : 'xls',
					handler : function() {
						exp.onExport(paymentDetailEditGrid, '收款明细列表', false);
					}
				}, '->', "*双击进入编辑状态"]
		},
		colModel : new Ext.grid.ColumnModel({
			columns : [new Ext.grid.RowNumberer(), {
						header : "收款Id",
						dataIndex : paymentChargedId,
						sortable : true,
						hidden : true,
						align : 'center',
						editor : paymentEditIdField
					}, {
						header : "收款日期",
						dataIndex : paymentChargedDateDisplayed,
						sortable : true,
						align : 'center',
						xtype : 'datecolumn',
						format : 'Y-m-d',
						editor : paymentDateEditField
					}, {
						xtype : 'numbercolumn',
						format : '0,000.00',
						header : "收款金额",
						dataIndex : paymentChargedAmt,
						sortable : true,
						align : 'right',
						editor : paymentAmtEditField

					}, {
						header : "经办人Id",
						hidden : true,
						dataIndex : paymentTransactorID,
						align : 'center',
						editor : paymentEditAEmpIDField
					}, {
						header : "经办人",
						dataIndex : paymentAEmpName,
						sortable : true,
						renderer : convertUserName,
						editor : searchPaymentAEmpField
					}, {
						header : "备注",
						dataIndex : paymentChargedRemark,
						sortable : true,
						renderer : function(value, cellMeta, record, rowIndex,
								columnIndex, store) {
							if (value != null) {
								var baseNum = 18;
								var msg = '';
								for (var i = 0; i < value.length / baseNum; i++) {
									msg += value.substr(i * baseNum, baseNum)
											+ "<br/>";
								}
								cellMeta.attr = 'ext:qtitle="备注" ext:qtip="'
										+ msg + '"';
							}
							return value;
						},
						editor : paymentEditRemarkField
					}, {
						header : "登记日期",
						dataIndex : paymentUpdateDateDisplayed,
						sortable : true,
						xtype : 'datecolumn',
						format : 'Y-m-d',
						align : 'center',
						editor : paymentUpDateDEditField
					}, {
						header : "登记人",
						dataIndex : paymentBEmpName,
						sortable : true,
						editor : null
					}]
		})

	});
	window.paymentDetailEditGrid = paymentDetailEditGrid;
	paymentListEditForm = new Ext.form.FormPanel({
				region : 'center',
				baseCls : 'x-plain',
				anchor : '100%',
				layout : 'border',
				items : [paymentDetailEditGrid]
			});

	this.hidePaymentEditGridInJS = function() {
		paymentEditPanel.setVisible(false);
	};

	var downloadGridInPayEd = new com.bitc.xmis.AttachmentList({
				attachmentTypeID : com.bitc.xmis.Attachment.payment,
				REFID : contractID,
				region : 'east',
				width : 200,
				title : '附件信息',
				enableColumnMove : false, // True表示启动对Column顺序的拖拽,反之禁用
				stripeRows : true, // True表示使用不同颜色间隔行，默认为false
				loadMask : true,
				actionColumnHidden : true
			});

	window.downloadGridInPayEd = downloadGridInPayEd;
	var paymentEditPanel = new Ext.Panel({
				hidden : true,
				// height : Ext.get('paymentDetailEditGridDiv').getHeight(),
				height : 413,
				renderTo : "paymentDetailEditGridDiv",
				title : "收款明细列表",
				layout : 'border',
				items : [paymentListEditForm, downloadGridInPayEd]

			});
	showPaymentEditGrid(contractID, contractName, contractType, '', contractAmt);
//	if (readOnly) {
//		paymentDetailEditGrid.getTopToolbar().hide(true);
//	}
});
