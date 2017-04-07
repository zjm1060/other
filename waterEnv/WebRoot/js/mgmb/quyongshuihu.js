Ext.define('bitc.wrenv.mgmb.quyongshuihu', {
    extend: 'Ext.panel.Panel',
    requires: ['Ext.grid.feature.Grouping','Ext.ux.InklingSearchField','bitc.report.widget.ChooseField'],
    layout: 'border',
    qyshId : null,
    initComponent: function () {
        var gridview = this.gridview = Ext.create('bitc.report.ReportView', {
            reportToken: 'quyongshuihuReport',
            reportCondition: this.qysczId ? "取用水户_ID='" + this.qysczId + "'" : null,
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
            formToken: 'quyongshuihuForm',
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
                                    tableName: 'mgmb_取用水户信息简表',
                                    rowKey: '取用水户_ID',
                                    rowKeyValue: me.qyshId
                                }, {
                                    tableName: 'wr_wiu_b',
                                    rowKey: 'WIU_CD',
                                    rowKeyValue: me.qyshDm
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

        var qyshquyongshuijiancedian = this.qyshquyongshuijiancedian = Ext.create('bitc.report.ReportView',{
            title: '取用水监测点',
            reportToken: 'qyshquyongshuijiancedian',
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
                            qysczId: record.get('取用水测站点_ID')
                        }
                    };
                    addWorkTab(data);
                },
                scope: this
            }]
        });

        var qyshqushuikou = this.qyshqushuikou = Ext.create('bitc.report.ReportView',{
            title: '取水口',
            reportToken: 'qyshqushuikou',
            defaultGridWidth : false,
            reportCondition: "1=2",
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
                }]
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
            }]
        });

        var qyshPwk = this.qyshPwk = Ext.create('bitc.report.ReportView',{
            title: '排污口',
            reportToken: 'qyshPwk',
            reportCondition: "1=2",
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
                            rhpwkId: record.get('入河排污口_ID')
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
            items: [form,qyshquyongshuijiancedian,qyshqushuikou,qyshPwk]
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
                    text: '可通过取用水户代码或者取用水户名称查询'
                },
                inklings: [{
                    name: '取用水户代码',
                    op: 'like'
                }, {
                    name: '取用水户名称',
                    op: 'like'
                }]

            }),{
                text:'显示所有',
                hidden:me.qyshId == null?true:false,
                handler:function(){
                    me.gridview.reportCondition=null;
                    me.gridview.reload();
                    this.hide();
                }
            }]
        );
        grid.on('itemdblclick', function (g, record) {
            this.link(record);
        }, this);
        if (this.qyshId) {
            this.gridview.grid.getStore().on('load', function (store, records) {
                var record = records[0];
                this.gridview.grid.getSelectionModel().select(0);
                this.link(record);
            }, this);
        }
    },
    link : function(record){
        this.form.setValues([{
            name: '取用水户_ID',
            value:record.get('取用水户_ID'),
            rowKeyValue: record.get('取用水户_ID')
        },{
            name: 'WIU_CD',
            value:record.get('取用水户代码'),
            rowKeyValue: record.get('取用水户代码')
        }, {
            name: 'WIU_NM',
            value: record.get('取用水户名称'),
            rowKeyValue: record.get('取用水户代码')
        }, {
            name: 'LR_NM',
            value: record.get('法人代表'),
            rowKeyValue: record.get('取用水户代码')
        }, {
            name: 'TRAD_TP',
            value: record.get('行业类别ID'),
            rowKeyValue: record.get('取用水户代码')
        }, {
            name: 'ECO_TP',
            value: record.get('经济类型ID'),
            rowKeyValue: record.get('取用水户代码')
        }, {
            name: 'MON_G',
            value: record.get('监控级别ID'),
            rowKeyValue: record.get('取用水户代码')
        }, {
            name: 'WEB',
            value: record.get('网址'),
            rowKeyValue: record.get('取用水户代码')
        }, {
            name: 'E_MAIL',
            value: record.get('邮箱'),
            rowKeyValue: record.get('取用水户代码')
        }, {
            name: 'ADDR',
            value: record.get('地址'),
            rowKeyValue: record.get('取用水户代码')
        }, {
            name: 'ZIP',
            value: record.get('邮编'),
            rowKeyValue: record.get('取用水户代码')
        },{
            name: 'TEL',
            value: record.get('办公室电话'),
            rowKeyValue: record.get('取用水户代码')
        },{
            name: '经纬度坐标',
            value: record.get('经纬度坐标'),
            rowKeyValue: record.get('取用水户_ID')
        }, {
            name: 'NT',
            value: record.get('备注'),
            rowKeyValue: record.get('取用水户代码')
        }, {
            name: 'FAX',
            value: record.get('传真号码'),
            rowKeyValue: record.get('取用水户_ID')
        }, {
            name: 'TS',
            value: null,
            rowKeyValue: record.get('取用水户代码')
        }, {
            name: 'wr_sd_code',
            value: record.get('取用水户代码'),
            rowKeyValue: record.get('取用水户_ID')
        }]);
        var qyshId = this.qyshId = record.get("取用水户_ID");
        var qyshDm = this.qyshDm = record.get("取用水户代码");
        this.qyshquyongshuijiancedian.reload("取用水户_ID='"+record.get("取用水户_ID")+"'");
        this.qyshqushuikou.reload("取用水户_ID='"+record.get("取用水户_ID")+"'");
        this.qyshPwk.reload("取用水户_ID='"+record.get("取用水户_ID")+"'");
    }
});