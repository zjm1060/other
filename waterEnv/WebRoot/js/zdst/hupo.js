Ext.define('bitc.wrenv.zdst.hupo', {
    extend: 'Ext.panel.Panel',
    requires: ['Ext.grid.feature.Grouping', 'Ext.ux.InklingSearchField'],
    layout: 'border',
    stId: null,
    initComponent: function () {
        var gridview = this.gridview = Ext.create('bitc.report.ReportView', {
            reportToken: 'hpxxxx',
            reportCondition: this.stId ? "水体_ID='" + this.stId + "'" : null,
            region: 'center',
            columnConfig : {
                '水面面积' : {
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return value;
                    }
                }
            },
            listeners: {
                gridready: this.initGrid,
                scope: this
            }

        });
        var form = this.form = Ext.create('bitc.report.FormView', {
            formToken: 'hupoForm',
            title: '基本信息',
            listeners: {
                'savesuccess': function () {
                    this.grid.getStore().reload();
                    this.form.getForm().reset();
                    this.getForm().getFields().each(function (field) {
                        field.rowKeyValue = null;
                    });
                },
                'formready': function () {
                    var me = this;
                    form.down('toolbar').insert(0, [{
                        xtype: 'button',
                        text: '新建',
                        scope: form,
                        handler: function () {
                            this.getForm().reset();
                            this.getForm().getFields().each(function (field) {
                                field.rowKeyValue = null;
                            });
                        }
                    }, {
                        xtype: 'deletebutton',
                        scope: form,
                        beforeDelete: function () {
                            return true;
                        },
                        getDeleteRows: function () {
                            return {
                                dbName: '_self',
                                formFields: [{
                                    tableName: 'zdst_水体主体信息简表',
                                    rowKey: '水体_ID',
                                    rowKeyValue: form['水体_ID']
                                }, {
                                    tableName: 'wr_lk_b',
                                    rowKey: 'LK_CD',
                                    rowKeyValue: form['湖泊代码']
                                }]
                            };
                        },
                        deleteSuccess: function () {
                            form.getForm().reset();
                            form.getForm().getFields().each(function (field) {
                                field.rowKeyValue = null;
                            });
                            me.grid.getStore().reload();
                        }
                    }]);

                },
                scope: this
            }
        });

        var superMap = this.superMap = Ext.create('bitc.report.withSuperMap');

        var relatedRiverPanel = this.relatedRiverPanel = Ext.create('bitc.report.ReportView', {
            title: '相关河流',
            reportToken: 'hpRelatedRiver',
            reportCondition: "1=2",
            defaultGridWidth : false,
            columnConfig: {
                '河流代码2': {
                    renderer: function (value, meta, record) {
                        return record.get('河流名称');
                    }
                },
                '出入标记2': {
                    renderer: function (value) {
                        return {'0': '流入', '1': '流出'}[value];
                    }
                }
            },
            rowContextMenu: [{
                text: '详细',
                iconCls: '',
                handler: function (widget,record) {
                    var data={
                        id: 'heliu',
                        text: '河流信息数据库',
                        action: 'bitc.wrenv.zdst.heliu',
                        leaf: true,
                        iconCls: '',
                        config: {
                            stId: record.get('水体_ID')
                        }
                    };
                    addWorkTab(data);
                },
                scope: this
            }],
            listeners: {
                gridready: function (view, grid) {
                    this.initEditEvent(grid);
                },
                scope: this
            }
        });
        var shuiwenPanel = this.shuiwenPanel = Ext.create('bitc.report.ReportView', {
            title: '水文测站',
            reportToken: 'swcz',

            gridConfig: {
                groupField: '水文测站_ID',
                features: [{
                    ftype: 'grouping',
                    groupHeaderTpl: ['{rows:this.getName}', {
                        getName: function (rows) {
                            return rows[0].get('测站类别名称');
                        }
                    }],
                    hideGroupedHeader: true,
                    startCollapsed: false
                }]
            },
            reportCondition: "1=2",
            rowContextMenu: [{
                text: '详细',
                iconCls: '',
                handler: function (widget,record) {
                    var data={
                        id: 'swcz',
                        text: '水文测站信息数据库',
                        action: 'bitc.wrenv.zdst.shuiwencezhan',
                        leaf: true,
                        iconCls: '',
                        config: {
                            swczId: record.get('水文测站_ID')
                        }
                    };
                    addWorkTab(data);
                },
                scope: this
            }]
        });
        var shuiliPanel = this.shuiliPanel = Ext.create('bitc.report.ReportView', {
            title: '水利工程信息',
            reportToken: 'slgc',
            defaultGridWidth : false,
            reportCondition: "1=2"
        });
        var hupoRuhepaiwukou = this.hupoRuhepaiwukou = Ext.create('bitc.report.ReportView',{
            title: '入河排污口',
            reportToken: 'stRuhepaiwukou',
            reportCondition: "1=2",
            columnConfig : {
                '设计日排污能力' : {
                    renderer : function(value,meta,record){
                        meta.align = "right";
                        return value;
                    }
                }
            },
            rowContextMenu: [{
                text: '详细',
                iconCls: '',
                handler: function (widget,record) {
                    var data={
                        id: 'ruhepaiwukou',
                        text: '入河排污口信息数据库',
                        action: 'bitc.wrenv.mgmb.ruhepaiwukou',
                        leaf: true,
                        iconCls: '',
                        config: {
                            rhpwkId: record.get('排污口_ID')
                        }
                    };
                    addWorkTab(data);
                },
                scope: this
            }]
        });
        this.items = [gridview, {
             xtype: 'panel',             title:'相关信息',
            autoScroll: true,
            collapsible: true,
            split: true,
            layout:'accordion',
            //collapseMode: 'mini',
            width: 600,
            frame: true,
            region: 'east',
            items: [form , relatedRiverPanel, shuiwenPanel, shuiliPanel,hupoRuhepaiwukou, superMap]
        }];

        this.callParent(arguments);
    },
    initGrid: function (view, grid) {
        var me = this;
        this.grid = grid;
        grid.down('toolbar').insert(0, [Ext.create('Ext.ux.InklingSearchField', {
                width: 180,
                emptyText: '请输入关键字查询',
                reportView: this.gridview,
                tooltip: {
                    title: '条件查询',
                    text: '可通过湖泊名称或者湖泊代码查询'
                },
                inklings: [{
                    name: '湖泊名称',
                    op: 'like'
                }, {
                    name: '湖泊代码',
                    op: 'like'
                }]

            }),{
                text:'显示所有',
                hidden:me.stId == null?true:false,
                handler:function(){
                    me.gridview.reportCondition=null;
                    me.gridview.reload();
                    this.hide();
                }
            }]
        );
        grid.on('itemdblclick', function (g, record) {
            this.link(record);
            this.superMap.reLocation(record.get('水体_ID'),record.get('湖泊名称'),record.get(''),record.get('经纬度坐标'));
        }, this);
        if (this.stId) {
            this.gridview.grid.getStore().on('load', function (store, records) {
                var record = records[0];
                this.gridview.grid.getSelectionModel().select(0);
                this.link(record);
            }, this);
        }
    },
    link: function (record) {
        this.form.setValues([{
            name: '水体_ID',
            value: record.get('水体_ID'),
            rowKeyValue: record.get('水体_ID')
        }, {
            name: 'LK_CD',
            value: record.get('湖泊代码'),
            rowKeyValue: record.get('湖泊代码')
        }, {
            name: 'LK_NM',
            value: record.get('湖泊名称'),
            rowKeyValue: record.get('湖泊代码')
        }, {
            name: 'WAT_A',
            value: record.get('水面面积'),
            rowKeyValue: record.get('湖泊代码')
        }, {
            name: 'SFW_PROP',
            value: record.get('咸淡水属性ID'),
            rowKeyValue: record.get('湖泊代码')
        }, {
            name: 'AVG_DEP',
            value: record.get('平均水深'),
            rowKeyValue: record.get('湖泊代码')
        }, {
            name: 'MAX_DEP',
            value: record.get('最大水深'),
            rowKeyValue: record.get('湖泊代码')
        }, {
            name: 'LK_V',
            value: record.get('湖泊容积'),
            rowKeyValue: record.get('湖泊代码')
        }, {
            name: '经纬度坐标',
            value: record.get('经纬度坐标'),
            rowKeyValue: record.get('水体_ID')
        }, {
            name: 'NT',
            value: record.get('备注'),
            rowKeyValue: record.get('湖泊代码')
        }, {
            name: 'TYPE_ID',
            value: 3,
            rowKeyValue: record.get('水体_ID')
        }, {
            name: 'TS',
            value: null,
            rowKeyValue: record.get('湖泊代码')
        }, {
            name: 'wr_sd_code',
            value: record.get('湖泊代码'),
            rowKeyValue: record.get('水体_ID')
        }]);
        this.form['湖泊代码'] = record.get('湖泊代码');
        this.form['水体_ID'] = record.get('水体_ID');
        this.relatedRiverPanel['水体_ID'] = record.get('水体_ID');
        this.shuiwenPanel.reload("水体_ID='" + record.get('水体_ID') + "'");
        this.shuiliPanel.reload("水体_ID='" + record.get('水体_ID') + "'");
        this.relatedRiverPanel.reload("湖泊_ID='" + record.get('水体_ID') + "'");
        this.hupoRuhepaiwukou.reload("排入水体id ='"+record.get('水体_ID')+"'");
    },
    initEditEvent: function (grid) {
        var me = this;
        var rowEditing = grid.rowEditing;
        grid.down('toolbar').insert(0, [{
            xtype: 'button',
            text: '增加河流',
            handler: function () {
                if (me.relatedRiverPanel['水体_ID'] == null) {
                    Ext.Msg.alert('提示', '请在左侧选中一条数据');
                    return;
                }
                var relatedRiverPanelStore = grid.getStore();
                var relatedRiverPanelModel = relatedRiverPanelStore.model;
                relatedRiverPanelStore.insert(0, relatedRiverPanelModel);
                rowEditing.startEdit(0, 0);
            },
            scope: grid
        }, {
            xtype: 'button',
            text: '删除河流',
            handler: function () {
                if (me.relatedRiverPanel['水体_ID'] == null) {
                    Ext.Msg.alert('提示', '请在左侧选中一条数据');
                    return;
                }
                var data = grid.getSelectionModel().getLastSelected();
                if (data.length == 0) {
                    Ext.Msg.alert('提示', '请选中一条数据再删除');
                } else {
                    Ext.Msg.confirm('提示', '确认要删除吗？', function (btn) {
                        if (btn == 'yes') {
                            Ext.Ajax.request({
                                url: __path + '/zdst/deleteHPRelatedRiver.do',
                                params: {
                                    reportToken: 'hpRelatedRiver',
                                    hupoID: data.data.湖泊_ID,
                                    riverCode: data.data.河流代码2
                                },
                                method: 'POST',
                                success: function () {
                                    grid.getStore().reload();
                                }
                            });
                        }
                    });
                }
            },
            scope: grid
        }]);
        rowEditing.on('beforeedit', function (editor, context) {
            var combo=editor.editor.getForm().findField('河流代码2');
            combo.getStore().clearFilter(true);
            combo.getStore().filter([{
                filterFn: function (item) {
                    if(item.get('valueColumn')==context.record.get('河流代码2')){
                        return true;
                    }
                    var r=grid.getStore().findRecord('河流代码2',item.get('valueColumn'));
                    return r==null;
                }
            }]);
        },this);
        rowEditing.on('edit', function (editor, e) {
            var record = e.record;
            //如果该记录的湖泊_ID为空时，即要插入一条相关河流信息
            if (record.data.湖泊_ID == '') {
                Ext.Ajax.request({
                    url: __path + '/zdst/insertHPRelatedRiver.do',
                    params: {
                        reportToken: 'hpRelatedRiver',
                        hupoID: me.relatedRiverPanel['水体_ID'],
                        riverCode: record.data.河流代码2,
                        riverDirect: record.data.出入标记2
                    },
                    method: 'POST',
                    success: function () {
                        grid.getStore().reload();
                    }
                });
            } else {
                //如果该记录的湖泊_ID不为空，即要修改相关河流信息
                Ext.Ajax.request({
                    url: __path + '/zdst/updateHPRelatedRiver.do',
                    params: {
                        reportToken: 'hpRelatedRiver',
                        hupoID: record.data.湖泊_ID,
                        riverCode: record.data.河流代码2,
                        riverDirect: record.data.出入标记2,
                        heliuID : record.data.水体_ID
                    },
                    method: 'POST',
                    success: function () {
                        grid.getStore().reload();
                    }
                });
            }

        });
    }
});