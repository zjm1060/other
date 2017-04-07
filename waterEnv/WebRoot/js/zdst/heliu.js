Ext.define('bitc.wrenv.zdst.heliu', {
    extend: 'Ext.panel.Panel',
    requires: ['Ext.grid.feature.Grouping', 'Ext.ux.InklingSearchField','bitc.report.widget.ChooseField'],
    layout: 'border',
    stId:null,
    initComponent: function () {
        var gridview = this.gridview = Ext.create('bitc.report.ReportView', {
            reportToken: 'hlxxxx',
            region: 'center',
            reportCondition: this.stId ? "水体_ID='" + this.stId + "'" : null,
            columnConfig : {
                '河流长度' : {
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
            //rowContextMenu: [{
            //    text: '基本信息',
            //    panelRef: 'form',
            //    handler: this.onContextMenu,
            //    scope: this
            //}, {
            //    text: '地图信息',
            //    panelRef: 'superMap',
            //    handler: this.onContextMenu,
            //    scope: this
            //},{
            //    panelRef: 'zhiliuPanel',
            //    text: '支流信息',
            //    handler: this.onContextMenu,
            //    scope: this
            //}, {
            //    panelRef: 'shuikuPanel',
            //    text: '水库信息',
            //    handler: this.onContextMenu,
            //    scope: this
            //}, {
            //    panelRef: 'hpPanel',
            //    text: '湖泊信息',
            //    handler: this.onContextMenu,
            //    scope: this
            //}, {
            //    panelRef: 'swPanel',
            //    text: '水文测站信息',
            //    handler: this.onContextMenu,
            //    scope: this
            //}, {
            //    panelRef: 'shuiliPanel',
            //    text: '水利工程信息',
            //    handler: this.onContextMenu,
            //    scope: this
            //}, {
            //    panelRef: 'shuiyuanPanel',
            //    text: '水源相关',
            //    handler: this.onContextMenu,
            //    scope: this
            //},{
            //    panelRef: 'heliuRuhepaiwukou',
            //    text: '入河排污口',
            //    handler: this.onContextMenu,
            //    scope: this
            //}]
        });
        var syList = {
            '地表水':'bitc.wrenv.mgmb.dibiaoshui',
            '地下水':'bitc.wrenv.mgmb.dixiashui'
        };
        //form表单
        var form = this.form = Ext.create('bitc.report.FormView', {
            formToken: 'wr_rv_b',
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
                                    tableName: 'wr_rv_b',
                                    rowKey: 'RV_CD',
                                    rowKeyValue: form['河流代码']
                                }, {
                                    tableName: 'zdst_rel_干支流关系表',
                                    rowKey: '支流_ID',
                                    rowKeyValue: form['水体_ID']
                                }, {
                                    tableName: 'zdst_rel_水库河流关系表',
                                    rowKey: '河流_ID',
                                    rowKeyValue: form['水体_ID']
                                }, {
                                    tableName: 'zdst_rel_河流湖泊关系表',
                                    rowKey: '河流_ID',
                                    rowKeyValue: form['水体_ID']
                                }, {
                                    tableName: 'zdst_rel_水利工程项目与水体',
                                    rowKey: '水体_ID',
                                    rowKeyValue: form['水体_ID']
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

        var superMap = this.superMap = Ext.create('bitc.report.withSuperMap',{
            //closable : true
        });

        //支流信息标签
        var zhiliuPanel = this.zhiliuPanel = Ext.create('bitc.report.ReportView', {
            title: '支流信息',
            reportToken: 'zlxx',
            defaultGridWidth : false,
            //closable : true,
            reportCondition: "1=2",
            columnConfig : {
                '河流长度' : {
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return value;
                    }
                }
            }
        });
        //显示河流与水库的关系列表
        var shuikuPanel = this.shuikuPanel = Ext.create('bitc.report.ReportView', {
            title: '水库信息',
            reportToken: 'skxx',
            defaultGridWidth : false,
            //closable : true,
            reportCondition: "1=2",
            rowContextMenu: [{
                text: '详细',
                iconCls: '',
                handler: function (widget,record) {
                    var data={
                        id: 'shuiku',
                        text: '水库信息数据库',
                        action: 'bitc.wrenv.zdst.shuiku',
                        leaf: true,
                        iconCls: '',
                        config: {
                            stId: record.get('水体_ID')
                        }
                    };
                    addWorkTab(data);
                },
                scope: this
            }]
        });
        //显示河流与湖泊的关系列表
        var hpPanel = this.hpPanel = Ext.create('bitc.report.ReportView', {
            title: '湖泊信息',
            reportToken: 'hpxx',
            defaultGridWidth : false,
            //closable : true,
            reportCondition: "1=2",
            rowContextMenu: [{
                text: '详细',
                iconCls: '',
                handler: function (widget,record) {
                    var data={
                        id: 'hupo',
                        text: '湖泊信息数据库',
                        action: 'bitc.wrenv.zdst.hupo',
                        leaf: true,
                        iconCls: '',
                        config: {
                            stId: record.get('水体_ID')
                        }
                    };
                    addWorkTab(data);
                },
                scope: this
            }]

        });
        //显示河流与水文测站的关系列表
        var swPanel = this.swPanel = Ext.create('bitc.report.ReportView', {
            title: '水文测站信息',
            reportToken: 'swcz',
            //closable : true,
            reportCondition: "1=2",
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
                            //reportToken: 'hpxxxx',
                            swczId: record.get('水文测站_ID')
                        }
                    };
                    addWorkTab(data);
                },
                scope: this
            }]
        });
        //显示河流与水利工程的关系列表
        var shuiliPanel = this.shuiliPanel = Ext.create('bitc.report.ReportView', {
            title: '水利工程信息',
            reportToken: 'slgc',
            defaultGridWidth : false,
            //closable : true,
            reportCondition: "1=2"
        });
        //河流与水源地相关
        var shuiyuanPanel = this.shuiyuanPanel = Ext.create('bitc.report.ReportView',{
            title: '水源地相关',
            reportToken: 'heliushuiyuan',
            defaultGridWidth : false,
            //closable : true,
            gridConfig: {
                groupField: '敏感目标类型_ID',
                features: [{
                    ftype: 'grouping',
                    groupHeaderTpl: ['{rows:this.getName}', {
                        getName: function (rows) {
                            return rows[0].get('敏感目标类型名称');
                        }
                    }],
                    hideGroupedHeader: true,
                    startCollapsed: false
                }],
                selModel: {
                    selType: 'checkboxmodel',
                    mode: 'SIMPLE'
                }
            },
            rowContextMenu: [{
                text: '详细',
                iconCls: '',
                handler: function (widget,record, event) {
                    var type = record.get('敏感目标类型名称');
                    var data={
                        id: type+record.get('敏感目标_ID'),
                        text: type+'详细信息',
                        action: syList[type],
                        leaf: true,
                        iconCls: '',
                        config : {
                            sydId: record.get('敏感目标_ID')
                        }
                    };
                    addWorkTab(data);
                },
                scope: this
            }],
            listeners: {
                gridready: this.initShuiyuanPanel,
                scope: this
            }
        });
        var heliuRuhepaiwukou = this.heliuRuhepaiwukou = Ext.create('bitc.report.ReportView',{
            title: '入河排污口',
            reportToken: 'stRuhepaiwukou',
            //closable : true,
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
        this.tabpanel = Ext.create('Ext.panel.Panel', {
            autoScroll: true,
            title:'相关信息',
            collapsible: true,
            split: true,
            width: 600,
            frame: true,
            layout:'accordion',
            region: 'east',
            items: [form,hpPanel,shuikuPanel,zhiliuPanel,swPanel,shuiliPanel,shuiyuanPanel,heliuRuhepaiwukou,superMap]
        });
        this.items = [gridview, this.tabpanel];

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
                    text: '可通过河流名称或者河流代码查询'
                },
                inklings: [{
                    name: '河流名称',
                    op: 'like'
                }, {
                    name: '河流代码',
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
        grid.on('itemdblclick', function (g, record, item, index, event) {
            this.link(record);
            this.superMap.reLocation(record.get('水体_ID'),record.get('河流名称'),record.get(''),record.get('经纬度坐标'));
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
            name: 'RV_CD',
            value: record.get('河流代码'),
            rowKeyValue: record.get('河流代码')
        }, {
            name: 'RV_NM',
            value: record.get('河流名称'),
            rowKeyValue: record.get('河流代码')
        }, {
            name: 'RV_G',
            value: record.get('河流等级代码'),
            rowKeyValue: record.get('河流代码')
        }, {
            name: 'RV_LEN',
            value: record.get('河流长度'),
            rowKeyValue: record.get('河流代码')
        }, {
            name: '经纬度坐标',
            value: record.get('经纬度坐标'),
            rowKeyValue: record.get('水体_ID')
        }, {
            name: 'wr_sd_code',
            value: record.get('河流代码'),
            rowKeyValue: record.get('水体_ID')
        }, {
            name: 'RV_AG',
            value: record.get('河流平均比降'),
            rowKeyValue: record.get('河流代码')
        }, {
            name: 'IF_INN_RV',
            value: record.get('是否为内流河') == '是' ? 1 : 0,
            rowKeyValue: record.get('河流代码')
        }, {
            name: 'NT',
            value: record.get('备注'),
            rowKeyValue: record.get('河流代码')
        }, {
            name: 'TYPE_ID',
            value: 1,
            rowKeyValue: record.get('水体_ID')
        }, {
            name: '干流_ID',
            value: record.get('干流_ID'),
            rowKeyValue: record.get('水体_ID')
        }, {
            name: '支流_ID',
            value: record.get('水体_ID'),
            rowKeyValue: record.get('水体_ID')
        }, {
            name: 'TS',
            value: null,
            rowKeyValue: record.get('河流代码')
        }, {
            name: '水体_ID',
            value: record.get('水体_ID'),
            rowKeyValue: record.get('水体_ID')
        }]);
        var combo = this.form.getForm().findField("干流_ID");
        combo.getStore().filterBy(function (r) {
            return r.data.valueColumn != record.get('水体_ID');
        });
        this.form['河流代码'] = record.get('河流代码');
        this.form['水体_ID'] = record.get('水体_ID');
        if (combo.rawValue != null) {
            this.form.getForm().findField("支流_ID").setValue(record.get('水体_ID'));
        }

        this.zhiliuPanel.reload("干流_ID='" + record.get('水体_ID') + "'");
        this.shuikuPanel.reload("河流_ID='" + record.get('水体_ID') + "'");
        this.hpPanel.reload("河流_ID='" + record.get('水体_ID') + "'");
        this.swPanel.reload("水体_ID='" + record.get('水体_ID') + "'");
        this.shuiliPanel.reload("水体_ID='" + record.get('水体_ID') + "'");
        this.heliuRuhepaiwukou.reload("排入水体id ='"+record.get('水体_ID')+"'");

        var stId = this.stId = record.get('水体_ID');
        Ext.Ajax.request({
            url: __path + "/zdst/getSt.do",
            params: {
                dbName: '_self',
                stId: stId
            },
            method: 'post',
            success: function (response) {
                var result = Ext.decode(response.responseText);
                var records = result.rows;
                this.selectStRecords(records);
            },
            scope: this
        });

    },
    selectStRecords: function (records) {
        var grid = this.shuiyuanPanel.grid;
        grid.selectRecords(function (record) {
            return records.indexOf(record.get('敏感目标_ID')) >= 0;
        });
    },
    initShuiyuanPanel : function(panel, grid){
        grid.down('toolbar').insert(0, {
            text: '保存',
            scope: this,
            handler: function () {
                var records = grid.getSelectionModel().getSelection();
                if (records.length == 0) {
                    Ext.MessageBox.alert("提示", "请先选中您要操作的行!");
                    return;
                } else if (this.stId == null) {
                    Ext.MessageBox.alert("提示", "请先选择相关联的河流!");
                    return;
                } else {
                    var syIds = "";
                    for (var i = 0; i < records.length; i++) {
                        syIds += records[i].get("敏感目标_ID");
                        if (i < records.length - 1) {
                            syIds = syIds + ",";
                        }
                    }
                    Ext.Ajax.request({
                        url: __path + "/zdst/saveSt.do",
                        params: {
                            stId: this.stId,
                            syIds: syIds,
                            dbName: '_self'
                        },
                        method: 'post',
                        success: function (response) {
                            var result = Ext.decode(response.responseText);
                            if (result.success) {
                                Ext.Msg.alert("提示", "保存成功！")
                            } else {
                                Ext.Msg.alert("提示", "保存失败！")
                            }
                        },
                        scope: this
                    });

                }
            }
        });
    },
    onContextMenu: function (widget,record) {
        var panel = this.tabpanel.child('#' + this[widget.panelRef].id);
        if (!panel) {
            panel = this[widget.panelRef];
            this.tabpanel.add(panel);
        }
        this.tabpanel.setActiveTab(panel);
        this.link(record);
        if(widget.panelRef == 'superMap'){
            panel.reLocation(record.get('水体_ID'),record.get('河流名称'),record.get(''),record.get('经纬度坐标'));
        }
    }
});