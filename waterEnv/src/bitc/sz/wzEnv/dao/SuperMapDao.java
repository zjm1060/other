package bitc.sz.wzEnv.dao;

import bitc.sz.wzEnv.model.IdAndName;
import bitc.sz.wzEnv.model.ShuiZiYuanBody;
import bitc.sz.wzEnv.model.TypeBody;
import bitc.sz.wzEnv.model.WaterBody;

import java.io.File;
import java.sql.Connection;
import java.sql.SQLException;
import java.util.Date;
import java.util.List;
import java.util.Map;

/**
 * Created by zsz on 2015/12/25.
 */
public interface SuperMapDao {
    String login(String userName, String password) throws Exception;
    List<WaterBody> getWaterBodyByUserID(String userId)throws Exception;

    List<IdAndName> getMainTypes(String userId, String waterBodyId)throws Exception;

    List<TypeBody> getTypes(String userId, String waterBodyId, String mainTypeId, String typeIds,String keyword)throws Exception;

    List<ShuiZiYuanBody> getObjsByType(String userId, String waterBodyId, String mainTypeId, String typeIds)throws Exception;

    List<ShuiZiYuanBody> getObjsBySearch(String userId, String waterBodyId, String mainTypeId, String typeIds,String keyword)throws Exception;

    List<ShuiZiYuanBody> getObjs(String userId, String waterBodyId, String mainTypeId, String typeIds, String keyword) throws Exception;

    List<Map<Object,Object>> getTarget(String userId,  String targetIds,String type ,String water_body_id,String searchType,String period_id) throws Exception;

    List<Map<String,String>> getObjDetailInfo(String objId) throws Exception;

    List<Map<Object,Object>> getPollutionItems(String objId,String water_body_id,String searchType,String period_id) throws SQLException;

    List<Map<String, String>> getSZPJDefaultObj(String waterBodyId) throws SQLException;

    List<Map<String,String>> getDefaultWaterFounc(String water_body_id) throws SQLException;

    List<Map<String,String>> getSZPJYearList(String water_body_id, String objIds) throws SQLException;

    List<Map<String,String>> getSZPJRiverList(String water_body_id, String p_in_year) throws SQLException;

    List<Map<String,String>> getSZPJPeriodList(String water_body_id,String obj_ids, String year, String searchType) throws SQLException;

    List<Map<String,Object>> getSZPJRiverLevel(String water_body_id, String searchType, String period_id, String obj_ids) throws Exception;

    List<Map<Object,Object>> getSZPJItemValue(String water_body_id, String searchType, String period_id, String obj_id) throws SQLException;

    Map<String,Object> getSZPJGgraphs(String period_id,String dmId,String itemId) throws SQLException;

    Map<String,Object> getSzyWaterQuality(List<Map<Object,Object>> xzqList,String water_body_id ,String xzq_id,String year) throws SQLException;

    List getAllXzqWaterQuality(String water_body_id,String year) throws SQLException;

    List<Map<String,Object>> getPollution(String water_body_id ,String xzq_id,String year) throws SQLException;

    boolean saveSzmxResult(List<String[]> strList, String[] strData1, Date date, int countNum) throws Exception;

    boolean saveSdlmxResult1(List<String[]> strList, String[] strData1, Date date, int countNum, String plan_id) throws Exception;

    boolean saveSzmxResultFile(File f,String step_id)throws Exception;
    //接口七
    List<Map<String,String>> getSZYYearList(String water_body_id) throws SQLException;
    //接口一
    List<Map<String,String>> getXZQDataList(String water_body_id,String year) throws SQLException;
    //接口二
    List<Map<String,Object>> getXZQCondition(String water_body_id ,String xzq_id,String year) throws SQLException;
    //接口三
    List<Map<Object,Object>> getRiverWaterFounctionCondition(String water_body_id ,String xzq_id,String year,String riverID) throws SQLException;
    //接口八
    //排污口地图标绘
    List<Map<String, String>> getSZYPWKObj(String waterBodyId) throws SQLException;
    //接口四
    List<Map<Object,Object>> getWaterFounctionItemValue(String water_body_id,String year,String xzq_id,String riverID,String waterFounctionID) throws SQLException;
    //接口五
    List<Map<Object,Object>> getPWKDetail(String water_body_id,String year,String xzq_id,String riverID,String waterFounctionID) throws SQLException;
    //接口六
    List<Map<Object,Object>> getXZQBubbleDetail(String water_body_id,String year,String xzq_id) throws SQLException;
    //接口九：水功能区气泡数据
    List<Map<String,String>> getWaterFounctionWaterregimen(String water_body_id) throws SQLException;

