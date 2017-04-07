package com.bitc.jk.xmis.service.impl;

import java.util.List;

import com.bitc.jk.xmis.dao.ConditionDAO;
import com.bitc.jk.xmis.dao.SysDAO;
import com.bitc.jk.xmis.model.Condition;
import com.bitc.jk.xmis.model.ConditionMap;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.sys.Business;
import com.bitc.jk.xmis.model.sys.Currency;
import com.bitc.jk.xmis.model.sys.Draft;
import com.bitc.jk.xmis.model.sys.MaterialsProperty;
import com.bitc.jk.xmis.model.sys.MaterialsUse;
import com.bitc.jk.xmis.model.sys.Power;
import com.bitc.jk.xmis.model.sys.ProduceStatus;
import com.bitc.jk.xmis.model.sys.Weight;
import com.bitc.jk.xmis.service.SysService;
import com.bitc.jk.xmis.util.WebUtil;

/**
 * @author Bom Wu
 * 
 */
public class SysServiceImpl implements SysService {

	private ConditionDAO conditionDAO;
	private SysDAO sysDAO;

	public Condition getCondition(int id) {
		return conditionDAO.getCondition(id);
	}

	public ConditionMap getConditionMap(int id) {
		return conditionDAO.getConditionMap(id);
	}

	public String getCurrencyStore() {
		List<Currency> lists = sysDAO.getCurrency();
		String store = WebUtil.buildJsonStoreByList(lists);
		return store;
	}
	/**
	 * 功率单位
	 * */
	public String getPowerStore() {
		List<Power> lists = sysDAO.getPower();
		String store = WebUtil.buildJsonStoreByList(lists);
		return store;
	}
	/**
	 * 重量单位
	 * */
	public String getWeightStore() {
		List<Weight> lists = sysDAO.getWeight();
		String store = WebUtil.buildJsonStoreByList(lists);
		return store;
	}
	public ConditionDAO getConditionDAO() {
		return conditionDAO;
	}

	public void setConditionDAO(ConditionDAO conditionDAO) {
		this.conditionDAO = conditionDAO;
	}

	public SysDAO getSysDAO() {
		return sysDAO;
	}

	public void setSysDAO(SysDAO sysDAO) {
		this.sysDAO = sysDAO;
	}

	public void doSaveDraft(Draft draft) {
		sysDAO.saveDraft(draft);
	}

	public String getDraftsByExample(Draft draft, PageInfo info) {
		List<Draft> list = sysDAO.getDraftsByExample(draft, info);
		return WebUtil.buildJsonStoreByList(list);
	}

	public Draft getDraftByModelAndAppId(String model, String appId) {
		Draft draft = new Draft();
		draft.setModel(model);
		draft.setAppId(appId);
		List<Draft> list = sysDAO.getDraftsByExample(draft);
		return list.size() > 0 ? list.get(0) : null;
	}

	public Draft getDraftsById(String draftId) {
		return sysDAO.getDraft(draftId);
	}

	public void doDeleteDraft(String draftId) {
		Draft draft = new Draft();
		draft.setId(draftId);
		sysDAO.deleteDraft(draft);
	}

	public String getMaterialsProperty() {
		List<MaterialsProperty> lists = sysDAO.getMaterialsProperty();
		String store = WebUtil.buildJsonStoreByList(lists);
		return store;
	}

	public String getBusiness() {
		List<Business> lists = sysDAO.getBusiness();
		String store = WebUtil.buildJsonStoreByList(lists);
		return store;
	}

	public String getMaterialsUse() {
		List<MaterialsUse> lists = sysDAO.getMaterialsUse();
		String store = WebUtil.buildJsonStoreByList(lists);
		return store;
	}

	public String getProduceStatus() {
		List<ProduceStatus> lists = sysDAO.getProduceStatus();
		String store = WebUtil.buildJsonStoreByList(lists);
		return store;
	}

	

	

}
