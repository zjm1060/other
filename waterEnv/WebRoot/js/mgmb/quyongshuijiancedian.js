Ext.define('bitc.wrenv.mgmb.quyongshuijiancedian', {
    extend: 'Ext.panel.Panel',
    layout: 'border',
    qysczId : null,
    requires: ['Ext.grid.feature.Grouping','Ext.ux.InklingSearchField','bitc.report.widget.ChooseField'],
    initComponent: function () {
        var gridview = this.gridview = Ext.create('bitc.report.ReportView',{
            reportToken:'quyongshuijiancedianReport',
            reportCondition: this.qysczId ? "取用水测站点_ID='" + this.qysczId + "'" : null,
            region: 'center',
            listeners: {
                gridready: this.initGrid,
                scope: this
            }
        });
        var form = this.form = Ext.create('bitc.report.FormView',{
            formToken: 'quyongshuijiancedianForm',
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
                                    tableName: 'mgmb_取用水测站测点简表',
                                    rowKey: '取用水测站点_ID',
                                    rowKeyValue: me.qysczId
                                }, {
                                    tableName: 'wr_mp_b',
                                    rowKey: 'MP_CD',
                                    rowKeyValue: me.jcdDm
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
        var quyongshuicezhan = this.quyongshuicezhan = Ext.create('bitc.report.ReportView',{
            title: '取用水测站',
            reportToken: 'qysjcdcz',
            defaultGridWidth : false,
            gridConfig: {
                groupField: '取用水测站类别ID',
                features: [{
                    ftype: 'grouping',
                    groupHeaderTpl: ['{rows:this.getName}', {
                        getName: function (rows) {
                            return rows[0].get('取用水测站类别名称');
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
            }],
            listeners: {
                gridready: this.initQysczGrid,
                scope: this
            }
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
            items: [form,superMap,quyongshuicezhan]
        }];
        this.callParent(arguments);
    },
    initQysczGrid : function (panel, grid){
        grid.down('toolbar').insert(0, {
            text: '保存',
            scope: this,
            handler: function () {
                var records = grid.getSelectionModel().getSelection();
                if (records.length == 0) {
                    Ext.MessageBox.alert("提示", "请先选中您要操作的行!");
                    return;
                } else if (this.qysczId == null) {
                    Ext.MessageBox.alert("提示", "请先选择相关联的取用水监测点!");
                    return;
                } else {
                    var qysczdIds = "";
                    for (var i = 0; i < records.length; i++) {
                        qysczdIds += records[i].get("取用水测站点_ID");
                        if (i < records.length - 1) {
                            qysczdIds = qysczdIds + ",";
                        }
                    }
                    Ext.Ajax.request({
                        url: __path + "/mgmb/saveQysczd.do",
                        params: {
                            qysczId: this.qysczId,
                            qysczdIds: qysczdIds,
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
        grid.down('toolbar').insert(0, Ext.create('Ext.ux.InklingSearchField', {
                width: 180,
                emptyText: '请输入关键字查询',
                reportView: this.gridview,
                tooltip: {
                    title: '条件查询',
                    text: '可通过取用水监测点代码或者取用水监测点名称查询'
                },
                inklings: [{
                    name: '取用水监测点代码',
                    op: 'like'
                }, {
                    name: '取用水监测点名称',
                    op: 'like'
                }]

            })
        );
        grid.on('itemdblclick', function (g, record) {
            this.link(record);
            this.superMap.reLocation(record.get('取用水测站点_ID'),record.get('取用水监测点名称'),record.get(''),record.get('经纬度坐标'));
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
            name: 'MP_CD',
            value:record.get('取用水监测点代码'),
            rowKeyValue: record.get('取用水监测点代码')
        }, {
            name: 'MP_NM',
            value: record.get('取用水监测点名称'),
            rowKeyValue: record.get('取用水监测点代码')
        }, {
            name: 'MP_ADDR',
            value: record.get('监测点地址'),
            rowKeyValue: record.get('取用水监测点代码')
        }, {
            name: 'MP_ITEM',
            value: record.get('监测项目ID'),
            rowKeyValue: record.get('取用水监测点代码')
        },{
            name: '经纬度坐标',
            value: record.get('经纬度坐标'),
            rowKeyValue: record.get('取用水测站点_ID')
        }, {
            name: 'NT',
            value: record.get('备注'),
            rowKeyValue: record.get('取用水监测点代码')
        }, {
            name: 'TYPE_ID',
            value: 2,
            rowKeyValue: record.get('取用水测站点_ID')
        }, {
            name: 'TS',
            value: null,
            rowKeyValue: record.get('取用水监测点代码')
        }, {
            name: 'wr_sd_code',
            value: record.get('取用水监测点代码'),
            rowKeyValue: record.get('取用水测站点_ID')
        }]);
        var qysczId = this.qysczId = record.get("取用水测站点_ID");
        var jcdDm = this.jcdDm = record.get("取用水监测点代码");
        //this.quyongshuicz.reload("取水口_ID='" + record.get('取水口_ID') + "'");
        Ext.Ajax.request({
            url: __path + "/mgmb/getQysczd.do",
            params: {
                dbName: '_self',
                qysczId: qysczId
            },
            method: 'post',
            success: function (response) {
                var result = Ext.decode(response.responseText);
                var records = result.rows;
                this.selectQysczdRecords(records);
            },
            scope: this
        });
    },
    selectQysczdRecords: function (records) {
        var grid = this.quyongshuicezhan.grid;
        grid.selectRecords(function (record) {
            return records.indexOf(record.get('取用水测站点_ID')) >= 0;
        });
    }
});