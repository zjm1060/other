package com.bitc.jk.xmis.util;

import javax.servlet.http.HttpServletRequest;

public class WebKeys {
	
	/*Layout Type*/
	public static final String Type_Layout_Ext = "extJS";
	public static final String Type_Layout_Without_Ext = "withoutExtJS";
	public static final String Session_Attribute_Name_Layout = "layout";
	
	/*Menu Name*/
	public static final String Tree_Menu_Name_myDeskMenu = "myDeskMenu";
	public static final String Tree_Menu_Name_myTaskMenu = "myTaskMenu";
	public static final String Tree_Menu_Name_ProjectMenu = "projectMenu";
	public static final String Tree_Menu_Name_ContractMenu = "contractMenu";
	public static final String Tree_Menu_Name_MaterialMenu = "materialMenu";
	public static final String Tree_Menu_Name_MeetingMenu = "meetingMenu";
	public static final String Tree_Menu_Name_CrmMenu = "crmMenu";
	public static final String Tree_Menu_Name_IssueMenu = "issueMenu";
	public static final String Tree_Menu_Name_OrganizationMenu = "organizationMenu";
	public static final String Tree_Menu_Name_ProductionMenu = "productionMenu";
	public static final String Tree_Menu_Name_WiKiMenu = "WiKiMenu";
	public static final String Tree_Menu_Name_SuggestionMenu = "SuggestionMenu";
	
	/*Tree ID for Left Menu Tree*/
	public static final int Menu_ID_myDesk_TreeID = 2001;		
	public static final int Menu_ID_myTask_TreeID = 2002;		
	public static final int Menu_ID_ProjectManagement_TreeID = 2003;		
	public static final int Menu_ID_ContractManagement_TreeID = 2004;		
	public static final int Menu_ID_MaterialManagement_TreeID = 2005;		
	public static final int Menu_ID_MeetingManagement_TreeID = 2006;		
	public static final int Menu_ID_CrmManagement_TreeID = 2007;		
	public static final int Menu_ID_IssueManagement_TreeID = 2008;		
	public static final int Menu_ID_OrganizationManagement_TreeID = 2009;		
	public static final int Menu_ID_ProductionManagement_TreeID = 2010;		
	public static final int Menu_ID_WiKiManagement_TreeID = 2011;		
	public static final int Menu_ID_SuggestionManagement_TreeID = 2012;		
	
	
	/*Menu id for Ext Layout*/
	public static final int Menu_ID_myDesk_WhatSNew = 1001;		
	public static final int Menu_ID_myDesk_MyHomePage = 1002;
	
	public static final int Menu_ID_myTask_TaskReceipt = 1003;
	public static final int Menu_ID_myTask_TaskExecute = 1004;
	public static final int Menu_ID_myTask_TaskHistory = 1005;
		
	public static final int Menu_ID_ProjectManagement_ProjectStart = 1006;		//项目启动
	public static final int Menu_ID_ProjectManagement_TaskAssign = 1007;			//任务分配
	public static final int Menu_ID_ProjectManagement_InnerAcceptance = 1008;	//内部验收
	public static final int Menu_ID_ProjectManagement_ExternalAcceptance = 1009;	//外部验收
	public static final int Menu_ID_ProjectManagement_ContractChange = 1010;		//合同变更
	public static final int Menu_ID_ProjectManagement_ProjectMaint = 1011;		//项目维护
	public static final int Menu_ID_ProjectManagement_LocaleReport = 1012;		//现场工作报告
	public static final int Menu_ID_ProjectManagement_CarryOverQuestion = 1013;		//遗留问题处理
	public static final int Menu_ID_ProjectManagement_IntegrationSearch = 1014;		//项目综合查询
		
	public static final int Menu_ID_ContractManagement_ContractEntry = 1015;		//合同录入
	public static final int Menu_ID_ContractManagement_InvoiceEntry= 1016;		//票据收发登记
	public static final int Menu_ID_ContractManagement_PaymentEntry = 1017;		//合同款首付登记
	public static final int Menu_ID_ContractManagement_IntegrationSearch = 1018;		//合同综合查询
	
