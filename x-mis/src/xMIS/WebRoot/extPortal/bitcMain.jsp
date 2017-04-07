<%@ page contentType="text/html; charset=utf-8"%>
<%@ page import="java.lang.*,java.util.*,java.text.*"%>
<%@ page import="com.bitc.jk.xmis.util.WebKeys"%>
<%@ page import="com.bitc.jk.xmis.model.EmployeeObj"%>


<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>


<script language="javascript">

var Ext_Portal_Head_File = "<%=WebKeys.Ext_Portal_Head_File %>";
var menuTitle = "<%=((EmployeeObj) session
									.getAttribute(WebKeys.Session_Attribute_Name_UserDNA))
									.getDivisionName()%>";


	var Menu_ID_myDesk_TreeID = <%=WebKeys.Menu_ID_myDesk_TreeID %>;		
	var Menu_ID_myTask_TreeID = <%=WebKeys.Menu_ID_myTask_TreeID %>;		
	var Menu_ID_ProjectManagement_TreeID = <%=WebKeys.Menu_ID_ProjectManagement_TreeID %>;			
	var Menu_ID_ContractManagement_TreeID = <%=WebKeys.Menu_ID_ContractManagement_TreeID %>;			
	var Menu_ID_MaterialManagement_TreeID = <%=WebKeys.Menu_ID_MaterialManagement_TreeID %>;			
	var Menu_ID_MeetingManagement_TreeID = <%=WebKeys.Menu_ID_MeetingManagement_TreeID %>;			
	var Menu_ID_CrmManagement_TreeID = <%=WebKeys.Menu_ID_CrmManagement_TreeID %>;			
	var Menu_ID_IssueManagement_TreeID = <%=WebKeys.Menu_ID_IssueManagement_TreeID %>;				
	var Menu_ID_OrganizationManagement_TreeID = <%=WebKeys.Menu_ID_OrganizationManagement_TreeID %>;			
	var Menu_ID_ProductionManagement_TreeID = <%=WebKeys.Menu_ID_ProductionManagement_TreeID %>;			
	var Menu_ID_WiKiManagement_TreeID = <%=WebKeys.Menu_ID_WiKiManagement_TreeID %>;			
	var Menu_ID_SuggestionManagement_TreeID = <%=WebKeys.Menu_ID_SuggestionManagement_TreeID %>;			

	var Tree_Menu_Name_ProjectMenu = "<%=WebKeys.Tree_Menu_Name_ProjectMenu %>";
	var Tree_Menu_Name_ContractMenu = "<%=WebKeys.Tree_Menu_Name_ContractMenu %>";
	var Tree_Menu_Name_myDeskMenu = "<%=WebKeys.Tree_Menu_Name_myDeskMenu %>";
	var Tree_Menu_Name_myTaskMenu = "<%=WebKeys.Tree_Menu_Name_myTaskMenu %>";
	var Tree_Menu_Name_MaterialMenu = "<%=WebKeys.Tree_Menu_Name_MaterialMenu %>";
	var Tree_Menu_Name_MeetingMenu = "<%=WebKeys.Tree_Menu_Name_MeetingMenu %>";
	var Tree_Menu_Name_CrmMenu = "<%=WebKeys.Tree_Menu_Name_CrmMenu %>";
	var Tree_Menu_Name_IssueMenu = "<%=WebKeys.Tree_Menu_Name_IssueMenu %>";
	var Tree_Menu_Name_OrganizationMenu = "<%=WebKeys.Tree_Menu_Name_OrganizationMenu %>";
	var Tree_Menu_Name_ProductionMenu = "<%=WebKeys.Tree_Menu_Name_ProductionMenu %>";
	var Tree_Menu_Name_WiKiMenu = "<%=WebKeys.Tree_Menu_Name_WiKiMenu %>";
	var Tree_Menu_Name_SuggestionMenu = "<%=WebKeys.Tree_Menu_Name_SuggestionMenu %>";


	var Menu_ID_myDesk_WhatSNew = <%=WebKeys.Menu_ID_myDesk_WhatSNew %>;		
	var Menu_ID_myDesk_MyHomePage = <%=WebKeys.Menu_ID_myDesk_MyHomePage %>;
	
	var Menu_ID_myTask_TaskReceipt = <%=WebKeys.Menu_ID_myTask_TaskReceipt %>;
	var Menu_ID_myTask_TaskExecute = <%=WebKeys.Menu_ID_myTask_TaskExecute %>;
	var Menu_ID_myTask_TaskHistory = <%=WebKeys.Menu_ID_myTask_TaskHistory %>;
	
	
	var Menu_ID_ProjectManagement_ProjectStart = <%=WebKeys.Menu_ID_ProjectManagement_ProjectStart %>;		//项目启动
	var Menu_ID_ProjectManagement_TaskAssign = <%=WebKeys.Menu_ID_ProjectManagement_TaskAssign %>;			//任务分配
	var Menu_ID_ProjectManagement_InnerAcceptance = <%=WebKeys.Menu_ID_ProjectManagement_InnerAcceptance %>;	//内部验收
	var Menu_ID_ProjectManagement_ExternalAcceptance = <%=WebKeys.Menu_ID_ProjectManagement_ExternalAcceptance %>;	//外部验收
	var Menu_ID_ProjectManagement_ContractChange = <%=WebKeys.Menu_ID_ProjectManagement_ContractChange %>;		//合同变更
	var Menu_ID_ProjectManagement_ProjectMaint = <%=WebKeys.Menu_ID_ProjectManagement_ProjectMaint %>;		//项目维护
	var Menu_ID_ProjectManagement_LocaleReport = <%=WebKeys.Menu_ID_ProjectManagement_LocaleReport %>;		//现场工作报告
	var Menu_ID_ProjectManagement_CarryOverQuestion = <%=WebKeys.Menu_ID_ProjectManagement_CarryOverQuestion %>;		//遗留问题处理
	var Menu_ID_ProjectManagement_IntegrationSearch = <%=WebKeys.Menu_ID_ProjectManagement_IntegrationSearch %>;		//项目综合查询
	
	
	var Menu_ID_ContractManagement_ContractEntry = <%=WebKeys.Menu_ID_ContractManagement_ContractEntry %>;		//合同录入
	var Menu_ID_ContractManagement_InvoiceEntry= <%=WebKeys.Menu_ID_ContractManagement_InvoiceEntry %>;		//票据收发登记
	var Menu_ID_ContractManagement_PaymentEntry = <%=WebKeys.Menu_ID_ContractManagement_PaymentEntry %>;		//合同款首付登记
	var Menu_ID_ContractManagement_IntegrationSearch = <%=WebKeys.Menu_ID_ContractManagement_IntegrationSearch %>;		//合同综合查询
	
	var Menu_ID_MaterialManagement_DevicePurchase = <%=WebKeys.Menu_ID_MaterialManagement_DevicePurchase %>;		//设备采购
	var Menu_ID_MaterialManagement_DeviceWarehouseEntry = <%=WebKeys.Menu_ID_MaterialManagement_DeviceWarehouseEntry %>;		//设备入库登记
	var Menu_ID_MaterialManagement_DeviceReceive = <%=WebKeys.Menu_ID_MaterialManagement_DeviceReceive %>;		//设备领用
	var Menu_ID_MaterialManagement_DeviceDelivery = <%=WebKeys.Menu_ID_MaterialManagement_DeviceDelivery %>;		//设备发货
	var Menu_ID_MaterialManagement_DeviceReturnChange = <%=WebKeys.Menu_ID_MaterialManagement_DeviceReturnChange %>;		//返修更换
	var Menu_ID_MaterialManagement_IntegrationSearch = <%=WebKeys.Menu_ID_MaterialManagement_IntegrationSearch %>;		//物资信息库
	
	
	var Menu_ID_MeetingManagement_MeetingStart = <%=WebKeys.Menu_ID_MeetingManagement_MeetingStart %>;		//会议发起
	
	var Menu_ID_CrmManagement_CRMEntry = <%=WebKeys.Menu_ID_CrmManagement_CRMEntry %>;			//客户信息登记
	
	
	var Menu_ID_IssueManagement_IssueEntry = <%=WebKeys.Menu_ID_IssueManagement_IssueEntry %>; 		//缺陷登记
	
	
	var Menu_ID_OrganizationManagement_OrganizationTree = <%=WebKeys.Menu_ID_OrganizationManagement_OrganizationTree %>;		//组织架构
	var Menu_ID_OrganizationManagement_GoOutEentry = <%=WebKeys.Menu_ID_OrganizationManagement_GoOutEentry %>;		//外出登记
	var Menu_ID_OrganizationManagement_BusinessTripEntry = <%=WebKeys.Menu_ID_OrganizationManagement_BusinessTripEntry %>;		//出差登记
	var Menu_ID_OrganizationManagement_DailyRecord = <%=WebKeys.Menu_ID_OrganizationManagement_DailyRecord %>;		//工作日志
	var Menu_ID_OrganizationManagement_VacationRequist = <%=WebKeys.Menu_ID_OrganizationManagement_VacationRequist %>;		//休假申请
	var Menu_ID_OrganizationManagement_UserRightRequist = <%=WebKeys.Menu_ID_OrganizationManagement_UserRightRequist %>;		//权限申请
	
	
	var Menu_ID_ProductionManagement_NewProduction = <%=WebKeys.Menu_ID_ProductionManagement_NewProduction %>;		//新产品开发
	var Menu_ID_ProductionManagement_SubsequentDevelop = <%=WebKeys.Menu_ID_ProductionManagement_SubsequentDevelop %>;		//后续开发
	var Menu_ID_ProductionManagement_ProductionRelease= <%=WebKeys.Menu_ID_ProductionManagement_ProductionRelease %>;		//产品发布
	
	
	var Menu_ID_WiKiManagement_WikiEntry = <%=WebKeys.Menu_ID_WiKiManagement_WikiEntry %>;		//新数据添加
	var Menu_ID_WiKiManagement_IntegrationSearch = <%=WebKeys.Menu_ID_WiKiManagement_IntegrationSearch %>;		//知识库查询

	
	var Menu_ID_SuggestionManagement_SuggestionEntry = <%=WebKeys.Menu_ID_SuggestionManagement_SuggestionEntry %>;		//信息采集

	var treePre = 'Tree_';

