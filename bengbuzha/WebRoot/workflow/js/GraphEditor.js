GraphEditor = {};

function main() {
	Ext.QuickTips.init();
	// Disables browser context menu
	mxEvent.disableContextMenu(document.body);

	// Makes the connection are smaller
	mxConstants.DEFAULT_HOTSPOT = 0.3;

	// Creates the graph and loads the default stylesheet
	var graph = new ysGraph();
	GraphEditor.graph = graph;
	// Inverts the elbow edge style without removing existing styles
	graph.flipEdge = function(edge) {
		if (edge != null) {
			var state = this.view.getState(edge);
			var style = (state != null) ? state.style : this.getCellStyle(edge);

			if (style != null) {
				var elbow = mxUtils.getValue(style, mxConstants.STYLE_ELBOW,
						mxConstants.ELBOW_HORIZONTAL);
				var value = (elbow == mxConstants.ELBOW_HORIZONTAL)
						? mxConstants.ELBOW_VERTICAL
						: mxConstants.ELBOW_HORIZONTAL;
				this.setCellStyles(mxConstants.STYLE_ELBOW, value, [edge]);
			}
		}
	};

	// Creates the command history (undo/redo)
	var history = new mxUndoManager();

	// Loads the default stylesheet into the graph

	var node = mxUtils.load(basePath + 'workflow/resources/default-style.xml')
			.getDocumentElement();

	var dec = new mxCodec(node.ownerDocument);
	dec.decode(node, graph.getStylesheet());

	graph.alternateEdgeStyle = 'vertical';

	// Creates the main containers
	var propWin = new PropWin();
	var mainPanel = new MainPanel(graph, history, propWin);
	var library = new LibraryPanel();

	var store = new Ext.data.ArrayStore({
				fields : ['name']
			});

	var updateHandler = function() {
		DiagramStore.getNames(store);
	};

	DiagramStore.addListener('put', updateHandler);
	DiagramStore.addListener('remove', updateHandler);
	updateHandler();

	var diagramPanel = new DiagramPanel(store, mainPanel);

	diagramPanel.on('dblclick', function(view, index, node, e) {
				var name = store.getAt(index).get('name');
				mainPanel.openDiagram(name);
			});

	// Creates the container for the outline
	var outlinePanel = new Ext.Panel({
				id : 'outlinePanel',
				layout : 'fit',
				split : true,
				height : 200,
				region : 'south'
			});

	// Creates the enclosing viewport
	var viewport = new Ext.Viewport({
		layout : 'border',
		items : [{
			xtype : 'panel',
			margins : '5 5 5 5',
			region : 'center',
			layout : 'border',
			border : false,
			items : [new Ext.Panel({
								region : 'west',
								title : '工具栏',
								layout : 'border',
								collapsible : true,
								split : true,
								width : 180,
								items : [library, outlinePanel]
							}), mainPanel]
		}		// end master panel
		]
			// end viewport items
		}); // end of new Viewport

	// Enables scrollbars for the graph container to make it more
	// native looking, this will affect the panning to use the
	// scrollbars rather than moving the container contents inline
	mainPanel.graphPanel.body.dom.style.overflow = 'auto';

	// Installs the command history after the initial graph
	// has been created
	var listener = function(sender, evt) {
		history.undoableEditHappened(evt.getProperty('edit'));
	};

	graph.getModel().addListener(mxEvent.UNDO, listener);
	graph.getView().addListener(mxEvent.UNDO, listener);

	// Keeps the selection in sync with the history
	var undoHandler = function(sender, evt) {
		var changes = evt.getProperty('edit').changes;
		graph.setSelectionCells(graph.getSelectionCellsForChanges(changes));
	};

	history.addListener(mxEvent.UNDO, undoHandler);
	history.addListener(mxEvent.REDO, undoHandler);

	// Initializes the graph as the DOM for the panel has now been created
	graph.init(mainPanel.graphPanel.body.dom);
	graph.setConnectable(true);
	graph.setDropEnabled(true);
	graph.setPanning(true);
	graph.setTooltips(true);
	graph.connectionHandler.setCreateTarget(false);

	// Sets the cursor
	graph.container.style.cursor = 'default';

	// Creates rubberband selection
	var rubberband = new mxRubberband(graph);

	// Adds some example cells into the graph
	mainPanel.newDiagram();
	var parent = graph.getDefaultParent();

	// Toolbar object for updating buttons in listeners
	var toolbarItems = mainPanel.graphPanel.getTopToolbar().items;

	// Updates the states of all buttons that require a selection
	var selectionListener = function() {
		var selected = !graph.isSelectionEmpty();

		toolbarItems.get('cut').setDisabled(!selected);
		toolbarItems.get('copy').setDisabled(!selected);
		toolbarItems.get('delete').setDisabled(!selected);

		if (selected && !propWin.hidden) {
			propWin.showProp(graph);
		}
	};

	graph.getSelectionModel().addListener(mxEvent.CHANGE, selectionListener);

	// Updates the states of the undo/redo buttons in the toolbar
	var historyListener = function() {
	};

	history.addListener(mxEvent.ADD, historyListener);
	history.addListener(mxEvent.UNDO, historyListener);
	history.addListener(mxEvent.REDO, historyListener);

	// Updates the button states once
	selectionListener();
	historyListener();
	var root = graph.getModel().getCell(1);
	root.value = processKey;
	root.name = processName;

	// Installs outline in outlinePanel
	var outline = new mxOutline(graph, outlinePanel.body.dom);
	outlinePanel.body.dom.style.cursor = 'move';

	// Adds the entries into the library
	insertVertexTemplate(library, graph, '开始', 'images/start.png',
			'start;image;image=images/start.png', 38, 38, "开始");
	insertVertexTemplate(library, graph, '结束', 'images/end.png',
			'end;image;image=images/end.png', 38, 38, "结束");
	insertVertexTemplate(library, graph, '分支', 'images/fork.png',
			'fork;image;image=images/fork.png', 38, 38, "分支");
	insertVertexTemplate(library, graph, '合并', 'images/join.png',
			'join;image;image=images/join.png', 38, 38, "合并");
	insertVertexTemplate(library, graph, '任务', 'images/task.png',
			'step;image;image=images/task.png', 58, 38, "任务");

	insertVertexTemplate(library, graph, '状态', 'images/swimlane.gif',
			'swimlane', 200, 200, '状态');
	var previousCreateGroupCell = graph.createGroupCell;

	graph.createGroupCell = function() {
		var group = previousCreateGroupCell.apply(this, arguments);
		group.setStyle('group');

		return group;
	};

	graph.connectionHandler.factoryMethod = function() {
		if (GraphEditor.edgeTemplate != null) {
			return graph.cloneCells([GraphEditor.edgeTemplate])[0];
		}

		return null;
	};

	// Uses the selected edge in the library as a template for new edges
	library.getSelectionModel().on('selectionchange', function(sm, node) {
				if (node != null && node.attributes.cells != null) {
					var cell = node.attributes.cells[0];

					if (cell != null && graph.getModel().isEdge(cell)) {
						GraphEditor.edgeTemplate = cell;
					}
				}
			});

	// Redirects tooltips to ExtJs tooltips. First a tooltip object
	// is created that will act as the tooltip for all cells.
	var tooltip = new Ext.ToolTip({
				target : graph.container,
				html : ''
			});

	// Disables the built-in event handling
	tooltip.disabled = true;

	// Installs the tooltip by overriding the hooks in mxGraph to
	// show and hide the tooltip.
	graph.tooltipHandler.show = function(tip, x, y) {
		if (tip != null && tip.length > 0) {
			// Changes the DOM of the tooltip in-place if
			// it has already been rendered
			if (tooltip.body != null) {
				tooltip.body.dom.firstChild.nodeValue = tip;
			}

			// Changes the html config value if the tooltip
			// has not yet been rendered, in which case it
			// has no DOM nodes associated
			else {
				tooltip.html = tip;
			}

			tooltip.showAt([x, y + mxConstants.TOOLTIP_VERTICAL_OFFSET]);
		}
	};

	graph.tooltipHandler.hide = function() {
		tooltip.hide();
	};

	// Updates the document title if the current root changes (drilling)
	var drillHandler = function(sender) {
		var model = graph.getModel();
		var cell = graph.getCurrentRoot();
		var title = '';

		while (cell != null && model.getParent(model.getParent(cell)) != null) {
			// Append each label of a valid root
			if (graph.isValidRoot(cell)) {
				title = ' > ' + graph.convertValueToString(cell) + title;
			}

			cell = graph.getModel().getParent(cell);
		}
	};

	graph.getView().addListener(mxEvent.DOWN, drillHandler);
	graph.getView().addListener(mxEvent.UP, drillHandler);

	// Transfer initial focus to graph container for keystroke handling
	graph.container.focus();

	// Handles keystroke events
	var keyHandler = new mxKeyHandler(graph);

	// Ignores enter keystroke. Remove this line if you want the
	// enter keystroke to stop editing
	keyHandler.enter = function() {
	};

	keyHandler.bindKey(8, function() {
				graph.foldCells(true);
			});

	keyHandler.bindKey(13, function() {
				graph.foldCells(false);
			});

	keyHandler.bindKey(33, function() {
				graph.exitGroup();
			});

	keyHandler.bindKey(34, function() {
				graph.enterGroup();
			});

	keyHandler.bindKey(36, function() {
				graph.home();
			});

	keyHandler.bindKey(35, function() {
				graph.refresh();
			});

	keyHandler.bindKey(37, function() {
				graph.selectPreviousCell();
			});

	keyHandler.bindKey(38, function() {
				graph.selectParentCell();
			});

	keyHandler.bindKey(39, function() {
				graph.selectNextCell();
			});

	keyHandler.bindKey(40, function() {
				graph.selectChildCell();
			});

	keyHandler.bindKey(46, function() {
				graph.removeCells();
			});

	keyHandler.bindKey(107, function() {
				graph.zoomIn();
			});

	keyHandler.bindKey(109, function() {
				graph.zoomOut();
			});

	keyHandler.bindKey(113, function() {
				graph.startEditingAtCell();
			});

	keyHandler.bindControlKey(65, function() {
				graph.selectAll();
			});

	keyHandler.bindControlKey(89, function() {
				history.redo();
			});

	keyHandler.bindControlKey(90, function() {
				history.undo();
			});

	keyHandler.bindControlKey(88, function() {
				mxClipboard.cut(graph);
			});

	keyHandler.bindControlKey(67, function() {
				mxClipboard.copy(graph);
			});

	keyHandler.bindControlKey(86, function() {
				mxClipboard.paste(graph);
			});

	keyHandler.bindControlKey(71, function() {
				graph.setSelectionCell(graph.groupCells(null, 20));
			});

	keyHandler.bindControlKey(85, function() {
				graph.setSelectionCells(graph.ungroupCells());
			});
}; // end of main

