package bitc.sz.wzEnv.query;

import bitc.sz.wzEnv.query.RelationalOperator;
import bitc.sz.wzEnv.query.Type;
import org.apache.commons.beanutils.BeanUtilsBean;
import org.apache.commons.beanutils.PropertyUtilsBean;
import org.apache.commons.lang.StringUtils;
import org.hibernate.criterion.Criterion;
import org.hibernate.criterion.DetachedCriteria;
import org.hibernate.criterion.Junction;
import org.hibernate.criterion.Restrictions;

import java.beans.PropertyDescriptor;
import java.lang.reflect.InvocationTargetException;
import java.math.BigDecimal;
import java.util.*;


public class CompositeCondition implements IStandardQuery {

	private static String OR = " or ";
	private static String AND = " and ";
	private static String NOT = " not ";

	private List<IStandardQuery> paramList;
	private String logicOperator;

	private CompositeCondition(String logicOperator) {
		this.logicOperator = logicOperator;
		paramList = new ArrayList<IStandardQuery>();
	}

	/**
	 * 构造关系为<em>AND</em>的复合条件
	 * 
	 * @return
	 */
	public static CompositeCondition newAnd() {
		return new CompositeCondition(AND);
	}

	/**
	 * 构造关系为<em>OR</em>的复合条件
	 * 
	 * @return
	 */
	public static CompositeCondition newOr() {
		return new CompositeCondition(OR);
	}

	/**
	 * 构造关系为<em>NOT</em>的复合条件。只能有一个子条件
	 * 
	 * @return
	 */
	public static CompositeCondition newNot() {
		return new CompositeCondition(NOT);
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
	 * @return {@link CompositeCondition}的对象，关系为 <em>AND</em>
	 */
	public static CompositeCondition buildForExample(Object example) {
		CompositeCondition param = new CompositeCondition(AND);
		param.add(getParams(null, example));
		return param;
	}

	private static List<SimpleCondition> getParams(String prefix, Object example) {
		List<SimpleCondition> list = new ArrayList<SimpleCondition>();
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
					list.add(SimpleCondition.newInstance(name, value,
							Type.STRING, eq));
				} else if (value instanceof Integer) {
					list.add(SimpleCondition.newInstance(name, value,
							Type.INTEGER, eq));
				} else if (value instanceof Boolean) {
					list.add(SimpleCondition.newInstance(name, value,
							Type.BOOLEAN, eq));
				} else if (value instanceof BigDecimal) {
					list.add(SimpleCondition.newInstance(name, value,
							Type.BIGDECIMAL, eq));
				} else if (value instanceof Double) {
					list.add(SimpleCondition.newInstance(name, value,
							Type.DOUBLE, eq));
				} else if (value instanceof Float) {
					list.add(SimpleCondition.newInstance(name, value,
							Type.FLOAT, eq));
				} else if (value instanceof Date) {
					list.add(SimpleCondition.newInstance(name, value,
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
	public CompositeCondition add(IStandardQuery query) {
		if (query != null) {
			this.paramList.add(query);
		}
		return this;
	}

	/**
	 * 向此复合条件中批量添加子条件
	 * 
	 * @param querys
	 * @return
	 */
	public CompositeCondition add(
			Collection<? extends IStandardQuery> querys) {
		this.paramList.addAll(querys);
		return this;
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
		for (Iterator<IStandardQuery> it = this.paramList.iterator(); it
				.hasNext();) {
			IStandardQuery query = it.next();
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

}
