package bitc.sz.wzEnv.zdst;

import bitc.sz.wzEnv.log.Log;
import bitc.sz.wzEnv.query.BaseDAO;
import bitc.sz.wzEnv.report.db.Executable;
import bitc.sz.wzEnv.report.db.ReportDAO;
import bitc.sz.wzEnv.report.db.SQLExecutor;
import bitc.sz.wzEnv.report.model.FormSetValueField;
import bitc.sz.wzEnv.report.model.Report;
import org.apache.commons.lang.StringUtils;
import org.apache.log4j.Logger;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Propagation;
import org.springframework.transaction.annotation.Transactional;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by zsz on 2015/12/2.
 */
@Service
@Transactional(propagation = Propagation.REQUIRED, rollbackFor = Exception.class)
public class ZdstServiceImpl implements ZdstService {
    private final static Logger logger = Logger.getLogger(ZdstServiceImpl.class);
    @Autowired
    private BaseDAO baseDAO;
    @Autowired
    private ReportDAO reportDAO;
    @Override
    public void insertHPRelatedRiver(Report report, String hupoID, String riverCode, String riverDirect) throws Exception {
        SQLExecutor sqlExecutor=null;
        try {
            sqlExecutor = new SQLExecutor(report.getDbName());
            sqlExecutor.execute("insert into zdst_rel_河流湖泊关系表 (湖泊_ID,河流_ID,出入标记) values (?,?,?)",new Object[]{hupoID,riverCode,riverDirect});
            sqlExecutor.commit();
        }catch (Exception e){
            sqlExecutor.rollback();
            throw e;
        }finally {
            sqlExecutor.close();
        }
    }
    @Override
    public List<String> getStWithsdz(String dbName, final String sdzId) throws Exception {
        final String sql = "select 水体_ID from zdst_rel_水利工程项目与水体 where 水利工程项目_id=?";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            List<String> list = executor.execute(new Executable<List<String>>() {
                public List<String> execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(sql);
                    ps.setString(1, sdzId);
                    ResultSet rs = ps.executeQuery();
                    List<String> list = new ArrayList<String>();
                    while (rs.next()) {
                        list.add(rs.getString(1));
                    }
                    return list;
                }
            });
            return list;
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            executor.rollback();
            throw  e;
        } finally {
            executor.close();
        }
    }

    @Override
    public void saveStWithsdz(String dbName, final String sdzId, final String stIds) throws Exception {
        //采取先把以前的都删除，然后插入新的
        final String stId[] = stIds.split(",");
        final String deleteSql = "delete from zdst_rel_水利工程项目与水体 where 水利工程项目_id=?";
        final String insertSql = "insert into zdst_rel_水利工程项目与水体 (水体_ID,水利工程项目_id) values(?,?)";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            executor.execute(new Executable() {
                public Object execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(deleteSql);
                    ps.setString(1, sdzId);
                    ps.executeQuery();
                    if (!StringUtils.isEmpty(stIds)) {
                        PreparedStatement ps1 = connection.prepareStatement(insertSql);
                        for (String id : stId) {
                            ps1.setString(1, id);
                            ps1.setString(2, sdzId);
                            ps1.addBatch();
                        }
                        ps1.executeBatch();
                    }
                    return null;
                }
            });
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            executor.rollback();
            throw  e;
        } finally {
            executor.close();
        }
    }

    @Override
    public void deleteHPRelatedRiver(Report report, String hupoID, String riverCode) throws Exception{
        SQLExecutor sqlExecutor=null;
        try {
            sqlExecutor = new SQLExecutor(report.getDbName());
            sqlExecutor.execute("delete from zdst_rel_河流湖泊关系表 where 河流_ID = ? and 湖泊_ID = ?",new Object[]{riverCode,hupoID});
            sqlExecutor.commit();
        }catch (Exception e){
            sqlExecutor.rollback();
            throw e;
        }finally {
            sqlExecutor.close();
        }
    }
    @Override
    public void saveStWithgcz(String dbName, final String swczId, final String stIds) throws Exception {
        //采取先把以前的都删除，然后插入新的
        final String stId[] = stIds.split(",");
        final String deleteSql = "delete from zdst_rel_水体与观测站关系表 where 水文测站_id=?";
        final String insertSql = "insert into zdst_rel_水体与观测站关系表 (水体_ID,水文测站_id) values(?,?)";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            executor.execute(new Executable() {
                public Object execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(deleteSql);
                    ps.setString(1, swczId);
                    ps.executeQuery();
                    if (!StringUtils.isEmpty(stIds)) {
                        PreparedStatement ps1 = connection.prepareStatement(insertSql);
                        for (String id : stId) {
                            ps1.setString(1, id);
                            ps1.setString(2, swczId);
                            ps1.addBatch();
                        }
                        ps1.executeBatch();
                    }
                    return null;
                }
            });
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            executor.rollback();
            throw e;
        } finally {
            executor.close();
        }

    }
    @Override
    public void savePwWithgcz(String dbName, final String swczId, final String pwIds) throws Exception {
        //采取先把以前的都删除，然后插入新的
        final String stId[] = pwIds.split(",");
        final String deleteSql = "delete from MGMB_REL_入河排污口与水文测站 where 水文测站_id=?";
        final String insertSql = "insert into MGMB_REL_入河排污口与水文测站 (入河排污口_ID,水文测站_ID) values(?,?)";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            executor.execute(new Executable() {
                public Object execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(deleteSql);
                    ps.setString(1, swczId);
                    ps.executeQuery();
                    if (!StringUtils.isEmpty(pwIds)) {
                        PreparedStatement ps1 = connection.prepareStatement(insertSql);
                        for (String id : stId) {
                            ps1.setString(1, id);
                            ps1.setString(2, swczId);
                            ps1.addBatch();
                        }
                        ps1.executeBatch();
                    }
                    return null;
                }
            });
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            executor.rollback();
            throw e;
        } finally {
            executor.close();
        }

    }
    @Override
    public void saveHddmWithgcz(String dbName, final String swczId, final String hddmIds) throws Exception {
        //采取先把以前的都删除，然后插入新的
        final String stId[] = hddmIds.split(",");
        final String deleteSql = "delete from MGMB_REL_河道断面与水文测站 where 水文测站_id=?";
        final String insertSql = "insert into MGMB_REL_河道断面与水文测站 (河道断面_ID,水文测站_ID) values(?,?)";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            executor.execute(new Executable() {
                public Object execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(deleteSql);
                    ps.setString(1, swczId);
                    ps.executeQuery();
                    if (!StringUtils.isEmpty(hddmIds)) {
                        PreparedStatement ps1 = connection.prepareStatement(insertSql);
                        for (String id : stId) {
                            ps1.setString(1, id);
                            ps1.setString(2, swczId);
                            ps1.addBatch();
                        }
                        ps1.executeBatch();
                    }
                    return null;
                }
            });
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            executor.rollback();
            throw e;
        } finally {
            executor.close();
        }
    }
    @Override
    public void saveSydWithgcz(String dbName, final String swczId, final String sydIds) throws Exception {
        //采取先把以前的都删除，然后插入新的
        final String stId[] = sydIds.split(",");
        final String deleteSql = "delete from MGMB_REL_水源地与水文测站 where 水文测站_id=?";
        final String insertSql = "insert into MGMB_REL_水源地与水文测站 (水源地_ID,水文测站_ID) values(?,?)";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            executor.execute(new Executable() {
                public Object execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(deleteSql);
                    ps.setString(1, swczId);
                    ps.executeQuery();
                    if (!StringUtils.isEmpty(sydIds)) {
                        PreparedStatement ps1 = connection.prepareStatement(insertSql);
                        for (String id : stId) {
                            ps1.setString(1, id);
                            ps1.setString(2, swczId);
                            ps1.addBatch();
                        }
                        ps1.executeBatch();
                    }
                    return null;
                }
            });
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            executor.rollback();
            throw e;
        } finally {
            executor.close();
        }

    }
    @Override
    public void saveSgnqWithgcz(String dbName, final String swczId, final String sgnqIds) throws Exception {
        //采取先把以前的都删除，然后插入新的
        final String stId[] = sgnqIds.split(",");
        final String deleteSql = "delete from mgmb_rel_水功能区与水文测站 where 水文测站_id=?";
        final String insertSql = "insert into mgmb_rel_水功能区与水文测站 (水功能区_ID,水文测站_ID) values(?,?)";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            executor.execute(new Executable() {
                public Object execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(deleteSql);
                    ps.setString(1, swczId);
                    ps.executeQuery();
                    if (!StringUtils.isEmpty(sgnqIds)) {
                        PreparedStatement ps1 = connection.prepareStatement(insertSql);
                        for (String id : stId) {
                            ps1.setString(1, id);
                            ps1.setString(2, swczId);
                            ps1.addBatch();
                        }
                        ps1.executeBatch();
                    }
                    return null;
                }
            });
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            executor.rollback();
            throw e;
        } finally {
            executor.close();
        }

    }
    @Override
    public List<String> getStWithgcz(String dbName, final String swczId) throws Exception {
        final String sql = "select 水体_ID from zdst_rel_水体与观测站关系表 where 水文测站_id=?";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            List<String> list = executor.execute(new Executable<List<String>>() {
                public List<String> execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(sql);
                    ps.setString(1, swczId);
                    ResultSet rs = ps.executeQuery();
                    List<String> list = new ArrayList<String>();
                    while (rs.next()) {
                        list.add(rs.getString(1));
                    }
                    return list;
                }
            });
            return list;
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            executor.rollback();
            throw e;
        } finally {
            executor.close();
        }
    }
    @Override
    public List<String> getSgnqWithgcz(String dbName, final String swczId) throws Exception {
        final String sql = "select 水功能区_id from mgmb_rel_水功能区与水文测站 where 水文测站_id=?";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            List<String> list = executor.execute(new Executable<List<String>>() {
                public List<String> execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(sql);
                    ps.setString(1, swczId);
                    ResultSet rs = ps.executeQuery();
                    List<String> list = new ArrayList<String>();
                    while (rs.next()) {
                        list.add(rs.getString(1));
                    }
                    return list;
                }
            });
            return list;
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            executor.rollback();
            throw e;
        } finally {
            executor.close();
        }
    }

    @Override
    public List<String> getStWithllb(String dbName, final String lhbId) throws Exception {
        final String sql = "select 水体_ID from zdst_rel_水利工程项目与水体 where 水利工程项目_id=?";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            List<String> list = executor.execute(new Executable<List<String>>() {
                public List<String> execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(sql);
                    ps.setString(1, lhbId);
                    ResultSet rs = ps.executeQuery();
                    List<String> list = new ArrayList<String>();
                    while (rs.next()) {
                        list.add(rs.getString(1));
                    }
                    return list;
                }
            });
            return list;
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            executor.rollback();
            throw  e;
        } finally {
            executor.close();
        }
    }
    @Override
    public void saveStWithlhb(String dbName, final String lhbId, final String stIds) throws Exception {
        //采取先把以前的都删除，然后插入新的
        final String stId[] = stIds.split(",");
        final String deleteSql = "delete from zdst_rel_水利工程项目与水体 where 水利工程项目_id=?";
        final String insertSql = "insert into zdst_rel_水利工程项目与水体 (水体_ID,水利工程项目_id) values(?,?)";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            executor.execute(new Executable() {
                public Object execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(deleteSql);
                    ps.setString(1, lhbId);
                    ps.executeQuery();
                    if (!StringUtils.isEmpty(stIds)) {
                        PreparedStatement ps1 = connection.prepareStatement(insertSql);
                        for (String id : stId) {
                            ps1.setString(1, id);
                            ps1.setString(2, lhbId);
                            ps1.addBatch();
                        }
                        ps1.executeBatch();
                    }
                    return null;
                }
            });
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            executor.rollback();
            throw  e;
        } finally {
            executor.close();
        }
    }
    @Override
    public List<String> getHddmWithgcz(String dbName, final String swczId) throws Exception {
        final String sql = "select 河道断面_ID from MGMB_REL_河道断面与水文测站 where 水文测站_id=?";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            List<String> list = executor.execute(new Executable<List<String>>() {
                public List<String> execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(sql);
                    ps.setString(1, swczId);
                    ResultSet rs = ps.executeQuery();
                    List<String> list = new ArrayList<String>();
                    while (rs.next()) {
                        list.add(rs.getString(1));
                    }
                    return list;
                }
            });
            return list;
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            executor.rollback();
            throw e;
        } finally {
            executor.close();
        }
    }
    @Override
    public List<String> getPwWithgcz(String dbName, final String swczId) throws Exception {
        final String sql = "select 入河排污口_ID from MGMB_REL_入河排污口与水文测站 where 水文测站_ID=?";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            List<String> list = executor.execute(new Executable<List<String>>() {
                public List<String> execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(sql);
                    ps.setString(1, swczId);
                    ResultSet rs = ps.executeQuery();
                    List<String> list = new ArrayList<String>();
                    while (rs.next()) {
                        list.add(rs.getString(1));
                    }
                    return list;
                }
            });
            return list;
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            executor.rollback();
            throw e;
        } finally {
            executor.close();
        }
    }
    @Override
    public List<String> getSydWithgcz(String dbName, final String swczId) throws Exception {
        final String sql = "select 水源地_ID from MGMB_REL_水源地与水文测站 where 水文测站_ID=?";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            List<String> list = executor.execute(new Executable<List<String>>() {
                public List<String> execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(sql);
                    ps.setString(1, swczId);
                    ResultSet rs = ps.executeQuery();
                    List<String> list = new ArrayList<String>();
                    while (rs.next()) {
                        list.add(rs.getString(1));
                    }
                    return list;
                }
            });
            return list;
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            executor.rollback();
            throw e;
        } finally {
            executor.close();
        }
    }
    @Override
    public List<FormSetValueField> getSwczscxm(String dbName, final String scxmId) throws Exception {
        final String sql = "select * from wr_stat_item_b where stcd=?";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            List<FormSetValueField> list = executor.execute(new Executable<List<FormSetValueField>>() {
                public List<FormSetValueField> execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(sql);
                    ps.setString(1, scxmId);
                    ResultSet rs = ps.executeQuery();
                    List<FormSetValueField> list = new ArrayList<FormSetValueField>();
                    String[] valueFields = {"STCD", "ITEM_P", "ITEM_E", "ITEM_Z", "ITEM_Q", "ITEM_GW_DEP", "ITEM_WQ", "ITEM_LAND_SUBS", "ITEM_SAND", "ITEM_ICESL", "ITEM_TIDE", "ITEM_OTHE", "TS", "NT"};
                    if (rs.next()) {
                        for (int i = 1; i <= valueFields.length; i++) {
                            FormSetValueField field = new FormSetValueField(valueFields[i - 1], rs.getString(i), rs.getString(1));
                            list.add(field);
                        }
                    }
                    return list;
                }
            });
            return list;
        } catch (Exception e) {
            executor.rollback();
            logger.error(e.getMessage(), e);
            throw e;
        } finally {
            executor.close();
        }
    }
    @Override
    public void updateHPRelatedRiver(Report report, String hupoID, String riverCode, String riverDirect, String heliuID) throws Exception {
        SQLExecutor sqlExecutor=null;
        try {
            sqlExecutor = new SQLExecutor(report.getDbName());
            sqlExecutor.execute("update zdst_rel_河流湖泊关系表 set 河流_ID = ? ,出入标记 = ? where 湖泊_ID = ? and 河流_ID = ?",new Object[]{riverCode,riverDirect,hupoID,heliuID});
            sqlExecutor.commit();
        }catch (Exception e){
            sqlExecutor.rollback();
            throw e;
        }finally {
            sqlExecutor.close();
        }
    }

    @Override
    public void deleteSKRelatedRiver(Report report, String shuikuID, String riverCode) throws Exception {
        SQLExecutor sqlExecutor=null;
        try {
            sqlExecutor = new SQLExecutor(report.getDbName());
            sqlExecutor.execute("delete from zdst_rel_水库河流关系表 where 河流_ID = ? and 水库_ID = ?",new Object[]{riverCode,shuikuID});
            sqlExecutor.commit();
        }catch (Exception e){
            sqlExecutor.rollback();
            throw e;
        }finally {
            sqlExecutor.close();
        }
    }

    @Override
    public void insertSKRelatedRiver(Report report, String shuikuID, String riverCode, String riverDirect) throws Exception {
        SQLExecutor sqlExecutor=null;
        try {
            sqlExecutor = new SQLExecutor(report.getDbName());
            sqlExecutor.execute("insert into zdst_rel_水库河流关系表 (水库_ID,河流_ID,出入库标记) values (?,?,?)",new Object[]{shuikuID,riverCode,riverDirect});
            sqlExecutor.commit();
        }catch (Exception e){
            sqlExecutor.rollback();
            throw e;
        }finally {
            sqlExecutor.close();
        }
    }

    @Override
    public void updateSKRelatedRiver(Report report, String shuikuID, String riverCode, String riverDirect,String heliuID) throws Exception {
        SQLExecutor sqlExecutor=null;
        try {
            sqlExecutor = new SQLExecutor(report.getDbName());
            sqlExecutor.execute("update zdst_rel_水库河流关系表 set 河流_ID = ? ,出入库标记 = ? where 水库_ID = ? and 河流_ID = ?",new Object[]{riverCode,riverDirect,shuikuID,heliuID});
            sqlExecutor.commit();
        }catch (Exception e){
            sqlExecutor.rollback();
            throw e;
        }finally {
            sqlExecutor.close();
        }
    }

    @Override
    public void saveSt(String dbName, final String stId, final String syIds) throws Exception {
        //采取先把以前的都删除，然后插入新的
        final String syId[] = syIds.split(",");
        final String deleteSql = "delete from MGMB_REL_敏感目标重点水体 where 水体对象_ID=?";
        final String insertSql = "insert into MGMB_REL_敏感目标重点水体 (水体对象_ID,敏感目标对象_ID) values(?,?)";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            executor.execute(new Executable() {
                public Object execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(deleteSql);
                    ps.setString(1, stId);
                    ps.executeQuery();
                    if (!StringUtils.isEmpty(syIds)) {
                        PreparedStatement ps1 = connection.prepareStatement(insertSql);
                        for (String id : syId) {
                            ps1.setString(1, stId);
                            ps1.setString(2, id);
                            ps1.addBatch();
                        }
                        ps1.executeBatch();
                    }
                    return null;
                }
            });
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            executor.rollback();
            throw e;
        } finally {
            executor.close();
        }
    }

    @Override
    public List<String> getSt(String dbName, final String stId) throws Exception {
        final String sql = "select  敏感目标对象_ID from MGMB_REL_敏感目标重点水体 where 水体对象_ID=?";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            List<String> list = executor.execute(new Executable<List<String>>() {
                public List<String> execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(sql);
                    ps.setString(1, stId);
                    ResultSet rs = ps.executeQuery();
                    List<String> list = new ArrayList<String>();
                    while (rs.next()) {
                        list.add(rs.getString(1));
                    }
                    return list;
                }
            });
            return list;
        } catch (Exception e) {
            logger.error(e.getMessage(), e);
            executor.rollback();
            throw  e;
        } finally {
            executor.close();
        }
    }
}
