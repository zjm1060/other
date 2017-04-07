package com.bitc.jk.xmis.dao;

import com.bitc.jk.xmis.model.Condition;
import com.bitc.jk.xmis.model.ConditionMap;

public interface ConditionDAO {
	
	public Condition getCondition(int id);

	public Condition saveCondition(Condition condition);

	public void deleteCondition(int id);

	public ConditionMap getConditionMap(int id);
}
