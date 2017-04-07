/**
 * Created by Administrator on 2016/4/29.
 */
Ext.define('bitc.wrenv.szpj.wrwruheliang', {
    extend: 'Ext.panel.Panel',
    requires : ['bitc.report.plugins.RowEditingPlugin'],
    layout: 'border',
    initComponent : function(){
        var me = this;
        var addData3 = [];
        var editData3 = [];
        var param3 = this.param3 = ['','cursor'];
        var wrwStore = this.wrwStore = new Ext.data.Store({
            proxy : {
                type: 'ajax',
                url: __path + '/szpj/getWrwZbValue.do'
            },
            fields: ['ITEM_ID','ITEM_NAME','ITEM_VALUE'],
            autoLoad : false
        });

        var wrwGridPanel = this.wrwGridPanel = Ext.create('bitc.report.ReportView',{
            region:'center',
            parameterValues:Ext.encode(param3),
            reportToken: 'wrwruheliangGrid',
            defaultGridWidth :false,
            gridConfig : {
                columnLines : true
            },
            columnConfig : {
                '监测日期':{
                    renderer : function(value,meta,record){
                        meta.column.width = 180;
                        if(value){
                            var str = Ext.util.Format.substr(value,0,10);
                            return Ext.util.Format.date(str,'Y-m-d');
                        }
                        return '';
                    }
                },
                'WFZ_NM' : {
                    renderer : function(value,meta,record){
                        meta.column.minWidth = 300;
                        return value;
                    }
                },
                '化学需氧量' : {
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                '氨氮' : {
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                }
            },
            listeners: {
                gridready: this.initGrid,
                scope: this
            }
        });

        var wrwFormPanel = this.wrwFormPanel = Ext.create('Ext.form.Panel',{
            region : 'east',
            autoScroll : true,
            layout : 'anchor',
            width: 500,
            labelSeparator: ':',
            defaults: {
                padding: '5 5 5 5'
            },
            defaultType: 'textfield',
            anchor:'100%',
            border : false,
            items : [{
                fieldLabel: 'sn_id',
                xtype: 'hiddenfield',
                id : 'snid'
            },{
                fieldLabel: '监测日期',
                xtype: 'datefield',
                id : 'jcTime',
                format :"Y-m-d",
                editable: false,
                allowBlank : false,
                msgTarget:'side'

            },{
                fieldLabel : '排污口ID',
                xtype : 'hiddenfield',
                id : 'waterFunctionID'
            },{
                fieldLabel : '排污口',
                xtype : 'triggerfield',
                id : 'waterFunctionName',
                width : 430,
                editable: false,
                allowBlank : false,
                msgTarget:'side',
                triggerCls : Ext.baseCSSPrefix + 'form-search-trigger',
                onTriggerClick : function(){
                    var store =new Ext.data.Store({
                        proxy:{
                            type:'ajax',
                            url: __path + '/szpj/getPwPlaces.do',
                            extraParams: {
                                waterBodyId: '1'
                            }
                        },
                        fields: [
                            {name: 'obj_id', type: ' string'},
                            {name: 'type',  type: 'string'},
                            {name: 'name',  type: 'string'}
                        ],
                        autoLoad:true
                    });

                    var waterFunctionWin = me.waterFunctionWin = Ext.create('Ext.window.Window', {
                        title: '选择列表',
                        width: 500,
                        height: 400,
                        //forceFit : true,
                        layout:'border',
                        modal:true,
                        closeAction: 'destroy',
                        dockedItems: [{
                            xtype: 'toolbar',
                            dock: 'top',
                            items: [{
                                xtype: 'button',
                                text: '选定',
                                handler:function(){
                                    var record = Ext.getCmp('waterFunctionGrid').getSelectionModel().getSelection();
                                    Ext.getCmp('waterFunctionName').setValue(record[0].data.name);
                                    Ext.getCmp('waterFunctionID').setValue(record[0].data.obj_id);
                                    me.waterFunctionWin.close();
                                }
                            }, {
                                xtype: 'button',
                                text: '取消'
                            }, {
                                xtype: 'textfield',
                                id: 'keywords3',
                                emptyText: '请输入关键字'
                            }, {
                                xtype: 'button',
                                text: '查询',
                                handler: function () {
                                    store.load({
                                        params:{
                                            "keywords":Ext.getCmp('keywords3').getValue()
                                        }
                                    });
                                }
                            }]
                        }],
                        items: [{
                            xtype: 'gridpanel',
                            id : 'waterFunctionGrid',
                            autoScroll : true,
                            region:'center',
                            store: store,
                            selModel: {
                                selType: 'checkboxmodel',
                                mode: 'SINGLE'
                            },
                            cellTip : true,
                            viewConfig: {
                                enableTextSelection: true
                            },
                            columns: [
                                {xtype: 'rownumberer', width: 40},
                                {text: 'id', dataIndex: 'obj_id',hidden:true},
                                {text: '类型', dataIndex: 'type',flex : 1},
                                {text: '名称', dataIndex: 'name',flex : 2}
                            ]
                        }],
                        bbar: ['->','共',{
                            id:'totalnumber',
                            xtype: 'tbtext',
                            text: '0'
                        },'条数据']
                    });
                    store.on('load',function(){
                        Ext.getCmp('totalnumber').setText(store.getTotalCount());
                    });
                    waterFunctionWin.show();
                }
            },{
                xtype : 'grid',
                id : 'wrwGrid',
                title : '污染物指标项',
                autoScroll : true,
                columnLines : true,
                width : 500,
                height : 353,
                store : wrwStore,
                plugins: {
                    ptype: 'editing'
                },
                columns: [
                    //{xtype: 'rownumberer', width: 40},
                    {text: 'item_id', dataIndex: 'ITEM_ID',hidden:true},
                    {text: '污染物指标', dataIndex: 'ITEM_NAME',flex : 2},
                    {text: '污染物入河量', dataIndex: 'ITEM_VALUE',flex : 1,
                        editor :{
                            xtype : 'numberfield',
                            //allowBlank : false,
                            //msgTarget:'side',
                            decimalPrecision: 4
                        },
                        renderer : function(value,meta)
                        {
                            meta.align = 'right';
                            return value;
                        }
                    }
                ],
                listeners : {
                    afterrender : function(grid){
                        //var rowEditing = grid.plugins[0];
                        var rowEditing = grid.rowEditing;
                        var count = 0;
                        var addData4 = [];
                        rowEditing.on('edit',function (editor, e){
                            var record = e.record;
                            if(record.raw.ITEM_VALUE==null){
                                //增加数据
                                var item_ID = record.data.ITEM_ID;
                                var item_NAME = record.data.ITEM_NAME;
                                var item_VALUE = record.data.ITEM_VALUE;

                                var addItem = {
                                    item_ID:item_ID,
                                    item_NAME : item_NAME,
                                    item_VALUE:item_VALUE
                                };
                                var flag = true;
                                addData4.forEach(function(e){
                                    if(e.item_ID == addItem.item_ID){
                                        e.item_NAME = addItem.item_NAME;
                                        e.item_VALUE = addItem.item_VALUE;
                                        flag = false;
                                    }
                                });
                                if(flag && item_VALUE && item_VALUE !=''){
                                    addData4.push(addItem);
                                    count++;
                                }
                                if(count==2){
                                    addData3 = addData4;
                                }
                            }else{
                                //更新数据
                                var item_ID = record.get('ITEM_ID');
                                var item_NAME = record.get('ITEM_NAME');
                                var item_VALUE = record.get('ITEM_VALUE');
                                var item = {
                                    item_ID:item_ID,
                                    item_NAME : item_NAME,
                                    item_VALUE:item_VALUE
                                };
                                var flag = true;
                                editData3.forEach(function(e){
                                    if(e.item_ID == item.item_ID){
                                        e.item_NAME = item.item_NAME;
                                        e.item_VALUE = item.item_VALUE;
                                        flag = false;
                                    }
                                });
                                if(flag && item_VALUE && item_VALUE !=''){
                                    editData3.push(item)
                                }
                            }
                        });
                    },
                    scope : this
                }
            }],
            dockedItems :[{
                xtype : 'toolbar',
                dock : 'top',
                items : [{
                    text : '新建',
                    handler : function(){
                        me.reset();
                    }
                },{
                    text : '保存',
                    handler : function () {
                        if(""==Ext.getCmp('snid').getValue()){
                            if(Ext.getCmp('jcTime').getValue()==null){
                                Ext.Msg.alert("提示信息","监测日期不能为空");
                                return;
                            }
                            if(Ext.getCmp('waterFunctionID').getValue()==""){
                                Ext.Msg.alert("提示信息","排污口不能为空");
                                return;
                            }
                            if(addData3==""){
                                Ext.Msg.alert("提示信息","污染物指标项不能为空");
                                return;
                            }
                            Ext.Ajax.request({
                                url : __path + '/szpj/saveSzy.do',
                                method : 'post',
                                params : {
                                    jcTime : Ext.getCmp('jcTime').getValue().getTime(),
                                    waterFunctionID : Ext.getCmp('waterFunctionID').getValue(),
                                    addData : Ext.encode(addData3)
                                },
                                success : function(response){
                                    var result = Ext.decode(response.responseText);
                                    if(result.success){
                                        Ext.Msg.alert("提示信息","保存成功");
                                        addData3 = [];
                                        editData3 = [];
                                        //保存成功之后清空form表单
                                        me.reset();
                                        me.wrwGridPanel.grid.getStore().reload();
                                    }else{
                                        Ext.Msg.alert("提示信息","保存失败");
                                    }
                                },
                                scope: this
                            });
                        }else{
                            Ext.Ajax.request({
                                url : __path + '/szpj/updateSzy.do',
                                method : 'post',
                                params : {
                                    jcTime : Ext.getCmp('jcTime').getValue().getTime(),
                                    waterFunctionID : Ext.getCmp('waterFunctionID').getValue(),
                                    editData : editData3==""?"":Ext.encode(editData3),
                                    beforeJcTime : me.beforeJcTime,
                                    beforeWaterFounctionID : me.beforeWaterFounctionID
                                },
                                success : function(response){
                                    var result = Ext.decode(response.responseText);
                                    if(result.success){
                                        Ext.Msg.alert("提示信息","修改成功");
                                        addData3 = [];
                                        editData3 = [];
                                        //保存成功之后清空form表单
                                        me.reset();
                                        me.wrwGridPanel.grid.getStore().reload();
                                    }else{
                                        Ext.Msg.alert("提示信息","修改失败");
                                    }
                                },
                                scope: this
                            });
                        }
                    }
                },{
                    text : '删除',
                    handler : function(){
                        Ext.Ajax.request({
                            url : __path + '/szpj/deleteSzy.do',
                            method : 'post',
                            params : {
                                jcTime : Ext.getCmp('jcTime').getValue().getTime(),
                                waterFunctionID : Ext.getCmp('waterFunctionID').getValue()
                            },
                            success : function(response){
                                var result = Ext.decode(response.responseText);
                                if(result.success){
                                    Ext.Msg.alert("提示信息","删除成功");
                                    me.reset();
                                    me.wrwGridPanel.grid.getStore().reload();
                                }
                            },
                            scope: this
                        });
                    }
                }]
            }]
        });
        wrwStore.load({
            params : {
                MONITORID : Ext.getCmp('waterFunctionID').getValue(),
                MONITORDATE : Ext.getCmp('jcTime').getValue()==null?'':Ext.getCmp('jcTime').getValue().getTime()
            }
        });
        this.items = [wrwGridPanel,wrwFormPanel];
        this.callParent(arguments);
    },
    initGrid : function(panel, grid){
        var me = this;
        grid.down('toolbar').insert(0, [{
            xtype: "fieldcontainer", layout: "hbox", items: [
                {
                    xtype: "textfield",
                    id : "key1"
                },
                {
                    xtype : "button",
                    text : "查询",
                    handler : function(){
                        me.param3[0] = Ext.getCmp("key1").getValue();
                        me.wrwGridPanel.repStoreReload(Ext.encode(me.param3));
                    }
                }]
        }]);
        grid.on('itemdblclick', function (g, record) {
            this.reset();
            me.beforeJcTime = Ext.util.Format.date(record.get('监测日期'),'Y-m-d');
            me.beforeWaterFounctionID = record.get('排污口_ID');
            Ext.getCmp('snid').setValue('uuid');//Ext.util.Format.date(record.get('监测日期'),'Y-m-d')
            Ext.getCmp('jcTime').setValue(Ext.util.Format.date(Ext.util.Format.substr(record.get('监测日期'),0,10), 'Y-m-d'));
            Ext.getCmp('waterFunctionID').setValue(record.get('排污口_ID'));
            Ext.getCmp('waterFunctionName').setValue(record.get('PDO_NM'));
            this.wrwStore.load({
                params : {
                    MONITORID : Ext.getCmp('waterFunctionID').getValue(),
                    MONITORDATE : Ext.getCmp('jcTime').getValue()==null?'':Ext.getCmp('jcTime').getValue().getTime()
                }
            });
        }, this);
    },
    reset : function(){
        Ext.getCmp('snid').setValue('');
        Ext.getCmp('jcTime').setValue('');
        Ext.getCmp('waterFunctionID').setValue('');
        Ext.getCmp('waterFunctionName').setValue('');
        this.wrwStore.load({
            params : {
                MONITORID : Ext.getCmp('waterFunctionID').getValue(),
                MONITORDATE : Ext.getCmp('jcTime').getValue()==null?'':Ext.getCmp('jcTime').getValue().getTime()
            }
        });
    }
});

