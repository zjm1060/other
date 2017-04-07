package com.bitc.jk.xmis.service.impl;

import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.List;

import org.apache.log4j.Logger;
import org.springframework.jdbc.core.PreparedStatementSetter;

import com.bitc.jk.xmis.dao.FileUploadDownloadDAO;
import com.bitc.jk.xmis.model.AttachmentFile;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.service.FileUploadDownloadService;
import com.bitc.jk.xmis.util.DBSchema;
import com.bitc.jk.xmis.util.SQLScript;
import com.bitc.jk.xmis.util.WebKeys;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public class FileUploadDownloadServiceImpl implements FileUploadDownloadService {
	private static Logger logger = Logger
			.getLogger(FileUploadDownloadServiceImpl.class);

	private FileUploadDownloadDAO fileUploadDownloadDAO;

	public FileUploadDownloadDAO getFileUploadDownloadDAO() {
		return fileUploadDownloadDAO;
	}

	public void setFileUploadDownloadDAO(
			FileUploadDownloadDAO fileUploadDownloadDAO) {
		this.fileUploadDownloadDAO = fileUploadDownloadDAO;
	}

	/**
	 * 保存附件
	 * 
	 * @param attFile
	 * @return
	 */
	public int doSaveAttachmentGetAttacheId(AttachmentFile attFile){
		logger.info("---> doSaveAttachment ? fileName = "
				+ attFile.getFileName() + "&REFID = " + attFile.getRefID());
		return fileUploadDownloadDAO.doSaveAttachmentGetAttacheId(attFile);
	}
	public boolean doSaveAttachment(AttachmentFile attFile) {
		logger.info("---> doSaveAttachment ? fileName = "
				+ attFile.getFileName() + "&REFID = " + attFile.getRefID());
		return fileUploadDownloadDAO.doSaveAttachment(attFile);
	}

	/**
	 * 获取附件列表
	 * 
	 * @param REFID
	 * @param attachmentTypeID
	 * @return
	 */
	public String getAttachmentList(String REFID, int attachmentTypeID) {
		logger.info("getAttachmentList---->REFID=" + REFID
				+ "   attachmentTypeID=" + attachmentTypeID);
		List storeList = fileUploadDownloadDAO.getAttachmentList(REFID,
				attachmentTypeID);
		// return WebUtil.buildStoreForJsonWithoutPageInfo(storeList,
		// WebKeys.Action_Method_Name_AttachedList);
		return WebUtil.buildStoreForJsonWithoutPageInfo(storeList);
	}

	/**
	 * 删除附件
	 * 
	 * @param attchmentID
	 * @return
	 */
	public void doDeleteAttachementById(int AttachementID) {
		fileUploadDownloadDAO.doDeleteAttachementById(AttachementID);
	}

	/**
	 * 提示上传文件已存在时(覆盖/取消)
	 * 
	 * @param attchmentTypeID
	 * @param fileName
	 * @return
	 */
	public int existValidate(int attachmentTypeID, String REFID, String fileName,int type) {

		return fileUploadDownloadDAO.existValidate(attachmentTypeID, REFID,
				fileName,type);

	}
	public int existValidateReturnAttID(int attachmentTypeID, String REFID, String fileName,int type) {
		
		return fileUploadDownloadDAO.existValidateReturnAttID(attachmentTypeID, REFID,
				fileName,type);
		
	}

	/**
	 * 上传文件已存在时删除原有的
	 * 
	 * @param attchmentTypeID
	 * @param REFID
	 * @param fileName
	 * @return
	 */
	public void doDeleteByConIDFileName(int attchmentTypeID, String REFID,
			String fileName) {
		fileUploadDownloadDAO.doDeleteByConIDFileName(attchmentTypeID, REFID,
				fileName);
	}

	/**
	 * 获得附件
	 * 
	 * @param attchmentID
	 * @param directPath
	 * @return
	 */
	public String getAttachmentByID(int attachmentID, String directPath) {

		return fileUploadDownloadDAO
				.getAttachmentByID(attachmentID, directPath);

	}

	/**
	 * 获取附件列表
	 * 
	 * @param REFID
	 * @param attachmentTypeID
	 * @return
	 */
	public String getAttachmentListForKZHB(List<StandardQuery> querys,
			PageInfo pageInfo) {
		List aList = fileUploadDownloadDAO.getAttachmentListForKZHB(querys,
				pageInfo);
		return (pageInfo != null && pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0) ? WebUtil.buildStoreForJsonWithoutPageInfo(
				aList, WebKeys.AttachmentListForKZHB) : WebUtil
				.buildStoreForJson(aList,
						WebKeys.AttachmentListForKZHB);
	}
	
	/**
	 * 删除科质部 文件    逻辑删除   将删除标志 置2
	 * */
	public void doDeleteKzhbFileById(int attachementID){
		fileUploadDownloadDAO.deleteKzhbFileById(attachementID);
	}
}
