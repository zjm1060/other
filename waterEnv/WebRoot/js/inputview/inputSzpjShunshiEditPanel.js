Ext.define('bitc.wrenv.inputview.inputSzpjShunshiEditPanel', {
    extend: 'Ext.form.Panel',
    border: false,
    autoScroll: true,
    maxHeight:550,
    defaults: {
        anchor: '98%',
        labelWidth: 80,
        xtype: 'fieldset',
        layout: 'anchor',
        padding: '5 5 5 5',
        bodyPadding: 10
    },
    initComponent: function () {
        var me = this;
        var heduan =this.heduan= Ext.create('bitc.wrenv.inputview.inputDayinDuanMianPanel');
        var wuranyuan= this.wuranyuan = Ext.create('bitc.wrenv.inputview.inputDayinDuanMianPanel');
        var wuranwuList = ['', 'BOD5', 'NBOD', '氨氮(NH3-N)', '亚硝酸盐氮', '硝酸盐氮', 'DO', '化学需氧量(COD)'];
        var jiangjieheshifang = this.jiangjieheshifang= Ext.create('Ext.form.FieldSet',{
            title:'降解和释放系数信息',
            anchor: '98%',
            autoScroll:true,
            maxHeight:400,
                xtype: 'fieldset',
            layout: 'anchor',
            padding: '5 5 5 5',
            bodyPadding: 10,
            items:[]
        });
        for(var i=1;i<8;i++){
            jiangjieheshifang.add(this.getWuRanWuConf('污染物'+i,'wrw_'+i+'_conf',wuranwuList[i]));
        }
        var wuranwu = this.wuranwu = Ext.create('Ext.form.FieldContainer',{
            //xtype: 'fieldcontainer',
            autoScroll: true,
            defaults: {
                labelWidth: 100
            },
            layout: {
                type: 'hbox',
                hideLabel: false,
                defaultMargins: {
                    top: 0,
                    right: 5,
                    bottom: 5,
                    left: 0
                }
            },
            items:[{
                xtype:'fieldcontainer',
                defaults: {
                    labelWidth: 60,
                    submitValue:false
                },
                layout: {
                    type: 'hbox',
                    hideLabel: false,
                    defaultMargins: {
                        top: 0,
                        right: 5,
                        bottom: 5,
                        left: 0
                    }
                },
                items:[{
                    xtype:'checkbox',
                    name:'wrw1',
                    boxLabel:'污染物1',
                    disabled:true,
                    width:80,
                    listeners:{
                        scope:this,
                        change:function(box,newValue){
                            if(newValue){
                                jiangjieheshifang.items.each(function(item){
                                    if(item.wrwName == '污染物1'){
                                        item.show();
                                    }
                                });
                            }else{
                                jiangjieheshifang.items.each(function(item){
                                    if(item.wrwName == '污染物1'){
                                        item.hide();
                                    }
                                });
                            }
                        }
                    }
                },{
                    xtype:'textfield',
                    name:'wrw1name',
                    value:'BOD5',
                    disabled:true,
                    width:80,
                    listeners:{
                        change:function(t,newValue){
                            jiangjieheshifang.items.each(function(item){
                                if(item.wrwName == '污染物1'){
                                    item.items.getAt(0).setValue(newValue);
                                }
                            });
                        }
                    }
                }]
            },{
                xtype:'fieldcontainer',
                defaults: {
                    labelWidth: 60,
                    submitValue:false
                },
                layout: {
                    type: 'hbox',
                    hideLabel: false,
                    defaultMargins: {
                        top: 0,
                        right: 5,
                        bottom: 5,
                        left: 0
                    }
                },
                items:[{
                    xtype:'checkbox',
                    name:'wrw2',
                    boxLabel:'污染物2',
                    disabled:true,
                    width:80,
                    listeners:{
                        scope:this,
                        change:function(box,newValue){
                            if(newValue){
                                jiangjieheshifang.items.each(function(item){
                                    if(item.wrwName == '污染物2'){
                                        item.show();
                                    }
                                });
                            }else{
                                jiangjieheshifang.items.each(function(item){
                                    if(item.wrwName == '污染物2'){
                                        item.hide();
                                    }
                                });
                            }
                        }
                    }
                },{
                    xtype:'textfield',
                    name:'wrw2name',
                    disabled:true,
                    value:'NBOD',
                    width:80,
                    listeners:{
                        change:function(t,newValue){
                            jiangjieheshifang.items.each(function(item){
                                if(item.wrwName == '污染物2'){
                                    item.items.getAt(0).setValue(newValue);
                                }
                            });
                        }
                    }
                }]
            },{
                xtype:'fieldcontainer',
                defaults: {
                    labelWidth: 60,
                    submitValue:false
                },
                layout: {
                    type: 'hbox',
                    hideLabel: false,
                    defaultMargins: {
                        top: 0,
                        right: 5,
                        bottom: 5,
                        left: 0
                    }
                },
                items:[{
                    xtype:'checkbox',
                    name:'wrw3',
                    boxLabel:'污染物3',
                    width:80,
                    listeners:{
                        scope:this,
                        change:function(box,newValue){
                            if(newValue){
                                jiangjieheshifang.items.each(function(item){
                                    if(item.wrwName == '污染物3'){
                                        item.show();
                                    }
                                });
                            }else{
                                jiangjieheshifang.items.each(function(item){
                                    if(item.wrwName == '污染物3'){
                                        item.hide();
                                    }
                                });
                            }
                        }
                    }
                },{
                    xtype:'textfield',
                    name:'wrw3name',
                    value:'氨氮(NH3-N)',
                    disabled:true,
                    width:80,
                    listeners:{
                        change:function(t,newValue){
                            jiangjieheshifang.items.each(function(item){
                                if(item.wrwName == '污染物3'){
                                    item.items.getAt(0).setValue(newValue);
                                }
                            });
                        }
                    }
                }]
            },{
                xtype:'fieldcontainer',
                defaults: {
                    labelWidth: 60,
                    submitValue:false
                },
                layout: {
                    type: 'hbox',
                    hideLabel: false,
                    defaultMargins: {
                        top: 0,
                        right: 5,
                        bottom: 5,
                        left: 0
                    }
                },
                items:[{
                    xtype:'checkbox',
                    name:'wrw4',
                    boxLabel:'污染物4',
                    disabled:true,
                    width:80,
                    listeners:{
                        scope:this,
                        change:function(box,newValue){
                            if(newValue){
                                jiangjieheshifang.items.each(function(item){
                                    if(item.wrwName == '污染物4'){
                                        item.show();
                                    }
                                });
                            }else{
                                jiangjieheshifang.items.each(function(item){
                                    if(item.wrwName == '污染物4'){
                                        item.hide();
                                    }
                                });
                            }
                        }
                    }
                },{
                    xtype:'textfield',
                    name:'wrw4name',
                    value:'亚硝酸盐氮',
                    disabled:true,
                    width:80,
                    listeners:{
                        change:function(t,newValue){
                            jiangjieheshifang.items.each(function(item){
                                if(item.wrwName == '污染物4'){
                                    item.items.getAt(0).setValue(newValue);
                                }
                            });
                        }
                    }
                }]
            },{
                xtype:'fieldcontainer',
                defaults: {
                    labelWidth: 60,
                    submitValue:false
                },
                layout: {
                    type: 'hbox',
                    hideLabel: false,
                    defaultMargins: {
                        top: 0,
                        right: 5,
                        bottom: 5,
                        left: 0
                    }
                },
                items:[{
                    xtype:'checkbox',
                    name:'wrw5',
                    boxLabel:'污染物5',
                    disabled:true,
                    width:80,
                    listeners:{
                        scope:this,
                        change:function(box,newValue){
                            if(newValue){
                                jiangjieheshifang.items.each(function(item){
                                    if(item.wrwName == '污染物5'){
                                        item.show();
                                    }
                                });
                            }else{
                                jiangjieheshifang.items.each(function(item){
                                    if(item.wrwName == '污染物5'){
                                        item.hide();
                                    }
                                });
                            }
                        }
                    }
                },{
                    xtype:'textfield',
                    name:'wrw5name',
                    value:'硝酸盐氮',
                    disabled:true,
                    width:80,
                    listeners:{
                        change:function(t,newValue){
                            jiangjieheshifang.items.each(function(item){
                                if(item.wrwName == '污染物5'){
                                    item.items.getAt(0).setValue(newValue);
                                }
                            });
                        }
                    }
                }]
            },{
                xtype:'fieldcontainer',
                defaults: {
                    labelWidth: 60,
                    submitValue:false
                },
                layout: {
                    type: 'hbox',
                    hideLabel: false,
                    defaultMargins: {
                        top: 0,
                        right: 5,
                        bottom: 5,
                        left: 0
                    }
                },
                items:[{
                    xtype:'checkbox',
                    name:'wrw6',
                    boxLabel:'污染物6',
                    disabled:true,
                    width:80,
                    listeners:{
                        scope:this,
                        change:function(box,newValue){
                            if(newValue){
                                jiangjieheshifang.items.each(function(item){
                                    if(item.wrwName == '污染物6'){
                                        item.show();
                                    }
                                });
                            }else{
                                jiangjieheshifang.items.each(function(item){
                                    if(item.wrwName == '污染物6'){
                                        item.hide();
                                    }
                                });
                            }
                        }
                    }
                },{
                    xtype:'textfield',
                    name:'wrw6name',
                    value:'DO',
                    disabled:true,
                    width:80,
                    listeners:{
                        change:function(t,newValue){
                            jiangjieheshifang.items.each(function(item){
                                if(item.wrwName == '污染物6'){
                                    item.items.getAt(0).setValue(newValue);
                                }
                            });
                        }
                    }
                }]
            },{
                xtype:'fieldcontainer',
                defaults: {
                    labelWidth: 60,
                    submitValue:false
                },
                layout: {
                    type: 'hbox',
                    hideLabel: false,
                    defaultMargins: {
                        top: 0,
                        right: 5,
                        bottom: 5,
                        left: 0
                    }
                },
                items:[{
                    xtype:'checkbox',
                    name:'wrw7',
                    boxLabel:'污染物7',
                    width:80,
                    listeners:{
                        scope:this,
                        change:function(box,newValue){
                            if(newValue){
                                jiangjieheshifang.items.each(function(item){
                                    if(item.wrwName == '污染物7'){
                                        item.show();
                                    }
                                });
                            }else{
                                jiangjieheshifang.items.each(function(item){
                                    if(item.wrwName == '污染物7'){
                                        item.hide();
                                    }
                                });
                            }
                        }
                    }
                },{
                    xtype:'textfield',
                    name:'wrw7name',
                    value:'化学需氧量(COD)',
                    width:80,
                    listeners:{
                        change:function(t,newValue){
                            jiangjieheshifang.items.each(function(item){
                                if(item.wrwName == '污染物7'){
                                    item.items.getAt(0).setValue(newValue);
                                }
                            });
                        }
                    }
                }]
            }]
        });
        this.items = [{
            title: '基本信息',
            autoScroll: true,
            items: [{
                xtype: 'fieldcontainer',
                fieldLabel: '河段数',
                labelWidth:80,
                defaults: {
                    readOnly: true,
                    xtype: 'textfield'
                },
                layout: {
                    type: 'hbox',
                    hideLabel: false,
                    defaultMargins: {
                        top: 0,
                        right: 5,
                        bottom: 5,
                        left: 0
                    }
                },
                items: [{
                    name: 'sz_nr',
                    allowBlank: false,
                    hideLabel: true,
                    submitValue: false,
                    value: 2,
                    width: 60
                }, {
                    name: 'sz_nj',
                    allowBlank: false,
                    submitValue: false,
                    fieldLabel: '河道节点数',
                    value: 3,
                    labelWidth:80,
                    width: 140
                }, {
                    name: 'sz_njb',
                    allowBlank: false,
                    submitValue: false,
                    fieldLabel: '边界节点个数',
                    value: 2,
                    labelWidth:100,
                    width: 160
                }, {
                    name: 'sz_nl',
                    allowBlank: false,
                    submitValue: false,
                    fieldLabel: '分区个数',
                    value: 2,
                    labelWidth:80,
                    width: 140
                },{
                    name: 'sz_mt',
                    allowBlank: false,
                    submitValue: false,
                    fieldLabel: '月调节系数',
                    value: 1,
                    labelWidth: 100,
                    width: 160
                }]
            },{
                xtype: 'fieldcontainer',
                labelWidth: 230,
                fieldLabel: '好氧状态的溶氧限制（单位:mg/L）',
                defaults: {
                    labelWidth: 230,
                    xtype: 'textfield'
                },
                layout: {
                    type: 'hbox',
                    hideLabel: false,
                    defaultMargins: {
                        top: 0,
                        right: 5,
                        bottom: 5,
                        left: 0
                    }
                },
                items: [{
                    name: 'sz_GDO',
                    allowBlank: false,
                    submitValue: false,
                    hideLabel: true,
                    value: 1.0,
                    width: 100
                }, {
                    labelWidth: 250,
                    name: 'sz_Bdo',
                    allowBlank: false,
                    submitValue: false,
                    fieldLabel: '厌氧氧状态的溶氧限值（单位:mg/L）',
                    value: 0.2,
                    width: 350
                }]
            },{
                xtype: 'fieldcontainer',
                labelWidth: 100,
                fieldLabel: '投放量',
                defaults: {
                    labelWidth: 100,
                    xtype: 'textfield'
                },
                layout: {
                    type: 'hbox',
                    hideLabel: false,
                    defaultMargins: {
                        top: 0,
                        right: 5,
                        bottom: 5,
                        left: 0
                    }
                },
                items: [{
                    name: 'sz_JHxT',
                    allowBlank: false,
                    submitValue: false,
                    hideLabel: true,
                    width: 120
                },{
                    name: 'sz_shijiantype',
                    readOnly:true,
                    allowBlank: false,
                    submitValue: false,
                    fieldLabel: '时间序列类型',
                    value: '等分时间序列',
                    width: 220
                }]
            },{
                xtype: 'fieldcontainer',
                labelWidth: 100,
                fieldLabel: '开始时间',
                defaults: {
                    labelWidth: 100,
                    xtype: 'datefield'
                },
                layout: {
                    type: 'hbox',
                    hideLabel: false,
                    defaultMargins: {
                        top: 0,
                        right: 5,
                        bottom: 5,
                        left: 0
                    }
                },
                items: [{
                    name: 'sz_start',
                    format:'Y-m-d H:i:s',
                    allowBlank: false,
                    submitValue: false,
                    hideLabel: true,
                    width: 200,
                    listeners:{
                        scope:this,
                        change:function(t,newVale){
                            if(this.getForm().findField('sz_start').isValid() &&  this.getForm().findField('sz_end').isValid()){
                                var start = this.getForm().findField('sz_start').getValue().getTime();
                                var end = this.getForm().findField('sz_end').getValue().getTime();
                                var count = this.getForm().findField('sz_dt').getValue();
                                var step = 1;
                                if(this.getForm().findField('sz_shijianjiange').getValue() == 'DAYS'){
                                    step = 86400000;
                                }else if(this.getForm().findField('sz_shijianjiange').getValue() == 'HOUR'){
                                    step = 3600000;
                                }else if(this.getForm().findField('sz_shijianjiange').getValue() == 'SECD'){
                                    step=1000;
                                }
                                var total = (end - start)/(step*parseInt(count));
                                this.getForm().findField('sz_zongbushu').setValue(Math.ceil(total));
                            }
                        }
                    }
                }, {
                    name: 'sz_end',
                    format:'Y-m-d H:i:s',
                    allowBlank: false,
                    submitValue: false,
                    fieldLabel: '结束时间',
                    width: 300,
                    listeners:{
                        scope:this,
                        change:function(t,newVale){
                            if(this.getForm().findField('sz_start').isValid() &&  this.getForm().findField('sz_end').isValid()){
                                var start = this.getForm().findField('sz_start').getValue().getTime();
                                var end = this.getForm().findField('sz_end').getValue().getTime();
                                var count = this.getForm().findField('sz_dt').getValue();
                                var step = 1;
                                if(this.getForm().findField('sz_shijianjiange').getValue() == 'DAYS'){
                                    step = 86400000;
                                }else if(this.getForm().findField('sz_shijianjiange').getValue() == 'HOUR'){
                                    step = 3600000;
                                }else if(this.getForm().findField('sz_shijianjiange').getValue() == 'SECD'){
                                    step=1000;
                                }
                                var total = (end - start)/(step*parseInt(count));
                                this.getForm().findField('sz_zongbushu').setValue(Math.ceil(total));
                            }
                        }
                    }
                }]
            },{
                xtype: 'fieldcontainer',
                labelWidth: 100,
                fieldLabel: '计算时间步长',
                defaults: {
                    labelWidth: 100
                },
                layout: {
                    type: 'hbox',
                    hideLabel: false,
                    defaultMargins: {
                        top: 0,
                        right: 5,
                        bottom: 5,
                        left: 0
                    }
                },
                items: [{
                    xtype: 'textfield',
                    name: 'sz_dt',
                    allowBlank: false,
                    submitValue: false,
                    value: 1,
                    labelWidth: 110,
                    width: 180,
                    listeners:{
                        scope:this,
                        change:function(t,newVale){
                            if(this.getForm().findField('sz_start').isValid() &&  this.getForm().findField('sz_end').isValid()){
                                var start = this.getForm().findField('sz_start').getValue().getTime();
                                var end = this.getForm().findField('sz_end').getValue().getTime();
                                var count = this.getForm().findField('sz_dt').getValue();
                                var step = 1;
                                if(this.getForm().findField('sz_shijianjiange').getValue() == 'DAYS'){
                                    step = 86400000;
                                }else if(this.getForm().findField('sz_shijianjiange').getValue() == 'HOUR'){
                                    step = 3600000;
                                }else if(this.getForm().findField('sz_shijianjiange').getValue() == 'SECD'){
                                    step=1000;
                                }
                                var total = (end - start)/(step*parseInt(count));
                                this.getForm().findField('sz_zongbushu').setValue(Math.ceil(total));
                            }
                        }
                    }
                },{
                    xtype:'combo',
                    editable: false,
                    fieldLabel: '时间间隔',
                    valueField: 'value',
                    displayField: 'name',
                    submitValue: false,
                    allowBlank:false,
                    triggerAction: 'all',
                    store:Ext.create('Ext.data.ArrayStore',{
                        fields:['name','value'],
                        data:[['天','DAYS'],['时','HOUR'],['秒','SECD']]
                    }),
                    name: 'sz_shijianjiange',
                    hideLabel: true,
                    value:'DAYS',
                    width: 60,
                    listeners:{
                        scope:this,
                        change:function(t,newVale){
                            if(this.getForm().findField('sz_start').isValid() &&  this.getForm().findField('sz_end').isValid()){
                                var start = this.getForm().findField('sz_start').getValue().getTime();
                                var end = this.getForm().findField('sz_end').getValue().getTime();
                                var count = this.getForm().findField('sz_dt').getValue();
                                var step = 1;
                                if(this.getForm().findField('sz_shijianjiange').getValue() == 'DAYS'){
                                    step = 86400000;
                                }else if(this.getForm().findField('sz_shijianjiange').getValue() == 'HOUR'){
                                    step = 3600000;
                                }else if(this.getForm().findField('sz_shijianjiange').getValue() == 'SECD'){
                                    step=1000;
                                }
                                var total = (end - start)/(step*parseInt(count));
                                this.getForm().findField('sz_zongbushu').setValue(Math.ceil(total));
                            }
                        }
                    }
                }, {
                    xtype:'textfield',
                    name: 'sz_zongbushu',
                    allowBlank: false,
                    //hidden:true,
                    readOnly:true,
                    submitValue: false,
                    fieldLabel: '总步骤数',
                    width: 300
                }]
            }]
        }, {
            title: '打印断面信息',
            items: [heduan]
        }, {
            title: '污染源位置设定信息',
            items: [wuranyuan]
        },{
            title:'污染物信息',
            autoScroll: true,
            items:[wuranwu]
        },jiangjieheshifang];
        this.tbar = [{
            xtype: 'button',
            text: '保存此页数据',
            scope: this,
            handler: this.onSave
        }];
        this.callParent(arguments);
    },
    onSave: function () {
        if (this.getForm().isValid()) {
            var sz_nr = this.getForm().findField('sz_nr').getValue();
            var sz_nj = this.getForm().findField('sz_nj').getValue();
            var sz_njb = this.getForm().findField('sz_njb').getValue();
            var sz_nl = this.getForm().findField('sz_nl').getValue();
            var sz_dt = this.getForm().findField('sz_dt').getValue();
            var sz_mt = this.getForm().findField('sz_mt').getValue();
            var sz_shijiantype = this.getForm().findField('sz_shijiantype').getValue();
            var sz_JHxT = this.getForm().findField('sz_JHxT').getValue();
            var sz_start = Ext.util.Format.date(new Date(this.getForm().findField('sz_start').getValue().getTime()),'Y-m-d h:i:s');
            var sz_end = Ext.util.Format.date(new Date(this.getForm().findField('sz_end').getValue().getTime()),'Y-m-d h:i:s');
            var sz_shijianjiange = this.getForm().findField('sz_shijianjiange').getValue();
            var sz_zongbushu = this.getForm().findField('sz_zongbushu').getValue();


            var sz_GDO = this.getForm().findField('sz_GDO').getValue();
            var sz_Bdo = this.getForm().findField('sz_Bdo').getValue();

            var data = {};
            data.sz_nr = sz_nr;
            data.sz_nj = sz_nj;
            data.sz_njb = sz_njb;
            data.sz_nl = sz_nl;
            data.sz_dt = sz_dt;
            data.sz_mt = sz_mt;
            data.sz_shijiantype = sz_shijiantype;
            data.sz_JHxT = sz_JHxT;
            data.sz_start = sz_start;
            data.sz_end = sz_end;
            data.sz_shijianjiange = sz_shijianjiange;
            data.sz_zongbushu = sz_zongbushu;
            data.sz_GDO = sz_GDO;
            data.sz_Bdo = sz_Bdo;
            data.sz_model_sub_type = '2';

            //固定参数
            data.sz_nc = '7';
            data.sz_njpm = '0';
            data.sz_nspm = '0';
            data.sz_mpr = '1';
            data.sz_month='1,2,3,4,5,6,7,8,9,10,11,12';

            var sz_nspr = '';
            this.heduan.items.each(function(item){
                if(item.isContext){
                    var val = item.items.getAt(0).getValue()+','+item.items.getAt(1).getValue();
                    sz_nspr = sz_nspr + val+ ';';
                }
            });
            data.sz_nspr = sz_nspr.substr(0,sz_nspr.length-1);

            var sz_wry = '';
            this.wuranyuan.items.each(function(item){
                if(item.isContext){
                    var val = item.items.getAt(0).getValue()+','+item.items.getAt(1).getValue();
                    sz_wry = sz_wry + val+ ';';
                }
            });
            data.sz_wry = sz_wry.substr(0,sz_wry.length-1);
            var xishu = '';
            var wuranwuNames = '';
            var wuranwuIndex = '';
            for(var i=0;i<this.wuranwu.items.length;i++){
                var item = this.wuranwu.items.getAt(i);
                var checked = item.items.getAt(0).getValue();
                var wuranwuName = item.items.getAt(1).getValue();
                wuranwuNames = wuranwuNames +wuranwuName;
                if(i != this.wuranwu.items.length-1){
                    wuranwuNames = wuranwuNames +","
                }
                if (checked) {
                    wuranwuIndex = wuranwuIndex + (i + 1) + ',';
                } else {
                    wuranwuIndex = wuranwuIndex + 0 + ',';
                }
                if(checked){
                    var val1 ="";
                    var val2 ="";
                    for(var j=0;j<4;j++){
                        var v1 = this.jiangjieheshifang.items.getAt(i).items.getAt(1).items.getAt(0).items.getAt(j).getValue();
                        val1 = val1 + v1+',';
                        var v2 = this.jiangjieheshifang.items.getAt(i).items.getAt(1).items.getAt(1).items.getAt(j).getValue();
                        val2 = val2 + v2+',';
                    }
                    xishu =xishu + 'wrw'+(i+1)+','+ val1+val2.substr(0,val2.length-1)+';';
                }
            }
            data.sz_wuranwuNames = wuranwuNames;
            data.sz_wuranwuIndex = wuranwuIndex.substr(0, wuranwuIndex.length - 1);
            data.sz_JHxS = xishu.substr(0,xishu.length-1);
            this.getForm().submit({
                url: __path + '/inputview/addShunshiData.do',
                method: 'post',
                params:{
                    plan_id:this.plan_id,
                    jsonStr:JSON.stringify(data)
                },
                waitMsg: '正在保存，请稍候...',
                scope: this,
                success: function (form, action) {
                    if (action.result.success) {
                        Ext.Msg.alert('提示','保存成功');
                    } else {
                        Ext.Msg.alert('错误', action.result
                            ? action.result.message
                            : '保存时服务器出现错误，请与管理员联系。');
                    }
                    this.fireEvent('savesuccess', this, action.result);
                },
                failure: function (form, action) {
                    Ext.Msg.alert('错误', action.result
                        ? action.result.message
                        : '保存时服务器出现错误，请与管理员联系。');
                }
            });
        }
    },
    getWuRanWuConf:function(name1,name2,name3){
        return {
            xtype:'fieldcontainer',
            hidden:true,
            wrwName:name1,
            anchor: '98%',
            layout: {
                type: 'hbox',
            },
            items:[ {
                xtype:'displayfield',
                flex:1,
                value:name3
            },{
                xtype:'fieldcontainer',
                flex:8,
                layout: {
                    type: 'vbox',
                },
                items:[{
                    xtype:'fieldcontainer',
                    flex:1,
                    defaults: {
                        xtype: 'textfield',
                        submitValue: false
                    },
                    layout: {
                        type: 'hbox',
                        defaultMargins: {
                            top: 0,
                            right: 5,
                            bottom: 5,
                            left: 0
                        }
                    },
                    items:[{
                        name:name2+'_1',
                        width:200,
                        labelWidth:150,
                        fieldLabel:'好氧DO浓度',
                        flex:1
                    },{
                        name:name2+'_2',
                        width:200,
                        labelWidth:150,
                        fieldLabel:'厌氧DO浓度',
                        flex:1
                    },{
                        name:name2+'_3',
                        width:200,
                        labelWidth:150,
                        fieldLabel:'河道一的好氧降解系数',
                        flex:1
                    },{
                        name:name2+'_4',
                        width:200,
                        labelWidth:150,
                        fieldLabel:'河道二的好氧降解系数',
                        flex:1
                    }]
                },{
                    xtype:'fieldcontainer',
                    flex:1,
                    defaults: {
                        xtype: 'textfield',
                        submitValue: false
                    },
                    layout: {
                        type: 'hbox',
                        defaultMargins: {
                            top: 0,
                            right: 5,
                            bottom: 5,
                            left: 0
                        }
                    },
                    items:[{
                        name:name2+'_5',
                        width:200,
                        labelWidth:150,
                        fieldLabel:'河道一的厌氧降解系数',
                        flex:1
                    },{
                        name:name2+'_6',
                        width:200,
                        labelWidth:150,
                        fieldLabel:'河道二的厌氧降解系数',
                        flex:1
                    },{
                        name:name2+'_7',
                        width:200,
                        labelWidth:150,
                        fieldLabel:'河道一的底泥释放系数',
                        flex:1
                    },{
                        name:name2+'_8',
                        width:200,
                        labelWidth:150,
                        fieldLabel:'河道二的底泥释放系数',
                        flex:1
                    }]
                }]
            }]
        };
    },
    setSzpjSsValue:function (data) {
        {
            var base = data.jibenxinxi;
            this.getForm().findField('sz_Bdo').setValue(base['sz_Bdo']);
            this.getForm().findField('sz_GDO').setValue(base['sz_GDO']);
            this.getForm().findField('sz_dt').setValue(base['sz_dt']);
            //this.getForm().findField('sz_mt').setValue(base['sz_mt']);
            //this.getForm().findField('sz_nj').setValue(base['sz_nj']);
            //this.getForm().findField('sz_njb').setValue(base['sz_njb']);
            //this.getForm().findField('sz_nl').setValue(base['sz_nl']);
            //this.getForm().findField('sz_nr').setValue(base['sz_nr']);

            this.getForm().findField('sz_JHxT').setValue(base['sz_JHxT']);
            //this.getForm().findField('sz_shijiantype').setValue(base['sz_shijiantype']);
            this.getForm().findField('sz_start').setValue(base['sz_start']);
            this.getForm().findField('sz_end').setValue(base['sz_end']);
            this.getForm().findField('sz_shijianjiange').setValue(base['sz_shijianjiange']);
            this.getForm().findField('sz_zongbushu').setValue(base['sz_zongbushu']);

            this.heduan.setHdValue(data.sz_nspr);
            this.wuranyuan.setHdValue(data.sz_wry);

            var names = [];
            if(data.sz_wuranwuNames){
                names =data.sz_wuranwuNames.split(',');
            }
            var indexs = [];
            if(data.sz_wuranwuIndex){
                indexs = data.sz_wuranwuIndex.split(',');
            }
            this.wuranwu.items.each(function (item,index) {
                if(indexs.indexOf((index+1)+'')>-1){
                    item.items.getAt(0).setValue(true);
                }else{
                    item.items.getAt(0).setValue(false);
                }
                if(names[index]){
                    item.items.getAt(1).setValue(names[index]);
                }
            });
            var wurwuJj = [];
            if(data.sz_JHxS){
                wurwuJj = data.sz_JHxS.split(";");
            }
            this.jiangjieheshifang.items.each(function (item,index) {
                if(indexs.indexOf((index+1)+'')>-1 && wurwuJj){
                    var jj = wurwuJj.shift();
                    if(jj){
                        var data = jj.split(',');
                        for(var i=0;i<4;i++){
                            item.items.getAt(1).items.getAt(0).items.getAt(i).setValue(data[i+1]);
                            item.items.getAt(1).items.getAt(1).items.getAt(i).setValue(data[i+5]);
                        }
                    }
                }
            });
        }

    }
});
