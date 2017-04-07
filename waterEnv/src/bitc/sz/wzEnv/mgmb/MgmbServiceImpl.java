package bitc.sz.wzEnv.mgmb;

import bitc.sz.wzEnv.query.BaseDAO;
import bitc.sz.wzEnv.report.db.Executable;
import bitc.sz.wzEnv.report.db.ReportDAO;
import bitc.sz.wzEnv.report.db.SQLExecutor;
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
import java.util.*;

/**
 * Created by Administrator on 2015/12/4.
 */
@Service
@Transactional(propagation = Propagation.REQUIRED, rollbackFor = Exception.class)
public class MgmbServiceImpl implements MgmbService{
    private final static Logger logger = Logger.getLogger(MgmbServiceImpl.class);
    @Autowired
    private BaseDAO baseDAO;
    @Autowired
    private ReportDAO reportDAO;
    @Override
    public void saveSy(String dbName, final String sydId, final String stIds) throws Exception {
        //采取先把以前的都删除，然后插入新的
        final String stId[] = stIds.split(",");
        final String deleteSql = "delete from MGMB_REL_敏感目标重点水体 where 敏感目标对象_ID=?";
        final String insertSql = "insert into MGMB_REL_敏感目标重点水体 (敏感目标对象_ID,水体对象_ID) values(?,?)";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            executor.execute(new Executable() {
                public Object execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(deleteSql);
                    ps.setString(1, sydId);
                    ps.executeQuery();
                    if (!StringUtils.isEmpty(stIds)) {
                        PreparedStatement ps1 = connection.prepareStatement(insertSql);
                        for (String id : stId) {
                            ps1.setString(1, sydId);
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
    public List<String> getSy(String dbName, final String sydId) throws Exception {
        final String sql = "select  水体对象_ID from MGMB_REL_敏感目标重点水体 where 敏感目标对象_ID=?";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            List<String> list = executor.execute(new Executable<List<String>>() {
                public List<String> execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(sql);
                    ps.setString(1, sydId);
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
    public void saveQsk(String dbName, final String qysczId, final String qskIds) throws Exception {
        //采取先把以前的都删除，然后插入新的
        final String qskId[] = qskIds.split(",");
        final String deleteSql = "delete from MGMB_REL_取水口与取用水测站点 where 取用水测站点_ID=?";
        final String insertSql = "insert into MGMB_REL_取水口与取用水测站点 (取用水测站点_ID,取水口_ID) values(?,?)";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            executor.execute(new Executable() {
                public Object execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(deleteSql);
                    ps.setString(1, qysczId);
                    ps.executeQuery();
                    if (!StringUtils.isEmpty(qskIds)) {
                        PreparedStatement ps1 = connection.prepareStatement(insertSql);
                        for (String id : qskId) {
                            ps1.setString(1, qysczId);
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
    public List<String> getQsk(String dbName, final String qysczId) throws Exception {
        final String sql = "select  取水口_ID from MGMB_REL_取水口与取用水测站点 where 取用水测站点_ID=?";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            List<String> list = executor.execute(new Executable<List<String>>() {
                public List<String> execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(sql);
                    ps.setString(1, qysczId);
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
    public void saveQysh(String dbName, final String qysczId, final String qyshIds) throws Exception {
        //采取先把以前的都删除，然后插入新的
        final String qyshId[] = qyshIds.split(",");
        final String deleteSql = "delete from MGMB_REL_取水户与取用水测站点 where 取用水测站点_ID=?";
        final String insertSql = "insert into MGMB_REL_取水户与取用水测站点 (取用水测站点_ID,取用水户_ID) values(?,?)";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            executor.execute(new Executable() {
                public Object execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(deleteSql);
                    ps.setString(1, qysczId);
                    ps.executeQuery();
                    if (!StringUtils.isEmpty(qyshIds)) {
                        PreparedStatement ps1 = connection.prepareStatement(insertSql);
                        for (String id : qyshId) {
                            ps1.setString(1, qysczId);
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
    public List<String> getQysh(String dbName, final String qysczId) throws Exception {
        final String sql = "select  取用水户_ID from MGMB_REL_取水户与取用水测站点 where 取用水测站点_ID=?";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            List<String> list = executor.execute(new Executable<List<String>>() {
                public List<String> execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(sql);
                    ps.setString(1, qysczId);
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
    public void saveQysczd(String dbName, final String qysczId, final String qysczdIds) throws Exception {
        //采取先把以前的都删除，然后插入新的
        final String qysczdId[] = qysczdIds.split(",");
        final String deleteSql = "delete from MGMB_REL_取水测站测点关系表 where 取用水测点_ID=?";
        final String insertSql = "insert into MGMB_REL_取水测站测点关系表 (取用水测点_ID,取用水测站_ID) values(?,?)";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            executor.execute(new Executable() {
                public Object execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(deleteSql);
                    ps.setString(1, qysczId);
                    ps.executeQuery();
                    if (!StringUtils.isEmpty(qysczdIds)) {
                        PreparedStatement ps1 = connection.prepareStatement(insertSql);
                        for (String id : qysczdId) {
                            ps1.setString(1, qysczId);
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
    public List<String> getQysczd(String dbName, final String qysczId) throws Exception {
        final String sql = "select  取用水测站_ID from MGMB_REL_取水测站测点关系表 where 取用水测点_ID=?";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            List<String> list = executor.execute(new Executable<List<String>>() {
                public List<String> execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(sql);
                    ps.setString(1, qysczId);
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
    public void saveQskqysh(String dbName, final String qskId, final String qyshIds) throws Exception {
        //采取先把以前的都删除，然后插入新的
        final String qyshId[] = qyshIds.split(",");
        final String deleteSql = "delete from MGMB_REL_取水口与取用水户 where 取水口_ID=?";
        final String insertSql = "insert into MGMB_REL_取水口与取用水户 (取水口_ID,取用水户_ID) values(?,?)";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            executor.execute(new Executable() {
                public Object execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(deleteSql);
                    ps.setString(1, qskId);
                    ps.executeQuery();
                    if (!StringUtils.isEmpty(qyshIds)) {
                        PreparedStatement ps1 = connection.prepareStatement(insertSql);
                        for (String id : qyshId) {
                            ps1.setString(1, qskId);
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
    public List<String> getQskqysh(String dbName, final String qskId) throws Exception {
        final String sql = "select 取用水户_ID from MGMB_REL_取水口与取用水户 where 取水口_ID=?";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            List<String> list = executor.execute(new Executable<List<String>>() {
                public List<String> execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(sql);
                    ps.setString(1, qskId);
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
    public void saveQsjqysh(String dbName, final String qsjId, final String qyshIds) throws Exception {
        //采取先把以前的都删除，然后插入新的
        final String qyshId[] = qyshIds.split(",");
        final String deleteSql = "delete from MGMB_REL_取水口与取用水户 where 取水口_ID=?";
        final String insertSql = "insert into MGMB_REL_取水口与取用水户 (取水口_ID,取用水户_ID) values(?,?)";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            executor.execute(new Executable() {
                public Object execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(deleteSql);
                    ps.setString(1, qsjId);
                    ps.executeQuery();
                    if (!StringUtils.isEmpty(qyshIds)) {
                        PreparedStatement ps1 = connection.prepareStatement(insertSql);
                        for (String id : qyshId) {
                            ps1.setString(1, qsjId);
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
    public List<String> getQsjqysh(String dbName, final String qsjId) throws Exception {
        final String sql = "select 取用水户_ID from MGMB_REL_取水口与取用水户 where 取水口_ID=?";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            List<String> list = executor.execute(new Executable<List<String>>() {
                public List<String> execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(sql);
                    ps.setString(1, qsjId);
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
    public void savePwkqysh(String dbName, final String rhpwkId, final String qyshIds) throws Exception {
        //采取先把以前的都删除，然后插入新的
        final String qyshId[] = qyshIds.split(",");
        final String deleteSql = "delete from MGMB_REL_入河排污口与取用水户 where 入河排污口_ID=?";
        final String insertSql = "insert into MGMB_REL_入河排污口与取用水户 (入河排污口_ID,取用水户_ID) values(?,?)";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            executor.execute(new Executable() {
                public Object execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(deleteSql);
                    ps.setString(1, rhpwkId);
                    ps.executeQuery();
                    if (!StringUtils.isEmpty(qyshIds)) {
                        PreparedStatement ps1 = connection.prepareStatement(insertSql);
                        for (String id : qyshId) {
                            ps1.setString(1, rhpwkId);
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
    public List<String> getPwkqysh(String dbName, final String rhpwkId) throws Exception {
        final String sql = "select 取用水户_ID from MGMB_REL_入河排污口与取用水户 where 入河排污口_ID=?";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            List<String> list = executor.execute(new Executable<List<String>>() {
                public List<String> execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(sql);
                    ps.setString(1, rhpwkId);
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
    public void saveQysczpwk(String dbName, final String qysczId, final String pwkIds) throws Exception {
        //采取先把以前的都删除，然后插入新的
        final String pwkId[] = pwkIds.split(",");
        final String deleteSql = "delete from MGMB_REL_入河排污口与取水测站 where 取用水测站点_ID=?";
        final String insertSql = "insert into MGMB_REL_入河排污口与取水测站 (取用水测站点_ID,入河排污口_ID) values(?,?)";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            executor.execute(new Executable() {
                public Object execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(deleteSql);
                    ps.setString(1, qysczId);
                    ps.executeQuery();
                    if (!StringUtils.isEmpty(pwkIds)) {
                        PreparedStatement ps1 = connection.prepareStatement(insertSql);
                        for (String id : pwkId) {
                            ps1.setString(1, qysczId);
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
    public List<String> getQysczpwk(String dbName, final String qysczId) throws Exception {
        final String sql = "select 入河排污口_ID from MGMB_REL_入河排污口与取水测站 where 取用水测站点_ID=?";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            List<String> list = executor.execute(new Executable<List<String>>() {
                public List<String> execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(sql);
                    ps.setString(1, qysczId);
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
    public List<Map<String,Objects>> getDmCommonMsg(String dbName, final String hddmId) throws Exception {
        SQLExecutor executor = new SQLExecutor(dbName);
        final String sql = "SELECT 断面_ID,左岸经纬度坐标,右岸经纬度坐标,断面代码,断面名称,所在地,水体ID,水体名称,水功能区ID,水功能区名称,备注 FROM ZDWARNER.V_河道断面信息 WHERE 断面_ID = ?";
        try {
            List<Map<String,Objects>> list = executor.execute(new Executable<List<Map<String,Objects>>>() {
                public List<Map<String,Objects>> execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(sql);
                    ps.setString(1, hddmId);
                    ResultSet rs = ps.executeQuery();
                    List<Map<String,Objects>> list = new ArrayList<Map<String,Objects>>();
                    while (rs.next()) {
                        Map map = new HashMap();
                        map.put("断面_ID",rs.getString(1));
                        map.put("左岸经纬度坐标",rs.getString(2));
                        map.put("右岸经纬度坐标",rs.getString(3));
                        map.put("断面代码",rs.getString(4));
                        map.put("断面名称",rs.getString(5));
                        map.put("所在地",rs.getString(6));
                        map.put("水体ID",rs.getString(7));
                        map.put("水体名称",rs.getString(8));
                        map.put("水功能区ID",rs.getString(9));
                        map.put("水功能区名称",rs.getString(10));
                        map.put("备注",rs.getString(11));
                        list.add(map);
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
    public void saveSgnqSyd(String dbName, final String sgnqId, final String syIds) throws Exception {
        //采取先把以前的都删除，然后插入新的
        final String syId[] = syIds.split(",");
        final String deleteSql = "delete from mgmb_rel_水源地与水功能区 where 水功能区_ID=?";
        final String insertSql = "insert into mgmb_rel_水源地与水功能区 (水功能区_ID,水源地_ID) values(?,?)";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            executor.execute(new Executable() {
                public Object execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(deleteSql);
                    ps.setString(1, sgnqId);
                    ps.executeQuery();
                    if (!StringUtils.isEmpty(syIds)) {
                        PreparedStatement ps1 = connection.prepareStatement(insertSql);
                        for (String id : syId) {
                            ps1.setString(1, sgnqId);
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
    public List<String> getSgnqSyd(String dbName, final String sgnqId) throws Exception {
        final String sql = "select 水源地_ID from mgmb_rel_水源地与水功能区 where 水功能区_ID=?";
        SQLExecutor executor = new SQLExecutor(dbName);
        try {
            List<String> list = executor.execute(new Executable<List<String>>() {
                public List<String> execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(sql);
                    ps.setString(1, sgnqId);
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
    public List<Map<String, Objects>> getDmController(String dbName, final String hddmdm) throws Exception {
        SQLExecutor executor = new SQLExecutor(dbName);
        final String sql = "SELECT CSB.RB_CD,CSB.ST_DT 设置日期, CSB.MON_CONT   监测内容, CSB.CONT_MIN_Z 控制最低水位, CSB.CONT_MIN_Q 控制最小流量,CSB.CONT_QUNT  控制水质要求,CSB.NT 备注 FROM ZDWARNER.MGMB_河道断面信息简表 MGMB, ZDWARNER.WR_CSB_B CSB WHERE  MGMB.WR_SD_CODE = CSB.RB_CD AND CSB.RB_CD = ?";
        try {
            List<Map<String,Objects>> list = executor.execute(new Executable<List<Map<String,Objects>>>() {
                public List<Map<String,Objects>> execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(sql);
                    ps.setString(1, hddmdm);
                    ResultSet rs = ps.executeQuery();
                    List<Map<String,Objects>> list = new ArrayList<Map<String,Objects>>();
                    while (rs.next()) {
                        Map map = new HashMap();
                        map.put("RB_CD",rs.getString(1));
                        map.put("设置日期",rs.getString(2));
                        map.put("监测内容",rs.getString(3));
                        map.put("控制最低水位",rs.getString(4));
                        map.put("控制最小流量",rs.getString(5));
                        map.put("控制水质要求",rs.getString(6));
                        map.put("备注",rs.getString(7));
                        list.add(map);
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
    public List<Map<String, Objects>> getDmMonitor(String dbName, final String hddmdm) throws Exception {
        SQLExecutor executor = new SQLExecutor(dbName);
        final String sql = "SELECT WFZB.RB_CD ,WFZB.ST_DT 设置日期,WFZB.MON_CONT 监测内容,WFZB.US_WFC_CD 上游水功能区代码,UP.WFZ_NM 上游水功能区名称,WFZB.DOWN_WFC_CD 下游水功能区代码,DOWN.WFZ_NM 下游水功能区名称,WFZB.NT 备注 FROM ZDWARNER.MGMB_河道断面信息简表 MGMB,ZDWARNER.WR_WFZB_B  WFZB,ZDWARNER.WR_WFZ_B UP,ZDWARNER.WR_WFZ_B DOWN WHERE MGMB.WR_SD_CODE = WFZB.RB_CD AND WFZB.US_WFC_CD = UP.WFZ_CD(+)  AND WFZB.DOWN_WFC_CD = DOWN.WFZ_CD(+) AND WFZB.RB_CD = ?";
        try {
            List<Map<String,Objects>> list = executor.execute(new Executable<List<Map<String,Objects>>>() {
                public List<Map<String,Objects>> execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(sql);
                    ps.setString(1, hddmdm);
                    ResultSet rs = ps.executeQuery();
                    List<Map<String,Objects>> list = new ArrayList<Map<String,Objects>>();
                    while (rs.next()) {
                        Map map = new HashMap();
                        map.put("RB_CD",rs.getString(1));
                        map.put("设置日期",rs.getString(2));
                        map.put("监测内容",rs.getString(3));
                        map.put("上游水功能区代码",rs.getString(4));
                        map.put("上游水功能区名称",rs.getString(5));
                        map.put("下游水功能区代码",rs.getString(6));
                        map.put("下游水功能区名称",rs.getString(7));
                        map.put("备注",rs.getString(8));
                        list.add(map);
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
    public List<Map<String, Objects>> getDmAdmin(String dbName, final String hddmdm) throws Exception {
        SQLExecutor executor = new SQLExecutor(dbName);
        final String sql = "SELECT AB.RB_CD ," +
                "AB.AB_G        行政区界断面级别代码," +
                "       ABG.AB_G_NM    行政区界断面级别名称," +
                "       AB.US_AD_CD    上游行政区划代码," +
                "       UP.AD_NM       上游行政区划名称," +
                "       AB.DOWN_AD_CD  下游行政区划代码," +
                "       DOWN.AD_NM     下游行政区划名称," +
                "       AB.SUP_ORG_CD  上级主管单位代码," +
                "       ORG.ORG_NM     上级主管单位名称," +
                "       AB.FROM_ASS_DT 开始考核日期," +
                "       AB.MAJ_AI      主要考核指标," +
                "       AB.NT  备注" +
                "  FROM ZDWARNER.MGMB_河道断面信息简表 MGMB," +
                "       ZDWARNER.WR_AB_B               AB," +
                "       ZDWARNER.P_WR_AB_G             ABG," +
                "       ZDWARNER.WR_AD_B               UP," +
                "       ZDWARNER.WR_AD_B               DOWN," +
                "       ZDWARNER.WR_ADMA_B             ORG" +
                " WHERE MGMB.WR_SD_CODE = AB.RB_CD" +
                "   AND AB.AB_G = ABG.AB_G(+)" +
                "   AND AB.US_AD_CD = UP.AD_CD(+)" +
                "   AND AB.DOWN_AD_CD = DOWN.AD_CD(+)" +
                "   AND AB.SUP_ORG_CD = ORG.ORG_CD(+)"+
                "   AND AB.RB_CD = ?";
        try {
            List<Map<String,Objects>> list = executor.execute(new Executable<List<Map<String,Objects>>>() {
                public List<Map<String,Objects>> execute(Connection connection) throws SQLException {
                    PreparedStatement ps = connection.prepareStatement(sql);
                    ps.setString(1, hddmdm);
                    ResultSet rs = ps.executeQuery();
                    List<Map<String,Objects>> list = new ArrayList<Map<String,Objects>>();
                    while (rs.next()) {
                        Map map = new HashMap();
                        map.put("RB_CD",rs.getString(1));
                        map.put("行政区界断面级别代码",rs.getString(2));
                        map.put("行政区界断面级别名称",rs.getString(3));
                        map.put("上游行政区划代码",rs.getString(4));
                        map.put("上游行政区划名称",rs.getString(5));
                        map.put("下游行政区划代码",rs.getString(6));
                        map.put("下游行政区划名称",rs.getString(7));
                        map.put("上级主管单位代码",rs.getString(8));
                        map.put("上级主管单位名称",rs.getString(9));
                        map.put("开始考核日期",rs.getString(10));
                        map.put("主要考核指标",rs.getString(11));
                        map.put("备注",rs.getString(12));
                        list.add(map);
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
