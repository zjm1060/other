package com.bitc.jk.xmis.export;

import java.sql.Types;
import java.text.DecimalFormat;
import java.util.Map;

import com.bitc.jk.xmis.model.PageInfo;
import com.bitc.jk.xmis.util.Type;

/**
 * 项目结余表 当年到款合计
 * */
public class ContractIncomeTotalForPB implements Formater {

	public String getFormatValue(String value, Map row, int rowIndex,
			PageInfo pageInfo) {
		Object a1 = row.get("合同到款");
		Object a2 = row.get("退税收入");
		Object a3 = row.get("转账收入");
		DecimalFormat a = new DecimalFormat(".00");

		Double result = Type.GetDouble(a1) + Type.GetDouble(a2)
				+ Type.GetDouble(a3);
		if (result == 0.00) {
			return "0.00";
		} else {
			return a.format(result);
		}
		
//		DecimalFormat a = new DecimalFormat(".00");
//		BigDecimal result = new BigDecimal((String) row.get("合同到款"));
//		result = result.add(new BigDecimal((String) row.get("退税收入")));
//		result = result.add(new BigDecimal((String) row.get("转账收入")));
//
//		if (new BigDecimal(0) == result) {
//			return "0.00";
//		} else {
//			return a.format(result.toString());
//		}

	}
}
