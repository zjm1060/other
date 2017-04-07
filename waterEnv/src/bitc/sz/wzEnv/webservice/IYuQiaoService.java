package bitc.sz.wzEnv.webservice;

import javax.jws.WebParam;
import javax.jws.WebService;
import java.io.IOException;
import java.sql.SQLException;

/**
 * Created by zsz on 2016/12/15.
 */
@WebService(endpointInterface = "bitc.sz.wzEnv.webservice.IYuQiaoService", serviceName = "yuqiao")
public interface IYuQiaoService {

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

    String renderFlowSpeedAndDirectionResult(@WebParam(name = "planId") String planId,@WebParam(name = "month") int month);


    String getWangegeData() throws SQLException;
}
