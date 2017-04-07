Ext.onReady(function() {
	var selectRowNum = '';
	var invoiceTotalSum = 0.00;
	var newPaymentEditRowFlag = 0;
	
	//合同的操作类型，判断是否有‘双击编辑’权限     
	//var invoiceConOpType;
	//var receiptConOpType;
	
	var invoiceEditRecord = Ext.data.Record.create([{
				name : DBSchema.InvoiceDetail.ContractID
			}, {
				name : DBSchema.InvoiceDetail.InvoiceNo
			}, {
				name : DBSchema.InvoiceDetail.InvoiceCode
			}, {
				name : DBSchema.InvoiceDetail.InvoiceAmt,
				type : 'float'
			}, {
				name : DBSchema.InvoiceDetail.InvoiceDate
			}, {
				name : DBSchema.InvoiceDetail.InvoiceDateDisplayed,
				type : 'date'
			}, {
				name : DBSchema.InvoiceDetail.UpdateDateDisplayed,
				type : 'date'
			}, {
				name : DBSchema.InvoiceDetail.UpdateID
			}, {
				name : DBSchema.InvoiceDetail.UpdateDate
			}, {
				name : DBSchema.InvoiceDetail.TransactorID
			}, {
				name : DBSchema.InvoiceDetail.ScanAttachment
			}, {
				name : DBSchema.InvoiceDetail.Remark
			}, {
				name : DBSchema.InvoiceDetail.TotalAmtByID
			}, {
				name : DBSchema.Employee.aEmpName
			}, {
				name : DBSchema.Employee.bEmpName
			}, {
				name : DBSchema.InvoiceDetail.UnAccountAmtTotalSum
			}, {
				name : DBSchema.InvoiceDetail.ReceiverID
			}, {
				name : DBSchema.Employee.cEmpName
			}, {
				name : DBSchema.InvoiceDetail.MailCode
			}, {
				name : DBSchema.InvoiceDetail.paidAmount,
				type : 'number'
			}, {
				name : DBSchema.InvoiceDetail.unPaidAmount,
				type : 'number'
			}]);
	var invoiceEditStore = new Ext.data.Store({
				url : __cxtPath + '/contractMain.do?method=getInvoiceList',
				remoteSort : false,
				sortInfo : {
					field : DBSchema.InvoiceDetail.InvoiceDateDisplayed,
					direction : 'ASC'
				},
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount',
							fields : invoiceEditRecord
						}),
				listeners : {
					'load' : function(s, records) {
						var sum = 0;
						Ext.iterate(records, function(record) {
									sum += record
											.get(DBSchema.InvoiceDetail.InvoiceAmt);
								});
						invoiceTotalSum = sum;
						if (sum != 0) {
							invoicePanel.setTitle('发票登记 - 合计：'
									+ Ext.util.Format.number(sum, '0,0.00'));
						} else {
							invoicePanel.setTitle('发票登记');
						}
					}
//				,
//					'beforeload' : function(){
//						//判断合同的操作类型
//						if (typeof (contractOpType) === 'undefined') {
//							Ext.Ajax.request({
//								url : __cxtPath
//										+'/contractMain.do?method=getContractOpType',
//								params : {
//									contractID : contractIdInEdit_invoice
//								},
//								success : function(response,opt){
//									var obj = Ext.decode(response.responseText);
//									var type = obj.attributes.contractOpType;
//									if(type ==5){
//										invoiceDetailEditGrid.getTopToolbar().enable();
//										downloadGridInInvoEd.setActionColumnHidden(false);
//										invoiceConOpType = 1;
//									}
//								},
//								failure : function(response,opt){
//									var obj = Ext.decode(response.responseText);
//									Ext.Msg
//											.alert(obj.messageTitle,
//													obj.message);
//								}
//							});
//						}else{
//							if(contractOpType ==5){
//								invoiceDetailEditGrid.getTopToolbar().enable();
//								downloadGridInInvoEd.setActionColumnHidden(false);
//								invoiceConOpType = 1;
//							}
//						}
//					}
				}
			});
	var invoiceEditAEmpIDField = new Ext.form.TextField({});
	var searchInvoiceAEmpField = new Ext.ux.form.ChooseField({
				name : 'AEmp',
				anchor : '80%',
				winConfig : {
					width : 350,
					title : '经办人'
				},
		addSearchField:function(grid,win){
			win.splice(0,0,new Ext.ux.form.SearchField({
				width : 140,
				value:userName,
				onTrigger1Click : function() {
					this.el.dom.value = '';
					this.triggers[0].hide();
				},
				onTrigger2Click : function() {
					var inputName = this.el.dom.value;
					if (inputName != null && '' != inputName.trim()) {
						grid.getStore().findBy(function(record){
							if(record.get('user_name') == inputName){
								grid.getSelectionModel().clearSelections();
								grid.getSelectionModel().selectRecords([record]);
							}
						});
						this.triggers[0].show();
					} else {
						Ext.MessageBox.alert("提示", "请输入要查询员工的姓名后进行查询");
					}
				}
			}));
		},
				paramFun : function() {
					return {
						compID : companyIdInInvo_invoice
					};
				},
				chooseUrl : __cxtPath + '/resources/choose/emp.json.jsp',
				setValueFun : function(o) {
					invoiceEditAEmpIDField.setValue(o
							.get(DBSchema.EmployeeBelongToDept.EmployeeID));
					searchInvoiceAEmpField.setValue(o.get('user_name'));
				},
				clearValueFun : function() {
					invoiceEditAEmpIDField.setValue('');
					searchInvoiceAEmpField.setValue('');
				}

			});
	var invoiceEditIdField = new Ext.form.TextField({});
	var invoiceCodeEditField = new Ext.form.TextField({});
	var invoicePayedEditField = new Ext.form.NumberField({
				format : '0,000.00',
				readOnly : true
			});
	var invoiceUnpayEditField = new Ext.form.NumberField({
				format : '0,000.00',
				readOnly : true
			});
	var invoiceAmtEditField = new Ext.form.NumberField({//
		// minValue : 0,
		allowBlank : false,
		listeners : {
			'blur' : function() {
				if (this.getValue() == '0') {
					Ext.Msg.alert("提示", "金额不能为0！");
					invoiceAmtEditField.setValue('');
					return false;
				}
				if (this.getValue() < 0) {
					Ext.Msg.alert('提示', '当前输入的票据金额小于0!');
				}
				if (this.getValue() > 0) {
					var paymentTotal = this.getValue();
					if (selectRowNum == '') {
						for (var i = 0; i < invoiceEditStore.getCount(); i++) {
							var amtInGrid = invoiceEditStore.getAt(i)
									.get(DBSchema.InvoiceDetail.InvoiceAmt);
							if (amtInGrid != '' && amtInGrid != undefined) {
								paymentTotal = paymentTotal + amtInGrid * 1;
							}
						}
					} else {
						for (var i = 0; i < invoiceEditStore.getCount(); i++) {
							var amtInGrid = invoiceEditStore.getAt(i)
									.get(DBSchema.InvoiceDetail.InvoiceAmt);
							if (amtInGrid != '' && amtInGrid != undefined
									&& i != selectRowNum) {
								paymentTotal = paymentTotal + amtInGrid * 1;
							}
						}

					}
					if (paymentTotal > choosedAmt) {
						Ext.MessageBox.show({
									title : "提示",
									msg : "发票金额大于合同总额，是否继续？",
									buttons : Ext.Msg.YESNO,
									fn : function(btn) {
										if (btn == 'no') {
											invoiceAmtEditField.setValue('');
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
	var invoiceDateEditField = new Ext.form.DateField({
				format : 'Y-m-d',
				allowBlank : false
			});
	var invoiceUpDateDEditField = new Ext.form.DateField({
				format : 'Y-m-d',
				readOnly : true
			});
	var invoiceEditRemarkField = new Ext.form.TextField();
	receiverIDField = new Ext.form.TextField();
	mailCodeField = new Ext.form.TextField();
	receiverNameField = new Ext.ux.form.ChooseField({
				winConfig : {
					width : 350,
					title : '收件人设置'
				},
		addSearchField:function(grid,win){
			win.splice(0,0,new Ext.ux.form.SearchField({
				width : 140,
				value:userName,
				onTrigger1Click : function() {
					this.el.dom.value = '';
					this.triggers[0].hide();
				},
				onTrigger2Click : function() {
					var inputName = this.el.dom.value;
					if (inputName != null && '' != inputName.trim()) {
						grid.getStore().findBy(function(record){
							if(record.get('user_name') == inputName){
								grid.getSelectionModel().clearSelections();
								grid.getSelectionModel().selectRecords([record]);
							}
						});
						this.triggers[0].show();
					} else {
						Ext.MessageBox.alert("提示", "请输入要查询员工的姓名后进行查询");
					}
				}
			}));
		},
				paramFun : function() {
					return {
						compID : aCmpId_invoice
					};
				},
				chooseUrl : __cxtPath + '/resources/choose/emp.json.jsp',
				setValueFun : function(o) {
					receiverNameField.setValue(o.get('user_name'));
					receiverIDField.setValue(o
							.get(DBSchema.EmployeeBelongToDept.EmployeeID));
				},
				clearValueFun : function() {
					receiverNameField.setValue('');
					receiverIDField.setValue('');
				}
			});

	var invoiceEditGridEditor = new Ext.ux.grid.RowEditor({
				saveText : '确认',
				cancelText : '取消',
				clicksToEdit : 2
			});
	invoiceEditGridEditor.on({
		scope : this,
		beforeedit : function() {
			invoiceDetailEditGrid.removeBtn.setDisabled(true);
			invoiceDetailEditGrid.newBtn.setDisabled(true);
			if(contractOpType != 5){
				return false;
			}
		},
		afteredit : function(roweditor, changes, record, rowIndex) {
			Ext.Ajax.request({
				url : __cxtPath + '/contractMain.do?method=saveInvoiceDetail',
				method : 'post',
				params : {
					type : '1',
					contractID : contractIdInEdit_invoice,
					invoiceID : invoiceEditIdField.getValue(),
					invoiceCode : invoiceCodeEditField.getValue(),
					contractType : contractTypeInEdit_invoice,
					invoiceDetailDate : invoiceDateEditField.getValue(),
					invoiceDetailAmt : invoiceAmtEditField.getValue(),
					invoiceRemark : invoiceEditRemarkField.getValue(),
					transavtorID : invoiceEditAEmpIDField.getValue(),
					receiverID : receiverIDField.getValue(),
					mailCode : mailCodeField.getValue()
				},
				success : function(response) {
					var action = Ext.decode(response.responseText);
					Ext.Msg.alert(action.messageTitle, action.message);
					if (action.success) {
						invoiceDetailEditGrid.getStore().reload({
							callback : function(record, options, success) {
								var invioicedSum = formatCurrency(null,
										invoiceTotalSum + '',
										Color_Font_Contract_Total_Invoiced_Amt);
								invoicedAmtTemp = invoiceTotalSum;
								var invoicedUnAmt = (invoiceTotalSum - earnedAmtTemp) <= 0
										? 0
										: invoiceTotalSum - earnedAmtTemp;
								var invoicedUnAmtStr = formatCurrency(null,
										invoicedUnAmt + '',
										Color_Font_Contract_Total_UnAccount_Amt);
								Ext.get('invoiced').update(invioicedSum);
								Ext.get('invoicedUnAmt').update('已开发票未到账金额：'
										+ invoicedUnAmtStr);

							}
						});
					} else {
						var s = invoiceDetailEditGrid.getSelectionModel()
								.getSelections();
						for (var i = 0, r; r = s[i]; i++) {
							invoiceEditStore.remove(r);
						}
					}
				},
				failure : function(response) {
				}
			});
			newPaymentEditRowFlag = 0;
			invoiceDetailEditGrid.removeBtn.setDisabled(false);
			invoiceDetailEditGrid.newBtn.setDisabled(false);
			newPaymentEditRowFlag = 0;
			selectRowNum = '';
		},
		canceledit : function() {
			if (newPaymentEditRowFlag == 1) {
				invoiceEditGridEditor.stopEditing(false);
				var s = invoiceDetailEditGrid.getSelectionModel()
						.getSelections();
				for (var i = 0, r; r = s[i]; i++) {
					invoiceEditStore.remove(r);
				};
				newPaymentEditRowFlag = 0;
			}

			invoiceDetailEditGrid.getView().refresh();
			invoiceDetailEditGrid.removeBtn.setDisabled(false);
			invoiceDetailEditGrid.newBtn.setDisabled(false);
			selectRowNum = '';
		}
	});

	var incomeCol = new Ext.grid.ColumnModel({
				columns : [new Ext.grid.RowNumberer(), {
							header : "票据Id",
							dataIndex : DBSchema.InvoiceDetail.InvoiceNo,
							sortable : true,
							hidden : true,
							align : 'center',
							editor : invoiceEditIdField
						}, {
							header : "开票日期",
							dataIndex : DBSchema.InvoiceDetail.InvoiceDateDisplayed,
							sortable : true,
							align : 'center',
							xtype : 'datecolumn',
							format : 'Y-m-d',
							editor : invoiceDateEditField
						}, {
							header : "票据号",
							dataIndex : DBSchema.InvoiceDetail.InvoiceCode,
							sortable : true,
							align : 'center',
							editor : invoiceCodeEditField
						}, {
							xtype : 'numbercolumn',
							format : '0,000.00',
							header : "票据金额",
							dataIndex : DBSchema.InvoiceDetail.InvoiceAmt,
							sortable : true,
							align : 'right',
							editor : invoiceAmtEditField

						}, {
							xtype : 'numbercolumn',
							format : '0,000.00',
							header : "已付金额",
							dataIndex : DBSchema.InvoiceDetail.paidAmount,
							sortable : true,
							align : 'right',
							editor : invoicePayedEditField
						}, {
							xtype : 'numbercolumn',
							format : '0,000.00',
							header : "未付金额",
							dataIndex : DBSchema.InvoiceDetail.unPaidAmount,
							sortable : true,
							align : 'right',
							editor : invoiceUnpayEditField
						}, {
							header : "经办人Id",
							hidden : true,
							dataIndex : DBSchema.InvoiceDetail.TransactorID,
							align : 'center',
							editor : invoiceEditAEmpIDField
						}, {
							header : "经办人",
							dataIndex : DBSchema.Employee.aEmpName,
							sortable : true,
							renderer : convertUserName,
							editor : searchInvoiceAEmpField
						}, {
							header : "备注",
							dataIndex : DBSchema.InvoiceDetail.Remark,
							sortable : true,
							renderer : function(value, cellMeta, record,
									rowIndex, columnIndex, store) {
								if (value != null) {
									var baseNum = 18;
									var msg = '';
									for (var i = 0; i < value.length / baseNum; i++) {
										msg += value.substr(i * baseNum,
												baseNum)
												+ "<br/>";
									}
									cellMeta.attr = 'ext:qtitle="备注" ext:qtip="'
											+ msg + '"';
								}
								return value;
							},
							editor : invoiceEditRemarkField
						}, {
							header : "登记日期",
							dataIndex : DBSchema.InvoiceDetail.UpdateDateDisplayed,
							sortable : true,
							xtype : 'datecolumn',
							format : 'Y-m-d',
							align : 'center',
							editor : invoiceUpDateDEditField
						}, {
							header : "登记人",
							dataIndex : DBSchema.Employee.bEmpName,
							sortable : true,
							editor : null
						}, {
							header : "收件人ID",
							dataIndex : DBSchema.InvoiceDetail.ReceiverID,
							hidden : true,
							sortable : false,
							editor : receiverIDField
						}, {
							header : "收件人",
							dataIndex : DBSchema.Employee.cEmpName,
							sortable : false,
							editor : receiverNameField
						}, {
							header : "邮件编号",
							dataIndex : DBSchema.InvoiceDetail.MailCode,
							sortable : false,
							editor : mailCodeField
						}]
			});

	var outCol = new Ext.grid.ColumnModel({
				columns : [new Ext.grid.RowNumberer(), {
							header : "票据Id",
							dataIndex : DBSchema.InvoiceDetail.InvoiceNo,
							sortable : true,
							hidden : true,
							align : 'center',
							editor : invoiceEditIdField
						}, {
							header : "开票日期",
							dataIndex : DBSchema.InvoiceDetail.InvoiceDateDisplayed,
							sortable : true,
							align : 'center',
							xtype : 'datecolumn',
							format : 'Y-m-d',
							editor : invoiceDateEditField
						}, {
							header : "票据号",
							dataIndex : DBSchema.InvoiceDetail.InvoiceCode,
							sortable : true,
							align : 'center',
							editor : invoiceCodeEditField
						}, {
							xtype : 'numbercolumn',
							format : '0,000.00',
							header : "票据金额",
							dataIndex : DBSchema.InvoiceDetail.InvoiceAmt,
							sortable : true,
							align : 'right',
							editor : invoiceAmtEditField

						}, {
							header : "经办人Id",
							hidden : true,
							dataIndex : DBSchema.InvoiceDetail.TransactorID,
							align : 'center',
							editor : invoiceEditAEmpIDField
						}, {
							header : "经办人",
							dataIndex : DBSchema.Employee.aEmpName,
							sortable : true,
							renderer : convertUserName,
							editor : searchInvoiceAEmpField
						}, {
							header : "备注",
							dataIndex : DBSchema.InvoiceDetail.Remark,
							sortable : true,
							renderer : function(value, cellMeta, record,
									rowIndex, columnIndex, store) {
								if (value != null) {
									var baseNum = 18;
									var msg = '';
									for (var i = 0; i < value.length / baseNum; i++) {
										msg += value.substr(i * baseNum,
												baseNum)
												+ "<br/>";
									}
									cellMeta.attr = 'ext:qtitle="备注" ext:qtip="'
											+ msg + '"';
								}
								return value;
							},
							editor : invoiceEditRemarkField
						}, {
							header : "登记日期",
							dataIndex : DBSchema.InvoiceDetail.UpdateDateDisplayed,
							sortable : true,
							xtype : 'datecolumn',
							format : 'Y-m-d',
							align : 'center',
							editor : invoiceUpDateDEditField
						}, {
							header : "登记人",
							dataIndex : DBSchema.Employee.bEmpName,
							sortable : true,
							editor : null
						}, {
							header : "收件人ID",
							dataIndex : DBSchema.InvoiceDetail.ReceiverID,
							hidden : true,
							sortable : false,
							editor : receiverIDField
						}, {
							header : "收件人",
							dataIndex : DBSchema.Employee.cEmpName,
							sortable : false,
							editor : receiverNameField
						}, {
							header : "邮件编号",
							dataIndex : DBSchema.InvoiceDetail.MailCode,
							sortable : false,
							editor : mailCodeField
						}]
			});

	var exp = new Ext.ux.Export({
				formater : [{
							columnHeader : '',
							formatClass : 'com.bitc.jk.xmis.export.RowNumber'
						}, {
							columnHeader : '开票日期',
							formatClass : 'com.bitc.jk.xmis.export.DateFormat'
						}, {
							columnHeader : '登记日期',
							formatClass : 'com.bitc.jk.xmis.export.DateFormat'
						}]
			});

	var invoiceDetailEditGrid = new Ext.grid.GridPanel({
		store : invoiceEditStore,
		stripeRows : true, // True表示使用不同颜色间隔行，默认为false
		region : 'center',
		autoHeight : false,
		autoWidth : false,
		listeners : {
			'rowdblclick' : function(gr, rowIndex, e) {
				selectRowNum = rowIndex;
			}
		},
		loadMask : true, // 读取数据时的遮罩和提示功能，默认为false
		plugins : !readOnly ? [invoiceEditGridEditor] : [],
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		tbar : {
			xtype : 'toolbar',
			disabled : contractOpType == 5 ? false : true,
			items : [{
				ref : '../newBtn',
				iconCls : 'add',
				text : '新建',
				handler : function() {
					var e = new invoiceEditRecord({
								invoiceContractID : '',
								InvoiceNo : '',
								InvoiceAmt : '',
								InvoiceDate : '',
								InvoiceDateDisplayed : new Date()
										.format('Y-m-d'),
								TotalAmtByID : '',// add 1
								invoiceRemark : '',
								ScanAttachment : '0',
								TransactorID : '',
								InvoiceUpdateDate : '',
								UpdateID : '',
								UpdateDateDisplayed : '',// add 2
								invoiceAEmpName : '',
								invoiceBEmpName : '',
								UnAccountAmtTotalSum : ''
							});
					invoiceEditGridEditor.stopEditing();
					invoiceEditStore.insert(0, e);
					invoiceDetailEditGrid.getView().refresh();
					invoiceDetailEditGrid.getSelectionModel().selectRow(0);
					invoiceEditGridEditor.startEditing(0);
					newPaymentEditRowFlag = 1;
				}
			}, '-', {
				ref : '../removeBtn',
				iconCls : 'remove',
				text : '删除',
				disabled : false,
				handler : function() {
					var record = invoiceDetailEditGrid.getSelectionModel()
							.getSelected();
					if (record == 'undefined' || record == undefined) {
						Ext.Msg.alert("未选择", "请先选择！");
						return true;
					}
					Ext.Msg.confirm("请确认", "确定要删除该条发票记录吗？", function(btn) {
						if (btn == 'yes') {
							Ext.Ajax.request({
								url : __cxtPath
										+ '/contractMain.do?method=delInvoiceDetail',
								method : 'post',
								params : {
									type : '1',
									contractID : contractIdInEdit_invoice,
									invoiceID : record
											.get(DBSchema.InvoiceDetail.InvoiceNo)
								},
								success : function(response) {
									var action = Ext
											.decode(response.responseText);
									Ext.Msg.alert(action.messageTitle,
											action.message);
									if (action.success) {
										invoiceDetailEditGrid.getStore()
												.reload({
													callback : function(
															record,
															options,
															success) {
														var invioicedSum = formatCurrency(
																null,
																invoiceTotalSum
																		+ '',
																Color_Font_Contract_Total_Invoiced_Amt);
														invoicedAmtTemp = invoiceTotalSum;
														if (invoiceTotalSum == 0) {
															invioicedSum = '';
														}
														invoicedAmtTemp = invoiceTotalSum;
														var invoicedUnAmt = (invoiceTotalSum - earnedAmtTemp) <= 0
																? 0
																: invoiceTotalSum
																		- earnedAmtTemp;
														var invoicedUnAmtStr = formatCurrency(
																null,
																invoicedUnAmt
																		+ '',
																Color_Font_Contract_Total_UnAccount_Amt);
														Ext
																.get('invoiced')
																.update(invioicedSum);
														Ext
																.get('invoicedUnAmt')
																.update('已开发票未到账金额：'
																		+ invoicedUnAmtStr);
													}
												});
										invoiceDetailEditGrid.getView()
												.refresh();
										newPaymentEditRowFlag = 0;
									}
								},
								failure : function(form, action) {
								}
							});
						}
					});
				}
			}, '-', new Ext.ux.UploadButton({
						disabled : false,
						reuses : false,
						attachmentTypeID : com.bitc.xmis.Attachment.invoice,
						// REFID : contractIdInEdit_invoice,
						REFIDFun : function() {
							return contractIdInEdit_invoice;
						},
						uploadPanelConfig : {
							listeners : {
								'uploadcomplete' : function() {
									downloadGridInInvoEd.reload();
								}
							}
						}
					}), '-', {
				ref : '../newSplitBtn',
				iconCls : 'add',
				text : '发票执行预估',
				id : 'splitInfo',
				handler : function() {
					var data = invoiceDetailEditGrid.getSelectionModel()
							.getSelected();
					if (undefined == data || 'undefined' == data) {
						Ext.Msg.alert('提示', '还没选择发票！');
					} else {
						showInvoiceSplit(invoiceDetailEditGrid
								.getSelectionModel().getSelected());
					}
				}
			}, '-', {
				xtype : 'button',
				text : '导出',
				iconCls : 'xls',
				handler : function() {
					exp.onExport(invoiceDetailEditGrid, '发票登记明细列表', false);
				}
			}, '->', "*双击进入编辑状态"]
		} ,
		colModel : contractTypeInEdit_invoice == 1 ? incomeCol : outCol
	});

	window.invoiceDetailEditGrid = invoiceDetailEditGrid;

	var downloadGridInInvoEd = new com.bitc.xmis.AttachmentList({
				attachmentTypeID : com.bitc.xmis.Attachment.invoice,
				REFID : contractIdInEdit_invoice,
				region : 'east',
				title : '附件信息',
				layout : 'fit',
				stripeRows : true, // True表示使用不同颜色间隔行，默认为false
				autoExpandColumn : 0,
				width : 200,
				loadMask : true,
				enableColumnMove : false,
				enableColumnResize : false,
				actionColumnHidden : contractOpType == 5 ? false : true
			});

	window.downloadGridInInvoEd = downloadGridInInvoEd;

	var detailTplMsg = '<table><tr name="invoiceTR"><td align=center width=100>{anticipatedTime}</td><td align=right width=100>{splitSum}</td><td align=center width=100>{payFlag:this.trans}</td><td width=150>{remark}</td></tr></table>';
	var detailTpl = new Ext.XTemplate(detailTplMsg);

	detailTpl.trans = function(v) {
		if (v == 'true') {
			return '<img width=12 height=12 src="' + path
					+ '/resources/images/split-true.gif"/>';
		} else {
			return '';
		}
	};

	var invoiceSplitDetailPanel = new Ext.Panel({
		region : 'center',
		autoHeight : true,
		border : false,
		id : 'haha',
		width : 500,
		bodyStyle : {
			background : '#f5f5f5',
			padding : '7px'
		},
		html : '<div><font color="#15428b"><b>发票执行情况：</b></font></div><div id="in1"><table><tr><td>您还没有选择查看的发票，请选择！</td></tr><tr><td height=48></td></tr></table><div>'
	});

	var southPanel = new Ext.Panel({
				region : 'south',
				layout : 'border',
				height : 100,
				width : 200,
				autoScroll : true,
				items : [invoiceSplitDetailPanel]
			});

	var invoiceCenterPanel = new Ext.Panel({
				layout : 'border',
				region : 'center',
				items : [invoiceDetailEditGrid, southPanel]
			});

	var invoicePanel = new Ext.Panel({
				title : '发票登记',
				layout : 'border',
				items : [invoiceCenterPanel, downloadGridInInvoEd]
			});

	// 收据
	var receiptselectRowNum = '';
	var receiptnewPaymentEditRowFlag = 0;
	var receiptEditRecord = Ext.data.Record.create([{
				name : DBSchema.InvoiceDetail.ContractID
			}, {
				name : DBSchema.InvoiceDetail.InvoiceNo
			}, {
				name : DBSchema.InvoiceDetail.InvoiceCode
			}, {
				name : DBSchema.InvoiceDetail.InvoiceAmt,
				type : 'float'
			}, {
				name : DBSchema.InvoiceDetail.InvoiceDate
			}, {
				name : DBSchema.InvoiceDetail.InvoiceDateDisplayed,
				type : 'date'
			}, {
				name : DBSchema.InvoiceDetail.UpdateDateDisplayed,
				type : 'date'
			}, {
				name : DBSchema.InvoiceDetail.UpdateID
			}, {
				name : DBSchema.InvoiceDetail.UpdateDate
			}, {
				name : DBSchema.InvoiceDetail.TransactorID
			}, {
				name : DBSchema.InvoiceDetail.ScanAttachment
			}, {
				name : DBSchema.InvoiceDetail.Remark
			}, {
				name : DBSchema.InvoiceDetail.TotalAmtByID
			}, {
				name : DBSchema.Employee.aEmpName
			}, {
				name : DBSchema.Employee.bEmpName
			}, {
				name : DBSchema.InvoiceDetail.UnAccountAmtTotalSum
			}, {
				name : DBSchema.InvoiceDetail.ReceiverID
			}, {
				name : DBSchema.Employee.cEmpName
			}, {
				name : DBSchema.InvoiceDetail.MailCode
			}]);
	var receiptEditStore = new Ext.data.Store({
				url : __cxtPath + '/contractMain.do?method=getInvoiceList',
				remoteSort : false,
				sortInfo : {
					field : DBSchema.InvoiceDetail.InvoiceDateDisplayed,
					direction : 'ASC'
				},
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount',
							fields : receiptEditRecord
						}),
				listeners : {
					'load' : function(s, records) {
						var sum = 0;
						Ext.iterate(records, function(record) {
									sum += record
											.get(DBSchema.InvoiceDetail.InvoiceAmt);
								});
						if (sum != 0) {
							receiptPanel.setTitle('收据登记 - 合计：'
									+ Ext.util.Format.number(sum, '0,0.00'));
						} else {
							receiptPanel.setTitle('收据登记');
						}
					}
//				,
//					'beforeload' : function(){
//						//判断合同的操作类型
//						if (typeof (contractOpType) === 'undefined') {
//							Ext.Ajax.request({
//								url : __cxtPath
//										+'/contractMain.do?method=getContractOpType',
//								params : {
//									contractID : contractIdInEdit_invoice
//								},
//								success : function(response,opt){
//									var obj = Ext.decode(response.responseText);
//									var type = obj.attributes.contractOpType;
//									if(type ==5){
//										receiptDetailEditGrid.getTopToolbar().enable();
//										receiptdownloadGridInInvoEd.setActionColumnHidden(false);
//										receiptConOpType = 1;
//									}
//								},
//								failure : function(response,opt){
//									var obj = Ext.decode(response.responseText);
//									Ext.Msg
//											.alert(obj.messageTitle,
//													obj.message);
//								}
//							});
//						}else{
//							if(contractOpType ==5){
//								receiptDetailEditGrid.getTopToolbar().enable();
//								receiptdownloadGridInInvoEd.setActionColumnHidden(false);
//								receiptConOpType = 1;
//							}
//						}
//					}
				}
			});
	var receiptEditAEmpIDField = new Ext.form.TextField({});
	var receiptsearchInvoiceAEmpField = new Ext.ux.form.ChooseField({
				name : 'AEmp',
				anchor : '80%',
				winConfig : {
					width : 350,
					title : '经办人'
				},
		addSearchField:function(grid,win){
			win.splice(0,0,new Ext.ux.form.SearchField({
				width : 140,
				value:userName,
				onTrigger1Click : function() {
					this.el.dom.value = '';
					this.triggers[0].hide();
				},
				onTrigger2Click : function() {
					var inputName = this.el.dom.value;
					if (inputName != null && '' != inputName.trim()) {
						grid.getStore().findBy(function(record){
							if(record.get('user_name') == inputName){
								grid.getSelectionModel().clearSelections();
								grid.getSelectionModel().selectRecords([record]);
							}
						});
						this.triggers[0].show();
					} else {
						Ext.MessageBox.alert("提示", "请输入要查询员工的姓名后进行查询");
					}
				}
			}));
		},
				paramFun : function() {
					return {
						compID : companyIdInInvo_invoice
					};
				},
				chooseUrl : __cxtPath + '/resources/choose/emp.json.jsp',
				setValueFun : function(o) {
					receiptEditAEmpIDField.setValue(o
							.get(DBSchema.EmployeeBelongToDept.EmployeeID));
					receiptsearchInvoiceAEmpField.setValue(o.get('user_name'));
				},
				clearValueFun : function() {
					receiptEditAEmpIDField.setValue('');
					receiptsearchInvoiceAEmpField.setValue('');
				}

			});
	var receiptEditIdField = new Ext.form.TextField({});
	var receiptCodeEditField = new Ext.form.TextField({});
	var receiptAmtEditField = new Ext.form.NumberField({//
		minValue : 0,
		allowBlank : false,
		listeners : {
			'blur' : function() {
				if (this.getValue() == '0') {
					Ext.Msg.alert("提示", "金额不能为0！");
					receiptAmtEditField.setValue('');
					return false;
				}
				if (this.getValue() > 0) {
					var paymentTotal = this.getValue();
					if (receiptselectRowNum == '') {
						for (var i = 0; i < receiptEditStore.getCount(); i++) {
							var amtInGrid = receiptEditStore.getAt(i)
									.get(DBSchema.InvoiceDetail.InvoiceAmt);
							if (amtInGrid != '' && amtInGrid != undefined) {
								paymentTotal = paymentTotal + amtInGrid * 1;
							}
						}
					} else {
						for (var i = 0; i < receiptEditStore.getCount(); i++) {
							var amtInGrid = receiptEditStore.getAt(i)
									.get(DBSchema.InvoiceDetail.InvoiceAmt);
							if (amtInGrid != '' && amtInGrid != undefined
									&& i != receiptselectRowNum) {
								paymentTotal = paymentTotal + amtInGrid * 1;
							}
						}

					}
					if (paymentTotal > choosedAmt) {
						Ext.MessageBox.show({
									title : "提示",
									msg : "发票金额大于合同总额，是否继续？",
									buttons : Ext.Msg.YESNO,
									fn : function(btn) {
										if (btn == 'no') {
											receiptAmtEditField.setValue('');
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
	var receiptDateEditField = new Ext.form.DateField({
				format : 'Y-m-d',
				allowBlank : false
			});
	var receiptUpDateDEditField = new Ext.form.DateField({
				format : 'Y-m-d',
				readOnly : true
			});
	var receiptEditRemarkField = new Ext.form.TextField();
	receiptreceiverIDField = new Ext.form.TextField();
	receiptmailCodeField = new Ext.form.TextField();
	receiptreceiverNameField = new Ext.ux.form.ChooseField({
				winConfig : {
					width : 350,
					title : '收件人设置'
				},
		addSearchField:function(grid,win){
			win.splice(0,0,new Ext.ux.form.SearchField({
				width : 140,
				value:userName,
				onTrigger1Click : function() {
					this.el.dom.value = '';
					this.triggers[0].hide();
				},
				onTrigger2Click : function() {
					var inputName = this.el.dom.value;
					if (inputName != null && '' != inputName.trim()) {
						grid.getStore().findBy(function(record){
							if(record.get('user_name') == inputName){
								grid.getSelectionModel().clearSelections();
								grid.getSelectionModel().selectRecords([record]);
							}
						});
						this.triggers[0].show();
					} else {
						Ext.MessageBox.alert("提示", "请输入要查询员工的姓名后进行查询");
					}
				}
			}));
		},
				paramFun : function() {
					return {
						compID : aCmpId_invoice
					};
				},
				chooseUrl : __cxtPath + '/resources/choose/emp.json.jsp',
				setValueFun : function(o) {
					receiptreceiverNameField.setValue(o.get('user_name'));
					receiptreceiverIDField.setValue(o
							.get(DBSchema.EmployeeBelongToDept.EmployeeID));
				},
				clearValueFun : function() {
					receiptreceiverNameField.setValue('');
					receiptreceiverIDField.setValue('');
				}
			});

	var receiptEditGridEditor = new Ext.ux.grid.RowEditor({
				saveText : '确认',
				cancelText : '取消',
				clicksToEdit : 2
			});
	receiptEditGridEditor.on({
		scope : this,
		beforeedit : function() {
			receiptDetailEditGrid.removeBtn.setDisabled(true);
			receiptDetailEditGrid.newBtn.setDisabled(true);
			if(contractOpType != 5){
				return false;
			}
		},
		afteredit : function(roweditor, changes, record, rowIndex) {
			Ext.Ajax.request({
						url : __cxtPath
								+ '/contractMain.do?method=saveInvoiceDetail',
						method : 'post',
						params : {
							type : '2',
							contractID : contractIdInEdit_invoice,
							invoiceID : receiptEditIdField.getValue(),
							invoiceCode : receiptCodeEditField.getValue(),
							contractType : contractTypeInEdit_invoice,
							invoiceDetailDate : receiptDateEditField.getValue(),
							invoiceDetailAmt : receiptAmtEditField.getValue(),
							invoiceRemark : receiptEditRemarkField.getValue(),
							transavtorID : receiptEditAEmpIDField.getValue(),
							receiverID : receiptreceiverIDField.getValue(),
							mailCode : receiptmailCodeField.getValue()
						},
						success : function(response) {
							var action = Ext.decode(response.responseText);
							Ext.Msg.alert(action.messageTitle, action.message);
							if (action.success) {
								receiptDetailEditGrid.getStore().reload();
							} else {
								var s = receiptDetailEditGrid
										.getSelectionModel().getSelections();
								for (var i = 0, r; r = s[i]; i++) {
									receiptEditStore.remove(r);
								}
							}
						},
						failure : function(response) {
						}
					});
			receiptnewPaymentEditRowFlag = 0;
			receiptDetailEditGrid.getStore().reload();

			receiptDetailEditGrid.removeBtn.setDisabled(false);
			receiptDetailEditGrid.newBtn.setDisabled(false);

			receiptnewPaymentEditRowFlag = 0;
			receiptselectRowNum = '';
		},
		canceledit : function() {
			if (receiptnewPaymentEditRowFlag == 1) {
				receiptEditGridEditor.stopEditing(false);
				var s = receiptDetailEditGrid.getSelectionModel()
						.getSelections();
				for (var i = 0, r; r = s[i]; i++) {
					receiptEditStore.remove(r);
				};
				receiptnewPaymentEditRowFlag = 0;
			}

			receiptDetailEditGrid.getView().refresh();
			receiptDetailEditGrid.removeBtn.setDisabled(false);
			receiptDetailEditGrid.newBtn.setDisabled(false);
			receiptselectRowNum = '';
		}
	});

	var exp = new Ext.ux.Export({
				formater : [{
							columnHeader : '',
							formatClass : 'com.bitc.jk.xmis.export.RowNumber'
						}, {
							columnHeader : '开票日期',
							formatClass : 'com.bitc.jk.xmis.export.DateFormat'
						}, {
							columnHeader : '登记日期',
							formatClass : 'com.bitc.jk.xmis.export.DateFormat'
						}]
			});

	var receiptDetailEditGrid = new Ext.grid.GridPanel({
		store : receiptEditStore,
		stripeRows : true, // True表示使用不同颜色间隔行，默认为false
		region : 'center',
		autoHeight : false,
		autoWidth : false,
		listeners : {
			'rowdblclick' : function(gr, rowIndex, e) {
				receiptselectRowNum = rowIndex;
			}
		},
		loadMask : true, // 读取数据时的遮罩和提示功能，默认为false
		plugins : !readOnly ? [receiptEditGridEditor] : [],
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		tbar : {
			xtype : 'toolbar',
			disabled : contractOpType == 5 ? false : true,
			items : [{
				ref : '../newBtn',
				iconCls : 'add',
				text : '新建',
				handler : function() {
					var e = new receiptEditRecord({
								invoiceContractID : '',
								InvoiceNo : '',
								InvoiceAmt : '',
								InvoiceDate : '',
								InvoiceDateDisplayed : new Date()
										.format('Y-m-d'),
								TotalAmtByID : '',// add 1
								invoiceRemark : '',
								ScanAttachment : '0',
								TransactorID : '',
								InvoiceUpdateDate : '',
								UpdateID : '',
								UpdateDateDisplayed : '',// add 2
								invoiceAEmpName : '',
								invoiceBEmpName : '',
								UnAccountAmtTotalSum : ''
							});
					receiptEditGridEditor.stopEditing();
					receiptEditStore.insert(0, e);
					receiptDetailEditGrid.getView().refresh();
					receiptDetailEditGrid.getSelectionModel().selectRow(0);
					receiptEditGridEditor.startEditing(0);
					receiptnewPaymentEditRowFlag = 1;
				}
			}, '-', {
				ref : '../removeBtn',
				iconCls : 'remove',
				text : '删除',
				disabled : false,
				handler : function() {
					var record = receiptDetailEditGrid.getSelectionModel()
							.getSelected();
					if (record == 'undefined' || record == undefined) {
						Ext.Msg.alert("未选择", "请先选择！");
						return true;
					}
					Ext.Msg.confirm("请确认", "确定要删除该条收据记录吗？", function(btn) {
						if (btn == 'yes') {
							Ext.Ajax.request({
								url : __cxtPath
										+ '/contractMain.do?method=delInvoiceDetail',
								method : 'post',
								params : {
									type : '2',
									contractID : contractIdInEdit_invoice,
									invoiceID : record
											.get(DBSchema.InvoiceDetail.InvoiceNo)
								},
								success : function(response) {
									var action = Ext
											.decode(response.responseText);
									Ext.Msg.alert(action.messageTitle,
											action.message);
									if (action.success) {
										receiptDetailEditGrid.getStore()
												.reload();
										receiptDetailEditGrid.getView()
												.refresh();
										receiptnewPaymentEditRowFlag = 0;
									}
								},
								failure : function(form, action) {
								}
							});
						}
					});
				}
			}, '-',

			new Ext.ux.UploadButton({
						disabled : false,
						reuses : false,
						attachmentTypeID : com.bitc.xmis.Attachment.receipt,
						// REFID : contractIdInEdit_invoice,
						REFIDFun : function() {
							return contractIdInEdit_invoice;
						},
						uploadPanelConfig : {
							listeners : {
								'uploadcomplete' : function() {
									receiptdownloadGridInInvoEd.reload();
								}
							}
						}
					}), '-', {
				xtype : 'button',
				text : '导出',
				iconCls : 'xls',
				handler : function() {
					exp.onExport(receiptDetailEditGrid, '收据登记明细列表', false);
				}
			}, '->', "*双击进入编辑状态"]
		},
		colModel : new Ext.grid.ColumnModel({
			columns : [new Ext.grid.RowNumberer(), {
						header : "票据Id",
						dataIndex : DBSchema.InvoiceDetail.InvoiceNo,
						sortable : true,
						hidden : true,
						align : 'center',
						editor : receiptEditIdField
					}, {
						header : "开票日期",
						dataIndex : DBSchema.InvoiceDetail.InvoiceDateDisplayed,
						sortable : true,
						align : 'center',
						xtype : 'datecolumn',
						format : 'Y-m-d',
						editor : receiptDateEditField
					}, {
						header : "票据号",
						dataIndex : DBSchema.InvoiceDetail.InvoiceCode,
						sortable : true,
						align : 'center',
						editor : receiptCodeEditField
					}, {
						xtype : 'numbercolumn',
						format : '0,000.00',
						header : "票据金额",
						dataIndex : DBSchema.InvoiceDetail.InvoiceAmt,
						sortable : true,
						align : 'right',
						editor : receiptAmtEditField

					}, {
						header : "经办人Id",
						hidden : true,
						dataIndex : DBSchema.InvoiceDetail.TransactorID,
						align : 'center',
						editor : receiptEditAEmpIDField
					}, {
						header : "经办人",
						dataIndex : DBSchema.Employee.aEmpName,
						sortable : true,
						renderer : convertUserName,
						editor : receiptsearchInvoiceAEmpField
					}, {
						header : "备注",
						dataIndex : DBSchema.InvoiceDetail.Remark,
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
						editor : receiptEditRemarkField
					}, {
						header : "登记日期",
						dataIndex : DBSchema.InvoiceDetail.UpdateDateDisplayed,
						sortable : true,
						xtype : 'datecolumn',
						format : 'Y-m-d',
						align : 'center',
						editor : receiptUpDateDEditField
					}, {
						header : "登记人",
						dataIndex : DBSchema.Employee.bEmpName,
						sortable : true,
						editor : null
					}, {
						header : "收件人ID",
						dataIndex : DBSchema.InvoiceDetail.ReceiverID,
						hidden : true,
						sortable : false,
						editor : receiptreceiverIDField
					}, {
						header : "收件人",
						dataIndex : DBSchema.Employee.cEmpName,
						sortable : false,
						editor : receiptreceiverNameField
					}, {
						header : "邮件编号",
						dataIndex : DBSchema.InvoiceDetail.MailCode,
						sortable : false,
						editor : receiptmailCodeField
					}]
		})
	});
	window.receiptDetailEditGrid = receiptDetailEditGrid;

	var receiptdownloadGridInInvoEd = new com.bitc.xmis.AttachmentList({
				attachmentTypeID : com.bitc.xmis.Attachment.receipt,
				REFID : contractIdInEdit_invoice,
				region : 'east',
				title : '附件信息',
				layout : 'fit',
				stripeRows : true, // True表示使用不同颜色间隔行，默认为false
				autoExpandColumn : 0,
				width : 200,
				loadMask : true,
				actionColumnHidden : contractOpType == 5 ? false : true
			});

	window.receiptdownloadGridInInvoEd = receiptdownloadGridInInvoEd;
	var receiptPanel = new Ext.Panel({
				title : '收据登记',
				layout : 'border',
				items : [receiptDetailEditGrid, receiptdownloadGridInInvoEd]
			});
	//
	new Ext.TabPanel({
				renderTo : 'invoiceDetailEditGridDiv',
				height : 413,
				border : false,
				activeItem : 0,
				items : [invoicePanel, receiptPanel]
			});
	invoiceEditStore.load({
				params : {
					contractID : contractIdInEdit_invoice,
					type : '1'
				}
			});
	receiptEditStore.load({
				params : {
					contractID : contractIdInEdit_invoice,
					type : '2'
				}
			});

	invoiceDetailEditGrid.getSelectionModel().on('rowselect',
			function(sm, rowIdx, r) {
				Ext.Ajax.request({
					url : __cxtPath + '/contractMain.do',
					success : function(response, config) {
						var action = Ext.decode(response.responseText);
						var detailPanel = Ext.getCmp('haha');
						var detailPanel1 = Ext.getCmp('in1');
						var a = document.getElementById('in1');
						var pchildren = a.childNodes;
						var b = pchildren.length;
						for (var j = 0; j < b; j++) {
							a.removeChild(pchildren[0]);
						}
						if (action.rows.length > 0) {
							document.getElementById('in1').innerHTML = '<table><tr><th width=100>预计付款时间</th><th width=100 align=right>预计金额</th><th width=100>是否已付款</th><th width=150 align=left>备注</th></tr></table>';
							for (var i = 0; i < action.rows.length; i++) {
								detailTpl.append('in1', action.rows[i]);
							}
							if (action.rows.length < 3) {
								for (var z = 0; z < 10 - action.rows.length * 4; z++) {
									detailTpl.append('in1', '');
								}
							}
						} else {
							document.getElementById('in1').innerHTML = '<table><tr><td>暂无信息！</td></tr><tr><td height=48></td></tr></table>';
						}

					},
					failure : function() {
					},
					method : "post",
					params : {
						method : 'getInvoiceSplitInfoBySimpleInvoice',
						contractID : r.get(DBSchema.InvoiceDetail.ContractID),
						invoiceNumber : r.get(DBSchema.InvoiceDetail.InvoiceNo)
					}
				});
			});

	if (contractTypeInEdit_invoice == 2) {
		Ext.getCmp('splitInfo').hide(true);
		southPanel.hide(true);
		invoiceCenterPanel.doLayout();
	}

//	if (readOnly) {
//		invoiceDetailEditGrid.getTopToolbar().hide(true);
//		receiptDetailEditGrid.getTopToolbar().hide(true);
//	}

});
function nameLinker(val) {
	var text = val.substring(0, val.indexOf("^"));
	var url = val.substring(val.indexOf("^") + 1, val.length);
	if (text != '0') {
		return '<a href="' + url + '">' + text + '</a>';
	} else
		return text;
}

var invoiceSplitWin;
function showInvoiceSplit(data) {
	invoiceSplitWin = new Ext.Window({
				modal : true,
				title : '发票执行预估(开票日期:'
						+ Ext.util.Format
								.date(
										data
												.get(DBSchema.InvoiceDetail.InvoiceDateDisplayed),
										'Y-m-d')
						+ ' 票据金额:'
						+ Ext.util.Format.number(
								data.get(DBSchema.InvoiceDetail.InvoiceAmt),
								'0,000.00') + ')',
				width : 600,
				height : 402,
				layout : 'border',
				items : new com.bitc.xmis.contract.incoiceSplitListPanel({
							contractID : data
									.get(DBSchema.InvoiceDetail.ContractID),
							invoiceNumber : data
									.get(DBSchema.InvoiceDetail.InvoiceNo),
							totalAmount : data
									.get(DBSchema.InvoiceDetail.InvoiceAmt),
							region : 'center'
						})
			});
	invoiceSplitWin.show();
}
