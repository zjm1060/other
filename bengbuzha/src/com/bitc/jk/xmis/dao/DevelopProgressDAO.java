package com.bitc.jk.xmis.dao;

import java.util.List;

/**
 * 
 * @author LHY
 *
 */
public interface DevelopProgressDAO {
	public List getDevelopProgressList();
	public List getMonthAccounting(String start,String end,String sort);
	public List getMonthAccounting(String start,String end,String sort,String deptName);
	public List getMonthAccounting(String start,String end,String sort,int deptID);
	public int getSubDeptCount(String deptName);
	public List getMonthAccounting2(String start,String end,String sort,int deptID);
	public List getMonthAccounting3(String start,String end,String sort,String deptName);
	public List getPersonAndMonthTotalInCompany(String start,String end,String sort);
	
	public List getLastAccounting(String sort);
	public List getLastAccounting(String sort,String deptName);
	public List getLastAccounting7(String sort,String deptName);
	public List getLastAccounting2(String sort,String deptName);
	public List getPersonOfCompInOneWeek(String start,String end,String sort);
	public List getMonthAccounting5(String start,String end,String sort,String deptName);
	public List getPersonOfCompByComp(String sort);
}