    void deleteModelData(String step_id) throws SQLException, Exception;
//做测试用
//    List<Map<Object,Object>> getxzqList(String water_body_id , String xzq_id) throws SQLException;

    List<Map<Object,Object>> getXZQResultChart(String water_body_id,String begin_year,String end_year) throws SQLException;

    List<Map<Object,Object>> getXZQPollutionQuantity(String water_body_id,String begin_year,String end_year) throws SQLException;

    List<Map<Object,Object>> getRiverPollutionQuantity(String water_body_id,String xzqid,String begin_year,String end_year) throws SQLException;

    List<Map<Object,Object>> getWaterFounctionPollutionQuantity(String water_body_id,String xzqid,String riverID,String begin_year,String end_year) throws SQLException;

    List<Map<Object,Object>> getXZQPollutionExcessive(String water_body_id,String begin_year,String end_year) throws SQLException;

    List<Map<Object,Object>> getRiverPollutionExcessive(String water_body_id,String xzqid,String begin_year,String end_year) throws SQLException;

    List<Map<Object,Object>> getXZQWaterRate(String water_body_id,String begin_year,String end_year) throws SQLException;

    List<Map<Object,Object>> getRiverWaterRate(String water_body_id,String xzqid,String begin_year,String end_year) throws SQLException;

    List<Map<Object,Object>> getXZQWaterRateExcessive(String water_body_id,String begin_year,String end_year) throws SQLException;

    List<Map<Object,Object>> getRiverWaterRateExcessive(String water_body_id,String xzqid,String begin_year,String end_year) throws SQLException;

    //河流水质评价统计
    List<Map<String,Object>> getRiverSZPJCount(String water_body_id,String begin_year,String end_year) throws SQLException;

    //水功能区水质评价
    List<Map<String,Object>> getWaterFounctionSZPJCount(String water_body_id,String riverID ,String begin_year,String end_year) throws SQLException;

    //水功能区水质评价详情
//    Map<String, Object> getWaterFounctionSZPJCountDetail(String water_body_id,String riverId,String waterFounctionID ,String begin_year,String end_year) throws SQLException;

    //某一水功能区断面
    List<Map<String,Object>> getDMSZPJCount(String water_body_id,String waterFounctionID ,String begin_year,String end_year) throws SQLException;

    //某一水功能区断面的统计详情
    Map<String,Object> getDMSZPJCountDetail(String water_body_id,String waterFounctionID ,String begin_year,String end_year) throws SQLException;

    boolean deletePlanById(String planId) throws SQLException;

    boolean deleteInputFileById(String planId) throws SQLException;

    boolean deleteInputParamsById(String planId) throws SQLException;

    void deleteExist(String step_id) throws SQLException;

//-----------------水利部版本的水功能区水质评价--------------------------------------------------
    //获取水质评价的起止日期
    Map<String,String> getStartAndEndDate(String period_id,String searchType,Connection connection) throws SQLException;

    //水功能区的水质评价现在按照水利部的版本重新设计
    //查询某条河流的所有水功能区信息
    List<Map<String,String>> getSZPJWaterFounctionInRiver(String riverId,String startDate,String endDate) throws SQLException;

    //查询一个或多个（断面或者水质站）的水质评价结果
//    Map<String,String> getSZPJDmOrSzInWaterFounction(Connection connection , String waterFounctionId);

    //水功能区水质评价详情
    Map<String,Object> getWaterFounctionSZPJCountDetail(String water_body_id,String riverId,String begin_year,String end_year) throws SQLException;

    //图例显示(wq:水质评价；wr：水资源承载力；ns：营养状态评价)
    List<Map<String,String>> getLegendByType(String type);
}
