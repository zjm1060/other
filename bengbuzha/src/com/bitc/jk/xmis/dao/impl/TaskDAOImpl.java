//Source file: D:\\Ted Li\\X-MIS\\src\\xMIS-source\\com\\bitc\\jk\\xmis\\dao\\impl\\TaskDAOImpl.java

package com.bitc.jk.xmis.dao.impl;

import com.bitc.jk.xmis.dao.TaskDAO;
import com.bitc.jk.xmis.model.TaskObj;

public class TaskDAOImpl extends BaseDAOImpl implements TaskDAO  
{

	public void saveTask(TaskObj task) {
		saveOrUpdateObject(task);
	}
}
