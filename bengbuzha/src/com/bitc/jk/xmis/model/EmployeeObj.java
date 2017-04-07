//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\model\\EmployeeObj.java

package com.bitc.jk.xmis.model;

import java.sql.Date;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import net.sf.json.JSONArray;
import net.sf.json.JSONObject;

import org.apache.log4j.Logger;

import com.bitc.jk.xmis.model.crm.Address;
import com.bitc.jk.xmis.model.security.BizFunctionObj;
import com.bitc.jk.xmis.model.security.BizModuleObj;
import com.bitc.jk.xmis.model.security.BizSubModuleObj;
import com.bitc.jk.xmis.model.sys.Degree;
import com.bitc.jk.xmis.model.sys.DutyLevel;
import com.bitc.jk.xmis.model.sys.Echnicalposition;
import com.bitc.jk.xmis.model.sys.Education;
import com.bitc.jk.xmis.model.sys.IdentityObj;
import com.bitc.jk.xmis.model.sys.InServiceType;
import com.bitc.jk.xmis.model.sys.MaritalStatus;
import com.bitc.jk.xmis.model.sys.PoliticalStatus;
import com.bitc.jk.xmis.model.sys.ProfessionalLevel;
import com.bitc.jk.xmis.model.sys.Sex;
import com.bitc.jk.xmis.model.sys.WorkType;
import com.bitc.jk.xmis.util.Type;

public class EmployeeObj extends SimpleEmployee {

	private static final long serialVersionUID = 6702683876500471139L;

	private static Logger logger = Logger.getLogger(EmployeeObj.class);

	private String firstName;

	private String lastName;

	private int sexID;

	private int companyID;

	private String companyName;

	private int divisionID;

	private String divisionName;

	private int departmentID;

	private String departmentName;

	private String position;
	private String positionEdit;//职务 2013-03-28

	public String getPositionEdit() {
		return positionEdit;
	}

	public void setPositionEdit(String positionEdit) {
		this.positionEdit = positionEdit;
	}

	private int educationHighestLevel;

	private String educationLevelName;

	private String mobileNo;

	private String switchboardNo;

	private String extNo;

	private String emailAddress;

	private String webPage;

	private String faxNo;

	private String homeAddress;

	private String homeTelNo;

	private int maritalStatus;

	private String spouseName;

	private String issueName;

	private String nativePlace;

	private int politicalStatus;

	private String hobby;

	private String specialty;
	private String urgentinfo;// 紧急联系方式

	private int disabledFlag;
	private String disabledReason;
	private String disabledReasonID;//选择禁用的ID
	private Date disabledDate;
	
	private Sex sex;
	private MaritalStatus marStat;
	private Sex issSex;           //子女性别
	private Education edu;
	private PoliticalStatus polStat;
	private List<Integer> roles;
	private Address address;
	
	private String pathId;
	private String pathName;

	public String getUrgentinfo() {
		return urgentinfo;
	}

	public void setUrgentinfo(String urgentinfo) {
		this.urgentinfo = urgentinfo;
	}

	public String getPathId() {
		return pathId;
	}
	
	public String getPathName() {
		return pathName;
	}

	public String getDisabledReasonID() {
		return disabledReasonID;
	}

	public void setDisabledReasonID(String disabledReasonID) {
		this.disabledReasonID = disabledReasonID;
	}

	public String getDisabledReason() {
		return disabledReason;
	}

	public void setDisabledReason(String disabledReason) {
		this.disabledReason = disabledReason;
	}

	public java.util.Date getDisabledDate() {
		return disabledDate;
	}

	public void setDisabledDate(Date disabledDate) {
		this.disabledDate = disabledDate;
	}

	public void setPathName(String pathName) {
		this.pathName = pathName;
	}

	public void setPathId(String pathId) {
		this.pathId = pathId;
	}

	public Address getAddress() {
		return address;
	}

	public void setAddress(Address address) {
		this.address = address;
	}

	public List<Integer> getRoles() {
		return roles;
	}

