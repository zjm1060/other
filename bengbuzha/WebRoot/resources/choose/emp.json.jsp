<%@page language="java" pageEncoding="utf-8"%>
<%@page import="com.bitc.jk.xmis.util.DBSchema"%>
{
	'action' : true,
	'message' : 'error!',
	'dataUrl' : 'crmMaint.do?method=getContactPersonByCompany',
	'sortInfo' : {
		field : '合同名称',
		direction : 'ASC'
	},
	'columnModle' : [{
				header : "部门名称",
				dataIndex : "deptName",
				sortable : true,
				hidden : true
			}, {
				header : "联系人ID",
				dataIndex : "<%=DBSchema.EmployeeBelongToDept.EmployeeID %>",
				sortable : true,
				hidden : true
			}, {
				header : "联系人名称",
				dataIndex : "user_name",
				sortable : true
			}],
	'fieldsNames' : [{
				name : 'deptName'
			}, {
				name : '<%=DBSchema.EmployeeBelongToDept.EmployeeID %>'
			}, {
				name : 'user_name'
			}],
	gridConfig : {
		store : new Ext.data.GroupingStore({
			proxy : new Ext.data.HttpProxy({
						url : __cxtPath+'/crmMaint.do?method=getContactPersonByCompany'
					}),
			remoteSort : true,
			reader : new Ext.data.JsonReader({
				root : 'rows',
				totalProperty : 'totalCount',
				remoteSort : true,
				fields : [{
							name : 'deptName'
						}, {
							name : '<%=DBSchema.EmployeeBelongToDept.EmployeeID %>'
						}, {
							name : 'user_name'
						},]
			}),
			groupField : 'deptName'
		}),
		view : new Ext.grid.GroupingView({
					forceFit : true,
					emptyText : "无相关数据",
					scrollToRecord : function(record) {
						var index = this.grid.getStore().indexOf(record);
						this.scrollToRow(index);
					},
					scrollToRow : function(rowIndex) {
						var firstRow = Ext.get(this.getRow(0));
						var row = Ext.get(this.getRow(rowIndex));
						var distance = row.getOffsetsTo(firstRow)[1];
						this.scroller.dom.scrollTop = 120;// distance;
					},
					hideGroupedColumn : false,
					showGroupName : false,
					startCollapsed : true
				}),
		tbar:[{
            ref: '../addBtn',
            iconCls: 'add',
            align: 'right',
            text: '新建',
            id:'addContactPerson',
            handler: function(){
            	showContactPersonWin("新联系人",this.ownerCt.ownerCt);
            	Ext.getCmp("contactPersonName").setValue('');
            	if(contructPersonFlag == "cEmpWin"){
            		newContactPersonForm.ownerCt.setTitle("新收货人");
            	};
            }
        }],
		bbar : null
	}
}
