package com.bitc.jk.xmis.dao.impl;

import java.sql.SQLException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import org.aspectj.lang.JoinPoint;
import org.hibernate.HibernateException;
import org.hibernate.Query;
import org.hibernate.Session;
import org.springframework.orm.hibernate3.HibernateCallback;

import com.bitc.jk.xmis.dao.SysLogDAO;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.model.sys.SysLog;
import com.bitc.jk.xmis.util.DateUtil;
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
		 "(SELECT COUNT(1) Num from xmis.sys_系统日志   where datediff(dd,opTime,getdate())<= datepart(dw,getdate()) AND opType='login' and userID=? ) AS a , "
		 +
		 "(select CONVERT(VARCHAR(100),MAX(opTime),120) AS last_date from xmis.sys_系统日志 where opType='login' and userID=? ) b ";

		// bbz mysql
//		final String sql = "select *  from xmis.sys_系统日志\n"
//				+ " WHERE opType = 'login'  AND userID =? and opTime >=?"
//				+ " order by opTime desc";

		return (List) ht.execute(new HibernateCallback() {
			
			public Object doInHibernate(Session s) throws HibernateException,
			SQLException {
		Query query = s.createSQLQuery(sql);
		query.setString(0, String.valueOf(userID));
		query.setString(1, String.valueOf(userID));
		return query.list();
			}
			

			//mysql
//			public Object doInHibernate(Session s) throws HibernateException,
//					SQLException {
//
//				Query query = s.createSQLQuery(sql);
//				query.setString(0, String.valueOf(userID));
//				query.setDate(1,
//						DateUtil.getFirstDayOfWeek(DateUtil.getCurrentDate()));
//				List it = query.list();
//				int count = it.size();
//				String lastLogTime = null;
//				SimpleDateFormat sdf=new SimpleDateFormat("yyyy-MM-dd");
//				if (it.size()>0) {
//					lastLogTime =sdf.format((Date) ((Object[])it.get(0))[2]);
//				}
//				List list = new ArrayList();
//				list.add(new Object[] { count, lastLogTime });
//				return list;
//			}
		});
	}
}