	public void setRoles(List<Integer> roles) {
		this.roles = roles;
	}

	public Sex getSex() {
		return sex;
	}

	public void setSex(Sex sex) {
		this.sex = sex;
	}

	public MaritalStatus getMarStat() {
		return marStat;
	}

	public void setMarStat(MaritalStatus marStat) {
		this.marStat = marStat;
	}

	public Sex getIssSex() {
		return issSex;
	}

	public void setIssSex(Sex issSex) {
		this.issSex = issSex;
	}

	public Education getEdu() {
		return edu;
	}

	public void setEdu(Education edu) {
		this.edu = edu;
	}

	public PoliticalStatus getPolStat() {
		return polStat;
	}

	public void setPolStat(PoliticalStatus polStat) {
		this.polStat = polStat;
	}

	private Object contralFile;

	private String remark;

	private String loginName;

	private String password;

	private int age;

	private List<EmployeePost> postList;

	private transient List companyList;

	private transient List departmentList;

	private List userRightsListByDefault;

	private Map userRightsMap = new HashMap();

	private Date joinDate;

	private Date birthday;

	private String zipCode;

	private int issueSex;

	private String IDCard;

	private String officeAddress;

	private String imgName;
	private String postCardName;//正面
	private String postCardBackName;//反面

	public String getPostCardName() {
		return postCardName;
	}

	public void setPostCardName(String postCardName) {
		this.postCardName = postCardName;
	}

	public String getPostCardBackName() {
		return postCardBackName;
	}

	public void setPostCardBackName(String postCardBackName) {
		this.postCardBackName = postCardBackName;
	}

	private transient Map politicalStatusMap;

	private transient Map lunchAddressMap;

	private String schoolName;

	private Date graduationDate;

	private String major;

	private String educationRemark;

	private int lunchAddressID;

	private String lunchAddress;

	private String lunchAddressData;

	private boolean locked;

	public Date getGraduationDate() {
		return graduationDate;
	}

	public void setGraduationDate(Date graduationDate) {
		this.graduationDate = graduationDate;
	}

	public String getMajor() {
		return major;
	}

	public void setMajor(String major) {
		this.major = major;
	}

	public String getEducationRemark() {
		return educationRemark;
	}

	public void setEducationRemark(String remark) {
		this.educationRemark = remark;
	}

	public String getSchoolName() {
		return schoolName;
	}

	public void setSchoolName(String schoolName) {
		this.schoolName = schoolName;
	}

	/**
	 * @roseuid 4C1F2C0E002E
	 */
	public EmployeeObj() {

	}

	public int getAge() {
		return age;
	}

	public void setAge(int age) {
		this.age = age;
	}

	public Date getBirthday() {
		return birthday;
	}

	public void setBirthday(Date birthday) {
		this.birthday = birthday;
	}

	public int getCompanyID() {
		return companyID;
	}

	public void setCompanyID(int companyID) {
		this.companyID = companyID;
	}

	public List getCompanyList() {
		return companyList;
	}

	public void setCompanyList(List companyList) {
		this.companyList = companyList;
	}

	public String getCompanyName() {
		return companyName;
	}

	public void setCompanyName(String companyName) {
		this.companyName = companyName;
	}

	public Object getContralFile() {
		return contralFile;
	}

	public void setContralFile(Object contralFile) {
		this.contralFile = contralFile;
	}

	public int getDepartmentID() {
		return departmentID;
	}

	public void setDepartmentID(int departmentID) {
		this.departmentID = departmentID;
	}

	public List getDepartmentList() {
		return departmentList;
	}

	public void setDepartmentList(List departmentList) {
		this.departmentList = departmentList;
	}

	public String getDepartmentName() {
		return departmentName;
	}

	public void setDepartmentName(String departmentName) {
		this.departmentName = departmentName;
	}

	public int getDisabledFlag() {
		return disabledFlag;
	}

	public void setDisabledFlag(int disabledFlag) {
		this.disabledFlag = disabledFlag;
	}

