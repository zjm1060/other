package com.bitc.jk.xmis.util;

import java.io.BufferedWriter;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.UnsupportedEncodingException;
import java.io.Writer;
import java.util.HashMap;
import java.util.Map;

import freemarker.template.Configuration;
import freemarker.template.Template;
import freemarker.template.TemplateException;
import freemarker.template.DefaultObjectWrapper;

/**
 * freemarker生成word文档
 * @author Administrator
 *
 */
public class DocumentHandler {
	public static void createDoc(Map dataMap , String templateName , String filePath , String fileName, File templateDirectory){
		//创建配置实例
		Configuration configuration = new Configuration();
		//设置编码
		configuration.setDefaultEncoding("utf-8");
		//ftl模板文件统一放在resources/deptTemplate下面
//		configuration.setClassForTemplateLoading(DocumentHandler.class, "/xMIS/WebRoot/resources/deptTemplate");
		try {			
			configuration.setDirectoryForTemplateLoading(templateDirectory);
			configuration.setObjectWrapper(new DefaultObjectWrapper());
		} catch (IOException e1) { 
			e1.printStackTrace();
		}
		//获取模板
		Template template = null;
		try {
			template = configuration.getTemplate(templateName,"utf-8");
		} catch (IOException e) {
			e.printStackTrace();
		}
		//输出文件
		File outFile = new File(filePath+File.separator+fileName);
		//如果输出目标文件夹不存在，则创建
		if(!outFile.getParentFile().exists()){
			outFile.getParentFile().mkdirs();
		}
		//将模板和数据模型合并生成文件
		Writer out = null;
		try {
			out = new BufferedWriter(new OutputStreamWriter(
					new FileOutputStream(outFile),"utf-8"));
		} catch (UnsupportedEncodingException e) {
			e.printStackTrace();
		} catch (FileNotFoundException e) {
			e.printStackTrace();
		}
		//生成文件
		try {
			template.process(dataMap, out);
		} catch (TemplateException e) {
			e.printStackTrace();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}
	//测试程序代码段
	@SuppressWarnings("unchecked")
	public static void main(String[] args) {
		Map dataMap = new HashMap();
		dataMap.put("title_name", "用户信息");
		dataMap.put("user_name", "张三");
		dataMap.put("org_name", "微软公司");
		dataMap.put("dept_name", "事业部");
		String templateName = "t1.ftl";
		String filePath = "D:/temp1/";
		String fileName = "hh.doc";
	}
}

