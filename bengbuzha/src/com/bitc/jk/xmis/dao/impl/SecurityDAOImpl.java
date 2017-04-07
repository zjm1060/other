/**
 * 
 */
package com.bitc.jk.xmis.dao.impl;


import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.List;
import java.util.Map;

import org.apache.log4j.Logger;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.PreparedStatementSetter;
import org.springframework.jdbc.core.RowMapper;

import com.bitc.jk.xmis.dao.SecurityDAO;
import com.bitc.jk.xmis.model.security.BizFunctionObj;
import com.bitc.jk.xmis.model.security.BizMappingObj;
import com.bitc.jk.xmis.model.security.BizModuleObj;
import com.bitc.jk.xmis.model.security.BizSubModuleObj;
import com.bitc.jk.xmis.model.security.NodeInfo;
import com.bitc.jk.xmis.util.DBSchema;
import com.bitc.jk.xmis.util.SQLScript;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.web.action.param.SecurityTreeParam;

/**
 * @author Ted Li
 * 
 *         2011-5-31
 */
public class SecurityDAOImpl implements SecurityDAO {

	private static Logger logger = Logger.getLogger(SecurityDAOImpl.class);

	private JdbcTemplate jt;

	// getters setters
	public JdbcTemplate getJt() {
		return jt;
	}
	
	public void setJt(JdbcTemplate jt) {
		this.jt = jt;
	}

	public List getSecurityTree(SecurityTreeParam param) {
		String sql = null;
		int nodeLevel = param.getNodeLevel();
		int needParams = 0;
		switch (nodeLevel) {
		case SecurityTreeParam.LEVEL_BIZMODULE:
			sql = "select a.业务模块ID as ID,a.业务模块名称 as name,case when b.业务模块ID is null then 0 else COUNT(a.业务模块ID) end as subNum, '2' as level,a.业务模块排位序号 as px"
					+ " from [xmis].[xmis].[sec_业务模块表] as a,sec_业务模块对应表 as b where a.业务模块ID *= b.业务模块ID "
					+ " group by a.业务模块ID,a.业务模块名称,a.业务模块排位序号,b.业务模块ID"
					+ " order by a.业务模块排位序号 asc";
			break;
		case SecurityTreeParam.LEVEL_BIZSUBMODULE:
			sql = "select a.业务子模块ID as ID,a.业务子模块名称 as name,case when c.业务子模块ID is null then 0 else COUNT(a.业务子模块ID) end as subNum, '3' as level,b.业务子模块排列顺序 as px"
					+ " from xmis.sec_业务子模块表 as a ,xmis.sec_业务模块对应表 as b,xmis.sec_业务模块功能对应表 as c "
					+ " where a.业务子模块ID = b.业务子模块ID and a.业务子模块ID *= c.业务子模块ID and b.业务模块ID = ? "
					+ " group by a.业务子模块ID,a.业务子模块名称,b.业务子模块排列顺序,c.业务子模块ID"
					+ " order by b.业务子模块排列顺序 asc";
			needParams = 1;
			break;
		case SecurityTreeParam.LEVEL_BIZFUNCTION:
			sql = "select a.bizFunctionID as ID,a.业务功能名称 as name,case when c.业务功能ID is null then 0 else COUNT(a.业务功能ID) end as subNum,'4' as level, a.业务功能ID as px"
					+ " from xmis.sec_业务功能表 as a,xmis.sec_业务模块功能对应表 as b,xmis.sec_业务功能字段对应表 as c"
					+ " where a.业务功能ID = b.业务功能ID and a.业务功能ID *=c.业务功能ID and b.业务子模块ID = ?"
					+ " group by a.业务功能ID,a.业务功能名称,c.业务功能ID,a.bizFunctionID"
					+ " order by a.业务功能ID asc,a.bizFunctionID";
			needParams = 1;
			break;
		default:
			break;
		}
		Object[] params = null;
		if (needParams > 0) {
			params = new Object[] { param.getNodeId() };
		}
		return jt.queryForList(sql, params);
	}

