package src;

import java.io.InputStream;
import java.io.Serializable;
import java.sql.Date;

public class AttachmentFile implements Serializable {
	
	private int refID;
	private int refType;
	private String refName;
	
	private String fileName;

	private Date uploadDate;

	private int uploadID;

	private int fileSize;

	private byte[] content;
	
	private InputStream fis;
	
	private int attachmentTypeID;
	

	/**
	 * @return the content
	 */
	public byte[] getContent() {
		return content;
	}

	/**
	 * @param content the content to set
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
	 * @param fileName the fileName to set
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
	 * @param fileSize the fileSize to set
	 */
	public void setFileSize(int fileSize) {
		this.fileSize = fileSize;
	}

	/**
	 * @return the uploadDate
	 */
	public Date getUploadDate() {
		return uploadDate;
	}

	/**
	 * @param uploadDate the uploadDate to set
	 */
	public void setUploadDate(Date uploadDate) {
		this.uploadDate = uploadDate;
	}

	/**
	 * @return the uploadID
	 */
	public int getUploadID() {
		return uploadID;
	}

	/**
	 * @param uploadID the uploadID to set
	 */
	public void setUploadID(int uploadID) {
		this.uploadID = uploadID;
	}

	/**
	 * @return the refID
	 */
	public int getRefID() {
		return refID;
	}

	/**
	 * @param refID the refID to set
	 */
	public void setRefID(int refID) {
		this.refID = refID;
	}

	/**
	 * @return the attachmentTypeID
	 */
	public int getAttachmentTypeID() {
		return attachmentTypeID;
	}

	/**
	 * @param attachmentTypeID the attachmentTypeID to set
	 */
	public void setAttachmentTypeID(int attachmentTypeID) {
		this.attachmentTypeID = attachmentTypeID;
	}

	/**
	 * @param fis the fis to set
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

	/**
	 * @return the refName
	 */
	public String getRefName() {
		return refName;
	}

	/**
	 * @param refName the refName to set
	 */
	public void setRefName(String refName) {
		this.refName = refName;
	}

	/**
	 * @return the refType
	 */
	public int getRefType() {
		return refType;
	}

	/**
	 * @param refType the refType to set
	 */
	public void setRefType(int refType) {
		this.refType = refType;
	}

}
