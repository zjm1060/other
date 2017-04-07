/*
 * If all uploads succeed: {"success":true} If an upload fails:
 * {"success":false,"error":"Reason for error!"}
 */
Ext.ns('Ext.ux');

// keel.UploadPanel
Ext.ux.UploadPanel = function(cfg) {
	this.width = 510;
	this.height = 200;
	Ext.apply(this, cfg);
	this.gp = new Ext.grid.GridPanel({
				border : false,
				store : new Ext.data.Store({
							fields : ['id', 'name', 'type', 'size', 'state',
									'percent', 'causes']
						}),
				columns : [new Ext.grid.RowNumberer(), {
							header : '文件名',
							width : 100,
							sortable : true,
							dataIndex : 'name',
							menuDisabled : true
						}, {
							header : '类型',
							width : 70,
							sortable : true,
							dataIndex : 'type',
							menuDisabled : true
						}, {
							header : '大小',
							width : 100,
							sortable : true,
							dataIndex : 'size',
							menuDisabled : true,
							renderer : this.formatFileSize
						}, {
							header : '进度',
							width : 150,
							sortable : true,
							dataIndex : 'percent',
							menuDisabled : true,
							renderer : this.formatProgressBar,
							scope : this
						}, {
							header : '状态',
							width : 70,
							sortable : true,
							dataIndex : 'state',
							menuDisabled : true,
							renderer : this.formatFileState,
							scope : this
						}, {
							header : '失败原因',
							width : 70,
							sortable : false,
							dataIndex : 'causes',
							menuDisabled : true,
							renderer : this.formatCauses
						}, {
							header : '&nbsp;',
							width : 40,
							dataIndex : 'id',
							menuDisabled : true,
							renderer : this.formatDelBtn
						}]
			});
	this.setting = {
		upload_url : this.uploadUrl,
		flash_url : this.flashUrl,
		file_size_limit : this.fileSize || __upload.fileSize || (1024 * 50),// 上传文件体积上限，单位MB
		// __upload为全局变量，在dynamic.jsp中定义
		file_post_name : this.filePostName,
		file_types : this.fileTypes || __upload.fileTypes || "*.*", // 允许上传的文件类型
		file_types_description : __upload.fileTypesDescription || "All Files", // 文件类型描述
		file_upload_limit : "0", // 限定用户一次性最多上传多少个文件，在上传过程中，该数字会累加，如果设置为“0”，则表示没有限制
		// file_queue_limit : "10",//上传队列数量限制，该项通常不需设置，会根据file_upload_limit自动赋值
		post_params : this.postParams || {
			savePath : 'upload\\'
		},
		use_query_string : true,
		debug : false,
		button_cursor : SWFUpload.CURSOR.HAND,
		button_window_mode : SWFUpload.WINDOW_MODE.TRANSPARENT,
		custom_settings : {// 自定义参数
			scope_handler : this
		},
		file_queued_handler : this.onFileQueued,
		swfupload_loaded_handler : function() {
		},// 当Flash控件成功加载后触发的事件处理函数
		file_dialog_start_handler : function() {
		},// 当文件选取对话框弹出前出发的事件处理函数
		file_dialog_complete_handler : this.onDiaogComplete,// 当文件选取对话框关闭后触发的事件处理
		upload_start_handler : this.onUploadStart,// 开始上传文件前触发的事件处理函数
		upload_success_handler : this.onUploadSuccess,// 文件上传成功后触发的事件处理函数
		swfupload_loaded_handler : function() {
		},// 当Flash控件成功加载后触发的事件处理函数
		upload_progress_handler : this.uploadProgress,
		upload_complete_handler : this.onUploadComplete,
		upload_error_handler : this.onUploadError,
		file_queue_error_handler : this.onFileError
	};
	Ext.ux.UploadPanel.superclass.constructor.call(this, {
				tbar : [{
							text : '添加文件',
							iconCls : 'add',
							ref : '../addBtn'
						}, '-', {
							text : '上传',
							ref : '../uploadBtn',
							iconCls : 'up',
							handler : this.startUpload,
							scope : this
						}, '-', {
							text : '停止上传',
							ref : '../stopBtn',
							iconCls : 'delete',
							handler : this.stopUpload,
							scope : this,
							disabled : true
						}, '-', {
							text : '移除所有',
							ref : '../deleteBtn',
							iconCls : 'delete2',
							handler : this.deleteAll,
							scope : this
						}, '-'],
				layout : 'fit',
				items : [this.gp],
				listeners : {
					'afterrender' : function() {
						var em = this.getTopToolbar().get(0).el.child('em');
						var placeHolderId = Ext.id();
						em.setStyle({
									position : 'relative',
									display : 'block'
								});
						em.createChild({
									tag : 'div',
									id : placeHolderId
								});
						this.swfupload = new SWFUpload(Ext.apply(this.setting,
								{
									button_width : em.getWidth(),
									button_height : em.getHeight(),
									button_placeholder_id : placeHolderId
								}));
						this.swfupload.uploadStopped = false;
						Ext.get(this.swfupload.movieName).setStyle({
									position : 'absolute',
									top : 0,
									left : 0
								});
					},
					scope : this,
					delay : 100
				}
			});
};
Ext.extend(Ext.ux.UploadPanel, Ext.Panel, {
	toggleBtn : function(bl) {
		this.addBtn.setDisabled(bl);
		this.uploadBtn.setDisabled(bl);
		this.deleteBtn.setDisabled(bl);
		this.stopBtn.setDisabled(!bl);
		this.gp.getColumnModel().setHidden(6, bl);
	},
	onUploadStart : function(file) {
		var post_params = this.settings.post_params;
		Ext.apply(post_params, {// 处理中文参数问题
			// fileName : file.name
			fileName : encodeURIComponent(file.name)
		});
		this.setPostParams(post_params);
	},
	startUpload : function(index) {
		// this.gp.store.each(this.check, this);
		if (typeof(index) == 'number' && index >= 0) {
			if (index >= this.gp.store.getCount()) {
				return;
			}
			var record = this.gp.store.getAt(index);
			if (record.get('state') != -1) {
				this.startUpload(index + 1);
			} else {
				this.check(record, index);
			}
		} else {
			var ds = this.gp.store;
			this.startUpload(0);
		}
	},
	confirmUpload : function(index) {
		if (this.swfupload) {
			if (this.swfupload.getStats().files_queued > 0) {
				this.swfupload.uploadStopped = false;
				this.toggleBtn(true);
				this.swfupload.uploadIndex = index;
				this.swfupload.startUpload();
				this.swfupload.uploadStopped = true;
			}
		}
	},
	// private
	check : function(record, index) {
		Ext.Ajax.request({
					url : String.format('{0}/fileUpload.do?type=9', __cxtPath),
					scope : this,
					success : function(response) {
						var action = Ext.decode(response.responseText);
						var flag = action.attributes.flag;

						if (flag) {// 存在
							Ext.Msg.confirm('确认', record.get('name')
											+ '已存在,是否覆盖?', function(buttonId) {
										if (buttonId == 'yes') {
											this.del(record, function() {
														this.confirmUpload();
													});
										} else {
											record.set('state', -5);
											record.commit();
											this.swfupload.cancelUpload(record
															.get('id'), false);
											this.startUpload(index + 1);
										}
									}, this);
						} else {// 不存在
							this.confirmUpload();
						}
					},
					failure : Ext.emptyFn,
					method : "post",
					params : Ext.applyIf({
								fileName : record.get('name')
							}, this.postParams || {})
				});

	},
	// private 删除
	del : function(record, callback) {
		Ext.Ajax.request({
					url : String.format('{0}/fileUpload.do?type=10', __cxtPath),
					scope : this,
					success : function() {
						if (callback) {
							callback.call(this);
						}
					},
					failure : Ext.emptyFn,
					method : "post",
					params : Ext.applyIf({
								fileName : record.get('name')
							}, this.postParams || {})
				});
	},
	formatFileSize : function(_v, celmeta, record) {
		return Ext.util.Format.fileSize(_v);
	},
	formatFileState : function(n) {// 文件状态
		switch (n) {
			case -1 :
				return '未上传';
				break;
			case -2 :
				return '正在上传';
				break;
			case -3 :
				return '<div style="color:red;">上传失败</div>';
				break;
			case -4 :
				return '上传成功';
				break;
			case -5 :
				return '取消上传';
				break;
			default :
				return n;
		}
	},
	formatProgressBar : function(v) {
		var progressBarTmp = this.getTplStr(v);
		return progressBarTmp;
	},
	getTplStr : function(v) {
		var bgColor = "orange";
		var borderColor = "#008000";
		return String
				.format(
						'<div>'
								+ '<div style="border:1px solid {0};height:10px;width:{1}px;margin:4px 0px 1px 0px;float:left;">'
								+ '<div style="float:left;background:{2};width:{3}%;height:10px;"><div></div></div>'
								+ '</div>'
								+ '<div style="text-align:center;float:right;width:40px;margin:3px 0px 1px 0px;height:10px;font-size:12px;">{3}%</div>'
								+ '</div>', borderColor, (90), bgColor, v);
	},
	onUploadComplete : function(file) {
		var me = this.customSettings.scope_handler;
		if (file.filestatus == -4) {
			var ds = me.gp.store;
			for (var i = 0; i < ds.getCount(); i++) {
				var record = ds.getAt(i);
				if (record.get('id') == file.id) {
					record.set('percent', 100);
					if (record.get('state') != -3) {
						record.set('state', file.filestatus);
					}
					record.commit();
				}
			}
		}

		if (this.getStats().files_queued > 0 // &&
		// this.uploadStopped
		// ==
		// false
		) {
			// this.startUpload();
			me.startUpload(this.uploadIndex + 1)
		} else {
			me.toggleBtn(false);
			me.linkBtnEvent();
			me.fireEvent('uploadcomplete');
		}
	},
	onFileQueued : function(file) {
		var me = this.customSettings.scope_handler;
		var rec = new Ext.data.Record({
					id : file.id,
					name : file.name,
					size : file.size,
					type : file.type,
					state : file.filestatus,
					percent : 0
				});
		me.gp.getStore().add(rec);
	},
	onUploadSuccess : function(file, serverData) {
		var me = this.customSettings.scope_handler;
		var ds = me.gp.store;
		var result = Ext.util.JSON.decode(serverData);
		if (result.success) {
			for (var i = 0; i < ds.getCount(); i++) {
				var rec = ds.getAt(i);
				if (rec.get('id') == file.id) {
					rec.set('state', file.filestatus);
					rec.commit();
				}
			}
		} else {
			for (var i = 0; i < ds.getCount(); i++) {
				var rec = ds.getAt(i);
				if (rec.get('id') == file.id) {
					rec.set('percent', 0);
					rec.set('state', -3);
					rec.set('causes', result);
					rec.commit();
					break;
				}
			}
		}
		me.linkBtnEvent();
	},
	uploadProgress : function(file, bytesComplete, totalBytes) {// 处理进度条
		var me = this.customSettings.scope_handler;
		var percent = Math.ceil((bytesComplete / totalBytes) * 100);
		percent = percent == 100 ? 99 : percent;
		var ds = me.gp.store;
		for (var i = 0; i < ds.getCount(); i++) {
			var record = ds.getAt(i);
			if (record.get('id') == file.id) {
				record.set('percent', percent);
				record.set('state', file.filestatus);
				record.commit();
			}
		}
	},
	onUploadError : function(file, errorCode, message) {
		var me = this.customSettings.scope_handler;
		me.linkBtnEvent();
		var ds = me.gp.store;
		for (var i = 0; i < ds.getCount(); i++) {
			var rec = ds.getAt(i);
			if (rec.get('id') == file.id) {
				rec.set('percent', 0);
				rec.set('state', file.filestatus);
				rec.commit();
			}
		}
	},
	onFileError : function(file, n) {
		switch (n) {
			case -100 :
				tip('待上传文件列表数量超限，不能选择！');
				break;
			case -110 :
				tip('文件太大，不能选择！');
				break;
			case -120 :
				tip('该文件大小为0，不能选择！');
				break;
			case -130 :
				tip('该文件类型不可以上传！');
				break;
		}
		function tip(msg) {
			Ext.Msg.show({
						title : '提示',
						msg : msg,
						width : 280,
						icon : Ext.Msg.WARNING,
						buttons : Ext.Msg.OK
					});
		}
	},
	onDiaogComplete : function() {
		var me = this.customSettings.scope_handler;
		me.linkBtnEvent();
	},
	stopUpload : function() {
		if (this.swfupload) {
			this.swfupload.uploadStopped = true;
			this.swfupload.stopUpload();
		}
	},
	deleteAll : function() {
		var ds = this.gp.store;
		for (var i = 0; i < ds.getCount(); i++) {
			var record = ds.getAt(i);
			var file_id = record.get('id');
			this.swfupload.cancelUpload(file_id, false);
		}
		ds.removeAll();
		this.swfupload.uploadStopped = false;
	},
	formatCauses : function(v, metadata, record) {
		if (Ext.isEmpty(v)) {
			return '';
		}
		return "<a href='#' id='cau_" + record.get('id')
				+ "' style='color:red' class='causes-btn' ext:qtip='"
				+ v.message + "'>" + v.message + "</a>";
	},
	formatDelBtn : function(v) {
		return "<a href='#' id='"
				+ v
				+ "'  style='color:blue' class='link-btn' ext:qtip='移除该文件'>移除</a>";
	},
	linkBtnEvent : function() {
		Ext.select('a.link-btn', false, this.gp.el.dom).on('click',
				function(o, e) {
					var ds = this.gp.store;
					for (var i = 0; i < ds.getCount(); i++) {
						var rec = ds.getAt(i);
						if (rec.get('id') == e.id) {
							ds.remove(rec);
							break;
						}
					}
					this.swfupload.cancelUpload(e.id, false);
				}, this);
		Ext.select('a.causes-btn', false, this.gp.el.dom).on('click',
				function(o, e) {
					var ds = this.gp.store;
					for (var i = 0; i < ds.getCount(); i++) {
						var rec = ds.getAt(i);
						if ('cau_' + rec.get('id') == e.id) {
							var result = rec.get('causes');
							Ext.Msg.alert('失败原因', result.message);
							break;
						}
					}
				}, this);
	},
	initComponent : function() {
		Ext.ux.UploadPanel.superclass.initComponent.call(this);
		this.addEvents('uploadcomplete');
	},
	onDestroy : function() {
		this.swfupload.destroy();
		Ext.ux.UploadPanel.superclass.onDestroy.call(this);
	}
});
Ext.reg('uploadPanel', Ext.ux.UploadPanel);

