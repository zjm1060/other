package com.bitc.jk.xmis.dao.impl;

import java.util.List;

import com.bitc.jk.xmis.dao.SysDAO;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.sys.Business;
import com.bitc.jk.xmis.model.sys.Currency;
import com.bitc.jk.xmis.model.sys.Degree;
import com.bitc.jk.xmis.model.sys.Draft;
import com.bitc.jk.xmis.model.sys.DutyLevel;
import com.bitc.jk.xmis.model.sys.Echnicalposition;
import com.bitc.jk.xmis.model.sys.IdentityObj;
import com.bitc.jk.xmis.model.sys.InServiceType;
import com.bitc.jk.xmis.model.sys.MaterialsProperty;
import com.bitc.jk.xmis.model.sys.MaterialsUse;
import com.bitc.jk.xmis.model.sys.OfficeSupplies;
import com.bitc.jk.xmis.model.sys.Power;
import com.bitc.jk.xmis.model.sys.ProduceStatus;
import com.bitc.jk.xmis.model.sys.ProfessionalLevel;
import com.bitc.jk.xmis.model.sys.Weight;
import com.bitc.jk.xmis.model.sys.WorkType;

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

		return ht.loadAll(Business.class);
	}

	public List<MaterialsProperty> getMaterialsProperty() {

		return ht.loadAll(MaterialsProperty.class);
	}

	public List<MaterialsUse> getMaterialsUse() {

		return ht.loadAll(MaterialsUse.class);
	}

	public List<ProduceStatus> getProduceStatus() {

		return ht.loadAll(ProduceStatus.class);
	}

	@SuppressWarnings("unchecked")
	public List<DutyLevel> getDutyLevel() {
		return ht.loadAll(DutyLevel.class);
	}
	
	@SuppressWarnings("unchecked")
	public List<Echnicalposition> getEchnicalposition() {
		String hql = "from Echnicalposition order by px";
		return ht.find(hql);
	}
	
	@SuppressWarnings("unchecked")
	public List<ProfessionalLevel> getProfessionalLevel() {
		return ht.loadAll(ProfessionalLevel.class);
	}
	
	@SuppressWarnings("unchecked")
	public List<Degree> getDegree() {
		return ht.loadAll(Degree.class);
	}
	
	@SuppressWarnings("unchecked")
	public List<WorkType> getWorkType() {
		return ht.loadAll(WorkType.class);
	}
	
	@SuppressWarnings("unchecked")
	public List<InServiceType> getInServiceType() {
		return ht.loadAll(InServiceType.class);
	}

	@SuppressWarnings("unchecked")
	public List<IdentityObj> getIdentityObjCombo() {
		return ht.loadAll(IdentityObj.class);
	}

	@SuppressWarnings("unchecked")
	public List<OfficeSupplies> getOfficeSuppliesCombo() {
		// TODO Auto-generated method stub
		return ht.loadAll(OfficeSupplies.class);
	}
	
}
