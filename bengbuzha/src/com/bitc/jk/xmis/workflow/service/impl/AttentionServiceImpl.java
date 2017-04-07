package com.bitc.jk.xmis.workflow.service.impl;

import java.util.List;

import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.workflow.dao.AttentionDAO;
import com.bitc.jk.xmis.workflow.model.Attention;
import com.bitc.jk.xmis.workflow.service.AttentionService;

public class AttentionServiceImpl implements AttentionService {
	private AttentionDAO attentionDAO;

	public AttentionDAO getAttentionDAO() {
		return attentionDAO;
	}

	public void setAttentionDAO(AttentionDAO attentionDAO) {
		this.attentionDAO = attentionDAO;
	}

	public Attention getAttentionById(String id) {
		return attentionDAO.get(Attention.class, id);
	}

	public Attention findAttentionByExample(Attention attention) {
		List<Attention> list = attentionDAO.findObjsByParam(attention);
		if (list == null) {
			return null;
		}
		if (list.size() == 0) {
			return null;
		}
		return list.get(0);
	}

	public void doPayAttention(Attention attention, boolean isAttention) {
		if (isAttention) {
			attentionDAO.deleteObject(attention);
		} else {
			attention.setId(null);
			attentionDAO.saveObject(attention);
		}
	}

	public String getAttentionListByQuery(StandardQuery query, PageInfo pageInfo) {
		List<Attention> list = attentionDAO.getListStore(query, pageInfo,
				Attention.class);
		return WebUtil.buildJsonStoreByList(list);
	}

}
