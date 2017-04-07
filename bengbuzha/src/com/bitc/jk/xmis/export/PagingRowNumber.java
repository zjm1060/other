package com.bitc.jk.xmis.export;

import java.util.Map;

import com.bitc.jk.xmis.model.PageInfo;

/**
 * 得到excel导出的结果的rownumber
 *
 */
public class PagingRowNumber implements Formater {

	public String getFormatValue(String value, Map row,int rowIndex, PageInfo pageInfo) {
		if (rowIndex != pageInfo.getLimit()) {
			return String.valueOf(pageInfo.getStart() + rowIndex + 1);
		} else {
			return "合计";
		}
	}
}
