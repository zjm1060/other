<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<%@ page import="com.bitc.jk.xmis.util.DBSchema"%>
<%@ page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<!DOCTYPE HTML>
<%
String path = request.getContextPath();
if("/".equals(path)){
	path="";
}
String deptLimits=(String)request.getSession().getAttribute("searchProfessionLimits");
EmployeeObj emp = (EmployeeObj)request.getSession().getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
boolean readOnly = emp.isReadOnly();
boolean decisionFlag = emp.hasDecisionRole();
%>
<html>
  <head>
    <title>合同列表</title>
    <script type="text/javascript">
var readOnly = <%=readOnly %>;
var decisionFlag = <%=decisionFlag %>;
var deptLimitsFlag = ("<%=deptLimits %>" == 'PASS') ? true : false;
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
var VendorAbb = "bCompany_name";
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
var ProjectNo = "<%=DBSchema.Project.ProjectCode %>";
var ProjectID = "<%=DBSchema.Project.ProjectID %>";
var ProjectCode = "<%=DBSchema.Project.ProjectCode %>";
var ProjectName = "<%=DBSchema.Project.ProjectName %>";
var CurrenyGraph = "<%=DBSchema.CurrencyType.CurrenyGraph %>";
var CurrencyID = "<%=DBSchema.Contract.CurrencyID %>";
var TotalAmtByID = "<%=DBSchema.ChargedDetail.TotalAmtByID %>";
var UnChargedAmtTotal = "<%=DBSchema.ChargedDetail.UnChargedAmtTotal %>";
var WarrantyEndDate = "<%=DBSchema.Contract.WarrantyEndDate %>";

var invoiceTotalAmt = "<%=DBSchema.InvoiceDetail.TotalAmtByID %>";
var invoiceUnAccountAmt = "<%=DBSchema.InvoiceDetail.UnAccountAmtTotalSum %>";

var receiptTotalAmt = "<%=DBSchema.Receipt.TotalAmtByID %>";

//付款推迟
var delayReason = "<%=DBSchema.DelayReason.Reason %>";
var delayOperatorName = "<%=DBSchema.DelayReason.OperatorName %>";
var delayDateOfRegistration = "<%=DBSchema.DelayReason.DateOfRegistration %>";
var delayRegistrantName = "<%=DBSchema.DelayReason.RegistrantName %>";

var contractInvoiceAmtInTimeScope = "contract_invoice_amt";
var contractInvoiceCountInTimeScope = "contract_invoice_count";

var contractPayedAmtInTimeScope = "contract_payed_amt";
var contractPayedCountInTimeScope = "contract_payed_count";

var contractEstimateAmtInTimeScope = "contract_estimate_amt";
var contractEstimateCountInTimeScope = "contract_estimate_count";

var incomAmtTotalByGl = "MB";
var elseIncomeAmtTotal = "elseMB";
var purAmtTotalByGl = "pay_amt";
var unIncomAmtTotalByGl = "unMB";
var officialReceiptsMB = "officialReceiptsMB"   //财务实收金额
var officialReceiptsPayAmt = "officialReceiptsPayAmt"   //财务实际支出金额
var PaymentIndvanceAmtTotalSum  = "<%=DBSchema.InvoiceDetail.PaymentIndvanceAmtTotalSum %>";


var warningDate = "warning_date";
var warningAmt = "warning_amt";
var warningBjamt = "warning_bjamt";
var warningDays = "warning_days";

var warningCount = "warning_count";

var alreadyMakeOutInvoiceOfNotBaseOnPlan = "按进度已开票未到款";
var didNotMakeOutInvoiceReceivableBaseOnPlan = "按进度未开票应收款";

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
    <script type="text/javascript" src="<%=path %>/resources/js/ChooseOrgTreeField.js"></script>
	<script type="text/javascript" src="<%=path %>/contract/delayReasonGridPanel.js"></script>
	<script type="text/javascript" src="<%=path %>/resources/js/TotalPagingToolbar.js"></script>
	<script type="text/javascript" src="<%=path %>/contract/ContractGridPanel.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/upload/swfupload.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/upload/uploaderPanel.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/js/NavButtonPanel.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/js/Ext.ux.grid.RowActions.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/js/Attachment.js"></script>
	<script type="text/javascript" src="<%=path%>/project/ProjectSchedule.js"></script>
	<script type="text/javascript" src="<%=path%>/project/estimatedPaymentPanel.js"></script>
	<script type="text/javascript" src="<%=path %>/contract/contractList.js"></script>
  </head>
  <body>
  </body>
</html>
<jsp:include page="<%=WebKeys.Include_JSP_File_paymentList %>" />
<jsp:include page="<%=WebKeys.Include_JSP_File_InvoiceList %>" />
	
<jsp:include page="<%=WebKeys.Include_JSP_File_glIncomList %>" />
<jsp:include page="<%=WebKeys.Include_JSP_File_glPurchaseList %>" />
<jsp:include page="<%=WebKeys.Include_JSP_File_glElseIncomeAmtList %>" />	
	
	
	
	
	
	

