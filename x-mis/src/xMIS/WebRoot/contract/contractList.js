Ext.onReady(function(){
    Ext.QuickTips.init();
    
    var warningTotalCountRecord = Ext.data.Record.create([
    	{name: ContractTypeID},
                 {name: warningDays},
                 {name: warningCount}
    ]);
    
	var warningTotalCountStore=new Ext.data.Store({
		proxy: new Ext.data.HttpProxy({
			url: 'contractMain.do?method=warningTotalCountForSum'
		}),
		reader:new Ext.data.JsonReader({
			root:'rows',
			totalProperty: 'totalCount'
		},warningTotalCountRecord)
	});    
 	

	
	warningTotalCountStore.on('load',function(store,records,options){

		var without_def_count="", earlier_30_days_count="", earlier_15_days_count ="", earlier_7_days_count="";
		var current_date_count="", overtime_current_date_count="", overtime_more_7_days_count="";
		var overtime_more_15_days_count="", overtime_more_30_days_count = "";
		for(var i=0;i<records.length;i++){
		  var record = records[i];
		  
		  	
		  	switch(record.get(warningDays)){
		  		case Type_Warning_Without_Definition:
					without_def_count = "(" + record.get(warningCount) + ")";
				  	break;
		  		
		  		case Type_Warning_Earlier_30_Days:
					earlier_30_days_count = "(" + record.get(warningCount) + ")";
		  			break;

		  		case Type_Warning_Earlier_15_Days:
					earlier_15_days_count = "(" + record.get(warningCount) + ")";
		  			break;

		  		case Type_Warning_Earlier_7_Days:
					earlier_7_days_count = "(" + record.get(warningCount) + ")";
		  			break;

		  		case Type_Warning_Current_Date:
					current_date_count = "(" + record.get(warningCount) + ")";
		  			break;

		  		case Type_Warning_Overtime_Current_Date:
					overtime_current_date_count = "(" + record.get(warningCount) + ")";
		  			break;

		  		case Type_Warning_Overtime_More_7_Days:
					overtime_more_7_days_count = "(" + record.get(warningCount) + ")";
		  			break;

		  		case Type_Warning_Overtime_More_15_Days:
					overtime_more_15_days_count = "(" + record.get(warningCount) + ")";
		  			break;

		  		case Type_Warning_Overtime_More_30_Days:
					overtime_more_30_days_count = "(" + record.get(warningCount) + ")";
		  			break;
		  	}
		  	
		}

//   	var warningLengedBtn = 
//    	"<table class=grid cellSpacing=1 cellPadding=1><tr class=warning>" +
//    		"<td><input type=button class=warning_without_def value='"+Tip_Title_Warning_Without_Definition+without_def_count+"' onClick=warningFilter('"+Type_Warning_Without_Definition+"')></td>" +
//    		"<td><input type=button class=warning_earlier_30 value='"+Tip_Title_Warning_Earlier_30_Days+earlier_30_days_count+"' onClick=warningFilter('"+Type_Warning_Earlier_30_Days+"')></td>" +
//    		"<td><input type=button class=warning_earlier_15 value='"+Tip_Title_Warning_Earlier_15_Days+earlier_15_days_count+"' onClick=warningFilter('"+Type_Warning_Earlier_15_Days+"')></td>" +
//    		"<td><input type=button class=warning_earlier_7 value='"+Tip_Title_Warning_Earlier_7_Days+earlier_7_days_count+"' onClick=warningFilter('"+Type_Warning_Earlier_7_Days+"')></td>" +
//    		"<td><input type=button class=warning_current_date value='"+Tip_Title_Warning_Current_Date+current_date_count+"' onClick=warningFilter('"+Type_Warning_Current_Date+"')></td>" +
//    		"<td><input type=button class=warning_overtime_current_date value='"+Tip_Title_Warning_Overtime_Current_Date+overtime_current_date_count+"' onClick=warningFilter('"+Type_Warning_Overtime_Current_Date+"')></td>" +
//    		"<td><input type=button class=warning_overtime_7 value='"+Tip_Title_Warning_Overtime_More_7_Days+overtime_more_7_days_count+"' onClick=warningFilter('"+Type_Warning_Overtime_More_7_Days+"')></td>" +
//    		"<td><input type=button class=warning_overtime_15 value='"+Tip_Title_Warning_Overtime_More_15_Days+overtime_more_15_days_count+"' onClick=warningFilter('"+Type_Warning_Overtime_More_15_Days+"')></td>" +
//    		"<td><input type=button class=warning_overtime_30 value='"+Tip_Title_Warning_Overtime_More_30_Days+overtime_more_30_days_count+"' onClick=warningFilter('"+Type_Warning_Overtime_More_30_Days+"')></td>" +
//   		"</tr></table>"; 
   		

   	var warningLengedBtn = 
    	"<table class=grid cellSpacing=1 cellPadding=1><tr class=warning>" +
    		(without_def_count==""?"":
    		("<td><input type=button class=warning_without_def value='"+Tip_Title_Warning_Without_Definition+without_def_count+"' onClick=warningFilter('"+Type_Warning_Without_Definition+"')></td>")) +
    		(earlier_30_days_count==""?"":
    		("<td><input type=button class=warning_earlier_30 value='"+Tip_Title_Warning_Earlier_30_Days+earlier_30_days_count+"' onClick=warningFilter('"+Type_Warning_Earlier_30_Days+"')></td>")) +
    		(earlier_15_days_count==""?"":
    		("<td><input type=button class=warning_earlier_15 value='"+Tip_Title_Warning_Earlier_15_Days+earlier_15_days_count+"' onClick=warningFilter('"+Type_Warning_Earlier_15_Days+"')></td>")) +
    		(earlier_7_days_count==""?"":
    		("<td><input type=button class=warning_earlier_7 value='"+Tip_Title_Warning_Earlier_7_Days+earlier_7_days_count+"' onClick=warningFilter('"+Type_Warning_Earlier_7_Days+"')></td>")) +
    		(current_date_count==""?"":
    		("<td><input type=button class=warning_current_date value='"+Tip_Title_Warning_Current_Date+current_date_count+"' onClick=warningFilter('"+Type_Warning_Current_Date+"')></td>")) +
    		(overtime_current_date_count==""?"":
    		("<td><input type=button class=warning_overtime_current_date value='"+Tip_Title_Warning_Overtime_Current_Date+overtime_current_date_count+"' onClick=warningFilter('"+Type_Warning_Overtime_Current_Date+"')></td>")) +
    		(overtime_more_7_days_count==""?"":
    		("<td><input type=button class=warning_overtime_7 value='"+Tip_Title_Warning_Overtime_More_7_Days+overtime_more_7_days_count+"' onClick=warningFilter('"+Type_Warning_Overtime_More_7_Days+"')></td>")) +
    		(overtime_more_15_days_count==""?"":
    		("<td><input type=button class=warning_overtime_15 value='"+Tip_Title_Warning_Overtime_More_15_Days+overtime_more_15_days_count+"' onClick=warningFilter('"+Type_Warning_Overtime_More_15_Days+"')></td>")) +
    		(overtime_more_30_days_count==""?"":
    		("<td><input type=button class=warning_overtime_30 value='"+Tip_Title_Warning_Overtime_More_30_Days+overtime_more_30_days_count+"' onClick=warningFilter('"+Type_Warning_Overtime_More_30_Days+"')></td>")) +
   		"</tr></table>"; 
		
		legendMsg.setText(warningLengedBtn, false);
		
		
	},this,{delay:100});	
    
    
	 var legendMsg = new Ext.form.Label({
//	 	html: warningLengedBtn
	 });       
	 
     var searchCompField = new Ext.ux.form.SearchField({
     	id: 'searchCompField',
		//store: pcStore,
		width: 155,
		
		onTrigger2Click : function(){//点击查询按钮或回车调用该方法 

			//peasonGrid.getView().expandAllGroups();
			pcStore.removeAll();
			//pcStore.proxy.setUrl('contractMain.do?method=getContractListForParentStroe');
			pcStore.setBaseParam("searchName", this.getRawValue());
			pcStore.setBaseParam("warningType", 0);
			
			pcStore.load({
				params:{
					start:0, 
					limit:20
				},
 			callback:function(record, options ,success){
				if(success == true){
				}else{
					Ext.Msg.alert(Ext_errMsg_For_Get_Store_Title,Ext_errMsg_For_Get_Store_Message);
				}
			}
			});  
			
//			warningTotalCountStore.setBaseParam("searchName", this.getRawValue());
//			warningTotalCountStore.load();
			
		}   
    });
    
    

    var pcRecord = Ext.data.Record.create([
                 {name: ContractTypeID},
                 {name: ContractID},
                 {name: ContractNo},
                 {name: ContractName},
                 {name: Abbreviation},
                 {name: ParentID},
                 {name: ParentContractAbb},
                 {name: SignatureDate},
                 {name: BuyerID},
                 {name: BuyerAbb},
                 {name: DeptIDOfBuyer},
                 {name: DeptNameOfBuyer},
                 {name: VendorID},
                 {name: VendorAbb},
                 {name: DeptIDOfVendor},
                 {name: DeptNameOfVendor},
                 {name: ContractAmt},
                 {name: BuyerRepName},
                 {name: VendorRepName},
                 {name: GovermentContractSourceName},
                 {name: ContractSummary},
                 {name: Remark},
                 {name: RepName},
                 {name: UpdateDate},
                 {name: ProjectID},
                 {name: ProjectName},
                 {name: CurrenyGraph},
                 {name: TotalAmtByID},
                 {name: UnChargedAmtTotal},
                 {name: invoiceTotalAmt},
                 {name: invoiceUnAccountAmt},
                 
                 {name: warningDate},
                 {name: warningAmt},
                 {name: warningDays}
                 //{name:'pageNo'}
    
    ]);
    
	var pcStore=new Ext.data.Store({
		proxy: new Ext.data.HttpProxy({
			url:'contractMain.do?method=getContractListByType' 
		}),
		remoteSort:true,
		sortInfo:{field: Abbreviation, direction: 'ASC'},
		reader:new Ext.data.JsonReader({
			root:'rows',
			totalProperty: 'totalCount'
		},pcRecord)
	});

	
	Ext.grid.RowNumberer = Ext.extend(Ext.grid.RowNumberer, {
	    renderer:function(value, cellmeta, record, rowIndex, columnIndex, store){
	        return store.lastOptions.params.start + rowIndex + 1;   
	    }   
	}); 

	
	var pcColM=new Ext.grid.ColumnModel(
		[
		new Ext.grid.RowNumberer(),
		{header: "合同编号",dataIndex:ContractNo, sortable:true, renderer: biuldLinker},
		{header: "合同名称",dataIndex:ContractName, sortable:true, renderer: biuldLinker},
		{header: "简称", id: 'abb',  dataIndex:Abbreviation, sortable:true, renderer: biuldLinker},
		{header: "父合同ID",dataIndex:ParentID, sortable:true, hidden:true},
		{header: "父合同简称",dataIndex:ParentContractAbb, sortable:true, renderer: biuldLinker},
		{header: "签订日期",dataIndex:SignatureDate, sortable:true, renderer: customer_formatDate, align: 'center'},
		{header: "甲方公司ID",dataIndex: BuyerID,
         
         hidden:true},
		{header: "甲方公司",dataIndex: BuyerAbb, sortable:true, renderer: biuldLinker, hidden: true},
		
		{header: "乙方公司ID",dataIndex: VendorID, hidden:true},
		{header: "乙方公司",dataIndex: VendorAbb, sortable:true, renderer: biuldLinker, hidden: true},
		
		
		{header: "<font color="+Color_Font_Contract_Total_Amt+">合同金额</font>",dataIndex:ContractAmt, sortable:true, 
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){

				return formatCurrency(record.data[CurrenyGraph], value, Color_Font_Contract_Total_Amt);
			}, 
			align: 'right'},
		
		{id: TotalAmtByID ,header: "<font color="+Color_Font_Contract_Total_Charged_Amt+">已收合同款</font>",dataIndex:TotalAmtByID, sortable:true, 
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
					return formatCurrency(record.data[CurrenyGraph], 
								value+'^onClick=showPaymentList(\''+ convertNameWithoutLinker(record.data[Abbreviation])+ '\','+record.data[ContractID]+')' +
										'   id=\'paymentListLink_'+record.data[ContractID] + '\'', Color_Font_Contract_Total_Charged_Amt);
			}, 
			align: 'right'},
			
		{header: "<font color="+Color_Font_Contract_Total_Invoiced_Amt+">已开发票金额</font>",dataIndex:invoiceTotalAmt, sortable:true, 
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
					return formatCurrency(record.data[CurrenyGraph], 
								value+'^onClick=showInvoiceList(\''+ convertNameWithoutLinker(record.data[Abbreviation])+ '\','+record.data[ContractID]+')' +
										'   id=\'invoiceListLink_'+record.data[ContractID] + '\'', Color_Font_Contract_Total_Invoiced_Amt);
					//return formatCurrency(record.data[CurrenyGraph], value, '#FF00FF');
			}, 
			align: 'right'},
			
		{header: "<font color="+Color_Font_Contract_Total_UnAccount_Amt+">已开票未到账金额</font>",dataIndex:invoiceUnAccountAmt, sortable:true, 
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
				
					return formatCurrency(record.data[CurrenyGraph], value, Color_Font_Contract_Total_UnAccount_Amt);
//					return  record.data[invoiceTotalAmt] == "0" ? formatCurrency(record.data[CurrenyGraph], record.data[invoiceTotalAmt], Color_Font_Contract_Total_UnAccount_Amt): 
//					formatCurrency(record.data[CurrenyGraph], value, Color_Font_Contract_Total_UnAccount_Amt);
			}, 
			align: 'right'},

		
		{header: "<font color="+Color_Font_Contract_Total_UnCharged_Amt+">未收合同款</font>",dataIndex:UnChargedAmtTotal, sortable:true, 
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
					cellMeta.attr = setWarningBackgroudColor(record.data[warningDate],record.data[warningAmt],record.data[warningDays],record.data[CurrenyGraph]);
				
					//return formatCurrency(record.data[CurrenyGraph], value, Color_Font_Contract_Total_UnCharged_Amt);
					return formatCurrency(record.data[CurrenyGraph], 
								value+'^onClick=showPaymentOption(\''+ convertNameWithoutLinker(record.data[Abbreviation])+ '\','+record.data[ContractID]+','+record.data[ContractTypeID]+')' +
										'   id=\'showPaymentOption_'+record.data[ContractID] + '\'', Color_Font_Contract_Total_UnCharged_Amt);
					
			}, 
			align: 'right'},
		
		{header: "甲方代表", dataIndex: BuyerRepName, sortable:true, renderer: biuldLinker, hidden:true},
		{header: "乙方代表", dataIndex: VendorRepName, sortable:true, renderer: biuldLinker, hidden:true},
		
		{header: "合同概要", dataIndex: ContractSummary, hidden:true},
		{header: "备注", dataIndex: Remark, hidden:true},
		
		{header: "登记人", dataIndex: RepName, sortable:true, renderer: biuldLinker, hidden:true},
		{header: "更新日期", dataIndex: UpdateDate, sortable:true, renderer: customer_formatDate, align: 'center', hidden:true},
		
		{header: "项目名称",dataIndex:ProjectName, sortable:true, renderer: biuldLinker},
		{header: "横向来源名称",dataIndex:GovermentContractSourceName, sortable:true, renderer: biuldLinker, hidden:true}
		]
	);  
	

	var incomeGrid = new Ext.grid.GridPanel({
		cm:pcColM,
		store:pcStore,
		autoExpandColumn: 'abb',
      	enableColumnMove:false,      //True表示启动对Column顺序的拖拽,反之禁用    
       	enableColumnResize:false,      //设置为false可以禁止这个表格的列改变宽度 (默认值为 true).    
		stripeRows: true,	//True表示使用不同颜色间隔行，默认为false
		autoHeight: false,  
		autoWidth: false,
		//height:Ext.get("pcWin").getHeight(),
		height: 505,
		width: Ext.get("contractList").getWidth(),
       	// config options for stateful behavior
       	stateful: true,
       	stateId: 'grid',  
       	forceFit:true, //Ture表示自动扩展或缩小列的宽度以适应grid的宽度,从而避免出现水平滚动条.   
		loadMask:true,      //读取数据时的遮罩和提示功能，默认为false   
       	loadMask: {msg:'正在加载数据，请稍侯……'},
       	viewConfig: { autoFill: true},
  

        tbar: [
        	'合同简称 / 合同编号: ', ' ',
        	searchCompField,'->',legendMsg],    

		// paging bar on the bottom
       	bbar: new Ext.PagingToolbar({
			pageSize: 20,
			store: pcStore,
			displayInfo: true,
			displayMsg: '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
			emptyMsg: "无显示数据"
        })
    });
    
    incomeGrid.getColumnModel().on('hiddenchange',function(cm,columnIndex,hidden){   
         
     });   
    
	  


        var tabs = new Ext.TabPanel({            
	        applyTo: 'contractList',
			width: Ext.get("contractList").getWidth(),
	        height: 535,
            region: 'center',                    
            margins:'3 3 3 0',                   
            activeTab: 1,                        
            //defaults:{autoScroll:true},          
                                                 
            items:[
	    	    {id: Type_ContractType_Income, title:'收入合同', items: [incomeGrid]},
    	    	{id: Type_ContractType_Payout, title:'支出合同'}
            ]                                  
        });   
		loadData(Type_ContractType_Income);
		
		function loadData(type){
		
			searchCompField.setValue("");
			pcStore.setBaseParam("searchName", "");
			pcStore.setBaseParam("contractTypeID", type);
			pcStore.setBaseParam("warningType", 0);
			
			pcStore.load({
				params:{
					start:0, 
					limit:20
					},
		  			callback:function(record, options ,success){
		  				
					if(success == true){
					}else{
						Ext.Msg.alert(Ext_errMsg_For_Get_Store_Title,Ext_errMsg_For_Get_Store_Message);
					}
				}
			});  	
			
			
			if(tabs.getActiveTab().getId() == Type_ContractType_Income){
				warningTotalCountStore.setBaseParam("searchName", "");
				warningTotalCountStore.setBaseParam("contractTypeID", type);
				warningTotalCountStore.setBaseParam("warningType", "");
				warningTotalCountStore.setBaseParam("searchType", Type_Contract_Integration_Simple);
				warningTotalCountStore.setBaseParam("begDate", '');
				warningTotalCountStore.setBaseParam("endDate", '');
				warningTotalCountStore.load();
			}else{
				warningTotalCountStore.removeAll();
//				legendMsg.setText("");
			}
		}

         tabs.on('tabchange',function(tabpanel,panel){
         	loadData(panel.getItemId()); 
			
			       
        	panel.removeAll();
        	if(Ext.getCmp('paymentOptionTabs'))Ext.getCmp('paymentOptionTabs').destroy();
        	
        	if(panel.getItemId() == Type_ContractType_Income){
        		incomeGrid.getColumnModel().setColumnHeader(12,"<font color="+Color_Font_Contract_Total_Charged_Amt+">已收合同款</font>");
        		incomeGrid.getColumnModel().setColumnHeader(13,"<font color="+Color_Font_Contract_Total_Invoiced_Amt+">已开发票金额</font>");
        		incomeGrid.getColumnModel().setHidden(14,false);
        		incomeGrid.getColumnModel().setColumnHeader(15,"<font color="+Color_Font_Contract_Total_UnCharged_Amt+">未收合同款</font>");
        		legendMsg.setVisible(true);
        	}        		
        	else if(panel.getItemId() == Type_ContractType_Payout){
        		incomeGrid.getColumnModel().setColumnHeader(12,"<font color="+Color_Font_Contract_Total_Charged_Amt+">已付合同款</font>");
        		incomeGrid.getColumnModel().setColumnHeader(13,"<font color="+Color_Font_Contract_Total_Invoiced_Amt+">已报销金额</font>");
        		incomeGrid.getColumnModel().setHidden(14,true);
        		incomeGrid.getColumnModel().setColumnHeader(15,"<font color="+Color_Font_Contract_Total_UnCharged_Amt+">未付合同款</font>");
        		legendMsg.setVisible(false);
        		
        	}
        	
        	
         	panel.add({items: incomeGrid});
         	tabpanel.doLayout();
//         	panel.doLayout();
		 });

                         
   	this.warningFilter = function(type){
   		
   			pcStore.removeAll();
			searchCompField.setValue("");
			pcStore.setBaseParam("warningType", type);
			pcStore.load({
				params:{
					start:0, 
					limit:20
					},
		  			callback:function(record, options ,success){
		  				
					if(success == true){
					}else{
						Ext.Msg.alert(Ext_errMsg_For_Get_Store_Title,Ext_errMsg_For_Get_Store_Message);
					}
				}
			});  
   	};
	
   	
   				var paymentOptionWin;
