window.cID;
showWin = function (store, record) {
	var payForm = new Ext.FormPanel({
		frame : true,
		region : 'center',
		lableWidth : 60,
		border : false,
		items : [{
			xtype : 'label',
			bodyStyle : "padding:2px,2px,2px,2px",
			html : '<table><tr><td width=40px><font color=red>提示 : </font></td><td><font style="font-size:12px;">如金额不符，请使用‘合同款收付登记’功能处理。</font></td></tr></table><hr>'
		}, {
			xtype : 'datefield',
			bodyStyle : "padding:2px",
			fieldLabel : '收款日期',
			name : 'imcomeDate',
			anchor : '90%',
			format : 'Y-m-d',
			ref : 'imcomeDate'
		}, {
			fieldLabel : '收款金额',
			xtype : 'label',
			style : "margin:4px",
			readOnly : true,
			anchor : '90%',
			name : 'incomeAmt',
			ref : 'incomeAmt'
		}, {
			xtype : 'hidden',
			name : 'operID',
			ref : 'operID'
		}, new Ext.ux.form.ChooseOrgTreeField({
					ref : 'oper',
					name : 'oper',
					anchor : '90%',
					fieldLabel : '经  办  人',
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
					clearValueFun : function() {
						payForm.operID.setValue(null);
						payForm.oper.setValue(null);
					},
					setValueFun : function(result) {
						var empName = '';
						var empID = '';
						for (var i = 0; i < result.length; i++) {
							if (i == result.length - 1) {
								empID = empID + result[i].id;
								empName = empName + result[i].text;
							} else {
								empID = empID + result[i].id + ',';
								empName = empName + result[i].text + ','
							}
						}
						payForm.operID.setValue(empID.split('-')[2]);
						payForm.oper.setValue(empName);
					}
				}), {
			fieldLabel : '备&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;注',
			xtype : 'textfield',
			anchor : '90%',
			name : 'payRemark',
			ref : 'payRemark'
		}, {
			fieldLabel : '登  记  人',
			anchor : '90%',
			xtype : 'label',
			style : "margin:5px",
			readOnly : true,
			name : 'regster',
			ref : 'regster'
		}]
	});
	//var ctitle = record.get(contractName) == '' ? '' : '(' + record.get(contractName) + ')';
	var win = new Ext.Window({
		title : '合同款收款登记',// + ctitle,
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
		buttons : [{
			text : "确认",
			handler : function() {
				if (Ext.isEmpty(payForm.operID.getValue())) {
					Ext.Msg.alert("提示", "请选择经办人！");
					return;
				}
				payForm.getForm().submit({
					url : __cxtPath
							+ '/contractMain.do?method=savePaymentDetailQuickly',
					method : 'post',
					params : {
						contractID : cID,
						paymentDetailDate : payForm.imcomeDate.getValue(),
						paymentDetailAmt : record.get('estimatedAmt'),
						paymentDetailRemark : payForm.payRemark.getValue(),
						transatorID : payForm.operID.getValue(),
						sourceType : '2',
						receiptID : record.get('ID')
					},
					success : function(form, action) {
						Ext.Msg.alert(action.result.messageTitle,
								action.result.message);
						win.close();
						store.reload();
					},
					failure : function(form, action) {
						Ext.Msg.alert(action.result.messageTitle,
								action.result.message);
					}
				});
			}
		}, {
			text : "取消",
			handler : function() {
				win.close();
			}
		}]
	});

	//win.setPosition(320, 80);
	win.show();
	payForm.imcomeDate.setValue(Ext.util.Format.date(new Date(), 'Y-m-d'));
	var eamt = Ext.util.Format.number(record.get('estimatedAmt'),'0,000.00');
	payForm.incomeAmt.setText(eamt, false);
	payForm.regster.setText(__emp.userName, false);
}