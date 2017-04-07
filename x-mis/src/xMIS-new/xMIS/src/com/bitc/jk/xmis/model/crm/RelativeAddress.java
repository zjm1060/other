package com.bitc.jk.xmis.model.crm;

import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.model.TableBean;

public class RelativeAddress extends TableBean {
	/**
	 *  公司相关地址表
	 */
	private static final long serialVersionUID = 1L;
	private String id;
	private Integer companyId;
	private Address address;
	private SimpleEmployee contactPersonA;
	private SimpleEmployee contactPersonB;
	
	public String getId() {
		return id;
	}
	public void setId(String id) {
		this.id = id;
	}
	public Integer getCompanyId() {
		return companyId;
	}
	public void setCompanyId(Integer companyId) {
		this.companyId = companyId;
	}
	public Address getAddress() {
		return address;
	}
	public void setAddress(Address address) {
		this.address = address;
	}
	public SimpleEmployee getContactPersonA() {
		return contactPersonA;
	}
	public void setContactPersonA(SimpleEmployee contactPersonA) {
		this.contactPersonA = contactPersonA;
	}
	public SimpleEmployee getContactPersonB() {
		return contactPersonB;
	}
	public void setContactPersonB(SimpleEmployee contactPersonB) {
		this.contactPersonB = contactPersonB;
	}

}
