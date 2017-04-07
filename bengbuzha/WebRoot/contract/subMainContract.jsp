<%@ page contentType="text/html; charset=utf-8"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<html>
  <head>
  <%@include file="../jsCss.jsp" %>
		<script type="text/javascript" src="<%=path %>/resources/js/RowEditor.js"></script>
		<script type="text/javascript" src="<%=path %>/resources/js/CheckColumn.js"></script>
		<script type="text/javascript" src="<%=path %>/resources/js/SearchField.js"></script>
		<script type="text/javascript" src="<%=path %>/resources/js/ChooseField.js"></script>
		<script type="text/javascript" src="<%=path %>/resources/js/MultiSearchPanel.js"></script>
		<script type="text/javascript" src="<%=path %>/resources/js/InklingSearchField.js"></script>

	<script type="text/javascript" src="<%=path %>/resources/js/common.js"></script>
	<script type="text/javascript" src="<%=path %>/resources/js/prototype.js"></script>
	<script type="text/javascript" src="<%=path %>/resources/js/bitc.js"></script>
  </head>
  <body>    
</body>
</html>

<script type="text/javascript">	
	var Ext_errMsg_For_Get_Store_Title = "<%=WebKeys.Ext_errMsg_For_Get_Store_Title %>";
	var	Ext_errMsg_For_Get_Store_Message = "<%=WebKeys.Ext_errMsg_For_Get_Store_Message %>";
	var	Ext_userValidateMsg_Message = "<%=WebKeys.Ext_userValidateMsg_Message %>";

    var Color_Font_Contract_Total_Amt = "<%=WebKeys.Color_Font_Contract_Total_Amt %>";
    var Color_Font_Contract_Total_Charged_Amt =  "<%=WebKeys.Color_Font_Contract_Total_Charged_Amt %>";
    var Color_Font_Contract_Total_UnCharged_Amt =  "<%=WebKeys.Color_Font_Contract_Total_UnCharged_Amt %>";
    var Color_Font_Contract_Total_Invoiced_Amt =  "<%=WebKeys.Color_Font_Contract_Total_Invoiced_Amt %>";
    var Color_Font_Contract_Total_UnAccount_Amt =  "<%=WebKeys.Color_Font_Contract_Total_UnAccount_Amt %>";
    
    var Color_BG_Warning_Earlier_30_Days =  "<%=WebKeys.Color_BG_Warning_Earlier_30_Days %>";
    var Color_BG_Warning_Earlier_15_Days =  "<%=WebKeys.Color_BG_Warning_Earlier_15_Days %>";
    var Color_BG_Warning_Earlier_7_Days =  "<%=WebKeys.Color_BG_Warning_Earlier_7_Days %>";
    var Color_BG_Warning_Current_Date =  "<%=WebKeys.Color_BG_Warning_Current_Date %>";
    var Color_BG_Warning_Overtime_Current_Date =  "<%=WebKeys.Color_BG_Warning_Overtime_Current_Date %>";
    var Color_BG_Warning_Overtime_More_7_Days =  "<%=WebKeys.Color_BG_Warning_Overtime_More_7_Days %>";
    var Color_BG_Warning_Overtime_More_15_Days =  "<%=WebKeys.Color_BG_Warning_Overtime_More_15_Days %>";
    var Color_BG_Warning_Overtime_More_30_Days =  "<%=WebKeys.Color_BG_Warning_Overtime_More_30_Days %>";
    var Color_BG_Warning_Without_Definition =  "<%=WebKeys.Color_BG_Warning_Without_Definition %>";

    
    var Type_Warning_Earlier_30_Days = "<%=WebKeys.Type_Warning_Earlier_30_Days %>";
    var Type_Warning_Earlier_15_Days = "<%=WebKeys.Type_Warning_Earlier_15_Days %>";
    var Type_Warning_Earlier_7_Days = "<%=WebKeys.Type_Warning_Earlier_7_Days %>";
    var Type_Warning_Current_Date = "<%=WebKeys.Type_Warning_Current_Date %>";
    var Type_Warning_Overtime_Current_Date = "<%=WebKeys.Type_Warning_Overtime_Current_Date %>";
    var Type_Warning_Overtime_More_7_Days = "<%=WebKeys.Type_Warning_Overtime_More_7_Days %>";
    var Type_Warning_Overtime_More_15_Days = "<%=WebKeys.Type_Warning_Overtime_More_15_Days %>";
    var Type_Warning_Overtime_More_30_Days = "<%=WebKeys.Type_Warning_Overtime_More_30_Days %>";

    var Type_Warning_Info = "<%=WebKeys.Type_Warning_Info %>";
    var Type_Warning_Without_Definition = "<%=WebKeys.Type_Warning_Without_Definition %>";

    
    var Tip_Title_Warning_Earlier_30_Days = "<%=WebKeys.Tip_Title_Warning_Earlier_30_Days %>";
    var Tip_Title_Warning_Earlier_15_Days = "<%=WebKeys.Tip_Title_Warning_Earlier_15_Days %>";
    var Tip_Title_Warning_Earlier_7_Days = "<%=WebKeys.Tip_Title_Warning_Earlier_7_Days %>";
    var Tip_Title_Warning_Current_Date = "<%=WebKeys.Tip_Title_Warning_Current_Date %>";
    var Tip_Title_Warning_Overtime_Current_Date = "<%=WebKeys.Tip_Title_Warning_Overtime_Current_Date %>";
    var Tip_Title_Warning_Overtime_More_7_Days = "<%=WebKeys.Tip_Title_Warning_Overtime_More_7_Days %>";
    var Tip_Title_Warning_Overtime_More_15_Days = "<%=WebKeys.Tip_Title_Warning_Overtime_More_15_Days %>";
    var Tip_Title_Warning_Overtime_More_30_Days = "<%=WebKeys.Tip_Title_Warning_Overtime_More_30_Days %>";
    
	var Tip_Title_Warning_Info = "<%=WebKeys.Tip_Title_Warning_Info %>";
	var Tip_Title_Warning_Without_Definition = "<%=WebKeys.Tip_Title_Warning_Without_Definition %>";
	
     
</script>
<%
	String subType = request
	.getParameter(WebKeys.Request_Attribute_Name_SubType) == null ? (String) request
	.getAttribute(WebKeys.Request_Attribute_Name_SubType)
	: request
	.getParameter(WebKeys.Request_Attribute_Name_SubType);

	System.out.println("subMainContract.jsp -> subType = " + subType);
%>
	<%
	if (subType.equals(WebKeys.Request_Attribute_Name_SubType_contractList)) {
	%>
	<jsp:include flush="true" page="<%=WebKeys.Include_JSP_File_contractList %>"></jsp:include>
	<%
	}else if (subType.equals(WebKeys.Request_Attribute_Name_SubType_contract)) {
	%>
	<jsp:include flush="true" page="<%=WebKeys.Include_JSP_File_contract %>"></jsp:include>
	<%
	}else if (subType.equals(WebKeys.Request_Attribute_Name_SubType_paymentListEdit)) {
	%>
	<jsp:include flush="true" page="<%=WebKeys.Include_JSP_File_paymentListEdit %>"></jsp:include>
	<%
	}else if (subType.equals(WebKeys.Request_Attribute_Name_SubType_invoiceEdit)) {
	%>
	<jsp:include flush="true" page="<%=WebKeys.Include_JSP_File_invoiceEdit %>"></jsp:include>
	<%}%>



	
