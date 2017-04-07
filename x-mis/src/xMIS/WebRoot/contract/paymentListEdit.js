Ext.onReady(function(){

    
    var chooseContractType;
    var strTips =invoiceEditFlag ==true ?'※点击\'合同名称\'等进入合同信息,点击\'已开发票金额/已报销金额\'进入票据收发列表':'※点击\'合同名称\'等进入合同信息,点击\'已收/付合同款\'进入收款信息列表';
    var searchCompanyInEditField = new Ext.ux.form.SearchField({
     	id: 'searchCompInEditField',
		store: pEStore,
		width: 155,
		
		onTrigger2Click : function(){//点击查询按钮或回车调用该方法   
            hidePaymentEditGrid();
            hideInvoiceEditGrid();
            var sForSer =this.getRawValue();
            if( paymentLECombo.value == undefined ||paymentLECombo.value ==''){
                Ext.Msg.alert("提示","请选择合同类型!")
                return false;
            }
            if(sForSer ==''){
                Ext.Msg.alert("提示","请输入合同简称!")
                return false;
            }
			pEStore.removeAll();
            

            if(paymentLECombo.getValue() == IncomeContractId){
                
                    chooseContractType = IncomeContractId;
              
                    paymentListEditGrid.getColumnModel().setColumnHeader(12,"<font color="+Color_Font_Contract_Total_Charged_Amt+">已收合同款</font>");
          
                    paymentListEditGrid.getColumnModel().setColumnHeader(13,"<font color="+Color_Font_Contract_Total_Invoiced_Amt+">已开发票金额</font>");
                    paymentListEditGrid.getColumnModel().setHidden(14,false);
                    paymentListEditGrid.getColumnModel().setColumnHeader(15,"<font color="+Color_Font_Contract_Total_UnCharged_Amt+">未收合同款</font>");
                    
                }else{

                    chooseContractType =PayoutContractId;
                    paymentListEditGrid.getColumnModel().setColumnHeader(12,"<font color="+Color_Font_Contract_Total_Charged_Amt+">已付合同款</font>");
                    paymentListEditGrid.getColumnModel().setColumnHeader(13,"<font color="+Color_Font_Contract_Total_Invoiced_Amt+">已报销金额</font>");
                    paymentListEditGrid.getColumnModel().setHidden(14,true);
                    paymentListEditGrid.getColumnModel().setColumnHeader(15,"<font color="+Color_Font_Contract_Total_UnCharged_Amt+">未付合同款</font>");
                    paymentListEditGrid.getColumnModel().setHidden(23,true);
                }
			pEStore.setBaseParam("searchName", sForSer)
            pEStore.setBaseParam("contractTypeID", paymentLECombo.value);
			pEStore.load({
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
		}   
    });
   

    var combodata=[['收入合同',IncomeContractId],['支出合同',PayoutContractId]];
    
    var paymentLECombo = new Ext.form.ComboBox({
        id:"paymentLECombo",

       // value: '收入合同',
        width: 120,
        store: new Ext.data.SimpleStore({
            fields: ['con','num'],
            data : combodata
        }),
        valueField : 'num',
        displayField : 'con',
        mode : 'local',
        editable:false,
        emptyText:'请选择合同类型',
        listeners:{
             "select":function(){
                
                 //alert(this.value);   //获取id为combo的值
             }
        },
        triggerAction : 'all'
    });


    var pERecord = Ext.data.Record.create([
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
         {name:invoiceTotalAmt},
         {name:invoiceUnAccountAmt}

         //{name:'pageNo'}
    
    ]);
    //var pEStore=new Ext.data.Store({
	var pEStore=new Ext.data.Store({
		proxy: new Ext.data.HttpProxy({
			url:'contractMain.do?method=getContractListByType' 
		}),
		remoteSort:true,
		sortInfo:{field: Abbreviation, direction: 'ASC'},
		reader:new Ext.data.JsonReader({
			root:'rows',
			totalProperty: 'totalCount'
		},pERecord)
	});

	
	Ext.grid.RowNumberer = Ext.extend(Ext.grid.RowNumberer, {
	    renderer:function(value, cellmeta, record, rowIndex, columnIndex, store){
	        return store.lastOptions.params.start + rowIndex + 1;   
	    }   
	}); 


 
	var paymentListEditSM= new Ext.grid.CheckboxSelectionModel({
        header: '',
        singleSelect:true,
        checkOnly : true,
        listeners:{
            rowselect : function( model, rowIndex,r ) {
               // alert('select'+rowIndex);
                /*if(projectSelFlag==0){
                    for(var j=0;paymentConditionGrid.getStore().getAt(j)!= undefined;j++){
                        if(r.get(ProjectStatusID) ==paymentConditionGrid.getStore().getAt(j).get(paymentConditionProjStatusID)){
                            //projSM.deselectRow(rowIndex);
                            break;

                        };
                    };
                }
                projectSelFlag=0;*/
            }
        }
    }); 

    


   /* showPaymentEditGrid  =function(id,contractName,contractType){

         paymentListEditForm.show();//显示
         paymentDetailEditGrid.setTitle("收款明细列表"+'-'+contractName);
         contractTypeInEdit =contractType;
         contractIdInEdit = id;
               paymentDetailEditStore.removeAll(true);
                paymentDetailEditStore.load({
                    params:{
                        contractID: id
                    }swv 
                });
    };*/
    this.reloadPaymentEditInJs =function(){
        pEStore.removeAll();
        pEStore.setBaseParam("searchName", searchCompanyInEditField.getValue());
        pEStore.setBaseParam("contractTypeID", paymentLECombo.value);
        pEStore.load({
            params:{
                start:0, 
                limit:20
            },
            callback:function(record, options ,success){
                if(success == true){
                  
                }else{
                    //Ext.Msg.alert(Ext_errMsg_For_Get_Store_Title,Ext_errMsg_For_Get_Store_Message);
                }
            }
      
        }); 
        
    }
 
    
     var paymentListEditGrid = new Ext.grid.GridPanel({
        id :"paymentListEditGrid",
        sm:paymentListEditSM,
        title :"合同款收付登记",
        renderTo: "paymentListEditDiv",
		//cm:pEColM,
		store:pEStore,
		autoExpandColumn: 'abb',
		stripeRows: true,
		layout:'fit',
      	enableColumnMove:false,      //True表示启动对Column顺序的拖拽,反之禁用    
       	enableColumnResize:true,      //设置为false可以禁止这个表格的列改变宽度 (默认值为 true).    
		stripeRows: true,	//True表示使用不同颜色间隔行，默认为false
		autoHeight: false,  
		autoWidth: false,
		//height:Ext.get("pcWin").getHeight(),
		height: 288,
        //width :800,
		width: Ext.get("paymentListEditDiv").getWidth(),
		loadMask:true,      //读取数据时的遮罩和提示功能，默认为false   
       	loadMask: {msg:'正在加载数据，请稍侯……'},
       //	viewConfig: {forceFit: true},
        listeners:{
            'beforerender':function(grid){
                if(invoiceEditFlag==true)
                    grid.setTitle("票据收发登记");
            },
            'rowclick' : function(grid,  rowIndex,  e ){
            },
            'rowdblclick':function(gr,  rowIndex,  e){
/*
               // showContractWin();
                paymentListEditForm.show();//显示
                
                paymentDetailEditStore.removeAll(true);
	            paymentDetailEditStore.load({
	                params:{
	                    contractID: paymentListEditGrid.getStore().getAt(rowIndex).get(ContractID)
	                },          
	                callback:function(record, options ,success){
	                    if(success == true){
	                    }else{
	                        Ext.Msg.alert(Ext_errMsg_For_Get_Store_Title ,Ext_errMsg_For_Get_Store_Message );
	                    }
	                }
	            });

                //contractInfoWin.show();
 */
            },
            'click' : function(  e ){
            } 
        },
        tbar: [
        	"合同类型:",paymentLECombo,'-','合同简称 / 合同编号: ', 
            searchCompanyInEditField,' ','->' ,strTips
        	//searchCompFi eld
            ],
        columns: [
        //paymentListEditSM,
        new Ext.grid.RowNumberer(),
        {header: "合同编号",dataIndex:ContractNo, sortable:true, 
         renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
                 if(value==''||value == null){
                    return  '';
                 }else{  
                    return gridColOnClick( convertNameWithoutLinker(value)+'^onClick=showContractWin('+record.data[ContractID]+')');
                 }   
            }
        },
        {header: "合同名称",dataIndex:ContractName, sortable:true, renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
                 if(value==''||value == null){
                    return  '';
                 }else{                     
                    return gridColOnClick( convertNameWithoutLinker(value)+'^onClick=showContractWin('+record.data[ContractID]+')');
                 }
            }},
        {header: "简称", id: 'abb',  dataIndex:Abbreviation, sortable:true, renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
            cellMeta.attr ="提示：";
                 if(value==''||value == null){
                    return  '';
                 }else{ 
                    return gridColOnClick( convertNameWithoutLinker(value)+'^onClick=showContractWin('+record.data[ContractID]+')');
                 }
            }},
        {header: "父合同ID",dataIndex:ParentID, sortable:true, hidden:true},
        {header: "父合同简称",dataIndex:ParentContractAbb, sortable:true, renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
            if(value==''||value == null){
                return  '';
            }else{      
                return gridColOnClick( convertNameWithoutLinker(value)+'^onClick=showContractWin('+record.data[ParentID]+')');
            }
        }},
        {header: "签订日期",dataIndex:SignatureDate, sortable:true, renderer: customer_formatDate, align: 'center'},
        {header: "甲方公司ID",dataIndex: BuyerID, hidden:true},
        {header: "甲方公司",dataIndex: BuyerAbb, sortable:true, renderer: biuldLinker, hidden:true},
        
        {header: "乙方公司ID",dataIndex: VendorID, hidden:true},
        {header: "乙方公司",dataIndex: VendorAbb, sortable:true, renderer: biuldLinker,hidden:true},
        
        {header: "<font color="+Color_Font_Contract_Total_Amt+">合同金额</font>",dataIndex:ContractAmt, sortable:true, 
            renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
                 var strName = convertNameWithoutLinker(record.data[Abbreviation]);
                var strType = paymentLECombo.value;
                 if(value==''||value == null){
                    return  '';
                 }else{
                    return formatCurrency(record.data[CurrenyGraph], convertNameWithoutLinker(value)+'^onClick=showContractWin('+record.data[ContractID]+',\''+strName+'\',\''+strType +'\')', 'green');
                 }
            }, 
            align: 'right'},
        
        {header: "<font color="+Color_Font_Contract_Total_Charged_Amt+">已收合同款</font>",dataIndex:TotalAmtByID, sortable:true, 
        
            renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
                var strName = convertNameWithoutLinker(record.data[Abbreviation]);
                var strType = chooseContractType;
                var srtCurrencyG = convertNameWithoutLinker(record.data[CurrenyGraph]);
                var strContractAmt = convertNameWithoutLinker(record.data[ContractAmt]);
                 if(value==''||value == null){
                    return  '';
                 }else{
                    if(invoiceEditFlag==true){
                        return formatCurrency(record.data[CurrenyGraph], value, 'blue');  
                    }else{
                     //return formatCurrency(record.data[CurrenyGraph], value, 'blue');
                        return formatCurrency(record.data[CurrenyGraph], 
                         value+'^onClick=showPaymentEditGrid('+record.data[ContractID]+',\''+strName+'\',\''+strType + '\',\''+srtCurrencyG+'\',\''+ strContractAmt+ '\')', 'blue');
                    }
                 }
            }, 
            align: 'right'},
           {header: "<font color="+Color_Font_Contract_Total_Invoiced_Amt+">已开发票金额</font>",dataIndex:invoiceTotalAmt, sortable:true, 
            renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
                var strName = convertNameWithoutLinker(record.data[Abbreviation]);
                var strType = chooseContractType;
                var srtCurrencyG = convertNameWithoutLinker(record.data[CurrenyGraph]);
                var strContractAmt = convertNameWithoutLinker(record.data[ContractAmt]);
             
                // return formatCurrency(record.data[CurrenyGraph], value, '#FF00FF');  
                 
                    if(invoiceEditFlag==true){
                         return formatCurrency(record.data[CurrenyGraph], 
                         value+'^onClick=showInvoiceEditGrid('+record.data[ContractID]+',\''+strName+'\',\''+strType + '\',\''+srtCurrencyG+'\',\''+ strContractAmt+ '\')', '#FF00FF');  
                    }else{
                      return formatCurrency(record.data[CurrenyGraph], value, '#FF00FF'); 
                        //return formatCurrency(record.data[CurrenyGraph], 
                        // value+'^onClick=showPaymentEditGrid('+record.data[ContractID]+',\''+strName+'\',\''+strType + '\',\''+srtCurrencyG+'\',\''+ strContractAmt+ '\')', 'blue');
                    }
                   // return formatCurrency(record.data[CurrenyGraph], 
                      //          value+'^onClick=showInvoiceList(\''+ convertNameWithoutLinker(record.data[Abbreviation])+ '\','+record.data[ContractID]+')' +
                       //                 '   id=\'invoiceListLink_'+record.data[ContractID] + '\'', Color_Font_Contract_Total_Invoiced_Amt);
                    //return formatCurrency(record.data[CurrenyGraph], value, '#FF00FF');
            }, 
            align: 'right'},
            
        {header: "<font color="+Color_Font_Contract_Total_UnAccount_Amt+">已开票未到账金额</font>",dataIndex:invoiceUnAccountAmt, sortable:true, 
            renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
               //return 0;   
                    return formatCurrency(record.data[CurrenyGraph], value, Color_Font_Contract_Total_UnAccount_Amt);
 //                 return  record.data[invoiceTotalAmt] == "0" ? formatCurrency(record.data[CurrenyGraph], record.data[invoiceTotalAmt], Color_Font_Contract_Total_UnAccount_Amt);
////                  formatCurrency(record.data[CurrenyGraph], value, Color_Font_Contract_Total_UnAccount_Amt);
           }, 
            align: 'right'},

        {header: "未收合同款",dataIndex:UnChargedAmtTotal, sortable:true, 
            renderer: function(value, cellMeta, record, rowIndex, columnIndex, store){
                    return formatCurrency(record.data[CurrenyGraph], value, 'red');
            }, 
            align: 'right'},
            
        {header: "甲方代表", dataIndex: BuyerRepName, sortable:true, renderer: biuldLinker, hidden:true},
        {header: "乙方代表", dataIndex: VendorRepName, sortable:true, renderer: biuldLinker, hidden:true},
        
        {header: "合同概要", dataIndex: ContractSummary, hidden:true},
        {header: "备注", dataIndex: Remark, hidden:true},
        
        {header: "登记人", dataIndex: RepName, sortable:true, renderer: biuldLinker, hidden:true},
        {header: "更新日期", dataIndex: UpdateDate, sortable:true, renderer: customer_formatDate, align: 'center', hidden:true},
        
        {header: "项目名称",dataIndex:ProjectName, sortable:true, renderer: biuldLinker},
        {header: "横向来源名称",dataIndex:GovermentContractSourceName, sortable:true, renderer: biuldLinker}
        ],

		// paging bar on the bottom
       	bbar: new Ext.PagingToolbar({
			pageSize: 20,
			store: pEStore,
			displayInfo: true,
			displayMsg: '当前显示 {0} - {1} 条记录 /共 {2} 条记录',
			emptyMsg: "无显示数据"
        })
    }); 
	  

 });    