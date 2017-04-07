package bitc.sz.wzEnv.dao;

import java.sql.SQLException;
import java.util.Date;
import java.util.List;
import java.util.Map;

/**
 * Created by zsz on 2016/5/18.
 */
public interface ExecuteExeSDLDao {
    List<Map<Integer, String>> getCOEDADate(String plan_id) throws Exception;

    String [][] getGateHYOData(String [] zhamen) throws Exception;

    int getGateHYOCount() throws Exception;

    Map<String, String> getZhamenDmHao() throws Exception;

    Map<String, String> getShuikuDmHao() throws Exception;

    String getZhiliuNames(String plan_id,String key)throws Exception;

    List<Map<Integer,String>> getTaiZiHQDate(String planId, String id) throws Exception;

    List<Map<String,String>> waterDMData(String planId, String dmId,String obj_id,String modelId, String start, String end) throws SQLException;

    Map<String,String> waterDMMetaData(String planId, String dmId) throws SQLException;

    Map<String, String> waterHDMetaData(String planId) throws SQLException;

    List<Map<Integer,String>> getBeisDate(String planId, String fileId) throws SQLException;

    Map<String,String> getExecFile(String fileId) throws Exception;

    List<Map<Integer, String>> getLiuhDate(String planId, String fileId) throws SQLException;

    List<Map<Integer,String>> writeTaiZiSC() throws SQLException;

    List<Map<String, String>> waterDMJCData(String planId, String dmId, String obj_id, String start, String end) throws SQLException;

    List<Map<String, String>> waterDMJCData(String planId,  String obj_name, String start, String end) throws SQLException;

    int getPlanYear(String planId) throws Exception;

    List<Map<String,String>> waterHDData(String planId,String modelId, String obj_id,String obj_id1, String start, String end)throws SQLException;

    List<Map<String,String>> waterHDDataByDay(String planId,String modelId, String obj_id,String obj_id1, String start, String end)throws SQLException;

    List<Map<String,String>> getPlanInfoList(String waterBodyId, String water_bady_id) throws SQLException;

    List<Map<String, String>> getDmInfoList(String waterBodyId) throws SQLException;

    List<Map<String, String>> getDMWaterPosition(String planId, String dmId, String start, String end) throws SQLException;

    List<Map<String, String>> getDMElevation(String dmId) throws SQLException;

    List<Map<String,String>> getCompareblePlanList(String planId) throws SQLException;

    String  getJisuanName(String jisuanId) throws SQLException;

    String getStepByPlanId(String plan_id) throws SQLException;
}
