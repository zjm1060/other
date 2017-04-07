package com.bitc.jk.xmis.service;

import java.io.FileNotFoundException;
import java.util.List;

import com.bitc.jk.xmis.model.AttachmentFile;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public interface FileUploadDownloadService {

	public boolean doSaveAttachment(AttachmentFile attFile,boolean isDocument);
	public int doSaveAttachmentGetAttacheId(AttachmentFile attFile);

	public String getAttachmentList(String REFID, int attachmentTypeID);

	public void doDeleteAttachementById(int AttachementID);

	public int existValidate(int attachmentTypeID, String REFID, String fileName,int type);
	public int existValidateReturnAttID(int attachmentTypeID, String REFID, String fileName,int type);

	public void doDeleteByConIDFileName(int attchmentTypeID, String REFID,
			String fileName);

	public String getAttachmentByID(int attachmentID, String directPath);
	
	public AttachmentFile getAttachmentByID(int attachmentID);

	public String getAttachmentListForKZHB(List<StandardQuery> querys,
			PageInfo pageInfo);
	
	public void doDeleteKzhbFileById(int attachementID);
	
	public void doUpdateAttachmentContentByID(int attachementID ) throws FileNotFoundException;
}
