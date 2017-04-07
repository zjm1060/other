package com.bitc.jk.xmis.model.crm;

import com.bitc.jk.xmis.model.TableBean;
import com.bitc.jk.xmis.util.JSONUtils;


/**
 * @author liying
 *    [洲际ID]
      ,[编码]
      ,[名称]
      ,[简称]
      ,[最后修改日期]
      ,[最后修改人ID]
[xmis].[add_洲际表]
 */
public class Internation  extends TableBean{

	/**
	 * 
	 */
	private static final long serialVersionUID = 1L;
	private Integer internationID;
	private String internationCode;
	private String internationName;
	private String abbreviation;
	
	public Integer getInternationID() {
		return internationID;
	}
	public void setInternationID(Integer internationID) {
		this.internationID = internationID;
	}
	public String getInternationCode() {
		return internationCode;
	}
	public void setInternationCode(String internationCode) {
		this.internationCode = internationCode;
	}
	public String getInternationName() {
		return internationName;
	}
	public void setInternationName(String internationName) {
		this.internationName = internationName;
	}
	public String getAbbreviation() {
		return abbreviation;
	}
	public void setAbbreviation(String abbreviation) {
		this.abbreviation = abbreviation;
	}
	public String toJsonString() {
		return JSONUtils.toJSON(this, true).toString();
	}
}
