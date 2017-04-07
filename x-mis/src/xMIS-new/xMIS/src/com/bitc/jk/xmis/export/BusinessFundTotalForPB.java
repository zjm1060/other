package com.bitc.jk.xmis.export;

import java.math.BigDecimal;
import java.text.DecimalFormat;
import java.util.Map;

import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.Type;

/**
 * 项目结余表 主要业务经费合计
 * */
public class BusinessFundTotalForPB implements Formater {

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
		DecimalFormat a = new DecimalFormat(".00");

		Double result = Type.GetDouble(a1) + Type.GetDouble(a2)
				+ Type.GetDouble(a3) + Type.GetDouble(a4) + Type.GetDouble(a5)
				+ Type.GetDouble(a6) + Type.GetDouble(a7) + Type.GetDouble(a8);

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
//				.add((BigDecimal) row.get("车辆使用"));
//		DecimalFormat a = new DecimalFormat(".00");
//
//		if (new BigDecimal(0) == result) {
//			return "0.00";
//		} else {
//			return a.format(result);
//		}
		
		
	}
}
