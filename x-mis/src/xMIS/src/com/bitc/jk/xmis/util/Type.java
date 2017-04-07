package com.bitc.jk.xmis.util;

import java.sql.Date;
import java.sql.Timestamp;
import java.text.SimpleDateFormat;

public class Type {
	
	public static int getFinalMaxID(int id){
		
		return id==0? 1:id;
	}
	
	public static int GetInt(Object obj) {
		if (obj == null || "".equals(obj))
			return 0;
		// return ((Integer) obj).intValue();
		return Integer.parseInt(String.valueOf(obj));
	}

	public static Integer getInteger(Object obj) {
		return (obj == null || "".equals(obj)) ? Integer.valueOf(0) : Integer
				.valueOf(String.valueOf(obj));
	}

	public static double GetDouble(Object obj) {
		// if (obj == null)
		// return 0.0;
		// return ((Double) obj).doubleValue();
		//		

		return (obj == null || "".equals(obj)) ? Double.valueOf(0)
				.doubleValue() : Double.valueOf(String.valueOf(obj))
				.doubleValue();
	}
	
	public static float GetFloat(Object obj) {

		return (obj == null || "".equals(obj)) ? Float.valueOf(0)
				.floatValue() : Float.valueOf(String.valueOf(obj))
				.floatValue();
	}
	

	public static Timestamp GetDate(Object obj) {
		System.out.print("123");
		if (obj == null)
			return Timestamp.valueOf("1770-1-1");
		else
			return (Timestamp) obj;

	}

	public static long getLong(Object obj) {

		return obj == null ? Long.parseLong("0") : Long.parseLong(String
				.valueOf(obj).trim());
	}

	public static String getString(Object obj) {

		return obj == null ? "" : String.valueOf(obj).trim();
	}

	public static String getString(int a) {
		return String.valueOf(a);
	}

	public static Date getDate(Object obj) {
		SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd");

		try {
			if("".equals(String.valueOf(obj)))
				return null;
				
			return obj == null ? null : Date.valueOf(df
					.format(new java.util.Date((String.valueOf(obj)).trim())));
		} catch (Exception e) {
			//e.printStackTrace();
			
//			System.out.println((String.valueOf(obj))
//					.trim().substring(0,10));
			return obj == null ? null : Date.valueOf((String.valueOf(obj))
					.trim().substring(0,10));
		}
	}

	public static int convertBinToInt(Object obj) {

		return obj == null ? 0
				: (Boolean.parseBoolean(String.valueOf(obj)) == true ? 1 : 0);
	}

	public static Timestamp getTimestamp(Object obj) {

		return obj == null ? null : (Timestamp) obj;

	}
	
	public static boolean getBoolean(Object obj){
		
		return GetInt(obj) == 0 ? false: true;
		
	}

}
