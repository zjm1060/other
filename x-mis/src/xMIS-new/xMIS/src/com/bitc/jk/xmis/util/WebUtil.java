package com.bitc.jk.xmis.util;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Random;
import java.util.Set;

import javax.servlet.ServletContext;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
import javax.servlet.jsp.JspWriter;

import net.sf.json.JSONObject;

import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.dom4j.Document;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;
import org.dom4j.io.OutputFormat;
import org.dom4j.io.XMLWriter;

import com.bitc.jk.xmis.model.ConditionMap;
import com.bitc.jk.xmis.model.EmployeeObj;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.model.sys.RelationalOperator;
import com.bitc.jk.xmis.service.SysService;
import com.bitc.jk.xmis.util.sql.CompositParam;
import com.bitc.jk.xmis.util.sql.SimpleParam;
import com.bitc.jk.xmis.util.sql.StandardQuery;

public class WebUtil {

	private static Logger logger = Logger.getLogger(WebUtil.class);

	private static ThreadLocal<EmployeeObj> threadLocal = new ThreadLocal<EmployeeObj>();

	public WebUtil() {
	}

	private static String buildURLLink(Entry entry, Map map, String type) {

		String urlStr = "";

		if ("".equals(type))
			return urlStr;

		if (type.equals(WebKeys.Action_Method_Name_DepartmentTotalSummary)) {
			if ("user_name".equals(String.valueOf(entry.getKey())))
				urlStr = "^" + "/xMIS/orgMaint.do?method="
						+ WebKeys.Action_Method_Name_GetEmployee + "&emp="
						+ String.valueOf(map.get("user_id"));
			else if ("proj_code".equals(String.valueOf(entry.getKey()))
					|| "proj_name".equals(String.valueOf(entry.getKey())))
				urlStr = "^" + "/xMIS/projMaint.do?method="
						+ WebKeys.Action_Method_Name_GetProject + "&proj="
						+ String.valueOf(map.get("proj_id"));
			else if ("finished_task".equals(String.valueOf(entry.getKey()))) {
				if (((Integer) entry.getValue()).intValue() > 0)
					urlStr = "^" + "/xMIS/projMaint.do?method="
							+ WebKeys.Action_Method_Name_TaskList + "&proj="
							+ String.valueOf(map.get("proj_id"))
							+ "&taskType=finished&emp="
							+ String.valueOf(map.get("user_id")) + "&empName="
							+ String.valueOf(map.get("user_name"));
			} else if ("unfinished_task".equals(String.valueOf(entry.getKey()))) {
				if (((Integer) entry.getValue()).intValue() > 0)
					urlStr = "^" + "/xMIS/projMaint.do?method="
							+ WebKeys.Action_Method_Name_TaskList + "&proj="
							+ String.valueOf(map.get("proj_id"))
							+ "&taskType=unfinished&emp="
							+ String.valueOf(map.get("user_id")) + "&empName="
							+ String.valueOf(map.get("user_name"));
			} else if ("not_start_task".equals(String.valueOf(entry.getKey()))) {
				if (((Integer) entry.getValue()).intValue() > 0)
					urlStr = "^" + "/xMIS/projMaint.do?method="
							+ WebKeys.Action_Method_Name_TaskList + "&proj="
							+ String.valueOf(map.get("proj_id"))
							+ "&taskType=notStart&emp="
							+ String.valueOf(map.get("user_id")) + "&empName="
							+ String.valueOf(map.get("user_name"));
			}
		} else if (type.equals(WebKeys.Action_Method_Name_AttachedList)) {
			if (DBSchema.Attachment.AttachmentName.equals(String.valueOf(entry
					.getKey())))
				urlStr = "^"
						+ "/xMIS/fileUpload.do?type=3&attID="
						+ Type.getString(map
								.get(DBSchema.Attachment.AttachmentID));
		} else if (type.equals(WebKeys.Action_Method_Name_ContractList)
				|| type.equals(WebKeys.Action_Method_Name_ChildContractList)) {
			if (Type.getString(entry.getKey()).equals(
					DBSchema.Contract.ContractNo)
					|| Type.getString(entry.getKey()).equals(
							DBSchema.Contract.ContractName)
					|| Type.getString(entry.getKey()).equals(
							DBSchema.Contract.Abbreviation)
					|| Type.getString(entry.getKey()).equals(
							DBSchema.Contract.ContractAmt))
				urlStr = "".equals(entry.getValue() == null ? "" : entry
						.getValue()) ? "" : "^"
						+ "/xMIS/contractMain.do?method=contract&contractID="
						+ Type.getString(map.get(DBSchema.Contract.ContractID));
			else if (Type.getString(entry.getKey()).equals(
					DBSchema.Contract.ParentContractAbb))
				urlStr = (entry.getValue() == null || "".equals(entry
						.getValue())) ? "" : "^"
						+ "/xMIS/contractMain.do?method=contract&contractID="
						+ Type.getString(map.get(DBSchema.Contract.ParentID));
		}
		return urlStr;
	}

	public static String buildStoreForJson(List aList, String frmName,
			PageInfo pageInfo) {
		return (pageInfo == null || (pageInfo.getStart() == 0 && pageInfo
				.getLimit() == 0)) ? buildStoreForJsonWithoutPageInfo(aList,
				frmName) : buildStoreForJson(aList, frmName);
	}

