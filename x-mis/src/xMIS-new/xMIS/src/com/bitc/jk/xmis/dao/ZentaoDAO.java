package com.bitc.jk.xmis.dao;

import java.util.List;

public interface ZentaoDAO {

	/** 新增
	 * @param emp
	 * @param misRole
	 * @return 
	 */
	public boolean  doInsertZentaoData (List dataList) ;

	public boolean doInsertZentaoUserGroupData(List list);
	
	public int doUpdateZentaoPwd(String name, String password);

}