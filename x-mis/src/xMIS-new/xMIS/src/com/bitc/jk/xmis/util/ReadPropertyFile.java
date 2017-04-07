package com.bitc.jk.xmis.util;

import java.io.IOException;
import java.io.InputStream;
import java.util.Properties;


public class ReadPropertyFile {
	
	private static ReadPropertyFile m_instance = null;
	private Properties p;
	
	private ReadPropertyFile(){
		try {
			InputStream in = this.getClass().getResourceAsStream("/system.properties");
			this.p = new Properties();
			p.load(in);
		} catch (IOException e) {
			System.out.println("读取文件失败！");
			e.printStackTrace();
		}
	}

    public synchronized static ReadPropertyFile getInstance() {    
	    if (m_instance == null) {    
	       m_instance = new ReadPropertyFile();    
	     }    
	    return m_instance;    
	}
	
	/**
	 * 根据属性名字得到参数的值
	 * 
	 * @param 配置文件的名字
	 * @return 对应的内容
	 */
	private String getValueByName(String paramName) {
		return p.getProperty(paramName);
	}
	
	/**
	 * 根据属性名字得到参数的值
	 * 
	 * @param 配置文件的名字
	 * @return 对应的内容
	 */
	public static String getValueByParam(String paramName) {
		ReadPropertyFile r = ReadPropertyFile.getInstance();
		String result = r.getValueByName(paramName); 
		return result;
	}

}
