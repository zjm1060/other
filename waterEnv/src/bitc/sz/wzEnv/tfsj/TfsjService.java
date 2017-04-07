package bitc.sz.wzEnv.tfsj;

import bitc.sz.wzEnv.query.PagingInfo;
import bitc.sz.wzEnv.util.ExtjsResponse;
import org.springframework.web.multipart.MultipartFile;

import java.io.BufferedInputStream;
import java.io.IOException;
import java.sql.SQLException;
import java.util.List;
import java.util.Map;

/**
 * Created by Administrator on 2016/8/30.
 */
public interface TfsjService {
    //污染物
    ExtjsResponse newWrwProperty(String number,String itemName) throws SQLException;

    ExtjsResponse updateWrwItemNameProperty(String itemId,String number,String itemName) throws SQLException;

    ExtjsResponse updateWrwProperty(String itemId,String number,String itemName) throws SQLException;

    Map<String, Object> getWrwPropertyMessage() throws SQLException;

    Map<String,Object> queryWrwMessage(String keyword,PagingInfo info) throws SQLException;

    Map<String,Object> getwrwQueryGridColums() throws SQLException;

    List<Map<String,String>> queryWrwDictionaryMessage(String wrwId) throws SQLException;

    ExtjsResponse saveWrwDictionary(String wrwId , String editWrwData,String addWrwData) throws SQLException;

    ExtjsResponse deleteWrwDictionary(String wrwId) throws SQLException;

    //风险源
    List<Map<String,String>> queryFxyDictionaryMessage(String fxyId) throws SQLException;

    ExtjsResponse newFxyProperty(String number,String itemName) throws SQLException;

    ExtjsResponse updateFxyProperty(String itemId,String startNumber,String currentNumber,String currentItemName) throws SQLException;

    Map<String, Object> getFxyPropertyMessage() throws SQLException;

    Map<String,Object> getfxyQueryGridColums() throws SQLException;

    Map<String,Object> queryFxyMessage(String keyword,PagingInfo info) throws SQLException;

    ExtjsResponse saveFxyDictionary(String fxyId, String coordinate, String editFxyData, String addFxyData) throws SQLException;

    ExtjsResponse deleteFxyDictionary(String fxyId) throws SQLException;

    List<String> getRelatedWaterBody(String id) throws SQLException;

    void saveRelatedWaterBody(String id,String waterBodyIds) throws SQLException;

    //专家库
    List<Map<String,String>> queryZjDictionaryMessage(String zjId) throws SQLException;

    ExtjsResponse newZjProperty(String number,String itemName) throws SQLException;

    ExtjsResponse updateZjProperty(String itemId,String startNumber,String currentNumber,String currentItemName) throws SQLException;

    Map<String, Object> getZjPropertyMessage() throws SQLException;

    Map<String,Object> getzjQueryGridColums() throws SQLException;

    Map<String,Object> queryZjMessage(String keyword,PagingInfo info) throws SQLException;

    void saveZjDictionary(String zjId, String editZjData, String addZjData,MultipartFile photo) throws SQLException;

    BufferedInputStream getZjPhoto(String zjId) throws SQLException;

    ExtjsResponse deleteZjDictionary(String zjId) throws SQLException;

    //应急预案和法规

    Map<String,Object> queryPlanAndLawMessage(String keyword,PagingInfo info,String typeId) throws SQLException;

    void savePlanAndLaws(String planAndLawsId,String planAndLawsName,Long planAndLawsTime,String planAndLawsType,MultipartFile file) throws SQLException;

    ExtjsResponse deletePlanAndLaws(String planAndLawsId) throws SQLException;

    Map<String,Object> downLoadPlanAndLaws(String planAndLawsId) throws SQLException, IOException;
}
