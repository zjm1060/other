package com.bitc.jk.xmis.export.service.impl;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.util.Map;

import javax.script.Invocable;
import javax.script.ScriptEngine;
import javax.script.ScriptEngineManager;
import javax.script.ScriptException;

import jxl.Workbook;
import jxl.write.Label;
import jxl.write.WritableSheet;
import jxl.write.WritableWorkbook;
import net.sf.json.JSONArray;
import net.sf.json.JSONObject;

import com.bitc.jk.xmis.export.Formater;
import com.bitc.jk.xmis.export.service.ExportService;
import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.JSONUtils;
import com.bitc.jk.xmis.util.TemplateUtil;

public class ExportServiceImpl implements ExportService {

	/**
	 * 写入excel
	 * 
	 * @param columnModeArray
	 * @param rowsArray
	 * @param targetFile
	 * @param formaterMap
	 * @return
	 * @throws Exception
	 */
	public void writeBook(JSONArray columnModeArray, JSONArray rowsArray,
			File targetFile, Map<String, String> formaterMap,
			PageInfo pageInfo, String webRoot) throws Exception {
		WritableWorkbook book = Workbook.createWorkbook(targetFile);
		WritableSheet sheet = book.createSheet("result", 0);

		// 新添加一列序号
		for (int j = 0; j < columnModeArray.size(); j++) {

			// 把表头写入excel
			JSONObject columnObject = columnModeArray.getJSONObject(j);
			String header = columnObject.getString("header");
			Label titleLabel = new Label(j, 0, TemplateUtil.delHtmlTag(header));

			// 设置宽度
			int widthInt = columnObject.getInt("width");
			int w = widthInt * 16 / 96;
			sheet.setColumnView(j, w);
			sheet.addCell(titleLabel);
			String dataIndex = columnObject.getString("dataIndex");
			String name = columnObject.getString("name");
			String mapping = columnObject.getString("mapping");
			String formaterScript = JSONUtils.getString(columnObject,"exportFormater");
			Formater formater = getFormater(formaterScript, header, formaterMap);

			for (int i = 0; i < rowsArray.size(); i++) {
				JSONObject row = rowsArray.getJSONObject(i);
				String data = null;
				JSONObject record = getRecord(row, name, mapping);
				if (record == null) {
					data = "";
				} else {
					data = JSONUtils.getString(record, dataIndex);
					if(data == null){
						data = "";
					}
				}
				// 如果mapping=质保期限
//				if (mapping.equals("质保期限")) {
//					JSONObject safeDateUnit = getRecord(row, "质保期计量单位",
//							"质保期计量单位");
//					String unit = JSONUtils.getString(safeDateUnit, "质保期计量单位");// 质保
//					if (data != "" && !data.equals("0.0")) {
//						if (unit.equals("1")) {
//							unit = "年";
//						} else if (unit.equals("2")) {
//							unit = "月";
//						} else if (unit.equals("3")) {
//							unit = "日";
//						}
//						if (data.indexOf(".") > 0) {
//							data = data.substring(0, data.indexOf(".")) + unit;
//						} else {
//							data = "";
//						}
//
//					} else {
//						data = "";
//					}
//
//				}
				// 如果mapping=有效期限
//				if (mapping.equals("有效期限")) {
//					JSONObject underlineDateUnit = getRecord(row, "有效期计量单位",
//							"有效期计量单位");
//					String underlineUnit = JSONUtils.getString(
//							underlineDateUnit, "有效期计量单位");// 有效期
//					if (data != "" && !data.equals("0.0")) {
//						if (underlineUnit.equals("1")) {
//							underlineUnit = "年";
//						} else if (underlineUnit.equals("2")) {
//							underlineUnit = "月";
//						} else if (underlineUnit.equals("3")) {
//							underlineUnit = "日";
//						}
//						if (data.indexOf(".") > 0) {
//							data = data.substring(0, data.indexOf("."))
//									+ underlineUnit;
//						} else {
//							data = "";
//						}
//					} else {
//						data = "";
//					}
//
//				}

				// data-合同概要的value 和 模板（summaryValue） 两个参数；
				// 如果数据中有">,<"，则调用js中的方法，传入两个参数；
				if (data.indexOf("&^$>,<$^&") > -1) {
					JSONObject summaryRecord = getRecord(row, "summarystencil",
							"summarystencil");
					String summaryValue = JSONUtils.getString(summaryRecord,
							"summarystencil");// 模板内容
					ScriptEngine engine = new ScriptEngineManager()
							.getEngineByName("javascript");

					try {
						engine.eval(new FileReader(webRoot
								+ "/resources/js/SummaryFormat.js"));
					} catch (FileNotFoundException e) {
						e.printStackTrace();
					} catch (ScriptException e) {
						e.printStackTrace();
					}
					if (engine instanceof Invocable) {// 是否可调用方法
						Invocable in = (Invocable) engine;
						try {
							data = (String) in
									.invokeFunction("SummaryFormatUtil", data,
											summaryValue, "");// 执行js中的函数
						} catch (ScriptException e) {
							e.printStackTrace();
						} catch (NoSuchMethodException e) {
							e.printStackTrace();
						}
					}
				}
				String value = data;
				// 去除^urld的-start
				if (value.indexOf("^") > -1) {
					value = value.substring(0, value.indexOf("^"));
				}
				value = TemplateUtil.delHtmlTag(value);
				// 去除连接的-stop
				if(formater!=null){
					value=formater.getFormatValue(value, row, i, pageInfo);
				}
				Label dataLabel = new Label(j, i + 1, value);
				sheet.addCell(dataLabel);
			}
		}
		book.write();
		book.close();
	}

	/**
	 * 读取Formater接口在js中的实现
	 * 
	 * @param formaterScript 从前台传来的exportFormater函数
	 * @param header 列的header名字
	 * @param formaterMap 某列对应处理的一个键值对
	 * @return
	 */
	private Formater getFormater(String formaterScript, String header,
			Map<String, String> formaterMap) {
		try {
			if (formaterScript != null) {
				String script = formaterScript.replace("function",
						"function getFormatValue");
				ScriptEngine engine = new ScriptEngineManager()  //java调用js代码
						.getEngineByName("javascript");
				engine.eval(script); //解析脚本
				Invocable jsInvoke = (Invocable) engine;
				//返回一个接口的实现，该接口使用解释器中编译的函数。该接口的方法是使用 invokeFunction 方法实现的。 
				//参数 要返回的接口的 Class 对象
				//返回的是所请求接口的实例
				return jsInvoke.getInterface(Formater.class);
			}
			String className = formaterMap.get(header);
			if (className == null) {
				return null;
			}
			return (Formater) Class.forName(className).newInstance();
		} catch (Exception e) {
			return null;
		}
	}

	private JSONObject getRecord(JSONObject row, String name, String mapping) {
		JSONObject record = new JSONObject();
		Object value = JSONUtils.get(row, mapping);
		if (value == null) {
			return null;
		}
		record.element(name, value);
		return record;
	}

	/**
	 * 格式转换
	 * 
	 * @param header
	 * @param data
	 * @param formaterMap
	 * @param rowIndex
	 * @param pageInfo
	 * @return
	 * @throws Exception
	 */
	private String getFormatValue(String header, String data,
			Map<String, String> formaterMap, int rowIndex, PageInfo pageInfo)
			throws Exception {
		String className = formaterMap.get(header);
		if (className == null) {
			return data;
		}
		Formater formater = (Formater) Class.forName(className).newInstance();
		return formater.getFormatValue(data, null, rowIndex, pageInfo);
	}

}
