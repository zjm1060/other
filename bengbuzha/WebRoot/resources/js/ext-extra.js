Ext.BLANK_IMAGE_URL = __cxtPath + '/resources/images/default/s.gif';
(function() {
	Ext.ua = navigator.userAgent.toLowerCase(), DOC = document, docMode = DOC.documentMode;
	check = function(r) {
		return r.test(Ext.ua);
	};
	Ext.isIE10 = Ext.isIE && check(/msie 10/);
	Ext.isIE9 = Ext.isIE && (check(/msie 9/) || (Ext.isIE10 && docMode == 9));
})();
Ext.data.Api.restActions.read = "POST";
Ext.data.Api.prepare = function(proxy) {
	if (!proxy.api) {
		proxy.api = {}; // <-- No api? create a blank one.
	}
	for (var verb in this.actions) {
		var action = this.actions[verb];
		proxy.api[action] = proxy.api[action] || proxy.url || proxy.directFn;
		if (typeof(proxy.api[action]) == 'string') {
			proxy.api[action] = {
				url : proxy.api[action],
				method : (proxy.restful === true)
						? Ext.data.Api.restActions[action]
						: 'POST'
			};
		}
	}
};

// 使Grid单元格内容可以选择复制
if (!Ext.grid.GridView.prototype.templates) {
	Ext.grid.GridView.prototype.templates = {};
}
Ext.grid.GridView.prototype.templates.cell = new Ext.Template(
		'<td class="x-grid3-col x-grid3-cell x-grid3-td-{id} x-selectable {css}" style="{style}" tabIndex="0" {cellAttr}>',
		'<div class="x-grid3-cell-inner x-grid3-col-{id}" {attr}>{value}</div>',
		'</td>');
// 增加Grid单元格ToolTip功能，配置 cellTip:true
// 修复Grid State中对column sortable设置错误的BUG
Ext.override(Ext.grid.GridPanel, {
	applyState : function(state) {
		var cm = this.colModel, cs = state.columns, store = this.store, s, c, colIndex;
		if (cs) {
			for (var i = 0, len = cs.length; i < len; i++) {
				s = cs[i];
				c = cm.getColumnById(s.id);
				if (c) {
					colIndex = cm.getIndexById(s.id);
					cm.setState(colIndex, {
								hidden : s.hidden || c.hidden,
								width : s.width || c.width,
								sortable : s.sortable || c.sortable
							});
					if (colIndex != i) {
						cm.moveColumn(colIndex, i);
					}
				}
			}
		}
		if (store) {
			s = state.sort;
			if (s) {
				store[store.remoteSort ? 'setDefaultSort' : 'sort'](s.field,
						s.direction);
			}
			s = state.group;
			if (store.groupBy) {
				if (s) {
					store.groupBy(s);
				} else {
					store.clearGrouping();
				}
			}

		}
		var o = Ext.apply({}, state);
		delete o.columns;
		delete o.sort;
		Ext.grid.GridPanel.superclass.applyState.call(this, o);
	},
	afterRender : Ext.grid.GridPanel.prototype.afterRender.createSequence(
			function() {
				if (!this.cellTip) {
					return;
				}
				// var store = this.getStore();
				// var cm = this.getColumnModel();
				var view = this.getView();
				this.tip = new Ext.ToolTip({
					target : view.mainBody,
					delegate : '.x-grid3-cell-inner',
					trackMouse : true,
					renderTo : document.body,
					listeners : {
						beforeshow : function updateTipBody(tip) {
							// var rowIndex = view
							// .findRowIndex(tip.triggerElement);
							// var cellIndex = view
							// .findCellIndex(tip.triggerElement);
							// tip.header.dom.innerHTML=cm.getColumnAt(cellIndex).header;
							// tip.body.dom.innerHTML = store
							// .getAt(rowIndex)
							// .get(cm.getColumnAt(cellIndex).dataIndex);
							if (Ext.isEmpty(tip.triggerElement.innerText)) {
								return false;
							}
							tip.body.dom.innerHTML = tip.triggerElement.innerText;
						}
					}
				});
			})
});
Ext.override(Ext.grid.RowNumberer, {
	renderer : function(value, cellmeta, record, rowIndex, columnIndex, store) {
		if (this.rowspan) {
			cellmeta.cellAttr = 'rowspan="' + this.rowspan + '"';
		}
		return (store.lastOptions && store.lastOptions.params && store.lastOptions.params.start)
				? store.lastOptions.params.start + rowIndex + 1
				: rowIndex + 1;
	}
});
Ext.override(Ext.form.TwinTriggerField, {
	initTrigger : function() {
		var ts = this.trigger.select('.x-form-trigger', true);
		var triggerField = this;
		ts.each(function(t, all, index) {
					var triggerIndex = 'Trigger' + (index + 1);
					t.hide = function() {
						var w = triggerField.width;
						this.dom.style.display = 'none';
						if (Ext.isNumber(w)) {
							triggerField.el.setWidth(w
									- triggerField.getTriggerWidth());
						}
						this['hidden' + triggerIndex] = true;
					};
					t.show = function() {
						var w = triggerField.width;
						this.dom.style.display = '';
						triggerField.el.setWidth(w
								- triggerField.getTriggerWidth());
						this['hidden' + triggerIndex] = false;
					};

					if (this['hide' + triggerIndex]) {
						t.dom.style.display = 'none';
						this['hidden' + triggerIndex] = true;
					}
					this.mon(t, 'click', this['on' + triggerIndex + 'Click'],
							this, {
								preventDefault : true
							});
					t.addClassOnOver('x-form-trigger-over');
					t.addClassOnClick('x-form-trigger-click');
				}, this);
		this.triggers = ts.elements;
	}
		/*
		 * , afterRender : function() {
		 * Ext.form.TwinTriggerField.superclass.afterRender.call(this);
		 * //debugger; /* var triggers = this.triggers, i = 0, len =
		 * triggers.length;
		 * 
		 * for (; i < len; ++i) { if (this['hideTrigger' + (i + 1)]) {
		 * triggers[i].hide(); } } this.on('afterrender', function() { var
		 * triggers = this.triggers, i = 0, len = triggers.length; for (; i <
		 * len; ++i) { if (this['hideTrigger' + (i + 1)]) { triggers[i].hide(); } } },
		 * this); }
		 */
	});
Ext.override(Ext.grid.PivotGridView,{
	initTemplates: function() {
        Ext.grid.PivotGridView.superclass.initTemplates.apply(this, arguments);
        
        var templates = this.templates || {};
        if (!templates.gcell) {
            templates.gcell = new Ext.XTemplate(
                '<td class="x-grid3-hd x-grid3-gcell x-grid3-td-{id} ux-grid-hd-group-row-{row} ' + this.colHeaderCellCls + '" style="{style}">',
                    '<div {tooltip} class="x-grid3-hd-inner x-grid3-hd-{id}" unselectable="on" style="{istyle}">', 
                        this.grid.enableHdMenu ? '<a class="x-grid3-hd-btn" href="#"></a>' : '', '{value}',
                    '</div>',
                '</td>'
            );
        }
        Ext.apply(templates,{master:this.masterTpl});
        this.templates = templates;
        this.hrowRe = new RegExp("ux-grid-hd-group-row-(\\d+)", "");
    }
});
Ext.util.JSON.encodeDate = function(d) {
	return d.format('"Y-m-d H:i:s"');
};
Ext.isCanvas = !!document.createElement('canvas').getContext;
if (!Ext.isIE) {
	// 非IE浏览器不支持selectSingleNode，selectNodes方法
	XMLDocument.prototype.selectSingleNode = Element.prototype.selectSingleNode = function(
			xpath) {
		var x = this.selectNodes(xpath)
		if (!x || x.length < 1)
			return null;
		return x[0];
	};
	XMLDocument.prototype.selectNodes = Element.prototype.selectNodes = function(
			xpath) {
		var xpe = new XPathEvaluator();
		var nsResolver = xpe.createNSResolver(this.ownerDocument == null
				? this.documentElement
				: this.ownerDocument.documentElement);
		var result = xpe.evaluate(xpath, this, nsResolver, 0, null);
		var found = [];
		var res;
		while (res = result.iterateNext())
			found.push(res);
		return found;
	};
}
// 处理超时及信息提示
Ext.Ajax.on('requestcomplete', function(conn, response, options) {
			try {
				var json = Ext.decode(response.responseText);
				if (json && !json.success && json.timeout) {
					window.location.href = json.redirectUri;
				}

				if (json && !json.success && json.showMessage) {
					Ext.Msg.alert(json.messageTitle, json.message);
					return false;
				}

			} catch (e) {
			}
		});
Ext.apply(Ext.DatePicker.prototype, {
			onMonthClick : function(e, t) {
				e.stopEvent();
				var el = new Ext.Element(t), pn;
				if (el.is('button.x-date-mp-cancel')) {
					this.hideMonthPicker();
				} else if (el.is('button.x-date-mp-ok')) {
					var d = new Date(this.mpSelYear, this.mpSelMonth,
							(this.activeDate || this.value).getDate());
					if (d.getMonth() != this.mpSelMonth) {
						// 'fix' the JS rolling date conversion if needed
						d = new Date(this.mpSelYear, this.mpSelMonth, 1)
								.getLastDateOfMonth();
					}
					this.update(d);
					this.hideMonthPicker();
				} else if ((pn = el.up('td.x-date-mp-month', 2))) {
					this.mpMonths.removeClass('x-date-mp-sel');
					pn.addClass('x-date-mp-sel');
					this.mpSelMonth = pn.dom.xmonth;
					this.onMonthDblClick(e, t);
				} else if ((pn = el.up('td.x-date-mp-year', 2))) {
					this.mpYears.removeClass('x-date-mp-sel');
					pn.addClass('x-date-mp-sel');
					this.mpSelYear = pn.dom.xyear;
				} else if (el.is('a.x-date-mp-prev')) {
					this.updateMPYear(this.mpyear - 10);
				} else if (el.is('a.x-date-mp-next')) {
					this.updateMPYear(this.mpyear + 10);
				}
			}
		});
