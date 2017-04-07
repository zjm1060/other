/*
 * ! Ext JS Library 3.2.1 Copyright(c) 2006-2010 Ext JS, Inc.
 * licensing@extjs.com http://www.extjs.com/license
 */
Ext.ns('Ext.ux.form');

/**
 * @class Ext.ux.form.MultiSelect
 * @extends Ext.form.Field A control that allows selection and form submission
 *          of multiple list items.
 * 
 * @history 2008-06-19 bpm Original code contributed by Toby Stuart (with
 *          contributions from Robert Williams) 2008-06-19 bpm Docs and demo
 *          code clean up
 * 
 * @constructor Create a new MultiSelect
 * @param {Object}
 *            config Configuration options
 * @xtype multiselect
 */
Ext.ux.form.MultiSelect = Ext.extend(Ext.form.Field, {
	/**
	 * @cfg {String} legend Wraps the object with a fieldset and specified
	 *      legend.
	 */
	/**
	 * @cfg {Ext.ListView} view The {@link Ext.ListView} used to render the
	 *      multiselect list.
	 */
	/**
	 * @cfg {String/Array} dragGroup The ddgroup name(s) for the MultiSelect
	 *      DragZone (defaults to undefined).
	 */
	/**
	 * @cfg {String/Array} dropGroup The ddgroup name(s) for the MultiSelect
	 *      DropZone (defaults to undefined).
	 */
	/**
	 * @cfg {Boolean} ddReorder Whether the items in the MultiSelect list are
	 *      drag/drop reorderable (defaults to false).
	 */
	ddReorder : false,
	/**
	 * @cfg {Object/Array} tbar The top toolbar of the control. This can be a
	 *      {@link Ext.Toolbar} object, a toolbar config, or an array of
	 *      buttons/button configs to be added to the toolbar.
	 */
	/**
	 * @cfg {String} appendOnly True if the list should only allow append drops
	 *      when drag/drop is enabled (use for lists which are sorted, defaults
	 *      to false).
	 */
	appendOnly : false,
	/**
	 * @cfg {Number} width Width in pixels of the control (defaults to 100).
	 */
	width : 100,
	/**
	 * @cfg {Number} height Height in pixels of the control (defaults to 100).
	 */
	height : 100,
	/**
	 * @cfg {String/Number} displayField Name/Index of the desired display field
	 *      in the dataset (defaults to 0).
	 */
	displayField : 0,
	/**
	 * @cfg {String/Number} valueField Name/Index of the desired value field in
	 *      the dataset (defaults to 1).
	 */
	valueField : 1,
	/**
	 * @cfg {Boolean} allowBlank False to require at least one item in the list
	 *      to be selected, true to allow no selection (defaults to true).
	 */
	allowBlank : true,
	/**
	 * @cfg {Number} minSelections Minimum number of selections allowed
	 *      (defaults to 0).
	 */
	minSelections : 0,
	/**
	 * @cfg {Number} maxSelections Maximum number of selections allowed
	 *      (defaults to Number.MAX_VALUE).
	 */
	maxSelections : Number.MAX_VALUE,
	/**
	 * @cfg {String} blankText Default text displayed when the control contains
	 *      no items (defaults to the same value as
	 *      {@link Ext.form.TextField#blankText}.
	 */
	blankText : Ext.form.TextField.prototype.blankText,
	/**
	 * @cfg {String} minSelectionsText Validation message displayed when
	 *      {@link #minSelections} is not met (defaults to 'Minimum {0} item(s)
	 *      required'). The {0} token will be replaced by the value of
	 *      {@link #minSelections}.
	 */
	minSelectionsText : 'Minimum {0} item(s) required',
	/**
	 * @cfg {String} maxSelectionsText Validation message displayed when
	 *      {@link #maxSelections} is not met (defaults to 'Maximum {0} item(s)
	 *      allowed'). The {0} token will be replaced by the value of
	 *      {@link #maxSelections}.
	 */
	maxSelectionsText : 'Maximum {0} item(s) allowed',
	/**
	 * @cfg {String} delimiter The string used to delimit between items when set
	 *      or returned as a string of values (defaults to ',').
	 */
	delimiter : ',',
	/**
	 * @cfg {Ext.data.Store/Array} store The data source to which this
	 *      MultiSelect is bound (defaults to <tt>undefined</tt>). Acceptable
	 *      values for this property are: <div class="mdetail-params">
	 *      <ul>
	 *      <li><b>any {@link Ext.data.Store Store} subclass</b></li>
	 *      <li><b>an Array</b> : Arrays will be converted to a
	 *      {@link Ext.data.ArrayStore} internally. <div class="mdetail-params">
	 *      <ul>
	 *      <li><b>1-dimensional array</b> : (e.g., <tt>['Foo','Bar']</tt>)<div
	 *      class="sub-desc"> A 1-dimensional array will automatically be
	 *      expanded (each array item will be the combo
	 *      {@link #valueField value} and {@link #displayField text})</div></li>
	 *      <li><b>2-dimensional array</b> : (e.g.,
	 *      <tt>[['f','Foo'],['b','Bar']]</tt>)<div class="sub-desc"> For a
	 *      multi-dimensional array, the value in index 0 of each item will be
	 *      assumed to be the combo {@link #valueField value}, while the value
	 *      at index 1 is assumed to be the combo {@link #displayField text}.
	 *      </div></li>
	 *      </ul>
	 *      </div></li>
	 *      </ul>
	 *      </div>
	 */

	/**
	 * @cfg {String} defaultValue the defaultValue
	 */
	defaultValue : '',
	defaultName : '',
	// private
	defaultAutoCreate : {
		tag : "div"
	},

	// private
	initComponent : function() {
		Ext.ux.form.MultiSelect.superclass.initComponent.call(this);

		if (Ext.isArray(this.store)) {
			if (Ext.isArray(this.store[0])) {
				this.store = new Ext.data.ArrayStore({
							fields : ['value', 'text'],
							data : this.store
						});
				this.valueField = 'value';
			} else {
				this.store = new Ext.data.ArrayStore({
							fields : ['text'],
							data : this.store,
							expandData : true
						});
				this.valueField = 'text';
			}
			this.displayField = 'text';
		} else {
			this.store = Ext.StoreMgr.lookup(this.store);
		}

		this.addEvents({
					'dblclick' : true,
					'click' : true,
					'change' : true,
					'drop' : true
				});
	},

	// private
	onRender : function(ct, position) {
		Ext.ux.form.MultiSelect.superclass.onRender.call(this, ct, position);

		var fs = this.fs = new Ext.form.FieldSet({
					renderTo : this.el,
					title : this.legend,
					height : this.height - 20,
					width : this.width,
					style : "padding:0;",
					tbar : this.tbar
				});
		fs.body.addClass('ux-mselect');

		this.view = new Ext.ListView({
					multiSelect : true,
					store : this.store,
					columns : [{
								header : 'Value',
								width : 1,
								dataIndex : this.displayField
							}],
					hideHeaders : true
				});

		fs.add(this.view);

		this.view.on('click', this.onViewClick, this);
		this.view.on('beforeclick', this.onViewBeforeClick, this);
		this.view.on('dblclick', this.onViewDblClick, this);

		this.hiddenName = this.name || Ext.id();
		var hiddenTag = {
			tag : "input",
			type : "hidden",
			value : "",
			name : this.hiddenName
		};
		this.hiddenField = this.el.createChild(hiddenTag);
		this.hiddenField.dom.disabled = this.hiddenName != this.name;
		fs.doLayout();
		/***********************************************************************
		 * 点开默认选择第一个条数据 defaultValue 默认下一个store的第一条数据的id
		 **********************************************************************/

		function isInRecords(defaultValue, records) {
			for (var j = 0; j < records.length; j++) {
				if (defaultValue == records[j].data.id) {
					return true;
				}
			}
			return false;
		}
		this.store.on('load', function(store, records) {
					if (records.length > 0) {
						var value = this.getValue();
						if (Ext.isEmpty(value)) {
							this.selectValues(new String(records[0].data.id));
							this.fireEvent('change', this,
									new String(records[0].data.id), null);
						} else {
							if (isInRecords(value, records)) {
								// this.setValue(this.defaultValue);
								this.selectValues(value);
							} else {
								this
										.selectValues(new String(records[0].data.id));
								this.fireEvent('change', this,
										new String(records[0].data.id), null);
							}
						}

					} else {
						this.setValue('');
					}
				}, this);
	},

	// private
	afterRender : function() {
		Ext.ux.form.MultiSelect.superclass.afterRender.call(this);

		if (this.ddReorder && !this.dragGroup && !this.dropGroup) {
			this.dragGroup = this.dropGroup = 'MultiselectDD-' + Ext.id();
		}

		if (this.draggable || this.dragGroup) {
			this.dragZone = new Ext.ux.form.MultiSelect.DragZone(this, {
						ddGroup : this.dragGroup
					});
		}
		if (this.droppable || this.dropGroup) {
			this.dropZone = new Ext.ux.form.MultiSelect.DropZone(this, {
						ddGroup : this.dropGroup
					});
		}
	},

	// private
	onViewClick : function(vw, index, node, e) {
		var value;
		var returnArray = [];
		var selectionsArray = this.view.getSelectedIndexes();
		if (selectionsArray.length == 0) {
			value = '';
		}
		for (var i = 0; i < selectionsArray.length; i++) {
			returnArray.push(this.store.getAt(selectionsArray[i])
					.get(this.valueField));
		}
		value = returnArray.join(this.delimiter);
		this.fireEvent('change', this, value, this.value);
		this.value = value;
		this.hiddenField.dom.value = value;
		this.fireEvent('click', this, e);
		this.validate();
	},

	// private
	onViewBeforeClick : function(vw, index, node, e) {
		if (this.disabled || this.readOnly) {
			return false;
		}
	},

	// private
	onViewDblClick : function(vw, index, node, e) {
		return this.fireEvent('dblclick', vw, index, node, e);
	},

	/**
	 * Returns an array of data values for the selected items in the list. The
	 * values will be separated by {@link #delimiter}.
	 * 
	 * @return {Array} value An array of string data values
	 */
	getValue : function(valueField) {
		// var returnArray = [];
		// var selectionsArray = this.view.getSelectedIndexes();
		// if (selectionsArray.length == 0) {
		// return '';
		// }
		// for (var i = 0; i < selectionsArray.length; i++) {
		// returnArray.push(this.store.getAt(selectionsArray[i])
		// .get((valueField != null)
		// ? valueField
		// : this.valueField));
		// }
		// return returnArray.join(this.delimiter);
		return this.value;
	},

	// private
	selectValues : function(values) {
		if (!this.view) {
			return;
		}
		var selections = [];
		this.view.clearSelections();
		this.hiddenField.dom.value = '';

		if (!Ext.isArray(values)) {
			values = values.split(this.delimiter);
		}
		for (var i = 0; i < values.length; i++) {
			index = this.view.store.indexOf(this.view.store.query(
					this.valueField, new RegExp('^' + values[i] + '$', "i"))
					.itemAt(0));
			selections.push(index);
		}
		this.view.select(selections);
		this.hiddenField.dom.value = values.join(this.delimiter);
		this.value = values.join(this.delimiter);
	},

	/**
	 * Sets a delimited string (using {@link #delimiter}) or array of data
	 * values into the list.
	 * 
	 * @param {String/Array}
	 *            values The values to set
	 */
	setValue : function(values) {
		var index;
		// -------------
		var oldvalue = this.value;
		this.value = values;

		this.selectValues(values);
		/**
		 * this.fireEvent('change', this, values,this.hiddenField.dom.value);
		 * 原this.getValue() 改为values，setValue()可以出发change 事件
		 */
		if (oldvalue != values) {
			this.fireEvent('change', this, values, oldvalue);
		}
		this.validate();
	},

	// inherit docs
	reset : function() {
		this.setValue('');
	},

	// inherit docs
	getRawValue : function(valueField) {
		var tmp = this.getValue(valueField);
		if (tmp.length) {
			tmp = tmp.split(this.delimiter);
		} else {
			tmp = [];
		}
		return tmp;
	},

	// inherit docs
	setRawValue : function(values) {
		setValue(values);
	},

	// inherit docs
	validateValue : function(value) {
		if (value.length < 1) { // if it has no value
			if (this.allowBlank) {
				this.clearInvalid();
				return true;
			} else {
				this.markInvalid(this.blankText);
				return false;
			}
		}
		if (value.length < this.minSelections) {
			this.markInvalid(String.format(this.minSelectionsText,
					this.minSelections));
			return false;
		}
		if (value.length > this.maxSelections) {
			this.markInvalid(String.format(this.maxSelectionsText,
					this.maxSelections));
			return false;
		}
		return true;
	},

	// inherit docs
	disable : function() {
		this.disabled = true;
		this.hiddenField.dom.disabled = true;
		this.fs.disable();
	},

	// inherit docs
	enable : function() {
		this.disabled = false;
		this.hiddenField.dom.disabled = false;
		this.fs.enable();
	},

	// inherit docs
	destroy : function() {
		Ext.destroy(this.fs, this.dragZone, this.dropZone);
		Ext.ux.form.MultiSelect.superclass.destroy.call(this);
	}
});

