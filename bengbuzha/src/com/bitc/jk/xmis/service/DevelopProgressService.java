package com.bitc.jk.xmis.service;

import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;

import com.bitc.jk.xmis.model.security.BizModuleObj;

/**
 * 
 * @author LHY
 *
 */
public interface DevelopProgressService {
	public List getDevelopProgressList();
	public List<Map<Integer, Integer>> getCounts(List list);
	public String getMonthAccounting(HttpServletRequest request);
	public int getSubDeptCount(String deptName);
	public String getWeekAccounting(HttpServletRequest request);
}
