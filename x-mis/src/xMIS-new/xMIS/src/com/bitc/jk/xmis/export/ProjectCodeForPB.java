package com.bitc.jk.xmis.export;

import java.util.Map;
import com.bitc.jk.xmis.model.PageInfo;

public class ProjectCodeForPB implements Formater {

	public String getFormatValue(String value, Map row, int rowIndex,
			PageInfo pageInfo) {
		if (value == null || "".equals(value)
				|| "null".equals(value.toString())) {
			if (row.get("项目编号") == null || "".equals(row.get("项目编号"))
					|| "null".equals(row.get("项目编号").toString())) {
				return "";
			} else {
				return (String) row.get("项目编号") + "(财)";
			}
		}
		return value;
	}
}
