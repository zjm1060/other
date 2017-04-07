package com.bitc.jk.xmis.util;

import java.sql.Date;
import java.text.NumberFormat;
import java.util.Iterator;
import java.util.List;

import org.apache.commons.lang.StringUtils;
import org.hibernate.Criteria;
import org.hibernate.criterion.CriteriaSpecification;
import org.hibernate.criterion.Criterion;
import org.hibernate.criterion.DetachedCriteria;
import org.hibernate.criterion.Disjunction;
import org.hibernate.criterion.Restrictions;

import com.bitc.jk.xmis.model.ConditionItem;
import com.bitc.jk.xmis.model.ConditionMap;
import com.bitc.jk.xmis.model.sys.RelationalOperator;

public class QueryParam {

	private String name;
	private Object value;
	private ConditionMap condition;

	public QueryParam() {
	}

	public QueryParam(String name, Object value) {
		super();
		this.name = name;
		this.value = value;
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

	public ConditionMap getCondition() {
		return condition;
	}

	public void setCondition(ConditionMap condition) {
		this.condition = condition;
	}

	public String getJdbcSQL() {
		if (this.condition == null
				|| StringUtils.isEmpty(this.condition.getSql())) {
			return null;
		}
		if (this.condition.getSql().indexOf("%?%") > -1) {
			return StringUtils.replace(this.condition.getSql(), "?",
					value.toString());
		}
		return StringUtils.replace(this.condition.getSql(), "?",
				"'" + value.toString() + "'");
	}

	public DetachedCriteria addToDetachedCriteria(DetachedCriteria criteria) {
		String op = this.getCondition().getOperator().getOperator().trim();
		if (RelationalOperator.EQ.equals(op)) {
			if (Type.DATE.equals(this.getCondition().getConditionItem()
					.getType())) {
				Date date = (Date) this.getValue();
				return criteria.add(Restrictions.between(this.getCondition()
						.getConditionItem().getName(), DateUtil.clearHMS(date),
						DateUtil.addFullHMS(date)));
			}
			return criteria.add(Restrictions.eq(this.getCondition()
					.getConditionItem().getName(), this.getValue()));
		} else if (RelationalOperator.GT.equals(op)) {
			return criteria.add(Restrictions.gt(this.getCondition()
					.getConditionItem().getName(), this.getValue()));
		} else if (RelationalOperator.LT.equals(op)) {
			return criteria.add(Restrictions.lt(this.getCondition()
					.getConditionItem().getName(), this.getValue()));
		} else if (RelationalOperator.NE.equals(op)) {
			return criteria.add(Restrictions.ne(this.getCondition()
					.getConditionItem().getName(), this.getValue()));
		} else if (RelationalOperator.LIKE.equals(op)) {
			return criteria.add(Restrictions.like(this.getCondition()
					.getConditionItem().getName(), "%"
					+ this.getValue().toString() + "%"));
		} else if (RelationalOperator.GE.equals(op)) {
			return criteria.add(Restrictions.ge(this.getCondition()
					.getConditionItem().getName(), this.getValue()));
		} else if (RelationalOperator.LE.equals(op)) {
			return criteria.add(Restrictions.le(this.getCondition()
					.getConditionItem().getName(), this.getValue()));
		} else if (RelationalOperator.SQL.equals(op)) {
			String name = this.getCondition().getConditionItem().getName();
			Criterion sqlRestriction = Restrictions.sqlRestriction(this
					.getCondition().getSql(), this.getValue(), Type
					.getHibernateType(this.getCondition().getConditionItem()
							.getType()));
			if (name.indexOf('.') > 0) {
				String[] names = StringUtils.split(name, ".");
				criteria.createCriteria(names[0], names[0],
						CriteriaSpecification.LEFT_JOIN).add(sqlRestriction);
			} else {
				criteria.add(sqlRestriction);
			}
		}
		return criteria;
	}

	public Criterion createCriterion() {
		String op = this.getCondition().getOperator().getOperator().trim();
		if (RelationalOperator.EQ.equals(op)) {
			if (Type.DATE.equals(this.getCondition().getConditionItem()
					.getType())) {
				Date date = (Date) this.getValue();
				return Restrictions.between(this.getCondition()
						.getConditionItem().getName(), DateUtil.clearHMS(date),
						DateUtil.addFullHMS(date));
			}
			return Restrictions.eq(this.getCondition().getConditionItem()
					.getName(), this.getValue());
		} else if (RelationalOperator.GT.equals(op)) {
			return Restrictions.gt(this.getCondition().getConditionItem()
					.getName(), this.getValue());
		} else if (RelationalOperator.LT.equals(op)) {
			return Restrictions.lt(this.getCondition().getConditionItem()
					.getName(), this.getValue());
		} else if (RelationalOperator.NE.equals(op)) {
			return Restrictions.ne(this.getCondition().getConditionItem()
					.getName(), this.getValue());
		} else if (RelationalOperator.LIKE.equals(op)) {
			return Restrictions.like(this.getCondition().getConditionItem()
					.getName(), "%" + this.getValue().toString() + "%");
		} else if (RelationalOperator.GE.equals(op)) {
			return Restrictions.ge(this.getCondition().getConditionItem()
					.getName(), this.getValue());
		} else if (RelationalOperator.LE.equals(op)) {
			return Restrictions.le(this.getCondition().getConditionItem()
					.getName(), this.getValue());
		}
		return null;
	}

	/**
	 * 快速构造一个QueryParam实例，用于Hibernate 条件{@link Criteria} 查询
	 * 
	 * @param name
	 *            条件名称
	 * @param value
	 *            参数值
	 * @param operator
	 *            条件运算符 see{@link RelationalOperator}
	 * @param type
	 *            值类型 see {@link Type}
	 * @return
	 */
	public static QueryParam buildQueryParam(String name, String value,
			String operator, String type) {

		ConditionMap map = new ConditionMap();

		ConditionItem item = new ConditionItem();
		item.setName(name);
		item.setType(type);
		map.setConditionItem(item);

		RelationalOperator op = new RelationalOperator();
		op.setOperator(operator);
		map.setOperator(op);

		QueryParam param = new QueryParam();
		param.setName(name);
		Object val = Type.get(type, value);
		param.setValue(val);
		param.setCondition(map);
		return param;
	}

	/**
	 * 快速构造一个QueryParam实例，用于JDBC SQL查询
	 * 
	 * @param sql
	 * @param value
	 * @return
	 */
	public static QueryParam buildQueryParam(String sql, String value) {
		ConditionMap map = new ConditionMap();
		map.setSql(sql);
		QueryParam param = new QueryParam();
		param.setValue(value);
		param.setCondition(map);
		return param;
	}

	/**
	 * @param list
	 *            {@link QueryParam}s
	 * @param logicalOperator
	 *            "and" 或者 "or"
	 * @return
	 */
	public static String getJdbcSql(List<QueryParam> list,
			String logicalOperator) {
		if (list.size() == 0) {
			return null;
		}
		if (StringUtils.isEmpty(logicalOperator)) {
			logicalOperator = "or";
		}
		StringBuilder sql = new StringBuilder(" where ");
		for (int i = 0; i < list.size(); i++) {
			QueryParam param = list.get(i);
			String op = param.getCondition().getOperator().getOperator();
			sql.append(param.name).append(" ").append(op).append(" '");
			if (op.equals(RelationalOperator.LIKE)) {
				sql.append("%").append(param.getValue().toString()).append("%");
			} else {
				Object value = param.getValue();
				if (value instanceof Double || value instanceof Float) {
					NumberFormat format = NumberFormat.getNumberInstance();
					format.setGroupingUsed(false);
					sql.append(format.format(value));
				} else {
					sql.append(value.toString());
				}
			}
			sql.append("'");
			if (i < list.size() - 1) {
				sql.append(" ").append(logicalOperator).append(" ");
			}
		}
		return sql.toString();
	}

	public static Criterion getRestrictionsOr(List<QueryParam> list) {
		Disjunction disjunction = Restrictions.disjunction();
		for (Iterator<QueryParam> it = list.iterator(); it.hasNext();) {
			Criterion criterion = it.next().createCriterion();
			if (criterion != null) {
				disjunction.add(criterion);
			}
		}
		return disjunction;
	}

}
