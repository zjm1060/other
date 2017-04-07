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
 	
	warningTotalCountStore.setBaseParam("searchName", "");
	warningTotalCountStore.setBaseParam("contractTypeID", Type_ContractType_Income);
	warningTotalCountStore.setBaseParam("warningType", "");
	warningTotalCountStore.setBaseParam("searchType", Type_Contract_Integration_Simple);
	warningTotalCountStore.setBaseParam("begDate", '');
	warningTotalCountStore.setBaseParam("endDate", '');
	warningTotalCountStore.load();
	
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
		width: 155,
		
		onTrigger2Click : function(){//点击查询按钮或回车调用该方法 
			
			loadData(tabs.getActiveTab().getItemId(), this.getRawValue(), 0, Type_Contract_Integration_Simple);
 
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
                 {name: CurrencyID},
                 {name: TotalAmtByID},
                 {name: UnChargedAmtTotal},
                 {name: invoiceTotalAmt},
                 {name: invoiceUnAccountAmt},
                 {name: WarrantyEndDate},
                 
                 {name: warningDate},
                 {name: warningAmt},
                 {name: warningDays},
                 
                 {name: contractInvoiceAmtInTimeScope},
                 {name: contractPayedAmtInTimeScope},
                 {name: contractEstimateAmtInTimeScope}
                 //{name:'pageNo'}
    
    ]);
   
	Ext.grid.RowNumberer = Ext.extend(Ext.grid.RowNumberer, {
	    renderer:function(value, cellmeta, record, rowIndex, columnIndex, store){
	        return store.lastOptions.params.start + rowIndex + 1;   
	    }   
	}); 

	
	var pcColM=new Ext.grid.ColumnModel(
		[
		new Ext.grid.RowNumberer(),		//0
//		{header: "合同编号",dataIndex:ContractNo, sortable:true, renderer: biuldLinker},		//1
		{header: "合同编号",dataIndex:ContractNo, sortable:true, 		//1
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){

                 if(value==''||value == null){
                    return  '';
                 }else{  
                    return gridColOnClick( convertNameWithoutLinker(value)+'^onClick=showContractWin('+record.data[ContractID]+')');
                 }   
			}, 
			align: 'left'},
		
//		{header: "合同名称",dataIndex:ContractName, sortable:true, renderer: biuldLinker},	//2
		{header: "合同名称",dataIndex:ContractName, sortable:true, 		//2
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){

                 if(value==''||value == null){
                    return  '';
                 }else{  
                    return gridColOnClick( convertNameWithoutLinker(value)+'^onClick=showContractWin('+record.data[ContractID]+')');
                 }   
			}, 
			align: 'left'},
			
//		{header: "简称", id: 'abb',  dataIndex:Abbreviation, sortable:true, renderer: biuldLinker},		//3
		{header: "简称",id: 'abb', dataIndex:Abbreviation, sortable:true, 		//3
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){

                 if(value==''||value == null){
                    return  '';
                 }else{  
                    return gridColOnClick( convertNameWithoutLinker(value)+'^onClick=showContractWin('+record.data[ContractID]+')');
                 }   
			}, 
			align: 'left'},
			
			
//		{header: "父合同ID",dataIndex:ParentID, sortable:true, hidden:true},		
//		{header: "父合同简称",dataIndex:ParentContractAbb, sortable:true, renderer: biuldLinker},		//4
		{header: "父合同简称",dataIndex:ParentContractAbb, sortable:true, 		//4
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){

                 if(value==''||value == null){
                    return  '';
                 }else{  
                    return gridColOnClick( convertNameWithoutLinker(value)+'^onClick=showContractWin('+record.data[ParentID]+')');
                 }   
			}, 
			align: 'left'},
			
		{header: "签订日期",dataIndex:SignatureDate, sortable:true, renderer: customer_formatDate, align: 'center'},		//5
//		{header: "甲方公司ID",dataIndex: BuyerID, hidden:true},
		{header: "甲方公司",dataIndex: BuyerAbb, sortable:true, renderer: biuldLinker, hidden: true},		//6
		
