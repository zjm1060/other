package com.bitc.jk.xmis.workflow.dao;

import java.util.List;

import com.bitc.jk.xmis.dao.impl.BaseDAOImpl;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.sys.RelationalOperator;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.sql.SimpleParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;
import com.bitc.jk.xmis.workflow.model.Attention;

public class AttentionDAO extends BaseDAOImpl {
	public void saveAttention(Attention attention) {
		ht.saveOrUpdate(attention);
	}

	public List<Attention> findAttetionsByUser(String userId) {
		StandardQuery query = SimpleParam.buildSimpleParam("userID", userId,
				Type.STRING, RelationalOperator.EQ);
		PageInfo info = new PageInfo();
		info.setSort("createTime");
		info.setDir(PageInfo.DESC);
		return getListStore(query, info, Attention.class);
	}
}
