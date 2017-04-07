// <%@page language='java' pageEncoding='utf-8'%>
// <%@page import='com.bitc.jk.xmis.util.DBSchema'%>
{
	'action' : true,
	'message' : 'error!',
	'dataUrl' : 'contractMain.do?method=getCompanyListStroe',
	'sortInfo' : {
		field : '公司名称',
		direction : 'ASC'
	},
	gridConfig : {
		autoExpandColumn:'companyName',
		tbar : [{
					ref : '../addBtn',
					iconCls : 'add',
					align : 'right',
					text : '新建',
					handler : function() {
						//showCompWindow('新公司', newCompForm,this.ownerCt.ownerCt);
						//Ext.getCmp('companyName').setValue('');
						//Ext.getCmp('shotCompName').setValue('');
						//Ext.getCmp('compPartName').setValue('');
						//Ext.getCmp('compDelegate').setValue('');
					 	Ext.Msg.show({
					 		title : '提示',
					 		msg : '新建成功后请刷新',
					 		buttons : Ext.Msg.OK,
					 		scope : this,
					 		fn : function(){
					 			this.grid.store.reload();
					 		}
					 	});
						parent.xMis.turn('company-info', '新建公司信息',
						'crm/companyInfo.jsp', '');
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
				header : '公司ID',
				dataIndex : '<%=DBSchema.Company.CompanyID %>',
				sortable : true,
				hidden : true
			}, {
				id:'companyName',
				header : '公司名称',
				dataIndex : '<%=DBSchema.Company.CompanyName %>',
				sortable : true
			}, {
				header : '简称',
				dataIndex : '<%=DBSchema.Company.Abbreviation %>',
				sortable : true
			}, {
				header : '省份',
				dataIndex : '<%=DBSchema.Province.ProvinceName %>',
				sortable : true
			}],
	'fieldsNames' : [{
				name : '<%=DBSchema.Company.CompanyID %>'
			}, {
				name : '<%=DBSchema.Company.CompanyName %>'
			}, {
				name : '<%=DBSchema.Company.Abbreviation %>'
			}, {
				name : '<%=DBSchema.Province.ProvinceName %>'
			}, {
				name : 'pageNo'
			}],
	'searchBar' : true,
	'searchFieldConfig' : {
		width : 120,
		tooltip : {
			title : '模糊查询',
			text : '输入公司名称或简称进行模糊查询。'
		},
		inklings : [{
					name : '<%=DBSchema.Company.CompanyName %>',
					op : 'like',
					type : 'string'
				}, {
					name : '<%=DBSchema.Company.Abbreviation %>',
					op : 'like',
					type : 'string'
				}]
	}
}