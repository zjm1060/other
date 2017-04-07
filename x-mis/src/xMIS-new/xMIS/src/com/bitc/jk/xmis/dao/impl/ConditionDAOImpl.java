package com.bitc.jk.xmis.dao.impl;

import org.springframework.orm.hibernate3.HibernateTemplate;

import com.bitc.jk.xmis.dao.ConditionDAO;
import com.bitc.jk.xmis.model.Condition;
import com.bitc.jk.xmis.model.ConditionMap;

public class ConditionDAOImpl implements ConditionDAO {
	private HibernateTemplate ht;

	public Condition getCondition(int id) {
		return (Condition) ht.get(Condition.class, id);
	}

	public Condition saveCondition(Condition condition) {
		ht.save(condition);
		return condition;
	}

	public void deleteCondition(int id) {
		Condition condition = new Condition();
		condition.setId(id);
		ht.delete(condition);
	}

	public HibernateTemplate getHt() {
		return ht;
	}

	public void setHt(HibernateTemplate ht) {
		this.ht = ht;
	}

	public ConditionMap getConditionMap(int id) {
		return (ConditionMap) ht.load(ConditionMap.class, id);
	}

}
