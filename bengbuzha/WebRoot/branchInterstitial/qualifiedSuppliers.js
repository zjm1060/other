Ext.onReady(function(){
	
	Ext.QuickTips.init();
	Ext.apply(Ext.QuickTips.getQuickTip(), {
				maxWidth : 500
			});
	var exportname = '合格供货商';
	var cm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({width :25}), {
									header : '公司ID',
									dataIndex : 'companyID',
									hidden : true,
									sortable : false
								}, {
									header : '公司名称',
									dataIndex : 'companyName',
									sortable : true,
									width : 160,
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
												+ '&tab=2\',\'menu-company-info\')">' + value + '</a>';
									}
								},{
									header : '资质情况',
									dataIndex : 'qualification',
									sortable : true,
									width : 220
								}, {
									header : '可供产品',
									dataIndex : 'supporting',
									sortable : true,
									width : 180
								},{
									header:'注册资本金',
									dataIndex : 'registeredCapital',
									sortable : true,
									width:100
								},{
									header : '是否为一般纳税人',
									dataIndex :'dddddddd',
									sortable : true,
									width :100,
									align : 'right'
								},{
									header : '公司网址',
									dataIndex :'website',
									sortable : true,
									width :140
								}, {
									header : '联系人',
									dataIndex : 'contactName',
									sortable : true,
									width : 80
								}, {
									header : '座机电话',
									dataIndex : 'landline',
									sortable : true,
									width : 120
								}, {
									header : '制表人',
									dataIndex : 'lastModifiedByName',
									sortable : true,
									width : 100
								}, {
									header : '制表日期',
									dataIndex : 'lastModifiedDate',
									sortable : true,
									width : 100,
									renderer : function(value, cellmeta, record, rowIndex, columnIndex,store) {
										return value.substring(0,10);
									}
								}]);
								
//								var qxMsg = new Ext.form.Label({});

//	if (!decisionFlag) {
//		qxMsg.setText('<font color=red>*权限不足，无权查看与金额相关的信息！&nbsp;</font>',
//						false);
//	}
	// reRecord org_公司
	var reRecord = Ext.data.Record.create([
	           {
	           	"mapping":"companyID",
				"name" : 'companyID'
			}, {
				"mapping":"companyName",
				"name": 'companyName'
			}, {
				name : "qualification"
			},{
				name : "supporting"
			}, {
				name : 'taxpayer'
			}, {
				name : 'website'
			}, {
				name : 'contactName'
			}, {
				name : 'contactID'
			}, {
				name : 'landline'
			}, {
				name : 'lastModifiedByName'
			}, {
				name : 'lastModifiedByID'
			}, {
				name : 'lastModifiedDate'
			},{
				name : 'dddddddd'
			},{
				name : 'registeredCapital'
			}]);
	// store org_公司
	var store = new Ext.data.Store({
						proxy : new Ext.data.HttpProxy({
							url : __cxtPath+ '/kzhbMaint.do?method=getQualifiedSuppliers',
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
					field : 'companyName',
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
//								width : 160,
								store : store,
								inklings : [
									{
											name : 'companyName',
											op : 'like',
											type : 'string'
										},{
											name:'landline',
											op:'like',
											type:'string'
										},{
											name:'supporting',
											op:'like',
											type:'string'
										},{
											name:'qualification',
											op:'like',
											type:'string'
										},{
											name:'contactName',
											op:'like',
											type:'string'
										}
										]
										,
								tooltip : {
									title : '模糊查询',
									text : '输入物资名称、项目编码、物资自身属性、物资业务属性、物资用途、型号、供应商进行查询。'
								}
							}),'->'],
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