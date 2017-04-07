//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\service\\Impl\\OrganizationServiceImpl.java
package com.bitc.jk.xmis.service.impl;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.sql.Date;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.Set;

import net.sf.json.JSON;
import net.sf.json.JSONArray;
import net.sf.json.JsonConfig;
import net.sf.json.util.CycleDetectionStrategy;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.dom4j.Attribute;
import org.dom4j.Document;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;
import org.dom4j.io.OutputFormat;
import org.dom4j.io.XMLWriter;

import com.bitc.jk.xmis.dao.CrmDAO;
import com.bitc.jk.xmis.dao.OrganizationDAO;
import com.bitc.jk.xmis.model.CompanyObj;
import com.bitc.jk.xmis.model.CompanySimpleObj;
import com.bitc.jk.xmis.model.DepartmentObj;
import com.bitc.jk.xmis.model.DepartmentTree;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.EmployeePost;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.SimpleDeptObj;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.model.T_OnWatch_schedule;
import com.bitc.jk.xmis.model.crm.Address;
import com.bitc.jk.xmis.model.sys.DisabledReason;
import com.bitc.jk.xmis.model.sys.Post;
import com.bitc.jk.xmis.service.OrganizationService;
import com.bitc.jk.xmis.util.DBSchema;
import com.bitc.jk.xmis.util.JSONUtils;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.FTPUploadFileUtil;
import com.bitc.jk.xmis.util.UUIDUtils;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.web.InitActionServlet;
import com.bitc.jk.xmis.web.SpringInit;
import com.bitc.jk.xmis.web.action.param.OrgTreeParam;
import com.bitc.jk.xmis.web.response.SyncTreeNode;
import com.bitc.jk.xmis.web.response.TreeLoaderResponse;
import com.bitc.jk.xmis.web.response.TreeNode;
import com.bitc.jk.xmis.web.response.TreeNodeAttributor;

//@Transactional(readOnly = true)
public class OrganizationServiceImpl implements OrganizationService {
	private static Logger logger = Logger
			.getLogger(OrganizationServiceImpl.class);

	private OrganizationDAO organizationDAO;
	private CrmDAO crmDAO;
	//2013-11-06 zentao代码注释	
/*	private ZentaoDAO zentaoDAO;
	public ZentaoDAO getZentaoDAO() {
		return zentaoDAO;
	}
	
	public void setZentaoDAO(ZentaoDAO zentaoDAO) {
		this.zentaoDAO = zentaoDAO;
	}*/

	private String webDir;

	private String webPath;

	/**
	 * @roseuid 4C1F03AE03D8
	 */
	public OrganizationServiceImpl() {

	}

	public void setWebDir(String aWebDir) {

		this.webDir = aWebDir;

	}

	public CrmDAO getCrmDAO() {
		return crmDAO;
	}

	public void setCrmDAO(CrmDAO crmDAO) {
		this.crmDAO = crmDAO;
	}

	public void setOrganizationDAO(OrganizationDAO aOrganizationDAO) {
		this.organizationDAO = aOrganizationDAO;
	}

	public OrganizationDAO getOrganizationDAO() {
		return organizationDAO;
	}

	/**
	 * @param companyID
	 * @return List
	 * @roseuid 4C1F03AF0000
	 */
	public void buildDepartmentTree(EmployeeObj userObj) {

		List deptTreeList = new ArrayList();
		List aList = organizationDAO.getDepartmentTree(userObj.getDivisionID());

		DepartmentTree deptTree;

		for (Iterator i = aList.listIterator(); i.hasNext();) {
			deptTree = new DepartmentTree();
			Map map = (HashMap) i.next();
			deptTree.setDepartID(((Integer) map.get("dept_id")).intValue());
			deptTree.setDepartName((String) map.get("dept_name"));
			deptTree.setUserID(map.get("user_id") == null ? 0 : ((Integer) map
					.get("user_id")).intValue());
			deptTree.setUserName(map.get("user_name") == null ? ""
					: (String) map.get("user_name"));
			deptTree.setPosition(map.get("position") == null ? ""
					: (String) map.get("position"));

			deptTree.setDeptDisabledFlag(Type.convertBinToInt(map
					.get("dept_disabled")));
			deptTree.setEmpDisabledFlag(Type.convertBinToInt(map
					.get("emp_disabled")));

			// logger.info(deptTree.getPosition());

			deptTreeList.add(deptTree);
		}

		this.buildXML(userObj, deptTreeList);

	}

	/**
	 * @param empID
	 *            ;int
	 * @return com.bitc.jk.xmis.model.EmployeeObj
	 * @roseuid 4C1F03AF002E
	 */
	public EmployeeObj getEmployeeInfo(int empID) {
		EmployeeObj emp = new EmployeeObj();
		SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd");

		try {

			Map map = organizationDAO.getEmployeeInfo(empID);

			emp.setAge(Type.GetInt(map.get("age")));
			// emp.setBirthday(map.get("birthday") == null ? null : Date
			// .valueOf(df.format(new java.util.Date((String.valueOf(map
			// .get("birthday"))).trim()))));

			emp.setBirthday(Type.getDate(map.get("birthday")));

			emp.setIssueSex(Type.GetInt(map.get(DBSchema.Employee.IssueSex)));
			emp.setCompanyID(Type.GetInt(map
					.get(DBSchema.DepartmentBelongToCompany.CompanyID)));
			emp.setCompanyName(Type.getString(map
					.get(DBSchema.Company.CompanyName)));
			emp.setDepartmentID(Type.GetInt(map
					.get(DBSchema.EmployeeBelongToDept.DepartmentID)));
			emp.setDepartmentName(Type.getString(map
					.get(DBSchema.Department.DepartmentName)));

			emp.setDisabledFlag(Type.convertBinToInt(map
					.get(DBSchema.Employee.DisabledFlag)));

			emp.setDivisionID(Type.GetInt(map.get("division_id")));
			emp.setDivisionName(Type.getString(map.get("division_name")));

			emp.setEducationLevelName(Type.getString(map
					.get(DBSchema.EducationLevel.EducationLevelName)));
			emp.setEducationHighestLevel(Type.GetInt(map
					.get(DBSchema.Employee.EduicationLevelID)));
			emp.setEmailAddress(Type.getString(map.get(DBSchema.Employee.Email)));
			emp.setExtNo(Type.getString(map.get(DBSchema.Employee.ExtNo)));

			emp.setFaxNo(Type.getString(map.get(DBSchema.Employee.FaxNo)));
			emp.setFirstName(Type.getString(map
					.get(DBSchema.Employee.FirstName)));
			emp.setHobby(Type.getString(map.get(DBSchema.Employee.Hobby)));
			emp.setHomeAddress(Type.getString(map
					.get(DBSchema.Employee.HomeAddress)));
			emp.setHomeTelNo(Type.getString(map
					.get(DBSchema.Employee.HomeTelNo)));
			emp.setImgName(Type.getString(map.get(DBSchema.Employee.imgName)));
			emp.setIDCard(Type.getString(map.get(DBSchema.Employee.IDCard)));
			emp.setIssueName(Type.getString(map
					.get(DBSchema.Employee.IssueName)));
			emp.setIssueSex(Type.GetInt(map.get(DBSchema.Employee.IssueSex)));

			// emp.setJoinDate(map.get("join_date") == null ? null : Date
			// .valueOf(df.format(new java.util.Date((String.valueOf(map
			// .get("join_date"))).trim()))));
			emp.setJoinDate(Type.getDate(map.get("join_date")));

			emp.setLastName(Type.getString(map.get(DBSchema.Employee.LastName)));
			emp.setLoginName(Type.getString(map
					.get(DBSchema.Employee.LoginName)));
			emp.setMaritalStatus(Type.GetInt(map
					.get(DBSchema.Employee.MaritalStatus)));
			emp.setMobileNo(Type.getString(map.get(DBSchema.Employee.MobileNo)));
			emp.setNativePlace(Type.getString(map
					.get(DBSchema.Employee.NativePlace)));
			emp.setPassword(Type.getString(map.get(DBSchema.Employee.Password)));

			emp.setOfficeAddress(Type.getString(map
					.get(DBSchema.Province.ProvinceName))
					+ Type.getString(map.get(DBSchema.City.CityName))
					+ Type.getString(map.get(DBSchema.Address.District))
					+ Type.getString(map.get(DBSchema.Address.AddressInfo))
					+ "\n邮政编码:"
					+ Type.getString(map.get(DBSchema.Address.ZipCode))
					+ "    长途区号:"
					+ Type.getString(map.get(DBSchema.City.AreaCode)));
			emp.setPoliticalStatus(Type.GetInt(map
					.get(DBSchema.Employee.PoliticalStatus)));
			emp.setPosition(Type.getString(map.get("position")));
			emp.setRemark(Type.getString(map.get(DBSchema.Employee.Remark)));
			emp.setSexID(Type.GetInt(map.get(DBSchema.Employee.SexID)));
			emp.setSpecialty(Type.getString(map
					.get(DBSchema.Employee.Specialty)));
			emp.setSpouseName(Type.getString(map
					.get(DBSchema.Employee.SpouseName)));
			emp.setSwitchboardNo(Type.getString(map
					.get(DBSchema.Employee.OfficeTelNo)));
			emp.setUserID(Type.GetInt(map.get(DBSchema.Employee.ID)));
			emp.setUserName(Type.getString(map.get(DBSchema.Employee.LastName))
					+ Type.getString(map.get(DBSchema.Employee.LastName)));
			emp.setWebPage(Type.getString(map.get(DBSchema.Employee.WebSite)));
			emp.setZipCode(Type.getString(map.get(DBSchema.Employee.ZipCode)));

			emp.setSchoolName(Type.getString(map
					.get(DBSchema.EducationLevel.SchoolName)));

			logger.debug(String.valueOf(map
					.get(DBSchema.EducationLevel.GraduationDate)));
			emp.setGraduationDate(map
					.get(DBSchema.EducationLevel.GraduationDate) == null ? null
					: Date.valueOf(df.format(new java.util.Date(
							(String.valueOf(map
									.get(DBSchema.EducationLevel.GraduationDate)))))));
			emp.setMajor(Type.getString(map.get(DBSchema.EducationLevel.Major)));
			emp.setEducationRemark(Type.getString(map.get("educationRemark")));

			List aList = organizationDAO.getPoliticalStatusList();
			Map aMap = new LinkedHashMap();
			for (Iterator i = aList.listIterator(); i.hasNext();) {
				// ListOrderedMap m
				Map iMap = (HashMap) i.next();
				aMap.put(Type.getInteger(iMap
						.get(DBSchema.PoliticalStatus.PoliticalStatusID)), iMap
						.get(DBSchema.PoliticalStatus.PoliticalStatusName));
			}

			emp.setPoliticalStatusMap(aMap);

			emp.setLunchAddressID(Type.GetInt(map.get("lunch_id")));
			emp.setLunchAddress(Type.getString(map
					.get(DBSchema.LunchAddress.LunchAddressName)));

			String lunchAddressData = "[";
			List tList = organizationDAO.getLunchAddressList();
			Map tMap = new LinkedHashMap();
			for (Iterator i = tList.listIterator(); i.hasNext();) {
				Map iMap = (HashMap) i.next();
				tMap.put(Type.getInteger(iMap
						.get(DBSchema.LunchAddress.LunchAddressID)), iMap
						.get(DBSchema.LunchAddress.LunchAddressName));

				lunchAddressData += "["
						+ Type.getString(iMap
								.get(DBSchema.LunchAddress.LunchAddressID))
						+ ",'"
						+ Type.getString(iMap
								.get(DBSchema.LunchAddress.LunchAddressName))
						+ "'],";

			}

			emp.setLunchAddressMap(tMap);
			logger.info(lunchAddressData.substring(0,
					lunchAddressData.length() - 1));
			emp.setLunchAddressData(lunchAddressData.substring(0,
					lunchAddressData.length() - 1) + "]");
			String pathId = organizationDAO.getEmpPathId(empID); // 根据人员id得到
																	// 此人的路径
																	// 人ID-部门ID-上级部门ID-...-上级部门ID-部门ID-公司ID
			String pathName = getPathNameByPathId(pathId);
			emp.setPathId(pathId);
			emp.setPathName(pathName);
		} catch (Exception e) {
			e.printStackTrace();
			return null;
		}

		logger.debug(emp.getOfficeAddress());
		return emp;
	}

