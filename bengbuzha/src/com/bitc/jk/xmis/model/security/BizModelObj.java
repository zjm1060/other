/**
 * 
 */
package com.bitc.jk.xmis.model.security;

import java.io.Serializable;
import java.sql.Date;
import java.util.HashMap;
import java.util.Map;

/**
 * @author Ted Li
 * 
 *         2011-5-12
 */
public class BizModelObj implements Serializable {

	private static final long serialVersionUID = 8432945251024559582L;
	protected int id;
	protected String name;
	protected String desc;
	protected String url;
	protected int sequenceNo;
	protected String iconCls;
	protected String identifies;
	private int parentID;
	private BizModelObj parentObject;
	private int commitPerson;
	private String commitPersonName;
	private Date commitDate;
	private int commitStatus;
	private String commitStatusName;

	private String remark;
	private String changedRecord;

	public BizModelObj() {
	}

	public BizModelObj(int id) {
		this.id = id;
	}
	
	private Map bizObjMap = new HashMap();

	public String getIconCls() {
		return iconCls;
	}

	public void setIconCls(String iconCls) {
		this.iconCls = iconCls;
	}

	public String getDesc() {
		return desc;
	}

	public void setDesc(String desc) {
		this.desc = desc;
	}

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public int getSequenceNo() {
		return sequenceNo;
	}

	public void setSequenceNo(int sequenceNo) {
		this.sequenceNo = sequenceNo;
	}

	public String getUrl() {
		return url;
	}

	public void setUrl(String url) {
		this.url = url;
	}

	public Map getBizObjMap() {
		return bizObjMap;
	}

	public void setBizObjMap(Map bizObjMap) {
		this.bizObjMap = bizObjMap;
	}

	public void addBizObjMap(String id, Object o) {
		this.bizObjMap.put(id, o);
	}

	public String getIdentifies() {
		return identifies;
	}

	public void setIdentifies(String identifies) {
		this.identifies = identifies;
	}

	public int getParentID() {
		return parentID;
	}

	public void setParentID(int parentID) {
		this.parentID = parentID;
	}

	public BizModelObj getParentObject() {
		return parentObject;
	}

	public void setParentObject(BizModelObj parentObject) {
		this.parentObject = parentObject;
	}

	public int getCommitPerson() {
		return commitPerson;
	}

	public void setCommitPerson(int commitPerson) {
		this.commitPerson = commitPerson;
	}

	public Date getCommitDate() {
		return commitDate;
	}

	public void setCommitDate(Date commitDate) {
		this.commitDate = commitDate;
	}

	public String getRemark() {
		return remark;
	}

	public void setRemark(String remark) {
		this.remark = remark;
	}

	public String getCommitPersonName() {
		return commitPersonName;
	}

	public void setCommitPersonName(String commitPersonName) {
		this.commitPersonName = commitPersonName;
	}

	public int getCommitStatus() {
		return commitStatus;
	}

	public void setCommitStatus(int commitStatus) {
		this.commitStatus = commitStatus;
	}

	public String getCommitStatusName() {
		return commitStatusName;
	}

	public void setCommitStatusName(String commitStatusName) {
		this.commitStatusName = commitStatusName;
	}

	public String getChangedRecord() {
		return changedRecord;
	}

	public void setChangedRecord(String changedRecord) {
		this.changedRecord = changedRecord;
	}

}
