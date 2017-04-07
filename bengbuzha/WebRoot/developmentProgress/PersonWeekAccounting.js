//-----------------------------------------------------------------------------------------------------
Ext.ns('com.bitc.xmis.accounting');
com.bitc.xmis.accounting.PersonLastWeekAxis = Ext.extend(Ext.grid.PivotAxis, {
	columnLines: true,
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
	buildHeaders: function() {
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
                	if(/^\d{4}-\d{2}-\d{2}Z\d{4}-\d{2}-\d{2}$/.test(previousHeader)){
                		var s = previousHeader.substring(0,10);
                		var e = previousHeader.substring(11,21);
                		t = '<a href=\"javascript:;\" onclick=\"javascript:reloadPersonWeekWin1(\''+previousHeader+'\');\">' + s + '至' + e + '</a>';
                	}else if(/^\d{1}$/.test(previousHeader)){
                		if(previousHeader == 4)
                			t = '当前周';
                		if(previousHeader == 3)
                			t = '前一周';
                		if(previousHeader == 2)
                			t = '前二周';
                		if(previousHeader == 1)
                			t = '前三周';
                	}else{
                		t = '<a href=\"javascript:;\" onclick=\"javascript:showPersonOfDeptForLastWeeksWin(\''+previousHeader+'\');\">' + previousHeader + '</a>';
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
                		t= '<a href=\"javascript:;\" onclick=\"javascript:reloadPersonWeekWin1(\''+currentHeader+'\');\">' + currentHeader + '</a>';
                	if(currentHeader == '合计')
                		t = '<a href=\"javascript:;\" onclick=\"javascript:showPersonOfDeptForLastWeeksWin(\''+currentHeader+'\',\'comp\');\"><font color=\"red\">' + currentHeader + '</font></a>';
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
    	com.bitc.xmis.accounting.PersonLastWeekAxis.superclass.initComponent.call(this);
    }
});
com.bitc.xmis.accounting.PersonWeekAccountGrid1 = Ext.extend(Ext.grid.PivotGrid, {
	initAxes: function() {
        var PivotAxis = com.bitc.xmis.accounting.PersonLastWeekAxis;
        
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
    stripeRows : true,
	realSortColumn : undefined,
	startMonth : undefined,
	endMonth : undefined,
	sortWeek : undefined,
	deptID : undefined,
	deptName : undefined,
	initComponent : function() {
		var grid = this;
		var record = Ext.data.Record.create([
		    {name: 'deptID',type: 'string'},
		    {name: 'deptName',type: 'string'},
		    {name: 'weeks',type: 'string'},
		    {name: 'total',type: 'int'},
		    {name: 'weeknum',type: 'string'},
		    {name: 'deptPx',type: 'int'}
		]);
		this.store = new Ext.data.Store({
					url : __cxtPath + '/developProgressMaint.do?method=getWeekAccounting',
					autoLoad: true,
			        baseParams : {
			        	searchType : 'personLastWeek',
			        	sortWeek : grid.sortWeek
			        },
			        reader: new Ext.data.JsonReader({
			            root: 'rows'
			        }, record)
				});
		this.leftAxis = [new com.bitc.xmis.accounting.PersonAxis({
		    dataIndex: 'deptName',
		    realSortColumn : 'deptPx',
		    direction : 'ASC',
		    width : 135
		})];
		this.topAxis = [ new com.bitc.xmis.accounting.PersonLastWeekAxis({
		    dataIndex: 'weeknum',
		    align : 'center'
		}),new com.bitc.xmis.accounting.PersonLastWeekAxis({
		    dataIndex: 'weeks',
		    align : 'center'
		 })];
		
		window.reloadPersonWeekWin1 = function(month){
	    	grid.store.setBaseParam('sortWeek',month);
	    	grid.sortWeek = month;
	    	grid.store.load();
	    };
		
		this.aggregator = function(records, measure) {
        	var result;
        	var total = records[0].get(measure);
        	if(grid.sortWeek == records[0].get('weeks')){
    			if("合计" == records[0].get('deptName')&&"小计" == records[0].get('weeks')){
    				total = '<a href=\"javascript:;\" onclick=\"javascript:showPersonOfDeptForLastWeeksWin(\'合计\',\'comp\');\">' + total + '</a>';
    				result = '<div style=\"text-align:center;height:26px;background-color: #ffff00;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
    			}else if("合计" != records[0].get('deptName')&&"小计" == records[0].get('weeks')){
    				total = '<a href=\"javascript:;\" onclick=\"javascript:showPersonOfDeptForLastWeeksWin(\''+records[0].get('deptName')+'\',undefined,\'dept\');\">' + total + '</a>';
    				result = '<div style=\"text-align:center;height:26px;background-color: #ffff00;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
    			}else{
    				result = '<div style=\"text-align:center;height:26px;background-color: #ffff00;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
    			}
            }else if("小计" == records[0].get('weeks') && "合计" != records[0].get('deptName')){
               	 if(total > 0)
               		 total = '<a href=\"javascript:;\" onclick=\"javascript:showPersonOfDeptForLastWeeksWin(\''+records[0].get('deptName')+'\',undefined,\'dept\');\">' + total + '</a>';
               	 result = '<div style=\"text-align:center;height:26px;background-color: #f1f2f4;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
            }else if("小计" == records[0].get('weeks') && "合计" == records[0].get('deptName')){
               	 total = '<a href=\"javascript:;\" onclick=\"javascript:showPersonOfDeptForLastWeeksWin(\'合计\',\'comp\');\">' + total + '</a>';
               	 result = '<div style=\"text-align:center;height:26px;background-color: #f1f2f4;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
            }else{
               	 result = '<div style=\"text-align:center;height:26px;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
            }
            return result;
        };
        this.measure = 'total';
        this.viewConfig = {
            title : '部门名称'
        };
        this.renderer = function(value) {
            return value;
        };
        
        
        com.bitc.xmis.accounting.PersonWeekAccountGrid1.superclass.initComponent.call(this);
	}
});
//com.bitc.xmis.accounting.PersonWeekAccountWindow = Ext.extend(Ext.Window, {
//	width : 600,
//	height : 450,
//	layout : 'border',
//	startMonth : undefined,
//	endMonth : undefined,
//	deptName : undefined,
//	sortWeek : undefined,
//	deptID : undefined,
//	initComponent : function() {
//		var win = this;
//		this.items = [new com.bitc.xmis.accounting.PersonWeekAccountGrid({
//			loadMask : true, // 读取数据时的遮罩和提示功能，默认为false
//			loadMask : {
//				msg : '正在加载数据，请稍侯……'
//			},
//			region : 'center',
//			startMonth : win.startMonth,
//			endMonth : win.endMonth,
//			sortWeek : win.sortWeek,
//			deptID : win.deptID,
//			deptName : win.deptName
//		})];
//		com.bitc.xmis.accounting.PersonWeekAccountWindow.superclass.initComponent.call(this);
//	}
//});


//--------------------------------------------------------------------------------------
com.bitc.xmis.accounting.PersonOfDeptForLastWeeksAxis = Ext.extend(Ext.grid.PivotAxis, {
	columnLines: true,
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
	buildHeaders: function() {
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
                	if(/^\d{4}-\d{2}-\d{2}Z\d{4}-\d{2}-\d{2}$/.test(previousHeader)){
                		var s = previousHeader.substring(0,10);
                		var e = previousHeader.substring(11,21);
                		t = '<a href=\"javascript:;\" onclick=\"javascript:reloadPersonMonthWin3(\''+previousHeader+'\');\">' + s + '至' + e + '</a>';
                	}else if(/^\d{1}$/.test(previousHeader)){
                		if(previousHeader == 4)
                			t = '当前周';
                		if(previousHeader == 3)
                			t = '前一周';
                		if(previousHeader == 2)
                			t = '前二周';
                		if(previousHeader == 1)
                			t = '前三周';
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
                    rows.push({
                        header: (currentHeader == '小计') ? '<a href=\"javascript:;\" onclick=\"javascript:reloadPersonMonthWin3(\''+currentHeader+'\');\">' + currentHeader + '</a>' : '<font color=\"red\">' + currentHeader + '</font>',
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
    	com.bitc.xmis.accounting.PersonOfDeptForLastWeeksAxis.superclass.initComponent.call(this);
    }
});
com.bitc.xmis.accounting.PersonOfDeptForLastWeeksAccountGrid = Ext.extend(Ext.grid.PivotGrid, {
	initAxes: function() {
        var PivotAxis = com.bitc.xmis.accounting.PersonOfDeptForLastWeeksAxis;
        
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
    //查询的两种情况，部门下查询人，公司下查询人
    PERSONINDEPT : "getPersonOfDeptByDeptID",
    PERSONINCOMP : "getPersonOfCompByComp",
    //设置排序月份的值
    setSortWeekValue : function(value){
    	this.sortWeek = value;
    	this.generateWinTitle(value);
    },
    generateSearchStr : function(){
    	if(this.searchType == 'comp')
    		return this.PERSONINCOMP;
    	return this.PERSONINDEPT;
    },
    generateWinTitle : function(value){
    	if(this.win){
    		if(this.generateSearchStr() == this.PERSONINCOMP){
    			var d = '中水科技 ';
    			var time = value;
    			if(time == '小计'){
    				time = '最近四周登录总次数前20名人员名单';
    			}else{
    				var s = value.split('Z');
    				time = s[0] + '至' + s[1] + '月登录次数前20名人员名单';
    			}
    			title = d + time;
    			this.win.setTitle(title);
    		}else{
    			var d = this.deptName + ' ';
    			var time = '最近四周人员登录统计信息';
    			title = d + time;
    			this.win.setTitle(title);
    		}
    	}
    },
    stripeRows : true,
	realSortColumn : undefined,
	startMonth : undefined,
	endMonth : undefined,
	sortWeek : undefined,
	deptName : undefined,
	searchType : undefined,
	win : undefined,
	initComponent : function() {
		var grid = this;
		grid.generateWinTitle(grid.sortWeek);
		var record = Ext.data.Record.create([
		    {name: 'userID',type: 'string'},
		    {name: 'userName',type: 'string'},
		    {name: 'weeknum',type: 'string'},
		    {name: 'total',type: 'int'},
		    {name: 'userPx',type: 'int'},
		    {name: 'weeks',type: 'string'}
		]);
		this.store = new Ext.data.Store({
					url : __cxtPath + '/developProgressMaint.do?method=getWeekAccounting',
					autoLoad: true,
			        baseParams : {
			        	searchType : grid.generateSearchStr(),
			        	sortWeek : grid.sortWeek,
			        	deptName : grid.deptName
			        },
			        reader: new Ext.data.JsonReader({
			            root: 'rows'
			        }, record)
				});
		
		window.reloadPersonMonthWin3 = function(month){
	    	grid.store.setBaseParam('sortWeek',month);
	    	grid.setSortWeekValue(month);
	    	grid.store.load();
	    };
	    
		this.leftAxis = [new com.bitc.xmis.accounting.PersonOfDeptForLastWeeksAxis({
		    dataIndex: 'userName',
		    realSortColumn : 'userPx',
		    direction : 'ASC',
		    width : 70
		})];
		this.topAxis = [ new com.bitc.xmis.accounting.PersonOfDeptForLastWeeksAxis({
		    dataIndex: 'weeknum',
		    align : 'center'
		}),new com.bitc.xmis.accounting.PersonOfDeptForLastWeeksAxis({
		    dataIndex: 'weeks',
		    align : 'center'
		 })];
		
		this.aggregator = function(records, measure) {
        	var result;
            var total = records[0].get(measure);
            result = '<div style=\"text-align:center;height:26px;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
            if("合计" == records[0].get('userName')){
            	result = '<div style=\"text-align:center;height:26px;background-color: #f1f2f4;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
            }
            if("小计" == records[0].get('weeks')){
            	result = '<div style=\"text-align:center;height:26px;background-color: #f1f2f4;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
            }
            if(grid.sortWeek == records[0].get('weeks')){
            	result = '<div style=\"text-align:center;height:26px;background-color: #ffff00;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
            }
            return result;
        };
        this.measure = 'total';
        this.viewConfig = {
            title : '员工姓名'
        };
        this.renderer = function(value) {
            return value;
        };
        
        
        com.bitc.xmis.accounting.PersonOfDeptForLastWeeksAccountGrid.superclass.initComponent.call(this);
	}
});
com.bitc.xmis.accounting.PersonOfDeptForLastWeeksAccountWindow = Ext.extend(Ext.Window, {
	width : 600,
	height : 450,
	layout : 'border',
	startMonth : undefined,
	endMonth : undefined,
	sortWeek : undefined,
	deptName : undefined,
	searchType : undefined,
	initComponent : function() {
		var win = this;
		this.items = [new com.bitc.xmis.accounting.PersonOfDeptForLastWeeksAccountGrid({
			loadMask : true, // 读取数据时的遮罩和提示功能，默认为false
			loadMask : {
				msg : '正在加载数据，请稍侯……'
			},
			region : 'center',
			sortWeek : win.sortWeek,
			deptName : win.deptName,
			searchType : win.searchType,
			win : win
		})];
		com.bitc.xmis.accounting.PersonOfDeptForLastWeeksAccountWindow.superclass.initComponent.call(this);
	}
});


//--------------------------------------------------------------------------
com.bitc.xmis.accounting.SubDeptOfDeptAxis = Ext.extend(Ext.grid.PivotAxis, {
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
                	if(/^\d{4}-\d{2}-\d{2}Z\d{4}-\d{2}-\d{2}$/.test(previousHeader)){
                		var s = previousHeader.substring(0,10);
                		var e = previousHeader.substring(11,21);
                		t = '<a href=\"javascript:;\" onclick=\"javascript:reloadDeptMonthWin5(\''+previousHeader+'\');\">' + s + '至' + e + '</a>';
                	}else if(/^\d{1}$/.test(previousHeader)){
                		if(previousHeader == 4)
                			t = '当前周';
                		if(previousHeader == 3)
                			t = '前一周';
                		if(previousHeader == 2)
                			t = '前二周';
                		if(previousHeader == 1)
                			t = '前三周';
                	}else{
                		t = '<a href=\"javascript:;\" onclick=\"javascript:showPersonOfDeptForLastWeeksWin(\''+previousHeader+'\');\">' + previousHeader + '</a>';
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
                		t = '<a href=\"javascript:;\" onclick=\"javascript:reloadDeptMonthWin5(\''+currentHeader+'\');\">' + currentHeader + '</a>';
                	if(currentHeader == '合计')
                		t = '<a href=\"javascript:;\" onclick=\"javascript:showPersonOfDeptForLastWeeksWin(\'监控事业部\',undefined,\'dept\');\"><font color=\"red\">' + currentHeader + '</font></a>';
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
    	com.bitc.xmis.accounting.SubDeptOfDeptAxis.superclass.initComponent.call(this);
    }
});
com.bitc.xmis.accounting.SubDeptOfDeptAccountGrid = Ext.extend(Ext.grid.PivotGrid, {
	initAxes: function() {
        var PivotAxis = com.bitc.xmis.accounting.SubDeptOfDeptAxis;
        
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
    startMonth : undefined,
	endMonth : undefined,
	sortWeek : undefined,
	deptID : undefined,
	deptName : undefined,
    initComponent : function() {
    	var grid = this;
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
    	                                         {name: 'weeknum',  type: 'string'},
    	                                         {name: 'total',     type: 'int'},
    	                                         {name: 'deptPx',    type: 'int'},
    	                                         {name: 'weeks',     type: 'string'}
    	                                     ]);
    	                                     
    	                                     this.store = new Ext.data.Store({
    	                                         url: 'developProgressMaint.do?method=getWeekAccounting',
    	                                         autoLoad: true,
    	                                         baseParams : {
    	                                        	 sortWeek : grid.sortWeek,
    	                                         	 deptName : grid.deptName,
    	                                         	 searchType : 'getSubDeptOfDeptByDeptID'
    	                                         },
    	                                         reader: new Ext.data.JsonReader({
    	                                             root: 'rows'
    	                                         }, SaleRecord)
    	                                     });
    	                                     
    	                                     window.reloadDeptMonthWin5 = function(month){
     	                             	    	grid.store.setBaseParam('sort',month);
     	                             	    	grid.sortWeek = month;
     	                             	    	grid.store.load();
     	                             	    };
    	                                     
    	                             this.aggregator = function(records, measure) {
    	                            	 var result;
    	                                 var total = records[0].get(measure);
    	                                 if(grid.sortWeek == records[0].get('weeks')){
    	                         			if("合计" == records[0].get('deptName')&&"小计" == records[0].get('weeks')){
    	                         				total = '<a href=\"javascript:;\" onclick=\"javascript:showPersonOfDeptForLastWeeksWin(\'监控事业部\',undefined,\'dept\');\">' + total + '</a>';
    	                         				result = '<div style=\"text-align:center;height:26px;background-color: #ffff00;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
    	                         			}else if("合计" != records[0].get('deptName')&&"小计" == records[0].get('weeks')){
    	                         				total = '<a href=\"javascript:;\" onclick=\"javascript:showPersonOfDeptForLastWeeksWin(\''+records[0].get('deptName')+'\',undefined,\'dept\');\">' + total + '</a>';
    	                         				result = '<div style=\"text-align:center;height:26px;background-color: #ffff00;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
    	                         			}else{
    	                         				result = '<div style=\"text-align:center;height:26px;background-color: #ffff00;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
    	                         			}
    	                                 }else if("小计" == records[0].get('weeks') && "合计" != records[0].get('deptName')){
    	                                    	 if(total > 0)
    	                                    		 total = '<a href=\"javascript:;\" onclick=\"javascript:showPersonOfDeptForLastWeeksWin(\''+records[0].get('deptName')+'\',undefined,\'dept\');\">' + total + '</a>';
    	                                    	 result = '<div style=\"text-align:center;height:26px;background-color: #f1f2f4;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
    	                                 }else if("小计" == records[0].get('weeks') && "合计" == records[0].get('deptName')){
    	                                    	 total = '<a href=\"javascript:;\" onclick=\"javascript:showPersonOfDeptForLastWeeksWin(\'监控事业部\',undefined,\'dept\');\">' + total + '</a>';
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
    	                                 new com.bitc.xmis.accounting.SubDeptAxis({
    	                                	 dataIndex: 'deptName',
    	                                     realSortColumn : 'deptPx',
    	                                     direction : 'asc',
    	                                     width : 135
    	                                 })
    	                             ];
    	                             this.topAxis = [
    	                                 new com.bitc.xmis.accounting.SubDeptOfDeptAxis({
    	                                     dataIndex: 'weeknum',
    	                                     align : 'center'
    	                                 }),
    	                                 new com.bitc.xmis.accounting.SubDeptOfDeptAxis({
    	                                     dataIndex: 'weeks',
    	                                     align : 'center'
    	                                 })
    	                             ];
    	                             com.bitc.xmis.accounting.SubDeptOfDeptAccountGrid.superclass.initComponent.call(this);
	}
});
com.bitc.xmis.accounting.SubDeptOfDeptAccountWindow = Ext.extend(Ext.Window, {
	width : 600,
	height : 450,
	layout : 'border',
	startMonth : undefined,
	endMonth : undefined,
	sortWeek : undefined,
	deptName : undefined,
	initComponent : function() {
		var win = this;
		this.items = [new com.bitc.xmis.accounting.SubDeptOfDeptAccountGrid({
			loadMask : true, // 读取数据时的遮罩和提示功能，默认为false
			loadMask : {
				msg : '正在加载数据，请稍侯……'
			},
			region : 'center',
			startMonth : win.startMonth,
			endMonth : win.endMonth,
			sortWeek : win.sortWeek,
			deptName : win.deptName
		})];
		com.bitc.xmis.accounting.SubDeptOfDeptAccountWindow.superclass.initComponent.call(this);
	}
});