Ext.onReady(function() {

	/**
	 * 数据一览表主体显示区
	 */

	/**
	 * 通过Ext.data.XmlReader读取指定的xml的特定节点，Ext.DomQuery进行xpath选取
	 * 
	 * 例如：获取指定的厂站下的所有设备 record:'station[id=1]>group' 或者 'station[id=1]/group'
	 * 
	 * 读取指定节点的属性，直接在mapping指定，加@符号
	 * 
	 * 读取节点内的内容，直接在mapping指定
	 */

	/**
	 * IIS发布 
	 * 
	 * 选择厂站：stationComb
	 * 选择设备：groupComb
	 * 选择数据类型：typeComb
	 * 以上三个下拉选框的数据都需要改为Ajax方式处理，可参考quickAlarm.js中quickAlarmStore
	 * 
	 * pointStore,数据一览表主体数据的获取同样改为Ajax方式处理，可参考quickAlarm.js中quickAlarmStore
	 * 
	 * 导出需要提供webservice
	 */

	var pageSize = 20;

	var stationComb = new Ext.form.ComboBox({
				name : 'stationComb',
				id : 'station',
				fieldLabel : '厂站',
				labelStyle : 'text-align:right;',
				displayField : 'comment',
				valueField : 'id',
				editable : false,
				mode : 'remote',
				triggerAction : 'all',
				emptyText : '请选择厂站',
				store : new Ext.data.Store({
							autoLoad : true,
							/**
							 * 调用WoixService.asmx/GetSetupXml(),返回厂站配置文件。
							 * 返回中station为厂站节点，group为设备节点，
							 * type为数据类型节点，entry属性为该厂站设备数据类型下包括数据总点数。
							 */
							url : '/Woix/ws?method=getSetupXml',
							reader : new Ext.data.XmlReader({
										record : 'station'
									}, [{
												name : 'id',
												mapping : '@id'
											}, {
												name : 'comment',
												mapping : '@comment'
											}]),
							listeners : {
								'load' : function(s) {
									stationComb.setRawValue(s.getAt(0)
											.get('comment'));
									stationComb.setValue(s.getAt(0).get('id'));
									stationComb
											.fireEvent('select', stationComb);
								}
							}
						}),
				listeners : {
					'select' : function(combo) {
						// 联动更改groupComb值
						groupComb.clearValue();
						var reader = new Ext.data.XmlReader({
									record : 'station[id=' + combo.getValue()
											+ ']>group'
								}, [{
											name : 'id',
											mapping : '@id'
										}, {
											name : 'comment',
											mapping : '@comment'
										}]);
						groupComb.store.reader = reader;
						groupComb.store.load({
									callback : function() {
										groupComb.setRawValue(groupComb.store
												.getAt(0).get('comment'));
										groupComb.setValue(groupComb.store
												.getAt(0).get('id'));
										groupComb
												.fireEvent('select', groupComb);
									}
								});
					}
				}
			});

	var groupComb = new Ext.form.ComboBox({
				name : 'groupComb',
				id : 'group',
				fieldLabel : '设备',
				labelStyle : 'text-align:right;',
				displayField : 'comment',
				valueField : 'id',
				editable : false,
				mode : 'remote',
				triggerAction : 'all',
				emptyText : '请选择设备',
				store : new Ext.data.Store({
							url : '/Woix/ws?method=getSetupXml',
							reader : null
						}),
				listeners : {
					'select' : function(combo, records, opt) {
						// 联动更改typeComb值
						typeComb.clearValue();
						var reader = new Ext.data.XmlReader({
							record : 'group[id=' + combo.getValue() + ']>type'
								// record : 'group[id=1] > type'
							}, [{
									name : 'id',
									mapping : '@id'
								}, {
									name : 'comment',
									mapping : '@comment'
								}, {
									name : 'entry',
									mapping : '@entry'
								}]);
						typeComb.store.reader = reader;
						typeComb.store.load({
									callback : function() {
										typeComb.setRawValue(typeComb.store
												.getAt(0).get('comment'));
										typeComb.setValue(typeComb.store
												.getAt(0).get('id'));
										typeComb.fireEvent('select', typeComb);
									}
								});
					}
				}
			});

	var typeComb = new Ext.form.ComboBox({
				fieldLabel : '数据类型',
				name : 'typeComb',
				id : 'type',
				labelStyle : 'text-align:right;',
				displayField : 'comment',
				valueField : 'id',
				editable : false,
				mode : 'remote',
				triggerAction : 'all',
				emptyText : '请选择数据类型',
				store : new Ext.data.Store({
							url : '/Woix/ws?method=getSetupXml',
							reader : null
						}),
				listeners : {
					'select' : function(combo) {
						pointStore
								.setBaseParam('stnId', stationComb.getValue());
						pointStore.setBaseParam('grpId', groupComb.getValue());
						pointStore.setBaseParam('typeId', typeComb.getValue());
						pointStore.load();

					}
				}
			});

	var pointRecord = new Ext.data.Record.create([{
				name : 'NO',
				mapping : 'NO'
			}, {
				name : 'POINTNAME',
				mapping : 'POINTNAME'
			}, {
				name : 'LONGNAME',
				mapping : 'LONGNAME'
			}, {
				name : 'DATA',
				mapping : 'DATA'
			}]);

	/**
	 * 调用WoixService.asmx/GetPointTableJson(stnId,grpId,typeId,startInx,endInx),
	 * 返回该设备下数据一览表。
	 * stnId为厂站Id，grpId为设备Id,typeId为数据类型Id，startInx为起始索引号，endInx为结束索引号。
	 * 
	 */
	var pointStore = new Ext.data.Store({
				proxy : new Ext.data.HttpProxy({
							url : '/Woix/ws?method=getPointTableJson'
						}),
				reader : new Ext.data.JsonReader({
							root : 'rows',
							totalProperty : 'totalCount'
						}, pointRecord),
				baseParams : {
					start : 0,
					limit : pageSize
				}
			});

	var pointColumn = new Ext.grid.ColumnModel([{
				header : '序号',
				dataIndex : 'NO',
				sortable : false,
				width : 50
			}, {
				header : '点名',
				dataIndex : 'POINTNAME',
				sortable : false,
				width : 300
			}, {
				header : '中文名',
				dataIndex : 'LONGNAME',
				sortable : false,
				width : 300
			}, {
				header : '状态',
				dataIndex : 'DATA',
				sortable : false,
				width : 100
			}]);

	var pointGrid = new Ext.grid.GridPanel({
		region : 'center',
		store : pointStore,
		cm : pointColumn,
		stripeRows : true,
		layout : 'fit',
		border : false,
		autoScroll : true,
		loadMask : true,
		tbar : ['厂站', stationComb, '-', '设备', groupComb, '-', '数据类型', typeComb,
				'-'],
		bbar : new Ext.PagingToolbar({
			pageSize : pageSize,
			store : pointStore,
			displayInfo : true,
			displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
			emptyMsg : "无显示数据",
			items : [new Ext.Button({
						text : '导出excel',
						iconCls : 'excel',

						tabTip : '请先查询，再导出。',
						menu : {
							items : [{
								text : '导出本页数据',
								handler : function() {
									exportExcel(
											pointStore.lastOptions.params.start,
											pointStore.lastOptions.params.limit);
								}
							}, {
								text : '导出所有数据',
								handler : function() {
									exportExcel(0, pointStore.getTotalCount());
								}
							}]
						}
					})],
			plugins : [new Ext.ux.PagelimitCombo]
		})
	});

	new Ext.Viewport({
				border : false,
				layout : 'border',
				items : [pointGrid]
			});

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

		var result = turnStringForColumn(pointGrid.getColumnModel(),
				pointStore.fields);

		Ext.Ajax.request({
					url : '/Woix/ws?method=pointTableExport',
					params : {
						stnId : stationComb.getValue(),
						grpId : groupComb.getValue(),
						typeId : typeComb.getValue(),
						start : start,
						limit : limit,
						header : result.columns,
						field : result.fields,
						title : '数据一览表'
					},
					method : 'post',
					timeout : 2 * 60 * 1000,
					success : function(response, opt) {
						var result = Ext.decode(response.responseText);
						if (result.success) {
							var path = result.attributes.downloadpath;
							location.href = '/Woix/ws?' + path;
							Ext.Msg.hide();
						} else {
							Ext.Msg.alert("失败", "导出失败，请重新导出。");
						}
					},
					failure : function() {
						Ext.Msg.alert("提示", "导出失败，请重试。");
					}
				});
	}
});