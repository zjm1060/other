<%@ page language="java" import="java.util.*" pageEncoding="GB18030"%>
<%@ page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<%@ page import="com.bitc.jk.xmis.util.DBSchema"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<!DOCTYPE HTML>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
EmployeeObj empInInvo = (EmployeeObj)request.getSession().getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
%>

<html>
  <head>
    <base href="<%=basePath%>">
    
    <title>My JSP 'invoiceEditGrid.jsp' starting page</title>
    
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	<!--
	<link rel="stylesheet" type="text/css" href="styles.css">
	-->

  </head>
  
  <body>
   <div id="invoiceDetailEditGridDiv" style="width:100%"></div>
  </body>
</html>

<script language="JavaScript" src="<%=path %>/contract/invoiceEditGrid.js"></script>

<script language="javascript">
    var companyIdInInvo =  "<%= empInInvo.getCompanyID()%>";
	var invoiceContractID = "<%=DBSchema.InvoiceDetail.ContractID%>";
	var InvoiceNo = "<%=DBSchema.InvoiceDetail.InvoiceNo%>";
	var InvoiceAmt = "<%=DBSchema.InvoiceDetail.InvoiceAmt%>";
	
	var InvoiceDate ="<%=DBSchema.InvoiceDetail.InvoiceDate%>";
	
	var TransactorID ="<%=DBSchema.InvoiceDetail.TransactorID%>";
	var ScanAttachment ="<%=DBSchema.InvoiceDetail.ScanAttachment%>";
	var invoiceRemark ="<%=DBSchema.InvoiceDetail.Remark%>";
	
	var InvoiceDateDisplayed = "<%=DBSchema.InvoiceDetail.InvoiceDateDisplayed%>";
	var UpdateID = "<%=DBSchema.InvoiceDetail.UpdateID%>";
	var InvoiceUpdateDate = "<%=DBSchema.InvoiceDetail.UpdateDate%>";
	var UpdateDateDisplayed ="<%=DBSchema.InvoiceDetail.UpdateDateDisplayed%>";
	var invoiceAEmpName = "<%=DBSchema.Employee.aEmpName %>";
	var invoiceBEmpName = "<%=DBSchema.Employee.bEmpName%>";
	var InvoiceCode = "<%=DBSchema.InvoiceDetail.InvoiceCode%>";
	var ReceiverID = "<%=DBSchema.InvoiceDetail.ReceiverID%>";
	var ReceiverName="<%=DBSchema.Employee.cEmpName%>";
	var MailCode = "<%=DBSchema.InvoiceDetail.MailCode%>";
	
	var TotalAmtByID ="<%=DBSchema.InvoiceDetail.TotalAmtByID%>";
	var UnAccountAmtTotalSum ="<%=DBSchema.InvoiceDetail.UnAccountAmtTotalSum%>";
	
	var ReceiptDateDisplayed = "<%=DBSchema.Receipt.ReceiptDateDisplayed%>";
	var ReceiptUpdateID = "<%=DBSchema.Receipt.UpdateID%>";
	var ReceiptUpdateDate = "<%=DBSchema.Receipt.UpdateDate%>";
	var ReceiptUpdateDateDisplayed ="<%=DBSchema.Receipt.UpdateDateDisplayed%>";
	var ReceiptAEmpName = "<%=DBSchema.Employee.aEmpName %>";
	var ReceiptBEmpName = "<%=DBSchema.Employee.bEmpName%>";
	var ReceiptCode = "<%=DBSchema.Receipt.ReceiptCode%>";
	
	var ReceiptTotalAmtByID ="<%=DBSchema.InvoiceDetail.TotalAmtByID%>";
	
	var attchmentType ="<%=WebKeys.Upload_Application_InvoiceInfo_Type%>";
	var attchmentType2 ="<%=WebKeys.Upload_Application_ReceiptInfo_Type%>";
	
	//accthment
	 var  AttachmentIDInInvoEd ='<%=DBSchema.Attachment.AttachmentID %>';
	 var  AttachmentNameInInvoEd='<%=DBSchema.Attachment.AttachmentName %>';
	 var  attachmentInvoUploaderName = "<%=DBSchema.Attachment.UploaderName%>";
	 var  attachmentInvoUploadDate = "<%=DBSchema.Attachment.UploadDisplayDate%>";
	
	//person
	
	var EmployeeIDInInvo = "<%=DBSchema.EmployeeBelongToDept.EmployeeID %>"

    //amt
    var choosedAmt;
    showInvoiceEditGrid  = function(id,contractName,contractType,c,amt,type,aCmpId){
    	switch(c){
	  		case "￥":
	  			currencyx = c;
	  			break;
	  		case "$"://＄
	  			currencyx = c;
	  			break;
	  	    case "欧元":
	  			currencyx = "€";
	  			break;
	  		case "hk$":
	  			currencyx = "hk$";
	  			break;
	  		default:
	  			currencyx = "￥";
	  			break;
	   	};
	   	choosedAmt = amt;
	        showInvoiceEditGridInJS(id,contractName,contractType,currencyx,type,aCmpId); 
	         
	    };
    hideInvoiceEditGrid = function(){
	    	hideInvoiceEditGridInJS();
    };
    var receiverNameField;
    var receiverIDField;
    var contructPersonFlag="";
    var aCmpId;
    var newContactPersonForm = new Ext.form.FormPanel({
		baseCls : 'x-plain',
		anchor : '100%',
		lableWidth : 10,
		defaultType : 'textfield',
		defaults : {
			xtype : "textfield",
			width : 200
		},
		items : [{
					id : 'contactPersonName',
					fieldLabel : "联系人名称",
					inputType : 'text',
					name : 'empName',
					allowBlank : false,
					blankText : "联系人名称不能为空",
					msgTarget : 'under'
				}],
		buttons : [{
			text : "创建",
			// disabled:true,
			id : 'PersonConfermBtn',
			handler : function() {
				validateFunc(
						'<%=WebKeys.New_Company_Simple_Type_ContactPerson %>',
						'contactPersonName', newContactPersonForm);
			}
		}, {
			text : "取消",
			handler : function() {
				newContactPersonForm.ownerCt.hide();
			}
		}]
	});
