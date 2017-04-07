package com.bitc.jk.xmis.service;

import java.util.List;

import com.bitc.jk.xmis.model.AttachmentFile;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.QueryParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public interface KZHBService {
	
	public String getAttchedFilesListStore(List<QueryParam> params, PageInfo pageInfo);
	
	public int doSaveAttachedFile(AttachmentFile attFile) throws Exception;
	
	public void doDeleteAttachment(int attID);

	public String getQualifiedSuppliers(PageInfo pageInfo, StandardQuery query) throws Exception;

	public String getQualificationList(PageInfo pageInfo, StandardQuery query) throws Exception;

	public String getPatentInfoList(PageInfo pageInfo, StandardQuery query);

}