	public static String buildStoreForJson(List aList, String frmName) {

		logger.info("---> buildGridStoreForJson:" + frmName);

		String totalStr = "{\"success\":\"true\",\"totalCount\":\"";
		StringBuilder store = new StringBuilder();
		if (aList != null && aList.size() > 0) {

			totalStr += String.valueOf(((Map) aList.get(0)).get("row_no"));

			aList.remove(0);

			StringBuilder row = new StringBuilder();
			for (Iterator i = aList.listIterator(); i.hasNext();) {
				Map map = (Map) i.next();

				int mapsize = map.size();
				Iterator it = map.entrySet().iterator();

				StringBuilder data = new StringBuilder();

				for (int j = 0; j < mapsize; j++) {
					Map.Entry entry = (Map.Entry) it.next();

					/* url link */
					String urlStr = buildURLLink(entry, map, frmName);

					data.append("\"")
							.append(Type.getString(entry.getKey()))
							.append("\":\"")
							.append((Type.getString(entry.getValue()).equals(
									"null") ? "" : Type.getString(entry
									.getValue()) + urlStr)).append("\"");
					if (j < mapsize - 1) {
						data.append(",");
					}
				}
				row.append("{").append(data).append("},");
			}
			row.deleteCharAt(row.length() - 1);
			store.append("[").append(row).append("]");
		} else {
			totalStr += "0";
			store.append("[]");
		}
		return new StringBuilder(totalStr).append("\",\"rows\":").append(store)
				.append("}").toString();
	}

	public static String buildStoreForJsonWithTotalInfo(List aList,
			String frmName) {

		logger.info("---> buildStoreForJsonWithTotalInfo:" + frmName);

		String totalStr = "{\"success\":\"true\",\"totalCount\":\"";
		StringBuilder store = new StringBuilder();
		if (aList != null && aList.size() > 0) {

			totalStr += String.valueOf(((Map) aList.get(aList.size() - 1))
					.get("row_no"));

			StringBuilder row = new StringBuilder();
			for (Iterator i = aList.listIterator(); i.hasNext();) {
				Map map = (Map) i.next();

				int mapsize = map.size();
				Iterator it = map.entrySet().iterator();

				StringBuilder data = new StringBuilder();

				for (int j = 0; j < mapsize; j++) {
					Map.Entry entry = (Map.Entry) it.next();

					/* url link */
					String urlStr = buildURLLink(entry, map, frmName);

					String key = EscapeUtils.escapeDoubleQuotationMarks(Type
							.getString(entry.getKey()));
					String value = EscapeUtils.escapeDoubleQuotationMarks(Type
							.getString(entry.getValue()));
					data.append("\"")
							.append(key)
							.append("\":\"")
							.append((value.equals("null") ? "" : value + urlStr))
							.append("\"");
					if (j < mapsize - 1) {
						data.append(",");
					}
				}
				row.append("{").append(data).append("},");
			}
			row.deleteCharAt(row.length() - 1);
			store.append("[").append(row).append("]");
		} else {
			totalStr += "0";
			store.append("[]");
		}
		return new StringBuilder(totalStr).append("\",\"rows\":").append(store)
				.append("}").toString();
	}

	public static String buildJsonStoreByList(List<?> list) {
		return buildJsonStoreByList(list, list.size());
	}

	public static String buildJsonStoreByList(List<?> list, int totalCount) {
		JSONObject object = new JSONObject();
		object.element("success", true);
		object.element("totalCount", totalCount);
		object.element("rows", JSONUtils.toJSON(list, true));
		return object.toString();
	}
	public static String buildJsonStoreBySet(Set<?> set) {
		return buildJsonStoreBySet(set, set.size());
	}
	public static String buildJsonStoreBySet(Set<?> set, int totalCount) {
		JSONObject object = new JSONObject();
		object.element("success", true);
		object.element("totalCount", totalCount);
		object.element("rows", JSONUtils.toJSON(set, true));
		return object.toString();
	}
	public static String buildStoreForJsonWithoutPageInfo(List aList,
			String frmName) {
		logger.info("---> buildStoreForJsonWithoutPageInfo" + aList.size());
		String totalStr = "{\"success\":\"true\",\"totalCount\":\"";
		StringBuilder store = new StringBuilder();
		if (aList != null && aList.size() > 0) {
			totalStr += String.valueOf(aList.size());
			StringBuilder row = new StringBuilder();
			for (Iterator i = aList.listIterator(); i.hasNext();) {
				Map map = (Map) i.next();
				int mapsize = map.size();
				Iterator it = map.entrySet().iterator();
				StringBuilder data = new StringBuilder();
				for (int j = 0; j < mapsize; j++) {
					Map.Entry entry = (Map.Entry) it.next();
					String urlStr = buildURLLink(entry, map, frmName);
					String key = EscapeUtils.escapeDoubleQuotationMarks(Type
							.getString(entry.getKey()));
					String value = EscapeUtils.escapeDoubleQuotationMarks(Type
							.getString(entry.getValue()));
					data.append("\"")
							.append(key)
							.append("\":\"")
							.append((value.equals("null") ? "" : value + urlStr))
							.append("\"");
					if (j < mapsize - 1) {
						data.append(",");
					}
				}
				row.append("{").append(data).append("},");
			}
			row.deleteCharAt(row.length() - 1);
			store.append("[").append(row).append("]");
		} else {
			totalStr += "0";
			store.append("[]");
		}
		return new StringBuilder(totalStr).append("\",\"rows\":").append(store)
				.append("}").toString();
	}

