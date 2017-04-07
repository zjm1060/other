/**
 * 采购清单相关,require GroupSummary.js,SearchField.js,InklingSearchField.js
 */
Ext.ns('com.bitc.xmis.device.PurchasingList');
/**
 * 采购清单明细Record
 */
com.bitc.xmis.device.PurchasingList.DetailListRecord = Ext.data.Record.create([
		'id', 'projectDeviceId', 'name', {
			name : 'number',
			type : 'int'
		}, 'modelType', 'standard', 'weight', 'projectId', 'projectCode',
		'projectName', {
			name : 'expectArrivalDate',
			type : 'date',
			dateFormat : 'Y-m-d H:i:s'
		}, 'technicalRequirement', 'remark', {
			name : 'group',
			convert : function(v, record) {
				return record.name + '（' + record.modelType + '）';
			}
		}, 'dept', 'applicant', 'requisitionDetailId', 'requisitionId']);
/**
 * @class com.bitc.xmis.device.PurchasingList.DetailList
 * @extends Ext.grid.GridPanel 采购清单明细列表
 */
com.bitc.xmis.device.PurchasingList.DetailList = Ext.extend(Ext.grid.GridPanel,
		{
			initComponent : function(a, b) {
				var autoExId = Ext.id();
				Ext.apply(this, {
					autoExpandColumn : autoExId,
					store : new Ext.data.Store({
						sortInfo : {
							field : 'name',
							direction : 'ASC'
						},
						reader : new Ext.data.JsonReader(
								{
									root : 'details'
								},
								com.bitc.xmis.device.PurchasingList.DetailListRecord)
					}),
					columns : [{
								header : '设备名称',
								dataIndex : 'name',
								sortable : true,
								width : 150
							}, {
								header : '型号',
								dataIndex : 'modelType',
								width : 100
							}, {
								header : '数量',
								align : 'right',
								dataIndex : 'number',
								width : 80
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
								xtype : 'datecolumn',
								dataIndex : 'expectArrivalDate',
								format : 'Y-m-d',
								header : '期望到货日期',
								align : 'right',
								width : 180,
								sortable : true,
								editor : {
									xtype : 'datefield',
									autoWidth : true,
									format : 'Y-m-d',
									minValue : new Date()
								}
							}, {
								dataIndex : 'technicalRequirement',
								header : '技术要求',
								width : 120
							}, {
								id : autoExId,
								dataIndex : 'remark',
								header : '备注',
								editor : {
									xtype : 'textarea',
									anchor : '95%'
								},
								width : 120
							}]
				});
				com.bitc.xmis.device.PurchasingList.DetailList.superclass.initComponent
						.call(this);
			}
		});

/**
 * @class com.bitc.xmis.device.PurchasingList.List
 * @extends Ext.grid.GridPanel 待采购列表
 */
