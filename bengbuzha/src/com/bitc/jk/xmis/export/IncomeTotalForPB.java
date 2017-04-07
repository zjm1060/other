package com.bitc.jk.xmis.export;

import java.text.DecimalFormat;
import java.util.Map;

import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.Type;

public class IncomeTotalForPB implements Formater {

	public String getFormatValue(String value, Map row, int rowIndex,
			PageInfo pageInfo) {
		Object a1 = row.get("合同到款");
		Object a2 = row.get("退税收入");
		Object a3 = row.get("转账收入");
		Object a4 = row.get("以前年度累计到款");
		
		DecimalFormat a = new DecimalFormat(".00");

		Double result = Type.GetDouble(a1) + Type.GetDouble(a2)
				+ Type.GetDouble(a3) + Type.GetDouble(a4);
		if (result == 0.00) {
			return "0.00";
		} else {
			return a.format(result);
		}
	}

}
