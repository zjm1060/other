Ext.onReady(function() {
	// Ext.QuickTips.init();//初始化
	var strReason = '';
	var recordInfun;
	var rowInfun;
	var newRowFlag = 0;
	var paymentTotal = 0;
	var projectSdFlag = 0;// 当projectStuatus列表中，选择了自定义的行，则
							// projectSdFlag为1,或修改一行并这行在projectStatusGrid中这行的Selfdefine=ture;
	var projectSelFlag = 0;
	var projectNameChangeFlag = 0;// 仅当出现项目状态名冲突的时候，提示窗口选否，值置1
	var projectNameChangeActionFlag = 0;// 当改变过项目状态，再点查询时，不发生change 事件

	var paymentSt = Ext.data.Record.create([{
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
				name : paymentConditionDateDisplayed
			}, {
				name : paymentConditionPercent
			}, {
				name : paymentConditionPaymentAmt
			}, {
				name : paymentConditionManualFlag,
				type : 'bool'
			}, {
				name : paymentConditionManualReason
			}]);

	/*
	 * ====================================================================
	 * project ;
	 * ====================================================================
	 */

	var projectStore = new Ext.data.GroupingStore({
				url : 'projectMaint.do?method=getProjectStatusStore',
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount',
							remoteSort : true,
							fields : [{
										name : ProjectStatusID
									}, {
										name : ProjectStatusName
									}, {
										name : ProjectStatusDescription
									}, {
										name : SelfDefinitionFlag
									}]
						})
			});

	projectStore.on('load', function(store, records, options) {
		var selectValue = searchProjField.getValue();
		var selectDescValue = paymentNameField.getValue();
		var payRecord = paymentConditionGrid.getSelectionModel().getSelected();
		var arr = [];
		projectSelFlag = 1;
		for (var i = 0; i < records.length; i++) {
			var record = records[i];
			if (projectIdField.getValue() == '') {
				if (selectValue == record.get(ProjectStatusName)
						&& selectDescValue == record
								.get(ProjectStatusDescription)) {
					arr.push(record);
					// break;
				}
			} else {
				if (payRecord.get(paymentConditionProjStatusID) == record
						.get(ProjectStatusID)) {
					arr.push(record);
					break;
				}
			}
		}
		projSM.selectRecords(arr);

		var projRecord = projectStatusGrid.getSelectionModel().getSelected();
		var index = projectStatusGrid.getStore().indexOf(projRecord);
		if (index != -1) {
			projectStatusGrid.getView().focusRow(index);
		}

		for (var j = 0; paymentConditionGrid.getStore().getAt(j) != undefined; j++) {
			// var arrey=[];
			for (var k = 0; k < records.length; k++) {

				var everyRecord = records[k];
				if (everyRecord.get(ProjectStatusID) == paymentConditionGrid
						.getStore().getAt(j).get(paymentConditionProjStatusID)) {
					var changeColorindex = projectStatusGrid.getStore()
							.indexOf(everyRecord);

					projectStatusGrid.getView().getCell(changeColorindex, 1).style.color = '#FF4500';
					projectStatusGrid.getView().getCell(changeColorindex, 3).style.color = '#FF4500';
					projectStatusGrid.getView().getCell(changeColorindex, 4).style.color = '#FF4500';
				}
			}
		}
	}, this, {
		delay : 100
	});
	/*
	 * ====================================================================
	 * project serchField
	 * ====================================================================
	 */
	var searchProjectSField = new Ext.ux.form.SearchField({
				width : 155,
				onTrigger2Click : function() {// 点击查询按钮或回车调用该方法
					projectStore.removeAll();
					projectStore.load({
								params : {
									searchName : searchProjectSField
											.getRawValue()
								},
								callback : function(record, options, success) {
									if (success == true) {
									} else {
										Ext.Msg
												.alert(
														Ext_errMsg_For_Get_Store_Title,
														Ext_errMsg_For_Get_Store_Message);
									}
								}
							});

				}
			});

	var searchProjectSForm = new Ext.form.FormPanel({
				baseCls : 'x-plain',
				anchor : '100%',
				width : 300,

				items : [{
							baseCls : 'x-plain',
							xtype : "panel",
							layout : "column",
							fieldLabel : "项目状态",
							// lableWidth:10,
							isFormField : true,
							items : [searchProjectSField]
						}]

			});

	var projSM = new Ext.grid.CheckboxSelectionModel({
				header : '',
				singleSelect : true,
				listeners : {
					rowselect : function(model, rowIndex, r) {
						/*
						 * if(projectSelFlag==0){ for(var
						 * j=0;paymentConditionGrid.getStore().getAt(j)!=
						 * undefined;j++){ if(r.get(ProjectStatusID)
						 * ==paymentConditionGrid.getStore().getAt(j).get(paymentConditionProjStatusID)){
						 * //projSM.deselectRow(rowIndex); break;
						 *  }; }; } projectSelFlag=0;
						 */
					}
				}
			});

	var projectStatusGrid = new Ext.grid.GridPanel({
		sm : projSM,
		store : projectStore,
		width : 310,
		height : 350,
		region : 'center',
		stripeRows : true, // True表示使用不同颜色间隔行，默认为false
		margins : '0 5 5 5',
		autoExpandColumn : 0,
		forceFit : true,
		listeners : {
			'cellclick' : function(g, rowIndex, columnIndex, e) {
				for (var j = 0; j < paymentConditionGrid.getStore().getCount(); j++) {
					if (projectStatusGrid.getStore().getAt(rowIndex)
							.get(ProjectStatusID) == paymentConditionGrid
							.getStore().getAt(j)
							.get(paymentConditionProjStatusID)) {

						Ext.Msg.alert("提示", "已有此项目状态！");
						break;

					};
				};

			}
		},
		bbar : new Ext.Toolbar({
					width : 305,
					height : 25,
					items : [{
								text : '<font color=#FF4500>*红色项目状态为已定义过的付款条件.</font>'
							}]
				}),

		tbar : [{
			ref : '../okBtn',
			iconCls : 'ok',
			text : '确定',
			handler : function() {
				// projiectNameChangeActionFlag=0;
				projectNameChangeFlag = 0;
				projectSdFlag = 0;
				var record = projectStatusGrid.getSelectionModel()
						.getSelected();
				if (record == 'undefined' || record == undefined) {
					Ext.Msg.alert("提示", "未选择，请先选择！");
					return true;
				};
				if (record.get(SelfDefinitionFlag) == 'true') {
					projectSdFlag = 1;

				};
				/*
				 * for(var j=0;paymentConditionGrid.getStore().getAt(j)!=
				 * undefined;j++){ if(record.get(ProjectStatusID)
				 * ==paymentConditionGrid.getStore().getAt(j).get(paymentConditionProjStatusID)){
				 * Ext.Msg.alert("提示","此状态已选择！"); return false; }; };
				 */
				projectIdField.setValue(record.get(ProjectStatusID));
				paymentNameField.setValue(record.get(ProjectStatusDescription));
				searchProjField.setValue(record.get(ProjectStatusName));
				projectStatusGrid.ownerCt.hide();
				projectNameChangeActionFlag = 0;
			}
		}, '-', {
			ref : '../cancelBtn',
			iconCls : 'remove',
			text : '取消',
			disabled : false,
			handler : function() {
				projectNameChangeActionFlag = 0;
				// Ext.Msg.alert("未选择","请先选择！");return true;
				projectStatusGrid.ownerCt.hide();
			}
		}],

		columns : [projSM, {
					// id: projectStatusName,
					header : '项目状态',
					dataIndex : ProjectStatusName,
					sortable : true
				}, {
					id : 'projecStatusId',
					header : '项目状态ID',
					dataIndex : ProjectStatusID,
					hidden : true,
					sortable : true
				}, {
					id : 'project_status',
					header : '项目状态解释',
					dataIndex : ProjectStatusDescription,
					sortable : true
				}, {
					id : 'selfDefinitionFlag',
					header : '自定义',
					dataIndex : SelfDefinitionFlag,
					hidden : false,
					sortable : true,
					width : 60,
					renderer : changText
				}

		]
	});

	function changText(value) {
		if (value == 'true') {
			return "是";
		} else {
			return " ";
		}

	};

	var projectWin = new Ext.Window({
				layout : 'form',
				modal : true,
				renderTo : Ext.getBody(),
				frame : true,
				plain : true,
				resizable : false,
				buttonAlign : "center",
				closable : false,
				bodyStyle : "padding:10px",
				width : 342,
				height : 420,
				items : [searchProjectSForm, projectStatusGrid]
			});

	var paymentStore = new Ext.data.GroupingStore({
		url : 'contractMain.do?method=getPaymentConditionList',
		reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount',
					remoteSort : true,
					fields : paymentSt
				})
			// ,
			// sortInfo:{field: 'st_py_id', direction: 'ASC'}

		});
	paymentStore.on('load', function(store, records, options) {
				var arr = [];
				for (var i = 0; i < records.length; i++) {
					var record = records[i];
					if (true == record.get(paymentConditionManualFlag)) {

						record.set(paymentConditionManualFlag, false);
					}
				}

			}, this, {
				delay : 0
			})

	var paymentEditor = new Ext.ux.grid.RowEditor({
				saveText : '确认',
				cancelText : '取消',
				clicksToEdit : 2,
				listeners : {
					'validateedit' : function() {
						// this.get('saveBtn').on('click', saveBtnClick, this, {
						// buffer: 700
						// });
					}
				}
			});
	var hideFlag;
	var setEditorHide = function(obj) {
		obj.reject(true);
		hideFlag = false;
	};
	var funInButtonYse = function(record, rowIndex) {

	}
	var reNameFlag = 0;
	var reNameId = '';
	var reNameSelfDFlag = '';
	paymentEditor.on({
		scope : this,
		beforeedit : function() {
			if (strReason != '') {
				strReason = '';
			}
			this.errorText = '错误';
			this.commitChangesText = '请先 确认/取消 当前的更改！';
			// this.btns.saveBtn.setDisabled(true);
			// this.saveBtn.setDisabled(true);
			paymentConditionGrid.removeBtn.setDisabled(true);
			paymentConditionGrid.newBtn.setDisabled(true);
			projectStore.load({
						params : {
							searchName : ''
						},
						callback : function(record, options, success) {
							if (success == true) {
							} else {
								Ext.Msg.alert(Ext_errMsg_For_Get_Store_Title,
										Ext_errMsg_For_Get_Store_Message);
							}
						}
					});

		},
		beforehide : function() {

			var flag = hideFlag;
			hideFlag = true;
			return flag;
		},
		validateedit : function(roweditor, changes, record, rowIndex) {
			// setEditorHide(record);
			// return false;

			if (searchProjField.getValue() == '') {

				Ext.Msg.alert("提示", "请输入项目状态！");
				setEditorHide(record);
				return false;
			};
			if (paymentNameField.getValue() == '') {
				Ext.Msg.alert("提示", "请输入条件描述！");
				setEditorHide(record);
				return false;
			};

		},
		afteredit : function(roweditor, changes, record, rowIndex) {

			if (projectIdField.getValue() != '') {
				// id 不为空
				for (var i = 0; i < projectStatusGrid.getStore().getCount(); i++) {

					if (projectStatusGrid.getStore().getAt(i)
							.get(ProjectStatusID) == projectIdField.getValue()) {
						// id与列表相同 的一项
						if (projectStatusGrid.getStore().getAt(i)
								.get(ProjectStatusName) != searchProjField
								.getValue()) {
							// 此时 该判断新名称是否重名
							for (var j = 0; j < projectStatusGrid.getStore()
									.getCount(); j++) {

								var projOne = projectStatusGrid.getStore()
										.getAt(j);
								if (searchProjField.getValue() == projOne
										.get(ProjectStatusName)) {
									reNameFlag = 1;
									reNameId = projOne.get(ProjectStatusID);
									reNameSelfDFlag = projOne
											.get(SelfDefinitionFlag);
									break;
								}
							};
							if (reNameFlag == 1) {
								// 重名
								projectIdField.setValue(reNameId);
								if (reNameSelfDFlag == 'true') {
									projectSdFlag = 1;
								} else {
									projectSdFlag = 0;
								};
								reNameFlag = 0;
								reNameId = '';
								reNameSelfDFlag = '';

							} else {
								projectIdField.setValue('');
								projectSdFlag = 0;
							}
						} else {
							if (projectStatusGrid.getStore().getAt(i)
									.get(SelfDefinitionFlag) == 'true') {
								projectSdFlag = 1;
							}
						}

					}
				}
			} else {
				// id 为空
				for (var k = 0; k < projectStatusGrid.getStore().getCount(); k++) {
					var projInGrid = projectStatusGrid.getStore().getAt(k);
					if (searchProjField.getValue() == projInGrid
							.get(ProjectStatusName)) {
						reNameFlag = 1;
						reNameId = projInGrid.get(ProjectStatusID);
						reNameSelfDFlag = projInGrid.get(SelfDefinitionFlag);
					}
				};
				if (reNameFlag == 1) {
					// 重名
					projectIdField.setValue(reNameId);
					if (reNameSelfDFlag == 'true') {
						projectSdFlag = 1;
					}
					reNameFlag = 0;
					reNameId = '';
					reNameSelfDFlag = '';

				} else {
					projectIdField.setValue('');
					projectSdFlag = 0;
				}

			};

			var paymented = 0
			for (var j = 0; j < paymentConditionGrid.getStore().getCount(); j++) {
				if (j != rowIndex) {
					paymented = paymented
							+ paymentConditionGrid.getStore().getAt(j)
									.get(paymentConditionPercent) * 1;
				}
			}

			if (paymentPersentField.getValue() > (100 - paymented)) {
				setEditorHide(record);
				Ext.Msg.alert("提示", "输入百分比超额！");

				return false;
			}
			if (record.get(paymentConditionManualFlag) == true) {
				if (paymentManualReason.getValue() == '') {
					Ext.Msg.alert("提示", "请输入手动设置原因！");
					setEditorHide(record);
					return false;
				}
				if (paymentDateField.getValue() == '') {
					Ext.Msg.alert("日期提示", "请输入日期！");
					setEditorHide(record);
					return false;
				}
			}
			if (projectSdFlag == 1) {

				if (paymentDateField.getValue() == '') {
					Ext.Msg.alert("日期提示", "请输入日期！");
					setEditorHide(record);
					return false;
				}
			};
			if (projectIdField.getValue() == '') {
				if (paymentDateField.getValue() == '') {

					// hideFlag = false;
					Ext.Msg.alert("无效日期", "请选择日期！");
					setEditorHide(record);
					return false;
				}
			};

			if (record.get(paymentConditionPercent) == 0
					|| record.get(paymentConditionPaymentAmt) == 0) {
				setEditorHide(record);
				Ext.Msg.alert("无效输入", "输入金额不能为0！");

			} else {

				if (contractIdInPay == 0) {
					Ext.Msg.alert("无合同编号", "请先录入！");
				} else {

					paymentForm.getForm().submit({
						url : 'contractMain.do?method=savePaymentCondition',
						method : 'post',
						params : {
							contractID : contractIdInPay,
							description : paymentNameField.getValue(),
							conditionID : record.get(paymentConditionId),
							projectStatusID : projectIdField.getValue(),
							projectStatusName : searchProjField.getValue(),
							delayDays : record.get(paymentConditionDelayDays),
							paymentDate : record
									.get(paymentConditionDateDisplayed),
							percent : record.get(paymentConditionPercent),
							amt : record.get(paymentConditionPaymentAmt),
							manualFlag : record.get(paymentConditionManualFlag),
							manualReason : paymentManualReason.getValue()
						},
						success : function(form, action) {
							Ext.Msg.alert(action.result.messageTitle,
									action.result.message);

							newRowFlag = 0;
							paymentConditionGrid.getStore().reload({
								callback : function(record, options, success) {
									if (success == true) {
									} else {
										Ext.Msg
												.alert(
														Ext_errMsg_For_Get_Store_Title,
														Ext_errMsg_For_Get_Store_Message);
									}
								}
							});
							// paymentConditionGrid.getView().refresh();
						},
						failure : function(form, action) {
							Ext.Msg.alert(action.result.messageTitle,
									action.result.message);
							var s = paymentConditionGrid.getSelectionModel()
									.getSelections();
							for (var i = 0, r; r = s[i]; i++) {
								paymentStore.remove(r);
							};
							// paymentConditionGrid.getView().refresh();
							newRowFlag = 0;
						}
					});
					paymentConditionGrid.getStore().reload({
						callback : function(record, options, success) {
							if (success == true) {
							} else {
								Ext.Msg.alert(Ext_errMsg_For_Get_Store_Title,
										Ext_errMsg_For_Get_Store_Message);
							}
						}
					});

					projectSdFlag = 0;
					paymentConditionGrid.removeBtn.setDisabled(false);
					paymentConditionGrid.newBtn.setDisabled(false);
					// paymentConditionGrid.getView().refresh();

				}
			}

		},
		canceledit : function() {
			searchProjField.setValue("请输入");
			paymentNameField.setValue("请输入");

			projectNameChangeFlag = 0;

			if (newRowFlag == 1) {
				paymentEditor.stopEditing(false);
				var s = paymentConditionGrid.getSelectionModel()
						.getSelections();
				for (var i = 0, r; r = s[i]; i++) {
					paymentStore.remove(r);
				};
				hideFlag = true;
				paymentConditionGrid.getView().refresh();
				newRowFlag = 0;
			}
			projectSdFlag = 0;
			paymentConditionGrid.getView().refresh();

			var btnDisabled;
			if (collapsible == false && collapsed == false) {
				// btnDisabled = true;
			} else {
				// btnDisabled = false;
				paymentConditionGrid.removeBtn.setDisabled(false);
				paymentConditionGrid.newBtn.setDisabled(false);
			}

		}

	});

	// var strReason = '';

	var paymentCheckbox = new Ext.grid.CheckColumn({
				header : "手动设置",
				dataIndex : paymentConditionManualFlag,
				align : 'center',
				width : 70,
				editor : {
					xtype : 'checkbox',
					value : 'false',
					listeners : {
						'check' : function() {
							// var strReason = ''; //还需要置空
							if (this.getValue() == true) {
								if (paymentManualReason.getValue() != '') {
									strReason = paymentManualReason.getValue();
									paymentManualReason.setValue('');
								}

							} else {
								if (strReason != '') {
									paymentManualReason.setValue(strReason);
								}
							}

						}

					}
				}
			});
	var idValue;
	var searchProjField = new Ext.ux.form.SearchField({
				allowBlank : false,
				// store: paymentStore,
				emptyText : "必输项...",
				hasSearch : false,
				validateOnBlur : true,
				width : 125,
				listeners : {
					'change' : function() {
					}

				},

				onTrigger2Click : function() {// 点击查询按钮或回车调用该方法
					projectWin.show();
					projectWin.setTitle("项目状态选择");
					projectNameChangeFlag = 0;
					projectNameChangeActionFlag = 1;
					searchProjectSField.setValue('');
					projectStore.load({
								params : {
									searchName : searchProjectSField
											.getRawValue()
								},
								callback : function(record, options, success) {
									if (success == true) {
									} else {

										Ext.Msg
												.alert(
														Ext_errMsg_For_Get_Store_Title,
														Ext_errMsg_For_Get_Store_Message);
									}
								}
							});
				}
			});
	var paymentManualReason = new Ext.form.TextField({
				emptyText : "手动设置原因..."
			});

	var paymentNameField = new Ext.form.TextField({
				allowBlank : false,
				emptyText : "必输项..."
			});
	var dateChangeFlag = 0;
	var paymentDateField = new Ext.form.DateField({
				format : 'Y-m-d',
				listeners : {
					'focus' : function() {
						dateChangeFlag = 0;
					},

					'select' : function() {
						if (dateChangeFlag == 0) {
							today = new Date().format('Y-m-d');
							if (paymentDateField.getValue().format('Y-m-d') < today) {
								dateChangeFlag = 1;
								Ext.MessageBox.show({
											title : "日期提示",
											msg : "日期早于当日，是否继续？",
											buttons : Ext.Msg.YESNO,
											fn : function(btn) {
												if (btn == 'no') {
													paymentDateField
															.setValue('');
												};
											},
											width : 260,
											icon : Ext.MessageBox.INFO,
											closable : false
										});
							}
						}
					},
					'change' : function() {
						if (dateChangeFlag == 0) {

							if (paymentDateField.getValue() == '') {
								return false;
							};
							today = new Date().format('Y-m-d');

							if (paymentDateField.getValue().format('Y-m-d') < today) {
								dateChangeFlag = 1;
								Ext.MessageBox.show({
											title : "日期提示",
											msg : "日期早于当日，是否继续？",
											buttons : Ext.Msg.YESNO,
											fn : function(btn) {
												if (btn == 'no') {
													paymentDateField
															.setValue('');
												};
											},
											width : 260,
											icon : Ext.MessageBox.INFO,
											closable : false
										});
							}
						}
					}
				}
			});
	var paymentDelayDaysField = new Ext.form.NumberField({
				value : 0,
				minValue : 0,
				decimalPrecision : 0,
				allowDecimals : false,
				nanText : "请输入整数",
				enableKeyEvents : true,
				allowBlank : false

			})
	var projectIdField = new Ext.form.TextField({});
	var paymentPersentField = new Ext.form.NumberField({
				minValue : 0,
				maxValue : 100,
				decimalPrecision : 0,
				allowDecimals : false,
				nanText : "请输入整数",
				enableKeyEvents : true,
				listeners : {
					'change' : function() {
						paymentAmtField.setValue(contractAmtInPay
								* paymentPersentField.getValue() / 100);
					},
					'keypress' : function() {
					}
				}
			});
	var paymentAmtField = new Ext.form.NumberField({
				minValue : 0,
				allowBlank : false,
				listeners : {
					'change' : function() {
						if (contractAmtInPay.value > 0) {
							// int i
							// =paymentAmtField.getValue()/MM_findObj("contractAmt").value*100;
							paymentPersentField.setValue(Math
									.round(paymentAmtField.getValue()
											/ contractAmtInPay * 100));
						}
					}
				}
			});

	var paymentConditionGrid = new Ext.grid.GridPanel({
				renderTo : paymentConditionDivName,
				title : "付款条件定义",
				store : paymentStore,
				// width: 800,
				layout : 'fit',
				width : Ext.get(paymentConditionDivName).getWidth(),
				height : 185,
				region : 'center',
				stripeRows : true, // True表示使用不同颜色间隔行，默认为false
				margins : '0 5 5 5',
				enableColumnResize : true,
				autoExpandColumn : ['delay_days'],
				plugins : [paymentEditor],

				// forceFit:true,
				autoWidth : true,
				stateful : true,
				stateId : 'paymentconditiongrid',
				forceFit : true, // Ture表示自动扩展或缩小列的宽度以适应grid的宽度,从而避免出现水平滚动条.
				loadMask : {
					msg : '正在加载数据，请稍侯……'
				},
				viewConfig : {
					forceFit : true
				},
				collapsible : collapsible,
				collapsed : collapsed,

				view : new Ext.grid.GroupingView({
							markDirty : false
						}),
				listeners : {
					'afterrender' : function() {
						if (paymentCondiFlag == 1) {
							this.getTopToolbar().hide();
						}
					},
					'expand' : function() {
						paymentConditionGrid.focus("true", 100);
						// contractInfopanelOther.ownerCt.scrollTo(0,100);

						// this.scroller.dom.scrollTop =200;
					},
					'rowdblclick' : function(gr, rowIndex, e) {
						if (paymentCondiFlag == 1) {
							return false;
						}
					}
				},
				tbar : [{
					ref : '../newBtn',
					iconCls : 'add',
					text : '新建',
					handler : function() {
						if (contractIdInPay == 0) {
							Ext.Msg.alert("无合同编号", "请先录入！");
							return true;
						};

						if (paymentTotal >= 0 && paymentTotal <= 100) {
							var payPersent = 100 - paymentTotal;
							var payAmt = payPersent * contractAmtInPay / 100;
						}
						var e = new paymentSt({
									paymentConditionId : '',
									paymentConditionDescription : '',
									paymentConditionProjStatusID : '',
									paymentConditionProjStatusName : '',
									paymentConditionDelayDays : "0",
									paymentConditionDateDisplayed : (new Date())
											.format('Y-m-d'),
									paymentConditionPercent : payPersent,
									paymentConditionPaymentAmt : payAmt,
									paymentConditionManualFlag : false
								});
						paymentEditor.stopEditing();
						paymentStore.insert(0, e);
						paymentConditionGrid.getView().refresh();
						paymentConditionGrid.getSelectionModel().selectRow(0);
						paymentEditor.startEditing(0);

						newRowFlag = 1;
						hideFlag = false;
						paymentPersentField.setValue(payPersent);
						paymentAmtField.setValue(payAmt);
						paymentDelayDaysField.setValue('0');
					}
				}, '-', {
					ref : '../removeBtn',
					iconCls : 'remove',
					text : '删除',
					disabled : false,
					handler : function() {
						var record = paymentConditionGrid.getSelectionModel()
								.getSelected();
						if (record == 'undefined' || record == undefined) {
							Ext.Msg.alert("未选择", "请先选择！");
							return true;
						}

						paymentForm.getForm().submit({
							url : 'contractMain.do?method=delPaymentCondition',
							method : 'post',
							params : {
								contractID : contractIdInPay,
								conditionID : record.get(paymentConditionId)
							},
							success : function(form, action) {
								Ext.Msg.alert(action.result.messageTitle,
										action.result.message);

								/*
								 * stEditor.stopEditing();
								 * 
								 * var s =
								 * stGrid.getSelectionModel().getSelections();
								 * for(var i = 0, r; r = s[i]; i++){
								 * stStore.remove(r); };
								 */
								paymentConditionGrid.getStore().reload({
									callback : function(record, options,
											success) {
										if (success == true) {
										} else {
											Ext.Msg
													.alert(
															Ext_errMsg_For_Get_Store_Title,
															Ext_errMsg_For_Get_Store_Message);
										}
									}
								});
								paymentConditionGrid.getView().refresh();
								newRowFlag = 0;

							},
							failure : function(form, action) {
								Ext.Msg.alert(action.result.messageTitle,
										action.result.message);
							}
						});

					}
				}, '->', "*双击进入编辑状态"
				/*
				 * ,'-',{ ref: '../refreshBtn', iconCls: 'x-tbar-loading', text:
				 * '刷新', disabled: false, handler: function(){ //
				 * paymentConditionGrid.hide();
				 * paymentConditionGrid.getTopToolbar().hide(); }
				 *  }
				 */
				],

				colModel : new Ext.grid.ColumnModel({

							columns : [new Ext.grid.RowNumberer({
												width : 23
											}), {
										id : 'projectId',
										header : '项目ID',
										dataIndex : paymentConditionProjStatusID,
										hidden : true,
										width : 20,
										sortable : true,
										editor : projectIdField
									}, {
										id : 'project_status',
										header : '对应项目状态',
										dataIndex : paymentConditionProjStatusName,
										sortable : true,
										editor : searchProjField
									}, {
										id : 'payment_name',
										header : '付款条件描述',
										// width:170,
										dataIndex : paymentConditionDescription,
										sortable : true,
										editor : paymentNameField
									}, {
										id : 'delay_days',
										header : '延迟天数',
										align : 'right',
										dataIndex : paymentConditionDelayDays,
										sortable : true,
										editor : paymentDelayDaysField
									}, {
										header : '收款日期',
										align : 'center',
										dataIndex : paymentConditionDateDisplayed,
										xtype : 'datecolumn',
										// renderer: formatDate,
										format : 'Y-m-d',
										sortable : true,
										editor : paymentDateField
									}, {
										xtype : 'numbercolumn',
										header : '收款百分比',
										align : 'right',
										format : '0%',
										dataIndex : paymentConditionPercent,
										editor : paymentPersentField,
										sortable : true
									}, {
										xtype : 'numbercolumn',
										align : 'right',
										format : '0,000.00',
										header : '收款金额',
										// renderer:
										// Ext.util.Format.Currency('￥'),
										// renderer: function(value, cellMeta,
										// record, rowIndex, columnIndex,
										// store){
										// return
										// formatCurrency(MM_findObj("currencyType").options[MM_findObj("currencyType").selectedIndex].name,
										// value, '');
										// },
										dataIndex : paymentConditionPaymentAmt,
										sortable : true,
										editor : paymentAmtField
									}, paymentCheckbox, {

										header : '设置原因',
										dataIndex : '',
										sortable : true,
										dataIndex : paymentConditionManualReason,
										editor : paymentManualReason
									}]
						})
			});
	this.setTbarDisable = function() {
		paymentConditionGrid.getTopToolbar().hide();

	};
	var paymentForm = new Ext.form.FormPanel({
				id : 'paymentConditionFrm',
				renderTo : paymentConditionDivName,
				baseCls : 'x-plain',
				anchor : '100%',
				items : [paymentConditionGrid]
			});

	paymentStore.load({
				params : {
					contractID : contractIdInPay
				},
				callback : function(record, options, success) {
					if (success == true) {
					} else {
						Ext.Msg.alert(Ext_errMsg_For_Get_Store_Title,
								Ext_errMsg_For_Get_Store_Message);
					}
				}
			});

	paymentStore.on('load', function(store, records, options) {
		var arr = [];
		paymentTotal = 0;
		for (var i = 0; i < records.length; i++) {

			var record = records[i];
			paymentTotal = paymentTotal + record.get(paymentConditionPercent)
					* 1;
		}

		if (paymentTotal > 100) {
			var s1 = String
					.format('<tr class=info><font color=red>付款定义超额</tr>');
			paymentConditionGrid.setTitle("付款条件定义" + "--" + s1
					+ (paymentTotal - 100) + '%');

		} else {
			if (paymentTotal >= 0 && paymentTotal < 100) {
				var s2 = String.format('<tr class=info><font color=red> </tr>');
				var s3 = String
						.format('<tr class=info><font color=red> 合同金额未定义</tr>');
				paymentConditionGrid.setTitle("付款条件定义" + "--" + s2
						+ (100 - paymentTotal) + '%' + s3);
			};
			if (paymentTotal == 100) {
				var s = String
						.format('<tr class=info><font color=green>付款条件定义完成</tr>');
				paymentConditionGrid.setTitle("付款条件定义" + "--" + s);
			}

		}

		if (paymentTotal > 0 && MM_findObj("amt")) {
			// alert("abs");
			MM_findObj("amt").readOnly = true;
			amtReadOnly = true;
		}

		if (collapsible == false && collapsed == false) {
			paymentConditionGrid.removeBtn.setVisible(false);
			paymentConditionGrid.newBtn.setVisible(false);
			paymentConditionGrid.doLayout();

		}

	}, this, {
		delay : 100
	});

});
// var contractIdInPay = document.contractFrm.all.contractID.value;
