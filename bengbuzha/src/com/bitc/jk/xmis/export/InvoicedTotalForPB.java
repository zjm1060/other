package com.bitc.jk.xmis.export;

import java.text.DecimalFormat;
import java.util.Map;

import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.Type;

/**
 * 项目结余表 已开发票金额合计
 * */
public class InvoicedTotalForPB implements Formater {

	public String getFormatValue(String value, Map row, int rowIndex,
			PageInfo pageInfo) {
		Object a1 = row.get("硬件");
		Object a2 = row.get("软件");
		Object a3 = row.get("其他");

		DecimalFormat a = new DecimalFormat(".00");

		Double result = Type.GetDouble(a1) + Type.GetDouble(a2)
				+ Type.GetDouble(a3);

		if (result == 0.00) {
			return "0.00";
		} else {
			return a.format(result);
		}
		
//		BigDecimal result = ((BigDecimal) row.get("硬件")).add(
//				(BigDecimal) row.get("软件")).add((BigDecimal) row.get("其他"));
//		DecimalFormat a = new DecimalFormat(".00");
//		if (new BigDecimal(0) == result) {
//			return "0.00";
//		} else {
//			return a.format(result);
//		}
	}

}