	public int getDivisionID() {
		return divisionID;
	}

	public void setDivisionID(int divisionID) {
		this.divisionID = divisionID;
	}

	public String getDivisionName() {
		return divisionName;
	}

	public void setDivisionName(String divisionName) {
		this.divisionName = divisionName;
	}

	public int getEducationHighestLevel() {
		return educationHighestLevel;
	}

	public void setEducationHighestLevel(int educationHighestLevel) {
		this.educationHighestLevel = educationHighestLevel;
	}

	public String getEmailAddress() {
		return emailAddress;
	}

	public void setEmailAddress(String emailAddress) {
		this.emailAddress = emailAddress;
	}

	public String getExtNo() {
		return extNo;
	}

	public void setExtNo(String extNo) {
		this.extNo = extNo;
	}

	public String getFaxNo() {
		return faxNo;
	}

	public void setFaxNo(String faxNo) {
		this.faxNo = faxNo;
	}

	public String getHobby() {
		return hobby;
	}

	public void setHobby(String hobby) {
		this.hobby = hobby;
	}

	public String getHomeAddress() {
		return homeAddress;
	}

	public void setHomeAddress(String homeAddress) {
		this.homeAddress = homeAddress;
	}

	public String getHomeTelNo() {
		return homeTelNo;
	}

	public void setHomeTelNo(String homeTelNo) {
		this.homeTelNo = homeTelNo;
	}

	public String getIDCard() {
		return IDCard;
	}

	public void setIDCard(String card) {
		IDCard = card;
	}

	public String getIssueName() {
		return issueName;
	}

	public void setIssueName(String issueName) {
		this.issueName = issueName;
	}

	public int getIssueSex() {
		return issueSex;
	}

	public void setIssueSex(int issueSex) {
		this.issueSex = issueSex;
	}

	public Date getJoinDate() {
		return joinDate;
	}

	public void setJoinDate(Date joinDate) {
		this.joinDate = joinDate;
	}

	public String getLoginName() {
		return loginName;
	}

	public void setLoginName(String loginName) {
		this.loginName = loginName;
	}

	public int getMaritalStatus() {
		return maritalStatus;
	}

	public void setMaritalStatus(int maritalStatus) {
		this.maritalStatus = maritalStatus;
	}

	public String getMobileNo() {
		return mobileNo;
	}

	public void setMobileNo(String mobileNo) {
		this.mobileNo = mobileNo;
	}

	public String getNativePlace() {
		return nativePlace;
	}

	public void setNativePlace(String nativePlace) {
		this.nativePlace = nativePlace;
	}

	public String getPassword() {
		return password;
	}

	public void setPassword(String password) {
		this.password = password;
	}

	public int getPoliticalStatus() {
		return politicalStatus;
	}

	public void setPoliticalStatus(int politicalStatus) {
		this.politicalStatus = politicalStatus;
	}

	public String getPosition() {
		return position;
	}

	public void setPosition(String position) {
		this.position = position;
	}

	public String getRemark() {
		return remark;
	}

	public void setRemark(String remark) {
		this.remark = remark;
	}

	public int getSexID() {
		return sexID;
	}

	public void setSexID(int sexID) {
		this.sexID = sexID;
	}

	public String getSpecialty() {
		return specialty;
	}

	public void setSpecialty(String specialty) {
		this.specialty = specialty;
	}

	public String getSpouseName() {
		return spouseName;
	}

	public void setSpouseName(String spouseName) {
		this.spouseName = spouseName;
	}

	public String getSwitchboardNo() {
		return switchboardNo;
	}

	public void setSwitchboardNo(String switchboardNo) {
		this.switchboardNo = switchboardNo;
	}

	public List getUserRightsListByDefault() {
		return userRightsListByDefault;
	}

	public void setUserRightsListByDefault(List userRight) {
		this.userRightsListByDefault = userRight;
	}

	public String getWebPage() {
		return webPage;
	}

	public void setWebPage(String webPage) {
		this.webPage = webPage;
	}

	public String getZipCode() {
		return zipCode;
	}

