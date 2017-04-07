Ext.ns('Ext.ux.grid');// (n*100).toFixed(2)+"%"
Ext.ux.grid.RowNumberer = Ext.extend(Object, {
			width : 45,
			fixed : true,
			sortable : false,
			hideable : false,
			menuDisabled : true,
			dataIndex : '序号',
			renderer : function(value, cellmeta, record, rowIndex, columnIndex,
					store) {
				if (store.getCount() - 1 < store.baseParams.limit
						&& store.getCount() == rowIndex + 1) {
					return '<font color=red>&nbsp;<b>合计</b>&nbsp;</font>';
				} else {
					if (rowIndex != store.baseParams.limit) {
						if (Ext.isEmpty(store.lastOptions.params)) {
							return rowIndex + 1;
						} else {
							return store.lastOptions.params.start + rowIndex
									+ 1;
						}
					} else {
						return '<font color=red>&nbsp;<b>合计</b>&nbsp;</font>';
					}
				}
			}
		});

Ext.onReady(function() {
	Ext.QuickTips.init();

	Ext.Msg.wait('正在加载，请稍后...', '');

	var gridLocal, groupLocal;
	var pageSize = 20;
	var typeString;
	var exportname = "项目结余表";

	var record = Ext.data.Record.create([{
				name : 'year'
			}, {
				name : 'citem_id'
			}, {
				name : 'project_code'
			}, {
				name : '项目编号'
			}, {
				name : '合同名称'
			}, {
				name : '合同或预算金额'
			}, {
				name : '到款率'
			}, {
				name : '标识'
			}, {
				name : '经费类别'
			}, {
				name : '部门'
			}, {
				name : '以前年度累计到款'
			}, {
				name : '以前年度累计支出'
			}, {
				name : '合同到款'
			}, {
				name : '退税收入'
			}, {
				name : '人员经费'
			}, {
				name : '采购支出'
			}, {
				name : '外加工费'
			}, {
				name : '技术服务费'
			}, {
				name : '外拨款'
			}, {
				name : '业务招待费'
			}, {
				name : '会议差旅费'
			}, {
				name : '车辆使用'
			}, {
				name : '研发支出'
			}, {
				name : '计提研发费'
			}, {
				name : '计提项目管理费'
			}, {
				name : '计提工资管理费'
			}, {
				name : '计提绩效管理费'
			}, {
				name : '计提福利费'
			}, {
				name : '计提教育经费'
			}, {
				name : '计提工会经费'
			}, {
				name : '应交增值税'
			}, {
				name : '其他支出'
			}, {
				name : '项目结余'
			}, {
				name : '可抵税金'
			}, {
				name : '已开增值税票'
			}, {
				name : '已开营业税票'
			}, {
				name : '在研项目数'
			}, {
				name : '转账收入'
			}, {
				name : '硬件'
			}, {
				name : '软件'
			}, {
				name : '其他'
			}, {
				name : '在研项目数'
			}, {
				name : 'deptId'
			}]);

	Ext.Ajax.request({
		url : __cxtPath
				+ '/financyMain.do?method=getProjectBalanceDynamicColumn',
		method : 'post',
		success : function(response) {
			var result = Ext.decode(response.responseText);
			if (result.success) {
				// 判断结转标志
				var bflag = result.attributes.bflag;
				// //当前可查看的最大日期
				// var maxValueDate = (bflag)
				// ? new Date().format('Y-m-d')
				// : ((new Date().getFullYear() - 1) + '-12-31');
				//
				var tip;
				if (bflag) {
					tip = '<font size= 2.5px>项目结余  = 当前累计到款  - 当前累计支出<br>合同额到款率 = 截止到查询日期，项目或部门的合同实际累计到款额 占 合同金额的百分比。<br>根据财资处约定，最早查询日期：2013-01-01 </font>'
				} else {
					tip = '<font size= 2.5px>项目结余  = 当前累计到款  - 当前累计支出<br>合同额到款率 = 截止到查询日期，项目或部门的合同实际累计到款额 占 合同金额的百分比。<br>根据财资处约定，最早查询日期：2013-01-01&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;现因'
							+ (new Date().getFullYear() - 1)
							+ '年财务结转尚未完成，导致'
							+ (new Date().getFullYear())
							+ '年应交增值税、当年支出、项目结余数据有误差，详情请联系财资处。</font>'
				}

				var maxValueDate = new Date().format('Y-m-d');

				// var tip = '<font size= 2.5px>项目结余 = 上年累计到款 + 当年到款小计 - 上年累计支出
				// - 当年支出小计<br>根据财资处约定，最早查询日期：2013-01-01 </font>'

				var columns = result.attributes;
				var columnOne = Ext.decode(columns.columnOne);
				var columnTwo = Ext.decode(columns.columnTwo);
				var columnThree = Ext.decode(columns.columnThree);
				var group = groupLocal = new Ext.ux.grid.ColumnHeaderGroup({
							rows : [columnOne, columnTwo, columnThree]
						});

				var columnMod = Ext.decode(columns.columnModel);
				columnMod.unshift(new Ext.ux.grid.RowNumberer({
							width : 35
						}));

				var columnModel = new Ext.grid.ColumnModel({
							columns : columnMod,
							defaults : {
								sortable : true
								// ,
								// menuDisabled : true
							}
						});

				// 导出，header字符串
				var incomeTotalString = "<a href=\"javascript:;\"  onclick=\"javascript:hiddenColumn('当前累计到款','#3300FF','11,15',1,'11,12,13,14,15');\" ><font color=#3300FF><img src=\""
						+ __cxtPath
						+ "/resources/images/default/grid/group-expand.gif\" style=\"vertical-align:middle;\" />当前累计到款</font></a>";
				var incomeString = "<a href=\"javascript:;\"  onclick=\"javascript:hiddenColumn('当年到款','#8A2BE2','12,13,14',2);\" ><font color=#8A2BE2><img src=\""
						+ __cxtPath
						+ "/resources/images/default/grid/group-expand.gif\" style=\"vertical-align:middle;\" />当年到款 </font></a>";
				var outTotalString = "<a href=\"javascript:;\"  onclick=\"javascript:hiddenColumn('当前累计支出','#3300FF','17,39',1,'17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39');\" ><font color=#3300FF><img src=\""
						+ __cxtPath
						+ "/resources/images/default/grid/group-expand.gif\" style=\"vertical-align:middle;\" />当前累计支出</font></a>";
				var outString = "<a href=\"javascript:;\"  onclick=\"javascript:hiddenColumn('当年支出','#FF4500','26,30,38',1,'18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38');\" ><font color=#FF4500><img src=\""
						+ __cxtPath
						+ "/resources/images/default/grid/group-expand.gif\" style=\"vertical-align:middle;\" />当年支出</font></a>";
				var businessFundString = "<a href=\"javascript:;\"  onclick=\"javascript:hiddenColumn('主要业务经费支出','#008000','18,19,20,21,22,23,24,25',2);\" ><font color=#008000><img src=\""
						+ __cxtPath
						+ "/resources/images/default/grid/group-expand.gif\" style=\"vertical-align:middle;\" />主要业务经费支出</font></a>";
				var elseOutString = "<a href=\"javascript:;\"  onclick=\"javascript:hiddenColumn('其他支出','#CC0066','27,28,29',2);\" ><font color=#CC0066><img src=\""
						+ __cxtPath
						+ "/resources/images/default/grid/group-expand.gif\" style=\"vertical-align:middle;\" />其他支出</font></a>";
				var countAndDrawString = "<a href=\"javascript:;\"  onclick=\"javascript:hiddenColumn('计提支出','#0066FF','31,32,33,34,35,36,37',2);\" ><font color=#0066FF><img src=\""
						+ __cxtPath
						+ "/resources/images/default/grid/group-expand.gif\" style=\"vertical-align:middle;\" />计提支出</font> </a>";
				var invoicedString = "<a href=\"javascript:;\"  onclick=\"javascript:hiddenColumn('已开发票金额','#996633','41,42,43',2);\" ><font color=#996633><img src=\""
						+ __cxtPath
						+ "/resources/images/default/grid/group-expand.gif\" style=\"vertical-align:middle;\" />已开发票金额</font></a>";

				var exp = new Ext.ux.Export({
					formater : [{
								columnHeader : '',
								formatClass : 'com.bitc.jk.xmis.export.PagingRowNumber'
							},
							// {
							// columnHeader : '项目编号',
							// formatClass :
							// 'com.bitc.jk.xmis.export.ProjectCodeForPB'
							// },
							{
								columnHeader : '合同金额',
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : '合同额到款率',
								formatClass : 'com.bitc.jk.xmis.export.PercentFormat'
							}, {
								columnHeader : '项目结余',
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : incomeTotalString + ' '
										+ '上年累计到款',
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : incomeTotalString + ' '
										+ incomeString + ' '
										+ "<font color=#8A2BE2>合同到款 </font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : incomeTotalString + ' '
										+ incomeString + ' '
										+ "<font color=#8A2BE2>退税收入</font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : incomeTotalString + ' '
										+ incomeString + ' '
										+ "<font color=#8A2BE2>转账收入</font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : incomeTotalString + ' '
										+ incomeString + ' '
										+ "<font color=#FF00FF>当年到款小计</font>",
								formatClass : 'com.bitc.jk.xmis.export.ContractIncomeTotalForPB'
							}, {
								columnHeader : incomeTotalString + ' '
										+ "<font color=#3300FF>当前累计到款合计</font>",
								formatClass : 'com.bitc.jk.xmis.export.IncomeTotalForPB'
							}, {
								columnHeader : outTotalString + ' ' + '上年累计支出',
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : outTotalString + ' ' + outString
										+ ' ' + businessFundString + ' '
										+ "<font color=#008000>人员经费</font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : outTotalString + ' ' + outString
										+ ' ' + businessFundString + ' '
										+ "<font color=#008000>采购支出</font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : outTotalString + ' ' + outString
										+ ' ' + businessFundString + ' '
										+ "<font color=#008000>外加工费</font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : outTotalString + ' ' + outString
										+ ' ' + businessFundString + ' '
										+ "<font color=#008000>技术服务费</font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : outTotalString + ' ' + outString
										+ ' ' + businessFundString + ' '
										+ "<font color=#008000>外拨款</font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : outTotalString + ' ' + outString
										+ ' ' + businessFundString + ' '
										+ "<font color=#008000>业务招待费</font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : outTotalString + ' ' + outString
										+ ' ' + businessFundString + ' '
										+ "<font color=#008000>会议差旅费</font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : outTotalString + ' ' + outString
										+ ' ' + businessFundString + ' '
										+ "<font color=#008000>车辆使用</font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : outTotalString + ' ' + outString
										+ ' ' + businessFundString + ' '
										+ "<font color=#FF4500>小计</font>",
								formatClass : 'com.bitc.jk.xmis.export.BusinessFundTotalForPB'
							}, {
								columnHeader : outTotalString + ' ' + outString
										+ ' ' + elseOutString + ' '
										+ "<font color=#CC0066>研发支出</font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : outTotalString + ' ' + outString
										+ ' ' + elseOutString + ' '
										+ "<font color=#CC0066>应交增值税</font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : outTotalString + ' ' + outString
										+ ' ' + elseOutString + ' '
										+ "<font color=#CC0066>其他支出</font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : outTotalString + ' ' + outString
										+ ' ' + elseOutString + ' '
										+ "<font color=#FF4500>小计</font>",
								formatClass : 'com.bitc.jk.xmis.export.ElsePayTotalForPB'
							}, {
								columnHeader : outTotalString + ' ' + outString
										+ ' ' + countAndDrawString + ' '
										+ "<font color=#0066FF>计提研发费</font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : outTotalString + ' ' + outString
										+ ' ' + countAndDrawString + ' '
										+ "<font color=#0066FF>计提项目管理费</font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : outTotalString + ' ' + outString
										+ ' ' + countAndDrawString + ' '
										+ "<font color=#0066FF>计提工资管理费</font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : outTotalString + ' ' + outString
										+ ' ' + countAndDrawString + ' '
										+ "<font color=#0066FF>计提绩效管理费</font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : outTotalString + ' ' + outString
										+ ' ' + countAndDrawString + ' '
										+ "<font color=#0066FF>计提福利费</font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : outTotalString + ' ' + outString
										+ ' ' + countAndDrawString + ' '
										+ "<font color=#0066FF>计提教育经费</font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : outTotalString + ' ' + outString
										+ ' ' + countAndDrawString + ' '
										+ "<font color=#0066FF>计提工会经费</font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : outTotalString + ' ' + outString
										+ ' ' + countAndDrawString + ' '
										+ "<font color=#FF4500>小计</font>",
								formatClass : 'com.bitc.jk.xmis.export.CountAndDrawTotalForPB'
							}, {
								columnHeader : outTotalString + ' ' + outString
										+ ' '
										+ "<font color=#FF00FF>当年支出小计</font>"
										+ ' ' + "<font color=#FF00FF>小计</font>",
								formatClass : 'com.bitc.jk.xmis.export.PayTotalForPB'
							}, {
								columnHeader : outTotalString + ' '
										+ "<font color=#3300FF>当前累计支出合计</font>",
								formatClass : 'com.bitc.jk.xmis.export.OutTotalForPB'
							}, {
								columnHeader : invoicedString + ' '
										+ "<font color=#996633>硬件</font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : invoicedString + ' '
										+ "<font color=#996633>软件</font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : invoicedString + ' '
										+ "<font color=#996633>其他</font>",
								formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
							}, {
								columnHeader : invoicedString + ' '
										+ "<font color=#FF00FF>已开发票金额小计</font>",
								formatClass : 'com.bitc.jk.xmis.export.InvoicedTotalForPB'
							}]
				});

				var store = new Ext.data.Store({
					proxy : new Ext.data.HttpProxy({
						url : __cxtPath
								+ '/financyMain.do?method=getProjectBalanceStore'
					}),
					baseParams : {
						start : 0,
						limit : pageSize,
						executeString : '{call xmis.sp_projectBalance(?,?,?,?,?,?,?)}',
						type : 2,
						endDate : maxValueDate,
						deptName : 'allDept'
					},
					remoteSort : true,
					sortInfo : {
						field : '项目结余',
						direction : 'DESC'
					},
					reader : new Ext.data.JsonReader({
								root : 'rows',
								totalProperty : 'totalCount'
							}, record)
				});

				var grid = gridLocal = new Ext.grid.GridPanel({
					region : 'center',
					cm : columnModel,
					store : store,
					bodyStyle : 'width : 33%',
					plugins : group,
					enableColumnMove : false,
					loadMask : true,
					loadMask : {
						msg : '正在加载数据，请稍侯……'
					},
					tbar : ['截止日期：', new Ext.form.DateField({
								ref : '../endDate',
								format : 'Y-m-d',
								minValue : '2013-01-01',
								maxValue : maxValueDate,
								fieldLabel : '截止日期',
								showToday : true,
								value : maxValueDate,
								anchor : '100%',
								width : 150,
								listeners : {
									'select' : function(dateField, date) {
										grid.dept.getStore()
												.setBaseParam('endDate',
														date.format('Y-m-d'));
										grid.secondDept.getStore()
												.setBaseParam('endDate',
														date.format('Y-m-d'));

										delete store.baseParams['endDate'];

										store.setBaseParam('endDate', date
														.format('Y-m-d'));
										store.load({
													params : {
														start : 0,
														limit : pageSize,
														sort : '项目结余',
														dir : 'DESC'
													}
												});
									}
								}
							}), '-', {
						ref : '../dept',
						xtype : 'combo',
						width : 150,
						displayField : 'dept',
						valueField : 'dept',
						editable : false,
						triggerAction : 'all',
						emptyText : '请选择部门',
						mode : 'remote',
						forceSelection : true,
						allowBlank : true,
						store : new Ext.data.JsonStore({
							url : __cxtPath
									+ '/financyMain.do?method=getProjectBalanceStore',
							fields : [{
										name : 'dept',
										mapping : '部门'
									}, {
										name : 'deptId'
									}],
							baseParams : {
								start : 0,
								limit : pageSize,
								executeString : '{call xmis.sp_projectBalance(?,?,?,?,?,?,?)}',
								type : 7,
								endDate : maxValueDate,
								deptName : ''
							},
							root : 'rows'
						}),
						listeners : {
							'select' : function(combo, record, index) {
								delete store.baseParams['type'];
								delete store.baseParams['endDate'];
								delete store.baseParams['deptName'];

								grid.inkling.el.dom.value = '';
								grid.inkling.triggers[0].hide();
								grid.inkling.hasSearch = false;

								if (combo.getRawValue() == '所有部门') {
									store.setBaseParam('type', 2);
									store
											.setBaseParam('endDate',
													grid.endDate.getValue()
															.format('Y-m-d'));
									store.setBaseParam('deptName', 'allDept');
									store.load({
												params : {
													start : 0,
													limit : pageSize,
													sort : '项目结余',
													dir : 'DESC'
												},
												callback : function() {
													setHiddenCol(true);
													setHiddenColumn('在研项目数',
															false);
													setHiddenColumn('部门', false);
												}
											});
								} else {
									store.setBaseParam('type', 3);
									store
											.setBaseParam('endDate',
													grid.endDate.getValue()
															.format('Y-m-d'));
									store.setBaseParam('deptName', grid.dept
													.getRawValue());
									store.load({
												params : {
													start : 0,
													limit : pageSize,
													sort : '项目结余',
													dir : 'DESC'
												},
												callback : function() {
													setHiddenColumn('在研项目数',
															true);
													setHiddenCol(false);
													setHiddenColumn('部门', true);
												}
											});
								}
								grid.secondDept.setValue('');
								grid.secondDept.store.removeAll();
								grid.secondDept.store.baseParams.deptName = grid.dept
										.getRawValue();
								grid.secondDept.store.load();
							}
						}
					}, '-', {
						hidden : true,
						ref : '../secondDept',
						xtype : 'combo',
						width : 150,
						displayField : 'dept',
						valueField : 'dept',
						editable : false,
						triggerAction : 'all',
						emptyText : '请选择执行部门',
						mode : 'remote',
						forceSelection : true,
						allowBlank : true,
						store : new Ext.data.JsonStore({
							url : __cxtPath
									+ '/financyMain.do?method=getProjectBalanceStore',
							fields : [{
										name : 'dept',
										mapping : '部门'
									}],
							baseParams : {
								start : 0,
								limit : pageSize,
								executeString : '{call xmis.sp_projectBalance(?,?,?,?,?,?,?)}',
								type : 8,
								endDate : maxValueDate,
								deptName : ''
							},
							root : 'rows',
							listeners : {
								'load' : function(store, records, options) {
									if (records.length == 0
											|| records.length == 1
											|| records.length == 2) {
										store.removeAll();
									}
								}
							}
						}),
						listeners : {
							'select' : function(combo) {
								delete store.baseParams['type'];
								delete store.baseParams['endDate'];
								delete store.baseParams['deptName'];
								if (combo.getValue() == '所有部门') {
									store.setBaseParam('type', 5);
									store
											.setBaseParam('endDate',
													grid.endDate.getValue()
															.format('Y-m-d'));
									store.setBaseParam('deptName', grid.dept
													.getRawValue());
									store.load({
												params : {
													start : 0,
													limit : pageSize,
													sort : '项目结余',
													dir : 'DESC'
												},
												callback : function() {
													setHiddenColumn('在研项目数',
															false);
													setHiddenCol(true);
													setHiddenColumn('部门', false);
												}
											});
								} else {
									store.setBaseParam('type', 4);
									store
											.setBaseParam('endDate',
													grid.endDate.getValue()
															.format('Y-m-d'));
									store.setBaseParam('deptName',
											grid.secondDept.getValue());
									store.load({
												params : {
													start : 0,
													limit : pageSize,
													sort : '项目结余',
													dir : 'DESC'
												},
												callback : function() {
													setHiddenCol(false);
													setHiddenColumn('在研项目数',
															true);
													setHiddenColumn('部门', true);
												}
											});
								}
							}
						}
					}, '-', '课题：', new Ext.ux.InklingSearchField({
						ref : '../inkling',
						width : 200,
						store : store,
						tooltip : {
							title : '模糊查询',
							text : '您可以输入项目编号、课题编号、合同名称进行查询。'
						},
						onTrigger1Click : function() {
							delete gridLocal.store.baseParams['type'];
							delete gridLocal.store.baseParams['endDate'];
							delete gridLocal.store.baseParams['deptName'];
							if (this.hasSearch) {
								this.el.dom.value = '';
								gridLocal.store.setBaseParam('type', 2);
								gridLocal.store
										.setBaseParam('endDate', grid.endDate
														.getValue()
														.format('Y-m-d'));
								gridLocal.store.setBaseParam('deptName',
										'allDept');
								gridLocal.store.load({
											params : {
												start : 0,
												limit : pageSize,
												sort : '项目结余',
												dir : 'DESC'
											},
											callback : function() {
												setHiddenCol(true);
												setHiddenColumn('在研项目数', false);
											}
										});
								this.triggers[0].hide();
								this.hasSearch = false;
							}
						},
						onTrigger2Click : function() {
							delete gridLocal.store.baseParams['type'];
							delete gridLocal.store.baseParams['endDate'];
							delete gridLocal.store.baseParams['deptName'];

							var v = this.getRawValue();
							if (v.length < 1) {
								Ext.Msg.alert("提示", "请输入查询条件！");
							} else {
								grid.dept.setValue('');
								grid.secondDept.setValue('');
								gridLocal.store.setBaseParam('type', 6);
								gridLocal.store
										.setBaseParam('endDate', grid.endDate
														.getValue()
														.format('Y-m-d'));
								gridLocal.store.setBaseParam('deptName',
										"project_code like '%" + v
												+ "%' or 合同名称  like '%" + v
												+ "%' or citem_id  like '%" + v
												+ "%'");
								gridLocal.store.load({
											params : {
												start : 0,
												limit : pageSize,
												sort : '项目结余',
												dir : 'DESC'
											},
											callback : function() {
												setHiddenCol(false);
												setHiddenColumn('在研项目数', true);
												setHiddenColumn('部门', false);
											}
										});
								this.hasSearch = true;
								this.triggers[0].show();
							}
						}
					})],
					bbar : new Ext.ux.TotalPagingToolbar({
								pageSize : 20,
								store : store,
								displayInfo : true,
								displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
								emptyMsg : "无显示数据",
								plugins : [exp, new Ext.ux.PagelimitCombo()]
							})
				});
				new Ext.Viewport({
							border : false,
							layout : 'border',
							items : [grid, {
										id : 'tip',
										border : false,
										region : 'north',
										height : 60,
										bodyStyle : {
											background : '#ffffff',
											padding : '7px'
										},
										// html : '<font size= 2.5px>项目结余 = 累计到款
										// + 当年到款小计 - 累计支出 -
										// 当年支出小计<br>最早查询日期：2013-01-01 </font>'
										html : tip
									}]
						});
				setHiddenCol(true);
				setHiddenColumn('在研项目数', false);

				store.load({
							callback : function() {
								typeString = store.reader.jsonData.attributes.type;
							}
						});
			}
			Ext.Msg.hide();
		},
		failure : function(response) {
			var result = Ext.decode(response.responseText);
			Ext.Msg.alert(result.messageTitle, result.message)
			return;
		}
	});

	this.formatAmt = function(value) {
		return Ext.util.Format.number(value, '0,000.00');
	}

	this.linkDeptStore = function(deptName) {
		delete gridLocal.store.baseParams['type'];
		delete gridLocal.store.baseParams['endDate'];
		delete gridLocal.store.baseParams['deptName'];

		if (gridLocal.secondDept.getRawValue() != null
				&& gridLocal.secondDept.getRawValue() != "") {
			gridLocal.store.setBaseParam('type', 4);
			gridLocal.store.setBaseParam('endDate', gridLocal.endDate
							.getValue().format('Y-m-d'));
			gridLocal.store.setBaseParam('deptName', deptName);
			gridLocal.store.load({
						params : {
							start : 0,
							limit : pageSize,
							sort : '项目结余',
							dir : 'DESC'
						},
						callback : function() {
							setHiddenCol(false);
							setHiddenColumn('在研项目数', true);
							setHiddenColumn('部门', true);
							gridLocal.secondDept.setValue(deptName);
						}
					});
		} else {
			gridLocal.store.setBaseParam('type', 3);
			gridLocal.store.setBaseParam('endDate', gridLocal.endDate
							.getValue().format('Y-m-d'));
			gridLocal.store.setBaseParam('deptName', deptName);
			gridLocal.store.load({
						params : {
							start : 0,
							limit : pageSize,
							sort : '项目结余',
							dir : 'DESC'
						},
						callback : function() {
							setHiddenCol(false);
							setHiddenColumn('在研项目数', true);
							setHiddenColumn('部门', true);
							gridLocal.dept.setValue(deptName);
							gridLocal.secondDept.store.baseParams.deptName = deptName;
						}
					});
		}
	}

	this.hiddenColumn = function(header, color, indexs, level, indexChilds) {
		var ahref = Ext.EventObject.getTarget();
		if (indexs != "") {
			var index = indexs.split(',');
			if (level == 1) {
				var indexChild = indexChilds.split(',');
				if (gridLocal.getColumnModel().config[index[0]].hidden == true) {
					for (var i = 0; i < index.length; i++) {
						gridLocal.getColumnModel().setHidden(index[i], false);
						ahref.innerHTML = '<font color='
								+ color
								+ '><img src=\"'
								+ __cxtPath
								+ '/resources/images/default/grid/group-collapse.gif\" style=\"vertical-align:middle;\" />'
								+ header + '  </font>';
					}
				} else {
					for (var j = 0; j < indexChild.length; j++) {
						if (gridLocal.getColumnModel().config[indexChild[j]].hidden == false) {
							gridLocal.getColumnModel().setHidden(indexChild[j],
									true);
							ahref.innerHTML = '<font color='
									+ color
									+ '><img src=\"'
									+ __cxtPath
									+ '/resources/images/default/grid/group-expand.gif\" style=\"vertical-align:middle;\" />'
									+ header + '  </font>';
						} else {
							ahref.innerHTML = '<font color='
									+ color
									+ '><img src=\"'
									+ __cxtPath
									+ '/resources/images/default/grid/group-collapse.gif\" style=\"vertical-align:middle;\" />'
									+ header + ' </font>';
						}
					}
				}
			} else {
				if (ahref.innerHTML.indexOf('group-expand') > 0) {
					ahref.innerHTML = '<font color='
							+ color
							+ '><img src=\"'
							+ __cxtPath
							+ '/resources/images/default/grid/group-collapse.gif\" style=\"vertical-align:middle;\" />'
							+ header + '  </font>';
					for (var i = 0; i < index.length; i++) {
						gridLocal.getColumnModel().setHidden(index[i], false);
					}
				} else {
					ahref.innerHTML = '<font color='
							+ color
							+ '><img src=\"'
							+ __cxtPath
							+ '/resources/images/default/grid/group-expand.gif\" style=\"vertical-align:middle;\" />'
							+ header + ' </font>';
					for (var i = 0; i < index.length; i++) {
						gridLocal.getColumnModel().setHidden(index[i], true);
					}
				}
			}
		}
	}

	this.setHiddenColumn = function(columnName, bool) {
		gridLocal.getColumnModel().setHidden(
				gridLocal.getColumnModel().findColumnIndex(columnName), bool);
	}

	this.setHiddenCol = function(bool) {
		setHiddenColumn('项目编号', bool);// dataIndex
		setHiddenColumn('合同名称', bool);
		// setHiddenColumn('合同或预算金额', bool);
		setHiddenColumn('标识', bool);
		setHiddenColumn('经费类别', bool);
	}

	// Ext.ns('Ext.ux.grid');
	// Ext.ux.grid.RowNumberer = Ext.extend(Object, {
	// id : 'cnumberer',
	// width : 40,
	// sortable : false,
	// hideable : false,
	// menuDisabled : true,
	// dataIndex : '',
	// renderer : function(value, cellmeta, record, rowIndex,
	// columnIndex, store) {
	// if (record.get('flag') == 1) {
	// return '<font color=red>&nbsp;<b>合计</b>&nbsp;</font>';
	// } else {
	// return rowIndex + 1;
	// }
	// }
	// });

	this.showDetailsWin = function(columnName, project_code, citem_id,
			deptName, title, totalAmt, record) {

		var lastParam = "";
		var dePageSize = 20;
		var typeParam = "(";
		var types = typeString.split(',');
		var names = columnName.split(',');
		for (var i = 0; i < types.length; i++) {
			for (var j = 0; j < names.length; j++) {
				if (names[j] == types[i]) {
					typeParam = typeParam + types[i - 1].trim() + ",";
				}
			}
		}

		typeParam = typeParam.substring(0, typeParam.length - 1) + ")";
		var deRecord = new Ext.data.Record.create([{
					name : 'citem_id'
				}, {
					name : '金额'
				}, {
					name : '凭单号'
				}, {
					name : '记账日期'
				}, {
					name : '备注'
				}, {
					name : 'type'
				}, {
					name : 'project_code'
				}, {
					name : 'flag'
				}]);

		var deStore = new Ext.data.Store({
			proxy : new Ext.data.HttpProxy({
				url : __cxtPath
						+ '/financyMain.do?method=getProjectDetailsForProjectBalance'
			}),
			baseParams : {
				start : 0,
				limit : dePageSize,
				executeString : '{call xmis.sp_projectDetailsForProjectBalance(?,?,?,?)}',
				type : typeParam,
				endDate : gridLocal.endDate.getValue().format('Y-m-d'),
				startDate : gridLocal.endDate.getValue().format('Y-m-d')
						.substring(0, 4)
						+ '-01-01'
			},
			romoteSort : true,
			reader : new Ext.data.JsonReader({
						root : 'rows',
						totalProperty : 'totalCount'
					}, deRecord)
		});

		deStore.on('load', function() {
					deGrid.setTitle("合计约:&nbsp;&nbsp;&nbsp;&nbsp;"
							+ formatCurrency("", totalAmt.toString(), "blue"));
				});
		var deColumn = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({
							width : 35
						}), {
					header : '项目编号',
					dataIndex : 'project_code',
					align : 'left',
					width : 150,
					sortable : true,
					renderer : function(value, cellmeta, record) {
						if (value == null || value == "") {
							if (record.get('citem_id') == null
									|| record.get('citem_id') == "") {
								return "";
							} else {
								return record.get('citem_id') + '(财)';
							}
						}
						return value;
					},
					exportFormater : function(value, record, index, pageinfo) {
						if (value == null || value == "") {
							if (record.get('citem_id') == null
									|| record.get('citem_id') == ""
									|| "null" == record.get('citem_id')) {
								return "";
							} else {
								return record.get('citem_id') + '(财)';
							}
						}
						return value;
					}
				}, {
					header : '凭单号',
					dataIndex : '凭单号',
					align : 'center',
					width : 100,
					sortable : true
				}, {
					header : '记账日期',
					dataIndex : '记账日期',
					align : 'center',
					width : 100,
					sortable : true,
					renderer : function(value) {
						if (value != null && value != "") {
							return new Date(value).format('Y-m-d');
						}
					}
				}, {
					header : '金额',
					dataIndex : '金额',
					align : 'right',
					width : 120,
					sortable : true,
					renderer : function(value) {
						return formatAmt(value);
					}
				}, {
					header : '备注',
					dataIndex : '备注',
					align : 'left',
					width : 200,
					sortable : true
				}]);

		var deExp = new Ext.ux.Export({
			formater : [{
						columnHeader : '',
						formatClass : 'com.bitc.jk.xmis.export.RowNumber'
					}, {
						columnHeader : '记账日期',
						formatClass : 'com.bitc.jk.xmis.export.FormateTimerForPB'
					}, {
						columnHeader : '金额',
						formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
					}]

		});

		var deGrid = new Ext.grid.GridPanel({

					region : 'center',
					store : deStore,
					cm : deColumn,
					stripeRows : true,
					layout : 'fit',
					bodyStyle : 'width : 33%',
					title : '项目结余明细列表',
					tbar : ['起始时间：', new Ext.form.DateField({
								ref : '../startDate',
								format : 'Y-m-d',
								minValue : gridLocal.endDate.getValue()
										.format('Y-m-d').substring(0, 4)
										+ '-01-01',
								maxValue : gridLocal.endDate.getValue()
										.format('Y-m-d').substring(0, 4)
										+ '-12-31',
								fieldLabel : '起始时间',
								value : gridLocal.endDate.getValue()
										.format('Y-m-d').substring(0, 4)
										+ '-01-01',
								anchor : '100%',
								width : 150,
								listeners : {
									'select' : function(dateField, date) {
										if (date > deGrid.endDate.getValue()) {

											Ext.Msg
													.alert("提示",
															"起始时间不能大于截止时间！");
											dateField
													.setValue(dateField.startValue);
										} else {
											delete deStore.baseParams['startDate'];
											deStore.setBaseParam('startDate',
													date.format('Y-m-d'));
											deStore.load({
														params : {
															start : 0,
															limit : dePageSize
														}
													});
										}
									}
								}
							}), '-', '截止时间：', new Ext.form.DateField({
								ref : '../endDate',
								format : 'Y-m-d',
								minValue : gridLocal.endDate.getValue()
										.format('Y-m-d').substring(0, 4)
										+ '-01-01',
								maxValue : gridLocal.endDate.getValue()
										.format('Y-m-d').substring(0, 4)
										+ '-12-31',
								fieldLabel : '截止时间',
								showToday : true,
								value : gridLocal.endDate.getValue(),
								anchor : '100%',
								width : 150,
								listeners : {
									'select' : function(dateField, date) {
										if (date < deGrid.startDate.getValue()) {
											Ext.Msg
													.alert("提示",
															"截止时间不能小于起始时间！");
											dateField
													.setValue(dateField.startValue);
										} else {
											delete deStore.baseParams['endDate'];
											deStore.setBaseParam('endDate',
													date.format('Y-m-d'));
											deStore.load({
														params : {
															start : 0,
															limit : dePageSize
														}
													});
										}
									}
								}
							}), '-', '模糊查询：', new Ext.ux.InklingSearchField({
								ref : '../inkling',
								width : 200,
								store : deStore,
								tooltip : {
									title : '模糊查询',
									text : '您可以输入项目编号、课题编号进行模糊查询。'
								},
								onTrigger1Click : function() {

									if (this.hasSearch) {

										this.el.dom.value = '';

										deStore
												.setBaseParam('param',
														lastParam);

										deStore.load({
													params : {
														start : 0,
														limit : pageSize
													}
												});

										this.triggers[0].hide();
										this.hasSearch = false;
									}
								},
								onTrigger2Click : function() {

									// var param =
									// deGrid.store.baseParams.param;

									var v = this.getRawValue();
									if (v.length < 1) {
										Ext.Msg.alert("提示", "请输入查询条件！");
									} else {

										deGrid.store
												.setBaseParam(
														'param',
														lastParam
																+ " and project_code like '%"
																+ v
																+ "%' or citem_id like '%"
																+ v + "%'");

										deStore.load({
													params : {
														start : 0,
														limit : dePageSize
													}
												});
										this.hasSearch = true;
										this.triggers[0].show();
									}
								}
							})],
					bbar : new Ext.PagingToolbar({
								pageSize : 20,
								store : deStore,
								displayInfo : true,
								displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
								emptyMsg : "无显示数据",
								plugins : [deExp, new Ext.ux.PagelimitCombo()]
							})
				});

		if (Ext.getCmp('detailsWinID')) {
			Ext.getCmp('detailsWinID').close();
		}

		var deWin = new Ext.Window({
					id : 'detailsWinID',
					title : title + '明细列表----' + project_code,
					height : 400,
					width : 750,
					closeAction : 'close',
					layout : 'border',
					items : [deGrid]
				});

		deStore.removeAll(true);
		delete deStore.baseParams['param'];
		// delete deStore.baseParams['signal'];

		// if ((project_code == null || project_code == "" || project_code ==
		// "null")
		// && (citem_id == null || citem_id == "" || citem_id == "null")) {
		if (citem_id == null || citem_id == "" || citem_id == "null") {

			if (deptName == '合计') {// '合计'没有明细
				// Ext.Msg.alert("提示", "没有指定部门！");

			} else if (gridLocal.secondDept.getValue() == '所有部门') {
				// 二级部门
				if (null == deptName || "" == deptName || "null" == deptName) {
					Ext.Msg.alert("提示", "没有指定部门！");
				} else {
					deStore.setBaseParam('param', " 执行部门 = '" + deptName + "'");

					deStore.load({
								params : {
									start : 0,
									limit : pageSize
								},
								callback : function() {
									deGrid.getColumnModel().setHidden(1, false);
									deWin.setTitle(title + '明细列表----'
											+ deptName);
									deWin.show();
								}
							});
				}
			} else {
				// 一级部门
				if (null == deptName || "" == deptName || "null" == deptName) {
					Ext.Msg.alert("提示", "没有指定部门！");
				} else {
					deStore.setBaseParam('param', " 部门 = '" + deptName + "'");

					deStore.load({
								params : {
									start : 0,
									limit : pageSize
								},
								callback : function() {
									deGrid.getColumnModel().setHidden(1, false);
									deWin.setTitle(title + '明细列表----'
											+ deptName);
									deWin.show();
								}
							});
				}
			}

			lastParam = deStore.baseParams.param;
		} else {
			// 单个项目
			deStore.setBaseParam('param', " citem_id = '" + citem_id + "'");

			deStore.load({
				params : {
					start : 0,
					limit : pageSize
				},
				callback : function() {
					deGrid.getColumnModel().setHidden(1, true);
					deWin
							.setTitle(title
									+ '明细列表----'
									+ ((project_code == null
											|| project_code == "" || project_code == "null")
											? citem_id
											: project_code));
					deWin.show();
				}
			});
			deGrid.getTopToolbar().hide();
		}
	}
});