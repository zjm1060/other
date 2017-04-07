<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<%@ page import="com.bitc.jk.xmis.util.DBSchema"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>

<script language="javascript">

var invoiceInvoiceDate = "<%=DBSchema.InvoiceDetail.InvoiceDate %>";
var invoiceAEmpName = "<%=DBSchema.Employee.aEmpName %>";
var invoiceInvoiceAmt = "<%=DBSchema.InvoiceDetail.InvoiceAmt %>";
var invoiceBEmpName = "<%=DBSchema.Employee.bEmpName  %>";
var invoiceUpdateDate = "<%=DBSchema.InvoiceDetail.UpdateDate %>";
var invoiceCurrencyGraph = "<%=DBSchema.CurrencyType.CurrenyGraph %>";
var invoiceCurrencyExchangeRate = "<%=DBSchema.CurrencyType.ExchangeRate %>";
var invoiceRemark = "<%=DBSchema.InvoiceDetail.Remark %>";

var invoiceListType = <%=WebKeys.Type_Total_Sum_Invoiced %>;

function showInvoiceList(contractAbb, contractID){
	showInvoiceListWindow(contractAbb, contractID, invoiceListType);
}


</script>


<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <title>My JSP 'paymentList.jsp' starting page</title>
    
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	
	<script language="JavaScript" src="/xMIS/contract/invoiceList.js"></script>

  </head>
</html>

