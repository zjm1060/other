Ext.define('bitc.wrenv.mgmb.qushuikou', {
    extend: 'Ext.panel.Panel',
    requires: ['Ext.grid.feature.Grouping','Ext.ux.InklingSearchField','bitc.report.widget.ChooseField'],
    layout: 'border',
    qsId : null,
    initComponent: function () {
        var gridview = this.gridview = Ext.create('bitc.report.ReportView', {
            reportToken: 'qushuikouReport',
            region: 'center',
            reportCondition: this.qsId ? "取水口_ID='" + this.qsId + "'" : null,
            columnConfig:{
                '开始取水日期':{
                    renderer:function(value){
                        if(value){
                            var str = Ext.util.Format.substr(value,0,10);
                            return Ext.util.Format.date(str,'Y-m-d');
                        }
                        return '';
                    }
                },
                '许可最大流量':{
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
        var qysList = {
            '取用水测点':'bitc.wrenv.mgmb.quyongshuijiancedian',
            '取用水测站':'bitc.wrenv.mgmb.quyongshuicezhan'
        };
        var form = this.form = Ext.create('bitc.report.FormView',{
            formToken: 'qushuikouForm',
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
                                    tableName: 'mgmb_取水口信息简表',
                                    rowKey: '取水口_ID',
                                    rowKeyValue: me.qskId
                                }, {
                                    tableName: 'wr_int_b',
                                    rowKey: 'INT_CD',
                                    rowKeyValue: me.qskDm
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
        var quyongshuicz = this.quyongshuicz = Ext.create('bitc.report.ReportView',{
            reportToken: 'quyongshuiczGrid',
            title: '取用水测站',
            defaultGridWidth : false,
            reportCondition: "1=2",
            rowContextMenu: [{
                text: '详细',
                iconCls: '',
                handler: function (widget,record, event) {
                    var type = record.get('取用水测站点名称');
                    var data={
                        id: type+record.get('取用水测站点_ID'),
                        text: type+'详细信息',
                        action: qysList[type],
                        leaf: true,
                        iconCls: '',
                        config : {
                            qysczId: record.get('取用水测站点_ID')
                        }
                    };
                    addWorkTab(data);
                },
                scope: this
            }]
        });

        var quyongshuihu = this.quyongshuihu = Ext.create('bitc.report.ReportView',{
            reportToken: 'qskqyshGrid',
            title: '取用水户',
            defaultGridWidth : false,
            gridConfig: {
                selModel: {
                    selType: 'checkboxmodel',
                    mode: 'SIMPLE'
                }
            },
            listeners: {
                gridready: this.initQskqyshGrid,
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
            items: [form,superMap,quyongshuicz,quyongshuihu]
        }];
        this.callParent(arguments);
    },
    initQskqyshGrid : function(panel,grid){
        grid.down('toolbar').insert(0, {
            text: '保存',
            scope: this,
            handler: function () {
                var records = grid.getSelectionModel().getSelection();
                if (records.length == 0) {
                    Ext.MessageBox.alert("提示", "请先选中您要操作的行!");
                    return;
                } else if (this.qskId == null) {
                    Ext.MessageBox.alert("提示", "请先选择相关联的取水口!");
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
                        url: __path + "/mgmb/saveQskqysh.do",
                        params: {
                            qskId: this.qskId,
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
                    text: '可通过地表水取水口代码或者地表水取水口名称查询'
                },
                inklings: [{
                    name: '地表水取水口代码',
                    op: 'like'
                }, {
                    name: '地表水取水口名称',
                    op: 'like'
                }]

            }),{
                text:'显示所有',
                hidden:me.qsId == null?true:false,
                handler:function(){
                    me.gridview.reportCondition=null;
                    me.gridview.reload();
                    this.hide();
                }
            }]
        );
        grid.on('itemdblclick', function (g, record) {
            this.link(record);
            this.superMap.reLocation(record.get('取水口_ID'),record.get('地表水取水口名称'),record.get(''),record.get('经纬度坐标'));
        }, this);
        if (this.qsId) {
            this.gridview.grid.getStore().on('load', function (store, records) {
                var record = records[0];
                this.gridview.grid.getSelectionModel().select(0);
                this.link(record);
            }, this);
        }
    },
    link : function(record){
        this.form.setValues([{
            name: '取水口_ID',
            value:record.get('取水口_ID'),
            rowKeyValue: record.get('取水口_ID')
        },{
            name: 'INT_CD',
            value:record.get('地表水取水口代码'),
            rowKeyValue: record.get('地表水取水口代码')
        }, {
            name: 'INT_NM',
            value: record.get('地表水取水口名称'),
            rowKeyValue: record.get('地表水取水口代码')
        }, {
            name: 'INT_TP',
            value: record.get('取水方式ID'),
            rowKeyValue: record.get('地表水取水口代码')
        }, {
            name: 'FROM_INT_DT',
            value: Ext.util.Format.date(Ext.util.Format.substr(record.get('开始取水日期'),0,10), 'Y-m-d'),
            rowKeyValue: record.get('地表水取水口代码')
        }, {
            name: 'MAX_PERM_Q',
            value: record.get('许可最大流量'),
            rowKeyValue: record.get('地表水取水口代码')
        }, {
            name: 'DES_Q',
            value: record.get('设计流量'),
            rowKeyValue: record.get('地表水取水口代码')
        }, {
            name: 'PERM_WW',
            value: record.get('许可总取水量'),
            rowKeyValue: record.get('地表水取水口代码')
        }, {
            name: '经纬度坐标',
            value: record.get('经纬度坐标'),
            rowKeyValue: record.get('取水口_ID')
        }, {
            name: 'NT',
            value: record.get('备注'),
            rowKeyValue: record.get('地表水取水口代码')
        }, {
            name: 'TYPE_ID',
            value: 1,
            rowKeyValue: record.get('取水口_ID')
        }, {
            name: 'TS',
            value: null,
            rowKeyValue: record.get('地表水取水口代码')
        }, {
            name: 'wr_sd_code',
            value: record.get('地表水取水口代码'),
            rowKeyValue: record.get('取水口_ID')
        },{
            name: 'ENG_MAN_CD',
            value: record.get('管理单位代码'),
            rowKeyValue: record.get('地表水取水口代码')
        },{
            name: 'ENG_MAN_NM',
            value: record.get('管理单位代码名称'),
            rowKeyValue: record.get('地表水取水口代码')
        }]);
        var qskId = this.qskId = record.get("取水口_ID");
        var qskDm = this.qskDm = record.get("地表水取水口代码");
        this.quyongshuicz.reload("取水口_ID='" + record.get('取水口_ID') + "'");
        Ext.Ajax.request({
            url: __path + "/mgmb/getQskqysh.do",
            params: {
                dbName: '_self',
                qskId: qskId
            },
            method: 'post',
            success: function (response) {
                var result = Ext.decode(response.responseText);
                var records = result.rows;
                this.selectQskqyshRecords(records);
            },
            scope: this
        });
    },
    selectQskqyshRecords: function (records) {
        var grid = this.quyongshuihu.grid;
        grid.selectRecords(function (record) {
            return records.indexOf(record.get('取用水户_ID')) >= 0;
        });
    }
});