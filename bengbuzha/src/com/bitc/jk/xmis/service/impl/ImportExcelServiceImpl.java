package com.bitc.jk.xmis.service.impl;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import jxl.Workbook;
import jxl.read.biff.BiffException;

import com.bitc.jk.xmis.dao.ImportExcelDAO;
import com.bitc.jk.xmis.model.DeviceSheetImportInfo;
import com.bitc.jk.xmis.service.ImportExcelService;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebKeys;

public class ImportExcelServiceImpl implements ImportExcelService {

	public ImportExcelDAO importExcelDAO;
	

	public ImportExcelDAO getImportExcelDAO() {
		return importExcelDAO;
	}


	public void setImportExcelDAO(ImportExcelDAO importExcelDAO) {
		this.importExcelDAO = importExcelDAO;
	}

	public List<String[]> getSheetInfos(String filePath) {
		List<String[]> sheetList = new ArrayList<String[]>();
		InputStream is = null;
		Workbook wb;
		try {
			is = new FileInputStream(filePath);
			wb = Workbook.getWorkbook(is);
			int sheet_size = wb.getNumberOfSheets();
			for (int i = 0; i < sheet_size; i++) {
				String[] array = new String[2];
				array[0] = Type.getString(i);
				array[1] = wb.getSheet(i).getName();
				sheetList.add(array);
			}
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		} catch (BiffException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();

		} finally {
			try {
				is.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
		}
		return sheetList;
	}
	


	public List<String[]> getSheetInfosWithImportFlag(String filePath,
			int attID) {
		List<String[]>  sheetInfoWithImportflagList = new ArrayList<String[]>();
		
		List<String[]>  sheetInfoList = getSheetInfos(filePath);
		for (int i = 0; i < sheetInfoList.size(); i++) {
			String[] array = new String[3];
			DeviceSheetImportInfo sheetInfoExist = importExcelDAO.getDeviceSheetImportInfoByInfo(attID,sheetInfoList.get(i)[1]);
			array[0]=sheetInfoList.get(i)[0];
			array[1]=sheetInfoList.get(i)[1];
			array[2]=Type.getString(sheetInfoExist.getImportFlag());
			sheetInfoWithImportflagList.add(array);
		}
		return sheetInfoWithImportflagList;
	}
	
	
	public int doSaveDeviceList(List<Map<String,String>> dataList, DeviceSheetImportInfo sheetInfoParams ) {
		DeviceSheetImportInfo sheetInfoExist = importExcelDAO.getDeviceSheetImportInfoByInfo(sheetInfoParams.getFileId(),sheetInfoParams.getSheetName());
		int count =0;
		for(int i=0;i<dataList.size();i++){
			String levelIdFront = formatSheetId(sheetInfoExist.getSheetId());
			count += importExcelDAO.doSaveDeviceList( dataList.get(i) ,sheetInfoExist.getSheetUuid(),levelIdFront );
		}
		sheetInfoExist.setCurrencyType(sheetInfoParams.getCurrencyType());
		sheetInfoExist.setCurrencyUnit(sheetInfoParams.getCurrencyUnit());
		sheetInfoExist.setImportFlag(WebKeys.IMPORT_STATE_SUCCESS);
		importExcelDAO.doSaveDevceSheetImportInfo(sheetInfoExist);
		return count;
	}


	private String formatSheetId(int sheetId) {
		String temp = "";
		String result = Type.getString(sheetId);
		int length =result.length();
		for (int j = 0; j < 3 - length; j++) {
			temp = temp.concat("0");
		}
		return temp.concat(result);
	}


	public boolean doSaveDeviceSheetImportInfo(int attID, List<String[]> sheetList) {
		for(int i=0;i<sheetList.size();i++){
			String[] sheetInfoArray = sheetList.get(i);
			DeviceSheetImportInfo sheetInfo = new DeviceSheetImportInfo();
			sheetInfo.setFileId(attID);
			sheetInfo.setSheetId(Type.GetInt(sheetInfoArray[0])+1);
			sheetInfo.setSheetName(sheetInfoArray[1]);
			sheetInfo.setImportFlag(0);
			sheetInfo.setCurrencyType(0);
			sheetInfo.setCurrencyUnit("");
			importExcelDAO.doSaveDevceSheetImportInfo(sheetInfo);
		}
		return true;
		
	}


	public void delSheetInfoByAttID(int attID) {
		//根据attID,删除已有的sheet
		importExcelDAO.delExistImportInfo(attID);
		importExcelDAO.delExistSheetInfo(attID);
		importExcelDAO.delAttachById(attID);
		
	}


	
}
