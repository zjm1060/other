package com.bitc.jk.xmis.util;

import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

import org.apache.commons.lang.StringUtils;

public class TemplateUtil {

	/**
	 * 格式化字符串，替换字符串中的变量，变量以{}开头结尾
	 * 
	 * @param template
	 *            模板
	 * @param variables
	 *            变量名-值，变量名不包括{}
	 * @return 格式化后的字符串
	 * @throws Exception
	 *             必须为所有变量指定值，否则会抛出异常
	 */
	public static String format(String template, Map<String, Object> variables)
			throws Exception {
		String temp = template;
		int start = 0;
		int end = 0;
		StringBuilder result = new StringBuilder();
		while (temp != null) {
			start = temp.indexOf('{');
			end = temp.indexOf('}');
			if (start > -1 && end > -1 && end > start) {
				result.append(temp.substring(0, start));
				String name = temp.substring(start + 1, end);
////如果解析的字符串中没有为某个参数设置值，则为其赋值为“”
//				if (!variables.containsKey(name)) {
//					throw new Exception("解析字符串（" + template + "）错误。没有设置变量"
//							+ name + "的值");
//				}
//				result.append(variables.get(name));
//				temp = temp.substring(end + 1);
				if (!variables.containsKey(name)) {
					result.append("");
				}else{
					result.append(variables.get(name));
				}
				temp = temp.substring(end + 1);
			} else {
				break;
			}
		}
		result.append(temp);
		return result.toString();
	}

	public static String format(String template, Object value) throws Exception {
		Object[] values = new Object[] { value };
		return format(template, values);
	}

	/**
	 * 替换字符串中的占位变量，如：{0},{1}。必须从0开始不间断编号
	 * 
	 * @param template
	 *            字符串模板
	 * @param values
	 *            变量值，顺序对应模板中的0,1,...
	 * @return
	 * @throws Exception
	 */
	public static String format(String template, Object[] values)
			throws Exception {
		Map<String, Object> variables = new HashMap<String, Object>();
		for (int i = 0; i < values.length; i++) {
			variables.put(String.valueOf(i), values[i]);
		}
		return format(template, variables);
	}

	public static String delHtmlTag(String htmlStr) {
		String regEx_html = "<[^>]+>";
		Pattern p_html = Pattern.compile(regEx_html, Pattern.CASE_INSENSITIVE);
		Matcher m_html = p_html.matcher(htmlStr);
		htmlStr = m_html.replaceAll("");
		return htmlStr.trim();
	}


	/** 
	 * @param ids
	 * @param regex
	 * @return
	 */
	@SuppressWarnings("null")
	public static String getStrByList(List<String> ids, String regex) {
		String idsStr = "";
		if (ids != null || ids.size() > 0) {
			for (int i = 0; i < ids.size(); i++) {
				idsStr += ids.get(i).toString() + regex;
			}
			if (idsStr.length() > 1) {
				idsStr = idsStr.substring(0, idsStr.length() - 1);
			}
			return idsStr;
		} else {
			return "";
		}
	}

	/**
	 * @param ids
	 * @param regex
	 * @return
	 */
	public static List<String> getListByIds(String ids, String regex) {
		List<String> list = new ArrayList<String>();
		if (ids.indexOf(",") > -1) {
			int length = ids.split(regex).length;
			String[] strs = ids.split(regex);
			for (int i = 0; i < length; i++) {
				list.add(strs[i]);
			}
		} else {
			list.add(ids);
		}

		return list;
	}
	

	/** 32位的加密
	 * @param msg
	 * @return
	 */
	public static String getMd5(String msg){
		MessageDigest md;
		if(!StringUtils.isEmpty(msg)){
			try {
				md = MessageDigest.getInstance("MD5");
				md.update(msg.getBytes());
				byte b[] = md.digest();
				
				int i;
				StringBuffer buf = new StringBuffer("");
				for (int offset = 0; offset < b.length; offset++) {
					i = b[offset];
					if (i < 0)
						i += 256;
					if (i < 16)
						buf.append("0");
					buf.append(Integer.toHexString(i));
				}
				return buf.toString();
			} catch (NoSuchAlgorithmException e) {
				e.printStackTrace();
			}
		}
		return msg;
	}
	/** 
	 * 根据人员ID zentao的登录名(得到缺陷管理 取消)
	 * 规则 正数的人员ID前缀000p 负数的人员ID前缀000n
	 * @param userIDInt
	 * @return
	 */
	public static String getUserIDStr(int userIDInt){
		String userIDStr = Type.getString(userIDInt);
		if(userIDInt<0){
			userIDStr = "000n"+userIDStr.substring(1);
		}else{
			userIDStr = "000p"+userIDStr;
		}
		return userIDStr;
	}
	//去除数组中重复的记录  
	// 去重复
	public static String[] getUniqueList(String[] a) {
		List<String> list = new ArrayList<String>();
		for (int i = 0; i < a.length; i++) {
			if (!list.contains(a[i])) {
				list.add(a[i]);
			}
		}
		return (String[])list.toArray(new String[list.size()]);  
	}
	
}
