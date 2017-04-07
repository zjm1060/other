package bitc.sz.wzEnv.report.core;

import bitc.sz.wzEnv.query.PagingInfo;
import bitc.sz.wzEnv.report.model.Column;
import bitc.sz.wzEnv.report.model.GridData;
import bitc.sz.wzEnv.report.model.Parameter;
import bitc.sz.wzEnv.report.model.Report;
import net.sf.jsqlparser.JSQLParserException;

import java.sql.SQLException;
import java.util.List;

/**
 * Created by BomWu on 2016/3/14 0014.
 */
public interface ReportExecutor {

//    List<Parameter> initParameter(Report report, String defaultValues) throws ReportException, JSQLParserException;

    List<Column> initColumns(Report report) throws ReportException;

    GridData selectGridData(Report report, PagingInfo info, Object[] parameterValues,String condition) throws ReportException;

}
