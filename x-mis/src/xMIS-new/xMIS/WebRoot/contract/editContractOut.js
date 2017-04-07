Ext.onReady(function() {
	Ext.QuickTips.init();
	Ext.form.Field.prototype.msgTarget = 'under';
	var rocuremenAmt_param = '';// 金额rocuremenAmt_param
	var display = '';// 支出合同项目编号
	var purdelID = '';
	var mxID = '';
	var mxName = '';

	var index_str = '';
	if (!(contractOpType == 5)) {
		//隐藏
		index_str += '2,3';
		if (decisionFlag) {
			//有权限,去掉2
			index_str = index_str.replace('2','');
		}
	}

	/**
	 * 附件列表
	 */
	var attGrid = new com.bitc.xmis.AttachmentList({
				attachmentTypeID : com.bitc.xmis.Attachment.contract,
				REFID : contract != null ? contract.contractID : 0,
				title : '其他相关文件',
				stripeRows : true,
				region : 'center',
				loadMask : true,
				enableColumnMove : false,
				enableColumnResize : false,
				actionColumnHiddenIndex : index_str
			});
	var deviceInGridPanel = new com.bitc.xmis.DeviceInGridPanel({
				contractId : contract != null ? contract.contractID : 0,
				autoScroll : true,
				title : '到货清单',
				border : false,
				region : 'center'
			});
	top.xMis.addRefresh('deviceInGridPanelLOAD', function() {
				deviceInGridPanel.store.reload();
			});

	var contractFormPanel = new Ext.FormPanel({
		autoScroll : true,
		width : 570,
		bodyStyle : 'padding:5px 5px 0px 5px',
		region : 'center',
		defaultType : 'textfield',
		labelAlign : 'right',
		labelWidth : 80,
		items : [{
					xtype : 'displayfield',
					name : 'relatedAmtTip',
					id : 'relatedAmtTipId',
					width : 450,
					style : 'color : red; font-weight : bold;',
					readOnly : true
				}, {
					xtype : 'fieldset',
					title : '科质部填写',
					width : 500,
					defaultType : 'textfield',
					defaults : {
						anchor : '80%'
					},
					items : [{
								xtype : 'hidden',
								name : 'newcontractflag',
								id : 'newcontractId'
							}, {
								xtype : 'hidden',
								id : 'contractID',
								name : 'contractID'
							}, {
								xtype : 'hidden',
								name : 'contractTypeID',
								value : contractType,
								listeners : {
									'afterrender' : function(cmp) {
										contractTypeChange(contractType);
									}
								}
							}, {
								xtype : 'hidden',
								name : 'contractDelimiter',
								value : '-ZC'
							}, {
								xtype : 'panel',
								layout : 'form',
								id : 'contractCodeCtPanelId',
								border : false,
								items : [{
											xtype : 'panel',
											layout : 'form',
											labelWidth : 80,
											border : false,
											id : 'contractCodeExistPanelId',
											items : [{
														xtype : 'displayfield',
														fieldLabel : '甲方合同编号',
														id : 'contractCodeExistId',
														name : 'contractCodeExist',
														allowBlank : false
													}]
										}, {
											xtype : 'panel',
											layout : 'form',
											labelWidth : 80,
											border : false,
											id : 'contractCodePanelId',
											border : false,
											labelAlign : 'right',
											items : [{
												xtype : 'compositefield',
												fieldLabel : '甲方合同编号',
												id : 'newcontractCodeId',
												name : 'newcontractCode',
												border : false,
												items : [
														new Ext.ux.form.ChooseField(
																{
																	fieldLabel : '甲方合同编号',
																	width : 192,
																	name : 'contractCode',
																	allowBlank : false,
																	winConfig : {
																		width : 765,
																		title : '选择对应合同编号'
																	},
																	baseParamFun : function() {
																		return {
																			searchName : '',
																			contractTypeID : 1,
																			limit : 20
																		};
																	},
																	paramFun : function() {
																		return {
																			start : 0
																		};
																	},
																	chooseUrl : __cxtPath
																			+ '/resources/choose/contract.json.jsp',
																	setValueFun : function(
																			o) {
																		if (Ext
																				.isEmpty(o)) {
																			Ext.Msg
																					.alert(
																							'提示',
																							'您还没有选择，请选择');
																		}
																		var abb = o
																				.get(DBSchema.Project.ProjectCode);
																		if (abb
																				.indexOf('^') > 0) {
																			abb = abb
																					.substring(
																							0,
																							abb
																									.indexOf('^'));
																		}

																		contractFormPanel
																				.getForm()
																				.findField('contractCode')
																				.setValue(abb);
																		contractFormPanel
																				.getForm()
																				.findField('newContractCodeNumber')
																				.loadValue(abb);

																		Ext.Ajax
																				.request(
																						{
																							url : __cxtPath
																									+ '/contractMain.do?method=getContractInfoByProjectCode',
																							params : {
																								code : abb
																							},
																							scope : this,
																							success : function(
																									response) {
																								var result = Ext
																										.decode(response.responseText);
																								if (result[0] != null) {
																									contractFormPanel
																											.getForm()
																											.findField('aDept_name')
																											.setValue(result[0].部门名称);

																									contractFormPanel
																											.getForm()
																											.findField('aDept_id')
																											.setValue(result[0].乙方部门ID);
																								}
																							},
																							failure : function() {
																							}
																						});
																	},
																	clearValueFun : function() {
																		contractFormPanel
																				.getForm()
																				.findField('contractCode')
																				.setValue('');
																		contractFormPanel
																				.getForm()
																				.findField('newContractCodeNumber')
																				.setValue('');
																		contractFormPanel
																				.getForm()
																				.findField('aDept_name')
																				.setValue('');

																		contractFormPanel
																				.getForm()
																				.findField('aDept_id')
																				.setValue('');
																	}
																}),
														// {
														// xtype :
														// 'displayfield',
														// value : delimiter
														// },
														{
															xtype : 'combo',
															width : 50,
															hiddenName : 'delimiterCombo',
															id : 'delimiterCombo',
															displayField : 'delimiter',
															valueField : 'delimiter',
															mode : 'romote',
															triggerAction : 'all',
															editable : false,
															value : '-ZC',
															store : new Ext.data.JsonStore(
																	{
																		url : __cxtPath
																				+ '/contractMain.do?method=getOutContractDelimiterStore',
																		autoLoad : true,
																		fields : [
																				{
																					name : 'delimiter'
																				}]
																	}),
															listeners : {
																'select' : function(
																		combo,
																		record,
																		index) {
																	contractFormPanel
																			.getForm()
																			.findField('contractDelimiter')
																			.setValue(combo
																					.getValue());
																}
															}
														}, {
															xtype : 'textfield',
															width : 45,
															// readOnly : true,
															id : 'newContractCodeNumberId',
															name : 'newContractCodeNumber',
															loadValue : function(
																	code) {
																Ext.Ajax
																		.request(
																				{
																					url : __cxtPath
																							+ '/contractMain.do?method=getContractCodeNumber',
																					params : {
																						contractCode : code
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
																					},
																					failure : function() {
																					}
																				});
															}
														}]
											}]
										}]
							}, {
								fieldLabel : '合同名称',
								id : 'contractNameId',
								name : 'contractName',
								allowBlank : false,
								width : 400
							}, {
								xtype : 'panel',
								layout : 'form',
								border : false,
								items : [{
									xtype : 'panel',
									id : 'contractAmtCtId',
									border : false,
									layout : 'column',
									fieldLabel : '合同金额',
									labelAlign : 'right',
									getAsycnValue : function(callback) {
										this.currency.store.on('load',
												function() {
													var r = this.currency
															.findRecord(
																	this.currency.valueField,
																	this.currency.value);
													var value = "";

													if (!(contract == null || decisionFlag)) {
														var value = Ext
																.getCmp('contractAmtdecision').el.dom.innerHTML;
													} else {
														var value = Ext.util.Format
																.number(
																		this.amt
																				.getValue(),
																		r
																				.get('symbol')
																				+ '0,0.00');
													}
													callback
															.call(window, value);
												}, this);
										return;
									},
									items : [
											(contract == null || decisionFlag)
													? {
														xtype : 'panel',
														border : false,
														layout : 'form',
														columnWidth : 0.7,
														hideLabels : true,
														items : [{
															ref : '../amt',
															xtype : 'formatnum',
															id : 'contractAmtId',
															name : 'contractAmt',
															style : 'text-align:right',
															FormatComma : true,
															width : 200,
															hideLabel : true
														}]
													}
													: {
														columnWidth : 0.7,
														xtype : 'panel',
														border : false,
														layout : 'form',
														items : [{
															xtype : 'hidden',
															id : 'contractAmt',
															name : 'contractAmt'
														}, {
															fieldLabel : '合同金额',
															xtype : 'label',
															id : 'contractAmtdecision',
															html : '<div style="margin-top:5px"><font color =red>权限不足，无法显示！</font></div>'
														}]
													},
											new com.bitc.xmis.Combos.CurrencyCombo(
													{
														columnWidth : 0.3,
														ref : 'currency',
														margins : '0 0 0 5',
														id : 'currencyTypeID',
														hiddenName : 'currencyType'
													})]
								}]
							}, {
								xtype : 'hidden',
								id : 'aCompany_id',
								name : 'aCompany_id'
							}, new Ext.ux.form.ChooseField({
										fieldLabel : '甲方公司',
										id : 'aCompany_name',
										name : 'aCompany_name',
										anchor : '90%',
										allowBlank : false,
										hideTrigger2 : true
									}), {
								xtype : 'hidden',
								id : 'bCompany_id',
								name : 'bCompany_id'
							}, new Ext.ux.form.ChooseField({
								fieldLabel : '乙方公司',
								id : 'bCompany_name',
								name : 'bCompany_name',
								// anchor : '90%',
								width : 296,
								winConfig : {
									title : '乙方公司设置'
								},
								baseParamFun : function() {
									winFlag = "bWin";
									return {
										start : 0,
										limit : 20
									};
								},
								allowBlank : false,
								chooseUrl : __cxtPath
										+ '/resources/choose/company.json.jsp',
								setValueFun : function(o) {
									Ext.getCmp('bCompany_id').setValue(o
											.get(DBSchema.Company.CompanyID));
									Ext.getCmp('bCompany_name').setValue(o
											.get(DBSchema.Company.CompanyName));
								},
								clearValueFun : function() {
									contractFormPanel.getForm()
											.findField('bCompany_id')
											.setValue('');
									contractFormPanel.getForm()
											.findField('bCompany_name')
											.setValue('');
									contractFormPanel.getForm()
											.findField('bDept_id').setValue('');
									contractFormPanel.getForm()
											.findField('bDept_name')
											.setValue('');
									contractFormPanel.getForm()
											.findField('bEmp_id').setValue('');
									contractFormPanel.getForm()
											.findField('bEmp_name')
											.setValue('');
								}
							}), {
								xtype : 'datefield',
								fieldLabel : '签订日期',
								id : 'signatureDateId',
								name : 'signatureDate',
								format : 'Y-m-d',
								allowBlank : false,
								getAsycnValue : function(callback) {
									callback.call(this, Ext.util.Format.date(
													this.getValue(),
													this.format));
								}
							}]
				}, {
					xtype : 'fieldset',
					title : '合同签订方',
					defaultType : 'textfield',
					width : 500,
					defaults : {
						anchor : '80%'
					},
					items : [{
								fieldLabel : '合同简称',
								id : 'abb',
								maxLength : 30,
								name : 'abb'
							}, {
								xtype : 'textarea',
								fieldLabel : '合同摘要',
								name : 'summary',
								width : 400,
								height : 40,
								listeners : {
									'render' : function(c) {
										Ext.QuickTips.register({
													target : c.getEl(),
													text : '"合同摘要"最多可输入2500字'
												});
									}
								}
							}, {
								xtype : 'hidden',
								id : 'directorID',
								name : 'directorID'
							}, new Ext.ux.form.ChooseOrgTreeField({
								fieldLabel : '合同负责人',
								width : 296,
								id : 'directorName',
								name : 'directorName',
								winConfig : {
									title : '选择合同负责人',
									defaultSearchValue:userName
								},
								singleExpand : true,
								singleSelect : true,
								onlyPerson : true,
								dataUrl : '/orgMaint.do?method=getOrganizationTree',
								rootConfig : function() {
									return {
										id : 'comp-old-1'
									}
								},
								clearValueList : 'directorID,directorName',
								setValueFun : function(result) {
									var directorName = '';
									var directorID = '';
									for (var i = 0; i < result.length; i++) {
										if (i == result.length - 1) {
											directorID = directorID
													+ result[i].id;
											directorName = directorName
													+ result[i].text;
										} else {
											directorID = directorID
													+ result[i].id + ',';
											directorName = directorName
													+ result[i].text + ',';
										}
									}
									Ext.getCmp('directorID')
											.setValue(directorID.split('-')[2]);
									Ext.getCmp('directorName')
											.setValue(directorName);
								},
								listeners : {
									'dblclick' : function(select, num) {
										var empId = Ext.getCmp('directorID')
												.getValue();
										com.bitc.xmis.util.EmpInfoWin(empId,
												'合同负责人信息');
									},
									'nodedblclick' : function(node, e) {
										var path = String.format('/root{0}',
												node.getPath());
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
							}), {
								xtype : 'hidden',
								id : 'aDept_id',
								name : 'aDept_id'
							}, {
								xtype : 'displayfield',
								width : 400,
								fieldLabel : '甲方部门',
								id : 'aDept_name',
								name : 'aDept_name'
							}, {
								xtype : 'hidden',
								id : 'aEmp_id',
								name : 'aEmp_id'
							}, new Ext.ux.form.ChooseOrgTreeField({
								fieldLabel : '甲方代表',
								id : 'aEmp_name',
								name : 'aEmp_name',
								// anchor : '80%',
								width : 296,
								winConfig : {
									title : '甲方代表设置',
									defaultSearchValue:userName
								},
								singleSelect : true,
								onlyPerson : true,
								expandNodePath : function() {
									var path = '/comp-old-1';
									var deptid = 0;
									if (Ext.getCmp('aDept_id') != null
											&& (Ext.getCmp('aDept_id')
													.getValue()).trim() != '') {
										path = path
												+ '/dept-old-'
												+ Ext.getCmp('aDept_id')
														.getValue();
										deptid = Ext.getCmp('aDept_id')
												.getValue();
									}
									return {
										deptid : parseInt(deptid),
										path : path
									}
								},
								rootConfig : function() {
									return {
										id : 'comp-old-1'
									}
								},
								dataUrl : '/orgMaint.do?method=getOrganizationTree',
								setValueFun : function(result) {
									var empName = '';
									var empID = '';
									for (var i = 0; i < result.length; i++) {
										if (i == result.length - 1) {
											empID = empID + result[i].id;
											empName = empName + result[i].text;
										} else {
											empID = empID + result[i].id + ',';
											empName = empName + result[i].text
													+ ','
										}
									}
									Ext.getCmp('aEmp_id').setValue(empID
											.split('-')[2]);
									Ext.getCmp('aEmp_name').setValue(empName);
								},
								clearValueList : 'aEmp_id,aEmp_name',
								listeners : {
									'dblclick' : function(select, num) {
										var empId = Ext.getCmp('aEmp_id')
												.getValue();
										com.bitc.xmis.util.EmpInfoWin(empId,
												'甲方代表信息');
									},
									'nodedblclick' : function(node, e) {
										var path = String.format('/root{0}',
												node.getPath());
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
							}), {
								xtype : 'hidden',
								id : 'delivery_id',
								name : 'delivery_id'
							}, new Ext.ux.form.ChooseOrgTreeField({
								fieldLabel : '甲方收货人',
								id : 'cEmp_name',
								name : 'cEmp_name',
								// anchor : '90%',
								width : 296,
								winConfig : {
									title : '收货人设置',
									defaultSearchValue:userName
								},
								singleSelect : true,
								onlyPerson : true,
								rootConfig : function() {
									return {
										id : 'comp-old-'
												+ Ext.getCmp('aCompany_id')
														.getValue()
									}
								},
								dataUrl : '/orgMaint.do?method=getOrganizationTree',
								setValueFun : function(result) {
									var empName = '';
									var empID = '';
									for (var i = 0; i < result.length; i++) {
										if (i == result.length - 1) {
											empID = empID + result[i].id;
											empName = empName + result[i].text;
										} else {
											empID = empID + result[i].id + ',';
											empName = empName + result[i].text
													+ ',';
										}
									}
									Ext.getCmp('delivery_id').setValue(empID
											.split('-')[2]);
									Ext.getCmp('cEmp_name').setValue(empName);
								},
								clearValueList : 'delivery_id,cEmp_name',
								listeners : {
									'beforeShowWin' : function() {
										var aCmpId = Ext.getCmp('aCompany_id')
												.getValue();
										var bCmpId = Ext.getCmp('bCompany_id')
												.getValue();
										if ((!aCmpId || aCmpId.trim() == "")
												&& (!bCmpId || bCmpId.trim() == "")) {
											Ext.Msg.alert("提示", "请先选择公司。");
											return false;
										}
									},
									'dblclick' : function(select, num) {
										var empId = Ext.getCmp('delivery_id')
												.getValue();
										com.bitc.xmis.util.EmpInfoWin(empId,
												'甲方收货人信息');
									},
									'nodedblclick' : function(node, e) {
										var path = String.format('/root{0}',
												node.getPath());
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
							}), {
								xtype : 'hidden',
								id : 'bDept_id',
								name : 'bDept_id'
							}, new Ext.ux.form.ChooseOrgTreeField({
								fieldLabel : '乙方部门',
								id : 'bDept_name',
								name : 'bDept_name',
								// anchor : '90%',
								width : 296,
								winConfig : {
									title : '乙方部门设置',
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
															.getCmp('bDept_name')
															.getTreePanel()
															.getLoader()
															.load(
																	Ext
																			.getCmp('bDept_name')
																			.getTreePanel()
																			.getRootNode(),
																	function(n) {
																		Ext
																				.getCmp('bDept_name')
																				.getTreePanel()
																				.expand();
																	});
												}
											});
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
													.getCmp('bDept_name')
													.getTreePanel()
													.getLoader()
													.load(
															Ext
																	.getCmp('bDept_name')
																	.getTreePanel()
																	.getRootNode(),
															function(n) {
																Ext
																		.getCmp('bDept_name')
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
												+ Ext.getCmp('bCompany_id')
														.getValue()
									};
								},
								dataUrl : '/orgMaint.do?method=getOrganizationTree',
								clearValueList : 'bDept_id,bDept_name,bEmp_id,bEmp_name',
								setValueFun : function(result) {
									var deptName = '';
									var deptID = '';
									for (var i = 0; i < result.length; i++) {
										if (i == result.length - 1) {
											deptID = deptID + result[i].id;
											deptName = deptName
													+ result[i].text;
										} else {
											deptID = deptID + result[i].id
													+ ',';
											deptName = deptName
													+ result[i].text + ','
										}
									}
									Ext.getCmp('bDept_id').setValue(deptID
											.split('-')[2]);
									Ext.getCmp('bDept_name').setValue(deptName);
								},
								listeners : {
									'beforeShowWin' : function() {
										var bCmpId = Ext.getCmp('bCompany_id')
												.getValue();
										if (!bCmpId || bCmpId.trim() == "") {
											Ext.Msg.alert("提示", "请先选择乙方公司。");
											return false;
										}
									}
								}
							}), {
								xtype : 'hidden',
								id : 'bEmp_id',
								name : 'bEmp_id'
							}, new Ext.ux.form.ChooseOrgTreeField({
								fieldLabel : '乙方代表',
								id : 'bEmp_name',
								name : 'bEmp_name',
								// anchor : '80%',
								width : 296,
								linkManDblclickFunction : true,
								modifyNameStr : '乙方代表',
								winConfig : {
									title : '乙方代表设置',
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
															.getCmp('bEmp_name')
															.getTreePanel()
															.getLoader()
															.load(
																	Ext
																			.getCmp('bEmp_name')
																			.getTreePanel()
																			.getRootNode(),
																	function(n) {
																		Ext
																				.getCmp('bEmp_name')
																				.getTreePanel()
																				.expand();
																	});
												}
											});
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
													.getCmp('bEmp_name')
													.getTreePanel()
													.getLoader()
													.load(
															Ext
																	.getCmp('bEmp_name')
																	.getTreePanel()
																	.getRootNode(),
															function(n) {
																Ext
																		.getCmp('bEmp_name')
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
											+ Ext.getCmp('bCompany_id')
													.getValue();
									return {
										id : searchId
									}
								},
								dataUrl : '/orgMaint.do?method=getOrganizationTree',
								setValueFun : function(result) {
									var empName = '';
									var empID = '';
									for (var i = 0; i < result.length; i++) {
										if (i == result.length - 1) {
											empID = empID + result[i].id;
											empName = empName + result[i].text;
										} else {
											empID = empID + result[i].id + ',';
											empName = empName + result[i].text
													+ ','
										}
									}
									Ext.getCmp('bEmp_id').setValue(empID
											.split('-')[2]);
									Ext.getCmp('bEmp_name').setValue(empName);
								},
								clearValueList : 'bEmp_id,bEmp_name',
								listeners : {
									'beforeshowwin' : function() {
										var bCmpId = Ext.getCmp('bCompany_id')
												.getValue();
										if (!bCmpId || bCmpId.trim() == "") {
											Ext.Msg.alert("提示", "请先选择乙方公司。");
											return false;
										}
									},
									'dblclick' : function(select, num) {
										var empId = Ext.getCmp('bEmp_id')
												.getValue();
										com.bitc.xmis.util.EmpInfoWin(empId,
												'乙方代表信息');
									},
									'nodedblclick' : function(node, e) {
										var path = String.format('/root{0}',
												node.getPath());
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
									'render' : function(c) {
										Ext.QuickTips.register({
													target : c.getEl(),
													text : '双击可显示详细信息'
												});
									}
								}
							})]
				}, {
					xtype : 'fieldset',
					title : '质保信息',
					defaultType : 'textfield',
					width : 500,
					defaults : {
						anchor : '80%'
					},
					items : [{
						xtype : 'panel',
						layout : 'column',
						border : false,
						items : [{
									xtype : 'panel',
									layout : 'form',
									columnWidth : 0.8,
									labelWidth : 80,
									labelAlign : 'right',
									border : false,
									items : [{
												xtype : 'numberfield',
												fieldLabel : '质保期限',
												id : 'warrantyLimit',
												name : 'warrantyLimit',
												anchor : '99%',
												listeners : {
													'change' : limitCalculate
												}
											}]
								}, {
									xtype : 'panel',
									border : false,
									columnWidth : 0.2,
									items : [{
										xtype : 'combo',
										id : 'warrantyUnitID',
										name : 'warrantyUnit',
										displayField : 'text',
										valueField : 'value',
										mode : 'local',
										width : 76,
										triggerAction : 'all',
										value : 1,
										store : new Ext.data.ArrayStore({
													fields : ['value', 'text'],
													data : [[1, '年'], [2, '月'],
															[3, '日']]
												}),
										listeners : {
											'select' : limitCalculate
										}
									}]
								}]
					}, {
						xtype : 'datefield',
						fieldLabel : '质保期起',
						id : 'warrantyStartDate',
						name : 'warrantyStartDate',
						format : 'Y-m-d',
						width : 120,
						listeners : {
							'select' : limitCalculate
						}
					}, {
						fieldLabel : '质保期止',
						id : 'warrantyEndDate',
						name : 'warrantyEndDate',
						readOnly : true,
						width : 120
					}, {
						xtype : 'textarea',
						fieldLabel : '质保条款定义',
						name : 'warrantyDefinition',
						width : 400,
						listeners : {
							'render' : function(c) {
								Ext.QuickTips.register({
											target : c.getEl(),
											text : '"质保条款定义"最多可输入200字'
										});
							}
						}
					}]
				}, {
					xtype : 'fieldset',
					title : '其它信息',
					defaultType : 'displayfield',
					width : 500,
					defaults : {
						anchor : '80%'
					},
					items : [{
								xtype : 'textarea',
								fieldLabel : '备注',
								name : 'remark',
								width : 450,
								listeners : {
									'render' : function(c) {
										Ext.QuickTips.register({
													target : c.getEl(),
													text : '"备注"最多可输入800字'
												});
									}
								}
							}, {
								xtype : 'panel',
								layout : 'column',
								width : 500,
								border : false,
								items : [{
									xtype : 'panel',
									layout : 'form',
									columnWidth : 0.5,
									labelAlign : 'right',
									labelWidth : 90,
									border : false,
									items : [{
										fieldLabel : '登记人',
										name : 'registerName',
										id : 'registerName',
										xtype : 'displayfield',
										value : empName,
										initEvents : function() {
											this.mon(this.el, 'dblclick',
													this.onDblClick, this);
										},
										onDblClick : function() {
											com.bitc.xmis.util.EmpInfoWin(
													RegisterID, '登记人信息');
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
									columnWidth : 0.5,
									labelAlign : 'right',
									labelWidth : 90,
									border : false,
									items : [{
												fieldLabel : '登记日期',
												id : 'updateDate',
												xtype : 'displayfield',
												name : 'updateDate',
												value : new Date()
														.format('Y-m-d')
											}]
								}]
							}, {
								xtype : 'panel',
								layout : 'column',
								width : 500,
								border : false,
								items : [{
									xtype : 'panel',
									layout : 'form',
									columnWidth : 0.5,
									labelAlign : 'right',
									labelWidth : 90,
									border : false,
									items : [{
										fieldLabel : '最后修改人',
										xtype : 'displayfield',
										name : 'lastModificatorName',
										id : 'lastModificatorName',
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
									columnWidth : 0.5,
									labelAlign : 'right',
									labelWidth : 90,
									border : false,
									items : [{
												fieldLabel : '最后修改日期',
												xtype : 'displayfield',
												id : 'lastModificationDate',
												name : 'lastModificationDate'
											}]
								}]
							}]
				}]
	});

	var relatedStore = new Ext.data.Store({
		id : 'rStore',
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
								type : 'float'
							}, {
								name : DBSchema.ContractRelated.Remark
							}, {
								name : DBSchema.ContractRelated.IncomeContractName
							}, {
								name : DBSchema.ContractRelated.OutContractName
							}, {
								name : DBSchema.ContractRelated.OutSubContractNo
							}, {
								name : DBSchema.ContractRelated.OutSubContractID
							}, {
								name : DBSchema.ContractRelated.Paymented,
								type : 'float'
							}, {
								name : DBSchema.ContractRelated.refund,
								type : 'float'
							}, {
								name : DBSchema.ContractRelated.RelatedID
							}]
				}),
		listeners : {
			'load' : function(s, records) {
				var glIncomeAmt = 0.00;
				var gl = 0;
				for (var i = 0; i < records.length; i++) {
					if (i == 0) {
						continue;
					}
					var record = records[i];
					// glIncomeAmt = glIncomeAmt
					// + Number(record.get(DBSchema.ContractRelated.Amt)
					// .toString());
					gl = gl + record.get(DBSchema.ContractRelated.Amt) * 100;
				}
				glIncomeAmt = gl / 100;

				if (hasContractControlOnCompanyLevel) {
					Ext.getCmp('contractAmtId').setMinValue(glIncomeAmt);
					Ext.getCmp('contractAmtId').minText = '该输入项的最小值不可小于分配合同款的总和：'
							+ Ext.util.Format.number(glIncomeAmt, '0,000.00');
				}
				glIncomeAmt = contract.contractAmt - glIncomeAmt;

				Ext.getCmp('relatedGrid').getTopToolbar().removeAll();
				Ext
						.getCmp('relatedGrid')
						.getTopToolbar()
						.add("<div style='width : 55%;float:left'>对应收入合同:(未分配合同款:"
								+ formatCurrency("", decisionFlag ? glIncomeAmt
												.toString() : '0', "blue")
								+ ")</div><div>*点击“支出合同项目编码”可编辑对应信息</div>");

				Ext.getCmp('relatedGrid').doLayout();

				// 未分配合同款>0 && hasContractControlOnCompanyLevel ,弹出“添加对应合同”窗口
				if (glIncomeAmt > 0) {
					contractFormPanel
							.getForm()
							.findField('relatedAmtTip')
							.setValue('合同金额未分配完，首页无法提示；<br/>可通过"合同业务数据查询"找到；事业部不能做信息维护。');
					Ext.getCmp('newcontractId').setValue('0');
					if (window.parent.xMis && window.parent.xMis.refresh) {
						window.parent.xMis.refresh('contractState');
					}
					if (hasContractControlOnCompanyLevel) {
						showAddRelated(relatedStore);
					}
				} else if (glIncomeAmt == 0) {
					contractFormPanel.getForm().findField('relatedAmtTip')
							.setValue('');
					if (window.parent.xMis && window.parent.xMis.refresh) {
						window.parent.xMis.refresh('contractState');
					}
				}

				if (records.length == 0) {
					return;
				}
				var invoiceMenu = mainPanel.invoiceButton.menu;
				invoiceMenu.removeAll(true);
				var paymentMenu = mainPanel.paymentButton.menu;
				paymentMenu.removeAll(true);
				Ext.iterate(records, function(record) {
					if (record.get(DBSchema.ContractRelated.OutContractID) != '') {
						invoiceMenu.addMenuItem({
							text : record
									.get(DBSchema.ContractRelated.OutSubContractNo),
							handler : showInvoice
									.createCallback(
											record
													.get(DBSchema.ContractRelated.OutSubContractID),
											record
													.get(DBSchema.ContractRelated.OutSubContractNo))
						});
						paymentMenu.addMenuItem({
							text : record
									.get(DBSchema.ContractRelated.OutSubContractNo),
							handler : showPayment
									.createCallback(
											record
													.get(DBSchema.ContractRelated.OutSubContractID),
											record
													.get(DBSchema.ContractRelated.OutSubContractNo),
											record
													.get(DBSchema.ContractRelated.OutContractName),
											record
													.get(DBSchema.ContractRelated.Amt))
						});
					}
				});
			}
		}
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
		header : '支出合同编号',
		sortable : true,
		dataIndex : DBSchema.ContractRelated.OutSubContractNo,
		renderer : function(value, metaData, record, rowIndex, colIndex, store) {
			if (0 == rowIndex) {
				return '<font color=red><b>合计信息</b></font>';
			} else {
				if (decisionFlag)
					return '<a href=# onclick=showupdateRelated('
							+ record
									.get(DBSchema.ContractRelated.OutContractID)
							+ ','
							+ record.get(DBSchema.ContractRelated.RelatedID)
							+ ')>' + value + '</a>';
				else
					return value;
			}
		},
		width : 145
	}, {
		id : 'related_name',
		header : '收入合同名称',
		sortable : true,
		dataIndex : DBSchema.ContractRelated.IncomeContractName,
		renderer : relatedRenderer
	}, {
		header : '对应金额',
		align : 'right',
		sortable : true,
		dataIndex : DBSchema.ContractRelated.Amt,
		renderer : function(value, metaData, record, rowIndex, colIndex, store) {
			return formatCurrency("", decisionFlag ? value + '' : '0', "");
			// if(rowIndex == 0){
			// return formatCurrency("",decisionFlag ? value +'' : '0',"") ;
			// }
			// return '<a href=# onclick=showupdateRelated('+
			// record.get(DBSchema.ContractRelated.OutContractID) +','+
			// record.get(DBSchema.ContractRelated.RelatedID) + ')>' +
			// formatCurrency("",decisionFlag ? value +'' : '0',"") + '</a>';
		}
	}, {
		header : '支出合同名称',
		dataIndex : DBSchema.ContractRelated.OutContractName,
		hidden : true,
		sortable : true,
		renderer : relatedRenderer
	}, {
		header : '已付合同款',
		align : 'right',
		sortable : true,
		dataIndex : DBSchema.ContractRelated.Paymented,
		renderer : function(value, metaData, record, rowIndex, colIndex, store) {
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
									+ record.get('支出子合同编号')
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
		header : '已报销金额',
		align : 'right',
		sortable : true,
		dataIndex : DBSchema.ContractRelated.refund,
		renderer : function(value, metaData, record, rowIndex, colIndex, store) {
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
									+ " onClick=showInvoice("
									+ record.get('支出子合同ID')
									+ ",\'"
									+ record.get('支出子合同编号')
									+ "\') >"
									+ formatCurrency('', value + '',
											Color_Font_Contract_Total_Charged_Amt))
							+ (value == 0 ? "" : "</a>");
				}
			}
		}
	}, {
		header : '备注',
		sortable : true,
		dataIndex : DBSchema.ContractRelated.Remark
	}]);

	/**
	 * 对应合同
	 */
	var relatedGrid = new Ext.grid.GridPanel({
				id : 'relatedGrid',
				cm : relatedCM,
				title : '对应合同',
				store : relatedStore,
				stripeRows : true,
				split : true,
				stateful : true,
				stateId : 'editContractOut-relatedGrid',
				region : 'north',
				forceFit : true,
				// height : Ext.getBody().getHeight() * 0.3,
				// height :170,
				loadMask : true,
				viewConfig : {
					getRowClass : function(record, rowIndex, rowParams, store) {
						if (0 == rowIndex) {
							return 'grid-contract-total-list-yellow';
						}
					}
				},
				tbar : []
			});
	var daohuo = {};
	var reRecord = Ext.data.Record.create([{
				"mapping" : "contractID",// 支出合同
				"name" : "contractID"
			}, {
				"mapping" : "deviceName",// 物资名称
				"name" : "matername"
			}, {
				"mapping" : "deviceID",// 物资ID
				"name" : "deviceID1"
			}, {
				"mapping" : "number",// 数量
				"name" : "namber"
			}, {
				"mapping" : "univalent",// 单价
				"name" : "DJ"
			}, {
				"mapping" : "contractNo",// 支出合同项目编号
				"name" : "contractNo"
			}, {
				"mapping" : "contractName",// 支出合同项目名称
				"name" : "contractName"
			}, {
				"mapping" : "expectArrivalDate",// 期望到货日期
				"name" : "expectArrivalDate"
			}, {
				"mapping" : "modelType",// 型号
				"name" : "modelType"
			}, {
				"mapping" : "standard",// 规格
				"name" : "standard"
			}, {
				"mapping" : "weight",// 重量
				"name" : "weight"
			}, {
				"mapping" : "referencePrice",// 官方报价
				"name" : "referencePrice"
			}, {
				"mapping" : "manufacturer",// 生产厂家
				"name" : "manufacturer"
			}, {
				"mapping" : "technicalRequirement",// 技术要求
				"name" : "technicalRequirement"
			}, {
				"mapping" : "remark",// 备注
				"name" : "remark"
			}, {
				"mapping" : "registrationDate",// 登记日期
				"name" : "registrationDate"
			}, {
				"mapping" : "userID",// 登记人
				"name" : "userID"
			}, {
				"mapping" : "purdelID",// 明细ID
				"name" : "purdelID"
			}, {
				"mapping" : "lastModifier",// 最后修改人ID
				"name" : "lastModifier"
			}, {
				"mapping" : "lastModified",// 最后修改时间
				"name" : "lastModified"
			}, {
				"mapping" : "userName",// 登记人name
				"name" : "userName"
			}, {
				"mapping" : "lastModifierName",// 最后修改人name
				"name" : "lastModifierName"
			}, {
				"mapping" : "purchasing",
				"name" : "purchasing"
			}, {
				"mapping" : "contranonono",
				"name" : "contranonono"
			}, {
				"mapping" : "numberun",
				"name" : "numberun"
			}, {
				"mapping" : "contractAmt",
				"name" : "contractAmt"
			}, {
				"mapping" : "difference",
				"name" : "difference"
			}, {
				"mapping" : "rrrr",
				"name" : "rrrr"
			}, {
				"mapping" : "stopTime",
				"name" : "stopTime"
			}, {
				"mapping" : "newupdateDate",
				"name" : "newupdateDate"
			}, {
				"mapping" : "listingDate",
				"name" : "listingDate"
			}, {
				"mapping" : "总数量",
				"name" : "总数量"
			}, {
				"mapping" : "到货数量",
				"name" : "到货数量",
				convert : function(v, record) {
					if (record['到货数量'] == null) {
						return 0;
					}
					// if(daohuo[record['deviceID'] + ',' +
					// record['contractID']]){
					// }

					var d = 0;

					if (daohuo[record['deviceID'] + ',' + record['contractID']] === undefined) {// mei
						// you
						daohuo[record['deviceID'] + ',' + record['contractID']] = record['到货数量'];
					}
					d = daohuo[record['deviceID'] + ',' + record['contractID']];
					if (record['number'] <= d) {
						daohuo[record['deviceID'] + ',' + record['contractID']] = d
								- record['number'];
						return record['number'];
					} else {
						daohuo[record['deviceID'] + ',' + record['contractID']] = 0;
						return d;
					}
				}
			}, {
				"mapping" : "未到货",
				"name" : "未到货"
			}]);
	/**
	 * 
	 */
	var materialsStore = new Ext.data.GroupingStore({
				groupField : 'rrrr',
				url : __cxtPath
						+ '/device.do?method=getPurchasingListByOutContract',
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount'
						}, reRecord),
				remoteSort : true,
				baseParams : {
					contractId : contract == null ? 0 : contract.contractID
				},
				autoLoad : true,
				listeners : {
					'load' : function(store, record) {
						for (var i = 0; i < store.data.length; i++) {
							// alert(store.data.length);
							sum = record[i].data.difference;
						}
						daohuo = {};
					}

				}
			});
	/**
	 * 
	 */
	var materialsCm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({
						width : 27
					}), {
				dataIndex : 'matername',
				sortable : true,
				header : '物资名称',
				listeners : {
					'click' : function(column, grid, rowIndex, e) {// 点击名称查询所选物资采购明细
						// var DHSL = record.get('到货数量');
						// alert(DHSL);
						var record = grid.getStore().getAt(rowIndex);
						var purdelID = record.data.purdelID;
						var DHSL = record.data.到货数量;
						var getRecordpurdelID = new Ext.data.JsonStore({
									url : __cxtPath
											+ '/device.do?method=getRecordpurdelID',
									root : 'rows',
									fields : [{
												"mapping" : "contractID",// 支出合同
												"name" : "contractID"
											}, {
												"mapping" : "deviceName",// 物资名称
												"name" : "matername"
											}, {
												"mapping" : "deviceID",// 物资ID
												"name" : "deviceID"
											}, {
												"mapping" : "number",// 数量
												"name" : "namber"
											}, {
												"mapping" : "univalent",// 单价
												"name" : "DJ"
											}, {
												"mapping" : "contractNo",// 支出合同项目编号
												"name" : "contractNo"
											}, {
												"mapping" : "contractName",// 支出合同项目名称
												"name" : "contractName"
											}, {
												"mapping" : "expectArrivalDate",// 期望到货日期
												"name" : "expectArrivalDate"
											}, {
												"mapping" : "modelType",// 型号
												"name" : "modelType"
											}, {
												"mapping" : "standard",// 规格
												"name" : "standard"
											}, {
												"mapping" : "weight",// 重量
												"name" : "weight"
											}, {
												"mapping" : "referencePrice",// 官方报价
												"name" : "referencePrice"
											}, {
												"mapping" : "manufacturer.companyName",// 生产厂家
												"name" : "manufacturer"
											}, {
												"mapping" : "technicalRequirement",// 技术要求
												"name" : "technicalRequirement"
											}, {
												"mapping" : "remark",// 备注
												"name" : "remark"
											}, {
												"mapping" : "registrationDate",// 登记日期
												"name" : "registrationDate"
											}, {
												"mapping" : "userID",// 登记人
												"name" : "userID"
											}, {
												"mapping" : "purdelID",// 明细ID
												"name" : "purdelID"
											}, {
												"mapping" : "lastModifier",// 最后修改人ID
												"name" : "lastModifier"
											}, {
												"mapping" : "lastModified",// 最后修改时间
												"name" : "lastModified"
											}, {
												"mapping" : "userName",// 登记人name
												"name" : "userName"
											}, {
												"mapping" : "lastModifierName",// 最后修改人name
												"name" : "lastModifierName"
											}, {
												"mapping" : "purchasing",
												"name" : "purchasing"
											}, {
												"mapping" : "contranonono",
												"name" : "contranonono"
											}, {
												"mapping" : "numberun",
												"name" : "numberun"
											}, {
												"mapping" : "contractAmt",
												"name" : "contractAmt"
											}, {
												"mapping" : "difference",
												"name" : "difference"
											}, {
												"mapping" : "rrrr",
												"name" : "rrrr"
											}, {
												"mapping" : "currencyName",
												"name" : "currencyName"
											}, {
												"mapping" : "stopTime",
												"name" : "stopTime"
											}, {
												"mapping" : "newupdateDate",
												"name" : "newupdateDate"
											}, {
												"mapping" : "listingDate",
												"name" : "listingDate"
											}, {
												"mapping" : "总数量",
												"name" : "总数量"
											}, {
												"mapping" : "到货数量",
												"name" : "到货数量"
											}, {
												"mapping" : "未到货",
												"name" : "未到货"
											}],

									baseParams : {
										purdelID : record.data.purdelID
									},
									autoLoad : true
								});

						getRecordpurdelID.on('load', function(store, record,
								opts) {
							materialsWin.show();
							amendProcurementForm.getForm().clear();
							amendProcurementForm.getForm()
									.findField('contractItemsID')
									.setValue(record[0].data.contractID);
							amendProcurementForm.getForm().findField('DHSL')
									.setValue(DHSL);
							if (!decisionFlag) {
								amendProcurementForm.getForm()
										.findField('purchasePrice')
										.setValue('--');
							} else {
								amendProcurementForm.getForm()
										.findField('purchasePrice')
										.setValue(record[0].data.DJ);
							}
							amendProcurementForm.getForm()
									.findField('hopearrivalDate')
									.setValue(record[0].data.expectArrivalDate);
							amendProcurementForm.getForm()
									.findField('modifyPurchaseQuantity')
									.setValue(record[0].data.namber);
							amendProcurementForm.getForm()
									.findField('equipmentID')
									.setValue(record[0].data.deviceID);
							amendProcurementForm.getForm()
									.findField('apparatusName')
									.setValue(record[0].data.matername);
							amendProcurementForm.getForm()
									.findField('itemCode')
									.setValue(record[0].data.contractNo);
							amendProcurementForm.getForm()
									.findField('equipmentName')
									.setValue(record[0].data.matername);
							amendProcurementForm.getForm()
									.findField('patternName')
									.setValue(record[0].data.modelType);
							amendProcurementForm.getForm()
									.findField('specificationName')
									.setValue(record[0].data.standard);
							amendProcurementForm.getForm()
									.findField('modifyWeightName')
									.setValue(record[0].data.weight);

							if (!Ext.isEmpty(record[0].data.listingDate)) {
								amendProcurementForm.getForm()
										.findField('TFSCSJ2')
										.setValue((record[0].data.listingDate)
												.substring(0, 10));
							} else {
								amendProcurementForm.getForm()
										.findField('TFSCSJ2')
										.setValue(record[0].data.listingDate);
							}
							if (!Ext.isEmpty(record[0].data.stopTime)) {
								amendProcurementForm.getForm()
										.findField('TCSJ2')
										.setValue((record[0].data.stopTime)
												.substring(0, 10));
							} else {
								amendProcurementForm.getForm()
										.findField('TCSJ2')
										.setValue(record[0].data.stopTime);
							}

							if (!decisionFlag) {
								amendProcurementForm
										.getForm()
										.findField('officialQuotation')
										.setValue('<div style="margin-top:5px"><font color =red>权限不足</font></div>');
							} else {
								amendProcurementForm
										.getForm()
										.findField('officialQuotation')
										.setValue(record[0].data.referencePrice);
							}
							if (record[0].data.manufacturer == null) {

								amendProcurementForm.getForm()
										.findField('productionAddress')
										.setValue('--');
							} else {
								amendProcurementForm.getForm()
										.findField('productionAddress')
										.setValue(record[0].data.manufacturer);
							}
							amendProcurementForm
									.getForm()
									.findField('technicalRequire')
									.setValue(record[0].data.technicalRequirement);
							amendProcurementForm.getForm()
									.findField('modifyRemarksName')
									.setValue(record[0].data.remark);
							amendProcurementForm.getForm()
									.findField('modifySubscriberName')
									.setValue(record[0].data.userName);
							amendProcurementForm.getForm()
									.findField('registrationDate')
									.setValue(record[0].data.registrationDate);
							amendProcurementForm.getForm()
									.findField('PurchasingDetailID')
									.setValue(record[0].data.purdelID);
							amendProcurementForm.getForm()
									.findField('lastModifiedDate')
									.setValue(record[0].data.lastModified);
							amendProcurementForm.getForm()
									.findField('lastModifiedName')
									.setValue(record[0].data.lastModifierName);
							amendProcurementForm.getForm()
									.findField('purchasingID')
									.setValue(record[0].data.purchasing);
							if (!decisionFlag) {
								amendProcurementForm.getForm()
										.findField('theTotalAmount')
										.setValue('权限不足');
							} else {
								amendProcurementForm.getForm()
										.findField('theTotalAmount')
										.setValue(record[0].data.DJ
												* record[0].data.namber);
							}
							amendProcurementForm.getForm()
									.findField('suOonename')
									.setValue(record[0].data.DJ
											* record[0].data.namber);
							amendProcurementForm
									.getForm()
									.findField('moneyUnit')
									.setValue(Ext.getCmp('currencyTypeID').lastSelectionText);
							amendProcurementForm.getForm()
									.findField('modifyLastName')
									.setValue(record[0].data.currencyName);

							if (record[0].data.stopTime >= new Date()
									.format('Y-m-d')
									&& record[0].data.stopTime > record[0].data.listingDate) {
								amendProcurementForm.getForm()
										.findField('yieldingFettle2')
										.setValue('在产');
							}
							if (Ext.isEmpty(record[0].data.stopTime)
									&& record[0].data.listingDate < new Date()
											.format('Y-m-d')) {
								amendProcurementForm.getForm()
										.findField('yieldingFettle2')
										.setValue('在产');
							}
							if (record[0].data.stopTime <= new Date()
									.format('Y-m-d')
									&& !Ext.isEmpty(record[0].data.listingDate)) {
								amendProcurementForm.getForm()
										.findField('yieldingFettle2')
										.setValue('停产');
							}
							if (Ext.isEmpty(record[0].data.listingDate)
									&& record[0].data.stopTime <= new Date()
											.format('Y-m-d')) {
								amendProcurementForm.getForm()
										.findField('yieldingFettle2')
										.setValue('停产');
							}
							if (Ext.isEmpty(record[0].data.listingDate)
									&& record[0].data.stopTime > new Date()
											.format('Y-m-d')) {
								amendProcurementForm.getForm()
										.findField('yieldingFettle2')
										.setValue('在产');
							}
							if (record[0].data.listingDate > new Date()
									.format('Y-m-d')
									&& record[0].data.stopTime > new Date()
											.format('Y-m-d')) {
								amendProcurementForm.getForm()
										.findField('yieldingFettle2')
										.setValue('');
							}

						});

					}
				}

			}, {
				dataIndex : 'namber',
				sortable : true,
				align : 'right',
				header : '采购数量'
			}, {
				dataIndex : 'DJ',
				sortable : true,
				header : '单价',
				align : 'right',
				renderer : function(value, metaData, record, rowIndex,
						colIndex, store) {
					if (!decisionFlag) {
						return formatCurrency("", '0', "");
					} else {
						return formatCurrency("", value, "");
					}
				}
			}, {
				dataIndex : 'numberun',
				sortable : true,
				header : '总额',
				align : 'right',
				renderer : function(value, metaData, record, rowIndex,
						colIndex, store) {
					if (!decisionFlag) {
						return formatCurrency("", '0', "");
					} else {
						return formatCurrency("", value, "");
					}
				}
			}, {
				dataIndex : '到货数量',
				sortable : true,
				align : 'right',
				header : '到货数量'
			}, {
				dataIndex : '',
				sortable : true,
				align : 'right',
				header : '未到货数量',
				renderer : function(value, metaData, record, rowIndex,
						colIndex, store) {
					return record.get('namber') - record.get('到货数量');
				}
			}, {
				dataIndex : 'rrrr',
				hidden : true,
				header : "rrr"
			}, {
				dataIndex : 'difference',
				hidden : true,
				header : " "
			}]);
	/**
	 * gird自动分类
	 */

	var sum;
	var gridPanel = new Ext.grid.GridPanel({
		id : 'gridPanel',
		cm : materialsCm,
		view : new Ext.grid.GroupingView({
			forceFit : true,
			// enableGroupingMenu : false,
			// startCollapsed : true,
			groupTextTpl : '{gvalue} ({[values.rs.length]} {[values.rs.length >= 1 ? "条" : "条"]})'// ?

		}),
		title : '采购明细',
		store : materialsStore,
		region : 'north'
	});

	var eastPanel = new Ext.TabPanel({
				region : 'east',
				width : 550,
				autoScroll : true,
				split : true,
				collapsible : true, // 是否有右上方展开收缩条
				collapseMode : 'mini', // 将滚动收缩条模式‘迷你’
				// layout : 'border',
				stateful : true,
				stateId : 'editContractout-eastpanel',
				stateEvent : ['resize'],
				activeTab : 0,
				items : [relatedGrid, gridPanel, attGrid, deviceInGridPanel]
			});
	// eastPanel.set
	/**
	 * 采购清单明细操作
	 */
	// var materialsWin;
	// function showMaterials() {
	var amendProcurementForm = new Ext.FormPanel({
		border : false,
		region : 'center',
		baseCls : 'x-plain',
		margins : '50 0 0 5',
		labelAlign : 'right',
		defaults : {
			anchor : '90%'
		},
		items : [{
			xtype : 'panel',
			labelAlign : 'right',
			baseCls : 'x-plain',// 背景渲染
			margins : '5 0 0 5',
			layout : 'form',
			border : false,
			width : 850,
			items : [{
				xtype : 'panel',
				labelAlign : 'right',
				baseCls : 'x-plain',// 背景渲染
				margins : '5 0 0 5',
				layout : 'column',

				border : false,
				items : [{
					// columnWidth : .5,
					width : 400,
					layout : "form",
					margins : '5 0 0 5',
					baseCls : 'x-plain',
					items : [{
								xtype : 'combo',
								name : 'itemCode',
								width : 220,
								fieldLabel : '支出合同编号',
								selectOnFocus : true,
								readOnly : true,
								editable : false,
								allowBlank : false,
								triggerAction : 'all',
								emptyText : '请选择合同',
								lazyInit : false
							}, {
								xtype : 'hidden'
							}, new Ext.ux.form.ChooseField({
								fieldLabel : '设备名称',
								allowBlank : false,
								name : 'apparatusName',
								width : 220,
								winConfig : {
									title : '物资信息'
								},
								baseParamFun : function() {
									winFlag = "bWin";
									return {
										start : 0,
										limit : 20
									};
								},
								allowBlank : false,// 弹出
								chooseUrl : __cxtPath
										+ '/resources/choose/deviceproject.json.jsp',
								setValueFun : function(o) {
									if (Ext.isEmpty(o)) {
										Ext.Msg.alert('提示', '您还没有选择，请选择');
									}
									if (!Ext.isEmpty(o.data.停产时间)
											&& o.data.停产时间 <= new Date()
													.format('Y-m-d')) {
										Ext.Msg.alert('提示', '此物资已停产');
									}
									amendProcurementForm.getForm()
											.findField('apparatusName')
											.setValue(o.data.物资名称);
									amendProcurementForm.getForm()
											.findField('equipmentName')
											.setValue(o.data.物资名称);
									amendProcurementForm.getForm()
											.findField('equipmentID')
											.setValue(o.data.物资ID);
									amendProcurementForm.getForm()
											.findField('patternName')
											.setValue(o.data.型号);
									amendProcurementForm.getForm()
											.findField('specificationName')
											.setValue(o.data.规格);
									amendProcurementForm.getForm()
											.findField('modifyWeightName')
											.setValue(o.data.重量);
									if (!decisionFlag) {
										amendProcurementForm.getForm()
												.findField('officialQuotation')
												.setValue('--');
									} else {
										amendProcurementForm.getForm()
												.findField('officialQuotation')
												.setValue(o.data.官方报价);
									}
									amendProcurementForm.getForm()
											.findField('productionAddress')
											.setValue(o.data.生产厂家名称);
									// amendProcurementForm.getForm()
									// .findField('modifyLastName')
									// .setValue(o.data.currency.name);
									if (!Ext.isEmpty(o.data.投入市场日期)) {
										amendProcurementForm.getForm()
												.findField('TFSCSJ2')
												.setValue((o.data.投入市场日期)
														.substring(0, 10));
									} else {
										amendProcurementForm.getForm()
												.findField('TFSCSJ2')
												.setValue(o.data.投入市场日期);
									}
									if (!Ext.isEmpty(o.data.停产时间)) {
										amendProcurementForm.getForm()
												.findField('TCSJ2')
												.setValue((o.data.停产时间)
														.substring(0, 10));
									} else {
										amendProcurementForm.getForm()
												.findField('TCSJ2')
												.setValue(o.data.停产时间);
									}
									amendProcurementForm.getForm()
											.findField('yieldingFettle2')
											.setValue(o.data.物资生产状态名称);

									// if(o.data.stopTime >=
									// o.data.newupdateDate && o.data.stopTime >
									// o.data.listingDate){
									// amendProcurementForm.getForm().findField('yieldingFettle2')
									// .setValue('在产');
									// }
									// if(Ext.isEmpty(o.data.stopTime) &&
									// o.data.listingDate<o.data.newupdateDate){
									// amendProcurementForm.getForm().findField('yieldingFettle2')
									// .setValue('在产');
									// }
									// if(o.data.stopTime <=
									// o.data.newupdateDate &&
									// !Ext.isEmpty(o.data.listingDate)){
									// amendProcurementForm.getForm().findField('yieldingFettle2')
									// .setValue('停产');
									// }
									// if(Ext.isEmpty(o.data.listingDate) &&
									// o.data.stopTime <= o.data.newupdateDate){
									// amendProcurementForm.getForm().findField('yieldingFettle2')
									// .setValue('停产');
									// }
									// if(Ext.isEmpty(o.data.listingDate) &&
									// o.data.stopTime > o.data.newupdateDate){
									// amendProcurementForm.getForm().findField('yieldingFettle2')
									// .setValue('在产');
									// }
									// if(o.data.listingDate>o.data.newupdateDate
									// && o.data.stopTime >
									// o.data.newupdateDate){
									// amendProcurementForm.getForm().findField('yieldingFettle2')
									// .setValue('');
									// }

								},
								listeners : {
									render : function() {// 渲染后添加click事件
										Ext.fly(this.el).on('dblclick',
												function(e, t) {

													parent.xMis
															.turn(
																	'company-info-'
																			+ amendProcurementForm
																					.getForm()
																					.findField('equipmentID')
																					.getValue(),
																	"[物资]"
																			+ amendProcurementForm
																					.getForm()
																					.findField('equipmentName')
																					.getValue(),
																	__cxtPath
																			+ '/device.do?method=getMaterialnfo&id='
																			+ amendProcurementForm
																					.getForm()
																					.findField('equipmentID')
																					.getValue(),
																	'menu-company-info',
																	true);
												});
									}
								}
							}), {
								xtype : 'panel',
								layout : 'column',
								baseCls : 'x-plain',// 背景渲染
								border : false,
								items : [{
									xtype : 'panel',
									layout : 'form',
									columnWidth : 0.7,
									baseCls : 'x-plain',// 背景渲染
									labelAlign : 'right',
									border : false,
									items : [{
										xtype : 'formatnum',
										allowBlank : false,
										fieldLabel : '采购单价',
										name : 'purchasePrice',
										anchor : '99%',
										listeners : {
											change : function(field, newValue,
													oldValue, eOpts) {
												amendProcurementForm
														.getForm()
														.findField('theTotalAmount')
														.setValue(amendProcurementForm
																.getForm()
																.findField('modifyPurchaseQuantity')
																.getValue()
																* amendProcurementForm
																		.getForm()
																		.findField('purchasePrice')
																		.getValue());
											}
										}
									}]
								}, {
									xtype : 'displayfield',
									width : 45,
									id : 'moneyUnitID',
									name : 'moneyUnit',
									align : 'right'
								}]
							}, {
								xtype : "numberfield",
								fieldLabel : "采购数量",
								name : 'modifyPurchaseQuantity',
								style : 'text-align:right',
								allowBlank : false,
								width : 220,
								listeners : {
									change : function(field, newValue,
											oldValue, eOpts) {
										amendProcurementForm
												.getForm()
												.findField('theTotalAmount')
												.setValue(amendProcurementForm
														.getForm()
														.findField('modifyPurchaseQuantity')
														.getValue()
														* amendProcurementForm
																.getForm()
																.findField('purchasePrice')
																.getValue());
									}
								}
							}, {
								xtype : "formatnum",
								fieldLabel : "总金额",
								name : 'theTotalAmount',
								editable : false,
								readOnly : true,
								allowBlank : false,
								width : 220
							}, {
								xtype : "displayfield",
								fieldLabel : "支出合同项目ID",
								name : 'contractItemsID',
								hidden : true
							}, {
								xtype : "hidden",
								fieldLabel : "到货数量",
								name : 'DHSL'
							}, {
								xtype : "formatnum",
								fieldLabel : "金额总数",
								name : 'suOonename',
								hidden : true
							}, {
								xtype : "displayfield",
								fieldLabel : "采购清单明细ID",
								name : 'PurchasingDetailID',
								hidden : true
							}, {
								xtype : "displayfield",
								fieldLabel : "采购清单ID",
								name : 'purchasingID',
								hidden : true
							}, {
								xtype : 'datefield',
								fieldLabel : '期望到货日期',
								name : 'hopearrivalDate',
								format : 'Y-m-d',
								width : 220,
								getAsycnValue : function(callback) {
									callback.call(this, Ext.util.Format.date(
													this.getValue(),
													this.format));
								}
							}, {
								xtype : 'textarea',
								fieldLabel : '技术要求',
								name : 'technicalRequire',
								width : 230,
								height : 60,
								listeners : {
									'render' : function(c) {
										Ext.QuickTips.register({
													target : c.getEl(),
													text : '"技术要求"最多可输入500字'
												});
									}
								}
							}, {
								xtype : 'textarea',
								fieldLabel : '备注',
								name : 'modifyRemarksName',
								width : 230,
								height : 60,
								listeners : {
									'render' : function(c) {
										Ext.QuickTips.register({
													target : c.getEl(),
													text : '"备注"最多可输入500字'
												});
									}
								}
							}]
				}, {
					xtype : 'fieldset',
					title : '物资信息',
					collapsible : true,// 收缩
					baseCls : 'x-plain',
					margins : '0 0 0 5',
					width : 300,
					items : [{
								xtype : "displayfield",
								fieldLabel : "物资名称",
								name : 'equipmentName'
							}, {
								xtype : "displayfield",
								fieldLabel : "物资ID",
								name : 'equipmentID',
								hidden : true
							}, {

								xtype : "displayfield",
								fieldLabel : "型号",
								name : 'patternName'
							}, {
								xtype : "displayfield",
								fieldLabel : "规格",
								name : 'specificationName'
							}, {
								xtype : "displayfield",
								fieldLabel : "重量",
								name : 'modifyWeightName'
							}, {
								xtype : 'panel',
								layout : 'column',
								baseCls : 'x-plain',// 背景渲染
								border : false,
								items : [{
											xtype : 'panel',
											layout : 'form',
											columnWidth : 0.63,
											baseCls : 'x-plain',// 背景渲染
											labelAlign : 'right',
											border : false,
											items : [{
														xtype : 'displayfield',
														allowBlank : false,
														fieldLabel : '采购单价',
														name : 'officialQuotation',
														anchor : '99%'
													}]
										}, {
											xtype : 'displayfield',
											width : 45,
											name : 'modifyLastName',
											align : 'right'
										}]
							}, {
								xtype : "displayfield",
								fieldLabel : "生产厂家",
								name : 'productionAddress'
							}, {
								xtype : 'displayfield',
								fieldLabel : '投入市场日期',
								name : 'TFSCSJ2'
							}, {
								xtype : 'displayfield',
								fieldLabel : '停产时间',
								name : 'TCSJ2'
							}, {
								xtype : 'displayfield',
								fieldLabel : '物资生产状态',
								name : 'yieldingFettle2'
							}]
				}]
			}, {
				xtype : 'panel',
				layout : 'column',
				baseCls : 'x-plain',
				border : false,
				items : [{
							xtype : 'panel',
							layout : 'form',
							baseCls : 'x-plain',
							columnWidth : 0.25,
							labelAlign : 'right',
							labelWidth : 90,
							border : false,
							items : [{
										fieldLabel : '登记人',
										name : 'modifySubscriberName',
										xtype : 'displayfield'
									}, {
										fieldLabel : '最后修改人',
										name : 'lastModifiedName',
										xtype : 'displayfield'
									}]
						}, {
							xtype : 'panel',
							layout : 'form',
							columnWidth : 0.25,
							baseCls : 'x-plain',
							labelAlign : 'right',
							labelWidth : 90,
							border : false,
							items : [{
										fieldLabel : '登记日期',
										xtype : 'displayfield',
										name : 'registrationDate'
									}, {

										fieldLabel : '最后修改日期',
										xtype : 'displayfield',
										name : 'lastModifiedDate'
									}]
						}]
			}]
		}]
	});

	var amendPromainPanel = new Ext.Panel({
		region : 'center',
		border : false,
		layout : 'border',
		items : [amendProcurementForm],
		tbar : {
			xtype : 'toolbar',
			disabled : false,
			items : [{
				ref : '../modifyButton',
				id : 'modifyButton',
				xtype : 'button',
				text : '修改',
				iconCls : 'ok',
				handler : function() {

					if (amendProcurementForm.getForm().findField('DHSL')
							.getValue() != 0) {
						Ext.MessageBox.confirm('提示',
								'此物资已做入库，如果修改可能会对数据产生严重影响是否继续？', function(btn,
										text) {
									if (btn == 'yes') {
										var sum = 0;
										amendProcurementForm.getForm().submit({
											url : __cxtPath
													+ '/device.do?method=getPurchasingDetailTotal',
											root : 'rows',
											fields : [{
														name : "tcontractAmt",
														mapping : "tcontractAmt"
													}, {
														name : "tnumber",
														mapping : "tnumber"
													}, {
														name : "tunivalent",
														mapping : "tunivalent"
													}],
											params : {
												contractID : amendProcurementForm
														.getForm()
														.findField('contractItemsID')
														.getValue()
											},
											autoLoad : true,
											waitMsg : '正在保存，请稍候……',
											waitTitle : '正在处理',
											success : function(form, action) {
												for (var i = 0; i < Ext
														.decode(action.response.responseText).totalCount; i++) {
													var rnumber = Ext
															.decode(action.response.responseText).rows[i].tnumber;
													var runivalent = Ext
															.decode(action.response.responseText).rows[i].tunivalent;
													sum += rnumber * runivalent;
												}
												if (amendProcurementForm
														.getForm()
														.findField('theTotalAmount')
														.getValue() <= (Ext
														.decode(action.response.responseText).rows[0].tcontractAmt - sum)
														+ (amendProcurementForm
																.getForm()
																.findField('suOonename')
																.getValue())) {

													amendProcurementForm
															.getForm().submit({

																url : __cxtPath
																		+ '/device.do?method=updatePurchasingDetail',
																params : {
																	materialID : amendProcurementForm
																			.getForm()
																			.findField('equipmentID')
																			.getValue(),
																	purdelID : amendProcurementForm
																			.getForm()
																			.findField('PurchasingDetailID')
																			.getValue(),
																	updateDate6 : new Date()
																			.format('Y-m-d'),
																	userID : __emp.userID,// 最后修改人ID
																	registerName5 : amendProcurementForm
																			.getForm()
																			.findField('lastModifiedName')
																			.getValue()
																	// 最后修改人name
																},
																waitMsg : '正在保存，请稍候……',
																waitTitle : '正在处理',
																success : function(
																		form,
																		action) {
																	Ext.Msg
																			.alert(
																					action.result.messageTitle,
																					action.result.message,
																					function() {
																						materialsWin
																								.hide();
																						Ext
																								.getCmp('gridPanel')
																								.getStore()
																								.reload();
																					});
																},
																failure : function(
																		form,
																		action) {

																},
																method : "post"
															});
												} else {
													Ext.Msg.alert('提示',
															'总采购金额已超出项目对应金额');
												}
											},
											failure : function(form, action) {
											}
										});
									}
								});// 提示括号
					} else {
						var sum = 0;
						amendProcurementForm.getForm().submit({
							url : __cxtPath
									+ '/device.do?method=getPurchasingDetailTotal',
							root : 'rows',
							fields : [{
										name : "tcontractAmt",
										mapping : "tcontractAmt"
									}, {
										name : "tnumber",
										mapping : "tnumber"
									}, {
										name : "tunivalent",
										mapping : "tunivalent"
									}],
							params : {
								contractID : amendProcurementForm.getForm()
										.findField('contractItemsID')
										.getValue()
							},
							autoLoad : true,
							waitMsg : '正在保存，请稍候……',
							waitTitle : '正在处理',
							success : function(form, action) {
								for (var i = 0; i < Ext
										.decode(action.response.responseText).totalCount; i++) {
									var rnumber = Ext
											.decode(action.response.responseText).rows[i].tnumber;
									var runivalent = Ext
											.decode(action.response.responseText).rows[i].tunivalent;
									sum += rnumber * runivalent;
								}
								if (amendProcurementForm.getForm()
										.findField('theTotalAmount').getValue() <= (Ext
										.decode(action.response.responseText).rows[0].tcontractAmt - sum)
										+ (amendProcurementForm.getForm()
												.findField('suOonename')
												.getValue())) {

									amendProcurementForm.getForm().submit({

										url : __cxtPath
												+ '/device.do?method=updatePurchasingDetail',
										params : {
											materialID : amendProcurementForm
													.getForm()
													.findField('equipmentID')
													.getValue(),
											purdelID : amendProcurementForm
													.getForm()
													.findField('PurchasingDetailID')
													.getValue(),
											updateDate6 : new Date()
													.format('Y-m-d'),
											userID : __emp.userID,// 最后修改人ID
											registerName5 : amendProcurementForm
													.getForm()
													.findField('lastModifiedName')
													.getValue()
											// 最后修改人name
										},
										waitMsg : '正在保存，请稍候……',
										waitTitle : '正在处理',
										success : function(form, action) {
											Ext.Msg.alert(
													action.result.messageTitle,
													action.result.message,
													function() {
														materialsWin.hide();
														Ext.getCmp('gridPanel')
																.getStore()
																.reload();
													});
										},
										failure : function(form, action) {

										},
										method : "post"
									});
								} else {
									Ext.Msg.alert('提示', '总采购金额已超出项目对应金额');
								}
							},
							failure : function(form, action) {
							}
						});
					}
				}
			}, {
				ref : '../deleteButton',
				id : 'deleteButton',
				xtype : 'button',
				text : '删除',
				iconCls : 'remove',
				handler : function() {
					if (amendProcurementForm.getForm().findField('DHSL')
							.getValue() != 0) {
						Ext.MessageBox.confirm('提示',
								'此物资已做入库，如果修改可能会对数据产生严重影响是否继续？', function(btn,
										text) {
									if (btn == 'yes') {
										amendProcurementForm.getForm().submit({
											url : __cxtPath
													+ '/device.do?method=deletePurchasingDetail',
											params : {
												purdelID : amendProcurementForm
														.getForm()
														.findField('PurchasingDetailID')
														.getValue()
											},
											waitMsg : '正在保存，请稍候……',
											waitTitle : '正在处理',
											success : function(form, action) {
												Ext.Msg
														.alert(
																action.result.messageTitle,
																action.result.message,
																function() {
																	materialsWin
																			.hide();
																	Ext
																			.getCmp('gridPanel')
																			.getStore()
																			.reload();
																});
											},
											failure : function(form, action) {
											},
											method : "post"
										});
									}
								});
					} else {
						amendProcurementForm.getForm().submit({
							url : __cxtPath
									+ '/device.do?method=deletePurchasingDetail',
							params : {
								purdelID : amendProcurementForm.getForm()
										.findField('PurchasingDetailID')
										.getValue()
							},
							waitMsg : '正在保存，请稍候……',
							waitTitle : '正在处理',
							success : function(form, action) {
								Ext.Msg.alert(action.result.messageTitle,
										action.result.message, function() {
											materialsWin.hide();
											Ext.getCmp('gridPanel').getStore()
													.reload();
										});
							},
							failure : function(form, action) {
							},
							method : "post"
						});
					}
				}
			}]

		}
	});
	if (!decisionFlag) {
		Ext.getCmp('modifyButton').disable();
		Ext.getCmp('deleteButton').disable();
	} else {
		Ext.getCmp('modifyButton').enable();
		Ext.getCmp('deleteButton').enable();
	}
	var materialsWin = new Ext.Window({
				modal : true,
				title : '变更采购明细',
				width : 800,
				height : 490,
				layout : 'border',
				closeAction : 'hide',
				items : [amendPromainPanel]
			});
	var mainPanel = new Ext.Panel({
		region : 'center',
		border : false,
		layout : 'border',
		items : [contractFormPanel, eastPanel],
		tbar : {
			xtype : 'toolbar',
			disabled : true,
			items : [{
				ref : '../saveButton',
				xtype : 'button',
				text : '保存',
				iconCls : 'ok',
				handler : function() {
					var refreshflag = false;
					if (contract == null) {
						refreshflag = true;
					}
					contractFormPanel.getForm().submit({
						url : __cxtPath + '/contractMain.do',
						params : {
							method : 'saveContractInfoNew',
							warrantyUnit : contractFormPanel.getForm()
									.findField('warrantyUnit').getValue(),
							contractCodeExist : contract
									? contract.contractNo
									: ''
						},
						waitMsg : '正在保存，请稍候……',
						waitTitle : '正在处理',
						success : function(form, action) {
							var message = action.result.message;
							var contractCode = action.result.attributes.contractCode;
							if (action.result.attributes.existflag == true) {
								message = "'"
										+ Ext
												.decode(action.result.attributes.json)[0]['合同编号']
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
										+ ",新的甲方合同编号：'" + contractCode + "'";
							}
							Ext.Msg.alert(action.result.messageTitle, message,
									function() {
										contractFormPanel
												.getForm()
												.findField('contractID')
												.setValue(action.result.attributes.contractID);
										// contract = {
										// contractID :
										// action.result.attributes.contractID,
										// contractAmt : contractFormPanel
										// .getForm()
										// .findField('contractAmt')
										// .getValue(),
										// buyerID : contractFormPanel
										// .getForm()
										// .findField('aCompany_id')
										// .getValue()
										// };
										mainPanel.paymentButton
												.setVisible(true);
										mainPanel.invoiceButton
												.setVisible(true);
										mainPanel.addRelated.setVisible(true);
										mainPanel.attButton.setVisible(true);
										mainPanel.addRelateds.setVisible(true);
										mainPanel.addreButton.setVisible(true);
										// mainPanel.modifyButton.setVisible(true);
										if (contract) {
											contract.contractAmt = contractFormPanel
													.getForm()
													.findField('contractAmt')
													.getValue();
											relatedStore.load({
												params : {
													contractID : contract.contractID,
													contractType : 2
												}
											});
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
												parent.xMis.closeTab(window);
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
			}, '-', {
				text : '合同款收付登记',
				ref : '../paymentButton',
				iconCls : 'document',
				hidden : true,
				menu : {
					items : []
				}
			}, {
				text : '票据收发登记',
				ref : '../invoiceButton',
				iconCls : 'document',
				hidden : true,
				menu : {
					items : []
				}
			}, '-', {
				xtype : 'button',
				ref : '../addSubContractButton',
				text : '添加子合同',
				iconCls : 'add',
				hidden : true,
				handler : function() {
					window.location.href = __cxtPath
							+ '/contractMain.do?method=contract&parentID='
							+ contract.contractID + '&parentNo='
							+ contract.contractNo + '&parentName='
							+ contract.contractName + '&parentAbb='
							+ contract.abbreviation;
				}
			}, {
				ref : '../addRelated',
				text : '添加对应合同',
				hidden : true,
				iconCls : 'add',
				handler : function() {
					eastPanel.setActiveTab(0);
					showAddRelated(relatedStore);
				}
			}, '-', {
				xtype : 'button',
				text : '添加采购明细',
				hidden : contract == null ? true : false,
				iconCls : 'add',
				ref : '../addreButton',
				handler : function() {
					eastPanel.setActiveTab(1);
					AddProcurementDetails(materialsStore);
				}

			}, '-', {
				ref : '../addRelateds',
				hidden : contract == null ? true : false,
				text : '到货',
				iconCls : 'add',
				handler : function() {

					com.bitc.xmis.device.windowShow('create', '',
							contract.contractID, 0, '', '', '');
					eastPanel.setActiveTab(3);
				}
			}, '-', new Ext.ux.UploadButton({
						ref : '../attButton',
						hidden : true,
						reuses : false,
						attachmentTypeID : com.bitc.xmis.Attachment.contract,
						REFIDFun : function() {
							eastPanel.setActiveTab(2);
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
				xtype : 'button',
				text : '刷新',
				hidden : true,
				iconCls : 'reload',
				ref : '../refreshButton',
				handler : function(tabs, item) {
					location.reload();
				}
			}]
		}
	});

	new Ext.Viewport({
				border : false,
				layout : 'border',
				items : [mainPanel]
			});

	if (contract == null) {
		Ext.getCmp('contractCodeCtPanelId').remove('contractCodeExistPanelId');
		Ext.getCmp('contractCodePanelId').show();
		mainPanel.getTopToolbar().enable();
	}
	if (contract != null) {
		// 显示合同明细
		Ext.apply(contract, {
			contractType : contract.contractTypeID,
			contractSourceType : contract.sourceTypeID,
			gSourceTypeID : contract.govermentContractSourceID,
			GSourceTypeName : contract.govermentContractSourceName,
			// contractCode : contract.contractNo,
			contractCodeExist : contract.contractNo,
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
			delivery_id : contract.deliveryID,
			cEmp_name : contract.deliveryName,
			parentNo : contract.parentCode,
			projectID : (contract.projectOBJ && contract.projectOBJ.projectID != 0)
					? contract.projectOBJ.projectID
					: '',
			projectCode : (contract.projectOBJ && contract.projectOBJ.projectID != 0)
					? contract.projectOBJ.projectCode
					: '',
			projName : (contract.projectOBJ && contract.projectOBJ.projectID != 0)
					? contract.projectOBJ.projectName
					: '',
			newcontractflag : contract.newflag
		});
		Ext.getCmp('contractCodeCtPanelId').remove('contractCodePanelId');
		Ext.getCmp('contractCodeExistPanelId').show();
		contractFormPanel.getForm().setValues(contract);
		setFormReadOnly(contractFormPanel, true);
		mainPanel.paymentButton.setVisible(true);
		mainPanel.addRelateds.setVisible(true);
		mainPanel.addreButton.setVisible(true);
		mainPanel.attButton.setVisible(true);
		mainPanel.invoiceButton.setVisible(true);
		mainPanel.refreshButton.setVisible(true);
		if (!contract.projectOBJ || contract.projectOBJ.projectID != 0) {
			mainPanel.addSubContractButton.setVisible(true);
		}
		mainPanel.addRelated.setVisible(true);
		//		
		// mainPanel.modifyButton.setVisible(true);
		mainPanel.getTopToolbar().doLayout();

		// 科质部权限
		if (!hasContractControlOnCompanyLevel) {
			com.bitc.xmis.util.ExtUtil.disableField(Ext
							.getCmp('contractNameId'), Ext
							.getCmp('contractAmtCtId'), Ext
							.getCmp('aCompany_name'), Ext
							.getCmp('bCompany_name'), Ext
							.getCmp('signatureDateId'));
			mainPanel.addRelated.hide(true);
			// mainPanel.addreButton.hide(true);
			Ext.getCmp('directorName').allowBlank = false;
		}

		if (contractOpType == 5) {// 对合同的操作类型
			mainPanel.getTopToolbar().enable();
			// attGrid.setActionColumnHidden(false);
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
						contractType : 2
					}
				});
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
		// var temp = "<table
		// width='100%'><tr><td>已付合同款：{0}</td><td>未付合同款：{1}</td></tr><tr><td>已报销金额：{2}</td><td>已付款未报销金额：{3}</td></tr></table>";

		var ht = "<table width='100%'><tr><td>已付合同款："
				+ formatCurrency(currencyType, decisionFlag
								? contract.earnedTotalAmt + ''
								: 0 + '',
						Color_Font_Contract_Total_UnAccount_Amt)
				+ "</td><td>"
				+ "未付合同款："
				+ formatCurrency(currencyType, decisionFlag
								? contract.arrearageTotalAmt + ''
								: 0 + '',
						Color_Font_Contract_Total_UnAccount_Amt)
				+ "</td></tr>"
				+ "<tr><td>已报销金额："
				+ formatCurrency(currencyType, decisionFlag
								? contract.invoicedTotalAmt + ''
								: 0 + '',
						Color_Font_Contract_Total_UnAccount_Amt)
				+ "</td><td>"
				+ "已付款未报销金额："
				+ formatCurrency(currencyType, decisionFlag ? 0
								- contract.unCreditedTotalAmt + '' : 0 + '',
						Color_Font_Contract_Total_UnAccount_Amt)
				+ "</td></tr></table>";

		if (!decisionFlag) {
			ht = "<table width='100%'>"
					+ "<tr><td><font color=red>提示：&nbsp;权限不足，无法查看金额及相关信息！</font></td></tr>"
					+ "<tr><td>已付合同款："
					+ formatCurrency(currencyType, decisionFlag
									? contract.earnedTotalAmt + ''
									: 0 + '',
							Color_Font_Contract_Total_UnAccount_Amt)
					+ "</td><td>"
					+ "未付合同款："
					+ formatCurrency(currencyType, decisionFlag
									? contract.arrearageTotalAmt + ''
									: 0 + '',
							Color_Font_Contract_Total_UnAccount_Amt)
					+ "</td></tr>"
					+ "<tr><td>已报销金额："
					+ formatCurrency(currencyType, decisionFlag
									? contract.invoicedTotalAmt + ''
									: 0 + '',
							Color_Font_Contract_Total_UnAccount_Amt)
					+ "</td><td>"
					+ "已付款未报销金额："
					+ formatCurrency(currencyType,
							decisionFlag
									? 0 - contract.unCreditedTotalAmt + ''
									: 0 + '',
							Color_Font_Contract_Total_UnAccount_Amt)
					+ "</td></tr></table>";
		}
		// var ht = String.format(
		// temp, Ext.util.Format.number(
		// contract.earnedTotalAmt, currencyType + '0,0.00'),
		// Ext.util.Format.number(
		// contract.arrearageTotalAmt, currencyType + '0,0.00'),
		// Ext.util.Format.number(
		// contract.invoicedTotalAmt, currencyType + '0,0.00'),
		// Ext.util.Format.number(
		// 0 - contract.unCreditedTotalAmt, currencyType + '0,0.00'));

		var payPanel = new Ext.Panel({
					border : false,
					id : 'payPanel',
					width : 500,
					height : decisionFlag ? 50 : 65,
					padding : '5 5 5 5',
					html : '<SPAN id=totalAmtMsg>' + ht + '</SPAN>'
				});
		contractFormPanel.insert(0, payPanel);
		contractFormPanel.doLayout();
		// contractFormPanel.getTopToolbar().setDisabled(true);
		// 添加票据编辑
		var invoiceRecord = Ext.data.Record.create([{
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
		var invoiceStore = new Ext.data.Store({
					url : __cxtPath + '/contractMain.do?method=getInvoiceList',
					remoteSort : false,
					sortInfo : {
						field : DBSchema.InvoiceDetail.InvoiceDateDisplayed,
						direction : 'ASC'
					},
					reader : new Ext.data.JsonReader({
								root : 'rows',
								totalProperty : 'totalCount',
								fields : invoiceRecord
							})
				});

		if (!decisionFlag) {
			mainPanel.paymentButton.hide(true);
			mainPanel.addRelateds.hide(true);
			mainPanel.addreButton.hide(true);
			mainPanel.invoiceButton.hide(true);
		}

	} else {
		if (parentID && parentID != null) {
			Ext.getCmp('parentFieldset').expand(false);
			contractFormPanel.getForm().findField('parentID')
					.setValue(parentID);
			contractFormPanel.getForm().findField('parentNo')
					.setValue(parentNo);
			contractFormPanel.getForm().findField('parentName')
					.setValue(parentName);
			contractFormPanel.getForm().findField('parentSimpleName')
					.setValue(parentAbb);

		}
	}
		// readOnly
		// if (readOnly) {
		// mainPanel.getTopToolbar().hide(true);
		// }

});
var modifiable = ['contractSourceType', 'contractName', 'abb', 'summary',
		'warrantyDefinition', 'warrantyUnit', 'warrantyLimit',
		'warrantyStartDate', 'contractAmt', 'signatureDate', 'remark'];

function setFormReadOnly(form, readonly) // 该函数设置里面的控件的可读性。
{
	if (true)
		return;
	form.getForm().items.eachKey(function(key, item) {
		if (readonly && modifiable.indexOf(item.name || item.hiddenName) == -1) {
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

function limitCalculate(field, newValue, oldValue) {
	var start = Ext.getCmp('warrantyStartDate');
	var limit = Ext.getCmp('warrantyLimit');
	var unit = Ext.getCmp('warrantyUnitID');
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

function nameLinker(val) {
	var text = val.substring(0, val.indexOf("^"));
	var url = val.substring(val.indexOf("^") + 1, val.length);
	if (text != '0') {
		return '<a href="' + url + '">' + text + '</a>';
	} else
		return text;
}

function turnToEditContract(contractID, contractName) {
	// parent.xMis.turnToMenu('contract-edit',
	parent.xMis.turnToMenu('contract-' + contractID,
			'contractMain.do?method=contract&contractID=' + contractID, '【合同】'
					+ contractName);
}

var invoiceOutWin;
function showInvoice(contractID, contractNo) {
	if (!invoiceOutWin) {
		invoiceOutWin = new Ext.Window({
			title : String.format('票据收发登记 - {0}', contractNo),
			width : 800,
			height : 450,
			layout : 'border',
			closeAction : 'hide',
			items : {
				xtype : 'panel',
				region : 'center',
				autoScroll : true,
				autoLoad : {
					url : String
							.format(
									'{0}/contract/invoice.jsp?contractID={1}&contractType=2&companyID={2}',
									__cxtPath, contractID, contract.buyerID),
					scripts : true
				}
			}

		});
	} else {
		invoiceOutWin.setTitle(String.format('票据收发登记 - {0}', contractNo));
		contractIdInEdit_invoice = contractID;
		window.invoiceDetailEditGrid.getStore().load({
					params : {
						contractID : contractID,
						type : '1'
					}
				});
		window.downloadGridInInvoEd.getStore().load({
					// params : {
					// applicationType : attchmentType,
					// refID : contractID
					// }
					params : {
						attachmentTypeID : com.bitc.xmis.Attachment.invoice,
						REFID : contractID
					}
				});
		window.receiptDetailEditGrid.getStore().load({
					params : {
						contractID : contractID,
						type : '2'
					}
				});
		window.receiptdownloadGridInInvoEd.getStore().load({
					// params : {
					// applicationType : attchmentType2,
					// refID : contractID
					// }
					params : {
						attachmentTypeID : com.bitc.xmis.Attachment.receipt,
						REFID : contractID
					}
				});
	}
	invoiceOutWin.show();
}

var paymentWin;
function showPayment(contractID, contractNo, contractName, contractAmt) {
	if (!paymentWin) {
		var _url = __cxtPath
				+ '/contract/paymentEdit.jsp?'
				+ encodeURI('contractID=' + contractID
						+ '&contractType=2&contractName=' + contractName
						+ '&contractAmt=' + contractAmt);
		paymentWin = new Ext.Window({
					title : String.format('合同款收付登记 - {0}', contractNo),
					width : 800,
					height : 448,
					layout : 'border',
					closeAction : 'hide',
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
	} else {
		paymentWin.setTitle(String.format('合同款收付登记 - {0}', contractNo));
		contractIdInEdit = contractID;
		window.paymentDetailEditGrid.getStore().load({
					params : {
						contractID : contractID
					}
				});
		window.downloadGridInPayEd.getStore().load({
					params : {
						attachmentTypeID : com.bitc.xmis.Attachment.payment,
						REFID : contractID
					}
				});
	}
	paymentWin.show();
}
var purchdeWin;
function showPayment(contractID, contractNo, contractName, contractAmt) {
	// if (!paymentWin) {
	var _url = __cxtPath
			+ '/contract/paymentEdit.jsp?'
			+ encodeURI('contractID=' + contractID
					+ '&contractType=2&contractName=' + contractName
					+ '&contractAmt=' + contractAmt);
	purchdeWin = new Ext.Window({
				title : String.format('合同款收付登记 - {0}', contractNo),
				width : 800,
				height : 448,
				layout : 'border',
				closeAction : 'hide',
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
	// } else {
	// paymentWin.setTitle(String.format('合同款收付登记 - {0}', contractNo));
	// contractIdInEdit = contractID;
	// window.paymentDetailEditGrid.getStore().load({
	// params : {
	// contractID : contractID
	// }
	// });
	// window.downloadGridInPayEd.getStore().load({
	// params : {
	// attachmentTypeID : com.bitc.xmis.Attachment.payment,
	// REFID : contractID
	// }
	// });
	// }
	purchdeWin.show();
}
function showAddRelated(relatedStore, related) {
	related = related || {};
	var sum = 0.0;
	var records = relatedStore.getRange(1);
	for (var i = 0; i < records.length; i++) {
		if (records[i] != null) {
			sum = sum
					+ Number(records[i].get(DBSchema.ContractRelated.Amt)
							.toString());
		}
	}
	// relatedStore.each(
	// function(record) {
	// sum += record.get(DBSchema.ContractRelated.Amt);
	// });
	// var dig = Ext
	// .max([contract.contractAmt.toString().length
	// - contract.contractAmt.toString().indexOf('.') - 1, sum
	// .toString().length
	// - sum.toString().indexOf('.') - 1]);
	// 小数点后保留2位小数
	var max = (contract.contractAmt - sum).toFixed(2);
	var relatedForm = new Ext.FormPanel({
		border : false,
		region : 'center',
		baseCls : 'x-plain',
		margins : '5 0 0 5',
		labelWidth : 115,
		labelAlign : 'right',
		defaults : {
			msgTarget : 'title',
			width : 275
		},
		items : [{
					xtype : 'hidden',
					name : 'contract1' // 收入
				}, {
					xtype : 'hidden',
					name : 'contract2', // 支出
					value : contract.contractID
				}, {
					xtype : 'hidden',
					name : 'relatedDelimiter',
					value : delimiter
				}, {
					xtype : 'compositefield',
					fieldLabel : '支出合同项目编号',
					name : 'contractNo',
					border : false,
					items : [new Ext.ux.form.ChooseField({
						fieldLabel : '甲方合同编号',
						width : 180,
						name : 'contractNoCode',
						allowBlank : false,
						winConfig : {
							width : 765,
							title : '选择对应合同编号'
						},
						baseParamFun : function() {
							return {
								searchName : '',
								contractTypeID : 1,
								limit : 20
							};
						},
						paramFun : function() {
							return {
								start : 0
							};
						},
						chooseUrl : __cxtPath
								+ '/resources/choose/contract.json.jsp',
						setValueFun : function(o) {
							if (Ext.isEmpty(o)) {
								Ext.Msg.alert('提示', '您还没有选择，请选择');
							}
							relatedForm.getForm().findField('contract1')
									.setValue(o
											.get(DBSchema.Contract.ContractID));
							var abb = o.get(DBSchema.Project.ProjectCode);
							if (abb.indexOf('^') > 0) {
								abb = abb.substring(0, abb.indexOf('^'));
							}
							relatedForm.getForm().findField('contractNoCode')
									.setValue(abb);
							relatedForm.getForm().findField('contractNoNumber')
									.loadValue(abb);
							relatedForm.getForm().findField('contract2Name')
									.setValue(abb);
						},
						clearValueFun : function() {
							relatedForm.getForm().findField('contractNoCode')
									.setValue('');
							relatedForm.getForm().findField('contract1')
									.setValue('');
							relatedForm.getForm().findField('contract2Name')
									.setValue('');
							relatedForm.getForm().findField('contractNoNumber')
									.setValue('');
						}
					}),
							// {
							// xtype : 'displayfield',
							// value : delimiter
							// }
							{

								xtype : 'combo',
								width : 45,
								hiddenName : 'relatedDelimiterCombo',
								id : 'relatedDelimiterCombo',
								displayField : 'delimiter',
								valueField : 'delimiter',
								mode : 'romote',
								triggerAction : 'all',
								editable : false,
								value : '-ZC',
								store : new Ext.data.JsonStore({
									url : __cxtPath
											+ '/contractMain.do?method=getOutContractDelimiterStore',
									autoLoad : true,
									fields : [{
												name : 'delimiter'
											}]
								}),
								listeners : {
									'select' : function(combo, record, index) {
										relatedForm.getForm()
												.findField('relatedDelimiter')
												.setValue(combo.getValue());
									}
								}

							}, {
								xtype : 'textfield',
								width : 40,
								// readOnly : true,
								name : 'contractNoNumber',
								loadValue : function(code) {
									Ext.Ajax.request({
										url : __cxtPath
												+ '/contractMain.do?method=getContractCodeNumber',
										params : {
											contractCode : code
										},
										scope : this,
										success : function(response) {
											var result = Ext
													.decode(response.responseText);
											if (result.success) {
												this
														.setValue(result.attributes.number);
											}
										},
										failure : function() {
										}
									});
								}
							}]
				}, {
					xtype : 'displayfield',
					fieldLabel : '收入合同编号',
					name : 'contract2Name'
				}, {
					xtype : 'panel',
					width : 400,
					labelWidth : 115,
					labelAlign : 'right',
					baseCls : 'x-plain',
					layout : 'form',
					border : false,
					items : [{
								xtype : 'numberfield',
								fieldLabel : '对应金额',
								width : 275,
								allowBlank : false,
								name : 'relatedAmt',
								value : max
							}]
				}, {
					xtype : 'textfield',
					fieldLabel : '备注',
					name : 'remark'
				}],
		buttonAlign : 'center',
		buttons : [{
			text : '确定',
			handler : function() {
				relatedForm.getForm().submit({
					url : __cxtPath + '/contractMain.do',
					params : {
						method : 'saveRelatedContract'
					},
					waitMsg : '正在保存，请稍候……',
					waitTitle : '正在处理',
					success : function(form, action) {
						Ext.Msg.alert(action.result.messageTitle,
								action.result.message, function() {
									relatedWin.close();
									relatedStore.load({
												params : {
													contractID : contract.contractID,
													contractType : 2
												}
											});
								});
					},
					failure : function(form, action) {
						com.bitc.xmis.util.AlertUtil
								.formSubmitFailedAlert(action);
					}
				});
			}
		}, {
			text : '取消',
			handler : function() {
				relatedWin.close();
				if (max > 0) {
					Ext.Msg.show({
								title : '提示',
								msg : Ext.getCmp('relatedAmtTipId').getValue(),
								buttons : Ext.Msg.OK
							});
				}
			}
		}]
	});
	var relatedWin = new Ext.Window({
				modal : true,
				title : '添加对应合同',
				width : 450,
				height : 180,
				layout : 'border',
				closeAction : 'close',
				items : [relatedForm]
			});
	relatedForm.getForm().setValues(related);
	relatedForm.getForm().findField('relatedAmt').setMaxValue(max);
	relatedWin.show();
}

function showupdateRelated(outContractID, relatedID) {
	var relatedRecord = Ext.data.Record.create([{
				name : 'inContractID',
				type : 'string'
			}, {
				name : 'pContractID',
				type : 'string'
			}, {
				name : 'relatedID',
				type : 'string'
			}, {
				name : 'relateAmt',
				type : 'number'
			}, {
				name : 'remark',
				type : 'string'
			}, {
				name : 'inContractNo',
				type : 'string'
			}, {
				name : 'contractNo',
				type : 'string'
			}, {
				name : 'samt',
				type : 'number'
			}, {
				name : 'contractNoCode',
				type : 'string'
			}, {
				name : 'contractNoNumber',
				type : 'string'
			}]);

	var relatedForm = new Ext.FormPanel({
				border : false,
				region : 'center',
				baseCls : 'x-plain',
				margins : '5 0 0 5',
				labelWidth : 115,
				reader : new Ext.data.JsonReader({}, relatedRecord),
				labelAlign : 'right',
				defaults : {
					msgTarget : 'title',
					width : 250
				},
				items : [{
							xtype : 'hidden',
							name : 'samt'// 金额最大值
						}, {
							xtype : 'hidden',
							name : 'inContractID' // 收入
						}, {
							xtype : 'hidden',
							name : 'pContractID', // 支出
							value : contract.contractID
						}, {
							xtype : 'textfield',
							width : 370,
							fieldLabel : '支出合同项目编码',
							name : 'contractNo',
							id : 'contractNoId'
						}, {
							xtype : 'displayfield',
							fieldLabel : '收入合同项目编号',
							name : 'inContractNo'
						}, {
							xtype : 'panel',
							width : 370,
							labelWidth : 115,
							labelAlign : 'right',
							baseCls : 'x-plain',
							layout : 'form',
							border : false,
							items : [{
										xtype : 'numberfield',
										id : 'relateAmtId',
										fieldLabel : '对应金额',
										width : 250,
										allowBlank : false,
										name : 'relateAmt'
									}]
						}, {
							xtype : 'textfield',
							fieldLabel : '备注',
							name : 'remark'
						}],
				buttonAlign : 'center',
				buttons : [{
					text : '确定',
					handler : function() {
						relatedForm.getForm().submit({
							url : __cxtPath + '/contractMain.do',
							params : {
								method : 'updateRelatedContract',
								relatedID : relatedID,
								outContractID : outContractID
							},
							waitMsg : '正在保存，请稍候……',
							waitTitle : '正在处理',
							success : function(form, action) {
								Ext.Msg.alert(action.result.messageTitle,
										action.result.message, function() {
											relatedWin.close();
											Ext.getCmp('relatedGrid')
													.getStore().reload();
										});
							},
							failure : function(form, action) {
							}
						});
					}
				}, {
					text : '取消',
					handler : function() {
						relatedWin.close();
					}
				}]
			});

	var relatedWin = new Ext.Window({
				modal : true,
				title : '修改对应合同',
				width : 450,
				height : 180,
				layout : 'border',
				closeAction : 'close',
				items : [relatedForm]
			});
	relatedWin.show();

	relatedForm.getForm().load({
		url : __cxtPath + '/contractMain.do',
		params : {
			method : 'getContractRelationInfo',
			relateID : relatedID,
			pContractID : outContractID
		},
		success : function(form, action) {
			relatedForm.getForm().findField('relateAmt')
					.setMaxValue(relatedForm.getForm().findField('samt')
							.getValue());
			com.bitc.xmis.util.ExtUtil.disableField(Ext.getCmp('contractNoId'));
			if (!hasContractControlOnCompanyLevel) {
				com.bitc.xmis.util.ExtUtil.disableField(Ext
						.getCmp('relateAmtId'));
			}
		},
		failure : function(form, action) {
		}
	});

}

function AddProcurementDetails(materialsStore, related) {

	var sum = 0;
	var maxx = 0;
	var comboxStroe = new Ext.data.JsonStore({
				url : __cxtPath + '/projectMain.do?method=getRelationProjectID',// 打开窗口查询项目
				root : 'rows',
				fields : [{
							name : 'contractCode',
							mapping : 'contractCode'
						}, {
							name : 'contractID',
							mapping : 'contractID'
						}, {
							name : 'contractSum',
							mapping : 'contractSum'
						}],

				baseParams : {
					contractId : contract == null ? 0 : contract.contractID
				},
				autoLoad : true
			});
	comboxStroe.on('load', function(store, record, opts) {
				var combo = procurementForm.getForm().findField('ddd');// Ext.getCmp("contractCodeee");
				var valueField = combo.store.data.items[0].data.contractID;
				// displayField = record[0].data.project;//这种方法也可以获得第一项的值
				var display = combo.store.data.items[0].data.contractCode;

				if (record.length == 1) {
					rocuremenAmt_param = combo.store.data.items[0].data.contractSum;
					combo.setValue(valueField);// 选中
				}

				var getcontractSum = new Ext.data.JsonStore({
							url : __cxtPath
									+ '/device.do?method=getcontractSum',// 查询总金额
							root : 'rows',
							fields : [{
										name : 'bnumber',
										mapping : 'bnumber'
									}, {
										name : 'bunivalent',
										mapping : 'bunivalent'
									}],
							baseParams : {
								valueField : procurementForm.getForm()
										.findField('ddd').getValue()
							},
							autoLoad : true,
							listeners : {
								'load' : function(store, record) {
									for (var i = 0; i < store.data.length; i++) {
										var vnumber = record[i].data.bnumber;
										var bunivalent = record[i].data.bunivalent;
										sum += vnumber * bunivalent;
									}
								}
							}

						});

			});

	var procurementForm = new Ext.FormPanel({
		border : false,
		region : 'center',
		baseCls : 'x-plain',
		margins : '50 0 0 5',
		labelAlign : 'right',
		defaults : {
			anchor : '90%'
		},
		items : [{
			xtype : 'panel',
			labelAlign : 'right',
			baseCls : 'x-plain',// 背景渲染
			margins : '5 0 0 5',
			layout : 'form',
			border : false,
			items : [{
				xtype : 'panel',
				labelAlign : 'right',
				baseCls : 'x-plain',// 背景渲染
				margins : '5 0 0 5',
				layout : 'column',
				border : false,
				items : [{
					columnWidth : .5,
					layout : "form",
					margins : '5 0 0 5',
					baseCls : 'x-plain',
					items : [{
						xtype : 'combo',
						name : 'ddd',
						store : comboxStroe,
						width : 220,
						fieldLabel : '支出合同编号',
						displayField : 'contractCode',
						valueField : 'contractID',
						selectOnFocus : true,
						editable : false,
						allowBlank : false,
						triggerAction : 'all',
						emptyText : '请选择合同',
						lazyInit : false,
						listeners : {
							'select' : function() {
								var getcontractSum = new Ext.data.JsonStore({
									url : __cxtPath
											+ '/device.do?method=getcontractSum',
									root : 'rows',
									fields : [{
												name : 'bnumber',
												mapping : 'bnumber'
											}, {
												name : 'bunivalent',
												mapping : 'bunivalent'
											}],

									baseParams : {
										valueField : procurementForm.getForm()
												.findField('ddd').getValue()
									},
									autoLoad : true,
									listeners : {
										'load' : function(store, record) {
											for (var i = 0; i < store.data.length; i++) {
												var vnumber = record[i].data.bnumber;
												var bunivalent = record[i].data.bunivalent;
												sum += vnumber * bunivalent;

											}
										}

									}

								});

							}
						}
					}, {
						xtype : 'hidden'
					}, new Ext.ux.form.ChooseField({
						fieldLabel : '设备名称',
						allowBlank : false,
						name : 'materiaName1',
						width : 220,
						winConfig : {
							title : '物资信息'
						},
						baseParamFun : function() {
							winFlag = "bWin";
							return {
								start : 0,
								limit : 20
							};
						},
						allowBlank : false,// 弹出选择框。
						chooseUrl : __cxtPath
								+ '/resources/choose/deviceproject.json.jsp',
						setValueFun : function(o) {
							if (Ext.isEmpty(o)) {
								Ext.Msg.alert('提示', '您还没有选择，请选择');
							}
							if (!Ext.isEmpty(o.data.停产时间)
									&& o.data.停产时间 <= new Date()
											.format('Y-m-d')) {
								Ext.Msg.alert('提示', '此物资已停产');
							}
							// mxID=o.data.id;
							procurementForm.getForm().findField('materiaName1')
									.setValue(o.data.物资名称);
							procurementForm.getForm().findField('materialName')
									.setValue(o.data.物资名称);
							procurementForm.getForm().findField('materialID')
									.setValue(o.data.物资ID);
							procurementForm.getForm().findField('modelName')
									.setValue(o.data.型号);
							procurementForm.getForm().findField('standardName')
									.setValue(o.data.规格);
							procurementForm.getForm().findField('weightName')
									.setValue(o.data.重量);
							procurementForm.getForm()
									.findField('officialMoney')
									.setValue(o.data.官方报价);
							procurementForm.getForm()
									.findField('manufacturerName')
									.setValue(o.data.生产厂家名称);
							procurementForm.getForm().findField('lastName')
									.setValue(o.data.货币名称);
							if (!Ext.isEmpty(o.data.投入市场日期)) {
								procurementForm.getForm().findField('TFSCSJ')
										.setValue((o.data.投入市场日期).substring(0,
												10));
							} else {
								procurementForm.getForm().findField('TFSCSJ')
										.setValue(o.data.投入市场日期);
							}
							if (!Ext.isEmpty(o.data.停产时间)) {
								procurementForm.getForm().findField('TCSJ')
										.setValue((o.data.停产时间)
												.substring(0, 10));
							} else {
								procurementForm.getForm().findField('TCSJ')
										.setValue(o.data.停产时间);
							}
							procurementForm.getForm()
									.findField('yieldingFettle')
									.setValue(o.data.物资生产状态名称);
							// if(o.data.stopTime >= o.data.newupdateDate &&
							// o.data.stopTime > o.data.listingDate){
							// procurementForm.getForm().findField('yieldingFettle')
							// .setValue('在产');
							// }
							// if(Ext.isEmpty(o.data.stopTime) &&
							// o.data.listingDate<o.data.newupdateDate){
							// procurementForm.getForm().findField('yieldingFettle')
							// .setValue('在产');
							// }
							// if(o.data.stopTime <= o.data.newupdateDate &&
							// !Ext.isEmpty(o.data.listingDate)){
							// procurementForm.getForm().findField('yieldingFettle')
							// .setValue('停产');
							// }
							// if(Ext.isEmpty(o.data.listingDate) &&
							// o.data.stopTime <= o.data.newupdateDate){
							// procurementForm.getForm().findField('yieldingFettle')
							// .setValue('停产');
							// }
							// if(Ext.isEmpty(o.data.listingDate) &&
							// o.data.stopTime > o.data.newupdateDate){
							// procurementForm.getForm().findField('yieldingFettle')
							// .setValue('在产');
							// }
							// if(o.data.listingDate>o.data.newupdateDate &&
							// o.data.stopTime > o.data.newupdateDate){
							// procurementForm.getForm().findField('yieldingFettle')
							// .setValue('');
							// }

						},
						listeners : {
							render : function() {// 渲染后添加click事件，双击物资名称跳转到物资信息界面
								Ext.fly(this.el).on('dblclick', function(e, t) {
									parent.xMis
											.turn(
													'company-info-'
															+ procurementForm
																	.getForm()
																	.findField('materialID')
																	.getValue(),
													"[物资]"
															+ procurementForm
																	.getForm()
																	.findField('materialName')
																	.getValue(),
													__cxtPath
															+ '/device.do?method=getMaterialnfo&id='
															+ procurementForm
																	.getForm()
																	.findField('materialID')
																	.getValue(),
													'menu-company-info', true);
								});
							}
						}
					}), {
						xtype : 'panel',
						layout : 'column',
						baseCls : 'x-plain',// 背景渲染
						border : false,
						items : [{
							xtype : 'panel',
							layout : 'form',
							columnWidth : 0.7,
							baseCls : 'x-plain',// 背景渲染
							labelAlign : 'right',
							// allowBlank : false,
							border : false,
							items : [{
								xtype : 'formatnum',
								allowBlank : false,
								fieldLabel : '采购单价',
								name : 'purchaseLimit',
								anchor : '99%',
								listeners : {
									change : function(field, newValue,
											oldValue, eOpts) {
										procurementForm
												.getForm()
												.findField('sumMoneyName')
												.setValue(procurementForm
														.getForm()
														.findField('Purchasequantity')
														.getValue()
														* procurementForm
																.getForm()
																.findField('purchaseLimit')
																.getValue());
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
						xtype : "numberfield",
						fieldLabel : "采购数量",
						name : 'Purchasequantity',
						style : 'text-align:right',
						allowBlank : false,
						labelAlign : 'right',
						width : 220,
						listeners : {
							change : function(field, newValue, oldValue, eOpts) {
								procurementForm
										.getForm()
										.findField('sumMoneyName')
										.setValue(procurementForm.getForm()
												.findField('Purchasequantity')
												.getValue()
												* procurementForm
														.getForm()
														.findField('purchaseLimit')
														.getValue());
							}
						}
					}, {
						xtype : "formatnum",
						fieldLabel : "总金额",
						name : 'sumMoneyName',
						editable : false,
						readOnly : true,
						allowBlank : false,
						width : 220
					}, {
						xtype : 'datefield',
						fieldLabel : '期望到货日期',
						name : 'expectedarrivalDate',
						format : 'Y-m-d',
						width : 220,
						getAsycnValue : function(callback) {
							callback.call(this, Ext.util.Format.date(this
													.getValue(), this.format));
						}
					}, {
						xtype : 'textarea',
						fieldLabel : '技术要求',
						name : 'technicalClaim',
						width : 230,
						height : 60,
						listeners : {
							'render' : function(c) {
								Ext.QuickTips.register({
											target : c.getEl(),
											text : '"技术要求"最多可输入500字'
										});
							}
						}
					}, {
						xtype : 'textarea',
						fieldLabel : '备注',
						name : 'remarkName',
						width : 230,
						height : 60,
						listeners : {
							'render' : function(c) {
								Ext.QuickTips.register({
											target : c.getEl(),
											text : '"备注"最多可输入500字'
										});
							}
						}
					}]
				}, {
					xtype : 'fieldset',
					title : '物资信息',
					collapsible : true,// 收缩
					baseCls : 'x-plain',
					columnWidth : .5,
					items : [{
								xtype : "displayfield",
								fieldLabel : "物资名称",
								name : 'materialName'
							}, {
								xtype : "displayfield",
								fieldLabel : "物资ID",
								name : 'materialID',
								hidden : true
							}, {

								xtype : "displayfield",
								fieldLabel : "型号",
								name : 'modelName'
							}, {
								xtype : "displayfield",
								fieldLabel : "规格",
								name : 'standardName'
							}, {
								xtype : "displayfield",
								fieldLabel : "重量",
								name : 'weightName'
							}, {
								xtype : 'panel',
								layout : 'column',
								baseCls : 'x-plain',// 背景渲染
								border : false,
								items : [{
											xtype : 'panel',
											layout : 'form',
											columnWidth : 0.63,
											baseCls : 'x-plain',// 背景渲染
											labelAlign : 'right',
											// allowBlank : false,
											border : false,
											items : [{
														xtype : 'displayfield',
														allowBlank : false,
														fieldLabel : '采购单价',
														name : 'officialMoney',
														anchor : '99%'
													}]
										}, {
											xtype : 'displayfield',
											width : 45,
											name : 'lastName',
											align : 'right'
										}]
							}, {
								xtype : "displayfield",
								fieldLabel : "生产厂家",
								name : 'manufacturerName'
							}, {
								xtype : 'displayfield',
								fieldLabel : '投入市场日期',
								name : 'TFSCSJ'
							}, {
								xtype : 'displayfield',
								fieldLabel : '停产时间',
								name : 'TCSJ'
							}, {
								xtype : "displayfield",
								fieldLabel : "物资生产状态",
								name : 'yieldingFettle'
							}]
				}]
			}, {
				xtype : 'panel',
				layout : 'column',
				baseCls : 'x-plain',
				border : false,
				items : [{
							xtype : 'panel',
							layout : 'form',
							baseCls : 'x-plain',
							columnWidth : 0.25,
							labelAlign : 'right',
							labelWidth : 90,
							border : false,
							items : [{
										fieldLabel : '登记人',
										name : 'registerName4',
										xtype : 'displayfield',
										value : empName
									}, {
										fieldLabel : '最后修改人',
										name : 'registerName5',
										xtype : 'displayfield'
									}]
						}, {
							xtype : 'panel',
							layout : 'form',
							columnWidth : 0.25,
							baseCls : 'x-plain',
							labelAlign : 'right',
							labelWidth : 90,
							border : false,
							items : [{
										fieldLabel : '登记日期',
										xtype : 'displayfield',
										name : 'updateDate3',
										value : new Date().format('Y-m-d')
									}, {
										fieldLabel : '最后修改日期',
										xtype : 'displayfield',
										name : 'updateDate6'
									}]
						}]
			}]
		}]

	});

	var promainPanel = new Ext.Panel({
		region : 'center',
		border : false,
		layout : 'border',
		items : [procurementForm],
		tbar : {
			xtype : 'toolbar',
			disabled : false,
			items : [{
				ref : '../saveButton',
				xtype : 'button',
				text : '确定',
				iconCls : 'ok',
				handler : function() {
					var checkFirstName = procurementForm.getForm()
							.findField('purchaseLimit').getValue();
					if ("0" == checkFirstName) {
						Ext.Msg.alert("错误", "采购单价不能为0");
						return false;
					}

					if (procurementForm.getForm().findField('sumMoneyName')
							.getValue() <= rocuremenAmt_param - sum) {
						procurementForm.getForm().submit({
							url : __cxtPath
									+ '/device.do?method=getPurchasingDetaildouble',
							params : {
								contractId : contract.contractID,
								contractID : procurementForm.getForm()
										.findField('ddd').getValue(),// 项目ID
								materialID : procurementForm.getForm()
										.findField('materialID').getValue(), // 物资ID
								updateDate3 : procurementForm.getForm()
										.findField('updateDate3').getValue(),// 登记日期
								userID : __emp.userID, // 登记人
								registerName4 : procurementForm.getForm()
										.findField('registerName4').getValue(),
								lastModifier : __emp.userID,
								registerName5 : procurementForm.getForm()
										.findField('registerName5').getValue(),
								updateDate6 : new Date().format('Y-m-d')
							},
							waitMsg : '正在保存，请稍候……',
							waitTitle : '正在处理',
							success : function(form, action) {
								if (Ext.decode(action.response.responseText).totalCount == 0) {
									procurementForm.getForm().submit({
										url : __cxtPath
												+ '/device.do?method=addMaterialsPurchasingDetail',// 查询是否为同一个项目重复购买物资
										params : {
											contractId : contract.contractID,
											contractID : procurementForm
													.getForm().findField('ddd')
													.getValue(),// 项目ID
											materialID : procurementForm
													.getForm()
													.findField('materialID')
													.getValue(), // 物资ID
											updateDate3 : procurementForm
													.getForm()
													.findField('updateDate3')
													.getValue(),// 登记日期
											userID : __emp.userID,// 登记人
											registerName4 : procurementForm
													.getForm()
													.findField('registerName4')
													.getValue(),
											lastModifier : __emp.userID,
											registerName5 : procurementForm
													.getForm()
													.findField('registerName5')
													.getValue(),
											updateDate6 : new Date()
													.format('Y-m-d')

										},
										success : function(form, action) {
											Ext.Msg.alert(
													action.result.messageTitle,
													action.result.message,
													function() {
														procurementWin.close();

														// materialsStore.reload();
														Ext.getCmp('gridPanel')
																.getStore()
																.reload();
													});
										},
										failure : function(form, action) {
										},
										method : "post"
									});
								} else {
									Ext.MessageBox.confirm('提示',
											'已为此项目购买此设备，是否确定提交？', function(btn,
													text) {
												if (btn == 'yes') {
													procurementForm.getForm()
															.submit({
																url : __cxtPath
																		+ '/device.do?method=addMaterialsPurchasingDetail',// 添加采购清单明细
																params : {
																	contractId : contract.contractID,
																	contractID : procurementForm
																			.getForm()
																			.findField('ddd')
																			.getValue(),// 项目ID
																	materialID : procurementForm
																			.getForm()
																			.findField('materialID')
																			.getValue(), // 物资ID
																	updateDate3 : procurementForm
																			.getForm()
																			.findField('updateDate3')
																			.getValue(),// 登记日期
																	userID : __emp.userID,// 登记人
																	registerName4 : procurementForm
																			.getForm()
																			.findField('registerName4')
																			.getValue(),
																	lastModifier : __emp.userID,
																	registerName5 : procurementForm
																			.getForm()
																			.findField('registerName5')
																			.getValue(),
																	updateDate6 : new Date()
																			.format('Y-m-d')
																},
																success : function(
																		form,
																		action) {
																	Ext.Msg
																			.alert(
																					action.result.messageTitle,
																					action.result.message,
																					function() {
																						procurementWin
																								.close();
																						// materialsStore
																						// .reload();
																						Ext
																								.getCmp('gridPanel')
																								.getStore()
																								.reload();
																					});
																},
																failure : function(
																		form,
																		action) {

																},
																method : "post"
															});
												}
											});
								}
							},
							failure : function(form, action) {
							}
						});
					} else {
						Ext.Msg.alert('提示', '总采购金额已超出项目对应金额');
					}
				}
			}, {
				hindf : '../saveButton',
				xtype : 'button',
				text : '取消',
				iconCls : 'remove',
				handler : function() {
					procurementWin.close();
				}
			}]

		}
	});

	var procurementWin = new Ext.Window({
				modal : true,
				title : '添加采购明细',
				width : 800,
				height : 470,
				layout : 'border',
				closeAction : 'close',
				items : [promainPanel]
			});

	procurementWin.show();
	procurementForm.getForm().findField('selfHardCur').setValue(Ext
			.getCmp('currencyTypeID').lastSelectionText);
	procurementForm.getForm().findField('ddd').on('select',
			function(combo, record, index) {
				rocuremenAmt_param = record.data.contractSum;
			});
}
