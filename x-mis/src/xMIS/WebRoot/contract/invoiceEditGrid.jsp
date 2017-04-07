<%@ page language="java" import="java.util.*" pageEncoding="GB18030"%>
<%@ page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<%@ page import="com.bitc.jk.xmis.util.DBSchema"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>

<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
EmployeeObj empInInvo = (EmployeeObj)request.getSession().getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
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

<script language="JavaScript" src="/xMIS/contract/invoiceEditGrid.js"></script>

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
	
	var TotalAmtByID ="<%=DBSchema.InvoiceDetail.TotalAmtByID%>";
	var UnAccountAmtTotalSum ="<%=DBSchema.InvoiceDetail.UnAccountAmtTotalSum%>";
	
	var attchmentType = "<%=WebKeys.Upload_Application_InvoiceInfo_Type%>";
	
	//accthment
	 var  AttachmentIDInInvoEd ='<%=DBSchema.Attachment.AttachmentID %>';
	 var  AttachmentNameInInvoEd='<%=DBSchema.Attachment.AttachmentName %>';
	 var  attachmentInvoUploaderName = "<%=DBSchema.Attachment.UploaderName%>";
	 var  attachmentInvoUploadDate = "<%=DBSchema.Attachment.UploadDisplayDate%>";
	
	//person
	
	var EmployeeIDInInvo = "<%=DBSchema.EmployeeBelongToDept.EmployeeID %>"

    //amt
    var choosedAmt;
    showInvoiceEditGrid  = function(id,contractName,contractType,c,amt){
    
    	switch(c){
	  		case "£¤":
	  			currencyx = c;
	  			break;
	  		case "$"://¡ç
	  			currencyx = c;
	  			break;
	  	    case "Å·Ôª":
	  			currencyx = "¢ã";
	  			break;
	  		case "hk$":
	  			currencyx = "hk$";
	  			break;
	  		default:
	  			currencyx = "£¤";
	  			break;
	   	};
	   	choosedAmt = amt;
	        showInvoiceEditGridInJS(id,contractName,contractType,currencyx); 
	         
	    };
    hideInvoiceEditGrid = function(){
	    	hideInvoiceEditGridInJS();
    };
    
</script>
