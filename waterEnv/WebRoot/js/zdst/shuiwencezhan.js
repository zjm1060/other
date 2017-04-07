Ext.define('bitc.wrenv.zdst.shuiwencezhan', {
    extend: 'Ext.panel.Panel',
    requires: ['Ext.grid.feature.Grouping', 'Ext.ux.InklingSearchField', 'bitc.report.widget.ChooseField'],
    layout: 'border',
    swczId: null,
    scxmId: null,
    czType:null,
    initComponent: function () {
        var gridview = this.gridview = Ext.create('bitc.report.ReportView', {
            reportToken: 'swczxxGrid',
            reportCondition: this.swczId ? "水文测站_ID='" + this.swczId + "'" : ("测站类别id='"+this.czType+"'"),
            region: 'center',
            //gridConfig: {
            //    groupField: '测站类别ID',
            //    features: [{
            //        ftype: 'grouping',
            //        groupHeaderTpl: ['{rows:this.getName}', {
            //            getName: function (rows) {
            //                return rows[0].get('测站类别名称');
            //            }
            //        }],
            //        hideGroupedHeader: true,
            //        startCollapsed: false
            //    }]
            //},
            listeners: {
                gridready: this.initGrid,
                scope: this
            }
        });
        //排污口
        var pwGrid = this.pwGrid = Ext.create('bitc.report.ReportView', {
            reportToken: 'paiwukouWithSwczGrid',
            title: '排污口',
            columnConfig : {
                '设计日排污能力' : {
                    renderer : function(value,meta,record){
                        meta.align = "right";
                        return value;
                    }
                }
            },
            gridConfig: {
                selModel: {
                    selType: 'checkboxmodel',
                    mode: 'SIMPLE'
                }
            },
            rowContextMenu: [{
                text: '详细',
                iconCls: '',
                handler: function (widget,record, event) {
                    var data={
                        id: record.get('排污口_ID'),
                        text: record.get('入河排污口名称')+'详细信息',
                        action: 'bitc.wrenv.mgmb.ruhepaiwukou',
                        leaf: true,
                        iconCls: '',
                        config : {
                            rhpwkId: record.get('排污口_ID')
                        }
                    };
                    addWorkTab(data);
                },
                scope: this
            }],
            listeners: {
                gridready: this.initPwGrid,
                scope: this
            }
        });
        var shuiyuandiType = {
            '地表水':'bitc.wrenv.mgmb.dibiaoshui',
            '地下水':'bitc.wrenv.mgmb.dixiashui'
        };
        //水源地
        var sydGrid = this.sydGrid = Ext.create('bitc.report.ReportView', {
            reportToken: 'shuiyuandiWithswczGrid',
            title: '水源地信息',
            defaultGridWidth : false,
            gridConfig: {
                groupField:'敏感目标类型_ID',
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
                    var data={
                        id: record.get('敏感目标类型_ID'),
                        text: record.get('敏感目标类型名称')+'详细信息',
                        action: shuiyuandiType[record.get('敏感目标类型名称')],
                        leaf: true,
                        iconCls: '',
                        config : {
                            sydId: record.get('敏感目标类型_ID')
                        }
                    };
                    addWorkTab(data);
                },
                scope: this
            }],
            listeners: {
                gridready: this.initsydGrid,
                scope: this
            }
        });
        //河道断面
        var hddmGrid = this.hddmGrid = Ext.create('bitc.report.ReportView', {
            reportToken: 'hedaoduanmianWithswcz',
            title: '河道断面信息',
            gridConfig: {
                selModel: {
                    selType: 'checkboxmodel',
                    mode: 'SIMPLE'
                }
            },
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
            rowContextMenu: [{
                text: '详细',
                iconCls: '',
                handler: function (widget,record, event) {
                    var data={
                        id: record.get('断面_ID'),
                        text: record.get('断面名称')+'详细信息',
                        action: 'bitc.wrenv.mgmb.hedaoduanmian',
                        leaf: true,
                        iconCls: '',
                        config : {
                            hddmId: record.get('断面_ID')
                        }
                    };
                    addWorkTab(data);
                },
                scope: this
            }],
            listeners: {
                gridready: this.inithddmGrid,
                scope: this
            }
        });
        //水功能区
        var sgnqGrid = this.sgnqGrid = Ext.create('bitc.report.ReportView', {
            reportToken: 'shuigongnengquWithswczGrid',
            title: '水功能区信息',
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
                handler: function (widget,record, event) {
                    var data={
                        id: record.get('水功能区_ID'),
                        text: record.get('水功能区名称')+'详细信息',
                        action: 'bitc.wrenv.mgmb.shuigongnengqu',
                        leaf: true,
                        iconCls: '',
                        config : {
                            sgnqId: record.get('水功能区_ID')
                        }
                    };
                    addWorkTab(data);
                },
                scope: this
            }],
            listeners: {
                gridready: this.initsgnqGrid,
                scope: this
            }
        });
        //服务水体表
        var stGrid = this.stGrid = Ext.create('bitc.report.ReportView', {
            reportToken: 'fuwushuitiGrid',
            title: '监测水体',
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
                    addWorkTab(data);
                },
                scope: this
            }],
            listeners: {
                gridready: this.initStGrid,
                scope: this
            }
        });
        var stList = {
            '河流':'bitc.wrenv.zdst.heliu',
            '湖泊':'bitc.wrenv.zdst.hupo',
            '水库':'bitc.wrenv.zdst.shuiku'
        };
        //基本信息表单
        var form = this.form = Ext.create('bitc.report.FormView', {
            formToken: 'swcexxForm',
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
                        },
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
                    this.gridview.reload();
                    this.reSetForm();
                },
                'formready': this.initForm,
                scope: this
            }
        });
        //测站施测项目信息
        var scForm = this.scForm = Ext.create('bitc.report.FormView', {
            formToken: 'swczscxmForm',
            title: '施测项目信息',
            listeners: {
                'savesuccess': function () {
                    this.gridview.reload();
                },
                'beforesubmit': function () {
                    if (!this.scxmId || this.scxmId == null) {
                        Ext.MessageBox.alert("提示", "请先选择相关联的水文测站!");
                        return false;
                    }
                    return true;
                },
                scope: this
            }
        });
        var superMap = this.superMap = Ext.create('bitc.report.withSuperMap');
        this.items = [gridview, {
             xtype: 'panel',             title:'相关信息',
            autoScroll: true,
            collapsible: true,
            deferredRender: false,
            layout:'accordion',
            //collapseMode: 'mini',
            width: 600,
            frame: true,
            region: 'east',
            items: [form, stGrid, scForm,pwGrid,sydGrid,hddmGrid,sgnqGrid,superMap]
        }];

        this.callParent(arguments);
    },
    initGrid: function (panel, grid) {
        var me = this;
        grid.down('toolbar').insert(0, [Ext.create('Ext.ux.InklingSearchField', {
                width: 180,
                emptyText: '请输入关键字查询',
                reportView: this.gridview,
                tooltip: {
                    title: '条件查询',
                    text: '可通过测站名称或者所在地查询'
                },
                inklings: [{
                    name: '测站名称',
                    op: 'like'
                }, {
                    name: '所在地',
                    op: 'like'
                }]
            }),{
                text:'显示所有',
                hidden:me.swczId == null?true:false,
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
        if (this.swczId) {
            this.gridview.grid.getStore().on('load', function (store, records) {
                var record = records[0];
                this.gridview.grid.getSelectionModel().select(0);
                this.link(record);
            }, this);
        }
    },
    initStGrid: function (panel, grid) {
        grid.down('toolbar').insert(0, {
            text: '保存',
            scope: this,
            handler: function () {
                var records = grid.getSelectionModel().getSelection();
                if (records.length == 0) {
                    Ext.MessageBox.alert("提示", "请先选中您要操作的行!");
                    return;
                } else if (this.swczId == null) {
                    Ext.MessageBox.alert("提示", "请先选择相关联的水文测站!");
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
                        url: __path + "/zdst/saveStWithgcz.do",
                        params: {
                            swczId: this.swczId,
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
    initPwGrid: function (panel, grid) {
        grid.down('toolbar').insert(0, {
            text: '保存',
            scope: this,
            handler: function () {
                var records = grid.getSelectionModel().getSelection();
                if (this.swczId == null) {
                    Ext.MessageBox.alert("提示", "请先选择相关联的水文测站!");
                    return;
                } else {
                    var pwIds = "";
                    for (var i = 0; i < records.length; i++) {
                        pwIds += records[i].get("排污口_ID")
                        if (i < records.length - 1) {
                            pwIds = pwIds + ",";
                        }
                    }
                    Ext.Ajax.request({
                        url: __path + "/zdst/savePwWithgcz.do",
                        params: {
                            swczId: this.swczId,
                            pwIds: pwIds,
                            dbName: '_self',
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
    initsydGrid: function (panel, grid) {
        grid.down('toolbar').insert(0, {
            text: '保存',
            scope: this,
            handler: function () {
                var records = grid.getSelectionModel().getSelection();
                if (this.swczId == null) {
                    Ext.MessageBox.alert("提示", "请先选择相关联的水文测站!");
                    return;
                } else {
                    var sydIds = "";
                    for (var i = 0; i < records.length; i++) {
                        sydIds += records[i].get("敏感目标_ID")
                        if (i < records.length - 1) {
                            sydIds = sydIds + ",";
                        }
                    }
                    Ext.Ajax.request({
                        url: __path + "/zdst/saveSydWithgcz.do",
                        params: {
                            swczId: this.swczId,
                            sydIds: sydIds,
                            dbName: '_self',
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
    inithddmGrid: function (panel, grid) {
        grid.down('toolbar').insert(0, [Ext.create('Ext.ux.InklingSearchField', {
            width: 180,
            emptyText: '请输入关键字查询',
            reportView: this.hddmGrid,
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
            text: '保存',
            scope: this,
            handler: function () {
                var records = grid.getSelectionModel().getSelection();
                if (this.swczId == null) {
                    Ext.MessageBox.alert("提示", "请先选择相关联的水文测站!");
                    return;
                } else {
                    var hddmIds = "";
                    for (var i = 0; i < records.length; i++) {
                        hddmIds += records[i].get("断面_ID")
                        if (i < records.length - 1) {
                            hddmIds = hddmIds + ",";
                        }
                    }
                    Ext.Ajax.request({
                        url: __path + "/zdst/saveHddmWithgcz.do",
                        params: {
                            swczId: this.swczId,
                            hddmIds: hddmIds,
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
        }]);
    },
    initsgnqGrid: function (panel, grid) {
        grid.down('toolbar').insert(0, {
            text: '保存',
            scope: this,
            handler: function () {
                var records = grid.getSelectionModel().getSelection();
                if (this.swczId == null) {
                    Ext.MessageBox.alert("提示", "请先选择相关联的水文测站!");
                    return;
                } else {
                    var sgnqIds = "";
                    for (var i = 0; i < records.length; i++) {
                        sgnqIds += records[i].get("水功能区_ID")
                        if (i < records.length - 1) {
                            sgnqIds = sgnqIds + ",";
                        }
                    }
                    Ext.Ajax.request({
                        url: __path + "/zdst/saveSgnqWithgcz.do",
                        params: {
                            swczId: this.swczId,
                            sgnqIds: sgnqIds,
                            dbName: '_self',
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
    selectStRecords: function (records) {
        var grid = this.stGrid.grid;
        grid.selectRecords(function (record) {
            return records.indexOf(record.get('水体_ID')) >= 0;
        });
    },
    selectPwRecords: function (records) {
        var grid = this.pwGrid.grid;
        grid.selectRecords(function (record) {
            return records.indexOf(record.get('排污口_ID')) >= 0;
        });
    },
    selectSydRecords: function (records) {
        var grid = this.sydGrid.grid;
        grid.selectRecords(function (record) {
            return records.indexOf(record.get('敏感目标_ID')) >= 0;
        });
    },
    selectHddmRecords: function (records) {
        var grid = this.hddmGrid.grid;
        grid.selectRecords(function (record) {
            return records.indexOf(record.get('断面_ID')) >= 0;
        });
    },
    selectSgnqRecords: function (records) {
        var grid = this.sgnqGrid.grid;
        grid.selectRecords(function (record) {
            return records.indexOf(record.get('水功能区_ID')) >= 0;
        });
    },
    initForm: function (panel) {
        var me = this;
        if(panel.getForm().findField("ST_TP")){
            panel.getForm().findField("ST_TP").setValue(this.czType);
        }
        panel.down('toolbar').insert(0, [{
            text: '新建',
            scope: this,
            handler: function () {
                this.reSetForm();
            }
        }, {
            xtype: 'deletebutton',

            beforeDelete: function () {
                if (panel.getForm().findField("STCD").value != null && panel.getForm().findField("水文测站_ID").value != null) {
                    return true;
                }
                return false;
            },
            getDeleteRows: function () {
                return {
                    dbName: '_self',
                    formFields: [{
                        tableName: 'wr_stat_b',
                        rowKey: 'STCD',
                        rowKeyValue: panel.getForm().findField("STCD").value
                    }, {
                        tableName: 'zdst_水文测站信息简表',
                        rowKey: '水文测站_ID',
                        rowKeyValue: panel.getForm().findField("水文测站_ID").value
                    }, {
                        tableName: 'zdst_rel_水体与观测站关系表',
                        rowKey: '水文测站_ID',
                        rowKeyValue: panel.getForm().findField("水文测站_ID").value
                    }]
                };
            },
            deleteSuccess: function () {
                me.form.getForm().reset();
                me.form.getForm().getFields().each(function (field) {
                    field.rowKeyValue = null;
                });
                me.gridview.reload();
            }
        }]);
    },
    reSetForm: function (record) {
        this.form.setValues([{
            name: '水文测站_ID',
            value: record ? record.get('水文测站_ID') : null,
            rowKeyValue: record ? record.get('水文测站_ID') : null
        }, {
            name: '经纬度坐标',
            value: record ? record.get('经纬度坐标') : null,
            rowKeyValue: record ? record.get('水文测站_ID') : null
        }, {
            name: 'STCD',
            value: record ? record.get('测站代码') : null,
            rowKeyValue: record ? record.get('测站代码') : null
        }, {
            name: 'ST_NM',
            value: record ? record.get('测站名称') : null,
            rowKeyValue: record ? record.get('测站代码') : null
        }, {
            name: 'ST_TP',
            value: record ? record.get('测站类别ID') : this.czType,
            rowKeyValue: record ? record.get('测站代码') : null
        }, {
            name: 'LOC',
            value: record ? record.get('所在地') : null,
            rowKeyValue: record ? record.get('测站代码') : null
        }, {
            name: 'DAT_ELEV',
            value: record ? record.get('基面高程') : null,
            rowKeyValue: record ? record.get('测站代码') : null
        }, {
            name: 'MOD_BAS_VAL',
            value: record ? record.get('修正基值') : null,
            rowKeyValue: record ? record.get('测站代码') : null
        }, {
            name: 'MOD_PARA',
            value: record ? record.get('修正参数') : null,
            rowKeyValue: record ? record.get('测站代码') : null
        }, {
            name: 'EST_ST_YM',
            value: record ? record.get('设站年月') : null,
            rowKeyValue: record ? record.get('测站代码') : null
        }, {
            name: 'ENG_MAN_CD',
            value: record ? record.get('管理单位代码') : null,
            rowKeyValue: record ? record.get('测站代码') : null
        }, {
            name: 'ENG_MAN_NM',
            value: record ? record.get('管理单位代码名称') : null,
            rowKeyValue: record ? record.get('测站代码') : null
        }, {
            name: 'PIN_YIN_CD',
            value: record ? record.get('拼音码') : null,
            rowKeyValue: record ? record.get('测站代码') : null
        }, {
            name: 'NT',
            value: record ? record.get('备注') : null,
            rowKeyValue: record ? record.get('测站代码') : null
        }, {
            name: '建站性质',
            value: record ? record.get('建站性质ID') : null,
            rowKeyValue: record ? record.get('水文测站_ID') : null
        }, {
            name: '使用性质',
            value: record ? record.get('使用性质ID') : null,
            rowKeyValue: record ? record.get('水文测站_ID') : null
        }, {
            name: 'STBK',
            value: record ? record.get('测站岸别ID') : null,
            rowKeyValue: record ? record.get('测站代码') : null
        }, {
            name: 'MON_TP_A',
            value: record ? record.get('监测方式AID') : null,
            rowKeyValue: record ? record.get('测站代码') : null
        }, {
            name: 'MON_TP_B',
            value: record ? record.get('监测方式BID') : null,
            rowKeyValue: record ? record.get('测站代码') : null
        }, {
            name: 'FLOW_DIR',
            value: record ? record.get('水流流向ID') : null,
            rowKeyValue: record ? record.get('测站代码') : null
        }, {
            name: 'DAT_TP',
            value: record ? record.get('基面类型ID') : null,
            rowKeyValue: record ? record.get('测站代码') : null
        }, {
            name: 'TS',
            value: null,
            rowKeyValue: record ? record.get('测站代码') : null
        }, {
            name: 'WR_SD_CODE',
            value: null,
            rowKeyValue: record ? record.get('水文测站_ID') : null
        }]);
    },
    link: function (record) {
        this.superMap.reLocation(record.get('水文测站_ID'),record.get('测站名称'),record.get(''),record.get('经纬度坐标'));
        this.reSetForm(record);
        var swczId = this.swczId = record.get('水文测站_ID');
        var scxmId = this.scxmId = record.get('测站代码');
        Ext.Ajax.request({
            url: __path + "/zdst/getSwczscxm.do",
            params: {
                dbName: '_self',
                scxmId: scxmId
            },
            method: 'post',
            success: function (response) {
                var result = Ext.decode(response.responseText);
                var records = result.rows;
                this.scForm.setValues(records);
            },
            scope: this
        });
        Ext.Ajax.request({
            url: __path + "/zdst/getStWithgcz.do",
            params: {
                dbName: '_self',
                swczId: swczId
            },
            method: 'post',
            success: function (response) {
                var result = Ext.decode(response.responseText);
                var records = result.rows;
                this.selectStRecords(records);
            },
            scope: this
        });
        Ext.Ajax.request({
            url: __path + "/zdst/getPwWithgcz.do",
            params: {
                dbName: '_self',
                swczId: swczId
            },
            method: 'post',
            success: function (response) {
                var result = Ext.decode(response.responseText);
                var records = result.rows;
                this.selectPwRecords(records);
            },
            scope: this
        });
        Ext.Ajax.request({
            url: __path + "/zdst/getSydWithgcz.do",
            params: {
                dbName: '_self',
                swczId: swczId
            },
            method: 'post',
            success: function (response) {
                var result = Ext.decode(response.responseText);
                var records = result.rows;
                this.selectSydRecords(records);
            },
            scope: this
        });
        Ext.Ajax.request({
            url: __path + "/zdst/getHddmWithgcz.do",
            params: {
                dbName: '_self',
                swczId: swczId
            },
            method: 'post',
            success: function (response) {
                var result = Ext.decode(response.responseText);
                var records = result.rows;
                this.selectHddmRecords(records);
            },
            scope: this
        });
        Ext.Ajax.request({
            url: __path + "/zdst/getSgnqWithgcz.do",
            params: {
                dbName: '_self',
                swczId: swczId
            },
            method: 'post',
            success: function (response) {
                var result = Ext.decode(response.responseText);
                var records = result.rows;
                this.selectSgnqRecords(records);
            },
            scope: this
        });
    }
});