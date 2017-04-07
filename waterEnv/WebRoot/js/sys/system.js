/**
 * Created by lh on 2016/12/15.
 */
Ext.define('bitc.wrenv.sys.system',{
    extend: 'Ext.panel.Panel',
    requires: ['Ext.grid.feature.Grouping','Ext.ux.InklingSearchField'],
    layout: 'border',
    initComponent : function () {
        var store = Ext.create('Ext.data.Store',{
            fields: ['userId','userName','password','relateWaterBody'],
            proxy : {
                reader:{
                    root:'rows',
                    type:'json'
                },
                type: 'ajax',
                url: __path + '/dataController/queryUserMessage.do'
            },
            autoLoad : true
        });
        var gridview = this.gridview = Ext.create('Ext.grid.Panel',{
            region:'center',
            autoScroll : true,
            columnLines : true,
            store : store,
            columns : [
                {xtype: 'rownumberer', width: 40},
                {text: 'userId', dataIndex: 'userId',hidden:true},
                {text: '用户名', dataIndex: 'userName',flex : 1},
                {text: 'password', dataIndex: 'password',hidden:true},
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
                        exportName : '用户信息'
                    })
                ]
            }],
            listeners: {
                scope: this,
                itemdblclick: function (t, record) {
                    this.link(record);

                }
            }
        });

        var form = this.form = Ext.create('bitc.report.FormView',{
            formToken: 'userForm',
            title: '基本信息',
            listeners : {
                'savesuccess': function () {
                    this.gridview.getStore().reload();
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
                            Ext.Ajax.request({
                                url: __path + "/dataController/deleteUserRelatedWaterBody.do",
                                params: {
                                    userId: me.userId
                                },
                                method: 'post',
                                success: function (response) {
                                    var result = Ext.decode(response.responseText);
                                    if (result.success) {
                                        Ext.Msg.alert("提示信息，删除成功");
                                    } else {
                                        Ext.Msg.alert("提示信息，删除失败");
                                    }
                                },
                                scope: this
                            });
                            return true;
                        },
                        getDeleteRows: function () {
                            return {
                                dbName: '_self',
                                formFields: [{
                                    tableName: 'syz_sys_用户表',
                                    rowKey: 'USER_ID',
                                    rowKeyValue:me.userId
                                }]
                            };
                        },
                        deleteSuccess: function () {
                            me.form.getForm().reset();
                            form.getForm().getFields().each(function (field) {
                                field.rowKeyValue = null;
                            });
                            me.gridview.getStore().reload();
                        }
                    }]);
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

        this.items = [gridview,{
            xtype: 'panel',
            title:'相关信息',
            autoScroll: true,
            collapsible: true,
            layout:'accordion',
            split: true,
            collapseMode: 'mini',
            width: 600,
            frame: true,
            region: 'east',
            items: [form,waterBodyPanel]
        }];
        this.callParent(arguments);
    },
    initWaterBodyGrid : function(panel,grid){
        grid.down('toolbar').insert(0, {
            text: '保存',
            scope: this,
            handler: function () {
                var records = grid.getSelectionModel().getSelection();
                if (records.length == 0) {
                    Ext.MessageBox.alert("提示", "请先选中您要操作的行!");
                    return;
                } else if (this.userId == null) {
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
                        url: __path + "/dataController/saveUserRelatedWaterBody.do",
                        params: {
                            userId: this.userId,
                            waterBodyIds: waterBodyIds
                        },
                        method: 'post',
                        success: function (response) {
                            var result = Ext.decode(response.responseText);
                            if (result.success) {
                                this.gridview.getStore().reload();
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
    link : function (record) {
        this.form.setValues([{
            name: 'USER_ID',
            value:record.data.userId,
            rowKeyValue: record.data.userId
        },{
            name: 'USER_NAME',
            value:record.data.userName,
            rowKeyValue: record.data.userId
        },{
            name: 'PASSWORD',
            value:record.data.password,
            rowKeyValue: record.data.userId
        }]);
        Ext.Ajax.request({
            url: __path + "/dataController/getUserRelatedWaterBody.do",
            params: {
                userId: record.data.userId
            },
            method: 'post',
            success: function (response) {
                var result = Ext.decode(response.responseText);
                var records = result.rows;
                this.selectStRecords(records);
            },
            scope: this
        });
        var userId = this.userId = record.data.userId;
    },
    selectStRecords: function (records) {
        var grid = this.waterBodyPanel.grid;
        grid.selectRecords(function (record) {
            return records.indexOf(record.get('OBJ_ID')) >= 0;
        });
    }
});

