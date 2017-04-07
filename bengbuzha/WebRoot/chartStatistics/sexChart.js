Ext.onReady(function() {
			Ext.QuickTips.init();
			Ext.getCmp('sexbtn').setText('<font color="red"><b>性别统计图</b></font>');
			Ext.getCmp('sexbtn').setHeight(25);
			var record = new Ext.data.Record.create([{
						name : 'sex'
					}, {
						name : 'data'
					}]);
			var store = new Ext.data.Store({

						proxy : new Ext.data.HttpProxy({
									url : __cxtPath
											+ '/orgMaint.do?method=getSexCount'
								}),
						reader : new Ext.data.JsonReader({
									root : 'rows'
								}, record),
						autoLoad : true,
						listeners:{
						
							load:function(){
								var temp='';
								for(var i=0;i<store.getCount();i++){
									temp= temp+'&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'+"<font color='blue'>"+store.getAt(i).get('sex')+'的人数:'+"</font>"+"<font color='red'>"+store.getAt(i).get('data')+"</font>";
								}
								Ext.getCmp('count').setTitle(temp);
							}
						}
					});
			var sexChart = new Ext.Panel({
						title : '男女比例柱状图',
						id:'sexChart',
						width:860,
						height:400,
						items : {
							xtype : 'columnchart',
							store : store,
							url : __cxtPath + '/resources/charts.swf',
							xField : 'sex',
							yField : 'data',
							categoryField: 'sex',
							tipRenderer : function(chart, record, index, series){
						 return record.data.sex+'的人数：'+record.data.data;
						},
							listeners : {
								itemclick : function(o) {
									var rec = store.getAt(o.index);
									sexgridstore.setBaseParam('sex', rec.get('sex'));
									sexgridstore.reload();
								}
							}
							
//		  extraStyle:
//            {
//                legend:
//                {
//                    display: 'bottom',
//                    padding: 5,
//                    font:
//                    {
//                        family: 'Tahoma',
//                        size: 13
//                    }
//                }
//            }
						}
					});

					
var sexChart1=	new Ext.Panel({
        title: '男女比例饼图',
        id:'sexChart1',
       width:860,
	   height:400,
        items: {
            store: store,
            xtype: 'piechart',
            dataField: 'data',
            categoryField: 'sex',
            url : __cxtPath + '/resources/charts.swf',
            tipRenderer : function(chart, record, index, series){
						 return record.data.sex+'的人数：'+record.data.data;
						},
             listeners : {
								itemclick : function(o) {
									var rec = store.getAt(o.index);
									sexgridstore.setBaseParam('sex', rec.get('sex'));
									sexgridstore.reload();
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
					
					
					var sexBotton = new Ext.Panel({
							id:'sexBotton',
							width:500,
							border:false,
							layout:'column',
							buttons:[
								{
								xtype:'button',
								text:'柱状图',
								handler: function(){
									sexChart1.hide();
									sexChart.show();
								}
							},{
								xtype:'button',
								text:'饼图',
								handler: function(){
									sexChart1.show();
									sexChart.hide();
								}
							}
							]
							
					});
			var gridrecord = Ext.data.Record.create([{
						name : 'name'
					}, {
						name : 'sex'
					}]);
			var sexgridstore = new Ext.data.Store({
						proxy : new Ext.data.HttpProxy({
									url : __cxtPath
											+ '/orgMaint.do?method=getSexList'
								}),
						baseParams : {
							start : 0,
							limit : 20,
							sex : '男性'
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
					}]);     
			sexGrid = new Ext.grid.GridPanel({
						title : '男女比例统计表',
						id:'sexGrid',
						region:'east',
						border : false,
						width : 300,
						loadMask : true,
						collapsible : true,
						autoScroll : true,
						collapseMode : 'mini',
						loadMask : {
							msg : '正在加载数据，请稍侯……'
						},
						stripeRows : true,
						store : sexgridstore,
						cm : cm,
						bbar : new Ext.PagingToolbar({
									pageSize : 20,
									store : sexgridstore,
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
					sexChart1.hide();
			new Ext.Viewport({
						border : false,
						layout : 'border',
						items : [{
							region:'center',
               				 margins:'35 5 5 0',
               				 border : false,
               				 layout:'column',
              				  items:[sexChart,sexChart1,sexBotton,count]
						},sexGrid,menu]
					});
		});