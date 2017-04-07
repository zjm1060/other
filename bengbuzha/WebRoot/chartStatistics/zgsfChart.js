Ext.onReady(function() {

			Ext.QuickTips.init();
			Ext.getCmp('zgsfbtn').setText('<font color="red"><b>职工身份统计图</b></font>');
			Ext.getCmp('zgsfbtn').setHeight(25);
			var record = new Ext.data.Record.create([{
						name : 'zgsf'
					}, {
						name : 'data'
					}]);
			var store = new Ext.data.Store({

						proxy : new Ext.data.HttpProxy({
									url : __cxtPath
											+ '/orgMaint.do?method=getZgsfCount'
								}),
						reader : new Ext.data.JsonReader({
									root : 'rows'
								}, record),
						autoLoad : true,
						listeners:{
						
							load:function(){
								var temp='';
								for(var i=0;i<store.getCount();i++){
									temp= temp+'&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'+"<font color='blue'>"+store.getAt(i).get('zgsf')+'的人数:'+"</font>"+"<font color='red'>"+store.getAt(i).get('data')+"</font>";
								}
								Ext.getCmp('count').setTitle(temp);
							}
						}

					});
			var zgsfChart = new Ext.Panel({
						title : '职工身份比例柱状图',
						id:'zgsfChart',
						width:860,
						height:400,
						items : {
							xtype : 'columnchart',
							store : store,
							url : __cxtPath + '/resources/charts.swf',
							xField : 'zgsf',
							yField : 'data',
							categoryField: 'zgsf',
							tipRenderer : function(chart, record, index, series){
						 return record.data.zgsf+'的人数：'+record.data.data;
						},
							listeners : {
								itemclick : function(o) {
									var rec = store.getAt(o.index);
									zgsfstore.setBaseParam('zgsf', rec.get('zgsf'));
									zgsfstore.reload();
								}
							}
							
						}
					});

					
var zgsfChart1=	new Ext.Panel({
        title: '职工身份结构比例饼图',
        id:'zgsfChart1',
       width:860,
	   height:400,
        items: {
            store: store,
            xtype: 'piechart',
            dataField: 'data',
            categoryField: 'zgsf',
            url : __cxtPath + '/resources/charts.swf',
            tipRenderer : function(chart, record, index, series){
						 return record.data.zgsf+'的人数：'+record.data.data;
						},
             listeners : {
								itemclick : function(o) {
									var rec = store.getAt(o.index);
									zgsfstore.setBaseParam('zgsf', rec.get('zgsf'));
									zgsfstore.reload();
								}
							},
			  extraStyle:
            {
                legend:
                {
                    display: 'bottom',
                    padding: 5,
                    font:
                    {
                        family: 'Tahoma',
                        size: 13
                    }
                }
            }
        }
       
    });
					
					
					var zgsfBotton = new Ext.Panel({
							id:'zgsfBotton',
							width:500,
							border : false,
							layout:'column',
							buttons:[
								{
								xtype:'button',
								text:'柱状图',
								handler: function(){
									zgsfChart1.hide();
									zgsfChart.show();
								}
							},{
								xtype:'button',
								text:'饼图',
								handler: function(){
									zgsfChart1.show();
									zgsfChart.hide();
								}
							}
							]
							
					});
			var gridrecord = Ext.data.Record.create([{
						name : 'name'
					}, {
						name : 'sex'
					},{
						name:'birthday'
					},{
						name:'age'
					}]);
			var zgsfstore = new Ext.data.Store({
						proxy : new Ext.data.HttpProxy({
									url : __cxtPath
											+ '/orgMaint.do?method=getZgsfList'
								}),
						baseParams : {
							start : 0,
							limit : 20,
							zgsf : '干部'
						},
						reader : new Ext.data.JsonReader({
									root : 'rows',
									totalProperty : 'totalCount'
								}, gridrecord),
						autoLoad : true
					});
			var cm = new Ext.grid.ColumnModel([new Ext.grid.RowNumberer({
								header : '序号',
								width : 50
							}), {
						header : '姓名',
						dataIndex : 'name'
					}, {
						header : '性别',
						dataIndex : 'sex',
						sortable : true
					}, {
						header : '出生日期',
						dataIndex : 'birthday',
						sortable : true
					}, {
						header : '年龄',
						dataIndex : 'age',
						sortable : true
					}]);
		var	zgsfGrid = new Ext.grid.GridPanel({
						title : '职工身份结构统计表',
						id:'zgsfGrid',
						border : false,
						region:'east',
						width : 460,
						loadMask : true,
						collapsible : true,
						autoScroll : true,
						collapseMode : 'mini',
						loadMask : {
							msg : '正在加载数据，请稍侯……'
						},
						stripeRows : true,
						store : zgsfstore,
						cm : cm,
						
						bbar : new Ext.PagingToolbar({
									pageSize : 20,
									store : zgsfstore,
									displayInfo : true,
									displayMsg : '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
									emptyMsg : "无显示数据",
									plugins : [new Ext.ux.PagelimitCombo()]
								})

					});
					var menu = xmis.chartStatistics.menu;
					var count = new Ext.Panel({
						width:860,
						height:20,
						border:false,
						title : ' ',
						id:'count'
					});
				zgsfChart1.hide();
			new Ext.Viewport({
						border : false,
						layout : 'border',
						items : [{
							region:'center',
               				 margins:'35 5 5 0',
               				 border : false,
               				 layout:'column',
              				  items:[zgsfChart,zgsfChart1,zgsfBotton,count]
						},zgsfGrid,menu]
					});
		});