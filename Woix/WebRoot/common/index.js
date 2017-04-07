Ext.onReady(function() {
	/**
	 * 判断是否需要登录界面
	 */
	var url = contextPath +	"WebRoot/login.html";
	Ext.Ajax.request({
		url : contextPath + 'WoixService.asmx/GetConfigValue',
		method : 'post',
		params : {
			paramName : 'Woix.Login'
		},
		success : function(response, opts) {
			var flag = Ext.DomQuery.selectNode('string', response.responseXML).textContent;
			if ("False" == flag) {
				url = contextPath +	"WebRoot/main.html";
			}
			window.top.location.href = url;
		},
		failure : function(response, opts) {
			Ext.Msg.alert("提示", "获取是否需要登录界面失败");
		}
	});
});
