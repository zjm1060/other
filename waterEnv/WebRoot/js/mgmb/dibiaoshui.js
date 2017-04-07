Ext.define('bitc.wrenv.mgmb.dibiaoshui',{
    extend: 'Ext.panel.Panel',
    requires: ['Ext.grid.feature.Grouping','Ext.ux.InklingSearchField','bitc.report.widget.ChooseField'],
    layout: 'border',
    sydId : null,
    initComponent : function (){
        var gridview = this.gridview = Ext.create('bitc.report.ReportView', {
            reportToken: 'dbsxxxx',
            region: 'center',
            reportCondition: this.sydId ? "水源地_ID='" + this.sydId + "'" : null,
            listeners: {
                gridready: this.initGrid,
                scope: this
            },
            columnConfig : {
                '水面面积' : {
                    renderer : function(value,meta,record){
                        meta.align = "right";
                        return value;
                    }
                }
            }
        });
        var stList = {
            '河流':'bitc.wrenv.zdst.heliu',
            '湖泊':'bitc.wrenv.zdst.hupo',
            '水库':'bitc.wrenv.zdst.shuiku'
        };
        var form = this.form = Ext.create('bitc.report.FormView',{
            formToken: 'dbsForm',
            title: '基本信息',
            fieldConfig: {
                WHS_MAN_NM: {
                    setValueFun: function (record) {
                        this.form.getForm().findField('WHS_MAN_CD').setValue(record.get('ORG_CD'));
                        this.form.getForm().findField('WHS_MAN_NM').setValue(record.get('ORG_NM'));
                    },
                    clearValueFun: function () {
                        this.form.getForm().findField('WHS_MAN_CD').setValue(null);
                        this.form.getForm().findField('WHS_MAN_NM').setValue(null);
                    },
                    winConfig: {
                        "title": "单位管理"
                    },
                    gridConfig: {
                        selModel: {
                            selType: 'checkboxmodel',
                            mode: 'SINGLE'
                        }
                    },
                    submitValue: false,
                    searchBar: true,
                    searchFieldConfig: {
                        xtype: "searchfield",
                        width: 180,
                        emptyText: "请输入关键字查询",
                        tooltip: {
                            title: "条件查询",
                            text: "可通过机构名称或者机构简介查询"
                        },
                        inklings: [
                            {
                                name: "ORG_NM",
                                op: "like"
                            },
                            {
                                name: "ORG_SHNM",
                                op: "like"
                            }
                        ]
                    },
                    scope: this
                },
                WHS_APPR_NM: {
                    setValueFun: function (record) {
                        this.form.getForm().findField('WHS_APPR_CD').setValue(record.get('ORG_CD'));
                        this.form.getForm().findField('WHS_APPR_NM').setValue(record.get('ORG_NM'));
                    },
                    clearValueFun: function () {
                        this.form.getForm().findField('WHS_APPR_CD').setValue(null);
                        this.form.getForm().findField('WHS_APPR_NM').setValue(null);
                    },
                    winConfig: {
                        "title": "单位管理"
                    },
                    gridConfig: {
                        selModel: {
                            selType: 'checkboxmodel',
                            mode: 'SINGLE'
                        }
                    },
                    submitValue: false,
                    searchBar: true,
                    searchFieldConfig: {
                        xtype: "searchfield",
                        width: 180,
                        emptyText: "请输入关键字查询",
                        tooltip: {
                            title: "条件查询",
                            text: "可通过机构名称或者机构简介查询"
                        },
                        inklings: [
                            {
                                name: "ORG_NM",
                                op: "like"
                            },
                            {
                                name: "ORG_SHNM",
                                op: "like"
                            }
                        ]
                    },
                    scope: this
                },
                EM_NM: {
                    setValueFun: function (record) {
                        this.form.getForm().findField('EM_CD').setValue(record.get('ORG_CD'));
                        this.form.getForm().findField('EM_NM').setValue(record.get('ORG_NM'));
                    },
                    clearValueFun: function () {
                        this.form.getForm().findField('EM_CD').setValue(null);
                        this.form.getForm().findField('EM_NM').setValue(null);
                    },
                    winConfig: {
                        "title": "单位管理"
                    },
                    gridConfig: {
                        selModel: {
                            selType: 'checkboxmodel',
                            mode: 'SINGLE'
                        }
                    },
                    submitValue: false,
                    searchBar: true,
                    searchFieldConfig: {
                        xtype: "searchfield",
                        width: 180,
                        emptyText: "请输入关键字查询",
                        tooltip: {
                            title: "条件查询",
                            text: "可通过机构名称或者机构简介查询"
                        },
                        inklings: [
                            {
                                name: "ORG_NM",
                                op: "like"
                            },
                            {
                                name: "ORG_SHNM",
                                op: "like"
                            }
                        ]
                    },
                    scope: this
                }
            },
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
                                    tableName: 'mgmb_水源地信息简表',
                                    rowKey: '水源地_ID',
                                    rowKeyValue: me.sydId
                                }, {
                                    tableName: 'wr_sws_b',
                                    rowKey: 'SWS_CD',
                                    rowKeyValue: me.sydDm
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
        var syGrid = this.syGrid = Ext.create('bitc.report.ReportView',{
            reportToken: 'fuwushuitiGrid',
            title: '水源来源',
            defaultGridWidth : false,
            gridConfig: {
                groupField: '水体类型_ID',
                features: [{
                    ftype: 'grouping',
                    groupHeaderTpl: ['{rows:this.getName}', {
                        getName: function (rows) {
                            return rows[0].get('水体类型名称');
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
                    var type = record.get('水体类型名称');
                    var data={
                        id: type+record.get('水体_ID'),
                        text: type+'详细信息',
                        action: stList[type],
                        leaf: true,
                        iconCls: '',
                        config : {
                            stId: record.get('水体_ID')
                        }
                    };
                    debugger;
                    addWorkTab(data);
                },
                scope: this
            }],
            listeners: {
                gridready: this.initSyGrid,
                scope: this
            }
        });
        var shuiwenDBPanel = this.shuiwenDBPanel = Ext.create('bitc.report.ReportView', {
            title: '水文测站',
            reportToken: 'sydcz',
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
                            swczId: record.get('水文测站_ID')
                        }
                    };
                    addWorkTab(data);
                },
                scope: this
            }]
        });
        this.items = [gridview,{
             xtype: 'panel',             title:'相关信息',
            autoScroll: true,
            collapsible: true,
            split: true,
            layout:'accordion',
            //collapseMode: 'mini',
            width: 600,
            frame: true,
            region: 'east',
            items: [form,superMap,syGrid,shuiwenDBPanel]
        }];
        this.callParent(arguments);
    },
    initGrid : function(view,grid){
        var me = this;
        this.grid = grid;
        grid.down('toolbar').insert(0, [Ext.create('Ext.ux.InklingSearchField', {
                width: 180,
                emptyText: '请输入关键字查询',
                reportView: this.gridview,
                tooltip: {
                    title: '条件查询',
                    text: '可通过地表水水源地名称或者地表水水源地代码查询'
                },
                inklings: [{
                    name: '地表水水源地代码',
                    op: 'like'
                }, {
                    name: '地表水水源地名称',
                    op: 'like'
                }]

            }),{
                text:'显示所有',
                hidden:me.sydId == null?true:false,
                handler:function(){
                    me.gridview.reportCondition=null;
                    me.gridview.reload();
                    this.hide();
                }
            }]
        );
        grid.on('itemdblclick', function (g, record) {
            this.link(record);
            this.superMap.reLocation(record.get('水源地_ID'),record.get('地表水水源地名称'),record.get(''),record.get('经纬度坐标'));
        }, this);
        if (this.sydId) {
            this.gridview.grid.getStore().on('load', function (store, records) {
                var record = records[0];
                this.gridview.grid.getSelectionModel().select(0);
                this.link(record);
            }, this);
        }
    },
    initSyGrid: function (panel, grid) {
        grid.down('toolbar').insert(0, {
            text: '保存',
            scope: this,
            handler: function () {
                var records = grid.getSelectionModel().getSelection();
                if (records.length == 0) {
                    Ext.MessageBox.alert("提示", "请先选中您要操作的行!");
                    return;
                } else if (this.sydId == null) {
                    Ext.MessageBox.alert("提示", "请先选择相关联的地表水!");
                    return;
                } else {
                    var stIds = "";
                    for (var i = 0; i < records.length; i++) {
                        stIds += records[i].get("水体_ID");
                        if (i < records.length - 1) {
                            stIds = stIds + ",";
                        }
                    }
                    Ext.Ajax.request({
                        url: __path + "/mgmb/saveSy.do",
                        params: {
                            sydId: this.sydId,
                            stIds: stIds,
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
    link: function (record) {
        this.form.setValues([{
            name: '水源地_ID',
            value:record.get('水源地_ID'),
            rowKeyValue: record.get('水源地_ID')
        },{
            name: 'SWS_CD',
            value:record.get('地表水水源地代码'),
            rowKeyValue: record.get('地表水水源地代码')
        }, {
            name: 'SWS_NM',
            value: record.get('地表水水源地名称'),
            rowKeyValue: record.get('地表水水源地代码')
        }, {
            name: 'SWS_TP',
            value: record.get('地表水水源地类型ID'),
            rowKeyValue: record.get('地表水水源地代码')
        }, {
            name: 'WAT_A',
            value: record.get('水面面积'),
            rowKeyValue: record.get('地表水水源地代码')
        }, {
            name: 'WQ_GOAL',
            value: record.get('水质目标ID'),
            rowKeyValue: record.get('地表水水源地代码')
        }, {
            name: 'CONS_COND',
            value: record.get('建设状况ID'),
            rowKeyValue: record.get('地表水水源地代码')
        }, {
            name: 'PUT_PROD_TM',
            value: Ext.util.Format.date(record.get('投产时间'), 'Y-m-d'),
            rowKeyValue: record.get('地表水水源地代码')
        }, {
            name: 'RUN_COND',
            value: record.get('运行状况'),
            rowKeyValue: record.get('地表水水源地代码')
        },{
            name: 'WS_COND',
            value: record.get('水源供水持续状况ID'),
            rowKeyValue: record.get('地表水水源地代码')
        },{
            name: 'WS_OBJ',
            value: record.get('供水对象ID'),
            rowKeyValue: record.get('地表水水源地代码')
        },{
            name: 'DYWS_PP',
            value: record.get('设计年供水人口'),
            rowKeyValue: record.get('地表水水源地代码')
        },{
            name: 'DYWS_W',
            value: record.get('设计年供水量'),
            rowKeyValue: record.get('地表水水源地代码')
        },{
            name: 'DDWS_W',
            value: record.get('设计日供水量'),
            rowKeyValue: record.get('地表水水源地代码')
        },{
            name: '经纬度坐标',
            value: record.get('经纬度坐标'),
            rowKeyValue: record.get('水源地_ID')
        }, {
            name: 'NT',
            value: record.get('备注'),
            rowKeyValue: record.get('地表水水源地代码')
        }, {
            name: 'TYPE_ID',
            value: 1,
            rowKeyValue: record.get('水源地_ID')
        }, {
            name: 'TS',
            value: null,
            rowKeyValue: record.get('地表水水源地代码')
        }, {
            name: 'wr_sd_code',
            value: record.get('地表水水源地代码'),
            rowKeyValue: record.get('水源地_ID')
        },{
            name: 'WHS_MAN_CD',
            value: record.get('水源地管理单位代码'),
            rowKeyValue: record.get('地表水水源地代码')
        },{
            name: 'WHS_APPR_CD',
            value: record.get('水源地审批单位代码'),
            rowKeyValue: record.get('地表水水源地代码')
        },{
            name: 'EM_CD',
            value: record.get('应急管理单位代码'),
            rowKeyValue: record.get('地表水水源地代码')
        },{
            name: 'WHS_MAN_NM',
            value: record.get('水源地管理单位名称'),
            rowKeyValue: record.get('地表水水源地代码')
        },{
            name: 'WHS_APPR_NM',
            value: record.get('水源地审批单位名称'),
            rowKeyValue: record.get('地表水水源地代码')
        },{
            name: 'EM_NM',
            value: record.get('应急管理单位名称'),
            rowKeyValue: record.get('地表水水源地代码')
        }]);
        this.shuiwenDBPanel.reload("水源地_ID='" + record.get('水源地_ID') + "'");
        var sydId = this.sydId = record.get('水源地_ID');
        var sydDm = this.sydDm = record.get('地表水水源地代码');
        Ext.Ajax.request({
            url: __path + "/mgmb/getSy.do",
            params: {
                dbName: '_self',
                sydId: sydId
            },
            method: 'post',
            success: function (response) {
                var result = Ext.decode(response.responseText);
                var records = result.rows;
                this.selectSyRecords(records);
            },
            scope: this
        });
    },
    selectSyRecords: function (records) {
        var grid = this.syGrid.grid;
        grid.selectRecords(function (record) {
            return records.indexOf(record.get('水体_ID')) >= 0;
        });
    }
});