Ext.define('bitc.wrenv.inputview.inputSzpjChangguiEditPanel', {
    extend: 'Ext.form.Panel',
    border: false,
    plan_id: null,
    autoScroll: true,
    maxHeight: 550,
    defaults: {
        anchor: '98%',
        labelWidth: 80,
        xtype: 'fieldset',
        layout: 'anchor',
        padding: '5 5 5 5',
        bodyPadding: 10
    },
    initComponent: function () {
        var heduan = this.heduan = Ext.create('bitc.wrenv.inputview.inputDayinDuanMianPanel');
        var wuranyuan = this.wuranyuan = Ext.create('bitc.wrenv.inputview.inputDayinDuanMianPanel');
        var jiangjieheshifang = this.jiangjieheshifang = Ext.create('Ext.form.FieldSet', {
            title: '降解和释放系数信息',
            anchor: '98%',
            autoScroll: true,
            maxHeight: 300,
            xtype: 'fieldset',
            layout: 'anchor',
            padding: '5 5 5 5',
            bodyPadding: 10,
            items: []
        });
        var chushitiaojian = this.chushitiaojian = Ext.create('Ext.form.FieldSet', {
            title: '初始条件信息',
            anchor: '98%',
            autoScroll: true,
            maxHeight: 300,
            xtype: 'fieldset',
            layout: 'anchor',
            padding: '5 5 5 5',
            bodyPadding: 10,
            items: []
        });
        var wuranwuList = ['', 'BOD5', 'NBOD', '氨氮(NH3-N)', '亚硝酸盐氮', '硝酸盐氮', 'DO', '化学需氧量(COD)'];
        for (var i = 1; i < 8; i++) {
            jiangjieheshifang.add(this.getWuRanWuConf('污染物' + i, 'wrw_' + i + '_conf', wuranwuList[i]));
        }
        for (var i = 1; i < 8; i++) {
            chushitiaojian.add(this.getChushiConf('污染物' + i, 'chushi_' + i + '_conf', wuranwuList[i]));
        }
        var bianjie = this.bianjie = Ext.create('bitc.wrenv.inputview.inputBianjiePanel', {plan_id: this.plan_id});
        var wuranwu = this.wuranwu = Ext.create('Ext.form.FieldContainer', {
            //xtype: 'fieldcontainer',
            autoScroll: true,
            defaults: {
                labelWidth: 100
            },
            layout: {
                type: 'hbox',
                hideLabel: false,
                defaultMargins: {
                    top: 0,
                    right: 5,
                    bottom: 5,
                    left: 0
                }
            },
            items: [{
                xtype: 'fieldcontainer',
                defaults: {
                    labelWidth: 60
                },
                layout: {
                    type: 'hbox',
                    hideLabel: false,
                    defaultMargins: {
                        top: 0,
                        right: 5,
                        bottom: 5,
                        left: 0
                    }
                },
                items: [{
                    xtype: 'checkbox',
                    name: 'wrw1',
                    disabled:true,
                    submitValue: false,
                    boxLabel: '污染物1',
                    width: 80,
                    listeners: {
                        scope: this,
                        change: function (box, newValue) {
                            if (newValue) {
                                jiangjieheshifang.items.each(function (item) {
                                    if (item.wrwName == '污染物1') {
                                        item.show();
                                    }
                                });
                                chushitiaojian.items.each(function (item) {
                                    if (item.chushiName == '污染物1') {
                                        item.show();
                                    }
                                });
                                bianjie.addColumn(0);
                            } else {
                                jiangjieheshifang.items.each(function (item) {
                                    if (item.wrwName == '污染物1') {
                                        item.hide();
                                    }
                                });
                                chushitiaojian.items.each(function (item) {
                                    if (item.chushiName == '污染物1') {
                                        item.hide();
                                    }
                                });
                                bianjie.removeColumn(0);
                            }
                        }
                    }
                }, {
                    xtype: 'textfield',
                    name: 'wrw1name',
                    submitValue: false,
                    disabled:true,
                    value: 'BOD5',
                    width: 80,
                    listeners: {
                        change: function (t, newValue) {
                            bianjie.changeName(0, newValue);
                            jiangjieheshifang.items.each(function (item) {
                                if (item.wrwName == '污染物1') {
                                    item.items.getAt(0).setValue(newValue);
                                }
                            });
                            chushitiaojian.items.each(function (item) {
                                if (item.chushiName == '污染物1') {
                                    item.setFieldLabel(newValue);
                                }
                            });
                        }
                    }
                }]
            }, {
                xtype: 'fieldcontainer',
                defaults: {
                    labelWidth: 60
                },
                layout: {
                    type: 'hbox',
                    hideLabel: false,
                    defaultMargins: {
                        top: 0,
                        right: 5,
                        bottom: 5,
                        left: 0
                    }
                },
                items: [{
                    xtype: 'checkbox',
                    name: 'wrw2',
                    submitValue: false,
                    disabled:true,
                    boxLabel: '污染物2',
                    width: 80,
                    listeners: {
                        scope: this,
                        change: function (box, newValue) {
                            if (newValue) {
                                jiangjieheshifang.items.each(function (item) {
                                    if (item.wrwName == '污染物2') {
                                        item.show();
                                    }
                                });
                                chushitiaojian.items.each(function (item) {
                                    if (item.chushiName == '污染物2') {
                                        item.show();
                                    }
                                });
                                bianjie.addColumn(1);
                            } else {
                                jiangjieheshifang.items.each(function (item) {
                                    if (item.wrwName == '污染物2') {
                                        item.hide();
                                    }
                                });
                                chushitiaojian.items.each(function (item) {
                                    if (item.chushiName == '污染物2') {
                                        item.hide();
                                    }
                                });
                                bianjie.removeColumn(1);
                            }
                        }
                    }
                }, {
                    xtype: 'textfield',
                    name: 'wrw2name',
                    submitValue: false,
                    value: 'NBOD',
                    width: 80,
                    disabled:true,
                    listeners: {
                        change: function (t, newValue) {
                            bianjie.changeName(1, newValue);
                            jiangjieheshifang.items.each(function (item) {
                                if (item.wrwName == '污染物2') {
                                    item.items.getAt(0).setValue(newValue);
                                }
                            });
                            chushitiaojian.items.each(function (item) {
                                if (item.chushiName == '污染物2') {
                                    item.setFieldLabel(newValue);
                                }
                            });
                        }
                    }
                }]
            }, {
                xtype: 'fieldcontainer',
                defaults: {
                    labelWidth: 60
                },
                layout: {
                    type: 'hbox',
                    hideLabel: false,
                    defaultMargins: {
                        top: 0,
                        right: 5,
                        bottom: 5,
                        left: 0
                    }
                },
                items: [{
                    xtype: 'checkbox',
                    name: 'wrw3',
                    submitValue: false,
                    boxLabel: '污染物3',
                    width: 80,
                    listeners: {
                        scope: this,
                        change: function (box, newValue) {
                            if (newValue) {
                                jiangjieheshifang.items.each(function (item) {
                                    if (item.wrwName == '污染物3') {
                                        item.show();
                                    }
                                });
                                chushitiaojian.items.each(function (item) {
                                    if (item.chushiName == '污染物3') {
                                        item.show();
                                    }
                                });
                                bianjie.addColumn(2);
                            } else {
                                jiangjieheshifang.items.each(function (item) {
                                    if (item.wrwName == '污染物3') {
                                        item.hide();
                                    }
                                });
                                chushitiaojian.items.each(function (item) {
                                    if (item.chushiName == '污染物3') {
                                        item.hide();
                                    }
                                });
                                bianjie.removeColumn(2);
                            }
                        }
                    }
                }, {
                    xtype: 'textfield',
                    name: 'wrw3name',
                    value: '氨氮(NH3-N)',
                    disabled:true,
                    submitValue: false,
                    width: 80,
                    listeners: {
                        change: function (t, newValue) {
                            bianjie.changeName(2, newValue);
                            jiangjieheshifang.items.each(function (item) {
                                if (item.wrwName == '污染物3') {
                                    item.items.getAt(0).setValue(newValue);
                                }
                            });
                            chushitiaojian.items.each(function (item) {
                                if (item.chushiName == '污染物3') {
                                    item.setFieldLabel(newValue);
                                }
                            });
                        }
                    }
                }]
            }, {
                xtype: 'fieldcontainer',
                defaults: {
                    labelWidth: 60
                },
                layout: {
                    type: 'hbox',
                    hideLabel: false,
                    defaultMargins: {
                        top: 0,
                        right: 5,
                        bottom: 5,
                        left: 0
                    }
                },
                items: [{
                    xtype: 'checkbox',
                    name: 'wrw4',
                    submitValue: false,
                    disabled:true,
                    boxLabel: '污染物4',
                    width: 80,
                    listeners: {
                        scope: this,
                        change: function (box, newValue) {
                            if (newValue) {
                                jiangjieheshifang.items.each(function (item) {
                                    if (item.wrwName == '污染物4') {
                                        item.show();
                                    }
                                });
                                chushitiaojian.items.each(function (item) {
                                    if (item.chushiName == '污染物4') {
                                        item.show();
                                    }
                                });
                                bianjie.addColumn(3);
                            } else {
                                jiangjieheshifang.items.each(function (item) {
                                    if (item.wrwName == '污染物4') {
                                        item.hide();
                                    }
                                });
                                chushitiaojian.items.each(function (item) {
                                    if (item.chushiName == '污染物4') {
                                        item.hide();
                                    }
                                });
                                bianjie.removeColumn(3);
                            }
                        }
                    }
                }, {
                    xtype: 'textfield',
                    submitValue: false,
                    name: 'wrw4name',
                    disabled:true,
                    value: '亚硝酸盐氮',
                    width: 80,
                    listeners: {
                        change: function (t, newValue) {
                            bianjie.changeName(3, newValue);
                            jiangjieheshifang.items.each(function (item) {
                                if (item.wrwName == '污染物4') {
                                    item.items.getAt(0).setValue(newValue);
                                }
                            });
                            chushitiaojian.items.each(function (item) {
                                if (item.chushiName == '污染物4') {
                                    item.setFieldLabel(newValue);
                                }
                            });
                        }
                    }
                }]
            }, {
                xtype: 'fieldcontainer',
                defaults: {
                    labelWidth: 60
                },
                layout: {
                    type: 'hbox',
                    hideLabel: false,
                    defaultMargins: {
                        top: 0,
                        right: 5,
                        bottom: 5,
                        left: 0
                    }
                },
                items: [{
                    xtype: 'checkbox',
                    name: 'wrw5',
                    disabled:true,
                    boxLabel: '污染物5',
                    submitValue: false,
                    width: 80,
                    listeners: {
                        scope: this,
                        change: function (box, newValue) {
                            if (newValue) {
                                jiangjieheshifang.items.each(function (item) {
                                    if (item.wrwName == '污染物5') {
                                        item.show();
                                    }
                                });
                                chushitiaojian.items.each(function (item) {
                                    if (item.chushiName == '污染物5') {
                                        item.show();
                                    }
                                });
                                bianjie.addColumn(4);
                            } else {
                                jiangjieheshifang.items.each(function (item) {
                                    if (item.wrwName == '污染物5') {
                                        item.hide();
                                    }
                                });
                                chushitiaojian.items.each(function (item) {
                                    if (item.chushiName == '污染物5') {
                                        item.hide();
                                    }
                                });
                                bianjie.removeColumn(4);
                            }
                        }
                    }
                }, {
                    xtype: 'textfield',
                    name: 'wrw5name',
                    submitValue: false,
                    disabled:true,
                    value: '硝酸盐氮',
                    width: 80,
                    listeners: {
                        change: function (t, newValue) {
                            bianjie.changeName(4, newValue);
                            jiangjieheshifang.items.each(function (item) {
                                if (item.wrwName == '污染物5') {
                                    item.items.getAt(0).setValue(newValue);
                                }
                            });
                            chushitiaojian.items.each(function (item) {
                                if (item.chushiName == '污染物5') {
                                    item.setFieldLabel(newValue);
                                }
                            });
                        }
                    }
                }]
            }, {
                xtype: 'fieldcontainer',
                defaults: {
                    labelWidth: 60
                },
                layout: {
                    type: 'hbox',
                    hideLabel: false,
                    defaultMargins: {
                        top: 0,
                        right: 5,
                        bottom: 5,
                        left: 0
                    }
                },
                items: [{
                    xtype: 'checkbox',
                    name: 'wrw6',
                    submitValue: false,
                    boxLabel: '污染物6',
                    disabled:true,
                    width: 80,
                    listeners: {
                        scope: this,
                        change: function (box, newValue) {
                            if (newValue) {
                                jiangjieheshifang.items.each(function (item) {
                                    if (item.wrwName == '污染物6') {
                                        item.show();
                                    }
                                });
                                chushitiaojian.items.each(function (item) {
                                    if (item.chushiName == '污染物6') {
                                        item.show();
                                    }
                                });
                                bianjie.addColumn(5);
                            } else {
                                jiangjieheshifang.items.each(function (item) {
                                    if (item.wrwName == '污染物6') {
                                        item.hide();
                                    }
                                });
                                chushitiaojian.items.each(function (item) {
                                    if (item.chushiName == '污染物6') {
                                        item.hide();
                                    }
                                });
                                bianjie.removeColumn(5);
                            }
                        }
                    }
                }, {
                    xtype: 'textfield',
                    name: 'wrw6name',
                    submitValue: false,
                    value: 'DO',
                    disabled:true,
                    width: 80,
                    listeners: {
                        change: function (t, newValue) {
                            bianjie.changeName(5, newValue);
                            jiangjieheshifang.items.each(function (item) {
                                if (item.wrwName == '污染物6') {
                                    item.items.getAt(0).setValue(newValue);
                                }
                            });
                            chushitiaojian.items.each(function (item) {
                                if (item.chushiName == '污染物6') {
                                    item.setFieldLabel(newValue);
                                }
                            });
                        }
                    }
                }]
            }, {
                xtype: 'fieldcontainer',
                defaults: {
                    labelWidth: 60
                },
                layout: {
                    type: 'hbox',
                    hideLabel: false,
                    defaultMargins: {
                        top: 0,
                        right: 5,
                        bottom: 5,
                        left: 0
                    }
                },
                items: [{
                    xtype: 'checkbox',
                    name: 'wrw7',
                    boxLabel: '污染物7',
                    submitValue: false,
                    width: 80,
                    listeners: {
                        scope: this,
                        change: function (box, newValue) {
                            if (newValue) {
                                jiangjieheshifang.items.each(function (item) {
                                    if (item.wrwName == '污染物7') {
                                        item.show();
                                    }
                                });
                                chushitiaojian.items.each(function (item) {
                                    if (item.chushiName == '污染物7') {
                                        item.show();
                                    }
                                });
                                bianjie.addColumn(6);
                            } else {
                                jiangjieheshifang.items.each(function (item) {
                                    if (item.wrwName == '污染物7') {
                                        item.hide();
                                    }
                                });
                                chushitiaojian.items.each(function (item) {
                                    if (item.chushiName == '污染物7') {
                                        item.hide();
                                    }
                                });
                                bianjie.removeColumn(6);
                            }
                        }
                    }
                }, {
                    xtype: 'textfield',
                    name: 'wrw7name',
                    disabled:true,
                    value: '化学需氧量(COD)',
                    submitValue: false,
                    width: 80,
                    listeners: {
                        change: function (t, newValue) {
                            bianjie.changeName(6, newValue);
                            jiangjieheshifang.items.each(function (item) {
                                if (item.wrwName == '污染物7') {
                                    item.items.getAt(0).setValue(newValue);
                                }
                            });
                            chushitiaojian.items.each(function (item) {
                                if (item.chushiName == '污染物7') {
                                    item.setFieldLabel(newValue);
                                }
                            });
                        }
                    }
                }]
            }]
        });
        this.items = [{
            autoScroll: true,
            title: '基本信息',
            items: [{
                xtype: 'fieldcontainer',
                fieldLabel: '河段数',
                labelWidth:80,
                defaults: {
                    readOnly: true,
                    xtype: 'textfield'
                },
                layout: {
                    type: 'hbox',
                    hideLabel: false,
                    defaultMargins: {
                        top: 0,
                        right: 5,
                        bottom: 5,
                        left: 0
                    }
                },
                items: [{
                    name: 'sz_nr',
                    allowBlank: false,
                    hideLabel: true,
                    submitValue: false,
                    value: 2,
                    width: 60
                }, {
                    name: 'sz_nj',
                    allowBlank: false,
                    submitValue: false,
                    fieldLabel: '河道节点数',
                    value: 3,
                    labelWidth:100,
                    width: 160
                }, {
                    name: 'sz_njb',
                    allowBlank: false,
                    submitValue: false,
                    fieldLabel: '边界节点个数',
                    value: 2,
                    labelWidth:100,
                    width: 160
                }, {
                    name: 'sz_nl',
                    allowBlank: false,
                    submitValue: false,
                    fieldLabel: '分区个数',
                    value: 2,
                    labelWidth:80,
                    width: 140
                }, {
                    name: 'sz_dt',
                    allowBlank: false,
                    submitValue: false,
                    fieldLabel: '计算时间步长(s)',
                    value: 86400,
                    labelWidth: 110,
                    width: 170
                }, {
                    name: 'sz_mt',
                    allowBlank: false,
                    submitValue: false,
                    fieldLabel: '月调节系数',
                    value: 1,
                    labelWidth: 80,
                    width: 140
                }]
            }, {
                xtype: 'checkboxgroup',
                columns: 12,
                anchor: '90%',
                fieldLabel: '计算月份',
                name: 'sz_month_group',
                submitValue: false,
                vertical: true,
                items: [
                    {boxLabel: '1月', name: 'sz_month1', inputValue: '1'},
                    {boxLabel: '2月', name: 'sz_month2', inputValue: '2'},
                    {boxLabel: '3月', name: 'sz_month3', inputValue: '3'},
                    {boxLabel: '4月', name: 'sz_month4', inputValue: '4'},
                    {boxLabel: '5月', name: 'sz_month5', inputValue: '5'},
                    {boxLabel: '6月', name: 'sz_month6', inputValue: '6'},
                    {boxLabel: '7月', name: 'sz_month7', inputValue: '7'},
                    {boxLabel: '8月', name: 'sz_month8', inputValue: '8'},
                    {boxLabel: '9月', name: 'sz_month9', inputValue: '9'},
                    {boxLabel: '10月', name: 'sz_month10', inputValue: '10'},
                    {boxLabel: '11月', name: 'sz_month11', inputValue: '11'},
                    {boxLabel: '12月', name: 'sz_month12', inputValue: '12', checked: true, readOnly: true}
                ]
            }, {
                xtype: 'fieldcontainer',
                labelWidth: 230,
                fieldLabel: '好氧状态的溶氧限制（单位:mg/L）',
                defaults: {
                    xtype: 'textfield'
                },
                layout: {
                    type: 'hbox',
                    hideLabel: false,
                    defaultMargins: {
                        top: 0,
                        right: 5,
                        bottom: 5,
                        left: 0
                    }
                },
                items: [{
                    name: 'sz_GDO',
                    allowBlank: false,
                    submitValue: false,
                    hideLabel: true,
                    value: 1.0,
                    width: 100
                }, {
                    labelWidth: 250,
                    name: 'sz_Bdo',
                    allowBlank: false,
                    submitValue: false,
                    hideLabel: false,
                    fieldLabel: '厌氧氧状态的溶氧限值（单位:mg/L）',
                    value: 0.2,
                    width: 350
                }]
            }]
        }, {
            title: '打印断面信息',
            items: [heduan]
        }, {
            title: '污染源位置设定信息',
            items: [wuranyuan]
        }, {
            title: '污染物信息',
            autoScroll: true,
            items: [wuranwu]
        }, jiangjieheshifang, chushitiaojian];
        this.tbar = [{
            xtype: 'button',
            text: '保存此页数据',
            scope: this,
            handler: this.onSave
        }];
        this.callParent(arguments);
    },
    onSave: function () {
        var me = this;
        if (this.getForm().isValid()) {
            var sz_nr = this.getForm().findField('sz_nr').getValue();
            var sz_nj = this.getForm().findField('sz_nj').getValue();
            var sz_njb = this.getForm().findField('sz_njb').getValue();
            var sz_nl = this.getForm().findField('sz_nl').getValue();
            var sz_dt = this.getForm().findField('sz_dt').getValue();
            var sz_mt = this.getForm().findField('sz_mt').getValue();

            var sz_month_group = this.getForm().findField('sz_month_group').getValue();

            var sz_GDO = this.getForm().findField('sz_GDO').getValue();
            var sz_Bdo = this.getForm().findField('sz_Bdo').getValue();

            var data = {};
            data.sz_nr = sz_nr;
            data.sz_nj = sz_nj;
            data.sz_njb = sz_njb;
            data.sz_nl = sz_nl;
            data.sz_dt = sz_dt;
            data.sz_mt = sz_mt;
            data.sz_GDO = sz_GDO;
            data.sz_Bdo = sz_Bdo;
            data.sz_model_sub_type = '1';
            var sz_month = '';
            for (var i = 0; i < sz_month_group.length; i++) {
                sz_month = sz_month + sz_month_group[i];
                if (i != sz_month_group.length - 1) {
                    sz_month = sz_month + ',';
                }
            }
            data.sz_month = sz_month;
            var sz_nspr = '';
            this.heduan.items.each(function (item) {
                if (item.isContext) {
                    var val = item.items.getAt(0).getValue() + ',' + item.items.getAt(1).getValue();
                    sz_nspr = sz_nspr + val + ';';
                }
            });
            data.sz_nspr = sz_nspr.substr(0, sz_nspr.length - 1);

            var sz_wry = '';
            this.wuranyuan.items.each(function (item) {
                if (item.isContext) {
                    var val = item.items.getAt(0).getValue() + ',' + item.items.getAt(1).getValue();
                    sz_wry = sz_wry + val + ';';
                }
            });
            data.sz_wry = sz_wry.substr(0, sz_wry.length - 1);
            var xishu = '';
            var chushi = '';
            var wuranwuNames = '';
            var wuranwuIndex = '';
            for (var i = 0; i < this.wuranwu.items.length; i++) {
                var item = this.wuranwu.items.getAt(i);
                var checked = item.items.getAt(0).getValue();
                var wuranwuName = item.items.getAt(1).getValue();
                wuranwuNames = wuranwuNames + wuranwuName;
                if (i != this.wuranwu.items.length - 1) {
                    wuranwuNames = wuranwuNames + ","
                }
                if (checked) {
                    wuranwuIndex = wuranwuIndex + (i + 1) + ',';
                } else {
                    wuranwuIndex = wuranwuIndex + 0 + ',';
                }
                if (checked) {
                    var val1 = "";
                    var val2 = "";
                    for (var j = 0; j < 4; j++) {
                        var v1 = this.jiangjieheshifang.items.getAt(i).items.getAt(1).items.getAt(0).items.getAt(j).getValue();
                        val1 = val1 + v1 + ',';
                        var v2 = this.jiangjieheshifang.items.getAt(i).items.getAt(1).items.getAt(1).items.getAt(j).getValue();
                        val2 = val2 + v2 + ',';
                    }
                    xishu = xishu + 'wrw' + (i + 1) + ',' + val1 + val2.substr(0, val2.length - 1) + ';';

                    chushi = chushi + 'wrw' + (i + 1) + ',' + this.chushitiaojian.items.getAt(i).items.getAt(0).getValue() + ',' + this.chushitiaojian.items.getAt(i).items.getAt(1).getValue() + ';';
                }
            }
            data.sz_wuranwuNames = wuranwuNames;
            data.sz_wuranwuIndex = wuranwuIndex.substr(0, wuranwuIndex.length - 1);
            data.sz_JHxS = xishu.substr(0, xishu.length - 1);
            data.sz_cbou = chushi.substr(0, chushi.length - 1);

            //固定参数
            data.sz_nc = '7';
            data.sz_njpm = '0';
            data.sz_nspm = '0';
            data.sz_mpr = '1';


            //JSON.stringify(data);
            var gridData = [];
            var grid = this.bianjie.items.getAt(1);
            var columns = this.bianjie.column;
            grid.getStore().each(function (record) {
                var val = {};
                for (var i = 1; i < columns.length; i++) {
                    if (!columns[i].hidden) {
                        val[columns[i].dataIndex] = record.get(columns[i].dataIndex);
                    }
                }
                gridData.push(val);
            });
            this.getForm().submit({
                url: __path + '/inputview/addChangguiData.do',
                method: 'post',
                params: {
                    plan_id: me.plan_id,
                    jsonStr: JSON.stringify(data),
                    gridData: JSON.stringify(gridData)
                },
                waitMsg: '正在保存，请稍候...',
                scope: this,
                success: function (form, action) {
                    if (action.result.success) {
                        Ext.Msg.alert('提示','保存成功');
                    } else {
                        Ext.Msg.alert('错误', action.result
                            ? action.result.message
                            : '保存时服务器出现错误，请与管理员联系。');
                    }
                    this.fireEvent('savesuccess', this, action.result);
                },
                failure: function (form, action) {
                    Ext.Msg.alert('错误', action.result
                        ? action.result.message
                        : '保存时服务器出现错误，请与管理员联系。');
                }
            });

        }
    },
    getWuRanWuConf: function (name1, name2, name3) {
        return {
            xtype:'fieldcontainer',
            hidden:true,
            wrwName:name1,
            anchor: '98%',
            layout: {
                type: 'hbox',
            },
            items:[ {
                xtype:'displayfield',
                flex:1,
                value:name3
            },{
                xtype:'fieldcontainer',
                flex:8,
                layout: {
                    type: 'vbox',
                },
                items:[{
                    xtype:'fieldcontainer',
                    flex:1,
                    defaults: {
                        xtype: 'textfield',
                        submitValue: false
                    },
                    layout: {
                        type: 'hbox',
                        defaultMargins: {
                            top: 0,
                            right: 5,
                            bottom: 5,
                            left: 0
                        }
                    },
                    items:[{
                        name:name2+'_1',
                        width:200,
                        labelWidth:150,
                        fieldLabel:'好氧DO浓度',
                        flex:1
                    },{
                        name:name2+'_2',
                        width:200,
                        labelWidth:150,
                        fieldLabel:'厌氧DO浓度',
                        flex:1
                    },{
                        name:name2+'_3',
                        width:200,
                        labelWidth:150,
                        fieldLabel:'河道一的好氧降解系数',
                        flex:1
                    },{
                        name:name2+'_4',
                        width:200,
                        labelWidth:150,
                        fieldLabel:'河道二的好氧降解系数',
                        flex:1
                    }]
                },{
                    xtype:'fieldcontainer',
                    flex:1,
                    defaults: {
                        xtype: 'textfield',
                        submitValue: false
                    },
                    layout: {
                        type: 'hbox',
                        defaultMargins: {
                            top: 0,
                            right: 5,
                            bottom: 5,
                            left: 0
                        }
                    },
                    items:[{
                        name:name2+'_5',
                        width:200,
                        labelWidth:150,
                        fieldLabel:'河道一的厌氧降解系数',
                        flex:1
                    },{
                        name:name2+'_6',
                        width:200,
                        labelWidth:150,
                        fieldLabel:'河道二的厌氧降解系数',
                        flex:1
                    },{
                        name:name2+'_7',
                        width:200,
                        labelWidth:150,
                        fieldLabel:'河道一的底泥释放系数',
                        flex:1
                    },{
                        name:name2+'_8',
                        width:200,
                        labelWidth:150,
                        fieldLabel:'河道二的底泥释放系数',
                        flex:1
                    }]
                }]
            }]
        };
    },
    getChushiConf: function (name1, name2, name3) {
        return {
            xtype: 'fieldcontainer',
            hidden: true,
            layout: {
                type: 'hbox',
                defaultMargins: {
                    top: 0,
                    right: 5,
                    bottom: 5,
                    left: 0
                }
            },
            chushiName: name1,
            fieldLabel: name3,
            anchor: '100%',
            labelWidth: 120,
            items: [{
                xtype: 'textfield',
                submitValue: false,
                emptyText: '进水浓度',
                name: name2 + '_in',
                width: 200
            }, {
                xtype: 'textfield',
                emptyText: '出水浓度',
                submitValue: false,
                name: name2 + '_out',
                width: 200
            }]
        };
    },
    setSzpjCgValue: function (data) {
        if (data) {
            var base = data.jibenxinxi;
            if(base){
                this.getForm().findField('sz_Bdo').setValue(base['sz_Bdo']);
                this.getForm().findField('sz_GDO').setValue(base['sz_GDO']);
                //this.getForm().findField('sz_dt').setValue(base['sz_dt']);
                //this.getForm().findField('sz_mt').setValue(base['sz_mt']);
                //this.getForm().findField('sz_nj').setValue(base['sz_nj']);
                //this.getForm().findField('sz_njb').setValue(base['sz_njb']);
                //this.getForm().findField('sz_nl').setValue(base['sz_nl']);
                //this.getForm().findField('sz_nr').setValue(base['sz_nr']);
                if (base['sz_month']) {
                    var sz_month = base['sz_month'].split(',');
                    this.getForm().findField('sz_month_group').setValue(sz_month);
                }
            }
            this.heduan.setHdValue(data.sz_nspr);
            this.wuranyuan.setHdValue(data.sz_wry);

            var names = [];
            if (data.sz_wuranwuNames) {
                names = data.sz_wuranwuNames.split(',');
            }
            var indexs = [];
            if (data.sz_wuranwuIndex) {
                indexs = data.sz_wuranwuIndex.split(',');
            }
            this.wuranwu.items.each(function (item, index) {
                if (indexs.indexOf((index + 1) + '') > -1) {
                    item.items.getAt(0).setValue(true);
                } else {
                    item.items.getAt(0).setValue(false);
                }
                if(names[index]){
                    item.items.getAt(1).setValue(names[index]);
                }
            });

            var wurwuJj = [];
            if (data.sz_JHxS) {
                wurwuJj = data.sz_JHxS.split(";");
            }
            this.jiangjieheshifang.items.each(function (item, index) {
                if (indexs.indexOf((index + 1) + '') > -1) {
                    if(wurwuJj){
                        var jj = wurwuJj.shift();
                        if(jj){
                            var data = jj.split(',');
                            for (var i = 0; i < 4; i++) {
                                item.items.getAt(1).items.getAt(0).items.getAt(i).setValue(data[i + 1]);
                                item.items.getAt(1).items.getAt(1).items.getAt(i).setValue(data[i + 5]);
                            }
                        }
                    }
                }
            });

            var wurwuCs = [];
            if (data.sz_cbou) {
                wurwuCs = data.sz_cbou.split(";");
            }
            this.chushitiaojian.items.each(function (item, index) {
                if (indexs.indexOf((index + 1) + '') > -1 && wurwuCs) {
                    var w = wurwuCs.shift();
                    if (w) {
                        var data = w.split(',');
                        item.items.getAt(0).setValue(data[1]);
                        item.items.getAt(1).setValue(data[2]);
                    }
                }
            });
            //var grid = this.bianjie.items.getAt(1);
            //var columns = this.bianjie.column;
            //grid.getStore().each(function (record) {
            //    var val = {};
            //    for (var i = 1; i < columns.length; i++) {
            //        if (!columns[i].hidden) {
            //            val[columns[i].dataIndex] = record.get(columns[i].dataIndex);
            //        }
            //    }
            //    gridData.push(val);
            //});
        }
    },

    afterFirstLayout:function () {
        this.callParent(arguments);
        this.add({
            title: '边界条件信息',
            items: [this.bianjie]
        });
    }
});