Ext.reg('multiselect', Ext.ux.form.MultiSelect);

// backwards compat
Ext.ux.Multiselect = Ext.ux.form.MultiSelect;

Ext.ux.form.MultiSelect.DragZone = function(ms, config) {
	this.ms = ms;
	this.view = ms.view;
	var ddGroup = config.ddGroup || 'MultiselectDD';
	var dd;
	if (Ext.isArray(ddGroup)) {
		dd = ddGroup.shift();
	} else {
		dd = ddGroup;
		ddGroup = null;
	}
	Ext.ux.form.MultiSelect.DragZone.superclass.constructor.call(this,
			this.ms.fs.body, {
				containerScroll : true,
				ddGroup : dd
			});
	this.setDraggable(ddGroup);
};

Ext.extend(Ext.ux.form.MultiSelect.DragZone, Ext.dd.DragZone, {
			onInitDrag : function(x, y) {
				var el = Ext.get(this.dragData.ddel.cloneNode(true));
				this.proxy.update(el.dom);
				el.setWidth(el.child('em').getWidth());
				this.onStartDrag(x, y);
				return true;
			},

			// private
			collectSelection : function(data) {
				data.repairXY = Ext.fly(this.view.getSelectedNodes()[0])
						.getXY();
				var i = 0;
				this.view.store.each(function(rec) {
							if (this.view.isSelected(i)) {
								var n = this.view.getNode(i);
								var dragNode = n.cloneNode(true);
								dragNode.id = Ext.id();
								data.ddel.appendChild(dragNode);
								data.records.push(this.view.store.getAt(i));
								data.viewNodes.push(n);
							}
							i++;
						}, this);
			},

			// override
			onEndDrag : function(data, e) {
				var d = Ext.get(this.dragData.ddel);
				if (d && d.hasClass("multi-proxy")) {
					d.remove();
				}
			},

			// override
			getDragData : function(e) {
				var target = this.view.findItemFromChild(e.getTarget());
				if (target) {
					if (!this.view.isSelected(target) && !e.ctrlKey
							&& !e.shiftKey) {
						this.view.select(target);
						this.ms.setValue(this.ms.getValue());
					}
					if (this.view.getSelectionCount() == 0 || e.ctrlKey
							|| e.shiftKey)
						return false;
					var dragData = {
						sourceView : this.view,
						viewNodes : [],
						records : []
					};
					if (this.view.getSelectionCount() == 1) {
						var i = this.view.getSelectedIndexes()[0];
						var n = this.view.getNode(i);
						dragData.viewNodes.push(dragData.ddel = n);
						dragData.records.push(this.view.store.getAt(i));
						dragData.repairXY = Ext.fly(n).getXY();
					} else {
						dragData.ddel = document.createElement('div');
						dragData.ddel.className = 'multi-proxy';
						this.collectSelection(dragData);
					}
					return dragData;
				}
				return false;
			},

			// override the default repairXY.
			getRepairXY : function(e) {
				return this.dragData.repairXY;
			},

			// private
			setDraggable : function(ddGroup) {
				if (!ddGroup)
					return;
				if (Ext.isArray(ddGroup)) {
					Ext.each(ddGroup, this.setDraggable, this);
					return;
				}
				this.addToGroup(ddGroup);
			}
		});

Ext.ux.form.MultiSelect.DropZone = function(ms, config) {
	this.ms = ms;
	this.view = ms.view;
	var ddGroup = config.ddGroup || 'MultiselectDD';
	var dd;
	if (Ext.isArray(ddGroup)) {
		dd = ddGroup.shift();
	} else {
		dd = ddGroup;
		ddGroup = null;
	}
	Ext.ux.form.MultiSelect.DropZone.superclass.constructor.call(this,
			this.ms.fs.body, {
				containerScroll : true,
				ddGroup : dd
			});
	this.setDroppable(ddGroup);
};

Ext.extend(Ext.ux.form.MultiSelect.DropZone, Ext.dd.DropZone, {
			/**
			 * Part of the Ext.dd.DropZone interface. If no target node is
			 * found, the whole Element becomes the target, and this causes the
			 * drop gesture to append.
			 */
			getTargetFromEvent : function(e) {
				var target = e.getTarget();
				return target;
			},

			// private
			getDropPoint : function(e, n, dd) {
				if (n == this.ms.fs.body.dom) {
					return "below";
				}
				var t = Ext.lib.Dom.getY(n), b = t + n.offsetHeight;
				var c = t + (b - t) / 2;
				var y = Ext.lib.Event.getPageY(e);
				if (y <= c) {
					return "above";
				} else {
					return "below";
				}
			},

			// private
			isValidDropPoint : function(pt, n, data) {
				if (!data.viewNodes || (data.viewNodes.length != 1)) {
					return true;
				}
				var d = data.viewNodes[0];
				if (d == n) {
					return false;
				}
				if ((pt == "below") && (n.nextSibling == d)) {
					return false;
				}
				if ((pt == "above") && (n.previousSibling == d)) {
					return false;
				}
				return true;
			},

			// override
			onNodeEnter : function(n, dd, e, data) {
				return false;
			},

			// override
			onNodeOver : function(n, dd, e, data) {
				var dragElClass = this.dropNotAllowed;
				var pt = this.getDropPoint(e, n, dd);
				if (this.isValidDropPoint(pt, n, data)) {
					if (this.ms.appendOnly) {
						return "x-tree-drop-ok-below";
					}

					// set the insert point style on the target node
					if (pt) {
						var targetElClass;
						if (pt == "above") {
							dragElClass = n.previousSibling
									? "x-tree-drop-ok-between"
									: "x-tree-drop-ok-above";
							targetElClass = "x-view-drag-insert-above";
						} else {
							dragElClass = n.nextSibling
									? "x-tree-drop-ok-between"
									: "x-tree-drop-ok-below";
							targetElClass = "x-view-drag-insert-below";
						}
						if (this.lastInsertClass != targetElClass) {
							Ext.fly(n).replaceClass(this.lastInsertClass,
									targetElClass);
							this.lastInsertClass = targetElClass;
						}
					}
				}
				return dragElClass;
			},

			// private
			onNodeOut : function(n, dd, e, data) {
				this.removeDropIndicators(n);
			},

			// private
			onNodeDrop : function(n, dd, e, data) {
				if (this.ms.fireEvent("drop", this, n, dd, e, data) === false) {
					return false;
				}
				var pt = this.getDropPoint(e, n, dd);
				if (n != this.ms.fs.body.dom)
					n = this.view.findItemFromChild(n);

				if (this.ms.appendOnly) {
					insertAt = this.view.store.getCount();
				} else {
					insertAt = n == this.ms.fs.body.dom ? this.view.store
							.getCount()
							- 1 : this.view.indexOf(n);
					if (pt == "below") {
						insertAt++;
					}
				}

				var dir = false;

				// Validate if dragging within the same MultiSelect
				if (data.sourceView == this.view) {
					// If the first element to be inserted below is the
					// target
					// node, remove it
					if (pt == "below") {
						if (data.viewNodes[0] == n) {
							data.viewNodes.shift();
						}
					} else { // If the last element to be inserted
						// above is
						// the target node, remove it
						if (data.viewNodes[data.viewNodes.length - 1] == n) {
							data.viewNodes.pop();
						}
					}

					// Nothing to drop...
					if (!data.viewNodes.length) {
						return false;
					}

					// If we are moving DOWN, then because a
					// store.remove()
					// takes place first,
					// the insertAt must be decremented.
					if (insertAt > this.view.store.indexOf(data.records[0])) {
						dir = 'down';
						insertAt--;
					}
				}

				for (var i = 0; i < data.records.length; i++) {
					var r = data.records[i];
					if (data.sourceView) {
						data.sourceView.store.remove(r);
					}
					this.view.store.insert(dir == 'down'
									? insertAt
									: insertAt++, r);
					var si = this.view.store.sortInfo;
					if (si) {
						this.view.store.sort(si.field, si.direction);
					}
				}
				return true;
			},

			// private
			removeDropIndicators : function(n) {
				if (n) {
					Ext.fly(n).removeClass(["x-view-drag-insert-above",
							"x-view-drag-insert-left",
							"x-view-drag-insert-right",
							"x-view-drag-insert-below"]);
					this.lastInsertClass = "_noclass";
				}
			},

			// private
			setDroppable : function(ddGroup) {
				if (!ddGroup)
					return;
				if (Ext.isArray(ddGroup)) {
					Ext.each(ddGroup, this.setDroppable, this);
					return;
				}
				this.addToGroup(ddGroup);
			}
		});

