package bitc.sz.wzEnv.report.db;

import bitc.sz.wzEnv.model.T_log;
import bitc.sz.wzEnv.report.model.DBConnect;
import bitc.sz.wzEnv.report.model.Form;
import bitc.sz.wzEnv.report.model.Report;

import java.util.List;

/**
 * Created by BomWu on 2015/11/12 0012.
 */
public interface ReportDAO{
    List<DBConnect> getDbConnects();

    DBConnect getDbConnectByName(String name);

    void saveReport(Report report);

    Report findReportByName(String token);

    Form findFormByName(String token);

    void delete(String reportId);

    void saveLog(T_log o);
}
