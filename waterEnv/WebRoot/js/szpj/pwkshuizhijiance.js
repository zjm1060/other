/**
 * Created by Administrator on 2016/4/21.
 */
Ext.define('bitc.wrenv.szpj.pwkshuizhijiance', {
    extend: 'Ext.panel.Panel',
    layout: 'border',
    requires: ['bitc.report.plugins.DateTimeField'],
    initComponent: function () {
        var param2 = this.param2 = ['排污口','','cursor'];
        var editData2=[];
        var addData2 = [];
        var me = this;
        var zbValueGridStore2 = this.zbValueGridStore2= new Ext.data.Store({
            proxy : {
                type: 'ajax',
                url: __path + '/szpj/getZbValue.do'
            },
            fields: ['iTEM_ID','iTEM_NAME', 'iTEM_VALUE','remark','sYS_ID'],
            autoLoad : false
        });
        var combostore2 = this.combostore2 = new Ext.data.Store({
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

        var zbcombostore2 = this.zbcombostore2 = new Ext.data.Store({
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


        var queryGrid2 =this.queryGrid2 =  Ext.create('bitc.report.ReportView', {
            region:'center',
            parameterValues:Ext.encode(param2),
            reportToken: 'pwkshuizhijiance',
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

        var formPanel2 = this.formPanel2 = Ext.create('Ext.panel.Panel', {
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
                    id : 'formid2'
                },{
                    fieldLabel: '取样时间',
                    xtype: 'sz_DatetimeField',
                    id : 'gcTime2',
                    width : 350,
                    editable: false
                }, {
                    xtype: "hiddenfield",
                    id : 'placeId2',
                    fieldLabel: "观测地点ID"
                },{
                    fieldLabel:'取样地点',
                    id : 'place2',
                    xtype : 'triggerfield',
                    width : 350,
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

                        var win2 = me.win2 = Ext.create('Ext.window.Window', {
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
                                        Ext.getCmp('place2').setValue(record[0].data.name);
                                        Ext.getCmp('placeId2').setValue(record[0].data.obj_id);
                                        me.win2.close();
                                    }
                                }, {
                                    xtype: 'button',
                                    text: '取消'
                                }, {
                                    xtype: 'textfield',
                                    id: 'keywords',
                                    emptyText: '请输入关键字'
                                }, {
                                    xtype: 'button',
                                    text: '查询',
                                    handler: function () {
                                        store.load({
                                            params:{
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
                        win2.show();

                    }
                },{
                    fieldLabel:'排放规律',
                    id:'paifang',
                    xtype: 'textfield',
                    value:0
                },{
                    fieldLabel : '取样位置',
                    id : 'qyPoint2',
                    xtype : "hiddenfield",
                    value : 0

                }, {
                    fieldLabel: '水位',
                    id:'shuiwei2',
                    xtype : 'numberfield'
                }, {
                    fieldLabel: '流量',
                    id : 'liuliang2',
                    xtype : 'numberfield',
                    decimalPrecision : 3
                }, {
                    fieldLabel: '水温',
                    id : 'shuiwen2',
                    xtype : 'numberfield'
                }, {
                    fieldLabel: '气温',
                    id : 'qiwen2',
                    xtype : 'numberfield'
                }, {
                    fieldLabel: '水质评价类别',
                    xtype: 'combo',
                    id:'szType2',
                    width : 350,
                    triggerAction: 'all',
                    store: combostore2,
                    editable: false,
                    queryMode: 'local',
                    displayField: 'dOMAIN_NAME',
                    valueField: 'dOMAIN_ID',
                    forceSelection: true,
                    listeners: {
                        scope:this,
                        'change' : function(comb,newValue,oldValue){
                            Ext.getCmp("zhibiaoType2").clearValue();
                            zbcombostore2.load({
                                params : {
                                    'DOMAIN_ID' : newValue
                                }
                            });
                        }

                    }
                }, {
                    fieldLabel: '指标类型',
                    xtype: 'combo',
                    id:'zhibiaoType2',
                    width : 350,
                    triggerAction: 'all',
                    store: zbcombostore2,
                    editable: false,
                    queryMode: 'local',
                    displayField: 'tYPE_NAME',
                    valueField: 'iTYPE_ID',
                    forceSelection: true,
                    listeners : {
                        'change' : function(comb,newValue,oldValue){
                            zbValueGridStore2.load({
                                params : {
                                    'DOMAIN_ID':Ext.getCmp('szType2').getValue(),
                                    'TYPEID':newValue,
                                    'MONITORID':Ext.getCmp('placeId2').getValue(),
                                    'MONITORSIDE':Ext.getCmp('qyPoint2').getValue(),
                                    'MONITORDATE':Ext.getCmp('gcTime2').getValue()==null?'':Ext.getCmp('gcTime2').getValue().getTime()
                                }
                            });
                        }


                    }
                }, {
                    fieldLabel: '备注',
                    xtype: 'textarea',
                    id : 'remark2',
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
                    id : 'zbgrid2',
                    title : '指标值',
                    autoScroll : true,
                    columnLines : true,
                    width : 500,
                    height : 353,
                    store : zbValueGridStore2,
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
                                    addData2.forEach(function(e){
                                        if(e.item_ID == addItem.item_ID){
                                            e.item_NAME = addItem.item_NAME;
                                            e.item_VALUE = addItem.item_VALUE;
                                            e.remark = addItem.remark;
                                            flag = false;
                                        }
                                    });
                                    if(flag && item_VALUE && item_VALUE !=''){
                                        addData2.push(addItem);
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
                                    editData2.forEach(function(e){
                                        if(e.item_ID == item.item_ID){
                                            e.item_NAME = item.item_NAME;
                                            e.item_VALUE = item.item_VALUE;
                                            e.remark = item.remark;
                                            flag = false;
                                        }
                                    });
                                    if(flag && item_VALUE && item_VALUE !=''){
                                        editData2.push(item)
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
                        if(""==Ext.getCmp('formid2').getValue()){
                            if(addData2==""){
                                Ext.Msg.alert("提示信息","指标项不能为空");
                                return;
                            }
                            Ext.Ajax.request({
                                url : __path + '/szpj/savePwkSzpj.do',
                                method : 'post',
                                params : {
                                    gcTime : Ext.getCmp('gcTime2').getValue().getTime(),
                                    placeId : Ext.getCmp('placeId2').getValue(),
                                    paifang : Ext.getCmp('paifang').getValue(),
                                    qyPoint : Ext.getCmp('qyPoint2').getValue(),
                                    shuiwei : Ext.getCmp('shuiwei2').getValue(),
                                    liuliang : Ext.getCmp('liuliang2').getValue(),
                                    shuiwen : Ext.getCmp('shuiwen2').getValue(),
                                    qiwen : Ext.getCmp('qiwen2').getValue(),
                                    remark : Ext.getCmp('remark2').getValue(),
                                    addData : Ext.encode(addData2)
                                },
                                success : function(response){

                                    var result = Ext.decode(response.responseText);
                                    if(result.success){
                                        Ext.Msg.alert("提示信息","保存成功");
                                        //保存之后要清空addData2和editData2里边的数据
                                        addData2 = [];
                                        editData2 = [];
                                        //保存成功之后清空form表单
                                        me.reset();
                                        me.up2.hide();
                                        me.queryGrid2.grid.getStore().reload();
                                    }else{
                                        Ext.Msg.alert("提示信息","保存失败");
                                    }
                                },
                                scope: this
                            });
                        }else{
                            Ext.Ajax.request({
                                url : __path + '/szpj/updatePwkSzpj.do',
                                method : 'post',
                                params : {
                                    formid : Ext.getCmp('formid2').getValue(),
                                    gcTime : Ext.getCmp('gcTime2').getValue().getTime(),
                                    placeId : Ext.getCmp('placeId2').getValue(),
                                    qyPoint :Ext.getCmp('qyPoint2').getValue(),
                                    paifang : Ext.getCmp('paifang').getValue(),
                                    shuiwei : Ext.getCmp('shuiwei2').getValue(),
                                    liuliang : Ext.getCmp('liuliang2').getValue(),
                                    shuiwen : Ext.getCmp('shuiwen2').getValue(),
                                    qiwen : Ext.getCmp('qiwen2').getValue(),
                                    remark : Ext.getCmp('remark2').getValue(),
                                    editData : editData2==""?"":Ext.encode(editData2),
                                    addData : addData2==""?"":Ext.encode(addData2)
                                },
                                success : function(response){
                                    var result = Ext.decode(response.responseText);
                                    if(result.success){
                                        Ext.Msg.alert("提示信息","修改成功");
                                        addData2 = [];
                                        editData2 = [];
                                        //修改成功之后form表单置为空
                                        me.reset();
                                        me.up2.hide();
                                        me.queryGrid2.grid.getStore().reload();
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
                                gcTime : Ext.getCmp('gcTime2').getValue().getTime(),
                                placeId : Ext.getCmp('placeId2').getValue(),
                                qyPoint : Ext.getCmp("qyPoint2").getValue()
                            },
                            success : function(response){
                                var result = Ext.decode(response.responseText);
                                if(result.success){
                                    Ext.Msg.alert("提示信息","删除成功");
                                    //修改成功之后form表单置为空
                                    me.reset();
                                    me.up2.hide();
                                    me.queryGrid2.grid.getStore().reload();
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
        var up2 = this.up2 = Ext.create('Ext.window.Window', {
            title: '基本信息',
            modal:true,
            height: 550,
            width: 900,
            layout: 'border',
            closeAction : 'hide'
        });
        up2.add(this.formPanel2);
        this.items = [queryGrid2];
        this.callParent(arguments);
    },
    initGrid: function (panel, grid) {
        var me = this;
        grid.down('toolbar').insert(0, [{
            xtype: "fieldcontainer", layout: "hbox", items: [
                {
                    xtype: "textfield",
                    id : "key2"
                },
                {
                    xtype : "button",
                    text : "查询",
                    handler : function(){
                        me.param2[1] = Ext.getCmp("key2").getValue();
                        me.queryGrid2.repStoreReload(Ext.encode(me.param2));
                    }
                }]
        },{
            text: '新建',
            scope: this,
            handler : function(){
                this.reset();
                this.up2.show();
            }
        }]);
        grid.on('itemdblclick', function (g, record) {
            this.reset();
            Ext.getCmp('gcTime2').setValue(record.get('观测时间')?record.get('观测时间').split('.')[0]:'');
            Ext.getCmp('placeId2').setValue(record.get('观测点_ID'));
            Ext.getCmp('formid2').setValue(record.get('SN_ID'));
            Ext.getCmp('place2').setValue(record.get('取样点'));
            //Ext.getCmp('qyPoint2').setValue('0');
            Ext.getCmp('paifang').setValue(record.get('排放规律'));
            Ext.getCmp('shuiwei2').setValue(record.get('水位'));
            Ext.getCmp('liuliang2').setValue(record.get('流量'));
            Ext.getCmp('shuiwen2').setValue(record.get('水温'));
            Ext.getCmp('qiwen2').setValue(record.get('气温'));
            //Ext.getCmp('szType').setValue(record.get(''));
            //Ext.getCmp('zhibiaoType').setValue(record.get(''));
            Ext.getCmp('remark2').setValue(record.get('备注'));
            this.up2.show();
        }, this);
    },
    reset : function(){
        Ext.getCmp("formid2")!=undefined ? Ext.getCmp("formid2").setValue(""):null;
        Ext.getCmp("gcTime2")!=undefined ?Ext.getCmp("gcTime2") .setValue(""):null;
        Ext.getCmp("placeId2")!=undefined?Ext.getCmp("placeId2").setValue(""):null;
        Ext.getCmp("place2")!=undefined?Ext.getCmp("place2").setValue(""):null;
        Ext.getCmp("paifang")!=undefined?Ext.getCmp("paifang").setValue(""):null;
        //Ext.getCmp("qyPoint2")!=undefined?Ext.getCmp("qyPoint2").setValue(""):null;
        Ext.getCmp("shuiwei2")!=undefined?Ext.getCmp("shuiwei2").setValue(""):null;
        Ext.getCmp("liuliang2")!=undefined?Ext.getCmp("liuliang2").setValue(""):null;
        Ext.getCmp("shuiwen2")!=undefined?Ext.getCmp("shuiwen2").setValue(""):null;
        Ext.getCmp("qiwen2")!=undefined?Ext.getCmp("qiwen2").setValue(""):null;
        Ext.getCmp("remark2")!=undefined?Ext.getCmp("remark2").setValue(""):null;
        Ext.getCmp("szType2")!=undefined?Ext.getCmp("szType2").setValue(""):null;
        Ext.getCmp("zhibiaoType2")!=undefined?Ext.getCmp("zhibiaoType2").setValue(""):null;

        this.combostore2.reload();
    }
});
