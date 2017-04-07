Ext.onReady(function() {
			var paymentTotal = 0;
			var projectSdFlag = 0;// 当projectStuatus列表中，选择了自定义的行，则
			// projectSdFlag为1,或修改一行并这行在projectStatusGrid中这行的Selfdefine=ture;
			var projectSelFlag = 0;
			var projectNameChangeFlag = 0;// 仅当出现项目状态名冲突的时候，提示窗口选否，值置1
			var projectNameChangeActionFlag = 0;// 当改变过项目状态，再点查询时，不发生change 事件
			var paymentTotalAmt = 0.00; // 金额总额
			var currentIndex = -1; // 当前选择的行
			
			var flag = false;
			
			var conOpType;//合同的操作类型，判断是否有‘单击编辑’权限    conOpType!=1，没有权限

			var paymentSt = Ext.data.Record.create([ {
				name : paymentConditionId
			}, {
				name : paymentConditionDescription
			}, {
				name : paymentConditionProjStatusID
			}, {
				name : paymentConditionProjStatusName
			}, {
				name : paymentConditionDelayDays
			}, {
				name : paymentConditionPaymentDate
			}, {
				name : paymentConditionDateDisplayed,
				type : 'date',
				format : 'Y-m-d'
			}, {
				name : paymentConditionPercent
			}, {
				name : paymentConditionPaymentAmt,
				type : 'number'
			}, {
				name : paymentConditionManualFlag,
				type : 'bool'
			}, {
				name : paymentConditionManualReason
			}, {
				name : realIncome,
				type : 'float'
			}, {
				name : paymentConditionPaymentNodeName
			}, {
				name : paymentConditionShouldPaymentDate,
				type : 'date',
				format : 'Y-m-d'
			}, {
				name : realDate,
				type : 'date'
			}, {
				name : realStartDate,
				type : 'date',
				format : 'Y-m-d'
			}, {
				name : 'start_date',
				type : 'date',
				format : 'Y-m-d'
			}, {
				name : cID
			} ]);

			var projectScheduleEditPanel = new com.bitc.xmis.project.ProjectScheduleEditPanel(
					{
						region:'center',
						projectId : projectId,
						border : false,
						scheduleTitle : '项目进度计划(双击编辑日期)',
						statusTitle : '可用于进度计划的项目状态<br/>（拖动一个状态到进度计划中）'
					});

			var projectWin = new Ext.Window({
				title : '创建项目进度计划',
				modal : true,
				width : 940,
				height : 452,
				autoScroll : true,
				closeAction : 'hide',
				layout:'border',
				items : [ projectScheduleEditPanel ],
				listeners : {
					'show' : function() {
						projectScheduleEditPanel.initDragDrop();
					}
				}
			});

			var paymentStore = new Ext.data.Store({
				url : __cxtPath
						+ '/contractMain.do?method=getPaymentConditionList',
				pruneModifiedRecords : true,
				reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount',
					remoteSort : true,
					fields : paymentSt
				}),
				listeners : {
					'beforeload' : function(){
						//判断合同的操作类型
						if(typeof (contractOpType) == 'undefined') {
							Ext.Ajax.request({
								url : __cxtPath
										+'/contractMain.do?method=getContractOpType',
								params : {
									contractID : contractIdInPay
								},
								success : function(response,opt){
									var obj = Ext.decode(response.responseText);
									var type = obj.attributes.contractOpType;
									if(type ==5){
										paymentConditionGrid.getTopToolbar().enable();
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
							if(contractOpType == 5){
								paymentConditionGrid.getTopToolbar().enable();
								conOpType = 1;
							}
						}
					}
				}
			});

			var searchProjField = new Ext.form.ComboBox(
					{
						emptyText : '请选择项目节点',
						editable : false,
						triggerAction : "all",
						store : projectScheduleEditPanel.schedulePlanGrid.store,
						displayField : 'statusName',
						valueField : 'statusName',
						mode : "remote",
						width : 180,
						listeners : {
							'select' : function(c, r, row) {
								var records = paymentStore.getRange();
								var flag = true;
								for ( var i = 0; i < records.length; i++) {
									if (records[i]
											.get(paymentConditionProjStatusID) == r
											.get('statusId')) {
										if (currentIndex != i) {
											flag = false;
											c.clearValue();
											paymentStore.getRange().remove(i);
											paymentStore
													.getAt(currentIndex)
													.set(
															paymentConditionProjStatusID,
															'');
											Ext.Msg
													.alert(
															"提示",
															"该项目节点已被使用，请重新选择！",
															function() {
																paymentConditionGrid
																		.startEditing(
																				currentIndex,
																				2);
															});
										}
									}
								}
								if (flag) {
									paymentStore.getAt(currentIndex).set(
											paymentConditionProjStatusID,
											r.get('statusId'));
									var day = paymentStore.getAt(currentIndex)
											.get(paymentConditionDelayDays);
									if (day == undefined || day == "undefined") {
										day = 0;
									}
									paymentStore.getAt(currentIndex).set(
											paymentConditionDateDisplayed,
											r.get('startDate').add(Date.DAY,
													parseInt(day)));
									paymentStore.getAt(currentIndex).set(
											'start_date', r.get('startDate'));
									if (r.get('actualStartDate') != null) {
										paymentStore
												.getAt(currentIndex)
												.set(
														paymentConditionShouldPaymentDate,
														r
																.get(
																		'actualStartDate')
																.add(
																		Date.DAY,
																		parseInt(day)));
										paymentStore.getAt(currentIndex).set(
												realStartDate,
												r.get('actualStartDate'));
										paymentStore.getAt(currentIndex).set(
												realDate,
												r.get('actualStartDate'));
									} else {
										paymentStore
												.getAt(currentIndex)
												.set(
														paymentConditionShouldPaymentDate,
														null);
										paymentStore.getAt(currentIndex).set(
												realStartDate, null);
										paymentStore.getAt(currentIndex).set(
												realDate, null);
									}
								}

							}
						}
					});

			var paymentNameField = new Ext.form.TextField({
				allowBlank : false,
				emptyText : "必输项..."
			});

			var paymentDelayDaysField = new Ext.form.NumberField(
					{
						value : 0,
						minValue : 0,
						maxValue : 10000,
						decimalPrecision : 0,
						allowDecimals : false,
						nanText : "请输入整数",
						enableKeyEvents : true,
						allowBlank : false,
						listeners : {
							'change' : function(f, newValue, oldValue) {
								var tt = paymentStore.getAt(currentIndex).get(
										paymentConditionDateDisplayed);
								if (typeof (tt) == "string") {
									if (tt.indexOf('/') > 0) {
										var d = new Date(tt);
										if (typeof (newValue) != "string") {
											paymentStore
													.getAt(currentIndex)
													.set(
															paymentConditionDateDisplayed,
															d
																	.add(
																			Date.DAY,
																			parseInt(newValue
																					- oldValue)));
										}
									} else {
										if (tt != "") {
											var time = tt.split(" ");
											var t = time[0].split("-");
											var dt = t[1] + "/" + t[2] + "/"
													+ t[0];
											var d = new Date(dt);
											if (typeof (newValue) != "string") {
												paymentStore
														.getAt(currentIndex)
														.set(
																paymentConditionDateDisplayed,
																d
																		.add(
																				Date.DAY,
																				parseInt(newValue
																						- oldValue)));
											}
										}
									}
								} else {
									if (typeof (newValue) != "string") {
										paymentStore.getAt(currentIndex).set(
												paymentConditionDateDisplayed,
												tt.add(Date.DAY,
														parseInt(newValue
																- oldValue)));
									}
								}

								var t1 = paymentStore.getAt(currentIndex).get(
										paymentConditionShouldPaymentDate);
								if (typeof (t1) == "string") {
									if (t1.indexOf('/') > 0) {
										var dd = new Date(t1);
										if (typeof (newValue) != "string") {
											paymentStore
													.getAt(currentIndex)
													.set(
															paymentConditionShouldPaymentDate,
															dd
																	.add(
																			Date.DAY,
																			parseInt(newValue
																					- oldValue)));
										}
									} else {
										if (t1 != "") {
											var times = t1.split(" ");
											var ta = times[0].split("-");
											var dt1 = ta[1] + "/" + ta[2] + "/"
													+ ta[0];
											var dd = new Date(dt1);
											if (typeof (newValue) != "string") {
												paymentStore
														.getAt(currentIndex)
														.set(
																paymentConditionShouldPaymentDate,
																dd
																		.add(
																				Date.DAY,
																				parseInt(newValue
																						- oldValue)));
											}
										}
									}
								} else {
									if (typeof (newValue) != "string") {
										paymentStore
												.getAt(currentIndex)
												.set(
														paymentConditionShouldPaymentDate,
														t1
																.add(
																		Date.DAY,
																		parseInt(newValue
																				- oldValue)));
									}
								}

							}
						}
					});
			var projectIdField = new Ext.form.TextField({});
			var paymentPersentField = new Ext.form.NumberField({
				minValue : 0,
				decimalPrecision : 2,
				allowDecimals : true,
				enableKeyEvents : true
			});

			var paymentAmtField = new Ext.form.NumberField({
				minValue : 0,
				allowBlank : false
			});

			var paymentConditionGrid = new Ext.grid.EditorGridPanel(
					{
						title : "收款计划",
						store : paymentStore,
						region : 'center',
						border : false,
						stripeRows : true, //True表示使用不同颜色间隔行，默认为false
						enableColumnResize : true,
						loadMask : {
							msg : '正在加载数据，请稍侯……'
						},
						viewConfig : {
							getRowClass : function(record, rowIndex, rowParams,
									store) {
								var hasRealStartDate = false;
								var rIndex = 0;
								if (store.getCount() > 0) {
									var records = store.getRange();
									for ( var i = 0; i < records.length; i++) {
										if (records[i].get(realDate) != null
												&& records[i].get(realDate) <= new Date()) {
											rIndex = i;
											hasRealStartDate = true;
										}
									}
								}
								if (hasRealStartDate && rowIndex <= rIndex) {
									return 'font-lightgreen';
								}
							}
						},
						selModel : new Ext.grid.RowSelectionModel({}),
						collapsible : collapsible,
						clicksToEdit : 1,
						listeners : {
							'afterrender' : function() {
								if (paymentCondiFlag == 1) {
									this.getTopToolbar().hide();
								}
							},
							'expand' : function() {
								paymentConditionGrid.focus("true", 100);
							},
							'cellclick' : function(g, rowIndex, columnIndex, e) {
								currentIndex = rowIndex;
							},
							'afteredit' : function(e) {
								if (e.field == '项目状态') {
									paymentStore.sort('start_date', 'ASC');
								}
								if(e.field == '收款百分比'){
									e.record.set(paymentConditionPaymentAmt,
											contractAmtInPay * e.value / 100);
								}
								if(e.field == '收款金额'){
									e.record.set(paymentConditionPercent,Math.round(e.value
											/ contractAmtInPay * 10000) / 100);
								}
							},
							'columnresize' : function(columnIndex,newSize){
								if(columnIndex == 4)
									paymentConditionGrid.getView().refresh(false);
							},
							'beforeedit' : function(){
								if( conOpType != 1){
									return false;
								}
							}
						},
						tbar : {
							xtype : 'toolbar',
							disabled : true,
							items : [{
											ref : '../newBtn',
											iconCls : 'add',
											text : '新建',
											handler : function() {
												if (projectScheduleEditPanel.schedulePlanGrid.store
														.getCount() == 0) {
													Ext.Msg.alert("提示", "请先创建项目进度计划!");
												} else if (projectScheduleEditPanel.schedulePlanGrid.store
														.getCount() != 0
														&& projectScheduleEditPanel.schedulePlanGrid.store
																.getCount() == paymentStore
																.getCount()) {
													Ext.Msg
															.alert("提示",
																	"创建的项目进度计划已全部使用，请先创建项目进度计划!");
												} else {
													var Plant = paymentConditionGrid
															.getStore().recordType;
													var p = new Plant({});
													paymentConditionGrid.stopEditing();
													paymentConditionGrid.getStore()
															.add([ p ]);
													paymentConditionGrid
															.startEditing(paymentStore
																	.getCount() - 1, 2);
													currentIndex = paymentStore
															.getCount() - 1;
													paymentConditionGrid.getSelectionModel().selectRow(currentIndex);
													if (projectScheduleEditPanel.schedulePlanGrid.store
															.getCount() == paymentStore
															.getCount())
														paymentConditionGrid.newBtn
																.setDisabled(true);
												}
											}
										},
										'-',
										{
											ref : '../saveBtn',
											iconCls : 'save',
											text : '保存',
											handler : function() {
												paymentConditionGrid.stopEditing(false);
												var records = paymentStore
														.getModifiedRecords();
												var sfsuccess = true;
												if (records.length > 0) {
													for (i = 0; i < paymentStore
															.getCount(); i++) {
														var r = paymentStore.getAt(i);
														if (r
																.get(paymentConditionProjStatusName) == null
																|| Ext
																		.isEmpty(r
																				.get(paymentConditionProjStatusName))) {
															row = i;
															Ext.Msg
																	.alert(
																			"提示",
																			"对应项目节点不能为空!",
																			function() {
																				paymentConditionGrid
																						.startEditing(
																								row,
																								2);
																				currentIndex = row;
																			});
															sfsuccess = false;
															return;
														}
														if (r
																.get(paymentConditionPaymentNodeName) == null
																|| Ext
																		.isEmpty(r
																				.get(paymentConditionPaymentNodeName))) {
															row = i;
															Ext.Msg
																	.alert(
																			"提示",
																			"收款节点名称不能为空!",
																			function() {
																				paymentConditionGrid
																						.startEditing(
																								row,
																								3);
																				currentIndex = row;
																			});
															sfsuccess = false;
															return;
														} else {
															if (r
																	.get(paymentConditionPaymentNodeName).length > 20) {
																row = i;
																Ext.Msg
																		.alert(
																				"提示",
																				"收款节点名称最多20个汉字，请检查！",
																				function() {
																					paymentConditionGrid
																							.startEditing(
																									row,
																									3);
																					currentIndex = row;
																				});
																sfsuccess = false;
																return;
															}
														}
														if (r
																.get(paymentConditionPaymentAmt) == null
																|| Ext
																		.isEmpty(r
																				.get(paymentConditionPaymentAmt))) {
															row = i;
															Ext.Msg
																	.alert(
																			"提示",
																			"预计到款金额不能为0!",
																			function() {
																				paymentConditionGrid
																						.startEditing(
																								row,
																								6);
																				currentIndex = row;
																			});
															sfsuccess = false;
															return;
														}
														if (r.get(paymentConditionDescription)!= null && !Ext.isEmpty(r
																.get(paymentConditionDescription)) && r.get(paymentConditionDescription).length > 200) {
															row = i;
															Ext.Msg
																	.alert(
																			"提示",
																			"收款条件描述最多200个汉字，请检查！",
																			function() {
																				paymentConditionGrid
																						.startEditing(
																								row,
																								4);
																				currentIndex = row;
																			});
															sfsuccess = false;
															return;
														}
													}

													if (sfsuccess) {
														var storeStr = com.bitc.xmis.util.ExtUtil.encodeStoreModifiedData(paymentStore);
														Ext.Ajax
																.request({
																	url : String
																			.format(
																					'{0}/contractMain.do?method=savePaymentCondition1',
																					__cxtPath),
																	params : {
																		contractID : contractIdInPay,
																		records : storeStr
																	},
																	scope : this,
																	success : function(
																			response,
																			opt) {
																		var result = Ext
																				.decode(response.responseText);
																		if (result.success) {
																			Ext.Msg.alert(
																					result.messageTitle,
																					result.message);
																			paymentStore
																					.reload();
																			paymentStore
																					.rejectChanges();
																		} else {
																			Ext.Msg.alert(
																					result.messageTitle,
																					result.message);
																		}
																		paymentConditionGrid.newBtn
																				.setDisabled(false);
																	},
																	failure : Ext.emptyFn,
																	waitMsg : '正在保存，请稍候……',
																	waitTitle : '保存'
																});
													}
												} else {
													Ext.Msg
															.alert("提示",
																	"您没有任何操作，无需保存！");
												}
											}

										},
										'-',
										{
											ref : '../removeBtn',
											iconCls : 'remove',
											text : '删除',
											disabled : false,
											handler : function() {
												var record = paymentConditionGrid
														.getSelectionModel()
														.getSelected();
												if (typeof (record) == 'undefined') {
													Ext.Msg.alert("未选择", "请先选择！");
													return true;
												} else {
													if (typeof (record
															.get(cID)) == 'undefined') {
														Ext.Msg.confirm("请确认", "确定要删除该条付款条件吗？",
																function(btn) {
																	if (btn == 'yes') {
																		paymentStore.remove(record);
																		paymentConditionGrid.newBtn.setDisabled(false);
																	}
															});
													} else {
														Ext.Msg
																.confirm(
																		"请确认",
																		"确定要删除该条付款条件吗？",
																		function(btn) {
																			if (btn == 'yes') {
																				paymentForm
																						.getForm()
																						.submit(
																								{
																									url : __cxtPath
																											+ '/contractMain.do?method=delPaymentCondition',
																									method : 'post',
																									params : {
																										contractID : contractIdInPay,
																										conditionID : record
																												.get(paymentConditionId)
																									},
																									success : function(
																											form,
																											action) {
																										Ext.Msg
																												.alert(
																														action.result.messageTitle,
																														action.result.message);
																										paymentConditionGrid
																												.getStore()
																												.reload();
																										paymentConditionGrid
																												.getView()
																												.refresh();
																										currentIndex = -1;
																									},
																									failure : function(
																											form,
																											action) {
																										Ext.Msg
																												.alert(
																														action.result.messageTitle,
																														action.result.message);
																									}
																								});
																			}
																		});
														paymentConditionGrid.newBtn
																.setDisabled(false);
													}
												}
											}
										},
										'-',
										{
											ref : '../changeBtn',
											iconCls : 'menu-contract-edit',
											text : '创建项目进度',
											handler : function() {
												projectWin.show();
											}
										},
										'-',
										{
											iconCls : 'reload',
											ref : '../refreshButton',
											text : '刷新',
											handler : function(tabs, item) {
												paymentStore.reload();
											}
										},
										'->',
										'<table><tr><td>图例:</td><td style="background-color : green;">&nbsp;&nbsp;&nbsp;&nbsp;</td><td style="color : green;">已执行完项目进度</td><td style="background-color : #000000;">&nbsp;&nbsp;&nbsp;&nbsp;</td><td>未执行项目进度</td><td style="color : #0000FF;">*单击进入编辑状态</td></tr></table>' ]
						},
						colModel : new Ext.grid.ColumnModel(
								{

									columns : [
											new Ext.grid.RowNumberer({
												width : 23
											}),
											{
												header : '项目状态ID',
												dataIndex : paymentConditionProjStatusID,
												hidden : true,
												width : 20,
												sortable : false,
												editor : projectIdField
											},
											{
												header : '对应项目节点',
												dataIndex : paymentConditionProjStatusName,
												sortable : false,
												editor : searchProjField,
												renderer : function(value,
														cellMeta, record,
														rowIndex, columnIndex,
														store) {
													if (value == null
															|| (value != null && value
																	.trim() == "")) {
														return '<font color="#C0C0C0">请选择项目节点</font>';
													}
													return value;
												}
											},
											{
												header : '收款节点名称',
												dataIndex : paymentConditionPaymentNodeName,
												sortable : false,
												renderer : function(value,
														cellMeta, record,
														rowIndex, columnIndex,
														store) {
													cellMeta.attr = 'ext:qtitle="收款节点名称" ext:qtip="'
															+ value + '"';
													if (value == null
															|| (value != null && value
																	.trim() == "")) {
														cellMeta.attr = 'ext:qtitle="收款节点名称" ext:qtip="例如:预付款"';
														return '<font color="#C0C0C0">例如:预付款</font>';
													}
													return value;
												},
												editor : new Ext.form.TextField(
														{
															allowBlank : false,
															emptyText : "必输项..."
														})
											},
											{
												id : 'payment_name',
												header : '收款条件描述',
												dataIndex : paymentConditionDescription,
												sortable : false,
												renderer : function(value,
														cellMeta, record,
														rowIndex, columnIndex,
														store) {
													if (value != null) {
														var baseNum = 18;
														var msg = '';
														for ( var i = 0; i < value.length
																/ baseNum; i++) {
															msg += value
																	.substr(
																			i
																					* baseNum,
																			baseNum)
																	+ "<br/>";
														}
														cellMeta.attr = 'ext:qtitle="收款条件描述" ext:qtip="'
																+ msg + '"';
													}
													if ((value != null && value
															.trim() == "")
															|| value == null) {
														cellMeta.attr = 'ext:qtitle="收款条件描述" ext:qtip="对收款节点名称的描述,可为空"';
														return '<font color="#C0C0C0">节点名称描述..</font>';
													}
													return Ext.util.Format.ellipsis(value, this.width/12 - 1 );
												},
												editor : new Ext.form.TextArea(
														{})
											},
											{
												xtype : 'numbercolumn',
												header : '收款百分比',
												align : 'right',
												width : 90,
												format : '00.00%',
												dataIndex : paymentConditionPercent,
												editor : paymentPersentField,
												sortable : false
											},
											{
												xtype : 'numbercolumn',
												align : 'right',
												format : '0,000.00',
												dataIndex : paymentConditionPaymentAmt,
												header : '预计到款金额',
												sortable : false,
												editor : paymentAmtField
											},
											{
												xtype : 'numbercolumn',
												align : 'right',
												format : '0,000.00',
												header : '财务实际到款',
												dataIndex : "realIncome",
												sortable : false
											},
											{
												header : '延迟天数',
												align : 'right',
												width : 75,
												dataIndex : paymentConditionDelayDays,
												sortable : false,
												editor : paymentDelayDaysField
											},
											{
												header : '预计到款时间',
												align : 'center',
												width : 85,
												dataIndex : paymentConditionDateDisplayed,
												renderer : function(value,
														cellMeta, record,
														rowIndex, columnIndex,
														store) {
													var msg = '预计到款时间由系统自动计算得出。计算规则为：<br>预计到款时间 = 项目节点预计进入日期 + 延迟天数';
													cellMeta.attr = 'ext:qtitle="预计到款时间" ext:qtip="'
															+ msg + '"';
													if (value == null) {
														return '<font color="#C0C0C0">系统自动计算!</font>';
													}
													return Ext.util.Format
															.date(value,
																	'Y-m-d');
												},
												sortable : false
											},
											{

												header : '节点实际进入日期',
												align : 'center',
												width : 110,
												dataIndex : realStartDate,
												renderer : function(value,
														cellMeta, record,
														rowIndex, columnIndex,
														store) {
													var msg = '项目节点实际进入日期';
													cellMeta.attr = 'ext:qtitle="节点进入日期" ext:qtip="'
															+ msg + '"';
													return Ext.util.Format
															.date(value,
																	'Y-m-d');
												},
												sortable : false
											},
											{
												header : '应收款日期',
												align : 'center',
												width : 85,
												dataIndex : paymentConditionShouldPaymentDate,
												renderer : function(value,
														cellMeta, record,
														rowIndex, columnIndex,
														store) {
													var msg = '应收款日期由系统自动计算得出。计算规则为：<br>应收款日期 = 项目节点实际进入日期 + 延迟天数';
													cellMeta.attr = 'ext:qtitle="应收款日期" ext:qtip="'
															+ msg + '"';
													if (value == null
															|| (value != null && value == "")) {
														return '<font color="#C0C0C0">系统自动计算!</font>';
													}
													return Ext.util.Format
															.date(value,
																	'Y-m-d');

												},
												sortable : false
											} ]
								})
					});
			this.setTbarDisable = function() {
				paymentConditionGrid.getTopToolbar().hide();

			};
			var paymentForm = new Ext.form.FormPanel({
				id : 'paymentConditionFrm',
				renderTo : paymentConditionDivName,
				layout : 'border',
				height : Ext.get(paymentConditionDivName).getHeight(),
				items : [ paymentConditionGrid ]
			});

			paymentStore.load({
				params : {
					contractID : contractIdInPay
				}
			});

			paymentStore
					.on(
							'load',
							function(store, records, options) {
								var arr = [];
								paymentTotal = 0;  //收款百分比
								paymentTotalAmt = 0.00;
								for ( var i = 0; i < records.length; i++) {
									var record = records[i];
									paymentTotal = paymentTotal
											+ record
													.get(paymentConditionPercent)
											* 1;
									paymentTotalAmt = parseFloat(paymentTotalAmt)
											+ parseFloat(record
													.get(paymentConditionPaymentAmt));
								}

								var msgtitle = '';
								if(paymentTotalAmt - contractAmtInPay < 0){
									var c = (paymentTotalAmt * 100) / contractAmtInPay;
									if(100 - c >= 1){
										msgtitle = String.format('<span style="float:left"><font color=red>{0}%合同金额未定义</span>',(100 - c).toFixed(2));
									}else{
										msgtitle = String.format('<span style="float:left"><font color=red>未定义合同金额: {0}</span>',Ext.util.Format.number((contractAmtInPay - paymentTotalAmt).toFixed(2),'0,000.00'));
									}
									if(Ext.getCmp('paymentConditionButton'))
										Ext.getCmp('paymentConditionButton')
											.setText('<font color=red>收款计划</font>');
								}else if(paymentTotalAmt - contractAmtInPay == 0){
									msgtitle = '<span style="float:left"><font color=green>收款计划完成</span>';
									if(Ext.getCmp('paymentConditionButton'))
									Ext.getCmp('paymentConditionButton').setText('收款计划');
								}else{
									msgtitle = String.format('<span style="float:left"><font color=red>收款计划定义超额: {0}</span>',Ext.util.Format.number((paymentTotalAmt - contractAmtInPay).toFixed(2),'0,000.00'));
									if(Ext.getCmp('paymentConditionButton'))
									Ext.getCmp('paymentConditionButton')
										.setText('收款计划');
								}
								
								
								
								

								var shouldPayedTotalAmt = 0.00; // 应到款总额
								var payedTotalAmt = 0.00; // 已到款总额
								var unpayedTotalAmt = 0.00; // 未到款总额

								var hasRealStartDate = false;
								var rIndex = 0;
								if (store.getCount() > 0) {
									var records = store.getRange();
									for ( var i = 0; i < records.length; i++) {
										if (records[i].get(realDate) != null
										&& records[i].get(realDate) <= new Date()) {
											rIndex = i;
											hasRealStartDate = true;
										}
									}
								}
								for ( var i = 0; i < records.length; i++) {
									var record = records[i];
									if (hasRealStartDate && i <= rIndex) {
										shouldPayedTotalAmt = parseFloat(shouldPayedTotalAmt)+ parseFloat(record.get(paymentConditionPaymentAmt));
										payedTotalAmt = parseFloat(payedTotalAmt)
												+ parseFloat(record
														.get(realIncome));
									}
								}
								unpayedTotalAmt = parseFloat(shouldPayedTotalAmt)- parseFloat(payedTotalAmt);
								var gl = undefined;
								gl = gls;
								if(gl=='null')
									gl = contract.incomAmtTotalByGl;
								paymentConditionGrid
										.setTitle(msgtitle
												+ '<span style="float:right"><font color=green>已收合同款:'
												+ Ext.util.Format
														.number(
																gl,
																'0,000.00')
												+ '</font>&nbsp;&nbsp;<font color=red>按进度应收账款:'
												+ Ext.util.Format.number(
														unpayedTotalAmt,
														'0,000.00')
												+ '</font></span>');

								if (paymentTotal > 0 && MM_findObj("amt")) {
									MM_findObj("amt").readOnly = true;
									amtReadOnly = true;
								}

								if (collapsible == false && collapsed == false) {
									paymentConditionGrid.removeBtn
											.setVisible(false);
									paymentConditionGrid.newBtn
											.setVisible(false);
									paymentConditionGrid.doLayout();
								}
							}, this, {
								delay : 100
							});

//			if (readOnly) {
//				paymentConditionGrid.getTopToolbar().hide(true);
//			}

		});