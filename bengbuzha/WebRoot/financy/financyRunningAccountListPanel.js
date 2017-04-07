function turnToEditContract(contractID, contractName, contractType) {
			parent.xMis.turn('contract-' + contractID, contractName,
					'contractMain.do?method=contract&contractID=' + contractID, 'menu-contract-edit');
		}
function linkRenderer(value, contractID, contractName) {
	if (Ext.isEmpty(value)) {
		value = '--';
	}
	var tmp = "<a href='javascript:;' onclick=\"javascript:turnToEditContract('{0}','{1}',{2});\">{3}</a>";
	return String.format(tmp, contractID, contractName,'undefined', value);
}
Ext.ns('Ext.ux.grid');
		Ext.ux.grid.RowNumberer = Ext.extend(Object, {
			id : 'cnumberer',
			width : 20,
			sortable : false,
			hideable : false,
			menuDisabled : true,
			dataIndex : '',
			renderer : function(value, cellmeta, record, rowIndex, columnIndex,
					store) {
				if (store.getCount() - 1 < store.baseParams.limit
						&& store.getCount() == rowIndex + 1) {
					return '<font color=red style="font-size=11px">&nbsp;<b>合计</b>&nbsp;</font>';
				} else {
					if (rowIndex != store.baseParams.limit) {
						return store.lastOptions.params.start + rowIndex + 1;
					} else {
						return '<font color=red style="font-size=11px">&nbsp;<b>合计</b>&nbsp;</font>';
					}
				}
			}
		});
Ext.ns('com.bitc.xmis.Records');
com.bitc.xmis.Records.FinancyRunningAccountRecord = Ext.data.Record.create([
		{
			name : 'year'
		}, {
			name : 'iperiod'
		}, {
			name : 'ino_id'
		},{
			name : 'MB',
			type : 'number'
		}, {
			name : 'cdigest'
		}, {
			name : 'dbill_date'
		}, {
			name : 'project_code'
		}, {
			name : 'flag',
			type : 'bool'
		}, {
			name : 'projectID'
		}, {
			name : 'contractID'
		}, {
			name : 'contractName'
		} ]);

