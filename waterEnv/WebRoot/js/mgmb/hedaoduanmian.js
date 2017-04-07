Ext.define('bitc.wrenv.mgmb.hedaoduanmian', {
    extend: 'Ext.panel.Panel',
    requires: ['Ext.grid.feature.Grouping','Ext.ux.InklingSearchField','bitc.report.widget.ChooseField'],
    layout: 'border',
    hddmId : null,
    initComponent: function () {
        var gridview = this.gridview = Ext.create('bitc.report.ReportView',{
            reportToken: 'hedaoduanmianReport',
            //reportCondition: this.hddmId ? "断面_ID='" + this.hddmId + "'" : null,
            region: 'center',
            columnConfig : {
                '水功能区监测断面' : {
                    renderer : function(value,meta,record){
                        meta.column.width = 125;
                        return value;
                    }
                },
                '行政区界断面' : {
                    renderer : function(value,meta,record){
                        meta.column.width  = 100;
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
            formToken: 'hedaoduanmianForm',
            title: '基本信息',
            fieldConfig: {
                水功能区_NM: {
                    setValueFun: function (record) {
                        this.form.getForm().findField('水功能区_ID').setValue(record.get('水功能区_ID'));
                        this.form.getForm().findField('水功能区_NM').setValue(record.get('水功能区名称'));
                    },
                    clearValueFun: function () {
                        this.form.getForm().findField('水功能区_ID').setValue(null);
                        this.form.getForm().findField('水功能区_NM').setValue(null);
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
                                name: "WFZ_NM",
                                op: "like"
                            }
                        ]
                    },
                    scope: this
                },
                水体_NM: {
                    setValueFun: function (record) {
                        this.form.getForm().findField('水体_ID').setValue(record.get('水体_ID'));
                        this.form.getForm().findField('水体_NM').setValue(record.get('水体名称'));
                    },
                    clearValueFun: function () {
                        this.form.getForm().findField('水体_ID').setValue(null);
                        this.form.getForm().findField('水体_NM').setValue(null);
                    },
                    winConfig: {
                        "title": "水体名称"
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
                                    tableName: 'wr_csb_b',
                                    rowKey: 'RB_CD',
                                    rowKeyValue: me.hddmdm
                                },{
                                    tableName: 'wr_wfzb_b',
                                    rowKey: 'RB_CD',
                                    rowKeyValue: me.hddmdm
                                },{
                                    tableName: 'wr_ab_b',
                                    rowKey: 'RB_CD',
                                    rowKeyValue: me.hddmdm
                                },{
                                    tableName: 'wr_rb_b',
                                    rowKey: 'RB_CD',
                                    rowKeyValue: me.hddmdm
                                },{
                                    tableName: 'mgmb_河道断面信息简表',
                                    rowKey: '河道断面_ID',
                                    rowKeyValue: me.hddmId
                                },{
                                    tableName: 'mgmb_rel_断面与水体水功能区',
                                    rowKey: '河道断面_ID',
                                    rowKeyValue: me.hddmId
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

        var kzdmForm = this.kzdmForm = Ext.create('bitc.report.FormView',{
            formToken: 'kzdmForm',
            title: '控制断面',
            listeners: {
                'savesuccess': function () {
                    this.grid.getStore().reload();
                    this.kzdmForm.getForm().reset();
                    this.kzdmForm.getForm().getFields().each(function (field) {
                        field.rowKeyValue = null;
                    });

                },
                'formready': function () {
                    var me = this;
                    me.kzdmForm.down('toolbar').insert(0, [{
                        xtype: 'button',
                        text: '新建',
                        handler: function () {
                            me.kzdmForm.getForm().reset();
                            me.kzdmForm.getForm().findField("RB_CD").setValue(me.hddmdm);
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
                                    tableName: 'wr_csb_b',
                                    rowKey: 'RB_CD',
                                    rowKeyValue: me.hddmdm
                                }]
                            };
                        },
                        deleteSuccess: function () {
                            me.kzdmForm.getForm().reset();
                            me.kzdmForm.getForm().getFields().each(function (field) {
                                field.rowKeyValue = null;
                            });
                            me.grid.getStore().reload();
                        }
                    }]);
                },
                scope: this
            }
        });

        var jcdmForm = this.jcdmForm = Ext.create('bitc.report.FormView',{
            formToken: 'jcdmForm',
            title: '水功能区监测断面',
            fieldConfig : {
                US_WFC_NM: {
                    setValueFun: function (record) {
                        this.jcdmForm.getForm().findField('US_WFC_CD').setValue(record.get('水功能区代码'));
                        this.jcdmForm.getForm().findField('US_WFC_NM').setValue(record.get('水功能区名称'));
                    },
                    clearValueFun: function () {
                        this.jcdmForm.getForm().findField('US_WFC_CD').setValue(null);
                        this.jcdmForm.getForm().findField('US_WFC_NM').setValue(null);
                    },
                    winConfig: {
                        "title": "上游水功能区"
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
                            text: "可通过水功能区代码或者水功能区名称查询"
                        },
                        inklings: [
                            {
                                name: "水功能区代码",
                                op: "like"
                            },
                            {
                                name: "水功能区名称",
                                op: "like"
                            }
                        ]
                    },
                    scope: this
                },
                DOWN_WFC_NM: {
                    setValueFun: function (record) {
                        this.jcdmForm.getForm().findField('DOWN_WFC_CD').setValue(record.get('水功能区代码'));
                        this.jcdmForm.getForm().findField('DOWN_WFC_NM').setValue(record.get('水功能区名称'));
                    },
                    clearValueFun: function () {
                        this.jcdmForm.getForm().findField('DOWN_WFC_CD').setValue(null);
                        this.jcdmForm.getForm().findField('DOWN_WFC_NM').setValue(null);
                    },
                    winConfig: {
                        "title": "下游水功能区"
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
                                name: "水功能区代码",
                                op: "like"
                            },
                            {
                                name: "水功能区名称",
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
                    this.jcdmForm.getForm().reset();
                    this.jcdmForm.getForm().getFields().each(function (field) {
                        field.rowKeyValue = null;
                    });
                },
                'formready': function () {
                    var me = this;
                    me.jcdmForm.down('toolbar').insert(0, [{
                        xtype: 'button',
                        text: '新建',
                        handler: function () {
                            me.jcdmForm.getForm().reset();
                            me.jcdmForm.getForm().findField("RB_CD").setValue(me.hddmdm);
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
                                    tableName: 'wr_wfzb_b',
                                    rowKey: 'RB_CD',
                                    rowKeyValue: me.hddmdm
                                }]
                            };
                        },
                        deleteSuccess: function () {
                            me.jcdmForm.getForm().reset();
                            me.jcdmForm.getForm().getFields().each(function (field) {
                                field.rowKeyValue = null;
                            });
                            me.grid.getStore().reload();
                        }
                    }]);
                },
                scope: this
            }
        });

        var xzqjdmForm = this.xzqjdmForm = Ext.create('bitc.report.FormView',{
            formToken: 'xzqjdmForm',
            title: '行政区界断面',
            fieldConfig : {
                US_AD_NM: {
                    setValueFun: function (record) {
                        this.xzqjdmForm.getForm().findField('US_AD_CD').setValue(record.get('行政区划代码'));
                        this.xzqjdmForm.getForm().findField('US_AD_NM').setValue(record.get('行政区划名称'));
                    },
                    clearValueFun: function () {
                        this.xzqjdmForm.getForm().findField('US_AD_CD').setValue(null);
                        this.xzqjdmForm.getForm().findField('US_AD_NM').setValue(null);
                    },
                    winConfig: {
                        "title": "上游行政区"
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
                            text: "可通过行政区划代码或者行政区划名称查询"
                        },
                        inklings: [
                            {
                                name: "行政区划代码",
                                op: "like"
                            },
                            {
                                name: "行政区划名称",
                                op: "like"
                            }
                        ]
                    },
                    scope: this
                },
                DOWN_AD_AM: {
                    setValueFun: function (record) {
                        this.xzqjdmForm.getForm().findField('DOWN_AD_CD').setValue(record.get('行政区划代码'));
                        this.xzqjdmForm.getForm().findField('DOWN_AD_AM').setValue(record.get('行政区划名称'));
                    },
                    clearValueFun: function () {
                        this.xzqjdmForm.getForm().findField('DOWN_AD_CD').setValue(null);
                        this.xzqjdmForm.getForm().findField('DOWN_AD_AM').setValue(null);
                    },
                    winConfig: {
                        "title": "上游行政区"
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
                            text: "可通过行政区划代码或者行政区划名称查询"
                        },
                        inklings: [
                            {
                                name: "行政区划代码",
                                op: "like"
                            },
                            {
                                name: "行政区划名称",
                                op: "like"
                            }
                        ]
                    },
                    scope: this
                },
                SUP_ORG_NM: {
                    setValueFun: function (record) {
                        this.xzqjdmForm.getForm().findField('SUP_ORG_CD').setValue(record.get('ORG_CD'));
                        this.xzqjdmForm.getForm().findField('SUP_ORG_NM').setValue(record.get('ORG_NM'));
                    },
                    clearValueFun: function () {
                        this.xzqjdmForm.getForm().findField('SUP_ORG_CD').setValue(null);
                        this.xzqjdmForm.getForm().findField('SUP_ORG_NM').setValue(null);
                    },
                    winConfig: {
                        "title": "上级主管单位"
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
                    this.xzqjdmForm.getForm().reset();
                    this.xzqjdmForm.getForm().getFields().each(function (field) {
                        field.rowKeyValue = null;
                    });
                },
                'formready': function () {
                    var me = this;
                    me.xzqjdmForm.down('toolbar').insert(0, [{
                        xtype: 'button',
                        text: '新建',
                        handler: function () {
                            me.xzqjdmForm.getForm().reset();
                            me.xzqjdmForm.getForm().findField("RB_CD").setValue(me.hddmdm);
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
                                    tableName: 'wr_ab_b',
                                    rowKey: 'RB_CD',
                                    rowKeyValue: me.hddmdm
                                }]
                            };
                        },
                        deleteSuccess: function () {
                            me.xzqjdmForm.getForm().reset();
                            me.xzqjdmForm.getForm().getFields().each(function (field) {
                                field.rowKeyValue = null;
                            });
                            me.grid.getStore().reload();
                        }
                    }]);
                },
                scope: this
            }
        });

        var hddmSwcz = this.hddmSwcz = Ext.create('bitc.report.ReportView',{
            reportToken: 'hddmSwcz',
            title : "水文测站",
            reportCondition: "1=2",
            gridConfig: {
                groupField: '测站类别ID',
                features: [{
                    ftype: 'grouping',
                    groupHeaderTpl: ['{rows:this.getName} —— {rows.length}项', {
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

        var superMap = this.superMap = Ext.create('bitc.report.withSuperMap');

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
            items: [form,kzdmForm,jcdmForm,xzqjdmForm,hddmSwcz,superMap]
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
                    text: '可通过断面代码或者断面名称查询'
                },
                inklings: [{
                    name: '断面代码',
                    op: 'like'
                }, {
                    name: '断面名称',
                    op: 'like'
                }]

            }),{
                text:'显示所有',
                hidden:me.hddmId == null?true:false,
                handler:function(){
                    me.gridview.reportCondition=null;
                    me.gridview.reload();
                    this.hide();
                }
            }]
        );
        grid.on('itemdblclick', function (g, record) {
            var hddmdm = this.hddmdm = record.data.断面代码;
            var hddmId = this.hddmId = record.data.断面_ID;
            //基本信息
            Ext.Ajax.request({
                url: __path + "/mgmb/getDmCommonMsg.do",
                params: {
                    dbName: '_self',
                    hddmId: hddmId
                },
                method: 'post',
                success: function (response) {
                    var result = Ext.decode(response.responseText);
                    var record = result.attributes.list[0];
                    this.link(record);
                    this.superMap.reLocation(record.断面_ID,record.断面名称,'',record.左岸经纬度坐标);
                },
                scope: this
            });
            //控制断面
            me.kzdmForm.getForm().reset();
            me.kzdmForm.getForm().findField("RB_CD").setValue(hddmdm);
            Ext.Ajax.request({
                url: __path + "/mgmb/getDmController.do",
                params: {
                    dbName: '_self',
                    hddmdm:hddmdm
                },
                method: 'post',
                success: function (response) {
                    var result = Ext.decode(response.responseText);
                    var controllerRecord = result.attributes.controllerList[0];
                    if(controllerRecord==undefined){
                        return;
                    }
                    this.linkController(controllerRecord);
                },
                scope: this
            });
            //水功能区监测断面
            me.jcdmForm.getForm().reset();
            me.jcdmForm.getForm().findField("RB_CD").setValue(hddmdm);
            Ext.Ajax.request({
                url: __path + "/mgmb/getDmMonitor.do",
                params: {
                    dbName: '_self',
                    hddmdm:hddmdm
                },
                method: 'post',
                success: function (response) {
                    var result = Ext.decode(response.responseText);
                    var monitorRecord = result.attributes.monitorList[0];
                    if(monitorRecord==undefined){
                        return;
                    }
                    this.linkMonitor(monitorRecord);
                },
                scope: this
            });
            //行政区界断面
            me.xzqjdmForm.getForm().reset();
            me.xzqjdmForm.getForm().findField("RB_CD").setValue(hddmdm);
            Ext.Ajax.request({
                url: __path + "/mgmb/getDmAdmin.do",
                params: {
                    dbName: '_self',
                    hddmdm:hddmdm
                },
                method: 'post',
                success: function (response) {
                    var result = Ext.decode(response.responseText);
                    var adminRecord = result.attributes.adminList[0];
                    if(adminRecord==undefined){
                        return;
                    }
                    this.linkAdmin(adminRecord);
                },
                scope: this
            });

        }, this);
        //if (this.qyshId) {
        //    this.gridview.grid.getStore().on('load', function (store, records) {
        //        var record = records[0];
        //        this.gridview.grid.getSelectionModel().select(0);
        //        this.link(record);
        //    }, this);
        //}
    },
    link : function(record){
        this.form.setValues([{
            name: '河道断面_ID',
            value:record.断面_ID,
            rowKeyValue: record.断面_ID
        },{
            name: 'RB_CD',
            value:record.断面代码,
            rowKeyValue: record.断面代码
        }, {
            name: 'RB_NM',
            value: record.断面名称,
            rowKeyValue: record.断面代码
        }, {
            name: 'LOC',
            value: record.所在地,
            rowKeyValue: record.断面代码
        }, {
            name: '水功能区_ID',
            value: record.水功能区ID,
            rowKeyValue: record.断面_ID
        }, {
            name: '水功能区_NM',
            value: record.水功能区名称,
            rowKeyValue: record.断面_ID
        }, {
            name: '水体_ID',
            value: record.水体ID,
            rowKeyValue: record.断面_ID
        }, {
            name: '水体_NM',
            value: record.水体名称,
            rowKeyValue: record.断面_ID
        }, {
            name: '经纬度坐标',
            value: record.左岸经纬度坐标,
            rowKeyValue: record.断面_ID
        },{
            name: '右岸坐标',
            value: record.右岸经纬度坐标,
            rowKeyValue: record.断面_ID
        }, {
            name: 'NT',
            value: record.备注,
            rowKeyValue: record.断面代码
        },  {
            name: 'TS',
            value: null,
            rowKeyValue: record.断面代码
        }, {
            name: 'wr_sd_code',
            value: record.断面代码,
            rowKeyValue: record.断面_ID
        }]);
        this.hddmSwcz.reload("河道断面_ID='" + record.断面_ID + "'");
    },
    linkController : function(controllerRecord){
        this.kzdmForm.setValues([{
            name: 'RB_CD',
            value:controllerRecord.RB_CD,
            rowKeyValue: controllerRecord.RB_CD
        },{
            name: 'ST_DT',
            value:Ext.util.Format.date(controllerRecord.设置日期, 'Y-m-d'),
            rowKeyValue: controllerRecord.RB_CD
        },{
            name: 'MON_CONT',
            value:controllerRecord.监测内容,
            rowKeyValue: controllerRecord.RB_CD
        },{
            name: 'CONT_MIN_Z',
            value:controllerRecord.控制最低水位,
            rowKeyValue: controllerRecord.RB_CD
        },{
            name: 'CONT_MIN_Q',
            value:controllerRecord.控制最小流量,
            rowKeyValue: controllerRecord.RB_CD
        },{
            name: 'CONT_QUNT',
            value:controllerRecord.控制水质要求,
            rowKeyValue: controllerRecord.RB_CD
        },{
            name: 'NT',
            value:controllerRecord.备注,
            rowKeyValue: controllerRecord.RB_CD
        },{
            name: 'TS',
            value:null,
            rowKeyValue: controllerRecord.RB_CD
        }]);
    },
    linkMonitor : function(monitorRecord){
        this.jcdmForm.setValues([{
            name: 'RB_CD',
            value:monitorRecord.RB_CD,
            rowKeyValue: monitorRecord.RB_CD
        },{
            name: 'ST_DT',
            value:Ext.util.Format.date(monitorRecord.设置日期, 'Y-m-d'),
            rowKeyValue: monitorRecord.RB_CD
        },{
            name: 'MON_CONT',
            value:monitorRecord.监测内容,
            rowKeyValue: monitorRecord.RB_CD
        },{
            name: 'US_WFC_CD',
            value:monitorRecord.上游水功能区代码,
            rowKeyValue: monitorRecord.RB_CD
        },{
            name: 'US_WFC_NM',
            value:monitorRecord.上游水功能区名称,
            rowKeyValue: monitorRecord.RB_CD
        },{
            name: 'DOWN_WFC_CD',
            value:monitorRecord.下游水功能区代码,
            rowKeyValue: monitorRecord.RB_CD
        },{
            name: 'DOWN_WFC_NM',
            value:monitorRecord.下游水功能区名称,
            rowKeyValue: monitorRecord.RB_CD
        },{
            name: 'TS',
            value:null,
            rowKeyValue: monitorRecord.RB_CD
        },{
            name: 'NT',
            value:monitorRecord.备注,
            rowKeyValue: monitorRecord.RB_CD
        }]);
    },
    linkAdmin : function(adminRecord){
        this.xzqjdmForm.setValues([{
            name: 'RB_CD',
            value:adminRecord.RB_CD,
            rowKeyValue: adminRecord.RB_CD
        },{
            name: 'AB_G',
            value:adminRecord.行政区界断面级别代码,
            rowKeyValue: adminRecord.RB_CD
        },{
            name: 'US_AD_CD',
            value:adminRecord.上游行政区划代码,
            rowKeyValue: adminRecord.RB_CD
        },{
            name: 'US_AD_NM',
            value:adminRecord.上游行政区划名称,
            rowKeyValue: adminRecord.RB_CD
        },{
            name: 'DOWN_AD_CD',
            value:adminRecord.下游行政区划代码,
            rowKeyValue: adminRecord.RB_CD
        },{
            name: 'DOWN_AD_AM',
            value:adminRecord.下游行政区划名称,
            rowKeyValue: adminRecord.RB_CD
        },{
            name: 'SUP_ORG_CD',
            value:adminRecord.上级主管单位代码,
            rowKeyValue: adminRecord.RB_CD
        },{
            name: 'SUP_ORG_NM',
            value:adminRecord.上级主管单位名称,
            rowKeyValue: adminRecord.RB_CD
        },{
            name: 'FROM_ASS_DT',
            value:Ext.util.Format.date(adminRecord.开始考核日期, 'Y-m-d'),
            rowKeyValue: adminRecord.RB_CD
        },{
            name: 'MAJ_AI',
            value:adminRecord.主要考核指标,
            rowKeyValue: adminRecord.RB_CD
        },{
            name: 'TS',
            value:null,
            rowKeyValue: adminRecord.RB_CD
        },{
            name: 'NT',
            value:adminRecord.备注,
            rowKeyValue: adminRecord.RB_CD
        }]);
    }
});