/**
 * @class com.bitc.UploadButton 添加附件按钮
 * @extends Ext.Button
 * @cfg {String} REFID 附件主体引用ID
 * @cfg {Integer} attachmentTypeID 附件主体类型ID
 * 
 * border fileSize fileTypes 允许配置
 */
Ext.ux.UploadButton = Ext.extend(Ext.Button, {
			text : '添加附件',
			iconCls : 'attatchment',
			/**
			 * @cfg {Function} REFIDFun 用于REFID有改变的情况
			 */
			REFIDFun : undefined,
			reuses : true,
			handler : function() {
				if (!this.reuses || !this.upWin) {
					this.initWindow();
				}
				// if(!this.upWin){
				// this.initWindow();
				// }
				if (this.REFIDFun) {
					this.setREFID(this.REFIDFun.call(this));
				}
				this.upWin.show();
			},
			initWindow : function() {
				var upPanelConfig = Ext.apply(Ext.apply({
									border : false,
									fileSize : 1024 * 50,// 限制文件大小
									fileTypes : '*.*'// 可上传文件类型
								}, this.uploadPanelConfig || {}), {
							uploadUrl : String.format(
									'{0}/fileUpload.do?type=2', __cxtPath),
							flashUrl : String.format(
									'{0}/resources/upload/swfupload.swf',
									__cxtPath),
							filePostName : 'userfile', // 后台接收参数
							postParams : {
								REFID : this.REFID,
								attachmentTypeID : this.attachmentTypeID
							}
						});
				var upPanel = this.upPanel = new Ext.ux.UploadPanel(upPanelConfig);
				for (var l in upPanelConfig.listeners || {}) {
					upPanel.on(l, upPanelConfig.listeners[l],
							upPanelConfig.listeners.scope || this);
				}
				this.upWin = new Ext.Window({
							width : 650,
							title : '附件上传',
							height : 300,
							layout : 'fit',
							closeAction : this.reuses ? 'hide' : 'close',
							// closeAction : 'hide',
							items : [upPanel]
						});
			},
			/**
			 * 设置REFID
			 * 
			 * @param {refid}
			 */
			setREFID : function(refid) {
				this.REFID = refid;
				this.upPanel.postParams.REFID = refid;
				// this.upPanel.gp.getStore().removeAll();
			}
		});
