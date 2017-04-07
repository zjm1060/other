Ext.ns('com.bitc.xmis');
//readOnly      true为只读，false或不填为可增删改
//secondTitle   Grid上的Title
//contractID    必填参数

com.bitc.xmis.delayReasonGridPanel = Ext
		.extend(
				Ext.Panel,
				{
					initComponent : function() {
						if (this.contractID) {
							this.layout = 'border';
							var grid = this.buildGrid();
							this.items = [ grid ];
						}
						com.bitc.xmis.delayReasonGridPanel.superclass.initComponent
								.call(this);
					},
					buildGrid : function() {
						var newInfo = false; // 是否新建数据
						
						var ID = "ID";
						var ContractID = "合同ID";
						var delayReason = "原因";
						var OperatorID = "经办人ID";
						var OperatorName = "经办人";
						var DateOfRegistration = "登记日期";
						var Registrant = "登记人ID";
						var RegistrantName = "登记人";
						
						var conOpType;//合同的操作类型，判断是否有‘双击编辑’权限  conOpType=1，没有权限

					var delayRecord = Ext.data.Record.create( [ {
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

					var projectStore = new Ext.data.Store(
							{
								url : __cxtPath + '/contractMain.do?method=getDelayReasonListByContractID',
								baseParams : {
									contractID : this.contractID
								},
								autoLoad : true,
								reader : new Ext.data.JsonReader( {
									root : 'rows',
									totalProperty : 'totalCount',
									remoteSort : true
								}, delayRecord),
								listeners : {
									//判断合同的操作类型
									'beforeload' : function(){
										if (typeof (contractOpType) === 'undefined') {
											Ext.Ajax.request({
												url : __cxtPath
														+'/contractMain.do?method=getContractOpType',
												params : {
													contractID : projectStore.baseParams.contractID
												},
												success : function(response,opt){
													var obj = Ext.decode(response.responseText);
													var type = obj.attributes.contractOpType;
													if(type !=5){
														delayGrid.getTopToolbar().disable(true);
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
											if(contractOpType !=5){
												delayGrid.getTopToolbar().disable(true);
												conOpType = 1;
											}
										}
									}
								}
							});

					var delayEditor = new Ext.ux.grid.RowEditor( {
						saveText : '确认',
						cancelText : '取消',
						clicksToEdit : 2
					});

					delayEditor
							.on( {
								scope : this,
								beforeedit : function(roweditor, changes,
										record, rowIndex) {
									delayGrid.removeBtn.setDisabled(true);
									delayGrid.newBtn.setDisabled(true);
									if(conOpType == 1){
										return false;
									}
								},
								validateedit : function(roweditor, changes,
										record, rowIndex) {
									if ((reason.getValue()).trim() == '') {

										Ext.Msg.alert("提示", "请输入原因！");
										setEditorHide(record);
										return false;
									}
									;
								},
								afteredit : function(roweditor, changes,
										record, rowIndex) {
									delayForm
											.getForm()
											.submit(
													{
														url : __cxtPath + '/contractMain.do?method=saveDelayInfo',
														method : 'post',
														params : {
															ID : record.get(ID),
															contractID : this.contractID,
															reason : reason
																	.getValue(),
															operator : operID
																	.getValue()
														},
														success : function(
																form, action) {
															Ext.Msg
																	.alert(
																			action.result.messageTitle,
																			action.result.message);
															delayGrid
																	.getStore()
																	.reload();
															delayGrid.removeBtn
																	.setDisabled(false);
															delayGrid.newBtn
																	.setDisabled(false);
															operID.setValue(0);
														},
														failure : function(
																form, action) {
															Ext.Msg
																	.alert(
																			action.result.messageTitle,
																			action.result.message);
															delayGrid.removeBtn
																	.setDisabled(false);
															delayGrid.newBtn
																	.setDisabled(false);
														}
													});

								},
								canceledit : function() {
									delayEditor.stopEditing(false);
									if (newInfo) {
										var s = delayGrid.getSelectionModel()
												.getSelections();
										for ( var i = 0, r; r = s[i]; i++) {
											projectStore.remove(r);
										}
										;
									}
									newInfo = false;
									delayGrid.getView().refresh();
									delayGrid.removeBtn.setDisabled(false);
									delayGrid.newBtn.setDisabled(false);
								}
							});

					var reason = new Ext.form.TextField( {
						allowBlank : false,
						emptyText : "原因..."

					});

					var operID = new Ext.form.TextField( {
						id : 'operID'
					});

					var delayDateField = new Ext.form.TextField( {
						format : 'Y-m-d',
						disabled : true,
						align : 'center'
					});

					var oper = new Ext.ux.form.ChooseOrgTreeField( {
						fieldLabel : '经办人',
						id : 'operName',
						anchor : '98%',
						winConfig : {
							title : '选择经办人'
						},
						singleSelect : true,
						onlyPerson : true,
						allowBlank : false,
						rootConfig : function() {
							return {
								id : 'comp-old-1'
							}
						},
						dataUrl : '/orgMaint.do?method=getOrganizationTree',
						setValueFun : function(result) {
							var empName = '';
							var empID = '';
							for ( var i = 0; i < result.length; i++) {
								if (i == result.length - 1) {
									empID = empID + result[i].id;
									empName = empName + result[i].text;
								} else {
									empID = empID + result[i].id + ',';
									empName = empName + result[i].text + ','
								}
							}
							Ext.getCmp('operID').setValue(empID.split('-')[2]);
							Ext.getCmp('operName').setValue(empName);
						},
						clearValueList : 'operID,operName'
					});

					var delayGrid = new Ext.grid.GridPanel(
							{
								title : (undefined == this.secondTitle||"undefined" == this.secondTitle || '' == this.secondTitle )?'':this.secondTitle,
								store : projectStore,
								region : 'center',
								plugins : readOnly == true ? []
										: [ delayEditor ],
								enableColumnResize : true, // 设置为false可以禁止这个表格的列改变宽度 (默认值为 true).
								stripeRows : true, // True表示使用不同颜色间隔行，默认为false
								loadMask : true, // 读取数据时的遮罩和提示功能，默认为false
								loadMask : {
									msg : '正在加载数据，请稍侯……'
								},
								columns : [ new Ext.grid.RowNumberer(), {
									header : "推迟原因",
									width : 400,
									dataIndex : delayReason,
									editor : reason,
									renderer : function(value, cellMeta, record, rowIndex, columnIndex,
									store){
									if(value!=null){
										var baseNum = 18;
										var msg = '';
										for(var i = 0 ; i < value.length/baseNum ; i++){
											msg += value.substr(i*baseNum,baseNum) + "<br/>";
										}
										cellMeta.attr = 'ext:qtitle="推迟原因" ext:qtip="'+msg+'"';
									}
										return value;
									}
								}, {
									header : "经办人",
									width : 120,
									dataIndex : OperatorName,
									editor : oper
								}, {
									header : "登记日期",
									width : 80,
									align : 'center',
									dataIndex : DateOfRegistration
								}, {
									header : "登记人",
									width : 115,
									dataIndex : RegistrantName
								} ],
								tbar : [
										{
											ref : '../newBtn',
											iconCls : 'add',
											text : '新建',
											handler : function() {
												newInfo = true;
												var e = new delayRecord( {
													ID : '',
													contractID : '',
													reason : '',
													operator : ''
												});
												delayEditor.stopEditing();
												projectStore.insert(0, e);
												delayGrid.getView().refresh();
												delayGrid.getSelectionModel()
														.selectRow(0);
												delayEditor.startEditing(0);
											}
										},
										'-',
										{
											ref : '../removeBtn',
											iconCls : 'remove',
											text : '删除',
											disabled : false,
											handler : function() {
												var record = delayGrid
														.getSelectionModel()
														.getSelected();
												if (record == 'undefined'
														|| record == undefined) {
													Ext.Msg.alert("未选择",
															"请先选择！");
													return true;
												} else {
													Ext.Msg
															.confirm(
																	"请确认",
																	"确定要删除该条推迟原因吗？",
																	function(
																			btn) {
																		if (btn == 'yes') {
																			delayForm
																					.getForm()
																					.submit(
																							{
																								url : __cxtPath + '/contractMain.do?method=deleteDelayReasonInfo',
																								method : 'post',
																								params : {
																									ID : record
																											.get(ID)
																								},
																								success : function(
																										form,
																										action) {
																									Ext.Msg
																											.alert(
																													action.result.messageTitle,
																													action.result.message);
																									delayGrid
																											.getStore()
																											.reload();
																									delayGrid
																											.getView()
																											.refresh();
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
												}
											}
										}, '->', "*双击进入编辑状态" ]
							});

					var delayForm = new Ext.form.FormPanel( {
						region : 'center',
						layout : 'border',
						items : [ delayGrid ]
					});
					
					if (readOnly) {
//						delayGrid.getTopToolbar().hide();
						return delayGrid;
					}else{
						return delayForm;
					}
				}

				});