// Ext.override(Ext.tree.TreeEventModel, {
// getNode : function(e) {
// var t;
// if (t = e.getTarget('.x-tree-node-el', 10)) {
// var id = Ext.fly(t, '_treeEvents').getAttribute(
// 'tree-node-id', 'ext')
// || t.getAttribute('ext:tree-node-id');
// if (id) {
// return this.tree.getNodeById(id);
// }
// }
// return null;
// }
// });
// 屏蔽BackSpace 除input元素外
Ext.onReady = Ext.onReady.createSequence(function() {
			Ext.getDoc().on('keydown', function(e, t, o) {
				if (e.getKey() == Ext.EventObject.BACKSPACE) {
					var obj = e.getTarget();
					if (obj
							&& obj.tagName
							&& (obj.tagName.toLowerCase() == "input" || obj.tagName
									.toLowerCase() == "textarea")) {
					} else {
						e.stopEvent();
					}
				}
			});
		});
function backSpaceKeyDown(e, t, o) {
	if (this.readOnly && e.getKey() == Ext.EventObject.BACKSPACE) {
		e.stopEvent();
	}
}
// 对Form Field添加Tooltip支持，配置qtip即可，类型为{Ext.ToolTip|String}，readOnly时屏蔽BackSpace
Ext.override(Ext.form.Field, {
			afterRender : Ext.form.Field.prototype.afterRender.createSequence(
					function() {
						if (this.qtip) {
							var target = this.getTipTarget();
							if (typeof this.qtip == 'object') {
								Ext.QuickTips.register(Ext.apply({
											target : target
										}, this.qtip));
							} else {
								target.dom.qtip = this.qtip;
							}
						}
						this.el.on('keydown', backSpaceKeyDown, this);
					}),
			getTipTarget : function() {
				return this.el;
			}
		});
// 对DisplayField添加click支持
Ext.override(Ext.form.DisplayField, {
	afterRender : Ext.form.Field.prototype.afterRender.createSequence(
			function() {
				if (this.clickAction) {
					this.el.on('click', function() {
								this.clickAction.call(this.scope || this, this);
							}, this);
					this.el.applyStyles({
								cursor : 'pointer',
								'text-decoration' : 'underline'
							});
				}
			})
});
// 对Combobox、ChooseField等 editable=false时屏蔽BackSpace
Ext.override(Ext.form.TriggerField, {
	afterRender : Ext.form.TriggerField.prototype.afterRender.createSequence(
			function() {
				this.el.on('keydown', function(e, t, o) {
							if (!this.editable
									&& e.getKey() == Ext.EventObject.BACKSPACE) {
								e.stopEvent();
							}
						}, this);
			})
});
// checkboxes and radios, the main element is a hidden input.
Ext.override(Ext.form.Checkbox, {
			getTipTarget : function() {
				return this.imageEl;
			},
			setBoxLabel : function(label) {
				this.boxLabel = label;
				if (this.wrap) {
					var box = this.wrap.down('.x-form-cb-label');
					if (box && box.dom) {
						box.dom.innerHTML = this.boxLabel;
					}
				}
			}
		});
Ext.form.BasicForm.prototype.clear = function() {
	this.el.dom.reset();
};
// 解决调用BasicForm.setValues时不能设置RadioGroup值的BUG，注意，RadioGroup必须设置name属性
Ext.override(Ext.form.RadioGroup, {
			getValue : function() {
				var v;
				if (this.rendered) {
					this.items.each(function(item) {
								if (!item.getValue())
									return true;
								v = item.getRawValue();
								return false;
							});
				} else {
					for (var k in this.items) {
						if (this.items[k].checked) {
							v = this.items[k].inputValue;
							break;
						}
					}
				}
				return v;
			},
			setValue : function(v) {
				if (this.rendered)
					this.items.each(function(item) {
								item.setValue(item.getRawValue() == v);
							});
				else {
					for (var k in this.items) {
						this.items[k].checked = this.items[k].inputValue == v;
					}
				}
			}
		});
/**
 * 
 * @param {Object}
 *            obj 要操作的对象
 * @param {propertyName}
 *            pname 属性名称。支持复合属性名，如name.firstname
 * @return {Mixed} 属性值
 */
Ext.getProperty = function(obj, pname) {
	var field = String(pname);
	var index = field.indexOf('.');
	if (index == -1) {
		return obj[pname];
	}
	var value = obj[field.substring(0, index)];
	if (value == null) {
		return null;
	}
	return Ext.getProperty(value, field.substring(index + 1, field.length));
};
/**
 * 
 * @param {Object}
 *            obj 要操作的对象
 * @param {propertyName}
 *            pname 属性名称。支持复合属性名，如name.firstname
 * @param {Mixed}
 *            要设置的属性值 value
 */
Ext.setProperty = function(obj, pname, value) {
	var field = String(pname);
	var index = field.indexOf('.');
	if (index == -1) {
		obj[pname] = value;
		return;
	}
	var obj1 = obj[field.substring(0, index)];
	Ext.setProperty(obj1, field.substring(index + 1, field.length), value);
};
// 对Record加入复合属性的支持。
Ext.override(Ext.data.Record, {
			get : function(name) {
				return Ext.getProperty(this.data, name);
			},
			set : function(name, value) {
				var encode = Ext.isPrimitive(value) ? String : Ext.encode;
				if (encode(this.get(name)) == encode(value)) {
					return;
				}
				this.dirty = true;
				if (!this.modified) {
					this.modified = {};
				}
				var rname = name.indexOf('.') == -1 ? name : name.substring(0,
						name.indexOf('.'))
				if (this.modified[rname] === undefined) {
					this.modified[rname] = this.data[rname];
				}
				Ext.setProperty(this.data, name, value);
				if (!this.editing) {
					this.afterEdit();
				}
			}
		});
// 修正Grid对复合属性的支持。这样在column的dataIndex中可以直接使用复合属性了，如：{dataIndex:'project.name'}
// 显示的是record.data.project.name的值
Ext.grid.GridView.prototype.doRender = function(columns, records, store,
		startRow, colCount, stripe) {
	var templates = this.templates, cellTemplate = templates.cell, rowTemplate = templates.row, last = colCount
			- 1;

	var tstyle = 'width:' + this.getTotalWidth() + ';';

	// buffers
	var rowBuffer = [], colBuffer = [], rowParams = {
		tstyle : tstyle
	}, meta = {}, column, record;

	// build up each row's HTML
	for (var j = 0, len = records.length; j < len; j++) {
		record = records[j];
		colBuffer = [];

		var rowIndex = j + startRow;

		// build up each column's HTML
		for (var i = 0; i < colCount; i++) {
			column = columns[i];

			meta.id = column.id;
			meta.css = i === 0 ? 'x-grid3-cell-first ' : (i == last
					? 'x-grid3-cell-last '
					: '');
			meta.attr = meta.cellAttr = '';
			meta.style = column.style;
			// 原为：record.data[column.name]
			meta.value = column.renderer
					.call(column.scope, record.get(column.name), meta, record,
							rowIndex, i, store);

			if (Ext.isEmpty(meta.value)) {
				meta.value = ' ';
			}

			if (this.markDirty && record.dirty
					&& Ext.isDefined(record.modified[column.name])) {
				meta.css += ' x-grid3-dirty-cell';
			}

			colBuffer[colBuffer.length] = cellTemplate.apply(meta);
		}

		// set up row striping and row dirtiness CSS classes
		var alt = [];

		if (stripe && ((rowIndex + 1) % 2 === 0)) {
			alt[0] = 'x-grid3-row-alt';
		}

		if (record.dirty) {
			alt[1] = ' x-grid3-dirty-row';
		}

		rowParams.cols = colCount;

		if (this.getRowClass) {
			alt[2] = this.getRowClass(record, rowIndex, rowParams, store);
		}

		rowParams.alt = alt.join(' ');
		rowParams.cells = colBuffer.join('');

		rowBuffer[rowBuffer.length] = rowTemplate.apply(rowParams);
	}

	return rowBuffer.join('');
};
Ext.grid.GridView.prototype.refreshRow = function(record) {
	var store = this.ds, colCount = this.cm.getColumnCount(), columns = this
			.getColumnData(), last = colCount - 1, cls = ['x-grid3-row'], rowParams = {
		tstyle : String.format("width: {0};", this.getTotalWidth())
	}, colBuffer = [], cellTpl = this.templates.cell, rowIndex, row, column, meta, css, i;

	if (Ext.isNumber(record)) {
		rowIndex = record;
		record = store.getAt(rowIndex);
	} else {
		rowIndex = store.indexOf(record);
	}

	if (!record || rowIndex < 0) {
		return;
	}

	for (i = 0; i < colCount; i++) {
		column = columns[i];

		if (i == 0) {
			css = 'x-grid3-cell-first';
		} else {
			css = (i == last) ? 'x-grid3-cell-last ' : '';
		}

		meta = {
			id : column.id,
			style : column.style,
			css : css,
			attr : "",
			cellAttr : ""
		};

		meta.value = column.renderer.call(column.scope,
				record.get(column.name), meta, record, rowIndex, i, store);

		if (Ext.isEmpty(meta.value)) {
			meta.value = '&#160;';
		}

		if (this.markDirty && record.dirty
				&& typeof record.modified[column.name] != 'undefined') {
			meta.css += ' x-grid3-dirty-cell';
		}

		colBuffer[i] = cellTpl.apply(meta);
	}

	row = this.getRow(rowIndex);
	row.className = '';

	if (this.grid.stripeRows && ((rowIndex + 1) % 2 === 0)) {
		cls.push('x-grid3-row-alt');
	}

	if (this.getRowClass) {
		rowParams.cols = colCount;
		cls.push(this.getRowClass(record, rowIndex, rowParams, store));
	}

	this.fly(row).addClass(cls).setStyle(rowParams.tstyle);
	rowParams.cells = colBuffer.join("");
	row.innerHTML = this.templates.rowInner.apply(rowParams);

	this.fireEvent('rowupdated', this, rowIndex, record);
};
// 修正EditorGridPanel对复合属性的支持。
Ext.grid.EditorGridPanel.prototype.preEditValue = function(r, field) {
	var value = r.get(field);// 原为r.data[field];
	return this.autoEncode && Ext.isString(value) ? Ext.util.Format
			.htmlDecode(value) : value;
};
/*
 * Ext.override(Ext.Window, { initComponent : Ext.Window.prototype.initComponent
 * .createInterceptor(function() { if (!this.tbar) { this.tbar = []; } if
 * (Ext.isArray(this.tbar)) { this.tbar.push('->'); this.tbar.push({ text :
 * '关闭', iconCls : 'close', scope : this, handler : function() {
 * this[this.closeAction](); } }); } else { this.tbar.add('->'); this.tbar.add({
 * text : '关闭', iconCls : 'close', scope : this, handler : function() {
 * this[this.closeAction](); } }); } }) });
 */