	public static Document buildGridStoreXML(List aList, String frmName) {
		logger.info("---> buildGridStoreXML");
		/** 建立document对象 */
		Document document = DocumentHelper.createDocument();
		if (aList != null && aList.size() > 0) {
			/** 建立XML文档的根 */
			Element rootElement = document.addElement("dataset");
			/* 记录 */
			rootElement.addElement("TotalResults").setText(
					String.valueOf(((Map) aList.get(0)).get("row_no")));
			// itemsElement.addElement("TotalPage").setText("3");

			aList.remove(0);
			for (Iterator i = aList.listIterator(); i.hasNext();) {
				Element itemElement = rootElement.addElement("Item");

				Map map = (Map) i.next();

				int mapsize = map.size();
				Iterator it = map.entrySet().iterator();
				for (int j = 0; j < mapsize; j++) {
					Map.Entry entry = (Map.Entry) it.next();

					/* url link */
					String urlStr = buildURLLink(entry, map, frmName);

					itemElement.addElement(String.valueOf(entry.getKey()))
							.setText(
									String.valueOf(entry.getValue()).equals(
											"null") ? "" : String.valueOf(entry
											.getValue()) + urlStr);
				}

			}

			String filePath = new String(
					"D:/apache-tomcat-5.5.28/webapps/xMIS/temp.xml");
			try {

				XMLWriter writer = new XMLWriter(new java.io.FileOutputStream(
						filePath));

				writer.write(document);
				writer.close();

			} catch (Exception ex) {

				ex.printStackTrace();

			}

		}
		return document;

	}

	// public static Document buildGridStoreXML(List aList, String frmName) {
	//
	// logger.info("---> buildGridStoreXML");
	//
	// /** 建立document对象 */
	// Document document = DocumentHelper.createDocument();
	//
	//
	// if (aList != null && aList.size() > 0) {
	// /** 建立XML文档的根 */
	// Element rootElement = document.addElement("ItemSearchResponse");
	// /* 应答信息 */
	// Element requestElement = rootElement.addElement("Request");
	// /* 记录 */
	// Element itemsElement = requestElement.addElement("Items");
	// itemsElement.addElement("TotalResults").setText(
	// String.valueOf(((Map) aList.get(0)).get("row_no")));
	// // itemsElement.addElement("TotalPage").setText("3");
	//
	// aList.remove(0);
	// for (Iterator i = aList.listIterator(); i.hasNext();) {
	// Element itemElement = itemsElement.addElement("Item");
	//
	// Map map = (HashMap) i.next();
	//
	// int mapsize = map.size();
	// Iterator it = map.entrySet().iterator();
	// for (int j = 0; j < mapsize; j++) {
	// Map.Entry entry = (Map.Entry) it.next();
	//
	// /* url link */
	// String urlStr = buildURLLink(entry, map, frmName);
	//
	// itemElement.addElement(String.valueOf(entry.getKey()))
	// .setText(
	// String.valueOf(entry.getValue()).equals(
	// "null") ? "" : String.valueOf(entry
	// .getValue())
	// + urlStr);
	// }
	//
	// }
	//
	// String filePath = new String(
	// "D:/apache-tomcat-5.5.28/webapps/xMIS/temp.xml");
	// try {
	// XMLWriter writer = new XMLWriter(new java.io.FileOutputStream(
	// filePath));
	//
	// writer.write(document);
	//
	// writer.close();
	//
	// } catch (Exception ex) {
	//
	// ex.printStackTrace();
	//
	// }
	//
	// }
	// return document;
	//
	// }

	public static Map getNewPart(String str) {

		// logger.info(str);
		if (str == null || str.length() == 0)
			return null;

		// List result = new ArrayList();
		Map mTemp = new HashMap();
		str = str.trim();

		char[] tmp = new char[1000];
		int tmpLength = 0;

		char[] chars = str.toCharArray();

		boolean inQuota = false;
		boolean newKeywordFlag = true;

		String key = "", value = "";

		for (int i = 0; i < chars.length; i++) {

			if (chars[i] == '"') {
				if (!inQuota) {
					inQuota = true;
				} else {
					inQuota = false;
					String aWord = new String(tmp, 0, tmpLength);
					// result.add(aWord);
					tmpLength = 0;

					// key = (aWord.indexOf("-new") > 0 && aWord.indexOf("M-") <
					// 0) ? aWord
					// : key;
					// value = (aWord.indexOf("M-") == 0 ||
					// aWord.indexOf("-new") < 0) ? aWord
					// : value;

					key = getMapKeyValue("key", aWord, key);
					value = getMapKeyValue("value", aWord, value);
				}
				newKeywordFlag = true;
			} else if (!inQuota
					&& (chars[i] == ' ' || chars[i] == '\n' || chars[i] == '\t'
							|| chars[i] == '\r' || chars[i] == ','
							|| chars[i] == ';' || chars[i] == '^')) {
				if (tmpLength > 0) {
					String aWord = new String(tmp, 0, tmpLength);
					if ("null".equals(aWord)) {

						// result.add(null);
					} else {
						// key = (aWord.indexOf("-new") > 0 &&
						// aWord.indexOf("M-") < 0) ? aWord
						// : key;
						// value = (aWord.indexOf("M-") == 0 || aWord
						// .indexOf("-new") < 0) ? aWord : value;

						key = getMapKeyValue("key", aWord, key);
						value = getMapKeyValue("value", aWord, value);
						// result.add(aWord);
					}
					tmpLength = 0;
				}
			} else {
				tmp[tmpLength++] = chars[i];
			}

			if (!"".equals(key) && !"".equals(value)) {
				mTemp.put(key, value);
				key = "";
				value = "";
			}

		}

		if (tmpLength > 0) {
			String aWord = new String(tmp, 0, tmpLength);
			if ("null".equals(aWord)) {
				// result.add(null);
			} else {
				// key = (aWord.indexOf("-new") > 0 && aWord.indexOf("M-") < 0)
				// ? aWord
				// : key;
				// value = (aWord.indexOf("M-") == 0 || aWord.indexOf("-new") <
				// 0) ? aWord
				// : value;

				key = getMapKeyValue("key", aWord, key);
				value = getMapKeyValue("value", aWord, value);

				// result.add(aWord);
			}
		}

		if (!"".equals(key) && !"".equals(value)) {

			logger.info(value);

			mTemp.put(key, value);
			key = "";
			value = "";
		}

		// for(int i=0; i<result.size(); i++){
		// logger.info((String)result.get(i));
		// }

		Iterator it = mTemp.entrySet().iterator();
		for (int j = 0; j < mTemp.size(); j++) {
			Map.Entry entry = (Map.Entry) it.next();

			logger.info(entry.getKey() + "==" + entry.getValue());
		}

		return mTemp;
	}

