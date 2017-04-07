package com.bitc.jk.xmis.service.impl;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import javax.servlet.http.HttpServletRequest;

import com.bitc.jk.xmis.dao.DevelopProgressDAO;
import com.bitc.jk.xmis.service.DevelopProgressService;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebUtil;

public class DevelopProgressServiceImpl implements DevelopProgressService {

	private DevelopProgressDAO developProgressDAO;

	public List getDevelopProgressList() {
		List list = developProgressDAO.getDevelopProgressList();
		return list;
	}

	public List<Map<Integer, Integer>> getCounts(List list){
		List<Integer> allModuleIds = new ArrayList<Integer>();
		List<Integer> allSubModuleIds = new ArrayList<Integer>();
		for (int i = 0; i < list.size(); i++) {
			if (i == 0) {
				allModuleIds.add(Type.getInteger(((Map) list.get(0))
						.get("业务模块ID")));
				allSubModuleIds.add(Type.getInteger(((Map) list.get(0))
						.get("业务子模块ID")));
			} else {
				this.addItem(
						Type.getInteger(((Map) list.get(i)).get("业务模块ID")),
						allModuleIds);
				this.addItem(Type
						.getInteger(((Map) list.get(i)).get("业务子模块ID")),
						allSubModuleIds);
			}
		}
		Map<Integer, Integer> allModuleRepeatNum = new HashMap<Integer, Integer>();
		for (Integer num : allModuleIds) {
			int count = 0;
			for (int i = 0; i < list.size(); i++) {
				if (num == Type.GetInt(((Map) list.get(i)).get("业务模块ID"))) {
					count++;
				}
			}
			allModuleRepeatNum.put(num, count);
		}
		Map<Integer, Integer> allSubModuleRepeatNum = new HashMap<Integer, Integer>();
		for (Integer num : allSubModuleIds) {
			int count = 0;
			for (int i = 0; i < list.size(); i++) {
				if (num == Type.GetInt(((Map) list.get(i)).get("业务子模块ID"))) {
					count++;
				}
			}
			allSubModuleRepeatNum.put(num, count);
		}
		List<Map<Integer, Integer>> result = new ArrayList<Map<Integer, Integer>>();
		result.add(allModuleRepeatNum);
		result.add(allSubModuleRepeatNum);
		return result;
	}
	
	private void addItem(int a, List<Integer> list) {
		boolean isRepeatflag = false;
		for (Integer num : list) {
			if (num == a) {
				isRepeatflag = true;
				break;
			}
		}
		if (!isRepeatflag) {
			list.add(a);
		}
	}
	
	public String getMonthAccounting(HttpServletRequest request) {
		String searchType = request.getParameter("searchType");
		String start = request.getParameter("start");
		String end = request.getParameter("end");
		String sort = request.getParameter("sort");
		String deptName = request.getParameter("deptName");
		int deptID = Type.GetInt(request.getParameter("deptID"));
		String sortWeek = request.getParameter("sortWeek");
		
		if(searchType != null && "personMonth".equals(searchType)){
			//部门下的月报,得到的结果为人
			int count = developProgressDAO.getSubDeptCount(deptName);
			List list = null;
			if(count > 0){
				//有子部门如监控
				list = developProgressDAO.getMonthAccounting5(start,end,sort,deptName);
			}else{
				//没有子部门
				list = developProgressDAO.getMonthAccounting(start,end,sort,deptName);
			}
			System.out.println(list.size());
			String json = WebUtil.buildJsonStoreByList(list);
			return json;
		}
		if(searchType != null && "personWeek".equals(searchType)){
			int count = developProgressDAO.getSubDeptCount(deptName);
			List list = null;
			if(count > 0){
				list = developProgressDAO.getMonthAccounting2(start,end,sortWeek,deptID);
			}else{
				list = developProgressDAO.getMonthAccounting(start,end,sortWeek,deptID);
			}
			
			System.out.println(list.size());
			String json = WebUtil.buildJsonStoreByList(list);
			return json;
		}
		if(searchType != null && "getPersonOfCompInOneWeek".equals(searchType)){
			List list = developProgressDAO.getPersonOfCompInOneWeek(start,end,sortWeek);
			System.out.println(list.size());
			String json = WebUtil.buildJsonStoreByList(list);
			return json;
		}
		if(searchType != null && "subMonth".equals(searchType)){
			List list = developProgressDAO.getMonthAccounting3(start,end,sort,deptName);
			System.out.println(list.size());
			String json = WebUtil.buildJsonStoreByList(list);
			return json;
		}
		
		if(searchType != null && "getPersonAndMonthTotalInCompany".equals(searchType)){
			List list = developProgressDAO.getPersonAndMonthTotalInCompany(start,end,sort);
			System.out.println(list.size());
			String json = WebUtil.buildJsonStoreByList(list);
			return json;
		}
		
		List list = developProgressDAO.getMonthAccounting(start, end,sort);
		System.out.println(list.size());
		String json = WebUtil.buildJsonStoreByList(list);
		return json;
	}
	
	public String getWeekAccounting(HttpServletRequest request) {
		String searchType = request.getParameter("searchType");
		String sortWeek = request.getParameter("sortWeek");
		String deptName = request.getParameter("deptName");
		
		if(searchType != null && "personLastWeek".equals(searchType)){
			List list = developProgressDAO.getLastAccounting(sortWeek);
			System.out.println(list.size());
			String json = WebUtil.buildJsonStoreByList(list);
			return json;
		}
		if(searchType != null && "getPersonOfDeptByDeptID".equals(searchType)){
			int count = developProgressDAO.getSubDeptCount(deptName);
			List list = null;
			if(count > 0){
				list = developProgressDAO.getLastAccounting7(sortWeek,deptName);
			}else{
				list = developProgressDAO.getLastAccounting(sortWeek,deptName);
			}
			System.out.println(list.size());
			String json = WebUtil.buildJsonStoreByList(list);
			return json;
		}
		if(searchType != null && "getSubDeptOfDeptByDeptID".equals(searchType)){
			List list = developProgressDAO.getLastAccounting2(sortWeek,deptName);
			System.out.println(list.size());
			String json = WebUtil.buildJsonStoreByList(list);
			return json;
		}
		if(searchType != null && "getPersonOfCompByComp".equals(searchType)){
			List list = developProgressDAO.getPersonOfCompByComp(sortWeek);
			System.out.println(list.size());
			String json = WebUtil.buildJsonStoreByList(list);
			return json;
		}
		return "";
	}

	public DevelopProgressDAO getDevelopProgressDAO() {
		return developProgressDAO;
	}

	public void setDevelopProgressDAO(DevelopProgressDAO developProgressDAO) {
		this.developProgressDAO = developProgressDAO;
	}

	public int getSubDeptCount(String deptName) {
		return developProgressDAO.getSubDeptCount(deptName);
	}

	

}
