Ext.require(['Ext.window.*', 'Ext.form.*', 'bitc.wrenv.LoginWindow']);
Ext.onReady(function() {
    Ext.QuickTips.init();
    Ext.create('bitc.wrenv.LoginWindow', {
        modal : false,
        listeners : {
            login : function(w, result) {
                w.close();
                if (__path == "") {
                    window.location.href = "/";
                } else {
                    window.location.href = __path;
                }
            }
        }
    }).show();
});