package com.bitc.jk.xmis.service.impl;

import java.util.ArrayList;
import java.util.Date;
import java.util.Iterator;
import java.util.List;

import org.apache.log4j.Logger;

import com.bitc.jk.xmis.dao.KZHBDAO;
import com.bitc.jk.xmis.model.AttachmentFile;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.SimpleEmployee;
import com.bitc.jk.xmis.service.KZHBService;
import com.bitc.jk.xmis.util.QueryParam;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.exception.DuplicatedException;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public class KZHBServiceImpl implements KZHBService {

	private static Logger logger = Logger.getLogger(ProjectServiceImpl.class);

	private KZHBDAO kzhbDAO;

	public KZHBServiceImpl() {

	}

	public KZHBDAO getKzhbDAO() {
		return kzhbDAO;
	}

	public void setKzhbDAO(KZHBDAO kzhbDAO) {
		this.kzhbDAO = kzhbDAO;
	}

	public String getAttchedFilesListStore(List<QueryParam> params,
			PageInfo pageInfo) {

		logger.info("--->  getAttchedFilesListStore ");

		List<AttachmentFile> list = kzhbDAO.getAttchedFilesListStore(params,
				pageInfo);
		int totalCount = kzhbDAO.getAttchedFilesListCount(params);
		return WebUtil.buildJsonStoreByList(list, totalCount);
	}

	public int doSaveAttachedFile(AttachmentFile attFile) throws Exception {

		logger.info("--->  saveAttachedFile ");

		attFile.setId(kzhbDAO.getAttchedFilesListCount(new ArrayList()) + 1);
		attFile.setUploadDate(new java.sql.Date(new Date().getTime()));

		SimpleEmployee emp = new SimpleEmployee();
		emp.setUserID(attFile.getUploadID());
		attFile.setPersonEmp(emp);
//		attFile.setAttachmentTypeID(WebKeys.Upload_Application_KzhbFile_Type);

		// if(!attFile.getCoerciveFlag() && existAttFileCheck(attFile)>0)
		// throw new DuplicatedException("文件“" + attFile.getFileName() +"”
		// 已存在");
		// else{
		emp.setUserID(attFile.getUploadID());
		kzhbDAO.saveAttachedFile(attFile);
		// }

		return attFile.getId();

	}

	public int doReplaceAttachedFile(AttachmentFile attFile) throws Exception {

		logger.info("--->  doReplaceAttachedFile ");

		attFile.setUploadDate(new java.sql.Date(new Date().getTime()));

		SimpleEmployee emp = new SimpleEmployee();
		emp.setUserID(attFile.getUploadID());
		attFile.setPersonEmp(emp);
//		attFile.setAttachmentTypeID(WebKeys.Upload_Application_KzhbFile_Type);

		emp.setUserID(attFile.getUploadID());
		kzhbDAO.replaceAttachedFile(attFile);

		return attFile.getId();

	}

	public int existAttFileCheck(AttachmentFile attFile) {

		AttachmentFile o = new AttachmentFile(WebKeys.Upload_Application_KzhbFile_Type);
		o.setFileName(attFile.getFileName());

		List<AttachmentFile> list = kzhbDAO.findAttFileByFileName(o);

		for (Iterator<AttachmentFile> it = list.iterator(); it.hasNext();) {

			return it.next().getId();

		}

		return -1;

	}
	
	public void doDeleteAttachment(int attID){
		AttachmentFile file = new AttachmentFile(WebKeys.Upload_Application_KzhbFile_Type);
//		file.setId(attID);
		
		file = kzhbDAO.getAttachmentObjByID(attID);
		
		file.setDeletedFlag(0);
		
		kzhbDAO.doDeleteAttachment(file);
		
	}


	public String getQualifiedSuppliers(PageInfo pageInfo, StandardQuery query) throws Exception {
		List listStore = kzhbDAO.getQualifiedSuppliers(query, pageInfo);
		return WebUtil.buildStoreForJson(listStore, "");
	}

	public String getQualificationList(PageInfo pageInfo, StandardQuery query)throws Exception {
		// TODO Auto-generated method stub
		List listStore = kzhbDAO.getQualificationList(query, pageInfo);
		return WebUtil.buildStoreForJson(listStore, "");
	}

	public String getPatentInfoList(PageInfo pageInfo, StandardQuery query) {
		List listStore = kzhbDAO.getPatentInfoList(query, pageInfo);
		return WebUtil.buildStoreForJson(listStore, "");
	}

}
