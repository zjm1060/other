package com.bitc.jk.xmis.export;

import java.text.DecimalFormat;
import java.util.Map;

import com.bitc.jk.xmis.model.PageInfo;

public class FormatAmt implements Formater {

	public String getFormatValue(String value, Map row, int rowIndex, PageInfo pageInfo) {
		DecimalFormat a = new DecimalFormat(".00");
		if (value.length() == 0 || Double.valueOf(value)==0.00) {
			return "0.00";
		} else {
			return a.format(Double.valueOf(value));
		}
	}
}