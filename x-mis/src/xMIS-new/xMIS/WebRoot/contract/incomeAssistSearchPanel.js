Ext.ns('com.bitc.xmis.contract');

function showWin(store,record){
	var payForm = new Ext.FormPanel({
			frame : true,
			region : 'center',
			lableWidth : 60,
			border : false,
			items : [{
				xtype : 'label',
				bodyStyle : "padding:2px,2px,2px,2px",
				html : '<table><tr><td width=40px><font color=red>提示 : </font></td><td>如金额不符，请使用‘合同款收付登记’功能处理，并修</td></tr><tr><td></td><td>改相关记录的‘发票执行预估-是否已付款状态’</td></tr></table><hr>'
			},{
				xtype : 'datefield',
				bodyStyle : "padding:2px",
				fieldLabel : '收款日期',
				name : 'imcomeDate',
				anchor : '90%',
				format : 'Y-m-d',
				id : 'imcomeDate'
			},{
				fieldLabel : '收款金额',
				xtype : 'label',
				style : "margin:4px",
				readOnly : true,
				anchor : '90%',
				name : 'incomeAmt',
				id : 'incomeAmt'
			},{
				xtype : 'hidden',
				name : 'operID',
				id : 'operID'
			},new Ext.ux.form.ChooseOrgTreeField({
				id : 'oper',
				name : 'oper',
				anchor : '90%',
				fieldLabel : '经  办  人',
				winConfig : {
					title : '选择经办人'
				},
				singleSelect : true,
				onlyPerson : true,
				allowBlank : false,
				rootConfig : function(){
					return {
						id : 'comp-old-1'
					}
				},
				dataUrl : '/orgMaint.do?method=getOrganizationTree',
				setValueFun : function(result) {
					var empName = '';
					var empID = '';
					for(var i=0; i < result.length;i++){
						if(i==result.length-1){
							empID = empID + result[i].id;
							empName = empName + result[i].text ;
						}else{
							empID = empID + result[i].id + ',';
							empName = empName + result[i].text + ','
						}
					}
					Ext.getCmp('operID').setValue(empID.split('-')[2]);
					Ext.getCmp('oper').setValue(empName);
				},
				clearValueList : 'operID,oper',
				name : ''
				})
			,{
				fieldLabel : '备&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;注',
				xtype : 'textfield',
				anchor : '90%',
				name : 'payRemark',
				id : 'payRemark'
			},{
				fieldLabel : '登  记  人',
				anchor : '90%',
				xtype : 'label',
				style : "margin:5px",
				readOnly : true,
				name : 'regster',
				id : 'regster'
			}]
		});
	var win = new Ext.Window({
		title : '合同款收款登记('+record.get(contractShortName)+')',
		modal : true,
		width : 380,
		height : 270,
		frame : true,
		border : false,
		layout : 'border',
		plain : true,
		resizable : false,
		buttonAlign : "center",
		bodyStyle : "padding:10px",
		items : [payForm],
		listeners : {
			'close' : function(){
				record.set(isFlag,false);
			}
		},
		buttons : [{
			text : "确认",
			handler : function(){
				if(Ext.isEmpty(Ext.getCmp('operID').getValue())){
					Ext.Msg.alert("提示","请选择经办人！");
					return ;
				}
				payForm.getForm().submit(
				{
					url : __cxtPath
							+ '/contractMain.do?method=savePaymentDetailQuickly',
					method : 'post',
					params : {
						contractID : record.get(contractID),
						paymentDetailID : '',
						contractType : 1,
						paymentDetailDate : Ext.getCmp('imcomeDate').getValue(),
						paymentDetailAmt : record.get(antipateAmt)!=0?record.get(antipateAmt):record.get(invoiceAmt),
						paymentDetailRemark : Ext.getCmp('payRemark').getValue(),
						transatorID : Ext.getCmp('operID').getValue(),
						opType : record.get(antipateAmt)!=0?'update':'new',
						invoiceNumber : record.get(invoiceNumber),
						ID : record.get(ID)
					},
					success : function(form, action) {
						Ext.Msg.alert(
							action.result.messageTitle, action.result.message);
						win.close();
						store.reload();
					},
					failure : function(form, action) {
						Ext.Msg.alert(
							action.result.messageTitle, action.result.message);
					}
				});
			}
		},{
			text : "取消",
			handler : function(){
				record.set(isFlag,false);
				win.close();
			}
		}]
	});
	
	win.setPosition(320, 80);
	win.show();
	Ext.getCmp('imcomeDate').setValue(Ext.util.Format.date(new Date(),'Y-m-d'));
	Ext.getCmp('incomeAmt').setText(record.get(antipateAmt)!=0?record.get(antipateAmt):record.get(invoiceAmt),false);
	Ext.getCmp('regster').setText(empName,false);
}

