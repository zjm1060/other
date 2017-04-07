Ext.define('bitc.wrenv.tfsj.laws', {
    extend: 'Ext.panel.Panel',
    layout: 'border',
    requires: ['bitc.report.widget.CombWidget'],
    initComponent: function () {

        var store = Ext.create('Ext.data.Store',{
            fields: ['planId','planName','date','typeId','fileName','relateWaterBody'],
            proxy : {
                reader:{
                    root:'rows',
                    type:'json'
                },
                type: 'ajax',
                url: __path + '/tfsj/queryPlanAndLawMessage.do',
                extraParams: {
                    typeId:'2'
                }
            },
            autoLoad : true
        });

        var planQueryGrid = this.planQueryGrid = Ext.create('Ext.grid.Panel',{
            region:'center',
            autoScroll : true,
            columnLines : true,
            store : store,
            viewConfig: {
                enableTextSelection: true
            },
            cellTip : true,
            columns : [
                {xtype: 'rownumberer', width: 40},
                {text: 'planId', dataIndex: 'planId',hidden:true},
                {text: '应急法规名称', dataIndex: 'planName',flex : 1},
                {text: '编制日期', dataIndex: 'date',hidden:true},
                {text: 'typeId', dataIndex: 'typeId',hidden:true},
                {text: 'fileName', dataIndex: 'fileName',hidden:true},
                {text: '适用水体', dataIndex: 'relateWaterBody',flex : 2}
            ],
            dockedItems: [{
                xtype : 'toolbar',
                dock : 'top',
                items : [{
                    xtype : 'triggerfield',
                    width : 200,
                    emptyText : '请输入关键字查询',
                    hasSearch: false,
                    enableKeyEvents : true,
                    triggerCls : Ext.baseCSSPrefix + 'form-search-trigger',
                    onTriggerClick : function(){
                        store.load({
                            params : {
                                keyword : this.getValue()
                            }
                        });
                    },
                    listeners : {
                        scope : this,
                        keyup : function(field, e) {
                            if (e.getKey() == Ext.EventObject.ENTER) {
                                store.load({
                                    params : {
                                        keyword : field.value
                                    }
                                });
                            }
                        }
                    }
                }]
            },{
                xtype: 'pagingtoolbar',
                store: store,
                dock: 'bottom',
                displayInfo: true,
                items : [
                    Ext.create('bitc.wrenv.tfsj.exportExcelFileButton',{
                        exportName : '应急法规'
                    })
                ]
            }],
            listeners: {
                scope: this,
                itemdblclick: function (t, record) {
                    var me = this;
                    var form = me.planAndLawsFormPanel.getForm();
                    form.findField('id').setValue(record.data.planId);
                    form.findField('name').setValue(record.data.planName);
                    form.findField('time').setValue(Ext.util.Format.date(Ext.util.Format.substr(record.data.date,0,10), 'Y-m-d'));
                    form.findField('file').setRawValue(record.data.fileName);

                    me.planQueryGrid['planId'] = record.data.planId;

                    Ext.Ajax.request({
                        url: __path + "/tfsj/getRelatedWaterBody.do",
                        params: {
                            id: me.planQueryGrid['planId']
                        },
                        method: 'post',
                        success: function (response) {
                            var result = Ext.decode(response.responseText);
                            var records = result.rows;
                            this.selectStRecords(records);
                        },
                        scope: this
                    });
                }
            }
        });

        var planAndLawsFormPanel = this.planAndLawsFormPanel = Ext.create('Ext.form.Panel', {
            title: '基本信息',
            region: 'east',
            width : 400,
            labelSeparator: ':',
            defaults: {
                padding: '5 5 5 5'
            },
            dockedItems: [{
                xtype: 'toolbar',
                docked: 'top',
                items: [{
                    text: '新建',
                    scope: this,
                    handler: this.new
                }, {
                    text: '保存',
                    scope: this,
                    handler: this.save
                }, {
                    text: '删除',
                    scope: this,
                    handler: this.delete
                },{
                    text : '下载',
                    scope : this,
                    handler : this.download
                }]
            }],
            items: [{
                xtype: 'hiddenfield',
                fieldLabel: 'id',
                name: 'id'
            }, {
                xtype: 'textfield',
                fieldLabel: '应急法规名称',
                name: 'name',
                allowBlank : false,
                msgTarget : 'side'
            }, {
                xtype: 'datefield',
                fieldLabel: '编制日期',
                name: 'time',
                format: 'Y-m-d',
                allowBlank : false,
                msgTarget : 'side'
            }, {
                xtype: 'hiddenfield',
                fieldLabel : '类型',
                name : 'type',
                value : '2'
            },{
                xtype : 'filefield',
                fieldLabel : '附件',
                name : 'file',
                //id :'file',
                buttonText : '选择附件',
                width : 350
            }]
        });

        var waterBodyPanel = this.waterBodyPanel = Ext.create('bitc.report.ReportView',{
            reportToken: 'waterBodyGrid',
            title: '适用水体',
            defaultGridWidth :false,
            gridConfig: {
                groupField: 'OBJ_ID',
                features: [{
                    ftype: 'grouping',
                    groupHeaderTpl: ['{rows:this.getName}', {
                        getName: function (rows) {
                            return rows[0].get('TYPE_NAME');
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
            listeners: {
                gridready: this.initWaterBodyGrid,
                scope: this
            }
        });


        this.items = [planQueryGrid,{
            xtype: 'panel',
            title:'相关信息',
            autoScroll: true,
            collapsible: true,
            split: true,
            layout:'accordion',
            width: 600,
            frame: true,
            region: 'east',
            items: [planAndLawsFormPanel ,waterBodyPanel]
        }];
        this.callParent(arguments);
    },
    initWaterBodyGrid : function(panel,grid){
        var me = this;
        grid.down('toolbar').insert(0, {
            text: '保存',
            scope: this,
            handler: function () {
                var records = grid.getSelectionModel().getSelection();
                if (records.length == 0) {
                    Ext.MessageBox.alert("提示", "请先选中您要操作的行!");
                    return;
                } else if (me.planQueryGrid['planId'] == null) {
                    Ext.MessageBox.alert("提示", "请先选择相关联的用户!");
                    return;
                } else {
                    var waterBodyIds = "";
                    for (var i = 0; i < records.length; i++) {
                        waterBodyIds += records[i].get("OBJ_ID");
                        if (i < records.length - 1) {
                            waterBodyIds = waterBodyIds + ",";
                        }
                    }
                    Ext.Ajax.request({
                        url: __path + "/tfsj/saveRelatedWaterBody.do",
                        params: {
                            id: me.planQueryGrid['planId'],
                            waterBodyIds: waterBodyIds
                        },
                        method: 'post',
                        success: function (response) {
                            var result = Ext.decode(response.responseText);
                            if (result.success) {
                                this.planQueryGrid.getStore().reload();
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
        var grid = this.waterBodyPanel.grid;
        grid.selectRecords(function (record) {
            return records.indexOf(record.get('OBJ_ID')) >= 0;
        });
    },
    download : function(){
        var me = this;
        if (me.planQueryGrid['planId'] == null || me.planQueryGrid['planId'] == undefined || me.planQueryGrid['planId'] == "") {
            Ext.Msg.alert("提示信息", "请先选择左侧列表一条信息");
            return;
        }
        window.open(__path + '/tfsj/downLoadPlanAndLaws.do?planAndLawsId='+me.planQueryGrid['planId']);
    },
    new: function () {
        this.planAndLawsFormPanel.getForm().reset();
    },
    save: function () {
        debugger;
        var me = this;
        if(this.planAndLawsFormPanel.isValid()){
            this.planAndLawsFormPanel.getForm().submit({
                url: __path + '/tfsj/savePlanAndLaws.do',
                method: 'POST',
                waitTitle: "提示信息",
                waitMsg: '正在保存...',
                params: {
                    planAndLawsId: this.planAndLawsFormPanel.getForm().findField('id').getValue() == undefined ? "" : this.planAndLawsFormPanel.getForm().findField('id').getValue(),
                    planAndLawsName : this.planAndLawsFormPanel.getForm().findField('name').getValue(),
                    planAndLawsTime : this.planAndLawsFormPanel.getForm().findField('time').getValue().getTime(),
                    planAndLawsType : this.planAndLawsFormPanel.getForm().findField('type').getValue()
                },
                success: function () {
                    Ext.Msg.alert("提示信息", "保存成功");
                    me.planAndLawsFormPanel.getForm().reset();
                    me.planQueryGrid.getStore().reload();
                },
                failure: function () {
                    Ext.Msg.alert("提示信息", "保存失败");
                }
            });
        }
    },
    delete: function () {
        var me = this;
        if (me.planQueryGrid['planId'] == null || me.planQueryGrid['planId'] == undefined || me.planQueryGrid['planId'] == "") {
            Ext.Msg.alert("提示信息", "请先选择左侧列表一条信息");
            return;
        }
        this.planAndLawsFormPanel.getForm().submit({
            url: __path + '/tfsj/deletePlanAndLaws.do',
            method: 'POST',
            params: {
                planAndLawsId: me.planQueryGrid['planId']
            },
            success: function () {
                me.planAndLawsFormPanel.getForm().reset();
                me.planQueryGrid.getStore().reload();
                Ext.Msg.alert("提示信息", "删除成功");
            },
            failure: function () {
                Ext.Msg.alert("提示信息", "删除失败");
            }
        });
    }
});
