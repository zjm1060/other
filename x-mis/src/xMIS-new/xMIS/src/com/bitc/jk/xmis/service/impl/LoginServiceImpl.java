//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\service\\Impl\\LoginServiceImpl.java

package com.bitc.jk.xmis.service.impl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import javax.servlet.ServletContext;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpSession;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;

import com.bitc.jk.xmis.dao.ContractDAO;

import com.bitc.jk.xmis.dao.OrganizationDAO;
import com.bitc.jk.xmis.dao.SysLogDAO;
import com.bitc.jk.xmis.dao.UserHabitDAO;
import com.bitc.jk.xmis.model.ContractObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.security.BizDataScopeObj;
import com.bitc.jk.xmis.model.security.BizFunctionObj;
import com.bitc.jk.xmis.model.security.BizModuleObj;
import com.bitc.jk.xmis.model.security.BizOperationObj;
import com.bitc.jk.xmis.model.security.BizSubModuleObj;
import com.bitc.jk.xmis.model.sys.SysLog;
import com.bitc.jk.xmis.model.sys.UserHabit;
import com.bitc.jk.xmis.service.LoginService;
import com.bitc.jk.xmis.util.DBSchema;
import com.bitc.jk.xmis.util.DateUtil;
import com.bitc.jk.xmis.util.QueryParam;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.CompositParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public class LoginServiceImpl implements LoginService {
	private static Logger logger = Logger.getLogger(LoginServiceImpl.class);

	private OrganizationDAO organizationDAO;

	private UserHabitDAO userHabitDAO;

	private SysLogDAO sysLogDAO;

	private ContractDAO contractDAO;

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
	public EmployeeObj doUserValidete(String loginName, String password,
			HttpServletRequest request) throws Exception {
		try {
			EmployeeObj emp = organizationDAO.userValidete(loginName, password);
			if (emp == null) {
				throw new Exception("登录验证错误");
			}
			// get left menu list by userRights
			emp.setUserRightsMap(getUserRightsOnFunctionLevel(emp.getUserID()));

			emp.setUserRightsListByDefault(organizationDAO
					.getUserRightByDefault());

			// get user roles

			if (!emp.isLocked()) {
				request.getSession().setAttribute(
						WebKeys.Session_Attribute_Name_UserDNA, emp);
				if (organizationDAO.getDeptOfUserLimit(emp.getDepartmentID())) {
					request.getSession().setAttribute("searchProfessionLimits",
							"PASS");
				}
				// get role
				emp.setRoles(organizationDAO.getUserRoles(emp.getUserID()));
				initUserHabit(emp, request);
				initLoginInfo(emp, request.getSession());
				doSaveLoginLog(request, emp.getUserID());

				logger.info(emp.isReadOnly());
			}

			return emp;
		} catch (Exception e) {
			e.printStackTrace();
			throw e;
		}
	}

	@SuppressWarnings("rawtypes")
	private void initLoginInfo(EmployeeObj emp, HttpSession session) {
		List list = new ArrayList();
		int userID = emp.getUserID();
		list = sysLogDAO.getLastLogTime(userID);
		if (list == null || list.size() == 0) {
			return;
		}
		Object[] obj = (Object[]) list.get(0);
		session.setAttribute(WebKeys.Session_Attribute_LOGIN_COUNT, obj[0]);
		session.setAttribute(WebKeys.Session_Attribute_LAST_LOGIN_TIME, obj[1]);
	}

	private void doSaveLoginLog(HttpServletRequest request, int userID) {
		String pagex = request.getRequestURI();
		String uri = pagex.substring(request.getContextPath().length());
		String opUri = uri
				+ StringUtils.defaultIfEmpty(request.getQueryString(), "");
		String opIP = WebUtil.getIpAddr(request);
		SysLog sysLog = new SysLog(String.valueOf(userID), opUri, opIP);
		sysLog.setOpType(SysLog.OP_TYPE_LOGIN);
		try {
			doSaveSysLog(sysLog);
		} catch (Exception e) {
			logger.error(e.getMessage());
		}
		HttpSession session = request.getSession();
		ServletContext context = session.getServletContext();
		Integer count = (Integer) context
				.getAttribute(WebKeys.Session_Application_Attr_CountOfOnline);

		if (count == null) {
			count = 1;
			context.setAttribute(
					WebKeys.Session_Application_Attr_CountOfOnline, count);
		} else {
			synchronized (count) {
				count = count + 1;
			}
		}
	}

	private void initUserHabit(EmployeeObj userObj, HttpServletRequest request) {
		try {
			UserHabit habit = getUserHabit(userObj);
			request.getSession().setAttribute(
					WebKeys.Session_Attribute_Name_UserHabit, habit);
		} catch (Exception e) {
			logger.error("获取用户习惯数据出错。\n");
			logger.error(e.getMessage());
		}
	}

	private Map getUserRightsOnFunctionLevel(int userID) {

		List aList = organizationDAO.getUserRightOnFunctionLevel(userID);

		Map userRightsOnModuleMap = new HashMap();

		if (aList != null && aList.size() > 0) {

			for (Iterator i = aList.listIterator(); i.hasNext();) {

				Map map = (Map) i.next();

				int mapsize = map.size();
				Iterator it = map.entrySet().iterator();

				BizModuleObj module = new BizModuleObj();
				BizSubModuleObj subModule = new BizSubModuleObj();
				BizFunctionObj function = new BizFunctionObj();
				BizOperationObj operation = new BizOperationObj();

				for (int j = 0; j < mapsize; j++) {

					Map.Entry entry = (Map.Entry) it.next();

					if (entry.getKey().equals(DBSchema.BizModule.BizModuleID))
						module.setId(Type.GetInt(entry.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizModule.BizModuleName))
						module.setName(Type.getString(entry.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizModule.BizModuleDesc))
						module.setDesc(Type.getString(entry.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizModule.BizModuleURL))
						module.setUrl(Type.getString(entry.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizModule.BizModuleIconCls))
						module.setIconCls(Type.getString(entry.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizModule.BizModuleSquenceNo))
						module.setSequenceNo(Type.GetInt(entry.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizModule.BizModuleIdentifies))
						module.setIdentifies(Type.getString(entry.getValue()));

					else if (entry.getKey().equals(
							DBSchema.BizSubModule.BizSubModuleID))
						subModule.setId(Type.GetInt(entry.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizSubModule.BizSubModuleName))
						subModule.setName(Type.getString(entry.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizSubModule.BizSubModuleDesc))
						subModule.setDesc(Type.getString(entry.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizSubModule.BizSubModuleURL))
						subModule.setUrl(Type.getString(entry.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizSubModule.BizSubModulesIconCls))
						subModule.setIconCls(Type.getString(entry.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizModuleMapping.BizSubModulesSquenceNo))
						subModule.setSequenceNo(Type.GetInt(entry.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizSubModule.BizSubModuleIdentifies))
						subModule
								.setIdentifies(Type.getString(entry.getValue()));

					else if (entry.getKey().equals(
							DBSchema.BizFunction.BizFunctionSqueID))
						function.setId(Type.GetInt(entry.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizFunction.BizFunctionName))
						function.setName(Type.getString(entry.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizFunction.BizFunctionDesc))
						function.setDesc(Type.getString(entry.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizFunction.BizFunctionURL))
						function.setUrl(Type.getString(entry.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizFunction.BizFunctionIdentifies))
						function.setIdentifies(Type.getString(entry.getValue()));

					else if (entry.getKey().equals(
							DBSchema.BizOperationRight.RightID))
						operation.setOperationID(Type.GetInt(entry.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizOperationRight.RightName))
						operation.setOperationName(Type.getString(entry
								.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizOperationRight.RightDesc))
						operation.setOperationDesc(Type.getString(entry
								.getValue()));

				}

				function.setBizOperation(operation);

				try {

					BizModuleObj existModule = (BizModuleObj) userRightsOnModuleMap
							.get(WebKeys.Biz_Module_Prefix
									+ Type.getString(module.getId()));
					try {
						BizSubModuleObj existSubModule = (BizSubModuleObj) existModule
								.getBizObjMap().get(
										WebKeys.Biz_SubModule_Prefix
												+ Type.getString(subModule
														.getId()));
						((BizSubModuleObj) (((BizModuleObj) userRightsOnModuleMap
								.get(WebKeys.Biz_Module_Prefix
										+ Type.getString(module.getId())))
								.getBizObjMap()
								.get(WebKeys.Biz_SubModule_Prefix
										+ Type.getString(subModule.getId()))))
								.addBizObjMap(WebKeys.Biz_Function_Prefix
										+ Type.getString(function.getId()),
										function);

					} catch (Exception e) {
						subModule.addBizObjMap(WebKeys.Biz_Function_Prefix
								+ Type.getString(function.getId()), function);
						((BizModuleObj) userRightsOnModuleMap
								.get(WebKeys.Biz_Module_Prefix
										+ Type.getString(module.getId())))
								.getBizObjMap().put(
										WebKeys.Biz_SubModule_Prefix
												+ Type.getString(subModule
														.getId()), subModule);
					}
				} catch (Exception e) {
					subModule.addBizObjMap(
							WebKeys.Biz_Function_Prefix
									+ Type.getString(function.getId()),
							function);
					module.addBizObjMap(
							WebKeys.Biz_SubModule_Prefix
									+ Type.getString(subModule.getId()),
							subModule);
					userRightsOnModuleMap.put(
							WebKeys.Biz_Module_Prefix
									+ Type.getString(module.getId()), module);
				}

			}

		}

		return userRightsOnModuleMap;
	}

	public String buildLeftMenu(String treeName, EmployeeObj userDNA) {

		String menuStr = "[";

		if (WebKeys.Tree_Menu_Name_myDeskMenu.equals(treeName)) {
			menuStr += "{id:" + WebKeys.Menu_ID_myDesk_MyHomePage + ",text:'"
					+ "My Home Page" + "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_myDesk_WhatSNew + ",text:'"
					+ "What\\'s New?" + "',leaf:true}";

		} else if (WebKeys.Tree_Menu_Name_myTaskMenu.equals(treeName)) {
			menuStr += "{id:" + WebKeys.Menu_ID_myTask_TaskReceipt + ",text:'"
					+ "接收任务" + "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_myTask_TaskExecute + ",text:'" + "执行任务"
					+ "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_myTask_TaskHistory + ",text:'" + "历史任务"
					+ "',leaf:true}";

		} else if (WebKeys.Tree_Menu_Name_ProjectMenu.equals(treeName)) {
			menuStr += "{id:" + WebKeys.Menu_ID_ProjectManagement_ProjectStart
					+ ",text:'" + "项目启动" + "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_ProjectManagement_TaskAssign + ",text:'"
					+ "任务分配" + "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_ProjectManagement_InnerAcceptance
					+ ",text:'" + "内部验收" + "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_ProjectManagement_ExternalAcceptance
					+ ",text:'" + "外部验收" + "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_ProjectManagement_ContractChange
					+ ",text:'" + "合同变更" + "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_ProjectManagement_ProjectMaint
					+ ",text:'" + "项目维护" + "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_ProjectManagement_LocaleReport
					+ ",text:'" + "现场工作报告" + "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_ProjectManagement_CarryOverQuestion
					+ ",text:'" + "遗留问题处理" + "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_ProjectManagement_IntegrationSearch
					+ ",text:'" + "项目综合查询" + "',leaf:true}";

		} else if (WebKeys.Tree_Menu_Name_ContractMenu.equals(treeName)) {
			menuStr += "{id:"
					+ WebKeys.Menu_ID_ContractManagement_ContractEntry
					+ ",text:'"
					+ "合同录入"
					+ "',leaf:true,url:'contractMain.do?method=contract'},"
					+ "{id:"
					+ WebKeys.Menu_ID_ContractManagement_InvoiceEntry
					+ ",text:'"
					+ "票据收发登记"
					+ "',leaf:true,url:'contractMain.do?method=invoiceEdit'},"
					+ "{id:"
					+ WebKeys.Menu_ID_ContractManagement_PaymentEntry
					+ ",text:'"
					+ "合同款收付登记"
					+ "',leaf:true,url:'contractMain.do?method=paymentListEdit'},"
					+ "{id:"
					+ WebKeys.Menu_ID_ContractManagement_IntegrationSearch
					+ ",text:'"
					+ "合同综合查询"
					+ "',leaf:true,url:'contractMain.do?method=getContractList&searchtype=integration'}";

		} else if (WebKeys.Tree_Menu_Name_MaterialMenu.equals(treeName)) {
			menuStr += "{id:"
					+ WebKeys.Menu_ID_MaterialManagement_DevicePurchase
					+ ",text:'" + "设备采购" + "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_MaterialManagement_DeviceWarehouseEntry
					+ ",text:'" + "设备入库登记" + "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_MaterialManagement_DeviceReceive
					+ ",text:'" + "设备领用" + "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_ContractManagement_InvoiceEntry
					+ ",text:'" + "票据收发登记" + "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_MaterialManagement_DeviceReturnChange
					+ ",text:'" + "返修更换" + "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_MaterialManagement_IntegrationSearch
					+ ",text:'" + "物资信息库" + "',leaf:true}";

		} else if (WebKeys.Tree_Menu_Name_MeetingMenu.equals(treeName)) {
			menuStr += "{id:" + WebKeys.Menu_ID_MeetingManagement_MeetingStart
					+ ",text:'" + "会议发起" + "',leaf:true}";

		} else if (WebKeys.Tree_Menu_Name_CrmMenu.equals(treeName)) {
			menuStr += "{id:" + WebKeys.Menu_ID_CrmManagement_CRMEntry
					+ ",text:'" + "客户信息登记" + "',leaf:true}";

		} else if (WebKeys.Tree_Menu_Name_IssueMenu.equals(treeName)) {
			menuStr += "{id:" + WebKeys.Menu_ID_IssueManagement_IssueEntry
					+ ",text:'" + "缺陷登记" + "',leaf:true}";

		} else if (WebKeys.Tree_Menu_Name_OrganizationMenu.equals(treeName)) {
			menuStr += "{id:"
					+ WebKeys.Menu_ID_OrganizationManagement_OrganizationTree
					+ ",text:'" + "组织架构" + "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_OrganizationManagement_GoOutEentry
					+ ",text:'" + "外出登记" + "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_OrganizationManagement_BusinessTripEntry
					+ ",text:'" + "出差登记" + "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_OrganizationManagement_DailyRecord
					+ ",text:'" + "工作日志" + "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_OrganizationManagement_VacationRequist
					+ ",text:'" + "休假申请" + "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_OrganizationManagement_UserRightRequist
					+ ",text:'" + "权限申请" + "',leaf:true}";

		} else if (WebKeys.Tree_Menu_Name_ProductionMenu.equals(treeName)) {
			menuStr += "{id:"
					+ WebKeys.Menu_ID_ProductionManagement_NewProduction
					+ ",text:'" + "新产品开发" + "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_ProductionManagement_SubsequentDevelop
					+ ",text:'" + "后续开发" + "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_ProductionManagement_ProductionRelease
					+ ",text:'" + "产品发布" + "',leaf:true}";

		} else if (WebKeys.Tree_Menu_Name_WiKiMenu.equals(treeName)) {
			menuStr += "{id:" + WebKeys.Menu_ID_WiKiManagement_WikiEntry
					+ ",text:'" + "新数据添加" + "',leaf:true}," + "{id:"
					+ WebKeys.Menu_ID_WiKiManagement_IntegrationSearch
					+ ",text:'" + "知识库查询" + "',leaf:true}";

		} else if (WebKeys.Tree_Menu_Name_SuggestionMenu.equals(treeName)) {
			menuStr += "{id:"
					+ WebKeys.Menu_ID_SuggestionManagement_SuggestionEntry
					+ ",text:'" + "信息采集" + "',leaf:true}";

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

	public UserHabit getUserHabit(EmployeeObj userObj) {
		UserHabit habit = userHabitDAO.getUserHabit(userObj.getUserID());
		return habit;
	}

	public void doSaveUserHabit(HttpServletRequest request, EmployeeObj userObj) {
		UserHabit habit = (UserHabit) request.getSession().getAttribute(
				WebKeys.Session_Attribute_Name_UserHabit);
		if (habit == null) {
			return;
		}
		UserHabit h = userHabitDAO.getUserHabit(userObj.getUserID());
		if (h != null) {
			h.setHabit(habit.getHabit());
			userHabitDAO.doUpdateUserHabit(h);
		} else {
			userHabitDAO.doSaveUserHabit(habit);
		}
	}

	public void doSaveSysLog(SysLog sysLog) {
		sysLogDAO.saveSysLog(sysLog);
	}

	public UserHabitDAO getUserHabitDAO() {
		return userHabitDAO;
	}

	public void setUserHabitDAO(UserHabitDAO userHabitDAO) {
		this.userHabitDAO = userHabitDAO;
	}

	public Map<String, List> getSecrityDataScope(String url,
			EmployeeObj userDNA, boolean recFlag) {

		Map<String, List> aMap = new HashMap<String, List>();

		Map bizMap = new HashMap<String, List>();

		bizMap.put(WebKeys.Biz_Logic_Department_Type, new ArrayList());
		bizMap.put(WebKeys.Biz_Logic_User_Type, new ArrayList());
		bizMap.put(WebKeys.Biz_Logic_Opration_Type, new ArrayList());

		List aList = organizationDAO.getSecrityDataScope(url,
				userDNA.getUserID());

		// Map dataDefiniensMap = new HashMap();

		List<QueryParam> paramList = new ArrayList<QueryParam>();
		List<QueryParam> paramListForOpType = new ArrayList<QueryParam>();
		List<String> paramListForOpTypeStr = new ArrayList<String>();

		String queryParamStr = "";

		String queryParamStrForOpType = "";

		if (aList != null && aList.size() > 0) {

			for (Iterator i = aList.listIterator(); i.hasNext();) {

				Map map = (Map) i.next();

				int mapsize = map.size();
				Iterator it = map.entrySet().iterator();

				BizDataScopeObj dataObj = new BizDataScopeObj();
				dataObj.setBizLogicMap(bizMap);

				for (int j = 0; j < mapsize; j++) {

					Map.Entry entry = (Map.Entry) it.next();

					if (entry.getKey().equals(DBSchema.BizDataDefiniens.DataID))// 数据字段ID
						dataObj.setDataDefinitionID(Type.GetInt(entry
								.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizDataDefiniens.DataName))// 数据字段名称
						dataObj.setDataDefinitionName(Type.getString(entry
								.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizDataDefiniens.DataAlias))// 数据字段别名
						dataObj.setDataAlias(Type.getString(entry.getValue()));
					else if (entry.getKey().equals(
							DBSchema.DataTypeDefiniens.DataTypeName))// 数据字段类型名称
						dataObj.setDataTypeDefinition(Type.getString(entry
								.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizDataDefiniens.DataDefaultValue))// 缺省值描述
						dataObj.setDataDefaultValue(Type.getString(entry
								.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizDataScopeRight.DataValue))// 数据字段值
						dataObj.setDataValue(Type.getString(entry.getValue()));
					else if (entry.getKey().equals(
							DBSchema.RelationalOperateDefiniens.OpName))// 数据运算符
						dataObj.setOperationDefinition(Type.getString(entry
								.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizDataScopeRight.IsSensitive))// 是否是敏感字段
						dataObj.setSensitive(Type.getBoolean(entry.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizDataScopeRight.NeedParsed))// 是否需要解析
						dataObj.setNeedParsed(Type.getBoolean(entry.getValue()));
					else if (entry.getKey().equals(
							DBSchema.BizDataDefiniens.DataBizlogicID))// 业务逻辑判断
						dataObj.setDataBizlogicID(Type.GetInt(entry.getValue()));
					else if (entry.getKey().equals(
							DBSchema.UserRoleRightsMapping.BizOperationID))// 操作权限ID
																			// 只读,修改
						dataObj.setOperationID(Type.GetInt(entry.getValue()));

				}

				if (dataObj.isNeedParsed())// 是否需要解析
					dataObj.setDataValue(parsedDataValue(dataObj, userDNA,
							false));

				else if (dataObj.getDataBizlogicID() == 1) {// 业务逻辑判断
					// dataObj.getDataBizlogicID() == 1 是按部门ID 处理

					Map<String, List> bMap = dataObj.getBizLogicMap();

					List bList = dataObj.getBizLogicMap().get(
							WebKeys.Biz_Logic_Department_Type);

					bList.add(dataObj.getDataValue());

					bMap.remove(WebKeys.Biz_Logic_Department_Type);

					bMap.put(WebKeys.Biz_Logic_Department_Type, bList);

					dataObj.setBizLogicMap(bMap);

				}
				if (dataObj.getOperationID() == 5) {
					// 操作权限为全控制的条件

					// 构造条件子句queryParamStr
					dataObj.buildSubSql();// subSqlStr

					dataObj.mergeSubStr(queryParamStrForOpType);

					queryParamStrForOpType = dataObj.getSubSqlStr();

				}

				dataObj.buildSubSql();// subSqlStr

				dataObj.mergeSubStr(queryParamStr);

				queryParamStr = dataObj.getSubSqlStr();

				bizMap = dataObj.getBizLogicMap();

			}
		}

		aMap.put(WebKeys.Request_Attribute_Name_DataScopeDeptList,
				getBizlogicList(bizMap, WebKeys.Biz_Logic_Department_Type));

		// 如果当前用户没有操作权限为全控制的条件限制，opType=5的权限定义缺省为1=2，
		// 则改用户无可编辑合同
		paramListForOpType.add(QueryParam.buildQueryParam(""
				.equals(queryParamStrForOpType) ? "1=2"
				: queryParamStrForOpType, ""));

		aMap.put(WebKeys.Request_Attribute_Name_DataScopeOpTypeList,
				paramListForOpType);

		// paramListForOpTypeStr.add(queryParamStrForOpType);
		// aMap.put(WebKeys.Request_Attribute_Name_DataScopeOpTypeStr,
		// paramListForOpTypeStr);

		paramList.add(QueryParam.buildQueryParam(queryParamStr, ""));
		aMap.put(WebKeys.Request_Attribute_Name_DataScopeDefiniensList,
				paramList);

		return aMap;
	}

	private List getBizlogicList(Map bizMap, String bizLogicType) {
		// 权限中，部门ID的处理 （bizLogicType = Biz_Logic_Department_Type）

		List bizLogicList = new ArrayList();
		List bizList = (ArrayList) bizMap.get(bizLogicType);
		String dataScopeStr = "";

		if (bizList.size() > 0) {
			for (Iterator i = bizList.listIterator(); i.hasNext();) {
				dataScopeStr += Type.getString(i.next()) + ",";

			}

			bizLogicList = organizationDAO
					.getBizloigcDataList(
							StringUtils.substring(dataScopeStr, 0,
									StringUtils.length(dataScopeStr) - 1),
							bizLogicType);

			// for(Iterator i = aList.iterator(); i.hasNext();){
			// Map map = (Map) i.next();
			//
			// int mapsize = map.size();
			// Iterator it = map.entrySet().iterator();
			//
			// Object bizObj = new Object();
			// for (int j = 0; j < mapsize; j++) {
			//
			// Map.Entry entry = (Map.Entry) it.next();
			//
			// if(bizLogicType.equals(WebKeys.Biz_Logic_Department_Type)){
			//
			// if(entry.getKey().equals(DBSchema.Department.DepartmentID))
			// ((DepartmentObj)bizObj).setDepartmentID(Type.GetInt(entry.getValue()));
			// else
			// if(entry.getKey().equals(DBSchema.Department.DepartmentName))
			// ((DepartmentObj)bizObj).setDepartmentName(Type.getString(entry.getValue()));
			// }
			// }
			//
			// bizLogicList.add(((DepartmentObj)bizObj));
			//
			// }

		}

		if (WebKeys.Biz_Logic_Department_Type.equals(bizLogicType)
				&& bizLogicList.size() <= 2)
			bizLogicList = new ArrayList();

		return bizLogicList;
	}

	private String parsedDataValue(BizDataScopeObj dataObj,
			EmployeeObj userDNA, boolean recFlag) {

		logger.info("dataValue === " + dataObj.getDataValue());

		Map bizMap = dataObj.getBizLogicMap();

		if (dataObj.getDataValue().equals("self-deptID")) {
			dataObj.setDataValue(Type.getString(userDNA.getDepartmentID()));

			List aList = (ArrayList) bizMap
					.get(WebKeys.Biz_Logic_Department_Type);
			aList.add(userDNA.getDepartmentID());
			bizMap.remove(WebKeys.Biz_Logic_Department_Type);
			bizMap.put(WebKeys.Biz_Logic_Department_Type, aList);
		} else if (dataObj.getDataValue().equals("self-userID")) {
			dataObj.setDataValue(Type.getString(userDNA.getUserID()));

			List aList = (ArrayList) bizMap.get(WebKeys.Biz_Logic_User_Type);
			aList.add(userDNA.getUserID());
			bizMap.remove(WebKeys.Biz_Logic_User_Type);
			// bizMap.put(WebKeys.Biz_Logic_Department_Type, aList);
			bizMap.put(WebKeys.Biz_Logic_User_Type, aList);

		} else if (dataObj.getDataValue().equals("self-divID")) {

			List bList = (ArrayList) bizMap
					.get(WebKeys.Biz_Logic_Department_Type);

			String val = "(" + userDNA.getDivisionID() + " ";
			bList.add(userDNA.getDivisionID());// bList.add事业部ID

			List aList = organizationDAO// SELECT * FROM xmis.org_部门归属部门表 WHERE
										// 父部门ID = ?
					.getDeptListByID(userDNA.getDivisionID(),
							WebKeys.Organization_Type_Division);

			if (aList != null && aList.size() > 0) {

				for (Iterator i = aList.listIterator(); i.hasNext();) {

					Map map = (Map) i.next();

					int mapsize = map.size();
					Iterator it = map.entrySet().iterator();

					for (int j = 0; j < mapsize; j++) {
						Map.Entry entry = (Map.Entry) it.next();

						if (entry.getKey().equals(
								DBSchema.DeptBelongToDept.DeptID)) {
							val += Type.getString(entry.getValue()) + " ";

							bList.add(Type.GetInt(entry.getValue()));// bList.add子部门
						}
					}
				}
			}
			dataObj.setDataValue(StringUtils.replace(val.trim(), " ", ",")
					+ ")");

			bizMap.remove(WebKeys.Biz_Logic_Department_Type);
			bizMap.put(WebKeys.Biz_Logic_Department_Type, bList);

		} else if (dataObj.getDataValue().equals("self-compID")) {

			List bList = (ArrayList) bizMap
					.get(WebKeys.Biz_Logic_Department_Type);

			String val = "(" + userDNA.getDivisionID() + " ";
			bList.add(userDNA.getDivisionID());// bList.add事业部ID

			List aList = organizationDAO.getDeptListByID(// SELECT * FROM
															// xmis.org_部门归属公司表
															// WHERE 公司ID = ?
					userDNA.getCompanyID(), WebKeys.Organization_Type_Company);

			if (aList != null && aList.size() > 0) {

				for (Iterator i = aList.listIterator(); i.hasNext();) {

					Map map = (Map) i.next();

					int mapsize = map.size();
					Iterator it = map.entrySet().iterator();

					for (int j = 0; j < mapsize; j++) {
						Map.Entry entry = (Map.Entry) it.next();

						if (entry
								.getKey()
								.equals(DBSchema.DepartmentBelongToCompany.DepartmentID)) {
							val += Type.getString(entry.getValue()) + " ";

							bList.add(Type.GetInt(entry.getValue()));// 公司下的各部门
						}

					}
				}
			}
			dataObj.setDataValue(StringUtils.replace(val.trim(), " ", ",")
					+ ")");

			bizMap.remove(WebKeys.Biz_Logic_Department_Type);
			bizMap.put(WebKeys.Biz_Logic_Department_Type, bList);

		} else if (dataObj.getDataValue().equals("in_contractList")) {
			// 根据合同查询权限，获得可查询的合同ID列表

			if (recFlag == false) {
				Map<String, List> dataScopeListMap = getSecrityDataScope(
						"/contractMain.do?method=getContractListByType&contractTypeID=1",
						userDNA, true);

				List<QueryParam> paramList = (List) dataScopeListMap
						.get(WebKeys.Request_Attribute_Name_DataScopeDefiniensList);

				List<StandardQuery> querys = WebUtil
						.buildStandardQuery(paramList);

				String sql = new String();
				for (StandardQuery query : querys) {
					sql = query.getJdbcSQL();

					if (query instanceof CompositParam) {
						sql = " where " + sql;
					}
				}

				// 构造where条件 子查询 子句
				dataObj.setDataValue(" (SELECT 合同ID FROM xmis.con_合同  where "
						+ sql + ")");

				// logger.info(dataObj.getDataValue());

			}

		} else if (dataObj.getDataValue().equals("prjIDList_in_contractScope")) {
			// 根据合同查询权限，获得可查询的项目ID列表

			BizDataScopeObj newDataObj = new BizDataScopeObj();

			newDataObj.setBizLogicMap(dataObj.getBizLogicMap());
			newDataObj.setDataValue("in_contractList");

			String dataValue = parsedDataValue(newDataObj, userDNA, recFlag);

			dataObj.setDataValue(" (SELECT 项目ID FROM xmis.prj_项目 WHERE 项目合同ID IN "
					+ dataValue + ")");

		} else if (dataObj.getDataValue().equals("self_prjMember")) {
			// 根据项目成员定义，获得可查询的项目ID列表

			dataObj.setDataValue(" (SELECT 项目ID FROM xmis.prj_项目任务成员, xmis.prj_项目任务表 WHERE 项目成员ID = "
					+ Type.getString(userDNA.getUserID())
					+ " AND xmis.prj_项目任务成员.项目任务ID = xmis.prj_项目任务表.项目任务ID  ) ");

		}

		// logger.info(dataObj.getDataValue());

		return dataObj.getDataValue();

	}

	public SysLogDAO getSysLogDAO() {
		return sysLogDAO;
	}

	public void setSysLogDAO(SysLogDAO sysLogDAO) {
		this.sysLogDAO = sysLogDAO;
	}

	public ContractDAO getContractDAO() {
		return contractDAO;
	}

	/**
	 * Sets the value of the contractDAO property.
	 * 
	 * @param aContractDAO
	 *            the new value of the contractDAO property
	 */
	public void setContractDAO(ContractDAO aContractDAO) {
		contractDAO = aContractDAO;
	}

	public int getContractOpType(EmployeeObj userDNA, ContractObj contract) {

		String url = "";

		// 按合同类型采用的相应合同列表的数据权限 url为改合同的权限
		if (contract.getContractTypeID() == 1)
			url = "/contractMain.do?method=getContractListByType&contractTypeID=1";
		else
			url = "/contractMain.do?method=getContractListByType&contractTypeID=2";

		Map<String, List> dataScopeListMap = getSecrityDataScope(url, userDNA,
				false);

		logger.info(dataScopeListMap
				.get(WebKeys.Request_Attribute_Name_DataScopeOpTypeList));

		List<QueryParam> paramList = (List) dataScopeListMap
				.get(WebKeys.Request_Attribute_Name_DataScopeOpTypeList);

		// //begin
		// List<String> paramListStr = (List) dataScopeListMap
		// .get(WebKeys.Request_Attribute_Name_DataScopeOpTypeStr);
		//
		// logger.info((String)paramListStr.get(0));
		// String paramStr = (String)paramListStr.get(0) + " and (合同ID = "
		// +contract.getContractID() + " or 父合同ID  = " +
		// contract.getContractID() + ")";
		// List<QueryParam> paramList = new ArrayList<QueryParam>();
		// paramList.add(QueryParam.buildQueryParam(paramStr, ""));
		// //end

		List<StandardQuery> querys = WebUtil.buildStandardQuery(paramList);

		boolean hasFullRights = contractDAO.hasFullRightsOnConcrat(
				contract.getContractID(), contract.getContractTypeID(), querys);

		return hasFullRights == true ? 5 : 0;

	}

}
