package com.bitc.jk.xmis.dao.impl;

import java.sql.SQLException;
import java.util.Arrays;
import java.util.List;

import org.aspectj.lang.JoinPoint;
import org.hibernate.HibernateException;
import org.hibernate.Query;
import org.hibernate.Session;
import org.springframework.orm.hibernate3.HibernateCallback;

import com.bitc.jk.xmis.dao.SysLogDAO;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.model.sys.SysLog;
import com.bitc.jk.xmis.util.SQLScript;
import com.bitc.jk.xmis.util.WebUtil;

public class SysLogDAOImpl extends BaseDAOImpl implements SysLogDAO {

	public void saveSysLog(SysLog log) {
		saveObject(log);
	}

	public void log(JoinPoint joinPoint) {
		SimpleEmployee emp = WebUtil.getCurrentEmployee();
		System.out.print("操作用户：" + emp.getUserName() + "\t切入成功，参数列表：");
		Object[] args = joinPoint.getArgs();
		System.out.println(Arrays.toString(args));

	}
	
	@SuppressWarnings("rawtypes")
	public List getLastLogTime(final int userID) {
		final String sql ="SELECT a.Num,b.last_date FROM " +
				"(SELECT COUNT(1) Num from xmis.sys_系统日志   where datediff(dd,opTime,getdate())<= datepart(dw,getdate()) AND opType='login' and userID=? ) AS a , " +
				"(select CONVERT(VARCHAR(100),MAX(opTime),120) AS last_date from xmis.sys_系统日志 where opType='login' and userID=? ) b ";
		return (List) ht.execute(new HibernateCallback() {
			
			public Object doInHibernate(Session s) throws HibernateException,
					SQLException {
				Query query = s.createSQLQuery(sql);
				query.setString(0, String.valueOf(userID));
				query.setString(1, String.valueOf(userID));
				return query.list();
			}
		});
	}

	@SuppressWarnings("rawtypes")
	public List getTodayList(final int userID) {

		return (List) ht.execute(new HibernateCallback() {
			String sql = SQLScript.getTodayList;
			public Object doInHibernate(Session s) throws HibernateException,
					SQLException {
				Query query = s.createSQLQuery(sql);
				query.setInteger(0, userID);
				return query.list();
			}
		});
	}

	@SuppressWarnings("rawtypes")
	public List getYesterdayList(final int userID) {
		return (List) ht.execute(new HibernateCallback() {
			String sql = SQLScript.getYesterdayList;
			public Object doInHibernate(Session s) throws HibernateException,
					SQLException {
				Query query = s.createSQLQuery(sql);
				query.setInteger(0, userID);
				return query.list();
			}
		});
	}
}
