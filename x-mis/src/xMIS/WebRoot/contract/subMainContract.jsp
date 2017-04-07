<%@ page contentType="text/html; charset=utf-8"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>

<html>
  <head>
	<link rel="stylesheet" type="text/css" href="/xMIS/resources/css/ext-all.css" />
    <link rel="stylesheet" type="text/css" href="/xMIS/resources/css/RowEditor.css" />
    <link rel="stylesheet" type="text/css" href="/xMIS/resources/css/ext-custom.css" />
    <link rel="stylesheet" type="text/css" href="/xMIS/resources/css/grid-examples.css" />
    
    
	<LINK href="/xMIS/resources/css/styleCIS.css" type=text/css	rel=stylesheet>
    <link href="/xMIS/resources/css/calendarStyle.css" type=text/css rel=stylesheet></link>

		<script type="text/javascript" src="/xMIS/resources/js/ext-base.js"></script>
		<script type="text/javascript" src="/xMIS/resources/js/ext-all.js"></script>
	  	<script type="text/javascript" src="/xMIS/resources/js/ext-lang-zh_CN.js"></script>    
	  	

		<script type="text/javascript" src="/xMIS/resources/js/RowEditor.js"></script>
		<script type="text/javascript" src="/xMIS/resources/js/CheckColumn.js"></script>
		<script type="text/javascript" src="/xMIS/resources/js/SearchField.js"></script>

	<script type="text/javascript" src="/xMIS/resources/js/common.js"></script>
	<script language="JavaScript" src="/xMIS/resources/js/prototype.js"></script>
	<script language="JavaScript" src="/xMIS/resources/js/bitc.js"></script>
  </head>
  <body>    
</body>
</html>

<script language="javascript">	
	var Ext_errMsg_For_Get_Store_Title = "<%=WebKeys.Ext_errMsg_For_Get_Store_Title %>";
	var	Ext_errMsg_For_Get_Store_Message = "<%=WebKeys.Ext_errMsg_For_Get_Store_Message %>";

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

	System.out.println("operationPart.jsp -> subType = " + subType);
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



	
