package com.bitc.jk.xmis.util;

import java.io.IOException;
import java.io.PrintWriter;
import java.util.HashMap;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.Map.Entry;

import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;

import org.apache.log4j.Logger;
import org.dom4j.Document;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;
import org.dom4j.io.OutputFormat;
import org.dom4j.io.XMLWriter;

import com.bitc.jk.xmis.model.PageInfo;

public class WebUtil {

	private static Logger logger = Logger.getLogger(WebUtil.class);

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
			if(DBSchema.Attachment.AttachmentName.equals(String.valueOf(entry.getKey())))
			urlStr = "^" + "/xMIS/fileUpload.do?type=3&attID="
					+ Type.getString(map.get(DBSchema.Attachment.AttachmentID));
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
				urlStr = "".equals(entry.getValue()) ? "" : "^"
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

	public static String buildStoreForJson(List aList, String frmName) {

		logger.info("---> buildGridStoreForJson");

		String store = "", totalStr = "{'success':'true','totalCount':'";

		if (aList != null && aList.size() > 0) {

			totalStr += String.valueOf(((HashMap) aList.get(0)).get("row_no"));

			aList.remove(0);

			String row = "";
			for (Iterator i = aList.listIterator(); i.hasNext();) {
				Map map = (HashMap) i.next();

				int mapsize = map.size();
				Iterator it = map.entrySet().iterator();

				String data = "";

				for (int j = 0; j < mapsize; j++) {
					Map.Entry entry = (Map.Entry) it.next();

					/* url link */
					String urlStr = buildURLLink(entry, map, frmName);

					data += "'"
							+ Type.getString(entry.getKey())
							+ "':'"
							+ (Type.getString(entry.getValue()).equals("null") ? ""
									: Type.getString(entry.getValue()) + urlStr)
							+ "',";

				}

				row += "{" + data.substring(0, data.length() - 1) + "},";
			}

			store += "[" + row.substring(0, row.length() - 1) + "]";

		} else {
			totalStr += "0";
			store = "[]";
		}

		//logger.info(store);
		
		logger.info(totalStr);
		
		return totalStr + "','rows':" + store + "}";

	}

	public static String buildStoreForJsonWithoutPageInfo(List aList,
			String frmName) {

		logger.info("---> buildStoreForJsonWithoutPageInfo" + aList.size());

		String store = "", totalStr = "{'success':'true','totalCount':'";
		
		if (aList != null && aList.size() > 0) {

			totalStr += String.valueOf(aList.size());

			String row = "";
			for (Iterator i = aList.listIterator(); i.hasNext();) {
				Map map = (HashMap) i.next();

				int mapsize = map.size();
				Iterator it = map.entrySet().iterator();

				String data = "";

				for (int j = 0; j < mapsize; j++) {
					Map.Entry entry = (Map.Entry) it.next();

					/* url link */
					String urlStr = buildURLLink(entry, map, frmName);

					data += "'"
							+ Type.getString(entry.getKey())
							+ "':'"
							+ (Type.getString(entry.getValue()).equals("null") ? ""
									: Type.getString(entry.getValue()) + urlStr)
							+ "',";

				}

				row += "{" + data.substring(0, data.length() - 1) + "},";
			}

			store += "[" + row.substring(0, row.length() - 1) + "]";

		} else {
			totalStr += "0";
			store = "[]";
		}
		
//		logger.info(totalStr + "','rows':" + store + "}");
		return totalStr + "','rows':" + store + "}";

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
					String.valueOf(((HashMap) aList.get(0)).get("row_no")));
			// itemsElement.addElement("TotalPage").setText("3");

			aList.remove(0);
			for (Iterator i = aList.listIterator(); i.hasNext();) {
				Element itemElement = rootElement.addElement("Item");

				Map map = (HashMap) i.next();

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
											.getValue())
											+ urlStr);
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
	// String.valueOf(((HashMap) aList.get(0)).get("row_no")));
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

		logger.info(message);
		try {
			// 返回成功标识
			response.setContentType("text/html; charset=utf-8");
			response.getWriter().write(message);
		} catch (IOException e) {
			e.printStackTrace();
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				response.getWriter().close();

			} catch (IOException e) {
				e.printStackTrace();
			}
		}
	}

	public static String generateSQLStr(String sqlStr, PageInfo pageInfo) {

		String orderByStr = (pageInfo.getSort() == null || "".equals(pageInfo
				.getSort())) ? "" : "\n order by " + pageInfo.getSort() + " "
				+ pageInfo.getDir();

//		logger.info(Type.getString(pageInfo.getStart()) + "//" + Type.getString(pageInfo.getLimit()));
		return (pageInfo.getStart() == 0 && pageInfo.getLimit() == 0) 
				? (sqlStr + SQLScript.resultStrByBiz + orderByStr + SQLScript.getResultStrWithoutPaging)
				: (sqlStr + SQLScript.resultStrByBiz + orderByStr + SQLScript.getResultStrWithPaging);
	}

	public static String generateSQLForPosition(String sqlStr,
			String columnName, PageInfo pageInfo) {

		String orderByStr = pageInfo.getSort() == null ? "" : "\n order by "
				+ pageInfo.getSort() + " " + pageInfo.getDir();

		return sqlStr + SQLScript.resultStrByBiz + orderByStr
				+ SQLScript.positionStr + " where " + columnName + "=?";
	}

	public static PageInfo generatePageInfo(HttpServletRequest request) {

		int start = new Integer(request.getParameter("start") != null ? request
				.getParameter("start") : "0").intValue();
		int limit = new Integer(request.getParameter("limit") != null ? request
				.getParameter("limit") : "0").intValue();
		String sort = request.getParameter("sort");
		String dir = request.getParameter("dir");
		
//		logger.info("==========" + sort + "==============");

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

		String store="", totalStr = "},";
		
		
		if (aList != null && aList.size() > 0) {
			String row = "";
			for (Iterator i = aList.listIterator(); i.hasNext();) {
				Map map = (HashMap) i.next();

				int mapsize = map.size();
				Iterator it = map.entrySet().iterator();

				String sumMsg = "";

				for (int j = 0; j < mapsize; j++) {
					Map.Entry entry = (Map.Entry) it.next();
					
					sumMsg += "'" + Type.getString(entry.getKey()) + "':'" + Type.getString(entry.getValue()) + "',";
					
				}
				row += "{" + sumMsg.substring(0, sumMsg.length() - 1) + "},";
				logger.info(row);

			}
			store += row.substring(0, row.length() - 1) + "]}";
			logger.info(store);
		} else {
			totalStr = "}]}";
		}

		//logger.info(store);
		
		logger.info(totalStr + store);
		
		return totalStr + store;

	}	
	
	public static int getCurrencyType(String currecnyGraphy){
		
		int currencyFlag = 0;
		
		if(WebKeys.Type_Currency_Graphy_RMB.equals(currecnyGraphy)){
			currencyFlag = WebKeys.Type_Currency_RMB;
		}else if(WebKeys.Type_Currency_Graphy_USD.equals(currecnyGraphy)){
			currencyFlag = WebKeys.Type_Currency_USD;
		}else if(WebKeys.Type_Currency_Graphy_Euro.equals(currecnyGraphy)){
			currencyFlag = WebKeys.Type_Currency_Euro;
		}else if(WebKeys.Type_Currency_Graphy_HKD.equals(currecnyGraphy)){
			currencyFlag = WebKeys.Type_Currency_HKD;
		}
		
		return currencyFlag;
	}
}