	public static final int Menu_ID_MaterialManagement_DevicePurchase = 1019;		//设备采购
	public static final int Menu_ID_MaterialManagement_DeviceWarehouseEntry = 1020;		//设备入库登记
	public static final int Menu_ID_MaterialManagement_DeviceReceive = 1021;		//设备领用
	public static final int Menu_ID_MaterialManagement_DeviceDelivery = 1022;		//设备发货
	public static final int Menu_ID_MaterialManagement_DeviceReturnChange = 1023;		//返修更换
	public static final int Menu_ID_MaterialManagement_IntegrationSearch = 1024;		//物资信息库
		
	public static final int Menu_ID_MeetingManagement_MeetingStart = 1025;		//会议发起
	
	public static final int Menu_ID_CrmManagement_CRMEntry = 1026;			//客户信息登记
		
	public static final int Menu_ID_IssueManagement_IssueEntry = 1027; 		//缺陷登记
		
	public static final int Menu_ID_OrganizationManagement_OrganizationTree = 1028;		//组织架构
	public static final int Menu_ID_OrganizationManagement_GoOutEentry = 1029;		//外出登记
	public static final int Menu_ID_OrganizationManagement_BusinessTripEntry = 1030;		//出差登记
	public static final int Menu_ID_OrganizationManagement_DailyRecord = 1031;		//工作日志
	public static final int Menu_ID_OrganizationManagement_VacationRequist = 1032;		//休假申请
	public static final int Menu_ID_OrganizationManagement_UserRightRequist = 1033;		//权限申请
		
	public static final int Menu_ID_ProductionManagement_NewProduction = 1034;		//新产品开发
	public static final int Menu_ID_ProductionManagement_SubsequentDevelop = 1035;		//后续开发
	public static final int Menu_ID_ProductionManagement_ProductionRelease= 1036;		//产品发布
		
	public static final int Menu_ID_WiKiManagement_WikiEntry = 1037;		//新数据添加
	public static final int Menu_ID_WiKiManagement_IntegrationSearch = 1038;		//知识库查询
	
	public static final int Menu_ID_SuggestionManagement_SuggestionEntry = 1039;		//信息采集
	
	
	/*Ext Portal */
	public static final String Ext_Portal_Head_File = "/xMIS/extPortal/head.jsp"; 
	
 
	/*Forword Name*/
	public static final String Forword_to_LoginSuccessfully = "loginSuccessfully";
	public static final String Forword_to_LoginSuccessfully_Ext = "loginSuccessfully_ext";

	public static final String Forword_to_LoginFailed = "loginFailed";
	public static final String Forword_to_ReLogin = "reLogin";

	public static final String Forword_to_orgMaint = "org_maint";
	public static final String Forword_to_deptTree = "deptTree";
	public static final String Forword_to_DivisionTotalSummary = "divisionTotalSummary";
	public static final String Forword_to_DepartmentTotalSummary = "deptTotalSummary";
	public static final String Forword_to_Employee = "employee";

	public static final String Forword_to_constractList="contractList";
	public static final String Forword_to_constractList_Ext="contractList_ext";
	public static final String Forword_to_constract="contract";
	public static final String Forword_to_constract_ext="contractExt";
	public static final String Forword_to_constractReadonly="contractReadOnly";
	public static final String Forword_to_paymentListEdit="paymentListEdit";
	public static final String Forword_to_invoiceEdit="invoiceEdit";
	public static final String Forword_to_PaymentOption="paymentOption";
	
	

	/*Session Attribute Name*/
	public static final String Session_Attribute_Name_UserDNA = "userDNA";

	/*Request Attribute Name & value*/
	public static final String Request_Attribute_Name_LoginFailed = "loginFailed";
	public static final String Request_Attribute_Name_LoginFailed_Value = "用户名 or 密码无效！";
	public static final String Request_Attribute_Name_ProjListByDiv = "project_list_by_division";
	public static final String Request_Attribute_Name_ErrMessage = "err_message";
	public static final String Request_Attribute_Name_ReLogin_Value = "请重新登录，谢谢！";
	public static final String Request_Attribute_Name_ErrMessage_Value = "发生异常！\n请联系管理员，或重新登录，谢谢！";
	public static final String Request_Attribute_Name_Grid_Title_Prefix = "grid_title_prefix";
	public static final String Request_Attribute_Name_EmployeeObjert = "employeeObject";
	public static final String Request_Attribute_Name_CurrentDATE = "currentDATE";
	public static final String Request_Attribute_Name_ContractObject = "contractObject";
	
	public static final String Request_Attribute_Name_SearchType_For_ContractList = "searchTypeForContractList";
	
