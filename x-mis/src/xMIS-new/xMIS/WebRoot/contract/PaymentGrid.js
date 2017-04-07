Ext.ns('xMis.contract');
xMis.grid.PaymentGrid = Ext.extend(Ext.grid.GridPanel, {
	constructor : function(config) {
		// Create configuration for this Grid.
		var paymentDetailRecord = Ext.data.Record.create([{
					name : paymentChargedDate
				}, {
					name : paymentAEmpName
				}, {
					name : paymentChargedAmt
				}, {
					name : paymentBEmpName
				}, {
					name : paymentUpdateDate
				}, {
					name : paymentCurrencyGraph
				}, {
					name : paymentCurrencyExchangeRate
				}, {
					name : paymentChargedRemark
				}]);
		var paymentDetailStore = new Ext.data.Store({
					proxy : new Ext.data.HttpProxy({
								url : __cxtPath
										+ '/contractMain.do?method=getPaymentList'
							}),
					remoteSort : false,
					sortInfo : {
						field : paymentChargedDate,
						direction : 'ASC'
					},
					reader : new Ext.data.JsonReader({
								root : 'rows',
								totalProperty : 'totalCount'
							}, paymentDetailRecord)
				});
		paymentDetailStore.on('load', function(store, records, options) {
					var paymentAmt = 0;

					for (var i = 0; i < records.length; i++) {
						var record = records[i];
						paymentAmt += formatCurrency("null", record
										.get(paymentCurrencyExchangeRate), "")
								* formatCurrency("null", record
												.get(paymentChargedAmt), "");
					}

					paymentDetailGrid
							.setTitle("已付合同款合计约:&nbsp;&nbsp;&nbsp;&nbsp;"
									+ formatCurrency("", paymentAmt.toString(),
											"blue"));
				}, this, {
					delay : 100
				});
		Ext.grid.RowNumberer = Ext.extend(Ext.grid.RowNumberer, {
					renderer : function(value, cellmeta, record, rowIndex,
							columnIndex, store) {
						return rowIndex + 1;
					}
				});
		var paymentDetailColM = new Ext.grid.ColumnModel([
				new Ext.grid.RowNumberer(), {
					header : "收款日期",
					dataIndex : paymentChargedDate,
					sortable : true,
					renderer : customer_formatDate,
					align : 'center'
				}, {
					header : "收款金额",
					dataIndex : paymentChargedAmt,
					sortable : true,
					renderer : function(value, cellMeta, record, rowIndex,
							columnIndex, store) {
						return formatCurrency(
								record.data[paymentCurrencyGraph], value,
								'blue');
					},
					align : 'right'
				}, {
					header : "经办人",
					dataIndex : paymentAEmpName,
					sortable : true,
					renderer : convertUserName
				}, {
					header : "登记日期",
					dataIndex : paymentUpdateDate,
					sortable : true,
					renderer : customer_formatDate,
					align : 'center'
				}, {
					header : "登记人",
					dataIndex : paymentBEmpName,
					sortable : true,
					renderer : convertUserName
				}, {
					header : "备注",
					dataIndex : paymentChargedRemark,
					sortable : true
				}

		]);
		// Create a new config object containing our computed properties
		// *plus* whatever was in the config parameter.
		config = Ext.apply({
					id : 'paymentDetailGrid',
					title : '收款明细列表',
					cm : paymentDetailColM,
					store : paymentDetailStore,
					stripeRows : true,
					layout : 'fit',
					enableColumnMove : false, // True表示启动对Column顺序的拖拽,反之禁用
					enableColumnResize : true, // 设置为false可以禁止这个表格的列改变宽度 (默认值为
					// true).
					stripeRows : true, // True表示使用不同颜色间隔行，默认为false
					autoHeight : false,
					autoWidth : false,
					height : 300,
					width : '100%',
					loadMask : true, // 读取数据时的遮罩和提示功能，默认为false
					// config options for stateful behavior
					forceFit : true, // Ture表示自动扩展或缩小列的宽度以适应grid的宽度,从而避免出现水平滚动条.
					loadMask : {
						msg : '正在加载数据，请稍侯……'
					},
					viewConfig : {
						forceFit : true
					}
				}, config);
		MyGridPanel.superclass.constructor.call(this, config);
		// Your postprocessing here
	}
});