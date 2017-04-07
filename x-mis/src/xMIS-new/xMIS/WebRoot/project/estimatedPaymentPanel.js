Ext.ns('com.bitc.xmis.Records');
com.bitc.xmis.Records.ProjectPaymentConditionRecord = Ext.data.Record.create([{
			name : '对应项目状态',
			mapping : '对应项目状态'
		}, {
			name : '收款条件ID',
			mapping : '收款条件ID'
		}, {
			name : '收款条件描述',
			mapping : '收款条件描述'
		}, {
			name : '项目状态',
			mapping : '项目状态'
		}, {
			name : '延迟天数',
			mapping : '延迟天数'
		}, {
			name : 'payment_date',
			mapping : 'payment_date'
		}, {
			name : '收款金额',
			mapping : '收款金额',
			type : 'number'
		}, {
			name : 'realIncome',
			type : 'float'
		}, {
			name : '收款节点名称',
			mapping : '收款节点名称'
		}, {
			name : '应收款日期',
			mapping : '应收款日期'
		}, {
			name : 'realDate',
			type : 'date'
		}, {
			name : '收款百分比',
			mapping : '收款百分比'
		}, {
			name : '实际开始日期',
			mapping : '实际开始日期'
		}, {
			name : 'start_date',
			type : 'date',
			format : 'Y-m-d'
		}, {
			name : 'contractID',
			mapping : '合同ID'
		}
		, {
			name : '预计开票日期'
		}
		]);

com.bitc.xmis.Records.ProjectEstimatedPaymentRecord = Ext.data.Record.create([{
			name : 'ID'
		}, {
			name : 'projectID',
			mapping : '项目ID'
		}, {
			name : 'estimatedDate',
			mapping : '预计到款时间',
			type : 'date'
		}, {
			name : 'estimatedAmt',
			mapping : '预计金额',
			type : 'number'
		}, {
			name : 'estimatedPercent',
			mapping : '收款百分比'
		}, {
			name : 'isPayed',
			mapping : '是否已到款',
			type : 'bool'
		}, {
			name : 'registerName',
			mapping : '登记人'
		}, {
			name : 'registerDate',
			mapping : '登记日期'
		}, {
			name : 'lastModifiedName',
			mapping : '最后修改人'
		}, {
			name : 'lastModifiedDate',
			mapping : '最后修改日期'
		}]);

