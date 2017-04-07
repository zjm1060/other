/*
 * $Id: MainPanel.js,v 1.83 2010/05/01 15:09:59 gaudenz Exp $ Copyright (c)
 * 2006-2010, JGraph Ltd
 */
MainPanel = function(graph, history, propWin) {
	graph.mainPanel = this;

	var executeLayout = function(layout, animate, ignoreChildCount) {
		var cell = graph.getSelectionCell();

		if (cell == null
				|| (!ignoreChildCount && graph.getModel().getChildCount(cell) == 0)) {
			cell = graph.getDefaultParent();
		}

		graph.getModel().beginUpdate();
		try {
			layout.execute(cell);
		} catch (e) {
			throw e;
		} finally {
			// Animates the changes in the graph model except
			// for Camino, where animation is too slow
			if (animate && navigator.userAgent.indexOf('Camino') < 0) {
				// New API for animating graph layout results asynchronously
				var morph = new mxMorphing(graph);
				morph.addListener(mxEvent.DONE, function() {
							graph.getModel().endUpdate();
						});

				morph.startAnimation();
			} else {
				graph.getModel().endUpdate();
			}
		}

	};

	// Simplified file and modified state handling
	this.filename = processName;
	this.modified = false;

	var updateTitle = mxUtils.bind(this, function() {
				this.setTitle(processName);
			});
	this.updateTitle = updateTitle;

	var changeHandler = mxUtils.bind(this, function(sender, evt) {
				this.modified = true;
				updateTitle();
			});

	this.saveDiagram = function(forceDialog) {
		graph.getModel().addListener(mxEvent.CHANGE, changeHandler);

		var root = graph.getModel().getCell(1);
		var tmpNode = "";
		var steps = [];
		var isValid = true;
		var errMsg = "";
		var form = null;
		for (var i = 0; i < root.getChildCount(); i++) {
			tmpNode = root.getChildAt(i);
			if (tmpNode.isEdge() || tmpNode.getStyle().indexOf("end;") == 0)
				continue;
			var formInfo = setupFormInfos.get(tmpNode.id);
			form = null;
			if (formInfo != null) {
				form = {
					id : tmpNode.form,
					formItems : formInfo.items
				}
			}
			steps.push({
						id : tmpNode.id,
						form : form
					})
		}
		if (!isValid) {
			errMsg += "未指定表单,请为每个步骤指定表单！";
			mxUtils.alert(errMsg, 400);
		}
		var name = this.filename;
		var wokfowid = processKey;
		var enc = new mxCodec(mxUtils.createXmlDocument());
		var node = enc.encode(graph.getModel());
		var xml = mxUtils.getXml(node);
		var actionUrl = "console.workflowAction.do?method=saveWorkFlow";
		var myMask = new Ext.LoadMask(Ext.getBody(), {
					msg : "获取表单数据中..."
				});
		myMask.show();
		var requestConfig = {
			url : actionUrl,
			jsonData : {
				name : name,
				id : wokfowid,
				schema : xml,
				steps : steps
			},
			method : 'post',
			callback : function(options, success, response) {
				var result = eval("(" + response.responseText + ")");
				if (result.err == undefined || result.err == null) {
					var text = "";
					if (result.result) {
						text = '流程信息保存成功！';
					}
				} else {
					text = '流程信息保存失败:<br>' + result.err.msg;
				}
				Ext.Msg.alert('提示', text);
				myMask.hide();
			}
		}
		Ext.Ajax.request(requestConfig);
	};

	this.openDiagram = function(name) {
		if (!this.modified || mxUtils.confirm('当前流程设计更改未保存，确认放弃保存吗?')) {
			var mainPanel = this;
			DiagramStore.get(name, function(xml) {
						if (xml != null && xml.length > 0) {
							var doc = mxUtils.parseXml(xml);
							var dec = new mxCodec(doc);
							dec.decode(doc.documentElement, graph.getModel());
							history.clear();
							mainPanel.filename = name;
							mainPanel.modified = false;
							updateTitle();
							mxUtils.alert('Opened "' + name + '": '
									+ xml.length + ' byte(s)');
						}
					});
		}
	};

	this.deploy = function(name) {
		if (this.modified) {
			Ext.Msg.alert('提示', '当前设计未保存，请先保存。');
			return;
		}
		if (check(graph)) {
			DiagramStore.deploy(name);
		}
	}

	this.newDiagram = function() {
		if (!this.modified || mxUtils.confirm('当前流程设计更改未保存，确认放弃保存吗?')) {
			var cell = new mxCell();
			cell.insert(new mxCell());
			graph.getModel().setRoot(cell);
			history.clear();
			this.filename = null;
			this.modified = false;
			updateTitle();
		}
	};

	this.graphPanel = new Ext.Panel({
				region : 'center',
				border : false,
				tbar : [{
							id : 'saveButton',
							text : '',
							iconCls : 'save-icon',
							tooltip : '保存',
							handler : function() {
								this.saveDiagram();
							},
							scope : this
						}, {
							id : 'cut',
							text : '',
							iconCls : 'cut-icon',
							tooltip : '剪切',
							handler : function() {
								mxClipboard.cut(graph);
							},
							scope : this
						}, {
							id : 'copy',
							text : '',
							iconCls : 'copy-icon',
							tooltip : '复制',
							handler : function() {
								mxClipboard.copy(graph);
							},
							scope : this
						}, {
							text : '',
							iconCls : 'paste-icon',
							tooltip : '粘贴',
							handler : function() {
								mxClipboard.paste(graph);
							},
							scope : this
						}, '-', {
							id : 'delete',
							text : '',
							iconCls : 'delete-icon',
							tooltip : '删除',
							handler : function() {
								graph.removeCells();
							},
							scope : this
						}, '-', {
							tooltip : '放大',
							iconCls : 'zoomin-icon',
							scope : this,
							handler : function(item) {
								graph.zoomIn();
							}
						}, {
							tooltip : '缩小',
							iconCls : 'zoomout-icon',
							scope : this,
							handler : function(item) {
								graph.zoomOut();
							}
						}, '-', {
							id : 'showxml',
							text : '显示XML',
							tooltip : '显示XML',
							handler : function(item) {
								var enc = new mxCodec(mxUtils
										.createXmlDocument());
								var node = enc.encode(graph.getModel());

								mxUtils.popup(mxUtils.getPrettyXml(node));
							},
							scope : this
						}, {
							id : 'parsexml',
							text : '解析XML',
							tooltip : '解析XML',
							handler : function(item) {
								Ext.MessageBox.prompt('Enter XML:', '',
										function(btn, xml) {
											if (btn == 'ok' && xml != null
													&& xml.length > 0) {
												var doc = mxUtils.parseXml(xml);
												var dec = new mxCodec(doc);
												dec.decode(doc.documentElement,
														graph.getModel());
											}
										}, null, true);
							},
							scope : this
						}],

				onContextMenu : function(node, e) {
					var selected = !graph.isSelectionEmpty();

					this.menu = new Ext.menu.Menu({
								items : [{
											text : '剪切',
											iconCls : 'cut-icon',
											disabled : !selected,
											scope : this,
											handler : function() {
												mxClipboard.cut(graph);
											}
										}, {
											text : '复制',
											iconCls : 'copy-icon',
											disabled : !selected,
											scope : this,
											handler : function() {
												mxClipboard.copy(graph);
											}
										}, {
											text : '粘贴',
											iconCls : 'paste-icon',
											disabled : mxClipboard.isEmpty(),
											scope : this,
											handler : function() {
												mxClipboard.paste(graph);
											}
										}, '-', {
											text : '删除',
											iconCls : 'delete-icon',
											disabled : !selected,
											scope : this,
											handler : function() {
												graph.removeCells();
											}
										}, '-', {
											text : '编辑',
											scope : this,
											handler : function() {
												graph.startEditing();
											}
										}, '-', {
											text : '选择所有节点',
											scope : this,
											handler : function() {
												graph.selectVertices();
											}
										}, {
											text : '选择所有路径',
											scope : this,
											handler : function() {
												graph.selectEdges();
											}
										}, '-', {
											text : '选择所有',
											scope : this,
											handler : function() {
												graph.selectAll();
											}
										}, '-', {
											text : '属性',
											scope : this,
											disabled : !selected,
											handler : function() {
												propWin.showProp(graph);
											}
										}]
							});

					this.menu.on('hide', this.onContextHide, this);
					this.menu.showAt([e.clientX, e.clientY]);
				},

				onContextHide : function() {
					if (this.ctxNode) {
						this.ctxNode.ui.removeClass('x-node-ctx');
						this.ctxNode = null;
					}
				}
			});

	MainPanel.superclass.constructor.call(this, {
				region : 'center',
				layout : 'fit',
				items : this.graphPanel
			});

	// Redirects the context menu to ExtJs menus
	graph.panningHandler.popup = mxUtils.bind(this, function(x, y, cell, evt) {
				this.graphPanel.onContextMenu(null, evt);
			});

	graph.panningHandler.hideMenu = mxUtils.bind(this, function() {
				if (this.graphPanel.menuPanel != null) {
					this.graphPanel.menuPanel.hide();
				}
			});

	// Fits the SVG container into the panel body
	this.graphPanel.on('resize', function() {
				graph.sizeDidChange();
			});
};

Ext.extend(MainPanel, Ext.Panel);
