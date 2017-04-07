package com.bitc.jk.xmis.dao;

import java.util.List;

import com.bitc.jk.xmis.model.CompanyObj;
import com.bitc.jk.xmis.model.CompanySimpleObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.MeetingRoomInformation;
import com.bitc.jk.xmis.model.OfficeSuppliesDetails;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.PatentInformation;
import com.bitc.jk.xmis.model.ProjectQualification;
import com.bitc.jk.xmis.model.QualificationObj;
import com.bitc.jk.xmis.model.SupplierInformation;
import com.bitc.jk.xmis.model.VehicleManagement;
import com.bitc.jk.xmis.model.crm.Address;
import com.bitc.jk.xmis.model.crm.Area;
import com.bitc.jk.xmis.model.crm.BusinessRelation;
import com.bitc.jk.xmis.model.crm.City;
import com.bitc.jk.xmis.model.crm.CompanyScale;
import com.bitc.jk.xmis.model.crm.Country;
import com.bitc.jk.xmis.model.crm.CreditLevel;
import com.bitc.jk.xmis.model.crm.CustomerLevel;
import com.bitc.jk.xmis.model.crm.Internation;
import com.bitc.jk.xmis.model.crm.Province;
import com.bitc.jk.xmis.model.crm.RelativeAddress;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public interface CrmDAO {

	public int companyValidate(String compnayName);
	public int departmentValidate(String deptName, int compID);
	public int empValidate(String empName, int compID);
	public int empValidate(String empName, int compID, int deptID);
	
	public int doCreateNewCompanySimple(CompanySimpleObj comp, EmployeeObj userDNA);
	public int doCreateNewDeptSimple(CompanySimpleObj comp, EmployeeObj userDNA);
	public int doCreateNewContactPersonSimple(CompanySimpleObj comp, EmployeeObj userDNA);
	public int doCreateNewContactLinkPersonSimple(CompanySimpleObj comp, EmployeeObj userDNA);
	public void doSetDeptBelongToCompanyBySimple(CompanySimpleObj comp, EmployeeObj userDNA);
	
	public int getDefaultDeptByCompany(int companyID);
	public List getPeasonListBySelfCompany(int companyID);
	public List getPeasonListByCompany(int companyID);

	public List getPeasonListBySelf(int companyID, String searchName);
	public List getPeasonListBySearch(int companyID, String searchName);
	
	//根据id查询人员
	public EmployeeObj getEmpInfoById(int empID);
	public void updateEmpInfo(EmployeeObj target,EmployeeObj userDNA);
	
	//根据公司ID查公司信息
	public CompanyObj getCompanyObjById(int companyID);
	//新建公司信息
	public void doSaveCompanyInfo(CompanyObj cmpObj);
	//获取业务关系的列表
	public List<BusinessRelation> getBusinessRelationCombox();
	//获取公司规模的列表
	public List<CompanyScale> getCmpScaleCombox();
	//获取客户级别的列表
	public List<CustomerLevel> getCustomerCombox();
	//获取信用等级的列表
	public List<CreditLevel> getCreditLevelCombox();
	//获取公司信息list
	public List<CompanyObj> getCompanyList(StandardQuery params,PageInfo pageInfo);
	//获取公司信息的数量
	public int getCompanyListCount(final StandardQuery params, PageInfo pageInfo);
	//省列表
	public List<Province> getProvinceData(int fid);
	//市列表
	public List<City> getCityData(int fid);
	//洲际列表  
	public List<Internation> getInternationData();
	//国家列表
	public List<Country> getCountryData(int fid);
	//地区列表
	public List<Area> getAreaData(int fid);
	//根据ID删除公司   未用
	public void dodelCompanyInfoById(int id);
	public RelativeAddress getRelativeAddress(String id );
	public void doSaveRelativeAddress(RelativeAddress relativeAddress);
	public void dodelRelativeAddressById(String id);
	public List<Address> getAddressListQuery(StandardQuery query);
	public Address getAddress(String id);
	public List<RelativeAddress> getRelativeAddressListM(StandardQuery query,
			PageInfo pageInfo);
	public List getAddressIDListByName(String name);
	public String getOutputContactMoney(int companyID);
	public String getInputContactMoney(int companyID);
	public List getCompanyInfoList(String params, PageInfo pageInfo);
	/**公司地址及相关地址的下拉框
	 * @param companyID
	 * @return
	 */
	public List getRelationAddressCombo(String companyID);
	public List getRelationAddress(String companyID);
	public void doSupplierInformation(SupplierInformation si);
	 List getSupplierInformation(int companyID);
	public List getQualificationDictionary();
	public void doQualificationInformation(QualificationObj qfo);
	public List getQualificationList(String companyID);
	public List getQualificationAssignment(String id);
	public List getProjectQualification(String companyID);
	public void doProcurementInformation(ProjectQualification rq);
	public List getUseProjectQualification(String companyID);
	public List getProjectQualificationAssignment(String id);
	public List getDecisionMakersInformation(Integer hrID);
	public void doPatentInformation(PatentInformation pf);
	public List getPatentInformationList(String companyID);
	public List getPatentAssignment(String sjid);
	public List getQualificationListC(StandardQuery query, PageInfo pageInfo);
	public List getPatentInfoListC(StandardQuery query, PageInfo pageInfo);
	public List getQualificationAssignmentr(String id);
	public List getPatentAssignmentr(String sjid);
	public void doMeetingRoomInfo(MeetingRoomInformation mr);
	public List getMeetingRoomInfo(String room_id);
	public List getMeetingRoomInfor();
	public void dodelMeetingRoomInfoById(String room_id);
	public void doVehicleManagement(VehicleManagement vm);
	public List getVehicleManagement();
	public List getVehicleManagemenid(String car_id);
	public void dodelVehicleManagemenById(String car_id);
	public void doOfficeSupplies(OfficeSuppliesDetails osds);
	public List getOfficeSupplies(String queryStr, String ordersql);
	public List getOfficeSuppliesid(String supplies_id);
	public void dodelOfficeSuppliesById(String supplies_id);
	public void doStaffRoles(String id, Integer userID, String empID);
	public List getStaffRoles(Integer userID);
	public void dodelSataffRoles(Integer userID);
	public List getDepartment();
	public List getDepartmentID(Integer userID);
	List getOfficeSuppliesCombot();
	public void doStaffRolestty(String enjsID, Integer userID,String empID);
	public void dodelSataffRolestty(Integer userID);
	public void doStaffRoles(Integer userID, String roeID);
	public void dodelSataffRolesteqw(Integer userID);
	public void doStaffRolesttyryyu(Integer userID);

}