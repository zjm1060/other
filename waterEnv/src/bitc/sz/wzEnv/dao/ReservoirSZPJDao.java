package bitc.sz.wzEnv.dao;

import java.sql.Connection;
import java.sql.SQLException;
import java.util.List;
import java.util.Map;

/**
 * Created by Administrator on 2016/7/13.
 * 于桥水库水质评价接口
 */

public interface ReservoirSZPJDao {
    //于桥水库参与评价的指标项
    List<Map<String,String>> getNutritionItems(String water_body_id) throws SQLException;
//    List getSZPJItemList(Connection connection,String water_body_id,String obj_id,String period_id,String searchType,String[] itemArr) throws SQLException;
    //于桥水库中的某个断面的营养状态评价
//    Map<String,Object> getSZPJNutrition(String water_body_id,String obj_id,String period_id,String searchType,String itemIDs) throws SQLException;

    List<Map<Object,Object>> getReservoirSZPJYearList(String water_body_id) throws SQLException;

//    List getReservoirSZPJItemList(Connection connection,String water_body_id,String period_id,String searchType,String[] itemArr) throws SQLException;
    //于桥水库的营养状态评价
    Map<String,Object> getReservoirSZPJNutrition(String water_body_id,String year,String itemIDs) throws SQLException;

    //于桥水库的营养状态评价统计
    List<Map<String,Object>> getReservoirSZPJNutritionCount(String water_body_id,String begin_year,String end_year,String itemIDs) throws SQLException;

    List<Map<String,String>> getWangegeData() throws SQLException;
}
