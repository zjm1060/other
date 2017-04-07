package bitc.sz.wzEnv.query;

import bitc.sz.wzEnv.util.DateUtil;
import bitc.sz.wzEnv.query.RelationalOperator;
import bitc.sz.wzEnv.query.Type;
import org.apache.commons.lang.StringUtils;
import org.hibernate.criterion.Criterion;
import org.hibernate.criterion.DetachedCriteria;
import org.hibernate.criterion.Restrictions;
import org.hibernate.sql.JoinType;

import java.sql.Date;
import java.util.HashMap;


/**
 * 
 * 标准化条件查询的基本实现。
 * <ul>
 * 两种形式：
 * <li>字段（属性） 关系运算符 值</li>
 * <li>SQL语句（JDBC时应指定关系运算符为 {@link RelationalOperator.JDBCSQL}）</li>
 * </ul>
 * 此类没有公开的构造器，请使用提供的静态方法获得实例
 * 
 * @author Bom Wu
 * 
 */
public class SimpleCondition implements IStandardQuery {

	private String name;
	private Object value;
	private Object[] values;
	private String operator;
	private String valueType;
	private String sql;

	private SimpleCondition() {

	}

	/**
	 * 静态方法，构造实例
	 * 
	 * @param name
	 *            字段/属性名
	 * @param value
	 *            值
	 * @param valueType
	 *            值类型，see {@link Type}
	 * @param operator
	 *            操作符类型，see {@link RelationalOperator}
	 * @return 构造的新实例
	 */
	public static SimpleCondition newInstance(String name, Object value,
			String valueType, String operator) {
		SimpleCondition simpleParam = new SimpleCondition();
		simpleParam.setName(name);
		simpleParam.setValue(value);
		simpleParam.setOperator(operator);
		simpleParam.setValueType(valueType);
		return simpleParam;
	}

	/**
	 * 静态方法，构造实例，用于IN，NOTIN
	 * 
	 * @param name
	 *            字段/属性名
	 * @param values
	 *            值
	 * @param operator
	 *            操作符类型，see {@link RelationalOperator}
	 * @return 构造的新实例
	 */
	public static SimpleCondition newInstance(String name, Object[] values,
			String operator) {
		SimpleCondition simpleParam = new SimpleCondition();
		simpleParam.setName(name);
		simpleParam.setValues(values);
		simpleParam.setOperator(operator);
		return simpleParam;
	}

	/**
	 * 处理连接查询，即条件名称中存在点号的情况 如 project.name
	 * 
	 * @param criteria
	 * @param propertyName
	 * @param aliasSet
	 */
	private void dealJoin(DetachedCriteria criteria, String prefix,
			String propertyName, HashMap<String, DetachedCriteria> aliasSet) {
		if (StringUtils.isEmpty(propertyName)) {
			return;
		}
		int index = propertyName.indexOf('.');
		DetachedCriteria dc = criteria;
		if (index > 0) {
			String alias = propertyName.substring(0, index);
			String key = prefix == null ? alias : prefix + "." + alias;
			if (!aliasSet.containsKey(key)) {
				dc = criteria.createCriteria(alias, alias,
						JoinType.LEFT_OUTER_JOIN);
				aliasSet.put(key, dc);
			} else {
				dc = aliasSet.get(key);
			}
			propertyName = propertyName.substring(index + 1);
			dealJoin(dc, key, propertyName, aliasSet);
		}
	}

	private Criterion getCriterion(DetachedCriteria dc, String propertyName) {
		if (RelationalOperator.EQ.equals(this.operator)) {
			if (Type.DATE.equals(valueType)) {
				Date date = (Date) this.value;
				return Restrictions.between(propertyName,
						DateUtil.clearHMS(date), DateUtil.addFullHMS(date));
			}
			return Restrictions.eq(propertyName, this.value);
		} else if (RelationalOperator.GT.equals(this.operator)) {
			return Restrictions.gt(propertyName, this.value);
		} else if (RelationalOperator.LT.equals(this.operator)) {
			return Restrictions.lt(propertyName, this.value);
		} else if (RelationalOperator.NE.equals(this.operator)) {
			return Restrictions.ne(propertyName, this.value);
		} else if (RelationalOperator.LIKE.equals(this.operator)) {
			return Restrictions.like(propertyName, "%" + this.value.toString()
					+ "%");
		} else if (RelationalOperator.GE.equals(this.operator)) {
			return Restrictions.ge(propertyName, this.value);
		} else if (RelationalOperator.LE.equals(this.operator)) {
			return Restrictions.le(propertyName, this.value);
		} else if (RelationalOperator.ISNULL.equals(this.operator)) {
			return Restrictions.isNull(propertyName);
		} else if (RelationalOperator.ISNOTNULL.equals(this.operator)) {
			return Restrictions.isNotNull(propertyName);
		} else if (RelationalOperator.SQL.equals(this.operator)
				|| RelationalOperator.JDBCSQL.equals(this.operator)) {
			if (StringUtils.isBlank(this.sql)) {
				return null;
			} else if (StringUtils.isEmpty(this.valueType)
					|| StringUtils.isEmpty(this.value.toString())) {
				return Restrictions.sqlRestriction(this.sql);
			}
			return Restrictions.sqlRestriction(this.sql, this.value,
					Type.getHibernateType(this.valueType));
		} else if (RelationalOperator.IN.equals(this.operator)) {
			return Restrictions.in(propertyName, this.values);
		} else if (RelationalOperator.NOTIN.equals(this.operator)) {
			return Restrictions.not(Restrictions.in(propertyName, this.values));
		}
		return null;
	}

	public Criterion getCriterion(DetachedCriteria criteria,
			HashMap<String, DetachedCriteria> aliasSet) {
		String propertyName = this.name;
		if (propertyName.indexOf('.') >= 0) {
			dealJoin(criteria, null, propertyName, aliasSet);
			String[] ps = StringUtils.split(propertyName, '.');
			if (ps.length > 2) {
				propertyName = ps[ps.length - 2] + "." + ps[ps.length - 1];
			}
		}
		return this.getCriterion(criteria, propertyName);
	}

	public DetachedCriteria addToDetachedCriteria(DetachedCriteria criteria,
			HashMap<String, DetachedCriteria> aliasSet) {
		if (RelationalOperator.SQL.equals(this.operator)) {
			Criterion sqlRestriction = Restrictions.sqlRestriction(this.sql,
					this.value, Type.getHibernateType(this.valueType));
			if (name.indexOf('.') > 0) {
				String[] names = StringUtils.split(name, ".");
				criteria.createCriteria(names[0], names[0],
						JoinType.LEFT_OUTER_JOIN).add(sqlRestriction);
			} else {
				criteria.add(sqlRestriction);
			}
		} else {
			Criterion criterion = this.getCriterion(criteria, aliasSet);
			if (criterion != null) {
				criteria.add(criterion);
			}
		}
		return criteria;
	}

	public String getName() {
		return name;
	}

	public void setName(String name) {
		this.name = name;
	}

	public Object getValue() {
		return value;
	}

	public void setValue(Object value) {
		this.value = value;
	}

	public String getOperator() {
		return operator;
	}

	public void setOperator(String operator) {
		this.operator = operator;
	}

	public String getValueType() {
		return valueType;
	}

	public void setValueType(String valueType) {
		this.valueType = valueType;
	}

	public String getSql() {
		return sql;
	}

	public void setSql(String sql) {
		this.sql = sql;
	}

	public Object[] getValues() {
		return values;
	}

	public void setValues(Object[] values) {
		this.values = values;
	}
}
