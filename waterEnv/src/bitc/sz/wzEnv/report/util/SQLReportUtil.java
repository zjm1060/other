package bitc.sz.wzEnv.report.util;


import bitc.sz.wzEnv.query.PagingInfo;
import org.apache.commons.lang.StringUtils;

public class SQLReportUtil {

    public static String getNoDataSQL(String sql) {
        return "select nodatasql.* from ( " + sql + " ) nodatasql where 1=2";
    }

    public static String getSQL(boolean isPaging, String type, String sql, PagingInfo info, String reportCondition) {
        if(info.getLimit() == 0){
            isPaging = false;
        }
        return isPaging ? getPagingSQL(type, sql, info, reportCondition) : getNoPagingSQL(sql, info, reportCondition);
    }

    private static String getPagingSQL(String type, String sql, PagingInfo info, String reportCondition) {
        int end = info.getStart() + info.getLimit();
        if ("oracle".equals(type)) {
            return "select * from (select rownum as iamtherownumber_,report_t_0.* from (" + sql + ") report_t_0 where rownum<=" + end + getCondition("and", reportCondition) + getOrderBy(info) + ") report_0_0 where report_0_0.iamtherownumber_>" + info.getStart();
        } else if ("mysql".equals(type)) {
            return "select * from (" + sql + ") report_util_t " + getCondition("where", reportCondition) + getOrderBy(info) + " limit " + info.getStart() + "," + info.getLimit();
        } else if ("sqlserver".equals(type)) {
            String str = "SET NOCOUNT ON " +
                    " select identity(int,1,1) as row_no,result.*  into #resultA from ("+sql+") result " + getCondition("where", reportCondition) + getOrderBy(info)+
                    " select * from #resultA  where row_no >= "+info.getStart() +" and row_no <="+ end;
            return str ;
        } else {
            return sql;
        }
    }

    public static String getNoPagingSQL(String sql, PagingInfo info, String reportCondition) {
        return "select * from (" + sql + ") report_util_t " + getCondition("where", reportCondition) + getOrderBy(info);
    }

    private static String getCondition(String prefix, String reportCondition) {
        if (StringUtils.isEmpty(reportCondition)) {
            return "";
        } else {
            return " "+ prefix + " " + reportCondition;
        }
    }

    private static String getOrderBy(PagingInfo info) {
        return info.getSort() == null ? "" : " order by " + info.getSort() + " " + info.getOrder();
    }


    public static String getTotalCount(String sql, String reportCondition) {
        String str;
        if (reportCondition == null || "".equals(reportCondition)) {
            str = "select count(*) as totalcount from (" + sql + ") report_util_t";
        } else {
            str = "select count(*) as totalcount from (" + sql + ") report_util_t where " + reportCondition ;
        }
        return str;
    }

    public static void main(String[] arg) {
        PagingInfo info = new PagingInfo();
        info.setLimit(20);
        info.setStart(10);
        System.out.println(getTotalCount("SELECT * FROM jeecmsv5.jc_channel", null));
    }
}
