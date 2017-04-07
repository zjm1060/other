<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<%@ page import="com.bitc.jk.xmis.model.ContractObj"%>
<%@ page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<!DOCTYPE HTML>
<%
ContractObj con = (ContractObj)request.getAttribute(WebKeys.Request_Attribute_Name_ContractObject);
EmployeeObj emp = (EmployeeObj)request.getSession().getAttribute(WebKeys.Session_Attribute_Name_UserDNA);


String currencyGraph = "";

				int mapsize = con.getCurrencyTypeMap().size();
				Iterator it = con.getCurrencyTypeMap().entrySet().iterator();
				for (int j = 0; j < mapsize; j++) {
					Map.Entry entry = (Map.Entry) it.next();
					currencyGraph = String.valueOf(con.getCurrencyCodeMap().get(entry.getKey()));
					if (((Integer)entry.getKey()).intValue() == con.getCurrencyID())
					break;
				}
				
System.out.println("========= "+currencyGraph);				
%>

<script type="text/javascript">

	var Ext_errMsg_For_Get_Store_Title = "<%=WebKeys.Ext_errMsg_For_Get_Store_Title %>";
	var	Ext_errMsg_For_Get_Store_Message = "<%=WebKeys.Ext_errMsg_For_Get_Store_Message %>";
	var paymentCondiFlag = 0;
	
	var contractID = "<%=con.getContractID() %>";
	var contractTypeID = "<%=con.getContractTypeID() %>";
	var abb = "<%=con.getAbbreviation() %>";
	var currencyGraph = "<%=currencyGraph %>";
	
	var Type_ContractType_Income = "<%=WebKeys.Type_ContractType_Income %>";
	var Type_ContractType_Payout = "<%=WebKeys.Type_ContractType_Payout %>";

</script>

<html>
  <head>
    <title>My JSP 'paymentConditionDetailSetting.jsp' starting page</title>
    
  </head>

<% if(con.getContractTypeID() == WebKeys.Type_ContractType_Income){ %>
<jsp:include page="<%=WebKeys.Include_JSP_File_paymentCondition %>">
		<jsp:param name="collapsible" value="false"/>
		<jsp:param name="collapsed" value="false"/>
</jsp:include>
<%} %>

<jsp:include page="<%=WebKeys.Include_JSP_File_paymentEdit %>" />

  <body>
  <div id="paymentOptionDiv" style="width:100%"></div> 
  </body>
</html>
<%
	String path = request.getContextPath();
	if("/".equals(path)){
		path="";
	}
%>

	<script language="JavaScript" src="<%=path %>/contract/paymentOption.js"></script>

