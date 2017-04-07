package com.bitc.jk.xmis.util.sql;

import java.util.HashMap;

import org.hibernate.criterion.Criterion;
import org.hibernate.criterion.DetachedCriteria;

import com.bitc.jk.xmis.util.SQLGenerator;

/**
 * 标准化的动态多条件查询，有两个基本的实现：
 * <ul>
 * <li> {@link SimpleParam} 简单的条件，最基本的组成元素</li>
 * <li>{@link CompositParam} 复杂的组合条件，可以由自身与 {@link SimpleParam}组合</li>
 * </ul>
 * 
 * @author Bom Wu
 * 
 */
public interface StandardQuery {

	/**
	 * @return 条件的JDBCSQL表示 <i>不包含 where 关键字</i>
	 */
	public String getJdbcSQL();

	/**
	 * @param criteria
	 * @param aliasSet
	 *            用于记录所有的Alias，避免重复创建Alias
	 * @return
	 */
	public Criterion getCriterion(DetachedCriteria criteria,
			HashMap<String, DetachedCriteria> aliasSet);

	/**
	 * @param criteria
	 * @param aliasSet
	 *            用于记录所有的Alias，避免重复创建Alias
	 * @return
	 */
	public DetachedCriteria addToDetachedCriteria(DetachedCriteria criteria,
			HashMap<String, DetachedCriteria> aliasSet);

	public void addToSQLGenerator(SQLGenerator generator);

}