</script>




<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">

	<link rel="stylesheet" type="text/css" href="/xMIS/resources/css/ext-all.css" />
    <link rel="stylesheet" type="text/css" href="/xMIS/resources/css/RowEditor.css" />
    <link rel="stylesheet" type="text/css" href="/xMIS/resources/css/ext-custom.css" />
    <link rel="stylesheet" type="text/css" href="/xMIS/resources/css/grid-examples.css" />
    
    
    <link href="/xMIS/resources/css/calendarStyle.css" type=text/css rel=stylesheet></link>
	<LINK href="/xMIS/resources/css/styleCIS.css" type=text/css	rel=stylesheet>

	<script type="text/javascript" src="/xMIS/resources/js/ext-base.js"></script>
	<script type="text/javascript" src="/xMIS/resources/js/ext-all.js"></script>
	<script type="text/javascript" src="/xMIS/resources/js/ext-lang-zh_CN.js"></script>    
	  	

	<script type="text/javascript" src="/xMIS/resources/js/RowEditor.js"></script>
	<script type="text/javascript" src="/xMIS/resources/js/CheckColumn.js"></script>
	<script type="text/javascript" src="/xMIS/resources/js/SearchField.js"></script>

	<script type="text/javascript" src="/xMIS/resources/js/common.js"></script>
	<script language="JavaScript" src="/xMIS/resources/js/prototype.js"></script>
	<script language="JavaScript" src="/xMIS/resources/js/bitc.js"></script>
	
	<style type="text/css">
		body .x-panel {
		    margin-bottom:0px;
		}
    </style>
	
	<script type="text/javascript"	src="/xMIS/extPortal/sideBar.js"></script>
	<script type="text/javascript"	src="/xMIS/extPortal/bitcMain.js"></script>


		<title>北京中水科技 X-MIS 综合信息管理系统</title>
		
  </head>
  
  <body>
  </body>
</html>

