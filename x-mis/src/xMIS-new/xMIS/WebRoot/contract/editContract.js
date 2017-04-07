Ext.onReady(function() {
	if (contract != null) {
		// 临时变量，已收款金额，当合同票据登记改变时，为计算已开票未到账金额
		window.earnedAmtTemp = contract.earnedTotalAmt;
		// 临时变量，已收款金额，当合同票据登记改变时，为计算已开票未到账金额
		window.invoicedAmtTemp = contract.invoicedTotalAmt;
	}

	Ext.QuickTips.init();
	Ext.form.Field.prototype.msgTarget = 'under';
	/**
	 * 附件列表
	 */
	var attGrid = new com.bitc.xmis.AttachmentList({
				attachmentTypeID : com.bitc.xmis.Attachment.contract,
				REFID : contract != null ? contract.contractID : 0,
				title : '其他相关文件',
				region : 'center',
				stripeRows : true,
//				height : Ext.getBody().getHeight() * 0.3,
				loadMask : true,
				enableColumnMove : false,
				enableColumnResize : false,
				actionColumnHidden : true
			});

	var conTextRecord = Ext.data.Record.create([{
				name : 'costID',
				mapping : 'ID'
			}, {
				name : 'costName',
				type : 'string',
				mapping : 'name'
			}, {
				name : 'costDesc',
				type : 'string',
				mapping : 'description'
			}, {
				name : 'costMoney',
				type : 'number',
				mapping : '金额'
			}, {
				name : 'costPercent',
				mapping : '百分比'
			}]);

	var conTextStore = new Ext.data.Store({
				url : __cxtPath + '/contractMain.do?method=getConTextSplitList',
				pruneModifiedRecords : true,
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount'
						}, conTextRecord),
				checkAmt : function(s, record, opt) {
					var sum = 0.00;
					var records = conTextGridPanel.getStore().getRange();
					for (var i = 0; i < records.length; i++) {
						if (typeof(records[i].get('costMoney')) != "undefined")
							sum += parseFloat(records[i].get('costMoney'))
									* 100;
					}
					sum = sum / 100;
					var titleStr = '按合同文本规定分解合同金额        ';
					var amt = Ext.getCmp('contractAmt').value;
					if (sum < amt) {
						var title = amt - sum;
						Ext.getCmp('moneygrid').setTitle(titleStr + '（还有'
								+ Ext.util.Format.number(title, '0,000.00')
								+ '未分解）');
					} else if (sum > amt) {
						var title = sum - amt;
						Ext.getCmp('moneygrid').setTitle(titleStr + '（金额超出'
								+ Ext.util.Format.number(title, '0,000.00')
								+ '）');
					} else if (sum == amt) {
						Ext
								.getCmp('moneygrid')
								.setTitle(titleStr
										+ '<span class="font-color-green">（分解完毕）</span>');
					}
				},
				listeners : {
					'update' : function(s, record, opt) {
						if (Ext.getCmp('contractAmt').value == 0
								|| Ext.getCmp('contractAmt').value == null) {
							Ext
									.getCmp('moneygrid')
									.setTitle('按合同文本规定分解合同金额        '
											+ '<font color=red><b>（请先填写合同金额）</b></font>');
						} else if (opt == Ext.data.Record.EDIT
								&& Ext.isNumber(record.get('costMoney'))) {
							conTextStore.checkAmt();
						}
					}
				}
			});

	var csm = new Ext.grid.CheckboxSelectionModel({
				checkOnly : true,
				singleSelect : false,
				listeners : {
					'selectionchange' : function(sm) {
						if (sm.getCount()) {
							conTextGridPanel.removeBtn.enable();
						} else {
							conTextGridPanel.removeBtn.disable();
						}
					}
				}
			});

	var currentIndex = -1;// 当前选择的行
	var conTextGridPanel = new Ext.grid.EditorGridPanel({
		store : conTextStore,
		id : 'elsesplit',
		width : 677,
		height : 200,
		border : true,
		stripeRows : true,
		enableColumnResize : false,
		selModel : new Ext.grid.RowSelectionModel({}),
		clicksToEdit : 1,
		sm : csm,
		listeners : {
			'cellclick' : function(g, rowIndex, columnIndex, e) {
				currentIndex = rowIndex;
			}
		},
		tbar : [{
					ref : '../newBtn',
					iconCls : 'add',
					text : '新建',
					width : 54,
					handler : function() {
						var Plant = conTextGridPanel.getStore().recordType;
						var p = new Plant({});
						conTextGridPanel.stopEditing();
						conTextGridPanel.getStore().add([p]);
						conTextGridPanel.startEditing(conTextStore.getCount()
										- 1, 1);
						currentIndex = conTextStore.getCount() - 1;
					}
				}, '-', {
					ref : '../removeBtn',
					iconCls : 'remove',
					text : '删除',
					width : 54,
					disabled : true,
					handler : function() {
						var rows = conTextGridPanel.getSelectionModel()
								.getSelections();// 获取所选行
						var costIDs = new Array();
						for (var i = 0; i < rows.length; i++) {
							if (typeof(rows[i].get('costID')) == 'undefined') {
								conTextStore.remove(rows[i]);
								conTextStore.checkAmt();
								clearAllSelectedCheckbox();
							} else {
								costIDs.push(rows[i].get('costID'));
							}
						}
						if (costIDs.length > 0) {
							Ext.Ajax.request({
								url : __cxtPath
										+ '/contractMain.do?method=deleteConTextRecord',
								params : {
									contractID : contract.contractID,
									costIDs : costIDs.join(',')
								},
								success : function(response, opt) {
									currentIndex = -1;
									conTextStore.reload({
												callback : function() {
													conTextStore.checkAmt();
													clearAllSelectedCheckbox();
												}
											});
									Ext.Msg.alert("提示", "删除成功");
								},
								failure : function(response, opt) {
									var obj = Ext.decode(response.responseText);
									Ext.Msg
											.alert(obj.messageTitle,
													obj.message);
									return;
								}
							});
						} else {
							Ext.Msg.alert("提示", "删除成功");
						}
						conTextGridPanel.getView().refresh();
					}
				}, '-', {
					iconCls : 'reload',
					ref : '../refreshButton',
					text : '刷新',
					handler : function(tabs, item) {
						conTextStore.reload();
						conTextStore.checkAmt();
					}
				}],
		colModel : new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({
							width : 23
						}), {
					header : '费用类型ID',
					dataIndex : 'costID',
					width : 20,
					hidden : true,
					sortable : true,
					editor : new Ext.form.TextField()
				}, {
					header : '费用名称',
					dataIndex : 'costName',
					width : 155,
					sortable : true,
					editor : new Ext.form.TextField({
								allowBlank : false,
								emptyText : "必输项"
							})
				}, {
					header : '费用描述',
					dataIndex : 'costDesc',
					width : 150,
					sortable : true,
					editor : new Ext.form.TextArea({})
				}, {
					xtype : 'numbercolumn',
					format : '0,000.00',
					header : '金额',
					dataIndex : 'costMoney',
					width : 155,
					sortable : true,
					editor : new Ext.form.NumberField({
								minValue : 0,
								allowBlank : false,
								listeners : {
									'change' : function(field, newValue,
											oldValue) {
										conTextStore.getAt(currentIndex).set(
												'costPercent',
												(newValue / Ext
														.getCmp('contractAmt')
														.getValue())
														* 100);
									}
								}
							})
				}, {
					xtype : 'numbercolumn',
					dataIndex : 'costPercent',
					header : '金额百分比',
					width : 155,
					format : '00.00%',
					sortable : true,
					editor : new Ext.form.NumberField({
								minValue : 0,
								allowBlank : false,
								listeners : {
									'change' : function(field, newValue,
											oldValue) {
										conTextStore.getAt(currentIndex).set(
												'costMoney',
												(newValue * Ext
														.getCmp('contractAmt')
														.getValue())
														/ 100);
									}
								}
							})
				}, csm])
	});

	var contractFormPanel = new Ext.FormPanel({
		id : 'contractFormPanel',
		region : 'center',
		// padding : 10,
		width : 780,
		bodyStyle : 'padding : 8,8,0,8',
		autoScroll : true,
		labelAlign : 'right',
		labelWidth : 90,
		items : [{
			xtype : 'fieldset',
			id : 'kzbfieldsetId',
			title : '科质部填写',
			width : 780,
			defaultType : 'textfield',
			defaults : {
				border : false
			},
			items : [{
						xtype : 'hidden',
						name : 'newcontractflag',
						id : 'newcontractId'
					}, {
						xtype : 'hidden',
						name : 'contractPrefix',
						value : prefix
					}, {
						xtype : 'panel',
						layout : 'form',
						id : 'projectCodeCtPanel',
						width : 714,
						items : [{
									xtype : 'hidden',
									name : 'projectID',
									id : 'projectID'
								}, {
									xtype : 'panel',
									layout : 'form',
									id : 'projectCodePanelId',
									name : 'projectCodePanel',
									border : false,
									labelWidth : 90,
									labelAlign : 'right',
									items : [{
										xtype : 'compositefield',
										fieldLabel : '项目编码',
										id : 'projectCodeId',
										name : 'projectCode',
										buildCombinedErrorMessage : function(
												errors) {
											var combined = [], error;
											for (var i = 0, j = errors.length; i < j; i++) {
												error = errors[i];
												combined.push(String.format(
														"{0}: {1}<br/>",
														error.errorName,
														error.error));
											}
											return combined.join("");
										},
										onFieldMarkInvalid : function(field,
												message) {
											var name = field.getItemId(), error = {
												field : name,
												errorName : field.fieldLabel
														|| name,
												error : message
											};
											this.fieldErrors.replace(name,
													error);

											if (!field.preventMark) {
												field.el
														.addClass(field.invalidClass);
											}
										},
										onFieldClearInvalid : function(field) {
											this.fieldErrors.removeKey(field
													.getItemId());
											field.el
													.removeClass(field.invalidClass);
										},
										items : [{
													xtype : 'displayfield',
													value : prefix
												}, {
													xtype : 'displayfield',
													value : '-'
												}, {
													xtype : 'combo',
													width : 67,
													itemId : '部门代码',
													id : 'projectDeptId',
													hiddenName : 'projectDept',
													mode : 'remote',
													triggerAction : 'all',
													forceSelection : true,
													editable : false,
													allowBlank : false,
													displayField : 'code',
													valueField : 'code',
													tpl : '<tpl for="."><div class="x-combo-list-item" ext:qtitle="" '
															+ ' ext:qtip="{deptName}">{code}</div></tpl>',
													store : new Ext.data.JsonStore(
															{
																url : __cxtPath
																		+ '/contractMain.do?method=getDeptCode',
																autoLoad : true,
																fields : [{
																	name : 'deptID',
																	mapping : '部门ID'
																}, {
																	name : 'deptName',
																	mapping : '部门名称'
																}, {
																	name : 'code'
																}]
															}),
													listeners : {
														'select' : function(
																combo, record,
																index) {
															contractFormPanel
																	.getForm()
																	.findField('projcodeTip')
																	.setValue('该合同将被分配给'
																			+ record
																					.get('deptName')
																			+ '执行');
															contractFormPanel
																	.getForm()
																	.findField('projectNumber')
																	.loadValue();
															contractFormPanel
																	.getForm()
																	.findField('bDept_id')
																	.setValue(record
																			.get('deptID'));
															contractFormPanel
																	.getForm()
																	.findField('bDept_name')
																	.setValue(record
																			.get('deptName'));

														}
													}
												}, {
													xtype : 'displayfield',
													value : '-'
												}, {
													xtype : 'combo',
													width : 67,
													id : 'projectYearId',
													hiddenName : 'projectYear',
													mode : 'local',
													value : new Date()
															.getFullYear(),
													triggerAction : 'all',
													forceSelection : true,
													editable : false,
													displayField : 'id',
													valueField : 'name',
													store : new Ext.data.ArrayStore(
															{
																fields : ['id',
																		'name'],
																data : []
															}),
													listeners : {
														'beforerender' : function(
																component) {
															var newyear = new Date()
																	.getFullYear();
															var yearlist = [];
															for (var i = newyear
																	+ 1; i >= 2005; i--) {
																yearlist.push([
																		i, i]);
															}
															this.store
																	.loadData(yearlist);
														},
														'select' : function(
																combo, record,
																index) {
															if (record
																	.get('name') != 2012) {
																contractFormPanel
																		.getForm()
																		.findField('projcodeTip')
																		.setValue('您选择了'
																				+ record
																						.get('name')
																				+ '年，项目编码将重新设置。');
															} else if (record
																	.get('name') == 2012) {
																contractFormPanel
																		.getForm()
																		.findField('projcodeTip')
																		.setValue('');
															}
															contractFormPanel
																	.getForm()
																	.findField('projectNumber')
																	.loadValue();
														}
													}
												}, {
													xtype : 'displayfield',
													value : '-'
												}, {
													xtype : 'combo',
													width : 66,
													id : 'projectCodeTypeId',
													itemId : '项目类型代码',
													hiddenName : 'projectCodeType',
													mode : 'remote',
													displayField : 'name',
													valueField : 'name',
													triggerAction : 'all',
													forceSelection : true,
													allowBlank : false,
													editable : false,
													tpl : '<tpl for="."><div class="x-combo-list-item" ext:qtitle="" '
															+ ' ext:qtip="{description}">{name}</div></tpl>',
													store : new Ext.data.JsonStore(
															{
																url : __cxtPath
																		+ '/contractMain.do?method=getProjectCodeTypeByParentID',
																baseParams : {
																	parentID : 14
																},
																autoLoad : true,
																fields : [{
																	name : 'name'
																}, {
																	name : 'description'
																}]
															}),
													listeners : {
														'select' : function(
																combo, record,
																index) {
															contractFormPanel
																	.getForm()
																	.findField('projectNumber')
																	.loadValue();
														}
													}
												}, {
													xtype : 'displayfield',
													value : '-'
												}, {
													xtype : 'textfield',
													width : 66,
													readOnly : true,
													id : 'projectNumberId',
													name : 'projectNumber',
													loadValue : function() {
														var projectDept = contractFormPanel
																.getForm()
																.findField('projectDept')
																.getValue();
														var projectYear = contractFormPanel
																.getForm()
																.findField('projectYear')
																.getValue();

														var projectCodeType = contractFormPanel
																.getForm()
																.findField('projectCodeType')
																.getValue();
														if (Ext
																.isEmpty(projectDept)
																|| Ext
																		.isEmpty(projectYear)
																|| Ext
																		.isEmpty(projectCodeType)) {
															return;
														}
														Ext.Ajax.request({
															url : __cxtPath
																	+ '/contractMain.do?method=getProjectNumber',
															params : {
																projectDept : projectDept,
																projectYear : projectYear,
																projectCodeType : projectCodeType
															},
															scope : this,
															success : function(
																	response) {
																var result = Ext
																		.decode(response.responseText);
																if (result.success) {
																	this
																			.setValue(result.attributes.number);
																}
																// else {
																//														
																// }
															},
															failure : function() {
															}
														});
													}
												}, {
													xtype : 'displayfield',
													width : 10
												}, {
													xtype : 'displayfield',
													name : 'projcodeTip',
													width : 250,
													style : 'color : red; font-weight : bold;',
													readOnly : true
												}]
									}]
								}, {
									xtype : 'panel',
									layout : 'form',
									labelWidth : 90,
									border : false,
									id : 'projectCodeExistPanel',
									hidden : true,
									items : [{
												xtype : 'displayfield',
												fieldLabel : '项目编码',
												id : 'projectCodeExistId',
												name : 'projectCodeExist',
												width : 200
											}]
								}]
					}, {
						xtype : 'panel',
						layout : 'form',
						labelWidth : 90,
						width : 600,
						autoWidth : true,
						labelAlign : 'right',
						items : [{
									xtype : 'textfield',
									id : 'contractNameId',
									name : 'contractName',
									allowBlank : false,
									fieldLabel : '合同名称',
									width : 348,
									allowBlack : false
								}]
					}, {
						xtype : 'panel',
						border : false,
						layout : 'form',
						items : {
							id : 'contractAmtCtId',
							xtype : 'panel',
							border : false,
							layout : 'hbox',
							fieldLabel : '合同金额',
							getAsycnValue : function(callback) {
								this.currency.store.on('load', function() {
									var r = this.currency.findRecord(
											this.currency.valueField,
											this.currency.value);
									var value = "";
									if (this.amt.getValue() >= 800000) {
										value = Ext.util.Format.number(this.amt
														.getValue(), r
														.get('symbol')
														+ '0,0.00')
												+ '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
												+ '<font color="blue">'
												+ this.amttip.getValue()
												+ '</font>';
									} else {
										var value = Ext.util.Format.number(
												this.amt.getValue(), r
														.get('symbol')
														+ '0,0.00');
									}
									callback.call(window, value);
								}, this);
								return;
							},
							// width : 714,
							items : [{
								ref : 'amt',
								xtype : 'formatnum',
								// fieldLabel : '合同金额',
								name : 'contractAmt',
								id : 'contractAmt',
								width : 243,
								allowBlank : false,
								listeners : {
									'change' : function() {
										contractFormPanel.getForm()
												.findField('contractAmt').sum();
										conTextStore.checkAmt();
										if (contractFormPanel.getForm()
												.findField('contractAmt')
												.getValue() >= 800000) {
											Ext.getCmp('contractAmtTipId')
													.show();
										} else {
											Ext.getCmp('contractAmtTipId')
													.hide();
										}
									}
								},
								sum : function() {
									// sum = contractFormPanel.getForm()
									// .findField('selfHard').sum()
									// + contractFormPanel.getForm()
									// .findField('homemadeSoft')
									// .sum()
									// + contractFormPanel.getForm()
									// .findField('softwareCost')
									// .sum()
									// + contractFormPanel.getForm()
									// .findField('constructCost')
									// .sum();

									sum = contractFormPanel.getForm()
											.findField('selfHard').sum()
											* 100
											+ contractFormPanel.getForm()
													.findField('homemadeSoft')
													.sum()
											* 100
											+ contractFormPanel.getForm()
													.findField('softwareCost')
													.sum()
											* 100
											+ contractFormPanel.getForm()
													.findField('constructCost')
													.sum() * 100;
									sum = sum / 100;
									var contractAmt = contractFormPanel
											.getForm().findField('contractAmt')
											.getValue();
									var titleStr = '按科质部资质管理标准分解合同金额     ';
									if (sum > contractAmt) {
										var title = sum - contractAmt;
										if (contractAmt >= 800000) {
											Ext
													.getCmp('moneyfieldset')
													.setTitle(titleStr
															+ '<span class="font-color-red">（金额超出：'
															+ Ext.util.Format
																	.number(
																			title,
																			'0,000.00')
															+ '）</span>');
										} else {
											Ext
													.getCmp('moneyfieldset')
													.setTitle(titleStr
															+ '（金额超出：'
															+ Ext.util.Format
																	.number(
																			title,
																			'0,000.00')
															+ '）');
										}
									} else if (sum < contractAmt) {
										var title = contractAmt - sum;
										if (contractAmt >= 800000) {
											Ext
													.getCmp('moneyfieldset')
													.setTitle(titleStr
															+ '<span class="font-color-red">（还有'
															+ Ext.util.Format
																	.number(
																			title,
																			'0,000.00')
															+ '没有分解）</span>');
										} else {
											Ext
													.getCmp('moneyfieldset')
													.setTitle(titleStr
															+ '（还有'
															+ Ext.util.Format
																	.number(
																			title,
																			'0,000.00')
															+ '没有分解）');
										}
									} else {
										Ext
												.getCmp('moneyfieldset')
												.setTitle(titleStr
														+ '<span class="font-color-green">（分解完毕）</span>');
									}
									sum = (sum == null || sum == '')
											? 0
											: parseFloat(sum);
									return sum;
								}
							}, new com.bitc.xmis.Combos.CurrencyCombo({
										margins : '0 0 0 5',
										ref : 'currency',
										id : 'currencyTypeID',
										hiddenName : 'currencyType',
										width : 100,
										listeners : {
											'setValueEvent' : function(f) {
												Ext.getCmp('selfHardCurID')
														.setValue(f
																.getRawValue());
												Ext.getCmp('homemadeHardCurID')
														.setValue(f
																.getRawValue());
												Ext.getCmp('importHardCurID')
														.setValue(f
																.getRawValue());
												Ext.getCmp('homemadeSoftCurID')
														.setValue(f
																.getRawValue());
												Ext.getCmp('importSoftCurID')
														.setValue(f
																.getRawValue());
												Ext.getCmp('softDevCurID')
														.setValue(f
																.getRawValue());
												Ext.getCmp('sysDesCurID')
														.setValue(f
																.getRawValue());
												Ext.getCmp('sysIntCurID')
														.setValue(f
																.getRawValue());
												Ext.getCmp('techCurID')
														.setValue(f
																.getRawValue());
												Ext.getCmp('constructCurID')
														.setValue(f
																.getRawValue());
												Ext.getCmp('managementCurID')
														.setValue(f
																.getRawValue());
												Ext.getCmp('transportCurID')
														.setValue(f
																.getRawValue());
												Ext.getCmp('taxesCurID')
														.setValue(f
																.getRawValue());
											},
											'select' : function(combo, record,
													index) {
												contractFormPanel
														.getForm()
														.findField('selfHardCur')
														.setValue(record
																.get('name'));
												contractFormPanel
														.getForm()
														.findField('homemadeHardCur')
														.setValue(record
																.get('name'));
												contractFormPanel
														.getForm()
														.findField('importHardCur')
														.setValue(record
																.get('name'));
												contractFormPanel
														.getForm()
														.findField('homemadeSoftCur')
														.setValue(record
																.get('name'));
												contractFormPanel
														.getForm()
														.findField('importSoftCur')
														.setValue(record
																.get('name'));
												contractFormPanel
														.getForm()
														.findField('softDevCur')
														.setValue(record
																.get('name'));
												contractFormPanel.getForm()
														.findField('sysDesCur')
														.setValue(record
																.get('name'));
												contractFormPanel.getForm()
														.findField('sysIntCur')
														.setValue(record
																.get('name'));
												contractFormPanel.getForm()
														.findField('techCur')
														.setValue(record
																.get('name'));
												contractFormPanel
														.getForm()
														.findField('constructCur')
														.setValue(record
																.get('name'));
												contractFormPanel
														.getForm()
														.findField('managementCur')
														.setValue(record
																.get('name'));
												contractFormPanel
														.getForm()
														.findField('transportCur')
														.setValue(record
																.get('name'));
												contractFormPanel.getForm()
														.findField('taxesCur')
														.setValue(record
																.get('name'));
											}
										}
									}), {
								xtype : 'panel',
								width : 250,
								style : 'color : blue;',
								height : 32,
								border : false,
								margins : '0 0 0 20',
								items : [{
											ref : '../amttip',
											xtype : 'displayfield',
											id : 'contractAmtTipId',
											style : 'color : blue;',
											value : '合同金额超过80万，请按科质部资质管理标准分解，并分解完毕',
											hidden : true,
											border : false
										}]
							}]
						}
					}, {
						xtype : 'panel',
						layout : 'column',
						width : 714,
						items : [{
							xtype : 'panel',
							layout : 'form',
							width : 495,
							labelWidth : 90,
							border : false,
							labelAlign : 'right',
							items : [{
										xtype : 'hidden',
										id : 'aCompany_id',
										name : 'aCompany_id'
									}, new Ext.ux.form.ChooseField({
										fieldLabel : '甲方公司',
										id : 'aCompany_name',
										name : 'aCompany_name',
										width : 348,
										winConfig : {
											title : '甲方公司设置'
										},
										baseParamFun : function() {
											winFlag = "aWin";
											return {
												start : 0,
												limit : 20
											};
										},
										allowBlank : false,
										chooseUrl : __cxtPath
												+ '/resources/choose/company.json.jsp',
										setValueFun : function(o) {
											Ext
													.getCmp('aCompany_id')
													.setValue(o
															.get(DBSchema.Company.CompanyID));
											Ext
													.getCmp('aCompany_name')
													.setValue(o
															.get(DBSchema.Company.CompanyName));
											contractFormPanel.getForm()
													.findField('aDept_id')
													.setValue('');
											contractFormPanel.getForm()
													.findField('aDept_name')
													.setValue('');
											contractFormPanel.getForm()
													.findField('aEmp_id')
													.setValue('');
											contractFormPanel.getForm()
													.findField('aEmp_name')
													.setValue('');
											contractFormPanel.getForm()
													.findField('contact_id')
													.setValue('');
											contractFormPanel.getForm()
													.findField('aContact_name')
													.setValue('');
										},
										clearValueFun : function() {
											contractFormPanel.getForm()
													.findField('aCompany_id')
													.setValue('');
											contractFormPanel.getForm()
													.findField('aCompany_name')
													.setValue('');
											contractFormPanel.getForm()
													.findField('aDept_id')
													.setValue('');
											contractFormPanel.getForm()
													.findField('aDept_name')
													.setValue('');
											contractFormPanel.getForm()
													.findField('aEmp_id')
													.setValue('');
											contractFormPanel.getForm()
													.findField('aEmp_name')
													.setValue('');
											contractFormPanel.getForm()
													.findField('contact_id')
													.setValue('');
											contractFormPanel.getForm()
													.findField('aContact_name')
													.setValue('');
										}
									})]
						}]
					}, {
						xtype : 'panel',
						layout : 'form',
						width : 714,
						items : [{
									xtype : 'panel',
									layout : 'form',
									width : 495,
									labelWidth : 90,
									labelAlign : 'right',
									border : false,
									items : [{
												xtype : 'hidden',
												id : 'bCompany_id',
												name : 'bCompany_id',
												value : '1'
											}, new Ext.ux.form.ChooseField({
														fieldLabel : '乙方公司',
														id : 'bCompany_name',
														name : 'bCompany_name',
														width : 348,
														value : '北京中水科水电科技开发有限公司 ',
														allowBlank : false,
														hideTrigger2 : true
													})]
								}]
					}, {
						xtype : 'panel',
						width : 348,
						layout : 'form',
						autoWidth : true,
						labelAlign : 'right',
						labelWidth : 90,
						border : false,
						items : [{
							xtype : 'datefield',
							fieldLabel : '签订日期',
							id : 'signatureDateId',
							name : 'signatureDate',
							width : 348,
							format : 'Y-m-d',
							allowBlank : false,
							getAsycnValue : function(callback) {
								callback.call(this, Ext.util.Format.date(this
														.getValue(),
												this.format));
							}
						}]
					}]
		}, {
			xtype : 'fieldset',
			// title : '以下由事业部填写',
			title : ' ',
			defaultType : 'textfield',
			width : 780,
			defaults : {
				border : false
			},
			items : [{
						xtype : 'panel',
						width : 714,
						layout : 'column',
						items : [{
									xtype : 'panel',
									layout : 'form',
									columnWidth : 0.33,
									labelAlign : 'right',
									labelWidth : 90,
									border : false,
									items : [{
												xtype : 'hidden',
												id : 'contractID',
												name : 'contractID'
											}, {
												xtype : 'textfield',
												fieldLabel : '甲方合同编号',
												name : 'contractCode',
												width : 136
											}]
								}, {
									xtype : 'panel',
									layout : 'form',
									columnWidth : 0.66,
									labelWidth : 90,
									labelAlign : 'right',
									border : false,
									items : [{
												xtype : 'textfield',
												fieldLabel : '合同简称',
												width : 345,
												id : 'abb',
												maxLength : 30,
												name : 'abb'
											}]
								}]
					}, {
						xtype : 'panel',
						width : 714,
						// autoWidth : true,
						layout : 'column',
						items : [{
							xtype : 'panel',
							layout : 'form',
							columnWidth : 0.33,
							labelAlign : 'right',
							labelWidth : 90,
							border : false,
							items : [{
								xtype : 'combo',
								fieldLabel : '合同来源',
								width : 136,
								hiddenName : 'contractSourceType',
								id : 'contractSourceType',
								displayField : 'sourcename',
								valueField : 'sourceid',
								mode : 'remote',
								triggerAction : 'all',
								editable : false,
								value : 102,// 默认‘横向合同’
								store : new Ext.data.JsonStore({
									url : __cxtPath
											+ '/contractMain.do?method=getContractSourceTypeID',
									baseParams : {
										level : 1
									},
									autoLoad : true,
									fields : [{
												name : 'sourceid',
												mapping : '合同来源类型ID'
											}, {
												name : 'sourcename',
												mapping : '合同来源类型'
											}],
									listeners : {
										'load' : function(s, records, options) {
											var cmp = Ext
													.getCmp('contractSourceType');
											if (cmp.value == 0) {
												cmp.setValue('');
											} else {
												cmp.setValue(cmp.value);
											}
										}
									}
								}),
								listeners : {
									'afterrender' : function(cmp) {
										if (cmp.getValue() == 102) {
											Ext.getCmp('GSourceTypeNamePanel')
													.hide();
										} else if (cmp.getValue() == 201) {
											Ext.getCmp('GSourceTypeNamePanel')
													.hide();
											Ext
													.getCmp('aCompany_name')
													.setValue('北京中水科水电科技开发有限公司');
											Ext.getCmp('aCompany_id')
													.setValue('1');
										} else {
											Ext.getCmp('GSourceTypeNamePanel')
													.show();
										}
									},
									'select' : function(combo, record, index) {
										if (record.get('sourceid') == 102) {
											Ext.getCmp('GSourceTypeNamePanel')
													.hide();
										} else if (record.get('sourceid') == 201) {
											Ext.getCmp('GSourceTypeNamePanel')
													.hide();
											Ext
													.getCmp('aCompany_name')
													.setValue('北京中水科水电科技开发有限公司');
											Ext.getCmp('aCompany_id')
													.setValue('1');
										} else {
											Ext.getCmp('GSourceTypeNamePanel')
													.show();
										}
									}
								}
							}]
						}, {
							xtype : 'panel',
							layout : 'form',
							columnWidth : 0.66,
							labelWidth : 90,
							labelAlign : 'right',
							id : 'GSourceTypeNamePanel',
							border : false,
							items : [{
										xtype : 'hidden',
										id : 'gSourceTypeID',
										name : 'gSourceTypeID'
									}, new Ext.ux.form.ChooseField({
												fieldLabel : '来源信息',
												id : 'GSourceTypeName',
												name : 'GSourceTypeName',
												anchor : '93.6%',
												onTrigger2Click : function() {
													if (this.disabled) {
														return;
													}
													showStWin();
												}
											})]
						}]
					}, {
						xtype : 'panel',
						layout : 'form',
						labelAlign : 'right',
						labelWidth : 90,
						width : 714,
						items : [{
							xtype : 'textarea',
							fieldLabel : '合同摘要',
							name : 'summary',
							id : 'summary',
							width : 580,
							readOnly : true,
							height : 70,
							listeners : {
								'change' : function(t, newValue, oldValue) {
									if (Ext.getCmp('summarycheck1ID')
											.getValue() == 2) {
										Ext.getCmp('summarywriteValue')
												.setValue(newValue);
									}
								},
								'render' : function(c) {
									Ext.QuickTips.register({
												target : c.getEl(),
												text : '"合同摘要"最多可输入2500字'
											});
								}
							}
						}, {
							xtype : 'hidden',
							id : 'summarystencilValue'// 记录“按模板编辑”临时数据
						}, {
							xtype : 'hidden',
							id : 'summarywriteValue'// 记录“直接编辑”临时数据
						}]
					}, {
						xtype : 'panel',
						layout : 'form',
						id : 'summaryCheckPanel',
						hidden : true,
						width : 714,
						labelAlign : 'right',
						labelWidth : 90,
						items : [{
							fieldLabel : '摘要编辑方式',
							xtype : 'radiogroup',
							name : "summarycheck1",
							id : 'summarycheck1ID',
							width : 200,
							items : [{
								name : "summarycheck",
								id : 'summarystencilId',
								xtype : 'radio',
								inputValue : 1,
								boxLabel : "按模版编辑",
								checked : true,
								listeners : {
									'check' : function(c, b) {
										if (b) {
											stencilFlag = true;
											Ext.getCmp('summary').setValue('');
											Ext.getCmp('summary')
													.setReadOnly(true);
											Ext.getCmp('summaryfieldset')
													.show();
											Ext.getCmp('summaryfieldset')
													.doLayout();

											var textAreaDiv = document
													.getElementById('textareadiv');
											if (textAreaDiv != null)
												document
														.getElementById('summary')
														.removeChild(textAreaDiv);
											txt = document.createElement('DIV');
											txt.id = 'textareadiv', txt.innerHTML = Ext
													.getCmp('summarystencilValue')
													.getValue();
											document.getElementById('summary')
													.appendChild(txt);
											Ext.getCmp('summary').onBlur();
										}
									}
								}
							}, {
								name : "summarycheck",
								id : 'summarywriteId',
								xtype : 'radio',
								inputValue : 2,
								boxLabel : "直接编辑",
								listeners : {
									'check' : function(c, b) {
										if (b) {
											stencilFlag = false;
											var textAreaDiv = document
													.getElementById('textareadiv');
											if (textAreaDiv != null)
												document
														.getElementById('summary')
														.removeChild(textAreaDiv);
											Ext.getCmp('summary')
													.setReadOnly(false);
											Ext.getCmp('summary')
													.setVisible(true);
											Ext.getCmp('summaryfieldset')
													.hide();

											Ext
													.getCmp('summary')
													.setValue(Ext
															.getCmp('summarywriteValue')
															.getValue());
											Ext.getCmp('summary').onBlur();
										}
									}
								}
							}]
						}]
					}, {
						xtype : 'fieldset',
						title : '按模版编辑合同摘要',
						width : 700,
						checkboxToggle : true,
						collapsed : true,
						id : 'summaryfieldset',
						hidden : true,
						border : true,
						listeners : {
							'expand' : function(p) {
								Ext.getCmp('stencilPanel').doLayout();
							}
						},
						items : [{
									xtype : 'panel',
									layout : 'form',
									labelAlign : 'right',
									labelWidth : 80,
									border : false,
									items : [{
												xtype : 'panel',
												id : 'stencilPanel',
												labelSeparator : '',
												fieldLabel : '&nbsp;',
												autoHeight : true,
												border : false,
												layout : 'form'
											}, {
												xtype : 'hidden',
												name : 'stencilContent',
												value : ''
											}]
								}]
					}, {
						xtype : 'panel',
						layout : 'column',
						id : 'warrantyfieldset',
						name : 'warrantyfieldset',
						width : 714,
						hidden : false,
						defaults : {
							border : false
						},
						items : [{
									xtype : 'panel',
									layout : 'form',
									columnWidth : 0.23,
									labelWidth : 90,
									labelAlign : 'right',
									items : [{
												xtype : 'numberfield',
												fieldLabel : '质保期限',
												width : 66,
												allowDecimals : false,
												minValue : 0,
												id : 'warrantyLimit',
												name : 'warrantyLimit',
												scope : this,
												listeners : {
													'change' : warLimitCalculate
												}
											}]
								}, {
									xtype : 'panel',
									columnWidth : 0.1,
									items : [{
										xtype : 'combo',
										id : 'warrantyUnit',
										name : 'warrantyUnit',
										displayField : 'text',
										valueField : 'value',
										mode : 'local',
										width : 69,
										triggerAction : 'all',
										allowBlank : false,
										value : 1,
										editable : false,
										store : new Ext.data.ArrayStore({
													fields : ['value', 'text'],
													data : [[1, '年'], [2, '月'],
															[3, '日']]
												}),
										listeners : {
											'select' : warLimitCalculate
										}
									}]
								}, {
									xtype : 'panel',
									layout : 'form',
									columnWidth : 0.33,
									labelWidth : 90,
									labelAlign : 'right',
									items : [{
												xtype : 'datefield',
												fieldLabel : '质保期起',
												id : 'warrantyStartDate',
												name : 'warrantyStartDate',
												format : 'Y-m-d',
												width : 136,
												listeners : {
													'select' : warLimitCalculate
												}
											}]
								}, {
									xtype : 'panel',
									layout : 'form',
									columnWidth : 0.33,
									labelWidth : 90,
									labelAlign : 'right',
									items : [{
												xtype : 'textfield',
												fieldLabel : '质保期止',
												id : 'warrantyEndDate',
												name : 'warrantyEndDate',
												readOnly : true,
												width : 136
											}]
								}]
					}, {
						xtype : 'panel',
						layout : 'column',
						id : 'validityfieldset',
						name : 'validityfieldset',
						width : 714,
						defaults : {
							border : false
						},
						items : [{
									xtype : 'panel',
									layout : 'form',
									columnWidth : 0.23,
									labelWidth : 90,
									labelAlign : 'right',
									items : [{
												xtype : 'numberfield',
												fieldLabel : '有效期限',
												width : 66,
												allowDecimals : false,
												minValue : 0,
												id : 'validityLimit',
												name : 'validityLimit',
												listeners : {
													'change' : valLimitCalculate
												}
											}]
								}, {
									xtype : 'panel',
									columnWidth : 0.1,
									items : [{
										xtype : 'combo',
										id : 'validityUnit',
										name : 'validityUnit',
										displayField : 'text',
										valueField : 'value',
										mode : 'local',
										width : 69,
										triggerAction : 'all',
										allowBlank : false,
										value : 1,
										editable : false,
										store : new Ext.data.ArrayStore({
													fields : ['value', 'text'],
													data : [[1, '年'], [2, '月'],
															[3, '日']]
												}),
										listeners : {
											'select' : valLimitCalculate
										}
									}]
								}, {
									xtype : 'panel',
									layout : 'form',
									columnWidth : 0.33,
									labelWidth : 90,
									labelAlign : 'right',
									items : [{
												xtype : 'datefield',
												fieldLabel : '有效期起',
												id : 'validityStartDate',
												name : 'validityStartDate',
												format : 'Y-m-d',
												width : 136,
												listeners : {
													'select' : valLimitCalculate
												}
											}]
								}, {
									xtype : 'panel',
									layout : 'form',
									columnWidth : 0.33,
									labelWidth : 90,
									labelAlign : 'right',
									items : [{
												xtype : 'textfield',
												fieldLabel : '有效期止',
												id : 'validityEndDate',
												name : 'validityEndDate',
												readOnly : true,
												width : 136
											}]
								}]
					}, {
						xtype : 'fieldset',
						title : '质保条款定义',
						width : 700,
						border : true,
						checkboxToggle : true,
						collapsed : true,
						items : [{
							xtype : 'panel',
							layout : 'form',
							labelAlign : 'right',
							labelWidth : 80,
							border : false,
							items : [{
										xtype : 'textarea',
										fieldLabel : '质保条款定义',
										name : 'warrantyDefinition',
										width : 580,
										border : false,
										listeners : {
											'render' : function(c) {
												Ext.QuickTips.register({
															target : c.getEl(),
															text : '"质保条款定义"最多可输入200字'
														});
											}
										}
									}]
						}]
					}, {
						xtype : 'panel',
						layout : 'column',
						width : 714,
						items : [{
							xtype : 'panel',
							border : false,
							layout : 'form',
							columnWidth : 0.33,
							labelWidth : 90,
							labelAlign : 'right',
							items : [{
										xtype : 'hidden',
										id : 'aDept_id',
										name : 'aDept_id'
									}, new Ext.ux.form.ChooseOrgTreeField({
										fieldLabel : '甲方部门',
										id : 'aDept_name',
										name : 'aDept_name',
										width : 136,
										winConfig : {
											title : '甲方部门设置',
											tbar : [{
												ref : '../addBtn',
												iconCls : 'add',
												align : 'right',
												text : '新建',
												handler : function() {
													Ext.Msg.show({
														title : '提示',
														msg : '新建成功后请刷新',
														buttons : Ext.Msg.OK,
														fn : function() {
															Ext
																	.getCmp('aDept_name')
																	.getTreePanel()
																	.getLoader()
																	.load(
																			Ext
																					.getCmp('aDept_name')
																					.getTreePanel()
																					.getRootNode(),
																			function(
																					n) {
																				Ext
																						.getCmp('aDept_name')
																						.getTreePanel()
																						.expand();
																			});
														}
													});
													parent.xMis
															.turn(
																	'company-info-'
																			+ Ext
																					.getCmp('aCompany_id')
																					.getValue(),
																	Ext
																			.getCmp('aCompany_name')
																			.getValue(),
																	__cxtPath
																			+ '/crmMaint.do?method=getCompanyInfo&companyID='
																			+ Ext
																					.getCmp('aCompany_id')
																					.getValue()
																			+ '&tab=1',
																	'menu-company-info',
																	true);
												}
											}, '-', {
												xtype : 'button',
												text : '刷新',
												iconCls : 'reload',
												ref : '../refreshButton',
												handler : function(tabs, item) {
													Ext
															.getCmp('aDept_name')
															.getTreePanel()
															.getLoader()
															.load(
																	Ext
																			.getCmp('aDept_name')
																			.getTreePanel()
																			.getRootNode(),
																	function(n) {
																		Ext
																				.getCmp('aDept_name')
																				.getTreePanel()
																				.expand();
																	});
												}
											}]
										},
										singleSelect : true,
										onlyPerson : false,
										rootConfig : function() {
											return {
												id : 'comp-old-'
														+ Ext
																.getCmp('aCompany_id')
																.getValue()
											};
										},
										dataUrl : '/orgMaint.do?method=getOrganizationTree',
										clearValueList : 'aDept_id,aDept_name,aEmp_id,aEmp_name',
										setValueFun : function(result) {
											var deptName = '';
											var deptID = '';
											for (var i = 0; i < result.length; i++) {
												if (i == result.length - 1) {
													deptID = deptID
															+ result[i].id;
													deptName = deptName
															+ result[i].text;
												} else {
													deptID = deptID
															+ result[i].id
															+ ',';
													deptName = deptName
															+ result[i].text
															+ ',';
												}
											}
											Ext
													.getCmp('aDept_id')
													.setValue(deptID.split('-')[2]);
											Ext.getCmp('aDept_name')
													.setValue(deptName);
										},
										listeners : {
											'beforeShowWin' : function() {
												var aCmpId = Ext
														.getCmp('aCompany_id')
														.getValue();
												if (!aCmpId
														|| aCmpId.trim() == "") {
													Ext.Msg.alert("提示",
															"请先选择甲方公司。");
													return false;
												}
											}
										}
									})]
						}, {
							xtype : 'panel',
							layout : 'form',
							columnWidth : 0.33,
							labelWidth : 90,
							labelAlign : 'right',
							border : false,
							items : [{
										xtype : 'hidden',
										id : 'aEmp_id',
										name : 'aEmp_id'
									}, new Ext.ux.form.ChooseOrgTreeField({
										fieldLabel : '甲方代表',
										id : 'aEmp_name',
										name : 'aEmp_name',
										width : 136,
										linkManDblclickFunction : true,
										modifyNameStr : '甲方代表',
										winConfig : {
											title : '甲方代表设置',
											tbar : [{
												ref : '../addBtn',
												iconCls : 'add',
												align : 'right',
												text : '新建',
												handler : function() {
													Ext.Msg.show({
														title : '提示',
														msg : '新建成功后请刷新',
														buttons : Ext.Msg.OK,
														fn : function() {
															Ext
																	.getCmp('aEmp_name')
																	.getTreePanel()
																	.getLoader()
																	.load(
																			Ext
																					.getCmp('aEmp_name')
																					.getTreePanel()
																					.getRootNode(),
																			function(
																					n) {
																				Ext
																						.getCmp('aEmp_name')
																						.getTreePanel()
																						.expand();
																			});
														}
													});
													parent.xMis
															.turn(
																	'company-info-'
																			+ Ext
																					.getCmp('aCompany_id')
																					.getValue(),
																	Ext
																			.getCmp('aCompany_name')
																			.getValue(),
																	__cxtPath
																			+ '/crmMaint.do?method=getCompanyInfo&companyID='
																			+ Ext
																					.getCmp('aCompany_id')
																					.getValue()
																			+ '&tab=1',
																	'menu-company-info',
																	true);
												}
											}, '-', {
												xtype : 'button',
												text : '刷新',
												iconCls : 'reload',
												ref : '../refreshButton',
												handler : function(tabs, item) {
													Ext
															.getCmp('aEmp_name')
															.getTreePanel()
															.getLoader()
															.load(
																	Ext
																			.getCmp('aEmp_name')
																			.getTreePanel()
																			.getRootNode(),
																	function(n) {
																		Ext
																				.getCmp('aEmp_name')
																				.getTreePanel()
																				.expand();
																	});
												}
											}]
										},
										singleSelect : true,
										onlyPerson : true,
										rootConfig : function() {
											var searchId = 'comp-old-'
													+ Ext.getCmp('aCompany_id')
															.getValue();
											return {
												id : searchId
											};
										},
										dataUrl : '/orgMaint.do?method=getOrganizationTree',
										setValueFun : function(result) {
											var empName = '';
											var empID = '';
											for (var i = 0; i < result.length; i++) {
												if (i == result.length - 1) {
													empID = empID
															+ result[i].id;
													empName = empName
															+ result[i].text;
												} else {
													empID = empID
															+ result[i].id
															+ ',';
													empName = empName
															+ result[i].text
															+ ',';
												}
											}
											Ext
													.getCmp('aEmp_id')
													.setValue(empID.split('-')[2]);
											Ext.getCmp('aEmp_name')
													.setValue(empName);
										},
										clearValueList : 'aEmp_id,aEmp_name',
										listeners : {
											'beforeShowWin' : function() {
												var aCmpId = Ext
														.getCmp('aCompany_id')
														.getValue();
												if (!aCmpId
														|| aCmpId.trim() == "") {
													Ext.Msg.alert("提示",
															"请先选择甲方公司。");
													return false;
												}
											},
											'dblclick' : function(select, num) {
												var empId = Ext
														.getCmp('aEmp_id')
														.getValue();
												com.bitc.xmis.util.EmpInfoWin(
														empId, '甲方代表信息');
											},
											'nodedblclick' : function(node, e) {
												var path = String.format(
														'/root{0}', node
																.getPath());
												parent.xMis
														.turn(
																'company-info-'
																		+ Ext
																				.getCmp('aCompany_id')
																				.getValue(),
																Ext
																		.getCmp('aCompany_name')
																		.getValue(),
																__cxtPath
																		+ '/crmMaint.do?method=getCompanyInfo&companyID='
																		+ Ext
																				.getCmp('aCompany_id')
																				.getValue()
																		+ '&tab=1&path='
																		+ path,
																'menu-company-info',
																true);
											},
											'render' : function(c) {
												Ext.QuickTips.register({
															target : c.getEl(),
															text : '双击可显示详细信息'
														});
											}
										}
									})]
						}, {
							xtype : 'panel',
							labelAlign : 'right',
							labelWidth : 90,
							columnWidth : 0.33,
							layout : 'form',
							border : false,
							items : [{
										xtype : 'hidden',
										id : 'contact_id',
										name : 'contact_id'
									}, new Ext.ux.form.ChooseOrgTreeField({
										fieldLabel : '甲方联系人',
										id : 'aContact_name',
										name : 'aContact_name',
										width : 136,
										linkManDblclickFunction : true,
										modifyNameStr : '联系人',
										winConfig : {
											title : '联系人设置',
											tbar : [{
												ref : '../addBtn',
												iconCls : 'add',
												align : 'right',
												text : '新建',
												handler : function() {
													Ext.Msg.show({
														title : '提示',
														msg : '新建成功后请刷新',
														buttons : Ext.Msg.OK,
														fn : function() {
															Ext
																	.getCmp('aContact_name')
																	.getTreePanel()
																	.getLoader()
																	.load(
																			Ext
																					.getCmp('aContact_name')
																					.getTreePanel()
																					.getRootNode(),
																			function(
																					n) {
																				Ext
																						.getCmp('aContact_name')
																						.getTreePanel()
																						.expand();
																			});
														}
													});
													parent.xMis
															.turn(
																	'company-info-'
																			+ Ext
																					.getCmp('aCompany_id')
																					.getValue(),
																	Ext
																			.getCmp('aCompany_name')
																			.getValue(),
																	__cxtPath
																			+ '/crmMaint.do?method=getCompanyInfo&companyID='
																			+ Ext
																					.getCmp('aCompany_id')
																					.getValue()
																			+ '&tab=1',
																	'menu-company-info',
																	true);
												}
											}, '-', {
												xtype : 'button',
												text : '刷新',
												iconCls : 'reload',
												ref : '../refreshButton',
												handler : function(tabs, item) {
													Ext
															.getCmp('aContact_name')
															.getTreePanel()
															.getLoader()
															.load(
																	Ext
																			.getCmp('aContact_name')
																			.getTreePanel()
																			.getRootNode(),
																	function(n) {
																		Ext
																				.getCmp('aContact_name')
																				.getTreePanel()
																				.expand();
																	});
												}
											}]
										},
										singleSelect : true,
										onlyPerson : true,
										rootConfig : function() {
											return {
												id : 'comp-old-'
														+ Ext
																.getCmp('aCompany_id')
																.getValue()
											};
										},
										dataUrl : '/orgMaint.do?method=getOrganizationTree',
										setValueFun : function(result) {
											var empName = '';
											var empID = '';
											for (var i = 0; i < result.length; i++) {
												if (i == result.length - 1) {
													empID = empID
															+ result[i].id;
													empName = empName
															+ result[i].text;
												} else {
													empID = empID
															+ result[i].id
															+ ',';
													empName = empName
															+ result[i].text
															+ ',';
												}
											}
											Ext
													.getCmp('contact_id')
													.setValue(empID.split('-')[2]);
											Ext.getCmp('aContact_name')
													.setValue(empName);
										},
										clearValueList : 'contact_id,aContact_name',
										listeners : {
											'beforeShowWin' : function() {
												var aCmpId = Ext
														.getCmp('aCompany_id')
														.getValue();
												if (!aCmpId
														|| aCmpId.trim() == "") {
													Ext.Msg.alert("提示",
															"请先选择甲方公司。");
													return false;
												}
											},
											'dblclick' : function(select, num) {
												var empId = Ext
														.getCmp('contact_id')
														.getValue();
												com.bitc.xmis.util.EmpInfoWin(
														empId, '甲方联系人信息');
											},
											'nodedblclick' : function(node, e) {
												var path = String.format(
														'/root{0}', node
																.getPath());
												parent.xMis
														.turn(
																'company-info-'
																		+ Ext
																				.getCmp('aCompany_id')
																				.getValue(),
																Ext
																		.getCmp('aCompany_name')
																		.getValue(),
																__cxtPath
																		+ '/crmMaint.do?method=getCompanyInfo&companyID='
																		+ Ext
																				.getCmp('aCompany_id')
																				.getValue()
																		+ '&tab=1&path='
																		+ path,
																'menu-company-info',
																true);
											},
											'render' : function(c) {
												Ext.QuickTips.register({
															target : c.getEl(),
															text : '双击可显示详细信息'
														});
											}
										}
									})]
						}]
					}, {
						xtype : 'panel',
						layout : 'column',
						width : 714,
						defaults : {
							border : false
						},
						items : [{
									xtype : 'panel',
									layout : 'form',
									columnWidth : 0.33,
									labelWidth : 90,
									labelAlign : 'right',
									items : [{
												xtype : 'hidden',
												id : 'bDept_id',
												name : 'bDept_id'
											}, {
												xtype : 'displayfield',
												fieldLabel : '乙方部门',
												width : 136,
												id : 'bDept_name',
												name : 'bDept_name'
											}]
								}, {
									xtype : 'panel',
									layout : 'form',
									columnWidth : 0.33,
									labelWidth : 90,
									labelAlign : 'right',
									items : [
											{
												xtype : 'hidden',
												id : 'bEmp_id',
												name : 'bEmp_id'
											},
											new Ext.ux.form.ChooseOrgTreeField(
													{
														fieldLabel : '乙方代表',
														id : 'bEmp_name',
														name : 'bEmp_name',
														width : 136,
														winConfig : {
															title : '乙方代表设置'
														},
														singleSelect : true,
														onlyPerson : true,
														expandNodePath : function() {
															var path = '/comp-old-1';
															var deptid = 0;
															if (Ext
																	.getCmp('bDept_id') != null
																	&& (Ext
																			.getCmp('bDept_id')
																			.getValue())
																			.trim() != '') {
																path = path
																		+ '/dept-old-'
																		+ Ext
																				.getCmp('bDept_id')
																				.getValue();
																deptid = Ext
																		.getCmp('bDept_id')
																		.getValue();
															}
															return {
																deptid : parseInt(deptid),
																path : path
															};
														},
														rootConfig : function() {
															return {
																id : 'comp-old-1'
															};
														},
														dataUrl : '/orgMaint.do?method=getOrganizationTree',
														setValueFun : function(
																result) {
															var empName = '';
															var empID = '';
															for (var i = 0; i < result.length; i++) {
																if (i == result.length
																		- 1) {
																	empID = empID
																			+ result[i].id;
																	empName = empName
																			+ result[i].text;
																} else {
																	empID = empID
																			+ result[i].id
																			+ ',';
																	empName = empName
																			+ result[i].text
																			+ ',';
																}
															}
															Ext
																	.getCmp('bEmp_id')
																	.setValue(empID
																			.split('-')[2]);
															Ext
																	.getCmp('bEmp_name')
																	.setValue(empName);
														},
														clearValueList : 'bEmp_id,bEmp_name',
														listeners : {
															'dblclick' : function(
																	select, num) {
																var empId = Ext
																		.getCmp('bEmp_id')
																		.getValue();
																com.bitc.xmis.util
																		.EmpInfoWin(
																				empId,
																				'乙方代表信息');

															},
															'nodedblclick' : function(
																	node, e) {
																var path = String
																		.format(
																				'/root{0}',
																				node
																						.getPath());
																parent.xMis
																		.turn(
																				'company-info-'
																						+ Ext
																								.getCmp('bCompany_id')
																								.getValue(),
																				Ext
																						.getCmp('bCompany_name')
																						.getValue(),
																				__cxtPath
																						+ '/crmMaint.do?method=getCompanyInfo&companyID='
																						+ Ext
																								.getCmp('bCompany_id')
																								.getValue()
																						+ '&tab=1&path='
																						+ path,
																				'menu-company-info',
																				true);
															},
															'render' : function(
																	c) {
																Ext.QuickTips
																		.register(
																				{
																					target : c
																							.getEl(),
																					text : '双击可显示详细信息'
																				});
															}
														}
													})]
								}, {
									xtype : 'panel',
									layout : 'form',
									columnWidth : 0.33,
									labelAlign : 'right',
									labelWidth : 90,
									items : [{
												xtype : 'hidden',
												id : 'directorID',
												name : 'directorID'
											}, {
												xtype : 'displayfield',
												fieldLabel : '项目经理',
												id : 'directorName',
												name : 'directorName',
												width : 136,
												initEvents : function() {
													this.mon(this.el,
															'dblclick',
															this.onDblClick,
															this);
												},
												onDblClick : function() {
													empId = contractFormPanel
															.getForm()
															.findField('directorID')
															.getValue();
													com.bitc.xmis.util
															.EmpInfoWin(empId,
																	'项目经理信息');
												},
												listeners : {
													'render' : function(c) {
														Ext.QuickTips.register(
																{
																	target : c
																			.getEl(),
																	text : '双击可显示详细信息'
																});
													}
												}
											}, {
												xtype : 'hidden',
												name : 'contractTypeID',
												value : contractType
											}]
								}]
					}, {
						xtype : 'fieldset',
						title : '备注',
						width : 700,
						checkboxToggle : true,
						collapsed : true,
						border : true,
						items : [{
							xtype : 'panel',
							layout : 'column',
							border : false,
							items : [{
								xtype : 'panel',
								border : false,
								layout : 'form',
								columnWidth : 1,
								labelWidth : 80,
								labelAlign : 'right',
								items : [{
											xtype : 'textarea',
											fieldLabel : '备注',
											name : 'remark',
											height : 52,
											width : 580,
											listeners : {
												'render' : function(c) {
													Ext.QuickTips.register({
																target : c
																		.getEl(),
																text : '"备注"最多可输入800字'
															});
												}

											}
										}]
							}]
						}]
					}, {
						xtype : 'panel',
						layout : 'form',
						id : 'splitmoney',
						hidden : false,
						width : 714,
						labelAlign : 'right',
						labelWidth : 90,
						items : [{
							fieldLabel : '合同金额分解',
							xtype : 'radiogroup',
							name : "splittype",
							width : 600,
							items : [{
										name : "moneycheck",
										id : 'moneycheckQuality',
										xtype : 'radio',
										inputValue : 1,
										width : 400,
										boxLabel : "按科质部资质管理标准分解",
										checked : true,
										listeners : {
											'check' : function(c, b) {
												if (b) {
													Ext.getCmp('moneyfieldset')
															.show();
													Ext.getCmp('moneygrid')
															.hide();
												}
											}
										}
									}, {
										name : "moneycheck",
										id : 'moneycheckContract',
										xtype : 'radio',
										inputValue : 2,
										width : 200,
										boxLabel : "按合同文本规定分解",
										listeners : {
											'check' : function(checkbox,
													checked) {
												if (checked) {
													Ext.getCmp('moneygrid')
															.show();
													Ext.getCmp('moneyfieldset')
															.hide();
												}
											}
										}
									}]
						}]
					}, {
						xtype : 'fieldset',
						title : '按科质部资质管理标准分解合同金额',
						width : 700,
						id : 'moneyfieldset',
						hidden : false,
						border : true,
						items : [{
							xtype : 'panel',
							layout : 'column',
							border : false,
							items : [{
								xtype : 'panel',
								layout : 'form',
								columnWidth : 0.5,
								border : false,
								items : [{
									xtype : 'fieldset',
									layout : 'auto',
									width : 300,
									id : 'hardwarePurchaseID',
									title : '硬件购置费用',
									border : true,
									items : [{
										xtype : 'panel',
										layout : 'hbox',
										border : false,
										items : [{
											xtype : 'panel',
											width : 239,
											labelAlign : 'right',
											labelWidth : 80,
											layout : 'form',
											border : false,
											items : [{
												xtype : 'formatnum',
												id : 'selfHardID',
												name : 'selfHard',
												fieldLabel : '自制硬件费用',
												width : 140,
												sum : function() {
													var selfHard = contractFormPanel
															.getForm()
															.findField('selfHard')
															.getValue();
													selfHard = (selfHard == null || selfHard == '')
															? 0
															: parseFloat(selfHard);
													var homemadeHard = contractFormPanel
															.getForm()
															.findField('homemadeHard')
															.getValue();
													homemadeHard = (homemadeHard == null || homemadeHard == '')
															? 0
															: parseFloat(homemadeHard);
													var importHard = contractFormPanel
															.getForm()
															.findField('importHard')
															.getValue();
													importHard = (importHard == null || importHard == '')
															? 0
															: parseFloat(importHard);
													// var sum = selfHard
													// + homemadeHard
													// + importHard;
													var sum = selfHard * 100
															+ homemadeHard
															* 100 + importHard
															* 100;
													sum = sum / 100;
													Ext
															.getCmp('hardwarePurchaseID')
															.setTitle('硬件购置费用（合计：'
																	+ Ext.util.Format
																			.number(
																					sum,
																					'0,000.00')
																	+ ')');
													sum = (sum == null || sum == '')
															? 0
															: parseFloat(sum);
													return sum;
												},
												listeners : {
													'change' : function(field,
															newValue, oldValue) {
														contractFormPanel
																.getForm()
																.findField('selfHard')
																.sum();
														contractFormPanel
																.getForm()
																.findField('contractAmt')
																.sum();
													},
													'render' : function(c) {
														Ext.QuickTips.register(
																{
																	target : c
																			.getEl(),
																	text : '自制硬件费：合同中企业自制的硬件设备（不含嵌入式软件）费用。'
																});
													}
												}
											}]
										}, {
											xtype : 'displayfield',
											width : 45,
											id : 'selfHardCurID',
											name : 'selfHardCur',
											align : 'right'
										}]
									}, {
										xtype : 'panel',
										layout : 'hbox',
										border : false,
										items : [{
											xtype : 'panel',
											width : 239,
											labelAlign : 'right',
											labelWidth : 80,
											layout : 'form',
											border : false,
											items : [{
												xtype : 'formatnum',
												id : 'homemadeHardID',
												name : 'homemadeHard',
												fieldLabel : '国产硬件费',
												width : 140,
												listeners : {
													'change' : function(field,
															newValue, oldValue) {
														contractFormPanel
																.getForm()
																.findField('selfHard')
																.sum();
														contractFormPanel
																.getForm()
																.findField('contractAmt')
																.sum();
													},
													'render' : function(c) {
														Ext.QuickTips.register(
																{
																	target : c
																			.getEl(),
																	text : '外购国产硬件费：合同中企业用于购置在我国境内'
																			+ '开发生产的计算机信息系统使用的硬件设备的费用。'
																});
													}
												}
											}]
										}, {
											xtype : 'displayfield',
											width : 45,
											id : 'homemadeHardCurID',
											name : 'homemadeHardCur',
											align : 'right'
										}]
									}, {
										xtype : 'panel',
										layout : 'hbox',
										border : false,
										items : [{
											xtype : 'panel',
											width : 239,
											labelAlign : 'right',
											labelWidth : 80,
											layout : 'form',
											border : false,
											items : [{
												xtype : 'formatnum',
												id : 'importHardID',
												name : 'importHard',
												fieldLabel : '进口硬件费',
												width : 140,
												listeners : {
													'change' : function(field,
															newValue, oldValue) {
														contractFormPanel
																.getForm()
																.findField('selfHard')
																.sum();
														contractFormPanel
																.getForm()
																.findField('contractAmt')
																.sum();
													},
													'render' : function(c) {
														Ext.QuickTips.register(
																{
																	target : c
																			.getEl(),
																	text : '外购进口硬件费：合同中用于购置在我国境外开发，'
																			+ '以各种形式在我国生产、经营的计算机信息系统使用的'
																			+ '硬件设备的费用。'
																});
													}
												}
											}]
										}, {
											xtype : 'displayfield',
											width : 45,
											id : 'importHardCurID',
											name : 'importHardCur',
											align : 'right'
										}]
									}]
								}]
							}, {
								xtype : 'panel',
								layout : 'form',
								columnWidth : 0.5,
								border : false,
								items : [{
									xtype : 'fieldset',
									layout : 'auto',
									id : 'softwarePurchaseID',
									title : '软件购置费用',
									width : 300,
									border : true,
									items : [{
										xtype : 'panel',
										layout : 'hbox',
										border : false,
										items : [{
											xtype : 'panel',
											width : 239,
											labelAlign : 'right',
											labelWidth : 80,
											layout : 'form',
											border : false,
											items : [{
												xtype : 'formatnum',
												id : 'homemadeSoftID',
												name : 'homemadeSoft',
												fieldLabel : '国产软件费',
												width : 140,
												sum : function() {
													var homemadeSoft = contractFormPanel
															.getForm()
															.findField('homemadeSoft')
															.getValue();
													homemadeSoft = (homemadeSoft == null || homemadeSoft == '')
															? 0
															: parseFloat(homemadeSoft);
													var importSoft = contractFormPanel
															.getForm()
															.findField('importSoft')
															.getValue();
													importSoft = (importSoft == null || importSoft == '')
															? 0
															: parseFloat(importSoft);
													// var sum = homemadeSoft
													// + importSoft;
													var sum = homemadeSoft
															* 100 + importSoft
															* 100;
													sum = sum / 100;
													Ext
															.getCmp('softwarePurchaseID')
															.setTitle('软件购置费用（合计：'
																	+ Ext.util.Format
																			.number(
																					sum,
																					'0,000.00')
																	+ ')');
													sum = (sum == null || sum == '')
															? 0
															: parseFloat(sum);
													return sum;
												},
												listeners : {
													'change' : function(field,
															newValue, oldValue) {
														contractFormPanel
																.getForm()
																.findField('homemadeSoft')
																.sum();
														contractFormPanel
																.getForm()
																.findField('contractAmt')
																.sum();
													},
													'render' : function(c) {
														Ext.QuickTips.register(
																{
																	target : c
																			.getEl(),
																	text : '外购国产软件费：合同中用于购置在我国境内开发生产的软件产品的费用。'
																});
													}
												}
											}]
										}, {
											xtype : 'displayfield',
											width : 45,
											id : 'homemadeSoftCurID',
											name : 'homemadeSoftCur',
											align : 'right'
										}]
									}, {
										xtype : 'panel',
										layout : 'hbox',
										border : false,
										items : [{
											xtype : 'panel',
											width : 239,
											labelAlign : 'right',
											labelWidth : 80,
											layout : 'form',
											border : false,
											items : [{
												xtype : 'formatnum',
												id : 'importSoftID',
												name : 'importSoft',
												fieldLabel : '进口软件费',
												width : 140,
												listeners : {
													'change' : function(field,
															newValue, oldValue) {
														contractFormPanel
																.getForm()
																.findField('homemadeSoft')
																.sum();
														contractFormPanel
																.getForm()
																.findField('contractAmt')
																.sum();
													},
													'render' : function(c) {
														Ext.QuickTips.register(
																{
																	target : c
																			.getEl(),
																	text : '外购进口软件费：合同中用于购置在我国境外开发，'
																			+ '以各种形式在我国生产、经营的软件产品的费用。'
																});
													}
												}
											}]
										}, {
											xtype : 'displayfield',
											width : 45,
											id : 'importSoftCurID',
											name : 'importSoftCur',
											align : 'right'
										}]
									}]
								}]
							}]
						}, {
							xtype : 'panel',
							layout : 'column',
							border : false,
							items : [{
								xtype : 'panel',
								layout : 'form',
								columnWidth : 0.5,
								border : false,
								items : [{
									xtype : 'fieldset',
									id : 'softservicesplit',
									layout : 'auto',
									width : 300,
									title : '软件服务费用',
									border : true,
									items : [{
										xtype : 'panel',
										layout : 'hbox',
										border : false,
										items : [{
											xtype : 'panel',
											width : 239,
											labelAlign : 'right',
											labelWidth : 80,
											layout : 'form',
											border : false,
											items : [{
												xtype : 'formatnum',
												id : 'softwareCostID',
												name : 'softwareCost',
												fieldLabel : '软件开发费用',
												width : 140,
												sum : function() {
													var softwareCost = contractFormPanel
															.getForm()
															.findField('softwareCost')
															.getValue();
													softwareCost = (softwareCost == null || softwareCost == '')
															? 0
															: parseFloat(softwareCost);
													var systemdesignCost = contractFormPanel
															.getForm()
															.findField('systemdesignCost')
															.getValue();
													systemdesignCost = (systemdesignCost == null || systemdesignCost == '')
															? 0
															: parseFloat(systemdesignCost);
													var systemintegrateCost = contractFormPanel
															.getForm()
															.findField('systemintegrateCost')
															.getValue();
													systemintegrateCost = (systemintegrateCost == null || systemintegrateCost == '')
															? 0
															: parseFloat(systemintegrateCost);
													var technical = contractFormPanel
															.getForm()
															.findField('technical')
															.getValue();
													technical = (technical == null || technical == '')
															? 0
															: parseFloat(technical);
													// var sum = softwareCost
													// + systemdesignCost
													// + systemintegrateCost
													// + technical;
													var sum = softwareCost
															* 100
															+ systemdesignCost
															* 100
															+ systemintegrateCost
															* 100 + technical
															* 100;
													sum = sum / 100;
													Ext
															.getCmp('softservicesplit')
															.setTitle('软件服务费用（合计：'
																	+ Ext.util.Format
																			.number(
																					sum,
																					'0,000.00')
																	+ ')');
													sum = (sum == null || sum == '')
															? 0
															: parseFloat(sum);
													return sum;
												},
												listeners : {
													'change' : function(field,
															newValue, oldValue) {
														contractFormPanel
																.getForm()
																.findField('softwareCost')
																.sum();
														contractFormPanel
																.getForm()
																.findField('contractAmt')
																.sum();
													},
													'render' : function(c) {
														Ext.QuickTips.register(
																{
																	target : c
																			.getEl(),
																	text : '软件开发费：合同中用于系统集成、'
																			+ '应用服务等技术服务过程，'
																			+ '根据用户特定需求提供的软件设计与开发服务的费用。'
																});
													}
												}
											}]
										}, {
											xtype : 'displayfield',
											width : 45,
											id : 'softDevCurID',
											name : 'softDevCur',
											align : 'right'
										}]
									}, {
										xtype : 'panel',
										layout : 'hbox',
										border : false,
										items : [{
											xtype : 'panel',
											width : 239,
											labelAlign : 'right',
											labelWidth : 80,
											layout : 'form',
											border : false,
											items : [{
												xtype : 'formatnum',
												id : 'systemdesignCostID',
												name : 'systemdesignCost',
												fieldLabel : '咨询设计费用',// 系统设计费用
												width : 140,
												listeners : {
													'change' : function(field,
															newValue, oldValue) {
														contractFormPanel
																.getForm()
																.findField('softwareCost')
																.sum();
														contractFormPanel
																.getForm()
																.findField('contractAmt')
																.sum();
													},
													'render' : function(c) {
														Ext.QuickTips.register(
																{
																	target : c
																			.getEl(),
																	text : '咨询设计费：合同中用于提出行业、区域或领域的信息化建设方案'
																			+ '（包括信息化远景、目标、战略和总体框架等咨询服务），'
																			+ '协助用户提升和优化信息化管理活动的咨询服务，'
																			+ '以及根据用户实际业务需求提供的信息系统需求分析、'
																			+ '体系结构设计、概要设计、详细设计以及实施方案、测试方案编制等的费用。'
																});
													}
												}
											}]
										}, {
											xtype : 'displayfield',
											width : 45,
											id : 'sysDesCurID',
											name : 'sysDesCur',
											align : 'right'
										}]
									}, {
										xtype : 'panel',
										layout : 'hbox',
										border : false,
										items : [{
											xtype : 'panel',
											width : 239,
											labelAlign : 'right',
											labelWidth : 80,
											layout : 'form',
											border : false,
											items : [{
												xtype : 'formatnum',
												id : 'systemintegrateCostID',
												name : 'systemintegrateCost',
												fieldLabel : '系统集成费用',
												width : 140,
												listeners : {
													'change' : function(field,
															newValue, oldValue) {
														contractFormPanel
																.getForm()
																.findField('softwareCost')
																.sum();
														contractFormPanel
																.getForm()
																.findField('contractAmt')
																.sum();
													},
													'render' : function(c) {
														Ext.QuickTips.register(
																{
																	target : c
																			.getEl(),
																	text : '系统集成是指从事计算机应用系统工程和网络系统工程的咨询设计、'
																			+ '集成实施、运营维护等服务，'
																			+ '涵盖总体策划、设计、开发、实施、服务及保障等环节。'
																});
													}
												}
											}]
										}, {
											xtype : 'displayfield',
											width : 45,
											id : 'sysIntCurID',
											name : 'sysIntCur',
											align : 'right'
										}]
									}, {
										xtype : 'panel',
										layout : 'hbox',
										border : false,
										items : [{
											xtype : 'panel',
											width : 239,
											labelAlign : 'right',
											labelWidth : 80,
											layout : 'form',
											border : false,
											items : [{
												xtype : 'formatnum',
												id : 'technicalID',
												name : 'technical',
												fieldLabel : '运营维护费',// 技术服务费用
												width : 140,
												listeners : {
													'change' : function(field,
															newValue, oldValue) {
														contractFormPanel
																.getForm()
																.findField('softwareCost')
																.sum();
														contractFormPanel
																.getForm()
																.findField('contractAmt')
																.sum();
													},
													'render' : function(c) {
														Ext.QuickTips.register(
																{
																	target : c
																			.getEl(),
																	text : '运行维护费：合同中用于通过采用信息技术手段及方法，'
																			+ '根据用户提出的服务级别要求，'
																			+ '对其所使用的信息系统运行环境、业务系统等提供综合服务的费用。'
																});
													}
												}
											}]
										}, {
											xtype : 'displayfield',
											width : 45,
											id : 'techCurID',
											name : 'techCur',
											align : 'right'
										}]
									}]
								}]
							}, {
								xtype : 'panel',
								layout : 'form',
								columnWidth : 0.5,
								border : false,
								items : [{
									xtype : 'fieldset',
									layout : 'auto',
									id : 'elsemoneysplit',
									title : '其他费用',
									width : 300,
									border : true,
									items : [{
										xtype : 'panel',
										layout : 'hbox',
										border : false,
										items : [{
											xtype : 'panel',
											width : 239,
											labelAlign : 'right',
											labelWidth : 80,
											layout : 'form',
											border : false,
											items : [{
												xtype : 'formatnum',
												id : 'constructCostID',
												name : 'constructCost',
												fieldLabel : '建筑工程费用',
												width : 140,
												sum : function() {
													var contructCost = contractFormPanel
															.getForm()
															.findField('constructCost')
															.getValue();
													contructCost = (contructCost == null || contructCost == '')
															? 0
															: parseFloat(contructCost);
													var managementCost = contractFormPanel
															.getForm()
															.findField('managementCost')
															.getValue();
													managementCost = (managementCost == null || managementCost == '')
															? 0
															: parseFloat(managementCost);
													var transportCost = contractFormPanel
															.getForm()
															.findField('transportCost')
															.getValue();
													transportCost = (transportCost == null || transportCost == '')
															? 0
															: parseFloat(transportCost);
													var taxes = contractFormPanel
															.getForm()
															.findField('taxes')
															.getValue();
													taxes = (taxes == null || taxes == '')
															? 0
															: parseFloat(taxes);
													var sum = 0;
													// sum = contructCost
													// + managementCost
													// + transportCost
													// + taxes;
													sum = contructCost * 100
															+ managementCost
															* 100
															+ transportCost
															* 100 + taxes * 100;
													sum = sum / 100;
													Ext
															.getCmp('elsemoneysplit')
															.setTitle('其他费用（合计：'
																	+ Ext.util.Format
																			.number(
																					sum,
																					'0,000.00')
																	+ ')');
													sum = (sum == null || sum == '')
															? 0
															: parseFloat(sum);
													return sum;
												},
												listeners : {
													'change' : function(field,
															newValue, oldValue) {
														contractFormPanel
																.getForm()
																.findField('constructCost')
																.sum();
														contractFormPanel
																.getForm()
																.findField('contractAmt')
																.sum();
													}
												}

											}]
										}, {
											xtype : 'displayfield',
											width : 45,
											id : 'constructCurID',
											name : 'constructCur',
											align : 'right'
										}]
									}, {
										xtype : 'panel',
										layout : 'hbox',
										border : false,
										items : [{
											xtype : 'panel',
											width : 239,
											labelAlign : 'right',
											labelWidth : 80,
											layout : 'form',
											border : false,
											items : [{
												xtype : 'formatnum',
												id : 'managementCostID',
												name : 'managementCost',
												fieldLabel : '管理费',
												width : 140,
												listeners : {
													'change' : function(field,
															newValue, oldValue) {
														contractFormPanel
																.getForm()
																.findField('constructCost')
																.sum();
														contractFormPanel
																.getForm()
																.findField('contractAmt')
																.sum();
													}
												}
											}]
										}, {
											xtype : 'displayfield',
											width : 45,
											id : 'managementCurID',
											name : 'managementCur',
											align : 'right'
										}]
									}, {
										xtype : 'panel',
										layout : 'hbox',
										border : false,
										items : [{
											xtype : 'panel',
											width : 239,
											labelAlign : 'right',
											labelWidth : 80,
											layout : 'form',
											border : false,
											items : [{
												xtype : 'formatnum',
												id : 'transportCostID',
												name : 'transportCost',
												fieldLabel : '运费',
												width : 140,
												listeners : {
													'change' : function(field,
															newValue, oldValue) {
														contractFormPanel
																.getForm()
																.findField('constructCost')
																.sum();
														contractFormPanel
																.getForm()
																.findField('contractAmt')
																.sum();
													}
												}
											}]
										}, {
											xtype : 'displayfield',
											width : 45,
											id : 'transportCurID',
											name : 'transportCur',
											align : 'right'
										}]
									}, {
										xtype : 'panel',
										layout : 'hbox',
										border : false,
										items : [{
											xtype : 'panel',
											width : 239,
											labelAlign : 'right',
											labelWidth : 80,
											layout : 'form',
											border : false,
											items : [{
												xtype : 'formatnum',
												id : 'taxesID',
												name : 'taxes',
												fieldLabel : '税金',
												width : 140,
												listeners : {
													'change' : function(field,
															newValue, oldValue) {
														contractFormPanel
																.getForm()
																.findField('constructCost')
																.sum();
														contractFormPanel
																.getForm()
																.findField('contractAmt')
																.sum();
													}
												}
											}]
										}, {
											xtype : 'displayfield',
											width : 45,
											id : 'taxesCurID',
											name : 'taxesCur',
											align : 'right'
										}]
									}]
								}]
							}]
						}]
					}, {
						xtype : 'fieldset',
						title : '按合同文本规定分解合同金额',
						width : 700,
						id : 'moneygrid',
						hidden : true,
						border : true,
						items : [conTextGridPanel]
					}, {
						xtype : 'panel',
						width : 714,
						layout : 'column',
						items : [{
							xtype : 'panel',
							layout : 'form',
							labelAlign : 'right',
							labelWidth : 90,
							columnWidth : 0.53,
							border : false,
							items : [new com.bitc.xmis.Combos.ConInfoObjCombo({
								fieldLabel : '项目来源',
								hiddenName : 'projectResource',
								width : 270,
								emptyText : '请选择项目来源',
								dataUrl : '/contractMain.do?method=getContractInfoByType',
								baseParams : {
									type : 2
								},
								listeners : {
									'afterrender' : function(cmp) {
										Ext.getCmp('countryPro').show();
									},
									'nextlevelLoad' : function(f) {
										if (f.getValue() == -1) {
											// Ext.getCmp('countryPro').hide();
											contractFormPanel
													.getForm()
													.findField('projectResourceInfoCon')
													.getStore().removeAll();
											contractFormPanel
													.getForm()
													.findField('projectResourceInfoCon')
													.getStore().setBaseParam(
															'parentID',
															f.getValue());
											contractFormPanel
													.getForm()
													.findField('projectResourceInfoCon')
													.getStore().load();
										} else {
											contractFormPanel
													.getForm()
													.findField('projectResourceInfoCon')
													.getStore().setBaseParam(
															'parentID',
															f.getValue());
											contractFormPanel
													.getForm()
													.findField('projectResourceInfoCon')
													.getStore().load();
										}
									},
									'select' : function(combo, record, index) {
										contractFormPanel
												.getForm()
												.findField('projectResourceInfoCon')
												.setValue(null);
										if (record.get('ID') == -1) {
											contractFormPanel
													.getForm()
													.findField('projectResourceInfoCon')
													.getStore().removeAll();
											contractFormPanel
													.getForm()
													.findField('projectResourceInfoCon')
													.getStore().setBaseParam(
															'parentID',
															record.get('ID'));
											// contractFormPanel
											// .getForm()
											// .findField('projectResourceInfoCon')
											// .getStore().load();
										} else {
											if (contract != null
													&& contract.contractInfoObj.projectResourceParentID == combo.value) {
												contractFormPanel
														.getForm()
														.findField('projectResourceInfoCon')
														.setValue((contract.contractInfoObj.projectResourceID != 0)
																? contract.contractInfoObj.projectResourceID
																: '');
											}
											contractFormPanel
													.getForm()
													.findField('projectResourceInfoCon')
													.getStore().setBaseParam(
															'parentID',
															record.get('ID'));
											contractFormPanel
													.getForm()
													.findField('projectResourceInfoCon')
													.getStore().load();
										}

									}
								}
							})]
						}, {
							xtype : 'panel',
							id : 'countryPro',
							columnWidth : 0.45,
							border : false,
							items : [new com.bitc.xmis.Combos.ConInfoObjCombo({
								hiddenName : 'projectResourceInfoCon',
								width : 270,
								emptyText : '请选择项目来源',
								dataUrl : '/contractMain.do?method=getContractInfoByParentID'
							})]
						}]
					}, {
						xtype : 'panel',
						width : 714,
						layout : 'column',
						items : [{
							xtype : 'panel',
							layout : 'form',
							labelAlign : 'right',
							labelWidth : 90,
							columnWidth : 0.53,
							border : false,
							items : [new com.bitc.xmis.Combos.ConInfoObjCombo({
								fieldLabel : '项目合作形式',
								hiddenName : 'projectOperationType',
								width : 270,
								emptyText : '请选择合作形式',
								dataUrl : '/contractMain.do?method=getContractInfoByType',
								baseParams : {
									type : 3
								},
								listeners : {
									'afterrender' : function(cmp) {
										Ext.getCmp('operationType').show();
									},
									'nextlevelLoad' : function(f) {
										if (f.getValue() == -1) {
											contractFormPanel
													.getForm()
													.findField('projectOperation')
													.getStore().removeAll();
										} else {
											contractFormPanel
													.getForm()
													.findField('projectOperation')
													.getStore().setBaseParam(
															'parentID',
															f.getValue());
											contractFormPanel
													.getForm()
													.findField('projectOperation')
													.getStore().load();
										}
									},
									'select' : function(combo, record, index) {
										contractFormPanel.getForm()
												.findField('projectOperation')
												.setValue(null);

										if (record.get('ID') == -1) {
											contractFormPanel
													.getForm()
													.findField('projectOperation')
													.getStore().removeAll();
											Ext.getCmp('operationType').show();
											contractFormPanel
													.getForm()
													.findField('projectOperation')
													.getStore().setBaseParam(
															'parentID',
															record.get('ID'));
										} else {
											if (contract != null
													&& contract.contractInfoObj.projectOperationTypeParentID == combo.value) {
												contractFormPanel
														.getForm()
														.findField('projectOperation')
														.setValue((contract.contractInfoObj.projectOperationTypeID != 0)
																? contract.contractInfoObj.projectOperationTypeID
																: '');
											}
											if (record.get('ID') == 8) {
												Ext.getCmp('operationType')
														.hide();
											} else {
												Ext.getCmp('operationType')
														.show();
												contractFormPanel
														.getForm()
														.findField('projectOperation')
														.getStore()
														.setBaseParam(
																'parentID',
																record
																		.get('ID'));
												contractFormPanel
														.getForm()
														.findField('projectOperation')
														.getStore().load();
											}
										}
									}
								}
							})]
						}, {
							xtype : 'panel',
							id : 'operationType',
							columnWidth : 0.45,
							border : false,
							items : [new com.bitc.xmis.Combos.ConInfoObjCombo({
								hiddenName : 'projectOperation',
								width : 270,
								emptyText : '请选择合作形式',
								dataUrl : '/contractMain.do?method=getContractInfoByParentID'
							})]
						}]
					}, {
						xtype : 'panel',
						layout : 'column',
						width : 714,
						items : [{
							xtype : 'panel',
							layout : 'form',
							columnWidth : 0.53,
							labelAlign : 'right',
							labelWidth : 90,
							border : false,
							items : [new com.bitc.xmis.Combos.ConInfoObjCombo({
								fieldLabel : '项目性质',
								hiddenName : 'projectProperty',
								width : 270,
								emptyText : '请选择项目性质',
								dataUrl : '/contractMain.do?method=getContractInfoByType',
								baseParams : {
									type : 4
								}
							})]
						}]
					}, {
						xtype : 'panel',
						layout : 'column',
						width : 714,
						items : [{
							xtype : 'panel',
							layout : 'form',
							labelAlign : 'right',
							labelWidth : 90,
							columnWidth : 0.52,
							border : false,
							items : [new com.bitc.xmis.Combos.ConInfoObjCombo({
								fieldLabel : '核算机构',
								hiddenName : 'accountingAgency',
								width : 270,
								emptyText : '请选择核算机构',
								dataUrl : '/contractMain.do?method=getContractInfoByType',
								baseParams : {
									type : 5
								},

								listeners : {
									'afterrender' : function(cmp) {
										if (contract == null) {
											Ext.getCmp('executiveDept').hide();
											Ext.getCmp('executiveAgency')
													.hide();
										} else if (contract.accountingAgency == 12) {
											Ext.getCmp('executiveDept').hide();
											Ext.getCmp('executiveAgency')
													.show();
										} else {
											Ext.getCmp('executiveDept').show();
											Ext.getCmp('executiveAgency')
													.hide();
										}
									},
									'nextlevelLoad' : function(f) {
										if (f.getValue() == -1
												|| f.getValue() == 0) {
											Ext.getCmp('executiveDept').hide();
											Ext.getCmp('executiveAgency')
													.hide();
										} else if (f.getValue() == 13) {
											contractFormPanel
													.getForm()
													.findField('executiveComDept')
													.setValue(contract.bDept_name);
											Ext.getCmp('executiveAgency')
													.hide();
											Ext.getCmp('executiveDept').show();
										} else {
											Ext.getCmp('executiveAgency')
													.show();
											Ext.getCmp('executiveDept').hide();
											contractFormPanel.getForm()
													.findField('executiveOrg')
													.loadValue();
										}
									},
									'select' : function(combo, record, index) {
										if (record.get('ID') == -1) {
											Ext.getCmp('executiveDept').hide();
											Ext.getCmp('executiveAgency')
													.hide();
										} else if (record.get('ID') == 12) {
											Ext.getCmp('executiveDept').hide();
											Ext.getCmp('executiveAgency')
													.show();
											contractFormPanel.getForm()
													.findField('executiveOrg')
													.loadValue();
										} else {
											Ext.getCmp('executiveAgency')
													.hide();
											Ext.getCmp('executiveDept').show();
											contractFormPanel
													.getForm()
													.findField('executiveComDept')
													.setValue(contractFormPanel
															.getForm()
															.findField('bDept_name')
															.getValue());
										}
									}
								}

							})]
						}, {
							xtype : 'panel',
							layout : 'form',
							labelAlign : 'right',
							id : 'executiveAgency',// 在院核算
							columnWidth : 0.45,
							labelWidth : 60,
							border : false,
							items : [{
										xtype : 'hidden',
										name : 'executiveOrgID'
									}, {
										xtype : 'displayfield',
										fieldLabel : '执行单位',
										id : 'executiveOrg',
										name : 'executiveOrg',
										width : 212,
										loadValue : function() {
											if (contractFormPanel.getForm()
													.findField('bDept_id')
													.getValue()) {
												Ext.Ajax.request({
													url : __cxtPath
															+ '/contractMain.do?method=getScientificName',// 研究所
													params : {
														deptId : contractFormPanel
																.getForm()
																.findField('bDept_id')
																.getValue()
													},
													scope : this,
													success : function(response) {
														var result = Ext
																.decode(response.responseText);
														if (result.success) {
															if (result.rows[0]) {
																contractFormPanel
																		.getForm()
																		.findField('executiveOrgID')
																		.setValue(result.rows[0].ID);
																contractFormPanel
																		.getForm()
																		.findField('executiveOrg')
																		.setRawValue(result.rows[0].name);
															}
														}
													},
													failure : function() {
													}
												});
											}
										}
									}]
						}, {
							xtype : 'panel',
							layout : 'form',
							id : 'executiveDept',// 在公司核算
							columnWidth : 0.45,
							labelWidth : 60,
							border : false,
							items : [{
										xtype : 'displayfield',
										fieldLabel : '执行单位',
										name : 'executiveComDept',
										width : 212
									}]
						}]
					}, {
						xtype : 'panel',
						layout : 'column',
						width : 714,
						defaults : {
							border : false
						},
						items : [{
							xtype : 'panel',
							layout : 'form',
							columnWidth : 0.25,
							labelAlign : 'right',
							labelWidth : 90,
							items : [{
								xtype : 'displayfield',
								fieldLabel : '登记人',
								name : 'registerName',
								id : 'registerName',
								value : empName,
								initEvents : function() {
									this.mon(this.el, 'dblclick',
											this.onDblClick, this);
								},
								onDblClick : function() {
									com.bitc.xmis.util.EmpInfoWin(RegisterID,
											'登记人信息');
								},
								listeners : {
									'render' : function(c) {
										Ext.QuickTips.register({
													target : c.getEl(),
													text : '双击可显示详细信息'
												});
									}
								}
							}]
						}, {
							xtype : 'panel',
							layout : 'form',
							columnWidth : 0.22,
							labelWidth : 55,
							labelAlign : 'right',
							items : [{
										xtype : 'displayfield',
										fieldLabel : '登记日期',
										id : 'updateDate',
										name : 'updateDate',
										value : new Date().format('Y-m-d')
									}]
						}, {
							xtype : 'panel',
							layout : 'form',
							columnWidth : 0.25,
							labelAlign : 'right',
							labelWidth : 80,
							id : 'lastname',
							hidden : true,
							items : [{
								xtype : 'displayfield',
								fieldLabel : '最后修改人',
								id : 'lastModificatorName',
								name : 'lastModificatorName',
								initEvents : function() {
									this.mon(this.el, 'dblclick',
											this.onDblClick, this);
								},
								onDblClick : function() {
									com.bitc.xmis.util.EmpInfoWin(
											lastModificator, '最后修改人信息');
								},
								listeners : {
									'render' : function(c) {
										Ext.QuickTips.register({
													target : c.getEl(),
													text : '双击可显示详细信息'
												});
									}
								}
							}]
						}, {
							xtype : 'panel',
							layout : 'form',
							columnWidth : 0.25,
							labelWidth : 90,
							labelAlign : 'right',
							id : 'lastdate',
							hidden : true,
							items : [{
										xtype : 'displayfield',
										fieldLabel : '最后修改日期',
										id : 'lastModificationDate',
										name : 'lastModificationDate'
									}]
						}]
					}]
		}]
	});

	// addressFormPanel.getForm().findField('areaID').on('change',
	// function(ms, value) {
	// addressFormPanel.getForm().findField('provinceID').store.load({
	// params : {
	// fid : value
	// }
	// });
	// });
	//
	// addressFormPanel.getForm().findField('provinceID').on('change',
	// function(ms, value) {
	// addressFormPanel.getForm().findField('cityID').store.load({
	// params : {
	// fid : value
	// }
	// });
	// });

	var relatedStore = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
					url : __cxtPath
							+ '/contractMain.do?method=getRelatedContractList'
				}),
		remoteSort : true,
		sortInfo : {
			field : DBSchema.ContractRelated.Amt,
			direction : 'DESC'
		},
		reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount',
					fields : [{
								name : DBSchema.ContractRelated.IncomeContractID
							}, {
								name : DBSchema.ContractRelated.OutContractID
							}, {
								name : DBSchema.ContractRelated.Amt,
								type : 'number'
							}, {
								name : DBSchema.ContractRelated.Remark
							}, {
								name : DBSchema.ContractRelated.IncomeContractName
							}, {
								name : DBSchema.ContractRelated.OutContractName
							}, {
								name : DBSchema.ContractRelated.OutContractDate
							}, {
								name : DBSchema.ContractRelated.Paymented,
								type : 'number'
							}, {
								name : DBSchema.ContractRelated.outContractNo
							}, {
								name : DBSchema.ContractRelated.OutSubContractID
							}]
				})
	});

	relatedStore.on('load', function(store, records, options) {
				var glIncomeAmt = 0.0;

				eastPanel.setTitle("对应支出合同:");
			}, this, {
				delay : 100
			});

	var relatedRenderer = function(value, metaData, record, rowIndex, colIndex,
			store) {
		var conId = (colIndex == 1 ? record
				.get(DBSchema.ContractRelated.IncomeContractID) : record
				.get(DBSchema.ContractRelated.OutContractID));
		var conName = (colIndex == 1 ? record
				.get(DBSchema.ContractRelated.IncomeContractName) : record
				.get(DBSchema.ContractRelated.OutContractName));
		return '<a href="javascript:;" onclick="turnToEditContract(\'' + conId
				+ '\',\'' + conName + '\')">' + value + '</a>';
	};

	var relatedCM = new Ext.grid.ColumnModel([{
				header : '支出合同ID',
				hidden : true,
				sortable : true,
				dataIndex : DBSchema.ContractRelated.OutContractID
			}, {
				header : '支出合同编码',
				hidden : true,
				width : 150,
				sortable : true,
				dataIndex : DBSchema.ContractRelated.outContractNo
			}, {
				header : '支出合同名称',
				dataIndex : DBSchema.ContractRelated.OutContractName,
				width : 150,
				sortable : true,
				renderer : function(value, metaData, record, rowIndex,
						colIndex, store) {
					if (0 == rowIndex) {
						return '<font color=red><b>合计信息</b></font>';
					} else {
						var conId = (colIndex == 1
								? record
										.get(DBSchema.ContractRelated.IncomeContractID)
								: record
										.get(DBSchema.ContractRelated.OutContractID));
						var conName = (colIndex == 1
								? record
										.get(DBSchema.ContractRelated.IncomeContractName)
								: record
										.get(DBSchema.ContractRelated.OutContractName));
						return '<a href="javascript:;" onclick="turnToEditContract(\''
								+ conId
								+ '\',\''
								+ conName
								+ '\')">'
								+ value
								+ '</a>';
					}
				}
			}, {
				header : '签订日期',
				width : 100,
				align : 'right',
				sortable : true,
				dataIndex : DBSchema.ContractRelated.OutContractDate,
				renderer : function(value, metaData, record, rowIndex,
						colIndex, store) {
					return value != null ? value.substr(0, 11) : null;
				}
			}, {
				header : '对应金额',
				align : 'right',
				width : 100,
				sortable : true,
				dataIndex : DBSchema.ContractRelated.Amt,
				renderer : function(value, metaData, record, rowIndex,
						colIndex, store) {
					if (!decisionFlag) {
						return formatCurrency("", '0', "");
					} else {
						return formatCurrency("", value, "");
					}
				}
			}, {
				header : '已付合同款',
				align : 'right',
				width : 100,
				sortable : true,
				dataIndex : DBSchema.ContractRelated.Paymented,
				renderer : function(value, metaData, record, rowIndex,
						colIndex, store) {
					if (0 == rowIndex) {
						if (value.length == 0) {
							value = '0';
						}
						value = formatCurrency("", value, "");
						if (!decisionFlag) {
							return '--';
						} else {
							return value == '--' ? '0.00' : value;
						}
					} else {
						if (!decisionFlag) {
							return '--';
						} else {
							return (value == 0
									? "0.00"
									: "<a href=# id=paymentListLink_"
											+ record.get('支出合同ID')
											+ " onClick=showPayment("
											+ record.get('支出子合同ID')
											+ ",\'"
											+ record.get('支出合同编码')
											+ "\',\'"
											+ record.get('支出合同名称')
											+ "\',"
											+ value
											+ ") >"
											+ formatCurrency('', value + '',
													Color_Font_Contract_Total_Charged_Amt))
									+ (value == 0 ? "" : "</a>");
						}
					}
				}
			}, {
				header : '备注',
				sortable : true,
				dataIndex : DBSchema.ContractRelated.Remark,
				renderer : function(value, cellMeta, record, rowIndex,
						columnIndex, store) {
					if (value != null) {
						var baseNum = 15;
						var msg = '';
						for (var i = 0; i < value.length / baseNum; i++) {
							msg += value.substr(i * baseNum, baseNum) + "<br/>";
						}
						cellMeta.attr = 'ext:qtitle="备注" ext:qtip="' + msg
								+ '"';
					}
					return value;
				}
			}]);

	/**
	 * 对应合同
	 */
	var relatedGrid = new Ext.grid.GridPanel({
				cm : relatedCM,
				store : relatedStore,
				title : '对应合同',
				autoScroll : true,
				stripeRows : true,
				// height : Ext.getBody().getHeight() * 0.6,
//				height : 200,
				region : 'north',
				split : true,
				stateful : true,
				stateId : 'editContract-relatedGrid',
				loadMask : true,
				forceFit : true,
//				margins : '0 0 5 0',
				viewConfig : {
					getRowClass : function(record, rowIndex, rowParams, store) {
						if (0 == rowIndex) {
							return 'grid-contract-total-list-yellow';
						}
					}
				}
			});
	
	/**
	 * 收款计划
	 * */
		var collectionCm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({
												width : 20
											}),{
												header : '项目状态ID',
												dataIndex : '对应项目状态',
												hidden : true,
//												width : 20,
												sortable : true
											},{
												header : '对应项目节点',
												dataIndex : '项目状态',
												sortable : false
											},
											{
												header : '收款节点名称',
												dataIndex : '收款节点名称',
												sortable : false
											},
											{
												header : '收款条件描述',
												dataIndex : '收款条件描述',
												sortable : false
											},
											{
												xtype : 'numbercolumn',
												header : '收款百分比',
												align : 'right',
												width : 90,
												format : '00.00%',
												dataIndex : '收款百分比',
												sortable : false
											},
											{
												xtype : 'numbercolumn',
												align : 'right',
												format : '0,000.00',
												dataIndex : '收款金额',
												header : '预计到款金额',
												sortable : false
											},
											{
												xtype : 'numbercolumn',
												align : 'right',
												format : '0,000.00',
												header : '财务实际到款',
												dataIndex : 'realIncome',
												sortable : false
											},
											{
												header : '延迟天数',
												align : 'right',
												width : 75,
												dataIndex : '延迟天数'
											},
											{
												header : '预计到款时间',
												align : 'center',
												width : 85,
												dataIndex : '预计到款日期'
											},
											{

												header : '节点实际进入日期',
												align : 'center',
												width : 110,
												dataIndex : '实际开始日期'
											},
											{
												header : '应收款日期',
												align : 'center',
												width : 85,
												dataIndex : '应收款日期'
											} 
											]);

			var collectionreRecord = Ext.data.Record.create([{
																"mapping":"对应项目状态",
																"name":"对应项目状态"
															},{
																"mapping":"项目状态",
																"name":"项目状态"
															},{
																"mapping":"收款节点名称",
																"name":"收款节点名称"
															},{
																"mapping":"收款条件描述",
																"name":"收款条件描述"
															},{
																"mapping":"收款百分比",
																"name":"收款百分比"
															},{
																"mapping":"收款金额",
																"name":"收款金额"
															},{
																"mapping":"realIncome",
																"name":"realIncome"
															},{
																"mapping":"延迟天数",
																"name":"延迟天数"
															},{
																"mapping":"预计到款日期",
																"name":"预计到款日期"
															},{
																"mapping":"实际开始日期",
																"name":"实际开始日期"
															},{
																"mapping":"应收款日期",
																"name":"应收款日期"
															}]);
			var collectionStore = new Ext.data.Store({
				url : __cxtPath
						+ '/contractMain.do?method=getPaymentConditionList',
//				pruneModifiedRecords : true,
				reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount'
				}, collectionreRecord),
				remoteSort : true,
				baseParams : {
					contractID :  contract == null ? 0 : contract.contractID//contract.contractID
				},
				autoLoad : true,// (contract != null),
				listeners : {
					'load' : function(s, records) {
						var receivables = 0.00;
						var sumReceivables = 0.00;
						for(var i=0;i<records.length;i++){
							receivables = receivables+(records[i].data.realIncome*1)
//							sumReceivables += receivables*1
						}
						var gl = undefined;
//								gl = gls;
								if(gl=='null')
									gl = contract.incomAmtTotalByGl;
						Ext.getCmp('collectionGrid')
						.getTopToolbar()
						.add('<span style="float:right"><font color=green>已收合同款:'
								+ Ext.util.Format
								.number(gl,'0,000.00')
						+ '</font>&nbsp;&nbsp;<font color=red>按进度应收账款:'
						+ Ext.util.Format.number('0,000.00')
						+ '</font></span>');
				Ext.getCmp('collectionGrid').doLayout();
						}
					}
				});
		/**
		 * A = 按进度预计来款合计 ：项目节点的实际进入日期小于等于当前日期的所有节点的预计来款金额总和。
			C = 财务来款
			按进度应收账款
			按进度应收账款 = A-C，若A<C，按进度应收账款 = 0
		 * */	
			
			var collectionGrid = new Ext.grid.GridPanel({
				id:'collectionGrid',
				cm : collectionCm,
				store : collectionStore,
				title : '收款计划',
				region : 'north',
				loadMask : true,
				forceFit : true,
				viewConfig : {
					getRowClass : function(record, rowIndex, rowParams, store) {
						if (0 == rowIndex) {
							return 'grid-contract-total-list-yellow';
						}
					}
				},
				tbar : []
			});
			


			
	var deferPaymentCm =  new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({
				width : 23
			}),{header : "推迟原因",
			width : 200,
			dataIndex : delayReason
		}, {
			header : "经办人",
			width : 120,
			dataIndex : OperatorName
		}, {
			header : "登记日期",
			width : 80,
			align : 'center',
			dataIndex : DateOfRegistration
		}, {
			header : "登记人",
			width : 115,
			dataIndex : RegistrantName
		} 
			]);
			var ID = "ID";
			var ContractID = "合同ID";
			var delayReason = "原因";
			var OperatorID = "经办人ID";
			var OperatorName = "经办人";
			var DateOfRegistration = "登记日期";
			var Registrant = "登记人ID";
			var RegistrantName = "登记人";
			
