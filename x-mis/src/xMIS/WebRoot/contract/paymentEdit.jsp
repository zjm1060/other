<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<%@ page import="com.bitc.jk.xmis.util.DBSchema"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>

<%
EmployeeObj empInPay = (EmployeeObj)request.getSession().getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
%>
<script language="javascript">
//contractType
var IncomeContractTypeInPE ="<%=WebKeys.Type_ContractType_Income%>";
//companyId
    var  companyIdInPay  ="<%=empInPay.getCompanyID() %>" ;
    
//attachment
    var  AttachmentIDInPayEd ='<%=DBSchema.Attachment.AttachmentID %>';
    var  AttachmentNameInPayEd='<%=DBSchema.Attachment.AttachmentName %>';
    var  attachmentUploaderName = "<%=DBSchema.Attachment.UploaderName%>";
    var  attachmentUploadDate = "<%=DBSchema.Attachment.UploadDisplayDate%>";
//以下为payment 录入GRID

var paymentChargedDate = "<%=DBSchema.ChargedDetail.ChargedDate%>";
var paymentChargedDateDisplayed = "<%=DBSchema.ChargedDetail.ChargedDateDisplayed%>";
var paymentChargedId   = "<%=DBSchema.ChargedDetail.ChargedID%>";
var paymentTransactorID = "<%=DBSchema.ChargedDetail.TransactorID%>"

var paymentAEmpName = "<%=DBSchema.Employee.aEmpName %>";
  /* if( paymentAEmpName == "aEmp_name"){
       paymentAEmpName = '';
       alert('aaa='+paymentAEmpName);
   }*/
var currencyx; 
var chooseContractAmt;
var paymentChargedAmt = "<%=DBSchema.ChargedDetail.ChargedAmt%>";
var paymentBEmpName = "<%=DBSchema.Employee.bEmpName%>";
var paymentUpdateDate = "<%=DBSchema.ChargedDetail.UpdateDate%>";
var paymentUpdateDateDisplayed ="<%=DBSchema.ChargedDetail.UpdateDateDisplay%>";
var paymentCurrencyGraph = "<%=DBSchema.CurrencyType.CurrenyGraph%>";

var paymentCurrencyExchangeRate = "<%=DBSchema.CurrencyType.ExchangeRate%>";
var paymentChargedRemark = "<%=DBSchema.ChargedDetail.ChargedRemark%>";
// 以下为 附件参数
var attachType = "<%=WebKeys.Upload_Application_PaymentInfo_Type%>";




// 以下为personSotre;
var EmployeeIDInPaymentCharged = "<%=DBSchema.EmployeeBelongToDept.EmployeeID %>"
    
    showPaymentEditGrid  = function(id,contractName,contractType,c,amt){
    
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
   	chooseContractAmt = amt;
   	//alert(chooseContractAmt+'##in pE')
        showPaymentEditGridInJS(id,contractName,contractType,currencyx); 
         
    };
    hidePaymentEditGrid = function(){
    	hidePaymentEditGridInJS();
    };

// 需要两个参数  合同类型 合同ID

 //    <tr>
//  	  <td rowSpan=6 width=740px valign= bottom align = right><div id=paymentDetailEditGridDiv width=740px></div><br>
		    
//	    <br></td>
//	 </tr>   

 //   <div id="paymentDetailEditGridDiv" style="width:100%"></div>
</script>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <title>My JSP 'paymentListEdit.jsp' starting page</title>
    
  </head>
  <body>
  	<div id="paymentDetailEditGridDiv" style="width:100%"></div>
  </body>  
</html>

<script language="JavaScript" src="/xMIS/contract/paymentEdit.js"></script>