//   	= new Ext.Window({ 
//					modal: true,
//					renderTo:Ext.getBody(), 
//					frame:true, 
//					plain:true, 
//					resizable:false, 
//					closeAction:"hide", 
//					//maximizable:true, 
//					closable:true, 
//					layout: 'fit',
//					bodyStyle:"padding:10px", 
//					width : 840,//'100%',
//					height: 380,
//					items:[
////						new Ext.Panel({
////							autoLoad: {url: 'contractMain.do?method=contract&contractID='+contractID+'&showType=paymentOption&d='+Math.random(), scripts: true, scope: this}
////						})
//					]
//				});
				
				
	this.showPaymentOption = function(contractAbb, contractID, paymentOptionType){
		
	    Ext.get('showPaymentOption_'+contractID).on('click', function(){
			
//	    	if(Ext.getCmp('paymentOptionTabs')) {
////	    		alert("true");
//	    		Ext.getCmp('paymentOptionTabs').destroy();
//	    	}

	    	if(paymentOptionWin) paymentOptionWin.destroy();
   				
   				paymentOptionWin= new Ext.Window({ 
   					title: contractAbb,
					modal: true,
					renderTo:Ext.getBody(), 
					frame:true, 
					plain:true, 
					resizable:false, 
					closeAction:"hide", 
					//maximizable:true, 
					closable:true, 
					layout: 'fit',
					bodyStyle:"padding:10px", 
					width : 840,//'100%',
					height: 400,
			        listeners:{
			            'beforehide' :function(){
			            	warningTotalCountStore.load();
			            	pcStore.reload();
			            }
			        },						
					items:[
						new Ext.Panel({
							id: 'paymentOptionPanel',
							height: 400,
							layout: 'fit',
							frame: true,
							autoLoad: {
								url: 'contractMain.do?method=contract&contractID='+contractID+'&showType=paymentOption&d='+Math.random(), 
								scripts: true, 
								scope: this
							}
						})
					]
				});	    	
	    	
//	    	paymentOptionWin.removeAll();
//	    	paymentOptionWin.add({
//				items: new Ext.Panel({
//							height: 380,
//							layout: 'fit',
//							frame: true,
//							autoLoad: {
//								url: 'contractMain.do?method=contract&contractID='+contractID+'&showType=paymentOption&d='+Math.random(), 
//								scripts: true, 
//								scope: this
//							}
//							
//						})

//	    		items: new Ext.TabPanel({     
//							//applyTo: "paymentOptionDiv", 
//					        height: 350,
//					        region: 'center',                    
//					        margins:'3 3 3 0',                   
//					        activeTab: 1,                        
//					        //defaults:{autoScroll:true},          
//					                                             
//							autoLoad: {
//								url: 'contractMain.do?method=contract&contractID='+contractID+'&showType=paymentOption&d='+Math.random(), 
//								scripts: true, 
//								scope: this
//							},
//					        items:[
//					    	    {id:'1', title:'付款条件手工设置'},
//						    	{id:'2', title:'收款信息登记'}
//					        ]                                  
//					    })   
//			});
//			paymentOptionWin.setTitle(contractAbb);
//			paymentOptionWin.doLayout();
			paymentOptionWin.show(); 
		
		});    			
		
	}   	
 });    