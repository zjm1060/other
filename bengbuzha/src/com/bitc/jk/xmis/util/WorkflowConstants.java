package com.bitc.jk.xmis.util;

public interface WorkflowConstants {
	static final String WF_START_PROJECT = "StartProject";

	static final String WF_PURCHASE_REQUISITION = "PurchaseRequisition";

	static final String WF_TASK_ASSIGN = "TaskAssign";

	static final String WF_PROJECT_TASK_ASSIGN = "ProjectTaskAssign";
	
	static final String WF_PROJECT_EXECUTIVE_DEPT_CHANGE= "ProjectExecutiveDeptChange";
	
	static final String WF_INTERCHECK= "ProjectCheck";//内部验收
	
	static final String WF_MISS_ATTENDANCE= "MissAttence";//补报考勤
	
	static final String WF_APPLYOFFICIAL_SUPPLIES= "ApplyOfficialSupplies";//办公用品领用
	
	static final String WF_APPLYOFFICIAL_SUPPLIES_REPAIR= "ApplyOfficialSuppliesRepair";//办公用品维修
	
	static final String WF_APPLY_BUSINESS_ENTERTAINMENT= "ApplyBusinessEntertainment";//业务招待 

	static final String WF_ASK_FOR_LEAVE= "AskForLeave";//请假
	
	static final String WF_APPLY_DOCUMENT_CIRCULATION= "ApplyDocumentCirculation";//请假
	
	static final String WF_Monthly_Management_Work= "MonthlyManagementWork";// 部门月报

}
