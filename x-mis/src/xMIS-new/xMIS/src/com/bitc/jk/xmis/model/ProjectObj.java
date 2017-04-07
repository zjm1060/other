//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\model\\ProjectObj.java

package com.bitc.jk.xmis.model;

import java.sql.Date;
import java.util.List;

import com.bitc.jk.xmis.util.JSONUtils;

@SuppressWarnings("rawtypes")
public class ProjectObj extends TableBean {
	private static final long serialVersionUID = -6642980343057356298L;
	private String projectID;
	private String projectCode;
	private String projectName;
	private int contractID;
	private Date startDate;
	private ProjectType type;
	private ProjectStatus status;
	private  ContractObj mainContract;//transient 删除 项目信息页面 需要合同的一个属性
	private SimpleEmployee manager;
	private SimpleDeptObj executiveDept;
	private SimpleEmployee creator;
	private Date createTime;

	private transient List<ContractObj> contractList;
	private transient List departmentList;
	private transient List employeeList;
	private transient List workProductList;
	private transient List functionList;
	private transient List deviceList;
	
	private String executiveDeptChangeReason;//变更执行部门原因

	/**
	 * 来款合计
	 */
	private double incomAmtGL;
	/**
	 * 采购支出合计
	 */
	private double purchaseAmtToGl;

	/**
	 * @roseuid 4C1F042001C5
	 */
	public ProjectObj() {

	}

	public ProjectObj(String projectID) {
		this.projectID = projectID;
	}

	/**
	 * Access method for the projectID property.
	 * 
	 * @return the current value of the projectID property
	 */
	public String getProjectID() {
		return projectID;
	}

	/**
	 * Sets the value of the projectID property.
	 * 
	 * @param aProjectID
	 *            the new value of the projectID property
	 */
	public void setProjectID(String aProjectID) {
		projectID = aProjectID;
	}

	/**
	 * Access method for the projectCode property.
	 * 
	 * @return the current value of the projectCode property
	 */
	public String getProjectCode() {
		return projectCode;
	}

	/**
	 * Sets the value of the projectCode property.
	 * 
	 * @param aProjectCode
	 *            the new value of the projectCode property
	 */
	public void setProjectCode(String aProjectCode) {
		projectCode = aProjectCode;
	}

	/**
	 * Access method for the projectName property.
	 * 
	 * @return the current value of the projectName property
	 */
	public String getProjectName() {
		return projectName;
	}

	/**
	 * Sets the value of the projectName property.
	 * 
	 * @param aProjectName
	 *            the new value of the projectName property
	 */
	public void setProjectName(String aProjectName) {
		projectName = aProjectName;
	}

	/**
	 * Access method for the contractID property.
	 * 
	 * @return the current value of the contractID property
	 */
	public int getContractID() {
		return contractID;
	}

	/**
	 * Sets the value of the contractID property.
	 * 
	 * @param aContractID
	 *            the new value of the contractID property
	 */
	public void setContractID(int aContractID) {
		contractID = aContractID;
	}

	/**
	 * Access method for the startDate property.
	 * 
	 * @return the current value of the startDate property
	 */
	public Date getStartDate() {
		return startDate;
	}

	/**
	 * Sets the value of the startDate property.
	 * 
	 * @param aStartDate
	 *            the new value of the startDate property
	 */
	public void setStartDate(Date aStartDate) {
		startDate = aStartDate;
	}

	/**
	 * Access method for the contractList property.
	 * 
	 * @return the current value of the contractList property
	 */
	public List<ContractObj> getContractList() {
		return contractList;
	}

	/**
	 * Sets the value of the contractList property.
	 * 
	 * @param aContractList
	 *            the new value of the contractList property
	 */
	public void setContractList(List<ContractObj> aContractList) {
		contractList = aContractList;
	}

	/**
	 * Access method for the departmentList property.
	 * 
	 * @return the current value of the departmentList property
	 */
	public List getDepartmentList() {
		return departmentList;
	}

	/**
	 * Sets the value of the departmentList property.
	 * 
	 * @param aDepartmentList
	 *            the new value of the departmentList property
	 */
	public void setDepartmentList(List aDepartmentList) {
		departmentList = aDepartmentList;
	}

	/**
	 * Access method for the employeeList property.
	 * 
	 * @return the current value of the employeeList property
	 */
	public List getEmployeeList() {
		return employeeList;
	}

	/**
	 * Sets the value of the employeeList property.
	 * 
	 * @param aEmployeeList
	 *            the new value of the employeeList property
	 */
	public void setEmployeeList(List aEmployeeList) {
		employeeList = aEmployeeList;
	}

	/**
	 * Access method for the workProductList property.
	 * 
	 * @return the current value of the workProductList property
	 */
	public List getWorkProductList() {
		return workProductList;
	}

