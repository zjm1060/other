package com.bitc.jk.xmis.export;

import java.text.DecimalFormat;
import java.util.Map;

import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.Type;

/**
 * 项目结余表 计提支出合计
 * */
public class CountAndDrawTotalForPB implements Formater {

	public String getFormatValue(String value, Map row, int rowIndex,
			PageInfo pageInfo) {
		Object a1 = row.get("计提研发费");
		Object a2 = row.get("计提项目管理费");
		Object a3 = row.get("计提工资管理费");
		Object a4 = row.get("计提绩效管理费");
		Object a5 = row.get("计提福利费");
		Object a6 = row.get("计提教育经费");
		Object a7 = row.get("计提工会经费");
		DecimalFormat a = new DecimalFormat(".00");

		Double result = Type.GetDouble(a1) + Type.GetDouble(a2)
				+ Type.GetDouble(a3) + Type.GetDouble(a4) + Type.GetDouble(a5)
				+ Type.GetDouble(a6) + Type.GetDouble(a7);

		if (result == 0.00) {
			return "0.00";
		} else {
			return a.format(result);
		}

//		BigDecimal result = ((BigDecimal) row.get("计提研发费"))
//				.add((BigDecimal) row.get("计提项目管理费"))
//				.add((BigDecimal) row.get("计提工资管理费"))
//				.add((BigDecimal) row.get("计提绩效管理费"))
//				.add((BigDecimal) row.get("计提福利费"))
//				.add((BigDecimal) row.get("计提教育经费"))
//				.add((BigDecimal) row.get("计提工会经费"));
//		DecimalFormat a = new DecimalFormat(".00");
//		if (new BigDecimal(0) == result) {
//			return "0.00";
//		} else {
//			return a.format(result);
//		}

	}
}
