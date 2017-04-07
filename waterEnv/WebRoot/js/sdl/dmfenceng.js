/**
 * Created by Administrator on 2016/5/20.
 */
Ext.define('bitc.wrenv.sdl.dmfenceng', {
    extend: 'Ext.panel.Panel',
    layout: 'border',
    requires : ['Ext.ux.InklingSearchField'],
    initComponent: function () {
        var gridview = this.gridview = Ext.create('bitc.report.ReportView',{
            region:'center',
            reportToken: 'sdldmfenceng',
            listeners: {
                gridready: this.initGrid,
                scope: this
            },
            columnConfig : {
                '河段名称' : {
                    renderer : function(value,meta,record){
                        meta.column.width = 200;
                        return value;
                    }
                },
                '距离下游断面的距离' : {
                    renderer : function(value,meta,record){
                        meta.align = "right";
                        meta.column.width = 150;
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                '剖面层数个数' : {
                    renderer : function(value,meta,record){
                        meta.align = "right";
                        meta.column.width = 100;
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                '河底高程' : {
                    renderer : function(value,meta,record){
                        meta.align = "right";
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                '计算断面号' : {
                    renderer : function(value,meta,record){
                        meta.align = "right";
                        meta.column.width = 100;
                        return Ext.util.Format.number(value,'0.####');
                    }
                }
            }
        });
        var form = this.form = Ext.create('bitc.report.FormView', {
            formToken: 'sdldmfcForm',
            title: '基本信息',
            listeners: {
                'savesuccess': function () {
                    var me = this;
                    if(this.form.getForm().findField('SN_ID').getValue()!=""){
                        me.form['newJisuan'] = this.form.getForm().findField('计算目标_ID').getValue();
                        me.form['newHdName'] = this.form.getForm().findField('河段名称').getValue();
                        me.form['newDmNumber'] = this.form.getForm().findField('计算断面号').getValue();
                        Ext.Ajax.request({
                            url: __path + '/sdl/updateTotaldmc.do',
                            params: {
                                jisuanid: me.form['jisuanid'],
                                hdName: me.form['hdName'],
                                dmNumber : me.form['dmNumber'],
                                newjisuanid : me.form['newJisuan'],
                                newHdName : me.form['newHdName'],
                                newDmNumber : me.form['newDmNumber']
                            },
                            method: 'POST',
                            success: function () {
                                me.dmCengGrid.grid.getStore().reload();
                            }
                        });
                    }
                    this.grid.getStore().reload();
                    this.form.getForm().reset();
                    this.form.getForm().getFields().each(function (field) {
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
                            me.form['SN_ID'] = null;
                            me.dmCengGrid.reload("1=2");
                        }
                    }, {
                        xtype: 'deletebutton',
                        scope: form,
                        beforeDelete: function () {
                            Ext.Ajax.request({
                                url: __path + "/sdl/deleteTotalDmc.do",
                                params: {
                                    jisuanid: me.form['jisuanid'],
                                    hdName: me.form['hdName'],
                                    dmNumber : me.form['dmNumber']
                                },
                                method: 'post',
                                success: function (response) {
                                    var result = Ext.decode(response.responseText);
                                    if(result.success){
                                        Ext.Msg.alert("提示信息","删除成功");
                                        me.dmCengGrid.grid.getStore().reload();
                                    }else{
                                        Ext.Msg.alert("提示信息","删除失败");
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
                                    tableName: 'sdlm_sys_断面分层水力参数',
                                    rowKey: 'SN_ID',
                                    rowKeyValue: form['SN_ID']
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
        var dmCengGrid = this.dmCengGrid = Ext.create('bitc.report.ReportView',{
            title: '断面层',
            reportToken: 'dmc',
            reportCondition: "1=2",
            columnConfig : {
                '层号' : {
                    renderer : function(value,meta,record){
                        meta.align = "right";
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                '层高程' : {
                    renderer : function(value,meta,record){
                        meta.align = "right";
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                '层河宽' : {
                    renderer : function(value,meta,record){
                        meta.align = "right";
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                '层面积' : {
                    renderer : function(value,meta,record){
                        meta.align = "right";
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                '层水力半径' : {
                    renderer : function(value,meta,record){
                        meta.align = "right";
                        meta.column.width = 100;
                        return Ext.util.Format.number(value,'0.####');
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
        this.items = [gridview, {
            xtype: 'panel',             title:'相关信息',
            autoScroll: true,
            collapsible: true,
            split: true,
            layout:'accordion',
            //collapseMode: 'mini',
            width: 600,
            frame: true,
            region: 'east',
            items: [form,dmCengGrid]
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
                    text: '可通过断面名称查询'
                },
                inklings: [{
                    name: '断面名称',
                    op: 'like'
                }]
            })]
        );
        grid.on('itemdblclick', function (g, record) {
            this.link(record);
        }, this);

    },
    link: function (record) {
        this.form.setValues([{
            name: 'SN_ID',
            value: record.get('SN_ID'),
            rowKeyValue: record.get('SN_ID')
        }, {
            name: '计算目标_ID',
            value: record.get('计算目标_ID'),
            rowKeyValue: record.get('SN_ID')
        }, {
            name: '河段名称',
            value: record.get('河段名称'),
            rowKeyValue: record.get('SN_ID')
        }, {
            name: '计算断面号',
            value: record.get('计算断面号'),
            rowKeyValue: record.get('SN_ID')
        },{
            name: '层号',
            value: record.get('层号'),
            rowKeyValue: record.get('SN_ID')
        },  {
            name: '河段级别',
            value: record.get('河段级别'),
            rowKeyValue: record.get('SN_ID')
        }, {
            name: '距离下游断面的距离',
            value: record.get('距离下游断面的距离'),
            rowKeyValue: record.get('SN_ID')
        }, {
            name: '剖面层数个数',
            value: record.get('剖面层数个数'),
            rowKeyValue: record.get('SN_ID')
        },{
            name: '河底高程',
            value: record.get('河底高程'),
            rowKeyValue: record.get('SN_ID')
        }]);
        this.form['SN_ID'] = record.get('SN_ID');
        this.form['jisuanid'] = record.get('计算目标_ID');
        this.form['hdName'] = record.get('河段名称');
        this.form['dmNumber'] = record.get('计算断面号');

        this.dmCengGrid.reload("计算目标_ID ='"+record.get('计算目标_ID')+"' and 河段名称 ='"+record.get('河段名称')+"' and 计算断面号 = '"+record.get('计算断面号')+"'");

    },
    initEditEvent: function (grid) {
        var me = this;
        var rowEditing = grid.rowEditing;
        grid.down('toolbar').insert(0, [{
            xtype: 'button',
            text: '增加断面层',
            handler: function () {
                debugger;
                if (me.form['SN_ID'] == null) {
                    Ext.Msg.alert('提示', '请在左侧选中一条数据');
                    return;
                }
                var dmCengGridStore = grid.getStore();
                var dmCengGridModel = dmCengGridStore.model;
                dmCengGridStore.insert(0, dmCengGridModel);
                rowEditing.startEdit(0, 0);
            },
            scope: grid
        }, {
            xtype: 'button',
            text: '删除断面层',
            handler: function () {
                if (me.form['SN_ID'] == null) {
                    Ext.Msg.alert('提示', '请在左侧选中一条数据');
                    return;
                }
                var data = grid.getSelectionModel().getLastSelected();
                if (data.length == 0) {
                    Ext.Msg.alert('提示', '请选中一条数据再删除');
                } else {
                    Ext.Msg.confirm('提示', '确认要删除吗？', function (btn) {
                        if (btn == 'yes') {
                            Ext.Ajax.request({
                                url: __path + '/sdl/deletedmc.do',
                                params: {
                                    snid: data.data.SN_ID
                                },
                                method: 'POST',
                                success: function () {
                                    grid.getStore().reload();
                                }
                            });
                        }
                    });
                }
            },
            scope: grid
        }]);
        rowEditing.on('edit', function (editor, e) {
            var record = e.record;
            //如果该记录的SN_ID为空时，即要插入一条相关河流信息
            if (record.data.SN_ID == '') {
                Ext.Ajax.request({
                    url: __path + '/sdl/insertdmc.do',
                    params: {
                        jisuanid: me.form['jisuanid'],
                        hdName: me.form['hdName'],
                        dmNumber : me.form['dmNumber'],
                        cNumber : record.data.层号,
                        cGao : record.data.层高程,
                        cWidth : record.data.层河宽,
                        cArea : record.data.层面积,
                        cRadius : record.data.层水力半径
                    },
                    method: 'POST',
                    success: function () {
                        grid.getStore().reload();
                    }
                });
            } else {
                //如果该记录的SN_ID不为空，即要修改相关河流信息
                Ext.Ajax.request({
                    url: __path + '/sdl/updatedmc.do',
                    params: {
                        snid: record.data.SN_ID,
                        cNumber : record.data.层号,
                        cGao : record.data.层高程,
                        cWidth : record.data.层河宽,
                        cArea : record.data.层面积,
                        cRadius : record.data.层水力半径
                    },
                    method: 'POST',
                    success: function () {
                        grid.getStore().reload();
                    }
                });
            }

        });
    }
});
