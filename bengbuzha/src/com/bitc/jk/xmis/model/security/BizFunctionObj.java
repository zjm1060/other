/**
 * 
 */
package com.bitc.jk.xmis.model.security;

/**
 * @author Ted Li
 * 
 *         2011-5-12
 */
public class BizFunctionObj extends BizModelObj {

	private int bizFunctionID;

	private BizOperationObj bizOperation;

	public BizFunctionObj() {
	}

	public BizFunctionObj(String name) {
		this.name = name;
	}

	public BizFunctionObj(int bizFunctionID) {
		this.bizFunctionID = bizFunctionID;
	}
	
	public BizFunctionObj(int bizFunctionID,String name) {
		this.bizFunctionID = bizFunctionID;
		this.name = name;
	}

	public BizOperationObj getBizOperation() {
		return bizOperation;
	}

	public void setBizOperation(BizOperationObj bizOperation) {
		this.bizOperation = bizOperation;
	}

	public int getBizFunctionID() {
		return bizFunctionID;
	}

	public void setBizFunctionID(int bizFunctionID) {
		this.bizFunctionID = bizFunctionID;
	}

	@Override
	public int hashCode() {
		final int prime = 31;
		int result = super.hashCode();
		result = prime * result + bizFunctionID;
		return result;
	}

	@Override
	public boolean equals(Object obj) {
		if (this == obj)
			return true;
		if (!super.equals(obj))
			return false;
		if (getClass() != obj.getClass())
			return false;
		BizFunctionObj other = (BizFunctionObj) obj;
		if (bizFunctionID != other.bizFunctionID)
			return false;
		return true;
	}

}
