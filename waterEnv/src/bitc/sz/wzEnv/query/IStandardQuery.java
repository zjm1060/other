package bitc.sz.wzEnv.query;

import java.util.HashMap;

import org.hibernate.criterion.Criterion;
import org.hibernate.criterion.DetachedCriteria;


/**
 * 标准化的动态多条件查询，有两个基本的实现：
 * <ul>
 * <li> {@link SimpleCondition} 简单的条件，最基本的组成元素</li>
 * <li>{@link CompositeCondition} 复杂的组合条件，可以由自身与 {@link SimpleCondition}组合</li>
 * </ul>
 * 
 * @author Bom Wu
 * 
 */
public interface IStandardQuery {

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

}
