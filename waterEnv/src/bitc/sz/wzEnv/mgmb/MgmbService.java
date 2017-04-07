package bitc.sz.wzEnv.mgmb;

import java.util.List;
import java.util.Map;
import java.util.Objects;

/**
 * Created by Administrator on 2015/12/4.
 */
public interface MgmbService {
    void saveSy(String dbName, String sydId, String stIds) throws Exception;

    List<String> getSy(String dbName, String sydId) throws Exception;

    void saveQsk(String dbName, String qysczId, String qskIds) throws Exception;

    List<String> getQsk(String dbName, String qysczId) throws Exception;

    void saveQysh(String dbName, String qysczId, String qyshIds) throws Exception;

    List<String> getQysh(String dbName, String qysczId) throws Exception;

    void saveQysczd(String dbName, String qysczId, String qysczdIds) throws Exception;

    List<String> getQysczd(String dbName, String qysczId) throws Exception;

    void saveQskqysh(String dbName, String qskId, String qyshIds) throws Exception;

    List<String> getQskqysh(String dbName, String qskId) throws Exception;

    void saveQsjqysh(String dbName, String qsjId, String qyshIds) throws Exception;

    List<String> getQsjqysh(String dbName, String qsjId) throws Exception;

    void savePwkqysh(String dbName, String rhpwkId, String qyshIds) throws Exception;

    List<String> getPwkqysh(String dbName, String rhpwkId) throws Exception;

    void saveQysczpwk(String dbName, String qysczId, String pwkIds) throws Exception;

    List<String> getQysczpwk(String dbName, String qysczId) throws Exception;

    List<Map<String,Objects>> getDmCommonMsg(String dbName,String hddmId)throws Exception;

    List<Map<String,Objects>> getDmController(String dbName,String hddmdm)throws Exception;

    List<Map<String,Objects>> getDmMonitor(String dbName,String hddmdm)throws Exception;

    List<Map<String,Objects>> getDmAdmin(String dbName,String hddmdm)throws Exception;

    void saveSgnqSyd(String dbName, String sgnqId, String syIds) throws Exception;

    List<String> getSgnqSyd(String dbName, String sgnqId) throws Exception;

}
