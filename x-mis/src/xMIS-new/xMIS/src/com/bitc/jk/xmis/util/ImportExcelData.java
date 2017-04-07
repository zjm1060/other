package com.bitc.jk.xmis.util;

import java.awt.Point;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStream;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.List;
import java.util.Map;
import java.util.Set;
import java.util.Map.Entry;

import com.bitc.jk.xmis.model.CoordinateObj;

import jxl.Sheet;
import jxl.Workbook;
import jxl.read.biff.BiffException;

public class ImportExcelData {
	static String need_columns = ReadPropertyFile
			.getValueByParam("need_columns");
	static String common_columns = ReadPropertyFile
			.getValueByParam("common_columns");
	static String all_columns = need_columns.concat(",").concat(common_columns);

	/**
	 * 是否是数字
	 * 
	 * @param str
	 * @return
	 */
	public static boolean isNumeric(String str) {
		// for (int i = str.length()-1;i>0;i--){
		str = str.trim();
		for (int i = str.length(); --i >= 0;) {
			if (!Character.isDigit(str.charAt(i))) {
				return false;
			}
		}
		return true;
	}

	/**
	 * 得到数据起始的行
	 * 
	 * @param sheet
	 * @param r
	 * @param tempcolumn
	 * @param rowNum
	 * @return
	 */
	public static int getStartRowNum(Sheet sheet) {
		int startRowNum = 0;
		int columnNum = sheet.getColumns();
		int rowNum = sheet.getRows();

		for (int c = 0; c < columnNum; c++) {
			for (int r = 0; r < rowNum; r++) {
				String comments = sheet.getCell(c, r).getContents();
				if (comments.equals("序号")) {

					for (int j = r + 1; j < rowNum; j++) {
						if (!sheet.getCell(c, j).getContents().equals("")) {
							return j;
						}
					}
				}

			}
		}
		return startRowNum;
	}

	/**
	 * 某一个列是否存在
	 * 
	 * @param wb
	 * @param columnName
	 * @return
	 */
	public static boolean isExist(Sheet sheet, String columnName) {
		int columnNum = sheet.getColumns();
		int rowNum = sheet.getRows();
		if (rowNum > 5) {
			rowNum = 5;
		}
		for (int c = 0; c < columnNum; c++) {
			for (int r = 0; r < rowNum; r++) {
				String comments = sheet.getCell(c, r).getContents();
				if (comments.equals(columnName)) {
					return true;
				}

			}
		}

		return false;
	}

	/**
	 * 列名和别名是否只出现一次
	 * 
	 * @return
	 */
	public static boolean isExistOnce(Sheet sheet) {
		// 验证必须存在的列
		if (need_columns != null) {
			String[] arr = need_columns.split(",");
			for (int j = 0; j < arr.length; j++) {
				String[] columnsArray = arr[j].split("=");
				if (columnsArray.length > 1) {
					int trueNums = 0;
					for (int k = 0; k < columnsArray.length; k++) {
						if (isExist(sheet, columnsArray[k])) {// 有别名的列的验证：存在，并且只出现一次
							trueNums += 1;
						}
					}
					if (trueNums == 1) {
						return true;
					} else {
						return false;
					}
				} else {
					int trueNums = 0;
					if (isExist(sheet, arr[j])) {// 无别名的列的验证：存在
						trueNums += 1;
					}
					if (trueNums == 1) {
						return true;
					} else {
						return false;
					}
				}
			}
		}
		// 验证普通列
		if (common_columns != null) {
			String[] arr = common_columns.split(",");
			for (int j = 0; j < arr.length; j++) {
				String[] columnsArray = arr[j].split("=");
				if (columnsArray.length > 1) {
					int trueNums = 0;
					for (int k = 0; k < columnsArray.length; k++) {
						if (isExist(sheet, columnsArray[k])) {// 有别名的列的验证：存在，并且只出现一次
							trueNums += 1;
						}
					}
					if (trueNums <= 1) {
						return true;
					}
				}
			}
		}

		return false;
	}

	public static String existOnceErrorMsg(Sheet sheet) {
		String msg = "";
		// 验证必须存在的列
		if (need_columns != null) {
			String[] arr = need_columns.split(",");
			for (int j = 0; j < arr.length; j++) {
				String[] columnsArray = arr[j].split("=");
				if (columnsArray.length > 1) {
					int trueNums = 0;
					for (int k = 0; k < columnsArray.length; k++) {
						if (isExist(sheet, columnsArray[k])) {// 有别名的列的验证：存在，并且只出现一次
							trueNums += 1;
						}
					}
					if (trueNums == 1) {
					} else if (trueNums > 1) {
						msg += columnsArray[0] + "列及别名同时出现；";
					} else {
						msg += columnsArray[0] + "列不存在；";
						
					}
				} else {
					int trueNums = 0;
					if (isExist(sheet, arr[j])) {// 无别名的列的验证：存在
					}
					  else {
						msg += arr[j] + "列不存在；";

					}

				}
			}
		}
		// 验证普通列
		if (common_columns != null) {
			String[] arr = common_columns.split(",");
			for (int j = 0; j < arr.length; j++) {
				String[] columnsArray = arr[j].split("=");
				if (columnsArray.length > 1) {
					int trueNums = 0;
					String msg1 = "";
					for (int k = 0; k < columnsArray.length; k++) {
						if (isExist(sheet, columnsArray[k])) {// 有别名的列的验证：存在，并且只出现一次
							trueNums += 1;
							msg1=columnsArray[k] + "列重复出现";
						}
					}
					if (trueNums > 1) {
						msg = msg+msg1;
					}
				}
			}
		}
//		System.out.println(msg);
		return msg;
	}

