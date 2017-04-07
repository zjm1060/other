Ext.ns('com.bitc.xmis');
com.bitc.xmis.ApplyMeetingFormTipsGrid = Ext.extend(Ext.grid.GridPanel, {
	autoWidth : true,
	initComponent : function() {
		this.store = new Ext.data.Store({
			url : __cxtPath
					+ '/applyManageMaint.do?method=getApplyMeetingFormTipsList',
			baseParams : {
				start : 0,
				limit : 0
			},
			autoLoad : true,
			reader : new Ext.data.JsonReader({
						root : 'rows',
						fields : [{
									name : 'type',
									mapping : 'type'
								}, {
									name : 'qty',
									mapping : 'qty'
								}, {
									name : 'name',
									mapping : 'name'
								}]
					}),
			listeners : {
				scope : this,
				'load' : function(s, records, options) {
					if (this.getStore().data.length) {
						this.setHeight(260);
					}
				}
			}
		});
		this.columns = [new Ext.grid.RowNumberer(), {
					header : '',
					width : 350,
					dataIndex : 'name',
					renderer : function(value, cellmeta, record) {
						if (record.get('qty') != 0) {
							return '<font  color="red">' + value + '</font>';
						}else{
							return value;
						}
					}
				}, {
					header : '',
					width : 50,
					dataIndex : 'qty',
					renderer : function(value, cellmeta, record) {
						if (record.get('qty') != 0) {
							return '<font  color="red">' + value + '</font>';
						}else{
							return value;
						}
					}
				}];
		com.bitc.xmis.ApplyMeetingFormTipsGrid.superclass.initComponent
				.call(this);
	},
	onRender : function(a, b) {
		this.on('rowclick', function(gridpanel, index, e) {
					var r = this.getStore().getAt(index);
					if (r.get('type') == 1) {
						xMis.turn('61', '会议室申请',
								'meetingRoom/applyMeetingRoomList.jsp');
					} else {
						xMis
								.turn('65', '公务车申请',
										'applymanage/applyCarList.jsp');

					}
				});
		com.bitc.xmis.ApplyMeetingFormTipsGrid.superclass.onRender.call(this,
				a, b);
	},
	reload : function() {
		this.getStore().reload();
	}

});

com.bitc.xmis.CurrentAndNextAbsentInfoGroupGrid = Ext.extend(
		Ext.grid.GridPanel, {
			autoWidth : true,
			initComponent : function() {
				this.store = new Ext.data.GroupingStore({
					url : __cxtPath
							+ '/applyManageMaint.do?method=getCurrentAndNextAbsentInfo',

					reader : new Ext.data.JsonReader({
								root : 'rows',
								totalProperty : 'totalCount',
								fields : [{
											"name" : "form_id",
											"type" : "string"
										}, {
											"name" : "reason",
											"type" : "string"
										}, {
											"name" : "absent_type_name",
											"type" : "string"
										}, {
											"name" : "app_user_id",
											"type" : "int"
										}, {
											"name" : "app_user_name",
											"type" : "string"
										}, {
											"name" : "type",
											"type" : "string"
										}, {
											"name" : "plan_begin_date",
											"type" : "string"
										}, {
											"name" : "plan_end_date",
											"type" : "string"

										}]
							}),
					groupField : 'type',
					baseParams : {
						start : 0,
						limit : 0
					},
					autoLoad : true,
					remoteSort : true,
					listeners : {
						scope : this,
						'load' : function(s, records, options) {
							if (this.getStore().data.length) {
								this.setHeight(200);
							}
						}
					}
				});
				var TYPE_ARRAY = ['', '当天请假人员情况', '未来请假人员情况'];
				this.columns = [new Ext.grid.RowNumberer(), {
							header : "姓名",
							width : 90,
							dataIndex : "app_user_name"
						}, {
							header : "原因",
							hidden : true,
							width : 90,
							dataIndex : "reason"
						}, {
							header : "请假类型",
							width : 90,
							dataIndex : "absent_type_name"
						}, {
							header : "预计开始时间",
							width : 180,
							dataIndex : "plan_begin_date"
						}, {
							header : "预计结束时间",
							width : 180,
							dataIndex : "plan_end_date"
						}, {
							dataIndex : 'type',
							hidden : true,
							header : "",
							renderer : function(value) {
								return TYPE_ARRAY[value];
							}
						}];
				this.view = new Ext.grid.GroupingView({
					enableGroupingMenu : false,
					startCollapsed : true,
					// forceFit : true,
					groupTextTpl : '{text} ({[values.rs.length]} {[values.rs.length >= 1 ? "条" : "条"]})'// ?
				});
				com.bitc.xmis.CurrentAndNextAbsentInfoGroupGrid.superclass.initComponent
						.call(this);
			},
			onRender : function(a, b) {
				this.on('rowclick', function(gridpanel, index, e) {
					var r = gridpanel.getStore().getAt(index);
						// xMis.turn('contract-' + r.data.ID, '【合同】' +
						// r.data.cname,
						// 'contractMain.do?method=contract&contractID='
						// + r.data.ID);
					});
				com.bitc.xmis.CurrentAndNextAbsentInfoGroupGrid.superclass.onRender
						.call(this, a, b);
			},
			reload : function() {
				this.getStore().reload();
			}

		});
