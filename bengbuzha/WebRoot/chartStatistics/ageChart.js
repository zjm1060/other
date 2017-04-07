Ext.onReady(function() {

			Ext.QuickTips.init();
			Ext.getCmp('agebtn').setText('<font color="red"><b>年龄统计图</b></font>');
			Ext.getCmp('agebtn').setHeight(25);
			var record = new Ext.data.Record.create([{
						name : 'age'
					}, {
						name : 'data'
					}]);
			var store = new Ext.data.Store({

						proxy : new Ext.data.HttpProxy({
									url : __cxtPath
											+ '/orgMaint.do?method=getAgeCount'
								}),
						reader : new Ext.data.JsonReader({
									root : 'rows'
								}, record),
						autoLoad : true,
						listeners:{
						
							load:function(){
								var temp='';
								for(var i=0;i<store.getCount();i++){
									temp= temp+'&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'+"<font color='blue'>"+store.getAt(i).get('age')+'的人数:'+"</font>"+"<font color='red'>"+store.getAt(i).get('data')+"</font>";
								}
								Ext.getCmp('count').setTitle(temp);
							}
						}

					});
			var ageChart = new Ext.Panel({
						title : '年龄比例柱状图',
						id:'ageChart',
						width:860,
						height:400,
						items : {
							xtype : 'columnchart',
							store : store,
							url : __cxtPath + '/resources/charts.swf',
							xField : 'age',
							yField : 'data',
							categoryField: 'age',
							tipRenderer : function(chart, record, index, series){
						 return record.data.age+'的人数：'+record.data.data;
						},
							listeners : {
								itemclick : function(o) {
									var rec = store.getAt(o.index);
									agestore.setBaseParam('age', rec.get('age'));
									agestore.reload();
								}
							}
							
						}
					});

					
var ageChart1=	new Ext.Panel({
        title: '年龄比例饼图',
        id:'ageChart1',
       width:860,
	   height:400,
        items: {
            store: store,
            xtype: 'piechart',
            dataField: 'data',
            categoryField: 'age',
            url : __cxtPath + '/resources/charts.swf',
            tipRenderer : function(chart, record, index, series){
						 return record.data.age+'的人数：'+record.data.data;
						},
             listeners : {
								itemclick : function(o) {
									var rec = store.getAt(o.index);
									agestore.setBaseParam('age', rec.get('age'));
									agestore.reload();
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
					
					
					var ageBotton = new Ext.Panel({
							id:'ageBotton',
							width:500,
							border : false,
							layout:'column',
							buttons:[
								{
								xtype:'button',
								text:'柱状图',
								handler: function(){
									ageChart1.hide();
									ageChart.show();
								}
							},{
								xtype:'button',
								text:'饼图',
								handler: function(){
									ageChart1.show();
									ageChart.hide();
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
			var agestore = new Ext.data.Store({
						proxy : new Ext.data.HttpProxy({
									url : __cxtPath
											+ '/orgMaint.do?method=getAgeList'
								}),
						baseParams : {
							start : 0,
							limit : 20,
							age : '30岁及以下'
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
		var	ageGrid = new Ext.grid.GridPanel({
						title : '年龄结构统计表',
						id:'ageGrid',
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
						store : agestore,
						cm : cm,
						
						bbar : new Ext.PagingToolbar({
									pageSize : 20,
									store : agestore,
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
				ageChart1.hide();
		var  age =	new Ext.Viewport({
						border : false,
						layout : 'border',
						items : [{
							region:'center',
               				 margins:'35 5 5 0',
               				 border : false,
               				 layout:'column',
              				  items:[ageChart,ageChart1,ageBotton,count]
						},ageGrid,menu]
					});
		});