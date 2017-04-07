//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\service\\LoginService.java

package com.bitc.jk.xmis.service;

import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;

import com.bitc.jk.xmis.model.ContractObj;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.sys.SysLog;
import com.bitc.jk.xmis.model.sys.UserHabit;

public interface LoginService {

	/**
	 * @param loginName
	 * @param password
	 * @param request
	 * @return com.bitc.jk.xmis.model.EmployeeObj
	 * @roseuid 4C198BC0008C
	 */
	public EmployeeObj doUserValidete(String loginName, String password,
			HttpServletRequest request) throws Exception;

	/**
	 * @param userID
	 * @return List
	 * @roseuid 4C198C0501F4
	 */
	public List getUserRight(int userID);

	public String buildLeftMenu(String treeName, EmployeeObj userDNA);

	public UserHabit getUserHabit(EmployeeObj userObj);

	public void doSaveUserHabit(HttpServletRequest request, EmployeeObj userObj);

	public Map getSecrityDataScope(String url, EmployeeObj userDNA, boolean recFlag);

	public void doSaveSysLog(SysLog sysLog);
	
	public int getContractOpType(EmployeeObj userDNA, ContractObj contract);
}
