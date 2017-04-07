package bitc.sz.wzEnv.report.core;

import bitc.sz.wzEnv.model.T_log;
import bitc.sz.wzEnv.query.PagingInfo;
import bitc.sz.wzEnv.report.db.ReportDAOImpl;
import bitc.sz.wzEnv.report.model.*;
import net.sf.jsqlparser.JSQLParserException;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

import java.sql.SQLException;
import java.util.List;

/**
 * Created by BomWu on 2015/11/12 0012.
 */
public interface ReportService {
    List<DBConnect> getDbConnects();

    void setReportDAO(ReportDAOImpl reportDAO);

    String doInitSQLReport(Report report, List<Parameter> defaultValues) throws SQLException, ReportException, JSQLParserException;

    Report getReportByName(String token) throws ReportException;

    void doInitForm(Form form,String tableName,String rowKey) throws SQLException;

    Form getFormByName(String reportToken) throws FormException;

    void saveOrUpdate(Form form, List<FormField> formfield) throws Exception;

    List<CombField> getCombos(CombField field) throws SQLException, Exception;

    void updateReport(Report report, List<Parameter> list) throws SQLException, ReportException;

    void deleteReport(String reportId);

    Report getReport(String reportId);

    Object getForm(String formId);

    void deleteRows(DeleteCommand deleteCommand) throws Exception;

    void saveLog(T_log log);

    GridData selectGridData(String reportToken, PagingInfo info, String parameterValues, String reportCondition) throws ReportException;
}
