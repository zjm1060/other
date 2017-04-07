Ext.define('bitc.wrenv.tfsj.zhuanjia', {
    extend: 'Ext.form.Panel',
    layout: 'border',
    requires : ['bitc.wrenv.tfsj.exportExcelFileButton'],
    initComponent: function () {
        var editZjData = this.editZjData = [];
        var addZjData = this.addZjData = [];
        var ZjDictionaryStore = this.ZjDictionaryStore = Ext.create('Ext.data.Store', {
            fields: ['zjId', 'item_id', 'item_name', 'item_values'],
            proxy: {
                type: 'ajax',
                url: __path + '/tfsj/queryZjDictionaryMessage.do'
            },
            autoLoad: false
        });
        ZjDictionaryStore.load({
            params: {
                zjId: ''
            }
        });
        //属性表
        var zjPropertyGrid = this.zjPropertyGrid = Ext.create('bitc.report.ReportView', {
            reportToken: 'tfsjzjPropertyGrid',
            reportTopHidden: true,
            region: 'center',
            listeners: {
                gridready: function (view, grid) {
                    this.initEditEvent(grid);
                },
                scope: this
            }
        });

        var zjPropertyWindow = this.zjPropertyWindow = Ext.create('Ext.window.Window', {
            title: '专家库属性表',
            width: 600,
            height: 500,
            layout: 'border',
            modal: true,
            closeAction: 'hide',
            items: [zjPropertyGrid]
        });
        this.initZjDictionaryGrid();
        this.callParent(arguments);
        this.initList();
    },
    initZjDictionaryGrid: function () {
        var me = this;
        var uploadImagePanel= me.uploadImagePanel = Ext.create("bitc.wrenv.tfsj.uploadPhoto", {});
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
                text: '新建专家',
                handler: function () {
                    me.zjQueryGrid['专家_ID'] = null;
                    me.uploadImagePanel.getForm().reset();
                    Ext.getCmp('browseImage').getEl().dom.src = '';
                    me.ZjDictionaryStore.load({
                        params: {
                            zjId: me.zjQueryGrid['专家_ID']
                        }
                    });
                }
            }, {
                xtype: 'button',
                text: '保存',
                handler: function(){
                    if (me.zjQueryGrid['专家_ID'] == null || me.zjQueryGrid['专家_ID'] == undefined) {
                        if (me.addZjData == "") {
                            Ext.Msg.alert("提示信息", "请添加专家属性再进行保存");
                            return;
                        }
                    }
                    me.uploadImagePanel.submit({
                        url:__path + '/tfsj/saveZjDictionary.do',
                        method:'POST',
                        waitTitle : "提示信息",
                        waitMsg: '正在保存...',
                        params: {
                            zjId: me.zjQueryGrid['专家_ID'] == undefined ? "" : me.zjQueryGrid['专家_ID'],
                            editZjData: me.editZjData == "" ? "" : Ext.encode(me.editZjData),
                            addZjData: me.addZjData == "" ? "" : Ext.encode(me.addZjData)
                        },
                        success: function(){
                            me.zjQueryGrid['专家_ID'] = null;
                            me.addZjData = [];
                            me.editZjData = [];
                            me.uploadImagePanel.getForm().reset();
                            Ext.getCmp('browseImage').getEl().dom.src = '';
                            me.ZjDictionaryStore.load({
                                params: {
                                    zjId: ''
                                }
                            });
                            me.zjQueryGrid.getStore().reload();
                            Ext.Msg.alert("提示信息", "保存成功");
                        },
                        failure: function(){
                            Ext.Msg.alert("提示信息", "保存失败");
                        }
                    });
                }
            },{
                xtype : 'button',
                text : '删除',
                handler : function(){
                    if (me.zjQueryGrid['专家_ID'] == null || me.zjQueryGrid['专家_ID'] == undefined || me.zjQueryGrid['专家_ID'] == "") {
                        Ext.Msg.alert("提示信息", "请左侧列表数据再进行删除");
                        return;
                    }
                    Ext.Ajax.request({
                        url:__path + '/tfsj/deleteZjDictionary.do',
                        method:'POST',
                        params: {
                            zjId: me.zjQueryGrid['专家_ID']
                        },
                        success: function(){
                            me.zjQueryGrid['专家_ID'] = null;
                            me.uploadImagePanel.getForm().reset();
                            Ext.getCmp('browseImage').getEl().dom.src = '';
                            me.ZjDictionaryStore.load({
                                params: {
                                    zjId: ''
                                }
                            });
                            me.zjQueryGrid.getStore().reload();
                            Ext.Msg.alert("提示信息", "保存成功");
                        }
                    });
                }
            }, {
                xtype: 'button',
                text: '添加/修改属性',
                handler: function () {
                    me.zjPropertyWindow.show();
                }
            }]
        });

        var zjDictionaryGrid = this.zjDictionaryGrid = Ext.create('Ext.grid.Panel', {
            title: '基本信息',
            width: 400,
            region: 'east',
            split: true,
            autoScroll: true,
            store: me.ZjDictionaryStore,
            plugins: {
                ptype: 'rowediting',
                clicksToEdit: 2,
                autoCancel: false,
                saveBtnText: '确定',
                cancelBtnText: '取消'
            },
            columns: [
                {xtype: 'rownumberer', width: 40},
                {text: '专家_ID', dataIndex: 'zjId', hidden: true},
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
                items: [functionButton, uploadImagePanel]
            }],
            listeners: {
                afterrender: function (grid) {
                    this.initEditZjEvent(grid);
                },
                scope: this
            }
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
            items: [zjDictionaryGrid ,waterBodyPanel]
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
                } else if (me.zjQueryGrid['专家_ID'] == null) {
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
                            id: me.zjQueryGrid['专家_ID'],
                            waterBodyIds: waterBodyIds
                        },
                        method: 'post',
                        success: function (response) {
                            var result = Ext.decode(response.responseText);
                            if (result.success) {
                                this.zjQueryGrid.getStore().reload();
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
    initEditZjEvent: function (grid) {
        var me = this;
        var rowEditing = grid.plugins[0];
        rowEditing.on('edit', function (editor, e) {
            var record = e.record;
            if (record.raw.zjId == null) {
                //增加数据
                var zjId = me.zjQueryGrid['专家_ID'];
                var item_id = record.data.item_id;
                var item_values = record.data.item_values;

                var addItem = {
                    zjId: zjId,
                    item_id: item_id,
                    item_values: item_values
                };
                var flag = false;
                me.addZjData.forEach(function (e) {
                    if (e.item_id == addItem.item_id) {
                        e.item_values = addItem.item_values;
                        flag = true;
                    }
                });
                if (!flag) {
                    me.addZjData.push(addItem);
                }

            } else {
                //更新数据
                var zjId = record.data.zjId;
                var item_id = record.data.item_id;
                var item_values = record.data.item_values;
                var item = {
                    zjId: zjId,
                    item_id: item_id,
                    item_values: item_values
                };
                var flag = false;
                me.editZjData.forEach(function (e) {
                    if (e.item_id == item.item_id) {
                        e.item_values = item.item_values;
                        flag = true;
                    }
                });
                if (!flag) {
                    me.editZjData.push(item);
                }
            }
        });
    },
    initEditEvent: function (grid) {
        this.getZjPropertyMessage();
        var me = this;
        var rowEditing = grid.rowEditing;
        me.zjPropertyWindow.down('toolbar').insert(0, [{
            xtype: 'button',
            text: '添加',
            handler: function () {
                var propertyGridStore = grid.getStore();
                var propertyGridModel = propertyGridStore.model;
                propertyGridStore.insert(me.zjRowCount, propertyGridModel);
                rowEditing.startEdit(me.zjRowCount, 0);
            },
            scope: grid
        }]);
        rowEditing.on('edit', function (editor, e) {
            var record = e.record;
            var numberList = me.zjNumberList;
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
                        url: __path + '/tfsj/newZjProperty.do',
                        params: {
                            number: record.data.序号,
                            itemName: record.data.ITEM_NAME
                        },
                        method: 'POST',
                        success: function () {
                            me.getZjPropertyMessage();
                            grid.getStore().reload();
                            me.ZjDictionaryStore.load({
                                params: {
                                    zjId: me.zjQueryGrid['专家_ID'] == undefined ? "" : me.zjQueryGrid['专家_ID']
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
                numberList.forEach(function (element, index) {
                    if (element == record.data.序号 && startNumber != currentNumber) {
                        Ext.Msg.alert("提示信息", "序号已存在,请重新编辑");
                        flag1 = true;
                    }
                });
                if (!flag1) {
                    Ext.Ajax.request({
                        url: __path + '/tfsj/updateZjProperty.do',
                        params: {
                            itemId: record.data.ITEM_ID,
                            startNumber: startNumber,
                            currentNumber: currentNumber,
                            currentItemName: currentItemName
                        },
                        method: 'POST',
                        success: function () {
                            grid.getStore().reload();
                            me.ZjDictionaryStore.load({
                                params: {
                                    zjId: me.zjQueryGrid['专家_ID'] == undefined ? "" : me.zjQueryGrid['专家_ID']
                                }
                            });
                            me.initList();
                            me.getZjPropertyMessage();
                        }
                    });
                }
            }
        });
    },
    getZjPropertyMessage: function () {
        var me = this;
        Ext.Ajax.request({
            url: __path + '/tfsj/getZjPropertyMessage.do',
            method: 'POST',
            async: false,
            success: function (response) {
                var responseText = Ext.decode(response.responseText);
                me.zjRowCount = responseText['rowCount'];
                me.zjNumberList = responseText['numberList'];
            }
        });
    },
    initList: function () {
        var me = this;
        if (this.zjQueryGrid) {
            this.remove(this.zjQueryGrid);
        }
        Ext.Ajax.request({
            url: __path + '/tfsj/getzjQueryGridColums.do',
            method: 'post',
            async: false,
            success: function (response) {
                var result = Ext.decode(response.responseText);
                if (result.success) {
                    var meta = result.attributes.meta;
                    var store = Ext.create('Ext.data.Store', {
                        fields: ['zjId','上传文件名'].concat(meta.field,'适用水体'),
                        proxy: {
                            reader:{
                                root:'rows',
                                type:'json'
                            },
                            type: 'ajax',
                            url: __path + '/tfsj/queryZjMessage.do'
                        },
                        autoLoad: true
                    });
                    var zjQueryGrid = this.zjQueryGrid = Ext.create('Ext.grid.Panel', {
                        region: 'center',
                        autoScroll: true,
                        store: store,
                        viewConfig: {
                            enableTextSelection: true
                        },
                        cellTip : true,
                        columns: [{xtype: 'rownumberer', width: 40}, {
                            text: '专家_ID',
                            dataIndex: 'zjId',
                            hidden: true
                        },{
                            text: '上传文件名',
                            dataIndex: '上传文件名',
                            hidden: true
                        }].concat(meta.column,{
                            text: '适用水体',
                            dataIndex: '适用水体',
                            flex:1
                        }),
                        dockedItems : [{
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
                                    exportName : '专家库'
                                })
                            ]
                        }],
                        listeners: {
                            scope: this,
                            itemdblclick: function (t, record) {
                                me.zjQueryGrid['专家_ID'] = record.data.zjId;
                                Ext.getCmp('browseImage').getEl().dom.src = __path + '/tfsj/getZjPhoto.do?zjId='+record.data.zjId;
                                me.ZjDictionaryStore.load({
                                    params: {
                                        zjId: me.zjQueryGrid['专家_ID']
                                    }
                                });
                                Ext.getCmp('photo').setRawValue(record.data.上传文件名);
                                Ext.Ajax.request({
                                    url: __path + "/tfsj/getRelatedWaterBody.do",
                                    params: {
                                        id: me.zjQueryGrid['专家_ID']
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
                    this.add([zjQueryGrid]);
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