package com.bitc.jk.xmis.task;

import java.util.Date;
import java.util.List;

import com.bitc.jk.xmis.dao.impl.BaseDAOImpl;
import com.bitc.jk.xmis.util.DateUtil;

public class TaskDAO extends BaseDAOImpl {

	@SuppressWarnings("unchecked")
	public List<Task> getAllActiveTask() {//查询任务列表
		String nowTime = DateUtil.formatDateTime(new Date());
		String hql = "from Task where state=0 and executeTime<='"+nowTime+"'";
		return ht.find(hql);
	}

	public void addTask(Task task){ //添加一条任务
		ht.save(task);
	}
	
}
