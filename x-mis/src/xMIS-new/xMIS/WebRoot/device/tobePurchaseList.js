// 待采购清单
Ext.onReady(function() {
			Ext.QuickTips.init();
			var tobeList = new com.bitc.xmis.device.PurchasingList.TobePurchaseList(
					{
						border : false,
						checkable : true,
						region : 'center',
						tbar : [{
							iconCls : 'add',
							text : '创建采购清单',
							handler : function() {
								var sm = tobeList.getSelectionModel();
								if (sm.getSelections().length == 0) {
									Ext.Msg.alert('提示', '您未选择任何设备。请先在列表中勾选设备。');
									return;
								}
								var records = sm.getSelections();
								var datas = com.bitc.xmis.util.ExtUtil
										.encodeRecords(records);
								var url = encodeURI(__cxtPath
										+ '/device/p.jsp?details=' + datas);
								if (parent && parent.xMis) {
									parent.xMis.turn('device-purchase-new',
											'新建采购清单', url);
								}
							}
						}, '-']
					})
			new Ext.Viewport({
						border : false,
						layout : 'border',
						items : [tobeList]
					});
		});