	public static final String Request_Attribute_Name_Type = "type";
	public static final String Request_Attribute_Name_Type_Employee = "Employee";
	public static final String Request_Attribute_Name_Type_Contract = "Contract";

	public static final String Request_Attribute_Name_SubType = "subType";
	public static final String Request_Attribute_Name_SubType_departmentTree = "departmentTree";
	public static final String Request_Attribute_Name_SubType_contractList = "contractList";
	public static final String Request_Attribute_Name_SubType_contract = "contract";
	public static final String Request_Attribute_Name_SubType_paymentListEdit = "paymentListEdit";
	public static final String Request_Attribute_Name_SubType_invoiceEdit = "invoiceEdit";
	
	public static final String Request_Attribute_Name_Department = "department"; 
	
	/*Include JSP File Name*/
	public static final String contextPath = "/xMIS";
	public static final String Include_JSP_File_mainmenu = "../portal/mainmenu.jsp";
	public static final String Include_JSP_File_xMisPortal = "../portal/xMisPortal.jsp";
	public static final String Include_JSP_File_subMain = "../portal/subMain.jsp";
	public static final String Include_JSP_File_operationPart = "../portal/operationPart.jsp";
	public static final String Include_JSP_File_sideBar = "../portal/siderBar.jsp";

	public static final String Include_JSP_File_building = "../building.html";
//	public static final String Include_JSP_File_building = "../portal/xMisPortal.jsp";
	
	/*contract JSP File Name*/
	public static final String Include_JSP_File_contract_subMain = "../contract/subMainContract.jsp";
	public static final String Include_JSP_File_contractList = "../contract/contractList.jsp";
	public static final String Include_JSP_File_contract = "../contract/contract.jsp";
	public static final String Include_JSP_File_paymentCondition = "../contract/paymentCondition.jsp";
	public static final String Include_JSP_File_ChildContractList = "../contract/childContractList.jsp";
	public static final String Include_JSP_File_paymentList = "../contract/paymentList.jsp";
	public static final String Include_JSP_File_paymentListEdit = "../contract/paymentListEdit.jsp";
	public static final String Include_JSP_File_paymentEdit = "../contract/paymentEdit.jsp";
	public static final String Include_JSP_File_InvoiceList = "../contract/invoiceList.jsp";
	public static final String Include_JSP_File_PaymentOption = "../contract/paymentOption.jsp";
	public static final String Include_JSP_File_invoiceEdit = "../contract/invoiceEdit.jsp";
	
	/*HR JSP File Name*/
	public static final String Include_JSP_File_org_subMain = "../organization/subMainOrganization.jsp";
	public static final String Include_JSP_File_departmentTree = "../organization/departmentTree.jsp";
	
	/*DepartmentTree XML Dir*/
	public static final String String_DepartmentTree_XML_Dir = "\\resources\\xml\\departmentTree.xml";

	
	/*Organization Type*/
	public static final String Type_Division = "division";
	public static final String Type_Department = Request_Attribute_Name_Department;
	public static final String Type_Department_Name = "deptName";
		
	/*Action Method Name*/
	public static final String Action_Method_Name_DeptTree = "deptTree";
	public static final String Action_Method_Name_DivisionTotalSummary = "divisionTotalSummary";
	public static final String Action_Method_Name_DepartmentTotalSummary = "deptTotalSummary";
	public static final String Action_Method_Name_DeptFinishedProjects = "deptFinishedProj";
	public static final String Action_Method_Name_DeptUnfinishedProjects = "deptUnfinishedProj";
	public static final String Action_Method_Name_DeptAllProjects = "deptAllProj";
	public static final String Action_Method_Name_GetEmployee = "getEmployee";
	public static final String Action_Method_Name_GetProject = "getProject";
	public static final String Action_Method_Name_TaskList = "taskList";
	public static final String Action_Method_Name_ContractListForParentID = "contractListForParentID";
	public static final String Action_Method_Name_AttachedList = "attachedList";
	public static final String Action_Method_Name_CompanyList = "companyList";
	public static final String Action_Method_Name_DeptList = "deptList";
	public static final String Action_Method_Name_ContactPeason = "contactPeason";
	public static final String Action_Method_Name_PaymentCondition = "paymentCondition";
	public static final String Action_Method_Name_PaymentList = "paymentList";
	public static final String Action_Method_Name_ProjectStatus = "projectStatus";
	public static final String Action_Method_Name_ContractList = "contractList";
	public static final String Action_Method_Name_ChildContractList = "childContractList";
	public static final String Action_Method_Name_InvoiceList = "invoiceList";
	public static final String Action_Method_Name_ContractListCount = "contractListCount";
	
	
	
