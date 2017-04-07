Ext.onReady(function() {
	/**
	 * 新用户注册页面
	 */
	
	/**
	 * IIS发布
	 * checkUsercode,createUser均为servlet实现，需要提供webservice
	 * */
	var form = new Ext.FormPanel({
		padding : 20,
		baseCls : 'x-plain',
		buttonAlign : 'center',
		items : [{
			xtype : 'textfield',
			name : 'usercode',
			fieldLabel : '用户',
			msgTarget : 'under',
			width : 180,
			allowBlank : false,
			blankText : '不能为空！',
			validator : function(value) {
				var isUsercodeOK = true;
				if (value != '') {
					Ext.Ajax.request({
								url : '/Woix/ws?method=checkUsercode',
								params : {
									usercode : value.trim()
								},
								success : function(response, opt) {
									if (Ext.decode(response.responseText).success) {
										form.getForm().findField('usercode')
												.markInvalid('用户名已被占用！');
										isUsercodeOK = false;
									} else {
										form.getForm().findField('usercode')
												.clearInvalid();
										isUsercodeOK = true;
									}
								}
							});
				}
				return isUsercodeOK;
			}
		}, {
			xtype : 'textfield',
			name : 'password',
			inputType : 'password',
			fieldLabel : '密码',
			msgTarget : 'under',
			width : 180,
			allowBlank : false,
			blankText : '不能为空！'
		}, {
			xtype : 'textfield',
			name : 'passwordConfirm',
			inputType : 'password',
			fieldLabel : '密码确认',
			msgTarget : 'under',
			width : 180,
			allowBlank : false,
			blankText : '不能为空！',
			validator : function(value) {
				if (form.getForm().findField('password').getValue() != value) {
					return false;
				} else {
					return true;
				}
			},
			invalidText : '两次密码不一致！'
		}, {
			xtype : 'combo',
			name : 'role',
			fieldLabel : '角色',
			displayField : 'value',
			valueField : 'value',
			value : '普通用户',
			width : 180,
			lazyRender : true,
			mode : 'local',
			triggerAction : 'all',
			editable : false,
			store : new Ext.data.ArrayStore({
						fields : ['value'],
						data : [['普通用户']]
					})
		}, {
			xtype : 'textfield',
			name : 'username',
			fieldLabel : '姓名',
			msgTarget : 'under',
			width : 180,
			allowBlank : false,
			blankText : '不能为空！'
		}, {
			xtype : 'textarea',
			name : 'survey',
			fieldLabel : '用户概况',
			msgTarget : 'under',
			allowBlank : false,
			blankText : '不能为空！',
			width : 180,
			height : 70
		}],
		buttons : [{
					text : '创建',
					handler : function(b, e) {
						creatUser();
					}
				}, {
					text : '重填',
					handler : function() {
						clearForm();
					}
				}]
	});

	var registrationWin = new Ext.Window({
				width : 400,
				height : 325,
				title : '注册',
				items : [form]
			}).show();

	function creatUser() {
		if (!form.getForm().isValid()) {
			return;
		}
		form.getForm().submit({
					waitMsg : '请稍后....',
					url : '/Woix/ws?method=createUser',
					success : function() {
						Ext.Msg.alert("提示", "注册成功", function() {
									registrationWin.hide();
									window.top.location.href = "/Woix/login.html";
								});
					},
					failure : function() {
						Ext.Msg.alert("提示", "注册失败，请重新注册");
					}
				});
	}

	function clearForm() {
		form.getForm().findField('usercode').setValue('');
		form.getForm().findField('password').setValue('');
		form.getForm().findField('passwordConfirm').setValue('');
		form.getForm().findField('role').setValue('');
		form.getForm().findField('username').setValue('');
		form.getForm().findField('survey').setValue('');
	}
});