Ext.reg('uploadButton', Ext.ux.UploadButton);

/**
 * @class com.bitc.UploadImgButton 添加图片按钮
 * @extends Ext.Button
 * @cfg {String} EMPID 人员ID
 * @cfg {String} IMGName 显示域的名字
 */
Ext.ux.UploadImgButton = Ext.extend(Ext.Button, {
	text : '添加图片',
	iconCls : 'attatchment',
	reuses : true,
	EMPIDFun : undefined,
	setImgNameFun:function(){
	},
	
	handler : function() {
		if (!this.reuses || !this.upWin) {
			this.initWindow();
		}
		if (!this.upWin) {
			this.initWindow();
		}
		if (this.EMPIDFun) {
			this.setEMPID(this.EMPIDFun.call(this));
		}
		this.upWin.setPosition(250, 100);
		this.upWin.show();
	},
	
	initWindow : function() {
		var a = this.EMPIDFun();
		var mythis = this;
		var isPostCard = this.ISPOSTCARD;
		var imgsName = this.IMGName;

		var upPanel = this.upPanel = new Ext.form.FormPanel({
					baseCls : 'x-plain',
					labelWidth : 70,
					fileUpload : true,
					defaultType : 'textfield',
					items : [{
								xtype : 'textfield',
								autoCreate : {
									tag : "input",
									type : "file",
									contenteditable : false
								},
								fieldLabel : '上传文件名',
								name : 'userfile',
								inputType : 'file',
								anchor : '100%'
							}]
				});
		var win = this.upWin = new Ext.Window({
			title : '图片上传',
			width : 400,
			height : 100,
			minWidth : 300,
			minHeight : 100,
			plain : true,
			bodyStyle : 'padding:5px;',
			buttonAlign : 'center',
			layout : 'fit',
			closeAction : this.reuses ? 'hide' : 'close',
			buttons : [{
				text : '上传',
				handler : function() {
					if (upPanel.getForm().isValid()) {
						if (upPanel.getForm().findField('userfile').getValue() == '') {
							Ext.MessageBox.alert('错误', '请选择你要上传的文件');
							return;
						}
						Ext.MessageBox.wait('正在上传图片, 请稍侯 ...', '提示');
						upPanel.getForm().submit({
							url : __cxtPath+'/fileUpload.do?type=1&userid=' + a
									+ "&isPostCard=" + isPostCard,
							method : 'post',
							success : function(form, action) {
								Ext.Msg.alert(action.result.messageTitle,
										action.result.message);
								imgsName
										.setSrc(

												__cxtPath
														+ "/fileUpload.do?type=7&userPic="
														+ action.result.attributes.picPath
														+ "&t=" + Math.random()
														+ "&isPostCard="
														+ isPostCard);
								mythis.setImgNameFun.call(this, action.result.attributes.picPath);
								win.close();

							},
							failure : function(form, action) {
								Ext.Msg.alert(action.result.messageTitle,
										action.result.message);
							}
						})
					}
				}
			}, {
				text : '关闭',
				handler : function() {
					win.close();
				}
			}],
			items : [upPanel]
		});
	},
	setEMPID : function(empId) {
		this.EMPID = empId;
	}
});
Ext.reg('uploadImgButton', Ext.ux.UploadImgButton);