//			var conOpType;//合同的操作类型，判断是否有‘双击编辑’权限  conOpType=1，没有权限

		var delayPaymentRecord = Ext.data.Record.create( [ {
														name : delayReason
													}, {
														name : OperatorName
													}, {
														name : DateOfRegistration
													}, {
														name : ID
													}, {
														name : ContractID
													}, {
														name : OperatorID
													}, {
														name : Registrant
													}, {
														name : RegistrantName
													} ]);
		var deferPaymentStore =  new Ext.data.Store({
			url : __cxtPath
							+ '/contractMain.do?method=getDelayReasonListByContractID',
							reader : new Ext.data.JsonReader({
								root : 'rows',
								totalProperty : 'totalCount'
							}, delayPaymentRecord),
							remoteSort : true,
							baseParams : {
								contractID : contract == null ? 0 : contract.contractID
							},
							autoLoad : true
			
		});	
		/**
		 * 付款推迟原因登记
		 * */	
				var deferPaymentGrid = new Ext.grid.GridPanel({
					id:'deferPaymentGrid',
					cm : deferPaymentCm,
					store : deferPaymentStore,
					title : '付款推迟原因',
					autoScroll : true,
					stripeRows : true,
					region : 'north',
					split : true,
					stateful : true,
					loadMask : true,
					forceFit : true
//					,
//					viewConfig : {
//						getRowClass : function(record, rowIndex, rowParams, store) {
//							if (0 == rowIndex) {
//								return 'grid-contract-total-list-yellow';
//							}
//						}
//					}
				});		
	

	var eastPanel = new Ext.TabPanel({
				region : 'east',
				width : 550,
				autoScroll : true,
//				title : '',
				split : true,
				collapsible : true, // 是否有右上方展开收缩条
				collapseMode : 'mini', // 将滚动收缩条模式‘迷你’
//				layout : 'border',
				stateful : true,
				stateId : 'editContract-eastpanel',
				stateEvent : ['resize'],
				activeTab: 0,
				// items : contract == null ? [attGrid] : [relatedGrid, attGrid]
				items : [relatedGrid,collectionGrid,deferPaymentGrid, attGrid]
//				title : '支出合同'
			});
	eastPanel.set
	var mainPanel = new Ext.Panel({
		region : 'center',
		border : false,
		layout : 'border',
		items : [contractFormPanel, eastPanel],
		tbar : {
			xtype : 'toolbar',
			disabled : true,
			items : [{
				ref : '../saveBtn',
				xtype : 'button',
				text : '保存',
				iconCls : 'ok',
				saveFun : function(summarysucess) {
					var quaSplitSuccess = false;// 按科质部标准分解合同
					var quaSum = 0;// 按科质部标准分解合同 金额总额

					var quaSum = contractFormPanel.getForm()
							.findField('contractAmt').sum();
					var quaBalance = parseFloat(contractFormPanel.getForm()
							.findField('contractAmt').getValue())
							- parseFloat(quaSum);

					var textSplitSuccess = false;// 按合同文本分解合同
					var editsuccess = false; // 按合同文本分解合同金额 编辑是否正确
					var texSum = 0; // 按合同文本分解合同金额 金额总额
					for (i = 0; i < conTextStore.getCount(); i++) {
						var r = conTextStore.getAt(i);
						if (Ext.isEmpty(r.get('costName'))
								|| Ext.isEmpty(r.get('costMoney'))) {
							if (Ext.isEmpty(r.get('costName'))
									&& !Ext.isEmpty(r.get('costMoney'))) {
								Ext.Msg.alert("提示", "按合同文本规定分解合同金额，第" + (i + 1)
												+ "行没有编辑费用名称");
								return;
							} else if (!Ext.isEmpty(r.get('costName'))
									&& Ext.isEmpty(r.get('costMoney'))) {
								Ext.Msg.alert("提示", "按合同文本规定分解合同金额，第" + (i + 1)
												+ "行没有编辑金额");
								return;
							} else if (!Ext.isEmpty(r.get('costDesc'))) {
								Ext.Msg.alert("提示", "按合同文本规定分解合同金额，第" + (i + 1)
												+ "行费用名称及金额均未编辑");
								return;
							} else {
								conTextStore.remove(r);
								i--;
								editsuccess = true;
							}
						} else {
							editsuccess = true;
						}
					}
					conTextStore.each(function(record) {
								texSum += (record.get('costMoney') == null || record
										.get('costMoney') == '') ? 0 : record
										.get('costMoney')
										* 100;
							});
					texSum = texSum / 100;

					if (editsuccess) {
						if (texSum > Ext.getCmp('contractAmt').value) {
							Ext.Msg
									.alert(
											"提示",
											"按合同文本规定分解合同金额，金额超出"
													+ Ext.util.Format
															.number(
																	(texSum - Ext
																			.getCmp('contractAmt').value),
																	'0,000.00')
													+ '，请正确分解金额');
						} else if (texSum < Ext.getCmp('contractAmt').value) {
							Ext.Msg
									.alert(
											"提示",
											"按合同文本规定分解合同金额，还有"
													+ Ext.util.Format
															.number(
																	(Ext
																			.getCmp('contractAmt').value - texSum),
																	'0,000.00')
													+ '未分解，请继续分解');
						} else {
							textSplitSuccess = true;
						}
					}

					if (!Ext.isEmpty(contract)
							&& Ext.getCmp('contractAmt').value >= 800000
							&& quaSum == 0) {
						Ext.Msg.alert('提示', '请按科质部资质管理标准将合同金额分解');
					}

					if (quaSum != 0) {
						if (quaBalance > 0) {
							Ext.Msg.alert('提示', '按科质部资质管理标准分解合同金额，还有'
											+ Ext.util.Format.number(
													quaBalance, '0,000.00')
											+ '未分解，请继续分解');
						} else if (quaBalance < 0) {
							Ext.Msg.alert('提示', '按科质部资质管理标准分解合同金额，金额超出'
											+ (Ext.util.Format.number(
													(-quaBalance), '0,000.00'))
											+ '，请正确分解');
						} else {
							quaSplitSuccess = true;
						}
					}

					if (!Ext.isEmpty(contract)
							&& Ext.getCmp('contractAmt').value < 800000
							&& quaSum == 0) {
						quaSplitSuccess = true;
					}

					if (!Ext.isEmpty(contract) && texSum == 0) {
						textSplitSuccess = true;
					}

					var projectCodeSuccess = false;

					if ((summarysucess && quaSplitSuccess && textSplitSuccess)
							|| contract == null
							|| (summarysucess && hasContractControlOnCompanyLevel)) {
						contractFormPanel.getForm().findField('contractAmt')
								.setRawValue(contractFormPanel.getForm()
										.findField('contractAmt').getValue());
						var summaryType = contractFormPanel.getForm()
								.findField('summarycheck1').getValue();

						var refreshflag = false;
						if (contract == null) {
							refreshflag = true;
						}

						if (contract != null && contract.stencil == null) {
							summaryType = 2;
						}
						if (contract == null && stencilKeyWords == null) {
							summaryType = 2;
						}
						if (summaryType == 1) { // 按模版保存
							var isEmpty = true;
							var keys = stencilKeyWords.split(',');
							var summary = '';
							for (var i = 0; i < keys.length; i++) {
								var msg = keys[i].split(';');
								var title = msg[0];
								var fieldType = msg[1];
								var val = Ext.getCmp('stencilParam' + i)
										.getValue();
								if ((val + '').trim() != '') {
									isEmpty = false;
								}
								if (fieldType == '3') {
									val = Ext.util.Format.date(val, 'Y-m-d');
								}
								val = '<$^&' + val + '&^$>';
								if (i == 0) {
									summary += val;
								} else {
									summary += ',' + val;
								}
							}
							contractFormPanel.getForm()
									.findField('stencilContent')
									.setValue(summary);
							var storeStr = com.bitc.xmis.util.ExtUtil
									.encodeStoreModifiedData(conTextStore);
							contractFormPanel.getForm().submit({
								url : __cxtPath + '/contractMain.do',
								params : {
									method : 'saveContractInfoNew',
									warrantyUnit : Ext
											.getCmp("contractFormPanel")
											.getForm()
											.findField('warrantyUnit')
											.getValue(),
									validityUnit : Ext
											.getCmp("contractFormPanel")
											.getForm()
											.findField('validityUnit')
											.getValue(),
									splitMoneyRecords : storeStr,
									projectNumberExist : contract
											? contract.projectOBJ.projectCode
											: ''
								},
								waitMsg : '正在保存，请稍候……',
								waitTitle : '正在处理',
								success : function(form, action) {
									var mess = action.result.message;
									var proCode = action.result.attributes.projectNumber;
									if (action.result.attributes.existflag == true) {
										mess = "'"
												+ Ext
														.decode(action.result.attributes.json)[0]['项目编码']
												+ "'已被"
												+ Ext
														.decode(action.result.attributes.json)[0]['合同名称']
												+ "合同在"
												+ (Ext
														.decode(action.result.attributes.json)[0]['最后修改日期'].year + 1900)
												+ "年"
												+ (Ext
														.decode(action.result.attributes.json)[0]['最后修改日期'].month + 1)
												+ "月"
												+ Ext
														.decode(action.result.attributes.json)[0]['最后修改日期'].date
												+ "日"
												+ Ext
														.decode(action.result.attributes.json)[0]['最后修改日期'].hours
												+ "时"
												+ Ext
														.decode(action.result.attributes.json)[0]['最后修改日期'].minutes
												+ "分"
												+ "占用,创建人是  "
												+ Ext
														.decode(action.result.attributes.json)[0]['userName']
												+ ",新的项目编码是：'" + proCode + "'";
									}
									Ext.Msg.alert(action.result.messageTitle,
											mess, function() {
												contractFormPanel
														.getForm()
														.findField('contractID')
														.setValue(action.result.attributes.contractID);
												contractFormPanel
														.getForm()
														.findField('contractAmt')
														.setValue(contractFormPanel
																.getForm()
																.findField('contractAmt')
																.getRawValue());
												// 设置模版摘要值
												if (!isEmpty) {
													var result = '';
													var content = stencilContent;
													for (var i = 0; i < keys.length; i++) {
														var msg = keys[i]
																.split(';');
														var title = msg[0];
														var fieldType = msg[1];
														if (content
																.indexOf('{}') > 0) {
															result = result
																	+ content
																			.substring(
																					0,
																					content
																							.indexOf('{}'));
															content = content
																	.substring(
																			content
																					.indexOf('{}')
																					+ 2,
																			content.length);
														} else {
															result = result
																	+ content;
														}
														var val = Ext
																.getCmp('stencilParam'
																		+ i)
																.getValue();
														if (fieldType == '3') {
															val = Ext.util.Format
																	.date(
																			Ext
																					.getCmp('stencilParam'
																							+ i)
																					.getValue(),
																			'Y-m-d');
														}
														result += ' <font color=red>'
																+ (val == ''
																		? '__'
																		: val)
																+ '</font> ';
													}
													Ext.getCmp('summary')
															.setValue('');
													var textAreaDiv = document
															.getElementById('textareadiv');
													if (textAreaDiv != null)
														document
																.getElementById('summary')
																.removeChild(textAreaDiv);
													txt = document
															.createElement('DIV');
													txt.id = 'textareadiv';
													txt.innerHTML = result;
													document
															.getElementById('summary')
															.appendChild(txt);
													Ext
															.getCmp('summarystencilValue')
															.setValue(result);
												} else {
													var textAreaDiv = document
															.getElementById('textareadiv');
													if (textAreaDiv != null)
														document
																.getElementById('summary')
																.removeChild(textAreaDiv);
												}
												Ext.getCmp('summarywriteValue')
														.setValue('');
												Ext.getCmp('summary').onBlur();
												if (window.parent.xMis
														&& window.parent.xMis.refresh) {
													window.parent.xMis
															.refresh('contractState');
												}
												if (refreshflag) {
													parent.xMis
															.turn(
																	'contract-'
																			+ action.result.attributes.contractID,
																	Ext
																			.isEmpty(contractFormPanel
																					.getForm()
																					.findField('abb')
																					.getValue())
																			? contractFormPanel
																					.getForm()
																					.findField('contractName')
																					.getValue()
																			: contractFormPanel
																					.getForm()
																					.findField('abb')
																					.getValue(),
																	__cxtPath
																			+ '/contractMain.do?method=contract&contractID='
																			+ action.result.attributes.contractID,
																	'menu-contract-info',
																	true);
													if (contract == null) {
														parent.xMis
																.closeTab(window);
													}
												}
											});

								},
								failure : function(form, action) {
									com.bitc.xmis.util.AlertUtil
											.formSubmitFailedAlert(action);
								}
							});

						} else { // 手写
							var storeStr = com.bitc.xmis.util.ExtUtil
									.encodeStoreModifiedData(conTextStore);

							contractFormPanel.getForm().submit({
								url : __cxtPath + '/contractMain.do',
								params : {
									method : 'saveContractInfoNew',
									warrantyUnit : Ext
											.getCmp("contractFormPanel")
											.getForm()
											.findField('warrantyUnit')
											.getValue(),
									validityUnit : Ext
											.getCmp("contractFormPanel")
											.getForm()
											.findField('validityUnit')
											.getValue(),
									splitMoneyRecords : storeStr,
									projectNumberExist : contract
											? contract.projectOBJ.projectCode
											: ''
								},
								waitMsg : '正在保存，请稍候……',
								waitTitle : '正在处理',
								success : function(form, action) {
									var mess = action.result.message;
									var proCode = action.result.attributes.projectNumber;
									if (action.result.attributes.existflag == true) {
										mess = "'"
												+ Ext
														.decode(action.result.attributes.json)[0]['项目编码']
												+ "'已被"
												+ Ext
														.decode(action.result.attributes.json)[0]['合同名称']
												+ "合同在"
												+ (Ext
														.decode(action.result.attributes.json)[0]['最后修改日期'].year + 1900)
												+ "年"
												+ (Ext
														.decode(action.result.attributes.json)[0]['最后修改日期'].month + 1)
												+ "月"
												+ Ext
														.decode(action.result.attributes.json)[0]['最后修改日期'].date
												+ "日"
												+ Ext
														.decode(action.result.attributes.json)[0]['最后修改日期'].hours
												+ "时"
												+ Ext
														.decode(action.result.attributes.json)[0]['最后修改日期'].minutes
												+ "分"
												+ "占用,创建人是  "
												+ Ext
														.decode(action.result.attributes.json)[0]['userName']
												+ ",新的项目编码是：'" + proCode + "'";
									}
									Ext.Msg.alert(action.result.messageTitle,
											mess, function() {
												contractFormPanel
														.getForm()
														.findField('contractID')
														.setValue(action.result.attributes.contractID);
												contractFormPanel
														.getForm()
														.findField('contractAmt')
														.setValue(contractFormPanel
																.getForm()
																.findField('contractAmt')
																.getRawValue());
												Ext
														.getCmp('summarystencilValue')
														.setValue('');
												Ext.getCmp('summaryfieldset')
														.collapse(true);

												if (contract
														&& contract.stencil) {
													var keys = contract.stencil.keyWords
															.split(',');// 清空模板
													for (var i = 0; i < keys.length
															+ 1; i++) {
														var id = 'stencilParam'
																+ i;
														if (Ext.getCmp(id)) {
															Ext
																	.getCmp(id)
																	.setValue('');
														}
													}
													Ext
															.getCmp('stencilbuttonId')
															.setText('摘要预览');
												}

												if (window.parent.xMis
														&& window.parent.xMis.refresh) {
													window.parent.xMis
															.refresh('contractState');
												}
												if (refreshflag) {
													parent.xMis
															.turn(
																	'contract-'
																			+ action.result.attributes.contractID,
																	Ext
																			.isEmpty(contractFormPanel
																					.getForm()
																					.findField('abb')
																					.getValue())
																			? contractFormPanel
																					.getForm()
																					.findField('contractName')
																					.getValue()
																			: contractFormPanel
																					.getForm()
																					.findField('abb')
																					.getValue(),
																	__cxtPath
																			+ '/contractMain.do?method=contract&contractID='
																			+ action.result.attributes.contractID,
																	'menu-contract-info',
																	true);
													if (contract == null) {
														parent.xMis
																.closeTab(window);
													}
												}
											});
								},
								failure : function(form, action) {
									com.bitc.xmis.util.AlertUtil
											.formSubmitFailedAlert(action);
								}
							});
						}
					}
				},
				handler : function() {
					var summarysucess = false;
					var summarywrite = Ext.getCmp('summarywriteValue')
							.getValue();
					var summarystencil = Ext.getCmp('summarystencilValue')
							.getValue();
					if (Ext.getCmp('summarycheck1ID').getValue() == 1) {
						if (Ext.isEmpty(summarystencil)
								&& (!Ext.isEmpty(summarywrite))) {
							Ext.Msg.show({
								title : '提示',
								msg : '您选择了"按模版编辑"，但是"按模版编辑"中并没有数据，"直接编辑"中有数据。您确定要保存么？',
								buttons : Ext.Msg.YESNO,
								fn : function(btn) {
									if (btn == 'yes') {
										summarysucess = true;
										mainPanel.saveBtn
												.saveFun(summarysucess);
									} else if (btn != 'yes') {
										return;
									}
								}
							});
						} else {
							summarysucess = true;
							mainPanel.saveBtn.saveFun(summarysucess);
						}
					} else if (Ext.getCmp('summarycheck1ID').getValue() == 2) {
						if (Ext.isEmpty(summarywrite)
								&& (!Ext.isEmpty(summarystencil))) {
							Ext.Msg.show({
								title : '提示',
								msg : '您选择了"直接编辑"，但是"直接编辑"中并没有数据，"按模版编辑"中有数据。您确定要保存么？',
								buttons : Ext.Msg.YESNO,
								fn : function(btn) {
									if (btn == 'yes') {
										summarysucess = true;
										mainPanel.saveBtn
												.saveFun(summarysucess);
									} else if (btn != 'yes') {
										return;
									}
								}
							});
						} else {
							summarysucess = true;
							mainPanel.saveBtn.saveFun(summarysucess);
						}
					}
				}
			}, '-', new Ext.ux.UploadButton({
						ref : '../attButton',
						hidden : true,
						reuses : false,
						attachmentTypeID : com.bitc.xmis.Attachment.contract,
						REFIDFun : function() {
							eastPanel.setActiveTab(3);
							return contractFormPanel.getForm()
									.findField('contractID').getValue();
						},
						uploadPanelConfig : {
							listeners : {
								'uploadcomplete' : function() {
									attGrid.reload();
								}
							}
						}
					}), '-', {
				id : 'paymentConditionButton',
				text : '收款计划',
				ref : '../paymentConditionButton',
				iconCls : 'document',
				hidden : true,
				handler : function() {
					eastPanel.setActiveTab(1);
					if (contract != null)
						showPaymentCondition(contract.contractID,
								contract.contractAmt, contract.projectID);
					else
						showPaymentCondition(Ext.getCmp('contractID')
										.getValue(), Ext.getCmp('contractAmt')
										.getValue(), Ext.getCmp('projectID')
										.getValue());
				}
			}, {
				text : '合同款收付登记',
				ref : '../paymentButton',
				iconCls : 'document',
				hidden : true,
				handler : function() {
					if (contract != null)
						showPaymentIn(contract.contractID,
								contract.contractName, contract.contractAmt,
								window.invoicedAmtTemp);
					else
						showPaymentIn(Ext.getCmp('contractID').getValue(), Ext
										.getCmp('abb').getValue(), Ext
										.getCmp('contractAmt').getValue(), 0);
				}
			}, {
				text : '票据收发登记',
				ref : '../invoiceButton',
				iconCls : 'document',
				hidden : true,
				handler : function() {
					if (contract != null)
						showInvoice(contract.contractID,
								contract.contractTypeID, contract.buyerID,
								window.earnedAmtTemp);
					else
						showInvoice(Ext.getCmp('contractID').getValue(), 1, 0,
								0);
				}
			}, {
				text : '付款推迟原因登记',
				ref : '../delayButton',
				iconCls : 'document',
				hidden : true,
				handler : function() {
					eastPanel.setActiveTab(2);
					showDelay();
					}
			}, '-', {
				xtype : 'button',
				text : '刷新',
				hidden : true,
				iconCls : 'reload',
				ref : '../refreshButton',
				handler : function(tabs, item) {
					location.reload();
				}
			}, '-', {
				text : '项目',
				ref : '../projectButton',
				iconCls : 'dept',
				hidden : true,
				handler : function() {
					parent.xMis.turn('project-info-' + contract.projectID,
							'项目信息' + contract.projectCodeExist,
							'projectMain.do?method=getProjectInfo&projectID='
									+ contract.projectID, 'menu-project-info');
				}
			},'-',{
				text : '下达任务书',
				ref : '../transmitButton',
				iconCls : 'document',
				hidden : true,
				handler:function(){
					if(contract != null){
						Ext.Ajax.request({
							url :  __cxtPath + '/contractMain.do?method=transmitAssignment',
							success : function(response , options){
								var obj = Ext.decode(response.responseText);
								Ext.Msg.alert(obj.messageTitle,obj.message);
								attGrid.reload();
							},
							method : "post",
							params : {
								contractID : contract.contractID,
								attachmentTypeID : com.bitc.xmis.Attachment.contract,
								type : '2',								
								projectCodeExist : contract.projectCodeExist
								
							}
					
						});
					
					}
				}
			},'-']
		}
	});

	new Ext.Viewport({
				layout : 'border',
				items : [mainPanel]
			});

	if (contract == null) {
		Ext.Ajax.request({
			url : __cxtPath
					+ '/contractMain.do?method=getContractSummaryStencilByDeptID',
			success : function(response, action) {
				var action = Ext.decode(response.responseText);
				if (action.success) {
					if (action.existStencil == '1') {
						stencilFlag = true;
						stencilKeyWords = action.stencilKeyWords;
						stencilContent = action.stencilContent;
						Ext.getCmp('summary').setReadOnly(true);
						var keys = action.stencilKeyWords.split(',');
						var panel = {
							xtype : 'panel',
							border : false,
							items : []
						};
						var subPanel;
						for (var i = 0; i < keys.length + 1; i++) {
							if (i % 2 == 0) {
								subPanel = {
									xtype : 'panel',
									layout : 'column',
									border : false,
									items : []
								};
								panel.items.push(subPanel);
							}
							if (i != keys.length) {
								var msg = keys[i].split(';');
								var title = msg[0];
								var fieldType = msg[1];
								var fieldTypeStr = 'textfield';
								if (fieldType == '2')
									fieldTypeStr = 'numberfield';
								if (fieldType == '3')
									fieldTypeStr = 'datefield';
								subPanel.items.push({
											xtype : 'panel',
											columnWidth : 0.5,
											layout : 'form',
											labelAlign : 'right',
											labelWidth : 110,
											border : false,
											items : {
												xtype : fieldTypeStr,
												id : 'stencilParam' + i,
												name : 'stencilParam' + i,
												anchor : '90%',
												format : fieldType == '3'
														? 'Y-m-d'
														: '',
												editable : fieldType == '3'
														? false
														: true,
												fieldLabel : title
											}
										});
							} else {
								subPanel.items.push({
									xtype : 'panel',
									columnWidth : 0.5,
									layout : 'form',
									labelAlign : 'right',
									labelWidth : 110,
									border : false,
									items : {
										xtype : 'button',
										id : 'stencilbuttonId',
										anchor : '90%',
										text : '摘要预览',
										labelSeparator : '',
										fieldLabel : '&nbsp;',
										setText : function(text) {
											this.text = text;
											if (this.el) {
												this.btnEl.update(text || ' ');
												this.setButtonClass();
											}
											return this;
										},
										listeners : {
											'click' : function(b, e) {
												if (b.text == '摘要预览') {
													var result = '';
													var content = action.stencilContent;
													for (var i = 0; i < keys.length; i++) {
														var msg = keys[i]
																.split(';');
														var title = msg[0];
														var fieldType = msg[1];
														if (content
																.indexOf('{}') > 0) {
															result = result
																	+ content
																			.substring(
																					0,
																					content
																							.indexOf('{}'));
															content = content
																	.substring(
																			content
																					.indexOf('{}')
																					+ 2,
																			content.length);
														} else {
															result = result
																	+ content;
														}
														var val = Ext
																.getCmp('stencilParam'
																		+ i)
																.getValue();
														if (fieldType == '3') {
															val = Ext.util.Format
																	.date(
																			Ext
																					.getCmp('stencilParam'
																							+ i)
																					.getValue(),
																			'Y-m-d');
														}
														result += ' <font color=red>'
																+ (val == ''
																		? '__'
																		: val)
																+ '</font> ';
													}
													var textAreaDiv = document
															.getElementById('textareadiv');
													if (textAreaDiv != null)
														document
																.getElementById('summary')
																.removeChild(textAreaDiv);
													txt = document
															.createElement('DIV');
													txt.id = 'textareadiv', txt.innerHTML = result;
													document
															.getElementById('summary')
															.appendChild(txt);

													if (Ext
															.getCmp('summarycheck1ID')
															.getValue() == 1) {
														Ext
																.getCmp('summarystencilValue')
																.setValue(result);
													}
													Ext.getCmp('summary')
															.onBlur();
													b.setText('取消预览');
												} else {
													var textAreaDiv = document
															.getElementById('textareadiv');
													if (textAreaDiv != null)
														document
																.getElementById('summary')
																.removeChild(textAreaDiv);

													if (Ext
															.getCmp('summarycheck1ID')
															.getValue() == 1) {
														Ext
																.getCmp('summarystencilValue')
																.setValue('');
													}
													Ext.getCmp('summary')
															.onBlur();
													b.setText('摘要预览');
												}
											}
										}
									}
								});
							}
						}
						Ext.getCmp('summaryCheckPanel').show();
						Ext.getCmp('summaryCheckPanel').doLayout();
						Ext.getCmp('stencilPanel').add(panel);
						Ext.getCmp('stencilPanel').doLayout();
						Ext.getCmp('summaryfieldset').show();
					} else {
						Ext.getCmp('summary').setReadOnly(false);
					}
					Ext.getCmp('summarycheck1ID').setValue('2');
				}
			},
			failure : function() {
			},
			method : "post",
			params : {
				deptID : empDeptID
			}
		});

		Ext.getCmp('projectCodeCtPanel').remove('projectCodeExistPanel');
		Ext.getCmp('projectCodePanelId').show();
		Ext.getCmp('moneycheckContract').setDisabled(true);
		mainPanel.getTopToolbar().enable();
	}

	if (contract != null) {
		
		// 显示合同明细
		Ext.apply(contract, {
			contractType : contract.contractTypeID,
			contractSourceType : contract.sourceTypeID,
			gSourceTypeID : contract.govermentContractSourceID,
			GSourceTypeName : contract.govermentContractSourceName,
			contractCode : contract.contractNo,
			abb : contract.abbreviation,
			summary : contract.contractSummary,
			currencyTypeID : contract.currencyID,
			aCompany_id : contract.buyerID,
			aCompany_name : contract.buyer,
			aDept_id : contract.deptIDOfBuyer,
			aDept_name : contract.deptNameOfBuyer,
			aEmp_id : contract.buyerRepresentativeID,
			aEmp_name : contract.buyerRepresentativeName,
			bCompany_id : contract.vendorID,
			bCompany_name : contract.vendorName,
			bDept_id : contract.deptIDOfVendor,
			bDept_name : contract.deptNameOfVendor,
			bEmp_id : contract.vendorRepresentativeID,
			bEmp_name : contract.vendorRepresentativeName,
			cEmp_name : contract.deliveryName,
			contact_id : contract.deliveryID,
			aContact_name : contract.deliveryName,
			parentNo : contract.parentCode,
			projectID : (contract.projectOBJ && contract.projectOBJ.projectID != 0)
					? contract.projectOBJ.projectID
					: '',
			projectCodeExist : (contract.projectOBJ && contract.projectOBJ.projectID != 0)
					? contract.projectOBJ.projectCode
					: '',
			projName : (contract.projectOBJ && contract.projectOBJ.projectID != 0)
					? contract.projectOBJ.projectName
					: '',
			projectResource : (contract.contractInfoObj && contract.contractInfoObj.projectResourceParentID != 0)
					? contract.contractInfoObj.projectResourceParentID
					: '',
			projectResourceInfoCon : (contract.contractInfoObj && contract.contractInfoObj.projectResourceID != 0)
					? contract.contractInfoObj.projectResourceID
					: '',
			projectOperationType : (contract.contractInfoObj && contract.contractInfoObj.projectOperationTypeParentID != 0)
					? contract.contractInfoObj.projectOperationTypeParentID
					: '',
			projectOperation : (contract.contractInfoObj && contract.contractInfoObj.projectOperationTypeID != 0)
					? contract.contractInfoObj.projectOperationTypeID
					: '',
			projectProperty : (contract.contractInfoObj && contract.contractInfoObj.projectPropertyID != 0)
					? contract.contractInfoObj.projectPropertyID
					: '',
			accountingAgency : (contract.contractInfoObj && contract.contractInfoObj.accountingAgencyID != 0)
					? contract.contractInfoObj.accountingAgencyID
					: '',
			executiveOrgID : (contract.contractInfoObj && contract.contractInfoObj.executiveOrgID != 0)
					? contract.contractInfoObj.executiveOrgID
					: '',
			newcontractflag : contract.newflag
		});
		
		// 当合同来源为102，即横向合同，或者201，公司自研，隐藏来源信息
		if (contract.sourceTypeID == 102 || contract.sourceTypeID == 201) {
			Ext.getCmp('GSourceTypeNamePanel').hide();
		} else {
			Ext.getCmp('GSourceTypeNamePanel').show();
		}

		if (contract.projectOperationType == 8) {
			Ext.getCmp('operationType').hide();
		}
		Ext.getCmp('projectCodeCtPanel').remove('projectCodePanelId');
		Ext.getCmp('projectCodeExistPanel').show();
		// 判断合同金额>=80万
		if (contract.contractAmt >= 800000) {
			var mq = Ext.getCmp('moneycheckQuality');
			mq.setBoxLabel(mq.boxLabel
					+ '<span class="font-color-red">（金额>=80万，必填）</span>');
			Ext.getCmp('contractAmtTipId').show();
		}

		if (contract.contractAmt > 0) {
			var sum = 0;
			for (var i = 0; i < contract.moneyList.length; i++) {
				var moneySplit = contract.moneyList[i];

				if (moneySplit.splitTypeID == 101) {
					contractFormPanel.getForm().findField('selfHard')
							.setValue(moneySplit.money);
					sum += moneySplit.money * 100;
				} else if (moneySplit.splitTypeID == 102) {
					contractFormPanel.getForm().findField('homemadeHard')
							.setValue(moneySplit.money);
					sum += moneySplit.money * 100;
				} else if (moneySplit.splitTypeID == 103) {
					contractFormPanel.getForm().findField('importHard')
							.setValue(moneySplit.money);
					sum += moneySplit.money * 100;
				} else if (moneySplit.splitTypeID == 201) {
					contractFormPanel.getForm().findField('homemadeSoft')
							.setValue(moneySplit.money);
					sum += moneySplit.money * 100;
				} else if (moneySplit.splitTypeID == 202) {
					contractFormPanel.getForm().findField('importSoft')
							.setValue(moneySplit.money);
					sum += moneySplit.money * 100;
				} else if (moneySplit.splitTypeID == 301) {
					contractFormPanel.getForm().findField('softwareCost')
							.setValue(moneySplit.money);
					sum += moneySplit.money * 100;
				} else if (moneySplit.splitTypeID == 302) {
					contractFormPanel.getForm().findField('systemdesignCost')
							.setValue(moneySplit.money);
					sum += moneySplit.money * 100;
				} else if (moneySplit.splitTypeID == 303) {
					contractFormPanel.getForm()
							.findField('systemintegrateCost')
							.setValue(moneySplit.money);
					sum += moneySplit.money * 100;
				} else if (moneySplit.splitTypeID == 304) {
					contractFormPanel.getForm().findField('technical')
							.setValue(moneySplit.money);
					sum += moneySplit.money * 100;
				} else if (moneySplit.splitTypeID == 401) {
					contractFormPanel.getForm().findField('constructCost')
							.setValue(moneySplit.money);
					sum += moneySplit.money * 100;
				} else if (moneySplit.splitTypeID == 402) {
					contractFormPanel.getForm().findField('managementCost')
							.setValue(moneySplit.money);
					sum += moneySplit.money * 100;
				} else if (moneySplit.splitTypeID == 403) {
					contractFormPanel.getForm().findField('transportCost')
							.setValue(moneySplit.money);
					sum += moneySplit.money * 100;
				} else if (moneySplit.splitTypeID == 404) {
					contractFormPanel.getForm().findField('taxes')
							.setValue(moneySplit.money);
					sum += moneySplit.money * 100;
				} else if (moneySplit.splitTypeID == 1) {
					Ext.getCmp('hardwarePurchaseID').setTitle('硬件购置费用（合计：'
							+ Ext.util.Format.number(moneySplit.money,
									'0,000.00') + ')');
				} else if (moneySplit.splitTypeID == 2) {
					Ext.getCmp('softwarePurchaseID').setTitle('软件购置费用（合计：'
							+ Ext.util.Format.number(moneySplit.money,
									'0,000.00') + ')');
				} else if (moneySplit.splitTypeID == 3) {
					Ext.getCmp('softservicesplit').setTitle('软件服务费用（合计：'
							+ Ext.util.Format.number(moneySplit.money,
									'0,000.00') + ')');
				} else if (moneySplit.splitTypeID == 4) {
					Ext.getCmp('elsemoneysplit').setTitle('其他费用（合计：'
							+ Ext.util.Format.number(moneySplit.money,
									'0,000.00') + ')');
				}
			}

			sum = sum / 100;
			var mf = Ext.getCmp('moneyfieldset');
			var title = parseFloat(contract.contractAmt) - sum;
			if (title == 0) {
				mf.setTitle('按科质部资质管理标准分解合同金额    '
						+ '<span class="font-color-green">（分解完毕）</span>');
			} else if (title > 0) {
				if (contract.contractAmt >= 800000) {
					mf.setTitle('按科质部资质管理标准分解合同金额     '
							+ '<span class="font-color-red">（还有'
							+ Ext.util.Format.number(title, '0,000.00')
							+ '未分解）</span>');
				} else {
					mf.setTitle('按科质部资质管理标准分解合同金额     ' + '（还有'
							+ Ext.util.Format.number(title, '0,000.00')
							+ '未分解）');
				}
			}
		}
		
		conTextStore.load({
			params : {
				contractID : contract.contractID
			},
			callback : function(r, option, success) {
				var conTextStoreSum = 0;
				conTextStore.each(function(record) {
							conTextStoreSum += parseFloat(record
									.get('costMoney'));
						});
				conTextStoreSum = (conTextStoreSum == '' || conTextStoreSum == null)
						? 0
						: parseFloat(conTextStoreSum) * 100;
				conTextStoreSum = conTextStoreSum / 100;
				var mg = Ext.getCmp('moneygrid');
				var title = parseFloat(contract.contractAmt) - conTextStoreSum;
				if (title > 0) {
					mg.setTitle(mg.title + '（还有'
							+ Ext.util.Format.number(title, '0,000.00')
							+ '未分解）');
				} else {
					mg.setTitle(mg.title
							+ '<span class="font-color-green">（分解完毕）</span>');
				}
			}
		});

		contractFormPanel.getForm().setValues(contract);
		mainPanel.refreshButton.setVisible(true);
		mainPanel.paymentConditionButton.setVisible(true);// 收款计划
		mainPanel.paymentButton.setVisible(true);// 合同款收付登记
		mainPanel.attButton.setVisible(true);
		mainPanel.invoiceButton.setVisible(true);// 票据收发登记
		mainPanel.delayButton.setVisible(true);// 付款推迟原因登记
		mainPanel.projectButton.setVisible(true);
		mainPanel.transmitButton.setVisible(true);

		if (!hasContractControlOnCompanyLevel) {
			com.bitc.xmis.util.ExtUtil.disableField(Ext
							.getCmp('contractNameId'), Ext
							.getCmp('aCompany_name'), Ext
							.getCmp('contractAmtCtId'), Ext
							.getCmp('bCompany_name'), Ext
							.getCmp('signatureDateId'));
			Ext.getCmp('summary').allowBlank = false;
			Ext.getCmp('validityEndDate').allowBlank = false;
			Ext.getCmp('validityStartDate').allowBlank = false;
			Ext.getCmp('contractSourceType').allowBlank = false;
			mainPanel.transmitButton.hide();	
		}
		

		if (contractOpType == 5) {// 对合同的操作类型
			mainPanel.getTopToolbar().enable();
			attGrid.setActionColumnHidden(false);
			
		}else{
			mainPanel.refreshButton.enable();
			mainPanel.projectButton.enable();
//			mainPanel.qqattention.enable();
//			mainPanel.attention.enable();
//			mainPanel.transmitButton.enable();
		}
		// 合同不为空时，显示最后修改人及时间
		Ext.getCmp('lastname').show();
		Ext.getCmp('lastdate').show();

		if (contract.stencil == null) {
			Ext.getCmp('summary').setReadOnly(false);
		}
		
		// 摘要模版相关
		if (contract.stencil != null) {
			Ext.getCmp('summaryfieldset').show();
			Ext.getCmp('summaryCheckPanel').show();
			stencilContent = contract.stencil.content;
			var stencilKeyWords = contract.stencil.keyWords;
			var keys = contract.stencil.keyWords.split(',');
			var panel = {
				xtype : 'panel',
				border : false,
				items : []
			};
			var subPanel;
			for (var i = 0; i < keys.length + 1; i++) {
				if (i % 2 == 0) {
					subPanel = {
						xtype : 'panel',
						layout : 'column',
						border : false,
						items : []
					};
					panel.items.push(subPanel);
				}
				if (i != keys.length) {
					var msg = keys[i].split(';');
					var title = msg[0];
					var fieldType = msg[1];
					var fieldTypeStr = 'textfield';
					if (fieldType == '2')
						fieldTypeStr = 'numberfield';
					if (fieldType == '3')
						fieldTypeStr = 'datefield';
					subPanel.items.push({
								xtype : 'panel',
								columnWidth : 0.5,
								layout : 'form',
								labelAlign : 'right',
								labelWidth : 110,
								border : false,
								items : {
									xtype : fieldTypeStr,
									id : 'stencilParam' + i,
									name : 'stencilParam' + i,
									anchor : '90%',
									format : fieldType == '3' ? 'Y-m-d' : '',
									editable : fieldType == '3' ? false : true,
									fieldLabel : title
								}
							});
				} else {
					subPanel.items.push({
						xtype : 'panel',
						columnWidth : 0.5,
						layout : 'form',
						labelAlign : 'right',
						labelWidth : 110,
						border : false,
						items : {
							xtype : 'button',
							id : 'stencilbuttonId',
							anchor : '90%',
							text : '摘要预览',
							labelSeparator : '',
							fieldLabel : '&nbsp;',
							setText : function(text) {
								this.text = text;
								if (this.el) {
									this.btnEl.update(text || ' ');
									this.setButtonClass();
								}
								return this;
							},
							listeners : {
								'click' : function(b, e) {
									if (b.text == '摘要预览') {
										var result = '';
										var content = contract.stencil.content;
										for (var i = 0; i < keys.length; i++) {
											var msg = keys[i].split(';');
											var title = msg[0];
											var fieldType = msg[1];
											if (content.indexOf('{}') > 0) {
												result = result
														+ content
																.substring(
																		0,
																		content
																				.indexOf('{}'));
												content = content.substring(
														content.indexOf('{}')
																+ 2,
														content.length);
											} else {
												result = result + content;
											}
											var val = Ext.getCmp('stencilParam'
													+ i).getValue();
											if (fieldType == '3') {
												val = Ext.util.Format
														.date(
																Ext
																		.getCmp('stencilParam'
																				+ i)
																		.getValue(),
																'Y-m-d');
											}
											result += ' <font color=red>'
													+ (val == '' ? '__' : val)
													+ '</font> ';
										}
										var textAreaDiv = document
												.getElementById('textareadiv');
										if (textAreaDiv != null)
											document.getElementById('summary')
													.removeChild(textAreaDiv);
										txt = document.createElement('DIV');
										txt.id = 'textareadiv', txt.innerHTML = result;
										document.getElementById('summary')
												.appendChild(txt);

										if (Ext.getCmp('summarycheck1ID')
												.getValue() == 1) {
											Ext.getCmp('summarystencilValue')
													.setValue(result);
										}
										Ext.getCmp('summary').onBlur();
										b.setText('取消预览');
									} else {
										var textAreaDiv = document
												.getElementById('textareadiv');
										if (textAreaDiv != null)
											document.getElementById('summary')
													.removeChild(textAreaDiv);

										if (Ext.getCmp('summarycheck1ID')
												.getValue() == 1) {
											Ext.getCmp('summarystencilValue')
													.setValue('');
										}
										Ext.getCmp('summary').onBlur();
										b.setText('摘要预览');
									}
								}
							}
						}
					});
				}
			}
			Ext.getCmp('stencilPanel').add(panel);
			Ext.getCmp('stencilPanel').doLayout();
			
			// 修改摘要显示
			if (contract.contractSummary.trim() != '') {
				var vals = contract.contractSummary.split('&^$>,<$^&');
				if (vals != null && vals.length > 0) {
					if (vals.length != 1) {// if为已有摘要信息判断
						vals[0] = vals[0].substring(4);
						var l = vals.length - 1;
						vals[l] = vals[l].substring(0, vals[l].length - 4);
					} else {
						contractFormPanel.getForm().findField('summarycheck1')
								.setValue('2');
					}
				}
				var result = '';
				var isEmpty = true;
				// if为已有摘要信息判断
				if (vals.length != 1) {
					for (var i = 0; i < vals.length; i++) {
						if (vals[i].trim() != '') {
							isEmpty = false;
						}
						Ext.getCmp('stencilParam' + i).setValue(vals[i]);
					}
				}
				//
				if (!isEmpty) {
					var content = contract.stencil.content;
					for (var i = 0; i < vals.length; i++) {
						if (content.indexOf('{}')) {
							result += content.substring(0, content
											.indexOf('{}'))
									+ ' <font color=red>'
									+ (vals[i].trim() == '' ? '__' : vals[i]
											.trim()) + '</font> ';
							content = content.substring(content.indexOf('{}')
									+ 2);
						} else {
							result += content;
						}
					}
					Ext.getCmp('summarystencilValue').setValue(result);
				}
				// if为已有摘要信息判断
				if (vals.length == 1 && vals[0].trim() != '') {
					result = vals[0];
					existOldSummary = result;
					Ext.getCmp('summarywriteValue').setValue(result);
				}
				Ext.getCmp('summary').setValue('');
				var textAreaDiv = document.getElementById('textareadiv');
				if (textAreaDiv != null)
					document.getElementById('summary').removeChild(textAreaDiv);
				txt = document.createElement('DIV');
				txt.id = 'textareadiv', txt.innerHTML = result;
				document.getElementById('summary').appendChild(txt);
			} else {
				Ext.getCmp('summarycheck1ID').setValue('2');
			}
		}

		Ext.getCmp('summary').onBlur();
		if (Ext.getCmp('summarycheck1ID').getValue() == 2) {
			var textAreaDiv = document.getElementById('textareadiv');
			if (textAreaDiv != null)
				document.getElementById('summary').removeChild(textAreaDiv);
			Ext.getCmp('summary').setValue(contract.contractSummary);
		}
		// 对应合同调整
		if (contract.contractTypeID == 1) {
			relatedGrid.getColumnModel().setHidden(1, true);
		} else {
			relatedGrid.getColumnModel().setHidden(3, true);
		}
		relatedStore.load({
					params : {
						contractID : contract.contractID,
						contractType : contract.contractTypeID
					}
				});
		
		// 显示合同收款信息
		var currencyType;
		switch (contract.currencyID) {
			case 1 :
				currencyType = '￥';
				break;
			case 2 :
				currencyType = 'U.S.$';
				break;
			case 3 :
				currencyType = '€';
				break;
			case 4 :
				currencyType = 'HK$';
				break;
			default :
				currencyType = '￥';
		}
		
		var financyOrBudgetAMT;
		
		if (contract.sourceTypeID == 201) {
			financyOrBudgetAMT = "预算拨款合计";
		} else {
			financyOrBudgetAMT = "财务来款合计";
		}
		
		var tmsg = (contract.earnedTotalAmt < contract.incomAmtTotalByGl)
				? '<tr><td><font color="red">&nbsp;&nbsp;有财务到帐未登记的金额！</font></td></tr>'
				: '';
		var ht = "<table width=780>"
				+ tmsg
				+ "<tr><td>&nbsp;&nbsp;已收合同金额："
				+ (contract.earnedTotalAmt == 0
						&& contract.incomAmtTotalByGl == 0
						? "<a id='Amt' href=# id=paymentListLink_"
								+ contract.contractID
								+ " onClick=\"showPaymentIn("
								+ contract.contractID + ",'"
								+ contract.contractName + "',"
								+ contract.contractAmt + ","
								+ window.invoicedAmtTemp + ")\" >"
						: "<a id='Amt' href=# id=paymentListLink_"
								+ contract.contractID
								+ " onClick=\"showPaymentIn("
								+ contract.contractID
								+ ",'"
								+ contract.contractName
								+ "',"
								+ contract.contractAmt
								+ ","
								+ window.invoicedAmtTemp
								+ ")\" >"
								+ formatCurrency(
										currencyType,
										(contract.earnedTotalAmt == 0 && contract.earnedTotalAmt < contract.incomAmtTotalByGl)
												? contract.incomAmtTotalByGl
												: contract.earnedTotalAmt + '',
										(contract.earnedTotalAmt < contract.incomAmtTotalByGl)
												? 'red'
												: Color_Font_Contract_Total_Charged_Amt))
				+ (contract.earnedTotalAmt == 0 ? "</a>" : "</a>")
				+ "</td><td id='unAmt'>未收合同金额："
				+ formatCurrency(currencyType, contract.arrearageTotalAmt + '',
						'red')
				+ "</td><td>已开发票金额："
				+ (contract.invoicedTotalAmt == 0
						? "<a id='invoiced' href=# id=invoiceListLink_"
								+ contract.contractID
								+ " onClick=\"showInvoice("
								+ contract.contractID + ","
								+ contract.contractTypeID + ","
								+ contract.buyerID + "," + earnedAmtTemp
								+ ")\" >"
						: "<a id='invoiced' href=# id=invoiceListLink_"
								+ contract.contractID
								+ " onClick=\"showInvoice("
								+ contract.contractID
								+ ","
								+ contract.contractTypeID
								+ ","
								+ contract.buyerID
								+ ","
								+ earnedAmtTemp
								+ ")\" >"
								+ formatCurrency(currencyType,
										contract.invoicedTotalAmt + '',
										Color_Font_Contract_Total_Invoiced_Amt))
				+ (contract.invoicedTotalAmt == 0 ? "</a>" : "</a>")
				+ "</td><td id='invoicedUnAmt'>已开发票未到账金额："
				+ formatCurrency(currencyType,
						contract.unCreditedTotalAmt + '',
						Color_Font_Contract_Total_UnAccount_Amt)
				+ "</td></tr>"
				// + "<tr><td>&nbsp;&nbsp;财务来款合计："
				+ "<tr><td>&nbsp;&nbsp;"
				+ financyOrBudgetAMT
				+ "："
				+ (contract.incomAmtTotalByGl == 0
						? ''
						: '<a href=# id=\'glIncomeListLink_'
								+ contract.projectOBJ.projectID
								+ '\' onClick=\"showGLIncomeList(\''
								+ convertNameWithoutLinker(contract.abbreviation)
								+ "\',\'"
								+ contract.projectOBJ.projectID
								+ "\')\" >"
								+ formatCurrency(currencyType,
										contract.incomAmtTotalByGl + '',
										Color_Font_Contract_Total_Charged_Amt))
				+ (contract.incomAmtTotalByGl == 0 ? '' : '</a>')
				+ "</td>"
				// + "<td>其他财务来款："
				+ "<td>退税收入来款："
				+ (contract.elseTotalAmt == 0
						? ''
						: '<a href=# id=\'glElseIncomeListLink_'
								+ contract.projectOBJ.projectID
								+ '\' onClick=\"showGLElseIncomeList(\''
								+ convertNameWithoutLinker(contract.abbreviation)
								+ "\',\'"
								+ contract.projectOBJ.projectID
								+ "\')\" >"
								+ formatCurrency(currencyType,
										contract.elseTotalAmt + '',
										Color_Font_Contract_Total_Charged_Amt))
				+ (contract.elseTotalAmt == 0 ? '' : '</a>')
				+ "</td>"
				+ "<td>财务采购支出合计："
				+ (decisionFlag
						? contract.purAmtTotalByGl == 0
								? ""
								: formatCurrency(
										currencyType,
										contract.purAmtTotalByGl
												+ '^onClick=\"showGLPurchaseList(\''
												+ convertNameWithoutLinker(contract.abbreviation)
												+ "\',\'"
												+ contract.projectOBJ.projectID
												+ "\',\'2\'" + ")\" "
												+ ' id=\'glPurchaseListLink_'
												+ contract.projectOBJ.projectID
												+ '\'',
										Color_Font_Contract_Total_Charged_Amt)
						: '<font color=red >权限不足,无法显示！</font>')
				+ "</td><td>合同欠款金额："
				+ formatCurrency(currencyType, contract.contractAmt
								- contract.incomAmtTotalByGl + '', 'red')
				+ "</td></tr></table>";
		var payPanel = new Ext.Panel({
					id : 'payPanel',
					border : false,
					height : (contract.earnedTotalAmt < contract.incomAmtTotalByGl)
							? 60
							: 45,
					padding : '5 5 5 5',
					html : '<SPAN id=totalAmtMsg>' + ht + '</SPAN>'
				});
		contractFormPanel.insert(0, payPanel);
		contractFormPanel.doLayout();
		// 付款条件定义金额<总金额,字体为红色
		Ext.Ajax.request({
			url : __cxtPath
					+ '/contractMain.do?method=getPaymentConditionTotalAmtById',
			success : function(response, action) {
				var action = Ext.decode(response.responseText);
				var conditionAmt = parseFloat(action.attributes.amt);
				var conditionPercent = parseFloat(action.attributes.percent);
				if (!(conditionAmt >= contract.contractAmt || Math.abs(10000
						- (conditionPercent * 100)) < 1)) {
					Ext.getCmp('paymentConditionButton')
							.setText('<font color=red>收款计划</font>');
				}
			},
			failure : function() {
			},
			method : "post",
			params : {
				contractID : contract.contractID
			}
		});
		// 关注
		var attention = new com.bitc.xmis.workflow.Attention({
			ref:'../qqattention',
					model : 'contract',
					url : 'contractMain.do?method=contract&contractID='
							+ contract.contractID,
					text : '【合同】' + contract.contractName,
					appId : contract.contractID,
					userID : __emp.userID,
					addText : '关注此合同'
				});
		attention.addButtonToBar(mainPanel.getTopToolbar());
		attention.button.enable();
	}

		// if (readOnly) {
		// mainPanel.getTopToolbar().hide(true);
		// }
	
	
	
	
});

