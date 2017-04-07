package bitc.sz.wzEnv.service.impl;

import bitc.sz.wzEnv.dao.EmergencyManagerDao;
import bitc.sz.wzEnv.service.EmergencyManagerService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import javax.servlet.http.HttpServletRequest;
import java.sql.SQLException;
import java.util.List;
import java.util.Map;

/**
 * Created by Administrator on 2016/9/9.
 */
@Service
@Transactional
public class EmergencyManagerServiceImpl implements EmergencyManagerService {
    @Autowired
    private EmergencyManagerDao emergencyManagerDao;

    @Override
    public List<Map<String, Object>> queryWrwMessage(String keyword) throws SQLException {
        return emergencyManagerDao.queryWrwMessage(keyword);
    }

    @Override
    public List<Map<String, Object>> queryFxyMessage(String water_body_id,String keyword) throws SQLException {
        return emergencyManagerDao.queryFxyMessage(water_body_id,keyword);
    }

    @Override
    public List<Map<String, Object>> queryZjMessage(String water_body_id,String keyword) throws SQLException {
        return emergencyManagerDao.queryZjMessage(water_body_id,keyword);
    }

    @Override
    public List<Map<String, Object>> queryYuanMessage(String water_body_id,String keyword) throws SQLException {
        return emergencyManagerDao.queryYuanMessage(water_body_id,keyword);
    }

    @Override
    public List<Map<String, Object>> queryFaguiMessage(String water_body_id,String keyword) throws SQLException {
        return emergencyManagerDao.queryFaguiMessage(water_body_id,keyword);
    }

    @Override
    public List<Map<String, Object>> queryWaterPollutionEventCoordinate(String water_body_id, String keyword) throws SQLException {
        return emergencyManagerDao.queryWaterPollutionEventCoordinate(water_body_id,keyword);
    }

    @Override
    public List<Map<String, String>> queryWrwMessageById(String wrwId) throws SQLException {
        return emergencyManagerDao.queryWrwMessageById(wrwId);
    }

    @Override
    public List<Map<String, String>> queryFxyMessageById(String fxyId) throws SQLException {
        return emergencyManagerDao.queryFxyMessageById(fxyId);
    }

    @Override
    public List<Map<String, String>> queryZjMessageById(String zjId, HttpServletRequest request) throws SQLException {
        return emergencyManagerDao.queryZjMessageById(zjId, request);
    }

    @Override
    public List<Map<String, Object>> queryYuanMessageById(String planId, HttpServletRequest request) throws SQLException {
        return emergencyManagerDao.queryYuanMessageById(planId,request);
    }

    @Override
    public List<Map<String, Object>> queryFaguiMessageById(String lawId, HttpServletRequest request) throws SQLException {
        return emergencyManagerDao.queryFaguiMessageById(lawId, request);
    }

    @Override
    public void saveWaterPollutionEvent(String pollutedJSON) throws SQLException {
        emergencyManagerDao.saveWaterPollutionEvent(pollutedJSON);
        return;
    }

    @Override
    public void deleteWaterPollutionEvent(String pollutionEventId) throws SQLException {
        emergencyManagerDao.deleteWaterPollutionEvent(pollutionEventId);
        return;
    }

    @Override
    public Map<String, Object> queryWaterPollutionEvent(String pollutionEventId) throws SQLException {
        return emergencyManagerDao.queryWaterPollutionEvent(pollutionEventId);
    }
}
