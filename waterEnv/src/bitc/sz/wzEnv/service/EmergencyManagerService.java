package bitc.sz.wzEnv.service;

import javax.servlet.http.HttpServletRequest;
import java.sql.SQLException;
import java.util.List;
import java.util.Map;

/**
 * Created by Administrator on 2016/9/9.
 */
public interface EmergencyManagerService {
    List<Map<String, Object>> queryWrwMessage(String keyword) throws SQLException;

    List<Map<String,Object>> queryFxyMessage(String water_body_id,String keyword) throws SQLException;

    List<Map<String,Object>> queryZjMessage(String water_body_id,String keyword) throws SQLException;

    List<Map<String,Object>> queryYuanMessage(String water_body_id,String keyword) throws SQLException;

    List<Map<String,Object>> queryFaguiMessage(String water_body_id,String keyword) throws SQLException;

    List<Map<String, Object>> queryWaterPollutionEventCoordinate(String water_body_id,String keyword) throws SQLException;

    List<Map<String, String>> queryWrwMessageById(String wrwId) throws SQLException;

    List<Map<String,String>> queryFxyMessageById(String fxyId) throws SQLException;

    List<Map<String, String>> queryZjMessageById(String zjId,HttpServletRequest request) throws SQLException;

    List<Map<String,Object>> queryYuanMessageById(String planId,HttpServletRequest request) throws SQLException;

    List<Map<String,Object>> queryFaguiMessageById(String lawId,HttpServletRequest request) throws SQLException;

    void saveWaterPollutionEvent(String pollutedJSON) throws SQLException;

    void deleteWaterPollutionEvent(String pollutionEventId) throws SQLException;

    Map<String,Object> queryWaterPollutionEvent(String pollutionEventId) throws SQLException;

}
