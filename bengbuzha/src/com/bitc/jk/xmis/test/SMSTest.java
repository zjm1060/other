package com.bitc.jk.xmis.test;
                 

import java.io.BufferedReader;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

import java.util.*;
import java.text.*;

public class SMSTest {

	private static final String addr = "http://api.sms.cn/mt/";
	private static final String userId = "abc";//"用户名";
	
	/*
	 * 如uid是：test，登录密码是：123123 
	   pwd=md5(123123test),即
       pwd=b9887c5ebb23ebb294acab183ecf0769
           b9887c5ebb23ebb294acab183ecf0769
       
       	线生成地址：http://www.sms.cn/password
	 */
	private static final String pwd = "440ac85892ca43ad26d44c7ad9d47d3e";//"MD5加密";  
	
	private static final String encode = "utf8";  

	public static void send(String msgContent, String mobile) throws Exception {
		
		//组建请求
		String straddr = addr + 
						"?uid="+userId+
						"&pwd="+pwd+
						"&mobile="+mobile+
						"&encode="+encode+
						"&content=" + msgContent;
		
		StringBuffer sb = new StringBuffer(straddr);
		System.out.println("URL:"+sb);
		
		//发送请求
		URL url = new URL(sb.toString());
		HttpURLConnection connection = (HttpURLConnection) url.openConnection();
		connection.setRequestMethod("POST");
		BufferedReader in = new BufferedReader(new InputStreamReader(
				url.openStream()));
		
		//返回结果
		String inputline = in.readLine();
		System.out.println("Response:"+inputline);
		
	}
	
	
	public static void main(String[] args) {
//		try {
//			send("内容", "13621269538");
//		} catch (Exception e) {
//			e.printStackTrace();
//		}
		
		   Date a = new Date();
			 SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd");


					 
				java.sql.Date b=java.sql.Date.valueOf(df
						.format(new java.util.Date((String.valueOf(a)).trim())));
				System.out.println(a+"////"+b);


	}
	
	
	
	
	
	
	

}