	/**
	 * 判断是否存在指定的列同列的情况
	 * 
	 * @param columnInfoList
	 * @return
	 */
	public static boolean isNotInSameColumn(
			List<Map<String, Integer>> columnInfoList, List<String> columnList) {
		List<Integer> numList = new ArrayList<Integer>();
		for (int i = 0; i < columnInfoList.size(); i++) {
			for (int j = 0; j < columnList.size(); j++) {
				Integer value = columnInfoList.get(i).get(columnList.get(j));
				if (value != null) {
					numList.add(value);
				}
			}
			int listSize = columnInfoList.size();
			Set<Integer> sets = new HashSet<Integer>();

			for (Integer item : numList) {
				sets.add(item);
			}
			int numListSize = sets.size();
			if (listSize == numListSize) {
				return true;
			}
		}
		return false;
	}

	public static String inSameColumn(
			List<Map<String, Integer>> columnInfoList, List<String> columnList) {
		String msg = "";
		List<Integer> numList = new ArrayList<Integer>();
		for (int i = 0; i < columnInfoList.size(); i++) {
			for (int j = 0; j < columnList.size(); j++) {
				Integer value = columnInfoList.get(i).get(columnList.get(j));
				if (value != null) {
					numList.add(value);
				}
			}

		}
		// 行数、出现的次数
		Map<Integer, Integer> countMap = new HashMap<Integer, Integer>();
		for (int i = 0; i < numList.size(); i++) {
			Integer data = numList.get(i);
			if (countMap.get(data) == null) {
				countMap.put(data, 1);
			} else {
				countMap.put(data, countMap.get(data) + 1);
			}
		}
		// 重复列
		Iterator iter = countMap.entrySet().iterator();
		List<Integer> repeatStrList = new ArrayList<Integer>();
		while (iter.hasNext()) {
			Map.Entry a = (Map.Entry) iter.next();
			if (Type.GetInt(a.getValue()) > 1) {
				repeatStrList.add(Type.getInteger(a.getKey()));
			}
		}

		if (repeatStrList != null && repeatStrList.size() > 0) {
			for (int j = 0; j < repeatStrList.size(); j++) {
				msg += Type.getString(repeatStrList.get(j)) + ",";
			}
			if (msg.length() > 0) {
				msg = msg.substring(0, msg.length() - 1);
				msg = msg + "列存在多个列";
			}

		}
		return msg;
	}

	/**
	 * 列的解析验证
	 * 
	 * @param wb
	 * @return
	 */
	public static List<String> getValidColumnList(Sheet sheet) {
		List<String> validColumnList = new ArrayList<String>();
		if (need_columns != null) {
			String[] arr = need_columns.split(",");
			for (int j = 0; j < arr.length; j++) {
				String[] columnsArray = arr[j].split("=");
				if (columnsArray.length > 1) {
					for (int k = 0; k < columnsArray.length; k++) {
						if (isExist(sheet, columnsArray[k])) {// 有别名的列
							validColumnList.add(columnsArray[k]);
						}
					}
				} else {
					if (isExist(sheet, arr[j])) {// 无别名的列
						validColumnList.add(arr[j]);
					}
				}
			}
		}
		// 验证普通列
		if (common_columns != null) {
			String[] arr = common_columns.split(",");
			for (int j = 0; j < arr.length; j++) {
				String[] columnsArray = arr[j].split("=");
				if (columnsArray.length > 1) {
					for (int k = 0; k < columnsArray.length; k++) {
						if (isExist(sheet, columnsArray[k])) {// 有别名的列
							validColumnList.add(columnsArray[k]);
						}

					}
				} else {
					if (isExist(sheet, arr[j])) {// 无别名的列
						validColumnList.add(arr[j]);
					}
				}
			}
		}

		return validColumnList;
	}

	/**
	 * 得到列名、和坐标列 从前5行
	 * 
	 * @param wb
	 * @param columnnameList
	 * @return
	 */
	public static List<Map<String, Integer>> getColumnNameAndColumnNum(
			Sheet sheet, List<String> columnnameList) {
		List<Map<String, Integer>> columnMap = new ArrayList<Map<String, Integer>>();
		int columnNum = sheet.getColumns();
		int rowNum = sheet.getRows();
		if (rowNum > 5) {
			rowNum = 5;
		}
		for (int c = 0; c < columnNum; c++) {
			for (int r = 0; r < rowNum; r++) {
				String comments = sheet.getCell(c, r).getContents();
				for (int j = 0; j < columnnameList.size(); j++) {
					String columnName = columnnameList.get(j).toString();
					if (comments.equals(columnName)) {
						Map<String, Integer> map1 = new HashMap<String, Integer>();
						map1.put(columnName, c);
						columnMap.add(map1);
					}
				}
			}
		}
		return columnMap;
	}

