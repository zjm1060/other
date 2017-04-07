package com.bitc.jk.xmis.dao;

import java.util.List;

import com.bitc.jk.xmis.model.AttachmentFile;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public interface FileUploadDownloadDAO {

	public boolean doSaveAttachment(AttachmentFile attFile);
	public int doSaveAttachmentGetAttacheId(final AttachmentFile attFile);

	public List getAttachmentList(String REFID, int attachmentTypeID);

	public void doDeleteAttachementById(int AttachementID);

	public int existValidate(int attachmentTypeID, String REFID, String fileName,int type);
	public int existValidateReturnAttID(int attachmentTypeID, String REFID, String fileName,int type);

	public void doDeleteByConIDFileName(int attchmentTypeID, String REFID,
			String fileName);

	public String getAttachmentByID(final int attachmentID,
			final String directPath);

	public List getAttachmentListForKZHB(List<StandardQuery> querys,
			PageInfo pageInfo);
	
	public void deleteKzhbFileById(int attachementID);
}
