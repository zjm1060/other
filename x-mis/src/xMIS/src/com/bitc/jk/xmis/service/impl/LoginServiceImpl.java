//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\service\\Impl\\LoginServiceImpl.java

package com.bitc.jk.xmis.service.impl;

import java.util.List;

import org.apache.log4j.Logger;

import com.bitc.jk.xmis.dao.OrganizationDAO;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.service.LoginService;
import com.bitc.jk.xmis.util.WebKeys;


public class LoginServiceImpl implements LoginService {
	private static Logger logger = Logger.getLogger(LoginServiceImpl.class);

	private OrganizationDAO organizationDAO;

	/**
	 * @roseuid 4C19886300CB
	 */
	public LoginServiceImpl() {

	}

	/**
	 * @param loginName
	 * @param password
	 * @return com.bitc.jk.xmis.model.EmployeeObj
	 * @roseuid 4C198C3B009C
	 */
	public EmployeeObj userValidete(String loginName, String password)
			throws Exception {
		try {
//			Calendar c = Calendar.getInstance();
//			c.setTime(new Date(System.currentTimeMillis()));
//
//			if (c.get(Calendar.DATE) == 1)
//				organizationDAO.doBookLunchAddress();

			return organizationDAO.userValidete(loginName, password);

		} catch (Exception e) {
			e.printStackTrace();
			throw e;
		}

	}
	