Ext.ns('com.bitc.xmis.util');
/**
 * @class com.bitc.xmis.util.AlertUtil 公用的提示工具类
 * @singleton
 */
com.bitc.xmis.util.AlertUtil = function() {
	return {
		/**
		 * FormPanel提交中失败提示
		 * 
		 * @param action
		 *            {Ext.form.Action}
		 */
		formSubmitFailedAlert : function(action) {
			switch (action.failureType) {
				case Ext.form.Action.CLIENT_INVALID :
					Ext.Msg.show({
								icon : Ext.Msg.ERROR,
								buttons : Ext.MessageBox.OK,
								title : '错误',
								msg : '存在必填项未填写或者数据不符合要求。<br />请注意页面红色提示，按要求重要输入后再试。',
								width : 300
							});
					break;
				case Ext.form.Action.CONNECT_FAILURE :
					Ext.Msg.show({
								icon : Ext.Msg.WARNING,
								buttons : Ext.MessageBox.OK,
								title : '操作失败',
								msg : '连接超时',
								width : 300
							});
					break;
				case Ext.form.Action.SERVER_INVALID :
					Ext.Msg.show({
								icon : Ext.Msg.WARNING,
								buttons : Ext.MessageBox.OK,
								title : action.result.messageTitle,
								msg : action.result.message,
								width : 300
							});
			}
		}
	}
}();
/**
 * @class com.bitc.xmis.util.ExtUtil Extjs方面的工具类
 * @singleton
 */
com.bitc.xmis.util.ExtUtil = function() {
	return {
		encodeRecords : function(records) {
			var data = [], i = 0;
			Ext.iterate(records, function(item) {
						data.push(item.data);
					});
			return Ext.encode(data);
		},
		/**
		 * encode Store中的数据
		 * 
		 * @param store
		 *            {Ext.data.Store}
		 * @return {String} The JSON string
		 */
		encodeStoreData : function(store) {
			return Ext.encode(com.bitc.xmis.util.ExtUtil.getStoreData(store));
		},
		/**
		 * @param store
		 *            {Ext.data.Store}
		 * @return {Object} the data of the store
		 */
		getStoreData : function(store) {
			var data = [], i = 0;
			store.each(function(record) {
						data[i++] = record.data;
					});
			return data;
		},
		/**
		 * encode Store中的被修改的数据
		 * 
		 * @param store
		 *            {Ext.data.Store}
		 * @return {String} The JSON string
		 */
		encodeStoreModifiedData : function(store) {
			return Ext.encode(com.bitc.xmis.util.ExtUtil
					.getStoreModifiedData(store));
		},
		getStoreModifiedData : function(store) {
			var data = [], i = 0;
			var records = store.getModifiedRecords();
			Ext.iterate(records, function(item) {
						data.push(item.data);
					});
			return data;
		},
		/**
		 * 替换filed为displayfied,保留value,fieldLabel
		 * 
		 * @augments 例如：Ext.getCmp('contractNameId')
		 * 
		 * 当要替换的field格式特殊或field的值需要格式化等，需要在field处提供getAsycnValue方法，方法格式如下：
		 * 
		 * <pre><code>
		 * getAsycnValue : function(callback) {
		 * 		callback.call(this, Ext.util.Format.date(this.getValue(),this.format));
		 * }
		 * </code></pre>
		 * 
		 */
		disableField : function(field1, field2) {
			if (arguments && arguments.length) {
				for (var i = 0; i < arguments.length; i++) {
					var field = arguments[i];
					var dfield = new Ext.form.DisplayField({
								value : field.getValue ? field.getValue() : '',
								fieldLabel : field.fieldLabel
							});
					if (field.getAsycnValue) {
						field.getAsycnValue(function(f) {
									return function(v) {
										f.setValue(v);
									}
								}(dfield));
					}
					var index = field.ownerCt.items.indexOf(field);
					field.hide();
					field.ownerCt.insert(index + 1, dfield);
					field.ownerCt.doLayout();
				}
			}
		}
	}
}();
/**
 * 合同摘要Format工具（合同列表，合同列表导出）
 * 
 * @param contractSummary
 *            合同摘要
 * @param summarystencil
 *            摘要模板
 * @param color
 *            颜色 例:'red'
 */
com.bitc.xmis.util.SummaryFormatUtil = function(contractSummary,
		summarystencil, color) {
	return SummaryFormatUtil(contractSummary, summarystencil, color);
};
/**
 * @class com.bitc.xmis.util.OrgUtil 组织机构工具类
 * @singleton
 */
com.bitc.xmis.util.OrgUtil = function() {
	return {
		/**
		 * @param {String}
		 *            roles
		 * @param {String}
		 *            role
		 */
		hasRole : function(role) {
			var roles = __emp.roles;
			return roles && roles.indexOf(role) >= 0;
		}
	}
}();
/**
 * @class com.bitc.xmis.util.RemoteProvider
 * @extends Ext.state.Provider 远程状态保持
 * @param {Object}
 *            config The config object
 */
com.bitc.xmis.util.RemoteProvider = function(config) {
	com.bitc.xmis.util.RemoteProvider.superclass.constructor.call(this);
	this.readUrl = '/';
	this.saveUrl = '/';
	this.paramName = 'extjsstate';
	Ext.apply(this, config);
	this.isInit = false;
	this.addEvents('init');
	this.readState();
};
Ext.extend(com.bitc.xmis.util.RemoteProvider, Ext.state.Provider, {

			set : function(name, value) {
				if (typeof value == "undefined" || value === null) {
					this.clear(name);
					return;
				}
				com.bitc.xmis.util.RemoteProvider.superclass.set.call(this,
						name, value);
			},

			getState : function() {
				return this.state;
			},

			/**
			 * 保存状态到后台
			 */
			saveState : function(cb, scope) {
				var params = {};
				params[this.paramName] = encodeURI(Ext.encode(this.getState()));
				Ext.Ajax.request({
							url : this.saveUrl,
							params : params,
							scope : this,
							success : function(response) {
								if (cb) {
									cb.call(scope || window);
								}
							},
							failure : function() {
								if (cb) {
									cb.call(scope || window);
								}
							}
						});
			},
			/**
			 * 从后台读取状态，初始化的时候调用一次就可以了
			 */
			readState : function() {
				Ext.Ajax.request({
							url : this.readUrl,
							scope : this,
							success : function(response) {
								this.state = Ext
										.decode(decodeURI(response.responseText));
								this.isInit = true;
								this.fireEvent('init');
							},
							failure : function() {
							}
						});
			}

		});
Ext.override(Ext.Component, {
			stateful : false
		});
Ext.override(Ext.grid.GridPanel, {
			stateEvents : ['columnresize', 'columnmove']
		});
/**
 * 状态管理器的单例
 * 
 * @type com.bitc.xmis.util.RemoteProvider
 */
com.bitc.xmis.util.RemoteProvider.single = parent.xMis
		? parent.com.bitc.xmis.util.RemoteProvider.single
		: new com.bitc.xmis.util.RemoteProvider({
					readUrl : __cxtPath + '/loginAction.do?method=getUserHabit',
					saveUrl : __cxtPath
							+ '/loginAction.do?method=saveUserHabit'
				});
Ext.state.Manager.setProvider(com.bitc.xmis.util.RemoteProvider.single);

