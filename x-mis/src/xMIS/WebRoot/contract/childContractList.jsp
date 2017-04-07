<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<%@ page import="com.bitc.jk.xmis.util.DBSchema"%>
<%@ page import="com.bitc.jk.xmis.model.ContractObj"%>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">

	<script type="text/javascript" src="/xMIS/contract/contractInfoShow.js"></script>
	<script type="text/javascript" src="/xMIS/contract/childContractList.js"></script>

  </head>
  
  <body>
    <div id="childContractList" style="width:100%"></div> 
  </body>
</html>

<script language="javascript">
var parentID = "<%=((ContractObj)request.getAttribute(WebKeys.Request_Attribute_Name_ContractObject)).getContractID() %>";

var childContractID = "<%=DBSchema.Contract.ContractID %>";
var childContractNo = "<%=DBSchema.Contract.ContractNo %>";
var childContractName = "<%=DBSchema.Contract.ContractName %>";
var childAbbreviation = "<%=DBSchema.Contract.Abbreviation %>";
var childSignatureDate = "<%=DBSchema.Contract.SignatureDate %>";
//var childParentID = "<%=DBSchema.Contract.ParentID %>";
var childContractAmt = "<%=DBSchema.Contract.ContractAmt %>";
var childCurrenyGraph = "<%=DBSchema.CurrencyType.CurrenyGraph %>";
var childExchangeRate = "<%=DBSchema.CurrencyType.ExchangeRate %>";
var childTotalAmtByID = "<%=DBSchema.ChargedDetail.TotalAmtByID %>";
var childUnChargedAmtTotal = "<%=DBSchema.ChargedDetail.UnChargedAmtTotal %>";


var childInvoiceTotalAmt = "<%=DBSchema.InvoiceDetail.TotalAmtByID %>";
var childInvoiceUnAccountAmt = "<%=DBSchema.InvoiceDetail.UnAccountAmtTotalSum %>";


</script>