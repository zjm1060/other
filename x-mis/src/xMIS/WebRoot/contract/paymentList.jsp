<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<%@ page import="com.bitc.jk.xmis.util.DBSchema"%>


<script language="javascript">



//var contractID4PaymentList = "<%=request.getParameter("contractID4PaymentList") %>";


var paymentChargedDate = "<%=DBSchema.ChargedDetail.ChargedDate %>";
var paymentAEmpName = "<%=DBSchema.Employee.aEmpName %>";
var paymentChargedAmt = "<%=DBSchema.ChargedDetail.ChargedAmt %>";
var paymentBEmpName = "<%=DBSchema.Employee.bEmpName  %>";
var paymentUpdateDate = "<%=DBSchema.ChargedDetail.UpdateDate %>";
var paymentCurrencyGraph = "<%=DBSchema.CurrencyType.CurrenyGraph %>";
var paymentCurrencyExchangeRate = "<%=DBSchema.CurrencyType.ExchangeRate %>";
var paymentChargedRemark = "<%=DBSchema.ChargedDetail.ChargedRemark %>";



function showPaymentList(contractAbb, contractID){
	showPaymentListWindow(contractAbb, contractID);
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
	
	<script language="JavaScript" src="/xMIS/contract/paymentList.js"></script>

  </head>
</html>
