Ext.onReady(function() {
	Ext.QuickTips.init();
	Ext.form.Field.prototype.msgTarget = 'under';
	var DTSJ = new Date().format('Y-m-d');
	var SJ = "";
	/**
	 * 附件列表
	 */
	var attFileGrid = new com.bitc.xmis.AttachmentList({
				title : '其他相关文件',
				stripeRows : true,
				REFID : materialsID == null ? 0 : materialsID,
				attachmentTypeID : com.bitc.xmis.Attachment.materials,
				width : 300,
				height : 100
			});
	// alert(dfo.factoryCommodityAddress);
	var imgs = new Ext.ux.ImgsBoxComponent({
		width : 330, // 图片宽度
		style : 'padding :0 0 0 0; margin: 0px 2px 2px 2px ;!important',
		picName : 'postCard.png',
		height : 330
			// 图片高度
		});

	// alert(materialsID == null ? 0 : materialsID);
	// alert(decisionFlag);
	var materialFromPanel = new Ext.FormPanel({
		id : 'materialFromPanel',
		autoScroll : true,
		bodyStyle : 'padding:30px 5px 0px 5px',
		region : 'center',
		defaultType : 'textfield',
		labelAlign : 'right',
		labelWidth : 80,
		items : [{
			xtype : 'panel',
			border : false,
			// columnWidth : .5,
			items : [{
				layout : 'column', // 从左往右布局
				xtype : 'panel',
				autoScroll : true,
				border : false,
				width : 1100,
				items : [{// 第一列
					// columnWidth : .34,
					layout : 'form',
					width : 400,
					xtype : 'panel',
					border : false,
					items : [{
								xtype : 'hidden',
								name : 'empID',
								id : 'empID'
							}, {
								xtype : 'textfield',
								fieldLabel : '物资ID',
								name : 'id',
								hidden : true
							}, {
								xtype : 'textfield',
								fieldLabel : '物资名称',
								name : 'materiaName',
								allowBlank : false,
								width : 250,
								blankText : '该输入项为必输项'
							}, {
								xtype : 'textfield',
								fieldLabel : '型号',
								width : 250,
								name : 'modelType'
							}, {
								xtype : 'textfield',
								fieldLabel : '规格',
								width : 250,
								name : 'standard'
							}, {
								xtype : 'textfield',
								fieldLabel : '体积(长×宽×高)',
								width : 250,
								name : 'volume'
							}, {
								xtype : 'textfield',
								fieldLabel : '功率(含单位)',
								width : 250,
								name : 'power'
							}, {
								xtype : 'textfield',
								fieldLabel : '重量(含单位)',
								width : 250,
								name : 'weight'
							}, {
								xtype : 'panel',
								layout : 'form',
								border : false,
								items : [{
									xtype : 'panel',
									border : false,
									layout : 'column',
									fieldLabel : '官方报价',
									labelAlign : 'right',
									items : [
											{
												border : false,
												layout : 'form',
												hideLabels : true,
												items : [{
															xtype : 'formatnum',
															name : 'referencePrice',
															width : 150
														}]
											},
											new com.bitc.xmis.Combos.CurrencyCombo(
													{
														ref : 'currency',
														margins : '0 0 0 5',
														width : 100,
														hiddenName : 'currency'
													})]
								}]
							}, new Ext.ux.form.ChooseField({
										fieldLabel : '生产厂家',
										name : 'materialsManufacturer',
										width : 250,
										winConfig : {
											title : '生产厂家列表'
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
											if (Ext.isEmpty(o)) {
												Ext.Msg.alert('提示',
														'您还没有选择，请选择');
											}
											materialFromPanel.getForm()
													.findField('manufacturer')
													.setValue(o.data.公司ID);
											materialFromPanel
													.getForm()
													.findField('materialsManufacturer')
													.setValue(o.data.公司名称);
										},
										clearValueFun : function() {
										}
									}), {
								xtype : 'textfield',
								fieldLabel : '生产厂家ID',
								name : 'manufacturer',
								hidden : true
							}, {
								xtype : 'panel',
								layout : 'form',
								border : false,
								id : 'cccc',
								items : [{
									xtype : 'panel',
									layout : 'form',
									border : false,
									items : [{
										xtype : 'textfield',
										fieldLabel : '厂家商品链接网址',
										name : 'factoryCommodityAddress',
										width : 250,
										listeners : {
											render : function() {// 渲染后添加click事件,双击超链接
												Ext.fly(this.el).on('dblclick',
														function(e, t) {
															window
																	.open(dfo.factoryCommodityAddress);
														});
											}
										}
									}]
								}]
							}, {
								xtype : 'panel',
								layout : 'form',
								border : false,
								items : [{
									xtype : 'panel',
									border : false,
									layout : 'column',
									fieldLabel : '物资自身属性',
									labelAlign : 'right',//配置字典表地址在ext-extra.js
									items : [new com.bitc.xmis.Combos.PropertyCombo(
											{
												margins : '0 0 0 5',
												width : 250,
												hiddenName : 'materialItself'
											})]
								}]
							}, {
								xtype : 'panel',
								layout : 'form',
								border : false,
								items : [{
									xtype : 'panel',
									border : false,
									layout : 'column',
									fieldLabel : '物资业务属性',
									labelAlign : 'right',
									items : [new com.bitc.xmis.Combos.BusinessCombo(
											{
												margins : '0 0 0 5',
												width : 250,
												hiddenName : 'materialsBusiness'
											})]
								}]
							}, {
								xtype : 'panel',
								layout : 'form',
								baseCls : 'x-plain',
								border : false,
								items : [{
									xtype : 'panel',
									border : false,
									layout : 'column',
									fieldLabel : '物资用途',
									labelAlign : 'right',
									baseCls : 'x-plain',
									items : [new com.bitc.xmis.Combos.MaterialsUseCombo(
											{
												margins : '0 0 0 5',
												width : 250,
												hiddenName : 'materialUse'
											})]
								}]
							}, {
								xtype : 'datefield',
								fieldLabel : '投入市场日期',
								name : 'listingDate',
								format : 'Y-m-d',
								width : 250,
								listeners : {
									select : function(combo, record, value) {
										var tt = new Date().format('Y-m-d');
										if (combo.value <= tt) {
											materialFromPanel.getForm()
									.findField('produceStatus1').setValue('在产')
										}else{
										Ext.Msg.alert("提示",
													"投入市场日期在未来时间请检查是否正确");
										}
									}
								}
							}, {
								xtype : 'datefield',
								fieldLabel : '停产日期',
								name : 'stopTime',
								format : 'Y-m-d',
								width : 250,
								listeners : {
									select : function(combo, record, value) {
										var tt = new Date().format('Y-m-d');
										if (combo.value <= tt) {
											materialFromPanel.getForm()
									.findField('produceStatus1').setValue('停产')
										} else {
											Ext.Msg.alert("提示",
													"停产日期在未来时间请检查是否正确");
											materialFromPanel.getForm()
									.findField('produceStatus1').setValue('在产')
										}
									}
								}
							}, {
								fieldLabel : '物资生产状态',
								xtype : 'displayfield',
								name : 'produceStatus1'
							},{
								xtype : 'panel',
								layout : 'column',
								border : false,
								style : 'padding:0px 5px 0px 2px;',
								items : [{
											xtype : 'panel',
											layout : 'form',
											labelAlign : 'right',
											labelWidth : 60,
											border : false,
											items : [{
														fieldLabel : '最新价格',
														name : 'lastPrice',
														xtype : 'displayfield'
													}, {
														fieldLabel : '最低价格',
														name : 'lowestPrices',
														xtype : 'displayfield'
													}, {
														xtype : 'displayfield',
														fieldLabel : '照片名称',
														name : 'pigNamehidde',
														hidden : true
		
													}]
										}, {
											xtype : 'panel',
											layout : 'form',
											labelAlign : 'right',
											labelWidth : 10,
											border : false,
											items : [{
														fieldLabel : '',
														xtype : 'displayfield',
														name : 'lastPriceTime'
													}, {
														fieldLabel : '',
														xtype : 'displayfield',
														name : 'lowestPricesTime'
													}]
										}, {
											xtype : 'panel',
											layout : 'form',
											labelAlign : 'right',
											border : false,
											hidden : true,
											items : [{
														fieldLabel : '最新价格采购合同ID',
														xtype : 'displayfield',
														name : 'lastPricePurchaseID'
													}, {
														fieldLabel : '最低价格采购合同ID',
														xtype : 'displayfield',
														name : 'lowestPricesPurchaseID'
													}]
										}, {
		
											xtype : 'panel',
											layout : 'form',
											labelAlign : 'right',
											labelWidth : 10,
											border : false,
											items : [{
														fieldLabel : '',
														xtype : 'displayfield',
														name : 'lastPricePurchaseCode'
													}, {
														fieldLabel : '',
														xtype : 'displayfield',
														name : 'lowestPricesPurchaseCode'
													}]
		
										}]
							}, {

								xtype : 'panel',
								layout : 'column',
								border : false,
								items : [{
											xtype : 'panel',
											layout : 'form',
											Width : 400,
											labelAlign : 'right',
											labelWidth : 80,
											border : false,
											items : [{
														fieldLabel : '登记人',
														name : 'newregisterName',
														xtype : 'displayfield',
														value : empName
													}, {
														fieldLabel : '最后修改人',
														name : 'lastModifier',
														xtype : 'displayfield'
													}]
										}, {
											xtype : 'panel',
											layout : 'form',
											Width : 400,
											labelAlign : 'right',
											labelWidth : 80,
											border : false,
											items : [{
												fieldLabel : '登记日期',
												xtype : 'displayfield',
												name : 'newupdateDate',
												value : new Date()
														.format('Y-m-d')
											}, {
												fieldLabel : '最后修改日期',
												xtype : 'displayfield',
												name : 'lastModified'
											}]
										}]

							}]
				}, {
					// 第二列
					width : 500,
					border : false,
					items : [{
						xtype : 'fieldset',
						title : '图片预览',
						name : 'pigName',
						style : 'padding :5px 3px 5px 3px; margin:0 0 0 0px ;!important',
						height : 360,
						width : 330,
						items : [imgs]
					}, {
						xtype : 'uploadImgButton',
						EMPIDFun : function() {
							return materialsID == null ? 0 : materialsID;
						},
						setImgNameFun : function(name) {
							materialFromPanel.getForm()
									.findField('pigNamehidde').setValue(name);
						},
						IMGName : imgs,
						ISPOSTCARD : 'deviceImg',
						disabled : true,
						reuses : false,
						text : '上传图片',
						id : 'imgs_btn',
						style : 'padding:10px 5px 0px 32px;',
						iconCls : 'folder_image'
					}]
				}]
			}]
		}],
		tbar : {
			xtype : 'toolbar',
			items : [{
				xtype : 'button',
				text : '保存',
				iconCls : 'ok',
				handler : function() {
					if (!Ext.isEmpty(materialFromPanel.getForm()
							.findField('stopTime').getValue())
							&& materialFromPanel.getForm()
									.findField('listingDate').getValue() > materialFromPanel
									.getForm().findField('stopTime').getValue()) {
						Ext.Msg.alert("提示", "停产日期在投入市场日期之前请检查是否正确");
						return;
					}
					materialFromPanel.getForm().submit({//保存修改物资库方法
						url : __cxtPath
								+ '/device.do?method=addMaterialInformation',
						params : {
							officialQuotation : materialFromPanel.getForm()
									.findField('referencePrice').getValue(),
							currencyType : materialFromPanel.getForm()
									.findField('currency').getValue(),
							pigName : materialFromPanel.getForm()
									.findField('pigNamehidde').getValue(),
							materialsManufacturerID : materialFromPanel
									.getForm().findField('manufacturer')
									.getValue()
						},
						waitMsg : '正在保存，请稍候……',
						waitTitle : '正在处理',
						success : function(form, action) {
							Ext.MessageBox.hide();
							var id = action.result.attributes.id;
							var name = action.result.attributes.name;
							Ext.MessageBox.alert(action.result.messageTitle,
									action.result.message, function() {
										parent.xMis
												.turn(
														'company-info-' + id,
														"[物资]" + name,
														__cxtPath
																+ '/device.do?method=getMaterialnfo&id='
																+ id,
														'menu-company-info',
														true);
									if(materialsID == null){
										parent.xMis.closeTab(window);
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
				xtype : 'button',
				text : '刷新',
				iconCls : 'reload',
				ref : '../refreshButton',
				handler : function() {
					location.reload();
				}
			}, '-', {
				xtype : 'uploadButton',
				text : '附件上传',
				reuses : false,
				hidden : true,
				id : 'attButton',
				REFIDFun : function() {
					eastPanelComp.setActiveTab(2);
					return materialsID;
				},
				attachmentTypeID : com.bitc.xmis.Attachment.materials,
				uploadPanelConfig : {
					listeners : {
						'uploadcomplete' : function() {
							attFileGrid.reload();
						}
					}
				}
			}]
		}
	});

	/**
	 * 最大供应商
	 */
	var maximumGYS = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({
						width : 25
					}), {
				header : '供应商',
				dataIndex : '公司名称',
				sortable : true,
				width : 230
			},  {
				header : '数量',
				dataIndex : '数量',
				sortable : true,
				width : 60
			}]);
	var reRecordGYS = Ext.data.Record.create([{
				name : '项目物资ID'
			}, {
				name : '公司名称'
			}, {
				name : '乙方公司ID'
			}, {
				name : '数量'
			}]);
	var storeGYS = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : __cxtPath
									+ '/device.do?method=getMaximumSupplier',//查询最大供应商
							method : "POST"
						}),
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount'
						}, reRecordGYS),
				remoteSort : true,
				baseParams : {
					materialsID : materialsID == null ? 0 : materialsID,
					start : 0,
					limit : 20
				},
				autoLoad : true
			});
	var supplierGird = new Ext.grid.GridPanel({
				region : 'center',
				title : '供应商',
				border : false,
				loadMask : true,
				loadMask : {
					msg : '正在加载数据，请稍侯……'
				},
				stripeRows : true,
				store : storeGYS,
				cm : maximumGYS
			});
	/**
	 * 最多次使用项目
	 */
	var mostXM = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({
						width : 25
					}), {
				header : '项目编码',
				dataIndex : '项目编码',
				sortable : true,
				width : 230
			}, {
				header : '数量',
				dataIndex : '数量',
				sortable : true,
				width : 60
			}]);
	var reRecordXM = Ext.data.Record.create([{
				name : '项目物资ID'
			}, {
				name : '项目ID'
			}, {
				name : '项目名称'
			},{
				name : '项目编码'
			
			}, {
				name : '数量'
			}]);
	var storeXM = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : __cxtPath + '/device.do?method=getUseProject',//查询项目最多使用项目
							method : "POST"
						}),
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount'
						}, reRecordXM),
				remoteSort : true,
				baseParams : {
					materialsID : materialsID == null ? 0 : materialsID,
					start : 0,
					limit : 20
				},
				autoLoad : true
			});
	var useProject = new Ext.grid.GridPanel({
				region : 'center',
				title : '使用项目',
				border : false,
				loadMask : true,
				loadMask : {
					msg : '正在加载数据，请稍侯……'
				},
				stripeRows : true,
				store : storeXM,
				cm : mostXM
			});

	var eastPanelComp = new Ext.TabPanel({
		id : 'eastPanelComp',
		region : 'east',
		width : 380,
		border : false,
		split : true,
		autoScroll : true,
		collapsible : true, // 是否有右上方展开收缩条
		collapseMode : 'mini', // 将滚动收缩条模式‘迷你’
		stateful : true,
		stateId : 'editContract-eastpanel',
		stateEvent : ['resize'],
		activeTab : 0,
		items : [supplierGird, useProject, attFileGrid]
		});

	new Ext.Viewport({
				layout : 'border',
				items : [materialFromPanel, eastPanelComp]
			});

	materialFromPanel.getForm().setValues(map);

	if (dfo == null) {

	}

	if (dfo != null) {
		materialFromPanel.getForm().setValues(dfo);
		Ext.getCmp('imgs_btn').setDisabled(false);
		Ext.getCmp('attButton').show();
		materialFromPanel.getForm().findField('currency')
				.setValue(dfo.currency.id);
				
		materialFromPanel.getForm().findField('materialsBusiness')
				.setValue(dfo.materialsBusiness.id);
		materialFromPanel.getForm().findField('materialItself')
				.setValue(dfo.materialItself.id);
		materialFromPanel.getForm().findField('materialUse')
				.setValue(dfo.materialUse.id);
		materialFromPanel.getForm().findField('manufacturer')
				.setValue(dfo.manufacturer.companyID);
		materialFromPanel.getForm().findField('materialsManufacturer')
				.setValue(dfo.manufacturer.companyName);
		materialFromPanel.getForm().findField('lastModifier')
				.setValue(dfo.lastModifier.userName);
		materialFromPanel.getForm().findField('newregisterName')
				.setValue(dfo.newregisterName.userName);
		materialFromPanel.getForm().findField('lastModified')
				.setValue((dfo.lastModified).substring(0, 10));
		materialFromPanel.getForm().findField('newupdateDate')
				.setValue((dfo.newupdateDate).substring(0, 10));
		
		materialFromPanel.getForm().findField('pigNamehidde')
				.setValue(dfo.pigName);
		if(!Ext.isEmpty(dfo.pigName)){//显示照片
			imgs.setSrc(__cxtPath
					+ "/fileUpload.do?type=7&isPostCard=deviceImg&userPic=/"
					+ dfo.pigName);
		}else{
//			imgs.pigName('postCard.png');
		}		
				
				if(!Ext.isEmpty(dfo.stopTime)){
		materialFromPanel.getForm().findField('stopTime')
				.setValue((dfo.stopTime).substring(0, 10));
				}else{
				materialFromPanel.getForm().findField('stopTime')
				.setValue(dfo.stopTime);
				}
				if(!Ext.isEmpty(dfo.listingDate)){
				materialFromPanel.getForm().findField('listingDate')
				.setValue((dfo.listingDate).substring(0, 10));
				}else{
				materialFromPanel.getForm().findField('listingDate')
				.setValue(dfo.listingDate);
				}
				
			if(dfo.stopTime >= dfo.lastModified && dfo.stopTime > dfo.listingDate){
				materialFromPanel.getForm().findField('produceStatus1')
				.setValue('在产');
				}
				if(Ext.isEmpty(dfo.stopTime) && dfo.listingDate <= dfo.lastModified){
					materialFromPanel.getForm().findField('produceStatus1')
				.setValue('在产');
				}
				if(dfo.stopTime <= dfo.lastModified && !Ext.isEmpty(dfo.listingDate)){
				materialFromPanel.getForm().findField('produceStatus1')
				.setValue('停产');
				}
				if(Ext.isEmpty(dfo.listingDate) && dfo.stopTime <= dfo.lastModified){
					materialFromPanel.getForm().findField('produceStatus1')
				.setValue('停产');
				}	
				if(Ext.isEmpty(dfo.listingDate) && dfo.stopTime > dfo.lastModified){
					materialFromPanel.getForm().findField('produceStatus1')
				.setValue('在产');
				}
				if(dfo.listingDate>dfo.lastModified && dfo.stopTime > dfo.lastModified){
					materialFromPanel.getForm().findField('produceStatus1')
				.setValue('');
				}
				
	} else {
		materialFromPanel.getTopToolbar().enable();
		// materialFromPanel.getForm().findField('pigNamehidde')
		// .setValue(dfo.pigName);
	}
	if (map != null) {
		if(!decisionFlag){
				materialFromPanel.getForm().findField('lastPrice')
				.setValue('<div style="margin-top:5px"><font color =red>权限不足</font></div>');
		materialFromPanel.getForm().findField('lowestPrices')
				.setValue('<div style="margin-top:5px"><font color =red>权限不足</font></div>');
		
		}else{
		materialFromPanel.getForm().findField('lastPrice')
				.setValue(map.lastPrice);
		materialFromPanel.getForm().findField('lowestPrices')
				.setValue(map.lowestPrices);
		}
			if(!Ext.isEmpty(map.lastPriceTime)){
				materialFromPanel.getForm().findField('lastPriceTime')
				.setValue((map.lastPriceTime).substring(0, 10));
			}else{
//		materialFromPanel.getForm().findField('lastPriceTime')
//				.setValue(map.lastPriceTime);
			}	
			if(!Ext.isEmpty(map.lowestPricesTime)){
				materialFromPanel.getForm().findField('lowestPricesTime')
				.setValue((map.lowestPricesTime).substring(0, 10));
			}else{
//		materialFromPanel.getForm().findField('lowestPricesTime')
//				.setValue(map.lowestPricesTime);
			}
	}
});