//		{header: "乙方公司ID",dataIndex: VendorID, hidden:true},
//		{header: "乙方公司",dataIndex: VendorAbb, sortable:true, renderer: biuldLinker, hidden: true},
		
		
		{header: "<font color="+Color_Font_Contract_Total_Amt+">合同金额</font>",dataIndex:ContractAmt, sortable:true, 		//7
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){

//				return formatCurrency(record.data[CurrenyGraph], value, Color_Font_Contract_Total_Amt);
                 if(value==''||value == null){
                    return  '';
                 }else{  
                    return gridColOnClick( formatCurrency(record.data[CurrenyGraph], convertNameWithoutLinker(value), Color_Font_Contract_Total_Amt)+'^onClick=showContractWin('+record.data[ContractID]+')');
                 }   			
            }, 
			align: 'right'},
		
		{header: "<font color="+Color_Font_Contract_Total_Charged_Amt+">已收合同款</font>",dataIndex:TotalAmtByID, sortable:true,		//8 
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
					return formatCurrency(record.data[CurrenyGraph], 
								value+'^onClick=showPaymentList(\''+ convertNameWithoutLinker(record.data[Abbreviation])+ '\','+record.data[ContractID]+')' +
										'   id=\'paymentListLink_'+record.data[ContractID] + '\'', Color_Font_Contract_Total_Charged_Amt);
			}, 
			align: 'right'},
			
		{header: "实收合同金额",dataIndex:contractPayedAmtInTimeScope, sortable:true, 		//9
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
//					return formatCurrency(record.data[CurrenyGraph], 
//								value+'^onClick=showPaymentList(\''+ convertNameWithoutLinker(record.data[Abbreviation])+ '\','+record.data[ContractID]+')' +
//										'   id=\'paymentListLink_'+record.data[ContractID] + '\'', Color_Font_Contract_Total_Charged_Amt);
					
					return formatCurrency(record.data[CurrenyGraph], value, 'black');
				
			}, 
			align: 'right'},
			
		{header: "<font color="+Color_Font_Contract_Total_Invoiced_Amt+">已开发票金额</font>",dataIndex:invoiceTotalAmt, sortable:true, 		//10
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
					return formatCurrency(record.data[CurrenyGraph], 
								value+'^onClick=showInvoiceList(\''+ convertNameWithoutLinker(record.data[Abbreviation])+ '\','+record.data[ContractID]+')' +
										'   id=\'invoiceListLink_'+record.data[ContractID] + '\'', Color_Font_Contract_Total_Invoiced_Amt);
					//return formatCurrency(record.data[CurrenyGraph], value, '#FF00FF');
			}, 
			align: 'right'},
			
		{header: "实开发票金额",dataIndex:contractInvoiceAmtInTimeScope, sortable:true, 		//11
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
//					return formatCurrency(record.data[CurrenyGraph], 
//								value+'^onClick=showInvoiceList(\''+ convertNameWithoutLinker(record.data[Abbreviation])+ '\','+record.data[ContractID]+')' +
//										'   id=\'invoiceListLink_'+record.data[ContractID] + '\'', Color_Font_Contract_Total_Invoiced_Amt);
					return formatCurrency(record.data[CurrenyGraph], value, 'black');
			}, 
			align: 'right'},

		{header: "<font color="+Color_Font_Contract_Total_UnAccount_Amt+">已开票未到账金额</font>",dataIndex:invoiceUnAccountAmt, sortable:true, 	//12
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
//					alert(record.data[invoiceTotalAmt] );
					return  formatCurrency(record.data[CurrenyGraph], value, Color_Font_Contract_Total_UnAccount_Amt);
			}, 
			align: 'right'},

		
		{header: "<font color="+Color_Font_Contract_Total_UnCharged_Amt+">未收合同款</font>",dataIndex:UnChargedAmtTotal, sortable:true, 		//13
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
					cellMeta.attr = setWarningBackgroudColor(record.data[warningDate],record.data[warningAmt],record.data[warningDays],record.data[CurrenyGraph]);
				
					return formatCurrency(record.data[CurrenyGraph], value, Color_Font_Contract_Total_UnCharged_Amt);
//					return formatCurrency(record.data[CurrenyGraph], 
//								value+'^onClick=showPaymentOption(\''+ convertNameWithoutLinker(record.data[Abbreviation])+ '\','+record.data[ContractID]+','+record.data[ContractTypeID]+')' +
//										'   id=\'showPaymentOption_'+record.data[ContractID] + '\'', Color_Font_Contract_Total_UnCharged_Amt);
					
			}, 
			align: 'right'},
		
		{header: "预计来款",dataIndex:contractEstimateAmtInTimeScope, sortable:true, 		//14
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
					return formatCurrency(record.data[CurrenyGraph], value, 'black');
			}, 
			align: 'right'},

//		{header: "甲方代表", dataIndex: BuyerRepName, sortable:true, renderer: biuldLinker, hidden:true},
//		{header: "乙方代表", dataIndex: VendorRepName, sortable:true, renderer: biuldLinker, hidden:true},
//		
//		{header: "合同概要", dataIndex: ContractSummary, hidden:true},
//		{header: "备注", dataIndex: Remark, hidden:true},
//		
//		{header: "登记人", dataIndex: RepName, sortable:true, renderer: biuldLinker, hidden:true},
//		{header: "更新日期", dataIndex: UpdateDate, sortable:true, renderer: customer_formatDate, align: 'center', hidden:true},
		
		{header: "质保到期",dataIndex:WarrantyEndDate, sortable:true, renderer: customer_formatDate, align: 'center'}		//15
//		{header: "项目名称",dataIndex:ProjectName, sortable:true, renderer: biuldLinker},
//		{header: "横向来源名称",dataIndex:GovermentContractSourceName, sortable:true, renderer: biuldLinker, hidden:true}
		]
	);  

