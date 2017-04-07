/**
 * 根据合同ID取得对应支出合同的编号下拉列表
 */
Ext.ns('com.bitc.xmis.Combos');

com.bitc.xmis.Combos.UseStypeIdCombo = Ext.extend(Ext.form.ComboBox, {
			valueField : 'id',
			displayField : 'name',
			triggerAction : 'all',
			lazyInit : false,
			mode : 'remote',
			emptyText : '选择领用级别',
			hiddenName : 'useStypeId',
			editable : false,
			initComponent : function() {
				this.store = new Ext.data.JsonStore({
							url : String.format(
									'{0}/device.do?method=getUseStypeList',
									__cxtPath),
							root : 'rows',
							autoLoad : true,
							fields : [{
										name : "id",
										type : "int",
										mapping : "id"
									}, {
										name : "name",
										type : "string",
										mapping : "name"
									}],
							listeners : {
								scope : this,
								'load' : function() {
									this.store.insert(0,
											new this.store.recordType({
														id : '0',
														name : '--'
													}));
									this.setValue(this.value);
								}
							}
						});
				com.bitc.xmis.Combos.UseStypeIdCombo.superclass.initComponent
						.call(this);
			}
		});
com.bitc.xmis.Combos.RelationChildContractCodeCombo = Ext.extend(
		Ext.form.ComboBox, {
			valueField : 'id',
			displayField : 'name',
			triggerAction : 'all',
			lazyInit : false,
			mode : 'remote',
			emptyText : '选择合同编号',
			editable : false,
			initComponent : function() {
				this.store = new Ext.data.JsonStore({
					url : String
							.format(
									'{0}/projectMain.do?method=getRelationChildContractCodeByID&&contractID={1}',
									__cxtPath, this.CONTRACTID),
					root : 'rows',
					autoLoad : true,
					fields : [{
								name : "id",
								type : "int",
								mapping : "contractID"
							}, {
								name : "name",
								type : "string",
								mapping : "contractCode"
							}, {
								name : "infoID",
								type : "string",
								mapping : "infoID"
							}, {
								name : "projectId",
								type : "string",
								mapping : "收入合同ID"
							}, {
								name : "projectCode",
								type : "string",
								mapping : "项目编码"
							}, {
								name : "projectName",
								type : "string",
								mapping : "项目名称"
							}],
					listeners : {
						scope : this,
						'load' : function(store, record) {
							this.setValue(this.value);
						}
					}
				});
				com.bitc.xmis.Combos.RelationChildContractCodeCombo.superclass.initComponent
						.call(this);
			}
		});
com.bitc.xmis.Combos.RelationChildContractCodeByProjectIdCombo = Ext.extend(
		Ext.form.ComboBox, {
			valueField : 'id',
			displayField : 'name',
			triggerAction : 'all',
			lazyInit : false,
			mode : 'remote',
			emptyText : '选择支出合同编号',
			editable : false,
			initComponent : function() {
				this.store = new Ext.data.JsonStore({
					url : String
							.format(
									'{0}/device.do?method=getContactListByProjectIdCombo ',
									__cxtPath),
					root : 'rows',
					autoLoad : true,
					fields : [{
								name : "id",
								type : "int",
								mapping : "支出子合同ID"
							}, {
								name : "name",
								type : "string",
								mapping : "合同编号"
							}],
					listeners : {
						scope : this,
						'load' : function() {
							this.store.insert(0, new this.store.recordType({
												id : '',
												name : '--'
											}));
							this.setValue(this.value);
						}
					}
				});
				com.bitc.xmis.Combos.RelationChildContractCodeByProjectIdCombo.superclass.initComponent
						.call(this);
			}
		});
com.bitc.xmis.Combos.RelationDeviceInfoCombo = Ext.extend(Ext.form.ComboBox, {
	valueField : 'id',
	displayField : 'name',
	triggerAction : 'all',
	lazyInit : false,
	mode : 'remote',
	emptyText : '选择物资名称',
	editable : false,
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
					url : String
							.format(
									'{0}/device.do?method=getDeviceInfoListWithPurchaseList',
									__cxtPath),
					root : 'rows',
					autoLoad : true,
					fields : [{
								name : "id",
								type : "string",
								mapping : "物资ID"
							}, {
								name : "name",
								type : "string",
								mapping : "物资名称"
							}, {
								name : "num",
								type : "string",
								mapping : "num"
							}],
					listeners : {
						scope : this,
						'load' : function() {
							this.store.insert(0, new this.store.recordType({
												id : '-1',
												name : '从物资库选择'
											})

							);
							this.setValue(this.value);
						}
					}
				});
		com.bitc.xmis.Combos.RelationDeviceInfoCombo.superclass.initComponent
				.call(this);
	}
});

com.bitc.xmis.Combos.RelationAddressCombo = Ext.extend(Ext.form.ComboBox, {
	valueField : 'id',
	displayField : 'name',
	triggerAction : 'all',
	lazyInit : false,
	mode : 'remote',
	emptyText : '选择相关地址',
	editable : false,
	hiddenName : '',
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
			url : String
					.format(
							'{0}/crmMaint.do?method=getRelationAddressCombo&&companyID={1}',
							__cxtPath, 1),
			root : 'rows',
			autoLoad : true,
			fields : [{
						name : "id",
						type : "string",
						mapping : "id"
					}, {
						name : "name",
						type : "string",
						mapping : "NAME"
					}],
			listeners : {
				scope : this,
				'load' : function() {
					this.setValue(this.value);
				}
			}
		});
		com.bitc.xmis.Combos.RelationAddressCombo.superclass.initComponent
				.call(this);
	}
});
/**
 * @class com.bitc.xmis.Combos.AddressCombo 库存位置选择器
 * @extends Ext.form.ComboBox
 */

com.bitc.xmis.Combos.AddressCombo = Ext.extend(Ext.form.ComboBox, {
			fieldLabel : '',
			hiddenName : '',
			width : 0,
			emptyText : '',
			dataUrl : '/device.do?method=getDeviceAddressCombo',
			baseParams : '',
			displayField : 'name',
			valueField : 'id',
			mode : 'remote',
			triggerAction : 'all',
			lazyInit : false,
			initComponent : function() {
				var url = this.dataUrl;
				this.store = new Ext.data.JsonStore({
							url : __cxtPath + url,
							baseParams : this.baseParams,
							root : 'rows',
							fields : [{
										name : 'id',
										mapping : 'id'
									}, {
										name : 'name',
										type : 'string',
										mapping : 'name'
									}],
							listeners : {
								scope : this,
								'load' : function(s, records, options) {
									this.setValue(this.value);
								}
							}
						});
				com.bitc.xmis.Combos.AddressCombo.superclass.initComponent
						.call(this);
			}
		});
