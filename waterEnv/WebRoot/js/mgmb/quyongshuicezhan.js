Ext.define('bitc.wrenv.mgmb.quyongshuicezhan', {
    extend: 'Ext.panel.Panel',
    layout: 'border',
    qysczId:null,
    requires: ['Ext.grid.feature.Grouping','Ext.ux.InklingSearchField','bitc.report.widget.ChooseField'],
    initComponent: function () {
        var gridview = this.gridview = Ext.create('bitc.report.ReportView',{
            reportToken:'quyongshuicezhanReport',
            reportCondition: this.qysczId ? "取用水测站点_ID='" + this.qysczId + "'" : null,
            region: 'center',
            listeners: {
                gridready: this.initGrid,
                scope: this
            }
        });
        var qsList = {
            '地表取水口':'bitc.wrenv.mgmb.qushuikou',
            '地下取水井':'bitc.wrenv.mgmb.qushuijing'
        };
        var form = this.form = Ext.create('bitc.report.FormView',{
            formToken: 'quyongshuicezhanForm',
            title: '基本信息',
            fieldConfig: {
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
                                    tableName: 'mgmb_取用水测站测点简表',
                                    rowKey: '取用水测站点_ID',
                                    rowKeyValue: me.qysczId
                                }, {
                                    tableName: 'wr_wiust_b',
                                    rowKey: 'WIUST_CD',
                                    rowKeyValue: me.qysczDm
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
        var qushuikou = this.qushuikou = Ext.create('bitc.report.ReportView',{
            title: '取水口',
            reportToken: 'qysczqsk',
            defaultGridWidth : false,
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
                        id: type+record.get('取水口_ID'),
                        text: type+'详细信息',
                        action: qsList[type],
                        leaf: true,
                        iconCls: '',
                        config : {
                            qsId: record.get('取水口_ID')
                        }
                    };
                    addWorkTab(data);
                },
                scope: this
            }],
            listeners: {
                gridready: this.initQskGrid,
                scope: this
            }
        });

        var jiancedian = this.jiancedian = Ext.create('bitc.report.ReportView',{
            title: '取用水监测点',
            reportToken: 'jiancedianGrid',
            defaultGridWidth : false,
            reportCondition: "1=2",
            rowContextMenu: [{
                text: '详细',
                iconCls: '',
                handler: function (widget,record) {
                    var data={
                        id: 'quyongshuijiancedian',
                        text: '取用水监测点数据库',
                        action: 'bitc.wrenv.mgmb.quyongshuijiancedian',
                        leaf: true,
                        iconCls: '',
                        config: {
                            qysczId: record.get('取用水测点_ID')
                        }
                    };
                    addWorkTab(data);
                },
                scope: this
            }]
        });

        var quyongshuihu = this.quyongshuihu = Ext.create('bitc.report.ReportView',{
            title: '取用水户',
            reportToken: 'quyongshuihuGrid',
            defaultGridWidth : false,
            gridConfig: {
                selModel: {
                    selType: 'checkboxmodel',
                    mode: 'SIMPLE'
                }
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
            }],
            listeners: {
                gridready: this.initQyshGrid,
                scope: this
            }
        });
        var qysczPwk = this.qysczPwk = Ext.create('bitc.report.ReportView',{
            title: '监测排污口',
            reportToken: 'qysczPwk',
            gridConfig: {
                selModel: {
                    selType: 'checkboxmodel',
                    mode: 'SIMPLE'
                }
            },
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
            }],
            listeners: {
                gridready: this.initqysczPwkGrid,
                scope: this
            }
        });
        this.items = [gridview,{
             xtype: 'panel',             title:'相关信息',
            autoScroll: true,
            layout:'accordion',
            collapsible: true,
            split: true,
            collapseMode: 'mini',
            width: 600,
            frame: true,
            region: 'east',
            items: [form,superMap,qushuikou,jiancedian,quyongshuihu,qysczPwk]
        }];
        this.callParent(arguments);
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
                    text: '可通过取用水测站代码或者取用水测站名称查询'
                },
                inklings: [{
                    name: '取用水测站代码',
                    op: 'like'
                }, {
                    name: '取用水测站点名称',
                    op: 'like'
                }]

            }),{
                text:'显示所有',
                hidden:me.qysczId == null?true:false,
                handler:function(){
                    me.gridview.reportCondition=null;
                    me.gridview.reload();
                    this.hide();
                }
            }]
        );
        grid.on('itemdblclick', function (g, record) {
            this.link(record);
            this.superMap.reLocation(record.get('取用水测站点_ID'),record.get('取用水测站点名称'),record.get(''),record.get('经纬度坐标'));
        }, this);
        if (this.qysczId) {
            this.gridview.grid.getStore().on('load', function (store, records) {
                var record = records[0];
                this.gridview.grid.getSelectionModel().select(0);
                this.link(record);
            }, this);
        }
    },
    link : function(record){
        this.form.setValues([{
            name: '取用水测站点_ID',
            value:record.get('取用水测站点_ID'),
            rowKeyValue: record.get('取用水测站点_ID')
        },{
            name: 'WIUST_CD',
            value:record.get('取用水测站代码'),
            rowKeyValue: record.get('取用水测站代码')
        }, {
            name: 'WIUST_NM',
            value: record.get('取用水测站点名称'),
            rowKeyValue: record.get('取用水测站代码')
        }, {
            name: 'WIUST_TP',
            value: record.get('取用水测站类别ID'),
            rowKeyValue: record.get('取用水测站代码')
        }, {
            name: 'LOC',
            value: record.get('所在地'),
            rowKeyValue: record.get('取用水测站代码')
        }, {
            name: 'DAT_TP',
            value: record.get('基面类型ID'),
            rowKeyValue: record.get('取用水测站代码')
        }, {
            name: 'DAT_ELEV',
            value: record.get('基面高程'),
            rowKeyValue: record.get('取用水测站代码')
        }, {
            name: 'MOD_BAS_VAL',
            value: record.get('修正基值'),
            rowKeyValue: record.get('取用水测站代码')
        }, {
            name: 'MOD_PARA',
            value: record.get('修正参数'),
            rowKeyValue: record.get('取用水测站代码')
        },{
            name: 'WELL_TP',
            value: record.get('测井类型ID'),
            rowKeyValue: record.get('取用水测站代码')
        },{
            name: 'WELL_LAND_ELEV',
            value: record.get('测井地面高程'),
            rowKeyValue: record.get('取用水测站代码')
        },{
            name: 'EST_ST_YM',
            value: record.get('设站年月'),
            rowKeyValue: record.get('取用水测站代码')
        },{
            name: '经纬度坐标',
            value: record.get('经纬度坐标'),
            rowKeyValue: record.get('取用水测站点_ID')
        }, {
            name: 'NT',
            value: record.get('备注'),
            rowKeyValue: record.get('取用水测站代码')
        }, {
            name: 'TYPE_ID',
            value: 1,
            rowKeyValue: record.get('取用水测站点_ID')
        }, {
            name: 'TS',
            value: null,
            rowKeyValue: record.get('取用水测站代码')
        }, {
            name: 'wr_sd_code',
            value: record.get('取用水测站代码'),
            rowKeyValue: record.get('取用水测站点_ID')
        },{
            name: 'ENG_MAN_CD',
            value: record.get('管理单位代码ID'),
            rowKeyValue: record.get('取用水测站代码')
        },{
            name: 'ENG_MAN_NM',
            value: record.get('管理单位名称'),
            rowKeyValue: record.get('取用水测站代码')
        }]);
        var qysczId = this.qysczId = record.get("取用水测站点_ID");
        var qysczDm = this.qysczDm = record.get("取用水测站代码");
        this.jiancedian.reload("取用水测站_ID='" + record.get('取用水测站点_ID') + "'");
        Ext.Ajax.request({
            url: __path + "/mgmb/getQsk.do",
            params: {
                dbName: '_self',
                qysczId: qysczId
            },
            method: 'post',
            success: function (response) {
                var result = Ext.decode(response.responseText);
                var records = result.rows;
                this.selectQskRecords(records);
            },
            scope: this
        });

        Ext.Ajax.request({
            url: __path + "/mgmb/getQysh.do",
            params: {
                dbName: '_self',
                qysczId: qysczId
            },
            method: 'post',
            success: function (response) {
                var result = Ext.decode(response.responseText);
                var records = result.rows;
                this.selectQyshRecords(records);
            },
            scope: this
        });

        Ext.Ajax.request({
            url: __path + "/mgmb/getQysczpwk.do",
            params: {
                dbName: '_self',
                qysczId: qysczId
            },
            method: 'post',
            success: function (response) {
                var result = Ext.decode(response.responseText);
                var records = result.rows;
                this.selectQysczpwkRecords(records);
            },
            scope: this
        });
    },
    selectQysczpwkRecords: function (records) {
        var grid = this.qysczPwk.grid;
        grid.selectRecords(function (record) {
            return records.indexOf(record.get('排污口_ID')) >= 0;
        });
    },
    selectQskRecords: function (records) {
        var grid = this.qushuikou.grid;
        grid.selectRecords(function (record) {
            return records.indexOf(record.get('取水口_ID')) >= 0;
        });
    },
    selectQyshRecords: function (records) {
        var grid = this.quyongshuihu.grid;
        grid.selectRecords(function (record) {
            return records.indexOf(record.get('取用水户_ID')) >= 0;
        });
    },
    initQskGrid : function (panel, grid){
        grid.down('toolbar').insert(0, {
            text: '保存',
            scope: this,
            handler: function () {
                var records = grid.getSelectionModel().getSelection();
                if (records.length == 0) {
                    Ext.MessageBox.alert("提示", "请先选中您要操作的行!");
                    return;
                } else if (this.qysczId == null) {
                    Ext.MessageBox.alert("提示", "请先选择相关联的取用水测站!");
                    return;
                } else {
                    var qskIds = "";
                    for (var i = 0; i < records.length; i++) {
                        qskIds += records[i].get("取水口_ID");
                        if (i < records.length - 1) {
                            qskIds = qskIds + ",";
                        }
                    }
                    Ext.Ajax.request({
                        url: __path + "/mgmb/saveQsk.do",
                        params: {
                            qysczId: this.qysczId,
                            qskIds: qskIds,
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
    initqysczPwkGrid : function(panel,grid){
        grid.down('toolbar').insert(0, {
            text: '保存',
            scope: this,
            handler: function () {
                var records = grid.getSelectionModel().getSelection();
                if (records.length == 0) {
                    Ext.MessageBox.alert("提示", "请先选中您要操作的行!");
                    return;
                } else if (this.qysczId == null) {
                    Ext.MessageBox.alert("提示", "请先选择相关联的取用水测站!");
                    return;
                } else {
                    var pwkIds = "";
                    for (var i = 0; i < records.length; i++) {
                        pwkIds += records[i].get("排污口_ID");
                        if (i < records.length - 1) {
                            pwkIds = pwkIds + ",";
                        }
                    }
                    Ext.Ajax.request({
                        url: __path + "/mgmb/saveQysczpwk.do",
                        params: {
                            qysczId: this.qysczId,
                            pwkIds: pwkIds,
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
    initQyshGrid : function(panel,grid){
        grid.down('toolbar').insert(0, {
            text: '保存',
            scope: this,
            handler: function () {
                var records = grid.getSelectionModel().getSelection();
                if (records.length == 0) {
                    Ext.MessageBox.alert("提示", "请先选中您要操作的行!");
                    return;
                } else if (this.qysczId == null) {
                    Ext.MessageBox.alert("提示", "请先选择相关联的取用水测站!");
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
                        url: __path + "/mgmb/saveQysh.do",
                        params: {
                            qysczId: this.qysczId,
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
    }
});