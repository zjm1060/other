package bitc.sz.wzEnv.webservice;

import javax.jws.WebParam;
import javax.jws.WebService;
import java.sql.SQLException;

/**
 * Created by BomWu on 2015/12/9 0009.
 */

@WebService(endpointInterface = "bitc.sz.wzEnv.webservice.IQueryService", serviceName = "query")
public interface IQueryService {
    String login(@WebParam(name="userName") String userName,
                 @WebParam(name="password")  String password) ;

    String getMainTypes(@WebParam(name="userId") String userId,
                        @WebParam(name="waterBodyId") String waterBodyId);

    /**
     * @param typeIds 1,2,3 多个下拉框类型ID拼一起，逗号分隔
     */
    String getTypes(@WebParam(name="userId") String userId,
                    @WebParam(name="waterBodyId") String waterBodyId,
                    @WebParam(name="mainTypeId") String mainTypeId,
                    @WebParam(name="typeIds") String typeIds,
                    @WebParam(name="keyword") String keyword) ;

    String searchTargets(@WebParam(name="userId") String userId,
                         @WebParam(name="waterBodyId") String waterBodyId,
                         @WebParam(name="mainTypeId") String mainTypeId,
                         @WebParam(name="keyword") String keyword) ;

    String getTarget(@WebParam(name="userId") String userId,
                     @WebParam(name="targetId") String targetId,
                     @WebParam(name="type")String type,
                     @WebParam(name="water_body_id")String water_body_id,
                     @WebParam(name="searchType")String searchType,
                     @WebParam(name="period_id")String period_id) ;

    String getObjsByType(@WebParam(name = "userId") String userId,
                        @WebParam(name="waterBodyId") String waterBodyId,
                        @WebParam(name="mainTypeId") String mainTypeId,
                        @WebParam(name="typeIds") String typeIds) ;

    String getObjsBySearch(@WebParam(name="userId") String userId,
                         @WebParam(name="waterBodyId") String waterBodyId,
                         @WebParam(name="mainTypeId") String mainTypeId,
                           @WebParam(name="typeIds") String typeIds,
                         @WebParam(name="keyword") String keyword) ;
    String getObjs(@WebParam(name = "userId") String userId,
                         @WebParam(name="waterBodyId") String waterBodyId,
                         @WebParam(name="mainTypeId") String mainTypeId,
                         @WebParam(name="typeIds") String typeIds,
                         @WebParam(name="keyword") String keyword) ;
    String getObjDetailInfo(@WebParam(name = "objId") String objId,
                            @WebParam(name = "water_body_id")String water_body_id,
                            @WebParam(name = "searchType")String searchType,
                            @WebParam(name = "period_id") String period_id);

    String getSZPJDefaultObj(@WebParam(name = "water_body_id") String water_body_id);

    String getDefaultWaterFounc(@WebParam(name = "water_body_id") String water_body_id);

    String getSZPJYearList(@WebParam(name = "water_body_id") String water_body_id,
                           @WebParam(name = "obj_id") String obj_id);
    String getSZPJRiverList(@WebParam(name = "water_body_id") String water_body_id,
                            @WebParam(name = "p_in_year") String p_in_year);

    String getSZPJPeriodList(@WebParam(name = "water_body_id") String water_body_id,
                             @WebParam(name = "obj_ids") String obj_ids,
                             @WebParam(name = "year") String year,
                             @WebParam(name = "searchType") String searchType);

    String getSZPJRiverLevel(@WebParam (name = "water_body_id") String water_body_id,
                             @WebParam (name = "searchType") String searchType,
                             @WebParam (name = "period_id") String period_id,
                             @WebParam (name = "obj_ids") String obj_ids);

    String getSZPJItemValue(@WebParam (name = "water_body_id") String water_body_id,
                            @WebParam (name = "searchType") String searchType,
                            @WebParam(name = "period_id")String period_id,
                            @WebParam (name = "obj_id")String obj_id);