	/**
	 * @param companyObj
	 * @roseuid 4C1F03AF005D
	 */
	public void doSaveCompanyInfo(CompanyObj companyObj) {

	}

	/**
	 * @param departmentObj
	 * @roseuid 4C1F03AF009C
	 */
	public void doSaveDepartmentInfo(DepartmentObj departmentObj) {

	}

	/**
	 * @param employeeObj
	 * @roseuid 4C1F03AF00DA
	 */
	public void doSaveEmployeeInfo(EmployeeObj employeeObj, EmployeeObj userDNA) {

		logger.info("---> doSaveEmployeeInfo");
		Address address = employeeObj.getAddress();
		try {
			organizationDAO.doSaveAddressInfo(address);
		} catch (Exception e) {
			logger.info("保存地址有误");
			e.printStackTrace();
		}
		organizationDAO.doSaveEmployeeInfo(employeeObj, userDNA);
	}

	public String getProjectListByDiv(int divisionID) {

		logger.info("---> getProjectListByDiv");

		List aList = this.organizationDAO.getProjectListByID(divisionID,
				WebKeys.Type_Division);
		// logger.info(String.valueOf(aList.size()));

		String dataStr = "";
		for (Iterator i = aList.listIterator(); i.hasNext();) {
			Map map = (HashMap) i.next();

			dataStr += "[" + "'" + (String) map.get("dept_name") + "^"
					+ webPath + "orgMaint.do?method="
					+ WebKeys.Action_Method_Name_DepartmentTotalSummary + "&"
					+ WebKeys.Type_Department + "="
					+ String.valueOf(map.get("dept_id")) + "&"
					+ WebKeys.Type_Department_Name + "="
					+ (String) map.get("dept_name") + "',"

					+ "'" + String.valueOf(map.get("finished_count")) + "^"
					+ webPath + "orgMaint.do?method="
					+ WebKeys.Action_Method_Name_DeptFinishedProjects + "&"
					+ WebKeys.Type_Department + "="
					+ String.valueOf(map.get("dept_id")) + "&"
					+ WebKeys.Type_Department_Name + "="
					+ (String) map.get("dept_name") + "',"

					+ "'" + String.valueOf(map.get("unfinished_count")) + "^"
					+ webPath + "orgMaint.do?method="
					+ WebKeys.Action_Method_Name_DeptUnfinishedProjects + "&"
					+ WebKeys.Type_Department + "="
					+ String.valueOf(map.get("dept_id")) + "&"
					+ WebKeys.Type_Department_Name + "="
					+ (String) map.get("dept_name") + "',"

					+ "'" + String.valueOf(map.get("total_count")) + "^"
					+ webPath + "orgMaint.do?method="
					+ WebKeys.Action_Method_Name_DeptAllProjects + "&"
					+ WebKeys.Type_Department + "="
					+ String.valueOf(map.get("dept_id")) + "&"
					+ WebKeys.Type_Department_Name + "="
					+ (String) map.get("dept_name") + "'"

					+ "],";
		}

		dataStr = dataStr.substring(0, dataStr.length() - 1);

		// logger.debug(dataStr);

		return "[" + dataStr + "]";
	}

	public Document getEmpTotalSummary(int departmentID, PageInfo pageInfo) {

		logger.info("---> getEmpTotalSummary ? deptID="
				+ String.valueOf(departmentID));

		List aList = organizationDAO.getTotalSummary(WebKeys.Type_Department,
				departmentID, pageInfo);

		return WebUtil.buildGridStoreXML(aList,
				WebKeys.Action_Method_Name_DepartmentTotalSummary);
	}

	private void buildXML(EmployeeObj userObj, List treeList) {

		logger.info(webDir);

		/** 建立document对象 */
		Document document = DocumentHelper.createDocument();
		document.addProcessingInstruction("xml",
				"version=\"1.0\" encoding=\"GBK\"");

		/** 建立XML文档的根 */
		Element treeElement = document.addElement("tree");
		treeElement.addAttribute("id", "0");

		/** 加入division节点 */
		Element divisionElement = treeElement.addElement("item");
		divisionElement.addAttribute("text", userObj.getDivisionName());
		divisionElement.addAttribute("id",
				"div-" + String.valueOf(userObj.getDivisionID()));
		divisionElement.addAttribute("open", "1");
		divisionElement.addAttribute("im0", "tombs.gif");
		divisionElement.addAttribute("im1", "tombs.gif");
		divisionElement.addAttribute("im2", "iconSafe.gif");
		divisionElement.addAttribute("call", "1");
		divisionElement.addAttribute("select", "1");
		divisionElement.addAttribute("tooltip",
				String.valueOf(userObj.getDivisionID()));

		Element userDataElement = divisionElement.addElement("userdata");
		userDataElement.addAttribute("name", "file");
		userDataElement.setText(webPath + "orgMaint.do?method="
				+ WebKeys.Action_Method_Name_DivisionTotalSummary);

		String deptName = userObj.getDivisionName();
		int deptID = userObj.getDivisionID();

		String redFont = "<font color=red>";
		String endFont = " [禁用]</font>";

		DepartmentTree deptTree;

		for (Iterator i = treeList.listIterator(); i.hasNext();) {
			deptTree = new DepartmentTree();
			deptTree = (DepartmentTree) i.next();

			// logger.info(deptTree.getDepartName());

			// if (!deptName.equals(deptTree.getDepartName())) {
			/*
			 * a new department Node
			 */
			if (deptID != deptTree.getDepartID()) {
				// logger.info("the first of dept");
				Element departmentElement = divisionElement.addElement("item");
				departmentElement.addAttribute(
						"text",
						deptTree.getDeptDisabledFlag() == 1 ? redFont
								+ deptTree.getDepartName() + endFont : deptTree
								.getDepartName());
				departmentElement.addAttribute("id",
						"dept-" + String.valueOf(deptTree.getDepartID()));
				departmentElement.addAttribute("im0", "folderClosed.gif");
				departmentElement.addAttribute("im1", "folderOpen.gif");
				departmentElement.addAttribute("im2", "folderClosed.gif");
				departmentElement.addAttribute("tooltip", deptTree
						.getDeptDisabledFlag() == 1 ? WebKeys.NodeTipOnDeptTree
						: String.valueOf(deptTree.getDepartID()));

				Element userDataForDept = departmentElement
						.addElement("userdata");
				userDataForDept.addAttribute("name", "file");
				userDataForDept.setText(webPath + "orgMaint.do?method="
						+ WebKeys.Action_Method_Name_DepartmentTotalSummary
						+ "&" + WebKeys.Type_Department + "="
						+ deptTree.getDepartID() + "&"
						+ WebKeys.Type_Department_Name + "="
						+ deptTree.getDepartName());

				if (deptTree.getUserID() > 0) { /*
												 * the userID>0 means that user
												 * is regular user & not admin
												 * user
												 */

					// logger.info("1="+deptTree.getUserName());
					Element employeeElement = departmentElement
							.addElement("item");
					employeeElement.addAttribute("text",
							deptTree.getEmpDisabledFlag() == 1 ? redFont
									+ deptTree.getUserName() + endFont
									: deptTree.getUserName()
					// + "-" + deptTree.getPosition()
							);
					employeeElement.addAttribute("id",
							"emp-" + String.valueOf(deptTree.getUserID()));
					employeeElement.addAttribute("im0", "01.gif");
					employeeElement.addAttribute("im1", "01.gif");
					employeeElement.addAttribute("im2", "01.gif");
					employeeElement
							.addAttribute(
									"tooltip",
									deptTree.getEmpDisabledFlag() == 1 ? WebKeys.NodeTipOnDeptTree
											: String.valueOf(deptTree
													.getUserID()));

					Element userDataForEmp = employeeElement
							.addElement("userdata");
					userDataForEmp.addAttribute("name", "file");
					userDataForEmp.setText(webPath + "orgMaint.do?method="
							+ WebKeys.Action_Method_Name_GetEmployee + "&emp="
							+ deptTree.getUserID());
				}
			} else if (deptTree.getUserID() > 0) { /*
													 * there are some users
													 * below the same division
													 * or department
													 */

				Element root = document.getRootElement();

				for (Iterator it = root.elementIterator(); it.hasNext();) {
					Element division = (Element) it.next();

					/* the user below the division */
					for (Iterator itDivAtt = division.attributeIterator(); itDivAtt
							.hasNext();) {
						Attribute divAtt = (Attribute) itDivAtt.next();
						if ("text".equals(divAtt.getName())
								&& String.valueOf(deptName).equals(
										divAtt.getValue())) {
							Element employeeElement = division
									.addElement("item");
							employeeElement.addAttribute("text", deptTree
									.getEmpDisabledFlag() == 1 ? redFont
									+ deptTree.getUserName() + endFont
									: deptTree.getUserName()
							// + "-" + deptTree.getPosition()
									);
							employeeElement.addAttribute(
									"id",
									"emp-"
											+ String.valueOf(deptTree
													.getUserID()));
							employeeElement.addAttribute("im0", "01.gif");
							employeeElement.addAttribute("im1", "01.gif");
							employeeElement.addAttribute("im2", "01.gif");
							employeeElement
									.addAttribute(
											"tooltip",
											deptTree.getEmpDisabledFlag() == 1 ? WebKeys.NodeTipOnDeptTree
													: String.valueOf(deptTree
															.getUserID()));

							Element userDataForEmp = employeeElement
									.addElement("userdata");
							userDataForEmp.addAttribute("name", "file");
							userDataForEmp.setText(webPath
									+ "orgMaint.do?method="
									+ WebKeys.Action_Method_Name_GetEmployee
									+ "&emp=" + deptTree.getUserID());

						}
					}

					for (Iterator itDivision = division.elementIterator(); itDivision
							.hasNext();) {
						Element dept = (Element) itDivision.next();

						/* the user below the department */
						for (Iterator itDeptAtt = dept.attributeIterator(); itDeptAtt
								.hasNext();) {
							Attribute deptAtt = (Attribute) itDeptAtt.next();

							if ("text".equals(deptAtt.getName())
									&& (String.valueOf(deptName).equals(deptAtt
											.getValue()))
									|| ((redFont + String.valueOf(deptName) + endFont)
											.equals(deptAtt.getValue()))) {

								Element employeeElement = dept
										.addElement("item");
								employeeElement.addAttribute("text", deptTree
										.getEmpDisabledFlag() == 1 ? redFont
										+ deptTree.getUserName() + endFont
										: deptTree.getUserName()
								// + "-" + deptTree.getPosition()
										);
								employeeElement.addAttribute("id", "emp-"
										+ String.valueOf(deptTree.getUserID()));
								employeeElement.addAttribute("im0", "01.gif");
								employeeElement.addAttribute("im1", "01.gif");
								employeeElement.addAttribute("im2", "01.gif");
								employeeElement
										.addAttribute(
												"tooltip",
												deptTree.getEmpDisabledFlag() == 1 ? WebKeys.NodeTipOnDeptTree
														: String.valueOf(deptTree
																.getUserID()));

								Element userDataForEmp = employeeElement
										.addElement("userdata");
								userDataForEmp.addAttribute("name", "file");
								userDataForEmp
										.setText(webPath
												+ "orgMaint.do?method="
												+ WebKeys.Action_Method_Name_GetEmployee
												+ "&emp="
												+ deptTree.getUserID());

							}
						}
					}
				}

			}

			deptName = deptTree.getDepartName();
			deptID = deptTree.getDepartID();

		}

		// Element departmentElement = divisionElement.addElement("item");
		// departmentElement.addAttribute("text", "工程一部");
		// departmentElement.addAttribute("id", "EngDept1");
		// departmentElement.addAttribute("im0", "folderClosed.gif");
		// departmentElement.addAttribute("im1", "folderOpen.gif");
		// departmentElement.addAttribute("im2", "folderClosed.gif");
		//
		// Element departmentElement2 = divisionElement.addElement("item");
		// departmentElement2.addAttribute("text", "开发部");
		// departmentElement2.addAttribute("id", "DevDept");
		// departmentElement2.addAttribute("im0", "folderClosed.gif");
		// departmentElement2.addAttribute("im1", "folderOpen.gif");
		// departmentElement2.addAttribute("im2", "folderClosed.gif");
		//
		// Element userDataElement1 = departmentElement2.addElement("userdata");
		// userDataElement1.addAttribute("name", "file");
		// userDataElement1.setText("http://#");
		//
		// Element employeeElement1 = departmentElement2.addElement("item");
		// employeeElement1.addAttribute("text", "李轩");
		// employeeElement1.addAttribute("id", "DevDept_01");
		// employeeElement1.addAttribute("im0", "01.gif");
		// employeeElement1.addAttribute("im1", "01.gif");
		// employeeElement1.addAttribute("im2", "01.gif");
		//
		// Element userDataElement2 = employeeElement1.addElement("userdata");
		// userDataElement2.addAttribute("name", "file");
		// userDataElement2.setText("http://www.sohu.com");

		try {

			/** 将document中的内容写入文件中 */

			OutputFormat format = OutputFormat.createCompactFormat();
			format.setEncoding("utf-8");
			XMLWriter writer = new XMLWriter(new FileWriter(new File(webDir
					+ WebKeys.String_DepartmentTree_XML_Dir)), format);

			writer.write(document);

			writer.close();

			/** 执行成功,需返回1 */

		} catch (Exception ex) {

			ex.printStackTrace();

		}

	}