GraphEditor.initData = function() {
	if (_graphXml != null && _graphXml.length > 0) {
		var doc = mxUtils.parseXml(_graphXml);
		var dec = new mxCodec(doc);
		dec.decode(doc.documentElement, this.graph.getModel());
	}
};

function insertSymbolTemplate(panel, graph, name, icon, rhombus) {
	var imagesNode = panel.symbols;
	var style = (rhombus) ? 'rhombusImage' : 'roundImage';
	return insertVertexTemplate(panel, graph, name, icon, style + ';image='
					+ icon, 50, 50, '', imagesNode);
};

function insertImageTemplate(panel, graph, name, icon, round) {
	var imagesNode = panel.images;
	var style = (round) ? 'roundImage' : 'image';
	return insertVertexTemplate(panel, graph, name, icon, style + ';image='
					+ icon, 50, 50, name, imagesNode);
};

function insertVertexTemplate(panel, graph, name, icon, style, width, height,
		value, parentNode) {
	var cells = [new mxCell((value != null) ? value : '', new mxGeometry(0, 0,
					width, height), style)];
	cells[0].vertex = true;

	var funct = function(graph, evt, target) {
		cells = graph.getImportableCells(cells);

		if (cells.length > 0) {
			var validDropTarget = (target != null) ? graph.isValidDropTarget(
					target, cells, evt) : false;
			var select = null;

			if (target != null && !validDropTarget
					&& graph.getModel().getChildCount(target) == 0
					&& graph.getModel().isVertex(target) == cells[0].vertex) {
				graph.getModel().setStyle(target, style);
				select = [target];
			} else {
				if (target != null && !validDropTarget) {
					target = null;
				}

				var pt = graph.getPointForEvent(evt);

				// Splits the target edge or inserts into target group
				if (graph.isSplitEnabled()
						&& graph.isSplitTarget(target, cells, evt)) {
					graph.splitEdge(target, cells, null, pt.x, pt.y);
					select = cells;
				} else {
					cells = graph.getImportableCells(cells);

					if (cells.length > 0) {
						select = graph.importCells(cells, pt.x, pt.y, target);
					}
				}
			}

			if (select != null && select.length > 0) {
				graph.scrollCellToVisible(select[0]);
				graph.setSelectionCells(select);
			}
		}
	};

	// Small hack to install the drag listener on the node's DOM element
	// after it has been created. The DOM node does not exist if the parent
	// is not expanded.
	var node = panel.addTemplate(name, icon, parentNode, cells);
	var installDrag = function(expandedNode) {
		if (node.ui.elNode != null) {
			// Creates the element that is being shown while the drag is in
			// progress
			var dragPreview = document.createElement('div');
			dragPreview.style.border = 'dashed black 1px';
			dragPreview.style.width = width + 'px';
			dragPreview.style.height = height + 'px';

			mxUtils.makeDraggable(node.ui.elNode, graph, funct, dragPreview, 0,
					0, graph.autoscroll, true);
		}
	};

	if (!node.parentNode.isExpanded()) {
		panel.on('expandnode', installDrag);
	} else {
		installDrag(node.parentNode);
	}

	return node;
};

