/**
 * 
 */
package com.bitc.jk.xmis.model.security;

/**
 * @author Ted Li
 *
 * 2011-5-13
 */
public class BizMappingObj {
	
	private int masterID;
	private int subID;
	
	public BizMappingObj(){
	}
	
	public BizMappingObj(int masterID,int subID){
		this.masterID = masterID;
		this.subID = subID;
	}
	
	public int getMasterID() {
		return masterID;
	}
	public void setMasterID(int masterID) {
		this.masterID = masterID;
	}
	public int getSubID() {
		return subID;
	}
	public void setSubID(int subID) {
		this.subID = subID;
	}
	

}
