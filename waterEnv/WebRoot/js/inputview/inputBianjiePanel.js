Ext.define('bitc.wrenv.inputview.inputBianjiePanel', {
    extend: 'Ext.form.Panel',
    border: false,
    plan_id: null,
    defaults: {
        anchor: '99%',
        labelWidth: 80
    },
    initComponent: function () {
        var gridstore = this.gridstore = Ext.create('Ext.data.Store', {
            fields: ['sz_heduan', 'sz_duanmian', 'sz_wrw_juli', 'sz_wrw_liuliang', 'sz_wrw1_c', 'sz_wrw1_p', 'sz_wrw2_c', 'sz_wrw2_p', 'sz_wrw3_c', 'sz_wrw3_p', 'sz_wrw4_c', 'sz_wrw4_p', 'sz_wrw5_c', 'sz_wrw5_p', 'sz_wrw6_c', 'sz_wrw6_p', 'sz_wrw7_c', 'sz_wrw7_p'],
            proxy: {
                type: 'ajax',
                url: __path + '/inputview/getSzpjGridData.do',
                extraParams: {plan_id: this.plan_id},
                reader: {
                    type: 'json',
                    root: 'rows'
                }
            },
            autoLoad:true,
            scope: this
        });
        var column = this.column = [
            {xtype: 'rownumberer', width: 40},
            {text: '河段', dataIndex: 'sz_heduan', width: 80},
            {text: '断面', dataIndex: 'sz_duanmian', width: 80},
            {
                text: '距离', dataIndex: 'sz_wrw_juli', width: 80
            }, {
                text: '流量(m³/s)', dataIndex: 'sz_wrw_liuliang', flex: 1, editor: {
                    xtype: 'textfield',
                    submitValue: false,
                    saveBtnText: '保存',
                    cancelBtnText: "取消",
                    allowBlank: false
                }
            }, {
                hidden: true,
                text: 'BOD5初始浓度(mg/l)', dataIndex: 'sz_wrw1_c', flex: 1, editor: {
                    xtype: 'textfield',
                    submitValue: false,
                    saveBtnText: '保存',
                    cancelBtnText: "取消",
                    allowBlank: false
                }
            }, {
                hidden: true,
                text: 'BOD5排放浓度(mg/l)', dataIndex: 'sz_wrw1_p', flex: 1, editor: {
                    xtype: 'textfield',
                    submitValue: false,
                    saveBtnText: '保存',
                    cancelBtnText: "取消",
                    allowBlank: false
                }
            }, {
                hidden: true,
                text: 'NBOD初始浓度(mg/l)', dataIndex: 'sz_wrw2_c', flex: 1, editor: {
                    xtype: 'textfield',
                    submitValue: false,
                    saveBtnText: '保存',
                    cancelBtnText: "取消",
                    allowBlank: false
                }
            }, {
                hidden: true,
                text: 'NBOD排放浓度(mg/l)', dataIndex: 'sz_wrw2_p', flex: 1, editor: {
                    xtype: 'textfield',
                    submitValue: false,
                    saveBtnText: '保存',
                    cancelBtnText: "取消",
                    allowBlank: false
                }
            }, {
                hidden: true,
                text: '氨氮初始浓度(mg/l)', dataIndex: 'sz_wrw3_c', flex: 1, editor: {
                    xtype: 'textfield',
                    submitValue: false,
                    saveBtnText: '保存',
                    cancelBtnText: "取消",
                    allowBlank: false
                }
            }, {
                hidden: true,
                text: '氨氮排放浓度(mg/l)', dataIndex: 'sz_wrw3_p', flex: 1, editor: {
                    xtype: 'textfield',
                    submitValue: false,
                    saveBtnText: '保存',
                    cancelBtnText: "取消",
                    allowBlank: false
                }
            }, {
                hidden: true,
                text: '亚硝酸盐氮初始浓度(mg/l)', dataIndex: 'sz_wrw4_c', flex: 1, editor: {
                    xtype: 'textfield',
                    submitValue: false,
                    saveBtnText: '保存',
                    cancelBtnText: "取消",
                    allowBlank: false
                }
            }, {
                hidden: true,
                text: '亚硝酸盐氮排放浓度(mg/l)', dataIndex: 'sz_wrw4_p', flex: 1, editor: {
                    xtype: 'textfield',
                    submitValue: false,
                    saveBtnText: '保存',
                    cancelBtnText: "取消",
                    allowBlank: false
                }
            }, {
                hidden: true,
                text: '硝酸盐氮初始浓度(mg/l)', dataIndex: 'sz_wrw5_c', flex: 1, editor: {
                    xtype: 'textfield',
                    submitValue: false,
                    saveBtnText: '保存',
                    cancelBtnText: "取消",
                    allowBlank: false
                }
            }, {
                hidden: true,
                text: '硝酸盐氮排放浓度(mg/l)', dataIndex: 'sz_wrw5_p', flex: 1, editor: {
                    xtype: 'textfield',
                    submitValue: false,
                    saveBtnText: '保存',
                    cancelBtnText: "取消",
                    allowBlank: false
                }
            }, {
                hidden: true,
                text: 'DO初始浓度(mg/l)', dataIndex: 'sz_wrw6_c', flex: 1, editor: {
                    xtype: 'textfield',
                    submitValue: false,
                    saveBtnText: '保存',
                    cancelBtnText: "取消",
                    allowBlank: false
                }
            }, {
                hidden: true,
                text: 'DO排放浓度(mg/l)', dataIndex: 'sz_wrw6_p', flex: 1, editor: {
                    xtype: 'textfield',
                    submitValue: false,
                    saveBtnText: '保存',
                    cancelBtnText: "取消",
                    allowBlank: false
                }
            }, {
                hidden: true,
                text: '化学需氧量初始浓度(mg/l)', dataIndex: 'sz_wrw7_c', flex: 1, editor: {
                    xtype: 'textfield',
                    submitValue: false,
                    saveBtnText: '保存',
                    cancelBtnText: "取消",
                    allowBlank: false
                }
            }, {
                hidden: true,
                text: '化学需氧量排放浓度(mg/l)', dataIndex: 'sz_wrw7_p', flex: 1, editor: {
                    xtype: 'textfield',
                    submitValue: false,
                    saveBtnText: '保存',
                    cancelBtnText: "取消",
                    allowBlank: false
                }
            }
        ];
        var grid = this.grid = Ext.create('Ext.grid.Panel', {
            store: gridstore,
            columns: column,
            anchor: '100%',
            height: 400,
            plugins: [
                Ext.create('Ext.grid.plugin.CellEditing', {
                    clicksToEdit: 1
                })
            ],
            autoScroll: true
        });
        this.items = [{
            xtype: 'filefield',
            anchor: '33%',
            name: 'bianjie_file',
            fieldLabel: '上传数据',
            buttonText: '文件选择'
        }, grid];
        this.callParent(arguments);
    },
    addColumn: function (index) {
        for (var i = 0; i < this.column.length; i++) {
            if ((i - 5) == index * 2) {
                this.column[i].hidden = false;
                this.column[i + 1].hidden = false;
            }
        }
        this.grid.reconfigure(this.gridstore, this.column);
    },
    removeColumn: function (index) {
        for (var i = 0; i < this.column.length; i++) {
            if ((i - 5) == index * 2) {
                this.column[i].hidden = true;
                this.column[i + 1].hidden = true;
            }
        }
        this.grid.reconfigure(null, this.column);
    },
    changeName: function (index, text) {
        for (var i = 0; i < this.column.length; i++) {
            if ((i - 5) == index * 2) {
                this.column[i].text = text + '初始浓度(mg/l)';
                this.column[i + 1].text = text + '排放浓度(mg/l)';
            }
        }
        this.grid.reconfigure(null, this.column);
    }
});
