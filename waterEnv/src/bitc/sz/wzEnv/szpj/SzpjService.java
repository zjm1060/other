package bitc.sz.wzEnv.szpj;


import bitc.sz.wzEnv.szpj.model.*;
import bitc.sz.wzEnv.util.ExtjsResponse;

import java.io.IOException;
import java.sql.SQLException;
import java.util.List;
import java.util.Map;

/**
 * Created by Administrator on 2016/3/9.
 */
public interface SzpjService {
    List<SzType> getSzType() throws SQLException;

    List<ZbType> getZbType(String DOMAIN_ID) throws Exception;

    List<Places> getPlaces(String waterBodyId,String keywords) throws Exception;

    List<Map<String,String>> getWaterBody() throws SQLException;

    List<ZbValue> getZbValue(String DOMAIN_ID,String TYPEID,String MONITORID,String MONITORSIDE,String MONITORDATE) throws Exception;

    List<QyPoint> getQyPoint() throws Exception;

    void saveSzpj(long gcTime,String placeId,String qyPoint,String shuiwei,String liuliang,String shuiwen,String qiwen,String remark,String addData) throws SQLException, IOException;

    void deleteSzpj(Long gcTime,String placeId,String qyPoint) throws SQLException;

    void updateSzpj(String formid,Long gcTime,String placeId,String qyPoint,String shuiwei,String liuliang,String shuiwen,String qiwen,String remark,String editData,String addData) throws SQLException;
    //排污口水质监测
    List<Places> getPwPlaces(String waterBodyId,String keywords) throws Exception;
    //排污口监测保存
    void savePwkSzpj(long gcTime,String placeId,String qyPoint,String shuiwei,String liuliang,String shuiwen,String qiwen,String remark,String addData,String paifang) throws SQLException, IOException;
    //排污口监测更新
    void updatePwkSzpj(String formid,Long gcTime,String placeId,String qyPoint,String shuiwei,String liuliang,String shuiwen,String qiwen,String remark,String editData,String addData,String paifang) throws SQLException;

    //污染物入河量
    List<Map<String,String>> getWrwZbValue(String MONITORID,String MONITORDATE) throws SQLException;

    void saveSzy(Long jcTime,String waterFunctionID,String addData) throws SQLException;

    void deleteSzy(Long jcTime,String waterFunctionID) throws SQLException;

    void updateSzy(Long jcTime,String waterFunctionID,String editData,String beforeJcTime,String beforeWaterFounctionID) throws SQLException;
}
