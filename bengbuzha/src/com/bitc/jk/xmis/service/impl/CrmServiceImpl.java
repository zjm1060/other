package com.bitc.jk.xmis.service.impl;

import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.springframework.dao.EmptyResultDataAccessException;
import org.springframework.dao.IncorrectResultSizeDataAccessException;

import com.bitc.jk.xmis.dao.CrmDAO;
import com.bitc.jk.xmis.dao.OrganizationDAO;
import com.bitc.jk.xmis.model.CompanyObj;
import com.bitc.jk.xmis.model.CompanySimpleObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.MeetingRoomInformation;
import com.bitc.jk.xmis.model.OfficeSuppliesDetails;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.PatentInformation;
import com.bitc.jk.xmis.model.ProjectQualification;
import com.bitc.jk.xmis.model.QualificationObj;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.model.SupplierInformation;
import com.bitc.jk.xmis.model.VehicleManagement;
import com.bitc.jk.xmis.model.crm.Address;
import com.bitc.jk.xmis.model.crm.RelativeAddress;
import com.bitc.jk.xmis.model.sys.RelationalOperator;
import com.bitc.jk.xmis.service.CrmService;
import com.bitc.jk.xmis.util.DBSchema;
import com.bitc.jk.xmis.util.SQLScript;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.exception.DuplicatedException;
import com.bitc.jk.xmis.util.sql.CompositParam;
import com.bitc.jk.xmis.util.sql.SimpleParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public class CrmServiceImpl implements CrmService {
	private static Logger logger = Logger.getLogger(CrmServiceImpl.class);

	private CrmDAO crmDAO;
	private OrganizationDAO organizationDAO;

	public OrganizationDAO getOrganizationDAO() {
		return organizationDAO;
	}

	public void setOrganizationDAO(OrganizationDAO organizationDAO) {
		this.organizationDAO = organizationDAO;
	}

	/**
	 * @return the crmDAO
	 */
	public CrmDAO getCrmDAO() {
		return crmDAO;
	}

	/**
	 * @param crmDAO
	 *            the crmDAO to set
	 */
	public void setCrmDAO(CrmDAO crmDAO) {
		this.crmDAO = crmDAO;
	}

	private int companySimpleValidate(CompanySimpleObj comp) {

		logger.info("---> companySimpleValidate ? type = " + comp.getType());

		try {
			if (WebKeys.New_Company_Simple_Type_Company.equals(comp.getType()))
				return crmDAO.companyValidate(comp.getCompanyName());

			else if (WebKeys.New_Company_Simple_Type_Department.equals(comp
					.getType()))
				return crmDAO.departmentValidate(comp.getDeptName(),
						comp.getCompanyID());

			else if (WebKeys.New_Company_Simple_Type_ContactPerson.equals(comp
					.getType()))
				return crmDAO.empValidate(comp.getEmpName(),
						comp.getCompanyID());

			else
				return 0;
		} catch (Exception e) {
			e.printStackTrace();
			return -1;
		}

	}

	public String dataValidate(CompanySimpleObj comp) {

		logger.info("---> dataValidate ? type = " + comp.getType());

		String msgStr = "";
		try {
			if (WebKeys.New_Company_Simple_Type_Company.equals(comp.getType())) {
				if (crmDAO.companyValidate(comp.getCompanyName()) > 0)
					msgStr = "公司：" + comp.getCompanyName();
			}

			else if (WebKeys.New_Company_Simple_Type_Department.equals(comp
					.getType())) {
				if (crmDAO.departmentValidate(comp.getDeptName(),
						comp.getCompanyID()) > 0)
					msgStr = "部门：" + comp.getDeptName();

			} else if (WebKeys.New_Company_Simple_Type_ContactPerson
					.equals(comp.getType())) {
				if (comp.getDeptID() == 0)
					// 得到部门
					comp.setDeptID(crmDAO.getDefaultDeptByCompany(comp
							.getCompanyID()));

				if (crmDAO.empValidate(comp.getEmpName(), comp.getCompanyID(),
						comp.getDeptID()) > 0)
					msgStr = "联系人：" + comp.getEmpName();
			}

		} catch (EmptyResultDataAccessException Ee) {
			Ee.printStackTrace();
			// return -1;
		} catch (IncorrectResultSizeDataAccessException Ie) {
			Ie.printStackTrace();

			if (WebKeys.New_Company_Simple_Type_Company.equals(comp.getType())) {
				msgStr = "公司：" + comp.getCompanyName();
			}

			else if (WebKeys.New_Company_Simple_Type_Department.equals(comp
					.getType())) {
				msgStr = "部门：" + comp.getDeptName();

			} else if (WebKeys.New_Company_Simple_Type_ContactPerson
					.equals(comp.getType())) {
				msgStr = "联系人：" + comp.getEmpName();
			}

		}

		return "".equals(msgStr) ? "" : msgStr + " 已存在，请确认是否新建？";
	}

	public String doCreateNewCompanySimple(CompanySimpleObj comp,
			EmployeeObj userDNA) throws DuplicatedException, Exception {

		logger.info("---> doCreateNewCompanySimple ? type = " + comp.getType());

		String returnInfo = "";

		try {
			// if (companySimpleValidate(comp) > 0) {
			//
			// String errMsg = "";
			// if (WebKeys.New_Company_Simple_Type_Company.equals(comp
			// .getType()))
			// errMsg = comp.getCompanyName();
			// else if (WebKeys.New_Company_Simple_Type_Department.equals(comp
			// .getType()))
			// errMsg = comp.getDeptName();
			// else if (WebKeys.New_Company_Simple_Type_ContactPerson
			// .equals(comp.getType()))
			// errMsg = comp.getEmpName();
			// throw new DuplicatedException(errMsg + " 已经存在，请重新输入！");
			// } else {

			// create new company
			if (WebKeys.New_Company_Simple_Type_Company.equals(comp.getType())) {

				if (comp.getCompanyAbb() == null
						|| "".equals(comp.getCompanyAbb()))
					comp.setCompanyAbb(comp.getCompanyName().trim());

				comp.setCompanyID(crmDAO
						.doCreateNewCompanySimple(comp, userDNA));

				// create new department
				if (comp.getDeptName() == null || "".equals(comp.getDeptName())) {
					comp.setDeptName(WebKeys.New_Company_Simple_For_Dept_Other);
				}
				comp.setDeptAbb(comp.getDeptName());
				comp.setDeptID(crmDAO.doCreateNewDeptSimple(comp, userDNA));
				comp.setDeptName(WebKeys.New_Company_Simple_For_Dept_Other
						.equals(comp.getDeptName()) ? "" : comp.getDeptName());
				comp.setDepartmentDirectlyFlag(1);
				crmDAO.doSetDeptBelongToCompanyBySimple(comp, userDNA);

				// create new crm user
				if (comp.getEmpName() != null && !"".equals(comp.getEmpName()))
					comp.setEmpID(crmDAO.doCreateNewContactPersonSimple(comp,
							userDNA));
			} else if (WebKeys.New_Company_Simple_Type_Department.equals(comp
					.getType())) {
				comp.setDeptAbb(comp.getDeptName());
				comp.setDeptID(crmDAO.doCreateNewDeptSimple(comp, userDNA));
				comp.setDepartmentDirectlyFlag(1);
				crmDAO.doSetDeptBelongToCompanyBySimple(comp, userDNA);
			} else if (WebKeys.New_Company_Simple_Type_ContactPerson
					.equals(comp.getType())) {

				if (comp.getDeptID() == 0) {
					try {
						comp.setDeptID(crmDAO.getDefaultDeptByCompany(comp
								.getCompanyID()));
					} catch (Exception e) {
						comp.setDeptName(WebKeys.New_Company_Simple_For_Dept_Other);
						comp.setDeptID(crmDAO.doCreateNewDeptSimple(comp,
								userDNA));
						comp.setDepartmentDirectlyFlag(1);
						crmDAO.doSetDeptBelongToCompanyBySimple(comp, userDNA);
					}
				}
				if ("linkMan".equals(comp.getEmpType())) {
					comp.setEmpID(crmDAO.doCreateNewContactLinkPersonSimple(
							comp, userDNA));
				} else {
					comp.setEmpID(crmDAO.doCreateNewContactPersonSimple(comp,
							userDNA));
				}

			}

			if (WebKeys.New_Company_Simple_Type_Company.equals(comp.getType()))
				returnInfo = "compID:'" + comp.getCompanyID() + "',compName:'"
						+ comp.getCompanyName().trim() + "',deptID:'"
						+ comp.getDeptID() + "',deptName:'"
						+ comp.getDeptName() + "',empID:'" + comp.getEmpID()
						+ "',empName:'" + comp.getEmpName() + "'";
			else if (WebKeys.New_Company_Simple_Type_Department.equals(comp
					.getType()))
				returnInfo = "deptID:'" + comp.getDeptID() + "',deptName:'"
						+ comp.getDeptName() + "'";
			else if (WebKeys.New_Company_Simple_Type_ContactPerson.equals(comp
					.getType()))
				returnInfo = "empID:'" + comp.getEmpID() + "',empName:'"
						+ comp.getEmpName() + "'";

			// }

			// } catch (DuplicatedException de) {
			//
			// throw de;

		} catch (Exception e) {
			e.printStackTrace();
			return "";
		}

		logger.info(returnInfo);
		return returnInfo;

	}

	public String getPeasonListByCompany(int compID, EmployeeObj userDNA) {

		logger.info("---> getPeasonListByCompany  ");

		List aList;
		// if (userDNA.getCompanyID() == compID) {
		// // get peason list by self-company
		//
		// aList = crmDAO.getPeasonListBySelfCompany(compID);
		//
		// } else {
		// // get peason list by other-company
		//
		// aList = crmDAO.getPeasonListByCompany(compID);
		//
		// }
		aList = crmDAO.getPeasonListBySelfCompany(compID);
		return WebUtil.buildStoreForJsonWithoutPageInfo(aList,
				WebKeys.Action_Method_Name_ContactPeason);
	}

	public String getContactPersonBySearchName(int compID, String searchName,
			EmployeeObj userDNA) {

		logger.info("---> getContactPersonBySearchName  ");

		List aList;
		// if (userDNA.getCompanyID() == compID) {
		// // get peason list by self-company
		//
		// aList = crmDAO.getPeasonListBySelf(compID, searchName);
		//
		// } else {
		// // get peason list by other-company
		//
		// aList = crmDAO.getPeasonListBySearch(compID, searchName);
		//
		// }

		aList = crmDAO.getPeasonListBySelf(compID, searchName);
		return WebUtil.buildStoreForJsonWithoutPageInfo(aList,
				WebKeys.Action_Method_Name_ContactPeason);
	}

	/*
	 * public String getEmpInfoById(int empID) {
	 * logger.info("---> getEmpInfoById  "); List list =
	 * crmDAO.getEmpInfoById(empID); StringBuilder store = new
	 * StringBuilder("[{"); if (list != null && list.size() > 0) { Map map =
	 * (Map) list.get(0);
	 * store.append("'id':'").append(map.get("人员ID")).append("'");
	 * store.append(",'empName1':'")
	 * .append(Type.getString(map.get("姓氏")).trim())
	 * .append(Type.getString(map.get("名字")).trim()).append("'");
	 * store.append(",'empAddr1':'") .append(map.get("家庭地址") == null ? "" :
	 * Type.getString(map .get("家庭地址"))).append("'");
	 * store.append(",'empPost1':'") .append(map.get("职务") == null ? "" :
	 * Type.getString(map .get("职务"))).append("'");
	 * store.append(",'empMobilePhone1':'") .append(map.get("移动电话") == null ? ""
	 * : Type.getString(map .get("移动电话"))).append("'");
	 * store.append(",'empPhone1':'") .append(map.get("单位电话") == null ? "" :
	 * Type.getString(map .get("单位电话"))).append("'");
	 * store.append(",'empEmail1':'") .append(map.get("电子邮件") == null ? "" :
	 * Type.getString(map .get("电子邮件"))).append("'");
	 * store.append(",'empPostCode1':'") .append(map.get("家庭邮编") == null ? "" :
	 * Type.getString(map .get("家庭邮编"))).append("'"); } store.append("}]");
	 * return store.toString(); }
	 */

	public void updateEmpInfo(EmployeeObj target, EmployeeObj userDNA) {
		logger.info("---> updateEmpInfo  ");
		crmDAO.updateEmpInfo(target, userDNA);
	}

	public CompanyObj getCompanyObjById(int companyID) {
		String inputContactMoney = crmDAO.getInputContactMoney(companyID);
		String outputContactMoney = crmDAO.getOutputContactMoney(companyID);
		CompanyObj company = crmDAO.getCompanyObjById(companyID);
		company.setInputContactMoney(inputContactMoney);
		company.setOutputContactMoney(outputContactMoney);
		return company;
		
	}

	public void doSaveCompanyInfo(CompanyObj cmpObj, String contactEdit,String legalEdit) {
		SimpleEmployee emp = WebUtil.getCurrentEmployee();
		crmDAO.doSaveCompanyInfo(cmpObj);
		//联系人编辑域有内容时候，保存联系人、部门、联系人部门、部门公司
		if(!contactEdit.equals("") || !contactEdit.trim().equals("")){
			String sql = SQLScript.getMaxIDFromTab(DBSchema.Department.DepartmentID,DBSchema.Department.TableName);
			int deptMaxId = organizationDAO.getMaxId(sql);
			int newEmpId = organizationDAO.doCreateUser(deptMaxId, contactEdit,emp.getUserID());//联系人和 人员部门表
			SimpleEmployee contactPerson = new SimpleEmployee();
			contactPerson.setUserID(Type.getInteger(newEmpId));
			cmpObj.setContactPerson(contactPerson);
			crmDAO.doSaveCompanyInfo(cmpObj);
			organizationDAO.doCreateOtherDept(cmpObj.getCompanyID(), deptMaxId,emp.getUserID());//部门 部门公司表
		}
		if(!legalEdit.equals("") || !legalEdit.trim().equals("")){
			String sql = SQLScript.getMaxIDFromTab(DBSchema.Department.DepartmentID,DBSchema.Department.TableName);
			int deptMaxId = organizationDAO.getMaxId(sql);
			int newEmpId = organizationDAO.doCreateUser(deptMaxId, legalEdit,emp.getUserID());//联系人和 人员部门表
			SimpleEmployee legalPerson = new SimpleEmployee();
			legalPerson.setUserID(Type.getInteger(newEmpId));
			cmpObj.setLegalRepresentative(legalPerson);
			crmDAO.doSaveCompanyInfo(cmpObj);
			organizationDAO.doCreateOtherDept(cmpObj.getCompanyID(), deptMaxId,emp.getUserID());//部门 部门公司表
		}
	}

	/**
	 * 获取公司信息列表
	 */
	 public String getCompanyListStoreByQuerys(String params, PageInfo pageInfo) {
		  List aList = crmDAO.getCompanyInfoList(params, pageInfo);
		  if(aList.size()<=1){
			  aList.clear();
		  }
		  return  WebUtil
					.buildStoreForJson(aList,
							WebKeys.Action_Method_Name_CompanyInfoList);
		 }
	/**
	 * 获取下拉列表信息
	 * 
	 * @param category 类别
	 * 
	 * @param fid 外键
	 * 
	 * @return
	 */
	@SuppressWarnings("rawtypes")
	public List getMultiData(String category, int fid) {
		List list = new ArrayList();
		if ("internation".equals(category)) {
			list = crmDAO.getInternationData();
		}
		// -------------------------------
		if ("country".equals(category)) {
			list = crmDAO.getCountryData(fid);
		}
		if ("area".equals(category)) {
			list = crmDAO.getAreaData(fid);
		}
		if ("province".equals(category)) {
			list = crmDAO.getProvinceData(fid);
		}
		if ("city".equals(category)) {
			list = crmDAO.getCityData(fid);
		}
		// -------------------------------
		if ("businessRelation".equals(category)) {
			list = crmDAO.getBusinessRelationCombox();
		}
		if ("companyScale".equals(category)) {
			list = crmDAO.getCmpScaleCombox();
		}
		if ("creditLevel".equals(category)) {
			list = crmDAO.getCreditLevelCombox();
		}
		if ("customerLevel".equals(category)) {
			list = crmDAO.getCustomerCombox();
		}
		return list;
	}

	/**
	 * 删除一条或者多条公司信息 【 未用】
	 */
	public void dodelCompanyInfoById(String ids) {
		logger.info("dodelCompanyInfoById//ids" + ids);
		String[] companyIDs = ids.split(",");
		for (int i = 0; i < companyIDs.length; i++) {
			int id = Type.getInteger(companyIDs[i]);
			crmDAO.dodelCompanyInfoById(id);
		}
	}
	
	/** 
	 * 获取相关地址信息列表  模糊查询
	 * @param companyID
	 * @param isNeedCompID null
	 * @param query
	 */
	public String getRelativeAddressListM(StandardQuery query, String companyID,
			PageInfo pageInfo) {
		if (!StringUtils.isEmpty(companyID)) {
			query = CompositParam
					.buildAndCompositParam()
					.addStandardQuery(
							SimpleParam.buildSimpleParam("companyId",
									Integer.valueOf(companyID), Type.INTEGER,
									RelationalOperator.EQ))
					.addStandardQuery(query);
		}
		if(query != null){
			List<RelativeAddress> list = crmDAO.getRelativeAddressListM(query, pageInfo);
			return WebUtil.buildJsonStoreByList(list);
		} else {
			return null;
		}
	}

	
	/**  
	 * 获取地址信息列表 包括公司的地址信息
	 * @param companyID
	 * @param isNeedCompID 
	 * @param query
	 */
	@SuppressWarnings("unchecked")
	public String getAllAddressList(StandardQuery query, String companyID,
			String isNeedCompID) {
		if (!StringUtils.isEmpty(companyID)) {
			query = CompositParam
			.buildAndCompositParam()
			.addStandardQuery(
					SimpleParam.buildSimpleParam("companyId",
							Integer.valueOf(companyID), Type.INTEGER,
							RelationalOperator.EQ))
							.addStandardQuery(query);
			CompanyObj company = crmDAO.getCompanyObjById(Type.getInteger(companyID));
			List<Address> relativeAddressesList= company.getAllAddress();
			Collections.sort(relativeAddressesList, new Comparator(){
				public int compare(Object o1, Object o2) {
					Address a1 = (Address) o1;
					Address a2 = (Address) o2;
					String s1 = "";
					if(a1 != null){
						s1 = a1.getAddressID();
					}
					String s2 = "";
					if(a2 != null){
						s2 = a2.getAddressID();
					}
					int len1 = s1.length();
					int len2 = s2.length();
					int n = Math.min(len1, len2);
					char v1[] = s1.toCharArray();
				    char v2[] = s2.toCharArray();
					int pos = 0;
					while (n-- != 0) {
					   char c1 = v1[pos];
					   char c2 = v2[pos];
					   if (c1 != c2) {
					    return c1 - c2;
					   }
					   pos++;
					}
					 return len1 - len2;
				}});
			return WebUtil.buildJsonStoreByList(relativeAddressesList);
		} else {
			return null;
		}
	}

	/** 
	 * 相关地址信息的编辑、保存
	 */
	public void doSaveRelativeAddress(RelativeAddress relativeAddress) {
		crmDAO.doSaveRelativeAddress(relativeAddress);
	}

	/** 
	 * 根据ID删除一条相关地址信息
	 */
	public void dodelRelativeAddressById(String id) {
		crmDAO.dodelRelativeAddressById(id);
	}

	public EmployeeObj getEmpInfoById(int empID) {
		EmployeeObj emp = crmDAO.getEmpInfoById(empID);
		String pathId=organizationDAO.getEmpPathId(empID); //根据人员id得到 此人的路径   人ID-部门ID-上级部门ID-...-上级部门ID-部门ID-公司ID
//		String pathName=getPathNameByPathId(pathId);
		emp.setPathId(pathId);
//		emp.setPathName(pathName);
		return emp;
	}
	/**
	 * 根据id得到相关地址
	 */
	public RelativeAddress getRelativeAddress(String id) {
		return crmDAO.getRelativeAddress(id);
	}

	public Address getAddress(String id) {
		return crmDAO.getAddress(id);
	}

	public String getAddressIDListByName(String name) {
		List list = crmDAO.getAddressIDListByName(name);
		String selectIds = "";
		if(list!= null && list.size()>0){
			String aid = ((Map) list.get(0)).get("aid").toString();
			String bid = ((Map) list.get(0)).get("bid").toString();
			String cid = ((Map) list.get(0)).get("cid").toString();
			String did = ((Map) list.get(0)).get("did").toString();
			String eid = ((Map) list.get(0)).get("eid").toString();
			selectIds=aid+","+bid+","+cid+","+did+","+eid;
		}
		return selectIds;
	}
	// 人ID-部门ID-上级部门ID-...-上级部门ID-部门ID-公司ID  根据ID查询对应的名字  
	public String getPathNameByPathId(String pathId){
		if("".equals(pathId) ||pathId.length()<1){
			return "";
		}
		String deptNames = "";
		String[] array =  pathId.split(",");
		SimpleEmployee emp = organizationDAO.getSimpleEmployee(Type.GetInt(array[0]));
		String empName = "";
		if(emp != null){
			empName = emp.getUserName()+",";
		}
		String compid = array[array.length-1];
		String compName="";
		CompanyObj comp = crmDAO.getCompanyObjById(Type.GetInt(compid));
		if(comp != null){
			compName = comp.getCompanyName();
		}
		for(int i=1;i<array.length-1;i++){
			deptNames+=organizationDAO.getDeptNameByDeptId(Type.GetInt(array[i]))+",";
			
		}
		return empName+deptNames+compName;
	}


	/**公司地址及相关地址的下拉框
	 * @param companyID
	 * @return
	 */
	@SuppressWarnings("rawtypes")
	public String getRelationAddressCombo(String companyID) {
		List list = crmDAO.getRelationAddressCombo(companyID);
		return WebUtil.buildJsonStoreByList(list);
	}

	public String getRelationAddress(String companyID) {
		// TODO Auto-generated method stub
		List list = crmDAO.getRelationAddress(companyID);
		return WebUtil.buildJsonStoreByList(list);
	}

	public void doSupplierInformation(SupplierInformation si) {
		// TODO Auto-generated method stub
		crmDAO.doSupplierInformation(si);
	}

	public Map<String, String> getSupplierInformation(int companyID) {
		List l = crmDAO.getSupplierInformation(companyID);
		Map<String,String> map=new HashMap<String,String>();
		for(int i=0;i<l.size();i++){
			Map row=(Map)l.get(i);
			map.put("ID",row.get("ID").toString());
			map.put("managerID",row.get("managerID").toString() );
			map.put("addressID",row.get("addressID").toString() );
			map.put("headOfUnit",row.get("headOfUnit").toString() );
			map.put("industryAssessment",row.get("industryAssessment").toString() );
			map.put("standard",row.get("standard").toString() );
			map.put("qualification",row.get("qualification").toString() );
			map.put("generalTaxpayer",row.get("generalTaxpayer").toString() );
			map.put("productsProduction",row.get("productsProduction").toString() );
			map.put("ancillaryProducts",row.get("ancillaryProducts").toString() );
			map.put("qualityAssurance",row.get("qualityAssurance").toString() );
			map.put("introductionLine", row.get("introductionLine").toString() );
			map.put("Yield",row.get("Yield").toString() );
			map.put("newregisterID",row.get("newregisterID").toString() );
			map.put("newupdateDate",row.get("newupdateDate").toString() );
			map.put("lastModifier",row.get("lastModifier").toString() );
			map.put("lastModified",row.get("lastModified").toString() );
			map.put("newregisterName",row.get("newregisterName").toString() );
			map.put("lastModifierName",row.get("lastModifierName").toString() );
			map.put("公司地址",row.get("公司地址").toString());
			map.put("经理姓名",row.get("经理姓名").toString());
			map.put("DH", Type.getString(row.get("DH")));
			map.put("CZ",  Type.getString(row.get("CZ")));
			map.put("Email", Type.getString((String)row.get("Email")));
			
		}
		return map;
	}

	public String getQualificationDictionary() {
		// TODO Auto-generated method stub
		List list = crmDAO.getQualificationDictionary();
		return WebUtil.buildJsonStoreByList(list);
	}

	public void doQualificationInformation(QualificationObj qfo) {
		// TODO Auto-generated method stub
		if ("".equals(qfo.getId())) {
			qfo.setId(null);
		}
		crmDAO.doQualificationInformation(qfo);
	}

	public String getQualificationList(String companyID) {
		// TODO Auto-generated method stub
		List list = crmDAO.getQualificationList(companyID);
		return WebUtil.buildJsonStoreByList(list);
	}

	public String getQualificationAssignment(String id) {
		// TODO Auto-generated method stub
		List list = crmDAO.getQualificationAssignment(id);
		return WebUtil.buildJsonStoreByList(list);
	}

	public String getProjectQualification(String companyID) {
		// TODO Auto-generated method stub
		List list = crmDAO.getProjectQualification(companyID);
		return WebUtil.buildJsonStoreByList(list);
	}

	public void doProcurementInformation(ProjectQualification rq) {
		// TODO Auto-generated method stub
		if ("".equals(rq.getId())) {
			rq.setId(null);
		}
		crmDAO.doProcurementInformation(rq);
	}

	public String getUseProjectQualification(String companyID) {
		List list = crmDAO.getUseProjectQualification(companyID);
		return WebUtil.buildJsonStoreByList(list);
	}

	public String getProjectQualificationAssignment(String id) {
		// TODO Auto-generated method stub
		List list = crmDAO.getProjectQualificationAssignment(id);
		return WebUtil.buildJsonStoreByList(list);
	}

	public String getDecisionMakersInformation(Integer hrID) {
		// TODO Auto-generated method stub
		List list = crmDAO.getDecisionMakersInformation(hrID);
		return WebUtil.buildJsonStoreByList(list);
	}

	public void doPatentInformation(PatentInformation pf) {
		// TODO Auto-generated method stub
		if ("".equals(pf.getSjid())) {
			pf.setSjid(null);
		}
		crmDAO.doPatentInformation(pf);
	}

	public String getPatentInformationList(String companyID) {
		// TODO Auto-generated method stub
		List list = crmDAO.getPatentInformationList(companyID);
		return WebUtil.buildJsonStoreByList(list);
	}

	public String getPatentAssignment(String sjid) {
		// TODO Auto-generated method stub
		List list = crmDAO.getPatentAssignment(sjid);
		return WebUtil.buildJsonStoreByList(list);
	}

	public String getQualificationListC(PageInfo pageInfo, StandardQuery query) {
		// TODO Auto-generated method stub
		List listStore = crmDAO.getQualificationListC(query, pageInfo);
		return WebUtil.buildStoreForJson(listStore, "");
	}

	public String getPatentInfoListC(PageInfo pageInfo, StandardQuery query) {
		// TODO Auto-generated method stub
		List listStore = crmDAO.getPatentInfoListC(query, pageInfo);
		return WebUtil.buildStoreForJson(listStore, "");
	}

	public Map<String, String> getQualificationAssignmentr(String id) {
		List l = crmDAO.getQualificationAssignmentr(id);
		Map<String,String> qualifica=new HashMap<String,String>();
		for(int i=0;i<l.size();i++){
			Map row=(Map)l.get(i);
			qualifica.put("id",row.get("id").toString());
			qualifica.put("companyID",row.get("companyID").toString() );
			qualifica.put("qualificationContactID",row.get("qualificationContactID").toString() );
			qualifica.put("qualificationContact",row.get("qualificationContact").toString() );
			qualifica.put("certificateValidQi",Type.getString(row.get("certificateValidQi").toString()) );
			qualifica.put("certificateValidZhi",Type.getString(row.get("certificateValidZhi").toString()) );
			qualifica.put("certificateNo",row.get("certificateNo").toString() );
			qualifica.put("qualificationRemark",row.get("qualificationRemark").toString() );
			qualifica.put("examined",row.get("examined").toString() );
			qualifica.put("qualificationName",row.get("qualificationName").toString() );
			qualifica.put("departmentName",row.get("departmentName").toString() );
			qualifica.put("newregisterName",row.get("newregisterName").toString() );
			qualifica.put("lastModifierName",row.get("lastModifierName").toString() );
			qualifica.put("lastModified",row.get("lastModified").toString() );
			qualifica.put("newupdateDate",row.get("newupdateDate").toString() );
//			qualifica.put("dianHua",Type.getString(row.get("dianHua").toString()) );
//			qualifica.put("youZhengBM",Type.getString(row.get("youZhengBM").toString()) );
			qualifica.put("TS",Type.getString(row.get("TS").toString()) );
			qualifica.put("DH", Type.getString(row.get("DH")));
			qualifica.put("CZ",  Type.getString(row.get("CZ")));
			qualifica.put("Email", Type.getString((String)row.get("Email")));
			
			
		}
		return qualifica;
	}

	public Map<String, String> getPatentAssignmentr(String sjid) {
		// TODO Auto-generated method stub
		List l = crmDAO.getPatentAssignmentr(sjid);
		Map<String,String> patent=new HashMap<String,String>();
		for(int i=0;i<l.size();i++){
			Map row=(Map)l.get(i);
			patent.put("sjid",row.get("sjid").toString());
			patent.put("companyID",row.get("companyID").toString() );
			patent.put("patentName",row.get("patentName").toString() );
			patent.put("patentsCategory",row.get("patentsCategory").toString() );
			patent.put("patentedDesignPeopleID1",row.get("patentedDesignPeopleID1").toString() );
			patent.put("patentedDesignPeopleID2",Type.getString(row.get("patentedDesignPeopleID2").toString()) );
			patent.put("patentedDesignPeopleID3",Type.getString(row.get("patentedDesignPeopleID3").toString()) );
			patent.put("patentRightPeopleID1",row.get("patentRightPeopleID1").toString() );
			patent.put("patentRightPeopleID2",Type.getString(row.get("patentRightPeopleID2").toString()) );
			patent.put("patentRightPeopleID3",Type.getString(row.get("patentRightPeopleID3").toString()) );
			patent.put("patentRightPeopleID4",Type.getString(row.get("patentRightPeopleID4").toString()) );
			patent.put("patentRightPeopleID5",Type.getString(row.get("patentRightPeopleID5").toString()) );
			patent.put("patentedDesignPeople1",Type.getString(row.get("patentedDesignPeople1") ));
			patent.put("patentedDesignPeople2",Type.getString(row.get("patentedDesignPeople2")) );
			patent.put("patentedDesignPeople3",Type.getString(row.get("patentedDesignPeople3") ));
			patent.put("patentRightPeople1",row.get("patentRightPeople1").toString() );
			patent.put("patentRightPeople2",Type.getString(row.get("patentRightPeople2")) );
			patent.put("patentRightPeople3",Type.getString(row.get("patentRightPeople3")) );
			patent.put("patentRightPeople4", Type.getString(row.get("patentRightPeople4")));
			patent.put("patentRightPeople5",  Type.getString(row.get("patentRightPeople5")));
			patent.put("patentNumber", Type.getString((String)row.get("patentNumber")));
			patent.put("patentCertificateNo", Type.getString((String)row.get("patentCertificateNo")));
			patent.put("applicationTime", Type.getString((String)row.get("applicationTime")));
			patent.put("authorizeTime", Type.getString((String)row.get("authorizeTime")));
			patent.put("patentregisterName", Type.getString((String)row.get("patentregisterName")));
			patent.put("patentlastModifierName", Type.getString((String)row.get("patentlastModifierName")));
			patent.put("patentupdateDate", Type.getString((String)row.get("patentupdateDate")));
			patent.put("patentlastModified", Type.getString((String)row.get("patentlastModified")));
			patent.put("validFrom", Type.getString((String)row.get("validFrom")));
			patent.put("validOnly", Type.getString((String)row.get("validOnly")));
			
		}
		return patent;
	}

	@Override
	public void doMeetingRoomInfo(MeetingRoomInformation mr) {
		// TODO Auto-generated method stub
		
		crmDAO.doMeetingRoomInfo(mr);
	}

	@Override
	public String getMeetingRoomInfo(String room_id) {
		// TODO Auto-generated method stub
		List list = crmDAO.getMeetingRoomInfo(room_id);
		return WebUtil.buildJsonStoreByList(list);
	}

	@Override
	public String getMeetingRoomInfor() {
		
		List list = crmDAO.getMeetingRoomInfor();
		return WebUtil.buildJsonStoreByList(list);
	}

	@Override
	public void dodelMeetingRoomInfoById(String room_id) {
		// TODO Auto-generated method stub
		crmDAO.dodelMeetingRoomInfoById(room_id);
	}

	@Override
	public void doVehicleManagement(VehicleManagement vm) {
		// TODO Auto-generated method stub
		crmDAO.doVehicleManagement(vm);
	}

	@Override
	public String getVehicleManagement() {
		// TODO Auto-generated method stub
		List list = crmDAO.getVehicleManagement();
		return WebUtil.buildJsonStoreByList(list);
	}

	@Override
	public String getVehicleManagemenid(String car_id) {
		// TODO Auto-generated method stub
		
		List list = crmDAO.getVehicleManagemenid(car_id);
		return WebUtil.buildJsonStoreByList(list);
	}

	@Override
	public void dodelVehicleManagemenById(String car_id) {
		// TODO Auto-generated method stub
		crmDAO.dodelVehicleManagemenById(car_id);
	}
	@Override
	public void doOfficeSupplies(OfficeSuppliesDetails osds) {
		// TODO Auto-generated method stub
		crmDAO.doOfficeSupplies(osds);
		
	}

	@Override
	public String getOfficeSupplies(PageInfo pageInfo, String queryStr) {
		// TODO Auto-generated method stub
		String ordersql = WebUtil.getOrderSqlByPageInfo(pageInfo);
		List list = crmDAO.getOfficeSupplies(queryStr,ordersql);
		List aList = getListWithPageInfo(list, pageInfo);
		return WebUtil.buildStoreForJsonWithPageInfoWithDateFormat(aList,list.size());
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
	public String getOfficeSuppliesid(String supplies_id) {
		// TODO Auto-generated method stub
		List list= crmDAO.getOfficeSuppliesid(supplies_id);
		return WebUtil.buildJsonStoreByList(list);
	}

	@Override
	public void dodelOfficeSuppliesById(String supplies_id) {
		// TODO Auto-generated method stub
		crmDAO.dodelOfficeSuppliesById(supplies_id);
	}

	@Override
	public void doStaffRoles(String id, Integer userID, String empID) {
		// TODO Auto-generated method stub
		crmDAO.doStaffRoles(id,userID,empID);
	}

	@Override
	public String getStaffRoles(Integer userID) {
		// TODO Auto-generated method stub
		List list = crmDAO.getStaffRoles(userID);
		return WebUtil.buildJsonStoreByList(list);
	}

	@Override
	public void dodelSataffRoles(Integer userID) {
		// TODO Auto-generated method stub
		
		
		crmDAO.dodelSataffRoles(userID);
	}

	@Override
	public String getDepartment() {
		// TODO Auto-generated method stub
		List list = crmDAO.getDepartment();
		return WebUtil.buildJsonStoreByList(list);
	}

	@Override
	public String getDepartmentID(Integer userID) {
		// TODO Auto-generated method stub
		
		List list = crmDAO.getDepartmentID(userID);
		return WebUtil.buildJsonStoreByList(list);
	}
	
	@Override
	public String getOfficeSuppliesCombot() {
		// TODO Auto-generated method stub
		List lists = crmDAO.getOfficeSuppliesCombot();
		String store = WebUtil.buildJsonStoreByList(lists);
		return store;
	}
 
	@Override
	public void doStaffRolestty(String enjsID, Integer userID,String empID ) {
		// TODO Auto-generated method stub
		crmDAO.doStaffRolestty(enjsID,userID,empID);
	}

	@Override
	public void dodelSataffRolestty(Integer userID) {
		// TODO Auto-generated method stub
		crmDAO.dodelSataffRolestty(userID);
	}

	@Override
	public void doStaffRoles(Integer userID, String roeID) {
		// TODO Auto-generated method stub
		
		crmDAO.doStaffRoles(userID,roeID);
	}

	@Override
	public void dodelSataffRolesteqw(Integer userID) {
		// TODO Auto-generated method stub
		crmDAO.dodelSataffRolesteqw(userID);
	}

	@Override
	public void doStaffRolesttyryyu(Integer userID) {
		// TODO Auto-generated method stub
		crmDAO.doStaffRolesttyryyu(userID);
	}
}
