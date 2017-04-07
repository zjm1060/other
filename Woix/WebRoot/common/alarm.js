Ext.onReady(function() {
	Ext.QuickTips.init();
	/**
	 * 报警一览表主体显示区
	 * 
	 */

	// 1.时间默认当天的0-24点，带时分秒
	var minDate = new Date();
	minDate.setHours(0);
	minDate.setMinutes(0);
	minDate.setSeconds(0);
	minDate.getTime();

	var maxDate = new Date();
	maxDate.setHours(23);
	maxDate.setMinutes(59);
	maxDate.setSeconds(59);
	maxDate.getTime();

	var pageSize = 20;

	var startInx = 0;
	var endInx = startInx + pageSize;

	var timer;// 定时刷新时间间隔

	var alarmRecord = new Ext.data.Record.create([{
				name : 'NO',
				mapping : 'NO'
			}, {
				name : 'DATETIME',
				mapping : 'DATETIME'
			}, {
				name : 'POINTNAME',
				mapping : 'POINTNAME'
			}, {
				name : 'LONGNAME',
				mapping : 'LONGNAME'
			}, {
				name : 'STATUS',
				mapping : 'STATUS'
			}, {
				name : 'HOST',
				mapping : 'HOST'
			}]);

	var alarmStore = new Ext.data.Store({
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount'
						}, alarmRecord)
			});

	var alarmColumn = new Ext.grid.ColumnModel([{
				header : '序号',
				dataIndex : 'NO',
				sortable : true,
				width : 100
			}, {
				header : '时间',
				dataIndex : 'DATETIME',
				sortable : true,
				width : 180
			}, {
				header : '点名',
				dataIndex : 'POINTNAME',
				sortable : true,
				width : 180
			}, {
				header : '中文名',
				dataIndex : 'LONGNAME',
				sortable : true,
				width : 320
			}, {
				header : '状态',
				dataIndex : 'STATUS',
				sortable : true,
				width : 180
			}, {
				header : '主机名',
				dataIndex : 'HOST',
				sortable : true,
				width : 180
			}]);

	var alarmGrid = new Ext.grid.GridPanel({
		id : 'alarmGrid',
		region : 'center',
		store : alarmStore,
		cm : alarmColumn,
		stripeRows : true,
		layout : 'fit',
		border : false,
		autoScroll : true,
		loadMask : true,
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		tbar : ['开始时间', new Ext.form.DateField({
							ref : '../startDate',
							format : 'Y-m-d H:i:s',
							showToday : true,
							// value : minDate.format('Y-m-d H:i:s'),
							value : '2013-10-21 00:00:00',
							anchor : '100%',
							width : 150,
							listeners : {
								'select' : function(dateField, date) {
									if (date > alarmGrid.endDate.getValue()) {
										Ext.Msg.alert('提示', '开始时间不能大于结束时间!');
									}
								}
							}
						}), '-', '结束时间', new Ext.form.DateField({
							ref : '../endDate',
							format : 'Y-m-d H:i:s',
							showToday : true,
							// value : maxDate.format('Y-m-d H:i:s'),
							value : '2013-10-31 23:59:59',
							anchor : '100%',
							width : 150,
							listeners : {
								'select' : function(dateField, date) {
									if (date < alarmGrid.startDate.getValue()) {
										Ext.Msg.alert('提示', '结束时间不能小于开始时间!');
									}
								}
							}

						}), '-', '模糊查询', {
					ref : '../inkling',
					width : 200,
					xtype : 'textfield'
				}, '-', {
					ref : '../submit',
					iconCls : 'ok',
					xtype : 'button',
					text : '确定',
					handler : function(b, e) {
						loadDatas(startInx, endInx);
					}
				}, '-', '自动刷新', {
					xtype : 'checkbox',
					checked : true,
					listeners : {
						'check' : function(checkbox, checked) {
							if (checked) {
								// alarmGrid.timername.setDisabled(false);
								// alarmGrid.timer.setDisabled(false);
								// alarmGrid.timerunit.setDisabled(false);
								reloadTimer(timer);
							} else {
								// alarmGrid.timername.setDisabled(true);
								// alarmGrid.timer.setDisabled(true);
								// alarmGrid.timerunit.setDisabled(true);
								taskRunner.stopAll();
							}
						}
					}
				}

		// , {
		// xtype : 'displayfield',
		// ref : '../timername',
		// value : '刷新间隔',
		// disabled : false
		// }, {
		// xtype : 'combo',
		// ref : '../timer',
		// disabled : false,
		// width : 50,
		// hiddenName : 'timer',
		// mode : 'local',
		// value : 10,
		// triggerAction : 'all',
		// forceSelection : true,
		// editable : false,
		// displayField : 'interval',
		// valueField : 'interval',
		// store : new Ext.data.ArrayStore({
		// fields : ['interval'],
		// data : [[10], [20], [30], [40], [50], [60]]
		// }),
		// listeners : {
		// 'select' : function(combo) {
		// reloadTimer(combo.getValue(), alarmStore);
		// }
		// }
		// }, {
		// xtype : 'displayfield',
		// ref : '../timerunit',
		// value : '秒',
		// disabled : false
		// }

		],
		bbar : new Ext.PagingToolbar({
			pageSize : 20,
			store : alarmStore,
			displayInfo : true,
			displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
			emptyMsg : "无显示数据",
			doLoad : function(start) {// 改源码
				var o = {}, pn = this.getParams();
				o[pn.start] = start;
				o[pn.limit] = this.pageSize;
				if (this.fireEvent('beforechange', this, o) !== false) {
					// this.store.load({params:o});
					startInx = o[pn.start];
					endInx = o[pn.start] + o[pn.limit];
					loadDatas(startInx, endInx);
				}
			},
			onLoad : function(store, r, o) {//改源码
				if (!this.rendered) {
					this.dsLoaded = [store, r, o];
					return;
				}
				var p = this.getParams();
				// this.cursor = (o.params && o.params[p.start])
				// ? o.params[p.start]
				// : 0;
				this.cursor = startInx;
				var d = this.getPageData(), ap = d.activePage, ps = d.pages;

				this.afterTextItem.setText(String.format(this.afterPageText,
						d.pages));
				this.inputItem.setValue(ap);
				this.first.setDisabled(ap == 1);
				this.prev.setDisabled(ap == 1);
				this.next.setDisabled(ap == ps);
				this.last.setDisabled(ap == ps);
				this.refresh.enable();
				this.updateInfo();
				this.fireEvent('change', this, d);
			},
			items : [new Ext.Button({
						text : '导出excel',
						iconCls : 'excel',
						ref : '../excel',
						tabTip : '请先查询，再导出。',
						menu : {
							items : [{
								text : '导出本页数据',
								handler : function() {
									exportExcel(
											alarmStore.lastOptions.params.start,
											alarmStore.lastOptions.params.limit);
								}
							}, {
								text : '导出所有数据',
								handler : function() {
									exportExcel(0, alarmStore.getTotalCount());
								}
							}]
						}
					})],
			plugins : [new Ext.ux.PagelimitCombo()]
		})
	});

	new Ext.Viewport({
				border : false,
				layout : 'border',
				items : [alarmGrid]
			});

	// 获取定时刷新间隔
	Ext.Ajax.request({
				url : contextPath + 'WoixService.asmx/GetConfigValue',
				method : 'post',
				params : {
					paramName : 'Woix.AlarmRefreshInterval'
				},
				success : function(response, opts) {
					var configValue = Ext.DomQuery.selectNode('string',
							response.responseXML).textContent;
					reloadTimer(configValue);
				},
				failure : function(response, opts) {
					Ext.Msg.alert("提示", "获取刷新时间间隔失败");
				}
			});

	// 定时刷新
	var taskRunner = new Ext.util.TaskRunner();
	function reloadTimer(time) {// time 毫秒单位
		var today = new Date();
		var h = today.getHours();
		var m = today.getMinutes();
		var s = h * 3600 + m * 60 + today.getSeconds();
		s = s * 1000;
		var task = {
			run : function() {
				var ct = new Date();
				var ch = ct.getHours();
				var cm = ct.getMinutes();
				var cs = ch * 3600 + cm * 60 + ct.getSeconds();

				cs = cs * 1000;
				if ((cs - s) % time == 0) {
					// store.reload();
					loadDatas(startInx, endInx);
				}
			},
			interval : 1000
		};
		taskRunner.stopAll();
		taskRunner.start(task);
	}

	loadDatas(startInx, endInx);

	// 加载数据
	function loadDatas(startInx, endInx) {
		var myMask = new Ext.LoadMask('alarmGrid', {
					msg : '正在加载数据，请稍候……'
				});
		myMask.show();

		var start = alarmGrid.startDate.getValue().format('Y-m-d H:i:s');
		var end = alarmGrid.endDate.getValue().format('Y-m-d H:i:s');
		var ink = alarmGrid.inkling.getValue();

		Ext.Ajax.request({
					url : contextPath + 'WoixService.asmx/GetAlarmJson',
					method : 'post',
					params : {
						startInx : startInx,
						endInx : endInx,
						// startTime : minDate.format('Y-m-d H:i:s'),
						// endTime : maxDate.format('Y-m-d H:i:s'),
						start : start,
						end : end,
						searchStr : ink,
						sort : 'NO'
					},
					success : function(response) {
						var result = Ext.DomQuery.selectNode('string',
								response.responseXML).textContent;
						var json = Ext.decode(result);
						if ("true" == json.success) {
							alarmStore.loadData(json);
							myMask.hide();
							var endTime = json.endTime;
							alarmGrid.endDate.setValue(endTime);
							Ext.Msg.alert("提示", "目前为止截止时间为：" + endTime);
						} else {
							Ext.Msg.alert('出错', '加载数据出错，请刷新页面重试。');
							return;
						}

					}
				});
	}

	// 导出
	function exportExcel(start, limit) {
		Ext.MessageBox.show({
					title : '请稍候',
					msg : '正在导出...',
					width : 300,
					wait : true,
					waitConfig : {
						interval : 200
					},
					closable : false
				});

		var result = turnStringForColumn(alarmGrid.getColumnModel(),
				alarmStore.fields);

		Ext.Ajax.request({
					url : '/Woix/ws?method=alarmExport',
					params : {
						startTime : alarmStore.baseParams.startTime,
						endTime : alarmStore.baseParams.endTime,
						searchStr : alarmStore.baseParams.searchStr,
						start : start,
						limit : limit,
						sort : alarmStore.sortInfo.field,
						dir : alarmStore.sortInfo.direction,
						header : result.columns,
						field : result.fields,
						title : '报警一览表'
					},
					method : 'post',
					timeout : 2 * 60 * 1000,
					success : function(response, opts) {
						var result = Ext.decode(response.responseText);
						if (result.success) {
							var path = result.attributes.downloadpath;
							location.href = '/Woix/ws?' + path;
							Ext.Msg.hide();
						} else {
							Ext.Msg.alert("失败", "导出失败,请重新导出");
						}
					},
					failure : function() {
						Ext.Msg.alert("提示", "导出失败，请重试");
					}
				});
	}
});