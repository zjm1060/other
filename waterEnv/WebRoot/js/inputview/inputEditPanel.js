Ext.define('bitc.wrenv.inputview.inputEditPanel', {
    extend: 'Ext.tab.Panel',
    border: false,
    plan_id: null,
    water_body_id: null,
    padding: '5 5 5 5',
    bodyPadding: 10,
    initComponent: function () {
        var me = this;
        var duanmian = this.duanmian = Ext.create('bitc.wrenv.inputview.inputDuanmianPanel');
        var hedao = this.hedao = Ext.create('bitc.wrenv.inputview.inputHedaoPanel');
        var zhiliu = this.zhiliu = Ext.create('bitc.wrenv.inputview.inputZhiliuPanel', {water_body_id: this.water_body_id});
        var zhamen = this.zhamen = Ext.create('bitc.wrenv.inputview.inputZhaMenPanel');
        var shuiku = this.shuiku = Ext.create('bitc.wrenv.inputview.inputShuikuPanel', {water_body_id: this.water_body_id});
        var hewangdixing = this.hewangdixing = Ext.create('Ext.form.Panel', {
            border: false,
            autoScroll: true,
            maxHeight:550,
            title: '河网地形参数',
            layout: 'anchor',
            defaults: {
                xtype: 'fieldset',
                anchor: '98%',
                layout: 'anchor',
                padding: '5 5 5 5',
                bodyPadding: 10
            },
            items: [{
                title: '断面参数',
                items: [duanmian]
            }, {
                title: '河道参数',
                items: [hedao]
            }, {
                title: '支流参数',
                items: [zhiliu]
            }, {
                title: '闸门水位流量曲线',
                items: [zhamen]
            }, {
                title: '水库参数',
                items: [shuiku]
            }],
            tbar: [{
                xtype: 'button',
                text: '保存此页数据',
                scope: this,
                handler: this.onHWDXSave
            }]
        });
        var bianjie = this.bianjie = Ext.create('bitc.wrenv.inputview.inputBianjieEditPanel', {
            columnWidth: 0.3, water_body_id: this.water_body_id,plan_id:this.plan_id,
            margin:'10 0 20 0',
            listeners: {
                scope:this,
                'showgriddata': function (rawValue, value) {
                        if(value && this.gridData[value]){
                            this.gridstore.loadData(this.gridData[value]);
                        }else{
                            var year = this.up().items.getAt(0).getForm().findField('run_year').getValue();
                            this.bianjieReload(year);
                        }
                },
                'uploadfiledata':function(){
                    if (this.bianjie.getForm().isValid()) {
                        var gridData = [];
                        me.sdlparams.items.getAt(1).getStore().each(function (record) {
                            gridData.push({date: record.get('date'), value: record.get('value')});
                        });
                        this.bianjie.getForm().submit({
                            url: __path + '/inputview/addSdlParams.do',
                            method: 'post',
                            params: {
                                plan_id: me.plan_id,
                                jsonStr: JSON.stringify(gridData)
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
                                debugger;
                                Ext.Msg.alert('错误', action.result
                                    ? action.result.message
                                    : '保存时服务器出现错误，请与管理员联系。');
                            }
                        });
                    }
                }
            }
        });
        var bianjie1 = this.bianjie1 = Ext.create('bitc.wrenv.inputview.inputBianjieEditPanel1', {
            columnWidth: 0.3, water_body_id: this.water_body_id,plan_id:this.plan_id,
            listeners: {
                scope:this,
                'showgriddata': function () {
                        if(this.gridData['TaiZiHQ.dat']){
                            this.gridstore.loadData(this.gridData['TaiZiHQ.dat']);
                        }else{
                            var year = this.up().items.getAt(0).getForm().findField('run_year').getValue();
                            this.bianjieReload(year);
                        }
                },
                'uploadfiledata':function(){
                    if (this.bianjie1.getForm().isValid()) {
                        var gridData = [];
                        me.sdlparams.items.getAt(1).getStore().each(function (record) {
                            gridData.push({date: record.get('date'), value: record.get('value')});
                        });
                        this.bianjie1.getForm().submit({
                            url: __path + '/inputview/addSdlParams.do',
                            method: 'post',
                            params: {
                                plan_id: me.plan_id,
                                jsonStr: JSON.stringify(gridData)
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
                                debugger;
                                Ext.Msg.alert('错误', action.result
                                    ? action.result.message
                                    : '保存时服务器出现错误，请与管理员联系。');
                            }
                        });
                    }
                }
            }
        });
        var bianjie2 = this.bianjie2 = Ext.create('bitc.wrenv.inputview.inputBianjieEditPanel2', {
            columnWidth: 0.3, water_body_id: this.water_body_id,plan_id:this.plan_id,
            margin:'0 0 20 0',
            listeners: {
                scope:this,
                'showgriddata': function () {
                        if(this.gridData['TaiZiHQ.dat_shuiwei']){
                            this.gridstore.loadData(this.gridData['TaiZiHQ.dat_shuiwei']);
                        }else{
                            var year = this.up().items.getAt(0).getForm().findField('run_year').getValue();
                            this.bianjieReload(year);
                        }
                },
                'uploadfiledata':function(){
                    if (this.bianjie2.getForm().isValid()) {
                        var gridData = [];
                        me.sdlparams.items.getAt(1).getStore().each(function (record) {
                            gridData.push({date: record.get('date'), value: record.get('value')});
                        });
                        this.bianjie2.getForm().submit({
                            url: __path + '/inputview/addSdlParams.do',
                            method: 'post',
                            params: {
                                plan_id: me.plan_id,
                                jsonStr: JSON.stringify(gridData)
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
                                debugger;
                                Ext.Msg.alert('错误', action.result
                                    ? action.result.message
                                    : '保存时服务器出现错误，请与管理员联系。');
                            }
                        });
                    }
                }
            }
        });
        var bianjieEditPanel = this.bianjieEditPanel = Ext.create('Ext.form.Panel',{
            border: false,
            columnWidth: 0.4,
            items:[ {
                title: '上边界设置',
                anchor: '80%',
                autoScroll: true,
                maxHeight: 300,
                xtype: 'fieldset',
                layout: 'anchor',
                padding: '5 5 5 5',
                bodyPadding: 10,
                items: [bianjie1]
            },{
                title: '下边界设置',
                anchor: '80%',
                autoScroll: true,
                maxHeight: 300,
                xtype: 'fieldset',
                layout: 'anchor',
                padding: '5 5 5 5',
                bodyPadding: 10,
                items: [bianjie2]
            },{
                title: '支流边界设置',
                anchor: '80%',
                autoScroll: true,
                maxHeight: 300,
                xtype: 'fieldset',
                layout: 'anchor',
                padding: '5 5 5 5',
                bodyPadding: 10,
                items: [bianjie]
            }]
        });
        var gridstore = this.gridstore = Ext.create('Ext.data.Store', {
            fields: ['date', 'value'],
            data: {'items': []},
            proxy: {
                type: 'memory',
                reader: {
                    type: 'json',
                    root: 'items'
                }
            }
        });
        var grid = this.grid = Ext.create('Ext.grid.Panel', {
            store: gridstore,
            columns: [
                {xtype: 'rownumberer', width: 40},
                {text: '日期', dataIndex: 'date', width: 150},
                {
                    text: '值', dataIndex: 'value', flex: 1, editor: {
                    xtype: 'textfield',
                    saveBtnText: '保存',
                    cancelBtnText: "取消",
                    allowBlank: false
                }
                }
            ],
            height: 550,
            plugins: [
                Ext.create('Ext.grid.plugin.CellEditing', {
                    clicksToEdit: 1
                })
            ],
            autoScroll: true,
            columnWidth: 0.4
        });
        var sdlparams = this.sdlparams = Ext.create('Ext.form.Panel', {
            border: false,
            title: '水动力参数',
            maxHeight:600,
            width:900,
            autoScroll: true,
            layout: 'column',
            items: [bianjieEditPanel, grid]
        });
        var szpjparams = this.szpjparams = Ext.create('bitc.wrenv.inputview.inputSzpjEditPanel', {
            autoScroll: true,
            maxHeight: 700,
            plan_id: this.plan_id
        });
        this.items = [hewangdixing, sdlparams, szpjparams];
        this.callParent(arguments);
    },
    bianjieReload: function (year) {
        if (year >= 1970) {
            var data = [];
            var start = year + '-01-01';
            var end = year + '-12-31';
            for (var i = Date.parse(start); i <= Date.parse(end); i += 86400000) {
                data.push({date: Ext.util.Format.date(new Date(i), 'Y-m-d'), value: ''});
            }
            this.gridstore.loadData(data);
        }
    },
    onHWDXSave: function () {
        if (this.hewangdixing.getForm().isValid()) {
            var duanmian = this.duanmian.getForm();
            var jsonStr = {};
            jsonStr['IDAN'] = duanmian.findField('IDAN').getValue();
            jsonStr['IGORY'] = duanmian.findField('IGORY').getValue();
            jsonStr['IE'] = duanmian.findField('IE').getValue();
            jsonStr['ISHI'] = duanmian.findField('ISHI').getValue();
            jsonStr['NJ'] = duanmian.findField('NJ').getValue();

            var hedao = this.hedao.getForm();
            jsonStr['ICLDNL'] = hedao.findField('ICL').getValue() + ',' + hedao.findField('DNL').getValue();
            jsonStr['IFE'] = hedao.findField('IFEGROUP').getValue()[0];
            jsonStr['IFFP'] = hedao.findField('IFFPGROUP').getValue()[0];
            jsonStr['IFHN'] = hedao.findField('IFHNGROUP').getValue()[0];
            jsonStr['ITEIKO'] = hedao.findField('ITEIKO').getValue();
            jsonStr['BAABAASA'] = hedao.findField('BAA').getValue() + ',' + hedao.findField('BAASA').getValue();

            jsonStr['ICHECK'] = '1';

            var zhiliuitems = this.zhiliu.items;

            if (zhiliuitems.length > 1) {
                jsonStr['IFR'] = '1';
            } else {
                jsonStr['IFR'] = '0';
            }

            var zhiliu_name_list = '';
            var zhiliu_dm_list = '';
            var zhiliu_cof = '';
            var zhiliu = this.zhiliu.getForm();
            var zhiliuArray = [];
            var flag = false;
            var mulitValue;
            zhiliuitems.each(function (item) {
                if (item.items.length > 1) {
                    var zl = item.items.getAt(0);
                    var mianji = item.items.getAt(1).getValue();
                    if(isNaN(mianji) || mianji == ''){
                        mianji = 0;
                    }
                    var xishu = item.items.getAt(2).getValue();
                    if(isNaN(xishu) || xishu == ''){
                        xishu = 0;
                    }
                    if(zhiliuArray.indexOf(zl.getValue())>=0){
                        flag = true;
                        mulitValue = zl.getRawValue();
                    }
                    zhiliuArray.push(zl.getValue());
                    zhiliu_name_list = zhiliu_name_list + zl.getValue().split(',')[1] + ',';
                    zhiliu_dm_list = zhiliu_dm_list + zl.getValue().split(',')[0] + ',';
                    zhiliu_cof = zhiliu_cof + (mianji+ ',' + xishu) + ';';
                }
            });
            if(flag){
                Ext.Msg.alert('提示',mulitValue+'存在多个，请检查后重试');
                return false;
            }
            zhiliu_name_list = zhiliu_name_list.substring(0, zhiliu_name_list.length - 1);
            zhiliu_dm_list = zhiliu_dm_list.substring(0, zhiliu_dm_list.length - 1);
            zhiliu_cof = zhiliu_cof.substring(0, zhiliu_cof.length - 1);
            jsonStr['ZHILIU_NAME_LIST'] = zhiliu_name_list;
            jsonStr['ZHILIU_DM_LIST'] = zhiliu_dm_list;
            jsonStr['ZHILIU_COF'] = zhiliu_cof;
            jsonStr['NR'] = zhiliuitems.length - 1;


            var zhamenitems = this.zhamen.items;
            var zhamen_list = '';
            var zhamenArray =[];
            if (zhamenitems.length > 1) {
                jsonStr['IFC'] = '1';
            } else {
                jsonStr['IFC'] = '0';
            }
            flag = false;
            zhamenitems.each(function (item) {
                if (item.items.length > 1) {
                    var zm = item.items.getAt(0);
                    var xishu = item.items.getAt(1);
                    if(zhamenArray.indexOf(zm.getValue())>=0){
                        flag = true;
                        mulitValue = zm.getRawValue();
                    }
                    zhamenArray.push(zm.getValue());
                    zhamen_list = zhamen_list + (zm.getValue() + "," + xishu.getValue()) + ';';
                }
            });
            if(flag){
                Ext.Msg.alert('提示',mulitValue+'存在多个，请检查后重试');
                return false;
            }
            zhamen_list = zhamen_list.substring(0, zhamen_list.length - 1);
            jsonStr['ZHAMEN_LIST'] = zhamen_list;
            jsonStr['ZHAMEN_COUNT'] = zhamenitems.length - 1;

            var shuikuitems = this.shuiku.items;
            var shuiku_list = '';
            var shuikuArray = [];
            jsonStr['NRESV'] = (shuikuitems.length - 1) + '';
            flag = false;
            shuikuitems.each(function (item) {
                if (item.items.length > 1) {
                    var sk = item.items.getAt(0);
                    var sw = item.items.getAt(1);
                    if(shuikuArray.indexOf(sk.getValue())>=0){
                        flag = true;
                        mulitValue = sk.getRawValue();
                    }
                    shuikuArray.push(sk.getValue());
                    shuiku_list = shuiku_list + (sk.getValue() + "," + sw.getValue()) + ';';
                }
            });
            if(flag){
                Ext.Msg.alert('提示',mulitValue+'存在多个，请检查后重试');
                return false;
            }
            shuiku_list = shuiku_list.substring(0, shuiku_list.length - 1);
            jsonStr['SHUIKU_LIST'] = shuiku_list;

            jsonStr['NQT'] = '1';
            jsonStr['IQT'] = '1';
            jsonStr['NCDH'] = '1';
            jsonStr['IDADHH'] = '1,2.0';
            jsonStr['NCL'] = '1';

            jsonStr['IHDOWN'] = '0';
            jsonStr['IFZB'] = '1';
            jsonStr['INQB'] = '1';
            jsonStr['INAR'] = '1';
            jsonStr['SSTO'] = '0.01';

            jsonStr['NCMAX'] = '100';
            jsonStr['DTBMIN'] = '30.0';
            jsonStr['RAMDAB'] = '0.4';
            jsonStr['RAMDAS'] = '0.4';
            jsonStr['RAMDAW'] = '0.7';

            jsonStr['ALF'] = '1.0';
            jsonStr['CKAPPA'] = '0.4';
            jsonStr['S'] = '2.65';

            this.hewangdixing.getForm().submit({
                url: __path + '/inputview/addHeWangDiXing.do',
                method: 'post',
                params: {
                    plan_id: this.plan_id,
                    jsonStr: JSON.stringify(jsonStr)
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
        } else {
            Ext.Msg.alert('提示', '请检查输入项是否正确！');
        }
    },
    onSDLSave: function () {
        var me = this;
        if (this.sdlparams.getForm().isValid()) {
            var gridData = [];
            me.sdlparams.items.getAt(1).getStore().each(function (record) {
                gridData.push({date: record.get('date'), value: record.get('value')});
            });
            this.sdlparams.getForm().submit({
                url: __path + '/inputview/addSdlParams.do',
                method: 'post',
                params: {
                    plan_id: me.plan_id,
                    jsonStr: JSON.stringify(gridData)
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
                    debugger;
                    Ext.Msg.alert('错误', action.result
                        ? action.result.message
                        : '保存时服务器出现错误，请与管理员联系。');
                }
            });
        }
    },
    setHeWangDiXing: function (data) {
        this.duanmian.setDmValue(data.duanmian);
        this.hedao.setHdValue(data.hedao);
        this.zhiliu.setZlValue(data.zhiliu);
        this.zhamen.setZmValue(data.zhamen);
        this.shuiku.setSkValue(data.shuiku);
    },
    setSdlValue: function (data) {
        this.gridData = data;
    },
    setSzpjCgValue: function (data1) {
        this.szpjparams.changgui.setSzpjCgValue(data1);
    },
    setSzpjCgGridValue:function(data){
        this.szpjparams.changgui.setSzpjCgGridValue(data);
    },
    setSzpjSsValue: function (data) {
        this.szpjparams.shunshi.setSzpjSsValue(data);
    },
    setSzpjCgOrSs:function (data) {
        this.szpjparams.getForm().findField('model_sub_type_group').setValue(data);
    },
    afterFirstLayout:function () {
        this.callParent(arguments);
        this.szpjparams.changgui.doLayout();
    }
});