	public WebKeys(HttpServletRequest request){
		hrefStr = "<a href='"+request.getContextPath().replaceAll("webBTLBBO","cis-sp")+"'>"; 
	}
	
	private String hrefStr = ""; 	
    public static final String TARGET_URL_SESSION_KEY = "com.bitc.jk.web.session.targetURL";

	public final static String RequestKey_Original_URL = "request.Original_URL";
	public final static String RequestKey_Original_URI = "request.Original_URI";
    public final static String RequestKey_Original_QueryString = "request.Original_RequestString";
    
    public final static String AttachmentWebPath = "resources\\images\\userPic\\";
    public final static String AttachmentURL = "/xMIS/resources/images/userPic/";
    
    
    public final static String orgMaintURI = "/orgMaint.do";
    public final static String contractMainURI = "/contractMain.do";
    
    
    public final static String NodeTipOnDeptTree = "如需启用，请联系管理员";
    public final static int LunchUpdatedTerm = 25;
    
    /*User Role Name*/
    public static final String User_Role_Name_Admin = "admin";
    
    /*Service Object Type*/
    public static final int ServiceImpl_Object_Organization = 1;
    public static final int ServiceImpl_Object_Contract = 2;
    
    /*CRM */
    public static final String New_Company_Simple_Type_Company = "company";
    public static final String New_Company_Simple_Type_Department = "department";
    public static final String New_Company_Simple_Type_ContactPerson = "contractPerson";
    public static final String New_Company_Simple_For_Dept_Other = "其它";
    
    
    /*Contract Warranty Unit Def*/
    public static final int Contract_Obj_Warranty_Unit_Def_DAY = 1;
    public static final int Contract_Obj_Warranty_Unit_Def_MONTH = 2;
    public static final int Contract_Obj_Warranty_Unit_Def_YEAR = 3;
    
    /*Ext errMsg for Get Store*/
    public static final String Ext_errMsg_For_Get_Store_Title = "提示";
    public static final String Ext_errMsg_For_Get_Store_Message = "获取数据失败！请重新刷新页面，或重新登录！谢谢。";
    
    public static final String Ext_Message_Alert_Title = "提示";
    public static final String Ext_Parent_Contract_Choose_Duplicate = "不能选择自己为父合同！";
    public static final String Ext_Message_For_Company_Null = "请先选择";
    
    /*Upload File Type*/
    public static final String Upload_File_Type_Allowed_Contract = "jpg,gif,bmp,png,pdf,doc,xls,ppt,txt,rar,zip,JPG,GIF,BMP,PNG,PDF,DOC,XLS,PPT,TXT,RAR,ZIP";
    public static final String Upload_File_Type_Denfid_Contract = "exe,bat,jsp,htm,html,EXE,BAT,JSP,HTM,HTML";
    public static final String Upload_File_Type_Err_Msg_Contract = "JPG,GIF,BMP,PNG,PDF,DOC,XLS,PPT,TXT,RAR,ZIP";
    
    public static final int Upload_Application_UserPhoto_Type = 1;	//user pic
    public static final int Upload_Application_Contract_Type = 2;	//contract attachment
    public static final int Upload_Application_Download_Type = 3;		//download type
    public static final int Upload_Application_PaymentInfo_Type = 4;	//payment info
    public static final int Upload_Application_InvoiceInfo_Type = 5;	//invoice info
    
    	
    
    /*Total Sum Type by ContractID*/
    public static final int Type_Total_Sum_Paymented = 1;
    public static final int Type_Total_Sum_Invoiced = 2;
    
    /*Contract Type*/
    public static final int Type_ContractType_Income = 1; //收入合同
    public static final int Type_ContractType_Payout = 2; //支出合同
    
    /*Payment Type */
    public static final int Type_PaymentType_Income = 1; //收入合同
    public static final int Type_PaymentType_Payout = 2; //收入合同
    
    /*Message for payment & invoice*/
    public static final String Message_PaymentType_Income = "来款信息";
    public static final String Message_PaymentType_Payout = "付款信息";
    
    public static final String Message_InvoiceType_Income = "收入合同发票信息";
    public static final String Message_InvoiceType_Payout = "支出合同发票信息";
    
