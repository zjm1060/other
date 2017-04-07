package com.bitc.jk.xmis.service;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import com.bitc.jk.xmis.model.DeviceSheetImportInfo;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.Type;

public interface BBZFinancyService {

	public String getAttachmentList(PageInfo pageInfo,int attID);

	public boolean doSaveSalayExcelInfo(int attID, List<String[]> sheetList);

	public void delSheetInfoByAttID(int attID);
	
	public void delYearSheetInfoByAttID(int attID);

	public String getSalarySheetList(PageInfo pageInfo, int attID);

	public void importSalarySheetList(List<Map> totalSQL, int iyear, int attID, int type, String sheet_uuid);

	public void importSalarySheetList(List<Map> totalSQL);
	public String getSalaryType();

	/**
	 * 获取sheet的列头
	 * */
	public String getSheetColumn(String sheet_uuid, String columnIndex);

	/**
	 * 获取指定年份，月份，类型的sheets
	 * 
	 * @return List (sheet_uuid,name_column,salary_column)
	 * */
	public List getSheetsForSalaryType(int year, int month, int type);

	/**
	 * 获取个人指定type的工资store
	 * */
	public String getPersonForOneSalaryType(int year, int month, int type,
			String username) throws Exception;

	public String getIncome(int year, int type);
	
	public String getBankDeposit(int year, int type);

	public boolean findByYearAndType(int i, int type, String sheet_uuid);

	public String getPayList(String iyear, String compareYear);

	public String getYearList();
	
	public String getSalaryYearList();

	public String getYearList(int type);

}