	public String getWebPath() {
		return webPath;
	}

	public void setWebPath(String webPath) {
		this.webPath = webPath;
	}

	private Document buildEmpTotalSummaryXML(List aList) {

		logger.info("---> buildEmpTotalSummaryXML");

		/** 建立document对象 */
		Document document = DocumentHelper.createDocument();
		/** 建立XML文档的根 */
		Element rootElement = document.addElement("ItemSearchResponse");
		/* 应答信息 */
		Element requestElement = rootElement.addElement("Request");
		/* 记录 */
		Element itemsElement = requestElement.addElement("Items");
		itemsElement.addElement("TotalResults", "3");
		itemsElement.addElement("TotalPages", "3");

		for (Iterator i = aList.listIterator(); i.hasNext();) {
			Element itemElement = itemsElement.addElement("Item");

			Map map = (HashMap) i.next();

			int mapsize = map.size();
			Iterator it = map.entrySet().iterator();
			for (int j = 0; j < mapsize; j++) {
				Map.Entry entry = (Map.Entry) it.next();
				itemElement.addElement(String.valueOf(entry.getKey())).setText(
						String.valueOf(entry.getValue()));
			}

		}

		// String filePath = new String(
		// "D:/apache-tomcat-5.5.28/webapps/xMIS/contract/empTotalSummary.xml");
		// try {
		// XMLWriter writer = new XMLWriter(new java.io.FileOutputStream(
		// filePath));
		//
		// writer.write(document);
		//
		// writer.close();
		//
		// } catch (Exception ex) {
		//
		// ex.printStackTrace();
		//
		// }
		return document;

	}


	public int doChangePassword(int userID, String password, int updateID) {
		try {
			return organizationDAO.doChangePassword(userID, password, updateID);
		} catch (Exception e) {
			e.printStackTrace();
			return -1;
		}
	}

	public int doSaveUserPhoto(int userID, String imgName, int updateID) {
		try {
			return organizationDAO.doSaveUserPhoto(userID, imgName, updateID);
		} catch (Exception e) {
			e.printStackTrace();
			return -1;
		}
	}

	public int doEditEducation(String oldEduName, EmployeeObj emp, int updateID) {

		logger.info(oldEduName + "=" + emp.getEducationLevelName());

		try {
			if (emp.getEducationHighestLevel() != 0) {
				// update sys.学历 only
				return organizationDAO.doUpdateEdu(emp, updateID);
			} else {
				// update hr.人员 & create new into sys.学历
				return organizationDAO.doCreateNewEdu(emp, updateID);
			}

		} catch (Exception e) {
			e.printStackTrace();
			return -1;
		}
	}

	public int doCreateNewUser(String deptID, String deptName, String userName,
			EmployeeObj operator) {

		logger.info("--->	doCreateNewUsr");

		try {
			if (deptID.indexOf("dept-new") > -1) {
				// neet to create new dept before creating new user

				return organizationDAO.doCreateUser(organizationDAO
						.doCreateDept(operator.getDivisionID(), deptName,
								operator.getUserID()), userName, operator
						.getUserID());

			} else {
				// create new user only

				return organizationDAO.doCreateUser(Integer.parseInt(deptID
						.substring(deptID.indexOf("-") + 1)), userName,
						operator.getUserID());
			}
		} catch (Exception e) {
			e.printStackTrace();
			return -1;
		}

	}

	public int doDisableTreeNode(String itemNode, EmployeeObj userObj)
			throws Exception {

		logger.info("--->	doDisableTreeNode//" + itemNode.substring(0, 4)
				+ "//" + itemNode.substring(9));

		try {

			if (itemNode.indexOf("empl-old-") == 0)
				organizationDAO.doDisableEmployee(
						Integer.parseInt(itemNode.substring(9)),
						userObj.getUserID());
			else
				organizationDAO.doDisableDepartment(
						Integer.parseInt(itemNode.substring(9)),
						userObj.getUserID());

			return 1;

		} catch (Exception e) {
			e.printStackTrace();
			throw e;
			// return -1;
		}
	}

	public int doChangeLunchAddress(int newAddress, int empID, int operatorID) {

		logger.info("--->	doUpdateLunchAddress//");

		try {
			return organizationDAO.doUpdateLunchAddress(newAddress, empID,
					operatorID);
		} catch (Exception e) {
			e.printStackTrace();
			return -1;
		}

	}

	public String getLunchAddressListByDiv(int divID) {

		logger.info("--->   getLunchAddressListByDiv");

		List aList = organizationDAO.getLunchAddressListByDiv(divID);

		String dataStr = "";
		for (Iterator i = aList.listIterator(); i.hasNext();) {
			Map map = (HashMap) i.next();

			// dataStr += "[" + "'" + (String)
			// map.get(DBSchema.Department.DepartmentName) + "',"
			//
			// + "'" + String.valueOf(map.get("user_name")) + "',"
			//
			// + "'" +
			// String.valueOf(map.get(DBSchema.LunchAddress.LunchAddressName)) +
			// "'"
			//
			// + "],";

			dataStr += "{'dept_name':'"
					+ Type.getString(map
							.get(DBSchema.Department.DepartmentName))
					+ "','user_name':'"
					+ Type.getString(map.get("user_name"))
					+ "','old_address_name':'"
					+ Type.getString(map.get("old_address_name"))
					+ "','lunchAddress_name':'"
					+ (Type.convertBinToInt(map
							.get(DBSchema.EmployeeLunchAddress.BookedFlag)) == 0 ? "<font color=red>"
							+ String.valueOf(map
									.get(DBSchema.LunchAddress.LunchAddressName))
							+ "</font>"
							: "")

					+ "','changed_flag':'"
					+ (Type.convertBinToInt(map
							.get(DBSchema.EmployeeLunchAddress.BookedFlag)) == 0 ? "<font color=red>变更</font>"
							: "无变化") + "'},";
		}
		dataStr = dataStr.substring(0, dataStr.length() - 1);

		logger.info(dataStr);

		return "{'totalCount':'" + String.valueOf(aList.size()) + "','rows':["
				+ dataStr + "]}";
	}

	public int doBookLunchAddress() {

		return organizationDAO.doBookLunchAddress();
	}

	public String getCompanyStore(int companyID) {
		CompanySimpleObj obj = organizationDAO.getCompanySimpleObj(companyID);
		JSON store = JSONUtils.toJSON(obj, true);
		return store.toString();
	}

	// @Transactional(readOnly = false, propagation = Propagation.REQUIRES_NEW,
	// rollbackFor = Exception.class)
	public int doCreateNewBatch(Map deptMap, Map empMap, Map empDeptMapping,
			EmployeeObj operator) throws Exception {

		logger.info("--->	doCreateNewBatch");

		try {
			if (deptMap != null) {
				Map newDeptMap = organizationDAO
						.doCreateDeptBatch(operator.getDivisionID(), deptMap,
								operator.getUserID());

				if (empMap != null) {

					Map newMapping = new HashMap();

					Iterator it = empDeptMapping.entrySet().iterator();
					for (int j = 0; j < empDeptMapping.size(); j++) {
						Map.Entry entry = (Map.Entry) it.next();

						newMapping.put(entry.getKey(), entry.getValue());
						logger.info((String) entry.getKey() + "="
								+ (String) entry.getValue() + "//"
								+ newDeptMap.get(entry.getValue()));

						if (newDeptMap.get(entry.getValue()) != null
								&& !"null".equals((String) newDeptMap.get(entry
										.getValue()))
								&& !"".equals((String) newDeptMap.get(entry
										.getValue()))) {
							newMapping.put(entry.getKey(),
									newDeptMap.get(entry.getValue()));
						}
					}

					organizationDAO.doCreateEmpBatch(empMap, newMapping,
							operator.getUserID());

				}
			} else {
				organizationDAO.doCreateEmpBatch(empMap, empDeptMapping,
						operator.getUserID());

			}
			return 1;

		} catch (Exception e) {
			e.printStackTrace();
			throw e;
			// return -1;
		}

	}

	public String getOrgTree(String node, int id) {
		logger.info("---> getOrgTree (serivce impl) id = " + id);
		logger.info("---> getOrgTree (serivce impl) node = " + node);
		logger.info("---> getOrgTree (serivce impl) compID = " + id);

		final int FIND_COMP = 1;
		final int FIND_BDEPT = 2;
		final int FIND_DEPT = 3;

		TreeLoaderResponse treeLoader = new TreeLoaderResponse();
		JsonConfig config = new JsonConfig();
		config.setExcludes(new String[] { "checked" });
		config.setIgnoreDefaultExcludes(false);
		config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);