	public void setZipCode(String zipCode) {
		this.zipCode = zipCode;
	}

	public String getOfficeAddress() {
		return officeAddress;
	}

	public void setOfficeAddress(String officeAddress) {
		this.officeAddress = officeAddress;
	}

	public String getEducationLevelName() {
		return educationLevelName;
	}

	public void setEducationLevelName(String educationLevelName) {
		this.educationLevelName = educationLevelName;
	}

	public String getImgName() {
		return imgName;
	}

	public void setImgName(String imgName) {
		this.imgName = imgName;
	}

	public Map getPoliticalStatusMap() {
		return politicalStatusMap;
	}

	public void setPoliticalStatusMap(Map politicalStatusMap) {
		this.politicalStatusMap = politicalStatusMap;
	}

	public String getLunchAddress() {
		return lunchAddress;
	}

	public void setLunchAddress(String lunchAddress) {
		this.lunchAddress = lunchAddress;
	}

	public int getLunchAddressID() {
		return lunchAddressID;
	}

	public void setLunchAddressID(int lunchAddressID) {
		this.lunchAddressID = lunchAddressID;
	}

	public Map getLunchAddressMap() {
		return lunchAddressMap;
	}

	public void setLunchAddressMap(Map lunchAddressMap) {
		this.lunchAddressMap = lunchAddressMap;
	}

	public String getLunchAddressData() {
		return lunchAddressData;
	}

	public void setLunchAddressData(String lunchAddressData) {
		this.lunchAddressData = lunchAddressData;
	}

	public Map getUserRightsMap() {
		return userRightsMap;
	}

	public void setUserRightsMap(Map userRightsMap) {
		this.userRightsMap = userRightsMap;
	}

	public String buildUserMenuJsonStr() {

		JSONArray menuJson = new JSONArray();

		String keyNameId = "id";
		String keyNameText = "text";
		String keyNameUrl = "url";
		String keyNameIconCls = "cls";
		String keyNameLeaf = "leaf";
		String keyNameSequenceNo = "sequenceNo";
		String keyNameIdentifies = "identifies";

		String keyNameChildren = "children";

		Iterator it = this.userRightsMap.entrySet().iterator();
		for (int j = 0; j < this.userRightsMap.size(); j++) {
			Map.Entry entry = (Map.Entry) it.next();

			JSONObject moduleJson = new JSONObject();
			BizModuleObj module = (BizModuleObj) entry.getValue();

			moduleJson.element(keyNameId, module.getId());
			moduleJson.element(keyNameText, module.getName());
			moduleJson.element(keyNameUrl, module.getUrl());
			moduleJson.element(keyNameIconCls, module.getIconCls());
			moduleJson.element(keyNameSequenceNo, module.getSequenceNo());
			moduleJson.element(keyNameIdentifies, module.getIdentifies());

			JSONArray childrenJson = new JSONArray();

			Iterator subIt = module.getBizObjMap().entrySet().iterator();
			for (int si = 0; si < module.getBizObjMap().size(); si++) {
				Map.Entry subEntry = (Map.Entry) subIt.next();
				BizSubModuleObj subModule = (BizSubModuleObj) subEntry
						.getValue();
				JSONObject subModuleJson = new JSONObject();
				subModuleJson.element(keyNameId, subModule.getId());
				subModuleJson.element(keyNameText, subModule.getName());
				subModuleJson.element(keyNameUrl, subModule.getUrl());
				subModuleJson.element(keyNameIconCls, subModule.getIconCls());
				subModuleJson.element(keyNameSequenceNo,
						subModule.getSequenceNo());
				subModuleJson.element(keyNameIdentifies,
						subModule.getIdentifies());

				childrenJson.add(subModuleJson);
			}
			moduleJson.element(keyNameChildren, childrenJson);

			menuJson.add(moduleJson);
		}

		return menuJson.toString();
	}

