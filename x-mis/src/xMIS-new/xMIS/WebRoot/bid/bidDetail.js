Ext.onReady(function() {
	var bidID = request.getParameter('bidID');
	var cmDetail = new Ext.grid.ColumnModel([ new Ext.grid.RowNumberer({
		header : '序号',
		width : 50
	}), {
		header : '报价公司',
		dataIndex : '报价公司',
		hidden : true
	}, {
		header : '投标单位',
		dataIndex : '投标单位',
		sortable : false
	}, {
		header : '投标价',
		dataIndex : '投标价',
		sortable : false,
		align:'right'
	}, {
		header : '优惠价',
		dataIndex : '优惠价',
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
	} , {
		name : 'min_price'
	}, {
		name : 'max_price'
	}, {
		name : 'ave_price'
	} ]);
	var reRecordDetail = Ext.data.Record.create([ {
		name : '报价公司'
	}, {
		name : '投标单位'
	}, {
		name : '投标价'
	}, {
		name : '优惠价'
	}]);
	var store = new Ext.data.Store({
		id:'store',
		proxy : new Ext.data.HttpProxy({
			url : __cxtPath + '/bidding.do?method=getBidAnalysisList',
			method : "POST"
		}),
		reader : new Ext.data.JsonReader({
			root : 'rows',
			totalProperty : 'totalCount'
		}, reRecord),
		baseParams : {
			bidID : bidID
		},
		remoteSort : true,
		autoLoad : true,
		listeners:{
			'load' : function() {
				Ext.getCmp('name').setValue(store.getAt(0).get('名称'));
				Ext.getCmp('company_name').setValue(store.getAt(0).get('公司名称'));
				Ext.getCmp('area').setValue(store.getAt(0).get('区域'));
				Ext.getCmp('date').setValue(store.getAt(0).get('投标日期'));
				Ext.getCmp('business_manager').setValue(store.getAt(0).get('商务经理'));
				Ext.getCmp('design_plan').setValue(store.getAt(0).get('方案编写'));
				Ext.getCmp('plan').setValue(store.getAt(0).get('方案'));
				Ext.getCmp('successful_bidder').setValue(store.getAt(0).get('中标单位'));
				Ext.getCmp('contract_price').setValue(store.getAt(0).get('合同价'));
				Ext.getCmp('min_price').setValue(store.getAt(0).get('min_price'));
				Ext.getCmp('ave_price').setValue(store.getAt(0).get('ave_price'));
				Ext.getCmp('max_price').setValue(store.getAt(0).get('max_price'));
			}
		}
	});
	var storeDetail = new Ext.data.Store({
		id : 'storeDetail',
		proxy : new Ext.data.HttpProxy({
			url : __cxtPath + '/bidding.do?method=getBidAnalysisDetail',
			method : "POST"
		}),
		reader : new Ext.data.JsonReader({
			root : 'rows',
			totalProperty : 'totalCount'
		}, reRecordDetail),
		baseParams : {
			bidID : bidID
		},
		autoLoad:true,
		remoteSort : true,
		listeners:{
			'load' : function() {
				
			}
		}
	});
	var grid = new Ext.grid.GridPanel({
		region : 'south',
		id : 'grid',
		border : false,
		width : 980,
		height:300,
		loadMask : true,
		collapsible : true,
		autoScroll : true,
		collapseMode : 'mini',
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		stripeRows : true,
		store : storeDetail,
		cm : cmDetail
	});
	var panel =  new Ext.Panel({
		region : 'center',
		layout : 'column',
		padding:'20px 0 0 0',
		items : [{
			xtype : 'panel',
			layout : 'form',
			border : false,
			labelAlign : 'right',
			items : [{
						xtype : 'displayfield',
						id : 'name',
						fieldLabel : '名称'
					},{
						xtype : 'displayfield',
						id : 'company_name',
						fieldLabel : '公司名称'
					},{
						xtype : 'displayfield',
						id : 'area',
						fieldLabel : '区域'
					},{
						xtype : 'displayfield',
						id : 'date',
						fieldLabel : '投标日期'
					},{
						xtype : 'displayfield',
						id : 'business_manager',
						fieldLabel : '商务经理'
					},{
						xtype : 'displayfield',
						id : 'design_plan',
						fieldLabel : '方案编写'
					},{
						xtype:'panel',
						layout : 'hbox',
						padding:'0 0 0 10px',
						border : false,
						width:400,
						fieldLabel : '方案',
						labelAlign : 'right',
						items:[{
							width:250,
							xtype : 'textarea',
							id : 'plan',
							maxLength: 250,
							height:200
						}, {
							xtype : 'displayfield',
							width : 10,
							value : ''
						},{
							xtype : 'button',
							id : 'planBtn',
							padding:'0px 0 0 10px',
							text : '编辑',
							width:30
						}]
					},{
						xtype : 'displayfield',
						id : 'successful_bidder',
						fieldLabel : '中标单位'
					},{
						xtype : 'displayfield',
						id : 'contract_price',
						fieldLabel : '合同价'
					},{
						xtype : 'displayfield',
						id : 'min_price',
						fieldLabel : '最低价'
					},{
						xtype : 'displayfield',
						id : 'ave_price',
						fieldLabel : '平均价'
					},{
						xtype : 'displayfield',
						id : 'max_price',
						fieldLabel : '最高价'
					}]

		}]
	});
	Ext.getCmp('planBtn').on('click',function(){
		var win = new Ext.Window({
			title : '方案的修改',
			layout : 'border',
			width : 300,
			height : 300,
			modal : true,
			border : false,
			items : [new Ext.form.FormPanel({
				region : 'center',
				ref : 'p',
				frame : true,
				border : false,
				layout : 'border',
				bodyStyle : 'padding:0px',
				buttonAlign : 'center',
				fbar : [{
					text : '确定',
					id : 'sign_no_btn',
					handler : function() {
						Ext.Ajax.request({
							url : __cxtPath + '/bidding.do?method=doSaveBidPlan',
							method : 'post',
							params : {
								bidID : bidID,
								content:Ext.getCmp('plan_update').getValue()
							},
							success : function(response, action) {
								Ext.getCmp('plan').setValue(Ext.getCmp('plan_update').getValue());
								win.hide();
							},
							failure : function(response, action) {
								var result = Ext
										.decode(response.responseText);
								Ext.MessageBox.hide();
								Ext.Msg.alert(result.messageTitle,
										result.message);
							}
						});

					}
				}, {
					text : '取消',
					handler : function() {
						win.hide();
					}
				}],
				items : [{region : 'center',
							xtype : 'textarea',
							id : 'plan_update',
							maxLength: 250,
							width : 270,
							height:250
						}]
			})]
		}).show();
		Ext.getCmp('plan_update').setValue(store.getAt(0).get('方案'));
	});

	new Ext.Viewport({
		layout : 'border',
		items : [ panel,grid ]
	});
});