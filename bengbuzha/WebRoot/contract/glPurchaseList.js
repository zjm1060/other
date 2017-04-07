Ext.onReady(function() {
	// Ext.QuickTips.init();
	var searchtype = '1'; // 1为财务来款，2为采购支出

	var glPurchaseDetailRecord = Ext.data.Record.create([{
				name : "year"
			}, {
				name : "iperiod"
			}, {
				name : "ino_id"
			}, {
				name : "MB"
			}, {
				name : "cdigest"
			}, {
				name : "dbill_date"
			}, {
				name : "project_code"
			}

	]);

	var glPurchaseDetailStore = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
					url : __cxtPath
							+ '/contractMain.do?method=getGLDetailList&glType=purchase'
				}),
		remoteSort : false,
		sortInfo : {
			field : 'dbill_date',
			direction : 'ASC'
		},

		reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount'
				}, glPurchaseDetailRecord)
	});

	glPurchaseDetailStore.on('load', function(store, records, options) {
				var glIncomeAmt = 0;

				for (var i = 0; i < records.length; i++) {
					var record = records[i];
					glIncomeAmt = glIncomeAmt
							+ Number(record.get('MB').toString());
				}
				if (searchtype == '1')
					glPurchaseDetailGrid
							.setTitle("合同来款合计约:&nbsp;&nbsp;&nbsp;&nbsp;"
									+ formatCurrency("",
											glIncomeAmt.toString(), "blue"));
				if (searchtype == '2')
					glPurchaseDetailGrid
							.setTitle("财务采购支出合计约:&nbsp;&nbsp;&nbsp;&nbsp;"
									+ formatCurrency("",
											glIncomeAmt.toString(), "blue"));
			}, this, {
				delay : 100
			})

	Ext.grid.RowNumberer = Ext.extend(Ext.grid.RowNumberer, {
				renderer : function(value, cellmeta, record, rowIndex,
						columnIndex, store) {
					return rowIndex + 1;
				}
			});

	var glPurchaseDetailColM = new Ext.grid.ColumnModel([
			new Ext.grid.RowNumberer(),

			{
				header : "凭单号",
				dataIndex : 'ino_id',
				sortable : true,
				align : 'right'
			}, {
				header : "记账日期",
				dataIndex : 'dbill_date',
				sortable : true,
				renderer : customer_formatDate,
				align : 'center'
			},

			{
				header : "采购支付金额",
				dataIndex : 'MB',
				sortable : true,
				renderer : function(value, cellMeta, record, rowIndex,
						columnIndex, store) {
					return formatCurrency(null, value, 'blue');
				},
				align : 'right'
			}, {
				header : "备注",
				dataIndex : 'cdigest',
				sortable : true,
				renderer : function(value, cellMeta, record, rowIndex,
						columnIndex, store) {
					if (value != null) {
						var baseNum = 18;
						var msg = '';
						for (var i = 0; i < value.length / baseNum; i++) {
							msg += value.substr(i * baseNum, baseNum) + "<br/>";
						}
						cellMeta.attr = 'ext:qtitle="备注" ext:qtip="' + msg
								+ '"';
					}
					return value;
				}
			}

	]);

	var exp = new Ext.ux.Export({
				formater : [{
							columnHeader : '',
							formatClass : 'com.bitc.jk.xmis.export.RowNumber'
						}, {
							columnHeader : '记账日期',
							formatClass : 'com.bitc.jk.xmis.export.SubstringTimer'
						}, {
							columnHeader : '采购支付金额',
							formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
						}]
			});

	var glPurchaseDetailGrid = new Ext.grid.GridPanel({
				id : 'glPurchaseDetailGrid',
				title : '采购支付明细列表',
				cm : glPurchaseDetailColM,
				store : glPurchaseDetailStore,
				// autoExpandColumn: 'abb',
				stripeRows : true,
				layout : 'fit',
				enableColumnMove : false, // True表示启动对Column顺序的拖拽,反之禁用
				enableColumnResize : true, // 设置为false可以禁止这个表格的列改变宽度 (默认值为
				// true).
				stripeRows : true, // True表示使用不同颜色间隔行，默认为false
				autoHeight : false,
				autoWidth : false,
				// height:Ext.get("pcWin").getHeight(),
				height : 400,// Ext.get("childContractList").getHeight(),
				width : '100%',
				loadMask : true, // 读取数据时的遮罩和提示功能，默认为false
				// config options for stateful behavior
				stateful : true,
				stateId : 'contract-payment-list-grid',
				forceFit : true, // Ture表示自动扩展或缩小列的宽度以适应grid的宽度,从而避免出现水平滚动条.
				loadMask : {
					msg : '正在加载数据，请稍侯……'
				},
				viewConfig : {
					forceFit : true
				}
				// collapsible: true,
				// collapsed: true,
				,
				tbar : [{
							xtype : 'button',
							text : '导出',
							iconCls : 'xls',
							handler : function() {
								exp.onExport(glPurchaseDetailGrid, '财务采购支出明细列表',
										false);
							}
						}]
			});

	var glPurchaseDetailWin;

	this.showGLPurchaseListWindow = function(contractAbb, contractID, type) {
		// Ext.get('glPurchaseListLink_'+contractID).on('click', function(){

		if (!glPurchaseDetailWin) {
			glPurchaseDetailWin = new Ext.Window({
						modal : true,
						title : type == '2' ? "财务采购支出明细列表" : "财务来款明细列表",
						renderTo : Ext.getBody(),
						frame : true,
						plain : true,
						resizable : false,
						closeAction : "hide",
						// maximizable:true,
						closable : true,
						bodyStyle : "padding:10px",
						width : 800,// '100%',
						height : 447,
						items : [glPurchaseDetailGrid]
					})
		}
		glPurchaseDetailStore.removeAll(true);

		glPurchaseDetailStore.load({
					params : {
						contractID : contractID
					},
					callback : function(records, options, success) {
						if (success == true) {
						} else {
							// Ext.Msg.alert(Ext_errMsg_For_Get_Store_Title
							// ,Ext_errMsg_For_Get_Store_Message );
						}
					}

				});
		searchtype = type;
		var title = (type == '2') ? "财务采购支出明细列表" : "财务来款明细列表";
		glPurchaseDetailWin.setTitle(title + " --  " + contractAbb);
		glPurchaseDetailWin.show();

		// });

	}

});
