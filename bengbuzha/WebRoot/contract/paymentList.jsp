<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<%@ page import="com.bitc.jk.xmis.util.DBSchema"%>
<!DOCTYPE HTML>

<script language="javascript">



var paymentChargedId = "<%=DBSchema.ChargedDetail.ChargedID %>";
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
<html>
  <head>
    <title>My JSP 'paymentList.jsp' starting page</title>
    
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	
	<style type="text/css">
		.fake_date_Red td{ background : #FFFF8E }
	</style>
	<%
	String path = request.getContextPath();
	if("/".equals(path)){
		path="";
	}
%>
	<script language="JavaScript" src="<%=path %>/contract/paymentList.js"></script>

  </head>
</html>