    String getSZPJGgraphs(@WebParam(name = "period_id")String period_id,
                          @WebParam(name = "dmId")String dmId,
                          @WebParam(name = "itemId")String itemId);

//    String getSzyWaterQuality(@WebParam(name = "water_body_id")String water_body_id ,
//                              @WebParam(name = "xzq_id")String xzq_id,
//                              @WebParam(name = "year") String year);

    String getAllXzqWaterQuality(@WebParam(name = "water_body_id")String water_body_id,
                                 @WebParam(name = "year")String year);
    String execSdlDataModel(@WebParam(name = "plan_id") String plan_id, String sdlFile) throws Exception;

    String execSzpjCgDataModel(@WebParam(name = "plan_id") String plan_id, String sdlFile, String szpjPath, @WebParam(name = "flag") boolean flag) throws Exception;

    String execSzpjSsDataModel(@WebParam(name = "plan_id") String plan_id, String sdlFile, String szpjPath, @WebParam(name = "flag") boolean flag) throws Exception;

    String execDataModel(String plan_id, String sdlFile, @WebParam(name = "plan_id") String szpjPath, String szpjSsPath) throws Exception;

    String getSZYYearList(@WebParam(name = "water_body_id") String water_body_id);

    String getXZQDataList(@WebParam(name = "water_body_id")String water_body_id,
                                 @WebParam(name = "year")String year);

    String getXZQCondition(@WebParam(name = "water_body_id") String water_body_id,
                           @WebParam(name = "xzq_id")String xzq_id,
                           @WebParam(name = "year") String year);

    String getRiverWaterFounctionCondition(@WebParam(name = "water_body_id")String water_body_id ,
                                           @WebParam(name = "xzq_id")String xzq_id,
                                           @WebParam(name = "year")String year,
                                           @WebParam(name = "riverID")String riverID);

    String getSZYPWKObj(@WebParam(name = "water_body_id") String water_body_id);

    String getWaterFounctionItemValue(@WebParam(name = "water_body_id")String water_body_id,
                                      @WebParam(name = "year")String year,
                                      @WebParam(name = "xzq_id")String xzq_id,
                                      @WebParam(name = "riverID")String riverID,
                                      @WebParam(name = "waterFounctionID")String waterFounctionID);

    String getPWKDetail(@WebParam(name = "water_body_id")String water_body_id,
                        @WebParam(name = "year")String year,
                        @WebParam(name = "xzq_id")String xzq_id,
                        @WebParam(name = "riverID")String riverID,
                        @WebParam(name = "waterFounctionID")String waterFounctionID);

    String getXZQBubbleDetail(@WebParam(name = "water_body_id")String water_body_id,
                              @WebParam(name = "year")String year,
                              @WebParam(name = "xzq_id")String xzq_id);

    String getWaterFounctionWaterregimen(@WebParam(name = "water_body_id")String water_body_id);

    String waterDMPositionData(@WebParam(name = "planId")String planId,
                             @WebParam(name = "dmId")String dmId,
                             @WebParam(name = "start")String start,
                             @WebParam(name = "end")String end) throws Exception;
    String waterDMDepthData(@WebParam(name = "planId")String planId,
                             @WebParam(name = "dmId")String dmId,
                             @WebParam(name = "start")String start,
                             @WebParam(name = "end")String end) throws Exception;
    String waterDMFlowData(@WebParam(name = "planId")String planId,
                             @WebParam(name = "dmId")String dmId,
                             @WebParam(name = "start")String start,
                             @WebParam(name = "end")String end) throws Exception;
    String waterDMSpeedData(@WebParam(name = "planId")String planId,
                             @WebParam(name = "dmId")String dmId,
                             @WebParam(name = "start")String start,
                             @WebParam(name = "end")String end) throws Exception;

    String waterHDPositionDataByDay(@WebParam(name = "planId")String planId, @WebParam(name = "start")String start,@WebParam(name = "end") String end) throws Exception;

