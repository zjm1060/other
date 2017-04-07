<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ page import="com.bitc.jk.xmis.util.DBSchema"%>
<%@ page import="com.bitc.jk.xmis.model.ContractObj"%>
<%@ page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>

<%
ContractObj con = (ContractObj)request.getAttribute(WebKeys.Request_Attribute_Name_ContractObject);
EmployeeObj emp = (EmployeeObj)request.getSession().getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

String paymentConditionDivName = "paymentConditionDiv_" + con.getContractID();

%>

   <script language="javascript">
   
	var paymentConditionDivName = "<%=paymentConditionDivName %>"
	
    var paymentConditionId = "<%=DBSchema.PaymentCondition.PaymentConditionID %>";
    var paymentConditionDescription ="<%=DBSchema.PaymentCondition.PaymentConditionDescription%>";
    var paymentConditionProjStatusID = "<%=DBSchema.PaymentCondition.ProjectStatusID%>";
	var paymentConditionProjStatusName = "<%=DBSchema.PaymentCondition.ProjectStatusName%>"; 
	var paymentConditionDelayDays ="<%=DBSchema.PaymentCondition.DelayDays%>";
	var paymentConditionPaymentDate ="<%=DBSchema.PaymentCondition.PaymentDate%>";
	var paymentConditionDateDisplayed ="<%=DBSchema.PaymentCondition.PaymentDateDisplayed%>";
	var paymentConditionPercent ="<%=DBSchema.PaymentCondition.PaymentPercent%>";
	var paymentConditionPaymentAmt ="<%=DBSchema.PaymentCondition.PaymentAmt%>";
	var paymentConditionManualFlag ="<%=DBSchema.PaymentCondition.ManualFlag%>";
	var paymentConditionManualReason ="<%=DBSchema.PaymentCondition.ManualReason%>"
	
	//以下为项目状态。
	var ProjectStatusID ="<%=DBSchema.ProjectStatus.ProjectStatusID%>";
	var ProjectStatusName ="<%=DBSchema.ProjectStatus.ProjectStatusName%>";  
    var ProjectStatusDescription ="<%=DBSchema.ProjectStatus.ProjectStatusDescription%>";
    var SelfDefinitionFlag ="<%=DBSchema.ProjectStatus.SelfDefinitionFlag%>";

     
    var contractIdInPay = "<%=con.getContractID() %>";
     
     setContractIdInPy = function(id){
     	contractIdInPay =id; 
     };     
    var contractAmtInPay = "<%=con.getContractAmt() %>";
    var collapsible=<%=(request.getParameter("collapsible")!=null && request.getParameter("collapsible").equals("false"))?"false":"true"%>;
    var collapsed=<%=(request.getParameter("collapsed")!=null && request.getParameter("collapsed").equals("false"))?"false":"true"%>;

     setContractAmtInPay = function(amt){
     	contractAmtInPay = amt;
     
     }; 
     function setPayConditionBtn(){
     	  	setTbarDisable();
	  		
     };
     function setCollapse(collapsibleVal, collapsedVal){
     	collapsible = collapsibleVal;
     	collapsed = collapsedVal;
     };
     
    </script>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    
    <title>My JSP 'paymentCondition.jsp' starting page</title>
  </head>
  
  <body>
  <div id=<%=paymentConditionDivName %> width=100%></div>

  </body>
</html>

<script language="JavaScript" src="/xMIS/contract/paymentCondition.js"></script>

<script type="text/javascript">



   
</script>
