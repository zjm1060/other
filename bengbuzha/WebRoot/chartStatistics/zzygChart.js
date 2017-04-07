Ext.onReady(function() {

			Ext.QuickTips.init();
			Ext.getCmp('zzygbtn').setText('<font color="red"><b>在职员工统计图</b></font>');
			Ext.getCmp('zzygbtn').setHeight(25);
				var record = new Ext.data.Record.create([{
						name : 'zzyg'
					}, {
						name : 'data'
					}]);
			var store = new Ext.data.Store({

						proxy : new Ext.data.HttpProxy({
									url : __cxtPath
											+ '/orgMaint.do?method=getZzygCount'
								}),
						reader : new Ext.data.JsonReader({
									root : 'rows'
								}, record),
						autoLoad : true,
						listeners:{
						
							load:function(){
								var temp='';
								for(var i=0;i<store.getCount();i++){
									temp= temp+'&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;'+"<font color='blue'>"+store.getAt(i).get('zzyg')+'的人数:'+"</font>"+"<font color='red'>"+store.getAt(i).get('data')+"</font>";
								}
								Ext.getCmp('count').setTitle(temp);
							}
						}

					});
			var Chart = new Ext.Panel({
						title : '在职人员比例柱状图',
						width:700,
						height:400,
						items : {
							xtype : 'columnchart',
							store : store,
							url : __cxtPath + '/resources/charts.swf',
							xField : 'zzyg',
							yField : 'data',
							categoryField: 'zzyg',
							tipRenderer : function(chart, record, index, series){
						 return record.data.zzyg+'的人数：'+record.data.data;
						},
							listeners : {
								itemclick : function(o) {
									var rec = store.getAt(o.index);
									gridstore.setBaseParam('zzyg', rec.get('zzyg'));
									gridstore.reload();
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

					
var Chart1=	new Ext.Panel({
        title: '在职人员比例饼图',
       width:700,
	   height:400,
        items: {
            store: store,
            xtype: 'piechart',
            dataField: 'data',
            categoryField: 'zzyg',
            url : __cxtPath + '/resources/charts.swf',
            tipRenderer : function(chart, record, index, series){
						 return record.data.zzyg+'的人数：'+record.data.data;
						},
             listeners : {
								itemclick : function(o) {
									var rec = store.getAt(o.index);
									gridstore.setBaseParam('zzyg', rec.get('zzyg'));
									gridstore.reload();
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
					
					
					var Botton = new Ext.Panel({
							width:500,
							border:false,
							layout:'column',
							buttons:[
								{
								xtype:'button',
								text:'柱状图',
								handler: function(){
									Chart1.hide();
									Chart.show();
								}
							},{
								xtype:'button',
								text:'饼图',
								handler: function(){
									Chart1.show();
									Chart.hide();
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
			var gridstore = new Ext.data.Store({
						proxy : new Ext.data.HttpProxy({
									url : __cxtPath
											+ '/orgMaint.do?method=getZzygList'
								}),
						baseParams : {
							start : 0,
							limit : 20,
							zzyg : '在职'
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
			Grid = new Ext.grid.GridPanel({
						title : '在职人员比例统计表',
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
						store : gridstore,
						cm : cm,
						
						bbar : new Ext.PagingToolbar({
									pageSize : 20,
									store : gridstore,
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
					Chart1.hide();
		 	new Ext.Viewport({
						border : false,
						layout : 'border',
						items : [{
							region:'center',
               				 margins:'35 5 5 0',
               				 border : false,
               				 layout:'column',
              				  items:[Chart,Chart1,Botton,count]
						},Grid,menu]
					});
		});