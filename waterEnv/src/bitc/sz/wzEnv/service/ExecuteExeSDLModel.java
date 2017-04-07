package bitc.sz.wzEnv.service;

import bitc.sz.wzEnv.dao.ExecuteExeSDLDao;
import bitc.sz.wzEnv.dao.ExecuteExeSZPJDao;
import bitc.sz.wzEnv.util.ExtjsResponse;

import java.io.File;
import java.io.IOException;
import java.sql.SQLException;

/**
 * Created by zsz on 2016/5/24.
 */
public interface ExecuteExeSDLModel {
    boolean writeGateHYO(File file,String [] zhamen,ExecuteExeSDLDao dao) throws Exception;

    ExtjsResponse waterDMPositionData(String planId, String dmId, String obj_id, String start, String end, ExecuteExeSDLDao dao);

    ExtjsResponse waterDMDepthData(String planId, String dmId, String obj_id, String start, String end, ExecuteExeSDLDao dao);

    ExtjsResponse waterDMSpeedData(String planId, String dmId, String obj_id, String start, String end, ExecuteExeSDLDao dao);

    ExtjsResponse waterDMFlowData(String planId, String dmId, String obj_id, String start, String end, ExecuteExeSDLDao dao);

    ExtjsResponse waterHDData(String planId, String obj_id, String obj_id1, String start, String end, ExecuteExeSDLDao dao, String name);

    ExtjsResponse waterHDDataByDay(String planId, String obj_id, String obj_id1, String start, String end, ExecuteExeSDLDao dao, String name);

    boolean writeCOEDA(File file,String plan_id,ExecuteExeSDLDao dao) throws Exception;

    boolean writeTaiZiHQ(File file, String id, ExecuteExeSDLDao dao, String planId) throws Exception;

    boolean writeTaiziTxt(File file, String fileId, ExecuteExeSDLDao dao, String xxxx, String name) throws Exception;

    boolean writeTaiZiHModelInFile(String plan_Id,String file, ExecuteExeSDLDao dao,String [] fileNames,String [] zhamenNames) throws Exception;

    boolean writeTaiZiSC(File file, ExecuteExeSDLDao dao) throws Exception;

    ExtjsResponse getPlanInfoList(String waterBodyId, String water_bady_id, String userid, ExecuteExeSDLDao executeExeDao);

    ExtjsResponse getDmInfoList(String planId, ExecuteExeSDLDao dao);

    ExtjsResponse  getTerrainProfileData(String planId, String dmId, String start, String end, ExecuteExeSDLDao dao);

    void exportDMWaterPositionExcel(String planId, String dmId, ExecuteExeSDLDao dao) throws IOException, SQLException;

    void exportDMWaterDepthExcel(String planId, String dmId, ExecuteExeSDLDao executeExeDao) throws IOException, SQLException;

    void exportDMWaterFlowExcel(String planId, String dmId, ExecuteExeSDLDao executeExeDao) throws IOException, SQLException;

    void exportDMWaterSpeedExcel(String planId, String dmId, ExecuteExeSDLDao executeExeDao) throws IOException, SQLException;

    ExtjsResponse getCompareblePlanList(String planId, ExecuteExeSDLDao executeExeDao);

    void exportHDWaterDataExcel(String planId, String modelId,String jisuanId, ExecuteExeSDLDao dao) throws Exception;

    void exportHDDensitySourceExcel(String planId,ExecuteExeSDLDao exeDao, ExecuteExeSZPJDao executeExeDao) throws Exception;

    void exportDMDensitySourceExcel(String planId, String juliId, String dmId, ExecuteExeSDLDao exeDao, ExecuteExeSZPJDao szpjDao) throws Exception;

    String getStepByPlanId(String plan_id,ExecuteExeSDLDao exeDao) throws SQLException;
}