	public BizModuleObj getBizModuleById(int id) {
		String sql = SQLScript.getBizModuleById;
		List list = jt.queryForList(sql, new Object[] { id });
		BizModuleObj bmo = null;
		if (list != null && list.size() > 0) {
			Map map = (Map) list.get(0);
			bmo = new BizModuleObj();
			bmo.setId(Type.GetInt(map.get(DBSchema.BizModule.BizModuleID)));
			bmo.setName(Type.getString(map
					.get(DBSchema.BizModule.BizModuleName)));
			bmo
					.setUrl(Type.getString(map
							.get(DBSchema.BizModule.BizModuleURL)));
			bmo.setDesc(Type.getString(map
					.get(DBSchema.BizModule.BizModuleDesc)));
			bmo.setSequenceNo(Type.GetInt(map
					.get(DBSchema.BizModule.BizModuleSquenceNo)));
			bmo.setIconCls(Type.getString(map
					.get(DBSchema.BizModule.BizModuleIconCls)));
			bmo.setIdentifies(Type.getString(map
					.get(DBSchema.BizModule.BizModuleIdentifies)));
			bmo.setCommitPerson(Type.GetInt(map
					.get(DBSchema.BizModule.BizModuleCommitPerson)));
			bmo.setCommitPersonName(Type.getString(map
					.get(DBSchema.BizModule.BizModuleCommitPersonName)));
			bmo.setCommitDate(Type.getDate(map
					.get(DBSchema.BizModule.BizModuleCommitDate)));
			bmo.setCommitStatus(Type.GetInt(map
					.get(DBSchema.BizModule.BizModuleCommitStatus)));
			bmo.setCommitStatusName(Type.getString(map
					.get(DBSchema.BizModule.BizModuleCommitStatusName)));
			bmo.setRemark(Type.getString(map.get(DBSchema.BizModule.remark)));
		}
		return bmo;
	}

	public List getBizSubModuleByModuleId(int id) {
		String sql = "select mapping.业务子模块ID,mapping.业务模块ID,submodel.业务子模块名称,mapping.业务子模块排列顺序 from xmis.sec_业务模块对应表 as mapping,xmis.sec_业务子模块表 as submodel"
				+ " where mapping.业务模块ID = ? and mapping.业务子模块ID = submodel.业务子模块ID"
				+ " order by mapping.业务子模块排列顺序";
		return jt.queryForList(sql, new Object[] { id });
	}

	public List getOtherBizModuleList(int id) {
		String sql = "select c.*,d.业务模块名称 " + "into #result" + "from "
				+ " (select a.业务子模块ID,a.业务子模块名称,b.业务模块ID,b.业务子模块排列顺序 "
				+ " from xmis.sec_业务子模块表 as a,xmis.sec_业务模块对应表 as b"
				+ " where a.业务子模块ID *= b.业务子模块ID) as c,xmis.sec_业务模块表 as d"
				+ " where c.业务模块ID *=d.业务模块ID"
				+ " delete from #result where #result.业务模块ID = ?"
				+ " select * from #result order by 业务模块ID,业务子模块排列顺序";
		return jt.queryForList(sql, new Object[] { id });
	}

	public List getAllStatusList() {
		logger.info("-->getAllStatusList");
		String sql = SQLScript.getAllStatusList;
		return jt.queryForList(sql);
	}

