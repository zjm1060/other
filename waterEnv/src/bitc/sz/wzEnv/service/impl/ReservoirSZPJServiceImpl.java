package bitc.sz.wzEnv.service.impl;

import bitc.sz.wzEnv.dao.ReservoirSZPJDao;
import bitc.sz.wzEnv.service.ReservoirSZPJService;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.sql.SQLException;
import java.util.Collections;
import java.util.List;
import java.util.Map;

/**
 * Created by Administrator on 2016/7/19.
 */
@Service
@Transactional
public class ReservoirSZPJServiceImpl implements ReservoirSZPJService {

    @Autowired
    private ReservoirSZPJDao dao;

    @Override
    public List<Map<String, String>> getNutritionItems(String water_body_id) throws SQLException {
        return dao.getNutritionItems(water_body_id);
    }

//    @Override
//    public Map<String, Object> getSZPJNutrition(String water_body_id, String obj_id, String period_id, String searchType, String itemIDs) throws SQLException {
//        return dao.getSZPJNutrition(water_body_id,obj_id,period_id,searchType,itemIDs);
//    }

    @Override
    public Map<String, Object> getReservoirSZPJNutrition(String water_body_id, String year, String itemIDs) throws SQLException {
        return dao.getReservoirSZPJNutrition(water_body_id,year,itemIDs);
    }

    @Override
    public List<Map<Object, Object>> getReservoirSZPJYearList(String water_body_id) throws SQLException {
        return dao.getReservoirSZPJYearList(water_body_id);
    }

    @Override
    public List<Map<String, Object>> getReservoirSZPJNutritionCount(String water_body_id, String begin_year, String end_year, String itemIDs) throws SQLException {
        return dao.getReservoirSZPJNutritionCount(water_body_id,begin_year,end_year,itemIDs);
    }

    @Override
    public List<Map<String,String>> getWangegeData() throws SQLException {
        List<Map<String,String>> data = dao.getWangegeData();
        return data;
    }
}
