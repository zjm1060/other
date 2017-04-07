Ext.define('bitc.wrenv.inputview.yuqiaoPlanEditPanel', {
    extend: 'Ext.form.Panel',
    padding: '5 5 5 5',
    bodyPadding: 10,
    border: false,
    autoScroll: true,
    water_body_name: '',
    plan_id: null,
    currentUser: null,
    defaults: {
        anchor: '100%',
        labelWidth: 100
    },
    initComponent: function () {
        var me = this;
        var heduanStore = Ext.create('Ext.data.Store', {
            fields: ['heduanName'],
            proxy: {
                type: 'ajax',
                url: __path + '/inputview/getHeDuanListData.do',
                reader: {
                    type: 'json',
                    root: 'rows'
                },
                extraParams: {water_body_id: me.water_body_id}
            },
            autoLoad: true
        });
        this.defaultType = 'textfield';
        var required = '<span style="color:red;font-weight:bold" data-qtip="Required">*</span>';
        this.fieldDefaults = {
            labelStyle: 'font-size:14px;',
            labelAlign: 'right',
            msgTarget: 'under',
            width: 300
        };
        this.items = [{
            xtype: 'fieldcontainer',
            fieldLabel: '方案名称',
            layout: {
                type: 'hbox',
                defaultMargins: {
                    top: 0,
                    right: 5,
                    bottom: 5,
                    left: 0
                }
            },
            items: [{
                xtype: 'textfield',
                hideLabel: true,
                name: 'plan_name',
                allowBlank: false
                //width: 200
            }, {
                xtype: 'textfield',
                fieldLabel: '制定时间',
                readOnly: true,
                allowBlank: false,
                name: 'plan_makeTime',
                width: 400,
                value: Ext.util.Format.date(new Date(), 'Y-m-d')
            }, {
                xtype: 'textfield',
                fieldLabel: '方案作者',
                readOnly: true,
                allowBlank: false,
                name: 'plan_owner',
                value: this.currentUser,
                width: 400
            }]
        }, {
            xtype: 'fieldcontainer',
            fieldLabel: '运算年份',
            layout: {
                type: 'hbox',
                defaultMargins: {
                    top: 0,
                    right: 5,
                    bottom: 5,
                    left: 0
                }
            },
            items: [{
                xtype: 'textfield',
                hideLabel: true,
                name: 'run_year',
                allowBlank: false,
                listeners: {
                    scope: this,
                    change: function (text, newValue, oldValue, eOpts) {
                        this.fireEvent('yearchange', newValue);
                    }
                }
                //width: 200
            }, {
                xtype: 'textfield',
                readOnly: true,
                fieldLabel: '运算对象',
                allowBlank: false,
                value: this.water_body_name,
                submitValue: false,
                name: 'run_obj_name',
                width: 400
            }, {
                xtype: 'hidden',
                value: this.water_body_id,
                name: 'run_obj_id'
            }, {
                xtype: 'combo',
                store: heduanStore,
                //editable: false,
                hidden:true,
                allowBlank: true,
                valueField: 'heduanName',
                displayField: 'heduanName',
                triggerAction: 'all',
                fieldLabel: '运算河段',
                name: 'run_segment',
                width: 400
            }]
        }, {
            xtype: 'hidden',
            name: 'plan_id',
            value: this.plan_id
        }, {
            xtype: 'textareafield',
            fieldLabel: '方案描述',
            name: 'plan_description',
            anchor: '60%'
        }];
        this.tbar = [{
            xtype: 'button',
            text: '保存方案',
            scope: this,
            handler: this.onSave
        }, {
            xtype: 'button',
            text: '运行方案',
            scope: this,
            handler: this.onRun
        }];
        this.addEvents('savesuccess');
        this.callParent(arguments);
    },
    onSave: function () {
        if (this.getForm().isValid()) {
            this.getForm().submit({
                url: __path + '/inputview/addModelPlan.do',
                method: 'post',
                waitMsg: '正在保存，请稍候...',
                scope: this,
                success: function (form, action) {
                    if (action.result.success) {
                        var plan_id = action.result.attributes.result;
                        this.getForm().findField('plan_id').setValue(plan_id);
                        this.fireEvent('savesuccess', plan_id);
                    } else {
                        Ext.Msg.alert('错误', action.result
                            ? action.result.message
                            : '保存时服务器出现错误，请与管理员联系。');
                    }
                },
                failure: function (form, action) {
                    Ext.Msg.alert('错误', action.result
                        ? action.result.message
                        : '保存时服务器出现错误，请与管理员联系。');
                }
            });
        } else {
            Ext.Msg.alert('提示', '请检查输入项是否正确！');
        }
    },
    onRun: function () {
        if (this.getForm().findField('plan_id').getValue()) {
            Ext.Ajax.request({
                url: __path + '/inputview/runYuQiaoPlan.do',
                scope: this,
                params: {
                    plan_id: this.plan_id
                },
                success: function (response, action) {
                    var res = Ext.decode(response.responseText);
                    if (res.success) {
                        Ext.Msg.alert('提示', '执行成功，方案正在后台运行！');
                    } else {
                        Ext.Msg.alert('提示', res.message);
                    }
                }
            });
        } else {
            Ext.Msg.alert('提示', '请先保存方案');
        }
    },
    requestDataFromDB: function () {
        Ext.Ajax.request({
            url: __path + '/inputview/getYuQiaoPlanData.do',
            method: 'POST',
            scope: this,
            params: {
                planId: this.plan_id
            },
            success: function (response) {
                var result = Ext.decode(response.responseText);
                if (result.success) {
                    var data = result.attributes.data;
                    if (data) {
                        var fields = ['plan_name', 'plan_makeTime', 'run_year', 'run_segment', 'plan_description','plan_owner','run_obj_name','run_obj_id'];
                        for (var i = 0; i < fields.length; i++) {
                            this.getForm().findField(fields[i]).setValue(data[fields[i]]);
                        }
                    }
                    this.fireEvent('savesuccess',this.plan_id);
                }
            }
        })
    },
    afterFirstLayout: function () {
        if (this.plan_id) {
            this.requestDataFromDB();
        }
        this.callParent(arguments);
    }
});
