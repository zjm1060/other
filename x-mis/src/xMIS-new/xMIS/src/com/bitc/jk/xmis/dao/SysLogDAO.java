package com.bitc.jk.xmis.dao;

import java.util.List;

import com.bitc.jk.xmis.model.sys.SysLog;

public interface SysLogDAO {

	public void saveSysLog(SysLog log);
	@SuppressWarnings("rawtypes")
	public List getLastLogTime(final int userID);
	public List getTodayList(int userID);
	public List getYesterdayList(int userID);

}