	public String buildMenuJsonStr(int moduleId) {

		BizModuleObj module = (BizModuleObj) this.userRightsMap.get("mod_"
				+ moduleId);
		if (module == null) {
			return "[]";
		}
		String keyNameId = "id";
		String keyNameText = "text";
		String keyNameUrl = "url";
		String keyNameIconCls = "iconCls";
		String keyNameLeaf = "leaf";
		String keyNameIdentifies = "identifies";
		JSONArray menuJson = new JSONArray();
		Map map = module.getBizObjMap();
		List<BizSubModuleObj> list = new ArrayList<BizSubModuleObj>();

		for (Iterator it = map.keySet().iterator(); it.hasNext();) {
			list.add((BizSubModuleObj) map.get(it.next()));
		}
		Collections.sort(list, new Comparator<BizSubModuleObj>() {
			public int compare(BizSubModuleObj obj, BizSubModuleObj otherObj) {
				return obj.getSequenceNo() - otherObj.getSequenceNo();
			}
		});
		for (Iterator<BizSubModuleObj> it = list.iterator(); it.hasNext();) {
			BizSubModuleObj subModule = it.next();
			JSONObject subModuleJson = new JSONObject();
			subModuleJson.element(keyNameId, subModule.getId());
			subModuleJson.element(keyNameIdentifies, subModule.getIdentifies());
			subModuleJson.element(keyNameText, subModule.getName());
			subModuleJson.element(keyNameUrl, subModule.getUrl());
			subModuleJson.element(keyNameIconCls, subModule.getIconCls());
			subModuleJson.element(keyNameLeaf, true);
			menuJson.add(subModuleJson);
		}
		return menuJson.toString();
	}

	public int getResourceID(String requestURL) {

		int resourceID = 0;
		String resourceURL = "";

		if (this.userRightsListByDefault != null
				&& this.userRightsListByDefault.size() > 0) {

			for (Iterator i = this.userRightsListByDefault.listIterator(); i
					.hasNext();) {

				Map map = (Map) i.next();

				Iterator it = map.entrySet().iterator();
				for (int j = 0; j < map.size(); j++) {
					Map.Entry entry = (Map.Entry) it.next();

					if (entry.getKey().equals("id"))
						resourceID = Type.GetInt(entry.getValue());
					else if (entry.getKey().equals("url"))
						resourceURL = Type.getString(entry.getValue());

				}

				if (requestURL.equals("/" + resourceURL))
					break;

			}
		}

		return resourceID;

	}

	public String getRescourceName(String requestURL) {

		String resourceName = "", resourceURL = "";

		// if (requestURL == null || requestURL.toUpperCase().equals("NULL")
		// || requestURL.equals(""))
		// return resourceName;

		if (this.userRightsListByDefault != null
				&& this.userRightsListByDefault.size() > 0) {

			for (Iterator i = this.userRightsListByDefault.listIterator(); i
					.hasNext();) {

				Map map = (Map) i.next();

				Iterator it = map.entrySet().iterator();
				for (int j = 0; j < map.size(); j++) {
					Map.Entry entry = (Map.Entry) it.next();

					if (entry.getKey().equals("name"))
						resourceName = Type.getString(entry.getValue());
					else if (entry.getKey().equals("url"))
						resourceURL = Type.getString(entry.getValue());

				}

				if (requestURL.equals("/" + resourceURL))
					break;

			}
		}

		return resourceName;

	}

