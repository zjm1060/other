package com.bitc.jk.xmis.dao.impl;

import java.math.BigDecimal;
import java.sql.CallableStatement;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.Collections;
import java.util.Comparator;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.springframework.dao.DataAccessException;
import org.springframework.jdbc.core.CallableStatementCallback;
import org.springframework.jdbc.core.CallableStatementCreator;
import org.springframework.jdbc.core.JdbcTemplate;
import org.springframework.jdbc.core.PreparedStatementSetter;

import com.bitc.jk.xmis.dao.BBZFinancyDAO;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.Type;
import com.bitc.jk.xmis.util.UUIDUtils;
import com.bitc.jk.xmis.util.sql.SQLScript_BBZFinancy;
import com.bitc.jk.xmis.web.response.JsonResponse;
import com.bitc.jk.xmis.web.response.StoreResponse;

public class BBZFinancyDAOImpl implements BBZFinancyDAO {

	private static Logger logger = Logger.getLogger(BBZFinancyDAOImpl.class);

	private JdbcTemplate jt;

	public JdbcTemplate getJt() {
		return jt;
	}

	public void setJt(JdbcTemplate jdbcTemplate) {
		this.jt = jdbcTemplate;
	}

	public List getListWithPageinfo(List list, PageInfo pageInfo) {
		List result = new ArrayList();
		int start = pageInfo.getStart();
		int limit = pageInfo.getLimit();
		int size = list.size();
		int limitSize = start + limit;
		if (limitSize > size) {
			limitSize = size;
		}
		if (limit == 0 || list.size() < limit) {
			return list;
		} else {
			for (int i = start; i < limitSize; i++) {
				// result.add((i - pageInfo.getStart()), list.get(i));
				result.add(list.get(i));
			}
			return result;
		}
	}

	public StoreResponse getAttachmentList(PageInfo pageInfo, int attID) {

		String sql = SQLScript_BBZFinancy.getAttachmentList;
		List list = jt.queryForList(sql, new Object[] { attID });
		List result = getListWithPageinfo(list, pageInfo);
		StoreResponse response = new StoreResponse();
		response.setSuccess(true);
		response.setTotalCount(result.size());
		response.setRows(result);
		return response;
	}

