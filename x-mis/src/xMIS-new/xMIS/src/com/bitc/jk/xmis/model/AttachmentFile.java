package com.bitc.jk.xmis.model;

import java.io.InputStream;
import java.io.Serializable;
import java.sql.Date;

import com.bitc.jk.xmis.util.WebKeys;

public class AttachmentFile implements Serializable {

	/**
	 * 
	 */
	private static final long serialVersionUID = 429186902928639261L;

	private int id;// 附件ID
	private Date uploadDate;// 上传时间
	private int uploadID;// 上传人ID
	private String desc; // 内容描述
	private String fileName;// 文件名
	private byte[] content;// 附件内容
	private int attachmentTypeID;// 附件主体类型ID

	private String refID;// REFID
	private String lastUpdateDate;// 最后修改日期
	private String lastUpdateEmpId;// 最后修改人
	private String keyWords;//关键字
	private String filePath;//文件路径
	private int deletedFlag;//删除标志

	// private int refType;
	// private String refName;

	private int fileSize;// 文件大小

	private InputStream fis;

	private SimpleEmployee personEmp;
	private boolean coerciveFlag;// 1:un-delete; 0:deleted

	public AttachmentFile() {

	}

	public AttachmentFile(int attType) {
		switch (attType) {
		case WebKeys.Upload_Application_KzhbFile_Type:
//			this.attachmentTypeID = attType;
			this.deletedFlag = 1;
			break;
		}

	}

	public boolean getCoerciveFlag() {
		return coerciveFlag;
	}

	public void setCoerciveFlag(boolean coerciveFlag) {
		this.coerciveFlag = coerciveFlag;
	}

	/**
	 * @return the content
	 */
	public byte[] getContent() {
		return content;
	}

	/**
	 * @param content
	 *            the content to set
	 */
	public void setContent(byte[] content) {
		this.content = content;
	}

	/**
	 * @return the fileName
	 */
	public String getFileName() {
		return fileName;
	}

	/**
	 * @param fileName
	 *            the fileName to set
	 */
	public void setFileName(String fileName) {
		this.fileName = fileName;
	}

	/**
	 * @return the fileSize
	 */
	public int getFileSize() {
		return fileSize;
	}

	/**
	 * @param fileSize
	 *            the fileSize to set
	 */
	public void setFileSize(int fileSize) {
		this.fileSize = fileSize;
	}

	/**
	 * @return the uploadID
	 */
	public int getUploadID() {
		return uploadID;
	}

	/**
	 * @param uploadID
	 *            the uploadID to set
	 */
	public void setUploadID(int uploadID) {
		this.uploadID = uploadID;
	}

	/**
	 * @return the refID
	 */
	public String getRefID() {
		return refID;
	}

	/**
	 * @param refID
	 *            the refID to set
	 */
	public void setRefID(String refID) {
		this.refID = refID;
	}

	/**
	 * @return the attachmentTypeID
	 */
	public int getAttachmentTypeID() {
		return attachmentTypeID;
	}

	/**
	 * @param attachmentTypeID
	 *            the attachmentTypeID to set
	 */
	public void setAttachmentTypeID(int attachmentTypeID) {
		this.attachmentTypeID = attachmentTypeID;
	}

	/**
	 * @param fis
	 *            the fis to set
	 */
	public void setFis(InputStream fis) {
		this.fis = fis;
	}

	/**
	 * @return the fis
	 */
	public InputStream getFis() {
		return fis;
	}

	// /**
	// * @return the refName
	// */
	// public String getRefName() {
	// return refName;
	// }
	//
	// /**
	// * @param refName the refName to set
	// */
	// public void setRefName(String refName) {
	// this.refName = refName;
	// }
	//
	// /**
	// * @return the refType
	// */
	// public int getRefType() {
	// return refType;
	// }
	//
	// /**
	// * @param refType the refType to set
	// */
	// public void setRefType(int refType) {
	// this.refType = refType;
	// }

	public int getId() {
		return id;
	}

	public void setId(int id) {
		this.id = id;
	}

	public String getDesc() {
		return desc;
	}

	public void setDesc(String desc) {
		this.desc = desc;
	}

	public String getFilePath() {
		return filePath;
	}

	public void setFilePath(String filePath) {
		this.filePath = filePath;
	}

	public String getKeyWords() {
		return keyWords;
	}

	public void setKeyWords(String keyWords) {
		this.keyWords = keyWords;
	}

	public SimpleEmployee getPersonEmp() {
		return personEmp;
	}

	public void setPersonEmp(SimpleEmployee personEmp) {
		this.personEmp = personEmp;
	}

	public Date getUploadDate() {
		return uploadDate;
	}

	public void setUploadDate(Date uploadDate) {
		this.uploadDate = uploadDate;
	}

	public int getDeletedFlag() {
		return deletedFlag;
	}

	public void setDeletedFlag(int deletedFlag) {
		this.deletedFlag = deletedFlag;
	}

	public String getLastUpdateDate() {
		return lastUpdateDate;
	}

	public void setLastUpdateDate(String lastUpdateDate) {
		this.lastUpdateDate = lastUpdateDate;
	}

	public String getLastUpdateEmpId() {
		return lastUpdateEmpId;
	}

	public void setLastUpdateEmpId(String lastUpdateEmpId) {
		this.lastUpdateEmpId = lastUpdateEmpId;
	}

}
