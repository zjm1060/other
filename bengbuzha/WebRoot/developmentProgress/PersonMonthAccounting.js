Ext.ns('com.bitc.xmis.accounting');
com.bitc.xmis.accounting.PersonAxis = Ext.extend(Ext.grid.PivotAxis, {
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
                	if(/^\d{4}-\d{2}$/.test(previousHeader)){
                		t = '<a href=\"javascript:;\" onclick=\"javascript:reloadPersonMonthWin(\''+previousHeader+'\');\">' + parseInt(previousHeader.substring(5,8)) + '月</a>';
                	}else if(/^\d{4}$/.test(previousHeader)){
                		t = previousHeader + '年';
                	}else{
                		t = previousHeader;
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
                		t = '<a href=\"javascript:;\" onclick=\"javascript:reloadPersonMonthWin(\''+currentHeader+'\');\">' + currentHeader + '</a>';
                	if(currentHeader == '合计')
                		t = '<font color=\"red\">' + currentHeader + '</font>';
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
    	com.bitc.xmis.accounting.PersonAxis.superclass.initComponent.call(this);
    }
});
com.bitc.xmis.accounting.PersonMonthAccountGrid = Ext.extend(Ext.grid.PivotGrid, {
	initAxes: function() {
        var PivotAxis = com.bitc.xmis.accounting.PersonAxis;
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
    PERSONINDEPT : "personMonth",
    PERSONINCOMP : "getPersonAndMonthTotalInCompany",
    //设置排序月份的值
    setSortMonthValue : function(value){
    	this.sortMonth = value;
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
    			var start = this.startMonthValue;
    			var end = this.endMonthValue;
    			var s = start.split('-');
    			var e = end.split('-');
    			var d = '中水科技 ';
    			var time = value;
    			if(time == '小计'){
    				time = s[0] + '年' + s[1] + '月至' + e[0] + '年' + e[1] + '月期间登录总次数前20名人员名单';
    			}else{
    				var s = value.split('-');
    				time = s[0] + '年' + s[1] + '月登录次数前20名人员名单';
    			}
    			title = d + time;
    			this.win.setTitle(title);
    		}else{
    			var start = this.startMonthValue;
    			var end = this.endMonthValue;
    			var s = start.split('-');
    			var e = end.split('-');
    			var d = this.deptName + ' ';
    			var time = s[0] + '年' + s[1] + '月至' + e[0] + '年' + e[1] + '月人员登录统计信息';
    			title = d + time;
    			this.win.setTitle(title);
    		}
    	}
    },
    stripeRows : true,
	realSortColumn : undefined,
	startMonthValue : undefined,
	endMonthValue : undefined,
	sortMonth : undefined,
	deptName : undefined,
	//查询类型，dept为部门内，comp为公司内（当在公司内时，win属性必填）,未定义默认为dept
	searchType : undefined,
	win : undefined,
	
	initComponent : function() {
		var grid = this;
		grid.generateWinTitle(grid.sortMonth);
		var record = Ext.data.Record.create([
		    {name: 'userID',type: 'string'},
		    {name: 'userName',type: 'string'},
		    {name: 'loginmonth',type: 'string'},
		    {name: 'total',type: 'int'},
		    {name: 'userPx',type: 'int'},
		    {name: 'year',type: 'string'},
		    {name: 'month',type: 'string'}
		]);
		this.store = new Ext.data.Store({
					url : __cxtPath + '/developProgressMaint.do?method=getMonthAccounting1',
					autoLoad: true,
			        baseParams : {
			        	searchType : grid.generateSearchStr(),
			        	start : grid.startMonthValue,
			        	end : grid.endMonthValue,
			        	sort : grid.sortMonth,
			        	deptName : grid.deptName
			        },
			        reader: new Ext.data.JsonReader({
			            root: 'rows'
			        }, record)
				});
		
		window.reloadPersonMonthWin = function(month){
	    	grid.store.setBaseParam('sort',month);
	    	grid.setSortMonthValue(month);
	    	grid.store.load();
	    };
	    
		this.leftAxis = [new com.bitc.xmis.accounting.PersonAxis({
		    dataIndex: 'userName',
		    realSortColumn : 'userPx',
		    direction : 'ASC',
		    width : 70
		})];
		this.topAxis = [ new com.bitc.xmis.accounting.PersonAxis({
		    dataIndex: 'year',
		    align : 'center'
		}),new com.bitc.xmis.accounting.PersonAxis({
		    dataIndex: 'loginmonth',
		    align : 'center'
		 })];
		
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
		this.tbar = [new Ext.form.Label({
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
			value : grid.startMonthValue.substring(0,4),
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
			value : grid.startMonthValue.substring(5,8),
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
			value : grid.endMonthValue.substring(0,4),
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
			value : grid.endMonthValue.substring(5,8),
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
						
					}
					grid.startMonthValue = start;
					grid.endMonthValue = end;
					grid.setSortMonthValue(sort);
					grid.store.setBaseParam('start',start);
					grid.store.setBaseParam('end',end);
					grid.store.setBaseParam('sort',sort);
					grid.store.load();
			}
		}];
		this.aggregator = function(records, measure) {
        	var result;
            var total = records[0].get(measure);
            result = '<div style=\"text-align:center;height:26px;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
            if("合计" == records[0].get('userName')){
            	result = '<div style=\"text-align:center;height:26px;background-color: #f1f2f4;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
            }
            if("小计" == records[0].get('loginmonth')){
            	result = '<div style=\"text-align:center;height:26px;background-color: #f1f2f4;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
            }
            if(grid.sortMonth == records[0].get('loginmonth')){
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
        
        
		com.bitc.xmis.accounting.PersonMonthAccountGrid.superclass.initComponent.call(this);
	}
});
com.bitc.xmis.accounting.PersonMonthAccountWindow = Ext.extend(Ext.Window, {
	width : 600,
	height : 450,
	layout : 'border',
	startMonth : undefined,
	endMonth : undefined,
	sortMonth : undefined,
	deptName : undefined,
	searchType : undefined,
	initComponent : function() {
		var win = this;
		this.items = [new com.bitc.xmis.accounting.PersonMonthAccountGrid({
			loadMask : true, // 读取数据时的遮罩和提示功能，默认为false
			loadMask : {
				msg : '正在加载数据，请稍侯……'
			},
			region : 'center',
			startMonthValue : win.startMonth,
			endMonthValue : win.endMonth,
			sortMonth : win.sortMonth,
			deptName : win.deptName,
			searchType : win.searchType,
			win : win
		})];
		com.bitc.xmis.accounting.PersonMonthAccountWindow.superclass.initComponent.call(this);
	}
});












