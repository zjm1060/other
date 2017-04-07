/**
 * 2013/10/16 朱传豪grid序号
 */
Ext.ns('com.bitc.xmis');
/**
 * @class com.bitc.xmis.Attachment
 * @static
 * @singlton
 */
com.bitc.xmis.Attachment = {
	contract : 1,
	payment : 2,
	receipt : 3,
	invoice : 4,
	project : 6,
	materials : 7,
	staffInfo : 8,// 人员相关文档
	intercheck : 9,// 内部验收文档
	supplier : 10,// 供方评定相关文档
	qualification : 11, // 资质信息相关文档
	importDevice : 12,// 导入设备清单
	patentDocuments : 13,// 专利文件
	bbzSalaryExcel : 14,// 工资
	absentForm : 15, // 请假单
	incomeAccount : 16, // 收入到账
	bankDeposits : 17,// 银行存款
	document : 18
	// 公文
	//
};
/**
 * @class com.bitc.xmis.AttachmentList 附件列表 (提供删除、下载)
 * @extends Ext.grid.GridPanel
 * 
 * @cfg {String} REFID 附件主体引用ID，如果为空（null,undefined,''）列表不会自动加载
 * @cfg {Integer} attachmentTypeID 附件主体类型ID
 */
com.bitc.xmis.AttachmentList = Ext.extend(Ext.grid.GridPanel, {
	autoExpandColumn : 'filename',
	hideHeaders : true,
	REFID : '',
	NOT_DELETE:false,
	READONLY : false,
	actionColumnHidden : false,
	viewConfig : {
		emptyText : '还未添加附件'
	},
	/**
	 * 设置隐藏的列
	 */
	setActionColumnHidden : function(flag) {
		this.getColumnModel().setHidden(2, flag);
		this.getColumnModel().setHidden(3, flag);
		this.getColumnModel().setHidden(4, flag);
	},
	/**
	 * 传2或3；将所传列隐藏
	 */
	actionColumnHiddenIndex : '',
	initComponent : function() {

		/**
		 * @property store Grid Store
		 * @type Ext.data.Store
		 */
		this.store = new Ext.data.JsonStore({
					url : String.format(
							'{0}/fileUpload.do?method=getAttachmentList',
							__cxtPath),
					root : 'rows',
					totalProperty : 'totalCount',
					baseParams : {
						REFID : this.REFID,
						attachmentTypeID : this.attachmentTypeID
					},
					fields : [DBSchema.Attachment.AttachmentID,
							DBSchema.Attachment.AttachmentName]
				});
		if (!Ext.isEmpty(this.REFID)) {
			this.store.load();
		}
		this.action_upload = new Ext.ux.grid.RowActions({
					header : '下载',
					dataIndex : DBSchema.Attachment.AttachmentID,
					width : 15,
					hidden : this.actionColumnHiddenIndex == ''
							? this.actionColumnHidden
							: (this.actionColumnHiddenIndex.indexOf('2') > -1
									? true
									: false),
					actions : [{
						iconCls : 'download',
						tooltip : '下载',
						callback : function(grid, record, action, rowIndex,
								colIndex) {
							location.href = __cxtPath
									+ '/fileUpload.do?type=3&attID='
									+ record
											.get(DBSchema.Attachment.AttachmentID);
						}
					}]

				});
		var s = this.store;
		this.action_del = new Ext.ux.grid.RowActions({
			header : '删除',
			dataIndex : DBSchema.Attachment.AttachmentID,
			width : 15,
			hidden : this.NOT_DELETE ? true : (this.actionColumnHiddenIndex == ''
					? this.actionColumnHidden
					: (this.actionColumnHiddenIndex.indexOf('3') > -1
							? true
							: false)),
			actions : [{
				iconCls : 'delete',
				tooltip : '删除',
				callback : function(grid, record, action, rowIndex, colIndex) {
					Ext.Msg
							.confirm(
									'确认',
									'您确定要删除'
											+ record
													.get(DBSchema.Attachment.AttachmentName)
											+ '?', function(buttonId) {
										if (buttonId == 'yes') {
											Ext.Ajax.request({
												url : String
														.format(
																'{0}/fileUpload.do?type=0',
																__cxtPath),
												scope : this,
												success : function() {
													s.reload();
												},
												method : "post",
												params : {
													AttachementID : record
															.get(DBSchema.Attachment.AttachmentID)
												}
											});
										}
									}, this);
				}
			}]

		});
		this.action_edit = new Ext.ux.grid.RowActions({
			header : '编辑',
			dataIndex : DBSchema.Attachment.AttachmentID,
			width : 15,
			actions : [{
				iconCls : 'edit',
				tooltip : '编辑',
				callback : function(grid, record, action, rowIndex, colIndex) {
					var attachementId = record
							.get(DBSchema.Attachment.AttachmentID);
					var xMis = xMis || parent.xMis;
					if (this.READONLY) {
						xMis
								.turn(
										'attachement-edit-' + attachementId,
										'文档预览-'
												+ record
														.get(DBSchema.Attachment.AttachmentName),
										__cxtPath
												+ '/weboffice.do?method=openAttachment&readOnly=true&attID='
												+ attachementId, '', false);

					} else {
						xMis
								.turn(
										'attachement-edit-' + attachementId,
										'文档预览编辑-'
												+ record
														.get(DBSchema.Attachment.AttachmentName),
										__cxtPath
												+ '/weboffice.do?method=openAttachment&attID='
												+ attachementId, '', false);
					}
					// var win;
					// if (!win) {
					// win = new Ext.Window({
					// title : record.get(DBSchema.Attachment.AttachmentName),
					// layout : 'border',
					// modal : true,
					// width : 780,
					// height : 480,
					// closeAction : 'hide',
					// plain : true,
					// items : [new Ext.Panel({
					// region : 'center',
					// ref : 'p',
					// frame : true,
					// border : false,
					// labelAlign : 'left',
					// labelWidth : 80,
					// // bodyStyle : 'padding:10px',
					// buttonAlign : 'center',
					// autoScroll : true,
					// items : [{
					// title : '弹出的窗口',
					// header : false,
					// html : '<iframe style="border-top-width: 0px;
					// border-left-width: 0px; border-bottom-width: 0px; width:
					// 728px; height: 455px; border-right-width: 0px" src="'
					// + __cxtPath
					// + '/weboffice.do?method=openAttachment&attID='
					// + record
					// .get(DBSchema.Attachment.AttachmentID)
					// + '" frameborder="0" width="100%" scrolling="no"
					// height="100%"></iframe>',
					// border : false
					// }]
					// })]
					//
					// });
					// }
					// win.show(this);
				}
			}]

		});
		this.columns = [new Ext.grid.RowNumberer({
							width : 30
						}), {
					id : 'filename',
					header : '文件名称',
					sortable : true,
					dataIndex : DBSchema.Attachment.AttachmentName
				}, this.action_upload, this.action_del,this.action_edit];
		this.plugins = [this.action_upload, this.action_del, this.action_edit];
		// this.action.on('action', function(grid, record, action, row, col) {},
		// this);
		com.bitc.xmis.AttachmentList.superclass.initComponent.call(this);
	},
	/**
	 * 重新加载附件列表
	 * 
	 * @param {Object}
	 *            options 调用Store的reload时使用的参数
	 * @return {String}
	 */
	reload : function(options) {
		this.getStore().reload(options);
		return '';
	}
});
