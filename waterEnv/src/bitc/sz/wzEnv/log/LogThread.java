package bitc.sz.wzEnv.log;

import bitc.sz.wzEnv.model.T_log;
import bitc.sz.wzEnv.report.core.ReportService;
import bitc.sz.wzEnv.report.db.ReportDAO;
import bitc.sz.wzEnv.report.db.ReportDAOImpl;
import org.springframework.beans.factory.annotation.Autowired;

/**
 * 日志执行单元
 *
 * @author imgavin
 */
public class LogThread implements Runnable {
    @Autowired
    private ReportService reportService;

    public LogThread(ReportService reportService) {
        this.reportService = reportService;
    }

    @Override
    public void run() {
        while (true) {
            T_log log = null;
            try {
                log = LogOperator.logInfos.take();
                reportService.saveLog(log);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

}
