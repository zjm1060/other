package bitc.sz.wzEnv.report.db;

import bitc.sz.wzEnv.log.Log;
import bitc.sz.wzEnv.model.T_log;
import bitc.sz.wzEnv.query.BaseDAO;
import bitc.sz.wzEnv.report.model.DBConnect;
import bitc.sz.wzEnv.report.model.Form;
import bitc.sz.wzEnv.report.model.Report;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public class ReportDAOImpl extends BaseDAO implements ReportDAO  {

    @Override
    public List<DBConnect> getDbConnects() {
        return findAll(DBConnect.class);
    }

    @Override
    public DBConnect getDbConnectByName(String name) {
        List list = sessionFactory.getCurrentSession().createQuery("from DBConnect d  where d.name=?").setString(0, name)
                .list();
        if (list != null && list.size() > 0) {
            return (DBConnect) list.get(0);
        }
        return null;
    }

    @Override
    public void saveReport(Report report) {
        saveOrUpdate(report);
    }

    @Override
    public Report findReportByName(String token) {
        List list = sessionFactory.getCurrentSession().createQuery("from Report r left join fetch r.columns where r.name=?").setString(0, token)
                .list();
        if (list != null && list.size() > 0) {
            return (Report) list.get(0);
        }
        return null;
    }
    @Override
    public Form findFormByName(String token) {
        List list = sessionFactory.getCurrentSession().createQuery("from Form f left join fetch f.fields where f.name=?").setString(0, token)
                .list();
        if (list != null && list.size() > 0) {
            return (Form) list.get(0);
        }
        return null;
    }

    @Override
    public void delete(String reportId) {
        Report report=get(Report.class, reportId);
        delete(report);
    }
    @Override
    public void saveLog(T_log o){
        saveOrUpdate(o);
    }
}