com.bitc.xmis.contract.incomeAssistSearchPanel = Ext
		.extend(
				Ext.Panel,
				{
					initComponent : function() {
						this.layout = 'border';
						var grid = this.buildGrid();
						this.items = [ grid ];
						com.bitc.xmis.contract.incomeAssistSearchPanel.superclass.initComponent
								.call(this);
					},
					buildGrid : function() {

						var incomeAssistRecord = Ext.data.Record.create( [ {
							name : ID
						}, {
							name : contractID
						}, {
							name : projectNo
						}, {
							name : invoiceNumber
						}, {
							name : contractShortName
						}, {
							name : invoiceAmt
						}, {
							name : antipateAmt
						}, {
							name : registrationTime,
							type : 'date',
							dateFormat : 'Y-m-d H:i:s.u'
						}, {
							name : remark
						}, {
							name : payerAccounts
						}, {
							name : companyID
						}, {
							name : companyShortName
						}, {
							name : isFlag,
							type : 'bool'
						}, {
							name : contractMsg
						}, {
							name : createTime,
							type : 'date',
							dateFormat : 'Y-m-d H:i:s.u'
						} ]);

//						var incomeAssistStore = new Ext.data.GroupingStore(
//								{
//									url : __cxtPath + '/contractMain.do?method=getIncomeAssistList',
//									baseParams : {
//										start : 0,
//										limit : 20,
//										sort : contractID,
//										dir : 'DESC'
//									},
//									reader : new Ext.data.JsonReader( {
//										root : 'rows',
//										totalProperty : 'totalCount',
//										remoteSort : true
//									}, incomeAssistRecord),
//									groupField : contractMsg,
//									listeners : {
//										'update' : function(store,record,operation){
//											if(record.get(isFlag)){
//												Ext.MessageBox.confirm("提示","本操作将 ￥"+(record.get(antipateAmt)!=0?record.get(antipateAmt):record.get(invoiceAmt))+" 自动做为合同到款登记处理,请确认?",function(btn){
//													if (btn == 'yes') {
//														showWin(store,record);
//													}else{
//														record.set(isFlag,false);
//													}
//												});
//											}else{
//												record.reject(true);
//											}
//										}
//									}
//								});
						
						var incomeAssistStore = new Ext.data.Store(
								{
									url : __cxtPath + '/contractMain.do?method=getIncomeAssistList',
									baseParams : {
										start : 0,
										limit : 20,
										sort : contractID,
										dir : 'DESC'
									},
									reader : new Ext.data.JsonReader( {
										root : 'rows',
										totalProperty : 'totalCount',
										remoteSort : true
									}, incomeAssistRecord),
									listeners : {
										'update' : function(store,record,operation){
											if(record.get(isFlag)){
												Ext.MessageBox.confirm("提示","本操作将 ￥"+(record.get(antipateAmt)!=0?record.get(antipateAmt):record.get(invoiceAmt))+" 自动做为合同到款登记处理,请确认?",function(btn){
													if (btn == 'yes') {
														showWin(store,record);
													}else{
														record.set(isFlag,false);
													}
												});
											}else{
												record.reject(true);
											}
										}
									}
								});

						var incomeAssistCheckbox = new Ext.grid.CheckColumn(
								{
									header : "是否已付款",
									dataIndex : isFlag,
									align : 'center',
									sortable : true,
									width : 70
								});

						var incomeAssistGrid = new Ext.grid.GridPanel(
								{
									store : incomeAssistStore,
									region : 'center',
									border : false,
									plugins: incomeAssistCheckbox,
        							clicksToEdit: 1,
									stripeRows : true, // True表示使用不同颜色间隔行，默认为false
									loadMask : true, // 读取数据时的遮罩和提示功能，默认为false
									iconCls : 'icon-grid',
									viewConfig : {
										forceFit : true
									},
//									view : new Ext.grid.GroupingView(
//											{
//												forceFit : true,
//												groupByText : '根据本列分组',
//												showGroupName : false,
//												showGroupsText : '是否采用分组显示',
//												groupTextTpl : '{text}(共{[values.rs.length]}项)'
//											}),
									loadMask : {
										msg : '正在加载数据，请稍侯……'
									},
									columns : [
											new Ext.grid.RowNumberer( {
												width : 35
											}),
											{
												header : "项目编号",
												//hidden : true,
												width : 200,
												sortable : true,
												dataIndex : projectNo
											},
											{
												header : "合同名称",
												//hidden : true,
												width : 100,
												sortable : true,
												dataIndex : contractShortName
											},
											{
												header : "开票日期",
												width : 100,
												align : 'center',
												sortable : true,
												dataIndex : createTime,
												renderer : function(v) {
													return Ext.util.Format
															.date(v, 'Y-m-d');
												}
											},
											{
												header : "<font color=#008000>发票金额</font>",
												width : 120,
												align : 'right',
												sortable : true,
												dataIndex : invoiceAmt,
												renderer : function(value,cellmeta, record,rowIndex, columnIndex,store) {
													if (Ext.isEmpty(value)) {
														return '';
													}else{
														value = '<font color=#008000> ' + Ext.util.Format.number(value,'0,000.00') + '</font>';
													}
													var contractName = record.get(contractShortName);
													var index = value
															.indexOf("^");
													var tmp = "<a href='javascript:;' onclick=\"javascript:turnToEditContract('{0}','{1}',{2});\">{3}</a>";
													if (index >= 0) {
														var text = value
																.substring(0,
																		index);
														var url = value
																.substring(
																		index + 1,
																		value.length);
														return String
																.format(
																		tmp,
																		record.get(contractID),
																		contractName,
																		1 ,
																		text);
													}
													return String
															.format(
																	tmp,
																	record.get(contractID),
																	contractName,
																	1 ,
																	value);
												}
											},
											{
												header : "预计付款时间",
												width : 100,
												align : 'center',
												sortable : true,
												dataIndex : registrationTime,
												renderer : function(v) {
													return Ext.util.Format
															.date(v, 'Y-m-d');
												}
											},
											{
												header : "<font color=#FF00FF>预计付款金额</font>",
												width : 120,
												sortable : true,
												align : 'right',
												sortable : true,
												dataIndex : antipateAmt,
												renderer : function(value,
														cellmeta, record,
														rowIndex, columnIndex,
														store) {
													if (value == 0) {
														return '';
													} else {
														return '<font color=#FF00FF>' + Ext.util.Format
																.number(value,
																		'0,000.00') + '</font>';
													}
												}
											},
											{
												header : "<font color=#8A2BE2>付款帐号</font>",
												width : 150,
												hidden : true,
												sortable : true,
												dataIndex : payerAccounts
											}, {
												header : "公司名称",
												width : 150,
												hidden : true,
												sortable : true,
												dataIndex : companyShortName
											}, incomeAssistCheckbox, {
												header : "备注",
												width : 100,
												sortable : true,
												dataIndex : remark
											}
//											, {
//												header : "合同信息",
//												width : 100,
//												hidden : true,
//												dataIndex : contractMsg
//											}
											],
									tbar : [
											'模糊查询：',
											new Ext.ux.InklingSearchField(
													{
														width : 120,
														store : incomeAssistStore,
														tooltip : {
															title : '模糊查询',
															text : '您可以输入任意关键词进行查询。<br/>可以在关键字前加运算符：>、<、>=、<=、=、<>、like等。<br/>按日期请按如下格式输入：<br/>2010-01-01'
														},
														inklings : [ {
															name : '预计付款金额',
															op : '=',
															type : 'double'
														}, {
															name : '发票金额',
															op : '=',
															type : 'double'
														}, {
															name : '付款人帐号',
															op : 'like',
															type : 'string'
														}, {
															name : '公司名称',
															op : 'like',
															type : 'string'
														}, {
															name : '项目编码',
															op : 'like',
															type : 'string'
														}],
														onTrigger1Click : function() {
															if (this.hasSearch) {
																this.el.dom.value = '';
																var o = {
																	start : 0
																};
																this.store.baseParams = this.store.baseParams
																		|| {};
																this.store.baseParams[this.paramName] = '';
																this.store
																		.reload( {
																			params : o
																		});
																this.triggers[0]
																		.hide();
																this.hasSearch = false;
															}
														},
														onTrigger2Click : function() {
															var v = this
																	.getRawValue();
															var params = '';
															Ext
																	.iterate(
																			this.inklings,
																			function(
																					item,
																					index,
																					allItems) {
																				params += item.name
																						+ ','
																						+ item.op
																						+ ','
																						+ item.type;
																				if (index < allItems.length - 1) {
																					params += '|';
																				}
																			},
																			this);
															var o = {
																start : 0
															};
															this.store.baseParams = this.store.baseParams
																	|| {};
															this.store.baseParams[this.paramName] = v;
															this.store.baseParams['inklings'] = params;
															this.store.reload( {
																params : o
															});
															this.hasSearch = true;
															if (v.length > 0) {
																this.triggers[0]
																		.show();
															}
														}
													}) ],
									bbar : new Ext.PagingToolbar(
											{
												pageSize : 20,
												store : incomeAssistStore,
												displayInfo : true,
												displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
												emptyMsg : "无显示数据"
											})
								});
						return incomeAssistGrid;
					}
				});