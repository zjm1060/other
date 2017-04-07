/**
 * @author Cindy Lee
 * @date 2010.9.22
 * 
 */
Ext.namespace("Ext.ux");
Ext.ux.ImageCuter = Ext.extend(Ext.Window, {
	id : "imageCuter",
	imagePanelId : "imagePanel",
	backAreaId : "backArea", // 图片背景区id
	drugAreaId : "drugArea", // 鼠标拖动区id
	baseImgId : "baseImg", // 图片剪裁框id
	baseImgX : 150, // 设置图片框的位置x
	baseImgY : 100, // 设置图片框的位置y
	baseImgWidth : 100, // 图片剪裁框的宽度度
	baseImgHeight : 100, // 图片剪裁框的高度
	baseImgBorder : true, // 图像剪裁框是否有边框
	imageUrl : "images/aa1.png", // 加载图片的路径
	opacity : 80, // 图片蒙版透明度
	imgWidth : 1278, // 图片原始宽度
	imgHeight : 1021, // 图片原始高度
	start : false,
	oldPointer : null,
	newPointer : null,
	oldBackgroundPositionX : 0,
	oldBackgroundPositionY : 0,
	positionX : 0,
	positionY : 0,
	title : "图片剪裁器",
	layout : 'absolute',
	width : 400,
	height : 300,
	border : false,
	resizable : false,
	initComponent : function() {
		if (this.imgWidth < this.width || this.imgHeight < this.height) {
			alert("图片大小不能小于图片框");
			return;
		}
		this.width = this.width + 13; // 扩充两边边界
		this.height = this.height + 63; // 扩充上面标题部分

		Ext.ux.ImageCuter.superclass.initComponent.call(this);

		// 添加裁剪框
		this.add(this.getMainPanel(this.width - 13, this.height - 63));
		// 注册移动事件
		this.initMovement(this);
		// 设置按钮
		this.setButton(this);
	},
	getMainPanel : function(w, h) {
		var imagePanelId = this.imagePanelId;
		var panel = new Ext.Panel({
					id : imagePanelId,
					layout : 'absolute',
					width : w,
					height : h,
					border : false
				});
		panel.add(this.getBackArea(w, h));
		panel.add(this.getDrugArea(w, h));
		panel.add(this.getBaseImg());
		return panel;
	},
	getBackArea : function(w, h) {
		var backAreaId = this.backAreaId;
		var imgUrl = this.imageUrl;
		var panel = new Ext.Panel({
					id : backAreaId,
					xtype : "panel",
					x : 0,
					y : 0,
					width : w,
					height : h,
					border : false,
					bodyStyle : {
						'background-image' : "url(" + imgUrl + ")",
						'background-position' : '0px 0px',
						'background-repeat' : 'no-repeat'
					}
				});

		return panel;
	},
	getDrugArea : function(w, h) {
		var drugAreaId = this.drugAreaId;
		var opacity = this.opacity;
		var opacity2 = opacity * 0.01;
		var panel = new Ext.Panel({
					id : drugAreaId,
					x : 0,
					y : 0,
					width : w,
					height : h,
					border : false,
					bodyStyle : {
						'filter' : 'alpha(opacity=' + opacity + ')',
						'cursor' : 'move',
						'opacity' : opacity2
					}
				});
		return panel;
	},
	getBaseImg : function() {
		var baseImgId = this.baseImgId;
		var w = this.baseImgWidth;
		var h = this.baseImgHeight;
		var x = this.baseImgX;
		var y = this.baseImgY;
		var imgUrl = this.imageUrl;
		var border = this.baseImgBorder;
		var positionX = x;
		var positionY = y;
		if (border) {
			positionX = x + 1;
			positionY = y + 1;
		}
		var panel = new Ext.Panel({
					id : baseImgId,
					x : x,
					y : y,
					width : w,
					height : h,
					border : border,
					bodyStyle : {
						'background-image' : "url(" + imgUrl + ")",
						'background-repeat' : 'no-repeat',
						'background-position' : (-positionX) + "px "
								+ (-positionY) + "px"
					}
				});
		return panel;
	},
	initMovement : function(w) {
		this.on("beforeshow", function(evt) {
			var drugArea = Ext.get(w.drugAreaId);
			var blackImg = Ext.get(w.backAreaId);
			var backArea = Ext.getCmp(w.backAreaId);
			var baseImg = Ext.getCmp(w.baseImgId);

			drugArea.on("mousedown", function(evt) {
						w.oldPointer = evt.getXY();
						w.start = true;
					});

			drugArea.on("mouseup", function(evt) {
						w.oldBackgroundPositionX = w.positionX;
						w.oldBackgroundPositionY = w.positionY;
						w.start = false;
					});

			drugArea.on("mousemove", function(evt) {
				if (w.start) {
					w.newPointer = evt.getXY();
					var moveX = w.newPointer[0] - w.oldPointer[0];
					var moveY = w.newPointer[1] - w.oldPointer[1];

					w.positionX = w.oldBackgroundPositionX + moveX;
					w.positionY = w.oldBackgroundPositionY + moveY;

					var imgX = 0;
					var imgY = 0;
					if (w.baseImgBorder) {
						imgX = w.baseImgX + 1;
						imgY = w.baseImgY + 1;
					}

					if ((w.positionX - imgX) > 0) {
						w.positionX = imgX;
					}

					if ((w.positionY - imgY) > 0) {
						w.positionY = imgY;
					}

					if ((w.positionX + w.imgWidth - imgX - w.baseImgWidth) < 0) {
						w.positionX = -(w.imgWidth - imgX - w.baseImgWidth);
					}

					if ((w.positionY + w.imgHeight - imgY - w.baseImgHeight) < 0) {
						w.positionY = -(w.imgHeight - imgY - w.baseImgHeight);
					}
					w.setBackgroundPosition(backArea.getLayoutTarget(),
							w.positionX, w.positionY);
					w.setBackgroundPosition(baseImg.getLayoutTarget(),
							(w.positionX - imgX), (w.positionY - imgY));
				}
			});

		});
	},
	setBackgroundPosition : function(el, x, y) {
		el.setStyle({
					"background-position" : x + "px " + y + "px"
				});
	},
	setButton : function(w) {
		this.addButton({
					text : "确定"
				}, function() {
					var x = 0;
					var y = 0;
					if (w.baseImgBorder) {
						x = w.baseImgX - w.positionX + 1;
						y = w.baseImgY - w.positionY + 1;
					} else {
						x = w.baseImgX - w.positionX;
						y = w.baseImgY - w.positionY;
					}
					w.onClickOKButton(x, y, w.baseImgWidth, w.baseImgHeight);
				});

		this.addButton({
					text : "取消"
				}, function() {
					w.onClickCancelButton();
				});

	},
	onClickOKButton : function(x, y, w, h) {
		// alert("x:"+x+" y:"+y+" w:"+w+" h:"+h);
	},
	onClickCancelButton : function() {
		// alert("cancel");
	}

});

Ext.reg('imagecuter', Ext.ux.ImageCuter);