var newContactPersonWin;
var chooseWin;
function showContactPersonWin(title, win) {
chooseWin = win;
if (!newContactPersonWin) {
	var newContactPersonWin = new Ext.Window({
				layout : 'fit',
				modal : true,
				title : title,
				renderTo : Ext.getBody(),
				frame : true,
				plain : true,
				resizable : false,
				buttonAlign : "center",
				closable : false,
				bodyStyle : "padding:10px",
				width : 360,
				height : 130,
				layout : "form",
				// lableWidth:10,
				defaults : {
					xtype : "textfield",
					width : 200
				},
				// monitorValid:true,
				listeners : {
					'show' : function() {
						newContactPersonForm.findByType('textfield')[0]
								.focus(true, true);
					}
				},
				items : newContactPersonForm

			});
};
Ext.getCmp('contactPersonName').setValue('');
newContactPersonWin.setTitle(title);
newContactPersonWin.show();
};
function validateFunc(type,id,chooseForm){ 
	if(chooseForm.findById(id).getValue() ==""){
	    Ext.Msg.alert("请输名称！","名称不能为空！");
		chooseForm.findById(id).focus();
		return  true;
	}	
	var deptID;
	chooseForm.getForm().submit({
         url : __cxtPath+'/crmMaint.do?method=dataValidate',
		 params : {
		 	compID: aCmpId,
		 	deptID:deptID,
		 	type: type
		 },
         method : 'post',
         success : function(form, action) {
          	submitForm();
         },
     	failure : function(form, action) {
			Ext.Msg.confirm(action.result.messageTitle,action.result.message, function(btn, text){
			    if (btn == 'yes'){
			        submitForm();
			    }
			    if(btn=='no'){
					chooseForm.findById(id).setValue('');
		         	chooseForm.findById(id).focus();
			    }
		});
   	}
});
	
	function submitForm(){
		   chooseForm.getForm().submit({
	         url : __cxtPath+'/crmMaint.do?method=createNewCompanySimple',
			 params : {
				 compID: aCmpId,
				 deptID:deptID,
			     type: type
			 },
	         method : 'post',
	         success : function(form, action) {
	         	//alert(action.result.message);
	         	Ext.Msg.alert(action.result.messageTitle,action.result.message);
	         	receiverIDField.setValue(action.result.empID);
	           	receiverNameField.setValue(action.result.empName);
	           	if(newContactPersonWin){
	           		newContactPersonWin.hide();
	           	}
	           	chooseWin.close();
	         },
		       	failure : function(form, action) {
		        	Ext.Msg.alert(action.result.messageTitle,action.result.message);
		       	}
			}); 
	 	};
}
    
</script>