function insertEdgeTemplate(panel, graph, name, icon, style, width, height,
		value, parentNode) {
	var cells = [new mxCell((value != null) ? value : '', new mxGeometry(0, 0,
					width, height), style)];
	cells[0].geometry.setTerminalPoint(new mxPoint(0, height), true);
	cells[0].geometry.setTerminalPoint(new mxPoint(width, 0), false);
	cells[0].edge = true;

	var funct = function(graph, evt, target) {
		cells = graph.getImportableCells(cells);

		if (cells.length > 0) {
			var validDropTarget = (target != null) ? graph.isValidDropTarget(
					target, cells, evt) : false;
			var select = null;

			if (target != null && !validDropTarget) {
				target = null;
			}

			var pt = graph.getPointForEvent(evt);
			var scale = graph.view.scale;

			pt.x -= graph.snap(width / 2);
			pt.y -= graph.snap(height / 2);

			select = graph.importCells(cells, pt.x, pt.y, target);

			// Uses this new cell as a template for all new edges
			GraphEditor.edgeTemplate = select[0];

			graph.scrollCellToVisible(select[0]);
			graph.setSelectionCells(select);
		}
	};

	// Small hack to install the drag listener on the node's DOM element
	// after it has been created. The DOM node does not exist if the parent
	// is not expanded.
	var node = panel.addTemplate(name, icon, parentNode, cells);
	var installDrag = function(expandedNode) {
		if (node.ui.elNode != null) {
			// Creates the element that is being shown while the drag is in
			// progress
			var dragPreview = document.createElement('div');
			dragPreview.style.border = 'dashed black 1px';
			dragPreview.style.width = width + 'px';
			dragPreview.style.height = height + 'px';

			mxUtils.makeDraggable(node.ui.elNode, graph, funct, dragPreview,
					-width / 2, -height / 2, graph.autoscroll, true);
		}
	};

	if (!node.parentNode.isExpanded()) {
		panel.on('expandnode', installDrag);
	} else {
		installDrag(node.parentNode);
	}

	return node;
};

// Defines a global functionality for displaying short information messages
Ext.example = function() {
	var msgCt;

	function createBox(t, s) {
		return [
				'<div class="msg">',
				'<div class="x-box-tl"><div class="x-box-tr"><div class="x-box-tc"></div></div></div>',
				'<div class="x-box-ml"><div class="x-box-mr"><div class="x-box-mc"><h3>',
				t,
				'</h3>',
				s,
				'</div></div></div>',
				'<div class="x-box-bl"><div class="x-box-br"><div class="x-box-bc"></div></div></div>',
				'</div>'].join('');
	}
	return {
		msg : function(title, format) {
			if (!msgCt) {
				msgCt = Ext.DomHelper.append(document.body, {
							id : 'msg-div'
						}, true);
			}
			msgCt.alignTo(document, 't-t');
			var s = String.format.apply(String, Array.prototype.slice.call(
							arguments, 1));
			var m = Ext.DomHelper.append(msgCt, {
						html : createBox(title, s)
					}, true);
			m.slideIn('t').pause(1).ghost("t", {
						remove : true
					});
		}
	};
}();
