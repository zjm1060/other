Ext.onReady(function(){
	
	Ext.QuickTips.init();
	Ext.apply(Ext.QuickTips.getQuickTip(), {
				maxWidth : 500
			});
	var exportname = '资质列表';
	var cm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({width :25}), {
									header : '资质名称对应ID',
									dataIndex : 'id',
									hidden : true,
									sortable : false
								},{
									header : '公司ID',
									dataIndex : 'companyID',
									hidden : true,
									sortable : false
								},{
									header : '公司名称',
									dataIndex : 'companyName',
									sortable : false,
									width : 260,
									renderer : function(value, metaData, record) {
										return '<a href="javascript:;" onclick="parent.xMis.turn(\'company-info-'
												+ record.get('companyID')
												+ '\',\''
												+ '[CRM]'
												+ record.get('companyName')
												+ '\',\''
												+ __cxtPath
												+ '/crmMaint.do?method=getCompanyInfo&companyID='
												+ record.get('companyID')
												+ '&tab=3'
												+ '&id='
												+ record.get('id')
												+ '\',\'menu-company-info\')">' + value + '</a>';
									}
								}, {
									header : '资质名称',
									dataIndex : 'qualifiationName',
									sortable : true,
									width : 160
								},{
									header : '证书号',
									dataIndex : 'certificateNo',
									sortable : true,
									width : 180
								}, {
									header : '证书有效期起',
									dataIndex : 'certificateValidQi',
									sortable : true,
									width : 100,
									renderer : function(value, cellmeta, record, rowIndex, columnIndex,store) {
										return value.substring(0,10);
									}
								},{
									header:'证书有效期止',
									dataIndex : 'certificateValidZhi',
									sortable : true,
									width:100,
									renderer : function(value, cellmeta, record, rowIndex, columnIndex,store) {
										return value.substring(0,10);
									}
								},{
									header : '年审',
									dataIndex :'examined',
									sortable : true,
									width :60,
									align : 'right'
								}, {
									header : '备注',
									dataIndex : 'qualificationRemark',
									sortable : true,
									width : 300
								}]);
								
								var qxMsg = new Ext.form.Label({});

//	if (!decisionFlag) {
//		qxMsg.setText('<font color=red>*权限不足，无权查看与金额相关的信息！&nbsp;</font>',
//						false);
//	}
	// reRecord org_公司
	var reRecord = Ext.data.Record.create([
				            {
							name : "id"
						},{
							name : 'companyID'
						},{
							name : 'companyName'
						},{
							name : "qualifiationName"
						}, {
							name : 'certificateNo'
						}, {
							name : 'certificateValidQi'
						}, {
							name : 'certificateValidZhi'
						}, {
							name : 'examined'
						}, {
							name : 'qualificationRemark'
						}, {
							name : 'frequencyOfUse'
						}]);
	// store org_公司
	var store = new Ext.data.Store({
						proxy : new Ext.data.HttpProxy({
							url : __cxtPath+ '/crmMaint.do?method=getQualificationListC',
							method : "POST"
						}),
						reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount'
						}, reRecord),
						remoteSort : true,
					baseParams : {
						start : 0,
						limit : 20
					},
					autoLoad : true,
					sortInfo : {//pageInfo排序条件
					field : 'qualifiationName',
					direction : 'ASC'
				}
	});
	
	
	var exp = new Ext.ux.Export({
				formater : []
			});
	var gridPanel = new Ext.grid.GridPanel({
		region : 'center',
		border : false,
		loadMask : true,
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		stripeRows : true,
		store : store,
		cm : cm,
		tbar :[
		       '模糊查询：', new Ext.ux.InklingSearchField({
								width : 160,
								store : store,
								inklings : [{
											name : 'qualifiationName',
											op : 'like',
											type : 'string'
										},{
											name:'certificateNo',
											op:'like',
											type:'string'
										},{
											name:'certificateValidZhi',
											op:'=',
											type:'date'
										}],
								tooltip : {
									title : '模糊查询',
									text : '输入资质名称、证书号、证书有效期止进行查询。'
								}
							}),'->',qxMsg],
		bbar : new Ext.PagingToolbar({
			pageSize : 20,
			store : store,
			displayInfo : true,
			displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
			emptyMsg : "无显示数据",
			plugins : [new Ext.ux.PagelimitCombo()],
			items : [new Ext.Button({
						text : this.text || '导出Excel',
						iconCls : 'xls',
						menu : {
							items : [{
								text : '导出本页数据',
								handler : function() {
									exp.onExport(gridPanel, exportname,
											true);
								}
							}, {
								text : '导出所有数据',
								handler : function() {
									exp.onExport(gridPanel, exportname,
											false);
								}
							}]
						}
					})]
		})
		});
	new Ext.Viewport({
				layout : 'border',
				items : [gridPanel]
			});
});