/**
 * @class Ext.ux.form.ItemSelector
 * @extends Ext.form.Field A control that allows selection of between two
 *          Ext.ux.form.MultiSelect controls.
 * 
 * @history 2008-06-19 bpm Original code contributed by Toby Stuart (with
 *          contributions from Robert Williams)
 * 
 * @constructor Create a new ItemSelector
 * @param {Object}
 *            config Configuration options
 * @xtype itemselector
 */
Ext.ux.form.ItemSelector = Ext.extend(Ext.form.Field, {
			hideNavIcons : false,
			imagePath : "",
			iconUp : "up2.gif",
			iconDown : "down2.gif",
			iconLeft : "left2.gif",
			iconRight : "right2.gif",
			iconTop : "top2.gif",
			iconBottom : "bottom2.gif",
			drawUpIcon : true,
			drawDownIcon : true,
			drawLeftIcon : true,
			drawRightIcon : true,
			drawTopIcon : true,
			drawBotIcon : true,
			delimiter : ',',
			bodyStyle : null,
			border : false,
			defaultAutoCreate : {
				tag : "div"
			},
			/**
			 * @cfg {Array} multiselects An array of
			 *      {@link Ext.ux.form.MultiSelect} config objects, with at
			 *      least all required parameters (e.g., store)
			 */
			multiselects : null,

			initComponent : function() {
				Ext.ux.form.ItemSelector.superclass.initComponent.call(this);
				this.addEvents({
							'rowdblclick' : true,
							'change' : true
						});
			},

			onRender : function(ct, position) {
				Ext.ux.form.ItemSelector.superclass.onRender.call(this, ct,
						position);

				// Internal default configuration for both multiselects
				var msConfig = [{
							legend : '可选',
							draggable : true,
							droppable : true,
							width : 100,
							height : 100
						}, {
							legend : '已选',
							droppable : true,
							draggable : true,
							width : 100,
							height : 100
						}];

				this.fromMultiselect = new Ext.ux.form.MultiSelect(Ext.applyIf(
						this.multiselects[0], msConfig[0]));
				this.fromMultiselect.on('dblclick', this.onRowDblClick, this);

				this.toMultiselect = new Ext.ux.form.MultiSelect(Ext.applyIf(
						this.multiselects[1], msConfig[1]));
				this.toMultiselect.on('dblclick', this.onRowDblClick, this);

				var p = new Ext.Panel({
							bodyStyle : this.bodyStyle,
							border : this.border,
							layout : "table",
							layoutConfig : {
								columns : 3
							}
						});

				p.add(this.fromMultiselect);
				var icons = new Ext.Panel({
							header : false
						});
				p.add(icons);
				p.add(this.toMultiselect);
				p.render(this.el);
				icons.el.down('.' + icons.bwrapCls).remove();

				// ICON HELL!!!
				if (this.imagePath != ""
						&& this.imagePath.charAt(this.imagePath.length - 1) != "/")
					this.imagePath += "/";
				this.iconUp = this.imagePath + (this.iconUp || 'up2.gif');
				this.iconDown = this.imagePath + (this.iconDown || 'down2.gif');
				this.iconLeft = this.imagePath + (this.iconLeft || 'left2.gif');
				this.iconRight = this.imagePath
						+ (this.iconRight || 'right2.gif');
				this.iconTop = this.imagePath + (this.iconTop || 'top2.gif');
				this.iconBottom = this.imagePath
						+ (this.iconBottom || 'bottom2.gif');
				var el = icons.getEl();
				this.toTopIcon = el.createChild({
							tag : 'img',
							src : this.iconTop,
							style : {
								cursor : 'pointer',
								margin : '2px'
							}
						});
				el.createChild({
							tag : 'br'
						});
				this.upIcon = el.createChild({
							tag : 'img',
							src : this.iconUp,
							style : {
								cursor : 'pointer',
								margin : '2px'
							}
						});
				el.createChild({
							tag : 'br'
						});
				this.addIcon = el.createChild({
							tag : 'img',
							src : this.iconRight,
							style : {
								cursor : 'pointer',
								margin : '2px'
							}
						});
				el.createChild({
							tag : 'br'
						});
				this.removeIcon = el.createChild({
							tag : 'img',
							src : this.iconLeft,
							style : {
								cursor : 'pointer',
								margin : '2px'
							}
						});
				el.createChild({
							tag : 'br'
						});
				this.downIcon = el.createChild({
							tag : 'img',
							src : this.iconDown,
							style : {
								cursor : 'pointer',
								margin : '2px'
							}
						});
				el.createChild({
							tag : 'br'
						});
				this.toBottomIcon = el.createChild({
							tag : 'img',
							src : this.iconBottom,
							style : {
								cursor : 'pointer',
								margin : '2px'
							}
						});
				this.toTopIcon.on('click', this.toTop, this);
				this.upIcon.on('click', this.up, this);
				this.downIcon.on('click', this.down, this);
				this.toBottomIcon.on('click', this.toBottom, this);
				this.addIcon.on('click', this.fromTo, this);
				this.removeIcon.on('click', this.toFrom, this);
				if (!this.drawUpIcon || this.hideNavIcons) {
					this.upIcon.dom.style.display = 'none';
				}
				if (!this.drawDownIcon || this.hideNavIcons) {
					this.downIcon.dom.style.display = 'none';
				}
				if (!this.drawLeftIcon || this.hideNavIcons) {
					this.addIcon.dom.style.display = 'none';
				}
				if (!this.drawRightIcon || this.hideNavIcons) {
					this.removeIcon.dom.style.display = 'none';
				}
				if (!this.drawTopIcon || this.hideNavIcons) {
					this.toTopIcon.dom.style.display = 'none';
				}
				if (!this.drawBotIcon || this.hideNavIcons) {
					this.toBottomIcon.dom.style.display = 'none';
				}

				var tb = p.body.first();
				this.el.setWidth(p.body.first().getWidth());
				p.body.removeClass();

				this.hiddenName = this.name;
				var hiddenTag = {
					tag : "input",
					type : "hidden",
					value : "",
					name : this.name
				};
				this.hiddenField = this.el.createChild(hiddenTag);
			},

			doLayout : function() {
				if (this.rendered) {
					this.fromMultiselect.fs.doLayout();
					this.toMultiselect.fs.doLayout();
				}
			},

			afterRender : function() {
				Ext.ux.form.ItemSelector.superclass.afterRender.call(this);

				this.toStore = this.toMultiselect.store;
				this.toStore.on('add', this.valueChanged, this);
				this.toStore.on('remove', this.valueChanged, this);
				this.toStore.on('load', this.valueChanged, this);
				this.valueChanged(this.toStore);
			},

			toTop : function() {
				var selectionsArray = this.toMultiselect.view
						.getSelectedIndexes();
				var records = [];
				if (selectionsArray.length > 0) {
					selectionsArray.sort();
					for (var i = 0; i < selectionsArray.length; i++) {
						record = this.toMultiselect.view.store
								.getAt(selectionsArray[i]);
						records.push(record);
					}
					selectionsArray = [];
					for (var i = records.length - 1; i > -1; i--) {
						record = records[i];
						this.toMultiselect.view.store.remove(record);
						this.toMultiselect.view.store.insert(0, record);
						selectionsArray.push(((records.length - 1) - i));
					}
				}
				this.toMultiselect.view.refresh();
				this.toMultiselect.view.select(selectionsArray);
			},

			toBottom : function() {
				var selectionsArray = this.toMultiselect.view
						.getSelectedIndexes();
				var records = [];
				if (selectionsArray.length > 0) {
					selectionsArray.sort();
					for (var i = 0; i < selectionsArray.length; i++) {
						record = this.toMultiselect.view.store
								.getAt(selectionsArray[i]);
						records.push(record);
					}
					selectionsArray = [];
					for (var i = 0; i < records.length; i++) {
						record = records[i];
						this.toMultiselect.view.store.remove(record);
						this.toMultiselect.view.store.add(record);
						selectionsArray.push((this.toMultiselect.view.store
								.getCount())
								- (records.length - i));
					}
				}
				this.toMultiselect.view.refresh();
				this.toMultiselect.view.select(selectionsArray);
			},

			up : function() {
				var record = null;
				var selectionsArray = this.toMultiselect.view
						.getSelectedIndexes();
				selectionsArray.sort();
				var newSelectionsArray = [];
				if (selectionsArray.length > 0) {
					for (var i = 0; i < selectionsArray.length; i++) {
						record = this.toMultiselect.view.store
								.getAt(selectionsArray[i]);
						if ((selectionsArray[i] - 1) >= 0) {
							this.toMultiselect.view.store.remove(record);
							this.toMultiselect.view.store.insert(
									selectionsArray[i] - 1, record);
							newSelectionsArray.push(selectionsArray[i] - 1);
						}
					}
					this.toMultiselect.view.refresh();
					this.toMultiselect.view.select(newSelectionsArray);
				}
			},

			down : function() {
				var record = null;
				var selectionsArray = this.toMultiselect.view
						.getSelectedIndexes();
				selectionsArray.sort();
				selectionsArray.reverse();
				var newSelectionsArray = [];
				if (selectionsArray.length > 0) {
					for (var i = 0; i < selectionsArray.length; i++) {
						record = this.toMultiselect.view.store
								.getAt(selectionsArray[i]);
						if ((selectionsArray[i] + 1) < this.toMultiselect.view.store
								.getCount()) {
							this.toMultiselect.view.store.remove(record);
							this.toMultiselect.view.store.insert(
									selectionsArray[i] + 1, record);
							newSelectionsArray.push(selectionsArray[i] + 1);
						}
					}
					this.toMultiselect.view.refresh();
					this.toMultiselect.view.select(newSelectionsArray);
				}
			},

			fromTo : function() {
				var selectionsArray = this.fromMultiselect.view
						.getSelectedIndexes();
				var records = [];
				if (selectionsArray.length > 0) {
					for (var i = 0; i < selectionsArray.length; i++) {
						record = this.fromMultiselect.view.store
								.getAt(selectionsArray[i]);
						records.push(record);
					}
					if (!this.allowDup)
						selectionsArray = [];
					for (var i = 0; i < records.length; i++) {
						record = records[i];
						if (this.allowDup) {
							var x = new Ext.data.Record();
							record.id = x.id;
							delete x;
							this.toMultiselect.view.store.add(record);
						} else {
							this.fromMultiselect.view.store.remove(record);
							this.toMultiselect.view.store.add(record);
							selectionsArray.push((this.toMultiselect.view.store
									.getCount() - 1));
						}
					}
				}
				this.toMultiselect.view.refresh();
				this.fromMultiselect.view.refresh();
				var si = this.toMultiselect.store.sortInfo;
				if (si) {
					this.toMultiselect.store.sort(si.field, si.direction);
				}
				this.toMultiselect.view.select(selectionsArray);
			},

			toFrom : function() {
				var selectionsArray = this.toMultiselect.view
						.getSelectedIndexes();
				var records = [];
				if (selectionsArray.length > 0) {
					for (var i = 0; i < selectionsArray.length; i++) {
						record = this.toMultiselect.view.store
								.getAt(selectionsArray[i]);
						records.push(record);
					}
					selectionsArray = [];
					for (var i = 0; i < records.length; i++) {
						record = records[i];
						this.toMultiselect.view.store.remove(record);
						if (!this.allowDup) {
							this.fromMultiselect.view.store.add(record);
							selectionsArray
									.push((this.fromMultiselect.view.store
											.getCount() - 1));
						}
					}
				}
				this.fromMultiselect.view.refresh();
				this.toMultiselect.view.refresh();
				var si = this.fromMultiselect.store.sortInfo;
				if (si) {
					this.fromMultiselect.store.sort(si.field, si.direction);
				}
				this.fromMultiselect.view.select(selectionsArray);
			},

			valueChanged : function(store) {
				var record = null;
				var values = [];
				for (var i = 0; i < store.getCount(); i++) {
					record = store.getAt(i);
					values.push(record.get(this.toMultiselect.valueField));
				}
				this.hiddenField.dom.value = values.join(this.delimiter);
				this.fireEvent('change', this, this.getValue(),
						this.hiddenField.dom.value);
			},

			getValue : function() {
				return this.hiddenField.dom.value;
			},

			onRowDblClick : function(vw, index, node, e) {
				if (vw == this.toMultiselect.view) {
					this.toFrom();
				} else if (vw == this.fromMultiselect.view) {
					this.fromTo();
				}
				return this.fireEvent('rowdblclick', vw, index, node, e);
			},

			reset : function() {
				range = this.toMultiselect.store.getRange();
				this.toMultiselect.store.removeAll();
				this.fromMultiselect.store.add(range);
				var si = this.fromMultiselect.store.sortInfo;
				if (si) {
					this.fromMultiselect.store.sort(si.field, si.direction);
				}
				this.valueChanged(this.toMultiselect.store);
			}
		});

