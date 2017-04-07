Ext.onReady(function() {

			Ext.QuickTips.init();
			Ext.getCmp('zzmmbtn').setText('<font color="red"><b>政治面貌统计图</b></font>');
			Ext.getCmp('zzmmbtn').setHeight(25);
			var record = new Ext.data.Record.create([{
						name : 'zzmm'
					}, {
						name : 'data'
					}]);
			var store = new Ext.data.Store({

						proxy : new Ext.data.HttpProxy({
									url : __cxtPath
											+ '/orgMaint.do?method=getZzmmCount'
								}),
						reader : new Ext.data.JsonReader({
									root : 'rows'
								}, record),
						autoLoad : true,
						listeners:{
						
							load:function(){
								var temp='';
								for(var i=0;i<store.getCount();i++){
									temp= temp+'&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'+"<font color='blue'>"+store.getAt(i).get('zzmm')+'的人数:'+"</font>"+"<font color='red'>"+store.getAt(i).get('data')+"</font>";
								}
								Ext.getCmp('count').setTitle(temp);
							}
						}

					});
			var zzmmChart = new Ext.Panel({
						title : '政治面貌比例柱状图',
						id:'zzmm',
						width:700,
						height:400,
						items : {
							xtype : 'columnchart',
							store : store,
							url : __cxtPath + '/resources/charts.swf',
							xField : 'zzmm',
							yField : 'data',
							categoryField: 'zzmm',
							tipRenderer : function(chart, record, index, series){
						 return record.data.zzmm+'的人数：'+record.data.data;
						},
							listeners : {
								itemclick : function(o) {
									var rec = store.getAt(o.index);
									zzmmstore.setBaseParam('zzmm', rec.get('zzmm'));
									zzmmstore.reload();
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

					
var zzmmChart1=	new Ext.Panel({
        title: '政治面貌比例饼图',
        id:'zzmm1',
       width:700,
	   height:400,
        items: {
            store: store,
            xtype: 'piechart',
            dataField: 'data',
            categoryField: 'zzmm',
            url : __cxtPath + '/resources/charts.swf',
            tipRenderer : function(chart, record, index, series){
						 return record.data.zzmm+'的人数：'+record.data.data;
						},
             listeners : {
								itemclick : function(o) {
								var rec = store.getAt(o.index);
								zzmmstore.setBaseParam('zzmm', rec.get('zzmm'));
								zzmmstore.reload();
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
					
					
					var zzmmBotton = new Ext.Panel({
							id:'zzmmBotton',
							width:500,
							border : false,
							layout:'column',
							buttons:[
								{
								xtype:'button',
								text:'柱状图',
								handler: function(){
									zzmmChart1.hide();
									zzmmChart.show();
								}
							},{
								xtype:'button',
								text:'饼图',
								handler: function(){
									zzmmChart1.show();
									zzmmChart.hide();
								}
							}
							]
							
					});
			var gridrecord = Ext.data.Record.create([{
						name : 'name'
					}, {
						name : 'sex'
					},{
						name:'entryDate'
					},{
						name:'age'
					}]);
			var zzmmstore = new Ext.data.Store({
						proxy : new Ext.data.HttpProxy({
									url : __cxtPath
											+ '/orgMaint.do?method=getZzmmList'
								}),
						baseParams : {
							start : 0,
							limit : 20,
							zzmm : '中央党员'
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
						header : '入职日期',
						dataIndex : 'entryDate',
						sortable : true
					}, {
						header : '年龄',
						dataIndex : 'age',
						sortable : true
					}]);
			zzmmGrid = new Ext.grid.GridPanel({
						title : '政治面貌统计表',
						id:'zzmmGrid',
						region:'east',
						border : false,
						width : 460,
						loadMask : true,
						collapsible : true,
						autoScroll : true,
						collapseMode : 'mini',
						loadMask : {
							msg : '正在加载数据，请稍侯……'
						},
						stripeRows : true,
						store : zzmmstore,
						cm : cm,
						
						bbar : new Ext.PagingToolbar({
									pageSize : 20,
									id:'bbar',
									store : zzmmstore,
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
			zzmmChart1.hide();
		var  zzmm =	new Ext.Viewport({
						border : false,
						layout : 'border',
						items : [{
							region:'center',
               				 margins:'35 5 5 0',
               				 border : false,
               				 layout:'column',
              				  items:[zzmmChart,zzmmChart1,zzmmBotton,count]
						},zzmmGrid,menu]
					});
		});