    String waterHDDepthDataByDay(@WebParam(name = "planId")String planId, @WebParam(name = "start")String start,@WebParam(name = "end") String end) throws Exception;

    String waterHDFlowDataByDay(@WebParam(name = "planId")String planId, @WebParam(name = "start")String start,@WebParam(name = "end") String end) throws Exception;

    String waterHDSpeedDataByDay(@WebParam(name = "planId")String planId, @WebParam(name = "start")String start,@WebParam(name = "end") String end) throws Exception;

    String writeDMTaiZiHQ(@WebParam(name = "model_id")String model_id) throws Exception;

    String waterHDPositionData(@WebParam(name = "planId")String planId,
                               @WebParam(name = "start")String start,
                               @WebParam(name = "end")String end) throws Exception;
    String waterHDDepthData(@WebParam(name = "planId")String planId,
                            @WebParam(name = "start")String start,
                            @WebParam(name = "end")String end) throws Exception;
    String waterHDFlowData(@WebParam(name = "planId")String planId,
                           @WebParam(name = "start")String start,
                           @WebParam(name = "end")String end) throws Exception;
    String waterHDSpeedData(@WebParam(name = "planId")String planId,
                            @WebParam(name = "start")String start,
                            @WebParam(name = "end")String end) throws Exception;

    String getPlanInfoList(@WebParam(name = "keyword")String keyword,
                           @WebParam(name = "water_bady_id")String water_bady_id,
                           @WebParam(name = "userid")String userid
                           )throws Exception;

    String getDmInfoList(@WebParam(name = "planId")String planId) throws Exception;

    String deletePlanById(@WebParam(name = "planId")String planId) throws Exception;

    String getTerrainProfileData(@WebParam(name = "planId")String planId,
                                 @WebParam(name = "dmId")String dmId,
                                 @WebParam(name = "start")String start,
                                 @WebParam(name = "end")String end)throws Exception;

//    void exportDMWaterPositionExcel(@WebParam(name = "planId")String planId,
//                                    @WebParam(name = "dmId")String dmId) throws IOException, SQLException;
//
//    void exportDMWaterFlowExcel(@WebParam(name = "planId")String planId,
//                                    @WebParam(name = "dmId")String dmId) throws IOException, SQLException;
//
//    void exportDMWaterDepthExcel(@WebParam(name = "planId")String planId,
//                                    @WebParam(name = "dmId")String dmId) throws IOException, SQLException;
//
//    void exportDMWaterSpeedExcel(@WebParam(name = "planId")String planId,
//                                 @WebParam(name = "dmId")String dmId) throws IOException, SQLException;

    String getDMExportUrlList(@WebParam(name = "planId")String planId,
                              @WebParam(name = "dmId")String dmId,@WebParam(name = "jisuanId") String jisuanId);

    String getHDExportUrlList(@WebParam(name = "planId")String planId,@WebParam(name = "jisuanId") String jisuanId);

    String getCompareblePlanList(@WebParam(name = "planId")String planId);

    String waterDMDensitySource(@WebParam(name = "planId")String planId,
                                @WebParam(name = "dmId")String dmId,
                              @WebParam(name = "start")String start,
                              @WebParam(name = "end")String end) throws Exception;

    String waterHDDensitySourceByDay(@WebParam(name = "planId")String planId,
                                     @WebParam(name = "start")String start,
                                     @WebParam(name = "end")String end) throws Exception;


    String waterHDDensitySource(@WebParam(name = "planId")String planId,
                                @WebParam(name = "start")String start,
                                @WebParam(name = "end") String end) throws Exception;

    String getXZQResultChart(@WebParam(name = "water_body_id")String water_body_id,
                             @WebParam(name = "begin_year")String begin_year,
                             @WebParam(name = "end_year")String end_year);

    String getXZQPollutionQuantity(@WebParam(name = "water_body_id")String water_body_id,
                                   @WebParam(name = "begin_year")String begin_year,
                                   @WebParam(name = "end_year")String end_year);