Ext.reg('itemselector', Ext.ux.form.ItemSelector);

// backwards compat
Ext.ux.ItemSelector = Ext.ux.form.ItemSelector;

/**
 * @class Ext.ux.GridRowEditForm
 * @extends Ext.util.Observable 按行编辑Grid数据的插件。会弹出一个自动生成Form的Window
 *          用于GridPanel。必须为需要编辑的列配置editor
 */
Ext.ux.GridRowEditForm = Ext.extend(Ext.util.Observable, {
	/**
	 * @cfg {Object} windowConfig config for the window
	 */
	windowConfig : {},
	/**
	 * @cfg {String} newTitlle 新建记录时window显示Title（默认“新建”）
	 */
	newTitle : '新建',
	/**
	 * @cfg {String} editTitle 编辑记录时window显示Title（默认“编辑”）
	 */
	editTitle : '编辑',
	/**
	 * @cfg {String} titleDataIndex
	 */
	titleDataIndex : null,
	/**
	 * @constructor
	 * @param {Object}
	 *            config The config object
	 */
	constructor : function(config) {
		Ext.apply(this, config || {});
		this.addEvents(
				/**
				 * @event beforeshow this event fires before the edit from show
				 * @param {Ext.form.FormPanel}
				 *            form this edit form
				 * @param {Ext.data.Record}
				 *            record the editing record
				 */
				'beforeshow',
				/**
				 * @event aftershow this event fires after the edit form show
				 * @param {Ext.form.FormPanel}
				 *            form this edit form
				 * @param {Ext.data.Record}
				 *            record the editing record
				 */
				'aftershow',
				/**
				 * @event afteredit this event fires after update the record
				 * @param {Ext.form.FormPanel}
				 *            form this edit form
				 * @param {Ext.data.Record}
				 *            record the editing record*
				 */
				'afteredit',
				/**
				 * @event afteredit this event fires after update the record
				 * @param {Ext.form.FormPanel}
				 *            form this edit form
				 * @param {Ext.data.Record}
				 *            record the editing record
				 */
				'canceledit');
		Ext.ux.GridRowEditForm.superclass.constructor.call(this);
	},
	// private
	init : function(grid) {
		this.grid = grid;
		grid.on('render', this.onRender, this);
		this.initWindow();
		grid.on('destroy', this.onDestroy, this);
	},
	initWindow : function() {
		var grid = this.grid;
		var colModel = grid.getColumnModel();
		var items = [];
		var height = 4 * 23;
		Ext.each(colModel.config, function(col) {
					if (!Ext.isEmpty(col.dataIndex) && col.editor) {
						var item = Ext.apply(Ext.apply({
											fieldLabel : col.header
										}, col.editor), {
									name : col.dataIndex
								});
						items.push(item);
						height += 23
								* ((item.xtype && item.xtype === 'textarea')
										? 3
										: 1);
					}
				}, this);
		this.form = new Ext.FormPanel({
					border : false,
					defaultType : 'textfield',
					labelWidth : 80,
					defaults : {
						msgTarget : 'under'
					},
					items : items,
					unstyled : true
				});

		var form = this.form;
		var winconfig = Ext.apply({
					layout : 'fit',
					items : form,
					width : 300,
					bodyStyle : 'padding:5px 5px 5px 5px',
					height : height,
					closeAction : 'hide',
					closable : false,
					modal : true,
					tbar : [{
								iconCls : 'ok',
								text : '确定',
								scope : this,
								handler : this.onUpdate
							}, {
								iconCls : 'cancel',
								text : '取消',
								scope : this,
								handler : function() {
									this.fireEvent('canceledit', this.form,
											this.record);
									this.window.hide();
								}
							}]
				}, this.windowConfig);
		this.window = new Ext.Window(winconfig);
	},
	onRender : function() {
		var grid = this.grid;
		// 为Grid添加newRecord方法
		grid.newRecord = this.editRecord.createDelegate(this, [null, true]);
		grid.on('rowdblclick', this.onRowDblClick, this);
	},
	onRowDblClick : function(grid, rowIdx, e) {
		var r = grid.getStore().getAt(rowIdx);
		this.editRecord(r, false, r.get(this.titleDataIndex) || this.editTitle);
	},
	/**
	 * 编辑记录
	 * 
	 * @param {Record}
	 *            r 要编辑的记录
	 * @param {Boolean}
	 *            isNew 是否为新建
	 * @param {String}
	 *            windowTitle 窗口标题
	 */
	editRecord : function(r, isNew, windowTitle) {
		this.record = r;
		this.isNew = isNew || false;
		if (this.fireEvent('beforeshow', this.form, r) === false) {
			return;
		};
		if (windowTitle) {
			this.window.setTitle(windowTitle);
		} else {
			this.window.setTitle(isNew ? this.newTitle : this.editTitle);
		}
		this.window.show(this.grid.getGridEl());
		if (!isNew) {
			this.form.getForm().loadRecord(r);
		} else {
			this.form.getForm().clear();
		}
		this.fireEvent('aftershow', this.form, r);
	},
	onUpdate : function() {
		if (!this.form.getForm().isValid()) {
			return;
		}
		this.form.getForm().updateRecord(this.record);
		this.fireEvent('afteredit', this.form, this.record);
		this.window.hide();
	},
	onDestroy : function() {
		Ext.destroy(this.window);
	}
});
/*
 * ! Ext JS Library 3.2.0 Copyright(c) 2006-2010 Ext JS, Inc.
 * licensing@extjs.com http://www.extjs.com/license
 */
Ext.ns('Ext.ux.form');
/**
 * @class Ext.ux.form.SearchField
 *        查询输入框。可用于对Grid等Store的查询。用户输入条件后，点击查询按扭或者使用回车键时，该组件将在Store的baseParams中加入用户输入的查询条件，并调用Store的reload方法：
 * 
 * <pre><code>
 *        	...
 *        	var o = {
 * 				start : 0
 * 			};
 * 		this.store.baseParams = this.store.baseParams || {};
 * 		this.store.baseParams[this.paramName] = v;
 * 		this.store.reload({
 * 				params : o
 * 			});
 *        	...
 * </code>
 * v为用户输入的值
 * </pre>
 * 
 * @extends Ext.form.TwinTriggerField
 */