Ext.ns('com.bitc.xmis');
com.bitc.xmis.ProjectPaymentConditionGrid = Ext.extend(
		Ext.grid.EditorGridPanel, {
			prjID : '',
			contID : -1,
			contAmt : 0.00,
			payedMB : 0.00,
			selModel : new Ext.grid.RowSelectionModel({}),
			setContractAmt : function(value) {
				this.contAmt = value;
			},
			clicksToEdit : 1,
			viewConfig : {
				getRowClass : function(record, rowIndex, rowParams, store) {
					var hasRealStartDate = false;
					var rIndex = 0;
					if (store.getCount() > 0) {
						var records = store.getRange();
						for (var i = 0; i < records.length; i++) {
							if (records[i].get('realDate') != null
									&& records[i].get('realDate') <= new Date()) {
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
			initComponent : function() {
				var prjID = this.prjID;
				var contID = this.contID;
				var currentIndex = -1; // 当前选择的行
				var flag = false;
				var grid = this;
				grid.on('cellclick', function(g, rowIndex, columnIndex, e) {
							currentIndex = rowIndex;
						});
				grid.on('afteredit', function(e) {
							if (e.field == '项目状态') {
								grid.store.sort('start_date', 'ASC');
							}
							if(e.field == '收款百分比'){
								e.record.set('收款金额',
										contractAmtInPay * e.value / 100);
							}
							if(e.field == '收款金额'){
								e.record.set('收款百分比',Math.round(e.value
										/ contractAmtInPay * 10000) / 100);
							}
						});
				grid.on('columnresize', function(columnIndex, newSize) {
							if (columnIndex == 4)
								grid.getView().refresh(false);
						});
				this.store = new Ext.data.Store({
					url : __cxtPath
							+ '/contractMain.do?method=getPaymentConditionListByProjectID&projectID='
							+ prjID,
					autoLoad : true,
					pruneModifiedRecords : true,
					reader : new Ext.data.JsonReader({
						root : 'rows',
						totalProperty : 'totalCount',
						remoteSort : true,
						fields : com.bitc.xmis.Records.ProjectPaymentConditionRecord
					})
				});
				var st = this.store;
				var projectScheduleEditPanel = new com.bitc.xmis.project.ProjectScheduleEditPanel(
						{
							projectId : prjID,
							border : false,
							region:'center',
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
							items : [projectScheduleEditPanel],
							listeners : {
								'show' : function() {
									projectScheduleEditPanel.initDragDrop();
								}
							}
						});
				var searchProjField = new Ext.form.ComboBox({
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
									var records = st.getRange();
									var flag = true;
									for (var i = 0; i < records.length; i++) {
										if (records[i].get('对应项目状态') == r
												.get('statusId')) {
											if (currentIndex != i) {
												flag = false;
												c.clearValue();
												st.getRange().remove(i);
												st.getAt(currentIndex).set(
														'对应项目状态', '');
												Ext.Msg.alert("提示",
														"该项目节点已被使用，请重新选择！",
														function() {
															grid
																	.startEditing(
																			currentIndex,
																			2);
														});
											}
										}
									}
									if (flag) {
										st.getAt(currentIndex).set('对应项目状态',
												r.get('statusId'));
										var day = st.getAt(currentIndex)
												.get('延迟天数');
										if (day == undefined
												|| day == "undefined") {
											day = 0;
										}
										st.getAt(currentIndex).set(
												'payment_date',
												r.get('startDate')
														.add(Date.DAY,
																parseInt(day)));
										st.getAt(currentIndex).set(
												'start_date',
												r.get('startDate'));
										if (r.get('actualStartDate') != null) {
											st
													.getAt(currentIndex)
													.set(
															'应收款日期',
															r
																	.get('actualStartDate')
																	.add(
																			Date.DAY,
																			parseInt(day)));
											st.getAt(currentIndex).set(
													'实际开始日期',
													r.get('actualStartDate'));
											st.getAt(currentIndex).set(
													'realDate',
													r.get('actualStartDate'));
										} else {
											st.getAt(currentIndex).set('应收款日期',
													null);
											st.getAt(currentIndex).set(
													'实际开始日期', null);
											st.getAt(currentIndex).set(
													'realDate', null);
										}
									}

								}
							}
						});

				var paymentNameField = new Ext.form.TextField({
							allowBlank : false,
							emptyText : "必输项..."
						});

				var paymentDelayDaysField = new Ext.form.NumberField({
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
							var tt = st.getAt(currentIndex).get('payment_date');
							if (typeof(tt) == "string") {
								if (tt.indexOf('/') > 0) {
									var d = new Date(tt);
									if (typeof(newValue) != "string") {
										st.getAt(currentIndex).set(
												'payment_date',
												d.add(Date.DAY,
														parseInt(newValue
																- oldValue)));
									}
								} else {
									if (tt != "") {
										var time = tt.split(" ");
										var t = time[0].split("-");
										var dt = t[1] + "/" + t[2] + "/" + t[0];
										var d = new Date(dt);
										if (typeof(newValue) != "string") {
											st
													.getAt(currentIndex)
													.set(
															'payment_date',
															d
																	.add(
																			Date.DAY,
																			parseInt(newValue
																					- oldValue)));
										}
									}
								}
							} else {
								if (typeof(newValue) != "string") {
									st.getAt(currentIndex).set(
											'payment_date',
											tt.add(Date.DAY, parseInt(newValue
															- oldValue)));
								}
							}

							var t1 = st.getAt(currentIndex).get('应收款日期');
							if (typeof(t1) == "string") {
								if (t1.indexOf('/') > 0) {
									var dd = new Date(t1);
									if (typeof(newValue) != "string") {
										st.getAt(currentIndex).set(
												'应收款日期',
												dd.add(Date.DAY,
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
										if (typeof(newValue) != "string") {
											st
													.getAt(currentIndex)
													.set(
															'应收款日期',
															dd
																	.add(
																			Date.DAY,
																			parseInt(newValue
																					- oldValue)));
										}
									}
								}
							} else {
								if (typeof(newValue) != "string") {
									st.getAt(currentIndex).set(
											'应收款日期',
											t1 == null ? null : t1.add(
													Date.DAY, parseInt(newValue
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
				var invoiceDateField = new Ext.form.DateField({
					format : 'Y-m-d',
					showToday : true,
					anchor : '100%'
				});
				this.store.on('load', function(store, records, options) {
					var arr = [];
					paymentTotal = 0;
					paymentTotalAmt = 0.00;
					for (var i = 0; i < records.length; i++) {
						var record = records[i];
						paymentTotal = paymentTotal + record.get('收款百分比') * 1;
						paymentTotalAmt = parseFloat(paymentTotalAmt)
								+ parseFloat(record.get('收款金额'));
					}

					var msgtitle = '';
					if(paymentTotalAmt - contractAmtInPay < 0){
						var c = (paymentTotalAmt * 100) / contractAmtInPay;
						if(100 - c >= 1){
							msgtitle = String.format('<span style="float:left"><font color=red>{0}%合同金额未定义</span>',(100 - c).toFixed(2));
						}else{
							msgtitle = String.format('<span style="float:left"><font color=red>未定义合同金额: {0}</span>',Ext.util.Format.number((contractAmtInPay - paymentTotalAmt).toFixed(2),'0,000.00'));
						}
					}else if(paymentTotalAmt - contractAmtInPay == 0){
						msgtitle = '<span style="float:left"><font color=green>收款计划完成</span>';
					}else{
						
						msgtitle = String.format('<span style="float:left"><font color=red>收款计划定义超额: {0}</span>',Ext.util.Format.number((paymentTotalAmt - contractAmtInPay).toFixed(2),'0,000.00'));
					}

					var shouldPayedTotalAmt = 0.00; // 应到款总额
					var payedTotalAmt = 0.00; // 已到款总额
					var unpayedTotalAmt = 0.00; // 未到款总额

					var hasRealStartDate = false;
					var rIndex = 0;
					if (store.getCount() > 0) {
						var records = store.getRange();
						for (var i = 0; i < records.length; i++) {
							if (records[i].get('realDate') != null
									&& records[i].get('realDate') <= new Date()) {
								rIndex = i;
								hasRealStartDate = true;
							}
						}
					}
					for (var i = 0; i < records.length; i++) {
						var record = records[i];
						if (hasRealStartDate && i <= rIndex) {
							shouldPayedTotalAmt = parseFloat(shouldPayedTotalAmt)
									+ parseFloat(record.get('收款金额'));
							payedTotalAmt = parseFloat(payedTotalAmt)
									+ parseFloat(record.get('realIncome'));
						}
					}
					Ext.Ajax.request({
						url : __cxtPath
								+ '/contractMain.do?method=getIncomAmtTotalByGl',
						success : function(response, action) {
							var action = Ext.decode(response.responseText);
							var mb = parseFloat(action.mb);
							contID = parseInt(action.conID);
							cID = parseInt(action.conID);
							var invoiceAmt = parseFloat(action.invoiceAmt);
							var unInvoiceAmt = parseFloat(shouldPayedTotalAmt)
								- parseFloat(invoiceAmt);
							var str2 = '&nbsp;<font color=\"green\">已开票金额:'+ Ext.util.Format
							.number(
									parseFloat(invoiceAmt),
									'0,000.00') + '&nbsp;<font color=\"red\">按进度未开票金额:' + Ext.util.Format.number(
											unInvoiceAmt,
									'0,000.00') + '</font>';
							var str = '<font color=green>已收合同款:'
									+ Ext.util.Format.number(mb, '0,000.00')
									+ '</font>&nbsp;&nbsp;';
							unpayedTotalAmt = parseFloat(shouldPayedTotalAmt)
									- parseFloat(payedTotalAmt);
							grid.setTitle(msgtitle
									+ '<span style="float:right">'
									+ str
									+ '<font color=red>按进度应收账款:'
									+ Ext.util.Format.number(unpayedTotalAmt,
											'0,000.00') + '</font>'
											+str2
											+'</span>');
						},
						failure : function() {
						},
						method : "post",
						params : {
							proID : prjID
						}
					});
				}, this, {
					delay : 100
				});
				this.columns = [new Ext.grid.RowNumberer({
									width : 23
								}), {
							header : '项目状态ID',
							dataIndex : '对应项目状态',
							hidden : true,
							width : 20,
							sortable : false,
							editor : projectIdField
						}, {
							header : '对应项目节点',
							dataIndex : '项目状态',
							sortable : false,
							editor : searchProjField,
							renderer : function(value, cellMeta, record,
									rowIndex, columnIndex, store) {
								if (value == null
										|| (value != null && value.trim() == "")) {
									return '<font color="#C0C0C0">请选择项目节点</font>';
								}
								return value;
							}
						}, {
							header : '收款节点名称',
							dataIndex : '收款节点名称',
							sortable : false,
							renderer : function(value, cellMeta, record,
									rowIndex, columnIndex, store) {
								cellMeta.attr = 'ext:qtitle="收款节点名称" ext:qtip="'
										+ value + '"';
								if (value == null
										|| (value != null && value.trim() == "")) {
									cellMeta.attr = 'ext:qtitle="收款节点名称" ext:qtip="例如:预付款"';
									return '<font color="#C0C0C0">例如:预付款</font>';
								}
								return value;
							},
							editor : new Ext.form.TextField({
										allowBlank : false,
										emptyText : "必输项..."
									})
						}, {
							header : '收款条件描述',
							dataIndex : '收款条件描述',
							sortable : false,
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
									cellMeta.attr = 'ext:qtitle="收款条件描述" ext:qtip="'
											+ msg + '"';
								}
								if ((value != null && value.trim() == "")
										|| value == null) {
									cellMeta.attr = 'ext:qtitle="收款条件描述" ext:qtip="对收款节点名称的描述,可为空"';
									return '<font color="#C0C0C0">节点名称描述..</font>';
								}
								return Ext.util.Format.ellipsis(value,
										this.width / 12 - 1);
							},
							editor : new Ext.form.TextArea({})
						}, {
							xtype : 'numbercolumn',
							header : '收款百分比',
							align : 'right',
							width : 90,
							format : '00.00%',
							editor : paymentPersentField,
							dataIndex : '收款百分比',
							sortable : false
						}, {
							header : '预计到款金额',
							xtype : 'numbercolumn',
							align : 'right',
							format : '0,000.00',
							editor : paymentAmtField,
							dataIndex : '收款金额',
							sortable : false
						}, {
							header : '财务实际到款',
							xtype : 'numbercolumn',
							align : 'right',
							format : '0,000.00',
							dataIndex : 'realIncome',
							sortable : false
						}, {
							header : '延迟天数',
							align : 'right',
							width : 75,
							dataIndex : '延迟天数',
							sortable : false,
							editor : paymentDelayDaysField
						}, {
							header : '预计到款时间',
							align : 'center',
							xtype : 'datecolumn',
							width : 85,
							format : 'Y-m-d',
							dataIndex : 'payment_date',
							sortable : false,
							renderer : function(value, cellMeta, record,
									rowIndex, columnIndex, store) {
								var msg = '预计到款时间由系统自动计算得出。计算规则为：<br>预计到款时间 = 项目节点预计进入日期 + 延迟天数';
								cellMeta.attr = 'ext:qtitle="预计到款时间" ext:qtip="'
										+ msg + '"';
								if (value == null) {
									return '<font color="#C0C0C0">系统自动计算!</font>';
								}
								return Ext.util.Format.date(value, 'Y-m-d');
							}
						}, {

							header : '节点实际进入日期',
							align : 'center',
							dataIndex : '实际开始日期',
							width : 110,
							renderer : function(value, cellMeta, record,
									rowIndex, columnIndex, store) {
								var msg = '项目节点实际进入日期';
								cellMeta.attr = 'ext:qtitle="节点进入日期" ext:qtip="'
										+ msg + '"';
								return Ext.util.Format.date(value, 'Y-m-d');
							},
							sortable : true
						}, {
							header : '应收款日期',
							align : 'center',
							xtype : 'datecolumn',
							width : 85,
							format : 'Y-m-d',
							dataIndex : '应收款日期',
							renderer : function(value, cellMeta, record,
									rowIndex, columnIndex, store) {
								var msg = '应收款日期由系统自动计算得出。计算规则为：<br>应收款日期 = 项目节点实际进入日期 + 延迟天数';
								cellMeta.attr = 'ext:qtitle="应收款日期" ext:qtip="'
										+ msg + '"';
								if (value == null
										|| (value != null && value == "")) {
									return '<font color="#C0C0C0">系统自动计算!</font>';
								}
								return Ext.util.Format.date(value, 'Y-m-d');

							},
							sortable : false
						}
						,{
							header : '预计开票日期',
							align : 'center',
							width : 105,
							xtype : 'datecolumn',
							format : 'Y-m-d',
							dataIndex : '预计开票日期',
							sortable : false,
							editor : invoiceDateField
						}
						];

				this.tbar = [
						{
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
												.getCount() == grid.store
												.getCount()) {
									Ext.Msg.alert("提示",
											"创建的项目进度计划已全部使用，请先创建项目进度计划!");
								} else {
									var Plant = grid.getStore().recordType;
									var p = new Plant({});
									grid.stopEditing();
									grid.getStore().add([p]);
									grid.startEditing(
											grid.store.getCount() - 1, 2);
									currentIndex = grid.store.getCount() - 1;
									grid.getSelectionModel().selectRow(currentIndex);
									if (projectScheduleEditPanel.schedulePlanGrid.store
											.getCount() == grid.store
											.getCount())
										grid.newBtn.setDisabled(true);
								}
							}
						},
						'-',
						{
							ref : '../saveBtn',
							iconCls : 'save',
							text : '保存',
							handler : function() {
								grid.stopEditing(false);
								var records = grid.store.getModifiedRecords();
								var sfsuccess = true;
								if (records.length > 0) {
									for (i = 0; i < grid.store.getCount(); i++) {
										var r = grid.store.getAt(i);
										if (r.get('项目状态') == null
												|| Ext.isEmpty(r.get('项目状态'))) {
											row = i;
											Ext.Msg.alert("提示", "对应项目节点不能为空!",
													function() {
														grid.startEditing(row,
																2);
														currentIndex = row;
													});
											sfsuccess = false;
											return;
										}
										if (r.get('收款节点名称') == null
												|| Ext.isEmpty(r.get('收款节点名称'))) {
											row = i;
											Ext.Msg.alert("提示", "收款节点名称不能为空!",
													function() {
														grid.startEditing(row,
																3);
														currentIndex = row;
													});
											sfsuccess = false;
											return;
										} else {
											if (r.get('收款节点名称').length > 20) {
												row = i;
												Ext.Msg.alert("提示",
														"收款节点名称最多20个字符，请检查！",
														function() {
															grid.startEditing(
																	row, 3);
															currentIndex = row;
														});
												sfsuccess = false;
												return;
											}
										}
										if (r.get('收款金额') == null
												|| Ext.isEmpty(r.get('收款金额'))) {
											row = i;
											Ext.Msg.alert("提示", "预计到款金额不能为0!",
													function() {
														grid.startEditing(row,
																6);
														currentIndex = row;
													});
											sfsuccess = false;
											return;
										}
										if (r.get('收款条件描述') != null
												&& !Ext
														.isEmpty(r
																.get('收款条件描述'))
												&& r.get('收款条件描述').length > 100) {
											row = i;
											Ext.Msg.alert("提示",
													"收款条件描述最多100个汉字，请检查！",
													function() {
														grid.startEditing(row,
																4);
														currentIndex = row;
													});
											sfsuccess = false;
											return;
										}
									}

									if (sfsuccess) {
										var storeStr = com.bitc.xmis.util.ExtUtil.encodeStoreModifiedData(grid.store);
										Ext.Ajax.request({
											url : String
													.format(
															'{0}/contractMain.do?method=savePaymentCondition1',
															__cxtPath),
											params : {
												contractID : contID,
												records : storeStr
											},
											scope : this,
											success : function(response, opt) {
												var result = Ext
														.decode(response.responseText);
												if (result.success) {
													Ext.Msg
															.alert(
																	result.messageTitle,
																	result.message);
													grid.store.reload();
													grid.store.rejectChanges();
												} else {
													Ext.Msg
															.alert(
																	result.messageTitle,
																	result.message);
												}
												grid.newBtn.setDisabled(false);
											},
											failure : Ext.emptyFn,
											waitMsg : '正在保存，请稍候……',
											waitTitle : '保存'
										});
									}
								} else {
									Ext.Msg.alert("提示", "您没有任何操作，无需保存！");
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
								var record = grid.getSelectionModel()
										.getSelected();
								if (typeof(record) == 'undefined') {
									Ext.Msg.alert("未选择", "请先选择！");
									return true;
								} else {
									if (typeof(record.get('contractID')) == 'undefined') {
										Ext.Msg.confirm("请确认", "确定要删除该条付款条件吗？",
											function(btn) {
												if (btn == 'yes') {
													grid.store.remove(record);
													grid.newBtn.setDisabled(false);
												}
										});
									} else {
										Ext.Msg.confirm("请确认", "确定要删除该条付款条件吗？",
												function(btn) {
													if (btn == 'yes') {
														Ext.Ajax.request({
															url : __cxtPath
																	+ '/contractMain.do?method=delPaymentCondition',
															method : 'post',
															params : {
																contractID : record
																		.get('contractID'),
																conditionID : record
																		.get('收款条件ID')
															},
															success : function(
																	form,
																	action) {
																Ext.Msg
																		.alert(
																				'成功',
																				'删除成功！');
																grid
																		.getStore()
																		.reload();
																grid
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
										grid.newBtn.setDisabled(false);
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
								grid.store.reload();
							}
						},
						'->',
						'<table><tr><td>图例:</td><td style="background-color : green;">&nbsp;&nbsp;&nbsp;&nbsp;</td><td style="color : green;">已执行完项目进度</td><td style="background-color : #000000;">&nbsp;&nbsp;&nbsp;&nbsp;</td><td>未执行项目进度</td><td style="color : #0000FF;">*单击进入编辑状态</td></tr></table>'];

				com.bitc.xmis.DeviceGridPanel.superclass.initComponent
						.call(this);
			}
		});

com.bitc.xmis.ProjectEstimatedPaymentGrid = Ext.extend(
		Ext.grid.EditorGridPanel, {
			prjID : '',
			contAmt : 0.00,
			clicksToEdit : 1,
			setContractAmt : function(value) {
				this.contAmt = value;
			},
			selModel : new Ext.grid.RowSelectionModel({
						listeners : {
							'beforerowselect' : function(sm, rowIndex,
									keepExisting, record) {
								if (record.get('isPayed'))
									return false;
							}
						}
					}),
			initComponent : function() {
				var grid = this;
				var prjID = this.prjID;
				var currentIndex = -1; // 当前选择的行
				this.on('cellclick', function(g, rowIndex, columnIndex, e) {
							currentIndex = rowIndex;
						});
				this.on('beforeedit', function(e) { // 已确认付款的数据，不能修改
							if (e.record.get('isPayed'))
								return false;
						}, this);
				grid.on('afteredit', function(e) {
					if(e.field == 'estimatedPercent'){
						e.record.set('estimatedAmt',
								grid.contAmt * e.value / 100);
					}
					if(e.field == 'estimatedAmt'){
						e.record.set('estimatedPercent',Math.round(e.value
								/ grid.contAmt * 10000) / 100);
					}
				});
				this.store = new Ext.data.Store({
					url : __cxtPath
							+ '/contractMain.do?method=getProjectEstimatedPaymentList&projectID='
							+ prjID,
					autoLoad : true,
					pruneModifiedRecords : true,
					reader : new Ext.data.JsonReader({
						root : 'rows',
						totalProperty : 'totalCount',
						remoteSort : true,
						fields : com.bitc.xmis.Records.ProjectEstimatedPaymentRecord
					})
				});
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

				this.columns = [new Ext.grid.RowNumberer({
									width : 23
								}), {
							header : '到款时间',
							align : 'center',
							xtype : 'datecolumn',
							format : 'Y-m-d',
							dataIndex : 'estimatedDate',
							editor : new Ext.form.DateField({
										format : 'Y-m-d',
										anchor : '100%'
									}),
							sortable : true
						}, {
							xtype : 'numbercolumn',
							header : '收款百分比',
							align : 'right',
							format : '00.00%',
							dataIndex : 'estimatedPercent',
							editor : paymentPersentField,
							sortable : false
						}, {
							header : '收款金额',
							xtype : 'numbercolumn',
							align : 'right',
							format : '0,000.00',
							editor : paymentAmtField,
							dataIndex : 'estimatedAmt',
							sortable : true
						}, {
							header : '登记人',
							dataIndex : 'registerName',
							sortable : true,
							align : 'center',
							width : 80
						}, {
							header : '登记日期',
							align : 'center',
							width : 80,
							dataIndex : 'registerDate',
							sortable : true
						}, {
							header : '是否已到款',
							xtype : 'actioncolumn',
							align : 'center',
							width : 45,
							items : [{
								getClass : function(v, meta, record, rowIndex,
										colIndex, store) {
									if (record.get('isPayed')) {
										this.items[0].tooltip = '已付款';
										return 'payed';
									}else {
									 	if(!Ext.isEmpty(record.get('ID'))){
									 		this.items[0].tooltip = '点击确认付款';
										 	return 'unPayed';
									 	}
									}
								},
								handler : function(grid, rowIndex, colIndex,
										item, e) {
									if (!grid.store.getAt(rowIndex)
											.get('isPayed')) {
										var win = new Ext.Window({
											title : '到款确认',
											modal : true,
											width : 300,
											height : 190,
											frame : true,
											border : false,
											layout : 'border',
											plain : true,
											resizable : false,
											buttonAlign : "center",
											bodyStyle : "padding:10px",
											items : [{
												xtype : 'panel',
												region : 'center',
												frame : true,
												html : '<table width="270">'+'<tr>'
												+ '<td colspan="2">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;请选择您的操作。<font color="#FF0000">提示：确认到款登记后，数据将无法修改及删除。</font></td>'
												+ '</tr><tr><td colspan="2"><hr></td></tr>'
												+ '<tr>'
												+ '<td align="right" height="18">全部保存:</td>'
												+ '<td>修改到款状态并登记来款信息</td>'
												+ '</tr>'
												+ '<tr>'
												+ '	<td align="right" height="18">只保存到款状态:</td>'
												+ '	<td>修改到款状态，不登记来款信息</td>'
												+ '</tr>'
												+ '<tr>'
												+ '	<td align="right" height="18">取消:</td>'
												+ '	<td>取消操作</td>'
												+ '</tr>'+'</table>'
											}],
											buttons : [{
												text : "全部保存",
												handler : function() {
													showWin(grid.store, grid.store.getAt(rowIndex));
													grid.store.rejectChanges();
													win.close();
												}
											},{
												text : "只保存到款状态",
												handler : function() {
													win.close();
													Ext.Ajax.request({
														url : String
																.format(
																		'{0}/contractMain.do?method=updateEstimatedPayedFlag',
																		__cxtPath),
														params : {
															estimatedID : grid.store.getAt(rowIndex).get('ID')
														},
														scope : this,
														success : function(response, opt) {
															var result = Ext
																	.decode(response.responseText);
															if (result.success) {
																Ext.Msg
																		.alert(
																				result.messageTitle,
																				result.message);
																grid.store.reload();
																grid.store.rejectChanges();
															} else {
																Ext.Msg
																		.alert(
																				result.messageTitle,
																				result.message);
															}
														},
														failure : Ext.emptyFn,
														waitMsg : '正在保存，请稍候……',
														waitTitle : '保存'
													});
												}
											}, {
												text : "取消",
												handler : function() {
													win.close();
												}
											}]
										});
										win.show();
									}
								}
							}]
						}];
				this.tbar = [{
							ref : '../newBtn',
							iconCls : 'add',
							text : '新建',
							handler : function() {
								var Plant = grid.getStore().recordType;
								var p = new Plant({});
								grid.stopEditing();
								grid.getStore().add([p]);
							}
						}, '-', {
							ref : '../saveBtn',
							iconCls : 'save',
							text : '保存',
							handler : function() {
								grid.stopEditing(false);
								var records = grid.store.getModifiedRecords();
								var sfsuccess = true;
								if (records.length > 0) {
									for (i = 0; i < grid.store.getCount(); i++) {
										var r = grid.store.getAt(i);
										if (r.get('estimatedDate') == null
												|| Ext.isEmpty(r
														.get('estimatedDate'))) {
											row = i;
											Ext.Msg.alert("提示", "对应项目节点不能为空!",
													function() {
														grid.startEditing(row,
																1);
														currentIndex = row;
													});
											sfsuccess = false;
											return;
										}
										if (r.get('estimatedAmt') == null
												|| Ext.isEmpty(r
														.get('estimatedAmt'))) {
											row = i;
											Ext.Msg.alert("提示", "预计到款金额不能为0!",
													function() {
														grid.startEditing(row,
																3);
														currentIndex = row;
													});
											sfsuccess = false;
											return;
										}
									}

									if (sfsuccess) {
										var storeStr = com.bitc.xmis.util.ExtUtil
												.encodeStoreModifiedData(grid.store);
										Ext.Ajax.request({
											url : String
													.format(
															'{0}/contractMain.do?method=saveEstimatedInfo',
															__cxtPath),
											params : {
												projectID : grid.prjID,
												records : storeStr
											},
											scope : this,
											success : function(response, opt) {
												var result = Ext
														.decode(response.responseText);
												if (result.success) {
													Ext.Msg
															.alert(
																	result.messageTitle,
																	result.message);
													grid.store.reload();
													grid.store.rejectChanges();
												} else {
													Ext.Msg
															.alert(
																	result.messageTitle,
																	result.message);
												}
												grid.newBtn.setDisabled(false);
											},
											failure : Ext.emptyFn,
											waitMsg : '正在保存，请稍候……',
											waitTitle : '保存'
										});
									}
								} else {
									Ext.Msg.alert("提示", "您没有任何操作，无需保存！");
								}
							}

						}, '-', {
							ref : '../removeBtn',
							iconCls : 'remove',
							text : '删除',
							disabled : false,
							handler : function() {
								var records = grid.getSelectionModel()
										.getSelections();
								if (records.length == 0) {
									Ext.Msg.alert("未选择", "请先选择！");
								} else {
									
									var data = [];
									Ext.iterate(records, function(item) {
										data.push('{ID:' + item.get('ID') + '}');
									});
									var estimatedIDS = '';
									for(var i = 0 ; i < records.length;i++){
										if(i==0){
											estimatedIDS += records[i].get('ID');
										}else{
											estimatedIDS +=  ',' + records[i].get('ID');
										}
									}
									
									Ext.Msg.confirm("请确认", String.format(
													"确定要删除这{0}条数据吗？",
													records.length), function(
													btn) {
												if (btn == 'yes') {
													Ext.Ajax.request({
														url : __cxtPath
																+ '/contractMain.do?method=deleteEstimatedInfo',
														method : 'post',
														params : {
															estimatedIDS : estimatedIDS
														},
														success : function(
																response, opt) {
															var result = Ext.decode(response.responseText);
															if (result.success) {
																Ext.Msg.alert(result.messageTitle,result.message);
																grid.store.reload();
																grid.store.rejectChanges();
															} else {
																Ext.Msg.alert(result.messageTitle,result.message);
															}
														},
														failure : function(
																response, opt) {
															Ext.Msg.alert(
																			response.messageTitle,
																			response.message);
														}
													});
												}
											});
									grid.newBtn.setDisabled(false);
								}
							}
						}, '-', {
							iconCls : 'reload',
							ref : '../refreshButton',
							text : '刷新',
							handler : function(tabs, item) {
								grid.store.reload();
							}
						}, '->', '<font color="red">已确认付款的条目不可修改及删除</font><font color="blue">*单击进入编辑状态</font>'];

				com.bitc.xmis.DeviceGridPanel.superclass.initComponent
						.call(this);
			}
		});