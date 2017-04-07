Ext.onReady(function(){
	Ext.QuickTips.init();	
    /******************
     * *paymentDetailEditGrid
     * ******************/
    var contractIdInEdit ;
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

    var paymentDetailEditRecord = Ext.data.Record.create([
         {name :paymentChargedId},
         {name: paymentChargedDateDisplayed,type:'date'},
         {name: paymentChargedDate},
         {name: paymentAEmpName},
         {name: paymentTransactorID},
         {name: paymentChargedAmt,type:'float'},
         {name: paymentBEmpName},
         {name: paymentUpdateDate},
         {name: paymentUpdateDateDisplayed,type:'date'}, 
         {name: paymentCurrencyGraph},
         {name: paymentCurrencyExchangeRate},
    
         {name: paymentChargedRemark}

    ]);
    var paymentDetailEditStore = new Ext.data.GroupingStore({
        url:'contractMain.do?method=getPaymentList',
        remoteSort:false,
        sortInfo:{field: paymentChargedDateDisplayed, direction: 'ASC'},
        reader:new Ext.data.JsonReader({
            root:'rows',
            totalProperty: 'totalCount',
            //remoteSort:false,
            fields:paymentDetailEditRecord
        })
        
    });
   /* paymentDetailEditStore.on('load',function(store,records,options){
        //alert(records.length+'==in load');
        for(var i=0;i<records.length;i++){
            var record = records[i];
            alert(record.get(paymentTransactorID));
        
      }
     
    },this,{delay:0})*/

    var paymentEditAEmpIDField =new Ext.form.TextField({
    });
    var paymentEditIdField = new Ext.form.TextField({
    });
    var paymentAmtEditField = new Ext.form.NumberField({
        id:"paymentAmtEditField",
        minValue: 0,
        allowBlank: false,
        listeners:{
            'blur':function(){
                if(this.getValue()=='0'){
                    Ext.Msg.alert("提示","金额不能为0！")
                    paymentAmtEditField.setValue('');
                    return false;
                }
                if(this.getValue()>0){
                    var paymentTotal =this.getValue();
                    if(selectRowNum == ''){
	                    for(var i =0;i<paymentDetailEditStore.getCount();i++){
	                       var amtInGrid  = paymentDetailEditStore.getAt(i).get(paymentChargedAmt);
	                       if(amtInGrid !=''&& amtInGrid!=undefined){
	                           paymentTotal = paymentTotal + amtInGrid*1;
	                       }
	                    }
                    }else{
                        for(var i =0;i<paymentDetailEditStore.getCount();i++){
                           var amtInGrid  = paymentDetailEditStore.getAt(i).get(paymentChargedAmt);
                           if(amtInGrid !=''&& amtInGrid!=undefined && i !=selectRowNum){
                               paymentTotal = paymentTotal + amtInGrid*1;
                           }
                        }
                        
                    }
                    if(paymentTotal>chooseContractAmt){
	                    Ext.MessageBox.show({
		                    title:"提示",
		                    msg:"收入金额大于合同总额，是否继续？",
		                    buttons:Ext.Msg.YESNO ,
			                    fn:function(btn){
	                                if(btn=='no'){
		                               paymentAmtEditField.setValue('');
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

    var paymentDateEditField = new Ext.form.DateField({
        id:"paymentDateEditField",
        format : 'Y-m-d',
        allowBlank: false  
    });
    var paymentUpDateDEditField = new Ext.form.DateField({
        id:"paymentUpDateDEditField",
	    format : 'Y-m-d',
	    readOnly: true
    });
    var paymentEditRemarkField = new Ext.form.TextField({

    });
   
 /**************************
  * PaymentAEmpPeasonWin
  * 
  */ 
    var currencyx = ''; 
      
    var PaymentAEmpPeasonSM= new Ext.grid.CheckboxSelectionModel({
         header: '',
         singleSelect:true
    }); 

    var PaymentAEmpPeasonColM=new Ext.grid.ColumnModel(
        [
        PaymentAEmpPeasonSM,
        {header: "部门名称",dataIndex: "deptName", sortable:true , hidden:true},
        //{header: "联系人ID",dataIndex: "<%=DBSchema.EmployeeBelongToDept.EmployeeID %>", sortable:true, hidden:true},
        {header: "联系人名称",dataIndex: "user_name", sortable:true}
        ]
    );  

    var PaymentAEmpPeasonStore=new Ext.data.GroupingStore({
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
                 {name:EmployeeIDInPaymentCharged},
                 {name:'user_name'}
           ]
        }),
        groupField:'deptName'
    });
    
     PaymentAEmpPeasonStore.on('load',function(store,records,options){
          var selectValue = searchPaymentAEmpField.getValue();
    if(selectValue !== ''){
        PaymentAEmpPeasonGrid.getView().expandAllGroups();
     
        var arr=[];
        for(var i=0;i<records.length;i++){
            var record = records[i];
            record.get('user_name') 
            if(record.get('user_name')==selectValue){
                arr.push(record);
            }
        }
        PaymentAEmpPeasonSM.selectRecords(arr);
    }else{
        PaymentAEmpPeasonGrid.getView().collapseAllGroups();
    }
  },this,{delay:100})
    this.showPaymentEditGridInJS= function( id,contractName,contractType,curr){

        if(curr=="欧元"){
            currencyx ='€';
        }else{
            if(curr==""){
               currencyx ='￥'; 
            }else{
                currencyx = curr;
            }
        };        
        paymentEditPanel.show();//显示整个信息panel
        if(contractType ==IncomeContractTypeInPE ){
            paymentEditPanel.setTitle("收款明细列表"+'-'+contractName);
            paymentDetailEditGrid.getColumnModel().setColumnHeader(2,"收款日期");
            paymentDetailEditGrid.getColumnModel().setColumnHeader(3,"收款金额");
            
        }else{
            paymentEditPanel.setTitle("付款明细列表"+'-'+contractName);
            paymentDetailEditGrid.getColumnModel().setHidden(1,true);
            paymentDetailEditGrid.getColumnModel().setColumnHeader(2,"付款日期");
            paymentDetailEditGrid.getColumnModel().setColumnHeader(3,"付款金额");
            paymentDetailEditGrid.getColumnModel().setHidden(8,false);
            paymentDetailEditGrid.getColumnModel().setHidden(7,false);
        }
        downloadStoreInPayEd.removeAll();
        downloadStoreInPayEd.load({
	        params:{
	            applicationType: attachType,
	            refID: id 
	        }
        });

        contractTypeInEdit =contractType;
        contractIdInEdit = id;
        contractNameInEdit= contractName;
        paymentDetailEditStore.removeAll(true);
        paymentDetailEditStore.load({
            params:{
                contractID: id
            }
        });
       if( paymentEditPanel.isVisible()){
           paymentEditPanel.focus('',5);
       }
        
    };


    var PaymentAEmpPeasonGrid = new Ext.grid.GridPanel({
        id:"PaymentAEmpPeasonGrid",
        sm: PaymentAEmpPeasonSM,
        cm: PaymentAEmpPeasonColM,
        store: PaymentAEmpPeasonStore,
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
        stateId: 'grid',  
        forceFit:true, //Ture表示自动扩展或缩小列的宽度以适应grid的宽度,从而避免出现水平滚动条.   
        loadMask: {msg:'正在加载数据，请稍侯……'},
        height:320,
        
        tbar: [{
            ref: '../okBtn',
            iconCls: 'ok',
            text: '确定',
            handler: function(){
                var record = PaymentAEmpPeasonGrid.getSelectionModel().getSelected();
                if(record == 'undefined' ||record ==undefined){Ext.Msg.alert("未选择","请先选择！");return true;}
                              
                searchPaymentAEmpField.setValue(record.get('user_name'));
                paymentEditAEmpIDField.setValue(record.get(EmployeeIDInPaymentCharged));
                PaymentAEmpPeasonWin.hide();
           
            }
        },'-',{
            ref: '../cancelBtn',
            iconCls: 'remove',
            text: '取消',
            disabled: false,
            handler: function(){
                if(PaymentAEmpPeasonWin) PaymentAEmpPeasonWin.hide();
                
                //there is an err of dom.style when window.close() on (window + grid), so shuold use window.hide() to close
            }
        }

        ]            

    });

    var PaymentAEmpPeasonWin = new Ext.Window({ 
        id:"PaymentAEmpPeasonWin",
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
        items:[PaymentAEmpPeasonGrid]   
    });

    var searchPaymentAEmpField = new Ext.ux.form.SearchField({
        id:"searchPaymentAEmpField",
        allowBlank: false,
        emptyText :"必选项...",
        hasSearch : false,
        validateOnBlur:true,
        editable :false,
        //width: 125,
        
        onTrigger2Click : function(){//点击查询按钮或回车调用该方法 

            PaymentAEmpPeasonWin.show();
            PaymentAEmpPeasonWin.setTitle("选择经办人");
            PaymentAEmpPeasonStore.removeAll();
            PaymentAEmpPeasonStore.proxy.setUrl('crmMaint.do?method=getContactPersonByCompany',true);
            PaymentAEmpPeasonStore.load({
                params:{
                    compID: companyIdInPay 
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
    
    var paymentDetailEditGridEditor = new Ext.ux.grid.RowEditor({
        id:"paymentDetailEditGridEditor",
        saveText: '确认',
        cancelText: '取消',
        clicksToEdit: 2 ,
        listeners:{

        }
    });
    paymentDetailEditGridEditor.on({ 
       scope: this, 
       beforeedit: function(){
           paymentDetailEditGrid.removeBtn.setDisabled(true);
           paymentDetailEditGrid.newBtn.setDisabled(true);
       },
       beforehide: function(){

            var flag = hidenFlag;
            hidenFlag = true ;
            return flag;
       },
       afteredit: function(roweditor, changes, record, rowIndex){

                 paymentListEditForm.getForm().submit({
                     url : 'contractMain.do?method=savePaymentDetail',
                     method : 'post',
                     params : {
                        contractID:contractIdInEdit,
                        paymentDetailID:paymentEditIdField.getValue(),
                        contractType:contractTypeInEdit,
                        paymentDetailDate:paymentDateEditField.getValue(),
                        paymentDetailAmt:paymentAmtEditField.getValue(),
                        paymentDetailRemark: paymentEditRemarkField.getValue(),
                        transatorID: paymentEditAEmpIDField.getValue()
                                               
                     },
                     success : function(form, action) {
                        Ext.Msg.alert(action.result.messageTitle,action.result.message);
                        if(Ext.getCmp("paymentListEditGrid")){
                            reloadPaymentListEdit();//刷新合同列表中的付款项
                        }
                        paymentDetailEditGrid.getStore().reload({
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
                        var s = paymentDetailEditGrid.getSelectionModel().getSelections();
                        for(var i = 0, r; r = s[i]; i++){
                            paymentDetailEditStore.remove(r);
                        };
                        //paymentConditionGrid.getView().refresh();
                      
                    }
                  }); 
                  newPaymentEditRowFlag = 0;
                  paymentDetailEditGrid.getStore().reload({
                      callback:function(record, options ,success){
                          if(success == true){
                          }else{
                              Ext.Msg.alert(Ext_errMsg_For_Get_Store_Title,Ext_errMsg_For_Get_Store_Message );
                          }
                      }
                  });
             
           paymentDetailEditGrid.removeBtn.setDisabled(false);
           paymentDetailEditGrid.newBtn.setDisabled(false);
           newPaymentEditRowFlag= 0;
           selectRowNum ='';
       },
       canceledit: function(){
            if(newPaymentEditRowFlag == 1) {
                paymentDetailEditGridEditor.stopEditing(false);
                var s = paymentDetailEditGrid.getSelectionModel().getSelections();
                for(var i = 0, r; r = s[i]; i++){
                    paymentDetailEditStore.remove(r);
                };
                hidenFlag = true;
                paymentDetailEditGrid.getView().refresh();
                newPaymentEditRowFlag = 0;
           }
        
           paymentDetailEditGrid.getView().refresh();
           paymentDetailEditGrid.removeBtn.setDisabled(false);
           paymentDetailEditGrid.newBtn.setDisabled(false);
           //newPaymentEditRowFlag= 0;
           selectRowNum ='';
       }
    })
   
   
    /****************
     * 收款明细列表
     *************************/
    var paymentDetailEditGrid = new Ext.grid.GridPanel({
        id :"paymentDetailEditGrid",
        store: paymentDetailEditStore,
        layout:'fit',
        enableColumnMove:false,      //True表示启动对Column顺序的拖拽,反之禁用    
        enableColumnResize:true,      //设置为false可以禁止这个表格的列改变宽度 (默认值为 true).    
        stripeRows: true,   //True表示使用不同颜色间隔行，默认为false
        autoHeight: false,  
        autoWidth: false,
        width:Ext.get("paymentDetailEditGridDiv").getWidth()*17/24,
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
        stateId: 'paymentDetailEditGrid', 
        plugins: [paymentDetailEditGridEditor],
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


                var e = new paymentDetailEditRecord({
                    paymentChargedId:'',
                    paymentChargedDateDisplayed: '',//(new Date()).format('Y-m-d'),
                    paymentChargedDate:'',// add 1
                    paymentAEmpName: '',
                    paymentChargedAmt: '0',
                    paymentBEmpName: '',
                    paymentUpdateDate: '',
                    paymentTransactorID:'',
                    paymentUpdateDateDisplayed :'',//add 2
                    paymentCurrencyGraph:'',
                    paymentCurrencyExchangeRate:'',
                    paymentChargedRemark:''
                });

                paymentDetailEditGridEditor.stopEditing();

                paymentDetailEditStore.insert(0, e);

                paymentDetailEditGrid.getView().refresh();

                paymentDetailEditGrid.getSelectionModel().selectRow(0);

                paymentDetailEditGridEditor.startEditing(0);
             
                newPaymentEditRowFlag =1;
          

            }
        }, '-' , {
            ref: '../removeBtn',
            iconCls: 'remove',
            text: '删除',
            disabled: false,
            handler: function(){
                
                var record = paymentDetailEditGrid.getSelectionModel().getSelected();
                if(record == 'undefined' ||record ==undefined){Ext.Msg.alert("未选择","请先选择！");return true;}
    
                paymentListEditForm.getForm().submit({
                      url : 'contractMain.do?method=delPaymentDetail',
                     method : 'post',
                     params : {
                        contractID: contractIdInEdit,
                        paymentDetailID: record.get(paymentChargedId)
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
                        paymentDetailEditGrid.getStore().reload({
                            callback:function(record, options ,success){
                                if(success == true){
                                }else{
                                    Ext.Msg.alert(Ext_errMsg_For_Get_Store_Title,Ext_errMsg_For_Get_Store_Message);
                                }
                            }
                        });
                        paymentDetailEditGrid.getView().refresh();
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
		             id: 'paymentDetailUpload',	
		             baseCls : 'x-plain',
		             labelWidth : 70,
		             fileUpload : true,
		             defaultType : 'textfield',
		             items : [{
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
                var strUrl = '/xMIS/fileUpload.do?type='+attachType;
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

		                                   downloadStoreInPayEd.load({
										        params:{
										            applicationType: attachType,
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
                header: "收款Id", dataIndex:paymentChargedId, sortable:true, 
                hidden: true,
                //renderer:customer_formatDate, 
                align: 'center',
                editor:paymentEditIdField
            },{
	            header: "收款日期", dataIndex:paymentChargedDateDisplayed, sortable:true,  
                //defaultSortable:true,
                align: 'center',
                xtype: 'datecolumn',
                format: 'Y-m-d',
                //renderer:customer_formatDate,
	            editor:paymentDateEditField
            },
        
	        {
	            xtype: 'numbercolumn',
	            format:'0,000.00',
                header: "收款金额", dataIndex:paymentChargedAmt, sortable:true, 
	            //renderer:formatCurInPayEdit, //function(value, cellMeta, record, rowIndex, columnIndex, store){
	                    //return formatCurInPayEdit( value, 'blue');
	           // }, 
	            align: 'right',
	            editor: paymentAmtEditField
	            
	        },{
                header: "经办人Id",   
                hidden: true,
                dataIndex:paymentTransactorID,
                //renderer:customer_formatDate, 
                align: 'center',
                editor:paymentEditAEmpIDField
            },{
	            header: "经办人",
                dataIndex:paymentAEmpName,
                sortable:true, 
                renderer: convertUserName,
		        editor:searchPaymentAEmpField
		    },
            {
                header: "备注", dataIndex: paymentChargedRemark, sortable: true,
                editor :paymentEditRemarkField
            },
	        {
	            header: "登记日期", dataIndex:paymentUpdateDateDisplayed, sortable:true, 
                //renderer:customer_formatDate,
		        xtype: 'datecolumn',
                //type:'date',
                format: 'Y-m-d',
		        align: 'center',
		        editor :paymentUpDateDEditField//此处为空会造成编辑状态格式问题
	        },
	        {
	            header: "登记人",dataIndex:paymentBEmpName, sortable:true, 
		        editor :null
	        }]
        })

    });
    
     paymentListEditForm = new Ext.form.FormPanel({
     	id: 'paymentListEditFrm',
        baseCls : 'x-plain',
        anchor : '100%',
        items:[paymentDetailEditGrid]
    });

    this.hidePaymentEditGridInJS=function(){
        paymentEditPanel.setVisible(false);
    };
    
    

   /*====================================================================
     * Show download files
     *====================================================================*/
     
    var downloadStoreInPayEd=new Ext.data.Store({
        proxy: new Ext.data.HttpProxy({
            url:'contractMain.do?method=getAttachedListStroeByApplicationType'//修改成新滴
        }),
        remoteSort:true,
        sortInfo:{field: attachmentUploadDate, direction: 'ASC'},
        reader:new Ext.data.JsonReader({
            root:'rows',
            totalProperty: 'totalCount',
            //remoteSort:true,
            fields:[
                {name:AttachmentIDInPayEd},
                {name:AttachmentNameInPayEd},
                {name:attachmentUploaderName},
                {name:attachmentUploadDate}
           ]
        })
    });   

  /*  downloadStoreInPayEd.load({
        params:{
            applicationType: attachType,
            refID: contractIdInEdit 
        }
    });*/
    downloadStoreInPayEd.on('load',function(store,records,options){

        if(records.length==0){
            downloadGridInPayEd.show();
        }else{
            downloadGridInPayEd.show();


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
            dataIndex: AttachmentNameInPayEd,
            renderer: nameLinker
        },{
            header:'上传人',
            dataIndex: attachmentUploaderName
        },{
            header: "上传日期", 
            dataIndex:attachmentUploadDate, 
            sortable:true,   
            xtype: 'datecolumn',
            format: 'Y-m-d',
            align: 'center'
        }]
    });
    var downloadGridInPayEd = new Ext.grid.GridPanel({
        id :'downloadGridInPay',
        title:'附件信息',
        enableColumnMove:false,      //True表示启动对Column顺序的拖拽,反之禁用    
        enableColumnResize:true,      //设置为false可以禁止这个表格的列改变宽度 (默认值为 true).    
        cm: downloadCMInPayEd,
        layout : 'fit',
        store: downloadStoreInPayEd,
        stripeRows: true,   //True表示使用不同颜色间隔行，默认为false
        autoExpandColumn: 0,
        viewConfig:{forceFit:true},
        stateful: true,
        height:185,
        autoHeight: false,
        autoWidth:false,
        width:Ext.get("paymentDetailEditGridDiv").getWidth()*7/24,
        stateId: 'downloadGrid',
        loadMask:true     //读取数据时的遮罩和提示功能，默认为false            
    });   

   var paymentEditPanel = new Ext.Panel({
        id:"paymentEditPanel",
        width :Ext.get("paymentDetailEditGridDiv").getWidth(),//
        height: 212,
        hidden:true,
        //border:false,
        renderTo:"paymentDetailEditGridDiv",
        title:"收款明细列表",
        layout:{
            type:'hbox',
            border:false,
            align:'stretchmax'
        },
        items:[{
            items:[paymentListEditForm],
            flex:17           
        },{
            items:[downloadGridInPayEd],
            flex:7          
        }]
        
    });	
 });    
