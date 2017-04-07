package com.bitc.jk.xmis.dao.impl;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.springframework.dao.DataAccessException;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.PreparedStatementCallback;

import com.bitc.jk.xmis.dao.BugdataDAO;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.SimpleEmployee;

public class BugdataImpl implements BugdataDAO {
	public static Logger logger = Logger.getLogger(BugdataImpl.class);

	
	private JdbcTemplate mjt;

	public JdbcTemplate getMjt() {
		return mjt;
	}

	public void setMjt(JdbcTemplate mjt) {
		this.mjt = mjt;
	}

	/** 根据名字判断mantis中此人是否存在
	 * @param name
	 * @return
	 */
	public boolean isExistName(final String name){
		String sql = "select count(id) from mantis_user_table where realname=?";
		Object o = mjt.execute(sql, new PreparedStatementCallback() {
			
			public Object doInPreparedStatement(PreparedStatement ps)
					throws SQLException, DataAccessException {
				int num = 0;
				boolean isExist = false;
				ps.setString(1, name);
				ResultSet rs = ps.executeQuery();
				if(rs.next()){
					num = rs.getInt(1);
					if(num>0){
						isExist = true;
					}else {
						isExist= false; 
					}
				} 
				return isExist;
			}
		});
		return (Boolean) o;
		
	}
	
	/** 修改已有用户信息
	 * @param emp
	 * @param misRole
	 * @param oldname 
	 */
	public void doUpdateMantisData(EmployeeObj emp, int misRole, String oldname) {
		logger.info("//updateMantisData-->");
		
		String email = emp.getEmailAddress();
		if(!StringUtils.isEmpty(email)){
			email = emp.getEmailAddress().trim();
		}
		logger.info("updateMantisData//email-->"+email);
		String username = emp.getLastName().trim()+emp.getFirstName().trim();
		String sql = "update mantis_user_table set username='"+emp.getLoginName()+"',realname='"+username+"',email='"+email+"',access_level='"+misRole+"' where realname='"+oldname+"'";
		mjt.update(sql);
	}
	
	/** 修改已有用户密码
	 * @param emp
	 * @param misRole
	 */
	public void doUpdateMantisPwd(String name,String password) {
		logger.info("updateMantisPwd//password-->"+password);
		if(isExistName(name)){//此人是否在mantis中存在
			if(!StringUtils.isEmpty(password)){
				password = password.trim();
			}
			String sql = "update mantis_user_table set password='"+getMd5(password)+"' where realname='"+name+"'";
			mjt.update(sql);
		}
		
	}

	/** 新增
	 * @param emp
	 * @param misRole
	 */
	public void  doInsertMantisData (EmployeeObj emp, int misRole) {
		logger.info("//insertMantisData-->");

		String f = emp.getFirstName();
		if(!StringUtils.isEmpty(f)){
			f = f.trim();
		}
		String l = emp.getLastName();
		if(!StringUtils.isEmpty(l)){
			l = l.trim();
		}
		String email = emp.getEmailAddress();
		if(!StringUtils.isEmpty(email)){
			email = emp.getEmailAddress().trim();
		}
		String password = "000000";
		logger.info("insertMantisData//email-->"+email);
		String sql = "insert into mantis_user_table(username,realname,email,password,access_level,cookie_string) values('"+emp.getLoginName()+"','"+(l+f)+"','"+email+"','"+getMd5(password)+"','"+misRole+"','"+emp.getLoginName()+"')";
		mjt.update(sql);
	}

	/** 根据名字得到mantis中角色
	 * @param name
	 * @return
	 */
	public int getMantisRole(String name){
		String sql = "select access_level from mantis_user_table where username='"+name+"'";
		Object o = mjt.execute(sql, new PreparedStatementCallback() {
			
			public Object doInPreparedStatement(PreparedStatement ps)
					throws SQLException, DataAccessException {
				ResultSet rs = ps.executeQuery();
				if(rs.next()){
					return rs.getObject(1);
				}
				return 0;
			}
		});
		return (Integer) o;
	}

	
	/** 32位的加密
	 * @param msg
	 * @return
	 */
	public static String getMd5(String msg){
		MessageDigest md;
		if(!StringUtils.isEmpty(msg)){
			try {
				md = MessageDigest.getInstance("MD5");
				md.update(msg.getBytes());
				byte b[] = md.digest();
				
				int i;
				StringBuffer buf = new StringBuffer("");
				for (int offset = 0; offset < b.length; offset++) {
					i = b[offset];
					if (i < 0)
						i += 256;
					if (i < 16)
						buf.append("0");
					buf.append(Integer.toHexString(i));
				}
				return buf.toString();
			} catch (NoSuchAlgorithmException e) {
				e.printStackTrace();
			}
		}
		return msg;
	}
	

}