//-----------------------------------------------------------------------------------------------------
com.bitc.xmis.accounting.PersonWeekAxis = Ext.extend(Ext.grid.PivotAxis, {
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
                	if(/^\d{1}$/.test(previousHeader))
                		t = '第' + previousHeader + '周';
                	if(/^\d{4}-\d{2}-\d{2}Z\d{4}-\d{2}-\d{2}$/.test(previousHeader)){
                		var m= parseInt(previousHeader.substring(5,8));
                		var s = parseInt(previousHeader.substring(8,10));
                		var e = parseInt(previousHeader.substring(19,21));
                		if(s != e){
                			t = '<a href=\"javascript:;\" onclick=\"javascript:reloadPersonWeekWin(\''+previousHeader+'\');\">' + m + '月' + s + '日至' + m + '月' + e + '日</a>';
                		}else{
                			t = '<a href=\"javascript:;\" onclick=\"javascript:reloadPersonWeekWin(\''+previousHeader+'\');\">' + m + '月' +s + '日</a>';
                		}
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
                		t= '<a href=\"javascript:;\" onclick=\"javascript:reloadPersonWeekWin(\''+currentHeader+'\');\">' + currentHeader + '</a>';
                	if(currentHeader == '合计')
                		t = '<font color=red>' + currentHeader + '</font>';
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
    	com.bitc.xmis.accounting.PersonWeekAxis.superclass.initComponent.call(this);
    }
});
com.bitc.xmis.accounting.PersonWeekAccountGrid = Ext.extend(Ext.grid.PivotGrid, {
	initAxes: function() {
        var PivotAxis = com.bitc.xmis.accounting.PersonWeekAxis;
        
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
    PERSONINDEPT : "personWeek",
    PERSONINCOMP : "getPersonOfCompInOneWeek",
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
    			var start = this.startMonth;
    			var s = start.split('-');
    			var d = '中水科技 ';
    			var time = value;
    			if(time == '小计'){
    				time = s[0] + '年' + s[1] + '月登录总次数前20名人员名单';
    			}else{
    				var ds = value.split('Z');
    				var starts = ds[0].split('-');
    				var ends = ds[1].split('-');
    				time = starts[0] + '年' + starts[1] + '月'+ starts[2]+ '日至'+ends[0] + '年' + ends[1] + '月'+ ends[2]+ '日'+'登录次数前20名人员名单';
    			}
    			title = d + time;
    			this.win.setTitle(title);
    		}else{
    			var title = '';
        		if(this.startMonth == '小计'){
        			title = this.deptName + ' 人员登录小计信息';
            	}else{
            		var s = this.startMonth.split('-');
                	var startYear = parseInt(s[0]);
            		var startMonth = parseInt(s[1]);
            		title = this.deptName + '' + startYear + '年' + startMonth +'月人员登录统计信息';
            	}
        		this.win.setTitle(title);
    		}
    		
    	}
    },
    stripeRows : true,
	realSortColumn : undefined,
	startMonth : undefined,
	endMonth : undefined,
	sortWeek : undefined,
	deptID : undefined,
	deptName : undefined,
	searchType : undefined,
	win : undefined,
	initComponent : function() {
		var grid = this;
		grid.generateWinTitle(grid.sortWeek);
		var record = Ext.data.Record.create([
		    {name: 'userID',type: 'string'},
		    {name: 'userName',type: 'string'},
		    {name: 'weeks',type: 'string'},
		    {name: 'total',type: 'int'},
		    {name: 'weeknum',type: 'string'},
		    {name: 'userPx',type: 'int'}
		]);
		this.store = new Ext.data.Store({
					url : __cxtPath + '/developProgressMaint.do?method=getMonthAccounting1',
					autoLoad: true,
			        baseParams : {
			        	searchType : grid.generateSearchStr(),
			        	start : grid.startMonth,
			        	end : grid.endMonth,
			        	sortWeek : grid.sortWeek,
			        	deptID : grid.deptID,
			        	deptName : grid.deptName
			        },
			        reader: new Ext.data.JsonReader({
			            root: 'rows'
			        }, record)
				});
		this.leftAxis = [new com.bitc.xmis.accounting.PersonAxis({
		    dataIndex: 'userName',
		    realSortColumn : 'userPx',
		    direction : 'ASC',
		    width : 70
		})];
		this.topAxis = [ new com.bitc.xmis.accounting.PersonAxis({
		    dataIndex: 'weeknum',
		    align : 'center'
		}),new com.bitc.xmis.accounting.PersonAxis({
		    dataIndex: 'weeks',
		    align : 'center'
		 })];
		
		window.reloadPersonWeekWin = function(month){
	    	grid.store.setBaseParam('sortWeek',month);
	    	grid.setSortWeekValue(month);
	    	grid.store.load();
	    };
		
		this.aggregator = function(records, measure) {
        	var result;
            var total = records[0].get(measure);
            result = '<div style=\"text-align:center;height:26px;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
            if("合计" == records[0].get('userName'))
            	result = '<div style=\"text-align:center;height:26px;background-color: #f1f2f4;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
            if("小计" == records[0].get('weeks'))
            	result = '<div style=\"text-align:center;height:26px;background-color: #f1f2f4;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
            if(grid.sortWeek == records[0].get('weeks'))
            	result = '<div style=\"text-align:center;height:26px;background-color: #ffff00;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
            
            return result;
        };
        this.measure = 'total';
        this.viewConfig = {
            title : '员工姓名'
        };
        this.renderer = function(value) {
            return value;
        };
        
        
        com.bitc.xmis.accounting.PersonWeekAccountGrid.superclass.initComponent.call(this);
	}
});
com.bitc.xmis.accounting.PersonWeekAccountWindow = Ext.extend(Ext.Window, {
	width : 600,
	height : 450,
	layout : 'border',
	startMonth : undefined,
	endMonth : undefined,
	deptName : undefined,
	sortWeek : undefined,
	deptID : undefined,
	searchType : undefined,
	initComponent : function() {
		var win = this;
		this.items = [new com.bitc.xmis.accounting.PersonWeekAccountGrid({
			loadMask : true, // 读取数据时的遮罩和提示功能，默认为false
			loadMask : {
				msg : '正在加载数据，请稍侯……'
			},
			region : 'center',
			startMonth : win.startMonth,
			endMonth : win.endMonth,
			sortWeek : win.sortWeek,
			deptID : win.deptID,
			deptName : win.deptName,
			searchType : win.searchType,
			win : win
		})];
		com.bitc.xmis.accounting.PersonWeekAccountWindow.superclass.initComponent.call(this);
	}
});