Ext.ux.form.SearchField = Ext.extend(Ext.form.TwinTriggerField, {
			/**
			 * @cfg {Ext.data.Store} store 绑定查询的Store
			 */
			initComponent : function() {
				Ext.ux.form.SearchField.superclass.initComponent.call(this);
				this.on('specialkey', function(f, e) {
							if (e.getKey() == e.ENTER) {
								this.onTrigger2Click();
							}
						}, this);
			},

			validationEvent : false,
			validateOnBlur : false,
			trigger1Class : 'x-form-clear-trigger',
			trigger2Class : 'x-form-search-trigger',
			hideTrigger1 : true,
			width : 180,
			hasSearch : false,
			/**
			 * @cfg {String} paramName
			 *      相当于Field的name属性，指定该查询发送到后台时使用的名称。（默认为"query"，即后台使用request.getParameter("query")可获得用户输入的查询条件）
			 */
			paramName : 'query',

			onTrigger1Click : function() {
				if (this.hasSearch) {
					this.el.dom.value = '';
					var o = {
						start : 0
					};
					this.store.baseParams = this.store.baseParams || {};
					this.store.baseParams[this.paramName] = '';
					this.store.reload({
								params : o
							});
					this.triggers[0].hide();
					this.hasSearch = false;
				}
			},

			onTrigger2Click : function() {
				var v = this.getRawValue();
				if (v.length < 1) {
					this.onTrigger1Click();
					return;
				}
				var o = {
					start : 0
				};
				this.store.baseParams = this.store.baseParams || {};
				this.store.baseParams[this.paramName] = v;
				this.store.reload({
							params : o
						});
				this.hasSearch = true;
				this.triggers[0].show();
			}
		});

Ext.ns('Ext.ux');
/**
 * @class Ext.ux.InklingSearchField
 * @extends Ext.ux.form.SearchField
 *          通用多条件模糊查询组件。常用放置在Grid的TopToolbar中。需要后台数据来源配合使用。
 *          此组件会在查询的时候为Store增加两个baseParam: <div class="sub-desc">
 *          <ul>
 *          <li><b>inklings</b>:要查询的字段 </li>
 *          <li><b>inkling</b>:用户输入的条件</li>
 *          </ul>
 *          </div> 后台详见StandardQuery相关内容
 */
Ext.ux.InklingSearchField = Ext.extend(Ext.ux.form.SearchField, {
			/**
			 * @cfg {Ext.data.Store} store 查询关联的Store
			 * @requires
			 */
			/**
			 * @cfg {Object} tooltip Tooltip配置对象，包含两个属性： <div class="sub-desc">
			 *      <ul>
			 *      <li><b>title</b>:标题</li>
			 *      <li><b>text</b>:提示内容</li>
			 *      </ul>
			 *      </div>
			 */
			// private
			initComponent : function() {
				Ext.ux.InklingSearchField.superclass.initComponent.call(this);
				this.addEvents('beforesearch');
			},
			// private
			onRender : function(ct, position) {
				Ext.ux.InklingSearchField.superclass.onRender.call(this, ct,
						position);
				if (this.tooltip && this.tooltip.text)
					new Ext.ToolTip({
								target : this.id,
								trackMouse : false,
								draggable : false,
								maxWidth : 200,
								minWidth : 100,
								title : this.tooltip.title,
								html : this.tooltip.text
							});
			},
			width : 120,
			hasSearch : false,
			emptyText : '请输入关键词',
			/**
			 * @cfg {String} paramName 查询值的参数名，默认为inkling，请不要随意更改此配置
			 */
			paramName : 'inkling',
			/**
			 * @cfg {Array} inklings
			 *      要进行查询匹配的列（属性）。此参数为一个数组，其中的元素为Object，每个元素应该包含以下参数： <div
			 *      class="sub-desc">
			 *      <ul>
			 *      <li><b>name</b>：{String}
			 *      列名或者属性名。后台为JDBC查询时使用列名，为Hibernate查询时使用属性名</li>
			 *      <li><b>op</b>：{String}
			 *      匹配时使用的运算符。可选：'=','&gt;','&lt;','&gt;=','&lt;=','&gt;&lt;','like'</li>
			 *      <li><b>type</b>：{String}
			 *      该字段或属性的类型。可选：'string','integer','float','double','date','datetime','boolean'</li>
			 *      </ul>
			 *      </div>
			 * @requires
			 */
			inklings : [],
			onTrigger2Click : function() {
				var params = '';
				Ext.iterate(this.inklings, function(item, index, allItems) {
							params += item.name + ',' + item.op + ','
									+ item.type;
							if (index < allItems.length - 1) {
								params += '|';
							}
						}, this);
				this.store.baseParams = this.store.baseParams || {};
				this.store.baseParams['inklings'] = params;
				this.fireEvent('beforesearch', this, this.store);
				Ext.ux.InklingSearchField.superclass.onTrigger2Click.call(this);
			},
			onTrigger1Click : function() {
				this.store.baseParams = this.store.baseParams || {};
				delete this.store.baseParams['inklings'];
				Ext.ux.InklingSearchField.superclass.onTrigger1Click.call(this);
			}
		});

Ext.override(Ext.grid.RowSelectionModel, {
			handleMouseDown : function(g, rowIndex, e) {
				if (e.button !== 0 || this.isLocked()) {
					return;
				}
				var view = this.grid.getView();
				if (e.shiftKey && !this.singleSelect && this.last !== false) {
					var last = this.last;
					this.selectRange(last, rowIndex, e.ctrlKey);
					this.last = last; // reset the last
					view.focusRow(rowIndex);
				} else {
					var isSelected = this.isSelected(rowIndex);
					if (isSelected) {
						this.deselectRow(rowIndex);
					} else if (!isSelected || this.getCount() > 1) {
						this.selectRow(rowIndex, true);
						view.focusRow(rowIndex);
					}
				}
			}
		});

Ext.ns('Ext.ux.form');
/**
 * @class Ext.ux.form.ChooseField
 * @extends Ext.form.TwinTriggerField 选择组件
 * @author bomwu
 * @version 1.0
 */
