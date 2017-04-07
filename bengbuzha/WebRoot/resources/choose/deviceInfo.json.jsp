 //<%@page language='java' pageEncoding='utf-8'%>
{
	'action' : true,
	'message' : 'error!',
	'dataUrl' : 'device.do?method=getDeviceInfoList',
	'sortInfo' : {
		field : '物资名称',
		direction : 'DESC'
	},
	gridConfig : {
		autoExpandColumn:'deviceName',
		tbar : [{
					ref : '../addBtn',
					iconCls : 'add',
					align : 'right',
					text : '新建',
					handler : function() {
					 	Ext.Msg.show({
					 		title : '提示',
					 		msg : '新建成功后请刷新',
					 		buttons : Ext.Msg.OK,
					 		scope : this,
					 		fn : function(){
					 			this.grid.store.reload();
					 		}
					 	});
						parent.xMis.turn('company-info', '新建物资信息',
						'materials/materialInformation.jsp', '');
				
					}
				}, '-', {
					xtype : 'button',
					text : '刷新',
					iconCls : 'reload',
					ref : '../refreshButton',
					handler : function() {
						this.grid.store.reload();
					}
				}]
	},
	'columnModle' : [{
				id:'deviceName',
				header : '物资名称',
				dataIndex : '物资名称',
				sortable : true
			}, {
				header : '型号',
				dataIndex : '型号',
				sortable : true
			}, {
				header : '规格',
				dataIndex : '规格',
				sortable : true
			}],
	'fieldsNames' : [{
				name : '物资ID'
			}, {
				name : '物资名称'
			}, {
				name : '型号'
			}, {
				name : '规格'
			}, {
				name : 'pageNo'
			}],
	'searchBar' : true,
	'searchFieldConfig' : {
		width : 120,
		tooltip : {
			title : '物资名字模糊查询',
			text : '输入物资名称或简称进行模糊查询。'
		},
		inklings : [{
					name : '物资名称',
					op : 'like',
					type : 'string'
				}]
	}
}