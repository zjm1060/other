<%@ page language="java" import="java.util.*" pageEncoding="UTF-8"%>
<%@ page import="com.bitc.jk.xmis.util.DBSchema"%>
<%@ page import="com.bitc.jk.xmis.model.ContractObj"%>
<%@ page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<%@ page import="com.bitc.jk.xmis.util.Type"%>
<!DOCTYPE HTML>
<%
String path=request.getContextPath();
ContractObj con = (ContractObj)request.getAttribute(WebKeys.Request_Attribute_Name_ContractObject);
EmployeeObj emp = (EmployeeObj)request.getSession().getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
if(con==null){
	con=new ContractObj();
	con.setContractID(Type.GetInt(request.getParameter("contractID")));
	con.setContractAmt(Type.GetDouble(request.getParameter("contractAmt")));
}
String paymentConditionDivName = "paymentConditionDiv_" + con.getContractID();
boolean readOnly = emp.isReadOnly();
String gls = request.getParameter("gls");
%>

   <script language="javascript">
    var readOnly = <%=readOnly%>; 
	var paymentConditionDivName = "<%=paymentConditionDivName %>";
	var gls = "<%=gls%>";
	
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
	var paymentConditionManualReason ="<%=DBSchema.PaymentCondition.ManualReason%>";
	var paymentConditionPaymentNodeName = "<%=DBSchema.PaymentCondition.PaymentNodeName%>";
	var paymentConditionShouldPaymentDate = "<%=DBSchema.PaymentCondition.ShouldPaymentDate%>";
	var realStartDate = '实际开始日期';
	var realIncome = 'realIncome';
	var realDate = 'realDate';
	var cID = '合同ID';
	
	//以下为项目状态。
	var ProjectStatusID ="id";
	var ProjectStatusName ="name";  
    var ProjectStatusDescription ="description";
    var SelfDefinitionFlag ="custom";

     
    var contractIdInPay = "<%=con.getContractID() %>";
    var projectId='<%=request.getParameter("projectId")%>';
     
     setContractIdInPy = function(id){
     	contractIdInPay =id; 
     };
    
    var paymentCondiFlag = typeof(paymentFlag) == "undefined"?0:paymentFlag; 
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

<html>
  <head>
    
    <title>My JSP 'paymentCondition.jsp' starting page</title>
    <script language="JavaScript" src="<%=path%>/contract/paymentCondition.js"></script>
    <script type="text/javascript" src="<%=path%>/resources/js/Ext.ux.grid.RowActions.js"></script>
	<script type="text/javascript" src="<%=path%>/resources/js/Attachment.js"></script>
  </head>
  <body>
  <div id="<%=paymentConditionDivName %>" style="width:100%;height:100%"></div>
  </body>
</html>


