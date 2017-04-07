Ext.onReady(function() {

	var tabDisable = false; // tab标签是否不可见的属性
	var currenttab = 0; // 当前Tab
	var selectCompanyID = null; // 公司ID
	var selectCompanyName = null;// 公司名称
	var selectLevel = "";// 地址选定的级别 省1 市2、

	var internationFid = ""; // 所选洲际的ID
	var countryFid = ""; // 所选的国家的ID
	var areaFid = ""; // 所选的地区的ID
	var provinceFid = ""; // 所选的省份的ID
	var wstate = 0;// 联系人新建、节点双击的状态标识
	var lstate = 0;// 法人代表新建、节点双击的状态标识
	var owner = 0;// 经理新建、节点双击的状态表示
	var qualifiedperson = 0;// 资质联系人
	var applicantTT = 0;// 申请人
	var patentHolder1 = 0;// 设计人1
	var patentHolder2 = 0;// 设计人2
	var patentHolder3 = 0;// 设计人3
	var patentHolder4 = 0;// 设计人4
	var patentHolder5 = 0;// 设计人5

	var patentRight1 = 0;// 专利权人1
	var patentRight2 = 0;// 专利权人2
	var patentRight3 = 0;// 专利权人3
	var patentRight4 = 0;// 专利权人4
	var patentRight5 = 0;// 专利权人5
	var cflag = 0;// 判断是否初次点击组织机构tab的标识
	var ddddd = "";

	Ext.QuickTips.init();
	Ext.form.Field.prototype.msgTarget = 'under';

	if (company != null) {
		selectCompanyID = company.companyID;
		selectCompanyName = company.companyName
	}
	var brCombo = new com.bitc.xmis.Combos.BusinessRelationCombo({
				fieldLabel : '<font color="#CC0000">*</font>业务关系',
				width : 150,
				hiddenName : 'businessRelationID'
			});

	var relativeAddressFormPanel = new Ext.form.FormPanel({
		title : '',
		bodyStyle : 'padding:10px;',
		frame : true,
		reader : new Ext.data.JsonReader({
					root : 'data'
				}, Ext.data.Record.create([{
					name : 'chooseAddress',
					mapping : 'address',
					convert : function(v) {
						if (v != null) {
							var isTrue = (v.city.province.provinceName.trim()) == (v.city.cityName
									.trim());
							var showname = '';
							if (isTrue) {
								showname = v.city.province.area.country.internation.internationName
										.trim()
										+ '·'
										+ v.city.province.area.country.countryName
												.trim()
										+ '·'
										+ v.city.province.area.areaName.trim()
										+ '·'
										+ v.city.province.provinceName.trim();
							} else {
								showname = v.city.province.area.country.internation.internationName
										.trim()
										+ '·'
										+ v.city.province.area.country.countryName
												.trim()
										+ '·'
										+ v.city.province.area.areaName.trim()
										+ '·'
										+ v.city.province.provinceName.trim()
										+ '·' + v.city.cityName;
							}
							return showname;
						} else {
							return '';
						}
					}
				}, {
					name : 'district',
					mapping : 'address',
					convert : function(v) {
						return v != null ? (v.district) : '';
					}
				}, {
					name : 'addressID',
					mapping : 'address',
					convert : function(v) {
						return v != null ? (v.addressID) : '';
					}
				}, {
					name : 'zipCode',
					mapping : 'address',
					convert : function(v) {
						return v != null
								? (v.zipCode == 0 ? '' : v.zipCode)
								: '';
					}
				}, {
					name : 'addressInfo',
					mapping : 'address',
					convert : function(v) {
						return v != null ? (v.addressInfo) : '';
					}
				}, {
					name : 'internationID',
					mapping : 'address',
					convert : function(v) {
						return v != null
								? (v.city.province.area.country.internation.internationID)
								: '';
					}
				}, {
					name : 'countryID',
					mapping : 'address',
					convert : function(v) {
						return v != null
								? (v.city.province.area.country.countryID)
								: '';
					}
				}, {
					name : 'areaID',
					mapping : 'address',
					convert : function(v) {
						return v != null ? (v.city.province.area.areaID) : '';
					}
				}, {
					name : 'provinceID',
					mapping : 'address',
					convert : function(v) {
						return v != null ? (v.city.province.provinceID) : '';
					}
				}, {
					name : 'cityID',
					mapping : 'address',
					convert : function(v) {
						return v != null ? (v.city.cityID) : '';
					}
				}, {
					name : 'id',
					mapping : 'id'
				}])),
		autoScroll : true,
		items : [new Ext.ux.form.AddressChooseField({
					fieldLabel : '<font color="#CC0000">*</font>城市选择',
					name : 'chooseAddress',
					anchor : '100%',
					editable : false,
					allowBlank : false,
					setValueFun : function(name, ids) {
						relativeAddressFormPanel.getForm()
								.findField('chooseAddress').setValue(name);

						relativeAddressFormPanel.getForm()
								.findField('internationID')
								.setValue(ids.internationID);

						relativeAddressFormPanel.getForm()
								.findField('countryID').setValue(ids.countryID);

						relativeAddressFormPanel.getForm().findField('areaID')
								.setValue(ids.areaID);

						relativeAddressFormPanel.getForm()
								.findField('provinceID')
								.setValue(ids.provinceID);

						relativeAddressFormPanel.getForm().findField('cityID')
								.setValue(ids.cityID);

					},
					getValueFun : function() {
						var a, b, c, d, e;
						a = relativeAddressFormPanel.getForm()
								.findField('internationID').getValue();

						b = relativeAddressFormPanel.getForm()
								.findField('countryID').getValue();

						c = relativeAddressFormPanel.getForm()
								.findField('areaID').getValue();

						d = relativeAddressFormPanel.getForm()
								.findField('provinceID').getValue();

						e = relativeAddressFormPanel.getForm()
								.findField('cityID').getValue();
						return [a, b, c, d, e];
					},
					clearValueFun : function() {
						relativeAddressFormPanel.getForm()
								.findField('chooseAddress').setValue('');

						relativeAddressFormPanel.getForm()
								.findField('internationID').setValue('');

						relativeAddressFormPanel.getForm()
								.findField('countryID').setValue('');

						relativeAddressFormPanel.getForm().findField('areaID')
								.setValue('');

						relativeAddressFormPanel.getForm()
								.findField('provinceID').setValue('');

						relativeAddressFormPanel.getForm().findField('cityID')
								.setValue('');
					}
				}), {
			xtype : 'textfield',
			fieldLabel : '&nbsp;&nbsp;区县',
			name : 'district',
			allowBlank : true,
			anchor : '100%'
		}, {
			xtype : 'textfield',
			fieldLabel : '&nbsp;&nbsp;邮编',
			maskRe : new RegExp('[' + Ext.escapeRe('0123456789') + ']'),
			regex : /^[1-9]\d{5}$/,
			name : 'zipCode',
			allowBlank : true,
			anchor : '100%'
		}, {
			xtype : 'textarea',
			fieldLabel : '<font color="#CC0000">*</font>详细地址',
			name : 'addressInfo',
			// allowBlank : false,
			anchor : '100%'
		}, {
			xtype : 'hidden',
			id : 'contactPersonID',
			name : 'contactPersonID'
		}, {
			xtype : 'hidden',
			fieldLabel : '洲际ID',
			name : 'internationID',
			allowBlank : true
		}, {
			xtype : 'hidden',
			fieldLabel : '国家ID',
			name : 'countryID',
			allowBlank : true
		}, {
			xtype : 'hidden',
			fieldLabel : '地区ID',
			name : 'areaID',
			allowBlank : true
		}, {
			xtype : 'hidden',
			fieldLabel : '省ID',
			name : 'provinceID',
			allowBlank : true
		}, {
			xtype : 'hidden',
			fieldLabel : '市ID',
			name : 'cityID',
			allowBlank : true
		}, {
			xtype : 'hidden',
			fieldLabel : '地址ID',
			name : 'addressID',
			allowBlank : true
		}, {
			xtype : 'hidden',
			id : 'id',
			name : 'id'
		}, {
			xtype : 'hidden',
			id : 'chooseAddressID',
			name : 'chooseAddressID'
		}]
	});

	// 窗口
	var relativeAddressWin = new Ext.Window({
				title : '相关地址信息',
				closable : true,
				closeAction : 'hide',
				width : 500,
				height : 260,
				plain : true,
				modal : true,
				layout : 'fit',
				tbar : {
					xtype : 'toolbar',
					items : [{
						xtype : 'button',
						text : '保存',
						iconCls : 'ok',
						handler : function() {
							Ext.MessageBox.show({
										title : '请稍候',
										msg : '正在处理...',
										progressText : '操作中...',
										width : 300,
										wait : true,
										waitConfig : {
											interval : 200
										},
										closable : false
									});

							relativeAddressFormPanel.getForm().submit({
								url : __cxtPath
										+ '/crmMaint.do?method=doSaveRelativeAddress',
								params : {
									companyId : selectCompanyID,
									addressID : relativeAddressFormPanel
											.getForm().findField('addressID')
											.getValue(),
									cityID : relativeAddressFormPanel.getForm()
											.findField('cityID').getValue(),
									id : relativeAddressFormPanel.getForm()
											.findField('id').getValue()
								},
								success : function(response, action) {
									Ext.MessageBox.hide();
									Ext.MessageBox.alert(
											action.result.messageTitle,
											action.result.message);
									relativeAddressWin.hide();
									relativeAdressStore.reload();
									// storeGYS.load({
									// params : {
									// start : 0,
									// selectCompanyID : selectCompanyID,
									// limit : 20
									// }
									// });
									Ext.getCmp('officeAddressNameCombo')
											.getStore().reload();

								},
								failure : function(response, action) {
									com.bitc.xmis.util.AlertUtil
											.formSubmitFailedAlert(action)
								}
							});
						}
					}, '-', {
						xtype : 'button',
						ref : '../cancelBtn',
						text : '取消',
						iconCls : 'remove',
						handler : function() {
							relativeAddressWin.hide();
						}
					}]
				},
				items : [relativeAddressFormPanel]
			});
	var labels = new Ext.form.Label({
				width : 4,
				hideMode : "visibility",
				html : "<font color='black' size='2px'><b>-</b></font>"
			});

	var compFormPanel = new Ext.form.FormPanel({
		region : 'center',
		padding : 20,
		width : 600,
		autoScroll : true,
		border : false,
		labelWidth : 101,
		labelAlign : 'right',
		items : [{
					xtype : 'hidden',
					name : 'companyID',
					fieldLabel : 'ID',
					style : 'color:blue'
				}, {
					xtype : 'textfield',
					fieldLabel : '<font color="#CC0000">*</font>公司名称',
					name : 'companyName',
					allowBlank : false,
					width : 300,
					blankText : '该输入项为必输项'
				}, {
					xtype : 'textfield',
					fieldLabel : '缩写',
					name : 'abbreviation',
					width : 300,
					allowBlank : true
				}, brCombo, {
					xtype : 'displayfield',
					fieldLabel : '签订合同总金额(约xxx万元人民币)',
					ref : 'contactMoney',
					hidden : true,
					name : 'contactMoney',
					editable : false,
					width : 300,
					allowBlank : true
				}, {
					xtype : 'hidden',
					fieldLabel : '地址ID',
					name : 'addressID',
					allowBlank : true
				}, {
					xtype : 'hidden',
					id : 'chooseAddressID',
					name : 'chooseAddressID'
				}, {
					xtype : 'fieldset',
					title : '办公地址',
					width : 400,
					defaultType : 'textfield',
					padding : '5px 0 5px 0px',
					defaults : {
						anchor : '100%'
					},
					items : [new Ext.ux.form.AddressChooseField({
								fieldLabel : '<font color="#CC0000">*</font>城市选择',
								name : 'chooseAddress',
								editable : false,
								allowBlank : false,
								msgTarget : 'under',
								setValueFun : function(name, ids, areaCodes) {
									compFormPanel.getForm()
											.findField('chooseAddress')
											.setValue(name);

									compFormPanel.getForm()
											.findField('internationID')
											.setValue(ids.internationID);

									compFormPanel.getForm()
											.findField('countryID')
											.setValue(ids.countryID);

									compFormPanel.getForm().findField('areaID')
											.setValue(ids.areaID);

									compFormPanel.getForm()
											.findField('provinceID')
											.setValue(ids.provinceID);

									compFormPanel.getForm().findField('cityID')
											.setValue(ids.cityID);

									var place = areaCodes.indexOf(',');
									var countryCode = areaCodes.substring(0,
											place);
									var cityCode = areaCodes.substring(place
													+ 1, areaCodes.length);
									compFormPanel.getForm()
											.findField('countryCode')
											.setValue(countryCode);
									compFormPanel.getForm()
											.findField('areaCode')
											.setValue(cityCode);
									compFormPanel.getForm()
											.findField('countryCode2')
											.setValue(countryCode);
									compFormPanel.getForm()
											.findField('areaCode2')
											.setValue(cityCode);
									compFormPanel.getForm()
											.findField('countryCode3')
											.setValue(countryCode);
									compFormPanel.getForm()
											.findField('areaCode3')
											.setValue(cityCode);
									compFormPanel.getForm()
											.findField('countryCode4')
											.setValue(countryCode);
									compFormPanel.getForm()
											.findField('areaCode4')
											.setValue(cityCode);

								},
								getValueFun : function() {
									var a, b, c, d, e;
									a = compFormPanel.getForm()
											.findField('internationID')
											.getValue();

									b = compFormPanel.getForm()
											.findField('countryID').getValue();

									c = compFormPanel.getForm()
											.findField('areaID').getValue();

									d = compFormPanel.getForm()
											.findField('provinceID').getValue();

									e = compFormPanel.getForm()
											.findField('cityID').getValue();
									return [a, b, c, d, e];
								},
								clearValueFun : function() {
									compFormPanel.getForm()
											.findField('chooseAddress')
											.setValue('');

									compFormPanel.getForm()
											.findField('internationID')
											.setValue('');

									compFormPanel.getForm()
											.findField('countryID')
											.setValue('');

									compFormPanel.getForm().findField('areaID')
											.setValue('');

									compFormPanel.getForm()
											.findField('provinceID')
											.setValue('');

									compFormPanel.getForm().findField('cityID')
											.setValue('');
								}
							}), {
						xtype : 'textfield',
						fieldLabel : '区县',
						name : 'district',
						allowBlank : true
					}, {
						xtype : 'textfield',
						fieldLabel : '邮编',
						maskRe : new RegExp('[' + Ext.escapeRe('0123456789')
								+ ']'),
						regex : /^[1-9]\d{5}$/,
						name : 'zipCode',
						allowBlank : true
					}, {
						xtype : 'textarea',
						fieldLabel : '详细地址',
						name : 'addressInfo',
						allowBlank : true
					}, {
						xtype : 'hidden',
						id : 'contactPersonID',
						name : 'contactPersonID'
					}, {
						xtype : 'hidden',
						fieldLabel : '洲际ID',
						name : 'internationID',
						allowBlank : true
					}, {
						xtype : 'hidden',
						fieldLabel : '洲际名称',
						name : 'internationName',
						allowBlank : true
					}, {
						xtype : 'hidden',
						fieldLabel : '国家ID',
						name : 'countryID',
						allowBlank : true
					}, {
						xtype : 'hidden',
						fieldLabel : '国家名称',
						name : 'countryName',
						allowBlank : true
					}, {
						xtype : 'hidden',
						fieldLabel : '地区ID',
						name : 'areaID',
						allowBlank : true
					}, {
						xtype : 'hidden',
						fieldLabel : '地区名称',
						name : 'areaName',
						allowBlank : true
					}, {
						xtype : 'hidden',
						fieldLabel : '省ID',
						name : 'provinceID',
						allowBlank : true
					}, {
						xtype : 'hidden',
						fieldLabel : '省名称',
						name : 'provinceName',
						allowBlank : true
					}, {
						xtype : 'hidden',
						fieldLabel : '市ID',
						name : 'cityID',
						allowBlank : true
					}, {
						xtype : 'hidden',
						fieldLabel : '市名称',
						name : 'cityName',
						allowBlank : true
					}]
				}, {
					xtype : 'textfield',
					ref : 'contactEditField',
					border : false,
					width : 160,
					fieldLabel : '联系人',
					emptyText : '输入姓名后请保存',
					name : 'contactEdit',
					allowBlank : true

				}, {
					xtype : 'textfield',
					ref : 'legalRepresentativeEditField',
					border : false,
					width : 160,
					fieldLabel : '法人代表',
					name : 'legalRepresentativeEdit',
					allowBlank : true

				}, {
					layout : 'column',
					xtype : 'panel',
					border : false,
					width : 370,
					items : [{
						columnWidth : .7,
						layout : 'form',
						border : false,
						items : [new Ext.ux.form.ChooseOrgTreeField({
							fieldLabel : '联系人',
							ref : '../../contactChooseOrgTreeField',
							name : 'contactPersonName',
							hidden : true,
							modal : true,
							width : 150,
							winConfig : {
								title : '联系人设置',
								bbar : ['<font color="#003366">*双击人员节点，可以查看此人的详细信息</font>'],
								tbar : [{
									ref : '../addBtn',
									iconCls : 'add',
									align : 'right',
									text : '新建',
									handler : function() {
										tabs.setActiveTab(1);
										wstate = 1;
										compFormPanel.contactChooseOrgTreeField.window
												.hide();
									}
								}]
							},
							singleSelect : true,
							onlyPerson : true,
							getExpandPath : function() {
								return {
									empId : compFormPanel.getForm()
											.findField('contactPersonID')
											.getValue()
								};
							},
							rootConfig : function() {
								return {
									id : 'comp-old-'
											+ compFormPanel.getForm()
													.findField('companyID')
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
												+ ','
									}
								}
								compFormPanel.getForm()
										.findField('contactPersonID')
										.setValue(empID.split('-')[2]);
								compFormPanel.getForm()
										.findField('contactPersonName')
										.setValue(empName);
							},
							listeners : {
								'beforeShowWin' : function() {
									var aCmpId = compFormPanel.getForm()
											.findField('companyID').getValue();
									if (!aCmpId || aCmpId.trim() == "") {
										Ext.Msg.alert("提示", "请先创建公司");
										return false;
									}
								},
								'dblclick' : function(node, e) {
									var empId = Ext.getCmp('contactPersonID')
											.getValue();
									com.bitc.xmis.util.EmpInfoWin(empId,
											'联系人信息');
								},
								'render' : function(c) {
									Ext.QuickTips.register({
												target : c.getEl(),
												text : '双击可显示详细信息'
											});
								},
								'nodedblclick' : function(node, e) {
									wstate = 2;
									tabs.setActiveTab(1);

									compFormPanel.contactChooseOrgTreeField.window
											.hide();
									var path = String.format('/root{0}', node
													.getPath());

									if (!Ext.isEmpty(path)) {
										Ext.getCmp('t2').items.items[0].orgtree
												.expandPath(path);
										Ext.getCmp('t2').items.items[0].orgtree
												.selectPath(path, null,
														function(s, n) {
															if (s) {
																Ext
																		.getCmp('t2').items.items[0].orgtree
																		.fireEvent(
																				'click',
																				n);
															}
														});
									}

								}
							},
							clearValueList : ''
						})]

					}, {
						columnWidth : .3,
						layout : 'form',
						border : false,
						items : [{
							layout : 'form',
							xtype : 'button',
							text : '<font color="#003366">编辑</font>',
							ref : '../../contractEdit_btn',
							hidden : true,
							handler : function() {
								var contactPersonID = compFormPanel.getForm()
										.findField('contactPersonID')
										.getValue();
								if (contactPersonID == null
										|| contactPersonID == '') {
									return;
								}
								Ext.Ajax.request({
									url : __cxtPath
											+ '/orgMaint.do?method=getEmpPathByEmpId',
									method : 'post',
									params : {
										id : contactPersonID
									},
									success : function(response, action) {
										var result = Ext
												.decode(response.responseText);
										var path = result.attributes.nodepath;

										tabs.setActiveTab(1);
										if (!Ext.isEmpty(path)) {
											Ext.getCmp('t2').items.items[0].orgtree
													.expandPath(path);
											Ext.getCmp('t2').items.items[0].orgtree
													.selectPath(path, null,
															function(s, n) {
																if (s) {
																	Ext
																			.getCmp('t2').items.items[0].orgtree
																			.fireEvent(
																					'click',
																					n);
																}
															});
										}

										// tabs.orgInfoPanel.orgtree
										// .expandPath(path);
										// tabs.orgInfoPanel.orgtree.selectPath(
										// path, null, function(s, n) {
										// if (s) {
										// tabs.orgInfoPanel.orgtree
										// .fireEvent(
										// 'click',
										// n);
										// }
										// });
									},
									failure : function(response, action) {
										Ext.MessageBox.hide();
										Ext.MessageBox.alert("提示", "操作失败！");
									}
								});

							}
						}]
					}]
				}, {
					layout : 'column',
					xtype : 'panel',
					border : false,
					width : 370,
					items : [{
						columnWidth : .7,
						layout : 'form',
						border : false,
						items : [new Ext.ux.form.ChooseOrgTreeField({
							fieldLabel : '法人代表',
							ref : '../../legalChooseOrgTreeField',
							hidden : true,
							width : 150,
							name : 'legalRepresentative',
							winConfig : {
								title : '选择法人代表',
								tbar : [{
									ref : '../addBtn',
									iconCls : 'add',
									align : 'right',
									text : '新建',
									bbar : ['*双击人员节点，可以查看此人的详细信息'],
									handler : function() {
										tabs.setActiveTab(1);
										lstate = 1;
										compFormPanel.legalChooseOrgTreeField.window
												.hide()
									}
								}]
							},
							singleSelect : true,
							onlyPerson : true,
							getExpandPath : function() {
								return {
									empId : compFormPanel.getForm()
											.findField('legalRepresentativeID')
											.getValue()
								};
							},
							rootConfig : function() {
								return {
									id : 'comp-old-'
											+ compFormPanel.getForm()
													.findField('companyID')
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
												+ ','
									}
								}
								compFormPanel.getForm()
										.findField('legalRepresentativeID')
										.setValue(empID.split('-')[2]);
								compFormPanel.getForm()
										.findField('legalRepresentative')
										.setValue(empName);

							},
							listeners : {
								'beforeShowWin' : function() {
									var aCmpId = compFormPanel.getForm()
											.findField('companyID').getValue();
									if (!aCmpId || aCmpId.trim() == "") {
										Ext.Msg.alert("提示", "请先创建公司");
										return false;
									}
								},
								'dblclick' : function(select, num) {
									var empId = Ext
											.getCmp('legalRepresentativeID')
											.getValue();
									com.bitc.xmis.util.EmpInfoWin(empId,
											'法人代表信息');
								},
								'render' : function(c) {
									Ext.QuickTips.register({
												target : c.getEl(),
												text : '双击可显示详细信息'
											});
								},
								'nodedblclick' : function(node, e) {
									lstate = 2;
									tabs.setActiveTab(1);
									compFormPanel.legalChooseOrgTreeField.window
											.hide();
									var path = String.format('/root{0}', node
													.getPath());

									if (!Ext.isEmpty(path)) {
										Ext.getCmp('t2').items.items[0].orgtree
												.expandPath(path);
										Ext.getCmp('t2').items.items[0].orgtree
												.selectPath(path, null,
														function(s, n) {
															if (s) {
																Ext
																		.getCmp('t2').items.items[0].orgtree
																		.fireEvent(
																				'click',
																				n);
															}
														});
									}
								}
							},
							clearValueList : ''
						})]
					}, {
						columnWidth : .3,
						layout : 'form',
						border : false,
						items : [{
							layout : 'form',
							xtype : 'button',
							ref : '../../legalEdit_btn',
							hidden : true,
							text : '<font color="#003366">编辑</font>',
							handler : function() {
								var legalRepresentativeID = compFormPanel
										.getForm()
										.findField('legalRepresentativeID')
										.getValue();
								if (legalRepresentativeID == null
										|| legalRepresentativeID == '') {
									return;
								}
								Ext.Ajax.request({
									url : __cxtPath
											+ '/orgMaint.do?method=getEmpPathByEmpId',
									method : 'post',
									params : {
										id : legalRepresentativeID
									},
									success : function(response, action) {
										var result = Ext
												.decode(response.responseText);
										var path = result.attributes.nodepath;

										tabs.setActiveTab(1);
										if (!Ext.isEmpty(path)) {
											Ext.getCmp('t2').items.items[0].orgtree
													.expandPath(path);
											Ext.getCmp('t2').items.items[0].orgtree
													.selectPath(path, null,
															function(s, n) {
																if (s) {
																	Ext
																			.getCmp('t2').items.items[0].orgtree
																			.fireEvent(
																					'click',
																					n);
																}
															});
										}
									},
									failure : function(response, action) {
										Ext.MessageBox.hide();
										Ext.MessageBox.alert("提示", "操作失败！");
									}
								});

							}
						}]
					}]

				}, {
					xtype : 'panel',
					border : false,
					layout : 'hbox',
					fieldLabel : '电话一',
					items : [{
								xtype : 'displayfield',
								name : 'countryCode',
								emptyText : '国际区号',
								readOnly : true,
								width : 32,
								height : 22,
								allowBlank : true
							}, {
								xtype : 'displayfield',
								value : '-'
							}, {
								xtype : 'displayfield',
								name : 'areaCode',
								margins : '0 0 0 5',
								readOnly : true,
								emptyText : '地区号码',
								width : 32,
								height : 22,
								allowBlank : true
							}, {
								xtype : 'displayfield',
								value : '-'
							}, {
								xtype : 'textfield',
								name : 'telephoneNo1',
								emptyText : '请输入电话号码',
								margins : '0 0 0 5',
								maskRe : new RegExp('['
										+ Ext.escapeRe('0123456789+-_ ()')
										+ ']'),
								width : 110,
								height : 22,
								allowBlank : true
							}]
				}, {
					xtype : 'panel',
					border : false,
					layout : 'hbox',
					fieldLabel : '电话二',
					items : [{
								xtype : 'displayfield',
								name : 'countryCode2',
								emptyText : '国际区号',
								readOnly : true,
								width : 32,
								height : 22,
								allowBlank : true
							}, {
								xtype : 'displayfield',
								value : '-'
							}, {
								xtype : 'displayfield',
								name : 'areaCode2',
								emptyText : '地区号码',
								readOnly : true,
								width : 32,
								margins : '0 0 0 5',
								height : 22,
								allowBlank : true
							}, {
								xtype : 'displayfield',
								value : '-'
								// }, {
							// xtype : 'numberfield',
							// name : 'telephoneNo2',
							// emptyText : '请输入电话号码',
							// width : 110,
							// margins : '0 0 0 5',
							// height : 22,
							// allowBlank : true
						}	, {
								xtype : 'textfield',
								name : 'telephoneNo2',
								emptyText : '请输入电话号码',
								width : 110,
								margins : '0 0 0 5',
								maskRe : new RegExp('['
										+ Ext.escapeRe('0123456789+-_ ()')
										+ ']'),
								height : 22,
								allowBlank : true
							}]
				}, {
					xtype : 'panel',
					border : false,
					layout : 'hbox',
					fieldLabel : '传真一',
					items : [{
								xtype : 'displayfield',
								name : 'countryCode3',
								emptyText : '国际区号',
								readOnly : true,
								width : 32,
								height : 22,
								allowBlank : true
							}, {
								xtype : 'displayfield',
								value : '-'
							}, {
								xtype : 'displayfield',
								name : 'areaCode3',
								emptyText : '地区号码',
								readOnly : true,
								width : 32,
								margins : '0 0 0 5',
								height : 22,
								allowBlank : true
							}, {
								xtype : 'displayfield',
								value : '-'
							}, {
								xtype : 'textfield',
								name : 'faxNo1',
								emptyText : '请输入传真号码',
								width : 110,
								margins : '0 0 0 5',
								maskRe : new RegExp('['
										+ Ext.escapeRe('0123456789+-_ ()')
										+ ']'),
								height : 22,
								allowBlank : true
							}]
				}, {
					xtype : 'panel',
					border : false,
					layout : 'hbox',
					fieldLabel : '传真二',
					items : [{
								xtype : 'displayfield',
								name : 'countryCode4',
								emptyText : '国际区号',
								readOnly : true,
								width : 32,
								height : 22,
								allowBlank : true
							}, {
								xtype : 'displayfield',
								value : '-'
							}, {
								xtype : 'displayfield',
								name : 'areaCode4',
								emptyText : '地区号码',
								readOnly : true,
								width : 32,
								margins : '0 0 0 5',
								height : 22,
								allowBlank : true
							}, {
								xtype : 'displayfield',
								value : '-'
							}, {
								xtype : 'textfield',
								name : 'faxNo2',
								emptyText : '请输入传真号码',
								width : 110,
								margins : '0 0 0 5',
								maskRe : new RegExp('['
										+ Ext.escapeRe('0123456789+-_ ()')
										+ ']'),
								height : 22,
								allowBlank : true
							}]
				}, {
					xtype : 'textfield',
					fieldLabel : '网址',
					name : 'webSite',
					width : 300,
					allowBlank : true,
					regex : /([\w-]+\.)+[\w-]+.([^a-z])(\[\w- .\?%&=]*)?|[a-zA-Z0-9\-\.][\w-]+.([^a-z])(\[\w- .\?%&=]*)? /
				}, {
					xtype : 'textfield',
					fieldLabel : '邮箱',
					name : 'emailAddress',
					regex : /^[\w-]+(\.[\w-]+)*@[\w-]+(\.[\w-]+)+$/,
					width : 300,
					allowBlank : true
				}, {
					xtype : 'textfield',
					fieldLabel : '开户银行',
					width : 300,
					name : 'bankName',
					allowBlank : true
				}, {
					xtype : 'textfield',
					fieldLabel : '账号',
					width : 300,
					name : 'accountNo',
					allowBlank : true
				}, {
					xtype : 'textfield',
					fieldLabel : '税号',
					name : 'taxNo',
					width : 300,
					allowBlank : true
				}, new com.bitc.xmis.Combos.CompanyScaleCombo({
							fieldLabel : '公司规模',
							width : 150,
							hiddenName : 'companyScaleID'
						}), {
					xtype : 'hidden',
					id : 'legalRepresentativeID',
					name : 'legalRepresentativeID'
				}, new com.bitc.xmis.Combos.CustomerLevelCombo({
							fieldLabel : '客户级别',
							width : 150,
							hiddenName : 'customerLevelID'
						}), new com.bitc.xmis.Combos.CreditLevelCombo({
							fieldLabel : '信用等级',
							width : 150,
							hiddenName : 'creditLevelID'
						}), {
					xtype : 'textfield',
					fieldLabel : '机构代码',
					width : 150,
					name : 'organizationCode',
					allowBlank : true
				}, {
					xtype : 'panel',
					border : false,
					layout : 'hbox',
					fieldLabel : '注册资金',
					items : [{
								xtype : 'formatnum',
								name : 'registeredCapital',
								width : 205,
								height : 22,
								allowBlank : true
							}, new com.bitc.xmis.Combos.CurrencyCombo({
										fieldLabel : '货币类型',
										margins : '0 0 0 5',
										hiddenName : 'currencyTypeID',
										height : 22,
										width : 90
									})]
				}, {
					xtype : 'textarea',
					fieldLabel : '备注',
					name : 'remark',
					width : 300,
					allowBlank : true
				}, {
					xtype : 'hidden',
					fieldLabel : '客户来源ID',
					name : 'customerSourceID',
					allowBlank : true,
					anchor : '60%'
				}, {
					xtype : 'hidden',
					fieldLabel : '创建人ID',
					name : 'updateID',
					allowBlank : true,
					anchor : '60%'
				}, {
					xtype : 'panel',
					layout : 'column',
					width : 400,
					border : false,
					items : [{
						xtype : 'panel',
						layout : 'form',
						columnWidth : .5,
						labelAlign : 'right',
						labelWidth : 90,
						border : false,
						items : [{
									xtype : 'hidden',
									name : 'updatePersonId'
								}, {
									xtype : 'displayfield',
									fieldLabel : '创建人',
									name : 'updatePerson',
									value : '',
									anchor : '100%',
									initEvents : function() {
										this.mon(this.el, 'dblclick',
												this.onDblClick, this);
									},
									onDblClick : function() {
										com.bitc.xmis.util.EmpInfoWin(
												company.updatePerson.userID,
												'创建人信息');
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
						columnWidth : .5,
						labelAlign : 'right',
						labelWidth : 90,
						border : false,
						items : [{
									xtype : 'hidden',
									name : 'updateDateId'
								}, {
									xtype : 'displayfield',
									fieldLabel : '创建日期',
									name : 'updateDate',
									value : '',
									anchor : '100%'
								}]
					}]
				}, {
					xtype : 'panel',
					layout : 'column',
					width : 400,
					border : false,
					items : [{
						xtype : 'panel',
						layout : 'form',
						columnWidth : .5,
						labelAlign : 'right',
						border : false,
						labelWidth : 90,
						items : [{
									xtype : 'hidden',
									name : 'lastModifierNameId'
								}, {
									xtype : 'displayfield',
									fieldLabel : '最后修改人',
									name : 'lastModifierName',
									value : '',
									anchor : '100%',
									initEvents : function() {
										this.mon(this.el, 'dblclick',
												this.onDblClick, this);
									},
									onDblClick : function() {
										com.bitc.xmis.util.EmpInfoWin(
												company.lastModifier.userID,
												'最后修改人信息');
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
						border : false,
						columnWidth : .5,
						labelAlign : 'right',
						labelWidth : 90,
						items : [{
									xtype : 'hidden',
									name : 'lastModifiedId'
								}, {
									xtype : 'displayfield',
									fieldLabel : '最后修改日期',
									name : 'lastModified',
									value : '',
									anchor : '100%'
								}]
					}]
				}, {
					xtype : 'hidden',
					fieldLabel : '保密级别',
					name : 'securityLevelID',
					allowBlank : true,
					anchor : '60%'
				}]
	});

	compFormPanel.getForm().findField('companyName').on('blur', function(c) {
		if (compFormPanel.getForm().findField('abbreviation').getValue() == '') {// 在缩写不为空时候，缩写为公司名称
			compFormPanel.getForm().findField('abbreviation')
					.setValue(compFormPanel.getForm().findField('companyName')
							.getValue());
		}
	});

	var relativeAdressStoreRecord = Ext.data.Record.create([{
				"mapping" : "id",
				"name" : "id",
				"type" : "string"
			}, {
				"mapping" : "companyId",
				"name" : "companyId",
				"type" : "int"
			}, {
				"mapping" : "address",
				"name" : "address",
				"type" : "auto"
			}]);

	var relativeAdressStore = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : __cxtPath
									+ '/crmMaint.do?method=getRelativeAddressList',
							method : "POST"
						}),
				baseParams : {
					companyId : selectCompanyID,
					limit : 20
				},
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount'
						}, relativeAdressStoreRecord),
				remoteSort : true
			});

	var relativeAdressCm = new Ext.grid.ColumnModel([
			new Ext.grid.RowNumberer(), {
				header : 'id',
				hidden : true,
				dataIndex : 'id'
			}, {
				header : '<font color="#003366"><b>地址</b></font>',
				width : 400,
				dataIndex : 'address',
				renderer : function(value) {
					if (value != null) {
						var isTrue = (value.city.province.provinceName.trim()) == (value.city.cityName
								.trim());
						var showname = '';
						if (isTrue) {
							showname = value.city.province.provinceName
									+ value.district
									+ value.addressInfo
									+ (value.zipCode == 0 ? '' : ' 邮编：'
											+ value.zipCode);
						} else {
							showname = value.city.province.provinceName
									+ value.city.cityName
									+ value.district
									+ value.addressInfo
									+ (value.zipCode == 0 ? '' : ' 邮编：'
											+ value.zipCode);

						}
						return showname;
					} else {
						return '';
					}
				}
			}, {
				xtype : 'actioncolumn',
				width : 120,
				align : 'center',
				items : [{
					iconCls : 'edit',
					tooltip : '编辑',
					handler : function(grid, rowIndex, colIndex) {
						var rec = relativeAdressStore.getAt(rowIndex);
						relativeAddressWin.show();
						relativeAddressFormPanel.getForm().load({
							url : __cxtPath
									+ '/crmMaint.do?method=getRelativeAddress',
							params : {
								id : rec.get('id')
							},
							waitMsg : '正在载入数据...',
							waitTitle : '请等待...',
							success : function(response, action) {
							},
							failure : function(response, action) {
								Ext.MessageBox.alert('提示', '载入失败');
							}

						});
					}
				}, {
					icon : './resources/images/split-false.gif',
					tooltip : '删除',
					handler : function(grid, rowIndex, colIndex) {
						var rec = relativeAdressStore.getAt(rowIndex);
						Ext.MessageBox.confirm("提示", "是否删除此条信息？",
								function(btn) {
									if (btn == 'yes') {
										Ext.Ajax.request({
											url : __cxtPath
													+ '/crmMaint.do?method=dodelRelativeAddressById',
											method : 'post',
											params : {
												id : rec.get('id')
											},
											success : function(response, action) {
												Ext.MessageBox.alert("提示",
														"删除成功！");
												relativeAdressStore.reload();
											},
											failure : function(response, action) {
												Ext.MessageBox.hide();
												Ext.MessageBox.alert("提示",
														"操作失败！");
											}
										});

									}
								});
					}
				}]
			}]);
	var relativeAdressGrid = new Ext.grid.GridPanel({
		region : 'center',
		forceFit : true,
		width : 550,
		height : 200,
		border : false,
		stripeRows : true,
		cm : relativeAdressCm,
		store : relativeAdressStore,
		tbar : ['模糊查询：', new Ext.ux.InklingSearchField({
							width : 120,
							store : relativeAdressStore,
							tooltip : {
								title : '模糊查询',
								text : '您可以输入“省”，“城市”，“区县”，“详细地址”中的任意一个关键词进行查询'
							},
							inklings : [{
										name : 'address.addressInfo',
										op : 'like',
										type : 'string'
									}, {
										name : 'address.district',
										op : 'like',
										type : 'string'
									}, {
										name : 'address.city.cityName',
										op : 'like',
										type : 'string'
									}, {
										name : 'address.city.province.provinceName',
										op : 'like',
										type : 'string'
									}]
						})]
	});
	var eastPanelComp = new Ext.Panel({
				region : 'east',
				width : 550,
				title : '相关地址',
				layout : 'border',
				border : false,
				split : true,
				autoScroll : true,
				collapsible : true, // 是否有右上方展开收缩条
				collapseMode : 'mini', // 将滚动收缩条模式‘迷你’
				items : [relativeAdressGrid]
			});

	var tabs = new Ext.TabPanel({
		region : 'center',
		margins : '3 3 3 0',
		activeTab : 0,
		autoScroll : true,
		width : 570,
		tabPosition : 'top',
		deferredRender : false,
		border : false,
		items : [{
			title : '基本信息',
			id : 't1',
			border : false,
			layout : 'border',
			tbar : {
				xtype : 'toolbar',
				id : 'tbar_saveCompInfo',
				items : [{
					xtype : 'button',
					text : '保存',
					iconCls : 'ok',
					handler : function() {
						if ("" == compFormPanel.getForm()
								.findField('companyName').getValue().trim()) {
							Ext.Msg.alert("提示", "公司名称为必输项！");
							return;
						}

						Ext.MessageBox.show({
									title : '请稍候',
									msg : '正在处理...',
									progressText : '操作中...',
									width : 300,
									wait : true,
									waitConfig : {
										interval : 200
									},
									closable : false
								});
						var abbreviation = compFormPanel.getForm()
								.findField('abbreviation').getValue();
						if ("" == abbreviation || "" == abbreviation.trim()) {
							compFormPanel.getForm().findField('abbreviation')
									.setValue(compFormPanel.getForm()
											.findField('companyName')
											.getValue());
						}
						compFormPanel.getForm().submit({
							url : __cxtPath
									+ '/crmMaint.do?method=doSaveCompanyInfo',
							params : {
								addressID : compFormPanel.getForm()
										.findField('addressID').getValue(),
								cityID : compFormPanel.getForm()
										.findField('cityID').getValue(),
								contactEdit : compFormPanel.getForm()
										.findField('contactEdit').getValue(),
								legalRepresentativeEdit : compFormPanel
										.getForm()
										.findField('legalRepresentativeEdit')
										.getValue(),
								companyID : compFormPanel.getForm()
										.findField('companyID').getValue(),
								telephoneNo1 : compFormPanel.getForm()
										.findField('telephoneNo1').getValue(),
								telephoneNo2 : compFormPanel.getForm()
										.findField('telephoneNo2').getValue(),
								faxNo1 : compFormPanel.getForm()
										.findField('faxNo1').getValue(),
								faxNo2 : compFormPanel.getForm()
										.findField('faxNo2').getValue()

							},
							success : function(response, action) {
								Ext.MessageBox.hide();
								if (selectCompanyID == null) {
									Ext.MessageBox.alert(
											action.result.messageTitle,
											action.result.message, function() {
												parent.xMis
														.turn(
																'company-info-'
																		+ action.result.attributes.id,
																compFormPanel
																		.getForm()
																		.findField('companyName')
																		.getValue(),
																__cxtPath
																		+ '/crmMaint.do?method=getCompanyInfo&companyID='
																		+ action.result.attributes.id,
																'menu-company-info',
																true);

												if (selectCompanyID == null) {
													parent.xMis
															.closeTab(window);
												}

											});

								} else {
									Ext.MessageBox.alert(
											action.result.messageTitle,
											action.result.message);

								}
								tabs.addRelativeAddressButton.setVisible(true);
								compFormPanel.contactChooseOrgTreeField
										.setVisible(true);
								compFormPanel.legalRepresentativeEditField
										.setVisible(false);
								compFormPanel.contactEditField
										.setVisible(false);
								compFormPanel.legalChooseOrgTreeField
										.setVisible(true);
								compFormPanel.contactMoney.setVisible(true);
								Ext.getCmp('officeAddressNameCombo').getStore()
										.reload();
							},
							failure : function(response, action) {
								com.bitc.xmis.util.AlertUtil
										.formSubmitFailedAlert(action)
							}
						});
					}
				}, '-', {
					xtype : 'button',
					ref : '../../addRelativeAddressButton',
					text : '添加相关地址',
					iconCls : 'add',
					hidden : true,
					handler : function() {
						relativeAddressFormPanel.getForm().reset();
						relativeAddressWin.show();
					}
				}, '-', {
					xtype : 'button',
					text : '刷新',
					iconCls : 'reload',
					ref : '../refreshButton',
					handler : function() {
						reloadtab();
					}
				}]
			},
			items : [compFormPanel, eastPanelComp]
		}],
		listeners : {
			'tabchange' : function(t, p) {
				currenttab = p.getItemId();
				if (p.getItemId() == 't2' && cflag == 0) {// 如果是初次点击“组织结构tab”
					var panels = buildPanel(selectCompanyID);
					var pp = new Ext.Panel({
								height : ht - 33,
								layout : 'border',
								border : false,
								items : [panels[0], panels[1]]
							});
					Ext.getCmp('t2').add(pp);
					cflag++;
					p.doLayout();

					// 其它页面跳转到人员的详细信息页面，节点选中
					var path = request.getParameter("path");
					if (!Ext.isEmpty(path)) {
						pp.orgtree.expandPath(path);
						pp.orgtree.selectPath(path, null, function(s, n) {
									if (s) {
										pp.orgtree.fireEvent('click', n);
									}
								});
					}
				}

				if (p.getItemId() == 't1' && wstate == 1) {
					compFormPanel.contactChooseOrgTreeField.window.show();
					Ext.Msg.show({
						title : '提示',
						msg : '新建成功后刷新',
						buttons : Ext.Msg.OK,
						fn : function() {
							compFormPanel.contactChooseOrgTreeField
									.getTreePanel()
									.getLoader()
									.load(
											compFormPanel.contactChooseOrgTreeField
													.getTreePanel()
													.getRootNode(),
											function(n) {
												compFormPanel.contactChooseOrgTreeField
														.getTreePanel()
														.expand();
											});
						}
					});
					wstate = 0;
				}

				if (p.getItemId() == 't1' && wstate == 2) {
					compFormPanel.contactChooseOrgTreeField.window.show();
					wstate = 0;
				}
				if (p.getItemId() == 't1' && lstate == 1) {
					compFormPanel.legalChooseOrgTreeField.window.show();
					Ext.Msg.show({
						title : '提示',
						msg : '新建成功后刷新',
						buttons : Ext.Msg.OK,
						fn : function() {
							compFormPanel.legalChooseOrgTreeField
									.getTreePanel()
									.getLoader()
									.load(
											compFormPanel.legalChooseOrgTreeField
													.getTreePanel()
													.getRootNode(),
											function(n) {
												compFormPanel.legalChooseOrgTreeField
														.getTreePanel()
														.expand();
											});
						}
					});
					lstate = 0;
				}

				if (p.getItemId() == 't1' && lstate == 2) {
					compFormPanel.legalChooseOrgTreeField.window.show();
					lstate = 0;
				}
				// 供方评定
				if (p.getItemId() == 't3' && owner == 1) {
					supplierInformation.managerChooseOrgTreeField.window.show();
					Ext.Msg.show({
						title : '提示',
						msg : '新建成功后刷新',
						buttons : Ext.Msg.OK,
						fn : function() {
							supplierInformation.managerChooseOrgTreeField
									.getTreePanel()
									.getLoader()
									.load(
											supplierInformation.managerChooseOrgTreeField
													.getTreePanel()
													.getRootNode(),
											function(n) {
												supplierInformation.managerChooseOrgTreeField
														.getTreePanel()
														.expand();
											});
						}
					});
					owner = 0;
				}

				if (p.getItemId() == 't3' && owner == 2) {
					supplierInformation.managerChooseOrgTreeField.window.show();
					owner = 0;
				}

				if (p.getItemId() == 't4' && qualifiedperson == 1) {
					qualificationPanel.qualificationChooseOrgTreeField.window
							.show();
					Ext.Msg.show({
						title : '提示',
						msg : '新建成功后刷新',
						buttons : Ext.Msg.OK,
						fn : function() {
							qualificationPanel.qualificationChooseOrgTreeField
									.getTreePanel()
									.getLoader()
									.load(
											qualificationPanel.qualificationChooseOrgTreeField
													.getTreePanel()
													.getRootNode(),
											function(n) {
												qualificationPanel.qualificationChooseOrgTreeField
														.getTreePanel()
														.expand();
											});
						}
					});
					qualifiedperson = 0;
				}

				if (p.getItemId() == 't4' && qualifiedperson == 2) {
					qualificationPanel.qualificationChooseOrgTreeField.window
							.show();
					qualifiedperson = 0;
				}
				// 设计人1
				if (p.getItemId() == 't5' && patentHolder1 == 1) {
					patentPanel.patentedDesignPeopleChooseOrgTreeField1.window
							.show();
					Ext.Msg.show({
						title : '提示',
						msg : '新建成功后刷新',
						buttons : Ext.Msg.OK,
						fn : function() {
							patentPanel.patentedDesignPeopleChooseOrgTreeField1
									.getTreePanel()
									.getLoader()
									.load(
											patentPanel.patentedDesignPeopleChooseOrgTreeField1
													.getTreePanel()
													.getRootNode(),
											function(n) {
												patentPanel.patentedDesignPeopleChooseOrgTreeField1
														.getTreePanel()
														.expand();
											});
						}
					});
					patentHolder1 = 0;
				}

				if (p.getItemId() == 't5' && patentHolder1 == 2) {
					patentPanel.patentedDesignPeopleChooseOrgTreeField1.window
							.show();
					patentHolder1 = 0;
				}
				// 设计人2
				if (p.getItemId() == 't5' && patentHolder2 == 1) {
					patentPanel.patentedDesignPeopleChooseOrgTreeField2.window
							.show();
					Ext.Msg.show({
						title : '提示',
						msg : '新建成功后刷新',
						buttons : Ext.Msg.OK,
						fn : function() {
							patentPanel.patentedDesignPeopleChooseOrgTreeField2
									.getTreePanel()
									.getLoader()
									.load(
											patentPanel.patentedDesignPeopleChooseOrgTreeField2
													.getTreePanel()
													.getRootNode(),
											function(n) {
												patentPanel.patentedDesignPeopleChooseOrgTreeField2
														.getTreePanel()
														.expand();
											});
						}
					});
					patentHolder2 = 0;
				}

				if (p.getItemId() == 't5' && patentHolder2 == 2) {
					patentPanel.patentedDesignPeopleChooseOrgTreeField2.window
							.show();
					patentHolder2 = 0;
				}
				// 设计人3
				if (p.getItemId() == 't5' && patentHolder3 == 1) {
					patentPanel.patentedDesignPeopleChooseOrgTreeField3.window
							.show();
					Ext.Msg.show({
						title : '提示',
						msg : '新建成功后刷新',
						buttons : Ext.Msg.OK,
						fn : function() {
							patentPanel.patentedDesignPeopleChooseOrgTreeField3
									.getTreePanel()
									.getLoader()
									.load(
											patentPanel.patentedDesignPeopleChooseOrgTreeField3
													.getTreePanel()
													.getRootNode(),
											function(n) {
												patentPanel.patentedDesignPeopleChooseOrgTreeField3
														.getTreePanel()
														.expand();
											});
						}
					});
					patentHolder3 = 0;
				}

				if (p.getItemId() == 't5' && patentHolder3 == 2) {
					patentPanel.patentedDesignPeopleChooseOrgTreeField3.window
							.show();
					patentHolder3 = 0;
				}
				// 设计人4
				if (p.getItemId() == 't5' && patentHolder4 == 1) {
					patentPanel.patentedDesignPeopleChooseOrgTreeField4.window
							.show();
					Ext.Msg.show({
						title : '提示',
						msg : '新建成功后刷新',
						buttons : Ext.Msg.OK,
						fn : function() {
							patentPanel.patentedDesignPeopleChooseOrgTreeField4
									.getTreePanel()
									.getLoader()
									.load(
											patentPanel.patentedDesignPeopleChooseOrgTreeField4
													.getTreePanel()
													.getRootNode(),
											function(n) {
												patentPanel.patentedDesignPeopleChooseOrgTreeField4
														.getTreePanel()
														.expand();
											});
						}
					});
					patentHolder4 = 0;
				}

				if (p.getItemId() == 't5' && patentHolder4 == 2) {
					patentPanel.patentedDesignPeopleChooseOrgTreeField4.window
							.show();
					patentHolder4 = 0;
				}
				// 设计人5
				if (p.getItemId() == 't5' && patentHolder5 == 1) {
					patentPanel.patentedDesignPeopleChooseOrgTreeField5.window
							.show();
					Ext.Msg.show({
						title : '提示',
						msg : '新建成功后刷新',
						buttons : Ext.Msg.OK,
						fn : function() {
							patentPanel.patentedDesignPeopleChooseOrgTreeField5
									.getTreePanel()
									.getLoader()
									.load(
											patentPanel.patentedDesignPeopleChooseOrgTreeField5
													.getTreePanel()
													.getRootNode(),
											function(n) {
												patentPanel.patentedDesignPeopleChooseOrgTreeField5
														.getTreePanel()
														.expand();
											});
						}
					});
					patentHolder5 = 0;
				}

				if (p.getItemId() == 't5' && patentHolder5 == 2) {
					patentPanel.patentedDesignPeopleChooseOrgTreeField5.window
							.show();
					patentHolder5 = 0;
				}
				// 专利权人1
				if (p.getItemId() == 't5' && patentRight1 == 1) {
					patentPanel.patentRightPeopleChooseOrgTreeField1.window
							.show();
					Ext.Msg.show({
						title : '提示',
						msg : '新建成功后刷新',
						buttons : Ext.Msg.OK,
						fn : function() {
							patentPanel.patentRightPeopleChooseOrgTreeField1
									.getTreePanel()
									.getLoader()
									.load(
											patentPanel.patentRightPeopleChooseOrgTreeField1
													.getTreePanel()
													.getRootNode(),
											function(n) {
												patentPanel.patentRightPeopleChooseOrgTreeField1
														.getTreePanel()
														.expand();
											});
						}
					});
					patentRight1 = 0;
				}

				if (p.getItemId() == 't5' && patentRight1 == 2) {
					patentPanel.patentRightPeopleChooseOrgTreeField1.window
							.show();
					patentRight1 = 0;
				}
				// 专利权人2
				if (p.getItemId() == 't5' && patentRight2 == 1) {
					patentPanel.patentRightPeopleChooseOrgTreeField2.window
							.show();
					Ext.Msg.show({
						title : '提示',
						msg : '新建成功后刷新',
						buttons : Ext.Msg.OK,
						fn : function() {
							patentPanel.patentRightPeopleChooseOrgTreeField2
									.getTreePanel()
									.getLoader()
									.load(
											patentPanel.patentRightPeopleChooseOrgTreeField2
													.getTreePanel()
													.getRootNode(),
											function(n) {
												patentPanel.patentRightPeopleChooseOrgTreeField2
														.getTreePanel()
														.expand();
											});
						}
					});
					patentRight2 = 0;
				}

				if (p.getItemId() == 't5' && patentRight2 == 2) {
					patentPanel.patentRightPeopleChooseOrgTreeField2.window
							.show();
					patentRight2 = 0;
				}
				// 专利权人3
				if (p.getItemId() == 't5' && patentRight3 == 1) {
					patentPanel.patentRightPeopleChooseOrgTreeField3.window
							.show();
					Ext.Msg.show({
						title : '提示',
						msg : '新建成功后刷新',
						buttons : Ext.Msg.OK,
						fn : function() {
							patentPanel.patentRightPeopleChooseOrgTreeField3
									.getTreePanel()
									.getLoader()
									.load(
											patentPanel.patentRightPeopleChooseOrgTreeField3
													.getTreePanel()
													.getRootNode(),
											function(n) {
												patentPanel.patentRightPeopleChooseOrgTreeField3
														.getTreePanel()
														.expand();
											});
						}
					});
					patentRight3 = 0;
				}

				if (p.getItemId() == 't5' && patentRight3 == 2) {
					patentPanel.patentRightPeopleChooseOrgTreeField3.window
							.show();
					patentRight3 = 0;
				}
				// 专利权人4
				if (p.getItemId() == 't5' && patentRight4 == 1) {
					patentPanel.patentRightPeopleChooseOrgTreeField4.window
							.show();
					Ext.Msg.show({
						title : '提示',
						msg : '新建成功后刷新',
						buttons : Ext.Msg.OK,
						fn : function() {
							patentPanel.patentRightPeopleChooseOrgTreeField4
									.getTreePanel()
									.getLoader()
									.load(
											patentPanel.patentRightPeopleChooseOrgTreeField4
													.getTreePanel()
													.getRootNode(),
											function(n) {
												patentPanel.patentRightPeopleChooseOrgTreeField4
														.getTreePanel()
														.expand();
											});
						}
					});
					patentRight4 = 0;
				}

				if (p.getItemId() == 't5' && patentRight4 == 2) {
					patentPanel.patentRightPeopleChooseOrgTreeField4.window
							.show();
					patentRight4 = 0;
				}
				// 专利权人5
				if (p.getItemId() == 't5' && patentRight5 == 1) {
					patentPanel.patentRightPeopleChooseOrgTreeField5.window
							.show();
					Ext.Msg.show({
						title : '提示',
						msg : '新建成功后刷新',
						buttons : Ext.Msg.OK,
						fn : function() {
							patentPanel.patentRightPeopleChooseOrgTreeField5
									.getTreePanel()
									.getLoader()
									.load(
											patentPanel.patentRightPeopleChooseOrgTreeField5
													.getTreePanel()
													.getRootNode(),
											function(n) {
												patentPanel.patentRightPeopleChooseOrgTreeField5
														.getTreePanel()
														.expand();
											});
						}
					});
					patentRight5 = 0;
				}

				if (p.getItemId() == 't5' && patentRight5 == 2) {
					patentPanel.patentRightPeopleChooseOrgTreeField5.window
							.show();
					patentRight5 = 0;
				}

			}
		}
	});

	var supplierInformation = new Ext.FormPanel({
		region : 'center',
		padding : 20,
		autoScroll : true,
		border : false,
		labelWidth : 90,
		labelAlign : 'right',
		items : [{
					xtype : 'hidden',
					name : 'companyID',
					fieldLabel : 'ID',
					style : 'color:blue'
				}, new Ext.ux.form.ChooseOrgTreeField({
					fieldLabel : '经理',
					ref : 'managerChooseOrgTreeField',
					allowBlank : false,
					name : 'manager',
					id : 'manager',
					// value:1,
					width : 220,
					winConfig : {
						title : '选择经理',
						tbar : [{
							ref : '../addBtn',
							iconCls : 'add',
							align : 'right',
							text : '新建',
							handler : function() {
								tabs.setActiveTab(1);
								owner = 1;
								supplierInformation.managerChooseOrgTreeField.window
										.hide()
							}
						}]
					},
					singleSelect : true,
					onlyPerson : true,
					rootConfig : function() {
						return {
							id : 'comp-old-' + selectCompanyID
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
								empName = empName + result[i].text + ','
							}
						}
						Ext.getCmp('managerID').setValue(empID.split('-')[2]);

						Ext.getCmp('manager').setValue(empName);
						// ----------------------------------------------------------------------------------------------
						Ext.Ajax.request({
							url : __cxtPath
									+ '/crmMaint.do?method=getDecisionMakersInformation',// 研究所
							params : {
								hrID : empID.split('-')[2]
							},
							scope : this,
							success : function(response) {
								var result = Ext.decode(response.responseText);
								if (result.success) {
									if (result.rows[0]) {
										supplierInformation.getForm()
												.findField('DH')
												.setValue(result.rows[0].DH);
										supplierInformation.getForm()
												.findField('CZ')
												.setRawValue(result.rows[0].CZ);
										supplierInformation
												.getForm()
												.findField('Email')
												.setRawValue(result.rows[0].Email);
									}
								}
							},
							failure : function() {
							}
						});
						//									

						// ===============================================================================================

					},
					clearValueList : 'managerID,manager',
					listeners : {
						'beforeShowWin' : function() {
							var aCmpId = selectCompanyID;
							if (!aCmpId || aCmpId == "") {
								Ext.Msg.alert("提示", "请先创建公司");
								return false;
							}
						},
						'nodedblclick' : function(node, e) {
							owner = 2;
							tabs.setActiveTab(1);
							supplierInformation.managerChooseOrgTreeField.window
									.hide();
							var path = String
									.format('/root{0}', node.getPath());

							if (!Ext.isEmpty(path)) {
								Ext.getCmp('t2').items.items[0].orgtree
										.expandPath(path);
								Ext.getCmp('t2').items.items[0].orgtree
										.selectPath(path, null, function(s, n) {
											if (s) {
												Ext.getCmp('t2').items.items[0].orgtree
														.fireEvent('click', n);
											}
										});
							}
						}
					},
					clearValueList : ''

				}), {
					xtype : 'displayfield',
					fieldLabel : '电话',
					name : 'DH',
					width : 220,
					allowBlank : false

				}, {
					xtype : 'displayfield',
					fieldLabel : '传真',
					name : 'CZ',
					width : 170,
					allowBlank : false

				}, {
					xtype : 'displayfield',
					fieldLabel : 'Email',
					name : 'Email',
					allowBlank : false

				}, {
					xtype : 'panel',
					layout : 'column',
					border : false,
					width : 412,
					items : [{
						xtype : 'panel',
						layout : 'form',
						columnWidth : 0.8,
						labelWidth : 90,
						labelAlign : 'right',
						border : false,
						items : [{
							xtype : 'combo',
							fieldLabel : '地址',
							name : 'officeAddressName',
							id : 'officeAddressNameCombo',
							width : 220,
							displayField : 'officeAddressName',
							valueField : 'officeAddressID',
							triggerAction : 'all',
							editable : false,
							mode : 'romote',
							emptyText : '请选择地址',
							store : new Ext.data.JsonStore({
								url : __cxtPath
										+ '/crmMaint.do?method=getRelationAddress',
								root : 'rows',
								idProperty : 'officeAddressID',
								autoLoad : true,
								fields : [{
											name : 'officeAddressName'
										}, {
											name : 'officeAddressID'
										}],
								baseParams : {
									selectCompanyID : selectCompanyID
								}
							}),
							listeners : {
								'select' : function(combo, record, index) {
									supplierInformation.getForm()
											.findField('officeAddressID')
											.setValue(record.id);
								}
							}
						}]
					}, {
						xtype : 'panel',
						border : false,
						columnWidth : 0.2,
						items : [{
									layout : 'form',
									xtype : 'button',
									text : '<font color="#003366">新建</font>',
									handler : function() {
										relativeAddressFormPanel.getForm()
												.reset();
										relativeAddressWin.show();
									}
								}]
					}]
				}, {
					xtype : 'hidden',
					name : 'managerID',
					id : 'managerID',
					listeners : {
						'change' : function(field, newValue, oldValue) {
						}
					}
				}, {
					xtype : 'hidden',
					name : 'officeAddressID'
				}, {
					xtype : 'hidden',
					name : 'newregisterID'
				}, {
					xtype : 'hidden',
					name : 'lastModifier'
				}, {
					xtype : 'textfield',
					fieldLabel : '主管单位',
					name : 'headOfUnit',
					width : 220,
					allowBlank : false
				}, {
					xtype : 'textarea',
					fieldLabel : '行业评定情况',
					name : 'industryAssessment',
					width : 220,
					allowBlank : false
				}, {
					xtype : 'textarea',
					fieldLabel : '资质情况',
					name : 'qualification',
					width : 220,
					allowBlank : false
				}, {
					layout : 'form',
					border : false,
					items : [{
								fieldLabel : '是否为一般纳税人',
								xtype : 'radiogroup',
								name : 'generalTaxpayer',
								width : 210,
								allowBlank : false,
								items : [{
											name : 'generalTaxpayer',
											boxLabel : "是",
											inputValue : 0
										}, {
											name : 'generalTaxpayer',
											inputValue : 1,
											boxLabel : "否"
										}]
							}]

				}, {
					xtype : 'textarea',
					fieldLabel : '主导产品及产量',
					name : 'productsProduction',
					width : 220,
					allowBlank : false
				}, {
					xtype : 'textarea',
					fieldLabel : '采用标准',
					name : 'standard',
					width : 220,
					allowBlank : false
				}, {
					xtype : 'textarea',
					fieldLabel : '为中水科技公司配套产品',
					name : 'ancillaryProducts',
					width : 220,
					allowBlank : false
				}, {
					layout : 'form',
					border : false,
					items : [{
								fieldLabel : '是否引进线',
								xtype : 'radiogroup',
								name : 'introductionLine',
								width : 210,
								allowBlank : false,
								items : [{
											name : 'introductionLine',
											boxLabel : "是",
											inputValue : 0
										}, {
											name : 'introductionLine',
											inputValue : 1,
											boxLabel : "否"
										}]
							}]
				}, {
					xtype : 'textfield',
					fieldLabel : '成品率',
					name : 'Yield',
					width : 220,
					allowBlank : false
				}, {
					xtype : 'radiogroup',
					fieldLabel : '质量保证体系运行情况',
					name : 'qualityAssurance',
					width : 420,
					allowBlank : false,
					items : [{
								boxLabel : '良好',
								name : 'qualityAssurance',
								inputValue : 1
							}, {
								boxLabel : '一般',
								name : 'qualityAssurance',
								inputValue : 2
							}, {
								boxLabel : '差',
								inputValue : 3,
								name : 'qualityAssurance'
							}, {
								boxLabel : '无质量保证体系',
								inputValue : 4,
								name : 'qualityAssurance'
							}]
				}, {
					xtype : 'panel',
					layout : 'column',
					border : false,

					items : [{
								xtype : 'panel',
								layout : 'form',
								labelAlign : 'right',
								border : false,
								width : 200,
								labelAlign : 'right',
								items : [{
											fieldLabel : '登记人',
											name : 'newregisterName',
											xtype : 'displayfield'
										}, {
											fieldLabel : '最后修改人',
											name : 'lastModifierName',
											xtype : 'displayfield'
										}]
							}, {
								xtype : 'panel',
								layout : 'form',
								border : false,
								labelAlign : 'right',
								items : [{
											fieldLabel : '登记日期',
											xtype : 'displayfield',
											name : 'newupdateDate'
										}, {
											fieldLabel : '最后修改日期',
											xtype : 'displayfield',
											name : 'lastModified'
										}]
							}]
				}]
	});
	var comboxStroe = new Ext.data.JsonStore({
				url : __cxtPath + '/crmMaint.do?method=getRelationAddress',
				root : 'rows',
				fields : [{
							"name" : "公司ID",
							"mapping" : "公司ID"
						}, {
							"name" : "公司名称",
							"mapping" : "公司名称"
						}, {

							"name" : "地址ID",
							"mapping" : "地址ID"
						}, {

							"name" : "公司地址",
							"mapping" : "公司地址"
						}],

				baseParams : {
					selectCompanyID : selectCompanyID,
					limit : 20
				},
				autoLoad : true
			});

	var attFileGrid = new com.bitc.xmis.AttachmentList({
				region : 'east',
				title : '供方相关文件',
				border : false,
				stripeRows : true,
				width : 550,
				split : true,
				stateful : true,
				autoScroll : true,
				// stateId : 'editContractout-eastpanel',
				stateEvent : ['resize'],
				collapsible : true, // 是否有右上方展开收缩条
				// height:500,
				REFID : selectCompanyID == null ? 0 : selectCompanyID,
				attachmentTypeID : com.bitc.xmis.Attachment.supplier
			});
	// var s = new Ext.Panel({
	// region : 'east',
	// width : 550,
	// title : '',
	// border : false,
	// split : true,
	// stateful : true,
	// autoScroll : true,
	//				
	//				
	// items : [attFileGrid]
	// });
	/**
	 * 供方评定调查
	 */
	var supplierAssessmentSurvey = new Ext.Panel({
				title : '供方评定调查信息',
				layout : 'border',
				id : 't3',
				autoScroll : true,
				border : false,
				tbar : {
					id : 'supplier_survey',
					items : [{
						xtype : 'button',
						text : '保存',
						iconCls : 'ok',
						handler : function() {
							supplierInformation.getForm().submit({
								url : __cxtPath
										+ '/crmMaint.do?method=doSupplierInformation',
								params : {
									selectCompanyID : selectCompanyID
								},
								waitMsg : '正在保存，请稍候……',
								waitTitle : '正在处理',
								success : function(form, action) {
									Ext.Msg.alert(action.result.messageTitle,
											action.result.message);
									// location.reload();
									// tabs.setActiveTab('t3');
									reloadtab();

								},
								failure : function(form, action) {
									com.bitc.xmis.util.AlertUtil
											.formSubmitFailedAlert(action);
								}
							});
						}
					}, '-', {
						xtype : 'uploadButton',
						text : '附件上传',
						reuses : false,
						id : 'attButton',
						REFIDFun : function() {
							return selectCompanyID;
						},
						attachmentTypeID : com.bitc.xmis.Attachment.supplier,
						uploadPanelConfig : {
							listeners : {
								'uploadcomplete' : function() {
									attFileGrid.reload();
								}
							}
						}
					}]
				},
				items : [supplierInformation, attFileGrid]
			});

	var qualificationPanel = new Ext.FormPanel({
		region : 'center',
		padding : 20,
		autoScroll : true,
		border : false,
		labelWidth : 90,
		labelAlign : 'right',
		items : [{
					xtype : 'hidden',
					name : 'qualificationcompanyID',
					fieldLabel : 'ID',
					style : 'color:blue'
				}, {
					xtype : 'hidden',
					name : 'qualificationID'
				}, {
					xtype : 'hidden',
					name : 'id'
				}, {
					xtype : 'textfield',
					fieldLabel : '资质名称',
					name : 'qualificationName',
					width : 280,
					allowBlank : false
				}, {
					xtype : 'textfield',
					fieldLabel : '管理部门',
					name : 'departmentName',
					width : 280,
					allowBlank : false
				}, new Ext.ux.form.ChooseOrgTreeField({
					fieldLabel : '联系人',
					ref : 'qualificationChooseOrgTreeField',
					allowBlank : false,
					name : 'qualificationContact',
					id : 'qualificationContact',
					// value:1,
					width : 280,
					winConfig : {
						title : '选择联系人',
						tbar : [{
							ref : '../addBtn',
							iconCls : 'add',
							align : 'right',
							text : '新建',
							handler : function() {
								tabs.setActiveTab(1);
								qualifiedperson = 1;
								qualificationPanel.qualificationChooseOrgTreeField.window
										.hide()
							}
						}]
					},
					singleSelect : true,
					onlyPerson : true,
					rootConfig : function() {
						return {
							id : 'comp-old-' + selectCompanyID
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
								empName = empName + result[i].text + ','
							}
						}
						Ext.getCmp('qualificationContactID').setValue(empID
								.split('-')[2]);

						Ext.getCmp('qualificationContact').setValue(empName);

						Ext.Ajax.request({
							url : __cxtPath
									+ '/crmMaint.do?method=getDecisionMakersInformation',// 研究所
							params : {
								hrID : empID.split('-')[2]
							},
							scope : this,
							success : function(response) {
								var result = Ext.decode(response.responseText);
								if (result.success) {
									if (result.rows[0]) {
										qualificationPanel.getForm()
												.findField('DH')
												.setValue(result.rows[0].DH);
										qualificationPanel.getForm()
												.findField('CZ')
												.setRawValue(result.rows[0].CZ);
										qualificationPanel
												.getForm()
												.findField('Email')
												.setRawValue(result.rows[0].Email);
									}
								}
							},
							failure : function() {
							}
						});

					},
					clearValueList : 'qualificationContactID,qualificationContact',
					listeners : {
						'beforeShowWin' : function() {
							var aCmpId = selectCompanyID;
							if (!aCmpId || aCmpId == "") {
								Ext.Msg.alert("提示", "请先创建公司");
								return false;
							}
						},
						'nodedblclick' : function(node, e) {
							qualifiedperson = 2;
							tabs.setActiveTab(1);
							qualificationPanel.qualificationChooseOrgTreeField.window
									.hide();
							var path = String
									.format('/root{0}', node.getPath());

							if (!Ext.isEmpty(path)) {
								Ext.getCmp('t2').items.items[0].orgtree
										.expandPath(path);
								Ext.getCmp('t2').items.items[0].orgtree
										.selectPath(path, null, function(s, n) {
											if (s) {
												Ext.getCmp('t2').items.items[0].orgtree
														.fireEvent('click', n);
											}
										});
							}
						}
					},
					clearValueList : ''

				}), {
					xtype : 'displayfield',
					fieldLabel : '电话',
					name : 'DH',
					width : 220,
					allowBlank : false

				}, {
					xtype : 'displayfield',
					fieldLabel : '传真',
					name : 'CZ',
					width : 170,
					allowBlank : false

				}, {
					xtype : 'displayfield',
					fieldLabel : 'Email',
					name : 'Email',
					allowBlank : false

				}, {
					xtype : 'hidden',
					name : 'qualificationContactID',
					id : 'qualificationContactID'
				}, {
					xtype : 'datefield',
					fieldLabel : '证书有效期起',
					name : 'certificateValidQi',
					format : 'Y-m-d',
					width : 280,
					allowBlank : false
				}, {
					xtype : 'datefield',
					fieldLabel : '证书有效期止',
					format : 'Y-m-d',
					name : 'certificateValidZhi',
					width : 280,
					allowBlank : false
				}, {
					xtype : 'textfield',
					fieldLabel : '证书号',
					name : 'certificateNo',
					width : 280,
					allowBlank : false
				}, {
					xtype : 'textfield',
					fieldLabel : '使用频次',
					name : 'frequencyOfUse',
					width : 280,
					hidden : selectCompanyID == '1' ? false : true,
					value : 0
				}, {
					layout : 'form',
					border : false,
					items : [{
								fieldLabel : '年审',
								xtype : 'radiogroup',
								name : 'examined',
								width : 270,
								allowBlank : false,
								items : [{
											name : 'examined',
											boxLabel : "是",
											inputValue : 0
										}, {
											name : 'examined',
											inputValue : 1,
											boxLabel : "否"
										}]
							}]

				}, {
					xtype : 'textarea',
					fieldLabel : '备注',
					name : 'qualificationRemark',
					width : 280,
					allowBlank : false
				}, {
					xtype : 'panel',
					layout : 'column',
					border : false,

					items : [{
								xtype : 'panel',
								layout : 'form',
								labelAlign : 'right',
								border : false,
								width : 260,
								labelAlign : 'right',
								items : [{
											fieldLabel : '登记人',
											name : 'newregisterName',
											xtype : 'displayfield'
										}, {
											fieldLabel : '最后修改人',
											name : 'lastModifierName',
											xtype : 'displayfield'
										}]
							}, {
								xtype : 'panel',
								layout : 'form',
								border : false,
								labelAlign : 'right',
								items : [{
											fieldLabel : '登记日期',
											xtype : 'displayfield',
											name : 'newupdateDate'
										}, {
											fieldLabel : '最后修改日期',
											xtype : 'displayfield',
											name : 'lastModified'
										}]
							}]
				}]
	});
	var qualificationRecord = Ext.data.Record.create([{
				name : 'id'
			}, {
				name : 'qualificationName'
			}, {
				name : 'certificateValidQi'
			}, {
				name : 'certificateValidZhi'
			}]);
	var qualificationZiDianStore = new Ext.data.Store({
				url : __cxtPath + '/crmMaint.do?method=getQualificationList',
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount'
						}, qualificationRecord),
				remoteSort : true,
				baseParams : {
					contractId : selectCompanyID
				},
				autoLoad : true
			});
	var qualificationZiDianCM = new Ext.grid.ColumnModel([
			new Ext.grid.RowNumberer({
						width : 27
					}), {
				header : '资质名称',
				dataIndex : 'qualificationName',
				sortable : true,
				width : 140,
				listeners : {
					'click' : function(t, grid, rowIndex, e) {
						qualification.attButton1.setVisible(true);
						var r = grid.getStore().getAt(rowIndex);
						var contactPersonID = r.data.id;
						var qualificationAssignment = new Ext.data.JsonStore({
							url : __cxtPath
									+ '/crmMaint.do?method=getQualificationAssignment',
							root : 'rows',
							fields : [{
										name : 'id'
									}, {
										name : 'companyID'
									}, {
										name : 'qualificationContactID'
									}, {
										name : 'qualificationContact'
									}, {
										name : 'certificateValidQi'
									}, {
										name : 'certificateValidZhi'
									}, {
										name : 'certificateNo'
									}, {
										name : 'qualificationRemark'
									}, {
										name : 'examined'
									}, {
										name : 'qualificationName'
									}, {
										name : 'departmentName'
									}, {
										name : 'newregisterName'
									}, {
										name : 'lastModifierName'
									}, {
										name : 'lastModified'
									}, {
										name : 'newupdateDate'
									}, {
										name : 'dianHua'
									}, {
										name : 'youZhengBM'
									}, {
										name : 'Email'
									}, {
										name : 'TS'
									}, {
										name : 'DH'
									}, {
										name : 'CZ'
									}],
							baseParams : {
								ID : r.data.id
							},
							autoLoad : true
						});
						qualificationAssignment.on('load', function(store,
								record, opts) {
							qualificationPanel.getForm().clear();
							qualificationPanel.getForm().findField('DH')
									.setValue('');
							qualificationPanel.getForm().findField('CZ')
									.setValue('');
							qualificationPanel.getForm().findField('Email')
									.setValue('');
							qualificationPanel.getForm().findField('id')
									.setValue(record[0].data.id);
							qualificationPanel
									.getForm()
									.findField('qualificationContactID')
									.setValue(record[0].data.qualificationContactID);
							qualificationPanel
									.getForm()
									.findField('qualificationContact')
									.setValue(record[0].data.qualificationContact);
							qualificationPanel
									.getForm()
									.findField('certificateValidQi')
									.setValue((record[0].data.certificateValidQi)
											.substring(0, 10));
							qualificationPanel
									.getForm()
									.findField('certificateValidZhi')
									.setValue((record[0].data.certificateValidZhi)
											.substring(0, 10));
							qualificationPanel
									.getForm()
									.findField('qualificationRemark')
									.setValue(record[0].data.qualificationRemark);
							qualificationPanel.getForm().findField('examined')
									.setValue(record[0].data.examined);
							qualificationPanel.getForm()
									.findField('qualificationName')
									.setValue(record[0].data.qualificationName);
							qualificationPanel.getForm()
									.findField('departmentName')
									.setValue(record[0].data.departmentName);
							qualificationPanel.getForm()
									.findField('lastModifierName')
									.setValue(record[0].data.lastModifierName);
							qualificationPanel.getForm()
									.findField('newregisterName')
									.setValue(record[0].data.newregisterName);
							qualificationPanel.getForm()
									.findField('lastModified')
									.setValue((record[0].data.lastModified)
											.substring(0, 10));
							qualificationPanel.getForm()
									.findField('newupdateDate')
									.setValue((record[0].data.newupdateDate)
											.substring(0, 10));
							qualificationPanel.getForm()
									.findField('certificateNo')
									.setValue(record[0].data.certificateNo);
							qualificationPanel.getForm()
									.findField('frequencyOfUse')
									.setValue(record[0].data.TS);
							qualificationPanel.getForm().findField('DH')
									.setValue(record[0].data.DH);
							qualificationPanel.getForm().findField('CZ')
									.setValue(record[0].data.CZ);
							qualificationPanel.getForm().findField('Email')
									.setValue(record[0].data.Email);
							qualificationGrid.reload({
										params : {
											REFID : qualificationPanel
													.getForm().findField('id')
													.getValue()
										}
									});
								// ddddd = record[0].data.id;
						});
					}
				}
			}, {
				header : '有效期起',
				dataIndex : 'certificateValidQi',

				sortable : true,
				width : 140,
				renderer : function(value, cellmeta, record, rowIndex,
						columnIndex, store) {
					return value.substring(0, 10);
				}
			}, {
				header : '有效期止',
				dataIndex : 'certificateValidZhi',
				sortable : true,
				width : 140,
				renderer : function(value, cellmeta, record, rowIndex,
						columnIndex, store) {
					return value.substring(0, 10);
				}
			}]);
	var qualificationZiDian = new Ext.grid.GridPanel({
				id : 'qualificationZiDian',
				cm : qualificationZiDianCM,
				title : '资质列表',
				store : qualificationZiDianStore,
				stripeRows : true,
				split : true,
				stateful : true,
				stateId : 'editContractOut-relatedGrid',
				region : 'north',
				forceFit : true,
				loadMask : true,
				tbar : []
			});
	var projectQualificationRecord = Ext.data.Record.create([{
				name : 'id'
			}, {
				name : 'qualificationName'
			}, {
				name : 'qualificationDepartment'
			}, {
				name : 'qualificationProject'
			}, {
				name : 'qualificationApplicant'
			}, {
				name : 'applicantDate'
			}, {
				name : 'ZIID'
			}, {
				name : 'ZIID2'
			}]);
	var projectQualificationStore = new Ext.data.GroupingStore({
				groupField : 'qualificationName',
				url : __cxtPath
						+ '/crmMaint.do?method=getUseProjectQualification',
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount'
						}, projectQualificationRecord),
				remoteSort : true,
				baseParams : {
					contractId : selectCompanyID
				},
				autoLoad : true
			});
	var projectQualificationCM = new Ext.grid.ColumnModel([
			new Ext.grid.RowNumberer({
						width : 27
					}), {
				header : 'id',
				dataIndex : 'id',
				sortable : true,
				hidden : true
			}, {
				header : 'ZIID',
				dataIndex : 'ZIID',
				sortable : true,
				hidden : true
			}, {
				header : 'ZIID2',
				dataIndex : 'ZIID2',
				sortable : true,
				hidden : true
			}, {
				header : '资质名称',
				dataIndex : 'qualificationName',
				sortable : true,
				width : 140,
				listeners : {
					'click' : function(t, grid, rowIndex, e) {
						// qualification.attButton1.enable();

						var r = grid.getStore().getAt(rowIndex);
						var contactPersonID = r.data.id;
						var projectQualificationAssignment = new Ext.data.JsonStore(
								{
									url : __cxtPath
											+ '/crmMaint.do?method=getProjectQualificationAssignment',
									root : 'rows',
									fields : [{
												name : 'id'
											}, {
												name : 'applicationSectorsID'
											}, {
												name : 'applicationSectorsName'
											}, {
												name : 'ziID'
											}, {
												name : 'qualificationName'
											}, {
												name : 'projectID'
											}, {
												name : 'projectName'
											}, {
												name : 'applicantID'
											}, {
												name : 'applicant'
											}, {
												name : 'applicationDate'
											}, {
												name : 'applicationReasons'
											}, {
												name : 'applicationNewregisterName'
											}, {
												name : 'applicationLastModifierName'
											}, {
												name : 'applicationNewupdateDate'
											}, {
												name : 'applicationLastModified'
											}],
									baseParams : {
										id : r.data.id
									},
									autoLoad : true
								});
						projectQualificationAssignment.on('load', function(
								store, record, opts) {
							procurementWin.show();
							procurementForm.getForm().clear();

							procurementForm.getForm().findField('id')
									.setValue(record[0].data.id);
							procurementForm
									.getForm()
									.findField('applicationSectorsID')
									.setValue(record[0].data.applicationSectorsID);
							procurementForm
									.getForm()
									.findField('applicationSectorsName')
									.setValue(record[0].data.applicationSectorsName);
							procurementForm.getForm().findField('ziID')
									.setValue(record[0].data.ziID);
							procurementForm.getForm()
									.findField('qualificationName')
									.setValue(record[0].data.qualificationName);
							procurementForm.getForm().findField('projectID')
									.setValue(record[0].data.projectID);
							procurementForm.getForm().findField('projectName')
									.setValue(record[0].data.projectName);
							procurementForm.getForm().findField('applicantID')
									.setValue(record[0].data.applicantID);
							procurementForm.getForm().findField('applicant')
									.setValue(record[0].data.applicant);
							procurementForm.getForm()
									.findField('applicationDate')
									.setValue((record[0].data.applicationDate)
											.substring(0, 10));
							procurementForm
									.getForm()
									.findField('applicationReasons')
									.setValue(record[0].data.applicationReasons);
							procurementForm
									.getForm()
									.findField('applicationNewregisterName')
									.setValue(record[0].data.applicationNewregisterName);
							procurementForm
									.getForm()
									.findField('applicationLastModifierName')
									.setValue(record[0].data.applicationLastModifierName);
							procurementForm
									.getForm()
									.findField('applicationNewupdateDate')
									.setValue((record[0].data.applicationNewupdateDate)
											.substring(0, 10));
							procurementForm
									.getForm()
									.findField('applicationLastModified')
									.setValue((record[0].data.applicationLastModified)
											.substring(0, 10));

						});
					}
				}
			}, {
				header : '申请部门',
				dataIndex : 'qualificationDepartment',

				sortable : true,
				width : 140
			}, {
				header : '申请项目',
				dataIndex : 'qualificationProject',
				sortable : true,
				width : 140
			}, {
				header : '申请人',
				dataIndex : 'qualificationApplicant',
				sortable : true,
				width : 140
			}, {
				header : '申请日期',
				dataIndex : 'applicantDate',
				sortable : true,
				width : 140,
				renderer : function(value, cellmeta, record, rowIndex,
						columnIndex, store) {
					return value.substring(0, 10);
				}
			}]);
	var projectQualificationp = new Ext.grid.GridPanel({
		id : 'projectQualificationp',
		cm : projectQualificationCM,
		title : '资质使用频次列表',
		store : projectQualificationStore,
		view : new Ext.grid.GroupingView({
			forceFit : true,
			enableGroupingMenu : false,
			startCollapsed : true,
			groupTextTpl : '{text} ({[values.rs.length]} {[values.rs.length >= 1 ? "条" : "条"]})'// ?
		}),
		region : 'north'
			// tbar : []
	});

	var qualificationGrid = new com.bitc.xmis.AttachmentList({
				region : 'east',
				title : '资质相关文件',
				border : false,
				stripeRows : true,
				width : 550,
				// height:500,
				REFID : qualificationPanel.getForm().findField('id').getValue(),
				attachmentTypeID : com.bitc.xmis.Attachment.qualification
			});
	// var qualificationt = new Ext.Panel({
	// region : 'east',
	// width : 550,
	// title : '资质相关文件',
	// border : false,
	// split : true,
	// autoScroll : true,
	// collapsible : true, // 是否有右上方展开收缩条
	// items : [qualificationGrid]
	// });
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
		items : [qualificationZiDian, qualificationGrid]
			// qualificationZiDian,
		});
	// 资质管理
	var qualification = new Ext.Panel({
		title : '资质信息管理',
		layout : 'border',
		id : 't4',
		autoScroll : true,
		border : false,
		tbar : {
			id : 'quallification_t4',
			items : [{
				xtype : 'button',
				text : '保存',
				iconCls : 'ok',
				handler : function() {
					qualificationPanel.getForm().submit({
						url : __cxtPath
								+ '/crmMaint.do?method=doQualificationInformation',
						params : {
							selectCompanyID : selectCompanyID
						},
						waitMsg : '正在保存，请稍候……',
						waitTitle : '正在处理',
						success : function(form, action) {
							Ext.Msg.alert(action.result.messageTitle,
									action.result.message);
							// location.reload();
							qualificationPanel.getForm().findField('id')
									.setValue('');
							qualificationPanel.getForm().findField('examined')
									.setValue('');
							qualificationPanel.getForm()
									.findField('lastModifierName').setValue('');
							qualificationPanel.getForm()
									.findField('newregisterName').setValue('');
							qualificationPanel.getForm()
									.findField('lastModified').setValue('');
							qualificationPanel.getForm()
									.findField('newupdateDate').setValue('');
							qualificationPanel.getForm().findField('DH')
									.setValue('');
							qualificationPanel.getForm().findField('CZ')
									.setValue('');
							qualificationPanel.getForm().findField('Email')
									.setValue('');
							qualificationPanel.getForm().clear();
							Ext.getCmp('qualificationZiDian').getStore()
									.reload();
							qualification.attButton1.setVisible(false);
							qualificationGrid.reload({
										params : {
											REFID : ''
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
				xtype : 'uploadButton',
				text : '附件上传',
				reuses : false,
				ref : '../attButton1',
				// id : 'attButton1',
				hidden : true,
				REFIDFun : function() {
					eastPanel.setActiveTab(1);
					return qualificationPanel.getForm().findField('id')
							.getValue();
				},
				attachmentTypeID : com.bitc.xmis.Attachment.qualification,
				uploadPanelConfig : {
					listeners : {
						'uploadcomplete' : function() {
							qualificationGrid.reload();
						}
					}
				}
			}, '-', {
				xtype : 'button',
				text : '资质使用申请登记',
				iconCls : 'add',
				hidden : true,
				ref : '../addreButton1',
				handler : function() {
					// AddProcurementDetails(selectCompanyID);
					eastPanel.setActiveTab(2);
					procurementWin.show();
					procurementForm.getForm().clear();
					procurementForm.getForm().findField('id').setValue('');
					procurementForm.getForm().findField('applicationSectorsID')
							.setValue('');
					procurementForm.getForm().findField('ziID').setValue('');
					procurementForm.getForm().findField('projectID')
							.setValue('');
					procurementForm.getForm().findField('applicantID')
							.setValue('');
					procurementForm.getForm()
							.findField('applicationNewregisterName')
							.setValue('');
					procurementForm.getForm()
							.findField('applicationLastModifierName')
							.setValue('');
					procurementForm.getForm()
							.findField('applicationNewupdateDate').setValue('');
					procurementForm.getForm()
							.findField('applicationLastModified').setValue('');
					procurementForm.getForm()
							.findField('applicationLastModified').setValue('');
				}
			}, '-', {
				xtype : 'button',
				text : '清空',
				iconCls : 'remove',
				ref : '../addreButton',
				handler : function() {
					qualificationPanel.getForm().findField('id').setValue('');
					qualificationPanel.getForm().findField('examined')
							.setValue('');
					qualificationPanel.getForm().findField('lastModifierName')
							.setValue('');
					qualificationPanel.getForm().findField('newregisterName')
							.setValue('');
					qualificationPanel.getForm().findField('lastModified')
							.setValue('');
					qualificationPanel.getForm().findField('newupdateDate')
							.setValue('');
					qualificationPanel.getForm().findField('newupdateDate')
							.setValue('');
					qualificationPanel.getForm().findField('newupdateDate')
							.setValue('');
					qualificationPanel.getForm().findField('newupdateDate')
							.setValue('');
					qualificationPanel.getForm().findField('DH').setValue('');
					qualificationPanel.getForm().findField('CZ').setValue('');
					qualificationPanel.getForm().findField('Email')
							.setValue('');
					qualificationPanel.getForm().clear();
					qualification.attButton1.setVisible(false);
					qualificationGrid.reload({
								params : {
									REFID : ''
								}
							});
				}
			}]
		},
		items : [qualificationPanel, eastPanel]
	});
	// ---------------
	var patentPanel = new Ext.FormPanel({
		region : 'center',
		padding : 20,
		autoScroll : true,
		border : false,
		labelWidth : 90,
		labelAlign : 'right',
		items : [{
					xtype : 'hidden',
					name : 'companyID'
				}, {
					xtype : 'hidden',
					name : 'patentedDesignPeopleID1',
					id : 'patentedDesignPeopleID1'
				}, {
					xtype : 'hidden',
					name : 'patentedDesignPeopleID2',
					id : 'patentedDesignPeopleID2'
				}, {
					xtype : 'hidden',
					name : 'patentedDesignPeopleID3',
					id : 'patentedDesignPeopleID3'
				},
				// {
				// xtype : 'hidden',
				// name : 'patentedDesignPeopleID4',
				// id:'patentedDesignPeopleID4'
				// },{
				// xtype : 'hidden',
				// name : 'patentedDesignPeopleID5',
				// id:'patentedDesignPeopleID5'
				// },
				{
					xtype : 'hidden',
					name : 'sjid'
				}, {
					xtype : 'textfield',
					fieldLabel : '专利名称',
					name : 'patentName',
					width : 220,
					allowBlank : false
				}, {
					xtype : 'textfield',
					fieldLabel : '类别',
					name : 'patentsCategory',
					width : 220,
					allowBlank : false
				}, new Ext.ux.form.ChooseOrgTreeField({
					fieldLabel : '设计人',
					ref : 'patentedDesignPeopleChooseOrgTreeField1',
					allowBlank : false,
					name : 'patentedDesignPeople1',
					id : 'patentedDesignPeople1',
					// value:1,
					width : 220,
					winConfig : {
						title : '选择设计人',
						tbar : [{
							ref : '../addBtn',
							iconCls : 'add',
							align : 'right',
							text : '新建',
							handler : function() {
								patentHolder1 = 1;
								tabs.setActiveTab(1);
								patentPanel.patentedDesignPeopleChooseOrgTreeField1.window
										.hide()
							}
						}]
					},
					singleSelect : true,
					onlyPerson : true,
					rootConfig : function() {
						return {
							id : 'comp-old-' + selectCompanyID
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
								empName = empName + result[i].text + ','
							}
						}
						Ext.getCmp('patentedDesignPeopleID1').setValue(empID
								.split('-')[2]);

						Ext.getCmp('patentedDesignPeople1').setValue(empName);
					},
					clearValueList : 'patentedDesignPeopleID1,patentedDesignPeople1',
					listeners : {
						'beforeShowWin' : function() {
							var aCmpId = selectCompanyID;
							if (!aCmpId || aCmpId == "") {
								Ext.Msg.alert("提示", "请先创建公司");
								return false;
							}
						},
						'dblclick' : function(select, num) {

							var empId = Ext.getCmp('patentedDesignPeopleID1')
									.getValue();
							com.bitc.xmis.util.EmpInfoWin(empId, '经理信息');
						},
						'render' : function(c) {
							Ext.QuickTips.register({
										target : c.getEl(),
										text : '双击可显示详细信息'
									});
						},
						'nodedblclick' : function(node, e) {
							// qualifiedperson = 2;
							// tabs.setActiveTab(1);
							patentPanel.patentedDesignPeopleChooseOrgTreeField1.window
									.hide();
							var path = String
									.format('/root{0}', node.getPath());

							if (!Ext.isEmpty(path)) {
								Ext.getCmp('t2').items.items[0].orgtree
										.expandPath(path);
								Ext.getCmp('t2').items.items[0].orgtree
										.selectPath(path, null, function(s, n) {
											if (s) {
												Ext.getCmp('t2').items.items[0].orgtree
														.fireEvent('click', n);
											}
										});
							}
						}
					},
					clearValueList : ''

				}), new Ext.ux.form.ChooseOrgTreeField({
					// fieldLabel : ' ',
					ref : 'patentedDesignPeopleChooseOrgTreeField2',
					// allowBlank : false,
					name : 'patentedDesignPeople2',
					id : 'patentedDesignPeople2',
					// value:1,
					width : 220,
					winConfig : {
						title : '选择设计人',
						tbar : [{
							ref : '../addBtn',
							iconCls : 'add',
							align : 'right',
							text : '新建',
							handler : function() {
								patentHolder2 = 1;
								tabs.setActiveTab(1);
								patentPanel.patentedDesignPeopleChooseOrgTreeField2.window
										.hide()
							}
						}]
					},
					singleSelect : true,
					onlyPerson : true,
					rootConfig : function() {
						return {
							id : 'comp-old-' + selectCompanyID
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
								empName = empName + result[i].text + ','
							}
						}
						Ext.getCmp('patentedDesignPeopleID2').setValue(empID
								.split('-')[2]);

						Ext.getCmp('patentedDesignPeople2').setValue(empName);
					},
					clearValueList : 'patentedDesignPeopleID2,patentedDesignPeople2',
					listeners : {
						'beforeShowWin' : function() {
							var aCmpId = selectCompanyID;
							if (!aCmpId || aCmpId == "") {
								Ext.Msg.alert("提示", "请先创建公司");
								return false;
							}
						},
						'dblclick' : function(select, num) {

							var empId = Ext.getCmp('patentedDesignPeopleID2')
									.getValue();
							com.bitc.xmis.util.EmpInfoWin(empId, '经理信息');
						},
						'render' : function(c) {
							Ext.QuickTips.register({
										target : c.getEl(),
										text : '双击可显示详细信息'
									});
						},
						'nodedblclick' : function(node, e) {
							// qualifiedperson = 2;
							// tabs.setActiveTab(1);
							patentPanel.patentedDesignPeopleChooseOrgTreeField2.window
									.hide();
							var path = String
									.format('/root{0}', node.getPath());

							if (!Ext.isEmpty(path)) {
								Ext.getCmp('t2').items.items[0].orgtree
										.expandPath(path);
								Ext.getCmp('t2').items.items[0].orgtree
										.selectPath(path, null, function(s, n) {
											if (s) {
												Ext.getCmp('t2').items.items[0].orgtree
														.fireEvent('click', n);
											}
										});
							}
						}
					},
					clearValueList : ''

				}), new Ext.ux.form.ChooseOrgTreeField({
					// fieldLabel : '设计人3',
					ref : 'patentedDesignPeopleChooseOrgTreeField3',
					// allowBlank : false,
					name : 'patentedDesignPeople3',
					id : 'patentedDesignPeople3',
					// value:1,
					width : 220,
					winConfig : {
						title : '选择设计人',
						tbar : [{
							ref : '../addBtn',
							iconCls : 'add',
							align : 'right',
							text : '新建',
							handler : function() {
								patentHolder3 = 1;
								tabs.setActiveTab(1);
								patentPanel.patentedDesignPeopleChooseOrgTreeField3.window
										.hide()
							}
						}]
					},
					singleSelect : true,
					onlyPerson : true,
					rootConfig : function() {
						return {
							id : 'comp-old-' + selectCompanyID
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
								empName = empName + result[i].text + ','
							}
						}
						Ext.getCmp('patentedDesignPeopleID3').setValue(empID
								.split('-')[2]);

						Ext.getCmp('patentedDesignPeople3').setValue(empName);
					},
					clearValueList : 'patentedDesignPeopleID3,patentedDesignPeople3',
					listeners : {
						'beforeShowWin' : function() {
							var aCmpId = selectCompanyID;
							if (!aCmpId || aCmpId == "") {
								Ext.Msg.alert("提示", "请先创建公司");
								return false;
							}
						},
						'dblclick' : function(select, num) {

							var empId = Ext.getCmp('patentedDesignPeopleID3')
									.getValue();
							com.bitc.xmis.util.EmpInfoWin(empId, '经理信息');
						},
						'render' : function(c) {
							Ext.QuickTips.register({
										target : c.getEl(),
										text : '双击可显示详细信息'
									});
						},
						'nodedblclick' : function(node, e) {
							// qualifiedperson = 2;
							// tabs.setActiveTab(1);
							patentPanel.patentedDesignPeopleChooseOrgTreeField3.window
									.hide();
							var path = String
									.format('/root{0}', node.getPath());

							if (!Ext.isEmpty(path)) {
								Ext.getCmp('t2').items.items[0].orgtree
										.expandPath(path);
								Ext.getCmp('t2').items.items[0].orgtree
										.selectPath(path, null, function(s, n) {
											if (s) {
												Ext.getCmp('t2').items.items[0].orgtree
														.fireEvent('click', n);
											}
										});
							}
						}
					},
					clearValueList : ''

				}),
				// new Ext.ux.form.ChooseOrgTreeField({
				// fieldLabel : '设计人4',
				// ref : 'patentedDesignPeopleChooseOrgTreeField4',
				// allowBlank : false,
				// name : 'patentedDesignPeople4',
				// id : 'patentedDesignPeople4',
				// // value:1,
				// width : 220,
				// winConfig : {
				// title : '选择设计人',
				// tbar : [{
				// ref : '../addBtn',
				// iconCls : 'add',
				// align : 'right',
				// text : '新建',
				// handler : function() {
				// patentHolder4 = 1;
				// tabs.setActiveTab(1);
				// patentPanel.patentedDesignPeopleChooseOrgTreeField4.window
				// .hide()
				// }
				// }]
				// },
				// singleSelect : true,
				// onlyPerson : true,
				// rootConfig : function() {
				// return {
				// id : 'comp-old-' + selectCompanyID
				// }
				// },
				// dataUrl : '/orgMaint.do?method=getOrganizationTree',
				// setValueFun : function(result) {
				// var empName = '';
				// var empID = '';
				// for (var i = 0; i < result.length; i++) {
				// if (i == result.length - 1) {
				// empID = empID + result[i].id;
				// empName = empName + result[i].text;
				// } else {
				// empID = empID + result[i].id + ',';
				// empName = empName + result[i].text + ','
				// }
				// }
				// Ext.getCmp('patentedDesignPeopleID4').setValue(empID.split('-')[2]);
				//			
				// Ext.getCmp('patentedDesignPeople4').setValue(empName);
				// },
				// clearValueList :
				// 'patentedDesignPeopleID4,patentedDesignPeople4',
				// listeners : {
				// 'beforeShowWin' : function() {
				// var aCmpId = selectCompanyID;
				// if (!aCmpId || aCmpId == "") {
				// Ext.Msg.alert("提示", "请先创建公司");
				// return false;
				// }
				// },
				// 'dblclick' : function(select, num) {
				//			
				// var empId = Ext.getCmp('patentedDesignPeopleID4').getValue();
				// com.bitc.xmis.util.EmpInfoWin(empId, '经理信息');
				// },
				// 'render' : function(c) {
				// Ext.QuickTips.register({
				// target : c.getEl(),
				// text : '双击可显示详细信息'
				// });
				// },
				// 'nodedblclick' : function(node, e) {
				// // qualifiedperson = 2;
				// // tabs.setActiveTab(1);
				// patentPanel.patentedDesignPeopleChooseOrgTreeField4.window
				// .hide();
				// var path = String
				// .format('/root{0}', node.getPath());
				//			
				// if (!Ext.isEmpty(path)) {
				// Ext.getCmp('t2').items.items[0].orgtree
				// .expandPath(path);
				// Ext.getCmp('t2').items.items[0].orgtree
				// .selectPath(path, null, function(s, n) {
				// if (s) {
				// Ext.getCmp('t2').items.items[0].orgtree
				// .fireEvent('click', n);
				// }
				// });
				// }
				// }
				// },
				// clearValueList : ''
				//			
				// }),new Ext.ux.form.ChooseOrgTreeField({
				// fieldLabel : '设计人5',
				// ref : 'patentedDesignPeopleChooseOrgTreeField5',
				// allowBlank : false,
				// name : 'patentedDesignPeople5',
				// id : 'patentedDesignPeople5',
				// // value:1,
				// width : 220,
				// winConfig : {
				// title : '选择设计人',
				// tbar : [{
				// ref : '../addBtn',
				// iconCls : 'add',
				// align : 'right',
				// text : '新建',
				// handler : function() {
				// patentHolder5 = 1;
				// tabs.setActiveTab(1);
				// patentPanel.patentedDesignPeopleChooseOrgTreeField5.window
				// .hide()
				// }
				// }]
				// },
				// singleSelect : true,
				// onlyPerson : true,
				// rootConfig : function() {
				// return {
				// id : 'comp-old-' + selectCompanyID
				// }
				// },
				// dataUrl : '/orgMaint.do?method=getOrganizationTree',
				// setValueFun : function(result) {
				// var empName = '';
				// var empID = '';
				// for (var i = 0; i < result.length; i++) {
				// if (i == result.length - 1) {
				// empID = empID + result[i].id;
				// empName = empName + result[i].text;
				// } else {
				// empID = empID + result[i].id + ',';
				// empName = empName + result[i].text + ','
				// }
				// }
				// Ext.getCmp('patentedDesignPeopleID5').setValue(empID.split('-')[2]);
				//			
				// Ext.getCmp('patentedDesignPeople5').setValue(empName);
				// },
				// clearValueList :
				// 'patentedDesignPeopleID5,patentedDesignPeople5',
				// listeners : {
				// 'beforeShowWin' : function() {
				// var aCmpId = selectCompanyID;
				// if (!aCmpId || aCmpId == "") {
				// Ext.Msg.alert("提示", "请先创建公司");
				// return false;
				// }
				// },
				// 'dblclick' : function(select, num) {
				//			
				// var empId = Ext.getCmp('patentedDesignPeopleID5').getValue();
				// com.bitc.xmis.util.EmpInfoWin(empId, '经理信息');
				// },
				// 'render' : function(c) {
				// Ext.QuickTips.register({
				// target : c.getEl(),
				// text : '双击可显示详细信息'
				// });
				// },
				// 'nodedblclick' : function(node, e) {
				// // qualifiedperson = 2;
				// // tabs.setActiveTab(1);
				// patentPanel.patentedDesignPeopleChooseOrgTreeField5.window
				// .hide();
				// var path = String
				// .format('/root{0}', node.getPath());
				//			
				// if (!Ext.isEmpty(path)) {
				// Ext.getCmp('t2').items.items[0].orgtree
				// .expandPath(path);
				// Ext.getCmp('t2').items.items[0].orgtree
				// .selectPath(path, null, function(s, n) {
				// if (s) {
				// Ext.getCmp('t2').items.items[0].orgtree
				// .fireEvent('click', n);
				// }
				// });
				// }
				// }
				// },
				// clearValueList : ''
				//			
				// }),
				new Ext.ux.form.ChooseOrgTreeField({
					fieldLabel : '专利权人',
					ref : 'patentRightPeopleChooseOrgTreeField1',
					allowBlank : false,
					name : 'patentRightPeople1',
					id : 'patentRightPeople1',
					// value:1,
					width : 220,
					winConfig : {
						title : '选择设计人',
						tbar : [{
							ref : '../addBtn',
							iconCls : 'add',
							align : 'right',
							text : '新建',
							handler : function() {
								tabs.setActiveTab(1);
								patentRight1 = 1;
								patentPanel.patentRightPeopleChooseOrgTreeField1.window
										.hide()
							}
						}]
					},
					singleSelect : true,
					onlyPerson : true,
					rootConfig : function() {
						return {
							id : 'comp-old-' + selectCompanyID
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
								empName = empName + result[i].text + ','
							}
						}
						Ext.getCmp('patentRightPeopleID1').setValue(empID
								.split('-')[2]);
						Ext.getCmp('patentRightPeople1').setValue(empName);
					},
					clearValueList : 'patentRightPeopleID1,patentRightPeople1',
					listeners : {
						'beforeShowWin' : function() {
							var aCmpId = selectCompanyID;
							if (!aCmpId || aCmpId == "") {
								Ext.Msg.alert("提示", "请先创建公司");
								return false;
							}
						},
						'dblclick' : function(select, num) {

							var empId = Ext.getCmp('patentRightPeopleID1')
									.getValue();
							com.bitc.xmis.util.EmpInfoWin(empId, '经理信息');
						},
						'render' : function(c) {
							Ext.QuickTips.register({
										target : c.getEl(),
										text : '双击可显示详细信息'
									});
						},
						'nodedblclick' : function(node, e) {
							// qualifiedperson = 2;
							// tabs.setActiveTab(1);
							patentPanel.patentRightPeopleChooseOrgTreeField1.window
									.hide();
							var path = String
									.format('/root{0}', node.getPath());

							if (!Ext.isEmpty(path)) {
								Ext.getCmp('t2').items.items[0].orgtree
										.expandPath(path);
								Ext.getCmp('t2').items.items[0].orgtree
										.selectPath(path, null, function(s, n) {
											if (s) {
												Ext.getCmp('t2').items.items[0].orgtree
														.fireEvent('click', n);
											}
										});
							}
						}
					},
					clearValueList : ''

				}), new Ext.ux.form.ChooseOrgTreeField({
					// fieldLabel : '专利权人2',
					ref : 'patentRightPeopleChooseOrgTreeField2',
					// allowBlank : false,
					name : 'patentRightPeople2',
					id : 'patentRightPeople2',
					// value:1,
					width : 220,
					winConfig : {
						title : '选择设计人',
						tbar : [{
							ref : '../addBtn',
							iconCls : 'add',
							align : 'right',
							text : '新建',
							handler : function() {
								tabs.setActiveTab(1);
								patentRight2 = 1;
								patentPanel.patentRightPeopleChooseOrgTreeField2.window
										.hide()
							}
						}]
					},
					singleSelect : true,
					onlyPerson : true,
					rootConfig : function() {
						return {
							id : 'comp-old-' + selectCompanyID
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
								empName = empName + result[i].text + ','
							}
						}
						Ext.getCmp('patentRightPeopleID2').setValue(empID
								.split('-')[2]);
						Ext.getCmp('patentRightPeople2').setValue(empName);
					},
					clearValueList : 'patentRightPeopleID2,patentRightPeople2',
					listeners : {
						'beforeShowWin' : function() {
							var aCmpId = selectCompanyID;
							if (!aCmpId || aCmpId == "") {
								Ext.Msg.alert("提示", "请先创建公司");
								return false;
							}
						},
						'dblclick' : function(select, num) {

							var empId = Ext.getCmp('patentRightPeopleID2')
									.getValue();
							com.bitc.xmis.util.EmpInfoWin(empId, '经理信息');
						},
						'render' : function(c) {
							Ext.QuickTips.register({
										target : c.getEl(),
										text : '双击可显示详细信息'
									});
						},
						'nodedblclick' : function(node, e) {
							// qualifiedperson = 2;
							// tabs.setActiveTab(1);
							patentPanel.patentRightPeopleChooseOrgTreeField2.window
									.hide();
							var path = String
									.format('/root{0}', node.getPath());

							if (!Ext.isEmpty(path)) {
								Ext.getCmp('t2').items.items[0].orgtree
										.expandPath(path);
								Ext.getCmp('t2').items.items[0].orgtree
										.selectPath(path, null, function(s, n) {
											if (s) {
												Ext.getCmp('t2').items.items[0].orgtree
														.fireEvent('click', n);
											}
										});
							}
						}
					},
					clearValueList : ''

				}), new Ext.ux.form.ChooseOrgTreeField({
					// fieldLabel : '专利权人3',
					ref : 'patentRightPeopleChooseOrgTreeField3',
					// allowBlank : false,
					name : 'patentRightPeople3',
					id : 'patentRightPeople3',
					// value:1,
					width : 220,
					winConfig : {
						title : '选择设计人',
						tbar : [{
							ref : '../addBtn',
							iconCls : 'add',
							align : 'right',
							text : '新建',
							handler : function() {
								tabs.setActiveTab(1);
								patentRight3 = 1;
								patentPanel.patentRightPeopleChooseOrgTreeField3.window
										.hide()
							}
						}]
					},
					singleSelect : true,
					onlyPerson : true,
					rootConfig : function() {
						return {
							id : 'comp-old-' + selectCompanyID
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
								empName = empName + result[i].text + ','
							}
						}
						Ext.getCmp('patentRightPeopleID3').setValue(empID
								.split('-')[2]);
						Ext.getCmp('patentRightPeople3').setValue(empName);
					},
					clearValueList : 'patentRightPeopleID3,patentRightPeople3',
					listeners : {
						'beforeShowWin' : function() {
							var aCmpId = selectCompanyID;
							if (!aCmpId || aCmpId == "") {
								Ext.Msg.alert("提示", "请先创建公司");
								return false;
							}
						},
						'dblclick' : function(select, num) {

							var empId = Ext.getCmp('patentRightPeopleID3')
									.getValue();
							com.bitc.xmis.util.EmpInfoWin(empId, '经理信息');
						},
						'render' : function(c) {
							Ext.QuickTips.register({
										target : c.getEl(),
										text : '双击可显示详细信息'
									});
						},
						'nodedblclick' : function(node, e) {
							// qualifiedperson = 2;
							// tabs.setActiveTab(1);
							patentPanel.patentRightPeopleChooseOrgTreeField3.window
									.hide();
							var path = String
									.format('/root{0}', node.getPath());

							if (!Ext.isEmpty(path)) {
								Ext.getCmp('t2').items.items[0].orgtree
										.expandPath(path);
								Ext.getCmp('t2').items.items[0].orgtree
										.selectPath(path, null, function(s, n) {
											if (s) {
												Ext.getCmp('t2').items.items[0].orgtree
														.fireEvent('click', n);
											}
										});
							}
						}
					},
					clearValueList : ''

				}), new Ext.ux.form.ChooseOrgTreeField({
					// fieldLabel : '专利权人4',
					ref : 'patentRightPeopleChooseOrgTreeField4',
					// allowBlank : false,
					name : 'patentRightPeople4',
					id : 'patentRightPeople4',
					// value:1,
					width : 220,
					winConfig : {
						title : '选择设计人',
						tbar : [{
							ref : '../addBtn',
							iconCls : 'add',
							align : 'right',
							text : '新建',
							handler : function() {
								tabs.setActiveTab(1);
								patentRight4 = 1;
								patentPanel.patentRightPeopleChooseOrgTreeField4.window
										.hide()
							}
						}]
					},
					singleSelect : true,
					onlyPerson : true,
					rootConfig : function() {
						return {
							id : 'comp-old-' + selectCompanyID
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
								empName = empName + result[i].text + ','
							}
						}
						Ext.getCmp('patentRightPeopleID4').setValue(empID
								.split('-')[2]);
						Ext.getCmp('patentRightPeople4').setValue(empName);
					},
					clearValueList : 'patentRightPeopleID4,patentRightPeople4',
					listeners : {
						'beforeShowWin' : function() {
							var aCmpId = selectCompanyID;
							if (!aCmpId || aCmpId == "") {
								Ext.Msg.alert("提示", "请先创建公司");
								return false;
							}
						},
						'dblclick' : function(select, num) {

							var empId = Ext.getCmp('patentRightPeopleID4')
									.getValue();
							com.bitc.xmis.util.EmpInfoWin(empId, '经理信息');
						},
						'render' : function(c) {
							Ext.QuickTips.register({
										target : c.getEl(),
										text : '双击可显示详细信息'
									});
						},
						'nodedblclick' : function(node, e) {
							// qualifiedperson = 2;
							// tabs.setActiveTab(1);
							patentPanel.patentRightPeopleChooseOrgTreeField4.window
									.hide();
							var path = String
									.format('/root{0}', node.getPath());

							if (!Ext.isEmpty(path)) {
								Ext.getCmp('t2').items.items[0].orgtree
										.expandPath(path);
								Ext.getCmp('t2').items.items[0].orgtree
										.selectPath(path, null, function(s, n) {
											if (s) {
												Ext.getCmp('t2').items.items[0].orgtree
														.fireEvent('click', n);
											}
										});
							}
						}
					},
					clearValueList : ''

				}), new Ext.ux.form.ChooseOrgTreeField({
					// fieldLabel : '专利权人',
					ref : 'patentRightPeopleChooseOrgTreeField5',
					// allowBlank : false,
					name : 'patentRightPeople5',
					id : 'patentRightPeople5',
					// value:1,
					width : 220,
					winConfig : {
						title : '选择设计人',
						tbar : [{
							ref : '../addBtn',
							iconCls : 'add',
							align : 'right',
							text : '新建',
							handler : function() {
								tabs.setActiveTab(1);
								patentRight5 = 1;
								patentPanel.patentRightPeopleChooseOrgTreeField5.window
										.hide()
							}
						}]
					},
					singleSelect : true,
					onlyPerson : true,
					rootConfig : function() {
						return {
							id : 'comp-old-' + selectCompanyID
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
								empName = empName + result[i].text + ','
							}
						}
						Ext.getCmp('patentRightPeopleID5').setValue(empID
								.split('-')[2]);
						Ext.getCmp('patentRightPeople5').setValue(empName);
					},
					clearValueList : 'patentRightPeopleID5,patentRightPeople5',
					listeners : {
						'beforeShowWin' : function() {
							var aCmpId = selectCompanyID;
							if (!aCmpId || aCmpId == "") {
								Ext.Msg.alert("提示", "请先创建公司");
								return false;
							}
						},
						'dblclick' : function(select, num) {

							var empId = Ext.getCmp('patentRightPeopleID5')
									.getValue();
							com.bitc.xmis.util.EmpInfoWin(empId, '经理信息');
						},
						'render' : function(c) {
							Ext.QuickTips.register({
										target : c.getEl(),
										text : '双击可显示详细信息'
									});
						},
						'nodedblclick' : function(node, e) {
							// qualifiedperson = 2;
							// tabs.setActiveTab(1);
							patentPanel.patentRightPeopleChooseOrgTreeField5.window
									.hide();
							var path = String
									.format('/root{0}', node.getPath());

							if (!Ext.isEmpty(path)) {
								Ext.getCmp('t2').items.items[0].orgtree
										.expandPath(path);
								Ext.getCmp('t2').items.items[0].orgtree
										.selectPath(path, null, function(s, n) {
											if (s) {
												Ext.getCmp('t2').items.items[0].orgtree
														.fireEvent('click', n);
											}
										});
							}
						}
					},
					clearValueList : ''

				}), {
					xtype : 'textfield',
					fieldLabel : '专利号',
					name : 'patentNumber',
					width : 220,
					allowBlank : false
				}, {
					xtype : 'textfield',
					fieldLabel : '证书号',
					name : 'patentCertificateNo',
					width : 220,
					allowBlank : false
				}, {
					xtype : 'hidden',
					name : 'patentRightPeopleID1',
					id : 'patentRightPeopleID1'
				}, {
					xtype : 'hidden',
					name : 'patentRightPeopleID2',
					id : 'patentRightPeopleID2'
				}, {
					xtype : 'hidden',
					name : 'patentRightPeopleID3',
					id : 'patentRightPeopleID3'
				}, {
					xtype : 'hidden',
					name : 'patentRightPeopleID4',
					id : 'patentRightPeopleID4'
				}, {
					xtype : 'hidden',
					name : 'patentRightPeopleID5',
					id : 'patentRightPeopleID5'
				}, {
					xtype : 'datefield',
					fieldLabel : '申请日',
					name : 'applicationTime',
					format : 'Y-m-d',
					width : 220,
					allowBlank : false
				}, {
					xtype : 'datefield',
					fieldLabel : '授权公告日',
					format : 'Y-m-d',
					name : 'authorizeTime',
					width : 220,
					allowBlank : false
				}, {
					xtype : 'datefield',
					fieldLabel : '有效期起',
					name : 'validFrom',
					format : 'Y-m-d',
					width : 220,
					allowBlank : false
				}, {
					xtype : 'datefield',
					fieldLabel : '有效期止',
					name : 'validOnly',
					format : 'Y-m-d',
					width : 220,
					allowBlank : false
				}, {
					xtype : 'panel',
					layout : 'column',
					border : false,
					items : [{
								xtype : 'panel',
								layout : 'form',
								labelAlign : 'right',
								border : false,
								width : 200,
								labelAlign : 'right',
								items : [{
											fieldLabel : '登记人',
											name : 'patentregisterName',
											xtype : 'displayfield'
										}, {
											fieldLabel : '最后修改人',
											name : 'patentlastModifierName',
											xtype : 'displayfield'
										}]
							}, {
								xtype : 'panel',
								layout : 'form',
								border : false,
								labelAlign : 'right',
								items : [{
											fieldLabel : '登记日期',
											xtype : 'displayfield',
											name : 'patentupdateDate'
										}, {
											fieldLabel : '最后修改日期',
											xtype : 'displayfield',
											name : 'patentlastModified'
										}]
							}]
				}]
	});

	var patentGrid = new com.bitc.xmis.AttachmentList({
				region : 'east',
				title : '专利相关文件',
				border : false,
				stripeRows : true,
				width : 550,
				// height:500,
				REFID : patentPanel.getForm().findField('sjid').getValue(),
				attachmentTypeID : com.bitc.xmis.Attachment.patentDocuments
			});

	var patentListreRecordGYS = Ext.data.Record.create([{
				name : 'sjid'
			}, {
				name : 'companyID'
			}, {
				name : 'patentName'
			}, {
				name : 'patentedDesignPeople'
			}, {
				name : 'patentRightPeople'
			}, {
				name : 'validFrom'
			}, {
				name : 'validOnly'
			}]);

	var patentListStore = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : __cxtPath
									+ '/crmMaint.do?method=getPatentInformationList',
							method : "POST"
						}),
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount'
						}, patentListreRecordGYS),
				remoteSort : true,
				baseParams : {
					selectCompanyID : selectCompanyID,
					start : 0,
					limit : 20
				},
				autoLoad : true
			});

	var patentListCM = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({
						width : 25
					}), {
				header : '专利名称',
				dataIndex : 'patentName',// +','+'patentedDesignPeople2'+','+'patentedDesignPeople3'
				sortable : true,
				width : 200,
				listeners : {
					'click' : function(t, grid, rowIndex, e) {
						patentInformation.attButtont.setVisible(true);
						var r = grid.getStore().getAt(rowIndex);
						var contactPersonID = r.data.sjid;
						var patentsAssignment = new Ext.data.JsonStore({
							url : __cxtPath
									+ '/crmMaint.do?method=getPatentAssignment',
							root : 'rows',
							fields : [{
										name : 'sjid'
									}, {
										name : 'companyID'
									}, {
										name : 'patentsCategory'
									}, {
										name : 'patentName'
									}, {
										name : 'patentedDesignPeopleID1'
									}, {
										name : 'patentedDesignPeopleID2'
									}, {
										name : 'patentedDesignPeopleID3'
									}, {
										name : 'patentRightPeopleID1'
									}, {
										name : 'patentRightPeopleID2'
									}, {
										name : 'patentRightPeopleID3'
									}, {
										name : 'patentRightPeopleID4'
									}, {
										name : 'patentRightPeopleID5'
									}, {
										name : 'patentedDesignPeople1'
									}, {
										name : 'patentedDesignPeople2'
									}, {
										name : 'patentedDesignPeople3'
									}, {
										name : 'patentRightPeople1'
									}, {
										name : 'patentRightPeople2'
									}, {
										name : 'patentRightPeople3'
									}, {
										name : 'patentRightPeople4'
									}, {
										name : 'patentRightPeople5'
									}, {
										name : 'patentNumber'
									}, {
										name : 'patentCertificateNo'
									}, {
										name : 'applicationTime'
									}, {
										name : 'authorizeTime'
									}, {
										name : 'patentregisterName'
									}, {
										name : 'patentlastModifierName'
									}, {
										name : 'patentupdateDate'
									}, {
										name : 'patentlastModified'
									}, {
										name : 'validFrom'
									}, {
										name : 'validOnly'
									}],
							baseParams : {
								sjid : r.data.sjid
							},
							autoLoad : true
						});
						patentsAssignment.on('load', function(store, record,
								opts) {
							patentPanel.getForm().clear();

							patentPanel.getForm().findField('sjid')
									.setValue(record[0].data.sjid);
							patentPanel.getForm().findField('companyID')
									.setValue(record[0].data.companyID);
							patentPanel.getForm().findField('patentName')
									.setValue(record[0].data.patentName);
							patentPanel.getForm().findField('patentsCategory')
									.setValue(record[0].data.patentsCategory);
							patentPanel
									.getForm()
									.findField('patentedDesignPeopleID1')
									.setValue(record[0].data.patentedDesignPeopleID1);
							patentPanel
									.getForm()
									.findField('patentedDesignPeople1')
									.setValue(record[0].data.patentedDesignPeople1);
							patentPanel
									.getForm()
									.findField('patentedDesignPeopleID2')
									.setValue(record[0].data.patentedDesignPeopleID2);
							patentPanel
									.getForm()
									.findField('patentedDesignPeople2')
									.setValue(record[0].data.patentedDesignPeople2);
							patentPanel
									.getForm()
									.findField('patentedDesignPeopleID3')
									.setValue(record[0].data.patentedDesignPeopleID3);
							patentPanel
									.getForm()
									.findField('patentedDesignPeople3')
									.setValue(record[0].data.patentedDesignPeople3);
							patentPanel
									.getForm()
									.findField('patentRightPeopleID1')
									.setValue(record[0].data.patentRightPeopleID1);
							patentPanel
									.getForm()
									.findField('patentRightPeople1')
									.setValue(record[0].data.patentRightPeople1);
							patentPanel
									.getForm()
									.findField('patentRightPeopleID2')
									.setValue(record[0].data.patentRightPeopleID2);
							patentPanel
									.getForm()
									.findField('patentRightPeople2')
									.setValue(record[0].data.patentRightPeople2);
							patentPanel
									.getForm()
									.findField('patentRightPeopleID3')
									.setValue(record[0].data.patentRightPeopleID3);
							patentPanel
									.getForm()
									.findField('patentRightPeople3')
									.setValue(record[0].data.patentRightPeople3);
							patentPanel
									.getForm()
									.findField('patentRightPeopleID4')
									.setValue(record[0].data.patentRightPeopleID4);
							patentPanel
									.getForm()
									.findField('patentRightPeople4')
									.setValue(record[0].data.patentRightPeople4);
							patentPanel
									.getForm()
									.findField('patentRightPeopleID5')
									.setValue(record[0].data.patentRightPeopleID5);
							patentPanel
									.getForm()
									.findField('patentRightPeople5')
									.setValue(record[0].data.patentRightPeople5);
							patentPanel.getForm().findField('patentNumber')
									.setValue(record[0].data.patentNumber);
							patentPanel
									.getForm()
									.findField('patentCertificateNo')
									.setValue(record[0].data.patentCertificateNo);
							patentPanel.getForm().findField('applicationTime')
									.setValue((record[0].data.applicationTime)
											.substring(0, 10));
							patentPanel.getForm().findField('authorizeTime')
									.setValue((record[0].data.authorizeTime)
											.substring(0, 10));
							patentPanel.getForm().findField('validFrom')
									.setValue((record[0].data.validFrom)
											.substring(0, 10));
							patentPanel.getForm().findField('validOnly')
									.setValue((record[0].data.validOnly)
											.substring(0, 10));
							patentPanel
									.getForm()
									.findField('patentregisterName')
									.setValue(record[0].data.patentregisterName);
							patentPanel
									.getForm()
									.findField('patentlastModifierName')
									.setValue(record[0].data.patentlastModifierName);
							patentPanel.getForm().findField('patentupdateDate')
									.setValue((record[0].data.patentupdateDate)
											.substring(0, 10));
							patentPanel
									.getForm()
									.findField('patentlastModified')
									.setValue((record[0].data.patentlastModified)
											.substring(0, 10));
							patentGrid.reload({
										params : {
											REFID : patentPanel.getForm()
													.findField('sjid')
													.getValue()
										}
									});

						});
					}
				}
			}, {
				header : '设计人',
				dataIndex : 'patentedDesignPeople',
				sortable : true,
				width : 230
			}, {
				header : '专利权人',
				dataIndex : 'patentRightPeople',
				sortable : true,
				width : 230
			}, {
				header : '有效期起',
				dataIndex : 'validFrom',
				sortable : true,
				// width : 230,
				renderer : function(value, cellmeta, record, rowIndex,
						columnIndex, store) {
					return value.substring(0, 10);
				}
			}, {
				header : '有效期止',
				dataIndex : 'validOnly',
				sortable : true,
				// width : 230,
				renderer : function(value, cellmeta, record, rowIndex,
						columnIndex, store) {
					return value.substring(0, 10);
				}
			}]);

	var patentList = new Ext.grid.GridPanel({
				id : 'patentList',
				cm : patentListCM,
				title : '专利列表',
				store : patentListStore,
				stripeRows : true,
				split : true,
				stateful : true,
				stateId : 'patentList-relatedGrid',
				region : 'north',
				forceFit : true,
				loadMask : true,
				tbar : []
			});

	var patentEastPanel = new Ext.TabPanel({
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
				items : [patentList, patentGrid]
			});

	/**
	 * 专利
	 */
	var patentInformation = new Ext.Panel({
		title : '专利信息管理',
		layout : 'border',
		id : 't5',
		autoScroll : true,
		border : false,
		tbar : {
			id : 'patent_t5',
			items : [{
				xtype : 'button',
				text : '保存',
				iconCls : 'ok',
				handler : function() {
					patentPanel.getForm().submit({
						url : __cxtPath
								+ '/crmMaint.do?method=doPatentInformation',
						params : {
							selectCompanyID : selectCompanyID
						},
						waitMsg : '正在保存，请稍候……',
						waitTitle : '正在处理',
						success : function(form, action) {
							Ext.Msg.alert(action.result.messageTitle,
									action.result.message);
							Ext.getCmp('patentList').getStore().reload();
							patentPanel.getForm().clear();
							patentInformation.attButtont.setVisible(false);

							patentPanel.getForm().findField('sjid')
									.setValue('');
							patentPanel.getForm().findField('companyID')
									.setValue('');
							patentPanel.getForm()
									.findField('patentedDesignPeopleID1')
									.setValue('');
							patentPanel.getForm()
									.findField('patentedDesignPeopleID2')
									.setValue('');
							patentPanel.getForm()
									.findField('patentedDesignPeopleID3')
									.setValue('');
							patentPanel.getForm()
									.findField('patentRightPeopleID1')
									.setValue('');
							patentPanel.getForm()
									.findField('patentRightPeopleID2')
									.setValue('');
							patentPanel.getForm()
									.findField('patentRightPeopleID3')
									.setValue('');
							patentPanel.getForm()
									.findField('patentRightPeopleID4')
									.setValue('');
							patentPanel.getForm()
									.findField('patentRightPeopleID5')
									.setValue('');
							patentPanel.getForm()
									.findField('patentregisterName')
									.setValue('');
							patentPanel.getForm()
									.findField('patentlastModifierName')
									.setValue('');
							patentPanel.getForm().findField('patentupdateDate')
									.setValue('');
							patentPanel.getForm()
									.findField('patentlastModified')
									.setValue('');

							patentGrid.reload({
										params : {
											REFID : ''
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
				xtype : 'uploadButton',
				text : '附件上传',
				reuses : false,
				ref : '../attButtont',
				// id : 'attButtonq17',
				hidden : true,
				REFIDFun : function() {
					patentEastPanel.setActiveTab(1);
					return patentPanel.getForm().findField('sjid').getValue();
				},
				attachmentTypeID : com.bitc.xmis.Attachment.patentDocuments,
				uploadPanelConfig : {
					listeners : {
						'uploadcomplete' : function() {
							patentGrid.reload();
						}
					}
				}
			}, '-', {
				xtype : 'button',
				text : '清空',
				iconCls : 'remove',
				ref : '../addreButton',
				handler : function() {
					patentPanel.getForm().clear();
					patentInformation.attButtont.setVisible(false);
					patentPanel.getForm().findField('sjid').setValue('');
					patentPanel.getForm().findField('companyID').setValue('');
					patentPanel.getForm().findField('patentedDesignPeopleID1')
							.setValue('');
					patentPanel.getForm().findField('patentedDesignPeopleID2')
							.setValue('');
					patentPanel.getForm().findField('patentedDesignPeopleID3')
							.setValue('');
					patentPanel.getForm().findField('patentRightPeopleID1')
							.setValue('');
					patentPanel.getForm().findField('patentRightPeopleID2')
							.setValue('');
					patentPanel.getForm().findField('patentRightPeopleID3')
							.setValue('');
					patentPanel.getForm().findField('patentRightPeopleID4')
							.setValue('');
					patentPanel.getForm().findField('patentRightPeopleID5')
							.setValue('');
					patentPanel.getForm().findField('patentregisterName')
							.setValue('');
					patentPanel.getForm().findField('patentlastModifierName')
							.setValue('');
					patentPanel.getForm().findField('patentupdateDate')
							.setValue('');
					patentPanel.getForm().findField('patentlastModified')
							.setValue('');
					// patentInformation.attButtont.setVisible(false);
					patentGrid.reload({
								params : {
									REFID : ''
								}
							});
				}
			}]
		},
		items : [patentPanel, patentEastPanel]
			//
		});
	/** ***************************************************************************************************** */

	/** **************************************************************************************************** */
	// function AddProcurementDetails(selectCompanyID){
	var procurementForm = new Ext.FormPanel({
		region : 'center',
		padding : 20,
		autoScroll : true,
		border : false,
		labelWidth : 101,
		labelAlign : 'right',
		items : [{
					xtype : 'hidden',
					name : 'id'
				}, {
					xtype : 'hidden',
					name : 'applicationSectorsID',// applicationSectorsID,applicationSectorsName,procurementFormChooseOrgTreeField
					id : 'applicationSectorsID'
				}, new Ext.ux.form.ChooseOrgTreeField({
					fieldLabel : '申请部门',
					id : 'applicationSectorsName',
					name : 'applicationSectorsName',
					width : 220,
					winConfig : {
						title : '选择申请部门',
						tbar : []
					},
					singleSelect : true,
					onlyPerson : false,
					rootConfig : function() {
						return {
							id : 'comp-old-' + selectCompanyID
						};
					},
					dataUrl : '/orgMaint.do?method=getOrganizationTree',
					clearValueList : 'applicationSectorsID,applicationSectorsName,aEmp_id,aEmp_name',
					setValueFun : function(result) {
						var deptName = '';
						var deptID = '';
						for (var i = 0; i < result.length; i++) {
							if (i == result.length - 1) {
								deptID = deptID + result[i].id;
								deptName = deptName + result[i].text;
							} else {
								deptID = deptID + result[i].id + ',';
								deptName = deptName + result[i].text + ',';
							}
						}
						Ext.getCmp('applicationSectorsID').setValue(deptID
								.split('-')[2]);
						Ext.getCmp('applicationSectorsName').setValue(deptName);
					},
					listeners : {}
				}), {
					xtype : 'hidden',
					name : 'ziID'
				}, {
					xtype : 'combo',
					fieldLabel : '资质名称',
					name : 'qualificationName',
					id : 'qualificationName',
					width : 220,
					displayField : 'qualificationName',
					valueField : 'id',
					triggerAction : 'all',
					editable : false,
					mode : 'romote',
					emptyText : '请选择资质',
					store : new Ext.data.JsonStore({
						url : __cxtPath
								+ '/crmMaint.do?method=getProjectQualification',
						root : 'rows',
						idProperty : 'id',
						autoLoad : true,
						fields : [{
									name : 'id'
								}, {
									name : 'qualificationName'
								}],
						baseParams : {
							selectCompanyID : selectCompanyID
						}
					}),
					listeners : {
						'select' : function(combo, record, index) {
							procurementForm.getForm().findField('ziID')
									.setValue(record.id);
						}
					}
				}, new Ext.ux.form.ChooseField({
					fieldLabel : '使用项目',
					// allowBlank : false,
					name : 'projectName',
					width : 220,
					winConfig : {
						title : '项目信息'
					},
					baseParamFun : function() {
						winFlag = "bWin";
						return {
							start : 0,
							limit : 20
						};
					},
					// allowBlank : false,// 弹出
					chooseUrl : __cxtPath
							+ '/resources/choose/projectQualification.json.jsp',
					setValueFun : function(o) {
						if (Ext.isEmpty(o)) {
							Ext.Msg.alert('提示', '您还没有选择，请选择');
						}
						procurementForm.getForm().findField('projectName')
								.setValue(o.data.projectName);
						procurementForm.getForm().findField('projectID')
								.setValue(o.data.projectID);
					},
					listeners : {}
				}), {
					xtype : 'hidden',
					name : 'projectID'
				}, {
					xtype : 'hidden',
					id : 'applicantID',
					name : 'applicantID'
				}, new Ext.ux.form.ChooseOrgTreeField({
					fieldLabel : '申请人',
					ref : 'applicantChooseOrgTreeField',
					allowBlank : false,
					name : 'applicant',
					id : 'applicant',
					// value:1,
					width : 220,
					winConfig : {
						title : '选择申请人',
						tbar : [
						// {
						//										
						// ref : '../addBtn',
						// iconCls : 'add',
						// align : 'right',
						// text : '新建',
						// handler : function() {
						// tabs.setActiveTab(1);
						// procurementWin.hide();
						// applicantTT = 1;
						// procurementForm.applicantChooseOrgTreeField.window
						// .hide()
						// }
						// }
						]
					},
					singleSelect : true,
					onlyPerson : true,
					rootConfig : function() {
						return {
							id : 'comp-old-' + selectCompanyID
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
								empName = empName + result[i].text + ','
							}
						}
						Ext.getCmp('applicantID').setValue(empID.split('-')[2]);

						Ext.getCmp('applicant').setValue(empName);

					},
					clearValueList : 'applicantID,applicant',
					listeners : {
						'beforeShowWin' : function() {
							var aCmpId = selectCompanyID;
							if (!aCmpId || aCmpId == "") {
								Ext.Msg.alert("提示", "请先创建公司");
								return false;
							}
						},
						'dblclick' : function(select, num) {

							var empId = Ext.getCmp('applicantID').getValue();
							com.bitc.xmis.util.EmpInfoWin(empId, '申请人信息');
						},
						'render' : function(c) {
							Ext.QuickTips.register({
										target : c.getEl(),
										text : '双击可显示详细信息'
									});
						},
						'nodedblclick' : function(node, e) {
							applicantTT = 2;
							tabs.setActiveTab(1);
							procurementForm.applicantChooseOrgTreeField.window
									.hide();
							var path = String
									.format('/root{0}', node.getPath());

							if (!Ext.isEmpty(path)) {
								Ext.getCmp('t2').items.items[0].orgtree
										.expandPath(path);
								Ext.getCmp('t2').items.items[0].orgtree
										.selectPath(path, null, function(s, n) {
											if (s) {
												Ext.getCmp('t2').items.items[0].orgtree
														.fireEvent('click', n);
											}
										});
							}
						}
					},
					clearValueList : ''

				}), {
					xtype : 'datefield',
					fieldLabel : '申请日期',
					format : 'Y-m-d',
					name : 'applicationDate',
					width : 220,
					allowBlank : false
				}, {
					xtype : 'textarea',
					fieldLabel : '申请原因',
					name : 'applicationReasons',
					width : 220,
					allowBlank : false
				}, {
					xtype : 'panel',
					layout : 'column',
					border : false,

					items : [{
								xtype : 'panel',
								layout : 'form',
								labelAlign : 'right',
								border : false,
								width : 200,
								labelAlign : 'right',
								items : [{
											fieldLabel : '登记人',
											name : 'applicationNewregisterName',
											xtype : 'displayfield'
										}, {
											fieldLabel : '最后修改人',
											name : 'applicationLastModifierName',
											xtype : 'displayfield'
										}]
							}, {
								xtype : 'panel',
								layout : 'form',
								border : false,
								labelAlign : 'right',
								items : [{
											fieldLabel : '登记日期',
											xtype : 'displayfield',
											name : 'applicationNewupdateDate'
										}, {
											fieldLabel : '最后修改日期',
											xtype : 'displayfield',
											name : 'applicationLastModified'
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
					procurementForm.getForm().submit({
						url : __cxtPath
								+ '/crmMaint.do?method=doProcurementInformation',
						waitMsg : '正在保存，请稍候……',
						waitTitle : '正在处理',
						success : function(form, action) {
							Ext.Msg.alert(action.result.messageTitle,
									action.result.message);
							procurementWin.hide();
							Ext.getCmp('projectQualificationp').getStore()
									.reload();
							// location.reload();

						},
						failure : function(form, action) {
							com.bitc.xmis.util.AlertUtil
									.formSubmitFailedAlert(action);
						}
					});
				}
			}, {
				hindf : '../saveButton',
				xtype : 'button',
				text : '取消',
				iconCls : 'remove',
				handler : function() {
					procurementWin.hide();
				}
			}]

		}
	});

	var procurementWin = new Ext.Window({
				modal : true,
				title : '资质使用申请登记',
				width : 430,
				height : 470,
				// id:'t6',
				layout : 'border',
				closeAction : 'hide',
				items : [promainPanel]
			});
	// procurementWin.show();
	// }

	compFormPanel.getForm().setValues(company);

	// 相关地址的load
	relativeAdressStore.load({
				params : {
					companyId : selectCompanyID,
					start : 0
				}
			});

	if (selectCompanyID == 1) {
		qualification.addreButton1.setVisible(true);
		eastPanel.add(projectQualificationp);
	}

	if (company != null) {
		tabs.addRelativeAddressButton.setVisible(true);
		compFormPanel.contactChooseOrgTreeField.setVisible(true);
		compFormPanel.legalRepresentativeEditField.setVisible(false);
		compFormPanel.contactEditField.setVisible(false);
		compFormPanel.legalChooseOrgTreeField.setVisible(true);
		compFormPanel.contactMoney.setVisible(true);

		// 联系人后面的编辑按钮是否隐藏
		if (company.contactPerson != null) {
			compFormPanel.contractEdit_btn.setVisible(true);
		}
		// 法人代表后面的编辑按钮是否隐藏
		if (company.legalRepresentative != null) {
			compFormPanel.legalEdit_btn.setVisible(true);
		}
		var contactMoney = '';
		var incontactMoney = '';
		var outcontactMoney = '';
		if (company.inputContactMoney != null
				&& "" != company.inputContactMoney.trim()
				&& formatMillion(company.inputContactMoney) != 0.00) {
			incontactMoney = '<font color=green>收入性:'
					+ formatMillion(company.inputContactMoney) + '</font><br>';
		}
		if (company.outputContactMoney != null
				&& "" != company.outputContactMoney.trim()
				&& formatMillion(company.outputContactMoney) != 0.00) {
			outcontactMoney = '<font color=#CC0000>支出性:'
					+ formatMillion(company.outputContactMoney) + '</font>';
		}
		contactMoney = incontactMoney + outcontactMoney;
		compFormPanel.getForm().findField('contactMoney')
				.setValue(contactMoney);

		compFormPanel.getForm().findField('contactPersonName')
				.setValue(company.contactPerson != null
						? company.contactPerson.userName.trim()
						: '');
		compFormPanel.getForm().findField('legalRepresentative')
				.setValue(company.legalRepresentative != null
						? company.legalRepresentative.userName.trim()
						: '');
		compFormPanel.getForm().findField('contactPersonID')
				.setValue(company.contactPerson != null
						? company.contactPerson.userID
						: '');
		compFormPanel.getForm().findField('legalRepresentativeID')
				.setValue(company.legalRepresentative != null
						? company.legalRepresentative.userID
						: '');
		compFormPanel.getForm().findField('updateID')
				.setValue(company.updatePerson != null
						? company.updatePerson.userID
						: '');
		compFormPanel.getForm().findField('updatePerson')
				.setValue(company.updatePerson != null
						? company.updatePerson.userName.trim()
						: '');
		compFormPanel.getForm().findField('lastModifierNameId')
				.setValue(company.lastModifier != null
						? company.lastModifier.userID
						: '');
		compFormPanel.getForm().findField('lastModifierName')
				.setValue(company.lastModifier != null
						? company.lastModifier.userName
						: '');
		compFormPanel.getForm().findField('updateDateId')
				.setValue(company.updateDate.trim());
		compFormPanel.getForm().findField('updateDate')
				.setValue(company.updateDate.trim().substr(0, 10));
		compFormPanel.getForm().findField('lastModified')
				.setValue(company.lastModified != null ? company.lastModified
						.trim().substr(0, 10) : '');

		// 对需要trim的字段做处理
		compFormPanel.getForm().findField('telephoneNo1')
				.setValue(company.telephoneNo1.trim());
		compFormPanel.getForm().findField('telephoneNo2')
				.setValue(company.telephoneNo2.trim());
		compFormPanel.getForm().findField('faxNo1').setValue(company.faxNo1
				.trim());
		compFormPanel.getForm().findField('faxNo2').setValue(company.faxNo2
				.trim());
		compFormPanel.getForm().findField('webSite').setValue(company.webSite
				.trim());
		compFormPanel.getForm().findField('emailAddress')
				.setValue(company.emailAddress.trim());
		compFormPanel.getForm().findField('bankName').setValue(company.bankName
				.trim());
		compFormPanel.getForm().findField('accountNo')
				.setValue(company.accountNo.trim());
		compFormPanel.getForm().findField('taxNo').setValue(company.taxNo
				.trim());
		compFormPanel.getForm().findField('organizationCode')
				.setValue(company.organizationCode.trim());
		compFormPanel.getForm().findField('registeredCapital')
				.setValue(company.registeredCapital.trim());

		compFormPanel.getForm().findField('businessRelationID')
				.setValue(company.businessRelation != null
						? company.businessRelation.businessRelationID
						: '');

		compFormPanel
				.getForm()
				.findField('currencyTypeID')
				.setValue(company.currency != null ? company.currency.id : '-1');

		compFormPanel.getForm().findField('companyScaleID')
				.setValue(company.companyScale != null
						? (company.companyScale.companyScaleID > 0
								? company.companyScale.companyScaleID
								: '')
						: '');
		compFormPanel.getForm().findField('customerLevelID')
				.setValue(company.customerLevel != null
						? company.customerLevel.customerLevelID
						: '');

		compFormPanel.getForm().findField('creditLevelID')
				.setValue(company.creditLevel != null
						? company.creditLevel.creditLevelID
						: '');

		// 地址实体中信息的显示
		compFormPanel.getForm().findField('addressID')
				.setValue(company.address != null
						? company.address.addressID
						: '');
		compFormPanel.getForm().findField('district')
				.setValue(company.address != null
						? company.address.district
						: '');
		compFormPanel.getForm().findField('zipCode')
				.setValue(company.address != null
						? (company.address.zipCode != 0
								? company.address.zipCode
								: '')
						: '');
		compFormPanel.getForm().findField('addressInfo')
				.setValue(company.address != null
						? company.address.addressInfo
						: '');

		if (company.address != null && company.address.city != null) {
			compFormPanel
					.getForm()
					.findField('internationID')
					.setValue(company.address.city.province.area.country.internation.internationID);

			compFormPanel
					.getForm()
					.findField('countryID')
					.setValue(company.address.city.province.area.country.countryID);
			var countryAreaCodeInfo = company.address.city.province.area.country.areaCode;
			compFormPanel.getForm().findField('countryCode')
					.setValue(countryAreaCodeInfo);
			compFormPanel.getForm().findField('countryCode2')
					.setValue(countryAreaCodeInfo);
			compFormPanel.getForm().findField('countryCode3')
					.setValue(countryAreaCodeInfo);
			compFormPanel.getForm().findField('countryCode4')
					.setValue(countryAreaCodeInfo);

			compFormPanel.getForm().findField('areaID')
					.setValue(company.address.city.province.area.areaID);

			compFormPanel.getForm().findField('provinceID')
					.setValue(company.address.city.province.provinceID);

			compFormPanel.getForm().findField('cityID')
					.setValue(company.address.city.cityID);

			var cityAreaCodeInfo = company.address.city.areaCode;
			compFormPanel.getForm().findField('areaCode')
					.setValue(cityAreaCodeInfo);
			compFormPanel.getForm().findField('areaCode2')
					.setValue(cityAreaCodeInfo);
			compFormPanel.getForm().findField('areaCode3')
					.setValue(cityAreaCodeInfo);
			compFormPanel.getForm().findField('areaCode4')
					.setValue(cityAreaCodeInfo);

			compFormPanel
					.getForm()
					.findField('internationName')
					.setValue(company.address.city.province.area.country.internation.internationName);

			compFormPanel
					.getForm()
					.findField('countryName')
					.setValue(company.address.city.province.area.country.countryName);

			compFormPanel.getForm().findField('areaName')
					.setValue(company.address.city.province.area.areaName);

			compFormPanel.getForm().findField('provinceName')
					.setValue(company.address.city.province.provinceName);

			compFormPanel.getForm().findField('cityName')
					.setValue(company.address.city.cityName);

			var isTrue = (company.address.city.province.provinceName.trim()) == (company.address.city.cityName
					.trim());
			var showname = "";
			if (isTrue) {
				showname = company.address.city.province.area.country.internation.internationName
						+ '·'
						+ company.address.city.province.area.country.countryName
						+ '·'
						+ company.address.city.province.area.areaName
						+ '·' + company.address.city.province.provinceName
			} else {
				showname = company.address.city.province.area.country.internation.internationName
						+ '·'
						+ company.address.city.province.area.country.countryName
						+ '·'
						+ company.address.city.province.area.areaName
						+ '·'
						+ company.address.city.province.provinceName
						+ '·' + company.address.city.cityName
			}
			compFormPanel.getForm().findField('chooseAddress')
					.setValue(showname);
		}

	}
	/**
	 * 供方评定调查信息赋值
	 */
	if (map != null) {

		if (map.ID != null) {
			supplierInformation.getForm().findField('companyID')
					.setValue(map.ID);
		}
		if (map['经理姓名'] != null) {
			supplierInformation.getForm().findField('manager')
					.setValue(map['经理姓名']);
		}
		if (map.managerID != null) {
			supplierInformation.getForm().findField('managerID')
					.setValue(map.managerID);
		}
		if (map.addressID != null) {
			supplierInformation.getForm().findField('officeAddressID')
					.setValue(map.addressID);
		}
		if (map['公司地址'] != null) {
			supplierInformation.getForm().findField('officeAddressName')
					.setValue(map['公司地址']);
		}
		if (map.headOfUnit != null) {
			supplierInformation.getForm().findField('headOfUnit')
					.setValue(map.headOfUnit);
		}
		if (map.industryAssessment != null) {
			supplierInformation.getForm().findField('industryAssessment')
					.setValue(map.industryAssessment);
		}
		if (map.standard != null) {
			supplierInformation.getForm().findField('standard')
					.setValue(map.standard);
		}
		if (map.qualification != null) {
			supplierInformation.getForm().findField('qualification')
					.setValue(map.qualification);
		}
		if (map.productsProduction != null) {
			supplierInformation.getForm().findField('productsProduction')
					.setValue(map.productsProduction);
		}
		if (map.ancillaryProducts != null) {
			supplierInformation.getForm().findField('ancillaryProducts')
					.setValue(map.ancillaryProducts);
		}
		if (map.Yield != null) {
			supplierInformation.getForm().findField('Yield')
					.setValue(map.Yield);
		}
		if (map.newregisterID != null) {
			supplierInformation.getForm().findField('newregisterID')
					.setValue(map.newregisterID);
		}
		if (map.newregisterName != null) {
			supplierInformation.getForm().findField('newregisterName')
					.setValue(map.newregisterName);
		}
		if (map.newupdateDate != null) {
			supplierInformation.getForm().findField('newupdateDate')
					.setValue((map.newupdateDate).substring(0, 10));
		}
		if (map.lastModifier != null) {
			supplierInformation.getForm().findField('lastModifier')
					.setValue(map.lastModifier);
		}
		if (map.lastModifierName != null) {
			supplierInformation.getForm().findField('lastModifierName')
					.setValue(map.lastModifierName);
		}
		if (map.lastModified != null) {
			supplierInformation.getForm().findField('lastModified')
					.setValue((map.lastModified).substring(0, 10));
		}
		if (map.generalTaxpayer != null) {
			supplierInformation.getForm().findField('generalTaxpayer')
					.setValue(((map.generalTaxpayer) == "true" ? 0 : 1));
		}
		if (map.qualityAssurance != null) {
			supplierInformation.getForm().findField('qualityAssurance')
					.setValue(map.qualityAssurance);
		}
		if (map.introductionLine != null) {
			supplierInformation.getForm().findField('introductionLine')
					.setValue((map.introductionLine) == "true" ? 0 : 1);
		}
		if (map.DH != null) {
			supplierInformation.getForm().findField('DH').setValue(map.DH);
		}
		if (map.CZ != null) {
			supplierInformation.getForm().findField('CZ').setRawValue(map.CZ);
		}
		if (map.Email != null) {
			supplierInformation.getForm().findField('Email')
					.setRawValue(map.Email);
		}
	}

	if (qualifica != null) {

		qualificationPanel.getForm().findField('id').setValue(qualifica.id);
		qualificationPanel.getForm().findField('qualificationContactID')
				.setValue(qualifica.qualificationContactID);
		if (qualifica.qualificationContact != null) {
			qualification.attButton1.setVisible(true);
			qualificationPanel.getForm().findField('qualificationContact')
					.setValue(qualifica.qualificationContact);
		}
		if (qualifica.certificateValidQi != null) {
			qualificationPanel.getForm().findField('certificateValidQi')
					.setValue((qualifica.certificateValidQi).substring(0, 10));
		}
		if (qualifica.certificateValidZhi != null) {
			qualificationPanel.getForm().findField('certificateValidZhi')
					.setValue((qualifica.certificateValidZhi).substring(0, 10));
		}
		qualificationPanel.getForm().findField('qualificationRemark')
				.setValue(qualifica.qualificationRemark);
		if (qualifica.examined != null) {
			qualificationPanel.getForm().findField('examined')
					.setValue((qualifica.examined) == "true" ? 1 : 0);
		}
		qualificationPanel.getForm().findField('qualificationName')
				.setValue(qualifica.qualificationName);
		qualificationPanel.getForm().findField('departmentName')
				.setValue(qualifica.departmentName);
		qualificationPanel.getForm().findField('lastModifierName')
				.setValue(qualifica.lastModifierName);
		qualificationPanel.getForm().findField('newregisterName')
				.setValue(qualifica.newregisterName);
		if (qualifica.lastModified != null) {
			qualificationPanel.getForm().findField('lastModified')
					.setValue((qualifica.lastModified).substring(0, 10));
		}
		if (qualifica.newupdateDate != null) {
			qualificationPanel.getForm().findField('newupdateDate')
					.setValue((qualifica.newupdateDate).substring(0, 10));
		}
		qualificationPanel.getForm().findField('certificateNo')
				.setValue(qualifica.certificateNo);
		qualificationPanel.getForm().findField('frequencyOfUse')
				.setValue(qualifica.TS);
		qualificationPanel.getForm().findField('DH').setValue(qualifica.DH);
		qualificationPanel.getForm().findField('CZ').setValue(qualifica.CZ);
		qualificationPanel.getForm().findField('Email')
				.setValue(qualifica.Email);

		qualificationGrid.reload({
					params : {
						REFID : qualificationPanel.getForm().findField('id')
								.getValue()
					}
				});
	}
	if (patent != null) {

		patentPanel.getForm().findField('sjid').setValue(patent.sjid);
		patentPanel.getForm().findField('companyID').setValue(patent.companyID);
		if (patent.patentName != null) {
			patentInformation.attButtont.setVisible(true);
			patentPanel.getForm().findField('patentName')
					.setValue(patent.patentName);
		}
		patentPanel.getForm().findField('patentsCategory')
				.setValue(patent.patentsCategory);
		patentPanel.getForm().findField('patentedDesignPeopleID1')
				.setValue(patent.patentedDesignPeopleID1);
		patentPanel.getForm().findField('patentedDesignPeople1')
				.setValue(patent.patentedDesignPeople1);
		patentPanel.getForm().findField('patentedDesignPeopleID2')
				.setValue(patent.patentedDesignPeopleID2);
		patentPanel.getForm().findField('patentedDesignPeople2')
				.setValue(patent.patentedDesignPeople2);
		patentPanel.getForm().findField('patentedDesignPeopleID3')
				.setValue(patent.patentedDesignPeopleID3);
		patentPanel.getForm().findField('patentedDesignPeople3')
				.setValue(patent.patentedDesignPeople3);
		patentPanel.getForm().findField('patentRightPeopleID1')
				.setValue(patent.patentRightPeopleID1);
		patentPanel.getForm().findField('patentRightPeople1')
				.setValue(patent.patentRightPeople1);
		patentPanel.getForm().findField('patentRightPeopleID2')
				.setValue(patent.patentRightPeopleID2);
		patentPanel.getForm().findField('patentRightPeople2')
				.setValue(patent.patentRightPeople2);
		patentPanel.getForm().findField('patentRightPeopleID3')
				.setValue(patent.patentRightPeopleID3);
		patentPanel.getForm().findField('patentRightPeople3')
				.setValue(patent.patentRightPeople3);
		patentPanel.getForm().findField('patentRightPeopleID4')
				.setValue(patent.patentRightPeopleID4);
		patentPanel.getForm().findField('patentRightPeople4')
				.setValue(patent.patentRightPeople4);
		patentPanel.getForm().findField('patentRightPeopleID5')
				.setValue(patent.patentRightPeopleID5);
		patentPanel.getForm().findField('patentRightPeople5')
				.setValue(patent.patentRightPeople5);
		patentPanel.getForm().findField('patentNumber')
				.setValue(patent.patentNumber);
		patentPanel.getForm().findField('patentCertificateNo')
				.setValue(patent.patentCertificateNo);
		if (patent.applicationTime != null) {
			patentPanel.getForm().findField('applicationTime')
					.setValue((patent.applicationTime).substring(0, 10));
		}
		if (patent.authorizeTime != null) {
			patentPanel.getForm().findField('authorizeTime')
					.setValue((patent.authorizeTime).substring(0, 10));
		}
		if (patent.validFrom != null) {
			patentPanel.getForm().findField('validFrom')
					.setValue((patent.validFrom).substring(0, 10));
		}
		if (patent.validOnly != null) {
			patentPanel.getForm().findField('validOnly')
					.setValue((patent.validOnly).substring(0, 10));
		}
		patentPanel.getForm().findField('patentregisterName')
				.setValue(patent.patentregisterName);
		patentPanel.getForm().findField('patentlastModifierName')
				.setValue(patent.patentlastModifierName);
		if (patent.patentupdateDate != null) {
			patentPanel.getForm().findField('patentupdateDate')
					.setValue((patent.patentupdateDate).substring(0, 10));
		}
		if (patent.patentlastModified != null) {
			patentPanel.getForm().findField('patentlastModified')
					.setValue((patent.patentlastModified).substring(0, 10));
		}
		patentGrid.reload({
					params : {
						REFID : patentPanel.getForm().findField('sjid')
								.getValue()
					}
				});
	}
	// 如果公司ID存在，“组织部门信息”标签显示，内容在tabchange后加载
	if (selectCompanyID != null
			|| compFormPanel.getForm().findField('companyID').getValue() != null) {
		// var panels = buildPanel(selectCompanyID);
		// tabs.add({
		// title : '组织部门信息',
		// layout : 'border',
		// ref : 'orgInfoPanel',
		// items : [panels[0], panels[1]]
		// });
		var pp = new Ext.Panel({
					layout : 'border',
					border : false,
					items : []
				});
		tabs.add({
					title : '组织部门信息',
					id : 't2',
					ref : 'organizationTab',
					border : false,
					ref : 'orgInfoPanel',
					items : []
				}, supplierAssessmentSurvey, qualification, patentInformation);// patentInformation
	}
	// 是中水科技的条件下 判断
	if (selectCompanyID == 1) {
		if (!isAdmin) {
			Ext.getCmp("tbar_saveCompInfo").hide();
		} else {
			Ext.getCmp("tbar_saveCompInfo").show();
		}
		// 判断操作人是否属于科质部
		if (isKZB ||isAdminUser) {
			Ext.getCmp('supplier_survey').show();
			Ext.getCmp('quallification_t4').show();
			Ext.getCmp('patent_t5').show();
		} else {
			Ext.getCmp('supplier_survey').hide();
			Ext.getCmp('quallification_t4').hide();
			Ext.getCmp('patent_t5').hide();
		}
	}

	// 其它页面跳转到此详细信息页面 tab外页面设置的一个参数
	var tab = request.getParameter("tab");
	if (tab != null) {
		currenttab = tab;
		if (!isNaN(tab)) {
			tabs.setActiveTab(Number(tab));
		} else {
			tabs.setActiveTab(tab);
		}
		// var path = request.getParameter("path");
		// if (!Ext.isEmpty(path)) {
		// tabs.orgInfoPanel.orgtree.expandPath(path);
		// tabs.orgInfoPanel.orgtree.selectPath(path, null, function(s, n) {
		// if (s) {
		// tabs.orgInfoPanel.orgtree.fireEvent('click', n);
		// }
		// });
		// }
	}
	// 变万元数据
	function formatMillion(data) {
		data = Ext.util.Format.number(data);
		data = data / 10000;
		if (data > 1) {
			data = Math.floor(data);
			data = Ext.util.Format.number(data, '0,000');
		} else {
			data = Ext.util.Format.number(data, '0,000.00');
		}
		return data;
	}
	var main = new Ext.Viewport({
				layout : 'border',
				items : tabs,
				border : false
			});

	function reloadtab(tabid) {
		var target = tabid || currenttab;
		var tab = request.getParameter("tab");
		var url = location.href;
		if (target != tab) {
			location.href = url.replace('tab=' + tab, 'tab=' + target);
		} else {
			location.reload();
		}
	}
});
