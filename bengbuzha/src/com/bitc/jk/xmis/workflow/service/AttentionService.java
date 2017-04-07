package com.bitc.jk.xmis.workflow.service;

import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.workflow.model.Attention;

public interface AttentionService {

	Attention getAttentionById(String id);

	Attention findAttentionByExample(Attention attention);

	void doPayAttention(Attention attention, boolean isAttention);

	String getAttentionListByQuery(StandardQuery query, PageInfo pageInfo);
}