Ext.ux.form.ChooseField = Ext.extend(Ext.form.TwinTriggerField, {
	initComponent : function() {
		Ext.ux.form.ChooseField.superclass.initComponent.call(this);
		this.addEvents('beforeshowwin', 'aftershowwin');
	},
	// private
	onRender : function(ct, position) {
		Ext.ux.form.ChooseField.superclass.onRender.call(this, ct, position);
		if (this.tooltip && this.tooltip.text)
			new Ext.ToolTip({
						target : this.id,
						trackMouse : false,
						draggable : false,
						maxWidth : 200,
						minWidth : 100,
						title : this.tooltip.title,
						html : this.tooltip.text
					});
	},
	selectedGridTitle : '',

	onlySearch : false,
	validationEvent : false,
	validateOnBlur : false,
	trigger1Class : 'x-form-clear-trigger',
	trigger2Class : 'x-form-search-trigger',
	hideTrigger1 : true,
	editable : false,
	/**
	 * @property hasSearch 是否已经查询
	 * @type Boolean
	 */
	hasSearch : false,
	// private
	hasInit : false,
	updateCondition : false,
	gStore : undefined,
	autoLoad : true,
	singleSelect : false,
	/**
	 * @cfg {Function} setValueFun 返回选择结果的回调函数
	 */
	setValueFun : Ext.emptyFn,
	/**
	 * @cfg {Function} clearValueFun 清除选择结果的回调函数
	 */
	clearValueFun : Ext.emptyFn,
	/**
	 * @cfg {String} chooseUrl 选择资源的URL，此URL请求的Response应该是一个JSON对象
	 */
	chooseUrl : '',
	/**
	 * @cfg {Function} baseParamFun 用于获取数据基础参数的回调函数，返回值将会用于Store的baseParams属性
	 *      应该返回一个JSON对象 例：
	 * 
	 * <pre><code>
	 * function() {
	 * 	return {
	 * 		contractTypeID : Ext.getCmp('contractType').getValue()
	 * 	};
	 * }
	 * </code></pre>
	 * 
	 */
	baseParamFun : function() {
		return {};
	},
	/**
	 * @cfg {Function} paramFun 用于获取数据参数的回调函数应该返回一个JSON对象 例：
	 * 
	 * <pre><code>
	 * function() {
	 * 	return {
	 * 		start : 0,
	 * 		limit : 20
	 * 	};
	 * }
	 * </code>
	 * </pre>
	 */
	paramFun : function() {
		return {};
	},
	/**
	 * @cfg {Object} winConfig 用于配置选择窗口
	 */
	winConfig : {},

	search : function() {
		if (this.searchField) {
			this.searchField.onTrigger2Click();
			this.hasSearch = true;
		}
	},

	onTrigger1Click : function() {
		this.clearValueFun.call(this.scope || window);
		this.setEditable(true);
		if (this.selected) {
			this.selected.getStore().removeAll();
			this.grid.getSelectionModel().clearSelections();
		}
		this.triggers[0].hide();
	},

	onTrigger2Click : function() {
		if (this.disabled) {
			return;
		}
		if (!this.fireEvent('beforeshowwin', this)) {
			return;
		}
		if (this.hasInit && this.updateCondition) {
			// this.gStore.setBaseParam('deptCode',
			// this.baseParamFun.call(this.scope).deptCode);
			// this.gStore.setBaseParam('bookCode',
			// this.baseParamFun.call(this.scope).bookCode);
			// this.gStore.setBaseParam('prjCode',
			// this.baseParamFun.call(this.scope).prjCode);
			Ext.apply(this.gStore.baseParams, this.baseParamFun
							.call(this.scope));
			this.gStore.load();
			this.updateCondition = false;
		}
		if (this.hasInit) {
			this.window.show();
			if (!Ext.isEmpty(this.getValue())) {
				this.searchField.setValue(this.getValue());
				this.searchField.onTrigger2Click();
			}
			this.fireEvent('aftershowwin', this);
			return;
		}
		var url = this.chooseUrl;
		var setValueFun = this.setValueFun;
		Ext.Ajax.request({
					url : url,
					scope : this,
					success : this.initWindow,
					failure : Ext.emptyFn
				});

	},
	// private
	initWindow : function(response, config) {
		var win = this;
		json = Ext.util.JSON.decode(response.responseText);

		var sm = new Ext.grid.CheckboxSelectionModel({
			singleSelect : this.singleSelect,
			listeners : {
				rowselect : function(csm, rowIndex, r) {
					var flag = true;
					for (var i = 0; i < selected.getStore().getCount(); i++) {
						if (r.get(json.selectColumns[0].mapping) == selected
								.getStore().getAt(i).get('id')) {
							flag = false;
							return;
						}
					}
					if (flag) {
						var rd = new Ext.data.Record({
									id : r.get(json.selectColumns[0].mapping),
									dname : r
											.get(json.selectColumns[1].mapping)
								});
						if (json.selectColumns.length > 2) {
							for (var i = 2; i < json.selectColumns.length; i++) {
								rd.set(json.selectColumns[i].dataIndex,
										r.get(json.selectColumns[i].dataIndex));
							}
						}
						selected.getStore().add(rd);
					}

				},
				rowdeselect : function(csm, rowIndex, r) {
					for (var i = 0; i < selected.getStore().getCount(); i++) {
						var record = selected.getStore().getAt(i);
						if (record.get('id') == r
								.get(json.selectColumns[0].mapping)) {
							selected.getStore().remove(record);
						}
					}
				}
			}
		});
		json.columnModle = [sm].concat(json.columnModle);
		var cm = new Ext.grid.ColumnModel(json.columnModle);
		var store = this.gStore = new Ext.data.Store({
			url : __path + json.dataUrl,
			remoteSort : true,
			sortInfo : json.sortInfo,
			reader : new Ext.data.JsonReader({
						root : 'rows',
						totalProperty : 'totalCount',
						remoteSort : true,
						idProperty : json.idProperty
								|| json.fieldsNames[0].name,
						fields : json.fieldsNames
					}),
			baseParams : this.baseParamFun.call(this.scope || window),
			listeners : {
				load : function(s, records, options) {
					for (var i = 0; i < records.length; i++) {
						for (var j = 0; j < selected.getStore().getCount(); j++) {
							if (records[i].get(json.selectColumns[0].mapping) == selected
									.getStore().getAt(j).get('id')) {
								sm.selectRow(i, true);
							}
						}
					}
				}
			}
		});

		var gridConfig = {
			sm : sm,
			cm : cm,
			store : store,
			disableSelection : false,
			region : 'center',
			layout : 'fit',
			enableColumnMove : false,
			enableColumnResize : true,
			stripeRows : true,
			height : 340,
			loadMask : {
				msg : '正在加载数据，请稍侯……'
			},
			bbar : new Ext.PagingToolbar({
						pageSize : 20,
						store : store,
						displayInfo : true,
						displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
						emptyMsg : "无显示数据"
					})
		};
		var tbar;
		if (json.gridConfig) {
			if (json.gridConfig.tbar) {
				tbar = json.gridConfig.tbar;
				json.gridConfig.tbar = undefined;
				if (tbar instanceof Array) {
					for (var i = 0; i < tbar.length; i++) {
						tbar[i].scope = this;
					}
				} else if (tbar instanceof Object) {
					tbar.scope = this;
				}
			}
			Ext.apply(gridConfig, json.gridConfig);
		}
		var grid = this.grid = new Ext.grid.GridPanel(gridConfig);
		// grid.on('rowdblclick', function(t, rowIndex, e) {
		// var record = grid.getSelectionModel().getSelected();
		//			
		// });

		var ssm = new Ext.grid.CheckboxSelectionModel({
					singleSelect : false
				});

		// var columns = [ssm].concat([{
		// header : win.columnIdText,
		// dataIndex : 'id'
		// },{
		// header : win.columnDisplayText,
		// dataIndex : 'name'
		// }]);
		var columns = [ssm].concat(json.selectColumns);

		var ccm = new Ext.grid.ColumnModel(columns);

		var selected = this.selected = new Ext.grid.GridPanel({
					title : win.selectedGridTitle,
					sm : ssm,
					cm : ccm,
					split : true,
					collapsible : true, // 是否有右上方展开收缩条
					disableSelection : false,
					layout : 'fit',
					enableColumnMove : false,
					enableColumnResize : true,
					width : 250,
					region : 'east',
					autoScroll : true,
					store : new Ext.data.ArrayStore({
								pruneModifiedRecords : true,
								fields : ['id', 'dname']
							}),
					tbar : [{
						ref : '../cancelBtn',
						iconCls : 'cancel',
						text : '删除',
						scope : this,
						handler : function() {
							var drs = ssm.getSelections();
							for (var i = 0; i < drs.length; i++) {
								for (var j = 0; j < grid.store.getCount(); j++) {
									if (drs[i].get('id') == grid.store.getAt(j)
											.get(json.selectColumns[0].mapping)) {
										sm.deselectRow(j);
									}
								}
							}
							selected.getStore().remove(drs);
						}
					}]
				});

		var wintbar = [{
					xtype : 'button',
					ref : '../okBtn',
					text : '选定',
					iconCls : 'ok',
					scope : this,
					handler : function() {
						var records = selected.getStore().getRange();
						this.setValueFun.call(this.scope || window, records);
						this.setEditable(false);
						this.triggers[0].show();
						this.window.hide();
					}
				}, '-', {
					ref : '../cancelBtn',
					iconCls : 'cancel',
					text : '取消',
					scope : this,
					handler : function() {
						this.window.hide();
					}
				}];
		var param = this.paramFun.call(this.scope || window);
		if (tbar && param.compID != 1) {
			wintbar.push('-');
			wintbar.push(tbar);
		}
		if (json.searchBar) {
			var searchConfig = Ext.apply(json.searchFieldConfig || {}, {
						store : grid.getStore()
					});
			wintbar.push('-');
			this.searchField = new Ext.ux.InklingSearchField(searchConfig);
			wintbar.push(this.searchField);
		}
		var windowConfig = {
			layout : 'border',
			items : [grid, selected],
			tbar : {
				xtype : 'toolbar',
				items : wintbar
			}
		};
		Ext.apply(windowConfig, this.winConfig);
		this.window = new Ext.ChooseWindow(windowConfig);
		this.hasInit = true;
		this.window.show();
		if (this.autoLoad) {
			if (!Ext.isEmpty(this.getValue())) {
				this.searchField.setValue(this.getValue());
				this.searchField.onTrigger2Click();
			} else {
				grid.getStore().load({
							params : param
						});
			}
			this.hasSearch = true;
		}
		grid.getView().refresh();
		if (this.onlySearch)
			this.window.addBtn.hide();
		this.fireEvent('aftershowwin', this);
	},
	// private
	onDestroy : function() {
		Ext.destroy(this.window);
		Ext.ux.form.ChooseField.superclass.onDestroy.call(this);
	}
});
Ext.reg('choosefield', Ext.ux.form.ChooseField);

Ext.ux.GetUsersInklingSearchField = Ext.extend(Ext.ux.InklingSearchField, {
			initComponent : function() {
				Ext.ux.InklingSearchField.superclass.initComponent.call(this);
				this.addEvents('beforesearch');
				this.addEvents('beforeonTrigger1Click');
			},
			onTrigger1Click : function() {
				this.store.baseParams = this.store.baseParams || {};
				delete this.store.baseParams['inklings'];
				this.fireEvent('beforeonTrigger1Click', this, this.store);
				Ext.ux.InklingSearchField.superclass.onTrigger1Click.call(this);
			}
		});

Ext.ux.form.GetUsersOfDeptChooseField = Ext.extend(Ext.ux.form.ChooseField, {
	initComponent : function() {
		Ext.ux.form.GetUsersOfDeptChooseField.superclass.initComponent
				.call(this);
		this.addEvents('beforeinklingsearch');
		this.addEvents('beforeon1click');
	},
	onTrigger1Click : function() {
		this.clearValueFun.call(this.scope || window);
		this.triggers[0].hide();
		if (this.searchField) {
			this.searchField.onTrigger1Click();
			this.grid.getSelectionModel().clearSelections();
		}
	},

	onTrigger2Click : function() {
		if (this.disabled) {
			return;
		}
		if (!this.fireEvent('beforeshowwin', this)) {
			return;
		}
		if (this.hasInit) {
			this.window.show();
			this.gStore.load();
			this.fireEvent('aftershowwin', this);
			return;
		}
		var url = this.chooseUrl;
		var setValueFun = this.setValueFun;
		Ext.Ajax.request({
					url : url,
					scope : this,
					success : this.initWindow,
					failure : Ext.emptyFn
				});

	},
	initWindow : function(response, config) {

		var win = this;
		json = Ext.util.JSON.decode(response.responseText);

		var sm = new Ext.grid.CheckboxSelectionModel({
					singleSelect : this.singleSelect,
					header : ''
				});
		json.columnModle = [sm].concat(json.columnModle);
		var cm = new Ext.grid.ColumnModel(json.columnModle);
		var store = this.gStore = new Ext.data.Store({
					url : __path + json.dataUrl,
					remoteSort : true,
					sortInfo : json.sortInfo,
					reader : new Ext.data.JsonReader({
								root : 'rows',
								totalProperty : 'totalCount',
								remoteSort : true,
								idProperty : json.idProperty
										|| json.fieldsNames[0].name,
								fields : json.fieldsNames
							}),
					baseParams : this.baseParamFun.call(this.scope || window)
				});

		var gridConfig = {
			sm : sm,
			cm : cm,
			store : store,
			disableSelection : false,
			region : 'center',
			layout : 'fit',
			enableColumnMove : false,
			enableColumnResize : true,
			stripeRows : true,
			height : 340,
			loadMask : {
				msg : '正在加载数据，请稍侯……'
			},
			bbar : new Ext.PagingToolbar({
						pageSize : 20,
						store : store,
						displayInfo : true,
						displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
						emptyMsg : "无显示数据"
					})
		};
		var tbar;
		if (json.gridConfig) {
			if (json.gridConfig.tbar) {
				tbar = json.gridConfig.tbar;
				json.gridConfig.tbar = undefined;
				if (tbar instanceof Array) {
					for (var i = 0; i < tbar.length; i++) {
						tbar[i].scope = this;
					}
				} else if (tbar instanceof Object) {
					tbar.scope = this;
				}
			}
			Ext.apply(gridConfig, json.gridConfig);
		}
		var grid = this.grid = new Ext.grid.GridPanel(gridConfig);

		var wintbar = [{
					xtype : 'button',
					ref : '../okBtn',
					text : '选定',
					iconCls : 'ok',
					scope : this,
					handler : function() {
						var record = grid.getSelectionModel().getSelected();
						this.setValueFun.call(this.scope || window, record);
						this.setEditable(false);
						this.triggers[0].show();
						this.window.hide();
					}
				}, '-', {
					ref : '../cancelBtn',
					iconCls : 'cancel',
					text : '取消',
					scope : this,
					handler : function() {
						this.window.hide();
					}
				}];
		var param = this.paramFun.call(this.scope || window);
		if (tbar && param.compID != 1) {
			wintbar.push('-');
			wintbar.push(tbar);
		}
		if (json.searchBar) {
			var searchConfig = Ext.apply(json.searchFieldConfig || {}, {
						store : grid.getStore(),
						listeners : {
							scope : this,
							'beforesearch' : function(ink, s) {
								this.fireEvent('beforeinklingsearch', ink, s);
							},
							'beforeonTrigger1Click' : function(ink, s) {
								this.fireEvent('beforeon1Click', ink, s);
							}
						}
					});
			wintbar.push('-');
			this.searchField = new Ext.ux.GetUsersInklingSearchField(searchConfig);
			wintbar.push(this.searchField);
		}
		var windowConfig = {
			layout : 'border',
			items : [grid],
			tbar : {
				xtype : 'toolbar',
				items : wintbar
			}
		};
		Ext.apply(windowConfig, this.winConfig);
		this.window = new Ext.ChooseWindow(windowConfig);
		this.hasInit = true;
		this.window.show();
		if (this.autoLoad) {
			grid.getStore().load({
						params : param
					});
			this.hasSearch = true;
		}
		grid.getView().refresh();
		if (this.onlySearch)
			this.window.addBtn.hide();
		this.fireEvent('aftershowwin', this);
	}
});
Ext.ns('Ext');
/**
 * @class Ext.ChooseWindow
 * @extends Ext.Window 选择窗口
 */
