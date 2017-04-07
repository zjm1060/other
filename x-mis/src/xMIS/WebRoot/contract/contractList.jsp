<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<%@ page import="com.bitc.jk.xmis.util.DBSchema"%>

<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>

<script language="javascript">

//var Ext_errMsg_For_Get_Store_Title = "<%=WebKeys.Ext_errMsg_For_Get_Store_Title %>";
//var Ext_errMsg_For_Get_Store_Message = "<%=WebKeys.Ext_errMsg_For_Get_Store_Message %>";

var ContractTypeID = "<%=DBSchema.Contract.ContractTypeID %>";
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
var ContractAmtCount = "<%=DBSchema.Contract.ContractAmtCount %>";
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
var CurrencyID = "<%=DBSchema.Contract.CurrencyID %>";
var TotalAmtByID = "<%=DBSchema.ChargedDetail.TotalAmtByID %>";
var UnChargedAmtTotal = "<%=DBSchema.ChargedDetail.UnChargedAmtTotal %>";
var WarrantyEndDate = "<%=DBSchema.Contract.WarrantyEndDate %>";

var invoiceTotalAmt = "<%=DBSchema.InvoiceDetail.TotalAmtByID %>";
var invoiceUnAccountAmt = "<%=DBSchema.InvoiceDetail.UnAccountAmtTotalSum %>";

var contractInvoiceAmtInTimeScope = "contract_invoice_amt";
var contractInvoiceCountInTimeScope = "contract_invoice_count";

var contractPayedAmtInTimeScope = "contract_payed_amt";
var contractPayedCountInTimeScope = "contract_payed_count";

var contractEstimateAmtInTimeScope = "contract_estimate_amt";
var contractEstimateCountInTimeScope = "contract_estimate_count";


var warningDate = "warning_date";
var warningAmt = "warning_amt";
var warningDays = "warning_days";

var warningCount = "warning_count";

var Type_ContractType_Income = "<%=WebKeys.Type_ContractType_Income %>";
var Type_ContractType_Payout = "<%=WebKeys.Type_ContractType_Payout %>";

var Type_Contract_Integration_Simple = "<%=WebKeys.Type_Contract_Integration_Simple %>";
var Type_Contract_Integration_New = "<%=WebKeys.Type_Contract_Integration_New %>";
var Type_Contract_Integration_Performed = "<%=WebKeys.Type_Contract_Integration_Performed %>";
var Type_Contract_Integration_EstimateContractAmt = "<%=WebKeys.Type_Contract_Integration_EstimateContractAmt %>";
var Type_Contract_Integration_Warranty = "<%=WebKeys.Type_Contract_Integration_Warranty %>";

var Type_subIntegration_Nomal = "<%=WebKeys.Type_subIntegration_Nomal %>";
var Type_subIntegration_Payed = "<%=WebKeys.Type_subIntegration_Payed %>";
var Type_subIntegration_Invoiced = "<%=WebKeys.Type_subIntegration_Invoiced %>";
var Type_subIntegration_Estimate = "<%=WebKeys.Type_subIntegration_Estimate %>";

</script>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">
    
    <title>合同列表</title>
    
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">

	<%if(WebKeys.Type_Search_For_ContractList_Normal.equals(request.getAttribute(WebKeys.Request_Attribute_Name_SearchType_For_ContractList))){ %>
	<script language="JavaScript" src="/xMIS/contract/contractList.js"></script>
	<%}else if(WebKeys.Type_Search_For_ContractList_Integration.equals(request.getAttribute(WebKeys.Request_Attribute_Name_SearchType_For_ContractList))){ %>
	<script language="JavaScript" src="/xMIS/contract/contractIntegrationList.js"></script>
	<script language="JavaScript" src="/xMIS/contract/contractInfoShow.js"></script>	
	<%} %>
  </head>
  
  <body>
  	<div id="contractList" style="width:98.5%"></div> 
  	<br>
  	<SPAN id=sumTitle></SPAN>
  	<br>
  	<SPAN id=sumMsg></SPAN>
  	
  	<a name="end" ><br></a>
  </body>
</html>




    
	<jsp:include page="<%=WebKeys.Include_JSP_File_paymentList %>" />
	<jsp:include page="<%=WebKeys.Include_JSP_File_InvoiceList %>" />
	
	
	
	
	
	
	
	