	/**
	 * Sets the value of the workProductList property.
	 * 
	 * @param aWorkProductList
	 *            the new value of the workProductList property
	 */
	public void setWorkProductList(List aWorkProductList) {
		workProductList = aWorkProductList;
	}

	/**
	 * Access method for the functionList property.
	 * 
	 * @return the current value of the functionList property
	 */
	public List getFunctionList() {
		return functionList;
	}

	/**
	 * Sets the value of the functionList property.
	 * 
	 * @param aFunctionList
	 *            the new value of the functionList property
	 */
	public void setFunctionList(List aFunctionList) {
		functionList = aFunctionList;
	}

	/**
	 * Access method for the deviceList property.
	 * 
	 * @return the current value of the deviceList property
	 */
	public List getDeviceList() {
		return deviceList;
	}

	/**
	 * Sets the value of the deviceList property.
	 * 
	 * @param aDeviceList
	 *            the new value of the deviceList property
	 */
	public void setDeviceList(List aDeviceList) {
		deviceList = aDeviceList;
	}

	public ProjectType getType() {
		return type;
	}

	public void setType(ProjectType type) {
		this.type = type;
	}

	public ProjectStatus getStatus() {
		return status;
	}

	public void setStatus(ProjectStatus status) {
		this.status = status;
	}

	// public ProjectExtra getExtra() {
	// double paymentSum = 0;
	// double receivedSum = 0;
	// double debtSum = 0;
	// double invoiceSum = 0;
	// double invoiceNotReachSum = 0;
	// double reimbursementSum = 0;
	// String currencySymbol = null;
	// if (this.getContractList() != null) {
	// for (Iterator<ContractObj> it = this.getContractList().iterator(); it
	// .hasNext();) {
	// ContractObj contract = it.next();
	// if (currencySymbol == null) {
	// currencySymbol = contract.getCurrency().getSymbol();
	// }
	// paymentSum += contract.getContractAmt();
	// if (contract.getPaymentList() != null) {
	// for (Iterator<PaymentDetailObj> pIt = contract
	// .getPaymentList().iterator(); pIt.hasNext();) {
	// PaymentDetailObj paymentDetail = pIt.next();
	// receivedSum += paymentDetail.getPaymentAmt();
	// }
	// }
	// if (contract.getInvoiceList() != null) {
	// for (Iterator<InvoiceObj> iIt = contract.getInvoiceList()
	// .iterator(); iIt.hasNext();) {
	// InvoiceObj invoice = iIt.next();
	// if (contract.getContractTypeID() == 1) {
	// invoiceSum += invoice.getInvoiceAMT();
	// } else {
	// reimbursementSum += invoice.getInvoiceAMT();
	// }
	// }
	// }
	// }
	// }
	// invoiceNotReachSum = invoiceSum - receivedSum;
	// debtSum = paymentSum - receivedSum;
	// return new ProjectExtra(paymentSum, receivedSum, debtSum, invoiceSum,
	// invoiceNotReachSum, reimbursementSum, currencySymbol);
	// }

	public String toJsonString() {
		return JSONUtils.toJSON(this, true).toString();
	}

	public ContractObj getMainContract() {
		return mainContract;
	}

	public void setMainContract(ContractObj mainContract) {
		this.mainContract = mainContract;
	}

	public SimpleEmployee getManager() {
		return manager;
	}

	public void setManager(SimpleEmployee manager) {
		this.manager = manager;
	}

	public SimpleEmployee getCreator() {
		return creator;
	}

	public void setCreator(SimpleEmployee creator) {
		this.creator = creator;
	}

	public Date getCreateTime() {
		return createTime;
	}

	public void setCreateTime(Date createTime) {
		this.createTime = createTime;
	}

	public Integer getMainContractID() {
		return this.mainContract == null ? null : this.mainContract
				.getContractID();
	}

	public double getIncomAmtGL() {
		return incomAmtGL;
	}

	public void setIncomAmtGL(double incomAmtGL) {
		this.incomAmtGL = incomAmtGL;
	}

	public double getPurchaseAmtToGl() {
		return purchaseAmtToGl;
	}

	public void setPurchaseAmtToGl(double purchaseAmtToGl) {
		this.purchaseAmtToGl = purchaseAmtToGl;
	}
	
	public SimpleDeptObj getExecutiveDept() {
		return executiveDept;
	}

	public void setExecutiveDept(SimpleDeptObj executiveDept) {
		this.executiveDept = executiveDept;
	}

	public String getExecutiveDeptChangeReason() {
		return executiveDeptChangeReason;
	}

	public void setExecutiveDeptChangeReason(String executiveDeptChangeReason) {
		this.executiveDeptChangeReason = executiveDeptChangeReason;
	}

}