com.bitc.xmis.device.PurchasingList.TobePurchaseList = Ext.extend(
		Ext.grid.GridPanel, {
			constructor : function(ct, cfg) {
				var store = new Ext.data.GroupingStore({
					url : String.format(
							'{0}/device.do?method=getTobePurchaseList',
							__cxtPath),
					reader : new Ext.data.JsonReader(
							{
								root : 'rows',
								idProperty : 'projectDeviceId',
								totalProperty : 'totalCount'
							},
							com.bitc.xmis.device.PurchasingList.DetailListRecord),
					groupField : 'group',
					autoLoad : true
				});
				var colmuns = [{
							header : '设备名称及型号',
							dataIndex : 'group',
							hidden : true
						}, {
							header : '设备名称',
							dataIndex : 'name',
							width : 150,
							summaryType : 'count',
							summaryRenderer : function(v, params, data) {
								return '共' + v + '条采购申请';
							}
						}, {
							xtype : 'datecolumn',
							dataIndex : 'expectArrivalDate',
							format : 'Y-m-d',
							header : '期望到货日期',
							align : 'right',
							summaryType : 'min',
							summaryRenderer : function(v, params, data) {
								return '最早：' + Ext.util.Format.date(v, 'Y-m-d')
							},
							width : 120
						}, {
							header : '所属项目编码',
							dataIndex : 'projectCode',
							width : 140
						}, {
							header : '所属项目名称',
							dataIndex : 'projectName',
							width : 160
						}, {
							header : '部门',
							dataIndex : 'projectName',
							width : 100,
							dataIndex : 'dept',
							renderer : function(value) {
								return value.departmentName;
							}
						}, {
							header : '型号',
							dataIndex : 'modelType',
							width : 100
						}, {
							header : '数量',
							align : 'right',
							dataIndex : 'number',
							width : 100,
							summaryType : 'sum',
							summaryRenderer : function(v, params, data) {
								return '数量合计：' + v
							}
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
							dataIndex : 'technicalRequirement',
							header : '技术要求',
							width : 120
						}, {
							dataIndex : 'remark',
							header : '备注'
						}];
				if (ct.checkable) {
					var sm = new Ext.grid.CheckboxSelectionModel({
						fixed : false,
						summaryType : 'ids',
						summaryRenderer : function(v, params, data) {
							return '<div class="x-grid3-summary x-grid3-check-col" ids="'
									+ v.join(',')
									+ '" title="选择该设备所有申请">&#160;</div>';
						}
					});
					colmuns = [sm].concat(colmuns);
					Ext.apply(this, {
								sm : sm
							})
				}
				ct.tbar = ct.tbar || [];
				ct.tbar = ct.tbar.concat(['模糊查询：',
						new Ext.ux.InklingSearchField({
									width : 120,
									store : store,
									tooltip : {
										title : '模糊查询',
										text : '输入设备名称或设备型号进行查询。'
									},
									inklings : [{
												name : 'name',
												op : 'like',
												type : 'string'
											}, {
												name : 'modelType',
												op : 'like',
												type : 'string'
											}]
								})]);
				Ext.ux.grid.GroupSummary.Calculations['ids'] = function(v,
						record, field) {
					if (v == 0) {
						return [record.id];
					}
					return v.push(record.id);
				};
				var summary = new Ext.ux.grid.GroupSummary();
				// summary.afterMethod('doGroupEnd',function(){console.info('...')});

				Ext.apply(this, {
							store : store,
							sm : sm,
							view : new Ext.grid.GroupingView({
								showGroupName : false,
								enableNoGroups : false,
								enableGroupingMenu : false,
								emptyText : '无待采购设备'
									// hideGroupedColumn : true,
								}),
							plugins : [summary],
							columns : colmuns
						});
				com.bitc.xmis.device.PurchasingList.TobePurchaseList.superclass.constructor
						.call(this, ct, cfg);
				this.on('render', function(grid, rowIndex, columnIndex, e) {
					var view = this.getView();
					view.mainBody.on('mousedown', function(e, t) {
						if (Ext.fly(t).hasClass('x-grid3-summary')) {
							e.stopEvent();
							var hd = Ext.fly(t);
							var ids = hd.getAttribute("ids").split(',');
							var store = this.getStore();
							var rows = new Array();
							for (var i = 0; i < ids.length; i++) {
								rows.push(store.indexOfId(ids[i]));
							}
							var isChecked = hd.hasClass('x-grid3-check-col-on');
							if (isChecked) {
								hd.removeClass('x-grid3-check-col-on');
								for (var i = 0; i < rows.length; i++) {
									this.getSelectionModel()
											.deselectRow(rows[i]);
								}
							} else {
								hd.addClass('x-grid3-check-col-on');
								this.getSelectionModel().selectRows(rows, true);
							}
						}
					}, this);
				}, this);
			},
			stripeRows : true
		});

