Ext.define('bitc.wrenv.tfsj.uploadPhoto', {
    extend: 'Ext.form.Panel',
    bodyPadding: "5",
    defaultType: "textfield",
    //enctype : 'multipart/form-data',
    fileUpload: true,
    fieldDefaults: {labelAlign: "left", labelWidth: 70},
    initComponent: function () {
        this.items = [
            {
                id: 'photo',
                name: 'photo',
                fieldLabel: '照片',
                xtype: 'filefield',
                fileUpload: true,
                buttonText : '选择照片',
                anchor: '40%',
                listeners: {//监听事件
                    'render': function () {//读取
                        Ext.getCmp('photo').on('change', function () {//上传图片的控件对象,刚刚选择的图片仿真路径，上次选择的图片仿真路径
                            var img_reg = /\.([jJ][pP][gG]){1}$|\.([jJ][pP][eE][gG]){1}$|\.([gG][iI][fF]){1}$|\.([pP][nN][gG]){1}$|\.([bB][mM][pP]){1}$/
                            var path = Ext.getCmp('photo').getValue();
                            var url = 'file:///' + path;
                            if (!img_reg.test(url)) {
                                Ext.Msg.alert('提示信息','您选择的文件格式不正确,请重新选择!');
                                Ext.getCmp('photo').setRawValue('');
                                Ext.getCmp('browseImage').getEl().dom.src = '';
                                return;
                            }
                            if (Ext.isIE) {

                            } else {//获取选择文件的路径信息？ 将路径绑定到显示图片的box内加载
                                var obj = Ext.getCmp('photo').fileInputEl.dom.files;
                                var len = obj.length;//选文件的数量
                                if (len < 1) {
                                    return;
                                }
                                var imgurl = window.URL.createObjectURL(obj[0]);
                                Ext.getCmp('browseImage').getEl().dom.src = imgurl;
                            }
                        }, this);//这是选择文件
                    }
                }
            },
            {
                xtype: 'box', //或者xtype: 'component',
                width: '100%', //图片宽度
                height: 200, //图片高度
                fieldLabel: "预览图片",
                id: 'browseImage',
                autoEl: {
                    tag: 'img',    //指定为img标签
                    src: Ext.BLANK_IMAGE_URL,//指定路径
                    id: 'imageBrowse'
                }
            }
        ];
        this.callParent(arguments);
    }
});