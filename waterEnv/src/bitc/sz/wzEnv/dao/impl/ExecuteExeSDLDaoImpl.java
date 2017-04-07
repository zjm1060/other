package bitc.sz.wzEnv.dao.impl;

import bitc.sz.wzEnv.dao.ExecuteExeSDLDao;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Repository;
import org.springframework.util.StringUtils;

import javax.sql.DataSource;
import java.sql.*;
import java.util.*;

/**
 * Created by zsz on 2016/5/18.
 */
@Repository
public class ExecuteExeSDLDaoImpl implements ExecuteExeSDLDao {
    @Autowired
    private DataSource basicDataSource;

    @Override
    public List<Map<Integer, String>> getCOEDADate(String plan_id) throws Exception {
        String[] block_ids = {  "416BAF19D1BD46DCAA9E0399735BAFD9",  //一般的
                                "232979D46CB94754AC0DEE57BE64C9B4",  //支流
                                "CF183739C49C4EEAA43F8C7415C40613",  //流域面积
                                "D402CA98D76645B098E1C4F81A01BDDB",  //闸门
                                "69B352ACCD5948E995497FE870C54B1E",  //水库
                                "C6BB084B65BD48348C6515318FA8B30B",  //流量
                                "764481F9997746C280CE5EFA6F70B44A"}; //其他
        String [][] keys ={{"IDAN","IGORY","ISHI","IE","NJ","IFR","IFC","IFFP","IFHN","IFE","ITEIKO","NRESV","ICHECK"},
                {"NR","ZHILIU_DM_LIST","ZHILIU_NAME_LIST"},
                {"BAABAASA","ZHILIU_COF"},
                {"ZHAMEN_COUNT","ZHAMEN_LIST"},
                {"SHUIKU_LIST"},
                {"NQT","IQT","NCDH","IDADHH","NCL","ICLDNL"},
                {"IHDOWN","IFZB","INQB","INAR","SSTO","NCMAX","DTBMIN","RAMDAB","RAMDAS","RAMDAW","ALF","CKAPPA","S"}};
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String sql = "select a.key_abbr,a.key_name, b.value\n" +
                "  from sdlm_sys_条件项目定义表 a\n" +
                "  left join sdlm_biz_方案条件设定表 b\n" +
                "    on a.key_id = b.key\n" +
                "  left join sdlm_sys_条件块定义表 c\n" +
                "    on a.block_id = c.block_id\n " +
                "   left join sdlm_biz_方案步骤定义 d\n" +
                "                     on b.step_id = d.step_id" +
                "    where a.block_id = ? and  d.plan_id= ? and d.model_code = '73476806f527419686d18gbca8485689'";
        Map<Integer ,String> map = null;
        List<Map<Integer, String>> result = null;
        try {
            connection = basicDataSource.getConnection();
            result = new ArrayList<Map<Integer, String>>();
            for(int i=0;i<block_ids.length;i++){
                ps = connection.prepareStatement(sql);
                ps.setString(1, block_ids[i]);
                ps.setString(2, plan_id);
                rs = ps.executeQuery();
                map = new HashMap<Integer,String>();
                String [] block_keys = keys[i];
                List <String> list = Arrays.asList(block_keys);
                String key_abbr = null;
                String key_name = null;
                String value  = null;
                String key = null;
                while (rs.next()){
                    key_abbr = rs.getString(1);
                    key_name = rs.getString(2);
                    value = rs.getString(3);
                    key = key_abbr;
                    if(key_name == null){
                        key_name = " ";
                    }
                    if(value == null){
                        value = " ";
                    }
                    map.put(list.indexOf(key),value);
                }
                result.add(map);
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
        return result;
    }

    @Override
    public String [][] getGateHYOData(String [] zhamen) throws Exception{
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String [][] result = null;
        int count = getGateHYOCount();
        String sql="select b.闸门_name as name, a.上游水位, a.下游流量\n" +
                "  from sdlm_sys_闸门参数表 a\n" +
                "  left join sdlm_sys_闸门定义表 b\n" +
                "    on a.闸门_id = b.闸门_id\n" +
                "    where 闸门断面号 = ? order by a.上游水位 asc\n";

        try {
            connection = basicDataSource.getConnection();
            result = new String[2][count*zhamen.length];
            for(int i=0;i<zhamen.length;i++){
                ps = connection.prepareStatement(sql);
                ps.setString(1,zhamen[i]);
                rs = ps.executeQuery();
                int num =0;
                while (rs.next()){
                    result[0][i*count+num] = rs.getString(2);
                    result[1][i*count+num] = rs.getString(3);
                    num++;
                }
            }
            return result;
        }catch (Exception e){
            e.printStackTrace();
            throw e;
        }finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
    }
    @Override
    public int getGateHYOCount() throws Exception{
        String sql = "select distinct count(*)  from sdlm_sys_闸门参数表 a\n" +
                "  left join sdlm_sys_闸门定义表 b\n" +
                "    on a.闸门_id = b.闸门_id group by a.闸门_id";

        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            rs = ps.executeQuery();
            if(rs.next()){
                return  rs.getInt(1);
            }
            return 0;
        }catch (Exception e){
            e.printStackTrace();
            throw e;
        }finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
    }
    @Override
    public Map<String, String> getZhamenDmHao() throws Exception {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        Map<String,String> map = null;
        String sql="select 闸门_name,闸门断面号 from sdlm_sys_闸门定义表";
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            rs = ps.executeQuery();
            map = new HashMap<String,String>();
            while (rs.next()){
                map.put(rs.getString(1), rs.getString(2));
            }
            return map;
        }catch (Exception e){
            e.printStackTrace();
            throw e;
        }finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
    }
    @Override
    public Map<String, String> getShuikuDmHao() throws Exception {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        Map<String,String> map = null;
        String sql="select 闸门_name,闸门断面号 from sdlm_sys_闸门定义表";
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            rs = ps.executeQuery();
            map = new HashMap<String,String>();
            while (rs.next()){
                map.put(rs.getString(1), rs.getString(2));
            }
            return map;
        }catch (Exception e){
            e.printStackTrace();
            throw e;
        }finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
    }
    @Override
    public List<Map<Integer, String>> getTaiZiHQDate(String planId, String fileId) throws Exception {
        String[] keys = {"水位", "流量"};
        String[] values = new String[2];
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<Integer, String>> list = new ArrayList<Map<Integer, String>>();
        String sql = "select t.时间, t.参数_value, d.参数_value\n" +
                "  from sdlm_biz_运算参数表 t\n" +
                "  left join sdlm_biz_运算参数表 d\n" +
                "    on t.时间 = d.时间 and t.方案_id = d.方案_id  \n" +
                "   and d.参数项目_id = ? " +

                " where t.参数项目_id =  ?  and  t.方案_id = ?\n" +
                " order by t.时间 asc\n";
        String sql1 = "select 参数项目名称,参数项目_id from sdlm_sys_运算参数定义表 where file_id = ?";
        try {
            Map<Integer, String> map = null;
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql1);
            ps.setString(1, fileId);
            rs = ps.executeQuery();
            while (rs.next()) {
                String value = rs.getString(1).trim();
                if (keys[0].equals(value)) {
                    values[0] = rs.getString(2);
                } else if (keys[1].equals(value)) {
                    values[1] = rs.getString(2);
                }
            }
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1, values[1]);
            ps.setString(2, values[0]);
            ps.setString(3, planId);
            rs = ps.executeQuery();
            while (rs.next()) {
                map = new HashMap<>();
                map.put(0, rs.getDate(1).toString());
                map.put(1, rs.getString(2));
                map.put(2, rs.getString(3));
                list.add(map);
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
        return list;
    }

    @Override
    public List<Map<String, String>> waterDMData(String planId, String dmId, String obj_id,String modelId, String start, String end) throws SQLException {

        String selectYear = "select 计算年份 from sdlm_biz_方案定义表 where plan_id = ?";
        String selectDmh = "select 计算断面号 from sdlm_sys_断面信息表 where obj_id = ?";
//        String selectDmh = "select 计算断面号 from sdlm_sys_断面分层水力参数 t where sn_id = ?";
        String selectStep = "select step_id from sdlm_biz_方案步骤定义 where plan_id = ? and model_code = ?";

        String sql = "select result_date,计算结果_value from sdlm_output_计算结果表 " +
                " where 计算项目_id = ? and step_id = ? and 计算断面号= ?  and result_date >= to_date(?,'yyyy-mm-dd') and result_date<= to_date(?,'yyyy-mm-dd') " +
                "  order by result_date asc,计算断面号 asc";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String step_id = null;
        int dmh = 0;
        List<Map<String, String>> dataList = new ArrayList<Map<String, String>>();
        try {
            Map dataMap = null;
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(selectStep);
            ps.setString(1, planId);
            ps.setString(2, modelId);
            rs = ps.executeQuery();
            if (rs.next()) {
                step_id = rs.getString(1);
            }
            ps = connection.prepareStatement(selectDmh);
            ps.setString(1, dmId);
            rs = ps.executeQuery();
            if (rs.next()) {
                dmh = rs.getInt(1);
            }
            if (StringUtils.isEmpty(start) || StringUtils.isEmpty(end)) {
                ps = connection.prepareStatement(selectYear);
                ps.setString(1, planId);
                rs = ps.executeQuery();
                if (rs.next()) {
                    int year = 0;
                    year = rs.getInt(1);
                    ps = connection.prepareStatement(sql);
                    ps.setString(1, obj_id);
                    ps.setString(2, step_id);
                    ps.setInt(3, dmh);
                    ps.setString(4, year + "-01-01");
                    ps.setString(5, year + "-12-31");
                    rs = ps.executeQuery();
                    while (rs.next()) {
                        if(!StringUtils.isEmpty(rs.getString(1)) && !StringUtils.isEmpty(rs.getString(2))){
                            dataMap = new HashMap();
                            dataMap.put("time", rs.getString(1).split(" ")[0]);
                            dataMap.put("value", getNmuber(rs.getString(2),true,false));
                            dataList.add(dataMap);
                        }
                    }
                }

            } else {
                ps = connection.prepareStatement(sql);
                ps.setString(1, obj_id);
                ps.setString(2, step_id);
                ps.setInt(3, dmh);
                ps.setString(4, start);
                ps.setString(5, end);
                rs = ps.executeQuery();
                while (rs.next()) {
                    if(!StringUtils.isEmpty(rs.getString(1)) && !StringUtils.isEmpty(rs.getString(2))) {
                        dataMap = new HashMap();
                        dataMap.put("time", rs.getString(1).split(" ")[0]);
                        dataMap.put("value", rs.getString(2));
                        dataList.add(dataMap);
                    }
                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
        return dataList;
    }

    @Override
    public List<Map<String, String>> waterDMJCData(String planId, String dmId, String obj_name, String start, String end) throws SQLException {

        String selectYear = "select 计算年份 from sdlm_biz_方案定义表 where plan_id = ?";
        String sql ="select to_char(观测时间,'yyyy-MM-dd'),round(avg("+obj_name+"),2) from szpj_biz_水文观测数据 a where 观测点_id = ? and 观测时间 >= to_date(?,'yyyy-mm-dd') and 观测时间<= to_date(?,'yyyy-mm-dd')  group by to_char(观测时间,'yyyy-MM-dd')";
        String sql1 = "select 水资源断面_id  from sdlm_sys_断面信息表 where obj_id = ?";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String szyDM_id = null;
        int dmh = 0;
        List<Map<String, String>> dataList = new ArrayList<Map<String, String>>();
        try {
            Map dataMap = null;
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql1);
            ps.setString(1, dmId);
            rs = ps.executeQuery();
            if (rs.next()) {
                szyDM_id = rs.getString(1);
            }
            if (StringUtils.isEmpty(start) || StringUtils.isEmpty(end)) {
                ps = connection.prepareStatement(selectYear);
                ps.setString(1, planId);
                rs = ps.executeQuery();
                if (rs.next()) {
                    int year = 0;
                    year = rs.getInt(1);
                    ps = connection.prepareStatement(sql);
                    ps.setString(1, szyDM_id);
                    ps.setString(2, year + "-01-01");
                    ps.setString(3, year + "-12-31");
                    rs = ps.executeQuery();
                    while (rs.next()) {
                        if(!StringUtils.isEmpty(rs.getString(1)) && !StringUtils.isEmpty(rs.getString(2))) {
                            dataMap = new HashMap();
                            dataMap.put("time", rs.getString(1));
                            dataMap.put("value", rs.getString(2));
                            dataList.add(dataMap);
                        }
                    }
                }

            } else {
                ps = connection.prepareStatement(sql);
                ps.setString(1, szyDM_id);
                ps.setString(2, start);
                ps.setString(3, end);
                rs = ps.executeQuery();
                while (rs.next()) {
                    if(!StringUtils.isEmpty(rs.getString(1)) && !StringUtils.isEmpty(rs.getString(2))){
                        dataMap = new HashMap();
                        dataMap.put("time", rs.getString(1));
                        dataMap.put("value", rs.getString(2));
                        dataList.add(dataMap);
                    }
                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
        return dataList;
    }

    @Override
    public List<Map<String, String>> waterDMJCData(String planId,String obj_name, String start, String end) throws SQLException {

        String selectYear = "select 计算年份 from sdlm_biz_方案定义表 where plan_id = ?";
        String sql ="select to_char(a.时间,'yyyy-MM-dd'), a.参数_value\n" +
                "  from sdlm_biz_运算参数表 a\n" +
                "  left join sdlm_sys_运算参数定义表 b\n" +
                "    on a.参数项目_id = b.参数项目_id\n" +
                "  left join sdlm_sys_参数文件 c\n" +
                "    on b.file_id = c.file_id\n" +
                "where c.file_name='TaiZiHQ.dat' and b.参数项目名称= ?  and a.时间 >= to_date(?,'yyyy-mm-dd') and a.时间<= to_date(?,'yyyy-mm-dd')   order by 时间";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        List<Map<String, String>> dataList = new ArrayList<Map<String, String>>();
        try {
            Map dataMap = null;
            connection = basicDataSource.getConnection();
            if (StringUtils.isEmpty(start) || StringUtils.isEmpty(end)) {
                ps = connection.prepareStatement(selectYear);
                ps.setString(1, planId);
                rs = ps.executeQuery();
                if (rs.next()) {
                    int year = 0;
                    year = rs.getInt(1);
                    ps = connection.prepareStatement(sql);
                    ps.setString(1, obj_name);
                    ps.setString(2, year + "-01-01");
                    ps.setString(3, year + "-12-31");
                    rs = ps.executeQuery();
                    while (rs.next()) {
                        if(!StringUtils.isEmpty(rs.getString(1)) && !StringUtils.isEmpty(rs.getString(2))){
                            dataMap = new HashMap();
                            dataMap.put("time", rs.getString(1));
                            dataMap.put("value", rs.getString(2));
                            dataList.add(dataMap);
                        }
                    }
                }

            } else {
                ps = connection.prepareStatement(sql);
                ps.setString(1, obj_name);
                ps.setString(2, start);
                ps.setString(3, end);
                rs = ps.executeQuery();
                while (rs.next()) {
                    if(!StringUtils.isEmpty(rs.getString(1)) && !StringUtils.isEmpty(rs.getString(2))){
                        dataMap = new HashMap();
                        dataMap.put("time", rs.getString(1));
                        dataMap.put("value", rs.getString(2));
                        dataList.add(dataMap);
                    }
                }
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(rs != null)
            rs.close();
            if(ps != null)
            ps.close();
            if(connection!=null)
            connection.close();
        }
        return dataList;
    }
    private String getNmuber(String num, boolean head, boolean tail) {
        if(num != null && !"".equals(num)){
            int index = num.indexOf(".");
            if (head && index == 0) {
                num = "0" + num;
            }
            if (tail && index == -1) {
                num = num + ".000";
            }
            if (index >= 0 && (num.length() > index + 4)) {
                num = num.substring(0, index + 4);
            }
        }
        return num;
    }

    @Override
    public int getPlanYear(String planId) throws Exception {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String selectYear = "select 计算年份 from sdlm_biz_方案定义表 where plan_id = ?";
        int year = 0;
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(selectYear);
            ps.setString(1, planId);
            rs = ps.executeQuery();
            if (rs.next()) {
                year = rs.getInt(1);
            }
            return year;
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
    }

    @Override
    public List<Map<String, String>> waterHDData(String planId,String modelId, String juli, String jisuanId, String start, String end) throws SQLException {
        String selectStep = "select step_id from sdlm_biz_方案步骤定义 where plan_id = ? and model_code = ?";
        String selectYear = "select 计算年份 from sdlm_biz_方案定义表 where plan_id = ?";
        String sql = "select chardate, 距离, round(avg(计算值),2)\n" +
                "  from (select a.计算断面号,\n" +
                "               a.result_date,\n" +
                "               a.chardate,\n" +
                "               a.计算结果_value as 距离,\n" +
                "               b.计算结果_value as 计算值\n" +
                "          from (select 计算项目_id,\n" +
                "                       计算断面号,\n" +
                "                       result_date,\n" +
                "                       to_char(result_date, 'yyyy-mm') as chardate,\n" +
                "                       计算结果_value\n" +
                "                  from sdlm_output_计算结果表\n" +
                "                 where 计算项目_id = ? \n" +
                "                   and step_id = ? \n" +
                "                   and result_date >= to_date(?, 'yyyy-mm-dd')\n" +
                "                   and result_date <= to_date(?, 'yyyy-mm-dd')) a,\n" +
                "               (select 计算项目_id,\n" +
                "                       计算断面号,\n" +
                "                       result_date,\n" +
                "                       to_char(result_date, 'yyyy-mm') as chardate,\n" +
                "                       计算结果_value\n" +
                "                  from sdlm_output_计算结果表\n" +
                "                 where 计算项目_id = ? \n" +
                "                   and step_id = ? \n" +
                "                   and result_date >= to_date(? , 'yyyy-mm-dd')\n" +
                "                   and result_date <= to_date(?, 'yyyy-mm-dd')) b\n" +
                "         where a.计算断面号 = b.计算断面号\n" +
                "           and a.result_date = b.result_date) c\n" +
                " group by 计算断面号, c.chardate, c.距离\n" +
                " order by chardate asc, 距离 asc\n";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String step_id = null;
        List<Map<String, String>> dataList = new ArrayList<Map<String, String>>();
        try {
            Map dataMap = null;
            connection = basicDataSource.getConnection();
            if (StringUtils.isEmpty(start) || StringUtils.isEmpty(end)) {
                ps = connection.prepareStatement(selectYear);
                ps.setString(1, planId);
                rs = ps.executeQuery();
                if (rs.next()) {
                    int year;
                    year = rs.getInt(1);
                     start = year + "-01-01";
                     end = year + "-01-01";
                }
            }

            ps = connection.prepareStatement(selectStep);
            ps.setString(1, planId);
            ps.setString(2, modelId);
            rs = ps.executeQuery();
            if (rs.next()) {
                step_id = rs.getString(1);
            }
            ps = connection.prepareStatement(sql);


            ps.setString(1, juli);
            ps.setString(2, step_id);

            ps.setString(3, start);
            ps.setString(4, end);

            ps.setString(5, jisuanId);
            ps.setString(6, step_id);
            ps.setString(7, start);
            ps.setString(8, end);
            rs = ps.executeQuery();
            String currentMonth = null;
            while (rs.next()) {
                dataMap = new HashMap();
                currentMonth = rs.getString(1);
                dataMap.put("month", currentMonth);
                dataMap.put("distance", getNmuber(rs.getString(2), true, false));
                dataMap.put("value", getNmuber(rs.getString(3), true, false));
                dataList.add(dataMap);
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
        return dataList;
    }

    @Override
    public List<Map<String, String>> waterHDDataByDay(String planId,String modelId, String juli, String jisuanId, String start, String end) throws SQLException {
        String selectStep = "select step_id from sdlm_biz_方案步骤定义 where plan_id = ? and model_code = ?";
        String selectYear = "select 计算年份 from sdlm_biz_方案定义表 where plan_id = ?";
        String sql ="select to_char(a.result_date, 'yyyy-mm-dd') as chardate,a.计算结果_value as 距离,\n" +
                "               b.计算结果_value as 计算值\n" +
                "          from (select 计算项目_id,\n" +
                "                       计算断面号,\n" +
                "                       result_date,\n" +
                "                       计算结果_value\n" +
                "                  from sdlm_output_计算结果表\n" +
                "                 where 计算项目_id = ? \n" +
                "                   and step_id = ? \n" +
                "                   and result_date >= to_date(?, 'yyyy-mm-dd')\n" +
                "                   and result_date <= to_date(?, 'yyyy-mm-dd')) a,\n" +
                "               (select 计算项目_id,\n" +
                "                       计算断面号,\n" +
                "                       result_date,\n" +
                "                       计算结果_value\n" +
                "                  from sdlm_output_计算结果表\n" +
                "                 where 计算项目_id = ? \n" +
                "                   and step_id = ? \n" +
                "                   and result_date >= to_date(? , 'yyyy-mm-dd')\n" +
                "                   and result_date <= to_date(?, 'yyyy-mm-dd')) b\n" +
                "         where a.计算断面号 = b.计算断面号\n" +
                "           and a.result_date = b.result_date order by a.result_date asc,a.计算结果_value asc";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String step_id = null;
        List<Map<String, String>> dataList = new ArrayList<Map<String, String>>();
        try {
            Map dataMap = null;
            connection = basicDataSource.getConnection();
            if (StringUtils.isEmpty(start) || StringUtils.isEmpty(end)) {
                ps = connection.prepareStatement(selectYear);
                ps.setString(1, planId);
                rs = ps.executeQuery();
                if (rs.next()) {
                    int year;
                    year = rs.getInt(1);
                    start = year + "-01-01";
                    end = year + "-01-01";
                }
            }

            ps = connection.prepareStatement(selectStep);
            ps.setString(1, planId);
            ps.setString(2, modelId);
            rs = ps.executeQuery();
            if (rs.next()) {
                step_id = rs.getString(1);
            }
            ps = connection.prepareStatement(sql);


            ps.setString(1, juli);
            ps.setString(2, step_id);

            ps.setString(3, start);
            ps.setString(4, end);

            ps.setString(5, jisuanId);
            ps.setString(6, step_id);
            ps.setString(7, start);
            ps.setString(8, end);
            rs = ps.executeQuery();
            while (rs.next()) {
                dataMap = new HashMap();
                dataMap.put("date", rs.getString(1));
                dataMap.put("distance", getNmuber(rs.getString(2), true, false));
                dataMap.put("value", getNmuber(rs.getString(3), true, false));
                dataList.add(dataMap);
            }
        } catch (SQLException e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
        return dataList;
    }

    @Override
    public Map<String, String> waterDMMetaData(String planId, String dmId) throws SQLException {
        String selectPlan = "select 计算年份,方案名称 from sdlm_biz_方案定义表 where plan_id = ?";
        String selectDmh = "select 断面名称 from sdlm_sys_断面信息表 where obj_id = ?";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String planName = null;
        Map<String, String> map = new HashMap<String, String>();
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(selectPlan);
            ps.setString(1, planId);
            rs = ps.executeQuery();
            if (rs.next()) {
                map.put("year", rs.getInt(1) + "");
                map.put("schemeName", rs.getString(2));
                map.put("schemeCode", planId);
            }
            ps = connection.prepareStatement(selectDmh);
            ps.setString(1, dmId);
            rs = ps.executeQuery();
            if (rs.next()) {
                map.put("sectionName", rs.getString(1));
                map.put("sectionCode", dmId);
            }
            return map;

        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
    }

    @Override
    public Map<String, String> waterHDMetaData(String planId) throws SQLException {
        String selectPlan = "select 计算年份,方案名称,河段名称 from sdlm_biz_方案定义表 where plan_id = ?";
//        String selectDmh = "select 断面名称 from sdlm_sys_断面信息表 where obj_id = ?";
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String planName = null;
        Map<String, String> map = new HashMap<String, String>();
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(selectPlan);
            ps.setString(1, planId);
            rs = ps.executeQuery();
            if (rs.next()) {
                map.put("year", rs.getInt(1) + "");
                map.put("schemeName", rs.getString(2));
                map.put("sectionName", rs.getString(3));
                map.put("sectionCode", rs.getString(3));
                map.put("schemeCode", planId);
            }
            return map;

        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
    }

    @Override
    public List<Map<Integer, String>> getBeisDate(String planId, String fileId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String value = null;
        List<Map<Integer, String>> list = new ArrayList<Map<Integer, String>>();
        String sql = "select t.时间,t.参数_value from sdlm_biz_运算参数表 t where t.参数项目_id= ? and t.方案_id = ? order by t.时间 asc";
        String sql1 = "select 参数项目_id from sdlm_sys_运算参数定义表 where file_id = ?";
        try {
            Map<Integer, String> map = null;
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql1);
            ps.setString(1, fileId);
            rs = ps.executeQuery();
            if (rs.next()) {
                value = rs.getString(1).trim();
            }
            ps = connection.prepareStatement(sql);
            ps.setString(1, value);
            ps.setString(2, planId);
            rs = ps.executeQuery();
            while (rs.next()) {
                map = new HashMap<>();
                map.put(0, rs.getDate(1).toString() + " 00:00:00");
                map.put(1, rs.getString(2));
                list.add(map);
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
        return list;
    }

    @Override
    public List<Map<Integer, String>> getLiuhDate(String planId, String fileId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String value = null;
        List<Map<Integer, String>> list = new ArrayList<Map<Integer, String>>();
        String sql = "select t.时间,t.参数_value from sdlm_biz_运算参数表 t where t.参数项目_id= ? and t.方案_id = ? order by t.时间 asc";
        String sql1 = "select 参数项目_id from sdlm_sys_运算参数定义表 where file_id = ?";
        try {
            Map<Integer, String> map = null;
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql1);
            ps.setString(1, fileId);
            rs = ps.executeQuery();
            if (rs.next()) {
                value = rs.getString(1).trim();
            }
            ps = connection.prepareStatement(sql);
            ps.setString(1, value);
            ps.setString(2, planId);
            rs = ps.executeQuery();
            while (rs.next()) {
                map = new HashMap<>();
                map.put(0, rs.getDate(1).toString() + "       0:00:00");
                map.put(1, rs.getString(2));
                list.add(map);
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
        return list;
    }

    @Override
    public List<Map<Integer, String>> writeTaiZiSC() throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String value = null;
        List<Map<Integer, String>> list = new ArrayList<Map<Integer, String>>();
        String sql = "select 计算断面号,层号,河段级别,距离下游断面的距离,剖面层数个数,河底高程,层高程,层河宽,层面积,层水力半径 from sdlm_sys_断面分层水力参数 t order by 计算断面号 asc,层号 asc";
        try {
            Map<Integer, String> map = null;
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            rs = ps.executeQuery();
            while (rs.next()) {
                map = new HashMap<>();
                map.put(1, rs.getString(1));
                map.put(2, rs.getString(2));
                map.put(3, rs.getString(3));
                map.put(4, getNmuber(rs.getString(4), true, true));
                map.put(5, rs.getString(5));
                map.put(6, getNmuber(rs.getString(6), true, true));
                map.put(7, getNmuber(rs.getString(7), true, true));
                map.put(8, getNmuber(rs.getString(8), true, true));
                map.put(9, getNmuber(rs.getString(9), true, true));
                map.put(10, getNmuber(rs.getString(10), true, true));
                list.add(map);
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
        return list;
    }

    @Override
    public Map<String, String> getExecFile(String fileId) throws Exception {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String sql = "select file_id,file_name from sdlm_sys_参数文件 a,sdlm_biz_方案步骤定义 b where a.model_id = b.model_code and b.plan_id = ?";
        Map<String, String> map = new HashMap<String, String>();
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1, fileId);
            rs = ps.executeQuery();
            while (rs.next()) {
                map.put(rs.getString(2), rs.getString(1));
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
        return map;
    }
    @Override
    public String getZhiliuNames(String plan_id,String key) throws  Exception{
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String names ="";
        String sql = "select value from SDLM_BIZ_方案条件设定表 a left join sdlm_biz_方案步骤定义 b on a.step_id = b.step_id where key = ? and b.plan_id= ?";
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1, key);
            ps.setString(2, plan_id);
            rs = ps.executeQuery();
            if (rs.next()) {
                names = rs.getString(1);
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
        return names;
    }
    @Override
    public List<Map<String, String>> getPlanInfoList(String keyword, String water_bady_id) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String sql = "select a.plan_id,a.方案名称, to_char(a.创建日期, 'yyyy-MM-dd'),a.描述,a.临时状态标识,\n" +
                "       case when b.plan_id is null then 'false' else 'true' end isHave,a.创建人 \n" +
                "        from sdlm_biz_方案定义表 a left join sdlm_biz_方案步骤定义 b on a.plan_id = b.plan_id and b.model_code='04b71ddcb30a454ga9ede57198834243' where "+
                "a.方案名称 like '%"+keyword+"%' and 计算目标_id = ? order by a.创建日期 desc";
        String sql1 = "select a.plan_id,a.方案名称, to_char(a.创建日期, 'yyyy-MM-dd'),a.描述,a.临时状态标识,\n" +
                "       case when b.plan_id is null then 'false' else 'true' end isHave,a.创建人\n" +
                "        from sdlm_biz_方案定义表 a left join sdlm_biz_方案步骤定义 b on a.plan_id = b.plan_id and b.model_code='04b71ddcb30a454ga9ede57198834243' where " +
                "  a.计算目标_id = ? order by a.创建日期 desc";
       List<Map<String, String>> list = new ArrayList<Map<String, String>>();
        Map<String, String> map =null;
        try {
            connection = basicDataSource.getConnection();
            if(StringUtils.isEmpty(keyword)){
                ps = connection.prepareStatement(sql1);
            }else{
                ps = connection.prepareStatement(sql);
            }
            ps.setString(1,water_bady_id);
            rs = ps.executeQuery();
            while (rs.next()) {
                map = new HashMap<String,String>();
                map.put("planId", rs.getString(1));
                map.put("planNm", rs.getString(2));
                map.put("createDate", rs.getString(3));
                map.put("describe", rs.getString(4));
                map.put("status", rs.getString(5));
                map.put("isHave", rs.getString(6));
                map.put("creator", rs.getString(7));
                list.add(map);
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
        return list;
    }

    @Override
    public List<Map<String, String>> getDmInfoList(String planId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String sql = "select obj_id,case when 计算标记 = 1 then '插值' else  '计算' end case, \n" +
                "      经度,纬度,\n" +
                "       断面名称,\n" +
                "       SUPER_OBJ_ID \n" +
                "  from sdlm_sys_断面信息表 t\n" +
                " where 计算目标_id = (select 计算目标_id\n" +
                "                    from sdlm_biz_方案定义表\n" +
                "                   where plan_id = ?) order by 计算断面号 asc";
        List<Map<String, String>> list = new ArrayList<Map<String, String>>();
        Map<String, String> map =null;
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1,planId);
            rs = ps.executeQuery();
            while (rs.next()) {
                map = new HashMap<String,String>();
                map.put("icoFlag", "1");
                map.put("sectionCode", rs.getString(1));
                map.put("type", rs.getString(2));
                map.put("lgtd", rs.getString(3));
                map.put("lttd", rs.getString(4));
                map.put("sectionName", rs.getString(5));
                map.put("superObjId", rs.getString(6));
                map.put("typeIDs", "2,8");
                list.add(map);
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
        return list;
    }
    @Override
    public List<Map<String, String>> getDMWaterPosition(String planId, String dmId, String start, String end) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String selectYear = "select 计算年份 from sdlm_biz_方案定义表 where plan_id = ?";
        String selectStep = "select step_id from sdlm_biz_方案步骤定义 where plan_id = ? and model_code = ?";
        String sql = "select to_char(result_date,'yyyy-MM-dd') as result_date,round(计算结果_value,2)\n" +
                "  from sdlm_output_计算结果表 t\n" +
                " where 计算项目_id = '38d5d1f65bc24641a6010759b949473b'\n" +
                " and step_id = ? " +
                " and result_date >= to_date(?, 'yyyy-mm-dd')\n" +
                " and result_date <= to_date(?, 'yyyy-mm-dd') \n" +
                " and 计算断面号 = (select 计算断面号\n" +
                "                  from sdlm_sys_断面信息表 t\n" +
                "                 where obj_id = ? )\n" +
                "  order by result_date asc";
        List<Map<String, String>> list = new ArrayList<Map<String, String>>();
        Map<String, String> map =null;
        String step_id = null;
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(selectStep);
            ps.setString(1, planId);
            ps.setString(2, "73476806f527419686d18gbca8485689");
            rs = ps.executeQuery();
            if (rs.next()) {
                step_id = rs.getString(1);
            }
            if (StringUtils.isEmpty(start) || StringUtils.isEmpty(end)) {
                ps = connection.prepareStatement(selectYear);
                ps.setString(1, planId);
                rs = ps.executeQuery();
                if (rs.next()) {
                    int year = rs.getInt(1);
                    ps = connection.prepareStatement(sql);
                    ps.setString(1,step_id);
                    ps.setString(2, year + "-01-01");
                    ps.setString(3, year + "-12-31");
                    ps.setString(4,dmId);
                    rs = ps.executeQuery();
                    while (rs.next()) {
                        map = new HashMap<String,String>();
                        map.put("date", rs.getString(1));
                        map.put("altitude", rs.getString(2));
                        list.add(map);
                    }
                }
            }else {
                ps = connection.prepareStatement(sql);
                ps.setString(1,step_id);
                ps.setString(2,start);
                ps.setString(3,end);
                ps.setString(4,dmId);
                rs = ps.executeQuery();
                while (rs.next()) {
                    map = new HashMap<String,String>();
                    map.put("date", rs.getString(1));
                    map.put("altitude", rs.getString(2));
                    list.add(map);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
        return list;
    }
    @Override
    public List<Map<String, String>> getDMElevation(String dmId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String sql = "select 距离,高程,备注 from sdlm_sys_断面剖面数据表 t where obj_id = ? order by 距离 asc";
        List<Map<String, String>> list = new ArrayList<Map<String, String>>();
        List<Map<String, String>> list1 = new ArrayList<Map<String, String>>();
        Map<String, String> map =null;
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1,dmId);
            rs = ps.executeQuery();
            float min = Float.MAX_VALUE;
            float juli = Float.MAX_VALUE;
            while (rs.next()) {
                if(min >= Float.valueOf(rs.getString(2))){
                    min = Float.valueOf(rs.getString(2));
                    juli = Float.valueOf(rs.getString(1));
                }
                map = new HashMap<String,String>();
                map.put("distance", rs.getString(1));
                map.put("altitude", rs.getString(2));
                map.put("note", rs.getString(3));
                list.add(map);
            }
            for(int i=0;i<list.size();i++){
                if(Math.abs(Float.valueOf(list.get(i).get("distance")) - juli) < 300){
                    list1.add(list.get(i));
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
        return list1;
    }

    @Override
    public List<Map<String, String>> getCompareblePlanList(String planId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String sql = "select a.plan_id,a.方案名称,to_char(a.创建日期,'yyyy-MM-dd'),a.描述 from sdlm_biz_方案定义表 a,sdlm_biz_方案定义表 b where a.河段名称 = b.河段名称 and b.plan_id = ? and a.plan_id != ?";
        List<Map<String, String>> list = new ArrayList<Map<String, String>>();
        Map<String, String> map =null;
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1,planId);
            ps.setString(2,planId);
            rs = ps.executeQuery();
            while (rs.next()) {
                map = new HashMap<String,String>();
                map.put("planId", rs.getString(1));
                map.put("planNm", rs.getString(2));
                map.put("createDate", rs.getString(3));
                map.put("describe", rs.getString(4));
                list.add(map);
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
        return list;
    }

    @Override
    public String getJisuanName(String jisuanId) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String name = "";
        String sql = "select 计算项目名称 from sdlm_sys_计算结果项目表 where 模型类型_id = '0' and 计算项目_ID = ?";
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1,jisuanId);
            rs = ps.executeQuery();
            if (rs.next()) {
                name = rs.getString(1);
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
        return name;
    }

    @Override
    public String getStepByPlanId(String plan_id) throws SQLException {
        Connection connection = null;
        PreparedStatement ps = null;
        ResultSet rs = null;
        String step_id = "";
        String sql = "select step_id from sdlm_biz_方案步骤定义 where plan_id = ? and model_code = '73476806f527419686d18gbca8485689'";
        try {
            connection = basicDataSource.getConnection();
            ps = connection.prepareStatement(sql);
            ps.setString(1,plan_id);
            rs = ps.executeQuery();
            if (rs.next()) {
                step_id = rs.getString(1);
            }
        } catch (Exception e) {
            e.printStackTrace();
            throw e;
        } finally {
            if(rs != null)
                rs.close();
            if(ps != null)
                ps.close();
            if(connection != null)
                connection.close();
        }
        return step_id;
    }

}
