package com.bitc.jk.xmis.export;

import java.util.Map;

import com.bitc.jk.xmis.model.PageInfo;

public class materialPropertiestoo implements Formater{

	public String getFormatValue(String value, Map row, int rowIndex,
			PageInfo pageInfo) {
		// TODO Auto-generated method stub
		
		if("选择物资用途".equals(value)){
			return "";
		}else if("选择物资自身属性".equals(value)){
			return "";
		}else if("选择物资业务".equals(value)){
			return "";
		}else{
			
			return value;
		}
	}

}
