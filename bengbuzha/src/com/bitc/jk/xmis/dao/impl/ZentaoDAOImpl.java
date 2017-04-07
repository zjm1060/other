package com.bitc.jk.xmis.dao.impl;

import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.List;
import java.util.Map;

import org.springframework.jdbc.core.BatchPreparedStatementSetter;
import org.springframework.jdbc.core.JdbcTemplate;

import com.bitc.jk.xmis.dao.ZentaoDAO;
import com.bitc.jk.xmis.util.TemplateUtil;
import com.bitc.jk.xmis.util.Type;

public class ZentaoDAOImpl implements ZentaoDAO {
	public JdbcTemplate zjt;

	public JdbcTemplate getZjt() {
		return zjt;
	}

	public void setZjt(JdbcTemplate zjt) {
		this.zjt = zjt;
	}

	public int doUpdateZentaoPwd(String name, String password) {
		String sql = "update zt_user set password='"+password+"' where account = '" +name+"'" ;
		return zjt.update(sql);
	}

	public boolean doInsertZentaoData(List dataList) {
			final List list = dataList;
//			String sql = "insert into a_lytestuser(realname,account,password,company,dept,commiter,gender) values(?,?,?,?,?,?,?) " ;
			String sql = "insert into zt_user(realname,account,password,company,dept,commiter,gender) values(?,?,?,?,?,?,?)";
			
			zjt.batchUpdate(sql, new BatchPreparedStatementSetter() {
				
				public void setValues(PreparedStatement ps, int i) throws SQLException {
					Map m = (Map) list.get(i);
					String realname = (m).get("真实姓名").toString();
					String account = (m).get("用户名").toString();
					String password = (m).get("密码").toString();
					String dept = (m).get("部门").toString();
					String gender = (m).get("性别").toString();
					ps.setString(1, realname);
					ps.setString(2, account);
					ps.setString(3, TemplateUtil.getMd5(password));
					ps.setInt(4, 1);
					ps.setInt(5, Type.GetInt(dept));
					ps.setString(6, "");
					ps.setString(7, gender);
				}
				
				public int getBatchSize() {
					return list.size();
				}
			});
		return true;
	}
	public boolean doInsertZentaoUserGroupData(List dataList) {
		final List list = dataList;
//		String sql = "insert into a_lytestgroup values(?,?,?)";
			String sql = "insert into zt_usergroup values(?,?,?)";
		
		zjt.batchUpdate(sql, new BatchPreparedStatementSetter() {
			
			public void setValues(PreparedStatement ps, int i) throws SQLException {
				Map m = (Map) list.get(i);
				String account = (m).get("用户名").toString();
				ps.setInt(1, 1);
				ps.setString(2, account);
				ps.setInt(3, 2);//目前是研发组
			}
			
			public int getBatchSize() {
				return list.size();
			}
		});
		return true;
	}

	// 插入测试表a_hr数据是否成功
//	public boolean doInsertHrData(final List list) {
//		String sql = "insert into a_hr(username,realname) values(?,?)";
//		
//		zjt.batchUpdate(sql, new BatchPreparedStatementSetter() {
//			
//			public void setValues(PreparedStatement ps, int i) throws SQLException {
//				Map m = (Map) list.get(i);
//				String username = (m).get("用户名").toString();
//				String realname = (m).get("真实姓名").toString();
//				ps.setString(1, username);
//				ps.setString(2, realname);
//			}
//			
//			public int getBatchSize() {
//				return list.size();
//			}
//		});
//		return true;
//	}
}
