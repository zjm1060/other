package com.bitc.jk.xmis.model;

public class DeviceSheetImportInfo {

	private String sheetUuid;
	private int fileId;
	private int sheetId;
	private String sheetName;
	private String currencyUnit;
	private int currencyType;
	private int importFlag;
	public String getSheetUuid() {
		return sheetUuid;
	}
	public void setSheetUuid(String sheetUuid) {
		this.sheetUuid = sheetUuid;
	}
	public int getFileId() {
		return fileId;
	}
	public void setFileId(int fileId) {
		this.fileId = fileId;
	}
	public int getSheetId() {
		return sheetId;
	}
	public void setSheetId(int sheetId) {
		this.sheetId = sheetId;
	}
	public String getSheetName() {
		return sheetName;
	}
	public void setSheetName(String sheetName) {
		this.sheetName = sheetName;
	}
	public String getCurrencyUnit() {
		return currencyUnit;
	}
	public void setCurrencyUnit(String currencyUnit) {
		this.currencyUnit = currencyUnit;
	}
	public int getCurrencyType() {
		return currencyType;
	}
	public void setCurrencyType(int currencyType) {
		this.currencyType = currencyType;
	}
	public int getImportFlag() {
		return importFlag;
	}
	public void setImportFlag(int importFlag) {
		this.importFlag = importFlag;
	}
	

}
