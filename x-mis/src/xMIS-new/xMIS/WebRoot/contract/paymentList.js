Ext.onReady(function() {
	// Ext.QuickTips.init();
	var contractAb = ''; // 合同简称

	var paymentDetailRecord = Ext.data.Record.create([{
				name : paymentChargedId
			}, {
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
			}

	]);

	var paymentDetailStore = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
					url : __cxtPath + '/contractMain.do?method=getPaymentList'
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
			// ,
			// listeners:{
			// exception: function(proxy, type, action, options, res, arg){
			// alert(type);
			// // if(type == 'remote'){
			// // Ext.Msg.show({
			// // title: 'REMOTE EXCEPTION',
			// // msg: res.message,
			// // icon: Ext.MessageBox.ERROR,
			// // buttons: Ext.Msg.OK
			// // });
			// // }
			// }
			// }

		});

	paymentDetailStore.on('load', function(store, records, options) {
		var paymentAmt = 0;

		for (var i = 0; i < records.length; i++) {
			var record = records[i];
			paymentAmt += formatCurrency("null", record
							.get(paymentCurrencyExchangeRate), "")
					* formatCurrency("null", record.get(paymentChargedAmt), "");
		}

		paymentDetailGrid.setTitle("已付合同款合计约:&nbsp;&nbsp;&nbsp;&nbsp;"
				+ formatCurrency("", paymentAmt.toString(), "blue"));
	}, this, {
		delay : 100
	})

	Ext.grid.RowNumberer = Ext.extend(Ext.grid.RowNumberer, {
				renderer : function(value, cellmeta, record, rowIndex,
						columnIndex, store) {
					return rowIndex + 1;
				}
			});

	var paymentDetailColM = new Ext.grid.ColumnModel([
					new Ext.grid.RowNumberer(),

					{
						header : "收款日期",
						dataIndex : paymentChargedDate,
						width : 80,
						sortable : true,
						renderer : customer_formatDate,
						align : 'center'
					},

					{
						header : "收款金额",
						dataIndex : paymentChargedAmt,
						width : 80,
						sortable : true,
						renderer : function(value, cellMeta, record, rowIndex,
								columnIndex, store) {
							return formatCurrency(
									record.data[paymentCurrencyGraph], value,
									'blue');
						},
						align : 'right'
					},

					{
						header : "经办人",
						dataIndex : paymentAEmpName,
						width : 75,
						sortable : true,
						renderer : convertUserName
					}, {
						header : "登记日期",
						dataIndex : paymentUpdateDate,
						width : 80,
						sortable : true,
						renderer : customer_formatDate,
						align : 'center'
					}, {
						header : "登记人",
						dataIndex : paymentBEmpName,
						width : 75,
						sortable : true,
						renderer : convertUserName
					}, {
						header : "备注",
						dataIndex : paymentChargedRemark,
						sortable : true,
						renderer : function(value, cellMeta, record, rowIndex,
								columnIndex, store) {
							if (value != null) {
								var baseNum = 19;
								var msg = '';
								for (var i = 0; i < value.length / baseNum; i++) {
									msg += value.substr(i * baseNum, baseNum)
											+ "<br/>";
								}
								cellMeta.attr = 'ext:qtitle="备注" ext:qtip="'
										+ msg + '"';
							}
							return value;
						}
					}

			]);

	var paymentDetailGrid = new Ext.grid.GridPanel({
		id : 'paymentDetailGrid',
		title : '收款明细列表',
		cm : paymentDetailColM,
		store : paymentDetailStore,
		// autoExpandColumn: 'abb',
		stripeRows : true,
		layout : 'fit',
		enableColumnMove : false, // True表示启动对Column顺序的拖拽,反之禁用
		enableColumnResize : true, // 设置为false可以禁止这个表格的列改变宽度 (默认值为 true).
		stripeRows : true, // True表示使用不同颜色间隔行，默认为false
		autoHeight : false,
		autoWidth : false,
		// height:Ext.get("pcWin").getHeight(),
		height : 380,// Ext.get("childContractList").getHeight(),
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
			getRowClass : function(record, rowIndex, rowParams, store) {
				if (record.get(paymentChargedId) == -1) {
					paymentDetailWin
							.setTitle("收款明细列表 --  "
									+ contractAb
									+ "<font color=red>&nbsp;(当前信息是通过财务系统计算得出的初始化数据！)</font>");
					return 'fake_date_Red';
				}
			},
			forceFit : true
		}
			// collapsible: true,
			// collapsed: true,

	});

	var paymentDetailWin;

	this.showPaymentListWindow = function(contractAbb, contractID) {
		contractAb = contractAbb;
		// Ext.get('paymentListLink_'+contractID).on('click', function(){
		if (!paymentDetailWin) {
			paymentDetailWin = new Ext.Window({
						modal : true,
						title : "收款明细列表",
						renderTo : Ext.getBody(),
						frame : true,
						plain : true,
						resizable : false,
						closeAction : "hide",
						// maximizable:true,
						closable : true,
						bodyStyle : "padding:10px",
						width : 800,// '100%',
						height : 430,
						items : [paymentDetailGrid]
					})
		}
		paymentDetailStore.removeAll(true);

		paymentDetailStore.load({
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

		paymentDetailWin.setTitle("收款明细列表 --  " + contractAbb);
		paymentDetailWin.show();

		// });

	}

});
