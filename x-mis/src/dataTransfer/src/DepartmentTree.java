package src;

public class DepartmentTree {

	private int departID;
	private String departName;
	private int userID;
	private String userName;
	private String position;
	private int deptDisabledFlag;
	private int empDisabledFlag;
	
	
	public String getPosition() {
		return position;
	}

	public void setPosition(String position) {
		this.position = position;
	}

	public DepartmentTree(){
		
	}
	
	public int getDepartID() {
		return departID;
	}
	public void setDepartID(int departID) {
		this.departID = departID;
	}
	public String getDepartName() {
		return departName;
	}
	public void setDepartName(String departName) {
		this.departName = departName;
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

	public int getDeptDisabledFlag() {
		return deptDisabledFlag;
	}

	public void setDeptDisabledFlag(int deptDisabledFlag) {
		this.deptDisabledFlag = deptDisabledFlag;
	}

	public int getEmpDisabledFlag() {
		return empDisabledFlag;
	}

	public void setEmpDisabledFlag(int empDisabledFlag) {
		this.empDisabledFlag = empDisabledFlag;
	}
}
