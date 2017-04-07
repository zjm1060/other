Ext.onReady(function() {
	/**
	 * 登录界面
	 */
	
	/**
	 * IIS发布
	 * checkUsercode,submitLogin都需要提供webservice.
	 * */
	var form = new Ext.FormPanel({
		padding : 20,
		baseCls : 'x-plain',
		buttonAlign : 'center',
		// monitorValid :
		// true,//monitorValid=true表示为通过不断触发一个事件，来监视有效值的状态（在客户端进行）
		items : [{
			xtype : 'textfield',
			name : 'usercode',
			fieldLabel : '用户名',
			msgTarget : 'under',
			allowBlank : false,
			blankText : '不能为空!',
			validator : function(value) {
				var isUsercodeOk = true;
				if (value != '') {
					Ext.Ajax.request({
								url : '/Woix/ws?method=checkUsercode',//checkUsercode是servlet提供的，需要提供webservice
								params : {
									usercode : value.trim()
								},
								success : function(response, opt) {
									if (!Ext.decode(response.responseText).success) {
										form.getForm().findField('usercode')
												.markInvalid('用户名不存在！');
										isUsercodeOK = false;
									} else {
										form.getForm().findField('usercode')
												.clearInvalid();
										isUsercodeOK = true;
									}
								}
							});
				}
				return isUsercodeOk;
			}
		}, {
			xtype : 'textfield',
			inputType : 'password',
			msgTarget : 'under',
			name : 'password',
			fieldLabel : '密码',
			allowBlank : false,
			blankText : '不能为空!',
			listeners : {
				specialkey : function(f, e) {
					if (e.getKey() == e.ENTER) {
						submitLogin();
					}
				}
			}
		}],
		buttons : [{
					text : '注册',
					handler : function() {
						loginWin.hide();
						window.top.location.href = "/Woix/registration.html";
					}
				}, {
					text : '登录',
					handler : function() {
						submitLogin();
					}
				}]
	});
	var loginWin = new Ext.Window({
				width : 400,
				height : 180,
				title : '登录',
				items : [form]
			}).show();

	function submitLogin() {
		if (!form.getForm().isValid()) {
			return;
		}
		form.getForm().submit({
					waitMsg : '正在登录，请稍候...',
					url : '/Woix/ws?method=submitLogin',//submitLogin是servlet提供的，需要提供webservice
					success : function(f, action) {
						if (action.result.success) {
							loginWin.hide();
							window.top.location.href = "/Woix/main.html";
						}
					},
					failure : function(f, action) {
						Ext.Msg.alert('错误', action.result.message);
					}
				});
	}
});