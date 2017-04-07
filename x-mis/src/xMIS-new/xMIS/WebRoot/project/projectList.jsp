<%@page import="net.sf.json.JsonConfig"%>
<%@page import="net.sf.json.JSONSerializer"%>
<%@page contentType="text/html; charset=utf-8"%>
<%@page import="java.lang.*,java.util.*,java.text.*"%>
<%@page import="com.bitc.jk.xmis.model.ProjectObj"%>
<%@ page import="com.bitc.jk.xmis.util.DBSchema"%>
<!DOCTYPE HTML>
<html>
  <head>

    <%@include file="../jsCss.jsp" %>
<script language="javascript">

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
var purAmtTotalByGl = "pay_amt";
var unIncomAmtTotalByGl = "unMB";
var officialReceiptsMB = "officialReceiptsMB"   //财务实收金额
var officialReceiptsPayAmt = "officialReceiptsPayAmt"   //财务实际支出金额


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
    <script type="text/javascript" src="<%=path %>/resources/js/SearchField.js"></script>
    <script type="text/javascript" src="<%=path %>/resources/js/InklingSearchField.js"></script>
    <script type="text/javascript" src="<%=path %>/resources/js/MultiSearchPanel.js"></script>
    <link rel="stylesheet" type="text/css" href="<%=path%>/project/projectInfo.css" />
    <style type="text/css">
      body .x-panel { margin-bottom: 0px; }
    </style>
    <script type="text/javascript" src="<%=path%>/project/projectList.js">
    </script>
    <title>
      北京中水科技 X-MIS
    </title>
  </head>
  <body>
  </body>
</html>

