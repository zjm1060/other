<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<%@ page import="com.bitc.jk.xmis.util.DBSchema"%>
<%@ page import="com.bitc.jk.xmis.model.ContractObj"%>
<!DOCTYPE HTML>
<%
	String path = request.getContextPath();
%>
<html>
<head>
<script type="text/javascript" src="<%=path %>/resources/js/WebKeys.jsp"></script>
<script type="text/javascript" src="<%=path %>/contract/contractInfoShow.js"></script>
<script type="text/javascript" src="<%=path %>/contract/childContractList.js"></script>

</head>

<body>
<div id="childContractList" style="width:100%;height:100%;"></div>
</body>
</html>

<script language="javascript">
var parentID = "<%=((ContractObj) request
					.getAttribute(WebKeys.Request_Attribute_Name_ContractObject))
					.getContractID()%>";
var childContractID = "<%=DBSchema.Contract.ContractID%>";
var childContractNo = "<%=DBSchema.Contract.ContractNo%>";
var childContractName = "<%=DBSchema.Contract.ContractName%>";
var childAbbreviation = "<%=DBSchema.Contract.Abbreviation%>";
var childSignatureDate = "<%=DBSchema.Contract.SignatureDate%>";
//var childParentID = "<%=DBSchema.Contract.ParentID%>";
var childContractAmt = "<%=DBSchema.Contract.ContractAmt%>";
var childCurrenyGraph = "<%=DBSchema.CurrencyType.CurrenyGraph%>";
var childExchangeRate = "<%=DBSchema.CurrencyType.ExchangeRate%>";
var childTotalAmtByID = "<%=DBSchema.ChargedDetail.TotalAmtByID%>";
var childUnChargedAmtTotal = "<%=DBSchema.ChargedDetail.UnChargedAmtTotal%>";


var childInvoiceTotalAmt = "<%=DBSchema.InvoiceDetail.TotalAmtByID%>";
var childInvoiceUnAccountAmt = "<%=DBSchema.InvoiceDetail.UnAccountAmtTotalSum%>";

var childIncomAmtTotalByGl = "MB";
var childPurAmtTotalByGl = "pay_amt";
var unIncomAmtTotalByGl = "unMB";

</script>