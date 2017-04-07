package com.bitc.jk.xmis.model.sys;

import java.io.Serializable;
import java.util.List;

import com.bitc.jk.xmis.model.security.BizModuleObj;

/**
 * 
 * @author LHY
 *
 */
public class Status implements Serializable {
	
	private static final long serialVersionUID = -6653328382673560475L;
	
	private int id;
	private String status;
	
	/**
	 * 业务模块列表
	 */
	private List<BizModuleObj> bizModuleObjs;
	
	public Status() {
	}
	
	public int getId() {
		return id;
	}
	public void setId(int id) {
		this.id = id;
	}
	public String getStatus() {
		return status;
	}
	public void setStatus(String status) {
		this.status = status;
	}
	
	public List<BizModuleObj> getBizModuleObjs() {
		return bizModuleObjs;
	}

	public void setBizModuleObjs(List<BizModuleObj> bizModuleObjs) {
		this.bizModuleObjs = bizModuleObjs;
	}

}
