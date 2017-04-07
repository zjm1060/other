
//申请日期为原申请日期；查询页面的参数；
Ext.onReady(function() {
	Ext.QuickTips.init();
	var newEditRowFlag = 0;
	var hidenFlag;
	var selectRowNum = '';
	var workItemId = request.getParameter("workItemId");// 
	var processInstanceId = request.getParameter("processInstanceId");
	var missAttendanceId_req = request.getParameter("missAttendanceId")
	var isOfficialTravel_req = request.getParameter("isOfficialTravel");
	var isSelect = (request.getParameter("isSelect")!=null)?true:false;
	var applicantdate_req = request.getParameter("applicantdate");
	var isReturnPage = (workItemId != null) ? true : false;
	
	var lastday_format =lastday<10?'0'+lastday:lastday;//
	
	var draft = new com.bitc.xmis.sys.Draft({
		model : 'missattendance',
		appId : 'step1-',
		userID : __emp.userID,
		text : '【补报考勤】' + (isOfficialTravel_req == 1 ? '考勤补报' : '出差补报')
				+ ' 创建时间' + new Date().format('m-d H:i'),
		url : isOfficialTravel_req == 1
				? (String
						.format('/attdMaint.do?method=showPage&isOfficialTravel=1'))
				: (String
						.format('/attdMaint.do?method=showPage&isOfficialTravel=0'))
	});

	var personByDeptIDCombo = new com.bitc.xmis.Combos.ApplicantCombo({
				fieldLabel : '申请人',
				ref : 'applicant',
				id : 'applicant',
				width : 100,
				hidden : true,
				readOnly : isReturnPage,
				DEPTID : __emp.departmentID,// aaa
				hiddenName : 'applicant'
			});

	var setEditorHide = function(obj) {
		obj.reject(true);
		hidenFlag = false;
	};
	var editGridEditor = new Ext.ux.grid.RowEditor({
				saveText : '确认',
				cancelText : '取消',
				clicksToEdit : 2
			});
	editGridEditor.on({
		scope : this,
		beforeedit : function() {
			contentGridPanel.removeBtn.setDisabled(true);
			contentGridPanel.newBtn.setDisabled(true);
			mainFormPanel.saveBtn.setDisabled(true);
		},
		beforehide : function() {
			var flag = hidenFlag;
			hidenFlag = true;
			return flag;
		},
		afteredit : function(roweditor, changes, record, rowIndex) {
			contentGridPanel.removeBtn.setDisabled(false);
			contentGridPanel.newBtn.setDisabled(false);
			mainFormPanel.saveBtn.setDisabled(false);
			if (isOfficialTravel_req == 1) {

			} else {
				if (record.data.startDate > record.data.stopDate) {
					Ext.MessageBox.alert("提示", "请检查确保开始日期应该结束日期之前，否则无法正常提交~");
					return false;
				}
			}
			if (isReturnPage) {

				if (!record.data.id) {// aaa
					Ext.Ajax.request({
						url : __cxtPath
								+ '/attdMaint.do?method=doSaveMissAttendanceInfo',
						params : {
							missAttendanceId : missAttendanceId_req,
							infoId : record.data.id,
							amorpm : record.data.amorpm,
							missDate : record.data.missDate,
							missDesc : record.data.missDesc,
							startDate : record.data.startDate,
							stopDate : record.data.stopDate
						},
						method : "post",
						success : function(response) {
							var result = Ext.decode(response.responseText);
							if (result.success) {
								var infoId = result.attributes.infoId;
								record.data.id = result.attributes.infoId;
								Ext.Msg.alert(result.messageTitle,
										result.message);
								contentStore.reload();
								parent.xMis.refresh('newtodo');
								parent.xMis.refresh('mytodo');
							} else {
								Ext.Msg.alert(result.messageTitle,
										result.message)
							}
						},
						failure : function(response) {
							Ext.MessageBox.alert("提示", "抱歉!操作失败，请重新操作，并联系管理员");
						}
					});

				} else {
					Ext.Ajax.request({
						url : __cxtPath
								+ '/attdMaint.do?method=doUpdateMissAttendanceInfo',
						params : {
							missAttendanceId : missAttendanceId_req,
							infoId : record.data.id,
							amorpm : record.data.amorpm,
							missDate : record.data.missDate,
							missDesc : record.data.missDesc,
							startDate : record.data.startDate,
							stopDate : record.data.stopDate
						},
						method : "post",
						success : function(response) {
							var result = Ext.decode(response.responseText);
							if (result.success) {
								Ext.Msg.alert(result.messageTitle,
										result.message);

								parent.xMis.refresh('newtodo');
								parent.xMis.refresh('mytodo');
							} else {
								Ext.Msg.alert(result.messageTitle,
										result.message)
							}
						},
						failure : function(response) {
							Ext.MessageBox.alert("提示", "抱歉!操作失败，请重新操作，并联系管理员");
						}
					});

				}

			}
		},
		canceledit : function() {
			if (newEditRowFlag == 1) {
				contentGridPanel.stopEditing(false);
				var s = contentGridPanel.getSelectionModel().getSelections();
				for (var i = 0, r; r = s[i]; i++) {
					contentStore.remove(r);
				};
				hidenFlag = true;
				contentGridPanel.getView().refresh();
				newEditRowFlag = 0;
			}
			contentGridPanel.getView().refresh();
			contentGridPanel.removeBtn.setDisabled(false);
			contentGridPanel.newBtn.setDisabled(false);
			mainFormPanel.saveBtn.setDisabled(false);
			selectRowNum = '';
		}
	});
	var contentEditRecord = new Ext.data.Record.create({
				name : '缺勤日期',
				type : 'date',
				dateFormat : 'Y-m-d'
			}, {
				name : '全日/上班/下班',
				type : 'string'
			}, {
				name : '原因',
				type : 'string'
			}, {
				name : 'id',
				type : 'string'
			});

	// 退回的缺勤信息
	var contentStore = new Ext.data.JsonStore({// aaa
		url : String
				.format(
						'{0}/attdMaint.do?method=getMissAttendanceInfos&&workItemId={1}',
						__cxtPath, workItemId),
		root : 'rows',
		idProperty : 'id',
		autoLoad : true,
		fields : [{
					name : 'missDate',
					type : 'date',
					dateFormat : 'Y-m-d'
				}, 'missDesc', 'id', {
					name : 'amorpm',
					type : 'int'
				}, {
					name : '考勤单ID',
					type : 'string'
				}, {
					name : '是否是代理',
					type : 'int'
				}, {
					name : '申请人ID',
					type : 'int'
				}, {
					name : '申请日期',
					type : 'date'
				}, {
					name : 'startDate',
					type : 'date',
					dateFormat : 'Y-m-d'
				}, {
					name : 'stopDate',
					type : 'date',
					dateFormat : 'Y-m-d'
				}]
	});
	var currentIndex = -1;// 当前选择的行
	var contentGridPanel = new Ext.grid.GridPanel({
		region : 'center',
		border : false,
		store : contentStore,
		autoScroll : true,
		autoExpandColumn : 'missDesc',
		height : 180,
		plugins : [editGridEditor],
		loadMask : true, // 读取数据时的遮罩和提示功能，默认为false
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		tbar : [{
					ref : '../newBtn',
					iconCls : 'add',
					text : '新建',
					handler : function() {
						var data = new contentEditRecord({
									'missDate' : '',
									'amorpm' : 0,
									'missDesc' : '',
									'startDate' : '',
									'stopDate' : '',
									'id' : ''
								});
						editGridEditor.stopEditing();
						contentStore.insert(0, data);
						contentGridPanel.getView().refresh();
						contentGridPanel.getSelectionModel().selectRow(0);
						editGridEditor.startEditing(0);
						newEditRowFlag = 1;
					}
				}, '-', {
					ref : '../removeBtn',
					iconCls : 'remove',
					text : '删除',
					handler : function() {
						editGridEditor.stopEditing();
						var record = contentGridPanel.getSelectionModel()
								.getSelected();
						if (record == 'undefined' || record == undefined) {
							Ext.Msg.alert("未选择", "请先选择！");
							return;
						}
						contentStore.remove(record);
						if (isReturnPage) {

							Ext.Ajax.request({
								url : __cxtPath
										+ '/attdMaint.do?method=doDeleteMissAttendanceInfo',
								params : {
									infoId : record.data.id
								},
								method : "post",
								success : function(response) {
									var result = Ext
											.decode(response.responseText);
									if (result.success) {
										Ext.Msg.alert(result.messageTitle,
												result.message, function() {
													if (contentStore.data.length == 0) {
														Ext.Msg
																.alert('提示',
																		'如果想删除此考勤单，请点击页面上方的“终止考勤单的审批”，结束流程。')
													}

												});
										parent.xMis.refresh('newtodo');
										parent.xMis.refresh('mytodo');
									} else {
										Ext.Msg.alert(result.messageTitle,
												result.message)
									}
								},
								failure : function(response) {
									Ext.MessageBox.alert("提示",
											"抱歉!操作失败，请重新操作，并联系管理员");
								}
							});
						}
					}
				}, '->', '*双击进入编辑状态'],
		colModel : new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({
							width : 23
						}), {
					header : 'id',
					dataIndex : 'id',
					hidden : true
				}, {
					header : '缺勤日期',
					dataIndex : 'missDate',
					hidden : isOfficialTravel_req == 0 ? true : false,
					xtype : 'datecolumn',
					format : 'Y-m-d',
					sortable : true,
					editor : new Ext.form.DateField({
								format : 'Y-m-d',
								minValue : MIN_DATE,
								maxValue : new Date(),
								allowBlank : isOfficialTravel_req == 0
										? true
										: false
							})
				}, {
					header : '全日/上班/下班',
					hidden : isOfficialTravel_req == 0 ? true : false,
					dataIndex : 'amorpm',
					sortable : true,
					renderer : function(value, cellmeta, record) {
						if (value == 1) {
							return '上班';
						} else if (value == 2) {
							return '下班';
						} else {
							return '全日';
						}
					},
					editor : new Ext.form.ComboBox({
								xtype : 'combo',
								displayField : 'text',
								valueField : 'value',
								mode : 'local',
								triggerAction : 'all',
								editable : false,
								store : new Ext.data.ArrayStore({
											fields : ['value', 'text'],
											data : [[0, '全日'], [1, '上班'],
													[2, '下班']]
										})
							})
				}, {
					header : '开始日期',
					dataIndex : 'startDate',
					xtype : 'datecolumn',
					format : 'Y-m-d',
					sortable : true,
					hidden : isOfficialTravel_req == 1 ? true : false,
					editor : new Ext.form.DateField({
								format : 'Y-m-d',
								minValue : MIN_DATE,
								maxValue : formatDate(new Date()),
								allowBlank : isOfficialTravel_req == 1
										? true
										: false
							})
				}, {
					header : '结束日期',
					dataIndex : 'stopDate',
					hidden : isOfficialTravel_req == 1 ? true : false,
					xtype : 'datecolumn',
					format : 'Y-m-d',
					sortable : true,
					editor : new Ext.form.DateField({
								format : 'Y-m-d',
								allowBlank : isOfficialTravel_req == 1
										? true
										: false
							})
				}, {
					header : '事由',
					dataIndex : 'missDesc',
					id : 'missDesc',
					sortable : true,
					editor : new Ext.form.TextField({
								allowBlank : false
							})
				}])

	});

	var mainFormPanel = new Ext.form.FormPanel({
		region : 'center',
		padding : 20,
		ref : 'form',
		labelWidth : 60,
		autoScroll : true,
		labelAlign : 'left',
		border : false,
		defaults : {
			anchor : '90%'
		},
		tbar : {
			xtype : 'toolbar',
			items : [{
				ref : '../saveBtn',
				xtype : 'button',
				text : '提交',
				hidden : isReturnPage,
				iconCls : 'ok',
				handler : function() {

					
					var applicantID = mainFormPanel.form
							.findField('applicantID').getValue();
					var isAgent = mainFormPanel.form.findField('isAgent')
							.getValue();// 是否代理
					if (isAgent == 0) {
						applicantID = Ext.getCmp('applicant').getValue();
						if (applicantID == '') {
							Ext.Msg.alert('提示', '请选择代理申请的申请人。');
							return;
						}
					}
					var store = contentGridPanel.store;
					if (store.data.length == 0) {
						Ext.Msg.alert('提示', '请录入信息后提交。');
						return;
					}
					Ext.MessageBox.confirm('提示', '是否确定提交给领导审批？', function(btn,
							text) {
						if (btn == 'yes') {
							var myMask = new Ext.LoadMask(Ext.getBody(), {msg:"正在提交..."});
 myMask.show();
							Ext.Ajax.request({
								url : __cxtPath
										+ '/attdMaint.do?method=startMissAttence',
								success : function(response) {
									
 myMask.hide();									
									var result = Ext
											.decode(response.responseText);
									if (result.success) {
										draft.remove(function() {
													if (parent && parent.xMis) {
														parent.xMis
																.refresh('mydraft');
													}
												});
										Ext.Msg.alert(result.messageTitle,
												result.message , function() {
													parent.xMis
															.refresh('newtodo');
													parent.xMis
															.refresh('mytodo');
																										parent.xMis
																												.closeTab(window);
												});

									} else {
										Ext.Msg.alert(result.messageTitle,
												result.message)
									}
								},
								failure : function(response) {
 myMask.hide();	
								},
								params : {
									applicantID : applicantID,
									isAgent : isAgent,// 是否代理
									registdate : Ext.util.Format.date(
											new Date(), 'Y-m-d H;i:s'),
									isOfficialTravel : isOfficialTravel_req,
									members : encodeStoreDataWithDate(contentGridPanel.store)
								},
								method : "post"

							});
						}
					});
				}

			}, {
				text : '保存草稿',
				ref : '../saveButton',
				hidden : isReturnPage,
				iconCls : 'save',
				handler : function() {
					var content = {
						form : mainFormPanel.getForm().getValues(),
						grid : getStoreDataWithDate(contentStore)
					}
					draft.save(function(d, result) {
								if (parent && parent.xMis) {
									parent.xMis.refresh('mydraft');
								}
								Ext.Msg.alert('提示',
										'保存草稿成功！<br/>你可以在首页的草稿箱中找到。');
							}, mainFormPanel, content);
				}
			}, {
				ref : '../returnBtn',
				xtype : 'button',
				text : '修改后提交',
				hidden : !isReturnPage,
				iconCls : 'ok',
				handler : function() {
					for (var i = 0; i < contentGridPanel.store.data.length; i++) {
						var missDate = contentGridPanel.store.data.items[i].data.missDate;
						var startDate = contentGridPanel.store.data.items[i].data.startDate;
						if (missDate !== null) {
							if (formatDate(missDate) < MIN_DATE) {
								Ext.Msg
										.alert('提示', '此考勤单的缺勤日期'
														+ formatDate(missDate)
														+ '已过期，应确保在' + MIN_DATE
														+ '日期后');
								return;
							}
						}
						if (startDate !== null) {
							if (formatDate(startDate) < MIN_DATE) {
								Ext.Msg
										.alert(
												'提示',
												'此出差单的开始日期'
														+ formatDate(record.data.missDate)
														+ '已过期，应确保在' + MIN_DATE
														+ '日期后');
								return;
							}
						}

					}

					Ext.MessageBox.confirm('提示', '是否确定提交给领导审批？', function(btn,
							text) {
						if (btn == 'yes') {
							Ext.Ajax.request({
								url : __cxtPath
										+ '/attdMaint.do?method=doGoOnSubmitMissAttence',
								success : function(response) {
									var result = Ext
											.decode(response.responseText);
									if (result.success) {
										Ext.Msg.alert(result.messageTitle,
												result.message, function() {
													parent.xMis
															.closeTab(window);
												});

										parent.xMis.refresh('newtodo');
										parent.xMis.refresh('mytodo');
									} else {
										Ext.Msg.alert(result.messageTitle,
												result.message)
									}
								},
								failure : function(response) {
									Ext.MessageBox.alert("提示",
											"抱歉!操作失败，请重新操作，并联系管理员");
								},
								params : {
									missAttendanceId : missAttendanceId_req,
									workItemId : workItemId
								},
								method : "post"

							});
						}
					});
				}

			}, {
				ref : '../returnBtn',
				xtype : 'button',
				text : '终止此考勤单的审批',
				hidden : !isReturnPage,
				iconCls : 'remove',
				handler : function() {// aaa

					Ext.Ajax.request({
								url : __cxtPath
										+ '/attdMaint.do?method=doEndMissAttence',
								success : function(response) {
									var result = Ext
											.decode(response.responseText);
									if (result.success) {
										Ext.Msg.alert(result.messageTitle,
												result.message, function() {
													parent.xMis
															.refresh('newtodo');
													parent.xMis
															.refresh('mytodo');
													parent.xMis
															.closeTab(window);
												});

									} else {
										Ext.Msg.alert(result.messageTitle,
												result.message)
									}
								},
								failure : function(response) {
									Ext.MessageBox.alert("提示",
											"抱歉!操作失败，请重新操作，并联系管理员");
								},
								params : {
									missAttendanceId : missAttendanceId_req,
									workItemId : workItemId
								},
								method : "post"

							});
				}

			}]
		},
		items : [new com.bitc.xmis.attd.Tip({
							hidden:isSelect?true:false,
							lastday : lastday_format
						}), new com.bitc.xmis.attd.TipWithCondition({
							lastday : lastday_format,
							hidden : isSelect||(Ext.util.Format.date(new Date(), "d") != lastday)
									? true
									: false
						}), new Ext.Panel({
					height : 60,
					border : false,
					hidden : isOfficialTravel_req == 0 ? true : false,
					html : '<div style=\" font-size:30px;text-align:center;\"><font>补报考勤登记表</font></div>'
				}), new Ext.Panel({
					height : 60,
					hidden : isOfficialTravel_req == 1 ? true : false,
					border : false,
					html : '<div style=\"font-size:30px;text-align:center;\"><font>出差人员登记表</font></div>'
				}), {
					layout : 'column',
					xtype : 'panel',
					border : false,
					items : [{
						columnWidth : .2,
						border : false,
						items : [{
							xtype : 'combo',
							id : 'isAgent',
							displayField : 'text',
							valueField : 'value',
							mode : 'local',
							triggerAction : 'all',
							readOnly : isReturnPage,
							value : 1,
							width : 100,
							store : new Ext.data.ArrayStore({
										fields : ['value', 'text'],
										data : [[1, '本人申请'], [0, '代理申请']]
									}),
							listeners : {
								'select' : function(combo, record, index) {
									var selValue = record.data.value;
									if (selValue == 0) {
										mainFormPanel.form
												.findField('applicantname')
												.setVisible(false);
										Ext.getCmp('applicant')
												.setVisible(true);
									} else if (selValue == 1) {
										mainFormPanel.form
												.findField('applicantname')
												.setVisible(true);
										Ext.getCmp('applicant')
												.setVisible(false);
									}
								}
							}
						}]

					}, {
						columnWidth : .25,
						layout : 'form',
						border : false,
						items : [{
									xtype : 'hidden',
									fieldLabel : '申请人ID',
									name : 'applicantID',
									width : 90,
									value : __emp.userID
								}, {
									xtype : 'displayfield',
									fieldLabel : '申请人',
									name : 'applicantname',
									width : 90,
									value : __emp.userName
								}, personByDeptIDCombo]

					}, {
						columnWidth : .25,
						layout : 'form',
						border : false,
						items : [{
									xtype : 'hidden',
									fieldLabel : '所在部门ID',
									name : 'deptID',
									value : __emp.departmentID
								}, {
									xtype : 'displayfield',
									fieldLabel : '所在部门',
									name : 'deptName',
									value : __emp.departmentName
								}]

					}, {
						columnWidth : .3,
						layout : 'form',
						border : false,
						items : [{
									xtype : 'datefield',
									fieldLabel : '申请日期',
									name : 'registdate',
									format : 'Y-m-d',
									readOnly:isSelect?true:false,
									minValue : MIN_DATE,
									value : Ext.util.Format.date(new Date(),
											'Y-m-d')

								}]

					}]
				}, {
					xtype : 'panel',
					region : 'center',
					title : '详细信息',
					height : 200,
					anchor : '90%',
					items : [contentGridPanel]
				}, new Ext.Panel({
							border : false,
							height : 30
						}), com.bitc.xmis.attd.Btip2]
	});
	var members = com.bitc.xmis.util.ExtUtil
			.encodeStoreData(contentGridPanel.store);
	function formatDate(data) {
		return Ext.util.Format.date(data, 'Y-m-d');
	}
	function getStoreDataWithDate(store) {
		var data = [], i = 0;
		store.each(function(record) {
					if (record.data.missDate instanceof Date) {
						record.data.missDate = formatDate(record.data.missDate);
					}
					if (record.data.startDate instanceof Date) {
						record.data.startDate = formatDate(record.data.startDate);
					}
					if (record.data.stopDate instanceof Date) {
						record.data.stopDate = formatDate(record.data.stopDate);
					}
					data[i++] = record.data;
				});
		return data;
	}
	function encodeStoreDataWithDate(store) {
		return Ext.encode(getStoreDataWithDate(store));
	}

	if (isReturnPage || isSelect) {
		var applicantID_req = request.getParameter("applicantID");
		var isAgent_req = request.getParameter("isAgent");
		if (isAgent_req == 1) {
			mainFormPanel.form.findField('isAgent').setValue(1);
				mainFormPanel.form.findField('applicantID')
						.setValue(applicantID_req);
				if (selectedEmployee != null) {
					mainFormPanel.form.findField('applicantname')
							.setValue(selectedEmployee.userName);
				}
				//申请人所在部门
				if (selectedDept != null) {
					mainFormPanel.form.findField('deptID')
							.setValue(selectedDept.departmentID);
					mainFormPanel.form.findField('deptName')
							.setValue(selectedDept.departmentName);
				}
		} else {
			mainFormPanel.form.findField('isAgent').setValue(0);
			mainFormPanel.form.findField('applicantname').setVisible(false);
			personByDeptIDCombo.setVisible(true);
			mainFormPanel.form.findField('applicant').setValue(applicantID_req);
		}
		if (applicantdate_req != null && applicantdate_req != '') {
			mainFormPanel.form.findField('registdate')
					.setValue(applicantdate_req);
		}
	}
	if(isSelect){
		mainFormPanel.getTopToolbar().hide();
		contentGridPanel.getTopToolbar().hide();
	}
	// 初始化草稿
	if (request.getParameter("draftId") != null) {
		var draftId = request.getParameter("draftId");
		draft.id = draftId;
		draft.load(function(d) {
			var c = d.content;
			mainFormPanel.getForm().setValues(c.form);
			if (c.form.isAgent == '本人申请') {
				mainFormPanel.form.findField('isAgent').setValue(1);
			} else {
				mainFormPanel.form.findField('isAgent').setValue(0);
				mainFormPanel.form.findField('applicantname').setVisible(false);
				personByDeptIDCombo.setVisible(true);
				mainFormPanel.form.findField('applicant')
						.setValue(c.form.applicant);
			}
			var records = [];
			Ext.iterate(c.grid, function(item) {
						records.push(new contentStore.recordType(item));
					}, this);
			contentStore.add(records);
		}, this);

	}

	var main = new Ext.Viewport({
				layout : 'border',
				items : mainFormPanel,
				border : false
			});

	if (request.getParameter("todoId") != null) {
		var approvalInfoGridPanel = new com.bitc.xmis.workflow.approvalInfoList(
				{
					collapsible : true,
					width : 700,
					height : 130,
					title : '审批记录',
					autoLoadData : true,
					autoScroll : true,
					processInstanceId : processInstanceId,
					sortInfo : {
						field : '审批时间',
						direction : 'ASC'
					}
				});
		mainFormPanel.insert(1, approvalInfoGridPanel);
	}
//	if (request.getParameter("todoId") != null) {
//		mainFormPanel.insert(0, new com.bitc.xmis.TodoInfoPanel({
//							todoId : request.getParameter("todoId"),
//							frame : true
//						}));
//	}
	// 如果workItem!=null 时候显示
	var step = request.getParameter("processTaskId");
	if (workItemId != null && step != null) {
		mainFormPanel.getTopToolbar()
				.add(new com.bitc.xmis.workflow.TraceButton({
							processInstanceId : processInstanceId
						}));
	}

});
