package com.bitc.jk.xmis.export;

import java.util.Map;

import com.bitc.jk.xmis.model.PageInfo;

/**
 * 得到excel导出所有的结果的rownumber
 *
 */
public class RowNumber implements Formater {

	public String getFormatValue(String value, Map row, int rowIndex, PageInfo pageInfo) {
		return String.valueOf(rowIndex + 1);
	}

}