		if ("root".equals(node.trim())) {
			List<Object[]> list = organizationDAO.getOrgTree(FIND_COMP, id);
			for (Object[] objs : list) {
				treeLoader.addTreeNode(String.valueOf(objs[1]).trim() + "("
						+ String.valueOf(objs[2]).trim() + ")", "comp-old-"
						+ objs[0] + "", false, "file", "comp");
			}
			return treeLoader.toString(config);
		}
		String[] strs = StringUtils.split(node, "-");
		if ("comp".equals(strs[0])) {
			id = Type.GetInt(strs[2]);
			List<Object[]> list = organizationDAO.getOrgTree(FIND_BDEPT, id);
			for (Object[] objs : list) {
				TreeNode tnode = new TreeNode();
				String name = String.valueOf(objs[1]).trim();
				String cls = "file";
				tnode.addAttribute("deptId", String.valueOf(objs[0]));
				tnode.addAttribute("deptName", name);
				tnode.addAttribute("forbidden", 
						(1 == Integer.valueOf(objs[3] + "")) ? "1" : "0");
				if ("1".equals(String.valueOf(objs[3]).trim())) {
					name = name + "(禁用)";
					cls = "x-tree-node-italic-red";
				}
				String userid = String.valueOf(objs[0]).trim();
				
				if (WebKeys.BBZ_DEPT_NO1.equals(userid)||WebKeys.BBZ_DEPT_NO2.equals(userid)||WebKeys.BBZ_DEPT_NO3.equals(userid)) {
					cls = "x-tree-node-italic-blue";
				}
				tnode.setText(name + "(" + String.valueOf(objs[2]).trim() + ")");
				tnode.setId("dept-old-" + objs[0]);
				tnode.setLeaf(false);
				tnode.setIconCls("dept");
				tnode.setCls(cls);
				treeLoader.addTreeNode(tnode);
			}
			return treeLoader.toString(config);
		}
		if ("dept".equals(strs[0])) {
			id = Type.GetInt(strs[2]);
			List<Object[]> list = organizationDAO.getOrgTree(FIND_DEPT, id);
			for (Object[] objs : list) {
				TreeNode tnode = new TreeNode();
				String objID = null;
				String cls = "file";
				String iconCls = "dept";
				String name = String.valueOf(objs[1]).trim();
				boolean leaf = true;
				tnode.addAttribute("forbidden",
						(1 == Integer.valueOf(objs[4] + "")) ? "1" : "0");
				if ("0".equals(String.valueOf(objs[3]).trim())) {
					String userId = String.valueOf(objs[2]).trim();
					objID = "empl-old-" + userId;
					tnode.addAttribute("userId", userId);
					iconCls = "personPic";
					if (1 == Integer.valueOf(objs[4] + "")) {
						cls = "x-tree-node-italic-red";
						name = name + "(禁用)";
					} else {
						if (!"null".equals(String.valueOf(objs[5]).trim())) {
							name = name + "(" + String.valueOf(objs[5]).trim()
									+ ")";
						}
					}
					if (1 == Integer.valueOf(objs[7] + "")) {
						cls = "x-tree-node-italic-green";
					}
				} else {
					tnode.addAttribute("deptId", String.valueOf(objs[0]).trim());
					objID = "dept-old-" + String.valueOf(objs[0]).trim();
					leaf = false;
					tnode.addAttribute("deptName", name);
					if (1 == Integer.valueOf(objs[4] + "")) {
						cls = "x-tree-node-italic-red";
						name = name + "(禁用)";
					} else {
						name = name + "(" + String.valueOf(objs[2]).trim()
								+ ")";
					}
				}
				tnode.setId(objID);
				tnode.setLeaf(leaf);
				tnode.setIconCls(iconCls);
				tnode.setCls(cls);
				tnode.setText(name);
				treeLoader.addTreeNode(tnode);
			}

			return treeLoader.toString(config);
		}