Ext.ns('Ext.ux');
Ext.ux.ExtendedTabPanel = Ext.extend(Ext.TabPanel, {
			initComponent : function() {
				Ext.ux.ExtendedTabPanel.superclass.initComponent.call(this);
				this.addEvents('tabdblclick');
			},
			initEvents : function() {
				Ext.ux.ExtendedTabPanel.superclass.initEvents.call(this);
				this.mon(this.strip, 'dblclick', function(e) {
							var t = this.findTargets(e);
							if (!t.close) {
								this.fireEvent('tabdblclick');
							}
						}, this);
			}
		});
Ext.reg('extendedtabpanel', Ext.ux.ExtendedTabPanel);
/**
 * @class Ext.ux.PagelimitCombo
 * @extends Object 用于PagingToolbar的扩展，使用户可以定制每一页显示的记录数
 * @plugins
 */
Ext.ux.PagelimitCombo = Ext.extend(Object, {
			width : 50,
			/**
			 * @cfg {Array} defaultData 选择框里默认的选择项。整型数组。（默认为[20, 30, 40, 50]）
			 */
			defaultData : [20, 30, 40, 50],
			/**
			 * @constructor
			 * @param {Object}
			 *            config The config object
			 */
			constructor : function(config) {
				if (config) {
					Ext.apply(this, config);
				}
			},
			// private
			init : function(parent) {
				this.parent = parent;
				this.combo = new Ext.form.ComboBox({
							store : this.defaultData,
							width : this.width,
							typeAhead : true,
							editable : true,
							triggerAction : 'all',
							value : parent.pageSize,
							onKeyUp : function(e) {
								var k = e.getKey();
								if (this.editable !== false
										&& this.readOnly !== true
										&& (k == e.BACKSPACE || !e
												.isSpecialKey())) {
									this.lastKey = k;
								}
								Ext.form.ComboBox.superclass.onKeyUp.call(this,
										e);
							}
						});
				this.combo.on('select', function(combo) {
							var value = combo.getRawValue();
							if (isNaN(value)) {
								Ext.Msg.alert("提示", "请输入数字。", function() {
											combo.focus(true);
										});
								return;
							}

							if (this.parent.store.lastOptions == undefined) {
								this.parent.limitCount = value;
							} else {
								Ext.apply(this.parent.store.lastOptions.params,
										{
											limit : value
										});
							}
							this.parent.store.baseParams['limit'] = value;
							this.parent.pageSize = parseInt(value);
							this.parent.moveFirst();
						}, this);
				this.combo.on('specialkey', function(f, e) {
							if (e.getKey() == e.ENTER) {
								this.combo.fireEvent('select', this.combo);
							}
						}, this);
				var index = parent.items.indexOf(parent.refresh);
				parent.insertButton(index + 1, ['-', '每页', this.combo, '条记录']);
				parent.doLayout();
			}
		});
Ext.ux.PagelimitComboForRememberPageSize = Ext.extend(Object, {
			rememberPage : 20,
			width : 50,
			/**
			 * @cfg {Array} defaultData 选择框里默认的选择项。整型数组。（默认为[20, 30, 40, 50]）
			 */
			defaultData : [20, 30, 40, 50],
			/**
			 * @constructor
			 * @param {Object}
			 *            config The config object
			 */
			constructor : function(config) {
				if (config) {
					Ext.apply(this, config);
				}
			},
			// private
			init : function(parent) {
				this.parent = parent;
				this.combo = new Ext.form.ComboBox({
							store : this.defaultData,
							width : this.width,
							typeAhead : true,
							editable : true,
							triggerAction : 'all',
							value : parent.pageSize,
							onKeyUp : function(e) {
								var k = e.getKey();
								if (this.editable !== false
										&& this.readOnly !== true
										&& (k == e.BACKSPACE || !e
												.isSpecialKey())) {
									this.lastKey = k;
								}
								Ext.form.ComboBox.superclass.onKeyUp.call(this,
										e);
							}
						});
				this.combo.on('render', function(comboBox) {
							var pageSize = this.rememberPage;
							comboBox.setValue(pageSize);
							if (this.parent.store.lastOptions == undefined) {
								this.parent.limitCount = pageSize;
							} else {
								Ext.apply(this.parent.store.lastOptions.params,
										{
											limit : pageSize
										});
							}
							this.parent.store.baseParams['limit'] = pageSize;
							this.parent.pageSize = parseInt(pageSize);
							this.parent.moveFirst();
						}, this);
				this.combo.on('select', function(combo) {
							var value = combo.getRawValue();
							if (isNaN(value)) {
								Ext.Msg.alert("提示", "请输入数字。", function() {
											combo.focus(true);
										});
								return;
							}

							if (this.parent.store.lastOptions == undefined) {
								this.parent.limitCount = value;
							} else {
								Ext.apply(this.parent.store.lastOptions.params,
										{
											limit : value
										});
							}
							this.parent.store.baseParams['limit'] = value;
							this.parent.pageSize = parseInt(value);
							this.parent.moveFirst();
						}, this);
				this.combo.on('specialkey', function(f, e) {
							if (e.getKey() == e.ENTER) {
								this.combo.fireEvent('select', this.combo);
							}
						}, this);
				var index = parent.items.indexOf(parent.refresh);
				parent.insertButton(index + 1, ['-', '每页', this.combo, '条记录']);
				parent.doLayout();
			}
		});
Ext.ns('Ext.ux.util');
Ext.ux.util.Export2Excel = function() {
	var startExpt = function(store, cm) {
		return function() {
			var oXL, oWB, oSheet;
			try {
				oXL = new ActiveXObject("Excel.Application");
				oWB = oXL.Workbooks.Add();
				oSheet = oWB.ActiveSheet;
			} catch (e) {
				Ext.Msg
						.alert(
								"导出错误",
								"请确认：<ul><li>您电脑上已经安装Excel</li><li>Internet选项=>安全=>启用\"对未标记为可安全执行的脚本的ActiveX控件初始化并执行\"</li></ul>");
				return;
			}
			var hiddenCount = 0;
			for (var i = 0; i < cm.getColumnCount(); i++) {// i列号
				if (cm.isHidden(i)) {
					hiddenCount++;
					continue;
				}
				oSheet.Cells(1, i + 1 - hiddenCount).value = Ext.util.Format
						.stripTags(cm.getColumnHeader(i));
				var col = cm.getColumnAt(i);
				for (var j = 0; j < store.getCount(); j++) {// j行号
					var r = store.getAt(j);
					// value, metaData, record, rowIndex, colIndex, store
					var v = col.renderer.call(col.scope, r.get(col.dataIndex),
							{}, r, j, i, store);
					oSheet.Cells(2 + j, i + 1 - hiddenCount).value = Ext.util.Format
							.stripTags(v);
				}
			}
			Ext.MessageBox.hide();
			oXL.DisplayAlerts = false;
			oXL.Save();
			oXL.DisplayAlerts = true;
			oXL.Quit();
			oXL = null;
			CollectGarbage();
		};
	};
	return {
		expt : function(store, cm) {
			setTimeout(startExpt(store, cm), 500);
		}
	};
}();

Ext.ux.Export2ExcelButton = Ext.extend(Object, {
	constructor : function(config) {
		if (config) {
			Ext.apply(this, config);
		}
	},
	init : function(parent) {
		this.parent = parent;
		this.button = new Ext.Button({
					text : this.text || '导出数据',
					menu : {
						items : [{
							text : '导出本页数据',
							handler : this.export2Excel.createDelegate(this,
									[true])
						}, {
							text : '导出所有数据',
							handler : this.export2Excel.createDelegate(this,
									[false])
						}]
					}
				});
		var index = parent.items.indexOf(parent.refresh);
		parent.insertButton(index + 1, ['-', this.button]);
		parent.doLayout();
	},
	export2Excel : function(isCurrentData) {
		Ext.MessageBox.show({
					title : '请稍候',
					msg : '正在导出...',
					progressText : '初始化中...',
					width : 300,
					wait : true,
					waitConfig : {
						interval : 200
					},
					closable : false
				});
		var store = this.parent.store;
		var grid = Ext.getCmp(this.parent.el.parent('.x-grid-panel').id);
		if (!isCurrentData) {
			store = new Ext.data.Store({
						proxy : new Ext.data.HttpProxy({
									url : this.parent.store.proxy.url
								}),
						reader : this.parent.store.reader,
						remoteSort : this.parent.store.remoteSort,
						sortInfo : this.parent.store.sortInfo
					});
			for (p in this.parent.store.baseParams) {
				if (p == 'limit') {
					store.setBaseParam('limit', 0);
				} else {
					store.setBaseParam(p, this.parent.store.baseParams[p]);
				}
			}
			var params = {};
			if (this.parent.store.lastOptions
					&& this.parent.store.lastOptions.params) {
				for (p in this.parent.store.lastOptions.params) {
					if (p == 'start' || p == 'limit') {
						params[p] = 0;
					} else {
						params[p] = this.parent.store.lastOptions.params;
					}
				}
			}
			store.on('load', function(s, records) {
						Ext.ux.util.Export2Excel.expt(s, grid.getColumnModel());
					}, this);
			store.load({
						params : params
					});
		} else {
			Ext.ux.util.Export2Excel.expt(store, grid.getColumnModel());
		}
	}
});
/**
 * 
 * @class Ext.ux.Export
 * @extends Object
 * 
 * @example
 * <code>
 * 	<pre>
 * 	为列定义exportFormater,定义导出的样式的配置
 *  有四个参数分别是
 *  此列的值value;
 *  一行的数据 row;
 *  行数index;
 *  页面信息 pageinfo;
 * </pre>
 * </code>
 */
