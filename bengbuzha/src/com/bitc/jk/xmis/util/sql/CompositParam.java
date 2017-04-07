package com.bitc.jk.xmis.util.sql;

import java.beans.PropertyDescriptor;
import java.lang.reflect.InvocationTargetException;
import java.util.ArrayList;
import java.util.Collection;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Set;

import org.apache.commons.beanutils.BeanUtilsBean;
import org.apache.commons.beanutils.PropertyUtilsBean;
import org.apache.commons.lang.StringUtils;
import org.hibernate.criterion.Criterion;
import org.hibernate.criterion.DetachedCriteria;
import org.hibernate.criterion.Junction;
import org.hibernate.criterion.Restrictions;

import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.model.sys.RelationalOperator;
import com.bitc.jk.xmis.util.SQLGenerator;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.workflow.model.WorkItemHistory;

public class CompositParam implements StandardQuery {

	private static String OR = " or ";
	private static String AND = " and ";
	private static String NOT = " not ";

	private List<StandardQuery> paramList;
	private String logicOperator;

	private CompositParam(String logicOperator) {
		this.logicOperator = logicOperator;
		paramList = new ArrayList<StandardQuery>();
	}

	/**
	 * 构造关系为<em>AND</em>的复合条件
	 * 
	 * @return
	 */
	public static CompositParam buildAndCompositParam() {
		return new CompositParam(AND);
	}

	/**
	 * 构造关系为<em>OR</em>的复合条件
	 * 
	 * @return
	 */
	public static CompositParam buildOrCompositParam() {
		return new CompositParam(OR);
	}

	/**
	 * 构造关系为<em>NOT</em>的复合条件。只能有一个子条件
	 * 
	 * @return
	 */
	public static CompositParam buildNotCompositParam() {
		return new CompositParam(NOT);
	}

	/**
	 * 按示例对象生成查询条件。 <br/>
	 * 例如要查询人员，以 姓名=王 AND 性别=女为条件：
	 * 
	 * <pre>
	 * <code>
	 * EmployeeObj example=new EmployeeObj();
	 * example.setLastName("王");
	 * example.setSex(new Sex(1));//Sex ID为1 表示女性
	 * StandardQuery query=CompositParam.buildForExample(example);
	 * List&lt;EmployeeObj&gt; employees=dao.getListStore(query,EmployeeObj.class);
	 * </code>
	 * </pre>
	 * 
	 * @param example
	 *            用于查询的示例对象
	 * @return {@link CompositParam}的对象，关系为 <em>AND</em>
	 */
	public static CompositParam buildForExample(Object example) {
		CompositParam param = new CompositParam(AND);
		param.addStandardQuery(getParams(null, example));
		return param;
	}

	private static List<SimpleParam> getParams(String prefix, Object example) {
		List<SimpleParam> list = new ArrayList<SimpleParam>();
		PropertyUtilsBean propertyUtils = BeanUtilsBean.getInstance()
				.getPropertyUtils();
		PropertyDescriptor[] descs = propertyUtils
				.getPropertyDescriptors(example);
		for (int i = 0; i < descs.length; i++) {
			String name = descs[i].getName();
			if ("class".equals(name)
					|| !propertyUtils.isReadable(example, name)) {
				continue;
			}
			try {
				Object value = propertyUtils.getProperty(example, name);
				if (value == null) {
					continue;
				}
				String eq = RelationalOperator.EQ;
				if (!StringUtils.isEmpty(prefix)) {
					name = prefix + "." + name;
				}
				if (value instanceof String) {
					list.add(SimpleParam.buildSimpleParam(name, value,
							Type.STRING, eq));
				} else if (value instanceof Integer) {
					list.add(SimpleParam.buildSimpleParam(name, value,
							Type.INTEGER, eq));
				} else if (value instanceof Boolean) {
					list.add(SimpleParam.buildSimpleParam(name, value,
							Type.BOOLEAN, eq));
				} else if (value instanceof Double) {
					list.add(SimpleParam.buildSimpleParam(name, value,
							Type.DOUBLE, eq));
				} else if (value instanceof Float) {
					list.add(SimpleParam.buildSimpleParam(name, value,
							Type.FLOAT, eq));
				} else if (value instanceof Date) {
					list.add(SimpleParam.buildSimpleParam(name, value,
							Type.DATE, eq));
				} else {
					list.addAll(getParams(name, value));
				}
			} catch (NoSuchMethodException e) {
			} catch (IllegalAccessException e) {
			} catch (InvocationTargetException e) {
			}
		}
		return list;
	}

	/**
	 * 向此复合条件中添加子条件
	 * 
	 * @param query
	 * @return
	 */
	public CompositParam addStandardQuery(StandardQuery query) {
		if (query != null) {
			this.paramList.add(query);
		}
		return this;
	}

	/**
	 * 向此复合条件中批量添加子条件
	 * 
	 * @param query
	 * @return
	 */
	public CompositParam addStandardQuery(
			Collection<? extends StandardQuery> querys) {
		this.paramList.addAll(querys);
		return this;
	}

	public String getJdbcSQL() {
		if (this.paramList.size() == 0) {
			return null;
		}
		if (NOT.equals(this.logicOperator)) {
			String sql = this.paramList.get(0).getJdbcSQL();
			return this.logicOperator + "(" + sql + ")";
		}
		StringBuilder builder = new StringBuilder("(");
		for (int i = 0; i < this.paramList.size(); i++) {
			StandardQuery query = this.paramList.get(i);
			String sql = query.getJdbcSQL();
			if (StringUtils.isEmpty(sql)) {
				continue;
			}
			builder.append(sql);
			if (i < this.paramList.size() - 1) {
				builder.append(this.logicOperator);
			}
		}
		return builder.append(")").toString();
	}

	public Criterion getCriterion(DetachedCriteria criteria,
			HashMap<String, DetachedCriteria> aliasSet) {
		if (this.paramList.size() == 0) {
			return null;
		}
		Junction junction = null;
		if (AND.equals(this.logicOperator)) {
			junction = Restrictions.conjunction();
		} else if (OR.equals(this.logicOperator)) {
			junction = Restrictions.disjunction();
		} else if (NOT.equals(this.logicOperator)) {
			return Restrictions.not(this.paramList.get(0).getCriterion(
					criteria, aliasSet));
		}
		for (Iterator<StandardQuery> it = this.paramList.iterator(); it
				.hasNext();) {
			StandardQuery query = it.next();
			Criterion criterion = query.getCriterion(criteria, aliasSet);
			if (criterion != null) {
				junction.add(criterion);
			}
		}
		return junction;
	}

	public DetachedCriteria addToDetachedCriteria(DetachedCriteria criteria,
			HashMap<String, DetachedCriteria> aliasSet) {
		Criterion criterion = this.getCriterion(criteria, aliasSet);
		if (criterion != null) {
			return criteria.add(criterion);
		}
		return criteria;
	}

	@Override
	public String toString() {
		return getJdbcSQL();
	}

	public void addToSQLGenerator(SQLGenerator generator) {
		String sql = getJdbcSQL();
		if (StringUtils.isNotEmpty(sql)) {
			generator.addAppendSQL(" where " + sql);
		}
	}

	public static void main(String[] args) {
		WorkItemHistory history = new WorkItemHistory();
		history.setActivityId("actId");
		history.setActor(new SimpleEmployee(1, "wu", false));
		System.out.println(buildForExample(history).getJdbcSQL());
	}
}