	public String buildLeftMenu(String treeName, EmployeeObj userDNA){
		
		String menuStr = "[";
		
		if(WebKeys.Tree_Menu_Name_myDeskMenu.equals(treeName)){
			menuStr += "{id:" + WebKeys.Menu_ID_myDesk_MyHomePage + ",text:'" + "My Home Page" + "',leaf:true},"
			+  "{id:" + WebKeys.Menu_ID_myDesk_WhatSNew + ",text:'" + "What\\'s New?" + "',leaf:true}"
			;
			
		}else if(WebKeys.Tree_Menu_Name_myTaskMenu.equals(treeName)){
				menuStr += "{id:" + WebKeys.Menu_ID_myTask_TaskReceipt + ",text:'" + "接收任务" + "',leaf:true},"
				+  "{id:" + WebKeys.Menu_ID_myTask_TaskExecute + ",text:'" + "执行任务" + "',leaf:true},"
				+  "{id:" + WebKeys.Menu_ID_myTask_TaskHistory + ",text:'" + "历史任务" + "',leaf:true}"
				;
				
		}else if(WebKeys.Tree_Menu_Name_ProjectMenu.equals(treeName)){
			menuStr += "{id:" + WebKeys.Menu_ID_ProjectManagement_ProjectStart + ",text:'" + "项目启动" + "',leaf:true},"
					+  "{id:" + WebKeys.Menu_ID_ProjectManagement_TaskAssign + ",text:'" + "任务分配" + "',leaf:true},"
					+  "{id:" + WebKeys.Menu_ID_ProjectManagement_InnerAcceptance + ",text:'" + "内部验收" + "',leaf:true},"
					+  "{id:" + WebKeys.Menu_ID_ProjectManagement_ExternalAcceptance + ",text:'" + "外部验收" + "',leaf:true},"
					+  "{id:" + WebKeys.Menu_ID_ProjectManagement_ContractChange + ",text:'" + "合同变更" + "',leaf:true},"
					+  "{id:" + WebKeys.Menu_ID_ProjectManagement_ProjectMaint + ",text:'" + "项目维护" + "',leaf:true},"
					+  "{id:" + WebKeys.Menu_ID_ProjectManagement_LocaleReport + ",text:'" + "现场工作报告" + "',leaf:true},"
					+  "{id:" + WebKeys.Menu_ID_ProjectManagement_CarryOverQuestion + ",text:'" + "遗留问题处理" + "',leaf:true},"
					+  "{id:" + WebKeys.Menu_ID_ProjectManagement_IntegrationSearch + ",text:'" + "项目综合查询" + "',leaf:true}"
			;
			
		}else if(WebKeys.Tree_Menu_Name_ContractMenu.equals(treeName)){
			menuStr += "{id:" + WebKeys.Menu_ID_ContractManagement_ContractEntry + ",text:'" + "合同录入" + "',leaf:true},"
			+  "{id:" + WebKeys.Menu_ID_ContractManagement_InvoiceEntry + ",text:'" + "票据收发登记" + "',leaf:true},"
			+  "{id:" + WebKeys.Menu_ID_ContractManagement_PaymentEntry + ",text:'" + "合同款首付登记" + "',leaf:true},"
			+  "{id:" + WebKeys.Menu_ID_ContractManagement_IntegrationSearch + ",text:'" + "合同综合查询" + "',leaf:true}"
			;
			
		}else if(WebKeys.Tree_Menu_Name_MaterialMenu.equals(treeName)){
			menuStr += "{id:" + WebKeys.Menu_ID_MaterialManagement_DevicePurchase + ",text:'" + "设备采购" + "',leaf:true},"
			+  "{id:" + WebKeys.Menu_ID_MaterialManagement_DeviceWarehouseEntry + ",text:'" + "设备入库登记" + "',leaf:true},"
			+  "{id:" + WebKeys.Menu_ID_MaterialManagement_DeviceReceive + ",text:'" + "设备领用" + "',leaf:true},"
			+  "{id:" + WebKeys.Menu_ID_ContractManagement_InvoiceEntry + ",text:'" + "票据收发登记" + "',leaf:true},"
			+  "{id:" + WebKeys.Menu_ID_MaterialManagement_DeviceReturnChange + ",text:'" + "返修更换" + "',leaf:true},"
			+  "{id:" + WebKeys.Menu_ID_MaterialManagement_IntegrationSearch + ",text:'" + "物资信息库" + "',leaf:true}"
			;
			
		}else if(WebKeys.Tree_Menu_Name_MeetingMenu.equals(treeName)){
			menuStr += "{id:" + WebKeys.Menu_ID_MeetingManagement_MeetingStart + ",text:'" + "会议发起" + "',leaf:true}"
			;

		}else if(WebKeys.Tree_Menu_Name_CrmMenu.equals(treeName)){
			menuStr += "{id:" + WebKeys.Menu_ID_CrmManagement_CRMEntry + ",text:'" + "客户信息登记" + "',leaf:true}"
			;

		}else if(WebKeys.Tree_Menu_Name_IssueMenu.equals(treeName)){
			menuStr += "{id:" + WebKeys.Menu_ID_IssueManagement_IssueEntry + ",text:'" + "缺陷登记" + "',leaf:true}"
			;

		}else if(WebKeys.Tree_Menu_Name_OrganizationMenu.equals(treeName)){
				menuStr += "{id:" + WebKeys.Menu_ID_OrganizationManagement_OrganizationTree + ",text:'" + "组织架构" + "',leaf:true},"
				+  "{id:" + WebKeys.Menu_ID_OrganizationManagement_GoOutEentry + ",text:'" + "外出登记" + "',leaf:true},"
				+  "{id:" + WebKeys.Menu_ID_OrganizationManagement_BusinessTripEntry + ",text:'" + "出差登记" + "',leaf:true},"
				+  "{id:" + WebKeys.Menu_ID_OrganizationManagement_DailyRecord + ",text:'" + "工作日志" + "',leaf:true},"
				+  "{id:" + WebKeys.Menu_ID_OrganizationManagement_VacationRequist + ",text:'" + "休假申请" + "',leaf:true},"
				+  "{id:" + WebKeys.Menu_ID_OrganizationManagement_UserRightRequist + ",text:'" + "权限申请" + "',leaf:true}"
				;

		}else if(WebKeys.Tree_Menu_Name_ProductionMenu.equals(treeName)){
			menuStr += "{id:" + WebKeys.Menu_ID_ProductionManagement_NewProduction + ",text:'" + "新产品开发" + "',leaf:true},"
			+  "{id:" + WebKeys.Menu_ID_ProductionManagement_SubsequentDevelop + ",text:'" + "后续开发" + "',leaf:true},"
			+  "{id:" + WebKeys.Menu_ID_ProductionManagement_ProductionRelease + ",text:'" + "产品发布" + "',leaf:true}"
			;

		}else if(WebKeys.Tree_Menu_Name_WiKiMenu.equals(treeName)){
			menuStr += "{id:" + WebKeys.Menu_ID_WiKiManagement_WikiEntry + ",text:'" + "新数据添加" + "',leaf:true},"
			+  "{id:" + WebKeys.Menu_ID_WiKiManagement_IntegrationSearch + ",text:'" + "知识库查询" + "',leaf:true}"
			;

		}else if(WebKeys.Tree_Menu_Name_SuggestionMenu.equals(treeName)){
			menuStr += "{id:" + WebKeys.Menu_ID_SuggestionManagement_SuggestionEntry + ",text:'" + "信息采集" + "',leaf:true}"
			;

		}
		
		return menuStr + "]";
	}

	/**
	 * @param userID
	 * @return List
	 * @roseuid 4C198C3B0128
	 */
	public List getUserRight(int userID) {
		return null;
	}

	private void doLunchAddrssBook() {

	}

	public OrganizationDAO getOrganizationDAO() {
		return organizationDAO;
	}

	public void setOrganizationDAO(OrganizationDAO organizationDAO) {
		this.organizationDAO = organizationDAO;
	}
}
