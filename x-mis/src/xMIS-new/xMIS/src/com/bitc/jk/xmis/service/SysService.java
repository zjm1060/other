package com.bitc.jk.xmis.service;

import com.bitc.jk.xmis.model.Condition;
import com.bitc.jk.xmis.model.ConditionMap;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.sys.Draft;

public interface SysService {

	Condition getCondition(int id);

	ConditionMap getConditionMap(int id);

	String getCurrencyStore();

	void doSaveDraft(Draft draft);

	String getDraftsByExample(Draft draft, PageInfo info);

	Draft getDraftByModelAndAppId(String model, String appId);

	Draft getDraftsById(String draftId);

	void doDeleteDraft(String draftId);

	String getPowerStore();

	String getWeightStore();

	String getMaterialsProperty();

	String getBusiness();

	String getMaterialsUse();

	String getProduceStatus();

}