Ext.ux.Export = Ext.extend(Object, {
	constructor : function(config) {
		if (config) {
			Ext.apply(this, config);
		}
	},
	init : function(parent) {
		this.parent = parent;
		this.button = new Ext.Button({
					text : this.text || '导出数据',
					iconCls : 'xls',
					menu : {
						items : [{
							text : '导出本页数据',
							handler : this.onExport.createDelegate(this, [
											undefined, this.exportname || '',
											true])
						}, {
							text : '导出所有数据',
							handler : this.onExport.createDelegate(this, [
											undefined, this.exportname || '',
											false])
						}]
					}
				});
		var index = parent.items.indexOf(parent.refresh);
		parent.insertButton(index + 1, ['-', this.button]);
		parent.doLayout();
	},
	onExport : function(grid, exportname, currentPage) {
		if (!grid) {
			grid = Ext.getCmp(this.parent.el.parent('.x-grid-panel').id);
		}
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
		this.store = grid.store;
		var storeUrl = this.store.proxy.url;
		var url = String.format('{0}/export.do?method=excel', __cxtPath);
		// var columns = grid.getColumnModel().getColumnsBy(function(c) {
		// return !c.hidden;
		// });
		var columns = grid.getColumnModel().config;

		var cms = new Array();
		var fields = this.store.fields;
		grid.getView().beforeColMenuShow();// gridview 拼接表头方法
		for (var i = 0; i < columns.length; i++) {
			var col = columns[i];
			if (col.hidden === true) {// 列是否可见
				continue;
			}
			var header = col.header ? col.header : '';
			if (!grid.getColumnModel().isMenuDisabled(i)) { // 当列的菜单没有被禁用
				header = grid.getView().colMenu.get('col-' + col.id).text; // text
				// 是
				// group的表头与列名的拼接
			}
			if (col.dataIndex || col.dataIndex == '') {
				var field = fields.get(col.dataIndex.split('.')[0]);
				var c = {
					dataIndex : col.dataIndex,
					header : header,
					name : field ? field.name : '',
					mapping : field ? (field.mapping == null
							? field.name
							: field.mapping) : '',
					width : col.width
				};
				if (col.exportFormater) { // 当页面的某一列配置exportFormater定义导出的样式
					c.exportFormater = col.exportFormater.toString();
				} else {
					c.exportFormater = null;
				}
				cms.push(c);
			}
		}
		var params = {
			name : exportname,
			storeUrl : storeUrl,
			pageSize : this.store.baseParams.limit,
			cms : Ext.encode(cms),
			formater : Ext.encode(this.formater || [])
		};
		if (this.store.baseParams) {
			Ext.apply(params, this.store.baseParams);
		}
		if (this.store.lastOptions && this.store.lastOptions.params) {
			Ext.apply(params, this.store.lastOptions.params);
		}

		var pn = this.store.paramNames;
		if (this.store.sortInfo && this.store.remoteSort) {
			params[pn.sort] = this.store.sortInfo.field;
			params[pn.dir] = this.store.sortInfo.direction;
		}
		if (!currentPage && params[pn.limit]) {
			params[pn.start] = 0;
			params[pn.limit] = this.store.getTotalCount();
		}
		Ext.Ajax.request({
					url : url,
					params : params,
					timeout : 2 * 60 * 1000,
					success : function(response, opts) {
						var result = Ext.decode(response.responseText);
						if (result.success) {
							var path = encodeURI(encodeURI(result.attributes.downloadpath));
							// window.open(__cxtPath + path);
							location.href = __cxtPath + path;
							Ext.Msg.hide();
						} else {
							Ext.Msg.alert(result.messageTitle, result.message);
						}
					},
					failure : function() {
					}
				});
	}
});

Ext.ns('com.bitc.xmis');
/**
 * @class com.bitc.xmis.Request
 *        相当于Java中的HttpSevletRequest对象。使得在JS中可以获取request中的参数。 <br>
 *        示例：
 * 
 * <pre>
 * 在JSP页面中使用如下代码输出request对象
 * <code>
 * &lt;%
 *  com.bitc.jk.xmis.util.WebUtil.outputRequestParameterAsScript(request,out,&quot;request&quot;);
 *  %&gt;
 * </code>
 * 第三个参数为生成的com
 * .bitc.xmis.Request对象的变量名。此段代码应该放置在所有Extjs类库加载之后
 * 在自己的JS中就可以这样获取request中的参数值
 * <code>
 * var projectId = request.getParameter('projectId');
 * </code>
 * </pre>
 * 
 * @param {Object}
 *            parameters 初始化参数
 */
com.bitc.xmis.Request = function(parameters) {
	this.parameters = parameters;
};
/**
 * 获取请求中对应参数的值
 * 
 * @param {String}
 *            name 参数名
 * @return {String|Array} 如果一个参数对应唯一的值，则返回一个String类型的结果，否则返回一个数组，包含对应的所有值
 */
com.bitc.xmis.Request.prototype.getParameter = function(name) {
	var values = this.parameters[name];
	if (Ext.isArray(values)) {
		return values.length > 1 ? values : values[0];
	} else if (values) {
		return values;
	}
	return null;
};
Ext.ns('com.bitc.xmis.Combos');
/**
 * @class com.bitc.xmis.Combos.CurrencyCombo 币种选择器
 * @extends Ext.form.ComboBox
 */
com.bitc.xmis.Combos.CurrencyCombo = Ext.extend(Ext.form.ComboBox, {
			valueField : 'id',
			displayField : 'name',
			triggerAction : 'all',
			lazyInit : false,
			mode : 'remote',
			emptyText : '选择币种',
			editable : false,
			value : 1,
			initComponent : function() {
				this.addEvents('setValueEvent');
				this.store = new Ext.data.JsonStore({
							url : String.format(
									'{0}/sys.do?method=getCurrency', __cxtPath),
							root : 'rows',
							autoLoad : true,
							fields : ['id', 'name', {
										name : 'rateOfExchange2RMB',
										type : 'double'
									}, 'symbol'],
							listeners : {
								scope : this,
								// 解决在Form中setValue时显示的是valueField而不是displayField的问题
								'load' : function() {
									this.setValue(this.value);
									this.fireEvent('setValueEvent', this);
								}
							}
						});
				com.bitc.xmis.Combos.CurrencyCombo.superclass.initComponent
						.call(this);
			}
		});

com.bitc.xmis.Combos.BusinessCombo = Ext.extend(Ext.form.ComboBox, {
			valueField : 'id',
			displayField : 'name',
			triggerAction : 'all',
			lazyInit : false,
			mode : 'remote',
			emptyText : '选择物资业务',
			editable : false,
			value : 1,
			initComponent : function() {

				this.addEvents('setValueEvent');
				this.store = new Ext.data.JsonStore({
							url : String.format(
									'{0}/sys.do?method=getBusiness', __cxtPath),
							root : 'rows',
							autoLoad : true,
							fields : ['id', 'name'],
							listeners : {
								scope : this,
								// 解决在Form中setValue时显示的是valueField而不是displayField的问题
								'load' : function() {
									this.setValue(this.value);
									this.fireEvent('setValueEvent', this);
								}
							}
						});
				com.bitc.xmis.Combos.BusinessCombo.superclass.initComponent
						.call(this);

			}
		});

/**
 * @class com.bitc.xmis.Combos.PowerCombo 功率单位选择器
 * @extends Ext.form.ComboBox
 */
com.bitc.xmis.Combos.PowerCombo = Ext.extend(Ext.form.ComboBox, {
			valueField : 'id',
			displayField : 'name',
			triggerAction : 'all',
			lazyInit : false,
			mode : 'remote',
			emptyText : '选择功率单位',
			editable : false,
			initComponent : function() {
				this.addEvents('setValueEvent');
				this.store = new Ext.data.JsonStore({
							url : String.format('{0}/sys.do?method=getPower',
									__cxtPath),
							root : 'rows',
							autoLoad : true,
							fields : ['id', 'name'],
							listeners : {
								scope : this,
								// 解决在Form中setValue时显示的是valueField而不是displayField的问题
								'load' : function() {
									this.setValue(this.value);
									this.fireEvent('setValueEvent', this);
								}
							}
						});
				com.bitc.xmis.Combos.PowerCombo.superclass.initComponent
						.call(this);

			}
		});
/**
 * @class com.bitc.xmis.Combos.WeightCombo 重量单位选择器
 * @extends Ext.form.ComboBox
 */
com.bitc.xmis.Combos.WeightCombo = Ext.extend(Ext.form.ComboBox, {
			valueField : 'id',
			displayField : 'name',
			triggerAction : 'all',
			lazyInit : false,
			mode : 'remote',
			emptyText : '选择重量单位',
			editable : false,
			initComponent : function() {

				this.addEvents('setValueEvent');
				this.store = new Ext.data.JsonStore({
							url : String.format('{0}/sys.do?method=getWeight',
									__cxtPath),
							root : 'rows',
							autoLoad : true,
							fields : ['id', 'name'],
							listeners : {
								scope : this,
								// 解决在Form中setValue时显示的是valueField而不是displayField的问题
								'load' : function() {
									this.setValue(this.value);
									this.fireEvent('setValueEvent', this);
								}
							}
						});
				com.bitc.xmis.Combos.WeightCombo.superclass.initComponent
						.call(this);

			}
		});
