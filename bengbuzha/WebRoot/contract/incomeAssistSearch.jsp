<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<!DOCTYPE HTML>
<%@ page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<%@include file="../jsCss.jsp" %>

<%
String basePath = request.getContextPath();
EmployeeObj emp = (EmployeeObj) request.getSession().getAttribute(
		WebKeys.Session_Attribute_Name_UserDNA);
%>

<html>
  <head>
  	<script type="text/javascript">
  	var empName = '<%=emp.getUserName()%>';
  	
	function turnToEditContract(contractID, contractName, contractType) {
	var mid = (contractType && contractType == 2)
			? 'contract-edit-out'
			: 'contract-edit';
	parent.xMis.turnToMenu(
		mid, 'contractMain.do?method=contract&contractID=' + contractID,
		contractName);
}
	var contractID = '合同ID';
	var projectNo = '项目编码';
	var contractShortName = '合同名称';
	var invoiceAmt = '发票金额';
	var antipateAmt = '预计付款金额';
	var registrationTime = '预计付款时间';
	var remark = '备注';
	var payerAccounts = '付款人帐号';
	var companyID = '公司ID';
	var companyShortName = '公司名称';
	var isFlag = '是否已付款';
	var contractMsg = 'contractMsg';
	var createTime = '开票日期';
	var invoiceNumber = '发票号';
	var ID = 'ID';
  	</script>
  </head>
  <body>
  	<script type="text/javascript" src="<%=path%>/resources/js/ChooseOrgTreeField.js"></script>
  	<script type="text/javascript" src="<%=path%>/resources/js/CheckColumn.js"></script>
  	<script type="text/javascript" src="<%=path %>/resources/js/SearchField.js"></script>
  	<script type="text/javascript" src="<%=path %>/resources/js/InklingSearchField.js"></script>
    <script type="text/javascript" src="<%=path %>/contract/incomeAssistSearchPanel.js"></script>
	<script type="text/javascript" src="<%=path %>/contract/incomeAssistSearch.js"></script>
  </body>
</html>
