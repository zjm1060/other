package bitc.sz.wzEnv.service;

import java.io.IOException;
import java.sql.SQLException;
import java.util.List;
import java.util.Map;

/**
 * Created by zsz on 2016/7/4.
 */
public interface ModelInput {
    List<Map<String,String>> getZhiliuListData(String water_body_id) throws Exception;

    List<Map<String,String>> getHeDuanListData(String water_body_id) throws Exception;

    List<Map<String,Object>> getZhaMenListDate(String water_body_id) throws Exception;

    List<Map<String,String>> getZhaMenNameAndDmId() throws Exception;

    List<Map<String,Object>> getShuiKuListDate(String water_body_id) throws Exception;

    List<Map<String,String>> getBianjieListData(String water_body_id,String plan_id) throws Exception;

    List<Map<String,String>> getHedaoListData() throws Exception;

    List<Map<String,String>> getDuanMianListData(int hedao) throws Exception;

    boolean writeEfdcFile(String planId,String path) throws IOException, SQLException;

    boolean writeAserFile(String planId, String path) throws Exception;

    boolean wirteDserFile(String planId, String path) throws IOException, SQLException;

    boolean wirteQserFile(String planId, String path) throws IOException, SQLException;

    boolean writeTserFile(String planId, String path) throws IOException, SQLException;

    boolean writeTempFile(String planId, String path) throws IOException, SQLException;

    boolean writeWserFile(String planId, String path) throws IOException, SQLException;

    boolean writeDyeFile(String planId, String path) throws IOException, SQLException;

//    List<String> getBianJieDate() throws Exception;
}