Ext.ChooseWindow = Ext.extend(Ext.Window, {
			initComponent : function() {
				Ext.ChooseWindow.superclass.initComponent.call(this);
			},
			width : 500,
			height : 425,
			closeAction : 'hide',
			layout : 'border',
			grid : undefined,
			callback : function() {
			}
		});
// 下拉选择树
Ext.form.TreeField = Ext.extend(Ext.form.TwinTriggerField, {
			/**
			 * @cfg {Boolean} readOnly 设置为只读状态
			 * 
			 */
			readOnly : true,
			/**
			 * @cfg {String} displayField 用于显示数据的字段名
			 * 
			 */
			displayField : 'text',
			/**
			 * @cfg {String} valueField 用于保存真实数据的字段名
			 */
			valueField : null,
			/**
			 * @cfg {String} hiddenName 保存真实数据的隐藏域名
			 */
			hiddenName : null,
			/**
			 * @cfg {Integer} listWidth 下拉框的宽度
			 */
			listWidth : null,
			/**
			 * @cfg {Integer} minListWidth 下拉框最小宽度
			 */
			minListWidth : 50,
			/**
			 * @cfg {Integer} listHeight 下拉框高度
			 */
			listHeight : null,
			/**
			 * @cfg {Integer} minListHeight 下拉框最小高度
			 */
			minListHeight : 50,
			/**
			 * @cfg {String} dataUrl 数据地址
			 */
			dataUrl : null,
			/**
			 * @cfg {Ext.tree.TreePanel} tree 下拉框中的树
			 */
			tree : null,
			/**
			 * @cfg {String} value 默认值
			 */
			value : null,
			/**
			 * @cfg {String} displayValue 用于显示的默认值
			 */
			displayValue : null,
			/**
			 * @cfg {Object} baseParams 向后台传递的参数集合
			 */
			baseParams : {},
			/**
			 * @cfg {String} selectParent 是否选择非子节点
			 */
			selectParent : true,
			/**
			 * @cfg {Object} treeRootConfig 树根节点的配置参数
			 */
			treeRootConfig : {
				id : '___',
				text : '经费类别',
				draggable : false
			},
			/**
			 * @cfg {String/Object} autoCreate A DomHelper element spec, or true
			 *      for a default element spec (defaults to {tag: "input", type:
			 *      "text", size: "24", autocomplete: "off"})
			 */
			defaultAutoCreate : {
				tag : "input",
				type : "text",
				size : "24",
				autocomplete : "off"
			},
			trigger1Class : 'x-form-clear-trigger',
			hideTrigger1 : true,
			initComponent : function() {
				Ext.form.TreeField.superclass.initComponent.call(this);
				this.addEvents('select', 'expand', 'collapse', 'beforeselect');
			},
			initList : function() {
				if (!this.list) {
					var cls = 'x-treefield-list';

					this.list = new Ext.Layer({
								shadow : this.shadow,
								cls : [cls, this.listClass].join(' '),
								constrain : false
							});

					var lw = this.listWidth
							|| Math
									.max(this.wrap.getWidth(),
											this.minListWidth);
					this.list.setWidth(lw);
					this.list.swallowEvent('mousewheel');

					this.innerList = this.list.createChild({
								cls : cls + '-inner'
							});
					this.innerList.setWidth(lw - this.list.getFrameWidth('lr'));
					this.innerList.setHeight(this.listHeight
							|| this.minListHeight);
					if (!this.tree) {
						this.tree = this.createTree(this.innerList);
					}
					this.tree.on('click', this.select, this);
					if (this.filter) {
						var treeFilter = new Ext.tree.TreeFilter(this.tree, {
									clearBlank : true,
									autoClear : true
								});
						this.tree.on('load', function() {
									treeFilter.filterBy(function(n) {
												return !this.filter.test(n.id);
											}, this);
								}, this);
					}
					this.tree.render();
				}
			},
			onRender : function(ct, position) {
				Ext.form.TreeField.superclass.onRender.call(this, ct, position);
				if (this.hiddenName) {
					this.hiddenField = this.el.insertSibling({
								tag : 'input',
								type : 'hidden',
								name : this.hiddenName,
								id : (this.hiddenId || this.hiddenName)
							}, 'before', true);
					this.hiddenField.value = this.hiddenValue !== undefined
							? this.hiddenValue
							: this.value !== undefined ? this.value : '';
					this.el.dom.removeAttribute('name');
				}
				if (Ext.isGecko) {
					this.el.dom.setAttribute('autocomplete', 'off');
				}

				this.initList();
			},
			select : function(node) {
				if (this.fireEvent('beforeselect', node, this) != false) {
					this.onSelect(node);
					this.fireEvent('select', this, node);
				}
			},
			onSelect : function(node) {
				if (!this.selectParent && !node.isLeaf())
					return;
				this.setValue(node);
				this.collapse();
				this.triggers[0].show();
			},
			createTree : function(el) {
				var Tree = Ext.tree;

				var tree = new Tree.TreePanel({
							el : el,
							autoScroll : true,
							animate : true,
							containerScroll : true,
							rootVisible : false,
							height : this.listHeight,
							loader : new Tree.TreeLoader({
										dataUrl : this.dataUrl,
										baseParams : this.baseParams
									})
						});

				var root = new Tree.AsyncTreeNode(this.treeRootConfig);
				tree.setRootNode(root);
				return tree;
			},

			getValue : function() {
				if (this.valueField) {
					return typeof this.value != 'undefined' ? this.value : '';
				} else {
					return Ext.form.TreeField.superclass.getValue.call(this);
				}
			},
			setValue : function(node) {
				// if(!node)return;
				var text, value;
				if (node && typeof node == 'object') {
					text = node.attributes[this.displayField];
					value = node.attributes[this.valueField
							|| this.displayField];
				} else {
					text = node;
					value = node;

				}
				if (this.hiddenField) {
					this.hiddenField.value = value;
				}
				Ext.form.TreeField.superclass.setValue.call(this, text);
				this.value = value;
			},
			onResize : function(w, h) {
				Ext.form.TreeField.superclass.onResize.apply(this, arguments);
				if (this.list && this.listWidth == null) {
					var lw = Math.max(w, this.minListWidth);
					this.list.setWidth(lw);
					this.innerList.setWidth(lw - this.list.getFrameWidth('lr'));
				}
			},
			validateBlur : function() {
				return !this.list || !this.list.isVisible();
			},
			onDestroy : function() {
				if (this.list) {
					this.list.destroy();
				}
				if (this.wrap) {
					this.wrap.remove();
				}
				Ext.form.TreeField.superclass.onDestroy.call(this);
			},
			collapseIf : function(e) {
				if (!e.within(this.wrap) && !e.within(this.list)) {
					this.collapse();
				}
			},

			collapse : function() {
				if (!this.isExpanded()) {
					return;
				}
				this.list.hide();
				Ext.getDoc().un('mousewheel', this.collapseIf, this);
				Ext.getDoc().un('mousedown', this.collapseIf, this);
				this.fireEvent('collapse', this);
			},
			expand : function() {
				if (this.isExpanded() || !this.hasFocus) {
					return;
				}
				this.onExpand();
				this.list.alignTo(this.wrap, this.listAlign);
				this.list.show();
				Ext.getDoc().on('mousewheel', this.collapseIf, this);
				Ext.getDoc().on('mousedown', this.collapseIf, this);
				this.fireEvent('expand', this);
			},
			onExpand : function() {
				var doc = Ext.getDoc();
				this.on('click', function() {
						}, this);
			},
			isExpanded : function() {
				return this.list && this.list.isVisible();
			},
			onTrigger1Click : function() {
				this.setValue('');
				this.fireEvent('select', this, null);
				this.triggers[0].hide();
			},
			onTrigger2Click : function() {
				if (this.disabled) {
					return;
				}
				if (this.isExpanded()) {
					this.collapse();
				} else {
					this.onFocus({});
					this.expand();
				}
				this.el.focus();
			}
		});
