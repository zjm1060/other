Ext.onReady(function(){
    Ext.QuickTips.init();    
    /******************
     * *invoiceEditGrid
     * ******************/
    var contractIdInEdit ;
    //var EmployeeIDIninvoiceCharged =0;//need del..
    var contractTypeInEdit;
    var contractNameInEdit;
    var hidenFlag ;
    var selectRowNum ='';
    var setEditorHide = function(obj){
        obj.reject(true);
        hidenFlag = false;
    };

    Ext.grid.RowNumberer = Ext.extend(Ext.grid.RowNumberer, {
        renderer:function(value, cellmeta, record, rowIndex, columnIndex, store){
            return rowIndex + 1;   
        }   
    }); 

    var invoiceEditRecord = Ext.data.Record.create([
         {name :invoiceContractID},//需要改
         {name: InvoiceNo},
         {name: InvoiceCode},
         {name: InvoiceAmt,type:'float'},
         {name: InvoiceDate},
         {name: InvoiceDateDisplayed,type:'date'},
         {name: UpdateDateDisplayed,type:'date'},
         {name: UpdateID},
         {name: InvoiceUpdateDate},
         {name: TransactorID},
         {name: ScanAttachment},
         {name: invoiceRemark},
         {name: TotalAmtByID},
         {name: invoiceAEmpName},
         {name: invoiceBEmpName},
         {name: UnAccountAmtTotalSum}

    ]);
    var invoiceEditStore = new Ext.data.GroupingStore({
        url:'contractMain.do?method=getInvoiceList',
        remoteSort:false,
        sortInfo:{field: InvoiceDateDisplayed, direction: 'ASC'},
        reader:new Ext.data.JsonReader({
            root:'rows',
            totalProperty: 'totalCount',
            //remoteSort:false,
            fields:invoiceEditRecord//invoice  paymentDetailEditRecord
        })
        
    });
   /* paymentDetailEditStore.on('load',function(store,records,options){
        //alert(records.length+'==in load');
        for(var i=0;i<records.length;i++){
            var record = records[i];
            alert(record.get(paymentTransactorID));
        
      }
     
    },this,{delay:0})*/

    var invoiceEditAEmpIDField =new Ext.form.TextField({//
    });
    var invoiceEditIdField = new Ext.form.TextField({
    });
    var invoiceCodeEditField = new Ext.form.TextField({
    });
    var invoiceAmtEditField = new Ext.form.NumberField({//
        id:"invoiceAmtEditField",
        minValue: 0,
        allowBlank: false,
        listeners:{
            'blur':function(){
                if(this.getValue()=='0'){
                    Ext.Msg.alert("提示","金额不能为0！");
                    invoiceAmtEditField.setValue('');
                    return false;
                }
                if(this.getValue()>0){
                    var paymentTotal =this.getValue();
                    if(selectRowNum == ''){
                        for(var i =0;i<invoiceEditStore.getCount();i++){
                           var amtInGrid  = invoiceEditStore.getAt(i).get(InvoiceAmt);
                           if(amtInGrid !=''&& amtInGrid!=undefined){
                               paymentTotal = paymentTotal + amtInGrid*1;
                           }
                        }
                    }else{
                        for(var i =0;i<invoiceEditStore.getCount();i++){
                           var amtInGrid  = invoiceEditStore.getAt(i).get(InvoiceAmt);
                           if(amtInGrid !=''&& amtInGrid!=undefined && i !=selectRowNum){
                               paymentTotal = paymentTotal + amtInGrid*1;
                           }
                        }
                        
                    }
                    if(paymentTotal>choosedAmt){
                        Ext.MessageBox.show({
                            title:"提示",
                            msg:"发票金额大于合同总额，是否继续？",
                            buttons:Ext.Msg.YESNO ,
                                fn:function(btn){
                                    if(btn=='no'){
                                       invoiceAmtEditField.setValue('');
                                    };
                                },
                            width:280,
                            icon:Ext.MessageBox.INFO,
                            closable:false
                    });
                    }
                }
            }
        }
    });

    var invoiceDateEditField = new Ext.form.DateField({
        id:"invoiceDateEditField",
        format : 'Y-m-d',
        allowBlank: false  
    });
    var invoiceUpDateDEditField = new Ext.form.DateField({
        id:"invoiceUpDateDEditField",
        format : 'Y-m-d',
        readOnly: true
    });
    var invoiceEditRemarkField = new Ext.form.TextField({

    });
   
 /**************************
  * invoiceAEmpPeasonWin
  * 
  */ 
    var currencyx = ''; 
      
    var invoiceAEmpPeasonSM= new Ext.grid.CheckboxSelectionModel({
         header: '',
         singleSelect:true
    }); 

    var invoiceAEmpPeasonColM=new Ext.grid.ColumnModel(
        [
        invoiceAEmpPeasonSM,
        {header: "部门名称",dataIndex: "deptName", sortable:true , hidden:true},
        //{header: "联系人ID",dataIndex: "<%=DBSchema.EmployeeBelongToDept.EmployeeID %>", sortable:true, hidden:true},
        {header: "联系人名称",dataIndex: "user_name", sortable:true}
        ]
    );  

    var invoiceAEmpPeasonStore=new Ext.data.GroupingStore({//
        proxy: new Ext.data.HttpProxy({
            url:'crmMaint.do?method=getContactPersonByCompany' 
        }),
        remoteSort:true,
        reader:new Ext.data.JsonReader({
            root:'rows',
            totalProperty: 'totalCount',
            remoteSort:true,
            fields:[
                 {name:'deptName'},
                 {name:EmployeeIDInInvo},//EmployeeIDInPaymentCharged 改
                 {name:'user_name'}
           ]
        }),
        groupField:'deptName'
    });
    
     invoiceAEmpPeasonStore.on('load',function(store,records,options){
          var selectValue = searchInvoiceAEmpField.getValue();//searchpaymentAEmpField
    if(selectValue != ''){
        invoiceAEmpPeasonGrid.getView().expandAllGroups();
     
        var arr=[];
        for(var i=0;i<records.length;i++){
            var record = records[i];
            record.get('user_name') 
            if(record.get('user_name')==selectValue){
                arr.push(record);
            }
        }
        invoiceAEmpPeasonSM.selectRecords(arr);
    }else{
        invoiceAEmpPeasonGrid.getView().collapseAllGroups();
    }
  },this,{delay:100})
    this.showInvoiceEditGridInJS= function( id,contractName,contractType,curr){

        if(curr=="欧元"){
            currencyx ='€';
        }else{
            if(curr==""){
               currencyx ='￥'; 
            }else{
                currencyx = curr;
            }
        };        
        invoiceEditPanel.show();//显示整个信息panel
        invoiceEditPanel.setTitle("票据收发列表"+'-'+contractName);
        
        downloadStoreInInvoEd.load({
            params:{
                applicationType: attchmentType,
                refID: id 
            }
        });

        contractTypeInEdit =contractType;
        contractIdInEdit = id;
        contractNameInEdit= contractName;
        invoiceEditStore.removeAll(true);
        invoiceEditStore.load({
            params:{
                contractID: id
            }
        });
       if( invoiceEditPanel.isVisible()){
           invoiceEditPanel.focus('',5);
       }
        
    };


    var invoiceAEmpPeasonGrid = new Ext.grid.GridPanel({
        id:"invoiceAEmpPeasonGrid",
        sm: invoiceAEmpPeasonSM,
        cm: invoiceAEmpPeasonColM,
        store: invoiceAEmpPeasonStore,
        view: new Ext.grid.GroupingView({
            forceFit:true,
            hideGroupedColumn: true,    //用来分组的数据点这一列是否要显示 
            
            showGroupName: false,   //用来分组的数据点这一列的header是否要随group name一起显示 
            
            startCollapsed: true //一开始进到grid这页，它的group是合起还是展开 
        }), //此为打开窗口时，找到已选的项
        stripeRows: true,
        enableColumnMove:false,      //True表示启动对Column顺序的拖拽,反之禁用    
        enableColumnResize:false,      //设置为false可以禁止这个表格的列改变宽度 (默认值为 true).    
        stripeRows: true,   //True表示使用不同颜色间隔行，默认为false
        autoHeight: false,   
        loadMask:true,      //读取数据时的遮罩和提示功能，默认为false   
        // config options for stateful behavior
        stateful: true,
        stateId: 'invoiceAEmpPeasonGrid',  
        forceFit:true, //Ture表示自动扩展或缩小列的宽度以适应grid的宽度,从而避免出现水平滚动条.   
        loadMask: {msg:'正在加载数据，请稍侯……'},
        height:320,
        
        tbar: [{
            ref: '../okBtn',
            iconCls: 'ok',
            text: '确定',
            handler: function(){
                var record = invoiceAEmpPeasonGrid.getSelectionModel().getSelected();
                if(record == 'undefined' ||record ==undefined){Ext.Msg.alert("未选择","请先选择！");return true;}
                              
                searchInvoiceAEmpField.setValue(record.get('user_name'));
                invoiceEditAEmpIDField.setValue(record.get(EmployeeIDInInvo));
                invoiceAEmpPeasonWin.hide();
           
            }
        },'-',{
            ref: '../cancelBtn',
            iconCls: 'remove',
            text: '取消',
            disabled: false,
            handler: function(){
                if(invoiceAEmpPeasonWin) invoiceAEmpPeasonWin.hide();
                
                //there is an err of dom.style when window.close() on (window + grid), so shuold use window.hide() to close
            }
        }

        ]            

    });

    var invoiceAEmpPeasonWin = new Ext.Window({ 
        id:"invoiceAEmpPeasonWin",
        modal: true,
        renderTo:Ext.getBody(), 
        frame:true, 
        plain:true, 
        resizable:false, 
        closeAction:"hide", 
        closable:true, 
        bodyStyle:"padding:10px", 
        width:300, 
        height:400, 
        items:[invoiceAEmpPeasonGrid]   
    });

    var searchInvoiceAEmpField = new Ext.ux.form.SearchField({
        id:"searchInvoiceAEmpField",
        allowBlank: false,
        emptyText :"必选项...",
        hasSearch : false,
        validateOnBlur:true,
        editable :false,
        //width: 125,
        
        onTrigger2Click : function(){//点击查询按钮或回车调用该方法 

            invoiceAEmpPeasonWin.show();
            invoiceAEmpPeasonWin.setTitle("选择经办人");
            invoiceAEmpPeasonStore.removeAll();
            invoiceAEmpPeasonStore.proxy.setUrl('crmMaint.do?method=getContactPersonByCompany',true);
            invoiceAEmpPeasonStore.load({
                params:{
                    compID: companyIdInInvo
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
 /******************flag*******************/ 
    var newPaymentEditRowFlag= 0;
    
    var invoiceEditGridEditor = new Ext.ux.grid.RowEditor({
        id:"invoiceEditGridEditor",
        saveText: '确认',
        cancelText: '取消',
        clicksToEdit: 2 ,
        listeners:{

        }
    });
    invoiceEditGridEditor.on({ 
       scope: this, 
       beforeedit: function(){
           invoiceDetailEditGrid.removeBtn.setDisabled(true);
           invoiceDetailEditGrid.newBtn.setDisabled(true);
       },
       beforehide: function(){

            var flag = hidenFlag;
            hidenFlag = true ;
            return flag;
       },
       afteredit: function(roweditor, changes, record, rowIndex){

                 invoiceListEditForm.getForm().submit({
                     url : 'contractMain.do?method=saveInvoiceDetail',
                     method : 'post',
                     params : {

                        contractID:contractIdInEdit,
                        invoiceID:invoiceEditIdField.getValue(),
                        invoiceCode:invoiceCodeEditField.getValue(),
                        contractType:contractTypeInEdit,
                        invoiceDetailDate:invoiceDateEditField.getValue(),
                        invoiceDetailAmt:invoiceAmtEditField.getValue(),
                        invoiceRemark: invoiceEditRemarkField.getValue(),
                        transavtorID: invoiceEditAEmpIDField.getValue()
                                               
                     },
                     success : function(form, action) {

                       Ext.Msg.alert(action.result.messageTitle,action.result.message);
                        if(Ext.getCmp("paymentListEditGrid")){//这个地方还需要刷新吗？
                            reloadPaymentListEdit();//刷新合同列表中的付款项
                        }
                        invoiceDetailEditGrid.getStore().reload({
                            callback:function(record, options ,success){
                                if(success == true){
                                }else{
                                    Ext.Msg.alert(Ext_errMsg_For_Get_Store_Title,Ext_errMsg_For_Get_Store_Message );
                                }
                            }
                        });
                        //paymentConditionGrid.getView().refresh();
                     },
                    failure : function(form, action) {

                        Ext.Msg.alert(action.result.messageTitle,action.result.message);
                        var s = invoiceDetailEditGrid.getSelectionModel().getSelections();
                        for(var i = 0, r; r = s[i]; i++){
                            invoiceEditStore.remove(r);
                        };
                        //paymentConditionGrid.getView().refresh();
                      
                    }
                  }); 
                  newPaymentEditRowFlag = 0;
                  invoiceDetailEditGrid.getStore().reload({
                      callback:function(record, options ,success){
                          if(success == true){
                          }else{
                              Ext.Msg.alert(Ext_errMsg_For_Get_Store_Title,Ext_errMsg_For_Get_Store_Message );
                          }
                      }
                  });
             
           invoiceDetailEditGrid.removeBtn.setDisabled(false);
           invoiceDetailEditGrid.newBtn.setDisabled(false);
           newPaymentEditRowFlag= 0;
           selectRowNum ='';
       },
       canceledit: function(){
            if(newPaymentEditRowFlag == 1) {
                invoiceEditGridEditor.stopEditing(false);
                var s = invoiceDetailEditGrid.getSelectionModel().getSelections();
                for(var i = 0, r; r = s[i]; i++){
                    invoiceEditStore.remove(r);
                };
                hidenFlag = true;
                invoiceDetailEditGrid.getView().refresh();
                newPaymentEditRowFlag = 0;
           }
        
           invoiceDetailEditGrid.getView().refresh();
           invoiceDetailEditGrid.removeBtn.setDisabled(false);
           invoiceDetailEditGrid.newBtn.setDisabled(false);
           //newPaymentEditRowFlag= 0;
           selectRowNum ='';
       }
    })
   
   
    /****************
     * 收款明细列表
     *************************/
    var invoiceDetailEditGrid = new Ext.grid.GridPanel({
        id :"invoiceDetailEditGrid",
        store: invoiceEditStore,
        layout:'fit',
        enableColumnMove:false,      //True表示启动对Column顺序的拖拽,反之禁用    
        enableColumnResize:true,      //设置为false可以禁止这个表格的列改变宽度 (默认值为 true).    
        stripeRows: true,   //True表示使用不同颜色间隔行，默认为false
        autoHeight: false,  
        autoWidth: false,
        width:Ext.get("invoiceDetailEditGridDiv").getWidth()*17/24,
        height:185,
        region:'center',
        margins: '0 5 5 5',
        listeners:{            
            'rowdblclick':function(gr,  rowIndex,  e){
                 selectRowNum = rowIndex;
            }
        },
        loadMask:true,      //读取数据时的遮罩和提示功能，默认为false   
        stateful: true,
        stateId: 'grid', 
        plugins: [invoiceEditGridEditor],
        //view: new Ext.grid.GroupingView({
         //   markDirty: false
       // }),

        //forceFit:true, //Ture表示自动扩展或缩小列的宽度以适应grid的宽度,从而避免出现水平滚动条.   
        loadMask: {msg:'正在加载数据，请稍侯……'},
        tbar: [{
            ref: '../newBtn',
            iconCls: 'add',
            text: '新建',
            handler: function(){  


                var e = new invoiceEditRecord({
                    invoiceContractID:'',
                    InvoiceNo:'',
                    InvoiceAmt:'',
                    InvoiceDate:'',
                    
                    InvoiceDateDisplayed: (new Date()).format('Y-m-d'),
                    TotalAmtByID:'',// add 1
                    invoiceRemark: '',
                    ScanAttachment: '0',
                    TransactorID: '',
                    InvoiceUpdateDate: '',
                    UpdateID:'',
                    UpdateDateDisplayed :'',//add 2
                    invoiceAEmpName:'',
                    invoiceBEmpName:'',
                    UnAccountAmtTotalSum:''
                });
      
                invoiceEditGridEditor.stopEditing();

                invoiceEditStore.insert(0, e);

                invoiceDetailEditGrid.getView().refresh();

                invoiceDetailEditGrid.getSelectionModel().selectRow(0);

                invoiceEditGridEditor.startEditing(0);
             
                newPaymentEditRowFlag =1;
          

            }
        }, '-' , {
            ref: '../removeBtn',
            iconCls: 'remove',
            text: '删除',
            disabled: false,
            handler: function(){
                
                var record = invoiceDetailEditGrid.getSelectionModel().getSelected();
                if(record == 'undefined' ||record ==undefined){Ext.Msg.alert("未选择","请先选择！");return true;}
               // alert('2='+contractIdInEdit+';1='+record.get(InvoiceNo));
                invoiceListEditForm.getForm().submit({
                      url : 'contractMain.do?method=delInvoiceDetail',
                     method : 'post',
                     params : {
                        contractID: contractIdInEdit,
                        invoiceID: record.get(InvoiceNo)
                     },
                     success : function(form, action) {
                        Ext.Msg.alert(action.result.messageTitle,action.result.message);
                        if(Ext.getCmp("paymentListEditGrid")){
                            reloadPaymentListEdit();//刷新合同列表中的付款项
                        }
                       /* stEditor.stopEditing();
                        
                        var s = stGrid.getSelectionModel().getSelections();
                        for(var i = 0, r; r = s[i]; i++){
                            stStore.remove(r);
                        };  */
                        invoiceDetailEditGrid.getStore().reload({
                            callback:function(record, options ,success){
                                if(success == true){
                                }else{
                                    Ext.Msg.alert(Ext_errMsg_For_Get_Store_Title,Ext_errMsg_For_Get_Store_Message);
                                }
                            }
                        });
                        invoiceDetailEditGrid.getView().refresh();
                        newPaymentEditRowFlag = 0;
                    
                     },
                    failure : function(form, action) {
                        Ext.Msg.alert(action.result.messageTitle,action.result.message);
                    }
                }); 
             
                
            
            }
        },'-',{
           // ref: '../refreshBtn',
            //icon :'"/xMIS/resources/images/png-0097.png',
            //cls: "x-btn-text-icon" ,
            iconCls:"updateAttc",
            text: '附件添加',
            disabled: false,
             handler: function(){ 
                var form ;
                if(!form){
                    form= new Ext.form.FormPanel({
                     id: 'invoiceUpload',	
                     baseCls : 'x-plain',
                     labelWidth : 70,
                     fileUpload : true,
                     defaultType : 'textfield',
                     items : [{
                     	id: 'invoiceUpl',
                        xtype : 'textfield',
                        autoCreate:{tag: "input", type: "file", contenteditable:false},
                        fieldLabel : '上传文件名',
                        id : 'userfile',
                        inputType : 'file',
                        contenteditable: false,
                        anchor : '100%' // anchor width by percentage,
                       }]
                    })
                   }; 
                
                var winUpl;
                var strUrl = '/xMIS/fileUpload.do?type='+attchmentType;
                if(!winUpl){
                  winUpl = new Ext.Window({
                     modal: true,
                     title : '附件上传',
                     width : 400,
                     height : 100,
                     minWidth : 300,
                     minHeight : 100,
                     layout : 'fit',
                     plain : true,
                     bodyStyle : 'padding:5px;',
                     buttonAlign : 'center',
                     items : form,
                     buttons : [{
                        text : '上传',
                        handler : function() {
                        
                            if (form.form.isValid()) {
                                if(Ext.getCmp('userfile').getValue() == ''){
                                    //Ext.MessageBox.alert('错误','请选择你要上传的文件');
                                    alert('请选择你要上传的文件');
                                    return;
                                }
                                Ext.MessageBox.wait('正在上传附件, 请稍侯 ...', '提示');   
                                
                                form.getForm().submit({
                                     url : strUrl,
                                     method : 'post',
                                     params : {
                                            conID: contractIdInEdit,
                                            conType: contractTypeInEdit,
                                            conName: contractNameInEdit
                                                   
                                     },
                                     success : function(form, action) {
                                        Ext.Msg.alert(action.result.messageTitle,action.result.message);

                                           downloadStoreInInvoEd.load({
                                                params:{
                                                    applicationType: attchmentType,
                                                    refID: contractIdInEdit 
                                                }
                                            });
                                        
                                        winUpl.close();
                                        
                                     },
                                    failure : function(form, action) {
                                        Ext.Msg.alert(action.result.messageTitle,action.result.message);
                                    }
                                })
                            }
                        }
                     }, {
                        text : '关闭',
                        handler : function() {
                            winUpl.close();
                        }
                   }]
            })
        };
        winUpl.setPosition(400, 200);
        winUpl.show();
        }
                
   
        
        },'->' ,"*双击进入编辑状态"
        /*,'-',{
            ref: '../refreshBtn',
            iconCls: 'x-tbar-loading',
            text: '刷新',
            disabled: false,
             handler: function(){ 
             paymentConditionGrid.hide();
             } 
        
        }*/
        ],
        viewConfig: {forceFit: true},
        colModel: new Ext.grid.ColumnModel({
            columns: 
            [

             new Ext.grid.RowNumberer(),
             {
                header: "发票Id", 
                dataIndex:InvoiceNo,  
                sortable:true, 
                hidden: true,
                //renderer:customer_formatDate, 
                align: 'center',
                editor:invoiceEditIdField
            },{
                      //   {name: InvoiceDateDisplayed,type:'date'},
         //{name: UpdateDateDisplayed,type:'date'},
                header: "开票日期", dataIndex:InvoiceDateDisplayed, sortable:true,   
                //renderer:customer_formatDate, 
                align: 'center',
                xtype: 'datecolumn',
                format: 'Y-m-d',
                editor:invoiceDateEditField
            },{
                header: "发票号", dataIndex:InvoiceCode, sortable:true,   
               // renderer:customer_formatDate, 
                align: 'center',
               // format:'0,000',
               // xtype: 'numbercolumn',
           
                editor:invoiceCodeEditField
            },
        
            {
                xtype: 'numbercolumn',
                format:'0,000.00',
                header: "开票金额", dataIndex:InvoiceAmt, sortable:true, 
                //renderer:formatCurInPayEdit, //function(value, cellMeta, record, rowIndex, columnIndex, store){
                        //return formatCurInPayEdit( value, 'blue');
               // }, 
                align: 'right',
                editor: invoiceAmtEditField
                
            },{
                header: "经办人Id",   
                hidden: true,
                dataIndex:TransactorID,
                //renderer:customer_formatDate, 
                align: 'center',
                editor:invoiceEditAEmpIDField
            },{
                header: "经办人",
                dataIndex:invoiceAEmpName,
                sortable:true, 
                renderer: convertUserName,
                editor:searchInvoiceAEmpField
            },
            {
                header: "备注", dataIndex: invoiceRemark, sortable: true,
                editor :invoiceEditRemarkField
            },
            {
                header: "登记日期", dataIndex:UpdateDateDisplayed, sortable:true,
                //renderer:customer_formatDate,
                xtype: 'datecolumn',
                format: 'Y-m-d',
                align: 'center',
                editor :invoiceUpDateDEditField//为空会编辑时出格式问题
            },
            {
                header: "登记人",dataIndex:invoiceBEmpName, sortable:true, 
                editor :null
            }]
        })

    });
    
     invoiceListEditForm = new Ext.form.FormPanel({
        id: 'invoiceListEditFrm',
        baseCls : 'x-plain',
        anchor : '100%',
        items:[invoiceDetailEditGrid]
    });

    this.hideInvoiceEditGridInJS=function(){
        invoiceEditPanel.setVisible(false);
    };
    
    

   /*====================================================================
     * Show download files
     *====================================================================*/
     
    var downloadStoreInInvoEd=new Ext.data.Store({
        proxy: new Ext.data.HttpProxy({
            url:'contractMain.do?method=getAttachedListStroeByApplicationType'//修改成新滴
        }),
        remoteSort:true,
        sortInfo:{field: attachmentInvoUploadDate, direction: 'ASC'},
        reader:new Ext.data.JsonReader({
            root:'rows',
            totalProperty: 'totalCount',
            //remoteSort:true,
            fields:[
                {name:AttachmentIDInInvoEd},
                {name:AttachmentNameInInvoEd},
                {name:attachmentInvoUploaderName},
                {name:attachmentInvoUploadDate}
           ]
        })
    });   


  /*  downloadStoreInPayEd.load({
        params:{
            applicationType: attachType,
            refID: contractIdInEdit 
        }
    });*/
    downloadStoreInInvoEd.on('load',function(store,records,options){

        if(records.length==0){
            downloadGridInInvoEd.show();
        }else{
            downloadGridInInvoEd.show();


        }
    },this,{delay:0})
    var attchDateColumn = new Ext.grid.DateColumn({
            header:'上传日期',
            //xtype: 'datecolumn',
            format : 'Y-m-d',
            //renderer:customer_formatDate, 
            //width:150,//Ext.get(ContractAttd).getWidth(), 
            dataIndex: attachmentUploadDate.trim().substring(0,10)
            //renderer: nameLinker
        
    });
    

    var downloadCMInPayEd=new Ext.grid.ColumnModel({
        columns:
        [new Ext.grid.RowNumberer(),
         {
            header:'附件名称',
            dataIndex: AttachmentNameInInvoEd,
            renderer: nameLinker
        },{
            header:'上传人',
            dataIndex: attachmentInvoUploaderName
        },{
            header: "上传日期", 
            dataIndex:attachmentInvoUploadDate, 
            sortable:true,   
            xtype: 'datecolumn',
            format: 'Y-m-d',
            align: 'center'
        }]
    });
    var downloadGridInInvoEd = new Ext.grid.GridPanel({
        id :'downloadGridInInvo',
        title:'附件信息',
        enableColumnMove:false,      //True表示启动对Column顺序的拖拽,反之禁用    
        enableColumnResize:true,      //设置为false可以禁止这个表格的列改变宽度 (默认值为 true).    
        cm: downloadCMInPayEd,
        layout : 'fit',
        store: downloadStoreInInvoEd,
        stripeRows: true,   //True表示使用不同颜色间隔行，默认为false
        autoExpandColumn: 0,
        viewConfig:{forceFit:true},
        stateful: true,
        height:185,
        autoHeight: false,
        autoWidth:false,
        width:Ext.get("invoiceDetailEditGridDiv").getWidth()*7/24,
        stateId: 'downloadGrid',
        loadMask:true     //读取数据时的遮罩和提示功能，默认为false            
    });   

   var invoiceEditPanel = new Ext.Panel({
        id:"invoiceEditPanel",
        width :Ext.get("invoiceDetailEditGridDiv").getWidth(),//
        height: 212,
        hidden:true,
        //border:false,
        renderTo:"invoiceDetailEditGridDiv",
        title:"票据收发列表",
        layout:{
            type:'hbox',
            border:false,
            align:'stretchmax'
        },
        items:[{
            items:[invoiceListEditForm],
            flex:17           
        },{
            items:[downloadGridInInvoEd],
            flex:7          
        }]
        
    }); 
 });    