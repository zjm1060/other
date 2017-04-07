package com.bitc.jk.xmis.util;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang.StringUtils;

import com.bitc.jk.xmis.dao.OrganizationDAO;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.web.SpringInit;

import edu.emory.mathcs.backport.java.util.Arrays;

/**
 * IKExpression自定义函数
 * 
 * @author BomWu
 * 
 */
public class ExpressionFunctions {

	public ExpressionFunctions() {
	}

	/**
	 * 根据部门ID，职务ID获取人员ID 。函数名为：post
	 * 
	 * @param deptId
	 *            部门ID
	 * @param postId
	 *            职务ID
	 * @return a List of emp ids
	 */
	public List<String> getEmpIdsByPost(int deptId, int postId) {
		OrganizationDAO service = (OrganizationDAO) SpringInit
				.getBean("organizationDAOImpl");
		List<SimpleEmployee> list = service.getEmpsByPost(deptId, postId);
		List<String> result = new ArrayList<String>();
		for (SimpleEmployee e : list) {
			if(e.getUserID()!=-1)
			result.add(String.valueOf(e.getUserID()));
		}
		return result;
	}

	/**
	 * 函数名：role
	 * 
	 * @param roleId
	 * @return
	 */
	public List<String> getEmpIdsByRole(int roleId) {
		OrganizationDAO service = (OrganizationDAO) SpringInit
				.getBean("organizationDAOImpl");
		List<SimpleEmployee> list = service.getEmpsByRole(roleId);
		List<String> result = new ArrayList<String>();
		for (SimpleEmployee e : list) {
			if(e.getUserID()!=-1)
			result.add(String.valueOf(e.getUserID()));
		}
		return result;
	}

	/**
	 * 函数名： deptRole
	 * 
	 * @param roleId
	 * @param deptId
	 * @return
	 */
	public List<String> getEmpsByRoleAndDept(int roleId, int deptId) {
		OrganizationDAO service = (OrganizationDAO) SpringInit
				.getBean("organizationDAOImpl");
		List<SimpleEmployee> list = service
				.getEmpsByRoleAndDept(roleId, deptId);
		List<String> result = new ArrayList<String>();
		for (SimpleEmployee e : list) {
			if(e.getUserID()!=-1)
			result.add(String.valueOf(e.getUserID()));
		}
		return result;
	}
	
	/**
	 * 函数名： deptLeaderRole
	 * 
	 * @param deptId
	 * @param roleFlag
	 * @return
	 */
	@SuppressWarnings("rawtypes")
	public List<String> getEmpsByDeptLeaderMapping(int deptId, int roleFlag) {
		OrganizationDAO service = (OrganizationDAO) SpringInit
		.getBean("organizationDAOImpl");
		List list = service
		.getEmpsByDeptLeaderMapping(deptId,roleFlag);
		List<String> result = new ArrayList<String>();
		for(int i=0;i<list.size();i++){
		//	Map map = (Map) list.get(i);
		//	result.add(String.valueOf(map.get("user_id")));
			String empID = ((Map) list.get(i)).get("user_id").toString();
			if(!empID.equals("-1"))
			result.add(empID);
		}
		return result;
	}

	public <T> List<T> joinList(List<T> a, List<T> b) {
		a.addAll(b);
		return a;
	}

	/**
	 * 以逗号为分隔的ID转为List
	 * 
	 * @param ids
	 *            以逗号分隔的IDs
	 * @return
	 */
	@SuppressWarnings("unchecked")
	public List<String> getEmpIdsByIds(String ids) {
		List<String> list = new ArrayList<String>();
		if (StringUtils.isEmpty(ids)) {
			return list;
		}
		return Arrays.asList(StringUtils.split(ids, ','));
	}

	public String substringBefore(String str, String separator) {
		return StringUtils.substringBefore(str, separator);
	}

	public String substring(String str, int start, int end) {
		return StringUtils.substring(str, start, end);
	}
	
	/** 函数名 divisionDeptRole
	 * @param roleId
	 * @param divisionId
	 * @return
	 */
	@SuppressWarnings("rawtypes")
	public List<String> getEmpsByRoleAndDivisionID(int roleId, int divisionId) {
		OrganizationDAO service = (OrganizationDAO) SpringInit
		.getBean("organizationDAOImpl");
		List list = service
		.getEmpsByRoleAndDivisionID(roleId, divisionId);
		List<String> result = new ArrayList<String>();
		for (int i=0;i<list.size();i++) {
			String empID = ((Map) list.get(i)).get("人员ID").toString();
			if(!empID.equals("-1"))
			result.add(empID);
		}
		return result;
	}
}