	/*
	 * return boolean[0] 是否允许该请求 
	 * return boolean[1] 是否对该请求记录至系统日志 
	 * return boolean[2] 是否对该请求只读属性
	 * return boolean[3] 是否需要显示提示信息
	 */
	public boolean[] userAccessRightValidate(String requestURL) {

		boolean isShowMessage = true;

		boolean existFlag = true;

		boolean userAccessRightValidate = true;

		boolean readonlyLimit = false;
		
//		UserSecrityInfo userSecInfo = new UserSecrityInfo();
//		userSecInfo.setAllowAccessRequest(false);
//		userSecInfo.setAddRequestIntoLog(false);
//		userSecInfo.setReadOnlyForRequest(true);
//		userSecInfo.setAllowShowMessage(false);
		

		if (requestURL == null || requestURL.toUpperCase().equals("NULL")
				|| requestURL.equals("")
				|| this.userRightsListByDefault == null
				|| this.userRightsListByDefault.size() == 0) {
			
			
			return new boolean[] { false, false, readonlyLimit,isShowMessage };
//			return new boolean[] { false, false, false, true };
			
//			return userSecInfo;
		}

		for (Iterator i = this.userRightsListByDefault.listIterator(); i
				.hasNext();) {

			String resourceName = "", resourceURL = "";

			Map map = (Map) i.next();

			Iterator it = map.entrySet().iterator();
			for (int j = 0; j < map.size(); j++) {
				
				Map.Entry entry = (Map.Entry) it.next();
				
				if (entry.getKey().equals("id"))
					isShowMessage = Type.GetInt(entry.getValue())<=0?false:true;				

				if (entry.getKey().equals("name"))
					resourceName = Type.getString(entry.getValue());
				else if (entry.getKey().equals("url"))
					resourceURL = Type.getString(entry.getValue());

			}

			existFlag = (requestURL.equals("/" + resourceURL)) ? true : false;

			if (existFlag) {
				userAccessRightValidate = false;
				break;

			} else {
				// existFlag = (requestURL.indexOf(resourceURL)>0) ? true :
				// false;
				//
				//
				// if(existFlag){
				// userAccessRightValidate = false;
				// break;
				// }else{
				//
				// userAccessRightValidate = true;
				// }
			}

		}

		if (userAccessRightValidate) {
			logger.info("This request("
					+ requestURL
					+ ") dose not in security scope, so it's will be allowed to access, return true!");
			
			return new boolean[] { true, false, readonlyLimit,isShowMessage };
			
			

		}

		Iterator it = this.userRightsMap.entrySet().iterator();
		for (int j = 0; j < this.userRightsMap.size(); j++) {

			Map.Entry entry = (Map.Entry) it.next();

			BizModuleObj module = (BizModuleObj) entry.getValue();

			if (module.getId() <= 0)
				isShowMessage = false;

			// userAccessRightValidate = url.indexOf(module.getUrl()) > 0 ? true
			// : false;
			userAccessRightValidate = (requestURL.equals("/" + module.getUrl())
			// || requestURL.indexOf(module.getUrl()) > 0
			) ? true : false;

			if (userAccessRightValidate) {

				logger.info("This request(" + requestURL
						+ ") is in security scope on module_level ["
						+ module.getUrl() + "] & matched OK with "
						+ this.userName
						+ " , so it's accessed safed, return true!");

				return new boolean[] { true, true, readonlyLimit,isShowMessage };
			}

			Iterator subIt = module.getBizObjMap().entrySet().iterator();
			for (int si = 0; si < module.getBizObjMap().size(); si++) {
				Map.Entry subEntry = (Map.Entry) subIt.next();

				BizSubModuleObj subModule = (BizSubModuleObj) subEntry
						.getValue();

				if (subModule.getId() <= 0)
					isShowMessage = false;
				// userAccessRightValidate = url.indexOf(subModule.getUrl()) > 0
				// ? true
				// : false;

				userAccessRightValidate = (requestURL.equals("/"
						+ subModule.getUrl())
				// || requestURL.indexOf(subModule.getUrl())>0
				) ? true : false;

				if (userAccessRightValidate) {

					logger.info("This request(" + requestURL
							+ ") is in security scope on subModule_level ["
							+ subModule.getUrl() + "] & matched OK with "
							+ this.userName
							+ ", so it's accessed safed, return true!");
					return new boolean[] { true, true, readonlyLimit,isShowMessage };
				}

				Iterator funIt = subModule.getBizObjMap().entrySet().iterator();
				for (int fi = 0; fi < subModule.getBizObjMap().size(); fi++) {
					Map.Entry funEntry = (Map.Entry) funIt.next();

					BizFunctionObj function = (BizFunctionObj) funEntry
							.getValue();

					if (function.getId() <= 0)
						isShowMessage = false;

					userAccessRightValidate = (requestURL.equals("/"
							+ function.getUrl())
					// || requestURL.indexOf(function.getUrl())>0
					) ? true : false;

					if (function.getBizOperation().getOperationID() == 1)
						readonlyLimit = true;

					if (userAccessRightValidate) {

						logger.info("This request(" + requestURL
								+ ") is in security scope on function_level ["
								+ function.getUrl() + "] & matched OK with "
								+ this.userName
								+ ", so it's accessed safed, return true!");
						return new boolean[] { true, true, readonlyLimit,isShowMessage };
					}

				}

			}
		}

		logger.info("This request("
				+ requestURL
				+ ") is a controlled security url, but it dose not be allow to access by "
				+ this.userName + ", return false! And isShowMessage is "
				+ isShowMessage);

		 return new boolean[] { false, true, readonlyLimit,isShowMessage };
//		return new boolean[] { isNonAccessControl, true, readonlyLimit };

	}

