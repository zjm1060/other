Ext.onReady(function() {

			/**
			 * 首页实时报警显示区
			 */

			var countNum = 2;// 报警条数
			var quickAlarmRecord = new Ext.data.Record.create([{
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

			var quickAlarmStore = new Ext.data.Store({
						reader : new Ext.data.JsonReader({}, quickAlarmRecord)
					});

			var quickAlarmColumn = new Ext.grid.ColumnModel([{
						header : 'NO',
						dataIndex : 'NO',
						sortable : false,
						width : 60
					}, {
						header : 'DATETIME',
						dataIndex : 'DATETIME',
						sortable : false,
						width : 180
					}, {
						header : 'POINTNAME',
						dataIndex : 'POINTNAME',
						sortable : false,
						width : 180
					}, {
						header : 'LONGNAME',
						dataIndex : 'LONGNAME',
						sortable : false,
						width : 300
					}, {
						header : 'STATUS',
						dataIndex : 'STATUS',
						sortable : false,
						width : 100
					}, {
						header : 'HOST',
						dataIndex : 'HOST',
						sortable : false,
						width : 180
					}]);

			var quickAlarmGrid = new Ext.grid.GridPanel({
						id : 'quickAlarmGrid',
						region : 'center',
						hideHeaders : true,
						store : quickAlarmStore,
						cm : quickAlarmColumn,
						stripeRows : true,
						layout : 'fit',
						height : 55,
						border : false,
						autoScroll : true,
						loadMask : true,
						loadMask : {
							msg : '正在加载数据，请稍候……'
						},
						listeners : {
							'rowdblclick' : function(grid, rowIndex, e) {
								// 实时报警区点击放大到整个区域
								Woix.turn(9, '报警一览表', '/Woix/alarm.html', '',
										true);
								Ext.getCmp('quickPanel').collapse(true);
							}
						}
					});

			// 获取定时刷新时间间隔
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
							loadDatas();
						}
					},
					interval : 1000
				}
				taskRunner.stopAll();
				taskRunner.start(task);
			}

			this.getQuickAlarmGrid = function() {
				return quickAlarmGrid;
			}

			loadDatas();
			// 加载数据
			function loadDatas() {
				var myMask = new Ext.LoadMask('quickAlarmGrid', {
							msg : '正在加载数据，请稍候……'
						});
				myMask.show();
				Ext.Ajax.request({
							url : contextPath
									+ 'WoixService.asmx/GetQuickAlarmJson',
							method : 'post',
							params : {
								count : countNum
							},
							success : function(response) {
								var result = Ext.DomQuery.selectNode('string',
										response.responseXML).textContent;

								if (Ext.isEmpty(result)) {
									Ext.Msg.alert('出错', '加载数据出错，请刷新页面重试。');
									return;
								}
								var json = Ext.decode(result);
								quickAlarmStore.loadData(json.rows);
								myMask.hide();
							}
						});

			}
		});