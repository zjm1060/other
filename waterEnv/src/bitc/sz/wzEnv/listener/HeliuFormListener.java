package bitc.sz.wzEnv.listener;

import bitc.sz.wzEnv.report.core.FormListener;
import bitc.sz.wzEnv.report.db.SQLExecutor;
import bitc.sz.wzEnv.report.model.Form;
import bitc.sz.wzEnv.report.model.FormField;
import bitc.sz.wzEnv.report.util.ReportUtil;
import org.apache.commons.lang.StringUtils;
import org.springframework.stereotype.Component;

import java.sql.SQLException;
import java.util.List;
import java.util.Map;

/**
 * Created by BomWu on 2015/11/23 0023.
 */
@Component("heliuFormListener")
public class HeliuFormListener implements FormListener {
    @Override
    public boolean beforeSave(Form form, Map<String, List<FormField>> fieldsMap, Map<String, String> generateKeys) {
        return true;
    }

    @Override
    public void afterSave(Form form, SQLExecutor executor, List<FormField> allFields, Map<String, List<FormField>> fieldsMap, Map<String, String> generateKeys) throws SQLException {
        String glid = ReportUtil.findFormField(allFields, "zdst_rel_干支流关系表", "干流_ID").getValue();
        String stid = ReportUtil.findFormField(allFields, "zdst_水体主体信息简表", "水体_ID").getValue();
        executor.execute("delete from zdst_rel_干支流关系表 where 支流_ID=?", new Object[]{stid});

        if (StringUtils.isNotEmpty(glid)) {
            executor.execute("insert into zdst_rel_干支流关系表(干流_ID,支流_ID) values(?,?)", new Object[]{glid, stid});
        }
    }

    @Override
    public void afterCommit() {

    }

}
