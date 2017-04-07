/**
 * Created by Administrator on 2016/3/9.
 */
Ext.define('bitc.wrenv.szpj.dmshuizhijiance', {
    extend: 'Ext.panel.Panel',
    layout: 'border',
    requires: ['bitc.report.plugins.DateTimeField'],
    initComponent: function () {
        var param = this.param = ['断面','','cursor'];
        var editData=[];
        var addData = [];
        var me = this;
        var qycombostore=this.qycombostore = new Ext.data.Store({
            proxy : {
                type: 'ajax',
                url: __path + '/szpj/getQyPoint.do'
            },
            fields: ['id','point'],
            autoLoad : true
        });
        var zbValueGridStore = this.zbValueGridStore= new Ext.data.Store({
            proxy : {
                type: 'ajax',
                url: __path + '/szpj/getZbValue.do'
            },
            fields: ['iTEM_ID','iTEM_NAME', 'iTEM_VALUE','remark','sYS_ID'],
            autoLoad : false
        });
        var combostore = this.combostore = new Ext.data.Store({
            proxy: {
                type: 'ajax',
                url: __path + '/szpj/getSzType.do',
                reader: {
                    type: 'json',
                    root: 'sz'
                }
            },
            //listeners: {
            //    load: function() {
            //        //获取下拉框的第一条记录
            //        var record = combostore.getAt(0).get('domain_ID');
            //        //把获取到第一条记录设置给下拉框
            //        Ext.getCmp("szType").setValue(record);
            //    }
            //},
            autoLoad: true,
            fields: ['dOMAIN_ID', 'dOMAIN_NAME']
        });

        var zbcombostore = this.zbcombostore = new Ext.data.Store({
            proxy : {
                type: 'ajax',
                url: __path + '/szpj/getZbType.do'
            },
            fields: ['iTYPE_ID', 'tYPE_NAME'],
            //listeners :{
            //    load :  function() {
            //        //获取下拉框的第一条记录
            //        var record = zbcombostore.getAt(0).get('itype_ID');
            //        //把获取到第一条记录设置给下拉框
            //        Ext.getCmp("zhibiaoType").setValue(record);
            //    }
            //},
            autoLoad : false
        });

        var waterBodyStore = this.waterBodyStore = Ext.create('Ext.data.Store',{
            proxy : {
                type: 'ajax',
                url: __path + '/szpj/getWaterBody.do'
            },
            fields: ['waterBodyId', 'waterBodyName'],
            autoLoad : true
        });


        var queryGrid =this.queryGrid =  Ext.create('bitc.report.ReportView', {
            region:'center',
            parameterValues:Ext.encode(param),
            reportToken: 'shuizhijianceGrid',
            gridConfig : {
                columnLines : true
            },
            columnConfig : {
                '取样点':{
                    renderer : function(value,meta,record){
                        //debugger;
                        meta.column.width = 300;
                        return value;
                    }
                },
                '观测时间' : {
                    renderer : function(value,meta,record){
                        meta.column.width = 180;
                        if(value){
                            var str = Ext.util.Format.substr(value,0,19);
                            var date = Ext.util.Format.substr(str,0,10);
                            var time = Ext.util.Format.substr(str,10,19);
                            return date+''+time;
                        }
                        return '';
                    }
                },
                '水位':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                '流量' : {
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                '水温':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                '气温':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'A':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'B':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'C':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'D':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        meta.column.width = 130;
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'E':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'F':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        meta.column.width = 130;
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'G':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'H':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'I':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'J':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'K':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'L':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'M':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'N':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'O':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'P':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'Q':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'R':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'S':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'T':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'U':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'V':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        meta.column.width = 130;
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'W':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'X':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'Y':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'Z':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'AA':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'AB':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'AC':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'AD':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'AE':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'AF':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        meta.column.width = 130;
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'AG':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        return Ext.util.Format.number(value,'0.####');
                    }
                },
                'AH':{
                    renderer : function(value,meta,record){
                        meta.align = 'right';
                        meta.column.width = 160;
                        return Ext.util.Format.number(value,'0.####');
                    }
                }
            },
            listeners: {
                gridready: this.initGrid,
                scope: this
            }
        });

        var formPanel = this.formPanel = Ext.create('Ext.panel.Panel', {
            //title: '基本信息',
            region: 'center',
            autoScroll : true,
            layout: {
                type : 'hbox',
                align : 'left'
            },
            width: 900,
            labelSeparator: ':',
            defaults: {
                padding: '5 5 5 5'
            },
            defaultType: 'textfield',
            items:[{
                xtype : 'panel',
                border:false,
                //anchor:'100%',
                layout: {
                    type : 'vbox',
                    align : 'left'
                },
                items:[{
                    fieldLabel: 'sn_id',
                    xtype: 'hiddenfield',
                    id : 'formid'
                },{
                    fieldLabel: '取样时间',
                    xtype: 'sz_DatetimeField',
                    id : 'gcTime',
                    width : 350,
                    editable: false
                }, {
                    xtype: "hiddenfield",
                    id : 'placeId',
                    fieldLabel: "观测地点ID"
                },{
                    fieldLabel:'取样地点',
                    id : 'place',
                    xtype : 'triggerfield',
                    width : 350,
                    triggerCls : Ext.baseCSSPrefix + 'form-search-trigger',
                    onTriggerClick : function(){
                        var store =new Ext.data.Store({
                            proxy:{
                                type:'ajax',
                                url: __path + '/szpj/getPlaces.do',
                                extraParams : {
                                    "waterBodyId":'1'
                                }
                            },
                            fields: [
                                {name: 'obj_id', type: ' string'},
                                {name: 'type',  type: 'string'},
                                {name: 'name',  type: 'string'}
                            ],
                            autoLoad:true
                        });

                        var win = me.win = Ext.create('Ext.window.Window', {
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
                                        var record = Ext.getCmp('gridPanel').getSelectionModel().getSelection();
                                        Ext.getCmp('place').setValue(record[0].data.name);
                                        Ext.getCmp('placeId').setValue(record[0].data.obj_id);
                                        me.win.close();
                                    }
                                }, {
                                    xtype: 'button',
                                    text: '取消'
                                }, {
                                    xtype: 'combo',
                                    id:'waterBody',
                                    width:150,
                                    triggerAction: 'all',
                                    store: waterBodyStore,
                                    editable: false,
                                    queryMode: 'local',
                                    displayField: 'waterBodyName',
                                    valueField: 'waterBodyId',
                                    listeners : {
                                        select : function (combo ,records ) {
                                            store.load({
                                                params:{
                                                    "waterBodyId":combo.value,
                                                    "keywords":Ext.getCmp('keywords').getValue()
                                                }
                                            });
                                        }
                                    }
                                },{
                                    xtype: 'textfield',
                                    id: 'keywords',
                                    emptyText: '请输入关键字'
                                }, {
                                    xtype: 'button',
                                    text: '查询',
                                    handler: function () {
                                        store.load({
                                            params:{
                                                "waterBodyId":Ext.getCmp('waterBody').getValue(),
                                                "keywords":Ext.getCmp('keywords').getValue()
                                            }
                                        });
                                    }
                                }]
                            }],
                            items: [{
                                xtype: 'gridpanel',
                                id : 'gridPanel',
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
                            //bbar : [{
                            //    id : 'numberBar',
                            //    text : '共有0条数据',
                            //    store : store
                            //}]
                            bbar: ['->','共',{
                                id:'totalnumber',
                                xtype: 'tbtext',
                                text: '0'
                            },'条数据']
                        });
                        //store.on('load',function(){
                        //    Ext.getCmp("numberBar").setText('共有'+store.getTotalCount()+'条数据');
                        //});
                        store.on('load',function(){
                            Ext.getCmp('totalnumber').setText(store.getTotalCount());
                        });
                        win.show();

                    }
                },{
                    fieldLabel:'取样位置',
                    xtype: 'combo',
                    id:'qyPoint',
                    triggerAction: 'all',
                    store: qycombostore,
                    editable: false,
                    queryMode: 'local',
                    displayField: 'point',
                    valueField: 'id'
                }, {
                    fieldLabel: '水位',
                    id:'shuiwei',
                    xtype : 'numberfield'
                }, {
                    fieldLabel: '流量',
                    id : 'liuliang',
                    xtype : 'numberfield',
                    decimalPrecision : 3
                }, {
                    fieldLabel: '水温',
                    id : 'shuiwen',
                    xtype : 'numberfield'
                }, {
                    fieldLabel: '气温',
                    id : 'qiwen',
                    xtype : 'numberfield'
                }, {
                    fieldLabel: '水质评价类别',
                    xtype: 'combo',
                    id:'szType',
                    width : 350,
                    triggerAction: 'all',
                    store: combostore,
                    editable: false,
                    queryMode: 'local',
                    displayField: 'dOMAIN_NAME',
                    valueField: 'dOMAIN_ID',
                    forceSelection: true,
                    listeners: {
                        scope:this,
                        'change' : function(comb,newValue,oldValue){
                            Ext.getCmp("zhibiaoType").clearValue();
                            zbcombostore.load({
                                params : {
                                    'DOMAIN_ID' : newValue
                                }
                            });
                        }

                    }
                }, {
                    fieldLabel: '指标类型',
                    xtype: 'combo',
                    id:'zhibiaoType',
                    width : 350,
                    triggerAction: 'all',
                    store: zbcombostore,
                    editable: false,
                    queryMode: 'local',
                    displayField: 'tYPE_NAME',
                    valueField: 'iTYPE_ID',
                    forceSelection: true,
                    listeners : {
                        'change' : function(comb,newValue,oldValue){
                            zbValueGridStore.load({
                                params : {
                                    'DOMAIN_ID':Ext.getCmp('szType').getValue(),
                                    'TYPEID':newValue,
                                    'MONITORID':Ext.getCmp('placeId').getValue(),
                                    'MONITORSIDE':Ext.getCmp('qyPoint').getValue(),
                                    'MONITORDATE':Ext.getCmp('gcTime').getValue()==null?'':Ext.getCmp('gcTime').getValue().getTime()
                                }
                            });
                        }


                    }
                }, {
                    fieldLabel: '备注',
                    xtype: 'textarea',
                    id : 'remark',
                    width : 350
                }]

            },{
                xtype : 'panel',
                border:false,
                //anchor:'100%',
                layout: {
                    type : 'vbox',
                    align : 'left'
                },
                items:[{
                    xtype : 'grid',
                    id : 'zbgrid',
                    title : '指标值',
                    autoScroll : true,
                    columnLines : true,
                    width : 500,
                    height : 353,
                    store : zbValueGridStore,
                    plugins: {
                        ptype: 'rowediting',
                        clicksToEdit: 2,
                        autoCancel: false,
                        saveBtnText:'确定',
                        cancelBtnText:'取消'
                    },
                    columns: [
                        {xtype: 'rownumberer', width: 40},
                        {text: 'item_id', dataIndex: 'iTEM_ID',hidden:true},
                        {text: 'sys_id',dataIndex: 'sYS_ID',hidden : true},
                        {text: '指标名称', dataIndex: 'iTEM_NAME',flex : 2},
                        {text: '指标值', dataIndex: 'iTEM_VALUE',flex : 1,editor : 'textfield',
                            renderer : function(value,meta)
                            {
                                meta.align = 'right';
                                return Ext.util.Format.number(value,'0.####');
                            }
                        },
                        {text: '说明', dataIndex : 'remark',flex : 4,editor : 'textfield'}
                    ],
                    listeners : {
                        afterrender : function(grid){
                            var rowEditing = grid.plugins[0];
                            rowEditing.on('edit',function (editor, e){
                                var record = e.record;
                                if(record.raw.iTEM_VALUE==""){
                                    //增加数据
                                    var item_ID = record.data.iTEM_ID;
                                    var sys_ID = record.data.sYS_ID;
                                    var item_NAME = record.data.iTEM_NAME;
                                    var item_VALUE = record.data.iTEM_VALUE;
                                    var remark = record.data.remark;

                                    var addItem = {
                                        item_ID:item_ID,
                                        sys_ID : sys_ID,
                                        item_NAME : item_NAME,
                                        item_VALUE:item_VALUE,
                                        remark:remark
                                    };
                                    var flag = true;
                                    addData.forEach(function(e){
                                        if(e.item_ID == addItem.item_ID){
                                            e.item_NAME = addItem.item_NAME;
                                            e.item_VALUE = addItem.item_VALUE;
                                            e.remark = addItem.remark;
                                            flag = false;
                                        }
                                    });
                                    if(flag && item_VALUE && item_VALUE !=''){
                                        addData.push(addItem);
                                    }

                                }else{
                                    //更新数据
                                    var item_ID = record.get('iTEM_ID');
                                    var sys_ID = record.get('sYS_ID');
                                    var item_NAME = record.get('iTEM_NAME');
                                    var item_VALUE = record.get('iTEM_VALUE');
                                    var remark = record.get('remark');
                                    var item = {
                                        item_ID:item_ID,
                                        sys_ID : sys_ID,
                                        item_NAME : item_NAME,
                                        item_VALUE:item_VALUE,
                                        remark:remark
                                    };
                                    var flag = true;
                                    editData.forEach(function(e){
                                        if(e.item_ID == item.item_ID){
                                            e.item_NAME = item.item_NAME;
                                            e.item_VALUE = item.item_VALUE;
                                            e.remark = item.remark;
                                            flag = false;
                                        }
                                    });
                                    if(flag && item_VALUE && item_VALUE !=''){
                                        editData.push(item)
                                    }
                                }
                            });
                        },
                        scope : this
                    }
                }]
            }],
            dockedItems : [{
                xtype : 'toolbar',
                dock : 'top',
                items : [{
                    text : '保存',
                    handler : function(){
                        if(""==Ext.getCmp('formid').getValue()){
                            if(addData==""){
                                Ext.Msg.alert("提示信息","指标项不能为空");
                                return;
                            }
                            Ext.Ajax.request({
                                url : __path + '/szpj/saveSzpj.do',
                                method : 'post',
                                params : {
                                    gcTime : Ext.getCmp('gcTime').getValue().getTime(),
                                    placeId : Ext.getCmp('placeId').getValue(),
                                    qyPoint : Ext.getCmp('qyPoint').getValue(),
                                    shuiwei : Ext.getCmp('shuiwei').getValue(),
                                    liuliang : Ext.getCmp('liuliang').getValue(),
                                    shuiwen : Ext.getCmp('shuiwen').getValue(),
                                    qiwen : Ext.getCmp('qiwen').getValue(),
                                    remark : Ext.getCmp('remark').getValue(),
                                    addData : Ext.encode(addData)
                                },
                                success : function(response){
                                    var result = Ext.decode(response.responseText);
                                    if(result.success){
                                        Ext.Msg.alert("提示信息","保存成功");
                                        addData = [];
                                        editData = [];
                                        //保存成功之后清空form表单
                                        me.reset();
                                        me.up.hide();
                                        me.queryGrid.grid.getStore().reload();
                                    }else{
                                        Ext.Msg.alert("提示信息","保存失败");
                                    }
                                },
                                scope: this
                            });
                        }else{

                            Ext.Ajax.request({
                                url : __path + '/szpj/updateSzpj.do',
                                method : 'post',
                                params : {
                                    formid : Ext.getCmp('formid').getValue(),
                                    gcTime : Ext.getCmp('gcTime').getValue().getTime(),
                                    placeId : Ext.getCmp('placeId').getValue(),
                                    qyPoint : Ext.getCmp('qyPoint').getValue(),
                                    shuiwei : Ext.getCmp('shuiwei').getValue(),
                                    liuliang : Ext.getCmp('liuliang').getValue(),
                                    shuiwen : Ext.getCmp('shuiwen').getValue(),
                                    qiwen : Ext.getCmp('qiwen').getValue(),
                                    remark : Ext.getCmp('remark').getValue(),
                                    editData : editData==""?"":Ext.encode(editData),
                                    addData : addData==""?"":Ext.encode(addData)
                                },
                                success : function(response){
                                    var result = Ext.decode(response.responseText);
                                    if(result.success){
                                        Ext.Msg.alert("提示信息","修改成功");
                                        addData = [];
                                        editData = [];
                                        //修改成功之后form表单置为空
                                        me.reset();
                                        me.up.hide();
                                        me.queryGrid.grid.getStore().reload();
                                    }else{
                                        Ext.Msg.alert("提示信息","修改失败");
                                    }
                                },
                                scope: this
                            });
                        }

                    }
                },'-',{
                    text : '删除',
                    handler : function(){
                        Ext.Ajax.request({
                            url : __path + '/szpj/deleteSzpj.do',
                            method : 'post',
                            params : {
                                gcTime : Ext.getCmp('gcTime').getValue().getTime(),
                                placeId : Ext.getCmp('placeId').getValue(),
                                qyPoint : Ext.getCmp('qyPoint').getValue()
                            },
                            success : function(response){
                                var result = Ext.decode(response.responseText);
                                if(result.success){
                                    Ext.Msg.alert("提示信息","删除成功");
                                    //修改成功之后form表单置为空
                                    me.reset();
                                    me.up.hide();
                                    me.queryGrid.grid.getStore().reload();
                                }else{
                                    Ext.Msg.alert("提示信息","删除失败");
                                }
                                //Ext.getCmp('zbgrid').getStore().reload();
                            },
                            scope: this
                        });
                    }
                }]
            }]


        });
        var up = this.up = Ext.create('Ext.window.Window', {
            title: '基本信息',
            modal:true,
            height: 550,
            width: 900,
            layout: 'border',
            closeAction : 'hide'
        });
        up.add(this.formPanel);
        this.items = [queryGrid];
        this.callParent(arguments);
    },
    initGrid: function (panel, grid) {
        var me = this;
        grid.down('toolbar').insert(0, [{
            xtype: "fieldcontainer", layout: "hbox", items: [
                {
                    xtype: "textfield",
                    id : "key"
                },
                {
                    xtype : "button",
                    text : "查询",
                    handler : function(){
                        me.param[1] = Ext.getCmp("key").getValue();
                        me.queryGrid.repStoreReload(Ext.encode(me.param));
                    }
                }]
        },{
            text: '新建',
            scope: this,
            handler : function(){
                this.reset();
                this.up.show();
            }
        }]);
        grid.on('itemdblclick', function (g, record) {
            this.reset();
            Ext.getCmp('gcTime').setValue(record.get('观测时间')?record.get('观测时间').split('.')[0]:'');
            Ext.getCmp('placeId').setValue(record.get('观测点_ID'));
            Ext.getCmp('formid').setValue(record.get('SN_ID'));
            Ext.getCmp('place').setValue(record.get('取样点'));
            Ext.getCmp('qyPoint').setValue(record.get('取样位置_ID'));
            Ext.getCmp('shuiwei').setValue(record.get('水位'));
            Ext.getCmp('liuliang').setValue(record.get('流量'));
            Ext.getCmp('shuiwen').setValue(record.get('水温'));
            Ext.getCmp('qiwen').setValue(record.get('气温'));
            //Ext.getCmp('szType').setValue(record.get(''));
            //Ext.getCmp('zhibiaoType').setValue(record.get(''));
            Ext.getCmp('remark').setValue(record.get('备注'));
            this.up.show();
        }, this);
    },
    reset : function(){
        Ext.getCmp("formid")!=undefined ? Ext.getCmp("formid").setValue(""):null;
        Ext.getCmp("gcTime")!=undefined ?Ext.getCmp("gcTime").setValue(""):null;
        Ext.getCmp("placeId")!=undefined ?Ext.getCmp("placeId").setValue(""):null;
        Ext.getCmp("place")!=undefined ?Ext.getCmp("place").setValue(""):null;
        Ext.getCmp("qyPoint")!=undefined ?Ext.getCmp("qyPoint").setValue(""):null;
        Ext.getCmp("shuiwei")!=undefined ?Ext.getCmp("shuiwei").setValue(""):null;
        Ext.getCmp("liuliang")!=undefined ?Ext.getCmp("liuliang").setValue(""):null;
        Ext.getCmp("shuiwen")!=undefined ?Ext.getCmp("shuiwen").setValue(""):null;
        Ext.getCmp("qiwen")!=undefined ?Ext.getCmp("qiwen").setValue(""):null;
        Ext.getCmp("remark")!=undefined ?Ext.getCmp("remark").setValue(""):null;
        Ext.getCmp("szType")!=undefined ?Ext.getCmp("szType").setValue(""):null;
        Ext.getCmp("zhibiaoType")!=undefined ?Ext.getCmp("zhibiaoType").setValue(""):null;

        this.qycombostore.reload();
        this.combostore.reload();
    }
});