Ext.reg('treeField', Ext.form.TreeField);
Ext.ux.OrderButton = Ext.extend(Ext.Button, {
			/**
			 * @cfg {Array} orders
			 */
			/**
			 * @cfg {Integer} orderCount
			 */
			/**
			 * @cfg {Object} windowConfig
			 */
			initComponent : function() {
				var orderArray = [];
				for (var i = 0; i < this.orderCount; i++) {
					orderArray.push({
								xtype : 'combo',
								fieldLabel : '排序列' + (i + 1),
								name : 'order-' + i,
								displayField : 'text',
								valueField : 'value',
								mode : 'local',
								triggerAction : 'all',
								editable : false,
								value : '',
								store : new Ext.data.ArrayStore({
											fields : ['value', 'text'],
											data : years
										})
							});
				}
				this.window = new Ext.Window(Ext.apply({
							width : 200,
							height : 300
						}, this.windowConfig));
				Ext.ux.OrderButton.superclass.initComponent.call(this);
			}
		});
Ext.ux.TabContextMenu = Ext.extend(Object, {

			items : [],

			constructor : function(config) {
				Ext.apply(this, config || {});
			},

			// public
			init : function(tabs) {
				this.tabs = tabs;
				tabs.addEvents('beforeshowcontextmenu');
				tabs.on({
							scope : this,
							contextmenu : this.onContextMenu,
							destroy : this.destroy
						});
			},

			destroy : function() {
				Ext.destroy(this.menu);
				delete this.menu;
				delete this.tabs;
				delete this.active;
			},

			// private
			onContextMenu : function(tabs, item, e) {
				this.active = item;
				var m = this.createMenu();
				e.stopEvent();
				if (tabs.fireEvent('beforeshowcontextmenu', tabs, item, m) !== false) {
					m.showAt(e.getPoint());
				}
			},

			createMenu : function() {
				if (!this.menu) {
					var menus = [];
					Ext.each(this.items, function(item) {
								if (item == '-') {
									menus.push('-');
								} else {
									menus.push({
												itemId : item.itemId,
												text : item.text,
												scope : this,
												handler : function() {
													item.handler.call(
															item.scope,
															this.tabs,
															this.active);
												}
											});
								}
							}, this);

					this.menu = new Ext.menu.Menu({
								items : menus
							});
				}
				return this.menu;
			}
		});

Ext.preg('tabcontextmenu', Ext.ux.TabContextMenu);

Ext.ux.Spotlight = function(config) {
	Ext.apply(this, config);
}
Ext.ux.Spotlight.prototype = {
	active : false,
	animate : true,
	duration : .25,
	easing : 'easeNone',

	// private
	animated : false,

	createElements : function() {
		var bd = Ext.getBody();

		this.right = bd.createChild({
					cls : 'x-spotlight'
				});
		this.left = bd.createChild({
					cls : 'x-spotlight'
				});
		this.top = bd.createChild({
					cls : 'x-spotlight'
				});
		this.bottom = bd.createChild({
					cls : 'x-spotlight'
				});

		this.all = new Ext.CompositeElement([this.right, this.left, this.top,
				this.bottom]);
	},

	show : function(el, callback, scope) {
		if (this.animated) {
			this.show.defer(50, this, [el, callback, scope]);
			return;
		}
		this.el = Ext.get(el);
		if (!this.right) {
			this.createElements();
		}
		if (!this.active) {
			this.all.setDisplayed('');
			this.applyBounds(true, false);
			this.active = true;
			Ext.EventManager.onWindowResize(this.syncSize, this);
			this.applyBounds(false, this.animate, false, callback, scope);
		} else {
			this.applyBounds(false, false, false, callback, scope); // all these
			// booleans
			// look
			// hideous
		}
	},

	hide : function(callback, scope) {
		if (this.animated) {
			this.hide.defer(50, this, [callback, scope]);
			return;
		}
		Ext.EventManager.removeResizeListener(this.syncSize, this);
		this.applyBounds(true, this.animate, true, callback, scope);
	},

	doHide : function() {
		this.active = false;
		this.all.setDisplayed(false);
	},

	syncSize : function() {
		this.applyBounds(false, false);
	},

	applyBounds : function(basePts, anim, doHide, callback, scope) {

		var rg = this.el.getRegion();

		var dw = Ext.lib.Dom.getViewWidth(true);
		var dh = Ext.lib.Dom.getViewHeight(true);

		var c = 0, cb = false;
		if (anim) {
			cb = {
				callback : function() {
					c++;
					if (c == 4) {
						this.animated = false;
						if (doHide) {
							this.doHide();
						}
						Ext.callback(callback, scope, [this]);
					}
				},
				scope : this,
				duration : this.duration,
				easing : this.easing
			};
			this.animated = true;
		}

		this.right.setBounds(rg.right, basePts ? dh : rg.top, dw - rg.right,
				basePts ? 0 : (dh - rg.top), cb);

		this.left.setBounds(0, 0, rg.left, basePts ? 0 : rg.bottom, cb);

		this.top.setBounds(basePts ? dw : rg.left, 0, basePts ? 0 : dw
						- rg.left, rg.top, cb);

		this.bottom.setBounds(0, rg.bottom, basePts ? 0 : rg.right, dh
						- rg.bottom, cb);

		if (!anim) {
			if (doHide) {
				this.doHide();
			}
			if (callback) {
				Ext.callback(callback, scope, [this]);
			}
		}
	},

	destroy : function() {
		this.doHide();
		Ext.destroy(this.right, this.left, this.top, this.bottom);
		delete this.el;
		delete this.all;
	}
};

// backwards compat
Ext.Spotlight = Ext.ux.Spotlight;

Ext.ns('Ext.ux');
/**
 * 基于Ext.ux.Spotlight写的教程组件,使用时需要Ext.ux.Spotlight.js，在Extjs官方例子中可以找到。
 * <p>
 * 用法： <code>
 * <pre>
 * var spot = new Ext.ux.Tour({
 * 			tours : [{
 * 						el : 'menubar',
 * 						title : '菜单栏', // String ,tour title
 * 						tip : '点击菜单栏',
 * 						bounds : [100, 100, 100, 100]
 * 					}, {
 * 						el : center.getEl().id,
 * 						title : '提示', // String ,tour title
 * 						tip : '信息2',
 * 						bounds : [200, 200, 200, 200]
 * 					}]
 * 		});
 * spot.first();
 * </pre>
 * </code>
 * </p>
 * 
 * @param {Object}
 *            config
 * @author Bom Wu
 * @email bomwu@hotmail.com
 * @version 0.1
 */
Ext.ux.Tour = function(config) {
	Ext.apply(this, config);
	this.spot = new Ext.Spotlight({
				easing : 'easeOut',
				duration : .3
			});
	this.win = new Ext.Window({
				closable : false,
				boxMinWidth : 250,
				boxMinHeight : 100,
				setTip : function(tip) {
					this.body.update('<div class="ux-tour-tip">' + tip
							+ '<div>');
				},
				buttons : [{
							width : 60,
							ref : 'back',
							text : '上一条',
							scope : this,
							handler : function() {
								this.back();
							}
						}, {
							width : 60,
							ref : 'next',
							text : '下一条',
							scope : this,
							handler : function() {
								this.next();
							}
						}, {
							width : 60,
							ref : 'end',
							text : '结束',
							scope : this,
							handler : function() {
								this.hide();
							}
						}]
			});
}
Ext.ux.Tour.prototype = {
	/**
	 * 数组，元素为Object，每个元素需要4个属性
	 * <ul>
	 * <li>el:String div id</li>
	 * <li>title:String 帮助标题</li>
	 * <li>tip:String 帮助信息，支持HTML</li>
	 * <li>bounds:Array 包含4个元素的整型数据，分别是[x,y,width,hegiht]。用于指定帮助框显示的位置及宽高</li>
	 * </ul>
	 * 例:
	 * 
	 * <pre>
	 * [{
	 * 			el : el, //String ,dom id
	 * 			title : '提示', //String ,tour title
	 * 			tip : '信息'
	 * 			bounds : [100, 100, 300, 100]
	 * 		}]
	 * </pre>
	 * 
	 * @cfg {Array} tours
	 */
	tours : [],
	// private
	currentIndex : -1,
	/**
	 * 显示指定索引的帮助
	 * 
	 * @param {Integer}
	 *            index
	 */
	show : function(index) {
		if (index == undefined) {
			index = 0;
		}
		if (index >= this.tours.length) {
			this.currentIndex = -1;
		}
		if (index >= this.tours.length || index < 0) {
			if (this.spot.alive) {
				this.spot.hide();
			}
			return;
		}
		if (index == 0) {
			this.win.buttons[0].disable();
		} else {
			this.win.buttons[0].enable();
		}
		if (index == this.tours.length - 1) {
			this.win.buttons[1].disable();
		} else {
			this.win.buttons[1].enable();
		}
		this.spot.show(this.tours[index].el);
		this.win.hide();
		this.win.setPosition(this.tours[index].bounds[0],
				this.tours[index].bounds[1])
		this.win.setSize(this.tours[index].bounds[2],
				this.tours[index].bounds[3]);
		this.win.setTitle(this.tours[index].title
				+ String.format("（{0}/{1}）", index + 1, this.tours.length));
		this.win.show();
		this.win.setTip(this.tours[index].tip);
	},
	/**
	 * 显示第一个帮助
	 */
	first : function() {
		this.currentIndex = 0;
		this.show(this.currentIndex);
	},
	/**
	 * 显示最上一个帮助
	 */
	back : function() {
		this.currentIndex--;
		this.show(this.currentIndex);
	},
	/**
	 * 显示最下一个帮助
	 */
	next : function() {
		this.currentIndex++;
		this.show(this.currentIndex);
	},
	/**
	 * 隐藏帮助
	 */
	hide : function(index) {
		this.spot.hide();
		this.win.hide();
	},
	destroy : function() {
		this.spot.destory()
		this.win.destory();
	}
}