Ext.ns('com.bitc.xmis');
com.bitc.xmis.FinancyRunningAccountGrid = Ext.extend(Ext.grid.GridPanel, {
	searchType : 1,
	initComponent : function() {
		var grid = this;
		this.store = new Ext.data.Store({
					url : __cxtPath + '/financyMain.do?method=getFinancyRunningAccountList',
					baseParams : {
						limit : 20,
						searchType : this.searchType
					},
					remoteSort : true,
					sortInfo : {
						field : 'dbill_date',
						direction : 'ASC'
					},
					reader : new Ext.data.JsonReader( {
						root : 'rows',
						totalProperty : 'totalCount',
						fields : com.bitc.xmis.Records.FinancyRunningAccountRecord
					})
				});
		this.columns = [new Ext.ux.grid.RowNumberer(),{
			header : '项目编码',
			dataIndex : 'project_code',
			sortable : true
		},{
			header : '合同名称',
			dataIndex : 'contractName',
			sortable : true,
			renderer : function(value, cellMeta, record, rowIndex,
					columnIndex, store) {
				if (store.getCount() - 1 < store.baseParams.limit
						&& store.getCount() == rowIndex + 1) {
					return value;
				} else {
					if (rowIndex == store.baseParams.limit) {
						return value;
					}
				}
				return linkRenderer(value, record.get('contractID'),value);
			}
		},{
			header : '凭单号',
			align : 'right',
			width : 50,
			dataIndex : 'ino_id',
			sortable : true
		},{
			header : '记账日期',
			align : 'center',
			dataIndex : 'dbill_date',
			width : 70,
			sortable : true,
			renderer : function(value, cellmeta, record, rowIndex, columnIndex,
					store) {
				if(record.get('flag'))
					return '';
				return value.substring(0,10);
			}
		},{
			header : this.searchType == 1 ? '来款金额' : '采购支出金额',
			align : 'right',
			dataIndex : 'MB',
			sortable : true,
			renderer : function(value, cellmeta, record, rowIndex, columnIndex,
					store) {
				return formatCurrency(null , value, 'blue');
			}
		},{
			header : '备注',
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
		} ];
		this.tbar = [{xtype : 'hidden',
			name : 'searchColumn',
			ref : '../searchColumn',
			value : 'project_code,contractName'
		},{
			xtype : 'combo',
			width : 100,
			tpl:'<tpl for="."><div class="x-combo-list-item comboBackColor">{text}</div></tpl>',
			ref : '../searchWay',
			displayField : 'text',
			valueField : 'value',
			style : 'margin : 0 0 0 2;background-image : url(""); background-color :  #99CCFF ;!important',
			mode : 'local',
			triggerAction : 'all',
			editable : false,
			value : 0,
			store : new Ext.data.ArrayStore({
						fields : ['value', 'text'],
						data : [[0,'部门'], [1,'模糊查询']]
					}),
			listeners : {
				'select' : function(combo, record, index) {
					switch (record.get('value')) {
						case 0 :
							grid.deptCombo.show();
							grid.inputText.hide();
							break;
						case 1 :
							grid.deptCombo.hide();
							grid.inputText.show();
							break;
					}
				}
			}
		},new Ext.form.TextField({
			width : 125,
			hidden : true,
			name : 'searchParam',
			ref : '../searchParam',
			style : 'margin : 0 0 0 5',
			tooltip : {
				title : '模糊查询',
				text : '您可以输入任意关键词进行查询。'
			},
			ref : '../inputText',
			emptyText : '请输入查询内容'
		}),{
			xtype : 'combo',
			ref : '../deptCombo',
			width : 130,
			style : 'margin : 0 5 0 5',
			displayField : 'deptName',
			valueField : 'paramValue',
			editable : false,
			triggerAction : 'all',
			emptyText : '请选择部门',
			lazyInit : false,
			store : new Ext.data.JsonStore({
						url : String
								.format(
										'{0}/contractMain.do?method=getDeptCombo',
										__cxtPath),
						root : 'rows',
						fields : ['paramValue', {
									name : 'deptName',
									mapping : '部门名称'
								}, {
									name : 'deptID',
									mapping : '部门ID'
								}],
						baseParams : {
							contractTypeID : grid.searchType,
							qxurl : 
//							+ '/contractMain.do?method=getContractListByType&contractTypeID='+type
							(grid.searchType == 1) ? __cxtPath + '/contractMain.do?method=integrationSearch' : __cxtPath + '/contractMain.do?method=integrationSearchPayout'
						},
						idProperty : 'deptID',
						autoLoad : true,
						listeners : {
							'load' : function(s, records) {
								if (!records || records.length == 0) {
									grid.deptCombo.setRawValue(__emp.divisionName);
									grid.deptCombo.setReadOnly(true);
								}
							}
						}
					})
			},'&nbsp;开始日期：',
			new Ext.form.DateField({
				ref : '../startDate',
				format : 'Y-m-d',
				minValue : '2008-01-01',
				showToday : true,
				value : new Date(new Date().getFullYear(),0, 1),
				anchor : '100%'
			}), '&nbsp;结束日期：',
			new Ext.form.DateField({
				ref : '../endDate',
				format : 'Y-m-d',
				minValue : '2008-01-01',
				showToday : true,
				value : new Date(),
				anchor : '100%'
			}),'-',{
				ref : '../searchBtn',
				iconCls : 'searchIcon',
				text : '查询',
				handler : function(b, e) {
					if(!dateCompare(grid.startDate.getValue(), grid.endDate.getValue())){
						Ext.Msg.alert('提示','结束日期必须晚于开始日期！');
					}else{
						if(grid.searchWay.getValue() == 0){
							if(grid.deptCombo.store.getCount()!=0){
								var deptID = grid.deptCombo.getValue();
								grid.store.setBaseParam('deptID',deptID);
							}
						}else{
							var searchContent = grid.inputText.getValue();
							var searchColumn = grid.searchColumn.getValue();
							grid.store.setBaseParam('searchContent',searchContent);
							grid.store.setBaseParam('searchColumn',searchColumn);
						}
						grid.store.setBaseParam('searchWay',grid.searchWay.getValue());
						var start = Ext.util.Format.date(grid.startDate.getValue(),'Y-m-d');
						var end = Ext.util.Format.date(grid.endDate.getValue(),'Y-m-d');
						
						grid.store.setBaseParam('begDate',start);
						grid.store.setBaseParam('endDate',end);
						grid.store.reload({
							params : {
								start : 0
							}
						});
					}
				}
			}
		];
		
		var exp = new Ext.ux.Export({
			formater : [{
						columnHeader : '',
						formatClass : 'com.bitc.jk.xmis.export.PagingRowNumber'
					}, {
						columnHeader : '记账日期',
						formatClass : 'com.bitc.jk.xmis.export.SubstringTimer'
					}, {
						columnHeader : '采购支出金额',
						formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
					}, {
						columnHeader : '来款金额',
						formatClass : 'com.bitc.jk.xmis.export.FormatAmt'
					}]
		});
		
		this.bbar = new Ext.ux.TotalPagingToolbar({
			pageSize : 20,
			store : grid.store,
			displayInfo : true,
			displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
			emptyMsg : "无显示数据",
			plugins : [new Ext.ux.PagelimitCombo()],
			items : [new Ext.Button({
				text : this.text || '导出Excel',
				iconCls : 'xls',
				menu : {
					items : [{
						text : '导出本页数据',
						handler : function() {
							exp.onExport(grid,
									grid.searchType == 1 ? '财务来款流水账' : '财务支出流水账', true);
						}
					}, {
						text : '导出所有数据',
						handler : function() {
							exp.onExport(grid,
									grid.searchType == 1 ? '财务来款流水账' : '财务支出流水账', false);
						}
					}]
				}
			})]
		});
		this.viewConfig = {
				forceFit : true,
				getRowClass : function(record, rowIndex, rowParams, store) {
					if (store.getCount() - 1 < store.baseParams.limit
							&& store.getCount() == rowIndex + 1) {
						return 'grid-contract-total-list-yellow';
					} else {
						if (rowIndex == store.baseParams.limit) {
							return 'grid-contract-total-list-yellow';
						}
					}
				}
		}
		com.bitc.xmis.FinancyRunningAccountGrid.superclass.initComponent.call(this);
	}
});