    String getRiverPollutionQuantity(@WebParam(name = "water_body_id")String water_body_id,
                                     @WebParam(name = "xzqid")String xzqid,
                                     @WebParam(name = "begin_year")String begin_year,
                                     @WebParam(name = "end_year")String end_year);

    String getWaterFounctionPollutionQuantity(@WebParam(name = "water_body_id")String water_body_id,
                                              @WebParam(name = "xzqid")String xzqid,
                                              @WebParam(name = "riverID")String riverID,
                                              @WebParam(name = "begin_year")String begin_year,
                                              @WebParam(name = "end_year")String end_year);

    String getXZQPollutionExcessive(@WebParam(name = "water_body_id")String water_body_id,
                                    @WebParam(name = "begin_year")String begin_year,
                                    @WebParam(name = "end_year")String end_year);

    String getRiverPollutionExcessive(@WebParam(name = "water_body_id")String water_body_id,
                                      @WebParam(name = "xzqid")String xzqid,
                                      @WebParam(name = "begin_year")String begin_year,
                                      @WebParam(name = "end_year")String end_year);

    String getXZQWaterRate(@WebParam (name = "water_body_id")String water_body_id,
                           @WebParam(name = "begin_year")String begin_year,
                           @WebParam(name = "end_year")String end_year);

    String getRiverWaterRate(@WebParam (name = "water_body_id")String water_body_id,
                             @WebParam(name = "xzqid")String xzqid,
                             @WebParam(name = "begin_year")String begin_year,
                             @WebParam(name = "end_year")String end_year);

    String getXZQWaterRateExcessive(@WebParam(name = "water_body_id")String water_body_id,
                                    @WebParam(name = "begin_year")String begin_year,
                                    @WebParam(name = "end_year")String end_year);

    String getRiverWaterRateExcessive(@WebParam(name = "water_body_id")String water_body_id,
                                      @WebParam(name = "xzqid")String xzqid,
                                      @WebParam(name = "begin_year")String begin_year,
                                      @WebParam(name = "end_year")String end_year);

    String getZhiliuListData(@WebParam(name = "water_body_id")String water_body_id);

    String getHeDuanListData(@WebParam(name = "water_body_id")String water_body_id);

    String getZhaMenListDate(@WebParam(name = "water_body_id")String water_body_id);

    String getShuiKuListDate(@WebParam(name = "water_body_id")String water_body_id);

    String getRiverSZPJCount(@WebParam(name = "water_body_id")String water_body_id,
                        @WebParam(name = "begin_year")String begin_year,
                        @WebParam(name = "end_year")String end_year);

    String getWaterFounctionSZPJCount(@WebParam(name = "water_body_id")String water_body_id,
                                      @WebParam(name = "riverID")String riverID ,
                                      @WebParam(name = "begin_year")String begin_year,
                                      @WebParam(name = "end_year")String end_year);

    String getWaterFounctionSZPJCountDetail(@WebParam(name = "water_body_id")String water_body_id,
                                            @WebParam(name = "riverId")String riverId,
                                            @WebParam(name = "begin_year")String begin_year,
                                            @WebParam(name = "end_year")String end_year);

    String getDMSZPJCount(@WebParam(name = "water_body_id")String water_body_id,
                          @WebParam(name = "waterFounctionID")String waterFounctionID ,
                          @WebParam(name = "begin_year")String begin_year,
                          @WebParam(name = "end_year")String end_year);

    String getDMSZPJCountDetail(@WebParam(name = "water_body_id")String water_body_id,
                                @WebParam(name = "waterFounctionID")String waterFounctionID ,
                                @WebParam(name = "begin_year")String begin_year,
                                @WebParam(name = "end_year")String end_year);

