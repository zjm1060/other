/**
 * 
 */
package com.bitc.jk.xmis.model.security;

import java.util.List;

/**
 * @author Ted Li
 * 
 *         2011-5-12
 */
public class BizSubModuleObj extends BizModelObj {

	private List<BizMappingObj> mappings;

	private List<BizFunctionObj> bizFunctionList;

	public BizSubModuleObj() {

	}

	public BizSubModuleObj(int id) {
		this.id = id;
	}

	public BizSubModuleObj(int id, String name) {
		this.id = id;
		this.name = name;
	}

	public BizSubModuleObj(String name) {
		this.name = name;
	}

	public List<BizMappingObj> getMappings() {
		return mappings;
	}

	public void setMappings(List<BizMappingObj> mappings) {
		this.mappings = mappings;
	}

	public List<BizFunctionObj> getBizFunctionList() {
		return bizFunctionList;
	}

	public void setBizFunctionList(List<BizFunctionObj> bizFunctionList) {
		this.bizFunctionList = bizFunctionList;
	}

	public void addBizFunctionList(BizFunctionObj obj) {
		this.bizFunctionList.add(obj);
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = 1;
		result = prime * result + id;
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (obj == null)
			return false;
		if (getClass() != obj.getClass())
			return false;
		BizModelObj other = (BizModelObj) obj;
		if (id != other.id)
			return false;
		return true;
	}
}