	/**
	 * 得到坐标列 从前5行
	 * 
	 * @param wb
	 * @param columnnameList
	 * @return
	 */
	public static List<Integer> getColumnNum(Sheet sheet,
			List<String> columnnameList) {
		List<Integer> columnList = new ArrayList<Integer>();
		int columnNum = sheet.getColumns();
		int rowNum = sheet.getRows();
		if (rowNum > 5) {
			rowNum = 5;
		}
		for (int c = 0; c < columnNum; c++) {
			for (int r = 0; r < rowNum; r++) {
				String comments = sheet.getCell(c, r).getContents();
				for (int j = 0; j < columnnameList.size(); j++) {
					String columnName = columnnameList.get(j).toString();
					if (comments.equals(columnName)) {
						columnList.add(c);
					}
				}
			}
		}
		return columnList;
	}

	public static String getNameByAlias(String alias) {

		// 如果有别名的情况，将列名替换
		String[] arr = all_columns.split(",");
		for (int j1 = 0; j1 < arr.length; j1++) {
			String[] columnsArray = arr[j1].split("=");
			if (columnsArray.length > 1) {
				for (int k = 0; k < columnsArray.length; k++) {
					// 有别名的列
					if (alias.equals(columnsArray[k])) {
						return columnsArray[0];

					}
				}

			} else {
				if (alias.equals(columnsArray[0])) {
					return alias;
				}
			}
		}
		return "";

	}

	/**
	 * 得到坐标列名字 从前5行
	 * 
	 * @param wb
	 * @param columnnameList
	 * @return
	 */
	public static List<String> getColumnName(Sheet sheet,
			List<String> columnnameList) {
		List<String> columnList = new ArrayList<String>();
		int columnNum = sheet.getColumns();
		int rowNum = sheet.getRows();
		if (rowNum > 5) {
			rowNum = 5;
		}
		for (int c = 0; c < columnNum; c++) {
			for (int r = 0; r < rowNum; r++) {
				String comments = sheet.getCell(c, r).getContents();
				for (int j = 0; j < columnnameList.size(); j++) {
					String columnName = columnnameList.get(j).toString();
					if (comments.equals(columnName)) {
						columnList.add(getNameByAlias(columnName));
					}
				}
			}
		}

		for (String s : columnList) {
			// System.out.println(s);
		}
		return columnList;
	}

	public static int getFrontRow(Sheet sheet, int c, int r) {
		if (sheet.getCell(c, r - 1).getContents() == null
				|| sheet.getCell(c, r - 1).getContents().equals("")) {
			for (int i = r - 1; i > 0; r--) {
				return getFrontRow(sheet, c, i - 1);
			}
		}
		return r;
	}

	/**
	 * 对序号列分级标识
	 * 
	 * @param comments
	 * @return
	 */
	public static String formatLevelFlag(String comments, Sheet sheet, int c,
			int r) {
		if (comments.indexOf("（") > -1 || comments.indexOf("）") > -1
				|| comments.indexOf("(") > -1 || comments.indexOf(")") > -1) {
			comments = comments.replace("（", "");
			comments = comments.replace("）", "");
			comments = comments.replace("(", "");
			comments = comments.replace(")", "");

			String front = sheet.getCell(c, r - 1).getContents();
			if (front == null || front.equals("")) {
				int row = getFrontRow(sheet, c, r);
				front = sheet.getCell(c, row - 1).getContents();
			}

			if (front.indexOf("（") > -1 || front.indexOf("）") > -1
					|| front.indexOf("(") > -1 || front.indexOf(")") > -1) {
				front = formatLevelFlag(front, sheet, c, r - 1);
				if (front.length() > 3) {
					front = front.substring(0, front.length() - 3);
					comments = front.concat(formatLevel(comments));
				} else {
					comments = formatLevel(comments);
				}
			} else {
				front = formatLevel(front);
				comments = front.concat(formatLevel(comments));
			}
		} else {
			comments = formatLevel(comments);
		}
		return comments;
	}

	/**
	 * 对序号列格式化
	 * 
	 * @param comments
	 * @return
	 */
	public static String formatLevel(String comments) {
		comments = comments.replace("、", ",");
		comments = comments.replace(",", ",");
		comments = comments.replace("，", ",");
		comments = comments.replace("。", ",");
		comments = comments.replace(".", ",");
		comments = comments.replace("．", ",");

		comments = comments.replace("（", "");
		comments = comments.replace("）", "");
		comments = comments.replace("(", "");
		comments = comments.replace(")", "");
 
		if (comments != null ) {
			// 分隔，补位
			comments=Type.toDBC(comments);//全角转半角
			if (comments.indexOf(",") > -1) {
				String[] array = comments.split(",");
				String formatStr = "";
				for (int i = 0; i < array.length; i++) {
					if (isNumeric(array[i])) {
						int length = array[i].trim().length();
						String temp = "";
						for (int j = 0; j < 3 - length; j++) {
							temp = temp.concat("0");
						}
						array[i] = temp.concat(array[i].trim());
						formatStr = formatStr.concat(array[i]);
					}
				}
				comments = formatStr;
			} else {
				if (isNumeric(comments)) {
					String temp = "";
					int length = comments.length();
					for (int j = 0; j < 3 - length; j++) {
						temp = temp.concat("0");
					}
					comments = temp.concat(comments);
				} else {
					comments = "";
				}
			}
		} else {
			comments = "";
		}
		comments = comments.replace(",", ".");
		return comments;
	}

