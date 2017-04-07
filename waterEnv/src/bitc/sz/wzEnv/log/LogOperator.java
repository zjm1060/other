package bitc.sz.wzEnv.log;

import bitc.sz.wzEnv.model.T_log;
import bitc.sz.wzEnv.report.core.ReportService;
import bitc.sz.wzEnv.report.db.ReportDAO;
import bitc.sz.wzEnv.report.db.ReportDAOImpl;
import org.apache.log4j.Logger;
import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.annotation.After;
import org.aspectj.lang.annotation.Aspect;
import org.aspectj.lang.annotation.Pointcut;
import org.aspectj.lang.reflect.MethodSignature;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

import javax.annotation.PostConstruct;
import javax.annotation.PreDestroy;
import java.util.Calendar;
import java.util.Date;
import java.util.UUID;
import java.util.concurrent.LinkedBlockingQueue;

@Aspect
@Component
public class LogOperator {
    @Autowired
    private ReportService reportService;
    private static final Logger logger = Logger.getLogger(LogOperator.class);
    public static LinkedBlockingQueue<T_log> logInfos = new LinkedBlockingQueue<T_log>(10000);
    private Thread thread;

    public LogOperator() {

    }

    @PostConstruct
    public void init() {
        thread = new Thread(new LogThread(reportService));
//        thread.start();
        logger.info("Log thread start.");
    }

    @PreDestroy
    public void destroy() {
        thread.interrupt();
        thread = null;
    }

    @Pointcut("@annotation(bitc.sz.wzEnv.log.Log)")
    public void gjsLogPointCut() {
    }

    @After("gjsLogPointCut()")
    public void afterLog(JoinPoint pjp) {
        T_log log = getLogInfo(pjp);
        addLog(log);
    }

    public static void addLog(T_log log) {
        try {
            logInfos.put(log);
        } catch (InterruptedException e) {
            logger.error("log error", e);
        }
    }

    private T_log getLogInfo(JoinPoint joinPoint) {
// 获取被注解的方法的参数
        Object[] o = joinPoint.getArgs();
        MethodSignature joinPointObject = (MethodSignature) joinPoint.getSignature();
        String[] pns = joinPointObject.getParameterNames();
        Object[] pts = joinPointObject.getParameterTypes();

        StringBuilder sb = new StringBuilder("");
        if (o.length > 0) {
            for (int i = 0; i < o.length; i++) {
                sb.append("[" + i + ":][" + pts[i].toString() + "][" + o[i] + "]");
                if (i < o.length - 1) {
                    sb.append(",");
                }
            }
        }

        T_log log = new T_log();
        log.setClassName(joinPointObject.getDeclaringTypeName());
        log.setMethodName(joinPointObject.getMethod().getName());
        log.setParameterName(sb.toString());
        log.setUserId("");
        log.setDoDate(Calendar.getInstance().getTime());
        return log;
    }
}
