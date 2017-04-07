package com.bitc.jk.xmis.export;

import java.text.DecimalFormat;
import java.util.Map;

import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.Type;

/**
 * 项目结余表 其他支出合计
 * */
public class ElsePayTotalForPB implements Formater {

	public String getFormatValue(String value, Map row, int rowIndex,
			PageInfo pageInfo) {
		Object a1 = row.get("研发支出");
		Object a2 = row.get("应交增值税");
		Object a3 = row.get("其他支出");

		DecimalFormat a = new DecimalFormat(".00");

		Double result = Type.GetDouble(a1) + Type.GetDouble(a2)
				+ Type.GetDouble(a3);

		if (result == 0.00) {
			return "0.00";
		} else {
			return a.format(result);
		}
		
//		BigDecimal result = ((BigDecimal) row.get("研发支出")).add(
//				(BigDecimal) row.get("应交增值税"))
//				.add((BigDecimal) row.get("其他支出"));
//		DecimalFormat a = new DecimalFormat(".00");
//		if (new BigDecimal(0) == result) {
//			return "0.00";
//		} else {
//			return a.format(result);
//		}
	}
}
