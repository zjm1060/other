Ext.onReady(function() {

	var tabDisable = false; // tab标签是否不可见的属性
	var selectCompanyID = null; // 公司ID
	var selectLevel = "";// 地址选定的级别 省1 市2、

	var internationFid = ""; // 所选洲际的ID
	var countryFid = ""; // 所选的国家的ID
	var areaFid = ""; // 所选的地区的ID
	var provinceFid = ""; // 所选的省份的ID
	var wstate = 0;// 联系人新建、节点双击的状态标识
	var lstate = 0;// 法人代表新建、节点双击的状态标识
	var cflag = 0;// 判断是否初次点击组织机构tab的标识

	Ext.QuickTips.init();
	Ext.form.Field.prototype.msgTarget = 'under';

	if (company != null) {
		selectCompanyID = company.companyID;
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
								maskRe : new RegExp('[' +
											 Ext.escapeRe('0123456789+-_ ()')
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
//							}, {
//								xtype : 'numberfield',
//								name : 'telephoneNo2',
//								emptyText : '请输入电话号码',
//								width : 110,
//								margins : '0 0 0 5',
//								height : 22,
//								allowBlank : true
							}, {
								xtype : 'textfield',
								name : 'telephoneNo2',
								emptyText : '请输入电话号码',
								width : 110,
								margins : '0 0 0 5',
								maskRe : new RegExp('[' +
											 Ext.escapeRe('0123456789+-_ ()')
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
								maskRe : new RegExp('[' +
											 Ext.escapeRe('0123456789+-_ ()')
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
								maskRe : new RegExp('[' +
											 Ext.escapeRe('0123456789+-_ ()')
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
						location.reload();
					}
				}]
			},
			items : [compFormPanel, eastPanelComp]
		}],
		listeners : {
			'tabchange' : function(t, p) {
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
			}
		}
	});

	compFormPanel.getForm().setValues(company);

	// 相关地址的load
	relativeAdressStore.load({
				params : {
					companyId : selectCompanyID,
					start : 0
				}
			});
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
				});
	}
	// 是中水科技的条件下 判断
	if (selectCompanyID == 1) {
		if (!isAdmin) {
			Ext.getCmp("tbar_saveCompInfo").hide();
		} else {
			Ext.getCmp("tbar_saveCompInfo").show();
		}
	}

	// 其它页面跳转到此详细信息页面 tab外页面设置的一个参数
	var tab = request.getParameter("tab");
	if (tab != null) {
		tabs.setActiveTab(Number(tab));

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
	
});
