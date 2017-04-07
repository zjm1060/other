com.bitc.xmis.accounting.DeptAxis = Ext.extend(Ext.grid.PivotAxis, {
	realSortColumn : undefined,
	renderHorizontalRows: function() {
        var headers  = this.buildHeaders(),
            rowCount = headers.length,
            rows     = [],
            cells, cols, colCount, i, j;
        for (i = 0; i < rowCount; i++) {
            cells = [];
            cols  = headers[i].items;
            colCount = cols.length;

            for (j = 0; j < colCount; j++) {
                cells.push({
                    tag: 'td',
                    html: cols[j].header,
                    colspan: cols[j].span
                });
            }

            rows[i] = {
                tag: 'tr',
                cn: cells
            };
        }
        
        return rows;
    },
	buildHeaders : function() {
        var tuples     = this.getTuples(),
            rowCount   = tuples.length,
            dimensions = this.dimensions,
            dimension,
            colCount   = dimensions.length,
            headers    = [],
            tuple, rows, currentHeader, previousHeader, span, start, isLast, changed, i, j;
        
        for (i = 0; i < colCount; i++) {
            dimension = dimensions[i];
            rows  = [];
            span  = 0;
            start = 0;
            
            for (j = 0; j < rowCount; j++) {
                tuple  = tuples[j];
                isLast = j == (rowCount - 1);
                currentHeader = tuple.data[dimension.dataIndex];
                
                changed = previousHeader != undefined && previousHeader != currentHeader;
                if (i > 0 && j > 0) {
                    changed = changed || tuple.data[dimensions[i-1].dataIndex] != tuples[j-1].data[dimensions[i-1].dataIndex];
                }
                
                if (changed) {
                	var t = previousHeader;
                	if(/^\d{4}-\d{2}$/.test(previousHeader)){
                		t = '<a href=\"javascript:;\" onclick=\"javascript:reloadDeptMonthWin(\''+previousHeader+'\');\">' + parseInt(previousHeader.substring(5,8)) + '月</a>';
                	}else if(/^\d{4}$/.test(previousHeader)){
                		t = previousHeader + '年';
                	}else{
                		t = '<a href=\"javascript:;\" onclick=\"javascript:showPersonMonthWin(\''+previousHeader+'\',\'dept\');\">' + previousHeader + '</a>';
                	}
                    rows.push({
                        header: t,
                        span  : span,
                        start : start
                    });
                    
                    start += span;
                    span = 0;
                }
                
                if (isLast) {
                	var t = currentHeader;
                	if(currentHeader == '小计')
                		t = '<a href=\"javascript:;\" onclick=\"javascript:reloadDeptMonthWin(\''+currentHeader+'\');\">' + currentHeader + '</a>';
                	if(currentHeader == '合计')
                		t = '<a href=\"javascript:;\" onclick=\"javascript:showPersonMonthWin(\''+currentHeader+'\',\'comp\');\"><font color=\"red\">' + currentHeader + '</font></a>';
                    rows.push({
                        header: t,
                        span  : span + 1,
                        start : start
                    });
                    
                    start += span;
                    span = 0;
                }
                
                previousHeader = currentHeader;
                span++;
            }
            
            headers.push({
                items: rows,
                width: dimension.width || this.defaultHeaderWidth
            });
            
            previousHeader = undefined;
        }
        
        return headers;
    },
	getTuples: function() {
        var newStore = new Ext.data.Store({});
        
        newStore.data = this.store.data.clone();
        newStore.fields = this.store.fields;
        
        var sorters    = [],
            dimensions = this.dimensions,
            length     = dimensions.length,
            i;
        
        for (i = 0; i < length; i++) {
            sorters.push({
                field    : dimensions[i].realSortColumn == undefined  ? dimensions[i].dataIndex : dimensions[i].realSortColumn,
                direction: dimensions[i].direction || 'ASC'
            });
        }
        
        newStore.sort(sorters);
        
        var records = newStore.data.items,
            hashes  = [],
            tuples  = [],
            recData, hash, info, data, key;
        
        length = records.length;
        
        for (i = 0; i < length; i++) {
            info = this.getRecordInfo(records[i]);
            data = info.data;
            hash = "";
            
            for (key in data) {
                hash += data[key] + '---';
            }
            
            if (hashes.indexOf(hash) == -1) {
                hashes.push(hash);
                tuples.push(info);
            }
        }
        newStore.destroy();
        return tuples;
    },
    initComponent : function() {
    	com.bitc.xmis.accounting.DeptAxis.superclass.initComponent.call(this);
    }
});