/**
 * @class com.bitc.xmis.Combos.BusinessCombo 物资业务选择器
 * @extends Ext.form.ComboBox
 */

/**
 * @class com.bitc.xmis.Combos.BusinessCombo 物资自身业务选择器
 * @extends Ext.form.ComboBox
 */
com.bitc.xmis.Combos.PropertyCombo = Ext.extend(Ext.form.ComboBox, {
			valueField : 'id',
			displayField : 'name',
			triggerAction : 'all',
			lazyInit : false,
			mode : 'remote',
			emptyText : '选择物资自身属性',
			editable : false,
			value : 1,
			initComponent : function() {

				this.addEvents('setValueEvent');
				this.store = new Ext.data.JsonStore({
							url : String.format(
									'{0}/sys.do?method=getMaterialsProperty',
									__cxtPath),
							root : 'rows',
							autoLoad : true,
							fields : ['id', 'name'],
							listeners : {
								scope : this,
								// 解决在Form中setValue时显示的是valueField而不是displayField的问题
								'load' : function() {
									this.setValue(this.value);
									this.fireEvent('setValueEvent', this);
								}
							}
						});
				com.bitc.xmis.Combos.PropertyCombo.superclass.initComponent
						.call(this);
			}
		});
/**
 * @class com.bitc.xmis.Combos.BusinessCombo 物资用途选择器
 * @extends Ext.form.ComboBox
 */
com.bitc.xmis.Combos.MaterialsUseCombo = Ext.extend(Ext.form.ComboBox, {
	valueField : 'id',
	displayField : 'name',
	triggerAction : 'all',
	lazyInit : false,
	mode : 'remote',
	emptyText : '选择物资用途',
	editable : false,
	value : 1,
	initComponent : function() {
		this.addEvents('setValueEvent');
		this.store = new Ext.data.JsonStore({
			url : String.format('{0}/sys.do?method=getMaterialsUse', __cxtPath),
			root : 'rows',
			autoLoad : true,
			fields : ['id', 'name'],
			listeners : {
				scope : this,
				// 解决在Form中setValue时显示的是valueField而不是displayField的问题
				'load' : function() {
					this.setValue(this.value);
					this.fireEvent('setValueEvent', this);
				}
			}
		});
		com.bitc.xmis.Combos.MaterialsUseCombo.superclass.initComponent
				.call(this);
	}
});
/**
 * @class com.bitc.xmis.Combos.ProduceStatusCombo 物资生产状态选择器
 * @extends Ext.form.ComboBox
 */
com.bitc.xmis.Combos.ProduceStatusCombo = Ext.extend(Ext.form.ComboBox, {
			valueField : 'id',
			displayField : 'name',
			triggerAction : 'all',
			lazyInit : false,
			mode : 'remote',
			emptyText : '选择生产状态',
			editable : false,
			value : 1,
			initComponent : function() {

				this.addEvents('setValueEvent');
				this.store = new Ext.data.JsonStore({
							url : String.format(
									'{0}/sys.do?method=getProduceStatus',
									__cxtPath),
							root : 'rows',
							autoLoad : true,
							fields : ['id', 'name'],
							listeners : {
								scope : this,
								// 解决在Form中setValue时显示的是valueField而不是displayField的问题
								'load' : function() {
									this.setValue(this.value);
									this.fireEvent('setValueEvent', this);
								}
							}
						});
				com.bitc.xmis.Combos.ProduceStatusCombo.superclass.initComponent
						.call(this);

			}
		});
/**
 * @class com.bitc.xmis.Combos.ProjectStatusCombo 项目状态选择器
 * @extends Ext.form.ComboBox
 */
com.bitc.xmis.Combos.ProjectStatusCombo = Ext.extend(Ext.form.ComboBox, {
			valueField : 'statusId',
			displayField : 'statusName',
			triggerAction : 'all',
			lazyInit : false,
			mode : 'remote',
			emptyText : '选择项目进度计划节点',
			initComponent : function() {
				var url = String.format(
						'{0}/projectMain.do?method=getProjectSchedulePlan',
						__cxtPath);
				this.store = new Ext.data.JsonStore({
							url : url,
							root : 'rows',
							idProperty : 'id',
							remoteSort : true,
							sortInfo : {
								field : 'startDate',
								direction : 'ASC'
							},
							totalProperty : 'totalCount',
							fields : ['id', {
										name : 'startDate',
										type : 'date',
										dateFormat : 'Y-m-d H:i:s'
									}, {
										name : 'actualStartDate',
										type : 'date',
										dateFormat : 'Y-m-d H:i:s'
									}, {
										name : 'statusId',
										mapping : 'status',
										convert : function(v) {
											return v.id;
										}
									}, {
										name : 'statusName',
										mapping : 'status',
										convert : function(v) {
											return v.name;
										}
									}, {
										name : 'statusDesc',
										mapping : 'status',
										convert : function(v) {
											return v.description;
										}
									}],
							baseParams : this.baseParams || {}
						});
				com.bitc.xmis.Combos.ProjectStatusCombo.superclass.initComponent
						.call(this);
			}
		});

/**
 * @class com.bitc.xmis.Combos.CompanyScaleCombo 公司规模选择器
 * @extends Ext.form.ComboBox
 */
com.bitc.xmis.Combos.CompanyScaleCombo = Ext.extend(Ext.form.ComboBox, {
	valueField : 'companyScaleID',
	displayField : 'companyScaleName',
	triggerAction : 'all',
	lazyInit : false,
	mode : 'remote',
	emptyText : '选择公司规模',
	editable : false,
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
			url : String
					.format(
							'{0}/crmMaint.do?method=getMultiData&&category=companyScale',
							__cxtPath),
			root : 'rows',
			autoLoad : true,
			fields : [{
						name : "companyScaleID",
						type : "int",
						mapping : "companyScaleID"
					}, {
						name : "companyScaleName",
						type : "string",
						mapping : "companyScaleName"
					}],
			listeners : {
				scope : this,
				'load' : function() {
					this.store.insert(0, new this.store.recordType({
										companyScaleID : '-1',
										companyScaleName : '--'
									}));
					this.setValue(this.value);
				}
			}
		});
		com.bitc.xmis.Combos.CompanyScaleCombo.superclass.initComponent
				.call(this);
	}
});

/**
 * @class com.bitc.xmis.Combos.CustomerLevelCombo 客户级别选择器
 * @extends Ext.form.ComboBox
 */
com.bitc.xmis.Combos.CustomerLevelCombo = Ext.extend(Ext.form.ComboBox, {
	valueField : 'id',
	displayField : 'name',
	triggerAction : 'all',
	lazyInit : false,
	mode : 'remote',
	emptyText : '选择客户级别',
	editable : false,
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
			url : String
					.format(
							'{0}/crmMaint.do?method=getMultiData&&category=customerLevel',
							__cxtPath),
			root : 'rows',
			autoLoad : true,
			fields : [{
						name : "id",
						type : "int",
						mapping : "customerLevelID"
					}, {
						name : "name",
						type : "string",
						mapping : "customerLevelName"
					}],
			listeners : {
				scope : this,
				'load' : function() {
					this.store.insert(0, new this.store.recordType({
										id : '-1',
										name : '--'
									}));
					this.setValue(this.value);
				}
			}
		});
		com.bitc.xmis.Combos.CustomerLevelCombo.superclass.initComponent
				.call(this);
	}
});

/**
 * @class com.bitc.xmis.Combos.CustomerLevelCombo 信用等级选择器
 * @extends Ext.form.ComboBox
 */
com.bitc.xmis.Combos.CreditLevelCombo = Ext.extend(Ext.form.ComboBox, {
	valueField : 'id',
	displayField : 'name',
	triggerAction : 'all',
	lazyInit : false,
	mode : 'remote',
	emptyText : '选择信用等级',
	editable : false,
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
			url : String
					.format(
							'{0}/crmMaint.do?method=getMultiData&&category=creditLevel',
							__cxtPath),
			root : 'rows',
			autoLoad : true,
			fields : [{
						name : "id",
						type : "int",
						mapping : "creditLevelID"
					}, {
						name : "name",
						type : "string",
						mapping : "creditLevelName"
					}],
			listeners : {
				scope : this,
				'load' : function() {
					this.store.insert(0, new this.store.recordType({
										id : '-1',
										name : '--'
									}));
					this.setValue(this.value);
				}
			}
		});
		com.bitc.xmis.Combos.CreditLevelCombo.superclass.initComponent
				.call(this);
	}
});

/**
 * @class com.bitc.xmis.Combos.BusinessRelationCombo 业务关系选择器
 * @extends Ext.form.ComboBox
 */
com.bitc.xmis.Combos.BusinessRelationCombo = Ext.extend(Ext.form.ComboBox, {
	valueField : 'id',
	displayField : 'name',
	triggerAction : 'all',
	lazyInit : false,
	mode : 'remote',
	emptyText : '选择业务关系',
	// anchor : '100%',
	msgTarget : 'under',
	editable : false,
	allowBlank : false,
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
			url : String
					.format(
							'{0}/crmMaint.do?method=getMultiData&&category=businessRelation',
							__cxtPath),
			root : 'rows',
			autoLoad : true,
			fields : [{
						name : "id",
						type : "int",
						mapping : "businessRelationID"
					}, {
						name : "name",
						type : "string",
						mapping : "businessRelationName"
					}],
			listeners : {
				scope : this,
				'load' : function() {
					this.setValue(this.value);
				}
			}
		});
		com.bitc.xmis.Combos.BusinessRelationCombo.superclass.initComponent
				.call(this);
	}
});
/**
 * @class com.bitc.xmis.Combos.BusinessRelationCombo 业务关系选择器 查询时候用的
 * @extends Ext.form.ComboBox
 */
