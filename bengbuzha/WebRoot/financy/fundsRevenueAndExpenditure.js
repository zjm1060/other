Ext.onReady(function() {
	var today = new Date();
	var month = today.getMonth() + 1;
	var year = today.getFullYear();
	pageSize = 0;
	var record = Ext.data.Record.create([{
				name : 'number',
				mapping : 'number'
			}, {
				name : '项目',
				mapping : '项目'
			}, {
				name : '序号',
				mapping : '序号'
			}, {
				name : '内容',
				mapping : '内容'
			}, {
				name : '1',
				mapping : 'Jan'
			}, {
				name : '2',
				mapping : 'Feb'
			}, {
				name : '3',
				mapping : 'Mar'
			}, {
				name : '4',
				mapping : 'Apr'
			}, {
				name : '5',
				mapping : 'May'
			}, {
				name : '6',
				mapping : 'Jun'
			}, {
				name : '7',
				mapping : 'Jul'
			}, {
				name : '8',
				mapping : 'Aug'
			}, {
				name : '9',
				mapping : 'Sep'
			}, {
				name : '10',
				mapping : 'Oct'
			}, {
				name : '11',
				mapping : 'Nov'
			}, {
				name : '12',
				mapping : 'Dec'
			}, {
				name : '金额',
				mapping : '金额'
			}]);
	var columnModel = new Ext.grid.ColumnModel([{
		header : '项目',
		dataIndex : '项目',
		hidden : false,
		width : 235,
		sortable : false,
		renderer : function(value) {

			var count = 0;
			var index = 0;
			var str = value;

			while ((index = str.indexOf('\t')) != -1) {
				str = str.substring(index + '\t'.length);
				count++;
			}

			if (count == 0) {
				return value;
			} else if (count == 1) {
				return '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;' + value;
			} else if (count == 2) {
				return '&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'
						+ value;
			}
		}
	}, {
		header : '一月',
		dataIndex : '1',
		hidden : false,
		width : 100,
		sortable : false,
		align : 'right',
		renderer : function(value) {
			return formatAmt(value);
		}
	}, {
		header : '二月',
		dataIndex : '2',
		hidden : false,
		width : 100,
		sortable : false,
		align : 'right',
		renderer : function(value) {
			return formatAmt(value);
		}
	}, {
		header : '三月',
		dataIndex : '3',
		hidden : false,
		width : 100,
		sortable : false,
		align : 'right',
		renderer : function(value) {
			return formatAmt(value);
		}
	}, {
		header : '四月',
		dataIndex : '4',
		hidden : false,
		width : 100,
		sortable : false,
		align : 'right',
		renderer : function(value) {
			return formatAmt(value);
		}
	}, {
		header : '五月',
		dataIndex : '5',
		hidden : false,
		width : 100,
		sortable : false,
		align : 'right',
		renderer : function(value) {
			return formatAmt(value);
		}
	}, {
		header : '六月',
		dataIndex : '6',
		hidden : false,
		width : 100,
		sortable : false,
		align : 'right',
		renderer : function(value) {
			return formatAmt(value);
		}
	}, {
		header : '七月',
		dataIndex : '7',
		hidden : false,
		width : 100,
		sortable : false,
		align : 'right',
		renderer : function(value) {
			return formatAmt(value);
		}
	}, {
		header : '八月',
		dataIndex : '8',
		hidden : false,
		width : 100,
		sortable : false,
		align : 'right',
		renderer : function(value) {
			return formatAmt(value);
		}
	}, {
		header : '九月',
		dataIndex : '9',
		hidden : false,
		width : 100,
		sortable : false,
		align : 'right',
		renderer : function(value) {
			return formatAmt(value);
		}
	}, {
		header : '十月',
		dataIndex : '10',
		hidden : false,
		width : 100,
		sortable : false,
		align : 'right',
		renderer : function(value) {
			return formatAmt(value);
		}
	}, {
		header : '十一月',
		dataIndex : '11',
		hidden : false,
		width : 100,
		sortable : false,
		align : 'right',
		renderer : function(value) {
			return formatAmt(value);
		}
	}, {
		header : '十二月',
		dataIndex : '12',
		hidden : false,
		width : 100,
		sortable : false,
		align : 'right',
		renderer : function(value) {
			return formatAmt(value);
		}
	}, {
		header : '本年累计',
		dataIndex : '金额',
		hidden : false,
		width : 100,
		sortable : false,
		align : 'right',
		renderer : function(value) {
			return formatAmt(value);
		}
	}]);

	// 在月份之前只有一列
	for (var i = 12; i > month; i--) {
		columnModel.setHidden(i, true);
	}

	var store = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
			url : __cxtPath
					+ '/financyMain.do?method=getFundsRevenueAndExpenditureStore'
		}),
		baseParams : {
			start : 0,
			limit : pageSize,
			executeString : '{call xmis.sp_fundsRevenueAndExpenditure(?)}',
			type : 1
		},
		// romoteSort : true,
		reader : new Ext.data.JsonReader({
					root : 'rows',
					totalProperty : 'totalCount'
				}, record)
	});

	var exp = new Ext.ux.Export({
				formater : [{
							columnHeader : '一月',
							formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
						}, {
							columnHeader : '二月',
							formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
						}, {
							columnHeader : '三月',
							formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
						}, {
							columnHeader : '四月',
							formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
						}, {
							columnHeader : '五月',
							formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
						}, {
							columnHeader : '六月',
							formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
						}, {
							columnHeader : '七月',
							formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
						}, {
							columnHeader : '八月',
							formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
						}, {
							columnHeader : '九月',
							formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
						}, {
							columnHeader : '十月',
							formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
						}, {
							columnHeader : '十一月',
							formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
						}, {
							columnHeader : '十二月',
							formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
						}, {
							columnHeader : '本年累计',
							formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
						}]
			});

	var grid = new Ext.grid.GridPanel({
				title : year + '年资金流量统计表',
				region : 'center',
				store : store,
				cm : columnModel,
				bodyStyle : 'width : 33%',
				// enableHdMenu : false,//右键菜单不能隐藏，导出有问题
				// enableColumnHide : false,
				loadMask : true,
				loadMask : {
					msg : '正在加载数据，请稍候...'
				},
				tbar : [{
							ref : '../Barchart',
							iconCls : 'chart_bar',
							text : '柱状图表',
							handler : function(b, e) {
								showHistogramWin();
							}
						}],
				bbar : new Ext.ux.TotalPagingToolbar({
							pageSize : 50,
							store : store,
							displayInfo : true,
							displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
							emptyMsg : "无显示数据",
							plugins : [exp, new Ext.ux.PagelimitCombo()]
						})
			});

	new Ext.Viewport({
				border : false,
				layout : 'border',
				items : [grid]
			});
	grid.getStore().on('load', function(s, records) {
		var gridcount = 0;
		s.each(function(r) {
			if (r.get('项目').indexOf('\t') == -1) {
				grid.getView().getRow(gridcount).style.backgroundColor = '#FFFF00';// #DCDCDC
			}
			gridcount = gridcount + 1;
		});
	});
	store.load();

	this.formatAmt = function(value) {
		return Ext.util.Format.number(value, '0,000.00');
	}

	this.showHistogramWin = function() {

		var pageSize = 0;
		var record_histogram = Ext.data.Record.create([{
					name : '月份',
					mapping : '月份'
				}, {
					name : '资金流入',
					mapping : '资金流入'
				}, {
					name : '资金流出',
					mapping : '资金流出'
				}]);

		var store_histogram = new Ext.data.Store({
			proxy : new Ext.data.HttpProxy({
				url : __cxtPath
						+ '/financyMain.do?method=getFundsRevenueAndExpenditureStore'
			}),
			baseParams : {
				start : 0,
				limit : pageSize,
				executeString : '{call xmis.sp_fundsRevenueAndExpenditure(?)}',
				type : 2
			},
			reader : new Ext.data.JsonReader({
						root : 'rows',
						totalProperty : 'totalCount'
					}, record_histogram),
			autoLoad : true
		});

		var histogram = new Ext.Panel({
			region : 'center',
			frame : true,
			layout : 'fit',
			title : '<font color="red">柱状图右键单击选择"Export to JPG"或者"Export to PNG"可导出保存为图片</font>',
			items : {
				xtype : 'columnchart',
				store : store_histogram,
				url : __cxtPath + '/resources/charts.swf',
				xField : '月份',// X轴显示的值
				yAxis : new Ext.chart.NumericAxis({
							displayName : '资金流出',
							labelRenderer : Ext.util.Format
									.numberRenderer('0,0')
						}), // Y 轴显示的值
				tipRenderer : function(chart, record, index, series) {
					if (series.yField == '资金流入') {
						return formatAmt(record.data['资金流入']);
					} else {
						return formatAmt(record.data['资金流出']);
					}
				},
				chartStyle : {
					padding : 10,
					animationEnabled : true,// 是否支持动态数据变化
					font : {// 图表整体字体
						name : 'Tahoma',
						color : 'silver',
						size : 11
					},
					border : {
		// color:'#3399FF',
					// size:1
					},
					background : {
						color : '#CCCCCC',
						alpha : 0.1
					},
					legend : {// 图例
						display : "bottom",
						spacing : 2,
						padding : 5,
						font : {
							name : 'Tahoma',
							color : '#3366FF',
							size : 12,
							bold : true
						}
					},
					dataTip : { // 鼠标经过,数据提示样式
						padding : 5,
						border : {
							color : "#66CCFF",
							size : 1
						},
						background : {
							color : 0xDAE7F6,
							alpha : .8
							// 背景颜色透明度
						},
						font : {
							name : 'Tahoma',
							// color : '#FF3300',
							size : 10,
							bold : true
						}
					},
					xAxis : { // X 轴
						color : 0x69aBc8, // X轴颜色
						majorTicks : {// 大刻度值
							color : 0x69aBc8,
							length : 4
						},
						minorTicks : {// 小刻度值
							color : 0x69aBc8,
							length : 2
						},
						majorGridLines : {
							size : 1,
							color : 0xeeeeee
						},
						// showLabels:false,
						labelDistance : 4
					},
					yAxis : {
						color : 0x69aBc8,
						majorTicks : {// 大刻度值
							color : 0x69aBc8,
							length : 4
						},
						minorTicks : {// 小刻度值
							color : 0x69aBc8,
							length : 2
						},
						majorGridLines : {
							size : 1,
							color : 0xeeeeee
						}
					}
				},
				series : [{
							type : 'column',
							displayName : '资金流入',
							yField : '资金流入',
							style : {
								color : '#FF3300'
							}
						}, {
							type : 'column',
							displayName : '资金流出',
							yField : '资金流出',
							style : {
								color : '#66CCFF'
							}
						}]
			}
		});

		var record_histogramgrid = Ext.data.Record.create([{
					name : 'number',
					mapping : 'number'
				}, {
					name : '项目',
					mapping : '项目'
				}, {
					name : '序号',
					mapping : '序号'
				}, {
					name : '内容',
					mapping : '内容'
				}, {
					name : '1',
					mapping : 'Jan'
				}, {
					name : '2',
					mapping : 'Feb'
				}, {
					name : '3',
					mapping : 'Mar'
				}, {
					name : '4',
					mapping : 'Apr'
				}, {
					name : '5',
					mapping : 'May'
				}, {
					name : '6',
					mapping : 'Jun'
				}, {
					name : '7',
					mapping : 'Jul'
				}, {
					name : '8',
					mapping : 'Aug'
				}, {
					name : '9',
					mapping : 'Sep'
				}, {
					name : '10',
					mapping : 'Oct'
				}, {
					name : '11',
					mapping : 'Nov'
				}, {
					name : '12',
					mapping : 'Dec'
				}, {
					name : '金额',
					mapping : '金额'
				}]);

		var columnModel_histogramgrid = new Ext.grid.ColumnModel([{
					header : '项目',
					dataIndex : '项目',
					hidden : false,
					width : 235,
					sortable : false,
					renderer : function(value) {
						return value;
					}
				}, {
					header : '一月',
					dataIndex : '1',
					hidden : false,
					width : 100,
					sortable : false,
					align : 'right',
					renderer : function(value) {
						return formatAmt(value);
					}
				}, {
					header : '二月',
					dataIndex : '2',
					hidden : false,
					width : 100,
					sortable : false,
					align : 'right',
					renderer : function(value) {
						return formatAmt(value);
					}
				}, {
					header : '三月',
					dataIndex : '3',
					hidden : false,
					width : 100,
					sortable : false,
					align : 'right',
					renderer : function(value) {
						return formatAmt(value);
					}
				}, {
					header : '四月',
					dataIndex : '4',
					hidden : false,
					width : 100,
					sortable : false,
					align : 'right',
					renderer : function(value) {
						return formatAmt(value);
					}
				}, {
					header : '五月',
					dataIndex : '5',
					hidden : false,
					width : 100,
					sortable : false,
					align : 'right',
					renderer : function(value) {
						return formatAmt(value);
					}
				}, {
					header : '六月',
					dataIndex : '6',
					hidden : false,
					width : 100,
					sortable : false,
					align : 'right',
					renderer : function(value) {
						return formatAmt(value);
					}
				}, {
					header : '七月',
					dataIndex : '7',
					hidden : false,
					width : 100,
					sortable : false,
					align : 'right',
					renderer : function(value) {
						return formatAmt(value);
					}
				}, {
					header : '八月',
					dataIndex : '8',
					hidden : false,
					width : 100,
					sortable : false,
					align : 'right',
					renderer : function(value) {
						return formatAmt(value);
					}
				}, {
					header : '九月',
					dataIndex : '9',
					hidden : false,
					width : 100,
					sortable : false,
					align : 'right',
					renderer : function(value) {
						return formatAmt(value);
					}
				}, {
					header : '十月',
					dataIndex : '10',
					hidden : false,
					width : 100,
					sortable : false,
					align : 'right',
					renderer : function(value) {
						return formatAmt(value);
					}
				}, {
					header : '十一月',
					dataIndex : '11',
					hidden : false,
					width : 100,
					sortable : false,
					align : 'right',
					renderer : function(value) {
						return formatAmt(value);
					}
				}, {
					header : '十二月',
					dataIndex : '12',
					hidden : false,
					width : 100,
					sortable : false,
					align : 'right',
					renderer : function(value) {
						return formatAmt(value);
					}
				}, {
					header : '本年累计',
					dataIndex : '金额',
					hidden : false,
					width : 100,
					sortable : false,
					renderer : function(value) {
						return formatAmt(value);
					}
				}]);

		var store_histogramgrid = new Ext.data.Store({
			proxy : new Ext.data.HttpProxy({
				url : __cxtPath
						+ '/financyMain.do?method=getFundsRevenueAndExpenditureStore'
			}),
			baseParams : {
				start : 0,
				limit : pageSize,
				executeString : '{call xmis.sp_fundsRevenueAndExpenditure(?)}',
				type : 3
			},
			reader : new Ext.data.JsonReader({
						root : 'rows',
						totalProperty : 'totalCount'
					}, record_histogramgrid)
		});

		var histogramgrid = new Ext.grid.GridPanel({
					height : 150,
					region : 'south',
					cm : columnModel_histogramgrid,
					store : store_histogramgrid,
					bodyStyle : 'width : 33%',
					loadMask : true,
					loadMask : {
						msg : '正在加载数据，请稍候...'
					}
				});

		var today = new Date();
		var month = today.getMonth() + 1;
		for (var i = 12; i > month; i--) {
			columnModel_histogramgrid.setHidden(i, true);
		}

		store_histogramgrid.load();

		var histogramWin = new Ext.Window({

					id : 'histogramWin',
					title : year + '年资金流量图',
					height : 500,
					width : 880,
					closeAction : 'close',
					layout : 'border',
					items : [histogram, histogramgrid]
				});
		histogramWin.show();
	}
});