var modifiable = ['contractCode', 'bCompany_name'];
function setFormReadOnly(form, readonly) // 该函数设置里面的控件的可读性。
{
	form.getForm().items.eachKey(function(key, item) {
				if (readonly
						&& modifiable.indexOf(item.name || item.hiddenName) > -1) {
					if (item.xtype == 'combo' || item.xtype == 'choosefield') {
						item.disabled = true;
					} else {
						item.el.dom.readOnly = true;
					}
				} else {
					item.el.dom.readOnly = false;
				}
			});
}

function warLimitCalculate() {
	var start = Ext.getCmp('warrantyStartDate');
	var limit = Ext.getCmp('warrantyLimit');
	var unit = Ext.getCmp('warrantyUnit');
	var end = Ext.getCmp('warrantyEndDate');
	if (start.getValue() == "" || limit.getValue() == "") {
		return;
	}
	var date = start.getValue();
	var result;
	if (unit.getValue() == 3) {
		result = date.add(Date.DAY, limit.getValue());
	} else if (unit.getValue() == 2) {
		result = date.add(Date.MONTH, limit.getValue());
	} else if (unit.getValue() == 1) {
		result = date.add(Date.YEAR, limit.getValue());
	}
	end.setValue(result.format('Y-m-d'));
}

function valLimitCalculate() {
	var start = Ext.getCmp('validityStartDate');
	var limit = Ext.getCmp('validityLimit');
	var unit = Ext.getCmp('validityUnit');
	var end = Ext.getCmp('validityEndDate');
	if (start.getValue() == "" || limit.getValue() == "") {
		return;
	}
	var date = start.getValue();
	var result;
	if (unit.getValue() == 3) {
		result = date.add(Date.DAY, limit.getValue());
	} else if (unit.getValue() == 2) {
		result = date.add(Date.MONTH, limit.getValue());
	} else if (unit.getValue() == 1) {
		result = date.add(Date.YEAR, limit.getValue());
	}
	end.setValue(result.format('Y-m-d'));
}

