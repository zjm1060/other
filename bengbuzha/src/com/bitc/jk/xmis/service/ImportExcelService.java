package com.bitc.jk.xmis.service;

import java.util.List;
import java.util.Map;

import com.bitc.jk.xmis.model.DeviceSheetImportInfo;

public interface ImportExcelService {

	public int doSaveDeviceList(List<Map<String,String>>dataList, DeviceSheetImportInfo sheetInfo  );

	public boolean doSaveDeviceSheetImportInfo(int attID, List<String[]> sheetList);

	public List<String[]> getSheetInfos(String path);

	public void delSheetInfoByAttID(int attID);


}
