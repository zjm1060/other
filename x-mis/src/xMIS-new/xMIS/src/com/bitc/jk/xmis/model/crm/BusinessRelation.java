package com.bitc.jk.xmis.model.crm;

import java.sql.Date;

import com.bitc.jk.xmis.model.TableBean;

/**
 * @author liying
 * 业务关系表
 * 	   [业务关系ID]
      ,[关系名称]
      ,[关系描述]
      ,[最后修改日期]
      ,[最后修改人ID]
 */
public class BusinessRelation extends TableBean{

	private int businessRelationID;
	private String businessRelationName;
	private String businessRelationDesc;
	private Date updateDate;
	private int updateID;
	
	public int getBusinessRelationID() {
		return businessRelationID;
	}
	public void setBusinessRelationID(int businessRelationID) {
		this.businessRelationID = businessRelationID;
	}
	public String getBusinessRelationName() {
		return businessRelationName;
	}
	public void setBusinessRelationName(String businessRelationName) {
		this.businessRelationName = businessRelationName;
	}
	public String getBusinessRelationDesc() {
		return businessRelationDesc;
	}
	public void setBusinessRelationDesc(String businessRelationDesc) {
		this.businessRelationDesc = businessRelationDesc;
	}
	public Date getUpdateDate() {
		return updateDate;
	}
	public void setUpdateDate(Date updateDate) {
		this.updateDate = updateDate;
	}
	public int getUpdateID() {
		return updateID;
	}
	public void setUpdateID(int updateID) {
		this.updateID = updateID;
	}
	
	
}
