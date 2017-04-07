package bitc.sz.wzEnv.action.export;


import bitc.sz.wzEnv.query.PagingInfo;

import java.util.Map;

public interface Formatter {
    /**
     * @param value    此列的值
     * @param row      此行数据
     * @param rowIndex 行数
     * @param pageInfo 页面信息
     * @return
     */
    String getFormatValue(String value, Map row, int rowIndex, PagingInfo pageInfo);
}