//	var integrationPerformedCM = new Ext.grid.ColumnModel(
//		[
//		new Ext.grid.RowNumberer(),
//		{header: "合同编号",dataIndex:ContractNo, sortable:true, renderer: biuldLinker},
//		{header: "合同名称",dataIndex:ContractName, sortable:true, renderer: biuldLinker},
//		{header: "简称", id: 'abb',  dataIndex:Abbreviation, sortable:true, renderer: biuldLinker},
//		{header: "父合同简称",dataIndex:ParentContractAbb, sortable:true, renderer: biuldLinker},
//		{header: "签订日期",dataIndex:SignatureDate, sortable:true, renderer: customer_formatDate, align: 'center'},
//		{header: "甲方公司",dataIndex: BuyerAbb, sortable:true, renderer: biuldLinker, hidden: true},
//		
//		{header: "<font color="+Color_Font_Contract_Total_Amt+">合同金额</font>",dataIndex:ContractAmt, sortable:true, 
//			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
//
//				return formatCurrency(record.data[CurrenyGraph], value, Color_Font_Contract_Total_Amt);
//			}, 
//			align: 'right'},
//		
//		{header: "<font color="+Color_Font_Contract_Total_Charged_Amt+">已收合同款</font>",dataIndex:TotalAmtByID, sortable:true, 
//			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
//					return formatCurrency(record.data[CurrenyGraph], 
//								value+'^onClick=showPaymentList(\''+ convertNameWithoutLinker(record.data[Abbreviation])+ '\','+record.data[ContractID]+')' +
//										'   id=\'paymentListLink_'+record.data[ContractID] + '\'', Color_Font_Contract_Total_Charged_Amt);
//			}, 
//			align: 'right'},
//			
//		{header: "实收合同金额",dataIndex:contractPayedAmtInTimeScope, sortable:true, 
//			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
////					return formatCurrency(record.data[CurrenyGraph], 
////								value+'^onClick=showPaymentList(\''+ convertNameWithoutLinker(record.data[Abbreviation])+ '\','+record.data[ContractID]+')' +
////										'   id=\'paymentListLink_'+record.data[ContractID] + '\'', Color_Font_Contract_Total_Charged_Amt);
//					
//					return formatCurrency(record.data[CurrenyGraph], value, 'black');
//				
//			}, 
//			align: 'right'},
//			
//			
//		{header: "<font color="+Color_Font_Contract_Total_Invoiced_Amt+">已开发票金额</font>",dataIndex:invoiceTotalAmt, sortable:true, 
//			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
//					return formatCurrency(record.data[CurrenyGraph], 
//								value+'^onClick=showInvoiceList(\''+ convertNameWithoutLinker(record.data[Abbreviation])+ '\','+record.data[ContractID]+')' +
//										'   id=\'invoiceListLink_'+record.data[ContractID] + '\'', Color_Font_Contract_Total_Invoiced_Amt);
//					//return formatCurrency(record.data[CurrenyGraph], value, '#FF00FF');
//			}, 
//			align: 'right'},
//			
//		{header: "实开发票金额",dataIndex:contractInvoiceAmtInTimeScope, sortable:true, 
//			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
////					return formatCurrency(record.data[CurrenyGraph], 
////								value+'^onClick=showInvoiceList(\''+ convertNameWithoutLinker(record.data[Abbreviation])+ '\','+record.data[ContractID]+')' +
////										'   id=\'invoiceListLink_'+record.data[ContractID] + '\'', Color_Font_Contract_Total_Invoiced_Amt);
//					return formatCurrency(record.data[CurrenyGraph], value, 'black');
//			}, 
//			align: 'right'},
//
//		{header: "<font color="+Color_Font_Contract_Total_UnAccount_Amt+">已开票未到账金额</font>",dataIndex:invoiceUnAccountAmt, sortable:true, 
//			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
////					alert(record.data[invoiceTotalAmt] );
//					return  formatCurrency(record.data[CurrenyGraph], value, Color_Font_Contract_Total_UnAccount_Amt);
//			}, 
//			align: 'right'},
//
//		
//		{header: "<font color="+Color_Font_Contract_Total_UnCharged_Amt+">未收合同款</font>",dataIndex:UnChargedAmtTotal, sortable:true, 
//			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
//					cellMeta.attr = setWarningBackgroudColor(record.data[warningDate],record.data[warningAmt],record.data[warningDays],record.data[CurrenyGraph]);
//				
//					//return formatCurrency(record.data[CurrenyGraph], value, Color_Font_Contract_Total_UnCharged_Amt);
//					return formatCurrency(record.data[CurrenyGraph], 
//								value+'^onClick=showPaymentOption(\''+ convertNameWithoutLinker(record.data[Abbreviation])+ '\','+record.data[ContractID]+','+record.data[ContractTypeID]+')' +
//										'   id=\'showPaymentOption_'+record.data[ContractID] + '\'', Color_Font_Contract_Total_UnCharged_Amt);
//					
//			}, 
//			align: 'right'},
//		
//		{header: "预计来款",dataIndex:contractEstimateAmtInTimeScope, sortable:true, 
//			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
//					return formatCurrency(record.data[CurrenyGraph], value, 'black');
//			}, 
//			align: 'right'},
//
//		
//		{header: "项目名称",dataIndex:ProjectName, sortable:true, renderer: biuldLinker, hidden:true},
//		{header: "横向来源名称",dataIndex:GovermentContractSourceName, sortable:true, renderer: biuldLinker, hidden:true}
//		]
//	);  	
//	
	var totalMsg = new Ext.form.Label({
	});

	var pcStore=new Ext.data.Store({
		proxy: new Ext.data.HttpProxy({
			url: 'contractMain.do?method=integrationSearch'
		}),
		remoteSort:true,
		sortInfo:{field: SignatureDate, direction: 'ASC'},
		reader:new Ext.data.JsonReader({
			root:'rows',
			totalProperty: 'totalCount'//,
//			messageProperty: 'sumMsg'
		},pcRecord)
	});
	
    var sumRecord = Ext.data.Record.create([
                 {name: ContractAmt},
                 {name: ContractAmtCount},
                 {name: CurrenyGraph},
                 {name: TotalAmtByID},
                 {name: UnChargedAmtTotal},
                 {name: invoiceTotalAmt},
                 {name: invoiceUnAccountAmt},
                 
                 {name: contractInvoiceAmtInTimeScope},
                 {name: contractPayedAmtInTimeScope},
                 {name: contractEstimateAmtInTimeScope},
                 {name: contractEstimateCountInTimeScope},
                 {name: contractInvoiceCountInTimeScope},
                 {name: contractPayedCountInTimeScope}
                 
    
    ]);
	var sumStore=new Ext.data.Store({
		proxy: new Ext.data.HttpProxy({
			url: 'contractMain.do?method=integrationSearchForSum'
		}),
		reader:new Ext.data.JsonReader({
			root:'rows',
			totalProperty: 'totalCount'
		},sumRecord)
	});    
 	


	sumStore.on('load',function(store,records,options){
		MM_findObj("sumTitle").innerHTML = "";	
//		MM_findObj("sumMsg").innerHTML = "";	
		var totalAmt="", changedTotalAmt="", InvoiceTotalAmt="", InvoiceUnAccountAmt="", unChargedAmtTotal="";
		var payedAmt="", invoicedAmt="", estimateAmt="";
		var totalInfo="", sumInfo="", currentColor="red";
		
		var blankPre = "&nbsp;&nbsp;";
		var totalAmtLabel = blankPre + ("合同金额合计：");
		var changedTotalAmtLabel = blankPre + (tabs.getActiveTab().getId() == Type_ContractType_Income ? "已收合同款金额合计：":"已付合同款金额合计：");
		var unChargedAmtTotalLabel = blankPre + (tabs.getActiveTab().getId() == Type_ContractType_Income ? "未收合同款金额合计：":"未付合同款金额合计：");
		var InvoiceTotalAmtLabel = blankPre + (tabs.getActiveTab().getId() == Type_ContractType_Income ? "已开发票金额合计：":"已报销金额合计：");
		var InvoiceUnAccountAmtLabel = blankPre + (tabs.getActiveTab().getId() == Type_ContractType_Income ? "已开票未到账金额合计：":"");
		
		var payedAmtLabel = blankPre + (tabs.getActiveTab().getId() == Type_ContractType_Income ? "实际收款合计：" : "实际付款合计：");
		var invoicedAmtLabel = blankPre + (tabs.getActiveTab().getId() == Type_ContractType_Income ? "实际开票合计：" : "实际报销合计：");
		var estimateAmtLable = blankPre + (tabs.getActiveTab().getId() == Type_ContractType_Income ? "预计来款合计：" : "");
		
		
		for(var i=0;i<records.length;i++){
		  var record = records[i];
		  
		  	
		  	switch(integrationTypeFlag){
		  		case Type_Contract_Integration_Simple:
		  		
				  totalAmt += convertNameWithoutLinker(record.get(ContractAmt))=="0"?"":formatCurrency(record.get(CurrenyGraph),convertNameWithoutLinker(record.get(ContractAmt))+"^onClick=integrationSearch(\'"+record.get(CurrenyGraph)+"\',\'"+Type_subIntegration_Nomal+"\')",currentColor)+"("+record.get(ContractAmtCount)+")" + ";&nbsp;&nbsp;&nbsp;&nbsp;";
				  changedTotalAmt += convertNameWithoutLinker(record.get(TotalAmtByID))=="0"?"":formatCurrency(record.get(CurrenyGraph),convertNameWithoutLinker(record.get(TotalAmtByID)),currentColor) + ";&nbsp;&nbsp;&nbsp;&nbsp;";
				  InvoiceTotalAmt += convertNameWithoutLinker(record.get(invoiceTotalAmt))=="0"?"":formatCurrency(record.get(CurrenyGraph),convertNameWithoutLinker(record.get(invoiceTotalAmt)),currentColor) + ";&nbsp;&nbsp;&nbsp;&nbsp;";
				  InvoiceUnAccountAmt += convertNameWithoutLinker(record.get(invoiceUnAccountAmt))=="0"?"":formatCurrency(record.get(CurrenyGraph),convertNameWithoutLinker(record.get(invoiceUnAccountAmt)),currentColor) + ";&nbsp;&nbsp;&nbsp;&nbsp;";
				  unChargedAmtTotal +=  convertNameWithoutLinker(record.get(UnChargedAmtTotal))=="0"?"":formatCurrency(record.get(CurrenyGraph),convertNameWithoutLinker(record.get(UnChargedAmtTotal)),currentColor) + ";&nbsp;&nbsp;&nbsp;&nbsp;";
				  
				  sumInfo = totalAmtLabel + totalAmt+ "<br>";
				  sumInfo += changedTotalAmtLabel + changedTotalAmt + "<br>";
				  sumInfo += unChargedAmtTotalLabel +　unChargedAmtTotal + "<br><br>";
				  sumInfo += InvoiceTotalAmtLabel + InvoiceTotalAmt + "<br>";
				  sumInfo += InvoiceUnAccountAmtLabel==""?"":(InvoiceUnAccountAmtLabel + InvoiceUnAccountAmt + "<br>");
		  		
				  break;
		  		
		  		case Type_Contract_Integration_New:

				  totalAmt += convertNameWithoutLinker(record.get(ContractAmt))=="0"?"":formatCurrency(record.get(CurrenyGraph),convertNameWithoutLinker(record.get(ContractAmt))+"^onClick=integrationSearch(\'"+record.get(CurrenyGraph)+"\',\'"+Type_subIntegration_Nomal+"\')",currentColor)+"("+record.get(ContractAmtCount)+")" + ";&nbsp;&nbsp;&nbsp;&nbsp;";
				  changedTotalAmt += convertNameWithoutLinker(record.get(TotalAmtByID))=="0"?"":formatCurrency(record.get(CurrenyGraph),convertNameWithoutLinker(record.get(TotalAmtByID)),currentColor) + ";&nbsp;&nbsp;&nbsp;&nbsp;";
				  InvoiceTotalAmt += convertNameWithoutLinker(record.get(invoiceTotalAmt))=="0"?"":formatCurrency(record.get(CurrenyGraph),convertNameWithoutLinker(record.get(invoiceTotalAmt)),currentColor) + ";&nbsp;&nbsp;&nbsp;&nbsp;";
				  InvoiceUnAccountAmt += convertNameWithoutLinker(record.get(invoiceUnAccountAmt))=="0"?"":formatCurrency(record.get(CurrenyGraph),convertNameWithoutLinker(record.get(invoiceUnAccountAmt)),currentColor) + ";&nbsp;&nbsp;&nbsp;&nbsp;";
				  unChargedAmtTotal +=  convertNameWithoutLinker(record.get(UnChargedAmtTotal))=="0"?"":formatCurrency(record.get(CurrenyGraph),convertNameWithoutLinker(record.get(UnChargedAmtTotal)),currentColor) + ";&nbsp;&nbsp;&nbsp;&nbsp;";
				  
				  sumInfo = totalAmtLabel + totalAmt+ "<br>";
				  sumInfo += changedTotalAmtLabel + changedTotalAmt + "<br>";
				  sumInfo += unChargedAmtTotalLabel +　unChargedAmtTotal + "<br><br>";
				  sumInfo += InvoiceTotalAmtLabel + InvoiceTotalAmt + "<br>";
				  sumInfo += InvoiceUnAccountAmtLabel==""?"":(InvoiceUnAccountAmtLabel + InvoiceUnAccountAmt + "<br>");
		  		  break;

		  		case Type_Contract_Integration_Performed:
		  		  
					payedAmt += convertNameWithoutLinker(record.get(contractPayedAmtInTimeScope))=="0"?"":formatCurrency(record.get(CurrenyGraph),convertNameWithoutLinker(record.get(contractPayedAmtInTimeScope))+"^onClick=integrationSearch(\'"+record.get(CurrenyGraph)+"\',\'"+Type_subIntegration_Payed+"\')",currentColor)+"("+record.get(contractPayedCountInTimeScope)+")" + ";&nbsp;&nbsp;&nbsp;&nbsp;";
					invoicedAmt += convertNameWithoutLinker(record.get(contractInvoiceAmtInTimeScope))=="0"?"":formatCurrency(record.get(CurrenyGraph),convertNameWithoutLinker(record.get(contractInvoiceAmtInTimeScope))+"^onClick=integrationSearch(\'"+record.get(CurrenyGraph)+"\',\'"+Type_subIntegration_Invoiced+"\')",currentColor)+"("+record.get(contractInvoiceCountInTimeScope)+")" + ";&nbsp;&nbsp;&nbsp;&nbsp;";
					estimateAmt += convertNameWithoutLinker(record.get(contractEstimateAmtInTimeScope))=="0"?"":formatCurrency(record.get(CurrenyGraph),convertNameWithoutLinker(record.get(contractEstimateAmtInTimeScope))+"^onClick=integrationSearch(\'"+record.get(CurrenyGraph)+"\',\'"+Type_subIntegration_Estimate+"\')",currentColor)+"("+record.get(contractEstimateCountInTimeScope)+")" + ";&nbsp;&nbsp;&nbsp;&nbsp;";
		  		  	
				  	sumInfo = payedAmtLabel + payedAmt + "<br>" 
				  	sumInfo += invoicedAmtLabel + invoicedAmt + "<br>"
				  	sumInfo += estimateAmtLable==""?"": (estimateAmt + "<br>");
		  			break;
		  	}
		  	
		}
		
		totalInfo += sumInfo ;
		
		totalMsg.setText(dateRange, false);
		
		if(totalInfo!=""){
			MM_findObj("sumTitle").innerHTML = "<h3>信息汇总</h3><br>" + totalInfo;
//			MM_findObj("sumMsg").innerHTML = totalInfo;		
			document.getElementById("end").scrollIntoView();
		}
		
	},this,{delay:100});	
	
	
	var incomeGrid = new Ext.grid.GridPanel({
		id: 'incomeGrid',
		cm:pcColM,
		store:pcStore,
		autoExpandColumn: 'abb',
		layout:'fit',
      	enableColumnMove:false,      //True表示启动对Column顺序的拖拽,反之禁用    
       	enableColumnResize:true,      //设置为false可以禁止这个表格的列改变宽度 (默认值为 true).    
		stripeRows: true,	//True表示使用不同颜色间隔行，默认为false
		autoHeight: false,  
		autoWidth: false,
		//height:Ext.get("pcWin").getHeight(),
		height: 480,
		width: Ext.get("contractList").getWidth(),
       	// config options for stateful behavior
       	stateful: true,
       	stateId: 'grid',  
       	forceFit:true, //Ture表示自动扩展或缩小列的宽度以适应grid的宽度,从而避免出现水平滚动条.   
		loadMask:true,      //读取数据时的遮罩和提示功能，默认为false   
       	loadMask: {msg:'正在加载数据，请稍侯……'},
       	viewConfig: {forceFit: true},

		// paging bar on the bottom
       	bbar: [
	        totalMsg,
	        '->',
	       	
	       	new Ext.PagingToolbar({
	       		id: 'integrationPagingbar',
				pageSize: 20,
				store: pcStore,
				displayInfo: true,
				displayMsg: '共 {2} 条记录',
				emptyMsg: "无显示数据"
	        })
        ]
    }); 
	  

        var tabs = new Ext.TabPanel({            
	        applyTo: 'contractList',
			width: Ext.get("contractList").getWidth(),
	        height: 535,
            region: 'center',                    
            margins:'3 3 3 0',                   
            activeTab: 1,                        
            //defaults:{autoScroll:true},          
 			tbar: ['合同简称 / 合同编号: ', ' ',searchCompField,'-',
	    	    	{
            			ref: '../conditionBtn',
			            iconCls: 'condition',
			            text: '条件设定',
			            handler: function(){
			            	Ext.getCmp('beginDate').setValue("");
			            	Ext.getCmp('endDate').setValue("");
			            	Ext.getCmp('searchConditionFrm').getForm().findField('searchType').setValue('1');
			            	searchConditionWin.show();
			            }
			        },'->',
	    	    	legendMsg],                                             
            items:[
	    	    {id:Type_ContractType_Income, title:'收入合同综合查询'},
    	    	{id:Type_ContractType_Payout, title:'支出合同综合查询'}
            ]                                  
        });   
        
        

    var dateTimeCondition = [ {
		baseCls : 'x-plain',
    	bodyStyle: 'padding-right:5px;',
        items: {
            xtype: 'fieldset',
            title: '日期定义',
            autoHeight: true,
            labelAlign: 'right',
            labelWidth: 60,
            fream: true,
            items: [                
            	new Ext.form.DateField({
			        id: 'beginDate',
			        format: 'Y-m-d',
//			        maxValue: new Date(),
			        minValue: '1900-01-01',
			        fieldLabel: '开始日期',
			        showToday:true ,
			        editable:false,
			        anchor: '100%'
			    })
				,
                new Ext.form.DateField({
			        id: 'endDate',
			        format: 'Y-m-d',
//			        maxValue: new Date(),
			        minValue: '1900-01-01',
			        fieldLabel: '结束日期',
			        showToday:true ,
			        editable:false,
			        anchor: '100%'
			    })
            ]
        }
    }];        
        var searchConditionWin= new Ext.Window({ 
   			title: '条件设定',
			modal: true,
			renderTo:Ext.getBody(), 
			frame:true, 
			plain:true, 
			resizable:false, 
			closeAction:"hide", 
			//maximizable:true, 
			closable:true, 
			layout:"form", 
			bodyStyle:"padding:10px", 
			width:280, 
//			height:100, 
			autoHeight: true,
	        				
			items: new Ext.form.FormPanel({
				id: 'searchConditionFrm',
			    baseCls : 'x-plain',
//			    anchor : '100%',
			    layout: 'form', 
			    hideLabels: true,
				items:[
					dateTimeCondition,
					{
						id: 'newContract',
		                xtype: 'radio',
		                boxLabel: '新签订的合同',
		                name: 'searchType',
		                inputValue: Type_Contract_Integration_New
		            }, {
		                xtype: 'radio',
		                boxLabel: '收付款情况', //<br>  已开发票金额<br>  已开票未到账金额',
		                name: 'searchType',
		                inputValue: Type_Contract_Integration_Performed
//		            }, {
//		                xtype: 'radio',
//		                boxLabel: '预计来款',
//		                name: 'searchType',
//		                inputValue: Type_Contract_Integration_EstimateContractAmt
		            }, {
		                xtype: 'radio',
		                boxLabel: '质保期到期',
		                name: 'searchType',
		                inputValue: Type_Contract_Integration_Warranty
		            }
				], 
				buttons:[
					{text:"确认",
						handler:function(){
							
							if(convertDateObjToFormatStr(Ext.getCmp('searchConditionFrm').getForm().findField('beginDate').getValue()) == "")
								Ext.Msg.alert("提示","请选择开始日期！");
							
							else if(convertDateObjToFormatStr(Ext.getCmp('searchConditionFrm').getForm().findField('endDate').getValue()) == "")
								Ext.Msg.alert("提示","请选择结束日期！");
								
							else if(!dateCompare(Ext.getCmp('searchConditionFrm').getForm().findField('beginDate').getValue(), Ext.getCmp('searchConditionFrm').getForm().findField('endDate').getValue()))
								Ext.Msg.alert("提示","结束日期必须晚于开始日期！");

							else if(Ext.getCmp('searchConditionFrm').getForm().findField('searchType').getGroupValue() == null)
								Ext.Msg.alert("提示","请选择查询类别！");

							else{
								loadData(tabs.getActiveTab().getItemId(), "", 0, Ext.getCmp('searchConditionFrm').getForm().findField('searchType').getGroupValue());
								searchConditionWin.hide();
							}

						}},
					{text:"取消",handler:function(){searchConditionWin.hide();}}
				]
		    }) 
		});	  
        
		var integrationTypeFlag, dateRange="", limitCount = 20, reconfigure = -1;
		function loadData(contractType, searchName, warningType, integrationType ){
			
			integrationTypeFlag = integrationType;
			searchCompField.setValue(searchName);
			tabs.getItem(tabs.getActiveTab().getItemId()).items.each(function(c){c.hide()});
			
			if(integrationType != Type_Contract_Integration_Simple)
				dateRange = convertDateObjToFormatStr(Ext.getCmp('searchConditionFrm').getForm().findField('beginDate').getValue()) 
					+ "至" + convertDateObjToFormatStr(Ext.getCmp('searchConditionFrm').getForm().findField('endDate').getValue());
			
			pcStore.setDefaultSort(	SignatureDate, 'ASC');
					
			pcStore.removeAll();
			pcStore.setBaseParam("currencyGraph","");
			pcStore.setBaseParam("searchName", searchName);
			pcStore.setBaseParam("contractTypeID", contractType);
			pcStore.setBaseParam("warningType", warningType);
			pcStore.setBaseParam("searchType", integrationType);
			pcStore.setBaseParam("begDate", '');
			pcStore.setBaseParam("endDate", '');
					
			sumStore.removeAll();
			sumStore.setBaseParam("searchName", searchName);
			sumStore.setBaseParam("contractTypeID", contractType);
			sumStore.setBaseParam("warningType", warningType);
			sumStore.setBaseParam("searchType", integrationType);
			sumStore.setBaseParam("begDate", '');
			sumStore.setBaseParam("endDate", '');
			
			switch (integrationType){
				case Type_Contract_Integration_Simple:

					dateRange = ""
				
//					if(reconfigure != Type_Contract_Integration_Simple && reconfigure != Type_Contract_Integration_New)
//					if(reconfigure == Type_Contract_Integration_Performed)
//						incomeGrid.reconfigure(pcStore,pcColM);
						
					
					break;
					
				case Type_Contract_Integration_New:
					
					dateRange += " 新签合同总统计"
				
					pcStore.setBaseParam("begDate", convertDateObjToFormatStr(Ext.getCmp('searchConditionFrm').getForm().findField('beginDate').getValue()));
					pcStore.setBaseParam("endDate", convertDateObjToFormatStr(Ext.getCmp('searchConditionFrm').getForm().findField('endDate').getValue()));

					sumStore.setBaseParam("begDate", convertDateObjToFormatStr(Ext.getCmp('searchConditionFrm').getForm().findField('beginDate').getValue()));
					sumStore.setBaseParam("endDate", convertDateObjToFormatStr(Ext.getCmp('searchConditionFrm').getForm().findField('endDate').getValue()));
		
//					if(reconfigure == Type_Contract_Integration_Performed)
//						incomeGrid.reconfigure(pcStore,pcColM);

					break;
					
				case Type_Contract_Integration_Performed:

					dateRange += " 合同收付款情况"

					pcStore.setBaseParam("begDate", convertDateObjToFormatStr(Ext.getCmp('searchConditionFrm').getForm().findField('beginDate').getValue()));
					pcStore.setBaseParam("endDate", convertDateObjToFormatStr(Ext.getCmp('searchConditionFrm').getForm().findField('endDate').getValue()));

					sumStore.setBaseParam("begDate", convertDateObjToFormatStr(Ext.getCmp('searchConditionFrm').getForm().findField('beginDate').getValue()));
					sumStore.setBaseParam("endDate", convertDateObjToFormatStr(Ext.getCmp('searchConditionFrm').getForm().findField('endDate').getValue()));

//					if(reconfigure != Type_Contract_Integration_Performed)
//						incomeGrid.reconfigure(pcStore,integrationPerformedCM);

					break;
					
				case Type_Contract_Integration_Warranty:

					dateRange += " 合同质保期到期"

					pcStore.setBaseParam("begDate", convertDateObjToFormatStr(Ext.getCmp('searchConditionFrm').getForm().findField('beginDate').getValue()));
					pcStore.setBaseParam("endDate", convertDateObjToFormatStr(Ext.getCmp('searchConditionFrm').getForm().findField('endDate').getValue()));

					sumStore.setBaseParam("begDate", convertDateObjToFormatStr(Ext.getCmp('searchConditionFrm').getForm().findField('beginDate').getValue()));
					sumStore.setBaseParam("endDate", convertDateObjToFormatStr(Ext.getCmp('searchConditionFrm').getForm().findField('endDate').getValue()));

//					if(reconfigure == Type_Contract_Integration_Performed)
//						incomeGrid.reconfigure(pcStore,pcColM);

					break;
			}
			
			pcStore.load({
				params:{
					start:0, 
					limit:limitCount
					},
		  			callback:function(record, options ,success){
		  				
						if(success == true){
							
						}else{
							Ext.Msg.alert(Ext_errMsg_For_Get_Store_Title,Ext_errMsg_For_Get_Store_Message);
						}
					}
			});  
			
			sumStore.load();
			
			
			tabs.getItem(tabs.getActiveTab().getItemId()).add(incomeGrid);
			incomeGrid.show();
			tabs.doLayout();
			
			reconfigure = integrationType;
		}


         tabs.on('tabchange',function(tabpanel,panel){
			searchCompField.setValue("");
			MM_findObj("sumTitle").innerHTML = "";	
			
			if(panel.getItemId() == Type_ContractType_Income){
				warningTotalCountStore.setBaseParam("searchName", "");
				warningTotalCountStore.setBaseParam("contractTypeID", Type_ContractType_Income);
				warningTotalCountStore.setBaseParam("warningType", "");
				warningTotalCountStore.setBaseParam("searchType", Type_Contract_Integration_Simple);
				warningTotalCountStore.setBaseParam("begDate", '');
				warningTotalCountStore.setBaseParam("endDate", '');
				warningTotalCountStore.load();
				
        		incomeGrid.getColumnModel().setColumnHeader(8,"<font color="+Color_Font_Contract_Total_Charged_Amt+">已收合同款</font>");
        		incomeGrid.getColumnModel().setColumnHeader(10,"<font color="+Color_Font_Contract_Total_Invoiced_Amt+">已开发票金额</font>");
        		incomeGrid.getColumnModel().setHidden(12,false);
        		incomeGrid.getColumnModel().setColumnHeader(13,"<font color="+Color_Font_Contract_Total_UnCharged_Amt+">未收合同款</font>");
        		legendMsg.setVisible(true);
        		
				
			}else{

        		incomeGrid.getColumnModel().setColumnHeader(8,"<font color="+Color_Font_Contract_Total_Charged_Amt+">已付合同款</font>");
        		incomeGrid.getColumnModel().setColumnHeader(10,"<font color="+Color_Font_Contract_Total_Invoiced_Amt+">已报销金额</font>");
        		incomeGrid.getColumnModel().setHidden(12,true);
        		incomeGrid.getColumnModel().setColumnHeader(13,"<font color="+Color_Font_Contract_Total_UnCharged_Amt+">未付合同款</font>");

//        		legendMsg.setText("");
        		legendMsg.setVisible(false);
        		
			}
				
			
			
//        	panel.removeAll();
         	tabpanel.doLayout();
		 });

                         
   	this.warningFilter = function(type){
   		
		loadData(tabs.getActiveTab().getItemId(), '', type, Type_Contract_Integration_Simple);
		MM_findObj("sumTitle").innerHTML = "";
		MM_findObj("sumMsg").innerHTML = "";		
		
	};
	
   	
	var paymentOptionWin;
				
				
	this.showPaymentOption = function(contractAbb, contractID, paymentOptionType){
		
	    Ext.get('showPaymentOption_'+contractID).on('click', function(){

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
			            'beforedestroy' :function(){
			                Ext.getCmp('paymentOptionTabs').destroy();
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
	    	
			paymentOptionWin.show(); 
		
		});    			
		
	}   
	
	pcStoreLoadForGraphy = function(currencyGraph, type){
			pcStore.setBaseParam("currencyGraph",currencyGraph);
			pcStore.setBaseParam("subIntegrationType",type);
			pcStore.load({
				params:{
					start:0, 
					limit:limitCount
					},
	  			callback:function(record, options ,success){
	  				
					if(success == true){
					}else{
						Ext.Msg.alert(Ext_errMsg_For_Get_Store_Title,Ext_errMsg_For_Get_Store_Message);
					}
				}
			});  		
	}
	
	
	pcStore.on('load',function(store,records,options){
		
		if(tabs.getActiveTab().getId() == Type_ContractType_Income){
		
			incomeGrid.getColumnModel().setHidden(4,false);		//父合同简称
			incomeGrid.getColumnModel().setHidden(9,false);		//实际收款
			incomeGrid.getColumnModel().setHidden(11,false);		//实际开票
			incomeGrid.getColumnModel().setHidden(14,false);		//预计来款
			incomeGrid.getColumnModel().setHidden(15,false);		//质保到期
					
			switch(reconfigure){
				case Type_Contract_Integration_Performed:
					incomeGrid.getColumnModel().setHidden(4,true);		//父合同简称
					incomeGrid.getColumnModel().setHidden(15,true);
					break;
	
				case Type_Contract_Integration_Warranty:
					incomeGrid.getColumnModel().setHidden(9,true);		//实际收款
					incomeGrid.getColumnModel().setHidden(11,true);		//实际开票
					incomeGrid.getColumnModel().setHidden(14,true);		//预计来款
					break;
	
				default:
					incomeGrid.getColumnModel().setHidden(9,true);		//实际收款
					incomeGrid.getColumnModel().setHidden(11,true);		//实际开票
					incomeGrid.getColumnModel().setHidden(14,true);		//预计来款
					incomeGrid.getColumnModel().setHidden(15,true);		//质保到期
					break;
			}
		}else if(tabs.getActiveTab().getId() == Type_ContractType_Payout){
			
		}
		
	}); 

	
	integrationSearch = function(currencyGraph, type){
		
		pcStoreLoadForGraphy(currencyGraph, type);
		
//		switch(integrationTypeFlag){
//		case Type_Contract_Integration_Simple:
//			pcStoreLoadForGraphy(currencyGraph);
//			break;
//		case Type_Contract_Integration_New:
//			pcStoreLoadForGraphy(currencyGraph);
//			break;
//		}

		
	}
 });    