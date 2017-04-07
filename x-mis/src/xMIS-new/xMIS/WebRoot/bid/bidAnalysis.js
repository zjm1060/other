Ext.onReady(function() {
	Ext.QuickTips.init();
	var cm = new Ext.grid.ColumnModel([ new Ext.grid.RowNumberer({
		header : '序号',
		width : 50
	}), {
		header : 'ID',
		dataIndex : 'ID',
		hidden : true
	}, {
		header : '名称',
		width:300,
		dataIndex : '名称',
		sortable : false
	}, {
		header : '公司名称',
		width:200,
		dataIndex : '公司名称',
		sortable : false
	}, {
		header : '区域',
		dataIndex : '区域',
		sortable : false
	}, {
		header : '投标日期',
		dataIndex : '投标日期'
	}, {
		header : '商务经理',
		dataIndex : '商务经理',
		sortable : false
	}, {
		header : '方案编写',
		dataIndex : '方案编写',
		sortable : false
	}, {
		header : '方案',
		dataIndex : '方案',
		sortable : false
	}, {
		header : '中标单位',
		dataIndex : '中标单位',
		sortable : false
	}, {
		header : '合同价',
		dataIndex : '合同价',
		sortable : false,
		align:'right'
	} ]);

	var reRecord = Ext.data.Record.create([ {
		name : 'ID'
	}, {
		name : '名称'
	}, {
		name : '公司名称'
	}, {
		name : '区域'
	}, {
		name : '投标日期'
	}, {
		name : '商务经理'
	}, {
		name : '方案编写'
	}, {
		name : '方案'
	}, {
		name : '中标单位'
	}, {
		name : '合同价'
	} ]);
	var store = new Ext.data.Store({
		proxy : new Ext.data.HttpProxy({
			url : __cxtPath + '/bidding.do?method=getBidAnalysisList',
			method : "POST"
		}),
		reader : new Ext.data.JsonReader({
			root : 'rows',
			totalProperty : 'totalCount'
		}, reRecord),
		baseParams : {
			start : 0,
			limit : 20
		},
		remoteSort : true,
		autoLoad : true
	});
	var grid = new Ext.grid.GridPanel({
		region : 'center',
		id : 'grid',
		border : false,
		width : 980,
		loadMask : true,
	//	collapsible : true,
		autoScroll : true,
		collapseMode : 'mini',
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		stripeRows : true,
		store : store,
		cm : cm,
		tbar : {
			xtype : 'toolbar',
			items : ['查询：', new Ext.ux.form.SearchField({
				width : 180,
				emptyText : '请输入条件',
				onRender : function(ct, position) {
					Ext.ux.InklingSearchField.superclass.onRender
							.call(this, ct, position);
					if (this.tooltip && this.tooltip.text)
						new Ext.ToolTip({
									target : this.id,
									trackMouse : false,
									draggable : false,
									maxWidth : 200,
									minWidth : 100,
									title : this.tooltip.title,
									html : this.tooltip.text
								});
				},
				tooltip : {
					title : '条件查询',
					text : '您可以输入项目名称、业主单位、所属地区、投标单位, 商务经理、 方案编写人'
				},
				onTrigger1Click : function() {

				},
				onTrigger2Click : function() {
					var inputName = this.el.dom.value;
					if (inputName != null
							&& '' != inputName.trim()) {
						//store.setBaseParam('query',
						//		inputName);
						store.reload({
									params : {
										start : 0,
										query:inputName
									}
								});
					} else {
						Ext.MessageBox.alert("提示",
						"请输入要查询的内容后进行查询");
			}
				}
			}),'-', {
				xtype : 'button',
				text : '查询所有',
				ref : '../refreshButton',
				handler : function() {
					store.reload({
							params :{
								start : 0,
								query:''
							}
				});
				}
			}]
		},
		bbar:new Ext.PagingToolbar({
			pageSize : 20,
			store : store,
			displayInfo : true,
			displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
			emptyMsg : "无显示数据",
			plugins : [new Ext.ux.PagelimitCombo()]}),
		listeners : {
			'rowclick' : function(grid, rowIndex, e) {
				var url = __cxtPath + '/bid/bidDetail.jsp?bidID='
						+ grid.getStore().getAt(rowIndex).get("ID");
				parent.xMis.turn('bid-detail-info'
						+ grid.getStore().getAt(rowIndex).get("ID"), '【投标】'+grid.getStore().getAt(rowIndex).get("名称"),
						url, null);
			}
		}
	});
	new Ext.Viewport({
		layout : 'border',
		items : [ grid ]
	});
});