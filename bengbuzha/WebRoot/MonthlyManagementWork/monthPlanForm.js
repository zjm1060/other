Ext.ns('com.bitc.xmis.MonthlyManagementWork');
com.bitc.xmis.MonthlyManagementWork.AbsentFormPanelRecord = Ext.data.Record.create([{
		mapping: 'form_id',
		name: 'form_id',
		type:'string'
	},{
		dateFormat : "Y-m",
		mapping: 'work_month',
		name: 'work_month',
		type:'date'
	},{
		mapping: 'plan_comments',
		name: 'plan_comments',
		type:'string'
	},{
		mapping: 'plan_assign_comments',
		name: 'plan_assign_comments',
		type:'string'
	},{
		mapping: 'plan_assigned_id',
		name: 'plan_assigned_id',
		type:'int'
	},{
		mapping: 'plan_assigned_name',
		name: 'plan_assigned_name',
		type:'String'
	},{
		mapping: 'plan_assigned_date',
		name: 'plan_assigned_date',
		type:'String'
	},{
		mapping: 'excuted_comments',
		name: 'excuted_comments',
		type:'string'
	},{
		mapping: 'unfinished_comments',
		name: 'unfinished_comments',
		type:'string'
	},{
		mapping: 'excuted_assigned_comments',
		name: 'excuted_assigned_comments',
		type:'string'
	},{
		mapping: 'excuted_assigned_id',
		name: 'excuted_assigned_id',
		type:'int'
	},{
		mapping: 'excuted_assigned_name',
		name: 'excuted_assigned_name',
		type:'String'
	},{
		mapping: 'excuted_assigned_date',
		name: 'excuted_assigned_date',
		type:'String'
	},{
		mapping: 'app_dept_id',
		name: 'app_dept_id',
		type:'int'
	},{
		mapping: 'app_dept_name',
		name: 'app_dept_name',
		type:'String'
	},{
		mapping: 'app_user_id',
		name: 'app_user_id',
		type:'int'
	},{
		mapping: 'app_user_name',
		name: 'app_user_name',
		type:'String'
	},{
		mapping: 'app_date',
		name: 'app_date',
		type:'String'
	}]);
com.bitc.xmis.MonthlyManagementWork.AbsentFormPanel = new Ext.form.FormPanel({
	ref:'form',
	region:'center',
	padding:20,
	layout: 'column',
	autoScroll:true,
	border: false,
	labelWidth: 90,
	width:900,
	labelAlign:'right',
	reader: new Ext.data.JsonReader({
								root:'rows'
								},com.bitc.xmis.MonthlyManagementWork.AbsentFormPanelRecord
	),
	items:[ {
				
						xtype : 'panel',
						layout : 'column',
						border : false,
						width:'100%',
						items : [{
									xtype : 'panel',
									layout : 'form',
									border : false,
									labelAlign : 'rigth',
									width:300,
									items : [{
												xtype : 'hidden',
												fieldLabel : '申请人ID',
												id : 'app_user_id',
												name : 'app_user_id'
											},{
												xtype : 'displayfield',
												fieldLabel : '申请人',
												name : 'app_user_name',
												id:'app_user_name'
											}]
								},{
									xtype : 'panel',
									layout : 'form',
									border : false,
									labelAlign : 'rigth',
									width:300,
									items : [{
												xtype : 'hidden',
												fieldLabel : '申请部门ID',
												id : 'app_dept_id',
												name : 'app_dept_id'
											},{
												xtype : 'displayfield',
												fieldLabel : '申请部门',
												name : 'app_dept_name',
												id:'app_dept_name'
											}]
								
								},{
									xtype : 'panel',
									layout : 'form',
									border : false,
									width:300,
									labelAlign : 'rigth',
									items : [{
												xtype : 'displayfield',
												fieldLabel : '申请时间',
												dateFormat:'Y-m-d',
												id:'app_date',
												name : 'app_date'
											}]
								}]
					
			},{
		xtype:'fieldset',
		id: 'EarlyPlans',
		height: 400,
		width:'45%',
		title:'月初上报',
		padding:20,
		defaults:{
			border: false
		},
		items:[
			{
				xtype:'hidden',
				fieldLabel:'表单ID',
				name:'form_id'
			},{
				xtype:'datefield',
				format : "Y-m",
				fieldLabel:'工作月份',
				name:'work_month',
				allowBlank: false,
				editable: false,
				width: 300,
				plugins:'monthPickerPlugin'
			},{
				xtype:'textarea',
				fieldLabel:'月初计划',
				name:'plan_comments',
				height: 200,
				width: 300
			},{
				xtype:'textarea',
				fieldLabel:'领导意见',
				readOnly: true,
				name:'plan_assign_comments',
				width: 300
			},{
				
						xtype : 'panel',
						layout : 'column',
						border : false,
						width: 500,
						items : [{
									xtype : 'panel',
									layout : 'form',
									columnWith : 0.5,
									border : false,
									labelAlign : 'rigth',
									items : [{
												xtype : 'hidden',
												id : 'plan_assigned_id',
												name : 'plan_assigned_id'
											},{
												xtype : 'displayfield',
												fieldLabel : '审批人',
												name : 'plan_assigned_name',
												id : 'plan_assigned_name'
											}]
								}, {
									xtype : 'panel',
									layout : 'form',
									columnWith : 0.5,
									border : false,
									labelAlign : 'rigth',
									items : [{
												xtype : 'displayfield',
												fieldLabel : '审批时间',
												name : 'plan_assigned_date',
												id : 'plan_assigned_date'
											}]
								}]
					
			}
		]
	},{
		xtype:'fieldset',
		id: 'TheEndOfThePlanned',
		height: 400,
		width:'45%',
		style:'margin: 0 0 0 50px',
		title:'月末完成情况',
		defaults:{
			border: false
		},
		items:[
			{	
				xtype:'hidden',
				fieldLabel:'表单ID',
				name:'form_id',
				width: 300
			},{
				xtype:'textarea',
				fieldLabel:'完成内容',
				height: 111,
				name:'excuted_comments',
				width: 300
			},{
				xtype:'textarea',
				fieldLabel:'未完成内容',
				name:'unfinished_comments',
				height:111,
				width: 300
			},{
				xtype:'textarea',
				fieldLabel:'领导意见',
				readOnly:true,
				name:'excuted_assigned_comments',
				width: 300
			},{
						xtype : 'panel',
						layout : 'column',
						border : false,
						width: 500,
						items : [{
									xtype : 'panel',
									layout : 'form',
									columnWith : 0.5,
									border : false,
									labelAlign : 'rigth',
									items : [{
												xtype : 'hidden',
												id : 'excuted_assigned_id',
												name : 'excuted_assigned_id'
											},{
												xtype : 'displayfield',
												fieldLabel : '审批人',
												name : 'excuted_assigned_name',
												id:'excuted_assigned_name'
											}]
								}, {
									xtype : 'panel',
									layout : 'form',
									columnWith : 0.5,
									border : false,
									labelAlign : 'rigth',
									items : [{
												xtype : 'displayfield',
												fieldLabel : '审批时间',
												name : 'excuted_assigned_date',
												id : 'excuted_assigned_date'
											}]
								}]
					
			
			}
		]
	}]
});