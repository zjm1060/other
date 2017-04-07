Ext.define('bitc.wrenv.tfsj.fxy', {
    extend: 'Ext.panel.Panel',
    layout: 'border',
    requires : ['bitc.wrenv.tfsj.exportExcelFileButton'],
    initComponent: function () {
        var editFxyData = this.editFxyData = [];
        var addFxyData = this.addFxyData = [];
        var fxyDictionaryStore = this.fxyDictionaryStore = Ext.create('Ext.data.Store', {
            fields: ['fxyId', 'item_id', 'item_name', 'item_values'],
            proxy: {
                type: 'ajax',
                url: __path + '/tfsj/queryFxyDictionaryMessage.do'
            },
            autoLoad: false
        });
        fxyDictionaryStore.load({
            params: {
                fxyId: ''
            }
        });
        //属性表
        var fxyPropertyGrid = this.fxyPropertyGrid = Ext.create('bitc.report.ReportView', {
            reportToken: 'tfsjfxyPropertyGrid',
            reportTopHidden: true,
            region: 'center',
            listeners: {
                gridready: function (view, grid) {
                    this.initEditEvent(grid);
                },
                scope: this
            }
        });

        var fxyPropertyWindow = this.fxyPropertyWindow = Ext.create('Ext.window.Window', {
            title: '污染物属性表',
            width: 600,
            height: 500,
            layout: 'border',
            modal: true,
            closeAction: 'hide',
            items: [fxyPropertyGrid]
        });
        this.initFxyDictionaryGrid();
        this.callParent(arguments);
        this.initList();
    },
    initFxyDictionaryGrid: function () {
        var me = this;
        var functionButton = Ext.create('Ext.panel.Panel', {
            border: false,
            layout: {
                type: 'hbox',
                defaultMargins: {
                    top: 0,
                    right: 5,
                    bottom: 0,
                    left: 0
                }
            },
            items: [{
                xtype: 'button',
                text: '新建风险源',
                handler: function () {
                    me.fxyQueryGrid['风险源_ID'] = null;
                    Ext.getCmp('fxyCoordinate').setValue('');
                    me.fxyDictionaryStore.load({
                        params: {
                            fxyId: me.fxyQueryGrid['风险源_ID']
                        }
                    });
                }
            }, {
                xtype: 'button',
                text: '保存',
                handler: function () {
                    if (me.fxyQueryGrid['风险源_ID'] == null || me.fxyQueryGrid['风险源_ID'] == undefined) {
                        if (me.addFxyData == "") {
                            Ext.Msg.alert("提示信息", "请添加风险源属性再进行保存");
                            return;
                        }
                    }
                    Ext.Ajax.request({
                        url: __path + '/tfsj/saveFxyDictionary.do',
                        params: {
                            fxyId: me.fxyQueryGrid['风险源_ID'] == undefined ? "" : me.fxyQueryGrid['风险源_ID'],
                            coordinate: Ext.getCmp('fxyCoordinate').getValue(),
                            editFxyData: me.editFxyData == "" ? "" : Ext.encode(me.editFxyData),
                            addFxyData: me.addFxyData == "" ? "" : Ext.encode(me.addFxyData)
                        },
                        method: 'POST',
                        success: function () {
                            me.fxyQueryGrid['风险源_ID'] = null;
                            me.addFxyData = [];
                            me.editFxyData = [];
                            Ext.getCmp('fxyCoordinate').setValue('');
                            me.fxyDictionaryStore.load({
                                params: {
                                    fxyId: ''
                                }
                            });
                            me.fxyQueryGrid.getStore().reload();
                            Ext.Msg.alert("提示信息", "保存成功");
                        }
                    });
                }
            },{
                xtype : 'button',
                text : '删除风险源',
                handler : function(){
                    if (me.fxyQueryGrid['风险源_ID'] == null || me.fxyQueryGrid['风险源_ID'] == undefined || me.fxyQueryGrid['风险源_ID'] == "") {
                        Ext.Msg.alert("提示信息", "请选择左侧列表数据再进行删除");
                        return;
                    }
                    Ext.Ajax.request({
                        url: __path + '/tfsj/deleteFxyDictionary.do',
                        params: {
                            fxyId: me.fxyQueryGrid['风险源_ID']
                        },
                        method: 'POST',
                        success: function () {
                            me.fxyQueryGrid['风险源_ID'] = null;
                            Ext.getCmp('fxyCoordinate').setValue('');
                            me.fxyDictionaryStore.load({
                                params: {
                                    fxyId: ''
                                }
                            });
                            me.fxyQueryGrid.getStore().reload();
                            Ext.Msg.alert("提示信息", "删除成功");
                        }
                    });
                }
            }, {
                xtype: 'button',
                text: '添加/修改属性',
                handler: function () {
                    me.fxyPropertyWindow.show();
                }
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

        var fxyDictionaryGrid = this.fxyDictionaryGrid = Ext.create('Ext.grid.Panel', {
            title: '基本信息',
            width: 400,
            store: me.fxyDictionaryStore,
            plugins: {
                ptype: 'rowediting',
                clicksToEdit: 2,
                autoCancel: false,
                saveBtnText: '确定',
                cancelBtnText: '取消'
            },
            columns: [
                {xtype: 'rownumberer', width: 40},
                {text: '风险源_ID', dataIndex: 'fxyId', hidden: true},
                {text: 'item_id', dataIndex: 'item_id', flex: 1, hidden: true},
                {text: '属性名称', dataIndex: 'item_name', flex: 1},
                {text: '属性值', dataIndex: 'item_values', flex: 1, editor: 'textfield'}
            ],
            dockedItems: [{
                xtype: 'panel',
                dock: 'top',
                border: false,
                layout: {
                    type: 'vbox',
                    defaultMargins: {
                        top: 5,
                        right: 5,
                        bottom: 5,
                        left: 5
                    }
                },
                items: [functionButton, {
                    xtype: 'textfield',
                    fieldLabel: '经纬度坐标',
                    id: 'fxyCoordinate',
                    width : 400,
                    vtype: 'LngLat',
                    msgTarget: 'side'
                }]
            }],
            listeners: {
                afterrender: function (grid) {
                    this.initEditFxyEvent(grid);
                },
                scope: this
            }
        });
        var superMap = this.superMap = Ext.create('bitc.report.withSuperMap',{
        });
        this.items = [{
            xtype: 'panel',
            title:'相关信息',
            autoScroll: true,
            collapsible: true,
            split: true,
            layout:'accordion',
            width: 600,
            frame: true,
            region: 'east',
            items: [fxyDictionaryGrid ,waterBodyPanel, superMap]
        }];
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
                } else if (me.fxyQueryGrid['风险源_ID'] == null) {
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
                            id: me.fxyQueryGrid['风险源_ID'],
                            waterBodyIds: waterBodyIds
                        },
                        method: 'post',
                        success: function (response) {
                            var result = Ext.decode(response.responseText);
                            if (result.success) {
                                this.fxyQueryGrid.getStore().reload();
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
    initEditFxyEvent: function (grid) {
        var me = this;
        var rowEditing = grid.plugins[0];
        rowEditing.on('edit', function (editor, e) {
            var record = e.record;
            if (record.raw.fxyId == null) {
                //增加数据
                var fxyId = me.fxyQueryGrid['风险源_ID'];
                var item_id = record.data.item_id;
                var item_values = record.data.item_values;

                var addItem = {
                    fxyId: fxyId,
                    item_id: item_id,
                    item_values: item_values
                };
                var flag = false;
                me.addFxyData.forEach(function (e) {
                    if (e.item_id == addItem.item_id) {
                        e.item_values = addItem.item_values;
                        flag = true;
                    }
                });
                if (!flag) {
                    me.addFxyData.push(addItem);
                }

            } else {
                //更新数据
                var fxyId = record.data.fxyId;
                var item_id = record.data.item_id;
                var item_values = record.data.item_values;
                var item = {
                    fxyId: fxyId,
                    item_id: item_id,
                    item_values: item_values
                };
                var flag = false;
                me.editFxyData.forEach(function (e) {
                    if (e.item_id == item.item_id) {
                        e.item_values = item.item_values;
                        flag = true;
                    }
                });
                if (!flag) {
                    me.editFxyData.push(item);
                }
            }
        });
    },
    initEditEvent: function (grid) {
        this.getFxyPropertyMessage();
        var me = this;
        var rowEditing = grid.rowEditing;
        me.fxyPropertyWindow.down('toolbar').insert(0, [{
            xtype: 'button',
            text: '添加',
            handler: function () {
                var propertyGridStore = grid.getStore();
                var propertyGridModel = propertyGridStore.model;
                propertyGridStore.insert(me.fxyRowCount, propertyGridModel);
                rowEditing.startEdit(me.fxyRowCount, 0);
            },
            scope: grid
        }]);
        rowEditing.on('edit', function (editor, e) {
            var record = e.record;
            var numberList = me.fxyNumberList;
            //该记录的ITEM_ID为空时，即要插入一条污染物属性信息
            if (record.data.ITEM_ID == '') {
                var flag = false;
                numberList.forEach(function (element, index) {
                    if (element == record.data.序号) {
                        Ext.Msg.alert("提示信息", "序号已存在");
                        flag = true;
                    }
                });
                if (!flag) {
                    Ext.Ajax.request({
                        url: __path + '/tfsj/newFxyProperty.do',
                        params: {
                            number: record.data.序号,
                            itemName: record.data.ITEM_NAME
                        },
                        method: 'POST',
                        success: function () {
                            me.getFxyPropertyMessage();
                            grid.getStore().reload();
                            me.fxyDictionaryStore.load({
                                params: {
                                    fxyId: me.fxyQueryGrid['风险源_ID'] == undefined ? "" : me.fxyQueryGrid['风险源_ID']
                                }
                            });
                            me.initList();
                        }
                    });
                }
            } else {
                //如果该记录的ITEM_ID不为空，即要修改污染物属性信息
                var startItemName = record.raw.ITEM_NAME;
                var currentItemName = record.data.ITEM_NAME;
                var startNumber = record.raw.序号;
                var currentNumber = record.data.序号;
                var flag1 = false;
                debugger;
                numberList.forEach(function (element, index) {
                    if (element == record.data.序号 && startNumber != currentNumber) {
                        Ext.Msg.alert("提示信息", "序号已存在,请重新编辑");
                        flag1 = true;
                    }
                });
                if (!flag1) {
                    Ext.Ajax.request({
                        url: __path + '/tfsj/updateFxyProperty.do',
                        params: {
                            itemId: record.data.ITEM_ID,
                            startNumber: startNumber,
                            currentNumber: currentNumber,
                            currentItemName: currentItemName
                        },
                        method: 'POST',
                        success: function () {
                            grid.getStore().reload();
                            me.fxyDictionaryStore.load({
                                params: {
                                    fxyId: me.fxyQueryGrid['风险源_ID'] == undefined ? "" : me.fxyQueryGrid['风险源_ID']
                                }
                            });
                            me.initList();
                            me.getFxyPropertyMessage();
                        }
                    });
                }
            }
        });
    },
    getFxyPropertyMessage: function () {
        var me = this;
        Ext.Ajax.request({
            url: __path + '/tfsj/getFxyPropertyMessage.do',
            method: 'POST',
            async: false,
            success: function (response) {
                var responseText = Ext.decode(response.responseText);
                me.fxyRowCount = responseText['rowCount'];
                me.fxyNumberList = responseText['numberList'];
            }
        });
    },
    initList: function () {
        var me = this;
        if (this.fxyQueryGrid) {
            this.remove(this.fxyQueryGrid);
        }
        Ext.Ajax.request({
            url: __path + '/tfsj/getfxyQueryGridColums.do',
            method: 'post',
            async: false,
            success: function (response) {
                var result = Ext.decode(response.responseText);
                if (result.success) {
                    var meta = result.attributes.meta;
                    var store = Ext.create('Ext.data.Store', {
                        fields: ['fxyId','经纬度坐标'].concat(meta.field,'适用水体'),
                        proxy: {
                            reader:{
                                root:'rows',
                                type:'json'
                            },
                            type: 'ajax',
                            url: __path + '/tfsj/queryFxyMessage.do'
                        },
                        autoLoad: true
                    });
                    var fxyQueryGrid = this.fxyQueryGrid = Ext.create('Ext.grid.Panel', {
                        region: 'center',
                        autoScroll: true,
                        store: store,
                        viewConfig: {
                            enableTextSelection: true
                        },
                        cellTip : true,
                        columns: [{xtype: 'rownumberer', width: 40}, {
                            text: '风险源_ID',
                            dataIndex: 'fxyId',
                            hidden: true
                        },{
                            text: '经纬度坐标',
                            dataIndex: '经纬度坐标',
                            hidden: true
                        }].concat(meta.column,{
                            text: '适用水体',
                            dataIndex: '适用水体',
                            flex : 1
                        }),
                        dockedItems: [{
                            xtype : 'toolbar',
                            dock : 'top',
                            items : [{
                                xtype : 'triggerfield',
                                width : 200,
                                emptyText : '请输入关键字',
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
                                    exportName : '污染风险源'
                                })
                            ]
                        }],
                        listeners: {
                            scope: this,
                            itemdblclick: function (t, record) {
                                me.fxyQueryGrid['风险源_ID'] = record.data.fxyId;
                                Ext.getCmp('fxyCoordinate').setValue(record.data.经纬度坐标);
                                me.fxyDictionaryStore.load({
                                    params: {
                                        fxyId: me.fxyQueryGrid['风险源_ID']
                                    }
                                });
                                Ext.Ajax.request({
                                    url: __path + "/tfsj/getRelatedWaterBody.do",
                                    params: {
                                        id: me.fxyQueryGrid['风险源_ID']
                                    },
                                    method: 'post',
                                    success: function (response) {
                                        var result = Ext.decode(response.responseText);
                                        var records = result.rows;
                                        this.selectStRecords(records);
                                    },
                                    scope: this
                                });
                                this.superMap.reLocation(record.get('fxyId'),record.get('风险源中文名称'),record.get(''),record.get('经纬度坐标'));

                            }
                        }
                    });
                    this.add([fxyQueryGrid]);
                    this.doLayout();
                }
            },
            scope: this
        });
    },
    selectStRecords: function (records) {
        var grid = this.waterBodyPanel.grid;
        grid.selectRecords(function (record) {
            return records.indexOf(record.get('OBJ_ID')) >= 0;
        });
    }
});