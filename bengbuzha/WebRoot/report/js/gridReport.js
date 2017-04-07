Ext.onReady(function() {
	Ext.Ajax.request({
		url : String.format('{0}/report.do?method=getGridMeta&reportToken={1}',
				__cxtPath, __token),
		method : 'post',
		success : function(response) {
			var result = Ext.decode(response.responseText);
			if (result.success) {
				var action = result.attributes.meta;
				var cm = new Ext.grid.ColumnModel(action.cm);
				var reader = new Ext.data.JsonReader(action.readerMetaData);
				var store = new Ext.data.Store({
							url : String
									.format(
											'{0}/report.do?method=getGridData&reportToken={1}',
											__cxtPath, __token),
							autoLoad : true,
							reader : reader,
							listeners:{
								exception:function(proxy,exceptiontype,actiontype,option,response){
									if(exceptiontype=='response'){
										var r=Ext.decode(response.responseText);
										Ext.Msg.alert(r.messageTitle, r.message);
									}
								}
							}
						});
				var grid = new Ext.grid.GridPanel({
							cm : cm,
							store : store,
							autoExpandColumn : action.cm[action.cm.length - 1].id,
							region : 'center'
						});
				new Ext.Viewport({
							layout : 'border',
							items : [grid]
						});
			} else {
				Ext.Msg.alert(result.messageTitle, result.message);
			}
		}
	});
});