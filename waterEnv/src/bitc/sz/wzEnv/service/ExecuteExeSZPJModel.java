package bitc.sz.wzEnv.service;

import bitc.sz.wzEnv.util.ExtjsResponse;

import java.io.File;
import java.sql.SQLException;
import java.util.List;
import java.util.Map;

/**
 * Created by zsz on 2016/6/22.
 */
public interface ExecuteExeSZPJModel {
    boolean readWQPLT2Database(File WQfile,String plan_id) throws Exception;

    ExtjsResponse waterDMDensitySource(String planId, String dmId, String start, String end) throws Exception;

    List<Map<String, Object>> waterHDNH3NSource(String planId, String juliId, String jisuanId, String item_id,String start, String end, String name) throws Exception;

    List<Map<String, Object>> waterHDNH3NSourceByDay(String planId, String juliId, String jisuanId, String item_id, String start, String end, String name) throws Exception;

    boolean exportDataFile(String step_id,String filePath) throws SQLException;

    boolean exportCBOUData(String step_id,String filePath) throws SQLException;

    boolean exportCgCTOTALData(String step_id,String filePath) throws SQLException;

    boolean exportJHXSData(String step_id, String filePath, boolean flag) throws  Exception;

    boolean writeInputFile(String plan_id, String file,boolean flag) throws Exception;

    String getModelType(String plan_id) throws SQLException;

    boolean exportJH7T(String step_id, String file) throws Exception;

    List<Map<String,String>> getWuRanWuList(String planId) throws SQLException;

    String getJianCeItemId(String s) throws SQLException;

    void updatePlanState(String plan_id,int i) throws SQLException;

    List<Map<String,Object>> waterHDSsSource(String planId, String js, String id, String name, String name1) throws Exception;

    int getPlanState(String plan_id) throws SQLException;
}
