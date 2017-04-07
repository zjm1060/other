package bitc.sz.wzEnv.report.core;

import bitc.sz.wzEnv.report.model.Report;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

/**
 * Created by BomWu on 2016/3/14 0014.
 */
@Component
public class ReportExecutorFactory {

    @Autowired
    private NormalReportExecutor normalReportExecutor;
    @Autowired
    private ProcedureReportExecutor procedureReportExecutor;

    public ReportExecutor getReportExecutor(Report report) {
        if (report.getSqlType() != null) {
            switch (report.getSqlType()) {
                case NORMARL:
                    return normalReportExecutor;
                case PROCEDURE:
                    return procedureReportExecutor;
                default:
                    return normalReportExecutor;
            }
        }else {
            return normalReportExecutor;
        }
    }
}
