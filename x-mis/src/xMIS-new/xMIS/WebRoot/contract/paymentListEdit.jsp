<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ page import="com.bitc.jk.xmis.util.DBSchema"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<!DOCTYPE HTML>
	 
<%
String path = request.getContextPath();
if("/".equals(path)){
	path="";
}
%>

<html>
  <head>
    
    <title>My JSP 'paymentListEdit.jsp' starting page</title>
    
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	<!--
	<link rel="stylesheet" type="text/css" href="styles.css">
	-->
	<script language="JavaScript" src="<%=path %>/contract/contractInfoShow.js"></script>
	<script language="JavaScript" src="<%=path %>/contract/paymentListEdit.js"></script>


  </head>
  
  <body>
    <div id="paymentListEditDiv" style="width:100%"></div>
    <jsp:include page="<%=WebKeys.Include_JSP_File_paymentEdit %>" />
	 <jsp:include page="../contract/invoiceEditGrid.jsp" />
  </body>
</html>

<script language="javascript">

    /*Contract Type*/
 //   public static final int Type_ContractType_Income = 1; //收入合同
 //   public static final int Type_ContractType_Payout = 2; //支出合同
var IncomeContractId =  "<%=WebKeys.Type_ContractType_Income%>";
var PayoutContractId =  "<%=WebKeys.Type_ContractType_Payout%>";

var invoiceEditFlag = false; //是否显示收款链接

var ContractID = "<%=DBSchema.Contract.ContractID %>";
var ContractNo = "<%=DBSchema.Contract.ContractNo %>";
var ContractName = "<%=DBSchema.Contract.ContractName %>";
var Abbreviation = "<%=DBSchema.Contract.Abbreviation %>";
var ParentID = "<%=DBSchema.Contract.ParentID %>";
var ParentContractAbb = "<%=DBSchema.Contract.ParentContractAbb %>";
var SignatureDate = "<%=DBSchema.Contract.SignatureDate %>";
var BuyerID = "<%=DBSchema.Contract.BuyerID %>";
var BuyerAbb = "<%=DBSchema.Contract.BuyerAbb %>";
var DeptIDOfBuyer = "<%=DBSchema.Contract.DeptIDOfBuyer %>";
var DeptNameOfBuyer = "<%=DBSchema.Contract.DeptNameOfBuyer %>";
var VendorID = "<%=DBSchema.Contract.VendorID %>";
var VendorAbb = "<%=DBSchema.Contract.VendorAbb %>";
var DeptIDOfVendor = "<%=DBSchema.Contract.DeptIDOfVendor %>";
var DeptNameOfVendor = "<%=DBSchema.Contract.DeptNameOfVendor %>";
var ContractAmt = "<%=DBSchema.Contract.ContractAmt %>";
var BuyerRepName = "<%=DBSchema.Contract.BuyerRepName %>";
var VendorRepName = "<%=DBSchema.Contract.VendorRepName %>";
var GovermentContractSourceName = "<%=DBSchema.Contract.GovermentContractSourceName %>";
var ContractSummary = "<%=DBSchema.Contract.ContractSummary %>";
var Remark = "<%=DBSchema.Contract.Remark %>";
var RepName = "<%=DBSchema.Contract.RepName %>";
var UpdateDate = "<%=DBSchema.Contract.UpdateDate %>";
var ProjectID = "<%=DBSchema.Project.ProjectID %>";
var ProjectName = "<%=DBSchema.Project.ProjectName %>";
var CurrenyGraph = "<%=DBSchema.CurrencyType.CurrenyGraph %>";
var TotalAmtByID = "<%=DBSchema.ChargedDetail.TotalAmtByID %>";
var UnChargedAmtTotal = "<%=DBSchema.ChargedDetail.UnChargedAmtTotal %>";

var invoiceTotalAmt = "<%=DBSchema.InvoiceDetail.TotalAmtByID %>";
var ReceiptTotalAmt = "<%=DBSchema.Receipt.TotalAmtByID %>";
var invoiceUnAccountAmt = "<%=DBSchema.InvoiceDetail.UnAccountAmtTotalSum %>";

    reloadPaymentListEdit = function(){
    	reloadPaymentEditInJs();
    };
/*
//以下为payment 录入GRID

var paymentChargedDate = "<%=DBSchema.ChargedDetail.ChargedDate%>";
var paymentChargedDateDisplayed = "<%=DBSchema.ChargedDetail.ChargedDateDisplayed%>";
var paymentChargedId   = "<%=DBSchema.ChargedDetail.ChargedID%>";
var paymentAEmpName = "<%=DBSchema.Employee.aEmpName%>";
var paymentChargedAmt = "<%=DBSchema.ChargedDetail.ChargedAmt%>";
var paymentBEmpName = "<%=DBSchema.Employee.bEmpName%>";
var paymentUpdateDate = "<%=DBSchema.ChargedDetail.UpdateDate%>";
var paymentUpdateDateDisplayed ="<%=DBSchema.ChargedDetail.UpdateDateDisplay%>";
var paymentCurrencyGraph = "<%=DBSchema.CurrencyType.CurrenyGraph%>";
var paymentCurrencyExchangeRate = "<%=DBSchema.CurrencyType.ExchangeRate%>";
var paymentChargedRemark = "<%=DBSchema.ChargedDetail.ChargedRemark%>";

// 以下为personSotre;
var EmployeeIDInPaymentCharged = "<%=DBSchema.EmployeeBelongToDept.EmployeeID %>"
*/
</script>