com.bitc.xmis.Combos.BusinessRelationComboAll = Ext.extend(Ext.form.ComboBox, {
	valueField : 'id',
	displayField : 'name',
	triggerAction : 'all',
	lazyInit : false,
	mode : 'remote',
	emptyText : '选择业务关系',
	msgTarget : 'under',
	editable : false,
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
			url : String
					.format(
							'{0}/crmMaint.do?method=getMultiData&&category=businessRelation',
							__cxtPath),
			root : 'rows',
			autoLoad : true,
			fields : [{
						name : "id",
						type : "int",
						mapping : "businessRelationID"
					}, {
						name : "name",
						type : "string",
						mapping : "businessRelationName"
					}],
			listeners : {
				scope : this,
				'load' : function() {
					this.store.insert(0, new this.store.recordType({
										id : '-1',
										name : '-全部-'
									}));
					this.setValue(this.value);
				}
			}
		});
		com.bitc.xmis.Combos.BusinessRelationComboAll.superclass.initComponent
				.call(this);
	}
});

com.bitc.xmis.EmpInfoPanel = Ext.extend(Ext.FormPanel, {
	unstyled : true,
	initComponent : function() {
		// items
		com.bitc.xmis.EmpInfoPanel.superclass.initComponent.call(this);
		this.addEvents('loadsuccess');
		this.on('afterrender',
				this.loadData.createDelegate(this, [this.empID]), this);
	},
	loadData : function(empID) {
		if (empID) {
			this.empID = empID;
		}
		var i;
		var ps = new Array();
		for (i = 0; i < this.items.length; i++) {
			ps.push(this.items.itemAt(i).name);
		}
		this.load({
					url : String.format(
							'{0}/crmMaint.do?method=getEmpInfoById', __cxtPath),
					params : {
						empID : this.empID,
						properties : ps.join(',')
					},
					scope : this,
					success : function(form, action) {
						this.fireEvent('loadsuccess', form, action);
					},
					failure : function(form, action) {
						Ext.Msg.alert(action.result.messageTitle,
								action.result.message);
					}
				});
	}
});

/**
 * @class com.bitc.xmis.Combos.CustomerLevelCombo 禁用原因选择器
 * @extends Ext.form.ComboBox
 */
com.bitc.xmis.Combos.DisabledReasonCombo = Ext.extend(Ext.form.ComboBox, {
			valueField : 'id',
			displayField : 'name',
			triggerAction : 'all',
			lazyInit : false,
			mode : 'remote',
			emptyText : '选择禁用原因',
			editable : false,
			initComponent : function() {
				this.store = new Ext.data.JsonStore({
							url : String
									.format(
											'{0}/orgMaint.do?method=getDisabledReasonComb',
											__cxtPath),
							root : 'rows',
							autoLoad : true,
							fields : [{
										name : "id",
										type : "string",
										mapping : "id"
									}, {
										name : "name",
										type : "string",
										mapping : "name"
									}],
							listeners : {
								scope : this,
								'load' : function() {
									this.store.insert(0,
											new this.store.recordType({
														id : '-1',
														name : '--'
													}));
									this.setValue(this.value);
								}
							}
						});
				com.bitc.xmis.Combos.DisabledReasonCombo.superclass.initComponent
						.call(this);
			}
		});

com.bitc.xmis.Combos.ProcessListCombo = Ext.extend(Ext.form.ComboBox, {
	valueField : 'processListID',
	displayField : 'processListName',
	triggerAction : 'all',
	lazyInit : false,
	mode : 'remote',
	emptyText : '选择流程',
	editable : false,
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
			url : String.format(
					'{0}/workflow.do?method=getProcessInstanceComb', __cxtPath),
			root : 'rows',
			autoLoad : true,
			fields : [{
						name : "processListID",
						type : "string",
						mapping : "id"
					}, {
						name : "processListName",
						type : "string",
						mapping : "name"
					}],
			listeners : {
				scope : this,
				'load' : function() {
					this.setValue(this.value);
				}
			}
		});
		com.bitc.xmis.Combos.ProcessListCombo.superclass.initComponent
				.call(this);
	}
});

/**
 * 公告、个人消息窗口
 */
showTopicMessageWinList = function(topicID, userID) {

	var message_ids = new Array();

	var smsStore = new Ext.data.JsonStore({
				url : __cxtPath
						+ '/orgMaint.do?method=getMessageByUserIDAndTopID',
				baseParams : {
					topicID : topicID,
					userID : userID
				},
				root : 'rows',
				fields : [{
							name : 'from_id'
						}, {
							name : 'from_name'
						}, {
							name : 'from_time'
						}, {
							name : 'message_id'
						}, {
							name : 'message_info'
						}, {
							name : 'system_msg_flag'
						}]
			});

	var smsList = new Ext.ListView({
		region : 'center',
		store : smsStore,
		hideHeaders : true,
		columns : [{
			header : '消息',
			dataIndex : 'message_info',
			tpl : new Ext.XTemplate('{[this.getMessage(values.message_info)]}',
					{
						getMessage : function(v) {
							var baseNum = 20;
							var msg = '';
							for (var i = 0; i < v.length / baseNum; i++) {
								msg += v.substr(i * baseNum, baseNum) + "<br/>";
							}
							return msg;
						}
					})
		}, {
			header : '发送时间、发送人',
			dataIndex : 'from_time',
			width : .3,
			align : 'right',
			tpl : '{values.from_time}<br/>{values.from_name}'
		}]
	});

	var smsWin = new Ext.Window({
		title : '消息',
		height : 270,
		width : 430,
		closeAction : 'hide',
		layout : 'border',
		id : 'smswin',
		border : false,
		items : [{
					xtype : 'panel',
					region : 'center',
					layout : 'form',
					autoScroll : true,
					items : [smsList]
				}, {
					xtype : 'panel',
					layout : 'border',
					region : 'south',
					height : 25,
					border : false,
					items : [{
								xtype : 'checkbox',
								region : 'center',
								id : 'smsCheck',
								hideLable : true,
								checked : true,
								boxLabel : '下次不再提示',
								style : 'margin-left:145px'
							}, {
								xtype : 'button',
								region : 'east',
								width : 60,
								text : '关闭',
								style : 'margin-left:-50px',
								handler : function() {
									if (Ext.getCmp('smsCheck').getValue()) {
										Ext.Ajax.request({
											url : __cxtPath
													+ '/orgMaint.do?method=saveUserTopicMessageHabit',
											params : {
												topicID : topicID,
												userID : userID,
												messageids : message_ids
														.join(',')
											},
											success : function(response, opt) {
												smsWin.close();
											},
											failure : function(response, opt) {
												var obj = Ext
														.decode(response.responseText);
												Ext.Msg.alert(obj.messageTitle,
														obj.message);
											}
										});
									} else {
										smsWin.close();
									}
								}
							}

					]
				}]
			// , buttons : ['<font size = "2px">' + '下次是否不再提示' + '</font>', {
			// text : "是",
			// handler : function() {
			// Ext.Ajax.request({
			// url : __cxtPath
			// + '/orgMaint.do?method=saveUserTopicMessageHabit',
			// params : {
			// topicID : topicID,
			// userID : userID,
			// messageids : message_ids.join(',')
			// },
			// success : function(response, opt) {
			// smsWin.close();
			// },
			// failure : function(response, opt) {
			// var obj = Ext.decode(response.responseText);
			// Ext.Msg.alert(obj.messageTitle, obj.message);
			// }
			// });
			// }
			// }, {
			// text : '否',
			// handler : function() {
			// smsWin.close();
			// }
			// }]
	});
	smsStore.load({
				params : {},
				callback : function(record, options, success) {
					if (success && record.length > 0) {
						for (var i = 0; i < record.length; i++) {
							message_ids.push(record[i].data['message_id']);
						}
						smsWin.show();
					}
				}
			});
};

Ext.ns('com.bitc.xmis');
com.bitc.xmis.Roles = {
	startproject : 18
}

com.bitc.xmis.existRole = function(roles,rolenum){
	var result = false;
	for(var i=0;i<roles.length;i++){
		if(rolenum==roles[i]){
			return true;
		}
	}
	return result;
}
Ext.ns('com.bitc.xmis.Combos');
/**
 * 职务级别
 * 
 * @class
 * @extends Ext.form.ComboBox
 */
com.bitc.xmis.Combos.DutyLevelCombo = Ext.extend(Ext.form.ComboBox, {
	valueField : 'ID',
	displayField : 'Name',
	triggerAction : 'all',
	lazyInit : false,
	mode : 'remote',
	emptyText : '选择职务级别',
	editable : false,
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
			url : String
					.format(
							'{0}/sys.do?method=getDutyLevel',
							__cxtPath),
			root : 'rows',
			autoLoad : true,
			fields : [{
						name : "ID",
						type : "int",
						mapping : "id"
					}, {
						name : "Name",
						type : "string",
						mapping : "name"
					}],
			listeners : {
				scope : this,
				'load' : function() {
					this.store.insert(0, new this.store.recordType({
										ID : '-1',
										Name : '--'
									}));
					this.setValue(this.value);
				}
			}
		});
		com.bitc.xmis.Combos.DutyLevelCombo.superclass.initComponent
				.call(this);
			}
		});
