package com.bitc.jk.xmis.service.impl;

import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

import jxl.Workbook;
import jxl.read.biff.BiffException;
import net.sf.json.JSONArray;
import net.sf.json.JsonConfig;
import net.sf.json.util.CycleDetectionStrategy;

import org.apache.log4j.Logger;

import com.bitc.jk.xmis.dao.BBZFinancyDAO;
import com.bitc.jk.xmis.model.DeviceSheetImportInfo;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.service.BBZFinancyService;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.WebUtil;
import com.bitc.jk.xmis.util.sql.SQLScript_BBZFinancy;
import com.bitc.jk.xmis.web.SpringInit;

public class BBZFinancyServiceImpl implements BBZFinancyService {

	private static Logger logger = Logger
			.getLogger(BBZFinancyServiceImpl.class);

	private BBZFinancyDAO bbzFinancyDAO;

	public BBZFinancyDAO getBbzFinancyDAO() {
		return bbzFinancyDAO;
	}

	public void setBbzFinancyDAO(BBZFinancyDAO bbzFinancyDAO) {
		this.bbzFinancyDAO = bbzFinancyDAO;
	}

	public String getAttachmentList(PageInfo pageInfo,int attID) {

		return bbzFinancyDAO.getAttachmentList(pageInfo,attID).toJsonString();
	}

	public boolean doSaveSalayExcelInfo(int attID, List<String[]> sheetList) {
		for (int i = 0; i < sheetList.size(); i++) {
			String[] sheetInfoArray = sheetList.get(i);

			bbzFinancyDAO.doSaveSalryExcelInfo(attID,
					Type.GetInt(sheetInfoArray[0]), sheetInfoArray[1], 0);// 导入标志默认未导入：0
		}
		return true;
	}

	public void delSheetInfoByAttID(int attID) {
		bbzFinancyDAO.delExcelSheetDate(attID);
		bbzFinancyDAO.delExcelSheetInfo(attID);
		bbzFinancyDAO.delAttachmentInfo(attID);
	}

	public void delYearSheetInfoByAttID(int attID) {
		bbzFinancyDAO.delYearExcelSheetDate(attID);
		bbzFinancyDAO.delExcelSheetInfo(attID);
		bbzFinancyDAO.delAttachmentInfo(attID);
	}
	public String getSalarySheetList(PageInfo pageInfo, int attID) {

		return bbzFinancyDAO.getSalarySheetList(pageInfo, attID).toJsonString();
	}

	public void importSalarySheetList(List<Map> totalSQL) {
		for (int i = 0; i < totalSQL.size(); i++) {
			Map temp = totalSQL.get(i);
			List sqlForSheet = (List) temp.get("sql");
			
			for (int j = 0; j < sqlForSheet.size(); j++) {
				bbzFinancyDAO
						.importSalarySheetList((String) sqlForSheet.get(j));
			}
			bbzFinancyDAO.updateIsImportForSheet((String) temp
					.get("sheet_uuid"));
		}
	}

	public String getSalaryType() {
		List storeList = bbzFinancyDAO.getSalaryType();
		JsonConfig config = new JsonConfig();
		config.setIgnoreDefaultExcludes(false);
		config.setCycleDetectionStrategy(CycleDetectionStrategy.LENIENT);
		return JSONArray.fromObject(storeList, config).toString();
	}

	public String getSheetColumn(String sheet_uuid, String columnIndex) {

		List list = bbzFinancyDAO.getSheetColumn(sheet_uuid, columnIndex);
		Map map = (Map) list.get(0);
		int index = 1;

		int type_id = Type.GetInt(map.get("type_id"));
		String renderer = "";
		if (type_id == 1) {

		}
		String columnString = "";
		for (int i = 1; i <= 60; i++) {
			index = i - 1;
			String str = "";
			if (map.get("column_" + i).toString().length() > 0) {

				String str_1 = "header" + ":'" + map.get("column_" + i) + "'";
				String str_2 = "dataIndex" + ":'" + "column_" + i + "'";

				str = "{" + str_1 + "," + str_2 + "}";
				columnString = columnString + str + ",";

			} else {
				break;
			}
		}
		columnString = "["
				+ columnString.substring(0, columnString.length() - 1) + "]";

		return bbzFinancyDAO.getSheetColumn(sheet_uuid, columnIndex).toString();
	}

	public List getSheetsForSalaryType(int year, int month, int type) {
		return bbzFinancyDAO.getSheetsForSalaryType(year, month, type);
	}

	public String getPersonForOneSalaryType(int year, int month, int type,
			String username)throws Exception {

		return  bbzFinancyDAO.getPersonForOneSalaryType(year, month, type,
				username).toJsonString();
//		return WebUtil.buildStoreForJsonWithoutPageInfo(list);
	}

	@Override
	public String getIncome(int year,int type) {
		return  bbzFinancyDAO.getIncome(year,type).toJsonString();
	}

	public void importSalarySheetList(List totalSQL, int year,int attID,int type ,String sheet_uuid) {

		bbzFinancyDAO.updateIsNotImportForSheet(year,type);
		bbzFinancyDAO.delExcelSheetDataByYear(year,type);  //删除对应年的数据
		bbzFinancyDAO.delExcelSheetDataBySheetUuid(sheet_uuid,type); //删除 对应sheet_uuid的数据
		for (int i = 0; i < totalSQL.size(); i++) {
			Map temp = (Map) totalSQL.get(i);
			List sqlForSheet = (List) temp.get("sql");
			
			for (int j = 0; j < sqlForSheet.size(); j++) {
				bbzFinancyDAO.importSalarySheetList((String) sqlForSheet.get(j));
			}
			bbzFinancyDAO.updateIsImportForSheet((String) temp.get("sheet_uuid"));
		}
	}

	@Override
	public String getBankDeposit(int year, int type) {
		return  bbzFinancyDAO.getBankDeposit(year,type).toJsonString();
	}

	@Override
	public boolean findByYearAndType(int year, int type,String sheet_uuid) {

		return  bbzFinancyDAO.findByYearAndType(year,type,sheet_uuid);
	}

	@Override
	public String getPayList(String iyear, String compareYear) {
		return bbzFinancyDAO.getPayList(iyear,compareYear).toJsonString();
	}

	@Override
	public String getYearList() {
		return bbzFinancyDAO.getYearList().toJsonString();
	}
	@Override
	public String getSalaryYearList() {
		return bbzFinancyDAO.getSalaryYearList().toJsonString();
	}

	@Override
	public String getYearList(int type) {
		return bbzFinancyDAO.getYearList(type).toJsonString();
	}

}
