Ext.onReady(function(){
    //Ext.QuickTips.init();

	var childRecord = Ext.data.Record.create([
                 {name: childContractID},
                 {name: childContractNo},
                 {name: childContractName},
                 {name: childAbbreviation},
                 {name: childCurrenyGraph},
                 {name: childContractAmt},
                 {name: childExchangeRate},
                 {name: childTotalAmtByID},
                 {name: childUnChargedAmtTotal},        
                 {name: childSignatureDate},
                 {name: childInvoiceTotalAmt},
                 {name: childInvoiceUnAccountAmt}
    
    ]);
    
	var childStore=new Ext.data.Store({
		proxy: new Ext.data.HttpProxy({
			url:'contractMain.do?method=getChildContractListByID' 
		}),
		remoteSort:false,
		sortInfo:{field: childAbbreviation, direction: 'ASC'},
		reader:new Ext.data.JsonReader({
			root:'rows',
			totalProperty: 'totalCount'
		},childRecord)
	});

	childStore.load({
		params:{
			parentID: parentID
		},			
		callback:function(record, options ,success){
			if(success == true){
			}else{
				Ext.Msg.alert(Ext_errMsg_For_Get_Store_Title ,Ext_errMsg_For_Get_Store_Message );
			}
		}
	}); 
	/*
	Ext.grid.RowNumberer = Ext.extend(Ext.grid.RowNumberer, {
	    renderer:function(value, cellmeta, record, rowIndex, columnIndex, store){
	        return store.lastOptions.params.start + rowIndex + 1;   
	    }   
	});
	*/ 


  var totalAmtMsg = new Ext.form.Label({
  });
  
  var totalAmt = 0, paymentAmt = 0, unPaymentAmt = 0, invoicedAmt = 0, invoicedUnAccount = 0;
  
  childStore.on('load',function(store,records,options){
    for(var i=0;i<records.length;i++){
      var record = records[i];
    //alert(formatCurrency("null",record.get(childExchangeRate),"")*formatCurrency("null",record.get(childContractAmt),""));
      totalAmt += formatCurrency("null",record.get(childExchangeRate),"")*formatCurrency("null",record.get(childContractAmt),"");
      paymentAmt += formatCurrency("null",record.get(childExchangeRate),"")*formatCurrency("null",record.get(childTotalAmtByID),"");
      unPaymentAmt += formatCurrency("null",record.get(childExchangeRate),"")*formatCurrency("null",record.get(childUnChargedAmtTotal),"");
      invoicedAmt +=  formatCurrency("null",record.get(childExchangeRate),"")*formatCurrency("null",record.get(childInvoiceTotalAmt),"");
      invoicedUnAccount +=  formatCurrency("null",record.get(childExchangeRate),"")*formatCurrency("null",record.get(childInvoiceUnAccountAmt),"");
    }
    //totalCount.setText("总共" + String.format('<font color="{0}">{1}</font>', "blue", store.getCount().toString())+" 份追加合同", false);
    totalAmtMsg.setText("已付约：" +formatCurrency("",paymentAmt.toString(),Color_Font_Contract_Total_Charged_Amt)
    	+ "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;未付约：" +formatCurrency("",unPaymentAmt.toString(),'red')
    	+ "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;已开票约：" +formatCurrency("",invoicedAmt.toString(),Color_Font_Contract_Total_Invoiced_Amt)
    	+ "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;未到账约：" +formatCurrency("",invoicedUnAccount.toString(),Color_Font_Contract_Total_UnAccount_Amt)
    	, false);
    	
    childGrid.setTitle("追加合同列表&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;共" 
    	+ String.format('<font color="{0}">{1}</font>', "blue", store.getCount().toString())+" 份追加合同，合同金额合计约：" 
    	+ formatCurrency("",totalAmt.toString(),Color_Font_Contract_Total_Amt) );
  },this,{delay:100})
  
	var childColM=new Ext.grid.ColumnModel(
		[
		new Ext.grid.RowNumberer(),

		{header: "合同简称", id: 'abb',  dataIndex:childAbbreviation, sortable:true, 
			renderer:function(value, cellMeta, record, rowIndex, columnIndex, store){

						cellMeta.attr = biuldTip("","合同名称："+record.data[childContractName]);
//						return biuldLinker(value);
						
						return showContractInfoSimple(value, record.data[childContractID]);
					} 
		},
		{header: "签订日期",dataIndex:childSignatureDate, sortable:true,	renderer:customer_formatDate, align: 'center'},
		
		{header: "合同金额",dataIndex:childContractAmt, sortable:true, 
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
					return formatCurrency(record.data[childCurrenyGraph], convertNameWithoutLinker(value), Color_Font_Contract_Total_Amt);
			}, 
			align: 'right'
		},
		
		{header: "已收合同金额",dataIndex:childTotalAmtByID, sortable:true, 
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
					return formatCurrency(record.data[childCurrenyGraph], value, Color_Font_Contract_Total_Charged_Amt);
			}, 
			align: 'right'
		},

		{header: "已开发票金额",dataIndex:childInvoiceTotalAmt, sortable:true, 
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
					return formatCurrency(record.data[childCurrenyGraph], value, Color_Font_Contract_Total_Invoiced_Amt);
			},
			
			/*
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
					return formatCurrency(record.data[childCurrenyGraph], 
								value+'^onClick=showInvoiceList(\''+ convertNameWithoutLinker(record.data[Abbreviation])+ '\','+record.data[ContractID]+')' +
										'   id=\'invoiceListLink_'+record.data[ContractID] + '\'', '#FF00FF');
					//return formatCurrency(record.data[CurrenyGraph], value, '#FF00FF');
			},
			*/ 
			align: 'right'},
			
		{header: "已开票未到账金额",dataIndex:childInvoiceUnAccountAmt, sortable:true, 
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
					return formatCurrency(record.data[childCurrenyGraph], value, Color_Font_Contract_Total_UnAccount_Amt);
			}, 
			align: 'right'},

		{header: "未收合同金额",dataIndex:childUnChargedAmtTotal, sortable:true, 
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
					return formatCurrency(record.data[childCurrenyGraph], value, 'red');
			}, 
			align: 'right'
		}
		
		]
	);  
	

	var childGrid = new Ext.grid.GridPanel({
		id: "childGrid",
		title: "追加合同列表",
		applyTo: "childContractList",
		cm: childColM,
		store: childStore,
		autoExpandColumn: 'abb',
		stripeRows: true,
		layout:'fit',
      	enableColumnMove:false,      //True表示启动对Column顺序的拖拽,反之禁用    
       	enableColumnResize:true,      //设置为false可以禁止这个表格的列改变宽度 (默认值为 true).    
		stripeRows: true,	//True表示使用不同颜色间隔行，默认为false
		autoHeight: false,  
		autoWidth: false,
		//height:Ext.get("pcWin").getHeight(),
		height: 185,//Ext.get("childContractList").getHeight(),
		width:Ext.get("childContractList").getWidth(),
		loadMask:true,      //读取数据时的遮罩和提示功能，默认为false   
       	// config options for stateful behavior
       	stateful: true,
       	stateId: 'grid',  
       	forceFit:true, //Ture表示自动扩展或缩小列的宽度以适应grid的宽度,从而避免出现水平滚动条.   
       	loadMask: {msg:'正在加载数据，请稍侯……'},
       	viewConfig: {forceFit: true},
       	collapsible: true,
        collapsed: true,

   

		// paging bar on the bottom
       	tbar: new Ext.Toolbar({
		    height: 28,
		    items: ['->',totalAmtMsg]
		    }) 
	  
	});
	

                   
	
	
 });    
 