package com.bitc.jk.xmis.dao;

import java.util.List;

import com.bitc.jk.xmis.model.AttachmentFile;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.QueryParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public interface KZHBDAO {
	
	public List getAttchedFilesListStore(List<QueryParam> params, PageInfo pageInfo);	
	
	public int getAttchedFilesListCount(List<QueryParam> params);
	
	public void saveAttachedFile(AttachmentFile attFile);
	
	public List findAttFileByFileName(AttachmentFile attFile);
	
	public void replaceAttachedFile(AttachmentFile attFile);
	
	public void doDeleteAttachment(AttachmentFile attFile);
	
	public AttachmentFile getAttachmentObjByID(int attID);

	public List getQualifiedSuppliers(StandardQuery query, PageInfo pageInfo)throws Exception;

	public List getQualificationList(StandardQuery query, PageInfo pageInfo)throws Exception;

	public List getPatentInfoList(StandardQuery query, PageInfo pageInfo);

}
