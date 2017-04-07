/*
 * ! Ext JS Library 3.2.0 Copyright(c) 2006-2010 Ext JS, Inc.
 * licensing@extjs.com http://www.extjs.com/license
 */
Ext.ns('Ext.ux.grid');

/**
 * @class Ext.ux.grid.CheckColumn
 * @extends Object GridPanel plugin to add a column with check boxes to a grid.
 *          <p>
 *          Example usage:
 *          </p>
 * 
 * <pre><code>
 *  // create the column
 *  var checkColumn = new Ext.grid.CheckColumn({
 *  	header: 'Indoor?',
 *  	dataIndex: 'indoor',
 *  	id: 'check',
 *  	width: 55
 *  });
 * 
 *  // add the column to the column model
 *  var cm = new Ext.grid.ColumnModel([{
 *  		header: 'Foo',
 *  		...
 *  	},
 *  	checkColumn
 *  ]);
 * 
 *  // create the grid
 *  var grid = new Ext.grid.EditorGridPanel({
 *  	...
 *  	cm: cm,
 *  	plugins: [checkColumn], // include plugin
 *  	...
 *  });
 * </code></pre>
 * 
 * In addition to storing a Boolean value within the record data, this class
 * toggles a css class between <tt>'x-grid3-check-col'</tt> and
 * <tt>'x-grid3-check-col-on'</tt> to alter the background image used for a
 * column.
 */
Ext.ux.grid.CheckColumn = Ext.extend(Ext.util.Observable, {
			constructor : function(config) {
				Ext.apply(this, config || {});
				this.addEvents(
						/**
						 * @event beforechange 选择状态改变前触发
						 * @param {Boolean}
						 *            newvalue 改变后的值
						 * @param {Ext.data.Record}
						 *            record 当前改变的record
						 * @param {Ext.grid.grid}
						 *            grid 当前的grid
						 */
						'beforechange',
						/**
						 * @event change 选择状态改变后触发
						 * @param {Boolean}
						 *            newvalue 改变后的值
						 * @param {Ext.data.Record}
						 *            record 当前改变的record
						 * @param {Ext.grid.grid}
						 *            grid 当前的grid
						 */
						'change');
				Ext.ux.grid.CheckColumn.superclass.constructor.call(this);
			},
			init : function(grid) {
				this.grid = grid;
				this.grid.on('render', function() {
							var view = this.grid.getView();
							view.mainBody.on('mousedown', this.onMouseDown,
									this);
						}, this);
			},

			onMouseDown : function(e, t) {
				if (Ext.fly(t).hasClass(this.createId())) {
					e.stopEvent();
					var index = this.grid.getView().findRowIndex(t);
					var record = this.grid.store.getAt(index);
					if (this.fireEvent('beforechange',
							!record.data[this.dataIndex], record, this.grid)) {
						record
								.set(this.dataIndex,
										!record.data[this.dataIndex]);
						this.fireEvent('change', record.data[this.dataIndex],
								record, this.grid);
					}
				}
			},

			renderer : function(v, p, record) {
				p.css += ' x-grid3-check-col-td';
				return String.format(
						'<div class="x-grid3-check-col{0} {1}">&#160;</div>', v
								? '-on'
								: '', this.createId());
			},

			createId : function() {
				return 'x-grid3-cc-' + this.id;
			}
		});

Ext.grid.Column.types['checkcolumn'] = Ext.ux.grid.CheckColumn;

// register ptype
Ext.preg('checkcolumn', Ext.ux.grid.CheckColumn);

// backwards compat
Ext.grid.CheckColumn = Ext.ux.grid.CheckColumn;