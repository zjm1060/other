/**
 * 采购申请相关
 */
Ext.ns('com.bitc.xmis.device.PurchaseRequisition');
/**
 * 采购申请物资明细表格
 * 
 * @class com.bitc.xmis.device.PurchaseRequisition.DetailGrid
 * @extends Ext.grid.GridPanel
 */
com.bitc.xmis.device.PurchaseRequisition.DetailGrid = Ext.extend(
		Ext.grid.GridPanel, {
			viewConfig : {
				emptyText : '无物资数据'
			},
			constructor : function(ct, cfg) {
				var store = new Ext.data.JsonStore({
							root : 'details',
							remoteSort : false,
							sortInfo : {
								field : 'name',
								dir : 'ASC'
							},
							fields : ['id', 'projectDeviceId', 'name', {
										name : 'number',
										type : 'int'
									}, {
										name : 'maxNumber',
										type : 'int',
										mapping : 'number'
									}, 'modelType', 'standard', 'weight', {
										name : 'referencePrice',
										type : 'float'
									}, 'currency', 'status', {
										name : 'expectArrivalDate',
										type : 'date',
										dateFormat : 'Y-m-d H:i:s'
									}, 'technicalRequirement', 'remark']
						});
				Ext.apply(this, {
					store : store,
					plugins : [new Ext.ux.GridRowEditForm({
								listeners : {
									'beforeshow' : function(f, r) {
										f
												.getForm()
												.findField('number')
												.setMaxValue(r.get('maxNumber'))
									}
								}
							})],
					columns : [new Ext.grid.RowNumberer(), {
								id : 'devName',
								header : '设备名称',
								dataIndex : 'name'
							}, {
								header : '数量',
								align : 'right',
								dataIndex : 'number',
								width : 80,
								editor : {
									xtype : 'numberfield',
									allowDecimals : false,
									allowBlank : false,
									minValue : 0
								}
							}, {
								dataIndex : 'referencePrice',
								header : '参考价格',
								align : 'right',
								width : 90,
								renderer : function(value, metaData, record) {
									return Ext.util.Format.number(value, record
													.get('currency.symbol')
													+ '0,0.00');
								}
							}, {
								xtype : 'datecolumn',
								dataIndex : 'expectArrivalDate',
								format : 'Y-m-d',
								header : '期望到货日期',
								editor : {
									xtype : 'datefield',
									autoWidth : true,
									format : 'Y-m-d',
									minValue : new Date()
								}
							}, {
								dataIndex : 'technicalRequirement',
								header : '技术要求',
								editor : {
									xtype : 'textarea',
									anchor : '100%'
								},
								width : 120
							}, {
								header : '型号',
								dataIndex : 'modelType',
								width : 100
							}, {
								dataIndex : 'standard',
								header : '规格',
								width : 90
							}, {
								dataIndex : 'weight',
								header : '重量',
								align : 'right',
								width : 90
							}, {
								dataIndex : 'remark',
								header : '备注',
								editor : {
									xtype : 'textarea',
									anchor : '100%'
								},
								width : 120
							}]
				});
				com.bitc.xmis.device.PurchaseRequisition.DetailGrid.superclass.constructor
						.call(this, ct, cfg);
			},
			autoExpandColumn : 'devName',
			stripeRows : true
		});

com.bitc.xmis.device.PurchaseRequisition.Record = Ext.data.Record.create(['id',
		'entryDate', 'projectId', 'projectCode', 'projectName', {
			name : 'entryName',
			mapping : 'entryEmp.userName',
			type : 'string'
		}, 'details', 'status', 'dept']);
/**
 * 采购申请列表
 * 
 * @class com.bitc.xmis.device.PurchaseRequisition.List
 * @extends Ext.grid.GridPanel
 */
com.bitc.xmis.device.PurchaseRequisition.List = Ext.extend(Ext.grid.GridPanel,
		{
			/**
			 * @cfg {Object} baseParams of Store
			 */
			storeBaseParams : null,
			initComponent : function() {
				var expander = new Ext.ux.grid.RowExpander({
					tpl : new Ext.XTemplate(
							'<table border="1" cellpadding="0" cellspacing="0" class="detail"> ',
							'<tr> <th width="40px"> 序号 </th> <th width="200px"> 名称 </th> <th width="100px"> 数量 </th> <th> 期望到货日期 </th><th> 状态 </th></tr> ',
							'<tpl for="details"> <tr style="border:1px solid;"> <td> {#} </td> <td> {name} </td> <td style="text-align:right;"> {number} </td> <td>{expectArrivalDate}</td><td>{[this.getStatus(values.status)]}</td></tr> </tpl> </table>',
							{
								disableFormats : true,
								getStatus : function(status) {
									switch (status) {
										case 'NEW' :
											return '申请中';
											break;
										case 'PURCHASING' :
											return '采购中';
											break;
										case 'PURCHASED' :
											return '采购完成，设备已入库';
											break;
										case 'ASSIGNED' :
											return '已分配，已出库';
											break;
									}
								}
							})
				});

				Ext.apply(this, {
					autoExpandColumn : 'remark',
					viewConfig : {
						emptyText : '无采购申请数据'
					},
					stripeRows : true,
					plugins : [expander],
					store : new Ext.data.Store({
						url : String.format(
								'{0}/device.do?method=getPurchaseRequisition',
								__cxtPath),
						baseParams : this.storeBaseParams,
						reader : new Ext.data.JsonReader({
									idProperty : 'id',
									root : 'rows'
								},
								com.bitc.xmis.device.PurchaseRequisition.Record)
					}),
					cm : new Ext.grid.ColumnModel({
						columns : [expander, {
							header : '申请时间',
							dataIndex : 'entryDate',
							renderer : function(value, metaData, record) {
								return String
										.format(
												'<a href="javascript:;" onclick="com.bitc.xmis.device.PurchaseRequisition.linkToPr(\'{0}\')">{1}</a>',
												record.get('id'), value);
							}
						}, {
							header : '申请人',
							dataIndex : 'entryName'
						}, {
							header : '部门',
							dataIndex : 'dept',
							renderer : function(value) {
								return value == null
										? ''
										: value.departmentName;
							}
						}, {
							header : '项目编码',
							dataIndex : 'projectCode',
							width : 160,
							renderer : function(value, meta, record) {
								return String
										.format(
												'<a href="#" onclick="parent.xMis.turn(\'project-info-{0}\',\'项目信息-{1}\',\'projectMain.do?method=getProjectInfo&projectID={2}\')">{3}</a>',
												record.data.projectId,
												record.data.projectCode,
												record.data.projectId,
												record.data.projectCode)
							}
						}, {
							header : '项目名称',
							dataIndex : 'projectName'
						}, {
							header : '状态',
							dataIndex : 'status'
						}, {
							header : '备注',
							id : 'remark',
							dataIndex : 'remark'
						}]
					})
				});
				com.bitc.xmis.device.PurchaseRequisition.List.superclass.initComponent
						.call(this);
			}
		});

/**
 * 链接到采购申请详细页面
 * 
 * @param {String}
 *            id
 */
com.bitc.xmis.device.PurchaseRequisition.linkToPr = function(id) {
	var url = String.format('{0}/device/pr.jsp?requestId={1}&readonly=true',
			__cxtPath, id);
	parent.xMis.turn('purchaseRequisition', '采购申请', url, '');
};