    /*font color for Amt on contract list*/
    public static final String Color_Font_Contract_Total_Amt = "#008000";
    public static final String Color_Font_Contract_Total_Charged_Amt = "#8A2BE2";
    public static final String Color_Font_Contract_Total_Invoiced_Amt = "#FF00FF";
    public static final String Color_Font_Contract_Total_UnAccount_Amt = "#FF4500";
    public static final String Color_Font_Contract_Total_UnCharged_Amt = "#00008B";
    
    
    /*cell background color for Warning on Contract List*/
    public static final String Color_BG_Warning_Earlier_30_Days = "#D7F5FF";
    public static final String Color_BG_Warning_Earlier_15_Days = "#B0E0E6";
    public static final String Color_BG_Warning_Earlier_7_Days = "#87CEEB";
    public static final String Color_BG_Warning_Current_Date = "#FFFF00";
    public static final String Color_BG_Warning_Overtime_Current_Date = "#ffde90";
    public static final String Color_BG_Warning_Overtime_More_7_Days = "#ffc436";
    public static final String Color_BG_Warning_Overtime_More_15_Days = "#ff8309";
    public static final String Color_BG_Warning_Overtime_More_30_Days = "#ff0000";

    public static final String Color_BG_Warning_Without_Definition = "#D8D8D8";
    
    
    /*warning type */
    public static final int Type_Warning_Earlier_30_Days = 30;
    public static final int Type_Warning_Earlier_15_Days = 15;
    public static final int Type_Warning_Earlier_7_Days = 7;
    public static final int Type_Warning_Current_Date = 1;
    public static final int Type_Warning_Overtime_Current_Date = -1;
    public static final int Type_Warning_Overtime_More_7_Days = -7;
    public static final int Type_Warning_Overtime_More_15_Days = -15;
    public static final int Type_Warning_Overtime_More_30_Days = -30;
    
    public static final int Type_Warning_Without_Definition = -31;
    public static final int Type_Warning_Info = 31;
    

    /*tip title for warning */
    public static final String Tip_Title_Warning_Earlier_30_Days = "30天来款";
    public static final String Tip_Title_Warning_Earlier_15_Days = "15天来款";
    public static final String Tip_Title_Warning_Earlier_7_Days = "7天来款";
    public static final String Tip_Title_Warning_Current_Date = "今日";
    public static final String Tip_Title_Warning_Overtime_Current_Date = "近日";
    public static final String Tip_Title_Warning_Overtime_More_7_Days = "超时7天";
    public static final String Tip_Title_Warning_Overtime_More_15_Days = "超时15天";
    public static final String Tip_Title_Warning_Overtime_More_30_Days = "超时30天";
    
    public static final String Tip_Title_Warning_Without_Definition = "无明确付款信息";
    public static final String Tip_Title_Warning_Info = "最近付款信息";
    
    
    /*Type of search for Contract List*/
    public static final String Type_Search_For_ContractList_Normal = "normal";
    public static final String Type_Search_For_ContractList_Integration = "integration";
    
    /*Contract Integration Search Type*/
    public static final int Type_Contract_Integration_Simple = 1; 			//正常查询
    public static final int Type_Contract_Integration_New = 2; 			//新签订的合同
    public static final int Type_Contract_Integration_Performed = 3; 	//合同执行情况
    public static final int Type_Contract_Integration_EstimateContractAmt = 4; 	//预计来款
    public static final int Type_Contract_Integration_Warranty = 5; 			//质保期到期
    
    
    /*Type of Currency */
    public static final int Type_Currency_RMB = 1;	//人民币
    public static final int Type_Currency_USD= 2;	//美元
    public static final int Type_Currency_Euro = 3;	//欧元
    public static final int Type_Currency_HKD = 4;	//港币
    
    public static final String Type_Currency_Graphy_RMB = "￥";
    public static final String Type_Currency_Graphy_USD = "$";
    public static final String Type_Currency_Graphy_Euro = "欧元";
    public static final String Type_Currency_Graphy_HKD = "HK$";
    
    /*SubIntegration Type for Currency*/
    public static final String Type_subIntegration_Nomal = "nomal";
    public static final String Type_subIntegration_Payed = "payed";
    public static final String Type_subIntegration_Invoiced = "invoiced";
    public static final String Type_subIntegration_Estimate = "estimate";
    
    
    
    
}
