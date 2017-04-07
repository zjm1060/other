package com.bitc.jk.xmis.model;

import java.io.Serializable;
import java.util.Date;

/**
 * 数据库表类Bean公共父类。都含有最后修改日期与最后修改人两个属性
 * 
 * @author BomWu
 * 
 */
public class TableBean implements Serializable {

	private static final long serialVersionUID = 3714302469474525974L;

	/**
	 * 最后修改日期，含时间
	 */
	private Date lastModified;
	/**
	 * 最后修改人
	 */
	private SimpleEmployee lastModifier;

	public Date getLastModified() {
		return lastModified;
	}

	public void setLastModified(Date lastModified) {
		this.lastModified = lastModified;
	}

	public SimpleEmployee getLastModifier() {
		return lastModifier;
	}

	public void setLastModifier(SimpleEmployee lastModifier) {
		this.lastModifier = lastModifier;
	}

}
