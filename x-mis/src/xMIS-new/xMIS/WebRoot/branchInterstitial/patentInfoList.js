Ext.onReady(function(){
	
	Ext.QuickTips.init();
	Ext.apply(Ext.QuickTips.getQuickTip(), {
				maxWidth : 500
			});
	var exportname = '专利列表';
	var cm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({width :25}), 
								{
										header : '公司ID',
									dataIndex : 'companyID',// +','+'patentedDesignPeople2'+','+'patentedDesignPeople3'
									hidden : true
								},{
									header : '专利名称',
									dataIndex : 'patentName',// +','+'patentedDesignPeople2'+','+'patentedDesignPeople3'
									sortable : true,
									width : 200,
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
												+ '&tab=4'
												+ '&sjid='
												+ record.get('sjid')
												+ '\',\'menu-company-info\')">' + value + '</a>';
									}
								},{
									header : '类别',
									dataIndex : 'patentsCategory',
									sortable : true,
									width : 230
								}, {
									header : '设计人',
									dataIndex : 'patentedDesignPeople',
									sortable : true,
									width : 230
								}, {
									header : '专利权人',
									dataIndex : 'patentRightPeople',
									sortable : true,
									width : 230
								},{
									header : '专利号',
									dataIndex : 'patentNumber',
									sortable : true,
									width : 230
								},{
									header : '证书号',
									dataIndex : 'patentCertificateNo',
									sortable : true,
									width : 230
								},{
									header : '申请日',
									dataIndex : 'applicationTime',
									sortable : true,
									renderer : function(value, cellmeta, record, rowIndex,
											columnIndex, store) {
										return value.substring(0, 10);
									}
								},{
									header : '授权公告日',
									dataIndex : 'authorizeTime',
									sortable : true,
									renderer : function(value, cellmeta, record, rowIndex,
											columnIndex, store) {
										return value.substring(0, 10);
									}
								}, {
									header : '有效期起',
									dataIndex : 'validFrom',
									sortable : true,
									// width : 230,
									renderer : function(value, cellmeta, record, rowIndex,
											columnIndex, store) {
										return value.substring(0, 10);
									}
								}, {
									header : '有效期止',
									dataIndex : 'validOnly',
									sortable : true,
									// width : 230,
									renderer : function(value, cellmeta, record, rowIndex,
											columnIndex, store) {
										return value.substring(0, 10);
									}
								}]);
								
								var qxMsg = new Ext.form.Label({});

//	if (!decisionFlag) {
//		qxMsg.setText('<font color=red>*权限不足，无权查看与金额相关的信息！&nbsp;</font>',
//						false);
//	}
	// reRecord org_公司
	var reRecord = Ext.data.Record.create([
				            {
								name : 'sjid'
							}, {
								name : 'companyID'
							},{
								name : 'companyName'							
							}, {
								name : 'patentName'
							},{
								name : 'patentsCategory'
							}, {
								name : 'patentedDesignPeople'
							}, {
								name : 'patentRightPeople'
							},{
								name : 'patentNumber'
							},{
								name : 'patentCertificateNo'
							},{
								name : 'applicationTime'
							},{
								name : 'authorizeTime'
							}, {
								name : 'validFrom'
							}, {
								name : 'validOnly'
						}]);
	// store org_公司
	var store = new Ext.data.Store({
						proxy : new Ext.data.HttpProxy({
							url : __cxtPath+ '/kzhbMaint.do?method=getPatentInfoList',
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
					field : 'patentName',
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
											name : 'patentName',
											op : 'like',
											type : 'string'
										},{
											name:'patentsCategory',
											op:'like',
											type:'string'
										},{
											name:'patentNumber',
											op:'like',
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