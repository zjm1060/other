//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\service\\Impl\\OrganizationServiceImpl.java

package com.bitc.jk.xmis.service.impl;

import java.io.File;
import java.io.FileWriter;
import java.io.PrintWriter;
import java.sql.Date;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Iterator;
import java.util.LinkedHashMap;
import java.util.List;
import java.util.Map;

import org.apache.log4j.Logger;
import org.dom4j.Attribute;
import org.dom4j.Document;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;
import org.dom4j.io.OutputFormat;
import org.dom4j.io.XMLWriter;

import com.bitc.jk.xmis.dao.OrganizationDAO;
import com.bitc.jk.xmis.model.CompanyObj;
import com.bitc.jk.xmis.model.DepartmentTree;
import com.bitc.jk.xmis.model.DeptartmentObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.service.OrganizationService;
import com.bitc.jk.xmis.util.DBSchema;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;

import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

//@Transactional(readOnly = true)
public class OrganizationServiceImpl implements OrganizationService {
	private static Logger logger = Logger
			.getLogger(OrganizationServiceImpl.class);

	private OrganizationDAO organizationDAO;

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
					: Type.getString( map.get("position")));

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
	 * @param empID;int
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
			emp.setEmailAddress(Type
					.getString(map.get(DBSchema.Employee.Email)));
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

			emp
					.setLastName(Type.getString(map
							.get(DBSchema.Employee.LastName)));
			emp.setLoginName(Type.getString(map
					.get(DBSchema.Employee.LoginName)));
			emp.setMaritalStatus(Type.GetInt(map
					.get(DBSchema.Employee.MaritalStatus)));
			emp
					.setMobileNo(Type.getString(map
							.get(DBSchema.Employee.MobileNo)));
			emp.setNativePlace(Type.getString(map
					.get(DBSchema.Employee.NativePlace)));
			emp
					.setPassword(Type.getString(map
							.get(DBSchema.Employee.Password)));

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
			emp.setSex(Type.GetInt(map.get(DBSchema.Employee.SexID)));
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
			emp
					.setGraduationDate(map
							.get(DBSchema.EducationLevel.GraduationDate) == null ? null
							: Date
									.valueOf(df
											.format(new java.util.Date(
													(String
															.valueOf(map
																	.get(DBSchema.EducationLevel.GraduationDate)))))));
			emp
					.setMajor(Type.getString(map
							.get(DBSchema.EducationLevel.Major)));
			emp.setEducationRemark(Type.getString(map.get("educationRemark")));

			List aList = organizationDAO.getPoliticalStatusList();
			Map aMap = new LinkedHashMap();
			for (Iterator i = aList.listIterator(); i.hasNext();) {
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
					lunchAddressData.length() - 1)
					+ "]");

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
	public void doSaveDepartmentInfo(DeptartmentObj departmentObj) {

	}

	/**
	 * @param employeeObj
	 * @roseuid 4C1F03AF00DA
	 */
	public void doSaveEmployeeInfo(EmployeeObj employeeObj, EmployeeObj userDNA) {

		logger.info("---> doSaveEmployeeInfo");

		organizationDAO.doSaveEmployeeInfo(employeeObj, userDNA);
	}

	/**
	 * @param employeeID
	 * @return List
	 * @roseuid 4C1F03AF0109
	 */
	public List getProjectListByEmp(int employeeID) {
		return null;
	}

	/**
	 * @param departmentID
	 * @return List
	 * @roseuid 4C1F03AF0157
	 */
	public List getProjectListByDept(int departmentID) {
		return null;
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
		divisionElement.addAttribute("id", "div-"
				+ String.valueOf(userObj.getDivisionID()));
		divisionElement.addAttribute("open", "1");
		divisionElement.addAttribute("im0", "tombs.gif");
		divisionElement.addAttribute("im1", "tombs.gif");
		divisionElement.addAttribute("im2", "iconSafe.gif");
		divisionElement.addAttribute("call", "1");
		divisionElement.addAttribute("select", "1");
		divisionElement.addAttribute("tooltip", String.valueOf(userObj
				.getDivisionID()));

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
				departmentElement.addAttribute("text", deptTree
						.getDeptDisabledFlag() == 1 ? redFont
						+ deptTree.getDepartName() + endFont : deptTree
						.getDepartName());
				departmentElement.addAttribute("id", "dept-"
						+ String.valueOf(deptTree.getDepartID()));
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
					employeeElement.addAttribute("text", deptTree
							.getEmpDisabledFlag() == 1 ? redFont
							+ deptTree.getUserName() + endFont : deptTree
							.getUserName()
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
														: String
																.valueOf(deptTree
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

		logger.info(oldEduName + "//" + emp.getEducationLevelName());

		try {
			if (oldEduName.equals(emp.getEducationLevelName())) {
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

	// @Transactional(readOnly = false, propagation = Propagation.REQUIRES_NEW,
	// rollbackFor = Exception.class)
	public int doCreateNewBatch(Map deptMap, Map empMap, Map empDeptMapping,
			EmployeeObj operator) throws Exception {

		logger.info("--->	doCreateNewBatch");

		try {
			if (deptMap != null) {
				Map newDeptMap = organizationDAO.doCreateDeptBatch(operator
						.getDivisionID(), deptMap, operator.getUserID());

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
							newMapping.put(entry.getKey(), newDeptMap.get(entry
									.getValue()));
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

	public int doDisableTreeNode(String itemNode, EmployeeObj userObj)
			throws Exception {

		logger.info("--->	doDisableTreeNode//" + itemNode.substring(0, 4)
				+ "//" + itemNode.substring(4));

		try {
			String prefix = itemNode.substring(0, 4);
			String id = itemNode.substring(4);

			if (itemNode.indexOf("emp-") == 0)
				organizationDAO.doDisableEmployee(Integer.parseInt(itemNode
						.substring(4)), userObj.getUserID());
			else
				organizationDAO.doDisableDepartment(Integer.parseInt(itemNode
						.substring(5)), userObj.getUserID());

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
							+ String
									.valueOf(map
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
}
