package com.bitc.jk.xmis.service;

import java.util.List;
import java.util.Map;

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
import com.bitc.jk.xmis.model.crm.RelativeAddress;
import com.bitc.jk.xmis.util.exception.DuplicatedException;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public interface CrmService {

	public String dataValidate(CompanySimpleObj comp);
	public String doCreateNewCompanySimple(CompanySimpleObj comp,
			EmployeeObj userDNA) throws DuplicatedException, Exception ;
	
	public String getPeasonListByCompany(int compID, EmployeeObj userDNA);

	public String getContactPersonBySearchName(int compID, String searchName,
			EmployeeObj userDNA);
	
//	public String getEmpInfoById(int empID);
	/**
	 * 	根据id查询人员信息
	 * */
	public EmployeeObj getEmpInfoById(int empID);
	public void updateEmpInfo(EmployeeObj target,EmployeeObj userDNA);
	/**
	 * 根据公司ID 的到公司的信息
	 */
	public CompanyObj getCompanyObjById(int companyID);

	/**
	 * 编辑、保存公司信息
	 * @param contactEdit 
	 * @param legalRepresentativeEdit 
	 */
	public void doSaveCompanyInfo(CompanyObj cmpObj, String contactEdit, String legalRepresentativeEdit) ;
//	/**
//	 * 获取公司信息列表
//	 */
//	public String getCompanyListStore(StandardQuery params,PageInfo pageInfo);
	/**
	 * 获取下拉框信息
	 */
	@SuppressWarnings("rawtypes")
	public List getMultiData(String category, int fid);
	/**
	 * 删除一条或者多条公司信息 【 未用】
	 */
	public void dodelCompanyInfoById(String ids);
	/**  
	 * 获取地址信息列表  模糊查询
	 * @param companyID
	 * @param query
	 */
	public String getRelativeAddressListM(StandardQuery query,
			String companyID, PageInfo pageInfo);
	/**  
	 * 获取地址信息列表 包括公司的地址信息
	 * @param companyID
	 * @param isNeedCompID 
	 * @param query
	 */
	public String getAllAddressList(StandardQuery query,
			String companyID , String isNeedCompID);
	/** 
	 * 相关地址信息的编辑、保存
	 */
	public void doSaveRelativeAddress(RelativeAddress relativeAddress);
	/** 
	 * 根据ID删除一条相关地址信息
	 */
	public void dodelRelativeAddressById(String id);
	/**
	 * 根据id得到相关地址
	 */
	public RelativeAddress getRelativeAddress(String id);
	/**
	 * 根据id得到地址
	 */
	public Address getAddress(String id);
	
	/**根据名称模糊查询城市选择框的ID
	 * */
	public String getAddressIDListByName(String name);
	public String getCompanyListStoreByQuerys(String sql_Str, PageInfo pageInfo);
	
	/**公司地址及相关地址的下拉框
	 * @param companyID
	 * @return
	 */
	public String getRelationAddressCombo(String companyID);
	public String getRelationAddress(String companyID);
	public void doSupplierInformation(SupplierInformation si);
	public Map<String, String> getSupplierInformation(int companyID);
	public String getQualificationDictionary();
	public void doQualificationInformation(QualificationObj qfo);
	public String getQualificationList(String companyID);
	public String getQualificationAssignment(String id);
	public String getProjectQualification(String companyID);
	public void doProcurementInformation(ProjectQualification rq);
	public String getUseProjectQualification(String companyID);
	public String getProjectQualificationAssignment(String id);
	public String getDecisionMakersInformation(Integer hrID);
	public void doPatentInformation(PatentInformation pf);
	public String getPatentInformationList(String companyID);
	public String getPatentAssignment(String sjid);
	public String getQualificationListC(PageInfo pageInfo, StandardQuery query);
	public String getPatentInfoListC(PageInfo pageInfo, StandardQuery query);
	public Map<String, String> getQualificationAssignmentr(String id);
	public Map<String, String> getPatentAssignmentr(String sjid);
	public void doMeetingRoomInfo(MeetingRoomInformation mr);
	public String getMeetingRoomInfo(String room_id);
	public String getMeetingRoomInfor();
	public void dodelMeetingRoomInfoById(String room_id);
	public void doVehicleManagement(VehicleManagement vm);
	public String getVehicleManagement();
	public String getVehicleManagemenid(String car_id);
	public void dodelVehicleManagemenById(String car_id);
	public void doOfficeSupplies(OfficeSuppliesDetails osds);
	public String getOfficeSupplies(PageInfo pageInfo, String queryStr);
	public String getOfficeSuppliesid(String supplies_id);
	public void dodelOfficeSuppliesById(String supplies_id);
	public void doStaffRoles(String id, Integer userID, String empID);
	public String getStaffRoles(Integer userID);
	public void dodelSataffRoles(Integer userID);
	public String getDepartment();
	public String getDepartmentID(Integer userID);
	
	String getOfficeSuppliesCombot();
	public void doStaffRolestty(String enjsID, Integer userID,String empID );
	public void dodelSataffRolestty(Integer userID);
	public void doStaffRoles(Integer userID, String roeID);
	public void dodelSataffRolesteqw(Integer userID);
	public void doStaffRolesttyryyu(Integer userID);
	
	
}
