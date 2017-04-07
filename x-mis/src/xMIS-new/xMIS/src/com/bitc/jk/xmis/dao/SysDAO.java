package com.bitc.jk.xmis.dao;

import java.util.List;

import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.sys.Business;
import com.bitc.jk.xmis.model.sys.Currency;
import com.bitc.jk.xmis.model.sys.Draft;
import com.bitc.jk.xmis.model.sys.MaterialsProperty;
import com.bitc.jk.xmis.model.sys.MaterialsUse;
import com.bitc.jk.xmis.model.sys.Power;
import com.bitc.jk.xmis.model.sys.ProduceStatus;
import com.bitc.jk.xmis.model.sys.Weight;

public interface SysDAO {

	List<Currency> getCurrency();

	void saveDraft(Draft draft);

	List<Draft> getDraftsByExample(Draft draft, PageInfo info);

	Draft getDraft(String draftId);

	List<Draft> getDraftsByExample(Draft draft);

	void deleteDraft(Draft draft);

	List<Power> getPower();

	List<Weight> getWeight();

	List<Business> getBusiness();

	List<MaterialsProperty> getMaterialsProperty();

	List<MaterialsUse> getMaterialsUse();

	List<ProduceStatus> getProduceStatus();



}
