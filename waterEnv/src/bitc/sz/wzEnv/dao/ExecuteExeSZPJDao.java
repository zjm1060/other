package bitc.sz.wzEnv.dao;

import bitc.sz.wzEnv.util.ExtjsResponse;

import java.sql.SQLException;
import java.util.List;
import java.util.Map;

/**
 * Created by zsz on 2016/6/22.
 */
public interface ExecuteExeSZPJDao {
    boolean readWQPLT2Database(List<String[]> list,long date,int countNum, String step_id) throws Exception;

    List<Map<String,String>> getDMJiceValue(String dmId, String aa, String s, String s1) throws SQLException;

    List<Map<String,String>> waterHDData(String planId, String modelId, String juliId, String jisuanId, String item_id, String start, String end) throws Exception;

    List<Map<String, String>> waterHDDataByDay(String planId, String modelId, String juli, String jisuanId, String item_id, String start, String end) throws Exception;

    boolean getExportData(String step_id,String filePath) throws SQLException;

    boolean exportCBOUData(String step_id,String filePath) throws SQLException;

    boolean exportCTOTALData(String step_id,String filePath) throws SQLException;

    String getStepByPlan(String plan_id) throws SQLException;

    String getSzpjData(String step_id,String key) throws SQLException;

    String getWrwIdByName(String key) throws SQLException;

    int getYearByPlan(String plan_id) throws SQLException;

    boolean exportSsCTOTALData(String step_id, String file) throws SQLException;

    List<Map<String,String>> getWuRanWuList(String planId) throws SQLException;

    String getJianCeItemId(String name) throws SQLException;

    void updatePlanState(String plan_id,int i) throws SQLException;

    List<Map<String,String>> waterHDSsSource(String stepId, String juliId, String jisuanId, String item_id, String start, String end) throws SQLException;

    int getPlanState(String plan_id) throws SQLException;
}
