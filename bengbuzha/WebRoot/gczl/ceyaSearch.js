Ext.ns('com.bitc.xmis');
com.bitc.xmis.SearchWinShow = function(storeGYS) {
	var myData = [['>', '5'], ['<', '10']];
	var gridstore = new Ext.data.ArrayStore({
				fields : [{
							name : 'columnname',
							type : 'string'
						}, {
							name : 'datatype',
							type : 'string'
						}, {
							name : 'dbname',
							type : 'string'
						}, {
							name : 'option',
							type : 'string'
						}, {
							name : 'value',
							type : 'string'
						}, {
							name : 'relation',
							type : 'string'

						}]
			});
	var selectnum = '';
	var contentGridPanel = new Ext.grid.GridPanel({
				region : 'center',
				store : gridstore,
				autoScroll : true,
				sm : new Ext.grid.RowSelectionModel({
							singleSelect : true,
							listeners : {
								rowselect : function(sm, row, record) {
									selectnum = row;
									searchForm.getForm().loadRecord(record);
									var value = record.get('value');
									var option = record.get('option');

									var dbname = record.get('dbname');
									var datatype = record.get('datatype');
									searchForm.getForm().findField('datatype')
											.setValue(datatype);
									searchForm.getForm().findField('dbname')
											.setValue(dbname);
									searchForm.getForm()
											.findField('displayvalue')
											.setVisible(false);
									if (datatype == 'date') {
										searchForm.getForm()
												.findField('datevalue')
												.setValue(value);
										searchForm.getForm()
												.findField('datevalue')
												.setVisible(true);
										searchForm.getForm()
												.findField('stringoption')
												.setVisible(false);
										searchForm.getForm()
												.findField('option')
												.setVisible(true);
									} else if (datatype == 'time') {
										searchForm.getForm()
												.findField('timevalue')
												.setValue(value);
										searchForm.getForm()
												.findField('timevalue')
												.setVisible(true);
										searchForm.getForm()
												.findField('stringoption')
												.setVisible(false);
										searchForm.getForm()
												.findField('option')
												.setVisible(true);
									} else if (datatype == 'numberic') {
										searchForm.getForm()
												.findField('numvalue')
												.setValue(value);
										searchForm.getForm()
												.findField('numvalue')
												.setVisible(true);

										searchForm.getForm()
												.findField('stringoption')
												.setVisible(false);
										searchForm.getForm()
												.findField('option')
												.setVisible(true);

									} else {
										if (datatype == 'string') {
											searchForm.getForm()
													.findField('stringoption')
													.setValue(option);
											searchForm.getForm()
													.findField('stringoption')
													.setVisible(true);
											searchForm.getForm()
													.findField('option')
													.setVisible(false);
										} else {
											searchForm.getForm()
													.findField('stringoption')
													.setVisible(false);
											searchForm.getForm()
													.findField('option')
													.setVisible(true);
										}
										searchForm.getForm().findField('value')
												.setVisible(true);

									}

								}
							}
						}),
				height : 200,
				columns : [{
							header : '列名字',
							width : 160,
							sortable : true,
							dataIndex : 'columnname'
						}, {
							header : '符号',
							width : 75,
							sortable : true,
							dataIndex : 'option'
						}, {
							header : '值',
							width : 160,
							sortable : true,
							dataIndex : 'value'
						}, {
							header : '关系符号',
							width : 75,
							sortable : true,
							dataIndex : 'relation'
						}, {
							header : '列类型',
							width : 160,
							hidden : true,
							dataIndex : 'datatype'
						}, {
							header : '数据库字段',
							width : 160,
							hidden : true,
							dataIndex : 'dbname'
						}, {
							xtype : 'actioncolumn',
							width : 50,
							align : 'center',
							items : [{
								iconCls : 'remove2',
								tooltip : '删除',
								handler : function(grid, rowIndex, colIndex) {
									var rec = gridstore.getAt(rowIndex);
									Ext.MessageBox.confirm("提示", "是否删除此条信息？",
											function(btn) {
												if (btn == 'yes') {
													gridstore.remove(rec);
												}
											});
								}
							}]
						}]
			});

	var searchForm = new Ext.form.FormPanel({
		region : 'north',
		width:550,
		height : 220,
		bodyStyle : 'padding:10px 5px 0px 5px',
		labelAlign : 'right',
		labelWidth : 100,
		bbar : [{
			text : '添加条件',
			iconCls : 'add',
			handler : function() {

				var datatype = searchForm.getForm().findField('datatype')
						.getValue();

				var array = new Array();

				array[0] = searchForm.getForm().findField('columnname')
						.getValue();
				array[1] = searchForm.getForm().findField('option').getValue();

				if (datatype == 'string') {
					array[1] = searchForm.getForm().findField('stringoption')
							.getValue();
				}
				array[2] = searchForm.getForm().findField('value').getValue();

				if (datatype == 'date') {
					array[2] = Ext.util.Format
							.date(	searchForm.getForm().findField('datevalue')
											.getValue(), 'Y-m-d');
				} else if (datatype == 'time') {
					array[2] = searchForm.getForm().findField('timevalue')
							.getValue();
				} else if (datatype == 'numberic') {
					array[2] = searchForm.getForm().findField('numvalue')
							.getValue();
				} else {
					array[2] = searchForm.getForm().findField('value')
							.getValue();
				}

				array[3] = searchForm.getForm().findField('relation')
						.getValue();
				array[4] = searchForm.getForm().findField('datatype')
						.getValue();
				array[5] = searchForm.getForm().findField('dbname').getValue();

				if (!(array[0] != '' && array[1] != '' && array[2] != ''
						&& array[3] != '' && array[4] != '' && array[5] != '')) {
					Ext.MessageBox.alert('提示', '查询条件不允许为空')
					return;
				}

				var u = new contentGridPanel.store.recordType({
							columnname : array[0],
							option : array[1],
							value : array[2],
							relation : array[3],
							datatype : array[4],
							dbname : array[5]
						});
				var nownum = contentGridPanel.store.data.length;
				contentGridPanel.store.insert(nownum, u);
				// contentGridPanel.getSelectionModel().selectRow(nownum);

				searchForm.getForm().reset();
				formpanelValueFieldReset(searchForm);
			}

		}, {
			text : '保存修改后的条件',
			iconCls : 'save',
			handler : function() {
				var array = new Array();

				array[0] = searchForm.getForm().findField('columnname')
						.getValue();
				array[1] = searchForm.getForm().findField('option').getValue();
				if (datatype == 'string') {
					array[1] = searchForm.getForm().findField('stringoption')
							.getValue();
				}

				array[2] = searchForm.getForm().findField('value').getValue();

				var datatype = searchForm.getForm().findField('datatype')
						.getValue();
				if (datatype == 'date') {
					array[2] = Ext.util.Format
							.date(	searchForm.getForm().findField('datevalue')
											.getValue(), 'Y-m-d');
				} else if (datatype == 'time') {
					array[2] = searchForm.getForm().findField('timevalue')
							.getValue();
				} else if (datatype == 'numberic') {
					array[2] = searchForm.getForm().findField('numvalue')
							.getValue();
				} else {
					array[2] = searchForm.getForm().findField('value')
							.getValue();
				}

				array[3] = searchForm.getForm().findField('relation')
						.getValue();
				array[4] = searchForm.getForm().findField('datatype')
						.getValue();
				array[5] = searchForm.getForm().findField('dbname').getValue();
				if (!(array[0] != '' && array[1] != '' && array[2] != ''
						&& array[3] != '' && array[4] != '' && array[5] != '')) {
					Ext.MessageBox.alert('提示', '查询条件不允许为空')
					return;
				}

				var u = new contentGridPanel.store.recordType({
							columnname : array[0],
							option : array[1],
							value : array[2],
							relation : array[3],
							datatype : array[4],
							dbname : array[4]
						});

				contentGridPanel.store.remove(contentGridPanel.store
						.getAt(selectnum));
				contentGridPanel.store.insert(selectnum, u);
				// contentGridPanel.getSelectionModel().selectRow(nownum);

				searchForm.getForm().reset();
				formpanelValueFieldReset(searchForm);
			}

			}, '->', '<font color=green>选中行可以对数据进行修改</font>'],
		items : [{
			layout : 'form',
			xtype : 'panel',
			autoScroll : true,
			border : false,
			defaultType : 'textfield',
			items : [new com.bitc.xmis.ColumnCombo({
								fieldLabel : '列名字',
								width : 200,
								allowBlank : false,
								hiddenName : 'columnname'
							}), {
						xtype : 'combo',
						displayField : 'text',
						valueField : 'text',
						mode : 'local',
						triggerAction : 'all',
						editable : false,
						fieldLabel : '符号',
						hiddenName : 'option',
						width : 200,
						store : new Ext.data.ArrayStore({
									fields : ['text'],
									data : [['='], ['>'], ['<'], ['>='],
											['<='], [' LIKE ']]
								})
					}, {
						xtype : 'combo',
						displayField : 'text',
						valueField : 'text',
						mode : 'local',
						triggerAction : 'all',
						editable : false,
						hidden : true,
						fieldLabel : '符号',
						hiddenName : 'stringoption',
						width : 200,
						store : new Ext.data.ArrayStore({
									fields : ['text'],
									data : [['='], [' LIKE ']]
								})
					}, {
						xtype : 'hidden',
						fieldLabel : '数据类型',
						width : 200,
						name : 'datatype'
					}, {
						xtype : 'hidden',
						fieldLabel : '数据库字段',
						width : 200,
						name : 'dbname'
					}, {
						xtype : 'textfield',
						fieldLabel : '值',
						width : 200,
						disabled : true,
						value : '请先选择列名字',
						name : 'displayvalue'
					}, {
						xtype : 'textfield',
						fieldLabel : '值',
						width : 200,
						hidden : true,
						name : 'value'
					}, {
						xtype : 'numberfield',
						fieldLabel : '值',
						width : 200,
						hidden : true,
						name : 'numvalue'
					}, {
						xtype : 'datefield',
						fieldLabel : '值',
						width : 200,
						hidden : true,
						format : 'Y-m-d',
						name : 'datevalue'
					}, {
						xtype : 'textfield',
						fieldLabel : '值',
						width : 200,
						regex : /^(0\d{1}|1\d{1}|2[0-3]):[0-5]\d{1}:([0-5]\d{1})$/,
						invalidText : "时间格式错误，正确格式**:**:**",
						hidden : true,
						name : 'timevalue'
					}, {
						xtype : 'combo',
						displayField : 'text',
						valueField : 'text',
						mode : 'local',
						allowBlank : false,
						triggerAction : 'all',
						editable : false,
						fieldLabel : '关系符号',
						hiddenName : 'relation',
						width : 200,
						value:'AND',
						store : new Ext.data.ArrayStore({
									fields : ['text'],
									data : [['AND'], [' OR']]
								})
					} ]
		}]
	});

	searchForm.getForm().findField('columnname').on('select',
			function(combo, record, index) {
				formpanelValueFieldReset(searchForm);

				var datatype = record.get('datatype');
				var dbname = record.get('dbname');
				searchForm.getForm().findField('datatype').setValue(datatype);
				searchForm.getForm().findField('dbname').setValue(dbname);
				searchForm.getForm().findField('displayvalue')
						.setVisible(false);

				if (datatype == 'date') {
					searchForm.getForm().findField('datevalue')
							.setVisible(true);

					searchForm.getForm().findField('option').setVisible(true);
					searchForm.getForm().findField('stringoption')
							.setVisible(false);
				} else if (datatype == 'time') {
					searchForm.getForm().findField('timevalue')
							.setVisible(true);

					searchForm.getForm().findField('option').setVisible(true);
					searchForm.getForm().findField('stringoption')
							.setVisible(false);
				} else if (datatype == 'numberic') {
					searchForm.getForm().findField('numvalue').setVisible(true);
					searchForm.getForm().findField('option').setVisible(true);

					searchForm.getForm().findField('stringoption')
							.setVisible(false);
				} else {
					if (datatype == 'string') {
						searchForm.getForm().findField('stringoption')
								.setVisible(true);
						searchForm.getForm().findField('option')
								.setVisible(false);

					}
					searchForm.getForm().findField('value').setVisible(true);
				}

			});
	var searchWin = new Ext.Window({
		title : '组合查询',
		width : 560,
		height : 480,
		border : false,
		fbar : {
			xtype : 'toolbar',
			buttonAlign : 'center',
			items : [{
				xtype : 'button',
				text : '查询',
				ref : '../save_btn',
				iconCls : 'save',
				handler : function() {
					if(contentGridPanel.store.data.length >0 ){
						var storeArray = com.bitc.xmis.util.ExtUtil
						.getStoreData(contentGridPanel.store);
				var str = "";
				for (var i = 0; i < contentGridPanel.store.data.length; i++) {
					var temp = storeArray[i].value;
					var option = storeArray[i].option;
					if(option ==' LIKE '){
						temp = '%'+temp+'%';
					}
					str += ' (' + storeArray[i].dbname
							+ option + "'"
							+ temp + "'" + ') '
							+ storeArray[i].relation;
							
				}
				str = str.substring(0, str.length - 3);
				searchWin.hide();
				storeGYS.setBaseParam('queryStr', str);
				storeGYS.reload({
							params : {
								start : 0,
								limit : 20
							}
						});
			}else{
				Ext.Msg.alert('提示','没有添加查询条件');
			}
		
		}
			}]
		},
		layout : 'border',
		items : [contentGridPanel, searchForm],
		closable : true,
		closeAction : 'hide',
		modal : true
	});
	searchWin.show();
	function formpanelValueFieldReset(searchForm) {
		searchForm.getForm().findField('displayvalue').setVisible(true);
		searchForm.getForm().findField('value').setVisible(false);
		searchForm.getForm().findField('datevalue').setVisible(false);
		searchForm.getForm().findField('timevalue').setVisible(false);
		searchForm.getForm().findField('numvalue').setVisible(false);

		searchForm.getForm().findField('option').setVisible(true);
		searchForm.getForm().findField('stringoption').setVisible(false);

	};
};

/**
 * 列名字：显示的名字、数据库中字段的名字、类型
 */
com.bitc.xmis.ColumnCombo = Ext.extend(Ext.form.ComboBox, {
	valueField : 'columnname',
	displayField : 'columnname',
	triggerAction : 'all',
	lazyInit : false,
	mode : 'remote',
	editable : false,
	emptyText : '选择列名字',
	editable : false,
	initComponent : function() {
    
		this.store = new Ext.data.JsonStore({
					url : String
							.format(
									'{0}/applyManageMaint.do?method=getCeYaColumnNameCombo',
									__cxtPath),
					root : 'rows',
					autoLoad : true,
					fields : [{
								name : 'columnname',
								type : 'string'
							}, {
								name : 'dbname',
								type : 'string'
							}, {
								name : 'datatype',
								type : 'string'
							}],
					listeners : {
						scope : this,
						'load' : function() {
							this.setValue(this.value);
						}
					}
				});

		com.bitc.xmis.ColumnCombo.superclass.initComponent.call(this);
	}
});