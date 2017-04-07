package com.bitc.jk.xmis.export;

import java.text.DecimalFormat;
import java.util.Map;

import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.Type;

/**
 * 项目结余表 当年支出合计
 * */
public class PayTotalForPB implements Formater {

	public String getFormatValue(String value, Map row, int rowIndex,
			PageInfo pageInfo) {

		Object a1 = row.get("人员经费");
		Object a2 = row.get("采购支出");
		Object a3 = row.get("外加工费");
		Object a4 = row.get("技术服务费");
		Object a5 = row.get("外拨款");
		Object a6 = row.get("业务招待费");
		Object a7 = row.get("会议差旅费");
		Object a8 = row.get("车辆使用");

		Object a9 = row.get("研发支出");
		Object a10 = row.get("应交增值税");
		Object a11 = row.get("其他支出");

		Object a12 = row.get("计提研发费");
		Object a13 = row.get("计提项目管理费");
		Object a14 = row.get("计提工资管理费");
		Object a15 = row.get("计提绩效管理费");
		Object a16 = row.get("计提福利费");
		Object a17 = row.get("计提教育经费");
		Object a18 = row.get("计提工会经费");

		DecimalFormat a = new DecimalFormat(".00");

		Double result = Type.GetDouble(a1) + Type.GetDouble(a2)
				+ Type.GetDouble(a3) + Type.GetDouble(a4) + Type.GetDouble(a5)
				+ Type.GetDouble(a6) + Type.GetDouble(a7) + Type.GetDouble(a8)
				+ Type.GetDouble(a9) + Type.GetDouble(a10)
				+ Type.GetDouble(a11) + Type.GetDouble(a12)
				+ Type.GetDouble(a13) + Type.GetDouble(a14)
				+ Type.GetDouble(a15) + Type.GetDouble(a16)
				+ Type.GetDouble(a17) + Type.GetDouble(a18);

		if (result == 0.00) {
			return "0.00";
		} else {
			return a.format(result);
		}

//		BigDecimal result = ((BigDecimal) row.get("人员经费"))
//				.add((BigDecimal) row.get("采购支出"))
//				.add((BigDecimal) row.get("外加工费"))
//				.add((BigDecimal) row.get("技术服务费"))
//				.add((BigDecimal) row.get("外拨款"))
//				.add((BigDecimal) row.get("业务招待费"))
//				.add((BigDecimal) row.get("会议差旅费"))
//				.add((BigDecimal) row.get("车辆使用"))
//				.add((BigDecimal) row.get("研发支出"))
//				.add((BigDecimal) row.get("应交增值税"))
//				.add((BigDecimal) row.get("其他支出"))
//				.add((BigDecimal) row.get("计提研发费"))
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
