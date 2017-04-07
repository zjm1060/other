<%@ page language="java" import="java.util.*" pageEncoding="utf-8"%>
<%@ page import="com.bitc.jk.xmis.model.ContractObj"%>
<%@ page import="com.bitc.jk.xmis.model.EmployeeObj"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<%@ page import="com.bitc.jk.xmis.util.DBSchema"%>

<%
ContractObj con = (ContractObj)request.getAttribute(WebKeys.Request_Attribute_Name_ContractObject);
EmployeeObj emp = (EmployeeObj)request.getSession().getAttribute(WebKeys.Session_Attribute_Name_UserDNA);

  String contractInfoDivName = "contractInfo_" + con.getContractID();
String contractInfoOtherDivName = "contractInfOther_" + con.getContractID();
String ContractAttd ="ContractAttd_"+con.getContractID();

int mapsize;

String val="";
  mapsize = con.getCurrencyTypeMap().size();
	Iterator it = con.getCurrencyTypeMap().entrySet().iterator();

	for (int j = 0; j < mapsize; j++) {
	   Map.Entry  entry = (Map.Entry) it.next();
		
		if(((Integer)entry.getKey()).intValue() == con.getCurrencyID())
			val = String.valueOf(entry.getValue());
		
		}
%>

<script language="javascript">

   // for download grid 
   
   var paymentCondiFlag=1;
   
    var  AttachmentID ='<%=DBSchema.Attachment.AttachmentID %>';
    var  AttachmentName='<%=DBSchema.Attachment.AttachmentName %>';
    
    var contractIdInfo ='<%=con.getContractID()%>';
	var contractInfoDivName = "<%=contractInfoDivName %>";
	var contractInfoOtherDivName = "<%=contractInfoOtherDivName %>";
	var ContractAttd ="<%=ContractAttd%>";
	
	//质保期
	var warrantyStartDate = "<%=con.getWarrantyStartDate()==null?"":con.getWarrantyStartDate().toString().trim().substring(0,10) %>"; //质保期起
    var WarrantyEndDate = "<%=con.getWarrantyEndDate()==null?"":con.getWarrantyEndDate() %>";
    //质保期单位
    var warrantyUnitName;
    var warrantyUnit ="<%=con.getWarrantyUnit()%>";

	switch(warrantyUnit){
  		case "<%=WebKeys.Contract_Obj_Warranty_Unit_Def_DAY %>":
  			warrantyUnitName = "日";
  			break;
  		case "<%=WebKeys.Contract_Obj_Warranty_Unit_Def_MONTH %>":
  			warrantyUnitName = "月";
  			break;
  		case "<%=WebKeys.Contract_Obj_Warranty_Unit_Def_YEAR %>":
  			warrantyUnitName = "年";
  			break;
  		default:
  			warrantyUnitName = "日";
  			break;
   	}

    
	var contractName = "<%=con.getContractName()%>";
	var contractAbbreviation ="<%=con.getAbbreviation()%>";
	var contractArrearageTotalAmt ;// ="<%=con.getArrearageTotalAmt()%>";//未收合同金额
	
	var contractBuyer ="<%=con.getBuyer()%>";
	var contractBuyerRepresentativeName ="<%=con.getBuyerRepresentativeName()%>";
	var amtInContract;//合同金额 ="<%=con.getContractAmt()%>";
	
	var ContractNoInfo ="<%=con.getContractNo()%>";
	var ContractSummaryInfo="<%=con.getContractSummary()%>";
	var ContractTypeIDInfo ="<%=con.getContractTypeID()%>";
	var ContractTypeNameInfo; //="<%=con.getContractTypeName()%>";
	var contractCurrencyId ="<%=con.getCurrencyID()%>";
	var CurrencyNameInfo = "<%=val%>"; 

	var UnCreditedTotalAmt;// ="<%=con.getUnCreditedTotalAmt() %>";//已开发票未到账金额
	var contractDeliveryName ="<%=con.getDeliveryName()%>";
	var contractDeptNameOfBuyer ="<%=con.getDeptNameOfBuyer()%>";
	var contractDeptNameOfVendor ="<%=con.getDeptNameOfVendor()%>";
	
	var contractEarnedTotalAmt;// ="<%=con.getEarnedTotalAmt()%>";//已收合同金额
	var GovermentContractSourceName ="<%=con.getGovermentContractSourceName()%>";
	var contractInvoicedTotalAmt;// ="<%=con.getInvoicedTotalAmt()%>";//已开发票金额
	var contractPaidTotalAmt ="<%=con.getPaidTotalAmt()%>";
	
	var contractParentAbb ="<%=con.getParentAbb()%>";
	var contractParentCode ="<%=con.getParentCode()%>";
	var contractParentName ="<%=con.getParentName()%>";
	var contractRegisterName ="<%=con.getRegisterName()%>";
	var contractRegisterSurname ="<%=con.getRegisterSurname()%>";
	var contractRemark ="<%=con.getRemark()%>";
	var contractSourceTypeName ="<%=con.getSourceTypeName()%>";
	var contractUnCreditedTotalAmt ="<%=con.getUnCreditedTotalAmt()%>";
	var contractUnPaidTotalAmt ="<%=con.getUnPaidTotalAmt()%>";
	
	var contractVendorName ="<%=con.getVendorName()%>";
	var contractVendorRepresentativeName ="<%=con.getVendorRepresentativeName()%>";
	var contractVendorRepresentativeSurname ="<%=con.getVendorRepresentativeSurname()%>";
	var contractWarrantyDefinition ="<%=con.getWarrantyDefinition()%>";
	var contractWarrantyLimit ="<%=(int)con.getWarrantyLimit()%>"+warrantyUnitName;//质保期
	var contractWarrantyUnit ="<%=con.getWarrantyUnit()%>";
	var contractRegisterName  ="<%=con.getRegisterName()==null?"":con.getRegisterName()%>";
	//项目
	var projectNameInContract = "<%=con.getProjectOBJ().getProjectName()%>";
	var projectIDInContract = "<%=con.getProjectOBJ().getProjectCode()==null?"":con.getProjectOBJ().getProjectCode() %>";
	var signatureDate ="<%=con.getSignatureDate()==null?"":con.getSignatureDate().toString().trim().substring(0,10) %>";
	//合同类型
	if(ContractTypeIDInfo=='1'){
	    ContractTypeNameInfo ="收入合同";
	}
	if(ContractTypeIDInfo=='2'){
	    ContractTypeNameInfo ="支出合同";
	}
	//货币类型

    var contractAmt = "<%=con.getContractAmt()>0?con.getContractAmt():0 %>";
   //需要写个函数处理...再补上
	if(CurrencyNameInfo.substr(0,3) =="人民币"){
	  amtInContract = "￥"+formatNumber(contractAmt,2)+"("+CurrencyNameInfo+")";	
	  contractArrearageTotalAmt = "￥"+formatNumber("<%=con.getArrearageTotalAmt()%>",2)+"("+CurrencyNameInfo+")";
	  UnCreditedTotalAmt = 	"￥"+formatNumber("<%=con.getUnCreditedTotalAmt() %>",2)+"("+CurrencyNameInfo+")";
	  contractEarnedTotalAmt ="￥"+formatNumber("<%=con.getEarnedTotalAmt()%>",2)+"("+CurrencyNameInfo+")";
	  contractInvoicedTotalAmt="￥"+formatNumber("<%=con.getInvoicedTotalAmt()%>",2)+"("+CurrencyNameInfo+")";
	}
	if(CurrencyNameInfo.substr(0,2) =="美元"){
	  amtInContract = "$"+formatNumber(contractAmt,2)+"("+CurrencyNameInfo+")";
	  contractArrearageTotalAmt = "$"+formatNumber("<%=con.getArrearageTotalAmt()%>",2)+"("+CurrencyNameInfo+")";
	  UnCreditedTotalAmt = 	"$"+formatNumber("<%=con.getUnCreditedTotalAmt() %>",2)+"("+CurrencyNameInfo+")";
	  contractEarnedTotalAmt ="$"+formatNumber("<%=con.getEarnedTotalAmt()%>",2)+"("+CurrencyNameInfo+")";
	  contractInvoicedTotalAmt="$"+formatNumber("<%=con.getInvoicedTotalAmt()%>",2)+"("+CurrencyNameInfo+")";	  	
	}
	if(CurrencyNameInfo.substr(0,2) =="欧元"){
	  amtInContract = "€"+formatNumber(contractAmt,2)+"("+CurrencyNameInfo+")";//€
	  contractArrearageTotalAmt = "€"+formatNumber("<%=con.getArrearageTotalAmt()%>",2)+"("+CurrencyNameInfo+")";
	  UnCreditedTotalAmt = 	"€"+formatNumber("<%=con.getUnCreditedTotalAmt() %>",2)+"("+CurrencyNameInfo+")";
	  contractEarnedTotalAmt ="€"+formatNumber("<%=con.getEarnedTotalAmt()%>",2)+"("+CurrencyNameInfo+")";
	  contractInvoicedTotalAmt="€"+formatNumber("<%=con.getInvoicedTotalAmt()%>",2)+"("+CurrencyNameInfo+")";
	  	
	}
	if(CurrencyNameInfo.substr(0,2) =="港币"){
	  amtInContract = "HK$"+formatNumber(contractAmt,2)+"("+CurrencyNameInfo+")";
	  contractArrearageTotalAmt = "HK$"+formatNumber("<%=con.getArrearageTotalAmt()%>",2)+"("+CurrencyNameInfo+")";
	  UnCreditedTotalAmt = 	"HK$"+formatNumber("<%=con.getUnCreditedTotalAmt() %>",2)+"("+CurrencyNameInfo+")";
	  contractEarnedTotalAmt ="HK$"+formatNumber("<%=con.getEarnedTotalAmt()%>",2)+"("+CurrencyNameInfo+")";
	  contractInvoicedTotalAmt="HK$"+formatNumber("<%=con.getInvoicedTotalAmt()%>",2)+"("+CurrencyNameInfo+")";
	  	
	}
	
	

</script>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <title>My JSP 'contractInfo.jsp' starting page</title>
    
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	<!--
	<link rel="stylesheet" type="text/css" href="styles.css">
	-->
	<script type="text/javascript" src="xMIS/resources/js/DataView-more.js"></script>
	<script language="JavaScript" src="/xMIS/contract/contractInfo.js"></script>

 
  </head>
  
  <body>

    <div id=<%=contractInfoDivName %>></div>
  	<div id=<%=contractInfoOtherDivName %>></div>
  	<div id=<%=ContractAttd %>></div>
  		<%if(con.getContractID() > 0 && con.getContractTypeID()== 1 && con.getContractAmt()>0){ %>
			<jsp:include page="<%=WebKeys.Include_JSP_File_paymentCondition %>" />

		<%} %>
  		<%if(con.isHadChild()){ %>
			<jsp:include page="<%=WebKeys.Include_JSP_File_ChildContractList %>" />
		<%} %>
  </body>
</html>

<script>


</script>