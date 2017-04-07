package bitc.sz.wzEnv.zdst;

import bitc.sz.wzEnv.report.model.FormSetValueField;
import bitc.sz.wzEnv.report.model.Report;

import java.sql.SQLException;
import java.util.List;

/**
 * Created by zsz on 2015/12/2.
 */
public interface ZdstService {
    
    void saveStWithgcz(String dbName, String swczId, String stIds) throws Exception;

    void savePwWithgcz(String dbName, String swczId, String pwIds) throws Exception;

    void saveSt(String dbName, String stId, String syIds) throws Exception;

    List<String> getStWithgcz(String dbName, String swczId) throws Exception;

    List<String> getPwWithgcz(String dbName, String swczId) throws Exception;

    List<String> getSt(String dbName, String stId) throws Exception;

    List<FormSetValueField> getSwczscxm(String dbName, String scxmId) throws Exception;

    void updateHPRelatedRiver(Report report,String hupoID,String riverCode,String riverDirect,String heliuID) throws Exception;

    void insertHPRelatedRiver(Report report,String hupoID,String riverCode,String riverDirect)throws Exception;

    void deleteHPRelatedRiver(Report report,String hupoID, String riverCode) throws Exception;

    void deleteSKRelatedRiver(Report report,String shuikuID, String riverCode) throws Exception;

    void insertSKRelatedRiver(Report report,String hupoID,String riverCode,String riverDirect)throws Exception;

    void updateSKRelatedRiver(Report report,String shuikuID,String riverCode,String riverDirect,String heliuID) throws Exception;

    List<String> getStWithsdz(String dbName, String sdzId) throws Exception;

    void saveStWithsdz(String dbName, String sdzId, String stIds) throws Exception;

    void saveSydWithgcz(String dbName, String swczId, String sydIds) throws Exception;

    List<String> getSydWithgcz(String dbName, String swczId) throws Exception;

    void saveHddmWithgcz(String dbName, String swczId, String hddmIds) throws Exception;

    List<String> getHddmWithgcz(String dbName, String swczId) throws Exception;

    void saveSgnqWithgcz(String dbName, String swczId, String sgnqIds) throws Exception;

    List<String> getSgnqWithgcz(String dbName, String swczId) throws Exception;

    List<String> getStWithllb(String dbName, String lhbId) throws Exception;

    void saveStWithlhb(String dbName, String lhbId, String stIds) throws Exception;
}
