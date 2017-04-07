/**
 * Created by Administrator on 2016/5/19.
 */
Ext.define('bitc.wrenv.sdl.dmxinxi', {
    extend: 'Ext.panel.Panel',
    layout: 'border',
    requires : ['Ext.ux.InklingSearchField'],
    initComponent: function () {
        var gridview = this.gridview = Ext.create('bitc.report.ReportView',{
            region:'center',
            reportToken: 'sdldmxx',
            //defaultGridWidth : false,
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
                '河底高程' : {
                    renderer : function(value,meta,record){
                        meta.align = "right";
                        //meta.column.width = 100;
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                '计算断面号' : {
                    renderer : function(value,meta,record){
                        meta.align = "right";
                        meta.column.width = 130;
                        return Ext.util.Format.number(value,'0.####');
                    }
                }
            }
        });
        var form = this.form = Ext.create('bitc.report.FormView', {
            formToken: 'sdldmForm',
            title: '基本信息',
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
                            me.poumianGrid['OBJ_ID']=null;
                            me.poumianGrid.reload("1=2");
                        }
                    }, {
                        xtype: 'deletebutton',
                        scope: form,
                        beforeDelete: function () {
                            Ext.Ajax.request({
                                url: __path + "/sdl/deleteDm.do",
                                params: {
                                    objid: form['OBJ_ID']
                                },
                                method: 'post',
                                success: function (response) {
                                    var result = Ext.decode(response.responseText);
                                    if(result.success){
                                        Ext.Msg.alert("提示信息","删除成功");
                                        me.poumianGrid.grid.getStore().reload();
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
                                    tableName: 'sdlm_sys_断面信息表',
                                    rowKey: 'OBJ_ID',
                                    rowKeyValue: form['OBJ_ID']
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
        var poumianGrid = this.poumianGrid = Ext.create('bitc.report.ReportView',{
            title: '断面剖面',
            reportToken: 'dmpm',
            reportCondition: "1=2",
            columnConfig : {
                '左右标记':{
                    renderer: function (value) {
                        return {'0': '', '1': '左','2': '右'}[value];
                    }
                },
                '经度' : {
                    renderer : function(value,meta,record){
                        meta.column.width = 200;
                        return value;
                    }
                },
                '纬度' : {
                    renderer : function(value,meta,record){
                        meta.column.width = 200;
                        return value;
                    }
                },
                '距离' : {
                    renderer : function(value,meta,record){
                        meta.align = "right";
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                '高程' : {
                    renderer : function(value,meta,record){
                        meta.align = "right";
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
            items: [form,poumianGrid]
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
            name: 'OBJ_ID',
            value: record.get('OBJ_ID'),
            rowKeyValue: record.get('OBJ_ID')
        }, {
            name: '计算目标_ID',
            value: record.get('计算目标_ID'),
            rowKeyValue: record.get('OBJ_ID')
        }, {
            name: '河段名称',
            value: record.get('河段名称'),
            rowKeyValue: record.get('OBJ_ID')
        },{
            name: '断面名称',
            value: record.get('断面名称'),
            rowKeyValue: record.get('OBJ_ID')
        }, {
            name: '计算断面号',
            value: record.get('计算断面号'),
            rowKeyValue: record.get('OBJ_ID')
        }, {
            name: '河底高程',
            value: record.get('河底高程'),
            rowKeyValue: record.get('OBJ_ID')
        }, {
            name: '计算标记',
            value: record.get('计算标记'),
            rowKeyValue: record.get('OBJ_ID')
        }]);
        this.form['OBJ_ID'] = record.get('OBJ_ID');
        this.poumianGrid['OBJ_ID'] = record.get('OBJ_ID');
        this.poumianGrid.reload("OBJ_ID ='"+record.get('OBJ_ID')+"'");
    },
    initEditEvent: function (grid) {
        var me = this;
        var rowEditing = grid.rowEditing;
        grid.down('toolbar').insert(0, [{
            xtype: 'button',
            text: '增加剖面',
            handler: function () {
                if (me.poumianGrid['OBJ_ID'] == null) {
                    Ext.Msg.alert('提示', '请在左侧选中一条数据');
                    return;
                }
                var poumianGridStore = grid.getStore();
                var poumianGridModel = poumianGridStore.model;
                poumianGridStore.insert(0, poumianGridModel);
                rowEditing.startEdit(0, 0);
            },
            scope: grid
        }, {
            xtype: 'button',
            text: '删除剖面',
            handler: function () {
                if (me.poumianGrid['OBJ_ID'] == null) {
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
                                url: __path + '/sdl/deletedmpm.do',
                                params: {
                                    snid: data.data.SN_ID,
                                    objid: data.data.OBJ_ID
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
            //如果该记录的OBJ_ID为空时，即要插入一条相关河流信息
            if (record.data.OBJ_ID == '') {
                Ext.Ajax.request({
                    url: __path + '/sdl/insertdmpm.do',
                    params: {
                        objid: me.poumianGrid['OBJ_ID'],
                        x: record.data.经度,
                        y: record.data.纬度,
                        distance : record.data.距离,
                        elevation : record.data.高程,
                        sign : record.data.左右标记,
                        remark : record.data.备注
                    },
                    method: 'POST',
                    success: function () {
                        grid.getStore().reload();
                    }
                });
            } else {
                //如果该记录的OBJ_ID不为空，即要修改相关河流信息
                Ext.Ajax.request({
                    url: __path + '/sdl/updatedmpm.do',
                    params: {
                        objid: me.poumianGrid['OBJ_ID'],
                        x: record.data.经度,
                        y: record.data.纬度,
                        distance : record.data.距离,
                        elevation : record.data.高程,
                        sign : record.data.左右标记,
                        remark : record.data.备注,
                        snid : record.data.SN_ID
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