Ext.ns('Ext.ux');
/**
 * @class Ext.ux.ImgsBoxComponent 头像、名片的面板
 * @extends Ext.BoxComponent
 * @param picName 默认图片的名字
 */
Ext.ux.ImgsBoxComponent = Ext.extend(Ext.BoxComponent, {
	width : '', // 图片宽度
	height : '', // 图片高度
	picName : '020.png',
	initComponent : function() {
		this.autoEl = {
			tag : 'img', // 指定为img标签
			src : __cxtPath + '/resources/images/' + this.picName // 指定url路径

		}, 
		this.setSrc = function(src) {
			this.src = src;
			if (this.el && this.el.dom) {
				this.el.dom.src = src;
			}
		}, 
		this.listeners = {
			afterlayout : function() {
				if (this.src) {
					this.setSrc(this.src);
				}
			}
		}
	}
		// autoEl : {
		// tag : 'img', // 指定为img标签
		// src : __cxtPath + '/resources/images/'+this.picName // 指定url路径
		// },
		// setSrc : function(src) {
		// this.src = src;
		// if (this.el && this.el.dom) {
		// this.el.dom.src = src;
		// }
		// },
		// listeners : {
		// afterlayout : function() {
		// if (this.src) {
		// this.setSrc(this.src);
		// }
		// }
		// }
	});