function nameLinker(val) {
	var text = val.substring(0, val.indexOf("^"));
	var url = val.substring(val.indexOf("^") + 1, val.length);
	if (text != '0') {
		return '<a href="' + url + '">' + text + '</a>';
	} else
		return text;
}
function turnToEditContract(contractID, contractName) {
	parent.xMis.turnToMenu('contract-' + contractID,
			'contractMain.do?method=contract&contractID=' + contractID, '【合同】'
					+ contractName);
}
var payCWin;
function showPaymentCondition(contractID, contractAmt, projectID) {
	if (!payCWin) {
		payCWin = new Ext.Window({
					modal : true,
					width : Ext.getBody().getWidth() * 0.95,
					title : '收款计划',
					height : Ext.getBody().getHeight() * 0.9,
					border : false,
					resizable : false,
					layout : 'border',
					closeAction : 'hide',
					items : {
						xtype : 'panel',
						region : 'center',
						autoScroll : true,
						autoLoad : {
							url : __cxtPath
									+ '/contract/paymentCondition.jsp?contractID='
									+ contractID + '&contractAmt='
									+ contractAmt + '&collapsible=false'
									+ '&projectId=' + projectID,
							scripts : true
						}
					}

				});
	}
	payCWin.show();
}
var contractID;
var delayWin;
function showDelay() {
	
	contractID = contract.contractID;
	if (!delayWin) {
		delayWin = new Ext.Window({
					title : contract.contractName,
					width : 800,
					height : 447,
					layout : 'border',
					closeAction : 'hide',
					items : new com.bitc.xmis.delayReasonGridPanel({
								title : "付款推迟原因登记",
								readOnly : false,
								region : 'center',
								contractID : contractID
							})
				});
	}
	delayWin.show();
}

