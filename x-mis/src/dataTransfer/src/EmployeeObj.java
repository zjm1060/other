//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\model\\EmployeeObj.java

package src;

import java.sql.Date;
import java.util.List;
import java.util.Map;

public class EmployeeObj {

	private int userID;

	private String userName;

	private int sex;

	private int companyID;

	private String companyName;

	private int divisionID;

	private String divisionName;

	private int departmentID;

	private String departmentName;

	private String position;

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

	private int disabledFlag;

	private Object contralFile;

	private String remark;

	private String loginName;

	private String password;

	private String firstName;

	private String lastName;

	private int age;

	private List companyList;

	private List departmentList;

	private List userRight;

	private Date joinDate;

	private Date birthday;

	private String zipCode;

	private int issueSex;

	private String IDCard;

	private String officeAddress;

	private String imgName;

	private Map politicalStatusMap;
	private Map lunchAddressMap;

	private String schoolName;

	private Date graduationDate;

	private String major;

	private String educationRemark;
	
	private int lunchAddressID;
	
	private String lunchAddress;
	
	private String lunchAddressData ;

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

	public String getFirstName() {
		return firstName;
	}

	public void setFirstName(String firstName) {
		this.firstName = firstName;
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

	public String getLastName() {
		return lastName;
	}

	public void setLastName(String lastName) {
		this.lastName = lastName;
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

	public int getSex() {
		return sex;
	}

	public void setSex(int sex) {
		this.sex = sex;
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

	public int getUserID() {
		return userID;
	}

	public void setUserID(int userID) {
		this.userID = userID;
	}

	public String getUserName() {
		return userName;
	}

	public void setUserName(String userName) {
		this.userName = userName;
	}

	public List getUserRight() {
		return userRight;
	}

	public void setUserRight(List userRight) {
		this.userRight = userRight;
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

}
