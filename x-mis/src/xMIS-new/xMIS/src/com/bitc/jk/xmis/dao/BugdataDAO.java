package com.bitc.jk.xmis.dao;

import com.bitc.jk.xmis.model.EmployeeObj;

public interface BugdataDAO {


	/** 根据名字判断mantis中此人是否存在
	 * @param name
	 * @return
	 */
	public boolean isExistName(String name);
	
	/** 修改已有用户信息
	 * @param emp
	 * @param misRole
	 * @param oldname 
	 */
	public void doUpdateMantisData(EmployeeObj emp, int misRole, String oldname) ;
	
	/** 修改已有用户密码
	 * @param emp
	 * @param misRole
	 */
	public void doUpdateMantisPwd(String name,String password) ;

	/** 新增
	 * @param emp
	 * @param misRole
	 */
	public void  doInsertMantisData (EmployeeObj emp, int misRole) ;

	/** 根据名字得到mantis中角色
	 * @param name
	 * @return
	 */
	public int getMantisRole(String name);

}