/**
 * 技术职务
 * 
 * @class
 * @extends Ext.form.ComboBox
 */
com.bitc.xmis.Combos.EchnicalpositionCombo = Ext.extend(Ext.form.ComboBox, {
	valueField : 'ID',
	displayField : 'Name',
	triggerAction : 'all',
	lazyInit : false,
	mode : 'remote',
	emptyText : '选择技术职务',
	editable : false,
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
			url : String
					.format(
							'{0}/sys.do?method=getEchnicalposition',
							__cxtPath),
			root : 'rows',
			autoLoad : true,
			fields : [{
						name : "ID",
						type : "int",
						mapping : "id"
					}, {
						name : "Name",
						type : "string",
						mapping : "name"
					}],
			listeners : {
				scope : this,
				'load' : function() {
					this.store.insert(0, new this.store.recordType({
										ID : '-1',
										Name : '--'
									}));
					this.setValue(this.value);
				}
			}
		});
		com.bitc.xmis.Combos.EchnicalpositionCombo.superclass.initComponent
				.call(this);
			}
		});
/**
 * 专业技术级别
 * 
 * @class
 * @extends Ext.form.ComboBox
 */
com.bitc.xmis.Combos.ProfessionalLevelCombo = Ext.extend(Ext.form.ComboBox, {
	valueField : 'ID',
	displayField : 'Name',
	triggerAction : 'all',
	lazyInit : false,
	mode : 'remote',
	emptyText : '选择专业技术级别',
	editable : false,
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
			url : String
					.format(
							'{0}/sys.do?method=getProfessionalLevel',
							__cxtPath),
			root : 'rows',
			autoLoad : true,
			fields : [{
						name : "ID",
						type : "int",
						mapping : "id"
					}, {
						name : "Name",
						type : "string",
						mapping : "name"
					}],
			listeners : {
				scope : this,
				'load' : function() {
					this.store.insert(0, new this.store.recordType({
										ID : '-1',
										Name : '--'
									}));
					this.setValue(this.value);
				}
			}
		});
		com.bitc.xmis.Combos.ProfessionalLevelCombo.superclass.initComponent
				.call(this);
			}
		});
/**
 * 现有文化程度
 * 
 * @class
 * @extends Ext.form.ComboBox
 */
com.bitc.xmis.Combos.DegreeCombo = Ext.extend(Ext.form.ComboBox, {
	valueField : 'ID',
	displayField : 'Name',
	triggerAction : 'all',
	lazyInit : false,
	mode : 'remote',
	emptyText : '选择现有文化程度',
	editable : false,
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
			url : String
					.format(
							'{0}/sys.do?method=getDegree',
							__cxtPath),
			root : 'rows',
			autoLoad : true,
			fields : [{
						name : "ID",
						type : "int",
						mapping : "id"
					}, {
						name : "Name",
						type : "string",
						mapping : "name"
					}],
			listeners : {
				scope : this,
				'load' : function() {
					this.store.insert(0, new this.store.recordType({
										ID : '-1',
										Name : '--'
									}));
					this.setValue(this.value);
				}
			}
		});
		com.bitc.xmis.Combos.DegreeCombo.superclass.initComponent
				.call(this);
			}
		});
/**
 * 工种
 * 
 * @class
 * @extends Ext.form.ComboBox
 */
com.bitc.xmis.Combos.WorkTypeCombo = Ext.extend(Ext.form.ComboBox, {
	valueField : 'ID',
	displayField : 'Name',
	triggerAction : 'all',
	lazyInit : false,
	mode : 'remote',
	emptyText : '选择工种',
	editable : false,
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
			url : String
					.format(
							'{0}/sys.do?method=getWorkType',
							__cxtPath),
			root : 'rows',
			autoLoad : true,
			fields : [{
						name : "ID",
						type : "int",
						mapping : "id"
					}, {
						name : "Name",
						type : "string",
						mapping : "name"
					}],
			listeners : {
				scope : this,
				'load' : function() {
					this.store.insert(0, new this.store.recordType({
										ID : '-1',
										Name : '--'
									}));
					this.setValue(this.value);
				}
			}
		});
		com.bitc.xmis.Combos.WorkTypeCombo.superclass.initComponent
				.call(this);
			}
		});
/**
 * 在职类型
 * 
 * @class
 * @extends Ext.form.ComboBox
 */
com.bitc.xmis.Combos.InServiceTypeCombo = Ext.extend(Ext.form.ComboBox, {
	valueField : 'ID',
	displayField : 'Name',
	triggerAction : 'all',
	lazyInit : false,
	mode : 'remote',
	emptyText : '选择在职类型',
	editable : false,
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
			url : String
					.format(
							'{0}/sys.do?method=getInServiceType',
							__cxtPath),
			root : 'rows',
			autoLoad : true,
			fields : [{
						name : "ID",
						type : "int",
						mapping : "id"
					}, {
						name : "Name",
						type : "string",
						mapping : "name"
					}],
			listeners : {
				scope : this,
				'load' : function() {
					this.store.insert(0, new this.store.recordType({
										ID : '-1',
										Name : '--'
									}));
					this.setValue(this.value);
				}
			}
		});
		com.bitc.xmis.Combos.InServiceTypeCombo.superclass.initComponent
				.call(this);
			}
		});
com.bitc.xmis.Combos.IdentityObjCombo = Ext.extend(Ext.form.ComboBox, {
	valueField : 'ID',
	displayField : 'Name',
	triggerAction : 'all',
	lazyInit : false,
	mode : 'remote',
	emptyText : '选择身份',
	editable : false,
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
			url : String
					.format(
							'{0}/sys.do?method=getIdentityObjCombo',
							__cxtPath),
			root : 'rows',
			autoLoad : true,
			fields : [{
						name : "ID",
						type : "int",
						mapping : "id"
					}, {
						name : "Name",
						type : "string",
						mapping : "name"
					}],
			listeners : {
				scope : this,
				'load' : function() {
					this.store.insert(0, new this.store.recordType({
										ID : '-1',
										Name : '--'
									}));
					this.setValue(this.value);
				}
			}
		});
		com.bitc.xmis.Combos.IdentityObjCombo.superclass.initComponent
				.call(this);
			}
		});
/**
 * 办公用品类别
 * 
 */
		com.bitc.xmis.Combos.OfficeSuppliesCombo = Ext.extend(Ext.form.ComboBox, {
			valueField : 'supplies_type_id',
			displayField : 'supplies_type_Name',
			triggerAction : 'all',
			lazyInit : false,
			
			mode : 'remote',
			emptyText : '选择办公用品类别',
			editable : false,
			initComponent : function() {

				this.addEvents('setValueEvent');
				this.store = new Ext.data.JsonStore({
							url : String.format(
									'{0}/sys.do?method=getOfficeSuppliesCombo', __cxtPath),
							root : 'rows',
							autoLoad : true,
							fields : ['supplies_type_id', 'supplies_type_Name'],
							listeners : {
								scope : this,
								// 解决在Form中setValue时显示的是valueField而不是displayField的问题
								'load' : function() {
									this.setValue(this.value);
									this.fireEvent('setValueEvent', this);
								}
							}
						});
				com.bitc.xmis.Combos.OfficeSuppliesCombo.superclass.initComponent
						.call(this);

			}
		});
		
com.bitc.xmis.Combos.OfficeSuppliesCombo1 = Ext.extend(Ext.form.ComboBox, {
			valueField : 'supplies_type_id',
			displayField : 'supplies_type_Name',
			triggerAction : 'all',
//			lazyInit : false,
			
			mode : 'remote',
//			emptyText : '选择办公用品类别',
			editable : false,
			initComponent : function() {

				this.addEvents('setValueEvent');
				this.store = new Ext.data.JsonStore({
							url : String.format(
									'{0}/crmMaint.do?method=getOfficeSuppliesCombot', __cxtPath),
							root : 'rows',
							autoLoad : true,
							fields : ['supplies_type_id', 'supplies_type_Name'],
							listeners : {
								scope : this,
								// 解决在Form中setValue时显示的是valueField而不是displayField的问题
								'load' : function() {
									this.setValue(this.value);
									this.fireEvent('setValueEvent', this);
								}
							}
						});
				com.bitc.xmis.Combos.OfficeSuppliesCombo1.superclass.initComponent
						.call(this);

			}
		});		
		
		
		

com.bitc.xmis.Combos.DocumentTypeInfoCombo = Ext.extend(Ext.form.ComboBox, {
	valueField : 'id',
	displayField : 'name',
	triggerAction : 'all',
	lazyInit : false,
	mode : 'remote',
	emptyText : '选择公文类别',
	editable : false,
	initComponent : function() {
		this.store = new Ext.data.JsonStore({
					url : String
							.format(
									'{0}/applyManageMaint.do?method=getDocumentTypeInfoCombo',
									__cxtPath),
					root : 'rows',
					autoLoad : true,
					fields : [{
								name : "id",
								type : "int",
								mapping : "document_type_id"
							}, {
								name : "name",
								type : "string",
								mapping : "document_type_Name"
							}],
					listeners : {
						scope : this,
						'load' : function() {
							this.setValue(this.value);
						}
					}
				});
		com.bitc.xmis.Combos.DocumentTypeInfoCombo.superclass.initComponent
				.call(this);
	}
});
		