Ext.onReady(function() {
	// Ext.QuickTips.init();

	var glElseIncomeDetailRecord = Ext.data.Record.create([{
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

	var glElseIncomeDetailStore = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
			url : __cxtPath
					+ '/contractMain.do?method=getGLDetailList&glType=elseincome'
		}),
		remoteSort : false,
		sortInfo : {
			field : 'dbill_date',
			direction : 'ASC'
		},

		reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount'
				}, glElseIncomeDetailRecord)
	});

	glElseIncomeDetailStore.on('load', function(store, records, options) {
				var glIncomeAmt = 0.0;

				for (var i = 0; i < records.length; i++) {
					var record = records[i];
					glIncomeAmt = glIncomeAmt
							+ Number(record.get('MB').toString());
				}
				glElseIncomeDetailGrid
						.setTitle("合同来款合计约:&nbsp;&nbsp;&nbsp;&nbsp;"
								+ formatCurrency("", glIncomeAmt.toString(),
										"blue"));
			}, this, {
				delay : 100
			})

	Ext.grid.RowNumberer = Ext.extend(Ext.grid.RowNumberer, {
				renderer : function(value, cellmeta, record, rowIndex,
						columnIndex, store) {
					return rowIndex + 1;
				}
			});

	var glIncomeDetailColM = new Ext.grid.ColumnModel([
			new Ext.grid.RowNumberer(), {
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
			}, {
				header : "来款金额",
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
			}]);

	var exp = new Ext.ux.Export({
				formater : [{
							columnHeader : '',
							formatClass : 'com.bitc.jk.xmis.export.RowNumber'
						}, {
							columnHeader : '记账日期',
							formatClass : 'com.bitc.jk.xmis.export.SubstringTimer'
						}, {
							columnHeader : '来款金额',
							formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
						}]
			});

	var glElseIncomeDetailGrid = new Ext.grid.GridPanel({
				region : 'center',
				id : 'glElseIncomeDetailGrid',
//				title : '其他财务来款明细列表',
				title : '退税收入明细列表',
				cm : glIncomeDetailColM,
				store : glElseIncomeDetailStore,
				stripeRows : true,
				layout : 'fit',
				enableColumnMove : false,
				enableColumnResize : true,
				stripeRows : true,
				autoHeight : false,
				autoWidth : false,
				height : 380,
				width : '100%',
				loadMask : true,
				stateful : true,
				stateId : 'contract-payment-list-grid',
				forceFit : true,
				loadMask : {
					msg : '正在加载数据，请稍侯……'
				},
				viewConfig : {
					forceFit : true
				},
				tbar : [{
					xtype : 'button',
					text : '导出',
					iconCls : 'xls',
					handler : function() {
						exp.onExport(glElseIncomeDetailGrid, '退税收入明细列表',
								false);
					}
				}]
			});

	var glElseIncomeDetailWin;

	this.showGLElseIncomeListWindow = function(contractAbb, contractID) {

		if (!glElseIncomeDetailWin) {
			glElseIncomeDetailWin = new Ext.Window({
						modal : true,
						title : "退税收入明细列表",
						renderTo : Ext.getBody(),
						frame : true,
						plain : true,
						resizable : false,
						closeAction : "hide",
						closable : true,
						bodyStyle : "padding:10px",
						width : 800,
						height : 430,
						layout : 'border',
						items : [glElseIncomeDetailGrid]
					})
		}
		glElseIncomeDetailStore.removeAll(true);

		glElseIncomeDetailStore.load({
					params : {
						contractID : contractID
					},
					callback : function(records, options, success) {
						if (success == true) {

						} else {
							Ext.Msg.alert(Ext_errMsg_For_Get_Store_Title,
									Ext_errMsg_For_Get_Store_Message);
						}
					}

				});

		glElseIncomeDetailWin.setTitle("退税收入明细列表 --  " + contractAbb);
		glElseIncomeDetailWin.show();

	}

});
