


Ext.onReady(function(){ 
    // 
    var data={ 

       read:[
        { lable:"合同编号", value:ContractNoInfo },
        { lable:"合同名称", value:contractName },
        { lable:"合同简称", value:contractAbbreviation },
        { lable:"项目名称", value:projectNameInContract },
        { lable:"甲方公司", value:contractBuyer },
        { lable:"甲方部门", value:contractDeptNameOfBuyer },
        { lable:"甲方代表", value:contractBuyerRepresentativeName },
        { lable:"合同金额", value:amtInContract},
        { lable:"合同摘要", value:ContractSummaryInfo },        
        { lable:"收 货 人", value:contractDeliveryName },                
        { lable:"乙方公司", value:contractVendorName },
        { lable:"乙方部门", value:contractDeptNameOfVendor }, 
        { lable:"乙方代表", value:contractVendorRepresentativeName }                
        ],
        read1:[
        { lable:"合同类型", value:ContractTypeNameInfo },

        { lable:"合同来源", value:GovermentContractSourceName },  
        
        { lable:"项目编号", value:projectIDInContract },
        
        { lable:"父合同简称", value:contractParentAbb },
        { lable:"父合同编号", value:contractParentCode },
        { lable:"父合同名称", value:contractParentName },

        { lable:"质保条款", value:contractWarrantyDefinition },
        { lable:"质保期起", value:warrantyStartDate },//warrantyStartDate
        { lable:"质保期止", value:WarrantyEndDate },
        { lable:"质保期限", value:contractWarrantyLimit },
        { lable:"合同备注", value:contractRemark },
        
    
        { lable:"签定日期", value:signatureDate }, 
        { lable:"登 记 人", value:contractRegisterName }      
      ] 
    } ;

    var contractInfopanel=new Ext.Panel({ 
        width:472,
        autoWidth :false,
        autoHeight :true,
        //height :400,
        id:"mypanel", 
        collapsible:true,
        layout:'fit',
        title:"合同基本性息",

        renderTo: contractInfoDivName
    }); 

    var tpl=new Ext.XTemplate( 
   
                 '<table >',
                    '<tbody>',
                    '<tr>',
                        
                        '<td>&nbsp;</td>',
                        '<td>',
                            '<br>',
                            '<table>',
                            //第一列
                            '<tpl for="read">',
					         '<tr><td>', '<span  class="terminal-detail-title" >{lable}:&nbsp<font color=blue>{value}</font></span>','</td></tr>', 
					        '</tpl>',
                            '</table>',
                        '</td>',
                        '<td>&nbsp;&nbsp;</td>',
                        '<td>',
                            '<br>',
                            '<table>',
                            //第二列
                             '<tpl for="read1">',
                             '<tr><td>', '<span  class="terminal-detail-title" >{lable}:&nbsp<font color=blue width=260px>{value}</font></span>','</td></tr>', 
                             '</tpl>',
                             '</table>',
                        '</td>',
                    '</tr>',
                    '<td>&nbsp;</td>',
                '</tbody>',
                '</table>'
    ); 

    tpl.overwrite(contractInfopanel.body,data); 
    
    var dataOther={ 

       read:[      
       
        { lable:"已收合同金额", value:contractEarnedTotalAmt },
        { lable:"未收合同金额", value:contractArrearageTotalAmt },//
        { lable:"已开发票金额", value:contractInvoicedTotalAmt },
        { lable:"已开发票未到账金额", value:UnCreditedTotalAmt }//
         
        ] 
    };
     var contractInfopanelOther=new Ext.Panel({ 
        width:472, 
        autoWidth :false,
        id:"mypanelOther", 
        collapsible:true,
        collapsed: true, 
        layout:'fit',
        title:"合同其他信息",
        listeners:{
             'expand':function(){
                //contractInfopanelOther.ownerCt.scrollTo(0,100);

             //this.scroller.dom.scrollTop =200;
             }
        },

        renderTo: contractInfoOtherDivName
    }); 
    /*
     *  '<table>', 
        '<tr><td>', 
        '<tpl for="read">',
         '<tr><td>', '<span  class="terminal-detail-title" >{lable}:&nbsp<font color=blue >{value}</font></span>','</td></tr>',

        '</tpl></td></tr></table>'
        <div id="downloadFileDiv"> 
<table border="1" width="300"> 

</table> 
</div> 

                    '<table>',
                    //第二列
                    '<tr><td>aaa</td>',
                         '<td><div id="downloadFileDiv">hgggg</div>',
                         '</td></tr>',
                  
                    '</table>',
         */
     var tplother=new Ext.XTemplate( 
         '<table >',
            '<tbody>',
            '<tr>',
                
                '<td>&nbsp;</td>',
                '<td>',
                    '<br>',
                    '<table>',
                    //第一列
                    '<tpl for="read">',
                      '<tr><td>', '<span  class="terminal-detail-title" >{lable}:&nbsp<font color=blue >{value}</font></span>','</td></tr>',
                    '</tpl>',
                    '</table>',
                '</td>',
                '<td>&nbsp;&nbsp;</td>',
                '<td>',
                   // '<br>',
                     //第二列
		        '</td>',
            '</tr>',
            '<td>&nbsp;</td>',
        '</tbody>',
        '</table>'
       
    ); 
     tplother.overwrite(contractInfopanelOther.body,dataOther); 
     
     /*====================================================================
     * Show download files
     *====================================================================*/
     
    var downloadStore=new Ext.data.Store({
        proxy: new Ext.data.HttpProxy({
            url:'contractMain.do?method=getAttachedListStroe'
        }),
        remoteSort:true,
        reader:new Ext.data.JsonReader({
            root:'rows',
            totalProperty: 'totalCount',
            remoteSort:true,
            fields:[
                {name:AttachmentID},
                {name:AttachmentName}
           ]
        })
    });   

    downloadStore.load({
        params:{
            contractID: contractIdInfo 
        }
        });
    downloadStore.on('load',function(store,records,options){
        if(records.length==0){
            downLoadcontractAtt.hide();
            
        }
  },this,{delay:0})

        var downloadCM=new Ext.grid.ColumnModel(
        [{
            width:Ext.get(ContractAttd).getWidth(), 
            dataIndex: AttachmentName,
            renderer: nameLinker
        }]
    );
    var downloadGrid = new Ext.grid.GridPanel({
        enableColumnMove:false,      //True表示启动对Column顺序的拖拽,反之禁用    
        enableColumnResize:false,      //设置为false可以禁止这个表格的列改变宽度 (默认值为 true).    
        cm: downloadCM,
        store: downloadStore,
        stripeRows: true,   //True表示使用不同颜色间隔行，默认为false
        autoExpandColumn: 0,
        //height:Ext.get("downloadFileDiv").getHeight(),
        //width:Ext.get("downloadFileDiv").getWidth(),
        stateful: true,
        autoHeight: true, 
        stateId: 'downloadGrid',
        loadMask:true,      //读取数据时的遮罩和提示功能，默认为false   
        forceFit:true //Ture表示自动扩展或缩小列的宽度以适应grid的宽度,从而避免出现水平滚动条.   
        
       
    });   
    var downLoadcontractAtt=new Ext.Panel({ 
        width:472, 
        autoWidth :false,
        id:"downloadContractAtt", 
        collapsible:true,
        collapsed: true,
        listeners:{
             'expand':function(){
                downLoadcontractAtt.focus(1,10);
                //contractInfopanelOther.ownerCt.scrollTo(0,100);

                //this.scroller.dom.scrollTop =200;
             }
        },
        layout:'fit',
        title:"附件信息",
        items:[downloadGrid],
        renderTo: ContractAttd
    }); 
    
}) 