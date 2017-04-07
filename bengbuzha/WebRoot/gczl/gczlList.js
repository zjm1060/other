Ext.onReady(function() {
	Ext.QuickTips.init();
	 
	var gczlgrid =	new com.xmis.gczl.grid();
	var comparisonGrid = new com.xmis.gczl.comparisonGrid();
	var excretiongrid = new com.xmis.gczl.excretiongrid();
	var floodSeasonGrid = new com.xmis.gczl.floodSeasonGrid();
	var floodGates = new com.xmis.gczl.floodGates();
	var everyMonthLevel =new com.xmis.gczl.everyMonthLevel();
	new Ext.Viewport({
				layout : 'column',
				autoScroll : true,

				items : [{
							border : false,
							columnWidth : .49,
							items : [{

										title : '最高最低水位/最大最小水位差/最大流量',
										style : 'padding:5px 5px 5px 5px',
										collapsible : true,
										// collapsed:true,// 初始化后自动打开，true为不打开
										items : gczlgrid,
										frame : true
									}, {
										title : '汛期/非汛期最高最低水位/最大最小水位差/最大流量',

										style : 'padding:5px 5px 5px 5px',
										collapsible : true,
										// collapsed:true,// 初始化后自动打开，true为不打开
										items :  floodSeasonGrid,
										frame : true
									}, {
										title : '开闸/关闸期间最高最低水位/最大最小水位差/最大流量',
										style : 'padding:5px 5px 5px 5px',
										collapsible : true,
										items : floodGates,
										frame : true
									}]
						}, {
							border : false,
							columnWidth : .49,
							items : [{
										title : '历史同期/最高、最低水位统计表',
										style : 'padding:5px 5px 5px 0px',
										collapsible : true,
										// collapsed:true,// 初始化后自动打开，true为不打开
										items : comparisonGrid,
										frame : true
									}, {
										title : '排泄量查询',
										style : 'padding:5px 5px 5px 0px',
										collapsible : true,
										// collapsed:true,// 初始化后自动打开，true为不打开
										items : excretiongrid,
										frame : true
									}, {
										title : '每年逐月平均/最高/最低水位',
										style : 'padding:5px 5px 5px 0px',
										collapsible : true,
										// collapsed:true,// 初始化后自动打开，true为不打开
										items : everyMonthLevel,
										frame : true
									}]
						}]
			});
});
