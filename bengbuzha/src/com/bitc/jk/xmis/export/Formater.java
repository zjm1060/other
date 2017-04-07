package com.bitc.jk.xmis.export;

import java.util.Map;

import com.bitc.jk.xmis.model.PageInfo;

/**
 * 用于excel导出的格式化接口
 * (此接口暂时只允许有一个方法)
 *
 */
public interface Formater {
	/**
	 * @param value 此列的值
	 * @param row 此行数据
	 * @param rowIndex 行数
	 * @param pageInfo 页面信息
	 * @return
	 */
	String getFormatValue(String value,Map row, int rowIndex, PageInfo pageInfo);
}