	private static String getMapKeyValue(String type, String aWord,
			String originalValue) {

		if ("key".equals(type))
			return (aWord.indexOf("-new") > 0 && aWord.indexOf("M-") < 0) ? aWord
					: originalValue;
		else
			return (aWord.indexOf("M-") == 0 || aWord.indexOf("-new") < 0) ? (aWord
					.indexOf("M-") == 0 ? aWord.substring(2) : aWord)
					: originalValue;
	}

	public static String getRandomString(int length) {
		String base = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
		Random random = new Random();
		StringBuffer sb = new StringBuffer();
		for (int i = 0; i < length; i++) {
			int number = random.nextInt(base.length());
			sb.append(base.charAt(number));
		}
		return sb.toString();
	}

	public static void returnResponse(HttpServletResponse response,
			String message) {
		returnResponse(response, message,"application/json; charset=utf-8");
	}

	public static void returnResponse(HttpServletResponse response,
			String message, String contentType) {
		logger.debug(message);
		try {
			response.setContentType(contentType);
			EscapeUtils.escape(response.getWriter(), message);
		} catch (IOException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public static void returnResponseForXML(HttpServletResponse response,
			String message) {
		logger.debug(message);
		try {
			response.setContentType("text/xml; charset=utf-8");
			response.getWriter().write(message);
		} catch (IOException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	// public static String generateSQLStr(String sqlStr, PageInfo pageInfo) {
	//
	// String orderByStr = (pageInfo.getSort() == null || "".equals(pageInfo
	// .getSort())) ? "" : "\n order by " + pageInfo.getSort() + " "
	// + pageInfo.getDir();
	//
	// // logger.info(Type.getString(pageInfo.getStart()) + "//" +
	// // Type.getString(pageInfo.getLimit()));
	// return (pageInfo.getStart() == 0 && pageInfo.getLimit() == 0) ? (sqlStr
	// + SQLScript.resultStrByBiz + orderByStr +
	// SQLScript.getResultStrWithoutPaging)
	// : (sqlStr + SQLScript.resultStrByBiz + orderByStr +
	// SQLScript.getResultStrWithPaging);
	// }

	
/*	public static String generateSQLStr(String sqlStr, PageInfo pageInfo) {
		sqlStr = "SET NOCOUNT ON \n" + sqlStr;
		String orderByStr = (pageInfo.getSort() == null || "".equals(pageInfo
				.getSort())) ? "" : "\n order by " + pageInfo.getSort() + " "
				+ pageInfo.getDir();

		return (pageInfo.getStart() == 0 && pageInfo.getLimit() == 0) ? (sqlStr
				+ SQLScript.getResultStrWithoutPaging + orderByStr)
				: (sqlStr + SQLScript.getResultStrWithPaging + orderByStr + SQLScript.getResultStringWithPagingDefault);
	}*/
	
	/** order by a desc,b asc
	 * @param sqlStr
	 * @param pageInfo
	 * @return
	 */
	public static String generateSQLStr(String sqlStr,
			PageInfo pageInfo) {
		sqlStr = "SET NOCOUNT ON \n" + sqlStr;
		String orderByStr = (pageInfo.getSort() == null || "".equals(pageInfo
				.getSort())) ? "" : "\n order by "
				+ getOrderStr(pageInfo.getSort(), pageInfo.getDir());

		return (pageInfo.getStart() == 0 && pageInfo.getLimit() == 0) ? (sqlStr
				+ SQLScript.getResultStrWithoutPaging + orderByStr)
				: (sqlStr + SQLScript.getResultStrWithPaging + orderByStr + SQLScript.getResultStringWithPagingDefault);
	}
	
	/** sortinfo拼order by 后的条件
	 * @param fields
	 * @param direction
	 * @return
	 */
	public static String getOrderStr(String fields, String direction) {
		StringBuilder result = new StringBuilder();
		int size = fields.split(",").length;
		String[] fieldsArray = fields.split(",");
		String[] directionArray = direction.split(",");
		if(direction.indexOf(",")<0){
			result.append(fields).append(" ").append(direction);
		}else{
			for (int i = 0; i < size; i++) {
				result.append(fieldsArray[i]).append(" ").append(directionArray[i])
				.append(",");
			}
			result.deleteCharAt(result.length() - 1);
		}
		return " " + result.toString() + " ";
	}

	public static String generateSQLStr(String sqlStr, PageInfo pageInfo,
			String totalInfo) {
		return generateSQLStr(sqlStr, pageInfo).replace(
				SQLScript.getResultStringWithPagingDefault, totalInfo);
	}

	public static String generateSQLForPosition(String sqlStr,
			String columnName, PageInfo pageInfo) {
		sqlStr = "SET NOCOUNT ON \n" + sqlStr;
		String orderByStr = pageInfo.getSort() == null ? "" : "\n order by "
				+ pageInfo.getSort() + " " + pageInfo.getDir();

		return sqlStr + SQLScript.resultStrByBiz + orderByStr
				+ SQLScript.positionStr + " where " + columnName + "=?";
	}

	public static PageInfo generatePageInfo(HttpServletRequest request) {

		int start = new Integer(
				request.getParameter("start") != null ? request
						.getParameter("start") : "0").intValue();
		int limit = new Integer(
				request.getParameter("limit") != null ? request
						.getParameter("limit") : "0").intValue();
		String sort = request.getParameter("sort");
		String dir = request.getParameter("dir");

		// logger.info("==========" + sort + "==============");

		PageInfo pageInfo = new PageInfo();
		pageInfo.setStart(start);
		pageInfo.setLimit(limit);
		pageInfo.setSort(sort);
		pageInfo.setDir(dir);

		return pageInfo;
	}

	public static void returnResponseForXML(HttpServletRequest request,
			HttpServletResponse response, Document doc) throws Exception {

		try {
			request.setAttribute("success", "true");
			response.setContentType("application/xml; charset=utf-8");
			OutputFormat format = OutputFormat.createCompactFormat();
			format.setEncoding("utf-8");
			PrintWriter out = response.getWriter();
			XMLWriter writer = new XMLWriter(out, format);

			writer.write(doc);
			writer.flush();
			writer.close();
		} catch (Exception e) {
			e.printStackTrace();
		}

	}

	public static String getMaxIDFromTab(String columnName, String tableName) {
		// select + "count(*)+1001" + from;

		return "select Max(" + columnName + ")+1 from " + tableName;
	}

	public static String buildSumInfo(List aList) {

		logger.info("---> buildSumInfo");

		String store = "", totalStr = "},";

		if (aList != null && aList.size() > 0) {
			String row = "";
			for (Iterator i = aList.listIterator(); i.hasNext();) {
				Map map = (Map) i.next();

				int mapsize = map.size();
				Iterator it = map.entrySet().iterator();

				String sumMsg = "";

				for (int j = 0; j < mapsize; j++) {
					Map.Entry entry = (Map.Entry) it.next();

					sumMsg += "\"" + Type.getString(entry.getKey()) + "\":\""
							+ Type.getString(entry.getValue()) + "\",";

				}
				row += "{" + sumMsg.substring(0, sumMsg.length() - 1) + "},";
				logger.info(row);

			}
			store += row.substring(0, row.length() - 1) + "]}";
			logger.info(store);
		} else {
			totalStr = "}]}";
		}

		// logger.info(store);

		logger.info(totalStr + store);

		return totalStr + store;

	}

	public static int getCurrencyType(String currecnyGraphy) {

		int currencyFlag = 0;

		if (WebKeys.Type_Currency_Graphy_RMB.equals(currecnyGraphy)) {
			currencyFlag = WebKeys.Type_Currency_RMB;
		} else if (WebKeys.Type_Currency_Graphy_USD.equals(currecnyGraphy)) {
			currencyFlag = WebKeys.Type_Currency_USD;
		} else if (WebKeys.Type_Currency_Graphy_Euro.equals(currecnyGraphy)) {
			currencyFlag = WebKeys.Type_Currency_Euro;
		} else if (WebKeys.Type_Currency_Graphy_HKD.equals(currecnyGraphy)) {
			currencyFlag = WebKeys.Type_Currency_HKD;
		}

		return currencyFlag;
	}

	public static List<QueryParam> generateQueryCondition(
			HttpServletRequest request, SysService sysService) {

		// List<QueryParam> list = new ArrayList<QueryParam>();

		// List<QueryParam> list = (ArrayList<QueryParam>) request
		// .getAttribute(WebKeys.Request_Attribute_Name_DataScopeQueryParam);

		List<QueryParam> list;

		if ((request.getAttribute("queryType") != null && request.getAttribute(
				"queryType").equals("Herb"))
				|| Type.GetInt(request.getParameter("contractTypeID")) == WebKeys.Type_ContractType_Payout) {
			list = new ArrayList<QueryParam>();
		} else {
			int warningType = Type.GetInt(request.getParameter("warningType"));
			list = (ArrayList<QueryParam>) request
					.getAttribute(WebKeys.Request_Attribute_Name_DataScopeQueryParam);
			if (warningType != 0) {
				return list;
			}
		}

		String selIds = request.getParameter("selIds");
		if (StringUtils.isEmpty(selIds)) {
			return list;
		}
		String[] ids = StringUtils.split(selIds, "|");
		for (String id : ids) {
			ConditionMap conditionMap = sysService.getConditionMap(Integer
					.parseInt(id.substring(4)));
			String value = request.getParameter(id);
			Object val = Type.get(conditionMap.getConditionItem().getType(),
					value);
			QueryParam queryParam = new QueryParam(id, val);

			queryParam.setCondition(conditionMap);
			list.add(queryParam);
		}

		return list;
	}

	public static List<QueryParam> generateInklingCondition(String inklings,
			String inkling) {
		List<QueryParam> list = new ArrayList<QueryParam>();
		String[] str1s = StringUtils.split(inklings, "|");
		String operator = getOperator(inkling);
		String value = null;
		if (operator == null) {
			value = inkling;
		} else {
			value = StringUtils.trim(inkling.substring(operator.length()));
		}
		for (String str1 : str1s) {
			String[] str2s = StringUtils.split(str1, ",");
			String name = str2s[0];
			String op = operator == null ? str2s[1] : operator;
			String type = str2s[2];
			if (Type.isType(value, type)) {
				if (Type.STRING.equals(type)
						&& (RelationalOperator.GT.equals(op)
								|| RelationalOperator.GE.equals(op)
								|| RelationalOperator.LT.equals(op) || RelationalOperator.LE
								.equals(op))) {
					continue;
				}
				QueryParam param = QueryParam.buildQueryParam(name, value, op,
						type);
				list.add(param);
			}
		}
		return list;
	}

	public static StandardQuery generateStandardQueryByInkling(
			HttpServletRequest request) {
		String inkling = request.getParameter("inkling");// 用户输入的值
		String inklings = request.getParameter("inklings");// 定义的查询属性、运算符、及类型
		if (StringUtils.isEmpty(inkling) || StringUtils.isEmpty(inklings)) {
			return null;
		}
		return generateStandardQueryByInkling(inklings, inkling);
	}

	/**
	 * 适用于前端使用InklingSearchField构造的模糊查询
	 * 
	 * @param inklings
	 *            查询的属性、类型、运算符
	 * @param inkling
	 *            用户输入的值
	 * @return
	 */
	public static StandardQuery generateStandardQueryByInkling(String inklings,
			String inkling) {
		String[] str1s = StringUtils.split(inklings, "|");
		String operator = getOperator(inkling);
		String value = null;
		if (operator == null) {
			value = inkling;
		} else {
			value = StringUtils.trim(inkling.substring(operator.length()));
		}
		CompositParam query = CompositParam.buildOrCompositParam();
		for (String str1 : str1s) {
			String[] str2s = StringUtils.split(str1, ",");
			String name = str2s[0];
			String op = operator == null ? str2s[1] : operator;
			String type = str2s[2];
			if (Type.isType(value, type)) {
				if (Type.STRING.equals(type)
						&& (RelationalOperator.GT.equals(op)
								|| RelationalOperator.GE.equals(op)
								|| RelationalOperator.LT.equals(op) || RelationalOperator.LE
								.equals(op))) {
					continue;
				}
				SimpleParam param = SimpleParam.buildSimpleParam(name,
						Type.get(type, value), type, op);
				query.addStandardQuery(param);
			}
		}
		return query;
	}

	/**
	 * 适用于前端使用MulitSearchPanel构造的组合查询
	 * 
	 * @param inklings
	 * @param inkling
	 * @return
	 */
	public static StandardQuery generateStandardQueryByMulit(
			HttpServletRequest request, SysService sysService) {
		String selIds = request.getParameter("selIds");
		if (StringUtils.isEmpty(selIds)) {
			return null;
		}
		CompositParam query = CompositParam.buildAndCompositParam();
		String[] ids = StringUtils.split(selIds, "|");
		for (String id : ids) {
			ConditionMap conditionMap = sysService.getConditionMap(Integer
					.parseInt(id.substring(4)));
			String value = request.getParameter(id);
			Object val = Type.get(conditionMap.getConditionItem().getType(),
					value);
			String op = conditionMap.getOperator().getOperator().trim();
			SimpleParam param;
			if (RelationalOperator.JDBCSQL.equals(op)) {
				String sql = conditionMap.getSql();
				param = SimpleParam.buildSimpleParam(op, sql, val);
			} else {
				param = SimpleParam.buildSimpleParam(conditionMap
						.getConditionItem().getName(), val, conditionMap
						.getConditionItem().getType(), op);
			}
			query.addStandardQuery(param);
		}
		return query;
	}

	/**
	 * 适用于前端使用MulitSearchPanel构造的组合查询
	 * 
	 * @param inklings
	 * @param inkling
	 * @return
	 */
	public static List<StandardQuery> generateStandardQuerysByMulit(
			HttpServletRequest request, SysService sysService) {
		String selIds = request.getParameter("selIds");
		if (StringUtils.isEmpty(selIds)) {
			return null;
		}
		List<StandardQuery> querys = new ArrayList<StandardQuery>();
		String[] ids = StringUtils.split(selIds, "|");
		for (String id : ids) {
			ConditionMap conditionMap = sysService.getConditionMap(Integer
					.parseInt(id.substring(4)));
			String value = request.getParameter(id);
			Object val = Type.get(conditionMap.getConditionItem().getType(),
					value);
			String op = conditionMap.getOperator().getOperator().trim();
			SimpleParam param;
			if (RelationalOperator.JDBCSQL.equals(op)) {
				String sql = conditionMap.getSql();
				param = SimpleParam.buildSimpleParam(op, sql, val);
			} else {
				param = SimpleParam.buildSimpleParam(conditionMap
						.getConditionItem().getName(), val, conditionMap
						.getConditionItem().getType(), op);
			}
			querys.add(param);
		}
		return querys;
	}

	public static List<QueryParam> generateInklingCondition(String inklings,
			String inkling, List<QueryParam> list) {

		// List<QueryParam> list = (ArrayList<QueryParam>) request
		// .getAttribute(WebKeys.Request_Attribute_Name_DataScopeQueryParam);

		String[] str1s = StringUtils.split(inklings, "|");
		String operator = getOperator(inkling);
		String value = null;
		if (operator == null) {
			value = inkling;
		} else {
			value = StringUtils.trim(inkling.substring(operator.length()));
		}
		for (String str1 : str1s) {
			String[] str2s = StringUtils.split(str1, ",");
			String name = str2s[0];
			String op = operator == null ? str2s[1] : operator;
			String type = str2s[2];
			if (Type.isType(value, type)) {
				if (Type.STRING.equals(type)
						&& (RelationalOperator.GT.equals(op)
								|| RelationalOperator.GE.equals(op)
								|| RelationalOperator.LT.equals(op) || RelationalOperator.LE
								.equals(op))) {
					continue;
				}
				QueryParam param = QueryParam.buildQueryParam(name, value, op,
						type);
				list.add(param);
			}
		}
		return list;
	}

	public static String getOperator(String str) {
		if (str.startsWith(RelationalOperator.GE)) {
			return RelationalOperator.GE;
		} else if (str.startsWith(RelationalOperator.LE)) {
			return RelationalOperator.LE;
		} else if (str.startsWith(RelationalOperator.NE)) {
			return RelationalOperator.NE;
		} else if (str.startsWith(RelationalOperator.GT)) {
			return RelationalOperator.GT;
		} else if (str.startsWith(RelationalOperator.LT)) {
			return RelationalOperator.LT;
		} else if (str.startsWith(RelationalOperator.EQ)) {
			return RelationalOperator.EQ;
		} else if (str.startsWith(RelationalOperator.LIKE)) {
			return RelationalOperator.LIKE;
		}
		return null;
	}

	public static void setCurrentEmployee(EmployeeObj obj) {
		threadLocal.set(obj);
	}

	public static EmployeeObj getCurrentEmployee() {
		return threadLocal.get();
	}

	public static EmployeeObj getCurrentEmployee(HttpSession session) {
		return (EmployeeObj) session
				.getAttribute(WebKeys.Session_Attribute_Name_UserDNA);
	}

	public static String getIpAddr(HttpServletRequest request) {
		String ip = request.getHeader("x-forwarded-for");
		if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {
			ip = request.getHeader("Proxy-Client-IP");
		}
		if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {
			ip = request.getHeader("WL-Proxy-Client-IP");
		}
		if (ip == null || ip.length() == 0 || "unknown".equalsIgnoreCase(ip)) {
			ip = request.getRemoteAddr();
		}
		return ip;
	}

	@SuppressWarnings("unchecked")
	public static void outputRequestParameterAsScript(
			HttpServletRequest request, JspWriter pw, String variableName)
			throws IOException {
		pw.print("<script type=\"text/javascript\">");
		pw.print("Ext.ns('");
		pw.print(variableName);
		pw.print("');");
		pw.print(variableName);
		pw.print("=new com.bitc.xmis.Request(");
		Map<String, String[]> map = request.getParameterMap();
		String mapStr = JSONUtils.toJSON(map, true).toString();
		pw.print(mapStr);
		pw.print(");");
		pw.print("</script>");
	}

	public static Map<String, String> strToMap(String str) {
		logger.info("---> strToMap");
		Map<String, String> map = null;
		if (str != null && !"".equals(str.trim())) {
			map = new HashMap<String, String>();
			String[] tempObj = str.split(",");
			if (tempObj != null && tempObj.length > 0) {
				for (int i = 0; i < tempObj.length; i++) {
					String[] tmp = tempObj[i].split(":");
					map.put(tmp[0], tmp[1]);
				}
			}
		}
		return map;
	}

	public static List<List<String>> strToList(String str) {
		logger.info("---> strToList");
		List<List<String>> list = null;
		if (str != null && !"".equals(str.trim())) {
			list = new ArrayList<List<String>>();
			if (str.indexOf(",") == -1) {
				String[] tmp = str.split(":");
				List<String> listTemp = new ArrayList<String>();
				for (int j = 0; j < tmp.length; j++) {
					listTemp.add(tmp[j]);
				}
				list.add(listTemp);
			} else {
				String[] tempObj = str.split(",");
				if (tempObj != null && tempObj.length > 0) {
					for (int i = 0; i < tempObj.length; i++) {
						String[] tmp = tempObj[i].split(":");
						List<String> listTemp = new ArrayList<String>();
						for (int j = 0; j < tmp.length; j++) {
							listTemp.add(tmp[j]);
						}
						list.add(listTemp);
					}
				}
			}

		}
		return list;
	}

	public static String buildStoreForJsonWithoutPageInfo(List aList) {
		logger.info("---> buildStoreForJsonWithoutPageInfo" + aList.size());
		String totalStr = "{\"success\":\"true\",\"totalCount\":\"";
		StringBuilder store = new StringBuilder();
		if (aList != null && aList.size() > 0) {
			totalStr += String.valueOf(aList.size());
			StringBuilder row = new StringBuilder();
			for (Iterator i = aList.listIterator(); i.hasNext();) {
				Map map = (Map) i.next();
				int mapsize = map.size();
				Iterator it = map.entrySet().iterator();
				StringBuilder data = new StringBuilder();
				for (int j = 0; j < mapsize; j++) {
					Map.Entry entry = (Map.Entry) it.next();
					String key = EscapeUtils.escapeDoubleQuotationMarks(Type
							.getString(entry.getKey()));
					String value = EscapeUtils.escapeDoubleQuotationMarks(Type
							.getString(entry.getValue()));
					if (entry.getValue() instanceof Date) {
						SimpleDateFormat sdf = new SimpleDateFormat(
								"yyyy-MM-dd");
						value = sdf.format(entry.getValue());
					}
					data.append("\"").append(key)
							.append("\":\"").append(value).append("\"");
					if (j < mapsize - 1) {
						data.append(",");
					}
				}
				row.append("{").append(data).append("},");
			}
			row.deleteCharAt(row.length() - 1);
			store.append("[").append(row).append("]");
		} else {
			totalStr += "0";
			store.append("[]");
		}
		return new StringBuilder(totalStr).append("\",\"rows\":").append(store)
				.append("}").toString();
	}
	
	public static String buildStoreForJsonWithoutPageInfoWithDateFormat(List aList) {
		logger.info("---> buildStoreForJsonWithoutPageInfoWithDateFormat" + aList.size());
		String totalStr = "{\"success\":\"true\",\"totalCount\":\"";
		StringBuilder store = new StringBuilder();
		if (aList != null && aList.size() > 0) {
			totalStr += String.valueOf(aList.size());
			StringBuilder row = new StringBuilder();
			for (Iterator i = aList.listIterator(); i.hasNext();) {
				Map map = (Map) i.next();
				int mapsize = map.size();
				Iterator it = map.entrySet().iterator();
				StringBuilder data = new StringBuilder();
				for (int j = 0; j < mapsize; j++) {
					Map.Entry entry = (Map.Entry) it.next();
					String key = EscapeUtils.escapeDoubleQuotationMarks(Type
							.getString(entry.getKey()));
					String value = EscapeUtils.escapeDoubleQuotationMarks(Type
							.getString(entry.getValue()));
					if (entry.getValue() instanceof Date) {
						SimpleDateFormat sdf = new SimpleDateFormat(
						"yyyy-MM-dd HH:mm:ss");
						value = sdf.format(entry.getValue());
					}
					data.append("\"").append(key)
					.append("\":\"").append(value).append("\"");
					if (j < mapsize - 1) {
						data.append(",");
					}
				}
				row.append("{").append(data).append("},");
			}
			row.deleteCharAt(row.length() - 1);
			store.append("[").append(row).append("]");
		} else {
			totalStr += "0";
			store.append("[]");
		}
		return new StringBuilder(totalStr).append("\",\"rows\":").append(store)
		.append("}").toString();
	}

	public static List<StandardQuery> buildStandardQuery(
			List<QueryParam> paramList) {
		List<StandardQuery> querys = new ArrayList<StandardQuery>();
		if (paramList == null || paramList.size() == 0) {
			return querys;
		}
		for (Iterator<QueryParam> it = paramList.iterator(); it.hasNext();) {
			QueryParam param = it.next();
			querys.add(SimpleParam.buildSimpleParam(RelationalOperator.JDBCSQL,
					param.getCondition().getSql(), param.getValue()));
		}
		return querys;
	}

	/**
	 * @param sc
	 *            ServletContext
	 * @param fileName
	 *            文件名，包括扩展名
	 * @return
	 */
	public static File newTempFile(ServletContext sc, String fileName) {
		String directPath = (String) sc
				.getAttribute(WebKeys.Session_Application_Cfg_TEMPFILEPATH);
		String filePath = directPath + fileName;
		return new File(filePath);
	}

	public static String buildStoreForJsonWithPageInfoWithDateFormat(
			List aList, int totalcount) {
		logger.info("---> buildStoreForJsonWithPageInfoWithDateFormat"
				+ totalcount);
		String totalStr = "{\"success\":\"true\",\"totalCount\":\"";
		StringBuilder store = new StringBuilder();
		if (aList != null && totalcount > 0) {
			totalStr += String.valueOf(totalcount);
			StringBuilder row = new StringBuilder();
			for (Iterator i = aList.listIterator(); i.hasNext();) {
				Map map = (Map) i.next();
				int mapsize = map.size();
				Iterator it = map.entrySet().iterator();
				StringBuilder data = new StringBuilder();
				for (int j = 0; j < mapsize; j++) {
					Map.Entry entry = (Map.Entry) it.next();
					String key = EscapeUtils.escapeDoubleQuotationMarks(Type
							.getString(entry.getKey()));
					String value = EscapeUtils.escapeDoubleQuotationMarks(Type
							.getString(entry.getValue()));
					if (entry.getValue() instanceof Date) {
						SimpleDateFormat sdf = new SimpleDateFormat(
								"yyyy-MM-dd HH:mm:ss");
						value = sdf.format(entry.getValue());
					}
					data.append("\"").append(key).append("\":\"").append(value)
							.append("\"");
					if (j < mapsize - 1) {
						data.append(",");
					}
				}
				row.append("{").append(data).append("},");
			}
			row.deleteCharAt(row.length() - 1);
			store.append("[").append(row).append("]");
		} else {
			totalStr += "0";
			store.append("[]");
		}
		return new StringBuilder(totalStr).append("\",\"rows\":").append(store)
				.append("}").toString();
	}

}