com.bitc.xmis.accounting.DeptMonthAccountGrid = Ext.extend(Ext.grid.PivotGrid, {
	initAxes: function() {
        var PivotAxis = com.bitc.xmis.accounting.DeptAxis;
        
        if (!(this.leftAxis instanceof PivotAxis)) {
            this.setLeftAxis(new PivotAxis({
                orientation: 'vertical',
                dimensions : this.leftAxis || [],
                store      : this.store
            }));
        };
        
        if (!(this.topAxis instanceof PivotAxis)) {
            this.setTopAxis(new PivotAxis({
                orientation: 'horizontal',
                dimensions : this.topAxis || [],
                store      : this.store
            }));
        };
    },
    sortMonth : undefined,
    initComponent : function() {
    	var grid = this;
    	grid.sortMonth = Ext.util.Format.date(new Date(),'Y-m');
    	var currentDate = new Date();
    	var startDate = new Date('1/1/2011');
    	var years =[];
    	var i = 0;
    	while(currentDate >= startDate){
    		var year = [];
    		year[0] = Ext.util.Format.date(currentDate,'Y');
    		year[1] = Ext.util.Format.date(currentDate,'Y')+ '年';
    		years.push(year);
    		i++;
    		currentDate = currentDate.add(Date.YEAR,-1);
    	}
    	
    	var SaleRecord = Ext.data.Record.create([
    	                                         {name: 'deptID',   type: 'string'},
    	                                         {name: 'deptName',   type: 'string'},
    	                                         {name: 'loginmonth',  type: 'string'},
    	                                         {name: 'total',     type: 'int'},
    	                                         {name: 'deptPx',    type: 'int'},
    	                                         {name: 'year',     type: 'string'},
    	                                         {name: 'month',    type: 'string'}
    	                                     ]);
    	                                     
    	                                     this.store = new Ext.data.Store({
    	                                         url: 'developProgressMaint.do?method=getMonthAccounting1',
    	                                         autoLoad: true,
    	                                         baseParams : {
    	                                         	start : Ext.util.Format.date(new Date(),'Y')+'-01',
    	                                         	end : Ext.util.Format.date(new Date(),'Y')+'-12',
    	                                         	sort : grid.sortMonth
    	                                         },
    	                                         reader: new Ext.data.JsonReader({
    	                                             root: 'rows'
    	                                         }, SaleRecord)
    	                                     });
    	                                     
    	                                     window.reloadDeptMonthWin = function(month){
    	                             	    	grid.store.setBaseParam('sort',month);
    	                             	    	grid.sortMonth = month;
    	                             	    	grid.store.load();
    	                             	    };
    	
    	                                     this.tbar = [new Ext.form.Label({
    	                     					style : 'margin: 0 0 0 4 ;!important',
    	                     					text : '统计日期：'
    	                     				}),{
    	                     					xtype : 'combo',
    	                     					width : 70,
    	                     					ref : '../startYear',
    	                     					displayField : 'text',
    	                     					valueField : 'value',
    	                     					mode : 'local',
    	                     					triggerAction : 'all',
    	                     					editable : false,
    	                     					value : Ext.util.Format.date(new Date(),'Y'),
    	                     					store : new Ext.data.ArrayStore({
    	                     						fields : ['value','text'],
    	                     						data : years
    	                     					})
    	                     				},{
    	                     					xtype : 'combo',
    	                     					width : 70,
    	                     					ref : '../startMonth',
    	                     					displayField : 'text',
    	                     					valueField : 'value',
    	                     					mode : 'local',
    	                     					triggerAction : 'all',
    	                     					editable : false,
    	                     					value : '01',
    	                     					store : new Ext.data.ArrayStore({
    	                     								fields : ['value','text'],
    	                     								data : [['01','1月'],['02','2月'],['03','3月'],['04','4月'],['05','5月'],['06','6月'],['07','7月'],['08','8月'],['09','9月'],['10','10月'],['11','11月'],['12','12月']]
    	                     							})
    	                     				},'至',{
    	                     					xtype : 'combo',
    	                     					width : 70,
    	                     					ref : '../endYear',
    	                     					displayField : 'text',
    	                     					valueField : 'value',
    	                     					mode : 'local',
    	                     					triggerAction : 'all',
    	                     					editable : false,
    	                     					value : Ext.util.Format.date(new Date(),'Y'),
    	                     					store : new Ext.data.ArrayStore({
    	                     								fields : ['value','text'],
    	                     								data : years
    	                     							})
    	                     				},{
    	                     					xtype : 'combo',
    	                     					width : 70,
    	                     					ref : '../endMonth',
    	                     					displayField : 'text',
    	                     					valueField : 'value',
    	                     					mode : 'local',
    	                     					triggerAction : 'all',
    	                     					editable : false,
    	                     					value : '12',
    	                     					store : new Ext.data.ArrayStore({
    	                     								fields : ['value','text'],
    	                     								data : [['01','1月'],['02','2月'],['03','3月'],['04','4月'],['05','5月'],['06','6月'],['07','7月'],['08','8月'],['09','9月'],['10','10月'],['11','11月'],['12','12月']]
    	                     							})
    	                     				}, '-', {
    	                     					ref : '../searchBtn',
    	                     					iconCls : 'searchIcon',
    	                     					text : '查询',
    	                     					handler : function(b, e) {
    	                     						var startYear = grid.startYear.getValue() + '';
    	                     						var startMonth = grid.startMonth.getValue();
    	                     						var endYear = grid.endYear.getValue() + '';
    	                     						var endMonth = grid.endMonth.getValue();
    	                     						
    	                     						var start = startYear + '-' + startMonth;
    	                     						var end = endYear + '-' + endMonth;
    	                     						var sort = grid.sortMonth;
    	                     						
    	                     						var t1 = new Date(startMonth + '/01/' + startYear);
    	                     						var t2 = new Date(endMonth + '/01/' + endYear);
    	                     						if(t1 > t2){
    	                     							Ext.Msg.alert("提示","开始日期必须小于等于结束日期，请重新选择！");
    	                     							return;
    	                     						}
    	                     						if(sort != '小计'){
    	                     							var startDate = new Date(startMonth + '/01/' + startYear);
        	                     						var endDate = new Date(endMonth + '/01/' + endYear).add(Date.MONTH,1);
        	                     						var dateStr = sort.split('-');
        	                     						var sortDate = new Date(dateStr[1] + '/01/' + dateStr[0]);
        	                     						if(sortDate >= endDate){
        	                     							sort = end;
        	                     						}
        	                     						if(sortDate < startDate && endDate <= new Date()){
        	                     							sort = end;
        	                     						}
        	                     						if(sortDate < startDate && endDate >= new Date()){
        	                     							sort = Ext.util.Format.date(new Date(),'Y-m');
        	                     						}
        	                     						grid.sortMonth = sort;
    	                     						}
    	                     						grid.store.setBaseParam('start',start);
    	                     						grid.store.setBaseParam('end',end);
    	                     						grid.store.setBaseParam('sort',sort);
    	                     						grid.store.load();
    	                     					}
    	                     				}],
    	                             this.aggregator = function(records, measure) {
    	                             	 var result;
    	                                 var total = records[0].get(measure);
    	                     			 if(total != 0 && "合计" != records[0].get('deptName')&& "小计" != records[0].get('loginmonth'))
    	                     				 total = '<a href=\"javascript:;\" onclick=\"javascript:showPersonWeekWin(' + records[0].get('deptID') + ',\'' + records[0].get('loginmonth') + '\',\''+records[0].get('deptName')+'\',\'dept\');\">' + total + '</a>';
    	                     			 if(grid.sortMonth == records[0].get('loginmonth')){
    	                     				 if("合计" == records[0].get('deptName')&&"小计" != records[0].get('loginmonth')){
    	                     					total = '<a href=\"javascript:;\" onclick=\"javascript:showPersonWeekWin(' + records[0].get('deptID') + ',\'' + records[0].get('loginmonth') + '\',\''+records[0].get('deptName')+'\',\'comp\');\">' + total + '</a>';
    	                     					result = '<div style=\"text-align:center;height:26px;background-color: #ffff00;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
    	                     				 }else if("合计" == records[0].get('deptName')&&"小计" == records[0].get('loginmonth')){
    	                     					total = '<a href=\"javascript:;\" onclick=\"javascript:showPersonMonthWin(\'合计\',\'comp\');\">' + total + '</a>';
    	                     					result = '<div style=\"text-align:center;height:26px;background-color: #ffff00;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
    	                     				 }else{
    	                     					if(total > 0)
    	                     						total = '<a href=\"javascript:;\" onclick=\"javascript:showPersonMonthWin(\''+records[0].get('deptName')+'\',\'dept\',undefined,\''+records[0].get('deptName')+'\');\">' + total + '</a>';
    	                     					result = '<div style=\"text-align:center;height:26px;background-color: #ffff00;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
    	                     				 }
    	                                 }else if("合计" == records[0].get('deptName')&& "小计" != records[0].get('loginmonth')){
    	                                	 total = '<a href=\"javascript:;\" onclick=\"javascript:showPersonWeekWin(' + records[0].get('deptID') + ',\'' + records[0].get('loginmonth') + '\',\''+records[0].get('deptName')+'\',\'comp\');\">' + total + '</a>';
    	                                	 result = '<div style=\"text-align:center;height:26px;background-color: #f1f2f4;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
    	                                 }else if("小计" == records[0].get('loginmonth') && "合计" != records[0].get('deptName')){
    	                                	 if(total > 0)
    	                                		 total = '<a href=\"javascript:;\" onclick=\"javascript:showPersonMonthWin(\''+records[0].get('deptName')+'\',\'dept\',undefined,\'小计\');\">' + total + '</a>';
    	                                	 result = '<div style=\"text-align:center;height:26px;background-color: #f1f2f4;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
    	                                 }else if("小计" == records[0].get('loginmonth') && "合计" == records[0].get('deptName')){
    	                                	 total = '<a href=\"javascript:;\" onclick=\"javascript:showPersonMonthWin(\'合计\',\'comp\',\'小计\');\">' + total + '</a>';
    	                                	 result = '<div style=\"text-align:center;height:26px;background-color: #f1f2f4;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
    	                                 }else{
    	                                	 result = '<div style=\"text-align:center;height:26px;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
    	                                 }
    	                                 return result;
    	                             };
    	                             this.measure = 'total';
    	                             this.viewConfig = {
    	                                 title: '部门名称'
    	                             };
    	                             this.renderer = function(value) {
    	                                 return value;
    	                             };
    	                             this.leftAxis = [
    	                                 new com.bitc.xmis.accounting.DeptAxis({
    	                                	 dataIndex: 'deptName',
    	                                     realSortColumn : 'deptPx',
    	                                     direction : 'asc',
    	                                     width : 135
    	                                 })
    	                             ];
    	                             this.topAxis = [
    	                                 new com.bitc.xmis.accounting.DeptAxis({
    	                                     dataIndex: 'year',
    	                                     align : 'center'
    	                                 }),
    	                                 new com.bitc.xmis.accounting.DeptAxis({
    	                                     dataIndex: 'loginmonth',
    	                                     align : 'center'
    	                                 })
    	                             ];
    	com.bitc.xmis.accounting.DeptMonthAccountGrid.superclass.initComponent.call(this);
	}
});

