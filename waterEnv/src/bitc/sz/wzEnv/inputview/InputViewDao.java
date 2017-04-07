package bitc.sz.wzEnv.inputview;

import bitc.sz.wzEnv.query.BaseDAO;
import com.alibaba.fastjson.JSONObject;
import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;

import javax.sql.DataSource;
import java.sql.*;
import java.sql.Date;
import java.text.SimpleDateFormat;
import java.util.*;

/**
 * Created by zsz on 2016/8/15.
 */
@Repository
public class InputViewDao extends BaseDAO {
    @Autowired
    private DataSource basicDataSource;

    public String addModelPlan(String plan_name, String plan_makeTime, String creator, String run_year, String run_obj_id, String run_segment, String plan_description) throws Exception {
        Connection connection = null;
        PreparedStatement ps = null;
        SimpleDateFormat ft = new SimpleDateFormat("yyyy-MM-dd");
        String plan_id = UUID.randomUUID().toString().replace("-", "");
        String sql = "insert into sdlm_biz_方案定义表 (plan_id,方案名称,计算目标_id,计算年份,描述,创建日期,创建人,临时状态标识,河段名称)values(?,?,?,?,?,?,?,?,?)";
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1, plan_id);
            ps.setString(2, plan_name);
            ps.setString(3, run_obj_id);
            ps.setString(4, run_year);
            ps.setString(5, plan_description);
            ps.setDate(6, new Date(ft.parse(plan_makeTime).getTime()));
            ps.setString(7, creator);
            ps.setInt(8, 0);
            ps.setString(9, run_segment);
            ps.execute();
            return plan_id;
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (ps != null)
                ps.close();
            if (connection != null)
                connection.close();
        }
    }

    public String updateModelPlan(String plan_id, String plan_name, String run_year, String run_segment, String plan_description) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        Calendar c = Calendar.getInstance();
        String sql = "update sdlm_biz_方案定义表 set 方案名称= ?,  计算年份 = ?,描述 = ?, 河段名称 = ? ,临时状态标识 = 0 where plan_id = ?";
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1, plan_name);
            ps.setString(2, run_year);
            ps.setString(3, plan_description);
            ps.setString(4, run_segment);
            ps.setString(5, plan_id);
            ps.execute();
            return plan_id;
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (ps != null)
                ps.close();
            if (connection != null)
                connection.close();
        }
    }

    public String updatePlanStep(String plan_id, String step_id, String step_name, String model_code, String model_description) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        Calendar c = Calendar.getInstance();
        String sql = "update sdlm_biz_方案定义表 set 步骤名称= ?, model_code = ?, 描述 = ?,状态变更时间=? where step_id = ?";
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1, step_name);
            ps.setString(2, model_code);
            ps.setString(3, model_description);
            ps.setDate(4, new Date(c.getTimeInMillis()));
            ps.setString(5, step_id);
            ps.execute();
            return plan_id;
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (ps != null)
                ps.close();
            if (connection != null)
                connection.close();
        }
    }

    public void addPlanStep(String plan_id, String step_id, String modelId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        Calendar c = Calendar.getInstance();
        String sql = "insert into sdlm_biz_方案步骤定义 (step_id,plan_id,model_code,创建日期,运行状态_id)values(?,?,?,?,?)";
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1, step_id);
            ps.setString(2, plan_id);
            ps.setString(3, modelId);
            ps.setDate(4, new Date(c.getTimeInMillis()));
            ps.setInt(5, 0);
            ps.execute();
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (ps != null)
                ps.close();
            if (connection != null)
                connection.close();
        }
    }

    public void addHeWangDiXing(String step_id, String jsonStr) throws SQLException {
        Connection connection = null;
        CallableStatement call = null;
        String sql = "{call INPUT_MODELINPUTVALUE(?,?,?)}";
        try {
            connection = basicDataSource.getConnection();
            Map<String, String> map = JSONObject.parseObject(jsonStr, Map.class);
            call = connection.prepareCall(sql);
            Iterator<String> it = map.keySet().iterator();
            while (it.hasNext()) {
                String name = it.next();
                call.setString(1, step_id);
                call.setString(2, name);
                call.setObject(3, map.get(name));
                call.execute();
            }
        } catch (Exception e) {
            connection.rollback();
            e.printStackTrace();
            throw e;
        } finally {
            if (call != null)
                call.close();
            if (connection != null)
                connection.close();
        }
    }

    public void addSdlBianjieFile(String plan_id, String bianjie_name, String bianjie_type, List<Map<String, String>> list) throws Exception {
        Connection connection = null;
        PreparedStatement ps = null;
        String sql = "insert into sdlm_biz_运算参数表 (sn_id,时间,参数项目_id,参数_value,方案_id)values(sys_guid(),?,?,?,?)";
        String sql1 = "select a.参数项目_id from sdlm_sys_运算参数定义表 a left join sdlm_sys_参数文件 b on a.file_id = b.file_id where b.file_name = ? and a.参数项目名称 = ?";
        String sql2 = "delete from sdlm_biz_运算参数表 where 参数项目_id = ? and 方案_id = ?";
        ResultSet rs = null;
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql1);
            ps.setString(1, bianjie_name);
            ps.setString(2, bianjie_type);
            rs = ps.executeQuery();
            String canshu = "";
            SimpleDateFormat format = new SimpleDateFormat("yyyy-MM-dd");
            if (rs.next()) {
                canshu = rs.getString(1);
            }
            if (canshu == null || "".equals(canshu)) {
                throw new Exception();
            } else {
                ps = connection.prepareStatement(sql2);
                ps.setString(1, canshu);
                ps.setString(2, plan_id);
                ps.execute();
                connection.setAutoCommit(false);
                ps = connection.prepareStatement(sql);
                Map<String, String> map = null;
                for (int i = 0; i < list.size(); i++) {
                    map = list.get(i);
                    ps.setDate(1, (new Date(format.parse(map.get("date")).getTime())));
                    ps.setString(2, canshu);
                    ps.setString(3, map.get("value"));
                    ps.setString(4, plan_id);
                    ps.addBatch();
                }
                ps.executeBatch();
                connection.commit();
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (rs != null)
                rs.close();
            if (ps != null)
                ps.close();
            if (connection != null)
                connection.close();
        }
    }

    public void addChangguiFileData(String step_id, List<String[]> list) throws Exception {
        Connection connection = null;
        CallableStatement call = null;
        PreparedStatement ps = null;
        String sql = "{call INPUT_SZPJINPARAMS(?,?,?,?,?)}";
        String sql1 = "delete from sdlm_biz_方案条件设定表 where step_id = ? and key in " +
                "('2DA57C31C7F24B2B82CA48157B0E74FE'," +
                "'0EC73495FE754C1CA6602C232777117B'," +
                "'A0BD2B717F30491B8CB0F2B85403883E'," +
                "'F778C416518946E3BD54A2C8ECE60E3E'," +
                "'F525FFD1E13F49A0A60E5224F057CE02'," +
                "'BD61ED754158413181479DB1F09B8154'," +
                "'ED6406CB0BF14CA39F8588161CC12272'," +
                "'28379483C6EF44A5ABF3891CDD9475B6'," +
                "'5276F2B9EC7246C383804E859B2DA3FA'," +
                "'EE36B74B5FD84DE5BFA2C9B8CD2FFFBD'," +
                "'0318BAD9EEDA43B1B52C9CD2B97CA124'," +
                "'DCE1A5FB640F4E469A36AC61FDA1A3B0'," +
                "'80B175536E6B4AC4B1E84343FA5A2753'," +
                "'BEFC74011EC94B1B8A9621E5CA588D66')";
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql1);
            ps.setString(1, step_id);
            ps.execute();
            call = connection.prepareCall(sql);
            String[] keys1 = new String[]{"sz_wrw1_c", "sz_wrw2_c", "sz_wrw3_c", "sz_wrw4_c", "sz_wrw5_c", "sz_wrw6_c", "sz_wrw7_c"};
            String[] keys2 = new String[]{"sz_wrw1_p", "sz_wrw2_p", "sz_wrw3_p", "sz_wrw4_p", "sz_wrw5_p", "sz_wrw6_p", "sz_wrw7_p"};
            for (int i = 0; i < list.size(); i++) {
                String[] strs = list.get(i);
                int i1 = (strs.length - 4) / 2;
                for (int j = 2; j < strs.length; j++) {
                    if (j == 2) { //距离
                        call.setString(4, "sz_wrw_juli");
                    } else if ((j - 3) == i1) { // 流量
                        call.setString(4, "sz_wrw_liuliang");
                    } else if ((j - 3) < i1) { //初始浓度
                        call.setString(4, keys1[j - 3]);
                    } else { // 排放浓度
                        call.setString(4, keys2[j - 4 - i1]);
                    }
                    call.setString(1, step_id);
                    call.setString(2, strs[0]);
                    call.setString(3, strs[1]);
                    call.setString(5, strs[j]);
                    call.addBatch();
                }
            }
            call.executeBatch();
        } catch (Exception e) {
            connection.rollback();
            e.printStackTrace();
            throw e;
        } finally {
            if (call != null)
                call.close();
            if (ps != null)
                ps.close();
            if (connection != null)
                connection.close();
        }
    }

    public void addChangguiBaseData(String step_id, Map<String, String> map) throws Exception {
        Connection connection = null;
        CallableStatement call = null;
        String sql = "{call INPUT_SZPJINBASEPARAMS(?,?,?)}";
        try {
            connection = basicDataSource.getConnection();
            call = connection.prepareCall(sql);
            Iterator<String> it = map.keySet().iterator();
            while (it.hasNext()) {
                String key = it.next();
                call.setString(1, step_id);
                call.setString(2, key);
                call.setString(3, map.get(key) + "");
                call.addBatch();
            }
            call.executeBatch();
        } catch (Exception e) {
            connection.rollback();
            e.printStackTrace();
            throw e;
        } finally {
            if (call != null)
                call.close();
            if (connection != null)
                connection.close();
        }
    }

    public void addChangguiGridData(String step_id, List<Map<String, String>> list) throws Exception {
        Connection connection = null;
        CallableStatement call = null;
        PreparedStatement ps = null;
        String sql = "{call INPUT_SZPJINPARAMS(?,?,?,?,?)}";
        String sql1 = "delete from sdlm_biz_方案条件设定表 where step_id = ? and key in('2DA57C31C7F24B2B82CA48157B0E74FE',\n" +
                "                                                                        '0EC73495FE754C1CA6602C232777117B',\n" +
                "                                                                        'A0BD2B717F30491B8CB0F2B85403883E',\n" +
                "                                                                        'F778C416518946E3BD54A2C8ECE60E3E',\n" +
                "                                                                        'F525FFD1E13F49A0A60E5224F057CE02',\n" +
                "                                                                        'BD61ED754158413181479DB1F09B8154',\n" +
                "                                                                        'ED6406CB0BF14CA39F8588161CC12272',\n" +
                "                                                                        '28379483C6EF44A5ABF3891CDD9475B6',\n" +
                "                                                                        '5276F2B9EC7246C383804E859B2DA3FA',\n" +
                "                                                                        'EE36B74B5FD84DE5BFA2C9B8CD2FFFBD',\n" +
                "                                                                        '0318BAD9EEDA43B1B52C9CD2B97CA124',\n" +
                "                                                                        'DCE1A5FB640F4E469A36AC61FDA1A3B0',\n" +
                "                                                                        '80B175536E6B4AC4B1E84343FA5A2753',\n" +
                "                                                                        'BEFC74011EC94B1B8A9621E5CA588D66')";
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql1);
            ps.setString(1, step_id);
            ps.execute();
            call = connection.prepareCall(sql);
            Map<String, String> map = null;
            Iterator<String> it = null;
            String heduan = null;
            String duanmian = null;
            String key = null;
            for (int i = 0; i < list.size(); i++) {
                map = list.get(i);
                heduan = map.remove("sz_heduan");
                duanmian = map.remove("sz_duanmian");
                it = map.keySet().iterator();
                while (it.hasNext()) {
                    key = it.next();
                    call.setString(1, step_id);
                    call.setString(2, heduan);
                    call.setString(3, duanmian);
                    call.setString(4, key);
                    call.setString(5, map.get(key));
                    call.addBatch();
                }
            }
            call.executeBatch();
        } catch (Exception e) {
            connection.rollback();
            e.printStackTrace();
            throw e;
        } finally {
            if (ps != null)
                ps.close();
            if (connection != null)
                connection.close();
        }
    }

    public List<Map<String, String>> getSzpjGridData(String step_id) throws Exception {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String, String>> list = new ArrayList<>();
        String sql = "select a.水质归属河段,a.水质计算断面号,to_char(a.距离间隔,'fm9999999990.00'),c.value,d.value,e.value,f.value,g.value,h.value,i.value,j.value,k.value,l.value,m.value,n.value,o.value,p.value,q.value from sdlm_sys_水质断面对应 a\n" +
                "left join (select value,水质断面对应_id from sdlm_biz_方案条件设定表 where step_id = ? and key = 'E6D227DE0C4345919BBCACCBBE4E3756') c on a.sn_id = c.水质断面对应_id\n" +
                "left join (select value,水质断面对应_id from sdlm_biz_方案条件设定表 where step_id = ? and key = '2DA57C31C7F24B2B82CA48157B0E74FE') d on a.sn_id = d.水质断面对应_id\n" +
                "left join (select value,水质断面对应_id from sdlm_biz_方案条件设定表 where step_id = ? and key = '0EC73495FE754C1CA6602C232777117B') e on a.sn_id = e.水质断面对应_id\n" +
                "left join (select value,水质断面对应_id from sdlm_biz_方案条件设定表 where step_id = ? and key = 'A0BD2B717F30491B8CB0F2B85403883E') f on a.sn_id = f.水质断面对应_id\n" +
                "left join (select value,水质断面对应_id from sdlm_biz_方案条件设定表 where step_id = ? and key = 'F778C416518946E3BD54A2C8ECE60E3E') g on a.sn_id = g.水质断面对应_id\n" +
                "left join (select value,水质断面对应_id from sdlm_biz_方案条件设定表 where step_id = ? and key = 'F525FFD1E13F49A0A60E5224F057CE02') h on a.sn_id = h.水质断面对应_id\n" +
                "left join (select value,水质断面对应_id from sdlm_biz_方案条件设定表 where step_id = ? and key = 'BD61ED754158413181479DB1F09B8154') i on a.sn_id = i.水质断面对应_id\n" +
                "left join (select value,水质断面对应_id from sdlm_biz_方案条件设定表 where step_id = ? and key = 'ED6406CB0BF14CA39F8588161CC12272') j on a.sn_id = j.水质断面对应_id\n" +
                "left join (select value,水质断面对应_id from sdlm_biz_方案条件设定表 where step_id = ? and key = '28379483C6EF44A5ABF3891CDD9475B6') k on a.sn_id = k.水质断面对应_id\n" +
                "left join (select value,水质断面对应_id from sdlm_biz_方案条件设定表 where step_id = ? and key = '5276F2B9EC7246C383804E859B2DA3FA') l on a.sn_id = l.水质断面对应_id\n" +
                "left join (select value,水质断面对应_id from sdlm_biz_方案条件设定表 where step_id = ? and key = 'EE36B74B5FD84DE5BFA2C9B8CD2FFFBD') m on a.sn_id = m.水质断面对应_id\n" +
                "left join (select value,水质断面对应_id from sdlm_biz_方案条件设定表 where step_id = ? and key = '0318BAD9EEDA43B1B52C9CD2B97CA124') n on a.sn_id = n.水质断面对应_id\n" +
                "left join (select value,水质断面对应_id from sdlm_biz_方案条件设定表 where step_id = ? and key = 'DCE1A5FB640F4E469A36AC61FDA1A3B0') o on a.sn_id = o.水质断面对应_id\n" +
                "left join (select value,水质断面对应_id from sdlm_biz_方案条件设定表 where step_id = ? and key = '80B175536E6B4AC4B1E84343FA5A2753') p on a.sn_id = p.水质断面对应_id\n" +
                "left join (select value,水质断面对应_id from sdlm_biz_方案条件设定表 where step_id = ? and key = 'BEFC74011EC94B1B8A9621E5CA588D66') q on a.sn_id = q.水质断面对应_id\n" +
                "order by  a.水质归属河段 asc,a.水质计算断面号 asc";
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1, step_id);
            ps.setString(2, step_id);
            ps.setString(3, step_id);
            ps.setString(4, step_id);
            ps.setString(5, step_id);
            ps.setString(6, step_id);
            ps.setString(7, step_id);
            ps.setString(8, step_id);
            ps.setString(9, step_id);
            ps.setString(10, step_id);
            ps.setString(11, step_id);
            ps.setString(12, step_id);
            ps.setString(13, step_id);
            ps.setString(14, step_id);
            ps.setString(15, step_id);
            rs = ps.executeQuery();
            Map<String, String> map = null;
            while (rs.next()) {
                map = new HashMap<>();
                map.put("sz_heduan", rs.getString(1));
                map.put("sz_duanmian", rs.getString(2));
                map.put("sz_wrw_juli", rs.getString(3));
                map.put("sz_wrw_liuliang", rs.getString(4));
                map.put("sz_wrw1_c", rs.getString(5));
                map.put("sz_wrw1_p", rs.getString(6));
                map.put("sz_wrw2_c", rs.getString(7));
                map.put("sz_wrw2_p", rs.getString(8));
                map.put("sz_wrw3_c", rs.getString(9));
                map.put("sz_wrw3_p", rs.getString(10));
                map.put("sz_wrw4_c", rs.getString(11));
                map.put("sz_wrw4_p", rs.getString(12));
                map.put("sz_wrw5_c", rs.getString(13));
                map.put("sz_wrw5_p", rs.getString(14));
                map.put("sz_wrw6_c", rs.getString(15));
                map.put("sz_wrw6_p", rs.getString(16));
                map.put("sz_wrw7_c", rs.getString(17));
                map.put("sz_wrw7_p", rs.getString(18));
                list.add(map);
            }
        } catch (Exception e) {
            connection.rollback();
            e.printStackTrace();
            throw e;
        } finally {
            if (ps != null)
                ps.close();
            if (connection != null)
                connection.close();
        }
        return list;
    }

    public String getStepByPlan(String plan_id, String modelId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String sql = "select step_id from sdlm_biz_方案步骤定义 where plan_id = ? and model_code = ?";
        String step_id = UUID.randomUUID().toString().replace("-", "");
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1, plan_id);
            ps.setString(2, modelId);
            rs = ps.executeQuery();
            if (rs.next()) {
                step_id = rs.getString(1);
            } else if (StringUtils.isNotBlank(plan_id) && StringUtils.isNotBlank(modelId)) {
                addPlanStep(plan_id, step_id, modelId);
            }
            return step_id;
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (rs != null)
                rs.close();
            if (ps != null)
                ps.close();
            if (connection != null)
                connection.close();
        }
    }

    public Map<String, String> getPlanDataById(String plan_id) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String sql = "select t.plan_id,t.方案名称,t.计算目标_id,t.描述,t.创建日期,t.创建人,t.计算年份,t.河段名称,v.WR_SD_NM from sdlm_biz_方案定义表 t left join ZDWARNER.V_水资源对象 v on t.计算目标_id = v.obj_id  where plan_id = ?";
        Map<String, String> map = new HashMap<>();
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1, plan_id);
            rs = ps.executeQuery();
            while (rs.next()) {
                map.put("plan_id", rs.getString(1));
                map.put("plan_name", rs.getString(2));
                map.put("run_obj_id", rs.getString(3));
                map.put("plan_description", rs.getString(4));
                map.put("plan_makeTime", rs.getString(5).substring(0, 10));
                map.put("plan_owner", rs.getString(6));
                map.put("run_year", rs.getString(7));
                map.put("run_segment", rs.getString(8));
                map.put("run_obj_name", rs.getString(9));
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (rs != null)
                rs.close();
            if (ps != null)
                ps.close();
            if (connection != null)
                connection.close();
        }
        return map;
    }

    public Map<String, Object> getHwdxInfoByPlanId(String plan_id) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String sql = "select a.value from SDLM_BIZ_方案条件设定表 a left join Sdlm_Sys_条件项目定义表 b on a.key = b.key_id  left join sdlm_biz_方案步骤定义 c on a.step_id=c.step_id  where b.key_abbr = ? and c.plan_id= ?";
        Map<String, Object> map = new HashMap<>();
        Map<String, String> map1 = null;
        String[] duanmian = new String[]{"IDAN", "IGORY", "IE", "ISHI", "NJ"};
        String[] hedao = new String[]{"ICLDNL", "IFE", "IFFP", "IFHN", "ITEIKO", "BAABAASA"};
        String[] zhiliu = new String[]{"ZHILIU_NAME_LIST", "ZHILIU_DM_LIST", "ZHILIU_COF", "NR"};
        String[] zhamen = new String[]{"ZHAMEN_LIST"};
        String[] shuiku = new String[]{"SHUIKU_LIST"};
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            map1 = new HashMap<>();
            for (int i = 0; i < duanmian.length; i++) {
                ps.setString(1, duanmian[i]);
                ps.setString(2, plan_id);
                rs = ps.executeQuery();
                if (rs.next()) {
                    map1.put(duanmian[i], rs.getString(1));
                }
            }
            map.put("duanmian", map1);

            map1 = new HashMap<>();
            for (int i = 0; i < hedao.length; i++) {
                ps.setString(1, hedao[i]);
                ps.setString(2, plan_id);
                rs = ps.executeQuery();
                if (rs.next()) {
                    map1.put(hedao[i], rs.getString(1));
                }
            }
            map.put("hedao", map1);

            map1 = new HashMap<>();
            for (int i = 0; i < zhiliu.length; i++) {
                ps.setString(1, zhiliu[i]);
                ps.setString(2, plan_id);
                rs = ps.executeQuery();
                if (rs.next()) {
                    map1.put(zhiliu[i], rs.getString(1));
                }
            }
            map.put("zhiliu", map1);

            map1 = new HashMap<>();
            for (int i = 0; i < zhamen.length; i++) {
                ps.setString(1, zhamen[i]);
                ps.setString(2, plan_id);
                rs = ps.executeQuery();
                if (rs.next()) {
                    map1.put(zhamen[i], rs.getString(1));
                }
            }
            map.put("zhamen", map1);

            map1 = new HashMap<>();
            for (int i = 0; i < shuiku.length; i++) {
                ps.setString(1, shuiku[i]);
                ps.setString(2, plan_id);
                rs = ps.executeQuery();
                if (rs.next()) {
                    map1.put(shuiku[i], rs.getString(1));
                }
            }
            map.put("shuiku", map1);
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (rs != null)
                rs.close();
            if (ps != null)
                ps.close();
            if (connection != null)
                connection.close();
        }
        return map;
    }

    public Map<String, Object> getSdlParamsData(String plan_id) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String sql = "select a.时间, a.参数_value, b.参数项目名称, c.file_name\n" +
                "  from sdlm_biz_运算参数表 a\n" +
                "  left join sdlm_sys_运算参数定义表 b\n" +
                "    on a.参数项目_id = b.参数项目_id\n" +
                "  left join sdlm_sys_参数文件 c\n" +
                "    on b.file_id = c.file_id\n" +
                " where a.方案_id = ? and c.file_name = ?\n" +
                " order by a.时间 asc\n";

        String TaiZiHQSql = "select a.时间, a.参数_value, b.参数项目名称, c.file_name\n" +
                "  from sdlm_biz_运算参数表 a\n" +
                "  left join sdlm_sys_运算参数定义表 b\n" +
                "    on a.参数项目_id = b.参数项目_id\n" +
                "  left join sdlm_sys_参数文件 c\n" +
                "    on b.file_id = c.file_id\n" +
                " where a.方案_id = ? and c.file_name = 'TaiZiHQ.dat' and  参数项目名称 = ?\n" +
                " order by a.时间 asc\n";

        Map<String, Object> map = new HashMap<>();
        Map<String, String> map1 = null;
        String selectZhiLiu = "select a.value from SDLM_BIZ_方案条件设定表 a left join sdlm_biz_方案步骤定义 b on a.step_id=b.step_id  where key = '31BB2E0C29864A908CACC114AC2ACBDB' and b.plan_id = ?";
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(selectZhiLiu);
            ps.setString(1, plan_id);
            rs = ps.executeQuery();
            String value;
            if (rs.next() && (value = rs.getString(1)) != null) {
                ps = connection.prepareStatement(sql);
                String[] names = value.split(",");
                for (int i = 0; i < names.length; i++) {
                    ps.setString(1, plan_id);
                    ps.setString(2, names[i]);
                    rs = ps.executeQuery();
                    List<Map<String, String>> list = new ArrayList<>();
                    while (rs.next()) {
                        map1 = new HashMap<>();
                        map1.put("date", rs.getString(1).substring(0, 10));
                        map1.put("value", rs.getString(2));
                        list.add(map1);
                    }
                    map.put(names[i], list);
                }
                ps = connection.prepareStatement(TaiZiHQSql);
                ps.setString(1, plan_id);
                ps.setString(2, "水位");
                rs = ps.executeQuery();
                List<Map<String, String>> list = new ArrayList<>();
                while (rs.next()) {
                    map1 = new HashMap<>();
                    map1.put("date", rs.getString(1).substring(0, 10));
                    map1.put("value", rs.getString(2));
                    list.add(map1);
                }
                map.put("TaiZiHQ.dat_shuiwei", list);

                list = new ArrayList<>();
                ps = connection.prepareStatement(TaiZiHQSql);
                ps.setString(1, plan_id);
                ps.setString(2, "流量");
                rs = ps.executeQuery();
                while (rs.next()) {
                    map1 = new HashMap<>();
                    map1.put("date", rs.getString(1).substring(0, 10));
                    map1.put("value", rs.getString(2));
                    list.add(map1);
                }
                map.put("TaiZiHQ.dat", list);
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (rs != null)
                rs.close();
            if (ps != null)
                ps.close();
            if (connection != null)
                connection.close();
        }
        return map;
    }

    public Map<String, Object> getSzpjCgBaseData(String plan_id) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String sql = "select a.value from SDLM_BIZ_方案条件设定表 a left join Sdlm_Sys_条件项目定义表 b on a.key = b.key_id  left join sdlm_biz_方案步骤定义 c on a.step_id=c.step_id  where b.key_abbr = ? and c.plan_id= ?";
        Map<String, Object> map = new HashMap<>();
        Map<String, String> map1 = null;
        String[] jiben = new String[]{"sz_nr", "sz_nj", "sz_njb", "sz_nl", "sz_dt", "sz_mt", "sz_GDO", "sz_Bdo", "sz_month"};
        String dayinduanmian = "sz_nspr";
        String wuranwuWeizhi = "sz_wry";
        String wuranwuNames = "sz_wuranwuNames";
        String wuranwuIndex = "sz_wuranwuIndex";
        String jiangjie = "sz_JHxS";
        String chushi = "sz_cbou";
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            map1 = new HashMap<>();
            for (int i = 0; i < jiben.length; i++) {
                ps.setString(1, jiben[i]);
                ps.setString(2, plan_id);
                rs = ps.executeQuery();
                if (rs.next()) {
                    map1.put(jiben[i], rs.getString(1));
                }
            }
            map.put("jibenxinxi", map1);

            ps.setString(1, dayinduanmian);
            ps.setString(2, plan_id);
            rs = ps.executeQuery();
            if (rs.next()) {
                map.put(dayinduanmian, rs.getString(1));
            }

            ps.setString(1, wuranwuWeizhi);
            ps.setString(2, plan_id);
            rs = ps.executeQuery();
            if (rs.next()) {
                map.put(wuranwuWeizhi, rs.getString(1));
            }

            ps.setString(1, wuranwuNames);
            ps.setString(2, plan_id);
            rs = ps.executeQuery();
            if (rs.next()) {
                map.put(wuranwuNames, rs.getString(1));
            }

            ps.setString(1, wuranwuIndex);
            ps.setString(2, plan_id);
            rs = ps.executeQuery();
            if (rs.next()) {
                map.put(wuranwuIndex, rs.getString(1));
            }

            ps.setString(1, jiangjie);
            ps.setString(2, plan_id);
            rs = ps.executeQuery();
            if (rs.next()) {
                map.put(jiangjie, rs.getString(1));
            }

            ps.setString(1, chushi);
            ps.setString(2, plan_id);
            rs = ps.executeQuery();
            if (rs.next()) {
                map.put(chushi, rs.getString(1));
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (rs != null)
                rs.close();
            if (ps != null)
                ps.close();
            if (connection != null)
                connection.close();
        }
        return map;
    }

    public Map<String, Object> getSzpjSsBaseData(String plan_id) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String sql = "select a.value from SDLM_BIZ_方案条件设定表 a left join Sdlm_Sys_条件项目定义表 b on a.key = b.key_id  left join sdlm_biz_方案步骤定义 c on a.step_id=c.step_id  where b.key_abbr = ? and c.plan_id= ?";
        Map<String, Object> map = new HashMap<>();
        Map<String, String> map1 = null;
        String[] jiben = new String[]{"sz_nr", "sz_nj", "sz_njb", "sz_nl", "sz_dt", "sz_mt", "sz_GDO", "sz_Bdo", "sz_JHxT", "sz_shijiantype", "sz_start", "sz_end", "sz_zongbushu", "sz_shijianjiange"};
        String dayinduanmian = "sz_nspr";
        String wuranwuWeizhi = "sz_wry";
        String wuranwuNames = "sz_wuranwuNames";
        String wuranwuIndex = "sz_wuranwuIndex";
        String jiangjie = "sz_JHxS";
        String chushi = "sz_cbou";
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            map1 = new HashMap<>();
            for (int i = 0; i < jiben.length; i++) {
                ps.setString(1, jiben[i]);
                ps.setString(2, plan_id);
                rs = ps.executeQuery();
                if (rs.next()) {
                    map1.put(jiben[i], rs.getString(1));
                }
            }
            map.put("jibenxinxi", map1);

            ps.setString(1, dayinduanmian);
            ps.setString(2, plan_id);
            rs = ps.executeQuery();
            if (rs.next()) {
                map.put(dayinduanmian, rs.getString(1));
            }

            ps.setString(1, wuranwuWeizhi);
            ps.setString(2, plan_id);
            rs = ps.executeQuery();
            if (rs.next()) {
                map.put(wuranwuWeizhi, rs.getString(1));
            }

            ps.setString(1, wuranwuNames);
            ps.setString(2, plan_id);
            rs = ps.executeQuery();
            if (rs.next()) {
                map.put(wuranwuNames, rs.getString(1));
            }

            ps.setString(1, wuranwuIndex);
            ps.setString(2, plan_id);
            rs = ps.executeQuery();
            if (rs.next()) {
                map.put(wuranwuIndex, rs.getString(1));
            }

            ps.setString(1, jiangjie);
            ps.setString(2, plan_id);
            rs = ps.executeQuery();
            if (rs.next()) {
                map.put(jiangjie, rs.getString(1));
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if (rs != null)
                rs.close();
            if (ps != null)
                ps.close();
            if (connection != null)
                connection.close();
        }
        return map;
    }

    public void addQiXiangData(String planId, List<String[]> list) throws SQLException {
        String[] items = new String[]{"D19CCCB4732F46859FB6E2B0A58F3174", "8B15A500C6174F078EDCE9B4DE137B4A", "6926DAEBC89B4D71883CD22A0A651C77",
                "2A4201E041EF4EDEA66DEAAA671466E2", "C15F02D85FA34A44A9F1EFF58D5898B2", "73920C3DAF3B4317935CAA5FF1DD46A8", "7F11E44802404EC0AF4DF1C6AC6D7CBF"};
        String sql = "insert into yqmx_biz_运算参数表(sn_id,方案_no,序列_id,day,参数项目_id,参数_value)values(sys_guid(),?,1,?,?,?)";
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            if(list.size()>0){
                connection = basicDataSource.getConnection();
                ps = connection.prepareStatement(sql);
                for (int i = 0; i < list.size(); i++) {
                    for (int j = 0; j < 7; j++) {
                        ps.setString(1, planId);
                        ps.setString(2, list.get(i)[0]);
                        ps.setString(3, items[j]);
                        ps.setString(4, list.get(i)[j + 1]);
                        ps.addBatch();
                    }
                }
                ps.executeBatch();
            }
        } catch (SQLException e) {
            e.printStackTrace();
        } finally {
            if (ps != null) {
                ps.close();
            }
            if (connection != null) {
                connection.close();
            }
        }
    }

    public void addFengChangData(String planId, List<String[]> list) throws SQLException {
        String[] items = new String[]{"E5F089CDE5504F3789F701B840A403B7", "6A9F342796A940AC847A0BC843EB01F4"};
        String sql = "insert into yqmx_biz_运算参数表(sn_id,方案_no,序列_id,day,参数项目_id,参数_value)values(sys_guid(),?,1,?,?,?)";
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            if(list.size()>0){
                connection = basicDataSource.getConnection();
                ps = connection.prepareStatement(sql);
                for (int i = 0; i < list.size(); i++) {
                    for (int j = 0; j < 2; j++) {
                        ps.setString(1, planId);
                        ps.setString(2, list.get(i)[0]);
                        ps.setString(3, items[j]);
                        ps.setString(4, list.get(i)[j + 1]);
                        ps.addBatch();
                    }
                }
                ps.executeBatch();
            }
        } catch (SQLException e) {
            e.printStackTrace();
        } finally {
            if (ps != null) {
                ps.close();
            }
            if (connection != null) {
                connection.close();
            }
        }
    }

    public void addWenDuData(String planId, int index, List<String[]> list) throws SQLException {
        String[] items = new String[]{"B8E50FC2AB8C496D89D38B539A450C6D", "343446D90C3144ABA493952C9975B216"};
        String sql = "insert into yqmx_biz_运算参数表(sn_id,方案_no,序列_id,day,参数项目_id,参数_value)values(sys_guid(),?,?,?,?,?)";
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            if(list.size()>0){
                connection = basicDataSource.getConnection();
                ps = connection.prepareStatement(sql);
                for (int i = 0; i < list.size(); i++) {
                    ps.setString(1, planId);
                    ps.setInt(2, index);
                    ps.setString(3, list.get(i)[0]);
                    ps.setString(4, "343446D90C3144ABA493952C9975B216");
                    ps.setString(5, list.get(i)[1]);
                    ps.addBatch();
                }
                ps.executeBatch();
            }
        } catch (SQLException e) {
            e.printStackTrace();
        } finally {
            if (ps != null) {
                ps.close();
            }
            if (connection != null) {
                connection.close();
            }
        }
    }

    public void addWuRanWuData(String planId, int index, List<String[]> list) throws SQLException {
        String sql = "insert into yqmx_biz_运算参数表(sn_id,方案_no,序列_id,day,参数项目_id,参数_value)values(sys_guid(),?,?,?,?,?)";
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            if(list.size()>0) {
                connection = basicDataSource.getConnection();
                ps = connection.prepareStatement(sql);
                for (int i = 0; i < list.size(); i++) {
                    ps.setString(1, planId);
                    ps.setInt(2, index);
                    ps.setString(3, list.get(i)[0]);
                    ps.setString(4, "EA2D29994A254C1B85C5B0C8F53A8648");
                    ps.setString(5, list.get(i)[1]);
                    ps.addBatch();
                }
                ps.executeBatch();
            }
        } catch (SQLException e) {
            e.printStackTrace();
        } finally {
            if (ps != null) {
                ps.close();
            }
            if (connection != null) {
                connection.close();
            }
        }
    }

    public void addLiuLiangData(String planId, int index, List<String[]> list) throws SQLException {
        String sql = "insert into yqmx_biz_运算参数表(sn_id,方案_no,序列_id,day,参数项目_id,参数_value)values(sys_guid(),?,?,?,?,?)";
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            if(list.size()>0) {
                connection = basicDataSource.getConnection();
                ps = connection.prepareStatement(sql);
                for (int i = 0; i < list.size(); i++) {
                    ps.setString(1, planId);
                    ps.setInt(2, index);
                    ps.setString(3, list.get(i)[0]);
                    ps.setString(4, "24D8C75DC4024CA78213956FE33F01BC");
                    ps.setString(5, list.get(i)[1]);
                    ps.addBatch();
                }
                ps.executeBatch();
            }
        } catch (SQLException e) {
            e.printStackTrace();
        } finally {
            if (ps != null) {
                ps.close();
            }
            if (connection != null) {
                connection.close();
            }
        }
    }

    public void addXuLieMappingData(String planId, Map<String, String> map) throws Exception {
        String sql = "insert into YQMX_BIZ_主控序列对应表(id,plan_id,i_code,j_code,ll_xulie,wd_xulie,wrw_xulie)values(sys_guid(),?,?,?,?,?,?)";
        String checkSql = "select count(*) from YQMX_BIZ_主控序列对应表 where plan_id =? and ((wd_xulie = ? and wd_xulie !=0) or  (wrw_xulie=? and wrw_xulie != 0)) ";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(checkSql);
            ps.setString(1,planId);
            ps.setString(2,map.get("WD_XULIE"));
            ps.setString(3, map.get("WRW_XULIE"));
            rs = ps.executeQuery();
            if(rs.next()){
                if(rs.getInt(1)>0){
                    throw new Exception("流量序列对应的温度序列或者污染物序列重复，请检查！");
                }
            }
            ps = connection.prepareStatement(sql);
            ps.setString(1, planId);
            ps.setString(2, map.get("I_CODE"));
            ps.setString(3, map.get("J_CODE"));
            ps.setString(4, map.get("LL_XULIE"));
            ps.setString(5, map.get("WD_XULIE"));
            ps.setString(6, map.get("WRW_XULIE"));
            ps.execute();
        } catch (SQLException e) {
            e.printStackTrace();
        } finally {
            if (ps != null) {
                ps.close();
            }
            if (connection != null) {
                connection.close();
            }
        }
    }
    public void deleteXuLieMappingData(String planId,int index) throws SQLException {
        String deleteSql = "delete from YQMX_BIZ_主控序列对应表 where plan_id =? and ll_xulie = ?";
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(deleteSql);
            ps.setString(1,planId);
            ps.setInt(2,index);
            ps.execute();
        } catch (SQLException e) {
            e.printStackTrace();
        } finally {
            if (ps != null) {
                ps.close();
            }
            if (connection != null) {
                connection.close();
            }
        }
    }
    public void deleteYuQiaoInputData(String planId,String fileId,int index) throws SQLException {
        String sql = "delete from yqmx_biz_运算参数表 where 方案_no = ? and 序列_id =? and 参数项目_id in (select 参数项目_id from sdlm_sys_运算参数定义表 where file_id =?)";
        Connection connection = null;
        PreparedStatement ps = null;
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1, planId);
            ps.setInt(2, index);
            ps.setString(3,fileId);
            ps.execute();
        } catch (SQLException e) {
            e.printStackTrace();
        } finally {
            if (ps != null) {
                ps.close();
            }
            if (connection != null) {
                connection.close();
            }
        }
    }
}
