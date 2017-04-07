Ext.define('bitc.report.NewReportEditPanel', {
    extend: 'Ext.form.Panel',
    url: __path + '/report/new.do',
    frame: true,
    width: '800',
    defaultType: 'textfield',
    autoScroll: true,
    bodyPadding: 10,
    initComponent: function () {
        this.preParameCount = 0;
        var me = this;
        var paramCount = 0;
        this.items = [{xtype: 'hidden', name: 'id'}, new Ext.form.ComboBox({
            allowBlank: false,
            name: 'dbName',
            fieldLabel: '数据库',
            displayField: 'dname',
            valueField: 'name',
            triggerAction: 'all',
            emptyText: '选择已注册的数据库',
            editable: false,
            store: new Ext.data.JsonStore({
                fields: ['name', 'dname'],
                proxy: {
                    type: 'ajax',
                    url: __path + '/report/dbs.do'
                }
            })
        }), {
            fieldLabel: '报表标识',
            name: 'name',
            emptyText: '输入报表的唯一标识',
            allowBlank: false
        }, {
            fieldLabel: '报表标题',
            allowBlank: false,
            name: 'title'
        }, {
            xtype: 'radiogroup',
            fieldLabel: 'SQL类型',
            name: 'sqlTypes',
            items: [
                {boxLabel: '普通sql', name: 'sqlType', inputValue: 'NORMARL', checked: true},
                {boxLabel: '存储过程', name: 'sqlType', inputValue: 'PROCEDURE'}
            ]
        }, {
            xtype: 'textarea',
            fieldLabel: 'SQL语句',
            name: 'sql',
            allowBlank: false,
            anchor: '90%',
            height: '200',
            listeners: {
                blur: function (componet, obj, option) {
                    if (componet.getValue() != null && componet.getValue() != '') {
                        Ext.Ajax.request({
                            url: '/report/getParametersCount.do',
                            params: {
                                sql: componet.getValue(),
                                sqlType: me.getForm().findField('sqlTypes').getValue()
                            },
                            success: function (response) {
                                var result = Ext.decode(response.responseText);
                                var count = paramCount = result.attributes.parameCount;
                                if (result.success) {
                                    me.preParameCount = count;
                                } else {
                                    Ext.Msg.alert(result.title, result.message);
                                    return;
                                }
                                var addFields = [];
                                var items = me.items;
                                items.each(function (item) {
                                    if (item.name && item.name.match('report_args_contains') != null) {
                                        me.remove(item.id);
                                    }
                                });
                                for (var i = 1; i <= count; i++) {
                                    var addField = {
                                        xtype: 'fieldcontainer',
                                        name: 'report_args_contains' + i,
                                        fieldLabel: '参数' + i + ' 初始值',
                                        allowBlank: false,
                                        layout: {
                                            type: 'hbox',
                                            defaultMargins: {
                                                top: 0,
                                                right: 5,
                                                bottom: 0,
                                                left: 0
                                            }
                                        },
                                        items: [{
                                            xtype: 'textfield',
                                            name: 'defaultValue' + i,
                                            hideLabel: true
                                        }, {
                                            xtype: 'radiogroup',
                                            width: 200,
                                            labelWidth: 70,
                                            fieldLabel: '参数类型',
                                            items: [
                                                {
                                                    boxLabel: 'IN',
                                                    name: 'parameterType' + i,
                                                    inputValue: 'IN',
                                                    checked: true
                                                },
                                                {boxLabel: 'OUT', name: 'parameterType' + i, inputValue: 'OUT'}
                                            ]
                                        }, {
                                            xtype: 'textfield',
                                            name: 'rep_arg_type' + i,
                                            fieldLabel: '数据类型'
                                        }, {
                                            xtype: 'hidden',
                                            name: 'rep_sort' + i,
                                            value: i
                                        }],
                                    };
                                    addFields.push(addField);
                                }
                                me.add(addFields);
                            }
                        });
                    }
                }
            }
        }, {
            xtype: 'radiogroup',
            fieldLabel: '是否分页',
            items: [
                {boxLabel: '否', name: 'isPaging', inputValue: 'false', checked: true},
                {boxLabel: '是', name: 'isPaging', inputValue: 'true'}
            ]
        }, {
            xtype: 'textfield',
            fieldLabel: '插件',
            name: 'plugins',
            allowBlank: true,
            anchor: '90%'
        }];
        this.dockedItems = [{
            xtype: 'toolbar',
            dock: 'top',
            items: [{
                text: '提交',
                handler: function () {
                    if (this.getForm().isValid()) {
                        this.getForm().submit({
                            waitMsg: '正在处理……',
                            waitTitle: '请稍候',
                            params: {
                                paramCount: paramCount
                            },
                            success: function (form, action) {
                                Ext.Msg.alert(action.result.title, action.result.message);
                                this.fireEvent('savesuccess');
                            },
                            failure: function (form, action) {
                            }
                        });
                    } else {
                        Ext.Msg.alert('提示', '根据提示完成后重试！');
                    }
                },
                scope: this
            }]
        }];
        this.addEvents('savesuccess');
        this.callParent(arguments);
    },

});