    String getLegendByType(@WebParam(name = "type") String type);

//    String getSZPJNutrition(@WebParam(name = "water_body_id")String water_body_id,
//                            @WebParam(name = "obj_id")String obj_id,
//                            @WebParam(name = "period_id")String period_id,
//                            @WebParam(name = "searchType")String searchType,
//                            @WebParam(name = "itemIDs")String itemIDs);

    String queryWrwMessage(@WebParam(name = "keyword")String keyword);

    String queryFxyMessage(@WebParam(name = "water_body_id")String water_body_id,
                           @WebParam(name = "keyword")String keyword);

    String queryZjMessage(@WebParam(name = "water_body_id")String water_body_id,
                          @WebParam(name = "keyword")String keyword);

    String queryYuanMessage(@WebParam(name = "water_body_id")String water_body_id,
                            @WebParam(name = "keyword")String keyword);

    String queryFaguiMessage(@WebParam(name = "water_body_id")String water_body_id,
                             @WebParam(name = "keyword")String keyword);

    String queryWaterPollutionEventCoordinate(@WebParam(name = "water_body_id")String water_body_id,
                                              @WebParam(name = "keyword")String keyword);

    String queryWrwMessageById(@WebParam(name = "wrwId")String wrwId);

    String queryFxyMessageById(@WebParam(name = "fxyId")String fxyId);

    String queryZjMessageById(@WebParam(name = "zjId")String zjId);

    String queryYuanMessageById(@WebParam(name = "planId")String planId);

    String queryFaguiMessageById(@WebParam(name = "lawId")String lawId);

    String saveWaterPollutionEvent(@WebParam(name = "pollutedJSON")String pollutedJSON);

    String deleteWaterPollutionEvent(@WebParam(name = "pollutionEventId")String pollutionEventId);

    String queryWaterPollutionEvent(@WebParam(name = "pollutionEventId")String pollutionEventId);
    void execYuqiaoDataModel(@WebParam(name="planId") String planId,@WebParam(name="filePath") String filePath) throws SQLException;

    String getNutritionItems(@WebParam(name = "water_body_id")String water_body_id);

    String getReservoirSZPJNutrition(@WebParam(name = "water_body_id")String water_body_id,
                                     @WebParam(name = "year")String year,
                                     @WebParam(name = "itemIDs")String itemIDs);

    String getReservoirSZPJYearList(@WebParam(name = "water_body_id")String water_body_id);

    String getReservoirSZPJNutritionCount(@WebParam(name = "water_body_id")String water_body_id,
                                          @WebParam(name = "begin_year") String begin_year,
                                          @WebParam(name = "end_year") String end_year,
                                          @WebParam(name = "itemIDs") String itemIDs);

    String getFlowBoundaryConditions(@WebParam(name = "planId") String planId);

    String getPollutionBoundaryConditions(@WebParam(name = "planId") String planId);

    String getTemperatureBoundaryConditions(@WebParam(name = "planId") String planId);

    String getDepthCurve(@WebParam(name = "planId")String planId,
                         @WebParam(name = "iCode")String iCode,
                         @WebParam(name = "jCode")String jCode);

    String getTemperatureCurve(@WebParam(name = "planId")String planId,
                               @WebParam(name = "iCode")String iCode,
                               @WebParam(name = "jCode")String jCode);

    String getPollutionCurve(@WebParam(name = "planId")String planId,
                             @WebParam(name = "iCode")String iCode,
                             @WebParam(name = "jCode")String jCode);

    String renderDepthResult(@WebParam(name = "planId") String planId,@WebParam(name = "month") int month);

    String renderTemperatureResult(@WebParam(name = "planId") String planId,@WebParam(name = "month") int month);

    String renderConcentrationResult(@WebParam(name = "planId") String planId,@WebParam(name = "month") int month);

    String renderFlowSpeedAndDirectionResult(@WebParam(name = "planId") String planId ,@WebParam(name = "month") int month);


    String getWangegeData() throws SQLException;

    String getMonth(@WebParam(name = "planId") String planId);
}
