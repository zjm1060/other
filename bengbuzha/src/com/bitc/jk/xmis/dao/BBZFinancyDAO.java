package com.bitc.jk.xmis.dao;

import java.util.List;

import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.web.response.JsonResponse;
import com.bitc.jk.xmis.web.response.StoreResponse;

public interface BBZFinancyDAO {

	public StoreResponse getAttachmentList(PageInfo pageInfo, int attID);

	public void doSaveSalryExcelInfo(int attID, int sheetIndex,
			String sheetName, int isImport);

	public void delAttachmentInfo(int attID);

	public void delExcelSheetInfo(int attID);

	public StoreResponse getSalarySheetList(PageInfo pageInfo, int attID);

	public void importSalarySheetList(String sql);

	public void updateIsImportForSheet(String sheet_uuid);

	public List getSalaryType();

	/**
	 * 获取sheet的列头
	 * */
	public List getSheetColumn(String sheet_uuid, String columnIndex);

	/**
	 * 获取指定年份，月份，类型的sheets
	 * 
	 * @return List (sheet_uuid,name_column,salary_column)
	 * */
	public List getSheetsForSalaryType(int year, int month, int type);

	/**
	 * 获取个人指定类型的工资store
	 * */
	public StoreResponse getPersonForOneSalaryType(int year, int month, int type,
			String personName)throws Exception;

	public void delExcelSheetDate(int attID);

	public void delYearExcelSheetDate(int attID);

	public JsonResponse getIncome(int year, int type);
	
	public JsonResponse getBankDeposit(int year, int type);

	public void updateIsNotImportForSheet(int attID, int type);

	public void delExcelSheetDataByYear(int year, int type);

	public boolean findByYearAndType(int year, int type, String sheet_uuid);

	public void delExcelSheetDataBySheetUuid(String sheet_uuid, int type);

	public void updateIsNotImportForSheet(String sheet_uuid);

	public JsonResponse getPayList(String iyear, String compareYear);

	public JsonResponse getYearList();

	public JsonResponse getYearList(int type);
	
	public JsonResponse getSalaryYearList();

}
