Ext.define('bitc.wrenv.mgmb.ruhepaiwukou', {
    extend: 'Ext.panel.Panel',
    requires: ['Ext.grid.feature.Grouping','Ext.ux.InklingSearchField','bitc.report.widget.ChooseField'],
    layout: 'border',
    rhpwkId : null,
    initComponent: function () {
        var gridview = this.gridview = Ext.create('bitc.report.ReportView', {
            reportToken: 'ruhepaiwukouReport',
            region: 'center',
            reportCondition: this.rhpwkId ? "排污口_ID='" + this.rhpwkId + "'" : null,
            columnConfig : {
                '设计日排污能力' : {
                    renderer : function(value,meta,record){
                        meta.align = "right";
                        return value;
                    }
                }
            },
            listeners: {
                gridready: this.initGrid,
                scope: this
            }
        });
        var form = this.form = Ext.create('bitc.report.FormView',{
            formToken: 'ruhepaiwukouForm',
            title: '基本信息',
            fieldConfig: {
                WFZ_NM: {
                    setValueFun: function (record) {
                        this.form.getForm().findField('WFZ_CD').setValue(record.get('水功能区代码'));
                        this.form.getForm().findField('WFZ_NM').setValue(record.get('水功能区名称'));
                    },
                    clearValueFun: function () {
                        this.form.getForm().findField('WFZ_CD').setValue(null);
                        this.form.getForm().findField('WFZ_NM').setValue(null);
                    },
                    winConfig: {
                        "title": "水功能区"
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
                            text: "可通过水功能区名称查询"
                        },
                        inklings: [
                            {
                                name: "水功能区名称",
                                op: "like"
                            }
                        ]
                    },
                    scope: this
                },
                ENG_MAN_NM: {
                    setValueFun: function (record) {
                        this.form.getForm().findField('ENG_MAN_CD').setValue(record.get('ORG_CD'));
                        this.form.getForm().findField('ENG_MAN_NM').setValue(record.get('ORG_NM'));
                    },
                    clearValueFun: function () {
                        this.form.getForm().findField('ENG_MAN_CD').setValue(null);
                        this.form.getForm().findField('ENG_MAN_NM').setValue(null);
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
                DWB_NM1: {
                    setValueFun: function (record) {
                        this.form.getForm().findField('DWB_NM').setValue(record.get('水体_ID'));
                        this.form.getForm().findField('DWB_NM1').setValue(record.get('水体名称'));
                    },
                    clearValueFun: function () {
                        this.form.getForm().findField('DWB_NM').setValue(null);
                        this.form.getForm().findField('DWB_NM1').setValue(null);
                    },
                    winConfig: {
                        "title": "排入水体名称"
                    },
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
                            text: "可通过水体代码或水体名称查询"
                        },
                        inklings: [
                            {
                                name: "水体代码",
                                op: "like"
                            },
                            {
                                name: "水体名称",
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
                    this.form.getForm().getFields().each(function (field) {
                        field.rowKeyValue = null;
                    });
                },
                'formready': function () {
                    debugger;
                    var me = this;
                    me.form.down('toolbar').insert(0, [{
                        xtype: 'button',
                        text: '新建',
                        handler: function () {
                            me.form.getForm().reset();
                            me.form.getForm().getFields().each(function (field) {
                                field.rowKeyValue = null;
                            });
                        }
                    }, {
                        xtype: 'deletebutton',
                        beforeDelete: function () {
                            return true;
                        },
                        getDeleteRows: function () {
                            return {
                                dbName: '_self',
                                formFields: [{
                                    tableName: 'mgmb_入河排污口信息简表',
                                    rowKey: '入河排污口_ID',
                                    rowKeyValue:me.rhpwkId
                                }, {
                                    tableName: 'wr_pdo_b',
                                    rowKey: 'PDO_CD',
                                    rowKeyValue: me.rhpwkDm
                                }]
                            };
                        },
                        deleteSuccess: function () {
                            me.form.getForm().reset();
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
        var paiwukouQuyongshuihu = this.paiwukouQuyongshuihu = Ext.create('bitc.report.ReportView',{
            reportToken: 'pwkqyshGrid',
            title: '取用水户',
            gridConfig: {
                selModel: {
                    selType: 'checkboxmodel',
                    mode: 'SIMPLE'
                }
            },
            listeners: {
                gridready: this.initPwkqyshGrid,
                scope: this
            },
            rowContextMenu: [{
                text: '详细',
                iconCls: '',
                handler: function (widget,record) {
                    var data={
                        id: 'quyongshuihu',
                        text: '取用水户信息数据库',
                        action: 'bitc.wrenv.mgmb.quyongshuihu',
                        leaf: true,
                        iconCls: '',
                        config: {
                            qyshId: record.get('取用水户_ID')
                        }
                    };
                    addWorkTab(data);
                },
                scope: this
            }]
        });
        var pwkSwczGrid = this.pwkSwczGrid = Ext.create('bitc.report.ReportView',{
            reportToken: 'pwkSwczGrid',
            title: '水文测站',
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

        var pwkQsczGrid = this.pwkQsczGrid = Ext.create('bitc.report.ReportView',{
            reportToken: 'pwkQsczGrid',
            title: '取水测站',
            defaultGridWidth : false,
            reportCondition: "1=2",
            gridConfig: {
                groupField: '取用水测站类别id',
                features: [{
                    ftype: 'grouping',
                    groupHeaderTpl: ['{rows:this.getName}', {
                        getName: function (rows) {
                            return rows[0].get('取用水测站类别名称');
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
                        id: 'quyongshuicezhan',
                        text: '取用水测站数据库',
                        action: 'bitc.wrenv.mgmb.quyongshuicezhan',
                        leaf: true,
                        iconCls: '',
                        config: {
                            qysczId: record.get('取用水测站点_ID')
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
            layout:'accordion',
            split: true,
            collapseMode: 'mini',
            width: 600,
            frame: true,
            region: 'east',
            items: [form,superMap,paiwukouQuyongshuihu,pwkSwczGrid,pwkQsczGrid]
        }];
        this.callParent(arguments);
    },
    initPwkqyshGrid : function(panel,grid){
        grid.down('toolbar').insert(0, {
            text: '保存',
            scope: this,
            handler: function () {
                var records = grid.getSelectionModel().getSelection();
                if (records.length == 0) {
                    Ext.MessageBox.alert("提示", "请先选中您要操作的行!");
                    return;
                } else if (this.rhpwkId == null) {
                    Ext.MessageBox.alert("提示", "请先选择相关联的排污口!");
                    return;
                } else {
                    var qyshIds = "";
                    for (var i = 0; i < records.length; i++) {
                        qyshIds += records[i].get("取用水户_ID");
                        if (i < records.length - 1) {
                            qyshIds = qyshIds + ",";
                        }
                    }
                    Ext.Ajax.request({
                        url: __path + "/mgmb/savePwkqysh.do",
                        params: {
                            rhpwkId: this.rhpwkId,
                            qyshIds: qyshIds,
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
    initGrid : function(view,grid){
        this.grid = grid;
        var me = this;
        grid.down('toolbar').insert(0, [Ext.create('Ext.ux.InklingSearchField', {
                width: 180,
                emptyText: '请输入关键字查询',
                reportView: this.gridview,
                tooltip: {
                    title: '条件查询',
                    text: '可通过入河排污口代码或者入河排污口名称查询'
                },
                inklings: [{
                    name: '入河排污口代码',
                    op: 'like'
                }, {
                    name: '入河排污口名称',
                    op: 'like'
                }]

            }),{
                text:'显示所有',
                hidden:me.rhpwkId == null?true:false,
                handler:function(){
                    me.gridview.reportCondition=null;
                    me.gridview.reload();
                    this.hide();
                }
            }]
        );
        grid.on('itemdblclick', function (g, record) {
            this.link(record);
            this.superMap.reLocation(record.get('排污口_ID'),record.get('入河排污口名称'),record.get(''),record.get('经纬度坐标'));
        }, this);
        if (this.rhpwkId) {
            this.gridview.grid.getStore().on('load', function (store, records) {
                var record = records[0];
                this.gridview.grid.getSelectionModel().select(0);
                this.link(record);
            }, this);
        }
    },
    link : function(record){
        this.form.setValues([{
            name: '入河排污口_ID',
            value:record.get('排污口_ID'),
            rowKeyValue: record.get('排污口_ID')
        },{
            name: 'WFZ_CD',
            value:record.get('水功能区代码'),
            rowKeyValue: record.get('入河排污口代码')
        },{
            name: 'WFZ_NM',
            value:record.get('水功能区名称'),
            rowKeyValue: record.get('入河排污口代码')
        },{
            name: 'PDO_CD',
            value:record.get('入河排污口代码'),
            rowKeyValue: record.get('入河排污口代码')
        },{
            name: 'PDO_NM',
            value:record.get('入河排污口名称'),
            rowKeyValue: record.get('入河排污口代码')
        },{
            name: 'WDPC_CD',
            value: record.get('排污许可证代码'),
            rowKeyValue: record.get('入河排污口代码')
        }, {
            name: 'ADDR',
            value: record.get('地址'),
            rowKeyValue: record.get('入河排污口代码')
        }, {
            name: 'PDO_TP',
            value: record.get('入河排污口性质ID'),
            rowKeyValue: record.get('入河排污口代码')
        }, {
            name: 'COMP_DT',
            value: Ext.util.Format.date(Ext.util.Format.substr(record.get('建成日期'),0,10), 'Y-m-d'),
            rowKeyValue: record.get('入河排污口代码')
        }, {
            name: 'IN_RV_TP',
            value: record.get('入河方式ID'),
            rowKeyValue: record.get('入河排污口代码')
        }, {
            name: 'EMIS_TP',
            value: record.get('排放方式ID'),
            rowKeyValue: record.get('入河排污口代码')
        }, {
            name: '经纬度坐标',
            value: record.get('经纬度坐标'),
            rowKeyValue: record.get('排污口_ID')
        }, {
            name: 'NT',
            value: record.get('备注'),
            rowKeyValue: record.get('入河排污口代码')
        }, {
            name: 'DWB_NM',
            value: record.get('排入水体ID'),
            rowKeyValue: record.get('入河排污口代码')
        },{
            name: 'DWB_NM1',
            value: record.get('排入水体名称'),
            rowKeyValue: record.get('入河排污口代码')
        }, {
            name: 'DES_POLL_CAP',
            value: record.get('设计日排污能力'),
            rowKeyValue: record.get('入河排污口代码')
        },{
            name: 'PDO_SIZE',
            value: record.get('排污口管径'),
            rowKeyValue: record.get('入河排污口代码')
        },{
            name: 'ENG_MAN_CD',
            value: record.get('管理单位代码'),
            rowKeyValue: record.get('入河排污口代码')
        },{
            name: 'TS',
            value: null,
            rowKeyValue:record.get('入河排污口代码')
        },{
            name: 'ENG_MAN_NM',
            value: record.get('管理单位名称'),
            rowKeyValue: record.get('入河排污口代码')
        }, {
            name: 'wr_sd_code',
            value: record.get('入河排污口代码'),
            rowKeyValue: record.get('排污口_ID')
        }]);
        var rhpwkId = this.rhpwkId = record.get("排污口_ID");
        var rhpwkDm = this.rhpwkDm = record.get("入河排污口代码");
        this.pwkSwczGrid.reload("入河排污口_ID='" + record.get('排污口_ID') + "'");
        this.pwkQsczGrid.reload("入河排污口_ID='" + record.get('排污口_ID') + "'");
        Ext.Ajax.request({
            url: __path + "/mgmb/getPwkqysh.do",
            params: {
                dbName: '_self',
                rhpwkId: rhpwkId
            },
            method: 'post',
            success: function (response) {
                var result = Ext.decode(response.responseText);
                var records = result.rows;
                this.selectPwkqyshRecords(records);
            },
            scope: this
        });
    },
    selectPwkqyshRecords: function (records) {
        var grid = this.paiwukouQuyongshuihu.grid;
        grid.selectRecords(function (record) {
            return records.indexOf(record.get('取用水户_ID')) >= 0;
        });
    }
});