	public void doSaveSalryExcelInfo(final int attID, final int sheetIndex,
			final String sheetName, final int isImport) {
		String sql = SQLScript_BBZFinancy.doSaveSalryExcelInfo;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setString(1, UUIDUtils.nextCode());
				ps.setInt(2, attID);
				ps.setInt(3, sheetIndex);
				ps.setString(4, sheetName);
				ps.setInt(5, isImport);
			}
		});
	}

	public void delAttachmentInfo(final int attID) {
		String sql = SQLScript_BBZFinancy.delAttachmentInfo;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, attID);
			}
		});
	}

	public void delExcelSheetInfo(final int attID) {
		String sql = SQLScript_BBZFinancy.delExcelSheetInfo;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, attID);
			}
		});
	}

	public void delExcelSheetDate(final int attID) {
		String sql = SQLScript_BBZFinancy.delExcelSheetDate;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, attID);
			}
		});
	}

	public void delYearExcelSheetDate(final int attID) {
		String sql = SQLScript_BBZFinancy.delYearExcelSheetDate;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, attID);
			}
		});
	}

	public StoreResponse getSalarySheetList(PageInfo pageInfo, final int attID) {
		String sql = SQLScript_BBZFinancy.getSalarySheetList;
		List list = jt.queryForList(sql, new Object[] { attID });
		List result = new ArrayList();
		int total = list.size();
		if (pageInfo.getLimit() == 0 || list.size() < pageInfo.getLimit()) {
			result = list;
		} else {
			for (int i = pageInfo.getStart(); i < (pageInfo.getStart() + pageInfo
					.getLimit()); i++) {
				result.add((i - pageInfo.getStart()), list.get(i));
			}
		}
		StoreResponse response = new StoreResponse();
		response.setSuccess(true);
		response.setTotalCount(total);
		response.setRows(result);
		return response;
	}

	public void importSalarySheetList(String sql) {
		jt.update(sql);
	}

	public void updateIsImportForSheet(final String sheet_uuid) {
		String sql = SQLScript_BBZFinancy.updateIsImportForSheet;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setString(1, sheet_uuid);
			}
		});
	}

	public List getSalaryType() {
		String sql = SQLScript_BBZFinancy.getSalaryType;
		return jt.queryForList(sql);
	}

	public List getSheetColumn(String sheet_uuid, String columnIndex) {
		String sql = SQLScript_BBZFinancy.getSheetColumn.replace("?*?",
				columnIndex);
		return jt.queryForList(sql, new Object[] { sheet_uuid });
	}

	/* 获取指定年份，月份，类型的sheets */
	public List getSheetsForSalaryType(int year, int month, int type) {
		String sql = SQLScript_BBZFinancy.getSheetsForSalaryType;
		return jt.queryForList(sql, new Object[] { new Integer(year),
				new Integer(month), new Integer(type) });
	}

	public StoreResponse getPersonForOneSalaryType(int year, int month,
			int type, String personName) {

		List list = getSheetsForSalaryType(year, month, type);

		String sheet_uuid = "";
		String name_column = "";

		// 循环遍历所有指定年，月份，类型的工资sheet
		for (int i = 0; i < list.size(); i++) {
			Map map = (Map) list.get(i);
			String sheet_uuid_temp = Type.getString(map.get("sheet_uuid"));
			String name_column_temp = Type.getString(map.get("name_column"));

			// 拿到每个sheet去判断是否存在指定人员
			String sql_temp = SQLScript_BBZFinancy.getIsNotHavePersonForSheet
					.replace("*?*", name_column_temp);
			List tempList = jt.queryForList(sql_temp, new Object[] {
					personName, sheet_uuid_temp });

			if (tempList.size() > 0) {
				sheet_uuid = sheet_uuid_temp;
				name_column = name_column_temp;
				break;
			}
		}

		String sql = SQLScript_BBZFinancy.getSheetsForPersonOneSalaryType
				.replace("*?*", name_column);

		logger.info(sql);
		List result;
		int size = 0;
		try {
			result = jt.queryForList(sql, new Object[] { sheet_uuid,
					new Integer(year), new Integer(month), new Integer(type),
					personName, sheet_uuid, new Integer(year),
					new Integer(month), new Integer(type), });
			size = result.size();
		} catch (Exception e) {
			result = null;
		}

		StoreResponse response = new StoreResponse();
		response.setSuccess(true);
		response.setTotalCount(size);
		response.setRows(result);
		return response;
	}

	/*
	 * 获取指定年份的sheets public List getSheetIncome(int year) { String sql =
	 * SQLScript_BBZFinancy.getSheetsIncome; return jt.queryForList(sql, new
	 * Object[] { new Integer(year) }); }
	 */

	private static boolean isNum(String str) {

		try {
			str = StringUtils.replace(str, ",", "");
			float num = Type.GetFloat(str);// 把字符串强制转换为数字
			return true;// 如果是数字，返回True
		} catch (Exception e) {
			return false;// 如果抛出异常，返回False
		}
	}

	@Override
	public JsonResponse getIncome(int year, int type) {
		String sql = SQLScript_BBZFinancy.getDateIncome;
		logger.info(sql);
		List result = null;
		int size = 0;
		final Map<String, String> month = new HashMap<String, String>();
		try {
			boolean flag = true;
			result = jt.queryForList(sql, new Object[] { new Integer(year),
					new Integer(type), new Integer(year), new Integer(type), });
			if (result.size() > 0) {
				Map totalmap = new HashMap<String, Object>();
				for (int i = 0; i < result.size(); i++) {
					Map<String, Object> temp = (Map<String, Object>) result
							.get(i);
					if("合计".equals(temp.get("column_1"))){
						flag = false;
					}
					month.put((String) temp.get("column_1"),
							((String) temp.get("column_1")).substring(
									0,
									(((String) temp.get("column_1"))
											.indexOf("月")) > 0 ? ((String) temp
											.get("column_1")).indexOf("月") : 0));
					
				}
				if(flag){
					for (int i = 2; i <= 30; i++) {
						BigDecimal sum = new BigDecimal(0);
						/*
						 * String bz = (String)((Map<String, Object>)
						 * result.get(0)).get("column_"+i); if("备注".equals(bz)||bz
						 * == "备注"|| "".equals(null)||"" == bz) continue;
						 */
						for (int ii = 1; ii < result.size(); ii++) {
							Map<String, Object> temp = (Map<String, Object>) result
									.get(ii);
							if (!((((String) temp.get("column_" + i)) == null) || ((String) temp
									.get("column_" + i)).equals(""))) {
								if (!isNum(temp.get("column_" + i).toString())) {
									ii = result.size();
									sum = new BigDecimal(0);
									continue;
								}
								BigDecimal colValue = new BigDecimal(temp.get(
										"column_" + i).toString());
								sum = sum.add(colValue);
							}
						}
						totalmap.put("column_" + i, sum);
					}
					totalmap.put("column_1", "合计");
					result.add(totalmap);
				}
				month.put("合计", 13 + "");
				month.put("月份", 0 + "");
				Collections.sort(result, new Comparator<Map<String, Object>>() {
					public int compare(Map<String, Object> o1,
							Map<String, Object> o2) {

						return Type.GetInt(month.get(o1.get("column_1")))
								- Type.GetInt(month.get(o2.get("column_1")));
					}
				});
			}
			size = result.size();
		} catch (Exception e) {
			result = null;
		}

		StoreResponse response = new StoreResponse();
		response.setSuccess(true);
		response.setTotalCount(size);
		response.setRows(result);
		return response;
	}

	@Override
	public void delExcelSheetDataByYear(final int year, final int type) {
		String sql = SQLScript_BBZFinancy.delExcelSheetDateByYear;

		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, year);
				ps.setInt(2, type);
			}
		});
	}

	@Override
	public void updateIsNotImportForSheet(final int year, final int type) {
		String sql = SQLScript_BBZFinancy.updateIsNotImportForSheet;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setInt(1, year);
				ps.setInt(2, type);
			}
		});
	}

	@Override
	public JsonResponse getBankDeposit(int year, int type) {
		String sql = SQLScript_BBZFinancy.getDateIncome;
		logger.info(sql);
		List result = null;
		int size = 0;
		final Map<String, String> month = new HashMap<String, String>();
		try {
			result = jt.queryForList(sql, new Object[] { new Integer(year),
					new Integer(type), new Integer(year), new Integer(type), });
			if (result.size() > 0) {
				for (int i = 0; i < result.size(); i++) {
					Map<String, Object> temp = (Map<String, Object>) result
							.get(i);
					month.put((String) temp.get("column_1"),
							((String) temp.get("column_1")).substring(
									0,
									(((String) temp.get("column_1"))
											.indexOf("月")) > 0 ? ((String) temp
											.get("column_1")).indexOf("月") : 0));
				}
				/*
				 * for(int i=2;i<=30;i++){ float sum = 0.00f; String bz =
				 * (String)((Map<String, Object>)
				 * result.get(0)).get("column_"+i); if("备注".equals(bz)||bz ==
				 * "备注"|| "".equals(null)||"" == bz) continue; for(int
				 * ii=1;ii<result.size();ii++){ Map<String, Object> temp =
				 * (Map<String, Object>) result.get(ii);
				 * if(!(((String)temp.get("column_"+i)) == null)){
				 * if(!isNum(temp.get("column_"+i).toString())){ ii =
				 * result.size(); sum = 0.00f; continue; }
				 * 
				 * float a = Type.GetFloat(temp.get("column_"+i)); sum = sum
				 * +Type.GetFloat(temp.get("column_"+i)); } };
				 * 
				 * totalmap.put("column_"+i, sum); }
				 */
				month.put("月份", -1 + "");
				month.put("年初数", 0 + "");
				Collections.sort(result, new Comparator<Map<String, Object>>() {
					public int compare(Map<String, Object> o1,
							Map<String, Object> o2) {

						return Type.GetInt(month.get(o1.get("column_1")))
								- Type.GetInt(month.get(o2.get("column_1")));
					}
				});
			}
			size = result.size();
		} catch (Exception e) {
			result = null;
		}

		StoreResponse response = new StoreResponse();
		response.setSuccess(true);
		response.setTotalCount(size);
		response.setRows(result);
		return response;
	}

	@Override
	public boolean findByYearAndType(int year, int type, String sheet_uuid) {
		String sql = SQLScript_BBZFinancy.findByYearOrSheetUuidAndType;
		List list = jt.queryForList(sql, new Object[] { new Integer(year),
				new String(sheet_uuid), new Integer(type) });
		return list.size() > 0 ? true : false;
	}

	@Override
	public void delExcelSheetDataBySheetUuid(final String sheet_uuid,
			final int type) {

		String sql = SQLScript_BBZFinancy.delExcelSheetDataBySheetUuid;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setString(1, sheet_uuid);
				ps.setInt(2, type);
			}
		});
	}

	@Override
	public void updateIsNotImportForSheet(final String sheet_uuid) {
		String sql = SQLScript_BBZFinancy.updateIsNotImportForSheetBySheet_Uuid;
		jt.update(sql, new PreparedStatementSetter() {
			public void setValues(PreparedStatement ps) throws SQLException {
				ps.setString(1, sheet_uuid);
			}
		});

	}

	@Override
	public JsonResponse getPayList(String iyear, String compareYear) {
		int size = 0;
		List result = null;
		final String sql = "{call sp_ywzhdf(?,?)}";
		if (StringUtils.isEmpty(compareYear)) {
			compareYear = null;
		}
		result = jt.queryForList(sql, new Object[] { iyear, compareYear });
		size = result.size();
		StoreResponse response = new StoreResponse();
		response.setSuccess(true);
		response.setTotalCount(size);
		response.setRows(result);
		return response;
	}

	@Override
	public JsonResponse getYearList() {
		String sql = "SELECT  DISTINCT year FROM xmis.fas_GL_accvouch gl "
						+ " WHERE   ccode = '115101' AND cdept_id IS NOT NULL ORDER BY year DESC";
		
		List result =  jt.queryForList(sql);
		int size = result.size();
		StoreResponse response = new StoreResponse();
		response.setSuccess(true);
		response.setTotalCount(size);
		response.setRows(result);
		return response;
	}
	@Override
	public JsonResponse getSalaryYearList() {
		String sql = " SELECT  DISTINCT iyear FROM xmis.T_Salary_Info ORDER BY iyear DESC";
		
		List result =  jt.queryForList(sql);
		int size = result.size();
		StoreResponse response = new StoreResponse();
		response.setSuccess(true);
		response.setTotalCount(size);
		response.setRows(result);
		return response;
	}
	@Override
	public JsonResponse getYearList(int type) {
		String sql = "SELECT  DISTINCT iyear FROM xmis.T_Income_Info  WHERE   type =? ORDER BY iyear DESC";

		List result = jt.queryForList(sql,new Object[]{type});
		int size = result.size();
		StoreResponse response = new StoreResponse();
		response.setSuccess(true);
		response.setTotalCount(size);
		response.setRows(result);
		return response;
	}
}
