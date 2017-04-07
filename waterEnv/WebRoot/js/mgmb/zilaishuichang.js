Ext.define('bitc.wrenv.mgmb.zilaishuichang', {
    extend: 'Ext.panel.Panel',
    requires: ['Ext.grid.feature.Grouping','Ext.ux.InklingSearchField','bitc.report.widget.ChooseField'],
    layout: 'border',
    initComponent: function () {
        var gridview = this.gridview = Ext.create('bitc.report.ReportView', {
            reportToken: 'zilaishuichangReport',
            region: 'center',
            //reportCondition: this.qsId ? "取水口_ID='" + this.qsId + "'" : null,
            columnConfig : {
              '清水库容量' : {
                  renderer : function(value,meta,record){
                      meta.align = "right";
                      return value;
                  }
              },
                '日生产能力' : {
                    renderer : function(value,meta,record){
                        meta.align = "right";
                        return value;
                    }
                },
                '供水人口': {
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
            formToken: 'zilaishuichangForm',
            title: '基本信息',
            fieldConfig: {
                WFCT_CD: {
                    setValueFun: function (record) {
                        this.form.getForm().findField('WFCT_CD').setValue(record.get('ORG_CD'));
                        this.form.getForm().findField('WFCT_NM').setValue(record.get('ORG_NM'));
                    },
                    clearValueFun: function () {
                        this.form.getForm().findField('WFCT_CD').setValue(null);
                        this.form.getForm().findField('WFCT_NM').setValue(null);
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
                    //submitValue: false,
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
                WFCT_NM : {
                    submitValue: false
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
                                    tableName: 'mgmb_自来水厂信息简表',
                                    rowKey: '自来水厂_ID',
                                    rowKeyValue: me.zlscId
                                }, {
                                    tableName: 'wr_wfct_b',
                                    rowKey: 'WFCT_CD',
                                    rowKeyValue: me.zlscDm
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
                    text: '可通过自来水厂代码或者自来水厂名称查询'
                },
                inklings: [{
                    name: '自来水厂代码',
                    op: 'like'
                }, {
                    name: '自来水厂名称',
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
            this.superMap.reLocation(record.get('自来水厂_ID'),record.get('自来水厂名称'),record.get(''),record.get('经纬度坐标'));
        }, this);
        //if (this.qsId) {
        //    this.gridview.grid.getStore().on('load', function (store, records) {
        //        var record = records[0];
        //        this.gridview.grid.getSelectionModel().select(0);
        //        this.link(record);
        //    }, this);
        //}
    },
    link : function(record){
        this.form.setValues([{
            name: '自来水厂_ID',
            value:record.get('自来水厂_ID'),
            rowKeyValue: record.get('自来水厂_ID')
        },{
            name: 'WFCT_CD',
            value:record.get('自来水厂代码'),
            rowKeyValue: record.get('自来水厂代码')
        },{
            name: 'WFCT_NM',
            value:record.get('自来水厂名称'),
            rowKeyValue: record.get('自来水厂代码')
        },{
            name: 'FRESH_V',
            value: record.get('清水库容量'),
            rowKeyValue: record.get('自来水厂代码')
        }, {
            name: 'DAY_CAP',
            value: record.get('日生产能力'),
            rowKeyValue: record.get('自来水厂代码')
        }, {
            name: 'PROD_LTP',
            value: record.get('出厂管线'),
            rowKeyValue: record.get('自来水厂代码')
        }, {
            name: 'PROD_LTV',
            value: record.get('出厂阀门'),
            rowKeyValue: record.get('自来水厂代码')
        }, {
            name: 'IN_LTP',
            value: record.get('进厂原水管线'),
            rowKeyValue: record.get('自来水厂代码')
        }, {
            name: 'IN_LTV',
            value: record.get('进厂原水阀门'),
            rowKeyValue: record.get('自来水厂代码')
        }, {
            name: '经纬度坐标',
            value: record.get('经纬度坐标'),
            rowKeyValue: record.get('自来水厂_ID')
        }, {
            name: 'NT',
            value: record.get('备注'),
            rowKeyValue: record.get('自来水厂代码')
        }, {
            name: 'PUMP1_NUM',
            value: record.get('一泵房出水泵数'),
            rowKeyValue: record.get('自来水厂代码')
        },{
            name: 'PUMP1_H',
            value: record.get('一泵房出水泵扬程'),
            rowKeyValue: record.get('自来水厂代码')
        }, {
            name: 'PUMP1_Q',
            value: record.get('一泵房出水泵流量'),
            rowKeyValue: record.get('自来水厂代码')
        },{
            name: 'PUMP2_NUM',
            value: record.get('二泵房出水泵数'),
            rowKeyValue: record.get('自来水厂代码')
        },{
            name: 'PUMP2_H',
            value: record.get('二泵房出水泵扬程'),
            rowKeyValue: record.get('自来水厂代码')
        },{
            name: 'PUMP2_Q',
            value: record.get('二泵房出水泵流量'),
            rowKeyValue: record.get('自来水厂代码')
        },{
            name: 'WS_OBJ',
            value: record.get('供水对象ID'),
            rowKeyValue: record.get('自来水厂代码')
        },{
            name: 'WS_PP',
            value: record.get('供水人口'),
            rowKeyValue: record.get('自来水厂代码')
        },{
            name: 'WW_COND',
            value: record.get('取水计量情况'),
            rowKeyValue: record.get('自来水厂代码')
        },{
            name: 'TS',
            value: null,
            rowKeyValue: record.get('自来水厂代码')
        }, {
            name: 'wr_sd_code',
            value: record.get('自来水厂代码'),
            rowKeyValue: record.get('自来水厂_ID')
        }]);
        var zlscId = this.zlscId = record.get("自来水厂_ID");
        var zlscDm = this.zlscDm = record.get("自来水厂代码");
        //this.quyongshuicz.reload("取水口_ID='" + record.get('取水口_ID') + "'");
    }
});