package com.bitc.jk.xmis.dao;

import java.util.Map;

import com.bitc.jk.xmis.model.DeviceSheetImportInfo;

public interface ImportExcelDAO {
	public int doSaveDeviceList(Map<String,String>  dataMap, String sheetUuid,String levelIdFront);

	public String doSaveDevceSheetImportInfo(DeviceSheetImportInfo sheetInfo);

	public DeviceSheetImportInfo getDeviceSheetImportInfoByInfo(int fileId,
			String sheetName);

	public void delExistImportInfo(int attID);

	public void delExistSheetInfo(int attID);

	public void delAttachById(int attID);

//	public int getSheetIdByFileId(int attID);
}