	/**
	 * sheet中 获取某一个坐标所在合并区域
	 * 
	 * @param coordinateObj
	 * @param dataPoint
	 * @return
	 */

	private static Point getMergeArea(List<CoordinateObj> coordinateObjList,
			Point dataPoint) {
		Point resultPoint = new Point();
		for (CoordinateObj coordinateObj : coordinateObjList) {
			if (dataPoint.x >= coordinateObj.getMinx()
					&& dataPoint.y >= coordinateObj.getMiny()
					&& dataPoint.x <= coordinateObj.getMaxx()
					&& dataPoint.y <= coordinateObj.getMaxy()) {
				resultPoint.setLocation(coordinateObj.getMinx(),
						coordinateObj.getMiny());
				return resultPoint;
			}
		}
		return resultPoint;
	}

	private static boolean isInMergeArea(List<CoordinateObj> coordinateObjList,
			Point dataPoint) {
		for (CoordinateObj coordinateObj : coordinateObjList) {
			if (dataPoint.x >= coordinateObj.getMinx()
					&& dataPoint.y >= coordinateObj.getMiny()
					&& dataPoint.x <= coordinateObj.getMaxx()
					&& dataPoint.y <= coordinateObj.getMaxy()) {

				return true;
			}
		}
		return false;
	}

	/**
	 * 获取某一个坐标所在合并区域的值
	 * 
	 * @param sheet
	 * @param dataPoint
	 * @return
	 */
	public static String getMergeAreaValue(Sheet sheet, Point dataPoint) {
		jxl.Range[] ranges = sheet.getMergedCells();
		List<CoordinateObj> coordinateObjLists = new ArrayList<CoordinateObj>();
		for (jxl.Range space : ranges) {
			CoordinateObj coordinateObj = new CoordinateObj();
			coordinateObj.setMinx(space.getTopLeft().getColumn());
			coordinateObj.setMiny(space.getTopLeft().getRow());
			coordinateObj.setMaxx(space.getBottomRight().getColumn());
			coordinateObj.setMaxy(space.getBottomRight().getRow());
			coordinateObjLists.add(coordinateObj);

			Point resultPoint = new Point();
			if (isInMergeArea(coordinateObjLists, dataPoint)) {
				resultPoint = getMergeArea(coordinateObjLists, dataPoint);
				return sheet.getCell(resultPoint.x, resultPoint.y)
						.getContents();
			}
		}
		return "";

	}

	/**
	 * 存储所有的列、及数据信息，包括合并的单元格处理；
	 * 
	 * @param sheet
	 * @param nameList
	 * @param numlist
	 * @param columnNameAndColumnNumMap
	 * @return
	 */
	private static List<String[]> collectAllDataList(Sheet sheet,
			List<String> nameList, List<Integer> numlist,
			List<Map<String, Integer>> columnNameAndColumnNumMap) {
		List<String[]> dataList = new ArrayList<String[]>();
		int arraylength = nameList.size();

		int startRow = getStartRowNum(sheet);
		int temp = startRow;
		int rowNum = sheet.getRows();

		for (int r = startRow; r < rowNum; r++) {
			String[] dataArray = new String[nameList.size() + 2];// 增加两列行号、level_id
			temp += 1;
			for (int j = 0; j < numlist.size(); j++) {
				String comments = sheet.getCell(numlist.get(j), r)
						.getContents();
				dataArray[j] = comments;
				Point dataPoint = new Point();
				dataPoint.setLocation(numlist.get(j), r);
				// 判断为空的是否在合并的单元格里
				if(comments.equals(WebKeys.BBZ_SUM)){
					continue;
				}
				if (comments.equals("")) {
					comments = getMergeAreaValue(sheet, dataPoint);
					dataArray[j] = comments;
				} else if (Type.getString(
						sheet.getCell(numlist.get(j), r).getType()).equals(
						"Numerical Formula")
						|| Type.getString(
								sheet.getCell(numlist.get(j), r).getType())
								.equals("Number")) {
					comments = comments.replace(",", "");
					dataArray[j] = comments;

				} else {
					comments = comments.replace("\n", "");
					comments = comments.replace("\"", "’");
					comments = comments.replace("'", "’");
					dataArray[j] = comments;

				}
				dataArray[arraylength] = temp + "";

			}
			dataList.add(dataArray);
		}
		// System.out.println(" 起始行号:" + (startRow + 1));
		// System.out.println(" 最大行号:" + rowNum);
		for (String[] array : dataList) {
			for (String str : array) {
//				 System.out.println("----dataList------  " + str);
//				 System.out.println("----dataList------  " );
			}
		}
		for (String str : nameList) {
//			 System.out.println("----nameList------  "+str);
		}
		return dataList;
	}
	private static String getFirstColumnNullMsg(
			List<String[]> dataList, List<String> nameList) {
		String message = "";
		List<String> rowNumberList = new ArrayList<String>();

		int numIndex = 0;// 序号列
		int rowIndex = nameList.size();// 行数所在的列
		for (int j = 0; j < nameList.size(); j++) {
			if (nameList.get(j).equals("序号")) {
				numIndex = j;
			}
		}
		for (int i = 0; i < dataList.size(); i++) {
			String xuhaocomments = dataList.get(i)[numIndex];
			String rowcomments = dataList.get(i)[rowIndex];
			// 判断为空
			if (xuhaocomments.equals("")) {
				rowNumberList.add(rowcomments);
			}   
			 
		}
		for(String s:rowNumberList){
			message += s+",";
		}
		if(message.indexOf(",")>-1){
			message = message.substring(0, message.length()-1);
		}
//		System.out.println( "<br/>请检查"+message+"行序号列为空");
		if(!message.equals("")){
			message = "<br/>请检查"+message+"行序号列为空;";
		}
		return message;
	}