com.bitc.xmis.Combos.AddressCombo = Ext.extend(Ext.form.ComboBox, {
			fieldLabel : '',
			hiddenName : '',
			width : 0,
			emptyText : '',
			dataUrl : '/device.do?method=getDeviceAddressCombo',
			baseParams : '',
			displayField : 'name',
			valueField : 'id',
			mode : 'remote',
			triggerAction : 'all',
			lazyInit : false,
			initComponent : function() {
				var url = this.dataUrl;
				this.store = new Ext.data.JsonStore({
							url : __cxtPath + url,
							baseParams : this.baseParams,
							root : 'rows',
							fields : [{
										name : 'id',
										mapping : 'id'
									}, {
										name : 'name',
										type : 'string',
										mapping : 'name'
									}],
							listeners : {
								scope : this,
								'load' : function(s, records, options) {
									this.setValue(this.value);
								}
							}
						});
				com.bitc.xmis.Combos.AddressCombo.superclass.initComponent
						.call(this);
			}
		});
com.bitc.xmis.Combos.AddressComboWareHouse = Ext.extend(Ext.form.ComboBox, {
			fieldLabel : '',
			hiddenName : '',
			width : 0,
			emptyText : '',
			dataUrl : '/device.do?method=getDeviceAddressCombo',
			baseParams : '',
			displayField : 'name',
			valueField : 'id',
			mode : 'remote',
			triggerAction : 'all',
			lazyInit : false,
			initComponent : function() {
				var url = this.dataUrl;
				this.store = new Ext.data.JsonStore({
							url : __cxtPath + url,
							baseParams : this.baseParams,
							root : 'rows',
							fields : [{
										name : 'id',
										mapping : 'id'
									}, {
										name : 'name',
										type : 'string',
										mapping : 'name'
									}, {
										name : 'personname',
										type : 'string',
										mapping : 'personname'
									}],
							listeners : {
								scope : this,
								'load' : function(s, records, options) {
									this.setValue(this.value);
								}
							}
						});
				com.bitc.xmis.Combos.AddressComboWareHouse.superclass.initComponent
						.call(this);
			}
		});

Ext.ns('com.bitc.xmis.device');