	/**
	 * 保存业务功能
	 */
	public int doSaveFunction(final BizFunctionObj obj) {
		logger.info("-->saveFunction");
		String sql = SQLScript.saveFunction;
		final int maxBizFunctionID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.BizFunction.BizFunctionID,
				DBSchema.BizFunction.TableName)) + 1;
		final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.BizFunction.FunctionID,
				DBSchema.BizFunction.TableName)) + 1;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, maxID); // 业务功能
				ps.setString(2, obj.getName()); // 业务功能名称
				ps.setInt(3, maxBizFunctionID); // bizFunctionID
			}
		});
		return maxBizFunctionID;
	}

	/**
	 * 保存模块
	 */
	public int doSaveModule(final BizModuleObj obj) {
		logger.info("-->saveModule");
		String sql = SQLScript.saveModule;
		final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.BizModule.BizModuleID, DBSchema.BizModule.TableName)) + 1;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, maxID); // 模块ID
				ps.setString(2, obj.getName()); // 模块名称
			}
		});
		return maxID;
	}

	/**
	 * 保存业务子模块
	 */
	public int doSaveSubModule(final BizSubModuleObj obj) {
		logger.info("-->saveSubModule");
		String sql = SQLScript.saveSubModule;
		final int maxID = jt.queryForInt(SQLScript.getMaxIDFromTab(
				DBSchema.BizSubModule.BizSubModuleID,
				DBSchema.BizSubModule.TableName)) + 1;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, maxID); // 子模块ID
				ps.setString(2, obj.getName()); // 子模块名称
			}
		});
		return maxID;
	}

	/**
	 * 删除功能
	 */
	public void doDeleteFunction(final BizFunctionObj obj) {
		logger.info("-->deleteFunction : " + obj.getBizFunctionID());

		String sql = SQLScript.deleteFunction;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, obj.getBizFunctionID()); // 功能bizFunctionID
			}
		});
	}

	/**
	 * 删除业务模块
	 */
	public void doDeleteModule(final BizModuleObj obj) {
		logger.info("-->deleteModule");

		String sql = SQLScript.deleteModule;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, obj.getId()); // 模块ID
			}
		});
	}

	/**
	 * 删除业务子模块
	 */
	public void doDeleteSubModule(final BizSubModuleObj obj) {
		logger.info("-->deleteSubModule");

		String sql = SQLScript.deleteSubModule;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, obj.getId()); // 子模块ID
			}
		});
	}

	/**
	 * 保存子模块功能对应关系
	 */
	public void doDeleteSubModuleFuncMappping(final BizMappingObj obj) {
		logger.info("-->deleteSubModuleFuncMappping");
		String sql = SQLScript.deleteSubModuleFunctionMappping;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, obj.getMasterID()); // 业务自模块ID
				ps.setInt(2, obj.getSubID()); // 业务功能ID
			}

		});
	}

	/**
	 * 删除子模块功能对应关系
	 */
	public void doSaveSubModuleFuncMappping(final BizMappingObj obj) {
		logger.info("-->saveSubModuleFuncMappping");
		String sql = SQLScript.saveSubModuleFuncMappping;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, obj.getMasterID()); // 业务子模块ID
				ps.setInt(2, obj.getSubID()); // 业务功能ID
			}
		});
	}

	/**
	 * 修改子模块功能对应关系
	 */
	public void doUpdateSubModuleFuncMappping(final BizMappingObj oldObj,
			final BizMappingObj newObj) {
		logger.info("-->updateSubModuleFuncMappping");
		String sql = SQLScript.updateSubModuleFuncMappping;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, newObj.getMasterID()); // 修改后的业务子模块ID
				ps.setInt(2, oldObj.getMasterID()); // 修改前的业务子模块ID
				ps.setInt(3, newObj.getSubID());    // 待修改业务功能功能ID
			}
		});
	}

	/**
	 * 修改功能名称
	 */
	public void doUpdateFuncName(final BizFunctionObj obj) {
		logger.info("-->updateFuncName");
		String sql = SQLScript.updateFuncName;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setString(1, obj.getName());
				ps.setInt(2, obj.getBizFunctionID());
			}
		});
	}

	/**
	 * 修改模块名称
	 */
	public void doUpdateModuleName(final BizModuleObj obj) {
		logger.info("-->updateModuleName");
		String sql = SQLScript.updateModuleName;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setString(1, obj.getName());
				ps.setInt(2, obj.getId());
			}
		});
	}

	/**
	 * 修改子模块名称
	 */
	public void doUpdateSubModuleName(final BizSubModuleObj obj) {
		logger.info("-->updateSubModuleName");
		String sql = SQLScript.updateSubModuleName;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setString(1, obj.getName());
				ps.setInt(2, obj.getId());
			}
		});
	}

	/**
	 * 通过bizfunctionID 查询功能ID
	 */
	public int getFuncIdByBizFunctionID(int bizFunctionID) {
		logger.info("-->getFuncIdByBizFunctionID");
		String sql = SQLScript.getFuncIdByBizFunctionID;
		return jt.queryForInt(sql, new Object[]{bizFunctionID});
	}

	/**
	 * 保存模块子模块对应关系
	 */
	public void doSaveModuleSubModuleMappping(final BizMappingObj obj) {
		logger.info("-->doSaveModuleSubModuleMappping");
		String sql = SQLScript.saveModuleSubModuleMappping;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, obj.getMasterID());
				ps.setInt(2, obj.getSubID());
			}
		});
	}

	public void doUpdateModuleSubModuleMappping(final BizMappingObj oldObj,
			final BizMappingObj newObj) {
		logger.info("-->doUpdateModuleSubModuleMappping");
		String sql = SQLScript.updateModuleSubModuleMappping;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, newObj.getMasterID()); // 修改后的业务子模块ID
				ps.setInt(2, oldObj.getMasterID()); // 修改前的业务子模块ID
				ps.setInt(3, newObj.getSubID());    // 待修改业务功能功能ID
			}
		});
	}

	public void doDeleteModuleSubModuleMappping(final BizMappingObj obj) {
		logger.info("-->doDeleteModuleSubModuleMappping");
		String sql = SQLScript.deleteModuleSubModuleMappping;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, obj.getSubID()); // 业务子模块ID
			}
		});
	}

	public List getFuncCountsByBizFunctionID(int bizFunctionID) {
		logger.info("-->getFuncCountsByBizFunctionID");
		String sql = SQLScript.getFuncCountsByBizFunctionID;
		return jt.queryForList(sql, new Object[]{bizFunctionID});
	}

	public void doDeleteSubModuleFuncMappping(final int bizFuncID) {
		logger.info("-->doDeleteSubModuleFuncMappping");
		String sql = SQLScript.deleteSubModuleFuncMappping;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, bizFuncID); // 业务功能ID
			}

		});
	}

	public List getSecurityInfoList() {
		String sql = "\ndeclare @maxA int, @maxPD int \n"+
		 " select @maxA = MAX(角色ID)+1 from xmis.sec_角色 \n"+
		 " select @maxPD = MAX(排序)+1 from xmis.sys_职务 \n"+
			// --中水科技的直属部门下的人员及角色信息 不含员工角色
			" select a.角色名称 , a.角色描述  as 角色说明,d.部门ID,d.部门名称,p.人员ID , rtrim(p.姓氏)+rtrim(p.名字) as 姓名 ,PD.排序 as sortType,a.角色ID as roleID ,c.排序  as deptSort,CASE WHEN s.排序 IS NULL THEN @maxPD ELSE s.排序   END AS positionSort,CASE WHEN a.排序 IS NULL THEN @maxA   WHEN a.排序 = 3 THEN 0 ELSE a.排序  END AS 角色排序" +
			"  from   "+
			" xmis.sec_角色 as a,  "+
			"  xmis.sec_人员角色 as b,  "+
			"   xmis.hr_人员 as p,  "+
			" XMIS.hr_人员部门关系表 pd,  "+
			" XMIS.ORG_部门归属公司表 c,  "+
			" xmis.sys_职务 s,  "+
			" xmis.org_部门 d  "+
			" where a.角色ID = b.角色ID  "+
			" AND b.人员ID = p.人员ID  " +
			" AND p.人员ID>0 "+
			" AND a.显示标志 =1 "+
			" AND a.全体员工标志=0 "+
			" and pd.部门ID = c.部门ID   "+
			" and d.部门ID = pd.部门ID  "+
			" AND  c.公司ID=1 AND c.是否直属=1  "+
			" AND P.人员ID = PD.人员ID  "+
			" AND b.部门级具有的角色标志 is null  "+
			" AND pd.职务*=s.职务ID   AND p.禁用 != 1 "+
			" union  "+
			 // --监控事业部下的人员及角色信息 不含员工
			"  select a.角色名称 , a.角色描述  as 角色说明,d.部门ID,d.部门名称,p.人员ID , rtrim(p.姓氏)+rtrim(p.名字) as 姓名 ,PD.排序  as sortType ,a.角色ID as roleID ,c.排序  as deptSort,CASE WHEN s.排序 IS NULL THEN @maxPD ELSE s.排序 END AS positionSort,CASE WHEN a.排序 IS NULL THEN @maxA  WHEN a.排序 = 3 THEN 0  ELSE a.排序  END AS 角色排序"+
			" FROM  "+
			" xmis.sec_角色 as a,  "+
			" xmis.sec_人员角色 as b,  "+
			" xmis.hr_人员 as p,  " +
			" XMIS.hr_人员部门关系表 pd,  "+
			" XMIS.ORG_部门归属部门表 dd,  "+
			" xmis.org_部门归属公司表    c ,  "+
			" xmis.sys_职务 s,  "+
			" xmis.org_部门 d  "+
			" where a.角色ID = b.角色ID   "+
			" AND b.人员ID = p.人员ID  "+
			" AND a.显示标志 =1 "+
			"  AND a.全体员工标志=0 "+
			" and pd.部门ID = dd.部门ID   "+
			" and dd.部门ID = pd.部门ID  "+
			" and dd.父部门ID = d.部门ID  "+
			" and p.人员ID=PD.人员ID  "+
			"AND p.人员ID>0"+
			" AND  dd.父部门ID=1  " +
			" and dd.父部门ID = c.部门ID " +
			"  AND b.部门级具有的角色标志 IS NULL " +
			" AND pd.职务*=s.职务ID  AND p.禁用 != 1 " +
			//所有员工的
			" UNION " +
			" SELECT a.角色名称 ,a.角色描述 AS 角色说明 ,null ,null , NULL ,'所有员工' ,CASE WHEN a.排序 IS NULL THEN @maxA   WHEN a.排序 = 3 THEN 0 ELSE a.排序  END AS 角色排序, 10000 ,@maxPD AS positionSort,a.角色ID AS roleID ,10000" +
			" FROM   xmis.sec_角色 AS a ,xmis.sec_人员角色 AS b ,xmis.hr_人员 AS p , XMIS.hr_人员部门关系表 pd ,XMIS.ORG_部门归属公司表 c ,xmis.org_部门 d " +
			"  WHERE  a.角色ID = b.角色ID " +
			" AND b.人员ID = p.人员ID " +
			" AND a.显示标志 =1 " +
			"  AND a.全体员工标志=1 " +
			" and pd.部门ID = c.部门ID   "+
			" and d.部门ID = pd.部门ID  "+
			" AND  c.公司ID=1 AND c.是否直属=1  "+
			" AND P.人员ID = PD.人员ID   AND p.禁用 != 1 "+
			
			//部门级的所有员工
			" UNION " +
			" select a.角色名称 , a.角色描述  as 角色说明,d.部门ID,d.部门名称,'' AS 人员ID , '所有员工' AS 姓名 ,PD.排序 as sortType,a.角色ID as roleID ,c.排序  as deptSort,@maxPD AS positionSort,CASE WHEN a.排序 IS NULL THEN @maxA  WHEN a.排序 = 3 THEN 0 ELSE a.排序  END AS 角色排序" +
			"  from   "+
			" xmis.sec_角色 as a,  "+
			"  xmis.sec_人员角色 as b,  "+
			"   xmis.hr_人员 as p,  "+
			" XMIS.hr_人员部门关系表 pd,  "+
			" XMIS.ORG_部门归属公司表 c,  "+
			" xmis.org_部门 d  "+
			" where a.角色ID = b.角色ID  "+
			" AND b.人员ID = p.人员ID  " +
			" AND p.人员ID>0 "+
			" AND a.显示标志 =1 "+
			" AND a.全体员工标志=0 "+
			" and pd.部门ID = c.部门ID   "+
			" and d.部门ID = pd.部门ID  "+
			" AND  c.公司ID=1 AND c.是否直属=1  "+
			" AND P.人员ID = PD.人员ID  "+
			" AND b.部门级具有的角色标志 =1   AND p.禁用 != 1 "+
			" union  "+
			 // --监控事业部下  所有员工
			"  select a.角色名称 , a.角色描述  as 角色说明,d.部门ID,d.部门名称,'' as 人员ID , '所有员工' as 姓名 ,PD.排序  as sortType ,a.角色ID as roleID ,c.排序  as deptSort,@maxPD AS positionSort,CASE WHEN a.排序 IS NULL THEN @maxA  WHEN a.排序 = 3 THEN 0 ELSE a.排序  END AS 角色排序"+
			" FROM  "+
			" xmis.sec_角色 as a,  "+
			" xmis.sec_人员角色 as b,  "+
			" xmis.hr_人员 as p,  " +
			" XMIS.hr_人员部门关系表 pd,  "+
			" XMIS.ORG_部门归属部门表 dd,  "+
			" xmis.org_部门归属公司表    c ,  "+
			" xmis.org_部门 d  "+
			" where a.角色ID = b.角色ID   "+
			" AND b.人员ID = p.人员ID  "+
			" AND a.显示标志 =1 "+
			"  AND a.全体员工标志=0 "+
			" and pd.部门ID = dd.部门ID   "+
			" and dd.部门ID = pd.部门ID  "+
			" and dd.父部门ID = d.部门ID  "+
			" and p.人员ID=PD.人员ID  "+
			"AND p.人员ID>0"+
			" AND  dd.父部门ID=1  " +
			" and dd.父部门ID = c.部门ID " +
			"  AND b.部门级具有的角色标志 =1  AND p.禁用 != 1 " +
			" ORDER BY 角色排序,deptSort,positionSort,sortType ";
		
			return jt.query(sql, new RowMapper(){

				public Object mapRow(ResultSet rs, int arg1)
						throws SQLException {
					NodeInfo[] nodes = new NodeInfo[3];
					nodes[0] = new NodeInfo(rs.getString(1),rs.getString(2));
					nodes[1] = new NodeInfo(rs.getString(4));
					nodes[2] = new NodeInfo(rs.getString(6));
					
					return nodes;
				}
				
			});
		
	}

	@Override
	public List getSecurityRoles(String queryStr, String ordersql) {
		// TODO Auto-generated method stub
		String sql =SQLScript.getSecurityRolesSql;
		if (queryStr != null && !"".equals(queryStr)) {
			queryStr = "'%" + queryStr + "%'";
			String sqlend = "\n and 角色名称 like  " + queryStr + "  \n"
					+ "or 角色描述 like    " + queryStr
					+ "  \n";
			String sqlresult = sql + sqlend + ordersql;
			return jt.queryForList(sqlresult);
		} else {
			return jt.queryForList(sql + ordersql);
		}
	}
	

}