	public List<EmployeePost> getPostList() {
		return postList;
	}

	public void setPostList(List<EmployeePost> postList) {
		this.postList = postList;
	}

	public String getFirstName() {
		return firstName;
	}

	public void setFirstName(String firstName) {
		this.firstName = firstName;
	}

	public String getLastName() {
		return lastName;
	}

	public void setLastName(String lastName) {
		this.lastName = lastName;
	}

	public boolean isLocked() {
		return locked;
	}

	public void setLocked(boolean locked) {
		this.locked = locked;
	}

	public boolean isReadOnly() {

		List<Integer> roles = this.roles;

		boolean readOnly = true;

		if (roles != null && roles.size() > 0) {
			for (Integer num : roles) {
				switch (num.intValue()) {
				case 8:
				case 6:
				case 3:
				case 4:
					readOnly = false;
					break;
				}
			}
		}

		return readOnly;
	}

	/*
	 * 系统管理员缺省拥有该权限
	 * 
	 * @roleID = 2
	 */
	private boolean findRole(int roleID) {
		boolean hasFlag = false;

		List<Integer> roles = this.roles;

		if (roles != null && roles.size() > 0) {
			for (Integer num : roles) {

				if (num == 2) {
					hasFlag = true;
					break;
				}

				if (roleID == num) {
					hasFlag = true;
					break;
				}
			}
		}

		return hasFlag;
	}

	/*
	 * 是否拥有合同价格决策权
	 * 
	 * @roleID = 11
	 */
	public boolean hasDecisionRole() {

		// List<Integer> roles = this.roles;
		// boolean decisionFlag = false;
		//
		// if (roles != null && roles.size() > 0) {
		// for (Integer num : roles) {
		// if (11 == num) {
		// decisionFlag = true;
		// break;
		// }
		// }
		// }
		// return decisionFlag;

		return this.findRole(11);

	}

	/*
	 * 是否拥有公司级合同管理权限
	 * 
	 * @roleID is 14
	 */
	public boolean hasContractControlOnCompanyLevel() {

		return this.findRole(14);
	}

	private Date createDate; // "创建日期";
	private SimpleEmployee createPerson; // "创建人ID";
	private Date updateDate; // "最后修改时间";
	private SimpleEmployee updatePerson; // "最后修改人ID";

	public Date getCreateDate() {
		return createDate;
	}

	public void setCreateDate(Date createDate) {
		this.createDate = createDate;
	}

	public SimpleEmployee getCreatePerson() {
		return createPerson;
	}

	public void setCreatePerson(SimpleEmployee createPerson) {
		this.createPerson = createPerson;
	}

	public SimpleEmployee getUpdatePerson() {
		return updatePerson;
	}

	public void setUpdatePerson(SimpleEmployee updatePerson) {
		this.updatePerson = updatePerson;
	}

	public Date getUpdateDate() {
		return updateDate;
	}