//--------------------------------------------------------------------------
com.bitc.xmis.accounting.SubDeptAxis = Ext.extend(Ext.grid.PivotAxis, {
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
//                    html: '<div style=\"height:24px;padding-top:5px;\">' + cols[j].header + '</div>',
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
                		t = '<a href=\"javascript:;\" onclick=\"javascript:reloadSubDeptMonthWin(\''+previousHeader+'\');\">' + parseInt(previousHeader.substring(5,8)) + '月</a>';
                	}else if(/^\d{4}$/.test(previousHeader)){
                		t = previousHeader+ '年';
                	}else{
                		t = '<a href=\"javascript:;\" onclick=\"javascript:showPersonMonthWin(\''+previousHeader+'\',\'personMonth\');\">' + previousHeader + '</a>';
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
                		t = '<a href=\"javascript:;\" onclick=\"javascript:reloadSubDeptMonthWin(\''+currentHeader+'\');\">' + currentHeader + '</a>';
                	if(currentHeader == '合计')
                		t = '<a href=\"javascript:;\" onclick=\"javascript:showPersonMonthWin(\'监控事业部\',\'dept\',undefined,\'小计\');\">' + currentHeader + '</a>';
                    
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
    	com.bitc.xmis.accounting.SubDeptAxis.superclass.initComponent.call(this);
    }
});
com.bitc.xmis.accounting.SubDeptMonthAccountGrid = Ext.extend(Ext.grid.PivotGrid, {
	initAxes: function() {
        var PivotAxis = com.bitc.xmis.accounting.SubDeptAxis;
        
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
    PERSONINDEPT : "subMonth",
    //设置排序月份的值
    setSortMonthValue : function(value){
    	this.sortMonth = value;
    	this.generateWinTitle(value);
    },
    generateSearchStr : function(){
    	return this.PERSONINDEPT;
    },
    generateWinTitle : function(value){
    	if(this.win){
    		var start = this.startMonthValue;
    		var end = this.endMonthValue;
    		var s = start.split('-');
    		var e = end.split('-');
    		var d = this.deptName + ' ';
    		var time = s[0] + '年' + s[1] + '月至' + e[0] + '年' + e[1] + '月人员登录统计信息';
    		title = d + time;
    		this.win.setTitle(title);
    	}
    },
    startMonthValue : undefined,
	endMonthValue : undefined,
	sortMonth : undefined,
	deptID : undefined,
	deptName : undefined,
	searchType : undefined,
	win : undefined,
    initComponent : function() {
    	var grid = this;
    	grid.generateWinTitle(grid.sortMonth);
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
    	                                         	sort : Ext.util.Format.date(new Date(),'Y-m'),
    	                                         	deptName : grid.deptName,
    	                                         	searchType : grid.generateSearchStr()
    	                                         },
    	                                         reader: new Ext.data.JsonReader({
    	                                             root: 'rows'
    	                                         }, SaleRecord)
    	                                     });
    	                                     
    	                                     window.reloadSubDeptMonthWin = function(month){
     	                             	    	grid.store.setBaseParam('sort',month);
     	                             	    	grid.sortMonth = month;
     	                             	    	grid.store.load();
     	                             	    };
    	                                     
    	                             		this.tbar = [new Ext.form.Label({
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
    	                             			value : grid.startMonthValue.substring(0,4),
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
    	                             			value : grid.startMonthValue.substring(5,8),
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
    	                             			value : grid.endMonthValue.substring(0,4),
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
    	                             			value : grid.endMonthValue.substring(5,8),
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
    	                     						}
    	                     						grid.startMonthValue = start;
    	                     						grid.endMonthValue = end;
    	                     						grid.setSortMonthValue(sort);
    	                     						grid.store.setBaseParam('start',start);
    	                     						grid.store.setBaseParam('end',end);
    	                     						grid.store.setBaseParam('sort',sort);
    	                     						grid.store.load();
    	                             			}
    	                             		}];
    	                             this.aggregator = function(records, measure) {
    	                            	 var result;
    	                                 var total = records[0].get(measure);
//    	                     			 if(total != 0 && "合计" != records[0].get('deptName')&& "小计" != records[0].get('loginmonth'))
//    	                     				 total = '<a href=\"javascript:;\" onclick=\"javascript:showPersonWeekWin(' + records[0].get('deptID') + ',\'' + records[0].get('loginmonth') + '\',\''+records[0].get('deptName')+'\');\">' + total + '</a>';
//    	                     			 if(grid.sortMonth == records[0].get('loginmonth')){
//    	                                 	result = '<div style=\"text-align:center;height:26px;background-color: #ffff00;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
//    	                                 }else if("合计" == records[0].get('deptName')){
//    	                                	 result = '<div style=\"text-align:center;height:26px;background-color: #f1f2f4;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
//    	                                 }else if("小计" == records[0].get('loginmonth')){
//    	                                	 result = '<div style=\"text-align:center;height:26px;background-color: #f1f2f4;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
//    	                                 }else{
//    	                                	 result = '<div style=\"text-align:center;height:26px;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
//    	                                 }
    	                                 if(total != 0 && "合计" != records[0].get('deptName')&& "小计" != records[0].get('loginmonth'))
    	                     				 total = '<a href=\"javascript:;\" onclick=\"javascript:showPersonWeekWin(' + records[0].get('deptID') + ',\'' + records[0].get('loginmonth') + '\',\''+records[0].get('deptName')+'\',\'dept\');\">' + total + '</a>';
    	                     			 if(grid.sortMonth == records[0].get('loginmonth')){
    	                     				 if("合计" == records[0].get('deptName')&&"小计" != records[0].get('loginmonth')){
    	                     					total = '<a href=\"javascript:;\" onclick=\"javascript:showPersonWeekWin(1,\'' + records[0].get('loginmonth') + '\',\''+grid.deptName+'\',\'dept\');\">' + total + '</a>';
    	                     					result = '<div style=\"text-align:center;height:26px;background-color: #ffff00;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
    	                     				 }else if("合计" == records[0].get('deptName')&&"小计" == records[0].get('loginmonth')){
    	                     					total = '<a href=\"javascript:;\" onclick=\"javascript:showPersonMonthWin(\''+grid.deptName+'\',\'dept\',undefined,\'小计\');\">' + total + '</a>';
    	                     					result = '<div style=\"text-align:center;height:26px;background-color: #ffff00;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
    	                     				 }else{
    	                     					if(total > 0)
    	                     						total = '<a href=\"javascript:;\" onclick=\"javascript:showPersonMonthWin(\''+records[0].get('deptName')+'\',\'dept\',undefined,\''+records[0].get('deptName')+'\');\">' + total + '</a>';
    	                     					result = '<div style=\"text-align:center;height:26px;background-color: #ffff00;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
    	                     				 }
    	                                 }else if("合计" == records[0].get('deptName')&& "小计" != records[0].get('loginmonth')){
    	                                	 total = '<a href=\"javascript:;\" onclick=\"javascript:showPersonWeekWin(1,\'' + records[0].get('loginmonth') + '\',\''+grid.deptName+'\',\'dept\');\">' + total + '</a>';
    	                                	 result = '<div style=\"text-align:center;height:26px;background-color: #f1f2f4;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
    	                                 }else if("小计" == records[0].get('loginmonth') && "合计" != records[0].get('deptName')){
    	                                	 if(total > 0)
    	                                		 total = '<a href=\"javascript:;\" onclick=\"javascript:showPersonMonthWin(\''+records[0].get('deptName')+'\',\'dept\',undefined,\'小计\');\">' + total + '</a>';
    	                                	 result = '<div style=\"text-align:center;height:26px;background-color: #f1f2f4;\"><div style=\"padding-top:6px;\">' + total + '</div></div>';
    	                                 }else if("小计" == records[0].get('loginmonth') && "合计" == records[0].get('deptName')){
    	                                	 total = '<a href=\"javascript:;\" onclick=\"javascript:showPersonMonthWin(\''+grid.deptName+'\',\'dept\',undefined,\'小计\');\">' + total + '</a>';
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
    	                                 new com.bitc.xmis.accounting.SubDeptAxis({
    	                                     dataIndex: 'year',
    	                                     align : 'center'
    	                                 }),
    	                                 new com.bitc.xmis.accounting.SubDeptAxis({
    	                                     dataIndex: 'loginmonth',
    	                                     align : 'center'
    	                                 })
    	                             ];
    	com.bitc.xmis.accounting.SubDeptMonthAccountGrid.superclass.initComponent.call(this);
	}
});
com.bitc.xmis.accounting.subMonthAccountWindow = Ext.extend(Ext.Window, {
	width : 600,
	height : 450,
	layout : 'border',
	startMonth : undefined,
	endMonth : undefined,
	sortMonth : undefined,
	deptName : undefined,
	initComponent : function() {
		var win = this;
		this.items = [new com.bitc.xmis.accounting.SubDeptMonthAccountGrid({
			loadMask : true, // 读取数据时的遮罩和提示功能，默认为false
			loadMask : {
				msg : '正在加载数据，请稍侯……'
			},
			region : 'center',
			startMonthValue : win.startMonth,
			endMonthValue : win.endMonth,
			sortMonth : win.sortMonth,
			deptName : win.deptName,
			win : win
		})];
		com.bitc.xmis.accounting.subMonthAccountWindow.superclass.initComponent.call(this);
	}
});