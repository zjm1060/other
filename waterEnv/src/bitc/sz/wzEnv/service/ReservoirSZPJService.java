package bitc.sz.wzEnv.service;

import java.sql.SQLException;
import java.util.List;
import java.util.Map;

/**
 * Created by Administrator on 2016/7/19.
 */
public interface ReservoirSZPJService {
    List<Map<String,String>> getNutritionItems(String water_body_id) throws SQLException;
    //于桥水库某个断面（湖泊营养状态评价）
//    Map<String,Object> getSZPJNutrition(String water_body_id,String obj_id,String period_id,String searchType,String itemIDs) throws SQLException;
    //于桥水库的营养状态评价
    Map<String,Object> getReservoirSZPJNutrition(String water_body_id,String year,String itemIDs)throws SQLException;

    List<Map<Object,Object>> getReservoirSZPJYearList(String water_body_id) throws SQLException;

    List<Map<String,Object>> getReservoirSZPJNutritionCount(String water_body_id,String begin_year,String end_year,String itemIDs) throws SQLException;

    List<Map<String,String>> getWangegeData() throws SQLException;
}
