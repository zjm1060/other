package bitc.sz.wzEnv.dao;

import org.apache.http.NameValuePair;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.sql.SQLException;
import java.util.List;
import java.util.Map;

/**
 * Created by zsz on 2016/7/4.
 */
public interface ModelInputDao {
    List<Map<String,String>> getZhiliuListData(String water_body_id) throws Exception;

    List<Map<String,String>> getHeDuanListData(String water_body_id) throws Exception;

    List<Map<String,Object>> getZhaMenListDate(String water_body_id) throws Exception;

    List<Map<String,Object>> getShuiKuListDate(String water_body_id) throws Exception;

    List<Map<String,String>> getZhaMenNameAndDmId() throws SQLException;

    List<Map<String,String>> getBianjieListData(String water_body_id,String plan_id) throws Exception;

    List<Map<String,String>> getHedaoListData() throws Exception;

    List<Map<String,String>> getDuanMianListData(int hedao) throws Exception;

    Map<String, String> getInputItemData(String planId, String[] params) throws SQLException;

    List<Map<String,String>> getXulieMapping(String planId) throws SQLException;

    List<List<NameValuePair>> getFileItemData(String planId, String itemName) throws SQLException;

//    List<String> getBianJieDate() throws Exception;
    void importVELOutputResult(File file, String planId) throws SQLException, IOException;

    void importWCOutputResult(File file, String planId) throws SQLException, IOException;

    void importWSOutputResult(File file, String planId) throws SQLException, IOException;

    void deleteResult (String planId) throws SQLException;

    //流量边界条件
    Map<String,Object> getFlowBoundaryConditions(String planId) throws SQLException;

    //污染物边界条件
    Map<String,Object> getPollutionBoundaryConditions(String planId) throws SQLException;

    //温度边界条件
    Map<String,Object> getTemperatureBoundaryConditions(String planId) throws SQLException;

    //水深曲线
    Map<String,Object> getDepthCurve(String planId , String iCode , String jCode) throws SQLException;

    //温度曲线
    Map<String,Object> getTemperatureCurve(String planId , String iCode , String jCode) throws SQLException;

    //污染物曲线
    Map<String,Object> getPollutionCurve(String planId , String iCode , String jCode) throws SQLException;

    //水深结果渲染
    Map<String,Object> renderDepthResult(String planId, int month) throws SQLException;

    //温度结果渲染
    Map<String,Object> renderTemperatureResult(String planId, int month) throws SQLException;

    //浓度结果渲染
    Map<String,Object> renderConcentrationResult(String planId, int month) throws SQLException;

    //流速流向结果渲染
    Map<String,Object> renderFlowSpeedAndDirectionResult(String planId , int month) throws SQLException;

    Map<String,Integer> getMonth(String planId) throws SQLException;
}
