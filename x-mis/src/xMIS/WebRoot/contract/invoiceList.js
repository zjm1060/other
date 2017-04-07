
Ext.onReady(function(){
    //Ext.QuickTips.init();

	var invoiceDetailRecord = Ext.data.Record.create([
                 {name: invoiceInvoiceDate},
                 {name: invoiceAEmpName},
                 {name: invoiceInvoiceAmt},
                 {name: invoiceBEmpName},
                 {name: invoiceUpdateDate},
                 {name: invoiceCurrencyGraph},
                 {name: invoiceCurrencyExchangeRate},
                 {name: invoiceRemark}
    
    ]);
    
	var invoiceDetailStore=new Ext.data.Store({
		proxy: new Ext.data.HttpProxy({
			url:'contractMain.do?method=getInvoiceList' 
		}),
		remoteSort:false,
		sortInfo:{field: invoiceInvoiceDate, direction: 'ASC'},
		reader:new Ext.data.JsonReader({
			root:'rows',
			totalProperty: 'totalCount'
		},invoiceDetailRecord)
	});



  
  invoiceDetailStore.on('load',function(store,records,options){
	var invoiceAmt = 0;
    for(var i=0;i<records.length;i++){
      var record = records[i];
      invoiceAmt += formatCurrency("null",record.get(invoiceCurrencyExchangeRate),"")*formatCurrency("null",record.get(invoiceInvoiceAmt),"");
    }
    	
    invoiceDetailGrid.setTitle("已开发票金额合计约:&nbsp;&nbsp;&nbsp;&nbsp;" + formatCurrency("",invoiceAmt.toString(),"blue"));
  },this,{delay:100})
  
  
	Ext.grid.RowNumberer = Ext.extend(Ext.grid.RowNumberer, {
	    renderer:function(value, cellmeta, record, rowIndex, columnIndex, store){
	        return rowIndex + 1;   
	    }   
	}); 
	
	
	var invoiceDetailColM=new Ext.grid.ColumnModel(
		[
		new Ext.grid.RowNumberer(),

		{header: "开票日期", dataIndex:invoiceInvoiceDate, sortable:true,	renderer:customer_formatDate, align: 'center'},
		
		{header: "开票金额", dataIndex:invoiceInvoiceAmt, sortable:true, 
			renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
					return formatCurrency(record.data[invoiceCurrencyGraph] , value, 'blue');
			}, 
			align: 'right'
		},

		{header: "经办人",dataIndex:invoiceAEmpName, sortable:true, renderer: convertUserName },
		{header: "登记日期", dataIndex:invoiceUpdateDate, sortable:true,	renderer:customer_formatDate, align: 'center'},
		{header: "登记人",dataIndex:invoiceBEmpName, sortable:true, renderer: convertUserName },
		{header: "备注", dataIndex: invoiceRemark, sortable: true}
		
		]
	);  
	

	var invoiceDetailGrid = new Ext.grid.GridPanel({
		id: 'invoiceDetailGrid',
		title: '已开发票金额明细',
		cm: invoiceDetailColM,
		store: invoiceDetailStore,
		//autoExpandColumn: 'abb',
		stripeRows: true,
		layout:'fit',
      	enableColumnMove:false,      //True表示启动对Column顺序的拖拽,反之禁用    
       	enableColumnResize:true,      //设置为false可以禁止这个表格的列改变宽度 (默认值为 true).    
		stripeRows: true,	//True表示使用不同颜色间隔行，默认为false
		autoHeight: false,  
		autoWidth: false,
		//height:Ext.get("pcWin").getHeight(),
		height: 300,//Ext.get("childContractList").getHeight(),
		width: '100%',
		loadMask:true,      //读取数据时的遮罩和提示功能，默认为false   
       	// config options for stateful behavior
       	stateful: true,
       	stateId: 'grid',  
       	forceFit:true, //Ture表示自动扩展或缩小列的宽度以适应grid的宽度,从而避免出现水平滚动条.   
       	loadMask: {msg:'正在加载数据，请稍侯……'},
       	viewConfig: {forceFit: true}
       	//collapsible: true,
        //collapsed: true,

	});
	
	
    var invoiceDetailWin;
    
	this.showInvoiceListWindow = function(contractAbb, contractID, type){
		
		
	    Ext.get('invoiceListLink_'+contractID).on('click', function(){
	         if(!invoiceDetailWin){
				invoiceDetailWin= new Ext.Window({ 
					modal: true,
					title: "已开发票明细", 
					renderTo:Ext.getBody(), 
					frame:true, 
					plain:true, 
					resizable:false, 
					closeAction:"hide", 
					//maximizable:true, 
					closable:true, 
					bodyStyle:"padding:10px", 
					width : 600,//'100%',   
					height:350, 
					items:[invoiceDetailGrid]
				}) 		
	        }
			invoiceDetailStore.removeAll(true);
	        
			invoiceDetailStore.load({
				params:{
					contractID: contractID
				},			
				callback:function(record, options ,success){
					if(success == true){
					}else{
						Ext.Msg.alert(Ext_errMsg_For_Get_Store_Title ,Ext_errMsg_For_Get_Store_Message );
					}
				}
			});         
			
			invoiceDetailWin.setTitle("已开发票明细 --  " + contractAbb);
			invoiceDetailWin.show(); 
		
		});    			
		
	}
	
});    














