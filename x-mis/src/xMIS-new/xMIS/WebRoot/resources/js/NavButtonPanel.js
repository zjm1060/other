Ext.ns('Ext.ux');
Ext.ux.NavButtonPanel = Ext.extend(Ext.Panel, {
			header : false,
			border : false,
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
			border : false,
			initComponent : function() {
				this.layout = 'table';
				this.layoutConfig = {
					columns : 1
				};
				this.cp = new Ext.Panel({
							border : false
						});
				this.items = this.cp;
				this.cp.on('render', this.initImage, this);
				Ext.ux.NavButtonPanel.superclass.initComponent.call(this);
			},
			initImage : function() {
				// this.el.down('.' + this.bwrapCls).remove();
				// ICON HELL!!!
				// x-table-layout
				this.body.down('.x-table-layout')
						.applyStyles("width:100%;height:100%;");
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
				var el = this.cp.body;
				this.toTopIcon = el.createChild({
							tag : 'img',
							src : this.iconTop,
							title : '移动到顶部',
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
							title : '上移',
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
							title : '左移',
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
							title : '右移',
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
							title : '下移',
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
							title : '移动到底部',
							style : {
								cursor : 'pointer',
								margin : '2px'
							}
						});
				this.toTopIcon.on('click', this.toTop, this.scope || this);
				this.upIcon.on('click', this.up, this.scope || this);
				this.downIcon.on('click', this.down, this.scope || this);
				this.toBottomIcon
						.on('click', this.toBottom, this.scope || this);
				this.addIcon.on('click', this.fromTo, this.scope || this);
				this.removeIcon.on('click', this.toFrom, this.scope || this);
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

			},
			toTop : Ext.emptyFn,
			up : Ext.emptyFn,
			down : Ext.emptyFn,
			toBottom : Ext.emptyFn,
			fromTo : Ext.emptyFn,
			toFrom : Ext.emptyFn
		});