		return treeLoader.toString(config);
	}
	public EmployeeObj getEmployeeById(int id){
		return organizationDAO.getEmployee(id);
		
	}
	
	public String getEmployee(int id) {
		logger.info("---> getEmployee (service impl)");
		EmployeeObj obj = organizationDAO.getEmployee(id);

		String pathId = organizationDAO.getEmpPathId(id); // 根据人员id得到 此人的路径
															// 人ID-部门ID-上级部门ID-...-上级部门ID-部门ID-公司ID
		String pathName = getPathNameByPathId(pathId);
		obj.setPathId(pathId);
		obj.setPathName(pathName);
		// 根据人员ID，禁用原因的ID(string) 查询此人的禁用的原因
		if (obj != null && obj.getDisabledReason() != ""
				&& obj.getDisabledReason() != null) {
			if (obj.getFlag()) {
				DisabledReason disabledReason = organizationDAO
						.getDisabledReason(obj.getDisabledReason());
				if (disabledReason != null) {
					obj.setDisabledReasonID(disabledReason.getId());
					obj.setDisabledReason(disabledReason.getName());
				}
			}

		}

		List<EmployeeObj> list = new ArrayList<EmployeeObj>();
		list.add(obj);
		return JSONUtils.toJSON(list, true).toString();
	}

	public String getEmployeeList(String ID, int start, int size) {
		// TODO Auto-generated method stub
		String[] strs = StringUtils.split(ID, "-");
		int id = Type.GetInt(strs[1]);
		String tableObj = "CompanySimpleObj";
		List<EmployeePost> employees = new ArrayList<EmployeePost>();

		JsonConfig config = new JsonConfig();
		config.setIgnoreDefaultExcludes(false);
		config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);

		if ("comp".equals(strs[0])) {
			List<CompanySimpleObj> list = organizationDAO.getEmployeeList(
					tableObj, id);
			for (CompanySimpleObj company : list) {
				for (DepartmentObj dept : company.getDepts()) {
					List<EmployeePost> emps = dept.getPosts();
					if (emps != null && emps.size() > 0) {
						for (EmployeePost emp : emps) {

							employees.add(emp);
						}
					}
					for (DepartmentObj d : dept.getDepts()) {
						for (EmployeePost emp : d.getPosts()) {
							employees.add(emp);
						}
					}

				}

			}
			return "{totalProperty:"
					+ employees.size()
					+ ",root:"
					+ JSONArray.fromObject(
							employees.subList(start,
									employees.size() - start >= size ? start
											+ size : employees.size()), config)
							.toString() + "}";
		}
		// if ("dept".equals(strs[0])) {
		// tableObj = "DepartmentObj";
		// List<DepartmentObj> list = organizationDAO.getOrgTree(tableObj, id);
		// // 数据封装成list
		//
		// for (DepartmentObj dept : list) {
		// List<EmployeePost> emps = dept.getPosts();
		// if (emps != null && emps.size() > 0) {
		// for (EmployeePost emp : emps) {
		// employees.add(emp);
		// }
		// }
		// for (DepartmentObj d : dept.getDepts()) {
		// for (EmployeePost emp : d.getPosts()) {
		// employees.add(emp);
		// }
		// }
		// }
		// return "{totalProperty:"
		// + employees.size()
		// + ",root:"
		// + JSONArray.fromObject(
		// employees.subList(start,
		// employees.size() - start >= size ? start
		// + size : employees.size()), config)
		// .toString() + "}";
		// }
		if ("empl".equals(strs[0])) {
			EmployeeObj obj = organizationDAO.getEmployee(id);
			List<EmployeeObj> list = new ArrayList<EmployeeObj>();
			list.add(obj);
			JSONArray array = JSONArray.fromObject(list, config);

			return "{totalProperty:"
					+ employees.size()
					+ ",root:[{employee:"
					+ array.toString().substring(1,
							array.toString().length() - 1) + "}]}";
		}
		return null;
	}

	public String getPoliticsStatusTotalSummary() {
		logger.info("---> getPoliticalStatusTotalSummary (service impl)");

		JsonConfig config = new JsonConfig();
		config.setIgnoreDefaultExcludes(false);
		config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);
		return JSONArray.fromObject(
				organizationDAO.getPoliticsStatusTotalSummary(), config)
				.toString();
	}

	public boolean checkLoginNameExist(String loginName, int userid) {
		// TODO Auto-generated method stub
		return organizationDAO.checkLoginNameExist(loginName, userid) == null ? true
				: false;
	}

	public String doSaveAllOperation(Map<String, String> deptMap,
			Map<String, String> emplMap, List<List<String>> mappingList,
			List<List<String>> dragList, Map<String, String> changeEmplName,
			Map<String, String> changeDeptName, EmployeeObj operator)
			throws Exception {
		logger.info("---> saveAllOperation (service impl)");

		// 保存新部门
		Map<String, String> newDeptMap = null;
		if (deptMap != null) {
			logger.info("---> saveAllOperation (service impl) add new dept");
			newDeptMap = organizationDAO.doCreateDeptBatch(deptMap,
					operator.getUserID());
			for (Map.Entry entry : newDeptMap.entrySet()) {
				for (List<String> list : mappingList) {
					if (list.get(0).equals(entry.getKey())) {
						list.remove(0);
						list.add(0, "dept-old-" + entry.getValue());
					}
					if (list.get(1).equals(entry.getKey())) {
						list.remove(1);
						list.add(1, "dept-old-" + entry.getValue());
					}
				}
			}
		}

		// 保存新员工
		Map<String, String> newEmplMap = null;
		if (emplMap != null) {
			logger.info("---> saveAllOperation (service impl) add new emp");
			try {
				newEmplMap = organizationDAO.doCreateEmplBatch(emplMap, null,
						operator.getUserID());
			} catch (Exception e) {
				logger.info("保存新员工的方法//doCreateEmplBatch--");
				e.printStackTrace();
			}
			for (Map.Entry entry : newEmplMap.entrySet()) {
				for (List<String> list : mappingList) {
					if (list.get(0).equals(entry.getKey())) {
						list.remove(0);
						list.add(0, "empl-old-" + entry.getValue());
					}
				}
			}
		}

		// 更改 员工-->部门、 部门-->部门、 部门-->公司 关系
		Map<String, String> mappingEmplMap = null;
		Map<String, String> mappingDeptMap = null;
		Map<String, String> mappingCompMap = null;
		if (deptMap != null) {
			mappingDeptMap = new HashMap<String, String>();
			for (List<String> list : mappingList) {
				if ("dept-old-".equals(list.get(0).substring(0, 9))
						&& "dept-old-".equals(list.get(1).substring(0, 9))) {
					mappingDeptMap.put(list.get(0), list.get(1));
				}
			}

			mappingCompMap = new HashMap<String, String>();
			for (List<String> list : mappingList) {
				if ("dept-old-".equals(list.get(0).substring(0, 9))
						&& "comp-old-".equals(list.get(1).substring(0, 9))) {
					mappingCompMap.put(list.get(0), list.get(1));
				}
			}
			organizationDAO.doUpdateDeptBatch(mappingDeptMap, mappingCompMap);
		}

		if (emplMap != null) {
			mappingEmplMap = new HashMap<String, String>();
			for (List<String> list : mappingList) {
				if ("empl-old-".equals(list.get(0).substring(0, 9))) {
					mappingEmplMap.put(list.get(0), list.get(1));
				}
			}
			organizationDAO.doUpdateEmpDeptBatch(mappingEmplMap,
					operator.getUserID());
			organizationDAO.doUpdateEmpRoleBatch(mappingEmplMap,
					operator.getUserID());
		}

		// 拖拽保存
		if (dragList != null) {
			Map<String, String> dragMap = new HashMap<String, String>();

			Map<String, String> dragEmplToDeptMap = new HashMap<String, String>();
			Map<String, String> dragDeptToDeptMap = new HashMap<String, String>();
			Map<String, String> dragCompToDeptMap = new HashMap<String, String>();
			Map<String, String> dragCompToCompMap = new HashMap<String, String>();
			Map<String, String> dragDeptToCompMap = new HashMap<String, String>();

			if (newDeptMap != null) {

				for (List<String> list : dragList) {
					if (newDeptMap.containsKey(list.get(2).trim())) {
						String key = list.get(2).trim();
						list.remove(2);
						list.add(2, "dept-old-" + newDeptMap.get(key) + ":"
								+ list.get(1));
					}
					dragMap.put(list.get(0), list.get(2));
				}
			}
			//
			for (List<String> l : dragList) {
				dragMap.put(l.get(0), l.get(2) + ":" + l.get(1));
			}
			//
			Set<String> set = dragMap.keySet();
			for (String s : set) {
				String[] tmp = dragMap.get(s).split(":");

				System.out.println(">>>>>>>>>>>>>>>>>>>>>>" + dragMap.get(s));

				if ("empl-old-".equals(s.substring(0, 9))) {
					dragEmplToDeptMap.put(s, tmp[0]);
				} else {
					if ("comp-old-".equals(tmp[0].substring(0, 9))) {
						if (tmp[0].substring(0, 9).equals(
								tmp[1].substring(0, 9))) {
							dragCompToCompMap.put(s, tmp[0]);
						} else {
							dragDeptToCompMap.put(s, dragMap.get(s));
						}
					} else {
						// 新部门
						if (tmp[0].substring(0, 9).equals(
								tmp[1].substring(0, 9))) {
							dragDeptToDeptMap.put(s, tmp[0]);
						} else {
							dragCompToDeptMap.put(s, dragMap.get(s));
						}
					}
				}
			}
			organizationDAO.doUpdateDeptBelongToDeptBatch(dragEmplToDeptMap,
					dragDeptToDeptMap, dragCompToCompMap, dragCompToDeptMap,
					dragDeptToCompMap);

		}

		// 更改用户名
		if (changeEmplName != null) {
			organizationDAO.doUpdateEmpNameBatch(changeEmplName);
		}
		// 更改部门名
		if (changeDeptName != null) {
			organizationDAO.doUpdateDeptNameBatch(changeDeptName);
		}
		String newID = "";
		String nodePath = "";
		if (mappingList != null && mappingList.size() > 0) {
			newID = mappingList.get(0).toString();
			if (newID.indexOf(",") > 0) {
				newID = newID.split(",")[0].substring(1);
			}
			/**
			 * 根据newID先判断是人员还是部门，得到此节点的路径
			 * */
			if ("dept-old-".equals(newID.substring(0, 9))) {
				String deptID = newID.substring(9);
				nodePath = getDeptNodePath(Type.GetInt(deptID));
			} else if ("empl-old-".equals(newID.substring(0, 9))) {
				String empID = newID.substring(9);
				nodePath = getEmpNodePath(Type.GetInt(empID));
			}
			logger.info("------此节点的路径" + nodePath);
		}
		return nodePath;
	}

	/**
	 * 根据部门ID 求上级的所有部门ID的字符串集
	 * 
	 * @param deptID
	 * @param result
	 *            此部门所在父部门的上级部门
	 * @return
	 */
	public String getDeptStr(int deptID, String result) {
		if (deptID != 0) {
			int fdeptID = 0;
			if(organizationDAO.getFdeptIDIsExistByDeptID(deptID)){
				fdeptID = organizationDAO.getFdeptIDByDeptID(deptID);// 此部门的父部门
			}
			if (fdeptID != 0) {
				result += fdeptID + "-";
			}
			return getDeptStr(fdeptID, result);
		} else {
			return result;
		}

	}
	
	/**
	 * 根据部门ID 得到此节点的路径
	 * 
	 * @param empID
	 * @return
	 */
	public String getDeptNodePath(int deptID) {
		String idStr = "";
		try {
			String deptStr = getDeptStr(deptID, deptID + "-");
			if (("").equals(deptStr)) { //
				int compID = organizationDAO.getCompIDByDeptID(deptID);
				idStr = "/root/comp-old-" + compID + "-/dept-old-" + deptID;
			} else {
				String[] array = deptStr.substring(0, deptStr.length() - 1)
						.split("-");
				int compID = organizationDAO.getCompIDByDeptID(Type
						.GetInt(array[array.length - 1]));
				String rs = "";
				for (int i = 0; i < array.length; i++) {
					rs += "/dept-old-" + array[array.length - 1 - i];
				}
				idStr = "/root/comp-old-" + compID + rs;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		return idStr;
	}

	/**
	 * 根据人员ID 得到此节点的路径
	 * 
	 * @param empID
	 * @return
	 */
	public String getEmpNodePath(int empID) {
		int deptID = organizationDAO.getDeptIDByUserID(Type.GetInt(empID));
		String empNodePath = getDeptNodePath(deptID) + "/empl-old-" + empID; // 此人所在部门节点路径+人员节点路径
		return empNodePath;
	}

	public String getPositionTotalSummary() {
		logger.info("---> getPositionTotalSummary (service impl)");
		List<Post> list = organizationDAO.getPositionTotalSummary();
		JsonConfig config = new JsonConfig();
		config.setIgnoreDefaultExcludes(false);
		config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);
		return JSONArray.fromObject(list, config).toString();
	}

	public String updatePost(int id, int postId) {
		// TODO Auto-generated method stub
		logger.info("---> updatePost (service impl)");

		return organizationDAO.updatePost(id, postId);

	}

	public void test() {
		// TODO Auto-generated method stub
		organizationDAO.test();
	}

	public String getOrganizationTree(String node, int type) {
		int id = 0; // 1为 仅部门

		logger.info("---> getOrgTree (serivce impl) id = " + id);
		logger.info("---> getOrgTree (serivce impl) node = " + node);
		logger.info("---> getOrgTree (serivce impl) type = " + type);

		final int FIND_COMP = 1;
		final int FIND_BDEPT = 2;
		final int FIND_DEPT = 3;

		final int FIND_DEPT_ONLY = 1;
		final int FIND_PERSON_ONLY = 2;

		TreeLoaderResponse treeLoader = new TreeLoaderResponse();

		String[] strs = StringUtils.split(node, "-");

		if ("root".equals(strs[0])) {
			List<Object[]> list = organizationDAO.getOrgTree(FIND_COMP, -1);
			for (Object[] objs : list) {
				treeLoader.addTreeNode(String.valueOf(objs[1]).trim() + "("
						+ String.valueOf(objs[2]).trim() + ")", "comp-old-"
						+ objs[0] + "", false, "file", ""); // ,false
			}
			return treeLoader.toString();
		}

		// 仅部门
		if (type == FIND_DEPT_ONLY) {
			if ("dept".equals(strs[0])) {
				id = Type.GetInt(strs[2]);
				List<Object[]> list = organizationDAO.getOrganizationTree(
						FIND_DEPT, id);
				for (Object[] objs : list) {
					String objID = null;
					String cls = "file";
					String icon = "";
					String name = String.valueOf(objs[1]).trim();
					boolean leaf = true;
					objID = "dept-old-" + String.valueOf(objs[0]).trim();
					treeLoader.addTreeNode(name, objID, leaf, cls, icon);
				}

				JsonConfig config = new JsonConfig();
				config.setIgnoreDefaultExcludes(false);
				config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);
				return treeLoader.toString();
			}
			if ("comp".equals(strs[0])) {
				id = Type.GetInt(strs[2]);
				List<Object[]> list = organizationDAO.getOrganizationTree(
						FIND_BDEPT, id);
				for (Object[] objs : list) {
					String name = String.valueOf(objs[1]).trim();
					String cls = "file";
					boolean leaf = true;
					if (Type.GetInt(objs[2]) > 0) {
						leaf = false;
					}
					treeLoader.addTreeNode(name, "dept-old-" + objs[0] + "",
							leaf, cls, "");
				}
				return treeLoader.toString();
			}
		}

		if (type != 1) {
			JsonConfig config = new JsonConfig();
			config.setIgnoreDefaultExcludes(false);
			config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);
			if ("comp".equals(strs[0])) {
				id = Type.GetInt(strs[2]);
				List<Object[]> list = organizationDAO
						.getOrgTree(FIND_BDEPT, id);
				for (Object[] objs : list) {
					String name = String.valueOf(objs[1]).trim();
					String cls = "file";
					if ("true".equals(objs[3])) {
						// 禁用的事业部
						continue;
					}
					treeLoader.addTreeNode(name, "dept-old-" + objs[0] + "",
							false, cls, "");// false
				}
				return treeLoader.toString(config);
			}
			if ("dept".equals(strs[0])) {
				id = Type.GetInt(strs[2]);
				List<Object[]> list = organizationDAO.getOrgTree(FIND_DEPT, id);
				for (Object[] objs : list) {
					String objID = null;
					String cls = "file";
					String icon = "";
					String name = String.valueOf(objs[1]).trim();
					boolean leaf = true;
					if ("0".equals(String.valueOf(objs[3]).trim())) {
						objID = "empl-old-" + String.valueOf(objs[2]).trim();
						icon = "";
						if (1 == Integer.valueOf(objs[4] + "")) {
							// 禁用的人
							continue;
						}
					} else {
						objID = "dept-old-" + String.valueOf(objs[0]).trim();
						leaf = false;
						if (1 == Integer.valueOf(objs[4] + "")) {
							// 禁用的部门
							continue;
						}
					}
					treeLoader.addTreeNode(name, objID, leaf, cls, icon);
				}
				return treeLoader.toString(config);
			}
		}
		return treeLoader.toString();

	}

	public TreeLoaderResponse getOrganizationTree(OrgTreeParam param) {
		TreeLoaderResponse loaderResponse = new TreeLoaderResponse();
		List<Object[]> list = organizationDAO.getOrgTree(param);
		for (Iterator<Object[]> it = list.iterator(); it.hasNext();) {
			Object[] objs = it.next();
			int level = Integer.parseInt(objs[2].toString());
			if (level > param.getLeafLevel()) {
				continue;
			}
			String objID = OrgTreeParam.getNodeId(level, objs[0].toString()
					.trim());
			String name = objs[1].toString().trim();
			String iconCls = objs[3].toString();
			boolean leaf = (param.getLeafLevel() == level ? true : (Integer
					.parseInt(objs[4].toString()) == 0));
			TreeNode tnode = loaderResponse.addTreeNode(name, objID, leaf,
					"file", iconCls);
			tnode.addAttribute("id", objs[0].toString());
			tnode.addAttribute("name", name);
			tnode.addAttribute("level", String.valueOf(level));
		}
		if (StringUtils.isNotEmpty(param.getAttributor())) {
			TreeNodeAttributor attributor = SpringInit.getBean(
					param.getAttributor(), TreeNodeAttributor.class);
			attributor.opt(loaderResponse.getNodes());
		}
		return loaderResponse;
	}

	@SuppressWarnings("rawtypes")
	public List getEmployeeInfoByQuery(StandardQuery query) {
		return organizationDAO.getEmployeeInfoByQuery(query);
	}

	@SuppressWarnings("rawtypes")
	public List getEmployeeInfoByDeptID(int deptNo) {
		logger.info("---->addressBook(service--employeeInfoByDeptID)");
		logger.info(deptNo);
		if(deptNo==-1){
			return organizationDAO.getAllEmpInfo();
		}
		
		return organizationDAO.getEmpInfoByDeptID(deptNo);
	}

	// public List<EmployeeObj> getLeaderInfo() {
	public List getLeaderInfo() {
		logger.info("---->addressBook(service--getLeaderInfo)");
		logger.info(organizationDAO.getLeaderInfo());
		return organizationDAO.getLeaderInfo();
	}

	@SuppressWarnings("rawtypes")
	public String getEmpListByName(String compID, String name, String isAdmin) {
		List list = organizationDAO.getEmpListByName(compID, name);
		if (isAdmin != null && isAdmin.equals("true")) {
			list = organizationDAO.getEmpListByNameAdmin(compID, name);
		}
		if (list != null && list.size() > 0) {
			String userID = ((Map) list.get(0)).get("EMPID").toString();
			String nodePath = getEmpNodePath(Type.GetInt(userID));
			return String.valueOf(nodePath);
		} else {
			return "";
		}
	}

	public String getCustomGroupTree(String node, String groupID) {
		logger.info("---> getCustomGroupTree (serivce impl) node = " + node);
		logger.info("---> getCustomGroupTree (serivce impl) groupID = "
				+ groupID);

		List<TreeNode> nodes = getCustomChildrenTreeNodes(node, groupID);
		TreeLoaderResponse treeLoader = new TreeLoaderResponse();
		JsonConfig config = new JsonConfig();
		config.setExcludes(new String[] { "checked" });
		config.setIgnoreDefaultExcludes(false);
		config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);
		treeLoader.setNodes(nodes);
		return treeLoader.toString(config);
	}

	public List<TreeNode> getCustomChildrenTreeNodes(String node, String groupID) {

		String[] strs = StringUtils.split(node, "_");
		List<TreeNode> nodes = new ArrayList<TreeNode>();
		if ("root".equals(strs[0])) {
			List<Object[]> list = organizationDAO.getCustomGroupTree(groupID);

			for (Object[] objs : list) {
				SyncTreeNode tnode = new SyncTreeNode();
				tnode.addAttribute("groupid", Type.getString(objs[0]).trim());
				tnode.addAttribute("father_groupid", Type.getString(objs[1])
						.trim());
				tnode.addAttribute("type", Type.getString(objs[2]).trim());
				tnode.addAttribute("name", Type.getString(objs[3]).trim());
				tnode.setText(String.valueOf(objs[3]).trim());
				// tnode.setId("FGroup_" + objs[0] + "__" + node);
				tnode.setId("Group_" + objs[0] + "__" + node);
				tnode.setLeaf(false);
				tnode.setIconCls("comp");
				tnode.setCls("file");
				tnode.setChildren(getCustomChildrenTreeNodes(tnode.getId(),
						(String) objs[0]));
				nodes.add(tnode);
			}

		}
		// if ("FGroup".equals(strs[0]) || "Group".equals(strs[0])) {
		if ("Group".equals(strs[0])) {
			groupID = Type.getString(strs[1]);
			List<Object[]> list = organizationDAO.getCustomGroupTree(groupID);
			for (Object[] objs : list) {
				SyncTreeNode tnode = new SyncTreeNode();
				tnode.addAttribute("groupid", Type.getString(objs[0]).trim());
				tnode.addAttribute("father_groupid", Type.getString(objs[1])
						.trim());
				tnode.addAttribute("type", Type.getString(objs[2]).trim());
				tnode.addAttribute("name", Type.getString(objs[3]).trim());
				tnode.setText(String.valueOf(objs[3]).trim());
				if ("Emp".equals(Type.getString(objs[2]).trim())) {
					tnode.setLeaf(true);
					tnode.setIconCls("male");
					tnode.setId("Emp_" + objs[0] + "__" + node);
				} else if ("Dept".equals(Type.getString(objs[2]).trim())) {
					tnode.setLeaf(true);
					tnode.setIconCls("users");
					tnode.setId("Dept_" + objs[0] + "__" + node);
				} else {
					tnode.setLeaf(false);
					tnode.setIconCls("comp");
					tnode.setId("Group_" + objs[0] + "__" + node);
					tnode.setChildren(getCustomChildrenTreeNodes(tnode.getId(),
							(String) objs[0]));
				}
				tnode.setCls("file");
				nodes.add(tnode);
				// treeLoader.addTreeNode(tnode);
			}
		}
		return nodes;
	}

	public boolean doDeleteCustomGroup(String node, String groupid,
			String father_groupid, String type) {
		boolean deleteSuccess = false;
		if ("Emp".equals(type) || "Dept".equals(type)) {
			deleteSuccess = doDeleteEmpOrDeptOrGroupOfCustomGroup(groupid,
					father_groupid, type);
		}
		if ("Group".equals(type)) {
			// 判断是一处占用，还是多处占用
			int num = getGroupBeUsedNum(groupid);
			if (num > 1) {
				deleteSuccess = doDeleteEmpOrDeptOrGroupOfCustomGroup(groupid,
						father_groupid, type);
			}
			if (num == 1) {
				// 获取子节点，递归删除
				deleteSuccess = doGetAndDeleteCustomChildrenTreeNodes(node,
						groupid)
						&& doDeleteEmpOrDeptOrGroupOfCustomGroup(groupid,
								father_groupid, type);
			}
		}
		return deleteSuccess;
	}

	private boolean doGetAndDeleteCustomChildrenTreeNodes(String node,
			String groupid) {
		boolean deleteSuccess = false;
		List<TreeNode> nodes = getCustomChildrenTreeNodes(node, groupid);
		for (int i = 0; i < nodes.size(); i++) {
			TreeNode n = nodes.get(i);
			deleteSuccess = doDeleteCustomGroup(n.getId(), n.getAttributes()
					.get("groupid"), n.getAttributes().get("father_groupid"), n
					.getAttributes().get("type"));

		}
		if (nodes.size() == 0) {
			deleteSuccess = true;

		}
		return deleteSuccess;
	}

	private boolean doDeleteEmpOrDeptOrGroupOfCustomGroup(String groupid,
			String father_groupid, String type) {
		boolean deleteSuccess = true;
		try {
			organizationDAO.deDeleteEmpOrDeptOrGroupOfCustomGroup(groupid,
					father_groupid, type);
		} catch (Exception e) {
			e.printStackTrace();
			deleteSuccess = false;
		}
		return deleteSuccess;
	}

	private int getGroupBeUsedNum(String groupid) {
		int result = organizationDAO.getGroupBeUsedNum(groupid);
		return result;
	}

	public void doSaveAllOfCustomGroup(Map<String, String> changeGroupName,
			Map<String, String> newGroup, List<List<String>> relationList,
			List<List<String>> ddRelationList, EmployeeObj emp)
			throws Exception {
		logger.info("---> doSaveAllOfCustomGroup (service impl)");

		try {
			// 保存新的自定义组
			Map<String, String> newGroupMap = null;
			if (newGroup != null) {
				newGroupMap = organizationDAO.doAddCustomGroupBatch(newGroup);
				for (Map.Entry entry : newGroupMap.entrySet()) {
					for (List<String> list : relationList) {
						if (list.get(0).equals(entry.getKey())) {
							list.remove(0);
							list.add(0, "Group_" + entry.getValue());
						}
						if (list.get(1).equals(entry.getKey())) {
							list.remove(1);
							list.add(1, "Group_" + entry.getValue());
						}
					}
					if (ddRelationList != null) {
						for (List<String> list : ddRelationList) {
							int i = ((String) entry.getKey()).indexOf("new");
							int j = ((String) entry.getKey()).indexOf("__");
							if (list.get(0).indexOf(
									((String) entry.getKey()).substring(i, j)) != -1) {
								String resource = list.get(0);
								int start = resource.indexOf((String) entry
										.getKey());
								if (start == -1) {
									start = resource.indexOf(((String) entry
											.getKey()).substring(i, j));
								}

								int end = start
										+ entry.getKey().toString().split("__")[0]
												.length();

								String result = resource.substring(0, start)
										+ "Group_" + entry.getValue()
										+ resource.substring(end);
								list.remove(0);
								list.add(0, result);
							}
						}
					}
					// if (changeGroupName != null) {
					// for (List<String> list : changeGroupName) {
					// if (((String) entry.getKey()).indexOf("new") != -1) {
					// String tempNewid = ((String) entry.getKey())
					// .split("__")[0].split("_")[((String) entry
					// .getKey()).split("__")[0].split("_").length - 1];
					// if (list.get(0).indexOf(tempNewid) != -1) {
					// int i = ((String) entry.getKey())
					// .indexOf("new");
					// int j = ((String) entry.getKey())
					// .indexOf("__");
					// String resource = list.get(0);
					// int start = resource.indexOf((String) entry
					// .getKey());
					// if (start == -1) {
					// start = resource
					// .indexOf(((String) entry
					// .getKey()).substring(i,
					// j));
					// }
					//
					// int end = start
					// + entry.getKey().toString()
					// .split("__")[0].length();
					//
					// String result = resource
					// .substring(0, start)
					// + "Group_"
					// + entry.getValue()
					// + resource.substring(end);
					// list.remove(0);
					// list.add(0, result);
					// }
					// }
					// }
					// }
				}
			}

			// 更改组-组映射
			Map<String, String> GroupMapping = null;
			if (newGroup != null) {
				GroupMapping = new HashMap<String, String>();
				for (List<String> list : relationList) {
					if ("Group".equals(list.get(0).substring(0, 5))) {
						GroupMapping.put(list.get(0), list.get(1));
					}
				}
				organizationDAO.doUpdateGroupRelationMappingBatch(GroupMapping);
			}

			// 拖拽保存（人员、部门、组）
			if (ddRelationList != null) {

				List<TreeNode> empDDGroupList = new ArrayList<TreeNode>();
				List<TreeNode> deptDDGroupList = new ArrayList<TreeNode>();
				List<TreeNode> groupDDGroupList = new ArrayList<TreeNode>();
				for (List<String> list : ddRelationList) {
					String temp = list.get(0);

					String groupid = temp.split("__")[0].split("_")[temp
							.split("__")[0].split("_").length - 1];

					String father_groupid = (temp.split("__")[1]).split("_")[(temp
							.split("__")[1]).split("_").length - 1];

					String type = temp.split("_")[1];

					String text = list.get(1);

					if (!isExsitGroup(groupid, father_groupid, type)) {
						TreeNode node = new TreeNode();
						node.setText(text);
						node.addAttribute("name", list.get(1));
						node.addAttribute("groupid", groupid);
						node.addAttribute("father_groupid", father_groupid);
						node.addAttribute("type", type);
						if ("drop_Emp_".equals(temp.substring(0, 9))) {
							node.setId(temp.split("__")[0].substring(9));
							node.setLeaf(true);
							node.setCls("file");
							node.setIconCls("male");
							empDDGroupList.add(node);
						}
						if ("drop_Dept_".equals(temp.substring(0, 10))) {
							node.setId(temp.split("__")[0].substring(10));
							node.setLeaf(true);
							node.setCls("file");
							node.setIconCls("users");
							deptDDGroupList.add(node);
						}
						if ("drop_Group_".equals(temp.substring(0, 11))) {
							node.setId(temp.split("__")[0].substring(11));
							node.setLeaf(false);
							node.setCls("file");
							node.setIconCls("comp");
							groupDDGroupList.add(node);
						}
					}
				}
				organizationDAO.dosaveDDRelationBatch(empDDGroupList,
						deptDDGroupList, groupDDGroupList);
			}

			// 更改组名称
			// Map<String, String> changeGroupNameMapping = null;
			if (changeGroupName != null) {
				// organizationDAO.updateCustomGroupNameBatch(changeGroupName);

				// changeGroupNameMapping = new HashMap<String, String>();
				// for (List<String> list : changeGroupName) {
				// changeGroupNameMapping.put(list.get(0), list.get(1));
				// }
				organizationDAO.updateCustomGroupNameBatch(changeGroupName);
			}

		} catch (Exception e) {
			e.printStackTrace();
			throw e;
		}
	}

	private boolean isExsitGroup(String groupid, String father_groupid,
			String type) {
		boolean result = organizationDAO.isExsitCustomGroup(groupid,
				father_groupid, type) > 0;
		return result;
	}

	public Map getEmpInfo(int empID) {
		return organizationDAO.getEmpInfo(empID);
	}

	public List getDeptsByFatherDeptID(int deptID) {
		return organizationDAO.getDeptListByFatherDeptID(deptID);
	}

	public String getCompStoreByEmpName(String empName, PageInfo pageInfo) {
		List list = organizationDAO.getCompInfoByEmpName(empName, pageInfo);
		List resultList = new ArrayList();
		for (int i = 0; i < list.size(); i++) {
			Map map = (Map) list.get(i);
			int row_no = (Integer) map.get("row_no");
			int compid = (Integer) map.get("公司ID");
			String name = (String) map.get("公司名称");
			String abb = (String) map.get("名称缩写 ");
			String nodePath = getEmpNodepathForOrgTreeByName(
					String.valueOf(compid), empName);
			Map resultmap = new HashMap();
			resultmap.put("row_no", row_no);
			resultmap.put("公司ID", compid);
			resultmap.put("公司名称", name);
			resultmap.put("名称缩写", abb);
			resultmap.put("nodePath", nodePath);
			resultList.add(resultmap);
		}
		return (pageInfo != null && pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0) ? WebUtil.buildStoreForJsonWithoutPageInfo(
				resultList, "公司列表") : WebUtil.buildStoreForJson(resultList,
				"公司列表");
	}

	public String getCustomGroupList(EmployeeObj empObj) {
		int userId = empObj.getUserID();
		List list = organizationDAO.getCustomGroupList(String.valueOf(userId));
		return WebUtil.buildStoreForJsonWithoutPageInfo(list);
	}

	public String getCustomGroupListByEmpname(EmployeeObj empObj, String empName) {
		int userId = empObj.getUserID();
		List list = organizationDAO.getCustomGroupList(String.valueOf(userId));// 一级组list
		List resultList = new ArrayList();
		for (int i = 0; i < list.size(); i++) {
			Map map = (Map) list.get(i);
			Map existmap = IsExistEmpnameIntheGroup(
					String.valueOf(map.get("groupid")), empName);
			if (existmap.size() > 0 && (Boolean) existmap.get("result")) {
				List currentgrouplist = organizationDAO
						.getCustomGroupListByGroupid(String.valueOf(map
								.get("groupid")));
				Map tempMap = (Map) currentgrouplist.get(0);
				String groupid = (String) tempMap.get("groupid");
				String father_groupid = (String) tempMap.get("father_groupid");
				String type = (String) tempMap.get("type");
				String groupname = (String) tempMap.get("groupname");
				String nodetype = (String) existmap.get("nodetype");
				String nodeid = (String) existmap.get("nodeid");

				Map resultmap = new HashMap();
				resultmap.put("groupid", groupid);
				resultmap.put("father_groupid", father_groupid);
				resultmap.put("type", type);
				resultmap.put("groupname", groupname);
				resultmap.put("nodetype", nodetype);
				resultmap.put("nodeid", nodeid);
				int compID = 0;
				if ("Dept".equals(nodetype)) {
					compID = organizationDAO.getCompIDByDeptID(Type
							.GetInt(nodeid));
					resultmap.put("compid", compID);
				} else if ("Emp".equals(nodetype)) {
					compID = organizationDAO.getCompIDByDeptID(organizationDAO
							.getDeptIDByUserID(Type.GetInt(nodeid)));
					resultmap.put("compid", compID);
				}
				resultmap.put(
						"nodePath",
						getEmpNodepathForOrgTreeByName(String.valueOf(compID),
								empName));
				resultList.add(resultmap);
			}
		}
		return WebUtil.buildStoreForJsonWithoutPageInfo(resultList);
	}

	/**
	 * 判断人员是否存在在指定通讯录自定义组中
	 * 
	 * @param groupid
	 * @param empName
	 * @param result
	 *            result(传参时，默认传false)
	 * */
	private Map IsExistEmpnameIntheGroup(String groupid, String empName) {
		List list = organizationDAO.getCustomGroupList(groupid);
		Map resultmap = new HashMap();
		for (int i = 0; i < list.size(); i++) {
			Map map = (Map) list.get(i);
			String type = (String) map.get("type");
			if ("Emp".equals(type)) {
				if (empName.equals((String) map.get("groupname"))) {
					resultmap.put("result", true);
					resultmap.put("nodetype", "Emp");
					resultmap.put("nodeid", (String) map.get("groupid"));
					break;
				}
			} else if ("Dept".equals(type)) {
				int deptID = Type.GetInt(map.get("groupid"));
				List empInfo = getAllEmpByDeptID(deptID, new ArrayList());
				for (int j = 0; j < empInfo.size(); j++) {
					Map emp = (Map) empInfo.get(j);
					if (empName.equals((String) emp.get("userName"))) {
						resultmap.put("result", true);
						resultmap.put("nodetype", "Dept");
						resultmap.put("nodeid", (String) map.get("groupid"));
						break;
					}
				}
			} else {
				resultmap = IsExistEmpnameIntheGroup(
						String.valueOf(map.get("groupid")), empName);
			}
			if (resultmap.size() > 0)
				break;
		}
		return resultmap;
	}

	/**
	 * 获取指定部门内的所有成员
	 * 
	 * @param deptID
	 * @param empInfo
	 *            empInfo(传参时，默认传 new ArraList() ，即空list)
	 * */
	private List getAllEmpByDeptID(int deptID, List empInfo) {
		List deptList = organizationDAO.getDeptListByFatherDeptID(deptID);
		if (deptList.size() == 0) {
			List tempList = organizationDAO.getEmpInfoByDeptID(deptID);
			for (int i = 0; i < tempList.size(); i++) {
				empInfo.add(empInfo.size(), tempList.get(i));
			}
		} else {
			List tempList_1 = organizationDAO.getEmpInfoByDeptID(deptID);
			for (int i = 0; i < tempList_1.size(); i++) {
				empInfo.add(empInfo.size(), tempList_1.get(i));
			}
			for (int j = 0; j < deptList.size(); j++) {
				Map map = (Map) deptList.get(j);
				List tempList_2 = getAllEmpByDeptID(
						Type.GetInt(map.get("部门ID")), empInfo);
			}
		}
		return empInfo;
	}

	public String getPathNameByPathId(String pathId) {
		if ("".equals(pathId) || pathId.length() < 1) {
			return "";
		}
		String deptNames = "";
		String[] array = pathId.split(",");
		SimpleEmployee emp = organizationDAO.getSimpleEmployee(Type
				.GetInt(array[0]));
		String empName = "";
		if (emp != null) {
			empName = emp.getUserName() + ",";
		}
		String compid = array[array.length - 1];
		String compName = "";
		CompanyObj comp = crmDAO.getCompanyObjById(Type.GetInt(compid));
		if (comp != null) {
			compName = comp.getCompanyName();
		}
		for (int i = 1; i < array.length - 1; i++) {
			deptNames += organizationDAO.getDeptNameByDeptId(Type
					.GetInt(array[i])) + ",";

		}
		return empName + deptNames + compName;
	}

	public List getDisabledReasonComb() {
		return organizationDAO.getDisabledReasonComb();
	}

	public String doUpdateDisableReasonAndDate(int empId, String reason,
			String disable) {
		organizationDAO.updateDisableReason(empId, reason, disable);
		if ("1".equals(disable)) {
			DisabledReason disabledReason = organizationDAO
					.getDisabledReason(reason);
			return disabledReason.getName() + "&" + disabledReason.getId();
		}
		return reason + "&";

	}

	public int doSavePostCardFrontPic(int userID, String postCardFrontName,
			int updateID) {
		try {
			return organizationDAO.doSavePostCardFrontPic(userID,
					postCardFrontName, updateID);
		} catch (Exception e) {
			e.printStackTrace();
			return -1;
		}
	}

	public int doSavePostCardBackPic(int userID, String postCardBackName,
			int updateID) {
		try {
			return organizationDAO.doSavePostCardBackPic(userID,
					postCardBackName, updateID);
		} catch (Exception e) {
			e.printStackTrace();
			return -1;
		}
	}

	public void doUpdatePostCard(int empId, Boolean isFront) {
		organizationDAO.doUpdatePostCard(empId, isFront);
	}

	/**
	 * 获取 OrgTree 人员节点路径
	 * 
	 * @param empID
	 * */
	private String getEmpOfOrgTreeNodePath(int empID) {
		int deptID = organizationDAO.getDeptIDByUserID(empID);
		String nodepath = getDeptOfOrgTreeNodePath(deptID) + "/emp-" + empID;
		return nodepath;
	}

	/**
	 * 获取 OrgTree 部门节点路径
	 * 
	 * @param deptID
	 * */
	private String getDeptOfOrgTreeNodePath(int deptID) {
		String nodepath = "";
		try {
			String deptStr = getDeptStr(deptID, deptID + "-");
			if ("".equals(deptStr)) {
				int compID = organizationDAO.getCompIDByDeptID(deptID);
				nodepath = "/comp-" + compID + "/dept1-" + deptID;
			} else {
				String[] arr = deptStr.substring(0, deptStr.length() - 1)
						.split("-");
				int compID = organizationDAO.getCompIDByDeptID(Type
						.GetInt(arr[arr.length - 1]));
				String str = "";
				for (int i = 0; i < arr.length; i++) {
					str += "/dept1-" + arr[arr.length - 1 - i];
				}
				nodepath = "/comp-" + compID + str;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
		return nodepath;
	}

	/**
	 * 通过人员姓名获取 OrgTree 上的节点路径
	 * 
	 * @param compID
	 * @param name
	 * */
	private String getEmpNodepathForOrgTreeByName(String compID, String name) {
		List list = organizationDAO.getEmpListByName(compID, name);
		if (list != null && list.size() > 0) {
			String userID = ((Map) list.get(0)).get("EMPID").toString();
			String nodePath = getEmpOfOrgTreeNodePath(Type.GetInt(userID));
			return String.valueOf(nodePath);
		} else {
			return "";
		}
	}

	public boolean sendMessage(int userid, String empids, String text) {
		boolean success = false;
		Map user = getEmpInfo(userid);
		String userName = (String) user.get("userName");
		String[] emps = empids.split(",");
		List<Map> list = new ArrayList<Map>();
		for (int i = 0; i < emps.length; i++) {
			logger.info("emps[i]:" + emps[i]);
			Map emp = getEmpInfo(Type.GetInt(emps[i]));
			list.add(emp);
		}
		SimpleDateFormat formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");// 设置日期格式
		String date = formatter.format(new java.util.Date());
		date = date.replace(" ", "").replace(":", "").replace("-", "");
		String fileName = "F" + userid + "-" + date + "-"
				+ (new Random().nextInt(100) + 100) + ".txt";
		String filePath = "D:/" + fileName;
		File file = new File(filePath);
		try {
			BufferedWriter writer = new BufferedWriter(new FileWriter(file));
			String fileContents = "";
			for (int j = 0; j < list.size(); j++) {
				String mobileNo = (String) list.get(j).get("mobileNo");
				if (mobileNo != null && !("".equals(mobileNo))) {
					fileContents += mobileNo.trim() + ",";
				}
			}
			fileContents = fileContents + "\r\n" + text.trim() + "(" + userName
					+ "发送)";

			writer.write(fileContents);
			writer.close();

			boolean flag = FTPUploadFileUtil.upload(file,
					InitActionServlet.properties.getProperty("sms.ftp.ip"),
					InitActionServlet.properties.getProperty("sms.ftp.user"),
					InitActionServlet.properties
							.getProperty("sms.ftp.password"), "/");
			if (flag) {

				success = true;
			}
		} catch (IOException e) {
			success = false;
			e.printStackTrace();
		}

		logger.info("sendMessage:" + fileName);
		return success;
	}

	/**
	 * 判断是否有缺陷管理的权限,即是监控事业部、水利部门的人员
	 * 
	 * @param empId
	 * @return
	 */
	public boolean isBugUser(int empId) {
		String pathID = organizationDAO.getEmpPathId(empId);
		pathID = pathID.substring(0, pathID.lastIndexOf(","));
		String divisionId = pathID.substring(pathID.lastIndexOf(",") + 1,
				pathID.length());
		if ("1".equals(divisionId) || "2".equals(divisionId)) {
			return true;
		}
		return false;
	}

	/**
	 * 获取新添加的角色
	 * 
	 * @param empId
	 * @return
	 */
	public int getDefaultRole(int empId) {
		String pathID = organizationDAO.getEmpPathId(empId);
		String deptIdStr = pathID.substring(pathID.indexOf(",") + 1);
		String deptId = deptIdStr.substring(0, deptIdStr.indexOf(','));
		if ("6".equals(deptId)) {
			return 55;
		}
		return 25;
	}

	public void saveUserTopicMessageHabit(String tipID, int userID,
			String[] messageIDs) {
		for (int j = 0; j < messageIDs.length; j++) {
			organizationDAO.saveUserTopicMessageHabit(tipID, userID,
					messageIDs[j]);
		}
	}

	public String getMessageByUserIDAndTopID(String topicID, int userID) {
		List list = organizationDAO.getMessageByUserIDAndTopID(topicID, userID);
		return WebUtil.buildStoreForJsonWithoutPageInfo(list);
	}

	public boolean sendAnnouncementMessage(int userid, String text,
			String topic_type) {
		String message_id = UUIDUtils.nextCode();
		int i = organizationDAO.doInsertMessageDefinition(message_id, userid,
				text, topic_type);
		int j = organizationDAO.doInsertMessageRecord(message_id);
		if (i > 0 && j > 0) {
			return true;
		} else {
			return false;
		}
	}

	public void sendPersonalMessage(int userid, String text, String topic_type,
			String empIds) {
		String message_id = UUIDUtils.nextCode();
		organizationDAO.doInsertPersonMessageDefinition(message_id, userid,
				text, topic_type);
		String[] empids = empIds.split(",");
		for (int i = 0; i < empids.length; i++) {
			organizationDAO.savePersonalMessageRecord(message_id,
					Type.GetInt(empids[i]));
		}
	}

	public SimpleDeptObj getSimpleDeptObj(int deptID) {
		return organizationDAO.getSimpleDeptObj(deptID);
	}

	public List getZentaoDataFromEmployee() {
		List list = organizationDAO.getZentaoDataFromEmployee();
		return list;
	}
	
	//2013-11-06 zentao代码注释
/*	public void doInsertZentaoData() {
		List list = organizationDAO.getZentaoDataFromEmployee();
		boolean f = zentaoDAO.doInsertZentaoData(list);//用户表
		System.out.println("***********************************************"+f);
		boolean ff = zentaoDAO.doInsertZentaoUserGroupData(list);//用户分组表
		System.out.println("***********************************************"+ff);
	}

	public void doUpdateZentaoPwd(int userIDInt, String password) {

		String userIDStr = Type.getString(userIDInt);
		if (userIDInt < 0) {
			userIDStr = "000n" + userIDStr.substring(1);
		} else {
			userIDStr = "000p" + userIDStr;
		}
		zentaoDAO.doUpdateZentaoPwd(userIDStr, TemplateUtil.getMd5(password));

	}*/

	//	public String getHaveReceivedMessageByUseId(int userid) {
	//		List list = organizationDAO.getHaveReceivedMessageByUseId(userid);
	//		return WebUtil.buildStoreForJsonWithoutPageInfo(list);
	//	}

	public List getPersonByDeptID(String detpId) {
		return organizationDAO.getPersonByDeptID(Type.GetInt(detpId));
	}
	
	public List getAllDivisionDept(int companyID) {
		return organizationDAO.getAllDivisionDept(companyID);
	}
	
	public List getSendMessageAfterProcessInstance(String processInstanceId){
		return organizationDAO.getSendMessageAfterProcessInstance(processInstanceId);
	}

	public int doSavePostMaterials(String materialsID, String imgName) {
		try {
			return organizationDAO.doSavePostMaterials(materialsID,imgName);
		} catch (Exception e) {
			e.printStackTrace();
			return -1;
		}
	}
	@SuppressWarnings({ "unchecked", "rawtypes" })
	public List getListWithPageInfo(List list, PageInfo pageInfo) {
		List resultList = new ArrayList();
		int limit = pageInfo.getLimit();
		int start = pageInfo.getStart();
		int size = list.size();
		int limitSize = start + limit;
		if (limitSize > size) {
			limitSize = size;
		}
		if (limit == 0 || list.size() < limit) {
			return list;
		} else {
			for (int i = start; i < limitSize; i++) {
				resultList.add(list.get(i));
			}
			return resultList;
		}

	}
	@Override
	public String getDutyPlanList(PageInfo pageInfo,String department,String onWatchDate) {
		if(department == null && onWatchDate == null){
			 List list = organizationDAO.getDutyPlanList(pageInfo);
			 List alist = getListWithPageInfo(list,pageInfo);
				return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(alist,list.size());
		}else if(Type.GetInt(department) == -1 || "".equals(department)){
			 if(onWatchDate == null || "".equals(onWatchDate)){
				 List list = organizationDAO.getDutyPlanList(pageInfo);
				 List alist = getListWithPageInfo(list,pageInfo);
					return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(alist,list.size());
			 }else{
				 List list = organizationDAO.getDutyPlanListByDate(pageInfo,onWatchDate);
				 List alist = getListWithPageInfo(list,pageInfo);
					return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(alist,list.size());
			 }
			
		}else{
			 if(onWatchDate == null || "".equals(onWatchDate)){
				 List list = organizationDAO.getDutyPlanListByPart(pageInfo,department);
				 List alist = getListWithPageInfo(list,pageInfo);
					return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(alist,list.size());
			 }else{
				 List list = organizationDAO.getDutyPlanListByPartAndDate(pageInfo,department,onWatchDate);
				 List alist = getListWithPageInfo(list,pageInfo);
					return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(alist,list.size());
			 }
		}
	}

	@Override
	public void doSaveDutyPlan(T_OnWatch_schedule tOsc) {
		organizationDAO.saveDutyPlan(tOsc);
		
	}

	@Override
	public void deleteDutyPlan(String id) {
		organizationDAO.deleteDutyPlan(id);
	}

	@Override
	public String getSexCount() {
		List list = organizationDAO.getSexCount();
		return WebUtil.buildJsonStoreByList(list);
	}

	@Override
	public String getSexList(String sex,PageInfo pageInfo) {
		List list = organizationDAO.getSexList(sex,pageInfo);
		 List alist = getListWithPageInfo(list,pageInfo);
		return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(alist,list.size());
	}

	@Override
	public String getZzmmCount() {
		List list = organizationDAO.getZzmmCount();
		return WebUtil.buildJsonStoreByList(list);
	}

	@Override
	public String getZzmmList(String zzmm, PageInfo pageInfo) {
		List list = organizationDAO.getZzmmList(zzmm,pageInfo);
		 List alist = getListWithPageInfo(list,pageInfo);
		return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(alist,list.size());
	}

	@Override
	public String getAgeCount() {
		List list = organizationDAO.getAgeCount();
		return WebUtil.buildJsonStoreByList(list);
	}

	@Override
	public String getAgeList(String age, PageInfo pageInfo) {
		List list = organizationDAO.getAgeList(age,pageInfo);
		 List alist = getListWithPageInfo(list,pageInfo);
		return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(alist,list.size());
	}

	@Override
	public String getDutyCount() {
		List list = organizationDAO.getDutyCount();
		return WebUtil.buildJsonStoreByList(list);
	}

	@Override
	public String getDutyList(String duty, PageInfo pageInfo) {
		List list = organizationDAO.getDutyList(duty,pageInfo);
		 List alist = getListWithPageInfo(list,pageInfo);
		return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(alist,list.size());
	}

	@Override
	public String getTitlesCount() {
		List list = organizationDAO.getTitlesCount();
		return WebUtil.buildJsonStoreByList(list);
	}

	@Override
	public String getTitlesList(String titles, PageInfo pageInfo) {
		List list = organizationDAO.getTitlesList(titles,pageInfo);
		 List alist = getListWithPageInfo(list,pageInfo);
		return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(alist,list.size());
	}

	@Override
	public String getJsjbCount() {
		List list = organizationDAO.getJsjbCount();
		return WebUtil.buildJsonStoreByList(list);
	}

	@Override
	public String getJsjbList(String jsjb, PageInfo pageInfo) {
		List list = organizationDAO.getJsjbList(jsjb,pageInfo);
		 List alist = getListWithPageInfo(list,pageInfo);
		return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(alist,list.size());
	}

	@Override
	public String getWhcdCount() {
		List list = organizationDAO.getWhcdCount();
		return WebUtil.buildJsonStoreByList(list);
	}

	@Override
	public String getWhcdList(String whcd, PageInfo pageInfo) {
		List list = organizationDAO.getWhcdList(whcd,pageInfo);
		 List alist = getListWithPageInfo(list,pageInfo);
		return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(alist,list.size());
	}

	@Override
	public String getZgsfCount() {
		List list = organizationDAO.getZgsfCount();
		return WebUtil.buildJsonStoreByList(list);
	}

	@Override
	public String getZgsfList(String zgsf, PageInfo pageInfo) {
		List list = organizationDAO.getZgsfList(zgsf,pageInfo);
		 List alist = getListWithPageInfo(list,pageInfo);
		return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(alist,list.size());
	}
	@Override
	public String getZzygCount() {
		List list = organizationDAO.getZzygCount();
		return WebUtil.buildJsonStoreByList(list);
	}
	@Override
	public String getZzygList(String zzyg, PageInfo pageInfo) {
		List list = organizationDAO.getZzygList(zzyg,pageInfo);
		 List alist = getListWithPageInfo(list,pageInfo);
		return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(alist,list.size());
	}

	@Override
	public String getDeptManagerList() {
		List list = organizationDAO.getDeptManagerList();
		return WebUtil.buildJsonStoreByList(list);
	}
}
