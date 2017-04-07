<%@page language="java" pageEncoding="utf-8"%>
<%@page import="com.bitc.jk.xmis.util.DBSchema"%>
{
	'action' : true,
	'message' : 'error!',
	'dataUrl' : 'contractMain.do?method=getDeptListStroeByCompany',
	'sortInfo' : {
		'field' : 'deptName',
		'direction' : 'ASC'
	},
	'gridConfig' : {
		store : new Ext.data.GroupingStore({
			proxy : new Ext.data.HttpProxy({
						url : __cxtPath
								+ '/contractMain.do?method=getDeptListStroeByCompany'
					}),
			remoteSort : true,
			reader : new Ext.data.JsonReader({
						root : 'rows',
						totalProperty : 'totalCount',
						remoteSort : true,
						fields : [{
									name : 'parentName'
								}, {
									name : 'deptID'
								}, {
									name : 'deptName'
								},]
					}),
			groupField : 'parentName'
		}),
		view : new Ext.grid.GroupingView({
					forceFit : true,
					hideGroupedColumn : true,
					showGroupName : false,
					startCollapsed : true
				}),
		tbar:[{
            ref: '../addBtn',
            iconCls: 'add',
            align: 'right',
            text: '新建',
            handler: function(){
            	showPartWindow('新部门', newpartForm,this.ownerCt.ownerCt);
            }
        }],
		bbar : null
	},
	'columnModle' : [{
				'header' : "公司名称",
				'dataIndex' : "parentName",
				'sortable' : true
			}, {
				'header' : "部门ID",
				'dataIndex' : "deptID",
				'sortable' : true,
				'hidden' : true
			}, {
				'header' : "部门名称",
				'dataIndex' : "deptName",
				'sortable' : true
			}],
	'fieldsNames' : [{
				'name' : 'parentName'
			}, {
				'name' : 'deptID'
			}, {
				'name' : 'deptName'
			}]
}