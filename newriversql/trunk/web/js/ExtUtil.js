// 处理超时及信息提示
Ext.Ajax.on('requestcomplete', function (conn, response, options) {
    try {
        var json = Ext.decode(response.responseText);
        if (json && !json.success && json.timeout) {
            // window.location.href = json.redirectUri;
            Ext.create('bitc.plan.view.LoginWindow', {
                modal: true,
                listeners: {
                    login: function (w) {
                        w.close();
                    }
                }
            }).show();
        }

        if (json && !json.success) {
            if (Ext.Msg.isVisible()) {
                alert('json.error');
            } else {
                Ext.Msg.alert('错误', json.error);
            }
            return false;
        }

    } catch (e) {
    }
});