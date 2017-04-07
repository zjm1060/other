Ext.define('bitc.wrenv.tfsj.wrw', {
    extend: 'Ext.panel.Panel',
    layout: 'border',
    requires : ['bitc.wrenv.tfsj.exportExcelFileButton'],
    initComponent: function () {
        var editWrwData = this.editWrwData = [];
        var addWrwData = this.addWrwData = [];
        var wrwDictionaryStore = this.wrwDictionaryStore = Ext.create('Ext.data.Store', {
            fields: ['wrwId', 'item_id', 'item_name', 'item_values'],
            proxy: {
                type: 'ajax',
                url: __path + '/tfsj/queryWrwDictionaryMessage.do'
            },
            autoLoad: false
        });
        wrwDictionaryStore.load({
            params: {
                wrwId: ''
            }
        });
        //属性表
        var wrwPropertyGrid = this.wrwPropertyGrid = Ext.create('bitc.report.ReportView', {
            reportToken: 'tfsjwrwPropertyGrid',
            reportTopHidden: true,
            region:'center',
            columnConfig : {
                '序号' : {
                    renderer : function(value,meta,record){
                        meta.column.width = 80;
                        return value;
                    }
                },
                'ITEM_NAME' : {
                    renderer : function(value,meta,record){
                        meta.column.width = 200;
                        return value;
                    }
                }
            },
            listeners: {
                gridready: function (view, grid) {
                    this.initEditEvent(grid);
                },
                scope: this
            }
        });

        var wrwPropertyWindow = this.wrwPropertyWindow = Ext.create('Ext.window.Window', {
            title: '污染物属性表',
            width: 600,
            height: 500,
            layout: 'border',
            modal: true,
            closeAction: 'hide',
            items: [wrwPropertyGrid]
        });
        this.initWrwDictionaryGrid();
        this.callParent(arguments);
        this.initList();
    },
    initWrwDictionaryGrid: function () {
        var me = this;
        var wrwDictionaryGrid = this.wrwDictionaryGrid = Ext.create('Ext.grid.Panel', {
            title: '基本信息',
            width: 400,
            region: 'east',
            split: true,
            autoScroll : true,
            store: me.wrwDictionaryStore,
            plugins: {
                ptype: 'rowediting',
                clicksToEdit: 2,
                autoCancel: false,
                saveBtnText: '确定',
                cancelBtnText: '取消'
            },
            columns: [
                {xtype: 'rownumberer', width: 40},
                {text: '污染物_ID', dataIndex: 'wrwId', hidden: true},
                {text: 'item_id', dataIndex: 'item_id', flex: 1, hidden: true},
                {text: '属性名称', dataIndex: 'item_name', flex: 1},
                {text: '属性值', dataIndex: 'item_values', flex: 1, editor: 'textfield'}
            ],
            dockedItems: [{
                xtype: 'toolbar',
                dock: 'top',
                items: [{
                    xtype: 'button',
                    text: '新建污染物',
                    handler: function () {
                        me.wrwQueryGrid['污染物_ID'] = null;
                        me.wrwDictionaryStore.load({
                            params: {
                                wrwId: me.wrwQueryGrid['污染物_ID']
                            }
                        });
                    }
                }, {
                    xtype: 'button',
                    text: '保存',
                    handler: function () {
                        if (me.wrwQueryGrid['污染物_ID'] == null || me.wrwQueryGrid['污染物_ID'] == undefined) {
                            if (me.addWrwData == "") {
                                Ext.Msg.alert("提示信息", "请添加污染物属性再进行保存");
                                return;
                            }
                        }
                        Ext.Ajax.request({
                            url: __path + '/tfsj/saveWrwDictionary.do',
                            params: {
                                wrwId: me.wrwQueryGrid['污染物_ID'] == undefined ? "" : me.wrwQueryGrid['污染物_ID'],
                                editWrwData: me.editWrwData == "" ? "" : Ext.encode(me.editWrwData),
                                addWrwData: me.addWrwData == "" ? "" : Ext.encode(me.addWrwData)
                            },
                            method: 'POST',
                            success: function () {
                                me.wrwQueryGrid['污染物_ID'] = null;
                                me.addWrwData = [];
                                me.editWrwData = [];
                                me.wrwDictionaryStore.load({
                                    params: {
                                        wrwId: ''
                                    }
                                });
                                me.wrwQueryGrid.getStore().reload();
                                Ext.Msg.alert("提示信息", "保存成功");
                            }
                        });
                    }
                },{
                    xtype : 'button',
                    text : '删除污染物',
                    handler : function(){
                        if (me.wrwQueryGrid['污染物_ID'] == null || me.wrwQueryGrid['污染物_ID'] == undefined || me.wrwQueryGrid['污染物_ID'] == "") {
                            Ext.Msg.alert("提示信息", "请选择左侧列表数据再进行删除");
                            return;
                        }
                        Ext.Ajax.request({
                            url: __path + '/tfsj/deleteWrwDictionary.do',
                            params: {
                                wrwId: me.wrwQueryGrid['污染物_ID']
                            },
                            method: 'POST',
                            success: function () {
                                me.wrwQueryGrid['污染物_ID'] = null;
                                me.wrwDictionaryStore.load({
                                    params: {
                                        wrwId: ''
                                    }
                                });
                                me.wrwQueryGrid.getStore().reload();
                                Ext.Msg.alert("提示信息", "删除成功");
                            }
                        });
                    }
                }, {
                    xtype: 'button',
                    text: '添加/修改属性',
                    handler: function () {
                        me.wrwPropertyWindow.show();
                    }
                }]
            }],
            listeners: {
                afterrender: function (grid) {
                    this.initEditWrwEvent(grid);
                },
                scope: this
            }
        });
        this.items=[wrwDictionaryGrid];
    },
    initEditWrwEvent: function (grid) {
        var me = this;
        var rowEditing = grid.plugins[0];
        rowEditing.on('edit', function (editor, e) {
            var record = e.record;
            debugger;
            if (record.raw.wrwId == null) {
                //增加数据
                var wrwId = me.wrwQueryGrid['污染物_ID'];
                var item_id = record.data.item_id;
                var item_values = record.data.item_values;

                var addItem = {
                    wrwId: wrwId,
                    item_id: item_id,
                    item_values: item_values
                };
                var flag = false;
                me.addWrwData.forEach(function (e) {
                    if (e.item_id == addItem.item_id) {
                        e.item_values = addItem.item_values;
                        flag = true;
                    }
                });
                if (!flag) {
                    me.addWrwData.push(addItem);
                }

            } else {
                //更新数据
                var wrwId = record.data.wrwId;
                var item_id = record.data.item_id;
                var item_values = record.data.item_values;
                var item = {
                    wrwId: wrwId,
                    item_id: item_id,
                    item_values: item_values
                };
                var flag = false;
                me.editWrwData.forEach(function (e) {
                    if (e.item_id == item.item_id) {
                        e.item_values = item.item_values;
                        flag = true;
                    }
                });
                if (!flag) {
                    me.editWrwData.push(item);
                }
            }
        });
    },
    initEditEvent: function (grid) {
        this.getWrwPropertyMessage();
        var me = this;
        var rowEditing = grid.plugins[0];
        me.wrwPropertyWindow.down('toolbar').insert(0, [{
            xtype: 'button',
            text: '添加',
            handler: function () {
                var propertyGridStore = grid.getStore();
                var propertyGridModel = propertyGridStore.model;
                propertyGridStore.insert(me.wrwRowCount, propertyGridModel);
                rowEditing.startEdit(me.wrwRowCount, 0);
            },
            scope: grid
        }]);
        rowEditing.on('edit', function (editor, e) {
            var record = e.record;
            var numberList = me.wrwNumberList;
            //该记录的ITEM_ID为空时，即要插入一条污染物属性信息
            if (record.data.ITEM_ID == '') {
                var flag = false;
                numberList.forEach(function (element, index) {
                    if (element == record.data.序号 ) {
                        Ext.Msg.alert("提示信息", "序号已存在");
                        flag = true;
                    }
                });
                if (!flag) {
                    Ext.Ajax.request({
                        url: __path + '/tfsj/newWrwProperty.do',
                        params: {
                            number: record.data.序号,
                            itemName: record.data.ITEM_NAME
                        },
                        method: 'POST',
                        success: function () {
                            me.getWrwPropertyMessage();
                            grid.getStore().reload();
                            me.wrwDictionaryStore.load({
                                params: {
                                    wrwId: me.wrwQueryGrid['污染物_ID'] == undefined ? "" : me.wrwQueryGrid['污染物_ID']
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
                if (startItemName != currentItemName && startNumber == currentNumber) {
                    Ext.Ajax.request({
                        url: __path + '/tfsj/updateWrwItemNameProperty.do',
                        params: {
                            itemId: record.data.ITEM_ID,
                            number: startNumber,
                            itemName: currentItemName
                        },
                        method: 'POST',
                        success: function () {
                            grid.getStore().reload();
                            me.wrwDictionaryStore.load({
                                params: {
                                    wrwId: me.wrwQueryGrid['污染物_ID'] == undefined ? "" : me.wrwQueryGrid['污染物_ID']
                                }
                            });
                            me.initList();
                            me.getWrwPropertyMessage();
                        }
                    });
                } else if (startNumber != currentNumber) {
                    var flag1 = false;
                    numberList.forEach(function (element, index) {
                        if (element == record.data.序号) {
                            Ext.Msg.alert("提示信息", "序号已存在,请重新编辑");
                            flag1 = true;
                        }
                    });
                    if (!flag1) {
                        Ext.Ajax.request({
                            url: __path + '/tfsj/updateWrwProperty.do',
                            params: {
                                itemId: record.data.ITEM_ID,
                                number: currentNumber,
                                itemName: currentItemName
                            },
                            method: 'POST',
                            success: function () {
                                grid.getStore().reload();
                                me.wrwDictionaryStore.load({
                                    params: {
                                        wrwId: me.wrwQueryGrid['污染物_ID'] == undefined ? "" : me.wrwQueryGrid['污染物_ID']
                                    }
                                });
                                me.initList();
                                me.getWrwPropertyMessage();
                            }
                        });
                    }
                }
            }
        });
    },
    getWrwPropertyMessage: function () {
        var me = this;
        Ext.Ajax.request({
            url: __path + '/tfsj/getWrwPropertyMessage.do',
            method: 'POST',
            async: false,
            success: function (response) {
                var responseText = Ext.decode(response.responseText);
                me.wrwRowCount = responseText['rowCount'];
                me.wrwNumberList = responseText['numberList'];
            }
        });
    },
    initList: function () {
        var me = this;
        if (this.wrwQueryGrid) {
            this.remove(this.wrwQueryGrid);
        }
        Ext.Ajax.request({
            url: __path + '/tfsj/getwrwQueryGridColums.do',
            method: 'post',
            async: false,
            success: function (response) {
                debugger;
                var result = Ext.decode(response.responseText);
                if (result.success) {
                    var meta = result.attributes.meta;
                    var store = Ext.create('Ext.data.Store', {
                        fields: ['wrwId'].concat(meta.field),
                        proxy: {
                            reader:{
                                root:'rows',
                                type:'json'
                            },
                            type: 'ajax',
                            url: __path + '/tfsj/queryWrwMessage.do'
                        },

                        autoLoad: true
                    });
                    var wrwQueryGrid = this.wrwQueryGrid = Ext.create('Ext.grid.Panel', {
                        region: 'center',
                        autoScroll: true,
                        store: store,
                        columns: [{xtype: 'rownumberer', width: 40}, {
                            text: '污染物_ID',
                            dataIndex: 'wrwId',
                            hidden: true
                        }].concat(meta.column),
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
                                    exportName : '污染物'
                                })
                            ]
                        }],
                        listeners: {
                            scope: this,
                            itemdblclick: function (t, record) {
                                me.wrwQueryGrid['污染物_ID'] = record.data.wrwId;
                                me.wrwDictionaryStore.load({
                                    params: {
                                        wrwId: me.wrwQueryGrid['污染物_ID']
                                    }
                                });
                            }
                        }
                    });
                    //this.items = [wrwQueryGrid,wrwDictionaryGrid];
                    this.add([wrwQueryGrid]);
                    this.doLayout();
                }
            },
            scope: this
        });
    }
});