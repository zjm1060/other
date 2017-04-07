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
};