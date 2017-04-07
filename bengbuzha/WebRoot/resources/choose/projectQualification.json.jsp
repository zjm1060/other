<%@page language="java" pageEncoding="utf-8"%>
<%@page import="com.bitc.jk.xmis.model.CompanyObj"%>
{
	'action' : true,
	'message' : 'error!',
	'dataUrl' : 'projectMain.do?method=getProjectQualificationName',
	'autoLoad' : true,
	'sortInfo' : {
		field : 'projectName',
		direction : 'DESC'
	},
	gridConfig : {
		tbar : [ {
					xtype : 'button',	
					text : '刷新',
					iconCls : 'reload',
					ref : '../refreshButton',
					handler : function() {
						this.grid.store.reload();
					}
				}]
	},
	'columnModle' : [ {
									header : '项目ID',
									dataIndex : 'projectID',
									hidden : true,
									sortable : false
								}, {
									header : '项目编码',
									dataIndex : 'projectCode',
									sortable : true,
									width : 200
								},{
									header : '项目名称',
									dataIndex : 'projectName',
									sortable : true,
									width : 400
								},{
								    header : '项目状态',
									dataIndex : 'projectZT',
									sortable : true,
									width : 100
								},{
								    header : '项目经理',
									dataIndex : 'projectJL',
									sortable : true,
									width : 100
								}],
	'fieldsNames' : [
	           {
				name : "projectID"
			},{
				name : "projectCode"
			}, {
				
				name : "projectName"
			}, {
				
				name : "projectZT"
			},{
				name : "projectJL"
			}],
	'searchBar' : true,
	
	'searchFieldConfig' : {
	width : 120,
		tooltip : {
			title : '模糊查询',
			text : '您可以输入项目编码、项目名称、经理名称、项目状态关键词进行查询。'
		},
		inklings : [{
											name : 'projectCode',
											op : 'like',
											type : 'string'
										}, {
											name : 'projectName',
											op : 'like',
											type : 'string'
										},{
											name : 'projectJL',
											op : 'like',
											type : 'string'
										},{
											name :'projectZT',
											op : 'like',
											type :'string'
										}]
	}
}
