package com.bitc.jk.xmis.dao.impl;

import java.util.List;


import com.bitc.jk.xmis.dao.SysDAO;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.sys.Business;
import com.bitc.jk.xmis.model.sys.Currency;
import com.bitc.jk.xmis.model.sys.Draft;
import com.bitc.jk.xmis.model.sys.MaterialsProperty;
import com.bitc.jk.xmis.model.sys.MaterialsUse;
import com.bitc.jk.xmis.model.sys.Power;
import com.bitc.jk.xmis.model.sys.ProduceStatus;
import com.bitc.jk.xmis.model.sys.Weight;

public class SysDAOImpl extends BaseDAOImpl implements SysDAO {

	@SuppressWarnings("unchecked")
	public List<Currency> getCurrency() {
		return ht.loadAll(Currency.class);
	}
	public List<Power> getPower() {
		return ht.loadAll(Power.class);
	}
	public List<Weight> getWeight() {
		return ht.loadAll(Weight.class);
	}
	public void saveDraft(Draft draft) {
		saveOrUpdateObject(draft);
	}

	public List<Draft> getDraftsByExample(Draft draft, PageInfo info) {
		return findObjsByParam(draft, info.getStart(), info.getLimit());
	}

	public Draft getDraft(String draftId) {
		return get(Draft.class, draftId);
	}

	public List<Draft> getDraftsByExample(Draft draft) {
		return findObjsByParam(draft);
	}

	public void deleteDraft(Draft draft) {
		deleteObject(draft);
	}
	public List<Business> getBusiness() {
		// TODO Auto-generated method stub
		return ht.loadAll(Business.class);
	}
	public List<MaterialsProperty> getMaterialsProperty() {
		// TODO Auto-generated method stub
		return ht.loadAll(MaterialsProperty.class);
	}
	public List<MaterialsUse> getMaterialsUse() {
		// TODO Auto-generated method stub
		return ht.loadAll(MaterialsUse.class);
	}
	public List<ProduceStatus> getProduceStatus() {
		// TODO Auto-generated method stub
		return ht.loadAll(ProduceStatus.class);
	}
	

	
}
