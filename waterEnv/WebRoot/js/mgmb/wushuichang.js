Ext.define('bitc.wrenv.mgmb.wushuichang', {
    extend: 'Ext.panel.Panel',
    requires: ['Ext.grid.feature.Grouping','Ext.ux.InklingSearchField','bitc.report.widget.ChooseField'],
    layout: 'border',
    initComponent: function () {
        var gridview = this.gridview = Ext.create('bitc.report.ReportView', {
            reportToken: 'wushuichangReport',
            region: 'center',
            //reportCondition: this.rhpwkId ? "排污口_ID='" + this.rhpwkId + "'" : null,
            columnConfig:{
                '建成日期':{renderer:function(value){
                    if(value){
                        var str = Ext.util.Format.substr(value,0,10);
                        return Ext.util.Format.date(str,'Y-m-d');
                    }
                    return '';
                }},
                "服务人口" : {
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
            formToken: 'wushuichangForm',
            title: '基本信息',
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
                                    tableName: 'mgmb_污水处理厂信息简表',
                                    rowKey: '污水处理厂_ID',
                                    rowKeyValue:me.wscId
                                }, {
                                    tableName: 'wr_stp_b',
                                    rowKey: 'STP_CD',
                                    rowKeyValue: me.wscDm
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
        this.items = [gridview,{
             xtype: 'panel',             title:'相关信息',
            autoScroll: true,
            collapsible: true,
            split: true,
            layout:'accordion',
            collapseMode: 'mini',
            width: 600,
            frame: true,
            region: 'east',
            items: [form,superMap]
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
                    text: '可通过污水处理厂代码或者污水处理厂名称查询'
                },
                inklings: [{
                    name: '污水处理厂代码',
                    op: 'like'
                }, {
                    name: '污水处理厂名称',
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
            this.superMap.reLocation(record.get('污水处理厂_ID'),record.get('污水处理厂名称'),record.get(''),record.get('经纬度坐标'));
        }, this);
        //if (this.rhpwkId) {
        //    this.gridview.grid.getStore().on('load', function (store, records) {
        //        var record = records[0];
        //        this.gridview.grid.getSelectionModel().select(0);
        //        this.link(record);
        //    }, this);
        //}
    },
    link : function(record){
        this.form.setValues([{
            name: '污水处理厂_ID',
            value:record.get('污水处理厂_ID'),
            rowKeyValue: record.get('污水处理厂_ID')
        },{
            name: 'STP_CD',
            value:record.get('污水处理厂代码'),
            rowKeyValue: record.get('污水处理厂代码')
        },{
            name: 'STP_NM',
            value:record.get('污水处理厂名称'),
            rowKeyValue: record.get('污水处理厂代码')
        },{
            name: 'TREA_UNIT_CONS',
            value:record.get('处理单耗'),
            rowKeyValue: record.get('污水处理厂代码')
        },{
            name: 'ST_CRAF',
            value: record.get('污水处理工艺'),
            rowKeyValue: record.get('污水处理厂代码')
        }, {
            name: 'SLUD_TREA_CRAF',
            value: record.get('污泥处理工艺'),
            rowKeyValue: record.get('污水处理厂代码')
        }, {
            name: 'LAND_A',
            value: record.get('占地面积'),
            rowKeyValue: record.get('污水处理厂代码')
        }, {
            name: 'SERV_OBJ',
            value: record.get('服务对象'),
            rowKeyValue: record.get('污水处理厂代码')
        }, {
            name: 'SERV_A',
            value: record.get('服务面积'),
            rowKeyValue: record.get('污水处理厂代码')
        }, {
            name: 'SERV_PP',
            value: record.get('服务人口'),
            rowKeyValue: record.get('污水处理厂代码')
        }, {
            name: '经纬度坐标',
            value: record.get('经纬度坐标'),
            rowKeyValue: record.get('污水处理厂_ID')
        }, {
            name: 'NT',
            value: record.get('备注'),
            rowKeyValue: record.get('污水处理厂代码')
        }, {
            name: 'CONS_SCAL',
            value: record.get('建设规模ID'),
            rowKeyValue: record.get('污水处理厂代码')
        },{
            name: 'COMP_DT',
            value: Ext.util.Format.date(Ext.util.Format.substr(record.get('建成日期'),0,10), 'Y-m-d'),
            rowKeyValue: record.get('污水处理厂代码')
        },{
            name: 'TS',
            value: null,
            rowKeyValue:record.get('污水处理厂代码')
        }, {
            name: 'wr_sd_code',
            value: record.get('污水处理厂代码'),
            rowKeyValue: record.get('污水处理厂_ID')
        }]);
        var wscId = this.wscId = record.get("污水处理厂_ID");
        var wscDm = this.wscDm = record.get("污水处理厂代码");
        //this.pwkSwczGrid.reload("入河排污口_ID='" + record.get('排污口_ID') + "'");
    }
});
