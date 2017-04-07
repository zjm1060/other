Ext.define('bitc.wrenv.inputview.inputYuQiao_zhukong',{
    extend: 'Ext.form.Panel',
    autoScroll: true,
    border: false,
    title: '主控参数',
    planId:null,
    defaults: {
        anchor: '98%',
        labelWidth: 80
    },
    initComponent: function () {
        this.tbar=[{
            xtype:'button',
            text:'保存',
            scope:this,
            handler:function(){
                this.onSave();
            }
        }];
        this.items=[{
            xtype:'fieldcontainer',
            defaults: {
                xtype: 'radiogroup',
                labelWidth: 90,
                width:220
            },
            layout: {
                type: 'hbox',
                defaultMargins: {
                    top: 0,
                    right: 5,
                    bottom: 5,
                    left: 10
                }
            },
            items:[{
                name: 'IFEGROUP',
                fieldLabel: '温度模块激活',
                name:'C6_TEM_ISTRAN_GROUP',
                allowBlank: false,
                submitValue:false,
                items: [{boxLabel: '激活', name: 'C6_TEM_ISTRAN',submitValue:false, inputValue: '1', checked: true}, {
                    boxLabel: '不激活',
                    name: 'C6_TEM_ISTRAN',
                    submitValue:false,
                    inputValue: '0'
                }]
            }, {
                fieldLabel: '染料模块激活',
                name: 'C6_DYE_ISTRAN_GROUP',
                allowBlank: false,
                submitValue: false,
                items: [{boxLabel: '激活', name: 'C6_DYE_ISTRAN', submitValue: false, inputValue: '1', checked: true}, {
                    boxLabel: '不激活',
                    name: 'C6_DYE_ISTRAN',
                    submitValue: false,
                    inputValue: '0'
                }]
            }]
        },{
            xtype:'fieldcontainer',
            defaults: {
                xtype: 'textfield',
                labelWidth: 90,
                width:150
            },
            layout: {
                type: 'hbox',
                defaultMargins: {
                    top: 0,
                    right: 5,
                    bottom: 5,
                    left: 10
                }
            },
            items:[{
                fieldLabel:'NPBS',
                name:'C16_NPBS',
                allowBlank: false,
                submitValue: false,
                value:0
            },{
                fieldLabel:'NPBW',
                name:'C16_NPBW',
                allowBlank: false,
                submitValue: false,
                value:0
            },{
                fieldLabel:'NPBE',
                name:'C16_NPBE',
                allowBlank: false,
                submitValue: false,
                value:0
            },{
                fieldLabel:'NPBN',
                name:'C16_NPBN',
                allowBlank: false,
                submitValue: false,
                value:0
            }]
        },{
            xtype:'fieldcontainer',
            defaults: {
                xtype: 'textfield',
                labelWidth: 90,
                width:150
            },
            layout: {
                type: 'hbox',
                defaultMargins: {
                    top: 0,
                    right: 5,
                    bottom: 5,
                    left: 10
                }
            },
            items:[{
                fieldLabel:'NPFOR',
                name:'C16_NPFOR',
                allowBlank: false,
                submitValue: false,
                value:0
            },{
                fieldLabel:'NPFORT',
                name:'C16_NPFORT',
                allowBlank: false,
                submitValue: false,
                value:0
            },{
                fieldLabel:'NPSER',
                name:'C16_NPSER',
                allowBlank: false,
                submitValue: false,
                value:0
            },{
                fieldLabel:'PDGINIT',
                name:'C16_PDGINIT',
                allowBlank: false,
                submitValue: false,
                value:0
            }]
        },{
            xtype:'fieldcontainer',
            defaults: {
                xtype: 'textfield',
                labelWidth: 90,
                width:150
            },
            layout: {
                type: 'hbox',
                defaultMargins: {
                    top: 0,
                    right: 5,
                    bottom: 5,
                    left: 10
                }
            },
            items:[{
                fieldLabel:'模拟天数',
                name:'C7_NTC',
                allowBlank: false,
                submitValue: false,
                value:365
            },{
                fieldLabel:'边界网格I索引',
                name:'C18_IPBS',
                allowBlank: false,
                submitValue: false,
                value:0
            },{
                fieldLabel:'边界网格J索引',
                name:'C18_JPBS',
                allowBlank: false,
                submitValue: false,
                value:0
            },{
                fieldLabel:'ISPBS',
                name:'C18_ISPBS',
                allowBlank: false,
                submitValue: false,
                value:0
            }]
        },{
            xtype:'fieldcontainer',
            defaults: {
                xtype: 'textfield',
                labelWidth: 90,
                width:150
            },
            layout: {
                type: 'hbox',
                defaultMargins: {
                    top: 0,
                    right: 5,
                    bottom: 5,
                    left: 10
                }
            },
            items:[{
                fieldLabel:'NPSERS',
                name:'C18_NPFORS',
                allowBlank: false,
                submitValue: false,
                value:0
            },{
                fieldLabel:'NPFORS',
                name:'C18_NPSERS',
                allowBlank: false,
                submitValue: false,
                value:0
            },{
                fieldLabel:'RKDYE',
                name:'C46_RKDYE_head',
                allowBlank: false,
                submitValue: false,
                value:'1.157'
            },{
                xtype:'displayfield',
                value:'E',
                width:8
            },{
                name:'C46_RKDYE_tail',
                allowBlank: false,
                submitValue: false,
                width:60,
                value:'-07'
            }]
        },{
            xtype:'fieldcontainer',
            defaults: {
                xtype: 'textfield',
                width:150
            },
            layout: {
                type: 'hbox',
                defaultMargins: {
                    top: 0,
                    right: 5,
                    bottom: 5,
                    left: 10
                }
            },
            items:[{
                fieldLabel:'初始温度',
                name:'temp_dafault',
                allowBlank: false,
                value:1
            },{
                fieldLabel:'污染物初始浓度',
                name:'dye_dafault',
                allowBlank: false,
                value:0
            }]
        }];
        this.callParent(arguments);
    },
    onSave:function(){
        var baseForm = this.getForm();
        var C6_TEM_ISTRAN = baseForm.findField('C6_TEM_ISTRAN_GROUP').getValue()[0];
        var C6_DYE_ISTRAN = baseForm.findField('C6_DYE_ISTRAN_GROUP').getValue()[0];
        var C7_NTC = baseForm.findField('C7_NTC').getValue();
        var C16_NPBS = baseForm.findField('C16_NPBS').getValue();
        var C16_NPBW = baseForm.findField('C16_NPBW').getValue();
        var C16_NPBE = baseForm.findField('C16_NPBE').getValue();
        var C16_NPBN = baseForm.findField('C16_NPBN').getValue();
        var C16_NPFOR = baseForm.findField('C16_NPFOR').getValue();
        var C16_NPFORT = baseForm.findField('C16_NPFORT').getValue();
        var C16_NPSER = baseForm.findField('C16_NPSER').getValue();
        var C16_PDGINIT = baseForm.findField('C16_PDGINIT').getValue();
        var C18_IPBS = baseForm.findField('C18_IPBS').getValue();
        var C18_JPBS = baseForm.findField('C18_JPBS').getValue();
        var C18_ISPBS = baseForm.findField('C18_ISPBS').getValue();
        var C46_RKDYE = baseForm.findField('C46_RKDYE_head').getValue()+'E'+baseForm.findField('C46_RKDYE_tail').getValue();
        var C18_NPSERS = baseForm.findField('C18_NPSERS').getValue();
        var C18_NPFORS = baseForm.findField('C18_NPFORS').getValue();
        var temp_dafault = baseForm.findField('temp_dafault').getValue();
        var dye_dafault = baseForm.findField('dye_dafault').getValue();
        var data = {};
        data['C6_TEM_ISTRAN']=C6_TEM_ISTRAN;
        data['C6_DYE_ISTRAN']=C6_DYE_ISTRAN;
        data['C7_NTC']=C7_NTC;
        data['C16_NPBS']=C16_NPBS;
        data['C16_NPBW']=C16_NPBW;
        data['C16_NPBE']=C16_NPBE;
        data['C16_NPBN']=C16_NPBN;
        data['C16_NPFOR']=C16_NPFOR;
        data['C16_NPFORT']=C16_NPFORT;
        data['C16_NPSER']=C16_NPSER;
        data['C16_PDGINIT']=C16_PDGINIT;
        data['C18_IPBS']=C18_IPBS;
        data['C18_JPBS']=C18_JPBS;
        data['C18_ISPBS']=C18_ISPBS;
        data['C18_NPFORS']=C18_NPFORS;
        data['C18_NPSERS']=C18_NPSERS;
        data['C46_RKDYE']=C46_RKDYE;
        data['temp_dafault']=temp_dafault;
        data['dye_dafault']=dye_dafault;
        //data['C22_NCSER2']='1';
        //data['C22_NCSER3']='1';
        //data['C23_NQSIJ']='1';
        //data['C23_NQSER']='1';

        if (baseForm.isValid()) {
            Ext.Ajax.request({
                url:__path+'/inputview/addKVData.do',
                method: 'POST',
                params: {
                    planId:this.planId,
                    data: JSON.stringify(data)
                },
                scope:this,
                success: function (response) {
                    var result = Ext.decode(response.responseText);
                    if (result.success) {
                        this.requestDataFromDB();
                        Ext.Msg.alert('提示','保存成功！');
                    } else {
                        Ext.Msg.alert('提示','保存失败，请重试！');
                    }
                },
                failure: function () {
                    callback.call(scope || window, false);
                }
            });
        }else{
            Ext.Msg.alert('提示','请检查输入项是否正确！');
        }
    },
    requestDataFromDB:function(){
        var keys =['C6_TEM_ISTRAN','C6_DYE_ISTRAN','C7_NTC','C16_NPBS','C16_NPBW','C16_NPBE','C16_NPBN','C16_NPFOR','C16_NPFORT','C16_NPSER','C16_PDGINIT',
            'C18_IPBS','C18_JPBS','C18_ISPBS','C18_NPFORS','C18_NPSERS','C46_RKDYE','temp_dafault','dye_dafault'];
        Ext.Ajax.request({
            url:__path+'/inputview/getYuQiaoInputData.do',
            method:'POST',
            scope:this,
            params:{
                planId:this.planId,
                keys:keys.join('#')
            },
            success:function(response){
                var result = Ext.decode(response.responseText);
                if (result.success) {
                    var data = result.attributes.data;
                    if(data){
                        var fields =['C7_NTC','C16_NPBS','C16_NPBW','C16_NPBE','C16_NPBN','C16_NPFOR','C16_NPFORT','C16_NPSER','C16_PDGINIT',
                        'C18_IPBS','C18_JPBS','C18_ISPBS','C18_NPFORS','C18_NPSERS','temp_dafault','dye_dafault'];
                        for(var i=0;i<fields.length;i++){
                            if(data[fields[i]]){
                                this.getForm().findField(fields[i]).setValue(data[fields[i]]);
                            }
                        }
                        if(data['C6_TEM_ISTRAN']){
                            this.getForm().findField('C6_TEM_ISTRAN_GROUP').setValue(data['C6_TEM_ISTRAN']);
                        }
                        if(data['C6_DYE_ISTRAN']){
                            this.getForm().findField('C6_DYE_ISTRAN_GROUP').setValue(data['C6_DYE_ISTRAN']);
                        }
                        if(data['C46_RKDYE']){
                            this.getForm().findField('C46_RKDYE_head').setValue(data['C46_RKDYE'].split('E')[0]);
                            this.getForm().findField('C46_RKDYE_tail').setValue(data['C46_RKDYE'].split('E')[1]);
                        }
                    }
                }
            }
        })
    },
    afterFirstLayout: function() {
        if(this.planId){
            this.requestDataFromDB();
        }
        this.callParent(arguments);
    }
});