com.bitc.xmis.device.windowShow = function(flag, ID, contractID,
		contractIDParam, contactCode, deviceInfoID, deviceInfoName,
		purchaseListID, projectIdParam, projectCodeParam) {

	var useStypeIdCombo = new com.bitc.xmis.Combos.UseStypeIdCombo({
				fieldLabel : '领用级别',
				hiddenName : 'useStypeId',
				width : 200
			});
	var relationChildContractCodeCombo = new com.bitc.xmis.Combos.RelationChildContractCodeCombo(
			{
				fieldLabel : '支出合同编号',
				// fieldLabel : '根据合同ID得到的 支出合同编号',
				ref : 'outContractCode',
				readOnly : flag == 'edit' ? true : false,
				width : 200,
				CONTRACTID : contractID,
				hiddenName : 'outContract',
				listeners : {
					'select' : function(combo, record, index) {
						deviceInAddFormpanel.getForm()
								.findField("outContractId")
								.setValue(record.data.id);
						deviceInAddFormpanel.getForm()
								.findField("purchasingListId")
								.setValue(record.data.infoID);
						deviceInAddFormpanel.getForm().findField("projectId")
								.setValue(record.data.projectId);
						deviceInAddFormpanel.getForm().findField("projectCode")
								.setValue(record.data.projectCode);
						deviceInAddFormpanel.getForm().findField("projectName")
								.setValue(record.data.projectName);
						// 决定物资名称
						relationDeviceInfoCombo.store.baseParams.contractId = record.data.id;
						relationDeviceInfoCombo.store.load();
					}
				}
			});
	relationChildContractCodeCombo.store.on('load', function(store, record,
					opts) {

				if (store.data.length == 1) {
					var data = record[0].data;

					deviceInAddFormpanel.getForm().findField("outContract")
							.setValue(data.id);
					deviceInAddFormpanel.getForm().findField("outContractId")
							.setValue(data.id);
					deviceInAddFormpanel.getForm()
							.findField("purchasingListId")
							.setValue(data.infoID);
					deviceInAddFormpanel.getForm().findField("projectId")
							.setValue(data.projectId);
					deviceInAddFormpanel.getForm().findField("projectCode")
							.setValue(data.projectCode);
					deviceInAddFormpanel.getForm().findField("projectName")
							.setValue(data.projectName);
					// 决定物资名称
					relationDeviceInfoCombo.store.baseParams.contractId = data.id;
					relationDeviceInfoCombo.store.load();
				}
			});

	var relationChildContractCodeByProjectIdCombo = new com.bitc.xmis.Combos.RelationChildContractCodeByProjectIdCombo(
			{
				fieldLabel : '支出合同编号',
				// fieldLabel : '根据收入合同ID得到的 支出合同编号',
				ref : 'outContractCode',
				width : 200,
				hiddenName : 'outContractByProjectId',
				listeners : {
					'select' : function(combo, record, index) {
						relationDeviceInfoCombo.setValue('');
						// 详细名字信息的显示
						deviceInAddFormpanel.getForm()
								.findField("deviceInfoNameShow").setValue('');
						deviceInAddFormpanel.getForm()
								.findField("outContractId")
								.setValue(record.data.id);

						deviceInAddFormpanel.getForm()
								.findField("deviceInfoName2").hide();
						// 决定物资名称

						relationDeviceInfoCombo.store.baseParams.contractId = record.data.id;
						relationDeviceInfoCombo.store.load();
					}
				}
			});
	var inContractInSelectList = new Ext.ux.form.ChooseField(new Ext.ux.form.ChooseField(
			{
				fieldLabel : '收入合同编号',
				width : 200,
				name : 'inContractInSelectList',
				allowBlank : flag == 'newcreate' ? false : true,
				hidden : flag == 'newcreate' ? false : true,
				winConfig : {
					width : 765,
					title : '收入合同编号选择'
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
				chooseUrl : __cxtPath + '/resources/choose/contract.json.jsp',
				setValueFun : function(o) {
					if (Ext.isEmpty(o)) {
						Ext.Msg.alert('提示', '您还没有选择，请选择');
					}
					var code = o.get('项目编码');
					if (code.indexOf('^') > 0) {
						code = code.substring(0, code.indexOf('^'));
					}
					deviceInAddFormpanel.getForm().findField('inContractId')
							.setValue(o.get('项目ID'));
					deviceInAddFormpanel.getForm()
							.findField('inContractInSelectList').setValue(code);

					relationChildContractCodeByProjectIdCombo.store.baseParams.projectId = o
							.get('项目ID');
					relationChildContractCodeByProjectIdCombo.store.load();
					
					var contractname = o.get('合同名称');
					if (contractname != '' && contractname != null
							&& contractname.indexOf('^')) {
						var array = contractname.split('^');
						contractname = array[0];
					}
					// 合同名字信息的显示
					deviceInAddFormpanel.getForm()
							.findField("inContractInSelectListNameShow")
							.setValue(contractname);
				},
				clearValueFun : function() {
					deviceInAddFormpanel.getForm().findField('inContractId')
							.setValue('');
					deviceInAddFormpanel.getForm()
							.findField('inContractInSelectList').setValue('');
					// 合同名字信息的显示
					deviceInAddFormpanel.getForm()
							.findField("inContractInSelectListNameShow")
							.setValue('');
				}
			}));
	var relationDeviceInfoCombo = new com.bitc.xmis.Combos.RelationDeviceInfoCombo(
			{
				fieldLabel : '<font color="#CC0000">*</font>物资名称',
				ref : 'deviceInfo',
				width : 95,
				readOnly : (flag == 'edit' || flag == 'editOutcontract')
						? true
						: false,
				hiddenName : 'deviceInfoName',
				listeners : {
					'select' : function(combo, record, index) {
						if (record.data.id != -1) {
							deviceInAddFormpanel.getForm().findField("num")
									.setValue(record.data.num);
							deviceInAddFormpanel.getForm()
									.findField("deviceInfoId")
									.setValue(record.data.id);
							deviceInAddFormpanel.getForm()
									.findField("deviceInfoName2").hide();

							// 详细名字信息的显示
							deviceInAddFormpanel.getForm()
									.findField("deviceInfoNameShow")
									.setValue(record.data.name);
						} else {
							deviceInAddFormpanel.getForm()
									.findField("deviceInfoName2").show();
							deviceInAddFormpanel.getForm()
									.findField('deviceInfoNameShow')
									.setValue('');
						}
					}
				}
			});

	var relationAddressCombo = new com.bitc.xmis.Combos.RelationAddressCombo({
				fieldLabel : '<font color="#CC0000">*</font>存储地点',
				width : 200,
				emptyText : '选择存储地址',
				allowBlank : false,
				hiddenName : 'addressId',
				listeners : {
					'select' : function(combo, record, index) {
						houseCombo.clearValue();
						houseCombo.store.baseParams.fid = record.data.id;

						houseCombo.store.load();
						deviceInAddFormpanel.getForm()
								.findField("addressnameShow")
								.setValue(record.data.name);
						deviceInAddFormpanel.getForm()
								.findField("wareHouseNameShow").setValue('');

					}
				}
			});

	var houseCombo = new com.bitc.xmis.Combos.AddressComboWareHouse({
				fieldLabel : '<font color="#CC0000">*</font>库房',
				width : 200,
				emptyText : '选择库房',
				allowBlank : false,
				baseParams : {
					type : 'house'
				},
				hiddenName : 'wareHouseId2',
				listeners : {
					'select' : function(combo, record, index) {
						shelfCombo.clearValue();
						shelfCombo.store.baseParams.fid = record.data.id;

						shelfCombo.store.load();
						deviceInAddFormpanel.getForm()
								.findField("wareHouseNameShow")
								.setValue(record.data.name + '管理人:'
										+ record.data.personname);
						deviceInAddFormpanel.getForm()
								.findField("shelfNameShow").setValue('');
					}
				}
			});
	var shelfCombo = new com.bitc.xmis.Combos.AddressCombo({
				fieldLabel : '<font color="#CC0000">*</font>货架或区域',
				width : 200,
				HOUSEID : 0,
				emptyText : '选择货架或区域',
				allowBlank : false,
				baseParams : {
					type : 'shelf'
				},
				hiddenName : 'shelfId2',
				listeners : {
					'select' : function(combo, record, index) {
						positionCombo.clearValue();
						positionCombo.store.baseParams.fid = record.data.id;

						positionCombo.store.load();

						deviceInAddFormpanel.getForm()
								.findField("shelfNameShow")
								.setValue(record.data.name);
						deviceInAddFormpanel.getForm()
								.findField("positionNameShow").setValue('');

					}
				}
			});
	var positionCombo = new com.bitc.xmis.Combos.AddressCombo({
				fieldLabel : '<font color="#CC0000">*</font>位置',
				width : 200,
				SHELFID : 0,
				emptyText : '选择位置',
				allowBlank : false,
				baseParams : {
					type : 'position'
				},
				hiddenName : 'positionId2',
				listeners : {
					'select' : function(combo, record, index) {

						deviceInAddFormpanel.getForm().findField("positionId")
								.setValue(record.data.id);
						deviceInAddFormpanel.getForm()
								.findField("positionNameShow")
								.setValue(record.data.name);
					}
				}
			});

	positionCombo.store.on('load', function(store, record, opts) {
				if (store.data.length == 1) {
					var data = record[0].data;
					deviceInAddFormpanel.getForm().findField("positionId")
							.setValue(data.id);
					deviceInAddFormpanel.getForm().findField("positionId2")
							.setValue(data.name);
					deviceInAddFormpanel.getForm()
							.findField("positionNameShow").setValue(data.name);

				}
			});

	var relativeAddressFormPanel = new Ext.form.FormPanel({
		title : '',
		bodyStyle : 'padding:10px;',
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
				height : 280,
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
									companyId : 1,
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
									relationAddressCombo.store.reload();
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
	var deviceInAddFormpanelRecord = Ext.data.Record.create([{
				"mapping" : "id",
				"name" : "id",
				"type" : "string"
			}, {
				"mapping" : "projectId",
				"name" : "projectId",
				"type" : "string"
			}, {
				"mapping" : "projectCode",
				"name" : "projectCode",
				"type" : "string"
			}, {
				name : "projectName",
				type : "string",
				mapping : "projectName"
			}, {
				"mapping" : "outContractId",
				"name" : "outContractId",
				"type" : "int"
			}, {
				"mapping" : "outContract",
				"name" : "outContract",
				"type" : "string"
			}, {
				"mapping" : "purchasingListId",
				"name" : "purchasingListId",
				"type" : "string"
			}, {
				"mapping" : "deviceInfoId",
				"name" : "deviceInfoId",
				"type" : "string"
			}, {
				"mapping" : "deviceInfoName",
				"name" : "deviceInfoName",
				"type" : "string"
			}, {
				"mapping" : "deviceInfoName2",
				"name" : "deviceInfoName2",
				"type" : "string"
			}, {
				"mapping" : "inNum",
				"name" : "inNum",
				"type" : "int"
			}, {
				"mapping" : "inDate",
				"name" : "inDate",
				"type" : "string"

			}, {
				"mapping" : "useStypeId",
				"name" : "useStypeId",
				"type" : "int"
			}, {
				"dateFormat" : "Y-m-d",
				"mapping" : "outFactoryDate",
				"name" : "outFactoryDate",
				"type" : "date"
			}, {
				"mapping" : "backDays",
				"name" : "backDays"
			}, {
				"mapping" : "changeDays",
				"name" : "changeDays"
			}, {
				"mapping" : "repairDays",
				"name" : "repairDays"
			}, {
				"mapping" : "registerId",
				"name" : "registerId",
				"type" : "int"
			}, {
				"mapping" : "registerName",
				"name" : "registerName",
				"type" : "string"
			}, {
				"mapping" : "registerDate",
				"name" : "registerDate",
				"type" : "string"
			}, {
				"mapping" : "lastModifierNameId",
				"name" : "lastModifierNameId",
				"type" : "int"
			}, {
				"mapping" : "lastModifierName",
				"name" : "lastModifierName",
				"type" : "string"
			}, {
				"mapping" : "lastModified",
				"name" : "lastModified",
				"type" : "string"
			}, {
				"mapping" : "remarks",
				"name" : "remarks",
				"type" : "string"
			}, {
				"mapping" : "positionId",
				"name" : "positionId",
				"type" : "string"
			}, {
				"mapping" : "shelfId",
				"name" : "shelfId",
				"type" : "string"
			}, {
				"mapping" : "wareHouseId",
				"name" : "wareHouseId",
				"type" : "string"
			}, {
				"mapping" : "positionName",
				"name" : "positionId2",
				"type" : "string"
			}, {
				"mapping" : "shelfName",
				"name" : "shelfId2",
				"type" : "string"
			}, {
				"mapping" : "wareHouseName",
				"name" : "wareHouseId2",
				"type" : "string"
			}, {
				"mapping" : "addressId",
				"name" : "addressId",
				"type" : "string"
			}, {
				"mapping" : "保修期限",
				"name" : "保修期限",
				"type" : "string"
			}, {
				"mapping" : "可退期限",
				"name" : "可退期限",
				"type" : "string"
			}, {
				"mapping" : "可换期限",
				"name" : "可换期限",
				"type" : "string"
			}, {
				"mapping" : "addressname",
				"name" : "addressname",
				"type" : "string"
			}, {
				"mapping" : "positionName",
				"name" : "positionName",
				"type" : "string"
			}, {
				"mapping" : "shelfName",
				"name" : "shelfName",
				"type" : "string"
			}, {
				"mapping" : "wareHouseName",
				"name" : "wareHouseName",
				"type" : "string"
			}, {
				"mapping" : "whpersonname",
				"name" : "whpersonname",
				"type" : "string"
			}]);
	var deviceInAddFormpanel = new Ext.FormPanel({
		ref : 'form',
		region : 'center',
		border : false,
		reader : new Ext.data.JsonReader({
					root : 'rows'
				}, deviceInAddFormpanelRecord),
		labelAlign : 'right',
		autoScroll : true,
		labelAlign : 'right',
		labelWidth : 90,
		bodyStyle : 'padding:10px;',
		items : [
				{
					xtype : 'hidden',
					width : 200,
					name : 'inContractId'
				},
				{
					layout : 'column',
					xtype : 'panel',
					border : false,
					items : [{
								columnWidth : .45,
								layout : 'form',
								border : false,
								items : [inContractInSelectList]

							}, {
								columnWidth : .55,
								border : false,
								items : [{
											xtype : 'displayfield',
											name : 'inContractInSelectListNameShow',
											width : 300
										}]

							}]
				},
				{
					xtype : 'hidden',
					width : 200,
					name : 'projectId'
				},
				{
					layout : 'column',
					xtype : 'panel',
					border : false,
					items : [{
						columnWidth : .45,
						layout : 'form',
						border : false,
						items : [{
							xtype : 'displayfield',
							fieldLabel : '<font color="#CC0000">*</font>收入合同编号',
							allowBlank : flag == 'newcreate' ? true : false,
							hidden : flag == 'newcreate' ? true : false,
							width : 200,
							name : 'projectCode'
						}]

					}, {
						columnWidth : .55,
						border : false,
						items : [{
									xtype : 'displayfield',
									allowBlank : flag == 'newcreate'
											? true
											: false,
									hidden : flag == 'newcreate' ? true : false,
									width : 300,
									name : 'projectName'
								}]

					}]
				}, relationChildContractCodeCombo,
				relationChildContractCodeByProjectIdCombo, {
					xtype : 'hidden',
					name : 'outContractId'
				}, {
					xtype : 'hidden',
					name : 'id'
				}, {
					xtype : 'hidden',
					fieldLabel : '采购清单ID',
					width : 200,
					name : 'purchasingListId'
				}, {
					xtype : 'hidden',
					name : 'deviceInfoId'
				}, {
					layout : 'column',
					xtype : 'panel',
					border : false,
					items : [{
								columnWidth : .26,
								layout : 'form',
								border : false,
								items : [relationDeviceInfoCombo]
							}, {
								columnWidth : .18,
								layout : 'form',
								border : false,
								labelWidth : 1,
								items : [new Ext.ux.form.ChooseField({
									width : 90,
									hidden : true,
									name : 'deviceInfoName2',
									winConfig : {
										title : '物资名称选择'
									},
									baseParamFun : function() {
										return {
											start : 0,
											limit : 20
										};
									},
									chooseUrl : __cxtPath
											+ '/resources/choose/deviceInfo.json.jsp',
									setValueFun : function(v) {

										deviceInAddFormpanel.getForm()
												.findField('deviceInfoId')
												.setValue(v.get('物资ID'));
										deviceInAddFormpanel.getForm()
												.findField('deviceInfoName2')
												.setValue(v.get('物资名称'));

										// 详细名字信息的显示
										deviceInAddFormpanel
												.getForm()
												.findField("deviceInfoNameShow")
												.setValue(v.get('物资名称'));
									},
									clearValueFun : function() {
										deviceInAddFormpanel.getForm()
												.findField('deviceInfoId')
												.setValue('');
										deviceInAddFormpanel.getForm()
												.findField('deviceInfoName2')
												.setValue('');
										deviceInAddFormpanel
												.getForm()
												.findField('deviceInfoNameShow')
												.setValue('');

									}

								})]
							}, {
								columnWidth : .56,
								border : false,
								items : [{
											xtype : 'displayfield',
											name : 'deviceInfoNameShow',
											width : 300
										}]
							}]

				}, {
					xtype : 'numberfield',
					fieldLabel : '<font color="#CC0000">*</font>到货数量',
					allowBlank : false,
					width : 200,
					name : 'inNum'
				}, {
					xtype : 'datefield',
					fieldLabel : '<font color="#CC0000">*</font>到货日期',
					width : 200,
					editable : false,
					allowBlank : false,
					value : Ext.util.Format.date(new Date(), 'Y-m-d'),
					format : 'Y-m-d',
					name : 'inDate'
				}, {
					xtype : 'datefield',
					fieldLabel : '出厂日期',
					width : 200,
					editable : false,
					format : 'Y-m-d',
					name : 'outFactoryDate'

				}, {
					xtype : 'textfield',
					fieldLabel : '已采购数量',
					hidden : true,
					width : 200,
					name : 'num'
				}, {
					xtype : 'panel',
					layout : 'column',
					border : false,
					items : [{
								xtype : 'panel',
								layout : 'form',
								columnWidth : .4,
								labelAlign : 'right',
								labelWidth : 90,
								border : false,
								items : [{
											xtype : 'numberfield',
											fieldLabel : '原厂可退天数',
											width : 200,
											name : 'backDays'
										}, {
											xtype : 'numberfield',
											fieldLabel : '原厂可换天数',
											width : 200,
											name : 'changeDays'
										}, {
											xtype : 'numberfield',
											fieldLabel : '原厂保修天数',
											width : 200,
											name : 'repairDays'
										}]
							}, {
								xtype : 'panel',
								layout : 'form',
								columnWidth : .6,
								labelAlign : 'right',
								labelWidth : 90,
								border : false,
								items : [{
											xtype : 'textfield',
											fieldLabel : '可退期限',
											name : '可退期限',
											emptyText : '出厂日期+原厂可退天数',
											readOnly : true,
											width : 200
										}, {
											xtype : 'textfield',
											fieldLabel : '可换期限',
											name : '可换期限',
											emptyText : '出厂日期+原厂可换天数',
											readOnly : true,
											width : 200
										}, {
											xtype : 'textfield',
											fieldLabel : '保修期限',
											name : '保修期限',
											emptyText : '出厂日期+原厂保修天数',
											readOnly : true,
											width : 200
										}]
							}

					]
				},// start
				{
					xtype : 'hidden',
					name : 'shelfId'
				}, {
					xtype : 'hidden',
					name : 'shelfName'
				}, {
					xtype : 'hidden',
					name : 'wareHouseName'
				}, {
					xtype : 'hidden',
					name : 'wareHouseId'
				}, {
					xtype : 'hidden',
					name : 'positionId'
				}, {
					xtype : 'hidden',
					name : 'positionName'
				}, {
					xtype : 'hidden',
					name : 'addressname'
				}, {
					xtype : 'hidden',
					name : 'whpersonname'
				}, {
					layout : 'column',
					xtype : 'panel',
					border : false,
					items : [{
								columnWidth : .4,
								layout : 'form',
								border : false,
								items : [relationAddressCombo]
							}, {
								columnWidth : .05,
								layout : 'form',
								border : false,
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
							}, {
								columnWidth : .55,
								border : false,
								items : [{
											xtype : 'displayfield',
											name : 'addressnameShow',
											readOnly : true,
											width : 300
										}]
							}]
				}, {
					layout : 'column',
					xtype : 'panel',
					border : false,
					items : [{
								columnWidth : .4,
								layout : 'form',
								border : false,
								items : [houseCombo]
							}, {
								columnWidth : .05,
								layout : 'form',
								border : false,
								items : [{
									layout : 'form',
									xtype : 'button',
									text : '<font color="#003366">新建</font>',
									handler : function() {

										var fid = relationAddressCombo
												.getValue();
										var win = new Ext.Window({
											title : '新建',
											layout : 'border',
											width : 300,
											height : 150,
											modal : true,
											tbar : {
												xtype : 'toolbar',
												items : [{
													xtype : 'button',
													text : '保存',
													iconCls : 'ok',
													handler : function() {

														win.form.getForm()
																.submit({
																	url : __cxtPath
																			+ '/device.do?method=addDeviceAddress',
																	params : {
																		type : 'house',
																		fid : fid,
																		name : win.form
																				.getForm()
																				.findField('housename')
																				.getValue(),
																		code : win.form
																				.getForm()
																				.findField('managerID')
																				.getValue()
																	},
																	success : function(
																			response,
																			action) {

																		Ext.MessageBox
																				.hide();
																		Ext.MessageBox
																				.alert(
																						action.result.messageTitle,
																						action.result.message);
																		win
																				.hide();
																		houseCombo.store
																				.reload();
																	},
																	failure : function(
																			response,
																			action) {

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
														win.hide();
													}
												}]
											},
											items : [new Ext.FormPanel({
												ref : 'form',
												border : false,
												region : 'center',
												labelAlign : 'right',
												labelWidth : 80,
												autoScroll : true,
												bodyStyle : 'padding:10px;',
												items : [
														{
															xtype : 'textfield',
															fieldLabel : '库房名称',
															width : 150,
															name : 'housename'
														},
														{
															xtype : 'hidden',
															name : 'managerID',
															fieldLabel : '管理人ID'
														},
														new Ext.ux.form.ChooseOrgTreeField(
																{
																	fieldLabel : '管理人',
																	name : 'manager',
																	modal : true,
																	width : 150,
																	winConfig : {
																		title : '管理人设置',
																		bbar : ['<font color="#003366">*双击人员节点，可以查看此人的详细信息</font>']
																	},
																	singleSelect : true,
																	onlyPerson : true,
																	getExpandPath : function() {
																		return {
																			empId : win.form
																					.getForm()
																					.findField('managerID')
																					.getValue()
																		};
																	},
																	rootConfig : function() {
																		return {
																			id : 'comp-old-1'
																		}
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
																						+ ','
																			}
																		}
																		win.form
																				.getForm()
																				.findField('manager')
																				.setValue(empName);
																		win.form
																				.getForm()
																				.findField('managerID')
																				.setValue(empID
																						.split('-')[2]);
																	}
																})]
											})]

										}).show();
									}
								}]
							}, {
								columnWidth : .55,
								border : false,
								items : [{
											xtype : 'displayfield',
											name : 'wareHouseNameShow',
											readOnly : true,
											width : 300
										}]
							}]
				}, {
					layout : 'column',
					xtype : 'panel',
					border : false,
					items : [{
								columnWidth : .4,
								layout : 'form',
								border : false,
								items : [shelfCombo]
							}, {
								columnWidth : .05,
								layout : 'form',
								border : false,
								items : [{
									layout : 'form',
									xtype : 'button',
									text : '<font color="#003366">新建</font>',
									handler : function() {
										var fid = houseCombo.getValue();

										var win = new Ext.Window({
											title : '新建',
											layout : 'border',
											width : 300,
											height : 150,
											modal : true,
											tbar : {
												xtype : 'toolbar',
												items : [{
													xtype : 'button',
													text : '保存',
													iconCls : 'ok',
													handler : function() {

														win.form.getForm()
																.submit({
																	url : __cxtPath
																			+ '/device.do?method=addDeviceAddress',
																	params : {
																		type : 'shelf',
																		fid : fid,
																		name : win.form
																				.getForm()
																				.findField('shelfname')
																				.getValue(),
																		code : win.form
																				.getForm()
																				.findField('desc')
																				.getValue()
																	},
																	success : function(
																			response,
																			action) {

																		Ext.MessageBox
																				.hide();
																		Ext.MessageBox
																				.alert(
																						action.result.messageTitle,
																						action.result.message);
																		win
																				.hide();
																		shelfCombo.store
																				.reload();
																	},
																	failure : function(
																			response,
																			action) {

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
														win.hide();
													}
												}]
											},
											items : [new Ext.FormPanel({
												ref : 'form',
												border : false,
												region : 'center',
												labelAlign : 'right',
												labelWidth : 80,
												autoScroll : true,
												bodyStyle : 'padding:10px;',
												items : [{
															xtype : 'textfield',
															fieldLabel : '货架编号',
															width : 130,
															name : 'shelfname'
														}, {
															xtype : 'textfield',
															fieldLabel : '描述',
															width : 130,
															name : 'desc'
														}]
											})]

										}).show();
									}
								}]
							}, {
								columnWidth : .55,
								border : false,
								items : [{
											xtype : 'displayfield',
											name : 'shelfNameShow',
											readOnly : true,
											width : 300
										}]
							}]
				}, {
					layout : 'column',
					xtype : 'panel',
					border : false,
					items : [{
								columnWidth : .4,
								layout : 'form',
								border : false,
								items : [positionCombo]
							}, {
								columnWidth : .05,
								layout : 'form',
								border : false,
								items : [{
									layout : 'form',
									xtype : 'button',
									text : '<font color="#003366">新建</font>',
									handler : function() {

										var win = new Ext.Window({
											title : '新建',
											layout : 'border',
											width : 250,
											height : 180,
											modal : true,
											tbar : {
												xtype : 'toolbar',
												items : [{
													xtype : 'button',
													text : '保存',
													iconCls : 'ok',
													handler : function() {
														var fid = shelfCombo
																.getValue();

														win.form.getForm()
																.submit({
																	url : __cxtPath
																			+ '/device.do?method=addDeviceAddress',
																	params : {
																		type : 'position',
																		fid : fid,
																		name : win.form
																				.getForm()
																				.findField('positionName')
																				.getValue(),
																		code : win.form
																				.getForm()
																				.findField('positionCode')
																				.getValue()
																	},
																	success : function(
																			response,
																			action) {

																		Ext.MessageBox
																				.hide();
																		Ext.MessageBox
																				.alert(
																						action.result.messageTitle,
																						action.result.message);
																		win
																				.hide();
																		positionCombo.store
																				.reload();
																	},
																	failure : function(
																			response,
																			action) {

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
														win.hide();
													}
												}]
											},
											items : [new Ext.FormPanel({
												ref : 'form',
												border : false,
												region : 'center',
												labelAlign : 'right',
												labelWidth : 80,
												autoScroll : true,
												bodyStyle : 'padding:10px;',
												items : [{
															xtype : 'textfield',
															fieldLabel : '位置编号',
															width : 130,
															name : 'positionCode'
														}, {
															xtype : 'textfield',
															fieldLabel : '描述',
															width : 130,
															name : 'positionName'
														}]
											})]

										}).show();

									}
								}]
							}, {
								columnWidth : .55,
								border : false,
								items : [{
											xtype : 'displayfield',
											name : 'positionNameShow',
											readOnly : true,
											width : 300
										}]
							}]

				}, useStypeIdCombo, {
					xtype : 'textarea',
					fieldLabel : '备注',
					width : 590,
					height : 30,
					name : 'remarks'
				}
				// stop
				, {
					xtype : 'panel',
					layout : 'column',
					border : false,
					hidden : (flag == 'edit' || flag == 'editOutcontract')
							? false
							: true,
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
									xtype : 'hidden',
									name : 'registerId'
								}, {
									xtype : 'displayfield',
									fieldLabel : '经手人',
									width : 200,
									name : 'registerName',
									value : '',
									anchor : '100%',
									initEvents : function() {
										this.mon(this.el, 'dblclick',
												this.onDblClick, this);
									},
									onDblClick : function() {
										com.bitc.xmis.util
												.EmpInfoWin(
														deviceInAddFormpanel
																.getForm()
																.findField("registerId")
																.getValue(),
														'经手人信息');
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
									width : 200,
									name : 'registerDate'
								}]
					}, {
						xtype : 'panel',
						layout : 'form',
						columnWidth : 0.25,
						labelAlign : 'right',
						labelWidth : 80,
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
										com.bitc.xmis.util
												.EmpInfoWin(
														deviceInAddFormpanel
																.getForm()
																.findField("lastModifierNameId")
																.getValue(),
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
						columnWidth : 0.25,
						labelWidth : 90,
						labelAlign : 'right',
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
				}]
	});
	deviceInAddFormpanel.getForm().findField('inNum').on('blur', function(t) {

		var value = deviceInAddFormpanel.getForm().findField('inNum')
				.getValue();

		var purchaseListID = deviceInAddFormpanel.getForm()
				.findField('purchasingListId').getValue();
		var deviceInfoID = deviceInAddFormpanel.getForm()
				.findField('deviceInfoId').getValue();;
		if (purchaseListID != '' && deviceInfoID != '') {
			Ext.Ajax.request({
						url : __cxtPath + '/device.do',
						success : function(response, config) {
							var action = Ext.decode(response.responseText);
							var num = action.attributes.num;
							if (value > num && num != 0) {
								Ext.Msg.alert("提示", "到货数量大于当前采购的数量" + num);
							}
							if (!action.success) {
								t.markInvalid(action.message);
							}

						},
						failure : function() {
						},
						method : "post",
						params : {
							method : 'getPurchaseSumNum',
							deviceInfoId : deviceInfoID,
							purchaseListId : purchaseListID
						}
					});

		}
	});
	var getDeadLineFun = function(rescfield, destfield) {
		var outFactoryDateStr = deviceInAddFormpanel.getForm()
				.findField('outFactoryDate').getValue();
		var days = deviceInAddFormpanel.getForm().findField(rescfield)
				.getValue();
		if (days != '' && outFactoryDateStr != '') {
			// var result = outFactoryDateStr.add(Date.DAY,days);
			// var back_Str = Ext.util.Format.date(result, 'Y-m-d');
			// deviceInAddFormpanel.getForm().findField(destfield).setValue(back_Str);
			Ext.Ajax.request({
						url : __cxtPath + '/device.do',
						success : function(response, config) {
							var action = Ext.decode(response.responseText);
							var result = action.attributes.result;
							deviceInAddFormpanel.getForm().findField(destfield)
									.setValue(result);

						},
						failure : function() {
						},
						method : "post",
						params : {
							method : 'getDeadlineByDateAndDays',
							rescDate : outFactoryDateStr,
							days : days
						}
					});
		}
	}
	deviceInAddFormpanel.getForm().findField('outFactoryDate').on('blur',
			function(c) {
				var backDays = deviceInAddFormpanel.getForm()
						.findField('backDays').getValue();
				var changeDays = deviceInAddFormpanel.getForm()
						.findField('changeDays').getValue();
				var repairDays = deviceInAddFormpanel.getForm()
						.findField('repairDays').getValue();
				if (backDays != '') {
					getDeadLineFun('backDays', '可退期限');
				}
				if (changeDays != '') {
					getDeadLineFun('changeDays', '可换期限');
				}
				if (repairDays != '') {
					getDeadLineFun('repairDays', '保修期限');
				}

			});
	deviceInAddFormpanel.getForm().findField('backDays').on('blur',
			function(c) {
				var outFactoryDateStr = deviceInAddFormpanel.getForm()
						.findField('outFactoryDate').getValue();
				if (outFactoryDateStr == ''
						&& deviceInAddFormpanel.getForm().findField('backDays')
								.getValue() != '') {
					Ext.MessageBox.alert('提示', '因出厂日期为空，无法计算可退期限，请选择出厂日期！');
				} else {
					getDeadLineFun('backDays', '可退期限');
				}
			});
	deviceInAddFormpanel.getForm().findField('changeDays').on('blur',
			function(c) {
				var outFactoryDateStr = deviceInAddFormpanel.getForm()
						.findField('outFactoryDate').getValue();
				if (outFactoryDateStr == ''
						&& deviceInAddFormpanel.getForm()
								.findField('changeDays').getValue() != '') {
					Ext.MessageBox.alert('提示', '因出厂日期为空，无法计算可换期限，请选择出厂日期！');
				} else {
					getDeadLineFun('changeDays', '可换期限');
				}
			});
	deviceInAddFormpanel.getForm().findField('repairDays').on('blur',
			function(c) {
				var outFactoryDateStr = deviceInAddFormpanel.getForm()
						.findField('outFactoryDate').getValue();
				if (outFactoryDateStr == ''
						&& deviceInAddFormpanel.getForm()
								.findField('repairDays').getValue() != '') {
					Ext.MessageBox.alert('提示', '因出厂日期为空，无法计算保修期限，请选择出厂日期！');
				} else {
					getDeadLineFun('repairDays', '保修期限');
				}
			});

	var win = new Ext.Window({
		title : '到货信息录入',
		layout : 'border',
		region : 'center',
		closeAction : 'hide',
		closable : true,
		width : 800,
		height : 520,
		modal : true,
		tbar : {
			xtype : 'toolbar',
			items : [{
				xtype : 'button',
				text : '保存',
				iconCls : 'ok',
				handler : function() {
					if (deviceInAddFormpanel.getForm().isValid()) {
						if (flag != 'newcreate'
								&& deviceInAddFormpanel.getForm()
										.findField("projectId").getValue() == ''
								|| deviceInAddFormpanel.getForm()
										.findField("projectId").getValue() == null) {
							Ext.MessageBox.alert('提示', '请选择“支出合同编号”!');
							return;
						}
						var projectId = '';
						if (flag != 'newcreate') {
							projectId = deviceInAddFormpanel.getForm()
									.findField("projectId").getValue();
						} else {
							projectId = deviceInAddFormpanel.getForm()
									.findField("inContractId").getValue();
						}
						if (deviceInAddFormpanel.getForm()
								.findField("deviceInfoId").getValue() == ''
								|| deviceInAddFormpanel.getForm()
										.findField("deviceInfoId").getValue() == null) {
							Ext.MessageBox.alert('提示', '请选择“物资名称”!');
							return;
						}

						var outFactoryDate = deviceInAddFormpanel.getForm()
								.findField("outFactoryDate").getValue();
						var backDays = deviceInAddFormpanel.getForm()
								.findField("backDays").getValue();
						var changeDays = deviceInAddFormpanel.getForm()
								.findField("changeDays").getValue();
						var repairDays = deviceInAddFormpanel.getForm()
								.findField("repairDays").getValue();
						var tipInfo = "";
						var t1 = "";
						var t2 = "";
						var t3 = "";
						var t4 = "";

						if (outFactoryDate == '' || outFactoryDate == null) {
							t1 = "出厂日期、";
						}
						if (backDays == null || backDays == '') {
							t3 = "原厂可退天数、";
						}
						if (changeDays == '' || changeDays == null) {
							t4 = "原厂可换天数、";
						}
						if (repairDays == '' || repairDays == null) {
							t2 = "原厂保修天数、";
						}
						tipInfo = t1 + t2 + t3 + t4;
						var b = deviceInAddFormpanel.getForm()
								.findField("outContractId").getValue();
						if (tipInfo == "") {
							deviceInAddFormpanel.getForm().submit({
								url : __cxtPath
										+ '/device.do?method=saveDeviceIn',
								params : {
									ID : deviceInAddFormpanel.getForm()
											.findField("id").getValue(),
									useStypeId : deviceInAddFormpanel.getForm()
											.findField("useStypeId").getValue(),
									outContractId : deviceInAddFormpanel
											.getForm()
											.findField("outContractId")
											.getValue(),
									projectId : projectId,
									purchasingListId : deviceInAddFormpanel
											.getForm()
											.findField("purchasingListId")
											.getValue(),
									deviceInfoId : deviceInAddFormpanel
											.getForm()
											.findField("deviceInfoId")
											.getValue(),
									positionId : deviceInAddFormpanel.getForm()
											.findField("positionId").getValue(),
									registerDate : deviceInAddFormpanel
											.getForm()
											.findField("registerDate")
											.getValue()
								},
								success : function(rResponse, action) {
									Ext.MessageBox.alert(
											action.result.messageTitle,
											action.result.message);
									win.hide();
									parent.xMis.refresh(
											'deviceInGridPanelLOAD',
											'deviceInRegisterLOAD');
								},
								failure : function(response, action) {
									Ext.MessageBox.alert("提示", "操作失败！");
								}
							});

						} else {
							tipInfo = tipInfo.substr(0, tipInfo.length - 1);
							Ext.MessageBox.confirm("提示", tipInfo
											+ "为空，是否继续保存操作？", function(btn) {
										if (btn == 'yes') {
											deviceInAddFormpanel.getForm()
													.submit({
														url : __cxtPath
																+ '/device.do?method=saveDeviceIn',
														params : {
															ID : deviceInAddFormpanel
																	.getForm()
																	.findField("id")
																	.getValue(),
															useStypeId : deviceInAddFormpanel
																	.getForm()
																	.findField("useStypeId")
																	.getValue(),
															outContractId : deviceInAddFormpanel
																	.getForm()
																	.findField("outContractId")
																	.getValue(),
															projectId : projectId,
															purchasingListId : deviceInAddFormpanel
																	.getForm()
																	.findField("purchasingListId")
																	.getValue(),
															deviceInfoId : deviceInAddFormpanel
																	.getForm()
																	.findField("deviceInfoId")
																	.getValue(),
															positionId : deviceInAddFormpanel
																	.getForm()
																	.findField("positionId")
																	.getValue(),
															registerDate : deviceInAddFormpanel
																	.getForm()
																	.findField("registerDate")
																	.getValue()
														},
														success : function(
																rResponse,
																action) {
															Ext.MessageBox
																	.alert(
																			action.result.messageTitle,
																			action.result.message);
															win.hide();
															parent.xMis
																	.refresh(
																			'deviceInGridPanelLOAD',
																			'deviceInRegisterLOAD');
														},
														failure : function(
																response,
																action) {
															Ext.MessageBox
																	.alert(
																			"提示",
																			"操作失败！");
														}
													});
										}
									});
						}
					}
				}
			}, {
				xtype : 'button',
				text : '删除',
				iconCls : 'delIcon',
				ref : '../delbtn',
				hidden : true,
				handler : function() {
					Ext.MessageBox.confirm("提示", "是否删除此条到货信息？", function(btn) {
						if (btn == 'yes') {
							Ext.Ajax.request({
								url : __cxtPath
										+ '/device.do?method=delDeviceInObjByID',
								method : 'post',
								params : {
									id : deviceInAddFormpanel.getForm()
											.findField("id").getValue()
								},
								success : function(response, action) {
									Ext.MessageBox.alert("提示", "删除成功！");
									win.hide();
									parent.xMis.refresh(
											'deviceInGridPanelLOAD',
											'deviceInRegisterLOAD');
								},
								failure : function(response, action) {
									Ext.MessageBox.hide();
									Ext.MessageBox.alert("提示", "操作失败！");
								}
							});
						}
					});
				}
			}]
		},
		items : [deviceInAddFormpanel]

	}).show();
	if (contractOpType != 5) {// 对合同的操作类型
		win.getTopToolbar().disable();
	}
	if (flag == 'edit' || flag == 'editOutcontract') {

		if (flag == 'editOutcontract') {
			relationChildContractCodeCombo.setVisible(false);
			relationChildContractCodeByProjectIdCombo.setVisible(true);
		} else {
			relationChildContractCodeByProjectIdCombo.setVisible(false);
		}
		deviceInAddFormpanel.getForm().load({
			url : __cxtPath + '/device.do?method=getDeviceInInfoByID',
			params : {
				id : ID
			},
			waitMsg : '正在载入数据...',
			waitTitle : '请等待...',
			success : function(form, action) {
				houseCombo.store.baseParams.fid = deviceInAddFormpanel
						.getForm().findField("addressId").getValue();
				shelfCombo.store.baseParams.fid = deviceInAddFormpanel
						.getForm().findField("wareHouseId").getValue();
				positionCombo.store.baseParams.fid = deviceInAddFormpanel
						.getForm().findField("shelfId").getValue();

				deviceInAddFormpanel.getForm().findField("useStypeId")
						.setValue(deviceInAddFormpanel.getForm()
								.findField("useStypeId").getValue() == 0
								? ''
								: deviceInAddFormpanel.getForm()
										.findField("useStypeId").getValue());

				deviceInAddFormpanel.getForm().findField("addressnameShow")
						.setValue(deviceInAddFormpanel.getForm()
								.findField("addressname").getValue());
				deviceInAddFormpanel.getForm().findField("wareHouseNameShow")
						.setValue(deviceInAddFormpanel.getForm()
								.findField("wareHouseName").getValue()
								+ '管理人：'
								+ deviceInAddFormpanel.getForm()
										.findField("whpersonname").getValue());
				deviceInAddFormpanel.getForm().findField("shelfNameShow")
						.setValue(deviceInAddFormpanel.getForm()
								.findField("shelfName").getValue());
				deviceInAddFormpanel.getForm().findField("positionNameShow")
						.setValue(deviceInAddFormpanel.getForm()
								.findField("positionName").getValue());
				// 详细名字信息的显示
				deviceInAddFormpanel.getForm().findField("deviceInfoNameShow")
						.setValue(deviceInAddFormpanel.getForm()
								.findField("deviceInfoName").getValue());

				if (flag == 'editOutcontract') {
					relationChildContractCodeByProjectIdCombo.store.baseParams.projectId = deviceInAddFormpanel
							.getForm().findField("projectId").getValue();
					relationChildContractCodeByProjectIdCombo.store.load();
				}
			},
			failure : function(form, action) {
				Ext.MessageBox.alert('提示', '载入失败');
			}

		});

		win.delbtn.show();

	} else if (flag == 'create') {
		relationChildContractCodeByProjectIdCombo.setVisible(false);
	} else if (flag == 'newcreate') {

		relationChildContractCodeCombo.setVisible(false);
		relationChildContractCodeByProjectIdCombo.setVisible(true);
	}
}