	/**
	 * 某一条“序号”列的不合法数据
	 * 
	 * @param dataList
	 * @param nameList
	 * @return
	 */
	private static String getFistColumnDataErrorMessage(String comments,
			String rownum) {
		
		String message = "";
		comments = comments.replace("、", ",");
		comments = comments.replace(",", ",");
		comments = comments.replace("，", ",");
		comments = comments.replace("，", ",");
		comments = comments.replace("。", ",");
		comments = comments.replace(".", ",");
		comments = comments.replace("．", ",");

		comments = comments.replace("（", "");
		comments = comments.replace("）", "");
		comments = comments.replace("(", "");
		comments = comments.replace(")", "");
		// 分隔，补位
		if(comments.equals(WebKeys.BBZ_SUM)){
			return "";
		}else if (comments.indexOf(",") > -1) {
			String[] array = comments.split(",");
			for (int i = 0; i < array.length; i++) {
				if (!isNumeric(array[i])) {
					message = "第" + rownum + "行的序号列存在非数字;";
				}
			}
		} else {
			if (!isNumeric(comments)) {
				message = "第" + rownum + "行的序号列存在非数字;";
			}
		}
		return message;
	}

	/**
	 * 遍历，查找“序号”列的不合法数据
	 * 
	 * @param dataList
	 * @param nameList
	 * @return
	 */
	private static String getFistColumnDataErrorMessages(
			List<String[]> dataList, List<String> nameList) {
		int numIndex = 0;// 序号列
		for (int j = 0; j < nameList.size(); j++) {
			if (nameList.get(j).equals("序号")) {
				numIndex = j;
			}
		}
		String message = "";
		// 找到序号列，最后一列行数
		for (int i = 0; i < dataList.size(); i++) {
			String messageStr = getFistColumnDataErrorMessage(
					dataList.get(i)[numIndex], dataList.get(i)[nameList.size()]);
			message = message.concat(messageStr);
		}
		if(!message.equals("")){
			message = "<br/>"+message;
		}
		return message;
	}

	/**
	 * 是否是末级的验证
	 * 
	 * @param firstComments
	 * @param nextFirstComments
	 * @return
	 */
	private static boolean isLastGrade(String firstComments,
			String nextFirstComments) {
		if (nextFirstComments.length() == 0
				|| nextFirstComments.substring(0,
						nextFirstComments.length() - 3).equals(firstComments)) {
			return false;
		}
		return true;
	}

	/**
	 * 对是末级列“shul”列的验证
	 * 
	 * @param dataList
	 * @param nameList
	 * @return
	 */
	private static String getNumberColumnDataErrorMessages(
			List<Map<String, String>> dataList, List<String> nameList) {

		int numIndex2 = 0;// 数量列
		for (int j = 0; j < nameList.size(); j++) {
			if (nameList.get(j).equals("数量")) {
				numIndex2 = j;
			}
		}
		String message = "";
		// 解析末级，验证数量
		// 找到WebKeys.LEVEL_ID列，最后一列行数
		for (int i = 0; i < dataList.size(); i++) {
			String firstComments = dataList.get(i).get(WebKeys.LEVEL_ID);
			String nextFirstComments = "";
			if (i == dataList.size() - 1) {
				nextFirstComments = "";
			} else {
				nextFirstComments = dataList.get(i + 1).get(WebKeys.LEVEL_ID);
			}
			String nunmberComments = dataList.get(i).get(
					nameList.get(numIndex2));
			String rownumber = dataList.get(i).get(WebKeys.ROW);
			boolean isLastGrade = isLastGrade(firstComments, nextFirstComments);
			if (isLastGrade) {
				if (nunmberComments.equals("")) {
					message = message.concat("第" + rownumber + "行数量为空；");
				}
			}

		}
		if(!message.equals("")){
			message = "<br/>"+message;
		}
		return message;
	}

	private static String getMoneyColumnDataErrorMessages(
			List<Map<String, String>> dataList, List<String> nameList) {
		String message = "";

		Map<String, String> map = dataList.get(0);
		String danjia = map.get("合同单价");
		String zongjia = map.get("合同合价");
		if (danjia == null || zongjia == null || danjia.equals("")
				|| zongjia.equals("")) {
			message = "";
		} else {
			int numIndex = Type.GetInt(danjia);// 合同单价列
			int numIndex2 = Type.GetInt(map.get("数量"));// 数量列
			int numIndex3 = Type.GetInt(zongjia);// 合计列

			// 找到单位列，最后一列行数
			for (int i = 0; i < dataList.size(); i++) {
				String rownumber = dataList.get(i).get(WebKeys.ROW);
				String data1 = dataList.get(i).get(nameList.get(numIndex));
				String data2 = dataList.get(i).get(nameList.get(numIndex2));
				String data3 = dataList.get(i).get(nameList.get(numIndex3));
				int result = Type.GetInt(data1.trim())
						* Type.GetInt(data2.trim());
				int data3Result = Type.GetInt(data3 != null ? data3.trim()
						: data3);

				if (result != data3Result) {
					message = message.concat("第" + rownumber + "行数据"
							+ data3Result + "与系统计算的合价（总价）" + result
							+ "有误差；<br>");
				}

			}
		}
		if(!message.equals("")){
			message = "<br/>"+message;
		}
		return message;
	}

