Ext.define('bitc.wrenv.mgmb.shuigongnengqu', {
    extend: 'Ext.panel.Panel',
    requires: ['Ext.grid.feature.Grouping','Ext.ux.InklingSearchField','bitc.report.widget.ChooseField','bitc.report.widget.CombWidget'],
    layout: 'border',
    sgnqId : null,
    initComponent: function () {
        var currentValue = null;
        var gridview = this.gridview = Ext.create('bitc.report.ReportView',{
            reportToken: 'shuigongnengquReport',
            //reportCondition: this.qysczId ? "取用水户_ID='" + this.qysczId + "'" : null,
            region: 'center',
            columnConfig : {
                '水功能区面积' : {
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
            formToken: 'shuigongnengquForm',
            title: '基本信息',
            fieldConfig: {
                '一级水功能区_ID':{
                    xtype:'combobox',
                    anchor: '90%',
                    valueField: 'valueColumn',
                    displayField: 'displayColumn',
                    editable: false,
                    triggerAction: "all",
                    msgTarget: "side",
                    store:Ext.create('Ext.data.Store',{
                        proxy: {
                            type: 'ajax',
                            url: __path + '/form/combo.do',
                            reader: {
                                type: 'json',
                                root: 'rows',
                                idProperty: 'valueColumn',
                                totalProperty: 'total'
                            },
                            extraParams: {
                                dbName: '_self',
                                valueColumn: 'SN_ID',
                                displayColumn: '等级名称',
                                tableName: 'mgmb_sys_水功能区分级表',
                                condition: 'PARENT_SN_ID is null order by 同级排序_ID'
                            }
                        },
                        autoLoad: true,
                        fields: ["valueColumn", "displayColumn"]
                    }),
                    listeners : {
                        'change' : function (comb,newValue,oldValue) {
                            form.getForm().findField('二级水功能区_ID').clearValue();
                            form.getForm().findField('二级水功能区_ID').getStore().proxy.extraParams['condition']='PARENT_SN_ID=\''+newValue+'\' order by 同级排序_ID';
                            form.getForm().findField('二级水功能区_ID').getStore().load();
                        },
                        scope : this
                    },
                    scope : this
                },
                '二级水功能区_ID':{
                    xtype:'combobox',
                    anchor: '90%',
                    valueField: 'valueColumn',
                    displayField: 'displayColumn',
                    editable: false,
                    triggerAction: "all",
                    msgTarget: "side",
                    store:Ext.create('Ext.data.Store',{
                        proxy: {
                            type: 'ajax',
                            url: __path + '/form/combo.do',
                            reader: {
                                type: 'json',
                                root: 'rows',
                                idProperty: 'valueColumn',
                                totalProperty: 'total'
                            },
                            extraParams: {
                                dbName: '_self',
                                valueColumn: 'SN_ID',
                                displayColumn: '等级名称',
                                tableName: 'mgmb_sys_水功能区分级表',
                                condition: '1 = 2'
                            }
                        },
                        autoLoad: false,
                        fields: ["valueColumn", "displayColumn"]
                    }),
                    scope : this
                },
                WFZ_STAR_NM: {
                    setValueFun: function (record) {
                        this.form.getForm().findField('WFZ_STAR').setValue(record.get('断面代码'));
                        this.form.getForm().findField('WFZ_STAR_NM').setValue(record.get('断面名称'));
                    },
                    clearValueFun: function () {
                        this.form.getForm().findField('WFZ_STAR').setValue(null);
                        this.form.getForm().findField('WFZ_STAR_NM').setValue(null);
                    },
                    winConfig: {
                        "title": "水功能区断面"
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
                            text: "可通过断面名称查询"
                        },
                        inklings: [
                            {
                                name: "断面名称",
                                op: "like"
                            }
                        ]
                    },
                    scope: this
                },
                控制断面_NM: {
                    setValueFun: function (record) {
                        this.form.getForm().findField('控制断面_CD').setValue(record.get('断面代码'));
                        this.form.getForm().findField('控制断面_NM').setValue(record.get('断面名称'));
                    },
                    clearValueFun: function () {
                        this.form.getForm().findField('控制断面_CD').setValue(null);
                        this.form.getForm().findField('控制断面_NM').setValue(null);
                    },
                    winConfig: {
                        "title": "控制断面"
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
                            text: "可通过断面名称查询"
                        },
                        inklings: [
                            {
                                name: "断面名称",
                                op: "like"
                            }
                        ]
                    },
                    scope: this
                },
                省界控制断面_NM: {
                    setValueFun: function (record) {
                        this.form.getForm().findField('省界控制断面_CD').setValue(record.get('断面代码'));
                        this.form.getForm().findField('省界控制断面_NM').setValue(record.get('断面名称'));
                    },
                    clearValueFun: function () {
                        this.form.getForm().findField('省界控制断面_CD').setValue(null);
                        this.form.getForm().findField('省界控制断面_NM').setValue(null);
                    },
                    winConfig: {
                        "title": "省界控制断面"
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
                            text: "可通过断面名称查询"
                        },
                        inklings: [
                            {
                                name: "断面名称",
                                op: "like"
                            }
                        ]
                    },
                    scope: this
                },
                WFZ_END_NM: {
                    setValueFun: function (record) {
                        this.form.getForm().findField('WFZ_END').setValue(record.get('断面代码'));
                        this.form.getForm().findField('WFZ_END_NM').setValue(record.get('断面名称'));
                    },
                    clearValueFun: function () {
                        this.form.getForm().findField('WFZ_END').setValue(null);
                        this.form.getForm().findField('WFZ_END_NM').setValue(null);
                    },
                    winConfig: {
                        "title": "水功能区断面"
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
                            text: "可通过断面名称查询"
                        },
                        inklings: [
                            {
                                name: "断面名称",
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
                },
                上游行政区划名称: {
                    setValueFun: function (record) {
                        this.form.getForm().findField('上游行政区_ID').setValue(record.get('行政区_ID'));
                        this.form.getForm().findField('上游行政区划名称').setValue(record.get('行政区划名称'));
                    },
                    clearValueFun: function () {
                        this.form.getForm().findField('上游行政区_ID').setValue(null);
                        this.form.getForm().findField('上游行政区划名称').setValue(null);
                    },
                    winConfig: {
                        "title": "行政区划名称"
                    },
                    gridConfig: {
                        groupField: '行政区级别_ID',
                        features: [{
                            ftype: 'grouping',
                            groupHeaderTpl: ['{rows:this.getName}', {
                                getName: function (rows) {
                                    return rows[0].get('行政区级别名称');
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
                            text: "可通过行政区代码或行政区划名称查询"
                        },
                        inklings: [
                            {
                                name: "WR_SD_CODE",
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
                下游行政区划名称: {
                    setValueFun: function (record) {
                        this.form.getForm().findField('下游行政区_ID').setValue(record.get('行政区_ID'));
                        this.form.getForm().findField('下游行政区划名称').setValue(record.get('行政区划名称'));
                    },
                    clearValueFun: function () {
                        this.form.getForm().findField('下游行政区_ID').setValue(null);
                        this.form.getForm().findField('下游行政区划名称').setValue(null);
                    },
                    winConfig: {
                        "title": "行政区划名称"
                    },
                    gridConfig: {
                        groupField: '行政区级别_ID',
                        features: [{
                            ftype: 'grouping',
                            groupHeaderTpl: ['{rows:this.getName}', {
                                getName: function (rows) {
                                    return rows[0].get('行政区级别名称');
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
                            text: "可通过行政区代码或行政区划名称查询"
                        },
                        inklings: [
                            {
                                name: "WR_SD_CODE",
                                op: "like"
                            },
                            {
                                name: "行政区划名称",
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
                                    tableName: 'wr_wfz_b',
                                    rowKey: 'WFZ_CD',
                                    rowKeyValue: me.sgnqDm
                                }, {
                                    tableName: 'mgmb_水功能区信息简表',
                                    rowKey: '水功能区_ID',
                                    rowKeyValue: me.sgnqId
                                },{
                                    tableName: 'mgmb_rel_水功能区与水体',
                                    rowKey: '水功能区_ID',
                                    rowKeyValue: me.sgnqId
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
        var sgnqSwczGrid = this.sgnqSwczGrid = Ext.create('bitc.report.ReportView',{
            reportToken: 'sgnqSwczGrid',
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

        var sgnqDbsGrid = this.sgnqDbsGrid = Ext.create('bitc.report.ReportView',{
            reportToken: 'sgnqDbsGrid',
            title: '地表水源地',
            gridConfig: {
                selModel: {
                    selType: 'checkboxmodel',
                    mode: 'SIMPLE'
                }
            },
            columnConfig : {
                '水面面积':{
                    renderer : function(value,meta,record){
                        meta.align = "right";
                        return value;
                    }
                },
                '地表水水源地类型名称':{
                    renderer : function(value,meta,record){
                        meta.column.width = 150;
                        return value;
                    }
                },
                '水源供水持续状况名称' : {
                    renderer : function(value,meta,record){
                        meta.column.width = 150;
                        return value;
                    }
                },
                '供水对象名称':{
                    renderer : function(value,meta,record){
                        meta.column.width = 100;
                        return value;
                    }
                }
            },
            listeners: {
                gridready: this.initSgnqDbsGrid,
                scope: this
            },
            rowContextMenu: [{
                text: '详细',
                iconCls: '',
                handler: function (widget,record) {
                    var data={
                        id: 'dibiaoshui',
                        text: '地表水信息数据库',
                        action: 'bitc.wrenv.mgmb.dibiaoshui',
                        leaf: true,
                        iconCls: '',
                        config: {
                            sydId: record.get('敏感目标_ID')
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
            collapseMode: 'mini',
            width: 600,
            frame: true,
            region: 'east',
            items: [form,superMap,sgnqSwczGrid,sgnqDbsGrid]
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
                    text: '可通过水功能区代码或者水功能区名称查询'
                },
                inklings: [{
                    name: '水功能区代码',
                    op: 'like'
                }, {
                    name: '水功能区名称',
                    op: 'like'
                }]

            }),{
                text:'显示所有',
                hidden:me.sgnqId == null?true:false,
                handler:function(){
                    me.gridview.reportCondition=null;
                    me.gridview.reload();
                    this.hide();
                }
            }]
        );
        grid.on('itemdblclick', function (g, record) {
            this.link(record);
            this.superMap.reLocation(record.get('水功能区_ID'),record.get('水功能区名称'),record.get(''),record.get('经纬度坐标'));
        }, this);
        //if (this.sgnqId) {
        //    this.gridview.grid.getStore().on('load', function (store, records) {
        //        var record = records[0];
        //        this.gridview.grid.getSelectionModel().select(0);
        //        this.link(record);
        //    }, this);
        //}
    },
    link : function(record){
        this.form.setValues([{
            name: '水功能区_ID',
            value:record.get('水功能区_ID'),
            rowKeyValue: record.get('水功能区_ID')
        },{
            name: 'WFZ_CD',
            value:record.get('水功能区代码'),
            rowKeyValue: record.get('水功能区代码')
        },{
            name: 'WFZ_NM',
            value:record.get('水功能区名称'),
            rowKeyValue: record.get('水功能区代码')
        },{
            name: '水功能区等级_ID',
            value:record.get('等级ID'),
            rowKeyValue: record.get('水功能区_ID')
        },{
            name: 'WFZ_FUN_SEQ',
            value:record.get('水功能区功能排序'),
            rowKeyValue: record.get('水功能区代码')
        },{
            name: '一级水功能区_ID',
            value: record.get('一级水功能区ID'),
            rowKeyValue: record.get('水功能区_ID')
        },{
            name: '二级水功能区_ID',
            value: record.get('二级水功能区ID'),
            rowKeyValue: record.get('水功能区_ID')
        },{
            name: 'WFZ_STAR',
            value: record.get('起始断面ID'),
            rowKeyValue: record.get('水功能区代码')
        },{
            name: 'WFZ_STAR_NM',
            value: record.get('起始断面名称'),
            rowKeyValue: record.get('水功能区代码')
        },{
            name: '控制断面_CD',
            value: record.get('控制断面ID'),
            rowKeyValue: record.get('水功能区_ID')
        },{
            name: '控制断面_NM',
            value: record.get('控制断面名称'),
            rowKeyValue: record.get('水功能区_ID')
        }, {
            name: '省界控制断面_CD',
            value: record.get('省界控制断面ID'),
            rowKeyValue: record.get('水功能区_ID')
        },{
            name: '省界控制断面_NM',
            value: record.get('省界控制断面名称'),
            rowKeyValue: record.get('水功能区_ID')
        },{
            name: 'WFZ_END',
            value: record.get('终止断面ID'),
            rowKeyValue: record.get('水功能区代码')
        }, {
            name: 'WFZ_END_NM',
            value: record.get('终止断面名称'),
            rowKeyValue: record.get('水功能区代码')
        }, {
            name: 'WFZ_A',
            value: record.get('水功能区面积'),
            rowKeyValue: record.get('水功能区代码')
        }, {
            name: 'WFZ_LEN',
            value: record.get('水功能区长度'),
            rowKeyValue: record.get('水功能区代码')
        }, {
            name: 'WFZ_LWL',
            value: record.get('水功能区最低水位'),
            rowKeyValue: record.get('水功能区代码')
        }, {
            name: 'WFZ_WQT',
            value: record.get('水质目标ID'),
            rowKeyValue: record.get('水功能区代码')
        },{
            name: 'MON_G',
            value: record.get('监控级别ID'),
            rowKeyValue: record.get('水功能区代码')
        }, {
            name: '经纬度坐标',
            value: record.get('经纬度坐标'),
            rowKeyValue: record.get('水功能区_ID')
        }, {
            name: 'NT',
            value: record.get('备注'),
            rowKeyValue: record.get('水功能区代码')
        }, {
            name: '水体_ID',
            value: record.get('水体ID'),
            rowKeyValue: record.get('水功能区_ID')
        },{
            name: '水体_NM',
            value: record.get('水体名称'),
            rowKeyValue: record.get('水功能区_ID')
        },{
            name: 'ENG_MAN_CD',
            value: record.get('管理单位代码'),
            rowKeyValue: record.get('水功能区代码')
        },{
            name: 'TS',
            value: null,
            rowKeyValue:record.get('水功能区代码')
        },{
            name: 'ENG_MAN_NM',
            value: record.get('管理单位名称'),
            rowKeyValue: record.get('水功能区代码')
        }, {
            name: 'wr_sd_code',
            value: record.get('水功能区代码'),
            rowKeyValue: record.get('水功能区_ID')
        },{
            name: '上游行政区_ID',
            value: record.get('上游行政区_ID'),
            rowKeyValue: record.get('水功能区_ID')
        },{
            name: '上游行政区划名称',
            value: record.get('上游行政区划名称'),
            rowKeyValue: record.get('水功能区_ID')
        },{
            name: '下游行政区_ID',
            value: record.get('下游行政区_ID'),
            rowKeyValue: record.get('水功能区_ID')
        },{
            name: '下游行政区划名称',
            value: record.get('下游行政区划名称'),
            rowKeyValue: record.get('水功能区_ID')
        },{
            name: '上游长度',
            value: record.get('上游长度'),
            rowKeyValue: record.get('水功能区_ID')
        },{
            name: '下游长度',
            value: record.get('下游长度'),
            rowKeyValue: record.get('水功能区_ID')
        }]);
        var sgnqId = this.sgnqId = record.get("水功能区_ID");
        var sgnqDm = this.sgnqDm = record.get("水功能区代码");
        this.sgnqSwczGrid.reload("水功能区_ID='" + record.get('水功能区_ID') + "'");
        Ext.Ajax.request({
            url: __path + "/mgmb/getSgnqSyd.do",
            params: {
                dbName: '_self',
                sgnqId: sgnqId
            },
            method: 'post',
            success: function (response) {
                var result = Ext.decode(response.responseText);
                var records = result.rows;
                this.selectSgnqSydRecords(records);
            },
            scope: this
        });
    },
    selectSgnqSydRecords: function (records) {
        var grid = this.sgnqDbsGrid.grid;
        grid.selectRecords(function (record) {
            return records.indexOf(record.get('敏感目标_ID')) >= 0;
        });
    },
    initSgnqDbsGrid : function(panel, grid){
        grid.down('toolbar').insert(0, {
            text: '保存',
            scope: this,
            handler: function () {
                var records = grid.getSelectionModel().getSelection();
                if (records.length == 0) {
                    Ext.MessageBox.alert("提示", "请先选中您要操作的行!");
                    return;
                } else if (this.sgnqId == null) {
                    Ext.MessageBox.alert("提示", "请先选择相关联的水功能区!");
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
                        url: __path + "/mgmb/saveSgnqSyd.do",
                        params: {
                            sgnqId: this.sgnqId,
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
    }
});