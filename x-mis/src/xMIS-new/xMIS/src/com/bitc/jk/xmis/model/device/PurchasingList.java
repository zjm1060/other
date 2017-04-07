package com.bitc.jk.xmis.model.device;

import java.util.Date;
import java.util.HashSet;
import java.util.Set;

import org.apache.commons.lang.StringUtils;

import com.bitc.jk.xmis.model.CompanySimpleObj;
import com.bitc.jk.xmis.model.SimpleDeptObj;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.model.TableBean;

/**
 * 采购清单
 * 
 * @author Bom Wu
 * 
 */
public class PurchasingList extends TableBean {

	private static final long serialVersionUID = 5005827200513861658L;

	/**
	 * ID
	 */
	private String id;
	/**
	 * 清单编码
	 */
	private String code;

	/**
	 * 备注
	 */
	private String remark;
	/**
	 * 清单明细
	 */
	private Set<PurchasingDetail> details;

	/**
	 * 填表人
	 */
	private SimpleEmployee entryEmp;

	/**
	 * 采购部门
	 */
	private SimpleDeptObj dept;
	
	public SimpleDeptObj getDept() {
		return dept;
	}

	public void setDept(SimpleDeptObj dept) {
		this.dept = dept;
	}

	/**
	 * 创建时间、更新时间
	 */
	private Date entryDate;

	/**
	 * 供货商
	 */
	private CompanySimpleObj supplier;
	
	private Integer contractId;

	public Integer getContractId() {
		return contractId;
	}

	public void setContractId(Integer contractId) {
		this.contractId = contractId;
	}

	public PurchasingList() {
		details = new HashSet<PurchasingDetail>();
	}

	public String getId() {
		return id;
	}

	public void setId(String id) {
		this.id = StringUtils.isBlank(id) ? null : id;
	}

	public String getCode() {
		return code;
	}

	public void setCode(String code) {
		this.code = code;
	}

	public String getRemark() {
		return remark;
	}

	public void setRemark(String remark) {
		this.remark = remark;
	}

	public SimpleEmployee getEntryEmp() {
		return entryEmp;
	}

	public void setEntryEmp(SimpleEmployee entryEmp) {
		this.entryEmp = entryEmp;
	}

	public Date getEntryDate() {
		return entryDate;
	}

	public void setEntryDate(Date entryDate) {
		this.entryDate = entryDate;
	}

	public void addDetail(PurchasingDetail detail) {
		this.details.add(detail);
	}

	public Set<PurchasingDetail> getDetails() {
		return details;
	}

	public void setDetails(Set<PurchasingDetail> details) {
		this.details = details;
	}

	public CompanySimpleObj getSupplier() {
		return supplier;
	}

	public void setSupplier(CompanySimpleObj supplier) {
		this.supplier = supplier;
	}

}