	/**
	 * 序号列的格式化为分级标识、为空的处理
	 * 
	 * @param dataList
	 * @param nameList
	 * @return
	 */
	private static List<Map<String, String>> formatFirstColumnData(Sheet sheet,
			List<String[]> dataList, List<String> nameList) {
		List<String[]> notValidList = new ArrayList<String[]>();
		int numIndex = 0;// 序号列
		for (int j = 0; j < nameList.size(); j++) {
			if (nameList.get(j).equals("序号")) {
				numIndex = j;
			}
		}
		int rowIndex = nameList.size();// 行数所在的列
		int levelIndex = nameList.size() + 1;// level_id_原序号所在的列

		// 找到序号列，最后一列行数
		for (int i = 0; i < dataList.size(); i++) {
			String comments = dataList.get(i)[numIndex];
			int row = Type.GetInt(dataList.get(i)[rowIndex]);
			int column = numIndex;
			if (!comments.equals("")&&!comments.equals(WebKeys.BBZ_SUM)) {
				dataList.get(i)[levelIndex] = formatLevelFlag(comments, sheet,
						column, row - 1);
				dataList.get(i)[numIndex] = comments;

			} else {
				notValidList.add(dataList.get(i));
			}
		}
		dataList.removeAll(notValidList);

		List<Map<String, String>> mapList = new ArrayList<Map<String, String>>();
		for (int i = 0; i < dataList.size(); i++) {
			Map<String, String> dataMap = new HashMap<String, String>();
			for (int j = 0; j <= levelIndex; j++) {
				if (j == rowIndex) {
					dataMap.put(WebKeys.ROW, dataList.get(i)[j]);
				} else if (j == levelIndex) {
					dataMap.put(WebKeys.LEVEL_ID, dataList.get(i)[j]);
				} else {
					dataMap.put(nameList.get(j), dataList.get(i)[j]);
				}
			}
			mapList.add(dataMap);
		}

//		 for (Map<String, String> map : mapList) {
//		 Iterator iter = map.entrySet().iterator();
//		 while (iter.hasNext()) {
//		 Map.Entry a = (Map.Entry) iter.next();
//		 System.out.println(a.getKey());
//		 System.out.println(a.getValue());
//		 }
//		 }
		return mapList;
	}

	/**
	 * 判断级次是否有重复
	 * 
	 * @param dataListFormat
	 * @return
	 */
	private static boolean isNotSameFormatFirstColumnData(
			List<Map<String, String>> dataListFormatList, List<String> nameList) {
		Set<String> sets = new HashSet<String>();
		for (int i = 0; i < dataListFormatList.size(); i++) {
			String data = dataListFormatList.get(i).get(WebKeys.LEVEL_ID);
			sets.add(data);
		}

		int listSize = dataListFormatList.size();

		int numListSize = sets.size();
		if (listSize == numListSize) {
			return true;
		}

		return false;
	}

	private static String sameFormatFirstColumnInfo(
			List<Map<String, String>> dataListFormatList, List<String> nameList) {
		String result = "";
		List<Map<String, Integer>> countMapList = new ArrayList<Map<String, Integer>>();
		Map<String, Integer> countMap = new HashMap<String, Integer>();
		for (int i = 0; i < dataListFormatList.size(); i++) {
			String data = dataListFormatList.get(i).get(WebKeys.LEVEL_ID);
			if (countMap.get(data) == null) {
				countMap.put(data, 1);
			} else {
				countMap.put(data, countMap.get(data) + 1);
			}
		}
		Iterator iter = countMap.entrySet().iterator();
		List<String> repeatStrList = new ArrayList<String>();
		while (iter.hasNext()) {
			Map.Entry a = (Map.Entry) iter.next();
			if (Type.GetInt(a.getValue()) > 1) {
				repeatStrList.add(Type.getString(a.getKey()));
			}
		}
		for (int j = 0; j < repeatStrList.size(); j++) {
			List<String> repeatPositionList = new ArrayList<String>();
			Map<String, String> repeatPositionMap = new HashMap<String, String>();
			for (int i = 0; i < dataListFormatList.size(); i++) {
				String data = dataListFormatList.get(i).get(WebKeys.LEVEL_ID);
				String rowNum = dataListFormatList.get(i).get(WebKeys.ROW);
				if (data.equals(repeatStrList.get(j))) {
					repeatPositionList.add(rowNum);
					if (repeatPositionMap.get(data) == null) {
						repeatPositionMap.put(data, rowNum);

					} else {
						repeatPositionMap.put(data, repeatPositionMap.get(data)
								.concat("、").concat(rowNum));
					}
				}
			}
			Iterator iter1 = repeatPositionMap.entrySet().iterator();
			while (iter1.hasNext()) {
				Map.Entry a = (Map.Entry) iter1.next();
				result += Type.getString(a.getValue()) + "行序号重复出现;";
			}

		}

		if(!result.equals("")){
			result = "<br/>"+result;
		}
		return result;
	}

