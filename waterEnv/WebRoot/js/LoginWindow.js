Ext.define('bitc.wrenv.LoginWindow', {
    extend: 'Ext.window.Window',
    requires : ['Ext.ux.layout.Center'],
    layout : 'ux.center',
    modal : false,
    width : 450,
    height : 200,
    title : '用户登录',
    iconCls : 'icon-login-16',
    closeAction : 'close',
    closable : false,
    initComponent: function () {
        var me = this;
        me.items = [{
            xtype : 'form',
            border : false,
            width : 400,
            height : 200,
            layout : {
                type : 'table',
                columns : 3
            },
            items : [{
                xtype : 'container',
                width : 100,
                padding : '0 0 0 18',
                items : new Ext.Img({
                    src : __path + '/resources/image/login_64.png'
                })

            }, {
                xtype : 'panel',
                border : false,
                bodyPadding : '20 20 20 20',
                width : 200,
                items : [{
                    xtype : 'textfield',
                    name : 'name',
                    fieldLabel : '用户名',
                    emptyText : '请输入用户名',
                    labelAlign : 'top',
                    anchor : '95%',
                    allowBlank : false,
                    enableKeyEvents : true,
                    listeners : {
                        scope : this,
                        keyup : function(field, e) {
                            if (e.getKey() == Ext.EventObject.ENTER) {
                                this.onLogin();
                            }
                        }
                    }
                }, {
                    xtype : 'textfield',
                    fieldLabel : '密码',
                    name : 'password',
                    emptyText : '请输入密码',
                    labelAlign : 'top',
                    inputType : 'password',
                    allowBlank : false,
                    anchor : '95%',
                    enableKeyEvents : true,
                    listeners : {
                        scope : this,
                        keyup : function(field, e) {
                            if (e.getKey() == Ext.EventObject.ENTER) {
                                this.onLogin();
                            }
                        }
                    }
                }]
            }, {
                xtype : 'button',
                iconCls : 'icon-login-btn',
                text : '登录',
                scale : 'large',
                iconAlign : 'top',
                scope : this,
                handler : me.onLogin
            }]
        }];
        this.addEvents('beforelogin', 'login');
        this.callParent(arguments);
    },
    onLogin : function() {
        if (!this.fireEvent('beforelogin', this)) {
            return;
        }
        var form = this.down('form');
        form.getForm().submit({
            url : __path + '/login.do',
            method : 'post',
            waitMsg : '正在登录，请稍候...',
            scope : this,
            success : function(form, action) {
                this.fireEvent('login', this, action.result);
            },
            failure : function(form, action) {
                if (action.failureType === Ext.form.action.Action.CONNECT_FAILURE) {
                    Ext.Msg.alert('网络较慢或者服务器繁忙，请重试或与管理员联系。');
                }
                Ext.Msg.alert('错误', action.result && action.result.message ? action.result.message : '登录错误，请重试或与管理员联系。');
            }
        });
    }
});