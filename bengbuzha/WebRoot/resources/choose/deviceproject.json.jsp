<%@page language="java" pageEncoding="utf-8"%>
<%@page import="com.bitc.jk.xmis.model.device.DeviceInfo"%>
{
	'action' : true,
	'message' : 'error!',
	'dataUrl' : 'device.do?method=getDeviceInfo',
	'autoLoad' : true,
	'sortInfo' : {
		field : '物资名称',
		direction : 'ASC'
	},
	gridConfig : {
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
	'columnModle' : [new Ext.grid.RowNumberer({width :25}), {
									header : '物资ID',
									dataIndex : '物资ID',
									hidden : true,
									sortable : false
								}, {
									header : '物资名称',
									dataIndex : '物资名称',
									sortable : true,
									width : 160
								},{
									header : '生产厂家',
									dataIndex : '生产厂家名称',
									sortable : true,
									width : 220
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
								}],
	'fieldsNames' : [
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
			},{
				name:'最后修改日期'
			},{
				name:'货币名称'
			}],
	'searchBar' : true,
	
	'searchFieldConfig' : {
		width : 120,
		tooltip : {
			title : '模糊查询',
			text : '您可以输入任意关键词进行查询。<br/>可以在关键字前加运算符：>、<、>=、<=、=、<>、like等。<br/>按日期请按如下格式输入：<br/>2010-01-01'
		},
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
										}]
	}
	
}
