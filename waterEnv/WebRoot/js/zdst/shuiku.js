Ext.define('bitc.wrenv.zdst.shuiku',{
    extend : 'Ext.panel.Panel',
    requires: ['Ext.ux.InklingSearchField','bitc.report.widget.ChooseField'],
    layout : 'border',
    stId:null,
    initComponent : function(){
        var gridview = this.gridview = Ext.create('bitc.report.ReportView',{
            reportToken: 'skxxxx',
            region: 'center',
            reportCondition: this.stId ? "水体_ID='" + this.stId + "'" : null,
            listeners : {
                gridready : function(view, grid){
                    this.grid = grid;
                    var me = this;
                    grid.down('toolbar').insert(0, [Ext.create('Ext.ux.InklingSearchField', {
                            width: 180,
                            emptyText: '请输入关键字查询',
                            reportView:this.gridview,
                            tooltip: {
                                title: '条件查询',
                                text: '可通过水库名称或者水库代码查询'
                            },
                            inklings: [{
                                name: '水库名称',
                                op: 'like'
                            }, {
                                name: '水库代码',
                                op: 'like'
                            }]

                        }),{
                            text:'显示所有',
                            hidden:me.stId == null?true:false,
                            handler:function(){
                                me.gridview.reportCondition=null;
                                me.gridview.reload();
                                this.hide();
                            }
                        }]
                    );
                    grid.on('itemdblclick',function(g, record){
                        this.link(record);
                        this.superMap.reLocation(record.get('水体_ID'),record.get('水库名称'),record.get(''),record.get('经纬度坐标'));
                    },this);
                    if (this.stId) {
                        this.gridview.grid.getStore().on('load', function (store, records) {
                            var record = records[0];
                            this.gridview.grid.getSelectionModel().select(0);
                            this.link(record);
                        }, this);
                    }
                },
                scope : this
            }
        });
        var form = this.form = Ext.create('bitc.report.FormView',{
            formToken: 'shuikuForm',
            title: '基本信息',
            fieldConfig: {
                ENG_MAN_NM: {
                    setValueFun: function (record) {
                        this.form.getForm().findField('END_MAN_CD').setValue(record.get('ORG_CD'));
                        this.form.getForm().findField('ENG_MAN_NM').setValue(record.get('ORG_NM'));
                    },
                    clearValueFun: function () {
                        this.form.getForm().findField('END_MAN_CD').setValue(null);
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
                }
            },
            listeners : {
                'savesuccess': function () {
                    this.grid.getStore().reload();
                    this.form.getForm().getFields().each(function (field) {
                        field.rowKeyValue = null;
                    });
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
                                    tableName: 'zdst_水体主体信息简表',
                                    rowKey: '水体_ID',
                                    rowKeyValue: form['水体_ID']
                                },{
                                    tableName: 'wr_res_b',
                                    rowKey: 'RES_CD',
                                    rowKeyValue: form['水库代码']
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
                scope : this
            }
        });
        var superMap = this.superMap = Ext.create('bitc.report.withSuperMap');
        var relatedSKRiverPanel = this.relatedSKRiverPanel = Ext.create('bitc.report.ReportView',{
            title: '相关河流',
            reportToken : 'skRelatedRiver',
            reportCondition: "1=2",
            defaultGridWidth : false,
            rowContextMenu: [{
                text: '详细',
                iconCls: '',
                handler: function (widget,record) {
                    var data={
                        id: 'heliu',
                        text: '河流信息数据库',
                        action: 'bitc.wrenv.zdst.heliu',
                        leaf: true,
                        iconCls: '',
                        config: {
                            stId: record.get('水体_ID')
                        }
                    };
                    addWorkTab(data);
                },
                scope: this
            }],
            columnConfig:{
                '河流代码2':{
                    renderer:function(value,meta,record){
                        return record.get('河流名称');
                    }
                },
                '出入库标记2':{
                    renderer:function(value){
                        return {'0':'流入','1':'流出'}[value];
                    }
                }
            },
            listeners:{
                gridready:function(view,grid){
                    this.initEditEvent(grid);
                },
                scope:this
            }
        });
        var shuiwenSKPanel = this.shuiwenSKPanel = Ext.create('bitc.report.ReportView', {
            title: '水文测站',
            reportToken: 'swcz',
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
        var shuiliSKPanel = this.shuiliSKPanel = Ext.create('bitc.report.ReportView', {
            title: '水利工程信息',
            reportToken: 'slgc',
            defaultGridWidth : false,
            reportCondition: "1=2"
        });
        var shuikuRuhepaiwukou = this.shuikuRuhepaiwukou = Ext.create('bitc.report.ReportView',{
            title: '入河排污口',
            reportToken: 'stRuhepaiwukou',
            reportCondition: "1=2",
            columnConfig : {
                '设计日排污能力' : {
                    renderer : function(value,meta,record){
                        meta.align = "right";
                        return value;
                    }
                }
            },
            rowContextMenu: [{
                text: '详细',
                iconCls: '',
                handler: function (widget,record) {
                    var data={
                        id: 'ruhepaiwukou',
                        text: '入河排污口信息数据库',
                        action: 'bitc.wrenv.mgmb.ruhepaiwukou',
                        leaf: true,
                        iconCls: '',
                        config: {
                            rhpwkId: record.get('排污口_ID')
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
            layout:'accordion',
            //collapseMode: 'mini',
            width: 600,
            frame: true,
            split:true,
            region: 'east',
            items: [form,relatedSKRiverPanel,shuiwenSKPanel,shuiliSKPanel,shuikuRuhepaiwukou,superMap]
        }];
        this.callParent(arguments);
    },
    link:function(record){
        this.form.setValues([{
            name: '水体_ID',
            value: record.get('水体_ID'),
            rowKeyValue: record.get('水体_ID')
        },{
            name: 'RES_CD',
            value: record.get('水库代码'),
            rowKeyValue: record.get('水库代码')
        },{
            name: 'RES_NM',
            value: record.get('水库名称'),
            rowKeyValue: record.get('水库代码')
        },{
            name: 'LOC',
            value: record.get('所在地'),
            rowKeyValue: record.get('水库代码')
        },{
            name: 'COMP_YM',
            value: record.get('建成年月'),
            rowKeyValue: record.get('水库代码')
        },{
            name: 'PROJ_SCAL',
            value: record.get('工程规模ID'),
            rowKeyValue: record.get('水库代码')
        },{
            name: 'DAT_TP',
            value: record.get('基面类型ID'),
            rowKeyValue: record.get('水库代码')
        },{
            name: 'CAT_A',
            value: record.get('集水面积'),
            rowKeyValue: record.get('水库代码')
        },{
            name: 'DES_FZ',
            value: record.get('设计洪水位'),
            rowKeyValue: record.get('水库代码')
        },{
            name: 'TOT_V',
            value: record.get('总库容'),
            rowKeyValue: record.get('水库代码')
        },{
            name: 'FRSC_V',
            value: record.get('调洪库容'),
            rowKeyValue: record.get('水库代码')
        },{
            name: 'NORM_WZ',
            value: record.get('正常储水位'),
            rowKeyValue: record.get('水库代码')
        },{
            name: 'CF_Z',
            value: record.get('校核洪水位'),
            rowKeyValue: record.get('水库代码')
        },{
            name: 'UTIL_V',
            value: record.get('兴利库容'),
            rowKeyValue: record.get('水库代码')
        },{
            name: 'FS_Z',
            value: record.get('防洪限制水位'),
            rowKeyValue: record.get('水库代码')
        },{
            name: 'FS_Z_V',
            value: record.get('防洪限制水位库容'),
            rowKeyValue: record.get('水库代码')
        },{
            name: 'DEAD_Z',
            value: record.get('死水位'),
            rowKeyValue: record.get('水库代码')
        },{
            name: 'DEAD_V',
            value: record.get('死库容'),
            rowKeyValue: record.get('水库代码')
        },{
            name: 'RES_REG_TP',
            value: record.get('水库调节方式ID'),
            rowKeyValue: record.get('水库代码')
        },{
            name: 'MIN_DISC',
            value: record.get('最小下泄流量'),
            rowKeyValue: record.get('水库代码')
        },{
            name: 'ST_END_LEN',
            value: record.get('发电引水口至尾水口河道长度'),
            rowKeyValue: record.get('水库代码')
        },{
            name: 'RHCC',
            value: record.get('水库枢纽建筑物组成'),
            rowKeyValue: record.get('水库代码')
        },{
            name: 'END_MAN_CD',
            value: record.get('管理单位代码'),
            rowKeyValue: record.get('水库代码')
        },{
            name: 'ENG_MAN_NM',
            value: record.get('管理单位名称'),
            rowKeyValue: record.get('测站代码')
        },{
            name: 'NT',
            value: record.get('备注'),
            rowKeyValue: record.get('水库代码')
        },{
            name: 'TS',
            value: null,
            rowKeyValue: record.get('水库代码')
        },{
            name: 'wr_sd_code',
            value: record.get('水库代码'),
            rowKeyValue: record.get('水体_ID')
        },{
            name: 'TYPE_ID',
            value: 2,
            rowKeyValue: record.get('水体_ID')
        }, {
            name: '经纬度坐标',
            value: record.get('经纬度坐标'),
            rowKeyValue: record.get('水体_ID')
        }]);
        this.form['水体_ID'] = record.get('水体_ID');
        this.form['水库代码'] = record.get('水库代码');
        this.relatedSKRiverPanel['水体_ID'] = record.get('水体_ID');
        this.relatedSKRiverPanel.reload("水库_ID='"+record.get('水体_ID')+"'");
        this.shuiwenSKPanel.reload("水体_ID='"+record.get('水体_ID')+"'");
        this.shuiliSKPanel.reload("水体_ID='"+record.get('水体_ID')+"'");
        this.shuikuRuhepaiwukou.reload("排入水体id ='"+record.get('水体_ID')+"'");
    },
    initEditEvent : function(grid){
        var me=this;
        var rowEditing=grid.rowEditing;
        grid.down('toolbar').insert(0,[{
            xtype : 'button',
            text : '增加河流',
            handler : function(){
                if(me.relatedSKRiverPanel['水体_ID'] == null){
                    Ext.Msg.alert('提示','请在左侧选中一条数据');
                    return;
                }
                var relatedRiverPanelStore = grid.getStore();
                var relatedRiverPanelModel = relatedRiverPanelStore.model;
                relatedRiverPanelStore.insert(0,relatedRiverPanelModel);
                rowEditing.startEdit(0,0);
            },
            scope : grid
        },{
            xtype : 'button',
            text : '删除河流',
            handler : function(){
                if(me.relatedSKRiverPanel['水体_ID'] == null){
                    Ext.Msg.alert('提示','请在左侧选中一条数据');
                    return;
                }
                var data = grid.getSelectionModel().getLastSelected();
                if(data.length == 0){
                    Ext.Msg.alert('提示','请选中一条数据再删除');
                }else{
                    Ext.Msg.confirm('提示', '确认要删除吗？', function (btn) {
                        if (btn == 'yes') {
                            Ext.Ajax.request({
                                url : __path + '/zdst/deleteSKRelatedRiver.do',
                                params : {
                                    reportToken : 'skRelatedRiver',
                                    shuikuID : data.data.水库_ID,
                                    riverCode : data.data.河流代码2
                                },
                                method : 'POST',
                                success : function(){
                                    grid.getStore().reload();
                                }
                            });
                        }
                    });
                }
            },
            scope : grid
        }]);
        rowEditing.on('beforeedit', function (editor, context) {
            var combo=editor.editor.getForm().findField('河流代码2');
            combo.getStore().clearFilter(true);
            combo.getStore().filter([{
                filterFn: function (item) {
                    if(item.get('valueColumn')==context.record.get('河流代码2')){
                        return true;
                    }
                    var r=grid.getStore().findRecord('河流代码2',item.get('valueColumn'));
                    return r==null;
                }
            }]);
        },this);
        rowEditing.on('edit',function(editor,e){
            var record= e.record;
            //如果该记录的水库_ID为空时，即要插入一条相关河流信息
            if(record.data.水库_ID==''){
                Ext.Ajax.request({
                    url : __path + '/zdst/insertSKRelatedRiver.do',
                    params : {
                        reportToken : 'skRelatedRiver',
                        shuikuID : me.relatedSKRiverPanel['水体_ID'],
                        riverCode : record.data.河流代码2,
                        riverDirect : record.data.出入库标记2
                    },
                    method : 'POST',
                    success:function(){
                        grid.getStore().reload();
                    }
                });
            }else{
                //如果该记录的水库_ID不为空，即要修改相关河流信息
                Ext.Ajax.request({
                    url:__path + '/zdst/updateSKRelatedRiver.do',
                    params:{
                        reportToken : 'hpRelatedRiver',
                        shuikuID : record.data.水库_ID,
                        riverCode : record.data.河流代码2,
                        riverDirect : record.data.出入库标记2,
                        heliuID : record.data.水体_ID
                    },
                    method : 'POST',
                    success:function(){
                        grid.getStore().reload();
                    }
                });
            }

        });
    }
});