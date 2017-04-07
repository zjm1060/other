Ext.onReady(function(){
	
	Ext.QuickTips.init();
	Ext.apply(Ext.QuickTips.getQuickTip(), {
				maxWidth : 500
			});
	var exportname = '物资列表';
	var cm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({width :25}), {
									header : '物资ID',
									dataIndex : '物资ID',
									hidden : true,
									sortable : false
								}, {
									header : '物资名称',
									dataIndex : '物资名称',
									sortable : true,
									width : 160,
									listeners:{
										
										'click' : function(t,grid, rowIndex, e){
												var r = grid.getStore().getAt(rowIndex);
												var contactPersonID = r.data.物资ID;
												var  materiaName = grid.getStore().getAt(rowIndex).data.物资名称;//点击跳转页面
												if (contactPersonID == null || contactPersonID == '') {
															return;
														}
															parent.xMis.turn(
																				'company-info-'+contactPersonID,
																				"[物资]"+materiaName,
																				__cxtPath
																						+ '/device.do?method=getMaterialnfo&id='+contactPersonID,
																				'menu-company-info', true);
													}
										}
								},{
									header : '生产厂家',
									dataIndex : '生产厂家名称',
									sortable : true,
									width : 220,//点击跳转页面
									renderer : function(value, metaData, record) {
// alert(record.get('manufacturer.companyID'));
										return '<a href="javascript:;" onclick="parent.xMis.turn(\'company-info-'
												+ record.get('生产厂家ID')
												+ '\',\''
												+ '[CRM]'
												+ value
												+ '\',\''
												+ __cxtPath
												+ '/crmMaint.do?method=getCompanyInfo&companyID='
												+ record.get('生产厂家ID')
												+ '\',\'menu-company-info\')">' + value + '</a>';
									}
								}, {
									header : '官方报价',
									dataIndex : '官方报价',
									sortable : true,
									width : 100,
									align : 'right'
// ,
// renderer : function(value, metaData, record, rowIndex,
// colIndex, store) {
// if(!decisionFlag){
// return formatCurrency("", '0', "");
// }else{
// return formatCurrency("", value, "");
// }
// }
								},{
									header : '最新价格',
									dataIndex :'最新单价',
									sortable : true,
									width :80,
									align : 'right',
									renderer:function(value, cellMeta, record, rowIndex, columnIndex,store){
										cellMeta.attr = 'ext:qtitle="最新价格登记日期" ext:qtip="' + (store.data.items[rowIndex].data.最新价格登记日期).substring(0,10) +'"';  
										if(!decisionFlag){
												return  formatCurrency("", '0', "");
											}else{
// return formatCurrency("", value, "");
               						 return value;
											}
									}
								},{
									header : '最低价格',
									dataIndex :'最低单价',
									sortable : true,
									width :80,
									align : 'right',
									renderer:function(value, cellMeta, record, rowIndex, columnIndex,store){
										cellMeta.attr = 'ext:qtitle="最低价格登记日期" ext:qtip="' + (store.data.items[rowIndex].data.最低价格登记日期).substring(0,10) +'"';  
										if(!decisionFlag){
												return  formatCurrency("", '0', "");
											}else{
// return formatCurrency("", value, "");
               						 return value;
											}
									}
								}, {
									header : '币种',
									dataIndex : '币种',
									hidden : true,
									sortable : true,
									width : 60
								}, {
									header : '最新采购价格',
									dataIndex : '最新采购价格',
									sortable : true,
									hidden : true,
									renderer : function(value, metaData, record, rowIndex,
											colIndex, store) {}
								}, {
									header : '物资业务属性',
									dataIndex : '物资业务属性名称',
									sortable : true,
									width : 100,
									renderer : function(value, metaData, record, rowIndex,
											colIndex, store) {
										if(record.get('物资业务属性')=='1'){
											return '';
										}else{
										return value;
										}	
											}
								}, {
									header : '物资自身属性',
									dataIndex : '物资自身属性名称',
									sortable : true,
									width : 100,
									renderer : function(value, metaData, record, rowIndex,
											colIndex, store) {
										if(record.get('物资自身属性')=='1'){
											return '';
										}else{
										return value;
										}	
											}
								}, {
									header : '物资用途',
									dataIndex : '物资用途名称',
									sortable : true,
									width : 100,
									renderer : function(value, metaData, record, rowIndex,
											colIndex, store) {
										if(record.get('物资用途')=='1'){
											return '';
										}else{
										return value;
										}	
											}
								},{
									header : '物资最大供应商',
									dataIndex : '公司名称',
									sortable : true,
									width : 180
								},{
									header : '物资使用最多项目',
									dataIndex : '项目编码',
									sortable : true,
									width : 180
								},{
// xtype:'datecolumn',
									header:'上市日期',
									dataIndex:'投入市场日期',
									sortable:true,
									width : 80,
									renderer : function(value, cellmeta, record, rowIndex, columnIndex,store) {
										return value.substring(0,10);
									}
								}, {
// xtype:'datecolumn',
									header : '停产时间',
									dataIndex : '停产时间',
									sortable : true,
									width : 80,
									renderer : function(value, cellmeta, record, rowIndex, columnIndex,store) {
										return value.substring(0,10);
									}
								},{
									header:'物资生产状态',
									dataIndex:'物资生产状态名称',
									sortable:true,
									width : 100
								}, {
									header : '型号',
									dataIndex : '型号',
									sortable : true,
									width : 100
								},  {
									header : '规格',
									dataIndex : '规格',
									sortable : true,
									width : 100
								}, {
									header : '重量',
									dataIndex : '重量',
									sortable : true,
									width : 100
								}, {
									header : '功率',
									dataIndex : '功率',
									width : 100,
									sortable : true
								},  {
									header : '体积',
									dataIndex : '体积',
									sortable : true,
									width : 80
								}]);
								
								var qxMsg = new Ext.form.Label({});

	if (!decisionFlag) {
		qxMsg.setText('<font color=red>*权限不足，无权查看与金额相关的信息！&nbsp;</font>',
						false);
	}
	// reRecord org_公司
	var reRecord = Ext.data.Record.create([
	           {
	           	"mapping":"物资ID",
				"name" : '物资ID'
			}, {
				"mapping":"物资名称",
				"name": '物资名称'
			}, {
				
				name : "型号"
			},{
				name : "规格"
			}, {
				name : '重量'
			}, {
				name : '功率'
			}, {
				name : '官方报价'
			}, {
				name : '价格'
			}, {
				name : '货币ID'
			}, {
				name : '配置'
			}, {
				name : '体积'
			}, {
				name : '物资业务属性名称'
			}, {
				name : '物资自身属性名称'
			}, {
				name : '物资用途名称'
			}, {
				name : '停产时间'
			},{
				name:'投入市场日期'
			} ,{
				name : "生产厂家ID"
			}, {
				name : '生产厂家名称'
			},{
				name:'物资生产状态名称'
			},{
				name :'公司名称'
			},{
				name :'乙方公司ID'
			},{
				name:'项目ID'
			},{
				name:'项目名称'
			},{
				name:'项目编码'
			},{
				name:'物资生产状态'
			},{
				name:'物资用途'
			},{
				name:'物资自身属性'
			},{
				name:'物资业务属性'
			},{
				name:'最新单价'
			},{
				name:'最低单价'
			},{
				name:'最新价格登记日期'
			},{
				name:'最低价格登记日期'
			},{
				name:'登记日期'
			}]);
	// store org_公司
	var store = new Ext.data.Store({
						proxy : new Ext.data.HttpProxy({
							url : __cxtPath+ '/device.do?method=getDeviceInfo',//查询物资列表。
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
					field : '物资名称',
					direction : 'ASC'
				}
	});
	
	
	var exp = new Ext.ux.Export({
				formater : [{
							columnHeader : '',
							formatClass : 'com.bitc.jk.xmis.export.PagingRowNumber'
						},{
							columnHeader : '最低价格',
							formatClass:'com.bitc.jk.xmis.export.formatExport'
						},{
							columnHeader :'最新价格',
							formatClass:'com.bitc.jk.xmis.export.formatExport'
						},{
							columnHeader :'物资业务属性',
							formatClass:'com.bitc.jk.xmis.export.materialPropertiestoo'
						},{
							columnHeader :'物资用途',
							formatClass:'com.bitc.jk.xmis.export.materialPropertiestoo'
						},{
							columnHeader :'物资自身属性',
							formatClass:'com.bitc.jk.xmis.export.materialPropertiestoo'
						}]
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
											name : '物资ID',
											op : 'like',
											type : 'string'
										}, {
											name : '物资名称',
											op : 'like',
											type : 'string'
										},{
											name : '型号',
											op : 'like',
											type : 'string'
										},{
											name :'物资业务属性名称',
											op : 'like',
											type :'string'
										},{
											name : '物资自身属性名称',
											op : 'like',
											type : 'string'
										},{
											name : '物资用途名称',
											op : 'like',
											type : 'string'
										},{
											name : '公司名称',
											op : 'like',
											type : 'string'
										},{
											name : '生产厂家名称',
											op : 'like',
											type : 'string'
										},{
											name : '项目名称',
											op : 'like',
											type : 'string'
										},{
											name : '项目ID',
											op : 'like',
											type : 'string'
										}],
								tooltip : {
									title : '模糊查询',
									text : '输入物资名称、项目编码、物资自身属性、物资业务属性、物资用途、型号、供应商进行查询。'
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