	public static boolean isTrueOrder(String str, String lastStr) {
		if (lastStr.length() - str.length() == 3) {
			if (Type.GetInt(lastStr) - Type.GetInt(str) == 1) {
				return true;
			} else {
				return false;
			}
		} else {
			String childStr = lastStr.substring(3, lastStr.length());
			if (childStr.equals("001")) {
				return true;
			} else {
				return false;
			}
		}
	}

	public static String getOrderErrorMessage(
			List<Map<String, String>> dataListFormatList) {
		for (int i = 0; i < dataListFormatList.size(); i++) {
			if (i != dataListFormatList.size() - 1) {
				boolean isTrueOrder = isTrueOrder(dataListFormatList.get(i)
						.get(WebKeys.LEVEL_ID),
						dataListFormatList.get(i).get(WebKeys.LEVEL_ID));
			}
			String data = dataListFormatList.get(i).get(WebKeys.LEVEL_ID);
			String rowNum = dataListFormatList.get(i).get(WebKeys.ROW);

		}

		return "";
	}

	/**
	 * 列的解析；
	 * 
	 * @param sheet
	 * @return
	 */
	public static boolean validColumn(Sheet sheet) {
		// 验证必须存在的列
		boolean isExistOnce = isExistOnce(sheet);
		if (isExistOnce) {
			// 列的解析
			List<String> columnList = getValidColumnList(sheet);
			// 查找关键字的列，范围仅限于前5行，确定每个列头所在的列；
			List<Map<String, Integer>> columnNameAndColumnNumMap = getColumnNameAndColumnNum(
					sheet, columnList);
			// 判断是否存在指定的列同列的情况
			boolean isNotInSameColumn = isNotInSameColumn(
					columnNameAndColumnNumMap, columnList);
			if (isNotInSameColumn) {
				return true;
			}
		}
		return false;
	}


	public static Map<Boolean, String> validColumnMsgMap(Sheet sheet) {
		Map<Boolean, String> resultMap = new HashMap<Boolean, String>();
		// 验证必须存在的列
		String msgExistOnce = existOnceErrorMsg(sheet);
		// 列的解析
		List<String> columnList = getValidColumnList(sheet);
		// 查找关键字的列，范围仅限于前5行，确定每个列头所在的列；
		List<Map<String, Integer>> columnNameAndColumnNumMap = getColumnNameAndColumnNum(
				sheet, columnList);
		// 判断是否存在指定的列同列的情况

		String msgInSameColumn = inSameColumn(columnNameAndColumnNumMap,
				columnList);
		if ((msgExistOnce != null&& !msgExistOnce.equals("") ) ||( msgInSameColumn != null
				&& !msgInSameColumn.equals(""))) {
			resultMap.put(false, msgExistOnce + msgInSameColumn);
		} else {
			resultMap.put(true, "");
		}
		return resultMap;
	}

