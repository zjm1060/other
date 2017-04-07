package com.bitc.jk.xmis.export;

import java.sql.Date;
import java.text.SimpleDateFormat;
import java.util.Map;

import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.Type;

public class FormateTimerForPB implements Formater {

	public String getFormatValue(String value, Map row, int rowIndex,
			PageInfo pageInfo) {
		if (value != null && value != "") {
			SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
			return sdf.format(new Date(Type.getLong(value)));
		} else {
			return "";
		}

	}

}
