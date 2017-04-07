var grid;
Ext.onReady(function() {
	function init(companies) {
		window.companies = companies;
		var currentEdit;
		var companyGroupRow = [{
			header : '',
			colspan : 8,
			align : 'center'
		}];
		var numberfield = new Ext.form.NumberField({
			minValue : 0
		});
		var textfield = new Ext.form.TextField();
		var columns = [new Ext.grid.RowNumberer(), {
			header : '',
			width : 40,
			dataIndex : 'id',
			renderer : function(value) {
				return '<a href="javascript:;" onclick="bid.showDetail(\''
					+ value + '\')">查看</a>'
			}
		}, {
			header : '名称',
			width : 200,
			dataIndex : 'name',
			editor : textfield
		}, {
			header : '业主单位',
			width : 200,
			dataIndex : 'bidCompany.companyName',
			editor : new Ext.ux.form.ChooseField({
				width : 348,
				winConfig : {
					title : '选择业主单位',
					modal : true
				},
				baseParamFun : function() {
					winFlag = "aWin";
					return {
						start : 0,
						limit : 20
					};
				},
				allowBlank : false,
				chooseUrl : __cxtPath + '/resources/choose/company.json.jsp',
				setValueFun : function(o) {
					currentEdit.record.set('bidCompany.companyID', o
							.get('公司ID'));
					currentEdit.record.set('bidCompany.companyName', o
							.get('公司名称'));
					updateAssociation(currentEdit.record, 'bidCompany', o
							.get('公司ID'), 'integer');
				},
				clearValueFun : function() {
				}
			})
		}, {
			header : '合同价',
			dataIndex : 'contractPrice',
			editor : numberfield,
			valueType : 'bigdecimal',
			align : 'right'
		}, {
			header : '均价',
			dataIndex : 'analysisPrices',
			align : 'right',
			renderer : function(value) {
				return value == undefined || value[2] == null
					? ''
					: Ext.util.Format.number(value[2], '0,000');
			}
		}, {
			header : '最高价',
			dataIndex : 'analysisPrices',
			align : 'right',
			renderer : function(value) {
				return value == undefined || value[1] == null
					? ''
					: Ext.util.Format.number(value[1], '0,000');
			}
		}, {
			header : '最低价',
			dataIndex : 'analysisPrices',
			align : 'right',
			renderer : function(value) {
				return value == undefined || value[0] == null
					? ''
					: Ext.util.Format.number(value[0], '0,000');
			}
		}];
		var fields = ['id', 'name', 'bidCompany', 'contractPrice',
		'solutionWriter', 'businessManager', 'region', 'solution',
		'winCompany', {
			name : 'bidDate',
			type : 'date',
			dateFormat : 'Y-m-d'
		}, 'analysisPrices'];
		Ext.iterate(companies, function(company) {
			companyGroupRow.push({
				header : company.name,
				colspan : 2,
				align : 'center'
			});
			fields.push("company_" + company.id + "_bid", "company_"
					+ company.id + "_preferential");
			columns.push({
				id : 'column_' + company.id + "_bid",
				header : '投标价',
				dataIndex : "company_" + company.id + "_bid",
				editor : numberfield,
				valueType : 'bigdecimal',
				align : 'right',
				xtype : 'numbercolumn',
				format : '0,000'
			}, {
				id : 'column_' + company.id + "_preferential",
				header : '优惠价',
				dataIndex : "company_" + company.id + "_preferential",
				editor : numberfield,
				valueType : 'bigdecimal',
				align : 'right',
				xtype : 'numbercolumn',
				format : '0,000'
			});
		});
		columns.push({
			header : '方案编写',
			dataIndex : 'solutionWriter.userName',
			editor : new Ext.ux.form.ChooseOrgTreeField({
				width : 136,
				winConfig : {
					title : '选择方案编写人员'
				},
				singleSelect : true,
				onlyPerson : true,
				rootConfig : function() {
					return {
						id : 'comp-old-1'
					};
				},
				dataUrl : '/orgMaint.do?method=getOrganizationTree',
				setValueFun : function(result) {
					if (result[0]) {
						var empId = result[0].id.split('-')[2];
						currentEdit.record.set('solutionWriter.userID', empId);
						currentEdit.record.set('solutionWriter.userName',
							result[0].text);
						updateAssociation(currentEdit.record, 'solutionWriter',
							empId, 'integer');
					}
				}
			}),
			valueType : 'association'
		}, {
			header : '商务经理',
			dataIndex : 'businessManager.userName',
			editor : new Ext.ux.form.ChooseOrgTreeField({
				width : 136,
				winConfig : {
					title : '选择商务经理'
				},
				singleSelect : true,
				onlyPerson : true,
				rootConfig : function() {
					return {
						id : 'comp-old-1'
					};
				},
				dataUrl : '/orgMaint.do?method=getOrganizationTree',
				setValueFun : function(result) {
					if (result[0]) {
						var empId = result[0].id.split('-')[2];
						currentEdit.record.set('businessManager.userID', empId);
						currentEdit.record.set('businessManager.userName',
							result[0].text);
						updateAssociation(currentEdit.record,
							'businessManager', empId, 'integer');
					}
				}
			}),
			valueType : 'association'
		}, {
			header : '区域',
			dataIndex : 'region',
			editor : textfield
		}, {
			header : '方案',
			dataIndex : 'solution',
			editor : textfield
		}, {
			xtype : 'datecolumn',
			header : '投标日期',
			format : 'Y-m-d',
			dataIndex : 'bidDate',
			editor : new Ext.form.DateField({
				format : 'Y-m-d'
			}),
			valueType : 'datetime'
		});
		companyGroupRow.push({
			header : ' ',
			colspan : 5,
			align : 'center'
		});
		var group = new Ext.ux.grid.ColumnHeaderGroup({
			rows : [companyGroupRow]
		});
		var store = new Ext.data.JsonStore({
			autoLoad : true,
			url : __cxtPath + '/bidding.do?method=getBiddingDisplay',
			fields : fields,
			root : 'rows',
			totalProperty : 'totalCount',
			idProperty : 'id'
		});
		var bidWin = null;
		grid = new Ext.grid.EditorGridPanel({
			id : 'biddingList',
			sm : new Ext.grid.RowSelectionModel(),
			region : 'center',
			title : '投标价格分析表',
			clicksToEdit : 2,
			store : store,
			stripeRows : true,
			loadMask : {
				msg : '正在加载数据，请稍侯……'
			},
			columns : columns,
			plugins : group,
			//view: new Ext.ux.grid.LockingGridView(),
			tbar : [{
				text : '添加',
				iconCls : 'add',
				handler : function() {
					Ext.Ajax.request({
						url : __cxtPath + '/bidding.do?method=addBidding',
						success : function(response) {
							var result = Ext.decode(response.responseText);
							if (result.success) {
								var Record = grid.getStore().recordType;
								var r = new Record({
									id : result.attributes.id
								}, result.attributes.id);
								grid.stopEditing();
								grid.getStore().insert(0, r);
								grid.startEditing(0, 1);
							}
						}
					});

				}
			}, {
				text : '删除',
				iconCls : 'remove',
				handler : function() {
					if (grid.getSelectionModel().getSelected() == null) {
						Ext.Msg.alert('提示', '请先选择要删除的行。');
						return;
					}
					var record = grid.getSelectionModel().getSelected();
					Ext.Msg.confirm('提示', '确定要删除方案【' + record.get('name')
						+ '】吗？', function(btn) {
						if (btn != 'yes') {
							return;
						}
						deleteBidding(record);
					});

				}
			}, '-', {
				text : '投标方管理',
				iconCls : 'announcement',
				handler : function() {
					if (bidWin == null) {
						bidWin = new com.bitc.xmis.bid.BidCompanyWindow({
							listeners : {
								selectcompany : function(selmodel, rowindex,
									company) {
									var cm = grid.getColumnModel();
									var index = cm.getIndexById('column_'
										+ company.get('id') + '_bid');
									cm.setHidden(index, false);
									index = cm.getIndexById('column_'
										+ company.get('id') + '_preferential');
									cm.setHidden(index, false);
								},
								deselectcompany : function(selmodel, rowindex,
									company) {
									var cm = grid.getColumnModel();
									var index = cm.getIndexById('column_'
										+ company.get('id') + '_bid');
									cm.setHidden(index, true);
									index = cm.getIndexById('column_'
										+ company.get('id') + '_preferential');
									cm.setHidden(index, true);
								}
							}
						});
					}
					bidWin.show();
				}
			}, '-', '模糊查询：', new Ext.ux.InklingSearchField({
				width : 200,
				store : store,
				tooltip : {
					title : '模糊查询',
					text : '您可以输入名称、业主单位、投票日期、方案编写人、商务经理等关键字进行模糊查询。<br/>按日期请按如下格式输入：<br/>2010-01-01'
				},
				inklings : [{
					name : 'name',
					op : 'like',
					type : 'string'
				}, {
					name : 'bidCompany.companyName',
					op : 'like',
					type : 'string'
				}, {
					name : 'bidDate',
					op : '=',
					type : 'date'
				}, {
					name : 'region',
					op : 'like',
					type : 'string'
				}, {
					name : 'solutionWriter.userName',
					op : 'like',
					type : 'string'
				}, {
					name : 'businessManager.userName',
					op : 'like',
					type : 'string'
				}, {
					name : 'solution',
					op : 'like',
					type : 'string'
				}]
			})],
			listeners : {
				beforeedit : function(e) {
					// The edit event object has the following properties
					// grid - This grid
					// record - The record being edited
					// field - The field name being edited
					// value - The value for the field being edited.
					// row - The grid row index
					// column - The grid column index
					// cancel - Set this to true to cancel the edit or return
					// false from your handler.
					currentEdit = e;
				},
				'afteredit' : function(e) {
					var column = e.grid.getColumnModel().config[e.column];
					updateRecord(e.record, e.field, e.value, column.valueType
							|| 'string');
				}
			}
		});
		new Ext.Viewport({
			layout : 'border',
			items : [grid]
		});
	}
	Ext.Ajax.request({
		url : __cxtPath + '/bidding.do?method=getBidCompanies',
		success : function(response) {
			var result = Ext.decode(response.responseText);
			if (result.success) {
				init(result.rows);
			}
		}
	});
	function updateRecord(record, field, value, valueType) {
		if (valueType == 'association') {
			return;
		}
		var i = field.indexOf('company_');
		var clazz, company, url;
		if (i == 0) {
			clazz = 'com.bitc.jk.xmis.model.bid.BidPrice';
			var tmps = field.split('_');
			company = tmps[1];
			var type = tmps[2];
			if (type == 'bid') {
				field = 'bidPrice';
			} else if (type = 'preferential') {
				field = 'preferentialPrice';
			}
			url = __cxtPath + '/bidding.do?method=updateBidPrice';
		} else {
			clazz = 'com.bitc.jk.xmis.model.bid.Bidding';
			url = __cxtPath + '/bidding.do?method=updateObject';
		}
		Ext.Ajax.request({
			url : url,
			params : {
				clazz : clazz,
				id : record.get('id'),
				field : field,
				value : value,
				company : company,
				valueType : valueType
			},
			success : function(response) {
				var result = Ext.decode(response.responseText);
				if (result.success) {
					record.commit();
				}
			}
		});
	}
	function updateAssociation(record, field, value, valueType) {
		Ext.Ajax.request({
			url : __cxtPath + '/bidding.do?method=updateAssociation',
			params : {
				clazz : 'com.bitc.jk.xmis.model.bid.Bidding',
				id : record.get('id'),
				field : field,
				value : value,
				valueType : valueType
			},
			success : function(response) {
				var result = Ext.decode(response.responseText);
				if (result.success) {
					record.commit();
				}
			}
		});
	}
	function deleteBidding(record) {
		Ext.Ajax.request({
			url : __cxtPath + '/bidding.do?method=deleteBidding',
			params : {
				id : record.get('id')
			},
			success : function(response) {
				var result = Ext.decode(response.responseText);
				if (result.success) {
					record.store.reload();
				}
			}
		});
	}
});
Ext.ns('bid');
bid.showDetail = function(id) {
	var store = grid.getStore();
	var record = store.getById(id);
	var fm = Ext.util.Format;
	var html = [
		'<div class="infodiv"><table class="table"><tr><td class="name">名称：</td><td class="value">'
			+ Ext.ifNull(record.get('name'), '') + '</td></tr>',
		'<tr><td class="name">业主单位：</td><td class="value">'
			+ Ext.ifNull(record.get('bidCompany.companyName'), '')
			+ '</td></tr>',
		'<tr><td class="name">合同价：</td><td class="value">'
			+ fm.number(record.get('contractPrice'), '0,000') + '</td></tr>',
		'<tr><td class="name">均价：</td><td class="value">'
			+ fm.number(record.get('analysisPrices')[2], '0,000')
			+ '</td></tr>',
		'<tr><td class="name">最高价：</td><td class="value">'
			+ fm.number(record.get('analysisPrices')[1], '0,000')
			+ '</td></tr>',
		'<tr><td class="name">最低价：</td><td class="value">'
			+ fm.number(record.get('analysisPrices')[0], '0,000')
			+ '</td></tr>',
		'<tr><td class="name">方案编写：</td><td class="value">'
			+ Ext.ifNull(record.get('solutionWriter.userName'), '')
			+ '</td></tr>',
		'<tr><td class="name">商务经理：</td><td class="value">'
			+ Ext.ifNull(record.get('businessManager.userName'), '')
			+ '</td></tr>',
		'<tr><td class="name">区域：</td><td class="value">'
			+ Ext.ifNull(record.get('region'), '') + '</td></tr>',
		'<tr><td class="name">方案：</td><td class="value">'
			+ Ext.ifNull(record.get('solution'), '') + '</td></tr>',
		'<tr><td class="name">投标日期：</td><td class="value">'
			+ fm.date(Ext.ifNull(record.get('bidDate'), ''), 'Y-m-d')
			+ '</td></tr>',
		'</table></div>',
		'<div class="detaildiv"><table class="table" width="100%"><thead><tr><th class="name">投标公司</th>',
		'<th class="name">投标价</th><th class="name">优惠价</th></tr></thead>',
		'<tbody>'];
	var currentCompanies=[];
	var series = [ {
		name : '投标价',
		type : 'bar',
		data : [],
		markPoint : {
			data : [ {
				type : 'max',
				name : '最大值'
			}, {
				type : 'min',
				name : '最小值'
			} ]
		},
		markLine : {
			data : [ {
				type : 'average',
				name : '平均值'
			} ]
		}
	} ];
	Ext.iterate(companies, function(company) {
		if (record.get("company_" + company.id + "_bid") != null
			|| record.get("company_" + company.id + "_preferential") != null) {
			html.push('<tr><td class="value">'
				+ company.name
				+ '</td><td  align="right"  class="value">'
				+ fm.number(record.get("company_" + company.id + "_bid"),
					'0,000')
				+ '</td><td align="right"  class="value">'
				+ fm.number(record.get("company_" + company.id
						+ "_preferential"), '0,000') + '</td></tr>');
			currentCompanies.push(company.name);
			series[0].data.push(record.get("company_" + company.id
					+ "_preferential")==null?record.get("company_" + company.id + "_bid"):record.get("company_" + company.id
							+ "_preferential"));
		}
	});
	html.push('</tbody></div>');
	html.push('<div id="chart_'+id+'" style="height:400px"></div>');
	var win = new Ext.Window({
		closeAction : 'close',
		title : record.get('name'),
		width : 1000,
		height : 400,
		autoScroll : true,
		html : html.join('')
	});
	win.show(null,function(){		
		require(
	            [
	                'echarts',
	                'echarts/chart/bar' // 使用柱状图就加载bar模块，按需加载
	            ],
	            function (ec) {
	                // 基于准备好的dom，初始化echarts图表
	                var myChart = ec.init(document.getElementById('chart_'+id)); 
	                
	                var option = {
	                    tooltip: {
	                        show: true
	                    },
	                    legend: {
	                        data:['投标价']
	                    },
	                    xAxis : [
	                        {
	                            type : 'category',
	                            data : currentCompanies
	                        }
	                    ],
	                    yAxis : [
	                        {
	                            type : 'value'
	                        }
	                    ],
	                    series :series
	                };
	        
	                // 为echarts对象加载数据 
	                myChart.setOption(option); 
	            }
	        );
	});
};