com.bitc.xmis.device.PurchasingList.List = Ext.extend(Ext.grid.GridPanel, {

	constructor : function(a, b) {
		var autoExId = Ext.id();
		var expander = new Ext.ux.grid.RowExpander({
			tpl : new Ext.XTemplate(
					'<table border="1" cellpadding="0" cellspacing="0" class="detail"> ',
					'<tr> <th width="40px"> 序号 </th> <th width="200px"> 名称 </th> <th width="100px"> 数量 </th> <th> 期望到货日期 </th></tr> ',
					'<tpl for="details"> <tr style="border:1px solid;"> <td> {#} </td> <td> {name} </td> <td style="text-align:right;"> {number} </td> <td>{expectArrivalDate}</td></tr> </tpl> </table>')
		});
		var store = new Ext.data.Store({
					url : __cxtPath + '/device.do?method=getPurchasingList',
					baseParams : {
						start : 0
					},
					reader : new Ext.data.JsonReader({
								idProperty : 'id',
								totalProperty : 'totalCount',
								root : 'rows'
							}, ["id", "code", "remark", {
										"mapping" : "details",
										"name" : "details",
										"type" : "auto"
									}, {
										"mapping" : "entryEmp",
										"name" : "entryEmp",
										"type" : "auto"
									}, {
										"mapping" : "dept",
										"name" : "dept",
										"type" : "auto"
									}, {
										"dateFormat" : "Y-m-d H:i:s",
										"mapping" : "entryDate",
										"name" : "entryDate",
										"type" : "date"
									}, {
										"mapping" : "supplier",
										"name" : "supplier",
										"type" : "auto"
									}])
				});
		Ext.apply(this, {
			autoExpandColumn : autoExId,
			store : store,
			plugins : [expander],
			viewConfig : {
				emptyText : '无采购清单'
			},
			columns : [expander, {
				header : '采购清单编码',
				dataIndex : 'code',
				width : 150,
				renderer : function(value, meta, record) {
					var url = String.format(
							'{0}/device/p.jsp?purchasingId={1}', __cxtPath,
							record.data.id);
					return String
							.format(
									'<a href="#" onclick="parent.xMis.turn(\'device-purchasing-{0}\',\'{1}\',\'{2}\')">{3}</a>',
									record.data.id, value, url, value)
				}
			}, {
				header : '采购部门',
				dataIndex : 'dept.departmentName',
				width : 120
			}, {
				header : '供应商',
				dataIndex : 'supplier.companyName',
				width : 150
			}, {
				header : '设备数量',
				align : 'right',
				dataIndex : 'details',
				width : 80,
				renderer : function(value) {
					if (value == null) {
						return '';
					}
					var sum = 0;
					for (var i = 0; i < value.length; i++) {
						sum += value[i].number;
					}
					return sum;
				}
			}, {
				dataIndex : 'entryEmp.userName',
				header : '创建人',
				width : 90
			}, {
				xtype : 'datecolumn',
				dataIndex : 'entryDate',
				header : '创建日期',
				align : 'right',
				format : 'Y-m-d',
				width : 90
			}, {
				id : autoExId,
				header : '备注',
				dataIndex : 'remark'
			}],
			loadMask : true,
			bbar : new Ext.PagingToolbar({
						store : store,
						pageSize : 20,
						displayInfo : true,
						plugins : new Ext.ux.PagelimitCombo()
					}),
			tbar : ['模糊查询：', new Ext.ux.InklingSearchField({
								width : 160,
								store : store,
								inklings : [{
											name : 'code',
											op : 'like',
											type : 'string'
										}, {
											name : 'dept.departmentName',
											op : 'like',
											type : 'string'
										}],
								tooltip : {
									title : '模糊查询',
									text : '输入采购清单编码、部门、供应商进行查询。'
								}
							})]
		});
		com.bitc.xmis.device.PurchasingList.List.superclass.constructor.call(
				this, a, b);
	}
});