	/**
	 * 1、列的解析；2、存储所有的列、及数据信息，包括合并的单元格处理； 3、对不合法的序号列进行提示；
	 * 4、格式化“序号”列为分级标识；判断级次是否有重复5、对末级的“数量”列进行校验提示；6、通过验证后，将数据按行做导入；
	 * 
	 * @param sheetIndexArray
	 * 
	 * 
	 */
	@SuppressWarnings("unchecked")
	public static Map<Boolean, Object> validExcelColumn(String filePath,
			String[] sheetIndexArray) {
		Map<Boolean, Object> messageMap = new HashMap<Boolean, Object>();
		InputStream is = null;
		try {
			is = new FileInputStream(filePath);
			Workbook wb = Workbook.getWorkbook(is);

			List<Object[]> resultList = new ArrayList<Object[]>();
			for (int i = 0; i < sheetIndexArray.length; i++) {
				String arrayStr = sheetIndexArray[i];
				// System.out.println(arrayStr.substring(2,arrayStr.length()));
				int index = Type.getInteger(arrayStr.substring(0, arrayStr.indexOf("^")));
				Sheet sheet = wb.getSheet(index);
				String sheetName = sheet.getName();
				// 列的解析
				Map<Boolean, String> validMsgMap = validColumnMsgMap(sheet);
				Iterator iter = validMsgMap.entrySet().iterator();
				while (iter.hasNext()) {
					Map.Entry a = (Map.Entry) iter.next();
					boolean key = Type.getBoolean(a.getKey());
					String value = Type.getString(a.getValue());
					// validColumn(sheet)
					if (key) {
						List<String> columnList = getValidColumnList(sheet);
						List<Map<String, Integer>> columnNameAndColumnNumMap = getColumnNameAndColumnNum(
								sheet, columnList);
						List<Integer> numlist = getColumnNum(sheet, columnList);
						List<String> nameList = getColumnName(sheet, columnList);// 有顺序的
						
						// 存储所有的列、及数据信息，包括合并的单元格处理；
						List<String[]> dataList = collectAllDataList(sheet,
								nameList, numlist, columnNameAndColumnNumMap);
						String firstColumnNullMsg= getFirstColumnNullMsg(
								dataList, nameList);
						// 对不合法的序号列进行提示；
						String fistColumnMessage = getFistColumnDataErrorMessages(
								dataList, nameList);
						// 格式化“序号”列为分级标识
						List<Map<String, String>> dataListFormat = formatFirstColumnData(
								sheet, dataList, nameList);

						// for(int i1=0;i1<dataList.size();i1++){
						// doSaveDeviceList(dataListFormat.get(i1),"-","-");
						// }

						boolean isNotSameFormatFirstColumn = isNotSameFormatFirstColumnData(
								dataListFormat, nameList);
						String sameErrowMessage = sameFormatFirstColumnInfo(
								dataListFormat, nameList);

						if (!firstColumnNullMsg.equals("")||!fistColumnMessage.equals("")
								|| !isNotSameFormatFirstColumn) {
							if (messageMap.containsKey(false)) {
								messageMap.put(false, messageMap.get(false)
										+ "<br/>工作表“" +sheetName + "”"
										+firstColumnNullMsg 
										+ fistColumnMessage 
										+ sameErrowMessage + "读取的数据条数："
										+ dataList.size() + ";");
							} else {
								messageMap.put(false, "<br/>工作表“" +sheetName + "”"
										+firstColumnNullMsg 
										+ fistColumnMessage 
										+ sameErrowMessage + "读取的数据条数："
										+ dataList.size() + ";");
							}
							continue;

						}
						// 对末级的“数量”列进行校验提示；
						String numberColumnDataErrorMessage = getNumberColumnDataErrorMessages(
								dataListFormat, nameList);
						if (!numberColumnDataErrorMessage.equals("")) {
							if (messageMap.containsKey(false)) {
								messageMap.put(false, messageMap.get(false)
										+ "<br/>工作表“" +sheetName + "”"
										+ numberColumnDataErrorMessage
										);
							} else {
								messageMap.put(false, "<br/>工作表“" +sheetName + "”"
										+ numberColumnDataErrorMessage
										);
							}
							continue;

						}
						// 对“金额”列进行校验提示；
						String moneyColumnDataErrorMessage = getMoneyColumnDataErrorMessages(
								dataListFormat, nameList);
						if (!moneyColumnDataErrorMessage.equals("")) {

							if (messageMap.containsKey(false)) {
								messageMap.put(false, messageMap.get(false)
										+ "<br/>工作表“" +sheetName + "”"
										+ moneyColumnDataErrorMessage
										);
							} else {
								messageMap.put(false, "<br/>工作表“" +sheetName + "”"
										+ moneyColumnDataErrorMessage
										);
							}
							continue;
						}

						Object[] resultArray = new Object[3];
						resultArray[0] = index;
						resultArray[1] = sheetName;
						resultArray[2] = dataListFormat;

						if (messageMap.containsKey(true)) {
							List<Object[]> valueList = (List<Object[]>) messageMap
									.get(true);
							valueList.add(resultArray);
							messageMap.put(true, valueList);
						} else {
							resultList.add(resultArray);
							messageMap.put(true, resultList);
						}
						continue;
					} else {
						if (messageMap.containsKey(false)) {
							messageMap.put(false, messageMap.get(false) +"<br/>工作表“" +sheetName + "”" + value);
						} else {
							messageMap.put(false, "<br/>工作表“" +sheetName + "”" + value
									);
						}
						continue;
					}
				}

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
		return messageMap;
	}

	public static void doSaveDeviceList(Map<String, String> dataMap,
			String sheetUuid, String levelIdFront) {
		Iterator iter = dataMap.entrySet().iterator();

		String columnName = "";
		String data = "";
		while (iter.hasNext()) {
			Map.Entry c = (Entry) iter.next();
			String key = Type.getString(c.getKey());
			if (!key.equals(WebKeys.ROW)) {
				columnName += key.concat(",");
				if (key.equals(WebKeys.UNIT_PRICE)
						|| key.equals(WebKeys.SUM_PRICE)
						|| key.equals(WebKeys.COST_PRICE)) {
					data += "'"
							+ Type.getInteger(c.getValue() != null ? c
									.getValue().toString().trim() : c
									.getValue()) + "',";
				} else if (key.equals(WebKeys.LEVEL_ID)) {
					data += "'" + levelIdFront + Type.getString(c.getValue())
							+ "',";

				} else {
					data += "'" + Type.getString(c.getValue()) + "',";
				}
			}
		}

		if (columnName.length() > 1) {
			columnName = columnName.substring(0, columnName.length() - 1);
		}
		if (data.length() > 1) {
			data = data.substring(0, data.length() - 1);
		}
		String sql = "INSERT INTO xmis.dev_设备清单导入信息 (\n id,sheet_uuid,"
				+ columnName + "\n) VALUES  (\n  '" + UUIDUtils.nextCode()
				+ "','" + sheetUuid + "'," + data + "\n)";
//		System.out.println(sql);
	}

	public static void main(String[] args) {
//		String filePath = "D:\\SC_CD_ZM.xls";
//		String[] array = { "0^控制系统分项" };
//		System.out.println("--");
//		validExcelColumn(filePath, array);
		
	}
}