	public void setUpdateDate(Date updateDate) {
		this.updateDate = updateDate;
	}
	
	
	
	 
	private String nation;//
	private Date getqualificationtime;//资格取得时间
	private Date text1;//字段1
	private Integer wagelevel;//工资级别
	private Date joinworkdate;//参加工作年月
	private Date joinpartydate;//入党年月
	private Date joingroupdate;//入团年月
	private Date retirementdate;//退休时间
	private String thetimeandschoolandprofessional;//何时何校何专业毕业
	private String remark1;//备注1
	private String remark2;//备注2
	private String remark3;//备注3

	private DutyLevel dutylevel;//职务级别
	private Echnicalposition echnicalposition;//技术职务
	private ProfessionalLevel professionallevel;//专业技术级别
	private Degree degree;//现有文化程度
	private WorkType worktype;//工种
	private InServiceType type;//在职类型
	private IdentityObj identity;//身份
	
	public InServiceType getType() {
		return type;
	}

	public String getNation() {
		return nation;
	}

	public Date getRetirementdate() {
		return retirementdate;
	}

	public void setRetirementdate(Date retirementdate) {
		this.retirementdate = retirementdate;
	}

	public String getRemark3() {
		return remark3;
	}

	public void setRemark3(String remark3) {
		this.remark3 = remark3;
	}

	public IdentityObj getIdentity() {
		return identity;
	}

	public void setIdentity(IdentityObj identity) {
		this.identity = identity;
	}

	public void setNation(String nation) {
		this.nation = nation;
	}

	public DutyLevel getDutylevel() {
		return dutylevel;
	}

	public void setDutylevel(DutyLevel dutylevel) {
		this.dutylevel = dutylevel;
	}

	public void setEchnicalposition(Echnicalposition echnicalposition) {
		this.echnicalposition = echnicalposition;
	}

	public void setProfessionallevel(ProfessionalLevel professionallevel) {
		this.professionallevel = professionallevel;
	}

	public void setDegree(Degree degree) {
		this.degree = degree;
	}

	public void setWorktype(WorkType worktype) {
		this.worktype = worktype;
	}

	public void setType(InServiceType type) {
		this.type = type;
	}

	public Date getGetqualificationtime() {
		return getqualificationtime;
	}

	public void setGetqualificationtime(Date getqualificationtime) {
		this.getqualificationtime = getqualificationtime;
	}

	public Date getText1() {
		return text1;
	}

	public void setText1(Date text1) {
		this.text1 = text1;
	}

	public Integer getWagelevel() {
		return wagelevel;
	}

	public void setWagelevel(Integer wagelevel) {
		this.wagelevel = wagelevel;
	}

	public Date getJoinworkdate() {
		return joinworkdate;
	}

	public void setJoinworkdate(Date joinworkdate) {
		this.joinworkdate = joinworkdate;
	}

	public Date getJoinpartydate() {
		return joinpartydate;
	}

	public void setJoinpartydate(Date joinpartydate) {
		this.joinpartydate = joinpartydate;
	}

	public Date getJoingroupdate() {
		return joingroupdate;
	}

	public void setJoingroupdate(Date joingroupdate) {
		this.joingroupdate = joingroupdate;
	}


	public Echnicalposition getEchnicalposition() {
		return echnicalposition;
	}

	public ProfessionalLevel getProfessionallevel() {
		return professionallevel;
	}

	public Degree getDegree() {
		return degree;
	}

	public WorkType getWorktype() {
		return worktype;
	}

	public String getThetimeandschoolandprofessional() {
		return thetimeandschoolandprofessional;
	}

	public void setThetimeandschoolandprofessional(
			String thetimeandschoolandprofessional) {
		this.thetimeandschoolandprofessional = thetimeandschoolandprofessional;
	}

	public String getRemark1() {
		return remark1;
	}

	public void setRemark1(String remark1) {
		this.remark1 = remark1;
	}

	public String getRemark2() {
		return remark2;
	}

	public void setRemark2(String remark2) {
		this.remark2 = remark2;
	}

	
}
