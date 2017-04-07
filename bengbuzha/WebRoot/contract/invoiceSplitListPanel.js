Ext.ns('com.bitc.xmis.contract');

com.bitc.xmis.contract.incoiceSplitListPanel = Ext
		.extend(
				Ext.Panel,
				{
					initComponent : function() {
						if (this.contractID && this.invoiceNumber) {
							this.layout = 'border';
							var grid = this.buildGrid();
							this.items = [ grid ];
						}
						com.bitc.xmis.contract.incoiceSplitListPanel.superclass.initComponent
								.call(this);
					},
					buildGrid : function() {
						var conID = this.contractID;
						var inNum = this.invoiceNumber;
						var totalAmount = this.totalAmount

					var ID = "ID";
					var contractID = "合同ID";
					var invoiceNumber = "发票号";
					var splitSum = "拆分金额";
					var payFlag = "是否已付款";
					var anticipatedTime = "预计付款时间";
					var remark = "备注";
					var registrationTime = "登记时间";
					var registrantName = "登记人";

					var invoiceRecord = Ext.data.Record.create( [ {
						name : ID
					}, {
						name : contractID
					}, {
						name : invoiceNumber
					}, {
						name : splitSum,
						type : 'number'
					}, {
						name : payFlag
					}, {
						name : anticipatedTime,
						type : 'date',
						dateFormat : 'Y-m-d' //H:i:s.u
					}, {
						name : remark
					}, {
						name : registrationTime
					}, {
						name : registrantName
					} ]);

					var invoiceStore = new Ext.data.Store({
								url : __cxtPath + '/contractMain.do?method=getInvoiceSplitDetailBySimpleInvoice',
								baseParams : {
									contractID : conID,
									invoiceNumber : inNum
								},
								autoLoad : true,
								reader : new Ext.data.JsonReader( {
									root : 'rows',
									totalProperty : 'totalCount',
									remoteSort : true
								}, invoiceRecord),
								listeners : {
									'remove' : function(store,record,index){
										if(''!=record.get(ID))
										Ext.Ajax.request({
											url : __cxtPath + '/contractMain.do?method=deleteInvoiceSplitInfo',
											method : 'post',
											params : {
												ID : record.get(ID)
											},
											success : function(response,config) {
												var action = Ext.decode(response.responseText);
												invoiceStore.reload();
												Ext.Msg.alert(action.messageTitle,action.message);
											},
											failure : function(response,config) {
												var action = Ext.decode(response.responseText);
												Ext.Msg.alert(action.messageTitle,action.message);
											}
										});
									},
									'update' : function(store,record,operation){
										var totalAmt = 0.00;
										for(var i = 0 ; i < store.getRange().length ; i ++ ){
											totalAmt += Number(store.getRange()[i].get(splitSum));
										}
										if(totalAmt > totalAmount){
											Ext.Msg.alert('提示','预计金额总额超出票据金额，请检查！');
											if('' == record.get(ID)){ //添加新数据
												
												var s = invoiceSplitGrid.getSelectionModel()
													.getSelections();
												for ( var i = 0, r; r = s[i]; i++) {
													if('' == r.get(ID)){
														store.remove(r);
													}
												}
												invoiceSplitGrid.getView().refresh();
											}else{ //修改数据
												record.reject(true);
											}
											invoiceSplitGrid.removeBtn.setDisabled(false);
											invoiceSplitGrid.newBtn.setDisabled(false);
											return; 
										}else{
											if(operation == Ext.data.Record.EDIT){
											var url = '';
											if('' == record.get(ID)){ //添加新数据
												url = __cxtPath + '/contractMain.do?method=saveInvoiceSplitInfo';
											}else{ //修改数据
												url = __cxtPath + '/contractMain.do?method=updateInvoiceSplitInfo';
											}
											Ext.Ajax.request({
											url : url,
											method : 'post',
											params : {
												ID : record.get(ID),
												contractID : conID,
												invoiceNumber : inNum,
												splitSum : record.get(splitSum),
												payFlag : record.get(payFlag),
												anticipatedTime : Ext.util.Format.date(record.get(anticipatedTime),'Y-m-d'),
												remark : record.get(remark)
											},
											success : function(response,config) {
												var action = Ext.decode(response.responseText);
												invoiceStore.reload();
												Ext.Msg.alert(action.messageTitle,action.message);
												invoiceSplitGrid.removeBtn.setDisabled(false);
												invoiceSplitGrid.newBtn.setDisabled(false);
											},
											failure : function(response,config) {
												var action = Ext.decode(response.responseText);
												Ext.Msg.alert(action.messageTitle,action.message);
											}
										});
											
										}
											
										}
										
									}
								}
					});

					var invoiceEditor = new Ext.ux.grid.RowEditor( {
						saveText : '确认',
						cancelText : '取消',
						clicksToEdit : 2
					});
					var invoiceDateEditField = new Ext.form.DateField(
						{
							format : 'Y-m-d',
							editable : false,
							allowBlank : false
					});
					
					var invoiceSplitSumField = new Ext.form.NumberField(
						{
							minValue : 0,
							allowBlank : false
					});
					
					var paymentCheckbox = new Ext.grid.CheckColumn({
						header : "是否已付款",
						dataIndex : payFlag,
						align : 'center',
						width : 80,
						sortable : true,
						renderer: function(v,p){
							p.css += ' x-grid3-check-col-td'; 
        					return String.format('<div class="x-grid3-check-col{0} {1}">&#160;</div>',   v=='true' || v== true ? '-on' : '', this.createId())
						},
						editor : {
							xtype : 'checkbox',
							value : 'false',
							listeners : {
								'check' : function() {
								}
							}
						}
					});
					
					var invoiceSplitRemarkField = new Ext.form.TextField();
					invoiceEditor
							.on( {
								scope : this,
								beforeedit : function(roweditor, changes,
										record, rowIndex) {
									invoiceSplitGrid.removeBtn.setDisabled(true);
									invoiceSplitGrid.newBtn.setDisabled(true);
								},
								validateedit : function(roweditor, changes,
										record, rowIndex) {
								},
								afteredit : function(roweditor, changes,
										record, rowIndex) {
								},
								canceledit : function() {
									invoiceEditor.stopEditing(false);
										var s = invoiceSplitGrid.getSelectionModel()
												.getSelections();
										for ( var i = 0, r; r = s[i]; i++) {
											if('' == r.get(ID)){
												invoiceStore.remove(r);
											}
										}
									invoiceSplitGrid.getView().refresh();
									invoiceSplitGrid.removeBtn.setDisabled(false);
									invoiceSplitGrid.newBtn.setDisabled(false);
								}
							});

					var invoiceSplitGrid = new Ext.grid.GridPanel(
							{
								store : invoiceStore,
								region : 'center',
								border : false,
								plugins : this.readOnly == true ? []
										: [ invoiceEditor ],
								enableColumnResize : true, // 设置为false可以禁止这个表格的列改变宽度 (默认值为 true).
								viewConfig: {
            						forceFit:true
        						},
								stripeRows : true, // True表示使用不同颜色间隔行，默认为false
								loadMask : true, // 读取数据时的遮罩和提示功能，默认为false
								loadMask : {
									msg : '正在加载数据，请稍侯……'
								},
								columns : [ new Ext.grid.RowNumberer(), {
									header : "预计付款时间",
									dataIndex : anticipatedTime,
									align : 'center',
									xtype : 'datecolumn',
									format : 'Y-m-d',
									sortable : true,
									editor : invoiceDateEditField
								}, {
									header : "预计付款金额",
									align : 'right',
									xtype : 'numbercolumn',
									format : '0,000.00',
									dataIndex : splitSum,
									sortable : true,
									editor : invoiceSplitSumField
								}
								, {
									header : "备注",
									width : 120,
									dataIndex : remark,
									renderer : function(value, cellMeta, record, rowIndex, columnIndex,
									store){
									if(value!=null){
										var baseNum = 18;
										var msg = '';
										for(var i = 0 ; i < value.length/baseNum ; i++){
											msg += value.substr(i*baseNum,baseNum) + "<br/>";
										}
										cellMeta.attr = 'ext:qtitle="备注" ext:qtip="'+msg+'"';
									}
										return value;
									},
									editor : invoiceSplitRemarkField
								}, paymentCheckbox
								, {
									header : "登记时间",
									align : 'center',
									dataIndex : registrationTime
								}, {
									header : "登记人",
									width : 80,
									dataIndex : registrantName
								} ],
								tbar : [
										{
											ref : '../newBtn',
											iconCls : 'add',
											text : '新建',
											handler : function() {
												var e = new invoiceRecord( {
													ID : '',
													splitSum : '',
													payFlag : '',
													anticipatedTime : '',
													remark : ''
												});
												invoiceEditor.stopEditing();
												invoiceStore.insert(0, e);
												invoiceSplitGrid.getSelectionModel()
														.selectRow(0);
												invoiceEditor.startEditing(0);
											}
										},
										'-',
										{
											ref : '../removeBtn',
											iconCls : 'remove',
											text : '删除',
											disabled : false,
											handler : function() {
												var record = invoiceSplitGrid
														.getSelectionModel()
														.getSelected();
												if (record == 'undefined'
														|| record == undefined) {
													Ext.Msg.alert("未选择",
															"请先选择！");
													return true;
												} else {
													Ext.Msg.confirm("请确认","确定要删除该条推迟原因吗？",function(btn) {
														if (btn == 'yes') {
															invoiceStore.remove(record);
														}
													});
												}
											}
										}, '->', "*双击进入编辑状态" ]
							});

					if (this.readOnly) {
						invoiceSplitGrid.getTopToolbar().hide();
						
					}
					return invoiceSplitGrid;
				}
				});