Ext.onReady(function() {
	
    var pivotGrid = new com.bitc.xmis.accounting.DeptMonthAccountGrid({
    	loadMask : true, // 读取数据时的遮罩和提示功能，默认为false
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
        columnLines: true,
        border : false,
        stripeRows : true,
        region : 'center'
     });
    
    var monthPanel = new Ext.Panel({
		layout : 'tdgi_border',
		region : 'center',
		split : true,
		border : false,
		items : [pivotGrid,new Ext.Panel({
			title : '说明',
			region : 'south',
			height:240,
			border : false,
			autoScroll : true,
			collapsedTitle: true,
			collapsible : true,
			split: true,
			html : '<div style=\"margin-top:2px;\">&nbsp;<font>X-MIS登录次数统计月报说明</font></div>'
				 + '<div style=\"margin-top:2px;\">&nbsp;&nbsp;&nbsp;1.设置日期：在上方菜单栏选择统计的开始日期及结束日期(默认：当年一月至十二月)，点击‘查询’按钮查询。</div>'
				 + '<div style=\"margin-top:1px;\">&nbsp;&nbsp;&nbsp;2.报表排序：点击报表标题栏的数字(例‘01’、‘02’等)或‘小计’，将现有报表结果按照选定月份或小计的<font color=\"blue\">降序</font>排序,排序列背景底色为黄色。</div>'
				 + '<div style=\"margin-top:1px;\">&nbsp;&nbsp;&nbsp;3.报表点击：</div>'
				 + '<div style=\"margin-top:1px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;A.部门：点击‘部门名称’，查询该部门下所有员工在所选日期范围内的登录统计信息；若该部门有子部门，则显示该部门下各子部门在所选日期范围内的登录统计信息。</div>'
				 + '<div style=\"margin-top:1px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;B.部门-月份：点击部门与月份的交叉值，查询该部门当月人员登录统计信息，子窗体以周为单位汇总，周日为一周的第一天。</div>'
				 + '<div style=\"margin-top:1px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;C.部门-小计：点击部门与小计的交叉值，查询该部门在所选日期范围内人员登录统计信息，子窗体以月为单位汇总。</div>'
				 + '<div style=\"margin-top:1px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;D.合计：点击‘合计’，查询公司在选定的<font color=\"blue\">排序</font>下，登录次数前20名的人员名单。</div>'
				 + '<div style=\"margin-top:1px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;E.合计-月份：点击合计与月份的交叉值，查询公司当月的人员登录统计信息，子窗体以周为单位汇总。</div>'
				 + '<div style=\"margin-top:1px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;F.合计-小计：行为合计，列为小计，功能同‘合计’。</div>'
				 + '<div style=\"margin-top:1px;\">&nbsp;&nbsp;&nbsp;4.其他：点击后各子窗体的操作同上所述。</div>',
			collapsed : true
			})]
	});
    
    var currentDate = new Date();
    var num = currentDate.getDay();
    var s,e;
    if(num == 0){
    	s = Ext.util.Format.date(currentDate,'Y-m-d');
    	var eDate = currentDate.add(Date.DAY,6);
    	if(eDate.getMonth() != currentDate.getMonth()){
    		eDate = eDate.getFirstDateOfMonth().add(Date.DAY,-1);
    	}
    	e = Ext.util.Format.date(eDate,'Y-m-d');
    }else{
    	s = Ext.util.Format.date(currentDate.add(Date.DAY,-num),'Y-m-d');
    	var eDate = currentDate.add(Date.DAY,-num).add(Date.DAY,6);
    	//if(eDate.getMonth() != currentDate.add(Date.DAY,-num).getMonth()){
    	//	eDate = eDate.getFirstDateOfMonth().add(Date.DAY,-1);
    	//}
    	e = Ext.util.Format.date(eDate,'Y-m-d');
    }
    var sortM = s + 'Z' + e;
    var pivotGrid2 = new com.bitc.xmis.accounting.PersonWeekAccountGrid1({
    	loadMask : true, // 读取数据时的遮罩和提示功能，默认为false
		loadMask : {
			msg : '正在加载数据，请稍侯……'
		},
		sortWeek : sortM,
        columnLines: true,
        stripeRows : true,
        border : false,
        region : 'center'
     });
    
    var weekPanel = new Ext.Panel({
		layout : 'tdgi_border',
		region : 'center',
		border : false,
		split : true,
		items : [pivotGrid2,new Ext.Panel({
			title : '说明',
			region : 'south',
			height:200,
			border : false,
			autoScroll : true,
			collapsedTitle: true,
			collapsible : true,
			split: true,
			html : '<div style=\"margin-top:2px;\">&nbsp;<font>X-MIS登录次数统计最近四周统计说明</font></div>'
				 + '<div style=\"margin-top:2px;\">&nbsp;&nbsp;&nbsp;1.统计范围：当前时间所在周及前三周，周日为一周的第一天。</div>'
				 + '<div style=\"margin-top:1px;\">&nbsp;&nbsp;&nbsp;2.报表排序：点击报表标题栏的日期范围(例‘2012-12-09至2012-12-15’)或‘小计’，将报表按照选定日期范围或小计的<font color=\"blue\">降序</font>排序,排序列背景底色为黄色。</div>'
				 + '<div style=\"margin-top:1px;\">&nbsp;&nbsp;&nbsp;3.报表点击：</div>'
				 + '<div style=\"margin-top:1px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;A.部门：点击‘部门名称’，查询该部门下所有员工最近四周的登录统计信息；若该部门有子部门，则显示该部门下各子部门最近四周的登录统计信息。</div>'
				 + '<div style=\"margin-top:1px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;B.部门-小计：点击部门与小计的交叉点，查询该部门最近四周人员登录统计信息。</div>'
				 + '<div style=\"margin-top:1px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;C.合计：点击‘合计’，查询公司在选定的<font color=\"blue\">排序</font>下，登录次数前20名的人员名单。</div>'
				 + '<div style=\"margin-top:1px;\">&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;D.合计-小计：点击部门与小计的交叉点，功能同‘合计’。</div>'
				 + '<div style=\"margin-top:1px;\">&nbsp;&nbsp;&nbsp;4.其他：点击后各窗体的操作同上所述。</div>',
			collapsed : true
			})]
	});
    
    var tabs = new Ext.TabPanel({
    	deferredRender : false,
		region : 'center',
		margins : '3 3 3 0',
		activeTab : 0,
		tabPosition : 'top',
		items : [{
					id : 'monthAccount',
					title : '月报',
					layout : 'border',
					items : [monthPanel]
				}, {
					id : 'weekAccount',
					title : '最近四周统计',
					layout : 'border',
					items : [weekPanel]
				}]
	});
    
    new Ext.Viewport({
		layout : 'border',
		items : tabs
	});
    
    this.showPersonMonthWin = function(deptName,searchType,s,t){
    	var startYear = pivotGrid.startYear.getValue() + '';
		var startMonth = pivotGrid.startMonth.getValue();
		var endYear = pivotGrid.endYear.getValue() + '';
		var endMonth = pivotGrid.endMonth.getValue();
		var sortMonth = pivotGrid.sortMonth;
		var start = startYear + '-' + startMonth;
		var end = endYear + '-' + endMonth;
		var sort = sortMonth;
		if(s)
			sort = s;
		Ext.Ajax.request({
			url : __cxtPath
					+ '/developProgressMaint.do?method=getSubDeptCount',
			success : function(response, action) {
				var action = Ext.decode(response.responseText);
				var count = parseInt(action.message);
				if(count == 0){
					var win = new com.bitc.xmis.accounting.PersonMonthAccountWindow({
						modal : true,
			    		searchType : searchType,
			    		startMonth : start,
			    		endMonth : end,
			    		sortMonth : sort,
			    		deptName : deptName,
			    		width : Ext.getBody().getWidth()*0.8,
			    		height : Ext.getBody().getHeight()*0.8
			    	});
					win.show();
				}else{
					if(t){
						var win = new com.bitc.xmis.accounting.PersonMonthAccountWindow({
							modal : true,
				    		startMonth : start,
				    		endMonth : end,
				    		sortMonth : sort,
				    		deptName : deptName,
				    		width : Ext.getBody().getWidth()*0.8,
				    		height : Ext.getBody().getHeight()*0.8
				    	});
						win.show();
					}else{
						var win = new com.bitc.xmis.accounting.subMonthAccountWindow({
							modal : true,
				    		startMonth : start,
				    		endMonth : end,
				    		sortMonth : sort,
				    		deptName : deptName,
				    		width : Ext.getBody().getWidth()*0.8,
				    		height : Ext.getBody().getHeight()*0.8
				    	});
						win.show();
					}
				}
			},
			failure : function() {
			},
			method : "post",
			params : {
				deptName : deptName
			}
		});
    };
    this.showPersonWeekWin = function(deptID,month,deptName,searchType){
    	var win = new com.bitc.xmis.accounting.PersonWeekAccountWindow({
    		modal : true,
    		startMonth : month,
    		endMonth : month,
    		sortWeek : '小计',
    		deptID : deptID,
    		deptName : deptName,
    		searchType : searchType,
    		width : Ext.getBody().getWidth()*0.8,
    		height : Ext.getBody().getHeight()*0.8
    	});
    	win.show();
    };
    
    this.showPersonOfDeptForLastWeeksWin = function(deptName,searchType,t){

    	var sortMonth = pivotGrid2.sortWeek;
    	var sort = sortMonth;
    	
    	Ext.Ajax.request({
    		url : __cxtPath
    				+ '/developProgressMaint.do?method=getSubDeptCount',
    		success : function(response, action) {
    			var action = Ext.decode(response.responseText);
    			var count = parseInt(action.message);
    			if(count == 0){
    				var win = new com.bitc.xmis.accounting.PersonOfDeptForLastWeeksAccountWindow({
    					modal : true,
    		    		searchType : searchType,
    		    		sortWeek : sort,
    		    		deptName : deptName,
    		    		width : Ext.getBody().getWidth()*0.8,
    		    		height : Ext.getBody().getHeight()*0.8
    		    	});
    				win.show();
    			}else{
    				if(t){
    					var win = new com.bitc.xmis.accounting.PersonOfDeptForLastWeeksAccountWindow({
        					modal : true,
        		    		searchType : searchType,
        		    		sortWeek : sort,
        		    		deptName : deptName,
        		    		width : Ext.getBody().getWidth()*0.8,
        		    		height : Ext.getBody().getHeight()*0.8
        		    	});
        				win.show();
    				}else{
    					var win = new com.bitc.xmis.accounting.SubDeptOfDeptAccountWindow({
    						modal : true,
    						title : deptName + '最近四周各部门登录统计信息',
    						sortWeek : sort,
    			    		deptName : deptName,
    			    		width : Ext.getBody().getWidth()*0.8,
    			    		height : Ext.getBody().getHeight()*0.8
    			    	});
    					win.show();
    				}
    			}
    		},
    		failure : function() {
    		},
    		method : "post",
    		params : {
    			deptName : deptName
    		}
    	});
    };
    
});