var invoiceWin;
function showInvoice(contractID, contractTypeID, buyerID, earnedAmtTemp) {
	if (!invoiceWin) {
		invoiceWin = new Ext.Window({
					title : '票据收发登记',
					width : 800,
					height : 449,
					layout : 'border',
					closeAction : 'hide',
					items : {
						xtype : 'panel',
						region : 'center',
						autoScroll : true,
						autoLoad : {
							url : __cxtPath
									+ '/contract/invoice.jsp?contractID='
									+ contractID + '&contractType='
									+ contractTypeID + '&companyID=' + buyerID
									+ '&earnedAmtTemp=' + earnedAmtTemp,
							scripts : true
						}
					}

				});
	}
	invoiceWin.show();
}

function showPayment(contractID, contractNo, contractName, contractAmt) {
	var paymentoutWin;
	var _url = __cxtPath
			+ '/contract/paymentEdit.jsp?'
			+ encodeURI('contractID=' + contractID + '&contractType=2'
					+ '&contractName=' + contractName + '&contractAmt='
					+ contractAmt);

	paymentoutWin = new Ext.Window({
				title : String.format('合同款收付登记 - {0}', contractNo),
				width : 800,
				height : 447,
				layout : 'border',
				items : {
					xtype : 'panel',
					region : 'center',
					autoScroll : true,
					autoLoad : {
						url : _url,
						scripts : true
					}
				}
			});
	paymentoutWin.show();
}

function showPaymentIn(contractID, contractName, contractAmt, invoicedAmtTemp) {
	var paymentWin;
	var _url = __cxtPath
			+ '/contract/paymentEdit.jsp?'
			+ encodeURI('contractID=' + contractID + '&contractType=1'
					+ '&contractName=' + contractName + '&contractAmt='
					+ contractAmt + '&invoicedAmtTemp=' + invoicedAmtTemp);
	paymentWin = new Ext.Window({
				title : '合同款收付登记',
				width : 800,
				height : 447,
				id : 'paymentWindow',
				layout : 'border',
				items : {
					xtype : 'panel',
					region : 'center',
					autoScroll : true,
					autoLoad : {
						url : _url,
						scripts : true
					}
				}
			});
	paymentWin.show();
}
function clearAllSelectedCheckbox() {
	var hd = Ext.fly(Ext.getCmp('elsesplit').getView().innerHd)
			.child('div.x-grid3-hd-checker');
	hd.removeClass('x-grid3-hd-checker-on');
	Ext.getCmp('elsesplit').getSelectionModel().clearSelections();
}
